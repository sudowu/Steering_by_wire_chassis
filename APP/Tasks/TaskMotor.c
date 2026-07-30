/**
 * @file    TaskMotor.c
 * @brief   电机与底盘控制任务集
 *
 * 本文件包含 5 个 FreeRTOS 任务/定时器回调：
 *   - vTaskMotor         初始化电机外设、底盘、PID，并创建子任务
 *   - vTaskRPM_Get       100Hz 定时器回调，读取左右电机转速
 *   - vTask_ADC_Sample   由 ADC DMA 完成中断驱动，采集电流并滤波
 *   - vTask_Data_Send    串口调试上报（转速/PWM），约 33Hz
 *   - vTask_SpeedControl 100Hz 底盘闭环主循环 + CAN 状态上报
 *
 * 初始化顺序有严格要求：motor_init → Chassis_Init → ADC → 偏置校准 → PID
 * 必须先完成偏置校准再启用电流采样，否则 ADC 读数为无效值。
 */

#include "FreeRTOS.h"
#include "main.h"
#include "TaskMotor.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "adc.h"
#include "Motor.h"
#include "queue.h"
#include "stm32f4xx_hal_gpio.h"
#include "task.h"
#include "TaskSeriel.h"
#include "tim.h"
#include "timers.h"
#include "usart.h"
#include "cmsis_os2.h"
#include "Filter.h"
#include "PID.h"
#include "Chassis.h"
#include "TaskCanInfo.h"

TaskHandle_t g_TaskMotor;
TaskHandle_t g_TaskSpeedControl;
SemaphoreHandle_t xSemaphoreADCReady;
TimerHandle_t xTimerRPMRead;

/* ==================== 电机初始化任务 ==================== */

/**
 * @brief 电机系统初始化任务
 *
 * 执行顺序：
 *   1. motor_init()         — 初始化 Motor_t 结构体、GPIO、PWM
 *   2. Chassis_Init()       — 将 Motor1(右) / Motor2(左) 绑定到底盘实例
 *   3. ADC DMA 启动         — 启动后等待 1s 让 ADC 硬件稳定
 *   4. Motor_OffsetCalibrate — 校准右电机相电流零偏
 *   5. 创建 RPM 定时器并启动霍尔传感器
 *   6. 初始化左右电机速度 PID（Kp=0.2, Ki=0.02, Kd=0.001）
 *   7. 设置加速斜率限制（500 RPM/s，防止阶跃指令冲击）
 *   8. 创建 vTask_SpeedControl 子任务
 *
 * 完成后本任务进入 2000ms 周期的空循环（保活），后续所有实时控制
 * 由 vTask_SpeedControl 和定时器回调驱动。
 */
void vTaskMotor(void* parameter)
{
    motor_init();

    Chassis_Init(&g_chassis, &g_Motor2, &g_Motor1);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Buffer, ADC_BUFFER_SIZE);
    vTaskDelay(1000);
    Motor_OffsetCalibrate(g_chassis.motor_right);

    xTimerRPMRead = xTimerCreate("rpm_get", pdMS_TO_TICKS(10), pdTRUE, NULL, vTaskRPM_Get);
    xTimerStart(xTimerRPMRead, 10);
    hall_start(g_chassis.motor_left);
    hall_start(g_chassis.motor_right);

    Motor_SpeedPID_Init(g_chassis.motor_left,  0.2f, 0.02f, 0.001f);
    Motor_SpeedPID_Init(g_chassis.motor_right, 0.2f, 0.02f, 0.001f);
    Motor_SetMaxAcceleration(g_chassis.motor_left, 500.0f);
    Motor_SetMaxAcceleration(g_chassis.motor_right, 500.0f);

    xTaskCreate(vTask_SpeedControl, "SpeedCtrl", 256, NULL, osPriorityNormal1, &g_TaskSpeedControl);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

/* ==================== 100Hz 转速采集定时器回调 ==================== */

/**
 * @brief 100Hz 软件定时器回调 — 读取左右电机实时转速
 *
 * 转速由霍尔换相计数器差值计算，motor_rpm_get() 内部完成
 * 换相计数 → RPM 的换算。LED0 翻转作为调试用心跳指示。
 */
void vTaskRPM_Get(TimerHandle_t xTimer)
{
    motor_rpm_get(g_chassis.motor_left);
    motor_rpm_get(g_chassis.motor_right);
    // HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
}

/* ==================== ADC 电流采样任务 ==================== */

/**
 * @brief ADC DMA 完成中断驱动的电流采样任务
 *
 * 由 ADC DMA 半传输/全传输完成信号量触发。对 Motor1 三相电流
 * 做中值滤波 + 低通滤波后供过流检测使用。
 *
 * 注意：偏置校准未完成时 (offset_calibrated == 0) 直接跳过采样，
 * 避免用未校准的零点偏移计算出错误的电流值。
 */
void vTask_ADC_Sample(void* parameter)
{
    float total_current = 0;
    float filtered_current = 0;
    // uint8_t erro_sum = 0;
    LowPassFilter current_lpf;
    MedianFilter3 current_median = {0};
    lpf_init(&current_lpf, 0.3f);

    xSemaphoreADCReady = xSemaphoreCreateBinary();
    while (1)
    {
        xSemaphoreTake(xSemaphoreADCReady, portMAX_DELAY);
        if (g_Motor1.adc_current.offset_calibrated == 0)
        {
            continue;
        }
        if (Motor_ReadAdcCurrent(&g_Motor1) == HAL_OK)
        {
            total_current = Motor_GetTotalCurrent(&g_Motor1);
            median_filter_3(&current_median, total_current);
            filtered_current = lpf_update(&current_lpf, total_current);
        }
    }
}

/* ==================== 串口调试数据上报任务 ==================== */

/**
 * @brief 串口调试数据上报任务 (~33Hz)
 *
 * 将左右电机 RPM 和 PWM 占空比格式化为字符串，通过串口队列
 * 发送至上位机调试。使用 pvPortMalloc 动态分配发送缓冲区，
 * 发送失败时释放内存避免泄漏。
 */
void vTask_Data_Send(void* parameter)
{
    while (1)
    {
        uint8_t* info_rpm = pvPortMalloc(100);
        sprintf((char*)info_rpm, "linear:%f,%f\n",
                g_chassis.cmd_linear_vel,
                g_chassis.actual_linear_vel);
        g_Motor1.commutating_counter = 0;
        if (xQueueSendToFront(xQueueSeriel, &info_rpm, 0) != pdPASS)
        {
            vPortFree(info_rpm);
        }

        
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

/* ==================== 100Hz 底盘闭环控制任务 ==================== */

/**
 * @brief 100Hz 底盘闭环控制主循环
 *
 * 每 10ms 执行：
 *   1. Chassis_Control()   — 逆运动学解算 + 速度PID + 超时安全停机
 *   2. CAN_SendChassisStatus() — 上报 RPM/状态/心跳
 *
 * 每 100ms 额外执行一次 CAN_SendChassisDiag()（故障码/PWM/PID误差）。
 */
void vTask_SpeedControl(void* parameter)
{
    const TickType_t xDelayTime = pdMS_TO_TICKS(10);
    uint8_t diag_counter = 0;
    uint8_t led_counter = 0;
    while (1)
    {
        Chassis_Control(&g_chassis);
        CAN_SendChassisStatus(&g_chassis);

        if (++diag_counter >= 10)
        {
            CAN_SendChassisDiag(&g_chassis);
            diag_counter = 0;
        }
        if (++led_counter >= 20) {
            HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
            led_counter = 0;
        }
        vTaskDelay(xDelayTime);
    }
}

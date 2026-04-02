//
// Created by yang on 2026/3/8.
//
#include "FreeRTOS.h"
#include "main.h"
#include "TaskMotor.h"

#include <stdio.h>
#include <string.h>

#include "adc.h"
#include "Motor.h"
#include "queue.h"
#include "task.h"
#include "TaskSeriel.h"
#include "tim.h"
#include "timers.h"
#include "usart.h"
#include "cmsis_os2.h"
#include "Filter.h"

TaskHandle_t g_TaskMotor;
SemaphoreHandle_t xSemaphoreADCReady;
TimerHandle_t xTimerRPMRead;

void vTaskMotor(void* parameter)
{
    motor_init();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Buffer, ADC_BUFFER_SIZE);
    vTaskDelay(1000); // 等待 ADC 稳定
    Motor_OffsetCalibrate(&g_Motor1);

    xTimerRPMRead = xTimerCreate("rpm_read", pdMS_TO_TICKS(100), pdTRUE, NULL, vTaskRPM_Read);
    xTimerStart(xTimerRPMRead, 200);
    hall_start(&g_Motor1);
    hall_start(&g_Motor2);
    motor_start(&g_Motor1, 2, 70);
    motor_start(&g_Motor2, 1, 70);


    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(200)); // 每10ms执行一次，控制频率约为100Hz
    }
}


void vTaskRPM_Read(TimerHandle_t xTimer)
{
    motor_rpm_read(&g_Motor1);
    motor_rpm_read(&g_Motor2);
    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
}

void vTask_ADC_Sample(void* parameter)
{
    float total_current = 0;
    float filtered_current = 0;
    uint8_t erro_sum = 0;
    LowPassFilter current_lpf;
    MedianFilter3 current_median = {0}; // 初始化中值滤波器状态
    lpf_init(&current_lpf, 0.3f); // 设置滤波系数，0.1表示较强的滤波效果，实际应用中可以调整这个

    xSemaphoreADCReady = xSemaphoreCreateBinary();
    while (1)
    {
        xSemaphoreTake(xSemaphoreADCReady, portMAX_DELAY);
        if (g_Motor1.adc_current.offset_calibrated == 0)
        {
            continue; // 偏置未校准，跳过当前值计算
        }
        if (Motor_ReadAdcCurrent(&g_Motor1) == HAL_OK)
        {
            total_current = Motor_GetTotalCurrent(&g_Motor1);
            median_filter_3(&current_median, total_current); // 更新中值滤波器状态
            filtered_current = lpf_update(&current_lpf, total_current); // 更新低通滤波器状态

            // uint8_t* info_current = pvPortMalloc(100);
            //
            // sprintf((char*)info_current, "current:%f,%f,%f,%f,%d,%f,%ld\n",
            //         g_Motor1.adc_current.adc_current_u,
            //         g_Motor1.adc_current.adc_current_v,
            //         g_Motor1.adc_current.adc_current_w,
            //         filtered_current,
            //         erro_sum,
            //         total_current,
            //         HAL_GetTick());
            // if (xQueueSendToFront(xQueueSeriel, &info_current, 1) != pdPASS)
            // {
            //     erro_sum++;
            //     vPortFree(info_current); // 发送失败，释放内存
            // }
            // else
            // {
            //     erro_sum = 0;
            // }
        }
    }
}

void vTask_Data_Send(void* parameter)
{
    int i = 0;
    int j = 0;
    float rpm_ave = 0;
    float rpm_buff[10] = {0};
    while (1)
    {
        rpm_buff[i++] = ((float)(g_Motor1.commutating_counter) * 50);
        i %= 10;
        for (j = 0; j < 10; j++)
        {
            rpm_ave += rpm_buff[j];
        }
        // g_Motor1.rpm = (rpm_ave / 10);
        rpm_ave = 0;
        uint8_t* info_rpm = pvPortMalloc(100);
        sprintf((char*)info_rpm, "rpm:%f,%f,%d\n", g_Motor1.rpm, g_Motor2.rpm, g_Motor1.pwm_duty);
        g_Motor1.commutating_counter = 0;
        if (xQueueSendToFront(xQueueSeriel, &info_rpm, 0) != pdPASS)
        {
            vPortFree(info_rpm); // 发送失败，释放内存
        }

        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/**
 * @file    TaskSbwControl.c
 * @brief   线控底盘 SbW 主控制任务实现
 *
 * 初始化五大功能模块和运动控制状态，以 100Hz 周期执行：
 *   各 Function_Update() → MotionControl_Update() → g_chassis
 *
 * 每周期调用顺序：
 *   1. ParkingFunction_Update   (EPB 驻车指令 → 执行器)
 *   2. Parking_Actuator_Poll     (EPB 状态机推进)
 *   3. GearFunction_Update       (档位切换 + 安全互锁)
 *   4. Gear_Actuator_Poll        (换档状态机推进)
 *   5. BrakingFunction_Update    (制动指令校验 + 制动灯 + 接管检测)
 *   6. MotionControl_Update      (五层仲裁 → v, ω)
 *   7. 写入 g_chassis
 */

#include "TaskSbwControl.h"

#include "FreeRTOS.h"
#include "main.h"
#include "task.h"

#include <math.h>

#include "Chassis.h"
#include "MotionControl.h"
#include "SteeringFunction.h"
#include "DriveFunction.h"
#include "BrakingFunction.h"
#include "GearFunction.h"
#include "ParkingFunction.h"

/* ---- 全局实例 ---- */
Chassis_Function Chassis_Function_instance = {0};

/* ---- 模块级状态 ---- */
static MotionControl_State_t mc_state;
static Parking_Actuator_t   epb_actuator;    // EPB 执行器
static Gear_Actuator_t      gear_actuator;   // 档位执行器

/**
 * @brief 检查是否有 SbW 功能模块处于使能状态
 */
static uint8_t SbwIsActive(const Chassis_Function* cf)
{
    return cf->Drive_Function.Drive_Control.Driving_Config_Enable     ||
           cf->Steering_Function.Steering_Control.Steering_Config_Enable ||
           cf->Braking_Function.Braking_Control.Braking_Config_Enable;
}

/**
 * @brief 初始化全部 SbW 子系统
 */
static void SbwInitAll(void)
{
    /* ---- Function 层初始化 ---- */
    SteeringFunction_Init(&Chassis_Function_instance.Steering_Function);
    DriveFunction_Init(&Chassis_Function_instance.Drive_Function);
    BrakingFunction_Init(&Chassis_Function_instance.Braking_Function);
    GearFunction_Init(&Chassis_Function_instance.Gear_Function);
    ParkingFunction_Init(&Chassis_Function_instance.Parking_Function);

    /* ---- 执行器初始化 ---- */
    Parking_Actuator_Init(&epb_actuator,
                          EPB_LEFT_PORT,  EPB_LEFT_PIN,
                          EPB_RIGHT_PORT, EPB_RIGHT_PIN);

    Gear_Actuator_Init(&gear_actuator,
                       GEAR_POS0_PORT, GEAR_POS0_PIN,
                       GEAR_POS1_PORT, GEAR_POS1_PIN);

    /* Gear_Feedback 同步执行器的初始 P 档状态 */
    Chassis_Function_instance.Gear_Function.Gear_Feedback.Gear_Position_Status = P;

    /* ---- 运动控制初始化 ---- */
    MotionControl_Init(&mc_state);

    Chassis_Function_instance.Current_Mode      = manual;
    Chassis_Function_instance.Fault_Summary     = 0;
    Chassis_Function_instance.Last_Command_Tick = HAL_GetTick();
}

/**
 * @brief SbW 主控制任务 (100Hz)
 */
void vTaskSbwControl(void* parameter)
{
    (void)parameter;

    /* ---- 初始化 ---- */
    SbwInitAll();

    /* 等待底盘初始化完成（Motor task 负责 Chassis_Init + 电机校准）*/
    while (!g_chassis.initialized)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    /* 100Hz 定时基准 */
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(10);

    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        /* ================================================================
         * Step 1: 驻车 — EPB 指令执行 + 状态机推进
         * ================================================================ */
        ParkingFunction_Update(&Chassis_Function_instance.Parking_Function,
                               &epb_actuator);
        Parking_Actuator_Poll(&epb_actuator);

        /* ================================================================
         * Step 2: 档位 — 换档指令 + 安全互锁 + 状态机推进
         * ================================================================ */
        float current_speed = fabsf(g_chassis.actual_linear_vel);

        GearFunction_Update(&Chassis_Function_instance.Gear_Function,
                            &gear_actuator, current_speed);
        Gear_Actuator_Poll(&gear_actuator);

        /* ================================================================
         * Step 3: 制动 — 指令校验 + 制动灯 + 人工接管检测
         *
         * 无独立制动器硬件：行车制动由 MotionControl Layer 3
         * 读取 Braking_Control.Target_Deceleration 以电机减速实现。
         * ================================================================ */
        BrakingFunction_Update(&Chassis_Function_instance.Braking_Function);

        /* ================================================================
         * Step 4: SbW 未使能 → 保留 CAN 0x100 直接控制
         * ================================================================ */
        if (!SbwIsActive(&Chassis_Function_instance))
        {
            continue;
        }

        Chassis_Function_instance.Last_Command_Tick = HAL_GetTick();

        /* ================================================================
         * Step 5: 运动控制 — 五层仲裁 → (v, ω)
         * ================================================================ */
        float v_target = 0.0f;
        float w_target = 0.0f;

        MotionControl_Update(&Chassis_Function_instance,
                             &mc_state,
                             &v_target, &w_target);

        /* ================================================================
         * Step 6: 写入底盘
         * ================================================================ */
        g_chassis.cmd_linear_vel  = v_target;
        g_chassis.cmd_angular_vel = w_target;
        g_chassis.cmd_enable      = (SbwIsActive(&Chassis_Function_instance) &&
                                     !MotionControl_IsEmergencyStop(&mc_state)) ? 1 : 0;
        g_chassis.cmd_timestamp   = HAL_GetTick();

        /* TODO: 发送 SbW 状态反馈帧 */
    }
}

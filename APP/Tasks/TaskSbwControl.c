/**
 * @file    TaskSbwControl.c
 * @brief   线控底盘 SbW 主控制任务实现
 *
 * 初始化五大功能模块和运动控制状态，以 100Hz 周期执行：
 *   Chassis_Function_instance → MotionControl_Update() → g_chassis
 */

#include "TaskSbwControl.h"

#include "FreeRTOS.h"
#include "main.h"
#include "task.h"

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
static uint8_t sbw_initialized = 0;

/**
 * @brief 检查是否有 SbW 模块处于使能状态
 *
 * 任一功能模块使能即认为 SbW 接管底盘控制权。
 * SbW 未使能时保留 g_chassis 原值，兼容直接 CAN 0x100 控制。
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
    SteeringFunction_Init(&Chassis_Function_instance.Steering_Function);
    DriveFunction_Init(&Chassis_Function_instance.Drive_Function);
    BrakingFunction_Init(&Chassis_Function_instance.Braking_Function);
    GearFunction_Init(&Chassis_Function_instance.Gear_Function);
    ParkingFunction_Init(&Chassis_Function_instance.Parking_Function);

    MotionControl_Init(&mc_state);

    Chassis_Function_instance.Current_Mode     = automatic;
    Chassis_Function_instance.Fault_Summary    = 0;
    Chassis_Function_instance.Last_Command_Tick = HAL_GetTick();

    sbw_initialized = 1;
}

/**
 * @brief SbW 主控制任务 (100Hz)
 */
void vTaskSbwControl(void* parameter)
{
    (void)parameter;

    /* ---- 初始化阶段 ---- */
    SbwInitAll();

    /* 等待底盘初始化完成（Motor task 负责 Chassis_Init + 电机校准）*/
    while (!g_chassis.initialized)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    /* 设置初始档位为 D 档（允许运动，实际应由上位机设置）*/
    Chassis_Function_instance.Gear_Function.Gear_Feedback.Gear_Position_Status = D;

    /* 精确 100Hz 定时基准 */
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(10);  // 10ms = 100Hz

    for (;;)
    {
        vTaskDelayUntil(&xLastWakeTime, xPeriod);

        /* 检查 SbW 是否接管控制权 */
        if (!SbwIsActive(&Chassis_Function_instance))
        {
            /* SbW 未使能 → 不修改 g_chassis，保留 CAN 0x100 直接控制 */
            continue;
        }

        /* 更新全局时间戳 */
        Chassis_Function_instance.Last_Command_Tick = HAL_GetTick();

        /* 核心控制：五层仲裁 → (v, ω) */
        float v_target = 0.0f;
        float w_target = 0.0f;

        MotionControl_Update(&Chassis_Function_instance,
                             &mc_state,
                             &v_target, &w_target);

        /* 将处理后的运动学目标写入底盘控制结构 */
        g_chassis.cmd_linear_vel  = v_target;
        g_chassis.cmd_angular_vel = w_target;
        g_chassis.cmd_enable      = (SbwIsActive(&Chassis_Function_instance) &&
                                     !MotionControl_IsEmergencyStop(&mc_state)) ? 1 : 0;
        g_chassis.cmd_timestamp   = HAL_GetTick();

        /* TODO: 发送 SbW 状态反馈帧 (CAN ID 0x120-0x124)
         *   - 各模块 Feedback 字段回传
         *   - 当前驾驶模式
         *   - 故障汇总码
         */
    }
}

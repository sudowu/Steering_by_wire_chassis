/**
 * @file    TaskSbwControl.c
 * @brief   线控底盘 SbW 主控制任务实现
 *
 * 管理两个 Chassis_Function 实例：
 *   g_chassis_auto   — 自动驾驶实例（接收上层 CAN 指令）
 *   g_chassis_manual — 人工驾驶实例（接收物理输入 CAN 帧）
 *
 * 每周期调用顺序（100Hz）：
 *   1. Parking_Actuator_Poll       (EPB 硬件状态机推进)
 *   2. Gear_Actuator_Poll          (档位硬件状态机推进)
 *   3. BrakingFunction_Update      (manual 实例：制动踏板接管检测)
 *   4. DriveFunction_Update        (manual 实例：加速踏板接管检测)
 *   5. DrivingModeFunction_Update  (汇总接管 → 仲裁全局驾驶模式)
 *   6. 模式分支：
 *        automatic    → Gear + Parking + MC 使用 auto 实例
 *        manual       → Gear + Parking + MC 使用 manual 实例
 *        interrupted  → 仅 MC 紧急制动
 *   7. P 档 EPB 夹紧 / 非 P 档释放
 *   8. MotionControl_Update → v, ω
 *   9. 写入 g_chassis + 反馈同步
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
#include "DrivingModeFunction.h"

/* ---- 全局实例 ---- */
Chassis_Function g_chassis_auto   = {0};   // 自动驾驶实例（接收上层 CAN 指令）
Chassis_Function g_chassis_manual = {0};   // 人工驾驶实例（接收物理输入 CAN 帧）

/* ---- 模块级状态 ---- */
static MotionControl_State_t      mc_state;
static Parking_Actuator_t         epb_actuator;    // EPB 执行器（共享硬件）
static Gear_Actuator_t            gear_actuator;   // 档位执行器（共享硬件）
static DrivingModeFunction_State_t dm_state;       // 驾驶模式状态机

/**
 * @brief 检查指定实例是否有 SbW 功能模块处于使能状态
 */
static uint8_t SbwIsActive(const Chassis_Function* cf)
{
    return cf->Drive_Function.Drive_Control.Driving_Config_Enable         ||
           cf->Steering_Function.Steering_Control.Steering_Config_Enable ||
           cf->Braking_Function.Braking_Control.Braking_Config_Enable;
}

/**
 * @brief P 档 EPB 联动：P → 夹紧，非 P → 释放
 *
 * 根据实际档位强制执行 EPB 动作，确保安全约束。
 */
static void SbwUpdateEPB(Parking_Actuator_t* epb, Gear_Position gear)
{
    if (gear == P)
    {
        if (!Parking_Actuator_IsClamped(epb))
        {
            Parking_Actuator_Clamp(epb);
        }
    }
    else
    {
        if (!Parking_Actuator_IsReleased(epb))
        {
            Parking_Actuator_Release(epb);
        }
    }
}

/**
 * @brief 初始化全部 SbW 子系统（双实例）
 */
static void SbwInitAll(void)
{
    /* ---- Function 层初始化：auto 实例 ---- */
    SteeringFunction_Init(&g_chassis_auto.Steering_Function);
    DriveFunction_Init(&g_chassis_auto.Drive_Function);
    BrakingFunction_Init(&g_chassis_auto.Braking_Function);
    GearFunction_Init(&g_chassis_auto.Gear_Function);
    ParkingFunction_Init(&g_chassis_auto.Parking_Function);

    /* ---- Function 层初始化：manual 实例 ---- */
    SteeringFunction_Init(&g_chassis_manual.Steering_Function);
    DriveFunction_Init(&g_chassis_manual.Drive_Function);
    BrakingFunction_Init(&g_chassis_manual.Braking_Function);
    GearFunction_Init(&g_chassis_manual.Gear_Function);
    ParkingFunction_Init(&g_chassis_manual.Parking_Function);

    /* ---- 执行器初始化（共享硬件，只初始化一次）---- */
    Parking_Actuator_Init(&epb_actuator,
                          EPB_LEFT_PORT,  EPB_LEFT_PIN,
                          EPB_RIGHT_PORT, EPB_RIGHT_PIN);

    Gear_Actuator_Init(&gear_actuator,
                       GEAR_POS0_PORT, GEAR_POS0_PIN,
                       GEAR_POS1_PORT, GEAR_POS1_PIN);

    /* Gear_Feedback 同步执行器的初始 P 档状态（两个实例一致）*/
    g_chassis_auto.Gear_Function.Gear_Feedback.Gear_Position_Status   = P;
    g_chassis_manual.Gear_Function.Gear_Feedback.Gear_Position_Status = P;

    /* ---- 运动控制初始化 ---- */
    MotionControl_Init(&mc_state);

    /* ---- 驾驶模式初始化（manual 为安全默认，等待条件满足后自动进入 auto）---- */
    DrivingModeFunction_Init(&dm_state);

    /* 初始模式同步到两个实例 */
    g_chassis_auto.Current_Mode   = dm_state.current_mode;
    g_chassis_manual.Current_Mode = dm_state.current_mode;

    g_chassis_auto.Fault_Summary     = 0;
    g_chassis_auto.Last_Command_Tick = HAL_GetTick();
    g_chassis_manual.Fault_Summary     = 0;
    g_chassis_manual.Last_Command_Tick = HAL_GetTick();
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

        float current_speed = fabsf(g_chassis.actual_linear_vel);

        /* ================================================================
         * Step 1: 硬件执行器推进（共享硬件，跑一次）
         * ================================================================ */
        Parking_Actuator_Poll(&epb_actuator);
        Gear_Actuator_Poll(&gear_actuator);

        /* ================================================================
         * Step 2: manual 实例模块更新（接管检测）
         *
         * 始终更新 manual 实例的三个模块，检测人工接管信号：
         *   - Braking:  制动踏板位置 → Manual_Takeover
         *   - Drive:    加速踏板位置 → Manual_Takeover
         *   - Steering: 物理方向盘数据由 CAN 直接写入，无独立 Update
         * ================================================================ */
        BrakingFunction_Update(&g_chassis_manual.Braking_Function);
        DriveFunction_Update(&g_chassis_manual.Drive_Function);

        /* ================================================================
         * Step 3: 驾驶模式仲裁
         *
         * DrivingModeFunction 汇总 manual 实例的接管信号 + 档位 mismatch +
         * auto 实例的 Over_Threshold → 仲裁全局模式。
         * ================================================================ */
        DrivingModeFunction_Update(&dm_state,
                                    &g_chassis_auto,
                                    &g_chassis_manual,
                                    current_speed);

        driving_mode current_mode = dm_state.current_mode;

        /* ================================================================
         * Step 4: 模式分支执行
         * ================================================================ */
        if (current_mode == automatic)
        {
            /* ---- auto 模式：执行 auto 实例的档位/驻车指令 ---- */

            /* 档位（auto 换档逻辑：P/R 等停，N/D 不等停）*/
            GearFunction_Update(&g_chassis_auto.Gear_Function,
                                &gear_actuator,
                                current_speed,
                                1);  // is_auto_mode = 1

            /* 驻车（auto 实例的 Parking_Request）*/
            ParkingFunction_Update(&g_chassis_auto.Parking_Function,
                                   &epb_actuator);
        }
        else if (current_mode == manual)
        {
            /* ---- manual 模式：执行 manual 实例的档位/驻车指令 ---- */

            /* 档位（manual 换档逻辑：全部等停车）*/
            GearFunction_Update(&g_chassis_manual.Gear_Function,
                                &gear_actuator,
                                current_speed,
                                0);  // is_auto_mode = 0

            /* 驻车（manual 实例的 Parking_Request）*/
            ParkingFunction_Update(&g_chassis_manual.Parking_Function,
                                   &epb_actuator);
        }
        /* interrupted 模式不做档位/驻车操作，直接走紧急制动 */

        /* ================================================================
         * Step 5: 档位反馈 → 同步执行器状态到两个实例
         *
         * 无论哪种模式，只有一个物理档位执行器，反馈应反映实际状态。
         * ================================================================ */
        {
            Gear_Position actual_gear = gear_actuator.current;
            g_chassis_auto.Gear_Function.Gear_Feedback.Gear_Position_Status   = actual_gear;
            g_chassis_manual.Gear_Function.Gear_Feedback.Gear_Position_Status = actual_gear;
        }

        /* ================================================================
         * Step 6: P 档 EPB 联动（以实际档位为准）
         *
         * P 档 → 强制夹紧 EPB
         * 非 P → 强制释放 EPB
         *
         * 此逻辑覆盖 ParkingFunction_Update 的 Parking_Request，
         * 确保档位与 EPB 状态一致的硬件级安全约束。
         * ================================================================ */
        SbwUpdateEPB(&epb_actuator, gear_actuator.current);

        /* ================================================================
         * Step 7: 选择活跃实例 + SbW 使能判断
         * ================================================================ */
        Chassis_Function* active = (current_mode == automatic)
                                   ? &g_chassis_auto : &g_chassis_manual;

        /* 同步当前模式到活跃实例 */
        active->Current_Mode = current_mode;

        /* SbW 未使能 → 跳过 MC，保留 CAN 0x100 调试直控 */
        if (!SbwIsActive(active))
        {
            continue;
        }

        active->Last_Command_Tick = HAL_GetTick();

        /* ================================================================
         * Step 8: 运动控制 — 五层仲裁 → (v, ω)
         *
         * auto 和 manual 走同一 MotionControl 管线：
         *   - auto:       五层正常输出
         *   - manual:     五层正常输出（不含接管检测）
         *   - interrupted: MC_SafetyGate 拦截 → emergency_stop
         * ================================================================ */
        float v_target = 0.0f;
        float w_target = 0.0f;

        MotionControl_Update(active,
                             &mc_state,
                             &v_target, &w_target);

        /* ================================================================
         * Step 9: 写入底盘
         * ================================================================ */
        g_chassis.cmd_linear_vel  = v_target;
        g_chassis.cmd_angular_vel = w_target;
        g_chassis.cmd_enable      = (SbwIsActive(active) &&
                                      !MotionControl_IsEmergencyStop(&mc_state)) ? 1 : 0;
        g_chassis.cmd_timestamp   = HAL_GetTick();

        /* ================================================================
         * Step 10: 反馈同步 — 以实际物理状态为准
         *
         * 底盘实际速度来自正运动学（Chassis_Control 每周期更新），
         * 同步到两个实例的反馈字段供 CAN 状态帧上报使用。
         * ================================================================ */
        {
            float actual_v_ms = fabsf(g_chassis.actual_linear_vel);

            /* 实际车速 (0.1 km/h per LSB) */
            uint8_t speed_fb = (uint8_t)(actual_v_ms * 3.6f * 10.0f);

            g_chassis_auto.Drive_Function.Drive_Feedback.Vehicle_Speed   = speed_fb;
            g_chassis_manual.Drive_Function.Drive_Feedback.Vehicle_Speed = speed_fb;

            /* EPB 状态同步 */
            uint8_t epb_clamped = Parking_Actuator_IsClamped(&epb_actuator);
            g_chassis_auto.Parking_Function.Parking_Feedback.EPB_Switch_status       = epb_clamped;
            g_chassis_auto.Parking_Function.Parking_Feedback.Parking_status_feedback  = epb_clamped;
            g_chassis_manual.Parking_Function.Parking_Feedback.EPB_Switch_status      = epb_clamped;
            g_chassis_manual.Parking_Function.Parking_Feedback.Parking_status_feedback = epb_clamped;
        }

        /* TODO: 发送 SbW 状态反馈帧（CAN ID 待定，以实际物理状态为准）*/
    }
}

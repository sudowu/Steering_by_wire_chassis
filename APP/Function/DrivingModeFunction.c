/**
 * @file    DrivingModeFunction.c
 * @brief   驾驶模式控制模块实现
 *
 * 三态状态机：automatic ↔ interrupted ↔ manual
 *
 *   automatic   — 正常自动驾驶，MotionControl 五层仲裁输出
 *   interrupted — 人工接管触发，紧急制动中，禁止恢复到 automatic
 *   manual      — 车辆已安全停机，等待人工驾驶或自动恢复
 *
 * 接管信号来源：
 *   g_chassis_manual（物理输入）— 刹车踏板 / 加速踏板 / 方向盘扭矩 / 档位 mismatch
 *   g_chassis_auto   （指令越界）— Over_Threshold
 *
 * 进入自动模式条件：
 *   - 物理档位 == D
 *   - Fault_Summary == 0
 *   - 无接管信号
 */

#include "DrivingModeFunction.h"
#include "main.h"

/* 安全停机速度阈值 (m/s)，低于此值视为"已停车" */
#define MODE_SAFE_STOP_THRESHOLD     0.05f

/* 安全停机后保持时间 (ms)，防止抖动 */
#define MODE_HOLD_TIME_MS            500

/**
 * @brief 检查是否存在任意接管或越界信号
 *
 * 接管（来自 manual 实例的物理输入）：
 *   - 刹车踏板 Manual_Takeover
 *   - 加速踏板 Manual_Takeover
 *   - 方向盘扭矩 Manual_Takeover
 *
 * 越界（来自 auto 实例的指令校验）：
 *   - 任一模块 Over_Threshold
 *
 * @param cf_auto     自动驾驶实例
 * @param cf_manual   人工驾驶实例
 * @return 1=存在接管/越界，0=无
 */
static uint8_t DriveMode_HasTakeover(const Chassis_Function* cf_auto,
                                      const Chassis_Function* cf_manual)
{
    /* 物理踏板接管（来自 manual 实例） */
    if (cf_manual->Braking_Function.Manual_Takeover ||
        cf_manual->Drive_Function.Manual_Takeover   ||
        cf_manual->Steering_Function.Manual_Takeover)
    {
        return 1;
    }

    /* 越界信号（来自 auto 实例的指令校验） */
    if (cf_auto->Steering_Function.Over_Threshold ||
        cf_auto->Drive_Function.Over_Threshold    ||
        cf_auto->Braking_Function.Over_Threshold)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief 检测物理档位是否不在 D 挡
 *
 * 当自动模式使能且物理档位 != D 时，视为人工接管。
 * 只有 D 挡允许自动驾驶，R/N/P 均触发接管。
 *
 * @param cf_auto     自动驾驶实例（读取 Gear_Control.Gear_Config_Enable）
 * @param cf_manual   人工驾驶实例（读取 Gear_Feedback.Gear_Position_Status）
 * @return 1=非 D 挡（触发接管），0=D 挡或 auto 档位未使能
 */
static uint8_t DriveMode_GearMismatch(const Chassis_Function* cf_auto,
                                       const Chassis_Function* cf_manual)
{
    /* auto 档位未使能 → 跳过检测 */
    if (!cf_auto->Gear_Function.Gear_Control.Gear_Config_Enable)
    {
        return 0;
    }

    Gear_Position phys_gear = cf_manual->Gear_Function.Gear_Feedback.Gear_Position_Status;

    return (phys_gear != D) ? 1 : 0;
}

/**
 * @brief 检查是否满足进入自动驾驶模式的条件
 *
 *   - 物理档位 == D（非 D 档不允许自动驾驶）
 *   - 无全局故障
 *
 * @param cf_manual 人工驾驶实例
 * @return 1=可以进入，0=条件不满足
 */
static uint8_t DriveMode_CanEnterAuto(const Chassis_Function* cf_manual)
{
    if (cf_manual->Gear_Function.Gear_Feedback.Gear_Position_Status != D)
    {
        return 0;
    }
    if (cf_manual->Fault_Summary != 0)
    {
        return 0;
    }
    return 1;
}

/**
 * @brief 同步全局驾驶模式到两个实例的各模块反馈
 */
static void DriveMode_SyncToModules(Chassis_Function* cf, driving_mode mode)
{
    cf->Steering_Function.Steering_Feedback.Steering_mode = mode;
    cf->Drive_Function.Drive_Feedback.Driving_mode        = mode;
    cf->Braking_Function.Braking_Feedback.Current_Braking_System_Driving_Mode = mode;

    cf->Current_Mode = mode;
}

/* ================================================================
 * 公开接口
 * ================================================================ */

void DrivingModeFunction_Init(DrivingModeFunction_State_t* state)
{
    /* 安全默认：启动时处于 manual 模式，等待条件满足后自动进入 automatic */
    state->current_mode     = manual;
    state->reenable_request = 0;
    state->mode_entry_tick  = HAL_GetTick();
    state->takeover_prev    = 0;
}

driving_mode DrivingModeFunction_Update(DrivingModeFunction_State_t* state,
                                         Chassis_Function* cf_auto,
                                         Chassis_Function* cf_manual,
                                         float vehicle_speed_mps)
{
    uint8_t takeover      = DriveMode_HasTakeover(cf_auto, cf_manual);
    uint8_t gear_mismatch = DriveMode_GearMismatch(cf_auto, cf_manual);

    /* 合并接管信号：物理输入接管 OR 档位 mismatch */
    uint8_t takeover_effective = takeover || gear_mismatch;

    /* 接管信号上升沿检测（用于日志/调试，当前不输出） */
    uint8_t takeover_rising = (takeover_effective && !state->takeover_prev);
    (void)takeover_rising;

    state->takeover_prev = takeover_effective;

    uint32_t now          = HAL_GetTick();
    uint32_t mode_elapsed = now - state->mode_entry_tick;

    switch (state->current_mode)
    {
    /* ================================================================
     * automatic — 自动驾驶中
     * ================================================================ */
    case automatic:
        if (takeover_effective)
        {
            state->current_mode    = interrupted;
            state->mode_entry_tick = now;
            DriveMode_SyncToModules(cf_auto,   state->current_mode);
            DriveMode_SyncToModules(cf_manual, state->current_mode);
            return state->current_mode;
        }
        /* 无接管 → 保持 automatic */
        DriveMode_SyncToModules(cf_auto,   state->current_mode);
        DriveMode_SyncToModules(cf_manual, state->current_mode);
        return state->current_mode;

    /* ================================================================
     * interrupted — 接管触发，紧急制动中
     * → 车辆安全停机后自动进入 manual
     * ================================================================ */
    case interrupted:
        if (vehicle_speed_mps < MODE_SAFE_STOP_THRESHOLD &&
            mode_elapsed > MODE_HOLD_TIME_MS)
        {
            state->current_mode    = manual;
            state->mode_entry_tick = now;
            DriveMode_SyncToModules(cf_auto,   state->current_mode);
            DriveMode_SyncToModules(cf_manual, state->current_mode);
            return state->current_mode;
        }
        /* 仍在减速中 → 保持 interrupted */
        DriveMode_SyncToModules(cf_auto,   state->current_mode);
        DriveMode_SyncToModules(cf_manual, state->current_mode);
        return state->current_mode;

    /* ================================================================
     * manual — 人工驾驶
     * → 物理 D 档 + 无故障 + 无接管 → 自动进入 automatic
     * ================================================================ */
    case manual:
        if (!takeover_effective && DriveMode_CanEnterAuto(cf_manual))
        {
            state->current_mode     = automatic;
            state->reenable_request  = 0;
            state->mode_entry_tick  = now;
            DriveMode_SyncToModules(cf_auto,   state->current_mode);
            DriveMode_SyncToModules(cf_manual, state->current_mode);
            return state->current_mode;
        }
        /* 条件不满足 → 保持 manual */
        DriveMode_SyncToModules(cf_auto,   state->current_mode);
        DriveMode_SyncToModules(cf_manual, state->current_mode);
        return state->current_mode;

    default:
        /* 未知状态 → 安全回退到 interrupted */
        state->current_mode    = interrupted;
        state->mode_entry_tick = now;
        DriveMode_SyncToModules(cf_auto,   state->current_mode);
        DriveMode_SyncToModules(cf_manual, state->current_mode);
        return state->current_mode;
    }
}

void DrivingModeFunction_RequestReenable(DrivingModeFunction_State_t* state)
{
    state->reenable_request = 1;
}

/**
 * @brief 清除单个实例的接管和越界信号
 */
static void DriveMode_ClearTakeover(Chassis_Function* cf)
{
    cf->Steering_Function.Manual_Takeover = 0;
    cf->Steering_Function.Over_Threshold  = 0;
    cf->Drive_Function.Manual_Takeover    = 0;
    cf->Drive_Function.Over_Threshold     = 0;
    cf->Braking_Function.Manual_Takeover  = 0;
    cf->Braking_Function.Over_Threshold   = 0;
}

void DrivingModeFunction_SetAutomatic(DrivingModeFunction_State_t* state,
                                       Chassis_Function* cf_auto,
                                       Chassis_Function* cf_manual)
{
    DriveMode_ClearTakeover(cf_auto);
    DriveMode_ClearTakeover(cf_manual);

    state->current_mode     = automatic;
    state->reenable_request  = 0;
    state->mode_entry_tick  = HAL_GetTick();
    state->takeover_prev    = 0;

    DriveMode_SyncToModules(cf_auto,   state->current_mode);
    DriveMode_SyncToModules(cf_manual, state->current_mode);
}

void DrivingModeFunction_SetManual(DrivingModeFunction_State_t* state,
                                    Chassis_Function* cf_auto,
                                    Chassis_Function* cf_manual)
{
    DriveMode_ClearTakeover(cf_auto);
    DriveMode_ClearTakeover(cf_manual);

    state->current_mode     = manual;
    state->reenable_request  = 0;
    state->mode_entry_tick  = HAL_GetTick();
    state->takeover_prev    = 0;

    DriveMode_SyncToModules(cf_auto,   state->current_mode);
    DriveMode_SyncToModules(cf_manual, state->current_mode);
}

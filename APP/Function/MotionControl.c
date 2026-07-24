/**
 * @file    MotionControl.c
 * @brief   运动控制顶层调度实现
 *
 * 五层优先级控制架构：
 *
 *   Layer 0 — 驾驶模式仲裁
 *     automatic:   正常执行上位机指令
 *     interrupted: 人工接管触发，执行安全停机
 *     manual:      人工驾驶模式（当前预留）
 *
 *   Layer 1 — 安全门（硬件级安全约束，不可绕过）
 *     Parking EPB 夹紧 → v=0, ω=0
 *     Gear = P / N    → v=0, ω=0
 *     Gear = R        → 线速度方向取反
 *     故障存在        → 紧急停车
 *
 *   Layer 2 — 驾驶意图 → 运动学目标
 *     Drive → 线速度 v_cmd（加速度→积分→速度）
 *     Steering → 曲率 κ → 角速度 ω_cmd = v × κ
 *
 *   Layer 3 — 制动仲裁
 *     制动激活时：制动优先于驱动，按减速度斜坡降速直到归零
 *     制动释放后：允许驱动正常控制
 *
 *   Layer 4 — 约束与平滑
 *     加速度/减速度限幅
 *     角加速度限幅
 *     侧向加速度安全约束 (v²·κ ≤ a_lat_max)
 *     速度上下限
 */

#include "MotionControl.h"

#include <math.h>
#include <string.h>

#include "main.h"

/* ================================================================
 * 内部辅助宏
 * ================================================================ */

#define MC_CLAMP(val, lo, hi)  ((val) < (lo) ? (lo) : ((val) > (hi) ? (hi) : (val)))
#define MC_SIGN(x)             ((x) >= 0.0f ? 1.0f : -1.0f)

/* ================================================================
 * 内部函数声明（static，仅本文件可见）
 * ================================================================ */

/** Layer 0: 遍历五个模块的人工接管标志，仲裁全局驾驶模式 */
static driving_mode MC_ArbitrateMode(const Chassis_Function* cf);

/** Layer 1: 安全门检查。返回 0=禁止运动（执行停机），1=允许通过 */
static uint8_t MC_SafetyGate(const Chassis_Function* cf,
                             MotionControl_State_t* state,
                             float* out_dir_sign);

/** Layer 2a: Drive → 线速度目标 (m/s) */
static float MC_ResolveVelocity(const Chassis_Function* cf,
                                const MotionControl_State_t* state,
                                float dt);

/** Layer 2b: Steering → 角速度目标 (rad/s)，v_signed 为带方向符号的线速度 */
static float MC_ResolveAngularVelocity(const Chassis_Function* cf,
                                       float v_signed);

/** Layer 3: 制动-驱动仲裁，返回仲裁后的线速度 */
static float MC_BrakingArbitrate(const Chassis_Function* cf,
                                 MotionControl_State_t* state,
                                 float v_cmd, float dt);

/** Layer 4: 施加全部运动学约束与平滑 */
static void MC_ApplyLimits(const MotionControl_State_t* state,
                           float* v, float* w, float dt);

/* ================================================================
 * 公开接口
 * ================================================================ */

void MotionControl_Init(MotionControl_State_t* state)
{
    memset(state, 0, sizeof(MotionControl_State_t));
    state->last_update_tick = HAL_GetTick();
}

void MotionControl_Update(const Chassis_Function* cf,
                          MotionControl_State_t* state,
                          float* out_v, float* out_w)
{
    /* ---- 计算控制周期 dt ---- */
    uint32_t now = HAL_GetTick();
    float dt = (float)(now - state->last_update_tick) / 1000.0f;
    state->last_update_tick = now;

    /* 首周期或 tick 回绕保护 */
    if (dt <= 0.0f || dt > 0.2f)
    {
        dt = (float)MC_CONTROL_PERIOD_MS / 1000.0f;
    }

    /* ================================================================
     * Layer 0: 驾驶模式仲裁
     * ================================================================ */
    (void)MC_ArbitrateMode(cf);

    /* ================================================================
     * Layer 1: 安全门
     * ================================================================ */
    float dir_sign = 1.0f;
    if (!MC_SafetyGate(cf, state, &dir_sign))
    {
        /* 安全门关闭 → 根据是否紧急停车选择减速度 */
        float decel = state->emergency_stop
                      ? MC_EMERGENCY_STOP_DECEL
                      : MC_MAX_LINEAR_DECEL;
        /*dv速度差值 */              
        float dv = decel * dt;

        if (fabsf(state->current_v_cmd) < dv)
        {
            state->current_v_cmd = 0.0f;
        }
        else
        {
            state->current_v_cmd -= MC_SIGN(state->current_v_cmd) * dv;
        }
        state->current_w_cmd = 0.0f;

        *out_v = state->current_v_cmd;
        *out_w = state->current_w_cmd;
        return;
    }

    /* ================================================================
     * Layer 2: 驾驶意图 → 运动学目标
     * ================================================================ */

    /* 2a: 线速度（绝对值，方向由档位决定）*/
    float v_cmd = MC_ResolveVelocity(cf, state, dt);
    v_cmd *= dir_sign;  // D=+1, R=-1

    /* 2b: 角速度（传入带符号的线速度，以正确处理倒车转向）*/
    float w_cmd = MC_ResolveAngularVelocity(cf, v_cmd);

    /* ================================================================
     * Layer 3: 制动仲裁
     * ================================================================ */
    v_cmd = MC_BrakingArbitrate(cf, state, v_cmd, dt);

    /* ================================================================
     * Layer 4: 约束与平滑
     * ================================================================ */
    MC_ApplyLimits(state, &v_cmd, &w_cmd, dt);

    /* 持久化当前周期结果 */
    state->current_v_cmd = v_cmd;
    state->current_w_cmd = w_cmd;

    *out_v = v_cmd;
    *out_w = w_cmd;
}

driving_mode MotionControl_GetMode(const MotionControl_State_t* state)
{
    (void)state;
    /* 当前驾驶模式暂未保存在 state 中，始终返回 automatic */
    return automatic;
}

uint8_t MotionControl_IsEmergencyStop(const MotionControl_State_t* state)
{
    return state ? state->emergency_stop : 0;
}

/* ================================================================
 * Layer 0: 驾驶模式仲裁
 * ================================================================ */

static driving_mode MC_ArbitrateMode(const Chassis_Function* cf)
{
    uint8_t takeover =
        cf->Steering_Function.Manual_Takeover ||
        cf->Drive_Function.Manual_Takeover     ||
        cf->Braking_Function.Manual_Takeover;

    if (takeover)
    {
        return interrupted;
    }

    return automatic;
}

/* ================================================================
 * Layer 1: 安全门
 * ================================================================ */

static uint8_t MC_SafetyGate(const Chassis_Function* cf,
                             MotionControl_State_t* state,
                             float* out_dir_sign)
{
    *out_dir_sign = 1.0f;

    /* 1.1: EPB 驻车夹紧 → 禁止一切运动 */
    if (cf->Parking_Function.Parking_Control.Parking_Request)
    {
        state->emergency_stop = 0;
        return 0;
    }

    /* 1.2: 档位安全门 */
    Gear_Position gear = cf->Gear_Function.Gear_Feedback.Gear_Position_Status;

    if (gear == P)
    {
        state->emergency_stop = 0;
        return 0;
    }

    if (gear == N)
    {
        state->emergency_stop = 0;
        return 0;
    }

    if (gear == R)
    {
        *out_dir_sign = -1.0f;
    }

    /* 1.3: 全局故障检查（任一模块报故障 → 紧急停车）*/
    uint8_t has_fault =
        cf->Steering_Function.Steering_Feedback.Fault_message   ||
        cf->Drive_Function.Drive_Feedback.Fault_message         ||
        cf->Braking_Function.Braking_Feedback.Fault_message     ||
        cf->Gear_Function.Gear_Feedback.Fault_message           ||
        cf->Parking_Function.Parking_Feedback.Fault_message_for_parking_system;

    if (has_fault)
    {
        state->emergency_stop = 1;
        return 0;
    }

    /* 故障清除 */
    state->emergency_stop = 0;
    return 1;
}

/* ================================================================
 * Layer 2a: Drive → 线速度目标
 *
 * 三个控制模式，按优先级互斥：
 *   1. 加速度控制 (Target_Longitudinal_Acceleration > 0)  — 首选
 *   2. 扭矩控制   (Target_Driving_Torque > 0)              — 备选
 *   3. 踏板位置   (Target_Accelerator_pedal_Position > 0)  — 备选
 * ================================================================ */

static float MC_ResolveVelocity(const Chassis_Function* cf,
                                const MotionControl_State_t* state,
                                float dt)
{
    const Drive_Control* dc = &cf->Drive_Function.Drive_Control;
    float v_cmd = state->current_v_cmd;

    /* 驱动未使能或存在故障 → 保持当前速度（由制动/安全门减速）*/
    if (!dc->Driving_Config_Enable ||
        cf->Drive_Function.Drive_Feedback.Fault_message)
    {
        return v_cmd;
    }

    /* ---- 模式 1: 加速度控制 (0.1 m/s² per LSB) ---- */
    if (dc->Target_Longitudinal_Acceleration > 0)
    {
        float a_target = (float)dc->Target_Longitudinal_Acceleration * 0.1f;
        v_cmd += a_target * dt;
        return v_cmd;
    }

    /* ---- 模式 2: 扭矩控制 (1 N·m per LSB) ----
     * 使用经验系数将扭矩折算为等效加速度（可通过实车标定调整）*/
    if (dc->Target_Driving_Torque > 0)
    {
        float torque       = (float)dc->Target_Driving_Torque;
        float a_equivalent = torque * 0.05f;  // N·m → m/s² (经验系数)
        v_cmd += a_equivalent * dt;
        return v_cmd;
    }

    /* ---- 模式 3: 踏板位置 (1% per LSB) ----
     * 将百分比映射到 [0, v_max]，以加速度限幅斜坡逼近 */
    if (dc->Target_Accelerator_pedal_Position > 0)
    {
        float pedal_pct = (float)dc->Target_Accelerator_pedal_Position / 100.0f;
        float v_target  = pedal_pct * MC_MAX_LINEAR_VELOCITY;

        if (v_target > v_cmd)
        {
            v_cmd += MC_MAX_LINEAR_ACCEL * dt;
            if (v_cmd > v_target) v_cmd = v_target;
        }
        else
        {
            v_cmd -= MC_MAX_LINEAR_DECEL * dt;
            if (v_cmd < v_target) v_cmd = v_target;
        }
        return v_cmd;
    }

    /* 无有效控制输入 → 保持 */
    return v_cmd;
}

/* ================================================================
 * Layer 2b: Steering → 角速度目标
 *
 * 差速底盘的核心设计决策：将"虚拟转向角"映射为运动曲率
 *
 *   编码规则：uint8 [0..255]，128 = 直行中心
 *   映射：angle_rad = (128 - raw) / 128 × MAX_ANGLE
 *   曲率：κ = tan(angle_rad) / virtual_wheelbase
 *   角速度：ω = v × κ
 *
 *   倒车修正：倒车时 ω 取反（倒车左转 = 车头向右）
 *   低速修正：v < v_min 时使用 v_min 替代，确保原地也能差速转向
 * ================================================================ */

static float MC_ResolveAngularVelocity(const Chassis_Function* cf,
                                       float v_signed)
{
    const Steering_Control* sc = &cf->Steering_Function.Steering_Control;

    /* 转向未使能或故障 → 直行 */
    if (!sc->Steering_Config_Enable ||
        cf->Steering_Function.Steering_Feedback.Fault_message)
    {
        return 0.0f;
    }

    /* uint8 [0..255] → 虚拟转向角 (rad) */
    float raw_angle = (float)sc->Target_Steering_Angle;
    float steer_rad = ((128.0f - raw_angle) / 128.0f) * MC_MAX_STEERING_ANGLE_RAD;

    /* 曲率 κ = tan(angle) / wheelbase */
    float curvature = tanf(steer_rad) / MC_STEERING_WHEELBASE;

    /* 低速修正：确保原地也能差速转向 */
    float v_eff = fmaxf(fabsf(v_signed), MC_MIN_SPEED_FOR_STEERING);

    /* 角速度 = 有效线速度 × 曲率 */
    float w_cmd = v_eff * curvature;

    /* 倒车转向方向反转 */
    if (v_signed < -MC_VELOCITY_ZERO_THRESHOLD)
    {
        w_cmd = -w_cmd;
    }

    return w_cmd;
}

/* ================================================================
 * Layer 3: 制动-驱动仲裁
 *
 * 制动使能 → 制动优先于驱动，按减速度斜坡降速
 * 制动故障 → 使用紧急减速度
 * 制动释放 → 允许驱动正常加速
 * ================================================================ */

static float MC_BrakingArbitrate(const Chassis_Function* cf,
                                 MotionControl_State_t* state,
                                 float v_cmd, float dt)
{
    const Braking_Control* bc = &cf->Braking_Function.Braking_Control;

    /* 制动故障 → 紧急制动 */
    if (cf->Braking_Function.Braking_Feedback.Fault_message)
    {
        state->braking_active = 1;
        float dv = MC_EMERGENCY_STOP_DECEL * dt;
        if (fabsf(v_cmd) < dv)
        {
            return 0.0f;
        }
        return v_cmd - MC_SIGN(v_cmd) * dv;
    }

    /* 制动使能且有减速度 → 执行制动 */
    if (bc->Braking_Config_Enable && bc->Target_Deceleration > 0)
    {
        if (!state->braking_active)
        {
            /* 制动切入瞬间：记录速度，用于上位机反馈 */
            state->braking_active         = 1;
            state->braking_start_tick     = HAL_GetTick();
            state->brake_velocity_capture = fabsf(v_cmd);
        }

        /* uint8: 0.1 m/s² per LSB */
        float dv = (float)bc->Target_Deceleration * 0.1f * dt;

        if (fabsf(v_cmd) < dv)
        {
            return 0.0f;
        }
        return v_cmd - MC_SIGN(v_cmd) * dv;
    }

    /* 制动释放：清除状态 */
    if (state->braking_active)
    {
        state->braking_active          = 0;
        state->brake_velocity_capture  = 0.0f;
    }

    return v_cmd;
}

/* ================================================================
 * Layer 4: 约束与平滑
 *
 *   4.1 线速度上下限（前进/倒车分别限制）
 *   4.2 线加速度/减速度限幅（基于 dt 的增量约束）
 *   4.3 角速度上下限
 *   4.4 角加速度限幅
 *   4.5 侧向加速度安全约束（v×ω ≤ a_lat_max，防侧翻）
 * ================================================================ */

static void MC_ApplyLimits(const MotionControl_State_t* state,
                           float* v, float* w, float dt)
{
    float v_cmd  = *v;
    float w_cmd  = *w;
    float v_prev = state->current_v_cmd;
    float w_prev = state->current_w_cmd;

    /* ---- 4.1: 线速度上下限 ---- */
    if (v_cmd > 0.0f)
    {
        v_cmd = MC_CLAMP(v_cmd, 0.0f, MC_MAX_LINEAR_VELOCITY);
    }
    else if (v_cmd < 0.0f)
    {
        v_cmd = MC_CLAMP(v_cmd, -MC_MAX_REVERSE_VELOCITY, 0.0f);
    }

    /* ---- 4.2: 线加速度/减速度限幅 ---- */
    float dv         = v_cmd - v_prev;
    float dv_max_acc = MC_MAX_LINEAR_ACCEL * dt;
    float dv_max_dec = MC_MAX_LINEAR_DECEL * dt;

    if (dv > dv_max_acc)
    {
        v_cmd = v_prev + dv_max_acc;
    }
    else if (dv < -dv_max_dec)
    {
        v_cmd = v_prev - dv_max_dec;
    }

    /* 接近零时直接归零 */
    if (fabsf(v_cmd) < MC_VELOCITY_ZERO_THRESHOLD)
    {
        v_cmd = 0.0f;
    }

    /* ---- 4.3: 角速度上下限 ---- */
    w_cmd = MC_CLAMP(w_cmd, -MC_MAX_ANGULAR_VELOCITY, MC_MAX_ANGULAR_VELOCITY);

    /* ---- 4.4: 角加速度限幅 ---- */
    float dw     = w_cmd - w_prev;
    float dw_max = MC_MAX_ANGULAR_ACCEL * dt;

    if (dw > dw_max)
    {
        w_cmd = w_prev + dw_max;
    }
    else if (dw < -dw_max)
    {
        w_cmd = w_prev - dw_max;
    }

    /* ---- 4.5: 侧向加速度安全约束 ----
     * a_lateral ≈ |v| × |ω| ≤ a_lat_max
     * 由此约束 |ω| ≤ a_lat_max / |v| （高速时自动限制转向角速度）*/
    if (fabsf(v_cmd) > MC_MIN_SPEED_FOR_STEERING)
    {
        float w_safe_max = MC_MAX_LATERAL_ACCEL / fabsf(v_cmd);
        w_cmd = MC_CLAMP(w_cmd, -w_safe_max, w_safe_max);
    }

    *v = v_cmd;
    *w = w_cmd;
}

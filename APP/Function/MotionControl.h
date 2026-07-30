/**
 * @file    MotionControl.h
 * @brief   运动控制顶层调度模块
 *
 * 作为线控底盘五大功能（转向/驱动/制动/档位/驻车）的顶层调度器，
 * 将 Chassis_Function 中的控制指令转换为底盘运动学目标（线速度/角速度）。
 *
 * 控制架构（五层优先级模型）：
 *   Layer 0: 驾驶模式仲裁（automatic / manual / interrupted）
 *   Layer 1: 安全门（Parking > Gear P/N > Gear R 换向 > 故障降级）
 *   Layer 2: 驾驶意图 → 运动学目标解算（Drive→v, Steering→ω）
 *   Layer 3: 制动仲裁（制动优先于驱动）
 *   Layer 4: 运动学约束与平滑（加速度限幅、曲率安全、速度上限）
 *
 * 本模块是上位机自动驾驶指令与底层底盘控制之间的桥接层。
 */

#ifndef BLDC_MOTIONCONTROL_H
#define BLDC_MOTIONCONTROL_H

#include "SbwTypes.h"

/* ================================================================
 * 运动控制参数（可根据实际底盘调校）
 * ================================================================ */

/* 速度与加速度上限 */
#define MC_MAX_LINEAR_VELOCITY          5.0f    // 最大线速度 (m/s)，前进方向
#define MC_MAX_REVERSE_VELOCITY         2.0f    // 最大倒车速度 (m/s)
#define MC_MAX_ANGULAR_VELOCITY         3.14f   // 最大角速度 (rad/s)，≈180°/s
#define MC_MAX_LINEAR_ACCEL             2.0f    // 最大线加速度 (m/s²)
#define MC_MAX_LINEAR_DECEL             3.0f    // 最大线减速度 (m/s²)，制动比加速更快
#define MC_MAX_ANGULAR_ACCEL            3.14f   // 最大角加速度 (rad/s²)

/* 转向-曲率模型参数 */
#define MC_STEERING_WHEELBASE           1.5f    // 虚拟轴距 (m)，用于将转向角映射为曲率
#define MC_MAX_STEERING_ANGLE_RAD       0.52f   // 最大虚拟转向角 (rad)，≈30°
#define MC_MIN_SPEED_FOR_STEERING       0.1f    // 原地转向最低有效速度 (m/s)，用于零速差速转向
#define MC_STEERING_DEADBAND_RAD        0.0175f // 转向中心死区 (rad)，≈±1°，抑制零速时微小角度偏差

/* 安全约束 */
#define MC_MAX_LATERAL_ACCEL            3.0f    // 最大侧向加速度 (m/s²)，≈0.3g，防侧翻
#define MC_EMERGENCY_STOP_DECEL         5.0f    // 紧急停车减速度 (m/s²)

/* 其他 */
#define MC_CONTROL_PERIOD_MS            10      // 控制周期 (ms)，与 SpeedControl 保持一致
#define MC_VELOCITY_ZERO_THRESHOLD      0.01f   // 速度归零判定阈值 (m/s)

/* ================================================================
 * 运动控制内部状态（跨周期持久化）
 * ================================================================ */
typedef struct {
    float    current_v_cmd;          // 当前平滑后的线速度指令 (m/s)
    float    current_w_cmd;          // 当前平滑后的角速度指令 (rad/s)
    float    current_v_fb;           // 实际线速度反馈 (m/s)，用于加速度闭环参考

    uint8_t  braking_active;         // 制动激活标志：1-制动中，0-未制动
    uint8_t  emergency_stop;         // 紧急停车标志：1-紧急停车中
    uint32_t last_update_tick;       // 上次更新时的 HAL_GetTick() 值
    uint32_t braking_start_tick;     // 制动开始时刻

    float    brake_velocity_capture; // 制动触发瞬间的速度快照，用于减速度斜坡计算
} MotionControl_State_t;

/* ================================================================
 * 公开接口
 * ================================================================ */

/**
 * @brief 初始化运动控制状态
 *
 * 将内部状态变量归零，应在底盘上电初始化阶段调用一次。
 * 调用时机：在 Chassis_Init() 之后、进入控制循环之前。
 *
 * @param state 运动控制状态指针
 */
void MotionControl_Init(MotionControl_State_t* state);

/**
 * @brief 运动控制更新函数（每周期调用，100Hz）
 *
 * 五层优先级控制：
 *   1. 读取 Chassis_Function 中各模块的控制指令与状态
 *   2. 执行驾驶模式仲裁
 *   3. 通过安全门检查
 *   4. 解算驾驶意图为运动学目标 (v, ω)
 *   5. 制动-驱动仲裁
 *   6. 施加加速度/曲率/速度约束
 *   7. 输出平滑后的目标线速度和角速度
 *
 * @param cf     底盘功能聚合实例（输入：控制指令 + 状态反馈）
 * @param state  运动控制持久化状态（读写：跨周期记忆）
 * @param out_v  输出：目标线速度 (m/s)，前进为正
 * @param out_w  输出：目标角速度 (rad/s)，逆时针为正
 */
void MotionControl_Update(const Chassis_Function* cf,
                          MotionControl_State_t* state,
                          float* out_v, float* out_w);

/**
 * @brief 获取当前驾驶模式
 * @param state 运动控制状态指针
 * @return 当前驾驶模式枚举值
 */
driving_mode MotionControl_GetMode(const MotionControl_State_t* state);

/**
 * @brief 检查是否处于紧急停车状态
 * @param state 运动控制状态指针
 * @return 1-紧急停车中，0-正常
 */
uint8_t MotionControl_IsEmergencyStop(const MotionControl_State_t* state);

#endif // BLDC_MOTIONCONTROL_H

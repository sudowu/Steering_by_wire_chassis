/**
 * @file    SbwTypes.h
 * @brief   线控底盘（Steer-by-Wire）功能类型定义
 *
 * 本文件定义了线控底盘五大功能模块的数据结构：
 *   - 线控转向 (Steering)
 *   - 线控驱动 (Drive)
 *   - 线控制动 (Braking)
 *   - 线控档位 (Gear)
 *   - 线控驻车 (Parking)
 *
 * 每个功能模块包含"控制指令"和"状态反馈"两部分，
 * 以及人工接管阈值和越界处理策略。
 * 所有模块聚合在 Chassis_Function 顶层结构体中。
 */

#ifndef __SBWTYPES_H__
#define __SBWTYPES_H__

#include "main.h"

/* ================================================================
 * CAN 帧 ID 定义 — 参见 TaskCanInfo.h（CAN 应用层协议完整定义）
 * ================================================================ */

/* ================================================================
 * 人工接管阈值（可独立标定）
 * ================================================================ */

/* 制动踏板接管阈值：0.1% per LSB，50 = 5% 踏板行程 */
#define TAKEOVER_BRAKE_PEDAL_THRESHOLD      50

/* 加速踏板接管阈值：1% per LSB，50 = 50% 踏板行程 */
#define TAKEOVER_ACCEL_PEDAL_THRESHOLD      50

/********************************************
 * 驾驶模式枚举
 ********************************************/
typedef enum {
    automatic = 0,        // 自动模式（上位机/自动驾驶控制）
    manual,               // 手动模式（人工驾驶）
    interrupted,          // 中断模式（被人工接管打断）
} driving_mode;

/********************************************
 * 线控转向功能 (Steering Function)
 *
 * 控制方向盘转向角度和转速。
 * 具备人工接管检测：当方向盘扭矩超过 Manual_Takeover 阈值时，
 * 系统退出自动驾驶模式，交由人工接管。
 ********************************************/

// 转向控制指令（上位机 → 底盘）
typedef struct {
    uint8_t Steering_Config_Enable;     // 转向使能：0-禁用，1-启用
    int8_t  Target_Steering_Angle;      // 目标转向角度，0=直行中心，单位：0.5 deg/LSB，±128→±64°
    uint8_t Target_Steering_Speed;      // 目标转向速度，单位：1 deg/s
} Steering_Control;

// 转向状态反馈（底盘 → 上位机）
typedef struct {
    uint8_t Steering_Angle;           // 当前方向盘角度，单位：0.1 deg
    uint8_t Steering_Speed;           // 当前方向盘转速，单位：1 deg/s
    driving_mode Steering_mode;       // 当前转向驾驶模式
    uint8_t Fault_message;            // 转向系统故障信息
} Steering_Feedback;

// 线控转向功能聚合体
typedef struct {
    Steering_Control Steering_Control;      // 转向控制指令
    Steering_Feedback Steering_Feedback;    // 转向状态反馈
    uint8_t Manual_Takeover;                // 人工接管触发阈值（方向盘扭矩门限）
    uint8_t Over_Threshold;                 // 越界处理：越界则拒绝执行并退出自动驾驶
} Steering_Function;

/********************************************
 * 线控驱动功能 (Drive Function)
 *
 * 控制车辆纵向运动（加速/匀速/减速）。
 * 支持加速踏板位置、目标加速度、目标驱动扭矩三种控制模式。
 * 人工接管条件：加速踏板指令覆盖（驾驶员踩踏板优先）。
 ********************************************/

// 驱动控制指令（上位机 → 底盘）
typedef struct {
    uint8_t Driving_Config_Enable;                   // 驱动使能：0-禁用，1-启用
    uint8_t Target_Accelerator_pedal_Position;       // 目标加速踏板位置，单位：1%（备选控制量）
    uint8_t Target_Longitudinal_Acceleration;        // 目标车辆纵向加速度，单位：0.1 m/s²（首选控制量）
    uint8_t Target_Driving_Torque;                   // 目标驱动扭矩，单位：1 N·m
} Drive_Control;

// 驱动状态反馈（底盘 → 上位机）
typedef struct {
    driving_mode Driving_mode;                  // 当前驱动驾驶模式
    uint8_t Accelerator_pedal_Position;         // 当前加速踏板位置，单位：1%
    uint8_t Longitudinal_Acceleration;          // 车辆实际纵向加速度，单位：0.1 m/s²
    uint8_t Vehicle_Speed;                      // 车辆实际纵向速度，单位：0.1 km/h
    uint8_t Fault_message;                      // 驱动系统故障信息
} Drive_Feedback;

// 线控驱动功能聚合体
typedef struct {
    Drive_Control Drive_Control;      // 驱动控制指令
    Drive_Feedback Drive_Feedback;    // 驱动状态反馈
    uint8_t Manual_Takeover;          // 人工接管：加速踏板指令覆盖
    uint8_t Over_Threshold;           // 越界处理：越界则拒绝执行并退出自动驾驶
} Drive_Function;

/********************************************
 * 线控制动功能 (Braking Function)
 *
 * 控制车辆减速和停车。
 * 支持制动踏板位置和目标减速度两种控制模式。
 * 包含制动灯控制（亮灯/熄灯）。
 * 人工接管条件：制动踏板指令覆盖。
 ********************************************/

// 制动控制指令（上位机 → 底盘）
typedef struct {
    uint8_t Braking_Config_Enable;              // 制动使能：0-禁用，1-启用
    uint8_t Target_Braking_Pedal_Position;      // 目标制动踏板位置，单位：0.1%
    uint8_t Target_Deceleration;                // 目标减速度，单位：0.1 m/s²
    uint8_t Break_Light_Control;                // 制动灯控制：0-熄灯，1-亮灯
} Braking_Control;

// 制动状态反馈（底盘 → 上位机）
typedef struct {
    driving_mode Current_Braking_System_Driving_Mode;       // 当前制动系统驾驶模式
    uint8_t Braking_Pedal_Position;                         // 当前制动踏板位置，单位：0.1%
    uint8_t Brake_Light_Status;                             // 制动灯状态：0-熄灭，1-点亮
    uint8_t Fault_message;                                  // 制动系统故障信息
} Braking_Feedback;

// 线控制动功能聚合体
typedef struct {
    Braking_Control Braking_Control;      // 制动控制指令
    Braking_Feedback Braking_Feedback;    // 制动状态反馈
    uint8_t Manual_Takeover;              // 人工接管：制动踏板指令覆盖
    uint8_t Over_Threshold;               // 越界处理：越界则拒绝执行并退出自动驾驶
} Braking_Function;

/********************************************
 * 线控档位功能 (Gear Function)
 *
 * 控制车辆档位切换（P/R/N/D）。
 * 换档需满足安全条件（如：D→R 需车速为零）。
 ********************************************/

// 档位位置枚举
typedef enum {
    P = 0,      // 驻车档（Parking）—— 变速箱锁止，驱动桥断开
    R,          // 倒车档（Reverse）
    N,          // 空档（Neutral）—— 驱动桥断开
    D,          // 前进档（Drive）
} Gear_Position;

// 档位控制指令（上位机 → 底盘）
typedef struct {
    uint8_t Gear_Config_Enable;          // 档位使能：0-禁用，1-启用
    Gear_Position Target_Gear_Position;  // 目标档位：P / R / N / D
} Gear_Control;

// 档位状态反馈（底盘 → 上位机）
typedef struct {
    Gear_Position Gear_Position_Status;     // 当前实际档位位置
    uint8_t Fault_message;                   // 档位系统故障信息
} Gear_Feedback;

// 线控档位功能聚合体
typedef struct {
    Gear_Control Gear_Control;      // 档位控制指令
    Gear_Feedback Gear_Feedback;    // 档位状态反馈
} Gear_Function;

/********************************************
 * 线控驻车功能 (Parking Function)
 *
 * 控制电子驻车制动系统（EPB）。
 * 驻车请求触发后，EPB 夹紧制动盘，反馈驻车状态。
 ********************************************/

// 驻车控制指令（上位机 → 底盘）
typedef struct {
    uint8_t Parking_Config_Enable;      // 驻车使能：0-禁用，1-启用
    uint8_t Parking_Request;            // 驻车请求：0-释放驻车，1-请求驻车
} Parking_Control;

// 驻车状态反馈（底盘 → 上位机）
typedef struct {
    uint8_t EPB_Switch_status;                  // EPB 开关状态：0-释放，1-夹紧
    uint8_t Parking_status_feedback;            // 驻车状态反馈：0-未驻车，1-已驻车
    uint8_t Fault_message_for_parking_system;   // 驻车系统故障信息
} Parking_Feedback;

// 线控驻车功能聚合体
typedef struct {
    Parking_Control Parking_Control;      // 驻车控制指令
    Parking_Feedback Parking_Feedback;    // 驻车状态反馈
} Parking_Function;

/********************************************
 * 线控底盘总控 (Chassis Function Aggregate)
 *
 * 聚合五大线控功能模块，作为底盘控制的顶层入口。
 * 全局实例 g_chassis_auto / g_chassis_manual 由 TaskSbwControl 任务持有。
 ********************************************/
typedef struct {
    Steering_Function Steering_Function;    // 线控转向功能
    Drive_Function Drive_Function;          // 线控驱动功能
    Braking_Function Braking_Function;      // 线控制动功能
    Gear_Function Gear_Function;            // 线控档位功能
    Parking_Function Parking_Function;      // 线控驻车功能

    driving_mode Current_Mode;              // 全局驾驶模式（由 MotionControl 仲裁）
    uint8_t Fault_Summary;                  // 故障汇总码：0-正常，非零-存在故障
    uint32_t Last_Command_Tick;             // 最后收到有效上位机指令的时间戳
} Chassis_Function;

/** 自动驾驶实例 — 接收上层 CAN 指令，定义在 TaskSbwControl.c */
extern Chassis_Function g_chassis_auto;

/** 人工驾驶实例 — 接收物理输入 CAN 帧，定义在 TaskSbwControl.c */
extern Chassis_Function g_chassis_manual;

#endif

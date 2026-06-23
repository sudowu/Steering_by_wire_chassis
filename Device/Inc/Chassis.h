//
// Created by yang on 2026/6/23.
//

#ifndef MOTOR_CHASSIS_H
#define MOTOR_CHASSIS_H

#include "Motor.h"
#include "Kinematics.h"

/* 底盘命令超时 (ms) —— 超时后视为通信丢失，自动停机 */
#define CHASSIS_COMMAND_TIMEOUT_MS  200

/**
 * @brief 底盘聚合结构体
 *
 * 聚合左右电机指针、运动学参数、目标速度、实际速度。
 * 电机左右映射在此定义一次，后续全部通过 Chassis_t 访问。
 */
typedef struct {
    Motor_t* motor_left;           // 左电机指针
    Motor_t* motor_right;          // 右电机指针
    Kinematics_Params_t kin;       // 运动学参数

    float cmd_linear_vel;          // 目标线速度 (m/s)
    float cmd_angular_vel;         // 目标角速度 (rad/s)
    uint8_t cmd_enable;            // 使能标志
    uint32_t cmd_timestamp;        // 最后指令时间戳 (HAL_GetTick)

    float actual_linear_vel;       // 实际线速度 (m/s)，由正运动学更新
    float actual_angular_vel;      // 实际角速度 (rad/s)

    uint8_t initialized;           // 初始化完成标志
} Chassis_t;

extern Chassis_t g_chassis;

void Chassis_Init(Chassis_t* c, Motor_t* left, Motor_t* right);
uint8_t Chassis_IsCommandValid(const Chassis_t* c);
void Chassis_Control(Chassis_t* c);

#endif // MOTOR_CHASSIS_H

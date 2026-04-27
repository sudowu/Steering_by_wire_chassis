//
// Created by yang on 2026/4/27.
//

#ifndef MOTOR_KINEMATICS_H
#define MOTOR_KINEMATICS_H

#include "main.h"

/* 默认运动学参数（需根据实际底盘修改）*/
#define KINEMATICS_WHEEL_RADIUS_DEFAULT    0.075f   // 车轮半径 (m)
#define KINEMATICS_TRACK_WIDTH_DEFAULT     0.350f   // 左右轮距 (m)
#define KINEMATICS_GEAR_RATIO_DEFAULT      1.0f     // 减速比 (电机轴转速 / 车轮转速)

/**
 * @brief 四轮差速底盘运动学参数
 */
typedef struct {
    float wheel_radius;    // 车轮半径 (m)
    float track_width;     // 左右轮距 (m)
    float gear_ratio;      // 减速比
} Kinematics_Params_t;

/**
 * @brief 车体运动状态
 */
typedef struct {
    float linear_velocity;  // 线速度 (m/s)，前进为正
    float angular_velocity; // 角速度 (rad/s)，逆时针为正
} Kinematics_Velocity_t;

/* --- 运动学参数配置 --- */
void Kinematics_Init(Kinematics_Params_t* params);
void Kinematics_SetParams(Kinematics_Params_t* params,
                          float wheel_radius,
                          float track_width,
                          float gear_ratio);

/* --- 正运动学：左右电机 RPM → 车体速度 --- */
Kinematics_Velocity_t Kinematics_Forward(const Kinematics_Params_t* params,
                                         float rpm_left, float rpm_right);

/* --- 逆运动学：车体速度 → 左右电机目标 RPM --- */
void Kinematics_Inverse(const Kinematics_Params_t* params,
                        float linear_vel, float angular_vel,
                        float* out_rpm_left, float* out_rpm_right);

/* --- 辅助函数 --- */
float Kinematics_RpmToRadS(float rpm);
float Kinematics_RadSToRpm(float rad_s);

#endif //MOTOR_KINEMATICS_H

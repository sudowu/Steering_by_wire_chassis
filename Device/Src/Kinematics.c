//
// Created by yang on 2026/4/27.
//

#include "../Inc/Kinematics.h"

#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

/**
 * @brief 使用默认参数初始化运动学模型
 * @param params 运动学参数指针
 */
void Kinematics_Init(Kinematics_Params_t* params)
{
    params->wheel_radius = KINEMATICS_WHEEL_RADIUS_DEFAULT;
    params->track_width  = KINEMATICS_TRACK_WIDTH_DEFAULT;
    params->gear_ratio   = KINEMATICS_GEAR_RATIO_DEFAULT;
}

/**
 * @brief 设置运动学参数
 * @param params 运动学参数指针
 * @param wheel_radius 车轮半径 (m)
 * @param track_width 左右轮距 (m)
 * @param gear_ratio 减速比
 */
void Kinematics_SetParams(Kinematics_Params_t* params,
                          float wheel_radius,
                          float track_width,
                          float gear_ratio)
{
    params->wheel_radius = wheel_radius;
    params->track_width  = track_width;
    params->gear_ratio   = gear_ratio;
}

/**
 * @brief 正运动学：由左右电机实际 RPM 计算车体线速度和角速度
 *
 *  设车轮角速度 ω_w = (RPM / gear_ratio) * (2π / 60)
 *     线速度 v = r * (ω_w_R + ω_w_L) / 2
 *     角速度 θ = r * (ω_w_R - ω_w_L) / track_width
 *
 * @param params 运动学参数
 * @param rpm_left  左电机当前转速 (RPM)
 * @param rpm_right 右电机当前转速 (RPM)
 * @return 车体速度 (线性速度 m/s, 角速度 rad/s)
 */
Kinematics_Velocity_t Kinematics_Forward(const Kinematics_Params_t* params,
                                         float rpm_left, float rpm_right)
{
    Kinematics_Velocity_t vel = {0.0f, 0.0f};
    float r   = params->wheel_radius;
    float L   = params->track_width;
    float G   = params->gear_ratio;

    // 电机 RPM → 车轮角速度 (rad/s)
    float w_left  = (rpm_left  / G) * (2.0f * M_PI / 60.0f);
    float w_right = (rpm_right / G) * (2.0f * M_PI / 60.0f);

    // 正运动学公式
    vel.linear_velocity  = r * (w_right + w_left) / 2.0f;
    vel.angular_velocity = r * (w_right - w_left) / L;

    return vel;
}

/**
 * @brief 逆运动学：由目标车体速度计算左右电机的目标 RPM
 *
 *  车轮角速度 ω_w_R = (2v + θ̇L) / (2r)
 *               ω_w_L = (2v - θ̇L) / (2r)
 *  电机 RPM = ω_w * G * (60 / 2π)
 *
 * @param params 运动学参数
 * @param linear_vel  目标线速度 (m/s)，前进为正
 * @param angular_vel 目标角速度 (rad/s)，逆时针为正
 * @param out_rpm_left  输出：左电机目标 RPM
 * @param out_rpm_right 输出：右电机目标 RPM
 */
void Kinematics_Inverse(const Kinematics_Params_t* params,
                        float linear_vel, float angular_vel,
                        float* out_rpm_left, float* out_rpm_right)
{
    float r = params->wheel_radius; //车轮半径
    float L = params->track_width;  //左右轮距
    float G = params->gear_ratio;   //减速比

    // 逆运动学公式：计算车轮角速度 (rad/s)
    float w_right = (2.0f * linear_vel + angular_vel * L) / (2.0f * r);
    float w_left  = (2.0f * linear_vel - angular_vel * L) / (2.0f * r);

    // 车轮角速度 → 电机目标 RPM
    *out_rpm_right = w_right * G * (60.0f / (2.0f * M_PI));
    *out_rpm_left  = w_left  * G * (60.0f / (2.0f * M_PI));
}

/**
 * @brief 将 RPM 转换为角速度 (rad/s)
 * @param rpm 转速 (RPM)
 * @return 角速度 (rad/s)
 */
float Kinematics_RpmToRadS(float rpm)
{
    return rpm * (2.0f * M_PI / 60.0f);
}

/**
 * @brief 将角速度 (rad/s) 转换为 RPM
 * @param rad_s 角速度 (rad/s)
 * @return 转速 (RPM)
 */
float Kinematics_RadSToRpm(float rad_s)
{
    return rad_s * (60.0f / (2.0f * M_PI));
}

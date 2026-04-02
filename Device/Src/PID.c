//
// Created by yang on 2026/3/20.
//

#include "../Inc/PID.h"

#include "main.h"
#include <math.h>
#include <string.h>

/**
 * @brief 初始化 PID 控制器
 * @param pid PID 控制器指针
 * @param Kp 比例增益
 * @param Ki 积分增益
 * @param Kd 微分增益
 * @param outMax 输出上限
 * @param outMin 输出下限
 */
void PID_Init(AdvancedPID_TypeDef* pid, float Kp, float Ki, float Kd, 
              float outMax, float outMin)
{
    // 清除所有参数
    memset(pid, 0, sizeof(AdvancedPID_TypeDef));
    
    // 设置 PID 系数
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->Kff = 0.0f;  // 前馈系数初始化为 0
    
    // 设置输出限幅
    pid->outMax = outMax;
    pid->outMin = outMin;
    
    // 设置积分限幅 (通常为输出的 50%)
    pid->intMax = (outMax - outMin) * 0.5f;
    pid->intMin = -(outMax - outMin) * 0.5f;
    
    // 设置死区
    pid->deadband = 0.0f;
    
    // 设置微分滤波系数 (0-1，越小滤波效果越强)
    pid->dFilterCoef = 0.1f;
    
    // 使能 PID
    pid->enable = 1;
    pid->mode = 0;  // 位置模式
}

/**
 * @brief 设置 PID 目标值
 * @param pid PID 控制器指针
 * @param target 目标值
 */
void PID_SetTarget(AdvancedPID_TypeDef* pid, float target)
{
    pid->target = target;
}

/**
 * @brief 设置 PID 反馈值
 * @param pid PID 控制器指针
 * @param feedback 反馈值
 */
void PID_SetFeedback(AdvancedPID_TypeDef* pid, float feedback)
{
    pid->feedback = feedback;
}

/**
 * @brief 重置 PID 积分和误差历史
 * @param pid PID 控制器指针
 */
void PID_Reset(AdvancedPID_TypeDef* pid)
{
    pid->error = 0.0f;
    pid->lastError = 0.0f;
    pid->prevError = 0.0f;
    pid->integral = 0.0f;
    pid->derivative = 0.0f;
    pid->output = 0.0f;
}

/**
 * @brief 计算 PID 输出 (增量式 PID)
 * @param pid PID 控制器指针
 * @return PID 输出值
 */
float PID_Calculate(AdvancedPID_TypeDef* pid)
{
    if (!pid->enable)
    {
        return 0.0f;
    }
    
    // 计算当前误差
    pid->error = pid->target - pid->feedback;
    
    // 死区处理
    if (fabsf(pid->error) < pid->deadband)
    {
        pid->error = 0.0f;
    }
    
    // 比例项
    float proportional = pid->Kp * pid->error;
    
    // 积分项 (带抗饱和)
    pid->integral += pid->Ki * pid->error;
    
    // 积分限幅
    if (pid->integral > pid->intMax)
    {
        pid->integral = pid->intMax;
    }
    else if (pid->integral < pid->intMin)
    {
        pid->integral = pid->intMin;
    }
    
    // 微分项 (带低通滤波)
    float raw_derivative = pid->error - pid->lastError;
    pid->derivative = pid->dFilterCoef * raw_derivative + 
                      (1.0f - pid->dFilterCoef) * pid->derivative;
    float derivative_term = pid->Kd * pid->derivative;
    
    // 前馈项
    float feedforward = pid->Kff * pid->target;
    
    // 计算总输出
    pid->output = proportional + pid->integral + derivative_term + feedforward;
    
    // 输出限幅
    if (pid->output > pid->outMax)
    {
        pid->output = pid->outMax;
    }
    else if (pid->output < pid->outMin)
    {
        pid->output = pid->outMin;
    }
    
    // 更新误差历史
    pid->prevError = pid->lastError;
    pid->lastError = pid->error;
    
    return pid->output;
}

/**
 * @brief 设置 PID 参数
 * @param pid PID 控制器指针
 * @param Kp 比例增益
 * @param Ki 积分增益
 * @param Kd 微分增益
 */
void PID_SetParams(AdvancedPID_TypeDef* pid, float Kp, float Ki, float Kd)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
}

/**
 * @brief 获取 PID 当前误差
 * @param pid PID 控制器指针
 * @return 当前误差值
 */
float PID_GetError(AdvancedPID_TypeDef* pid)
{
    return pid->error;
}

/**
 * @brief 启用/禁用 PID 控制器
 * @param pid PID 控制器指针
 * @param enable 1-启用，0-禁用
 */
void PID_SetEnable(AdvancedPID_TypeDef* pid, uint8_t enable)
{
    pid->enable = enable;
    if (!enable)
    {
        PID_Reset(pid);
    }
}






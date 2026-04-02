//
// Created by yang on 2026/3/20.
//

#ifndef MOTOR_PID_H
#define MOTOR_PID_H

#include "main.h"

typedef struct {
    // --- 1. 系数 ---
    float Kp;          // 比例增益
    float Ki;          // 积分增益
    float Kd;          // 微分增益
    float Kff;         // 前馈系数

    // --- 2. 设定与反馈 ---
    float target;      // 目标值
    float feedback;    // 反馈值

    // --- 3. 误差与历史 ---
    float error;       // 当前误差
    float lastError;   // 上一次误差
    float prevError;   // 上上次误差
    float integral;    // 积分累加

    // --- 4. 微分滤波 ---
    float derivative;  // 滤波后的微分值
    float dFilterCoef; // 微分低通滤波系数

    // --- 5. 限幅 ---
    float outMax;      // 输出上限
    float outMin;      // 输出下限
    float intMax;      // 积分上限
    float intMin;      // 积分下限

    // --- 6. 死区 ---
    float deadband;    // 误差死区

    // --- 7. 输出 ---
    float output;      // 最终输出

    // --- 8. 模式控制 ---
    uint8_t enable;    // PID使能标志
    uint8_t mode;      // PID模式

} AdvancedPID_TypeDef;

// PID 函数声明
void PID_Init(AdvancedPID_TypeDef* pid, float Kp, float Ki, float Kd, 
              float outMax, float outMin);
void PID_SetTarget(AdvancedPID_TypeDef* pid, float target);
void PID_SetFeedback(AdvancedPID_TypeDef* pid, float feedback);
void PID_Reset(AdvancedPID_TypeDef* pid);
float PID_Calculate(AdvancedPID_TypeDef* pid);
void PID_SetParams(AdvancedPID_TypeDef* pid, float Kp, float Ki, float Kd);
float PID_GetError(AdvancedPID_TypeDef* pid);
void PID_SetEnable(AdvancedPID_TypeDef* pid, uint8_t enable);

#endif //MOTOR_PID_H
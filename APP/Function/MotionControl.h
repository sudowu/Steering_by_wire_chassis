/**
 * @file    MotionControl.h
 * @brief   运动控制顶层调度模块
 *
 * 作为线控底盘五大功能的顶层调度器，将 Chassis_Function 中的
 * 转向/驱动/制动/档位/驻车指令转换为底盘运动学目标（线速度/角速度）。
 *
 * 本模块是上位机自动驾驶指令与底层底盘控制之间的桥接层。
 */

#ifndef BLDC_MOTIONCONTROL_H
#define BLDC_MOTIONCONTROL_H

#include "SbwTypes.h"

/**
 * @brief 运动控制更新函数（每周期调用）
 *
 * 读取 Chassis_Function 中各模块的控制指令，决策出目标线速度
 * 和角速度，供 Chassis_Control() 的逆运动学解算使用。
 *
 * @param Chassis_Function_instance 底盘功能聚合实例（输入：控制指令）
 * @param Target_linear_velocity    输出：目标线速度 (m/s)，前进为正
 * @param Target_angular_velocity   输出：目标角速度 (rad/s)，逆时针为正
 *
 * @note 当前为空桩代码，待上层控制逻辑实现后填充。
 */
void MotionControl_Update(Chassis_Function* Chassis_Function_instance,
    float* Target_linear_velocity, float* Target_angular_velocity);

#endif //BLDC_MOTIONCONTROL_H

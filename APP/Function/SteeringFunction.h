/**
 * @file    SteeringFunction.h
 * @brief   线控转向功能模块
 *
 * 负责方向盘转向角度和转速的线控控制。
 * 包含控制指令接收、状态反馈上报、人工接管检测和越界处理。
 */

#ifndef __SteeringFunction_H__
#define __SteeringFunction_H__

#include "SbwTypes.h"

/**
 * @brief 初始化转向功能模块
 *
 * 将控制指令清零、反馈状态置为 safe 默认值（manual 模式）。
 * 启用前需先通过 CAN/上位机设置 Steering_Control 字段。
 *
 * @param sf 转向功能实例指针
 */
void SteeringFunction_Init(Steering_Function* sf);

#endif

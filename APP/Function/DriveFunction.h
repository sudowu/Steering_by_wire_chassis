/**
 * @file    DriveFunction.h
 * @brief   线控驱动功能模块
 *
 * 负责车辆纵向运动控制（加速/匀速/减速）。
 * 支持三种控制模式：加速踏板位置、目标纵向加速度、目标驱动扭矩。
 * 驾驶员踩加速踏板时触发人工接管，优先响应人工操作。
 */

#ifndef __DriveFunction_H__
#define __DriveFunction_H__

#include "SbwTypes.h"

/**
 * @brief 初始化驱动功能模块
 *
 * 将控制指令清零、反馈状态置为安全默认值（manual 模式）。
 *
 * @param df 驱动功能实例指针
 */
void DriveFunction_Init(Drive_Function* df);

#endif

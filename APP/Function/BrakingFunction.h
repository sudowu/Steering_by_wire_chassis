/**
 * @file    BrakingFunction.h
 * @brief   线控制动功能模块
 *
 * 负责车辆减速和停车控制。
 * 支持制动踏板位置和目标减速度两种控制模式。
 * 包含制动灯自动控制逻辑。
 * 驾驶员踩制动踏板时触发人工接管。
 */

#ifndef __BrakingFunction_H__
#define __BrakingFunction_H__

#include "SbwTypes.h"

/**
 * @brief 初始化制动功能模块
 *
 * 将控制指令清零、反馈状态置为安全默认值（manual 模式）。
 *
 * @param bf 制动功能实例指针
 */
void BrakingFunction_Init(Braking_Function* bf);

#endif

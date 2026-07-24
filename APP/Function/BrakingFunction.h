/**
 * @file    BrakingFunction.h
 * @brief   线控制动功能模块
 *
 * 负责车辆减速和停车控制。
 *
 * 当前无独立制动器硬件 —— 行车制动通过电机减速实现：
 *   MotionControl Layer 3 直接读取 Braking_Control.Target_Deceleration
 *   以控制减速度斜坡降速，电机即"制动执行器"。
 *
 * 本模块职责（决策层）：
 *   - 校验制动指令合法性
 *   - 制动灯自动控制逻辑
 *   - 反馈状态同步
 *   - 制动踏板人工接管检测
 *
 * 支持两种控制模式（优先级互斥）：
 *   1. 目标减速度 (Target_Deceleration) — 首选
 *   2. 制动踏板位置 (Target_Braking_Pedal_Position) — 百分比映射减速度
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

/**
 * @brief 制动功能更新（每周期调用）
 *
 * 决策逻辑：
 *   1. 校验目标值合法性（越界 → Fault_message + 拒绝执行）
 *   2. 使能 + 有指令 → 同步制动灯 + 反馈状态
 *   3. 未使能         → 清制动灯 + 同步状态
 *   4. 制动踏板 > 阈值 → Manual_Takeover 置位
 *
 * 注意：实际减速度由 MotionControl Layer 3 执行，
 *       本函数只做指令校验和状态同步，不直接控制电机。
 *
 * @param bf 制动功能实例
 */
void BrakingFunction_Update(Braking_Function* bf);

#endif

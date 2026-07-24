/**
 * @file    TaskSbwControl.h
 * @brief   线控底盘 SbW 主控制任务
 *
 * 本任务是线控底盘五大功能模块（转向/驱动/制动/档位/驻车）的主调度器。
 *
 * 控制循环 (100Hz):
 *   1. 从 Chassis_Function_instance 读取上位机指令（由 CAN RX 填充）
 *   2. 调用 MotionControl_Update() 执行五层安全仲裁与运动学解算
 *   3. 将处理后的 (v, ω) 写入 g_chassis，供 Chassis_Control() 执行
 *   4. 发送 SbW 状态反馈帧（预留）
 *
 * 与直接 CAN 0x100 控制的关系：
 *   - SbW 使能时（任一功能模块 Enable=1），本任务覆盖 g_chassis
 *   - SbW 未使能时，保持 g_chassis 不变（兼容直接 CAN 0x100 控制）
 */

#ifndef __TASKSBWCONTROL_H__
#define __TASKSBWCONTROL_H__

#include "SbwTypes.h"

/**
 * @brief SbW 主控制任务入口
 *
 * 由 FreeRTOS 调度器创建，优先级建议 osPriorityNormal+2（高于 SpeedControl）。
 * 栈大小建议 256 words。
 *
 * @param parameter 未使用
 */
void vTaskSbwControl(void* parameter);

#endif

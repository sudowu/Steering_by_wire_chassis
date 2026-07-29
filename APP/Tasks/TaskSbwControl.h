/**
 * @file    TaskSbwControl.h
 * @brief   线控底盘 SbW 主控制任务
 *
 * 本任务是线控底盘五大功能模块（转向/驱动/制动/档位/驻车）的主调度器。
 *
 * 管理两个 Chassis_Function 实例：
 *   g_chassis_auto   — 自动驾驶实例（接收上层 CAN 指令 frames 0x200-0x201）
 *   g_chassis_manual — 人工驾驶实例（接收物理输入 CAN frames 0x210-0x212）
 *
 * 控制循环 (100Hz):
 *   1. 硬件执行器推进（EPB + 档位）
 *   2. manual 实例模块更新（接管检测）
 *   3. DrivingModeFunction 仲裁全局驾驶模式
 *   4. 模式分支执行档位/驻车 + MotionControl 五层仲裁
 *   5. P 档 EPB 联动
 *   6. 写入 g_chassis + 反馈同步（以实际物理状态为准）
 *
 * 与直接 CAN 0x100 控制的关系：
 *   - SbW 使能时（任一功能模块 Enable=1），本任务覆盖 g_chassis
 *   - SbW 未使能时，保持 g_chassis 不变（兼容直接 CAN 0x100 调试控制）
 */

#ifndef __TASKSBWCONTROL_H__
#define __TASKSBWCONTROL_H__

#include "SbwTypes.h"

/** 自动驾驶实例 — 接收上层 CAN 指令 */
extern Chassis_Function g_chassis_auto;

/** 人工驾驶实例 — 接收物理输入 CAN 帧 */
extern Chassis_Function g_chassis_manual;

/**
 * @brief SbW 主控制任务入口
 *
 * 由 FreeRTOS 调度器创建，优先级建议 osPriorityNormal+2（高于 SpeedControl）。
 * 栈大小建议 512 words（双实例状态 + MotionControl + 接管检测）。
 *
 * @param parameter 未使用
 */
void vTaskSbwControl(void* parameter);
uint8_t SbwIsActive(const Chassis_Function* cf);
void SbwEnableAll(Chassis_Function* cf);
void SbwDisableAll(Chassis_Function* cf);
#endif

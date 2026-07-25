/**
 * @file    DrivingModeFunction.h
 * @brief   驾驶模式控制模块
 *
 * 统一管理全局驾驶模式（automatic / interrupted / manual）的状态机，
 * 汇总来自 g_chassis_manual 的人工接管信号（踏板/方向盘/档位mismatch）和
 * g_chassis_auto 的 Over_Threshold 信号，仲裁模式切换并同步到两个实例的各模块反馈。
 *
 * 状态机：
 *   ┌───────────┐  接管触发    ┌─────────────┐  安全停机完成   ┌────────┐
 *   │ automatic │ ──────────→ │ interrupted │ ─────────────→ │ manual │
 *   │  自动驾驶  │              │  紧急停机中   │                │ 人工驾驶 │
 *   └───────────┘              └─────────────┘                └───┬────┘
 *        ↑                                                       │
 *        └───────────────────────────────────────────────────────┘
 *                 进入条件：物理 D 档 + 无故障 + 无接管
 *
 * 接管信号来源（来自 g_chassis_manual 的物理输入）：
 *   - Braking_Function.Manual_Takeover    (刹车踏板 > 阈值)
 *   - Drive_Function.Manual_Takeover      (加速踏板 > 阈值)
 *   - Steering_Function.Manual_Takeover  (方向盘扭矩 > 阈值)
 *   - 物理档位 ≠ 自动档位                 (档位 mismatch)
 *
 * 越界信号来源（来自 g_chassis_auto）：
 *   - 任一模块 Over_Threshold
 *
 * 进入自动模式条件（manual → automatic）：
 *   - 物理档位 == D
 *   - Fault_Summary == 0
 *   - 无接管信号
 */

#ifndef __DrivingModeFunction_H__
#define __DrivingModeFunction_H__

#include "SbwTypes.h"

/**
 * @brief 驾驶模式状态机持久状态
 *
 * 存放模式切换锁存、计时和请求等内部状态。
 * 对外输出写入 Chassis_Function.Current_Mode。
 */
typedef struct {
    driving_mode current_mode;       /**< 当前全局驾驶模式              */
    uint8_t      reenable_request;   /**< 请求恢复自动驾驶: 0=无, 1=请求 */
    uint32_t     mode_entry_tick;    /**< 进入当前模式的系统时刻          */
    uint8_t      takeover_prev;      /**< 上周期接管状态（上升沿检测）    */
} DrivingModeFunction_State_t;

/**
 * @brief 初始化驾驶模式状态机
 *
 * 默认为 manual 模式（安全默认），等待物理 D 档 + 无故障自动进入 automatic。
 *
 * @param state 状态机实例
 */
void DrivingModeFunction_Init(DrivingModeFunction_State_t* state);

/**
 * @brief 驾驶模式状态机更新（每周期调用）
 *
 * 汇总手动实例的接管信号 + 档位 mismatch → 仲裁模式切换 → 写回两个实例。
 *
 * 模式切换逻辑：
 *   - automatic → interrupted: 任一接管信号置位或档位 mismatch
 *   - interrupted → manual:   车速归零后保持 MODE_HOLD_TIME_MS
 *   - manual → automatic:     物理 D 档 + 无故障 + 无接管（自动进入，无需 reenable_request）
 *
 * @param state            状态机实例
 * @param cf_auto          自动驾驶实例（读取 Over_Threshold + 自动档位指令）
 * @param cf_manual        人工驾驶实例（读取接管信号 + 物理档位）
 * @param vehicle_speed_mps 当前车速 (m/s)，用于判断安全停机完成
 * @return driving_mode    当前全局驾驶模式
 */
driving_mode DrivingModeFunction_Update(DrivingModeFunction_State_t* state,
                                         Chassis_Function* cf_auto,
                                         Chassis_Function* cf_manual,
                                         float vehicle_speed_mps);

/**
 * @brief 请求恢复自动驾驶
 *
 * 上位机在确认人工驾驶结束后调用此函数。
 * 当物理 D 档 + 无接管 + 无故障时，状态机将在下一周期切回 automatic。
 *
 * @param state 状态机实例
 */
void DrivingModeFunction_RequestReenable(DrivingModeFunction_State_t* state);

/**
 * @brief 直接设置为自动驾驶模式
 *
 * CAN 收到上位机 SbW 使能指令时调用。
 * 清除两个实例的 Manual_Takeover 和 Over_Threshold 信号，
 * 立即切换到 automatic 并同步到 Chassis_Function。
 *
 * 注意：若此后仍有外部信号触发接管，Update() 会打断回 interrupted。
 *
 * @param state 状态机实例
 * @param cf_auto    自动驾驶实例
 * @param cf_manual  人工驾驶实例
 */
void DrivingModeFunction_SetAutomatic(DrivingModeFunction_State_t* state,
                                       Chassis_Function* cf_auto,
                                       Chassis_Function* cf_manual);

/**
 * @brief 直接设置为人工驾驶模式
 *
 * CAN 收到上位机 SbW 解除指令或发生严重故障时调用。
 * 清除两个实例的 Manual_Takeover 和 Over_Threshold 信号，
 * 立即切换到 manual 并同步到 Chassis_Function。
 *
 * @param state 状态机实例
 * @param cf_auto    自动驾驶实例
 * @param cf_manual  人工驾驶实例
 */
void DrivingModeFunction_SetManual(DrivingModeFunction_State_t* state,
                                    Chassis_Function* cf_auto,
                                    Chassis_Function* cf_manual);

#endif

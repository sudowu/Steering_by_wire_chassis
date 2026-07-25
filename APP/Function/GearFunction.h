/**
 * @file    GearFunction.h
 * @brief   线控档位功能模块
 *
 * 负责车辆档位切换（P/R/N/D）。
 *
 * 安全互锁规则：
 *   - 任意档位 → P: 需车速归零
 *   - D ↔ R:       需车速归零（行进中换向危险）
 *   - D → N / N → D: 任意车速允许
 *   - 换档进行中忽略新请求
 *
 * 职责划分：
 *   本模块（APP/Function）— 决策层：安全互锁判断
 *   Device/Gear.c        — 执行层：GPIO 编码输出
 */

#ifndef __GearFunction_H__
#define __GearFunction_H__

#include "SbwTypes.h"
#include "Gear.h"       // Device 层档位执行器

/**
 * @brief 初始化档位功能模块
 *
 * 将控制指令和反馈状态均置为 P 档（驻车）。
 *
 * @param gf 档位功能实例指针
 */
void GearFunction_Init(Gear_Function* gf);

/**
 * @brief 档位功能更新（每周期调用）
 *
 * 决策逻辑：
 *   1. 读取 Gear_Control.Target_Gear_Position
 *   2. 安全互锁检查（车速、当前档位、驾驶模式）
 *   3. 通过 → Gear_Actuator_SetPosition()
 *   4. 拒绝 → 反馈沿用当前档位，不执行
 *   5. 同步执行器状态到 Gear_Feedback
 *
 * 模式差异：
 *   - manual 模式：所有换档操作均需车速归零
 *   - auto 模式：  P/R 需停车，N↔D 任意速度允许
 *
 * @param gf         档位功能实例
 * @param actuator   档位执行器实例
 * @param vehicle_speed_mps  当前车速 (m/s)，绝对值，用于安全互锁
 * @param is_auto_mode      0=manual（全部等停），1=auto（N/D不等停）
 */
void GearFunction_Update(Gear_Function* gf,
                         Gear_Actuator_t* actuator,
                         float vehicle_speed_mps,
                         uint8_t is_auto_mode);

#endif

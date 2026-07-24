/**
 * @file    ParkingFunction.h
 * @brief   线控驻车功能模块
 *
 * 负责电子驻车制动系统（EPB）的线控控制。
 *
 * 职责划分：
 *   本模块（APP/Function）— 决策层：驻车请求处理、安全互锁
 *   Device/Parking.c      — 执行层：左右 EPB GPIO 控制
 */

#ifndef __ParkingFunction_H__
#define __ParkingFunction_H__

#include "SbwTypes.h"
#include "Parking.h"    // Device 层 EPB 执行器

/**
 * @brief 初始化驻车功能模块
 *
 * 将控制指令和反馈状态全部清零。
 *
 * @param pf 驻车功能实例指针
 */
void ParkingFunction_Init(Parking_Function* pf);

/**
 * @brief 驻车功能更新（每周期调用）
 *
 * 决策逻辑：
 *   1. 读取 Parking_Control.Parking_Request
 *   2. 请求夹紧 → 调用 Parking_Actuator_Clamp()
 *   3. 请求释放 → 调用 Parking_Actuator_Release()
 *   4. 同步 EPB 状态到 Parking_Feedback
 *   5. 故障处理
 *
 * @param pf        驻车功能实例（控制指令 + 状态反馈）
 * @param actuator  EPB 执行器实例
 */
void ParkingFunction_Update(Parking_Function* pf,
                            Parking_Actuator_t* actuator);

#endif

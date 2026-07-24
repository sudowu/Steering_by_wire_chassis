/**
 * @file    ParkingFunction.h
 * @brief   线控驻车功能模块
 *
 * 负责电子驻车制动系统（EPB）的线控控制。
 * 支持远程驻车请求/释放和 EPB 开关状态反馈。
 */

#ifndef __ParkingFunction_H__
#define __ParkingFunction_H__

#include "SbwTypes.h"

/**
 * @brief 初始化驻车功能模块
 *
 * 将控制指令和反馈状态全部清零。
 *
 * @param pf 驻车功能实例指针
 */
void ParkingFunction_Init(Parking_Function* pf);

#endif

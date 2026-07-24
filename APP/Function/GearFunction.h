/**
 * @file    GearFunction.h
 * @brief   线控档位功能模块
 *
 * 负责车辆档位切换（P/R/N/D）。
 * 换档需满足安全条件（如 R→D 切换需车速归零）。
 * 不包含人工接管机制——档位由上层决策，无需直接响应驾驶员操作。
 */

#ifndef __GearFunction_H__
#define __GearFunction_H__

#include "SbwTypes.h"

/**
 * @brief 初始化档位功能模块
 *
 * 将控制指令和反馈状态均置为 P 档（驻车），故障信息清零。
 *
 * @param gf 档位功能实例指针
 */
void GearFunction_Init(Gear_Function* gf);

#endif

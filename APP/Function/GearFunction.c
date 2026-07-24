/**
 * @file    GearFunction.c
 * @brief   线控档位功能实现
 */

#include "GearFunction.h"

/**
 * @brief 初始化档位功能模块
 *
 * 将控制使能关闭、目标档位和反馈状态均置为 P 档（驻车）、故障信息清零。
 * 初始化为 P 档确保上电后底盘处于安全状态。
 *
 * @param gf 档位功能实例指针
 */
void GearFunction_Init(Gear_Function* gf)
{
    gf->Gear_Control.Gear_Config_Enable   = 0;
    gf->Gear_Control.Target_Gear_Position = P;

    gf->Gear_Feedback.Gear_Position_Status = P;
    gf->Gear_Feedback.Fault_message        = 0;
}

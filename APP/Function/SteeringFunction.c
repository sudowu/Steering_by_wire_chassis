/**
 * @file    SteeringFunction.c
 * @brief   线控转向功能实现
 */

#include "SteeringFunction.h"

/**
 * @brief 初始化转向功能模块
 *
 * 将控制指令归零、反馈置为默认手动模式、人工接管和越界阈值清零。
 * 该函数仅做结构体初始化，不涉及硬件操作。
 *
 * @param sf 转向功能实例指针
 */
void SteeringFunction_Init(Steering_Function* sf)
{
    sf->Steering_Control.Steering_Config_Enable = 0;
    sf->Steering_Control.Target_Steering_Angle   = 0;
    sf->Steering_Control.Target_Steering_Speed   = 0;

    sf->Steering_Feedback.Steering_Angle = 0;
    sf->Steering_Feedback.Steering_Speed = 0;
    sf->Steering_Feedback.Steering_mode  = manual;
    sf->Steering_Feedback.Fault_message  = 0;

    sf->Manual_Takeover = 0;
    sf->Over_Threshold  = 0;
}

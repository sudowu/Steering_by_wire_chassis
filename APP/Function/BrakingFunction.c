/**
 * @file    BrakingFunction.c
 * @brief   线控制动功能实现
 */

#include "BrakingFunction.h"

/**
 * @brief 初始化制动功能模块
 *
 * 将所有控制字段归零、反馈置为手动模式、人工接管和越界阈值清零。
 * 该函数仅做结构体初始化，不涉及硬件操作。
 *
 * @param bf 制动功能实例指针
 */
void BrakingFunction_Init(Braking_Function* bf)
{
    bf->Braking_Control.Braking_Config_Enable         = 0;
    bf->Braking_Control.Target_Braking_Pedal_Position = 0;
    bf->Braking_Control.Target_Deceleration           = 0;
    bf->Braking_Control.Break_Light_Control           = 0;

    bf->Braking_Feedback.Current_Braking_System_Driving_Mode = manual;
    bf->Braking_Feedback.Braking_Pedal_Position              = 0;
    bf->Braking_Feedback.Brake_Light_Status                  = 0;
    bf->Braking_Feedback.Fault_message                       = 0;

    bf->Manual_Takeover = 0;
    bf->Over_Threshold  = 0;
}

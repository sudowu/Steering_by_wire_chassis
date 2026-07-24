/**
 * @file    DriveFunction.c
 * @brief   线控驱动功能实现
 */

#include "DriveFunction.h"

/**
 * @brief 初始化驱动功能模块
 *
 * 将所有控制字段归零、反馈置为手动模式、人工接管和越界阈值清零。
 * 该函数仅做结构体初始化，不涉及硬件操作。
 *
 * @param df 驱动功能实例指针
 */
void DriveFunction_Init(Drive_Function* df)
{
    df->Drive_Control.Driving_Config_Enable             = 0;
    df->Drive_Control.Target_Accelerator_pedal_Position = 0;
    df->Drive_Control.Target_Longitudinal_Acceleration  = 0;
    df->Drive_Control.Target_Driving_Torque             = 0;

    df->Drive_Feedback.Driving_mode              = manual;
    df->Drive_Feedback.Accelerator_pedal_Position = 0;
    df->Drive_Feedback.Longitudinal_Acceleration  = 0;
    df->Drive_Feedback.Vehicle_Speed              = 0;
    df->Drive_Feedback.Fault_message              = 0;

    df->Manual_Takeover = 0;
    df->Over_Threshold  = 0;
}

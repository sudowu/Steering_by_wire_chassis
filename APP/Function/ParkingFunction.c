/**
 * @file    ParkingFunction.c
 * @brief   线控驻车功能实现
 */

#include "ParkingFunction.h"

/**
 * @brief 初始化驻车功能模块
 *
 * 将所有控制字段和反馈字段归零。该函数仅做结构体初始化，不涉及硬件操作。
 *
 * @param pf 驻车功能实例指针
 */
void ParkingFunction_Init(Parking_Function* pf)
{
    pf->Parking_Control.Parking_Config_Enable = 0;
    pf->Parking_Control.Parking_Request       = 0;

    pf->Parking_Feedback.EPB_Switch_status                 = 0;
    pf->Parking_Feedback.Parking_status_feedback           = 0;
    pf->Parking_Feedback.Fault_message_for_parking_system  = 0;
}

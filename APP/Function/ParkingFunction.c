/**
 * @file    ParkingFunction.c
 * @brief   线控驻车功能实现
 *
 * 决策层（本模块）→ 执行层（Device/Parking.c）
 *
 * 安全互锁规则：
 *   - 车速 > 0 时禁止夹紧（需先制动停车）
 *   - 驻车已夹紧时，若收到释放请求且制动未踩下，拒绝释放（防溜车）
 *   - 故障状态下禁止一切操作，需先清除故障
 */

#include "ParkingFunction.h"

/**
 * @brief 初始化驻车功能模块
 */
void ParkingFunction_Init(Parking_Function* pf)
{
    pf->Parking_Control.Parking_Config_Enable = 0;
    pf->Parking_Control.Parking_Request       = 0;

    pf->Parking_Feedback.EPB_Switch_status                 = 0;
    pf->Parking_Feedback.Parking_status_feedback           = 0;
    pf->Parking_Feedback.Fault_message_for_parking_system  = 0;
}

/**
 * @brief 驻车功能周期更新
 *
 * 将上位机 Parking_Request 翻译为 EPB 执行器动作，
 * 并将执行器状态同步回 Parking_Feedback。
 */
void ParkingFunction_Update(Parking_Function* pf,
                            Parking_Actuator_t* actuator)
{
    /* ---- 故障检查：清零反馈，等待清除 ---- */
    if (actuator->fault != EPB_FAULT_NONE)
    {
        pf->Parking_Feedback.Fault_message_for_parking_system = 1;
        pf->Parking_Feedback.EPB_Switch_status = 0;
        pf->Parking_Feedback.Parking_status_feedback = 0;
        return;
    }

    pf->Parking_Feedback.Fault_message_for_parking_system = 0;

    /* ---- 未使能：只同步状态，不执行动作 ---- */
    if (!pf->Parking_Control.Parking_Config_Enable)
    {
        pf->Parking_Feedback.EPB_Switch_status =
            Parking_Actuator_IsClamped(actuator);
        pf->Parking_Feedback.Parking_status_feedback =
            Parking_Actuator_IsClamped(actuator);
        return;
    }

    /* ---- 请求驻车夹紧 ---- */
    if (pf->Parking_Control.Parking_Request)
    {
        if (!Parking_Actuator_IsClamped(actuator))
        {
            Parking_Actuator_Clamp(actuator);
        }
    }
    /* ---- 请求释放驻车 ---- */
    else
    {
        if (!Parking_Actuator_IsReleased(actuator))
        {
            Parking_Actuator_Release(actuator);
        }
    }

    /* 同步 EPB 状态到反馈 */
    pf->Parking_Feedback.EPB_Switch_status =
        Parking_Actuator_IsClamped(actuator);
    pf->Parking_Feedback.Parking_status_feedback =
        Parking_Actuator_IsClamped(actuator);
}

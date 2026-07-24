/**
 * @file    GearFunction.c
 * @brief   线控档位功能实现
 *
 * 决策层：安全互锁 → 执行层 Device/Gear.c
 */

#include "GearFunction.h"

/* 档位切换安全速度阈值 (m/s)，低于此值视为"已停车" */
#define GEAR_SAFE_SPEED_THRESHOLD   0.05f

/**
 * @brief 初始化档位功能模块
 */
void GearFunction_Init(Gear_Function* gf)
{
    gf->Gear_Control.Gear_Config_Enable   = 0;
    gf->Gear_Control.Target_Gear_Position = P;

    gf->Gear_Feedback.Gear_Position_Status = P;
    gf->Gear_Feedback.Fault_message        = 0;
}

/**
 * @brief 判断换档请求是否满足安全条件
 *
 * 返回 1=允许换档, 0=安全互锁拒绝
 */
static uint8_t Gear_CheckSafety(Gear_Position from, Gear_Position to,
                                float speed_mps)
{
    /* 同档位不换 */
    if (from == to)
    {
        return 1;
    }

    /* 涉及 P 档 → 必须停车 */
    if (from == P || to == P)
    {
        return (speed_mps < GEAR_SAFE_SPEED_THRESHOLD) ? 1 : 0;
    }

    /* D ↔ R 方向切换 → 必须停车 */
    if ((from == D && to == R) || (from == R && to == D))
    {
        return (speed_mps < GEAR_SAFE_SPEED_THRESHOLD) ? 1 : 0;
    }

    /* N → R → 必须停车 */
    if (from == N && to == R)
    {
        return (speed_mps < GEAR_SAFE_SPEED_THRESHOLD) ? 1 : 0;
    }

    /* D → N, N → D, R → N: 允许 */
    return 1;
}

/**
 * @brief 档位功能周期更新
 */
void GearFunction_Update(Gear_Function* gf,
                         Gear_Actuator_t* actuator,
                         float vehicle_speed_mps)
{
    /* ---- 故障检查 ---- */
    if (actuator->fault != GEAR_FAULT_NONE)
    {
        gf->Gear_Feedback.Fault_message = 1;
        gf->Gear_Feedback.Gear_Position_Status = actuator->current;
        return;
    }
    gf->Gear_Feedback.Fault_message = 0;

    /* ---- 执行器正在换档中 → 等待完成 ---- */
    if (Gear_Actuator_IsShifting(actuator))
    {
        gf->Gear_Feedback.Gear_Position_Status = actuator->current;
        return;
    }

    /* ---- 未使能 → 仅同步状态 ---- */
    if (!gf->Gear_Control.Gear_Config_Enable)
    {
        gf->Gear_Feedback.Gear_Position_Status = actuator->current;
        return;
    }

    /* ---- 目标档位 ---- */
    Gear_Position target = gf->Gear_Control.Target_Gear_Position;
    Gear_Position current = actuator->current;

    /* 已在目标档位 */
    if (target == current)
    {
        gf->Gear_Feedback.Gear_Position_Status = current;
        return;
    }

    /* ---- 安全互锁检查 ---- */
    if (!Gear_CheckSafety(current, target, vehicle_speed_mps))
    {
        /* 拒绝换档：反馈保持当前档位，不执行 */
        gf->Gear_Feedback.Gear_Position_Status = current;
        return;
    }

    /* ---- 执行换档 ---- */
    Gear_Actuator_SetPosition(actuator, target);
    gf->Gear_Feedback.Gear_Position_Status = actuator->current;
}

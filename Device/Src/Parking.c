/**
 * @file    Parking.c
 * @brief   EPB 驻车制动执行器实现
 *
 * 两个 GPIO 控制左右制动器，左右分时上电避免电流冲击。
 */

#include "Parking.h"

/* ================================================================
 * 初始化
 * ================================================================ */

void Parking_Actuator_Init(Parking_Actuator_t* act,
                           GPIO_TypeDef* left_port,  uint16_t left_pin,
                           GPIO_TypeDef* right_port, uint16_t right_pin)
{
    act->left_port  = left_port;
    act->left_pin   = left_pin;
    act->right_port = right_port;
    act->right_pin  = right_pin;

    act->state   = EPB_STATE_RELEASED;
    act->fault   = EPB_FAULT_NONE;
    act->action_start_tick = 0;
    act->left_active  = 0;
    act->right_active = 0;

    /* 确保初始状态为释放 */
    HAL_GPIO_WritePin(act->left_port,  act->left_pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(act->right_port, act->right_pin, GPIO_PIN_RESET);
}

/* ================================================================
 * 夹紧 — 左右分时上电
 * ================================================================ */

int8_t Parking_Actuator_Clamp(Parking_Actuator_t* act)
{
    if (act->state == EPB_STATE_CLAMPING || act->state == EPB_STATE_CLAMPED)
    {
        return -1;  // 已在夹紧中/已夹紧
    }

    if (act->state == EPB_STATE_FAULT)
    {
        return -1;  // 故障中，先清除故障
    }

    /* 先上电左侧 */
    HAL_GPIO_WritePin(act->left_port, act->left_pin, GPIO_PIN_SET);
    act->left_active = 1;
    act->right_active = 0;
    act->state = EPB_STATE_CLAMPING;
    act->action_start_tick = HAL_GetTick();

    return 0;
}

/* ================================================================
 * 释放 — 左右同时断电
 * ================================================================ */

int8_t Parking_Actuator_Release(Parking_Actuator_t* act)
{
    if (act->state == EPB_STATE_RELEASING || act->state == EPB_STATE_RELEASED)
    {
        return -1;
    }

    /* 立即切断左右制动器 */
    HAL_GPIO_WritePin(act->left_port,  act->left_pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(act->right_port, act->right_pin, GPIO_PIN_RESET);

    act->left_active  = 0;
    act->right_active = 0;
    act->state = EPB_STATE_RELEASED;

    return 0;
}

/* ================================================================
 * 状态机轮询
 * ================================================================ */

void Parking_Actuator_Poll(Parking_Actuator_t* act)
{
    uint32_t elapsed;

    switch (act->state)
    {
        case EPB_STATE_CLAMPING:
            elapsed = HAL_GetTick() - act->action_start_tick;

            /* 超时检测 */
            if (elapsed > EPB_CLAMP_TIMEOUT_MS)
            {
                /* 超时 → 故障：强制断电保护 */
                HAL_GPIO_WritePin(act->left_port,  act->left_pin,  GPIO_PIN_RESET);
                HAL_GPIO_WritePin(act->right_port, act->right_pin, GPIO_PIN_RESET);
                act->left_active  = 0;
                act->right_active = 0;
                act->state = EPB_STATE_FAULT;
                act->fault = EPB_FAULT_TIMEOUT;
                return;
            }

            /* 左侧上电后延迟 EPB_ACTION_DELAY_MS 再上电右侧 */
            if (act->left_active && !act->right_active
                && elapsed > EPB_ACTION_DELAY_MS)
            {
                HAL_GPIO_WritePin(act->right_port, act->right_pin, GPIO_PIN_SET);
                act->right_active = 1;
            }

            /* 右侧也上电后再延迟 50ms 确认夹紧完成 */
            if (act->left_active && act->right_active
                && elapsed > (EPB_ACTION_DELAY_MS + 50))
            {
                act->state = EPB_STATE_CLAMPED;
            }
            break;

        case EPB_STATE_RELEASED:
        case EPB_STATE_CLAMPED:
        case EPB_STATE_FAULT:
            /* 稳态，无动作 */
            break;

        default:
            break;
    }
}

/* ================================================================
 * 状态查询
 * ================================================================ */

uint8_t Parking_Actuator_IsClamped(const Parking_Actuator_t* act)
{
    return (act->state == EPB_STATE_CLAMPED) ? 1 : 0;
}

uint8_t Parking_Actuator_IsReleased(const Parking_Actuator_t* act)
{
    return (act->state == EPB_STATE_RELEASED) ? 1 : 0;
}

EPB_Fault_t Parking_Actuator_GetFault(const Parking_Actuator_t* act)
{
    return act->fault;
}

void Parking_Actuator_ClearFault(Parking_Actuator_t* act)
{
    /* 强制断电 */
    HAL_GPIO_WritePin(act->left_port,  act->left_pin,  GPIO_PIN_RESET);
    HAL_GPIO_WritePin(act->right_port, act->right_pin, GPIO_PIN_RESET);

    act->state   = EPB_STATE_RELEASED;
    act->fault   = EPB_FAULT_NONE;
    act->left_active  = 0;
    act->right_active = 0;
}

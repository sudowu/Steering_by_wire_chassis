/**
 * @file    Gear.c
 * @brief   档位执行器实现
 *
 * 2 位 GPIO 编码控制 4 个档位 (P/R/N/D)。
 */

#include "Gear.h"

/* ---- 档位 → GPIO 编码表 ---- */
static void Gear_WriteGpio(const Gear_Actuator_t* act,
                           uint8_t pos0, uint8_t pos1)
{
    #if GEAR_USE_HARDWARE
    HAL_GPIO_WritePin(act->pos0_port, act->pos0_pin,
                      pos0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(act->pos1_port, act->pos1_pin,
                      pos1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
    #endif
}

/* ================================================================
 * 初始化 — 默认 P 档
 * ================================================================ */

void Gear_Actuator_Init(Gear_Actuator_t* act,
                        GPIO_TypeDef* pos0_port, uint16_t pos0_pin,
                        GPIO_TypeDef* pos1_port, uint16_t pos1_pin)
{
    #if GEAR_USE_HARDWARE
    act->pos0_port = pos0_port;
    act->pos0_pin  = pos0_pin;
    act->pos1_port = pos1_port;
    act->pos1_pin  = pos1_pin;
    #endif

    act->state   = GEAR_ACT_IDLE;
    act->fault   = GEAR_FAULT_NONE;
    act->current = P;
    act->target  = P;
    act->shift_start_tick = 0;

    /* 输出 P 档编码: 00 */
    Gear_WriteGpio(act, 0, 0);
}

/* ================================================================
 * 换档请求
 * ================================================================ */

int8_t Gear_Actuator_SetPosition(Gear_Actuator_t* act, Gear_Position pos)
{
    if (act->state == GEAR_ACT_SHIFTING || act->state == GEAR_ACT_FAULT)
    {
        return -1;
    }

    if (pos == act->current)
    {
        return 0;  // 已在目标档位，无需动作
    }

    act->target = pos;
    act->state  = GEAR_ACT_SHIFTING;
    act->shift_start_tick = HAL_GetTick();

    /* 输出目标档位 GPIO */
    switch (pos)
    {
        case P: Gear_WriteGpio(act, 0, 0); break;
        case R: Gear_WriteGpio(act, 1, 0); break;
        case N: Gear_WriteGpio(act, 0, 1); break;
        case D: Gear_WriteGpio(act, 1, 1); break;
        default: break;
    }

    return 0;
}

/* ================================================================
 * 周期轮询
 * ================================================================ */

void Gear_Actuator_Poll(Gear_Actuator_t* act)
{
    uint32_t elapsed;

    switch (act->state)
    {
        case GEAR_ACT_SHIFTING:
            elapsed = HAL_GetTick() - act->shift_start_tick;

            /* 超时 */
            if (elapsed > GEAR_SHIFT_TIMEOUT_MS)
            {
                act->state = GEAR_ACT_FAULT;
                act->fault = GEAR_FAULT_SHIFT_TIMEOUT;
                return;
            }

            /* 稳定时间后确认换档完成 */
            if (elapsed > GEAR_SETTLE_MS)
            {
                act->current = act->target;
                act->state   = GEAR_ACT_IDLE;
            }
            break;

        case GEAR_ACT_IDLE:
        case GEAR_ACT_FAULT:
        default:
            break;
    }
}

/* ================================================================
 * 状态查询
 * ================================================================ */

Gear_Position Gear_Actuator_GetPosition(const Gear_Actuator_t* act)
{
    return act->current;
}

uint8_t Gear_Actuator_IsShifting(const Gear_Actuator_t* act)
{
    return (act->state == GEAR_ACT_SHIFTING) ? 1 : 0;
}

Gear_Fault_t Gear_Actuator_GetFault(const Gear_Actuator_t* act)
{
    return act->fault;
}

void Gear_Actuator_ClearFault(Gear_Actuator_t* act)
{
    /* 强制回 P 档 */
    Gear_WriteGpio(act, 0, 0);
    act->state   = GEAR_ACT_IDLE;
    act->fault   = GEAR_FAULT_NONE;
    act->current = P;
    act->target  = P;
}

/**
 * @file    Gear.h
 * @brief   档位执行器硬件抽象层
 *
 * 两个 GPIO 编码 4 个档位（P/R/N/D），连接外部继电器或电磁阀：
 *   POS0  POS1  | 档位
 *   ────────────┼─────
 *    0     0    |  P  (驻车锁止)
 *    1     0    |  R  (倒车)
 *    0     1    |  N  (空挡)
 *    1     1    |  D  (前进)
 *
 * 用法：
 *   1. Gear_Actuator_Init() 绑定 GPIO
 *   2. Gear_Actuator_SetPosition() 发起换档
 *   3. Gear_Actuator_Poll() 周期调用做超时保护
 */

#ifndef DEVICE_GEAR_H
#define DEVICE_GEAR_H
#define GEAR_USE_HARDWARE   0

#if GEAR_USE_HARDWARE
#include "main.h"
#endif

#include "SbwTypes.h"   // Gear_Position 枚举


// #if GEAR_USE_HARDWARE
/* ---- 默认引脚 ---- */
#define GEAR_POS0_PIN      LED0_Pin
#define GEAR_POS0_PORT     LED0_GPIO_Port
#define GEAR_POS1_PIN      LED1_Pin
#define GEAR_POS1_PORT     LED1_GPIO_Port
// #endif

/* ---- 时序 ---- */
#define GEAR_SHIFT_TIMEOUT_MS   500     // 换档动作最大时间 (ms)
#define GEAR_SETTLE_MS          80      // 换档后稳定时间 (ms)

/* ---- 执行器状态 ---- */
typedef enum {
    GEAR_ACT_IDLE = 0,          // 空闲（档位稳定）
    GEAR_ACT_SHIFTING,          // 正在换档
    GEAR_ACT_FAULT,             // 故障
} Gear_ActState_t;

typedef enum {
    GEAR_FAULT_NONE = 0,
    GEAR_FAULT_SHIFT_TIMEOUT,   // 换档超时
} Gear_Fault_t;

/* ---- 执行器结构体 ---- */
typedef struct {
    /* 硬件 */
    #if GEAR_USE_HARDWARE
    GPIO_TypeDef* pos0_port;
    uint16_t      pos0_pin;
    GPIO_TypeDef* pos1_port;
    uint16_t      pos1_pin;
    #endif

    /* 状态 */
    Gear_ActState_t state;          // 执行器当前状态
    Gear_Fault_t    fault;          // 当前故障码
    Gear_Position   current;        // 当前实际档位
    Gear_Position   target;         // 目标档位
    uint32_t        shift_start_tick;  // 换档开始时刻
} Gear_Actuator_t;

/* ---- 接口 ---- */

/**
 * @brief 初始化档位执行器，默认置 P 档
 */
void Gear_Actuator_Init(Gear_Actuator_t* act,
                        GPIO_TypeDef* pos0_port, uint16_t pos0_pin,
                        GPIO_TypeDef* pos1_port, uint16_t pos1_pin);

/**
 * @brief 发起换档请求
 * @param pos 目标档位 (P/R/N/D)
 * @return 0=已接受, -1=正在换档中或故障
 */
int8_t Gear_Actuator_SetPosition(Gear_Actuator_t* act, Gear_Position pos);

/**
 * @brief 周期轮询 — 超时检测与换档完成判定
 */
void Gear_Actuator_Poll(Gear_Actuator_t* act);

/** @return 当前实际档位 */
Gear_Position Gear_Actuator_GetPosition(const Gear_Actuator_t* act);

/** @return 1=正在换档中 */
uint8_t Gear_Actuator_IsShifting(const Gear_Actuator_t* act);

/** @return 当前故障码 */
Gear_Fault_t Gear_Actuator_GetFault(const Gear_Actuator_t* act);

/** 清除故障，强制回 P 档 */
void Gear_Actuator_ClearFault(Gear_Actuator_t* act);

#endif

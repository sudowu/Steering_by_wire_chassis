/**
 * @file    Parking.h
 * @brief   EPB 驻车制动执行器硬件抽象层
 *
 * 两个 GPIO 分别控制左右制动器：
 *   - EPB_LEFT:  高电平=夹紧, 低电平=释放
 *   - EPB_RIGHT: 高电平=夹紧, 低电平=释放
 *
 * 使用方式：
 *   1. Parking_Actuator_Init() 绑定 GPIO
 *   2. Parking_Actuator_Clamp() 夹紧 / Release() 释放
 *   3. Parking_Actuator_Poll() 周期调用做超时保护
 */

#ifndef DEVICE_PARKING_H
#define DEVICE_PARKING_H

#include "main.h"

/* ---- 默认引脚（可在 CubeMX 中重新分配后修改）---- */
/* 当前暂用 BEEP 引脚做演示，实际硬件需改为对应的 EPB GPIO */
#define EPB_LEFT_PIN       PARK_LEFT_Pin
#define EPB_LEFT_PORT      PARK_LEFT_GPIO_Port
#define EPB_RIGHT_PIN      PARK_RIGHT_Pin
#define EPB_RIGHT_PORT     PARK_RIGHT_GPIO_Port

/* ---- 时序参数 ---- */
#define EPB_CLAMP_TIMEOUT_MS    2000    // 夹紧动作最大时间 (ms)
#define EPB_ACTION_DELAY_MS     100     // 左右分时动作间隔 (ms)，避免同时上电冲击

/* ---- 状态 ---- */
typedef enum {
    EPB_STATE_RELEASED = 0,     // 已释放
    EPB_STATE_CLAMPED,          // 已夹紧
    EPB_STATE_CLAMPING,         // 正在夹紧（动作中）
    EPB_STATE_RELEASING,        // 正在释放（动作中）
    EPB_STATE_FAULT,            // 故障
} EPB_State_t;

typedef enum {
    EPB_FAULT_NONE = 0,
    EPB_FAULT_TIMEOUT,          // 动作超时
} EPB_Fault_t;

/* ---- 执行器结构体 ---- */
typedef struct {
    /* 硬件 */
    GPIO_TypeDef* left_port;
    uint16_t      left_pin;
    GPIO_TypeDef* right_port;
    uint16_t      right_pin;

    /* 状态 */
    EPB_State_t   state;
    EPB_Fault_t   fault;
    uint32_t      action_start_tick;  // 动作开始时刻
    uint8_t       left_active : 1;    // 左侧正在动作
    uint8_t       right_active : 1;   // 右侧正在动作
} Parking_Actuator_t;

/* ---- 接口 ---- */

/**
 * @brief 初始化 EPB 执行器，绑定左右 GPIO
 *
 * @param act        执行器实例
 * @param left_port  左制动器 GPIO 端口
 * @param left_pin   左制动器 GPIO 引脚
 * @param right_port 右制动器 GPIO 端口
 * @param right_pin  右制动器 GPIO 引脚
 */
void Parking_Actuator_Init(Parking_Actuator_t* act,
                           GPIO_TypeDef* left_port,  uint16_t left_pin,
                           GPIO_TypeDef* right_port, uint16_t right_pin);

/**
 * @brief EPB 夹紧（左右依次上电）
 * @return 0=已触发, -1=已在夹紧中
 */
int8_t Parking_Actuator_Clamp(Parking_Actuator_t* act);

/**
 * @brief EPB 释放（左右同时断电）
 * @return 0=已触发, -1=已在释放中
 */
int8_t Parking_Actuator_Release(Parking_Actuator_t* act);

/**
 * @brief 周期轮询（超时检测）
 */
void Parking_Actuator_Poll(Parking_Actuator_t* act);

/** @return 1=已夹紧 */
uint8_t Parking_Actuator_IsClamped(const Parking_Actuator_t* act);

/** @return 1=已释放 */
uint8_t Parking_Actuator_IsReleased(const Parking_Actuator_t* act);

/** @return 当前故障码 */
EPB_Fault_t Parking_Actuator_GetFault(const Parking_Actuator_t* act);

/** 清除故障，强制释放 */
void Parking_Actuator_ClearFault(Parking_Actuator_t* act);

#endif

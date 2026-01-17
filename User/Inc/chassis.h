/*
 * @file    chassis.h
 * @brief   底盘控制模块头文件
 * @details 包含底盘对象定义和控制函数声明
 * @author  作者
 * @date    2024-01-XX
 */

#ifndef _CHASSIS_H
#define _CHASSIS_H

#include "bldc.h"              // 无刷直流电机控制模块
#include "main.h"              // 主程序配置
#include "stm32f407xx.h"       // STM32F407系列芯片寄存器定义
#include <stdint.h>            // 标准整数类型定义

#define remote_ch2 2            // 遥控器第2通道宏定义
#define remote_ch4 4            // 遥控器第4通道宏定义

/**
 * @brief 底盘对象结构体
 * @details 定义了底盘控制所需的各种参数和状态变量
 */
typedef struct {
  __IO bldc_obj *bldc1;       // 底盘上的第一个无刷电机对象指针
  __IO bldc_obj *bldc2;       // 底盘上的第二个无刷电机对象指针
  __IO int32_t spead;          // 【拼写错误，应为speed】速度变量
  __IO int32_t velocity_x;     // X轴方向速度分量（前进/后退）
  __IO int32_t velocity_z;     // Z轴方向角速度分量（左右转向）
  __IO uint32_t remote_count_ch2;  // 遥控器第2通道计数值
  __IO uint32_t remote_count_ch4;  // 遥控器第4通道计数值
  __IO uint16_t valid_can_num;     // CAN信号有效计数值
  __IO uint16_t valid_remote_num;  // 遥控信号有效计数值
} chassis_obj;

/**
 * @brief 信号源枚举类型
 * @details 定义了底盘控制的信号来源
 */
typedef enum{
  can_signal = 0,             // CAN总线信号
  remote_signal               // 遥控器信号
}signal_source;

extern chassis_obj s_chassis;   // 全局底盘对象实例声明

/**
 * @brief 底盘控制函数声明
 * @param chassis 底盘对象指针
 * @param signal_type 信号源类型
 * @retval None
 * @details 根据输入的速度分量控制底盘运动
 */
void chassis_control(chassis_obj *chassis, signal_source signal_type);
#endif

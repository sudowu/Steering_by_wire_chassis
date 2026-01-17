/*
 * @file    chassis.c
 * @brief   底盘控制模块实现
 * @details 包含底盘运动学控制算法和双电机协调控制逻辑
 * @author  作者
 * @date    2026-01-17
 * @note    实现差速驱动底盘的运动学模型和双电机协调控制
 */

#include "chassis.h"                   // 底盘控制模块声明
#include "bldc.h"                    // 无刷直流电机控制模块
#include "can.h"                     // CAN通信模块
#include "main.h"                    // 主程序配置
#include "stdio.h"                   // 标准输入输出库
#include "stm32_hal_legacy.h"        // STM32 HAL库兼容层
#include <stdint.h>                  // 标准整数类型定义


/* 全局底盘对象实例，连接到底盘的两个电机对象 */
chassis_obj s_chassis = {&g_bldc_motor1, &g_bldc_motor2, 0, 0, 0, 0, 0, 0, 0};
 

/**
 * @brief 底盘运动控制函数
 * @param chassis 底盘对象指针，包含两轮速度、方向等信息
 * @param signal_type 信号源类型，区分CAN信号和遥控信号
 * @retval None
 * 
 * @details
 * 该函数实现了差速驱动底盘的运动学模型，根据输入的X轴平移速度(velocity_x)
 * 和Z轴旋转速度(velocity_z)，计算出两个驱动轮的目标速度(duty1, duty2)，
 * 并分别控制两个无刷直流电机的转速和转向。
 * 
 * 运动学模型：
 * - duty1 = velocity_x + velocity_z (左轮速度)
 * - duty2 = velocity_x - velocity_z (右轮速度)
 * 
 * 控制优先级：当CAN信号有效时，忽略其他信号源，确保通信安全。
 */
void chassis_control(chassis_obj *chassis , signal_source signal_type) {
  // 安全检查：如果CAN信号有效但当前不是CAN信号源，则返回不执行控制
  if (chassis->valid_can_num != 0 && signal_type != can_signal) {
    return;
  }
  
  // 根据运动学模型计算两个电机的目标占空比
  // 左电机目标值 = X轴速度 + Z轴旋转速度
  int32_t duty1 = chassis->velocity_x + (chassis->velocity_z );
  // 右电机目标值 = X轴速度 - Z轴旋转速度
  int32_t duty2 = chassis->velocity_x - (chassis->velocity_z );
  
  // 控制左电机（MOTOR_1）
  if (duty1 >= 0) {
    // 当目标值非负时，设置为逆时针方向(CCW)，占空比为目标值
    chassis->bldc1->pwm_duty_target = duty1;
    chassis->bldc1->dir_set = CCW;
    // duty2 = chassis->velocity_x - chassis->velocity_z;
  } else {
    // 当目标值为负时，设置为顺时针方向(CW)，占空比为绝对值
    chassis->bldc1->pwm_duty_target = -duty1;
    chassis->bldc1->dir_set = CW;
    // duty2 = chassis->velocity_x + chassis->velocity_z;
  }
  
  // 控制右电机（MOTOR_2）
  if (duty2 >= 0) {
    // 当目标值非负时，设置为顺时针方向(CW)，占空比为目标值
    chassis->bldc2->pwm_duty_target = duty2;
    chassis->bldc2->dir_set = CW;
  } else {
    // 当目标值为负时，设置为逆时针方向(CCW)，占空比为绝对值
    chassis->bldc2->pwm_duty_target = -duty2;
    chassis->bldc2->dir_set = CCW;
  }
  
  // 根据信号源类型更新对应的超时计数器，用于判断信号有效性
  if (signal_type == can_signal) {
    // CAN信号有效，设置较长的超时计数值
    chassis->valid_can_num = 1000;
  }
  else if (signal_type == remote_signal) {
    // 遥控信号有效，设置较短的超时计数值
    chassis->valid_remote_num = 100;
  }

  // 设置两个电机的运行标志，并启动电机
  g_bldc_motor1.run_flag = RUN;    // 左电机运行使能
  g_bldc_motor2.run_flag = RUN;    // 右电机运行使能
  start_motor1();                  // 启动左电机
  start_motor2();                  // 启动右电机
}



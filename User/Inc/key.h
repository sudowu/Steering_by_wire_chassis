/*
 * @file    key.h
 * @brief   按键检测模块头文件
 * @details 包含按键定义和函数声明
 * @author  yang
 * @date    2025-12-11
 * @note    实现了按键扫描和检测功能
 */

#ifndef BLDC_KEY_H
#define BLDC_KEY_H

#include "main.h"

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)    /* 读取按键0状态 */
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)    /* 读取按键1状态 */
#define KEY2        HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)    /* 读取按键2状态 */

#define KEY0_PRES       1    /* 按键0按下返回值 */
#define KEY1_PRES       2    /* 按键1按下返回值 */
#define KEY2_PRES       3    /* 按键2按下返回值 */

/**
 * @brief 按键扫描函数
 * @param None
 * @retval uint8_t 按键状态，返回按键按下对应的值
 */
uint8_t key_scan(void);


#endif //BLDC_KEY_H

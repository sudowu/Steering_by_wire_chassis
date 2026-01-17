/*
 * @file    led.h
 * @brief   LED控制模块头文件
 * @details 包含LED控制相关的宏定义
 * @author  yang
 * @date    2025-12-11
 * @note    实现了LED开关和翻转控制功能
 */

#ifndef BLDC_LED_H
#define BLDC_LED_H

#include "main.h"

#define LED0(x)     do{x ? \
                        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET) : \
                        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET); \
                        }while (0);    /* 控制LED0亮灭，x为1时点亮，x为0时熄灭 */
#define LED1(x)     do{x ? \
                        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET): \
                        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET) \
                        }while (0);    /* 控制LED1亮灭，x为1时点亮，x为0时熄灭 */
#define LED0_TOGGLE()   do{HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);}while(0);  /* 翻转LED0状态 */
#define LED1_TOGGLE()   do{HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);}while(0);  /* 翻转LED1状态 */



#endif //BLDC_LED_H

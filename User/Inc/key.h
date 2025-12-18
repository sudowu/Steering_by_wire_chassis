//
// Created by yang on 2025/12/11.
//

#ifndef BLDC_KEY_H
#define BLDC_KEY_H

#include "main.h"

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define KEY2        HAL_GPIO_ReadPin(KEY2_GPIO_Port,KEY2_Pin)

#define KEY0_PRES       1
#define KEY1_PRES       2
#define KEY2_PRES       3

uint8_t key_scan(void);


#endif //BLDC_KEY_H

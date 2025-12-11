//
// Created by yang on 2025/12/11.
//

#ifndef BLDC_LED_H
#define BLDC_LED_H

#include "main.h"

#define LED0(x)     do{x ? \
                        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET) : \
                        HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET); \
                        }while (0);
#define LED1(x)     do{x ? \
                        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET): \
                        HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET) \
                        }while (0);
#define LED0_TOGGLE()   do{HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin);}while(0);
#define LED1_TOGGLE()   do{HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);}while(0);



#endif //BLDC_LED_H
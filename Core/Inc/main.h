/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY0_Pin GPIO_PIN_2
#define KEY0_GPIO_Port GPIOE
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY2_Pin GPIO_PIN_4
#define KEY2_GPIO_Port GPIOE
#define BEEP_Pin GPIO_PIN_0
#define BEEP_GPIO_Port GPIOF
#define SHUTDOWN2_Pin GPIO_PIN_2
#define SHUTDOWN2_GPIO_Port GPIOF
#define SHUTDOWN1_Pin GPIO_PIN_10
#define SHUTDOWN1_GPIO_Port GPIOF
#define HALL1_TIM_CH1_Pin GPIO_PIN_10
#define HALL1_TIM_CH1_GPIO_Port GPIOH
#define HALL1_TIM_CH2_Pin GPIO_PIN_11
#define HALL1_TIM_CH2_GPIO_Port GPIOH
#define HALL1_TIM_CH3_Pin GPIO_PIN_12
#define HALL1_TIM_CH3_GPIO_Port GPIOH
#define M1_LOW_SIDE_U_Pin GPIO_PIN_13
#define M1_LOW_SIDE_U_GPIO_Port GPIOB
#define M1_LOW_SIDE_V_Pin GPIO_PIN_14
#define M1_LOW_SIDE_V_GPIO_Port GPIOB
#define M1_LOW_SIDE_W_Pin GPIO_PIN_15
#define M1_LOW_SIDE_W_GPIO_Port GPIOB
#define HALL2_TIM_CH1_Pin GPIO_PIN_12
#define HALL2_TIM_CH1_GPIO_Port GPIOD
#define HALL2_TIM_CH2_Pin GPIO_PIN_13
#define HALL2_TIM_CH2_GPIO_Port GPIOD
#define M2_LOW_SIDE_U_Pin GPIO_PIN_13
#define M2_LOW_SIDE_U_GPIO_Port GPIOH
#define M2_LOW_SIDE_V_Pin GPIO_PIN_14
#define M2_LOW_SIDE_V_GPIO_Port GPIOH
#define M2_LOW_SIDE_W_Pin GPIO_PIN_15
#define M2_LOW_SIDE_W_GPIO_Port GPIOH
#define BRAKE1_Pin GPIO_PIN_4
#define BRAKE1_GPIO_Port GPIOB
#define BRAKE2_Pin GPIO_PIN_5
#define BRAKE2_GPIO_Port GPIOB
#define HALL2_TIM_CH3_Pin GPIO_PIN_8
#define HALL2_TIM_CH3_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_0
#define LED0_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

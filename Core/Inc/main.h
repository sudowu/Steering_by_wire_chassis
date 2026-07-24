/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#define BEEP_Pin GPIO_PIN_0
#define BEEP_GPIO_Port GPIOF
#define PM2_CTRL_SD_Pin GPIO_PIN_2
#define PM2_CTRL_SD_GPIO_Port GPIOF
#define PM1_CTRL_SD_Pin GPIO_PIN_10
#define PM1_CTRL_SD_GPIO_Port GPIOF
#define PM1_AMPW_Pin GPIO_PIN_3
#define PM1_AMPW_GPIO_Port GPIOA
#define PM1_AMPV_Pin GPIO_PIN_6
#define PM1_AMPV_GPIO_Port GPIOA
#define PM1_AMPU_Pin GPIO_PIN_0
#define PM1_AMPU_GPIO_Port GPIOB
#define PM1_HALLU_Pin GPIO_PIN_10
#define PM1_HALLU_GPIO_Port GPIOH
#define PM1_HALLV_Pin GPIO_PIN_11
#define PM1_HALLV_GPIO_Port GPIOH
#define PM1_HALLW_Pin GPIO_PIN_12
#define PM1_HALLW_GPIO_Port GPIOH
#define PM1_PWM_UL_Pin GPIO_PIN_13
#define PM1_PWM_UL_GPIO_Port GPIOB
#define PM1_PWM_VL_Pin GPIO_PIN_14
#define PM1_PWM_VL_GPIO_Port GPIOB
#define PM1_PWM_WL_Pin GPIO_PIN_15
#define PM1_PWM_WL_GPIO_Port GPIOB
#define PM2_HALLU_Pin GPIO_PIN_12
#define PM2_HALLU_GPIO_Port GPIOD
#define PM2_HALLV_Pin GPIO_PIN_13
#define PM2_HALLV_GPIO_Port GPIOD
#define PM2_PWM_UL_Pin GPIO_PIN_13
#define PM2_PWM_UL_GPIO_Port GPIOH
#define PM2_PWM_VL_Pin GPIO_PIN_14
#define PM2_PWM_VL_GPIO_Port GPIOH
#define PM2_PWM_WL_Pin GPIO_PIN_15
#define PM2_PWM_WL_GPIO_Port GPIOH
#define PARK_RIGHT_Pin GPIO_PIN_4
#define PARK_RIGHT_GPIO_Port GPIOB
#define PARK_LEFT_Pin GPIO_PIN_5
#define PARK_LEFT_GPIO_Port GPIOB
#define PM2_HALLW_Pin GPIO_PIN_8
#define PM2_HALLW_GPIO_Port GPIOB
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

/**
  ******************************************************************************
  * @file    iwdg.h
  * @brief   IWDG driver header for STM32F4xx.
  ******************************************************************************
  */

#ifndef __IWDG_H
#define __IWDG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @defgroup IWDG_Prescaler IWDG Prescaler values */
#define IWDG_PRESCALER_4    0   /*!< IWDG prescaler set to 4 */
#define IWDG_PRESCALER_8    1   /*!< IWDG prescaler set to 8 */
#define IWDG_PRESCALER_16   2   /*!< IWDG prescaler set to 16 */
#define IWDG_PRESCALER_32   3   /*!< IWDG prescaler set to 32 */
#define IWDG_PRESCALER_64   4   /*!< IWDG prescaler set to 64 */
#define IWDG_PRESCALER_128  5   /*!< IWDG prescaler set to 128 */
#define IWDG_PRESCALER_256  6   /*!< IWDG prescaler set to 256 */

/**
  * @brief IWDG Init structure definition
  */
typedef struct
{
  uint32_t Prescaler;  /*!< Specifies the prescaler */
  uint32_t Reload;     /*!< Specifies the IWDG down-counter reload value */
} IWDG_InitTypeDef;

/**
  * @brief IWDG handle Structure definition
  */
typedef struct
{
  IWDG_TypeDef        *Instance;  /*!< Register base address */
  IWDG_InitTypeDef    Init;       /*!< IWDG required parameters */
} IWDG_HandleTypeDef;

/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef HAL_IWDG_Init(IWDG_HandleTypeDef *hiwdg);
HAL_StatusTypeDef HAL_IWDG_Refresh(IWDG_HandleTypeDef *hiwdg);

#ifdef __cplusplus
}
#endif

#endif /* __IWDG_H */

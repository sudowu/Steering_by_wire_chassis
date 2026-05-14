/**
  ******************************************************************************
  * @file    iwdg.c
  * @brief   IWDG driver for STM32F4xx.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "iwdg.h"

/**
  * @brief  Initializes the IWDG according to the specified parameters.
  * @param  hiwdg  pointer to a IWDG_HandleTypeDef structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IWDG_Init(IWDG_HandleTypeDef *hiwdg)
{
  uint32_t tickstart;

  /* Check the IWDG handle allocation */
  if (hiwdg == NULL)
  {
    return HAL_ERROR;
  }

  /* Check the parameters */
  if (hiwdg->Init.Prescaler > 7 || hiwdg->Init.Reload > 0x0FFF)
  {
    return HAL_ERROR;
  }

  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  hiwdg->Instance->KR = 0x5555;

  /* Write to IWDG registers the Prescaler & Reload values */
  hiwdg->Instance->PR = hiwdg->Init.Prescaler;
  hiwdg->Instance->RLR = hiwdg->Init.Reload;

  /* Reload IWDG counter with value defined in the reload register */
  hiwdg->Instance->KR = 0xAAAA;

  /* Wait for the IWDG to be ready */
  tickstart = HAL_GetTick();
  while ((hiwdg->Instance->SR & 0x01) != 0U)  /* PVU bit */
  {
    if ((HAL_GetTick() - tickstart) > 1000U)
    {
      return HAL_TIMEOUT;
    }
  }

  /* Enable the IWDG peripheral */
  hiwdg->Instance->KR = 0xCCCC;

  /* Return function status */
  return HAL_OK;
}

/**
  * @brief  Refreshes the IWDG.
  * @param  hiwdg  pointer to a IWDG_HandleTypeDef structure
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_IWDG_Refresh(IWDG_HandleTypeDef *hiwdg)
{
  /* Reload IWDG counter with value defined in the reload register */
  hiwdg->Instance->KR = 0xAAAA;

  /* Return function status */
  return HAL_OK;
}

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pwm_duty_analyzer.h"
#include "stm32f407xx.h"
#include "usart.h"
#include "bldc.h"
#include "chassis.h"
#include "stdio.h"
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc3;
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim8;
extern UART_HandleTypeDef huart1;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
  uint32_t timeout = 0;
  uint32_t maxDelay = 0x1FFFF;
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
  timeout = 0;
  while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY) 
  {
    timeout++;
    if(timeout > maxDelay) 
    {
      break;
    }
  }
  timeout = 0;
  while (HAL_UART_Receive_IT(&huart1, g_rx_buffer, RXBUFFERSIZE) != HAL_OK) 
  {
    timeout++;
    if(timeout > maxDelay) 
    {
      break;
    }
    
  }
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles TIM8 update interrupt and TIM13 global interrupt.
  */
void TIM8_UP_TIM13_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 0 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 0 */
  HAL_TIM_IRQHandler(&htim8);
  /* USER CODE BEGIN TIM8_UP_TIM13_IRQn 1 */

  /* USER CODE END TIM8_UP_TIM13_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC1 and DAC2 underrun error interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */

  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */
  
  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc3);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  uint8_t bldc_dir = 0;
  if (htim->Instance == TIM1) {
    if (g_bldc_motor1.run_flag == RUN) {

      // 读取霍尔值获取转子位置
      if (g_bldc_motor1.dir == CW) {
        g_bldc_motor1.step_sta = hallsensor_get_state(MOTOR_1);
      } else if (g_bldc_motor1.dir == CCW) {
        g_bldc_motor1.step_sta = 7 - hallsensor_get_state(MOTOR_1);
      }
      // 判断霍尔值是否正常，驱动电机1
      if (g_bldc_motor1.step_sta <= 6 && g_bldc_motor1.step_sta >= 1) {
        pfunclist_m1[g_bldc_motor1.step_sta - 1]();
      } else {
        stop_motor1();
        g_bldc_motor1.run_flag = STOP;
        g_bldc_motor1.pwm_duty = 0;
      }
      // 如果读取霍尔值不同则进行换向
      if (g_bldc_motor1.step_sta != g_bldc_motor1.step_last) {
        g_bldc_motor1.hall_keep_t = 0;
        bldc_dir = check_hall_dir(&g_bldc_motor1);
        if (bldc_dir == CW) {
          g_bldc_motor1.pos += 1;
        } else if (bldc_dir == CCW) {
          g_bldc_motor1.pos -= 1;
        }
        g_bldc_motor1.step_last = g_bldc_motor1.step_sta;
      }
      // else if (g_bldc_motor1.run_flag == RUN) {
      //   g_bldc_motor1.hall_keep_t++;
      // }
    }
  }
  // 电机2
  else if (htim->Instance == TIM8) {
    if (g_bldc_motor2.run_flag == RUN) {
      // 电机1实际占空比控制

      // 读取霍尔值获取转子位置
      if (g_bldc_motor2.dir == CW) {
        g_bldc_motor2.step_sta = hallsensor_get_state(MOTOR_2);
      } else {
        g_bldc_motor2.step_sta = 7 - hallsensor_get_state(MOTOR_2);
      }
      // 判断霍尔值是否正常，驱动电机2
      if (g_bldc_motor2.step_sta <= 6 && g_bldc_motor2.step_sta >= 1) {
        pfunclist_m2[g_bldc_motor2.step_sta - 1]();
      } else {
        stop_motor2();
        g_bldc_motor2.run_flag = STOP;
        g_bldc_motor2.pwm_duty = 0;
      }
      // 如果读取霍尔值不同则进行换向
      if (g_bldc_motor2.step_sta != g_bldc_motor2.step_last) {
        g_bldc_motor2.hall_keep_t = 0;
        bldc_dir = check_hall_dir(&g_bldc_motor2);
        if (bldc_dir == CW) {
          g_bldc_motor2.pos += 1;
        } else if (bldc_dir == CCW) {
          g_bldc_motor2.pos -= 1;
        }
        g_bldc_motor2.step_last = g_bldc_motor2.step_sta;
      }
      // else if (g_bldc_motor2.run_flag == RUN) {
      //   g_bldc_motor2.hall_keep_t++;
      // }
    }
  } else if (htim->Instance == TIM6) {
    // 电机1实际占空比控制

    // 电机换向控制
    if (g_bldc_motor1.pwm_duty == 0 && g_bldc_motor1.pwm_duty_target == 0) {
      HAL_GPIO_WritePin(BRAKE1_GPIO_Port, BRAKE1_Pin, GPIO_PIN_SET);
    }
    else {
      HAL_GPIO_WritePin(BRAKE1_GPIO_Port, BRAKE1_Pin, GPIO_PIN_RESET);
    }
    if (g_bldc_motor2.pwm_duty == 0 && g_bldc_motor2.pwm_duty_target == 0) {
      HAL_GPIO_WritePin(BRAKE2_GPIO_Port, BRAKE2_Pin, GPIO_PIN_SET);
    }
    else {
      HAL_GPIO_WritePin(BRAKE2_GPIO_Port, BRAKE2_Pin, GPIO_PIN_RESET);
    }
    if (g_bldc_motor1.dir != g_bldc_motor1.dir_set) {
      if (g_bldc_motor1.pwm_duty == 0) {
        g_bldc_motor1.dir = g_bldc_motor1.dir_set;
      } else {
        g_bldc_motor1.pwm_duty_target = 0;
      }
    }
    if (g_bldc_motor2.dir != g_bldc_motor2.dir_set) {
      if (g_bldc_motor2.pwm_duty == 0) {
        g_bldc_motor2.dir = g_bldc_motor2.dir_set;
      } else {
        g_bldc_motor2.pwm_duty_target = 0;
      }
    }

    if (g_bldc_motor1.valid_data_num <= 0 ||
        g_bldc_motor2.valid_data_num <= 0) {
      g_bldc_motor1.pwm_duty_target = 0;
      g_bldc_motor2.pwm_duty_target = 0;
    } else {
      g_bldc_motor1.valid_data_num--;
    }

    // 电机限速控制
    if (g_bldc_motor1.pwm_duty_target > (MAX_PWM_DUTY / 2) ||
        g_bldc_motor1.pwm_duty_target < -(MAX_PWM_DUTY / 2)) {
      g_bldc_motor1.pwm_duty_target = g_bldc_motor1.pwm_duty;
    }
    if (g_bldc_motor2.pwm_duty_target > (MAX_PWM_DUTY / 2) ||
        g_bldc_motor2.pwm_duty_target < -(MAX_PWM_DUTY / 2)) {
      g_bldc_motor2.pwm_duty_target = g_bldc_motor2.pwm_duty;
    }

    // 电机缓加速缓减速控制
    if (g_bldc_motor1.pwm_duty_target > g_bldc_motor1.pwm_duty) {
      g_bldc_motor1.pwm_duty += DUTY_STEP_UP;
    } else if (g_bldc_motor1.pwm_duty_target < g_bldc_motor1.pwm_duty) {
      g_bldc_motor1.pwm_duty -= DUTY_STEP_DOWN;
    }
    if (g_bldc_motor2.pwm_duty_target > g_bldc_motor2.pwm_duty) {
      g_bldc_motor2.pwm_duty += DUTY_STEP_UP;
    } else if (g_bldc_motor2.pwm_duty_target < g_bldc_motor2.pwm_duty) {
      g_bldc_motor2.pwm_duty -= DUTY_STEP_DOWN;
    }
    // HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);
  } else if (htim->Instance == TIM7) {
    uint32_t ch2 = 0, ch4 = 0;
    ch2 = s_chassis.remote_count_ch2;
    ch4 = s_chassis.remote_count_ch4;
    s_chassis.remote_count_ch2 = 0;
    s_chassis.remote_count_ch4 = 0;
    printf("ch2:%ld\r\n", ch2);
    printf("ch4:%ld\r\n", ch4);
    // HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);
  } else if (htim->Instance == TIM3) {
    if (HAL_GPIO_ReadPin(REMOTE_CH2_GPIO_Port, REMOTE_CH2_Pin) ==
        GPIO_PIN_RESET) {
      s_chassis.remote_count_ch2++;
    }
    if (HAL_GPIO_ReadPin(REMOTE_CH4_GPIO_Port, REMOTE_CH4_Pin) ==
        GPIO_PIN_RESET) {
      s_chassis.remote_count_ch4++;
      
    }
    // HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin);
  }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  static uint32_t count = 0;
  if (hadc->Instance == ADC3 && flag_adc_dma == 0) {
    adc_buffer1[count] = dma_buffer[0];
    adc_buffer2[count++] = dma_buffer[1];
    if (count >= PWM_ADC_BUFFER_SIZE) {
      flag_adc_dma = 1;
      count = 0;
    }
  }
}

// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
//   if (GPIO_Pin == REMOTE_CH2_EXTI_Pin) {
//     s_chassis.remote_count_ch2++;
//   } else if (GPIO_Pin == REMOTE_CH4_EXTI_Pin) {
//     s_chassis.remote_count_ch4++;
//   }
// }
/* USER CODE END 1 */

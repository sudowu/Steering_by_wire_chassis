/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bldc.h"
#include "chassis.h"
#include "key.h"
#include "led.h"
#include "stdio.h"
#include <stdint.h>
#include <sys/_intsup.h>
#include "pwm_duty_analyzer.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  int32_t remote_2 = 0, remote_4 = 0; 
  uint32_t remote_zero = 0;
  uint8_t t = 0;
  int16_t pwm_duty_temp = 0;
  int16_t pwm_duty_last = 0;
  uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_USART1_UART_Init();
  MX_CAN1_Init();
  MX_TIM6_Init();
  MX_ADC3_Init();
  /* USER CODE BEGIN 2 */
  bldc_ctrl(MOTOR_1, CCW, 0);
  bldc_ctrl(MOTOR_2, CCW, 0);
  
  
  // printf("按下KEY0 开始正转加速\r\n");
  // printf("按下KEY1 开始反转加速\r\n");
  // printf("按下KEY2 停止电机\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // while (adc_pwm1_hight_count != 143);
  // while (adc_pwm2_hight_count != 143);  
  while (1) {
    t++;
    if (t == 20) {
      LED0_TOGGLE();
      CAN_Send_HAL(0x01, data, 8);
      t = 0;
      // printf("remote ch2：%d ch4：%d\r\n", adc_pwm2_hight_count,adc_pwm1_hight_count);

    }
    if (pwm_duty_last != pwm_duty_temp) {
      pwm_duty_last = pwm_duty_temp;
      printf("pwm:%d\r\n", pwm_duty_last);
    }
    if (flag_adc_dma == 1) {
      flag_adc_dma = 0;
      // HAL_ADC_Start_DMA(&hadc3, dma_buffer, sizeof(dma_buffer)/sizeof(uint32_t));
    }
    if (remote_zero == 0 && adc_pwm1_hight_count > 140&& adc_pwm1_hight_count < 150) {
      remote_zero = adc_pwm1_hight_count ;
    }
    remote_2 = remote_zero-adc_pwm1_hight_count;
    remote_4 = remote_zero-adc_pwm2_hight_count;
    // remote_2 = 0;
    // remote_4 = 0;
    if (remote_2 < 60 && remote_2 > -60 && (remote_2 > 5 || remote_2 < -5)) {
      s_chassis.velocity_x = remote_2 * 20;
      printf("speed1:%d\r\n", s_chassis.velocity_x);
    }
    else if (remote_2 < 5 && remote_2 > -5){
      s_chassis.velocity_x = 0;
    }
    if (remote_4 < 60 && remote_4 > -60 && (remote_4 > 5 || remote_4 < -5)) {
      s_chassis.velocity_z = remote_4 * 20;
      printf("speed2:%d\r\n", s_chassis.velocity_z);
    }
    else if (remote_4 < 5 && remote_4 > -5){
      s_chassis.velocity_z = 0;
    }
    chassis_control(&s_chassis, remote_signal); 

    // key = key_scan();
    // if (key == KEY0_PRES)           //按下key0占空比++
    // {
    //   pwm_duty_temp += 500;
    //   if (pwm_duty_temp > MAX_PWM_DUTY/2)
    //     pwm_duty_temp = pwm_duty_last;
    //   if (pwm_duty_temp > 0) {
    //     g_bldc_motor1.pwm_duty_target = pwm_duty_temp;
    //     g_bldc_motor1.dir_set = CW;
    //     g_bldc_motor2.pwm_duty_target = pwm_duty_temp;
    //     g_bldc_motor2.dir_set = CW;
    //   }
    //   else {
    //     g_bldc_motor1.pwm_duty_target = -pwm_duty_temp;
    //     g_bldc_motor1.dir_set = CCW;
    //     g_bldc_motor2.pwm_duty_target = -pwm_duty_temp;
    //     g_bldc_motor2.dir_set = CCW;
    //   }
    //   g_bldc_motor1.run_flag = RUN;
    //   g_bldc_motor2.run_flag = RUN;
    //   start_motor1();
    //   start_motor2();
    // }
    // else if (key == KEY1_PRES)      //按下key1占空比--
    // {
    //   pwm_duty_temp -= 500;
    //   if (pwm_duty_temp < -(MAX_PWM_DUTY/2))
    //     pwm_duty_temp = pwm_duty_last;
    //   if (pwm_duty_temp > 0) {
    //     g_bldc_motor1.pwm_duty_target = pwm_duty_temp;
    //     g_bldc_motor1.dir_set = CW;
    //     g_bldc_motor2.pwm_duty_target = pwm_duty_temp;
    //     g_bldc_motor2.dir_set = CW;
    //   }
    //   else {
    //     g_bldc_motor1.pwm_duty_target = -pwm_duty_temp;
    //     g_bldc_motor1.dir_set = CCW;
    //     g_bldc_motor2.pwm_duty_target = -pwm_duty_temp;
    //     g_bldc_motor2.dir_set = CCW;
    //   }
    //   g_bldc_motor1.run_flag = RUN;
    //   g_bldc_motor2.run_flag = RUN;
    //   start_motor1();
    //   start_motor2();
    // }
    // else if (key == KEY2_PRES)      //按下key0停止电机
    // {
    //   pwm_duty_temp = 0;
    //   g_bldc_motor1.pwm_duty_target = 0;
    //   g_bldc_motor2.pwm_duty_target = 0;
    //   // g_bldc_motor1.run_flag = STOP;

    //   stop_motor1();
    //   stop_motor2();
    //   // g_bldc_motor2.run_flag = STOP;

    // }
    HAL_Delay(10);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

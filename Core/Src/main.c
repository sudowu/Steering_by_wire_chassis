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

    int32_t remote_2 = 0, remote_4 = 0;      /* 遥控器通道2和通道4的偏差值 */
    uint32_t remote_zero = 0;                /* 遥控器零点基准值 */
    uint8_t t = 0;                           /* 计时计数器，用于LED闪烁和CAN通信定时 */
    int16_t pwm_duty_temp = 0;               /* 临时PWM占空比值 */
    int16_t pwm_duty_last = 0;               /* 上次PWM占空比值，用于检测变化 */
    uint8_t data[8] = {0};                   /* CAN通信测试数据 */
    uint8_t data1[8] = {0};                   /* CAN通信测试数据 */
    uint8_t remote_zero_count = 10;          /* 遥控器零点校准计数器 */
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
    // 初始化两个电机，设置方向为逆时针(CCW)，占空比为0，即停止状态
    bldc_ctrl(MOTOR_1, CCW, 0);
    bldc_ctrl(MOTOR_2, CCW, 0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    /* 主控制循环 */
    while (1)
    {
        // 计数器递增，用于计时控制
        t++;
        // 每20个循环周期（200ms）执行一次LED闪烁和CAN通信
        if (t == 20)
        {
            LED0_TOGGLE();                    // 翻转LED0的状态（闪烁指示系统运行）
            data[0] = (s_chassis.bldc1->pwm_duty) & 0xff;
            data[1] = (s_chassis.bldc1->pwm_duty) >> 8;
            data[2] = (s_chassis.bldc2->pwm_duty) & 0xff;
            data[3] = (s_chassis.bldc2->pwm_duty) >> 8;
            data[4] = (adc_pwm1_hight_count & 0xff);
            data[5] = (adc_pwm1_hight_count >> 8);
            data[6] = (adc_pwm2_hight_count & 0xff);
            data[7] = (adc_pwm2_hight_count >> 8);
            CAN_Send_HAL(0x09, data, 8);      // 通过CAN总线发送ID为0x09的心跳包，包含8字节数据

            data1[0] = HAL_GPIO_ReadPin(BRAKE1_GPIO_Port, BRAKE1_Pin);
            data1[1] = HAL_GPIO_ReadPin(BRAKE2_GPIO_Port, BRAKE2_Pin);
            CAN_Send_HAL(0x0A, data1, 8);      // 通过CAN总线发送ID为0x09的心跳包，包含8字节数据
            t = 0;                            // 重置计数器
        }
        // 检测PWM占空比是否有变化，如有变化则输出当前PWM值到串口
        if (pwm_duty_last != pwm_duty_temp)
        {
            pwm_duty_last = pwm_duty_temp;      // 更新上次的PWM占空比值
            printf("pwm:%d\r\n", pwm_duty_last); // 串口打印当前PWM值，用于调试
        }
        // 如果ADC DMA转换完成标志被置位，则清除该标志
        if (flag_adc_dma == 1)
        {
            flag_adc_dma = 0;                   // 清除ADC DMA转换完成标志
        }
        // 校准遥控器零点：当remote_zero为0且adc_pwm1_hight_count在140-170范围内且remote_zero_count不为0时
        if (adc_pwm1_hight_count > 140 &&
            adc_pwm1_hight_count < 170 && remote_zero_count != 0)
        {
            // 进行10次采样取平均值作为遥控器中位点
            if (remote_zero_count == 10)
            {
                remote_zero = adc_pwm1_hight_count; // 第一次记录当前ADC值作为基准
            }
            else
            {
                remote_zero = (adc_pwm1_hight_count + remote_zero) / 2; // 后续取平均值优化精度
            }
            remote_zero_count--; // 减少计数，进行下一次校准采样
        }
        // 计算遥控器通道2和通道4的偏差值（相对于零点）
        remote_2 = remote_zero - adc_pwm1_hight_count; // 通道2偏差，通常对应前进/后退
        remote_4 = remote_zero - adc_pwm2_hight_count; // 通道4偏差，通常对应左转/右转
        // 处理通道2的输入：计算X轴速度分量（前进/后退速度）
        if (remote_2 < 60 && remote_2 > -60 && (remote_2 > 5 || remote_2 < -5))
        {
            // 当偏差在合理范围内且不接近零点时，设置X轴速度（放大25倍）
            s_chassis.velocity_x = remote_2 * 25;  // 设置底盘X轴速度（前进/后退）
            printf("speed1:%d\r\n", s_chassis.velocity_x); // 输出X轴速度，用于调试
        }
        else if (remote_2 < 5 && remote_2 > -5)
        {
            // 当偏差非常小时，将X轴速度设为0（消除抖动和噪声）
            s_chassis.velocity_x = 0;
        }
        // 处理通道4的输入：计算Z轴速度分量（转向速度）
        if (remote_4 < 60 && remote_4 > -60 && (remote_4 > 5 || remote_4 < -5))
        {
            // 当偏差在合理范围内且不接近零点时，设置Z轴速度（放大25倍）
            s_chassis.velocity_z = remote_4 * 25;  // 设置底盘Z轴速度（转向）
            printf("speed2:%d\r\n", s_chassis.velocity_z); // 输出Z轴速度，用于调试
        }
        else if (remote_4 < 5 && remote_4 > -5)
        {
            // 当偏差非常小时，将Z轴速度设为0（消除抖动和噪声）
            s_chassis.velocity_z = 0;
        }
        // 根据计算出的速度分量执行底盘运动控制
        chassis_control(&s_chassis, remote_signal);  // 执行底盘差速驱动控制算法

        HAL_Delay(10); // 延时10毫秒，控制主循环频率，约100Hz的控制频率


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
    while (1)
    {
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

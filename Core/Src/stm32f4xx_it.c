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
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "bldc.h"
#include "chassis.h"
#include "pwm_duty_analyzer.h"
#include "stdio.h"
#include "stm32f407xx.h"
#include "usart.h"
#include <stdint.h>
#include "can.h"
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
        if (timeout > maxDelay)
        {
            break;
        }
    }
    timeout = 0;
    while (HAL_UART_Receive_IT(&huart1, g_rx_buffer, RXBUFFERSIZE) != HAL_OK)
    {
        timeout++;
        if (timeout > maxDelay)
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

/**
 * @brief 定时器周期溢出回调函数
 * @param htim 指向定时器句柄的指针
 * @retval None
 * 
 * @details
 * 该函数处理三个定时器的中断任务：
 * - TIM1: 控制电机1的换向逻辑
 * - TIM8: 控制电机2的换向逻辑
 * - TIM6: 处理电机的刹车、方向切换、信号超时和速度控制
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    uint8_t bldc_dir = 0;
    
    // 处理定时器1中断 - 控制电机1
    if (htim->Instance == TIM1)
    {
        if (g_bldc_motor1.run_flag == RUN)
        {
            // 读取霍尔传感器值以获取转子位置
            if (g_bldc_motor1.dir == CW)
            {
                // 顺时针方向时直接读取霍尔值
                g_bldc_motor1.step_sta = hallsensor_get_state(MOTOR_1);
            }
            else if (g_bldc_motor1.dir == CCW)
            {
                // 逆时针方向时对霍尔值取反（通过7减去原值）
                g_bldc_motor1.step_sta = 7 - hallsensor_get_state(MOTOR_1);
            }
            // 判断霍尔值是否在有效范围内(1-6)，驱动电机1
            if (g_bldc_motor1.step_sta <= 6 && g_bldc_motor1.step_sta >= 1)
            {
                // 根据霍尔状态执行相应的换向函数
                pfunclist_m1[g_bldc_motor1.step_sta - 1]();
            }
            else
            {
                // 霍尔值异常时停止电机
                stop_motor1();
                g_bldc_motor1.run_flag = STOP;
                g_bldc_motor1.pwm_duty = 0;
            }
            // 如果当前霍尔值与上一次不同则进行换向处理
            if (g_bldc_motor1.step_sta != g_bldc_motor1.step_last)
            {
                g_bldc_motor1.hall_keep_t = 0;
                bldc_dir = check_hall_dir(&g_bldc_motor1);
                if (bldc_dir == CW)
                {
                    g_bldc_motor1.pos += 1;  // 顺时针旋转，位置+1
                }
                else if (bldc_dir == CCW)
                {
                    g_bldc_motor1.pos -= 1;  // 逆时针旋转，位置-1
                }
                g_bldc_motor1.step_last = g_bldc_motor1.step_sta;  // 更新上次霍尔值
            }
            // else if (g_bldc_motor1.run_flag == RUN) {
            //   g_bldc_motor1.hall_keep_t++;
            // }
        }
    }
    // 处理定时器8中断 - 控制电机2
    else if (htim->Instance == TIM8)
    {
        if (g_bldc_motor2.run_flag == RUN)
        {
            // 电机2实际占空比控制

            // 读取霍尔传感器值以获取转子位置
            if (g_bldc_motor2.dir == CW)
            {
                // 顺时针方向时直接读取霍尔值
                g_bldc_motor2.step_sta = hallsensor_get_state(MOTOR_2);
            }
            else
            {
                // 逆时针方向时对霍尔值取反
                g_bldc_motor2.step_sta = 7 - hallsensor_get_state(MOTOR_2);
            }
            // 判断霍尔值是否在有效范围内(1-6)，驱动电机2
            if (g_bldc_motor2.step_sta <= 6 && g_bldc_motor2.step_sta >= 1)
            {
                // 根据霍尔状态执行相应的换向函数
                pfunclist_m2[g_bldc_motor2.step_sta - 1]();
            }
            else
            {
                // 霍尔值异常时停止电机
                stop_motor2();
                g_bldc_motor2.run_flag = STOP;
                g_bldc_motor2.pwm_duty = 0;
            }
            // 如果当前霍尔值与上一次不同则进行换向处理
            if (g_bldc_motor2.step_sta != g_bldc_motor2.step_last)
            {
                g_bldc_motor2.hall_keep_t = 0;
                bldc_dir = check_hall_dir(&g_bldc_motor2);
                if (bldc_dir == CW)
                {
                    g_bldc_motor2.pos += 1;  // 顺时针旋转，位置+1
                }
                else if (bldc_dir == CCW)
                {
                    g_bldc_motor2.pos -= 1;  // 逆时针旋转，位置-1
                }
                g_bldc_motor2.step_last = g_bldc_motor2.step_sta;  // 更新上次霍尔值
            }
            // else if (g_bldc_motor2.run_flag == RUN) {
            //   g_bldc_motor2.hall_keep_t++;
            // }
        }
    }
    // 处理定时器6中断 - 执行电机的高级控制功能
    else if (htim->Instance == TIM6)
    {
        // 电机刹车控制
        if (g_bldc_motor1.pwm_duty == 0 && g_bldc_motor1.pwm_duty_target == 0)
        {
            // 当电机1目标占空比和当前占空比都为0时，激活刹车
            HAL_GPIO_WritePin(BRAKE1_GPIO_Port, BRAKE1_Pin, GPIO_PIN_SET);
        }
        else
        {
            // 否则释放刹车
            HAL_GPIO_WritePin(BRAKE1_GPIO_Port, BRAKE1_Pin, GPIO_PIN_RESET); //解除刹车
        }
        if (g_bldc_motor2.pwm_duty == 0 && g_bldc_motor2.pwm_duty_target == 0)
        {
            // 当电机2目标占空比和当前占空比都为0时，激活刹车
            HAL_GPIO_WritePin(BRAKE2_GPIO_Port, BRAKE2_Pin, GPIO_PIN_SET);
        }
        else
        {
            // 否则释放刹车
            HAL_GPIO_WritePin(BRAKE2_GPIO_Port, BRAKE2_Pin, GPIO_PIN_RESET); //解除刹车
        }
        
        // 电机方向切换控制
        if (g_bldc_motor1.dir != g_bldc_motor1.dir_set)
        {
            if (g_bldc_motor1.pwm_duty == 0)
            {
                // 只有当电机1当前占空比为0时才改变方向
                g_bldc_motor1.dir = g_bldc_motor1.dir_set;
            }
            else
            {
                // 否则先将目标占空比设为0，等待电机停稳后再切换方向
                g_bldc_motor1.pwm_duty_target = 0;
            }
        }
        if (g_bldc_motor2.dir != g_bldc_motor2.dir_set)
        {
            if (g_bldc_motor2.pwm_duty == 0)
            {
                // 只有当电机2当前占空比为0时才改变方向
                g_bldc_motor2.dir = g_bldc_motor2.dir_set;
            }
            else
            {
                // 否则先将目标占空比设为0，等待电机停稳后再切换方向
                g_bldc_motor2.pwm_duty_target = 0;
            }
        }

        // 信号超时控制：当CAN和遥控信号都超时时，停止电机
        if (s_chassis.valid_can_num <= 0 && s_chassis.valid_remote_num <= 0)
        {
            g_bldc_motor1.pwm_duty_target = 0;
            g_bldc_motor2.pwm_duty_target = 0;
        }
        else if (s_chassis.valid_can_num > 0)
        {
            s_chassis.valid_can_num--;  // CAN信号有效计数递减
        }
        else if (s_chassis.valid_remote_num > 0)
        {
            s_chassis.valid_remote_num--;  // 遥控信号有效计数递减
        }

        // 电机限速控制：限制目标占空比在安全范围内
        if (g_bldc_motor1.pwm_duty_target > (MAX_PWM_DUTY / 2) ||
            g_bldc_motor1.pwm_duty_target < -(MAX_PWM_DUTY / 2))
        {
            g_bldc_motor1.pwm_duty_target = g_bldc_motor1.pwm_duty;  // 超限时保持当前值
        }
        if (g_bldc_motor2.pwm_duty_target > (MAX_PWM_DUTY / 2) ||
            g_bldc_motor2.pwm_duty_target < -(MAX_PWM_DUTY / 2))
        {
            g_bldc_motor2.pwm_duty_target = g_bldc_motor2.pwm_duty;  // 超限时保持当前值
        }

        // 电机缓加速缓减速控制：实现平滑的速度过渡
        if (g_bldc_motor1.pwm_duty_target > g_bldc_motor1.pwm_duty)
        {
            // 目标值大于当前值时，逐步增加当前值
            g_bldc_motor1.pwm_duty += DUTY_STEP_UP;
        }
        else if (g_bldc_motor1.pwm_duty_target < g_bldc_motor1.pwm_duty)
        {
            // 目标值小于当前值时，逐步减少当前值
            g_bldc_motor1.pwm_duty -= DUTY_STEP_DOWN;
        }
        if (g_bldc_motor2.pwm_duty_target > g_bldc_motor2.pwm_duty)
        {
            // 目标值大于当前值时，逐步增加当前值
            g_bldc_motor2.pwm_duty += DUTY_STEP_UP;
        }
        else if (g_bldc_motor2.pwm_duty_target < g_bldc_motor2.pwm_duty)
        {
            // 目标值小于当前值时，逐步减少当前值
            g_bldc_motor2.pwm_duty -= DUTY_STEP_DOWN;
        }
    }

}

/**
 * @brief ADC转换完成回调函数
 * @param hadc 指向ADC句柄的指针
 * @retval None
 * 
 * @details
 * 该函数处理ADC3的DMA转换完成事件，通过统计高电平持续时间
 * 来测量PWM信号的占空比，并重新启动DMA传输以实现连续采样。
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // static uint32_t count = 0;
    static uint32_t count1 = 0;  // 统计第一个ADC通道高电平持续时间
    static uint32_t count2 = 0;  // 统计第二个ADC通道高电平持续时间
    
    if (hadc->Instance == ADC3)
    {
        // adc_buffer1[count] = dma_buffer[0] & 0xff;
        // adc_buffer2[count++] = dma_buffer[1] & 0xff;
        
        // 分析第一个ADC通道的值，判断是否为高电平
        if (dma_buffer[0] > 250)
        {
            count1++;  // 高电平时计数器递增
        }
        else if (count1 != 0)
        {
            // 低电平且计数器非零时，保存高电平持续时间
            // flag_adc_dma = 1;
            adc_pwm1_hight_count = count1;  // 记录PWM1的高电平计数
            count1 = 0;  // 重置计数器
        }

        // 分析第二个ADC通道的值，判断是否为高电平
        if (dma_buffer[1] > 250)
        {
            count2++;  // 高电平时计数器递增
        }
        else if (count2 != 0)
        {
            // 低电平且计数器非零时，保存高电平持续时间
            // flag_adc_dma = 1;
            adc_pwm2_hight_count = count2;  // 记录PWM2的高电平计数
            count2 = 0;  // 重置计数器
        }
        // if (count >= PWM_ADC_BUFFER_SIZE) {
        //   count = 0;
        // } else {
        // 重新启动DMA传输以继续采集数据
        HAL_ADC_Start_DMA(&hadc3, dma_buffer,
                          sizeof(dma_buffer) / sizeof(uint32_t));
        // }
    }
}

/**
 * @brief CAN接收FIFO0消息挂起回调函数
 * @param hcan 指向CAN句柄的指针
 * @retval None
 * 
 * @details
 * 该函数处理CAN1接收到的消息，当收到ID为0x08的标准帧时，
 * 解析其中的速度指令并更新底盘控制参数。
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (hcan->Instance == CAN1) {
        // 从CAN FIFO0中获取接收到的消息
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
        // printf("ID:%d len:%d data:", (int)RxHeader.StdId, (int)RxHeader.DLC);
        // for (uint32_t i = 0; i < RxHeader.DLC; i++) {
        //     printf("%x ", RxData[i]);
        // }
        // printf("\r\n");
        
        // 检查是否为预设的控制指令ID
        if (RxHeader.StdId == 0x08) {
            // 解析CAN数据中的速度命令（16位有符号整数）
            s_chassis.velocity_x = RxData[0] | RxData[1] << 8;  // X轴速度
            s_chassis.velocity_z = RxData[2] | RxData[3] << 8;  // Z轴旋转速度
            
            // 将无符号整数转换为有符号整数（处理负数）
            if (s_chassis.velocity_x > 32767) {
                s_chassis.velocity_x = s_chassis.velocity_x - 65536;  // 转换为负数
            }
            if (s_chassis.velocity_z > 32767) {
                s_chassis.velocity_z = s_chassis.velocity_z - 65536;  // 转换为负数
            }
            
            // 执行底盘控制算法
            chassis_control(&s_chassis, can_signal);
            printf("pwm:%ld\r\n", s_chassis.velocity_x);  // 串口输出X轴速度
        }
    }
}
/* USER CODE END 1 */

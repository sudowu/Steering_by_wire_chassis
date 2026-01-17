/*
 * @file    pwm_duty_analyzer.c
 * @brief   PWM占空比分析模块实现
 * @details 包含PWM信号占空比分析相关的全局变量定义
 * @version 1.0.0
 * @author  YourName
 * @date    2024-01-XX
 * @note    实现了对未知频率PWM信号的占空比测量功能
 */

#include "pwm_duty_analyzer.h"
#include <math.h>
#include <stdint.h>
#include <string.h>

/* PWM占空比分析模块使用的全局变量 */
uint8_t adc_buffer1[PWM_ADC_BUFFER_SIZE] = {0};  /* ADC1采样数据缓冲区，用于存储PWM1信号的采样值 */
uint8_t adc_buffer2[PWM_ADC_BUFFER_SIZE] = {0};  /* ADC2采样数据缓冲区，用于存储PWM2信号的采样值 */
uint32_t adc_pwm1_hight_count = 0;               /* ADC采样中PWM1信号高电平期间的计数值 */
uint32_t adc_pwm2_hight_count = 0;               /* ADC采样中PWM2信号高电平期间的计数值 */
uint8_t flag_adc_dma = 0;                        /* ADC DMA转换完成标志，用于通知主循环处理新数据 */



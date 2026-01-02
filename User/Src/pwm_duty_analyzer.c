/**
 * @file    pwm_duty_analyzer.c
 * @brief   PWM占空比分析实现
 * @version 1.0.0
 */

#include "pwm_duty_analyzer.h"
#include <math.h>
#include <stdint.h>
#include <string.h>

uint32_t adc_buffer1[PWM_ADC_BUFFER_SIZE] = {0};
uint32_t adc_buffer2[PWM_ADC_BUFFER_SIZE] = {0};

uint8_t flag_adc_dma = 0;



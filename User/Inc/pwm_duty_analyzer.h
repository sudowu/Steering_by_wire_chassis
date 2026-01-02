/**
 * @file    pwm_duty_analyzer.h
 * @brief   PWM占空比分析模块
 * @details 用于未知频率PWM信号的占空比测量
 * @author  YourName
 * @date    2024-01-XX
 */

#ifndef __PWM_DUTY_ANALYZER_H
#define __PWM_DUTY_ANALYZER_H


#include "stm32f4xx_hal.h"

#define PWM_ADC_BUFFER_SIZE        4096    // ADC缓冲区大小
#define PWM_SAMPLE_RATE_MAX_HZ     2400000 // 最大采样率
#define PWM_MIN_FREQ_DETECT_HZ     10      // 可检测的最小频率

typedef struct {
    uint32_t frequency_hz;          // PWM频率（Hz）
    float duty_cycle_percent;       // 占空比（%）
    uint32_t pulse_width_us;        // 脉宽（微秒）
    uint32_t period_us;             // 周期（微秒）
    uint8_t signal_valid;           // 信号是否有效
    uint16_t min_adc_value;         // 最小ADC值
    uint16_t max_adc_value;         // 最大ADC值
} pwm_analysis_result_t;

typedef enum {
    PWM_ANALYSIS_MODE_STATISTICAL = 0,     // 统计模式（适合低频）
    PWM_ANALYSIS_MODE_EDGE_DETECTION,      // 边沿检测模式（适合高频）
    PWM_ANALYSIS_MODE_ADAPTIVE             // 自适应模式（自动选择）
} pwm_analysis_mode_t;

extern uint32_t adc_buffer1[PWM_ADC_BUFFER_SIZE];
extern uint32_t adc_buffer2[PWM_ADC_BUFFER_SIZE];
extern uint8_t flag_adc_dma;
#endif /* __PWM_DUTY_ANALYZER_H */



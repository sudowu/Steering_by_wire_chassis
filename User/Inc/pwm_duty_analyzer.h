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

#define PWM_ADC_BUFFER_SIZE        1024    /* ADC采样缓冲区大小，用于存储ADC采样数据 */
#define PWM_SAMPLE_RATE_MAX_HZ     2400000 /* 最大采样频率，单位Hz */
#define PWM_MIN_FREQ_DETECT_HZ     10      /* 可检测的最小PWM频率，单位Hz */

/* PWM信号分析结果结构体，包含PWM信号的关键参数 */
typedef struct {
    uint32_t frequency_hz;          /* 测量得到的PWM信号频率，单位Hz */
    float duty_cycle_percent;       /* 计算得出的占空比百分比 */
    uint32_t pulse_width_us;        /* 脉冲宽度，单位微秒 */
    uint32_t period_us;             /* 信号周期，单位微秒 */
    uint8_t signal_valid;           /* 信号有效性标志，1表示有效，0表示无效 */
    uint16_t min_adc_value;         /* 采样期间检测到的最小ADC值 */
    uint16_t max_adc_value;         /* 采样期间检测到的最大ADC值 */
} pwm_analysis_result_t;

/* 外部全局变量声明 */
extern uint32_t adc_pwm1_hight_count;        /* ADC采样中PWM1高电平计数值 */
extern uint32_t adc_pwm2_hight_count;        /* ADC采样中PWM2高电平计数值 */
extern uint8_t adc_buffer1[PWM_ADC_BUFFER_SIZE];  /* ADC1采样数据缓冲区 */
extern uint8_t adc_buffer2[PWM_ADC_BUFFER_SIZE];  /* ADC2采样数据缓冲区 */
extern uint8_t flag_adc_dma;                  /* ADC DMA转换完成标志 */
#endif /* __PWM_DUTY_ANALYZER_H */



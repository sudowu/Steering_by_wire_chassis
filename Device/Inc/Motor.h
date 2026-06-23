//
// Created by yang on 2026/3/8.
//

#ifndef MOTOR_MOTOR_H
#define MOTOR_MOTOR_H
#include "main.h"
#include "PID.h"

// ADC 缓冲区 (DMA 循环模式)
#define ADC_CHANNELS        3
#define ADC_SAMPLE_GROUP    4        //正常采样次数，越大越稳定，但响应越慢
#define ADC_BUFFER_SIZE     (ADC_CHANNELS * ADC_SAMPLE_GROUP) // ADC DMA 缓冲区大小
#define ADC_OFFSET_SAMPLES  16            // 用于电流偏置校准的采样次数

typedef struct {
    uint8_t h_pwm_channel;
    uint8_t l_on_channel;
}motor_pwm_config_t ;


typedef enum
{
    speed = 0, // 速度模式
    position = 1, // 位置模式
    current = 2 // 电流模式
}configMotorMode_t;

typedef enum
{
    MOTOR1 = 0,
    MOTOR2 = 1,
}motorId;

typedef enum
{
    STOP = 0,
    CW = 1, // 顺时针
    CCW = 2 // 逆时针
}direction_t;

typedef struct
{
    // 新增：ADC 电流采样相关
    float adc_offset_u;          // U 相偏置电压 (0A 时的 ADC 值)
    float adc_offset_v;          // V 相偏置电压
    float adc_offset_w;          // W 相偏置电压
    uint8_t offset_calibrated;      // 偏置校准标志：0-未校准，1-已校准

    float adc_current_u;          // U 相实际电流 (减去偏置后)
    float adc_current_v;          // V 相实际电流
    float adc_current_w;          // W 相实际电流
}adc_current_t;

typedef struct
{
    motorId id; // 电机ID
    TIM_HandleTypeDef* htim; // 关联的定时器句柄
    TIM_HandleTypeDef* encoder; // 关联的编码器定时器句柄
    uint8_t run_state; // 运行状态：0-停止，1-运行
    direction_t direction; // 方向：0-停止，1-顺时针，2-逆时针
    direction_t forward_direction; // 前进时的旋转方向（电机1为CCW，电机2为CW）
    direction_t encoder_direction; // 编码器方向：0-正转，1-反转
    uint8_t hall_state; // 霍尔状态，范围0-7
    uint16_t pwm_duty; // PWM占空比，范围0-1000
    uint32_t commutating_counter; // 计数器，用于换相
    double rpm;
    int32_t last_encoder_count; // 上次编码器计数，用于计算delta
    adc_current_t adc_current; // 电流采样数据
    AdvancedPID_TypeDef pid; // PID控制器实例
} Motor_t;

extern Motor_t g_Motor1;
extern Motor_t g_Motor2;
extern uint16_t ADC_Buffer[ADC_SAMPLE_GROUP][ADC_CHANNELS]; // ADC DMA 缓冲区
extern uint8_t ADC_DataReady; // ADC 数据就绪标志

void motor_init();
void hall_start(Motor_t* motor);
void hall_state_read(Motor_t* motor);
void motor_commutating_phase_callback(Motor_t* motor);
void MotorControl(const Motor_t* motor, const motor_pwm_config_t* config);
void motor_rpm_get(Motor_t* motor);
void Motor_OffsetCalibration(Motor_t* motor);
HAL_StatusTypeDef Motor_OffsetCalibrate(Motor_t* motor);
HAL_StatusTypeDef Motor_ReadAdcCurrent(Motor_t* motor);
float Motor_GetTotalCurrent(Motor_t* motor);

/* 电机控制 API */
HAL_StatusTypeDef Motor_SetDirection(Motor_t* motor, direction_t direction);
HAL_StatusTypeDef Motor_SetDutyCycle(Motor_t* motor, uint16_t duty_cycle);
HAL_StatusTypeDef Motor_Start(Motor_t* motor, direction_t direction, uint16_t duty_cycle);
HAL_StatusTypeDef Motor_Stop(Motor_t* motor);
direction_t Motor_GetDirection(Motor_t* motor);
uint16_t Motor_GetDutyCycle(Motor_t* motor);
uint8_t Motor_IsRunning(Motor_t* motor);

/* 电机 PID 转速控制 API */
void Motor_SpeedPID_Init(Motor_t* motor, float Kp, float Ki, float Kd);
HAL_StatusTypeDef Motor_SpeedControl(Motor_t* motor, float target_rpm);
void Motor_SpeedPID_SetParams(Motor_t* motor, float Kp, float Ki, float Kd);
float Motor_SpeedPID_GetError(Motor_t* motor);
void Motor_SpeedPID_Enable(Motor_t* motor, uint8_t enable);
#endif //MOTOR_MOTOR_H


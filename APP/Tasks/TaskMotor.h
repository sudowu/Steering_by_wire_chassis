//
// Created by yang on 2026/3/8.
//

#ifndef MOTOR_TASKMOTOR_H
#define MOTOR_TASKMOTOR_H
#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"

extern TaskHandle_t g_TaskMotor;
extern SemaphoreHandle_t xSemaphoreADCReady;
extern TaskHandle_t g_TaskSpeedControl;  // 速度控制任务句柄

void vTaskRPM_Get(TimerHandle_t xTimer);
void vTaskMotor(void* parameter);
// void motor_config_timer_callback(TimerHandle_t xTimer);
void vTask_Data_Send(void* parameter);
void vTask_ADC_Sample(void* parameter);
void vTask_SpeedControl(void* parameter);  // 速度控制任务
#endif //MOTOR_TASKMOTOR_H
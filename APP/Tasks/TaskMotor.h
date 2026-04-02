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
void vTaskRPM_Read(TimerHandle_t xTimer);
void vTaskMotor(void* parameter);
// void motor_config_timer_callback(TimerHandle_t xTimer);
void vTask_Data_Send(void* parameter);
void vTask_ADC_Sample(void* parameter);
#endif //MOTOR_TASKMOTOR_H
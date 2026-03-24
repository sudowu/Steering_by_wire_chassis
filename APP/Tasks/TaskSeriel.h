//
// Created by yang on 2026/3/18.
//

#ifndef MOTOR_TASKSERIEL_H
#define MOTOR_TASKSERIEL_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

extern QueueHandle_t xQueueSeriel;
extern SemaphoreHandle_t xSemaphoreSeriel;
extern TaskHandle_t g_TaskSeriel;
void vTaskSeriel(void* argument);
#endif //MOTOR_TASKSERIEL_H
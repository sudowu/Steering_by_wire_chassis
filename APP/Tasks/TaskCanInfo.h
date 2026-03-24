//
// Created by yang on 2026/3/7.
//

#ifndef MOTOR_TASKCANINFO_H
#define MOTOR_TASKCANINFO_H
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "queue.h"
#include "semphr.h"

extern TaskHandle_t g_TaskCanInfo;
extern QueueHandle_t canTxQueue; // CAN发送消息队列
extern SemaphoreHandle_t canTxCompleteSemaphore;
typedef struct
{
    uint32_t StdId; // 标准ID
    uint8_t Len; // 数据长度（0-8）
    uint8_t Data[8]; // 数据内容
} CAN_Message_t;


void vTaskCanInfo(void* param);
HAL_StatusTypeDef CAN_Send_HAL(CAN_Message_t* message);


#endif //MOTOR_TASKCANINFO_H

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
extern QueueHandle_t canTxQueue;
extern SemaphoreHandle_t canTxCompleteSemaphore;

typedef struct
{
    uint32_t StdId;
    uint8_t Len;
    uint8_t Data[8];
} CAN_Message_t;

typedef struct {
    CAN_Message_t msg;
    uint8_t used;
} CanTxBuf_t;

void vTaskCanInfo(void* param);
HAL_StatusTypeDef CAN_Send_HAL(CAN_Message_t* message);
CanTxBuf_t* CAN_GetFreeTxBuf(void);

// 便捷宏：发送CAN消息
#define CAN_SEND(id, len, ...) \
    do { \
        extern QueueHandle_t canTxQueue; \
        CanTxBuf_t* _buf = CAN_GetFreeTxBuf(); \
        if (_buf) { \
            _buf->msg.StdId = id; \
            _buf->msg.Len = len; \
            uint8_t _data[] = { __VA_ARGS__ }; \
            memcpy(_buf->msg.Data, _data, len); \
            xQueueSend(canTxQueue, &_buf, 0); \
        } \
    } while(0)

#endif //MOTOR_TASKCANINFO_H

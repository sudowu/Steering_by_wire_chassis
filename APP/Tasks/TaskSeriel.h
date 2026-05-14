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

// 静态缓冲区API
typedef struct {
    uint8_t data[128];
    uint16_t len;
    uint8_t used;
} SerialTxBuf_t;

SerialTxBuf_t* Serial_GetFreeTxBuf(void);

// 便捷宏：格式化发送
#define SERIAL_SEND(fmt, ...) \
    do { \
        extern QueueHandle_t xQueueSeriel; \
        SerialTxBuf_t* _buf = Serial_GetFreeTxBuf(); \
        if (_buf) { \
            _buf->len = snprintf((char*)_buf->data, sizeof(_buf->data), fmt, ##__VA_ARGS__); \
            xQueueSend(xQueueSeriel, &_buf, 0); \
        } \
    } while(0)

#endif //MOTOR_TASKSERIEL_H
//
// Created by yang on 2026/3/18.
//

#include "TaskSeriel.h"

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "main.h"
#include "Motor.h"
#include "usart.h"

// 静态缓冲区配置
#define SERIAL_TX_BUF_SIZE  128
#define SERIAL_TX_BUF_COUNT 4

// SerialTxBuf_t 定义在 TaskSeriel.h 中

static SerialTxBuf_t tx_buffers[SERIAL_TX_BUF_COUNT];
static StaticQueue_t tx_queue_static;
static uint8_t tx_queue_storage[SERIAL_TX_BUF_COUNT * sizeof(SerialTxBuf_t*)];

QueueHandle_t xQueueSeriel;
SemaphoreHandle_t xSemaphoreSeriel;
TaskHandle_t g_TaskSeriel;

void vTaskSeriel(void* argument)
{
    xSemaphoreSeriel = xSemaphoreCreateBinary();
    vQueueAddToRegistry(xSemaphoreSeriel, "Serial Semaphore");
    
    // 使用静态队列，减少动态内存
    xQueueSeriel = xQueueCreateStatic(
        SERIAL_TX_BUF_COUNT,
        sizeof(SerialTxBuf_t*),
        tx_queue_storage,
        &tx_queue_static
    );
    vQueueAddToRegistry(xQueueSeriel, "Serial Queue");
    
    if (xQueueSeriel == NULL)
    {
        while (1);
    }
    
    while (1)
    {
        SerialTxBuf_t* tx_buf;
        if (xQueueReceive(xQueueSeriel, &tx_buf, portMAX_DELAY) == pdPASS)
        {
            if (tx_buf != NULL && tx_buf->used)
            {
                // 发送数据
                if (HAL_UART_Transmit_DMA(&huart1, tx_buf->data, tx_buf->len) == HAL_OK)
                {
                    // 等待发送完成
                    if (xSemaphoreTake(xSemaphoreSeriel, pdMS_TO_TICKS(100)) == pdTRUE)
                    {
                        // 发送完成
                    }
                }
                // 标记缓冲区为空闲
                tx_buf->used = 0;
            }
        }
    }
}

// 获取空闲发送缓冲区
SerialTxBuf_t* Serial_GetFreeTxBuf(void)
{
    for (int i = 0; i < SERIAL_TX_BUF_COUNT; i++)
    {
        if (!tx_buffers[i].used)
        {
            tx_buffers[i].used = 1;
            return &tx_buffers[i];
        }
    }
    return NULL;
}

//
// Created by yang on 2026/3/7.
//

#include "TaskCanInfo.h"

#include <string.h>

#include "FreeRTOS.h"
#include "main.h"
#include "task.h"

#include "can.h"
#include "queue.h"
#include "semphr.h"

// 静态缓冲区配置
#define CAN_TX_BUF_COUNT  8

// CanTxBuf_t 定义在 TaskCanInfo.h 中

static CanTxBuf_t tx_buffers[CAN_TX_BUF_COUNT];
static StaticQueue_t tx_queue_static;
static uint8_t tx_queue_storage[CAN_TX_BUF_COUNT * sizeof(CanTxBuf_t*)];

TaskHandle_t g_TaskCanInfo;
SemaphoreHandle_t canTxCompleteSemaphore;
QueueHandle_t canTxQueue;

CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];
uint32_t TxMailbox;

// 获取空闲CAN发送缓冲区
CanTxBuf_t* CAN_GetFreeTxBuf(void)
{
    for (int i = 0; i < CAN_TX_BUF_COUNT; i++)
    {
        if (!tx_buffers[i].used)
        {
            tx_buffers[i].used = 1;
            return &tx_buffers[i];
        }
    }
    return NULL;
}

void vTaskCanInfo(void* paramter)
{
    canTxCompleteSemaphore = xSemaphoreCreateBinary();
    vQueueAddToRegistry(canTxCompleteSemaphore, "CAN Tx Complete Semaphore");
    
    // 使用静态队列
    canTxQueue = xQueueCreateStatic(
        CAN_TX_BUF_COUNT,
        sizeof(CanTxBuf_t*),
        tx_queue_storage,
        &tx_queue_static
    );
    vQueueAddToRegistry(canTxQueue, "CAN Tx Queue");
    
    HAL_CAN_Start(&hcan1);

    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
    {
        Error_Handler();
    }
    
    while (1)
    {
        CanTxBuf_t* tx_buf;
        if (xQueueReceive(canTxQueue, &tx_buf, portMAX_DELAY) == pdPASS)
        {
            if (tx_buf != NULL && tx_buf->used)
            {
                if (CAN_Send_HAL(&tx_buf->msg) == HAL_OK)
                {
                    if (xSemaphoreTake(canTxCompleteSemaphore, pdMS_TO_TICKS(100)) != pdTRUE)
                    {
                        // 超时处理
                    }
                }
                // 释放缓冲区
                tx_buf->used = 0;
            }
        }
    }
}

HAL_StatusTypeDef CAN_Send_HAL(CAN_Message_t* message)
{
    static CAN_TxHeaderTypeDef TxHeader;
    TxHeader.StdId = message->StdId;
    TxHeader.ExtId = 0;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = message->Len;
    TxHeader.TransmitGlobalTime = DISABLE;

    return HAL_CAN_AddTxMessage(&hcan1, &TxHeader, message->Data, &TxMailbox);
}

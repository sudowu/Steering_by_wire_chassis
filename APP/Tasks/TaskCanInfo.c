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

TaskHandle_t g_TaskCanInfo;
SemaphoreHandle_t canTxCompleteSemaphore;
QueueHandle_t canTxQueue; // CAN接收消息队列

CAN_RxHeaderTypeDef RxHeader; // 接收报文头
uint8_t RxData[8]; // 接收数据缓冲区
uint32_t TxMailbox; // 发送邮箱号

void vTaskCanInfo(void* paramter)
{
    canTxCompleteSemaphore = xSemaphoreCreateBinary(); // 创建二值信号量，用于CAN发送完成通知
    vQueueAddToRegistry(canTxCompleteSemaphore, "CAN Tx Complete Semaphore");
    HAL_CAN_Start(&hcan1); // 启动CAN模块，准备发送和接收数据

    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) // 激活CAN发送完成中断通知
    {
        Error_Handler();
    }
    while (1)
    {
        CAN_Message_t* message;
        xQueueReceive(canTxQueue, &message, portMAX_DELAY); // 从CAN发送消息队列中接收数据，等待直到有数据可用
        if (CAN_Send_HAL(message) == HAL_OK) // 发送ID为0xFF的CAN报文，包含8字节数据
        {
            xSemaphoreTake(canTxCompleteSemaphore, portMAX_DELAY);
        }
        vPortFree(message); // 释放动态分配的消息内存
    }
}

HAL_StatusTypeDef CAN_Send_HAL(CAN_Message_t* message)
{
    static CAN_TxHeaderTypeDef TxHeader; // 发送报文头
    // static uint8_t TxData[8]; // 发送数据缓冲区
    // 1. 配置发送报文头
    TxHeader.StdId = message->StdId; // 标准ID
    TxHeader.ExtId = 0; // 扩展ID（标准帧设为0）
    TxHeader.IDE = CAN_ID_STD; // 标准帧
    TxHeader.RTR = CAN_RTR_DATA; // 数据帧
    TxHeader.DLC = message->Len; // 数据长度（0-8）
    TxHeader.TransmitGlobalTime = DISABLE; // 不启用全局时间

    // 2. 复制数据
    // memset(TxData, 0, sizeof(TxData)); // 清空发送数据缓冲区
    // memcpy(TxData, message->Data, message->Len); // 将消息数据复制到发送缓冲区

    // 3. 发送报文
    return HAL_CAN_AddTxMessage(&hcan1, &TxHeader, message->Data, &TxMailbox);
}

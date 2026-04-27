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
QueueHandle_t canTxQueue; // CAN发送消息队列
QueueHandle_t canRxQueue; // CAN接收消息队列

CAN_RxHeaderTypeDef RxHeader; // 接收报文头
uint8_t RxData[8]; // 接收数据缓冲区
uint32_t TxMailbox; // 发送邮箱号

/* 底盘命令全局变量（默认停车、禁用）*/
ChassisCommand_t g_chassis_cmd = {0.0f, 0.0f, 0, 0};

/**
 * @brief 解析并处理接收到的 CAN 消息（在任务上下文中调用）
 */
void CAN_ProcessRxMessage(const CAN_Message_t* msg)
{
    switch (msg->StdId)
    {
        case CAN_ID_CHASSIS_CMD:
        {
            /* 解析线速度 (int16, mm/s) */
            int16_t linear_raw = (int16_t)(msg->Data[0] | ((uint16_t)msg->Data[1] << 8));
            /* 解析角速度 (int16, mrad/s) */
            int16_t angular_raw = (int16_t)(msg->Data[2] | ((uint16_t)msg->Data[3] << 8));

            g_chassis_cmd.linear_velocity  = (float)linear_raw / 1000.0f;   // mm/s → m/s
            g_chassis_cmd.angular_velocity = (float)angular_raw / 1000.0f;  // mrad/s → rad/s
            g_chassis_cmd.enable    = (msg->Data[4] & CHASSIS_FLAG_ENABLE) ? 1 : 0;
            g_chassis_cmd.timestamp = HAL_GetTick();
            break;
        }

        default:
            break;
    }
}

void vTaskCanInfo(void* paramter)
{
    canTxCompleteSemaphore = xSemaphoreCreateBinary(); // 创建二值信号量，用于CAN发送完成通知
    vQueueAddToRegistry(canTxCompleteSemaphore, "CAN Tx Complete Semaphore");

    /* 配置 CAN 滤波器并启动 */
    CAN_Filter_Config();
    HAL_CAN_Start(&hcan1);

    /* 激活 TX 和 RX 中断通知 */
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }

    while (1)
    {
        CAN_Message_t* message;

        /* 等待 TX 队列消息（带 10ms 超时，同时可处理 RX）*/
        if (xQueueReceive(canTxQueue, &message, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            if (CAN_Send_HAL(message) == HAL_OK)
            {
                xSemaphoreTake(canTxCompleteSemaphore, portMAX_DELAY);
            }
            vPortFree(message); // 释放动态分配的消息内存
        }

        /* 非阻塞检查 RX 队列 */
        CAN_Message_t* rxMsg;
        while (xQueueReceive(canRxQueue, &rxMsg, 0) == pdTRUE)
        {
            CAN_ProcessRxMessage(rxMsg);
            vPortFree(rxMsg);
        }
    }
}

HAL_StatusTypeDef CAN_Send_HAL(CAN_Message_t* message)
{
    static CAN_TxHeaderTypeDef TxHeader; // 发送报文头

    TxHeader.StdId = message->StdId;
    TxHeader.ExtId = 0;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = message->Len;
    TxHeader.TransmitGlobalTime = DISABLE;

    return HAL_CAN_AddTxMessage(&hcan1, &TxHeader, message->Data, &TxMailbox);
}

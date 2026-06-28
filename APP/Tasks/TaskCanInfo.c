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

#include "Chassis.h"
#include "Motor.h"

TaskHandle_t g_TaskCanInfo;
SemaphoreHandle_t canTxCompleteSemaphore;
QueueHandle_t canTxQueue; // CAN发送消息队列
QueueHandle_t canRxQueue; // CAN接收消息队列

CAN_RxHeaderTypeDef RxHeader; // 接收报文头
uint8_t RxData[8]; // 接收数据缓冲区
uint32_t TxMailbox; // 发送邮箱号

uint8_t g_can_bus_off_flag = 0;  // CAN Bus-Off 标志，由错误回调置位

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

            g_chassis.cmd_linear_vel  = (float)linear_raw / 1000.0f;   // mm/s → m/s
            g_chassis.cmd_angular_vel = (float)angular_raw / 1000.0f;  // mrad/s → rad/s
            g_chassis.cmd_enable    = (msg->Data[4] & CHASSIS_FLAG_ENABLE) ? 1 : 0;
            g_chassis.cmd_timestamp = HAL_GetTick();
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
                if (xSemaphoreTake(canTxCompleteSemaphore, pdMS_TO_TICKS(50)) != pdTRUE)
                {
                    /* 超时：总线异常（如 bus-off），中止发送并消耗 abort 产生的信号量 */
                    HAL_CAN_AbortTxRequest(&hcan1, TxMailbox);
                    xSemaphoreTake(canTxCompleteSemaphore, pdMS_TO_TICKS(10));
                }
            }
            vPortFree(message);
        }

        /* 非阻塞检查 RX 队列 */
        CAN_Message_t rxMsg;
        while (xQueueReceive(canRxQueue, &rxMsg, 0) == pdTRUE)
        {
            CAN_ProcessRxMessage(&rxMsg);
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

/**
 * @brief 发送底盘综合状态帧 (CAN ID 0x101, ~100Hz)
 *
 * 合并原 0x101 状态帧与 0x102 转速帧：上报 RPM、运行状态、使能回显、心跳。
 */
void CAN_SendChassisStatus(Chassis_t* c)
{
    CAN_Message_t* tx = pvPortMalloc(sizeof(CAN_Message_t));
    if (tx == NULL)
    {
        return;
    }

    static uint8_t heartbeat = 0;

    int16_t rpm_left  = (int16_t)(c->motor_left->rpm);
    int16_t rpm_right = (int16_t)(c->motor_right->rpm);

    tx->StdId = CAN_ID_CHASSIS_STATUS;
    tx->Len   = 8;
    tx->Data[0] = (uint8_t)(rpm_left & 0xFF);
    tx->Data[1] = (uint8_t)((rpm_left >> 8) & 0xFF);
    tx->Data[2] = (uint8_t)(rpm_right & 0xFF);
    tx->Data[3] = (uint8_t)((rpm_right >> 8) & 0xFF);
    tx->Data[4] = 0;
    tx->Data[5] = 0;

    tx->Data[6] = 0;
    if (Motor_IsRunning(c->motor_left))  tx->Data[6] |= CHASSIS_STATUS_LEFT_RUN;
    if (Motor_IsRunning(c->motor_right)) tx->Data[6] |= CHASSIS_STATUS_RIGHT_RUN;
    if (c->cmd_enable)                   tx->Data[6] |= CHASSIS_STATUS_ENABLED;
    if (Chassis_IsCommandValid(c))       tx->Data[6] |= CHASSIS_STATUS_CMD_VALID;

    tx->Data[7] = heartbeat++;

    if (xQueueSendToBack(canTxQueue, &tx, 0) != pdTRUE)
    {
        vPortFree(tx);
    }
}

/**
 * @brief 发送底盘诊断帧 (CAN ID 0x103, ~10Hz)
 *
 * 上报故障码、PWM 占空比、PID 误差。
 */
void CAN_SendChassisDiag(Chassis_t* c)
{
    CAN_Message_t* tx = pvPortMalloc(sizeof(CAN_Message_t));
    if (tx == NULL)
    {
        return;
    }

    uint8_t fault = 0;
    if (Motor_GetTotalCurrent(c->motor_left)  > MOTOR_OVERCURRENT_THRESHOLD_A) fault |= CHASSIS_FAULT_LEFT_OC;
    if (Motor_GetTotalCurrent(c->motor_right) > MOTOR_OVERCURRENT_THRESHOLD_A) fault |= CHASSIS_FAULT_RIGHT_OC;
    if (g_can_bus_off_flag)                                                      fault |= CHASSIS_FAULT_CAN_BUSOFF;
    if (!Chassis_IsCommandValid(c))                                              fault |= CHASSIS_FAULT_CMD_TIMEOUT;

    int16_t err_left  = (int16_t)(Motor_SpeedPID_GetError(c->motor_left));
    int16_t err_right = (int16_t)(Motor_SpeedPID_GetError(c->motor_right));

    tx->StdId = CAN_ID_CHASSIS_DIAG;
    tx->Len   = 8;
    tx->Data[0] = fault;
    tx->Data[1] = (uint8_t)(Motor_GetDutyCycle(c->motor_left)  * 250 / 1000);
    tx->Data[2] = (uint8_t)(Motor_GetDutyCycle(c->motor_right) * 250 / 1000);
    tx->Data[3] = (uint8_t)(err_left & 0xFF);
    tx->Data[4] = (uint8_t)((err_left >> 8) & 0xFF);
    tx->Data[5] = (uint8_t)(err_right & 0xFF);
    tx->Data[6] = (uint8_t)((err_right >> 8) & 0xFF);
    tx->Data[7] = 0;

    if (xQueueSendToBack(canTxQueue, &tx, 0) != pdTRUE)
    {
        vPortFree(tx);
    }
}

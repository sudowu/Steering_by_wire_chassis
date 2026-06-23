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
extern QueueHandle_t canRxQueue; // CAN接收消息队列
extern SemaphoreHandle_t canTxCompleteSemaphore;

typedef struct
{
    uint32_t StdId; // 标准ID
    uint8_t Len; // 数据长度（0-8）
    uint8_t Data[8]; // 数据内容
} CAN_Message_t;

/* ==================== CAN 应用层协议 ==================== */

/* 底盘速度指令 ID */
#define CAN_ID_CHASSIS_CMD         0x100

/* 底盘状态上报 ID */
#define CAN_ID_CHASSIS_STATUS      0x101

/* 底盘速度指令格式 (ID 0x100, 8 bytes)
 *  Byte[0-1]:  线速度 (int16, 单位: mm/s, scale 1)
 *  Byte[2-3]:  角速度 (int16, 单位: mrad/s, scale 1)
 *  Byte[4]:    控制标志
 *    Bit 0:    使能 (1=电机通电, 0=断电)
 *    Bit 1-7:  保留
 *  Byte[5-7]:  保留
 */

/* 底盘状态上报格式 (ID 0x101, 8 bytes)
 *  Byte[0-1]:  实际线速度 (int16, 单位: mm/s)
 *  Byte[2-3]:  实际角速度 (int16, 单位: mrad/s)
 *  Byte[4]:    状态标志
 *    Bit 0:    左电机运行状态
 *    Bit 1:    右电机运行状态
 *    Bit 2:    左电机故障
 *    Bit 3:    右电机故障
 *  Byte[5]:    左电机电流 (uint8, 0.1A/LSB)
 *  Byte[6]:    右电机电流 (uint8, 0.1A/LSB)
 *  Byte[7]:    保留
 */

/* 底盘控制命令标志位 */
#define CHASSIS_FLAG_ENABLE        0x01

#include "Chassis.h"

/* ==================== 函数声明 ==================== */

void vTaskCanInfo(void* param);
HAL_StatusTypeDef CAN_Send_HAL(CAN_Message_t* message);
void CAN_ProcessRxMessage(const CAN_Message_t* msg);
void CAN_SendChassisStatus(Chassis_t* c);

#endif //MOTOR_TASKCANINFO_H

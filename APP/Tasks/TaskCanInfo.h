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

/* ---- 调试帧（直接底盘控制，不参与模式逻辑）---- */

/* 底盘速度指令 ID */
#define CAN_ID_CHASSIS_CMD         0x100

/* 底盘状态上报 ID（合并原 0x101 状态帧与 0x102 转速帧）*/
#define CAN_ID_CHASSIS_STATUS      0x101

/* 底盘诊断信息 ID */
#define CAN_ID_CHASSIS_DIAG        0x103

/* ---- SbW 上层自动驾驶指令帧（占位符，协议待定）---- */

/* 自动转向 */
#define CAN_ID_SBW_AUTO_STEERING   0x200

/* 自动驱动 */
#define CAN_ID_SBW_AUTO_DRIVE      0x201

/* 自动档位 */
#define CAN_ID_SBW_AUTO_GEAR       0x202

/* 自动制动 */
#define CAN_ID_SBW_AUTO_BRAKE      0x203

/* ---- SbW 物理输入帧（占位符，协议待定）---- */

/* 物理档位选择器 */
#define CAN_ID_SBW_MANUAL_GEAR     0x210

/* 物理方向盘（转角 + 扭矩）*/
#define CAN_ID_SBW_MANUAL_STEERING 0x211

/* 物理油门踏板 */
#define CAN_ID_SBW_MANUAL_ACCEL    0x212

/* 物理制动踏板 */
#define CAN_ID_SBW_MANUAL_BRAKE    0x213

/* 底盘速度指令格式 (ID 0x100, 8 bytes)
 *  Byte[0-1]:  线速度 (int16, 单位: mm/s, scale 1)
 *  Byte[2-3]:  角速度 (int16, 单位: mrad/s, scale 1)
 *  Byte[4]:    控制标志
 *    Bit 0:    使能 (1=电机通电, 0=断电)
 *    Bit 1-7:  保留
 *  Byte[5-7]:  保留
 */

/* 底盘综合状态上报格式 (ID 0x101, 8 bytes, ~100Hz)
 *  Byte[0-1]:  左电机转速 (int16, 单位: RPM)
 *  Byte[2-3]:  右电机转速 (int16, 单位: RPM)
 *  Byte[4-5]:  保留
 *  Byte[6]:    状态标志
 *    Bit 0:    左电机运行
 *    Bit 1:    右电机运行
 *    Bit 2:    系统使能回显
 *    Bit 3:    命令有效 (1=未超时)
 *    Bit 4-7:  保留
 *  Byte[7]:    心跳计数 (uint8, 每帧+1 溢出回零)
 */

/* 底盘诊断信息格式 (ID 0x103, 8 bytes, ~10Hz)
 *  Byte[0]:    故障码
 *    Bit 0:    左电机过流
 *    Bit 1:    右电机过流
 *    Bit 2:    CAN Bus-Off 发生过
 *    Bit 3:    命令超时
 *    Bit 4-7:  保留
 *  Byte[1]:    左电机 PWM 占空比 (uint8, 0.4%/LSB, 0-250)
 *  Byte[2]:    右电机 PWM 占空比 (uint8, 0.4%/LSB, 0-250)
 *  Byte[3-4]:  左电机 PID 误差 (int16, 单位: RPM)
 *  Byte[5-6]:  右电机 PID 误差 (int16, 单位: RPM)
 *  Byte[7]:    保留
 */

/* 底盘控制命令标志位 */
#define CHASSIS_FLAG_ENABLE        0x01

/* 状态标志位 (0x101 Byte[6]) */
#define CHASSIS_STATUS_LEFT_RUN    0x01
#define CHASSIS_STATUS_RIGHT_RUN   0x02
#define CHASSIS_STATUS_ENABLED     0x04
#define CHASSIS_STATUS_CMD_VALID   0x08

/* 故障码位 (0x103 Byte[0]) */
#define CHASSIS_FAULT_LEFT_OC      0x01
#define CHASSIS_FAULT_RIGHT_OC     0x02
#define CHASSIS_FAULT_CAN_BUSOFF   0x04
#define CHASSIS_FAULT_CMD_TIMEOUT  0x08

/* 过流保护阈值 (A) */
#define MOTOR_OVERCURRENT_THRESHOLD_A  10.0f

#include "Chassis.h"
#include "SbwTypes.h"

/* SbW 双实例引用（定义在 TaskSbwControl.c）*/
extern Chassis_Function g_chassis_auto;
extern Chassis_Function g_chassis_manual;

/* ==================== 函数声明 ==================== */

void vTaskCanInfo(void* param);
HAL_StatusTypeDef CAN_Send_HAL(CAN_Message_t* message);
void CAN_ProcessRxMessage(const CAN_Message_t* msg);
void CAN_SendChassisStatus(Chassis_t* c);
void CAN_SendChassisDiag(Chassis_t* c);

/* CAN Bus-Off 标志，由 HAL CAN 错误回调置位 */
extern uint8_t g_can_bus_off_flag;

#endif //MOTOR_TASKCANINFO_H

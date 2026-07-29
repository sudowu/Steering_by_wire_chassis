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

/* ============================================================
 * CAN 应用层协议 — 详细格式参见 doc/CAN_Protocol.md
 *
 * ID 分配：
 *   0x100-0x103  底盘调试帧       0x200-0x203  自动驾驶指令帧
 *   0x110-0x115  状态反馈帧       0x210-0x213  物理驾驶输入帧
 *   0x220        SbW 使能控制
 * ============================================================ */

/* ---- 1. 底盘调试帧 (0x100-0x103) — 电机直控，不参与 SbW 模式 ---- */

#define CAN_ID_CHASSIS_CMD         0x100   // 速度指令 (RX): int16 v[mm/s] + int16 ω[mrad/s] + enable flag
#define CAN_ID_CHASSIS_STATUS      0x101   // 综合状态上报 (TX, ~100Hz): 左右 RPM + 状态位 + 心跳
#define CAN_ID_CHASSIS_DIAG        0x103   // 底盘诊断 (TX, ~10Hz): 故障码 + PWM + PID误差

#define CHASSIS_FLAG_ENABLE        0x01    // 0x100 Byte[4] Bit0: 电机使能

#define CHASSIS_STATUS_LEFT_RUN    0x01    // 0x101 Byte[6] Bit0
#define CHASSIS_STATUS_RIGHT_RUN   0x02    // 0x101 Byte[6] Bit1
#define CHASSIS_STATUS_ENABLED     0x04    // 0x101 Byte[6] Bit2
#define CHASSIS_STATUS_CMD_VALID   0x08    // 0x101 Byte[6] Bit3

#define CHASSIS_FAULT_LEFT_OC      0x01    // 0x103 Byte[0] Bit0: 左电机过流 (>10A)
#define CHASSIS_FAULT_RIGHT_OC     0x02    // 0x103 Byte[0] Bit1
#define CHASSIS_FAULT_CAN_BUSOFF   0x04    // 0x103 Byte[0] Bit2
#define CHASSIS_FAULT_CMD_TIMEOUT  0x08    // 0x103 Byte[0] Bit3

/* ---- 2. SbW 全线控使能帧 (0x220) ---- */

#define CAN_ID_SBW_CONTROL          0x220   // 全线控使能 (RX): Byte[0]=0 关闭 / ≠0 开启, 双实例同步

/* ---- 3. 自动驾驶指令帧 (0x200-0x203, RX) → g_chassis_auto ---- */

#define CAN_ID_SBW_AUTO_STEERING   0x200   // 转向: Enable + Angle[0.5°/LSB] + Speed[1°/s]
#define CAN_ID_SBW_AUTO_DRIVE      0x201   // 驱动: Enable + Pedal[1%] + Accel[0.1m/s²] + Torque[1Nm]
#define CAN_ID_SBW_AUTO_GEAR       0x202   // 档位+驻车: Gear/Parking Enable + Target Gear + Parking Request
#define CAN_ID_SBW_AUTO_BRAKE      0x203   // 制动: Enable + Decel[0.1m/s²] + Brake Light

/* ---- 4. 物理驾驶输入帧 (0x210-0x213, RX) → g_chassis_manual, 同时用于接管检测 ---- */

#define CAN_ID_SBW_MANUAL_GEAR     0x210   // 档位: Position (0=P/R/N/D), Config_Enable 强制置1
#define CAN_ID_SBW_MANUAL_STEERING 0x211   // 方向盘: Angle[0.5°/LSB] + Torque[0.1Nm]
#define CAN_ID_SBW_MANUAL_ACCEL    0x212   // 油门: Pedal[1%], Config_Enable 强制置1
#define CAN_ID_SBW_MANUAL_BRAKE    0x213   // 制动: Pedal[0.1%], Config_Enable 强制置1

/* ---- 5. SbW 状态反馈帧 (0x110-0x115, TX) — 分频: 50/50/20/20/10/5 Hz ---- */

#define CAN_ID_SBW_VEHICLE_STATE   0x110   // 车辆状态 (50Hz): mode + flags + v/ω + fault_summary + heartbeat
#define CAN_ID_SBW_STEERING_FB     0x111   // 转向反馈 (50Hz): angle + torque + speed + mode + fault
#define CAN_ID_SBW_DRIVE_FB        0x112   // 驱动反馈 (20Hz): speed + pedal + accel + mode + takeover + fault
#define CAN_ID_SBW_BRAKING_FB      0x113   // 制动反馈 (20Hz): pedal + decel + mode + takeover + brake_light + fault
#define CAN_ID_SBW_GEAR_PARKING_FB 0x114   // 档位+驻车 (10Hz): gear_position + EPB status + faults
#define CAN_ID_SBW_MC_DETAIL       0x115   // MC调试 (5Hz): cmd_v/ω + mc_flags + layer

/* 0x110 Byte[1] system_flags */
#define SBW_FLAG_EMERGENCY_STOP   0x01
#define SBW_FLAG_CMD_TIMEOUT      0x02
#define SBW_FLAG_SBW_ACTIVE       0x04
#define SBW_FLAG_CHASSIS_ENABLED  0x08

/* 0x110 Byte[6] fault_summary */
#define SBW_FAULT_STEERING        0x01
#define SBW_FAULT_DRIVE           0x02
#define SBW_FAULT_BRAKING         0x04
#define SBW_FAULT_GEAR            0x08
#define SBW_FAULT_PARKING         0x10

/* 0x111 Byte[6] steering_flags */
#define SBW_STEER_FLAG_ENABLE     0x04
#define SBW_STEER_MODE_MASK       0x03
#define SBW_STEER_MODE_AUTO       0x00
#define SBW_STEER_MODE_MANUAL     0x01
#define SBW_STEER_MODE_INTERRUPT  0x02

/* 0x111 Byte[7] steering_fault */
#define SBW_STEER_FAULT_NONE      0
#define SBW_STEER_FAULT_SENSOR    1
#define SBW_STEER_FAULT_MOTOR     2
#define SBW_STEER_FAULT_TIMEOUT   3

/* 0x112 Byte[5] drive_flags */
#define SBW_DRIVE_FLAG_ENABLE     0x04
#define SBW_DRIVE_FLAG_TAKEOVER   0x08
#define SBW_DRIVE_MODE_MASK       0x03
#define SBW_DRIVE_MODE_AUTO       0x00
#define SBW_DRIVE_MODE_MANUAL     0x01
#define SBW_DRIVE_MODE_INTERRUPT  0x02

/* 0x112 Byte[6] drive_fault */
#define SBW_DRIVE_FAULT_NONE      0
#define SBW_DRIVE_FAULT_MOTOR     1
#define SBW_DRIVE_FAULT_OVERCUR   2
#define SBW_DRIVE_FAULT_TIMEOUT   3

/* 0x113 Byte[3] brake_flags */
#define SBW_BRAKE_FLAG_ENABLE     0x04
#define SBW_BRAKE_FLAG_TAKEOVER   0x08
#define SBW_BRAKE_FLAG_LIGHT      0x10
#define SBW_BRAKE_MODE_MASK       0x03
#define SBW_BRAKE_MODE_AUTO       0x00
#define SBW_BRAKE_MODE_MANUAL     0x01
#define SBW_BRAKE_MODE_INTERRUPT  0x02

/* 0x113 Byte[4] brake_fault */
#define SBW_BRAKE_FAULT_NONE      0
#define SBW_BRAKE_FAULT_SYSTEM    1
#define SBW_BRAKE_FAULT_LIGHT     2
#define SBW_BRAKE_FAULT_TIMEOUT   3

/* 0x114 Byte[2] epb_status */
#define SBW_EPB_CLAMPED            0x01
#define SBW_EPB_PARKING_STATUS     0x02

/* 0x114 Byte[1] gear_fault */
#define SBW_GEAR_FAULT_NONE        0
#define SBW_GEAR_FAULT_SENSOR      1
#define SBW_GEAR_FAULT_ACTUATOR    2
#define SBW_GEAR_FAULT_TIMEOUT     3
#define SBW_GEAR_FAULT_INTERLOCK   4

/* 0x114 Byte[3] parking_fault */
#define SBW_PARKING_FAULT_NONE     0
#define SBW_PARKING_FAULT_LEFT     1
#define SBW_PARKING_FAULT_RIGHT    2
#define SBW_PARKING_FAULT_TIMEOUT  3

/* 0x115 Byte[4] mc_flags */
#define SBW_MC_FLAG_BRAKING        0x01
#define SBW_MC_FLAG_EMERGENCY      0x02
#define SBW_MC_FLAG_SAFETY_GATE    0x04

/* 过流保护阈值 (A) */
#define MOTOR_OVERCURRENT_THRESHOLD_A  10.0f

#include "Chassis.h"
#include "SbwTypes.h"
#include "MotionControl.h"

/* SbW 双实例引用（定义在 TaskSbwControl.c）*/
extern Chassis_Function g_chassis_auto;
extern Chassis_Function g_chassis_manual;

/* ==================== 函数声明 ==================== */

void vTaskCanInfo(void* param);
HAL_StatusTypeDef CAN_Send_HAL(CAN_Message_t* message);
void CAN_ProcessRxMessage(const CAN_Message_t* msg);
void CAN_SendChassisStatus(Chassis_t* c);
void CAN_SendChassisDiag(Chassis_t* c);

/* ---- SbW 状态反馈发送函数 ---- */

void CAN_SendSbwVehicleState(const Chassis_t* c, driving_mode mode,
                             uint8_t system_flags, uint8_t fault_summary);
void CAN_SendSbwSteeringFB(const Steering_Function* sf);
void CAN_SendSbwDriveFB(const Drive_Function* df);
void CAN_SendSbwBrakingFB(const Braking_Function* bf);
void CAN_SendSbwGearParkingFB(const Gear_Function* gf,
                              const Parking_Function* pf);
void CAN_SendSbwMCDetail(const MotionControl_State_t* mc,
                         const Chassis_t* c);

/* CAN Bus-Off 标志，由 HAL CAN 错误回调置位 */
extern uint8_t g_can_bus_off_flag;

#endif //MOTOR_TASKCANINFO_H

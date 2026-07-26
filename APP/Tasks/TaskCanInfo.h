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

/* ---- SbW 状态反馈帧（底盘 → 上位机）---- */

/* 车辆运动状态 + 系统模式 + 故障汇总 (50Hz) */
#define CAN_ID_SBW_VEHICLE_STATE   0x110

/* 转向反馈: 角度 + 扭矩 + 转速 (50Hz) */
#define CAN_ID_SBW_STEERING_FB     0x111

/* 驱动反馈: 车速 + 油门 + 加速度 (20Hz) */
#define CAN_ID_SBW_DRIVE_FB        0x112

/* 制动反馈: 踏板位置 + 减速度 + 制动灯 (20Hz) */
#define CAN_ID_SBW_BRAKING_FB      0x113

/* 档位 & 驻车反馈 (10Hz) */
#define CAN_ID_SBW_GEAR_PARKING_FB 0x114

/* 运动控制调试帧 (5Hz) */
#define CAN_ID_SBW_MC_DETAIL       0x115

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

/* ==================== SbW 状态反馈帧格式定义 ==================== */

/*
 * 0x110 SbwVehicleState (50Hz, 8 bytes) — 车辆运动状态 + 系统模式 + 故障汇总
 *
 *  Byte[0]:    driving_mode (uint8): 0=automatic, 1=manual, 2=interrupted
 *  Byte[1]:    system_flags (uint8):
 *                Bit 0: emergency_stop   (1=紧急停车中)
 *                Bit 1: cmd_timeout      (1=CAN 指令超时)
 *                Bit 2: sbw_active       (1=SbW 功能使能中)
 *                Bit 3: chassis_enabled  (1=底盘使能回显)
 *                Bit 4-7: 保留
 *  Byte[2-3]:  linear_velocity (int16, mm/s): 前进为正, 范围 ±32.7 m/s
 *  Byte[4-5]:  angular_velocity (int16, mrad/s): 逆时针为正, 范围 ±32.7 rad/s
 *  Byte[6]:    fault_summary (uint8):
 *                Bit 0: steering_fault
 *                Bit 1: drive_fault
 *                Bit 2: braking_fault
 *                Bit 3: gear_fault
 *                Bit 4: parking_fault
 *                Bit 5-7: 保留
 *  Byte[7]:    heartbeat (uint8): 递增计数器, 每帧+1, 溢出回零
 */

/* system_flags 位域 (0x110 Byte[1]) */
#define SBW_FLAG_EMERGENCY_STOP   0x01
#define SBW_FLAG_CMD_TIMEOUT      0x02
#define SBW_FLAG_SBW_ACTIVE       0x04
#define SBW_FLAG_CHASSIS_ENABLED  0x08

/* fault_summary 位域 (0x110 Byte[6]) */
#define SBW_FAULT_STEERING        0x01
#define SBW_FAULT_DRIVE           0x02
#define SBW_FAULT_BRAKING         0x04
#define SBW_FAULT_GEAR            0x08
#define SBW_FAULT_PARKING         0x10

/*
 * 0x111 SbwSteeringFB (50Hz, 8 bytes) — 转向角 + 扭矩 + 转速
 *
 *  Byte[0-1]:  steering_angle (int16, 0.1°/LSB): 0=正中, 左转为正, ±3276.7°
 *  Byte[2-3]:  steering_torque (int16, 0.1 N·m/LSB): ±3276.7 N·m
 *  Byte[4-5]:  steering_speed (uint16, 1°/s per LSB): 0~65535°/s (无符号, 转速大小)
 *  Byte[6]:    steering_flags (uint8):
 *                Bit 0-1: steer_mode (0=auto, 1=manual, 2=interrupted)
 *                Bit 2:   config_enable
 *                Bit 3-7: 保留
 *  Byte[7]:    steering_fault (uint8): 0=正常, 1=传感器故障, 2=电机故障, 3=通信超时
 */

/* steering_flags 位域 (0x111 Byte[6]) */
#define SBW_STEER_FLAG_ENABLE     0x04
#define SBW_STEER_MODE_MASK       0x03
#define SBW_STEER_MODE_AUTO       0x00
#define SBW_STEER_MODE_MANUAL     0x01
#define SBW_STEER_MODE_INTERRUPT  0x02

/* 转向故障码 (0x111 Byte[7]) */
#define SBW_STEER_FAULT_NONE      0
#define SBW_STEER_FAULT_SENSOR    1
#define SBW_STEER_FAULT_MOTOR     2
#define SBW_STEER_FAULT_TIMEOUT   3

/*
 * 0x112 SbwDriveFB (20Hz, 8 bytes) — 车速 + 油门 + 加速度
 *
 *  Byte[0-1]:  vehicle_speed (uint16, 0.1 km/h/LSB): 0~6553.5 km/h (方向由档位体现)
 *  Byte[2]:    accel_pedal_pos (uint8, 1%/LSB): 0~100%
 *  Byte[3-4]:  long_accel (int16, 0.01 m/s²/LSB): 正=加速, 负=减速, ±327.67 m/s²
 *  Byte[5]:    drive_flags (uint8):
 *                Bit 0-1: drive_mode (0=auto, 1=manual, 2=interrupted)
 *                Bit 2:   config_enable
 *                Bit 3:   manual_takeover
 *                Bit 4-7: 保留
 *  Byte[6]:    drive_fault (uint8): 0=正常, 1=电机故障, 2=过流, 3=通信超时
 *  Byte[7]:    保留
 */

/* drive_flags 位域 (0x112 Byte[5]) */
#define SBW_DRIVE_FLAG_ENABLE     0x04
#define SBW_DRIVE_FLAG_TAKEOVER   0x08
#define SBW_DRIVE_MODE_MASK       0x03
#define SBW_DRIVE_MODE_AUTO       0x00
#define SBW_DRIVE_MODE_MANUAL     0x01
#define SBW_DRIVE_MODE_INTERRUPT  0x02

/* 驱动故障码 (0x112 Byte[6]) */
#define SBW_DRIVE_FAULT_NONE      0
#define SBW_DRIVE_FAULT_MOTOR     1
#define SBW_DRIVE_FAULT_OVERCUR   2
#define SBW_DRIVE_FAULT_TIMEOUT   3

/*
 * 0x113 SbwBrakingFB (20Hz, 8 bytes) — 制动踏板 + 减速度 + 制动灯
 *
 *  Byte[0-1]:  brake_pedal_pos (uint16, 0.1%/LSB): 0~65535, 实际 0~1000=0%~100.0%
 *  Byte[2]:    target_decel (uint8, 0.1 m/s²/LSB): 0~25.5 m/s² (无符号)
 *  Byte[3]:    brake_flags (uint8):
 *                Bit 0-1: brake_mode (0=auto, 1=manual, 2=interrupted)
 *                Bit 2:   config_enable
 *                Bit 3:   manual_takeover
 *                Bit 4:   brake_light
 *                Bit 5-7: 保留
 *  Byte[4]:    brake_fault (uint8): 0=正常, 1=系统故障, 2=制动灯故障, 3=通信超时
 *  Byte[5-7]:  保留
 */

/* brake_flags 位域 (0x113 Byte[3]) */
#define SBW_BRAKE_FLAG_ENABLE     0x04
#define SBW_BRAKE_FLAG_TAKEOVER   0x08
#define SBW_BRAKE_FLAG_LIGHT      0x10
#define SBW_BRAKE_MODE_MASK       0x03
#define SBW_BRAKE_MODE_AUTO       0x00
#define SBW_BRAKE_MODE_MANUAL     0x01
#define SBW_BRAKE_MODE_INTERRUPT  0x02

/* 制动故障码 (0x113 Byte[4]) */
#define SBW_BRAKE_FAULT_NONE      0
#define SBW_BRAKE_FAULT_SYSTEM    1
#define SBW_BRAKE_FAULT_LIGHT     2
#define SBW_BRAKE_FAULT_TIMEOUT   3

/*
 * 0x114 SbwGearParkingFB (10Hz, 8 bytes) — 档位 + EPB 状态
 *
 *  Byte[0]:    gear_position (uint8): 0=P, 1=R, 2=N, 3=D
 *  Byte[1]:    gear_fault (uint8): 0=正常, 1=传感器故障, 2=执行器故障,
 *                                   3=换档超时, 4=安全互锁拒绝
 *  Byte[2]:    epb_status (uint8):
 *                Bit 0: epb_clamped    (0=释放, 1=夹紧)
 *                Bit 1: parking_status (0=未驻车, 1=已驻车)
 *                Bit 2-7: 保留
 *  Byte[3]:    parking_fault (uint8): 0=正常, 1=EPB左故障, 2=EPB右故障, 3=超时
 *  Byte[4-7]:  保留
 */

/* epb_status 位域 (0x114 Byte[2]) */
#define SBW_EPB_CLAMPED            0x01
#define SBW_EPB_PARKING_STATUS     0x02

/* 档位故障码 (0x114 Byte[1]) */
#define SBW_GEAR_FAULT_NONE        0
#define SBW_GEAR_FAULT_SENSOR      1
#define SBW_GEAR_FAULT_ACTUATOR    2
#define SBW_GEAR_FAULT_TIMEOUT     3
#define SBW_GEAR_FAULT_INTERLOCK   4

/* 驻车故障码 (0x114 Byte[3]) */
#define SBW_PARKING_FAULT_NONE     0
#define SBW_PARKING_FAULT_LEFT     1
#define SBW_PARKING_FAULT_RIGHT    2
#define SBW_PARKING_FAULT_TIMEOUT  3

/*
 * 0x115 SbwMCDetail (5Hz, 8 bytes) — MC 内部状态调试帧
 *
 *  Byte[0-1]:  cmd_linear_v (int16, mm/s): MC 输出的目标线速度
 *  Byte[2-3]:  cmd_angular_w (int16, mrad/s): MC 输出的目标角速度
 *  Byte[4]:    mc_flags (uint8):
 *                Bit 0: braking_active
 *                Bit 1: emergency_stop
 *                Bit 2: safety_gate_active
 *                Bit 3-7: 保留
 *  Byte[5]:    mc_layer (uint8): 当前仲裁层级 0-4
 *  Byte[6]:    heartbeat (uint8): 与 0x110 同步
 *  Byte[7]:    保留
 */

/* mc_flags 位域 (0x115 Byte[4]) */
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

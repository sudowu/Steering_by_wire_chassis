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
        /* ---- 调试帧：直接底盘控制（不参与模式逻辑）---- */
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

        /* ---- SbW 自动转向帧 (ID 0x200) ----
         * Byte[0]: Steering_Config_Enable (0=off, 1=on)
         * Byte[1]: Target_Steering_Angle (uint8, 0.5 deg/LSB, 128=center)
         * Byte[2]: Target_Steering_Speed (uint8, 1 deg/s per LSB)
         * Byte[3-7]: 保留
         */
        case CAN_ID_SBW_AUTO_STEERING:
        {
            g_chassis_auto.Steering_Function.Steering_Control.Steering_Config_Enable
                = msg->Data[0];
            g_chassis_auto.Steering_Function.Steering_Control.Target_Steering_Angle
                = msg->Data[1];
            g_chassis_auto.Steering_Function.Steering_Control.Target_Steering_Speed
                = msg->Data[2];
            g_chassis_auto.Last_Command_Tick = HAL_GetTick();
            break;
        }

        /* ---- SbW 自动驱动帧 (ID 0x201) ----
         * Byte[0]: Driving_Config_Enable (0=off, 1=on)
         * Byte[1]: Target_Accelerator_pedal_Position (uint8, 1% per LSB)
         * Byte[2]: Target_Longitudinal_Acceleration (uint8, 0.1 m/s² per LSB)
         * Byte[3]: Target_Driving_Torque (uint8, 1 N·m per LSB)
         * Byte[4-7]: 保留
         */
        case CAN_ID_SBW_AUTO_DRIVE:
        {
            g_chassis_auto.Drive_Function.Drive_Control.Driving_Config_Enable
                = msg->Data[0];
            g_chassis_auto.Drive_Function.Drive_Control.Target_Accelerator_pedal_Position
                = msg->Data[1];
            g_chassis_auto.Drive_Function.Drive_Control.Target_Longitudinal_Acceleration
                = msg->Data[2];
            g_chassis_auto.Drive_Function.Drive_Control.Target_Driving_Torque
                = msg->Data[3];
            g_chassis_auto.Last_Command_Tick = HAL_GetTick();
            break;
        }

        /* ---- SbW 自动档位帧 (ID 0x202) ----
         * Byte[0]: Gear_Config_Enable (0=off, 1=on)
         * Byte[1]: Target_Gear_Position (uint8: 0=P, 1=R, 2=N, 3=D)
         * Byte[2]: Parking_Config_Enable (0=off, 1=on)
         * Byte[3]: Parking_Request (0=释放, 1=夹紧)
         * Byte[4-7]: 保留
         */
        case CAN_ID_SBW_AUTO_GEAR:
        {
            g_chassis_auto.Gear_Function.Gear_Control.Gear_Config_Enable
                = msg->Data[0];
            g_chassis_auto.Gear_Function.Gear_Control.Target_Gear_Position
                = (Gear_Position)msg->Data[1];
            g_chassis_auto.Parking_Function.Parking_Control.Parking_Config_Enable
                = msg->Data[2];
            g_chassis_auto.Parking_Function.Parking_Control.Parking_Request
                = msg->Data[3];
            g_chassis_auto.Last_Command_Tick = HAL_GetTick();
            break;
        }

        /* ---- SbW 自动制动帧 (ID 0x203) ----
         * Byte[0]: Braking_Config_Enable (0=off, 1=on)
         * Byte[1]: Target_Deceleration (uint8, 0.1 m/s² per LSB)
         * Byte[2]: Break_Light_Control (0=熄灯, 1=亮灯)
         * Byte[3-7]: 保留
         */
        case CAN_ID_SBW_AUTO_BRAKE:
        {
            g_chassis_auto.Braking_Function.Braking_Control.Braking_Config_Enable
                = msg->Data[0];
            g_chassis_auto.Braking_Function.Braking_Control.Target_Deceleration
                = msg->Data[1];
            g_chassis_auto.Braking_Function.Braking_Control.Break_Light_Control
                = msg->Data[2];
            g_chassis_auto.Last_Command_Tick = HAL_GetTick();
            break;
        }

        /* ---- SbW 物理档位帧 (ID 0x210) ----
         * Byte[0]: Gear_Position (uint8: 0=P, 1=R, 2=N, 3=D)
         * Byte[1-7]: 保留
         */
        case CAN_ID_SBW_MANUAL_GEAR:
        {
            g_chassis_manual.Gear_Function.Gear_Control.Gear_Config_Enable = 1;
            g_chassis_manual.Gear_Function.Gear_Control.Target_Gear_Position
                = (Gear_Position)msg->Data[0];
            break;
        }

        /* ---- SbW 物理方向盘帧 (ID 0x211) ----
         * Byte[0]: Steering_Angle (uint8, 0.5 deg/LSB, 128=center)
         * Byte[1]: Steering_Torque (uint8, 0.1 Nm per LSB)
         * Byte[2-7]: 保留
         */
        case CAN_ID_SBW_MANUAL_STEERING:
        {
            g_chassis_manual.Steering_Function.Steering_Control.Steering_Config_Enable = 1;
            g_chassis_manual.Steering_Function.Steering_Control.Target_Steering_Angle
                = msg->Data[0];
            /* 物理方向盘扭矩暂复用 Steering_Speed 字段传递 */
            g_chassis_manual.Steering_Function.Steering_Feedback.Steering_Speed
                = msg->Data[1];
            break;
        }

        /* ---- SbW 物理油门踏板帧 (ID 0x212) ----
         * Byte[0]: Accelerator_Pedal_Position (uint8, 1% per LSB)
         * Byte[1-7]: 保留
         */
        case CAN_ID_SBW_MANUAL_ACCEL:
        {
            g_chassis_manual.Drive_Function.Drive_Control.Driving_Config_Enable = 1;
            g_chassis_manual.Drive_Function.Drive_Control.Target_Accelerator_pedal_Position
                = msg->Data[0];
            break;
        }

        /* ---- SbW 物理制动踏板帧 (ID 0x213) ----
         * Byte[0]: Braking_Pedal_Position (uint8, 0.1% per LSB)
         * Byte[1-7]: 保留
         */
        case CAN_ID_SBW_MANUAL_BRAKE:
        {
            g_chassis_manual.Braking_Function.Braking_Control.Braking_Config_Enable = 1;
            g_chassis_manual.Braking_Function.Braking_Control.Target_Braking_Pedal_Position
                = msg->Data[0];
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

/* ==================== SbW 状态反馈发送函数 ==================== */

/**
 * @brief 发送车辆运动状态帧 (CAN ID 0x110, 50Hz)
 */
void CAN_SendSbwVehicleState(const Chassis_t* c, driving_mode mode,
                             uint8_t system_flags, uint8_t fault_summary)
{
    CAN_Message_t* tx = pvPortMalloc(sizeof(CAN_Message_t));
    if (tx == NULL) return;

    static uint8_t heartbeat = 0;

    int16_t linear  = (int16_t)(c->actual_linear_vel * 1000.0f);  // m/s → mm/s
    int16_t angular = (int16_t)(c->actual_angular_vel * 1000.0f); // rad/s → mrad/s

    /* 基础状态标志（由调用方提供 emergency_stop 和 sbw_active）*/
    if (!Chassis_IsCommandValid(c))  system_flags |= SBW_FLAG_CMD_TIMEOUT;
    if (c->cmd_enable)                system_flags |= SBW_FLAG_CHASSIS_ENABLED;

    tx->StdId = CAN_ID_SBW_VEHICLE_STATE;
    tx->Len   = 8;
    tx->Data[0] = (uint8_t)mode;
    tx->Data[1] = system_flags;
    tx->Data[2] = (uint8_t)(linear & 0xFF);
    tx->Data[3] = (uint8_t)((linear >> 8) & 0xFF);
    tx->Data[4] = (uint8_t)(angular & 0xFF);
    tx->Data[5] = (uint8_t)((angular >> 8) & 0xFF);
    tx->Data[6] = fault_summary;
    tx->Data[7] = heartbeat++;

    if (xQueueSendToBack(canTxQueue, &tx, 0) != pdTRUE)
        vPortFree(tx);
}

/**
 * @brief 发送转向反馈帧 (CAN ID 0x111, 50Hz)
 */
void CAN_SendSbwSteeringFB(const Steering_Function* sf)
{
    CAN_Message_t* tx = pvPortMalloc(sizeof(CAN_Message_t));
    if (tx == NULL) return;

    int16_t angle  = (int16_t)(sf->Steering_Feedback.Steering_Angle);           // 0.1°/LSB (同 scale)
    int16_t torque = 0; /* 当前无扭矩传感器, 预留 */
    uint16_t speed  = (uint16_t)(sf->Steering_Feedback.Steering_Speed);       // 1°/s

    /* steering_flags */
    uint8_t flags = 0;
    flags |= (sf->Steering_Feedback.Steering_mode & 0x03);
    if (sf->Steering_Control.Steering_Config_Enable) flags |= SBW_STEER_FLAG_ENABLE;

    tx->StdId = CAN_ID_SBW_STEERING_FB;
    tx->Len   = 8;
    tx->Data[0] = (uint8_t)(angle & 0xFF);
    tx->Data[1] = (uint8_t)((angle >> 8) & 0xFF);
    tx->Data[2] = (uint8_t)(torque & 0xFF);
    tx->Data[3] = (uint8_t)((torque >> 8) & 0xFF);
    tx->Data[4] = (uint8_t)(speed & 0xFF);
    tx->Data[5] = (uint8_t)((speed >> 8) & 0xFF);
    tx->Data[6] = flags;
    tx->Data[7] = sf->Steering_Feedback.Fault_message;

    if (xQueueSendToBack(canTxQueue, &tx, 0) != pdTRUE)
        vPortFree(tx);
}

/**
 * @brief 发送驱动反馈帧 (CAN ID 0x112, 20Hz)
 */
void CAN_SendSbwDriveFB(const Drive_Function* df)
{
    CAN_Message_t* tx = pvPortMalloc(sizeof(CAN_Message_t));
    if (tx == NULL) return;

    uint16_t speed  = (uint16_t)(df->Drive_Feedback.Vehicle_Speed);              // 0.1 km/h
    int16_t  accel  = (int16_t)(df->Drive_Feedback.Longitudinal_Acceleration * 10); // uint8@0.1→int16@0.01 m/s²
    uint8_t  pedal  = df->Drive_Feedback.Accelerator_pedal_Position;             // 1%

    /* drive_flags */
    uint8_t flags = 0;
    flags |= (df->Drive_Feedback.Driving_mode & 0x03);
    if (df->Drive_Control.Driving_Config_Enable) flags |= SBW_DRIVE_FLAG_ENABLE;
    if (df->Manual_Takeover)                     flags |= SBW_DRIVE_FLAG_TAKEOVER;

    tx->StdId = CAN_ID_SBW_DRIVE_FB;
    tx->Len   = 8;
    tx->Data[0] = (uint8_t)(speed & 0xFF);
    tx->Data[1] = (uint8_t)((speed >> 8) & 0xFF);
    tx->Data[2] = pedal;
    tx->Data[3] = (uint8_t)(accel & 0xFF);
    tx->Data[4] = (uint8_t)((accel >> 8) & 0xFF);
    tx->Data[5] = flags;
    tx->Data[6] = df->Drive_Feedback.Fault_message;
    tx->Data[7] = 0;

    if (xQueueSendToBack(canTxQueue, &tx, 0) != pdTRUE)
        vPortFree(tx);
}

/**
 * @brief 发送制动反馈帧 (CAN ID 0x113, 20Hz)
 */
void CAN_SendSbwBrakingFB(const Braking_Function* bf)
{
    CAN_Message_t* tx = pvPortMalloc(sizeof(CAN_Message_t));
    if (tx == NULL) return;

    uint16_t pedal = (uint16_t)(bf->Braking_Feedback.Braking_Pedal_Position); // 0.1%

    /* brake_flags */
    uint8_t flags = 0;
    flags |= (bf->Braking_Feedback.Current_Braking_System_Driving_Mode & 0x03);
    if (bf->Braking_Control.Braking_Config_Enable) flags |= SBW_BRAKE_FLAG_ENABLE;
    if (bf->Manual_Takeover)                       flags |= SBW_BRAKE_FLAG_TAKEOVER;
    if (bf->Braking_Feedback.Brake_Light_Status)   flags |= SBW_BRAKE_FLAG_LIGHT;

    tx->StdId = CAN_ID_SBW_BRAKING_FB;
    tx->Len   = 8;
    tx->Data[0] = (uint8_t)(pedal & 0xFF);
    tx->Data[1] = (uint8_t)((pedal >> 8) & 0xFF);
    tx->Data[2] = bf->Braking_Control.Target_Deceleration;
    tx->Data[3] = flags;
    tx->Data[4] = bf->Braking_Feedback.Fault_message;
    tx->Data[5] = 0;
    tx->Data[6] = 0;
    tx->Data[7] = 0;

    if (xQueueSendToBack(canTxQueue, &tx, 0) != pdTRUE)
        vPortFree(tx);
}

/**
 * @brief 发送档位 & 驻车反馈帧 (CAN ID 0x114, 10Hz)
 */
void CAN_SendSbwGearParkingFB(const Gear_Function* gf,
                              const Parking_Function* pf)
{
    CAN_Message_t* tx = pvPortMalloc(sizeof(CAN_Message_t));
    if (tx == NULL) return;

    /* epb_status */
    uint8_t epb = 0;
    if (pf->Parking_Feedback.EPB_Switch_status)        epb |= SBW_EPB_CLAMPED;
    if (pf->Parking_Feedback.Parking_status_feedback)   epb |= SBW_EPB_PARKING_STATUS;

    tx->StdId = CAN_ID_SBW_GEAR_PARKING_FB;
    tx->Len   = 8;
    tx->Data[0] = (uint8_t)gf->Gear_Feedback.Gear_Position_Status;
    tx->Data[1] = gf->Gear_Feedback.Fault_message;
    tx->Data[2] = epb;
    tx->Data[3] = pf->Parking_Feedback.Fault_message_for_parking_system;
    tx->Data[4] = 0;
    tx->Data[5] = 0;
    tx->Data[6] = 0;
    tx->Data[7] = 0;

    if (xQueueSendToBack(canTxQueue, &tx, 0) != pdTRUE)
        vPortFree(tx);
}

/**
 * @brief 发送运动控制调试帧 (CAN ID 0x115, 5Hz)
 */
void CAN_SendSbwMCDetail(const MotionControl_State_t* mc,
                         const Chassis_t* c)
{
    CAN_Message_t* tx = pvPortMalloc(sizeof(CAN_Message_t));
    if (tx == NULL) return;

    int16_t v_cmd = (int16_t)(c->cmd_linear_vel * 1000.0f);    // m/s → mm/s
    int16_t w_cmd = (int16_t)(c->cmd_angular_vel * 1000.0f);   // rad/s → mrad/s

    /* mc_flags */
    uint8_t mc_flags = 0;
    if (mc->braking_active)   mc_flags |= SBW_MC_FLAG_BRAKING;
    if (mc->emergency_stop)   mc_flags |= SBW_MC_FLAG_EMERGENCY;

    tx->StdId = CAN_ID_SBW_MC_DETAIL;
    tx->Len   = 8;
    tx->Data[0] = (uint8_t)(v_cmd & 0xFF);
    tx->Data[1] = (uint8_t)((v_cmd >> 8) & 0xFF);
    tx->Data[2] = (uint8_t)(w_cmd & 0xFF);
    tx->Data[3] = (uint8_t)((w_cmd >> 8) & 0xFF);
    tx->Data[4] = mc_flags;
    tx->Data[5] = 0; /* mc_layer 暂未暴露, 预留 */
    tx->Data[6] = 0; /* heartbeat 由调用方填充 */
    tx->Data[7] = 0;

    if (xQueueSendToBack(canTxQueue, &tx, 0) != pdTRUE)
        vPortFree(tx);
}

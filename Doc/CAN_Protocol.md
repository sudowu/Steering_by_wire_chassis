# 线控底盘 CAN 应用层协议

> 版本: v2.0 | 更新: 2026-07-29 | 硬件: STM32F407IG CAN1 @ 1Mbps

---

## 1. 硬件层

| 项 | 配置 |
|---|------|
| 外设 | CAN1 (hcan1) |
| GPIO | PI9 (RX), PB9 (TX) |
| 波特率 | 1 Mbps (42MHz APB1 / 6 / 7tq) |
| 滤波器 | Bank 0, 32位 ID 掩码模式, 掩码=0x00000000 (接收全部标准帧) |
| 中断 | TX_IRQn (Preempt 7), RX0_IRQn (Preempt 7) |
| 消息结构 | `CAN_Message_t`: StdId(uint32) + Len(uint8) + Data[8] |
| TX 队列 | `canTxQueue` (指针传递, 深度 10, FreeRTOS 堆分配) |
| RX 队列 | `canRxQueue` (值传递, 深度 10) |

---

## 2. ID 分配总览

```
0x100-0x103  底盘调试帧（电机直控/状态/诊断）
0x110-0x115  SbW 状态反馈帧（底盘 → 上位机）
0x200-0x203  SbW 自动驾驶指令帧（上位机 → 底盘）
0x210-0x213  SbW 物理驾驶输入帧（物理设备 → 底盘）
0x220        SbW 全线控使能帧（上位机 → 底盘）
```

---

## 3. 底盘调试帧 (0x100-0x103)

> 电机直控通道，不参与 SbW 模式逻辑。仅当 SbW 未使能时生效。

### 3.1 0x100 — 底盘速度指令 (RX)

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0-1] | 线速度 | int16 | mm/s | 小端序 |
| Byte[2-3] | 角速度 | int16 | mrad/s | 小端序 |
| Byte[4] Bit 0 | 使能 | — | — | 1=电机通电, 0=断电 |
| Byte[4] Bit 1-7 | 保留 | — | — | |
| Byte[5-7] | 保留 | — | — | |

宏: `CHASSIS_FLAG_ENABLE = 0x01`

### 3.2 0x101 — 底盘综合状态 (TX, ~100Hz)

> 合并原 0x101 状态帧与 0x102 转速帧。

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0-1] | 左电机转速 | int16 | RPM | 小端序 |
| Byte[2-3] | 右电机转速 | int16 | RPM | 小端序 |
| Byte[4-5] | 保留 | — | — | |
| Byte[6] Bit 0 | 左电机运行 | — | — | |
| Byte[6] Bit 1 | 右电机运行 | — | — | |
| Byte[6] Bit 2 | 系统使能回显 | — | — | |
| Byte[6] Bit 3 | 命令有效 | — | — | 1=未超时 |
| Byte[6] Bit 4-7 | 保留 | — | — | |
| Byte[7] | 心跳计数 | uint8 | — | 每帧+1, 溢出回零 |

宏: `CHASSIS_STATUS_LEFT_RUN=0x01`, `CHASSIS_STATUS_RIGHT_RUN=0x02`, `CHASSIS_STATUS_ENABLED=0x04`, `CHASSIS_STATUS_CMD_VALID=0x08`

### 3.3 0x103 — 底盘诊断 (TX, ~10Hz)

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0] Bit 0 | 左电机过流 | — | — | 触发阈值 >10A |
| Byte[0] Bit 1 | 右电机过流 | — | — | |
| Byte[0] Bit 2 | CAN Bus-Off | — | — | 发生过即锁存 |
| Byte[0] Bit 3 | 命令超时 | — | — | |
| Byte[0] Bit 4-7 | 保留 | — | — | |
| Byte[1] | 左电机 PWM | uint8 | 0.4%/LSB | 0-250 |
| Byte[2] | 右电机 PWM | uint8 | 0.4%/LSB | 0-250 |
| Byte[3-4] | 左电机 PID 误差 | int16 | RPM | 小端序 |
| Byte[5-6] | 右电机 PID 误差 | int16 | RPM | 小端序 |
| Byte[7] | 保留 | — | — | |

宏: `CHASSIS_FAULT_LEFT_OC=0x01`, `CHASSIS_FAULT_RIGHT_OC=0x02`, `CHASSIS_FAULT_CAN_BUSOFF=0x04`, `CHASSIS_FAULT_CMD_TIMEOUT=0x08`

---

## 4. SbW 全线控使能帧 (0x220)

### 4.1 0x220 — SbW 全线控使能 (RX, 按需)

| 字节 | 字段 | 类型 | 说明 |
|------|------|------|------|
| Byte[0] | SbW_Enable | uint8 | 0=关闭全线控, 非0=开启全线控 |
| Byte[1-7] | 保留 | — | |

同时控制 `g_chassis_auto` 和 `g_chassis_manual` 的驱动/转向/制动三个模块 Config_Enable。

---

## 5. SbW 自动驾驶指令帧 (0x200-0x203, RX)

> 由上层自动驾驶平台（如 Apollo）发送，写入 `g_chassis_auto` 实例。

### 5.1 0x200 — 自动转向指令

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0] | Steering_Config_Enable | uint8 | — | 0=关闭, 1=使能 |
| Byte[1] | Target_Steering_Angle | int8 | 0.5°/LSB | 0=正中, ±128→±64° |
| Byte[2] | Target_Steering_Speed | uint8 | 1°/s | |
| Byte[3-7] | 保留 | — | — | |

### 5.2 0x201 — 自动驱动指令

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0] | Driving_Config_Enable | uint8 | — | 0=关闭, 1=使能 |
| Byte[1] | Target_Accelerator_pedal_Position | uint8 | 1%/LSB | 备选控制量 |
| Byte[2] | Target_Longitudinal_Acceleration | uint8 | 0.1 m/s²/LSB | 首选控制量 |
| Byte[3] | Target_Driving_Torque | uint8 | 1 N·m/LSB | |
| Byte[4-7] | 保留 | — | — | |

### 5.3 0x202 — 自动档位 + 驻车指令

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0] | Gear_Config_Enable | uint8 | — | 0=关闭, 1=使能 |
| Byte[1] | Target_Gear_Position | uint8 | — | 0=P, 1=R, 2=N, 3=D |
| Byte[2] | Parking_Config_Enable | uint8 | — | 0=关闭, 1=使能 |
| Byte[3] | Parking_Request | uint8 | — | 0=释放, 1=夹紧 |
| Byte[4-7] | 保留 | — | — | |

### 5.4 0x203 — 自动制动指令

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0] | Braking_Config_Enable | uint8 | — | 0=关闭, 1=使能 |
| Byte[1] | Target_Deceleration | uint8 | 0.1 m/s²/LSB | |
| Byte[2] | Break_Light_Control | uint8 | — | 0=熄灯, 1=亮灯 |
| Byte[3-7] | 保留 | — | — | |

---

## 6. SbW 物理驾驶输入帧 (0x210-0x213, RX)

> 由物理驾驶舱设备发送，写入 `g_chassis_manual` 实例，同时用于人工接管检测。

### 6.1 0x210 — 物理档位

| 字节 | 字段 | 类型 | 说明 |
|------|------|------|------|
| Byte[0] | Gear_Position | uint8 | 0=P, 1=R, 2=N, 3=D |
| Byte[1-7] | 保留 | — | |

写入时 Gear_Config_Enable 强制置 1。

### 6.2 0x211 — 物理方向盘

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0] | Steering_Angle | int8 | 0.5°/LSB | 0=正中, ±128→±64° |
| Byte[1] | Steering_Torque | uint8 | 0.1 N·m/LSB | 暂复用 Steering_Speed 字段传递 |
| Byte[2-7] | 保留 | — | — | |

写入时 Steering_Config_Enable 强制置 1。

### 6.3 0x212 — 物理油门踏板

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0] | Accelerator_Pedal_Position | uint8 | 1%/LSB | |
| Byte[1-7] | 保留 | — | — | |

写入时 Driving_Config_Enable 强制置 1。

### 6.4 0x213 — 物理制动踏板

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0] | Braking_Pedal_Position | uint8 | 0.1%/LSB | |
| Byte[1-7] | 保留 | — | — | |

写入时 Braking_Config_Enable 强制置 1。

---

## 7. SbW 状态反馈帧 (0x110-0x115, TX)

> 由 `vTaskSbwControl` 按优先级分频发送，以活跃实例 (auto/manual) 的真实物理状态为数据源。

| ID | 帧名 | 频率 | 周期 |
|----|------|------|------|
| 0x110 | SbwVehicleState | 50Hz | 每 2 周期 |
| 0x111 | SbwSteeringFB | 50Hz | 每 2 周期 |
| 0x112 | SbwDriveFB | 20Hz | 每 5 周期 |
| 0x113 | SbwBrakingFB | 20Hz | 每 5 周期 |
| 0x114 | SbwGearParkingFB | 10Hz | 每 10 周期 |
| 0x115 | SbwMCDetail | 5Hz | 每 20 周期 |

### 7.1 0x110 — 车辆运动状态 + 系统模式 + 故障汇总

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0] | driving_mode | uint8 | — | 0=automatic, 1=manual, 2=interrupted |
| Byte[1] Bit 0 | emergency_stop | — | — | 1=紧急停车中 |
| Byte[1] Bit 1 | cmd_timeout | — | — | 1=CAN 指令超时 |
| Byte[1] Bit 2 | sbw_active | — | — | 1=SbW 功能使能中 |
| Byte[1] Bit 3 | chassis_enabled | — | — | 1=底盘使能回显 |
| Byte[1] Bit 4-7 | 保留 | — | — | |
| Byte[2-3] | linear_velocity | int16 | mm/s | 前进为正, ±32.7 m/s |
| Byte[4-5] | angular_velocity | int16 | mrad/s | 逆时针为正, ±32.7 rad/s |
| Byte[6] Bit 0 | steering_fault | — | — | |
| Byte[6] Bit 1 | drive_fault | — | — | |
| Byte[6] Bit 2 | braking_fault | — | — | |
| Byte[6] Bit 3 | gear_fault | — | — | |
| Byte[6] Bit 4 | parking_fault | — | — | |
| Byte[6] Bit 5-7 | 保留 | — | — | |
| Byte[7] | heartbeat | uint8 | — | 每帧+1, 溢出回零 |

宏:
- system_flags: `SBW_FLAG_EMERGENCY_STOP=0x01`, `SBW_FLAG_CMD_TIMEOUT=0x02`, `SBW_FLAG_SBW_ACTIVE=0x04`, `SBW_FLAG_CHASSIS_ENABLED=0x08`
- fault_summary: `SBW_FAULT_STEERING=0x01`, `SBW_FAULT_DRIVE=0x02`, `SBW_FAULT_BRAKING=0x04`, `SBW_FAULT_GEAR=0x08`, `SBW_FAULT_PARKING=0x10`

### 7.2 0x111 — 转向反馈

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0-1] | steering_angle | int16 | 0.1°/LSB | 0=正中, 左转为正, ±3276.7° |
| Byte[2-3] | steering_torque | int16 | 0.1 N·m/LSB | 当前无传感器, 预留为 0 |
| Byte[4-5] | steering_speed | uint16 | 1°/s | 无符号, 表示幅值 |
| Byte[6] Bit 0-1 | steer_mode | — | — | 0=auto, 1=manual, 2=interrupted |
| Byte[6] Bit 2 | config_enable | — | — | |
| Byte[6] Bit 3-7 | 保留 | — | — | |
| Byte[7] | steering_fault | uint8 | — | 0=正常, 1=传感器, 2=电机, 3=超时 |

宏: `SBW_STEER_MODE_AUTO=0`, `SBW_STEER_MODE_MANUAL=1`, `SBW_STEER_MODE_INTERRUPT=2`, `SBW_STEER_FLAG_ENABLE=0x04`, `SBW_STEER_MODE_MASK=0x03`

### 7.3 0x112 — 驱动反馈

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0-1] | vehicle_speed | uint16 | 0.1 km/h | 方向由档位体现 |
| Byte[2] | accel_pedal_pos | uint8 | 1%/LSB | 0-100% |
| Byte[3-4] | long_accel | int16 | 0.01 m/s² | 正=加速, 负=减速 |
| Byte[5] Bit 0-1 | drive_mode | — | — | 0=auto, 1=manual, 2=interrupted |
| Byte[5] Bit 2 | config_enable | — | — | |
| Byte[5] Bit 3 | manual_takeover | — | — | 1=人工接管中 |
| Byte[5] Bit 4-7 | 保留 | — | — | |
| Byte[6] | drive_fault | uint8 | — | 0=正常, 1=电机, 2=过流, 3=超时 |
| Byte[7] | 保留 | — | — | |

宏: `SBW_DRIVE_FLAG_ENABLE=0x04`, `SBW_DRIVE_FLAG_TAKEOVER=0x08`, `SBW_DRIVE_MODE_MASK=0x03`

### 7.4 0x113 — 制动反馈

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0-1] | brake_pedal_pos | uint16 | 0.1%/LSB | 0~1000 = 0%~100.0% |
| Byte[2] | target_decel | uint8 | 0.1 m/s² | 无符号 |
| Byte[3] Bit 0-1 | brake_mode | — | — | 0=auto, 1=manual, 2=interrupted |
| Byte[3] Bit 2 | config_enable | — | — | |
| Byte[3] Bit 3 | manual_takeover | — | — | |
| Byte[3] Bit 4 | brake_light | — | — | 1=制动灯亮 |
| Byte[3] Bit 5-7 | 保留 | — | — | |
| Byte[4] | brake_fault | uint8 | — | 0=正常, 1=系统, 2=灯, 3=超时 |
| Byte[5-7] | 保留 | — | — | |

宏: `SBW_BRAKE_FLAG_ENABLE=0x04`, `SBW_BRAKE_FLAG_TAKEOVER=0x08`, `SBW_BRAKE_FLAG_LIGHT=0x10`, `SBW_BRAKE_MODE_MASK=0x03`

### 7.5 0x114 — 档位 + 驻车反馈

| 字节 | 字段 | 类型 | 说明 |
|------|------|------|------|
| Byte[0] | gear_position | uint8 | 0=P, 1=R, 2=N, 3=D |
| Byte[1] | gear_fault | uint8 | 0=正常, 1=传感器, 2=执行器, 3=换档超时, 4=安全互锁拒绝 |
| Byte[2] Bit 0 | epb_clamped | — | 0=释放, 1=夹紧 |
| Byte[2] Bit 1 | parking_status | — | 0=未驻车, 1=已驻车 |
| Byte[2] Bit 2-7 | 保留 | — | |
| Byte[3] | parking_fault | uint8 | 0=正常, 1=EPB左故障, 2=EPB右故障, 3=超时 |
| Byte[4-7] | 保留 | — | |

宏: `SBW_EPB_CLAMPED=0x01`, `SBW_EPB_PARKING_STATUS=0x02`

### 7.6 0x115 — MC 内部状态调试帧

| 字节 | 字段 | 类型 | 单位 | 说明 |
|------|------|------|------|------|
| Byte[0-1] | cmd_linear_v | int16 | mm/s | MC 输出的目标线速度 |
| Byte[2-3] | cmd_angular_w | int16 | mrad/s | MC 输出的目标角速度 |
| Byte[4] Bit 0 | braking_active | — | — | |
| Byte[4] Bit 1 | emergency_stop | — | — | |
| Byte[4] Bit 2 | safety_gate_active | — | — | |
| Byte[4] Bit 3-7 | 保留 | — | — | |
| Byte[5] | mc_layer | uint8 | — | 当前仲裁层级 0-4 (预留) |
| Byte[6] | heartbeat | uint8 | — | (预留) |
| Byte[7] | 保留 | — | — | |

宏: `SBW_MC_FLAG_BRAKING=0x01`, `SBW_MC_FLAG_EMERGENCY=0x02`, `SBW_MC_FLAG_SAFETY_GATE=0x04`

---

## 8. 处理流程

### 8.1 RX 处理 (CAN_ProcessRxMessage)

| ID | 帧名 | 处理逻辑 |
|----|------|----------|
| 0x100 | CHASSIS_CMD | `SbwIsActive` 门控 → 解析 v/ω → 写 `g_chassis` |
| 0x220 | SBW_CONTROL | Byte[0] ? SbwEnableAll : SbwDisableAll → 双实例 |
| 0x200 | AUTO_STEERING | → `g_chassis_auto.Steering_Function` |
| 0x201 | AUTO_DRIVE | → `g_chassis_auto.Drive_Function` |
| 0x202 | AUTO_GEAR | → `g_chassis_auto.Gear_Function` + `Parking_Function` |
| 0x203 | AUTO_BRAKE | → `g_chassis_auto.Braking_Function` |
| 0x210 | MANUAL_GEAR | Gear_Config_Enable=1 + Position → `g_chassis_manual` |
| 0x211 | MANUAL_STEERING | Steering_Config_Enable=1 + Angle/Torque → `g_chassis_manual` |
| 0x212 | MANUAL_ACCEL | Driving_Config_Enable=1 + Pedal → `g_chassis_manual` |
| 0x213 | MANUAL_BRAKE | Braking_Config_Enable=1 + Pedal → `g_chassis_manual` |

### 8.2 TX 发送源

| 帧 | 发送源 | 函数 |
|----|--------|------|
| 0x101 | vTask_SpeedControl (100Hz) | `CAN_SendChassisStatus()` |
| 0x103 | vTask_SpeedControl (每 100ms) | `CAN_SendChassisDiag()` |
| 0x110-0x115 | vTaskSbwControl (分频) | `CAN_SendSbw*()` |

---

## 9. 相关源文件

| 文件 | 内容 |
|------|------|
| [TaskCanInfo.h](../APP/Tasks/TaskCanInfo.h) | CAN ID 宏定义、位标志、数据结构、函数声明 |
| [TaskCanInfo.c](../APP/Tasks/TaskCanInfo.c) | RX 消息分发、TX 发送函数、CAN_Send_HAL |
| [TaskSbwControl.c](../APP/Tasks/TaskSbwControl.c) | 100Hz 主循环, 分频调用反馈帧发送 |
| [TaskMotor.c](../APP/Tasks/TaskMotor.c) | 底盘调试帧 0x101/0x103 发送 |
| [stm32f4xx_it.c](../Core/Src/stm32f4xx_it.c) | CAN TX/RX ISR 回调 |
| [can.c](../Core/Src/can.c) | CAN1 硬件初始化 (1Mbps, 过滤器) |

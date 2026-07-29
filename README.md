# 线控底盘控制系统 (Steering-by-Wire Chassis)

基于 STM32F407 的差速底盘线控系统（SbW 2.0），支持双电机协同控制、CAN 总线通信、双实例（自动驾驶/人工驾驶）模式仲裁和 FreeRTOS 实时调度。

## 硬件平台

| 项目 | 规格 |
|------|------|
| MCU | STM32F407IGT6 (Cortex-M4F, 168MHz) |
| 电机 | 双 BLDC 无刷直流电机，霍尔传感器 6 步梯形换向 |
| 编码器 | 1024 PPR 增量式编码器 |
| 通信 | CAN 1Mbps + USART1 DMA 调试输出 |
| 执行器 | EPB 电子驻车 + 2 线档位执行器 |
| RTOS | FreeRTOS V10.3.1 (CMSIS-RTOS V2, 1000Hz tick, heap_4) |

## 主要特性

- **线控底盘 (SbW 2.0)** — 转向/驱动/制动/档位/驻车五大线控功能模块，支持 CAN 指令控制
- **双实例架构** — `g_chassis_auto`（自动驾驶）和 `g_chassis_manual`（人工驾驶）并行运行，模式自动仲裁
- **五层运动控制仲裁** (MotionControl) — 制动 > 紧急停车 > 安全门 > 限幅 > 正常输出
- **人工接管检测** — 制动/油门踏板阈值触发 → `manual` 切换，停车后自动回 `auto`
- **双电机差速控制** — 霍尔传感器 6 步换向 + 速度 PID 闭环 + 逆/正运动学解算
- **CAN 总线通信** — 1Mbps，18 条 CAN 帧覆盖指令/反馈/诊断/调试
- **安全保护** — 命令超时 500ms 自动停车、加速斜率限幅、PID 抗饱和、ADC 过流检测
- **实时调度** — 100Hz 控制循环、多任务架构、队列+信号量同步

## 项目结构

```
├── Core/                          # STM32CubeMX 生成的 HAL 外设驱动
│   ├── Inc/                       # main.h, FreeRTOSConfig.h, can.h 等
│   └── Src/                       # main.c, freertos.c, stm32f4xx_it.c 等
├── APP/
│   ├── Function/                  # 线控功能模块
│   │   ├── SbwTypes.h             # 五大功能模块数据结构 + 驾驶模式枚举
│   │   ├── SteeringFunction.c/h   # 线控转向
│   │   ├── DriveFunction.c/h      # 线控驱动
│   │   ├── BrakingFunction.c/h    # 线控制动
│   │   ├── GearFunction.c/h       # 线控档位
│   │   ├── ParkingFunction.c/h    # 线控驻车 (EPB)
│   │   ├── MotionControl.c/h      # 五层运动控制仲裁
│   │   └── DrivingModeFunction.c/h # 驾驶模式仲裁
│   ├── Chassis/                   # 底盘聚合层
│   │   └── Chassis.c/h            # 运动学解算 + 双电机协同 + 安全停机
│   └── Tasks/                     # FreeRTOS 应用任务
│       ├── TaskCanInfo.c/h         # CAN 收发 + 应用层协议
│       ├── TaskSbwControl.c/h      # SbW 主控制任务 (100Hz)
│       ├── TaskMotor.c/h           # 电机初始化 + ADC 采样
│       └── TaskSeriel.c/h          # 串口调试输出
├── Device/                        # 硬件抽象层
│   ├── Inc/
│   │   ├── Motor.h                # 电机驱动接口 (6步换相/PID/ADC)
│   │   ├── Chassis.h              # 底盘聚合接口
│   │   ├── Kinematics.h           # 差速运动学
│   │   ├── PID.h                  # 位置式 PID 控制器
│   │   └── Filter.h               # 数字滤波器
│   └── Src/                       # 对应实现
├── doc/                           # 项目文档
│   └── CAN_Protocol.md            # CAN 应用层协议完整规范
├── Drivers/                       # STM32 HAL 库 + CMSIS（禁止修改）
├── Middlewares/                    # FreeRTOS V10.3.1（禁止修改）
├── cmake/                         # CMake 工具链 + CubeMX 子构建
├── CMakeLists.txt                 # 顶层构建
├── motor.ioc                      # STM32CubeMX 项目配置
└── STM32F407XX_FLASH.ld           # 链接脚本 (1024KB Flash, 128KB RAM)
```

## 构建

```bash
cmake --preset Debug          # 配置（需 arm-none-eabi-gcc 在 PATH 中）
cmake --build --preset Debug  # 编译

# 产物: build/Debug/motor.{elf,bin,hex}
```

- **工具链**: `arm-none-eabi-gcc`，Cortex-M4 硬浮点 (`-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard`)
- **构建系统**: CMake 3.22+ + Ninja，C11 标准

## SbW 架构

### 双实例设计

```
┌─────────────────────────────────────────┐
│            CAN Bus (1 Mbps)             │
├────────────────────┬────────────────────┤
│   自动驾驶指令帧    │   物理驾驶输入帧     │
│   0x200-0x203, 0x220│   0x210-0x213       │
├────────────────────┼────────────────────┤
│  g_chassis_auto     │  g_chassis_manual   │
│  (自动驾驶实例)      │  (人工驾驶实例)      │
├────────────────────┴────────────────────┤
│         DrivingModeFunction              │
│  接管信号 + 档位匹配 + 超速 → 仲裁模式    │
├──────────┬──────────┬────────────────────┤
│ automatic│  manual  │    interrupted     │
│  auto实例 │ manual实例│   紧急制动          │
├──────────┴──────────┴────────────────────┤
│           MotionControl (五层仲裁)        │
│  制动 > 紧急停车 > 安全门 > 限幅 > 正常    │
├─────────────────────────────────────────┤
│           Chassis (运动学 + 电机)         │
│  Kinematics → Motor_SpeedControl → PID   │
└─────────────────────────────────────────┘
```

### 驾驶模式

| 模式 | 值 | 触发条件 | 行为 |
|------|-----|----------|------|
| `automatic` | 0 | 无接管 + 档位匹配 + SbW 使能 | auto 实例控制档位/驻车/运动 |
| `manual` | 1 | 制动/油门踏板超过接管阈值 | manual 实例接管，P 档可切回 auto |
| `interrupted` | 2 | auto 模式下档位不匹配 | 仅紧急制动，P 档恢复后切回 auto |

### 100Hz 控制循环

`vTaskSbwControl` → 每 10ms 执行：

1. EPB + 档位硬件状态机推进
2. manual 实例接管检测（制动踏板 / 加速踏板）
3. DrivingModeFunction 仲裁全局模式
4. 模式分支执行档位/驻车指令
5. 档位反馈同步到双实例
6. P 档 EPB 强制联动
7. 选择活跃实例 → MotionControl 五层仲裁 → 输出 v, ω
8. 写入 `g_chassis` 底盘命令
9. 实际车速/EPB 状态反馈同步
10. 分频发送 6 条 SbW 状态反馈 CAN 帧

## CAN 通信协议

> 完整协议规范参见 [doc/CAN_Protocol.md](doc/CAN_Protocol.md)。以下为概要。

### 物理层

| 参数 | 值 |
|------|-----|
| 外设 | CAN1 (hcan1) |
| 引脚 | RX: PI9, TX: PB9 |
| 波特率 | 1 Mbps |
| 帧格式 | 标准帧 (11-bit ID) |
| 滤波器 | 接收全部标准帧 |
| TX 队列 | `canTxQueue` (指针传递, 深度 10) |
| RX 队列 | `canRxQueue` (值传递, 深度 10) |

### ID 分配

| 范围 | 方向 | 用途 | 帧数 |
|------|------|------|------|
| `0x100-0x103` | RX+TX | 底盘调试帧（电机直控/状态/诊断） | 3 |
| `0x110-0x115` | TX | SbW 状态反馈（50/20/10/5Hz 分频） | 6 |
| `0x200-0x203` | RX | 自动驾驶指令（转向/驱动/档位/制动） | 4 |
| `0x210-0x213` | RX | 物理驾驶输入（档位/方向盘/油门/制动） | 4 |
| `0x220` | RX | SbW 全线控使能控制 | 1 |

### 底盘调试帧

| ID | 方向 | 频率 | 内容 |
|----|------|------|------|
| `0x100` | RX | 按需 | 线速度 (int16 mm/s) + 角速度 (int16 mrad/s) + 使能位 |
| `0x101` | TX | ~100Hz | 左右 RPM + 运行状态 + 心跳 |
| `0x103` | TX | ~10Hz | 故障码 + PWM + PID 误差 |

> 0x100 仅在 SbW 未使能时生效。SbW 使能后速度由 MotionControl 管线接管。

### SbW 状态反馈帧（底盘 → 上位机）

| ID | 频率 | 内容 |
|----|------|------|
| `0x110` | 50Hz | 驾驶模式 + 系统标志 + 实际 v/ω + 故障汇总 + 心跳 |
| `0x111` | 50Hz | 转向角度/扭矩/转速 + 转向模式 + 故障码 |
| `0x112` | 20Hz | 车速 + 油门 + 加速度 + 驱动模式 + 接管标志 + 故障码 |
| `0x113` | 20Hz | 制动踏板 + 减速度 + 制动模式 + 接管/制动灯 + 故障码 |
| `0x114` | 10Hz | 档位位置 + EPB 状态 + 档位/驻车故障码 |
| `0x115` | 5Hz | MC 输出 v/ω + MC 内部标志 + 仲裁层级 |

### 自动驾驶指令帧（上位机 → 底盘）

| ID | 内容 |
|----|------|
| `0x200` | 转向：Enable + 目标角度 (0.5°/LSB) + 目标转速 (1°/s) |
| `0x201` | 驱动：Enable + 油门 (1%/LSB) + 加速度 (0.1 m/s²) + 扭矩 (1 Nm) |
| `0x202` | 档位+驻车：Gear/Parking Enable + 目标档位 (P/R/N/D) + 驻车请求 |
| `0x203` | 制动：Enable + 减速度 (0.1 m/s²) + 制动灯控制 |

### 物理驾驶输入帧（物理设备 → 底盘）

| ID | 内容 |
|----|------|
| `0x210` | 档位选择器位置 (P/R/N/D) |
| `0x211` | 方向盘转角 (0.5°/LSB) + 扭矩 (0.1 Nm) |
| `0x212` | 油门踏板位置 (1%/LSB) |
| `0x213` | 制动踏板位置 (0.1%/LSB) |

### 全线控使能

| ID | 内容 |
|----|------|
| `0x220` | Byte[0]: 0=关闭全线控 / 非0=开启，同时控制 auto 和 manual 双实例 |

## FreeRTOS 任务

| 任务 | 栈 | 频率 | 职责 |
|------|-----|------|------|
| `vTaskSbwControl` | 256 | 100Hz | SbW 主循环：模式仲裁 + MC + CAN 反馈发送 |
| `vTask_SpeedControl` | 256 | 100Hz | 底盘运动学 + 电机 PID + 调试 CAN 发送 |
| `vTaskCanInfo` | 128 | 事件驱动 | CAN 收发队列处理 + 消息分发 |
| `vTask_ADC_Sample` | 512 | DMA 触发 | 三相电流计算 + 过流检测 |
| `vTaskMotor` | 512 | 初始化 | 电机/底盘/PID 初始化后保活 |
| `vTask_Data_Send` | 256 | 33Hz | 串口 RPM/PWM 调试上报 |
| `vTaskSeriel` | 128 | 33Hz | USART1 DMA 调试输出 |

## 控制参数

### 运动学 & 电机

| 参数 | 默认值 | 说明 |
|------|--------|------|
| 车轮半径 | 0.15 m | 运动学模型 |
| 轮距 | 0.60 m | 左右轮中心距 |
| 减速比 | 10:1 | 电机到车轮 |
| PWM 频率 | 16.8 kHz | 时基 168MHz / 10 / 1000 |
| PWM 分辨率 | 0-1000 | 占空比步进 0.1% |

### PID & 安全

| 参数 | 默认值 | 说明 |
|------|--------|------|
| 速度 PID Kp | 0.2 | 比例增益 |
| 速度 PID Ki | 0.02 | 积分增益 |
| 速度 PID Kd | 0.001 | 微分增益 |
| 最大加速度 | 500 RPM/s | 斜率限幅 |
| 命令超时 | 500 ms | CAN 离线保护 |
| 停机阈值 | 30 RPM | 低于此值切断驱动桥 |
| 过流阈值 | 10 A | 相电流保护 |
| 最小占空比 | 50/1000 | 防止电机堵转 |

### 人工接管

| 参数 | 默认值 | 说明 |
|------|--------|------|
| 制动接管阈值 | 50 (5%) | 踏板 0.1%/LSB |
| 加速接管阈值 | 50 (50%) | 踏板 1%/LSB |

## 许可证

**GNU Affero General Public License v3.0** (AGPL-3.0)

本项目为自由软件：您可以遵照自由软件基金会发布的 GNU Affero 通用公共许可证（版本 3 或更新版本）的条款对其进行修改和重新发布。分发本项目的目的是希望它有用，但**不作任何保证**，甚至不隐含适销性或特定用途适用性的保证。详见 [LICENSE](LICENSE) 文件。

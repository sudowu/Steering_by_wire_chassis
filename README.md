# 线控底盘控制系统 (Steering-by-Wire Chassis)

基于 STM32F407 的差速底盘线控系统，支持双 BLDC 电机协同控制、CAN 总线通信和 FreeRTOS 实时调度。

## 硬件平台

| 项目 | 规格 |
|------|------|
| MCU | STM32F407IGT6 (Cortex-M4F, 168MHz) |
| 电机 | 双 BLDC 无刷直流电机，霍尔传感器换向 |
| 编码器 | 1024 PPR 增量式编码器 |
| 通信 | CAN 1Mbps + USART 调试 |
| RTOS | FreeRTOS V10.3.1 (CMSIS-RTOS V2) |

## 主要特性

- **双电机 FOC 替代方案** — 霍尔传感器 6 步换向 + 速度 PID 闭环控制
- **差速运动学** — 逆运动学（车体速度 → 电机转速）和正运动学（实际转速 → 车体速度）
- **CAN 总线通信** — 1Mbps 波特率，3 条 CAN 消息（指令/状态/转速）
- **安全保护** — 命令超时自动停车、加速斜率限幅、PID 抗饱和、ADC 电流检测
- **实时调度** — 100Hz 控制循环、FreeRTOS 多任务架构
- **信号滤波** — 中值滤波 + 低通滤波，抑制编码器噪声

## 项目结构

```
├── Core/                       # STM32CubeMX 生成的 HAL 外设驱动
│   ├── Inc/                    # main.h, FreeRTOSConfig.h, can.h 等
│   └── Src/                    # main.c, freertos.c, stm32f4xx_it.c 等
├── APP/Tasks/                  # FreeRTOS 应用任务
│   ├── TaskCanInfo.c/h         # CAN 收发任务
│   ├── TaskMotor.c/h           # 电机控制任务
│   └── TaskSeriel.c/h          # 串口调试任务
├── Device/                     # 硬件抽象层
│   ├── Inc/
│   │   ├── Motor.h             # 电机驱动接口
│   │   ├── Chassis.h           # 底盘聚合接口
│   │   ├── Kinematics.h        # 差速运动学
│   │   ├── PID.h               # PID 控制器
│   │   └── Filter.h            # 数字滤波器
│   └── Src/                    # 对应实现文件
├── Drivers/                    # STM32 HAL 库 + CMSIS
├── Middlewares/                 # FreeRTOS
├── cmake/                      # CMake 工具链 + CubeMX 子构建
├── CMakeLists.txt              # 顶层构建文件
├── motor.ioc                   # STM32CubeMX 项目配置
└── STM32F407XX_FLASH.ld        # 链接脚本
```

## 构建

### 环境要求

- **工具链**: `arm-none-eabi-gcc` (GCC ARM Embedded)
- **构建系统**: CMake 3.22+ + Ninja
- **IDE (可选)**: CLion 或 VSCode + STM32 插件

### 编译步骤

```bash
# 配置 Debug 构建
cmake --preset Debug

# 编译
cmake --build --preset Debug

# 或使用 Release 构建
cmake --preset Release
cmake --build --preset Release
```

输出产物位于 `build/` 目录：`motor.elf`、`motor.bin`、`motor.hex`。

## CAN 通信协议

CAN1, 波特率 1Mbps, 标准帧 ID。

| ID | 方向 | 名称 | 数据格式 |
|----|------|------|----------|
| `0x100` | RX | 底盘速度指令 | Byte[0-1]: 线速度 mm/s (int16)<br>Byte[2-3]: 角速度 mrad/s (int16)<br>Byte[4]: 控制标志 (bit0=使能) |
| `0x101` | TX | 底盘状态 | Byte[0-1]: 实际线速度<br>Byte[2-3]: 实际角速度<br>Byte[4]: 状态标志<br>Byte[5-6]: 左右电机电流 |
| `0x102` | TX | 电机转速 | Byte[0-1]: 左轮 RPM<br>Byte[2-3]: 右轮 RPM |

## FreeRTOS 任务

| 任务 | 频率 | 职责 |
|------|------|------|
| 速度控制 | 100Hz | 底盘运动学解算 + 电机 PID + CAN 状态发送 |
| 电机任务 | 初始化 | 电机/底盘/PID 初始化后自行删除 |
| CAN 信息 | 事件驱动 | CAN 消息收发队列处理 |
| ADC 采样 | DMA 完成触发 | 三相电流计算 |
| 串口任务 | 30Hz | RPM 数据调试输出 |

## 控制参数

| 参数 | 默认值 | 说明 |
|------|--------|------|
| 车轮半径 | 0.15 m | 运动学模型参数 |
| 轮距 | 0.60 m | 左右轮中心距 |
| 减速比 | 10:1 | 电机到车轮 |
| 速度 PID Kp | 0.2 | 比例增益 |
| 速度 PID Ki | 0.02 | 积分增益 |
| 速度 PID Kd | 0.001 | 微分增益 |
| 最大加速度 | 500 RPM/s | 斜率限幅 |
| 命令超时 | 500 ms | CAN 离线保护 |
| 停机阈值 | 30 RPM | 低于此值切断驱动 |

## 许可证

**GNU Affero General Public License v3.0** (AGPL-3.0)

本项目为自由软件：您可以遵照自由软件基金会发布的 GNU Affero 通用公共许可证（版本 3 或更新版本）的条款对其进行修改和重新发布。分发本项目的目的是希望它有用，但**不作任何保证**，甚至不隐含适销性或特定用途适用性的保证。详见 [LICENSE](LICENSE) 文件。

# CLAUDE.md

本文件为 Claude Code (claude.ai/code) 在此仓库中工作时提供指引。

## 构建

```bash
cmake --preset Debug       # 配置（需要 arm-none-eabi-gcc 在 PATH 中）
cmake --build --preset Debug   # 编译

# 产物位于 build/Debug/: motor.elf, motor.bin, motor.hex
```

- **工具链**: `arm-none-eabi-gcc`，Cortex-M4 硬浮点（`-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard`）
- **构建系统**: CMake 3.22+ + Ninja，C11 标准
- **IDE**: 同时支持 CLion（`.idea/`）和 VSCode（`.vscode/`）；`build/Debug/compile_commands.json` 已生成供 clangd 使用
- **链接脚本**: `STM32F407XX_FLASH.ld`（1024KB Flash, 128KB RAM + 64KB CCM）
- 无测试框架

## 架构

基于 STM32F407IG（Cortex-M4F，168MHz）的**差速线控底盘**。双 BLDC 电机采用霍尔传感器 6 步梯形换向 + 速度 PID 闭环控制。CAN 1Mbps 通信，FreeRTOS V10.3.1（CMSIS-RTOS V2 封装，1000Hz 滴答，heap_4，15KB 堆）。

### 分层结构

```
APP/Function/        车辆级线控功能（转向/驱动/制动/档位/驻车/MotionControl）
APP/Chassis/         底盘聚合层：运动学解算 + 双电机协同 + 安全停机
APP/Tasks/           FreeRTOS 任务：CAN 通信、电机控制、串口调试、SbW 控制
Device/              硬件抽象层：Motor、PID、Kinematics、Filter
Core/                STM32CubeMX 生成的 HAL 外设驱动
Drivers/             STM32F4 HAL 库 + CMSIS（禁止修改）
Middlewares/         FreeRTOS V10.3.1 内核（禁止修改）
```

### 硬件-电机映射（关键，容易搞混）

| 外设 | Motor1 (右侧) | Motor2 (左侧) |
|------|--------------|--------------|
| PWM 定时器 | TIM1 | TIM8 |
| 编码器定时器 | TIM3 | TIM2 |
| 霍尔传感器定时器 | TIM5 | TIM4 |
| 前进方向 | CCW | CW |
| 编码器极性 | **翻转** (`encoder_direction=1`) | 正常 (`encoder_direction=0`) |

底盘聚合时：`Chassis_Init(&g_chassis, &g_Motor2, &g_Motor1)` — **Motor2=左，Motor1=右**（与直觉相反）。

### 核心控制循环（100Hz）

`vTask_SpeedControl`（优先级 Normal+1）→ `Chassis_Control()`：

1. **检查 CAN 命令超时**（`CHASSIS_COMMAND_TIMEOUT_MS = 500ms`）
2. **使能且有效** → 逆运动学解算目标 RPM → 逐电机 `Motor_SpeedControl()`
3. **失能或超时** → 两阶段安全停机：先将目标 RPM 设为 0 减速，转速低于 `MOTOR_STOP_RPM_THRESHOLD`（30 RPM）后再切断驱动桥
4. **正运动学反馈**：实际 RPM → 实际线速度/角速度。CCW 方向的电机 RPM 取反后再代入正运动学公式
5. 每周期发送 CAN 状态帧 (0x101)，每 10 周期发送诊断帧 (0x103)

### 电机驱动 (`Device/Src/Motor.c`，698 行)

- **6 步换相**：霍尔传感器 GPIO 读取 → `hall_state_read()` → 查表 `motor_pwm_configs[6]` → `MotorControl()` 设置上桥 PWM 通道 + 下桥 GPIO 导通
- **PWM**：时基 168MHz / (9+1) / (999+1) = 16.8kHz，占空比范围 0-1000
- **转速测量**：1024 PPR 增量编码器，TIM2/TIM3 编码器模式，100Hz 采样，`motor_rpm_get()` 内部做 16 位溢出处理和 4 倍频修正
- **相电流检测**：ADC1 DMA 循环模式，3 通道（U/V/W），`ADC_SAMPLE_GROUP=4` 组过采样。换算公式：`(ADC值 - 偏置) × 3.3V / 4096 / 0.12Ω`（0.12Ω 采样电阻）
- **启动流程**（`vTaskMotor`）：`motor_init()` → `Chassis_Init()` → ADC DMA 启动 → 等 1s → `Motor_OffsetCalibrate()` 校准右电机零偏 → 创建 100Hz RPM 定时器 → 启动霍尔传感器 → 初始化速度 PID（Kp=0.2, Ki=0.02, Kd=0.001）→ 设置加速度限幅（500 RPM/s）→ 创建 `vTask_SpeedControl`

### PID 控制器 (`Device/Src/PID.c`，193 行)

**位置式 PID**（非增量式），输出 = P + I + D + 前馈。包含：积分分离/钳位（`intMax = outMax × 0.5`）、微分低通滤波（`dFilterCoef = 0.1`）、死区、前馈项、输出限幅（0–1000）。`PID_Reset()` 清零积分和全部误差历史。

`Motor_SpeedControl()` 的细节：
- 目标 < 0.5 RPM 且已停机 → 直接返回，不做任何操作
- 电机未运行时启动：以当前转速种子斜坡目标，`PID_Reset()` 清除残留积分，以占空比 70 起转
- 运行时最小占空比 50/1000（防止堵转）
- 停机时允许占空比降到 0，转速低于 30 RPM 后 `Motor_Stop()` 切断驱动桥

### CAN 通信协议

| 帧 | ID | 方向 | 频率 | 内容 |
|----|-----|------|------|------|
| 指令 | 0x100 | RX | 按需 | int16 线速度 mm/s + int16 角速度 mrad/s + 使能标志 bit0 |
| 状态 | 0x101 | TX | ~100Hz | int16 左右 RPM + 运行状态标志 + 心跳计数 |
| 诊断 | 0x103 | TX | ~10Hz | 故障码 + PWM 占空比 + PID 误差 |

消息传递：`canTxQueue`（指针传递，深度 10）、`canRxQueue`（值传递，深度 10）。TX 完成由 `canTxCompleteSemaphore` 同步。发送超时 50ms 后 abort 并消耗 abort 产生的额外信号量。队列满时 `pvPortFree` 释放内存，静默丢弃。

### FreeRTOS 任务

所有任务在 `Core/Src/freertos.c` 的 `StartDefaultTask()` 中通过 `xTaskCreate` 创建：

| 任务 | 栈 (words) | 优先级 | 触发方式 |
|------|-----------|--------|----------|
| vTask_ADC_Sample | 512 | Normal | `xSemaphoreADCReady` 信号量（ADC DMA 完成 ISR 给出） |
| vTaskCanInfo | 128 | Normal | 事件驱动（TX 队列阻塞 10ms + RX 队列非阻塞轮询） |
| vTaskSeriel | 128 | Normal | 33Hz 周期（USART1 DMA 调试输出） |
| vTaskMotor | 512 | Normal | 初始化完成后 2s 周期空循环保活 |
| vTask_SpeedControl | 256 | Normal+1 | 100Hz（`vTaskDelay(10ms)`） |
| vTask_Data_Send | 256 | Normal | 33Hz 周期（串口 RPM/PWM 上报） |

`vTaskRPM_Get` 是 100Hz 软件定时器回调（非任务），读取左右电机转速并翻转 LED0。

### 中断回调（`Core/Src/stm32f4xx_it.c`）

- `HAL_CAN_TxMailbox{0,1,2}CompleteCallback` → 三个邮箱共用一个 `canTxCompleteSemaphore`
- `HAL_CAN_RxFifo0MsgPendingCallback` → 读取消息 → `xQueueSendFromISR(canRxQueue)`
- `HAL_TIM_IC_CaptureCallback` → **TIM4→Motor2, TIM5→Motor1** 换相
- `HAL_ADC_ConvCpltCallback` → 置位 `ADC_DataReady` + 给出 `xSemaphoreADCReady`
- `HAL_UART_TxCpltCallback` → 给出 `xSemaphoreSeriel`

### 已实现 vs. 桩代码

**已实现**：电机驱动、PID、运动学、底盘控制循环、CAN 协议、ADC 电流检测、串口调试输出。

**桩代码**：`APP/Function/` 下所有功能模块（SteeringFunction、DriveFunction、BrakingFunction、GearFunction、ParkingFunction、MotionControl）和 `TaskSbwControl`。这些模块在 `SbwTypes.h` 中定义了完整的线控底盘数据结构（转向/驱动/制动/档位/驻车控制+反馈），`_Init()` 函数将字段初始化为零，但无实际控制逻辑。`MotionControl_Update()` 为空函数。预留用于未来上层自动驾驶平台对接（如 Apollo）。

## 代码修改规则

### STM32CubeMX 生成文件 (`Core/`)

`/* USER CODE BEGIN ... */` 和 `/* USER CODE END ... */` 之间的代码在 CubeMX 重新生成时保留，标记之外全部覆盖。在生成的 `.c`/`.h` 中加代码时**必须放在正确的 USER CODE 块内**。用户自行创建的文件（`APP/`、`Device/`）不受 CubeMX 影响。

禁止修改 `Drivers/` 和 `Middlewares/`。

### 添加新源文件

在根目录 `CMakeLists.txt` 的 `target_sources()`（约第 47 行）添加 `.c` 文件，`target_include_directories()`（约第 67 行）添加头文件路径。CubeMX 管理的源文件在 `cmake/stm32cubemx/CMakeLists.txt` 中添加。

### 引脚与外设配置

所有硬件配置定义在 `motor.ioc`。修改引脚分配、时钟树或外设设置时，在 CubeMX 中打开 `motor.ioc` 修改后重新生成——不要手动编辑 `Core/` 下的生成文件。

### FreeRTOS 注意事项

`FreeRTOSConfig.h` 中 `configENABLE_FPU = 0`（FreeRTOS 不管理 FPU 上下文），但 `SystemInit()` 启用了 FPU。这意味着**中断服务例程（ISR）中不能使用浮点运算**，否则会破坏任务上下文。所有浮点运算必须在任务中完成。

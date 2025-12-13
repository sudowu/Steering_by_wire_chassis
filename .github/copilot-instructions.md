<!-- Copilot / AI agent guidance for this repository -->
# 说明（给 AI 代理）

目的：快速让 AI 代理理解本仓库的架构、构建/烧写流程、代码约定与硬件映射，便于安全、可预测地修改固件代码。

要点摘要
- 架构：基于 STM32 HAL（CubeMX 生成）的固件。项目分为 `Core/`（自动生成的 HAL 配置和启动代码）、`Drivers/`（CMSIS 与 HAL driver）、`User/`（应用逻辑：`User/Inc` 与 `User/Src`）、顶层 CMake 构建。
- 构建：使用 CMake + Ninja 与交叉工具链 `arm-none-eabi-`（参见 `CMakePresets.json` 与 `cmake/gcc-arm-none-eabi.cmake`）。
- 硬件接口：电机驱动基于定时器 PWM（`htim1`, `htim8`），霍尔传感器通过 GPIO/EXTI。许多硬件符号在 `*.ioc` 或 `Core/Inc/main.h` 与 `User/Inc/*.h` 中定义。

关键文件（快速索引）
- 构建与工具链：`CMakePresets.json`, `cmake/gcc-arm-none-eabi.cmake`
- 启动与 HAL：`Core/Inc/*`, `Core/Src/*`（由 CubeMX 生成，保留 USER CODE 区域）
- 应用逻辑：`User/Inc/bldc.h`, `User/Src/bldc.c`, `User/Src/key.c`, `User/Src/led.c`
- 链接脚本：`STM32F407XX_FLASH.ld`

构建/运行/烧写（可复制命令）
- 生成与编译（要求 `arm-none-eabi-*` 在 PATH）：
  - `cmake --preset Debug`
  - `cmake --build --preset Debug`
- 生成 Release：替换 `Debug` 为 `Release` 或其他 preset。
- 直接进入构建目录并使用 ninja：`cd build/Debug && ninja`
- 烧录 / 调试：项目不强制绑定特定工具。常见方式：
  - 使用 ST-Link + OpenOCD 与 GDB：`openocd -f interface/stlink.cfg -f target/stm32f4x.cfg` 然后 `arm-none-eabi-gdb build/Debug/BLDC.elf`。
  - 使用 `st-flash` 或 IDE（STM32CubeIDE / Keil / IAR）按需烧写 `.elf`/`.bin`。

重要约定与编码模式（不要违反）
- CubeMX 自动生成的代码：修改时仅在 `/* USER CODE BEGIN */` / `/* USER CODE END */` 区域内更改，以避免下次由 CubeMX 导出时丢失改动。
- 硬件符号与宏：GPIO 端口、引脚与外设句柄以 `M1_LOW_SIDE_U_GPIO_Port`、`HALL1_TIM_CH1_Pin`、`htim1` 等命名；直接在 `User/Inc/*.h` 中使用这些符号。
- PWM 与换相：六步换相实现位于 `User/Src/bldc.c`，通过写 `htimX.Instance->CCRn` 与 `HAL_GPIO_WritePin(...)` 控制上下桥。示例：`m1_uhvl()` 使用 `htim1.Instance->CCR1 = g_bldc_motor1.pwm_duty`。
- 霍尔传感器：通过 `HAL_GPIO_ReadPin(...)` 读取（见 `hallsensor_get_state()`），方向检测使用预定义查表 `hall_cw_table` / `hall_ccw_table`。
- 启停控制：使用宏 `SHUTDOWN1_EN` / `SHUTDOWN1_OFF` 控制半桥使能信号（定义在 `User/Inc/bldc.h`）。

安全与测试注意
- 任何影响 PWM、使能或 GPIO 的改动都可能导致硬件损坏；建议在仿真或限流电源上验证。
- 初次运行：保持低占空比并确认相序与编码器方向。

AI 编辑约束（针对 Copilot / 自动化）
- 不要更改 CubeMX 生成的外设初始化（`Core/Src/*`）除非必要；若改动，请把更改限制在 `USER CODE` 区域并记录原因。
- 对于与硬件绑定的常量（如 PWM 周期、定时器分频、ADC 通道），优先搜索 `tim.c`、`main.c` 与 `stm32f4xx_hal_conf.h`，并在 PR 描述中注明验证步骤。
- 新增公共接口时在对应头文件 (`User/Inc/*.h`) 添加注释，保持其它模块可以通过 `#include "main.h"` 拿到硬件句柄与 HAL 定义。

示例任务线索（快速上手）
- 添加一个新电机控制命令：修改 `User/Inc/bldc.h` 增加接口声明，并在 `User/Src/bldc.c` 添加实现，使用已有 `bldc_ctrl()` 模式更新 `g_bldc_motorX` 状态，再在安全检查后调用 `start_motorX()`。
- 调整 PWM 周期：修改 `tim.c` 中对应定时器的 `ARR` 值（注意同步 `MAX_PWM_DUTY` 定义 `User/Inc/bldc.h`）。

未发现的内容 / 建议补充给维护者
- 提供一个常用的烧录脚本（OpenOCD / st-flash 示例）并写入 `README.md` 或 `scripts/`。
- 若存在硬件设计文件（BOM / 原理图），建议把路径放入 README 以便代码与硬件对应。

请审阅这份机器代理说明：告诉我哪些区域需要更详细的示例或添加特定的本地脚本。

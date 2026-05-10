---
title: FMC 外设拓展 SDRAM 详解
category: STM32/FMC
tags:
  - STM32
  - FMC
  - SDRAM
  - 嵌入式
---
## 01. 本节课核心概要

本节课系统讲解了如何使用 STM32 高级系列（F429/F7/H7）的 **FMC（Flexible Memory Controller）外设** 扩展外部 **SDRAM（Synchronous Dynamic Random-Access Memory）** 作为系统 RAM 空间。课程首先对比了早期 FSMC（仅支持静态存储器如 SRAM）与 FMC（支持动态存储器如 SDRAM）的功能差异，随后深入剖析了 SDRAM 芯片（以 W9825G6KH 为例）的内部结构、信号线功能、工作原理及关键操作（如刷新、预充电、突发传输），并详细阐述了 SDRAM 的初始化流程和 STM32 FMC 控制器的寄存器配置与地址映射机制。最后简要介绍了 SDRAM 在 STM32 地址空间中的属性限制及代码执行可能性。

下面将按照 **核心考点与重难点 → 课堂疑难解答（无）→ 详细课堂内容逻辑梳理 → 课后作业与拓展** 的结构展开，全面还原课程知识体系。

## 02. 核心考点与重难点

### 重点

- **FMC vs FSMC**：FSMC（F1/F407 系列）仅支持静态存储器（SRAM/NOR Flash）；FMC（F429/F7/H7 系列）新增 SDRAM 控制器，支持动态存储器。
- **SDRAM 核心特性**：
  - **动态存储**：依赖电容存储电荷（1/0 由电压高低表示），需周期性刷新防止数据丢失（典型刷新周期 64ms）。
  - **同步接口**：所有操作在时钟（CLK）上升沿同步进行。
  - **行列寻址 + Bank 结构**：通过 RAS（行选通）、CAS（列选通）复用地址线，结合 BA（Bank 地址）选择存储阵列，大幅减少引脚数量。
- **关键信号线功能**：
  - `CKE`（Clock Enable）：时钟使能，低电平时触发自刷新（Self Refresh）。
  - `DQM`（Data Mask）：数据掩码，控制高低字节有效（如 16 位总线中选 8 位）。
  - `CS#`（Chip Select）：片选（低有效）。
- **SDRAM 初始化流程**（6 步）：
  1. 上电并提供时钟；
  2. 保持 CKE=1 至少 200μs；
  3. 对所有 Bank 预充电（Precharge All）；
  4. 执行 8 次自动刷新（Auto Refresh）；
  5. 加载模式寄存器（Mode Register Set）；
  6. 激活（Active）任一行（可选，标志初始化完成）。
- **FMC 地址映射**：
  - SDRAM 映射至 External Device 区域（默认不支持 XIP，即片上执行代码）；
  - 可通过 `SYSCFG_MEMRMP` 寄存器重映射至 External RAM 区域以支持 XIP（但受时钟频率限制）。

### 难点

- **刷新机制区分**：
  - **自动刷新（Auto Refresh）**：由 STM32 主动发起，在正常工作时周期性执行（需停止所有 Bank 操作）。
  - **自刷新（Self Refresh）**：进入低功耗模式时，SDRAM 内部自主刷新（无需外部时钟，仅需 CKE=0）。
- **模式寄存器配置参数**：
  - **突发长度（Burst Length, BL）**：连续传输的数据单元数（如 BL=8 表示一次读 8 个连续地址）。
  - **CAS 延迟（CAS Latency, CL）**：从发出读命令到数据有效所需的时钟周期数（通常 2 或 3）。
  - **突发类型（Burst Type, BT）**：顺序（Sequential）或间隔（Interleaved）访问模式。
- **FMC 控制器寄存器组**：
  - `SDCRx`（SDRAM Control Register）：配置位宽、CL、行列地址位数等。
  - `SDTRx`（SDRAM Timing Register）：设置时序参数（如 tRCD、tRP、tRC）。
  - `SDCMR`（SDRAM Command Mode Register）：发送初始化命令（预充电、刷新、模式寄存器加载）。

> [!note] AI 注释
> **CKE（Clock Enable）**：SDRAM 的时钟使能信号。当 CKE 为高电平时，外部时钟有效，SDRAM 正常工作；当 CKE 拉低时，SDRAM 进入自刷新模式，依靠内部振荡器维持刷新操作，适用于系统休眠场景以降低功耗。

> [!tip] AI 潜台词挖掘
> SDRAM 的"动态"特性（电容自放电）是其成本优势（同等容量价格低于 SRAM）与性能劣势（需刷新开销、时序复杂）的根本原因。这一物理限制直接决定了：任何基于 SDRAM 的系统设计都必须将刷新机制纳入实时性考量，否则可能导致数据损坏或系统崩溃。

## 03. 课堂疑难解答 (Q&A)

本节课为单向授课，无学生提问互动环节。

## 04. 详细课堂内容逻辑梳理

### 1. SDRAM 器件基础与内部结构

#### 1.1 知识锚点与定义

- **SDRAM（Synchronous DRAM）**：同步动态随机存取存储器，其操作与时钟信号同步，通过电容存储数据，需定期刷新。
- **Bank**：SDRAM 内部的独立存储阵列单元，现代 SDRAM 通常包含 4 个 Bank，通过 `BA[1:0]` 信号线选择。

#### 1.2 原理背景与深度精讲

- **背景**：相比 SRAM，SDRAM 单位容量成本更低、体积更小，适用于大内存需求场景（如图形处理、摄像头缓冲），但需复杂控制逻辑。
- **内部功能框图解析**（以 W9825G6KH 为例）：
  - **信号接口层**：包括 CLK、CKE、CS#、RAS#、CAS#、WE#、DQM、BA[1:0]、A[12:0]、DQ[15:0]。
  - **控制逻辑层**：命令解码器，将外部信号组合译码为内部操作命令（如 Active、Read、Write、Precharge）。
  - **存储阵列层**：由多个 Bank 组成，每个 Bank 为行列矩阵结构。
  - **数据 I/O 层**：通过 DQ 总线同步传输数据，DQM 控制字节有效性。

#### 1.3 经典例题/案例演示

- **芯片型号**：W9825G6KH（华邦电子）
  - 容量：32 MB（256 Mbit）
  - 数据位宽：16 bit
  - Bank 数量：4
  - 引脚封装：54-pin TSOP-II
- **信号线功能表**（关键信号）：

| 信号 | 功能 | 说明 |
|---|---|---|
| `CLK` | 同步时钟 | 所有输入在上升沿采样 |
| `CKE` | 时钟使能 | 低电平触发自刷新 |
| `CS#` | 片选 | 低有效 |
| `RAS#` | 行地址选通 | 低电平时 A[12:0] 锁存为行地址 |
| `CAS#` | 列地址选通 | 低电平时 A[12:0] 锁存为列地址 |
| `WE#` | 写使能 | 低电平为写操作 |
| `DQM` | 数据掩码 | 控制 DQ[15:8]/DQ[7:0] 有效 |
| `BA[1:0]` | Bank 地址 | 选择 4 个 Bank 之一 |

#### 1.4 避坑指南与总结

- **行列地址复用**：A[12:0] 总线在 RAS# 和 CAS# 控制下分别传输行/列地址，不可同时有效。
- **Bank 选择必要性**：大容量 SDRAM 必须分 Bank 设计，否则行列地址线过多导致芯片面积和成本剧增。
- **DQM 使用场景**：仅在需要非对齐访问（如 8 位数据写入 16 位总线）时启用，否则可置无效。

### 2. SDRAM 关键操作与命令集

#### 2.1 知识锚点与定义

- **预充电（Precharge）**：关闭当前激活的行，为新行访问做准备。
- **自动刷新（Auto Refresh）**：周期性对所有行刷新，维持数据完整性。
- **突发传输（Burst Transfer）**：基于起始地址连续读写多个数据单元。

#### 2.2 原理背景与深度精讲

- **刷新必要性**：电容自放电导致电压衰减（如 3V → 1.2V），若低于阈值（如 1.5V）则数据翻转（1→0）。
- **命令生成机制**：通过 CS#、RAS#、CAS#、WE# 的高低电平组合生成命令（查表译码）。
- **突发模式意义**：减少地址总线切换次数，提升连续数据传输效率。

#### 2.3 经典例题/案例演示

- **预充电命令**：
  - A10=1：预充电所有 Bank；
  - A10=0：仅预充电 BA 指定的 Bank。
- **读写时序关键参数**：

| 参数 | 含义 |
|---|---|
| tRCD（RAS to CAS Delay） | 行激活到列访问的最小延迟 |
| tRP（Row Precharge Time） | 预充电到下一次行激活的最小延迟 |
| tRC（Row Cycle Time） | 同一行两次激活的最小间隔 |
| CL（CAS Latency） | 列访问到数据输出的延迟周期数 |

#### 2.4 避坑指南与总结

- **刷新期间禁止操作**：自动刷新时所有 Bank 停止工作，持续 N 个时钟周期（由芯片手册指定）。
- **初始化顺序不可颠倒**：必须先预充电 → 刷新 → 模式寄存器加载，否则 SDRAM 无法正常工作。
- **CL 设置需匹配时钟频率**：高速时钟下需增大 CL 值以满足建立/保持时间。

### 3. SDRAM 初始化流程

#### 3.1 知识锚点与定义

- **初始化**：SDRAM 上电后必须执行特定命令序列，使其进入稳定工作状态。

#### 3.2 原理背景与深度精讲

- **与 SRAM 的本质区别**：SRAM 上电即用；SDRAM 需初始化建立内部状态（如刷新计数器、模式寄存器）。

#### 3.3 经典例题/案例演示

**W9825G6KH 初始化步骤**：

1. 上电，提供 CLK；
2. 保持 CKE=1 ≥ 200μs（等待内部稳压）；
3. 发送 Precharge All 命令；
4. 执行 8 次 Auto Refresh；
5. 发送 Load Mode Register 命令（配置 BL/CL/BT 等）；
6. （可选）发送 Active 命令激活某行。

#### 3.4 避坑指南与总结

- **延迟时间需满足最小值**：200μs 是 W9825G6KH 要求，不同芯片可能不同（查手册！）。
- **刷新次数固定为 8 次**：这是 JEDEC 标准要求，不可省略。

### 4. STM32 FMC 控制器架构

#### 4.1 知识锚点与定义

- **FMC（Flexible Memory Controller）**：STM32 高级系列的存储控制器，支持 SDRAM/NOR/SRAM/NAND。

#### 4.2 原理背景与深度精讲

- **多控制器集成**：FMC 内部包含独立的 SDRAM 控制器、NOR/SRAM 控制器、NAND 控制器。
- **地址映射机制**：通过访问特定地址范围（如 `0xC0000000–0xDFFFFFFF`）触发 FMC 自动产生 SDRAM 时序。

#### 4.3 经典例题/案例演示

**FMC SDRAM 寄存器组**：

| 寄存器 | 功能 |
|---|---|
| `FMC_SDCRx`（x=1/2） | 配置 Bank1/Bank2 的位宽、CL、行列地址位数 |
| `FMC_SDTRx`（x=1/2） | 配置 tRCD/tRP/tRC 等时序 |
| `FMC_SDCMR` | 发送初始化命令（如 MODES=0b010 表示 Load Mode Register） |
| `FMC_SDRTR` | 配置自动刷新周期（如 COUNT=0x3FF 表示 4096 行刷新间隔） |

**地址空间分配**：

| Bank | 地址范围 | 大小 |
|---|---|---|
| Bank1 | `0xC0000000 – 0xCFFFFFFF` | 256 MB |
| Bank2 | `0xD0000000 – 0xDFFFFFFF` | 256 MB |

#### 4.4 避坑指南与总结

- **SDRAM 不支持默认 XIP**：因其位于 External Device 区域（无指令缓存），需重映射才能执行代码。
- **双 SDRAM 支持**：FMC 提供两组独立信号（如 SDCKE0/1, SDNE0/1），可连接两片 SDRAM。

## 05. 课后作业与拓展

### 课后任务

- [ ] 使用 STM32CubeMX 配置 FMC SDRAM 控制器（针对野火 F429/H743 开发板）。
- [ ] 编写代码验证 SDRAM 读写功能（如填充数组并校验）。
- [ ] 尝试通过 `SYSCFG->MEMRMP` 寄存器重映射 SDRAM 地址以支持 XIP，并测试代码执行性能。

### 参考资料

- 《STM32F7 中文参考手册》第 37 章：FMC 控制器
- W9825G6KH 数据手册（华邦电子）
- JEDEC SDRAM 标准文档（JESD79F）

### 待查证盲区

- SDRAM 乒乓操作（Ping-Pong Buffer）在视频流处理中的应用。
- FMC 仲裁机制：当 CPU、DMA、GPU 同时访问 SDRAM 时的优先级策略



<iframe src="http://player.bilibili.com/player.html?isOutside=true&bvid=BV18X4y1M763&p=71&autoplay=0" scrolling="no" border="0" frameborder="no" framespacing="0" allowfullscreen="true" width="100%" height="500"></iframe>

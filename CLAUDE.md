# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Steer-by-Wire chassis control software — a Windows Forms (.NET Framework 4.8) desktop application for controlling and monitoring a steer-by-wire vehicle chassis via CAN bus, using ZLG (周立功) USBCAN2 hardware.

## Build & Run

- **Solution**: `control_software.sln` (Visual Studio 2022, v17.14)
- **Target framework**: .NET Framework 4.8
- **Output type**: Windows Application (`WinExe`)
- **Platforms**: AnyCPU, x64 (Debug and Release)
- **Unsafe code**: Required — `AllowUnsafeBlocks` is enabled in Debug configs

```bash
# Build from command line (requires MSBuild in PATH):
msbuild control_software.sln /p:Configuration=Debug /p:Platform=x64

# Or open in Visual Studio:
start control_software.sln
```

There are no automated tests or CI pipelines. The native `ControlCAN.dll` must be present in the output directory (it's set to `CopyToOutputDirectory=Always` in the project file).

## Architecture

### Source Files

| File | Purpose |
|------|---------|
| `Program.cs` | Entry point — launches `Form1` |
| `Form1.cs` | Main UI form. Contains **all** CAN communication logic: device open/close, CAN start/stop, send/receive via a timer tick, button event handlers for motion control (advance, retreat, turn left/right, stop), and UI updates from received telemetry |
| `Form1.Designer.cs` | Auto-generated WinForms designer code — layout, control declarations, `InitializeComponent()` |
| `Chassis.cs` | Domain types: `CHASSIS_STATUS` enum (IDLE, RETREAT, RUNNING, LEFT, RIGHT) and `CHASSIS_INFO` struct (Max_Power, status, braking, stop_flag) |
| `controlcan.cs` | P/Invoke wrapper around the native `ControlCAN.dll` (ZLG USBCAN2 SDK). Defines all CAN data types (`VCI_CAN_OBJ`, `VCI_INIT_CONFIG`, `VCI_BOARD_INFO`) and `DllImport` declarations for the ZLG API |

### CAN Communication Flow

1. **Connection**: `buttonConnect_Click` opens the USBCAN2 device via `VCI_OpenDevice`, then initializes the CAN channel via `VCI_InitCAN` with config (baud rate 500Kbps, Timing0=0x00, Timing1=0x14, filter and mode settings)
2. **Start**: `button_StartCAN_Click` calls `VCI_StartCAN` to enable the CAN channel
3. **Periodic I/O**: A WinForms `Timer` (`timer_rec`) drives the main loop:
   - **Receive**: `VCI_Receive` reads up to 1000 CAN frames with 100ms timeout; parses frames and updates UI (motor speeds, brake status, remote control channel values)
   - **Transmit**: If chassis is not idle and CAN is started, builds an 8-byte control frame (ID `0x08`) from current speed/angle values and sends via `VCI_Transmit`
4. **Motion control buttons** (advance/retreat/left/right/stop) set `cHASSIS_INFO.status` and `stop_flag`; the timer tick reads these flags to determine what to send

### CAN Protocol (all IDs are standard 11-bit)

| ID | Direction | Data Layout |
|----|-----------|-------------|
| `0x08` | PC → Chassis | `[0:1]` speed (uint16, little-endian), `[2:3]` angle (uint16, LE), `[4:7]` reserved (0) |
| `0x09` | Chassis → PC | `[0:1]` motor1 RPM, `[2:3]` motor2 RPM, `[4:5]` remote channel 2 (throttle), `[6:7]` remote channel 4 (steering) |
| `0x0A` | Chassis → PC | `[0]` brake1 state (1=engaged, 0=released), `[1]` brake2 state |

- Negative speed encodes reverse; negative angle encodes right turn — both achieved via unsigned integer wraparound (two's complement interpretation at the receiver)
- Speed/angle values are scaled: `(Max_Power / 100) * slider_percent`

### Key Design Notes

- The `controlcan` class and `VCI_CAN_OBJ`/`VCI_BOARD_INFO` structs use `fixed byte[]` arrays (unsafe code) — this is the ZLG SDK convention for their DllImport signatures
- `CHASSIS_INFO.stop_flag` is a one-shot latch: set to 1 by the stop button, cleared by the timer tick after processing (forces speed to 0 and resets status to IDLE)
- The `CHASSIS_INFO` struct uses `UInt16` for `Max_Power` (default `0x0600` = 1536), which is the full-scale value against which slider percentages are computed
- UI uses Chinese labels throughout

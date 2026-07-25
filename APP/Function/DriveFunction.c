/**
 * @file    DriveFunction.c
 * @brief   线控驱动功能实现
 *
 * 决策层：人工接管检测 + 指令校验
 * 执行层：MotionControl Layer 2 读取 Drive_Control 生成目标速度
 */

#include "DriveFunction.h"

/**
 * @brief 初始化驱动功能模块
 *
 * 将所有控制字段归零、反馈置为手动模式、人工接管和越界阈值清零。
 * 该函数仅做结构体初始化，不涉及硬件操作。
 *
 * @param df 驱动功能实例指针
 */
void DriveFunction_Init(Drive_Function* df)
{
    df->Drive_Control.Driving_Config_Enable             = 0;
    df->Drive_Control.Target_Accelerator_pedal_Position = 0;
    df->Drive_Control.Target_Longitudinal_Acceleration  = 0;
    df->Drive_Control.Target_Driving_Torque             = 0;

    df->Drive_Feedback.Driving_mode              = manual;
    df->Drive_Feedback.Accelerator_pedal_Position = 0;
    df->Drive_Feedback.Longitudinal_Acceleration  = 0;
    df->Drive_Feedback.Vehicle_Speed              = 0;
    df->Drive_Feedback.Fault_message              = 0;

    df->Manual_Takeover = 0;
    df->Over_Threshold  = 0;
}

/**
 * @brief 驱动功能周期更新（每 10ms 调用）
 *
 * 在 manual 实例上：检测加速踏板人工接管
 * 在 auto 实例上：指令校验（Over_Threshold 由 CAN 层设定）
 *
 * 人工接管逻辑：
 *   物理加速踏板位置 ≥ TAKEOVER_ACCEL_PEDAL_THRESHOLD → Manual_Takeover 置位
 *   低于阈值 → 清除
 *
 * 接管信号由 DrivingModeFunction 统一仲裁。
 */
void DriveFunction_Update(Drive_Function* df)
{
    Drive_Control*  ctrl = &df->Drive_Control;
    Drive_Feedback* fb   = &df->Drive_Feedback;

    /* Over_Threshold 由外部设定（如 CAN 层校验失败），此处透传 */
    if (df->Over_Threshold)
    {
        fb->Fault_message = 1;
        return;
    }

    fb->Fault_message = 0;

    /* 反馈同步：加速踏板位置 */
    fb->Accelerator_pedal_Position = ctrl->Target_Accelerator_pedal_Position;

    /* 人工接管检测：加速踏板位置超阈值
     *
     * manual 实例的踏板数据来自物理 CAN 帧 → 真实踏板接管
     * auto 实例的踏板数据恒为 0 → 永不触发接管
     */
    if (ctrl->Target_Accelerator_pedal_Position >= TAKEOVER_ACCEL_PEDAL_THRESHOLD)
    {
        df->Manual_Takeover = 1;
    }
    else
    {
        df->Manual_Takeover = 0;
    }
}

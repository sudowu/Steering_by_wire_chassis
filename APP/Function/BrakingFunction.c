/**
 * @file    BrakingFunction.c
 * @brief   线控制动功能实现
 *
 * 决策层：指令校验 + 制动灯控制 + 状态同步
 * 执行层：MotionControl Layer 3 直接电机减速（本模块无独立 Device 层）
 */

#include "BrakingFunction.h"

/* 制动灯自动点亮阈值 (0.1 m/s²) */
#define BRAKE_LIGHT_ON_THRESHOLD    5    // ≥0.5 m/s² 减速度 → 亮灯
#define BRAKE_LIGHT_OFF_THRESHOLD   2    // <0.2 m/s² 减速度 → 熄灯

/* 人工接管检测：制动踏板位置阈值 (0.1%) */
#define MANUAL_TAKEOVER_PEDAL_THRESHOLD  50  // ≥5% 视为人工踩踏板

/* 指令合法性上限（uint8_t 自身限幅 0..255，此处仅校验减速度）*/
#define MAX_VALID_DECELERATION      100  // 10.0 m/s² (合理上限)

/**
 * @brief 初始化制动功能模块
 *
 * 将所有控制字段归零、反馈置为手动模式、人工接管和越界阈值清零。
 * 该函数仅做结构体初始化，不涉及硬件操作。
 *
 * @param bf 制动功能实例指针
 */
void BrakingFunction_Init(Braking_Function* bf)
{
    bf->Braking_Control.Braking_Config_Enable         = 0;
    bf->Braking_Control.Target_Braking_Pedal_Position = 0;
    bf->Braking_Control.Target_Deceleration           = 0;
    bf->Braking_Control.Break_Light_Control           = 0;

    bf->Braking_Feedback.Current_Braking_System_Driving_Mode = manual;
    bf->Braking_Feedback.Braking_Pedal_Position              = 0;
    bf->Braking_Feedback.Brake_Light_Status                  = 0;
    bf->Braking_Feedback.Fault_message                       = 0;

    bf->Manual_Takeover = 0;
    bf->Over_Threshold  = 0;
}

/**
 * @brief 制动功能周期更新（每 10ms 调用）
 *
 * 本模块无独立制动器硬件。行车制动由电机减速实现：
 *   MotionControl Layer 3 读取 Braking_Control.Target_Deceleration
 *   执行减速度斜坡降速，电机即"制动执行器"。
 *
 * 本函数只做：
 *   - 指令合法性校验
 *   - 制动灯自动控制（减速度阈值触发）
 *   - 反馈状态同步
 *   - 制动踏板人工接管检测
 */
void BrakingFunction_Update(Braking_Function* bf)
{
    Braking_Control*  ctrl = &bf->Braking_Control;
    Braking_Feedback* fb   = &bf->Braking_Feedback;

    /* ================================================================
     * Step 1: 指令合法性校验
     * ================================================================ */

    if (ctrl->Target_Deceleration > MAX_VALID_DECELERATION)
    {
        fb->Fault_message = 1;
        fb->Brake_Light_Status = 0;
        return;
    }

    /* Over_Threshold 由外部设定（如传感器上报越界），此处透传 */
    if (bf->Over_Threshold)
    {
        fb->Fault_message = 1;
        fb->Brake_Light_Status = 0;
        return;
    }

    fb->Fault_message = 0;

    /* ================================================================
     * Step 2: 制动灯自动控制
     *
     *   优先级：上位机直接控制 > 减速度阈值自动触发
     * ================================================================ */

    if (ctrl->Break_Light_Control)
    {
        /* 上位机直接控制制动灯 */
        fb->Brake_Light_Status = 1;
    }
    else if (ctrl->Target_Deceleration >= BRAKE_LIGHT_ON_THRESHOLD)
    {
        /* 减速度 ≥ 0.5 m/s² → 自动亮灯 */
        fb->Brake_Light_Status = 1;
    }
    else if (ctrl->Target_Deceleration <= BRAKE_LIGHT_OFF_THRESHOLD)
    {
        /* 减速度 < 0.2 m/s² → 熄灯 */
        fb->Brake_Light_Status = 0;
    }
    /* 中间区间保持上一次状态，避免闪烁 */

    /* ================================================================
     * Step 3: 制动踏板位置反馈同步
     * ================================================================ */

    fb->Braking_Pedal_Position = ctrl->Target_Braking_Pedal_Position;

    /* ================================================================
     * Step 4: 驾驶模式反馈
     * ================================================================ */

    if (ctrl->Braking_Config_Enable)
    {
        fb->Current_Braking_System_Driving_Mode = automatic;

        /* 人工接管：制动踏板被踩下 → 触发接管 */
        if (ctrl->Target_Braking_Pedal_Position >= MANUAL_TAKEOVER_PEDAL_THRESHOLD)
        {
            bf->Manual_Takeover = 1;
        }
        else
        {
            bf->Manual_Takeover = 0;
        }
    }
    else
    {
        fb->Current_Braking_System_Driving_Mode = manual;
        bf->Manual_Takeover = 0;
    }
}

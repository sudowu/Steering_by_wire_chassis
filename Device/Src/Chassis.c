//
// Created by yang on 2026/6/23.
//

#include "Chassis.h"

#include <math.h>

#include "main.h"

Chassis_t g_chassis;

void Chassis_Init(Chassis_t* c, Motor_t* left, Motor_t* right)
{
    c->motor_left  = left;
    c->motor_right = right;
    Kinematics_Init(&c->kin);

    c->cmd_linear_vel  = 0.0f;
    c->cmd_angular_vel = 0.0f;
    c->cmd_enable       = 0;
    c->cmd_timestamp    = 0;

    c->actual_linear_vel  = 0.0f;
    c->actual_angular_vel = 0.0f;

    c->initialized = 1;
}

uint8_t Chassis_IsCommandValid(const Chassis_t* c)
{
    return (HAL_GetTick() - c->cmd_timestamp) < CHASSIS_COMMAND_TIMEOUT_MS;
}

void Chassis_Control(Chassis_t* c)
{
    if (!c->initialized)
    {
        return;
    }

    if (c->cmd_enable && Chassis_IsCommandValid(c))
    {
        float rpm_left, rpm_right;
        Kinematics_Inverse(&c->kin,
                           c->cmd_linear_vel,
                           c->cmd_angular_vel,
                           &rpm_left, &rpm_right);

        Motor_SpeedControl(c->motor_left,  rpm_left);
        Motor_SpeedControl(c->motor_right, rpm_right);
    }
    else
    {
        /* 两阶段停机：先减速到 0，转速低于阈值后再切断驱动桥 */
        uint8_t left_stopped  = fabsf(c->motor_left->rpm)  < MOTOR_STOP_RPM_THRESHOLD;
        uint8_t right_stopped = fabsf(c->motor_right->rpm) < MOTOR_STOP_RPM_THRESHOLD;

        if (left_stopped && right_stopped)
        {
            Motor_Stop(c->motor_left);
            Motor_Stop(c->motor_right);
        }
        else
        {
            Motor_SpeedControl(c->motor_left,  0.0f);
            Motor_SpeedControl(c->motor_right, 0.0f);
        }
    }

    /* 正运动学：实际 RPM → 实际车体速度（供状态上报使用）*/
    float fb_l = (c->motor_left->direction == CCW)
                 ? -c->motor_left->rpm : c->motor_left->rpm;
    float fb_r = (c->motor_right->direction == CCW)
                 ? -c->motor_right->rpm : c->motor_right->rpm;

    Kinematics_Velocity_t actual = Kinematics_Forward(&c->kin, fb_l, fb_r);
    c->actual_linear_vel  = actual.linear_velocity;
    c->actual_angular_vel = actual.angular_velocity;
}

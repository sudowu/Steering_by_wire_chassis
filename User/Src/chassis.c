#include "chassis.h"
#include "bldc.h"

chassis_obj s_chassis = {&g_bldc_motor1, &g_bldc_motor2, 0, 0, 0};


void chassis_control(chassis_obj* chassis)
{
    if (chassis->velocity_x >= 0) {
        chassis->bldc1->pwm_duty_target = chassis->velocity_x;
        chassis->bldc2->pwm_duty_target = chassis->velocity_x;
    }
}

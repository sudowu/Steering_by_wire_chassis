#include "chassis.h"
#include "bldc.h"

chassis_obj s_chassis = {&g_bldc_motor1, &g_bldc_motor2, 0, 0, 0};


void chassis_control(chassis_obj* chassis)
{
    if (chassis->velocity_x >= 0) {
        chassis->bldc1->pwm_duty_target = chassis->velocity_x;
        chassis->bldc2->pwm_duty_target = chassis->velocity_x;
    }

    
      if (chassis->velocity_x > MAX_PWM_DUTY/2) 
        chassis->velocity_x  = MAX_PWM_DUTY/2;
      if (chassis->velocity_x > 0) {
        g_bldc_motor1.pwm_duty_target = chassis->velocity_x;
        g_bldc_motor1.dir = CCW;
        g_bldc_motor2.pwm_duty_target = chassis->velocity_x;
        g_bldc_motor2.dir = CW;
      }
      else {
        g_bldc_motor1.pwm_duty_target = -chassis->velocity_x;
        g_bldc_motor1.dir = CW;
        g_bldc_motor2.pwm_duty_target = -chassis->velocity_x;
        g_bldc_motor2.dir = CCW;
      }
      g_bldc_motor1.run_flag = RUN;
      g_bldc_motor2.run_flag = RUN;
      start_motor1();
      start_motor2();
}

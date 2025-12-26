#include "chassis.h"
#include "bldc.h"
#include "main.h"
#include "can.h"
#include "stdio.h"
#include <stdint.h>

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
        g_bldc_motor1.dir_set = CCW;
        g_bldc_motor2.pwm_duty_target = chassis->velocity_x;
        g_bldc_motor2.dir_set = CW;
      }
      else {
        g_bldc_motor1.pwm_duty_target = -chassis->velocity_x;
        g_bldc_motor1.dir_set = CW;
        g_bldc_motor2.pwm_duty_target = -chassis->velocity_x;
        g_bldc_motor2.dir_set = CCW;
      }
      g_bldc_motor1.run_flag = RUN;
      g_bldc_motor2.run_flag = RUN;
      start_motor1();
      start_motor2();
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if (hcan->Instance == CAN1) {
    HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO0 , &RxHeader, RxData);
    printf("ID:%d len:%d data:",(int)RxHeader.StdId, (int)RxHeader.DLC);
    for (uint32_t i = 0; i < RxHeader.DLC; i++) {
      printf("%x ",RxData[i]);
    }
    printf("\r\n");
    if (RxHeader.StdId == 0x08) {
      s_chassis.velocity_x = RxData[0] | RxData[1] << 8;
      chassis_control(&s_chassis);
    }
    
  }
}


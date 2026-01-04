#include "chassis.h"
#include "bldc.h"
#include "can.h"
#include "main.h"
#include "stdio.h"
#include "stm32_hal_legacy.h"
#include <stdint.h>


chassis_obj s_chassis = {&g_bldc_motor1, &g_bldc_motor2, 0, 0, 0, 0, 0, 0, 0};
 

void chassis_control(chassis_obj *chassis , signal_source signal_type) {
  if (chassis->valid_can_num != 0 && signal_type != can_signal) {
    return;
  }
  int32_t duty1 = chassis->velocity_x + (chassis->velocity_z / 2);
  int32_t duty2 = chassis->velocity_x - (chassis->velocity_z / 2);
  // if (chassis->velocity_z > 0) {
    // duty1 = chassis->velocity_x + (chassis->velocity_z / 2);
    // duty2 = chassis->velocity_x - (chassis->velocity_z / 2);
  // }
  if (duty1 >= 0) {
    chassis->bldc1->pwm_duty_target = duty1;
    chassis->bldc1->dir_set = CCW;
    // duty2 = chassis->velocity_x - chassis->velocity_z;
  } else {
    chassis->bldc1->pwm_duty_target = -duty1;
    chassis->bldc1->dir_set = CW;
    // duty2 = chassis->velocity_x + chassis->velocity_z;
  }
  if (duty2 >= 0) {
    chassis->bldc2->pwm_duty_target = duty2;
    chassis->bldc2->dir_set = CW;
  } else {
    chassis->bldc2->pwm_duty_target = -duty2;
    chassis->bldc2->dir_set = CCW;
  }
  if (signal_type == can_signal) {
    chassis->valid_can_num= 1000;
  }
  else if (signal_type == remote_signal) {
    chassis->valid_remote_num = 10;
  }

  g_bldc_motor1.run_flag = RUN;
  g_bldc_motor2.run_flag = RUN;
  start_motor1();
  start_motor2();
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
  if (hcan->Instance == CAN1) {
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
    printf("ID:%d len:%d data:", (int)RxHeader.StdId, (int)RxHeader.DLC);
    for (uint32_t i = 0; i < RxHeader.DLC; i++) {
      printf("%x ", RxData[i]);
    }
    printf("\r\n");
    if (RxHeader.StdId == 0x08) {
      s_chassis.velocity_x = RxData[0] | RxData[1] << 8;
      s_chassis.velocity_z = RxData[2] | RxData[3] << 8;
      if (s_chassis.velocity_x > 32767) {
        s_chassis.velocity_x = s_chassis.velocity_x - 65536;
      }
      if (s_chassis.velocity_z > 32767) {
        s_chassis.velocity_z = s_chassis.velocity_z - 65536;
      }
      chassis_control(&s_chassis, can_signal);
      printf("pwm:%d\r\n", s_chassis.velocity_x);
    }
  }
}

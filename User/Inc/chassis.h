#ifndef _CHASSIS_H
#define _CHASSIS_H

#include "bldc.h"
#include "main.h"
#include "stm32f407xx.h"
#include <stdint.h>

#define remote_ch2 2
#define remote_ch4 4

typedef struct {
  __IO bldc_obj *bldc1;
  __IO bldc_obj *bldc2;
  __IO int32_t spead;
  __IO int32_t velocity_x;
  __IO int32_t velocity_z;
  __IO uint32_t remote_count_ch2;
  __IO uint32_t remote_count_ch4;

} chassis_obj;

extern chassis_obj s_chassis;
void chassis_control(chassis_obj *chassis);
#endif

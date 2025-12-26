#ifndef _CHASSIS_H
#define _CHASSIS_H

#include "bldc.h"
#include "main.h"
#include "stm32f407xx.h"
#include <stdint.h>


typedef struct {
  __IO bldc_obj *bldc1;
  __IO bldc_obj *bldc2;
  __IO int32_t spead;
  __IO int32_t velocity_x;
  __IO int32_t velocity_z;

} chassis_obj;

#endif

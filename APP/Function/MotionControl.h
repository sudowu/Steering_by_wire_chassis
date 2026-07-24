//
// Created by yang on 2026/7/11.
//

#ifndef BLDC_MOTIONCONTROL_H
#define BLDC_MOTIONCONTROL_H

#include "SbwTypes.h"


void MotionControl_Update(Chassis_Function* Chassis_Function_instance,
    float* Target_linear_velocity, float* Target_angular_velocity);

#endif //BLDC_MOTIONCONTROL_H

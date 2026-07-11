#include "SteeringFunction.h"

void SteeringFunction_Init(Steering_Function* sf)
{
    sf->Steering_Control.Steering_Config_Enable = 0;
    sf->Steering_Control.Target_Steeing_Angle   = 0;
    sf->Steering_Control.Target_Steeing_Speed   = 0;

    sf->Steering_Feedback.Steering_Angle = 0;
    sf->Steering_Feedback.Steering_Speed = 0;
    sf->Steering_Feedback.Steering_mode  = automatic;
    sf->Steering_Feedback.Fault_message  = 0;

    sf->Manual_Takeover = 0;
    sf->Over_Threshold  = 0;
}

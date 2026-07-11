#include "BrakingFunction.h"

void BrakingFunction_Init(Braking_Function* bf)
{
    bf->Braking_Control.Braking_Config_Enable         = 0;
    bf->Braking_Control.Target_Braking_Pedal_Position = 0;
    bf->Braking_Control.Target_Deceleration           = 0;
    bf->Braking_Control.Break_Light_Control           = 0;

    bf->Braking_Feedback.Current_Braking_System_Driving_Mode = automatic;
    bf->Braking_Feedback.Braking_Pedal_Position              = 0;
    bf->Braking_Feedback.Brake_Light_Status                  = 0;
    bf->Braking_Feedback.Fault_message                       = 0;

    bf->Manual_Takeover = 0;
    bf->Over_Threshold  = 0;
}

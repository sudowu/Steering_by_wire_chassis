#include "GearFunction.h"

void GearFunction_Init(Gear_Function* gf)
{
    gf->Gear_Control.Gear_Config_Enable   = 0;
    gf->Gear_Control.Target_Gear_Position = P;

    gf->Gear_Feedback.Gear_Position_Status = P;
    gf->Gear_Feedback.Fault_message        = 0;
}

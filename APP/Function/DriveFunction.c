#include "DriveFunction.h"

void DriveFunction_Init(Drive_Function* df)
{
    df->Drive_Control.Driving_Config_Enable             = 0;
    df->Drive_Control.Target_Accelerator_pedal_Position = 0;
    df->Drive_Control.Target_Longitudinal_Acceleration  = 0;
    df->Drive_Control.Target_Driving_Torque             = 0;

    df->Drive_Feedback.Driving_mode              = automatic;
    df->Drive_Feedback.Accelerator_pedal_Position = 0;
    df->Drive_Feedback.Longitudinal_Acceleration  = 0;
    df->Drive_Feedback.Vehicle_Speed              = 0;
    df->Drive_Feedback.Fault_message              = 0;

    df->Manual_Takeover = 0;
    df->Over_Threshold  = 0;
}

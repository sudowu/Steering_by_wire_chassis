#include "ParkingFunction.h"

void ParkingFunction_Init(Parking_Function* pf)
{
    pf->Parking_Control.Parking_Config_Enable = 0;
    pf->Parking_Control.Parking_Request       = 0;

    pf->Parking_Feedback.EPB_Switch_status                 = 0;
    pf->Parking_Feedback.Parking_status_feedback           = 0;
    pf->Parking_Feedback.Fault_message_for_parking_system  = 0;
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace control_software
{
    public enum CHASSIS_STATUS : byte
    {
        CHASSIS_STATUS_IDLE = 0,
        CHASSIS_STATUS_RETREAT = 1,
        CHASSIS_STATUS_RUNNING = 2,

    }
    public struct CHASSIS_INFO
    {
        public UInt16 Max_Power;
        public CHASSIS_STATUS status;
        public byte braking;
        public Byte stop_flag;
    }

    public class Chassis
    {




    }
}

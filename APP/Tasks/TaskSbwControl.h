#ifndef __TaskSbwControl_H__
#define __TaskSbwControl_H__
#include "main.h"
#include "FreeRTOS.h"



/********************************************
*驾驶模式
********************************************/
typedef enum {
    automatic = 0,        //自动模式
    manual,               //手动模式
    interrupted,          //中断模式(被人工接管)
}driving_mode;

/********************************************
*线控转向功能
********************************************/

//转向控制
typedef struct {
    uint8_t Steering_Config_Enable;     //使能
    uint8_t Target_Steeing_Angle;     //目标转向角度  1(deg)
    uint8_t Target_Steeing_Speed;     //目标转向速度  1(deg/s)
}Steering_Control;

//转向反馈
typedef struct {
    uint8_t Steering_Angle;           //方向盘角度  0.1(deg)
    uint8_t Steering_Speed;           //方向盘速度  1(deg/s)
    driving_mode Steering_mode;       //转向驾驶模式
    uint8_t Fault_message;            //故障信息
}Steering_Feedback;

//线控转向功能
typedef struct {
    Steering_Control Steering_Control;      //转向控制
    Steering_Feedback Steering_Feedback;    //转向反馈
    uint8_t Manual_Takeover;                //人工接管 方向盘扭矩门限
    uint8_t Over_Threshold;                 //越界处理 越界拒绝执行，并退出自动驾驶模式
}Steering_Function;

/********************************************
*线控驱动功能
********************************************/
//驱动控制
typedef struct {
    uint8_t Driving_Config_Enable;                   //使能
    uint8_t Target_Accelerator_pedal_Position;       //目标加速踏板位置 1(%)备选
    uint8_t Target_Longitudinal_Acceleration;        //目标车辆加速度  0.1(m/s^2)首选
    uint8_t Target_Driving_Torque;                   //目标驱动扭矩  1(N·m)
}Drive_Control;

//驱动反馈
typedef struct {
    driving_mode Driving_mode;                  //驾驶模式
    uint8_t Accelerator_pedal_Position;         //加速踏板位置  1(%)
    uint8_t Longitudinal_Acceleration;          //车辆实际纵向加速度  0.1(m/s^2)
    uint8_t Vehicle_Speed;                      //车辆实际纵向速度  0.1kph
    uint8_t Fault_message;                      //故障信息
}Drive_Feedback;

//线控驱动功能
typedef struct {
    Drive_Control Drive_Control;      //驱动控制
    Drive_Feedback Drive_Feedback;    //驱动反馈
    uint8_t Manual_Takeover;          //人工接管 加速踏板指令覆盖
    uint8_t Over_Threshold;           //越界处理 越界拒绝执行，并退出自动驾驶模式
}Drive_Function;
/********************************************
*线控制动功能
********************************************/
//制动控制
typedef struct {
    uint8_t Braking_Config_Enable;              //使能
    uint8_t Target_Braking_Pedal_Position;      //目标制动踏板位置 0.1%
    uint8_t Target_Deceleration;                //目标减速度  0.1(m/s^2)
    uint8_t Break_Light_Control;                //制动灯控制
}Braking_Control;

//制动反馈
typedef struct {
    driving_mode Current_Braking_System_Driving_Mode;       //当前驾驶模式
    uint8_t Braking_Pedal_Position;                         //制动踏板位置  0.1%
    uint8_t Brake_Light_Status;                             //制动灯状态
    uint8_t Fault_message;                                  //故障信息
}Braking_Feedback;

//线控制动功能
typedef struct {
    Braking_Control Braking_Control;      //制动控制
    Braking_Feedback Braking_Feedback;    //制动反馈
    uint8_t Manual_Takeover;          //人工接管 制动踏板指令覆盖
    uint8_t Over_Threshold;           //越界处理 越界拒绝执行，并退出自动驾驶模式
}Braking_Function;

/********************************************
*线控挡位功能
********************************************/
//档位位置
typedef enum {
    P = 0,      //驻车
    R,          //倒挡
    N,          //空挡
    D,          //前进挡
}Gear_Position;

//挡位控制
typedef struct {
    uint8_t Gear_Config_Enable;       //使能
    Gear_Position Target_Gear_Position;     //目标挡位位置P、R、N、D
}Gear_Control;

//档位反馈
typedef struct {
    Gear_Position Gear_Position_Status;     //当前挡位位置
    uint8_t Fault_message;                   //故障信息
}Gear_Feedback;

//线控档位功能
typedef struct {
    Gear_Control Gear_Control;      //档位控制
    Gear_Feedback Gear_Feedback;    //档位反馈
}Gear_Function;

/********************************************
*线控驻车功能
********************************************/
//驻车控制
typedef struct {
    uint8_t Parking_Config_Enable;      //使能
    uint8_t Parking_Request;            //驻车请求
}Parking_Control;

//驻车反馈
typedef struct {
    uint8_t EPB_Switch_status;                  //EPB开关状态
    uint8_t Parking_status_feedback;            //驻车状态反馈
    uint8_t Fault_message_for_parking_system;   //故障信息
}Parking_Feedback;

//线控驻车功能
typedef struct {
    Parking_Control Parking_Control;      //驻车控制
    Parking_Feedback Parking_Feedback;    //驻车反馈
}Parking_Function;

/********************************************
*线控底盘控制功能
********************************************/
typedef struct {
    Steering_Function Steering_Function;    //线控转向功能
    Drive_Function Drive_Function;          //线控驱动功能
    Braking_Function Braking_Function;      //线控制动功能
    Gear_Function Gear_Function;            //线控档位功能
    Parking_Function Parking_Function;      //线控驻车功能
}Chassis_Function;

extern Chassis_Function Chassis_Function_instance;





#endif

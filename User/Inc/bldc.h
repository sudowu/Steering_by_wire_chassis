//
// Created by yang on 2025/12/11.
//

#ifndef BLDC_BLDC_H
#define BLDC_BLDC_H

#include "main.h"

#define MOTOR_1                     1
#define MOTOR_2                     2

#define CCW             (1)     //逆时针
#define CW              (2)     //顺时针
#define HALL_ERROR      (0XF0)  //霍尔错误标志
#define RUN             (1)
#define STOP            (0)

typedef struct
{
    __IO uint8_t run_flag;                  /* 运行标志 */
    __IO uint8_t locket_rotor;              /* 堵转标记 */
    __IO uint8_t step_sta;                  /* 本次霍尔状态 */
    __IO uint8_t hall_single_sta;           /* 单个霍尔状态 */
    __IO uint8_t hall_sta_edge;             /* 单个霍尔状态跳变 */
    __IO uint8_t step_last;                 /* 上次霍尔状态 */
    __IO uint8_t dir;                       /* 电机旋转方向 */
    __IO int32_t pos;                       /* 电机位置 */
    __IO int32_t speed;                     /* 电机速度 */
    __IO int16_t current;                   /* 电机速度 */
    __IO uint16_t pwm_duty;                 /* 电机占空比 */
    __IO uint32_t hall_keep_t;              /* 霍尔保持时间 */
    __IO uint32_t hall_pull_num;            /* 霍尔传感器脉冲数 */
    __IO uint32_t lock_time;                /* 电机堵转时间 */
    __IO uint32_t no_single;
    __IO uint32_t count_j;
} bldc_obj;




#define SHUTDOWN1_EN    HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port, SHUTDOWN1_Pin, GPIO_PIN_SET);
#define SHUTDOWN1_OFF   HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port, SHUTDOWN1_Pin, GPIO_PIN_RESET);

#define SHUTDOWN2_EN    HAL_GPIO_WritePin(SHUTDOWN2_GPIO_Port, SHUTDOWN2_Pin, GPIO_PIN_SET);
#define SHUTDOWN2_OFF   HAL_GPIO_WritePin(SHUTDOWN2_GPIO_Port, SHUTDOWN2_Pin, GPIO_PIN_RESET);

#define MAX_PWM_DUTY    ((10000-1)*0.96)

typedef void(*pctr) (void);

void stop_motor1(void);
void stop_motor2(void);
void start_motor1(void);
void start_motor2(void);

/******************************************************************************************/
/* 外部接口函数*/
// void bldc_init(uint16_t arr, uint16_t psc);             /* BLDC初始化 */
uint8_t check_hall_dir(bldc_obj * obj);                /* 检测电机旋转方向 */
void hall_gpio_init(void);                              /* 霍尔接口初始化 */
uint32_t hallsensor_get_state(uint8_t motor_id);        /* 获取霍尔状态 */

extern pctr pfunclist_m1[6];                            /* 六步换相函数指针数组 */
void bldc_ctrl(uint8_t motor_id,int32_t dir,float duty);/* bldc控制函数 */
extern pctr pfunclist_m2[6];                            /* 六步换相函数指针数组 */

/*  六步换相 */
void m1_uhvl(void);
void m1_uhwl(void);
void m1_vhwl(void);
void m1_vhul(void);
void m1_whul(void);
void m1_whvl(void);

void m2_uhvl(void);
void m2_uhwl(void);
void m2_vhwl(void);
void m2_vhul(void);
void m2_whul(void);
void m2_whvl(void);



#endif //BLDC_BLDC_H
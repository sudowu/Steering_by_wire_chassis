/*
 * @file    bldc.h
 * @brief   无刷直流电机控制模块头文件
 * @details 包含BLDC电机对象定义、常量定义和函数声明
 * @author  yang
 * @date    2025-12-11
 * @note    定义了BLDC电机的控制接口和数据结构
 */

#ifndef BLDC_BLDC_H
#define BLDC_BLDC_H

#include "main.h"
#include <stdint.h>

#define DUTY_STEP_UP                1      /* PWM占空比递增步长 */
#define DUTY_STEP_DOWN              1      /* PWM占空比递减步长 */

#define MOTOR_1                     1      /* 电机1标识符 */
#define MOTOR_2                     2      /* 电机2标识符 */

#define CCW             (1)     /* 逆时针旋转方向标志 */
#define CW              (2)     /* 顺时针旋转方向标志 */
#define HALL_ERROR      (0XF0)  /* 霍尔传感器错误标志 */
#define RUN             (1)     /* 电机运行状态标志 */
#define STOP            (0)     /* 电机停止状态标志 */

/* BLDC电机对象结构体，包含电机的所有状态和控制参数 */
typedef struct
{
    __IO uint8_t run_flag;                  /* 电机运行标志：RUN/STOP */
    __IO uint8_t locket_rotor;              /* 电机堵转检测标记 */
    __IO uint8_t step_sta;                  /* 当前霍尔传感器状态 */
    __IO uint8_t hall_single_sta;           /* 单个霍尔传感器状态 */
    __IO uint8_t hall_sta_edge;             /* 霍尔状态跳变沿检测 */
    __IO uint8_t step_last;                 /* 上一次霍尔传感器状态 */
    __IO uint8_t dir;                       /* 电机实际旋转方向 */
    __IO uint8_t dir_set;                  /* 设定的电机旋转方向 */
    __IO int32_t pos;                       /* 电机当前位置 */
    __IO int32_t speed;                     /* 电机当前转速 */
    __IO int16_t current;                   /* 电机电流值 */
    __IO uint16_t pwm_duty;                 /* 当前PWM占空比 */
    __IO uint16_t pwm_duty_target;          /* 目标PWM占空比 */
    __IO uint32_t hall_keep_t;              /* 霍尔状态保持时间 */
    __IO uint32_t hall_pull_num;            /* 霍尔传感器捕获的脉冲数 */
    __IO uint32_t lock_time;                /* 电机堵转持续时间 */
    __IO uint32_t no_single;                /* 未使用的参数1 */
    __IO uint32_t count_j;                  /* 未使用的参数2 */
} bldc_obj;

/* 全局BLDC电机对象实例，分别对应系统中的两个电机 */
extern bldc_obj g_bldc_motor1;              /* 外部声明的电机1对象 */
extern bldc_obj g_bldc_motor2;              /* 外部声明的电机2对象 */


#define SHUTDOWN1_EN    HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port, SHUTDOWN1_Pin, GPIO_PIN_SET);
#define SHUTDOWN1_OFF   HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port, SHUTDOWN1_Pin, GPIO_PIN_RESET);

#define SHUTDOWN2_EN    HAL_GPIO_WritePin(SHUTDOWN2_GPIO_Port, SHUTDOWN2_Pin, GPIO_PIN_SET);
#define SHUTDOWN2_OFF   HAL_GPIO_WritePin(SHUTDOWN2_GPIO_Port, SHUTDOWN2_Pin, GPIO_PIN_RESET);

#define MAX_PWM_DUTY    ((4000-1)*0.96)     /* 最大PWM占空比限制，保留一定余量防止达到100% */

typedef void(*pctr) (void);

/**
 * @brief 停止电机1
 * @param None
 * @retval None
 */
void stop_motor1(void);

/**
 * @brief 停止电机2
 * @param None
 * @retval None
 */
void stop_motor2(void);

/**
 * @brief 启动电机1
 * @param None
 * @retval None
 */
void start_motor1(void);

/**
 * @brief 启动电机2
 * @param None
 * @retval None
 */
void start_motor2(void);

/******************************************************************************************/
/* 外部接口函数*/
// void bldc_init(uint16_t arr, uint16_t psc);             /* BLDC初始化 */
/**
 * @brief 检测电机旋转方向
 * @param obj 电机对象指针
 * @retval uint8_t 电机实际旋转方向
 */
uint8_t check_hall_dir(bldc_obj * obj);                /* 检测电机旋转方向 */

/**
 * @brief 霍尔传感器GPIO初始化
 * @param None
 * @retval None
 */
void hall_gpio_init(void);                              /* 霍尔接口初始化 */

/**
 * @brief 获取霍尔传感器状态
 * @param motor_id 电机ID
 * @retval uint32_t 霍尔传感器状态值
 */
uint32_t hallsensor_get_state(uint8_t motor_id);        /* 获取霍尔状态 */

extern pctr pfunclist_m1[6];                            /* 电机1六步换相函数指针数组 */

/**
 * @brief BLDC电机控制函数
 * @param motor_id 电机ID
 * @param dir 电机旋转方向
 * @param duty PWM占空比
 * @retval None
 */
void bldc_ctrl(uint8_t motor_id,int32_t dir,float duty);/* bldc控制函数 */
extern pctr pfunclist_m2[6];                            /* 电机2六步换相函数指针数组 */

/*  电机1六步换相函数声明 */
/**
 * @brief 电机1换相步骤1：U相高边导通，V相低边导通
 * @param None
 * @retval None
 */
void m1_uhvl(void);

/**
 * @brief 电机1换相步骤2：U相高边导通，W相低边导通
 * @param None
 * @retval None
 */
void m1_uhwl(void);

/**
 * @brief 电机1换相步骤3：V相高边导通，W相低边导通
 * @param None
 * @retval None
 */
void m1_vhwl(void);

/**
 * @brief 电机1换相步骤4：V相高边导通，U相低边导通
 * @param None
 * @retval None
 */
void m1_vhul(void);

/**
 * @brief 电机1换相步骤5：W相高边导通，U相低边导通
 * @param None
 * @retval None
 */
void m1_whul(void);

/**
 * @brief 电机1换相步骤6：W相高边导通，V相低边导通
 * @param None
 * @retval None
 */
void m1_whvl(void);

/*  电机2六步换相函数声明 */
/**
 * @brief 电机2换相步骤1：U相高边导通，V相低边导通
 * @param None
 * @retval None
 */
void m2_uhvl(void);

/**
 * @brief 电机2换相步骤2：U相高边导通，W相低边导通
 * @param None
 * @retval None
 */
void m2_uhwl(void);

/**
 * @brief 电机2换相步骤3：V相高边导通，W相低边导通
 * @param None
 * @retval None
 */
void m2_vhwl(void);

/**
 * @brief 电机2换相步骤4：V相高边导通，U相低边导通
 * @param None
 * @retval None
 */
void m2_vhul(void);

/**
 * @brief 电机2换相步骤5：W相高边导通，U相低边导通
 * @param None
 * @retval None
 */
void m2_whul(void);

/**
 * @brief 电机2换相步骤6：W相高边导通，V相低边导通
 * @param None
 * @retval None
 */
void m2_whvl(void);



#endif //BLDC_BLDC_H

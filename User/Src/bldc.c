//
// Created by yang on 2025/12/11.
//

#include "../Inc/bldc.h"

#include "tim.h"

bldc_obj g_bldc_motor1 = {STOP,0,0,CCW,0,0,0,0,0,0};   /* 电机结构体 */
bldc_obj g_bldc_motor2 = {STOP,0,0,CCW,0,0,0,0,0,0};   /* 电机结构体 */

const uint8_t hall_table_cw[6] = {6,2,3,1,5,4};         /* 顺时针旋转表 */
const uint8_t hall_table_ccw[6] = {5,1,3,2,6,4};        /* 逆时针旋转表 */

const uint8_t hall_cw_table[12] = {0x62,0x23,0x31,0x15,0x54,0x46,0x63,0x21,0x35,0x14,0x56,0x42};
const uint8_t hall_ccw_table[12] = {0x45,0x51,0x13,0x32,0x26,0x64,0x41,0x53,0x12,0x36,0x24,0x65};
void stop_motor1(void)
{
    /* 关闭半桥芯片输出 */
    SHUTDOWN1_OFF;
    /* 关闭PWM输出 */
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_3);
    /* 上下桥臂全部关断 */
    htim1.Instance->CCR2 = 0;
    htim1.Instance->CCR1 = 0;
    htim1.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port,M1_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port,M1_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port,M1_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
}
void stop_motor2(void);
void start_motor1(void);
void start_motor2(void);

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








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

void bldc_ctrl(uint8_t motor_id, int32_t dir, float duty)
{
    if(motor_id == MOTOR_1)
    {
        g_bldc_motor1.dir = dir;            /* 方向 */
        g_bldc_motor1.pwm_duty = duty;      /* 占空比 */
    }
    if(motor_id == MOTOR_2)
    {
        g_bldc_motor2.dir = dir;            /* 方向 */
        g_bldc_motor2.pwm_duty = duty;      /* 占空比 */
    }
}


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
void stop_motor2(void)
{
    {
        SHUTDOWN2_OFF;
        HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_1);
        HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_2);
        HAL_TIM_PWM_Stop(&htim8,TIM_CHANNEL_3);
        HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port,M2_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port,M2_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port,M2_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
    }
}
void start_motor1(void)
{
    SHUTDOWN1_EN;
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
}
void start_motor2(void)
{
    SHUTDOWN2_EN;
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
}

uint8_t check_hall_dir(bldc_obj * obj)                /* 检测电机旋转方向 */
{
    {
        uint8_t temp,res = HALL_ERROR;
        if((obj->step_last <= 6)&&(obj->step_sta <= 6))
        {
            temp = ((obj->step_last & 0x0F) << 4)|(obj->step_sta & 0x0F);
            if((temp == hall_ccw_table[0])||(temp == hall_ccw_table[1])||\
                    (temp == hall_ccw_table[2])||(temp == hall_ccw_table[3])||\
                    (temp == hall_ccw_table[4])||(temp == hall_ccw_table[5]))
            {
                res  = CCW;
            }
            else if((temp == hall_cw_table[0])||(temp == hall_cw_table[1])||\
                    (temp == hall_cw_table[2])||(temp == hall_cw_table[3])||\
                    (temp == hall_cw_table[4])||(temp == hall_cw_table[5]))
            {
                res  = CW;
            }
        }
        return res;
    }
}

uint32_t hallsensor_get_state(uint8_t motor_id)        /* 获取霍尔状态 */
{
    static __IO uint32_t state ;
    state  = 0;
    if(motor_id == MOTOR_1)
    {
        if(HAL_GPIO_ReadPin(HALL1_TIM_CH1_GPIO_Port,HALL1_TIM_CH1_Pin) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x01U;
        }
        if(HAL_GPIO_ReadPin(HALL1_TIM_CH2_GPIO_Port,HALL1_TIM_CH2_Pin) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x02U;
        }
        if(HAL_GPIO_ReadPin(HALL1_TIM_CH3_GPIO_Port,HALL1_TIM_CH3_Pin) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x04U;
        }
    }
    else if(motor_id == MOTOR_2)
    {
        if(HAL_GPIO_ReadPin(HALL2_TIM_CH1_GPIO_Port,HALL2_TIM_CH1_Pin) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x01U;
        }
        if(HAL_GPIO_ReadPin(HALL2_TIM_CH2_GPIO_Port,HALL2_TIM_CH2_Pin) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x02U;
        }
        if(HAL_GPIO_ReadPin(HALL2_TIM_CH3_GPIO_Port,HALL2_TIM_CH3_Pin) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x04U;
        }
    }
    return state;
}
/*  六步换向函数指针 */
pctr pfunclist_m1[6] =
{
    &m1_uhwl, &m1_vhul, &m1_vhwl,
    &m1_whvl, &m1_uhvl, &m1_whul
};

/*  六步换相 */
void m1_uhvl(void)
{
    htim1.Instance->CCR1 = g_bldc_motor1.pwm_duty;
    htim1.Instance->CCR2 = 0;
    htim1.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port,M1_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port,M1_LOW_SIDE_V_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port,M1_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
}
void m1_uhwl(void)
{
    htim1.Instance->CCR1 = g_bldc_motor1.pwm_duty;
    htim1.Instance->CCR2 = 0;
    htim1.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port,M1_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port,M1_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port,M1_LOW_SIDE_W_Pin,GPIO_PIN_SET);
}
void m1_vhwl(void)
{
    htim1.Instance->CCR1 = 0;
    htim1.Instance->CCR2 = g_bldc_motor1.pwm_duty;
    htim1.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port,M1_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port,M1_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port,M1_LOW_SIDE_W_Pin,GPIO_PIN_SET);
}
void m1_vhul(void)
{
    htim1.Instance->CCR1 = 0;
    htim1.Instance->CCR2 = g_bldc_motor1.pwm_duty;
    htim1.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port,M1_LOW_SIDE_U_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port,M1_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port,M1_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
}
void m1_whul(void)
{
    htim1.Instance->CCR1 = 0;
    htim1.Instance->CCR2 = 0;
    htim1.Instance->CCR3 = g_bldc_motor1.pwm_duty;
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port,M1_LOW_SIDE_U_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port,M1_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port,M1_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
}
void m1_whvl(void)
{
    htim1.Instance->CCR1 = 0;
    htim1.Instance->CCR2 = 0;
    htim1.Instance->CCR3 = g_bldc_motor1.pwm_duty;
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port,M1_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port,M1_LOW_SIDE_V_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port,M1_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
}

pctr pfunclist_m2[6] =
{
    &m2_uhwl, &m2_vhul, &m2_vhwl,
    &m2_whvl, &m2_uhvl, &m2_whul
};

void m2_uhvl(void)
{
    htim8.Instance->CCR1 = g_bldc_motor1.pwm_duty;
    htim8.Instance->CCR2 = 0;
    htim8.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port,M2_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port,M2_LOW_SIDE_V_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port,M2_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
}
void m2_uhwl(void)
{
    htim8.Instance->CCR1 = g_bldc_motor1.pwm_duty;
    htim8.Instance->CCR2 = 0;
    htim8.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port,M2_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port,M2_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port,M2_LOW_SIDE_W_Pin,GPIO_PIN_SET);
}
void m2_vhwl(void)
{
    htim8.Instance->CCR1 = 0;
    htim8.Instance->CCR2 = g_bldc_motor1.pwm_duty;
    htim8.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port,M2_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port,M2_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port,M2_LOW_SIDE_W_Pin,GPIO_PIN_SET);
}
void m2_vhul(void)
{
    htim8.Instance->CCR1 = 0;
    htim8.Instance->CCR2 = g_bldc_motor1.pwm_duty;
    htim8.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port,M2_LOW_SIDE_U_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port,M2_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port,M2_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
}
void m2_whul(void)
{
    htim8.Instance->CCR1 = 0;
    htim8.Instance->CCR2 = 0;
    htim8.Instance->CCR3 = g_bldc_motor1.pwm_duty;
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port,M2_LOW_SIDE_U_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port,M2_LOW_SIDE_V_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port,M2_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
}
void m2_whvl(void)
{
    htim8.Instance->CCR1 = 0;
    htim8.Instance->CCR2 = 0;
    htim8.Instance->CCR3 = g_bldc_motor1.pwm_duty;
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port,M2_LOW_SIDE_U_Pin,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port,M2_LOW_SIDE_V_Pin,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port,M2_LOW_SIDE_W_Pin,GPIO_PIN_RESET);
}



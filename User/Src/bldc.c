/*
 * @file    bldc.c
 * @brief   无刷直流电机控制模块实现
 * @details 包含六步换相算法、霍尔传感器检测、电机启停控制等功能
 * @author  yang
 * @date    2025-12-11
 * @note    实现了BLDC电机的控制逻辑和方向检测算法
 */

#include "../Inc/bldc.h"

#include "chassis.h"
#include "main.h"
#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "tim.h"
#include <stdint.h>
#include <stdio.h>

/* 全局电机对象实例，分别对应两个无刷直流电机 */
bldc_obj g_bldc_motor1 = {STOP, 0, 0, CCW, 0, 0, 0, 0, 0,
                          0, 0,    0, 0, 0,   0, 0, 0, 0}; /* 电机1结构体初始化 */
bldc_obj g_bldc_motor2 = {STOP, 0, 0, CCW, 0, 0, 0, 0, 0,
                          0, 0,    0, 0, 0,   0, 0, 0, 0}; /* 电机2结构体初始化 */

/* 电机换相表，用于实现六步换相算法 */
const uint8_t hall_table_cw[6] = {6, 2, 3, 1, 5, 4};  /* 顺时针旋转换相表 */
const uint8_t hall_table_ccw[6] = {5, 1, 3, 2, 6, 4}; /* 逆时针旋转换相表 */

/* 霍尔传感器状态检测表，用于判断电机实际旋转方向 */
const uint8_t hall_cw_table[12] = {0x62, 0x23, 0x31, 0x15, 0x54, 0x46,
                                   0x63, 0x21, 0x35, 0x14, 0x56, 0x42}; /* 顺时针旋转霍尔状态表 */
const uint8_t hall_ccw_table[12] = {0x45, 0x51, 0x13, 0x32, 0x26, 0x64,
                                    0x41, 0x53, 0x12, 0x36, 0x24, 0x65}; /* 逆时针旋转霍尔状态表 */

/**
 * @brief BLDC电机控制函数
 * @param motor_id 电机ID，MOTOR_1或MOTOR_2
 * @param dir 电机旋转方向，CCW(逆时针)或CW(顺时针)
 * @param duty PWM占空比，控制电机转速
 * @retval None
 * 
 * @details
 * 该函数根据指定的电机ID设置其旋转方向和目标占空比，
 * 为后续的电机驱动提供控制参数。
 */
void bldc_ctrl(uint8_t motor_id, int32_t dir, float duty) {
  if (motor_id == MOTOR_1) {
    g_bldc_motor1.dir_set = dir;          /* 设置电机1的旋转方向 */
    g_bldc_motor1.pwm_duty_target = duty; /* 设置电机1的目标占空比 */
  }
  if (motor_id == MOTOR_2) {
    g_bldc_motor2.dir_set = dir;          /* 设置电机2的旋转方向 */
    g_bldc_motor2.pwm_duty_target = duty; /* 设置电机2的目标占空比 */
  }
}

/**
 * @brief 停止电机1
 * @param None
 * @retval None
 * 
 * @details
 * 该函数安全地停止电机1的运行，包括逐步降低占空比、
 * 关闭PWM输出、切断半桥芯片供电等步骤，确保电机平稳停止。
 */
void stop_motor1(void) {
  /* 逐步关闭电机，等待电机完全停止 */
  uint8_t time_count = 0;
  g_bldc_motor1.pwm_duty_target = 0;
  /* 等待电机停止，最多等待2秒 */
  while (g_bldc_motor1.pwm_duty && time_count <= 20) {
    HAL_Delay(100);
    time_count++;
  }
  g_bldc_motor1.run_flag = STOP;      /* 设置电机运行标志为停止 */
  SHUTDOWN1_OFF;                      /* 关闭电机1的半桥驱动芯片 */
  /* 安全关闭所有PWM通道输出 */
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
  /* 强制将上下桥臂全部关断，确保电机完全停止 */
  htim1.Instance->CCR2 = 0;
  htim1.Instance->CCR1 = 0;
  htim1.Instance->CCR3 = 0;
  /* 将低侧开关管全部拉低，确保电机绕组不产生反电动势 */
  HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port, M1_LOW_SIDE_U_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port, M1_LOW_SIDE_V_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port, M1_LOW_SIDE_W_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 停止电机2
 * @param None
 * @retval None
 * 
 * @details
 * 该函数安全地停止电机2的运行，包括逐步降低占空比、
 * 关闭PWM输出、切断半桥芯片供电等步骤，确保电机平稳停止。
 */
void stop_motor2(void) {
  {
    /* 逐步关闭电机，等待电机完全停止 */
    uint8_t time_count = 0;
    g_bldc_motor2.pwm_duty_target = 0;
    /* 等待电机停止，最多等待2秒 */
    while (g_bldc_motor2.pwm_duty && time_count <= 20) {
      HAL_Delay(100);
      time_count++;
    }
    // while (g_bldc_motor2.pwm_duty);
    g_bldc_motor2.run_flag = STOP;      /* 设置电机运行标志为停止 */
    SHUTDOWN2_OFF;                      /* 关闭电机2的半桥驱动芯片 */
    /* 安全关闭所有PWM通道输出 */
    HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
    /* 将低侧开关管全部拉低，确保电机绕组不产生反电动势 */
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port, M2_LOW_SIDE_U_Pin,
                      GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port, M2_LOW_SIDE_V_Pin,
                      GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port, M2_LOW_SIDE_W_Pin,
                      GPIO_PIN_RESET);
  }
}

void start_motor1(void) {
  SHUTDOWN1_EN;
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
}

void start_motor2(void) {
  SHUTDOWN2_EN;
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
}

uint8_t check_hall_dir(bldc_obj *obj) /* 检测电机旋转方向 */
{
  {
    uint8_t temp, res = HALL_ERROR;
    if ((obj->step_last <= 6) && (obj->step_sta <= 6)) {
      temp = ((obj->step_last & 0x0F) << 4) | (obj->step_sta & 0x0F);
      if ((temp == hall_ccw_table[0]) || (temp == hall_ccw_table[1]) ||
          (temp == hall_ccw_table[2]) || (temp == hall_ccw_table[3]) ||
          (temp == hall_ccw_table[4]) || (temp == hall_ccw_table[5])) {
        res = CCW;
      } else if ((temp == hall_cw_table[0]) || (temp == hall_cw_table[1]) ||
                 (temp == hall_cw_table[2]) || (temp == hall_cw_table[3]) ||
                 (temp == hall_cw_table[4]) || (temp == hall_cw_table[5])) {
        res = CW;
      }
    }
    return res;
  }
}

uint32_t hallsensor_get_state(uint8_t motor_id) /* 获取霍尔状态 */
{
  static __IO uint32_t state;
  state = 0;
  if (motor_id == MOTOR_1) {
    if (HAL_GPIO_ReadPin(HALL1_TIM_CH1_GPIO_Port, HALL1_TIM_CH1_Pin) !=
        GPIO_PIN_RESET) /* 霍尔传感器状态获取 */
    {
      state |= 0x01U;
    }
    if (HAL_GPIO_ReadPin(HALL1_TIM_CH2_GPIO_Port, HALL1_TIM_CH2_Pin) !=
        GPIO_PIN_RESET) /* 霍尔传感器状态获取 */
    {
      state |= 0x02U;
    }
    if (HAL_GPIO_ReadPin(HALL1_TIM_CH3_GPIO_Port, HALL1_TIM_CH3_Pin) !=
        GPIO_PIN_RESET) /* 霍尔传感器状态获取 */
    {
      state |= 0x04U;
    }
  } else if (motor_id == MOTOR_2) {
    if (HAL_GPIO_ReadPin(HALL2_TIM_CH1_GPIO_Port, HALL2_TIM_CH1_Pin) !=
        GPIO_PIN_RESET) /* 霍尔传感器状态获取 */
    {
      state |= 0x01U;
    }
    if (HAL_GPIO_ReadPin(HALL2_TIM_CH2_GPIO_Port, HALL2_TIM_CH2_Pin) !=
        GPIO_PIN_RESET) /* 霍尔传感器状态获取 */
    {
      state |= 0x02U;
    }
    if (HAL_GPIO_ReadPin(HALL2_TIM_CH3_GPIO_Port, HALL2_TIM_CH3_Pin) !=
        GPIO_PIN_RESET) /* 霍尔传感器状态获取 */
    {
      state |= 0x04U;
    }
  }
  return state;
}

/*  六步换向函数指针 */
pctr pfunclist_m1[6] = {&m1_uhwl, &m1_vhul, &m1_vhwl,
                        &m1_whvl, &m1_uhvl, &m1_whul};

/*  六步换相 */
void m1_uhvl(void) {
  htim1.Instance->CCR1 = g_bldc_motor1.pwm_duty;
  htim1.Instance->CCR2 = 0;
  htim1.Instance->CCR3 = 0;
  HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port, M1_LOW_SIDE_U_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port, M1_LOW_SIDE_V_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port, M1_LOW_SIDE_W_Pin, GPIO_PIN_RESET);
}

void m1_uhwl(void) {
  htim1.Instance->CCR1 = g_bldc_motor1.pwm_duty;
  htim1.Instance->CCR2 = 0;
  htim1.Instance->CCR3 = 0;
  HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port, M1_LOW_SIDE_U_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port, M1_LOW_SIDE_V_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port, M1_LOW_SIDE_W_Pin, GPIO_PIN_SET);
}

void m1_vhwl(void) {
  htim1.Instance->CCR1 = 0;
  htim1.Instance->CCR2 = g_bldc_motor1.pwm_duty;
  htim1.Instance->CCR3 = 0;
  HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port, M1_LOW_SIDE_U_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port, M1_LOW_SIDE_V_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port, M1_LOW_SIDE_W_Pin, GPIO_PIN_SET);
}

void m1_vhul(void) {
  htim1.Instance->CCR1 = 0;
  htim1.Instance->CCR2 = g_bldc_motor1.pwm_duty;
  htim1.Instance->CCR3 = 0;
  HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port, M1_LOW_SIDE_U_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port, M1_LOW_SIDE_V_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port, M1_LOW_SIDE_W_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 电机1换相步骤5：W相高边导通，U相低边导通
 * @param None
 * @retval None
 * 
 * @details
 * W相接高边(+)，U相接低边(-)，V相反电动势
 * 此时电流从W相流向U相，产生预定方向的磁场
 */
void m1_whul(void) {
  htim1.Instance->CCR1 = 0;                           /* U相低电平 */
  htim1.Instance->CCR2 = 0;                           /* V相反电动势 */
  htim1.Instance->CCR3 = g_bldc_motor1.pwm_duty;      /* W相PWM输出 */
  /* 控制低边开关管状态 */
  HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port, M1_LOW_SIDE_U_Pin, GPIO_PIN_SET);    /* U相低边导通 */
  HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port, M1_LOW_SIDE_V_Pin, GPIO_PIN_RESET);  /* V相低边关断 */
  HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port, M1_LOW_SIDE_W_Pin, GPIO_PIN_RESET);  /* W相低边关断 */
}

/**
 * @brief 电机1换相步骤6：W相高边导通，V相低边导通
 * @param None
 * @retval None
 * 
 * @details
 * W相接高边(+)，V相接低边(-)，U相反电动势
 * 此时电流从W相流向V相，产生预定方向的磁场
 */
void m1_whvl(void) {
  htim1.Instance->CCR1 = 0;                           /* U相反电动势 */
  htim1.Instance->CCR2 = 0;                           /* V相低电平 */
  htim1.Instance->CCR3 = g_bldc_motor1.pwm_duty;      /* W相PWM输出 */
  /* 控制低边开关管状态 */
  HAL_GPIO_WritePin(M1_LOW_SIDE_U_GPIO_Port, M1_LOW_SIDE_U_Pin, GPIO_PIN_RESET);  /* U相低边关断 */
  HAL_GPIO_WritePin(M1_LOW_SIDE_V_GPIO_Port, M1_LOW_SIDE_V_Pin, GPIO_PIN_SET);    /* V相低边导通 */
  HAL_GPIO_WritePin(M1_LOW_SIDE_W_GPIO_Port, M1_LOW_SIDE_W_Pin, GPIO_PIN_RESET);  /* W相低边关断 */
}

/*  电机2六步换向函数指针数组，按照六步换相顺序排列 */
pctr pfunclist_m2[6] = {&m2_uhwl, &m2_vhul, &m2_vhwl,
                        &m2_whvl, &m2_uhvl, &m2_whul};

void m2_uhvl(void) {
  htim8.Instance->CCR1 = g_bldc_motor2.pwm_duty;
  htim8.Instance->CCR2 = 0;
  htim8.Instance->CCR3 = 0;
  HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port, M2_LOW_SIDE_U_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port, M2_LOW_SIDE_V_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port, M2_LOW_SIDE_W_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 电机2换相步骤2：U相高边导通，W相低边导通
 * @param None
 * @retval None
 * 
 * @details
 * U相接高边(+)，W相接低边(-)，V相反电动势
 * 此时电流从U相流向W相，产生预定方向的磁场
 */
void m2_uhwl(void) {
  htim8.Instance->CCR1 = g_bldc_motor2.pwm_duty;      /* U相PWM输出 */
  htim8.Instance->CCR2 = 0;                           /* V相反电动势 */
  htim8.Instance->CCR3 = 0;                           /* W相低电平 */
  /* 控制低边开关管状态 */
  HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port, M2_LOW_SIDE_U_Pin, GPIO_PIN_RESET);  /* U相低边关断 */
  HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port, M2_LOW_SIDE_V_Pin, GPIO_PIN_RESET);  /* V相低边关断 */
  HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port, M2_LOW_SIDE_W_Pin, GPIO_PIN_SET);    /* W相低边导通 */
}

/**
 * @brief 电机2换相步骤3：V相高边导通，W相低边导通
 * @param None
 * @retval None
 * 
 * @details
 * V相接高边(+)，W相接低边(-)，U相反电动势
 * 此时电流从V相流向W相，产生预定方向的磁场
 */
void m2_vhwl(void) {
  htim8.Instance->CCR1 = 0;                           /* U相反电动势 */
  htim8.Instance->CCR2 = g_bldc_motor2.pwm_duty;      /* V相PWM输出 */
  htim8.Instance->CCR3 = 0;                           /* W相低电平 */
  /* 控制低边开关管状态 */
  HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port, M2_LOW_SIDE_U_Pin, GPIO_PIN_RESET);  /* U相低边关断 */
  HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port, M2_LOW_SIDE_V_Pin, GPIO_PIN_RESET);  /* V相低边关断 */
  HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port, M2_LOW_SIDE_W_Pin, GPIO_PIN_SET);    /* W相低边导通 */
}

/**
 * @brief 电机2换相步骤4：V相高边导通，U相低边导通
 * @param None
 * @retval None
 * 
 * @details
 * V相接高边(+)，U相接低边(-)，W相反电动势
 * 此时电流从V相流向U相，产生预定方向的磁场
 */
void m2_vhul(void) {
  htim8.Instance->CCR1 = 0;                           /* U相低电平 */
  htim8.Instance->CCR2 = g_bldc_motor2.pwm_duty;      /* V相PWM输出 */
  htim8.Instance->CCR3 = 0;                           /* W相反电动势 */
  /* 控制低边开关管状态 */
  HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port, M2_LOW_SIDE_U_Pin, GPIO_PIN_SET);    /* U相低边导通 */
  HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port, M2_LOW_SIDE_V_Pin, GPIO_PIN_RESET);  /* V相低边关断 */
  HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port, M2_LOW_SIDE_W_Pin, GPIO_PIN_RESET);  /* W相低边关断 */
}

void m2_whul(void) {
  htim8.Instance->CCR1 = 0;
  htim8.Instance->CCR2 = 0;
  htim8.Instance->CCR3 = g_bldc_motor2.pwm_duty;
  HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port, M2_LOW_SIDE_U_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port, M2_LOW_SIDE_V_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port, M2_LOW_SIDE_W_Pin, GPIO_PIN_RESET);
}

void m2_whvl(void) {
  htim8.Instance->CCR1 = 0;
  htim8.Instance->CCR2 = 0;
  htim8.Instance->CCR3 = g_bldc_motor2.pwm_duty;
  HAL_GPIO_WritePin(M2_LOW_SIDE_U_GPIO_Port, M2_LOW_SIDE_U_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M2_LOW_SIDE_V_GPIO_Port, M2_LOW_SIDE_V_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(M2_LOW_SIDE_W_GPIO_Port, M2_LOW_SIDE_W_Pin, GPIO_PIN_RESET);
}



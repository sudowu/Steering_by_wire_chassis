//
// Created by yang on 2026/3/8.
//

#include "Motor.h"

#include <math.h>

#include "tim.h"

uint16_t ADC_Buffer[ADC_SAMPLE_GROUP][ADC_CHANNELS] = {0}; // ADC DMA 缓冲区
uint8_t ADC_DataReady = 0;


Motor_t g_Motor1 = {
    .id = MOTOR1,
    .direction = 0,
    .forward_direction = CCW,
    .hall_state = 0,
    .pwm_duty = 0,
    .run_state = 0,
    .commutating_counter = 0,
    .last_encoder_count = 0
};
Motor_t g_Motor2 = {
    .id = MOTOR2,
    .direction = 0,
    .forward_direction = CW,
    .hall_state = 0,
    .pwm_duty = 0,
    .run_state = 0,
    .commutating_counter = 0,
    .last_encoder_count = 0
};

const motor_pwm_config_t motor_pwm_configs[6] = {
    {TIM_CHANNEL_1, 3}, // 霍尔状态0b001
    {TIM_CHANNEL_2, 1}, // 霍尔状态0b010
    {TIM_CHANNEL_2, 3}, // 霍尔状态0b011
    {TIM_CHANNEL_3, 2}, // 霍尔状态0b100
    {TIM_CHANNEL_1, 2}, // 霍尔状态0b101
    {TIM_CHANNEL_3, 1} // 霍尔状态0b110
};


void motor_init()
{
    // 这里可以添加电机初始化的代码，例如配置GPIO、定时器等
    // 初始化电机1
    g_Motor1.id = MOTOR1;
    g_Motor1.htim = &htim1; // 关联定时器1
    g_Motor1.encoder = &htim3; // 编码器定时器
    g_Motor1.direction = 0;
    g_Motor1.forward_direction = CCW;
    g_Motor1.hall_state = 0;
    g_Motor1.pwm_duty = 0;
    g_Motor1.run_state = 0;
    g_Motor1.commutating_counter = 0;
    g_Motor1.last_encoder_count = 0;
    // 初始化电机2
    g_Motor2.id = MOTOR2;
    g_Motor2.htim = &htim8; // 关联定时器8
    g_Motor2.encoder = &htim2; // 编码器定时器
    g_Motor2.direction = 0;
    g_Motor2.forward_direction = CW;
    g_Motor2.hall_state = 0;
    g_Motor2.pwm_duty = 0;
    g_Motor2.run_state = 0;
    g_Motor2.commutating_counter = 0;
    g_Motor2.last_encoder_count = 0;
}


void motor_rpm_get(Motor_t* motor)
{
    // 这里可以添加读取电机转速的代码，例如通过编码器或霍尔传感器计算转速
    // 计算转速并更新 motor->rpm 字段

    int32_t currentCount = __HAL_TIM_GET_COUNTER(motor->encoder);
    int32_t delta = currentCount - motor->last_encoder_count;

    // 处理计数器溢出
    if (delta > 0x8000) delta -= 0x10000;
    else if (delta < -0x8000) delta += 0x10000;

    // RPM计算: 每转脉冲数=1024PPR, 采样周期=0.01s
    motor->rpm =  - (delta * 6000.0) / (1024 * 4); // 4倍频修正

    motor->last_encoder_count = currentCount;
}

HAL_StatusTypeDef Motor_OffsetCalibrate(Motor_t* motor)
{
    uint32_t sum_u = 0, sum_v = 0, sum_w = 0;
    // 确保电机处于停止状态
    if (motor->run_state != 0)
    {
        return HAL_ERROR; // 电机运行中，不能校准
    }
    // 等待 ADC 启动完成
    while (ADC_DataReady != 1)
    {
    }
    // 读取多组数据求平均
    for (int i = 0; i < ADC_OFFSET_SAMPLES; i++)
    {
        // 等待数据就绪
        while (ADC_DataReady != 1)
        {
        }
        ADC_DataReady = 0;
        // 累加当前值 (使用循环缓冲区)
        for (int j = 0; j < ADC_SAMPLE_GROUP; j++)
        {
            sum_u += ADC_Buffer[j][0];
            sum_v += ADC_Buffer[j][1];
            sum_w += ADC_Buffer[j][2];
        }
    }
    // 计算平均值作为偏置
    motor->adc_current.adc_offset_u = (float)sum_u / (ADC_OFFSET_SAMPLES * ADC_SAMPLE_GROUP); // 转换为电压值
    motor->adc_current.adc_offset_v = (float)sum_v / (ADC_OFFSET_SAMPLES * ADC_SAMPLE_GROUP);
    motor->adc_current.adc_offset_w = (float)sum_w / (ADC_OFFSET_SAMPLES * ADC_SAMPLE_GROUP);
    motor->adc_current.offset_calibrated = 1; // 标记已校准
    return HAL_OK;
}

float Motor_GetTotalCurrent(Motor_t* motor)
{
    float iu = motor->adc_current.adc_current_u;
    float iv = motor->adc_current.adc_current_v;
    float iw = motor->adc_current.adc_current_w;

    // 两导通相平均 → DC 母线电流幅值
    return (fabsf(iu) + fabsf(iv) + fabsf(iw)) / 2.0f;
}

HAL_StatusTypeDef Motor_ReadAdcCurrent(Motor_t* motor)
{
    uint32_t sum_u = 0, sum_v = 0, sum_w = 0;
    if (motor->adc_current.offset_calibrated == 0)
    {
        return HAL_ERROR; // 偏置未校准，无法读取电流
    }
    // 等待 ADC 启动完成
    while (ADC_DataReady != 1)
    {
    }
    ADC_DataReady = 0;
    // 读取当前值 (使用循环缓冲区的最新数据)
    for (int j = 0; j < ADC_SAMPLE_GROUP; j++)
    {
        sum_u += ADC_Buffer[j][0];
        sum_v += ADC_Buffer[j][1];
        sum_w += ADC_Buffer[j][2];
    }
    motor->adc_current.adc_current_u = (float)(sum_u / ADC_SAMPLE_GROUP - motor->adc_current.adc_offset_u) * 3.3 /
        4096.0 / 0.12;
    motor->adc_current.adc_current_v = (float)(sum_v / ADC_SAMPLE_GROUP - motor->adc_current.adc_offset_v) * 3.3 /
        4096.0 / 0.12;
    motor->adc_current.adc_current_w = (float)(sum_w / ADC_SAMPLE_GROUP - motor->adc_current.adc_offset_w) * 3.3 /
        4096.0 / 0.12;
    return HAL_OK;
}

void hall_start(Motor_t* motor)
{
    if (motor->id == MOTOR1)
    {
        if (HAL_TIMEx_HallSensor_Start_IT(&htim5) != HAL_OK)
        {
            while (1);
        }
    }
    else if (motor->id == MOTOR2)
    {
        if (HAL_TIMEx_HallSensor_Start_IT(&htim4) != HAL_OK)
        {
            while (1);
        }
    }
}

void hall_state_read(Motor_t* motor)
{
    uint8_t motor_hall_state = 0; // 这里需要根据实际硬件读取霍尔状态的代码来获取当前霍尔状态
    if (motor->id == MOTOR1)
    {
        motor_hall_state |= HAL_GPIO_ReadPin(PM1_HALLU_GPIO_Port, PM1_HALLU_Pin) << 0; // 霍尔1状态
        motor_hall_state |= HAL_GPIO_ReadPin(PM1_HALLV_GPIO_Port, PM1_HALLV_Pin) << 1; // 霍尔2状态
        motor_hall_state |= HAL_GPIO_ReadPin(PM1_HALLW_GPIO_Port, PM1_HALLW_Pin) << 2; // 霍尔3状态
    }
    else if (motor->id == MOTOR2)
    {
        motor_hall_state |= HAL_GPIO_ReadPin(PM2_HALLU_GPIO_Port, PM2_HALLU_Pin) << 0; // 霍尔1状态
        motor_hall_state |= HAL_GPIO_ReadPin(PM2_HALLV_GPIO_Port, PM2_HALLV_Pin) << 1; // 霍尔2状态
        motor_hall_state |= HAL_GPIO_ReadPin(PM2_HALLW_GPIO_Port, PM2_HALLW_Pin) << 2; // 霍尔3状态
    }
    if (motor_hall_state > 7)
    {
        motor_hall_state = 0; // 如果读取的霍尔状态超过7，重置为0，避免无效状态
    }
    if (motor->direction == 2) // 如果方向为逆时针，进行状态转换
    {
        motor_hall_state = 7 - motor_hall_state; // 逆时针状态转换
    }
    motor->hall_state = motor_hall_state; // 更新电机的霍尔状态
}

void motor_commutating_phase_callback(Motor_t* motor)
{
    if (motor->run_state == 0)
        return;
    hall_state_read(motor);
    if (motor->hall_state < 1 || motor->hall_state > 6)
        return;
    MotorControl(motor, &motor_pwm_configs[motor->hall_state - 1]);
    motor->commutating_counter++;
}

void MotorControl(const Motor_t* motor, const motor_pwm_config_t* config)
{
    __HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_3, 0);
    if (motor->id == MOTOR1)
    {
        switch (config->l_on_channel)
        {
        case 1:
            HAL_GPIO_WritePin(PM1_PWM_UL_GPIO_Port, PM1_PWM_UL_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(PM1_PWM_VL_GPIO_Port,PM1_PWM_VL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(PM1_PWM_WL_GPIO_Port,PM1_PWM_WL_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(PM1_PWM_UL_GPIO_Port, PM1_PWM_UL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(PM1_PWM_VL_GPIO_Port,PM1_PWM_VL_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(PM1_PWM_WL_GPIO_Port,PM1_PWM_WL_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            HAL_GPIO_WritePin(PM1_PWM_UL_GPIO_Port, PM1_PWM_UL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(PM1_PWM_VL_GPIO_Port,PM1_PWM_VL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(PM1_PWM_WL_GPIO_Port,PM1_PWM_WL_Pin, GPIO_PIN_SET);
            break;
        default:
            break;
        }
    }
    else if (motor->id == MOTOR2)
    {
        switch (config->l_on_channel)
        {
        case 1:
            HAL_GPIO_WritePin(PM2_PWM_UL_GPIO_Port, PM2_PWM_UL_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(PM2_PWM_VL_GPIO_Port,PM2_PWM_VL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(PM2_PWM_WL_GPIO_Port,PM2_PWM_WL_Pin, GPIO_PIN_RESET);
            break;
        case 2:
            HAL_GPIO_WritePin(PM2_PWM_UL_GPIO_Port, PM2_PWM_UL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(PM2_PWM_VL_GPIO_Port,PM2_PWM_VL_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(PM2_PWM_WL_GPIO_Port,PM2_PWM_WL_Pin, GPIO_PIN_RESET);
            break;
        case 3:
            HAL_GPIO_WritePin(PM2_PWM_UL_GPIO_Port, PM2_PWM_UL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(PM2_PWM_VL_GPIO_Port,PM2_PWM_VL_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(PM2_PWM_WL_GPIO_Port,PM2_PWM_WL_Pin, GPIO_PIN_SET);
            break;
        default:
            break;
        }
    }
    __HAL_TIM_SET_COMPARE(motor->htim, config->h_pwm_channel, motor->pwm_duty);
}

/* ==================== 电机控制 API 实现 ==================== */

/**
 * @brief 设置电机转向
 * @param motor 电机指针
 * @param direction 转向：STOP-停止，CW-顺时针，CCW-逆时针
 * @return HAL 状态
 */
HAL_StatusTypeDef Motor_SetDirection(Motor_t* motor, direction_t direction)
{
    if (motor == NULL)
    {
        return HAL_ERROR;
    }

    // 检查方向参数有效性
    if (direction > CCW)
    {
        return HAL_ERROR;
    }

    // 如果方向没有变化，直接返回
    if (motor->direction == direction)
    {
        return HAL_OK;
    }

    // 如果电机正在运行，需要先停止再重新换相
    if (motor->run_state == 1)
    {
        // 保存当前占空比
        uint16_t current_duty = motor->pwm_duty;

        // 先停止电机
        Motor_Stop(motor);

        // 更新方向
        motor->direction = direction;

        // 如果新方向不是停止，则重新启动电机
        if (direction != STOP)
        {
            // 短暂延时确保电机完全停止
            for(volatile int i = 0; i < 1000; i++);

            // 重新启动电机
            Motor_Start(motor, direction, current_duty);
        }
    }
    else
    {
        // 电机未运行，仅更新方向
        motor->direction = direction;
    }

    return HAL_OK;
}

/**
 * @brief 设置电机 PWM 占空比
 * @param motor 电机指针
 * @param duty_cycle 占空比值，范围 0-1000 (对应 0%-100%)
 * @return HAL 状态
 */
HAL_StatusTypeDef Motor_SetDutyCycle(Motor_t* motor, uint16_t duty_cycle)
{
    if (motor == NULL)
    {
        return HAL_ERROR;
    }

    // 限制占空比范围 0-1000
    if (duty_cycle > 1000)
    {
        duty_cycle = 1000;
    }

    // 如果占空比没有变化，直接返回
    if (motor->pwm_duty == duty_cycle)
    {
        return HAL_OK;
    }

    motor->pwm_duty = duty_cycle;

    // 如果电机正在运行，立即更新 PWM 输出
    if (motor->run_state == 1 && motor->hall_state >= 1 && motor->hall_state <= 6)
    {
        // 清除所有通道
        __HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_2, 0);
        __HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_3, 0);

        // 设置当前霍尔状态对应的通道占空比
        uint8_t channel = motor_pwm_configs[motor->hall_state - 1].h_pwm_channel;
        __HAL_TIM_SET_COMPARE(motor->htim, channel, motor->pwm_duty);
    }

    return HAL_OK;
}

/**
 * @brief 启动电机
 * @param motor 电机指针
 * @param direction 转向：CW-顺时针，CCW-逆时针
 * @param duty_cycle 占空比值，范围 0-1000
 * @return HAL 状态
 */
HAL_StatusTypeDef Motor_Start(Motor_t* motor, direction_t direction, uint16_t duty_cycle)
{
    if (motor == NULL)
    {
        return HAL_ERROR;
    }

    if (direction == STOP)
    {
        return Motor_Stop(motor);
    }

    // 限制占空比范围
    if (duty_cycle > 1000)
    {
        duty_cycle = 1000;
    }

    motor->direction = direction;
    motor->pwm_duty = duty_cycle;
    motor->run_state = 1;

    // 读取霍尔状态
    hall_state_read(motor);

    // 执行换相
    if (motor->hall_state < 1 || motor->hall_state > 6)
    {
        motor->run_state = 0;
        return HAL_ERROR;
    }

    // 启动 PWM
    HAL_TIM_PWM_Start(motor->htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(motor->htim, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(motor->htim, TIM_CHANNEL_3);

    // 使能驱动芯片
    if (motor->id == MOTOR1)
    {
        HAL_GPIO_WritePin(PM1_CTRL_SD_GPIO_Port, PM1_CTRL_SD_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(PM1_PWM_UL_GPIO_Port, PM1_PWM_UL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PM1_PWM_VL_GPIO_Port, PM1_PWM_VL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PM1_PWM_WL_GPIO_Port, PM1_PWM_WL_Pin, GPIO_PIN_RESET);
    }
    else if (motor->id == MOTOR2)
    {
        HAL_GPIO_WritePin(PM2_CTRL_SD_GPIO_Port, PM2_CTRL_SD_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(PM2_PWM_UL_GPIO_Port, PM2_PWM_UL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PM2_PWM_VL_GPIO_Port, PM2_PWM_VL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PM2_PWM_WL_GPIO_Port, PM2_PWM_WL_Pin, GPIO_PIN_RESET);
    }

    MotorControl(motor, &motor_pwm_configs[motor->hall_state - 1]);

    return HAL_OK;
}

/**
 * @brief 停止电机
 * @param motor 电机指针
 * @return HAL 状态
 */
HAL_StatusTypeDef Motor_Stop(Motor_t* motor)
{
    if (motor == NULL)
    {
        return HAL_ERROR;
    }

    motor->run_state = 0;
    motor->pwm_duty = 0;

    // 关闭 PWM 输出
    __HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_2, 0);
    __HAL_TIM_SET_COMPARE(motor->htim, TIM_CHANNEL_3, 0);

    HAL_TIM_PWM_Stop(motor->htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(motor->htim, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(motor->htim, TIM_CHANNEL_3);

    // 关闭驱动芯片下桥臂
    if (motor->id == MOTOR1)
    {
        HAL_GPIO_WritePin(PM1_PWM_UL_GPIO_Port, PM1_PWM_UL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PM1_PWM_VL_GPIO_Port, PM1_PWM_VL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PM1_PWM_WL_GPIO_Port, PM1_PWM_WL_Pin, GPIO_PIN_RESET);
    }
    else if (motor->id == MOTOR2)
    {
        HAL_GPIO_WritePin(PM2_PWM_UL_GPIO_Port, PM2_PWM_UL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PM2_PWM_VL_GPIO_Port, PM2_PWM_VL_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(PM2_PWM_WL_GPIO_Port, PM2_PWM_WL_Pin, GPIO_PIN_RESET);
    }

    return HAL_OK;
}

/**
 * @brief 获取电机当前转向
 * @param motor 电机指针
 * @return 当前转向
 */
direction_t Motor_GetDirection(Motor_t* motor)
{
    if (motor == NULL)
    {
        return STOP;
    }
    return motor->direction;
}

/**
 * @brief 获取电机当前占空比
 * @param motor 电机指针
 * @return 占空比值 (0-1000)
 */
uint16_t Motor_GetDutyCycle(Motor_t* motor)
{
    if (motor == NULL)
    {
        return 0;
    }
    return motor->pwm_duty;
}

/**
 * @brief 检查电机是否正在运行
 * @param motor 电机指针
 * @return 1-运行中，0-停止
 */
uint8_t Motor_IsRunning(Motor_t* motor)
{
    if (motor == NULL)
    {
        return 0;
    }
    return motor->run_state;
}

/* ==================== 电机 PID 转速控制实现 ==================== */

/**
 * @brief 初始化电机速度 PID 控制器
 * @param motor 电机指针
 * @param Kp 比例增益
 * @param Ki 积分增益
 * @param Kd 微分增益
 */
void Motor_SpeedPID_Init(Motor_t* motor, float Kp, float Ki, float Kd)
{
    if (motor == NULL)
    {
        return;
    }

    // 初始化 PID 控制器，输出范围 0-1000 (对应 PWM 占空比)
    PID_Init(&motor->pid, Kp, Ki, Kd, 1000.0f, 0.0f);

    // 设置目标速度为 0
    PID_SetTarget(&motor->pid, 0.0f);
}

/**
 * @brief 电机速度闭环控制
 * @param motor 电机指针
 * @param target_rpm 目标转速 (RPM)
 * @return HAL 状态
 */
HAL_StatusTypeDef Motor_SpeedControl(Motor_t* motor, float target_rpm)
{
    if (motor == NULL)
    {
        return HAL_ERROR;
    }

    // 如果电机未运行，先启动电机
    if (!Motor_IsRunning(motor))
    {
        // 确定启动方向
        direction_t start_dir = (target_rpm >= 0) ? motor->forward_direction : (motor->forward_direction == CW ? CCW : CW);

        // 以较小的占空比启动电机
        if (Motor_Start(motor, start_dir, 70) != HAL_OK)
        {
            return HAL_ERROR;
        }
    }

    // 更新反馈速度 (使用绝对值)
    float feedback_rpm = (motor->direction == motor->forward_direction) ? motor->rpm : -motor->rpm;
    PID_SetFeedback(&motor->pid, feedback_rpm);

    // 设置目标速度 (使用绝对值)
    PID_SetTarget(&motor->pid, fabsf(target_rpm));
    
    // 计算 PID 输出
    float pid_output = PID_Calculate(&motor->pid);
    
    // 将 PID 输出转换为 PWM 占空比并应用
    uint16_t pwm_duty = (uint16_t)pid_output;
    
    // 确保占空比在有效范围内
    if (pwm_duty < 50) pwm_duty = 50;   // 最小启动占空比
    if (pwm_duty > 1000) pwm_duty = 1000;
    
    // 更新 PWM 占空比
    Motor_SetDutyCycle(motor, pwm_duty);
    
    return HAL_OK;
}

/**
 * @brief 设置速度环 PID 参数
 * @param motor 电机指针
 * @param Kp 比例增益
 * @param Ki 积分增益
 * @param Kd 微分增益
 */
void Motor_SpeedPID_SetParams(Motor_t* motor, float Kp, float Ki, float Kd)
{
    if (motor == NULL)
    {
        return;
    }
    
    PID_SetParams(&motor->pid, Kp, Ki, Kd);
}

/**
 * @brief 获取速度环当前误差
 * @param motor 电机指针
 * @return 速度误差 (RPM)
 */
float Motor_SpeedPID_GetError(Motor_t* motor)
{
    if (motor == NULL)
    {
        return 0.0f;
    }
    
    return PID_GetError(&motor->pid);
}

/**
 * @brief 启用/禁用速度环 PID 控制
 * @param motor 电机指针
 * @param enable 1-启用，0-禁用
 */
void Motor_SpeedPID_Enable(Motor_t* motor, uint8_t enable)
{
    if (motor == NULL)
    {
        return;
    }
    
    PID_SetEnable(&motor->pid, enable);
}



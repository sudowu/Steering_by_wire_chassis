//
// Created by yang on 2026/3/8.
//
#include "FreeRTOS.h"
#include "main.h"
#include "TaskMotor.h"

#include <stdio.h>
#include <string.h>

#include "adc.h"
#include "Motor.h"
#include "queue.h"
#include "task.h"
#include "TaskSeriel.h"
#include "tim.h"
#include "timers.h"
#include "usart.h"
#include "cmsis_os2.h"


TaskHandle_t g_TaskMotor;
SemaphoreHandle_t xSemaphoreADCReady;
void vTaskMotor(void* parameter)
{
    motor_init();

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Buffer, ADC_BUFFER_SIZE);
    vTaskDelay(1000); // 等待 ADC 稳定
    Motor_OffsetCalibrate(&g_Motor1);

    hall_start(&g_Motor1);
    hall_start(&g_Motor2);
    motor_start(&g_Motor1, 1, 60);


    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(200)); // 每10ms执行一次，控制频率约为100Hz
    }
}


void vTask_ADC_Sample(void* parameter)
{
    xSemaphoreADCReady = xSemaphoreCreateBinary();
    while (1)
    {
        xSemaphoreTake(xSemaphoreADCReady, portMAX_DELAY);
         if (g_Motor1.adc_current.offset_calibrated == 0)
         {
             continue; // 偏置未校准，跳过当前值计算
         }
        if (Motor_ReadAdcCurrent(&g_Motor1) == HAL_OK)
        {
            float total_current = Motor_GetTotalCurrent(&g_Motor1);
            uint8_t* info_current = pvPortMalloc(100);
            sprintf((char*)info_current, "current:%f,%f,%f, %f\n",
                    g_Motor1.adc_current.adc_current_u,
                    g_Motor1.adc_current.adc_current_v,
                    g_Motor1.adc_current.adc_current_w,
                    total_current);
            if (xQueueSendToFront(xQueueSeriel, &info_current, 0) != pdPASS)
            {
                vPortFree(info_current); // 发送失败，释放内存
            }
        }

    }
}

void vTask_Data_Send(void* parameter)
{
    int i = 0;
    int j = 0;
    float rpm_ave = 0;
    float rpm_buff[10] = {0};
    while (1)
    {
        rpm_buff[i++] = ((float)(g_Motor1.commutating_counter) * 50);
        i %= 10;
        for (j = 0; j < 10; j++)
        {
            rpm_ave += rpm_buff[j];
        }
        g_Motor1.rpm = (rpm_ave / 10);
        rpm_ave = 0;
        // uint8_t* info_rpm = pvPortMalloc(100);
        // sprintf((char*)info_rpm, "rpm:%f,%ld,%d,", g_Motor1.rpm, g_Motor1.commutating_counter,
        //         g_Motor1.pwm_duty);
        // g_Motor1.commutating_counter = 0;
        // if (xQueueSendToFront(xQueueSeriel, &info_rpm, 0) != pdPASS)
        // {
        //     vPortFree(info_rpm); // 发送失败，释放内存
        // }

        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

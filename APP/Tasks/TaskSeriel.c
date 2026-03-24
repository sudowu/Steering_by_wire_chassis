//
// Created by yang on 2026/3/18.
//

#include "TaskSeriel.h"

#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "main.h"
#include "Motor.h"
#include "usart.h"

QueueHandle_t xQueueSeriel;
SemaphoreHandle_t xSemaphoreSeriel;
TaskHandle_t g_TaskSeriel;
void vTaskSeriel(void* argument)
{
    xSemaphoreSeriel = xSemaphoreCreateBinary();
    vQueueAddToRegistry(xSemaphoreSeriel, "Serial Semaphore");
    xQueueSeriel = xQueueCreate(100, sizeof(uint8_t*));
    vQueueAddToRegistry(xQueueSeriel, "Serial Queue");
    if (xQueueSeriel == NULL)
    {
        while (1);
    }
    while (1)
    {
        uint8_t* dataToSend;
        if (xQueueReceive(xQueueSeriel, &dataToSend, portMAX_DELAY) == pdPASS)
        {
            // 发送数据成功
            if (HAL_UART_Transmit_DMA(&huart1, dataToSend, strlen((char*)dataToSend)) == HAL_OK)
            {
                // 数据发送成功
                xSemaphoreTake(xSemaphoreSeriel, portMAX_DELAY); // 等待发送完成信号量
                if (g_Motor1.adc_current.offset_calibrated == 0)
                {
                    continue; // 偏置未校准，跳过当前值计算
                }
            }
            else
            {

            }
        }
        vPortFree(dataToSend); // 释放动态分配的内存

    }
}

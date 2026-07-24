/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "queue.h"
#include "TaskCanInfo.h"
#include "TaskMotor.h"
#include "TaskSeriel.h"
#include "TaskSbwControl.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
TaskHandle_t g_testTask;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void vTaskTest(void *argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  taskENTER_CRITICAL();

  canTxQueue = xQueueCreate(10, sizeof(CAN_Message_t*));
  vQueueAddToRegistry(canTxQueue, "CAN Tx Queue");
  canRxQueue = xQueueCreate(10, sizeof(CAN_Message_t));
  vQueueAddToRegistry(canRxQueue, "CAN Rx Queue");

  // xTaskCreate(vTaskTest,"test Task", 128, NULL, osPriorityNormal, &g_testTask);
  xTaskCreate(vTask_ADC_Sample,"ADC Sample Task", 512, NULL, osPriorityNormal, NULL);
  xTaskCreate(vTaskCanInfo,"CAN Info Task", 128, NULL, osPriorityNormal, &g_TaskCanInfo);
  xTaskCreate(vTaskSeriel,"Serial Task", 128, NULL, osPriorityNormal, &g_TaskSeriel);
  xTaskCreate(vTaskMotor,"Motor Task", 512, NULL, osPriorityNormal, &g_TaskMotor);
  xTaskCreate(vTask_Data_Send, "Data Send Task", 256, NULL, osPriorityNormal, NULL);
  // xTaskCreate(vTaskSbwControl, "SbW Control Task", 256, NULL, osPriorityNormal+2, NULL);
  taskEXIT_CRITICAL();
  /* Infinite loop */
  vTaskDelete(NULL);
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void vTaskTest(void *argument)
{
  xQueueHandle queue = xQueueCreate(10, sizeof(int));
  vQueueAddToRegistry(queue, "TestQueue");
  for(;;)
  {
    CAN_Message_t* message = pvPortMalloc(sizeof(CAN_Message_t));
    message->StdId = 0xFF;
    message->Len = 8;
    for (int i = 0; i < 8; i++) {
        message->Data[i] = i;
    }
    xQueueSend(canTxQueue, &message, portMAX_DELAY);
    // HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, HAL_GPIO_ReadPin(PM1_HALLU_GPIO_Port, PM1_HALLU_Pin));

    vTaskDelay(500);
  }
}
/* USER CODE END Application */


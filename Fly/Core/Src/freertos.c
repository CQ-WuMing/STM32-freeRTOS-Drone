/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "nrf24L01.h"
#include"Fly_Data.h"
#include"pid.h"
#include"motor.h"
#include"mpu6050.h"
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

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId Task_2msHandle;
osThreadId Task_4msHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask_2ms(void const * argument);
void StartTask_4ms(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Task_2ms */
  osThreadDef(Task_2ms, StartTask_2ms, osPriorityHigh, 0, 128);
  Task_2msHandle = osThreadCreate(osThread(Task_2ms), NULL);

  /* definition and creation of Task_4ms */
  osThreadDef(Task_4ms, StartTask_4ms, osPriorityIdle, 0, 128);
  Task_4msHandle = osThreadCreate(osThread(Task_4ms), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask_2ms */
/**
* @brief Function implementing the Task_2ms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_2ms */
void StartTask_2ms(void const * argument)
{
  /* USER CODE BEGIN StartTask_2ms */
	TickType_t pxPreviousWakeTime;
	pxPreviousWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  MPU6050_Read_Result();//获取数据，计算欧拉角
	  PID_Control();
	  Motor_Control();//电机控制
	  vTaskDelayUntil(&pxPreviousWakeTime, 2);
  }
  /* USER CODE END StartTask_2ms */
}

/* USER CODE BEGIN Header_StartTask_4ms */
/**
* @brief Function implementing the Task_4ms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask_4ms */
void StartTask_4ms(void const * argument)
{
  /* USER CODE BEGIN StartTask_4ms */
	TickType_t pxPreviousWakeTime;
	pxPreviousWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  NRF24L01_RxPacket(RX_BUFF);//接收数据
	  DataRead(RX_BUFF,TX_PLOAD_WIDTH);//解析数据
	  vTaskDelayUntil(&pxPreviousWakeTime, 4);
  }
  /* USER CODE END StartTask_4ms */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


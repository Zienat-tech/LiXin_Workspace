/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "bsp_key.h"
#include "bsp_led.h"
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

//QueueHandle_t g_xQueue_BlockedReceive;  //定义队列

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

//void vTaskSender1(void *argument);
//void vTaskSender2(void *argument);
//void vTaskReceiver(void *argument);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
//	g_xQueue_BlockedReceive = xQueueCreate(5, sizeof(int32_t));  //创建队列
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	//判断队列创建是否成功，只有成功创建队列，才会创建对该队列操作的任务
//	if(g_xQueue_BlockedReceive != NULL)
//	{
//		//发送任务
//		xTaskCreate(vTaskSender1, "Sender1", (128*4), NULL, osPriorityNormal, NULL);
//		xTaskCreate(vTaskSender2, "Sender2", (128*4), NULL, osPriorityNormal, NULL);
//		//接收任务
//		xTaskCreate(vTaskReceiver, "Receiver", (128*4), NULL, (osPriorityNormal+1), NULL);
//	}
//  xTaskCreate( key_task_func, "key_task", (128*4), NULL,
//		      (osPriorityNormal+1), &key_task_handle );
//  xTaskCreate( led_task_func, "led_task", (128*4), NULL,
//  		      (osPriorityNormal-1), &led_task_handle );
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
  /* Infinite loop */
//	key_event_t key_value = KEY_RELEASE;
//	led_event_t led_value = LED_NONE;
  for(;;)
  {
//	  if(key_queue != 0)
//	  {
//		  if(pdPASS == xQueueReceive(key_queue, &key_value, portMAX_DELAY))
//		  {
//			  switch(key_value)
//			  {
//			  	  case KEY_RELEASE:
//			  		  led_value = LED_NONE;
//			  		  break;
//			  	  case KEY_SHORT_PRESS:
//			  		  led_value = LED_ON;
//			  		  break;
//			  	  case KEY_DOUBLE_PRESS:
//			  		  led_value = LED_TOGGLE;
//			  		  break;
//			  	  case KEY_LONG_PRESS:
//			  		  led_value = LED_BLINK_1;
//			  		  break;
//			  	  case KEY_LONG_PRESS_3:
//					  led_value = LED_BLINK_3;
//					  break;
//			  	  case KEY_LONG_PRESS_5:
//					  led_value = LED_BLINK_10;
//					  break;
//			  	  default:
//			  		  break;
//			  }
//			  printf("key_value is [%d], now = [%ld]\r\n", key_value, xTaskGetTickCount());
//
//			  if(pdPASS == xQueueSend(led_queue, &led_value, 0))
//			  {
//				  printf("led_value send successfully\r\n");
//			  }
//		  }
//	  }

//	  printf("Hello SEGGER_RTT, @ %ld\r\n", xTaskGetTickCount());
	  elog_a("ASSERT",  "HEllo World");
	  elog_e("ERROR",   "HEllo World");
	  elog_w("WARNING", "HEllo World");
	  elog_i("INFO",    "HEllo World");
	  elog_d("DEBUG",   "HEllo World");
	  elog_v("VERBOSE", "HEllo World");

	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	  vTaskDelay(1000);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

//void vTaskSender1(void *argument)
//{
//	int32_t l_ValueToSend = 0;
//	BaseType_t l_xStatus;
//
//	for(;;)
//	{
//		//vTaskDelay(100);
//		l_ValueToSend = 100;
//		//将100写入队列
//		l_xStatus = xQueueSend(g_xQueue_BlockedReceive, &l_ValueToSend, 0);
//		if(l_xStatus != pdPASS)
//		{
//			//队列已满，无法继续写入
//			printf("vTaskSender1 can not send to the queue \r\n");
//			//如何让程序执行到这里？用 vTaskDelay 减慢接收任务接收数据的速度
//		}
//	}
//}
//void vTaskSender2(void *argument)
//{
//	int32_t l_ValueToSend = 0;
//	BaseType_t l_xStatus;
//
//	for(;;)
//	{
//		//vTaskDelay(200);
//		l_ValueToSend = 200;
//		//将200写入队列
//		l_xStatus = xQueueSend(g_xQueue_BlockedReceive, &l_ValueToSend, 0);
//		if(l_xStatus != pdPASS)
//		{
//			printf("vTaskSender2 can not send to the queue \r\n");
//		}
//	}
//}
//void vTaskReceiver(void *argument)
//{
//	int32_t l_ValueOfReceived = 0;
//	BaseType_t l_xStatus;
//	const TickType_t l_xTicksToWait = pdMS_TO_TICKS(100);
//
//	for(;;)
//	{
//		if(uxQueueMessagesWaiting(g_xQueue_BlockedReceive) != 0) //判断队列中是否有数据
//		{
//			printf("Queue is not empty.\r\n");  //这个永远不会被执行，思考为什么？
//			//如何让程序执行到这里？只有当读取一次队列后，队列中还存在数据，才会执行到这里
//		}
//		//读取队列 g_xQueue_BlockedReceive 中的数据，保存在 l_ValueOfReceived 中
//		l_xStatus = xQueueReceive(g_xQueue_BlockedReceive,
//								  &l_ValueOfReceived,
//								  l_xTicksToWait);
//		if(l_xStatus == pdPASS)
//		{
//			//打印从队列中接收的数据
//			printf("Received:%ld.\r\n", l_ValueOfReceived);
//		}
//		else
//		{
//			//这个永远不会被执行，思考为什么？
//			printf("Could not receive from the queue.\r\n");
//			//如何让程序执行到这里？比如用 vTaskDelay
//			//让写入队列任务主动挂起超过 xQueueReceive 的等待时间
//		}
//		//vTaskDelay(500);
//	}
//}

/* USER CODE END Application */


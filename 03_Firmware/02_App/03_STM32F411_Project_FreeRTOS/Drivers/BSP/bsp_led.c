/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_led.c
 *
 * @par dependencies
 * - bsp_led.h
 *
 * @author Zienat
 *
 * @brief Provide the HAL APIs of KEY and corresponding opetions.
 *
 * Processing flow:
 *
 * call directly.
 *
 * @version V1.0 2025-02-04
 *
 * @note 1 tab == 4 spaces!
 *
 *****************************************************************************/
//******************************** Includes *********************************//
#include "bsp_led.h"

//******************************** Defines **********************************//
TaskHandle_t     led_task_handle;
QueueHandle_t    led_queue;
static uint8_t   g_led_blink_cnt = 0;

//******************************** Declaring ********************************//


//******************************** Function  ********************************//

/**
 * @brief This function is used to create an LED event queue and continuously
 *        receive LED events from the queue to control the state of the LED.
 *
 * Steps:
 *  1. Create LED event queue.
 *  2. Check if the queue is created successfully.
 *  3. Receive LED event from the queue.
 *  4. Control the LED state according to the received event.
 *
 * @param[in] argument: Not used.
 *
 * @return led_handler_status_t : Status of the function.
 *
 * */
void led_task_func(void * argument)
{
	//1. Create LED event queue
	led_event_t led_event = LED_NONE;
	led_queue = xQueueCreate(5, sizeof(led_event_t));
	//2. Check if the queue is created successfully
	if(NULL == led_queue)
	{
		printf("led_queue create failure\r\n");
	}
	else
	{
		printf("led_queue create successfully\r\n");
	}
	while(1)
	{
		//3. Receive LED event from the queue
		if(pdPASS == xQueueReceive(led_queue, &led_event, portMAX_DELAY))
		{
			//4. Control the LED state according to the received event
			printf("led_event is [%d]\r\n", led_event);
			led_control(led_event);
		}
	}
}

/**
 * @brief This function controls the on/off/toggle/blink state of the LED
 *        based on the incoming LED event parameter.
 *
 * Steps:
 *  1. Execute the corresponding operation based on the incoming LED event
 *
 * @param[in] led_event: The LED event to be executed:
 * 					     LED_NONE, LED_ON ,LED_OFF, LED_BLINK3
 *
 * @return None.
 *
 * */
void led_control(led_event_t led_event)
{
	//Execute the corresponding operation based on the incoming LED event
	switch(led_event)
	{
		case LED_NONE:
			break;
		case LED_ON:
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			break;
		case LED_OFF:
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			break;
		case LED_TOGGLE:
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			break;
//		case LED_BLINK_3:
//			for(uint8_t i = 0; i < 6; i++)
//			{
//				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//				vTaskDelay(200);
//			}
			break;
		case LED_BLINK_1:
			g_led_blink_cnt = 2;
			break;
		case LED_BLINK_3:
			g_led_blink_cnt = 6;
			break;
		case LED_BLINK_10:
			g_led_blink_cnt = 20;
			break;
		default:
			break;
	}
}

/**
 * @brief Timer 2 interrupt callback function for controlling LED blinking.
 *
 * Steps:
 *  1. If the global variable g_led_blink_cnt is greater than 0, it calls the
 *     led_control function to toggle the LED state and
 *     decrements g_led_blink_cnt by 1.
 *
 * @param[in] None.
 *
 * @return None.
 *
 * */
void led_tim2_callback(void)
{
	if(g_led_blink_cnt > 0)
	{
		led_control(LED_TOGGLE);
		g_led_blink_cnt--;
	}
}

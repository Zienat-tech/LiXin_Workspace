/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_led.c
 *
 * @par dependencies
 * - stdio.h
 * - stdint.h
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
TaskHandle_t  led_task_handle;
QueueHandle_t led_queue;


//******************************** Declaring ********************************//


/**
 * @brief Instantiates the bsp_led_handler_t target.
 *
 * Steps:
 *  1. Adds Core interfaces into bsp_led_driver instance target.
 *  2. Adds OS interfaces into bsp_led_driver instance target.
 *  3. Adds timebase interfaces into bsp_led_driver instance target.
 *
 * @param[in] self        : Pointer to the target of handler.
 * @param[in] os_delay    : Pointer to the os_delay_interface.
 * @param[in] os_queue    : Pointer to the os_queue_interface.
 * @param[in] os_critical : Pointer to the os_critical_interface.
 * @param[in] os_thread   : Pointer to the os_thread_interface.
 * @param[in] time_base   : Pointer to the time_base_interface.
 *
 * @return led_handler_status_t : Status of the function.
 *
 * */
void led_task_func(void * argument)
{
	led_event_t led_event = LED_OFF;
	led_queue = xQueueCreate(5, sizeof(led_event_t));
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
		if(pdPASS == xQueueReceive(led_queue, &led_event, 100))
		{
			led_control(led_event);
			printf("led_event is [%d]\r\n", led_event);
		}
	}

}

/**
 * @brief Instantiates the bsp_led_handler_t target.
 *
 * Steps:
 *  1. Adds Core interfaces into bsp_led_driver instance target.
 *  2. Adds OS interfaces into bsp_led_driver instance target.
 *  3. Adds timebase interfaces into bsp_led_driver instance target.
 *
 * @param[in] self        : Pointer to the target of handler.
 * @param[in] os_delay    : Pointer to the os_delay_interface.
 * @param[in] os_queue    : Pointer to the os_queue_interface.
 * @param[in] os_critical : Pointer to the os_critical_interface.
 * @param[in] os_thread   : Pointer to the os_thread_interface.
 * @param[in] time_base   : Pointer to the time_base_interface.
 *
 * @return led_handler_status_t : Status of the function.
 *
 * */
void led_control(led_event_t led_event)
{
	switch(led_event)
	{
		case LED_ON:
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			break;
		case LED_OFF:
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			break;
		case LED_TOGGLE:
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		default:
//			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			break;
	}
}









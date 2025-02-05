/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_key.c
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

#include "bsp_key.h"


//******************************** Defines **********************************//
TaskHandle_t  key_task_handle;
QueueHandle_t key_queue;


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
void key_task_func(void * argument)
{
	key_event_t key_even = KEY_RELEASED;
	key_queue            = xQueueCreate(5, sizeof(key_event_t));
	if(NULL == key_queue)
	{
		printf("key_queue create failure\r\n");
	}
	else
	{
		printf("key_queue create successfully\r\n");
	}
	while(1)
	{
		key_even = key_scan();
		if(KEY_PRESSED == key_even)
		{
			printf("KEY_PRESSED\r\n");
			if(pdPASS == xQueueSend(key_queue, &key_even, 0))
			{
				printf("key_even send to key_queue\r\n");
			}
			else
			{
				printf("key_queue is full\r\n");
			}
		}
		else if(KEY_RELEASED == key_even)
		{
			printf("KEY_RELEASED\r\n");
		}
		vTaskDelay(100);
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
key_event_t key_scan(void)
{
	static BaseType_t   debouncing            = pdFALSE;
	static TickType_t   debouncing_start_cnt  = 0;
	TickType_t          now;
	key_event_t         current_key_event     = KEY_RELEASED;

	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin))
	{
		if(!debouncing)
		{
			debouncing = pdTRUE;
			debouncing_start_cnt = xTaskGetTickCount();
		}
		else
		{
			now = xTaskGetTickCount();
			if((now - debouncing_start_cnt) >= KEY_DEBOUNCE_DELAY)
			{
				current_key_event = KEY_PRESSED;
				debouncing = pdFALSE;
			}
		}
	}
	else
	{
		if(debouncing)
		{
			debouncing = pdFALSE;
		}
		current_key_event = KEY_RELEASED;
	}
//	if(current_key_event != last_key_event)
//	{
//		last_key_event = current_key_event;
//	}
	return current_key_event;
}

































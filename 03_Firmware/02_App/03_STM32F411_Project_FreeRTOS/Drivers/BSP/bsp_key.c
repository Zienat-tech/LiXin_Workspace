/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_key.c
 *
 * @par dependencies
 * - bsp_key.h
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


//******************************** Function  ********************************//

/**
 * @brief This function is used to create a key event queue and continuously
 *        scan the key status, sending key events to the queue.
 *
 * Steps:
 *  1. Create key_queue and check whether the queue is created successfully.
 *  2. Get key status.
 *  3. Select whether to update the queue based on the key status.
 *
 * @param[in] argument: Not used.
 *
 * @return None.
 *
 * */
void key_task_func(void * argument)
{
	//1. Create key_queue
	key_event_t key_even = KEY_RELEASED;
	key_queue            = xQueueCreate(5, sizeof(key_event_t));
	//2. check whether the queue is created successfully
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
		//3. Get key status
		key_even = key_scan();
		//4. Select whether to update the queue based on the key status
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
 * @brief This function is used to scan the key status and
 *        perform debounce processing.
 *
 * Steps:
 *  1. Read the key status.
 *  2. If the key is pressed, start debounce.
 *  3. Debounce time is up, confirm the key is pressed.
 *  4. If the key is released, end debounce.
 *
 * @param[in] None.
 *
 * @return key_event_t : Returns the key event, KEY_PRESSED or KEY_RELEASED.
 *
 * */
key_event_t key_scan(void)
{
	//1. Define variables
	static BaseType_t   debouncing            = pdFALSE;
	static TickType_t   debouncing_start_cnt  = 0;
	TickType_t          now;
	key_event_t         current_key_event     = KEY_RELEASED;
	//2. Read the key status
	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin))
	{
		//3. If the key is pressed, start debounce
		if(!debouncing)
		{
			debouncing = pdTRUE;
			debouncing_start_cnt = xTaskGetTickCount();
		}
		else
		{
			//4. Debounce time is up, confirm the key is pressed
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
		//5. If the key is released, end debounce
		if(debouncing)
		{
			debouncing = pdFALSE;
		}
		current_key_event = KEY_RELEASED;
	}
	return current_key_event;
}

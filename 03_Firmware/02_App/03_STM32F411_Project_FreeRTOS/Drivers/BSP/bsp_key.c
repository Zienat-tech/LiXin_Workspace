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
	key_event_t key_even = KEY_RELEASE;
//	key_queue            = xQueueCreate(5, sizeof(key_event_t));
	//2. check whether the queue is created successfully
//	if(NULL == key_queue)
//	{
//		printf("key_queue create failure\r\n");
//	}
//	else
//	{
//		printf("key_queue create successfully\r\n");
//	}
	while(1)
	{
		//3. Get key status
		key_even = key_scan();
		//4. Select whether to update the queue based on the key status
		switch(key_even)
		{
			case KEY_RELEASE:
				printf("KEY_RELEASE\r\n");
				break;
			case KEY_SHORT_PRESS:
				printf("KEY_SHORT_PRESS111111111\r\n");
				break;
			case KEY_DOUBLE_PRESS:
				printf("KEY_DOUBLE_PRESS2222222222222222\r\n");
				break;
			case KEY_LONG_PRESS:
				printf("KEY_LONG_PRESS\r\n");
				break;
			default:
				break;
		}

//		if(KEY_PRESSED == key_even)
//		{
//			printf("KEY_PRESSED\r\n");
//			if(pdPASS == xQueueSend(key_queue, &key_even, 0))
//			{
//				printf("key_even send to key_queue\r\n");
//			}
//			else
//			{
//				printf("key_queue is full\r\n");
//			}
//		}
//		else if(KEY_RELEASED == key_even)
//		{
//			printf("KEY_RELEASED\r\n");
//		}
		vTaskDelay(10);
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
//key_event_t key_scan(void)
//{
//	//1. Define variables
//	static BaseType_t   debouncing            = pdFALSE;
//	static TickType_t   debouncing_start_cnt  = 0;
//	TickType_t          now;
//	key_event_t         current_key_event     = KEY_RELEASED;
//	//2. Read the key status
//	if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin))
//	{
//		//3. If the key is pressed, start debounce
//		if(!debouncing)
//		{
//			debouncing = pdTRUE;
//			debouncing_start_cnt = xTaskGetTickCount();
//		}
//		else
//		{
//			//4. Debounce time is up, confirm the key is pressed
//			now = xTaskGetTickCount();
//			if((now - debouncing_start_cnt) >= KEY_DEBOUNCE_DELAY)
//			{
//				current_key_event = KEY_PRESSED;
//				debouncing = pdFALSE;
//			}
//		}
//	}
//	else
//	{
//		//5. If the key is released, end debounce
//		if(debouncing)
//		{
//			debouncing = pdFALSE;
//		}
//		current_key_event = KEY_RELEASED;
//	}
//	return current_key_event;
//}

key_event_t key_scan(void)
{
	static key_state_machine_t state            = KEY_RELEASING;
	static TickType_t          state_entry_time = 0;
	static uint8_t             key_press_cnt    = 0;
	key_event_t                key_event        = KEY_RELEASE;
	const GPIO_PinState pin_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
	const TickType_t    current_time = xTaskGetTickCount();
	switch(state)
	{
		case KEY_RELEASING:
			if(GPIO_PIN_RESET == pin_state)
			{
				if(GET_TICK_DIFF(state_entry_time) >= pdMS_TO_TICKS(20))
				{
					state = KEY_PRESSING;
					state_entry_time = current_time;
				}
			}
			else
			{
				state_entry_time = current_time;
			}
			break;
		case KEY_PRESSING:
			const TickType_t press_duration = GET_TICK_DIFF(state_entry_time);
			if(GPIO_PIN_SET == pin_state)
			{
				if(press_duration >= KEY_SHORT_PRESS_DELAY)
				{
					key_press_cnt++;
					state = KEY_WAIT_DOUBLE;
					state_entry_time = current_time;
				}
				else if((press_duration < KEY_SHORT_PRESS_DELAY) && (key_press_cnt == 2))
				{
					key_press_cnt = 0;
					key_event = KEY_SHORT_PRESS;
					state = KEY_RELEASING;
					state_entry_time = current_time;
				}
				else
				{
					state = KEY_RELEASING;
					state_entry_time = current_time;
				}
			}
			else if(GET_TICK_DIFF(state_entry_time) >= KEY_LONG_PRESS_DELAY)
			{
				key_event = KEY_LONG_PRESS;
				state = KEY_LONG_PRESSING;
			}
			break;
		case KEY_WAIT_DOUBLE:
			if(GET_TICK_DIFF(state_entry_time) < KEY_DOUBLE_PRESS_DELAY)
			{
				if(key_press_cnt > 2)
				{
					key_press_cnt = 0;
					key_event = KEY_DOUBLE_PRESS;
					state = KEY_RELEASING;
				}
				if(GPIO_PIN_RESET == pin_state)
				{
					if(GET_TICK_DIFF(state_entry_time) >= pdMS_TO_TICKS(20))
					{
						key_press_cnt++;
						state = KEY_PRESSING;
						state_entry_time = current_time;
					}
				}
			}
			else if(GET_TICK_DIFF(state_entry_time) > KEY_DOUBLE_PRESS_DELAY)
			{
				key_press_cnt = 0;
				key_event = KEY_SHORT_PRESS;
				state = KEY_RELEASING;
				state_entry_time = current_time;
			}
			else if(GET_TICK_DIFF(state_entry_time) >= KEY_LONG_PRESS_DELAY)
			{
				key_event = KEY_LONG_PRESS;
				state = KEY_LONG_PRESSING;
				state_entry_time = current_time;
			}
			break;
		case KEY_LONG_PRESSING:
			key_event = KEY_LONG_PRESS;
			state = KEY_LONG_PRESSING;
			if(GPIO_PIN_SET == pin_state)
			{
				state = KEY_RELEASING;
			}
			break;
		default:
			break;
	}
	return key_event;
}




























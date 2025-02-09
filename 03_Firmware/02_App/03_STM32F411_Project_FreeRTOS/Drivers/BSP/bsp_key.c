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
 * @brief Task function to handle key events.
 *
 * Steps:
 *  1. Create key_queue and check whether the queue is created successfully.
 *  2. Get key_even and check whether the key_even changes.
 *  3. If the key_even changes, the key_even is sent to the key queue and
 *     the key_event_last is updated.
 *
 * @param[in] argument: Not used.
 *
 * @return None.
 *
 * */
void key_task_func(void * argument)
{
	key_event_t        key_even       = KEY_RELEASE;
	static key_event_t key_event_last = KEY_RELEASE;

	// 1. Create key_queue
	key_queue = xQueueCreate(5, sizeof(key_event_t));

	// 2. check whether the queue is created successfully
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
		// 3. Get key status
		key_even = key_scan();

		// 4. Check whether the key status changes
		if(key_event_last != key_even)
		{
			// 4.1. Whether the key queue is successfully sent
			if(pdPASS == xQueueSend(key_queue, &key_even, 0))
			{
				// 4.1.1 Print the key status
				switch(key_even)
				{
					case KEY_RELEASE:
						printf("KEY_RELEASE, now = [%ld]\r\n", \
								                          xTaskGetTickCount());
						break;
					case KEY_SHORT_PRESS:
						printf("KEY_SHORT_PRESS, now = [%ld]\r\n", \
								                          xTaskGetTickCount());
						break;
					case KEY_DOUBLE_PRESS:
						printf("KEY_DOUBLE_PRESS, now = [%ld]\r\n", \
								                          xTaskGetTickCount());
						break;
					case KEY_LONG_PRESS:
						printf("KEY_LONG_PRESS, now = [%ld]\r\n", \
								                          xTaskGetTickCount());
						break;
					default:
						break;
				}
			}
			// 4.2 Failed to send the key queue
			else
			{
				printf("Failed to send the key_queue, now = [%ld]\r\n", \
						                                  xTaskGetTickCount());
			}
			// 4.3 Update key status
			key_event_last = key_even;
		}
		vTaskDelay(10);
	}
}

/**
 * @brief This function is used to scan the key state and return the
 *        corresponding key event based on the duration and number
 *        of key presses.
 *
 * Steps:
 *  1. Read the state of the key pin.
 *  2. Get the current system time and the duration of state entry.
 *  3. Processes the state machine according to the current state.
 *
 * @param[in] None.
 *
 * @return key_event_t : Returns the key event:
 *                       KEY_RELEASE      | KEY_SHORT_PRESS
 *                       KEY_DOUBLE_PRESS | KEY_LONG_PRESS.
 *
 * */
key_event_t key_scan(void)
{
	/* Variables */
	static key_state_machine_t state            = KEY_RELEASING;
	static TickType_t          state_entry_time = 0;
	static uint8_t             key_press_cnt    = 0;
	key_event_t                key_event        = KEY_RELEASE;

	//1. Read the state of the key pin.
	const GPIO_PinState pin_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);

	//2. Get the current system time.
	const TickType_t    current_time = xTaskGetTickCount();

	//3. Get the duration of state entry.
	const TickType_t    state_entry_duration = GET_TICK_DIFF(state_entry_time);

	//4. Processes the state machine according to the current state.
	switch(state)
	{
		case KEY_RELEASING:
			//4.1.1 Determine whether the key is pressed
			if(GPIO_PIN_RESET == pin_state)
			{
				//4.1.2 Determine whether the shake off time is exceeded
				if(state_entry_duration >= pdMS_TO_TICKS(20))
				{
					state = KEY_PRESSING;
					state_entry_time = current_time;
				}
			}
			//4.1.3 Default hold KEY_PRESSING state
			else
			{
				key_event = KEY_RELEASE;
				state = KEY_PRESSING;
				state_entry_time = current_time;
			}
			break;

			case KEY_PRESSING:
				//4.2.1 Determine whether the key is released
				if(GPIO_PIN_SET == pin_state)
				{
					//4.2.1.1 Determine whether the press event exceeds
					//        the KEY_SHORT_PRESS_DELAY
					if(state_entry_duration < KEY_SHORT_PRESS_DELAY)
					{
						// Determine whether it is a second press
						if(key_press_cnt >= 2)
						{
							key_press_cnt = 0;
							key_event = KEY_SHORT_PRESS;
						}
						state = KEY_RELEASING;
						state_entry_time = current_time;
					}
					// The press time is longer than the KEY_SHORT_PRESS_DELAY
					else if(state_entry_duration >= KEY_SHORT_PRESS_DELAY)
					{
						key_press_cnt++;
						state = KEY_WAIT_DOUBLE;
						state_entry_time = current_time;
					}
				}
				//4.2.2 If the key is not released, determine whether the press
				//      time is longer than the KEY_LONG_PRESS_DELAY
				else if(state_entry_duration >= KEY_LONG_PRESS_DELAY)
				{
					key_press_cnt = 0;  // Avoid when the second LONG_PRESS
					                    // triggering a SHORT_PRESS
					key_event = KEY_LONG_PRESS;
					state = KEY_LONG_PRESSING;
					state_entry_time = current_time;
				}
				break;

		case KEY_WAIT_DOUBLE:
			// 4.3.1 Determine whether the key was pressed within the
			//       KEY_DOUBLE_PRESS_DELAY
			if(state_entry_duration <= KEY_DOUBLE_PRESS_DELAY)
			{
				if(key_press_cnt > 2)
				{
					key_press_cnt = 0;
					key_event = KEY_DOUBLE_PRESS;
					state = KEY_RELEASING;
					state_entry_time = current_time;
				}
				else if(GPIO_PIN_RESET == pin_state)
				{
					if(state_entry_duration >= pdMS_TO_TICKS(20))
					{
						key_press_cnt++;
						state = KEY_PRESSING;
						state_entry_time = current_time;
					}
				}
			}
			else if(state_entry_duration > KEY_DOUBLE_PRESS_DELAY)
			{
				key_press_cnt = 0;
				key_event = KEY_SHORT_PRESS;
				state = KEY_RELEASING;
				state_entry_time = current_time;
			}
			break;

		case KEY_LONG_PRESSING:
			if(GPIO_PIN_SET == pin_state)
			{
				key_event = KEY_RELEASE;
				state = KEY_RELEASING;
				state_entry_time = current_time;
			}
			else
			{
				key_event = KEY_LONG_PRESS;
				state = KEY_LONG_PRESSING;
			}
			break;
		default:
			break;
	}
	return key_event;
}

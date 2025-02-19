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
TaskHandle_t      key_task_handle;
QueueHandle_t     key_queue;
key_irq_struct_t  key_irq_data = {0};
QueueHandle_t     key_irq_queue;

//******************************** Declaring ********************************//


//******************************** Function  ********************************//
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
#if (KEY_DETECTION_TYPE == KEY_POLLING_TYPE)
static key_event_t key_scan(void)
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
#endif

/**
 * @brief Detect key interrupt events.
 *
 * Steps:
 *  1. Record the time of the first press, the start time of the key press,
 *     the number of key presses and the type of key event.
 *  2. Process the key triggers a falling edge interrupt.
 *  3. Process the key triggers a rising edge interrupt and the key event is
 *     KEY_DETECTION.
 *
 * @param[in] None.
 *
 * @return key_event_t : Returns the key event:                 KEY_DETECTION
 *                       						KEY_RELEASE   | KEY_SHORT_PRESS
 *                       					 KEY_DOUBLE_PRESS | KEY_LONG_PRESS.
 *
 * */
#if (KEY_DETECTION_TYPE == KEY_INTERRUPT_TYPE)
static key_event_t key_irq_event_detection(void)
{
	// 1.Record the time of the first press, the start time of the key press,
	//   the number of key presses and the type of key event.
	static TickType_t  first_press_time = 0;
	static TickType_t  press_start_time = 0;
	static uint8_t     press_count      = 0;
	static key_event_t key_event        = KEY_RELEASE;

	// 2.If the key triggers a falling edge interrupt
	if(key_irq_data.key_edge_state == KEY_EDGE_FALLING)
	{
		// 2.1 Get the press time and key_event change to KEY_DETECTION
		TickType_t current_time = key_irq_data.time;
		key_event = KEY_DETECTION;

		// 2.2.1.Check if the double-click detection time is met.
		if(((current_time - first_press_time) <= KEY_DOUBLE_PRESS_DELAY) &&
									   ( (key_event != KEY_LONG_PRESS)   ||
										 (key_event != KEY_LONG_PRESS_3) ||
									 	 (key_event != KEY_LONG_PRESS_5) ) )
		{
			press_count++;
		}
		// 2.2.2.If not double-click
		else
		{
			press_count = 1;
			first_press_time = current_time;
		}
		// 2.3.Update the start time of the key press to the current time.
		press_start_time = current_time;
		printf("NOW = [%ld], key_event is [KEY_DETECTION]\r\n",
				                                             press_start_time);
	}
	// 3.If the key triggers a rising edge interrupt and
	//   the key event is KEY_DETECTION
	else if((key_irq_data.key_edge_state == KEY_EDGE_RISING) &&
	                                              (key_event == KEY_DETECTION))
	{
		// 3.1.Calculate the duration of the key press
		TickType_t press_duration = key_irq_data.time - press_start_time;

		// 3.2.If the duration is less than the debounce delay time
		if(press_duration < KEY_DEBOUNCE_DELAY)
		{
			key_event = KEY_RELEASE;
			printf("Rising debounce: %ld\n", press_duration);
			return key_event;
		}

		// 3.3.If the duration is greater than the long press delay time
		if(press_duration > KEY_LONG_PRESS_DELAY)
		{
			if(press_duration > KEY_LONG_PRESS_DELAY_5)
			{
				key_event = KEY_LONG_PRESS_5;
				printf("press_duration = %ld, key_event is [KEY_LONG_PRESS_5]\r\n",
																   press_duration);
			}
			else if(press_duration > KEY_LONG_PRESS_DELAY_3)
			{
				key_event = KEY_LONG_PRESS_3;
				printf("press_duration = %ld, key_event is [KEY_LONG_PRESS_3]\r\n",
												                   press_duration);
			}
			else
			{
				key_event = KEY_LONG_PRESS;
				printf("press_duration = %ld, key_event is [KEY_LONG_PRESS]\r\n",
								                                 press_duration);
			}

//			key_event = KEY_LONG_PRESS;
//			printf("press_duration = %ld, key_event is [KEY_LONG_PRESS]\r\n",
//					                                         press_duration);
			press_count = 0;
		}
		// 3.4.If the number of key presses is greater than or equal to 2
		else if(press_count >= 2)
		{
			key_event = KEY_DOUBLE_PRESS;
			printf("press_duration = %ld, key_event is [KEY_DOUBLE_PRESS]\r\n",
								                               press_duration);
			press_count = 0;
		}
		// 3.5.It's KEY_SHORT_PRESS
		else
		{
			key_event = KEY_SHORT_PRESS;
			first_press_time = key_irq_data.time;
			printf("press_duration = %ld, key_event is [KEY_SHORT_PRESS]\r\n",
											                  press_duration);
		}

		printf("rising time is [%ld]\r\n", key_irq_data.time);
	}
	return key_event;
}
#endif

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
//	static key_event_t key_event_last = KEY_RELEASE;

	// 1. Create key_queue
	key_queue     = xQueueCreate(10, sizeof(key_event_t));
	key_irq_queue = xQueueCreate(10, sizeof(key_irq_struct_t));

	// 2. check whether the queue is created successfully
	if(NULL == key_queue)
	{
		printf("key_queue or key_irq_queue create failure\r\n");
	}
	else
	{
		printf("key_queue and key_irq_queue create successfully\r\n");
	}
	while(1)
	{
#if (KEY_DETECTION_TYPE == KEY_POLLING_TYPE)
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
#endif

#if (KEY_DETECTION_TYPE == KEY_INTERRUPT_TYPE)
	// 3.Receive key interrupt data from the key_irq_queue
	if(pdPASS == xQueueReceive(key_irq_queue, &key_irq_data, portMAX_DELAY))
	{
		printf("key_irq_queue receive successfully.\r\n");
	}
	else
	{
		printf("key_irq_queue fail to receive.\r\n");
	}

	// 4.Detect key interrupt events
	key_even = key_irq_event_detection();

	// 5.If the key event is not in the detection state
	if(key_even != KEY_DETECTION)
	{
		// 6.Send the key event to the queue
		if(pdPASS == xQueueSend(key_queue, &key_even, pdMS_TO_TICKS(100)))
		{
			printf("key_queue send successfully.\r\n");
		}
		else
		{
			printf("key_queue fail to send.\r\n");
		}
	}
#endif
//		vTaskDelay(10);
	}
}
/**
 * @brief GPIO External Interrupt Callback Function.
 *
 * Steps:
 *  1. Declare a variable to indicate if a higher priority task has been woken up.
 *  2. Read the current state of the GPIO pin that triggered the interrupt.
 *  3. Determine if it's a rising or falling edge interrupt based on the pin state.
 *  4. Get the current system tick count to record the time of the interrupt.
 *  5. Send the interrupt data to the queue for other tasks to handle.
 *  6. If a higher priority task has been woken up, perform a task switch.
 *
 * @param[in] GPIO_Pin: The GPIO pin number that triggered the interrupt.
 *
 * @return None.
 *
 * */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	HAL_GPIO_WritePin(IRQ_TRACE_GPIO_Port, IRQ_TRACE_Pin, GPIO_PIN_SET);
	// 1.Declare a variable to indicate if a higher priority task
	//   has been woken up.
	BaseType_t    xHigherPriorityTaskWoken = pdFALSE;
	// 2.Read the current state of the GPIO pin that triggered the interrupt
	GPIO_PinState pin_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, GPIO_Pin);
	// 3.Determine if it's a rising or falling edge interrupt
	//   based on the pin state
	key_irq_data.key_edge_state = ( (pin_state == GPIO_PIN_RESET) ?
									   KEY_EDGE_FALLING : KEY_EDGE_RISING);
	// 4.Get the current system tick count to record the time of the interrupt
	key_irq_data.time = xTaskGetTickCountFromISR();
	// 5.Send the interrupt data to the queue for other tasks to handle
	xQueueSendFromISR(             key_irq_queue,
					               &key_irq_data,
			          &xHigherPriorityTaskWoken);
	// 6.If a higher priority task has been woken up, perform a task switch
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	HAL_GPIO_WritePin(IRQ_TRACE_GPIO_Port, IRQ_TRACE_Pin, GPIO_PIN_RESET);
}



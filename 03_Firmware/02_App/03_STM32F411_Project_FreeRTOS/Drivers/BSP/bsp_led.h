/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_led.h
 *
 * @par dependencies
 * - stdint.h
 * - FreeRTOS.h
 * - main.h
 * - cmsis_os.h
 * - queue.h
 *
 * @author Zienat
 *
 * @brief Provide the HAL APIs of LED and corresponding opetions.
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
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

//******************************** Includes *********************************//
#include <stdint.h>

#include "FreeRTOS.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"

//******************************** Defines **********************************//


//******************************** Declaring ********************************//

/* Function return enum */
typedef enum
{
  LED_OK                  = 0,         /* Operation completed successfully.  */
  LED_ERROR               = 1,         /* Run-time error without case matched*/
  LED_ERRORTIMEOUT        = 2,         /* Operation failed with timeout      */
  LED_ERRORRESOURCE       = 3,         /* Resource not available.            */
  LED_ERRORPARAMETER      = 4,         /* Parameter error.                   */
  LED_ERRORNOMEMORY       = 5,         /* Out of memory.                     */
  LED_ERRORISR            = 6,         /* Not allowed in ISR context         */
  LED_RESERVED            = 0x7FFFFFFF /* Reserved                           */
} led_status_t;

/* Led even enum */
typedef enum
{
	LED_NONE              = 0,
	LED_ON                = 1,
	LED_OFF               = 2,
	LED_TOGGLE            = 3,
	LED_BLINK3            = 4
}led_event_t;

/* Variables Declaring */
extern TaskHandle_t    led_task_handle;
extern QueueHandle_t   led_queue;

/* Function Declaring */
extern void            led_task_func(void * argument);
extern void            led_control(led_event_t led_event);

#endif /* End of __BSP_LED_H__ */

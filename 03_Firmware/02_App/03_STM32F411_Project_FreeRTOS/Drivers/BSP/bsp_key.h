/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_key.h
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
#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

//******************************** Includes *********************************//
#include <stdint.h>

#include "FreeRTOS.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"

//******************************** Defines **********************************//
//#define KEY_DEBOUNCE_DELAY     (pdMS_TO_TICKS(50))
#define KEY_SHORT_PRESS_DELAY   (pdMS_TO_TICKS(50))
#define KEY_DOUBLE_PRESS_DELAY  (pdMS_TO_TICKS(300))
#define KEY_LONG_PRESS_DELAY    (pdMS_TO_TICKS(1000))
#define GET_TICK_DIFF(old)      ((xTaskGetTickCount()) - (old))

//******************************** Declaring ********************************//

/* Function return enum */
typedef enum
{
  KEY_OK                  = 0,         /* Operation completed successfully.  */
  KEY_ERROR               = 1,         /* Run-time error without case matched*/
  KEY_ERRORTIMEOUT        = 2,         /* Operation failed with timeout      */
  KEY_ERRORRESOURCE       = 3,         /* Resource not available.            */
  KEY_ERRORPARAMETER      = 4,         /* Parameter error.                   */
  KEY_ERRORNOMEMORY       = 5,         /* Out of memory.                     */
  KEY_ERRORISR            = 6,         /* Not allowed in ISR context         */
  KEY_RESERVED            = 0x7FFFFFFF /* Reserved                           */
} key_status_t;

/* Key even enum */
typedef enum
{
	KEY_RELEASE           = 0,
	KEY_SHORT_PRESS       = 1,
	KEY_DOUBLE_PRESS      = 2,
	KEY_LONG_PRESS        = 3,
}key_event_t;

typedef enum
{
    KEY_RELEASING         = 0,
	KEY_PRESSING          = 1,
    KEY_WAIT_DOUBLE       = 2,
    KEY_LONG_PRESSING     = 3
}key_state_machine_t;

typedef enum
{
	KEY_EDGE_NONE         = 0,
	KEY_EDGE_FALLING      = 1,
	KEY_EDGE_RISING       = 2,
}key_edge_state_t;

typedef struct
{
	TickType_t            time;
	key_edge_state_t      key_edge_state;
}key_irq_struct_t;

/* Variables Declaring */
extern TaskHandle_t       key_task_handle;
extern QueueHandle_t      key_queue;
extern key_irq_struct_t   key_irq_struct;

/* Function Declaring */
extern void               key_task_func(void * argument);
extern key_event_t        key_scan(void);
extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);



#endif /* End of __BSP_KEY_H__ */

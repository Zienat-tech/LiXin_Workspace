/******************************************************************************
 * Copyright (C) 2024 EternalChip, Inc.(Gmbh) or its affiliates.
 *
 * All Rights Reserved.
 *
 * @file bsp_key.h
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
#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

//******************************** Includes *********************************//
#include <stdint.h>

#include "FreeRTOS.h"
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"

//******************************** Defines **********************************//
#define KEY_DEBOUNCE_DELAY  (pdMS_TO_TICKS(50))


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

typedef enum
{
	KEY_PRESSED           = 0,
	KEY_RELEASED          = 1,
}key_event_t;


//******************************** Declaring ********************************//
extern TaskHandle_t    key_task_handle;
extern QueueHandle_t   key_queue;

extern void            key_task_func(void * argument);
extern key_event_t     key_scan(void);




#endif /* End of __BSP_KEY_H__ */

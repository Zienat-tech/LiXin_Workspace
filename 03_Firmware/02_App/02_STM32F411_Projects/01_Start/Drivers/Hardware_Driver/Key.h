#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

#define KEY_RELEASE           (0)
#define KEY_PRESS             (1)
#define KEY_READ_WAIT_TIME    (10)

extern uint8_t g_key_state;



extern void key_task_runnable(void);


#endif

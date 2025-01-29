#include "Key.h"

uint8_t g_key_state = KEY_RELEASE;

static uint8_t key_init(void)
{
	/*  */
	return 0;
}

static uint8_t key_get_state(void)
{
	uint8_t key_state = 0;
	key_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, GPIO_PIN_0);
	if(key_state == 1)
	{
		key_state = 0;
	}
	else
	{
		key_state = 1;
	}
	return key_state;
}

void key_task_runnable(void)
{
	static uint8_t key_init_state = INIT_NOT_SUCCESS;
	uint8_t key_state = KEY_RELEASE;

	if(key_init_state == INIT_NOT_SUCCESS)
	{
		if(key_init() == 0)
		{
			key_init_state = INIT_SUCCESS;
		}
	}
	else
	{
		key_state = key_get_state();
		if(key_state == KEY_PRESS)
		{
			HAL_Delay(KEY_READ_WAIT_TIME);
			if(key_state == KEY_PRESS)
			{
				g_key_state = KEY_PRESS;
			}
			else
			{
				g_key_state = KEY_RELEASE;
			}
		}
		else
		{
			g_key_state = KEY_RELEASE;
		}

	}
}









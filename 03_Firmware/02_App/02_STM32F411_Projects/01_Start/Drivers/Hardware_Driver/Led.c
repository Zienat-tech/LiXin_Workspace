#include "Led.h"

static uint8_t led_init(void)
{
	/*  */
	return 0;
}

void led_task_runnable(void)
{
	static uint8_t led_init_state = INIT_NOT_SUCCESS;
	static uint8_t led_state = 0;
	static uint8_t led_count = 0;

	if(led_init_state == INIT_NOT_SUCCESS)
	{
		if(led_init() == 0)
		{
			led_init_state = INIT_SUCCESS;
		}
	}
	else
	{
		/* 检测KEY按下的时间，超过规定时间且松开后LED翻转 */
		if(g_key_state == KEY_PRESS)
		{
			led_state = 1;
		}
		else
		{
			led_state = 0;
		}
		if(led_state == 1)
		{
			led_count++;
		}
		else
		{
			if(led_count >= LED_FLIP_EVENT_TIME)
			{
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			}
			led_count = 0;
		}
#if 0
		/* KEY按下时闪烁，松开时常亮 */
		if(g_key_state == KEY_PRESS)
		{
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			HAL_Delay(500);
		}
		else if(g_key_state == KEY_RELEASE)
		{
			HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
		}
#endif
	}
}







#ifndef _PRIVATE_CFG_H_
#define _PRIVATE_CFG_H_

#ifndef LOG_OUTPUT_MODE
	#define LOG_UART_MODE              0
	#define LOG_RTT_MODE               1
	#define LOG_ELOG_MODE              2
	#define LOG_OUTPUT_MODE            LOG_ELOG_MODE
#endif

#ifndef INTERRUPT_DELAY_CAPTURE
	#define INTERRUPT_DELAY_CAPTURE    0
#endif

#ifndef KEY_DETECTION_MODE
	#define KEY_POLLING_MODE      0
	#define KEY_IRQ_MODE          1
	#define KEY_DETECTION_MODE    KEY_IRQ_MODE
#endif


#endif

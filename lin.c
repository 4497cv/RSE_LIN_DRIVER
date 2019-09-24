/*
	\file lin.c
	\brief	This is the source file for the LIN device driver for Kinetis K64.
	
	\authors César Villarreal, @4497cv
			 Tsipini Franco,
			 Moisés López,
	\date	 24/09/2019
*/


/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_flexcan.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"

#include "lin.h"
#include "uart.h"

#define SYSTEM_CLOCK (21000000U)

void LIN_init(const lin_config_t* LIN_config)
{
	UART_init(LIN_config->uart_channel, (uint32_t) system_clk, LIN_config->baudrate);
}

int main(void)
{
	/* Initialize board hardware. */
	BOARD_InitPins();
	BOARD_BootClockRUN();
	BOARD_InitDebugConsole();
	CAN_Init();

	const lin_config_t LIN_config = 
	{
		UART_0,       //uart channel
		SYSTEM_CLOCK, //system clock
		BD_9600       //uart's transference baud-rate
	}

    xTaskCreate(task_100ms, "100ms Task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);
    vTaskStartScheduler();

    for (;;)
	{

	}

	return 0;
}


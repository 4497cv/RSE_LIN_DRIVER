/*
 * main.c
 *
 *  Created on: 10.10.2019
 *      Author: César
 */

#include "lin.h"
#include <stdio.h>
#include "MK64F12.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "rtos_uart.h"
#include "bits.h"
#include "pin_mux.h"
#include "fsl_port.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "can.h"
#include "lin.h"

int main(void)
{
	lin_config_t LIN_config =
	{
		LIN_BD_9600,       //uart's transference baud-rate
		SLAVE,		       //system's mode (slave or master)
		MESSAGE_ID,	       //message identifier
		MESSAGE_PARITY     //the identifier bits sets the size of data field
	};

	BOARD_BootClockRUN();

	LIN_init(&LIN_config);

	switch(LIN_config.operation_mode)
	{
		case MASTER:
			/* Master thread creation */
			xTaskCreate(LIN_MESSAGE_HEADER_THREAD, "Message_Header", 1000, NULL, 1, NULL);
		break;
		case SLAVE:
			/* Slave thread creation */
			xTaskCreate(LIN_MESSAGE_RESPONSE_THREAD, "Message_Response", 1000, NULL, 1, NULL);
		break;
		default:
		break;
	}

	/* Start scheduler */
	vTaskStartScheduler();

	for(;;)
	{

	}

	return 0;
}

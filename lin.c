/*
	\file lin.c
	\brief	This is the source file for the LIN device driver for Kinetis K64.
	
	\authors César Villarreal, @4497cv
			 Tsipini Franco, @t51p
			 Moisés López, @DES7RIKER
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
uart_channel_t g_uart_channel;

static FSM_master_t FSM_master[HEADER_ST] =
{
	{LIN_SYNC_BREAK,  {synch_break, synch_field, ident_field}},
	{LIN_SYNC_FIELD,  {synch_field, ident_field, synch_break}},
	{LIN_IDENT_FIELD, {ident_field, synch_break, synch_field}}
}

void LIN_init(const lin_config_t* LIN_config)
{
	UART_init(LIN_config->uart_channel, (uint32_t) LIN_config->system_clk, LIN_config->baudrate);
	g_uart_channel = LIN_config->uart_channel;
	
	switch(LIN_config->operation_mode)
	{
		case MASTER:

		break;
		case SLAVE:

		break;
		default:
		break;
	}
}

static void LIN_SYNC_BREAK()
{
	/*
		A dominant signal is recognized as Synch Break field,
		if it is longer than the maximum regular sequence of 
		dominant bits in the protocol.
	*/
	uint8_t  low_phase_first;
	uint8_t  low_phase_second;

	low_phase_first = 0x00;
	low_phase_second = 0x07;

	UART_put_char(g_uart_channel, low_phase_first);
	UART_put_char(g_uart_channel, low_phase_second);
}

static void LIN_SYNC_FIELD()
{
	uint8_t  synch_field_data;

	synch_field_data = 0x55;

	UART_put_char(g_uart_channel, synch_field_data);
}

static void LIN_IDENT_FIELD()
{
	
}

//***********//
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
		MASTER
	}

	LIN_init(LIN_config);
    //xTaskCreate(task_100ms, "100ms Task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL);
    //vTaskStartScheduler();

    for(;;)
	{

	}

	return 0;
}

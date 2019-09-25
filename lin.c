/*
	\file lin.c
	\brief	This is the source file for the LIN device driver for Kinetis K64.
	
	\authors César Villarreal, @4497cv
			 Tsipini Franco, @t51p
			 Moisés López, @DES7RIKER
	\date	 24/09/2019
*/

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
	{LIN_SYNC_BREAK,  {synch_field, ident_field, synch_break}},
	{LIN_SYNC_FIELD,  {ident_field, synch_break, synch_field}},
	{LIN_IDENT_FIELD, {synch_break, synch_field, ident_field}}
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

/*~~~~~~~~~~~~~~~~~ <HEADER> ~~~~~~~~~~~~~~~~~~~*/
void LIN_SEND_MESSAGE_HEADER()
{
	lin_header_st hd_state = synch_break;

	while(synch_break != hd_state.next[0])
	{
		FSM_master[hd_state].fptr();
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
	/*
		"The  SYNCH  FIELD  contains  the  information for  the  clock
		synchronization.  TheSYNCH FIELD is the data '0x55' inside 
		a byte field, which is characterized by five fall-ing edges
		(i.e. ërecessiveí to ëdominantí edges) within eight bit times
		distance", p.21 

		https://lniv.fe.uni-lj.si/courses/ndes/LIN_Spec_1_3.pdf
	*/

	uint8_t  synch_field_data;

	synch_field_data = 0x55;

	UART_put_char(g_uart_channel, synch_field_data);
}

/*
	This field provides identification for each message on the network
	and ultimately determines which nodes in the network receive or respond 
	to each transmission.
*/
static void LIN_IDENT_FIELD()
{
	/*
		The IDENTIFIER FIELD (ID-Field) denotes the content of a message. 
		The content is represented by six IDENTIFIER bits and two ID PARITY 
		bits.

		the  IDENTIFIER  bits  ID4  andID5  may  define  the  number  of  data 
		fields in a message. This divides the set of 64 identifiers in four subsets 
		of sixteen identifiers, with 2, 4, and 8 data fields, respectively. In any case 
		the length of a data field is defined in the configuration 
		description file.

		ID5 ID4	NDATA
		0    0    2
		0    1    2
		1    0    4
		1    1    8
	*/

	id_field_t ID = {0};

	ID.parity_1 = (ID.id_0 && ID.id_1 && ID.id_2 && ID.id_4)
	ID.parity_0 = !(ID.id_0 && ID.id_3 && ID.id_4 && ID.id_5)

}

/*~~~~~~~~~~~~~~~~~ </HEADER> ~~~~~~~~~~~~~~~~~~~*/

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

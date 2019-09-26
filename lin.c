/*
	\file lin.c
	\brief	This is the source file for the LIN device driver for Kinetis K64.
	
	\authors César Villarreal, @4497cv
			 Tsipini Franco, @t51p
			 Moisés López, @DES7RIKER
	\date	 24/09/2019
*/

/* Freescale includes. */
#include "MK64F12.h"
#include "lin.h"
#include "uart.h"
#include "bits.h"
#include "gpio.h"

#define SYSTEM_CLOCK (21000000U)
uart_channel_t g_uart_channel;

static FSM_master_t FSM_master[HEADER_ST] =
{
	{LIN_SYNC_BREAK,  {synch_field, ident_field, synch_break}},
	{LIN_SYNC_FIELD,  {ident_field, synch_break, synch_field}},
	{LIN_IDENT_FIELD, {synch_break, synch_field, ident_field}}
};

void LIN_init(const lin_config_t* LIN_config)
{
	UART_init(LIN_config->uart_channel, (uint32_t) LIN_config->system_clk, LIN_config->baud_rate);
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

//	while(ident_field != FSM_master[hd_state].next[0])
//	{
//		FSM_master[hd_state].fptr();
//		hd_state = FSM_master[hd_state].next[0];
//	}

	LIN_SYNC_BREAK();
/* 
	UART_put_char(g_uart_channel, low_phase_first);
	UART_put_char(g_uart_channel, low_phase_second);

	UART_put_char(g_uart_channel, synch_field_data); */

}

void LIN_SYNC_BREAK()
{
	/*
		A dominant signal is recognized as Synch Break field,
		if it is longer than the maximum regular sequence of 
		dominant bits in the protocol.
	*/

	uint8_t  low_phase_first;
	uint8_t  low_phase_second;

	low_phase_first = 0x00;
	low_phase_second = 0x00;

	UART_put_char(g_uart_channel, low_phase_first);
	UART_put_char(g_uart_channel, low_phase_second);
	UART_put_char(g_uart_channel, SYNC_FIELD_MASK);
}

void LIN_SYNC_FIELD()
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

	synch_field_data = SYNC_FIELD_MASK;

	UART_put_char(g_uart_channel, synch_field_data);
}

/*
	This field provides identification for each message on the network
	and ultimately determines which nodes in the network receive or respond 
	to each transmission.
*/
void LIN_IDENT_FIELD(uint8_t message_id, uint8_t message_parity)
{
	uint8_t id_field;
	boolean_t id_valid;
	
	id_field = 0;
	id_valid = is_identifier_valid(message_id);
	
	if(TRUE == id_valid)
	{
		message_parity &= (MSG_PARITY_MASK);
		message_id &= (MSG_ID_MASK);
		
		id_field |= message_parity;
		id_field |= message_id;

		/*
			The IDENTIFIER FIELD (ID-Field) denotes the content of a message. 
			The content is represented by six IDENTIFIER bits and two ID PARITY 
			bits.

			the  IDENTIFIER  bits  ID4  andID5  may  define  the  number  of  data 
			fields in a message. This divides the set of 64 identifiers in four subsets 
			of sixteen identifiers, with 2, 4, and 8 data fields, respectively. In any case 
			the length of a data field is defined in the configuration 
			description file.

										ID5  ID4 NDATA
										 0    0    2
										 0    1    2
										 1    0    4
										 1    1    8
		*/

		UART_put_char(g_uart_channel, id_field);
	}
	else
	{
		/* ID NOT VALID*/
	}
	
}

boolean_t is_identifier_valid(uint8_t message_id)
{
	boolean_t ret_val;

	if((message_id != RID0) &&
	   (message_id != RID1) &&
	   (message_id != RID2) &&
	   (message_id != RID3) &&
	   (message_id != RID4) &&
	   (message_id != RID5))
	{	
		ret_val = TRUE;
	}
	else
	{
		ret_val = FALSE;
	}
	
	return ret_val;
}


/*~~~~~~~~~~~~~~~~~ </HEADER> ~~~~~~~~~~~~~~~~~~~*/

//***********//
int main(void)
{
	const lin_config_t LIN_config =
	{
		UART_3,       //uart channel
		SYSTEM_CLOCK, //system clock
		BD_9600,       //uart's transference baud-rate
		MASTER
	};

	/* System GPIO configuration */
	gpio_pin_control_register_t uart_config = GPIO_MUX3;
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);

	/**Configures the pin control register of pin16 in PortB as UART RX*/
	GPIO_pin_control_register(GPIO_C, BIT16, &uart_config);
	/**Configures the pin control registe of pin16 in PortB as UART TX*/
	GPIO_pin_control_register(GPIO_C, BIT17, &uart_config);

	//GPIO_data_direction_pin(GPIO_C, GPIO_OUTPUT, BIT17);
	//GPIO_set_pin(GPIO_B, BIT19);

	LIN_init(&LIN_config);

    for(;;)
	{
    	LIN_SEND_MESSAGE_HEADER();
	}

	return 0;
}

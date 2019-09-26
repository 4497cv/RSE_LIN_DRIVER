/*
	\file lin.c
	\brief	This is the source file for the LIN device driver for Kinetis K64.
	
	\authors César Villarreal, @4497cv
			 Tsipini Franco, @t51p
			 Moisés López, @DES7RIKER
	\date	 24/09/2019


	\TODO: SLAVE MODE
*/

/* Freescale includes. */
#include "MK64F12.h"
#include "lin.h"
#include "uart.h"
#include "bits.h"
#include "gpio.h"

/* Definitions */
#define SYSTEM_CLOCK   (21000000U)
#define MESSAGE_ID     (0X10U)
#define MESSAGE_PARITY (0X00U)
#define LOW_PHASE_VAL  (0X00U)

/* Global variables */
static lin_config_t* g_lin_config; //configuration values

static FSM_master_t FSM_master[3] =
{
	{LIN_SYNC_BREAK,  {synch_field, ident_field, synch_break}},
	{LIN_SYNC_FIELD,  {ident_field, synch_break, synch_field}},
	{LIN_IDENT_FIELD, {synch_break, synch_field, ident_field}}
};

void LIN_init(lin_config_t* LIN_config)
{
	UART_init(LIN_config->uart_channel, (uint32_t) LIN_config->system_clk, LIN_config->baud_rate);
	g_lin_config = LIN_config;

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
	lin_header_st hd_state; //header's state

	/* header's initial state */
	hd_state = synch_break;

	do
	{
		/* invoke header's state function */
		FSM_master[hd_state].fptr();
		/* switch to the next state */
		hd_state = FSM_master[hd_state].next[0];
	}while(synch_break != hd_state);
}

static void LIN_SYNC_BREAK()
{
	/*
		A dominant signal is recognized as Synch Break field,
		if it is longer than the maximum regular sequence of 
		dominant bits in the protocol.
	*/

	uint8_t sb_low_phase;

	sb_low_phase = LOW_PHASE_VAL;

	UART_put_char(g_lin_config->uart_channel, sb_low_phase);
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

	synch_field_data = SYNC_FIELD_MASK;

	UART_put_char(g_lin_config->uart_channel, synch_field_data);
}

/*
	This field provides identification for each message on the network
	and ultimately determines which nodes in the network receive or respond 
	to each transmission.
*/
static void LIN_IDENT_FIELD()
{
	boolean_t id_valid;
	uint8_t id_field;
	
	id_field = 0;
	id_valid = is_identifier_valid(g_lin_config->message_id);
	
	if(TRUE == id_valid)
	{
		g_lin_config->message_parity &= (MSG_PARITY_MASK);
		g_lin_config->message_id &= (MSG_ID_MASK);
		
		id_field |= g_lin_config->message_parity;
		id_field |= g_lin_config->message_id;

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

		UART_put_char(g_lin_config->uart_channel, id_field);
	}
	else
	{
		/* ID NOT VALID*/
	}
	
}

boolean_t is_identifier_valid(uint8_t message_id)
{
	boolean_t ret_val;

	/* verify that the id is not reserved */
	if((message_id != RID0) &&
	   (message_id != RID1) &&
	   (message_id != RID2) &&
	   (message_id != RID3) &&
	   (message_id != RID4) &&
	   (message_id != RID5))
	{	
		/* message ID is valid */
		ret_val = TRUE;
	}
	else
	{
		/* message ID is invalid */
		ret_val = FALSE;
	}
	
	return ret_val;
}


/*~~~~~~~~~~~~~~~~~ </HEADER> ~~~~~~~~~~~~~~~~~~~*/

//***********//
int main(void)
{
	lin_config_t LIN_config =
	{
		UART_3,        //selected UART's channel
		SYSTEM_CLOCK,  //system clock reference
		BD_9600,       //uart's transference baud-rate
		MASTER,		   //system's mode (slave or master)
		MESSAGE_ID,	   //message identifier
		MESSAGE_PARITY //the identifier bits sets the size of data field
	};

	/* System GPIO configuration */
	gpio_pin_control_register_t uart_config = GPIO_MUX3;

	/* Sytem's clock gating initizalization  */
	GPIO_clock_gating(GPIO_B);
	GPIO_clock_gating(GPIO_C);

	/* Configure GPIO PTC_16 for UART's Rx*/
	GPIO_pin_control_register(GPIO_C, BIT16, &uart_config);
	/* Configure GPIO PTC_16 for UART's Tx*/
	GPIO_pin_control_register(GPIO_C, BIT17, &uart_config);

	/* Local Interconnect Network driver initialization */
	LIN_init(&LIN_config);

	/* SUPER-LOOP */
    for(;;)
	{
    	LIN_SEND_MESSAGE_HEADER(); //THIS IS TEMPORAL
	}

	return 0;
}

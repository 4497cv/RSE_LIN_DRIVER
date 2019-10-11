/*
	\file lin.c
	\brief	This is the source file for the LIN device driver for Kinetis K64.
	
	\authors: César Villarreal, @4497cv
			  Tsipini Franco, @t51p
			  Moisés López, @DES7RIKER
	\date	  24/09/2019
*/

/* Freescale includes. */

#include "lin.h"

/* Global variables */
static lin_config_t* g_lin_config; //configuration values
static uint8_t message_id_g;
static uint8_t message_parity_g;
static uint8_t data_g[N_DATA];
static uint8_t message_header_g[3];
static SemaphoreHandle_t header_sem;
static SemaphoreHandle_t response_sem;

static fsm_header_t FSM_HEADER[HEADER_ST] =
{
	{LIN_SYNC_BREAK,  {synch_field, ident_field, synch_break}},
	{LIN_SYNC_FIELD,  {ident_field, synch_break, synch_field}},
	{LIN_IDENT_FIELD, {synch_break, synch_field, ident_field}}
};

static fsm_response_t FSM_RESPONSE[RESPONSE_ST] =
{
	{LIN_SEND_DATA, {check_sum, send_data}},
	{LIN_CHECKSUM,  {send_data, check_sum}}
};

void LIN_init(lin_config_t* LIN_config)
{
	rtos_uart_config_t config;

	switch(LIN_config->operation_mode)
	{
		case MASTER:
			/* UART configuration for master */
			config.baudrate = 9600;
			config.rx_pin = 16;
			config.tx_pin = 17;
			config.pin_mux = kPORT_MuxAlt3;
			config.uart_number = rtos_uart3;
			config.port = rtos_uart_portC;
			rtos_uart_init(config);
		break;
		case SLAVE:
			/* UART configuration for slave */
			config.baudrate = 9600;
			config.rx_pin = 16;
			config.tx_pin = 17;
			config.pin_mux = kPORT_MuxAlt3;
			config.uart_number = rtos_uart3;
			config.port = rtos_uart_portC;
			rtos_uart_init(config);
		break;
		default:
		break;
	}

	/* UART configuration for serial port */
	config.baudrate = 115200;
	config.rx_pin = 16;
	config.tx_pin = 17;
	config.pin_mux = kPORT_MuxAlt3;
	config.uart_number = rtos_uart0;
	config.port = rtos_uart_portB;
	rtos_uart_init(config);

	message_id_g = LIN_config->message_id;
	message_parity_g = LIN_config->message_parity;

	message_header_g[0] = LOW_PHASE_VAL_1;
	message_header_g[1] = SYNC_FIELD_MASK;
	message_header_g[2] = MESSAGE_ID;
	data_g[0] = 0x10;

//	header_sem =  xSemaphoreCreateBinary();
//	response_sem =  xSemaphoreCreateBinary();
 }

/*~~~~~~~~~~~~~~~~~ <HEADER> ~~~~~~~~~~~~~~~~~~~*/
void LIN_MESSAGE_HEADER_THREAD(void* args)
{
	lin_header_st hd_state; //header's state
	uint8_t adc_val;
	uint8_t checksum;

	/* header's initial state */
	hd_state = synch_break;

	for(;;)
	{
		do
		{
			/* invoke header's state function */
			FSM_HEADER[hd_state].fptr();
			/* switch to the next state */
			hd_state = FSM_HEADER[hd_state].next[0];
		}while(synch_break != hd_state);
	}
}

void LIN_MESSAGE_RESPONSE_THREAD(void* args)
{
	uint8_t data;
	uint8_t buffer[3];
	uint8_t i = 0;
	lin_response_st response_st = send_data;

	for(;;)
	{
		rtos_uart_receive(rtos_uart3, &data, 1);

		if(i == 3)
		{
			if(TRUE == is_header_valid(buffer[0], buffer[1], buffer[2]))
			{
				do
				{
					FSM_RESPONSE[response_st].fptr();
					response_st = FSM_RESPONSE[response_st].next[0];
				} while(response_st != send_data);

			}
			else
			{

			}
			i = 0;
		}
		else if(i < 3)
		{
			buffer[i] = data;
			i++;
		}
		else
		{

		}

	}
}

boolean_t is_header_valid(uint8_t buffer_0, uint8_t buffer_1, uint8_t buffer_2)
{
	uint8_t y;
	uint8_t buffer[3];

	buffer[0] = buffer_0;
	buffer[1] = buffer_1;
	buffer[2] = buffer_2;
	boolean_t valid_buffer;

	for(y = 0; y < 3; y++)
	{
		if(message_header_g[y] == buffer[y])
		{
			valid_buffer = TRUE;
		}
		else
		{
			valid_buffer = FALSE;
		}
	}

	return valid_buffer;

}

static void LIN_SYNC_BREAK()
{
	/*
		A dominant signal is recognized as Synch Break field,
		if it is longer than the maximum regular sequence of 
		dominant bits in the protocol.
	*/

	uint8_t sb_low_phase_first;
	//uint8_t sb_low_phase_second;

	sb_low_phase_first = LOW_PHASE_VAL_1;
	//sb_low_phase_second = LOW_PHASE_VAL_2;

	rtos_uart_send(rtos_uart3, &sb_low_phase_first,  DATA_LENGTH);
	//rtos_uart_send(rtos_uart3, &sb_low_phase_second, DATA_LENGTH);
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

	rtos_uart_send(rtos_uart3, &synch_field_data, DATA_LENGTH);
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
		id_field |= message_parity_g;
		id_field |= message_id_g;

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

		rtos_uart_send(rtos_uart3, &id_field, DATA_LENGTH);
	}
	else
	{
		/* ID NOT VALID*/
	}
}

/*~~~~~~~~~~~~~~~~~ <RESPONSE> ~~~~~~~~~~~~~~~~~~~*/

void LIN_SEND_DATA(void)
{
	uint8_t i;

	for(i = 0; i < N_DATA; i++)
	{
		rtos_uart_send(rtos_uart3, &data_g[i], DATA_LENGTH);
	}
}

void LIN_CHECKSUM(void)
{
	uint8_t chksum = 0;
	uint8_t i = 0;

	for(i = 0; i < N_DATA; i++)
	{
		chksum+=data_g[i];

		if(chksum >= 256)
		{
			chksum -= 255;
		}
	}

	chksum = 0xFF & ~(chksum);


	rtos_uart_send(rtos_uart3, &chksum, DATA_LENGTH);
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


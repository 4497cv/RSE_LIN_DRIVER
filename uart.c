/**
	\file 	  uart.c
	\brief
			  This is the source file for the UART Full Duplex device driver for Kinetis K64.
	\authors: César Villarreal Hernández, ie707560
	          José Luis Rodríguez Gutierrez,ie705694
	\date	  08/04/2019
 */

#include <uart.h>

uart_mail_box_t g_mail_box_uart_0;
uart_mail_box_t g_mail_box_uart_1;
uart_mail_box_t g_mail_box_uart_2;
uart_mail_box_t g_mail_box_uart_3;
uart_mail_box_t g_mail_box_uart_4;
uart_mail_box_t g_mail_box_uart_5;

void UART0_RX_TX_IRQHandler(void)
{
	/*First is verified if the serial port finished to transmit*/
	UART_Wait_Transmit_Complete();
	/*The info is saved in Data Register*/
	g_mail_box_uart_0.mail_box = UART_Get_Receiver_Data_Buffer(UART_0);
	/*There are new data*/
	g_mail_box_uart_0.flag = TRUE;
}

void UART_init(uart_channel_t uart_channel, uint32_t system_clk, baud_rate_t baud_rate)
{
	uint16_t SBR_BRFD;
	switch(uart_channel)
	{
    case UART_0:
		/** Activate UART 0 clock gating **/
		SIM->SCGC4 = SIM_SCGC4_UART0_MASK;
		/** Disabling transmission and reception **/
		UART0->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		/*Calculating the value of the Baud Rate Fine Adjustment*/
		SBR_BRFD = (uint16_t)(system_clk/(SDR * baud_rate));
		/** Set Uart Baud Rate Bits (SBR) bits to zero **/
		UART0->BDH &= ~(UART_BDH_SBR(UART_BDH_SBR_MASK));
		/** Set SBR buffer data in temporary location **/
		UART0->BDH |= ((SBR_BRFD & UART_BDH_SBR_MASK) >> (shift_8bit));
		/** Put the SBR buffer data in permanent location **/
		UART0->BDL = (uint8_t) (SBR_BRFD & UART_BDL_SBR_MASK);
		/** The parity bit is the tenth bit in the serial transmission **/
		UART0->C4 |= UART_C4_BRFA_MASK;
		/** Set baud rate fine adjust: adding more timing resolution to baud freq.**/
		UART0->C4 = SBR_BRFD;
		/** Enabling transmission and reception **/
		UART0->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
		/** Baud rate generation enabled **/
    break;
    case UART_1:
		/** Activate UART 0 clock gating **/
		SIM->SCGC4 = SIM_SCGC4_UART1_MASK;
		/** Disabling transmission and reception **/
		UART1->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		/*Calculating the value of the Baud Rate Fine Adjustment*/
		SBR_BRFD = (uint16_t)(system_clk/(SDR * baud_rate));
		/** Set Uart Baud Rate Bits (SBR) bits to zero **/
		UART1->BDH &= ~(UART_BDH_SBR(UART_BDH_SBR_MASK));
		/** Set SBR buffer data in temporary location **/
		UART1->BDH |= ((SBR_BRFD & UART_BDH_SBR_MASK) >> (shift_8bit));
		/** Put the SBR buffer data in permanent location **/
		UART1->BDL = (uint8_t) (SBR_BRFD & UART_BDL_SBR_MASK);
		/** The parity bit is the tenth bit in the serial transmission **/
		UART1->C4 |= UART_C4_BRFA_MASK;
		/** Set baud rate fine adjust: adding more timing resolution to baud freq.**/
		UART1->C4 = SBR_BRFD;
		/** Enabling transmission and reception **/
		UART1->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
		/** Baud rate generation enabled **/
    break;
    case UART_2:
		/** Activate UART 0 clock gating **/
		SIM->SCGC4 = SIM_SCGC4_UART2_MASK;
		/** Disabling transmission and reception **/
		UART2->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		/*Calculating the value of the Baud Rate Fine Adjustment*/
		SBR_BRFD = (uint16_t)(system_clk/(SDR * baud_rate));
		/** Set UART Baud Rate Bits (SBR) bits to zero **/
		UART2->BDH &= ~(UART_BDH_SBR(UART_BDH_SBR_MASK));
		/** Set SBR buffer data in temporary location **/
		UART2->BDH |= ((SBR_BRFD & UART_BDH_SBR_MASK) >> (shift_8bit));
		/** Put the SBR buffer data in permanent location **/
		UART2->BDL = (uint8_t) (SBR_BRFD & UART_BDL_SBR_MASK);
		/** The parity bit is the tenth bit in the serial transmission **/
		UART2->C4 |= UART_C4_BRFA_MASK;
		/** Set baud rate fine adjust: adding more timing resolution to baud freq.**/
		UART2->C4 = SBR_BRFD;
		/** Enabling transmission and reception **/
		UART2->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
		/** Baud rate generation enabled **/
    break;
    case UART_3:
		/** Activate UART 0 clock gating **/
		SIM->SCGC4 = SIM_SCGC4_UART3_MASK;
		/** Disabling transmission and reception **/
		UART3->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		/*Calculating the value of the Baud Rate Fine Adjustment*/
		SBR_BRFD = (uint16_t)(system_clk/(SDR * baud_rate));
		/** Set UART Baud Rate Bits (SBR) bits to zero **/
		UART3->BDH &= ~(UART_BDH_SBR(UART_BDH_SBR_MASK));
		/** Set SBR buffer data in temporary location **/
		UART3->BDH |= ((SBR_BRFD & UART_BDH_SBR_MASK) >> (shift_8bit));
		/** Put the SBR buffer data in permanent location **/
		UART3->BDL = (uint8_t) (SBR_BRFD & UART_BDL_SBR_MASK);
		/** The parity bit is the tenth bit in the serial transmission **/
		UART3->C4 |= UART_C4_BRFA_MASK;
		/** Set baud rate fine adjust: adding more timing resolution to baud freq.**/
		UART3->C4 = SBR_BRFD;
		/** Enabling transmission and reception **/
		UART3->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
		/** Baud rate generation enabled **/
    break;
    case UART_4:
		/** Activate UART 0 clock gating **/
		SIM->SCGC1 = SIM_SCGC1_UART4_MASK;
		/** Disabling transmission and reception **/
		UART4->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		/*Calculating the value of the Baud Rate Fine Adjustment*/
		SBR_BRFD = (uint16_t)(system_clk/(SDR * baud_rate));
		/** Set Uart Baud Rate Bits (SBR) bits to zero **/
		UART4->BDH &= ~(UART_BDH_SBR(UART_BDH_SBR_MASK));
		/** Set SBR buffer data in temporary location **/
		UART4->BDH |= ((SBR_BRFD & UART_BDH_SBR_MASK) >> (shift_8bit));
		/** Put the SBR buffer data in permanent location **/
		UART4->BDL = (uint8_t) (SBR_BRFD & UART_BDL_SBR_MASK);
		/** The parity bit is the tenth bit in the serial transmission **/
		UART4->C4 |= UART_C4_BRFA_MASK;
		/** Set baud rate fine adjust: adding more timing resolution to baud freq.**/
		UART4->C4 = SBR_BRFD;
		/** Enabling transmission and reception **/
		UART4->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
    /** Baud rate generation enabled **/
    break;
    case UART_5:
    	/** Activate UART 0 clock gating **/
		SIM->SCGC1 = SIM_SCGC1_UART5_MASK;
		/** Disabling transmission and reception **/
		UART5->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK );
		/*Calculating the value of the Baud Rate Fine Adjustment*/
		SBR_BRFD = (uint16_t)(system_clk/(SDR * baud_rate));
		/** Set Uart Baud Rate Bits (SBR) bits to zero **/
		UART5->BDH &= ~(UART_BDH_SBR(UART_BDH_SBR_MASK));
		/** Set SBR buffer data in temporary location **/
		UART5->BDH |= ((SBR_BRFD & UART_BDH_SBR_MASK) >> (shift_8bit));
		/** Put the SBR buffer data in permanent location **/
		UART5->BDL = (uint8_t) (SBR_BRFD & UART_BDL_SBR_MASK);
		/** The parity bit is the tenth bit in the serial transmission **/
		UART5->C4 |= UART_C4_BRFA_MASK;
		/** Set baud rate fine adjust: adding more timing resolution to baud freq.**/
		UART5->C4 = SBR_BRFD;
		/** Enabling transmission and reception **/
		UART5->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
		/** Baud rate generation enabled **/
    break;
	default:
	break;
  }
}

void UART_interrupt_enable(uart_channel_t uart_channel)
{
	//** Receiver Full Interrupt or DMA Transfer Enable  **/
	switch(uart_channel)
	{
  	  /** Enabling Interrupt or DMA Transfer Requests for UART 0**/
	  case UART_0:
		  UART0->C2 |= UART_C2_RIE_MASK;
	  break;
	  /** Enabling Interrupt or DMA Transfer Requests for UART 1**/
	  case UART_1:
		  UART1->C2 |= UART_C2_RIE_MASK;
	  break;
	  /** Enabling Interrupt or DMA Transfer Requests for UART 2**/
	  case UART_2:
		  UART2->C2 |= UART_C2_RIE_MASK;
	  break;
  	  /** Enabling Interrupt or DMA Transfer Requests for UART 3**/
	  case UART_3:
		  UART3->C2 |= UART_C2_RIE_MASK;
	  break;
  	  /** Enabling Interrupt or DMA Transfer Requests for UART 4**/
	  case UART_4:
		  UART4->C2 |= UART_C2_RIE_MASK;
	  break;
	  /** Enabling Interrupt or DMA Transfer Requests for UART 5**/
	  case UART_5:
		  UART5->C2 |= UART_C2_RIE_MASK;
	  break;
	  default:
	  break;
	}
}


void UART_put_char(uart_channel_t uart_channel, uint8_t character)
{
	switch (uart_channel)
	{
	case UART_0:
		/** Wait until transmission is completed **/
		UART_Wait_Transmit_Complete();
		/** Send character to data buffer **/
		UART_Set_Receiver_Data_Buffer(uart_channel, character);
	break;
	case UART_1:
		/** Wait until transmission is completed **/
		UART_Wait_Transmit_Complete();
		/** Send character to data buffer **/
		UART_Set_Receiver_Data_Buffer(uart_channel, character);
	break;
	case UART_2:
		/** Wait until transmission is completed **/
		UART_Wait_Transmit_Complete();
		/** Send character to data buffer **/
		UART_Set_Receiver_Data_Buffer(uart_channel, character);
	break;
	case UART_3:
		/** Wait until transmission is completed **/
		UART_Wait_Transmit_Complete();
		/** Send character to data buffer **/
		UART_Set_Receiver_Data_Buffer(uart_channel, character);
	break;
	case UART_4:
		/** Wait until transmission is completed **/
		UART_Wait_Transmit_Complete();
		/** Send character to data buffer **/
		UART_Set_Receiver_Data_Buffer(uart_channel, character);
	break;
	case UART_5:
		/** Wait until transmission is completed **/
		UART_Wait_Transmit_Complete();
		/** Send character to data buffer **/
		UART_Set_Receiver_Data_Buffer(uart_channel, character);
	break;
	default:
	break;
	}
}

void UART_put_string(uart_channel_t uart_channel, int8_t* string)
{
	switch(uart_channel)
	{
	case UART_0:
		UART_Data_Transmit(uart_channel, string);
	break;
	case UART_1:
		UART_Data_Transmit(uart_channel, string);
	break;
	case UART_2:
		UART_Data_Transmit(uart_channel, string);
	break;
	case UART_3:
		UART_Data_Transmit(uart_channel, string);
	break;
	case UART_4:
		UART_Data_Transmit(uart_channel, string);
	break;
	case UART_5:
		UART_Data_Transmit(uart_channel, string);
	break;
	default:
	break;
	}
}


static void UART_Wait_Transmit_Complete(void)
{
	boolean_t ver_transmission;

	do
	{
		/** Verify if transmitter state is idle (transmission activity complete **/
		ver_transmission = ((UART0->S1) && (UART_S1_TC_MASK));
	}
	while(FALSE == ver_transmission);
}

static void UART_Set_Receiver_Data_Buffer(uart_channel_t uart_channel, uint8_t character)
{
	switch(uart_channel)
	{
	case UART_0:
		UART0->D = character;
	break;
	case UART_1:
		UART1->D = character;
	break;
	case UART_2:
		UART2->D = character;
	break;
	case UART_3:
		UART3->D = character;
	break;
	case UART_4:
		UART4->D = character;
	break;
	case UART_5:
		UART5->D = character;
	break;
	default:
	break;
	}
}

static uint8_t UART_Get_Receiver_Data_Buffer(uart_channel_t uart_channel)
{
	uint8_t data_buffer;

	switch(uart_channel)
	{
	case UART_0:
		data_buffer = UART0->D;
	break;
	case UART_1:
		data_buffer = UART1->D;
	break;
	case UART_2:
		data_buffer = UART2->D;
	break;
	case UART_3:
		data_buffer = UART3->D;
	break;
	case UART_4:
		data_buffer = UART4->D;
	break;
	case UART_5:
		data_buffer = UART5->D;
	break;
	default:
	break;
	}

	return data_buffer;
}

static void UART_Data_Transmit(uart_channel_t uart_channel, int8_t* data_string)
{
	while(*data_string)
	{
		/*Keeps sending char until ptr is null*/
		UART_put_char(uart_channel,*data_string++);
	}
}


uint8_t UART_get_mailbox(uart_channel_t uart_channel)
{
	uint8_t flag;
	switch(uart_channel)
	{
	case UART_0:
		flag = g_mail_box_uart_0.mail_box;
	break;
	case UART_1:
		flag = g_mail_box_uart_1.mail_box;
	break;
	case UART_2:
		flag = g_mail_box_uart_2.mail_box;
	break;
	case UART_3:
		flag = g_mail_box_uart_3.mail_box;
	break;
	case UART_4:
		flag = g_mail_box_uart_4.mail_box;
	break;
	case UART_5:
		flag = g_mail_box_uart_5.mail_box;
	break;
	default:
	break;
	}

	return flag;
}

void UART_empty_mailbox(uart_channel_t uart_channel)
{
	switch(uart_channel)
	{
	case UART_0:
		g_mail_box_uart_0.flag = 0;
		g_mail_box_uart_0.mail_box = 0;
	break;
	case UART_1:
		g_mail_box_uart_1.flag = 0;
		g_mail_box_uart_1.mail_box = 0;
	break;
	case UART_2:
		g_mail_box_uart_2.flag = 0;
		g_mail_box_uart_2.mail_box = 0;
	break;
	case UART_3:
		g_mail_box_uart_3.flag = 0;
		g_mail_box_uart_3.mail_box = 0;
	break;
	case UART_4:
		g_mail_box_uart_4.flag = 0;
		g_mail_box_uart_4.mail_box = 0;
	break;
	case UART_5:
		g_mail_box_uart_5.flag = 0;
		g_mail_box_uart_5.mail_box = 0;
	break;
	default:
	break;
	}
}

boolean_t UART_get_mailbox_flag(uart_channel_t uart_channel)
{
	uint8_t flag;
	switch(uart_channel)
	{
	case UART_0:
		flag = g_mail_box_uart_0.flag;
	break;
	case UART_1:
		flag = g_mail_box_uart_1.flag;
	break;
	case UART_2:
		flag = g_mail_box_uart_2.flag;
	break;
	case UART_3:
		flag = g_mail_box_uart_3.flag;
	break;
	case UART_4:
		flag = g_mail_box_uart_4.flag;
	break;
	case UART_5:
		flag = g_mail_box_uart_5.flag;
	break;
	default:
	break;
	}

	return flag;
}

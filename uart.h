/**
	\file 	  uart.h
	\brief
			  This is the header file for the UART device driver for Kinetis K64.
	\authors: César Villarreal Hernández, ie707560
	          José Luis Rodríguez Gutierrez,ie705694
	\date	  08/04/2019
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include "MK64F12.h"
#include "bits.h"

#define SDR 16 /** Serial Data Rate **/
#define shift_8bit 8

typedef struct{
	boolean_t flag; /** Flag to indicate that there is new data*/
	uint8_t mail_box; /** it contains the received data*/
} uart_mail_box_t;

/** This enumeration defines the UART's ports to be used **/
typedef enum {UART_0,UART_1,UART_2,UART_3,UART_4,UART_5} uart_channel_t;

/** This enumeration defines some common transmission baud rates **/
typedef enum {BD_4800 = 4800,BD_9600 = 9600,BD_5600 = 5600, BD_115200 = 115200} baud_rate_t;


/*!
 	 \brief	 It configures the UART to be used
 	 \param[in]  uartChannel indicates which UART will be used.
 	 \param[in]  systemClk indicates the MCU frequency.
 	 \param[in]  baudRate sets the baud rate to transmit.
 	 \return void
 */
void UART_init(uart_channel_t uart_channel, uint32_t system_clk, baud_rate_t baud_rate);

/*!
 	 \brief	 enables the RX UART interrupt). This function should include the next sentence:
 	 while (!(UART0_S1 & UART_S1_RDRF_MASK)). It is to guaranty that the incoming data is complete
 	 when reception register is read. For more details see chapter 52 in the kinetis reference manual.
 	 \param[in]  uartChannel indicates the UART channel.
 	 \return void
 */
void UART_interrupt_enable(uart_channel_t uart_channel);


/*!
 	 \brief	 It sends one character through the serial port. This function should include the next sentence:
 	 while(!(UART0_S1 & UART_S1_TC_MASK)). It is to guaranty that before to try to transmit a byte, the previous
 	 one was transmitted. In other word, to avoid to transmit data while the UART is busy transmitting information.
 	 \param[in]  uartChannel indicates the UART channel.
 	 \param[in]  character to be transmitted.
 	 \return void
 */

void UART_put_char (uart_channel_t uart_channel, uint8_t character);

/*!
 	 \brief	 It sends a string character through the serial port.
 	 \param[in]  uartChannel indicates the UART channel.
 	 \param[in]  string pointer to the string to be transmitted.
 	 \return void
 */
void UART_put_string(uart_channel_t uart_channel, int8_t* string);

/*!
 	 \brief			 It waits until transmission is complete.
 	 \param[in]  void
 	 \return     static void
 */
static void UART_Wait_Transmit_Complete(void);

/*!
 	 \brief			 It sets the receiver data buffer with a given character.
 	 \param[in]  void
 	 \return     static void
 */
static void UART_Set_Receiver_Data_Buffer(uart_channel_t uart_channel, uint8_t character);

/*!
 	 \brief		 It gets the current receiver data buffer.
 	 \param[in]  void
 	 \return     static uint8_t
 */

static uint8_t UART_Get_Receiver_Data_Buffer(uart_channel_t uart_channel);

/*!
 	 \brief		 It transmits the data to a specific UART channel.
 	 \param[in]  void
 	 \return     static void
 */
static void UART_Data_Transmit(uart_channel_t uart_channel, int8_t* data_string);

/*!
 	 \brief		 It gets the current mailbox value.
 	 \param[in]  uart_channel_t
 	 \return     uint8_t 
 */
uint8_t UART_get_mailbox(uart_channel_t uart_channel);

/*!
 	 \brief		 This function clears the mailbox's flag and value.
 	 \param[in]  uart_channel_t
 	 \return     void 
 */
void UART_empty_mailbox(uart_channel_t uart_channel);

/*!
 	 \brief		 It gets the current mailbox flag state
 	 \param[in]  uart_channel_t
 	 \return     uint8_t 
 */
boolean_t UART_get_mailbox_flag(uart_channel_t uart_channel);

#endif /* UART_H_ */

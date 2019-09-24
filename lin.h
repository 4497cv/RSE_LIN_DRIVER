/*
	\file lin.h
	\brief	This is the source file for the LIN device driver for Kinetis K64.
	
	\authors César Villarreal, @4497cv
			 Tsipini Franco,
			 Moisés López,
	\date	 24/09/2019
*/
#include "uart.h"

#ifndef LIN_H_
#define LIN_H_

typedef enum
{
	dominant,
	recesive
} signal_state;

typedef struct
{
	uart_channel_t uart_channel;
} lin_config_t;

typedef enum 
{
	BD_2400 = 2400,
	BD_9600 = 9600,
	BD_19200 = 19200
} lin_baud_rate_t;

typedef struct 
{
	
};


#endif
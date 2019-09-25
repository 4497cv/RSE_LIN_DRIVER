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

#define HEADER_ST 3

typedef enum
{
	DOMINANT = 0,
	RECESIVE = 1
} signal_state_t;

typedef enum
{
	SLAVE,
	MASTER
} lin_operation_mode_t

typedef enum
{
	synch_break,
	synch_field,
	ident_field
} lin_header_t;

typedef struct
{
	void(*fptr)(void);
	lin_header_t next[3];
} FSM_master_t;

typedef struct
{
	uart_channel_t uart_channel; //uart's channel
	uint32_t system_clk;
	lin_baud_rate_t baud_rate;
	lin_operation_mode_t operation_mode; //indicate if slave or master
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
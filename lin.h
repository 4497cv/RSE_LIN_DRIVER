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
#define MSG_PARITY_MASK (0X03U)
#define SYNC_FIELD_MASK (0x55U)
#define MSG_ID_MASK     (0x3FU)

typedef enum
{
	DOMINANT = 0,
	RECESIVE = 1
} signal_state_t;

typedef enum
{
	SLAVE,
	MASTER
} lin_operation_mode_t;

typedef enum
{
	RID0 = 0X3C,
	RID1 = 0X3D,
	RID2 = 0X3E,
	RID3 = 0X3F,
	RID4 = 0XBF,
	RID5 = 0XFE,
} reserved_id;

typedef enum
{
	synch_break,
	synch_field,
	ident_field
} lin_header_st;

typedef struct
{
	void(*fptr)(void);
	lin_header_st next[3];
} FSM_master_t;

typedef enum
{
	LIN_BD_2400 = 2400,
	LIN_BD_9600 = 9600,
	LIN_BD_19200 = 19200
} lin_baud_rate_t;

typedef struct
{
	uart_channel_t uart_channel; //uart's channel
	uint32_t system_clk;
	lin_baud_rate_t baud_rate;
	lin_operation_mode_t operation_mode; //indicate if slave or master
} lin_config_t;


boolean_t is_identifier_valid(uint8_t message_id);
void LIN_IDENT_FIELD(uint8_t message_id, uint8_t message_parity);
void LIN_SYNC_FIELD();
void LIN_SYNC_BREAK();
void LIN_SEND_MESSAGE_HEADER(); 
void LIN_init(const lin_config_t* LIN_config);

#endif

/*
	\file lin.h
	\brief	This is the source file for the LIN device driver for Kinetis K64.
	
	\authors César Villarreal, @4497cv
			 Tsipini Franco,
			 Moisés López,
	\date	 24/09/2019
*/
#include <stdio.h>
#include "MK64F12.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "rtos_uart.h"
#include "bits.h"
#include "pin_mux.h"
#include "fsl_port.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "ADC.h"

#ifndef LIN_H_
#define LIN_H_

/* Definitions */
#define SYSTEM_CLOCK     (21000000U)
#define MESSAGE_ID       (0X48U)
#define MESSAGE_PARITY   (0X00U)
#define LOW_PHASE_VAL_1  (0X00U)
#define LOW_PHASE_VAL_2  (0XE0U)
#define HEADER_ST   3
#define RESPONSE_ST 2
#define MSG_PARITY_MASK (0X03U)
#define SYNC_FIELD_MASK (0x55)
#define MSG_ID_MASK     (0x3FU)
#define N_DATA 1
#define DATA_LENGTH 	  1


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

typedef enum
{
	send_data,
	check_sum
} lin_response_st;

typedef struct
{
	void(*fptr)(void);
	lin_header_st next[3];
} fsm_header_t;

typedef struct
{
	void(*fptr)(void);
	lin_response_st next[3];
} fsm_response_t;

typedef enum
{
	LIN_BD_2400 = 2400,
	LIN_BD_9600 = 9600,
	LIN_BD_19200 = 19200
} lin_baud_rate_t;

typedef struct
{
	lin_baud_rate_t baud_rate;
	lin_operation_mode_t operation_mode; //indicate if slave or master
	uint8_t message_id;
	uint8_t message_parity;
} lin_config_t;


boolean_t is_identifier_valid(uint8_t message_id);
static void LIN_IDENT_FIELD();
static void LIN_SYNC_FIELD();
static void LIN_SYNC_BREAK();
void LIN_init(lin_config_t* LIN_config);
boolean_t is_header_valid(uint8_t buffer_0, uint8_t buffer_1, uint8_t buffer_2);

void LIN_SEND_DATA(void);
void LIN_CHECKSUM(void);

void LIN_MESSAGE_HEADER_THREAD(void* args);
void LIN_MESSAGE_RESPONSE_THREAD(void* args);

#endif

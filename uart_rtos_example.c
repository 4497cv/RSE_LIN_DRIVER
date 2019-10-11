/*
 * Copyright (c) 2017, NXP Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    uart_rtos.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK64F12.h"
#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_port.h"
#include "rtos_uart.h"

//void uart_echo_task(void * args)
//{
//	rtos_uart_config_t config;
//	config.baudrate = 115200;
//	config.rx_pin = 16;
//	config.tx_pin = 17;
//	config.pin_mux = kPORT_MuxAlt3;
//	config.uart_number = rtos_uart0;
//	config.port = rtos_uart_portB;
//
//	rtos_uart_init(config);
//
//	uint8_t data = 0x55;
//
//	for(;;)
//	{
//		//rtos_uart_receive(rtos_uart0, &data, 1);
//
//	}
//}


void uart_node_1_task()
{
	uint8_t data = 'c';

	for(;;)
	{
		//rtos_uart_receive(rtos_uart3, &data, 1);
		rtos_uart_send(rtos_uart3, &data, 1);
	}
}

void uart_node_2_task()
{
	uint8_t data;

	for(;;)
	{
		rtos_uart_receive(rtos_uart3, &data, 1);
		//rtos_uart_send(rtos_uart3, &data, 1);
		rtos_uart_send(rtos_uart0, &data, 1);
		vTaskDelay(100);
	}
}

//int main(void)
//{
//	BOARD_BootClockRUN();
//
//	//xTaskCreate(uart_echo_task, "uart_echo_task", 110, NULL, 1, NULL);
//	xTaskCreate(uart_node_2_task, "node2", 110, NULL, 1, NULL);
//	vTaskStartScheduler();
//
//	for(;;);
//	return 0;
//}

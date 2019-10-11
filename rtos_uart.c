/*
 * rtos_uart.c
 *
 *  Created on: Sep 12, 2018
 *      Author:
 */



#include "rtos_uart.h"

#include "fsl_uart.h"
#include "fsl_clock.h"
#include "fsl_port.h"

#include "FreeRTOS.h"
#include "semphr.h"
#define NUMBER_OF_SERIAL_PORTS (4)

static inline void enable_port_clock(rtos_uart_port_t);
static inline UART_Type * get_uart_base(rtos_uart_number_t);
static inline PORT_Type * get_port_base(rtos_uart_port_t);
static void fsl_uart_callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData);

typedef struct
{
	uint8_t is_init;
	uart_handle_t fsl_uart_handle;
	SemaphoreHandle_t mutex_rx;
	SemaphoreHandle_t mutex_tx;
	SemaphoreHandle_t rx_sem;
	SemaphoreHandle_t tx_sem;
}rtos_uart_hanlde_t;

static rtos_uart_hanlde_t uart_handles[NUMBER_OF_SERIAL_PORTS] = {0};

static void fsl_uart_callback(UART_Type *base, uart_handle_t *handle, status_t status, void *userData)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if (kStatus_UART_TxIdle == status)
	{
		if(UART0 == base)
		{
			xSemaphoreGiveFromISR(uart_handles[rtos_uart0].tx_sem, &xHigherPriorityTaskWoken );
		}
		else if(UART1 == base)
		{
			xSemaphoreGiveFromISR(uart_handles[rtos_uart1].tx_sem, &xHigherPriorityTaskWoken );
		}
		else if(UART3 == base)
		{
			xSemaphoreGiveFromISR(uart_handles[rtos_uart3].tx_sem, &xHigherPriorityTaskWoken );
		}
		else
		{

		}
	}

	if (kStatus_UART_RxIdle == status)
	{
		if(UART0 == base)
		{
			xSemaphoreGiveFromISR(uart_handles[rtos_uart0].rx_sem, &xHigherPriorityTaskWoken );
		}
		else if(UART1 == base)
		{
			xSemaphoreGiveFromISR(uart_handles[rtos_uart1].rx_sem, &xHigherPriorityTaskWoken );
		}
		else if(UART3 == base)
		{
			xSemaphoreGiveFromISR(uart_handles[rtos_uart3].rx_sem, &xHigherPriorityTaskWoken );
		}
		else
		{

		}

	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

rtos_uart_flag_t rtos_uart_init(rtos_uart_config_t config)
{
	rtos_uart_flag_t retval = rtos_uart_fail;
	uart_config_t fsl_config;

	if(config.uart_number < NUMBER_OF_SERIAL_PORTS)
	{
		if(!uart_handles[config.uart_number].is_init)
		{
			uart_handles[config.uart_number].mutex_rx = xSemaphoreCreateMutex();
			uart_handles[config.uart_number].mutex_tx = xSemaphoreCreateMutex();

			uart_handles[config.uart_number].rx_sem = xSemaphoreCreateBinary();
			uart_handles[config.uart_number].tx_sem = xSemaphoreCreateBinary();


			enable_port_clock(config.port);
			PORT_SetPinMux(get_port_base(config.port), config.rx_pin, config.pin_mux);
			PORT_SetPinMux(get_port_base(config.port), config.tx_pin, config.pin_mux);

			UART_GetDefaultConfig(&fsl_config);
			fsl_config.baudRate_Bps = config.baudrate;
			fsl_config.enableTx = true;
			fsl_config.enableRx = true;

			if(rtos_uart0 == config.uart_number)
			{
				UART_Init(get_uart_base(rtos_uart0), &fsl_config, CLOCK_GetFreq(UART0_CLK_SRC));
				NVIC_SetPriority(UART0_RX_TX_IRQn, 5);
			}
			else if(rtos_uart1 == config.uart_number)
			{
				UART_Init(get_uart_base(rtos_uart1), &fsl_config, CLOCK_GetFreq(UART1_CLK_SRC));
				NVIC_SetPriority(UART1_RX_TX_IRQn, 5);
			}
			else if(rtos_uart3 == config.uart_number)
			{
				UART_Init(get_uart_base(rtos_uart3), &fsl_config, CLOCK_GetFreq(UART3_CLK_SRC));
				NVIC_SetPriority(UART3_RX_TX_IRQn, 5);
			}
			else
			{

			}

			UART_TransferCreateHandle(get_uart_base(config.uart_number),
			&uart_handles[config.uart_number].fsl_uart_handle, fsl_uart_callback, NULL);

			uart_handles[config.uart_number].is_init = 1;
			retval = rtos_uart_sucess;
		}
	}
	return retval;
}

rtos_uart_flag_t rtos_uart_send(rtos_uart_number_t uart_number, uint8_t * buffer, uint16_t lenght)
{
	rtos_uart_flag_t flag = rtos_uart_fail;
	uart_transfer_t xfer;
	if(uart_handles[uart_number].is_init)
	{
		xfer.data= buffer;
		xfer.dataSize = lenght;
		xSemaphoreTake(uart_handles[uart_number].mutex_tx, portMAX_DELAY);
		UART_TransferSendNonBlocking(get_uart_base(uart_number), &uart_handles[uart_number].fsl_uart_handle, &xfer);

		xSemaphoreTake(uart_handles[uart_number].tx_sem,portMAX_DELAY);

		xSemaphoreGive(uart_handles[uart_number].mutex_tx);
		flag = rtos_uart_sucess;
	}
	return flag;
}

rtos_uart_flag_t rtos_uart_receive(rtos_uart_number_t uart_number, uint8_t * buffer, uint16_t lenght)
{
	uart_transfer_t xfer;

	if(uart_handles[uart_number].is_init)
	{
		xfer.data = buffer;
		xfer.dataSize = lenght;

		xSemaphoreTake(uart_handles[uart_number].mutex_rx, portMAX_DELAY);

		UART_TransferReceiveNonBlocking(
				get_uart_base(uart_number),
				&uart_handles[uart_number].fsl_uart_handle,
				&xfer,
				NULL);

		xSemaphoreTake(uart_handles[uart_number].rx_sem,portMAX_DELAY);

		xSemaphoreGive(uart_handles[uart_number].mutex_rx);
	}
}


static inline void enable_port_clock(rtos_uart_port_t port)
{
	switch(port)
	{
	case rtos_uart_portA:
		CLOCK_EnableClock(kCLOCK_PortA);
		break;
	case rtos_uart_portB:
		CLOCK_EnableClock(kCLOCK_PortB);
		break;
	case rtos_uart_portC:
		CLOCK_EnableClock(kCLOCK_PortC);
		break;
	case rtos_uart_portD:
		CLOCK_EnableClock(kCLOCK_PortD);
		break;
	case rtos_uart_portE:
		CLOCK_EnableClock(kCLOCK_PortE);
		break;
	}
}

static inline UART_Type * get_uart_base(rtos_uart_number_t uart_number)
{
	UART_Type * retval = UART0;

	switch(uart_number)
	{
		case rtos_uart0:
			retval = UART0;
		break;
		case rtos_uart1:
			retval = UART1;
		break;
		case rtos_uart3:
			retval = UART3;
		break;
		case rtos_uart4:
			retval = UART4;
		break;
	}

	return retval;
}

static inline PORT_Type * get_port_base(rtos_uart_port_t port)
{
	PORT_Type * port_base = PORTA;

	switch(port)
	{
	case rtos_uart_portA:
		port_base = PORTA;
		break;
	case rtos_uart_portB:
		port_base = PORTB;
		break;
	case rtos_uart_portC:
		port_base = PORTC;
		break;
	case rtos_uart_portD:
		port_base = PORTD;
		break;
	case rtos_uart_portE:
		port_base = PORTE;
		break;
	}

	return port_base;
}

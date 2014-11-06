#include <stm32f0xx.h>
#include <stdint.h>
#include "serial.h"
#include "eventqueue.h"

void USART1_IRQHandler()
{
	static uint8_t state = 0;
	volatile uint8_t c = 0;
	if (USART1->ISR & USART_ISR_RXNE)
		c = USART1->RDR;

	if (state == 0 && c == 'p')
		state = 1;
	else if (state == 1 && c == 'o')
		state = 2;
	else if (state == 2 && c == 'n')
		state = 3;
	else if (state == 3 && c == 'g')
		state = 4;
	else if (state == 4 && c == '\r')
		state = 5;
	else if ((state == 4 || state == 5) && c == '\n')
	{
		queue_put(EVENT_PONG);
	}
	else
	{
		state = 0;
	}

}

void serial_init()
{

}

void serial_send(char *s, uint32_t len)
{
	while(len--)
	{
		while((USART1->ISR & USART_ISR_TXE) == 0);
		USART1->TDR = *s++;
	}

}

void serial_send_string(char *s)
{
	while(*s != '\0')
	{
		while((USART1->ISR & USART_ISR_TXE) == 0);
		USART1->TDR = *s++;
	}
}

void serial_ping()
{
	serial_send_string("ping\r\n");
}

void serial_shutdown()
{
	serial_send_string("shutdown\r\n");
}

#include <stm32f0xx.h>
#include <stdint.h>
#include "serial.h"


void USART1_IRQHandler()
{
	volatile uint8_t c = 0;
	if (USART1->ISR & USART_ISR_RXNE)
		c = USART1->RDR;
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

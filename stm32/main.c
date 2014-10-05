#include <stm32f0xx.h>
#include "config.h"
#include "sysevent.h"
#include "led.h"

void uart_sendc(char c)
{
	while((USART1->ISR & USART_ISR_TXE) == 0);
	USART1->TDR = c;
}

void uart_sends(char *s)
{
	do
	{
		uart_sendc(*s);
	} while(*++s);
}


int main()
{

	configure();
	sysevent_init();
	led_init();
	led_set(LED_MODE_SHORT);

	uart_sends("Hello World!\r\n");

    while(1){

    }

    return 0;
}

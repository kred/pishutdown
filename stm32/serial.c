#include <stm32f0xx.h>
#include <stdint.h>
#include <core_cm0.h>
#include "serial.h"
#include "eventqueue.h"

#define COMMAND_LENGTH 16


volatile uint8_t serial_buffer[COMMAND_LENGTH];
volatile uint8_t serial_offset = 0;

typedef struct
{
	char command[COMMAND_LENGTH];
	EVENTS event;
} Command;

Command commands[] = {
		{ "pong", EVENT_PONG },
		{ "ack", EVENT_ACK },
};

static void shift_left(volatile uint8_t *buf)
{
    uint8_t i;
    for(i = 8; i < COMMAND_LENGTH; i++)
        buf[i-8] = buf[i];
    buf[COMMAND_LENGTH-1] = '\0';
}

static void clear(volatile uint8_t *buf)
{
    uint8_t i;
    for(i = 0; i < COMMAND_LENGTH; i++)
        buf[i] = '\0';
}

static uint8_t xstrlen(char *buf)
{
    uint8_t len = 0;
    while(*buf++ != '\0')
        len++;

    return len;
}

static uint8_t find_command(uint8_t *buf, EVENTS *event)
{
    uint8_t i;
    uint8_t j;
    uint8_t comm;
    uint8_t found;
    uint8_t len;

    for(comm = 0; comm < (sizeof(commands)/sizeof(Command)); comm++)
    {
        len = xstrlen(commands[comm].command);

        for(i = 0; i < COMMAND_LENGTH; i++)
        {
            found = 0;
            for(j = 0; j < len; j++)
            {
                if (i+j < COMMAND_LENGTH && buf[i+j] != '\0')
                {
                    if (buf[i+j] != commands[comm].command[j])
                        break;
                    else
                        found++;
                }
            }

            found = (found == len);

            if (found)
            {
            	*event = commands[comm].event;
                return found;
            }
        }

    }

    return found;
}

void USART1_IRQHandler()
{
	if (USART1->ISR & USART_ISR_RXNE)
	{
	    if (serial_offset == 0)
	        clear(serial_buffer);

	    serial_buffer[serial_offset++] = USART1->RDR;

        if (serial_offset == COMMAND_LENGTH)
        {
            shift_left(serial_buffer);
            serial_offset = COMMAND_LENGTH - 8;
        }
	}
}

void serial_check()
{
    EVENTS event;
    uint8_t buffer[COMMAND_LENGTH];
    uint8_t i;

    __disable_irq();
    for(i = 0; i < COMMAND_LENGTH; i++)
    	buffer[i] = serial_buffer[i];
    __enable_irq();
    __ISB();

    if (serial_offset)
    {
    	if (find_command(buffer, &event) == 1)
    	{
    		queue_put(event);
    		serial_offset = 0;
    	}
    }

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

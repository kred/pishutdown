#include <stm32f0xx.h>
#include <stdint.h>
#include "serial.h"
#include "eventqueue.h"

#define COMMAND_LENGTH 16

typedef struct
{
	char command[COMMAND_LENGTH];
	EVENTS event;
} Command;

Command commands[] = {
		{ "pong", EVENT_PONG },
		{ "ack", EVENT_ACK },
};


static void shift_left(uint8_t *buf)
{
    uint8_t i;
    for(i = 1; i < COMMAND_LENGTH; i++)
        buf[i-1] = buf[i];
    buf[COMMAND_LENGTH-1] = '\0';
}

static void clear(uint8_t *buf)
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
                break;
        }

        if (found)
            break;
    }

    *event = commands[comm].event;

    return found;
}

void USART1_IRQHandler()
{
    static uint8_t buf[COMMAND_LENGTH];
    static uint8_t i = 0;
    EVENTS event;


	if (USART1->ISR & USART_ISR_RXNE)
	{
	    if (i == 0)
	        clear(buf);

	    buf[i++] = USART1->RDR;

	    if (find_command(buf, &event) == 1)
	    {
			queue_put(event);
	        i = 0;
	    }
	    else
	    {
	        if (i == COMMAND_LENGTH)
	        {
	            shift_left(buf);
	            i = COMMAND_LENGTH - 1;
	        }
	    }
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

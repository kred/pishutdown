#include <stm32f0xx.h>
#include <core_cm0.h>
#include "eventqueue.h"

#define QUEUE_SIZE 8

volatile EVENTS queue_events[QUEUE_SIZE];
volatile uint32_t queue_first;
volatile uint32_t queue_last;
volatile uint32_t queue_size;


void queue_init()
{
	uint32_t i;
	for(i = 0; i < QUEUE_SIZE; i++)
		queue_events[i] = EVENT_NONE;

	queue_first = 0;
	queue_last = 0;
	queue_size = 0;
}

EVENTS queue_put(EVENTS event)
{
	__disable_irq();

	if (queue_size < QUEUE_SIZE)
	{
		queue_events[queue_last] = event;
		queue_size++;
		if (++queue_last == QUEUE_SIZE)
			queue_last = 0;
	}
	else
	{
		event = EVENT_NONE;
	}
	__enable_irq();
	__ISB();

	return event;
}

EVENTS queue_get()
{
	__disable_irq();
	EVENTS event = EVENT_NONE;

	if (queue_size > 0)
	{
		event = queue_events[queue_first];
		queue_events[queue_first] = EVENT_NONE;
		queue_size--;
		if (++queue_first == QUEUE_SIZE)
			queue_first = 0;
	}
	__enable_irq();
	__ISB();

	return event;
}

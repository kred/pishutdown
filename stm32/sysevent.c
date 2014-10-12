#include <stdint.h>
#include "sysevent.h"

struct Event systick_events[5];
volatile uint32_t systick_registered_events = 0;
volatile uint32_t delay_counter = 0;
volatile uint32_t delay_value = 0;
volatile uint32_t delayevent_counter = 0;
volatile uint32_t delayevent_value = 0;
volatile EVENTS delayevent_event = EVENT_NONE;


void delay_handler()
{
	if (delay_value > 0)
		delay_counter++;
}

void delayevent_handler()
{
	if (delayevent_value > 0)
	{
		if (++delayevent_counter == delayevent_value)
		{
			queue_put(delayevent_event);
			delayevent_value = 0;
		}
	}
}


void sysevent_init()
{
	sysevent_add(delay_handler, 1);
	sysevent_add(delayevent_handler, 1);
}

void sysevent_add(EventHandler handler, uint32_t ms)
{
	systick_events[systick_registered_events].handler = handler;
	systick_events[systick_registered_events].ms = ms;
	systick_registered_events++;
}

void delay_ms(uint32_t ms)
{
	delay_counter = 0;
	delay_value = ms;

	while(delay_counter < delay_value);
}

void delay_event(uint32_t ms, EVENTS event)
{
	delayevent_event = event;
	delayevent_counter = 0;
	delayevent_value = ms;
}

void delay_event_cancel()
{
	delayevent_value = 0;
}

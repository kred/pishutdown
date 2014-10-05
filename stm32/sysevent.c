#include <stdint.h>
#include "sysevent.h"

struct Event systick_events[5];
volatile uint32_t systick_registered_events = 0;
volatile uint32_t delay_counter;
volatile uint32_t delay_value;


void delay_handler()
{
	if (delay_value > 0)
		delay_counter++;
}



void sysevent_init()
{
	sysevent_add(delay_handler, 1);
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

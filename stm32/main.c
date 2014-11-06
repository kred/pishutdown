#include <stm32f0xx.h>
#include "config.h"
#include "eventqueue.h"
#include "sysevent.h"
#include "led.h"
#include "serial.h"

#define PI_POLL_VALUE 24 // how many times RPi will be 'ping' before switch off
#define PI_POLL_DELAY_VALUE 5000 // 5000ms between pings
#define PI_SHUTDOWN_DELAY_VALUE 120000 // 120000ms delay before shutdown

typedef enum
{
	STATE_PI_DISABLED = 0,
	STATE_ENABLE_PI,
	STATE_POLL_PI,
	STATE_PI_ENABLED,
	STATE_DISABLE_PI,
	STATE_DISABLE_PI_DELAYED,

} STATES;

int main()
{
	EVENTS event;
	STATES state = STATE_PI_DISABLED;
	uint32_t pi_poll_counter = 0;

	configure();
	queue_init();
	sysevent_init();
	serial_init();
	led_init();

	while(1)
	{
		event = queue_get();

		switch(state)
		{
		case STATE_PI_DISABLED:
			if (event == EVENT_IGNITION_ON)
			{
				state = STATE_ENABLE_PI;
			}
			break;

		case STATE_ENABLE_PI:
			PI_ON;
			led_set(LED_MODE_SHORT);
			pi_poll_counter = 0;
			delay_event(PI_POLL_DELAY_VALUE, EVENT_PING);
			state = STATE_POLL_PI;
			break;

		case STATE_POLL_PI:
			if (event == EVENT_PONG)
			{
				state = STATE_PI_ENABLED;
			}
			else if (event == EVENT_PING)
			{
				if (++pi_poll_counter == PI_POLL_VALUE)
				{
					state = STATE_DISABLE_PI;
				}
				else
				{
					serial_ping();
					delay_event(PI_POLL_DELAY_VALUE, EVENT_PING);
				}
			}
			else if (event == EVENT_IGNITION_OFF)
			{
				led_set(LED_MODE_FAST);
				delay_event(PI_SHUTDOWN_DELAY_VALUE, EVENT_DISABLE_PI);
				state = STATE_DISABLE_PI_DELAYED;
			}
			break;

		case STATE_PI_ENABLED:
			pi_poll_counter = 0;
			led_set(LED_MODE_SLOW);
			state = STATE_POLL_PI;
			break;

		case STATE_DISABLE_PI:
			PI_OFF;
			led_set(LED_MODE_OFF);
			state = STATE_PI_DISABLED;
			break;

		case STATE_DISABLE_PI_DELAYED:
			if (event == EVENT_DISABLE_PI)
			{
				state = STATE_DISABLE_PI;
			}
			else if (event == EVENT_IGNITION_ON)
			{
				state = STATE_PI_ENABLED;
			}
			break;

		}
	}

    return 0;
}

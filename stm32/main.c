#include <stm32f0xx.h>
#include "config.h"
#include "eventqueue.h"
#include "sysevent.h"
#include "led.h"
#include "serial.h"

int main()
{
	EVENTS event;

	configure();
	queue_init();
	sysevent_init();
	serial_init();
	led_init();


	queue_put(EVENT_ENABLE_PI);

    while(1){
    	event = queue_get();

    	switch(event)
    	{
    	case EVENT_NONE: break;
    	case EVENT_PING: break;
    	case EVENT_PONG: break;
    	case EVENT_ENABLE_PI:
    		PI_ON;
    		led_set(LED_MODE_SHORT);
    		delay_event(5000, EVENT_DISABLE_PI);
    		break;
    	case EVENT_DISABLE_PI:
    		PI_OFF;
    		led_set(LED_MODE_LONG);
    		break;
    	}


    }

    return 0;
}

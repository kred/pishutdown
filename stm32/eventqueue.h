#ifndef EVENTQUEUE_H__
#define EVENTQUEUE_H__

#include <stdint.h>

typedef enum
{
	EVENT_NONE = 0,
	EVENT_PING,
	EVENT_PONG,
	EVENT_ACK,
	EVENT_ENABLE_PI,
	EVENT_DISABLE_PI,
	EVENT_IGNITION_ON,
	EVENT_IGNITION_OFF,
	EVENT_SCREEN_ON,
	EVENT_SCREEN_OFF,
} EVENTS;


void queue_init();
EVENTS queue_put(EVENTS event);
EVENTS queue_get();


#endif


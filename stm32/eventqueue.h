#ifndef EVENTQUEUE_H__
#define EVENTQUEUE_H__

#include <stdint.h>

typedef enum
{
	EVENT_NONE = 0,
	EVENT_PING,
	EVENT_PONG,
	EVENT_ENABLE_PI,
	EVENT_DISABLE_PI,
} EVENTS;


void queue_init();
EVENTS queue_put(EVENTS event);
EVENTS queue_get();


#endif


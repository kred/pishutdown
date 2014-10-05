#ifndef SYSEVENT_H_
#define SYSEVENT_H_

typedef void (*EventHandler)(void);
struct Event {
	EventHandler handler;
	uint32_t ms;
};


void sysevent_init();
void sysevent_add(EventHandler handler, uint32_t ms);

void delay_ms(uint32_t ms);


#endif /* SYSEVENT_H_ */

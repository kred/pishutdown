#ifndef LED_H_
#define LED_H_

typedef enum
{
	LED_MODE_OFF = 0,
	LED_MODE_ON,
	LED_MODE_SLOW,
	LED_MODE_MIDDLE,
	LED_MODE_FAST,
	LED_MODE_SHORT,
	LED_MODE_LONG
} LED_MODE;


void led_init();
void led_set(LED_MODE mode);


#endif /* LED_H_ */

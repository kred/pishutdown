#include <stm32f0xx.h>
#include <stdint.h>
#include "config.h"
#include "sysevent.h"
#include "led.h"

volatile uint32_t led_mode;
volatile uint32_t led_counter;

struct Led_Mode_Threshold
{
	uint32_t led_on_threshold;
	uint32_t led_off_threshold;
	uint32_t reload_threshold;
} led_mode_threshold[7];


void led_handler()
{
	if (led_counter == led_mode_threshold[led_mode].led_on_threshold)
		LED_ON;
	if (led_counter == led_mode_threshold[led_mode].led_off_threshold)
		LED_OFF;
	if (++led_counter == led_mode_threshold[led_mode].reload_threshold)
		led_counter = 0;
}


void led_init()
{
	led_counter = 0;
	led_mode = LED_MODE_OFF;

	led_mode_threshold[LED_MODE_OFF].led_on_threshold = 10001;
	led_mode_threshold[LED_MODE_OFF].led_off_threshold = 0;
	led_mode_threshold[LED_MODE_OFF].reload_threshold = 10000;

	led_mode_threshold[LED_MODE_ON].led_on_threshold = 0;
	led_mode_threshold[LED_MODE_ON].led_off_threshold = 10001;
	led_mode_threshold[LED_MODE_ON].reload_threshold = 10000;

	led_mode_threshold[LED_MODE_SLOW].led_on_threshold = 0;
	led_mode_threshold[LED_MODE_SLOW].led_off_threshold = 500;
	led_mode_threshold[LED_MODE_SLOW].reload_threshold = 1000;

	led_mode_threshold[LED_MODE_MIDDLE].led_on_threshold = 0;
	led_mode_threshold[LED_MODE_MIDDLE].led_off_threshold = 250;
	led_mode_threshold[LED_MODE_MIDDLE].reload_threshold = 500;

	led_mode_threshold[LED_MODE_FAST].led_on_threshold = 0;
	led_mode_threshold[LED_MODE_FAST].led_off_threshold = 100;
	led_mode_threshold[LED_MODE_FAST].reload_threshold = 200;

	led_mode_threshold[LED_MODE_SHORT].led_on_threshold = 0;
	led_mode_threshold[LED_MODE_SHORT].led_off_threshold = 50;
	led_mode_threshold[LED_MODE_SHORT].reload_threshold = 1000;

	led_mode_threshold[LED_MODE_LONG].led_on_threshold = 0;
	led_mode_threshold[LED_MODE_LONG].led_off_threshold = 950;
	led_mode_threshold[LED_MODE_LONG].reload_threshold = 1000;

	sysevent_add(led_handler, 1);
}

void led_set(LED_MODE mode)
{
	if (mode != led_mode)
	{
		led_mode = mode;
		led_counter = 0;
	}
}



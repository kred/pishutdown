#include <stm32f0xx.h>
#include "config.h"
#include "sysevent.h"
#include "led.h"
#include "serial.h"

int main()
{

	configure();
	sysevent_init();
	serial_init();
	led_init();

	led_set(LED_MODE_SHORT);

    while(1){

    }

    return 0;
}

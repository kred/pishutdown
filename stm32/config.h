
#ifndef __CONFIG_H
#define __CONFIG_H

#define LED_ON GPIOF->BSRR |= GPIO_BSRR_BS_0
#define LED_OFF GPIOF->BRR |= GPIO_BRR_BR_0

#define PI_ON GPIOB->BRR |= GPIO_BRR_BR_1
#define PI_OFF GPIOB->BSRR |= GPIO_BSRR_BS_1

#define IGNITION_SET (GPIOF->IDR & GPIO_IDR_1)

void configure();


#endif

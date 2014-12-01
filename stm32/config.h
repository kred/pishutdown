
#ifndef __CONFIG_H
#define __CONFIG_H

#include <stm32f0xx.h>
#include <core_cm0.h>

#define LED_ON GPIOF->BSRR |= GPIO_BSRR_BS_0
#define LED_OFF GPIOF->BRR |= GPIO_BRR_BR_0

#define PI_ON GPIOB->BRR |= GPIO_BRR_BR_1
#define PI_OFF GPIOB->BSRR |= GPIO_BSRR_BR_1
//#define PI_OFF GPIOB->BSRR |= GPIO_BSRR_BS_1

#define IGNITION_STATUS (GPIOF->IDR & GPIO_IDR_1)
#define SCREEN_STATUS (GPIOA->IDR & GPIO_IDR_0)

void configure();


#endif

#include "config.h"
#include "sysevent.h"

volatile uint32_t __systick_counter = 0;
extern struct Event systick_events[5];
extern uint32_t systick_registered_events;

void configure()
{
	// We are missing external oscillator, so SysClock is set to HSI. One thing
	// left is to configure PLL

	if (RCC->CR & RCC_CR_HSIRDY)
	{
		FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
		RCC->CFGR &= ~(RCC_CFGR_HPRE);
		RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
		RCC->CFGR &= ~(RCC_CFGR_PPRE);
		RCC->CFGR |= RCC_CFGR_PPRE_DIV1;

		// disable external oscillator (even if there is no one)
		RCC->CR &= ~(RCC_CR_HSEON);

		// We are running 8MHz now - enable and configure peripherals

		// GPIO
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOFEN;

		// LED
		GPIOF->MODER |= GPIO_MODER_MODER0_0; // gpio mode
		GPIOF->OTYPER &= ~(GPIO_OTYPER_OT_0);
		GPIOF->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR0;
		GPIOF->PUPDR &= ~(GPIO_PUPDR_PUPDR0); // no push-up, no pull-down

		// MCU_ENABLE (or PI enable/disable)
		GPIOB->MODER |= GPIO_MODER_MODER1_0; // gpio mode
		GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_1);
		GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR1;
		GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR1); // no push-up, no pull-down
		GPIOB->PUPDR |= GPIO_PUPDR_PUPDR1_0; // and set to push-up

		// IGNITION sense
		GPIOF->MODER &= ~(GPIO_MODER_MODER1); // PORTF.1 input
		GPIOF->PUPDR &= ~(GPIO_PUPDR_PUPDR1);
		GPIOF->PUPDR |= GPIO_PUPDR_PUPDR1_1; // pull-down

		// HEAD UP open/close sense
		GPIOA->MODER &= ~(GPIO_MODER_MODER0);
		GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR0);
		GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1; // pull-down

		LED_OFF;
		PI_OFF;

		// USART
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
		GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);
		GPIOA->AFR[0] &= ~(GPIO_AFRL_AFR2 | GPIO_AFRL_AFR3);
		GPIOA->AFR[0] |= (1 << 8) | (1 << 12); // AF1 for A.2, A.3


		USART1->BRR = 0xD0; // 38400 bauds 8N1
		USART1->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;

		NVIC_SetPriority(USART1_IRQn, 1);
		NVIC_EnableIRQ(USART1_IRQn);


		// SysTick every 1ms
		if (SysTick_Config(8000))
		{
			while(1); // failed
		}

	}
	else
	{
		while(1); // HSI is not up and running, HSE enabled. Not supported so far.
	}

}

void SysTick_Handler()
{
	uint32_t i;

	for(i = 0; i < systick_registered_events; i++)
	{
		if(__systick_counter % systick_events[i].ms == 0)
			systick_events[i].handler();
	}

	if (++__systick_counter == 1000)
		__systick_counter = 0;

}

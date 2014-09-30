#include <stm32f0xx.h>

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
		RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL);
		RCC->CFGR |= RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMUL12; // HSI/2 * 12MHz = 48MHz

		RCC->CR |= RCC_CR_PLLON;

		while((RCC->CR & RCC_CR_PLLRDY) == 0);
		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_PLL;

		while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

		// We are running 48MHz now - enable peripherals

		// GPIO
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOFEN;

		// USART
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	}
	else
	{
		while(1); // HSI is not up and running, HSE enabled. Not supported so far.
	}

}

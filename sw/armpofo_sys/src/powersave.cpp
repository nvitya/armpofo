/*
 * powersave.cpp
 *
 *  Created on: 6 Dec 2020
 *      Author: vitya
 */

#include "platform.h"
#include "powersave.h"
#include "board_config.h"
#include "syskeyboard.h"
#include "sysdisplay.h"

void cpu_soft_reset()
{
	uint32_t tmp = SCB->AIRCR;
	tmp &= 0x0000FF00;
	tmp |= 0x05FA0004; // write key + reset (SYSRESETREQ)
	SCB->AIRCR = tmp;

	while (1)
	{
		// wait for the reset...
	}
}

void cpu_enter_stop_mode()
{
  /* Select the regulator state in STOP mode */
  //PWR->CR1 |= PWR_CR1_LPDS;
  PWR->CR1 &= ~PWR_CR1_LPDS;

  /* Keep DSTOP mode when Cortex-M7 enter in DEEP-SLEEP */
  PWR->CPUCR &= ~(PWR_CPUCR_PDDS_D1 | PWR_CPUCR_PDDS_D2 | PWR_CPUCR_PDDS_D3); // stop
  //PWR->CPUCR |= (PWR_CPUCR_PDDS_D1 | PWR_CPUCR_PDDS_D2 | PWR_CPUCR_PDDS_D3);

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

  /* Ensure that all instructions are done before entering STOP mode */
  __DSB ();
  __ISB ();

  /* Select STOP mode entry */
  /* Request Wait For Interrupt */
  __WFI ();
}

void cpu_enter_standby_mode()
{
  /* Select the regulator state in STOP mode */
  //PWR->CR1 |= PWR_CR1_LPDS;
  PWR->CR1 &= ~PWR_CR1_LPDS;

  /* Keep DSTOP mode when Cortex-M7 enter in DEEP-SLEEP */
  //PWR->CPUCR &= ~(PWR_CPUCR_PDDS_D1 | PWR_CPUCR_PDDS_D2 | PWR_CPUCR_PDDS_D3); // stop
  PWR->CPUCR |= (PWR_CPUCR_PDDS_D1 | PWR_CPUCR_PDDS_D2 | PWR_CPUCR_PDDS_D3);  // standby

  /* Set SLEEPDEEP bit of Cortex System Control Register */
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

  /* Ensure that all instructions are done before entering STOP mode */
  __DSB ();
  __ISB ();

  /* Select STOP mode entry */
  /* Request Wait For Interrupt */
  __WFI ();
}

void rtc_writeprotect_disable()
{
  RTC->WPR = 0xCA;
  RTC->WPR = 0x53;
}

void rtc_writeprotect_enable()
{
  RTC->WPR = 0xFF;
}

#define IRQHANDLER_RTC_WAKEUP  IRQ_Handler_03

#define EXTI_WAKEUP_BIT  (1 << 19)

extern "C" void IRQHANDLER_RTC_WAKEUP(void)
{
  //HAL_RTCEx_WakeUpTimerIRQHandler(&RTCHandle);

	EXTI_D1->PR1 = EXTI_WAKEUP_BIT;

  rtc_writeprotect_disable();
	RTC->ISR = 0; // clear the interrupt

  uint32_t tmp;
  tmp = RTC->CR;
  tmp &= ~(RTC_CR_WUTE);    // disable the wakeup timer
  tmp &= ~(RTC_CR_WUTIE);   // disable wakeup interrupt
  RTC->CR = tmp;

  for (int i = 0; i < 100; ++i)
  {
  	if (RTC->ISR) {  }
  }

	cpu_soft_reset();
}

void rtc_start()
{
  /* Enable write access to Backup domain */
  PWR->CR1 |= PWR_CR1_DBP;

  /* Wait for Backup domain Write protection disable */
  for (int i = 0; i < 10000; ++i)
  {
  	__NOP();
  }

  RCC->BDCR = (0
   | (1 << 15) // RTCEN
   | (1 <<  8) // RTCSRC(2): 1 = LSE, 2 = LSI
   | (0 <<  6) // LSECSSD
   | (0 <<  5) // LSECSSON
   | (1 <<  3) // LSEDRV(2)
   | (0 <<  2) // LSEBYP
   | (0 <<  1) // LSERDY
   | (1 <<  0) // LSEON
  );

  while (0 == (RCC->BDCR & 2))
  {
  	// wait until LSE + RTC is ready
  }

  for (int i = 0; i < 10000; ++i)
  {
  	__NOP();
  }
}

void low_power_turnoff_periph()
{
	// turn off the display power!
	hwpinctrl.PinSetup(PORTNUM_C, 6, PINCFG_OUTPUT | PINCFG_OPENDRAIN | PINCFG_GPIO_INIT_1);  // open drain because of the 5V !

	// pull all the display lines low !
	hwpinctrl.PinSetup(PORTNUM_E, 7, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0); // aka RS
	hwpinctrl.PinSetup(PORTNUM_B, 1, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_D, 9, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_D, 8, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_D, 10, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);

	// Data Bus pins:
	hwpinctrl.PinSetup(PORTNUM_E,  8, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_E,  9, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_E, 10, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_E, 11, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_E, 12, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_E, 13, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_E, 14, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	hwpinctrl.PinSetup(PORTNUM_E, 15, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);

	// contrast PWM
	hwpinctrl.PinSetup(PORTNUM_B, 0, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0); // select TIM3_CH3, open drain because of the 5V !
}

void enter_low_power()
{
	SysTick->CTRL = 0; // stop the systick IRQ

	//low_power_turnoff_periph();

	rtc_start();

  pin_led1.Set0();

  PWR->CR1 |= PWR_CR1_DBP;
  rtc_writeprotect_disable();

  uint32_t tmp;
  tmp = RTC->CR;
  tmp &= ~(RTC_CR_WUTE);    // disable the wakeup timer
  tmp &= ~(RTC_CR_WUTIE);   // disable wakeup interrupt
  tmp &= ~(RTC_CR_WUCKSEL); // select RTC/16 for the WU clock
  RTC->CR = tmp;

	RTC->ISR = 0; // clear the wakeup flag

  for (int i = 0; i < 1000; ++i)
  {
  	__NOP();
  }

  NVIC_EnableIRQ(RTC_WKUP_IRQn);
	mcu_enable_interrupts();

  RTC->WUTR = (POWERSAVE_WAKEUP_PERIOD_MS * 32768 / (16 * 1000));

  EXTI_D1->IMR1 |= EXTI_WAKEUP_BIT;  // RTC wakeup event
  EXTI->RTSR1 |= EXTI_WAKEUP_BIT;  // rising edge

  PWR->WKUPCR = 0x3F; // clear all wakeup flags
	EXTI_D1->PR1 = EXTI_WAKEUP_BIT;

  RTC->CR |= (RTC_CR_WUTIE | RTC_CR_WUTE);  // enable the wakeup timer + IT

  while (1)
  {
  	// wait for the RTC wakeup IRQ
  	//cpu_enter_stop_mode();
  	cpu_enter_standby_mode();
  	//__NOP();
  }
	//__WFI();
}

void enter_powersave()
{
	g_display.TurnOnOff(false);
	pin_led1.Set0();

	// wait until all the keys are released

	enter_low_power();
}

void standby_wakeup_test()
{
	g_keyboard.Init();

	if (g_keyboard.AnyKeyPressed(20))
	{
		cpu_soft_reset();
	}

	// go back to sleep
	enter_low_power();
}


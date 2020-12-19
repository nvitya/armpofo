// ARMPOFO Powersaving Test

#include "platform.h"
#include "hwclkctrl.h"
#include "hwpins.h"
#include "cppinit.h"
#include "clockcnt.h"
#include "hwpwm.h"
#include "keymatrix.h"

#include "board_config.h"
#include "systimer.h"
#include "syskeyboard.h"

#define LOW_POWER_TEST 0

void init_pins()
{
	// go on with the hardware initializations
	board_init();  // led1 etc

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
	hwpinctrl.PinSetup(PORTNUM_B, 0, PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
}

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

#if 0

void enter_low_power2() // called right after the start
{
	pin_led1.Set0();

  //SysTick->CTRL = 0;  // stop the systick timer !

	// switch back to the 4MHz CSI CLOCK

	RCC->CR |= (RCC_CR_CSION | RCC_CR_CSIKERON);
	while ((RCC->CR & RCC_CR_CSIRDY) == 0)
	{
		// wait
	}



#if 1
	uint32_t tmp = RCC->CFGR;
	tmp &= ~RCC_CFGR_SW_Msk;
  tmp |= RCC_CFGR_SW_CSI;
	RCC->CFGR = tmp;
	unsigned n = 0;
  while (((RCC->CFGR >> 2) & 3) != RCC_CFGR_SW_CSI) // wait until it is set
  {
  	__NOP();
  	++n;
  	if (n > 10000)  break; // however it is effective the status bits does not reflect !
  }

  unsigned low_clock_mhz = 4000000;

#else
	uint32_t tmp = RCC->CFGR;
	tmp &= ~RCC_CFGR_SW_Msk;
  tmp |= RCC_CFGR_SW_HSI;
	RCC->CFGR = tmp;
  while (((RCC->CFGR >> 2) & 3) != RCC_CFGR_SW_HSI) // wait until it is set
  {
  	__NOP();
  }

  unsigned low_clock_mhz = 64000000;

#endif

	hwclkctrl.SetClockInfo(low_clock_mhz);


#if 1
	RCC->CR &= ~(RCC_CR_PLLON | RCC_CR_HSEON);  // disable the PLL1, HSE

	SYSCFG->PWRCR &= ~SYSCFG_PWRCR_ODEN;
	while (0 == (PWR->D3CR & PWR_D3CR_VOSRDY))
	{
		// wait until ready
	}

/*
  tmp = PWR->D3CR;
  tmp &= PWR_D3CR_VOS_Msk;
  tmp |= (PWR_D3CR_VOS_1 << PWR_D3CR_VOS_Pos); // VOS3 slowest mode
	PWR->D3CR = tmp;

	while (0 == (PWR->D3CR & PWR_D3CR_VOSRDY))
	{
		// wait until ready
	}
*/

#endif

	//RCC->AHB4ENR = 0; // disable all GPIO

#if 1

	PWR->WKUPCR = 0xFF;

	SysTick_Config(low_clock_mhz * 3);

	cpu_enter_stop_mode();
	//__WFI();

	cpu_soft_reset();

#else

	unsigned t0, t1;
	unsigned ccnt = 0;

	t0 = CLOCKCNT;
	unsigned tstart = t0;

	while (1)
	{
		t1 = CLOCKCNT;

		if (t1 - tstart > (low_clock_mhz << 2))
		{
			cpu_soft_reset();
		}

		if (t1 - t0 > (low_clock_mhz >> 4))
		{
			pin_led1.Toggle();
			t0 = t1;
		}
	}
#endif
}


#endif

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

void enter_low_power()
{
	SysTick->CTRL = 0; // stop the systick IRQ

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

  RTC->WUTR = (1 * 32768/16); // 4s timeout

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

// the C libraries require "_start" so we keep it as the entry point
extern "C" __attribute__((noreturn)) void _start(void)
{
	// the processor jumps here right after the reset
	// the MCU runs slower, using the internal RC oscillator
	// all variables are unstable, they will be overridden later

	mcu_disable_interrupts();

  mcu_preinit_code(); // inline code for preparing the MCU, RAM regions. Without this even the stack does not work on some MCUs.

	// Set the interrupt vector table offset, so that the interrupts and exceptions work
	mcu_init_vector_table();

	bool standby_wakeup = false;

  /* Check and handle if the system was resumed from StandBy mode */
  if (PWR->CPUCR & PWR_CPUCR_SBF)
  {
    /* Clear Standby flag */
    PWR->CPUCR = PWR_CPUCR_CSSF;

    //cpu_soft_reset();
    standby_wakeup = true;
  }

  unsigned clockspeed = MCU_CLOCK_SPEED;

  if (!standby_wakeup)
  {

	#ifdef MCU_INPUT_FREQ
		if (!hwclkctrl.InitCpuClock(MCU_INPUT_FREQ, clockspeed))  // activate the external crystal oscillator with multiplication x2
	#else
		if (!hwclkctrl.InitCpuClockIntRC(MCU_INTRC_SPEED, clockspeed))  // activate the external crystal oscillator with multiplication x2
	#endif
		{
			while (1)
			{
				// the external oscillator did not start.
			}
		}
  }
  else
  {
  	clockspeed = 64000000;
  }

	cppinit();

	// provide info to the system about the clock speed:
	hwclkctrl.SetClockInfo(clockspeed);

	mcu_enable_fpu();    // enable coprocessor if present
	mcu_enable_icache(); // enable instruction cache if present

	clockcnt_init();

	// go on with the hardware initializations
	init_pins();

	if (standby_wakeup)
	{
    standby_wakeup_test();	// never returns, either goes back to sleep or resets
	}

	//---------------------------------------------------------

	mcu_enable_interrupts();

	systimer_init();

	pin_led1.Set1();

	g_keyboard.Init();

	unsigned hbclocks = SystemCoreClock / 2;

	unsigned t0, t1;

	t0 = CLOCKCNT;

	unsigned cyclecnt = 0;

	unsigned prev_scanserial = g_keyscan_events.serial;

	unsigned ststart = g_sysms;

	// Infinite loop
	while (1)
	{
		t1 = CLOCKCNT;

		if (g_sysms - ststart > 3000)
		{
			enter_low_power(); // never returns
		}

		if (g_keyboard.AnyKeyPressed(20))
		{
			pin_led1.Set0();
		}
		else
		{
			pin_led1.Set1();
		}

		++cyclecnt;
	}
}

// ----------------------------------------------------------------------------

// ARMPOFO QSPI Test

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
#include "traces.h"

#include "hwqspi.h"
#include "qspiflashtest.h"

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

  unsigned clockspeed = MCU_CLOCK_SPEED;

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

	cppinit();

	// provide info to the system about the clock speed:
	hwclkctrl.SetClockInfo(clockspeed);

	mcu_enable_fpu();    // enable coprocessor if present
	mcu_enable_icache(); // enable instruction cache if present

	clockcnt_init();

	// go on with the hardware initializations
	init_pins();

	traces_init();

	TRACE("\r\n------------------------------------------------------------------\r\n");
	TRACE("ARMPOFO QSPI TEST\r\n");

	//---------------------------------------------------------

	mcu_enable_interrupts();

	systimer_init();

	pin_led1.Set1();

	qspi_flash_test();

	unsigned hbclocks = SystemCoreClock / 2;

	unsigned t0, t1;

	t0 = CLOCKCNT;

	unsigned cyclecnt = 0;

	// Infinite loop
	while (1)
	{
		t1 = CLOCKCNT;

		if (t1 - t0 > hbclocks)
		{
			pin_led1.Toggle();
			t0 = t1;
		}

		++cyclecnt;
	}
}

// ----------------------------------------------------------------------------

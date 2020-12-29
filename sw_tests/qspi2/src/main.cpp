// ARMPOFO QSPI Test

#include "platform.h"
#include "hwclkctrl.h"
#include "hwpins.h"
#include "cppinit.h"
#include "clockcnt.h"
#include "hwpwm.h"

#include "board_config.h"
#include "traces.h"

#include "hwqspi.h"
#include "qspiflashtest.h"


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
	board_init();

	traces_init();

	TRACE("\r\n------------------------------------------------------------------\r\n");
	TRACE("ARMPOFO QSPI TEST\r\n");

	//---------------------------------------------------------

	mcu_enable_interrupts();

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

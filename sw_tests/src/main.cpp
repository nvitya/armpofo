// ATARILCD Main

#include <commandline.h>
#include "platform.h"
#include "hwclkctrl.h"
#include "hwpins.h"
#include "cppinit.h"
#include "clockcnt.h"
#include "hwpwm.h"
#include "keymatrix.h"

#include "board_config.h"
#include "systimer.h"
#include "sysdisplay.h"
#include "syskeyboard.h"

#include "powersave.h"
#include "battery.h"
#include "sysproc.h"

TCommandLine  cmdline;

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

	// now the MCU runs faster, start the memory, and C/C++ initialization:
	cppinit();
	// ...from now on all the variables work, static classes are initialized.

	// provide info to the system about the clock speed:
	hwclkctrl.SetClockInfo(clockspeed);

	mcu_enable_fpu();    // enable coprocessor if present
	mcu_enable_icache(); // enable instruction cache if present

	clockcnt_init();

	// go on with the hardware initializations
	board_init();

	battery_init();

	if (standby_wakeup)
	{
    standby_wakeup_test();	// never returns, either goes back to sleep or resets
	}

	//---------------------------------------------------------

	mcu_enable_interrupts();

	systimer_init();

	pin_led1.Set1();

	g_keyboard.Init();

	g_display.Init();

	for (unsigned n = 32; n < 127; ++n)
	{
		g_display.WriteChar(n);
	}
	g_display.WriteChar('\n');

	delay_us(100);
	g_display.printf("BAT  = %u mV\n", battery_get_u_bat());
	g_display.printf("I    = %i mA\n", battery_get_i_charge());
	g_display.printf("Supp = %u mV\n", battery_get_u_5V());

	g_display.Run();

	unsigned hbclocks = SystemCoreClock / 1;

	unsigned t0, t1;

	t0 = CLOCKCNT;

	cmdline.Clear();
	cmdline.Draw();

	// Start with the command line processing
	while (1)
	{
		sys_run();

		cmdline.Run();
	}
}

// ----------------------------------------------------------------------------

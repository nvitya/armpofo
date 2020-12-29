// ATARILCD Main

#include "string.h"
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
#include "extflash.h"
#include "sysif.h"
#include "traces.h"

TCommandLine  cmdline;

void start_sys(unsigned aentry)
{
  __asm("mov  r3, %0" : : "r" (aentry) : "memory");
  __asm("orr  r3, #1");  // use THUMB code
	__asm("bx   r3");
}

void load_and_start_sys()
{
	g_extflash.Init();
	if (!g_extflash.initialized)
	{
		return;
	}

	g_extflash.StartReadMem(SYSIF_SYS_FLASH_ADDR, (void *)SYSIF_SYS_LOAD_ADDR, sizeof(TAppHeader));
	g_extflash.WaitForComplete();
	if (g_extflash.errorcode)
	{
		TRACE("SYS header load error.\r\n");
		return;
	}

	TAppHeader * pheader = (TAppHeader *)SYSIF_SYS_LOAD_ADDR;

	// check the header
	if (strncmp(&pheader->name[0], "ARMPOFO_SYS", sizeof(pheader->name)) != 0)
	{
		TRACE("SYS header signature error.\r\n");
		return;
	}

	if (sys_header_checksum(pheader) != pheader->header_checksum)
	{
		TRACE("SYS header checksum error.\r\n");
		return;
	}

	TRACE("Loading content...\r\n");
	g_extflash.StartReadMem(
			SYSIF_SYS_FLASH_ADDR + sizeof(TAppHeader),
			(void *)(SYSIF_SYS_LOAD_ADDR + sizeof(TAppHeader)),
			pheader->content_length
	);
	g_extflash.WaitForComplete();
	if (g_extflash.errorcode)
	{
		TRACE("SYS content load error.\r\n");
		return;
	}

	unsigned csum = sys_content_checksum((void *)(SYSIF_SYS_LOAD_ADDR + sizeof(TAppHeader)), pheader->content_length);
	if (csum != pheader->content_checksum)
	{
		TRACE("SYS content checksum error.\r\n");
		return;
	}

	TRACE("Starting system...\r\n");

  start_sys(pheader->entry_point);
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

	if (standby_wakeup)
	{
    standby_wakeup_test();	// never returns, either goes back to sleep or resets
	}

	//---------------------------------------------------------

	pin_led1.Set1();

	traces_init();

	load_and_start_sys(); // returs only if not successful.

	// unsuccessful system load from QSPI flash
	TRACE("Staying in bootloader...\r\n");

	mcu_enable_interrupts();

	systimer_init();

	battery_init();

	g_keyboard.Init();

	g_display.Init();

	g_display.printf("ARMPOFO Bootloader v1.x.x\n");
	g_display.Run();

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

// ATARILCD Main

#include "stdlib.h"
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
#include "flasher.h"

#include "traces.h"

TCommandLine  cmdline;

// Entry point for QSPI Flash boot
// The IMXRT FlexSPI boot process starts not at the vector table _start but rather here
extern "C" __attribute__((section(".rom_startup"))) void _startrom(void)
{
  mcu_disable_interrupts();
	mcu_init_vector_table();   // set the vector table address
  mcu_preinit_code();        // required for RAM preparation

	// the stack might not be set properly so set it
  asm("ldr  r0, =__stack");
  asm("mov  sp, r0");

  // start main
  asm("ldr  r1, =_main");
  asm("movs r0, #0"); // signalize ROM boot
  asm("bx   r1");
}

// Entry point for Development
extern "C" __attribute__((section(".startup"))) void _start(void)
{
	// the processor jumps here right after the reset

  mcu_disable_interrupts();
	mcu_init_vector_table();   // set the vector table address
  mcu_preinit_code();        // required for RAM preparation

	// the stack might not be set properly so set it
  asm("ldr  r0, =__stack");
  asm("mov  sp, r0");

  // start main
  asm("ldr  r1, =_main");
  asm("movs r0, #1");  // signalize Development (RAM) boot
  asm("bx   r1");
}

extern unsigned __image_end;

void run_flasher()
{
	// FLASHER
	void * tempbuf = malloc(0x1000);  // allocate the temp buffer on the heap !!!

	// calculate checksums first
	TAppHeader * pheader;
	uint32_t cs;
	uint32_t startaddr;
  unsigned copylength;

  // CODE

  volatile unsigned imgend = unsigned(&__image_end);
  volatile unsigned imgsize = imgend - SYSIF_SYS_LOAD_ADDR - sizeof(TAppHeader);

	pheader = (TAppHeader *)SYSIF_SYS_LOAD_ADDR;
	startaddr = SYSIF_SYS_LOAD_ADDR + sizeof(TAppHeader);
	cs = sys_content_checksum((void *)startaddr, pheader->content_length);
	pheader->content_checksum = cs;
	pheader->header_checksum = sys_header_checksum(pheader);

  copylength = unsigned(&__image_end) - SYSIF_SYS_LOAD_ADDR;
  copylength = ((copylength + 255) & 0xFFF00);
	flasher_copy((void *)(SYSIF_SYS_LOAD_ADDR), SYSIF_SYS_FLASH_ADDR, copylength, tempbuf);

	free(tempbuf);
}

// the C libraries require "_start" so we keep it as the entry point
extern "C" __attribute__((section(".startup"),used,noreturn))
void _main(unsigned ramboot)
{
	// setup clock first

  unsigned clockspeed = MCU_CLOCK_SPEED;

#ifdef MCU_INPUT_FREQ
	if (!hwclkctrl.InitCpuClock(MCU_INPUT_FREQ, clockspeed))
#else
	if (!hwclkctrl.InitCpuClockIntRC(MCU_INTRC_SPEED, clockspeed))
#endif
	{
		while (1)
		{
			// the external oscillator did not start.
		}
	}

	mcu_enable_fpu();    // enable coprocessor if present
	mcu_enable_icache(); // enable instruction cache if present

	// now the MCU runs faster, start the memory, and C/C++ initialization:
	cppinit();
	// ...from now on all the variables work, static classes are initialized.

	// provide info to the system about the clock speed:
	hwclkctrl.SetClockInfo(clockspeed);

	//---------------------------------------------------------
	clockcnt_init();
	// go on with the hardware initializations
	board_init();

	pin_led1.Set1();

	traces_init();
	TRACE("\r\n-------------------------------\r\n");
	TRACE("Initializing ARMPOFO System...\r\n");

	systimer_init();
	mcu_enable_interrupts();

	g_display.Init();
	g_display.cursor_on = false;

	g_display.printf("ARMPOFO Sys v1.x.x\n");
	g_display.Run();

	g_extflash.Init();
	g_display.Run();

	if (ramboot)
	{
		run_flasher();
	}

	g_extflash.qspi.SetMemMappedMode();

	battery_init();

	g_keyboard.Init();

#if 0
	g_display.printf("BAT  = %u mV\n", battery_get_u_bat());
	g_display.printf("I    = %i mA\n", battery_get_i_charge());
	g_display.printf("Supp = %u mV\n", battery_get_u_5V());
#endif

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

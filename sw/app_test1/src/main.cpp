// APP: TEST1

#include "stdlib.h"
#include "platform.h"
#include "hwclkctrl.h"
#include "hwpins.h"
#include "cppinit.h"
#include "clockcnt.h"

#include "sysif.h"

// Entry point for QSPI Flash boot
// The IMXRT FlexSPI boot process starts not at the vector table _start but rather here
extern "C" __attribute__((section(".rom_startup"))) void _startrom(void)
{
  mcu_disable_interrupts();

	// the stack might not be set properly so set it
  asm("ldr  r0, =__stack");
  asm("mov  sp, r0");

  // start main
  asm("ldr  r1, =_main");
  asm("movs r0, #0"); // signalize ROM boot
  asm("bx   r1");

  __BKPT();
}

// Entry point for Development
extern "C" __attribute__((section(".startup"))) void _start(void)
{
	// the processor jumps here right after the reset

  mcu_disable_interrupts();

	// the stack might not be set properly so set it
  asm("ldr  r0, =__stack");
  asm("mov  sp, r0");

  // start main
  asm("ldr  r1, =_main");
  asm("movs r0, #1");  // signalize Development (RAM) boot
  asm("bx   r1");

  __BKPT();
}


extern unsigned __image_end;

extern const TAppHeader application_header;

void run_flasher()
{
	// calculate checksums first
	TAppHeader * pheader;
	uint32_t cs;
	uint32_t startaddr;

  // CODE

  volatile unsigned imgend = unsigned(&__image_end);
  volatile unsigned imgsize = imgend - SYSIF_APP_LOAD_ADDR - sizeof(TAppHeader);

	pheader = (TAppHeader *)SYSIF_APP_LOAD_ADDR;
	startaddr = SYSIF_APP_LOAD_ADDR + sizeof(TAppHeader);
	cs = sys_content_checksum((void *)startaddr, pheader->content_length);
	pheader->content_checksum = cs;
	pheader->header_checksum = sys_header_checksum(pheader);

	(* psys_app_save)((TAppHeader *)&application_header); // never returns.
}

extern "C" __attribute__((section(".startup"),used,noreturn))
void _main(unsigned ramboot)
{
	cppinit();

	if (!sys_init_application())
	{
		__BKPT();  // reset if debugger not connected.
	}

	if (ramboot)
	{
		run_flasher(); // should not return
		(* psys_printf)("\n");
	}

	(*psys_printf)("Hello from app...\n");
	(*psys_trace)("TRACE from app ! \r\n");

	unsigned hbclocks = SystemCoreClock / 5;
	unsigned hbcnt = 0;

	unsigned t0, t1;

	t0 = CLOCKCNT;

	while (1)
	{
		t1 = CLOCKCNT;

		//sys_run();

		(*psys_run)();

		if (t1 - t0 > hbclocks)
		{
			(* psys_led_set)(0, hbcnt & 1);

			++hbcnt;
			t0 = t1;
		}
	}
}

// ----------------------------------------------------------------------------

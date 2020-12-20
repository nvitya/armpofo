/*
 * sysproc.cpp
 *
 *  Created on: 8 Dec 2020
 *      Author: vitya
 */

#include "sysproc.h"
#include "sysdisplay.h"
#include "syskeyboard.h"
#include "clockcnt.h"
#include "board_config.h"
#include "powersave.h"

unsigned g_sys_hbcounter = 0;
unsigned g_sys_hbtime = 0;

static uint16_t  sys_prev_symserial = 0xFFFF;

void sys_run() // run system idle tasks
{
	unsigned t = CLOCKCNT;

	g_display.Run();
	g_keyboard.Run();

	// process system special keys

	if (sys_prev_symserial != g_keysym_events.serial)
	{
		TKeySymbolEvent * psyme = &g_keysym_events.events[g_keysym_events.serial & (MAX_KEYSYMEVENTS-1)];
		uint16_t keysym = psyme->symbol;

		if (KEYSYM_RESET == keysym)
		{
			g_display.printf("\nRESET...");
			g_display.Run();

			// wait until all the keys are released

			while (g_keyboard.keys64 != 0)
			{
				g_keyboard.Run();
			}

			cpu_soft_reset();
		}
		else if (KEYSYM_SLEEP == keysym)
		{
			g_display.printf("\nTURNING OFF...");
			g_display.Run();

			while (g_keyboard.keys64 != 0)
			{
				g_keyboard.Run();
			}

			enter_powersave();
		}

		sys_prev_symserial = g_keysym_events.serial;
	}

	if (t - g_sys_hbtime > SystemCoreClock)
	{
		pin_led1.Toggle();
		g_sys_hbtime = t;
	}
}

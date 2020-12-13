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

unsigned g_sys_hbcounter = 0;
unsigned g_sys_hbtime = 0;

void sys_run() // run system idle tasks
{
	unsigned t = CLOCKCNT;

	g_display.Run();
	g_keyboard.Run();

	if (t - g_sys_hbtime > SystemCoreClock / 10)  // fast led blinking
	{
		pin_led1.Toggle();
		g_sys_hbtime = t;
	}
}

/*
 * systimer.cpp
 *
 *  Created on: 15 Nov 2020
 *      Author: vitya
 */

#include "platform.h"
#include "systimer.h"


uint32_t volatile g_sysms;

extern "C" void SysTick_Handler(void)
{
	++g_sysms;
}

void systimer_init()
{
	g_sysms = 0;

	SysTick_Config(SystemCoreClock / 1000); // required for the contrast control !
}



/*
 * extflash.cpp
 *
 *  Created on: 11 Dec 2020
 *      Author: vitya
 */

#include <extflash.h>
#include "hwpins.h"
#include "traces.h"

TExtFlash g_extflash;

void TExtFlash::Init()
{
	uint32_t qspipincfg = 0; //PINCFG_SPEED_MEDIUM;

	hwpinctrl.PinSetup(PORTNUM_B,  2, qspipincfg | PINCFG_AF_9);   // CLK
	hwpinctrl.PinSetup(PORTNUM_B, 10, qspipincfg | PINCFG_AF_9);   // NCS

	hwpinctrl.PinSetup(PORTNUM_D, 11, qspipincfg | PINCFG_AF_9);   // IO0
	hwpinctrl.PinSetup(PORTNUM_D, 12, qspipincfg | PINCFG_AF_9);   // IO1

	hwpinctrl.PinSetup(PORTNUM_E,  2, qspipincfg | PINCFG_AF_9);   // IO2
	hwpinctrl.PinSetup(PORTNUM_D, 13, qspipincfg | PINCFG_AF_9);   // IO3

	// it should go much faster and with 4 lines

	// at 100 MHz CPU speed high QSPI speeds are not working!
	qspi.speed = 10000000; // 10 MHz seems to be stable
	qspi.multi_line_count = 4; // the memmap mode seems to be not working with this
	qspi.idleclk_high = true;
	has4kerase = true;
	if (!super::Init())
	{
		TRACE("QSPI Flash init failed!\r\n");
		return;
	}

	TRACE("QSPI ID = %06X, %u k\r\n", idcode, (bytesize >> 10));
}

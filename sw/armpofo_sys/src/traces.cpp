/*
 * traces.cpp
 *
 *  Created on: 12 Dec 2020
 *      Author: vitya
 */


#include "hwpins.h"
#include "hwuart.h"
#include "traces.h"

THwUart trace_uart;

void traces_init()
{
	hwpinctrl.PinSetup(PORTNUM_A, 9, PINCFG_AF_7);  // USART1_TX
	trace_uart.Init(1);
}

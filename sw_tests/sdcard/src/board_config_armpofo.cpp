/*
 * board_config_armpofo.cpp
 *
 *  Created on: 27 Nov 2020
 *      Author: vitya
 */

#include "hwpins.h"
#include "board_config.h"

TGpioPin  pin_led1(PORTNUM_D, 15, true);

void board_init()
{
	pin_led1.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1); // initialize as off
	
	// SDCARD Pins
	hwpinctrl.PinSetup(PORTNUM_C,  8, PINCFG_AF_12); // SDMMC_D0
	hwpinctrl.PinSetup(PORTNUM_C,  9, PINCFG_AF_12); // SDMMC_D1
	hwpinctrl.PinSetup(PORTNUM_C, 10, PINCFG_AF_12); // SDMMC_D2
	hwpinctrl.PinSetup(PORTNUM_C, 11, PINCFG_AF_12); // SDMMC_D3
	hwpinctrl.PinSetup(PORTNUM_C, 12, PINCFG_AF_12); // SDMMC_CK
	hwpinctrl.PinSetup(PORTNUM_D,  2, PINCFG_AF_12); // SDMMC_CMD
}

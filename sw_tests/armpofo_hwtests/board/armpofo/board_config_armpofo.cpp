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
}

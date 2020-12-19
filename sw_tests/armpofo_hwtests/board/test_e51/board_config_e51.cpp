// board_config_test_e51.cpp

#include "hwpins.h"
#include "board_config.h"

TGpioPin  pin_led1(PORTNUM_A, 1, true);

void board_init()
{
	pin_led1.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1); // initialize as off
}

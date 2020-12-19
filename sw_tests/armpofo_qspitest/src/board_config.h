// board_config.h

// common board definition for all different boards

#ifndef SRC_BOARD_CONFIG_H_
#define SRC_BOARD_CONFIG_H_

#include "hwpins.h"
#include "syskeyboard.h"

extern TGpioPin  pin_led1;  // every board must have at least one controllable led :)

void board_init();

#endif /* SRC_BOARD_CONFIG_H_ */

// sysif_display.h

#ifndef SYSIF_DISPLAY_H_
#define SYSIF_DISPLAY_H_

#include "stdint.h"

struct TDisplayInfo
{
	unsigned  rows;
	unsigned  cols;
	unsigned  cposx;
	unsigned  cposy;

	unsigned  cursor_x;
	unsigned  cursor_y;

	uint8_t   cursor_on;
	uint8_t   _reserved1[15];
}; // 64 bytes

#endif /* SYSIF_DISPLAY_H_ */

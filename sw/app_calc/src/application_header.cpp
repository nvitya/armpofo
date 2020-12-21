// application_header.cpp

#include "platform.h"
#include "stdint.h"
#include "sysif.h"

extern unsigned __image_end;

extern "C" void _startrom(void);

extern const TAppHeader application_header; // this is required somehow, otherwise it won't link

__attribute__((section(".application_header"),used))
const TAppHeader application_header =
{
  .name = "CALC\0\0\0\0\0\0\0\0\0\0\0", // the last null added by the compiler

	.imgtype = 2,
	.load_address = SYSIF_APP_LOAD_ADDR,
	.content_length = (((unsigned)&__image_end) - SYSIF_APP_LOAD_ADDR - sizeof(TAppHeader)),
	.entry_point = (unsigned)_startrom,

	0, // service function address
	0, // reserved
	0, // reserved
	0, // reserved

	0, // reserved
	0, // reserved
	.content_checksum = 0,
	.header_checksum = 0
};



// application_header.cpp

#include "platform.h"
#include "stdint.h"
#include "sysif.h"
#include "appif.h"

extern unsigned __image_end;

extern "C" void _startrom(void);

__attribute__((section(".application_header"),used))
const TAppHeader application_header =
{
  .name = "ARMPOFO_SYS\0\0\0\0", // one null added by the compiler

	.imgtype = 1,
	.load_address = SYSIF_SYS_LOAD_ADDR,
	.content_length = (((unsigned)&__image_end) - SYSIF_SYS_LOAD_ADDR - sizeof(TAppHeader)),
	.entry_point = (unsigned)_startrom,

	.func_service = (unsigned)sys_getfuncaddr, // service function address
	0, // reserved
	0, // reserved
	0, // reserved

	0, // reserved
	0, // reserved
	.content_checksum = 0,
	.header_checksum = 0
};



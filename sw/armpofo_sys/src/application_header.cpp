// application_header.cpp

#include "platform.h"
#include "stdint.h"
#include "apsysif.h"

extern unsigned __image_end;

extern "C" void _startrom(void);
extern "C" void * apsys_get_proc_addr(const char * aname);

__attribute__((section(".application_header"),used))
const TAppHeader application_header =
{
  .name = "ARMPOFO_SYS\0\0\0\0", // one null added by the compiler

	.imgtype = 1,
	.load_address = APSYS_SYS_LOAD_ADDR,
	.content_length = (((unsigned)&__image_end) - APSYS_SYS_LOAD_ADDR - sizeof(TAppHeader)),
	.entry_point = (unsigned)_startrom,

	.func_service = (unsigned)apsys_get_proc_addr, // service function address
	0, // reserved
	0, // reserved
	0, // reserved

	0, // reserved
	0, // reserved
	.content_checksum = 0,
	.header_checksum = 0
};



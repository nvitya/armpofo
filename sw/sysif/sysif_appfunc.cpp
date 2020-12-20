// sysif_appfunc.cpp

#include "platform.h"

#define DEFINE_SYSFUNC_VARS
#include "sysif.h"

bool setfuncaddr(const char * afuncname, void * avariable)
{
	void * res = (*psys_getfuncaddr)(afuncname);

	*((void **)avariable) = res; // store always

	if (res == nullptr)
	{
		if (psys_trace)  (*psys_trace)("Error getting address of function %s!\r\n", afuncname);
		return false;
	}

	return true;
}

bool sys_init_application()
{
	TAppHeader * pheader = (TAppHeader *)SYSIF_SYS_LOAD_ADDR;
	// some checking
	if (sys_header_checksum(pheader) != pheader->header_checksum)
	{
		return false;
	}

	// access the application "GETFUNCADDR" function, which provides the rest of the system functions
	psys_getfuncaddr = (t_sys_getfuncaddr)pheader->func_service;

	psys_trace = nullptr;

	bool bok = true;

	// these functions should always exist:
	bok = (bok && setfuncaddr("TRACE",             &psys_trace));
	bok = (bok && setfuncaddr("PRINTF",            &psys_printf));
	bok = (bok && setfuncaddr("RUN",               &psys_run));
	bok = (bok && setfuncaddr("APP_SAVE",          &psys_app_save));
	bok = (bok && setfuncaddr("GET_DATA_PTR",      &psys_get_data_ptr));

	// less important functions
	bok = (bok && setfuncaddr("GET_SYS_VERSION",   &psys_get_sys_version));
	bok = (bok && setfuncaddr("LED_SET",           &psys_led_set));


	// set SystemCoreClock !!!!
	SystemCoreClock = MCU_CLOCK_SPEED; // todo: make a system function to get the actual value

	return bok;
}




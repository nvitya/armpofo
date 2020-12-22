// appif.cpp

#include <stdarg.h>
#include "stdlib.h"
#include "string.h"
#include "mp_printf.h"

#include "appif.h"
#include "sysif.h"
#include "traces.h"

#include "extflash.h"
#include "flasher.h"

#include "sysproc.h"
#include "sysdisplay.h"
#include "syskeyboard.h"
#include "powersave.h"
#include "board_config.h"

//----------------------------------------------

int sys_getversion()
{
	return 1001001;
}

void sys_led_set(unsigned aledid, unsigned avalue)
{
	// aledid ignored.
	pin_led1.SetTo(avalue);
}

void sys_printf(const char * fmt, ...)
{
  va_list arglist;
  va_start(arglist, fmt);
  g_display.printf_va(fmt, arglist);
  va_end(arglist);
}

void sys_trace(const char * fmt, ...)
{
  va_list arglist;
  va_start(arglist, fmt);
  trace_uart.printf_va(fmt, arglist);
  va_end(arglist);
}

void sys_reset(const char * fmt, ...)
{
	cpu_soft_reset();
}

void sys_disp_setpos(int ax, int ay)
{
	g_display.SetPos(ax, ay);
}

void sys_disp_writechar(char c)
{
	g_display.WriteChar(c);
}

void sys_disp_setcursor(bool aon, int ax, int ay)
{
	g_display.cursor_on = aon;
	g_display.cursor_x = ax;
	g_display.cursor_y = ay;
}

void * sys_keyb_getptr(const char * aname)
{
	if (strcmp(aname, "KEYB_SCAN_EVENTS") == 0)
	{
		return &g_keyscan_events;
	}
	else if (strcmp(aname, "KEYB_SYM_EVENTS") == 0)
	{
		return &g_keysym_events;
	}
	else
	{
		return nullptr;
	}
}

void sys_disp_getinfo(TDisplayInfo * rinfo)
{
  if (!rinfo)  return;

  rinfo->cols = g_display.cols;
  rinfo->rows = g_display.rows;
  rinfo->cposx = g_display.cposx;
  rinfo->cposy = g_display.cposy;
  rinfo->cursor_x = g_display.cursor_x;
  rinfo->cursor_y = g_display.cursor_y;
  rinfo->cursor_on = g_display.cursor_on;
}

void sys_app_save(TAppHeader * pheader) // does not return on success
{
	// some simple sanity check
	if (pheader->header_checksum != sys_header_checksum(pheader))
	{
		TRACE("APP_SAVE: header checksum error.\r\n");
		return;
	}

	if (pheader->content_checksum != sys_content_checksum((pheader+1), pheader->content_length))
	{
		TRACE("APP_SAVE: content checksum error.\r\n");
		return;
	}

	TRACE("APP_SAVE: scanning application slots...\r\n");

	// scan trough all the app slots for the existing application
	TAppHeader fheader;

	unsigned   faddr = SYSIF_APP_FLASH_START;
	unsigned   firstfree = 0;
	unsigned   existing = 0;
	while (faddr < SYSIF_APP_FLASH_END)
	{
		g_extflash.StartReadMem(faddr, &fheader, sizeof(fheader));
		g_extflash.WaitForComplete();
		if ( !g_extflash.errorcode
			   && (fheader.header_checksum == sys_header_checksum(&fheader))
			 )
		{
			if (strncmp(&fheader.name[0], &pheader->name[0], sizeof(fheader.name)) == 0)
			{
				TRACE("Existing application found.\r\n");
				existing = faddr;
				break;
			}
		}
		else
		{
			if (!firstfree)	 firstfree = faddr;
		}

		faddr += SYSIF_APP_SIZE;
	}

	faddr = (existing ? existing : firstfree);
	if (!faddr)
	{
		TRACE("APP_SAVE: no more free application slots!\r\n");
		return;
	}

	void * tempbuf = malloc(0x1000);  // allocate the temp buffer on the heap !!!
	if (!flasher_copy(pheader, faddr, sizeof(TAppHeader) + pheader->content_length, tempbuf))
	{
		TRACE("APP_SAVE: flasher error.\r\n");
		return;
	}
	free(tempbuf);

	// application saved, start it

	// reset the stack
  asm("ldr  r0, =__stack");
  asm("mov  sp, r0");

	PEntryFunc pentry = (PEntryFunc)(pheader->entry_point);

	(*pentry)();

	// reset on return.

	cpu_soft_reset();
}

void sys_list_apps()
{
	// scan trough all the app slots for the existing application
	TAppHeader fheader;

	g_display.printf("APPS: ");

	unsigned appcnt = 0;

	unsigned faddr = SYSIF_APP_FLASH_START;
	while (faddr < SYSIF_APP_FLASH_END)
	{
		g_extflash.StartReadMem(faddr, &fheader, sizeof(fheader));
		g_extflash.WaitForComplete();
		if ( !g_extflash.errorcode
			   && (fheader.header_checksum == sys_header_checksum(&fheader))
			 )
		{
			if (appcnt)  g_display.printf(", ");
			g_display.printf("%s", &fheader.name[0]);
			++appcnt;
		}

		faddr += SYSIF_APP_SIZE;
	}
	g_display.printf("\n");
}

//----------------------------------------------

typedef int (* sysfuncptr)(void);

typedef struct t_func_table_entry
{
	const char *  funcname;
	sysfuncptr    funcptr;
//
} t_func_table_entry;

t_func_table_entry  g_functable[] =
{
	{"SYS_GETVERSION",    (sysfuncptr) sys_getversion},
	{"PRINTF",						(sysfuncptr) sys_printf},
	{"TRACE",					  	(sysfuncptr) sys_trace},
	{"RUN",     					(sysfuncptr) sys_run},
	{"APP_SAVE",  				(sysfuncptr) sys_app_save},
	{"RESET",  				    (sysfuncptr) sys_reset},

	{"KEYB_GETPTR",       (sysfuncptr) sys_keyb_getptr},

	{"DISP_GETINFO",      (sysfuncptr) sys_disp_getinfo},
	{"DISP_SETCURSOR",		(sysfuncptr) sys_disp_setcursor},
	{"DISP_SETPOS",				(sysfuncptr) sys_disp_setpos},
	{"DISP_WRITECHAR",		(sysfuncptr) sys_disp_writechar},

	{"LED_SET",  					(sysfuncptr) sys_led_set},


	// closing:
	{"", nullptr}
};

void * sys_getfuncaddr(const char * astr)
{
	//TRACE("getfuncaddr(%s)\r\n", astr);

	t_func_table_entry * pentry = &g_functable[0];

	while (pentry->funcptr != nullptr)
	{
		if (strcmp(astr, pentry->funcname) == 0)
		{
			return (void *)pentry->funcptr;
		}
		++pentry;
	}

	TRACE("getfuncaddr(%s): NOT FOUND!\r\n", astr);

	return nullptr;
}


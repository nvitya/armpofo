// sysif_appfunc.h

#ifndef SYSIF_APPFUNC_H_
#define SYSIF_APPFUNC_H_

#include "stdint.h"
#include "sysif_display.h"

#define SYSIF_SYS_LOAD_ADDR   0x24060000
#define SYSIF_SYS_FLASH_ADDR    0x000000

#define SYSIF_APP_LOAD_ADDR   0x24000000
#define SYSIF_APP_FLASH_START   0x020000
#define SYSIF_APP_FLASH_END     0x100000  // 15 applications
#define SYSIF_APP_SIZE          0x020000

typedef void (* PEntryFunc)();

struct TAppHeader
{
	char         name[16]; // name filled up with zeroes

	unsigned     imgtype;  // 1 = system, 2 = application
	unsigned     load_address;
	unsigned     content_length;
	unsigned     entry_point;

	unsigned     func_service;     // service function address
	unsigned     func_devservice;  // special service routine for the development (saveme)
	unsigned     _reserved_9;
	unsigned     _reserved_10;

	unsigned     saveme;           // save flag for the application development
	unsigned     _reserved_12;
	unsigned     content_checksum;
	unsigned     header_checksum;
};

uint32_t sys_content_checksum(void * adataptr, uint32_t adatalen);
uint32_t sys_header_checksum(TAppHeader * aheader);

#ifdef DEFINE_SYSFUNC_VARS
  #define AS_EXTERN
#else
  #define AS_EXTERN  extern
#endif

typedef void * (* t_sys_getfuncaddr)(const char * idstr);
AS_EXTERN t_sys_getfuncaddr       psys_getfuncaddr;

AS_EXTERN int     (* psys_getversion)();
AS_EXTERN void    (* psys_printf)(const char * fmt, ...);
AS_EXTERN void    (* psys_trace)(const char * fmt, ...);
AS_EXTERN void    (* psys_run)();
AS_EXTERN void    (* psys_reset)();

AS_EXTERN void *  (* psys_keyb_getptr)(const char * name);  // keyboard event structures

AS_EXTERN void    (* psys_disp_getinfo)(TDisplayInfo * rinfo);
AS_EXTERN void    (* psys_disp_setcursor)(bool aon, int ax, int ay);
AS_EXTERN void    (* psys_disp_setpos)(int ax, int ay);
AS_EXTERN void    (* psys_disp_writechar)(char c);

AS_EXTERN void    (* psys_led_set)(unsigned ledid, unsigned value);
AS_EXTERN void    (* psys_app_save)(TAppHeader * pheader);


#undef DEFINE_SYSFUNC_VARS

bool sys_init_application();


#endif /* SYSIF_APPFUNC_H_ */

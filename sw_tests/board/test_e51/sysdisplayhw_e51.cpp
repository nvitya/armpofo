// sysdisplayhw_e51.cpp

#include "clockcnt.h"
#include "hwpins.h"
#include "traces.h"
#include "sysdisplayhw.h"
#include "monolcd_spi.h"

//-----------------------------------------------------------------------------
// GFX Display

#define GFX_DISPLAY_WIDTH   128
#define GFX_DISPLAY_HEIGHT   64
#define GFX_DISPLAY_CTRL    MLCD_CTRL_UC1701

TMonoLcd_spi  gfx_display;
uint8_t       gfx_display_buf[GFX_DISPLAY_WIDTH * GFX_DISPLAY_HEIGHT / 8];

#if 1
  #include "stdmonofont.h"
  TGfxFont font_mono(&stdmonofont);
#else
  // bigger font
  #include "font_FreeMono9pt7b.h"
  TGfxFont font_mono(&FreeMono9pt7b);
#endif

bool gfx_display_init();

//-----------------------------------------------------------------------------
// Text Display

#define SYSDISP_CHAR_COLS   20
#define SYSDISP_CHAR_ROWS    8
#define SYSDISP_CHAR_NUM    (SYSDISP_CHAR_COLS * SYSDISP_CHAR_ROWS)

uint8_t sys_disp_chars[SYSDISP_CHAR_NUM];
uint8_t sys_disp_changemap[SYSDISP_CHAR_NUM / 8];

//-----------------------------------------------------------------------------

bool gfx_display_init()
{
	gfx_display.pin_reset.Assign(PORTNUM_A, 19, false);
	gfx_display.pin_reset.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);

	gfx_display.pin_cs.Assign(PORTNUM_B, 16, false);
	gfx_display.pin_cs.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);

	gfx_display.pin_cd.Assign(PORTNUM_A, 18, false);
	gfx_display.pin_cd.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);


	// SERCOM3
	unsigned pinflags = PINCFG_AF_D | PINCFG_PULLUP | PINCFG_DRIVE_STRONG;
	hwpinctrl.PinSetup(PORTNUM_A, 16, pinflags);  // SERCOM3_PAD1: SCK
	hwpinctrl.PinSetup(PORTNUM_A, 17, pinflags);  // SERCOM3_PAD0: MOSI
	//disp.txdma.Init(4, SERCOM3_DMAC_ID_TX);

	gfx_display.spi.idleclk_high = false;
	gfx_display.spi.datasample_late = false;
	gfx_display.spi.speed = 4000000; // 4 MHz
	gfx_display.spi.Init(3);

	gfx_display.rotation = 0;
	if (!gfx_display.Init(GFX_DISPLAY_CTRL, GFX_DISPLAY_WIDTH, GFX_DISPLAY_HEIGHT, &gfx_display_buf[0]))
	{
		TRACE("Error Initializing GFX LCD display!\r\n");
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------

bool TSysDisplayHw::InitHw()
{
	InitCharBuf(SYSDISP_CHAR_COLS, SYSDISP_CHAR_ROWS, &sys_disp_chars[0], &sys_disp_changemap[0]);

	// initialize the GFX display first
	if (!gfx_display_init())
	{
		return false;
	}

	// this sets the actual cols and rows based on the given font
	InitTextGfx(&gfx_display, 0, 0, gfx_display.width, gfx_display.height, &font_mono);  //&font_big);

	return true;
}

void TSysDisplayHw::Run()
{
	if (!disp)
	{
		return;
	}

	super::Run();
	gfx_display.Run();
}

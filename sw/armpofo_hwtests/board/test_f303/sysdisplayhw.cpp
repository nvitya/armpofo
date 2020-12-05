// sysdisplayhw.cpp (f303)

#include "clockcnt.h"
#include "hwpins.h"
#include "traces.h"
#include "sysdisplayhw.h"
#include "monolcd_spi.h"

//-----------------------------------------------------------------------------
// GFX Display

#define GFX_DISPLAY_WIDTH   256
#define GFX_DISPLAY_HEIGHT  160
#define GFX_DISPLAY_CTRL    MLCD_CTRL_ST75256

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

#define SYSDISP_CHAR_COLS   42
#define SYSDISP_CHAR_ROWS   20
#define SYSDISP_CHAR_NUM    (SYSDISP_CHAR_COLS * SYSDISP_CHAR_ROWS)

uint8_t sys_disp_chars[SYSDISP_CHAR_NUM];
uint8_t sys_disp_changemap[SYSDISP_CHAR_NUM / 8];

//-----------------------------------------------------------------------------

bool gfx_display_init()
{
	gfx_display.pin_reset.Assign(PORTNUM_A, 1, false);
	gfx_display.pin_reset.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);

	gfx_display.pin_cs.Assign(PORTNUM_A, 0, false);
	gfx_display.pin_cs.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);

	gfx_display.pin_cd.Assign(PORTNUM_A, 2, false);
	gfx_display.pin_cd.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);

	// SPI1

	hwpinctrl.PinSetup(PORTNUM_A, 5, PINCFG_OUTPUT | PINCFG_AF_5); // SPI1_SCK
	hwpinctrl.PinSetup(PORTNUM_A, 7, PINCFG_OUTPUT | PINCFG_AF_5); // SPI1_MOSI
	//disp.txdma.Init(1, 3, 1);  // SPI1/TX = DMA channel 3

	gfx_display.spi.idleclk_high = false;
	gfx_display.spi.datasample_late = false;
	gfx_display.spi.speed = 8000000; // 4 MHz
	gfx_display.spi.Init(1);

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

// sysdisplayhw_armpofo.cpp (f303)

#include "clockcnt.h"
#include "hwpins.h"
#include "traces.h"
#include "sysdisplayhw.h"

//-----------------------------------------------------------------------------
// Text Display

#define SYSDISP_CHAR_COLS   40
#define SYSDISP_CHAR_ROWS    8
#define SYSDISP_CHAR_NUM    (SYSDISP_CHAR_COLS * SYSDISP_CHAR_ROWS)

uint8_t sys_disp_chars[SYSDISP_CHAR_NUM];
uint8_t sys_disp_changemap[SYSDISP_CHAR_NUM / 8];

//-----------------------------------------------------------------------------

bool TSysDisplayHw::InitHw()
{
	InitCharBuf(SYSDISP_CHAR_COLS, SYSDISP_CHAR_ROWS, &sys_disp_chars[0], &sys_disp_changemap[0]);

	pin_inst.Assign(PORTNUM_E, 7, false); // aka RS
	pin_en.Assign(PORTNUM_B, 1, false);
	pin_cs.Assign(PORTNUM_D, 9, false);
	pin_wr.Assign(PORTNUM_D, 8, false);
	pin_audio.Assign(PORTNUM_D, 10, false);
	pin_onoff.Assign(PORTNUM_C,  6, true);

	pin_inst.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	pin_en.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);
	pin_cs.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);
	pin_wr.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);
	pin_audio.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);
	pin_onoff.Setup(PINCFG_OUTPUT | PINCFG_OPENDRAIN | PINCFG_GPIO_INIT_0);  // open drain because of the 5V !

	// Data Bus pins:
	hwpinctrl.PinSetup(PORTNUM_E,  8, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_E,  9, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_E, 10, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_E, 11, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_E, 12, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_E, 13, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_E, 14, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_E, 15, PINCFG_OUTPUT);
	data_set_clr_reg = &(GPIOE->BSRR);

	// contrast PWM
	hwpinctrl.PinSetup(PORTNUM_B, 0, PINCFG_AF_2); // select TIM3_CH3, open drain because of the 5V !
	pwm_contrast.frequency = 10000;
	pwm_contrast.Init(3, 3, 0); // Timer2/Ch2/output0
	pwm_contrast.SetOnClocks((pwm_contrast.periodclocks * 80) / 100);
	pwm_contrast.Enable();


	// ganaric setup

	delay_us(10000);
	AudioInit();

	delay_us(100000); // wait for the start, and give some time to the PWM to charge the LCD circuits

	LcdInit();

	delay_us(10000); // repeat, sometimes at cold start the sound keeps running
	AudioInit();

	return true;
}

inline void TSysDisplayHw::LcdSetData(uint8_t adata)
{
	uint32_t rv = (adata | ((adata ^ 0xFF) << 16));
	*data_set_clr_reg = (rv << 8);
}

void TSysDisplayHw::LcdInit()
{
	// clear all registers
	for (int i = 0; i < 16; ++i)
	{
		LcdWrite(i, 0x00);
	}

	LcdWrite(0x01, 0x75); // character pitch: 6x8
	LcdWrite(0x04,    7); // cursor position: 7 = last character row
	LcdWrite(0x02,   39); // horizontal character count: 40 (for character mode only)
	LcdWrite(0x08,    0); // start address low
	LcdWrite(0x09,    0); // start address high

	LcdWrite(0x03,   64); // duty: number of rows - 1
	LcdWrite(0x00, mode_base); // display mode: 0x3D = display on, master mode, character mode with external generator, cursor blink

	LcdWrite(0x01, 0x75); // character pitch: 6x8
}

void TSysDisplayHw::AudioInit()
{
	// reset audio
	LcdSetData(0x40);
	delay_us(2);
	pin_audio.Set1();
	delay_us(10);
	LcdSetData(0x00);
	delay_us(10);
	pin_audio.Set0();
}

void TSysDisplayHw::LcdSetAddress(unsigned x, unsigned y)
{
  unsigned address;
  address = y * 40 + x;
  LcdWrite(0x0A, address & 0xFF);    // writeadress low
  LcdWrite(0x0B, address >> 8);      // writeadress high

  prevaddr = address;
}

void TSysDisplayHw::LcdSetAddress(unsigned address)
{
  LcdWrite(0x0A, (address & 0xFF));    // writeadress low
  LcdWrite(0x0B, (address >> 8));      // writeadress high

  prevaddr = address;
}

void TSysDisplayHw::DrawChar(unsigned acol, unsigned arow, char ach)
{
	unsigned addr = AddrFromColRow(acol, arow);
#if 1
	if (prevaddr + 1 != addr)
	{
		LcdSetAddress(addr);
	}
	else
	{
		++prevaddr;
	}

	if (prevcmd != 0x0C)
	{
		LcdWrite(0x0C, uint8_t(ach));
	}
	else
	{
		LcdWriteData(uint8_t(ach));
	}

#else

	LcdSetAddress(addr);
	LcdWrite(0x0C, uint8_t(ach));

#endif
}

void TSysDisplayHw::LcdWrite(uint8_t acmd, uint8_t adata)
{
	unsigned us_clockcnt = (SystemCoreClock / 1000000);
	unsigned rs_wait_clocks = (us_clockcnt >> 2); // 0.25 us
	unsigned e_wait_clocks = (us_clockcnt >> 1); // 0.5 us

	pin_cs.Set0();
	pin_en.Set0();

	// command part

	LcdSetData(acmd);
	pin_inst.Set1();
	delay_clocks(rs_wait_clocks); // min 0.2 us required
	pin_en.Set1(); // rise the E
	delay_clocks(e_wait_clocks); // min 0.5 us
	pin_en.Set0(); // E = 0
	delay_clocks(e_wait_clocks); // min 0.5 us

  // data part

	LcdSetData(adata);
	pin_inst.Set0();
	delay_clocks(rs_wait_clocks); // min 0.2 us required
	pin_en.Set1(); // rise the E
	delay_clocks(e_wait_clocks); // min 0.5 us
	pin_en.Set0(); // E = 0
	delay_clocks(e_wait_clocks); // min 0.5 us

  delay_us(6);  // let some time to execute

  prevcmd = acmd;
}

void TSysDisplayHw::LcdWriteData(uint8_t adata)
{
	unsigned us_clockcnt = (SystemCoreClock / 1000000);
	unsigned rs_wait_clocks = (us_clockcnt >> 2); // 0.25 us
	unsigned e_wait_clocks = (us_clockcnt >> 1); // 0.5 us

	LcdSetData(adata);
	pin_inst.Set0();
	delay_clocks(rs_wait_clocks); // min 0.2 us required
	pin_en.Set1(); // rise the E
	delay_clocks(e_wait_clocks); // min 0.5 us
	pin_en.Set0(); // E = 0
	delay_clocks(e_wait_clocks); // min 0.5 us

  delay_us(4);  // let some time to execute
}

void TSysDisplayHw::LcdFill(uint8_t adata)
{
	LcdSetAddress(0);

	for (unsigned n = 0; n < 40*8; ++n)
	{
		LcdWrite(0x0C, adata);
	}
}

void TSysDisplayHw::SetCursor()
{
	if (cursor_on)
	{
		if (!prev_cursor_on)
		{
			LcdWrite(0x00, mode_base | 0xC); // turn on blinking cursor
			prev_cursor_on = true;
		}
		LcdSetAddress(cursor_x, cursor_y);
	}
	else
	{
		if (prev_cursor_on)
		{
			LcdWrite(0x00, mode_base);
			prev_cursor_on = false;
		}
	}
}


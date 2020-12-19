/*
 * pofodisplay.cpp
 *
 *  Created on: 13 Nov 2020
 *      Author: vitya
 */

#include <pofodisplay.h>
#include "clockcnt.h"

TPofoDisplay   g_display;

#define POFO_DISP_CHAR_NUM  (40*8)

uint8_t pofo_disp_chars[POFO_DISP_CHAR_NUM];
uint8_t pofo_disp_changemap[POFO_DISP_CHAR_NUM / 8];

bool TPofoDisplay::Init()
{
	TTextScreen::Init(40, 8, &pofo_disp_chars[0], &pofo_disp_changemap[0]);

	pwm_contrast.frequency = 10000;

	if (!InitHw())
	{
		return false;
	}

	// HW intependent setup:

	pin_inst.Setup(PINCFG_OUTPUT);
	pin_en.Setup(PINCFG_OUTPUT);
	pin_cs.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);
	pin_wr.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);
	pin_audio.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_0);

	// Contrast PWM

	pwm_contrast.SetOnClocks((pwm_contrast.periodclocks * 80) / 100);
	pwm_contrast.Enable();

	delay_us(10000);

	AudioInit();

	delay_us(100000); // wait for the start, and give some time to the PWM to charge the LCD circuits

	LcdInit();

	Clear();

	Update();

	return true;
}

#if defined(BOARD_MIBO48_STM32F303)

// display test set with a bread board

// connections to MIBO48-F303:
//----------------------------
// Display data
//  A0-A7: LCD4-11: D0-D7 (Bidirectional)
// Control Signals
//   B3: LCD12: ??? LCD common control, contrast, PWM required !
//   B4: LCD14: RS (1 = instruction, 0 = data)
//   B5: LCD15: E (data strobe)
//   B6: LCD16: CS (active low)
//   B7: LCD17: Data R/~W, inverted ! 1 = write, 0 = read ?
// Not connected:
//   B8: LCD13: sound LATCH Enable (74HC373)

bool TPofoDisplay::InitHw()
{
	pin_inst.Assign(PORTNUM_B, 4, false);
	pin_en.Assign(PORTNUM_B, 5, false);
	pin_cs.Assign(PORTNUM_B, 6, false);
	pin_wr.Assign(PORTNUM_B, 7, false);
	pin_audio.Assign(PORTNUM_B, 8, false);

	// Data Bus pins:
	hwpinctrl.PinSetup(PORTNUM_A, 0, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_A, 1, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_A, 2, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_A, 3, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_A, 4, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_A, 5, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_A, 6, PINCFG_OUTPUT);
	hwpinctrl.PinSetup(PORTNUM_A, 7, PINCFG_OUTPUT);

	data_set_clr_reg = &(GPIOA->BSRR);

	hwpinctrl.PinSetup(PORTNUM_B, 3, PINCFG_AF_1); // select TIM2_CH2

	pwm_contrast.Init(2, 2, 0); // Timer2/Ch2/output0

	return true;
}

inline void TPofoDisplay::LcdSetData(uint8_t adata)
{
	uint32_t rv = (adata | ((~adata & 0xFF) << 16));
	*data_set_clr_reg = rv;
}

#else
  #error "unimplemented board"
#endif

void TPofoDisplay::LcdInit()
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

void TPofoDisplay::AudioInit()
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

void TPofoDisplay::LcdSetAddress(unsigned x, unsigned y)
{
  unsigned address;
  address = y * 40 + x;
  LcdWrite(0x0A, address & 0xFF);    // writeadress low
  LcdWrite(0x0B, address >> 8);      // writeadress high

  prevaddr = address;
}

void TPofoDisplay::LcdSetAddress(unsigned address)
{
  LcdWrite(0x0A, (address & 0xFF));    // writeadress low
  LcdWrite(0x0B, (address >> 8));      // writeadress high

  prevaddr = address;
}

void TPofoDisplay::DrawChar(unsigned aaddr, char ach)
{
#if 1
	if (prevaddr + 1 != aaddr)
	{
		LcdSetAddress(aaddr);
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

	LcdSetAddress(aaddr);
	LcdWrite(0x0C, uint8_t(ach));

#endif
}

void TPofoDisplay::LcdWrite(uint8_t acmd, uint8_t adata)
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

void TPofoDisplay::LcdWriteData(uint8_t adata)
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

void TPofoDisplay::LcdFill(uint8_t adata)
{
	LcdSetAddress(0);

	for (unsigned n = 0; n < 40*8; ++n)
	{
		LcdWrite(0x0C, adata);
	}
}

void TPofoDisplay::SetCursor()
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

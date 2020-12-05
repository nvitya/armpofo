// syskeyboard_e51.cpp

#include "syskeyboard.h"

// special conversion table for the 4 x 4 foil keyboard

struct TSymConvItem
{
	uint16_t   shiftbits;
	uint16_t   sym_normal;
	uint16_t   sym_shift;
	uint16_t   sym_alt;
};

TSymConvItem sym_conv_table[4 * 4] =
{
//  shift		      normal,          shift,            alt
		{0x00,           '1',            '+',            'A'},  // 1
		{0x00,           '2',            '-',            'B'},  // 2
		{0x00,           '3',            '*',            'C'},  // 3
		{KEYSHIFT_SHIFT,  0,               0,              0},  // A

		{0x00,           '4',            '/',            'D'},  // 4
		{0x00,           '5',            '^',            'E'},  // 5
		{0x00,           '6',            '=',            'F'},  // 6
		{KEYSHIFT_ALT,    0,               0,              0},  // B

		{0x00,           '7',            '(',            'X'},  // 7
		{0x00,           '8',            ')',            'Y'},  // 8
		{0x00,           '9',            ',',            'Z'},  // 9
		{0x00, KEYSYM_BACKSPACE,   KEYSYM_BACKSPACE,   KEYSYM_BACKSPACE},  // C

		{0x00,   KEYSYM_LEFT,    KEYSYM_DOWN,    KEYSYM_PGDN},  // *
		{0x00,           '0',            '.',            '"'},  // 0
		{0x00,  KEYSYM_RIGHT,      KEYSYM_UP,    KEYSYM_PGUP},  // #
		{0x00,  KEYSYM_ENTER,   KEYSYM_ENTER,   KEYSYM_ENTER},  // D

};

bool TSysKeyboard::InitHw()
{

	colpin[0].Assign(PORTNUM_B, 14, false);
	colpin[1].Assign(PORTNUM_B, 15, false);
	colpin[2].Assign(PORTNUM_A, 12, false);
	colpin[3].Assign(PORTNUM_A, 13, false);

	rowpin[0].Assign(PORTNUM_B, 10, false);
	rowpin[1].Assign(PORTNUM_B, 11, false);
	rowpin[2].Assign(PORTNUM_B, 12, false);
	rowpin[3].Assign(PORTNUM_B, 13, false);

  if (!TKeyMatrix::Init(4, 4))
  {
  	return false;
  }

  return true;
}

void TSysKeyboard::ProcessScanEvent(uint8_t aevtype, uint8_t ascancode)
{
	TSymConvItem * psc = &sym_conv_table[ascancode];
	if (psc->shiftbits)
	{
		if (aevtype == KEYSCAN_EVENT_PUSH)
		{
			shiftstate |= psc->shiftbits;
		}
		else
		{
			shiftstate &= ~psc->shiftbits;
		}
	}
	else if (aevtype == KEYSCAN_EVENT_PUSH)
	{
		if (shiftstate & KEYSHIFT_SHIFT)
		{
			AddSymEvent(psc->sym_shift);
		}
		else if (shiftstate & KEYSHIFT_ALT)
		{
			AddSymEvent(psc->sym_alt);
		}
		else
		{
			AddSymEvent(psc->sym_normal);
		}
	}
}




// syskeyboard_test_f303.cpp

#include "syskeyboard.h"

// special conversion table for the 4 x 5 foil keyboard

struct TSymConvItem
{
	uint16_t   shiftbits;
	uint16_t   sym_normal;
	uint16_t   sym_shift;
	uint16_t   sym_alt;
};

TSymConvItem sym_conv_table[5 * 4] =
{
//  shift		      normal,          shift,            alt
		{0x00,   KEYSYM_LEFT,    KEYSYM_LEFT,    KEYSYM_HOME},
		{0x00,           '0',            '.',            '0'},
		{0x00,  KEYSYM_RIGHT,   KEYSYM_RIGHT,     KEYSYM_END},
		{0x00,  KEYSYM_ENTER,   KEYSYM_ENTER,   KEYSYM_ENTER},

		{0x00,           '7',            '7',            '7'},
		{0x00,           '8',            ',',            '8'},
		{0x00,           '9',            '9',            '9'},
		{0x00, KEYSYM_BACKSPACE,   KEYSYM_BACKSPACE,   KEYSYM_BACKSPACE},

		{0x00,           '4',            '4',            '4'},
		{0x00,           '5',            '5',            '5'},
		{0x00,           '6',            '6',            '6'},
		{0x00,   KEYSYM_DOWN,    KEYSYM_DOWN,   KEYSYM_DOWN},

		{0x00,           '1',            '1',            '1'},
		{0x00,           '2',            '(',            '2'},
		{0x00,           '3',            ')',            '3'},
		{0x00,     KEYSYM_UP,    KEYSYM_UP,   KEYSYM_UP},

		{KEYSHIFT_SHIFT,  0,               0,              0},
		{KEYSHIFT_ALT,    0,               0,              0},
		{0x00,           '+',            '-',            '^'},
		{0x00,           '*',            '/',            '^'},
};

bool TSysKeyboard::InitHw()
{

	colpin[0].Assign(PORTNUM_B,  0, false);
	colpin[1].Assign(PORTNUM_B,  1, false);
	colpin[2].Assign(PORTNUM_B,  2, false);
	colpin[3].Assign(PORTNUM_B, 10, false);

	rowpin[0].Assign(PORTNUM_B, 11, false);
	rowpin[1].Assign(PORTNUM_B, 12, false);
	rowpin[2].Assign(PORTNUM_B, 13, false);
	rowpin[3].Assign(PORTNUM_B, 14, false);
	rowpin[4].Assign(PORTNUM_B, 15, false);

  if (!TKeyMatrix::Init(5, 4))
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




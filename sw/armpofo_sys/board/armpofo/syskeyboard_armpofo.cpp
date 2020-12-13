// syskeyboard_armpofo.cpp

#include "syskeyboard.h"

// special conversion table for the Atari Portfolio 8 x 8 keyboard matrix

struct TSymConvItem
{
	uint16_t   sym_normal;
	uint16_t   sym_shift;
	uint16_t   sym_alt;
	uint16_t   sym_ctrl;
	uint16_t   sym_fn;
	uint16_t   sym_sys;
	uint16_t   shiftbits;
};

const TSymConvItem sym_conv_table[8 * 8] =
{
//                {normal, shift+, alt+, ctrl+, fn+, sys+, shiftbits}

/*R1 C1 FN      */{0, 0, 0, 0, 0, 0, KEYSHIFT_FN},
/*R1 C2 DEL/INS */{KEYSYM_DELETE, 0, 0, 0, KEYSYM_INSERT, 0, 0},
/*R1 C3 TAB     */{KEYSYM_TAB, 0, 0, 0, 0, 0, 0},
/*R1 C4 0       */{'0', ')', 0, 0, KEYSYM_F10, '+', 0},
/*R1 C5 S       */{'s', 'S', 0, 0, 0, 0, 0},
/*R1 C6 F       */{'f', 'F', 0, 0, 0, 0, 0},
/*R1 C7 \|      */{'\\', '|', 0, 0, 0, 0, 0},
/*R1 C8 C       */{'c', 'C', 0, 0, 0, 0, 0},

/*R2 C1         */{0, 0, 0, 0, 0, 0, 0}, // not existing
/*R2 C2 Alt     */{0, 0, 0, 0, 0, 0, KEYSHIFT_ALT},
/*R2 C3 W       */{'w', 'W', 0, 0, 0, 0, 0},
/*R2 C4 I       */{'i', 'I', 0, 0, 0, '5', 0},
/*R2 C5 P       */{'p', 'P', 0, 0, 0, '*', 0},
/*R2 C6 H       */{'h', 'H', 0, 0, 0, 0, 0},
/*R2 C7 Z       */{'z', 'Z', 0, 0, 0, 0, 0},
/*R2 C8 V       */{'v', 'V', 0, 0, 0, 0, 0},

/*R3 C1 1       */{'1', '!', 0, 0, KEYSYM_F1, 0, 0},
/*R3 C2 Q       */{'q', 'Q', 0, 0, 0, 0, 0},
/*R3 C3 Ctrl    */{0, 0, 0, 0, 0, 0, KEYSHIFT_CTRL},
/*R3 C4 -_      */{'-', '_', 0, 0, 0, 0, 0},
/*R3 C5 4       */{'4', '$', 0, 0, KEYSYM_F4, 0, 0},
/*R3 C6 J       */{'j', 'J', 0, 0, 0, '1', 0},
/*R3 C7 Space   */{' ', 0, 0, 0, 0, 0, 0},
/*R3 C8 B       */{'b', 'B', 0, 0, 0, 0, 0},

/*R4 C1 2       */{'2', '@', 0, 0, KEYSYM_F2, 0, 0},
/*R4 C2 U       */{'u', 'U', 0, 0, 0, '4', 0},
/*R4 C3 E       */{'e', 'E', 0, 0, 0, 0, 0},
/*R4 C4 LSHIFT  */{0, 0, 0, 0, 0, 0, KEYSHIFT_SHIFT},
/*R4 C5 G       */{'g', 'G', 0, 0, 0, 0, 0},
/*R4 C6 LEFT    */{KEYSYM_LEFT, 0, 0, 0, KEYSYM_HOME, 0, 0},
/*R4 C7 ;:      */{';', ':', 0, 0, 0, '-', 0},
/*R4 C8 N       */{'n', 'N', 0, 0, 0, 0, 0},

/*R5 C1 3       */{'3', '#', 0, 0, KEYSYM_F3, 0, 0},
/*R5 C2 O       */{'o', 'O', 0, 0, KEYSYM_SLEEP, '6', 0},
/*R5 C3 R       */{'r', 'R', 0, 0, KEYSYM_RESET, 0, 0},
/*R5 C4 [       */{'[', '{', 0, 0, 0, 0, 0},
/*R5 C5 RSHIFT  */{0, 0, 0, 0, 0, 0, KEYSHIFT_SHIFT},
/*R5 C6 RIGHT   */{KEYSYM_RIGHT, 0, 0, 0, KEYSYM_END, 0, 0},
/*R5 C7 .>      */{'.', '>', 0, 0, 0, '.', 0},
/*R5 C8 M       */{'m', 'M', 0, 0, 0, '0', 0},

/*R6 C1 D       */{'d', 'D', 0, 0, 0, 0, 0},
/*R6 C2 7       */{'7', '&', 0, 0, KEYSYM_F7, '7', 0},
/*R6 C3 T       */{'t', 'T', 0, 0, 0, 0, 0},
/*R6 C4 UP      */{KEYSYM_UP, 0, 0, 0, KEYSYM_PGUP, 0, 0},
/*R6 C5 DOWN    */{KEYSYM_DOWN, 0, 0, 0, KEYSYM_PGDN, 0, 0},
/*R6 C6 CAPS    */{0, 0, 0, 0, 0, 0, KEYSHIFT_LOCK_CAPS},
/*R6 C7 =+      */{'=', '+', 0, 0, 0, 0, 0},
/*R6 C8 /?      */{'/', '?', 0, 0, 0, '/', 0},

/*R7 C1 5       */{'5', '%', 0, 0, KEYSYM_F5, 0, 0},
/*R7 C2 BACKSP. */{KEYSYM_BACKSPACE, 0, 0, 0, 0, 0, 0},
/*R7 C3 ENTER   */{KEYSYM_ENTER, 0, 0, 0, 0, '=', 0},
/*R7 C4 '"      */{'\'', '\"', '`', 0, 0, 0, 0},
/*R7 C5 ,<      */{',', '<', 0, 0, 0, '.', 0},
/*R7 C6 8       */{'8', '*', 0, 0, KEYSYM_F8, '8', 0},
/*R7 C7 Atari   */{0, 0, 0, 0, 0, 0, KEYSHIFT_SYS},
/*R7 C8 A       */{'a', 'A', 0, 0, 0, 0, 0},

/*R8 C1 6       */{'6', '^', 0, 0, KEYSYM_F6, '6', 0},
/*R8 C2 9       */{'9', '(', 0, 0, KEYSYM_F9, '9', 0},
/*R8 C3 Y       */{'y', 'Y', 0, 0, 0, 0, 0},
/*R8 C4 ]       */{']', '}', '~', 0, 0, 0, 0},
/*R8 C5 L       */{'l', 'L', 0, 0, 0, '3', 0},
/*R8 C6 K       */{'k', 'K', 0, 0, 0, '2', 0},
/*R8 C7 X       */{'x', 'X', 0, 0, 0, 0, 0},
/*R8 C8 ESC     */{KEYSYM_ESC, 0, 0, 0, 0, 0, 0},

};


bool TSysKeyboard::InitHw()
{

	colpin[0].Assign(PORTNUM_B,  4, false);
	colpin[1].Assign(PORTNUM_B,  3, false);
	colpin[2].Assign(PORTNUM_D,  7, false);
	colpin[3].Assign(PORTNUM_D,  6, false);
	colpin[4].Assign(PORTNUM_D,  5, false);
	colpin[5].Assign(PORTNUM_D,  4, false);
	colpin[6].Assign(PORTNUM_D,  3, false);
	colpin[7].Assign(PORTNUM_A, 15, false);

	rowpin[0].Assign(PORTNUM_E,  6, false);
	rowpin[1].Assign(PORTNUM_E,  5, false);
	rowpin[2].Assign(PORTNUM_E,  4, false);
	rowpin[3].Assign(PORTNUM_E,  3, false);
	rowpin[4].Assign(PORTNUM_E,  1, false);
	rowpin[5].Assign(PORTNUM_E,  0, false);
	rowpin[6].Assign(PORTNUM_B,  9, false);
	rowpin[7].Assign(PORTNUM_B,  8, false);

  if (!TKeyMatrix::Init(8, 8))
  {
  	return false;
  }

  return true;
}

void TSysKeyboard::ProcessScanEvent(uint8_t aevtype, uint8_t ascancode)
{
	const TSymConvItem * psc = &sym_conv_table[ascancode];
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
		uint16_t sym = 0;

		if (shiftstate & KEYSHIFT_SHIFT)
		{
			sym = psc->sym_shift;
		}
		else if (shiftstate & KEYSHIFT_ALT)
		{
			sym = psc->sym_alt;
		}
		else if (shiftstate & KEYSHIFT_FN)
		{
			sym = psc->sym_fn;
		}
		else if (shiftstate & KEYSHIFT_SYS)
		{
			sym = psc->sym_sys;
		}
		else
		{
			sym = psc->sym_normal;
		}

		if (sym)
		{
			if (shiftstate & KEYSHIFT_CTRL)
			{
				sym |= KEYSYM_CTRL;
			}

			AddSymEvent(sym);
		}
	}
}




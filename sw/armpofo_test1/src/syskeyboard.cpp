// pofokeyboard.cpp

#include "stddef.h"
#include <syskeyboard.h>
#include "systimer.h"

TSysKeyboard        g_keyboard;
TKeyScanEventBuf    g_keyscan_events;
TKeySymbolEventBuf  g_keysym_events;

#if defined(BOARD_MIBO48_STM32F303)

// special conversion table for the

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

#else
  #error "Unimplemented playform keyboard"
#endif

bool TSysKeyboard::Init()
{
	scan_events = &g_keyscan_events;
	scan_events->max_events = MAX_KEYSCANEVENTS;
	scan_events->headsize = offsetof(TKeyScanEventBuf, events);
	scan_events->serial = 0;

	sym_events = &g_keysym_events;
	sym_events->max_events = MAX_KEYSYMEVENTS;
	sym_events->headsize = offsetof(TKeySymbolEventBuf, events);
	sym_events->serial = 0;

	if (!InitHw())
	{
		return false;
	}

  for (unsigned n = 0; n < KEY_PUSHMAP_DWORDS; ++n)
  {
  	prev_pushmap[n] = keys32[n];
  }

  shiftstate = 0;

	return true;
}

void TSysKeyboard::Run()
{
	super::Run();

	// check and generate events

  for (unsigned n = 0; n < KEY_PUSHMAP_DWORDS; ++n)
  {
  	uint32_t kdiff = (prev_pushmap[n] ^ keys32[n]);
  	while (kdiff)
  	{
  		unsigned bitnum = __CLZ(kdiff);
  		if (bitnum > 31)  break;
  		bitnum = (31 - bitnum);

  		unsigned bitmask = (1u << bitnum);
  		unsigned scancode = n*32 + bitnum;

  		if (keys32[n] & bitmask)
  		{
  			AddScanEvent(KEYSCAN_EVENT_PUSH, scancode);
  		}
  		else
  		{
  			AddScanEvent(KEYSCAN_EVENT_RELEASE, scancode);
  		}

  		kdiff &= ~(bitmask);
  	}

  	prev_pushmap[n] = keys32[n];
  }
}

void TSysKeyboard::AddScanEvent(uint8_t aevtype, uint8_t ascancode)
{
	uint16_t next_serial = scan_events->serial + 1;
	TKeyScanEvent * pevent = &scan_events->events[next_serial & (MAX_KEYSCANEVENTS - 1)];
	pevent->evtime = g_sysms;
	pevent->evtype = aevtype;
	pevent->scancode = ascancode;
	scan_events->serial = next_serial;

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

void TSysKeyboard::AddSymEvent(uint16_t asymbol)
{
	uint16_t next_serial = sym_events->serial + 1;
	TKeySymbolEvent * pevent = &sym_events->events[next_serial & (MAX_KEYSYMEVENTS - 1)];
	pevent->evtime = g_sysms;
	pevent->symbol = asymbol;
	pevent->shiftstate = shiftstate;
	sym_events->serial = next_serial;
}

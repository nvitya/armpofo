// pofokeyboard.cpp

#include "stddef.h"
#include <syskeyboard.h>
#include "systimer.h"

TSysKeyboard        g_keyboard;
TKeyScanEventBuf    g_keyscan_events;
TKeySymbolEventBuf  g_keysym_events;


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

	if (!InitHw()) //  board dependent
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

	ProcessScanEvent(aevtype, ascancode); // board dependent
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

/*
 * textscreen.cpp
 *
 *  Created on: 2017. szept. 22.
 *      Author: vitya
 */

#include "string.h"
#include <stdio.h>
#include <stdarg.h>
#include <textscreen.h>

void TTextScreen::Init(unsigned acols, unsigned arows, uint8_t * ascreenbuf, uint8_t * achangemap)
{
	rows = arows;
	cols = acols;
	screenbuf = ascreenbuf;
	changemap = (uint32_t *)achangemap;

	cposx = 0;
	cposy = 0;

	// fill the text buffer with spaces
	Clear();
}

void TTextScreen::SetScreenBufChar(unsigned aaddr, char ach)
{
	if (screenbuf[aaddr] != ach)
	{
		screenbuf[aaddr] = uint8_t(ach);
		changemap[aaddr >> 5] |= (1 << (aaddr & 31));
		screenchanged = true;
	}
}

void TTextScreen::WriteChar(char ach)
{
	unsigned row, col;
	unsigned addr, endaddr;

	if (ach == 10)
	{
		++cposy;
		cposx = 0;
	}
	else if (uint8_t(ach) < 32)
	{
		// skip special chars
	}
	else
	{
		// handle scrolling now to have a filled last row, and allowing to write to the right bottom corner

		if (cposx >= cols)
		{
			cposx = 0;
			++cposy;
		}

		if (cposy >= rows)
		{
			// scroll the screen
			addr = 0;
			endaddr = (rows * cols) - cols;
			while (addr < endaddr)
			{
				SetScreenBufChar(addr, screenbuf[addr + cols]);
				++addr;
			}

			// fill the last row with spaces
			endaddr += cols;
			while (addr < endaddr)
			{
				SetScreenBufChar(addr, 32);
				++addr;
			}

			cposy = rows - 1;
			cposx = 0;
		}

		SetScreenBufChar((cposy * cols) + cposx, ach);
		++cposx;
	}
}

void TTextScreen::Update()
{
	if (!screenchanged)
	{
		SetCursor();
		return;
	}

	// run trough the change map and draw changed characters on the screen

	unsigned addr = 0;
	unsigned endaddr = rows * cols;
	while (addr < endaddr)
	{
		unsigned cmaddr = (addr >> 5);
		if (changemap[cmaddr])
		{
			if (changemap[cmaddr] & (1 << (addr & 31)))
			{
				// character changed, it must be drawn
				DrawChar(addr, screenbuf[addr]);

				changemap[cmaddr] &= ~(1 << (addr & 31));
			}
			++addr;
		}
		else
		{
			addr = ((addr+32) & ~31); // go to the next 32 bits
		}
	}

	SetCursor();

	screenchanged = false;
}

#define FMT_BUFFER_SIZE  256

void TTextScreen::printf(const char* fmt, ...)
{
  va_list arglist;
  va_start(arglist, fmt);
  char * pch;

  // allocate format buffer on the stack:
  char fmtbuf[FMT_BUFFER_SIZE];

  pch = &fmtbuf[0];

  *pch = 0;

  vsnprintf(pch, FMT_BUFFER_SIZE, fmt, arglist);

  while (*pch != 0)
  {
  	WriteChar(*pch);
    ++pch;
  }

  va_end(arglist);
}

void TTextScreen::Refresh()
{
	memset(changemap, 0xFF, (rows * cols) >> 8);
	screenchanged = true;
	Update();
}

void TTextScreen::Clear()
{
	cposx = 0;
	cposy = 0;

	memset(&screenbuf[0], 32, cols * rows);
	memset(&changemap[0], 0xFF, (cols * rows) >> 3);
	screenchanged = true;
}

void TTextScreen::SetPos(unsigned acol, unsigned arow)
{
	cposx = acol;
	cposy = arow;
}

void TTextScreen::DrawChar(unsigned aaddr, char ach)
{
  // must be implemented in the descendant
}

void TTextScreen::SetCursor()
{
	// can be implemented
}

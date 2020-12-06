// lineedit.cpp

#include "string.h"
#include "sysdisplay.h"
#include <commandline.h>
#include "exprcalc.h"
#include "hwclkctrl.h"
#include "board_config.h"
#include "powersave.h"

TExprCalc  g_calc;
TCalcVar   calcvar;

TCommandLine::~TCommandLine()
{
	// TODO Auto-generated destructor stub
}

void enter_powersave()
{
	g_display.TurnOnOff(false);
	pin_led1.Set0();

	// wait until all the keys are released

	while (g_keyboard.keys64 != 0)
	{
		g_keyboard.Run();
	}

	enter_low_power();
}

void TCommandLine::Run()
{
	unsigned n;
	if (prev_symserial != g_keysym_events.serial)
	{
		TKeySymbolEvent * psyme = &g_keysym_events.events[g_keysym_events.serial & (MAX_KEYSYMEVENTS-1)];

		#if 0
			g_display.SetPos(0, 6);
			g_display.printf("sym %i: ss=%02X, sym=%04X",
					 g_keysym_events.serial,
					 psyme->shiftstate,
					 psyme->symbol
			);
		#endif

		// some processing
		uint16_t keysym = psyme->symbol;
		if (KEYSYM_BACKSPACE == keysym)
		{
			if (editpos > 0)
			{
				DeletePos(editpos - 1);
				--editpos;
				--editlen;
			}
		}
		else if (KEYSYM_DELETE == keysym)
		{
			if (editpos < editlen)
			{
				DeletePos(editpos);
				--editlen;
			}
		}
		else if (KEYSYM_LEFT == keysym)
		{
			if (editpos > 0)
			{
				--editpos;
				CorrectStartPos();
			}
		}
		else if (KEYSYM_HOME == keysym)
		{
			editpos = 0;
			CorrectStartPos();
		}
		else if (KEYSYM_END == keysym)
		{
			editpos = editlen;
			CorrectStartPos();
		}
		else if (KEYSYM_RIGHT == keysym)
		{
			if (editpos < editlen)
			{
				++editpos;
				CorrectStartPos();
			}
		}
		else if ('l' + KEYSYM_CTRL == keysym)
		{
			Clear();
		}
		else if (KEYSYM_UP == keysym)
		{
			int newhistpos = (hist_load_pos > 0 ? hist_load_pos - 1 : CMDLINE_HIST_LEN - 1);
			if (hist[newhistpos].len > 0)
			{
				hist_load_pos = newhistpos;
				HistLoad(hist_load_pos);
			}
		}
		else if (KEYSYM_UP == keysym)
		{
			int newhistpos = (hist_load_pos > 0 ? hist_load_pos - 1 : CMDLINE_HIST_LEN - 1);
			if (hist[newhistpos].len > 0)
			{
				hist_load_pos = newhistpos;
				HistLoad(hist_load_pos);
			}
		}
		else if (KEYSYM_DOWN == keysym)
		{
			int newhistpos = (hist_load_pos >= CMDLINE_HIST_LEN - 1 ? 0 : hist_load_pos + 1);
			if (hist[newhistpos].len > 0)
			{
				hist_load_pos = newhistpos;
				HistLoad(hist_load_pos);
			}
			else
			{

			}
		}
		else if (KEYSYM_ENTER == keysym)
		{
			if (editlen > 0)
			{
				// save history
				HistSave(hist_pos);
				++hist_pos;
				if (hist_pos >= CMDLINE_HIST_LEN)  hist_pos = 0;
				hist_load_pos = hist_pos;

				// execute
				Draw();
				g_display.WriteChar(10); // new line

				Execute();

				g_display.WriteChar(10); // new line

				// reset
				Clear();
			}
		}
		else if (KEYSYM_RESET == keysym)
		{
			cpu_soft_reset();
		}
		else if (KEYSYM_SLEEP == keysym)
		{
		  enter_powersave();
		}
		else if ((editlen < CMDLINE_MAX_LENGTH) && (keysym >= 32) && (keysym <= 127))
		{
			if (editpos < editlen)
			{
				// make an insert
				n = editlen;
				while (n > editpos)
				{
					editrow[n] = editrow[n-1];
					--n;
				}
			}
			editrow[editpos] = keysym;
			++editlen;
			++editpos;

			CorrectStartPos();
		}

		Draw();

		prev_symserial = g_keysym_events.serial;
	}
}

void TCommandLine::Draw()
{
	// print editrow
	g_display.SetPos(0, g_display.rows-1);
	g_display.WriteChar('>');
	for (unsigned n = 0; n < g_display.cols-2; n++)
	{
		unsigned cpos = startpos + n;

		if (cpos < editlen)
		{
			g_display.WriteChar(editrow[cpos]);
		}
		else
		{
			g_display.WriteChar(32);
		}
	}

	g_display.cursor_on = true;
	g_display.cursor_x = 1+editpos-startpos;
	g_display.cursor_y = g_display.rows-1;
}

void TCommandLine::Clear()
{
	editpos = 0;
	editlen = 0;
	editrow[0] = 0;
}

void TCommandLine::Execute()
{
	int err = g_calc.Evaluate(&editrow[0], editlen, &calcvar);
	if (err)
	{
		g_display.printf("= [error %i]\n", err);
	}
	else
	{
		g_display.printf("= %0.8f\n", calcvar.floatvalue);
	}
	g_display.WriteChar('>');
}

void TCommandLine::DeletePos(unsigned apos)
{
	unsigned n = apos;
	while (n+1 < editlen)
	{
		editrow[n] = editrow[n+1];
		++n;
	}
}

void TCommandLine::CorrectStartPos()
{
	if (editpos < startpos)
	{
		startpos = editpos;
	}
	else if (editpos - startpos > g_display.cols-2)
	{
		startpos = editpos - (g_display.cols-2);
	}
}

void TCommandLine::HistSave(unsigned apos)
{
	if ((editlen <= 0) || (apos >= CMDLINE_HIST_LEN))
	{
		return;
	}

	memcpy(&hist[apos].row[0], &editrow[0], editlen);
	hist[apos].len = editlen;
}

void TCommandLine::HistLoad(unsigned apos)
{
	if (apos >= CMDLINE_HIST_LEN)
	{
		Clear();
		return;
	}

	editlen = hist[apos].len;
	memcpy(&editrow[0], &hist[apos].row[0], editlen);
	editpos = editlen;
	startpos = 0;
	CorrectStartPos();
}

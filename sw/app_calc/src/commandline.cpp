// lineedit.cpp

#include "string.h"
#include "sysif.h"
#include <commandline.h>
#include "exprcalc.h"
#include "hwclkctrl.h"
#include "clockcnt.h"
#include "traces.h"

TCommandLine  g_commandline;

TExprCalc     g_calc;
TCalcValue    calcvar;

void TCommandLine::Init()
{
	initialized = false;

	if (!psymevents)
	{
		psymevents = (TKeySymbolEventBuf *) (* psys_keyb_getptr)("KEYB_SYM_EVENTS");
		if (!psymevents)
		{
			TRACE("Error getting System Keyboard Symbol Events !\r\n");
			__BKPT();
		}
	}

	(* psys_disp_getinfo)(&dispinfo);

	initialized = true;
}

void TCommandLine::Run()
{
	unsigned n;

	if (!initialized)
	{
		Init();
		if (!initialized)
		{
			return;
		}
	}

	if (prev_symserial != psymevents->serial)
	{
		TKeySymbolEvent * psyme = &psymevents->events[psymevents->serial & (MAX_KEYSYMEVENTS-1)];

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
				(* psys_disp_writechar)(10); // new line

				Execute();

				//g_display.WriteChar(10); // new line

				// reset
				Clear();
			}
		}
#if 0	 // handled in the system level
		else if (KEYSYM_RESET == keysym)
		{
			cpu_soft_reset();
		}
		else if (KEYSYM_SLEEP == keysym)
		{
		  enter_powersave();
		}
#endif
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

		prev_symserial = psymevents->serial;
	}
	else
	{
		(* psys_disp_getinfo)(&dispinfo);
		if (!dispinfo.cursor_on)
		{
			(* psys_disp_setcursor)(1, dispinfo.cursor_x, dispinfo.cursor_y);
		}
	}
}

void TCommandLine::Draw()
{
	// print editrow
	(* psys_disp_getinfo)(&dispinfo);

	unsigned row = dispinfo.cposy;
	if (row >= dispinfo.rows-1)  row = dispinfo.rows-1;

	(* psys_disp_setpos)(0, row);
	(* psys_disp_writechar)('>');
	for (unsigned n = 0; n < dispinfo.cols-2; n++)
	{
		unsigned cpos = startpos + n;

		if (cpos < editlen)
		{
			(* psys_disp_writechar)(editrow[cpos]);
		}
		else
		{
			(* psys_disp_writechar)(32);
		}
	}

	(* psys_disp_setcursor)(1, 1+editpos-startpos, row);
}

void TCommandLine::Clear()
{
	editpos = 0;
	editlen = 0;
	editrow[0] = 0;
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
	else if (editpos - startpos > dispinfo.cols-2)
	{
		startpos = editpos - (dispinfo.cols-2);
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

bool TCommandLine::ExecInternalCommand()
{
	sp.Init(&editrow[0], editlen);
	sp.SkipSpaces();

	if (!sp.ReadIdentifier())
	{
		return false;
	}

	if (sp.UCComparePrev("EXIT"))
	{
		(* psys_printf)("Exiting...\n");
		exitcommand = true;
		return true;
	}

	return false;
}


void TCommandLine::Execute()
{
	// zero terminate the line for some c string routines
	editrow[editlen] = 0;

	if (ExecInternalCommand())
	{
		// print already done there
	}
	else // try to evaluate as math expression
	{
		if (g_calc.Evaluate(&editrow[0], editlen, &calcvar))
		{
			(* psys_printf)("= %0.8f", calcvar.floatvalue);
		}
		else
		{
			(* psys_printf)("ERR: %s", &g_calc.errormsg[0]);
		}
	}
	(* psys_printf)("\n>");
}

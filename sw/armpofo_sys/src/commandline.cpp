// lineedit.cpp

#include "string.h"
#include "sysdisplay.h"
#include <commandline.h>
#include "exprcalc.h"
#include "hwclkctrl.h"
#include "board_config.h"
#include "powersave.h"
#include "clockcnt.h"
#include "sysproc.h"

#include "battery.h"
#include "extflash.h"
#include "traces.h"

TExprCalc  g_calc;
TCalcValue   calcvar;

TCommandLine::~TCommandLine()
{
	// TODO Auto-generated destructor stub
}

void TCommandLine::Run()
{
	unsigned n;

	g_display.cursor_on = true;

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

		prev_symserial = g_keysym_events.serial;
	}
}

void TCommandLine::Draw()
{
	// print editrow

	unsigned row = g_display.cposy;
	if (row >= g_display.rows-1)  row = g_display.rows-1;

	g_display.SetPos(0, row);
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
	g_display.cursor_y = row;
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

void show_battery_status()
{
	uint16_t lastkeyserial = g_keysym_events.serial;
	unsigned st = CLOCKCNT - SystemCoreClock;
	unsigned t;

	g_display.cursor_on = false;

	while (lastkeyserial == g_keysym_events.serial)
	{
		t = CLOCKCNT;

		if (t - st > SystemCoreClock)
		{
		  g_display.printf("BAT: %i mV / ", battery_get_u_bat());
		  g_display.printf("%i mA, ", battery_get_i_charge());
		  g_display.printf("Supp = %u mV\n", battery_get_u_5V());
		  st = t;
		}

		sys_run();
	}
}

bool TCommandLine::ExecApplication()
{
	sp.Init(&editrow[0], editlen);
	sp.SkipSpaces();

	if (!sp.ReadIdentifier())
	{
		return false;
	}

	// then UC check will be used

	// scan trough all the app slots for the existing application
	TAppHeader fheader;

	unsigned   faddr = SYSIF_APP_FLASH_START;
	while (faddr < SYSIF_APP_FLASH_END)
	{
		g_extflash.StartReadMem(faddr, &fheader, sizeof(fheader));
		g_extflash.WaitForComplete();
		if ( !g_extflash.errorcode
			   && (fheader.header_checksum == sys_header_checksum(&fheader))
			 )
		{
			if (sp.UCComparePrev((const char *)&fheader.name[0]))
			{
				// load the application
				TRACE("Executing App: %s\r\n", &fheader.name[0]);

				g_extflash.StartReadMem(faddr, (void *)SYSIF_APP_LOAD_ADDR, sizeof(fheader) + fheader.content_length);
				g_extflash.WaitForComplete();

				//if (g_extflash.errorcode || (fheader.content_checksum != sys_content_checksum((void *)(fheader))

				PEntryFunc pentry = (PEntryFunc)(fheader.entry_point);

				(*pentry)(); // execute

				return true;
			}
		}

		faddr += SYSIF_APP_SIZE;
	}

	return false;
}

bool TCommandLine::ExecInternalCommand()
{
	sp.Init(&editrow[0], editlen);

	sp.SkipSpaces();

	if (sp.CheckSymbol("bat"))
	{
		show_battery_status();
		return true;
	}
	else if (sp.CheckSymbol("pwm"))
	{
		sp.SkipSpaces();
	  sp.CheckSymbol("(");  // optional
		sp.SkipSpaces();
		if (!sp.ReadDecimalNumbers())
		{
			g_display.printf(" Actual pwm duty: %i %%", battery_get_charge_percent());
			return true;
		}

		int arg = sp.PrevToInt();
		if ((arg < 0) || (arg > 100))
		{
			g_display.printf(" Argument out of range: 0-100");
			return true;
		}

		g_display.printf("Setting battery charge to %i %%.", arg);
		battery_set_charge_percent(arg);

		return true;
	}

	return false;
}


void TCommandLine::Execute()
{
	if (ExecInternalCommand())
	{
		// print already done there
	}
	else if (ExecApplication())
	{

	}
	else // try to evaluate as math expression
	{
		if (g_calc.Evaluate(&editrow[0], editlen, &calcvar))
		{
			g_display.printf("= %0.8f", calcvar.floatvalue);
		}
		else
		{
			g_display.printf("ERR: %s", &g_calc.errormsg[0]);
		}
	}
	g_display.printf("\n>");
}

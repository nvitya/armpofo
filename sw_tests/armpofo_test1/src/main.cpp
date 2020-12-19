// ATARILCD Main

#include "platform.h"
#include "hwclkctrl.h"
#include "hwpins.h"
#include "cppinit.h"
#include "clockcnt.h"
#include "hwpwm.h"
#include "keymatrix.h"

#include "systimer.h"
#include "syskeyboard.h"

#include "pofodisplay.h"

TKeyMatrix keys;

TGpioPin  led1pin(PORTNUM_C, 13, true);

#if defined(BOARD_MIBO48_STM32F303)

// special preliminary breadboard test without the custom H750 hardware

void setup_keys()
{
	hwpinctrl.PinSetup(PORTNUM_B, 0, PINCFG_INPUT | PINCFG_PULLUP);
	//hwpinctrl.PinSetup(PORTNUM_B, 0, PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);

	keys.colpin[0].Assign(PORTNUM_B,  0, false);
	keys.colpin[1].Assign(PORTNUM_B,  1, false);
	keys.colpin[2].Assign(PORTNUM_B,  2, false);
	keys.colpin[3].Assign(PORTNUM_B, 10, false);

	keys.rowpin[0].Assign(PORTNUM_B, 11, false);
	keys.rowpin[1].Assign(PORTNUM_B, 12, false);
	keys.rowpin[2].Assign(PORTNUM_B, 13, false);
	keys.rowpin[3].Assign(PORTNUM_B, 14, false);
	keys.rowpin[4].Assign(PORTNUM_B, 15, false);

	keys.Init(5, 4);
}

void setup_board()
{
	led1pin.Setup(PINCFG_OUTPUT | PINCFG_GPIO_INIT_1);

	//setup_keys();
}

#else
  #error "undefined board"
#endif

void idle_task()
{
}

unsigned hbcounter = 0;

void heartbeat_task() // invoked every 0.5 s
{
	++hbcounter;

	led1pin.SetTo(hbcounter >> 0);

	//g_display.printf("%i. systick = %u\n", hbcounter, systick);
}

// the C libraries require "_start" so we keep it as the entry point
extern "C" __attribute__((noreturn)) void _start(void)
{
	// the processor jumps here right after the reset
	// the MCU runs slower, using the internal RC oscillator
	// all variables are unstable, they will be overridden later

	mcu_disable_interrupts();

  mcu_preinit_code(); // inline code for preparing the MCU, RAM regions. Without this even the stack does not work on some MCUs.

	// Set the interrupt vector table offset, so that the interrupts and exceptions work
	mcu_init_vector_table();

  unsigned clockspeed = MCU_CLOCK_SPEED;

#ifdef MCU_INPUT_FREQ
	if (!hwclkctrl.InitCpuClock(MCU_INPUT_FREQ, clockspeed))  // activate the external crystal oscillator with multiplication x2
#else
	if (!hwclkctrl.InitCpuClockIntRC(MCU_INTRC_SPEED, clockspeed))  // activate the external crystal oscillator with multiplication x2
#endif
	{
		while (1)
		{
			// the external oscillator did not start.
		}
	}

	// now the MCU runs faster, start the memory, and C/C++ initialization:
	cppinit();
	// ...from now on all the variables work, static classes are initialized.

	// provide info to the system about the clock speed:
	hwclkctrl.SetClockInfo(clockspeed);

	mcu_enable_fpu();    // enable coprocessor if present
	mcu_enable_icache(); // enable instruction cache if present

	clockcnt_init();

	// go on with the hardware initializations
	setup_board();

	mcu_enable_interrupts();

	systimer_init();

	g_keyboard.Init();

	led1pin.Set1();

	g_display.Init();

	g_display.printf("Hello World !\n");

	g_display.Update();

	//setup_keys();

#if 0
	g_display.LcdFill('0');
	g_display.LcdSetAddress(0);
	g_display.LcdWrite(0x0C, '1');
	g_display.LcdSetAddress(6);
	g_display.LcdWrite(0x0C, '2');
#endif

	led1pin.Set0();

	unsigned hbclocks = SystemCoreClock / 2;  // start blinking fast

	unsigned t0, t1;

	t0 = CLOCKCNT;

	unsigned cyclecnt = 0;

	unsigned prev_scanserial = g_keyscan_events.serial;
	unsigned prev_symserial = g_keysym_events.serial;

	char editrow[128];
	uint8_t editpos = 0;
	editrow[0] = 0;

	// Infinite loop
	while (1)
	{
		t1 = CLOCKCNT;

		idle_task();

		//keys.Run();
		g_keyboard.Run();

#if 0
		g_display.SetPos(0, 2);
		g_display.printf("cyclecnt = %u\n", cyclecnt);
		g_display.printf("systick = %u\n", g_sysms);
		g_display.printf("heartbeat = %u\n", hbcounter);
#endif

		if (prev_scanserial != g_keyscan_events.serial)
		{
			TKeyScanEvent * pevent = &g_keyscan_events.events[g_keyscan_events.serial & 31];

			g_display.SetPos(0, 5);
			g_display.printf("scan %i: ev=%i, key=%2i",
					 g_keyscan_events.serial,
					 pevent->evtype,
					 pevent->scancode
			);

			prev_scanserial = g_keyscan_events.serial;
		}

		if (prev_symserial != g_keysym_events.serial)
		{
			TKeySymbolEvent * psyme = &g_keysym_events.events[g_keysym_events.serial & 15];

			g_display.SetPos(0, 6);
			g_display.printf("sym %i: ss=%02X, sym=%04X",
					 g_keysym_events.serial,
					 psyme->shiftstate,
					 psyme->symbol
			);

			// some processing
			uint16_t keysym = psyme->symbol;
			if (KEYSYM_BACKSPACE == keysym)
			{
				if (editpos > 0)
				{
					--editpos;
					editrow[editpos] = 0;
				}
			}
			else if (KEYSYM_UP == keysym)
			{
				if (g_display.cursor_y > 0)  --g_display.cursor_y;
			}
			else if (KEYSYM_LEFT == keysym)
			{
				if (g_display.cursor_x > 0)  --g_display.cursor_x;
			}
			else if (KEYSYM_DOWN == keysym)
			{
				if (g_display.cursor_y < 7)  ++g_display.cursor_y;
			}
			else if (KEYSYM_RIGHT == keysym)
			{
				if (g_display.cursor_x < 39)  ++g_display.cursor_x;
			}
			else if (KEYSYM_ENTER == keysym)
			{
				// execute

				// reset
				editpos = 0;
				editrow[0] = 0;
			}
			else if ((editpos < 38) && (keysym >= 32) && (keysym <= 127))
			{
				editrow[editpos] = keysym;
				++editpos;
			}

			// print editrow
			g_display.SetPos(0, 7);
			for (unsigned n = 0; n < 40; n++)
			{
				if (n < editpos)
				{
					g_display.WriteChar(editrow[n]);
				}
				else
				{
					g_display.WriteChar(32);
				}
			}

			g_display.cursor_on = true;
			//g_display.cursor_x = editpos;
			//g_display.cursor_y = 7;

			prev_symserial = g_keysym_events.serial;
		}

		g_display.Update(); // must be called regularly

		if (t1-t0 > hbclocks)
		{
			heartbeat_task();
			t0 = t1;

			//if (hbcounter > 20)  hbclocks = SystemCoreClock / 2;  // slow down to 0.5 s
		}

		++cyclecnt;
	}
}

// ----------------------------------------------------------------------------

// ARMPOFO QSPI Test

#include "platform.h"
#include "hwclkctrl.h"
#include "hwpins.h"
#include "cppinit.h"
#include "clockcnt.h"
#include "hwpwm.h"

#include "board_config.h"
#include "traces.h"

#include "hwsdcard.h"

THwSdcard sdcard;

int teststate = 0;

uint8_t testbuf[16384] __attribute__((aligned(4)));
uint32_t testlen = 512 * 2; //2048;
uint32_t testcnt = 0;


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

	cppinit();

	// provide info to the system about the clock speed:
	hwclkctrl.SetClockInfo(clockspeed);

	mcu_enable_fpu();    // enable coprocessor if present
	mcu_enable_icache(); // enable instruction cache if present

	clockcnt_init();

	// go on with the hardware initializations
	board_init();

	traces_init();

	TRACE("\r\n------------------------------------------------------------------\r\n");
	TRACE("ARMPOFO SDCARD TEST\r\n");

	sdcard.Init();

	//---------------------------------------------------------

	mcu_enable_interrupts();

	pin_led1.Set1();

	unsigned hbclocks = SystemCoreClock / 2;

	unsigned t0, t1;
	unsigned rstart, rend;
	int i;

	t0 = CLOCKCNT;

	unsigned cyclecnt = 0;

	// Infinite loop
	while (1)
	{
		t1 = CLOCKCNT;

		sdcard.Run();


		if (0 == teststate)
		{
			if (sdcard.card_initialized)
			{
				TRACE("APP: SD Card initialized.\r\n");
				teststate = 1; // start read
				//teststate = 11; // end test
			}
		}
		else if (1 == teststate)
		{
			// start block read
			//sdcard.StartReadBlocks(2561, &testbuf[0], testlen / 512);
			sdcard.StartReadBlocks(0, &testbuf[0], testlen / 512);
			rstart = CLOCKCNT;
			teststate = 2;
		}
		else if (2 == teststate)
		{
			// wait until read completed
			if (sdcard.completed)
			{
				if (sdcard.errorcode)
				{
					TRACE("Read error!\r\n");
					teststate = 10;
				}
				else
				{
					rend = CLOCKCNT;
					TRACE("Read ok, clocks = %u\r\n", rend - rstart);
#if 1
					for (i = 0; i < testlen; ++i)
					{
						if (i != 0)
						{
							if ((i % 16) == 0)  TRACE("\r\n");
							if ((i % 512) == 0) TRACE("\r\n");
						}

						TRACE(" %02X", testbuf[i]);
					}
					TRACE("\r\n");
#endif

					teststate = 10;
				}
			}
		}
		else if (10 == teststate)
		{
			TRACE("Test %i finished.\r\n", testcnt);
			// the end.
			++testcnt;
			if (testcnt < 2)
			{
				teststate = 1; // repeat
			}
			else
			{
				teststate = 11;
			}
		}
		else if (11 == teststate)
		{
			// stay here.
		}

		if (t1 - t0 > hbclocks)
		{
			pin_led1.Toggle();
			t0 = t1;
		}

		++cyclecnt;
	}
}

// ----------------------------------------------------------------------------

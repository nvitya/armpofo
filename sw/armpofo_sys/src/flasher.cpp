// flasher.cpp

#include "string.h"
#include "traces.h"
#include "flasher.h"
#include "extflash.h"
#include "clockcnt.h"

#define FLASHER_BUFSIZE  4096

bool flasher_copy(void * memaddr, uint32_t flashaddr, uint32_t len, unsigned tempbuf_4k_addr)
{
	unsigned t0, t1;

	len = ((len + 7) & 0xFFFFFFF8); // length must be also dividible with 8 ! (FlexSPI alignment restriction)

	uint8_t * flasherbuf = (uint8_t *)tempbuf_4k_addr;

	TRACE("Fl. m%08X -> f%06X, len = %u\n", memaddr, flashaddr, len);
	TRACE_FLUSH();

	t0 = CLOCKCNT;

	unsigned   remaining = len;
	unsigned   chunksize = len;
	unsigned   faddr = flashaddr;
	uint8_t *  maddr = (uint8_t *)memaddr;
	uint32_t * mdp32;
	uint32_t * fdp32;
	uint32_t * endptr;

	uint32_t progresssize = FLASHER_BUFSIZE;
	uint32_t curprogress = 0;
	char progresschar = '.';

	while (remaining > 0)
	{
		// read the flash first
		chunksize = remaining;
		if (chunksize > FLASHER_BUFSIZE)  chunksize = FLASHER_BUFSIZE;

		g_extflash.StartReadMem(faddr, flasherbuf, chunksize);
		g_extflash.WaitForComplete();

		if (g_extflash.errorcode != 0)
		{
			TRACE(" ERROR = %i\n", g_extflash.errorcode);
			return false;
		}

		// compare memory
		bool   erased = true;
		bool   match = true;
		mdp32  = (uint32_t *)(maddr);
		fdp32  = (uint32_t *)&(flasherbuf[0]);
		endptr = (uint32_t *)&(flasherbuf[chunksize]);

		while (fdp32 < endptr)
		{
			if (*fdp32 != 0xFFFFFFFF)  erased = false;
			if (*fdp32 != *mdp32)
			{
				match = false; // do not break for complete the erased check!
			}

			++fdp32;
			++mdp32;
		}

		if (!match)
		{
			// must be rewritten

			if (!erased)
			{
				g_extflash.StartEraseMem(faddr, chunksize);
				g_extflash.WaitForComplete();
			}

			g_extflash.StartWriteMem(faddr, maddr, chunksize);
			g_extflash.WaitForComplete();
		}

		faddr += chunksize;
		maddr += chunksize;
		remaining -= chunksize;

		// progress info
		if (!match)
		{
			progresschar = 'W';
		}

		curprogress += chunksize;
		if ((curprogress >= progresssize) || (remaining == 0))
		{
			// write progress dot
			TRACE("%c", progresschar);
			TRACE_FLUSH();

			progresschar = '.';
			curprogress = 0;
		}
	}
	// Compare the memory first

	t1 = CLOCKCNT;

	unsigned clocksperus = SystemCoreClock / 1000000;

	TRACE("\n  Finished in %u us\n", (t1 - t0) / clocksperus);
	TRACE_FLUSH();

	return true;
}


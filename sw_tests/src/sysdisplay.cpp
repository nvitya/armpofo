// sysdisplay.cpp

#include <sysdisplay.h>

TSysDisplay  g_display;

bool TSysDisplay::Init()
{
	initialized = false;

	if (!InitHw())
	{
		return false;
	}

	initialized = true;
	return true;
}

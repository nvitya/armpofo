// sysdisplay.h

#ifndef SRC_SYSDISPLAY_H_
#define SRC_SYSDISPLAY_H_

#include "sysdisplayhw.h"

class TSysDisplay : public TSysDisplayHw
{
public:
	bool        initialized = false;

	virtual     ~TSysDisplay() { }

	bool        Init();
};

extern TSysDisplay g_display;

#endif /* SRC_SYSDISPLAY_H_ */

// sysdisplayhw.h

// using a big 5" monochrome LCD with the resolution of 256x160

#ifndef SRC_SYSDISPLAYHW_H_
#define SRC_SYSDISPLAYHW_H_

#include "hwpins.h"
#include "hwpwm.h"
#include "gfxtextscreen.h"


class TSysDisplayHw : public TGfxTextScreen
{
private:
	typedef TGfxTextScreen super;

public:
	bool            InitHw();

	virtual void    Run();

};

#endif /* SRC_SYSDISPLAYHW_H_ */

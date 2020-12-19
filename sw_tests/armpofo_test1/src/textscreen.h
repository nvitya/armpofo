// textscreen.h

#ifndef TEXTSCREEN_H_
#define TEXTSCREEN_H_

#include "stdint.h"

class TTextScreen
{
protected:
	// row[0] is always at the bottom
	uint8_t *       screenbuf = nullptr;
	uint32_t *      changemap = nullptr;
	bool            screenchanged;

	void            SetScreenBufChar(unsigned aaddr, char ach);

public:
	virtual ~TTextScreen() { }

	unsigned 				cols = 0;
	unsigned 				rows = 0;

	unsigned        cposx;
	unsigned        cposy;

	bool            cursor_on = true;
	unsigned        cursor_x = 0;
	unsigned        cursor_y = 0;

	void            Init(unsigned cols, unsigned arows, uint8_t * ascreenbuf, uint8_t * achangemap);

	void            WriteChar(char ach);

	void            SetPos(unsigned acol, unsigned arow);

	void            printf(const char * fmt, ...);

	void            Refresh();

	void            Update();
	void            Clear();

	virtual void    DrawChar(unsigned aaddr, char ach);
	virtual void    SetCursor();
};

#endif /* TEXTSCREEN_H_ */

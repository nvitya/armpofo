// sysdisplayhw_armpofo.h

#ifndef SYSDISPLAYHW_ARMPOFO_H_
#define SYSDISPLAYHW_ARMPOFO_H_

#include "hwpins.h"
#include "hwpwm.h"
#include "textscreen.h"

class TSysDisplayHw : public TTextScreen
{
private:
	typedef TTextScreen   super;

protected:
	uint8_t   mode_base = 0x31; // character mode with external generator
	uint8_t   prev_cursor_on = false;

	uint8_t   prevcmd = 0xFF;
	unsigned  prevaddr = 0x7FFF;

	TGpioPin  pin_inst;
	TGpioPin  pin_en;
	TGpioPin  pin_cs;
	TGpioPin  pin_wr;
	TGpioPin  pin_audio;

	TGpioPin  pin_onoff;

	volatile uint32_t *  data_set_clr_reg = nullptr;

public:
	THwPwmChannel  pwm_contrast;

	virtual ~TSysDisplayHw() { }

	bool         Init();

	bool         InitHw();

	void         LcdInit();
	void         AudioInit();

	void         LcdWrite(uint8_t acmd, uint8_t adata);
	void         LcdWriteData(uint8_t adata);
	inline void  LcdSetData(uint8_t adata);

	void         LcdFill(uint8_t adata);

	void         LcdSetAddress(unsigned x, unsigned y);
	void         LcdSetAddress(unsigned addr);

public: // textscreen


	virtual void    DrawChar(unsigned acol, unsigned arow, char ach);
	virtual void    SetCursor();

};

#endif /* SYSDISPLAYHW_ARMPOFO_H_ */

/*
 * pofodisplay.h
 *
 *  Created on: 13 Nov 2020
 *      Author: vitya
 */

#ifndef SRC_POFODISPLAY_H_
#define SRC_POFODISPLAY_H_

#include "hwpins.h"
#include "hwpwm.h"
#include "textscreen.h"

class TPofoDisplay : public TTextScreen
{
private:
	typedef TTextScreen   parent;

protected:
	uint8_t   prevcmd = 0xFF;
	unsigned  prevaddr = 0x7FFF;

	TGpioPin  pin_inst;
	TGpioPin  pin_en;
	TGpioPin  pin_cs;
	TGpioPin  pin_wr;
	TGpioPin  pin_audio;

	volatile uint32_t *  data_set_clr_reg = nullptr;

public:
	THwPwmChannel  pwm_contrast;

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
	virtual void    DrawChar(unsigned aaddr, char ach);  // textscreen mandatory
};

extern TPofoDisplay   g_display;

#endif /* SRC_POFODISPLAY_H_ */

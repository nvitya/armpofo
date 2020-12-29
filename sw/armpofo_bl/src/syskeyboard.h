// pofokeyboard.h

#ifndef SRC_SYSKEYBOARD_H_
#define SRC_SYSKEYBOARD_H_

#include "stdint.h"
#include "sysif.h"  // includes sysif_keyboard.h
#include "keymatrix.h"

class TSysKeyboard : public TKeyMatrix
{
private:
	typedef TKeyMatrix     super;

	TKeyScanEventBuf *     scan_events;
	TKeySymbolEventBuf *   sym_events;

protected:

	uint32_t  prev_pushmap[KEY_PUSHMAP_DWORDS];

public:
	virtual ~TSysKeyboard() { }

	uint8_t  shiftstate;

	bool Init();


	uint32_t * GetPushMap() { return (uint32_t *)&keys32[0]; }

	void Run();

	void AddScanEvent(uint8_t aevtype, uint8_t ascancode);
	void AddSymEvent(uint16_t asymbol);

public: // board dependent implementations

	bool InitHw();
	void ProcessScanEvent(uint8_t aevtype, uint8_t ascancode);

};

extern TSysKeyboard       g_keyboard;
extern TKeyScanEventBuf   g_keyscan_events;
extern TKeySymbolEventBuf g_keysym_events;

#endif /* SRC_SYSKEYBOARD_H_ */

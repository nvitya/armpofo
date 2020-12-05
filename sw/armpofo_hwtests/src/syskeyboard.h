// pofokeyboard.h

#ifndef SRC_SYSKEYBOARD_H_
#define SRC_SYSKEYBOARD_H_

#include "stdint.h"
#include "keymatrix.h"

#define MAX_KEYSCANEVENTS     32 // must be power of 2 !
#define MAX_KEYSYMEVENTS      16 // must be power of 2 !

#define KEYSYM_BACKSPACE       8
#define KEYSYM_TAB             9
#define KEYSYM_ENTER          13
#define KEYSYM_ESC            27

#define KEYSYM_END        0x8001  // last number match to the PC numeric keyboard
#define KEYSYM_DOWN       0x8002
#define KEYSYM_PGDN       0x8003
#define KEYSYM_LEFT       0x8004
#define KEYSYM_RIGHT      0x8006
#define KEYSYM_HOME       0x8007
#define KEYSYM_UP         0x8008
#define KEYSYM_PGUP       0x8009
#define KEYSYM_INSERT     0x800A
#define KEYSYM_DELETE     0x800B

#define KEYSYM_MENU       0x800C
#define KEYSYM_PAUSE      0x800E
#define KEYSYM_BREAK      0x800F

#define KEYSYM_F1         0x8011
#define KEYSYM_F2         0x8012
#define KEYSYM_F3         0x8013
#define KEYSYM_F4         0x8014
#define KEYSYM_F5         0x8015
#define KEYSYM_F6         0x8016
#define KEYSYM_F7         0x8017
#define KEYSYM_F8         0x8018
#define KEYSYM_F9         0x8019
#define KEYSYM_F10        0x801A
#define KEYSYM_F11        0x801B
#define KEYSYM_F12        0x801C

#define KEYSHIFT_SHIFT        0x01
#define KEYSHIFT_CTRL         0x02
#define KEYSHIFT_ALT          0x04
#define KEYSHIFT_SYS          0x08
#define KEYSHIFT_FN           0x10

#define KEYSHIFT_LOCK_CAPS    0x20
#define KEYSHIFT_LOCK_NUM     0x40
#define KEYSHIFT_LOCK_SCROLL  0x80

#define KEY_PUSHMAP_DWORDS     2

#define KEYSCAN_EVENT_PUSH     1
#define KEYSCAN_EVENT_RELEASE  2

struct TKeyScanEvent // 8 bytes
{
	uint32_t   evtime;     // milliseconds
	uint8_t    evtype;     // 1 = push, 2 = release
	uint8_t    scancode;

	uint8_t    _reserved[2];
};

struct TKeyScanEventBuf
{
	uint8_t        headsize;
	uint8_t        max_events;
  uint16_t       serial;      // when the serial is 0, then the buffer is empty
                              // otherwise the last 5 bits show the index of the last event

	TKeyScanEvent  events[MAX_KEYSCANEVENTS];
};

struct TKeySymbolEvent // 8 bytes
{
	uint32_t   evtime;     // milliseconds
	uint16_t   symbol;     // 0 - 255: ascii code, over 0x8000: special keyboard codes
	uint8_t    _reserved;
	uint8_t    shiftstate;
};

struct TKeySymbolEventBuf
{
	uint8_t          headsize;
	uint8_t          max_events;
  uint16_t         serial;      // when the serial is 0, then the buffer is empty
                              // otherwise the last 5 bits show the index of the last event

  TKeySymbolEvent  events[MAX_KEYSYMEVENTS];
};


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

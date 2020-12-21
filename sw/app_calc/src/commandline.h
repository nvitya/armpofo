// commandline.h

#ifndef SRC_COMMANDLINE_H_
#define SRC_COMMANDLINE_H_

#include "sysif.h"
#include "strparse.h"

#define CMDLINE_MAX_LENGTH  252

#define CMDLINE_HIST_LEN     16

struct TCmdHist
{
	int       len;
	char      row[CMDLINE_MAX_LENGTH];
};

class TCommandLine
{
public:

	TKeySymbolEventBuf * psymevents = nullptr;

	uint16_t  prev_symserial = 0xFFFF;

	int       editpos = 0;
	int       startpos = 0;
	int       editlen = 0;
	char      editrow[CMDLINE_MAX_LENGTH];

	int       hist_pos = 0;
	int       hist_load_pos = 0;

	bool          exitcommand = false;  // signalize to the caller that exit required

	bool          initialized = false;

	TStrParseObj  sp;
	TDisplayInfo  dispinfo;

	TCmdHist      hist[CMDLINE_HIST_LEN];

	virtual   ~TCommandLine() { }

	void      Init();

	void      Run();

	void      Clear();
	void      Draw();
	void      Execute();
	bool      ExecInternalCommand();
	bool      ExecApplication();
	void      DeletePos(unsigned apos);
	void      CorrectStartPos();

	void      HistSave(unsigned apos);
	void      HistLoad(unsigned apos);
};

extern TCommandLine g_commandline;

#endif /* SRC_COMMANDLINE_H_ */

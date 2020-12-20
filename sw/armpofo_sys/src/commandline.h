// lineedit.h

#ifndef SRC_COMMANDLINE_H_
#define SRC_COMMANDLINE_H_

#include "syskeyboard.h"
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

	uint16_t  prev_symserial = 0xFFFF;

	int       editpos = 0;
	int       startpos = 0;
	int       editlen = 0;
	char      editrow[CMDLINE_MAX_LENGTH];

	int       hist_pos = 0;
	int       hist_load_pos = 0;

	TStrParseObj sp;
	TCmdHist  hist[CMDLINE_HIST_LEN];

	virtual   ~TCommandLine();

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

#endif /* SRC_COMMANDLINE_H_ */

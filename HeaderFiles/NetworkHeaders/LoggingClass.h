#pragma once

// Global log sink: anything the server writes through CLoggingClass or
// CServerEventList::AddEvent is also handed to this callback (if set) so a
// host window can show it live.  Called from ANY thread - the sink must
// marshal to its own thread (e.g. PostMessage).
typedef void (*AXLogSinkFn)(const char * line, void * context);
void AXSetLogSink(AXLogSinkFn fn, void * context);
void AXLogSinkWrite(const char * line);

class CLoggingClass
{
public:
	CLoggingClass(void);
	~CLoggingClass(void);

	//VARS
	int         m_loggingType;//0=none 1=text
	CString     m_logFileName;

	void SetDetails(int type, CString filename)
	{
        m_loggingType = type;
        m_logFileName = filename;
	}

	//FUNCTIONS
	void ConvertToString(long stringTemp, char * buff);
    void WriteDebugFile(long debugString,BOOL clearIt,BOOL returnCase);
    void WriteDebugFile(LPSTR debugString,BOOL clearIt,BOOL returnCase);
};

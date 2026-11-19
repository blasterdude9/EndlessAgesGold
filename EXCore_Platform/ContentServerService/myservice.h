// myservice.h
#include "axcontentserver.h"
#include "ntservice.h"

//WARNING: C4996 --> This function or variable may be unsafe
//--> disable deprecated function warning
#define _CRT_SECURE_NO_DEPRECATE

class CMyService : public CNTService
{
public:
	CMyService();
	virtual BOOL OnInit();
    virtual void Run();
	virtual void OnStop();
    virtual BOOL OnUserControl(DWORD dwOpcode);
	virtual void OnShutdown();
	void MakeStatusEntry(CString name,DWORD val);
	void MakeStatusEntryString(CString name,CString val);
	LPSTR GetSoftwareRegkeyValue(LPSTR headerX,LPSTR valueRequest);
	void SetSoftwareRegistry(LPSTR valueName,LPSTR valueDataX);

	
	CString                           m_workingPath;
	CAXContentServer				* m_contentServer;

    void SaveStatus();

	// Control parameters
	int m_iStartParam;
	int m_iIncParam;

	// Current state
	int m_iState;
};

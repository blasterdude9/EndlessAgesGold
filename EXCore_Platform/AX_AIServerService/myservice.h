// myservice.h

#include "ntservice.h"
#include "centralprocessor.h"


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

	CCentralProcessor				* m_aiServer;
	CString                           m_workingPath;

    void SaveStatus();

	// Control parameters
	int m_iStartParam;
	int m_iIncParam;

	// Current state
	int m_iState;
};

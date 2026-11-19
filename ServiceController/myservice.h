// myservice.h

#include "ntservice.h"
#include "arbservicecontroller.h"
#include "dbconlimitedSystem.h"

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
	CString GetSoftwareRegkeyValue(LPSTR headerX,LPSTR valueRequest);
	void SetSoftwareRegistry(LPSTR valueName,LPSTR valueDataX);


	CString                           m_workingPath;

	CARBServiceController            *m_serviceController;

	CString m_sqlServerIP,m_sqlServerUsername,m_sqlServerPassword,m_sqlServerCatalog,m_sqlServerTable,m_serverIdName;

    void SaveStatus();

	// Control parameters
	int m_iStartParam;
	int m_iIncParam;

	// Current state
	int m_iState;
};

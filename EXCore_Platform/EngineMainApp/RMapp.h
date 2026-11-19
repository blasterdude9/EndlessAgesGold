#ifndef RMAPP_H
#define RMAPP_H


class RMApp : public CWinApp
{
public:
	RMApp();
	BOOL enabled;
	HANDLE  m_randerCallHandle;
	DWORD m_workerThreadID;
	virtual BOOL InitInstance();
	BOOL OnIdle(LONG lCount);
	// Log + show any MFC exception that escapes a message handler (otherwise a
	// CException thrown during WM_CREATE silently kills the window and the app).
	virtual LRESULT ProcessWndProcException(CException* e, const MSG* pMsg);
protected:
	//{{AFX_MSG(RMApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	friend class RMWin;
	RMWin* rmwin;

};

#endif

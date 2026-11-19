// ClientSimulatorDlg.h : header file
//
#include "clientmodule.h"
#include "loggingclass.h"
#pragma once
#include "afxwin.h"

#define MAXRANDOMCHATCOUNT 9
#define SPEAKPERCENTAGE 5
#define REQ_INVENTORY_PERCENTAGE 5
#define REQ_STORE_INVENTORY_PERCENTAGE 5

struct acStruct
{
   char username[25];
   char password[25];
   DWORD packetsSent;
   DWORD packetsReceived;
   DWORD curSendBytes;
   DWORD curRecieveBytes;
   D3DVECTOR position;
};


// CClientSimulatorDlg dialog
class CClientSimulatorDlg : public CDialog
{
// Construction
public:
	CClientSimulatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLIENTSIMULATOR_DIALOG };

	
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    CRITICAL_SECTION m_mSect;
	CRITICAL_SECTION m_logSect;
	BOOL m_closeDown;

	MatrixARB GL_matrixFunctions;
	acStruct  m_accounts[5000];
	int       m_accountAddTracker;

	int       m_threadRunningCount;
	int       m_currentThreadCreate;

	HANDLE  * m_threadHandles;
	DWORD   * m_threadIDs; 
	int       m_threadHandleCount;
	BOOL      m_logBothTextFileAndList;

	//
	//randoms
	CString		m_randomChat[MAXRANDOMCHATCOUNT+1];






public:
	void Log(CString msg);
	void ClearLog();
	void InListAccounts(int sel);
	D3DVECTOR GetRandomPositionInRadius(float radius,D3DVECTOR origin,float minimum);
	void WriteConfigFile(CString filename);
	void ReadConfigFile(CString filename);

// Implementation
public:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonLogonstresstest();
	afx_msg void OnBnClickedButtonGameplaystress();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonAddmanualaccount();
	

public:

	CLoggingClass       m_realtimeLogger;
	CEdit m_serverIP;
	CEdit m_username;
	CEdit m_password;
	CButton m_singleLogonModeChk;
public:
	CListBox m_clientConnectionsList;
public:
	CListBox m_logOutput;
public:
	afx_msg void OnBnClickedButtonRefresh();
public:
	CEdit m_sequenceStart;
public:
	CEdit m_sequenceEnd;
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CEdit m_radius;
};

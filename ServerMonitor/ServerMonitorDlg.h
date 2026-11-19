// ServerMonitorDlg.h : header file
//

#if !defined(AFX_SERVERMONITORDLG_H__D6D7FE03_67E5_4FA2_A4BA_6DB60E2CB530__INCLUDED_)
#define AFX_SERVERMONITORDLG_H__D6D7FE03_67E5_4FA2_A4BA_6DB60E2CB530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CServerMonitorDlg dialog

class CServerMonitorDlg : public CDialog
{
// Construction
public:
	CServerMonitorDlg(CWnd* pParent = NULL);	// standard constructor
    CRITICAL_SECTION       m_criticalSection;
	int m_currentStatusBool;
	long m_lastUpdateStamp;
	CString m_sqlIp;
	CString m_sqlUserid;
	CString m_sqlPass;
	CString m_sqlDB;
// Dialog Data
	//{{AFX_DATA(CServerMonitorDlg)
	enum { IDD = IDD_SERVERMONITOR_DIALOG };
	CString	m_gameServerIP;
	CString	m_gameServerName;
	CString	m_processName;
	CString	m_currentStatus;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerMonitorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
    CString EnumProcessName( DWORD processID );
	BOOL FindProcess(CString processname);
	void UpdateSQLTable(CString serverName, CString ipAdress, CString status);


	// Generated message map functions
	//{{AFX_MSG(CServerMonitorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERMONITORDLG_H__D6D7FE03_67E5_4FA2_A4BA_6DB60E2CB530__INCLUDED_)

// AIServerApplicationDlg.h : header file
//
// Endless Ages AI server console window.
//
// Shows a live log (everything the AI server writes through CServerEventList /
// CLoggingClass, plus host messages) and a status line (worlds, bots,
// traffic, uptime).  The log is also written with timestamps to AIServerLog.txt
// beside the exe.

#pragma once
#include "centralprocessor.h"

#define WM_AISERVER_LOGLINE   (WM_APP + 1)   // lParam = CString* (heap) - log line from any thread
#define WM_AISERVER_STARTED   (WM_APP + 2)   // wParam = 1 ok / 0 failed

class CAIServerApplicationDlg : public CDialog
{
// Construction
public:
	CAIServerApplicationDlg(CWnd* pParent = NULL);	// standard constructor

	CCentralProcessor * m_aiServer;

// Dialog Data
	enum { IDD = IDD_AISERVERAPPLICATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON     m_hIcon;
	CEdit     m_log;
	CStatic   m_status;
	CFont     m_logFont;
	HANDLE    m_startThread;
	DWORD     m_startTick;
	BOOL      m_serverRunning;
	BOOL      m_startFailed;
	CString   m_logFilePath;
	int       m_logLines;
	DWORD     m_lostTick;       // when the game-server link was lost (0 = connected)
	BOOL      m_reconnectMode;  // started with /reconnect: keep retrying on failure

	static void   LogSink(const char * line, void * context);   // called from any thread
	static DWORD WINAPI StartServerThread(LPVOID param);

	void Log(const CString & line);          // UI thread only
	void Log(LPCTSTR line) { Log(CString(line)); }
	void AppendToLogFile(const CString & stampedLine);
	void UpdateStatus();
	void LayoutControls();
	void StopServer();
	void RelaunchForReconnect();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg LRESULT OnLogLine(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnServerStarted(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnClearLog();
	afx_msg void OnBnClickedBtnOpenLog();
};

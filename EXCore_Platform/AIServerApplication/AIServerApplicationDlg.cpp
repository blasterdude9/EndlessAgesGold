// AIServerApplicationDlg.cpp : implementation file
//
// Endless Ages AI server console window - see AIServerApplicationDlg.h.

#include "stdafx.h"
#include "AIServerApplication.h"
#include "AIServerApplicationDlg.h"
#include "LoggingClass.h"
#include <shellapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define AISERVER_TIMER_STATUS   1
#define AISERVER_LOG_MAX_CHARS  (512 * 1024)   // trim the on-screen log past this (file keeps everything)

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAIServerApplicationDlg dialog

CAIServerApplicationDlg::CAIServerApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAIServerApplicationDlg::IDD, pParent)
{
	m_hIcon         = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_aiServer      = NULL;
	m_lostTick      = 0;
	m_reconnectMode = FALSE;
	m_startThread   = NULL;
	m_startTick     = 0;
	m_serverRunning = FALSE;
	m_startFailed   = FALSE;
	m_logLines      = 0;
}

void CAIServerApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_log);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_status);
}

BEGIN_MESSAGE_MAP(CAIServerApplicationDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_AISERVER_LOGLINE, OnLogLine)
	ON_MESSAGE(WM_AISERVER_STARTED, OnServerStarted)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CLEARLOG, OnBnClickedBtnClearLog)
	ON_BN_CLICKED(IDC_BTN_OPENLOG, OnBnClickedBtnOpenLog)
END_MESSAGE_MAP()


// CAIServerApplicationDlg message handlers

BOOL CAIServerApplicationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Log window: fixed-pitch font, unlimited text, newest line at the bottom.
	m_logFont.CreateFont(-13, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
	                     DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
	                     CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, "Consolas");
	m_log.SetFont(&m_logFont);
	m_log.SetLimitText(0);

	// AIServerLog.txt beside the exe (cwd is already the exe folder - see InitInstance).
	char exePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, exePath, MAX_PATH);
	CString dir(exePath);
	int slash = dir.ReverseFind('\\');
	if(slash >= 0) dir = dir.Left(slash + 1);
	m_logFilePath = dir + "AIServerLog.txt";

	CString ver;
	ver.Format("Endless Ages AI server host  (pid %u)", GetCurrentProcessId());
	SetWindowText("Endless Ages AI Server");
	Log("================================================================");
	Log(ver);
	Log("Log file: " + m_logFilePath);
	Log("Working folder: " + dir);
	Log("Starting AI server ...");
	if(strstr(GetCommandLine(), "/reconnect"))
	{
		m_reconnectMode = TRUE;
		Log("(restarted automatically to reconnect to the game server)");
	}

	LayoutControls();
	UpdateStatus();

	// Everything the AI server logs from any thread now lands in this window.
	AXSetLogSink(LogSink, this);

	CoInitialize(NULL);
	m_startTick = GetTickCount();
	SetTimer(AISERVER_TIMER_STATUS, 1000, NULL);

	// Server start-up (SQL, data files, connection to main server) can take a while -
	// do it on a worker thread so the window stays alive and shows progress.
	DWORD tid = 0;
	m_startThread = CreateThread(NULL, 0, StartServerThread, this, 0, &tid);
	if(!m_startThread)
	{
		Log("ERROR: could not create the AI server start-up thread");
		m_startFailed = TRUE;
		UpdateStatus();
	}
	return TRUE;
}

DWORD WINAPI CAIServerApplicationDlg::StartServerThread(LPVOID param)
{
	CAIServerApplicationDlg * dlg = (CAIServerApplicationDlg *)param;
	CoInitialize(NULL);

	CCentralProcessor * server = new CCentralProcessor(5);
	dlg->m_aiServer = server;

	BOOL ok = FALSE;
	if(server && server->m_connected)
		ok = TRUE;

	if(::IsWindow(dlg->GetSafeHwnd()))
		dlg->PostMessage(WM_AISERVER_STARTED, ok ? 1 : 0, 0);
	return 0;
}

// Called from ANY server thread - hand the line to the UI thread.
void CAIServerApplicationDlg::LogSink(const char * line, void * context)
{
	CAIServerApplicationDlg * dlg = (CAIServerApplicationDlg *)context;
	if(!dlg || !line) return;
	HWND hwnd = dlg->GetSafeHwnd();
	if(!hwnd || !::IsWindow(hwnd)) return;

	CString * copy = new CString(line);
	if(!::PostMessage(hwnd, WM_AISERVER_LOGLINE, 0, (LPARAM)copy))
		delete copy;
}

LRESULT CAIServerApplicationDlg::OnLogLine(WPARAM /*wParam*/, LPARAM lParam)
{
	CString * line = (CString *)lParam;
	if(line)
	{
		Log(*line);
		delete line;
	}
	return 0;
}

LRESULT CAIServerApplicationDlg::OnServerStarted(WPARAM wParam, LPARAM /*lParam*/)
{
	if(m_startThread)
	{
		CloseHandle(m_startThread);
		m_startThread = NULL;
	}

	if(wParam)
	{
		m_serverRunning = TRUE;
		int wCount = (m_aiServer && m_aiServer->m_aiWorldsList) ? m_aiServer->m_aiWorldsList->GetCount_safe() : 0;
		CString msg;
		msg.Format("AI server started - connected to main server. Worlds running: %d.", wCount);
		Log(msg);
	}
	else
	{
		m_startFailed = TRUE;
		Log("ERROR: AI server failed to start (see lines above, AIServerLog.txt and CrashLog.txt).");
		Log("Check that SQL Server is running and the main game server (AxserverApp) is online.");
		if(m_reconnectMode)
		{
			Log("Will try to reconnect again in 30 seconds ...");
			m_lostTick = GetTickCount() | 1;   // timer relaunches 30 s from now
		}
	}
	UpdateStatus();
	return 0;
}

void CAIServerApplicationDlg::Log(const CString & lineIn)
{
	CString line = lineIn;
	line.TrimRight("\r\n");
	line.Replace("\r", "");
	line.Replace("\n", " | ");
	if(line.IsEmpty()) return;

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString stamped;
	stamped.Format("[%02d:%02d:%02d] %s", st.wHour, st.wMinute, st.wSecond, (LPCSTR)line);

	if(m_log.GetSafeHwnd())
	{
		int len = m_log.GetWindowTextLength();
		if(len > AISERVER_LOG_MAX_CHARS)
		{
			// Drop the oldest half so the window never bloats; the file keeps everything.
			int cut = m_log.LineIndex(m_log.LineFromChar(len / 2) + 1);
			if(cut > 0)
			{
				m_log.SetSel(0, cut);
				m_log.ReplaceSel("");
				len = m_log.GetWindowTextLength();
			}
		}
		m_log.SetSel(len, len);
		m_log.ReplaceSel(stamped + "\r\n");
		m_logLines++;
	}

	CString fileLine;
	fileLine.Format("%04d-%02d-%02d %s", st.wYear, st.wMonth, st.wDay, (LPCSTR)stamped);
	AppendToLogFile(fileLine);
}

void CAIServerApplicationDlg::AppendToLogFile(const CString & stampedLine)
{
	if(m_logFilePath.IsEmpty()) return;
	HANDLE h = CreateFile(m_logFilePath, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE,
	                      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(h == INVALID_HANDLE_VALUE) return;
	DWORD written = 0;
	WriteFile(h, (LPCSTR)stampedLine, stampedLine.GetLength(), &written, NULL);
	WriteFile(h, "\r\n", 2, &written, NULL);
	CloseHandle(h);
}

void CAIServerApplicationDlg::UpdateStatus()
{
	CString s;
	DWORD up = (GetTickCount() - m_startTick) / 1000;

	if(m_startFailed)
		s = "AI SERVER FAILED TO START - see log.";
	else if(!m_serverRunning)
		s.Format("Starting AI server ... (%us)", up);
	else
	{
		int wCount = (m_aiServer && m_aiServer->m_aiWorldsList) ? m_aiServer->m_aiWorldsList->GetCount_safe() : 0;
		if(m_aiServer && m_aiServer->m_aiServer)
		{
			AB_aiServerStatusLive & st = m_aiServer->m_aiServer->m_aiServerStatus;
			s.Format("Running  |  AI Worlds: %d  |  Bots active: %ld (peak %ld)  |  Traffic in %lu / out %lu bytes/s  |  Uptime %02u:%02u:%02u",
			         wCount, st.m_botsOnline, st.m_botsSpikeOnline,
			         st.m_bandwidthIncommingPerSecond, st.m_bandwidthOutgoingPerSecond,
			         up / 3600, (up / 60) % 60, up % 60);
			if(m_aiServer->m_aiServer->m_systemShutDown)
				s += "  |  SHUTDOWN IN PROGRESS";
		}
		else
		{
			s.Format("Running  |  AI Worlds: %d  |  Uptime %02u:%02u:%02u",
			         wCount, up / 3600, (up / 60) % 60, up % 60);
		}
	}

	if(m_status.GetSafeHwnd())
	{
		CString cur;
		m_status.GetWindowText(cur);
		if(cur != s) m_status.SetWindowText(s);
	}
}

void CAIServerApplicationDlg::LayoutControls()
{
	if(!m_log.GetSafeHwnd()) return;
	CRect rc;
	GetClientRect(&rc);
	const int margin = 8, btnW = 110, btnH = 24, statusH = 20;

	m_status.MoveWindow(margin, margin, rc.Width() - 2 * margin, statusH);
	m_log.MoveWindow(margin, margin + statusH + 4,
	                 rc.Width() - 2 * margin, rc.Height() - (margin * 3 + statusH + 4 + btnH));

	int y = rc.Height() - margin - btnH;
	CWnd * b;
	if((b = GetDlgItem(IDOK)) != NULL)            b->MoveWindow(rc.Width() - margin - btnW, y, btnW, btnH);
	if((b = GetDlgItem(IDC_BTN_OPENLOG)) != NULL) b->MoveWindow(rc.Width() - margin - 2 * btnW - 6, y, btnW, btnH);
	if((b = GetDlgItem(IDC_BTN_CLEARLOG)) != NULL) b->MoveWindow(margin, y, btnW, btnH);
}

void CAIServerApplicationDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	LayoutControls();
}

void CAIServerApplicationDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 520;
	lpMMI->ptMinTrackSize.y = 320;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CAIServerApplicationDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == AISERVER_TIMER_STATUS)
	{
		UpdateStatus();

		// Game server went away (shut down / crashed / timed out)?  Restart this
		// AI server so it reconnects cleanly once the game server is back.
		if(!m_lostTick && m_serverRunning && !m_startThread && m_aiServer && m_aiServer->IsConnectionLost())
		{
			m_lostTick = GetTickCount() | 1;
			Log("Lost connection to the main game server. Restarting the AI server in 10 seconds to reconnect ...");
		}
		if(m_lostTick && !m_startThread && (GetTickCount() - m_lostTick) > (m_startFailed ? 30000u : 10000u))
		{
			m_lostTick = 0;
			RelaunchForReconnect();
			return;
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CAIServerApplicationDlg::PreTranslateMessage(MSG* pMsg)
{
	// Enter/Esc must never shut the server down by accident - only the button or close box.
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CAIServerApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CAIServerApplicationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CAIServerApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAIServerApplicationDlg::OnDestroy()
{
	AXSetLogSink(NULL, NULL);
	CDialog::OnDestroy();
}

void CAIServerApplicationDlg::StopServer()
{
	KillTimer(AISERVER_TIMER_STATUS);
	if(m_aiServer)
	{
		Log("Shutting down AI server ...");
		m_status.SetWindowText("Shutting down ...");
		UpdateWindow();
		delete m_aiServer;
		m_aiServer = NULL;
		Log("AI server stopped.");
	}
	m_serverRunning = FALSE;
	AXSetLogSink(NULL, NULL);
	CoUninitialize();
}

// Start a fresh AIServerApp (it reconnects from scratch) and close this one.
// A new process is used rather than rebuilding the server in-process, so no
// state from the dropped session can leak into the new one.
void CAIServerApplicationDlg::RelaunchForReconnect()
{
	char exePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, exePath, MAX_PATH);
	CString cmd;
	cmd.Format("\"%s\" /reconnect", exePath);

	STARTUPINFO si; PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si)); si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	if(!CreateProcess(NULL, cmd.GetBuffer(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		cmd.ReleaseBuffer();
		Log("ERROR: could not restart the AI server - please start it again by hand.");
		return;
	}
	cmd.ReleaseBuffer();
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	Log("Started a new AI server to reconnect; closing this one.");
	StopServer();
	OnOK();
}

void CAIServerApplicationDlg::OnBnClickedOk()
{
	if(m_startThread)
	{
		// Constructor still running on the worker thread - cannot tear down yet.
		Log("AI server is still starting - wait for 'AI server started' (or the failure message) before closing.");
		return;
	}
	StopServer();
	OnOK();
}

void CAIServerApplicationDlg::OnBnClickedCancel()
{
	// Close box / Esc behaves like the Shutdown button.
	OnBnClickedOk();
}

void CAIServerApplicationDlg::OnBnClickedBtnClearLog()
{
	m_log.SetWindowText("");
	m_logLines = 0;
}

void CAIServerApplicationDlg::OnBnClickedBtnOpenLog()
{
	ShellExecute(GetSafeHwnd(), "open", m_logFilePath, NULL, NULL, SW_SHOWNORMAL);
}

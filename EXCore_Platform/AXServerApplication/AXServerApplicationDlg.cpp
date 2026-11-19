// AXServerApplicationDlg.cpp : implementation file
//
// Endless Ages game server console window - see AXServerApplicationDlg.h.

#include "stdafx.h"
#include "AXServerApplication.h"
#include "AXServerApplicationDlg.h"
#include "LoggingClass.h"
#include <shellapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define AXSERVER_TIMER_STATUS   1
#define AXSERVER_LOG_MAX_CHARS  (512 * 1024)   // trim the on-screen log past this (file keeps everything)

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


// CAXServerApplicationDlg dialog

CAXServerApplicationDlg::CAXServerApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAXServerApplicationDlg::IDD, pParent)
{
	m_hIcon         = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_axserver      = NULL;
	m_startThread   = NULL;
	m_startTick     = 0;
	m_serverRunning = FALSE;
	m_startFailed   = FALSE;
	m_logLines      = 0;
}

void CAXServerApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LOG, m_log);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_status);
}

BEGIN_MESSAGE_MAP(CAXServerApplicationDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_AXSERVER_LOGLINE, OnLogLine)
	ON_MESSAGE(WM_AXSERVER_STARTED, OnServerStarted)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CLEARLOG, OnBnClickedBtnClearLog)
	ON_BN_CLICKED(IDC_BTN_OPENLOG, OnBnClickedBtnOpenLog)
END_MESSAGE_MAP()


// CAXServerApplicationDlg message handlers

BOOL CAXServerApplicationDlg::OnInitDialog()
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

	// ServerLog.txt beside the exe (cwd is already the exe folder - see InitInstance).
	char exePath[MAX_PATH] = {0};
	GetModuleFileName(NULL, exePath, MAX_PATH);
	CString dir(exePath);
	int slash = dir.ReverseFind('\\');
	if(slash >= 0) dir = dir.Left(slash + 1);
	m_logFilePath = dir + "ServerLog.txt";

	CString ver;
	ver.Format("Endless Ages game server host  (pid %u)", GetCurrentProcessId());
	SetWindowText("Endless Ages Game Server");
	Log("================================================================");
	Log(ver);
	Log("Log file: " + m_logFilePath);
	Log("Working folder: " + dir);
	Log("Starting server ...");

	LayoutControls();
	UpdateStatus();

	// Everything the server logs from any thread now lands in this window.
	AXSetLogSink(LogSink, this);

	CoInitialize(NULL);
	m_startTick = GetTickCount();
	SetTimer(AXSERVER_TIMER_STATUS, 1000, NULL);

	// Server start-up (SQL, data files, ENet host) can take a while - do it on a
	// worker thread so the window stays alive and shows progress.
	DWORD tid = 0;
	m_startThread = CreateThread(NULL, 0, StartServerThread, this, 0, &tid);
	if(!m_startThread)
	{
		Log("ERROR: could not create the server start-up thread");
		m_startFailed = TRUE;
		UpdateStatus();
	}
	return TRUE;
}

DWORD WINAPI CAXServerApplicationDlg::StartServerThread(LPVOID param)
{
	CAXServerApplicationDlg * dlg = (CAXServerApplicationDlg *)param;
	CoInitialize(NULL);

	CServerProc * server = new CServerProc();
	dlg->m_axserver = server;

	// 1 = running, 0 = failed, 2 = the UDP port could not be opened (normally
	// because another game server is already running on it).
	WPARAM result = 0;
	if(server && server->m_serverSystem && server->m_serverSystem->m_sharedInterface &&
	   server->m_serverSystem->m_sharedInterface->m_netComObject &&
	   server->m_serverSystem->m_sharedInterface->m_netComObject->m_netHost)
		result = server->m_serverSystem->m_sharedInterface->m_netComObject->m_netHost->IsActive() ? 1 : 2;

	if(::IsWindow(dlg->GetSafeHwnd()))
		dlg->PostMessage(WM_AXSERVER_STARTED, result, 0);
	return 0;
}

// Called from ANY server thread - hand the line to the UI thread.
void CAXServerApplicationDlg::LogSink(const char * line, void * context)
{
	CAXServerApplicationDlg * dlg = (CAXServerApplicationDlg *)context;
	if(!dlg || !line) return;
	HWND hwnd = dlg->GetSafeHwnd();
	if(!hwnd || !::IsWindow(hwnd)) return;

	CString * copy = new CString(line);
	if(!::PostMessage(hwnd, WM_AXSERVER_LOGLINE, 0, (LPARAM)copy))
		delete copy;
}

LRESULT CAXServerApplicationDlg::OnLogLine(WPARAM /*wParam*/, LPARAM lParam)
{
	CString * line = (CString *)lParam;
	if(line)
	{
		Log(*line);
		delete line;
	}
	return 0;
}

LRESULT CAXServerApplicationDlg::OnServerStarted(WPARAM wParam, LPARAM /*lParam*/)
{
	if(m_startThread)
	{
		CloseHandle(m_startThread);
		m_startThread = NULL;
	}

	if(wParam == 2)
	{
		m_startFailed = TRUE;
		DWORD port = m_axserver->m_serverSystem->m_sharedInterface->m_netComObject->m_port;
		if(!port) port = 25857;
		CString msg;
		msg.Format("ERROR: UDP port %u is already in use - another game server is probably already running.", port);
		Log(msg);
		Log("Close the other AxserverApp window (or end it in Task Manager), then start this one again.");
		AfxMessageBox(msg + "\n\nClose the other game server first. This one will not accept players.", MB_ICONERROR);
	}
	else if(wParam)
	{
		m_serverRunning = TRUE;
		CString msg;
		DWORD port = m_axserver->m_serverSystem->m_sharedInterface->m_netComObject->m_port;
		msg.Format("Server started - listening on UDP port %u. Waiting for players.", port);
		Log(msg);
	}
	else
	{
		m_startFailed = TRUE;
		Log("ERROR: server failed to start (see lines above, realtimeWriteLog.txt and CrashLog.txt).");
		Log("Check serverstartup.cfg / authAccount.cfg / baseAccount.cfg and that SQL Server is running.");
	}
	UpdateStatus();
	return 0;
}

void CAXServerApplicationDlg::Log(const CString & lineIn)
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
		if(len > AXSERVER_LOG_MAX_CHARS)
		{
			// Drop the oldest half so the window never bloats; the file has it all.
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

void CAXServerApplicationDlg::AppendToLogFile(const CString & stampedLine)
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

void CAXServerApplicationDlg::UpdateStatus()
{
	CString s;
	DWORD up = (GetTickCount() - m_startTick) / 1000;

	if(m_startFailed)
		s = "SERVER FAILED TO START - see log.";
	else if(!m_serverRunning)
		s.Format("Starting server ... (%us)", up);
	else
	{
		CMultiplayerObj * net = m_axserver->m_serverSystem->m_sharedInterface->m_netComObject;
		AB_serverStatusLive & st = m_axserver->m_serverSystem->m_serverStatus;
		s.Format("Running  |  UDP port %u  |  Players online: %d (peak %d)  |  Traffic in %lu / out %lu bytes (last pass)  |  Uptime %02u:%02u:%02u",
		         net->m_port, net->m_currentPlayersOnline, net->m_highestConstantPlayersOnline,
		         st.m_bandwidthIncommingPerSecond, st.m_bandwidthOutgoingPerSecond,
		         up / 3600, (up / 60) % 60, up % 60);
		if(m_axserver->m_serverSystem->m_shuttingDown)
			s += "  |  SHUTDOWN IN PROGRESS";
	}

	if(m_status.GetSafeHwnd())
	{
		CString cur;
		m_status.GetWindowText(cur);
		if(cur != s) m_status.SetWindowText(s);
	}
}

void CAXServerApplicationDlg::LayoutControls()
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

void CAXServerApplicationDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	LayoutControls();
}

void CAXServerApplicationDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 520;
	lpMMI->ptMinTrackSize.y = 320;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CAXServerApplicationDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == AXSERVER_TIMER_STATUS)
	{
		UpdateStatus();

		//GM "!shutdown" countdown - warn players, then close the server normally
		if(m_serverRunning && !m_startThread && m_axserver && m_axserver->m_serverSystem &&
		   m_axserver->m_serverSystem->m_sharedInterface)
		{
			CSharedNet * shared = m_axserver->m_serverSystem->m_sharedInterface;
			shared->ShutdownTick();
			if(shared->m_shutdownNow)
			{
				Log("Timed shutdown reached - shutting down.");
				OnBnClickedOk();
				return;
			}
			shared->RaidTick();
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CAXServerApplicationDlg::PreTranslateMessage(MSG* pMsg)
{
	// Enter/Esc must never shut the server down by accident - only the button or close box.
	if(pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN))
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CAXServerApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAXServerApplicationDlg::OnPaint()
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

HCURSOR CAXServerApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAXServerApplicationDlg::OnDestroy()
{
	AXSetLogSink(NULL, NULL);
	CDialog::OnDestroy();
}

void CAXServerApplicationDlg::StopServer()
{
	KillTimer(AXSERVER_TIMER_STATUS);
	if(m_axserver)
	{
		Log("Shutting down server ...");
		m_status.SetWindowText("Shutting down ...");
		UpdateWindow();
		delete m_axserver;
		m_axserver = NULL;
		Log("Server stopped.");
	}
	m_serverRunning = FALSE;
	AXSetLogSink(NULL, NULL);
	CoUninitialize();
}

void CAXServerApplicationDlg::OnBnClickedOk()
{
	if(m_startThread)
	{
		// Constructor still running on the worker thread - cannot tear down yet.
		Log("Server is still starting - wait for 'Server started' (or the failure message) before closing.");
		return;
	}
	StopServer();
	OnOK();
}

void CAXServerApplicationDlg::OnBnClickedCancel()
{
	// Close box / Esc behaves like the Shutdown button.
	OnBnClickedOk();
}

void CAXServerApplicationDlg::OnBnClickedBtnClearLog()
{
	m_log.SetWindowText("");
	m_logLines = 0;
}

void CAXServerApplicationDlg::OnBnClickedBtnOpenLog()
{
	ShellExecute(GetSafeHwnd(), "open", m_logFilePath, NULL, NULL, SW_SHOWNORMAL);
}

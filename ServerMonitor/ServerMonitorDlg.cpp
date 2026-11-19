// ServerMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServerMonitor.h"

#import "msado15.dll" \
         no_namespace rename("EOF", "EndOfFile")

#include <Psapi.h>
#include "ServerMonitorDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerMonitorDlg dialog

CServerMonitorDlg::CServerMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerMonitorDlg)
	m_gameServerIP = _T("");
	m_gameServerName = _T("");
	m_processName = _T("");
	m_currentStatus = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerMonitorDlg)
	DDX_Text(pDX, IDC_EDIT_gameServerIp, m_gameServerIP);
	DDX_Text(pDX, IDC_EDIT_gameServerName, m_gameServerName);
	DDX_Text(pDX, IDC_EDIT_processName, m_processName);
	DDX_Text(pDX, IDC_STATIC_CurrentStatus, m_currentStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CServerMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerMonitorDlg message handlers

BOOL CServerMonitorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    m_lastUpdateStamp = GetTickCount();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_currentStatusBool = -1;
	//read cfg
	UpdateData(TRUE);
	m_currentStatus = "Initializing...";
	CFileException exc;
    CFile fl;
	 CArchive the_Archive(&fl,CArchive::load);
     BOOL res = fl.Open("monitorcfg.cfg",CFile::modeRead, &exc);
	 if(res)
	 {
        the_Archive.ReadString(m_gameServerIP);
		the_Archive.ReadString(m_gameServerName);
		the_Archive.ReadString(m_processName);
        the_Archive.ReadString(m_sqlUserid);
		the_Archive.ReadString(m_sqlPass);
		the_Archive.ReadString(m_sqlDB);
		the_Archive.ReadString(m_sqlIp);
	
     the_Archive.Close();
	 fl.Close();
	 }
	UpdateData(FALSE);
	//end cfg
	InitializeCriticalSection(  &m_criticalSection );
	if(FAILED(CoInitialize(NULL)))
        return FALSE;
	SetTimer( 0, 60000, 0 );

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerMonitorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CServerMonitorDlg::FindProcess(CString processname)
{
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return FALSE;

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.

    for ( i = 0; i < cProcesses; i++ )
	{
        if(EnumProcessName( aProcesses[i] )==processname)
		{
		   
		   return TRUE;
		}
	}

	return FALSE;
}


CString CServerMonitorDlg::EnumProcessName( DWORD processID )
{
    
    char szProcessName[MAX_PATH] = "unknown";

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if ( hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName, 
                               sizeof(szProcessName) );
        }
    }

    CloseHandle( hProcess );
  return szProcessName;
}


void CServerMonitorDlg::OnTimer(UINT nIDEvent) 
{
	EnterCriticalSection(  &m_criticalSection  );
	UpdateData(TRUE);
	if(FindProcess(m_processName))
	{
       m_currentStatus = "Process is online";
	   m_currentStatusBool = 1;
	   UpdateSQLTable(m_gameServerName,m_gameServerIP,"Online");
	   m_lastUpdateStamp = GetTickCount();
	}
	else{
       m_currentStatus = "Process is offline";
	   m_currentStatusBool = 0;
	   UpdateSQLTable(m_gameServerName,m_gameServerIP,"Offline");
	   m_lastUpdateStamp = GetTickCount();
	}
	UpdateData(FALSE);
	
	CDialog::OnTimer(nIDEvent);
	LeaveCriticalSection( &m_criticalSection);
}

void CServerMonitorDlg::UpdateSQLTable(CString serverName, CString ipAdress, CString status)
{


    //_ConnectionPtr pConn;
    _RecordsetPtr  pRst;
	_RecordsetPtr  secondaryRecords;
    _bstr_t strCnn("Provider=sqloledb;Data Source="+m_sqlIp+";Initial Catalog="+m_sqlDB+";User Id="+m_sqlUserid+";Password="+m_sqlPass+";");
	pRst.CreateInstance(__uuidof(Recordset));
	secondaryRecords.CreateInstance(__uuidof(Recordset));
    BOOL enabled = TRUE;
	try
    {
	//execute connection method
      pRst->Open(
         "ServerStatus",//table
         strCnn,
         adOpenStatic,
         adLockOptimistic,
         adCmdTable);

         CString cmdBuild = "select * from ServerStatus where ServerName ='";
         cmdBuild = operator+(cmdBuild,serverName);
	     cmdBuild = operator+(cmdBuild,"'");

	     _bstr_t cmd = cmdBuild;
         secondaryRecords->Open(
           cmd,//table
           strCnn,
           adOpenStatic,
           adLockOptimistic,
           adCmdText);

           int curCount = secondaryRecords->RecordCount;
	       if(curCount == 0)
	       {// no records add one
             pRst->AddNew();
             pRst->Fields->GetItem("ServerName")->Value = (_bstr_t)serverName;//Works
             pRst->Fields->GetItem("ServerIP")->Value = (_bstr_t)ipAdress;//Works
	         pRst->Fields->GetItem("Status")->Value = (_bstr_t)status;//Works
		     pRst->Update();
	       }
	       else{//modify
             secondaryRecords->Fields->GetItem("ServerName")->Value = (_bstr_t)serverName;//Works
             secondaryRecords->Fields->GetItem("ServerIP")->Value = (_bstr_t)ipAdress;//Works
	         secondaryRecords->Fields->GetItem("Status")->Value = (_bstr_t)status;//Works
		     secondaryRecords->Update();
	       }//end modify
	     secondaryRecords->Close(); 
	     pRst->Close();  
      
	}
    catch(_com_error &e)
    {
	    enabled = FALSE;
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
    }	
}

void CServerMonitorDlg::OnBnClickedOk()
{  
	EnterCriticalSection(  &m_criticalSection  );
	UpdateSQLTable(m_gameServerName,m_gameServerIP,"Updating");
	LeaveCriticalSection( &m_criticalSection);
	OnOK();
}

void CServerMonitorDlg::OnDestroy()
{
	CDialog::OnDestroy();

	CoUninitialize();
}

// ContentServerApplicationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ContentServerApplication.h"
#include "ContentServerApplicationDlg.h"
#include ".\contentserverapplicationdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CContentServerApplicationDlg dialog



CContentServerApplicationDlg::CContentServerApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContentServerApplicationDlg::IDD, pParent)
	, m_bytesDownloaded(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CContentServerApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BytesDownloaded, m_bytesDownloaded);
	DDX_Control(pDX, IDC_progressTest, m_progressBar);
}

BEGIN_MESSAGE_MAP(CContentServerApplicationDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_StartServer, OnBnClickedBtnStartserver)
	ON_BN_CLICKED(IDC_BTN_StartClient, OnBnClickedBtnStartclient)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
END_MESSAGE_MAP()


// CContentServerApplicationDlg message handlers

BOOL CContentServerApplicationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    int structSizeTest = sizeof(cdlMemHolder);

	structSizeTest = sizeof(cdlMemHolderHash);
	
    CoInitialize(NULL);
	m_progressBar.SetRange32(0,100);
	m_contentServer = NULL;//new CAXContentServer();
	m_contentClient = NULL;
	mcServer = NULL;
    mcClient = NULL;
	SetTimer(1, 500, 0);
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CContentServerApplicationDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CContentServerApplicationDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CContentServerApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CContentServerApplicationDlg::OnDestroy()
{
	CDialog::OnDestroy();

	if(m_contentServer)
	{
       delete m_contentServer;
	   m_contentServer = 0;
	}
	if(m_contentClient)
	{
       delete m_contentClient;
	   m_contentClient = 0;
	}
	if(mcServer)
	{
       delete mcServer;
	   mcServer = 0;
	}
	if(mcClient)
	{
       delete mcClient;
	   mcClient = 0;
	}
	CoUninitialize();
}

void CContentServerApplicationDlg::OnBnClickedBtnStartserver()
{
	m_contentServer = new CAXContentServer();
}

void CContentServerApplicationDlg::OnBnClickedBtnStartclient()
{
	m_contentClient = new CAXContentClient();
	m_contentClient->SetCacheDirectory("C:\\");
    m_contentClient->SetUsernamePassword("ARB","RACKMONK");
	

	m_contentClient->DownloadFileRequest("AncientStaging.cab","64.34.176.219", 80);

	//m_contentClient->DownloadFileRequest("AncientStaging.cab","127.0.0.1", 80);
    //m_contentClient->DownloadFileRequest("EndlessMenuDynTextures.cab","127.0.0.1", 80);
	//m_contentClient->DownloadFileRequest("tool.bmp","127.0.0.1", 1050);
    //m_contentClient->DownloadFileRequest("256tikiimp.jpg","127.0.0.1", 1050);
	//m_contentClient->DownloadFileRequest("airworld_cvtest.cab","127.0.0.1", 1050);
	//m_contentClient->DownloadFileRequest("airworld_cv.cab","72.29.80.113", 21);
	//m_contentClient->DownloadFileRequest("acData.bkk","127.0.0.1", 1050);
	//m_contentClient->DownloadFileRequest("airworld_cv.cab","www.x-src.com", 21);
	//m_contentClient->DownloadFileRequest("airworld_cv.cab","www.mmolands.com", 21);
	//m_contentClient->DownloadFileRequest("airworld_cvtest.cab","192.168.1.199", 1050);
}

void CContentServerApplicationDlg::OnBnClickedButton1()
{
    mcServer = new CAX_XMLConnector(1040);
    mcClient = new CAX_XMLConnector(1040,"127.0.0.1");

	//CString data = "shut the heck up!@$";

	mcServer->SendMessage((BYTE*)"shut the heck up!@$",19,true);

	mcServer->SendMessage((BYTE*)"shut the heck up!@4",19,true);
	mcServer->SendMessage((BYTE*)"shut the heck up!@5",19,true);
	mcServer->SendMessage((BYTE*)"shut the heck up!@6",19,true);

	//mcClient->SendMessage((BYTE*)"test2",data.GetLength());
	//mcClient->SendMessage((BYTE*)data.GetBuffer(),data.GetLength());
	mcClient->SendMessage((BYTE*)"Villi test 2",12,true);
	Sleep(2000);
	mcClient->SendMessage((BYTE*)"Villi test 3",12,true);
	mcClient->SendMessage((BYTE*)"Villi test 4",12,true);
	mcClient->SendMessage((BYTE*)"Villi test 5",12,true);
}

void CContentServerApplicationDlg::OnTimer(UINT nIDEvent)
{
	if(m_contentClient)
	{
		UpdateData(TRUE);
	    m_progressBar.SetPos(m_contentClient->GetPercentageOfDownload());
        m_bytesDownloaded = m_contentClient->GetKBSec();
		UpdateData(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CContentServerApplicationDlg::OnBnClickedButton2()
{
	CAX_XMLCommandQueObj * cmPtr = mcServer->m_msgBuilder->GetNextCompletedMsg();
	if(cmPtr)
	{
       AfxMessageBox((char*)cmPtr->m_dataBlock,0,0);
	   delete cmPtr;
	   cmPtr = 0;
	}
	else
	{
	   AfxMessageBox("None in que",0,0);
       delete mcServer;
	   mcServer = 0;
	}
}

void CContentServerApplicationDlg::OnBnClickedButton3()
{
	CAX_XMLCommandQueObj * cmPtr = mcClient->m_msgBuilder->GetNextCompletedMsg();
	if(cmPtr)
	{
       AfxMessageBox((char*)cmPtr->m_dataBlock,0,0);
	   delete cmPtr;
	   cmPtr = 0;
	}
	else
	{
	   AfxMessageBox("None in que",0,0);
       delete mcClient;
	   mcClient = 0;

	}
}

void CContentServerApplicationDlg::OnBnClickedButton4()
{
	mcClient = new CAX_XMLConnector(1050,"127.0.0.1");

	//CString data = "shut the heck up!@$";

	mcClient->SendMessage((BYTE*)"<INIT>ENGINE</INIT>",19,true);
}

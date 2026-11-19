// CRCGeneratorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CRCGenerator.h"
#include "CRCGeneratorDlg.h"
#include ".\crcgeneratordlg.h"
#include "direct.h"

#include "CAXCabControl.h"

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


// CCRCGeneratorDlg dialog



CCRCGeneratorDlg::CCRCGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCRCGeneratorDlg::IDD, pParent)
	, m_CRCOUT(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCRCGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CRCOUT, m_CRCOUT);
}

BEGIN_MESSAGE_MAP(CCRCGeneratorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PickFile, OnBnClickedButtonPickfile)
	ON_BN_CLICKED(IDC_BUTTON_CabTest, OnBnClickedButtonCabtest)
END_MESSAGE_MAP()


// CCRCGeneratorDlg message handlers

BOOL CCRCGeneratorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

void CCRCGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCRCGeneratorDlg::OnPaint() 
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
HCURSOR CCRCGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCRCGeneratorDlg::OnBnClickedButtonPickfile()
{
static char BASED_CODE filter[] = "*.* Files|*.*|Any File|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   
	   CString fileName = opendialog.GetPathName();
	    CFileException exc;
		CFile fl;
		BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
		if(res == FALSE){
		    AfxMessageBox("Failed to open"+fileName);

			return;
		}
		UpdateData(TRUE);
		m_CRCOUT = getcrc( &fl );
		
		UpdateData(FALSE);
		fl.Close();
	   
	 }	
}

unsigned long CCRCGeneratorDlg::getcrc( CFile *fp )
{
	crcgen( );
	register unsigned long crc;
	unsigned long c;

	int bufSz = 2048;
    BYTE buffer[2048];
    int bytesRead = 0;
	crc = 0xFFFFFFFF;
	while(1) 
	{
        bytesRead = fp->Read(buffer,bufSz);
        if(bytesRead < 1)
		   break;

        for(int i=0;i<bytesRead;i++)
		{
			c = buffer[i];
		    crc = ((crc>>8) & 0x00FFFFFF) ^ crcTable[ (crc^c) & 0xFF ];

		}
	}
	return( crc^0xFFFFFFFF );
}

void CCRCGeneratorDlg::crcgen( )
{
	unsigned long	crc, poly;
	int	i, j;

	poly = 0xEDB88320L;
	for (i=0; i<256; i++) {
		crc = i;
		for (j=8; j>0; j--) {
			if (crc&1) {
				crc = (crc >> 1) ^ poly;
			} else {
				crc >>= 1;
			}
		}
		crcTable[i] = crc;
	}
}

void CCRCGeneratorDlg::OnBnClickedButtonCabtest()
{
    /*static char BASED_CODE filter[] = "*.* Files|*.*|Any File|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetPathName();
	    
		
		CCabinetObj cbObj;
	    int percentage = 0;
		cbObj.ExtractCabinetToDirectory("c:\\TESTDIR\\",fileName,&percentage,NULL);
		
	   UpdateData(FALSE);
	 }	*/


}

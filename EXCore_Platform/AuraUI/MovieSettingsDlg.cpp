// MovieSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MovieSettingsDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMovieSettingsDlg dialog


CMovieSettingsDlg::CMovieSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMovieSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMovieSettingsDlg)
	m_movieFile = _T("");
	m_percBottom = 0;
	m_percLeft = 0;
	m_percRight = 0;
	m_percTop = 0;
	//}}AFX_DATA_INIT
}


void CMovieSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMovieSettingsDlg)
	DDX_Control(pDX, IDC_RADIO_MovieLoops, m_radioMovieLoops);
	DDX_Text(pDX, IDC_EDIT_MovieFile, m_movieFile);
	DDX_Text(pDX, IDC_EDIT_PercBottom, m_percBottom);
	DDX_Text(pDX, IDC_EDIT_PercLeft, m_percLeft);
	DDX_Text(pDX, IDC_EDIT_PercRight, m_percRight);
	DDX_Text(pDX, IDC_EDIT_PercTop, m_percTop);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMovieSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CMovieSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEfile, OnBUTTONBROWSEfile)
	ON_BN_CLICKED(IDC_RADIO_MovieLoops, OnRADIOMovieLoops)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMovieSettingsDlg message handlers

void CMovieSettingsDlg::OnBUTTONBROWSEfile() 
{
    //use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\Movies\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization
    static char BASED_CODE filter[] = "avi Files|*.avi|MPEG Files|*.mpg|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_movieFile = opendialog.GetFileName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CMovieSettingsDlg::OnRADIOMovieLoops() 
{
	if(movieLoops == TRUE){
       movieLoops = FALSE;
	   m_radioMovieLoops.SetCheck(0);
	}
	else{
       movieLoops = TRUE;
	   m_radioMovieLoops.SetCheck(1);
	}
	
}

BOOL CMovieSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(movieLoops==TRUE)
    m_radioMovieLoops.SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// SoundRangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "SoundRangeDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundRangeDlg dialog


CSoundRangeDlg::CSoundRangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundRangeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundRangeDlg)
	m_endRange = 0;
	m_fileName = _T("");
	m_startRange = 0;
	m_id = 0;
	m_soundFactor = 0.0f;
	m_clipRadius = 0.0f;
	//}}AFX_DATA_INIT
}


void CSoundRangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundRangeDlg)
	DDX_Text(pDX, IDC_EDIT_ENDRANGE, m_endRange);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_fileName);
	DDX_Text(pDX, IDC_EDIT_STARTRANGE, m_startRange);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_SoundFactor, m_soundFactor);
	DDX_Text(pDX, IDC_EDIT_ClipRadius, m_clipRadius);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_LOOPING, m_loopingCheck);
	DDX_Control(pDX, IDC_CHECK_VelocityBased, m_velocityBasedCheck);
}


BEGIN_MESSAGE_MAP(CSoundRangeDlg, CDialog)
	//{{AFX_MSG_MAP(CSoundRangeDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundRangeDlg message handlers

void CSoundRangeDlg::OnButtonBrowse() 
{
	//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\Sounds\\");
			 SetCurrentDirectory(CurDir);
 //end use mapsmodels folder for organization
 static char BASED_CODE filter[] = "Wav Files|*.wav|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_fileName = opendialog.GetFileName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

BOOL CSoundRangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);
	m_loopingCheck.SetCheck(m_looping);
	m_velocityBasedCheck.SetCheck(m_velocityBased);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSoundRangeDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_looping = m_loopingCheck.GetCheck();
	m_velocityBased = m_velocityBasedCheck.GetCheck();
	UpdateData(FALSE);
	OnOK();
}

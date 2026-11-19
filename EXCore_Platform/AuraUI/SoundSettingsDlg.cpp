// SoundSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SoundSettingsDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundSettingsDlg dialog


CSoundSettingsDlg::CSoundSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundSettingsDlg)
	m_distanceFactor = 0.0f;
	m_fileName = _T("");
	m_sndID = 0;
	//}}AFX_DATA_INIT
}


void CSoundSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundSettingsDlg)
	DDX_Control(pDX, IDC_CHECK_LOOPING, m_checkLooping);
	DDX_Control(pDX, IDC_CHECK_ENABLED, m_checkEnabled);
	DDX_Text(pDX, IDC_EDIT_DistanceFactor, m_distanceFactor);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_fileName);
	DDX_Text(pDX, IDC_EDIT_SoundID, m_sndID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSoundSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_BrowseFileName, OnBUTTONBrowseFileName)
	ON_BN_CLICKED(IDC_CHECK_ENABLED, OnCheckEnabled)
	ON_BN_CLICKED(IDC_CHECK_LOOPING, OnCheckLooping)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundSettingsDlg message handlers

void CSoundSettingsDlg::OnBUTTONBrowseFileName() 
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
     static char BASED_CODE filter[] = "wav Files|*.wav|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   m_fileName = fileName;
	   UpdateData(FALSE);
	 }	
	SetCurrentDirectory(MainDir);
}

BOOL CSoundSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(enabled == TRUE)
	   m_checkEnabled.SetCheck(1);

	if(looping == TRUE)
	   m_checkLooping.SetCheck(1);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoundSettingsDlg::OnCheckEnabled() 
{
	if(enabled == TRUE){
	   m_checkEnabled.SetCheck(0);
	   enabled = FALSE;
	}
	else{
       m_checkEnabled.SetCheck(1);
	   enabled = TRUE;
	}
}

void CSoundSettingsDlg::OnCheckLooping() 
{
	if(looping == TRUE){
	   m_checkLooping.SetCheck(0);
	   looping = FALSE;
	}
	else{
       m_checkLooping.SetCheck(1);
	   looping = TRUE;
	}
}

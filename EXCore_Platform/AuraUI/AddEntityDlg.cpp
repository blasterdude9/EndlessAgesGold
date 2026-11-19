// AddEntityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "AddEntityDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddEntityDlg dialog


CAddEntityDlg::CAddEntityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddEntityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddEntityDlg)
	m_entityName = _T("");
	m_filename = _T("");
	m_secondaryCollisionFile = _T("");
	//}}AFX_DATA_INIT
}


void CAddEntityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddEntityDlg)
	DDX_Control(pDX, IDC_CHECK_SaveCollisionInfo, m_checkSaveCollisionInfo);
	DDX_Text(pDX, IDC_EDIT_ENTITYNAME, m_entityName);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_filename);
	DDX_Text(pDX, IDC_EDIT_SecondaryColisFile, m_secondaryCollisionFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddEntityDlg, CDialog)
	//{{AFX_MSG_MAP(CAddEntityDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_CHECK_SaveCollisionInfo, OnCHECKSaveCollisionInfo)
	ON_BN_CLICKED(IDC_BUTTON_BrowseSecondary, OnBUTTONBrowseSecondary)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddEntityDlg message handlers

BOOL CAddEntityDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	if(saveCollisionInfo == TRUE)
	m_checkSaveCollisionInfo.SetCheck(1);
	else
	m_checkSaveCollisionInfo.SetCheck(0);
    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddEntityDlg::OnButtonBrowse() 
{
//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
 //end use mapsmodels folder for organization
 static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_filename = opendialog.GetFileName();
	   m_pathGlobal = opendialog.GetPathName();
	   m_folderPath = m_pathGlobal;
	   m_folderPath.Replace(m_filename,"");
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CAddEntityDlg::OnCHECKSaveCollisionInfo() 
{
    UpdateData(TRUE);
	if(saveCollisionInfo == TRUE){
	m_checkSaveCollisionInfo.SetCheck(0);
	saveCollisionInfo=FALSE;
	}
	else{
	m_checkSaveCollisionInfo.SetCheck(1);
    saveCollisionInfo=TRUE;
	}
    UpdateData(FALSE);
}

void CAddEntityDlg::OnBUTTONBrowseSecondary() 
{
	//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
 //end use mapsmodels folder for organization
 static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_secondaryCollisionFile = opendialog.GetFileName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CAddEntityDlg::OnOK() 
{
	UpdateData(TRUE);
	if(saveCollisionInfo == FALSE && m_secondaryCollisionFile == ""){
        MessageBox("Must have a secondary collision file specified");
		return;
	}
	UpdateData(FALSE);
	CDialog::OnOK();
}

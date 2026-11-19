// AddScriptEventDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AddScriptEventDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddScriptEventDlg dialog


CAddScriptEventDlg::CAddScriptEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddScriptEventDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddScriptEventDlg)
	m_event = _T("");
	m_miscInt = 0;
	m_miscText = _T("");
	//}}AFX_DATA_INIT
}


void CAddScriptEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddScriptEventDlg)
	DDX_CBString(pDX, IDC_COMBO_EVENT, m_event);
	DDX_Text(pDX, IDC_EDIT_MISCINT, m_miscInt);
	DDX_Text(pDX, IDC_EDIT_MISCTEXT, m_miscText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddScriptEventDlg, CDialog)
	//{{AFX_MSG_MAP(CAddScriptEventDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddScriptEventDlg message handlers

void CAddScriptEventDlg::OnButtonBrowse() 
{
//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
//end use mapsmodels folder for organization
static char BASED_CODE filter[] = "*.* Files|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   m_miscText = fileName;
	   UpdateData(FALSE);
	 }	
	SetCurrentDirectory(MainDir);
}

void CAddScriptEventDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CAddScriptEventDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

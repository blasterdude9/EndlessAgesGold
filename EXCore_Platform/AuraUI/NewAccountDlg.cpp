// NewAccountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../EngineMainApp/resource.h"
#include "NewAccountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewAccountDlg dialog


CNewAccountDlg::CNewAccountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewAccountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewAccountDlg)
	m_password = _T("");
	m_userName = _T("");
	//}}AFX_DATA_INIT
}


void CNewAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewAccountDlg)
	DDX_Control(pDX, IDC_CHECK_REGISTEREDVER, m_registeredVersion);
	DDX_Text(pDX, IDC_EDIT_Password, m_password);
	DDX_Text(pDX, IDC_EDIT_USERNAME, m_userName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewAccountDlg, CDialog)
	//{{AFX_MSG_MAP(CNewAccountDlg)
	ON_BN_CLICKED(IDC_CHECK_REGISTEREDVER, OnCheckRegisteredver)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewAccountDlg message handlers

BOOL CNewAccountDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    registered= FALSE;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewAccountDlg::OnCheckRegisteredver() 
{
	if(registered == TRUE)
	   registered=FALSE;
	else
       registered=TRUE;
}

// ServerSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ServerSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSetupDlg dialog


CServerSetupDlg::CServerSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerSetupDlg)
	m_connectionLimit = 0;
	m_password = _T("");
	m_serverName = _T("");
	//}}AFX_DATA_INIT
}


void CServerSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerSetupDlg)
	DDX_Control(pDX, IDC_CHECK_PasswordRequired, m_checkPasswordRequired);
	DDX_Control(pDX, IDC_CHECK_Encyption, m_checkEncyption);
	DDX_Text(pDX, IDC_EDIT_ConnectionLimit, m_connectionLimit);
	DDX_Text(pDX, IDC_EDIT_Password, m_password);
	DDX_Text(pDX, IDC_EDIT_SERVERNAME, m_serverName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CServerSetupDlg)
	ON_BN_CLICKED(IDC_CHECK_Encyption, OnCHECKEncyption)
	ON_BN_CLICKED(IDC_CHECK_PasswordRequired, OnCHECKPasswordRequired)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerSetupDlg message handlers

void CServerSetupDlg::OnCHECKEncyption() 
{
	if(encrypted == TRUE){
	   m_checkEncyption.SetCheck(0);
       encrypted= FALSE;
	   }
	else{
       m_checkEncyption.SetCheck(1);
       encrypted= TRUE;
	}
}

void CServerSetupDlg::OnCHECKPasswordRequired() 
{
	if(passwordRequired == TRUE){
	   m_checkPasswordRequired.SetCheck(0);
       passwordRequired= FALSE;
	   }
	else{
       m_checkPasswordRequired.SetCheck(1);
       passwordRequired= TRUE;
	}
}

BOOL CServerSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(passwordRequired == TRUE)
	   m_checkPasswordRequired.SetCheck(1);
	if(encrypted == TRUE)
	   m_checkEncyption.SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

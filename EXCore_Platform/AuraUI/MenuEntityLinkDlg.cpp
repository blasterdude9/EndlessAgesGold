// MenuEntityLinkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MenuEntityLinkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuEntityLinkDlg dialog


CMenuEntityLinkDlg::CMenuEntityLinkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuEntityLinkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMenuEntityLinkDlg)
	m_databaseIndex = 0;
	m_initialAnimationIndex = 0;
	m_aiCfgOverride = 0;
	//}}AFX_DATA_INIT
}


void CMenuEntityLinkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuEntityLinkDlg)
	DDX_Control(pDX, IDC_CHECK_runtimeDB, m_runtimeDBCheckBox);
	DDX_Text(pDX, IDC_EDIT_database, m_databaseIndex);
	DDX_Text(pDX, IDC_EDIT_initialAnimationIndex, m_initialAnimationIndex);
	DDX_Text(pDX, IDC_EDIT_AiCfgOverride, m_aiCfgOverride);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuEntityLinkDlg, CDialog)
	//{{AFX_MSG_MAP(CMenuEntityLinkDlg)
	ON_BN_CLICKED(IDC_CHECK_runtimeDB, OnCHECKruntimeDB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuEntityLinkDlg message handlers

void CMenuEntityLinkDlg::OnCHECKruntimeDB() 
{
	if(runtimeDB)
       runtimeDB=FALSE;
	else
       runtimeDB=TRUE;
	
}

BOOL CMenuEntityLinkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	if(runtimeDB)
       m_runtimeDBCheckBox.SetCheck(1);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

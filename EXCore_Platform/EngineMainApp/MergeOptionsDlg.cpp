// MergeOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MergeOptionsDlg.h"
#include ".\mergeoptionsdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMergeOptionsDlg dialog


CMergeOptionsDlg::CMergeOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMergeOptionsDlg::IDD, pParent)
	, m_groupMode(0)
	, m_transitionGroupID(0)
{
	//{{AFX_DATA_INIT(CMergeOptionsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMergeOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMergeOptionsDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_groupMode, m_groupMode);
	DDX_Text(pDX, IDC_EDIT_groupMode2, m_transitionGroupID);
}


BEGIN_MESSAGE_MAP(CMergeOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(CMergeOptionsDlg)
	ON_BN_CLICKED(IDC_CHECK_AiMergeMode, OnCHECKAiMergeMode)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMergeOptionsDlg message handlers

BOOL CMergeOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_aiMergeType = FALSE;
	m_groupMode = -1;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMergeOptionsDlg::OnCHECKAiMergeMode() 
{
	if(m_aiMergeType)
	   m_aiMergeType = FALSE;
	else
	   m_aiMergeType = TRUE;
	
}

void CMergeOptionsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

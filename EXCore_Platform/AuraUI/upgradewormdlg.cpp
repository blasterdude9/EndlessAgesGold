// UpgradeWormDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "UpgradeWormDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpgradeWormDlg dialog


CUpgradeWormDlg::CUpgradeWormDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpgradeWormDlg::IDD, pParent)
	, m_specialCode(_T(""))
{
	//{{AFX_DATA_INIT(CUpgradeWormDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUpgradeWormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpgradeWormDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_specialCode, m_specialCode);
}


BEGIN_MESSAGE_MAP(CUpgradeWormDlg, CDialog)
	//{{AFX_MSG_MAP(CUpgradeWormDlg)
	ON_BN_CLICKED(IDC_CHECK_DisarmAll, OnCHECKDisarmAll)
	ON_BN_CLICKED(IDC_CHECK_DeleteUnknownItems, OnCHECKDeleteUnknownItems)
	ON_BN_CLICKED(IDC_CHECK_useDeathCfg, OnCHECKuseDeathCfg)
	ON_BN_CLICKED(IDC_CHECK_rebuildGroupDatabases, OnCHECKrebuildGroupDatabases)
	ON_BN_CLICKED(IDC_CHECK_clear, OnCHECKclear)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpgradeWormDlg message handlers

void CUpgradeWormDlg::OnCHECKDisarmAll() 
{
	if(!m_disarmAll)
		m_disarmAll = TRUE;
	else
	    m_disarmAll = FALSE;
}

BOOL CUpgradeWormDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_disarmAll = FALSE;
	m_useDeathCfg = FALSE;
	m_rebuildGroupDatabases = FALSE;
	m_clearAll = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUpgradeWormDlg::OnCHECKDeleteUnknownItems() 
{
	
	if(!m_deleteAllUnknownItems)
		m_deleteAllUnknownItems = TRUE;
	else
	    m_deleteAllUnknownItems = FALSE;
	
}

void CUpgradeWormDlg::OnCHECKuseDeathCfg() 
{
	if(!m_useDeathCfg)
		m_useDeathCfg = TRUE;
	else
	    m_useDeathCfg = FALSE;
}

void CUpgradeWormDlg::OnCHECKrebuildGroupDatabases() 
{
	if(!m_rebuildGroupDatabases)
		m_rebuildGroupDatabases = TRUE;
	else
	    m_rebuildGroupDatabases = FALSE;
	
}

void CUpgradeWormDlg::OnCHECKclear() 
{
	if(!m_clearAll)
		m_clearAll = TRUE;
	else
	    m_clearAll = FALSE;
}

void CUpgradeWormDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

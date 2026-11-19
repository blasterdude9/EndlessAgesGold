// WorldGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WorldGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorldGroupDlg dialog


CWorldGroupDlg::CWorldGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorldGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWorldGroupDlg)
	m_groupID = 0;
	m_groupName = _T("");
	//}}AFX_DATA_INIT
}


void CWorldGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorldGroupDlg)
	DDX_Text(pDX, IDC_EDIT_GroupID, m_groupID);
	DDX_Text(pDX, IDC_EDIT_GroupName, m_groupName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorldGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CWorldGroupDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_IDSpinner, OnDeltaposSPINIDSpinner)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorldGroupDlg message handlers

void CWorldGroupDlg::OnDeltaposSPINIDSpinner(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    m_groupID -= pNMUpDown->iDelta;
    UpdateData(FALSE);
	*pResult = 0;
}

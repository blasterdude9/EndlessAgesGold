// MenuAddUnitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "MenuAddUnitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuAddUnitDlg dialog


CMenuAddUnitDlg::CMenuAddUnitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuAddUnitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMenuAddUnitDlg)
	m_name = _T("New Visual");
	m_visualID = 0;
	m_visualType = 0;
	m_freeVisualID = 0;
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CMenuAddUnitDlg, CDialog)
	//{{AFX_MSG_MAP(CMenuAddUnitDlg)
	ON_BN_CLICKED(IDC_BUTTON_VisualGetFreeVisualID, OnButtonGetFreeVisualID)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CMenuAddUnitDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuAddUnitDlg message handlers

void CMenuAddUnitDlg::OnOK() 
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_name = temp;

	m_visualID	 = GetDlgItemInt(IDC_EDIT_VisualVisualID);
	m_visualType = SendDlgItemMessage(IDC_COMBO_visualType, CB_GETCURSEL, 0, 0) + 2; //Add 2 to skip over NONE and WINDOW

	CDialog::OnOK();
} 

BOOL CMenuAddUnitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_EDIT_VisualName, "Unnamed");
	SetDlgItemInt(IDC_EDIT_VisualVisualID, m_freeVisualID);
	SendDlgItemMessage(IDC_COMBO_visualType, CB_SETCURSEL, 0, 0); //TODO: Switch to selecting 0
	return TRUE;
}

void CMenuAddUnitDlg::OnButtonGetFreeVisualID()
{
	SetDlgItemInt(IDC_EDIT_VisualVisualID, m_freeVisualID);
}

void CMenuAddUnitDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

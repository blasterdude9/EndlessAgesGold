// CurrencyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CurrencyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurrencyDlg dialog


CCurrencyDlg::CCurrencyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurrencyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCurrencyDlg)
	m_currencyName = _T("");
	//}}AFX_DATA_INIT
}


void CCurrencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurrencyDlg)
	DDX_Text(pDX, IDC_EDIT_CurrencyName, m_currencyName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCurrencyDlg, CDialog)
	//{{AFX_MSG_MAP(CCurrencyDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurrencyDlg message handlers

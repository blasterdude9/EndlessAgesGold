// AIRaidDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AIRaidDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAIRaidDlg dialog


CAIRaidDlg::CAIRaidDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAIRaidDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAIRaidDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAIRaidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAIRaidDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAIRaidDlg, CDialog)
	//{{AFX_MSG_MAP(CAIRaidDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAIRaidDlg message handlers

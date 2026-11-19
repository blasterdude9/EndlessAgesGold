// ProgressMeterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ProgressMeterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProgressMeterDlg dialog


CProgressMeterDlg::CProgressMeterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressMeterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProgressMeterDlg)
	//}}AFX_DATA_INIT
}


void CProgressMeterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressMeterDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProgressMeterDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressMeterDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


BOOL CProgressMeterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

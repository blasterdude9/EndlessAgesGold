// ConnectionMethodDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ConnectionMethodDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectionMethodDlg dialog


CConnectionMethodDlg::CConnectionMethodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectionMethodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectionMethodDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConnectionMethodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectionMethodDlg)
	DDX_Control(pDX, IDC_LIST_ConnectionMethods, m_listMethods);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectionMethodDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectionMethodDlg)
	ON_LBN_DBLCLK(IDC_LIST_ConnectionMethods, OnDblclkLISTConnectionMethods)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectionMethodDlg message handlers

BOOL CConnectionMethodDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	for(int loop=0;loop<SessionCount;loop++){
	m_listMethods.AddString(Sessions[loop]);
	}
	if(SessionCount > 0)
	m_listMethods.SetCurSel(0);
	finalSelection=0;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectionMethodDlg::OnDblclkLISTConnectionMethods() 
{
	int curSelect = m_listMethods.GetCurSel();
	if(curSelect == -1)
		return;

	finalSelection = curSelect;
	OnOK();
}

void CConnectionMethodDlg::OnOK() 
{
	int curSelect = m_listMethods.GetCurSel();
	if(curSelect == -1)
		return;

	finalSelection = curSelect;
	CDialog::OnOK();
}

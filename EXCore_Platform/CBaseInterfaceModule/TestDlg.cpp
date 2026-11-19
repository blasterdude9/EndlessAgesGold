// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBaseInterfaceModule.h"
#include "TestDlg.h"


// TestDlg dialog

IMPLEMENT_DYNAMIC(TestDlg, CDialog)
TestDlg::TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TestDlg::IDD, pParent)
	, m_str(_T(""))
{
}

TestDlg::~TestDlg()
{
}

void TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_testStaticed, m_str);
}


BEGIN_MESSAGE_MAP(TestDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &TestDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// TestDlg message handlers

void TestDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}

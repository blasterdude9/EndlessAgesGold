// testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "testDlg.h"


// CtestDlg dialog

IMPLEMENT_DYNAMIC(CtestDlg, CDialog)
CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestDlg::IDD, pParent)
	, m_string(_T(""))
{
}

CtestDlg::~CtestDlg()
{
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CtestDlg, CDialog)
END_MESSAGE_MAP()


// CtestDlg message handlers

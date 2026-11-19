// TestExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TestExportDlg.h"


// CTestExportDlg dialog

IMPLEMENT_DYNAMIC(CTestExportDlg, CDialog)
CTestExportDlg::CTestExportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestExportDlg::IDD, pParent)
{
}

CTestExportDlg::~CTestExportDlg()
{
}

void CTestExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestExportDlg, CDialog)
END_MESSAGE_MAP()


// CTestExportDlg message handlers

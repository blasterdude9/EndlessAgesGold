// TreeImportOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TreeImportOptionsDlg.h"


// CTreeImportOptionsDlg dialog

IMPLEMENT_DYNAMIC(CTreeImportOptionsDlg, CDialog)
CTreeImportOptionsDlg::CTreeImportOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreeImportOptionsDlg::IDD, pParent)
	, m_leafLodCount(0)
	, m_sizeOverride(0)
{
}

CTreeImportOptionsDlg::~CTreeImportOptionsDlg()
{
}

void CTreeImportOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_size_ovverride, m_sizeOverride);
	DDX_Text(pDX, IDC_EDIT_leafLodCount, m_leafLodCount);
}


BEGIN_MESSAGE_MAP(CTreeImportOptionsDlg, CDialog)
END_MESSAGE_MAP()


// CTreeImportOptionsDlg message handlers

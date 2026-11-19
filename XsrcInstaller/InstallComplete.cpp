// InstallComplete.cpp : implementation file
//

#include "stdafx.h"
#include "XsrcInstaller.h"
#include "InstallComplete.h"


// CInstallComplete dialog

IMPLEMENT_DYNAMIC(CInstallComplete, CDialog)
CInstallComplete::CInstallComplete(CWnd* pParent /*=NULL*/)
	: CDialog(CInstallComplete::IDD, pParent)
{
}

CInstallComplete::~CInstallComplete()
{
}

void CInstallComplete::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInstallComplete, CDialog)
END_MESSAGE_MAP()


// CInstallComplete message handlers

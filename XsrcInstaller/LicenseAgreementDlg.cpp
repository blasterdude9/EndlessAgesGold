// LicenseAgreementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XsrcInstaller.h"
#include "LicenseAgreementDlg.h"


// CLicenseAgreementDlg dialog

IMPLEMENT_DYNAMIC(CLicenseAgreementDlg, CDialog)
CLicenseAgreementDlg::CLicenseAgreementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLicenseAgreementDlg::IDD, pParent)
	, m_licenseEdit(_T(""))
{
}

CLicenseAgreementDlg::~CLicenseAgreementDlg()
{
}

void CLicenseAgreementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_License, m_licenseEdit);
}


BEGIN_MESSAGE_MAP(CLicenseAgreementDlg, CDialog)
END_MESSAGE_MAP()


// CLicenseAgreementDlg message handlers

#pragma once


// CLicenseAgreementDlg dialog

class CLicenseAgreementDlg : public CDialog
{
	DECLARE_DYNAMIC(CLicenseAgreementDlg)

public:
	CLicenseAgreementDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLicenseAgreementDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_License };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_licenseEdit;
};

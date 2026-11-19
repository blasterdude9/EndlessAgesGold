#pragma once


// CInstallComplete dialog

class CInstallComplete : public CDialog
{
	DECLARE_DYNAMIC(CInstallComplete)

public:
	CInstallComplete(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstallComplete();

// Dialog Data
	enum { IDD = IDD_DIALOG_InstallationComplete };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

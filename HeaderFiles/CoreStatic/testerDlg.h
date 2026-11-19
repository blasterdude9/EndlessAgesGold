#pragma once


// CtesterDlg dialog

class AFX_EXT_CLASS  CtesterDlg : public CDialog
{
	DECLARE_DYNAMIC(CtesterDlg)

public:
	CtesterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CtesterDlg();

// Dialog Data

	enum { IDD = IDD_DIALOG_Tester2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

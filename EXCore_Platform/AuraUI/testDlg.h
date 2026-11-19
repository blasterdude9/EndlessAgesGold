#pragma once


// CtestDlg dialog

class CtestDlg : public CDialog
{
	DECLARE_DYNAMIC(CtestDlg)

public:
	CtestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CtestDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_string;
};

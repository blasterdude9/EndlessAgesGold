#pragma once


// CtestEmbeddedDlg dialog

class AFX_EXT_CLASS CtestEmbeddedDlg : public CDialog
{
	DECLARE_DYNAMIC(CtestEmbeddedDlg)

public:
	CtestEmbeddedDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CtestEmbeddedDlg();

//#ifndef IDD_DIALOG_testEmbedded

//#endif

// Dialog Data

	enum { IDD = IDD_DIALOG_testEmbedded };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtontest();
};

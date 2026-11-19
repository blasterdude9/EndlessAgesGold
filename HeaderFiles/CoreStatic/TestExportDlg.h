#pragma once


// CTestExportDlg dialog

class AFX_EXT_CLASS  CTestExportDlg : public CDialog
{
	DECLARE_DYNAMIC(CTestExportDlg)

public:
	CTestExportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestExportDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_testExport };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

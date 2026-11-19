#pragma once


//CDirectorySelect dialog

class CDirectorySelect : public CDialog
{
	DECLARE_DYNAMIC(CDirectorySelect)

public:
	CDirectorySelect(CWnd* pParent = NULL);   //standard constructor
	virtual ~CDirectorySelect();

//Dialog Data
	enum { IDD = IDD_DIALOG_DirectorySelect };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    //DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedOk();
	CString m_selectedDirectory;
};

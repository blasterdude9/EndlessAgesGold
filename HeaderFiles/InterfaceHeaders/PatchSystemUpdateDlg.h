
#include "CRCgenerator.h"
#pragma once
#include "afxwin.h"



// CPatchSystemUpdateDlg dialog

class CPatchSystemUpdateDlg : public CDialog
{
	DECLARE_DYNAMIC(CPatchSystemUpdateDlg)

public:
	CPatchSystemUpdateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPatchSystemUpdateDlg();

	CCRCgenerator m_crcGenerator;
	CString m_fileName;
	CString m_filePath;
	__int64 m_crc;
	CString m_crcString;
	__int64 m_cabCrc;
	CString m_cabCrcString;
	CString m_cabName;
	CString m_cabPath;

	CString m_offsetPath;


// Dialog Data
	enum { IDD = IDD_DIALOG_patchFileUpdate };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedButtongmcconetntbrowsefile();
public:
	afx_msg void OnBnClickedButtongmcupdatetable();
public:
	CString m_crcOutput;
public:
	CString m_filePathOutput;
public:
	afx_msg void OnBnClickedOk();
public:
	CEdit m_contentID;
public:
	afx_msg void OnBnClickedButtonBrowsefilename();
public:
	afx_msg void OnBnClickedButtonBrowsecabname();
public:
	CEdit m_fileNameEdit;
public:
	CEdit m_cabFileEdit;
public:
	CString m_crccaboutput;
};

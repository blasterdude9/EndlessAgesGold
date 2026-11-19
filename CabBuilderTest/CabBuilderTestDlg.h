// CabBuilderTestDlg.h : header file
//
#include "Cabinet.Hpp"
#include "afxwin.h"
#include "afxcmn.h"
#pragma once


// CCabBuilderTestDlg dialog
class CCabBuilderTestDlg : public CDialog
{
// Construction
public:
	CCabBuilderTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CABBUILDERTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonaddfiles();
	CString m_baseDirectory;
	CListBox m_fileList;
	afx_msg void OnBnClickedButtonBasedirectory();
	CProgressCtrl m_progressCtrl;
	afx_msg void OnBnClickedButtoncompress();
	CString m_cabName;
	afx_msg void OnBnClickedButtonOpenfiles();
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedButtonExtract();
};

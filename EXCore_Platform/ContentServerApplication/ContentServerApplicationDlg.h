// ContentServerApplicationDlg.h : header file
//
#include ".\axcontentserver.h"
#include ".\axcontentclient.h"
#include "afxcmn.h"
#include "ax_xmlconnector.h"
#pragma once

struct cdlMemHolder
{
 void * m_platerObj;
 char m_inUse;//0=no 1=yes
};

struct cdlMemHolderHash
{
 CObList limitedSearchList;
};


// CContentServerApplicationDlg dialog
class CContentServerApplicationDlg : public CDialog
{
// Construction
public:
	CContentServerApplicationDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CONTENTSERVERAPPLICATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CAXContentServer * m_contentServer;
	CAXContentClient * m_contentClient;

	CAX_XMLConnector * mcServer;
    CAX_XMLConnector * mcClient;
	

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnStartserver();
	afx_msg void OnBnClickedBtnStartclient();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT nIDEvent);
	long m_bytesDownloaded;
	CProgressCtrl m_progressBar;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
};

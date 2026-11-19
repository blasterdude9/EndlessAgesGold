// XsrcInstallerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "installbuilder.h"
#include "cabinet.hpp"
//#include "shellapi.h"
//#include "shobjidl.h"


#include "CAXCabControl.h"
// CXsrcInstallerDlg dialog
class CXsrcInstallerDlg : public CDialog
{
// Construction
public:
	CXsrcInstallerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_XSRCINSTALLER_DIALOG };

	public:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    BOOL m_closeNow;
	int m_globalPercDone;
	CRITICAL_SECTION m_cSection;
	HANDLE m_lgThreadHandle;
    CString m_appTitle;
	CString m_statusPub;
// Implementation
protected:
	HICON m_hIcon;
	BOOL m_timerLock;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonInstall();
	afx_msg void OnBnClickedButtonBuildinstaller();

	void MakeDirectory(LPSTR path);
	CProgressCtrl m_progressInstall;
	CString m_installPath;
	afx_msg void OnBnClickedButtonUninstall();
	afx_msg void OnBnClickedOk();
	HRESULT CreateLink(CString lpszPathObj, CString lpszPathLink, CString workingDirectoy, CString lpszDesc) ;
    CString GetSystemDrive();
	CString GetDeskTopFolder();
	CString ConvertLPWSTR(LPWSTR & string);
	CString ConvToString(int number);
	DWORD m_bytesDownloaded;
	DWORD m_lastbytesDownloaded;
	DWORD m_LastTimeStampBytes;
	DWORD m_kbSec;
	/*void SetSoftwareRegistry(LPSTR headerX,
							 LPSTR valueName,
							 LPSTR valueDataX);
	LPSTR GetSoftwareRegkeyValue(LPSTR headerX,LPSTR valueRequest);*/
	afx_msg void OnDestroy();
	CButton m_buildInstallerBtn;
	CButton m_installBtn;
	CButton m_uninstallBtn;
	CEdit m_installPathCnt;
	afx_msg void OnTimer(UINT nIDEvent);
	CString m_statusOutput;
	CString m_kbout;
	afx_msg void OnEnChangeEditinstallationpath();
	CStatic m_kbOutVis;
};

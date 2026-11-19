#if !defined(AFX_SERVERSETTINGSDLG_H__0D35D5F4_5EC3_46F2_A5A1_1876124DE690__INCLUDED_)
#define AFX_SERVERSETTINGSDLG_H__0D35D5F4_5EC3_46F2_A5A1_1876124DE690__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSettingsDlg.h : header file
//
//#include "CFilePatchClass.h"
#include "CServerList.h"

/////////////////////////////////////////////////////////////////////////////
// CServerSettingsDlg dialog

class CServerSettingsDlg : public CDialog
{
// Construction
public:
	CServerSettingsDlg(CWnd* pParent = NULL);   // standard constructor

	CFilePatchObjectList         *m_filePatchDBREF;
	CServerListObjectDB          *m_availableServersREF;

// Dialog Data
	//{{AFX_DATA(CServerSettingsDlg)
	enum { IDD = IDD_DIALOG_serverSettings };
	CListBox	m_availableServerList;
	CListBox	m_patchDatabase;
	CString	m_serverName;
	CString	m_ipAdress;
	int		m_versionOutput;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InListAvailableServers(int sel);
    void InListPatches(int sel);
	CString ConvToString(int number);
	// Generated message map functions
	//{{AFX_MSG(CServerSettingsDlg)
	afx_msg void OnBUTTONLoadDatabase();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAddServer();
	afx_msg void OnBUTTONDeleteServer();
	afx_msg void OnBUTTONLoadFromFile();
	afx_msg void OnBUTTONSaveToFile();
	afx_msg void OnSelchangeLISTAvailableServers();
	afx_msg void OnBUTTONAddPatch();
	afx_msg void OnBUTTONDeletePatch();
	afx_msg void OnBUTTONSetVersion();
	afx_msg void OnSelchangeLISTPatchDatabase();
	afx_msg void OnBUTTONSaveDatabase();
	afx_msg void OnBUTTONLoadAccounts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSETTINGSDLG_H__0D35D5F4_5EC3_46F2_A5A1_1876124DE690__INCLUDED_)

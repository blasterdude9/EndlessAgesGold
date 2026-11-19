#include "afxwin.h"
#if !defined(AFX_ADVANCEDNETWORKDLG_H__BBBAE6C1_8EFB_11D3_B7BC_0000B4B0CE0A__INCLUDED_)
#define AFX_ADVANCEDNETWORKDLG_H__BBBAE6C1_8EFB_11D3_B7BC_0000B4B0CE0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdvancedNetworkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdvancedNetworkDlg dialog

class AFX_EXT_CLASS   CAdvancedNetworkDlg : public CDialog
{
// Construction
public:
	CAdvancedNetworkDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL RecordNetworkData;
	BOOL ClientSideFireing;
	BOOL NoRenderDedicated;
	BOOL persistentServerType;
	BOOL requiresPassword;
	BOOL aiServer;
	BOOL enableAutobackup;
// Dialog Data
	//{{AFX_DATA(CAdvancedNetworkDlg)
	enum { IDD = IDD_DIALOG_AdvancedNetwork };
	CButton	m_enableAutoBackup;
	CButton	m_aiSever;
	CButton	m_checkRequirePassword;
	CButton	m_persistentServer;
	CButton	m_noRenderDedicated;
	CButton	m_clientSideFireing;
	CButton	m_recordNetworkData;
	float	m_calcEntityRadius;
	float	m_calcItemRadius;
	int		m_maxLoggedOnPlayers;
	int		m_availableBandwidth;
	int		m_frequencyAdjustBuffer;
	int		m_frequencyIncrement;
	int		m_frequencyLimit;
	CString	m_serverKey;
	CString	m_serverName;
	long	m_deadEntityDissolveTime;
	long	m_aiServerCycle;
	long	m_deadEntityNPCDissolveTime;
	CString	m_autoBackupFile;
	long	m_durationBetweenBackups;
	int		m_versionNumber;
	int		m_murdererLevel;
	CString	m_sqlIP;
	CString	m_sqlPassword;
	CString	m_sqlUsername;
	CString	m_sqlTableName;
	CString	m_sqlCatalogDB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdvancedNetworkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdvancedNetworkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCHECKRecordNetworkdata();
	afx_msg void OnCHECKClientSideFireing();
	afx_msg void OnCHECKNoRenderDedicated();
	afx_msg void OnCHECKPersistentServer();
	afx_msg void OnCHECKRequirePassword();
	afx_msg void OnCheckAiserver();
	afx_msg void OnBUTTONBrowseAutoBackupFile();
	afx_msg void OnCHECKenableAutobackup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	long m_murderWorkOffRate;
	int m_levelRestriction;
	afx_msg void OnBnClickedOk();
	int m_maxPacketIntake;
	int m_maxPacketOutput;
	CEdit m_houseVisRange;
	int m_houseVisRangeValue;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADVANCEDNETWORKDLG_H__BBBAE6C1_8EFB_11D3_B7BC_0000B4B0CE0A__INCLUDED_)

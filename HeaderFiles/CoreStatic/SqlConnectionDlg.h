#pragma once
#include "afxwin.h"
#include "resource.h"
#include "axencryptionlib.h"

// CSqlConnectionDlg dialog

class CSqlConnectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSqlConnectionDlg)

	
public:
	CSqlConnectionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSqlConnectionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SQLConnection };

protected:
	CString m_encDefault;
	CObList m_connectionsList;
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
public:
	CComboBox m_connectionDropList;
public:
	CString m_username;
public:
	CString m_password;
public:
	CString m_database;

	CString m_ipAddress;


	void InListConnections(int sel);
public:
	afx_msg void OnCbnSelchangeComboconnectionhistory();
public:
	CString m_connectionIpListText;
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnBnClickedBtnRemoveconnection();
};

class ACConnectionObject : public CObject
{
public:
	CString m_ipAddress;
	CString m_userName;
	CString m_pass;
	CString m_databaseName;
};
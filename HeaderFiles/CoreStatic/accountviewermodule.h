#if !defined(AFX_ACCOUNTVIEWERMODULE_H__FEB49E01_21D8_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_ACCOUNTVIEWERMODULE_H__FEB49E01_21D8_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountViewerModule.h : header file

#include "CMultiplayerClass.h"
#include "CAccountClass.h"
#include "CABFunctionLib.h"
#include "CMovementModel.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CAccountViewerModule dialog

class CAccountViewerModule : public CDialog
{
// Construction
public:
	CAccountViewerModule(CWnd* pParent = NULL);   // standard constructor

	CAccountObject * accountPtr;
	CABFunctionLib cabLib;
	CMovementObjList * m_entityDatabase;
	CMultiplayerObj * m_multiplayerRef;
	CSkillObjectList * m_skillDatabaseREF;
// Dialog Data
	//{{AFX_DATA(CAccountViewerModule)
	enum { IDD = IDD_DIALOG_AccountViewerDlg };
	CButton	m_canSpawn;
	CButton	m_gmType;
	CButton	m_adminPrivlidge;
	CButton	m_suspenAccountCheck;
	CListBox	m_statistics;
	CListBox	m_globalInventory;
	CListBox	m_skillsDB;
	CButton	m_enableAIserver;
	CListBox	m_charCurrentInventory;
	CListBox	m_characterBankInventory;
	CListBox	m_charactersOnAccount;
	CString	m_passwordChange;
	CString	m_passwordChangeVerify;
	int		m_murders;
	int		m_currentHealth;
	CString	m_accountName;
	CString	m_minutesPlayed;
	int		m_onPersonCash;
	int		m_bankCash;
	float	m_wldSpcaceX;
	float	m_wldSpaceY;
	float	m_wldSpaceZ;
	CString	m_userNameEdit;
	float	m_statValueEdit;
	CString	m_suspentionNotes;
	int		m_staticHandle;
	int		m_setWldID;
	float	m_skillCurExp;
	int		m_houseBind;
	int		m_accountNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountViewerModule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListCharactersOnAccount(int sel);
	void InListSkills(int sel);
	void InListPlayerInventory(int sel);
	void InListGlobalInventory(int sel);
	// Generated message map functions
	//{{AFX_MSG(CAccountViewerModule)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLISTCharactersOnAccount();
	afx_msg void OnBTNDeleteChar();
	afx_msg void OnBUTTONapplyPasswordChange();
	afx_msg void OnCHECKenableAISERVER();
	afx_msg void OnBUTTONsetBankCash();
	afx_msg void OnBUTTONsetOnPlayerCash();
	afx_msg void OnBUTTONAddBankItem();
	afx_msg void OnBUTTONAddCharItem();
	afx_msg void OnBUTTONDeleteBankItem();
	afx_msg void OnBUTTONDeleteCharItem();
	afx_msg void OnBUTTONSetPosition();
	afx_msg void OnBUTTONApplyNameChange();
	afx_msg void OnBUTTONExportChar();
	afx_msg void OnBUTTONImportChar();
	afx_msg void OnBUTTONSetStatValue();
	virtual void OnOK();
	afx_msg void OnBUTTONsetData();
	afx_msg void OnSelchangeLISTskills();
	afx_msg void OnBUTTONSetSkillValue();
	afx_msg void OnCHECKadminPrivilidge();
	afx_msg void OnBUTTONarmInven();
	afx_msg void OnBUTTONdisarmInven();
	afx_msg void OnCHECKGMType();
	afx_msg void OnCHECKCanSpawn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_acGroupList;
	afx_msg void OnBnClickedChecksuspendaccount();
	CString m_lastLoggedOn;
	afx_msg void OnBnClickedButtonDeletealltype();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTVIEWERMODULE_H__FEB49E01_21D8_11D5_B1EE_0000B4BD56DD__INCLUDED_)

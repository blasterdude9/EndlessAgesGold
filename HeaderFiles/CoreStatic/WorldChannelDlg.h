#pragma once

#include "CWorldChannelRules.h"
#include "afxwin.h"
// CWorldChannelDlg dialog

class AFX_EXT_CLASS   CWorldChannelDlg : public CDialog
{
	DECLARE_DYNAMIC(CWorldChannelDlg)

public:
	CWorldChannelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWorldChannelDlg();

	CWorldChannelRuleList * channelDatabaseRef;
// Dialog Data
	enum { IDD = IDD_COMBO_RuleChannel };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
    void InList(int sel);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddrule();
	afx_msg void OnBnClickedButtonDeleterule();
	afx_msg void OnBnClickedButtonReplacerule();
	CComboBox m_channelRulesCombo;
	int m_worldID;
	CListBox m_currentRulesList;
	afx_msg void OnLbnSelchangeListCurrentrulelist();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnLoad();
};

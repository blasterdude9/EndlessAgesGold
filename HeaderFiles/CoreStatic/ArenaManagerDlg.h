#pragma once


// CArenaManagerDlg dialog
#include "CBattleSchedulerClass.h"
#include "afxwin.h"

class AFX_EXT_CLASS   CArenaManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CArenaManagerDlg)

public:
	CArenaManagerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CArenaManagerDlg();


	CBattleSchedulerList * m_battleSysListRef;
// Dialog Data
	enum { IDD = IDD_DIALOG_ArenaManager };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
	void Inlist(int sel);
	void InListSpawnPoints(int sel);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonReplace();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnLbnSelchangeListBattlelist();
	afx_msg void OnBnClickedButtonAddspawnpoint();
	afx_msg void OnBnClickedButtonReplacespawnpoint();
	afx_msg void OnBnClickedButtonDeletespawnpoint();
	afx_msg void OnLbnSelchangeListSpawnpoints();
	CListBox m_battlesList;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonSave();
	CString m_arenaName;
	CComboBox m_arenaType;
	long m_battleMinimum;
	long m_battleMaximum;
	long m_battleDuration;
	int m_levelRangeMax;
	int m_levelRangeMin;
	int m_visualWorldIndex;
	int m_worldChannel;
	CListBox m_spawnPoints;
	float m_spawnPointX;
	float m_spawnPointY;
	float m_spawnPointZ;
};

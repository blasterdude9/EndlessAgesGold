#if !defined(AFX_AIRAIDDBCLASS_H__36F67211_3CB1_4F0C_A82A_36667E918E59__INCLUDED_)
#define AFX_AIRAIDDBCLASS_H__36F67211_3CB1_4F0C_A82A_36667E918E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AIRaidDbClass.h : header file
//
#include "CAIRaidClass.h"
/////////////////////////////////////////////////////////////////////////////
// CAIRaidDbClass dialog

class AFX_EXT_CLASS CAIRaidDbClass : public CDialog
{
// Construction
public:
	CAIRaidDbClass(CWnd* pParent = NULL);   // standard constructor

	CAIRaidObjList * m_aiRaidRefList;
// Dialog Data
	//{{AFX_DATA(CAIRaidDbClass)
	enum { IDD = IDD_DIALOG_AIRaidDlg };
	CListBox	m_raidList;
	CListBox	m_spawnPointList;
	CListBox	m_aiCfgList;
	int		m_aiCfg;
	float	m_bonusPoints;
	long	m_manditoryWait;
	float	m_notificationPosX;
	float	m_notificationPosY;
	float	m_notificationPosZ;
	float	m_notificationRadius;
	int		m_notificationWldID;
	float	m_posX;
	float	m_posY;
	float	m_posZ;
	long	m_raidDuration;
	CString	m_raidName;
	CString	m_raidWarningString;
	int		m_worldID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAIRaidDbClass)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListRaids(int sel);
    void InListSpawnPoints(int sel);
    void InListAiCfOptions(int sel);

	// Generated message map functions
	//{{AFX_MSG(CAIRaidDbClass)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAddRaid();
	afx_msg void OnBUTTONAddSpawnPoint();
	afx_msg void OnBUTTONDeleteRaid();
	afx_msg void OnBTNSave();
	afx_msg void OnBTNLoadDB();
	afx_msg void OnBUTTONAddCfgOption();
	afx_msg void OnSelchangeLISTRaidDB();
	afx_msg void OnSelchangeLISTSpawnPointDB();
	afx_msg void OnSelchangeLISTAIcfgDatabase();
	afx_msg void OnBUTTONReplaceRaid();
	afx_msg void OnBUTTONDeleteSpawnPoint();
	afx_msg void OnBUTTONReplaceSpawnPoint();
	afx_msg void OnBUTTONDelCfgOption();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_raidPercent;
	afx_msg void OnBnClickedButtonLoadunitai();
	afx_msg void OnBnClickedButtonSaveunitai();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AIRAIDDBCLASS_H__36F67211_3CB1_4F0C_A82A_36667E918E59__INCLUDED_)

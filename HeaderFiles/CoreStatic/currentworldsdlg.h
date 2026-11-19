#if !defined(AFX_CURRENTWORLDSDLG_H__2D910461_1D52_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_CURRENTWORLDSDLG_H__2D910461_1D52_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurrentWorldsDlg.h : header file
//
#include "CMultiplayerClass.h"
/////////////////////////////////////////////////////////////////////////////
// CCurrentWorldsDlg dialog

class CCurrentWorldsDlg : public CDialog
{
// Construction
public:
	CCurrentWorldsDlg(CWnd* pParent = NULL);   // standard constructor
    CMultiplayerObj * m_multiplayerObject;
// Dialog Data
	//{{AFX_DATA(CCurrentWorldsDlg)
	enum { IDD = IDD_DIALOG_CurrentWorlds };
	CListBox	m_spawnPointCfgList;
	CListBox	m_worldList;
	CString	m_worldName;
	float	m_positionX;
	float	m_positionY;
	float	m_positionZ;
	CString	m_spawnPointCfgName;
	int		m_wldId;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurrentWorldsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListCurrentWorlds(int sel);
	void InListCurrentSpawnCfg(int sel);
	CString ConvToString(int number);
	// Generated message map functions
	//{{AFX_MSG(CCurrentWorldsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONBrowseFromFile();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONReplace();
	afx_msg void OnBUTTONAdd();
	afx_msg void OnBUTTONAddSpawnPointCfg();
	afx_msg void OnBUTTONDeleteSpawnPointCfg();
	afx_msg void OnBUTTONReplaceSpawnPointCfg();
	afx_msg void OnSelchangeLISTSpawnPointCfgList();
	afx_msg void OnSelchangeLISTWorldList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnAddworldc();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURRENTWORLDSDLG_H__2D910461_1D52_11D5_B1EE_0000B4BD56DD__INCLUDED_)

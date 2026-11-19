#if !defined(AFX_STORMDLG_H__D40F43C2_00D2_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_STORMDLG_H__D40F43C2_00D2_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StormDlg.h : header file
//
#include "CStormClass.h"
#include "CTextureDBClass.h"
/////////////////////////////////////////////////////////////////////////////
// CStormDlg dialog

class AFX_EXT_CLASS   CStormDlg : public CDialog
{
// Construction
public:
	CStormDlg(CWnd* pParent = NULL);   // standard constructor
    CStormObj           * m_stormRef;
	CTextureDBObjList   * m_textureDataBaseRef;
// Dialog Data
	//{{AFX_DATA(CStormDlg)
	enum { IDD = IDD_DIALOG_Storm };
	CListBox	m_strikeVariationsList;
	long	m_duration;
	long	m_frequency;
	int		m_particleLink;
	float	m_stormRadius;
	float	m_strikeDepth;
	long	m_strikeDuration;
	int		m_strikesPerMinute;
	float	m_strikeSpawnHieght;
	float	m_strikeWidth;
	long	m_transitionTime;
	CString	m_stormName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStormDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListStrikeVariations(int sel);
	// Generated message map functions
	//{{AFX_MSG(CStormDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONADDStrike();
	virtual void OnOK();
	afx_msg void OnBUTTONDeleteStrike();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STORMDLG_H__D40F43C2_00D2_11D5_B1EE_0000B4BD56DD__INCLUDED_)

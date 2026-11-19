#if !defined(AFX_TIMESYSTEMDLG_H__D40F43C1_00D2_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_TIMESYSTEMDLG_H__D40F43C1_00D2_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimeSystemDlg.h : header file
//
#include "CTextureDBClass.h"
#include "CEnvironmentClass.h"
/////////////////////////////////////////////////////////////////////////////
// CTimeSystemDlg dialog

class AFX_EXT_CLASS   CTimeSystemDlg : public CDialog
{
// Construction
public:
	CTimeSystemDlg(CWnd* pParent = NULL);   // standard constructor

	CEnvironmentObj   * m_enviormentObjRef;
	CTextureDBObjList * m_textureDataBaseRef;
	IDirectSound8     * m_directSoundRef;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
// Dialog Data
	//{{AFX_DATA(CTimeSystemDlg)
	enum { IDD = IDD_DIALOG_TIMESYSTEM };
	CListBox	m_stormSystemsList;
	CListBox	m_dayStatesList;
	int		m_daysPerMonth;
	int		m_monthsPerYear;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimeSystemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListDayStates(int sel);
    void InListStorms(int sel);
	// Generated message map functions
	//{{AFX_MSG(CTimeSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdddaystate();
	afx_msg void OnBUTTONAddStorm();
	afx_msg void OnBUTTONEditStorm();
	virtual void OnOK();
	afx_msg void OnBUTTONDeleteStorm();
	afx_msg void OnButtonEditdaystate();
	afx_msg void OnButtonDeletedaystate();
	afx_msg void OnBUTTONAddVisual();
	afx_msg void OnBUTTONAccessCharona();
	afx_msg void OnBUTTONRemoveVisual();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMESYSTEMDLG_H__D40F43C1_00D2_11D5_B1EE_0000B4BD56DD__INCLUDED_)

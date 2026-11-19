#if !defined(AFX_SAFEZONEDLG_H__F5715081_591F_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_SAFEZONEDLG_H__F5715081_591F_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SafeZoneDlg.h : header file
//
#include "CSafeZones.h"
/////////////////////////////////////////////////////////////////////////////
// CSafeZoneDlg dialog

class AFX_EXT_CLASS   CSafeZoneDlg : public CDialog
{
// Construction
public:
	CSafeZoneDlg(CWnd* pParent = NULL);   // standard constructor

    CSafeZoneObjList * m_safeZoneDBREF;
// Dialog Data
	//{{AFX_DATA(CSafeZoneDlg)
	enum { IDD = IDD_DIALOG_SAFEZONEDLG };
	CButton	m_allowNPCconfrontationOnly;
	CListBox	m_safeZoneDB;
	float	m_popoutBoxSizeMaxX;
	float	m_popoutBoxSizeMaxY;
	float	m_popoutBoxSizeMaxZ;
	float	m_popoutBoxSizeMinX;
	float	m_popoutBoxSizeMinY;
	float	m_popoutBoxSizeMinZ;
	int		m_worldID;
	CString	m_zoneName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSafeZoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    BOOL InListSafeZones(int sel);
	// Generated message map functions
	//{{AFX_MSG(CSafeZoneDlg)
	afx_msg void OnBUTTONAddZone();
	afx_msg void OnBUTTONDeleteZone();
	afx_msg void OnButtonLoad();
	afx_msg void OnBUTTONReplaceZone();
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLISTSafeZoneDB();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAFEZONEDLG_H__F5715081_591F_11D5_B1EE_0000B4BD56DD__INCLUDED_)

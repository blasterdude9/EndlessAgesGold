#if !defined(AFX_WATERZONEDBDLG_H__71700A3D_4B0C_48D3_93B0_563FE9C064B6__INCLUDED_)
#define AFX_WATERZONEDBDLG_H__71700A3D_4B0C_48D3_93B0_563FE9C064B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaterZoneDBDLG.h : header file
//
#include "CWaterZoneClass.h"
/////////////////////////////////////////////////////////////////////////////
// CWaterZoneDBDLG dialog


#ifndef _FAFX
class AFX_EXT_CLASS CWaterZoneDBDLG : public CDialog
#endif
#ifdef _FAFX
class CWaterZoneDBDLG : public CDialog
#endif
{
// Construction
public:
	CWaterZoneDBDLG(CWnd* pParent = NULL);   // standard constructor

	CWaterZoneObjList * m_waterZnDBREF;
	CWaterZoneObjList ** m_waterZnDBREF_memloc;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
// Dialog Data
	//{{AFX_DATA(CWaterZoneDBDLG)
	enum { IDD = IDD_DIALOG_WaterZoneDB };
	CListBox	m_waterZoneDB;
	CButton	m_Chk_IsExclusionZn;
	float	m_maxX;
	float	m_maxY;
	float	m_maxZ;
	float	m_minX;
	float	m_minY;
	float	m_minZ;
	CString	m_zoneName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaterZoneDBDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListWaterZones(int sel);
	// Generated message map functions
	//{{AFX_MSG(CWaterZoneDBDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONReplace();
	afx_msg void OnBUTTONLoadDB();
	afx_msg void OnButtonLoadfromarw();
	afx_msg void OnBUTTONSaveDB();
	afx_msg void OnSelchangeLISTWaterZoneDB();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAddwaterzone();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WATERZONEDBDLG_H__71700A3D_4B0C_48D3_93B0_563FE9C064B6__INCLUDED_)

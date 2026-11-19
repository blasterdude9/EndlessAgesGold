#if !defined(AFX_PORTALSDLG_H__9CC8E8AA_F81A_48FE_8E31_F0F524B050A1__INCLUDED_)
#define AFX_PORTALSDLG_H__9CC8E8AA_F81A_48FE_8E31_F0F524B050A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortalsDlg.h : header file
//
#include "CPortalSysClass.h"
/////////////////////////////////////////////////////////////////////////////
// CPortalsDlg dialog

class AFX_EXT_CLASS   CPortalsDlg : public CDialog
{
// Construction
public:
	CPortalsDlg(CWnd* pParent = NULL);   // standard constructor

	CPortalSysList * m_portalsDBRef;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
// Dialog Data
	//{{AFX_DATA(CPortalsDlg)
	enum { IDD = IDD_DIALOG_PortalsDlg };
	CListBox	m_portalsDB;
	CString	m_portalName;
	int		m_requiredKeyID;
	int		m_worldID;
	float	m_xPos;
	float	m_yPos;
	float	m_zPos;
	float	m_zoneMaxX;
	float	m_zoneMaxY;
	float	m_zoneMaxZ;
	float	m_zoneMinX;
	float	m_zoneMinY;
	float	m_zoneMinZ;
	int		m_zoneWldID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortalsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InList(int sel);
	// Generated message map functions
	//{{AFX_MSG(CPortalsDlg)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONReplace();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLISTPortalsDB();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	afx_msg void OnBUTTONLoadBoxFromARW();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTALSDLG_H__9CC8E8AA_F81A_48FE_8E31_F0F524B050A1__INCLUDED_)

#if !defined(AFX_MENUCLICKZONEDLG_H__BB93B121_AC1B_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_MENUCLICKZONEDLG_H__BB93B121_AC1B_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuClickZoneDlg.h : header file
//
#include "Cmenu3DClass.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuClickZoneDlg dialog

class AFX_EXT_CLASS   CMenuClickZoneDlg : public CDialog
{
// Construction
public:
	CMenuClickZoneDlg(CWnd* pParent = NULL);   // standard constructor
    CMenu3DObj * menu3DObjRef;
	CCLickZoneObj * clickZoneRef;
// Dialog Data
	//{{AFX_DATA(CMenuClickZoneDlg)
	enum { IDD = IDD_DIALOG_MenuClickZone };
	CButton	m_execOnClosing;
	CButton	m_radioRadiusType;
	CComboBox	m_actions;
	CComboBox	m_visualsCombo;
	float	m_centerX;
	float	m_centerY;
	float	m_centerZ;
	float	m_maxX;
	float	m_maxY;
	float	m_minX;
	float	m_minY;
	int		m_miscInt;
	int		m_miscInt2;
	float	m_radius;
	CString	m_miscString;
	int		m_miscInt3;
	CString	m_staticMiscInt;
	CString	m_staticMiscInt2;
	CString	m_staticMiscString;
	CString	m_staticMiscInt3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuClickZoneDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void TextInterperate(int sel);
	// Generated message map functions
	//{{AFX_MSG(CMenuClickZoneDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCHECKRadiusType();
	virtual void OnOK();
	afx_msg void OnSelchangeCOMBOActions();
	afx_msg void OnCHECKExecOnClosing();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckDropzone();
	CButton m_dropZoneCheck;
	CButton m_defaultZone;
public:
	afx_msg void OnEnChangeEditMenumiscint();
public:
	afx_msg void OnBnClickedCheckDefaultzone();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUCLICKZONEDLG_H__BB93B121_AC1B_11D4_B1EE_0000B4BD56DD__INCLUDED_)

#if !defined(AFX_RUNTIMEENTITYVIEWERDLG_H__416D9DC1_68E5_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_RUNTIMEENTITYVIEWERDLG_H__416D9DC1_68E5_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RuntimeEntityViewerDlg.h : header file
//
#include "CMovementModel.h"

/////////////////////////////////////////////////////////////////////////////
// CRuntimeEntityViewerDlg dialog

class CRuntimeEntityViewerDlg : public CDialog
{
// Construction
public:
	CRuntimeEntityViewerDlg(CWnd* pParent = NULL);   // standard constructor


	CMovementObj * m_runtimeEntityRef;
// Dialog Data
	//{{AFX_DATA(CRuntimeEntityViewerDlg)
	enum { IDD = IDD_DIALOG_RuntimeEntityInfo };
	CButton	m_hasTarget;
	CButton	m_isAI;
	int		m_networkDefinedID;
	int		m_targetNetworkID;
	float	m_worldPositionX;
	float	m_worldPositionY;
	float	m_worldPositionZ;
	int		m_localTraceNumber;
	int		m_targetTraceNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRuntimeEntityViewerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRuntimeEntityViewerDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNTIMEENTITYVIEWERDLG_H__416D9DC1_68E5_11D5_B1EE_0000B4BD56DD__INCLUDED_)

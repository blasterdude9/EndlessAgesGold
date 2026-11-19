#if !defined(AFX_GLOBALADVANCEDDLG_H__2FCBC761_18A9_11D3_BDB1_FC946FAFAD40__INCLUDED_)
#define AFX_GLOBALADVANCEDDLG_H__2FCBC761_18A9_11D3_BDB1_FC946FAFAD40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GlobalAdvancedDlg.h : header file
//
#include "CShaderLibClass.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CGlobalAdvancedDlg dialog

class AFX_EXT_CLASS   CGlobalAdvancedDlg : public CDialog
{
// Construction
public:
	CGlobalAdvancedDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL mouseSelectionMethodOn;
	BOOL w_bufferPreffered;
	BOOL calcEntityCollision;
	CShaderObjList * m_shaderAvailable;
	int m_currentWorldShader;
// Dialog Data
	//{{AFX_DATA(CGlobalAdvancedDlg)
	enum { IDD = IDD_DIALOG_GLOBAL_ADVANCED };
	CButton	m_calcEntityCollision;
	CButton	m_wbufferPreffered;
	CButton	m_mouseSelectonMethod;
	int		m_targetFps;
	int		m_interpolationGlobal;
	int		m_resolutionHieght;
	int		m_resolutionWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGlobalAdvancedDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGlobalAdvancedDlg)
	afx_msg void OnCHECKSELECTIONMethodOn();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckWbuffer();
	afx_msg void OnCHECKCalcEntityCollision();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_globalShaderList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLOBALADVANCEDDLG_H__2FCBC761_18A9_11D3_BDB1_FC946FAFAD40__INCLUDED_)

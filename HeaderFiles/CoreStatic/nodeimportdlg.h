#if !defined(AFX_NODEIMPORTDLG_H__6F6228A1_4021_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_NODEIMPORTDLG_H__6F6228A1_4021_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeImportDlg.h : header file
//
#include "CReferenceLibrary.h"
/////////////////////////////////////////////////////////////////////////////
// CNodeImportDlg dialog

class AFX_EXT_CLASS   CNodeImportDlg : public CDialog
{
// Construction
public:
	CNodeImportDlg(CWnd* pParent = NULL);   // standard constructor

	CReferenceObjList * m_referenceDBREF;
	int  m_selection;
	int *m_selectionArray;
	int  m_selectionCount;

// Dialog Data
	//{{AFX_DATA(CNodeImportDlg)
	enum { IDD = IDD_DIALOG_NodeImport };
	CListBox	m_referenceLibDB;
	float	m_popoutBoxMaxX;
	float	m_popoutBoxMaxY;
	float	m_popoutBoxMaxZ;
	float	m_popoutBoxSizeMinX;
	float	m_popoutBoxSizeMinY;
	float	m_popoutBoxSizeMinZ;
	float	m_moveX;
	float	m_moveY;
	float	m_moveZ;
	float	m_scaleX;
	float	m_scaleY;
	float	m_scaleZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeImportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListReferenceLibrary(int sel);
	// Generated message map functions
	//{{AFX_MSG(CNodeImportDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODEIMPORTDLG_H__6F6228A1_4021_11D5_B1EE_0000B4BD56DD__INCLUDED_)

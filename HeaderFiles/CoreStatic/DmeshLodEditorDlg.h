#if !defined(AFX_DMESHLODEDITORDLG_H__91CCD201_ECA7_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_DMESHLODEDITORDLG_H__91CCD201_ECA7_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DmeshLodEditorDlg.h : header file
//
#include "cdeformablemeshclass.h"
#include "chiarclemeshclass.h"
#include "cabfunctionlib.h"
/////////////////////////////////////////////////////////////////////////////
// CDmeshLodEditorDlg dialog

class AFX_EXT_CLASS  CDmeshLodEditorDlg : public CDialog
{
// Construction
public:
	CDmeshLodEditorDlg(CWnd* pParent = NULL);   // standard constructor
    CDeformableVisList * m_lodChainRef;
	CHiarcleVisList    * m_lodChainRefHiarchy;
	int                  m_selection;
	int                  m_listSelection;

	CABFunctionLib       m_cablib;
// Dialog Data
	//{{AFX_DATA(CDmeshLodEditorDlg)
	enum { IDD = IDD_DIALOG_DmeshEditor };
	CListBox	m_selectionList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDmeshLodEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDmeshLodEditorDlg)
	afx_msg void OnBTNDelete();
	afx_msg void OnBTNReplace();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMESHLODEDITORDLG_H__91CCD201_ECA7_11D4_B1EE_0000B4BD56DD__INCLUDED_)

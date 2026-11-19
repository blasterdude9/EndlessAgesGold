


#if !defined(AFX_TRIGGEREDITORDLG_H__4FB4D621_FFF8_11D3_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_TRIGGEREDITORDLG_H__4FB4D621_FFF8_11D3_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TriggerEditorDlg.h : header file
//
#include "CWldObject.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CTriggerEditorDlg dialog

class AFX_EXT_CLASS   CTriggerEditorDlg : public CDialog
{
// Construction
public:
	CTriggerEditorDlg(CWnd* pParent = NULL);   // standard constructor
    CWldObject * GL_worldObject;
// Dialog Data
	//{{AFX_DATA(CTriggerEditorDlg)
	enum { IDD = IDD_DIALOG_Trigger };
	CListBox	m_triggerDatabase;
	float	m_dataRadius;
	int		m_miscInt;
	CString	m_pullDownActions;
	CString	m_miscTest;
	float	m_miscVectX;
	float	m_miscVectY;
	float	m_miscVectZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTriggerEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    CString ConvertLongToString(int number);
	void InListTriggers();
	// Generated message map functions
	//{{AFX_MSG(CTriggerEditorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAddtrigger();
	afx_msg void OnBUTTONDeleteTRIGGER();
	afx_msg void OnBUTTONReplaceTRIGGER();
	afx_msg void OnBUTTONBROWSEfile();
	afx_msg void OnSelchangeLISTTriggerList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_doOnlyOnce;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIGGEREDITORDLG_H__4FB4D621_FFF8_11D3_B1E8_0000B4BD56DD__INCLUDED_)

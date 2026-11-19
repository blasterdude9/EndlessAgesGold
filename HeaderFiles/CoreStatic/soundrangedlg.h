#include "afxwin.h"
#if !defined(AFX_SOUNDRANGEDLG_H__C9F7C501_312B_11D4_B1E9_0000B4BD56DD__INCLUDED_)
#define AFX_SOUNDRANGEDLG_H__C9F7C501_312B_11D4_B1E9_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SoundRangeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSoundRangeDlg dialog

class AFX_EXT_CLASS   CSoundRangeDlg : public CDialog
{
// Construction
public:
	CSoundRangeDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL m_looping;
	BOOL m_velocityBased;
// Dialog Data
	//{{AFX_DATA(CSoundRangeDlg)
	enum { IDD = IDD_DIALOG_SOUNDRANGE };
	int		m_endRange;
	CString	m_fileName;
	int		m_startRange;
	int		m_id;
	float	m_soundFactor;
	float	m_clipRadius;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSoundRangeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSoundRangeDlg)
	afx_msg void OnButtonBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_loopingCheck;
	CButton m_velocityBasedCheck;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOUNDRANGEDLG_H__C9F7C501_312B_11D4_B1E9_0000B4BD56DD__INCLUDED_)

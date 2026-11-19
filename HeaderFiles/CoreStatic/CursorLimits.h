#if !defined(AFX_CURSORLIMITS_H__2C4F6E81_22F6_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_CURSORLIMITS_H__2C4F6E81_22F6_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CursorLimits.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCursorLimits dialog

class  CCursorLimits : public CDialog
{
// Construction
public:
	CCursorLimits(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCursorLimits)
	enum { IDD = IDD_DIALOG_CursorLimits };
	float	m_xLimit;
	float	m_yLimit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCursorLimits)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCursorLimits)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURSORLIMITS_H__2C4F6E81_22F6_11D5_B1EE_0000B4BD56DD__INCLUDED_)

#if !defined(AFX_GROUPBOUNDINGSETDLG_H__4BA42A01_F065_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_GROUPBOUNDINGSETDLG_H__4BA42A01_F065_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupBoundingSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupBoundingSetDlg dialog

class AFX_EXT_CLASS   CGroupBoundingSetDlg : public CDialog
{
// Construction
public:
	CGroupBoundingSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGroupBoundingSetDlg)
	enum { IDD = IDD_DIALOG_BndBoxGroupSet };
	int		m_group;
	float	m_XnegBump;
	float	m_XposBump;
	float	m_YnegBump;
	float	m_YposBump;
	float	m_ZnegBump;
	float	m_ZposBump;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupBoundingSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupBoundingSetDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPBOUNDINGSETDLG_H__4BA42A01_F065_11D4_B1EE_0000B4BD56DD__INCLUDED_)

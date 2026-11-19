#if !defined(AFX_ADDHUD_H__01A22368_A2C0_11D3_B7BC_0000B4BD56DD__INCLUDED_)
#define AFX_ADDHUD_H__01A22368_A2C0_11D3_B7BC_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddHud.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddHud dialog

class  CAddHud : public CDialog
{
// Construction
public:
	CAddHud(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddHud)
	enum { IDD = IDD_DIALOG_AddNewHud };
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddHud)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddHud)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDHUD_H__01A22368_A2C0_11D3_B7BC_0000B4BD56DD__INCLUDED_)

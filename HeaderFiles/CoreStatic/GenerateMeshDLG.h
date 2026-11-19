#if !defined(AFX_GENERATEMESHDLG_H__36BB3B81_7313_11D4_A9C0_0000B4C06895__INCLUDED_)
#define AFX_GENERATEMESHDLG_H__36BB3B81_7313_11D4_A9C0_0000B4C06895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenerateMeshDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GenerateMeshDLG dialog

class AFX_EXT_CLASS   GenerateMeshDLG : public CDialog
{
// Construction
public:
	GenerateMeshDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GenerateMeshDLG)
	enum { IDD = IDD_DIALOG_GenerateMesh };
	float	m_meshWidth;
	float	m_meshHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GenerateMeshDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GenerateMeshDLG)
	afx_msg void OnBTNGenerateMeshSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERATEMESHDLG_H__36BB3B81_7313_11D4_A9C0_0000B4C06895__INCLUDED_)

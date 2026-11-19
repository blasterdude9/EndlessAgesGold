#if !defined(AFX_COLLISIONRESOLUTIONDLG_H__B7A10EC1_E4DC_11D3_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_COLLISIONRESOLUTIONDLG_H__B7A10EC1_E4DC_11D3_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CollisionResolutionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCollisionResolutionDlg dialog

class AFX_EXT_CLASS   CCollisionResolutionDlg : public CDialog
{
// Construction
public:
	CCollisionResolutionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCollisionResolutionDlg)
	enum { IDD = IDD_DIALOG_CollisionTableSize };
	int		m_XResolution;
	int		m_YResolution;
	int		m_ZResolution;
	int		m_maxfaceResident;
	int		m_levelsDeep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCollisionResolutionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCollisionResolutionDlg)
		afx_msg void OnCHECKAdvancedEditCollisions();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLLISIONRESOLUTIONDLG_H__B7A10EC1_E4DC_11D3_B1E8_0000B4BD56DD__INCLUDED_)

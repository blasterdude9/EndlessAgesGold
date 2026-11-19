#if !defined(AFX_TRANSFORMANIMATIONDLG_H__0E14AD61_873C_11D3_B7BB_0000B4B0CE0A__INCLUDED_)
#define AFX_TRANSFORMANIMATIONDLG_H__0E14AD61_873C_11D3_B7BB_0000B4B0CE0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransformAnimationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransformAnimationDlg dialog

class AFX_EXT_CLASS   CTransformAnimationDlg : public CDialog
{
// Construction
public:
	CTransformAnimationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTransformAnimationDlg)
	enum { IDD = IDD_DIALOG_TransformAnimationDialog };
	float	m_speedTransZ;
	int		m_conditional;
	float	m_speedTransX;
	float	m_speedTransY;
	float	m_speedRoatationX;
	float	m_speedRoatationY;
	float	m_speedRoatationZ;
	float	m_destTransX;
	float	m_destTransY;
	float	m_destTransZ;
	float	m_destRotationY;
	float	m_destRotationZ;
	float	m_destRotationX;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransformAnimationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransformAnimationDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFORMANIMATIONDLG_H__0E14AD61_873C_11D3_B7BB_0000B4B0CE0A__INCLUDED_)

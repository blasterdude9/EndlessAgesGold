#if !defined(AFX_TRANSFORMATIONDLG_H__BC4FA4A1_EFA7_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_TRANSFORMATIONDLG_H__BC4FA4A1_EFA7_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TransformationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTransformationDlg dialog

class AFX_EXT_CLASS   CTransformationDlg : public CDialog
{
// Construction
public:
	CTransformationDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTransformationDlg)
	enum { IDD = IDD_DIALOG_Transformations };
	float	m_rotX;
	float	m_rotY;
	float	m_rotZ;
	float	m_transX;
	float	m_transY;
	float	m_transZ;
	float	m_scaleX;
	float	m_scaleY;
	float	m_scaleZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransformationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTransformationDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFORMATIONDLG_H__BC4FA4A1_EFA7_11D4_B1EE_0000B4BD56DD__INCLUDED_)

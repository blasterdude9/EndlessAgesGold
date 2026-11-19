#if !defined(AFX_PLACEMENTDLG_H__429EBAC0_FD77_11D2_9FE7_E0A74CC10000__INCLUDED_)
#define AFX_PLACEMENTDLG_H__429EBAC0_FD77_11D2_9FE7_E0A74CC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlacementDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlacementDlg dialog

class AFX_EXT_CLASS   CPlacementDlg : public CDialog
{
// Construction
public:
	CPlacementDlg(CWnd* pParent = NULL);   // standard constructor
    int PlacementMethod;
// Dialog Data
	//{{AFX_DATA(CPlacementDlg)
	enum { IDD = IDD_DIALOG_PLACEMENT };
	CButton	m_basedOnUserDefine;
	CButton	m_radioBasedOnEPosition;
	CButton	m_radioBasedOnEMatrix;
	float	m_posX;
	float	m_posY;
	float	m_posZ;
	CString	m_comboEntityType;
	int		m_aiBotIndex;
	//}}AFX_DATA
    int     m_animationOv;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlacementDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlacementDlg)
	afx_msg void OnRADIOBasedOnEMatrix();
	afx_msg void OnRADIOBasedOnEPosition();
	afx_msg void OnRADIOUserDefine();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLACEMENTDLG_H__429EBAC0_FD77_11D2_9FE7_E0A74CC10000__INCLUDED_)


#pragma once
//
#include "CDayStateClass.h"

/////////////////////////////////////////////////////////////////////////////
// CDayStateDlg dialog

class CDayStateDlg : public CDialog
{
// Construction
public:
	CDayStateDlg(CWnd* pParent = NULL);   // standard constructor
    CDayStateObj     * m_dayStateRef;
// Dialog Data
	//{{AFX_DATA(CDayStateDlg)
	enum { IDD = IDD_DIALOG_DayStateDlg };
	double	m_duration;
	float	m_fogEndRange;
	float	m_fogStartRange;
	double	m_sunDistance;
	float	m_sunRotationX;
	float	m_sunRotationY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDayStateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDayStateDlg)
	afx_msg void OnBUTTONAmbientRGB();
	afx_msg void OnBUTTONFogRGB();
	afx_msg void OnBUTTONSunRGB();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


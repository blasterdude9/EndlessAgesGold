#if !defined(AFX_ANIMSETTINGSDLG_H__BA95CB81_CDCB_11D2_BDB1_DAED7BA36D64__INCLUDED_)
#define AFX_ANIMSETTINGSDLG_H__BA95CB81_CDCB_11D2_BDB1_DAED7BA36D64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnimSettingsDlg dialog

class AFX_EXT_CLASS   CAnimSettingsDlg : public CDialog
{
// Construction
public:
	CAnimSettingsDlg(CWnd* pParent = NULL);   // standard constructor
    int selectedLoop;
	BOOL vertexAnimation;
	CString path;
	D3DVECTOR               trans_BasedSpeed;
	D3DVECTOR               trans_Destination;
	D3DVECTOR               rotate_BasedSpeed;
	D3DVECTOR               rotate_Destination;
	int                     transformConditional;
// Dialog Data
	//{{AFX_DATA(CAnimSettingsDlg)
	enum { IDD = IDD_MDLG_ANIMATION_SETTINGS };
	CButton	m_radioVertexType;
	CButton	m_radioTranslationType;
	CString	m_animationName;
	CString	m_startFileName;
	int		m_totalFrames;
	CString	m_comboLoopType;
	float	m_transY;
	float	m_transZ;
	float	m_transX;
	float	m_rotateX;
	float	m_rotateY;
	float	m_rotateZ;
	float	m_scaleX;
	float	m_scaleY;
	float	m_scaleZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimSettingsDlg)
	afx_msg void OnButtonBrowsevertexstfile();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioTranslation();
	afx_msg void OnRadioVertex();
	afx_msg void OnBUTTONTransSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMSETTINGSDLG_H__BA95CB81_CDCB_11D2_BDB1_DAED7BA36D64__INCLUDED_)

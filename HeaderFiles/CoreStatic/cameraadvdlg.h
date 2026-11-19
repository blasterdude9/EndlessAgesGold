#if !defined(AFX_CAMERAADVDLG_H__C81A8961_FE65_11D2_BDB1_908ED5928F44__INCLUDED_)
#define AFX_CAMERAADVDLG_H__C81A8961_FE65_11D2_BDB1_908ED5928F44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CameraAdvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCameraAdvDlg dialog

class AFX_EXT_CLASS   CCameraAdvDlg : public CDialog
{
// Construction
public:
	CCameraAdvDlg(CWnd* pParent = NULL);   // standard constructor
    int cameraTypeInt;
	BOOL floatingUsesCollision;
	BOOL g_filterFirstPersonModeRef;
// Dialog Data
	//{{AFX_DATA(CCameraAdvDlg)
	enum { IDD = IDD_DIALOG_CamerAdvancedSettings };
	CButton	m_checkFloatHasCollision;
	float	m_fixedAfterPosX;
	float	m_fixedAfterPosY;
	float	m_fixedAfterPosZ;
	float	m_floatingMovePwr;
	float	m_fov;
	float	m_clipPlane;
	CString	m_cameraType;
	float	m_zoomInSpeed;
	float	m_zoomInStop;
	float	m_zoomOutSpeed;
	float	m_InitialPosX;
	float	m_InitialPosY;
	float	m_InitialPosZ;
	float	m_floatTargetPosX;
	float	m_floatTargetPosY;
	float	m_floatTargetPosZ;
	float	m_baseHieghtAspect;
	float	m_pitchRotation;
	//}}AFX_DATA
	int m_miscVal1;
	CButton m_filterFirstPersonMode;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraAdvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCameraAdvDlg)
	afx_msg void OnCHECKFloatingHasCollision();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERAADVDLG_H__C81A8961_FE65_11D2_BDB1_908ED5928F44__INCLUDED_)

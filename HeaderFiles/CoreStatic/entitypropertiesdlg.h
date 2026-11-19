#if !defined(AFX_ENTITYPROPERTIESDLG_H__F89644E1_F094_11D3_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_ENTITYPROPERTIESDLG_H__F89644E1_F094_11D3_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityPropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEntityPropertiesDlg dialog

class AFX_EXT_CLASS   CEntityPropertiesDlg : public CDialog
{
// Construction
public:
	CEntityPropertiesDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL firstPersonModel;
	BOOL disableCollision;
// Dialog Data
	//{{AFX_DATA(CEntityPropertiesDlg)
	enum { IDD = IDD_DIALOG_EntityPropertiesExt };
	CButton	m_checkDisableCollision;
	CButton	m_checkFirstPersonModel;
	CButton	m_checkFilterLodBalancing;

	BOOL    m_filterLodBalancing;
	int		m_deathCamIndex;
	int		m_energy;
	int		m_firstPersonCameraIndex;
	int		m_firstPersonModelRef;
	int		m_fullModelRef;
	int		m_hudLink;
	int		m_hudLinkThirdPersonSpecific;
	float	m_meshRadius;
	int		m_modelType;
	float	m_popoutRadius;
	int		m_thirdPersonCameraIndex;
	int		m_mountBoneIndex;
	int		m_team;
	CString	m_entityName;
	int		m_raceID;
	int     m_selectionEffectTest;
	//}}AFX_DATA
    float m_hardPointOverd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityPropertiesDlg)
	afx_msg void OnCheckFirstpersonmodel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCHECKDisableAllCollision();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_svrPhysics;
	afx_msg void OnBnClickedOk();
	int m_collisionType;
	int m_shadowPref;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYPROPERTIESDLG_H__F89644E1_F094_11D3_B1E8_0000B4BD56DD__INCLUDED_)

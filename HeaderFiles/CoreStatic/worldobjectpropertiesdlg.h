#if !defined(AFX_WORLDOBJECTPROPERTIESDLG_H__7721ED81_E93F_11D3_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_WORLDOBJECTPROPERTIESDLG_H__7721ED81_E93F_11D3_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorldObjectPropertiesDlg.h : header file
//
#include "CContainmentClass.h"
#include "CWldObject.h"
#include "ctextureDBClass.h"
#include "CWorldGroupClass.h"
/////////////////////////////////////////////////////////////////////////////
// CWorldObjectPropertiesDlg dialog

  class AFX_EXT_CLASS   CWorldObjectPropertiesDlg : public CDialog
{
// Construction
public:
	CWorldObjectPropertiesDlg(CWnd* pParent = NULL);   // standard constructor
     int attachedToScene;
	 BOOL vicinityBased;
	 BOOL autoRetract;
	 BOOL removeEnvioronment;
	 BOOL showBoundingBox;
     CTextureDBObjList * GL_textureDataBase;
	 CWldObject         * m_wldObjectRef;
	 CWldGroupObjList   * m_worldGroupListRef;
	 LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
// Dialog Data
	//{{AFX_DATA(CWorldObjectPropertiesDlg)
	enum { IDD = IDD_DIALOG_WorldObjProperties };
	CComboBox	m_groupFilterMod;
	CButton	m_renderFilter;
	CButton	m_collisionFilter;
	//CButton	m_renderInMirrors;
	//CEdit	m_cntDispersal;
	//CEdit	m_cntCurrentAmount;
	//CEdit	m_cntMaxAmount;
	//CEdit	m_cntRespawnTime;
	//CEdit	m_cntMiscType;
	//CComboBox	m_containmentbasis;
	CButton	m_showBoundingBox;
	CButton	m_attachedToScene;
	CButton	m_vicinityBased;
	CButton	m_removeEnviorment;
	CButton	m_autoRetract;
	//float	m_meshRadius;
	int		m_objectGroup;
	int		m_otherAttributeType;
	int		m_otherAttributeValue;
	int		m_otherObjectSelect;
	//float	m_popoutRadius;
	int		m_selfAttributeType;
	int		m_selfAttributeValue;
	//int		m_spawnPoint;
	float	m_popoutBoxSizeMaxX;
	float	m_popoutBoxSizeMaxY;
	float	m_popoutBoxSizeMaxZ;
	float	m_popoutBoxSizeMinX;
	float	m_popoutBoxSizeMinY;
	float	m_popoutBoxSizeMinZ;
	float	m_actualBoxSizeMinX;
	float	m_actualBoxSizeMinY;
	float	m_actualBoxSizeMinZ;
	float	m_actualBoxSizeMaxX;
	float	m_actualBoxSizeMaxY;
	float	m_actualBoxSizeMaxZ;
	float	m_multiplier;
	int		m_haloLink;
	int		m_lensflareLink;
	int		m_particlelink;
	float	m_particleVectorX;
	float	m_particleVectorY;
	float	m_particleVectorZ;
	//}}AFX_DATA

	CComboBox m_envAttrib;//
    CComboBox m_surfacePropertiesList;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorldObjectPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWorldObjectPropertiesDlg)
	afx_msg void OnDeltaposSPINGroupSpinner(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckAttachtoscene();
	afx_msg void OnCheckAutoretract();
	afx_msg void OnCHECKRemoveEnvironment();
	afx_msg void OnCheckVicinitybased();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONActualReference();
	afx_msg void OnCHECKShowBoundingBox();
	//afx_msg void OnBUTTONdisableContainment();
	//afx_msg void OnBUTTONEnableContainment();
	//afx_msg void OnChangeEDITContMiscType();
	//afx_msg void OnChangeEDITcntCurrentAmount();
	//afx_msg void OnChangeEDITcntDispersalAmount();
	//afx_msg void OnChangeEDITcntMaxAmount();
	//afx_msg void OnChangeEDITRespawnTime();
	//afx_msg void OnSelchangeCOMBOContainbasis();
	//afx_msg void OnCHECKRenderInMirrors();
	afx_msg void OnCHECKCollisionFilter();
	afx_msg void OnCHECKRenderFilter();
	afx_msg void OnChangeEDITParticleLink();
	afx_msg void OnBUTTONCharonaSystem();
	virtual void OnOK();
	afx_msg void OnBUTTONLoadBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORLDOBJECTPROPERTIESDLG_H__7721ED81_E93F_11D3_B1E8_0000B4BD56DD__INCLUDED_)

#pragma once
// GroupModifierDlg.h : header file
//
#include "ctextureDBClass.h"
#include "CRTLightClass.h"
#include "CCharonaClass.h"
/////////////////////////////////////////////////////////////////////////////
// CGroupModifierDlg dialog

class AFX_EXT_CLASS   CGroupModifierDlg : public CDialog
{
// Construction
public:
	CGroupModifierDlg(CWnd* pParent = NULL);   // standard constructor
    float diffuseRed;
	float diffuseGreen;
	float diffuseBlue;
	float emmisiveRed;
	float emmisiveGreen;
	float emmisiveBlue;
	float specularRed;
	float specularGreen;
	float specularBlue;
	int   blendMode;
	BOOL m_rangeModEnable;
	BOOL m_diffuseEffectEnable;
	BOOL m_applyGlobal;
	BOOL normalModEnable;
	BOOL multipassModEnable;
	BOOL materialModEnable;
	BOOL inipendentNormalType;
	BOOL boundingBoxModifier;
	BOOL hiarchyModifier;
	BOOL wrapModifier;
	BOOL boundingBoxModifierMirror;
	BOOL meshOptimizer;
	BOOL textureDataSwapEnable;
	BOOL filterCollisionModEnable;
	BOOL filterCollisionOn;
	BOOL UnifyVertsByTolerance;
	BOOL particleEmmiterEnable;
	BOOL formatBoxEnable;
	BOOL m_generateTangents;
	BOOL m_renderFilterEnable;
	BOOL m_renderFilterOn;
	int wrapType;
	BOOL m_groupIdModifier;
		LPDIRECT3DDEVICE9 g_pd3dDeviceRef;


    CRTLightObj * m_lightObject;
	CCharonaObj * m_charonaObject;
	BOOL m_enableLightOption;
	BOOL m_enableCharonaOption;

	CTextureDBObjList * GL_textureDataBase;
// Dialog Data
	//{{AFX_DATA(CGroupModifierDlg)
	enum { IDD = IDD_DIALOG_GroupModifier };
	CButton	m_enableParticleEmmitter;
	CButton	m_enableUnifyVerts;
	CButton	m_enableGroupIDchange;
	CButton	m_checkTextureDataSwap;
	CButton	m_checkMeshOptimizer;
	CButton	m_enableBoundingBoxMirror;
	CButton	m_checkWrapModEnable;
	CButton	m_enableHiarcyMod;
	CButton	m_enableBoundingBoxMod;
	CButton	m_checkMultiTextureModEnable;
	CButton	m_checkIndipendentNormalize;
	CButton	m_checkGroupNormalize;
	CButton	m_checkGlobalMaterialModEnable;
	CButton	m_checkEnableNormalMod;
	CButton	m_checkEnableGenerateTangents;
	CString	m_multitextureMethod;
	float	m_groupMultiplier;
	int		m_groupHiarchy;
	CString	m_comboWrapType;
	float	m_bndBoxMaxX;
	float	m_bndBoxMinX;
	float	m_bndBoxMaxY;
	float	m_bndBoxMinY;
	float	m_bndBoxMaxZ;
	float	m_bndBoxMinZ;
	int		m_texturedataSwapIndex1;
	int		m_texturedataSwapIndex2;
	int		m_modGroupID;
	float	m_tolerance;
	int		m_particleDBref;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupModifierDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupModifierDlg)
	afx_msg void OnCHECKEnableNormalsMod();
	afx_msg void OnCHECKGroupNormalize();
	afx_msg void OnCHECKIndipendentNormalize();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCHECKEnableBoxModifier();
	afx_msg void OnCHECKWrapModEnable();
	afx_msg void OnCHECKEnableBoundingBox();
	afx_msg void OnCHECKCollisionFilterOn();
	afx_msg void OnCHECKEnableCollisionFilterMod();
	afx_msg void OnCHECKEnableGroupIDchange();
	afx_msg void OnBUTTONLoadBox();
	afx_msg void OnCHECKenableParticleEmmitter();
	afx_msg void OnBUTTONinitializeLightObject();
	afx_msg void OnBUTTONinitializeCharonaObject();
	afx_msg void OnCHECKenableCharonaObject();
	afx_msg void OnCHECKenableLightObject();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_diffuseVxEffect;
	afx_msg void OnBnClickedCheckenablediffuseeffect();
	afx_msg void OnBnClickedCheckAffectglobal();
	float m_reletiveBoxSize;
	afx_msg void OnBnClickedCheckenablerelativeboxformat();
public:
	afx_msg void OnBnClickedOk();
public:
	float m_rangeMod;
public:
	afx_msg void OnBnClickedCheckrangemodenable();
public:
	afx_msg void OnBnClickedCheckRenderfilter();
public:
	afx_msg void OnBnClickedCheckrenderfilterenable();
};

#include "CGlobalMaterialShaderLib.h"
#include "CEXMeshClass.h"
#include "CTextureDBClass.h"
#include "CShaderLibClass.h"
#include "afxwin.h"
#pragma once
// MaterialDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMaterialDlg dialog

class AFX_EXT_CLASS   CMaterialDlg : public CDialog
{
// Construction
public:
	CMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	CABFunctionLib cablib;
    BOOL  decalOn;
	CTextureDBObjList * GL_textureDataBase;
	BOOL  forceWireFrame;
	BOOL  makeMirror;
	BOOL  doubleSidedRendering;
	CGlobalMaterialShaderList * m_globalShaderRef;
	BOOL                        m_updateLODmatrialChain;

    BOOL  enableTextureMovement;
	BOOL  enableUVSet0,enableUVSet1;

	CShaderObjList           * m_pxShaderSystemRef;
    CShaderObjList           * m_vxShaderSystemRef;


	BOOL applyMaterialToGroup;
	BOOL m_includeBlendMethods;
	int wrapType;

    float m_ambientRed;
    float m_ambientGreen;
	float m_ambientBlue;
	CEXMeshObj * meshRef;
// Dialog Data
	//{{AFX_DATA(CMaterialDlg)
	enum { IDD = IDD_DIALOG_MaterialModify };
	CButton	m_fogOverride;
	CButton	m_checkFogFilter;
	CButton	m_fadeByDistance;
	CComboBox	m_textureBlend6;
	CComboBox	m_textureBlend5;
	CComboBox	m_textureBlend4;
	CComboBox	m_textureBlend3;
	CComboBox	m_textureBlend2;
	CComboBox	m_textureBlend1;
	CComboBox	m_cmboTextureIndex7;
	CComboBox	m_cmboTextureIndex6;
	CComboBox	m_cmboTextureIndex5;
	CComboBox	m_cmboTextureIndex4;
	CComboBox	m_cmboTextureIndex3;
	CComboBox	m_cmboTextureIndex2;
	CComboBox	m_textureBlend0;
	CComboBox	m_cmboTextureIndex1;
	CComboBox	m_cmboTextureIndex0;
	CComboBox	m_translucencyMode;
	CComboBox	m_modulationType;
	CComboBox	m_mirrorOptionLevelList;
	CComboBox	m_mirrorTypeList;
	CButton	m_checkChromeEnable;
	CButton	m_checkDoubleSidedRendering;
	CButton	m_enableUVSet1;
	CButton	m_enableUVSet0;
	CButton	m_textureMovementEnable;
	CButton	m_makeMirror;
	CButton	m_forceWireFrame;
	CButton	m_decalOn;
	float	m_decalBlue;
	float	m_decalGreen;
	float	m_decalRed;
	float	m_emissiveBlue;
	float	m_emissiveGreen;
	float	m_emissiveRed;
	float	m_opacity;
	float	m_power;
	float	m_specularRed;
	float	m_specularGreen;
	float	m_specularBlue;
	float	m_colorBlue;
	float	m_colorGreen;
	float	m_colorRed;
	int		m_texture_0;
	int		m_blendType;
	int		m_texture_1;
	int		m_texturesInUse;
	int		m_multiTextureMode;
	CString	m_wrapType;
	int		m_texture_2;
	int		m_multiTextureMode2;
	int		m_multiTextureMode3;
	float	m_uset0;
	float	m_uset1;
	float	m_uset2;
	float	m_uset3;
	float	m_vset0;
	float	m_vset1;
	float	m_vset2;
	float	m_vset3;
	int		m_modCycleTime;
	float	m_fadeVisibleDistance;
	float	m_fadeMaxDistance;
	int		m_zBias;
	//}}AFX_DATA

    CComboBox m_colorArg1_L0;
	CComboBox m_colorArg1_L1;
	CComboBox m_colorArg1_L2;
	CComboBox m_colorArg1_L3;
	CComboBox m_colorArg1_L4;
	CComboBox m_colorArg1_L5;
	CComboBox m_colorArg1_L6;
	CComboBox m_colorArg1_L7;

	CComboBox m_colorArg2_L0;
	CComboBox m_colorArg2_L1;
	CComboBox m_colorArg2_L2;
	CComboBox m_colorArg2_L3;
	CComboBox m_colorArg2_L4;
	CComboBox m_colorArg2_L5;
	CComboBox m_colorArg2_L6;
	CComboBox m_colorArg2_L7;

	CComboBox m_texCoordOp_L0;
	CComboBox m_texCoordOp_L1;
	CComboBox m_texCoordOp_L2;
	CComboBox m_texCoordOp_L3;
	CComboBox m_texCoordOp_L4;
	CComboBox m_texCoordOp_L5;
	CComboBox m_texCoordOp_L6;
	CComboBox m_texCoordOp_L7;

    CComboBox m_pixelShaderList;

	CComboBox m_vertexShaderList;

	CButton m_enablePerPixelAlphaTest;

	CButton	m_checkIncludeBlendMethods;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaterialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMaterialDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBTNDecalRGB();
	afx_msg void OnBTNEmisiveRGB();
	afx_msg void OnCHECKDecalOn();
	afx_msg void OnBTNmatSpecularRGB();
	afx_msg void OnBTNColorRGB();
	afx_msg void OnCHECKForceWireframe();
	virtual void OnOK();
	afx_msg void OnCHECKMATMakeMirror();
	afx_msg void OnBTNAmbientRGB();
	afx_msg void OnCHECKTextureMovementEnable();
	afx_msg void OnCHECKUVEnableSet0();
	afx_msg void OnCHECKUVEnableSet1();
	afx_msg void OnCHECKMATDoubleSidedRendering();
	afx_msg void OnCHECKChromeEnable();
	afx_msg void OnEditchangeCOMBOModulationType();
	afx_msg void OnChangeEDITModCycleTime();
	afx_msg void OnBUTTONModAmbientTarget();
	afx_msg void OnBUTTONModDiffuseTarget();
	afx_msg void OnBUTTONModEmmisiveTarget();
	afx_msg void OnCHECKapplyMateriaToGroup();
	afx_msg void OnCHECKFadeByDistance();
	afx_msg void OnCHECKFogFilter();
	afx_msg void OnCHECKfogOverride();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_textBlendMethod9;
	int m_chromeUvSet;
	CComboBox m_optionalShaders;
	afx_msg void OnBnClickedCheckenableperpixelalphatest();
public:
	afx_msg void OnBnClickedOk();
};


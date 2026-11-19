// MaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "d3d9.h"
#include "MaterialDlg.h"
#include "ColorDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMaterialDlg dialog


CMaterialDlg::CMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialDlg::IDD, pParent)
	, m_chromeUvSet(0)
{
	//{{AFX_DATA_INIT(CMaterialDlg)
	m_decalBlue = 0.0f;
	m_decalGreen = 0.0f;
	m_decalRed = 0.0f;
	m_emissiveBlue = 0.0f;
	m_emissiveGreen = 0.0f;
	m_emissiveRed = 0.0f;
	m_opacity = 0.0f;
	m_power = 0.0f;
	m_specularRed = 0.0f;
	m_specularGreen = 0.0f;
	m_specularBlue = 0.0f;
	m_colorBlue = 0.0f;
	m_colorGreen = 0.0f;
	m_colorRed = 0.0f;
	m_texture_0 = 0;
	m_blendType = 0;
	m_texture_1 = 0;
	m_texturesInUse = 0;
	m_multiTextureMode = 0;
	m_wrapType = _T("");
	m_texture_2 = 0;
	m_multiTextureMode2 = 0;
	m_multiTextureMode3 = 0;
	m_uset0 = 0.0f;
	m_uset1 = 0.0f;
	m_vset0 = 0.0f;
	m_vset1 = 0.0f;
	m_modCycleTime = 0;
	m_fadeVisibleDistance = 0.0f;
	m_fadeMaxDistance = 0.0f;
	m_zBias = 0;
	//}}AFX_DATA_INIT
	m_pxShaderSystemRef = NULL;
    m_vxShaderSystemRef = NULL;
}


void CMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaterialDlg)
	DDX_Control(pDX, IDC_CHECK_fogOverride, m_fogOverride);
	DDX_Control(pDX, IDC_CHECK_FogFilter, m_checkFogFilter);
	DDX_Control(pDX, IDC_CHECK_FadeByDistance, m_fadeByDistance);
	DDX_Control(pDX, IDC_COMBO_TexBlendMethod6, m_textureBlend6);
	DDX_Control(pDX, IDC_COMBO_TexBlendMethod5, m_textureBlend5);
	DDX_Control(pDX, IDC_COMBO_TexBlendMethod4, m_textureBlend4);
	DDX_Control(pDX, IDC_COMBO_TexBlendMethod3, m_textureBlend3);
	DDX_Control(pDX, IDC_COMBO_TexBlendMethod2, m_textureBlend2);
	DDX_Control(pDX, IDC_COMBO_TexBlendMethod1, m_textureBlend1);
	DDX_Control(pDX, IDC_COMBO_TextureIndex7, m_cmboTextureIndex7);
	DDX_Control(pDX, IDC_COMBO_TextureIndex6, m_cmboTextureIndex6);
	DDX_Control(pDX, IDC_COMBO_TextureIndex5, m_cmboTextureIndex5);
	DDX_Control(pDX, IDC_COMBO_TextureIndex4, m_cmboTextureIndex4);
	DDX_Control(pDX, IDC_COMBO_TextureIndex3, m_cmboTextureIndex3);
	DDX_Control(pDX, IDC_COMBO_TextureIndex2, m_cmboTextureIndex2);
	DDX_Control(pDX, IDC_COMBO_TexBlendMethod0, m_textureBlend0);
	DDX_Control(pDX, IDC_COMBO_TextureIndex1, m_cmboTextureIndex1);
	DDX_Control(pDX, IDC_COMBO_TextureIndex0, m_cmboTextureIndex0);
	DDX_Control(pDX, IDC_COMBO_TranslucencyMode, m_translucencyMode);
	DDX_Control(pDX, IDC_COMBO_ModulationType, m_modulationType);
    DDX_Control(pDX, IDC_COMBO_MirrorTarget, m_mirrorOptionLevelList);
	DDX_Control(pDX, IDC_COMBO_MirrorType, m_mirrorTypeList);
	DDX_Control(pDX, IDC_CHECK_ChromeEnable, m_checkChromeEnable);
	DDX_Control(pDX, IDC_CHECK_MAT_DoubleSidedRendering, m_checkDoubleSidedRendering);
	DDX_Control(pDX, IDC_CHECK_UVEnableSet1, m_enableUVSet1);
	DDX_Control(pDX, IDC_CHECK_UVEnableSet0, m_enableUVSet0);
	DDX_Control(pDX, IDC_CHECK_TextureMovementEnable, m_textureMovementEnable);
	DDX_Control(pDX, IDC_CHECK_MAT_MakeMirror, m_makeMirror);
	DDX_Control(pDX, IDC_CHECK_ForceWireframe, m_forceWireFrame);
	DDX_Control(pDX, IDC_CHECK_DecalOn, m_decalOn);
	DDX_Text(pDX, IDC_EDIT_DecalBlue, m_decalBlue);
	DDX_Text(pDX, IDC_EDIT_DecalGreen, m_decalGreen);
	DDX_Text(pDX, IDC_EDIT_DecalRed, m_decalRed);
	DDX_Text(pDX, IDC_EDIT_EmisiveBlue, m_emissiveBlue);
	DDX_Text(pDX, IDC_EDIT_EmisiveGreen, m_emissiveGreen);
	DDX_Text(pDX, IDC_EDIT_EmisiveRed, m_emissiveRed);
	DDX_Text(pDX, IDC_EDIT_Opacity, m_opacity);
	DDX_Text(pDX, IDC_EDIT_mat_Power, m_power);
	DDX_Text(pDX, IDC_EDIT_SpecularRed, m_specularRed);
	DDX_Text(pDX, IDC_EDIT_SpecularGreen, m_specularGreen);
	DDX_Text(pDX, IDC_EDIT_SpecularBlue, m_specularBlue);
	DDX_Text(pDX, IDC_EDIT_ColorBlue, m_colorBlue);
	DDX_Text(pDX, IDC_EDIT_ColorGreen, m_colorGreen);
	DDX_Text(pDX, IDC_EDIT_ColorRed, m_colorRed);
	DDX_Text(pDX, IDC_EDIT_TEXTURE_0, m_texture_0);
	DDX_Text(pDX, IDC_EDIT_BlendTypr, m_blendType);
	DDX_Text(pDX, IDC_EDIT_TEXTURE_1, m_texture_1);
	DDX_Text(pDX, IDC_EDIT_texturesInUse, m_texturesInUse);
	DDX_Text(pDX, IDC_EDIT_MultitextureMode, m_multiTextureMode);
	DDX_CBString(pDX, IDC_COMBO_WRAPTYPE, m_wrapType);
	DDX_Text(pDX, IDC_EDIT_TEXTURE_2, m_texture_2);
	DDX_Text(pDX, IDC_EDIT_MultitextureMode2, m_multiTextureMode2);
	DDX_Text(pDX, IDC_EDIT_MultitextureMode3, m_multiTextureMode3);
	DDX_Text(pDX, IDC_EDIT_USET0, m_uset0);
	DDX_Text(pDX, IDC_EDIT_USET1, m_uset1);
	DDX_Text(pDX, IDC_EDIT_USET2, m_uset2);
	DDX_Text(pDX, IDC_EDIT_USET3, m_uset3);
	DDX_Text(pDX, IDC_EDIT_VSET0, m_vset0);
	DDX_Text(pDX, IDC_EDIT_VSET1, m_vset1);
	DDX_Text(pDX, IDC_EDIT_VSET2, m_vset2);
	DDX_Text(pDX, IDC_EDIT_VSET3, m_vset3);
	DDX_Text(pDX, IDC_EDIT_ModCycleTime, m_modCycleTime);
	DDX_Text(pDX, IDC_EDIT_visibleDistance, m_fadeVisibleDistance);
	DDX_Text(pDX, IDC_EDIT_MaxDistance, m_fadeMaxDistance);
	DDX_Text(pDX, IDC_EDIT_Zbias, m_zBias);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_TexBlendMetho9, m_textBlendMethod9);
	DDX_Text(pDX, IDC_EDIT_ChromeUvSet, m_chromeUvSet);
	DDX_Control(pDX, IDC_COMBO_optinalShaders, m_optionalShaders);

    DDX_Control(pDX, IDC_COMBO_TextureArg2_L0, m_colorArg2_L0);
	DDX_Control(pDX, IDC_COMBO_TextureArg2_L1, m_colorArg2_L1);
	DDX_Control(pDX, IDC_COMBO_TextureArg2_L2, m_colorArg2_L2);
	DDX_Control(pDX, IDC_COMBO_TextureArg2_L3, m_colorArg2_L3);
	DDX_Control(pDX, IDC_COMBO_TextureArg2_L4, m_colorArg2_L4);
	DDX_Control(pDX, IDC_COMBO_TextureArg2_L5, m_colorArg2_L5);
	DDX_Control(pDX, IDC_COMBO_TextureArg2_L6, m_colorArg2_L6);
	DDX_Control(pDX, IDC_COMBO_TextureArg2_L7, m_colorArg2_L7);

	DDX_Control(pDX, IDC_COMBO_TextureArg1_L0, m_colorArg1_L0);
	DDX_Control(pDX, IDC_COMBO_TextureArg1_L1, m_colorArg1_L1);
	DDX_Control(pDX, IDC_COMBO_TextureArg1_L2, m_colorArg1_L2);
	DDX_Control(pDX, IDC_COMBO_TextureArg1_L3, m_colorArg1_L3);
	DDX_Control(pDX, IDC_COMBO_TextureArg1_L4, m_colorArg1_L4);
	DDX_Control(pDX, IDC_COMBO_TextureArg1_L5, m_colorArg1_L5);
	DDX_Control(pDX, IDC_COMBO_TextureArg1_L6, m_colorArg1_L6);
	DDX_Control(pDX, IDC_COMBO_TextureArg1_L7, m_colorArg1_L7);

	DDX_Control(pDX, IDC_COMBO_TextureCoordOp_L0, m_texCoordOp_L0);
	DDX_Control(pDX, IDC_COMBO_TextureCoordOp_L1, m_texCoordOp_L1);
	DDX_Control(pDX, IDC_COMBO_TextureCoordOp_L2, m_texCoordOp_L2);
	DDX_Control(pDX, IDC_COMBO_TextureCoordOp_L3, m_texCoordOp_L3);
	DDX_Control(pDX, IDC_COMBO_TextureCoordOp_L4, m_texCoordOp_L4);
	DDX_Control(pDX, IDC_COMBO_TextureCoordOp_L5, m_texCoordOp_L5);
	DDX_Control(pDX, IDC_COMBO_TextureCoordOp_L6, m_texCoordOp_L6);
	DDX_Control(pDX, IDC_COMBO_TextureCoordOp_L7, m_texCoordOp_L7);

	DDX_Control(pDX, IDC_COMBO_optionalPxShaders, m_pixelShaderList);

    DDX_Control(pDX, IDC_COMBO_optionalVxShaders, m_vertexShaderList);

	
	DDX_Control(pDX, IDC_CHECK_enablePerPixelAlphaTest, m_enablePerPixelAlphaTest);
	DDX_Control(pDX, IDC_CHECK_includeBlendMethods, m_checkIncludeBlendMethods);

	
	

}


BEGIN_MESSAGE_MAP(CMaterialDlg, CDialog)
	//{{AFX_MSG_MAP(CMaterialDlg)
	ON_BN_CLICKED(IDC_BTN_DecalRGB, OnBTNDecalRGB)
	ON_BN_CLICKED(IDC_BTN_EmisiveRGB, OnBTNEmisiveRGB)
	ON_BN_CLICKED(IDC_CHECK_DecalOn, OnCHECKDecalOn)
	ON_BN_CLICKED(IDC_BTN_mat_SpecularRGB, OnBTNmatSpecularRGB)
	ON_BN_CLICKED(IDC_BTN_ColorRGB, OnBTNColorRGB)
	ON_BN_CLICKED(IDC_CHECK_ForceWireframe, OnCHECKForceWireframe)
	ON_BN_CLICKED(IDC_CHECK_MAT_MakeMirror, OnCHECKMATMakeMirror)
	ON_BN_CLICKED(IDC_BTN_AmbientRGB, OnBTNAmbientRGB)
	ON_BN_CLICKED(IDC_CHECK_TextureMovementEnable, OnCHECKTextureMovementEnable)
	ON_BN_CLICKED(IDC_CHECK_UVEnableSet0, OnCHECKUVEnableSet0)
	ON_BN_CLICKED(IDC_CHECK_UVEnableSet1, OnCHECKUVEnableSet1)
	ON_BN_CLICKED(IDC_CHECK_MAT_DoubleSidedRendering, OnCHECKMATDoubleSidedRendering)
	ON_BN_CLICKED(IDC_CHECK_ChromeEnable, OnCHECKChromeEnable)
	ON_CBN_EDITCHANGE(IDC_COMBO_ModulationType, OnEditchangeCOMBOModulationType)
	ON_EN_CHANGE(IDC_EDIT_ModCycleTime, OnChangeEDITModCycleTime)
	ON_BN_CLICKED(IDC_BUTTON_ModAmbientTarget, OnBUTTONModAmbientTarget)
	ON_BN_CLICKED(IDC_BUTTON_ModDiffuseTarget, OnBUTTONModDiffuseTarget)
	ON_BN_CLICKED(IDC_BUTTON_ModEmmisiveTarget, OnBUTTONModEmmisiveTarget)
	ON_BN_CLICKED(IDC_CHECK_applyMateriaToGroup, OnCHECKapplyMateriaToGroup)
	ON_BN_CLICKED(IDC_CHECK_FadeByDistance, OnCHECKFadeByDistance)
	ON_BN_CLICKED(IDC_CHECK_FogFilter, OnCHECKFogFilter)
	ON_BN_CLICKED(IDC_CHECK_fogOverride, OnCHECKfogOverride)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_enablePerPixelAlphaTest, OnBnClickedCheckenableperpixelalphatest)

	ON_BN_CLICKED(IDOK, &CMaterialDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaterialDlg message handlers

BOOL CMaterialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_includeBlendMethods = 0;
	UpdateData(TRUE);

	m_updateLODmatrialChain = TRUE;
	m_decalOn.SetCheck(1);

	m_optionalShaders.AddString("UNBOUND");
	if(m_globalShaderRef)
	{
      for(POSITION gPos = m_globalShaderRef->GetHeadPosition();gPos != NULL;)
	  {
        CGlobalMaterialShader * mPtr = (CGlobalMaterialShader *)m_globalShaderRef->GetNext(gPos);
        m_optionalShaders.AddString(mPtr->m_shaderName);

	  }
	 m_optionalShaders.SetCurSel(meshRef->List_materialObject->m_optionalShaderIndex);
	}
	else
     m_optionalShaders.SetCurSel(0);

	if (!meshRef)
		return FALSE;
	 if(meshRef->List_materialObject->m_transition != -1)
		m_modulationType.SetCurSel(meshRef->List_materialObject->m_transition);
    
	 applyMaterialToGroup = FALSE;
	 m_modCycleTime       = meshRef->List_materialObject->m_cycleTime;
	 /*m_multiTextureMode   = meshRef->List_BlendMethod[0];
	 m_multiTextureMode2  = meshRef->List_BlendMethod[1];
	 m_multiTextureMode3  = meshRef->List_BlendMethod[2];*/


     m_power              = meshRef->List_materialObject->m_baseMaterial.Power;
	 m_emissiveRed        = meshRef->List_materialObject->m_baseMaterial.Emissive.r;
	 m_emissiveGreen      = meshRef->List_materialObject->m_baseMaterial.Emissive.g;
	 m_emissiveBlue       = meshRef->List_materialObject->m_baseMaterial.Emissive.b;	   
	 m_specularRed        = meshRef->List_materialObject->m_baseMaterial.Specular.r;
	 m_specularGreen      = meshRef->List_materialObject->m_baseMaterial.Specular.g;
	 m_specularBlue       = meshRef->List_materialObject->m_baseMaterial.Specular.b;
	 m_colorRed           = meshRef->List_materialObject->m_baseMaterial.Diffuse.r;
	 m_colorGreen         = meshRef->List_materialObject->m_baseMaterial.Diffuse.g;
	 m_colorBlue          = meshRef->List_materialObject->m_baseMaterial.Diffuse.b;
	 m_ambientRed         = meshRef->List_materialObject->m_baseMaterial.Ambient.r;
	 m_ambientGreen       = meshRef->List_materialObject->m_baseMaterial.Ambient.g;
	 m_ambientBlue        = meshRef->List_materialObject->m_baseMaterial.Ambient.b;
	 /*m_texture_0          = meshRef->List_textureSelect[0];
	 m_texture_1          = meshRef->List_textureSelect[1];
	 m_texture_2          = meshRef->List_textureSelect[2];*/
	 m_uset0              = meshRef->List_materialObject->m_textureMovement[0].tu;
     m_vset0              = meshRef->List_materialObject->m_textureMovement[0].tv;
	 m_uset1              = meshRef->List_materialObject->m_textureMovement[1].tu;
     m_vset1              = meshRef->List_materialObject->m_textureMovement[1].tv;
	 m_uset2              = meshRef->List_materialObject->m_textureMovement[2].tu;
     m_vset2              = meshRef->List_materialObject->m_textureMovement[2].tv;
	 m_uset3              = meshRef->List_materialObject->m_textureMovement[3].tu;
     m_vset3              = meshRef->List_materialObject->m_textureMovement[3].tv;

	 /*m_fadeVisibleDistance = meshRef->List_materialObject->m_fadeVisDist;*/

	 m_fadeVisibleDistance = (float)meshRef->List_materialObject->m_fadeVisDist;
	 //WARNING: C4244 --> conversion from long to float, possible loss of data

	 /*m_fadeMaxDistance    = meshRef->List_materialObject->m_fadeMaxDist;*/

	 m_fadeMaxDistance    = (float)meshRef->List_materialObject->m_fadeMaxDist;
	 //WARNING: C4244 --> conversion from long to float, possible loss of data

	 m_zBias              = meshRef->List_materialObject->m_zBias;
	 m_chromeUvSet        = meshRef->m_chromeUvSet;

	 m_mirrorOptionLevelList.AddString("NONE");
	 m_mirrorOptionLevelList.AddString("Layer 0");
	 m_mirrorOptionLevelList.AddString("Layer 1");
	 m_mirrorOptionLevelList.AddString("Layer 2");
	 m_mirrorOptionLevelList.AddString("Layer 3");
	 m_mirrorOptionLevelList.AddString("Layer 4");
	 m_mirrorOptionLevelList.SetCurSel(meshRef->List_materialObject->m_mirrorEnabled);

	 
	 m_mirrorTypeList.AddString("Portal Mirror");
	 m_mirrorTypeList.AddString("Projection Mirror");
	 m_mirrorTypeList.SetCurSel(meshRef->List_materialObject->m_mirrorProcessType);

     if(GL_textureDataBase){//texture fill
		int trace=0;
		m_cmboTextureIndex0.AddString("NONE");
	    m_cmboTextureIndex1.AddString("NONE");
		m_cmboTextureIndex2.AddString("NONE");
	    m_cmboTextureIndex3.AddString("NONE");
		m_cmboTextureIndex4.AddString("NONE");
	    m_cmboTextureIndex5.AddString("NONE");
		m_cmboTextureIndex6.AddString("NONE");
	    m_cmboTextureIndex7.AddString("NONE");
        for(POSITION posLoc = GL_textureDataBase->GetHeadPosition();posLoc!=NULL;)
		{
			CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetNext(posLoc);
            CString num = operator+(cablib.ConvToString(trace)," ");
			num = operator+(num,texture->List_textureName);
			m_cmboTextureIndex0.AddString(num);
			m_cmboTextureIndex1.AddString(num);
			m_cmboTextureIndex2.AddString(num);
			m_cmboTextureIndex3.AddString(num);
			m_cmboTextureIndex4.AddString(num);
			m_cmboTextureIndex5.AddString(num);
			m_cmboTextureIndex6.AddString(num);
			m_cmboTextureIndex7.AddString(num);
			trace++;
		}
	 }//end texture fill
	 //set focus
	 m_textureBlend0.SetCurSel(meshRef->List_BlendMethod[0]+1);
	 m_textureBlend1.SetCurSel(meshRef->List_BlendMethod[1]+1);
	 m_textureBlend2.SetCurSel(meshRef->List_BlendMethod[2]+1);
	 m_textureBlend3.SetCurSel(meshRef->List_BlendMethod[3]+1);
	 m_textureBlend4.SetCurSel(meshRef->List_BlendMethod[4]+1);
	 m_textureBlend5.SetCurSel(meshRef->List_BlendMethod[5]+1);
	 m_textureBlend6.SetCurSel(meshRef->List_BlendMethod[6]+1);
	 m_textBlendMethod9.SetCurSel(meshRef->List_BlendMethod[9]+1);
	 m_cmboTextureIndex0.SetCurSel(meshRef->List_textureSelect[0]+1);
	 m_cmboTextureIndex1.SetCurSel(meshRef->List_textureSelect[1]+1);
	 m_cmboTextureIndex2.SetCurSel(meshRef->List_textureSelect[2]+1);
	 m_cmboTextureIndex3.SetCurSel(meshRef->List_textureSelect[3]+1);
	 m_cmboTextureIndex4.SetCurSel(meshRef->List_textureSelect[4]+1);
	 m_cmboTextureIndex5.SetCurSel(meshRef->List_textureSelect[5]+1);
	 m_cmboTextureIndex6.SetCurSel(meshRef->List_textureSelect[6]+1);
	 m_cmboTextureIndex7.SetCurSel(meshRef->List_textureSelect[7]+1);

     m_colorArg2_L0.SetCurSel(meshRef->List_materialObject->m_colorArg2[0]);
	 m_colorArg2_L1.SetCurSel(meshRef->List_materialObject->m_colorArg2[1]);
	 m_colorArg2_L2.SetCurSel(meshRef->List_materialObject->m_colorArg2[2]);
	 m_colorArg2_L3.SetCurSel(meshRef->List_materialObject->m_colorArg2[3]);
	 m_colorArg2_L4.SetCurSel(meshRef->List_materialObject->m_colorArg2[4]);
	 m_colorArg2_L5.SetCurSel(meshRef->List_materialObject->m_colorArg2[5]);
	 m_colorArg2_L6.SetCurSel(meshRef->List_materialObject->m_colorArg2[6]);
	 m_colorArg2_L7.SetCurSel(meshRef->List_materialObject->m_colorArg2[7]);


	 m_colorArg1_L0.SetCurSel(meshRef->List_materialObject->m_colorArg1[0]);
	 m_colorArg1_L1.SetCurSel(meshRef->List_materialObject->m_colorArg1[1]);
	 m_colorArg1_L2.SetCurSel(meshRef->List_materialObject->m_colorArg1[2]);
	 m_colorArg1_L3.SetCurSel(meshRef->List_materialObject->m_colorArg1[3]);
	 m_colorArg1_L4.SetCurSel(meshRef->List_materialObject->m_colorArg1[4]);
	 m_colorArg1_L5.SetCurSel(meshRef->List_materialObject->m_colorArg1[5]);
	 m_colorArg1_L6.SetCurSel(meshRef->List_materialObject->m_colorArg1[6]);
	 m_colorArg1_L7.SetCurSel(meshRef->List_materialObject->m_colorArg1[7]);

	 m_texCoordOp_L0.SetCurSel(meshRef->List_materialObject->m_texcoordLevels[0]);
	 m_texCoordOp_L1.SetCurSel(meshRef->List_materialObject->m_texcoordLevels[1]);
	 m_texCoordOp_L2.SetCurSel(meshRef->List_materialObject->m_texcoordLevels[2]);
	 m_texCoordOp_L3.SetCurSel(meshRef->List_materialObject->m_texcoordLevels[3]);
	 m_texCoordOp_L4.SetCurSel(meshRef->List_materialObject->m_texcoordLevels[4]);
	 m_texCoordOp_L5.SetCurSel(meshRef->List_materialObject->m_texcoordLevels[5]);
	 m_texCoordOp_L6.SetCurSel(meshRef->List_materialObject->m_texcoordLevels[6]);
	 m_texCoordOp_L7.SetCurSel(meshRef->List_materialObject->m_texcoordLevels[7]);
	 

	   
     m_translucencyMode.SetCurSel((meshRef->List_materialObject->m_blendMode+1));

	 if(meshRef->List_materialObject->m_forceWireFrame == TRUE)
	    m_forceWireFrame.SetCheck(TRUE);

     if(meshRef->List_materialObject->m_textureTileType == 0)
        m_wrapType = "WRAP";
	 if(meshRef->List_materialObject->m_textureTileType == 1)
        m_wrapType = "MIRROR";
	 if(meshRef->List_materialObject->m_textureTileType == 2)
        m_wrapType = "CLAMP";

	 

	 /*if(meshRef->List_materialObject->m_mirrorEnabled == TRUE)
        m_makeMirror.SetCheck(1);
	 else
        m_makeMirror.SetCheck(0);*/


	 if(meshRef->List_materialObject->m_enableTextureMovement == TRUE)
		m_textureMovementEnable.SetCheck(1);
	 if(meshRef->List_materialObject->m_enableSet0 == TRUE)
		m_enableUVSet0.SetCheck(1);
	 if(meshRef->List_materialObject->m_enableSet1 == TRUE)
		m_enableUVSet1.SetCheck(1);
	 if(meshRef->List_materialObject->m_cullOn == FALSE)
		m_checkDoubleSidedRendering.SetCheck(1);

	 if(meshRef->m_chromeWrap == TRUE)
        m_checkChromeEnable.SetCheck(1);

  
	 if(meshRef->List_materialObject->m_fadeByDistance)
	    m_fadeByDistance.SetCheck(1);
	 else
	    m_fadeByDistance.SetCheck(0);

    
	 if(meshRef->List_materialObject->m_fogOverride)
	    m_fogOverride.SetCheck(1);
	 else
	    m_fogOverride.SetCheck(0);

	 if(meshRef->List_materialObject->m_doAlphaDepthTest)
	    m_enablePerPixelAlphaTest.SetCheck(1);
	 else
	    m_enablePerPixelAlphaTest.SetCheck(0);

	 

    
	 if(meshRef->List_materialObject->m_fogFilter)
        m_checkFogFilter.SetCheck(1);


	 m_pixelShaderList.AddString("NONE");
	 if(m_pxShaderSystemRef)
     for(POSITION posLoc = m_pxShaderSystemRef->GetHeadPosition();posLoc!=NULL;)
	 {
         CShaderObj * shaderPtr = (CShaderObj *)m_pxShaderSystemRef->GetNext(posLoc);
		 m_pixelShaderList.AddString(shaderPtr->m_shaderName);
	 }
	 m_pixelShaderList.SetCurSel(meshRef->List_materialObject->m_pixelShader+1);


     m_vertexShaderList.AddString("NONE");
	 if(m_vxShaderSystemRef)
     for(POSITION posLoc = m_vxShaderSystemRef->GetHeadPosition();posLoc!=NULL;)
	 {
         CShaderObj * shaderPtr = (CShaderObj *)m_vxShaderSystemRef->GetNext(posLoc);
		 m_vertexShaderList.AddString(shaderPtr->m_shaderName);
	 }
	 m_vertexShaderList.SetCurSel(meshRef->List_materialObject->m_vertexShader+1);
	 

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMaterialDlg::OnBTNDecalRGB() 
{
UpdateData(TRUE);
CColorDlg dlg;
dlg.m_red = m_decalRed;
dlg.m_green = m_decalGreen;
dlg.m_blue = m_decalBlue;

dlg.DoModal();
m_decalRed = dlg.m_red;
m_decalGreen = dlg.m_green; 
m_decalBlue = dlg.m_blue;
UpdateData(FALSE);
}

void CMaterialDlg::OnBTNEmisiveRGB() 
{
UpdateData(TRUE);
CColorDlg dlg;
dlg.m_red   = meshRef->List_materialObject->m_baseMaterial.Emissive.r;
dlg.m_green = meshRef->List_materialObject->m_baseMaterial.Emissive.g; 
dlg.m_blue  = meshRef->List_materialObject->m_baseMaterial.Emissive.b;
dlg.m_alpha = meshRef->List_materialObject->m_baseMaterial.Emissive.a;
dlg.DoModal();
meshRef->List_materialObject->m_baseMaterial.Emissive.r = dlg.m_red;
meshRef->List_materialObject->m_baseMaterial.Emissive.g = dlg.m_green; 
meshRef->List_materialObject->m_baseMaterial.Emissive.b = dlg.m_blue;
meshRef->List_materialObject->m_baseMaterial.Emissive.a = dlg.m_alpha;
UpdateData(FALSE);		
}

void CMaterialDlg::OnCHECKDecalOn() 
{
	if(m_updateLODmatrialChain)
	   m_updateLODmatrialChain = FALSE;
	else
	   m_updateLODmatrialChain = TRUE;
}

void CMaterialDlg::OnBTNmatSpecularRGB() 
{
UpdateData(TRUE);
CColorDlg dlg;
dlg.m_red   = meshRef->List_materialObject->m_baseMaterial.Specular.r;
dlg.m_green = meshRef->List_materialObject->m_baseMaterial.Specular.g; 
dlg.m_blue  = meshRef->List_materialObject->m_baseMaterial.Specular.b;
dlg.m_alpha = meshRef->List_materialObject->m_baseMaterial.Specular.a;
dlg.DoModal();
meshRef->List_materialObject->m_baseMaterial.Specular.r = dlg.m_red;
meshRef->List_materialObject->m_baseMaterial.Specular.g = dlg.m_green; 
meshRef->List_materialObject->m_baseMaterial.Specular.b = dlg.m_blue;
meshRef->List_materialObject->m_baseMaterial.Specular.a = dlg.m_alpha;
UpdateData(FALSE);		
}

void CMaterialDlg::OnBTNColorRGB() 
{
UpdateData(TRUE);
CColorDlg dlg;
dlg.m_red   = meshRef->List_materialObject->m_baseMaterial.Diffuse.r;
dlg.m_green = meshRef->List_materialObject->m_baseMaterial.Diffuse.g; 
dlg.m_blue  = meshRef->List_materialObject->m_baseMaterial.Diffuse.b;
dlg.m_alpha = meshRef->List_materialObject->m_baseMaterial.Diffuse.a;
dlg.DoModal();
meshRef->List_materialObject->m_baseMaterial.Diffuse.r = dlg.m_red;
meshRef->List_materialObject->m_baseMaterial.Diffuse.g = dlg.m_green; 
meshRef->List_materialObject->m_baseMaterial.Diffuse.b = dlg.m_blue;
meshRef->List_materialObject->m_baseMaterial.Diffuse.a = dlg.m_alpha;
UpdateData(FALSE);		
}

void CMaterialDlg::OnCHECKForceWireframe() 
{
    if(meshRef->List_materialObject->m_forceWireFrame == TRUE){
       meshRef->List_materialObject->m_forceWireFrame = FALSE;
	}
	else{
       meshRef->List_materialObject->m_forceWireFrame = TRUE;
	}
}

void CMaterialDlg::OnOK() 
{
	UpdateData(TRUE);

    meshRef->List_materialObject->m_mirrorEnabled = m_mirrorOptionLevelList.GetCurSel();

	meshRef->List_materialObject->m_mirrorProcessType = m_mirrorTypeList.GetCurSel();
     
    m_includeBlendMethods = m_checkIncludeBlendMethods.GetCheck();

	if(m_wrapType == "WRAP")
	meshRef->List_materialObject->m_textureTileType = 0;
    if(m_wrapType == "MIRROR")
	meshRef->List_materialObject->m_textureTileType = 1;
	if(m_wrapType == "CLAMP")
	meshRef->List_materialObject->m_textureTileType = 2;
	  if(GL_textureDataBase){
		  
         meshRef->List_BlendMethod[0]   = m_textureBlend0.GetCurSel()-1;
		 meshRef->List_BlendMethod[1]   = m_textureBlend1.GetCurSel()-1;
		 meshRef->List_BlendMethod[2]   = m_textureBlend2.GetCurSel()-1;
		 meshRef->List_BlendMethod[3]   = m_textureBlend3.GetCurSel()-1;
		 meshRef->List_BlendMethod[4]   = m_textureBlend4.GetCurSel()-1;
		 meshRef->List_BlendMethod[5]   = m_textureBlend5.GetCurSel()-1;
		 meshRef->List_BlendMethod[6]   = m_textureBlend6.GetCurSel()-1;
         meshRef->List_BlendMethod[9]   = m_textBlendMethod9.GetCurSel()-1;


         meshRef->List_materialObject->m_colorArg2[0] = m_colorArg2_L0.GetCurSel();
		 meshRef->List_materialObject->m_colorArg2[1] = m_colorArg2_L1.GetCurSel();
		 meshRef->List_materialObject->m_colorArg2[2] = m_colorArg2_L2.GetCurSel();
		 meshRef->List_materialObject->m_colorArg2[3] = m_colorArg2_L3.GetCurSel();
		 meshRef->List_materialObject->m_colorArg2[4] = m_colorArg2_L4.GetCurSel();
		 meshRef->List_materialObject->m_colorArg2[5] = m_colorArg2_L5.GetCurSel();
		 meshRef->List_materialObject->m_colorArg2[6] = m_colorArg2_L6.GetCurSel();
		 meshRef->List_materialObject->m_colorArg2[7] = m_colorArg2_L7.GetCurSel();

		 meshRef->List_materialObject->m_colorArg1[0] = m_colorArg1_L0.GetCurSel();
		 meshRef->List_materialObject->m_colorArg1[1] = m_colorArg1_L1.GetCurSel();
		 meshRef->List_materialObject->m_colorArg1[2] = m_colorArg1_L2.GetCurSel();
		 meshRef->List_materialObject->m_colorArg1[3] = m_colorArg1_L3.GetCurSel();
		 meshRef->List_materialObject->m_colorArg1[4] = m_colorArg1_L4.GetCurSel();
		 meshRef->List_materialObject->m_colorArg1[5] = m_colorArg1_L5.GetCurSel();
		 meshRef->List_materialObject->m_colorArg1[6] = m_colorArg1_L6.GetCurSel();
		 meshRef->List_materialObject->m_colorArg1[7] = m_colorArg1_L7.GetCurSel();

		 meshRef->List_materialObject->m_texcoordLevels[0] = m_texCoordOp_L0.GetCurSel();
		 meshRef->List_materialObject->m_texcoordLevels[1] = m_texCoordOp_L1.GetCurSel();
		 meshRef->List_materialObject->m_texcoordLevels[2] = m_texCoordOp_L2.GetCurSel();
		 meshRef->List_materialObject->m_texcoordLevels[3] = m_texCoordOp_L3.GetCurSel();
		 meshRef->List_materialObject->m_texcoordLevels[4] = m_texCoordOp_L4.GetCurSel();
		 meshRef->List_materialObject->m_texcoordLevels[5] = m_texCoordOp_L5.GetCurSel();
		 meshRef->List_materialObject->m_texcoordLevels[6] = m_texCoordOp_L6.GetCurSel();
		 meshRef->List_materialObject->m_texcoordLevels[7] = m_texCoordOp_L7.GetCurSel();

	     meshRef->List_textureSelect[0] = m_cmboTextureIndex0.GetCurSel()-1;
		 POSITION texPos = GL_textureDataBase->FindIndex(meshRef->List_textureSelect[0]);
		 if(texPos){
            CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetAt(texPos);
			meshRef->List_storedTextureFromFile[0] = texture->List_textureName;
		 }
   	     meshRef->List_textureSelect[1] = m_cmboTextureIndex1.GetCurSel()-1;
		 texPos = GL_textureDataBase->FindIndex(meshRef->List_textureSelect[1]);
		 if(texPos){
            CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetAt(texPos);
			meshRef->List_storedTextureFromFile[1] = texture->List_textureName;
		 }
		 meshRef->List_textureSelect[2] = m_cmboTextureIndex2.GetCurSel()-1;
		 texPos = GL_textureDataBase->FindIndex(meshRef->List_textureSelect[2]);
		 if(texPos){
            CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetAt(texPos);
			meshRef->List_storedTextureFromFile[2] = texture->List_textureName;
		 }
		 meshRef->List_textureSelect[3] = m_cmboTextureIndex3.GetCurSel()-1;
		 texPos = GL_textureDataBase->FindIndex(meshRef->List_textureSelect[3]);
		 if(texPos){
            CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetAt(texPos);
			meshRef->List_storedTextureFromFile[3] = texture->List_textureName;
		 }
		 meshRef->List_textureSelect[4] = m_cmboTextureIndex4.GetCurSel()-1;
		 texPos = GL_textureDataBase->FindIndex(meshRef->List_textureSelect[4]);
		 if(texPos){
            CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetAt(texPos);
			meshRef->List_storedTextureFromFile[4] = texture->List_textureName;
		 }
		 meshRef->List_textureSelect[5] = m_cmboTextureIndex5.GetCurSel()-1;
		 texPos = GL_textureDataBase->FindIndex(meshRef->List_textureSelect[5]);
		 if(texPos){
            CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetAt(texPos);
			meshRef->List_storedTextureFromFile[5] = texture->List_textureName;
		 }
		 meshRef->List_textureSelect[6] = m_cmboTextureIndex6.GetCurSel()-1;
		 texPos = GL_textureDataBase->FindIndex(meshRef->List_textureSelect[6]);
		 if(texPos){
            CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetAt(texPos);
			meshRef->List_storedTextureFromFile[6] = texture->List_textureName;
		 }
		 meshRef->List_textureSelect[7] = m_cmboTextureIndex7.GetCurSel()-1;
		 texPos = GL_textureDataBase->FindIndex(meshRef->List_textureSelect[7]);
		 if(texPos){
            CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetAt(texPos);
			meshRef->List_storedTextureFromFile[7] = texture->List_textureName;
		 }
	  }


    meshRef->List_materialObject->m_baseMaterial.Power       = m_power;
	
	meshRef->List_materialObject->m_textureMovement[0].tu      = m_uset0;
    meshRef->List_materialObject->m_textureMovement[0].tv      = m_vset0;
	meshRef->List_materialObject->m_textureMovement[1].tu      = m_uset1;
    meshRef->List_materialObject->m_textureMovement[1].tv      = m_vset1;
	meshRef->List_materialObject->m_textureMovement[2].tu      = m_uset2;
    meshRef->List_materialObject->m_textureMovement[2].tv      = m_vset2;
	meshRef->List_materialObject->m_textureMovement[3].tu      = m_uset3;
    meshRef->List_materialObject->m_textureMovement[3].tv      = m_vset3;
	meshRef->List_materialObject->m_zBias                      = m_zBias;

	/*meshRef->List_materialObject->m_fadeVisDist = m_fadeVisibleDistance;
	meshRef->List_materialObject->m_fadeMaxDist = m_fadeMaxDistance;*/

	meshRef->List_materialObject->m_fadeVisDist = (long)m_fadeVisibleDistance;
	meshRef->List_materialObject->m_fadeMaxDist = (long)m_fadeMaxDistance;
	//WARNING: C4244 --> conversion from float to long, possible loss of data

	meshRef->m_chromeUvSet = m_chromeUvSet;

	int blendModeOp = m_translucencyMode.GetCurSel();
	if(blendModeOp > 0)
	meshRef->List_materialObject->m_baseBlendMode = blendModeOp-1;
	else
	meshRef->List_materialObject->m_baseBlendMode = -1;
  
	int selection = m_modulationType.GetCurSel();
	if(selection != -1)
	   meshRef->List_materialObject->m_transition = m_modulationType.GetCurSel();
	else
	   meshRef->List_materialObject->m_transition = 0;

    meshRef->List_materialObject->m_zBias = m_zBias;

	meshRef->List_materialObject->m_optionalShaderIndex = m_optionalShaders.GetCurSel();

    if(m_pixelShaderList.GetCurSel() > -1)
	   meshRef->List_materialObject->m_pixelShader = m_pixelShaderList.GetCurSel()-1;

	if(m_vertexShaderList.GetCurSel() > -1)
	   meshRef->List_materialObject->m_vertexShader = m_vertexShaderList.GetCurSel()-1;


	
	UpdateData(FALSE);
    meshRef->List_materialObject->Reset();

	CDialog::OnOK();
}

void CMaterialDlg::OnCHECKMATMakeMirror() 
{
  if(makeMirror == TRUE){
       makeMirror = FALSE;
	}
	else{
       makeMirror = TRUE;
	}
}

void CMaterialDlg::OnBTNAmbientRGB() 
{
UpdateData(TRUE);
CColorDlg dlg;
dlg.m_red   = meshRef->List_materialObject->m_baseMaterial.Ambient.r;
dlg.m_green = meshRef->List_materialObject->m_baseMaterial.Ambient.g; 
dlg.m_blue  = meshRef->List_materialObject->m_baseMaterial.Ambient.b;
dlg.m_alpha = meshRef->List_materialObject->m_baseMaterial.Ambient.a;
dlg.DoModal();
meshRef->List_materialObject->m_baseMaterial.Ambient.r = dlg.m_red;
meshRef->List_materialObject->m_baseMaterial.Ambient.g = dlg.m_green; 
meshRef->List_materialObject->m_baseMaterial.Ambient.b = dlg.m_blue;
meshRef->List_materialObject->m_baseMaterial.Ambient.a = dlg.m_alpha;
UpdateData(FALSE);		
}

void CMaterialDlg::OnCHECKTextureMovementEnable() 
{
        if(meshRef->List_materialObject->m_enableTextureMovement == TRUE)
		   meshRef->List_materialObject->m_enableTextureMovement = FALSE;
		else
		   meshRef->List_materialObject->m_enableTextureMovement = TRUE;
}

void CMaterialDlg::OnCHECKUVEnableSet0() 
{
	if(meshRef->List_materialObject->m_enableSet0 == TRUE)
	   meshRef->List_materialObject->m_enableSet0 = FALSE;
    else
	   meshRef->List_materialObject->m_enableSet0 = TRUE;
	
}

void CMaterialDlg::OnCHECKUVEnableSet1() 
{
	if(meshRef->List_materialObject->m_enableSet1 == TRUE)
	   meshRef->List_materialObject->m_enableSet1 = FALSE;
    else
	   meshRef->List_materialObject->m_enableSet1 = TRUE;
}

void CMaterialDlg::OnCHECKMATDoubleSidedRendering() 
{
	
	if(meshRef->List_materialObject->m_cullOn == TRUE)
	   meshRef->List_materialObject->m_cullOn = FALSE;
    else
	   meshRef->List_materialObject->m_cullOn = TRUE;
	
}

void CMaterialDlg::OnCHECKChromeEnable() 
{
	if(meshRef->m_chromeWrap == TRUE)
	   meshRef->m_chromeWrap = FALSE;
    else
	   meshRef->m_chromeWrap = TRUE;	
}

void CMaterialDlg::OnEditchangeCOMBOModulationType() 
{
	UpdateData(TRUE);
	int selection = m_modulationType.GetCurSel();
	if(selection == -1)
	   return;
	meshRef->List_materialObject->m_transition = m_modulationType.GetCurSel();

	UpdateData(FALSE);
}

void CMaterialDlg::OnChangeEDITModCycleTime() 
{
	UpdateData(TRUE);
	meshRef->List_materialObject->m_cycleTime = m_modCycleTime;
	UpdateData(FALSE);
}

void CMaterialDlg::OnBUTTONModAmbientTarget() 
{
CColorDlg dlg;
dlg.m_red = meshRef->List_materialObject->m_targetModMaterial.Ambient.r;
dlg.m_green = meshRef->List_materialObject->m_targetModMaterial.Ambient.g;
dlg.m_blue = meshRef->List_materialObject->m_targetModMaterial.Ambient.b;
dlg.DoModal();
meshRef->List_materialObject->m_targetModMaterial.Ambient.r = dlg.m_red;
meshRef->List_materialObject->m_targetModMaterial.Ambient.g = dlg.m_green; 
meshRef->List_materialObject->m_targetModMaterial.Ambient.b = dlg.m_blue;	
}

void CMaterialDlg::OnBUTTONModDiffuseTarget() 
{
CColorDlg dlg;
dlg.m_red   = meshRef->List_materialObject->m_targetModMaterial.Diffuse.r;
dlg.m_green = meshRef->List_materialObject->m_targetModMaterial.Diffuse.g;
dlg.m_blue  = meshRef->List_materialObject->m_targetModMaterial.Diffuse.b;
dlg.DoModal();
meshRef->List_materialObject->m_targetModMaterial.Diffuse.r = dlg.m_red;
meshRef->List_materialObject->m_targetModMaterial.Diffuse.g = dlg.m_green; 
meshRef->List_materialObject->m_targetModMaterial.Diffuse.b = dlg.m_blue;
}

void CMaterialDlg::OnBUTTONModEmmisiveTarget() 
{
CColorDlg dlg;
dlg.m_red = meshRef->List_materialObject->m_targetModMaterial.Emissive.r;
dlg.m_green = meshRef->List_materialObject->m_targetModMaterial.Emissive.g;
dlg.m_blue = meshRef->List_materialObject->m_targetModMaterial.Emissive.b;
dlg.DoModal();
meshRef->List_materialObject->m_targetModMaterial.Emissive.r = dlg.m_red;
meshRef->List_materialObject->m_targetModMaterial.Emissive.g = dlg.m_green; 
meshRef->List_materialObject->m_targetModMaterial.Emissive.b = dlg.m_blue;
}

void CMaterialDlg::OnCHECKapplyMateriaToGroup() 
{
	if(applyMaterialToGroup)
	   applyMaterialToGroup = FALSE;
	else
	   applyMaterialToGroup = TRUE;
	
}

void CMaterialDlg::OnCHECKFadeByDistance() 
{
	if(meshRef->List_materialObject->m_fadeByDistance)
	   meshRef->List_materialObject->m_fadeByDistance = FALSE;
	else
	   meshRef->List_materialObject->m_fadeByDistance = TRUE;
}

void CMaterialDlg::OnCHECKFogFilter() 
{
	if(meshRef->List_materialObject->m_fogFilter)
	   meshRef->List_materialObject->m_fogFilter = FALSE;
	else
	   meshRef->List_materialObject->m_fogFilter = TRUE;
	
}

void CMaterialDlg::OnCHECKfogOverride() 
{
    if(meshRef->List_materialObject->m_fogOverride)
	   meshRef->List_materialObject->m_fogOverride = FALSE;
	else
	   meshRef->List_materialObject->m_fogOverride = TRUE;
}

void CMaterialDlg::OnBnClickedCheckenableperpixelalphatest()
{
    if(meshRef->List_materialObject->m_doAlphaDepthTest)
	   meshRef->List_materialObject->m_doAlphaDepthTest = FALSE;
	else
	   meshRef->List_materialObject->m_doAlphaDepthTest = TRUE;
}


void CMaterialDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

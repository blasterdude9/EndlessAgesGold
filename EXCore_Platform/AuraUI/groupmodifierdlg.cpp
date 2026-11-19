// GroupModifierDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "GroupModifierDlg.h"
#include "ColorDlg.h"
#include "direct.h"
#include "CEXMeshClass.h"
#include "LightInterfaceDlg.h"
#include "CharonaDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupModifierDlg dialog


CGroupModifierDlg::CGroupModifierDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupModifierDlg::IDD, pParent)
	, m_diffuseVxEffect(0)
	, m_reletiveBoxSize(0)
	, m_rangeMod(0)
{
	//{{AFX_DATA_INIT(CGroupModifierDlg)
	m_multitextureMethod = _T("");
	m_groupMultiplier = 0.0f;
	m_groupHiarchy = 0;
	m_comboWrapType = _T("");
	m_bndBoxMaxX = 0.0f;
	m_bndBoxMinX = 0.0f;
	m_bndBoxMaxY = 0.0f;
	m_bndBoxMinY = 0.0f;
	m_bndBoxMaxZ = 0.0f;
	m_bndBoxMinZ = 0.0f;
	m_texturedataSwapIndex1 = 0;
	m_texturedataSwapIndex2 = 0;
	m_modGroupID = 0;
	m_tolerance = 0.0f;
	m_particleDBref = 0;
	//}}AFX_DATA_INIT
}


void CGroupModifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupModifierDlg)
	DDX_Control(pDX, IDC_CHECK_enableParticleEmmitter, m_enableParticleEmmitter);
	DDX_Control(pDX, IDC_CHECK_EnableGroupIDchange, m_enableGroupIDchange);
	DDX_Control(pDX, IDC_CHECK_EnableBoundingBox, m_enableBoundingBoxMirror);
	DDX_Control(pDX, IDC_CHECK_WrapModEnable, m_checkWrapModEnable);
	DDX_Control(pDX, IDC_CHECK_EnableBoxModifier, m_enableBoundingBoxMod);
	DDX_Control(pDX, IDC_CHECK_IndipendentNormalize, m_checkIndipendentNormalize);
	DDX_Control(pDX, IDC_CHECK_GroupNormalize, m_checkGroupNormalize);
	DDX_Control(pDX, IDC_CHECK_EnableNormalsMod, m_checkEnableNormalMod);
	DDX_Text(pDX, IDC_EDIT_GROUP_MULTIPLIER, m_groupMultiplier);
	DDX_CBString(pDX, IDC_COMBO_WRAPMODIFIER, m_comboWrapType);
	DDX_Text(pDX, IDC_EDIT_GROUP_BoxXMax, m_bndBoxMaxX);
	DDX_Text(pDX, IDC_EDIT_GROUP_BoxXmin, m_bndBoxMinX);
	DDX_Text(pDX, IDC_EDIT_GROUP_BoxYMax, m_bndBoxMaxY);
	DDX_Text(pDX, IDC_EDIT_GROUP_BoxYMin, m_bndBoxMinY);
	DDX_Text(pDX, IDC_EDIT_GROUP_BoxZMax, m_bndBoxMaxZ);
	DDX_Text(pDX, IDC_EDIT_GROUP_BoxZMin, m_bndBoxMinZ);
	DDX_Text(pDX, IDC_EDIT_Mod_groupID, m_modGroupID);
	DDX_Text(pDX, IDC_EDIT_particleDBRef, m_particleDBref);	
	//}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_CHECK_GenerateTangentsCheck, m_checkEnableGenerateTangents);
	
}


BEGIN_MESSAGE_MAP(CGroupModifierDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupModifierDlg)
	ON_BN_CLICKED(IDC_CHECK_EnableNormalsMod, OnCHECKEnableNormalsMod)
	ON_BN_CLICKED(IDC_CHECK_GroupNormalize, OnCHECKGroupNormalize)
	ON_BN_CLICKED(IDC_CHECK_IndipendentNormalize, OnCHECKIndipendentNormalize)
	ON_BN_CLICKED(IDC_CHECK_EnableBoxModifier, OnCHECKEnableBoxModifier)
	ON_BN_CLICKED(IDC_CHECK_WrapModEnable, OnCHECKWrapModEnable)
	ON_BN_CLICKED(IDC_CHECK_EnableBoundingBox, OnCHECKEnableBoundingBox)
	ON_BN_CLICKED(IDC_CHECK_CollisionFilterOn, OnCHECKCollisionFilterOn)
	ON_BN_CLICKED(IDC_CHECK_EnableCollisionFilterMod, OnCHECKEnableCollisionFilterMod)
	ON_BN_CLICKED(IDC_CHECK_EnableGroupIDchange, OnCHECKEnableGroupIDchange)
	ON_BN_CLICKED(IDC_BUTTON_LoadBox, OnBUTTONLoadBox)
	ON_BN_CLICKED(IDC_CHECK_enableParticleEmmitter, OnCHECKenableParticleEmmitter)
	ON_BN_CLICKED(IDC_BUTTON_initializeLightObject, OnBUTTONinitializeLightObject)
	ON_BN_CLICKED(IDC_BUTTON_initializeCharonaObject, OnBUTTONinitializeCharonaObject)
	ON_BN_CLICKED(IDC_CHECK_enableCharonaObject, OnCHECKenableCharonaObject)
	ON_BN_CLICKED(IDC_CHECK_enableLightObject, OnCHECKenableLightObject)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_AffectGlobal, OnBnClickedCheckAffectglobal)
	ON_BN_CLICKED(IDOK, &CGroupModifierDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_RenderFilter, &CGroupModifierDlg::OnBnClickedCheckRenderfilter)
	ON_BN_CLICKED(IDC_CHECK_renderFilterEnable, &CGroupModifierDlg::OnBnClickedCheckrenderfilterenable)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupModifierDlg message handlers

void CGroupModifierDlg::OnCHECKEnableNormalsMod() 
{
	if(normalModEnable == TRUE){
	   normalModEnable = FALSE;
       m_checkEnableNormalMod.SetCheck(0);
	}
	else{
       normalModEnable = TRUE;
       m_checkEnableNormalMod.SetCheck(1);
	}
}


void CGroupModifierDlg::OnCHECKGroupNormalize() 
{
	//if(inipendentNormalType == TRUE){
	   inipendentNormalType = FALSE;
       m_checkGroupNormalize.SetCheck(1);
       m_checkIndipendentNormalize.SetCheck(0);
	//}
}

void CGroupModifierDlg::OnCHECKIndipendentNormalize() 
{
	
    //if(inipendentNormalType == FALSE){
	   inipendentNormalType = TRUE;
       m_checkGroupNormalize.SetCheck(0);
       m_checkIndipendentNormalize.SetCheck(1);
	//}
	
}



void CGroupModifierDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if("NONE" == m_multitextureMethod)
       blendMode = -1;
	if("MODULATED" == m_multitextureMethod)
       blendMode = 0;
	if("ADD" == m_multitextureMethod)
       blendMode = 1;
	if("SUBTRACT" == m_multitextureMethod)
       blendMode = 2;
	if("ADD SMOOTH" == m_multitextureMethod)
       blendMode = 3;
	if("ADD SIGNED" == m_multitextureMethod)
       blendMode = 4;
    if("MODULATED X2" == m_multitextureMethod)
       blendMode = 5;
	if("BUMPMAP" == m_multitextureMethod)
       blendMode = 6;

    if(m_comboWrapType == "WRAP")
    wrapType = 0;
    else
    wrapType = 1;


	

    UpdateData(FALSE);
	CDialog::OnOK();
}

BOOL CGroupModifierDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	 UpdateData(TRUE);
     m_checkIndipendentNormalize.SetCheck(1);
	 m_rangeModEnable = FALSE;
	 inipendentNormalType = TRUE;
	 m_generateTangents = FALSE;
	 formatBoxEnable = FALSE;
	 boundingBoxModifier = FALSE;
	 hiarchyModifier = FALSE;
	 normalModEnable = FALSE;
	 multipassModEnable = FALSE;
	 materialModEnable = FALSE;
	 inipendentNormalType = FALSE;
	 boundingBoxModifier = FALSE;
	 hiarchyModifier = FALSE;
	 wrapModifier = FALSE;
	 meshOptimizer = FALSE;
	 boundingBoxModifierMirror = FALSE;
	 textureDataSwapEnable = FALSE;
	 m_diffuseEffectEnable = FALSE;
	 m_applyGlobal = FALSE;
	 m_renderFilterEnable = FALSE;
	 m_renderFilterOn = FALSE;

     filterCollisionModEnable = FALSE;
     filterCollisionOn = FALSE;
	 m_groupIdModifier = FALSE;
     UnifyVertsByTolerance = FALSE;
	 particleEmmiterEnable = FALSE;

	 m_lightObject = NULL;
	 m_charonaObject = NULL;
	 m_enableLightOption = FALSE;
	 m_enableCharonaOption = FALSE;

	 wrapType = 0;
	 m_comboWrapType = "WRAP";
	 UpdateData(FALSE);
	 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGroupModifierDlg::OnCHECKEnableBoxModifier() 
{
	
    if(boundingBoxModifier == TRUE){
	   boundingBoxModifier = FALSE;
       m_enableBoundingBoxMod.SetCheck(0);
	}
	else{
       boundingBoxModifier = TRUE;
       m_enableBoundingBoxMod.SetCheck(1);
	}
}

void CGroupModifierDlg::OnCHECKWrapModEnable() 
{
	if(wrapModifier == TRUE){
	   wrapModifier = FALSE;
       m_checkWrapModEnable.SetCheck(0);
	}
	else{
       wrapModifier = TRUE;
       m_checkWrapModEnable.SetCheck(1);
	}
	
}

void CGroupModifierDlg::OnCHECKEnableBoundingBox() 
{
    if(boundingBoxModifierMirror == TRUE){
	   boundingBoxModifierMirror = FALSE;
       m_enableBoundingBoxMirror.SetCheck(0);
	}
	else{
       boundingBoxModifierMirror = TRUE;
       m_enableBoundingBoxMirror.SetCheck(1);
	}	
}

void CGroupModifierDlg::OnCHECKCollisionFilterOn() 
{
	if(filterCollisionOn == TRUE)
	   filterCollisionOn = FALSE;
	else
       filterCollisionOn = TRUE;
}

void CGroupModifierDlg::OnCHECKEnableCollisionFilterMod() 
{
	if(filterCollisionModEnable == TRUE)
	   filterCollisionModEnable = FALSE;
	else
       filterCollisionModEnable = TRUE;
}

void CGroupModifierDlg::OnCHECKEnableGroupIDchange() 
{
	if(m_groupIdModifier)
	   m_groupIdModifier = FALSE;
	else
	   m_groupIdModifier = TRUE;
	
}


void CGroupModifierDlg::OnBUTTONLoadBox() 
{
//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
 //end use mapsmodels folder for organization
 static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   UpdateData(FALSE);

       CEXMeshObj * m_mesh = new CEXMeshObj();
	   m_mesh->List_materialObject = new CMaterialObject();
	   float dist = 0.0f;
       if(m_mesh->LoadGeometryFromPlugin(fileName,&dist,g_pd3dDeviceRef,NULL))
	   {//1
          UpdateData(TRUE);
          m_bndBoxMaxX = m_mesh->List_boundingBox.maxX;
          m_bndBoxMinX = m_mesh->List_boundingBox.minX;
          m_bndBoxMaxY = m_mesh->List_boundingBox.maxY;
          m_bndBoxMinY = m_mesh->List_boundingBox.minY;
          m_bndBoxMaxZ = m_mesh->List_boundingBox.maxZ;
          m_bndBoxMinZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

void CGroupModifierDlg::OnCHECKenableParticleEmmitter() 
{
	if(particleEmmiterEnable)
	   particleEmmiterEnable = FALSE;
	else
	   particleEmmiterEnable = TRUE;
}

void CGroupModifierDlg::OnBUTTONinitializeLightObject() 
{
	CLightInterfaceDlg dlg;
    dlg.GL_textureDataBase = GL_textureDataBase;
    dlg.m_lightREF = m_lightObject;
    if(dlg.DoModal()==IDOK)
	   m_lightObject = dlg.m_lightREF;
}

void CGroupModifierDlg::OnBUTTONinitializeCharonaObject() 
{
	CCharonaDlg dlg;

	dlg.m_charonaRef = m_charonaObject;
	dlg.m_textureDataBase = GL_textureDataBase;
	if(dlg.DoModal()==IDOK)
	   m_charonaObject = dlg.m_charonaRef;
	
}

void CGroupModifierDlg::OnCHECKenableCharonaObject() 
{
	if(m_enableCharonaOption)
	   m_enableCharonaOption = FALSE;
	else
	   m_enableCharonaOption = TRUE;
}

void CGroupModifierDlg::OnCHECKenableLightObject() 
{
	if(m_enableLightOption)
	   m_enableLightOption = FALSE;
	else
	   m_enableLightOption = TRUE;
}



void CGroupModifierDlg::OnBnClickedCheckAffectglobal()
{
    if(m_applyGlobal)
	   m_applyGlobal = FALSE;
	else
	   m_applyGlobal = TRUE;
}



void CGroupModifierDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
    m_generateTangents = m_checkEnableGenerateTangents.GetCheck();

 
	UpdateData(FALSE);
	OnOK();
}



void CGroupModifierDlg::OnBnClickedCheckRenderfilter()
{
	if(m_renderFilterOn)
	   m_renderFilterOn = FALSE;
	else
	   m_renderFilterOn = TRUE;
}

void CGroupModifierDlg::OnBnClickedCheckrenderfilterenable()
{
	if(m_renderFilterEnable)
	   m_renderFilterEnable = FALSE;
	else
	   m_renderFilterEnable = TRUE;
}

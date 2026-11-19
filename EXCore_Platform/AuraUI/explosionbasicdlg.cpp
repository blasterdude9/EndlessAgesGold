// ExplosionBasicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "ExplosionBasicDlg.h"
#include "MaterialDlg.h"
#include "SoundSettingsDlg.h"
#include "Direct.h"
#include "LightInterfaceDlg.h"
#include "SkeletalAnimationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExplosionBasicDlg dialog


CExplosionBasicDlg::CExplosionBasicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExplosionBasicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExplosionBasicDlg)
	m_fileName = _T("");
	m_rotationKeyX = 0.0f;
	m_rotationKeyY = 0.0f;
	m_rotationKeyZ = 0.0f;
	m_scaleKeyX = 0.0f;
	m_scaleKeyY = 0.0f;
	m_scaleKeyZ = 0.0f;
	m_keyDuration = 0;
	m_animationDuration = 0;
	m_setFadeAt = 0;
	m_explosionName = _T("");
	m_splashDamage = 0;
	m_damageRadius = 0.0f;
	m_pushPower = 0.0f;
	m_specifiedDamageID = 0;
	//}}AFX_DATA_INIT
}


void CExplosionBasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExplosionBasicDlg)
	DDX_Control(pDX, IDC_COMBO_Elements, m_elementDatabase);
	DDX_Control(pDX, IDC_RADIO_oToNone, m_radioOriNone);
	DDX_Control(pDX, IDC_RADIO_oToCamera, m_radioOriCamera);
	DDX_Control(pDX, IDC_RADIO_oToGround, m_radioOriGround);
	DDX_Control(pDX, IDC_LIST_Keyframes, m_keyframeList);
	DDX_Text(pDX, IDC_EDIT_mFileName, m_fileName);
	DDX_Text(pDX, IDC_EDIT_KeyRotationX, m_rotationKeyX);
	DDX_Text(pDX, IDC_EDIT_KeyRotationY, m_rotationKeyY);
	DDX_Text(pDX, IDC_EDIT_KeyRotationZ, m_rotationKeyZ);
	DDX_Text(pDX, IDC_EDIT_KeyScaleX, m_scaleKeyX);
	DDX_Text(pDX, IDC_EDIT_KeyScaleY, m_scaleKeyY);
	DDX_Text(pDX, IDC_EDIT_KeyScaleZ, m_scaleKeyZ);
	DDX_Text(pDX, IDC_EDIT_KeyDuration, m_keyDuration);
	DDX_Text(pDX, IDC_EDIT_AnimationDuration, m_animationDuration);
	DDX_Text(pDX, IDC_EDIT_SetFadeAt, m_setFadeAt);
	DDX_Text(pDX, IDC_EDIT_explosionName, m_explosionName);
	DDX_Text(pDX, IDC_EDIT_SplashDamage, m_splashDamage);
	DDX_Text(pDX, IDC_EDIT_DamageRadius, m_damageRadius);
	DDX_Text(pDX, IDC_EDIT_pushPower, m_pushPower);
	DDX_Text(pDX, IDC_EDIT_specifiedDamageID, m_specifiedDamageID);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_SkillCriteria, m_skillCriteriaLoc);
}


BEGIN_MESSAGE_MAP(CExplosionBasicDlg, CDialog)
	//{{AFX_MSG_MAP(CExplosionBasicDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddKeyframe, OnBUTTONAddKeyframe)
	ON_BN_CLICKED(IDC_BUTTON_SetMeshImage, OnBUTTONSetMeshImage)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceKeyframe, OnBUTTONReplaceKeyframe)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_DeleteKeyframe, OnBUTTONDeleteKeyframe)
	ON_LBN_SELCHANGE(IDC_LIST_Keyframes, OnSelchangeLISTKeyframes)
	ON_BN_CLICKED(IDC_RADIO_oToCamera, OnRADIOoToCamera)
	ON_BN_CLICKED(IDC_RADIO_oToGround, OnRADIOoToGround)
	ON_BN_CLICKED(IDC_RADIO_oToNone, OnRADIOoToNone)
	ON_BN_CLICKED(IDC_BUTTON_MaterialAccess, OnBUTTONMaterialAccess)
	ON_BN_CLICKED(IDC_BUTTON_Sound, OnBUTTONSound)
	ON_BN_CLICKED(IDC_BUTTON_DeleteLightSys, OnBUTTONDeleteLightSys)
	ON_BN_CLICKED(IDC_BUTTON_LightSys, OnBUTTONLightSys)
	ON_BN_CLICKED(IDC_BUTTON_SkeletalSys, OnBUTTONSkeletalSys)
	ON_BN_CLICKED(IDC_BUTTON_RemoveSkeletalSys, OnBUTTONRemoveSkeletalSys)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExplosionBasicDlg message handlers

BOOL CExplosionBasicDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
     if(m_explosionRef->m_orientation == 1)
		m_radioOriCamera.SetCheck(1);
	 else if(m_explosionRef->m_orientation == 2)
        m_radioOriGround.SetCheck(1);
	 else
        m_radioOriNone.SetCheck(1);


	m_animationDuration      = m_explosionRef->m_duration;
	m_setFadeAt              = m_explosionRef->m_startFadeAt;
	m_explosionName          = m_explosionRef->m_explosionName;
	m_splashDamage           = m_explosionRef->m_radiusDamage;
	m_damageRadius           = m_explosionRef->m_damageRadius; 
	m_pushPower              = m_explosionRef->m_pushPower;
	m_specifiedDamageID      = m_explosionRef->m_damageSpecific;
    POSITION posLoc;
	for(posLoc = m_elementDBREF->GetHeadPosition();posLoc != NULL;)
	{ 
	    CElementObj * elementPtr = (CElementObj *)m_elementDBREF->GetNext(posLoc);
     
        m_elementDatabase.AddString(elementPtr->m_elementName);
	}
	m_elementDatabase.SetCurSel(m_explosionRef->m_damageChannel);

    m_skillCriteriaLoc.AddString("NONE");
	for(posLoc = m_skillDatabaseRef->GetHeadPosition();posLoc != NULL;)
	{ 
	    CSkillObject * skillPtr = (CSkillObject *)m_skillDatabaseRef->GetNext(posLoc);
     
        m_skillCriteriaLoc.AddString(skillPtr->m_skillName);
	}
	m_skillCriteriaLoc.SetCurSel(m_skillDatabaseRef->GetSkillIndexByType(m_explosionRef->m_criteriaForSkillUse)+1);
	
	UpdateData(FALSE);
	InListKeyframes(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void CExplosionBasicDlg::InListKeyframes(int sel)
{
   UpdateData(TRUE);
   m_keyframeList.ResetContent();
   if(!m_explosionRef->m_keyFrames)
       return;
   int trace=0;
   for(POSITION posLoc = m_explosionRef->m_keyFrames->GetHeadPosition();posLoc != NULL;)
   {
       CExpKeyFrameObj * keyObj = (CExpKeyFrameObj *)m_explosionRef->m_keyFrames->GetNext(posLoc);
       m_keyframeList.AddString((operator+("Key -",CABLIB.ConvToString(trace))));
	   trace++;
   }
   if(sel != -1)
   m_keyframeList.SetCurSel(sel);
   UpdateData(FALSE);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void CExplosionBasicDlg::OnBUTTONAddKeyframe() 
{
	UpdateData(TRUE);
	D3DVECTOR scale,rotation;
	rotation.x   = m_rotationKeyX;
	rotation.y   = m_rotationKeyY;
	rotation.z   = m_rotationKeyZ;
	scale.x      = m_scaleKeyX;
	scale.y      = m_scaleKeyY;
	scale.z      = m_scaleKeyZ;
	
	m_explosionRef->AddKeyFrame(scale,rotation,m_keyDuration);
	UpdateData(FALSE);
	InListKeyframes(m_explosionRef->m_keyFrames->GetCount()-1);
}

void CExplosionBasicDlg::OnBUTTONSetMeshImage() 
{
	UpdateData(TRUE);
	m_explosionRef->LoadBasicMesh(m_fileName,g_pd3dDeviceRef);
	UpdateData(FALSE);
}

void CExplosionBasicDlg::OnBUTTONReplaceKeyframe() 
{
	int keyIndex = m_keyframeList.GetCurSel();
	if(keyIndex == -1){
	   MessageBox("Must select a keyframe");	
	   return;
	}

	POSITION expPos = m_explosionRef->m_keyFrames->FindIndex(keyIndex);
	if(!expPos)
		return;
	CExpKeyFrameObj * keyObj = (CExpKeyFrameObj *)m_explosionRef->m_keyFrames->GetAt(expPos);

	UpdateData(TRUE);
	keyObj->m_rotationX    = m_rotationKeyX;
	keyObj->m_rotationY    = m_rotationKeyY;
	keyObj->m_rotationZ    = m_rotationKeyZ;
	keyObj->m_scalerX      = m_scaleKeyX;
	keyObj->m_scalerY      = m_scaleKeyY;
	keyObj->m_scalerZ      = m_scaleKeyZ;
	keyObj->m_duration     = m_keyDuration;

	UpdateData(FALSE);
}

void CExplosionBasicDlg::OnButtonBrowse() 
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
	   m_fileName = opendialog.GetPathName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CExplosionBasicDlg::OnBUTTONDeleteKeyframe() 
{
	UpdateData(TRUE);
	int keyIndex = m_keyframeList.GetCurSel();
	if(keyIndex == -1){
	   MessageBox("Must Select a keyframe");	
	   return;
	}
    
	POSITION expPos = m_explosionRef->m_keyFrames->FindIndex(keyIndex);
	if(!expPos)
		return;
	    CExpKeyFrameObj * keyObj = (CExpKeyFrameObj *)m_explosionRef->m_keyFrames->GetAt(expPos);
        delete keyObj;
		keyObj=0;
		m_explosionRef->m_keyFrames->RemoveAt(expPos);

    UpdateData(FALSE);
	InListKeyframes(keyIndex-1);
}

void CExplosionBasicDlg::OnSelchangeLISTKeyframes() 
{
	int keyIndex = m_keyframeList.GetCurSel();
	if(keyIndex == -1)
		return;

	POSITION expPos = m_explosionRef->m_keyFrames->FindIndex(keyIndex);
	if(!expPos)
		return;
	CExpKeyFrameObj * keyObj = (CExpKeyFrameObj *)m_explosionRef->m_keyFrames->GetAt(expPos);

	UpdateData(TRUE);
	m_rotationKeyX         = keyObj->m_rotationX;
	m_rotationKeyY         = keyObj->m_rotationY;
	m_rotationKeyZ         = keyObj->m_rotationZ;
	m_scaleKeyX            = keyObj->m_scalerX;
	m_scaleKeyY            = keyObj->m_scalerY;
	m_scaleKeyZ            = keyObj->m_scalerZ;
	m_keyDuration          = keyObj->m_duration;

	UpdateData(FALSE);
	
}

void CExplosionBasicDlg::OnOK() 
{
	UpdateData(TRUE);
	m_explosionRef->m_duration      = m_animationDuration;
	m_explosionRef->m_startFadeAt   = m_setFadeAt;
	m_explosionRef->m_explosionName = m_explosionName;
	m_explosionRef->m_radiusDamage  = m_splashDamage;
	m_explosionRef->m_damageRadius  = m_damageRadius; 
	m_explosionRef->m_pushPower     = m_pushPower;
	m_explosionRef->m_damageSpecific= m_specifiedDamageID;

	int damageChannelSelection = m_elementDatabase.GetCurSel();
	if(damageChannelSelection < 0) 
	   damageChannelSelection = 0;
	m_explosionRef->m_damageChannel = damageChannelSelection;

    CSkillObject * skillPtr = m_skillDatabaseRef->GetSkillByIndex(m_skillCriteriaLoc.GetCurSel()-1);
	if(skillPtr)
      m_explosionRef->m_criteriaForSkillUse = skillPtr->m_skillType;
	else
      m_explosionRef->m_criteriaForSkillUse = -1;

	UpdateData(FALSE);
	
	CDialog::OnOK();
}

void CExplosionBasicDlg::OnRADIOoToCamera() 
{
    m_explosionRef->m_orientation = 1;
    m_radioOriCamera.SetCheck(1);
    m_radioOriGround.SetCheck(0);
    m_radioOriNone.SetCheck(0);
}

void CExplosionBasicDlg::OnRADIOoToGround() 
{
	m_explosionRef->m_orientation = 2;
    m_radioOriCamera.SetCheck(0);
    m_radioOriGround.SetCheck(1);
    m_radioOriNone.SetCheck(0);
}

void CExplosionBasicDlg::OnRADIOoToNone() 
{
	m_explosionRef->m_orientation = 0;
    m_radioOriCamera.SetCheck(0);
    m_radioOriGround.SetCheck(0);
    m_radioOriNone.SetCheck(1);
}

void CExplosionBasicDlg::OnBUTTONMaterialAccess() 
{
	if(!m_explosionRef->m_meshObject){
        MessageBox("no valid mesh is set");
        return;
	}
    CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef  = NULL;
	dlg.meshRef            = m_explosionRef->m_meshObject;
	dlg.GL_textureDataBase = GL_textureDataBase;
	//dlg.
	dlg.DoModal();
	
}

void CExplosionBasicDlg::OnBUTTONSound() 
{
	CSoundSettingsDlg dlg;
	  dlg.enabled          = m_explosionRef->m_enableSound; 
	  dlg.m_distanceFactor = m_explosionRef->m_distanceFactor;
	  dlg.m_sndID          = m_explosionRef->m_snID;
	  
	  if(dlg.DoModal()==IDOK){
		  
		  if(dlg.enabled){
			 m_explosionRef->m_enableSound      = dlg.enabled;
             m_explosionRef->m_distanceFactor   = dlg.m_distanceFactor;
			 m_explosionRef->m_snID             = dlg.m_sndID;
		  }
	  }	
}

void CExplosionBasicDlg::OnBUTTONDeleteLightSys() 
{
	if(m_explosionRef->m_light)
	{
	   delete m_explosionRef->m_light;
	   m_explosionRef->m_light = 0;
       AfxMessageBox("Done");
	}
	else
		AfxMessageBox("No Light exists");
	
}

void CExplosionBasicDlg::OnBUTTONLightSys() 
{
	CLightInterfaceDlg dlg;
	         dlg.GL_textureDataBase = GL_textureDataBase;
	         dlg.m_lightREF = m_explosionRef->m_light;
	         if(dlg.DoModal()==IDOK)
		        m_explosionRef->m_light = dlg.m_lightREF;
}

void CExplosionBasicDlg::OnBUTTONSkeletalSys() 
{
	if(!m_explosionRef->m_skeletalObj)
	{//must initialize
            m_explosionRef->m_skeletalObj = new CSkeletonObject();
			m_explosionRef->m_skeletalObj->m_SkeletalName = "EXP";
            //use mapsmodels folder for organization
            CString CurDir;
            CString MainDir;
            char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
            //end use mapsmodels folder for organization
            static char BASED_CODE filter[] = "ARB Files|*.ARB|ARB Files|*.ARB|All Files (*.*)|*.*||";
	        CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
            if ( opendialog.DoModal() == IDOK )
			{ 
	             CString filenamePathandAll = opendialog.GetPathName();
	             m_explosionRef->m_skeletalObj->InitBoneSystem(filenamePathandAll,g_pd3dDeviceRef);
	             //armedPtr->m_firstPersonSystem->OptimizeVertexAssignment();
			}
			else{
	        SetCurrentDirectory(MainDir);
			return;
			}
            SetCurrentDirectory(MainDir);


	}//end must initialize
    CSkeletalAnimationDlg dlg;
	dlg.m_globalShaderRef = NULL;
	    dlg.m_elementDBREF = NULL;
		dlg.m_skillDatabaseREF = NULL;
		dlg.GL_textureDataBase = GL_textureDataBase;
		dlg.m_skeletonReferencePtr = m_explosionRef->m_skeletalObj;
		dlg.directSoundRef = m_dirSndRef;
		dlg.DoModal();
}

void CExplosionBasicDlg::OnBUTTONRemoveSkeletalSys() 
{
	if(!m_explosionRef->m_skeletalObj)
	{//must initialize
       AfxMessageBox("Must delete skeletal system");
       return;
	}//end must initialize
	else{//delete skeletal sys
       m_explosionRef->m_skeletalObj->SafeDelete();
	   delete m_explosionRef->m_skeletalObj;
	   m_explosionRef->m_skeletalObj = 0;
       AfxMessageBox("system terminated");
       return;
	}//end delete skeletal sys
}

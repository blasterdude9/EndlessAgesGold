// SkeletalAnimationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "d3d9.h"
#include "SkeletalAnimationDlg.h"
#include "SoundRangeDlg.h"
#include "direct.h"
#include "MaterialDlg.h"
#include "CharonaDlg.h"
#include "DmeshLodEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkeletalAnimationDlg dialog


CSkeletalAnimationDlg::CSkeletalAnimationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkeletalAnimationDlg::IDD, pParent)
	, m_sndSoundFactor(0)
	, m_clipRadius(0)
	, m_sndSoundID(0)
	, m_maxVelocityDist(0)
	, m_startFreq(0)
{
	m_visibilityTrackStartRange = 0;
	m_visibilityTrackEndRange = 0;
	m_visibilityTrackAnimationID = 0;
	//{{AFX_DATA_INIT(CSkeletalAnimationDlg)
	m_animationType = _T("");
	m_fileName = _T("");
	m_requiredDuration = 0;
	m_animVersion = 0;
	m_damageSphereRadius = 0.0f;
	m_damageShphereAmplification = 0.0f;
	m_sphereCalcRadius = 0.0f;
	m_bdSphereRadius = 0.0f;
	m_dsphereComment = _T("");
	m_dPathRadius = 0.0f;
	m_dPathMinValue = 0;
	m_dPathMaxValue = 0;
	m_dPathDotValue = -1;
	m_dPathAnimationTrace = 0;
	m_ptStartRange = 0;
	m_particleLink = 0;
	m_ptEndRange = 0;
	m_curCfgDefSpawn = 0;
	m_fireQueStartRange = 0;
	m_fireQueEndRange = 0;
	m_fireQueMissileIndex = 0;
	m_fireQueZlocalAdjust = 0.0f;
	m_fireQueXpAdjust = 0.0f;
	m_fireQueYpAdjust = 0.0f;
	m_fps = 0;
	m_bonePosX = 0.0f;
	m_bonePosY = 0.0f;
	m_bonePosZ = 0.0f;
	m_damageSpecific = 0;
	//}}AFX_DATA_INIT
	m_baseAccuracy = 0.0f;
	m_renderStateObjectRef = NULL;
}


void CSkeletalAnimationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkeletalAnimationDlg)
	DDX_Control(pDX, IDC_COMBO_DMPSkillUse, m_dmpSkillTypeUse);
	DDX_Control(pDX, IDC_COMBO_ElementsAvailable, m_elementsAvailable);
	DDX_Control(pDX, IDC_CHECK_secondaryOverridable, m_secondaryOverridable);
    DDX_Control(pDX, IDC_CHECK_HMeshCollisionFilter, m_checkHMeshCollisionFilter);
    DDX_Control(pDX, IDC_CHECK_DMeshCollisionFilter, m_checkDMeshCollisionFilter);
	DDX_Control(pDX, IDC_LIST_FireQueDB, m_fireQueDB);
	DDX_Control(pDX, IDC_LIST_DeformableCfgs, m_deformableCfgs);
	DDX_Control(pDX, IDC_CHECK_UseRange, m_emitPtUseRange);
	DDX_Control(pDX, IDC_LIST_ParticleEvents, m_listParticleEvents);
	DDX_Control(pDX, IDC_LIST_DamagePathTrackList, m_dPathList);
	DDX_Control(pDX, IDC_LIST_BodyColLocationSpheres, m_listBDCSpheres);
	DDX_Control(pDX, IDC_COMBO_CSBoneSel, m_csBoneSel);
	DDX_Control(pDX, IDC_CHECK_EnableSphereCollisionSys, m_checkEnableSphereCalc);
	DDX_Control(pDX, IDC_LIST_LocationSpheres, m_listDamageSpheres);
	DDX_Control(pDX, IDC_LIST_SoundEvents, m_listSoundEvents);
	DDX_Control(pDX, IDC_LIST_HIARCHMESHLIST, m_listHiarchlMeshs);
	DDX_Control(pDX, IDC_LIST_DEFORMABLEMESHLIST, m_listDeformableMeshs);
	DDX_Control(pDX, IDC_LIST_Bones, m_listBones);
	DDX_Control(pDX, IDC_LIST_ANIMATIONS, m_animationsList);
	DDX_Control(pDX, IDC_CHECK_LOOPABLE, m_checkLoopable);
	DDX_CBString(pDX, IDC_COMBO_ANIMTYPE, m_animationType);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_fileName);
	DDX_Text(pDX, IDC_EDIT_requiredDuration, m_requiredDuration);
	DDX_Text(pDX, IDC_EDIT_Version, m_animVersion);
	DDX_Text(pDX, IDC_EDIT_LCSphereRadius, m_damageSphereRadius);
	DDX_Text(pDX, IDC_EDIT_LCSphereDamageAmplification, m_damageShphereAmplification);
	DDX_Text(pDX, IDC_EDIT_sphereCalcRadius, m_sphereCalcRadius);
	DDX_Text(pDX, IDC_EDIT_CSEntityRadius, m_bdSphereRadius);
	DDX_Text(pDX, IDC_EDIT_DSphereComment, m_dsphereComment);
	DDX_Text(pDX, IDC_EDIT_DPathRadius, m_dPathRadius);
	DDX_Text(pDX, IDC_EDIT_DPathDamageValue, m_dPathMinValue);
	DDX_Text(pDX, IDC_EDIT_DPathMaxDamageValue, m_dPathMaxValue);
	DDX_Text(pDX, IDC_EDIT_damageDotRef, m_dPathDotValue);
	DDX_Text(pDX, IDC_EDIT_DPathAnimationTrace, m_dPathAnimationTrace);
	DDX_Text(pDX, IDC_EDIT_ptStartRange, m_ptStartRange);
	DDX_Text(pDX, IDC_EDIT_ptParticleLink, m_particleLink);
	DDX_Text(pDX, IDC_EDIT_PtEndRange, m_ptEndRange);
	DDX_Text(pDX, IDC_EDIT_CurCfg, m_curCfgDefSpawn);
	DDX_Text(pDX, IDC_EDIT_fireQueStartRange, m_fireQueStartRange);
	DDX_Text(pDX, IDC_EDIT_fireQueEndRange, m_fireQueEndRange);
	DDX_Text(pDX, IDC_EDIT_fireQueMissileIndex, m_fireQueMissileIndex);
	DDX_Text(pDX, IDC_EDIT_fireQuelocalZadjust, m_fireQueZlocalAdjust);
	DDX_Text(pDX, IDC_EDIT_fireQueXpAdjust, m_fireQueXpAdjust);
	DDX_Text(pDX, IDC_EDIT_FireQueYpAdjust, m_fireQueYpAdjust);
	DDX_Text(pDX, IDC_EDIT_FPS, m_fps);
	DDX_Text(pDX, IDC_EDIT_BonePosX, m_bonePosX);
	DDX_Text(pDX, IDC_EDIT_BonePosY, m_bonePosY);
	DDX_Text(pDX, IDC_EDIT_BonePosZ, m_bonePosZ);
	DDX_Text(pDX, IDC_EDIT_damageSpecific, m_damageSpecific);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_PublicCfg, m_checkPublicCfg);
	DDX_Text(pDX, IDC_EDIT_baseAccuracy, m_baseAccuracy);


	DDX_Text(pDX, IDC_EDIT_sndSoundFactor, m_sndSoundFactor);
	DDX_Text(pDX, IDC_EDIT_sndClipRadius, m_clipRadius);
	DDX_Text(pDX, IDC_EDIT_sndSoundID, m_sndSoundID);
	DDX_Control(pDX, IDC_CHECK_sndVelocityBased, m_sndVelocityBased);
	DDX_Text(pDX, IDC_EDIT_maxVelocityDist, m_maxVelocityDist);
	DDX_Text(pDX, IDC_EDIT_sndStartFreq, m_startFreq);

	DDX_Control(pDX, IDC_LIST_visibilityTrack, m_visibilityTrackList);
	DDX_Text(pDX, IDC_EDIT_visibilityTrackStartRange, m_visibilityTrackStartRange);
	DDX_Text(pDX, IDC_EDIT_visibilityTrackEndRange, m_visibilityTrackEndRange);
	DDX_Text(pDX, IDC_EDIT_visibilityTrackAnimationId , m_visibilityTrackAnimationID);

}


BEGIN_MESSAGE_MAP(CSkeletalAnimationDlg, CDialog)
	//{{AFX_MSG_MAP(CSkeletalAnimationDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_CHECK_LOOPABLE, OnCheckLoopable)
	ON_LBN_SELCHANGE(IDC_LIST_ANIMATIONS, OnSelchangeListAnimations)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SET_SOUND_RANGE, OnButtonSetSoundRange)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSoundEvent, OnBUTTONDeleteSoundEvent)
	ON_BN_CLICKED(IDC_BUTTON_SETBONE_FOCUS1, OnButtonSetboneFocus1)
	ON_BN_CLICKED(IDC_BUTTON_DeformableMaterial, OnBUTTONDeformableMaterial)
	ON_BN_CLICKED(IDC_BUTTON_HiarchleMaterial, OnBUTTONHiarchleMaterial)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceValues, OnBUTTONReplaceValues)
	ON_BN_CLICKED(IDC_BUTTON_AddLSphere, OnBUTTONAddLSphere)
	ON_BN_CLICKED(IDC_BUTTON_DeleteLSphere, OnBUTTONDeleteLSphere)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceLCSphere, OnBUTTONReplaceLCSphere)
	ON_LBN_SELCHANGE(IDC_LIST_LocationSpheres, OnSelchangeLISTLocationSpheres)
	ON_EN_CHANGE(IDC_EDIT_sphereCalcRadius, OnChangeEDITsphereCalcRadius)
	ON_BN_CLICKED(IDC_CHECK_EnableSphereCollisionSys, OnCHECKEnableSphereCollisionSys)
	ON_CBN_SELCHANGE(IDC_COMBO_CSBoneSel, OnSelchangeCOMBOCSBoneSel)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceBDSphere, OnBUTTONReplaceBDSphere)
	ON_BN_CLICKED(IDC_BUTTON_DeleteBDSphere, OnBUTTONDeleteBDSphere)
	ON_BN_CLICKED(IDC_BUTTON_AddBDSphere, OnBUTTONAddBDSphere)
	ON_LBN_SELCHANGE(IDC_LIST_BodyColLocationSpheres, OnSelchangeLISTBodyColLocationSpheres)
	ON_BN_CLICKED(IDC_BUTTON_dPathAdd, OnBUTTONdPathAdd)
	ON_BN_CLICKED(IDC_BUTTON_dPathDelete, OnBUTTONdPathDelete)
	ON_BN_CLICKED(IDC_BUTTON_dPathReplace, OnBUTTONdPathReplace)
	ON_LBN_SELCHANGE(IDC_LIST_DamagePathTrackList, OnSelchangeLISTDamagePathTrackList)
	ON_BN_CLICKED(IDC_BUTTON_addParticleEvent, OnBUTTONaddParticleEvent)
	ON_BN_CLICKED(IDC_BUTTON_deleteParticleEvent, OnBUTTONdeleteParticleEvent)
	ON_BN_CLICKED(IDC_BUTTON_replaceParticleEvent, OnBUTTONreplaceParticleEvent)
	ON_LBN_SELCHANGE(IDC_LIST_ParticleEvents, OnSelchangeLISTParticleEvents)
	ON_BN_CLICKED(IDC_CHECK_UseRange, OnCHECKUseRange)
	ON_BN_CLICKED(IDC_BUTTON_unsetBoneLook, OnBUTTONunsetBoneLook)
	ON_BN_CLICKED(IDC_BUTTON_unsetSideStep, OnBUTTONunsetSideStep)
	ON_BN_CLICKED(IDC_BUTTON_SETBONE_SetSideStep, OnBUTTONSETBONESetSideStep)
	ON_BN_CLICKED(IDC_BUTTON_charona, OnBUTTONcharona)
	ON_BN_CLICKED(IDC_BUTTON_set, OnBUTTONset)
	ON_BN_CLICKED(IDC_BUTTON_lodHiarchy, OnBUTTONlodHiarchy)
	ON_BN_CLICKED(IDC_BUTTON_addDeformableCfg, OnBUTTONaddDeformableCfg)
	ON_BN_CLICKED(IDC_BUTTON_setLodDefCfg, OnBUTTONsetLodDefCfg)
	ON_BN_CLICKED(IDC_BUTTON_DeformableCfgMaterial, OnBUTTONDeformableCfgMaterial)
	ON_LBN_SELCHANGE(IDC_LIST_DEFORMABLEMESHLIST, OnSelchangeListDeformablemeshlist)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_defSpawnCfg, OnDeltaposSPINdefSpawnCfg)
	ON_BN_CLICKED(IDC_BUTTON_deleteDeformableCfg, OnBUTTONdeleteDeformableCfg)
	ON_BN_CLICKED(IDC_BTN_AddFireQue, OnBTNAddFireQue)
	ON_BN_CLICKED(IDC_BTN_DeleteFireQue, OnBTNDeleteFireQue)
	ON_BN_CLICKED(IDC_BTN_EditFireQue, OnBTNEditFireQue)
	ON_LBN_SELCHANGE(IDC_LIST_FireQueDB, OnSelchangeLISTFireQueDB)
	ON_BN_CLICKED(IDC_CHECK_secondaryOverridable, OnCHECKsecondaryOverridable)
	ON_LBN_SELCHANGE(IDC_LIST_Bones, OnSelchangeLISTBones)
	ON_BN_CLICKED(IDC_BUTTON_SetCenterOfMassLink, OnBUTTONSetCenterOfMassLink)
	ON_BN_CLICKED(IDC_BUTTON_UnsetCenterOfMassLink, OnBUTTONUnsetCenterOfMassLink)
	ON_BN_CLICKED(IDC_BUTTON_EditSoundEvent, OnBUTTONEditSoundEvent)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBUTTONReplace)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_PublicCfg, OnBnClickedCheckPubliccfg)
	ON_LBN_SELCHANGE(IDC_LIST_DeformableCfgs, OnLbnSelchangeListDeformablecfgs)
	ON_BN_CLICKED(IDC_BUTTON_setLodSet, OnBnClickedButtonsetlodset)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_VisibilityTrackAdd, &CSkeletalAnimationDlg::OnBnClickedBtnVisibilitytrackadd)
	ON_BN_CLICKED(IDC_BTN_visibilityTrackReplace, &CSkeletalAnimationDlg::OnBnClickedBtnvisibilitytrackreplace)
	ON_BN_CLICKED(IDC_BTN_visibilityTrackDelete, &CSkeletalAnimationDlg::OnBnClickedBtnvisibilitytrackdelete)
	ON_LBN_SELCHANGE(IDC_LIST_visibilityTrack, &CSkeletalAnimationDlg::OnLbnSelchangeListvisibilitytrack)
	ON_BN_CLICKED(IDC_BUTTON_applyAllPublicState, &CSkeletalAnimationDlg::OnBnClickedButtonapplyallpublicstate)
	ON_LBN_SELCHANGE(IDC_LIST_HIARCHMESHLIST, &CSkeletalAnimationDlg::OnLbnSelchangeListHiarchmeshlist)
	ON_BN_CLICKED(IDC_CHECK_HMeshCollisionFilter, &CSkeletalAnimationDlg::OnBnClickedCheckHmeshcollisionfilter)
	ON_BN_CLICKED(IDC_CHECK_DMeshCollisionFilter, &CSkeletalAnimationDlg::OnBnClickedCheckDmeshcollisionfilter)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkeletalAnimationDlg message handlers

BOOL CSkeletalAnimationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	//m_baseAccuracy = 0;
	m_animationType = "STAND";
	m_loopableState = FALSE;
	
	m_sndSoundFactor			= m_skeletonReferencePtr->m_soundFactor;
    m_clipRadius				= m_skeletonReferencePtr->m_clipRadius;
	m_sndSoundID				= m_skeletonReferencePtr->m_sndID;
    m_sndVelocityBased.SetCheck(m_skeletonReferencePtr->m_sndVelocityBased);
	m_maxVelocityDist			= m_skeletonReferencePtr->m_sndVelocityMaxDist;
	m_startFreq					= m_skeletonReferencePtr->m_startFrequency;
	
	if(m_elementDBREF)
    for(POSITION posLoc = m_elementDBREF->GetHeadPosition();posLoc != NULL;)
	{ 
	    CElementObj * elementPtr = (CElementObj *)m_elementDBREF->GetNext(posLoc);
     
        m_elementsAvailable.AddString(elementPtr->m_elementName);
	}
	m_elementsAvailable.SetCurSel(0);
	   
    m_dmpSkillTypeUse.AddString("NONE");
	if(m_skillDatabaseREF)
	for(POSITION posLoc = m_skillDatabaseREF->GetHeadPosition();posLoc != NULL;)
	{
        CSkillObject * sklPtr = (CSkillObject *)m_skillDatabaseREF->GetNext(posLoc);
        m_dmpSkillTypeUse.AddString(sklPtr->m_skillName);
	}
	m_dmpSkillTypeUse.SetCurSel(0);
		
	UpdateData(FALSE);
	
	
	AnimationInlist(-1);
	InlistBones();
	InListSoundEvents(0);
	
	InListDeformableMesh();
	InListHiarchleMesh();
	InListDamageSpheres(0);
	InListBDCSpheres(0);
	InListParticleEventsList(0);
	InListDPathList(0);
	InListFireQues(0);
	InListVisibilityTracks(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::OnButtonAdd() 
{
    UpdateData(TRUE);
	int type = -1;
	if(m_animationType == "WALK")
	   type = 3;
    if(m_animationType == "RUN")
	   type = 2;
    if(m_animationType == "STAND")
	   type = 1;
    if(m_animationType == "DIE")
       type = 8;
    if(m_animationType == "JUMP")
	   type = 5;
    if(m_animationType == "SIDESTEP LEFT")
	   type = 6;
    if(m_animationType == "SIDESTEP RIGHT")
	   type = 7;
	if(m_animationType == "CROUCH")
	   type = 11;
	if(m_animationType == "FIRE")
	   type = 12;
    if(m_animationType == "SWIM IDLE")
	   type = 14;
	if(m_animationType == "SWIM ACTION")
	   type = 15;
	if(m_animationType == "JET PACK")
	   type = 16;
	if(m_animationType == "PAIN")
	   type = 17;
	if(m_animationType == "TAUNT1")
	   type = 18;
	if(m_animationType == "TAUNT2")
	   type = 19;
	if(m_animationType == "TAUNT3")
	   type = 20;
	if(m_animationType == "DANCE1")
	   type = 21;
	if(m_animationType == "DANCE2")
	   type = 22;
	if(m_animationType == "DANCE3")
	   type = 23;
	if(m_animationType == "WAVE")
	   type = 24;
	if(m_animationType == "SALUTE")
	   type = 25;
	if(m_animationType == "BOW")
	   type = 26;
	if(m_animationType == "LAUGH")
	   type = 27;
	if(m_animationType == "SIT")
	   type = 28;
	if(m_animationType == "CROUCH")
	   type = 29;
	if(m_animationType == "MOUNT")
	   type = 30;
	if(m_animationType == "CAST1")
	   type = 31;
	if(m_animationType == "CAST2")
	   type = 32;
	if(m_animationType == "CAST3")
	   type = 33;

   
    if(type == -1)
	   return;

    //use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization

	if(m_skeletonReferencePtr->LoadAnimation(m_fileName,m_loopableState,type,m_requiredDuration,-1)==FALSE)
	   MessageBox("Failed to load animation file onto skeletal object");
	   AnimationInlist(-1);

	 SetCurrentDirectory(MainDir);
	UpdateData(FALSE);
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::OnButtonDelete() 
{
    UpdateData(TRUE);
    int index = m_animationsList.GetCurSel();
	if(index == -1){
	    return;
		MessageBox("select a animation");
	   }

    m_skeletonReferencePtr->DeleteAnimationByIndex(index);
	AnimationInlist(index-1);
	UpdateData(FALSE);
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::OnCheckLoopable() 
{
	if(m_loopableState == TRUE)
	   m_loopableState = FALSE;
	else
	   m_loopableState = TRUE;	
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::OnSelchangeListAnimations() 
{
	int animSel = m_animationsList.GetCurSel();
	if(animSel < 0)
	   return;
	
	
	
	POSITION bonepos = m_skeletonReferencePtr->m_bonesList->FindIndex(0);
	if(!bonepos)
		return;
        CBoneObject * boneptr = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(bonepos);
        POSITION animPos = boneptr->m_animationList->FindIndex(animSel);
		if(!animPos)
			return;
            CBoneAnimationObject * animationPtr = (CBoneAnimationObject *)boneptr->m_animationList->GetAt(animPos);
  
        UpdateData(TRUE);

		m_requiredDuration = animationPtr->m_minPlayDuration;
		m_animVersion =      animationPtr->m_version;

        if(animationPtr->m_looping)
           m_checkLoopable.SetCheck(1);
		else
		   m_checkLoopable.SetCheck(0);

		         if(animationPtr->m_type == 3)
					m_animationType = "WALK";
                 if(animationPtr->m_type == 2)
					m_animationType = "RUN";
                 if(animationPtr->m_type == 1)
					m_animationType = "STAND";
                 if(animationPtr->m_type == 8)
					m_animationType = "DIE";
				 if(animationPtr->m_type == 5)
					m_animationType = "JUMP";
                 if(animationPtr->m_type == 6)
					m_animationType = "SIDESTEP LEFT";
                 if(animationPtr->m_type == 7)
					m_animationType = "SIDESTEP RIGHT";
	             if(animationPtr->m_type == 11)
					m_animationType = "CROUCH";
				 if(animationPtr->m_type == 12)
					m_animationType = "FIRE";
				 if(animationPtr->m_type == 14)
	                m_animationType = "SWIM IDLE";
	             if(animationPtr->m_type == 15)
	                m_animationType = "SWIM ACTION";
	             if(animationPtr->m_type == 16)
	                m_animationType = "JET PACK";
	             if(animationPtr->m_type == 17)
	                m_animationType = "PAIN";
                 if(animationPtr->m_type == 18)
	                m_animationType = "TAUNT1";
                 if(animationPtr->m_type == 19)
	                m_animationType = "TAUNT2";
				 if(animationPtr->m_type == 20)
	                m_animationType = "TAUNT3";
                 if(animationPtr->m_type == 21)
	                m_animationType = "DANCE1";
				 if(animationPtr->m_type == 22)
	                m_animationType = "DANCE2";
				 if(animationPtr->m_type == 23)
	                m_animationType = "DANCE3";
				 if(animationPtr->m_type == 24)
	                m_animationType = "WAVE";
				 if(animationPtr->m_type == 25)
	                m_animationType = "SALUTE";
				 if(animationPtr->m_type == 26)
	                m_animationType = "BOW";
				 if(animationPtr->m_type == 27)
	                m_animationType = "LAUGH";
				 if(animationPtr->m_type == 28)
	                m_animationType = "SIT";
				 if(animationPtr->m_type == 29)
	                m_animationType = "CROUCH";
				 if(animationPtr->m_type == 30)
	                m_animationType = "MOUNT";
				 if(animationPtr->m_type == 31)
	                m_animationType = "CAST1";
				 if(animationPtr->m_type == 32)
	                m_animationType = "CAST2";
				 if(animationPtr->m_type == 33)
	                m_animationType = "CAST3";
   
        //calculate FPS
        m_fps = 1000/animationPtr->m_millisecondsPerFrame;



	    UpdateData(FALSE);
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::AnimationInlist(int sel)
{
    m_animationsList.ResetContent();
	if(m_skeletonReferencePtr->m_bonesList){//bones list valid
	   POSITION bonePos = m_skeletonReferencePtr->m_bonesList->FindIndex(0);
	   if(bonePos){//valid bone position
	      CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(bonePos);
		  
          int tracer = 0;
		  if(bone->m_animationList){
             for(POSITION animPos = bone->m_animationList->GetHeadPosition();animPos!=NULL;)
			 {
		         CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetNext(animPos);
			     CString indexTrace = operator+(ConvertLongToString(tracer),":");

			     if(animPtr->m_type == 3)
					m_animationsList.AddString(operator+(indexTrace,"WALK"));
                 else if(animPtr->m_type == 2)
					m_animationsList.AddString(operator+(indexTrace,"RUN"));
                 else if(animPtr->m_type == 1)
					m_animationsList.AddString(operator+(indexTrace,"STAND"));
                 else if(animPtr->m_type == 8)
					m_animationsList.AddString(operator+(indexTrace,"DIE"));
				 else if(animPtr->m_type == 5)
					m_animationsList.AddString(operator+(indexTrace,"JUMP"));
                 else if(animPtr->m_type == 6)
					m_animationsList.AddString(operator+(indexTrace,"SIDESTEP LEFT"));
                 else if(animPtr->m_type == 7)
					m_animationsList.AddString(operator+(indexTrace,"SIDESTEP RIGHT"));
	             else if(animPtr->m_type == 11)
					m_animationsList.AddString(operator+(indexTrace,"CROUCH"));
				 else if(animPtr->m_type == 12)
					m_animationsList.AddString(operator+(indexTrace,"FIRE"));
                 else if(animPtr->m_type == 14)
	                m_animationsList.AddString(operator+(indexTrace,"SWIM IDLE"));
	             else if(animPtr->m_type == 15)
	                m_animationsList.AddString(operator+(indexTrace,"SWIM ACTION"));
	             else if(animPtr->m_type == 16)
	                m_animationsList.AddString(operator+(indexTrace,"JET PACK"));
	             else if(animPtr->m_type == 17)
	                m_animationsList.AddString(operator+(indexTrace,"PAIN"));
                 else if(animPtr->m_type == 18)
	                m_animationsList.AddString(operator+(indexTrace,"TAUNT1"));
				 else if(animPtr->m_type == 19)
	                m_animationsList.AddString(operator+(indexTrace,"TAUNT2"));
				 else if(animPtr->m_type == 20)
	                m_animationsList.AddString(operator+(indexTrace,"TAUNT3"));
				 else if(animPtr->m_type == 21)
	                m_animationsList.AddString(operator+(indexTrace,"DANCE1"));
				 else if(animPtr->m_type == 22)
	                m_animationsList.AddString(operator+(indexTrace,"DANCE2"));
                 else if(animPtr->m_type == 23)
	                m_animationsList.AddString(operator+(indexTrace,"DANCE3"));
				 else if(animPtr->m_type == 24)
	                m_animationsList.AddString(operator+(indexTrace,"WAVE"));
				 else if(animPtr->m_type == 25)
	                m_animationsList.AddString(operator+(indexTrace,"SALUTE"));
				 else if(animPtr->m_type == 26)
	                m_animationsList.AddString(operator+(indexTrace,"BOW"));
				 else if(animPtr->m_type == 27)
	                m_animationsList.AddString(operator+(indexTrace,"LAUGH"));
				 else if(animPtr->m_type == 28)
	                m_animationsList.AddString(operator+(indexTrace,"SIT"));
				 else if(animPtr->m_type == 29)
	                m_animationsList.AddString(operator+(indexTrace,"CROUCH"));
				 else if(animPtr->m_type == 30)
	                m_animationsList.AddString(operator+(indexTrace,"MOUNT"));
				 else if(animPtr->m_type == 31)
	                m_animationsList.AddString(operator+(indexTrace,"CAST1"));
				 else if(animPtr->m_type == 32)
	                m_animationsList.AddString(operator+(indexTrace,"CAST2"));
				 else if(animPtr->m_type == 33)
	                m_animationsList.AddString(operator+(indexTrace,"CAST3"));
				 else 
				    m_animationsList.AddString("Unknown");
			 
				 tracer++;
			 }
		  }
	   }//end valid bone position
	}//end bones list valid
	if(sel > -1)
	m_animationsList.SetCurSel(sel);

}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::InlistBones()
{
  UpdateData(TRUE);
  m_listBones.ResetContent();
  if(m_skeletonReferencePtr->m_bonesList){
	  for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	  {
          CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
	      m_listBones.AddString(bone->m_boneName);
		  m_csBoneSel.AddString(bone->m_boneName);
	  }
  }
 
  if(m_skeletonReferencePtr->m_calulationSphere){
     m_csBoneSel.SetCurSel(m_skeletonReferencePtr->m_calulationSphere->m_boneIndex);
	 m_sphereCalcRadius = m_skeletonReferencePtr->m_calulationSphere->m_sphereRadius;
     m_checkEnableSphereCalc.SetCheck(1);
  }
  else
	m_csBoneSel.SetCurSel(0);

  UpdateData(FALSE);
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::OnButtonBrowse() 
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
 static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_fileName = opendialog.GetPathName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::OnButtonSetSoundRange() 
{
UpdateData(TRUE);
    if(!m_skeletonReferencePtr->m_bonesList){
		MessageBox("invalid Bones list");
	    return;
	}
  

    int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	   }
    int animindex = m_animationsList.GetCurSel();
	if(animindex == -1){
	   return;
	   MessageBox("select a animation");
	}

    CSoundRangeDlg dlg;
	dlg.m_velocityBased = FALSE;
    dlg.m_looping = FALSE;

	if(dlg.DoModal()==IDOK){
	   POSITION bonePos = m_skeletonReferencePtr->m_bonesList->FindIndex(boneindex);
       if(!bonePos){
		   MessageBox("Invalid Bone");
		   return;
	   }
	       CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(bonePos);
		   if(!bone->m_animationList){
           MessageBox("Invalid Animation List");
           return;
		   }
		   POSITION animPos = bone->m_animationList->FindIndex(animindex);
		   if(!bonePos){
		   MessageBox("Invalid Animation");
		   return;
		   }
		   CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetAt(animPos);
	       //dlg.m_clipRadius = 500.0f;//default
		   animPtr->SetSoundActivationEvent(dlg.m_fileName,
									        dlg.m_startRange,
									        dlg.m_endRange,
											dlg.m_id,
											dlg.m_soundFactor,
											dlg.m_clipRadius,
											dlg.m_velocityBased,
											dlg.m_looping);
		   InListSoundEvents(animPtr->m_soundRanges->GetCount()-1);
	}		
UpdateData(FALSE);	
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONDeleteSoundEvent() 
{
UpdateData(TRUE);
int sel = m_listSoundEvents.GetCurSel();
if(sel == -1){
   MessageBox("Must select a sound range");
   return;
}
if(m_skeletonReferencePtr->m_bonesList){
	  int trace=0;
	  for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	  {
          CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
	      if(bone->m_animationList)
			 for(POSITION posAnim = bone->m_animationList->GetHeadPosition();posAnim != NULL;)
			 {//animation list
		        CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetNext(posAnim);
                POSITION posLast;
				if(animPtr->m_soundRanges)
                for(POSITION soundPos = animPtr->m_soundRanges->GetHeadPosition();(posLast = soundPos) != NULL;)
				{//sound range loop
                      CSoundRangeObject * sndRngObj = (CSoundRangeObject *)animPtr->m_soundRanges->GetNext(soundPos);
					  if(trace == sel){//delete it
                         sndRngObj->SafeDelete();
						 delete sndRngObj;
						 sndRngObj=0;
                         animPtr->m_soundRanges->RemoveAt(posLast);
					  }//end delete it
				trace++;
				}//end sound range loop
			 }//end animation list
	  }
  }
UpdateData(FALSE);
InListSoundEvents(sel-1);
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
void CSkeletalAnimationDlg::InListSoundEvents(int sel)
{
UpdateData(TRUE);
  m_listSoundEvents.ResetContent();
  if(m_skeletonReferencePtr->m_bonesList){
	  for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	  {
          CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
	      if(bone->m_animationList)
			 for(POSITION posAnim = bone->m_animationList->GetHeadPosition();posAnim != NULL;)
			 {//animation list
		        CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetNext(posAnim);
                if(animPtr->m_soundRanges)
                for(POSITION soundPos = animPtr->m_soundRanges->GetHeadPosition();soundPos != NULL;)
				{//sound range loop
                      CSoundRangeObject * sndRngObj = (CSoundRangeObject *)animPtr->m_soundRanges->GetNext(soundPos);
					  CString startRange = ConvertLongToString(sndRngObj->m_startRange);
					  CString endRange = ConvertLongToString(sndRngObj->m_endRange);
					  CString ranges = operator+(" Ranges: ",startRange);
					  ranges = operator+(ranges,operator+(" ",endRange));
                   m_listSoundEvents.AddString(operator+(sndRngObj->m_fileName,ranges));
				}//end sound range loop
			 }//end animation list
	  }
  }
  if(sel > -1)
	 m_listSoundEvents.SetCurSel(sel);
UpdateData(FALSE);
}


//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//----------this function converts long to cstring-------------//
CString CSkeletalAnimationDlg::ConvertLongToString(int number)
{    
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
			  return temp;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----Get File Ext--------------------------------------------//
CString CSkeletalAnimationDlg::FloatToStingCnv(float number)
{	
 
   int  decimal, sign;   
   char *buffer;   
   float source = number;
   buffer = _fcvt( source, 7, &decimal, &sign );
   CString final = buffer;
   if(decimal == 0){
      final = operator+(".",final);
   }
   else if(decimal < 0){
	   for(int loop=0;loop<abs(decimal);loop++){
		   final = operator+("0",final);
	   }
      final = operator+(".",final);
   }
   else{
	   CString tempFinal;
	   for(int loop=0;loop<final.GetLength();loop++){
		   if(loop == decimal)
		   tempFinal = operator+(tempFinal,".");
		   
		   tempFinal = operator+(tempFinal,final.GetAt(loop));
	   }
	   final = tempFinal;
   }
   if(sign != 0)
      final = operator+("-",final);

   return final;
}

void CSkeletalAnimationDlg::OnButtonSetboneFocus1() 
{
	UpdateData(TRUE);
    if(!m_skeletonReferencePtr->m_bonesList){
		MessageBox("invalid Bones list");
	    return;
	}
  

    int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	   }
	
    m_skeletonReferencePtr->m_freeLookBone = boneindex;

	UpdateData(FALSE);
}


void CSkeletalAnimationDlg::InListHiarchleMesh()
{
    UpdateData(TRUE);
	m_listHiarchlMeshs.ResetContent();
    if(m_skeletonReferencePtr->m_bonesList)
	  for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	  {
          CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
          if(bone->m_hiarchyMesh){
		     m_listHiarchlMeshs.AddString(bone->m_boneName);
		  }
	  }
    UpdateData(FALSE);
}



//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------secondary animation---------------------------//
void CSkeletalAnimationDlg::InListDeformableMesh()
{
    UpdateData(TRUE);
    m_listDeformableMeshs.ResetContent();
    for(int loop=0;loop<m_skeletonReferencePtr->m_deformableMeshArrayCount;loop++){
        POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(loop);
	     if(alPos)
		 {
	        CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);
            if(alPtr->m_alterUnitName != "")
			   m_listDeformableMeshs.AddString(alPtr->m_alterUnitName);
			else
			   m_listDeformableMeshs.AddString("Deform Cfg");
		 }
		 else
		    m_listDeformableMeshs.AddString("Deform Cfg");
	}
    UpdateData(FALSE);
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONDeformableMaterial() 
{
  
  
	m_skeletonReferencePtr->GenerateTangents(NULL);
   
    AfxMessageBox("Completed");
}

void CSkeletalAnimationDlg::OnBUTTONHiarchleMaterial() 
{
    int meshindex = m_listHiarchlMeshs.GetCurSel();
	if(meshindex == -1){
	   MessageBox("select a hiarcle mesh");
       return;
	}
	UpdateData(TRUE);
    CEXMeshObj * refMesh = NULL;
	int trace = 0;
	CBoneObject * bone = NULL;
	if(m_skeletonReferencePtr->m_bonesList)
	{
	  for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	  {
          bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
          if(bone->m_hiarchyMesh){
			  if(trace == meshindex){
				 CHiarcleVisObj * hPtr = bone->m_hiarchyMesh->GetByIndex(0);
                 refMesh = hPtr->m_mesh;
				 break;
			  }
		  trace++;
		  }
	  }
	}
	else{
    MessageBox("error code 444");
	return;
	}
    if(!refMesh){
		MessageBox("error code 445");
	    return;
	}

	if(!refMesh->List_materialObject)
		refMesh->List_materialObject = new CMaterialObject();

    CMaterialDlg dlg;
	if(m_renderStateObjectRef)
	{
	dlg.m_pxShaderSystemRef = m_renderStateObjectRef->m_pxShderSystemRef;
	dlg.m_vxShaderSystemRef = m_renderStateObjectRef->m_shderSystemRef;
	}
	dlg.m_globalShaderRef = m_globalShaderRef;
	dlg.GL_textureDataBase = GL_textureDataBase;
       dlg.meshRef = refMesh;
	   
	   if(dlg.DoModal()==IDOK){
		  refMesh->List_materialObject->Reset();
        
		  int trace = 0;
		  for(POSITION posLoc = bone->m_hiarchyMesh->m_lodChain->GetHeadPosition();posLoc != NULL;)
		  {//update lod chain
			  CHiarcleVisObj * vObj = (CHiarcleVisObj *)bone->m_hiarchyMesh->m_lodChain->GetNext(posLoc);
			  if(trace == 0){
                 trace++;
				 continue;
			  }
			  delete vObj->m_mesh->List_materialObject;
			  vObj->m_mesh->List_materialObject = 0;
              refMesh->List_materialObject->Clone(&vObj->m_mesh->List_materialObject);
			  vObj->m_mesh->List_materialObject->Reset();
              for(int loop=0;loop<10;loop++)
			  {
			      vObj->m_mesh->List_storedTextureFromFile[loop] = refMesh->List_storedTextureFromFile[loop];
                  vObj->m_mesh->List_textureSelect[loop]         = refMesh->List_textureSelect[loop];
				  vObj->m_mesh->List_BlendMethod[loop]           = refMesh->List_BlendMethod[loop];
			  }
			  
		  }//end update lod chain
          //if update all
		  if(refMesh)
          if(dlg.applyMaterialToGroup)
		  {//apply all to hiarchy
			  trace = 0;
              if(m_skeletonReferencePtr->m_bonesList)
			  for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
			  {
				  bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
				  if(!bone->m_hiarchyMesh)
					 continue;
			       
				  
				  if(trace == meshindex)
				  {
				     trace++;
			         continue;
				  }

				  trace++;
		          
				  
				  for(POSITION lPos = bone->m_hiarchyMesh->m_lodChain->GetHeadPosition();lPos != NULL;)
				  {
                      CHiarcleVisObj * vObj = (CHiarcleVisObj *)bone->m_hiarchyMesh->m_lodChain->GetNext(lPos);
				      delete vObj->m_mesh->List_materialObject;
			          vObj->m_mesh->List_materialObject = 0;
                      refMesh->List_materialObject->Clone(&vObj->m_mesh->List_materialObject);
				      if(dlg.m_includeBlendMethods)
					  for(int loop=0;loop<10;loop++)
					  {
						  //vObj->m_mesh->List_storedTextureFromFile[loop] = refMesh->List_storedTextureFromFile[loop];
						  //vObj->m_mesh->List_textureSelect[loop]         = refMesh->List_textureSelect[loop];
						  vObj->m_mesh->List_BlendMethod[loop]           = refMesh->List_BlendMethod[loop];
					  }
				  
				  }
			  }
		  }//end apply all to hiarchy
	   }

	UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBUTTONReplaceValues() 
{
	int animSel = m_animationsList.GetCurSel();
	if(animSel < 0){
       AfxMessageBox("Must select a animation...");
	   return;
	   }
	
	
	POSITION bonepos = m_skeletonReferencePtr->m_bonesList->FindIndex(0);
	if(!bonepos)
		return;
        CBoneObject * boneptr = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(bonepos);
        //POSITION animPos = boneptr->m_animationList->FindIndex(animSel);
		//if(!animPos)
		//	return;
          //  CBoneAnimationObject * animationPtr = (CBoneAnimationObject *)boneptr->m_animationList->GetAt(animPos);
  
		int trace = 0;
        for(POSITION animPos = boneptr->m_animationList->GetHeadPosition();animPos != NULL;)
		{
            CBoneAnimationObject * animationPtr = (CBoneAnimationObject *)boneptr->m_animationList->GetNext(animPos);
			UpdateData(TRUE);

			

			animationPtr->m_millisecondsPerFrame = 1000/m_fps;

			if(animSel == trace)
			{
			animationPtr->m_looping = m_checkLoopable.GetCheck();
            animationPtr->m_minPlayDuration = m_requiredDuration;
			animationPtr->m_version = m_animVersion;
			}

			UpdateData(FALSE);
			trace++;
		}
}

void CSkeletalAnimationDlg::InListDamageSpheres(int sel)
{
	UpdateData(TRUE);
     m_listDamageSpheres.ResetContent();
     if(!m_skeletonReferencePtr->m_sphereLocationalDamageList)
		 return;

     for(POSITION posLoc = m_skeletonReferencePtr->m_sphereLocationalDamageList->GetHeadPosition();posLoc != NULL;)
	 {
		 CSphereCollisionObj * sphereCollisionObj = (CSphereCollisionObj *)m_skeletonReferencePtr->m_sphereLocationalDamageList->GetNext(posLoc);
         POSITION bonePos = m_skeletonReferencePtr->m_bonesList->FindIndex(sphereCollisionObj->m_boneIndex);
		 if(bonePos){
            CBoneObject * bonePtr = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(bonePos);
            CString strBuild = operator+(bonePtr->m_boneName," Radius:");
			strBuild = operator+(strBuild,ConvertLongToString(sphereCollisionObj->m_sphereRadius));
			strBuild = operator+(strBuild," Amp:");
			strBuild = operator+(strBuild,FloatToStingCnv(sphereCollisionObj->m_damageMultiplier));
		    m_listDamageSpheres.AddString(strBuild);
		 }
		 else
		 m_listDamageSpheres.AddString("Bone Locational Error 100293");
	 }
	 if(sel > -1)
		m_listDamageSpheres.SetCurSel(sel); 

	 UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBUTTONAddLSphere() 
{
	UpdateData(TRUE);
	int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	}
	if(!m_skeletonReferencePtr->m_sphereLocationalDamageList)
		m_skeletonReferencePtr->m_sphereLocationalDamageList = new CSphereCollisionObjList;
	
    CSphereCollisionObj * spPtr = new CSphereCollisionObj();
	spPtr->m_boneIndex         = boneindex;
	spPtr->m_damageMultiplier  = m_damageShphereAmplification;
	spPtr->m_sphereRadius      = m_damageSphereRadius;
	spPtr->m_comment           = m_dsphereComment;
	m_skeletonReferencePtr->m_sphereLocationalDamageList->AddTail(spPtr);
	UpdateData(FALSE);
	InListDamageSpheres(m_skeletonReferencePtr->m_sphereLocationalDamageList->GetCount());
}

void CSkeletalAnimationDlg::OnBUTTONDeleteLSphere() 
{
	int sphIndex = m_listDamageSpheres.GetCurSel();
	if(sphIndex == -1){
	   MessageBox("Must select a damage shpere collision object..");
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_sphereLocationalDamageList->FindIndex(sphIndex);
    if(!posLoc)
		return;
        CSphereCollisionObj * sphereCollisionObj = (CSphereCollisionObj *)m_skeletonReferencePtr->m_sphereLocationalDamageList->GetAt(posLoc);
        delete sphereCollisionObj;
		sphereCollisionObj=0;
		m_skeletonReferencePtr->m_sphereLocationalDamageList->RemoveAt(posLoc);
		InListDamageSpheres(m_skeletonReferencePtr->m_sphereLocationalDamageList->GetCount());
}

void CSkeletalAnimationDlg::OnBUTTONReplaceLCSphere() 
{
	int sphIndex = m_listDamageSpheres.GetCurSel();
	if(sphIndex == -1){
	   MessageBox("Must select a damage shpere collision object..");
       return;
	}
	int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_sphereLocationalDamageList->FindIndex(sphIndex);
    if(!posLoc)
		return;
        CSphereCollisionObj * sphereCollisionObj = (CSphereCollisionObj *)m_skeletonReferencePtr->m_sphereLocationalDamageList->GetAt(posLoc);
        UpdateData(TRUE);
          m_listBones.SetCurSel(sphereCollisionObj->m_boneIndex);
		  sphereCollisionObj->m_boneIndex        = boneindex;
          sphereCollisionObj->m_damageMultiplier = m_damageShphereAmplification;
          sphereCollisionObj->m_sphereRadius     = m_damageSphereRadius;
		  sphereCollisionObj->m_comment          = m_dsphereComment;
		UpdateData(FALSE);
		InListDamageSpheres(sphIndex);
}

void CSkeletalAnimationDlg::OnSelchangeLISTLocationSpheres() 
{
    int sphIndex = m_listDamageSpheres.GetCurSel();
	if(sphIndex == -1){
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_sphereLocationalDamageList->FindIndex(sphIndex);
    if(!posLoc)
		return;
        CSphereCollisionObj * sphereCollisionObj = (CSphereCollisionObj *)m_skeletonReferencePtr->m_sphereLocationalDamageList->GetAt(posLoc);
        UpdateData(TRUE);
          m_listBones.SetCurSel(sphereCollisionObj->m_boneIndex);
          m_damageShphereAmplification = sphereCollisionObj->m_damageMultiplier;
          m_damageSphereRadius         = sphereCollisionObj->m_sphereRadius;
		  m_dsphereComment             = sphereCollisionObj->m_comment;
		UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnChangeEDITsphereCalcRadius() 
{
	UpdateData(TRUE);
	if(m_skeletonReferencePtr->m_calulationSphere){
       m_skeletonReferencePtr->m_calulationSphere->m_sphereRadius = m_sphereCalcRadius;
	}
	UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnCHECKEnableSphereCollisionSys() 
{
	UpdateData(TRUE);
	if(!m_skeletonReferencePtr->m_calulationSphere){
       m_skeletonReferencePtr->m_calulationSphere = new CSphereCollisionObj();
	   m_skeletonReferencePtr->m_calulationSphere->m_sphereRadius = m_sphereCalcRadius;
	   int boneIndex = m_csBoneSel.GetCurSel();
	   if(boneIndex == -1)
		  boneIndex = 0;
       m_skeletonReferencePtr->m_calulationSphere->m_boneIndex = boneIndex;
       m_checkEnableSphereCalc.SetCheck(1);
	}
	else{
       delete m_skeletonReferencePtr->m_calulationSphere;
	   m_skeletonReferencePtr->m_calulationSphere = 0;
       m_checkEnableSphereCalc.SetCheck(0);
	}
	UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnSelchangeCOMBOCSBoneSel() 
{
	UpdateData(TRUE);
	if(m_skeletonReferencePtr->m_calulationSphere){
	   int boneIndex = m_csBoneSel.GetCurSel();
	   if(boneIndex == -1)
		  boneIndex = 0;
       m_skeletonReferencePtr->m_calulationSphere->m_boneIndex = boneIndex;
	  
	}
	UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBUTTONReplaceBDSphere() 
{
	int sphIndex = m_listBDCSpheres.GetCurSel();
	if(sphIndex == -1){
	   MessageBox("Must select a Body sphere collision object..");
       return;
	}
	int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_sphereBodyCollisionSystem->FindIndex(sphIndex);
    if(!posLoc)
		return;
        CSphereCollisionObj * sphereCollisionObj = (CSphereCollisionObj *)m_skeletonReferencePtr->m_sphereBodyCollisionSystem->GetAt(posLoc);
        UpdateData(TRUE);
          m_listBones.SetCurSel(sphereCollisionObj->m_boneIndex);
		  sphereCollisionObj->m_boneIndex        = boneindex;
          sphereCollisionObj->m_sphereRadius     = m_bdSphereRadius;
		UpdateData(FALSE);
		InListBDCSpheres(sphIndex);
}

void CSkeletalAnimationDlg::OnBUTTONDeleteBDSphere() 
{
	int sphIndex = m_listBDCSpheres.GetCurSel();
	if(sphIndex == -1){
	   MessageBox("Must select a Body sphere collision object..");
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_sphereBodyCollisionSystem->FindIndex(sphIndex);
    if(!posLoc)
		return;
        CSphereCollisionObj * sphereCollisionObj = (CSphereCollisionObj *)m_skeletonReferencePtr->m_sphereBodyCollisionSystem->GetAt(posLoc);
        delete sphereCollisionObj;
		sphereCollisionObj=0;
		m_skeletonReferencePtr->m_sphereBodyCollisionSystem->RemoveAt(posLoc);
		InListBDCSpheres(m_skeletonReferencePtr->m_sphereBodyCollisionSystem->GetCount());
}

void CSkeletalAnimationDlg::OnBUTTONAddBDSphere() 
{
	UpdateData(TRUE);
	int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	}
	if(!m_skeletonReferencePtr->m_sphereBodyCollisionSystem)
		m_skeletonReferencePtr->m_sphereBodyCollisionSystem = new CSphereCollisionObjList;
	
    CSphereCollisionObj * spPtr = new CSphereCollisionObj();
	spPtr->m_boneIndex         = boneindex;
	spPtr->m_sphereRadius      = m_bdSphereRadius;
	m_skeletonReferencePtr->m_sphereBodyCollisionSystem->AddTail(spPtr);
	UpdateData(FALSE);
	InListBDCSpheres(m_skeletonReferencePtr->m_sphereBodyCollisionSystem->GetCount());
}

void CSkeletalAnimationDlg::InListBDCSpheres(int sel)
{
	UpdateData(TRUE);
     m_listBDCSpheres.ResetContent();
     if(!m_skeletonReferencePtr->m_sphereBodyCollisionSystem)
		 return;

     for(POSITION posLoc = m_skeletonReferencePtr->m_sphereBodyCollisionSystem->GetHeadPosition();posLoc != NULL;)
	 {
		 CSphereCollisionObj * sphereCollisionObj = (CSphereCollisionObj *)m_skeletonReferencePtr->m_sphereBodyCollisionSystem->GetNext(posLoc);
         POSITION bonePos = m_skeletonReferencePtr->m_bonesList->FindIndex(sphereCollisionObj->m_boneIndex);
		 if(bonePos){
            CBoneObject * bonePtr = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(bonePos);
            CString strBuild = operator+(bonePtr->m_boneName," Radius:");
			strBuild = operator+(strBuild,ConvertLongToString(sphereCollisionObj->m_sphereRadius));
		    m_listBDCSpheres.AddString(strBuild);
		 }
		 else
		 m_listBDCSpheres.AddString("Bone Locational Error 100294");
	 }
	 if(sel > -1)
		m_listBDCSpheres.SetCurSel(sel); 

	 UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnSelchangeLISTBodyColLocationSpheres() 
{
	int sphIndex = m_listBDCSpheres.GetCurSel();
	if(sphIndex == -1){
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_sphereBodyCollisionSystem->FindIndex(sphIndex);
    if(!posLoc)
		return;
        CSphereCollisionObj * sphereCollisionObj = (CSphereCollisionObj *)m_skeletonReferencePtr->m_sphereBodyCollisionSystem->GetAt(posLoc);
        UpdateData(TRUE);
          m_listBones.SetCurSel(sphereCollisionObj->m_boneIndex);
          m_bdSphereRadius = sphereCollisionObj->m_sphereRadius;
		UpdateData(FALSE);
	
}

void CSkeletalAnimationDlg::InListDPathList(int sel)
{
   UpdateData(TRUE);
   m_dPathList.ResetContent();
   if(!m_skeletonReferencePtr->m_damageSphereList)
	   return;
     for(POSITION posLoc = m_skeletonReferencePtr->m_damageSphereList->GetHeadPosition();posLoc != NULL;)
	 {
		 CDamagePathObj * damagePath = (CDamagePathObj *)m_skeletonReferencePtr->m_damageSphereList->GetNext(posLoc);
         POSITION bonePos = m_skeletonReferencePtr->m_bonesList->FindIndex(damagePath->m_boneIndex);
		 if(bonePos){
            CBoneObject * bonePtr = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(bonePos);
            CString strBuild = operator+(bonePtr->m_boneName," Radius:");
			strBuild = operator+(strBuild,ConvertLongToString(damagePath->m_radius));
		    m_dPathList.AddString(strBuild);
		 }
		 else
		 m_dPathList.AddString("Bone Locational Error 100296");
	 }
	 if(sel > -1)
		m_dPathList.SetCurSel(sel); 
   
   UpdateData(FALSE);
}


void CSkeletalAnimationDlg::OnBUTTONdPathAdd() 
{
	UpdateData(TRUE);
	int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	}


	if(!m_skeletonReferencePtr->m_damageSphereList)
		m_skeletonReferencePtr->m_damageSphereList = new CDamagePathObjList;
	
    CDamagePathObj * spPtr = new CDamagePathObj();
	spPtr->m_boneIndex               = boneindex;
	spPtr->m_enableOnAnimationIndex  = m_dPathAnimationTrace;
	spPtr->m_radius                  = m_dPathRadius;
	spPtr->m_damageMinValue          = m_dPathMinValue;
	spPtr->m_damageMaxValue          = m_dPathMaxValue;
	spPtr->m_damageDotValue          = m_dPathDotValue;
	spPtr->m_damageChannel           = m_elementsAvailable.GetCurSel();
	spPtr->m_damageSpecific          = m_damageSpecific;


    CSkillObject * skillPtr = m_skillDatabaseREF->GetSkillByIndex(m_dmpSkillTypeUse.GetCurSel()-1);
    if(skillPtr)
	  spPtr->m_criteriaForSkillUse     = skillPtr->m_skillType;
	else
	  spPtr->m_criteriaForSkillUse     = -1;

	if(spPtr->m_damageChannel == -1)
	   spPtr->m_damageChannel = 0;
	m_skeletonReferencePtr->m_damageSphereList->AddTail(spPtr);
	UpdateData(FALSE);
	InListDPathList(m_skeletonReferencePtr->m_damageSphereList->GetCount());
}

void CSkeletalAnimationDlg::OnBUTTONdPathDelete() 
{
	int sphIndex = m_dPathList.GetCurSel();
	if(sphIndex == -1){
	   MessageBox("Must select a damage path...");
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_damageSphereList->FindIndex(sphIndex);
    if(!posLoc)
		return;
        CDamagePathObj * dPathObj = (CDamagePathObj *)m_skeletonReferencePtr->m_damageSphereList->GetAt(posLoc);
        delete dPathObj;
		dPathObj=0;
		m_skeletonReferencePtr->m_damageSphereList->RemoveAt(posLoc);
		InListDPathList(m_skeletonReferencePtr->m_damageSphereList->GetCount());
}

void CSkeletalAnimationDlg::OnBUTTONdPathReplace() 
{
	int sphIndex = m_dPathList.GetCurSel();
	if(sphIndex == -1){
	   MessageBox("Must select a damage path...");
       return;
	}

    int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	}


	POSITION posLoc = m_skeletonReferencePtr->m_damageSphereList->FindIndex(sphIndex);
    if(!posLoc)
		return;
        CDamagePathObj * aPathPtr = (CDamagePathObj *)m_skeletonReferencePtr->m_damageSphereList->GetAt(posLoc);
        UpdateData(TRUE);
          aPathPtr->m_boneIndex              = boneindex;
          aPathPtr->m_radius                 = m_dPathRadius;
	      aPathPtr->m_enableOnAnimationIndex = m_dPathAnimationTrace;
	      aPathPtr->m_damageMinValue         = m_dPathMinValue;
		  aPathPtr->m_damageMaxValue         = m_dPathMaxValue;
		  aPathPtr->m_damageDotValue         = m_dPathDotValue;
		  aPathPtr->m_damageChannel          = m_elementsAvailable.GetCurSel();
		  aPathPtr->m_damageSpecific         = m_damageSpecific;
		  CSkillObject * skillPtr = m_skillDatabaseREF->GetSkillByIndex(m_dmpSkillTypeUse.GetCurSel()-1);
          if(skillPtr)
	         aPathPtr->m_criteriaForSkillUse     = skillPtr->m_skillType;
	       else
	         aPathPtr->m_criteriaForSkillUse     = -1;
	      if(aPathPtr->m_damageChannel == -1)
	         aPathPtr->m_damageChannel = 0;
		UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnSelchangeLISTDamagePathTrackList() 
{
	int sphIndex = m_dPathList.GetCurSel();
	if(sphIndex == -1){
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_damageSphereList->FindIndex(sphIndex);
    if(!posLoc)
		return;
        CDamagePathObj * aPathPtr = (CDamagePathObj *)m_skeletonReferencePtr->m_damageSphereList->GetAt(posLoc);
        UpdateData(TRUE);
          m_listBones.SetCurSel(aPathPtr->m_boneIndex);
          m_dPathRadius                    = aPathPtr->m_radius;
	      m_dPathAnimationTrace            = aPathPtr->m_enableOnAnimationIndex;
	      m_dPathMinValue                  = aPathPtr->m_damageMinValue;
		  m_dPathMaxValue                  = aPathPtr->m_damageMaxValue;
		  m_dPathDotValue                  = aPathPtr->m_damageDotValue;
          m_damageSpecific                 = aPathPtr->m_damageSpecific;

		  m_dmpSkillTypeUse.SetCurSel(m_skillDatabaseREF->GetSkillIndexByType(aPathPtr->m_criteriaForSkillUse)+1);

		  m_elementsAvailable.SetCurSel(aPathPtr->m_damageChannel);
		UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBUTTONaddParticleEvent() 
{
	 UpdateData(TRUE);
	 int boneindex = m_listBones.GetCurSel();
	 if(boneindex == -1){
	    MessageBox("select a bone");
        return;
	 }
	 int animSel = m_animationsList.GetCurSel();
	 if(animSel < 0){
       AfxMessageBox("Must select a animation...");
	   return;
	 }

	 if(!m_skeletonReferencePtr->m_particleEmiterList)
	 m_skeletonReferencePtr->m_particleEmiterList = new CParticleEmitRangeObjList;
     CParticleEmitRangeObj * emitPt = new CParticleEmitRangeObj();
     emitPt->m_animationQue    = animSel;
	 emitPt->m_startRange      = m_ptStartRange;
	 emitPt->m_endRange        = m_ptEndRange;
	 emitPt->m_particleDBIndex = m_particleLink;
	 emitPt->m_boneLink        = boneindex;
	 emitPt->m_useExactRange   = m_emitPtUseRange.GetCheck();
	 m_skeletonReferencePtr->m_particleEmiterList->AddTail(emitPt);
	
	 UpdateData(FALSE);
	 InListParticleEventsList(m_skeletonReferencePtr->m_particleEmiterList->GetCount()-1);
}

void CSkeletalAnimationDlg::OnBUTTONdeleteParticleEvent() 
{
	int ptIndex = m_listParticleEvents.GetCurSel();
	if(ptIndex == -1){
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_particleEmiterList->FindIndex(ptIndex);
    if(!posLoc)
		return;
        CParticleEmitRangeObj * emitPt = (CParticleEmitRangeObj *)m_skeletonReferencePtr->m_particleEmiterList->GetAt(posLoc);
        UpdateData(TRUE);
        emitPt->SafeDelete();
		delete emitPt;
		emitPt=0;
		m_skeletonReferencePtr->m_particleEmiterList->RemoveAt(posLoc);
		UpdateData(FALSE);
		InListParticleEventsList(ptIndex-1);
}

void CSkeletalAnimationDlg::OnBUTTONreplaceParticleEvent() 
{
	int ptIndex = m_listParticleEvents.GetCurSel();
	int boneindex = m_listBones.GetCurSel();
	 if(boneindex == -1){
	    MessageBox("select a bone");
        return;
	 }
	 int animSel = m_animationsList.GetCurSel();
	 if(animSel < 0){
       AfxMessageBox("Must select a animation...");
	   return;
	 }
	 if(ptIndex == -1){
	   AfxMessageBox("Must select a particle index...");
       return;
	 }
	 POSITION posLoc = m_skeletonReferencePtr->m_particleEmiterList->FindIndex(ptIndex);
     if(!posLoc)
		return;
        CParticleEmitRangeObj * emitPt = (CParticleEmitRangeObj *)m_skeletonReferencePtr->m_particleEmiterList->GetAt(posLoc);
        UpdateData(TRUE);
        emitPt->m_animationQue    = animSel;
	    emitPt->m_startRange      = m_ptStartRange;
   	    emitPt->m_endRange        = m_ptEndRange;
	    emitPt->m_particleDBIndex = m_particleLink;
	    emitPt->m_boneLink        = boneindex;
	    emitPt->m_useExactRange   = m_emitPtUseRange.GetCheck();
		UpdateData(FALSE);
		InListParticleEventsList(ptIndex);
	
}

void CSkeletalAnimationDlg::OnSelchangeLISTParticleEvents() 
{
	int ptIndex = m_listParticleEvents.GetCurSel();
	if(ptIndex == -1){
       return;
	}
	POSITION posLoc = m_skeletonReferencePtr->m_particleEmiterList->FindIndex(ptIndex);
    if(!posLoc)
		return;
        CParticleEmitRangeObj * emitPt = (CParticleEmitRangeObj *)m_skeletonReferencePtr->m_particleEmiterList->GetAt(posLoc);
        UpdateData(TRUE);
          m_listBones.SetCurSel(emitPt->m_boneLink);
		  m_animationsList.SetCurSel(emitPt->m_animationQue);
	      m_ptStartRange = emitPt->m_startRange;
	      m_ptEndRange = emitPt->m_endRange;
	      m_particleLink = emitPt->m_particleDBIndex;
	      m_emitPtUseRange.SetCheck(emitPt->m_useExactRange);
         
		UpdateData(FALSE);
}

void CSkeletalAnimationDlg::InListParticleEventsList(int sel)
{
   UpdateData(TRUE);
   m_listParticleEvents.ResetContent();
   if(!m_skeletonReferencePtr->m_particleEmiterList)
	   return;
     for(POSITION posLoc = m_skeletonReferencePtr->m_particleEmiterList->GetHeadPosition();posLoc != NULL;)
	 {
		 CParticleEmitRangeObj * emitPt = (CParticleEmitRangeObj *)m_skeletonReferencePtr->m_particleEmiterList->GetNext(posLoc);
         POSITION bonePos = m_skeletonReferencePtr->m_bonesList->FindIndex(emitPt->m_boneLink);
		 if(bonePos){
            CBoneObject * bonePtr = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(bonePos);
            CString strBuild = operator+(bonePtr->m_boneName," DB:");
			strBuild = operator+(strBuild,ConvertLongToString(emitPt->m_particleDBIndex));
		    m_listParticleEvents.AddString(strBuild);
		 }
		 else
		 m_listParticleEvents.AddString("Bone Locational Error 100297");
	 }
	 if(sel > -1)
		m_listParticleEvents.SetCurSel(sel); 
   
   UpdateData(FALSE);
}


void CSkeletalAnimationDlg::OnCHECKUseRange() 
{
	
}

void CSkeletalAnimationDlg::OnBUTTONunsetBoneLook() 
{
	m_skeletonReferencePtr->m_freeLookBone = -1;
	m_skeletonReferencePtr->m_freeLookBone2 = 0;
}

void CSkeletalAnimationDlg::OnBUTTONunsetSideStep() 
{
	m_skeletonReferencePtr->m_waistBone = -1;
	
}

void CSkeletalAnimationDlg::OnBUTTONSETBONESetSideStep() 
{
	UpdateData(TRUE);
    if(!m_skeletonReferencePtr->m_bonesList){
		MessageBox("invalid Bones list");
	    return;
	}
  
    int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	   }
	
    m_skeletonReferencePtr->m_waistBone = boneindex;

	UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBUTTONcharona() 
{
UpdateData(TRUE);
	int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	}

	POSITION posLoc = m_skeletonReferencePtr->m_bonesList->FindIndex(boneindex);
		if(!posLoc)
			return;
		    CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(posLoc);

	UpdateData(FALSE);

	CCharonaDlg clg;

    if(!bone->m_hiarchyMesh){
       MessageBox("no valid mesh object available on bone");
       return;
	}

    CHiarcleVisObj * cObj = bone->m_hiarchyMesh->GetByIndex(0);
    clg.m_charonaRef = cObj->m_mesh->m_charona;


    clg.m_textureDataBase = GL_textureDataBase;
    clg.DoModal();
    cObj->m_mesh->m_charona = clg.m_charonaRef;
	
}

void CSkeletalAnimationDlg::OnBUTTONset() 
{
	UpdateData(TRUE);
    if(!m_skeletonReferencePtr->m_bonesList){
		MessageBox("invalid Bones list");
	    return;
	}
  

    int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a bone");
       return;
	   }
	
    m_skeletonReferencePtr->m_freeLookBone2 = boneindex;

	UpdateData(FALSE);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONlodHiarchy() 
{
	UpdateData(TRUE);
    if(!m_skeletonReferencePtr->m_bonesList){
		MessageBox("invalid bones mesh list");
	    return;
	}
  
    int boneindex = m_listHiarchlMeshs.GetCurSel();
	if(boneindex == -1){
	   MessageBox("select a hiarchy mesh");
       return;
	}

	int trace = 0;
	CBoneObject * bone = NULL;
	if(m_skeletonReferencePtr->m_bonesList){
	  for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	  {
          CBoneObject * bn = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
          if(bn->m_hiarchyMesh){
			  if(trace == boneindex){
			     bone = bn;
			  }
		  trace++;
		  
		  }
	  }
	}
	else{
    MessageBox("error code 444");
	return;
	}

    //use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization
        CDmeshLodEditorDlg dlg;
		dlg.m_lodChainRef = NULL;
		dlg.m_lodChainRefHiarchy = bone->m_hiarchyMesh->m_lodChain;
        if(dlg.DoModal() == IDOK){
		   
		   if(dlg.m_selection == 1 || dlg.m_selection == 2){
	          
              static char BASED_CODE filter[] = "HIM Deformable modules|*.him|All Files (*.*)|*.*||";
	          CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
              if ( opendialog.DoModal() != IDOK )
			  {
	               SetCurrentDirectory(MainDir);
	               return;
			  }

              m_fileName = opendialog.GetPathName();

			  if(dlg.m_selection == 1)
			     bone->m_hiarchyMesh->SetLevel(1000,m_fileName);
		
		      if(dlg.m_selection == 2)
			     bone->m_hiarchyMesh->SetLevel(dlg.m_listSelection,m_fileName);
		   
		   
		   }  
		   
		   if(dlg.m_selection == 3)
		   {//delete que

			  POSITION posLoc = bone->m_hiarchyMesh->m_lodChain->FindIndex(dlg.m_listSelection);
			  if(posLoc)
			  {
                 CHiarcleVisObj * vPtr = (CHiarcleVisObj *)bone->m_hiarchyMesh->m_lodChain->GetAt(posLoc);
                 vPtr->SafeDelete();
				 delete vPtr;
				 vPtr=0;
                 bone->m_hiarchyMesh->m_lodChain->RemoveAt(posLoc);
			  }
		   }//end delete que
		}
    SetCurrentDirectory(MainDir);


	UpdateData(FALSE);
}

//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONDeformableCfgMaterial() 
{
    UpdateData(TRUE);
  
    int meshindex = m_listDeformableMeshs.GetCurSel();
	if(meshindex == -1){
	   MessageBox("select a deformable mesh");
       return;
	}

	int dcfgIndex = m_deformableCfgs.GetCurSel();
	if(dcfgIndex == -1){
	   MessageBox("select a deformable mesh cfg");
       return;
	}

	BOOL applyToAllDim = FALSE;
	BOOL includeBlendMethods = FALSE;
	CDeformableVisObj * materialRefDimVis = NULL;

    if(!m_skeletonReferencePtr->m_alternateConfigurations)
	    return;
	     
	
	
	     POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(meshindex);
	     if(!alPos)
		     return;	
	         CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);

             POSITION posLoc = alPtr->m_configurations->FindIndex(dcfgIndex);
			 if(!posLoc)
				 return;
             CDeformableMesh * dMesh = (CDeformableMesh *) alPtr->m_configurations->GetAt(posLoc);
			 POSITION dmPos = dMesh->m_lodChain->FindIndex(0);
			 if(!dmPos)
			     return;
			     CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetAt(dmPos);
	
                 CEXMeshObj * refMesh = dVis->m_mesh;
                 CMaterialDlg dlg;
				 if(m_renderStateObjectRef)
				 {
				 dlg.m_pxShaderSystemRef = m_renderStateObjectRef->m_pxShderSystemRef;
	             dlg.m_vxShaderSystemRef = m_renderStateObjectRef->m_shderSystemRef;
				 }
				 dlg.m_globalShaderRef = m_globalShaderRef;
	              dlg.GL_textureDataBase = GL_textureDataBase;
                  dlg.meshRef = refMesh;
				  if(dlg.DoModal()==IDOK){
		             refMesh->List_materialObject->Reset();
                     applyToAllDim = dlg.applyMaterialToGroup;
					 includeBlendMethods = dlg.m_includeBlendMethods;

                     int trace = 0;
		             for(POSITION posLoc = dMesh->m_lodChain->GetHeadPosition();posLoc != NULL;)
					 {//update lod chain
			            CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(posLoc);
						if(trace == 0){
						   materialRefDimVis = dVis;
                           trace++;
				           continue;
						}
			            delete dVis->m_mesh->List_materialObject;
			            dVis->m_mesh->List_materialObject = 0;
                        refMesh->List_materialObject->Clone(&dVis->m_mesh->List_materialObject);
			            dVis->m_mesh->List_materialObject->Reset();
						
                         for(int loop=0;loop<10;loop++)
						 {
			                dVis->m_mesh->List_storedTextureFromFile[loop] = refMesh->List_storedTextureFromFile[loop];
                            dVis->m_mesh->List_textureSelect[loop]         = refMesh->List_textureSelect[loop];
				            dVis->m_mesh->List_BlendMethod[loop]           = refMesh->List_BlendMethod[loop];
						 }
					 }//end update lod chain
				  }


    //Apply to group
         if(applyToAllDim)
		 if(materialRefDimVis)
         for(POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->GetHeadPosition();alPos != NULL;)
		 {
	         CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);

             for(POSITION posLoc = alPtr->m_configurations->GetHeadPosition();posLoc != NULL;)
			 {
              CDeformableMesh * dMesh = (CDeformableMesh *) alPtr->m_configurations->GetNext(posLoc);
			  POSITION dmPos = dMesh->m_lodChain->FindIndex(0);
			  if(!dmPos)
			     return;
			     CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetAt(dmPos);
	             if(dVis == materialRefDimVis)
				    continue;
                 CEXMeshObj * refMesh = materialRefDimVis->m_mesh;
				 if(m_renderStateObjectRef)
				 {
				    dlg.m_pxShaderSystemRef = m_renderStateObjectRef->m_pxShderSystemRef;
	                dlg.m_vxShaderSystemRef = m_renderStateObjectRef->m_shderSystemRef;
				 }
				  dlg.m_globalShaderRef = m_globalShaderRef;
	              dlg.GL_textureDataBase = GL_textureDataBase;
                  dlg.meshRef = refMesh;
				 
                     int trace = 0;
		             for(POSITION posLoc = dMesh->m_lodChain->GetHeadPosition();posLoc != NULL;)
					 {//update lod chain
			            dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(posLoc);
			            if(materialRefDimVis == dVis){
				           continue;
						}
			            delete dVis->m_mesh->List_materialObject;
			            dVis->m_mesh->List_materialObject = 0;
                        refMesh->List_materialObject->Clone(&dVis->m_mesh->List_materialObject);
			            dVis->m_mesh->List_materialObject->Reset();
                         for(int loop=0;loop<10;loop++)
						 {
			                //dVis->m_mesh->List_storedTextureFromFile[loop] = refMesh->List_storedTextureFromFile[loop];
                            //dVis->m_mesh->List_textureSelect[loop]         = refMesh->List_textureSelect[loop];
							if(includeBlendMethods)
							   dVis->m_mesh->List_BlendMethod[loop]           = refMesh->List_BlendMethod[loop];
						 }
					 }//end update lod chain
			 }
		 }


				  
	UpdateData(FALSE);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnSelchangeListDeformablemeshlist() 
{
  int selection = m_listDeformableMeshs.GetCurSel();
  if(selection == -1)
	 return;
    
     UpdateData(TRUE);
     if(m_skeletonReferencePtr->m_spawnCfg)
     m_curCfgDefSpawn = m_skeletonReferencePtr->m_spawnCfg[selection];//valid
	 UpdateData(FALSE);
	 
	 POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(selection);
	 if(!alPos)
		 return;	
	         
	 CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);
     POSITION posLoc = alPtr->m_configurations->FindIndex(0);
	 if(!posLoc)
	     return;
                 
	 CDeformableMesh * dMesh = (CDeformableMesh *) alPtr->m_configurations->GetAt(posLoc);

     m_checkDMeshCollisionFilter.SetCheck(dMesh->m_collisionFiltered);
	 

	 InListCfgs(0);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONsetLodDefCfg() 
{
	UpdateData(TRUE);
    int meshindex = m_listDeformableMeshs.GetCurSel();
	if(meshindex == -1){
	   MessageBox("select a deformable mesh");
       return;
	}

	int dcfgIndex = m_deformableCfgs.GetCurSel();
	if(dcfgIndex == -1){
	   MessageBox("select a deformable mesh cfg");
       return;
	}

    if(!m_skeletonReferencePtr->m_alternateConfigurations)
		 return;
	     POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(meshindex);
	     if(!alPos)
			 return;	
	         CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);
             POSITION posLoc = alPtr->m_configurations->FindIndex(dcfgIndex);
			 if(!posLoc)
				 return;
                 CDeformableMesh * dMesh = (CDeformableMesh *) alPtr->m_configurations->GetAt(posLoc);
                 //use mapsmodels folder for organization
                  CString CurDir;
                  CString MainDir;
                  char buffer[_MAX_PATH];
			      _getcwd( buffer, _MAX_PATH );
			      MainDir = CurDir = buffer;
			      CurDir = operator+(CurDir,"\\MapsModels\\");
			      SetCurrentDirectory(CurDir);
                 //end use mapsmodels folder for organization
                 CDmeshLodEditorDlg dlg;
		         dlg.m_lodChainRefHiarchy = NULL;
		         dlg.m_lodChainRef = dMesh->m_lodChain;
                 if(dlg.DoModal() == IDOK){
		           if(dlg.m_selection == 1 || dlg.m_selection == 2){
	          
                    static char BASED_CODE filter[] = "DIM Deformable modules|*.dim|All Files (*.*)|*.*||";
	                CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
                    if ( opendialog.DoModal() != IDOK )
					{
	                    SetCurrentDirectory(MainDir);
	                    return;
					}

                    m_fileName = opendialog.GetPathName();

			        if(dlg.m_selection == 1)
			           dMesh->SetLevel(1000,m_fileName);
		
		            if(dlg.m_selection == 2)
			           dMesh->SetLevel(dlg.m_listSelection,m_fileName);
				   }  
		   
		           if(dlg.m_selection == 3)
				   {//delete que

			          POSITION posLoc = dMesh->m_lodChain->FindIndex(dlg.m_listSelection);
			          if(posLoc)
					  {
                        CDeformableVisObj * vPtr = (CDeformableVisObj *)dMesh->m_lodChain->GetAt(posLoc);
                        vPtr->SafeDelete();
				        delete vPtr;
				        vPtr=0;
                        dMesh->m_lodChain->RemoveAt(posLoc);
					  }
				   }//end delete que
				 }
                 SetCurrentDirectory(MainDir);
	UpdateData(FALSE);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONaddDeformableCfg() 
{
	int meshindex = m_listDeformableMeshs.GetCurSel();
	if(meshindex == -1){
	   MessageBox("select a deformable mesh");
       return;
	}
	
	if(!m_skeletonReferencePtr->m_alternateConfigurations)
		 return;
	     POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(meshindex);
	     if(!alPos)
		     return;	
	         CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);
             //use mapsmodels folder for organization
                  CString CurDir;
                  CString MainDir;
                  char buffer[_MAX_PATH];
			      _getcwd( buffer, _MAX_PATH );
			      MainDir = CurDir = buffer;
			      CurDir = operator+(CurDir,"\\MapsModels\\");
			      SetCurrentDirectory(CurDir);
             //end use mapsmodels folder for organization
			       static char BASED_CODE filter[] = "DIM Deformable modules|*.dim|All Files (*.*)|*.*||";
	               CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
                   if ( opendialog.DoModal() != IDOK )
				   {
	                     SetCurrentDirectory(MainDir);
	                     return;
				   }
			             m_fileName = opendialog.GetPathName();
			             CDeformableMesh * dMesh = new CDeformableMesh();
						 dMesh->m_lodChain = new CDeformableVisList;
			             dMesh->SetLevel(0,m_fileName);
			             alPtr->m_configurations->AddTail(dMesh);
						 InListCfgs((alPtr->m_configurations->GetCount()-1));
						 SetCurrentDirectory(MainDir);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::InListCfgs(int sel)
{
	int meshindex = m_listDeformableMeshs.GetCurSel();
	if(meshindex == -1){
	   MessageBox("select a deformable mesh");
       return;
	}
     if(!m_skeletonReferencePtr->m_alternateConfigurations)
		 return;
	     m_deformableCfgs.ResetContent();
	     POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(meshindex);
	     if(!alPos)
		     return;	
	         CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);

             for(POSITION posLoc = alPtr->m_configurations->GetHeadPosition();posLoc != NULL;)
			 {
               CDeformableMesh * dMesh = (CDeformableMesh *) alPtr->m_configurations->GetNext(posLoc);
			   POSITION dmPos = dMesh->m_lodChain->FindIndex(0);
			   if(dmPos){
			      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetAt(dmPos);
			      if(dVis)
				  {
                     if(dMesh->m_dimName == "")
			            m_deformableCfgs.AddString(dVis->m_mesh->List_storedTextureFromFile[0]);
					 else
						 m_deformableCfgs.AddString(dMesh->m_dimName);
				  }
			   }
			   else
				   m_deformableCfgs.AddString("error 011");
			 }
             if(sel != -1)
		  	    m_deformableCfgs.SetCurSel(sel);
}     
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnDeltaposSPINdefSpawnCfg(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int meshindex = m_listDeformableMeshs.GetCurSel();
	if(meshindex == -1){
	   MessageBox("select a deformable mesh");
       return;
	}
	
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	UpdateData(TRUE);
    int old = m_skeletonReferencePtr->m_spawnCfg[meshindex];
    m_skeletonReferencePtr->m_spawnCfg[meshindex] -= pNMUpDown->iDelta;
	if(m_skeletonReferencePtr->m_spawnCfg[meshindex] < 0)
	   m_skeletonReferencePtr->m_spawnCfg[meshindex] = 0;
       

    POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(meshindex);
	     if(!alPos)
		     return;	
	         CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);

			 POSITION posLoc = alPtr->m_configurations->FindIndex(m_skeletonReferencePtr->m_spawnCfg[meshindex]);
             if(!posLoc){
				 m_skeletonReferencePtr->m_spawnCfg[meshindex] = old;
				 return;
			 }
	             m_curCfgDefSpawn = m_skeletonReferencePtr->m_spawnCfg[meshindex];//valid
    UpdateData(FALSE);
	
	*pResult = 0;
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONdeleteDeformableCfg() 
{

    if(AfxMessageBox("Are you sure you want to delete this skeletal system ?",MB_OKCANCEL,0)!=IDOK)
	   return;

    int meshindex = m_listDeformableMeshs.GetCurSel();
	if(meshindex == -1){
	   MessageBox("select a deformable mesh");
       return;
	}

	int dcfgIndex = m_deformableCfgs.GetCurSel();
	if(dcfgIndex == -1){
	   MessageBox("select a deformable mesh cfg");
       return;
	}
	
	 if(!m_skeletonReferencePtr->m_alternateConfigurations)
		 return;
	     POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(meshindex);
	     if(!alPos)
		     return;	
	         CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);
               
			 POSITION cfgPos = alPtr->m_configurations->FindIndex(dcfgIndex);
			 if(!cfgPos)
				 return;
                   CDeformableMesh * dMesh = (CDeformableMesh *)alPtr->m_configurations->GetAt(cfgPos);
				   dMesh->SafeDelete();
				   delete dMesh;
				   dMesh = 0;
                   alPtr->m_configurations->RemoveAt(cfgPos);
				   InListCfgs((alPtr->m_configurations->GetCount()-1));
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBTNAddFireQue() 
{
    int animSel = m_animationsList.GetCurSel();
	if(animSel < 0){
       AfxMessageBox("Must select a animation...");
	   return;
	}

	int boneSel = m_listBones.GetCurSel();
	if(boneSel < 0){
       AfxMessageBox("Must select a bone...");
	   return;
	}

	if(!m_skeletonReferencePtr->m_fireQueList)
		m_skeletonReferencePtr->m_fireQueList = new CFireRangeObjectList();


    UpdateData(TRUE);

    CFireRangeObject * m_fireRange = new CFireRangeObject();

	m_fireRange->m_animationIndex           = animSel;
	m_fireRange->m_boneIndex                = boneSel;
	m_fireRange->m_animationFrameRangeEnd   = m_fireQueEndRange;
	m_fireRange->m_animationFrameRangeStart = m_fireQueStartRange;
	m_fireRange->m_missileIndex             = m_fireQueMissileIndex;
	m_fireRange->m_hardPointAdjust.z        = m_fireQueZlocalAdjust;
    m_fireRange->m_hardPointAdjust.x        = m_fireQueXpAdjust;
    m_fireRange->m_hardPointAdjust.y        = m_fireQueYpAdjust;
	m_fireRange->m_accuracyRating           = m_baseAccuracy;
 
	m_skeletonReferencePtr->m_fireQueList->AddTail(m_fireRange);
	
	UpdateData(FALSE);
    InListFireQues(m_skeletonReferencePtr->m_fireQueList->GetCount());
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBTNDeleteFireQue() 
{
	int fireQueSel = m_fireQueDB.GetCurSel();
	if(fireQueSel < 0){
	   return;
	}

	if(!m_skeletonReferencePtr->m_fireQueList)
		return;


    UpdateData(TRUE);

    POSITION fPos = m_skeletonReferencePtr->m_fireQueList->FindIndex(fireQueSel);
	if(!fPos)
		return;
        CFireRangeObject * m_fireRange = (CFireRangeObject *)m_skeletonReferencePtr->m_fireQueList->GetAt(fPos);

	    delete m_fireRange;
		m_fireRange = 0;

		m_skeletonReferencePtr->m_fireQueList->RemoveAt(fPos);
        
	UpdateData(FALSE);
	InListFireQues(fireQueSel-1);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBTNEditFireQue() 
{
    int animSel = m_animationsList.GetCurSel();
	if(animSel < 0){
       AfxMessageBox("Must select a animation...");
	   return;
	}

    int fireQueSel = m_fireQueDB.GetCurSel();
	if(fireQueSel < 0){
       AfxMessageBox("Must select a Fire Que...");
	   return;
	}

	int boneSel = m_listBones.GetCurSel();
	if(boneSel < 0){
       AfxMessageBox("Must select a bone...");
	   return;
	}

	if(!m_skeletonReferencePtr->m_fireQueList)
		return;


    UpdateData(TRUE);

    POSITION fPos = m_skeletonReferencePtr->m_fireQueList->FindIndex(fireQueSel);
	if(!fPos)
		return;
        CFireRangeObject * m_fireRange = (CFireRangeObject *)m_skeletonReferencePtr->m_fireQueList->GetAt(fPos);

	    m_fireRange->m_animationIndex           = animSel;
	    m_fireRange->m_boneIndex                = boneSel;
	    m_fireRange->m_animationFrameRangeEnd   = m_fireQueEndRange;
	    m_fireRange->m_animationFrameRangeStart = m_fireQueStartRange;
	    m_fireRange->m_missileIndex             = m_fireQueMissileIndex;
		m_fireRange->m_hardPointAdjust.z        = m_fireQueZlocalAdjust;
 	    m_fireRange->m_hardPointAdjust.x        = m_fireQueXpAdjust;
	    m_fireRange->m_hardPointAdjust.y        = m_fireQueYpAdjust;
		m_fireRange->m_accuracyRating           = m_baseAccuracy;
 	
	UpdateData(FALSE);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnSelchangeLISTFireQueDB() 
{
    
    int fireQueSel = m_fireQueDB.GetCurSel();
	if(fireQueSel < 0){
	   return;
	}

	if(!m_skeletonReferencePtr->m_fireQueList)
		return;


    UpdateData(TRUE);

    POSITION fPos = m_skeletonReferencePtr->m_fireQueList->FindIndex(fireQueSel);
	if(!fPos)
		return;
        CFireRangeObject * m_fireRange = (CFireRangeObject *)m_skeletonReferencePtr->m_fireQueList->GetAt(fPos);

	    m_animationsList.SetCurSel(m_fireRange->m_animationIndex);
		m_listBones.SetCurSel(m_fireRange->m_boneIndex);
	    m_fireQueEndRange             = m_fireRange->m_animationFrameRangeEnd;
	    m_fireQueStartRange           = m_fireRange->m_animationFrameRangeStart;
	    m_fireQueMissileIndex         = m_fireRange->m_missileIndex;

		m_fireQueZlocalAdjust         = m_fireRange->m_hardPointAdjust.z;
 	    m_fireQueXpAdjust             = m_fireRange->m_hardPointAdjust.x;
	    m_fireQueYpAdjust             = m_fireRange->m_hardPointAdjust.y;
		m_baseAccuracy                = m_fireRange->m_accuracyRating;

	UpdateData(FALSE);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::InListFireQues(int sel)
{
	m_fireQueDB.ResetContent();
    if(!m_skeletonReferencePtr->m_fireQueList)
		return;

    for(POSITION posLoc = m_skeletonReferencePtr->m_fireQueList->GetHeadPosition();posLoc != NULL;)
	{
        CFireRangeObject * fPtr = (CFireRangeObject *)m_skeletonReferencePtr->m_fireQueList->GetNext(posLoc);
        m_fireQueDB.AddString("Fire Que");
	}

	if(sel > -1)
	   m_fireQueDB.SetCurSel(sel);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnCHECKsecondaryOverridable() 
{
	int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   AfxMessageBox("Must select a bone");
       return;
	}

	POSITION posLoc = m_skeletonReferencePtr->m_bonesList->FindIndex(boneindex);
    if(!posLoc)
		return;
          CBoneObject * bn = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(posLoc);
		  if(bn->m_secondaryOverridable)
			 bn->m_secondaryOverridable = FALSE;
		  else
			 bn->m_secondaryOverridable = TRUE;
	
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnSelchangeLISTBones() 
{
	UpdateData(TRUE);
	m_secondaryOverridable.SetCheck(0);
    int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
       return;
	}

	if(!m_skeletonReferencePtr->m_bonesList)
		return;
	
	POSITION posLoc = m_skeletonReferencePtr->m_bonesList->FindIndex(boneindex);
    if(!posLoc)
		return;
          CBoneObject * bn = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(posLoc);
		  if(bn->m_secondaryOverridable)
			  m_secondaryOverridable.SetCheck(1);

		  D3DVECTOR tempBuild;
		  bn->m_boneFrame->GetPosition(NULL,&tempBuild);
		  
		  m_bonePosX = tempBuild.x;
	      m_bonePosY = tempBuild.y;
	      m_bonePosZ = tempBuild.z;

	UpdateData(FALSE);
		  
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONSetCenterOfMassLink() 
{
	int boneindex = m_listBones.GetCurSel();
	if(boneindex == -1){
	   AfxMessageBox("Must select a bone");
       return;
	}

    m_skeletonReferencePtr->m_secondaryCOMInheritBone = boneindex;
	AfxMessageBox("Done...");

}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONUnsetCenterOfMassLink() 
{ 
	m_skeletonReferencePtr->m_secondaryCOMInheritBone = -1;
	AfxMessageBox("Done...");
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONEditSoundEvent() 
{
UpdateData(TRUE);
    
    

 
	   int sndRangeSelection = m_listSoundEvents.GetCurSel();
	   if(sndRangeSelection == -1)
	   {
              AfxMessageBox("Must select a sound range");
              return;
	   }

	   CSoundRangeObject * sndRPtr = NULL;
	   int tracer = 0;
       for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	   {
          CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
	      if(bone->m_animationList)
			 for(POSITION posAnim = bone->m_animationList->GetHeadPosition();posAnim != NULL;)
			 {//animation list
		        CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetNext(posAnim);
                if(animPtr->m_soundRanges)
                for(POSITION soundPos = animPtr->m_soundRanges->GetHeadPosition();soundPos != NULL;)
				{//sound range loop
                      CSoundRangeObject * sndRngObj = (CSoundRangeObject *)animPtr->m_soundRanges->GetNext(soundPos);
					  if(tracer == sndRangeSelection){
				       if(!sndRPtr){
						 sndRPtr = sndRngObj;
                         break;
					   }
					  }
			     tracer++;
				}//end sound range loop
				if(sndRPtr)
				   break;
			 }
			 if(sndRPtr)
				break;
	   }
	   if(!sndRPtr){
           AfxMessageBox("cannot find sound range");
		   return;
	   }
		   
    CSoundRangeDlg dlg;
	   dlg.m_id				= sndRPtr->m_id;
	   dlg.m_clipRadius		= sndRPtr->m_radiusClipping;
	   dlg.m_soundFactor	= sndRPtr->m_soundFactor;
       dlg.m_startRange		= sndRPtr->m_startRange;
       dlg.m_endRange		= sndRPtr->m_endRange;
	   dlg.m_velocityBased	= sndRPtr->m_velocityBased;
	   dlg.m_looping		= sndRPtr->m_looping;

	if(dlg.DoModal()==IDOK){
	   sndRPtr->m_id               = dlg.m_id;
	   sndRPtr->m_radiusClipping   = dlg.m_clipRadius;
	   sndRPtr->m_soundFactor      = dlg.m_soundFactor;
	   sndRPtr->m_velocityBased    = dlg.m_velocityBased;
	   sndRPtr->m_looping          = dlg.m_looping;
	   sndRPtr->SetRanges(dlg.m_startRange,dlg.m_endRange);
       InListSoundEvents(sndRangeSelection);
	}		
UpdateData(FALSE);		
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CSkeletalAnimationDlg::OnBUTTONReplace() 
{
    int animSel = m_animationsList.GetCurSel();
	if(animSel < 0)
	{
       MessageBox("Must select a animation");
	   return;
	}

	UpdateData(TRUE);
	int type = -1;
	if(m_animationType == "WALK")
	   type = 3;
    if(m_animationType == "RUN")
	   type = 2;
    if(m_animationType == "STAND")
	   type = 1;
    if(m_animationType == "DIE")
       type = 8;
    if(m_animationType == "JUMP")
	   type = 5;
    if(m_animationType == "SIDESTEP LEFT")
	   type = 6;
    if(m_animationType == "SIDESTEP RIGHT")
	   type = 7;
	if(m_animationType == "CROUCH")
	   type = 11;
	if(m_animationType == "FIRE")
	   type = 12;
    if(m_animationType == "SWIM IDLE")
	   type = 14;
	if(m_animationType == "SWIM ACTION")
	   type = 15;
	if(m_animationType == "JET PACK")
	   type = 16;
	if(m_animationType == "PAIN")
	   type = 17;
    if(m_animationType == "TAUNT1")
	   type = 18;
	if(m_animationType == "TAUNT2")
	   type = 19;
	if(m_animationType == "TAUNT3")
	   type = 20;
	if(m_animationType == "DANCE1")
	   type = 21;
	if(m_animationType == "DANCE2")
	   type = 22;
	if(m_animationType == "DANCE3")
	   type = 23;
	if(m_animationType == "WAVE")
	   type = 24;
	if(m_animationType == "SALUTE")
	   type = 25;
	if(m_animationType == "BOW")
	   type = 26;
	if(m_animationType == "LAUGH")
	   type = 27;
	if(m_animationType == "SIT")
	   type = 28;
	if(m_animationType == "CROUCH")
	   type = 29;
	if(m_animationType == "MOUNT")
	   type = 30;
	if(m_animationType == "CAST1")
	   type = 31;
	if(m_animationType == "CAST2")
	   type = 32;
	if(m_animationType == "CAST3")
	   type = 33;
   
    if(type == -1)
	   return;




	if(m_fileName.GetLength()==0)
	{
      if(m_skeletonReferencePtr->m_bonesList)
	  {//bones list valid
	   POSITION bonePos = m_skeletonReferencePtr->m_bonesList->FindIndex(0);
	   if(bonePos)
	   {//valid bone position
	      CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetAt(bonePos);
		  
          int tracer = 0;
		  if(bone->m_animationList)
		  {
             POSITION animPos = bone->m_animationList->FindIndex(animSel);
			 if(animPos)
			 {
		         CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetNext(animPos);
				 animPtr->m_type = type;
			 }
		  }
	   }
	  }
	  return;
	}

    //use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization

	if(m_skeletonReferencePtr->LoadAnimation(m_fileName,m_loopableState,type,m_requiredDuration,animSel)==FALSE)
	   MessageBox("Failed to load animation file onto skeletal object");
	   AnimationInlist(animSel);

	 SetCurrentDirectory(MainDir);
	UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBnClickedCheckPubliccfg()
{
	int meshindex = m_listDeformableMeshs.GetCurSel();
	if(meshindex == -1){
	   MessageBox("select a deformable mesh");
       return;
	}

	int dcfgIndex = m_deformableCfgs.GetCurSel();
	if(dcfgIndex == -1){
	   MessageBox("select a deformable mesh cfg");
       return;
	}

    if(!m_skeletonReferencePtr->m_alternateConfigurations)
		 return;
	POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(meshindex);
    if(!alPos)
	     return;	
	CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);
    POSITION posLoc = alPtr->m_configurations->FindIndex(dcfgIndex);
	if(!posLoc)
	     return;
    CDeformableMesh * dMesh = (CDeformableMesh *) alPtr->m_configurations->GetAt(posLoc);
    UpdateData(TRUE);
    dMesh->m_publicDim = m_checkPublicCfg.GetCheck();
    UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnLbnSelchangeListDeformablecfgs()
{
	int meshindex = m_listDeformableMeshs.GetCurSel();
	if(meshindex == -1){
	  
       return;
	}

	int dcfgIndex = m_deformableCfgs.GetCurSel();
	if(dcfgIndex == -1){
	   
       return;
	}

    if(!m_skeletonReferencePtr->m_alternateConfigurations)
		 return;
	POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(meshindex);
    if(!alPos)
	     return;	
	CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);
    POSITION posLoc = alPtr->m_configurations->FindIndex(dcfgIndex);
	if(!posLoc)
	     return;
    CDeformableMesh * dMesh = (CDeformableMesh *) alPtr->m_configurations->GetAt(posLoc);
	m_checkPublicCfg.SetCheck(dMesh->m_publicDim);
}

void CSkeletalAnimationDlg::OnBnClickedButtonsetlodset()
{
    int dcfgIndex = m_deformableCfgs.GetCurSel();
	if(dcfgIndex == -1){
	   AfxMessageBox("Must select a dim cfg for index purpose");
       return;
	}


	//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
     //end use mapsmodels folder for organization
     static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_fileName = opendialog.GetPathName();//GetFileName();
	   
	   m_skeletonReferencePtr->LoadLod(m_fileName,dcfgIndex);
	   
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CSkeletalAnimationDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
     m_skeletonReferencePtr->m_soundFactor		= m_sndSoundFactor;
     m_skeletonReferencePtr->m_clipRadius		= m_clipRadius;
	 m_skeletonReferencePtr->m_sndID			= m_sndSoundID;
	 m_skeletonReferencePtr->m_sndVelocityBased	= m_sndVelocityBased.GetCheck();
	 m_skeletonReferencePtr->m_sndVelocityMaxDist = m_maxVelocityDist;
	 m_skeletonReferencePtr->m_startFrequency   = m_startFreq;

    UpdateData(FALSE);
	OnOK();
}

void CSkeletalAnimationDlg::InListVisibilityTracks(int sel)
{
	m_visibilityTrackList.ResetContent();
    if(!m_skeletonReferencePtr->m_visibilityTrackList)
		return;

    for(POSITION posLoc = m_skeletonReferencePtr->m_visibilityTrackList->GetHeadPosition();posLoc != NULL;)
	{
        CFireRangeObject * fPtr = (CFireRangeObject *)m_skeletonReferencePtr->m_visibilityTrackList->GetNext(posLoc);
        m_visibilityTrackList.AddString("Vis-Track");
	}

	if(sel > -1)
	   m_visibilityTrackList.SetCurSel(sel);
}

void CSkeletalAnimationDlg::OnBnClickedBtnVisibilitytrackadd()
{
	UpdateData(TRUE);

    if(!m_skeletonReferencePtr->m_visibilityTrackList)
        m_skeletonReferencePtr->m_visibilityTrackList = new CFireRangeObjectList();

	CFireRangeObject * newFireRange = new CFireRangeObject();
    newFireRange->m_animationFrameRangeStart	= m_visibilityTrackStartRange;
	newFireRange->m_animationFrameRangeEnd		= m_visibilityTrackEndRange;
	newFireRange->m_animationIndex				= m_visibilityTrackAnimationID;

	m_skeletonReferencePtr->m_visibilityTrackList->AddTail(newFireRange);

	UpdateData(FALSE);

	InListVisibilityTracks(m_skeletonReferencePtr->m_visibilityTrackList->GetCount()-1);
}

void CSkeletalAnimationDlg::OnBnClickedBtnvisibilitytrackreplace()
{
	int sel = m_visibilityTrackList.GetCurSel();
	if(sel < 0)
	{
	   AfxMessageBox("Must select a visibility track");
	   return;
	}

	if(!m_skeletonReferencePtr->m_visibilityTrackList)
	    return;
	
	
	
	POSITION visPos = m_skeletonReferencePtr->m_visibilityTrackList->FindIndex(sel);
	if(!visPos)
		return;
        CFireRangeObject * vPtr = (CFireRangeObject *)m_skeletonReferencePtr->m_visibilityTrackList->GetAt(visPos);
		
		UpdateData(TRUE);

		vPtr->m_animationFrameRangeStart	= m_visibilityTrackStartRange;
	    vPtr->m_animationFrameRangeEnd		= m_visibilityTrackEndRange;
	    vPtr->m_animationIndex				= m_visibilityTrackAnimationID;

		UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBnClickedBtnvisibilitytrackdelete()
{
	int sel = m_visibilityTrackList.GetCurSel();
	if(sel < 0)
	{
	   AfxMessageBox("Must select a visibility track");
	   return;
	}

	if(!m_skeletonReferencePtr->m_visibilityTrackList)
	    return;
	
	
	
	POSITION visPos = m_skeletonReferencePtr->m_visibilityTrackList->FindIndex(sel);
	if(!visPos)
		return;
        CFireRangeObject * vPtr = (CFireRangeObject *)m_skeletonReferencePtr->m_visibilityTrackList->GetAt(visPos);
		delete vPtr;
		vPtr = 0;
		m_skeletonReferencePtr->m_visibilityTrackList->RemoveAt(visPos);
		InListVisibilityTracks(sel-1);
		if(m_skeletonReferencePtr->m_visibilityTrackList->GetCount()==0)
		{
           delete m_skeletonReferencePtr->m_visibilityTrackList;
		   m_skeletonReferencePtr->m_visibilityTrackList = 0;
		}
		      
}

void CSkeletalAnimationDlg::OnLbnSelchangeListvisibilitytrack()
{
	int sel = m_visibilityTrackList.GetCurSel();
	if(sel < 0)
	   return;

	if(!m_skeletonReferencePtr->m_visibilityTrackList)
	    return;
	
	
	
	POSITION visPos = m_skeletonReferencePtr->m_visibilityTrackList->FindIndex(sel);
	if(!visPos)
		return;
        CFireRangeObject * vPtr = (CFireRangeObject *)m_skeletonReferencePtr->m_visibilityTrackList->GetAt(visPos);
        
        UpdateData(TRUE);

		m_visibilityTrackStartRange			= vPtr->m_animationFrameRangeStart;
	    m_visibilityTrackEndRange			= vPtr->m_animationFrameRangeEnd;
	    m_visibilityTrackAnimationID		= vPtr->m_animationIndex;

		UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBnClickedButtonapplyallpublicstate()
{
	if(AfxMessageBox("Are you sure you want apply this public state to all dims ?",MB_OKCANCEL,0)!=IDOK)
	   return;


    if(!m_skeletonReferencePtr->m_alternateConfigurations)
		 return;
	
	UpdateData(TRUE);
	BOOL currentState = m_checkPublicCfg.GetCheck();
	UpdateData(FALSE);
	for(POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->GetHeadPosition();alPos != NULL;)
	{
	    CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);
        for(POSITION posLoc = alPtr->m_configurations->GetHeadPosition();posLoc != NULL;)
		{
            CDeformableMesh * dMesh = (CDeformableMesh *) alPtr->m_configurations->GetNext(posLoc);
            dMesh->m_publicDim = currentState;
		}
	}
}

void CSkeletalAnimationDlg::OnLbnSelchangeListHiarchmeshlist()
{
	int meshindex = m_listHiarchlMeshs.GetCurSel();
	if(meshindex == -1){
       return;
	}
	UpdateData(TRUE);
	int trace = 0;
	if(m_skeletonReferencePtr->m_bonesList)
	  for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	  {
          CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
          if(!bone->m_hiarchyMesh)continue;

			  if(trace == meshindex)
			  {
			     if(bone->m_hiarchyMesh->m_collisionFiltered)
				    m_checkHMeshCollisionFilter.SetCheck(1);
				 else
				    m_checkHMeshCollisionFilter.SetCheck(0);
				 break;
			  }
		      trace++;
	  }
	UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBnClickedCheckHmeshcollisionfilter()
{
	int meshindex = m_listHiarchlMeshs.GetCurSel();
	if(meshindex == -1){
       return;
	}
	UpdateData(TRUE);
	int trace = 0;
	if(m_skeletonReferencePtr->m_bonesList)
	  for(POSITION posLoc = m_skeletonReferencePtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	  {
          CBoneObject * bone = (CBoneObject *)m_skeletonReferencePtr->m_bonesList->GetNext(posLoc);
          if(!bone->m_hiarchyMesh)continue;

		  if(trace == meshindex){
             if(m_checkHMeshCollisionFilter.GetCheck())
			    bone->m_hiarchyMesh->m_collisionFiltered = true;
			 else
                bone->m_hiarchyMesh->m_collisionFiltered = false;
			 break;
		  }
		  trace++;
	  }
	UpdateData(FALSE);
}

void CSkeletalAnimationDlg::OnBnClickedCheckDmeshcollisionfilter()
{
	int meshindex = m_listDeformableMeshs.GetCurSel();
	if(meshindex == -1){
	   MessageBox("select a deformable mesh");
       return;
	}

	
	
    POSITION alPos = m_skeletonReferencePtr->m_alternateConfigurations->FindIndex(meshindex);
	if(!alPos)
	    return;	
	    CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)m_skeletonReferencePtr->m_alternateConfigurations->GetNext(alPos);

	UpdateData(TRUE);
			 for(POSITION posLoc = alPtr->m_configurations->GetHeadPosition();posLoc != NULL;)
			 {
                 CDeformableMesh * mPtr = (CDeformableMesh *) alPtr->m_configurations->GetNext(posLoc);
				 mPtr->m_collisionFiltered = m_checkDMeshCollisionFilter.GetCheck();
				 return;
			 }
    UpdateData(FALSE);
}

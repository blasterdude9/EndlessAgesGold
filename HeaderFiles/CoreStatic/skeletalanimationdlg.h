#include "CSkeletalClass.h"
#include "ctextureDBClass.h"
#include "CElementsClass.h"
#include "CSkillClass.h"
#include "CStateManagementClass.h"
#include <algorithm>
#pragma once

// SkeletalAnimationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkeletalAnimationDlg dialog

class CSkeletalAnimationDlg : public CDialog
{

    //DECLARE_DYNAMIC(CSkeletalAnimationDlg)

// Construction
public:
	CSkeletalAnimationDlg(CWnd* pParent = NULL);   // standard constructor
	//virtual ~CSkeletalAnimationDlg();
   
	CSkeletonObject * m_skeletonReferencePtr;
	IDirectSound8* directSoundRef;
	BOOL m_loopableState;
	CTextureDBObjList * GL_textureDataBase;
	CElementObjList * m_elementDBREF;
	CSkillObjectList * m_skillDatabaseREF;
	CGlobalMaterialShaderList * m_globalShaderRef;
	CStateManagementObj * m_renderStateObjectRef;
	enum { IDD = IDD_DIALOG_SkeletalAnimationEditor };

	CComboBox	m_dmpSkillTypeUse;
	CComboBox	m_elementsAvailable;
	CButton	m_secondaryOverridable;
	CListBox	m_fireQueDB;
	CListBox	m_deformableCfgs;
	CButton	m_emitPtUseRange;
	CListBox	m_listParticleEvents;
	CListBox	m_dPathList;
	CListBox	m_listBDCSpheres;
	CComboBox	m_csBoneSel;
	CButton	m_checkEnableSphereCalc;
	CListBox	m_listDamageSpheres;
	CListBox	m_listSoundEvents;
	CListBox	m_listHiarchlMeshs;
	CListBox	m_listDeformableMeshs;
	CListBox	m_listBones;
	CListBox	m_animationsList;
	CButton	m_checkHMeshCollisionFilter;
	CButton	m_checkDMeshCollisionFilter;
	CButton	m_checkLoopable;
	CString	m_animationType;
	CString	m_fileName;
	int		m_requiredDuration;
	int		m_animVersion;
	float	m_damageSphereRadius;
	float	m_damageShphereAmplification;
	float	m_sphereCalcRadius;
	float	m_bdSphereRadius;
	CString	m_dsphereComment;
	float	m_dPathRadius;
	int		m_dPathMinValue;
	int		m_dPathMaxValue;
	int		m_dPathDotValue;
	int		m_dPathAnimationTrace;
	int		m_ptStartRange;
	int		m_particleLink;
	int		m_ptEndRange;
	int		m_curCfgDefSpawn;
	int		m_fireQueStartRange;
	int		m_fireQueEndRange;
	int		m_fireQueMissileIndex;
	float	m_fireQueZlocalAdjust;
	float	m_fireQueXpAdjust;
	float	m_fireQueYpAdjust;
	long	m_fps;
	float	m_bonePosX;
	float	m_bonePosY;
	float	m_bonePosZ;
	int		m_damageSpecific;
    CButton m_checkPublicCfg;

	CListBox	m_visibilityTrackList;
	int         m_visibilityTrackStartRange;
	int         m_visibilityTrackEndRange;
	int         m_visibilityTrackAnimationID;


	float m_baseAccuracy;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
    public:
	//CDeformableMesh* GetSelectedDeformableConfiguration();
	void InListVisibilityTracks(int sel);
    void AnimationInlist(int sel);
	void InlistBones();
	void InListSoundEvents(int sel);
	CString ConvertLongToString(int number);
    void InListDeformableMesh();
	void InListHiarchleMesh();
	void InListDamageSpheres(int sel);
	CString FloatToStingCnv(float number);
	void InListBDCSpheres(int sel);
	void InListDPathList(int sel);
	void InListParticleEventsList(int sel);
	void InListCfgs(int sel);
	void InListFireQues(int sel);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnCheckLoopable();
	afx_msg void OnSelchangeListAnimations();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonSetSoundRange();
	afx_msg void OnBUTTONDeleteSoundEvent();
	afx_msg void OnButtonSetboneFocus1();
	afx_msg void OnBUTTONDeformableMaterial();
	afx_msg void OnBUTTONHiarchleMaterial();
	afx_msg void OnBUTTONReplaceValues();
	afx_msg void OnBUTTONAddLSphere();
	afx_msg void OnBUTTONDeleteLSphere();
	afx_msg void OnBUTTONReplaceLCSphere();
	afx_msg void OnSelchangeLISTLocationSpheres();
	afx_msg void OnChangeEDITsphereCalcRadius();
	afx_msg void OnCHECKEnableSphereCollisionSys();
	afx_msg void OnSelchangeCOMBOCSBoneSel();
	afx_msg void OnBUTTONReplaceBDSphere();
	afx_msg void OnBUTTONDeleteBDSphere();
	afx_msg void OnBUTTONAddBDSphere();
	afx_msg void OnSelchangeLISTBodyColLocationSpheres();
	afx_msg void OnBUTTONdPathAdd();
	afx_msg void OnBUTTONdPathDelete();
	afx_msg void OnBUTTONdPathReplace();
	afx_msg void OnSelchangeLISTDamagePathTrackList();
	afx_msg void OnBUTTONaddParticleEvent();
	afx_msg void OnBUTTONdeleteParticleEvent();
	afx_msg void OnBUTTONreplaceParticleEvent();
	afx_msg void OnSelchangeLISTParticleEvents();
	afx_msg void OnCHECKUseRange();
	afx_msg void OnBUTTONunsetBoneLook();
	afx_msg void OnBUTTONunsetSideStep();
	afx_msg void OnBUTTONSETBONESetSideStep();
	afx_msg void OnBUTTONcharona();
	afx_msg void OnBUTTONset();
	afx_msg void OnBUTTONlodHiarchy();
	afx_msg void OnBUTTONaddDeformableCfg();
	afx_msg void OnBUTTONsetLodDefCfg();
	afx_msg void OnBUTTONDeformableCfgMaterial();
	afx_msg void OnSelchangeListDeformablemeshlist();
	afx_msg void OnDeltaposSPINdefSpawnCfg(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBUTTONdeleteDeformableCfg();
	afx_msg void OnBTNAddFireQue();
	afx_msg void OnBTNDeleteFireQue();
	afx_msg void OnBTNEditFireQue();
	afx_msg void OnSelchangeLISTFireQueDB();
	afx_msg void OnCHECKsecondaryOverridable();
	afx_msg void OnSelchangeLISTBones();
	afx_msg void OnBUTTONSetCenterOfMassLink();
	afx_msg void OnBUTTONUnsetCenterOfMassLink();
	afx_msg void OnBUTTONEditSoundEvent();
	afx_msg void OnBUTTONReplace();
	afx_msg void OnBnClickedCheckPubliccfg();
	afx_msg void OnLbnSelchangeListDeformablecfgs();
	afx_msg void OnBnClickedButtonsetlodset();
	DECLARE_MESSAGE_MAP()
public:
	float m_sndSoundFactor;
	float m_clipRadius;
	int m_sndSoundID;
	CButton m_sndVelocityBased;
	afx_msg void OnBnClickedOk();
	float m_maxVelocityDist;
	float m_startFreq;
public:
	afx_msg void OnBnClickedBtnVisibilitytrackadd();
public:
	afx_msg void OnBnClickedBtnvisibilitytrackreplace();
public:
	afx_msg void OnBnClickedBtnvisibilitytrackdelete();
public:
	afx_msg void OnLbnSelchangeListvisibilitytrack();
public:
	afx_msg void OnBnClickedButtonapplyallpublicstate();
public:
	afx_msg void OnLbnSelchangeListHiarchmeshlist();
public:
	afx_msg void OnBnClickedCheckHmeshcollisionfilter();
public:
	afx_msg void OnBnClickedCheckDmeshcollisionfilter();
};



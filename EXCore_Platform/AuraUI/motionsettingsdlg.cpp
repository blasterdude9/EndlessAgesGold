// MotionSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"

#include "MotionSettingsDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotionSettingsDlg dialog


CMotionSettingsDlg::CMotionSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotionSettingsDlg::IDD, pParent)
	, m_groundGripEdit(0)
	, m_inertiaEdit(0)
	, m_crnStiffnessR(0)
	, m_crnStiffnessF(0)
	, m_maxGripEdit(0)
	, m_aeroDynamicsEdit(0)
	, m_autoLevelTol(0)
{
	//{{AFX_DATA_INIT(CMotionSettingsDlg)
	m_accelerate = 0.0f;
	m_decellerate = 0.0f;
	m_maxPitch = 0.0f;
	m_maxSpeed = 0.0f;
	m_maxYaw = 0.0f;
	m_minSpeed = 0.0f;
	m_pitchAccel = 0.0f;
	m_pitchDecel = 0.0f;
	m_yawAccel = 0.0f;
	m_yawDecel = 0.0f;
	m_mouseRestriction = 0.0f;
	m_jumpPower = 0.0f;
	m_freeLookMaxPitch = 0.0f;
	m_freeLookMaxYaw = 0.0f;
	m_freeLookSpeed = 0.0f;
	m_mouseSensitivity = 0.0f;
	m_mass = 0.0f;
	m_fovSensitivity = 0.0f;
	m_maxFovSetting = 0.0f;
	m_minFovSetting = 0.0f;
	m_rollAccel = 0.0f;
	m_rollDecel = 0.0f;
	m_maxRoll = 0.0f;
	m_submergedHieght = 0.0f;
	m_stepTolerance = 0.0f;
	m_strafeDegrade = 0.0f;
	//}}AFX_DATA_INIT
}


void CMotionSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotionSettingsDlg)

	DDX_Control(pDX, IDC_CHECK_pkSysSwitchAllowed, m_pkSys_userConfigurable);
	DDX_Control(pDX, IDC_COMBO_pkSysFirstPersonMode, m_pkSys_firstPersonMode);
	DDX_Control(pDX, IDC_COMBO_pkSysThirdPersonMode, m_pkSys_thirdPersonMode);
	DDX_Control(pDX, IDC_COMBO_pkSysAiPhyModel, m_pkSys_aiCfg);
   

	DDX_Control(pDX, IDC_CHECK_EntityFloatsWhenDead, m_checkFloatWhenDead);
	DDX_Control(pDX, IDC_SLIDER_FluidControl, m_fluidControl);
	DDX_Control(pDX, IDC_SLIDER_AutoLevelXStrength, m_autoLevelXStrength);
	DDX_Control(pDX, IDC_SLIDER_AutoLevelZStrength, m_autoLevelZStrength);
	DDX_Control(pDX, IDC_CHECK_AutoLevelingX, m_autoLevelingX);
	DDX_Control(pDX, IDC_CHECK_AutoLevelingZ, m_autoLevelingZ);
	DDX_Control(pDX, IDC_SLIDER_RotationDragOnGround, m_sliderRotationDragOnGround);
	DDX_Control(pDX, IDC_SLIDER_RotationDragConstant, m_sliderRotationDragConst);
	DDX_Control(pDX, IDC_SLIDER_AIRCONTROL, m_sliderAirControl);
	DDX_Control(pDX, IDC_CHECK_AntiGravity, m_antiGravity);
	DDX_Control(pDX, IDC_SLIDER_AERODYNAMIC, m_sliderAeroDynamics);
	DDX_Control(pDX, IDC_SLIDER_GROUNDGRIP, m_sliderGroundGrip);
	DDX_Control(pDX, IDC_RADIO_AutoDecelFcZ, m_autoDecelForceZ);
	DDX_Control(pDX, IDC_RADIO_AutoDecelFcY, m_autoDecelForceY);
	DDX_Control(pDX, IDC_CHECK_JointMouseFocus, m_checkJointMouseFocus);
	DDX_Control(pDX, IDC_RADIO_AUTODECEL, m_radioAutoDecel);
	DDX_Text(pDX, IDC_EDIT_ACCELERATE, m_accelerate);
	DDX_Text(pDX, IDC_EDIT_DECELLERATE, m_decellerate);
	DDX_Text(pDX, IDC_EDIT_MAXPITCH, m_maxPitch);
	DDX_Text(pDX, IDC_EDIT_MAXSPEED, m_maxSpeed);
	DDX_Text(pDX, IDC_EDIT_MAXYAW, m_maxYaw);
	DDX_Text(pDX, IDC_EDIT_MINSPEED, m_minSpeed);
	DDX_Text(pDX, IDC_EDIT_PITCHACCEL, m_pitchAccel);
	DDX_Text(pDX, IDC_EDIT_PITCHDECEL, m_pitchDecel);
	DDX_Text(pDX, IDC_EDIT_YAWACCEL, m_yawAccel);
	DDX_Text(pDX, IDC_EDIT_YAWDECEL, m_yawDecel);
	DDX_Text(pDX, IDC_EDIT_MouseRestriction, m_mouseRestriction);
	DDX_Text(pDX, IDC_EDIT_JumpPower, m_jumpPower);
	DDX_Text(pDX, IDC_EDIT_freeLookMaxPitch, m_freeLookMaxPitch);
	DDX_Text(pDX, IDC_EDIT_freeLookmaxYaw, m_freeLookMaxYaw);
	DDX_Text(pDX, IDC_EDIT_freeLookSpeed, m_freeLookSpeed);
	DDX_Text(pDX, IDC_EDIT_MouseSensitivity, m_mouseSensitivity);
	DDX_Text(pDX, IDC_EDIT_Mass, m_mass);
	DDX_Text(pDX, IDC_EDIT_FovSensitivity, m_fovSensitivity);
	DDX_Text(pDX, IDC_EDIT_MaxFovSetting, m_maxFovSetting);
	DDX_Text(pDX, IDC_EDIT_MinFovSetting, m_minFovSetting);
	DDX_Text(pDX, IDC_EDIT_ROLLACCEL, m_rollAccel);
	DDX_Text(pDX, IDC_EDIT_ROLLDECEL, m_rollDecel);
	DDX_Text(pDX, IDC_EDIT_MAXROLL, m_maxRoll);
	DDX_Text(pDX, IDC_EDIT_SubmergedHieght, m_submergedHieght);
	DDX_Text(pDX, IDC_EDIT_stepTolerance, m_stepTolerance);
	DDX_Text(pDX, IDC_EDIT_strafeDegrade, m_strafeDegrade);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_jumpRecovery, m_jumpRecoveryDur);

	DDX_Text(pDX, IDC_EDIT_groundDragEdit, m_groundGripEdit);
	DDX_Text(pDX, IDC_EDIT_Inertia, m_inertiaEdit);
	DDX_Text(pDX, IDC_EDIT_CorneringStiffnessR, m_crnStiffnessR);
	DDX_Text(pDX, IDC_EDIT_corneringStiffnessF, m_crnStiffnessF);
	DDX_Text(pDX, IDC_EDIT_MaxGrip, m_maxGripEdit);
	DDX_Text(pDX, IDC_EDIT_AirDynamicEdit, m_aeroDynamicsEdit);
	DDX_Text(pDX, IDC_EDIT_AutoLevelTol, m_autoLevelTol);
}


BEGIN_MESSAGE_MAP(CMotionSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CMotionSettingsDlg)
	ON_BN_CLICKED(IDC_LOAD_CONFIG, OnLoadConfig)
	ON_BN_CLICKED(IDC_SAVE_CONFIG, OnSaveConfig)
	ON_BN_CLICKED(IDC_RADIO_AUTODECEL, OnRadioAutodecel)
	ON_BN_CLICKED(IDC_CHECK_JointMouseFocus, OnCHECKJointMouseFocus)
	ON_BN_CLICKED(IDC_RADIO_AutoDecelFcY, OnRADIOAutoDecelFcY)
	ON_BN_CLICKED(IDC_RADIO_AutoDecelFcZ, OnRADIOAutoDecelFcZ)
	ON_BN_CLICKED(IDC_CHECK_AntiGravity, OnCHECKAntiGravity)
	ON_BN_CLICKED(IDC_CHECK_AutoLevelingX, OnCHECKAutoLevelingX)
	ON_BN_CLICKED(IDC_CHECK_AutoLevelingZ, OnCHECKAutoLevelingZ)
	ON_BN_CLICKED(IDC_CHECK_EntityFloatsWhenDead, OnCHECKEntityFloatsWhenDead)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotionSettingsDlg message handlers

BOOL CMotionSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	m_sliderAeroDynamics.SetRange(0,1000,TRUE);
	m_sliderGroundGrip.SetRange(0,1000,TRUE);
	m_sliderAirControl.SetRange(0,1000,TRUE);
	m_sliderRotationDragOnGround.SetRange(0,1000,TRUE);
	m_sliderRotationDragConst.SetRange(0,1000,TRUE);
	m_fluidControl.SetRange(0,1000,TRUE);
	m_autoLevelXStrength.SetRange(0,100,TRUE);
	m_autoLevelZStrength.SetRange(0,100,TRUE);
	UpdateData(FALSE);
	
	
	
	Refresh();
    
    //fill in lists
    m_pkSys_userConfigurable.SetCheck(movementDB->m_pckSys_changeable);
	m_pkSys_firstPersonMode.AddString("Realtime control");
	m_pkSys_firstPersonMode.AddString("Point/Click control");
	m_pkSys_firstPersonMode.SetCurSel(movementDB->m_pckSys_firstPersonMode);
	m_pkSys_thirdPersonMode.AddString("Realtime control");
	m_pkSys_thirdPersonMode.AddString("Point/Click control");
	m_pkSys_thirdPersonMode.SetCurSel(movementDB->m_pckSys_thirdPersonMode);
	
	if(m_aiListRef)
	{
		for(POSITION posLoc = m_aiListRef->GetHeadPosition();posLoc != NULL;)
		{
			CAIObj * aiPtr = (CAIObj *)m_aiListRef->GetNext(posLoc);
			m_pkSys_aiCfg.AddString(aiPtr->m_aiCfgName);
		}
	}
	m_pkSys_aiCfg.SetCurSel(movementDB->m_pckSys_aiControlModel);
    

    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMotionSettingsDlg::OnLoadConfig() 
{
	//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization
	UpdateData(TRUE);
    static char BASED_CODE filter[] = "Movement Data|*.MDB|All Files (*.*)|*.*||";
	CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
	  if ( opendialog.DoModal() == IDOK )
	  {
		CString FileName = opendialog.GetFileName();
		CFileException exc;
        CFile fl;
        fl.Open(FileName,CFile::modeRead, &exc);
        CArchive the_in_Archive(&fl,CArchive::load);
	    the_in_Archive >> movementDB;
	    the_in_Archive.Close();
        fl.Close();
	  }
	  UpdateData(FALSE);
	  Refresh();
      SetCurrentDirectory(MainDir);
}

void CMotionSettingsDlg::OnSaveConfig() 
{
	//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization
	 UpdateData(TRUE);
     static char BASED_CODE filter[] = "Movement Data|*.MDB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "MDB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CFileException exc;
       CFile fl;
	   CString FileName = opendialog.GetFileName();
       fl.Open(FileName,CFile::modeCreate | CFile::modeWrite, &exc);
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << movementDB;
		 the_out_Archive.Close();
         fl.Close();
	 }
	 UpdateData(FALSE);
	 SetCurrentDirectory(MainDir);
}

void CMotionSettingsDlg::OnOK() 
{
	UpdateData(TRUE);
	movementDB->List_freeLookCurMaxPitch = m_freeLookMaxPitch;
	movementDB->List_freeLookCurMaxYaw = m_freeLookMaxYaw;
	movementDB->List_freeLookSpeed = m_freeLookSpeed;
	movementDB->List_accellerate = m_accelerate;
	movementDB->List_decellerate = m_decellerate;
	movementDB->List_maxPitch = m_maxPitch;
	movementDB->List_maxSpeed = m_maxSpeed;
	movementDB->List_maxYaw = m_maxYaw;
	movementDB->List_minSpeed = m_minSpeed;
	movementDB->List_pitchAccel = m_pitchAccel;
	movementDB->List_pitchDecel = m_pitchDecel;
	movementDB->List_yawAccel = m_yawAccel;
	movementDB->List_yawDecel = m_yawDecel;
	movementDB->List_mouseRestriction = m_mouseRestriction; 
	movementDB->List_jumpPower = m_jumpPower;
	movementDB->List_mouseSensitivity = m_mouseSensitivity;
	movementDB->List_mass = m_mass;
	movementDB->List_capFovChangeSensitivity = m_fovSensitivity;
	movementDB->List_capabilityFovMax = m_maxFovSetting;
	movementDB->List_capabilityFovMin = m_minFovSetting;
	movementDB->m_strafeReduction = m_strafeDegrade;

	movementDB->m_autoLevelTolerance = m_autoLevelTol;

	movementDB->m_maxRoll             = m_maxRoll;
	movementDB->m_rollAccel           = m_rollAccel;
	movementDB->m_rollDecel           = m_rollDecel;
	movementDB->m_submergedHieght     = m_submergedHieght;
	movementDB->m_rampTolerance       = m_stepTolerance;
	movementDB->m_jumpRecoverTime     = m_jumpRecoveryDur;

	movementDB->m_inertia             = m_inertiaEdit;
	movementDB->m_corneringStiffnessR = m_crnStiffnessR;
	movementDB->m_corneringStiffnessF = m_crnStiffnessF;
	movementDB->m_maxGrip             = m_maxGripEdit;


	movementDB->m_crossSectionalArea    = m_aeroDynamicsEdit;//movementDB->m_crossSectionalArea    = ((float)m_sliderAeroDynamics.GetPos())*.001f;
	movementDB->List_groundDrag         = m_groundGripEdit;//((float)m_sliderGroundGrip.GetPos())*.001f;
	movementDB->m_airControl            = ((float)m_sliderAirControl.GetPos())*.001f;
	movementDB->m_rotationalGroundDrag  = ((float)m_sliderRotationDragOnGround.GetPos())*.001f;
	movementDB->m_rotationDragConstant  = ((float)m_sliderRotationDragConst.GetPos())*.001f;
	movementDB->m_autoLevelXStrength    = ((float)m_autoLevelXStrength.GetPos())*.001f;
	movementDB->m_autoLevelZStrength    = ((float)m_autoLevelZStrength.GetPos())*.001f;
	movementDB->m_inLiquidMotionModifier= ((float)m_fluidControl.GetPos())*.001f;



	movementDB->m_pckSys_changeable = m_pkSys_userConfigurable.GetCheck();
	movementDB->m_pckSys_firstPersonMode = m_pkSys_firstPersonMode.GetCurSel();
	movementDB->m_pckSys_thirdPersonMode = m_pkSys_thirdPersonMode.GetCurSel();
	movementDB->m_pckSys_aiControlModel = m_pkSys_aiCfg.GetCurSel();
	
	
	
	UpdateData(FALSE);
	
	CDialog::OnOK();
}

void CMotionSettingsDlg::OnRadioAutodecel() 
{
    if(movementDB->List_autoDecel == TRUE){
	   movementDB->List_autoDecel=FALSE;
       m_radioAutoDecel.SetCheck(0);
	}
	else{
       movementDB->List_autoDecel=TRUE;
       m_radioAutoDecel.SetCheck(1);
	}	
}


void CMotionSettingsDlg::Refresh()
{
UpdateData(TRUE);

    m_freeLookMaxPitch = movementDB->List_freeLookCurMaxPitch;
	m_freeLookMaxYaw = movementDB->List_freeLookCurMaxYaw;
	m_freeLookSpeed = movementDB->List_freeLookSpeed;
	m_accelerate = movementDB->List_accellerate;
	m_decellerate = movementDB->List_decellerate;
	m_maxPitch = movementDB->List_maxPitch;
	m_maxSpeed = movementDB->List_maxSpeed;
	m_maxYaw = movementDB->List_maxYaw;
	m_minSpeed = movementDB->List_minSpeed;
	m_pitchAccel = movementDB->List_pitchAccel;
	m_pitchDecel = movementDB->List_pitchDecel;
	m_yawAccel = movementDB->List_yawAccel;
	m_yawDecel = movementDB->List_yawDecel;
	m_mouseRestriction = movementDB->List_mouseRestriction; 
	m_jumpPower = movementDB->List_jumpPower;
	m_mouseSensitivity = movementDB->List_mouseSensitivity;
	m_mass = movementDB->List_mass;
	m_fovSensitivity = movementDB->List_capFovChangeSensitivity;
	m_maxFovSetting = movementDB->List_capabilityFovMax;
	m_minFovSetting = movementDB->List_capabilityFovMin;
	m_submergedHieght = movementDB->m_submergedHieght;
    m_stepTolerance = movementDB->m_rampTolerance;
	m_strafeDegrade			= movementDB->m_strafeReduction;

	m_autoLevelTol = movementDB->m_autoLevelTolerance;

	m_inertiaEdit			= movementDB->m_inertia;
	m_crnStiffnessR			= movementDB->m_corneringStiffnessR;
	m_crnStiffnessF			= movementDB->m_corneringStiffnessF;
	m_maxGripEdit           = movementDB->m_maxGrip;
	
	m_jumpRecoveryDur		= movementDB->m_jumpRecoverTime;

    m_maxRoll				= movementDB->m_maxRoll;
	m_rollAccel				= movementDB->m_rollAccel;
	m_rollDecel				= movementDB->m_rollDecel;


    if(movementDB->m_antiGravity)
	   m_antiGravity.SetCheck(1);  

	if(movementDB->List_autoDecel == TRUE)
       m_radioAutoDecel.SetCheck(1);
	else
       m_radioAutoDecel.SetCheck(0);

    if(movementDB->List_autoDecelY == TRUE)
       m_autoDecelForceY.SetCheck(1);
	else
       m_autoDecelForceY.SetCheck(0);

	if(movementDB->List_autoDecelZ == TRUE)
       m_autoDecelForceZ.SetCheck(1);
	else
       m_autoDecelForceZ.SetCheck(0);

    if(movementDB->List_jointMouseFocus == TRUE)
       m_checkJointMouseFocus.SetCheck(1);
	else
       m_checkJointMouseFocus.SetCheck(0);


	m_groundGripEdit   = movementDB->List_groundDrag;
	m_aeroDynamicsEdit = movementDB->m_crossSectionalArea;

	//m_sliderAeroDynamics.SetPos((movementDB->m_crossSectionalArea * 1000.0f));
    //m_sliderGroundGrip.SetPos((movementDB->List_groundDrag * 1000.0f));
	m_sliderAirControl.SetPos((movementDB->m_airControl * 1000.0f));
    m_sliderRotationDragOnGround.SetPos((movementDB->m_rotationalGroundDrag * 1000.0f));
	m_sliderRotationDragConst.SetPos((movementDB->m_rotationDragConstant * 1000.0f));
	m_autoLevelXStrength.SetPos((movementDB->m_autoLevelXStrength * 1000.0f));
	m_autoLevelZStrength.SetPos((movementDB->m_autoLevelZStrength * 1000.0f));
	m_fluidControl.SetPos((movementDB->m_inLiquidMotionModifier * 1000.0f));

	if(movementDB->m_autoLevelX == TRUE)
       m_autoLevelingX.SetCheck(1);
	else
	   m_autoLevelingX.SetCheck(0);

	if(movementDB->m_autoLevelZ == TRUE)
       m_autoLevelingZ.SetCheck(1);
	else
	   m_autoLevelingZ.SetCheck(0);

	if(movementDB->m_floatWhenDead == TRUE)
       m_checkFloatWhenDead.SetCheck(1);
	else
	   m_checkFloatWhenDead.SetCheck(0);

	UpdateData(FALSE);
}

void CMotionSettingsDlg::OnCHECKJointMouseFocus() 
{
	if(movementDB->List_jointMouseFocus == TRUE){
	   movementDB->List_jointMouseFocus=FALSE;
       m_checkJointMouseFocus.SetCheck(0);
	}
	else{
       movementDB->List_jointMouseFocus=TRUE;
       m_checkJointMouseFocus.SetCheck(1);
	}	
}

void CMotionSettingsDlg::OnRADIOAutoDecelFcY() 
{
	if(movementDB->List_autoDecelY == TRUE){
	   movementDB->List_autoDecelY=FALSE;
       m_autoDecelForceY.SetCheck(0);
	}
	else{
       movementDB->List_autoDecelY=TRUE;
       m_autoDecelForceY.SetCheck(1);
	}	
	
}

void CMotionSettingsDlg::OnRADIOAutoDecelFcZ() 
{
	if(movementDB->List_autoDecelZ == TRUE){
	   movementDB->List_autoDecelZ=FALSE;
       m_autoDecelForceZ.SetCheck(0);
	}
	else{
       movementDB->List_autoDecelZ=TRUE;
       m_autoDecelForceZ.SetCheck(1);
	}	
	
}

void CMotionSettingsDlg::OnCHECKAntiGravity() 
{
	if(movementDB->m_antiGravity){
       movementDB->m_antiGravity = FALSE;
	   m_antiGravity.SetCheck(0);
	}
	else{
       movementDB->m_antiGravity = TRUE;
	   m_antiGravity.SetCheck(1);
	}
}

void CMotionSettingsDlg::OnCHECKAutoLevelingX() 
{
	if(movementDB->m_autoLevelX == TRUE)
		movementDB->m_autoLevelX = FALSE;
	else
		movementDB->m_autoLevelX = TRUE;
}

void CMotionSettingsDlg::OnCHECKAutoLevelingZ() 
{
	if(movementDB->m_autoLevelZ == TRUE)
		movementDB->m_autoLevelZ = FALSE;
	else
		movementDB->m_autoLevelZ = TRUE;
}

void CMotionSettingsDlg::OnCHECKEntityFloatsWhenDead() 
{
	if(movementDB->m_floatWhenDead == TRUE)
	   movementDB->m_floatWhenDead = FALSE;
    else
       movementDB->m_floatWhenDead = TRUE;
}

void CMotionSettingsDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


// EntityPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EntityPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityPropertiesDlg dialog


CEntityPropertiesDlg::CEntityPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityPropertiesDlg::IDD, pParent)
	, m_svrPhysics(0)
	, m_collisionType(0)
	, m_shadowPref(0)
{
	//{{AFX_DATA_INIT(CEntityPropertiesDlg)
	m_deathCamIndex = 0;
	m_energy = 0;
	m_firstPersonCameraIndex = 0;
	m_firstPersonModelRef = 0;
	m_fullModelRef = 0;
	m_hudLink = 0;
	m_meshRadius = 0.0f;
	m_modelType = 0;
	m_popoutRadius = 0.0f;
	m_thirdPersonCameraIndex = 0;
	m_mountBoneIndex = 0;
	m_team = 0;
	m_entityName = _T("");
	m_raceID = 0;
	//}}AFX_DATA_INIT
}


void CEntityPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityPropertiesDlg)
	DDX_Control(pDX, IDC_CHECK_DisableAllCollision, m_checkDisableCollision);
	DDX_Control(pDX, IDC_CHECK_FIRSTPERSONMODEL, m_checkFirstPersonModel);
    DDX_Control(pDX, IDC_CHECK_FilterLodBalancing, m_checkFilterLodBalancing);
	
	DDX_Text(pDX, IDC_EDIT_DeathCamIndex, m_deathCamIndex);
	DDX_Text(pDX, IDC_EDIT_Energy, m_energy);
	DDX_Text(pDX, IDC_EDIT_FirstPersonCamIndex, m_firstPersonCameraIndex);
	DDX_Text(pDX, IDC_EDIT_FirstPersonModelRef, m_firstPersonModelRef);
	DDX_Text(pDX, IDC_EDIT_Fullmodelref, m_fullModelRef);
	DDX_Text(pDX, IDC_EDIT_HudLink, m_hudLink);
	DDX_Text(pDX, IDC_EDIT_meshRadius, m_meshRadius);
	DDX_Text(pDX, IDC_EDIT_modelType, m_modelType);
	DDX_Text(pDX, IDC_EDIT_PopoutRadius, m_popoutRadius);
	DDX_Text(pDX, IDC_EDIT_ThirdPersonCamIndex, m_thirdPersonCameraIndex);
	DDX_Text(pDX, IDC_EDIT_MountBoneIndex, m_mountBoneIndex);
	DDX_Text(pDX, IDC_EDIT_team, m_team);
	DDX_Text(pDX, IDC_EDIT_ENTITYNAME, m_entityName);
	DDX_Text(pDX, IDC_EDIT_RaceID, m_raceID);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_svrPhysics, m_svrPhysics);
	DDX_Text(pDX, IDC_EDIT_hardpointYovrd, m_hardPointOverd);

	DDX_Text(pDX, IDC_EDIT_collisonType, m_collisionType);
	DDX_Text(pDX, IDC_EDIT_shadowPref, m_shadowPref);
    DDX_Text(pDX, IDC_EDIT_HudLinkThirdPerson, m_hudLinkThirdPersonSpecific);
    DDX_Text(pDX, IDC_EDIT_selectionEffectTest, m_selectionEffectTest);


	
	
}


BEGIN_MESSAGE_MAP(CEntityPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CEntityPropertiesDlg)
	ON_BN_CLICKED(IDC_CHECK_FIRSTPERSONMODEL, OnCheckFirstpersonmodel)
	ON_BN_CLICKED(IDC_CHECK_DisableAllCollision, OnCHECKDisableAllCollision)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityPropertiesDlg message handlers

void CEntityPropertiesDlg::OnCheckFirstpersonmodel() 
{
	if(firstPersonModel == TRUE){
	   m_checkFirstPersonModel.SetCheck(0);
       firstPersonModel = FALSE;
	}
	else{
       m_checkFirstPersonModel.SetCheck(1);
       firstPersonModel = TRUE;
	}
}

BOOL CEntityPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(firstPersonModel == TRUE)
	   m_checkFirstPersonModel.SetCheck(1);

	m_checkFilterLodBalancing.SetCheck(m_filterLodBalancing);

    if(disableCollision == TRUE)
       m_checkDisableCollision.SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEntityPropertiesDlg::OnCHECKDisableAllCollision() 
{
	if(disableCollision == TRUE){
	   m_checkDisableCollision.SetCheck(0);
       disableCollision = FALSE;
	}
	else{
       m_checkDisableCollision.SetCheck(1);
       disableCollision = TRUE;
	}
}

void CEntityPropertiesDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_filterLodBalancing = m_checkFilterLodBalancing.GetCheck();
	UpdateData(FALSE);
	OnOK();
}

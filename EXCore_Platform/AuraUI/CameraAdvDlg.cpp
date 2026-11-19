// CameraAdvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CameraAdvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCameraAdvDlg dialog


CCameraAdvDlg::CCameraAdvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraAdvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCameraAdvDlg)
	m_fixedAfterPosX = 0.0f;
	m_fixedAfterPosY = 0.0f;
	m_fixedAfterPosZ = 0.0f;
	m_floatingMovePwr = 0.0f;
	m_fov = 0.0f;
	m_clipPlane = 0.0f;
	m_cameraType = _T("");
	m_zoomInSpeed = 0.0f;
	m_zoomInStop = 0.0f;
	m_zoomOutSpeed = 0.0f;
	m_InitialPosX = 0.0f;
	m_InitialPosY = 0.0f;
	m_InitialPosZ = 0.0f;
	m_floatTargetPosX = 0.0f;
	m_floatTargetPosY = 0.0f;
	m_floatTargetPosZ = 0.0f;
	m_baseHieghtAspect = 0.0f;
	m_pitchRotation = 0.0f;
	//}}AFX_DATA_INIT
}


void CCameraAdvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCameraAdvDlg)
	DDX_Control(pDX, IDC_CHECK_FloatingHasCollision, m_checkFloatHasCollision);
	DDX_Text(pDX, IDC_EDIT_FixedAfterPosX, m_fixedAfterPosX);
	DDX_Text(pDX, IDC_EDIT_FixedAfterPosY, m_fixedAfterPosY);
	DDX_Text(pDX, IDC_EDIT_FixedAfterPosZ, m_fixedAfterPosZ);
	DDX_Text(pDX, IDC_EDIT_FloatingMovePwr, m_floatingMovePwr);
	DDX_Text(pDX, IDC_EDIT_FOV, m_fov);
	DDX_Text(pDX, IDC_EDIT_CLIPPLANE, m_clipPlane);
	DDX_CBString(pDX, IDC_COMBO_CAMERAtype, m_cameraType);
	DDX_Text(pDX, IDC_EDIT_ZoomInSpeed, m_zoomInSpeed);
	DDX_Text(pDX, IDC_EDIT_ZoomInStop, m_zoomInStop);
	DDX_Text(pDX, IDC_EDIT_ZoomOutSpeed, m_zoomOutSpeed);
	DDX_Text(pDX, IDC_EDIT_FixedInitialPosX, m_InitialPosX);
	DDX_Text(pDX, IDC_EDIT_FixedInitialPosY, m_InitialPosY);
	DDX_Text(pDX, IDC_EDIT_FixedInitialPosZ, m_InitialPosZ);
	DDX_Text(pDX, IDC_EDIT_FloatTargetPosX, m_floatTargetPosX);
	DDX_Text(pDX, IDC_EDIT_FloatTargetPosY, m_floatTargetPosY);
	DDX_Text(pDX, IDC_EDIT_FloatTargetPosZ, m_floatTargetPosZ);
	DDX_Text(pDX, IDC_EDIT_BaseHieght, m_baseHieghtAspect);
	DDX_Text(pDX, IDC_EDIT_PitchRotation, m_pitchRotation);
	//}}AFX_DATA_MAP
	
    DDX_Text(pDX, IDC_EDIT_MiscVal1, m_miscVal1);
	DDX_Control(pDX, IDC_CHECK_FilterFirstPersonMode, m_filterFirstPersonMode);

	
}


BEGIN_MESSAGE_MAP(CCameraAdvDlg, CDialog)
	//{{AFX_MSG_MAP(CCameraAdvDlg)
	ON_BN_CLICKED(IDC_CHECK_FloatingHasCollision, OnCHECKFloatingHasCollision)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraAdvDlg message handlers

void CCameraAdvDlg::OnCHECKFloatingHasCollision() 
{
	if(floatingUsesCollision == TRUE)
	   floatingUsesCollision = FALSE;
	else
       floatingUsesCollision = TRUE;

}

void CCameraAdvDlg::OnOK() 
{
UpdateData(TRUE);
if(m_cameraType == "Zoomable Fixed")
   cameraTypeInt = 1;
if(m_cameraType == "Floating")
   cameraTypeInt = 2;
if(m_cameraType == "Fixed")
   cameraTypeInt = 0;
if(m_cameraType == "Bone Cam")
   cameraTypeInt = 3;
UpdateData(FALSE);
	
   CDialog::OnOK();
}

BOOL CCameraAdvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);
	if(cameraTypeInt == 1)
       m_cameraType="Zoomable Fixed";
    if(cameraTypeInt == 2)
       m_cameraType="Floating";
    if(cameraTypeInt == 0)
       m_cameraType="Fixed";
	if(cameraTypeInt == 3)
       m_cameraType="Bone Cam";

    if(floatingUsesCollision == TRUE)
	   m_checkFloatHasCollision.SetCheck(1);
	else
       m_checkFloatHasCollision.SetCheck(0);


	
	if(g_filterFirstPersonModeRef == TRUE)
	   m_filterFirstPersonMode.SetCheck(1);
	else
       m_filterFirstPersonMode.SetCheck(0);


	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCameraAdvDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	g_filterFirstPersonModeRef = m_filterFirstPersonMode.GetCheck();
	UpdateData(FALSE);
	
	OnOK();
}

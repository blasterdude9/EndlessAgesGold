// AnimSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include <d3d9.h>
#include "AnimSettingsDlg.h"
#include "TransformAnimationDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimSettingsDlg dialog


CAnimSettingsDlg::CAnimSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnimSettingsDlg)
	m_animationName = _T("");
	m_startFileName = _T("");
	m_totalFrames = 0;
	m_comboLoopType = _T("");
	m_transY = 0.0f;
	m_transZ = 0.0f;
	m_transX = 0.0f;
	m_rotateX = 0.0f;
	m_rotateY = 0.0f;
	m_rotateZ = 0.0f;
	m_scaleX = 0.0f;
	m_scaleY = 0.0f;
	m_scaleZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CAnimSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimSettingsDlg)
	DDX_Control(pDX, IDC_RADIO_VERTEX, m_radioVertexType);
	DDX_Control(pDX, IDC_RADIO_TRANSLATION, m_radioTranslationType);
	DDX_Text(pDX, IDC_EDIT_ANIMATION_NAME, m_animationName);
	DDX_Text(pDX, IDC_EDIT_START_FILE, m_startFileName);
	DDX_Text(pDX, IDC_EDIT_TOTAL_FRAMES, m_totalFrames);
	DDX_CBString(pDX, IDC_COMBO_LOOP_TYPE, m_comboLoopType);
	DDX_Text(pDX, IDC_EDIT_TRANSY, m_transY);
	DDX_Text(pDX, IDC_EDIT_TRANSZ, m_transZ);
	DDX_Text(pDX, IDC_EDIT_TRANSX, m_transX);
	DDX_Text(pDX, IDC_EDIT_ROTATEX, m_rotateX);
	DDX_Text(pDX, IDC_EDIT_ROTATEY, m_rotateY);
	DDX_Text(pDX, IDC_EDIT_ROTATEZ, m_rotateZ);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_scaleX);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_scaleY);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_scaleZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CAnimSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEVERTEXSTFILE, OnButtonBrowsevertexstfile)
	ON_BN_CLICKED(IDC_RADIO_TRANSLATION, OnRadioTranslation)
	ON_BN_CLICKED(IDC_RADIO_VERTEX, OnRadioVertex)
	ON_BN_CLICKED(IDC_BUTTON_TransSettings, OnBUTTONTransSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimSettingsDlg message handlers

void CAnimSettingsDlg::OnButtonBrowsevertexstfile() 
{
 //use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\Anim\\");
			 SetCurrentDirectory(CurDir);
 //end use mapsmodels folder for organization
 static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_startFileName = opendialog.GetFileName();
       path = opendialog.GetPathName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

BOOL CAnimSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	selectedLoop = 0;
	m_comboLoopType = "NONE";
	m_animationName = "ANIM_NAME";
	m_startFileName = "STARTFILE";
	m_totalFrames = 10;
	vertexAnimation = TRUE;
	m_radioVertexType.SetCheck(1);
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimSettingsDlg::OnOK() 
{
	UpdateData(TRUE);
    if(m_comboLoopType == "NONE")
       selectedLoop=0;
	if(m_comboLoopType == "Back And Forth")
       selectedLoop=1;
	if(m_comboLoopType == "Reset")
       selectedLoop=2;

	UpdateData(FALSE);
	CDialog::OnOK();
}

void CAnimSettingsDlg::OnRadioTranslation() 
{
	m_radioVertexType.SetCheck(0);
	m_radioTranslationType.SetCheck(1);
	vertexAnimation=FALSE;
	
}

void CAnimSettingsDlg::OnRadioVertex() 
{
	m_radioVertexType.SetCheck(1);
	m_radioTranslationType.SetCheck(0);
	vertexAnimation=TRUE;
}

void CAnimSettingsDlg::OnBUTTONTransSettings() 
{
	CTransformAnimationDlg dlg;
	dlg.m_speedTransX = trans_BasedSpeed.x;
	dlg.m_speedTransY = trans_BasedSpeed.y;
	dlg.m_speedTransZ = trans_BasedSpeed.z;
	dlg.m_destTransX = trans_Destination.x;
	dlg.m_destTransY = trans_Destination.y;
	dlg.m_destTransZ = trans_Destination.z;
	dlg.m_speedRoatationX = rotate_BasedSpeed.x;
	dlg.m_speedRoatationY = rotate_BasedSpeed.y;
	dlg.m_speedRoatationZ = rotate_BasedSpeed.z;
	dlg.m_destRotationX = rotate_Destination.x;
	dlg.m_destRotationY = rotate_Destination.y;
	dlg.m_destRotationZ = rotate_Destination.z;
    dlg.m_conditional = transformConditional;
	
	if(dlg.DoModal()==IDOK){
    trans_BasedSpeed.x = dlg.m_speedTransX;
	trans_BasedSpeed.y = dlg.m_speedTransY;
	trans_BasedSpeed.z = dlg.m_speedTransZ;
	trans_Destination.x = dlg.m_destTransX;
	trans_Destination.y = dlg.m_destTransY;
	trans_Destination.z = dlg.m_destTransZ;
	rotate_BasedSpeed.x = dlg.m_speedRoatationX;
	rotate_BasedSpeed.y = dlg.m_speedRoatationY;
	rotate_BasedSpeed.z = dlg.m_speedRoatationZ;
	rotate_Destination.x = dlg.m_destRotationX;
	rotate_Destination.y = dlg.m_destRotationY;
	rotate_Destination.z = dlg.m_destRotationZ;
    transformConditional = dlg.m_conditional;
	}	
}

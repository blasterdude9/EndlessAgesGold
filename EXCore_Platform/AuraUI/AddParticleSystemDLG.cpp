  // AddParticleSystemDLG.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AddParticleSystemDLG.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddParticleSystemDLG dialog


CAddParticleSystemDLG::CAddParticleSystemDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CAddParticleSystemDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddParticleSystemDLG)
	m_fileName = _T("");
	m_textureRef = 0;
	m_startHieght = 0.0f;
	m_particleRed = 0.0f;
	m_particlelifeTime = 0;
	m_particleGreen = 0.0f;
	m_particleCount = 0;
	m_particleBlue = 0.0f;
	m_fieldRadius = 0.0f;
	m_initialSpeedX = 0.0f;
	m_initialSpeedY = 0.0f;
	m_initialSpeedZ = 0.0f;

	/*m_fadeStart = 0.0f;
	m_fadeEnd = 0.0f;*/

	m_fadeStart = 0;
	m_fadeEnd = 0;
	//WARNING: C4244 --> conversion from float to int, possible loss of data

	m_directpartSpeed = 0.0f;
	m_directpartSpread = 0.0f;
	m_BSpeed = 0.0f;
	m_BSpeedDRate = 0.0f;
	m_BSpread = 0.0f;
	m_BSpreadDRate = 0.0f;
	m_scaleEnd = 0;
	m_scaleStart = 0;
	m_meshScale = 0.0f;
	//}}AFX_DATA_INIT
}


void CAddParticleSystemDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddParticleSystemDLG)
	DDX_Control(pDX, IDC_CHECK_VaryLifetime, m_VaryLifetime);
	DDX_Control(pDX, IDC_CHECK_Regenerate, m_Regenerate);
	DDX_Control(pDX, IDC_CHECK_FadeTime, m_fadeTime);
	DDX_Control(pDX, IDC_CHECK_IsDirectional, m_isDirectional);
	DDX_Control(pDX, IDC_CHECK_BillboardOn, m_billboardOn);
	DDX_Control(pDX, IDC_CHECK_SphereType, m_sphereType);
	DDX_Control(pDX, IDC_CHECK_SortOn, m_sortOn);
	DDX_Control(pDX, IDC_CHECK_EndSpwn, m_endSpawn);
	DDX_Text(pDX, IDC_EDIT_VisFileName, m_fileName);
	DDX_Text(pDX, IDC_EDIT_textureRef, m_textureRef);
	DDX_Text(pDX, IDC_EDIT_StartHieght, m_startHieght);
	DDX_Text(pDX, IDC_EDIT_ParticleRed, m_particleRed);
	DDX_Text(pDX, IDC_EDIT_ParticleLifeTime, m_particlelifeTime);
	DDX_Text(pDX, IDC_EDIT_ParticleGreen, m_particleGreen);
	DDX_Text(pDX, IDC_EDIT_ParticleCount, m_particleCount);
	DDX_Text(pDX, IDC_EDIT_ParticleBlue, m_particleBlue);
	DDX_Text(pDX, IDC_EDIT_fieldradius, m_fieldRadius);
	DDX_Text(pDX, IDC_EDIT_InitialSpeedX, m_initialSpeedX);
	DDX_Text(pDX, IDC_EDIT_InitialSpeedY, m_initialSpeedY);
	DDX_Text(pDX, IDC_EDIT_InitialSpeedZ, m_initialSpeedZ);
	DDX_Text(pDX, IDC_EDIT_FadeStart, m_fadeStart);
	DDX_Text(pDX, IDC_EDIT_FadeEnd, m_fadeEnd);
	DDX_Text(pDX, IDC_EDIT_DirectPartSpeed, m_directpartSpeed);
	DDX_Text(pDX, IDC_EDIT_DirectPartSpread, m_directpartSpread);
	DDX_Text(pDX, IDC_EDIT_BurstSpeed, m_BSpeed);
	DDX_Text(pDX, IDC_EDIT_BurstSpeedDeath, m_BSpeedDRate);
	DDX_Text(pDX, IDC_EDIT_BurstSpread, m_BSpread);
	DDX_Text(pDX, IDC_EDIT_BurstSpreadDeath, m_BSpreadDRate);
	DDX_Text(pDX, IDC_EDIT_EditParticle_ScaleEnd, m_scaleEnd);
	DDX_Text(pDX, IDC_EDIT_EditParticle_ScaleStart, m_scaleStart);
	DDX_Text(pDX, IDC_EDIT_meshScale, m_meshScale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddParticleSystemDLG, CDialog)
	//{{AFX_MSG_MAP(CAddParticleSystemDLG)
	ON_BN_CLICKED(IDC_BUTTON_BrowseFileName, OnBUTTONBrowseFileName)
	ON_BN_CLICKED(IDC_CHECK_BillboardOn, OnCHECKBillboardOn)
	ON_BN_CLICKED(IDC_CHECK_EndSpwn, OnCHECKEndSpwn)
	ON_BN_CLICKED(IDC_CHECK_SortOn, OnCHECKSortOn)
	ON_BN_CLICKED(IDC_CHECK_SphereType, OnCHECKSphereType)
	ON_BN_CLICKED(IDC_CHECK_IsDirectional, OnCHECKIsDirectional)
	ON_BN_CLICKED(IDC_CHECK_FadeTime, OnCHECKFadeTime)
	ON_BN_CLICKED(IDC_CHECK_Regenerate, OnCHECKRegenerate)
	ON_BN_CLICKED(IDC_CHECK_VaryLifetime, OnCHECKVaryLifetime)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddParticleSystemDLG message handlers

BOOL CAddParticleSystemDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(EndSpawn      == TRUE) m_endSpawn.SetCheck(1);
	if(SphereType    == TRUE) m_sphereType.SetCheck(1);
    if(SortOn        == TRUE) m_sortOn.SetCheck(1);
	if(BillboardOn   == TRUE) m_billboardOn.SetCheck(1);
	if(IsDirectional == TRUE) m_isDirectional.SetCheck(1);
	if(FadeTime      == TRUE) m_fadeTime.SetCheck(1);
	if(Regenerate    == TRUE) m_Regenerate.SetCheck(1);
	if(VaryLifetime  == TRUE) m_VaryLifetime.SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddParticleSystemDLG::OnBUTTONBrowseFileName() 
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

void CAddParticleSystemDLG::OnCHECKBillboardOn() 
{
	if(BillboardOn == TRUE){
	   BillboardOn = FALSE;
       m_billboardOn.SetCheck(0);
	}
	else{
       BillboardOn = TRUE;
       m_billboardOn.SetCheck(1);
	}
	
}

void CAddParticleSystemDLG::OnCHECKEndSpwn() 
{
	if(EndSpawn == TRUE){
	   EndSpawn = FALSE;
       m_endSpawn.SetCheck(0);
	}
	else{
       EndSpawn = TRUE;
       m_endSpawn.SetCheck(1);
	}
}

void CAddParticleSystemDLG::OnCHECKSortOn() 
{
	if(SortOn == TRUE){
	   SortOn = FALSE;
       m_sortOn.SetCheck(0);
	}
	else{
       SortOn = TRUE;
       m_sortOn.SetCheck(1);
	}
}

void CAddParticleSystemDLG::OnCHECKSphereType() 
{
	if(SphereType == TRUE){
	   SphereType = FALSE;
       m_sphereType.SetCheck(0);
	}
	else{
       SphereType = TRUE;
       m_sphereType.SetCheck(1);
	}
}

void CAddParticleSystemDLG::OnCHECKIsDirectional() 
{
	if(IsDirectional == TRUE){
	   IsDirectional = FALSE;
       m_isDirectional.SetCheck(0);
	}
	else{
       IsDirectional = TRUE;
       m_isDirectional.SetCheck(1);
	}	
}

void CAddParticleSystemDLG::OnCHECKFadeTime() 
{
	if(FadeTime == TRUE){
	   FadeTime = FALSE;
       m_fadeTime.SetCheck(0);
	}
	else{
       FadeTime = TRUE;
       m_fadeTime.SetCheck(1);
	}
}

void CAddParticleSystemDLG::OnCHECKRegenerate() 
{
	if(Regenerate == TRUE){
	   Regenerate = FALSE;
       m_Regenerate.SetCheck(0);
	}
	else{
       Regenerate = TRUE;
       m_Regenerate.SetCheck(1);
	}	
}

void CAddParticleSystemDLG::OnOK() 
{
	
	CFileException exc;
    CFile fl;
    BOOL res = fl.Open(m_fileName,CFile::modeRead, &exc);
	
	if(res == FALSE) {
		MessageBox("Please select a valid file");
		OnBUTTONBrowseFileName();
	}
	else {
		fl.Close();
		CDialog::OnOK();
	}
}

void CAddParticleSystemDLG::OnCHECKVaryLifetime() 
{
	if(VaryLifetime == TRUE){
	   VaryLifetime = FALSE;
       m_VaryLifetime.SetCheck(0);
	}
	else{
       VaryLifetime = TRUE;
       m_VaryLifetime.SetCheck(1);
	}	
}

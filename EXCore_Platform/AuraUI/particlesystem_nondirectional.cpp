// ParticleSystem_NonDirectional.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "ParticleSystem_NonDirectional.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ParticleSystem_NonDirectional property page

IMPLEMENT_DYNCREATE(ParticleSystem_NonDirectional, CPropertyPage)

ParticleSystem_NonDirectional::ParticleSystem_NonDirectional() : CPropertyPage(ParticleSystem_NonDirectional::IDD)
{
	//{{AFX_DATA_INIT(ParticleSystem_NonDirectional)
	m_scaleEnd = 0;
	m_scaleStart = 0;
	m_fadeEnd = 0;
	m_fadeStart = 0;
	m_fieldRadius = 0;
	m_xSpeed = 0.0f;
	m_ySpeed = 0.0f;
	m_zSpeed = 0.0f;
	m_startHeight = 0;
	//}}AFX_DATA_INIT
}

ParticleSystem_NonDirectional::~ParticleSystem_NonDirectional()
{
}

void ParticleSystem_NonDirectional::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ParticleSystem_NonDirectional)
	DDX_Text(pDX, IDC_EDIT_EditParticle_ScaleEnd, m_scaleEnd);
	DDX_Text(pDX, IDC_EDIT_EditParticle_ScaleStart, m_scaleStart);
	DDX_Text(pDX, IDC_EDIT_FadeEnd, m_fadeEnd);
	DDX_Text(pDX, IDC_EDIT_FadeStart, m_fadeStart);
	DDX_Text(pDX, IDC_EDIT_fieldradius, m_fieldRadius);
	DDX_Text(pDX, IDC_EDIT_InitialSpeedX, m_xSpeed);
	DDX_Text(pDX, IDC_EDIT_InitialSpeedY, m_ySpeed);
	DDX_Text(pDX, IDC_EDIT_InitialSpeedZ, m_zSpeed);
	DDX_Text(pDX, IDC_EDIT_StartHieght, m_startHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ParticleSystem_NonDirectional, CPropertyPage)
	//{{AFX_MSG_MAP(ParticleSystem_NonDirectional)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ParticleSystem_NonDirectional message handlers

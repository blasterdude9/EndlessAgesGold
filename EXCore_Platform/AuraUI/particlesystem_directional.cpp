// ParticleSystem_Directional.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "ParticleSystem_Directional.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ParticleSystem_Directional property page

IMPLEMENT_DYNCREATE(ParticleSystem_Directional, CPropertyPage)

ParticleSystem_Directional::ParticleSystem_Directional() : CPropertyPage(ParticleSystem_Directional::IDD)
{
	//{{AFX_DATA_INIT(ParticleSystem_Directional)
	m_burstSpeed = 0.0f;
	m_burstSpeedDeath = 0.0f;
	m_burstSpread = 0.0f;
	m_burstSpreadDeath = 0.0f;
	m_particleSpeed = 0.0f;
	m_particleSpread = 0.0f;
	m_scaleEnd = 0;
	m_scaleStart = 0;
	m_fadeEnd = 0;
	m_fadeStart = 0;
	m_fieldRadius = 0;
	m_fieldHeight = 0;
	//}}AFX_DATA_INIT
}

ParticleSystem_Directional::~ParticleSystem_Directional()
{
}

void ParticleSystem_Directional::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ParticleSystem_Directional)
	DDX_Text(pDX, IDC_EDIT_BurstSpeed, m_burstSpeed);
	DDX_Text(pDX, IDC_EDIT_BurstSpeedDeath, m_burstSpeedDeath);
	DDX_Text(pDX, IDC_EDIT_BurstSpread, m_burstSpread);
	DDX_Text(pDX, IDC_EDIT_BurstSpreadDeath, m_burstSpreadDeath);
	DDX_Text(pDX, IDC_EDIT_DirectPartSpeed, m_particleSpeed);
	DDX_Text(pDX, IDC_EDIT_DirectPartSpread, m_particleSpread);
	DDX_Text(pDX, IDC_EDIT_EditParticle_ScaleEnd, m_scaleEnd);
	DDX_Text(pDX, IDC_EDIT_EditParticle_ScaleStart, m_scaleStart);
	DDX_Text(pDX, IDC_EDIT_FadeEnd, m_fadeEnd);
	DDX_Text(pDX, IDC_EDIT_FadeStart, m_fadeStart);
	DDX_Text(pDX, IDC_EDIT_fieldradius, m_fieldRadius);
	DDX_Text(pDX, IDC_EDIT_StartHieght, m_fieldHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ParticleSystem_Directional, CPropertyPage)
	//{{AFX_MSG_MAP(ParticleSystem_Directional)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	//ON_MESSAGE(PSM_QUERYSIBLINGS, OnQuerySiblings)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ParticleSystem_Directional message handlers
int ParticleSystem_Directional::OnQuerySiblings()
{
	


	EnableWindow(FALSE);	
	return 1;
}

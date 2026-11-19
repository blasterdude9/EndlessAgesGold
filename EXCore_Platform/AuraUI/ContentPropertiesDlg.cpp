// ContentPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ContentPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContentPropertiesDlg dialog


CContentPropertiesDlg::CContentPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CContentPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContentPropertiesDlg)
	m_gunType = 0;
	m_hrdX = 0.0f;
	m_hrdY = 0.0f;
	m_hrdZ = 0.0f;
	m_fireRate = 0;
	m_uMov = 0.0f;
	m_vMov = 0.0f;
	m_offsetX = 0.0f;
	m_offsetZ = 0.0f;
	m_offsetY = 0.0f;
	m_dependancyIndex = 0;
	m_animTextureDBRef = 0;
	m_animTexturelink = 0;
	m_contentType = 0;
	m_maxFuel = 0;
	m_fuelUseRate = 0;
	m_fuelRegenRate = 0;
	m_particleLink = 0;
	m_particleDirX = 0.0f;
	m_particleDirY = 0.0f;
	m_particleDirZ = 0.0f;
	m_modulationMethod = 0;
	m_mirrorRenderable = 0;
	m_haloType = 0;
	m_lensflareType = 0;
	//}}AFX_DATA_INIT
}


void CContentPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContentPropertiesDlg)
	DDX_Text(pDX, IDC_EDIT_GunType, m_gunType);
	DDX_Text(pDX, IDC_EDIT_HardPointX, m_hrdX);
	DDX_Text(pDX, IDC_EDIT_HardPointY, m_hrdY);
	DDX_Text(pDX, IDC_EDIT_HardPointZ, m_hrdZ);
	DDX_Text(pDX, IDC_EDIT_FIRERATE, m_fireRate);
	DDX_Text(pDX, IDC_EDIT_Umov, m_uMov);
	DDX_Text(pDX, IDC_EDIT_Vmov, m_vMov);
	DDX_Text(pDX, IDC_EDIT_OFFSETX, m_offsetX);
	DDX_Text(pDX, IDC_EDIT_OFFESTZ, m_offsetZ);
	DDX_Text(pDX, IDC_EDIT_OFFSETY, m_offsetY);
	DDX_Text(pDX, IDC_EDIT_DependancyIndex, m_dependancyIndex);
	DDX_Text(pDX, IDC_EDIT_AnimTextureDBRef, m_animTextureDBRef);
	DDX_Text(pDX, IDC_EDIT_AnimTextureLink, m_animTexturelink);
	DDX_Text(pDX, IDC_EDIT_ContentType, m_contentType);
	DDX_Text(pDX, IDC_EDIT_MAXFUEL, m_maxFuel);
	DDX_Text(pDX, IDC_EDIT_FUELUSERATE, m_fuelUseRate);
	DDX_Text(pDX, IDC_EDIT_REGENRATE, m_fuelRegenRate);
	DDX_Text(pDX, IDC_EDIT_ParticleLink, m_particleLink);
	DDX_Text(pDX, IDC_EDIT_ParticleDirX, m_particleDirX);
	DDX_Text(pDX, IDC_EDIT_ParticleDirY, m_particleDirY);
	DDX_Text(pDX, IDC_EDIT_ParticleDirZ, m_particleDirZ);
	DDX_Text(pDX, IDC_EDIT_ModulationMethod, m_modulationMethod);
	DDX_Text(pDX, IDC_EDIT_MirrorRenderable, m_mirrorRenderable);
	DDX_Text(pDX, IDC_EDIT_HALOTYPE, m_haloType);
	DDX_Text(pDX, IDC_EDIT_LENSFLARETYPE, m_lensflareType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContentPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CContentPropertiesDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContentPropertiesDlg message handlers

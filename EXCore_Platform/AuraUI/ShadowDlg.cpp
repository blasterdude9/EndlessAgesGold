// ShadowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ShadowDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShadowDlg dialog


CShadowDlg::CShadowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShadowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShadowDlg)
	m_above = 0.0f;
	m_depth = 0.0f;
	m_collisionRadius = 0.0f;
	m_radius = 0.0f;
	m_textureIndex = 0;
	//}}AFX_DATA_INIT
}


void CShadowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShadowDlg)
	DDX_Control(pDX, IDC_CHECK_Enable, m_shadowEnabled);
	DDX_Text(pDX, IDC_EDIT_ABOVE, m_above);
	DDX_Text(pDX, IDC_EDIT_DEPTH, m_depth);
	DDX_Text(pDX, IDC_EDIT_CollisionRadius, m_collisionRadius);
	DDX_Text(pDX, IDC_EDIT_Radius, m_radius);
	DDX_Text(pDX, IDC_EDIT_TEXTUREINDEX, m_textureIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShadowDlg, CDialog)
	//{{AFX_MSG_MAP(CShadowDlg)
	//ON_BN_CLICKED(IDC_BUTTON_BrowseAlternateFile, OnBUTTONBrowseAlternateFile)
	ON_BN_CLICKED(IDC_CHECK_Enable, OnCHECKEnable)
//	ON_BN_CLICKED(IDC_CHECK_UseAlternateFile, OnCHECKUseAlternateFile)
	//ON_BN_CLICKED(IDC_CHECK_TRUEALPHA, OnCheckTruealpha)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShadowDlg message handlers

void CShadowDlg::OnBUTTONBrowseAlternateFile() 
{

}

void CShadowDlg::OnCHECKEnable() 
{
   if(shadowEnabled==TRUE){
	   m_shadowEnabled.SetCheck(0);
       shadowEnabled=FALSE;
	}
	else{
       m_shadowEnabled.SetCheck(1);
       shadowEnabled=TRUE;
	}
}

void CShadowDlg::OnCHECKUseAlternateFile() 
{

}

BOOL CShadowDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(shadowEnabled==TRUE)
	   m_shadowEnabled.SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShadowDlg::OnCheckTruealpha() 
{

}

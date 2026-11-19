// LaserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "LaserDlg.h"
#include "MaterialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLaserDlg dialog


CLaserDlg::CLaserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLaserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLaserDlg)
	m_length = 0.0f;
	m_millisecondsOfFade = 0;
	m_width = 0.0f;
	//}}AFX_DATA_INIT
}


void CLaserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLaserDlg)
	DDX_Control(pDX, IDC_CHECK_FadeOut, m_checkFadeOut);
	DDX_Control(pDX, IDC_CHECK_lockDown, m_checkLockDown);
	DDX_Text(pDX, IDC_EDIT_Length, m_length);
	DDX_Text(pDX, IDC_EDIT_millisecondsOfFade, m_millisecondsOfFade);
	DDX_Text(pDX, IDC_EDIT_width, m_width);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLaserDlg, CDialog)
	//{{AFX_MSG_MAP(CLaserDlg)
	ON_BN_CLICKED(IDC_BUTTON_MaterialAccess, OnBUTTONMaterialAccess)
	ON_BN_CLICKED(IDC_CHECK_FadeOut, OnCHECKFadeOut)
	ON_BN_CLICKED(IDC_CHECK_lockDown, OnCHECKlockDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLaserDlg message handlers

void CLaserDlg::OnBUTTONMaterialAccess() 
{
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.meshRef = m_laserObjectRef->m_meshObject;
	dlg.GL_textureDataBase = GL_textureDataBase;
	dlg.DoModal();
	
}

void CLaserDlg::OnCHECKFadeOut() 
{
	if(m_laserObjectRef->m_fadeOut)
	   m_laserObjectRef->m_fadeOut = FALSE;
	else
	   m_laserObjectRef->m_fadeOut = TRUE;
}

void CLaserDlg::OnCHECKlockDown() 
{
	if(m_laserObjectRef->m_lockDown)
	   m_laserObjectRef->m_lockDown = FALSE;
	else
	   m_laserObjectRef->m_lockDown = TRUE;
	
}

BOOL CLaserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	
	m_width                = m_laserObjectRef->m_currentWidth;
	m_length               = m_laserObjectRef->m_currentLength;
    m_millisecondsOfFade   = m_laserObjectRef->m_millisecondsOfFade;

    if(m_laserObjectRef->m_lockDown)
       m_checkLockDown.SetCheck(1);
	if(m_laserObjectRef->m_fadeOut)
       m_checkFadeOut.SetCheck(1);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLaserDlg::OnOK() 
{
	UpdateData(TRUE);
	
	m_laserObjectRef->m_currentWidth          = m_width;
	m_laserObjectRef->m_currentLength         = m_length;
    m_laserObjectRef->m_millisecondsOfFade    = m_millisecondsOfFade;

	UpdateData(FALSE);
	
	CDialog::OnOK();
}

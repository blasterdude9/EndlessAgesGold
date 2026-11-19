// BarSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "BarSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarSettingsDlg dialog


CBarSettingsDlg::CBarSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBarSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBarSettingsDlg)
	m_barLength = 0.0f;
	m_barWidth = 0.0f;
	m_visualReference = 0;
	m_rootX = 0.0f;
	m_rootY = 0.0f;
	//}}AFX_DATA_INIT
}


void CBarSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBarSettingsDlg)
	DDX_Control(pDX, IDC_COMBO_Link, m_cmboLink);
	DDX_Text(pDX, IDC_EDIT_BarLength, m_barLength);
	DDX_Text(pDX, IDC_EDIT_BarWidth, m_barWidth);
	DDX_Text(pDX, IDC_EDIT_visualReference, m_visualReference);
	DDX_Text(pDX, IDC_EDIT_RootX, m_rootX);
	DDX_Text(pDX, IDC_EDIT_RootY, m_rootY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBarSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CBarSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_staticAccessMaterial, OnBUTTONstaticAccessMaterial)
	ON_BN_CLICKED(IDC_BUTTON_dynamicAccessMaterial, OnBUTTONdynamicAccessMaterial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBarSettingsDlg message handlers

void CBarSettingsDlg::OnBUTTONstaticAccessMaterial() 
{
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = GL_textureDataBase;
       dlg.meshRef = menuBarRef->m_barStatic;
	   if(dlg.DoModal()==IDOK){
		  menuBarRef->m_barStatic->List_materialObject->Reset();
	   }
	
}

void CBarSettingsDlg::OnBUTTONdynamicAccessMaterial() 
{
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = GL_textureDataBase;
       dlg.meshRef = menuBarRef->m_barDynamic;
	   if(dlg.DoModal()==IDOK){
		  menuBarRef->m_barDynamic->List_materialObject->Reset();
	   }
	
}

BOOL CBarSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);
	m_barLength = menuBarRef->m_barLength;
    m_barWidth = menuBarRef->m_barWidth;
	m_visualReference = menuBarRef->m_visualReference;
	m_rootX = menuBarRef->m_xRoot;
	m_rootY = menuBarRef->m_yRoot;
	m_cmboLink.SetCurSel(menuBarRef->m_barUseType);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBarSettingsDlg::OnOK() 
{
	UpdateData(TRUE);
	menuBarRef->m_barLength = m_barLength;
    menuBarRef->m_barWidth = m_barWidth;
	menuBarRef->m_visualReference = m_visualReference;
	menuBarRef->m_xRoot = m_rootX;
	menuBarRef->m_yRoot = m_rootY;
	menuBarRef->m_barUseType = m_cmboLink.GetCurSel();
	UpdateData(FALSE);
	CDialog::OnOK();
}

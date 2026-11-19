// LightInterfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include <d3d9.h>
#include "LightInterfaceDlg.h"
#include "MaterialDlg.h"
#include "CEXMeshClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightInterfaceDlg dialog


CLightInterfaceDlg::CLightInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightInterfaceDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightInterfaceDlg)
	m_attenuation0 = 0.0f;
	m_attenuation1 = 0.0f;
	m_attenuation2 = 0.0f;
	m_range = 0.0f;
	//}}AFX_DATA_INIT
}


void CLightInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightInterfaceDlg)
	DDX_Text(pDX, IDC_EDIT_Attenuation0, m_attenuation0);
	DDX_Text(pDX, IDC_EDIT_Attenuation1, m_attenuation1);
	DDX_Text(pDX, IDC_EDIT_Attenuation2, m_attenuation2);
	DDX_Text(pDX, IDC_EDIT_Range, m_range);
	DDX_Control(pDX, IDC_CHECK_LightOn, m_checkLightOn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightInterfaceDlg, CDialog)
	//{{AFX_MSG_MAP(CLightInterfaceDlg)
	ON_BN_CLICKED(IDC_BUTTON_MaterialAccess, OnBUTTONMaterialAccess)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightInterfaceDlg message handlers

void CLightInterfaceDlg::OnBUTTONMaterialAccess() 
{
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	CEXMeshObj * meshTemp = new CEXMeshObj();
	m_lightREF->m_material->Clone(&meshTemp->List_materialObject);
	dlg.GL_textureDataBase = GL_textureDataBase;
       dlg.meshRef = meshTemp;
	   if(dlg.DoModal()==IDOK){
		  meshTemp->List_materialObject->Reset();
		  if(m_lightREF->m_material)
		  {
             delete m_lightREF->m_material;
             m_lightREF->m_material = 0;
		  }
		  meshTemp->List_materialObject->Clone(&m_lightREF->m_material);
		  meshTemp->SafeDelete();
		  delete meshTemp;
		  meshTemp = 0;
	   }
}

BOOL CLightInterfaceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(!m_lightREF){
		m_lightREF = new CRTLightObj();
        m_lightREF->m_material = new CMaterialObject();
	}
	
	UpdateData(TRUE);
    m_attenuation0 = m_lightREF->m_attenuation0;
	m_attenuation1 = m_lightREF->m_attenuation1;
	m_attenuation2 = m_lightREF->m_attenuation2;
	m_range        = m_lightREF->m_range;
	m_checkLightOn.SetCheck(m_lightREF->m_lightOn);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLightInterfaceDlg::OnOK() 
{
	UpdateData(TRUE);
    m_lightREF->m_attenuation0 = m_attenuation0;
	m_lightREF->m_attenuation1 = m_attenuation1;
	m_lightREF->m_attenuation2 = m_attenuation2;
	m_lightREF->m_range        = m_range;
	m_lightREF->m_lightOn      = m_checkLightOn.GetCheck();
	UpdateData(FALSE);
	
	CDialog::OnOK();
}

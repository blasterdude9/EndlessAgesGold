// LightSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "LightSettingsDlg.h"
#include "ColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightSettingsDlg dialog


CLightSettingsDlg::CLightSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightSettingsDlg)
	m_attenuation = 0.0f;
	m_attenuationAdjust = 0.0f;
	m_ocilatingSteps = 0;
	m_dalay = 0;
	m_flickerRange = 0;
	m_range = 0.0f;
	//}}AFX_DATA_INIT
}


void CLightSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightSettingsDlg)
	DDX_Control(pDX, IDC_CHECK_LinearAttenuation, m_linearAttenuation);
	DDX_Control(pDX, IDC_CHECK_FLICKERING, m_checkFlicker);
	DDX_Control(pDX, IDC_CHECK_OCILATING, m_checkOscilating);
	DDX_Text(pDX, IDC_EDIT_Attenuation, m_attenuation);
	DDX_Text(pDX, IDC_EDIT_AttenuationAdjust, m_attenuationAdjust);
	DDX_Text(pDX, IDC_EDIT_OcilatingSteps, m_ocilatingSteps);
	DDX_Text(pDX, IDC_EDIT_Delay, m_dalay);
	DDX_Text(pDX, IDC_EDIT_FlickerRange, m_flickerRange);
	DDX_Text(pDX, IDC_EDIT_Range, m_range);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CLightSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_LightColor, OnBUTTONLightColor)
	ON_BN_CLICKED(IDC_CHECK_OCILATING, OnCheckOcilating)
	ON_BN_CLICKED(IDC_CHECK_FLICKERING, OnCheckFlickering)
	ON_BN_CLICKED(IDC_CHECK_LinearAttenuation, OnCHECKLinearAttenuation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightSettingsDlg message handlers

void CLightSettingsDlg::OnBUTTONLightColor() 
{
	CColorDlg dlg;
	dlg.color255 = FALSE;
	dlg.m_red = Lred;
	dlg.m_green = Lgreen;
	dlg.m_blue = Lblue;
	if(dlg.DoModal()==IDOK){
       Lred = dlg.m_red;
	   Lgreen = dlg.m_green;
	   Lblue = dlg.m_blue;
	}
}

void CLightSettingsDlg::OnCheckOcilating() 
{
	if(ocilating == TRUE){
	   ocilating = FALSE;
	}
	else{
       ocilating = TRUE;
	}
}

BOOL CLightSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);

	if(ocilating == TRUE){
	   m_checkOscilating.SetCheck(1);
	}
	else{
       m_checkOscilating.SetCheck(0);
	}
	if(flickering == TRUE){
	   m_checkFlicker.SetCheck(1);
	}
	else{
       m_checkFlicker.SetCheck(0);
	}
	if(linearAttenuation == TRUE){
	   m_linearAttenuation.SetCheck(1);
	}
	else{
       m_linearAttenuation.SetCheck(0);
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLightSettingsDlg::OnCheckFlickering() 
{
if(flickering == TRUE){
	   flickering = FALSE;
	}
	else{
       flickering = TRUE;
	}
}


void CLightSettingsDlg::OnCHECKLinearAttenuation() 
{	
    if(linearAttenuation == TRUE){
	   linearAttenuation = FALSE;
	}
	else{
       linearAttenuation = TRUE;
	}
}

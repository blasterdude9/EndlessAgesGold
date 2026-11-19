// DayStateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include <d3d9.h>
#include "DayStateDlg.h"
#include "ColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDayStateDlg dialog


CDayStateDlg::CDayStateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDayStateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDayStateDlg)
	m_duration = 0.0;
	m_fogEndRange = 0.0f;
	m_fogStartRange = 0.0f;
	m_sunDistance = 0.0;
	m_sunRotationX = 0.0f;
	m_sunRotationY = 0.0f;
	//}}AFX_DATA_INIT
}


void CDayStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDayStateDlg)
	DDX_Text(pDX, IDC_EDIT_Duration, m_duration);
	DDX_Text(pDX, IDC_EDIT_FogEndRange, m_fogEndRange);
	DDX_Text(pDX, IDC_EDIT_FogStartRange, m_fogStartRange);
	DDX_Text(pDX, IDC_EDIT_SunDistance, m_sunDistance);
	DDX_Text(pDX, IDC_EDIT_SunRotationX, m_sunRotationX);
	DDX_Text(pDX, IDC_EDIT_SunRotationY, m_sunRotationY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDayStateDlg, CDialog)
	//{{AFX_MSG_MAP(CDayStateDlg)
	ON_BN_CLICKED(IDC_BUTTON_AmbientRGB, OnBUTTONAmbientRGB)
	ON_BN_CLICKED(IDC_BUTTON_FogRGB, OnBUTTONFogRGB)
	ON_BN_CLICKED(IDC_BUTTON_SunRGB, OnBUTTONSunRGB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDayStateDlg message handlers

void CDayStateDlg::OnBUTTONAmbientRGB() 
{
	CColorDlg dlg;
	dlg.color255 = FALSE;
	 
	dlg.m_red    = m_dayStateRef->m_ambientColorRed;
    dlg.m_green  = m_dayStateRef->m_ambientColorGreen;
	dlg.m_blue   = m_dayStateRef->m_ambientColorBlue;
    if(dlg.DoModal()==IDOK)
	{
       m_dayStateRef->m_ambientColorRed    = dlg.m_red;
       m_dayStateRef->m_ambientColorGreen  = dlg.m_green;
	   m_dayStateRef->m_ambientColorBlue   = dlg.m_blue;
	}
}

void CDayStateDlg::OnBUTTONFogRGB() 
{
	CColorDlg dlg;
	dlg.color255 = FALSE;
	 
	dlg.m_red    = m_dayStateRef->m_fogColorRed;
    dlg.m_green  = m_dayStateRef->m_fogColorGreen;
	dlg.m_blue   = m_dayStateRef->m_fogColorBlue;
    if(dlg.DoModal()==IDOK)
	{
       m_dayStateRef->m_fogColorRed    = dlg.m_red;
       m_dayStateRef->m_fogColorGreen  = dlg.m_green;
	   m_dayStateRef->m_fogColorBlue   = dlg.m_blue;
	}
}

void CDayStateDlg::OnBUTTONSunRGB() 
{
	CColorDlg dlg;
	dlg.color255 = FALSE;
	 
	dlg.m_red    = m_dayStateRef->m_sunColorRed;
    dlg.m_green  = m_dayStateRef->m_sunColorGreen;
	dlg.m_blue   = m_dayStateRef->m_sunColorBlue;
    if(dlg.DoModal()==IDOK)
	{
       m_dayStateRef->m_sunColorRed    = dlg.m_red;
       m_dayStateRef->m_sunColorGreen  = dlg.m_green;
	   m_dayStateRef->m_sunColorBlue   = dlg.m_blue;
	}
}

BOOL CDayStateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);
    double durationVar    = m_dayStateRef->m_duration;
	m_duration            = (durationVar/60000.0f);
	m_fogEndRange         = m_dayStateRef->m_fogEndRange;
	m_fogStartRange       = m_dayStateRef->m_fogStartRange;
	m_sunDistance         = m_dayStateRef->m_sunDistance;
	m_sunRotationX        = m_dayStateRef->m_sunRotationX;
	m_sunRotationY        = m_dayStateRef->m_sunRotationY;
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDayStateDlg::OnOK() 
{
	UpdateData(TRUE);
	double durationVar              = m_duration;
	/*m_dayStateRef->m_duration       = 60000.0f * durationVar;*/
	
	m_dayStateRef->m_duration       = (long)(60000.0f * durationVar);
	//WARNING: C4244 --> conversion from double to long, possible loss of data

	m_dayStateRef->m_fogEndRange    = m_fogEndRange;
	m_dayStateRef->m_fogStartRange  = m_fogStartRange;
	/*m_dayStateRef->m_sunDistance    = m_sunDistance;*/
	
	m_dayStateRef->m_sunDistance    = (long)m_sunDistance;
	//WARNING: C4244 --> conversion from double to long, possible loss of data

	m_dayStateRef->m_sunRotationX   = m_sunRotationX;
	m_dayStateRef->m_sunRotationY   = m_sunRotationY;
	UpdateData(FALSE);
	
	CDialog::OnOK();
}

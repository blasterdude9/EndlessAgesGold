// PlacementDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PlacementDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlacementDlg dialog


CPlacementDlg::CPlacementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlacementDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlacementDlg)
	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;
	m_comboEntityType = _T("");
	m_aiBotIndex = 0;
	//}}AFX_DATA_INIT
}


void CPlacementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlacementDlg)
	DDX_Control(pDX, IDC_RADIO_UserDefine, m_basedOnUserDefine);
	DDX_Control(pDX, IDC_RADIO_BasedOnEPosition, m_radioBasedOnEPosition);
	DDX_Control(pDX, IDC_RADIO_BasedOnEMatrix, m_radioBasedOnEMatrix);
	DDX_Text(pDX, IDC_EDIT_LocationX, m_posX);
	DDX_Text(pDX, IDC_EDIT_LocationY, m_posY);
	DDX_Text(pDX, IDC_EDIT_LocationZ, m_posZ);
	DDX_CBString(pDX, IDC_COMBO_EntityType, m_comboEntityType);
	DDX_Text(pDX, IDC_EDIT_AI_botIndex, m_aiBotIndex);
	//}}AFX_DATA_MAP
    DDX_Text(pDX, IDC_EDIT_AnimationOv, m_animationOv);
}


BEGIN_MESSAGE_MAP(CPlacementDlg, CDialog)
	//{{AFX_MSG_MAP(CPlacementDlg)
	ON_BN_CLICKED(IDC_RADIO_BasedOnEMatrix, OnRADIOBasedOnEMatrix)
	ON_BN_CLICKED(IDC_RADIO_BasedOnEPosition, OnRADIOBasedOnEPosition)
	ON_BN_CLICKED(IDC_RADIO_UserDefine, OnRADIOUserDefine)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlacementDlg message handlers

void CPlacementDlg::OnRADIOBasedOnEMatrix() 
{
	m_basedOnUserDefine.SetCheck(0);
	m_radioBasedOnEPosition.SetCheck(0);
	m_radioBasedOnEMatrix.SetCheck(1);
	PlacementMethod=2;
}

void CPlacementDlg::OnRADIOBasedOnEPosition() 
{
	m_basedOnUserDefine.SetCheck(0);
	m_radioBasedOnEPosition.SetCheck(1);
	m_radioBasedOnEMatrix.SetCheck(0);
	PlacementMethod=1;
}

void CPlacementDlg::OnRADIOUserDefine() 
{
	m_basedOnUserDefine.SetCheck(1);
	m_radioBasedOnEPosition.SetCheck(0);
	m_radioBasedOnEMatrix.SetCheck(0);
	PlacementMethod=0;
}

BOOL CPlacementDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	m_basedOnUserDefine.SetCheck(1);
	PlacementMethod=0;
    m_comboEntityType = "USER";
	m_animationOv = -1;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPlacementDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

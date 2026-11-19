// AnimPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AnimPropertiesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimPropertiesDlg dialog


CAnimPropertiesDlg::CAnimPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnimPropertiesDlg)
	m_animationAction = 0;
	//}}AFX_DATA_INIT
}


void CAnimPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimPropertiesDlg)
	DDX_Control(pDX, IDC_CHECK_MustFinishAnim, m_mustFinishAnim);
	DDX_Control(pDX, IDC_CHECK_UseInterpolation, m_useInterpolation);
	DDX_Text(pDX, IDC_EDIT_AnimationAction, m_animationAction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CAnimPropertiesDlg)
	ON_BN_CLICKED(IDC_CHECK_UseInterpolation, OnCHECKUseInterpolation)
	ON_BN_CLICKED(IDC_CHECK_MustFinishAnim, OnCHECKMustFinishAnim)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimPropertiesDlg message handlers

void CAnimPropertiesDlg::OnCHECKUseInterpolation() 
{
	if(useInterpolation == TRUE){
	m_useInterpolation.SetCheck(0);
    useInterpolation = FALSE;
	}
	else{
    m_useInterpolation.SetCheck(1);
    useInterpolation = TRUE;
	}
}

BOOL CAnimPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(useInterpolation == TRUE)
	m_useInterpolation.SetCheck(1);

	if(mustFinishAnim == TRUE)
    m_mustFinishAnim.SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimPropertiesDlg::OnCHECKMustFinishAnim() 
{
	if(mustFinishAnim == TRUE){
	m_mustFinishAnim.SetCheck(0);
    mustFinishAnim = FALSE;
	}
	else{
    m_mustFinishAnim.SetCheck(1);
    mustFinishAnim = TRUE;
	}
}

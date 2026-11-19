// GetSignificantOptionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GetSignificantOptionsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GetSignificantOptionsDlg dialog


GetSignificantOptionsDlg::GetSignificantOptionsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GetSignificantOptionsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(GetSignificantOptionsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void GetSignificantOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetSignificantOptionsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GetSignificantOptionsDlg, CDialog)
	//{{AFX_MSG_MAP(GetSignificantOptionsDlg)
	ON_BN_CLICKED(IDC_CHECK_GetInverseOnly, OnCHECKGetInverseOnly)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GetSignificantOptionsDlg message handlers

void GetSignificantOptionsDlg::OnCHECKGetInverseOnly() 
{
	if(m_getInverseOnly)
	   m_getInverseOnly = FALSE;
	else
	   m_getInverseOnly = TRUE;
	
}

BOOL GetSignificantOptionsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_getInverseOnly = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

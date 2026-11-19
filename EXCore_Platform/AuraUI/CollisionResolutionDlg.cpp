// CollisionResolutionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CollisionResolutionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCollisionResolutionDlg dialog


CCollisionResolutionDlg::CCollisionResolutionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCollisionResolutionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCollisionResolutionDlg)
	// Normal defaults were 0,0,0,0,0 but changed them to 2,2,2,0,10
	// because we are locking the form and only allowing users to
	// edit the maxfaceResident field.
	m_XResolution = 2;
	m_YResolution = 2;
	m_ZResolution = 2;
	m_maxfaceResident = 0;
	m_levelsDeep = 10;
	//}}AFX_DATA_INIT
}


void CCollisionResolutionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCollisionResolutionDlg)
	DDX_Text(pDX, IDC_EDIT_Xresolution, m_XResolution);
	DDX_Text(pDX, IDC_EDIT_Yresolution, m_YResolution);
	DDX_Text(pDX, IDC_EDIT_Zresolution, m_ZResolution);
	DDX_Text(pDX, IDC_EDIT_MaxFaceResident, m_maxfaceResident);
	DDX_Text(pDX, IDC_EDIT_LevelsDeep, m_levelsDeep); 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCollisionResolutionDlg, CDialog)
	//{{AFX_MSG_MAP(CCollisionResolutionDlg)
	ON_BN_CLICKED(IDC_CHECK_AdvancedEditCollisions, OnCHECKAdvancedEditCollisions)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCollisionResolutionDlg message handlers
void CCollisionResolutionDlg::OnCHECKAdvancedEditCollisions() 
{

}

// TransformAnimationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TransformAnimationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransformAnimationDlg dialog


CTransformAnimationDlg::CTransformAnimationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransformAnimationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransformAnimationDlg)
	m_speedTransZ = 0.0f;
	m_conditional = 0;
	m_speedTransX = 0.0f;
	m_speedTransY = 0.0f;
	m_speedRoatationX = 0.0f;
	m_speedRoatationY = 0.0f;
	m_speedRoatationZ = 0.0f;
	m_destTransX = 0.0f;
	m_destTransY = 0.0f;
	m_destTransZ = 0.0f;
	m_destRotationY = 0.0f;
	m_destRotationZ = 0.0f;
	m_destRotationX = 0.0f;
	//}}AFX_DATA_INIT
}


void CTransformAnimationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransformAnimationDlg)
	DDX_Text(pDX, IDC_EDIT__speedTransZ, m_speedTransZ);
	DDX_Text(pDX, IDC_EDIT_Conditional, m_conditional);
	DDX_Text(pDX, IDC_EDIT_speedTransX, m_speedTransX);
	DDX_Text(pDX, IDC_EDIT_speedTransY, m_speedTransY);
	DDX_Text(pDX, IDC_EDIT_speedRotationX, m_speedRoatationX);
	DDX_Text(pDX, IDC_EDIT_speedRotationY, m_speedRoatationY);
	DDX_Text(pDX, IDC_EDIT_speedRotationZ, m_speedRoatationZ);
	DDX_Text(pDX, IDC_EDIT_destTransX, m_destTransX);
	DDX_Text(pDX, IDC_EDIT_destTransY, m_destTransY);
	DDX_Text(pDX, IDC_EDIT_destTransZ, m_destTransZ);
	DDX_Text(pDX, IDC_EDIT_destRotationY, m_destRotationY);
	DDX_Text(pDX, IDC_EDIT_destRotationZ, m_destRotationZ);
	DDX_Text(pDX, IDC_EDIT_destRotationX, m_destRotationX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransformAnimationDlg, CDialog)
	//{{AFX_MSG_MAP(CTransformAnimationDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransformAnimationDlg message handlers

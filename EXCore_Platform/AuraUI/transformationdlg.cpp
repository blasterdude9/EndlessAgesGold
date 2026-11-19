// TransformationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\EngineMainApp\resource.h"
#include "TransformationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransformationDlg dialog


CTransformationDlg::CTransformationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransformationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransformationDlg)
	m_rotX = 0.0f;
	m_rotY = 0.0f;
	m_rotZ = 0.0f;
	m_transX = 0.0f;
	m_transY = 0.0f;
	m_transZ = 0.0f;
	m_scaleX = 0.0f;
	m_scaleY = 0.0f;
	m_scaleZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CTransformationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransformationDlg)
	DDX_Text(pDX, IDC_EDIT_RotX, m_rotX);
	DDX_Text(pDX, IDC_EDIT_RotY, m_rotY);
	DDX_Text(pDX, IDC_EDIT_RotZ, m_rotZ);
	DDX_Text(pDX, IDC_EDIT_TRANSX, m_transX);
	DDX_Text(pDX, IDC_EDIT_TRANSY, m_transY);
	DDX_Text(pDX, IDC_EDIT_TRANSZ, m_transZ);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_scaleX);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_scaleY);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_scaleZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransformationDlg, CDialog)
	//{{AFX_MSG_MAP(CTransformationDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransformationDlg message handlers

void CTransformationDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

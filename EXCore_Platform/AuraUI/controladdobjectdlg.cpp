// ControlAddObjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ControlAddObjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlAddObjectDlg dialog


CControlAddObjectDlg::CControlAddObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControlAddObjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CControlAddObjectDlg)
	m_name = _T("");
	m_comboBasis = _T("");
	//}}AFX_DATA_INIT
}


void CControlAddObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControlAddObjectDlg)
	DDX_Control(pDX, IDC_CHECK_InvertMouse, m_invertMouse);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_CBString(pDX, IDC_COMBO_BASIS, m_comboBasis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CControlAddObjectDlg, CDialog)
	//{{AFX_MSG_MAP(CControlAddObjectDlg)
	ON_BN_CLICKED(IDC_CHECK_InvertMouse, OnCHECKInvertMouse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlAddObjectDlg message handlers

BOOL CControlAddObjectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	if(controlType==0)
	   m_comboBasis = "Keyboard";
    if(controlType==1)
	   m_comboBasis = "Mouse";
	if(controlType==2)
	   m_comboBasis = "Joystick/ Gamepad";

	if(GL_invertMouse)
		m_invertMouse.SetCheck(1);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CControlAddObjectDlg::OnOK() 
{
	UpdateData(TRUE);
	if(m_comboBasis == "Keyboard")
	   controlType=0;
	if(m_comboBasis == "Mouse")
	   controlType=1;
	if(m_comboBasis == "Joystick/ Gamepad")
	   controlType=2;
	UpdateData(FALSE);
	CDialog::OnOK();
}

void CControlAddObjectDlg::OnCHECKInvertMouse() 
{
	if(GL_invertMouse)
		GL_invertMouse = FALSE;
	else
	   GL_invertMouse = TRUE;
	
}

// MenuTextObjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MenuTextObjectDlg.h"
#include ".\menutextobjectdlg.h"
#include "ColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuTextObjectDlg dialog


CMenuTextObjectDlg::CMenuTextObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuTextObjectDlg::IDD, pParent)
	, m_justify(0)
{
	//{{AFX_DATA_INIT(CMenuTextObjectDlg)
	m_fontSizeX = 0.0f;
	m_fontSizeY = 0.0f;
	m_textData = _T("");
	//}}AFX_DATA_INIT
}


void CMenuTextObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuTextObjectDlg)
	DDX_Control(pDX, IDC_COMBO_Blend, m_blendCombo);
	DDX_Text(pDX, IDC_EDIT_FontSizeX, m_fontSizeX);
	DDX_Text(pDX, IDC_EDIT_FontSizeY, m_fontSizeY);
	DDX_Text(pDX, IDC_EDIT_TextData, m_textData);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_justiftText, m_justify);
}


BEGIN_MESSAGE_MAP(CMenuTextObjectDlg, CDialog)
	//{{AFX_MSG_MAP(CMenuTextObjectDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FontColor, OnBnClickedButtonFontcolor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuTextObjectDlg message handlers

BOOL CMenuTextObjectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    UpdateData(TRUE);
	m_blendCombo.SetCurSel(m_blendSelection+1);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMenuTextObjectDlg::OnOK() 
{
	UpdateData(TRUE);
	m_blendSelection = m_blendCombo.GetCurSel()-1;
	UpdateData(FALSE);
	CDialog::OnOK();
}

void CMenuTextObjectDlg::OnBnClickedButtonFontcolor()
{
	
    CColorDlg dlg;
    dlg.m_red = (float)m_colorLinkRed;
    dlg.m_green = (float)m_colorLinkGreen;
    dlg.m_blue = (float)m_colorLinkBlue;
    if(dlg.DoModal()==TRUE){
	    m_colorLinkRed = (float)dlg.m_red;
        m_colorLinkGreen = (float)dlg.m_green;
        m_colorLinkBlue = (float)dlg.m_blue;
     }	
}

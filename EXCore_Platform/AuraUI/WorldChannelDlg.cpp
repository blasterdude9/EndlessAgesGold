// WorldChannelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "WorldChannelDlg.h"
#include "direct.h"
#include "afxdlgs.h"
#include "CWorldChannelRules.h"


// CWorldChannelDlg dialog

IMPLEMENT_DYNAMIC(CWorldChannelDlg, CDialog)
CWorldChannelDlg::CWorldChannelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorldChannelDlg::IDD, pParent)
	, m_worldID(0)
{
}

CWorldChannelDlg::~CWorldChannelDlg()
{

}

void CWorldChannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_visualType, m_channelRulesCombo);
	DDX_Text(pDX, IDC_EDIT_WorldID, m_worldID);
	DDX_Control(pDX, IDC_LIST_CurrentRuleList, m_currentRulesList);
}


BEGIN_MESSAGE_MAP(CWorldChannelDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AddRule, OnBnClickedButtonAddrule)
	ON_BN_CLICKED(IDC_BUTTON_DELETERule, OnBnClickedButtonDeleterule)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceRule, OnBnClickedButtonReplacerule)
	ON_LBN_SELCHANGE(IDC_LIST_CurrentRuleList, OnLbnSelchangeListCurrentrulelist)
	ON_BN_CLICKED(ID_BTN_SAVE, OnBnClickedBtnSave)
	ON_BN_CLICKED(ID_BTN_LOAD, OnBnClickedBtnLoad)
END_MESSAGE_MAP()

//--------------------
BOOL CWorldChannelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_channelRulesCombo.SetCurSel(0);
	UpdateData(TRUE);
	

	UpdateData(FALSE);
	InList(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//OBSOLETE

// CWorldChannelDlg message handlers
void CWorldChannelDlg::OnBnClickedButtonAddrule()
{
   
}

void CWorldChannelDlg::OnBnClickedButtonDeleterule()
{
	
}

void CWorldChannelDlg::OnBnClickedButtonReplacerule()
{
	
}

void CWorldChannelDlg::InList(int sel)
{
  
}

void CWorldChannelDlg::OnLbnSelchangeListCurrentrulelist()
{

		
}

void CWorldChannelDlg::OnBnClickedBtnSave()
{
   
}

void CWorldChannelDlg::OnBnClickedBtnLoad()
{
  
}

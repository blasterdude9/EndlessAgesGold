// SkillRangesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "SkillRangesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillRangesDlg dialog


CSkillRangesDlg::CSkillRangesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillRangesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkillRangesDlg)
	m_rangeTitle = _T("");
	m_maxExp = 0;
	m_minExp = 0;
	//}}AFX_DATA_INIT
}


void CSkillRangesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillRangesDlg)
	DDX_Control(pDX, IDC_LIST_LevelRangesDB, m_levelRangesDB);
	DDX_Text(pDX, IDC_EDIT_RangeTitle, m_rangeTitle);
	DDX_Text(pDX, IDC_EDIT_maxExp, m_maxExp);
	DDX_Text(pDX, IDC_EDIT_minExp, m_minExp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkillRangesDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillRangesDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddRange, OnBUTTONAddRange)
	ON_BN_CLICKED(IDC_BUTTON_DeleteRange, OnBUTTONDeleteRange)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceRange, OnBUTTONReplaceRange)
	ON_LBN_SELCHANGE(IDC_LIST_LevelRangesDB, OnSelchangeLISTLevelRangesDB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillRangesDlg message handlers

BOOL CSkillRangesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListRanges(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSkillRangesDlg::InListRanges(int sel)
{
  UpdateData(TRUE);
  m_levelRangesDB.ResetContent();
	for(POSITION posLoc = m_levelRangeDBRef->GetHeadPosition();posLoc != NULL;)
	{
        CLevelRangeObject * skillRangeObj = (CLevelRangeObject *) m_levelRangeDBRef->GetNext(posLoc);
        m_levelRangesDB.AddString(skillRangeObj->m_levelTitle);
	}

  if(sel >= 0)
	 m_levelRangesDB.SetCurSel(sel);

  UpdateData(FALSE);
}

void CSkillRangesDlg::OnBUTTONAddRange() 
{
	UpdateData(TRUE);
		
	CLevelRangeObject * skillRangeObj = new CLevelRangeObject();
	skillRangeObj->m_levelTitle = m_rangeTitle;
	skillRangeObj->m_rangeEnd   = m_maxExp;
	skillRangeObj->m_rangeStart = m_minExp;
	m_levelRangeDBRef->AddTail(skillRangeObj);

	UpdateData(FALSE);
	InListRanges((m_levelRangeDBRef->GetCount()-1));
}

void CSkillRangesDlg::OnBUTTONDeleteRange() 
{
	int rangeSelection = m_levelRangesDB.GetCurSel();
	if(rangeSelection == -1)
	{
		MessageBox("Must select a range");
		return;
	}
	POSITION posLoc = m_levelRangeDBRef->FindIndex(rangeSelection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CLevelRangeObject * skillRangeObj = (CLevelRangeObject *)m_levelRangeDBRef->GetAt(posLoc);
	UpdateData(TRUE);
	skillRangeObj->m_rangeEnd      = m_maxExp;
	skillRangeObj->m_rangeStart    = m_minExp;
	skillRangeObj->m_levelTitle    = m_rangeTitle;
	UpdateData(FALSE);
	InListRanges(rangeSelection-1);
}

void CSkillRangesDlg::OnBUTTONReplaceRange() 
{
	int rangeSelection = m_levelRangesDB.GetCurSel();
	if(rangeSelection == -1)
	   return;
       POSITION posLoc = m_levelRangeDBRef->FindIndex(rangeSelection);
	   if(!posLoc)
		   return;
	       CLevelRangeObject * skillRangeObj = (CLevelRangeObject *)m_levelRangeDBRef->GetAt(posLoc);
           UpdateData(TRUE);
           skillRangeObj->m_rangeEnd   = m_maxExp;
	       skillRangeObj->m_rangeStart = m_minExp;
		   skillRangeObj->m_levelTitle = m_rangeTitle;
		   UpdateData(FALSE);
		   InListRanges(rangeSelection);
}

void CSkillRangesDlg::OnSelchangeLISTLevelRangesDB() 
{
	int rangeSelection = m_levelRangesDB.GetCurSel();
	if(rangeSelection == -1)
	   return;
       POSITION posLoc = m_levelRangeDBRef->FindIndex(rangeSelection);
	   if(!posLoc)
		   return;
	       CLevelRangeObject * skillRangeObj = (CLevelRangeObject *)m_levelRangeDBRef->GetAt(posLoc);
           UpdateData(TRUE);
           m_maxExp = skillRangeObj->m_rangeEnd;
	       m_minExp = skillRangeObj->m_rangeStart;
		   m_rangeTitle = skillRangeObj->m_levelTitle;
		   UpdateData(FALSE);
}

// SkillsOnCharEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "SkillsOnCharEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillsOnCharEditorDlg dialog


CSkillsOnCharEditorDlg::CSkillsOnCharEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillsOnCharEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkillsOnCharEditorDlg)
	m_currentExp = 0;
	//}}AFX_DATA_INIT
}


void CSkillsOnCharEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillsOnCharEditorDlg)
	DDX_Control(pDX, IDC_LIST_StatsOnChar, m_statsOnChar);
	DDX_Control(pDX, IDC_LIST_StatisticsDB, m_statisticsDB);
	DDX_Control(pDX, IDC_LIST_SkillsDB, m_skillsDB);
	DDX_Control(pDX, IDC_LIST_CurrentSkills, m_currentSkills);
	DDX_Text(pDX, IDC_EDIT_currentExp, m_currentExp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkillsOnCharEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillsOnCharEditorDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddSkill, OnBUTTONAddSkill)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSkill, OnBUTTONDeleteSkill)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSkillValues, OnBUTTONReplaceSkillValues)
	ON_LBN_SELCHANGE(IDC_LIST_CurrentSkills, OnSelchangeLISTCurrentSkills)
	ON_BN_CLICKED(IDC_BUTTON_AddStat, OnBUTTONAddStat)
	ON_BN_CLICKED(IDC_BUTTON_DeleteStat, OnBUTTONDeleteStat)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillsOnCharEditorDlg message handlers

void CSkillsOnCharEditorDlg::OnBUTTONAddSkill() 
{
	int dbSelection = m_skillsDB.GetCurSel();
	if(dbSelection == -1)
	{ 
       AfxMessageBox("Must select a Database skill");
       return;
	}

	POSITION posLoc = m_skillDBRef->FindIndex(dbSelection);
	if(!posLoc)
		return;
        CSkillObject * skilPtr = (CSkillObject *) m_skillDBRef->GetAt(posLoc);

        CSkillObject * newSklPtr = NULL;
		skilPtr->Clone(&newSklPtr);
        m_onCharSkills->AddTail(newSklPtr);
		InListCharacterSkills(m_onCharSkills->GetCount()-1);
	
}

void CSkillsOnCharEditorDlg::OnBUTTONDeleteSkill() 
{
	int selection = m_currentSkills.GetCurSel();
	if(selection == -1)
	{ 
		AfxMessageBox("Must select a character skill");
		return;
	}

	POSITION cPos = m_onCharSkills->FindIndex(selection);
	if(!cPos)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CSkillObject * skilPtr = (CSkillObject *)m_onCharSkills->GetAt(cPos);
	skilPtr->SafeDelete();
	delete skilPtr;
	skilPtr = 0;
	m_onCharSkills->RemoveAt(cPos);

	InListCharacterSkills(selection-1);
}

void CSkillsOnCharEditorDlg::OnBUTTONReplaceSkillValues() 
{
	int selection = m_currentSkills.GetCurSel();
	if(selection == -1)
	{ 
       AfxMessageBox("Must select a character skill");
       return;
	}

	POSITION cPos = m_onCharSkills->FindIndex(selection);
	if(!cPos)
		return;
	    CSkillObject * skilPtr = (CSkillObject *)m_onCharSkills->GetAt(cPos);
		UpdateData(TRUE);
		/*skilPtr->m_currentExperience = m_currentExp;*/

		skilPtr->m_currentExperience = (float)m_currentExp;
		//WARNING: C4244 --> conversion from long to float, possible loss of data
		UpdateData(FALSE);
	
}

void CSkillsOnCharEditorDlg::InListCharacterSkills(int sel)
{
    UpdateData(TRUE);
    m_currentSkills.ResetContent();

    for(POSITION posLoc = m_onCharSkills->GetHeadPosition();posLoc != NULL;)
	{
        CSkillObject * skilPtr = (CSkillObject *) m_onCharSkills->GetNext(posLoc);
        m_statsOnChar.AddString(skilPtr->m_skillName);
	}
    if(sel > -1)
	   m_currentSkills.SetCurSel(sel);
	UpdateData(FALSE);
}

void CSkillsOnCharEditorDlg::InListCharacterStats(int sel)
{
    UpdateData(TRUE);
    m_statsOnChar.ResetContent();

    for(POSITION posLoc = m_statisticsOnChar->GetHeadPosition();posLoc != NULL;)
	{
        CStatObj * statPtr = (CStatObj *) m_statisticsOnChar->GetNext(posLoc);
        m_statsOnChar.AddString(statPtr->m_statName);
	}
    if(sel > -1)
	   m_statsOnChar.SetCurSel(sel);
	UpdateData(FALSE);
}

BOOL CSkillsOnCharEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    UpdateData(TRUE);
	POSITION posLoc;
	for(posLoc = m_skillDBRef->GetHeadPosition();posLoc != NULL;)
	{
        CSkillObject * skilPtr = (CSkillObject *) m_skillDBRef->GetNext(posLoc);
		m_skillsDB.AddString(skilPtr->m_skillName);
	}
    m_skillsDB.SetCurSel(0);
	
    for(posLoc = m_statisticDBREF->GetHeadPosition();posLoc != NULL;)
	{
        CStatObj * statPtr = (CStatObj *) m_statisticDBREF->GetNext(posLoc);
		m_statisticsDB.AddString(statPtr->m_statName);
	}
    m_statisticsDB.SetCurSel(0);


	UpdateData(FALSE);
	InListCharacterSkills(0);
	InListCharacterStats(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSkillsOnCharEditorDlg::OnSelchangeLISTCurrentSkills() 
{
	int selection = m_currentSkills.GetCurSel();
	if(selection == -1)
	{ 
       return;
	}

	POSITION cPos = m_onCharSkills->FindIndex(selection);
	if(!cPos)
		return;
	    CSkillObject * skilPtr = (CSkillObject *)m_onCharSkills->GetAt(cPos);
		UpdateData(TRUE);
		/*m_currentExp = skilPtr->m_currentExperience;*/

		m_currentExp = (long)skilPtr->m_currentExperience;
		//WARNING: C4244 --> conversion from float to long, possible loss of data

		UpdateData(FALSE);
}

void CSkillsOnCharEditorDlg::OnBUTTONAddStat() 
{
	int dbSelection = m_statisticsDB.GetCurSel();
	if(dbSelection == -1)
	{ 
       AfxMessageBox("Must select a statistic");
       return;
	}

	POSITION posLoc = m_statisticDBREF->FindIndex(dbSelection);
	if(!posLoc)
		return;
        CStatObj * statPtr = (CStatObj *) m_statisticDBREF->GetAt(posLoc);

        CStatObj * newStatPtr = NULL;
		statPtr->Clone(&newStatPtr);
        m_statisticsOnChar->AddTail(newStatPtr);
		InListCharacterStats(m_statisticsOnChar->GetCount()-1);
}

void CSkillsOnCharEditorDlg::OnBUTTONDeleteStat() 
{
	int dbSelection = m_statsOnChar.GetCurSel();
	if(dbSelection == -1)
	{ 
		AfxMessageBox("Must select a statistic on char");
		return;
	}

	POSITION posLoc = m_statisticsOnChar->FindIndex(dbSelection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CStatObj * statPtr = (CStatObj *) m_statisticsOnChar->GetAt(posLoc);
	delete statPtr;
	statPtr = 0;
	m_statisticsOnChar->RemoveAt(posLoc);

	InListCharacterStats(dbSelection-1);
}

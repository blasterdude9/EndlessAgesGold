// SkillBuilderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "SkillBuilderDlg.h"
#include "SkillRangesDlg.h"
#include "direct.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillBuilderDlg dialog


CSkillBuilderDlg::CSkillBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillBuilderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkillBuilderDlg)
	m_skillName = _T("");
	m_gainIncrement = 0;
	m_type = 0;
	//}}AFX_DATA_INIT
}


void CSkillBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillBuilderDlg)
	DDX_Control(pDX, IDC_LIST_SkillsAvailable, m_skillsAvailable);
	DDX_Text(pDX, IDC_EDIT_SkillName, m_skillName);
	DDX_Text(pDX, IDC_EDIT_gainIncrement, m_gainIncrement);
	DDX_Text(pDX, IDC_EDIT_type, m_type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkillBuilderDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillBuilderDlg)
	ON_BN_CLICKED(IDC_BUTTON_Abilities, OnBUTTONAbilities)
	ON_BN_CLICKED(IDC_BUTTON_AddSkill, OnBUTTONAddSkill)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_LevelRanges, OnBUTTONLevelRanges)
	ON_LBN_SELCHANGE(IDC_LIST_SkillsAvailable, OnSelchangeLISTSkillsAvailable)
	ON_BN_CLICKED(IDC_BUTTON_SaveDB, OnBUTTONSaveDB)
	ON_BN_CLICKED(IDC_BUTTON_LoadDB, OnBUTTONLoadDB)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSkill, OnBUTTONReplaceSkill)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillBuilderDlg message handlers
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
BOOL CSkillBuilderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListSkills(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::InListSkills(int sel)
{

	UpdateData(TRUE);
      m_skillsAvailable.ResetContent();
      
	  if(!m_skillDatabaseRef)
		  return;

	  for(POSITION posLoc = m_skillDatabaseRef->GetHeadPosition();posLoc != NULL;)
	  {
          CSkillObject * sklObj = (CSkillObject *)m_skillDatabaseRef->GetNext(posLoc);
          m_skillsAvailable.AddString(sklObj->m_skillName);
	  }

	  if(sel >= 0)
         m_skillsAvailable.SetCurSel(sel);
	UpdateData(FALSE);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::OnBUTTONAddSkill() 
{
	UpdateData(TRUE);

    CSkillObject * sklObj = new CSkillObject();
    sklObj->m_skillName     = m_skillName;
	/*sklObj->m_gainIncrement = m_gainIncrement;*/

	sklObj->m_gainIncrement = (float)m_gainIncrement;
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	sklObj->m_skillType     = m_type;
    m_skillDatabaseRef->AddTail(sklObj);

	UpdateData(FALSE);
	InListSkills(m_skillDatabaseRef->GetCount()-1);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::OnButtonDelete() 
{
	int skillSelection = m_skillsAvailable.GetCurSel();
	if(skillSelection == -1)
	{
		MessageBox("Must select a skill");
		return;
	}

	POSITION posSkill = m_skillDatabaseRef->FindIndex(skillSelection);
	if(!posSkill)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CSkillObject * sklObj = (CSkillObject *)m_skillDatabaseRef->GetAt(posSkill);
	sklObj->SafeDelete();
	delete sklObj;
	sklObj=0;
	m_skillDatabaseRef->RemoveAt(posSkill);

	InListSkills(skillSelection-1);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::OnSelchangeLISTSkillsAvailable() 
{
	int skillSelection = m_skillsAvailable.GetCurSel();
    if(skillSelection == -1)
	   return;


	   POSITION posSkill = m_skillDatabaseRef->FindIndex(skillSelection);
	   if(!posSkill)
		   return;

	       CSkillObject * sklObj = (CSkillObject *)m_skillDatabaseRef->GetAt(posSkill);
 

		   UpdateData(TRUE);

		   m_skillName      = sklObj->m_skillName;

	       /*m_gainIncrement  = sklObj->m_gainIncrement;*/

		   m_gainIncrement  = (int)sklObj->m_gainIncrement;
		   //WARNING: C4244 --> conversion from float to int, possible loss of data

		   m_type           = sklObj->m_skillType;
	

		   UpdateData(FALSE);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::OnBUTTONLevelRanges() 
{
     

	int skillSelection = m_skillsAvailable.GetCurSel();
    if(skillSelection == -1)
	{
	    MessageBox("Must select a skill");
		return;
	}

	POSITION posSkill = m_skillDatabaseRef->FindIndex(skillSelection);
	   if(!posSkill)
		   return;

	       CSkillObject * sklObj = (CSkillObject *)m_skillDatabaseRef->GetAt(posSkill);

		   if(!sklObj->m_levelRangeDB)
			   sklObj->m_levelRangeDB = new CLevelRangeList;

           CSkillRangesDlg dlg;
	       dlg.m_levelRangeDBRef = sklObj->m_levelRangeDB;

		   if(dlg.DoModal()==IDOK){


		   }
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::OnBUTTONAbilities() 
{

	
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::OnBUTTONSaveDB() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "SKL Files|*.SKL|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "SKL", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  MessageBox(fileName);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_skillDatabaseRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::OnBUTTONLoadDB() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization 	 
    static char BASED_CODE filter[] = "SKL Files|*.SKL|All Files (*.*)|*.*||";
	   CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
       if ( opendialog.DoModal() == IDOK )
	   {   
	      CString fileName = opendialog.GetFileName();  
	      SetCurrentDirectory(MainDir);
	      LoadSkillDB(fileName);
	   }
	   SetCurrentDirectory(MainDir);
	 return;
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::LoadSkillDB(CString fileName)
{
//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization 	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(MainDir); 
		return;
	 }

	 if(m_skillDatabaseRef)
	 {
        m_skillDatabaseRef->SafeDelete();
		delete m_skillDatabaseRef;
		m_skillDatabaseRef=0;
	 }

	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_skillDatabaseRef;
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(MainDir);
	 InListSkills(0);
}
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
//----------------------------------------------------------------------//
void CSkillBuilderDlg::OnBUTTONReplaceSkill() 
{
	int skillSelection = m_skillsAvailable.GetCurSel();
    if(skillSelection == -1)
	{
	    MessageBox("Must select a skill");
		return;
	}

	POSITION posSkill = m_skillDatabaseRef->FindIndex(skillSelection);
	   if(!posSkill)
		   return;

	       CSkillObject * sklObj = (CSkillObject *)m_skillDatabaseRef->GetAt(posSkill);
		  
           UpdateData(TRUE);

           sklObj->m_skillName     = m_skillName;

	       /*sklObj->m_gainIncrement = m_gainIncrement;*/
		   sklObj->m_gainIncrement = (float)m_gainIncrement;

		   //WARNING: C4244 --> conversion from int to float, possible loss of data

		   sklObj->m_skillType     = m_type;

		   UpdateData(FALSE);
		  
		   InListSkills(skillSelection);
}

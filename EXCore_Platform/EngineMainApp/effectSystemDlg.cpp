// effectSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "effectSystemDlg.h"
#include "direct.h"

// CeffectSystemDlg dialog

IMPLEMENT_DYNAMIC(CeffectSystemDlg, CDialog)

CeffectSystemDlg::CeffectSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CeffectSystemDlg::IDD, pParent)
	, m_effectName(_T(""))
{

}

CeffectSystemDlg::~CeffectSystemDlg()
{
}

void CeffectSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SelectionEffectList, m_effectList);
	DDX_Text(pDX, IDC_EDIT_EffectName, m_effectName);
}


BEGIN_MESSAGE_MAP(CeffectSystemDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AddSelectionEffect, &CeffectSystemDlg::OnBnClickedButtonAddselectioneffect)
	ON_BN_CLICKED(IDC_BUTTON_EditSelectionEffect, &CeffectSystemDlg::OnBnClickedButtonEditselectioneffect)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSelectionEffect2, &CeffectSystemDlg::OnBnClickedButtonReplaceselectioneffect2)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSelectionEffect3, &CeffectSystemDlg::OnBnClickedButtonDeleteselectioneffect3)
	ON_BN_CLICKED(IDC_BUTTON_LoadSelectionEffects, &CeffectSystemDlg::OnBnClickedButtonLoadselectioneffects)
	ON_BN_CLICKED(IDC_BUTTON_SaveSelectionEffects, &CeffectSystemDlg::OnBnClickedButtonSaveselectioneffects)
	ON_LBN_SELCHANGE(IDC_LIST_SelectionEffectList, &CeffectSystemDlg::OnLbnSelchangeListSelectioneffectlist)
END_MESSAGE_MAP()


// CeffectSystemDlg message handlers

BOOL CeffectSystemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InListEffects(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CeffectSystemDlg::InListEffects(int sel)
{
	m_effectList.ResetContent();
    for(POSITION posLoc = m_selectionEffectDBRef->GetHeadPosition();posLoc != NULL;)
	{
        CSelectionEffect * cePtr = (CSelectionEffect *) m_selectionEffectDBRef->GetNext(posLoc);

		m_effectList.AddString(cePtr->m_effectName);

	}
	if(sel > -1)
	   m_effectList.SetCurSel(sel);
}

void CeffectSystemDlg::OnBnClickedButtonAddselectioneffect()
{
	CSelectionEffect * effectPtr = new CSelectionEffect();
	effectPtr->m_skeletalPtr = new CSkeletonObject();
	UpdateData(TRUE);
	effectPtr->m_effectName = m_effectName;
    UpdateData(FALSE);
	//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
    char buffer[_MAX_PATH];
	 _getcwd( buffer, _MAX_PATH );
	 MainDir = CurDir = buffer;
    //end use mapsmodels folder for organization
    static char BASED_CODE filter[] = "ARB Files|*.ARB|ARB Files|*.ARB|All Files (*.*)|*.*||";
    CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
    if ( opendialog.DoModal() == IDOK )
	{ 
         CString filenamePathandAll = opendialog.GetPathName();
         effectPtr->m_skeletalPtr->InitBoneSystem(filenamePathandAll,m_pd3dDeviceRef);
	}
    SetCurrentDirectory(MainDir);
	//

	m_selectionEffectDBRef->AddTail(effectPtr);
	m_gStatus->SkeletalTextureReassignment(effectPtr->m_skeletalPtr);
	InListEffects(m_selectionEffectDBRef->GetCount()-1);
}

void CeffectSystemDlg::OnBnClickedButtonEditselectioneffect()
{
	int selection = m_effectList.GetCurSel();
	if(selection < 0)
	{
       AfxMessageBox("Must select a effect");
	   return;
	}

    POSITION posLoc = m_selectionEffectDBRef->FindIndex(selection);
    if(!posLoc)
		return;

	CSelectionEffect * effectPtr = (CSelectionEffect *)m_selectionEffectDBRef->GetAt(posLoc);

	CSkeletalAnimationDlg dlg;
	dlg.m_renderStateObjectRef	= m_renderStateObjectRef;
	dlg.m_globalShaderRef		= m_cgGlobalShaderDB;
	dlg.m_skillDatabaseREF		= m_skillDatabaseREF;
	dlg.m_elementDBREF			= m_elementDBREF;
	dlg.GL_textureDataBase		= m_textureDataBase;
	dlg.m_skeletonReferencePtr	= effectPtr->m_skeletalPtr;
	dlg.directSoundRef			= m_dirSndRef;
	dlg.DoModal();
}

void CeffectSystemDlg::OnBnClickedButtonReplaceselectioneffect2()
{
	int selection = m_effectList.GetCurSel();
	if(selection < 0)
	{
       AfxMessageBox("Must select a effect");
	   return;
	}

    POSITION posLoc = m_selectionEffectDBRef->FindIndex(selection);
    if(!posLoc)
		return;

	CSelectionEffect * effectPtr = (CSelectionEffect *)m_selectionEffectDBRef->GetAt(posLoc);

	UpdateData(TRUE);
	effectPtr->m_effectName = m_effectName;
    UpdateData(FALSE);

	InListEffects(selection);

}

void CeffectSystemDlg::OnBnClickedButtonDeleteselectioneffect3()
{
	int selection = m_effectList.GetCurSel();
	if(selection < 0)
	{
       AfxMessageBox("Must select a effect");
	   return;
	}

    POSITION posLoc = m_selectionEffectDBRef->FindIndex(selection);
    if(!posLoc)
		return;

	CSelectionEffect * effect = (CSelectionEffect *)m_selectionEffectDBRef->GetAt(posLoc);
    
	delete effect;
	effect = 0;

	m_selectionEffectDBRef->RemoveAt(posLoc);


	InListEffects(selection-1);
}

void CeffectSystemDlg::OnBnClickedButtonLoadselectioneffects()
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

   static char BASED_CODE filter[] = "Selection Effect Files|*.SEF|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   delete m_selectionEffectDBRef;
	   m_selectionEffectDBRef = 0;
	   the_in_Archive >> m_selectionEffectDBRef;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListEffects(0);
}

void CeffectSystemDlg::OnBnClickedButtonSaveselectioneffects()
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
     static char BASED_CODE filter[] = "Selection Effect Files|*.SEF|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "SEF", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_selectionEffectDBRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CeffectSystemDlg::OnLbnSelchangeListSelectioneffectlist()
{
	int selection = m_effectList.GetCurSel();
	if(selection < 0)
	{
	   return;
	}

    POSITION posLoc = m_selectionEffectDBRef->FindIndex(selection);
    if(!posLoc)
		return;

	CSelectionEffect * effectPtr = (CSelectionEffect *)m_selectionEffectDBRef->GetAt(posLoc);

	UpdateData(TRUE);
	m_effectName = effectPtr->m_effectName;
    UpdateData(FALSE);

}

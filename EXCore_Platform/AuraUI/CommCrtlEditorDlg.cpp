// CommCrtlEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\EngineMainApp\resource.h"
#include "d3d9.h"
#include "direct.h"
#include "SkeletalAnimationDlg.h"
#include "CommCrtlEditorDlg.h"


// CCommCrtlEditorDlg dialog

IMPLEMENT_DYNAMIC(CCommCrtlEditorDlg, CDialog)
CCommCrtlEditorDlg::CCommCrtlEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommCrtlEditorDlg::IDD, pParent)
	, m_controlName(_T(""))
{
}

CCommCrtlEditorDlg::~CCommCrtlEditorDlg()
{
}

void CCommCrtlEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_controlName, m_controlName);
	DDX_Control(pDX, IDC_LIST_Controls, m_controlList);
}


BEGIN_MESSAGE_MAP(CCommCrtlEditorDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_idleState, OnBnClickedButtonidlestate)
	ON_BN_CLICKED(IDC_BUTTON_downState, OnBnClickedButtondownstate)
	ON_BN_CLICKED(IDC_BUTTON_upState, OnBnClickedButtonupstate)
	ON_BN_CLICKED(IDC_BUTTON_newControl, OnBnClickedButtonnewcontrol)
	ON_BN_CLICKED(IDC_BUTTON_deleteControl, OnBnClickedButtondeletecontrol)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_replaceCntrlName, OnBnClickedButtonreplacecntrlname)
	ON_BN_CLICKED(IDC_BUTTON_delIdle, OnBnClickedButtondelidle)
	ON_BN_CLICKED(IDC_BUTTON_deleteUpState, OnBnClickedButtondeleteupstate)
	ON_BN_CLICKED(IDC_BUTTON_deleteDownState, OnBnClickedButtondeletedownstate)
	ON_BN_CLICKED(IDC_BUTTON_LoadLib, OnBnClickedButtonLoadlib)
	ON_BN_CLICKED(IDC_BUTTON_SaveLib, OnBnClickedButtonSavelib)
END_MESSAGE_MAP()


// CCommCrtlEditorDlg message handlers
BOOL CCommCrtlEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(!m_controlDatabase)
		m_controlDatabase = new CCommonCtrlList();

	InListControls(0);
  return TRUE;
}

void CCommCrtlEditorDlg::OnBnClickedButtonidlestate()
{
	int sel = m_controlList.GetCurSel();
	if(sel == -1)
	{
       MessageBox("Must select a control object");
	   return;
	}

	POSITION posLoc = m_controlDatabase->FindIndex(sel);
	if(!posLoc)
		return;

	CCommonCtrlObj * cntrlObj = (CCommonCtrlObj *)m_controlDatabase->GetAt(posLoc);

	if(!cntrlObj->m_hoverState)
	{//must initialize
            cntrlObj->m_hoverState = new CSkeletonObject();
			cntrlObj->m_hoverState->m_SkeletalName = "EXP";
            //use mapsmodels folder for organization
            CString CurDir;
            CString MainDir;
            char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
            //end use mapsmodels folder for organization
            static char BASED_CODE filter[] = "ARB Files|*.ARB|ARB Files|*.ARB|All Files (*.*)|*.*||";
	        CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
            if ( opendialog.DoModal() == IDOK )
			{ 
	             CString filenamePathandAll = opendialog.GetPathName();
	             cntrlObj->m_hoverState->InitBoneSystem(filenamePathandAll,g_pd3dDeviceRef);
				 cntrlObj->m_hoverState->Reinit(g_pd3dDeviceRef);
			}
			else{
	        SetCurrentDirectory(MainDir);
			return;
			}
            SetCurrentDirectory(MainDir);
	}//end must initialize
	CSkeletalAnimationDlg dlg;
	dlg.m_globalShaderRef = NULL;

	    dlg.m_elementDBREF = NULL;
		dlg.m_skillDatabaseREF = NULL;
		dlg.GL_textureDataBase = GL_textureDataBase;
		dlg.m_skeletonReferencePtr = cntrlObj->m_hoverState;
		dlg.directSoundRef = g_directSoundRef;
		dlg.DoModal();
    
}

void CCommCrtlEditorDlg::OnBnClickedButtondownstate()
{
	int sel = m_controlList.GetCurSel();
	if(sel == -1)
	{
       MessageBox("Must select a control object");
	   return;
	}

	POSITION posLoc = m_controlDatabase->FindIndex(sel);
	if(!posLoc)
		return;

	CCommonCtrlObj * cntrlObj = (CCommonCtrlObj *)m_controlDatabase->GetAt(posLoc);

	if(!cntrlObj->m_clickState)
	{//must initialize
            cntrlObj->m_clickState = new CSkeletonObject();
			cntrlObj->m_clickState->m_SkeletalName = "EXP";
            //use mapsmodels folder for organization
            CString CurDir;
            CString MainDir;
            char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
            //end use mapsmodels folder for organization
            static char BASED_CODE filter[] = "ARB Files|*.ARB|ARB Files|*.ARB|All Files (*.*)|*.*||";
	        CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
            if ( opendialog.DoModal() == IDOK )
			{ 
	             CString filenamePathandAll = opendialog.GetPathName();
	             cntrlObj->m_clickState->InitBoneSystem(filenamePathandAll,g_pd3dDeviceRef);
				 cntrlObj->m_clickState->Reinit(g_pd3dDeviceRef);
			}
			else{
	        SetCurrentDirectory(MainDir);
			return;
			}
            SetCurrentDirectory(MainDir);
	}//end must initialize
	CSkeletalAnimationDlg dlg;
	dlg.m_globalShaderRef = NULL;
	    dlg.m_elementDBREF = NULL;
		dlg.m_skillDatabaseREF = NULL;
		dlg.GL_textureDataBase = GL_textureDataBase;
		dlg.m_skeletonReferencePtr = cntrlObj->m_clickState;
		dlg.directSoundRef = g_directSoundRef;
		dlg.DoModal();
}

void CCommCrtlEditorDlg::OnBnClickedButtonupstate()
{
	int sel = m_controlList.GetCurSel();
	if(sel == -1)
	{
       MessageBox("Must select a control object");
	   return;
	}

	POSITION posLoc = m_controlDatabase->FindIndex(sel);
	if(!posLoc)
		return;

	CCommonCtrlObj * cntrlObj = (CCommonCtrlObj *)m_controlDatabase->GetAt(posLoc);

	if(!cntrlObj->m_idleState)
	{//must initialize
            cntrlObj->m_idleState = new CSkeletonObject();
			cntrlObj->m_idleState->m_SkeletalName = "EXP";
            //use mapsmodels folder for organization
            CString CurDir;
            CString MainDir;
            char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
            //end use mapsmodels folder for organization
            static char BASED_CODE filter[] = "ARB Files|*.ARB|ARB Files|*.ARB|All Files (*.*)|*.*||";
	        CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
            if ( opendialog.DoModal() == IDOK )
			{ 
	             CString filenamePathandAll = opendialog.GetPathName();
	             cntrlObj->m_idleState->InitBoneSystem(filenamePathandAll,g_pd3dDeviceRef);
				 cntrlObj->m_idleState->Reinit(g_pd3dDeviceRef);
			}
			else{
	        SetCurrentDirectory(MainDir);
			return;
			}
            SetCurrentDirectory(MainDir);
	}//end must initialize
	CSkeletalAnimationDlg dlg;
	dlg.m_globalShaderRef = NULL;
	    dlg.m_elementDBREF = NULL;
		dlg.m_skillDatabaseREF = NULL;
		dlg.GL_textureDataBase = GL_textureDataBase;
		dlg.m_skeletonReferencePtr = cntrlObj->m_idleState;
		dlg.directSoundRef = g_directSoundRef;
		dlg.DoModal();
}

void CCommCrtlEditorDlg::OnBnClickedButtonnewcontrol()
{
	
	CCommonCtrlObj * cntlObj = new CCommonCtrlObj();

	UpdateData(TRUE);
	cntlObj->m_controlName = m_controlName;



	UpdateData(FALSE);
	m_controlDatabase->AddTail(cntlObj);
	InListControls(m_controlDatabase->GetCount()-1);
}

void CCommCrtlEditorDlg::InListControls(int sel)
{
	m_controlList.ResetContent();
	UpdateData(TRUE);
    for(POSITION posLoc = m_controlDatabase->GetHeadPosition();posLoc != NULL;)
	{
        CCommonCtrlObj * cntlObj = (CCommonCtrlObj *)m_controlDatabase->GetNext(posLoc);
        m_controlList.AddString(cntlObj->m_controlName);
	}
	if(sel > -1)
	m_controlList.SetCurSel(sel);

	UpdateData(FALSE);
}

void CCommCrtlEditorDlg::OnBnClickedButtondeletecontrol()
{
	int sel = m_controlList.GetCurSel();
	if(sel == -1)
	{
       MessageBox("Must select a control object");
	   return;
	}

	POSITION posLoc = m_controlDatabase->FindIndex(sel);
	if(!posLoc)
		return;

	CCommonCtrlObj * cntrlObj = (CCommonCtrlObj *)m_controlDatabase->GetAt(posLoc);
	cntrlObj->SafeDelete();
	delete cntrlObj;
	cntrlObj = 0;
	m_controlDatabase->RemoveAt(posLoc);
	InListControls(sel-1);
}

int CCommCrtlEditorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	

	return 0;
}

void CCommCrtlEditorDlg::OnBnClickedButtonreplacecntrlname()
{
	int sel = m_controlList.GetCurSel();
	if(sel == -1)
	{
       MessageBox("Must select a control object");
	   return;
	}

	POSITION posLoc = m_controlDatabase->FindIndex(sel);
	if(!posLoc)
		return;

	CCommonCtrlObj * cntrlObj = (CCommonCtrlObj *)m_controlDatabase->GetAt(posLoc);
	UpdateData(TRUE);
	cntrlObj->m_controlName = m_controlName;
	UpdateData(FALSE);
	InListControls(sel);
}

void CCommCrtlEditorDlg::OnBnClickedButtondelidle()
{
	int sel = m_controlList.GetCurSel();
	if(sel == -1)
	{
       MessageBox("Must select a control object");
	   return;
	}

	POSITION posLoc = m_controlDatabase->FindIndex(sel);
	if(!posLoc)
		return;

	CCommonCtrlObj * cntrlObj = (CCommonCtrlObj *)m_controlDatabase->GetAt(posLoc);
	if(cntrlObj->m_hoverState)
	{
       cntrlObj->m_hoverState->SafeDelete();
	   delete cntrlObj->m_hoverState;
	   cntrlObj->m_hoverState = 0;

	}
}

void CCommCrtlEditorDlg::OnBnClickedButtondeleteupstate()
{
	int sel = m_controlList.GetCurSel();
	if(sel == -1)
	{
       MessageBox("Must select a control object");
	   return;
	}

	POSITION posLoc = m_controlDatabase->FindIndex(sel);
	if(!posLoc)
		return;

	CCommonCtrlObj * cntrlObj = (CCommonCtrlObj *)m_controlDatabase->GetAt(posLoc);
	if(cntrlObj->m_idleState)
	{
       cntrlObj->m_idleState->SafeDelete();
	   delete cntrlObj->m_idleState;
	   cntrlObj->m_idleState = 0;

	}
}

void CCommCrtlEditorDlg::OnBnClickedButtondeletedownstate()
{
	int sel = m_controlList.GetCurSel();
	if(sel == -1)
	{
       MessageBox("Must select a control object");
	   return;
	}

	POSITION posLoc = m_controlDatabase->FindIndex(sel);
	if(!posLoc)
		return;

	CCommonCtrlObj * cntrlObj = (CCommonCtrlObj *)m_controlDatabase->GetAt(posLoc);
	if(cntrlObj->m_clickState)
	{
       cntrlObj->m_clickState->SafeDelete();
	   delete cntrlObj->m_clickState;
	   cntrlObj->m_clickState = 0;

	}
}

void CCommCrtlEditorDlg::OnBnClickedButtonLoadlib()
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

   static char BASED_CODE filter[] = "Comm Ctrl Files|*.CMT|All Files (*.*)|*.*||";
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
	   m_controlDatabase->SafeDelete();
	   delete m_controlDatabase;
	   m_controlDatabase = 0;
	   the_in_Archive >> m_controlDatabase;
	   m_controlDatabase->ReinitAll(g_pd3dDeviceRef);
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListControls(0);
}

void CCommCrtlEditorDlg::OnBnClickedButtonSavelib()
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
     static char BASED_CODE filter[] = "Comm Ctrl Files|*.CMT|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "CMT", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_controlDatabase;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

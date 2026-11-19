// PatchSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../EngineMainApp/resource.h"
#include "PatchSystemDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatchSystemDlg dialog


CPatchSystemDlg::CPatchSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchSystemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatchSystemDlg)
	m_oldFileName = _T("");
	m_browseNewVersion = _T("");
	//}}AFX_DATA_INIT
}


void CPatchSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchSystemDlg)
	DDX_Control(pDX, IDC_LIST_PatchHistory, m_patchHistory);
	DDX_Text(pDX, IDC_EDIT_BrowseOldVersion, m_oldFileName);
	DDX_Text(pDX, IDC_EDIT_BrowseNewVersion, m_browseNewVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatchSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CPatchSystemDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_BROWSENEWVERSION, OnButtonBrowsenewversion)
	ON_BN_CLICKED(IDC_BUTTON_CREATEPATCH, OnButtonCreatepatch)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SaveDatabase, OnBUTTONSaveDatabase)
	ON_BN_CLICKED(IDC_BUTTON_TESTPATCH, OnButtonTestpatch)
	ON_BN_CLICKED(IDC_BUTTON_LoadDatabase, OnBUTTONLoadDatabase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchSystemDlg message handlers

BOOL CPatchSystemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListPatchDatabase(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPatchSystemDlg::InListPatchDatabase(int sel)
{
	UpdateData(TRUE);
    m_patchHistory.ResetContent();
	for(POSITION posLoc = m_patchDatabaseRef->GetHeadPosition();posLoc != NULL;)
	{
        CFilePatchObject * patchObj = (CFilePatchObject *) m_patchDatabaseRef->GetNext(posLoc);
        
		CString output = operator+("ver.",m_cabLib.ConvToString(patchObj->m_versionNumber));
		output = operator+(output," ");
		output = operator+(output,patchObj->m_fileTarget);
		m_patchHistory.AddString(output);
	}

	if(sel > -1)
	   m_patchHistory.SetCurSel(sel);
	UpdateData(FALSE);
}

void CPatchSystemDlg::OnButtonAdd() 
{
	CString CurDir;
    CString MainDir;
    CString Mdir;
    char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	MainDir = CurDir = buffer;
	static char BASED_CODE filter[] = "PATCH Files|*.PCH|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString patchFileName = opendialog.GetPathName();

	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(patchFileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir); 
		  return;
	   }

	   CFilePatchObject * patchObj = NULL;
	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> patchObj;
	   the_in_Archive.Close();
       fl.Close();
       SetCurrentDirectory(MainDir);
	   patchObj->m_versionNumber = m_patchDatabaseRef->GetNextVersionNumber();
	   m_patchDatabaseRef->AddTail(patchObj);
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);

	 InListPatchDatabase(m_patchDatabaseRef->GetCount()-1);
}

void CPatchSystemDlg::OnButtonBrowse() 
{
	CString CurDir;
    CString MainDir;
    CString Mdir;
    char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	MainDir = CurDir = buffer;
	static char BASED_CODE filter[] = "*.* Old Files|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_oldFileName = opendialog.GetPathName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CPatchSystemDlg::OnButtonBrowsenewversion() 
{
	CString CurDir;
    CString MainDir;
    CString Mdir;
    char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	MainDir = CurDir = buffer;
	

	static char BASED_CODE filter[] = "*.* Old Files|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString path = opendialog.GetPathName();
	   int mainDirCount = MainDir.GetLength()+1;
	   path.Delete(0,mainDirCount);
	
	   m_browseNewVersion = path;
	      
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CPatchSystemDlg::OnButtonCreatepatch() 
{
	CFilePatchObject * patchObj = new CFilePatchObject();
 
	UpdateData(TRUE);
	if(patchObj->ConstructPatch(m_oldFileName,m_browseNewVersion,500000))
	{//seccess
     //Build patch file and add to patch list

     CFileException exc;
     CFile fl;
     BOOL res = fl.Open("TEMP.PCH",CFile::modeCreate | CFile::modeWrite, &exc);
     if(res == FALSE)
	    return;
        CArchive the_out_Archive(&fl,CArchive::store);
        the_out_Archive << patchObj;
        the_out_Archive.Close();
        fl.Close();

     MessageBox("Completed Patch contruction sent to file");
	}//end success
	else
	 MessageBox("Failed to construct patch...");
	UpdateData(FALSE);

	
}

void CPatchSystemDlg::OnButtonDelete() 
{
	int patchSelection = m_patchHistory.GetCurSel();
    if(patchSelection == -1)
	{
       AfxMessageBox("Must select a patch");
       return;
	}

	POSITION posLoc = m_patchDatabaseRef->FindIndex(patchSelection);
	if(posLoc)
	{
        CFilePatchObject * patchObj = (CFilePatchObject *) m_patchDatabaseRef->GetAt(posLoc);
		if(patchObj)
		{
           patchObj->SafeDelete();
		   delete patchObj;
		   patchObj = 0;
		   m_patchDatabaseRef->RemoveAt(posLoc);
		}
	}
	InListPatchDatabase(patchSelection-1);
}

void CPatchSystemDlg::OnBUTTONSaveDatabase() 
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
     static char BASED_CODE filter[] = "Patch Databse Files|*.PDB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "PDB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_patchDatabaseRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CPatchSystemDlg::OnButtonTestpatch() 
{
	CString CurDir;
    CString MainDir;
    CString Mdir;
    char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	MainDir = CurDir = buffer;
	static char BASED_CODE filter[] = "PATCH Files|*.PCH|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString patchFileName = opendialog.GetPathName();

	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(patchFileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir); 
		  return;
	   }

	   CFilePatchObject * patchObj = NULL;
	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> patchObj;
	   the_in_Archive.Close();
       fl.Close();
       SetCurrentDirectory(MainDir);

	   patchObj->ApplyPatchToDirectory();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
	
}

void CPatchSystemDlg::OnBUTTONLoadDatabase() 
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

   static char BASED_CODE filter[] = "Patch Database Files|*.PDB|All Files (*.*)|*.*||";
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
	   m_patchDatabaseRef->SafeDelete();
	   delete m_patchDatabaseRef;
	   m_patchDatabaseRef = 0;
	   the_in_Archive >> m_patchDatabaseRef;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListPatchDatabase(0);
}

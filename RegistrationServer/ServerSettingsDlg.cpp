// ServerSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchServer.h"
#include "ServerSettingsDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerSettingsDlg dialog


CServerSettingsDlg::CServerSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerSettingsDlg)
	m_serverName = _T("");
	m_ipAdress = _T("");
	m_versionOutput = 0;
	//}}AFX_DATA_INIT
}


void CServerSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerSettingsDlg)
	DDX_Control(pDX, IDC_LIST_AvailableServers, m_availableServerList);
	DDX_Control(pDX, IDC_LIST_PatchDatabase, m_patchDatabase);
	DDX_Text(pDX, IDC_EDIT_ServerName, m_serverName);
	DDX_Text(pDX, IDC_EDIT_IPAdress, m_ipAdress);
	DDX_Text(pDX, IDC_EDIT_versionOutput, m_versionOutput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CServerSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CServerSettingsDlg)
	ON_BN_CLICKED(IDC_BUTTON_LoadDatabase, OnBUTTONLoadDatabase)
	ON_BN_CLICKED(IDC_BUTTON_AddServer, OnBUTTONAddServer)
	ON_BN_CLICKED(IDC_BUTTON_DeleteServer, OnBUTTONDeleteServer)
	ON_BN_CLICKED(IDC_BUTTON_LoadFromFile, OnBUTTONLoadFromFile)
	ON_BN_CLICKED(IDC_BUTTON_SaveToFile, OnBUTTONSaveToFile)
	ON_LBN_SELCHANGE(IDC_LIST_AvailableServers, OnSelchangeLISTAvailableServers)
	ON_BN_CLICKED(IDC_BUTTON_AddPatch, OnBUTTONAddPatch)
	ON_BN_CLICKED(IDC_BUTTON_DeletePatch, OnBUTTONDeletePatch)
	ON_BN_CLICKED(IDC_BUTTON_SetVersion, OnBUTTONSetVersion)
	ON_LBN_SELCHANGE(IDC_LIST_PatchDatabase, OnSelchangeLISTPatchDatabase)
	ON_BN_CLICKED(IDC_BUTTON_SaveDatabase, OnBUTTONSaveDatabase)
	ON_BN_CLICKED(IDC_BUTTON_LoadAccounts, OnBUTTONLoadAccounts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerSettingsDlg message handlers


void CServerSettingsDlg::InListPatches(int sel)
{
     m_patchDatabase.ResetContent();
 
	 for(POSITION posLoc = m_filePatchDBREF->GetHeadPosition();posLoc != NULL;)
	 {
         CFilePatchObject * patchObject = (CFilePatchObject *)m_filePatchDBREF->GetNext(posLoc);
		 CString build = operator+(ConvToString(patchObject->m_versionNumber),":");
		 build = operator+(build,patchObject->m_fileTarget);
         m_patchDatabase.AddString(build);
	 }

	 if(sel > -1)
	 {
        m_patchDatabase.SetCurSel(sel);
	 }
}

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CServerSettingsDlg::ConvToString(int number)
{    
              if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
		
			  return temp;
}

void CServerSettingsDlg::OnBUTTONLoadDatabase() 
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
	   m_filePatchDBREF->SafeDelete();
	   delete m_filePatchDBREF;
	   m_filePatchDBREF = 0;
	   the_in_Archive >> m_filePatchDBREF;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListPatches(0);
}

BOOL CServerSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListPatches(0);
	InListAvailableServers(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CServerSettingsDlg::InListAvailableServers(int sel)
{
   m_availableServerList.ResetContent();
    for(POSITION posLoc = m_availableServersREF->GetHeadPosition();posLoc != NULL;)
	{
        CServerListObject * svrObj = (CServerListObject *) m_availableServersREF->GetNext(posLoc);
		m_availableServerList.AddString(svrObj->m_serverName);
	}

   if(sel > -1)
	  m_availableServerList.SetCurSel(sel);
}

void CServerSettingsDlg::OnBUTTONAddServer() 
{
	UpdateData(TRUE);
    CServerListObject * svrObj = new CServerListObject();
    svrObj->m_IPadress = m_ipAdress;
    svrObj->m_serverName = m_serverName;
    m_availableServersREF->AddTail(svrObj);
    UpdateData(FALSE);

	InListAvailableServers(m_availableServersREF->GetCount());
}

void CServerSettingsDlg::OnBUTTONDeleteServer() 
{
	int sel = m_availableServerList.GetCurSel();
	if(sel == -1)
	{
       AfxMessageBox("Must select a server");
	   return;
	}

	POSITION posLoc = m_availableServersREF->FindIndex(sel);
	if(!posLoc)
		return;
        CServerListObject * svrObj = (CServerListObject *)m_availableServersREF->GetAt(posLoc);
        svrObj->SafeDelete();
		delete svrObj;
		svrObj = 0;

		m_availableServersREF->RemoveAt(posLoc);

	    InListAvailableServers(sel-1);
}

void CServerSettingsDlg::OnBUTTONLoadFromFile() 
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

    static char BASED_CODE filter[] = "Server List|*.SVS|All Files (*.*)|*.*||";
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
	   m_availableServersREF->SafeDelete();
	   delete m_availableServersREF;
	   m_availableServersREF = 0;
	   the_in_Archive >> m_availableServersREF;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListAvailableServers(0);
}

void CServerSettingsDlg::OnBUTTONSaveToFile() 
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
     static char BASED_CODE filter[] = "Server List|*.SVS|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "SVS", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_availableServersREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CServerSettingsDlg::OnSelchangeLISTAvailableServers() 
{
	int sel = m_availableServerList.GetCurSel();
	if(sel == -1)
	{
	   return;
	}

	POSITION posLoc = m_availableServersREF->FindIndex(sel);
	if(!posLoc)
		return;
        CServerListObject * svrObj = (CServerListObject *)m_availableServersREF->GetAt(posLoc);
		
		UpdateData(TRUE);
		m_ipAdress    = svrObj->m_IPadress;
	    m_serverName  = svrObj->m_serverName;
        UpdateData(FALSE);
}

void CServerSettingsDlg::OnBUTTONAddPatch() 
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
	   patchObj->m_versionNumber = m_filePatchDBREF->GetNextVersionNumber();
	   m_filePatchDBREF->AddTail(patchObj);
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);

	 InListPatches(m_filePatchDBREF->GetCount()-1);
}

void CServerSettingsDlg::OnBUTTONDeletePatch() 
{
	int selection = m_patchDatabase.GetCurSel();
	if(selection == -1)
	{
       return;
	}
 
	POSITION posLoc = m_filePatchDBREF->FindIndex(selection);
	if(!posLoc)
		return;
        CFilePatchObject * patchObject = (CFilePatchObject *)m_filePatchDBREF->GetAt(posLoc);
		patchObject->SafeDelete();
		delete patchObject;
		patchObject = 0;
        m_filePatchDBREF->RemoveAt(posLoc);
	
		InListPatches(selection-1);
}

void CServerSettingsDlg::OnBUTTONSetVersion() 
{
	int selection = m_patchDatabase.GetCurSel();
	if(selection == -1)
	{
       return;
	}
 
	POSITION posLoc = m_filePatchDBREF->FindIndex(selection);
	if(!posLoc)
		return;
        CFilePatchObject * patchObject = (CFilePatchObject *)m_filePatchDBREF->GetAt(posLoc);

		UpdateData(TRUE);
		patchObject->m_versionNumber = m_versionOutput;
        UpdateData(FALSE);

		InListPatches(selection);
}

void CServerSettingsDlg::OnSelchangeLISTPatchDatabase() 
{
	int selection = m_patchDatabase.GetCurSel();
	if(selection == -1)
	{
       return;
	}
 
	POSITION posLoc = m_filePatchDBREF->FindIndex(selection);
	if(!posLoc)
		return;
        CFilePatchObject * patchObject = (CFilePatchObject *)m_filePatchDBREF->GetAt(posLoc);

		UpdateData(TRUE);
		m_versionOutput = patchObject->m_versionNumber;

	


        UpdateData(FALSE);
}

void CServerSettingsDlg::OnBUTTONSaveDatabase() 
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
       the_out_Archive << m_filePatchDBREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CServerSettingsDlg::OnBUTTONLoadAccounts() 
{
	
}

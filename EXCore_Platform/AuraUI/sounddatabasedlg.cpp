// SoundDatabaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "SoundDatabaseDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSoundDatabaseDlg dialog


CSoundDatabaseDlg::CSoundDatabaseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSoundDatabaseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSoundDatabaseDlg)
	m_uniqueID = 0;
	//}}AFX_DATA_INIT
}


void CSoundDatabaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSoundDatabaseDlg)
	DDX_Control(pDX, IDC_LIST_SoundBank, m_soundBank);
	DDX_Text(pDX, IDC_EDIT_uniqueID, m_uniqueID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSoundDatabaseDlg, CDialog)
	//{{AFX_MSG_MAP(CSoundDatabaseDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBUTTONReplace)
	ON_LBN_SELCHANGE(IDC_LIST_SoundBank, OnSelchangeLISTSoundBank)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CSoundDatabaseDlg::OnBnClickedButtonTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSoundDatabaseDlg message handlers

void CSoundDatabaseDlg::OnButtonAdd() 
{
	//use mapsmodels folder for organization
    //end use mapsmodels folder for organization
      static char BASED_CODE filter[] = "WAV |*.wav|All Files (*.*)|*.*||";
	  CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
      if ( opendialog.DoModal() != IDOK )
	  {
	       return;
	  }
	  UpdateData(TRUE);


      if(m_uniqueID == 0)
	  {//get next id
         for(POSITION posLoc = m_soundManagerRef->GetHeadPosition();posLoc != NULL;)
	     {
             CSoundBankObj * sbPtr = (CSoundBankObj *)m_soundManagerRef->GetNext(posLoc);
			 if(sbPtr->m_id >= m_uniqueID)
			    m_uniqueID = sbPtr->m_id+1;
		 }
	  }//end get next id

	  SetCurrentDirectory(m_gstatusRef->GL_pathGameSounds);
      if(!m_soundManagerRef->CreateSoundBank(opendialog.GetFileName(),m_uniqueID))
	  {
		  UpdateData(FALSE);
		  SetCurrentDirectory(m_gstatusRef->GL_workingDirectory);
          AfxMessageBox("Sound Format cannot load");
		  return;
	  }
	  UpdateData(FALSE);
	  SetCurrentDirectory(m_gstatusRef->GL_workingDirectory);
	  
	  InListSoundFiles(m_soundManagerRef->GetCount()-1);
}

void CSoundDatabaseDlg::OnButtonDelete() 
{
	int bankIndex = m_soundBank.GetCurSel();
	if(bankIndex == -1)
	{
		AfxMessageBox("Must select a sound Object");
		return;
	}
	POSITION posLoc = m_soundManagerRef->FindIndex(bankIndex);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CSoundBankObj * sbPtr = (CSoundBankObj *)m_soundManagerRef->GetAt(posLoc);
	sbPtr->SafeDelete();
	delete sbPtr;
	sbPtr = 0;
	m_soundManagerRef->RemoveAt(posLoc);
	InListSoundFiles(bankIndex-1);
}

BOOL CSoundDatabaseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListSoundFiles(0);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSoundDatabaseDlg::InListSoundFiles(int sel)
{
	m_soundBank.ResetContent();
    for(POSITION posLoc = m_soundManagerRef->GetHeadPosition();posLoc != NULL;)
	{
        CSoundBankObj * sbPtr = (CSoundBankObj *)m_soundManagerRef->GetNext(posLoc);
		CString build = m_abLib.ConvToString(sbPtr->m_id);
		build = operator+(" ID:",build);
		m_soundBank.AddString(operator+(sbPtr->m_primary->m_fileName,build));
	}
	if(sel > -1)
	   m_soundBank.SetCurSel(sel);
}
void CSoundDatabaseDlg::OnBUTTONReplace() 
{
	int bankIndex = m_soundBank.GetCurSel();
    if(bankIndex == -1)
	{
       AfxMessageBox("Must select a sound Object");
       return;
	}
	POSITION posLoc = m_soundManagerRef->FindIndex(bankIndex);
	if(!posLoc)
		return;
	    CSoundBankObj * sbPtr = (CSoundBankObj *)m_soundManagerRef->GetAt(posLoc);

        UpdateData(TRUE);
		sbPtr->m_id = m_uniqueID;
		UpdateData(FALSE);
    InListSoundFiles(bankIndex);
}

void CSoundDatabaseDlg::OnSelchangeLISTSoundBank() 
{
	int bankIndex = m_soundBank.GetCurSel();
    if(bankIndex < 0)
	{
       return;
	}
	POSITION posLoc = m_soundManagerRef->FindIndex(bankIndex);
	if(!posLoc)
		return;
	    CSoundBankObj * sbPtr = (CSoundBankObj *)m_soundManagerRef->GetAt(posLoc);

	


        UpdateData(TRUE);
		m_uniqueID = sbPtr->m_id;
		UpdateData(FALSE);
}

void CSoundDatabaseDlg::OnButtonSave() 
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
    static char BASED_CODE filter[] = "SBK ENGIN|*.SBK|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "SBK", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   m_soundManagerRef->Save(fileName);
	 }
	 SetCurrentDirectory(MainDir);
}

void CSoundDatabaseDlg::OnButtonLoad() 
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
	 static char BASED_CODE filter[] = "SBK |*.SBK|All Files (*.*)|*.*||";
	  CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
      if ( opendialog.DoModal() != IDOK )
	  {
	       SetCurrentDirectory(MainDir);
	       return;
	  }
	 CString fileName = opendialog.GetFileName();
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(MainDir);
		return;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 m_soundManagerRef->SafeDelete();
	 delete m_soundManagerRef;
	 m_soundManagerRef = 0;
	 the_in_Archive >> m_soundManagerRef;
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(MainDir);
	 SetCurrentDirectory(m_gstatusRef->GL_pathGameSounds);
	 m_soundManagerRef->Initialize(directSoundRef,m_gstatusRef->m_soundListener);
	 SetCurrentDirectory(m_gstatusRef->GL_workingDirectory);
	 InListSoundFiles(0);
}

void CSoundDatabaseDlg::OnBnClickedButtonTest()
{
	int bankIndex = m_soundBank.GetCurSel();
    if(bankIndex < 0)
	{
       return;
	}
	POSITION posLoc = m_soundManagerRef->FindIndex(bankIndex);
	if(!posLoc)
		return;
	    CSoundBankObj * sbPtr = (CSoundBankObj *)m_soundManagerRef->GetAt(posLoc);

        D3DVECTOR pos;
		ZeroMemory(&pos,sizeof(D3DVECTOR));

		float soundFactor = .1f;//doesnt use
		float clipRadius = 2000.0f;

		m_soundManagerRef->PlayByID(sbPtr->m_id,FALSE,pos,soundFactor,clipRadius);

}

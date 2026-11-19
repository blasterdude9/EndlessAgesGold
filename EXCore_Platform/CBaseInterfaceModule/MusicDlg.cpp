// MusicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBaseInterfaceModule.h"
#include "MusicDlg.h"
#include ".\musicdlg.h"
#include "direct.h"


// CMusicDlg dialog

IMPLEMENT_DYNAMIC(CMusicDlg, CDialog)
CMusicDlg::CMusicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMusicDlg::IDD, pParent)
	, m_soundTrackName(_T(""))
	, m_urlPath(_T(""))
	, m_urlStatus(_T(""))
{
}

CMusicDlg::~CMusicDlg()
{
}

void CMusicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SoundTrackName, m_soundTrackName);
	DDX_Text(pDX, IDC_EDIT_SoundTrackURLorPATH, m_urlPath);
	DDX_Control(pDX, IDC_LIST_SoundTrackList, m_musicList);
	DDX_Text(pDX, IDC_STATIC_outputUrltype, m_urlStatus);
}


BEGIN_MESSAGE_MAP(CMusicDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AddSoundTrack, OnBnClickedButtonAddsoundtrack)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSoundTrack, OnBnClickedButtonDeletesoundtrack)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSoundTrack, OnBnClickedButtonReplacesoundtrack)
	ON_LBN_SELCHANGE(IDC_LIST_SoundTrackList, OnLbnSelchangeListSoundtracklist)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(ID_BTN_LoadSDDB, OnBnClickedBtnLoadsddb)
	ON_BN_CLICKED(ID_BTN_SaveSDDB, OnBnClickedBtnSavesddb)
	ON_BN_CLICKED(ID_BTN_TestMusic, OnBnClickedBtnTestmusic)
END_MESSAGE_MAP()


// CMusicDlg message handlers
void CMusicDlg::InList(int sel)
{
     m_musicList.ResetContent();
     int tracer = 0;
	 for(POSITION posLoc = m_soundTrackModuleRef->GetHeadPosition();posLoc != NULL;)
	 { 
         CSoundTrackObj * sdPtr = (CSoundTrackObj*)m_soundTrackModuleRef->GetNext(posLoc);

		 m_musicList.AddString(m_cabLib.ConvToString(tracer)+")  "+sdPtr->m_trackName);
		 tracer++;
	 }
	 if(sel > -1)
	    m_musicList.SetCurSel(sel);
}

void CMusicDlg::OnBnClickedButtonAddsoundtrack()
{
	UpdateData(TRUE);
	CSoundTrackObj * sdPtr = new CSoundTrackObj();
	sdPtr->m_trackName			= m_soundTrackName;
	sdPtr->m_fileName			= m_urlPath;

	sdPtr->m_url = false;
	CString urlDetect;
    if(m_urlPath.GetLength() > 4)
	{
	   urlDetect += m_urlPath.GetAt(0);
       urlDetect += m_urlPath.GetAt(1);
	   urlDetect += m_urlPath.GetAt(2);
	   urlDetect += m_urlPath.GetAt(3);
	}
    if(urlDetect.CompareNoCase("http") == 0)
	   sdPtr->m_url = true;

    m_soundTrackModuleRef->AddTail(sdPtr);
	UpdateData(FALSE);

	InList(m_soundTrackModuleRef->GetCount()-1);
}

void CMusicDlg::OnBnClickedButtonDeletesoundtrack()
{
	int musicSelection = m_musicList.GetCurSel();
    if(musicSelection < 0)
	{
        AfxMessageBox("Must select a music object");
		return;
	}
 
    m_soundTrackModuleRef->RemoveTrack(musicSelection);
    InList(musicSelection-1);
}

void CMusicDlg::OnBnClickedButtonReplacesoundtrack()
{
	int musicSelection = m_musicList.GetCurSel();
    if(musicSelection < 0)
	{
        AfxMessageBox("Must select a music object");
		return;
	}

	POSITION posLoc = m_soundTrackModuleRef->FindIndex(musicSelection);

	CSoundTrackObj * cPtr = (CSoundTrackObj *)m_soundTrackModuleRef->GetAt(posLoc);

	UpdateData(TRUE);
	cPtr->m_fileName = m_urlPath;
    cPtr->m_trackName = m_soundTrackName;

	cPtr->m_url = false;
	CString urlDetect;
    if(m_urlPath.GetLength() > 4)
	{
	   urlDetect += m_urlPath.GetAt(0);
       urlDetect += m_urlPath.GetAt(1);
	   urlDetect += m_urlPath.GetAt(2);
	   urlDetect += m_urlPath.GetAt(3);
	}
    if(urlDetect.CompareNoCase("http") == 0)
	   cPtr->m_url = true;

    if(cPtr->m_url == true)
       m_urlStatus = "URL Detected";
	else
	   m_urlStatus = "";


    UpdateData(FALSE);
	InList(musicSelection);
}

void CMusicDlg::OnLbnSelchangeListSoundtracklist()
{
	int musicSelection = m_musicList.GetCurSel();
    if(musicSelection < 0)
	{
		return;
	}

	POSITION posLoc = m_soundTrackModuleRef->FindIndex(musicSelection);

	CSoundTrackObj * cPtr = (CSoundTrackObj *)m_soundTrackModuleRef->GetAt(posLoc);

	UpdateData(TRUE);
	m_urlPath = cPtr->m_fileName;
    m_soundTrackName = cPtr->m_trackName;

	if(cPtr->m_url == true)
       m_urlStatus = "URL Detected";
	else
	   m_urlStatus = "";

    UpdateData(FALSE);
}

void CMusicDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CMusicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InList(-1);
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMusicDlg::OnBnClickedBtnLoadsddb()
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

    static char BASED_CODE filter[] = "Soundtrack Database Files|*.STK|All Files (*.*)|*.*||";
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
	   delete m_soundTrackModuleRef;
	   m_soundTrackModuleRef = 0;
	   the_in_Archive >> m_soundTrackModuleRef;
	   the_in_Archive.Close();
       fl.Close();
	   *m_soundTrackMemoryRef = m_soundTrackModuleRef;
       m_soundTrackModuleRef->Initialize(m_hwndMainRef,m_dsoundRef);
	 }
     SetCurrentDirectory(MainDir);
	 InList(-1);
}

void CMusicDlg::OnBnClickedBtnSavesddb()
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
     static char BASED_CODE filter[] = "Soundtrack Database Files|*.STK|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "STK", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_soundTrackModuleRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CMusicDlg::OnBnClickedBtnTestmusic()
{
	int musicSelection = m_musicList.GetCurSel();
    if(musicSelection < 0)
	{
        AfxMessageBox("Must select a music object");
		return;
	}


	m_soundTrackModuleRef->Play(musicSelection,m_soundFilePath);
}

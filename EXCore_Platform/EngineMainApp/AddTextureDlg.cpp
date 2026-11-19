// AddTextureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AddTextureDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddTextureDlg dialog


CAddTextureDlg::CAddTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddTextureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddTextureDlg)
	m_fileName = _T("");
	m_mipLevels = 0;
	//}}AFX_DATA_INIT
}


void CAddTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddTextureDlg)
	DDX_Control(pDX, IDC_LIST_Files, m_fileList);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_fileName);
	DDX_Text(pDX, IDC_EDIT_MIPLEVELS, m_mipLevels);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddTextureDlg, CDialog)
	//{{AFX_MSG_MAP(CAddTextureDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MipSpinner, OnDeltaposSPINMipSpinner)
	ON_BN_CLICKED(IDC_BUTTON_BrowseFileName, OnBUTTONBrowseFileName)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CAddTextureDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddTextureDlg message handlers

void CAddTextureDlg::OnDeltaposSPINMipSpinner(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
	UpdateData(TRUE);
    m_mipLevels -= pNMUpDown->iDelta;
    UpdateData(FALSE);
	*pResult = 0;
}

void CAddTextureDlg::OnBUTTONBrowseFileName() 
{
//char temp;
UpdateData(TRUE);
m_fileName.Empty();

char bigBuff[20000] = "";  // maximum common dialog buffer size
//use mapsmodels folder for organization
CString CurDir;
CString MainDir;
CString Mdir;
 char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 Mdir = CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
//end use mapsmodels folder for organization
	static char BASED_CODE filter[] = "Texture Files|*.tga;*.bmp;*.png;*.dds;*.jpg;*.jpeg|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT |OFN_FILEMUSTEXIST, filter, this );
     opendialog.m_ofn.hInstance = AfxGetInstanceHandle();
	 opendialog.m_ofn.lpstrFile = bigBuff;
	 opendialog.m_ofn.nMaxFile = sizeof(bigBuff);
	 fileCounter = 0;
	  if ( opendialog.DoModal() == IDOK )
	  {//file chosen
		  m_fileList.ResetContent();
	      //solisify directory
		  char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 CurDir = buffer;
             pathOfFiles = buffer;
		  
		  //-------------build selected files database------------//
	      fileCounter = 0;
	      int length = 0;
	      int length2 = 0;
		  int loop=0;
	       for(loop=0;loop<15000;loop++){
		    if(bigBuff[loop] == NULL)
			{
		       fileCounter++;
		       if(bigBuff[loop+1] == NULL)
               break; 
			   loop++;
			}
		    CString temp = (CString)bigBuff[loop];
		    FileNames[fileCounter] = operator+(FileNames[fileCounter],temp);
		   }
	       if(fileCounter == 1){
			 FileNames[0] = pathOfFiles;
             FileNames[1] = opendialog.GetFileName();
	         fileCounter++;
		   }
		    for(loop=0;loop<fileCounter;loop++){
			  m_fileList.AddString(FileNames[loop]);
			}
	  }// end file chosen
	  
SetCurrentDirectory(MainDir);
UpdateData(FALSE);
}

BOOL CAddTextureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddTextureDlg::OnOK() 
{
	if(fileCounter == 0){
       MessageBox("No Files selected");
	   return;
	}
	
	CDialog::OnOK();
}

void CAddTextureDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

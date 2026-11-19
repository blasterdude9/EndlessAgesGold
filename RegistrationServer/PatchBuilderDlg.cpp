// PatchBuilderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchServer.h"
#include "PatchBuilderDlg.h"
#include "Direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatchBuilderDlg dialog


CPatchBuilderDlg::CPatchBuilderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchBuilderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatchBuilderDlg)
	m_newFilePath = _T("");
	m_oldFilePath = _T("");
	m_patchServerSearchLength = 0;
	m_patchLabel = _T("");
	//}}AFX_DATA_INIT
}


void CPatchBuilderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchBuilderDlg)
	DDX_Control(pDX, IDC_CHECK_UseExistingFile, m_checkExistingFile);
	DDX_Text(pDX, IDC_EDIT_newFile, m_newFilePath);
	DDX_Text(pDX, IDC_EDIT_OldFilePath, m_oldFilePath);
	DDX_Text(pDX, IDC_EDIT_SearchLength, m_patchServerSearchLength);
	DDX_Text(pDX, IDC_EDIT_patchLabel, m_patchLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPatchBuilderDlg, CDialog)
	//{{AFX_MSG_MAP(CPatchBuilderDlg)
	ON_BN_CLICKED(IDC_BUTTON_BroewseForOldFile, OnBUTTONBroewseForOldFile)
	ON_BN_CLICKED(IDC_BUTTON_BrowseForNewFile, OnBUTTONBrowseForNewFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchBuilderDlg message handlers

BOOL CPatchBuilderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);
	m_patchLabel = "PATCH";
	m_patchServerSearchLength = 500000;
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPatchBuilderDlg::OnBUTTONBroewseForOldFile() 
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
	   m_oldFilePath = opendialog.GetPathName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
	
}

void CPatchBuilderDlg::OnBUTTONBrowseForNewFile() 
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
	
	   m_newFilePath = path;

	
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
	
}

void CPatchBuilderDlg::OnOK() 
{
	CFilePatchObject * patchObj = new CFilePatchObject();
	UpdateData(TRUE);
    
	if(m_checkExistingFile.GetCheck() == FALSE)
	{//exist 0
	  if(patchObj->ConstructPatch(m_newFilePath))
	  {//success
        //Build patch file and add to patch list
        CFileException exc;
        CFile fl;
        BOOL res = fl.Open(operator+(m_patchLabel,".PCH"),CFile::modeCreate | CFile::modeWrite, &exc);
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
	}
    if(m_checkExistingFile.GetCheck() == TRUE)
	{//exist 0
	  if(patchObj->ConstructPatch(m_oldFilePath,m_newFilePath,m_patchServerSearchLength))
	  {//seccess
       //Build patch file and add to patch list
       CFileException exc;
       CFile fl;
       BOOL res = fl.Open(operator+(m_patchLabel,".PCH"),CFile::modeCreate | CFile::modeWrite, &exc);
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
	}
	UpdateData(FALSE);
	
	CDialog::OnOK();
}

// ElementsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ElementsDlg.h"
#include "Direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CElementsDlg dialog


CElementsDlg::CElementsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CElementsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CElementsDlg)
	m_elementName = _T("");
	//}}AFX_DATA_INIT
}


void CElementsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElementsDlg)
	DDX_Control(pDX, IDC_LIST_ElementsDB, m_elementsList);
	DDX_Text(pDX, IDC_EDIT_elementName, m_elementName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElementsDlg, CDialog)
	//{{AFX_MSG_MAP(CElementsDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElementsDlg message handlers

void CElementsDlg::OnButtonAdd() 
{
	CElementObj * newElement = new CElementObj();
	UpdateData(TRUE);
	
    newElement->m_elementName = m_elementName;
    m_elementDBREF->AddTail(newElement);
	UpdateData(FALSE);
	InListElementals(m_elementDBREF->GetCount());
}

void CElementsDlg::OnButtonDelete() 
{
	int sel = m_elementsList.GetCurSel();
	if(sel == -1)
	{
       AfxMessageBox("Must select a Element");
       return;
	}

	POSITION posLoc = m_elementDBREF->FindIndex(sel);
	if(posLoc)
	{
	   CElementObj * newElement = (CElementObj *) m_elementDBREF->GetAt(posLoc);
       newElement->SafeDelete();
	   delete newElement;
	   newElement = 0;
       m_elementDBREF->RemoveAt(posLoc);
	}
	InListElementals(sel-1);
}

void CElementsDlg::InListElementals(int sel)
{
	m_elementsList.ResetContent();
     for(POSITION posLoc = m_elementDBREF->GetHeadPosition();posLoc != NULL;)
     {
         CElementObj * elementPtr = (CElementObj *)m_elementDBREF->GetNext(posLoc);
         m_elementsList.AddString(elementPtr->m_elementName);
	 }
	 if(sel > -1)
		 m_elementsList.SetCurSel(sel);
}

BOOL CElementsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListElementals(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CElementsDlg::OnButtonSave() 
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
     static char BASED_CODE filter[] = "Element Data Files|*.ELD|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "ELD", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_elementDBREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CElementsDlg::OnButtonLoad() 
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

   static char BASED_CODE filter[] = "Element Data Files|*.ELD|All Files (*.*)|*.*||";
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
	   m_elementDBREF->SafeDelete();
	   delete m_elementDBREF;
	   m_elementDBREF = 0;
	   the_in_Archive >> m_elementDBREF;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListElementals(0);
}

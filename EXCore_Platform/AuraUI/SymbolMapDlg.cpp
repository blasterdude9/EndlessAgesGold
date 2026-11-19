// SymbolMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SymbolMapDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSymbolMapDlg dialog


CSymbolMapDlg::CSymbolMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSymbolMapDlg::IDD, pParent)
	, m_symbolGroupID(_T(""))
	, m_optText(_T(""))
{
	//{{AFX_DATA_INIT(CSymbolMapDlg)
	m_gridDensity = 0;
	m_horizUVIndex = 0.0f;
	m_idCharForm = _T("");
	m_charSpacingPercentage = 0.0f;
	m_vertUVIndex = 0.0f;
	//}}AFX_DATA_INIT
}


void CSymbolMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSymbolMapDlg)
	DDX_Control(pDX, IDC_LIST_SymbolMapList, m_symbolMapList);
	DDX_Text(pDX, IDC_EDIT_GridDensity, m_gridDensity);
	DDX_Text(pDX, IDC_EDIT_horizUVIndex, m_horizUVIndex);
	DDX_Text(pDX, IDC_EDIT_ID, m_idCharForm);
	DDX_Text(pDX, IDC_EDIT_spacingPercentage, m_charSpacingPercentage);
	DDX_Text(pDX, IDC_EDIT_vertUVIndex, m_vertUVIndex);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SymbolGroupSTR, m_symbolGroupID);
	DDX_Text(pDX, IDC_EDIT_SymbolOpt, m_optText);
}


BEGIN_MESSAGE_MAP(CSymbolMapDlg, CDialog)
	//{{AFX_MSG_MAP(CSymbolMapDlg)
	ON_BN_CLICKED(IDC_BUTTON_SaveDB, OnBUTTONSaveDB)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBUTTONReplace)
	ON_BN_CLICKED(IDC_BUTTON_LoadDB, OnBUTTONLoadDB)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_LBN_SELCHANGE(IDC_LIST_SymbolMapList, OnSelchangeLISTSymbolMapList)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_Merge, OnBnClickedButtonMerge)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSymbolMapDlg message handlers

BOOL CSymbolMapDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);

    m_gridDensity = 16;
	m_charSpacingPercentage = 1.0f;


	UpdateData(FALSE);
	
	InListSymbols(m_symbolMapDBREF->GetCount()-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSymbolMapDlg::InListSymbols(int sel)
{
	m_symbolMapList.ResetContent();
    for(POSITION posLoc = m_symbolMapDBREF->GetHeadPosition();posLoc != NULL;)
	{
        CSymbolMapperObj * symPtr = (CSymbolMapperObj *)m_symbolMapDBREF->GetNext(posLoc);

		CString temp = symPtr->m_uniqueID;
        m_symbolMapList.AddString(temp);
	}
	if(sel>-1)
	   m_symbolMapList.SetCurSel(sel);
}


void CSymbolMapDlg::OnBUTTONSaveDB() 
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
     static char BASED_CODE filter[] = "Symbol Mapper Files|*.SYM|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "SYM", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_symbolMapDBREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CSymbolMapDlg::OnBUTTONReplace() 
{
	int selection = m_symbolMapList.GetCurSel();
	if(selection == -1)
	{
       AfxMessageBox("Must select a symbol");
       return;
	}
	POSITION posLoc = m_symbolMapDBREF->FindIndex(selection);
	if(!posLoc)
		return;
        CSymbolMapperObj * symPtr = (CSymbolMapperObj *)m_symbolMapDBREF->GetAt(posLoc);
        UpdateData(TRUE);
		//unsigned int idTemp = m_idCharForm.GetAt(0);
		symPtr->Calculate(m_gridDensity,m_horizUVIndex,m_vertUVIndex,m_idCharForm,m_charSpacingPercentage);
		symPtr->m_group = m_symbolGroupID;
		symPtr->m_extendedOption = m_optText;
		UpdateData(FALSE);
		InListSymbols(selection);

}

void CSymbolMapDlg::OnBUTTONLoadDB() 
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

   static char BASED_CODE filter[] = "Symbol Mapper Files|*.SYM|All Files (*.*)|*.*||";
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
	   m_symbolMapDBREF->SafeDelete();
	   delete m_symbolMapDBREF;
	   m_symbolMapDBREF = 0;
	   the_in_Archive >> m_symbolMapDBREF;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListSymbols(0);
}

void CSymbolMapDlg::OnButtonDelete() 
{
	int selection = m_symbolMapList.GetCurSel();
	if(selection == -1)
	{
		AfxMessageBox("Must select a symbol");
		return;
	}
	POSITION posLoc = m_symbolMapDBREF->FindIndex(selection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CSymbolMapperObj * symPtr = (CSymbolMapperObj *)m_symbolMapDBREF->GetAt(posLoc);
	delete symPtr;
	symPtr = 0;
	m_symbolMapDBREF->RemoveAt(posLoc);
	InListSymbols(selection-1);
}

void CSymbolMapDlg::OnButtonAdd() 
{
    UpdateData(TRUE);
	 CSymbolMapperObj * symPtr = new CSymbolMapperObj();
	 //unsigned int idTemp = m_idCharForm.GetAt(0);
	 symPtr->Calculate(m_gridDensity,m_horizUVIndex,m_vertUVIndex,m_idCharForm,m_charSpacingPercentage);
	 symPtr->m_group = m_symbolGroupID;
	  symPtr->m_extendedOption = m_optText;
	 m_symbolMapDBREF->AddTail(symPtr);

	UpdateData(FALSE);
	InListSymbols(m_symbolMapDBREF->GetCount()-1);
}

void CSymbolMapDlg::OnSelchangeLISTSymbolMapList() 
{
	int selection = m_symbolMapList.GetCurSel();
	if(selection == -1)
	{
       AfxMessageBox("Must select a symbol");
       return;
	}
	POSITION posLoc = m_symbolMapDBREF->FindIndex(selection);
	if(!posLoc)
		return;
        CSymbolMapperObj * symPtr = (CSymbolMapperObj *)m_symbolMapDBREF->GetAt(posLoc);
		UpdateData(TRUE);

		m_gridDensity = symPtr->m_gridDensity;
	    m_idCharForm = symPtr->m_uniqueID;
	    m_charSpacingPercentage = symPtr->m_percentageX;
		m_horizUVIndex = symPtr->m_origGridIndexHoriz;
	    m_vertUVIndex = symPtr->m_origGridIndexVert;
		m_symbolGroupID = symPtr->m_group;
		m_optText = symPtr->m_extendedOption;

		UpdateData(FALSE);
	
}

void CSymbolMapDlg::OnBnClickedButtonMerge()
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

   static char BASED_CODE filter[] = "Symbol Mapper Files|*.SYM|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	 CSymbolMapperObjList * m_symbolMapAdditional;
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
	   
	   the_in_Archive >> m_symbolMapAdditional;
	   the_in_Archive.Close();
       fl.Close();
	   //merge
	   for(POSITION posLoc = m_symbolMapAdditional->GetHeadPosition();posLoc != NULL;)
	   {
        CSymbolMapperObj * symPtr = (CSymbolMapperObj *)m_symbolMapAdditional->GetNext(posLoc);

		m_symbolMapDBREF->AddTail(symPtr);
	   }
       m_symbolMapAdditional->RemoveAll();
	   delete m_symbolMapAdditional;
	   m_symbolMapAdditional = 0;
	   
	 }
     SetCurrentDirectory(MainDir);
	 InListSymbols(0);
}

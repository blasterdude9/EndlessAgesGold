// StatisticsDBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "StatisticsDBDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDBDlg dialog


CStatisticsDBDlg::CStatisticsDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatisticsDBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsDBDlg)
	m_gainBasis = 0.0f;
	m_id = 0;
	m_startValue = 0.0f;
	m_statisticCap = 0.0f;
	m_statisticName = _T("");
	//}}AFX_DATA_INIT
}


void CStatisticsDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsDBDlg)
	DDX_Control(pDX, IDC_COMBO_BenefitType, m_benefitType);
	DDX_Control(pDX, IDC_LIST_Statistics, m_statisticList);
	DDX_Text(pDX, IDC_EDIT_gainBasis, m_gainBasis);
	DDX_Text(pDX, IDC_EDIT_ID, m_id);
	DDX_Text(pDX, IDC_EDIT_startValue, m_startValue);
	DDX_Text(pDX, IDC_EDIT_statisticCap, m_statisticCap);
	DDX_Text(pDX, IDC_EDIT_statisticName, m_statisticName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsDBDlg, CDialog)
	//{{AFX_MSG_MAP(CStatisticsDBDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBUTTONReplace)
	ON_LBN_SELCHANGE(IDC_LIST_Statistics, OnSelchangeLISTStatistics)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDBDlg message handlers

BOOL CStatisticsDBDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_benefitType.SetCurSel(0);
	
	InList(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatisticsDBDlg::InList(int sel)
{
	m_statisticList.ResetContent();
     for(POSITION posLoc = statisticDBREF->GetHeadPosition();posLoc!= NULL;)
	 {
         CStatObj * statObj = (CStatObj *) statisticDBREF->GetNext(posLoc);
         m_statisticList.AddString(statObj->m_statName);
	 }

	if(sel > -1)
	   m_statisticList.SetCurSel(sel);
}

void CStatisticsDBDlg::OnButtonAdd() 
{
	CStatObj * statObj = new CStatObj();
    UpdateData(TRUE);

	statObj->m_gainBasis         = m_gainBasis;
	statObj->m_id                = m_id;
	statObj->m_currentValue      = m_startValue;
	statObj->m_capValue          = m_statisticCap;
	statObj->m_statName          = m_statisticName;
    statObj->m_benefitType       = m_benefitType.GetCurSel();

	if(statObj->m_gainBasis == -1)
	   statObj->m_gainBasis = 0;

	statisticDBREF->AddTail(statObj);
	UpdateData(FALSE);
	InList((statisticDBREF->GetCount()-1));
}

void CStatisticsDBDlg::OnButtonDelete() 
{
	int selection = m_statisticList.GetCurSel();
	if(selection == -1)
	{
		MessageBox("must select a statistic");
		return;
	}

	POSITION posLoc = statisticDBREF->FindIndex(selection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CStatObj * statPtr = (CStatObj *) statisticDBREF->GetAt(posLoc);
	delete statPtr;
	statPtr = 0;

	statisticDBREF->RemoveAt(posLoc);
}

void CStatisticsDBDlg::OnBUTTONReplace() 
{
	int selection = m_statisticList.GetCurSel();
    if(selection == -1)
	{
       MessageBox("must select a statistic");
	   return;
	}

	POSITION posLoc = statisticDBREF->FindIndex(selection);
	if(!posLoc)
		return;

	UpdateData(TRUE);
        CStatObj * statPtr = (CStatObj *) statisticDBREF->GetAt(posLoc);
		statPtr->m_gainBasis         = m_gainBasis;
	    statPtr->m_id                = m_id;
	    statPtr->m_currentValue      = m_startValue;
	    statPtr->m_capValue          = m_statisticCap;
	    statPtr->m_statName          = m_statisticName;
        statPtr->m_benefitType       = m_benefitType.GetCurSel();
	UpdateData(FALSE);
	InList(selection);
}

void CStatisticsDBDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CStatisticsDBDlg::OnSelchangeLISTStatistics() 
{
 	int selection = m_statisticList.GetCurSel();
    if(selection == -1)
	return;

	POSITION posLoc = statisticDBREF->FindIndex(selection);
	if(!posLoc)
		return;

        CStatObj * statPtr = (CStatObj *) statisticDBREF->GetAt(posLoc);
         
		UpdateData(TRUE);
		
		
		m_gainBasis     = statPtr->m_gainBasis;
      	m_id            = statPtr->m_id;
	    m_startValue    = statPtr->m_currentValue;
	    m_statisticCap  = statPtr->m_capValue;
	    m_statisticName = statPtr->m_statName;

		m_benefitType.SetCurSel(statPtr->m_benefitType);

		UpdateData(FALSE);

		
}

void CStatisticsDBDlg::OnButtonLoad() 
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

   static char BASED_CODE filter[] = "Statistic Files|*.STA|All Files (*.*)|*.*||";
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
	   statisticDBREF->SafeDelete();
	   delete statisticDBREF;
	   statisticDBREF = 0;
	   the_in_Archive >> statisticDBREF;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InList(0);
}

void CStatisticsDBDlg::OnButtonSave() 
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
     static char BASED_CODE filter[] = "Statistic Files|*.STA|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "STA", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << statisticDBREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

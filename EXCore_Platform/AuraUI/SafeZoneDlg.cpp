// SafeZoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "SafeZoneDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSafeZoneDlg dialog


CSafeZoneDlg::CSafeZoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSafeZoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSafeZoneDlg)
	m_popoutBoxSizeMaxX = 0.0f;
	m_popoutBoxSizeMaxY = 0.0f;
	m_popoutBoxSizeMaxZ = 0.0f;
	m_popoutBoxSizeMinX = 0.0f;
	m_popoutBoxSizeMinY = 0.0f;
	m_popoutBoxSizeMinZ = 0.0f;
	m_worldID = 0;
	m_zoneName = _T("");
	//}}AFX_DATA_INIT
}


void CSafeZoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSafeZoneDlg)
	DDX_Control(pDX, IDC_CHECK_allowNPCconfrontationOnly, m_allowNPCconfrontationOnly);
	DDX_Control(pDX, IDC_LIST_SafeZoneDB, m_safeZoneDB);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxX, m_popoutBoxSizeMaxX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxY, m_popoutBoxSizeMaxY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxZ, m_popoutBoxSizeMaxZ);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinX, m_popoutBoxSizeMinX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinY, m_popoutBoxSizeMinY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinZ, m_popoutBoxSizeMinZ);
	DDX_Text(pDX, IDC_EDIT_WorldID, m_worldID);
	DDX_Text(pDX, IDC_EDIT_ZoneName, m_zoneName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSafeZoneDlg, CDialog)
	//{{AFX_MSG_MAP(CSafeZoneDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddZone, OnBUTTONAddZone)
	ON_BN_CLICKED(IDC_BUTTON_DeleteZone, OnBUTTONDeleteZone)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceZone, OnBUTTONReplaceZone)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_LBN_SELCHANGE(IDC_LIST_SafeZoneDB, OnSelchangeLISTSafeZoneDB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSafeZoneDlg message handlers

void CSafeZoneDlg::OnBUTTONAddZone() 
{
	CSafeZoneObj * znPtr = new CSafeZoneObj();
    UpdateData(TRUE);
	znPtr->m_worldID          = m_worldID;
	znPtr->m_zoneName         = m_zoneName;
    znPtr->m_boundingBoxMax.x = m_popoutBoxSizeMaxX;
	znPtr->m_boundingBoxMax.y = m_popoutBoxSizeMaxY;
	znPtr->m_boundingBoxMax.z = m_popoutBoxSizeMaxZ;
	znPtr->m_boundingBoxMin.x = m_popoutBoxSizeMinX;
	znPtr->m_boundingBoxMin.y = m_popoutBoxSizeMinY;
	znPtr->m_boundingBoxMin.z = m_popoutBoxSizeMinZ;
	znPtr->m_rebirthableZone  = m_allowNPCconfrontationOnly.GetCheck();
    m_safeZoneDBREF->AddTail(znPtr);
	UpdateData(FALSE);
	InListSafeZones(m_safeZoneDBREF->GetCount()-1);
}

void CSafeZoneDlg::OnBUTTONDeleteZone() 
{
	int safeZoneselection = m_safeZoneDB.GetCurSel();
	if(safeZoneselection == -1)
	{
	AfxMessageBox("Must select a zone");
	return;
	}

	POSITION posLoc = m_safeZoneDBREF->FindIndex(safeZoneselection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CSafeZoneObj * zoneObject = (CSafeZoneObj *) m_safeZoneDBREF->GetAt(posLoc);
	delete zoneObject;
	zoneObject = 0;
	m_safeZoneDBREF->RemoveAt(posLoc);
	InListSafeZones(safeZoneselection-1);
}

void CSafeZoneDlg::OnButtonLoad() 
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

   static char BASED_CODE filter[] = "Safe Zone Files|*.SZN|All Files (*.*)|*.*||";
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
	   m_safeZoneDBREF->SafeDelete();
	   delete m_safeZoneDBREF;
	   m_safeZoneDBREF = 0;
	   the_in_Archive >> m_safeZoneDBREF;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListSafeZones(0);
}

void CSafeZoneDlg::OnBUTTONReplaceZone() 
{
	int safeZoneselection = m_safeZoneDB.GetCurSel();
	if(safeZoneselection == -1)
	{
       AfxMessageBox("Must select a zone");
	   return;
	}

    POSITION posLoc = m_safeZoneDBREF->FindIndex(safeZoneselection);
	if(!posLoc)
		return;
        CSafeZoneObj * znPtr = (CSafeZoneObj *) m_safeZoneDBREF->GetAt(posLoc);
		UpdateData(TRUE);
        znPtr->m_worldID          = m_worldID;
	    znPtr->m_zoneName         = m_zoneName;
        znPtr->m_boundingBoxMax.x = m_popoutBoxSizeMaxX;
	    znPtr->m_boundingBoxMax.y = m_popoutBoxSizeMaxY;
	    znPtr->m_boundingBoxMax.z = m_popoutBoxSizeMaxZ;
	    znPtr->m_boundingBoxMin.x = m_popoutBoxSizeMinX;
	    znPtr->m_boundingBoxMin.y = m_popoutBoxSizeMinY;
	    znPtr->m_boundingBoxMin.z = m_popoutBoxSizeMinZ;
		znPtr->m_rebirthableZone  = m_allowNPCconfrontationOnly.GetCheck();
        UpdateData(FALSE);
		InListSafeZones(safeZoneselection);
}

void CSafeZoneDlg::OnButtonSave() 
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
     static char BASED_CODE filter[] = "Sank Zone Files|*.SZN|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "SZN", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_safeZoneDBREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
	
}

BOOL CSafeZoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	



	InListSafeZones(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSafeZoneDlg::InListSafeZones(int sel)
{
	UpdateData(TRUE);
	m_safeZoneDB.ResetContent();
     for(POSITION posLoc = m_safeZoneDBREF->GetHeadPosition();posLoc != NULL;)
	 {
         CSafeZoneObj * znPtr = (CSafeZoneObj *) m_safeZoneDBREF->GetNext(posLoc);
         m_safeZoneDB.AddString(znPtr->m_zoneName);
	 }

	if(sel > -1)
	   m_safeZoneDB.SetCurSel(sel);

	UpdateData(FALSE);
	return TRUE;
}


void CSafeZoneDlg::OnSelchangeLISTSafeZoneDB() 
{
	int safeZoneselection = m_safeZoneDB.GetCurSel();
	if(safeZoneselection == -1)
	{
	   return;
	}

    POSITION posLoc = m_safeZoneDBREF->FindIndex(safeZoneselection);
	if(!posLoc)
		return;
        CSafeZoneObj * zoneObject = (CSafeZoneObj *) m_safeZoneDBREF->GetAt(posLoc);
		UpdateData(TRUE);
        m_worldID                 = zoneObject->m_worldID;
	    m_zoneName                = zoneObject->m_zoneName;
        m_popoutBoxSizeMaxX       = zoneObject->m_boundingBoxMax.x;
	    m_popoutBoxSizeMaxY       = zoneObject->m_boundingBoxMax.y;
	    m_popoutBoxSizeMaxZ       = zoneObject->m_boundingBoxMax.z;
	    m_popoutBoxSizeMinX       = zoneObject->m_boundingBoxMin.x;
	    m_popoutBoxSizeMinY       = zoneObject->m_boundingBoxMin.y;
	    m_popoutBoxSizeMinZ       = zoneObject->m_boundingBoxMin.z;
		m_allowNPCconfrontationOnly.SetCheck(zoneObject->m_rebirthableZone);
        UpdateData(FALSE);
}

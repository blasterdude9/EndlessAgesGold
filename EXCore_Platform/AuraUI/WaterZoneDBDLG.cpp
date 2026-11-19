// WaterZoneDBDLG.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include <afxtempl.h>
#include "direct.h"
#include "WaterZoneDBDLG.h"
#include "CEXMeshClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaterZoneDBDLG dialog


CWaterZoneDBDLG::CWaterZoneDBDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CWaterZoneDBDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWaterZoneDBDLG)
	m_maxX = 0.0f;
	m_maxY = 0.0f;
	m_maxZ = 0.0f;
	m_minX = 0.0f;
	m_minY = 0.0f;
	m_minZ = 0.0f;
	m_zoneName = _T("");
	//}}AFX_DATA_INIT
}


void CWaterZoneDBDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWaterZoneDBDLG)
	DDX_Control(pDX, IDC_LIST_WaterZoneDB, m_waterZoneDB);
	DDX_Control(pDX, IDC_CHECK_IsExclusionZone, m_Chk_IsExclusionZn);
	DDX_Text(pDX, IDC_EDIT_MaxX, m_maxX);
	DDX_Text(pDX, IDC_EDIT_MaxY, m_maxY);
	DDX_Text(pDX, IDC_EDIT_MaxZ, m_maxZ);
	DDX_Text(pDX, IDC_EDIT_MinX, m_minX);
	DDX_Text(pDX, IDC_EDIT_MinY, m_minY);
	DDX_Text(pDX, IDC_EDIT_MinZ, m_minZ);
	DDX_Text(pDX, IDC_EDIT_ZoneName, m_zoneName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWaterZoneDBDLG, CDialog)
	//{{AFX_MSG_MAP(CWaterZoneDBDLG)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBUTTONReplace)
	ON_BN_CLICKED(IDC_BUTTON_LoadDB, OnBUTTONLoadDB)
	ON_BN_CLICKED(IDC_BUTTON_LOADFROMARW, OnButtonLoadfromarw)
	ON_BN_CLICKED(IDC_BUTTON_SaveDB, OnBUTTONSaveDB)
	ON_LBN_SELCHANGE(IDC_LIST_WaterZoneDB, OnSelchangeLISTWaterZoneDB)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ADDWaterZone, OnBnClickedButtonAddwaterzone)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaterZoneDBDLG message handlers

BOOL CWaterZoneDBDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListWaterZones(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWaterZoneDBDLG::InListWaterZones(int sel)
{
	UpdateData(TRUE);
    m_waterZoneDB.ResetContent();
    for(POSITION posLoc = m_waterZnDBREF->GetHeadPosition();posLoc != NULL;)
	{
        CWaterZoneObj * wzPtr = (CWaterZoneObj *) m_waterZnDBREF->GetNext(posLoc);
        m_waterZoneDB.AddString(wzPtr->m_waterZoneName);
	}
	if(sel > -1)
	   m_waterZoneDB.SetCurSel(sel);
	UpdateData(FALSE);
}

void CWaterZoneDBDLG::OnButtonAdd() 
{
	
}

void CWaterZoneDBDLG::OnButtonDelete() 
{
	int selection = m_waterZoneDB.GetCurSel();
	if(selection == -1)
	{
		MessageBox("Must select a water zone");
		return;
	}

	POSITION posLoc = m_waterZnDBREF->FindIndex(selection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CWaterZoneObj * wzPtr = (CWaterZoneObj *) m_waterZnDBREF->GetAt(posLoc);
	delete wzPtr;
	wzPtr = 0;
	m_waterZnDBREF->RemoveAt(posLoc);
	InListWaterZones(selection-1);
}

void CWaterZoneDBDLG::OnBUTTONReplace() 
{
	int selection = m_waterZoneDB.GetCurSel();
	if(selection == -1)
	{
       MessageBox("Must select a water zone");
	   return;
	}

    POSITION posLoc = m_waterZnDBREF->FindIndex(selection);
	if(!posLoc)
		return;
        CWaterZoneObj * wzPtr = (CWaterZoneObj *) m_waterZnDBREF->GetAt(posLoc);

		UpdateData(TRUE);
		wzPtr->m_isExclusionZone = m_Chk_IsExclusionZn.GetCheck();
	    wzPtr->m_waterArea.maxX = m_maxX;
	    wzPtr->m_waterArea.maxY = m_maxY;
	    wzPtr->m_waterArea.maxZ = m_maxZ;
	    wzPtr->m_waterArea.minX = m_minX;
	    wzPtr->m_waterArea.minY = m_minY;
	    wzPtr->m_waterArea.minZ = m_minZ;
	    wzPtr->m_waterZoneName  = m_zoneName;
		UpdateData(FALSE);
		InListWaterZones(selection);
	
}

void CWaterZoneDBDLG::OnBUTTONLoadDB() 
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

     static char BASED_CODE filter[] = "Water Zone Files|*.WTZ|All Files (*.*)|*.*||";
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
	   //AFX_MANAGE_STATE(AfxGetStaticModuleState());
	   CArchive the_in_Archive(&fl,CArchive::load);
	   m_waterZnDBREF->SafeDelete();
	   delete m_waterZnDBREF;
	   m_waterZnDBREF = 0;
	   
	  
	   the_in_Archive >> m_waterZnDBREF;
     
	   
	   the_in_Archive.Close();
       fl.Close();
	   AfxMessageBox("loaded");
	   *m_waterZnDBREF_memloc = m_waterZnDBREF;
	 }
     SetCurrentDirectory(MainDir);
	 InListWaterZones(0);
}

void CWaterZoneDBDLG::OnButtonLoadfromarw() 
{
 //use mapsmodels folder for organization
 CString CurDir;
 CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
 //end use mapsmodels folder for organization
 static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
 if ( opendialog.DoModal() == IDOK )
 {
	   
	CEXMeshObj * newMesh = new CEXMeshObj();
	newMesh->List_materialObject = new CMaterialObject();
	float sightRangeDump = 0.0f;
    newMesh->LoadGeometryFromPlugin(opendialog.GetPathName(),&sightRangeDump,g_pd3dDeviceRef,NULL);
    UpdateData(TRUE);
	m_maxX = newMesh->List_boundingBox.maxX;
	m_maxY = newMesh->List_boundingBox.maxY;
	m_maxZ = newMesh->List_boundingBox.maxZ;
    m_minX = newMesh->List_boundingBox.minX;
	m_minY = newMesh->List_boundingBox.minY;
	m_minZ = newMesh->List_boundingBox.minZ;
	UpdateData(FALSE);
	newMesh->SafeDelete();
	delete newMesh;
	newMesh = 0;
 }
 SetCurrentDirectory(MainDir);
}

void CWaterZoneDBDLG::OnBUTTONSaveDB() 
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
     static char BASED_CODE filter[] = "Water Zone DB Files|*.WTZ|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "WTZ", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_waterZnDBREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CWaterZoneDBDLG::OnSelchangeLISTWaterZoneDB() 
{
	int selection = m_waterZoneDB.GetCurSel();
	if(selection == -1)
	{
       MessageBox("Must select a water zone");
	   return;
	}

    POSITION posLoc = m_waterZnDBREF->FindIndex(selection);
	if(!posLoc)
		return;
        CWaterZoneObj * wzPtr = (CWaterZoneObj *) m_waterZnDBREF->GetAt(posLoc);

		UpdateData(TRUE);
		m_Chk_IsExclusionZn.SetCheck(wzPtr->m_isExclusionZone);
	    m_maxX =     wzPtr->m_waterArea.maxX;
	    m_maxY =     wzPtr->m_waterArea.maxY;
	    m_maxZ =     wzPtr->m_waterArea.maxZ;
	    m_minX =     wzPtr->m_waterArea.minX;
	    m_minY =     wzPtr->m_waterArea.minY;
	    m_minZ =     wzPtr->m_waterArea.minZ;
	    m_zoneName = wzPtr->m_waterZoneName;
		UpdateData(FALSE);
}

void CWaterZoneDBDLG::OnBnClickedButtonAddwaterzone()
{
	UpdateData(TRUE);
	CWaterZoneObj * wzPtr = new CWaterZoneObj();
	
	wzPtr->m_isExclusionZone = m_Chk_IsExclusionZn.GetCheck();
	wzPtr->m_waterArea.maxX = m_maxX;
	wzPtr->m_waterArea.maxY = m_maxY;
	wzPtr->m_waterArea.maxZ = m_maxZ;
	wzPtr->m_waterArea.minX = m_minX;
	wzPtr->m_waterArea.minY = m_minY;
	wzPtr->m_waterArea.minZ = m_minZ;
	wzPtr->m_waterZoneName  = m_zoneName;
    
    m_waterZnDBREF->AddTail(wzPtr);

    UpdateData(FALSE);
	InListWaterZones(m_waterZnDBREF->GetCount());
}



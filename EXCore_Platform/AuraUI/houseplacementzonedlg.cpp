// HousePlacementZoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "HousePlacementZoneDlg.h"
#include "direct.h"
#include "CEXMeshClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHousePlacementZoneDlg dialog


CHousePlacementZoneDlg::CHousePlacementZoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHousePlacementZoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHousePlacementZoneDlg)
	m_zoneName = _T("");
	m_worldID = 0;
	m_subDivisionZ = 0.0f;
	m_subDivisionY = 0.0f;
	m_subDivisionX = 0.0f;
	m_density = 0.0f;
	//}}AFX_DATA_INIT
}


void CHousePlacementZoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHousePlacementZoneDlg)
	DDX_Control(pDX, IDC_LIST_ZonesDB, m_zonesDB);
	DDX_Text(pDX, IDC_EDIT_ZoneName, m_zoneName);
	DDX_Text(pDX, IDC_EDIT_WorldID, m_worldID);
	DDX_Text(pDX, IDC_EDIT_subDivisionZ, m_subDivisionZ);
	DDX_Text(pDX, IDC_EDIT_subDivisionY, m_subDivisionY);
	DDX_Text(pDX, IDC_EDIT_subDivisionX, m_subDivisionX);
	DDX_Text(pDX, IDC_EDIT_density, m_density);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_HousePlacements, m_housePlacementsList);
	DDX_Control(pDX, IDC_LIST_HouseVault, m_houseVaultInv);
	DDX_Control(pDX, IDC_LIST_HouseStore, m_storeInv);
}


BEGIN_MESSAGE_MAP(CHousePlacementZoneDlg, CDialog)
	//{{AFX_MSG_MAP(CHousePlacementZoneDlg)
	ON_BN_CLICKED(IDC_BTN_AddZone, OnBTNAddZone)
	ON_BN_CLICKED(IDC_BTN_DeleteZone, OnBTNDeleteZone)
	ON_BN_CLICKED(IDC_BTN_ReplaceZone, OnBTNReplaceZone)
	ON_BN_CLICKED(IDC_BTN_ZN_Save, OnBTNZNSave)
	ON_BN_CLICKED(IDC_BTN_ZN_Load, OnBTNZNLoad)
	ON_BN_CLICKED(IDC_BTN_LoadVisibleBox, OnBTNLoadVisibleBox)
	ON_BN_CLICKED(IDC_BUTTON_LoadZoneBox, OnBUTTONLoadZoneBox)
	ON_LBN_SELCHANGE(IDC_LIST_ZonesDB, OnSelchangeLISTZonesDB)
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_LIST_HousePlacements, OnLbnSelchangeListHouseplacements)
	ON_BN_CLICKED(IDC_BTN_ExportToSQLHSD, OnBnClickedBtnExporttosqlhsd)
	ON_BN_CLICKED(IDC_BTN_ImportFromSQLHSD, OnBnClickedBtnImportfromsqlhsd)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHousePlacementZoneDlg message handlers

void CHousePlacementZoneDlg::OnBTNAddZone() 
{
	CHousingZone * hsZone = new CHousingZone();
    BNDBOX box;
    ZeroMemory(&box,sizeof(BNDBOX));
	
    UpdateData(TRUE);
	hsZone->m_densityAllowance = m_density;
	hsZone->CreateNewHouseingZone(m_zoneName,m_subDivisionX,m_subDivisionY,m_subDivisionZ,m_worldID,box,box);
	UpdateData(FALSE);
	m_housingZonesDB->AddTail(hsZone);
	InList(m_housingZonesDB->GetCount()-1);
}

void CHousePlacementZoneDlg::OnBTNDeleteZone() 
{
	int selection = m_zonesDB.GetCurSel();
	if(selection == -1)
	{
		AfxMessageBox("must select a zone");
		return;
	}
	
	POSITION posLoc = m_housingZonesDB->FindIndex(selection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CHousingZone * hsZone = (CHousingZone *)m_housingZonesDB->GetAt(posLoc);
	hsZone->SafeDelete();
	delete hsZone;
	hsZone = 0;
	m_housingZonesDB->RemoveAt(posLoc);
	InList(selection-1);
}

void CHousePlacementZoneDlg::OnBTNReplaceZone() 
{
    int selection = m_zonesDB.GetCurSel();
	if(selection == -1)
	{
       AfxMessageBox("must select a zone");
       return;
	}
    POSITION posLoc = m_housingZonesDB->FindIndex(selection);
	if(!posLoc)
	    return;
        
	    UpdateData(TRUE);
	    CHousingZone * hsZone = (CHousingZone *)m_housingZonesDB->GetAt(posLoc);
		hsZone->m_densityAllowance = m_density;
		hsZone->CreateNewHouseingZone(m_zoneName,
			                          m_subDivisionX,
									  m_subDivisionY,
									  m_subDivisionZ,
									  m_worldID,
									  hsZone->m_zoneBox,
									  hsZone->m_visibleBox);
		UpdateData(FALSE);
		InList(selection);
	
}

BOOL CHousePlacementZoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InList(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHousePlacementZoneDlg::InList(int sel)
{
	m_zonesDB.ResetContent();
    for(POSITION posLoc = m_housingZonesDB->GetHeadPosition();posLoc != NULL;)
	{
       CHousingZone * hsZone = (CHousingZone *) m_housingZonesDB->GetNext(posLoc);
       if(hsZone)
	   {
          m_zonesDB.AddString(hsZone->m_zoneLabel);
	   }
	}

	if(sel > -1)
	   m_zonesDB.SetCurSel(sel);
}

void CHousePlacementZoneDlg::OnBTNZNSave() 
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
     static char BASED_CODE filter[] = "House Placement DB Files|*.HPB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "BZN", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_housingZonesDB;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CHousePlacementZoneDlg::OnBTNZNLoad() 
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

   static char BASED_CODE filter[] = "House Placement DB Files|*.HPB|All Files (*.*)|*.*||";
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
	   m_housingZonesDB->SafeDelete();
	   delete m_housingZonesDB;
	   m_housingZonesDB = 0;
	   the_in_Archive >> m_housingZonesDB;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InList(0);
}

void CHousePlacementZoneDlg::OnBTNLoadVisibleBox() 
{
	int selection = m_zonesDB.GetCurSel();
	if(selection == -1)
	{
       AfxMessageBox("must select a zone");
       return;
	}
    POSITION posLoc = m_housingZonesDB->FindIndex(selection);
	if(!posLoc)
	    return;

	    CHousingZone * hsZone = (CHousingZone *)m_housingZonesDB->GetAt(posLoc);
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
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   UpdateData(FALSE);

       CEXMeshObj * m_mesh = new CEXMeshObj();
	   float dist = 0.0f;
       if(m_mesh->LoadGeometryFromPlugin(fileName,&dist,g_pd3dDeviceRef,NULL))
	   {//1
          UpdateData(TRUE);
          hsZone->m_visibleBox.max.x = m_mesh->List_boundingBox.maxX;
          hsZone->m_visibleBox.max.y = m_mesh->List_boundingBox.maxY;
          hsZone->m_visibleBox.max.z = m_mesh->List_boundingBox.maxZ;
          hsZone->m_visibleBox.min.x = m_mesh->List_boundingBox.minX;
          hsZone->m_visibleBox.min.y = m_mesh->List_boundingBox.minY;
          hsZone->m_visibleBox.min.z = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

void CHousePlacementZoneDlg::OnBUTTONLoadZoneBox() 
{
	int selection = m_zonesDB.GetCurSel();
	if(selection == -1)
	{
       AfxMessageBox("must select a zone");
       return;
	}
    POSITION posLoc = m_housingZonesDB->FindIndex(selection);
	if(!posLoc)
	    return;

	    CHousingZone * hsZone = (CHousingZone *)m_housingZonesDB->GetAt(posLoc);
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
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   UpdateData(FALSE);

       CEXMeshObj * m_mesh = new CEXMeshObj();
	   float dist = 0.0f;
       if(m_mesh->LoadGeometryFromPlugin(fileName,&dist,g_pd3dDeviceRef,NULL))
	   {//1
          UpdateData(TRUE);
          hsZone->m_zoneBox.max.x = m_mesh->List_boundingBox.maxX;
          hsZone->m_zoneBox.max.y = m_mesh->List_boundingBox.maxY;
          hsZone->m_zoneBox.max.z = m_mesh->List_boundingBox.maxZ;
          hsZone->m_zoneBox.min.x = m_mesh->List_boundingBox.minX;
          hsZone->m_zoneBox.min.y = m_mesh->List_boundingBox.minY;
          hsZone->m_zoneBox.min.z = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

void CHousePlacementZoneDlg::OnSelchangeLISTZonesDB() 
{
	int selection = m_zonesDB.GetCurSel();
	if(selection == -1)
	{
       return;
	}
    POSITION posLoc = m_housingZonesDB->FindIndex(selection);
	if(!posLoc)
	    return;

	    CHousingZone * hsZone = (CHousingZone *)m_housingZonesDB->GetAt(posLoc);

		UpdateData(TRUE);
		m_zoneName           = hsZone->m_zoneLabel;
	    m_subDivisionX       = hsZone->m_subDivisionX;
	    m_subDivisionY       = hsZone->m_subDivisionY;
	    m_subDivisionZ       = hsZone->m_subDivisionZ;
	    m_worldID            = hsZone->m_wldID;
		m_density            = hsZone->m_densityAllowance;
	    
		
		m_housePlacementsList.ResetContent();
		if(hsZone->m_houseDatabase)
		for(POSITION posHs = hsZone->m_houseDatabase->GetHeadPosition();posHs != NULL;)
		{
			CHPlacementObj * hsPlc = (CHPlacementObj *) hsZone->m_houseDatabase->GetNext(posHs);
			m_housePlacementsList.AddString("House");
		}
		
		
		
		
		UpdateData(FALSE);
}

void CHousePlacementZoneDlg::OnLbnSelchangeListHouseplacements()
{
    m_houseVaultInv.ResetContent();
	m_storeInv.ResetContent();
    int selection = m_zonesDB.GetCurSel();
	if(selection == -1)
	{
       return;
	}
    POSITION posLoc = m_housingZonesDB->FindIndex(selection);
	if(!posLoc)
	    return;

	CHousingZone * hsZone = (CHousingZone *)m_housingZonesDB->GetAt(posLoc);
	if(!hsZone->m_houseDatabase)
		return;

	int selection2 = m_housePlacementsList.GetCurSel();
	if(selection2 == -1)
	{
       return;
	}

    POSITION posHs = hsZone->m_houseDatabase->FindIndex(selection2);
    if(!posHs)
		return;
	CHPlacementObj * hsPlc = (CHPlacementObj *) hsZone->m_houseDatabase->GetAt(posHs);
	
    if(hsPlc->m_vaultInventory)
       for(POSITION vPos = hsPlc->m_vaultInventory->GetHeadPosition();vPos != NULL;)
	   {
           CInventoryObj * objPtr = (CInventoryObj *) hsPlc->m_vaultInventory->GetNext(vPos);

		   m_houseVaultInv.AddString(objPtr->m_itemData->m_itemName);
	   }

    if(hsPlc->m_storeInventory)
       for(POSITION vPos = hsPlc->m_storeInventory->GetHeadPosition();vPos != NULL;)
	   {
           CInventoryObj * objPtr = (CInventoryObj *) hsPlc->m_storeInventory->GetNext(vPos);

		   m_storeInv.AddString(objPtr->m_itemData->m_itemName);
	   }


}

void CHousePlacementZoneDlg::OnBnClickedBtnExporttosqlhsd()
{
	m_housingZonesDB->ExportToSQL(NULL);
}

void CHousePlacementZoneDlg::OnBnClickedBtnImportfromsqlhsd()
{
	
}

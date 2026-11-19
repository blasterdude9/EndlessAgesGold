// CommerceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "CommerceDlg.h"
#include "direct.h"
#include "CEXMeshClass.h"
#include "globalstatusvariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommerceDlg dialog


CCommerceDlg::CCommerceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommerceDlg::IDD, pParent)
	, m_raceFilterID(0)
	, m_bumpAmount(0)
{
	//{{AFX_DATA_INIT(CCommerceDlg)
	m_storeID = 0;
	m_storeName = _T("");
	m_popoutBoxSizeMaxX = 0.0f;
	m_popoutBoxSizeMaxY = 0.0f;
	m_popoutBoxSizeMaxZ = 0.0f;
	m_popoutBoxSizeMinX = 0.0f;
	m_popoutBoxSizeMinY = 0.0f;
	m_popoutBoxSizeMinZ = 0.0f;
	m_worldID = 0;
	m_menuBind = 0;
	//}}AFX_DATA_INIT
}


void CCommerceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommerceDlg)
	DDX_Control(pDX, IDC_CHECK_willDealWithMurderers, m_willDealWithMurderers);
	DDX_Control(pDX, IDC_LIST_StoreInventory, m_storeInventory);
	DDX_Control(pDX, IDC_LIST_GlobalInventory, m_globalInventory);
	DDX_Control(pDX, IDC_LIST_CommerceDB, m_commerceDB);
	DDX_Text(pDX, IDC_EDIT_StoreID, m_storeID);
	DDX_Text(pDX, IDC_EDIT_StoreName, m_storeName);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxX, m_popoutBoxSizeMaxX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxY, m_popoutBoxSizeMaxY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxZ, m_popoutBoxSizeMaxZ);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinX, m_popoutBoxSizeMinX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinY, m_popoutBoxSizeMinY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinZ, m_popoutBoxSizeMinZ);
	DDX_Text(pDX, IDC_EDIT_WorldID, m_worldID);
	DDX_Text(pDX, IDC_EDIT_MenuBind, m_menuBind);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_RaceSpecificFilter, m_raceFilterID);
	DDX_Text(pDX, IDC_EDIT_BumpAmount, m_bumpAmount);
}


BEGIN_MESSAGE_MAP(CCommerceDlg, CDialog)
	//{{AFX_MSG_MAP(CCommerceDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddItem, OnBUTTONAddItem)
	ON_BN_CLICKED(IDC_BUTTON_AddStore, OnBUTTONAddStore)
	ON_BN_CLICKED(IDC_BUTTON_DeleteItem, OnBUTTONDeleteItem)
	ON_BN_CLICKED(IDC_BUTTON_DeleteStore, OnBUTTONDeleteStore)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceStore, OnBUTTONReplaceStore)
	ON_LBN_SELCHANGE(IDC_LIST_CommerceDB, OnSelchangeLISTCommerceDB)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_CHECK_willDealWithMurderers, OnCHECKwillDealWithMurderers)
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_LIST_StoreInventory, OnLbnSelchangeListStoreinventory)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceItem, OnBnClickedButtonReplaceitem)
	ON_BN_CLICKED(IDC_BUTTON_LoadBox, OnBnClickedButtonLoadbox)
	ON_BN_CLICKED(IDC_BUTTON_InsertItem, OnBnClickedButtonInsertitem)
	ON_BN_CLICKED(IDC_BUTTON_BumpBox, OnBnClickedButtonBumpbox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommerceDlg message handlers
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
BOOL CCommerceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
    UpdateData(TRUE);
	
	for(unsigned int cLoop=0;cLoop<gStatus->m_itemManager->m_cellCount;cLoop++)
	{
		 for(CAxLinkObj * pos = gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		 {
			 CItemObj * iPtr = (CItemObj *) gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
			 m_globalInventory.AddString(iPtr->m_itemName);
		 }
	}	
	m_globalInventory.SetCurSel(0);

    m_raceFilterID = -1;
	UpdateData(FALSE);

	InListCommerceDB(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
void CCommerceDlg::InListCommerceDB(int sel)
{
	
    UpdateData(TRUE);
	m_commerceDB.ResetContent();
	for(POSITION posLoc = m_commerceObjDBRef->GetHeadPosition();posLoc != NULL;)
	{
	    CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetNext(posLoc);
	    m_commerceDB.AddString(cPtr->m_establishmentName);
	}
	UpdateData(FALSE);

    if(sel > -1)
	m_commerceDB.SetCurSel(sel);
}
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
void CCommerceDlg::InListStoreInventory(int sel)
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
    int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{
       MessageBox("Must select a store");
	   return;
	}

	m_storeInventory.ResetContent();
	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	    CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
        
		
		for(POSITION invPos = cPtr->m_storeInventory->GetHeadPosition();invPos != NULL;)
		{
            CStoreInventoryObj * invPtr = (CStoreInventoryObj *)cPtr->m_storeInventory->GetNext(invPos);
			CItemObj * glRefPtr = gStatus->m_itemManager->GetItemByID(invPtr->m_globalInventoryID);
			
			if(glRefPtr)
			{
               m_storeInventory.AddString(glRefPtr->m_itemName);
			}
			else{
               m_storeInventory.AddString("Unknown Object");
			}
		}

		if(sel > -1)
		m_storeInventory.SetCurSel(sel);
}
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
void CCommerceDlg::OnBUTTONAddItem() 
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
    int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{
       MessageBox("Must select a store");
	   return;
	}

	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	    CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
		
		int globalInvObj = m_globalInventory.GetCurSel();
		if(globalInvObj == -1)
		{
           MessageBox("Must Select a  Global Inventory Object");
		   return;
		}
        CString itemName;
		m_globalInventory.GetText(globalInvObj,itemName);
        

		CItemObj * glPtr = gStatus->m_itemManager->GetItemByName(itemName.GetBuffer());
		
		if(!glPtr)
			return;

		    UpdateData(TRUE);
			CStoreInventoryObj * invPtr = new CStoreInventoryObj();
            invPtr->m_globalInventoryID = glPtr->m_globalID;
			invPtr->m_raceID = m_raceFilterID;
		 
		    cPtr->m_storeInventory->AddTail(invPtr);
            UpdateData(FALSE);
			InListStoreInventory(cPtr->m_storeInventory->GetCount()-1);
}
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
void CCommerceDlg::OnBUTTONAddStore() 
{
	UpdateData(TRUE);

    CCommerceObj * cPtr = new CCommerceObj();
	cPtr->m_uniqueID          = m_storeID;
	cPtr->m_establishmentName = m_storeName;
    cPtr->m_boundingBoxMax.x  = m_popoutBoxSizeMaxX;
    cPtr->m_boundingBoxMax.y  = m_popoutBoxSizeMaxY;
    cPtr->m_boundingBoxMax.z  = m_popoutBoxSizeMaxZ;
    cPtr->m_boundingBoxMin.x  = m_popoutBoxSizeMinX;
    cPtr->m_boundingBoxMin.y  = m_popoutBoxSizeMinY;
    cPtr->m_boundingBoxMin.z  = m_popoutBoxSizeMinZ;
    cPtr->m_worldID           = m_worldID;
	cPtr->m_menuBind          = m_menuBind;
	cPtr->m_storeInventory    = new CStoreInventoryObjList();
	m_commerceObjDBRef->AddTail(cPtr);

	UpdateData(FALSE);

	InListCommerceDB(0);
}
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
void CCommerceDlg::OnBUTTONDeleteItem() 
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{
		MessageBox("Must select a store");
		return;
	}

	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	
	CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
	int storeInvObj = m_storeInventory.GetCurSel();
	if(storeInvObj == -1)
	{
		MessageBox("Must Select a  store Inventory Object");
		return;
	}

	POSITION invPos = cPtr->m_storeInventory->FindIndex(storeInvObj);
	if(!invPos)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CStoreInventoryObj * invPtr = (CStoreInventoryObj *)cPtr->m_storeInventory->GetAt(invPos);
	invPtr->SafeDelete();
	delete invPtr;
	invPtr = 0;
	cPtr->m_storeInventory->RemoveAt(invPos);

	InListStoreInventory(storeInvObj-1);
}
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
void CCommerceDlg::OnBUTTONDeleteStore() 
{
	int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{
		MessageBox("Must select a store");
		return;
	}

	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
	cPtr->SafeDelete();
	delete cPtr;
	cPtr = 0;
	m_commerceObjDBRef->RemoveAt(posLoc);

	InListCommerceDB(storeSelection-1);	
}
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
void CCommerceDlg::OnBUTTONReplaceStore() 
{
	int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{
       MessageBox("Must select a store");
	   return;
	}

	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	    CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
	    UpdateData(TRUE);
		cPtr->m_uniqueID          = m_storeID;
	    cPtr->m_establishmentName = m_storeName;
		cPtr->m_boundingBoxMax.x  = m_popoutBoxSizeMaxX;
		cPtr->m_boundingBoxMax.y  = m_popoutBoxSizeMaxY;
		cPtr->m_boundingBoxMax.z  = m_popoutBoxSizeMaxZ;
        cPtr->m_boundingBoxMin.x  = m_popoutBoxSizeMinX;
		cPtr->m_boundingBoxMin.y  = m_popoutBoxSizeMinY;
		cPtr->m_boundingBoxMin.z  = m_popoutBoxSizeMinZ;
	    cPtr->m_worldID           = m_worldID;
		cPtr->m_menuBind          = m_menuBind;
		UpdateData(FALSE);

		InListCommerceDB(storeSelection);
}
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
void CCommerceDlg::OnSelchangeLISTCommerceDB() 
{
	int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{
	   return;
	}

	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	    CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
	    UpdateData(TRUE);
		m_storeID           = cPtr->m_uniqueID;
	    m_storeName         = cPtr->m_establishmentName;
		m_popoutBoxSizeMaxX = cPtr->m_boundingBoxMax.x;
        m_popoutBoxSizeMaxY = cPtr->m_boundingBoxMax.y;
        m_popoutBoxSizeMaxZ = cPtr->m_boundingBoxMax.z;
        m_popoutBoxSizeMinX = cPtr->m_boundingBoxMin.x;
        m_popoutBoxSizeMinY = cPtr->m_boundingBoxMin.y;
        m_popoutBoxSizeMinZ = cPtr->m_boundingBoxMin.z;
        m_worldID           = cPtr->m_worldID;
		m_menuBind          = cPtr->m_menuBind;

		if(cPtr->m_willDealWithCriminals)
           m_willDealWithMurderers.SetCheck(1);
		else
           m_willDealWithMurderers.SetCheck(0);

		UpdateData(FALSE);

		InListStoreInventory(0);
}

void CCommerceDlg::OnButtonLoad() 
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

   static char BASED_CODE filter[] = "Commerce DB Files|*.CDB|All Files (*.*)|*.*||";
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
	   m_commerceObjDBRef->SafeDelete();
	   delete m_commerceObjDBRef;
	   m_commerceObjDBRef = 0;
	   the_in_Archive >> m_commerceObjDBRef;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListCommerceDB(0);
}

void CCommerceDlg::OnButtonSave() 
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
     static char BASED_CODE filter[] = "CDB Library Files|*.CDB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "CDB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_commerceObjDBRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CCommerceDlg::OnCHECKwillDealWithMurderers() 
{
	int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{
	   return;
	}

	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	    CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
    UpdateData(TRUE);
	if(cPtr->m_willDealWithCriminals)
	   cPtr->m_willDealWithCriminals = FALSE;
	else
	   cPtr->m_willDealWithCriminals = TRUE;
	UpdateData(FALSE);
	
}

void CCommerceDlg::OnLbnSelchangeListStoreinventory()
{
    int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{

	   return;
	}

	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	    CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
		

		int storeInvObj = m_storeInventory.GetCurSel();
		if(storeInvObj == -1)
		{
		   return;
		}

		POSITION invPos = cPtr->m_storeInventory->FindIndex(storeInvObj);
		if(!invPos)
			return;
		    CStoreInventoryObj * invPtr = (CStoreInventoryObj *)cPtr->m_storeInventory->GetAt(invPos);
		 
			UpdateData(TRUE);
			m_raceFilterID = invPtr->m_raceID;
		    UpdateData(FALSE);
}

void CCommerceDlg::OnBnClickedButtonReplaceitem()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{
       MessageBox("Must select a store");
	   return;
	}

	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	    CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
		

		int storeInvObj = m_storeInventory.GetCurSel();
		if(storeInvObj == -1)
		{
           MessageBox("Must Select a  store Inventory Object");
		   return;
		}

		POSITION invPos = cPtr->m_storeInventory->FindIndex(storeInvObj);
		if(!invPos)
			return;
		    CStoreInventoryObj * invPtr = (CStoreInventoryObj *)cPtr->m_storeInventory->GetAt(invPos);
		 
			int globalInvObj = m_globalInventory.GetCurSel();
		    if(globalInvObj == -1)
		    {
               MessageBox("Must Select a  Global Inventory Object");
		       return;
		    }
            CString miscText;
			m_globalInventory.GetText(globalInvObj,miscText);

			CItemObj * glPtr = gStatus->m_itemManager->GetItemByName(miscText.GetBuffer());
		    if(!glPtr)
			    return;


				UpdateData(TRUE);
				invPtr->m_globalInventoryID = glPtr->m_globalID;
			    invPtr->m_raceID = m_raceFilterID;
                UpdateData(FALSE);

			InListStoreInventory(storeInvObj);
	
}

void CCommerceDlg::OnBnClickedButtonLoadbox()
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
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   UpdateData(FALSE);

       CEXMeshObj * m_mesh = new CEXMeshObj();
	   m_mesh->List_materialObject = new CMaterialObject();
	   float dist = 0.0f;
       if(m_mesh->LoadGeometryFromPlugin(fileName,&dist,NULL,NULL))
	   {//1
          UpdateData(TRUE);
          m_popoutBoxSizeMaxX = m_mesh->List_boundingBox.maxX;
          m_popoutBoxSizeMinX = m_mesh->List_boundingBox.minX;
          m_popoutBoxSizeMaxY = m_mesh->List_boundingBox.maxY;
          m_popoutBoxSizeMinY = m_mesh->List_boundingBox.minY;
          m_popoutBoxSizeMaxZ = m_mesh->List_boundingBox.maxZ;
          m_popoutBoxSizeMinZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

void CCommerceDlg::OnBnClickedButtonInsertitem()
{
	int storeSelection = m_commerceDB.GetCurSel();
	if(storeSelection == -1)
	{
       MessageBox("Must select a store");
	   return;
	}

	int inventoryItemSel = m_storeInventory.GetCurSel();
	if(inventoryItemSel == -1)
	{
       MessageBox("Must select a store item");
	   return;
	}

	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	POSITION posLoc = m_commerceObjDBRef->FindIndex(storeSelection);
	if(!posLoc)
		return;
	    CCommerceObj * cPtr = (CCommerceObj *)m_commerceObjDBRef->GetAt(posLoc);
		
		int globalInvObj = m_globalInventory.GetCurSel();
		if(globalInvObj == -1)
		{
           MessageBox("Must Select a  Global Inventory Object");
		   return;
		}

		CString miscText;
		m_globalInventory.GetText(globalInvObj,miscText);

		CItemObj * glPtr = gStatus->m_itemManager->GetItemByName(miscText.GetBuffer());
		if(!glPtr)
			return;
		    
		    UpdateData(TRUE);
			POSITION posInsert = cPtr->m_storeInventory->FindIndex(inventoryItemSel);
			if(posInsert)
			{
			
			CStoreInventoryObj * invPtr = new CStoreInventoryObj();
            invPtr->m_globalInventoryID = glPtr->m_globalID;
			invPtr->m_raceID = m_raceFilterID;
		    
		    cPtr->m_storeInventory->InsertAfter(posInsert,invPtr);
			}
            UpdateData(FALSE);
			InListStoreInventory(inventoryItemSel+1);
}

void CCommerceDlg::OnBnClickedButtonBumpbox()
{
	      UpdateData(TRUE);
          m_popoutBoxSizeMaxX += m_bumpAmount;
          m_popoutBoxSizeMinX -= m_bumpAmount;
          m_popoutBoxSizeMaxY += m_bumpAmount;
          m_popoutBoxSizeMinY -= m_bumpAmount;
          m_popoutBoxSizeMaxZ += m_bumpAmount;
          m_popoutBoxSizeMinZ -= m_bumpAmount;
          UpdateData(FALSE);
}

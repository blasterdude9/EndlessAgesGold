// HousingSysDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "direct.h"
#include "HousingSysDlg.h"
#include "VisualManagerDlg.h"
#include "LightInterfaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHousingSysDlg dialog


CHousingSysDlg::CHousingSysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHousingSysDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHousingSysDlg)
	m_houseName = _T("");
	m_xPosition = 0.0f;
	m_yPosition = 0.0f;
	m_zPosition = 0.0f;
	//}}AFX_DATA_INIT
}


void CHousingSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHousingSysDlg)
	DDX_Control(pDX, IDC_LIST_HouseDatabase, m_houseDBList);
	DDX_Text(pDX, IDC_EDIT_HouseName, m_houseName);
	DDX_Text(pDX, IDC_EDIT_Xposition, m_xPosition);
	DDX_Text(pDX, IDC_EDIT_Yposition, m_yPosition);
	DDX_Text(pDX, IDC_EDIT_Zposition, m_zPosition);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHousingSysDlg, CDialog)
	//{{AFX_MSG_MAP(CHousingSysDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_LoadHousingSystem, OnBTNLoadHousingSystem)
	ON_BN_CLICKED(IDC_BTN_SaveHousingSystem, OnBTNSaveHousingSystem)
	ON_BN_CLICKED(IDC_BUTTON_addCloseStateCollision, OnBUTTONaddCloseStateCollision)
	ON_BN_CLICKED(IDC_BUTTON_AddExternalVisualsL1, OnBUTTONAddExternalVisualsL1)
	ON_BN_CLICKED(IDC_BUTTON_AddOpenStateCollision, OnBUTTONAddOpenStateCollision)
	ON_BN_CLICKED(IDC_BUTTON_CreateNew, OnBUTTONCreateNew)
	ON_BN_CLICKED(IDC_BUTTON_LoadInternalVisualSet, OnBUTTONLoadInternalVisualSet)
	ON_BN_CLICKED(IDC_BUTTON_LoadClipBox, OnBUTTONLoadClipBox)
	ON_BN_CLICKED(IDC_BUTTON_LoadInternalAreaBox, OnBUTTONLoadInternalAreaBox)
	ON_BN_CLICKED(IDC_BUTTON_AddExternalVisualsL2, OnBUTTONAddExternalVisualsL2)
	ON_BN_CLICKED(IDC_BUTTON_AddExternalVisualsL3, OnBUTTONAddExternalVisualsL3)
	ON_BN_CLICKED(IDC_BUTTON_LoadLodArea, OnBUTTONLoadLodArea)
	ON_BN_CLICKED(IDC_BUTTON_LoadLod2Area, OnBUTTONLoadLod2Area)
	ON_BN_CLICKED(IDC_BUTTON_LoadLod3Area, OnBUTTONLoadLod3Area)
	ON_BN_CLICKED(IDC_BUTTON_LoadDoorVisual, OnBUTTONLoadDoorVisual)
	ON_BN_CLICKED(IDC_BUTTON_AddLight, OnBUTTONAddLight)
	ON_BN_CLICKED(IDC_BUTTON_DeleteLight, OnBUTTONDeleteLight)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSettings, OnBUTTONReplaceSettings)
	ON_LBN_SELCHANGE(IDC_LIST_HouseDatabase, OnSelchangeLISTHouseDatabase)
	ON_BN_CLICKED(IDC_CHECK_spawnHouseOnClose, OnCHECKspawnHouseOnClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHousingSysDlg message handlers

BOOL CHousingSysDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListHouses(0);
	m_spwnHouseOnClose = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHousingSysDlg::OnClose()
{
	CDialog::OnClose();
}

void CHousingSysDlg::InListHouses(int sel)
{
	m_houseDBList.ResetContent();
	for(POSITION posLoc = m_houseDBREF->GetHeadPosition();posLoc != NULL;)
	{
        CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetNext(posLoc);
        m_houseDBList.AddString(hsPtr->m_houseName);
	}

	if(sel > -1)
	   m_houseDBList.SetCurSel(sel);
}

void CHousingSysDlg::OnBTNLoadHousingSystem() 
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

   static char BASED_CODE filter[] = "House Files|*.HSF|All Files (*.*)|*.*||";
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
	   m_houseDBREF->SafeDelete();
	   delete m_houseDBREF;
	   m_houseDBREF = 0;
	   the_in_Archive >> m_houseDBREF;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListHouses(0);
	 m_houseDBREF->ReinitAll(g_pd3dDeviceRef);
}

void CHousingSysDlg::OnBTNSaveHousingSystem() 
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
     static char BASED_CODE filter[] = "House Files|*.HSF|All Files (*.*)|*.*||";
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
       the_out_Archive << m_houseDBREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);
}

void CHousingSysDlg::OnBUTTONaddCloseStateCollision() 
{
    int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}


	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);
        
		if(hsPtr->m_closedState)
		{
            hsPtr->m_closedState->SafeDelete();
            delete hsPtr->m_closedState;
			hsPtr->m_closedState = 0;
		}

        //use gamefiles folder for organization
        CString CurDir;
        CString MainDir;
        char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
       //end use gamefiles folder for organization

       static char BASED_CODE filter[] = "Collision Unit Files|*.CUI|All Files (*.*)|*.*||";
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
	      the_in_Archive >> hsPtr->m_closedState;
	      the_in_Archive.Close();
          fl.Close();
	   }
       SetCurrentDirectory(MainDir);	
}

void CHousingSysDlg::OnBUTTONAddExternalVisualsL1() 
{
    int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}


	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);

    CVisualManagerDlg dlg;

    if(!hsPtr->m_externalVisuals)
		hsPtr->m_externalVisuals = new CEXMeshObjList();

	dlg.GL_textureDataBase = GL_textureDataBase;
	dlg.m_exMeshListREF = hsPtr->m_externalVisuals;
	dlg.g_pd3dDeviceRef = g_pd3dDeviceRef;
	dlg.DoModal();
	

	if(hsPtr->m_externalVisuals->GetCount()==0)
	{
		hsPtr->m_externalVisuals->SafeDelete();
		delete hsPtr->m_externalVisuals;
		hsPtr->m_externalVisuals = 0;
	}
}

void CHousingSysDlg::OnBUTTONAddOpenStateCollision() 
{
    int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}


	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);
        
		if(hsPtr->m_closedState)
		{
            hsPtr->m_openState->SafeDelete();
            delete hsPtr->m_openState;
			hsPtr->m_openState = 0;
		}

        //use gamefiles folder for organization
        CString CurDir;
        CString MainDir;
        char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
       //end use gamefiles folder for organization

       static char BASED_CODE filter[] = "Collision Unit Files|*.CUI|All Files (*.*)|*.*||";
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
	      the_in_Archive >> hsPtr->m_openState;
	      the_in_Archive.Close();
          fl.Close();
	   }
       SetCurrentDirectory(MainDir);	
}

void CHousingSysDlg::OnBUTTONCreateNew() 
{
	CHousingObj * hsPtr = new CHousingObj();

    UpdateData(TRUE);
	hsPtr->m_houseName = m_houseName;
    m_houseDBREF->AddTail(hsPtr);
	UpdateData(FALSE);
	InListHouses(m_houseDBREF->GetCount()-1);
}

void CHousingSysDlg::OnOK() 
{
	m_houseDBREF->ReinitAll(g_pd3dDeviceRef);
	
	CDialog::OnOK();
}

void CHousingSysDlg::OnCancel()
{
	EndDialog(IDCANCEL);
	CDialog::OnCancel();
}

void CHousingSysDlg::OnBUTTONLoadInternalVisualSet() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);

    CVisualManagerDlg dlg;

    if(!hsPtr->m_internalVisuals)
		hsPtr->m_internalVisuals = new CEXMeshObjList();

	dlg.GL_textureDataBase = GL_textureDataBase;
	dlg.m_exMeshListREF = hsPtr->m_internalVisuals;
	dlg.g_pd3dDeviceRef = g_pd3dDeviceRef;
	dlg.DoModal();
	

	if(hsPtr->m_internalVisuals->GetCount()==0)
	{
		hsPtr->m_internalVisuals->SafeDelete();
		delete hsPtr->m_internalVisuals;
		hsPtr->m_internalVisuals = 0;
	}
}

void CHousingSysDlg::OnBUTTONLoadClipBox() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);
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
          hsPtr->m_clipBox.maxX = m_mesh->List_boundingBox.maxX;
          hsPtr->m_clipBox.maxY = m_mesh->List_boundingBox.maxY;
          hsPtr->m_clipBox.maxZ = m_mesh->List_boundingBox.maxZ;
          hsPtr->m_clipBox.minX = m_mesh->List_boundingBox.minX;
          hsPtr->m_clipBox.minY = m_mesh->List_boundingBox.minY;
          hsPtr->m_clipBox.minZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

void CHousingSysDlg::OnBUTTONLoadInternalAreaBox() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);
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
          hsPtr->m_internalArea.maxX = m_mesh->List_boundingBox.maxX;
          hsPtr->m_internalArea.maxY = m_mesh->List_boundingBox.maxY;
          hsPtr->m_internalArea.maxZ = m_mesh->List_boundingBox.maxZ;
          hsPtr->m_internalArea.minX = m_mesh->List_boundingBox.minX;
          hsPtr->m_internalArea.minY = m_mesh->List_boundingBox.minY;
          hsPtr->m_internalArea.minZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

void CHousingSysDlg::OnBUTTONAddExternalVisualsL2() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);

    CVisualManagerDlg dlg;

    if(!hsPtr->m_lod2Set)
		hsPtr->m_lod2Set = new CEXMeshObjList();

	dlg.GL_textureDataBase = GL_textureDataBase;
	dlg.m_exMeshListREF = hsPtr->m_lod2Set;
	dlg.g_pd3dDeviceRef = g_pd3dDeviceRef;
	dlg.DoModal();
	

	if(hsPtr->m_lod2Set->GetCount()==0)
	{
		hsPtr->m_lod2Set->SafeDelete();
		delete hsPtr->m_lod2Set;
		hsPtr->m_lod2Set = 0;
	}
}

void CHousingSysDlg::OnBUTTONAddExternalVisualsL3() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);

    CVisualManagerDlg dlg;

    if(!hsPtr->m_lod3Set)
		hsPtr->m_lod3Set = new CEXMeshObjList();

	dlg.GL_textureDataBase = GL_textureDataBase;
	dlg.m_exMeshListREF = hsPtr->m_lod3Set;
	dlg.g_pd3dDeviceRef = g_pd3dDeviceRef;
	dlg.DoModal();
	

	if(hsPtr->m_lod3Set->GetCount()==0)
	{
		hsPtr->m_lod3Set->SafeDelete();
		delete hsPtr->m_lod3Set;
		hsPtr->m_lod3Set = 0;
	}
}

void CHousingSysDlg::OnBUTTONLoadLodArea() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);
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
          hsPtr->m_extrnalRangeLod1.maxX = m_mesh->List_boundingBox.maxX;
          hsPtr->m_extrnalRangeLod1.maxY = m_mesh->List_boundingBox.maxY;
          hsPtr->m_extrnalRangeLod1.maxZ = m_mesh->List_boundingBox.maxZ;
          hsPtr->m_extrnalRangeLod1.minX = m_mesh->List_boundingBox.minX;
          hsPtr->m_extrnalRangeLod1.minY = m_mesh->List_boundingBox.minY;
          hsPtr->m_extrnalRangeLod1.minZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

void CHousingSysDlg::OnBUTTONLoadLod2Area() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);
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
          hsPtr->m_extrnalRangeLod2.maxX = m_mesh->List_boundingBox.maxX;
          hsPtr->m_extrnalRangeLod2.maxY = m_mesh->List_boundingBox.maxY;
          hsPtr->m_extrnalRangeLod2.maxZ = m_mesh->List_boundingBox.maxZ;
          hsPtr->m_extrnalRangeLod2.minX = m_mesh->List_boundingBox.minX;
          hsPtr->m_extrnalRangeLod2.minY = m_mesh->List_boundingBox.minY;
          hsPtr->m_extrnalRangeLod2.minZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

void CHousingSysDlg::OnBUTTONLoadLod3Area() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);
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
          hsPtr->m_extrnalRangeLod3.maxX = m_mesh->List_boundingBox.maxX;
          hsPtr->m_extrnalRangeLod3.maxY = m_mesh->List_boundingBox.maxY;
          hsPtr->m_extrnalRangeLod3.maxZ = m_mesh->List_boundingBox.maxZ;
          hsPtr->m_extrnalRangeLod3.minX = m_mesh->List_boundingBox.minX;
          hsPtr->m_extrnalRangeLod3.minY = m_mesh->List_boundingBox.minY;
          hsPtr->m_extrnalRangeLod3.minZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

void CHousingSysDlg::OnBUTTONLoadDoorVisual() 
{
    int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);

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
		
      if(hsPtr->m_doorVisual)
	  {
        hsPtr->m_doorVisual->SafeDelete();
		delete hsPtr->m_doorVisual;
		hsPtr->m_doorVisual = 0;
	  }


	  hsPtr->m_doorVisual = new CEXMeshObj();
	  hsPtr->m_doorVisual->List_materialObject = new CMaterialObject();
	
      float sightRange = 0.0f;
      hsPtr->m_doorVisual->LoadGeometryFromPlugin(fileName,&sightRange,g_pd3dDeviceRef,NULL);//proprietory file format
	}
	SetCurrentDirectory(MainDir);
}

void CHousingSysDlg::OnBUTTONAddLight() 
{
    int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);

	CLightInterfaceDlg dlg;
	   dlg.GL_textureDataBase = GL_textureDataBase;
	   dlg.m_lightREF = hsPtr->m_lightObj;
	   if(dlg.DoModal()==IDOK)
		  hsPtr->m_lightObj = dlg.m_lightREF;
}

void CHousingSysDlg::OnBUTTONDeleteLight() 
{
	int select = m_houseDBList.GetCurSel();
	if(select == -1)
	{
		AfxMessageBox("Must select a house");
		return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	
	CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);
	if(hsPtr->m_lightObj)
	{
		if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
			return;
		
		delete hsPtr->m_lightObj;
		hsPtr->m_lightObj = 0;
	}
}

void CHousingSysDlg::OnBUTTONReplaceSettings() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);

        UpdateData(TRUE);
        
		hsPtr->m_houseName = m_houseName;
        hsPtr->m_lightPosition.x = m_xPosition;
        hsPtr->m_lightPosition.y = m_yPosition;
	    hsPtr->m_lightPosition.z = m_zPosition;

        UpdateData(FALSE);
}

void CHousingSysDlg::OnSelchangeLISTHouseDatabase() 
{
	int select = m_houseDBList.GetCurSel();
    if(select == -1)
	{
	   AfxMessageBox("Must select a house");
	   return;
	}

	POSITION posLoc = m_houseDBREF->FindIndex(select);
	if(!posLoc)
		return;
	    CHousingObj * hsPtr = (CHousingObj *)m_houseDBREF->GetAt(posLoc);

        UpdateData(TRUE);
        
		m_houseName = hsPtr->m_houseName;
        m_xPosition = hsPtr->m_lightPosition.x;
        m_yPosition = hsPtr->m_lightPosition.y;
	    m_zPosition = hsPtr->m_lightPosition.z;

        UpdateData(FALSE);
}

void CHousingSysDlg::OnCHECKspawnHouseOnClose() 
{
	if(m_spwnHouseOnClose)
	   m_spwnHouseOnClose = FALSE;
	else
	   m_spwnHouseOnClose = TRUE;
	

	m_houseSelection = m_houseDBList.GetCurSel();
	if(m_houseSelection < 0)
	   m_houseSelection = 0;
}

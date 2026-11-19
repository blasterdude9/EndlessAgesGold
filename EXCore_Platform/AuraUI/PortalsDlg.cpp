// PortalsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PortalsDlg.h"
#include "direct.h"
#include "CEXMeshClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortalsDlg dialog


CPortalsDlg::CPortalsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPortalsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPortalsDlg)
	m_portalName = _T("");
	m_requiredKeyID = 0;
	m_worldID = 0;
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_zPos = 0.0f;
	m_zoneMaxX = 0.0f;
	m_zoneMaxY = 0.0f;
	m_zoneMaxZ = 0.0f;
	m_zoneMinX = 0.0f;
	m_zoneMinY = 0.0f;
	m_zoneMinZ = 0.0f;
	m_zoneWldID = 0;
	//}}AFX_DATA_INIT
}


void CPortalsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPortalsDlg)
	DDX_Control(pDX, IDC_LIST_PortalsDB, m_portalsDB);
	DDX_Text(pDX, IDC_EDIT_portalName, m_portalName);
	DDX_Text(pDX, IDC_EDIT_RequiredKeyID, m_requiredKeyID);
	DDX_Text(pDX, IDC_EDIT_WorldID, m_worldID);
	DDX_Text(pDX, IDC_EDIT_Xpos, m_xPos);
	DDX_Text(pDX, IDC_EDIT_Ypos, m_yPos);
	DDX_Text(pDX, IDC_EDIT_Zpos, m_zPos);
	DDX_Text(pDX, IDC_EDIT_zoneMaxX, m_zoneMaxX);
	DDX_Text(pDX, IDC_EDIT_zoneMaxY, m_zoneMaxY);
	DDX_Text(pDX, IDC_EDIT_zoneMaxZ, m_zoneMaxZ);
	DDX_Text(pDX, IDC_EDIT_zoneMinX, m_zoneMinX);
	DDX_Text(pDX, IDC_EDIT_zoneMinY, m_zoneMinY);
	DDX_Text(pDX, IDC_EDIT_zoneMinZ, m_zoneMinZ);
	DDX_Text(pDX, IDC_EDIT_zoneWldID, m_zoneWldID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPortalsDlg, CDialog)
	//{{AFX_MSG_MAP(CPortalsDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBUTTONReplace)
	ON_LBN_SELCHANGE(IDC_LIST_PortalsDB, OnSelchangeLISTPortalsDB)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LoadBoxFromARW, OnBUTTONLoadBoxFromARW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPortalsDlg message handlers

void CPortalsDlg::OnButtonAdd() 
{
	UpdateData(TRUE);
	CPortalSysObj * portalPtr = new CPortalSysObj();
	portalPtr->m_keyRequired = m_requiredKeyID;
	portalPtr->m_portalName = m_portalName;
	portalPtr->m_targetWorld = m_worldID;
    portalPtr->m_wldPosition.x = m_xPos;
    portalPtr->m_wldPosition.y = m_yPos;
	portalPtr->m_wldPosition.z = m_zPos;
	portalPtr->m_portalZone.maxX = m_zoneMaxX;
    portalPtr->m_portalZone.maxY = m_zoneMaxY;
	portalPtr->m_portalZone.maxZ = m_zoneMaxZ;
    portalPtr->m_portalZone.minX = m_zoneMinX;
    portalPtr->m_portalZone.minY = m_zoneMinY;
    portalPtr->m_portalZone.minZ = m_zoneMinZ;
	portalPtr->m_zoneWldID = m_zoneWldID;
	m_portalsDBRef->AddTail(portalPtr);
	UpdateData(FALSE);
	InList(m_portalsDBRef->GetCount());
}

void CPortalsDlg::OnButtonDelete() 
{
	int select = m_portalsDB.GetCurSel();
	if(select == -1)
		return;

	POSITION posLoc = m_portalsDBRef->FindIndex(select);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CPortalSysObj * portalPtr = (CPortalSysObj *)m_portalsDBRef->GetAt(posLoc);
	delete portalPtr;
	portalPtr = 0;

	m_portalsDBRef->RemoveAt(posLoc);
	InList(select-1);
}

void CPortalsDlg::OnBUTTONReplace() 
{
	int select = m_portalsDB.GetCurSel();
    if(select == -1)
	{
	   MessageBox("Must select a portal");
	   return;
	}

	POSITION posLoc = m_portalsDBRef->FindIndex(select);
	if(!posLoc)
		return;

        CPortalSysObj * portalPtr = (CPortalSysObj *)m_portalsDBRef->GetAt(posLoc);

		UpdateData(TRUE);
	    portalPtr->m_keyRequired = m_requiredKeyID;
	    portalPtr->m_portalName = m_portalName;
	    portalPtr->m_targetWorld = m_worldID;
        portalPtr->m_wldPosition.x = m_xPos;
        portalPtr->m_wldPosition.y = m_yPos;
   	    portalPtr->m_wldPosition.z = m_zPos;
		portalPtr->m_portalZone.maxX = m_zoneMaxX;
        portalPtr->m_portalZone.maxY = m_zoneMaxY;
	    portalPtr->m_portalZone.maxZ = m_zoneMaxZ;
        portalPtr->m_portalZone.minX = m_zoneMinX;
        portalPtr->m_portalZone.minY = m_zoneMinY;
        portalPtr->m_portalZone.minZ = m_zoneMinZ;
		portalPtr->m_zoneWldID = m_zoneWldID;
	    UpdateData(FALSE);
}

BOOL CPortalsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InList(0);
	


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPortalsDlg::InList(int sel)
{
   m_portalsDB.ResetContent();
   for(POSITION posLoc = m_portalsDBRef->GetHeadPosition();posLoc !=NULL;)
   {
       CPortalSysObj * portalPtr = (CPortalSysObj *)m_portalsDBRef->GetNext(posLoc);
	   m_portalsDB.AddString(portalPtr->m_portalName);
   }
   if(sel > -1)
	 m_portalsDB.SetCurSel(sel);
}

void CPortalsDlg::OnSelchangeLISTPortalsDB() 
{
	int select = m_portalsDB.GetCurSel();
    if(select == -1)
	   return;

	POSITION posLoc = m_portalsDBRef->FindIndex(select);
	if(!posLoc)
		return;

        CPortalSysObj * portalPtr = (CPortalSysObj *)m_portalsDBRef->GetAt(posLoc);

		UpdateData(TRUE);
	    m_requiredKeyID = portalPtr->m_keyRequired;
	    m_portalName = portalPtr->m_portalName;
	    m_worldID = portalPtr->m_targetWorld;
        m_xPos = portalPtr->m_wldPosition.x;
        m_yPos = portalPtr->m_wldPosition.y;
   	    m_zPos = portalPtr->m_wldPosition.z;
		m_zoneMaxX = portalPtr->m_portalZone.maxX;
        m_zoneMaxY = portalPtr->m_portalZone.maxY;
	    m_zoneMaxZ = portalPtr->m_portalZone.maxZ;
        m_zoneMinX = portalPtr->m_portalZone.minX;
        m_zoneMinY = portalPtr->m_portalZone.minY;
        m_zoneMinZ = portalPtr->m_portalZone.minZ;
		m_zoneWldID = portalPtr->m_zoneWldID;
	    UpdateData(FALSE);
}

void CPortalsDlg::OnButtonLoad() 
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

   static char BASED_CODE filter[] = "Portal Zone Files|*.PZN|All Files (*.*)|*.*||";
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
	   m_portalsDBRef->SafeDelete();
	   delete m_portalsDBRef;
	   m_portalsDBRef = 0;
	   the_in_Archive >> m_portalsDBRef;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InList(0);
}

void CPortalsDlg::OnButtonSave() 
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
     static char BASED_CODE filter[] = "Portal Zone Files|*.PZN|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "PZN", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_portalsDBRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CPortalsDlg::OnBUTTONLoadBoxFromARW() 
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
	   float dist = 0.0f;
       if(m_mesh->LoadGeometryFromPlugin(fileName,&dist,g_pd3dDeviceRef,NULL))
	   {//1
          UpdateData(TRUE);
          m_zoneMaxX = m_mesh->List_boundingBox.maxX;
          m_zoneMinX = m_mesh->List_boundingBox.minX;
          m_zoneMaxY = m_mesh->List_boundingBox.maxY;
          m_zoneMinY = m_mesh->List_boundingBox.minY;
          m_zoneMaxZ = m_mesh->List_boundingBox.maxZ;
          m_zoneMinZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

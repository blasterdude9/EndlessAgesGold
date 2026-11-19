// BankZonesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "BankZonesDlg.h"
#include "direct.h"
#include "CEXMeshClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBankZonesDlg dialog


CBankZonesDlg::CBankZonesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBankZonesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBankZonesDlg)
	m_menuBind = 0;
	m_popoutBoxMaxX = 0.0f;
	m_popoutBoxMaxY = 0.0f;
	m_popoutBoxMaxZ = 0.0f;
	m_popoutBoxMinX = 0.0f;
	m_popoutBoxMinY = 0.0f;
	m_popoutBoxMinZ = 0.0f;
	m_worldID = 0;
	m_bankName = _T("");
	//}}AFX_DATA_INIT
}


void CBankZonesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBankZonesDlg)
	DDX_Control(pDX, IDC_CHECK_willDealWithMurderers, m_willDealWithMurderers);
	DDX_Control(pDX, IDC_LIST_BankDB, m_bankDBList);
	DDX_Text(pDX, IDC_EDIT_MenuBind, m_menuBind);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxX, m_popoutBoxMaxX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxY, m_popoutBoxMaxY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxZ, m_popoutBoxMaxZ);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinX, m_popoutBoxMinX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinY, m_popoutBoxMinY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinZ, m_popoutBoxMinZ);
	DDX_Text(pDX, IDC_EDIT_WorldID, m_worldID);
	DDX_Text(pDX, IDC_EDIT_BankName, m_bankName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBankZonesDlg, CDialog)
	//{{AFX_MSG_MAP(CBankZonesDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddBank, OnBUTTONAddBank)
	ON_BN_CLICKED(IDC_BUTTON_DeleteBank, OnBUTTONDeleteBank)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceBank, OnBUTTONReplaceBank)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_LBN_SELCHANGE(IDC_LIST_BankDB, OnSelchangeLISTBankDB)
	ON_BN_CLICKED(IDC_CHECK_willDealWithMurderers, OnCHECKwillDealWithMurderers)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONLoadBox, OnBnClickedButtonloadbox)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBankZonesDlg message handlers

void CBankZonesDlg::OnBUTTONAddBank() 
{
	CBankObj * bankObj = new CBankObj();
	UpdateData(TRUE);
    bankObj->m_menuBind            = m_menuBind;
	bankObj->m_boundingBoxMax.x    = m_popoutBoxMaxX;
	bankObj->m_boundingBoxMax.y    = m_popoutBoxMaxY;
	bankObj->m_boundingBoxMax.z    = m_popoutBoxMaxZ;
	bankObj->m_boundingBoxMin.x    = m_popoutBoxMinX;
	bankObj->m_boundingBoxMin.y    = m_popoutBoxMinY;
	bankObj->m_boundingBoxMin.z    = m_popoutBoxMinZ;
	bankObj->m_worldID             = m_worldID;
	bankObj->m_bankName            = m_bankName;
	m_bankDBRef->AddTail(bankObj);
	UpdateData(FALSE);
	InListBankZones(m_bankDBRef->GetCount()-1);

}

void CBankZonesDlg::OnBUTTONDeleteBank() 
{
	int bankSelection = m_bankDBList.GetCurSel();
	if(bankSelection == -1)
	{
		AfxMessageBox("Select a bank");
		return;
	}

	POSITION posLoc = m_bankDBRef->FindIndex(bankSelection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CBankObj * bankObj = (CBankObj *)m_bankDBRef->GetAt(posLoc);
	delete bankObj;
	bankObj = 0;
	m_bankDBRef->RemoveAt(posLoc);
	InListBankZones(bankSelection-1);	
}

void CBankZonesDlg::OnBUTTONReplaceBank() 
{
	int bankSelection = m_bankDBList.GetCurSel();
	if(bankSelection == -1)
	{
       AfxMessageBox("Select a bank");
	   return;
	}

	POSITION posLoc = m_bankDBRef->FindIndex(bankSelection);
	if(!posLoc)
		return;
	CBankObj * bankObj = (CBankObj *)m_bankDBRef->GetAt(posLoc);
	UpdateData(TRUE);

	bankObj->m_menuBind            = m_menuBind;
	bankObj->m_boundingBoxMax.x    = m_popoutBoxMaxX;
	bankObj->m_boundingBoxMax.y    = m_popoutBoxMaxY;
	bankObj->m_boundingBoxMax.z    = m_popoutBoxMaxZ;
	bankObj->m_boundingBoxMin.x    = m_popoutBoxMinX;
	bankObj->m_boundingBoxMin.y    = m_popoutBoxMinY;
	bankObj->m_boundingBoxMin.z    = m_popoutBoxMinZ;
	bankObj->m_worldID             = m_worldID;
	bankObj->m_bankName            = m_bankName;

	UpdateData(FALSE);
	InListBankZones(bankSelection);	
}

BOOL CBankZonesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InListBankZones(0);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBankZonesDlg::InListBankZones(int sel)
{
	UpdateData(TRUE);
    m_bankDBList.ResetContent();
	for(POSITION posLoc = m_bankDBRef->GetHeadPosition();posLoc != NULL;)
	{
        CBankObj * bankObj = (CBankObj *) m_bankDBRef->GetNext(posLoc);
        m_bankDBList.AddString(bankObj->m_bankName);
	}
    UpdateData(FALSE);
	if(sel > -1)
	   m_bankDBList.SetCurSel(sel);
}



void CBankZonesDlg::OnButtonSave() 
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
     static char BASED_CODE filter[] = "Bank Zone Files|*.BZN|All Files (*.*)|*.*||";
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
       the_out_Archive << m_bankDBRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CBankZonesDlg::OnButtonLoad() 
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

   static char BASED_CODE filter[] = "Bank Zone Files|*.BZN|All Files (*.*)|*.*||";
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
	   m_bankDBRef->SafeDelete();
	   delete m_bankDBRef;
	   m_bankDBRef = 0;
	   the_in_Archive >> m_bankDBRef;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListBankZones(0);
}

void CBankZonesDlg::OnSelchangeLISTBankDB() 
{
	int bankSelection = m_bankDBList.GetCurSel();
	if(bankSelection == -1)
	{
	   return;
	}

	POSITION posLoc = m_bankDBRef->FindIndex(bankSelection);
	if(!posLoc)
		return;
	CBankObj * bankObj = (CBankObj *)m_bankDBRef->GetAt(posLoc);
	UpdateData(TRUE);

	m_menuBind      = bankObj->m_menuBind;
	m_popoutBoxMaxX = bankObj->m_boundingBoxMax.x;
	m_popoutBoxMaxY = bankObj->m_boundingBoxMax.y;
	m_popoutBoxMaxZ = bankObj->m_boundingBoxMax.z;
	m_popoutBoxMinX = bankObj->m_boundingBoxMin.x;
	m_popoutBoxMinY = bankObj->m_boundingBoxMin.y;
	m_popoutBoxMinZ = bankObj->m_boundingBoxMin.z;
	m_worldID       = bankObj->m_worldID;
	m_bankName      = bankObj->m_bankName;

	 if(bankObj->m_willDealWithCriminals)
	   m_willDealWithMurderers.SetCheck(1);
	 else
       m_willDealWithMurderers.SetCheck(0);

	UpdateData(FALSE);
}

void CBankZonesDlg::OnCHECKwillDealWithMurderers() 
{
	int bankSelection = m_bankDBList.GetCurSel();
	if(bankSelection == -1)
	{
	   return;
	}

	POSITION posLoc = m_bankDBRef->FindIndex(bankSelection);
	if(!posLoc)
		return;
	CBankObj * bankObj = (CBankObj *)m_bankDBRef->GetAt(posLoc);
	UpdateData(TRUE);

	if(bankObj->m_willDealWithCriminals)
	   bankObj->m_willDealWithCriminals = FALSE;
	else
	   bankObj->m_willDealWithCriminals = TRUE;

	UpdateData(FALSE);
}

void CBankZonesDlg::OnBnClickedButtonloadbox()
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
          m_popoutBoxMaxX = m_mesh->List_boundingBox.maxX;
          m_popoutBoxMinX = m_mesh->List_boundingBox.minX;
          m_popoutBoxMaxY = m_mesh->List_boundingBox.maxY;
          m_popoutBoxMinY = m_mesh->List_boundingBox.minY;
          m_popoutBoxMaxZ = m_mesh->List_boundingBox.maxZ;
          m_popoutBoxMinZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
	 SetCurrentDirectory(MainDir);
}

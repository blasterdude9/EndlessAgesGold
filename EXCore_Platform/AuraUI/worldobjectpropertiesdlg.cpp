// WorldObjectPropertiesDlg.cpp : implementation file
//

#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "resource.h"
#include "WorldObjectPropertiesDlg.h"
#include "CharonaDlg.h"
#include "direct.h"
#include "CEXMeshClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorldObjectPropertiesDlg dialog


CWorldObjectPropertiesDlg::CWorldObjectPropertiesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorldObjectPropertiesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWorldObjectPropertiesDlg)
	//m_meshRadius = 0.0f;
	m_objectGroup = 0;
	m_otherAttributeType = 0;
	m_otherAttributeValue = 0;
	m_otherObjectSelect = 0;
	//m_popoutRadius = 0.0f;
	m_selfAttributeType = 0;
	m_selfAttributeValue = 0;
	//m_spawnPoint = 0;
	m_popoutBoxSizeMaxX = 0.0f;
	m_popoutBoxSizeMaxY = 0.0f;
	m_popoutBoxSizeMaxZ = 0.0f;
	m_popoutBoxSizeMinX = 0.0f;
	m_popoutBoxSizeMinY = 0.0f;
	m_popoutBoxSizeMinZ = 0.0f;
	m_actualBoxSizeMinX = 0.0f;
	m_actualBoxSizeMinY = 0.0f;
	m_actualBoxSizeMinZ = 0.0f;
	m_actualBoxSizeMaxX = 0.0f;
	m_actualBoxSizeMaxY = 0.0f;
	m_actualBoxSizeMaxZ = 0.0f;
	m_multiplier = 0.0f;
	m_haloLink = 0;
	m_lensflareLink = 0;
	m_particlelink = 0;
	m_particleVectorX = 0.0f;
	m_particleVectorY = 0.0f;
	m_particleVectorZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CWorldObjectPropertiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorldObjectPropertiesDlg)
	DDX_Control(pDX, IDC_COMBO_GroupFilterMod, m_groupFilterMod);
	DDX_Control(pDX, IDC_CHECK_RenderFilter, m_renderFilter);
	DDX_Control(pDX, IDC_CHECK_CollisionFilter, m_collisionFilter);
	//DDX_Control(pDX, IDC_CHECK_RenderInMirrors, m_renderInMirrors);
	//DDX_Control(pDX, IDC_EDIT_cntDispersalAmount, m_cntDispersal);
	//DDX_Control(pDX, IDC_EDIT_cntCurrentAmount, m_cntCurrentAmount);
	//DDX_Control(pDX, IDC_EDIT_cntMaxAmount, m_cntMaxAmount);
	//DDX_Control(pDX, IDC_EDIT_RespawnTime, m_cntRespawnTime);
	//DDX_Control(pDX, IDC_EDIT_ContMiscType, m_cntMiscType);
	//DDX_Control(pDX, IDC_COMBO_Containbasis, m_containmentbasis);
	DDX_Control(pDX, IDC_CHECK_ShowBoundingBox, m_showBoundingBox);
	DDX_Control(pDX, IDC_CHECK_ATTACHTOSCENE, m_attachedToScene);
	DDX_Control(pDX, IDC_CHECK_VICINITYBASED, m_vicinityBased);
	DDX_Control(pDX, IDC_CHECK_RemoveEnvironment, m_removeEnviorment);
	DDX_Control(pDX, IDC_CHECK_AUTORETRACT, m_autoRetract);
	//DDX_Text(pDX, IDC_EDIT_MeshORadius, m_meshRadius);
	DDX_Text(pDX, IDC_EDIT_ObjectGroup, m_objectGroup);
	DDX_Text(pDX, IDC_EDIT_OtherAttributeType, m_otherAttributeType);
	DDX_Text(pDX, IDC_EDIT_OtherAttributeValue, m_otherAttributeValue);
	DDX_Text(pDX, IDC_EDIT_OtherObjectSelect, m_otherObjectSelect);
	//DDX_Text(pDX, IDC_EDIT_PopoutORadius, m_popoutRadius);
	DDX_Text(pDX, IDC_EDIT_SelfAttributeType, m_selfAttributeType);
	DDX_Text(pDX, IDC_EDIT_SelfAttributeValue, m_selfAttributeValue);
	//DDX_Text(pDX, IDC_EDIT_SpawnPoint, m_spawnPoint);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxX, m_popoutBoxSizeMaxX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxY, m_popoutBoxSizeMaxY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxZ, m_popoutBoxSizeMaxZ);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinX, m_popoutBoxSizeMinX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinY, m_popoutBoxSizeMinY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinZ, m_popoutBoxSizeMinZ);
	DDX_Text(pDX, IDC_EDIT_ActualBoxSizeMinX, m_actualBoxSizeMinX);
	DDX_Text(pDX, IDC_EDIT_ActualBoxSizeMinY, m_actualBoxSizeMinY);
	DDX_Text(pDX, IDC_EDIT_ActualBoxSizeMinZ, m_actualBoxSizeMinZ);
	DDX_Text(pDX, IDC_EDIT_ActualBoxSizeMaxX, m_actualBoxSizeMaxX);
	DDX_Text(pDX, IDC_EDIT_ActualBoxSizeMaxY, m_actualBoxSizeMaxY);
	DDX_Text(pDX, IDC_EDIT_ActualBoxSizeMaxZ, m_actualBoxSizeMaxZ);
	DDX_Text(pDX, IDC_EDIT_Multiplier, m_multiplier);
	DDX_Text(pDX, IDC_EDIT_HaloLink, m_haloLink);
	DDX_Text(pDX, IDC_EDIT_LensflareLink, m_lensflareLink);
	DDX_Text(pDX, IDC_EDIT_ParticleLink, m_particlelink);
	DDX_Text(pDX, IDC_EDIT_particleDirVectX, m_particleVectorX);
	DDX_Text(pDX, IDC_EDIT_particleDirVectY, m_particleVectorY);
	DDX_Text(pDX, IDC_EDIT_particleDirVectZ, m_particleVectorZ);
	//}}AFX_DATA_MAP

    DDX_Control(pDX, IDC_COMBO_EnvAttrib, m_envAttrib);
    DDX_Text(pDX, IDC_EDIT_enviormantObjType, attachedToScene);
    DDX_Control(pDX, IDC_COMBO_surfacePropList, m_surfacePropertiesList);

	
	
    
}


BEGIN_MESSAGE_MAP(CWorldObjectPropertiesDlg, CDialog)
	//{{AFX_MSG_MAP(CWorldObjectPropertiesDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GroupSpinner, OnDeltaposSPINGroupSpinner)
	ON_BN_CLICKED(IDC_CHECK_ATTACHTOSCENE, OnCheckAttachtoscene)
	ON_BN_CLICKED(IDC_CHECK_AUTORETRACT, OnCheckAutoretract)
	ON_BN_CLICKED(IDC_CHECK_RemoveEnvironment, OnCHECKRemoveEnvironment)
	ON_BN_CLICKED(IDC_CHECK_VICINITYBASED, OnCheckVicinitybased)
	ON_BN_CLICKED(IDC_BUTTON_ActualReference, OnBUTTONActualReference)
	ON_BN_CLICKED(IDC_CHECK_ShowBoundingBox, OnCHECKShowBoundingBox)
	//ON_BN_CLICKED(IDC_BUTTON_disableContainment, OnBUTTONdisableContainment)
	//ON_BN_CLICKED(IDC_BUTTON_EnableContainment, OnBUTTONEnableContainment)
	//ON_EN_CHANGE(IDC_EDIT_ContMiscType, OnChangeEDITContMiscType)
	//ON_EN_CHANGE(IDC_EDIT_cntCurrentAmount, OnChangeEDITcntCurrentAmount)
	//ON_EN_CHANGE(IDC_EDIT_cntDispersalAmount, OnChangeEDITcntDispersalAmount)
	//ON_EN_CHANGE(IDC_EDIT_cntMaxAmount, OnChangeEDITcntMaxAmount)
	//ON_EN_CHANGE(IDC_EDIT_RespawnTime, OnChangeEDITRespawnTime)
	//ON_CBN_SELCHANGE(IDC_COMBO_Containbasis, OnSelchangeCOMBOContainbasis)
	//ON_BN_CLICKED(IDC_CHECK_RenderInMirrors, OnCHECKRenderInMirrors)
	ON_BN_CLICKED(IDC_CHECK_CollisionFilter, OnCHECKCollisionFilter)
	ON_BN_CLICKED(IDC_CHECK_RenderFilter, OnCHECKRenderFilter)
	ON_EN_CHANGE(IDC_EDIT_ParticleLink, OnChangeEDITParticleLink)
	ON_BN_CLICKED(IDC_BUTTON_CharonaSystem, OnBUTTONCharonaSystem)
	ON_BN_CLICKED(IDC_BUTTON_LoadBox, OnBUTTONLoadBox)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorldObjectPropertiesDlg message handlers

void CWorldObjectPropertiesDlg::OnDeltaposSPINGroupSpinner(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    UpdateData(TRUE);
    m_objectGroup -= pNMUpDown->iDelta;
    UpdateData(FALSE);
	//m_objectGroup
	*pResult = 0;
}

void CWorldObjectPropertiesDlg::OnCheckAttachtoscene() 
{
	if(attachedToScene==TRUE){
        m_attachedToScene.SetCheck(0);
		attachedToScene = FALSE;
	}
	else{
        m_attachedToScene.SetCheck(1);
		attachedToScene = TRUE;
	}
}

void CWorldObjectPropertiesDlg::OnCheckAutoretract() 
{
	if(autoRetract==TRUE){
        m_autoRetract.SetCheck(0);
		autoRetract = FALSE;
	}
	else{
        m_autoRetract.SetCheck(1);
		autoRetract = TRUE;
	}
}

void CWorldObjectPropertiesDlg::OnCHECKRemoveEnvironment() 
{
	if(removeEnvioronment==TRUE){
        m_removeEnviorment.SetCheck(0);
		removeEnvioronment = FALSE;
	}
	else{
        m_removeEnviorment.SetCheck(1);
		removeEnvioronment = TRUE;
	}
}

void CWorldObjectPropertiesDlg::OnCheckVicinitybased() 
{
	if(vicinityBased==TRUE){
        m_vicinityBased.SetCheck(0);
		vicinityBased = FALSE;
	}
	else{
        m_vicinityBased.SetCheck(1);
		vicinityBased = TRUE;
	}
}

BOOL CWorldObjectPropertiesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	 if(attachedToScene==TRUE)
        m_attachedToScene.SetCheck(1);
	 if(vicinityBased==TRUE)
        m_vicinityBased.SetCheck(1);
	 if(autoRetract==TRUE)
        m_autoRetract.SetCheck(1);
	 if(removeEnvioronment==TRUE)
        m_removeEnviorment.SetCheck(1);

	 m_multiplier = 1;
	 if(showBoundingBox == TRUE)
		m_showBoundingBox.SetCheck(1);

  //   if(m_wldObjectRef->m_renderInMirrorTag == TRUE)
		//m_renderInMirrors.SetCheck(1);

	 if(m_wldObjectRef->m_collisionInfoFilter == TRUE)
		 m_collisionFilter.SetCheck(1);

	 if(m_wldObjectRef->m_renderFilter == TRUE)
		 m_renderFilter.SetCheck(1);

	 m_particlelink = m_wldObjectRef->m_particleLink;

	 //if(m_wldObjectRef->m_containmentObject){
  //      m_containmentbasis.EnableWindow(TRUE);
	 //   m_cntMiscType.EnableWindow(TRUE);
  //      m_cntCurrentAmount.EnableWindow(TRUE);
  //      m_cntMaxAmount.EnableWindow(TRUE);
  //      m_cntRespawnTime.EnableWindow(TRUE);
	 //   m_cntDispersal.EnableWindow(TRUE);
  //      SetDlgItemInt(IDC_EDIT_cntDispersalAmount,m_wldObjectRef->m_containmentObject->m_dispersalAmount,TRUE);
	 //   SetDlgItemInt(IDC_EDIT_cntCurrentAmount,m_wldObjectRef->m_containmentObject->m_currentAmount,TRUE);
	 //   SetDlgItemInt(IDC_EDIT_cntMaxAmount,m_wldObjectRef->m_containmentObject->m_maxAmount,TRUE);
	 //   SetDlgItemInt(IDC_EDIT_RespawnTime,m_wldObjectRef->m_containmentObject->m_respawnTime,TRUE);
	 //   SetDlgItemInt(IDC_EDIT_ContMiscType,m_wldObjectRef->m_containmentObject->m_miscType,TRUE);
  //      m_containmentbasis.SetCurSel(m_wldObjectRef->m_containmentObject->m_containmentBasis);
	 //}
	 //else{
  //      m_containmentbasis.EnableWindow(FALSE);
	 //   m_cntMiscType.EnableWindow(FALSE);
  //      m_cntCurrentAmount.EnableWindow(FALSE);
  //      m_cntMaxAmount.EnableWindow(FALSE);
  //      m_cntRespawnTime.EnableWindow(FALSE);
	 //   m_cntDispersal.EnableWindow(FALSE);
	 //}

     //groups
	 m_groupFilterMod.AddString("NONE");
	 int trace = 0;
	 int selected = -1;
	 for(POSITION posLoc = m_worldGroupListRef->GetHeadPosition();posLoc != NULL;)
	 {
         CWldGroupObj * gPtr = (CWldGroupObj *)m_worldGroupListRef->GetNext(posLoc);
         m_groupFilterMod.AddString(gPtr->List_groupName);
         if(m_wldObjectRef->m_filterGroup == gPtr->List_groupInt)
            selected = trace;

         trace++;
	 }
	 m_groupFilterMod.SetCurSel((selected+1));
     //
	 m_particleVectorX = m_wldObjectRef->m_particleDirection.x;
 	 m_particleVectorY = m_wldObjectRef->m_particleDirection.y;
 	 m_particleVectorZ = m_wldObjectRef->m_particleDirection.z;

	 m_envAttrib.AddString("Disabled");
     m_envAttrib.AddString("Lock Emmisive/Modulation");
	 m_envAttrib.AddString("Lock Emmisive");
	 m_envAttrib.AddString("3rd pass Modulate X2 on/off");

     m_envAttrib.SetCurSel(m_wldObjectRef->m_envReactionAttrib);



     m_surfacePropertiesList.AddString("Disabled");
     m_surfacePropertiesList.AddString("Ladder");

	 m_surfacePropertiesList.SetCurSel(m_wldObjectRef->m_surfaceProperty);

	 if(m_wldObjectRef->m_meshObject)
	 {
	    m_selfAttributeType = m_wldObjectRef->m_meshObject->m_abVertexArray.m_uvCount;
	    m_selfAttributeValue = m_wldObjectRef->m_meshObject->m_abVertexArray.m_indecieCount;
		m_otherAttributeType = m_wldObjectRef->m_meshObject->m_abVertexArray.m_vertexCount;
	 }

     UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWorldObjectPropertiesDlg::OnBUTTONActualReference() 
{
	UpdateData(TRUE);
	m_popoutBoxSizeMaxX = m_actualBoxSizeMaxX + ((m_actualBoxSizeMaxX-m_actualBoxSizeMinX) * m_multiplier);
	m_popoutBoxSizeMaxY = m_actualBoxSizeMaxY + ((m_actualBoxSizeMaxY-m_actualBoxSizeMinY) * m_multiplier);
	m_popoutBoxSizeMaxZ = m_actualBoxSizeMaxZ + ((m_actualBoxSizeMaxZ-m_actualBoxSizeMinZ) * m_multiplier);
	m_popoutBoxSizeMinX = m_actualBoxSizeMinX - ((m_actualBoxSizeMaxX-m_actualBoxSizeMinX) * m_multiplier);
	m_popoutBoxSizeMinY = m_actualBoxSizeMinY - ((m_actualBoxSizeMaxY-m_actualBoxSizeMinY) * m_multiplier);
	m_popoutBoxSizeMinZ = m_actualBoxSizeMinZ - ((m_actualBoxSizeMaxZ-m_actualBoxSizeMinZ) * m_multiplier);

	UpdateData(FALSE);
}

void CWorldObjectPropertiesDlg::OnCHECKShowBoundingBox() 
{
    if(showBoundingBox==TRUE){
        m_showBoundingBox.SetCheck(0);
		showBoundingBox = FALSE;
	}
	else{
        m_showBoundingBox.SetCheck(1);
		showBoundingBox = TRUE;
	}	
}

//void CWorldObjectPropertiesDlg::OnBUTTONdisableContainment() 
//{
//	m_containmentbasis.EnableWindow(FALSE);
//	m_cntMiscType.EnableWindow(FALSE);
//    m_cntCurrentAmount.EnableWindow(FALSE);
//    m_cntMaxAmount.EnableWindow(FALSE);
//    m_cntRespawnTime.EnableWindow(FALSE);
//	m_cntDispersal.EnableWindow(FALSE);
//	
//	if(m_wldObjectRef->m_containmentObject){
//	delete m_wldObjectRef->m_containmentObject;
//	m_wldObjectRef->m_containmentObject=0;
//	}
//	
//}

//void CWorldObjectPropertiesDlg::OnBUTTONEnableContainment() 
//{
//	if(m_wldObjectRef->m_containmentObject){
//       MessageBox("Already enabled");
//       return;
//	}
//	
//	m_containmentbasis.EnableWindow(TRUE);
//	m_cntMiscType.EnableWindow(TRUE);
//    m_cntCurrentAmount.EnableWindow(TRUE);
//    m_cntMaxAmount.EnableWindow(TRUE);
//    m_cntRespawnTime.EnableWindow(TRUE);
//	m_cntDispersal.EnableWindow(TRUE);
//
//	m_wldObjectRef->m_containmentObject = new CContainmentObject();
//	
//    SetDlgItemInt(IDC_EDIT_cntDispersalAmount,m_wldObjectRef->m_containmentObject->m_dispersalAmount,TRUE);
//	SetDlgItemInt(IDC_EDIT_cntCurrentAmount,m_wldObjectRef->m_containmentObject->m_currentAmount,TRUE);
//	SetDlgItemInt(IDC_EDIT_cntMaxAmount,m_wldObjectRef->m_containmentObject->m_maxAmount,TRUE);
//	SetDlgItemInt(IDC_EDIT_RespawnTime,m_wldObjectRef->m_containmentObject->m_respawnTime,TRUE);
//	SetDlgItemInt(IDC_EDIT_ContMiscType,m_wldObjectRef->m_containmentObject->m_miscType,TRUE);
//	
//    m_containmentbasis.SetCurSel(0);
//}

//void CWorldObjectPropertiesDlg::OnChangeEDITContMiscType() 
//{
//	m_wldObjectRef->m_containmentObject->m_miscType = GetDlgItemInt(IDC_EDIT_ContMiscType,NULL,TRUE);   
//}
//
//void CWorldObjectPropertiesDlg::OnChangeEDITcntCurrentAmount() 
//{
//	m_wldObjectRef->m_containmentObject->m_currentAmount = GetDlgItemInt(IDC_EDIT_cntCurrentAmount,NULL,TRUE); 
//}
//
//void CWorldObjectPropertiesDlg::OnChangeEDITcntDispersalAmount() 
//{
//	m_wldObjectRef->m_containmentObject->m_dispersalAmount = GetDlgItemInt(IDC_EDIT_cntDispersalAmount,NULL,TRUE); 
//}
//
//void CWorldObjectPropertiesDlg::OnChangeEDITcntMaxAmount() 
//{
//	m_wldObjectRef->m_containmentObject->m_maxAmount = GetDlgItemInt(IDC_EDIT_cntMaxAmount,NULL,TRUE); 
//}
//
//void CWorldObjectPropertiesDlg::OnChangeEDITRespawnTime() 
//{
//	m_wldObjectRef->m_containmentObject->m_respawnTime = GetDlgItemInt(IDC_EDIT_RespawnTime,NULL,TRUE); 
//}

//void CWorldObjectPropertiesDlg::OnSelchangeCOMBOContainbasis() 
//{
//	int sel = m_containmentbasis.GetCurSel();
//	if(sel == -1)
//	   return;
//	   m_wldObjectRef->m_containmentObject->m_containmentBasis = sel;
//}

//void CWorldObjectPropertiesDlg::OnCHECKRenderInMirrors() 
//{
//	if(m_wldObjectRef->m_renderInMirrorTag == TRUE)
//	   m_wldObjectRef->m_renderInMirrorTag = FALSE;
//	else
//	   m_wldObjectRef->m_renderInMirrorTag = TRUE;
//	
//}

void CWorldObjectPropertiesDlg::OnCHECKCollisionFilter() 
{
	if(m_wldObjectRef->m_collisionInfoFilter == TRUE)
	   m_wldObjectRef->m_collisionInfoFilter = FALSE;
	else
	   m_wldObjectRef->m_collisionInfoFilter = TRUE;
}

void CWorldObjectPropertiesDlg::OnCHECKRenderFilter() 
{
	if(m_wldObjectRef->m_renderFilter == TRUE)
	   m_wldObjectRef->m_renderFilter = FALSE;
	else
	   m_wldObjectRef->m_renderFilter = TRUE;
}

void CWorldObjectPropertiesDlg::OnChangeEDITParticleLink() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_wldObjectRef->m_particleLink = m_particlelink;
	UpdateData(FALSE);
}

void CWorldObjectPropertiesDlg::OnBUTTONCharonaSystem() 
{
    CCharonaDlg clg;
	    if(m_wldObjectRef->m_meshObject)
		   clg.m_charonaRef = m_wldObjectRef->m_meshObject->m_charona;
		else
		   clg.m_charonaRef = m_wldObjectRef->m_charona;
        clg.m_textureDataBase = GL_textureDataBase;
		clg.DoModal();
		

		if(m_wldObjectRef->m_meshObject)
		   m_wldObjectRef->m_meshObject->m_charona = clg.m_charonaRef;
		else
		   m_wldObjectRef->m_charona = clg.m_charonaRef;
	
}

void CWorldObjectPropertiesDlg::OnOK() 
{
    UpdateData(TRUE);
   
    int dbIndex = m_groupFilterMod.GetCurSel()-1;
	if(dbIndex == -1)
	   m_wldObjectRef->m_filterGroup = -1;
	else{
        POSITION posLoc = m_worldGroupListRef->FindIndex(m_groupFilterMod.GetCurSel()-1);
		if(posLoc){
           CWldGroupObj * gPtr = (CWldGroupObj *)m_worldGroupListRef->GetAt(posLoc);
	       m_wldObjectRef->m_filterGroup = gPtr->List_groupInt;
		}
	}

    m_wldObjectRef->m_particleDirection.x = m_particleVectorX;
 	m_wldObjectRef->m_particleDirection.y = m_particleVectorY;
 	m_wldObjectRef->m_particleDirection.z = m_particleVectorZ;

	m_wldObjectRef->m_envReactionAttrib = m_envAttrib.GetCurSel();

	m_wldObjectRef->m_surfaceProperty = m_surfacePropertiesList.GetCurSel();

	UpdateData(FALSE);
	
	CDialog::OnOK();
}

void CWorldObjectPropertiesDlg::OnBUTTONLoadBox() 
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
       if(m_mesh->LoadGeometryFromPlugin(fileName,&dist,g_pd3dDeviceRef,NULL))
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

void CWorldObjectPropertiesDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

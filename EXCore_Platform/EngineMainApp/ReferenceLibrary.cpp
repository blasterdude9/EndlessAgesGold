// ReferenceLibrary.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "ReferenceLibrary.h"
#include "MaterialDlg.h"
#include "direct.h"
#include "LodSystemDlg.h"
#include "VertexAnimSettingsDlg.h"
#include "SkeletalAnimationDlg.h"
#include "globalstatusvariables.h"
#include "EntityLODSystemsDlg.h"
//#include "CTreeImporter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReferenceLibrary dialog


CReferenceLibrary::CReferenceLibrary(CWnd* pParent /*=NULL*/)
	: CDialog(CReferenceLibrary::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReferenceLibrary)
	m_popoutBoxMaxX = 0.0f;
	m_popoutBoxMaxY = 0.0f;
	m_popoutBoxMaxZ = 0.0f;
	m_popoutBoxMinX = 0.0f;
	m_popoutBoxMinY = 0.0f;
	m_popoutBoxMinZ = 0.0f;
	m_nameEdit = _T("");
	//}}AFX_DATA_INIT
	m_sequenceIndex = 0;
}


void CReferenceLibrary::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReferenceLibrary)
	DDX_Control(pDX, IDC_LIST_LodDB, m_lodDB);
	DDX_Control(pDX, IDC_LIST_ReferenceLibrary, m_referenceLibraryDB);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxX, m_popoutBoxMaxX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxY, m_popoutBoxMaxY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxZ, m_popoutBoxMaxZ);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinX, m_popoutBoxMinX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinY, m_popoutBoxMinY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinZ, m_popoutBoxMinZ);
    DDX_Text(pDX, IDC_EDIT_SequenceLoad, m_sequenceIndex);
	DDX_Text(pDX, IDC_EDIT_NameEdit, m_nameEdit);
	DDX_Control(pDX, IDC_CHECK_FaceToCam, m_faceCameraChk);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReferenceLibrary, CDialog)
	//{{AFX_MSG_MAP(CReferenceLibrary)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_RefMaterial, OnBUTTONRefMaterial)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBUTTONReplace)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSkeletal, OnBUTTONReplaceSkeletal)
	ON_BN_CLICKED(IDC_BUTTON_RemoveCollisionModel, OnBUTTONRemoveCollisionModel)
	ON_BN_CLICKED(IDC_BUTTON_AddCollisionModel, OnBUTTONAddCollisionModel)
	ON_BN_CLICKED(IDC_BUTTON_AddLod, OnBUTTONAddLod)
	ON_BN_CLICKED(IDC_BUTTON_DeleteLod, OnBUTTONDeleteLod)
	ON_BN_CLICKED(IDC_BUTTON_EditLod, OnBUTTONEditLod)
	ON_LBN_SELCHANGE(IDC_LIST_ReferenceLibrary, OnSelchangeLISTReferenceLibrary)
	ON_BN_CLICKED(IDC_BUTTON_SetPopoutBox, OnBUTTONSetPopoutBox)
	ON_BN_CLICKED(IDC_BUTTON_SaveLib, OnBUTTONSaveLib)
	ON_BN_CLICKED(IDC_BUTTON_SaveUnit, OnBUTTONSaveUnit)
	ON_BN_CLICKED(IDC_BUTTON_ImportUnit, OnBUTTONImportUnit)
	ON_BN_CLICKED(IDC_BUTTON_LoadLib, OnBUTTONLoadLib)
	ON_BN_CLICKED(IDC_BUTTON_SetName, OnBUTTONSetName)
	ON_BN_CLICKED(IDC_BUTTON_ReinitClipBoxes, OnBUTTONReinitClipBoxes)
	ON_BN_CLICKED(IDC_BUTTON_CopyClipBoxToLods, OnBUTTONCopyClipBoxToLods)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_AnimationSettings, OnBnClickedButtonAnimationsettings)
	ON_BN_CLICKED(IDC_BUTTON_UnifyNormals, OnBnClickedButtonUnifynormals)
	ON_BN_CLICKED(IDC_BUTTON_RegenerateNormals, OnBnClickedButtonRegeneratenormals)
	ON_BN_CLICKED(IDC_CHECK_FaceToCam, OnBnClickedCheckFacetocam)
	ON_BN_CLICKED(IDC_BUTTON_loadSpt, OnBnClickedButtonloadspt)
	ON_BN_CLICKED(IDC_BUTTON_editLodMaterial, OnBnClickedButtoneditlodmaterial)
	ON_BN_CLICKED(IDC_BUTTON_squenceLoadSetBtn, OnBnClickedButtonsquenceloadsetbtn)
	ON_BN_CLICKED(IDC_BTN_SkeletalAddBasic, &CReferenceLibrary::OnBnClickedBtnSkeletaladdbasic)
	ON_BN_CLICKED(IDOK, &CReferenceLibrary::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SkeletalEdit, &CReferenceLibrary::OnBnClickedBtnSkeletaledit)
	ON_BN_CLICKED(IDC_BTN_SkeletalLodSys, &CReferenceLibrary::OnBnClickedBtnSkeletallodsys)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceUnit, &CReferenceLibrary::OnBnClickedButtonReplaceunit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReferenceLibrary message handlers

void CReferenceLibrary::OnButtonAdd() 
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
	   CString path = opendialog.GetPathName();
	   CReferenceObj * newLibRef = new CReferenceObj();
	   newLibRef->InitFromPath(path,g_pd3dDeviceRef);
	   newLibRef->m_referenceName = opendialog.GetFileTitle();
	   newLibRef->m_popoutBox.max.x = m_popoutBoxMaxX;
	   newLibRef->m_popoutBox.max.y = m_popoutBoxMaxY;
	   newLibRef->m_popoutBox.max.z = m_popoutBoxMaxZ;
	   newLibRef->m_popoutBox.min.x = m_popoutBoxMinX;
	   newLibRef->m_popoutBox.min.y = m_popoutBoxMinY;
	   newLibRef->m_popoutBox.min.z = m_popoutBoxMinZ;

	   GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
	   //auto assign textures
	   if(newLibRef->m_meshObject)
	      gStatus->AddTextureToDatabase(newLibRef->m_meshObject,10);

	   newLibRef->m_faceCamera = m_faceCameraChk.GetCheck();
	   UpdateData(FALSE);
	   m_referenceDBREF->AddTail(newLibRef);
	 }
	 SetCurrentDirectory(MainDir);
	 InList((m_referenceDBREF->GetCount()-1));
	
}

void CReferenceLibrary::OnButtonDelete() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1)
	{
		AfxMessageBox("Must select a library reference");
		return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
	refObj->SafeDelete();
	delete refObj;
	refObj = 0;
	m_referenceDBREF->RemoveAt(posLoc);
	InList(libraryRef-1);	
}

void CReferenceLibrary::OnBUTTONRefMaterial() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}

    POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);

		if(!refObj->m_meshObject)
		{
            AfxMessageBox("no static mesh available",0,0);
			return;
		}

	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef		= m_renderStateObjectRef->m_pxShderSystemRef;
	dlg.m_vxShaderSystemRef		= m_renderStateObjectRef->m_shderSystemRef;
	dlg.m_globalShaderRef		= m_cgGlobalShaderDB;
	dlg.GL_textureDataBase		= GL_textureDataBase;
	dlg.meshRef					= refObj->m_meshObject;
	dlg.DoModal();

    GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;

    if(refObj->m_meshObject)
       refObj->m_meshObject->InitBuffer(g_pd3dDeviceRef,gStatus->m_cgGlobalShaderDB->GetByIndex(refObj->m_meshObject->List_materialObject->m_optionalShaderIndex-1));

    if(dlg.m_updateLODmatrialChain)
	   refObj->UpdateLods();


	
	
}

void CReferenceLibrary::OnBUTTONReplace() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);

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
	   CString path = opendialog.GetPathName();
	   UpdateData(FALSE);
	   refObj->InitFromPath(path,g_pd3dDeviceRef);
	   refObj->m_referenceName = path;
	   refObj->m_referenceName = opendialog.GetFileTitle();
	   refObj->m_faceCamera = m_faceCameraChk.GetCheck();
	   GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
	   if(refObj->m_meshObject)
	      gStatus->AddTextureToDatabase(refObj->m_meshObject,10);
	 }
	 SetCurrentDirectory(MainDir);
	 InList(libraryRef);
}

void CReferenceLibrary::OnBUTTONReplaceSkeletal() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;

        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
//use mapsmodels folder for organization
            CString CurDir;
            CString MainDir;
            char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
            //end use mapsmodels folder for organization
            static char BASED_CODE filter[] = "ARB Files|*.ARB|ARB Files|*.ARB|All Files (*.*)|*.*||";
	        CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
            if ( opendialog.DoModal() == IDOK )
			{ 
				 CReferenceObj * newLibRef      = new CReferenceObj();
				 newLibRef->m_skeletalOption	= new CSkeletonObject();
			     newLibRef->m_skeletalOption->m_SkeletalName	= "";
	             CString filenamePathandAll		= opendialog.GetPathName();
				 CString baseName				= opendialog.GetFileTitle();
				 UpdateData(TRUE);
				 newLibRef->m_referenceName   = m_nameEdit;
				 newLibRef->m_popoutBox.max.x = m_popoutBoxMaxX;
			     newLibRef->m_popoutBox.max.y = m_popoutBoxMaxY;
			     newLibRef->m_popoutBox.max.z = m_popoutBoxMaxZ;
			     newLibRef->m_popoutBox.min.x = m_popoutBoxMinX;
			     newLibRef->m_popoutBox.min.y = m_popoutBoxMinY;
			     newLibRef->m_popoutBox.min.z = m_popoutBoxMinZ;
			     newLibRef->m_faceCamera = m_faceCameraChk.GetCheck();

				 UpdateData(FALSE);
				 
	             if(!newLibRef->m_skeletalOption->InitBoneSystem(filenamePathandAll,g_pd3dDeviceRef))//init base skeletal
				 {
					AfxMessageBox("InitBoneSystem() Failed:"+filenamePathandAll);
					SetCurrentDirectory(MainDir);
					return;
				 }
				 newLibRef->m_skeletalOption->LoadAnimation(filenamePathandAll,TRUE,0,1,-1);//load defaul animation
				 if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD1.arb","_LOD1",g_pd3dDeviceRef))
				    if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD2.arb","_LOD2",g_pd3dDeviceRef))
						if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD3.arb","_LOD3",g_pd3dDeviceRef))
							if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD4.arb","_LOD4",g_pd3dDeviceRef))
								if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD5.arb","_LOD5",g_pd3dDeviceRef))
								{

								}

				 
				//m_referenceDBREF->AddTail(newLibRef);
				m_referenceDBREF->SetAt(posLoc, newLibRef);
	             //InList((m_referenceDBREF->GetCount()-1));
				 InList(libraryRef);
				 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 gStatus->SkeletalTextureReassignment(newLibRef->m_skeletalOption);

				 

	             CSkeletalAnimationDlg dlg;
				 dlg.m_renderStateObjectRef		= m_renderStateObjectRef;
				 dlg.m_globalShaderRef			= m_cgGlobalShaderDB;
				 dlg.m_skillDatabaseREF			= NULL;
				 dlg.m_elementDBREF				= m_elementDBREF;
				 dlg.GL_textureDataBase			= GL_textureDataBase;
				 dlg.m_skeletonReferencePtr		= newLibRef->m_skeletalOption;
				 dlg.directSoundRef				= m_dirSndRef;
				 dlg.DoModal();

			}
            SetCurrentDirectory(MainDir);
		
}

BOOL CReferenceLibrary::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	InList(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CReferenceLibrary::InList(int sel) 
{
	UpdateData(TRUE);
	m_referenceLibraryDB.ResetContent();
    for(POSITION posLoc = m_referenceDBREF->GetHeadPosition();posLoc != NULL;)
	{
        CReferenceObj * refObj = (CReferenceObj *) m_referenceDBREF->GetNext(posLoc);
        m_referenceLibraryDB.AddString(refObj->m_referenceName);
	}
	if(sel > -1)
	m_referenceLibraryDB.SetCurSel(sel);
	UpdateData(FALSE);
	return TRUE;
}

void CReferenceLibrary::OnBUTTONRemoveCollisionModel() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
		if(refObj->m_collisionModel)
		{
            refObj->m_collisionModel->SafeDelete();
			delete refObj->m_collisionModel;
			refObj->m_collisionModel = 0;
 
			AfxMessageBox("Done");
		}
		else
		    AfxMessageBox("No collision model to remove");
	
}

void CReferenceLibrary::OnBUTTONAddCollisionModel() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
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
	   CString path = opendialog.GetPathName();
	   UpdateData(FALSE);
	   refObj->InitCollisionObjectFromPath(path,g_pd3dDeviceRef);
	  
	 }
	 SetCurrentDirectory(MainDir);
}

void CReferenceLibrary::InListLods(int sel)
{
	m_lodDB.ResetContent();
    int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       return;
	}
	UpdateData(TRUE);
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
		if(!refObj->m_lodVisualList)
			return;
   
		int trace = 0;
		for(posLoc = refObj->m_lodVisualList->GetHeadPosition();posLoc != NULL;)
		{
            CMeshLODObject * mLod = (CMeshLODObject *)refObj->m_lodVisualList->GetNext(posLoc);
            m_lodDB.AddString(operator+("LOD ",m_cabLib.ConvToString(trace)));
			trace++;
		}

    if(sel > -1)
	m_lodDB.SetCurSel(sel);

	UpdateData(FALSE);

}

void CReferenceLibrary::OnBUTTONAddLod() 
{
	
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * newLibRef = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
		CLodSystemDlg dlg;

		if(!newLibRef->m_lodVisualList)
			newLibRef->m_lodVisualList = new CMeshLODObjectList();

		dlg.m_lodType = newLibRef->m_lodVisualList->m_lodType;
		
		dlg.g_pd3dDeviceRef = g_pd3dDeviceRef;
		if(dlg.DoModal()==IDOK){
			 

             ABBOX trnsferBx;
			 trnsferBx.maxX = dlg.m_maxX;
			 trnsferBx.maxY = dlg.m_maxY;
			 trnsferBx.maxZ = dlg.m_maxZ;
			 trnsferBx.minX = dlg.m_minX;
			 trnsferBx.minY = dlg.m_minY;
			 trnsferBx.minZ = dlg.m_minZ;

             if(dlg.m_lodType == 0)
			 newLibRef->m_lodVisualList->AddLOD(0,NULL,dlg.m_activation,dlg.m_fileName,newLibRef->m_meshObject->List_materialObject,g_pd3dDeviceRef);
			 else
			 newLibRef->m_lodVisualList->AddLOD(1,&trnsferBx,dlg.m_activation,dlg.m_fileName,newLibRef->m_meshObject->List_materialObject,g_pd3dDeviceRef);
			 newLibRef->m_lodVisualList->Initialize(g_pd3dDeviceRef,m_cgGlobalShaderDB);

			 //ptrLoc->UpdateLods();
			 if(newLibRef->m_lodVisualList->GetCount()==0){
                newLibRef->m_lodVisualList->SafeDelete();
				delete newLibRef->m_lodVisualList;
                newLibRef->m_lodVisualList=0;
			 }
			 InListLods(newLibRef->m_lodVisualList->GetCount()-1);
		 }
	 
}

void CReferenceLibrary::OnBUTTONDeleteLod() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1)
	{
		AfxMessageBox("Must select a library reference");
		return;
	}
	int lodIndex = m_lodDB.GetCurSel();
	if(lodIndex == -1)
	{
		AfxMessageBox("Must select a lod");
		return;
	}

	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
	CReferenceObj * newLibRef = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
	if(!newLibRef->m_lodVisualList)
		return;
	POSITION lodPos = newLibRef->m_lodVisualList->FindIndex(lodIndex);
	if(!lodPos)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CMeshLODObject * lodobj = (CMeshLODObject *)newLibRef->m_lodVisualList->GetAt(lodPos);
	lodobj->SafeDelete();
	delete lodobj;
	lodobj = 0;
	newLibRef->m_lodVisualList->RemoveAt(lodPos);
	
	InListLods(lodIndex-1);
}

void CReferenceLibrary::OnBUTTONEditLod() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	int lodIndex = m_lodDB.GetCurSel();
    if(lodIndex == -1){
       AfxMessageBox("Must select a lod");
       return;
	}

	
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * newLibRef = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
        if(!newLibRef->m_lodVisualList)
			return;

		    POSITION lodPos = newLibRef->m_lodVisualList->FindIndex(lodIndex);
			if(!lodPos)
				return;

			CMeshLODObject * mLod = (CMeshLODObject *)newLibRef->m_lodVisualList->GetAt(lodPos);

			 CLodSystemDlg dlg;
             dlg.g_pd3dDeviceRef = g_pd3dDeviceRef;
			 dlg.m_maxX = mLod->m_boxOption.maxX;
			 dlg.m_maxY = mLod->m_boxOption.maxY;
			 dlg.m_maxZ = mLod->m_boxOption.maxZ;
			 dlg.m_minX = mLod->m_boxOption.minX;
		     dlg.m_minY = mLod->m_boxOption.minY;
			 dlg.m_minZ = mLod->m_boxOption.minZ;

		
			 dlg.m_lodType = newLibRef->m_lodVisualList->m_lodType;
			

             dlg.m_activation = mLod->m_distance;

	          if(dlg.DoModal()==IDOK){

				    if(dlg.m_fileName.GetLength() > 0)
					{//change mesh
				      float sightRange = 0.0f;
					  if(mLod->m_meshLodLevel)
					  {
                        mLod->m_meshLodLevel->SafeDelete();
					    delete mLod->m_meshLodLevel;
					    mLod->m_meshLodLevel = 0;
					  }
					  mLod->m_meshLodLevel = new CEXMeshObj();
                      BOOL result = mLod->m_meshLodLevel->LoadGeometryFromPlugin(dlg.m_fileName,&sightRange,g_pd3dDeviceRef,NULL);
					  if(mLod->m_meshLodLevel->List_materialObject)
					     mLod->m_meshLodLevel->InitBuffer(g_pd3dDeviceRef,m_cgGlobalShaderDB->GetByIndex(mLod->m_meshLodLevel->List_materialObject->m_optionalShaderIndex-1));
					  else
					     mLod->m_meshLodLevel->InitBuffer(g_pd3dDeviceRef,NULL);
					}//end change mesh

			        mLod->m_boxOption.maxX = dlg.m_maxX;
			        mLod->m_boxOption.maxY = dlg.m_maxY;
			        mLod->m_boxOption.maxZ = dlg.m_maxZ;
			        mLod->m_boxOption.minX = dlg.m_minX;
			        mLod->m_boxOption.minY = dlg.m_minY;
			        mLod->m_boxOption.minZ = dlg.m_minZ;

					mLod->m_distance = dlg.m_activation;

					newLibRef->m_lodVisualList->m_lodType = dlg.m_lodType;
					
             
				  //newLibRef->UpdateLods();
			  }
}

void CReferenceLibrary::OnSelchangeLISTReferenceLibrary() 
{
	InListLods(0);

	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * newLibRef = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);

 
		UpdateData(TRUE);
	    m_popoutBoxMaxX = newLibRef->m_popoutBox.max.x;
	    m_popoutBoxMaxY = newLibRef->m_popoutBox.max.y;
	    m_popoutBoxMaxZ = newLibRef->m_popoutBox.max.z;
	    m_popoutBoxMinX = newLibRef->m_popoutBox.min.x;
	    m_popoutBoxMinY = newLibRef->m_popoutBox.min.y;
	    m_popoutBoxMinZ = newLibRef->m_popoutBox.min.z;
		m_sequenceIndex = newLibRef->m_seqLoad;
        m_faceCameraChk.SetCheck(newLibRef->m_faceCamera);
		m_nameEdit = newLibRef->m_referenceName;
	    UpdateData(FALSE);
	
}

void CReferenceLibrary::OnBUTTONSetPopoutBox() 
{
    int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * newLibRef = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);

 
		UpdateData(TRUE);
	    newLibRef->m_popoutBox.max.x = m_popoutBoxMaxX;
	    newLibRef->m_popoutBox.max.y = m_popoutBoxMaxY;
	    newLibRef->m_popoutBox.max.z = m_popoutBoxMaxZ;
	    newLibRef->m_popoutBox.min.x = m_popoutBoxMinX;
	    newLibRef->m_popoutBox.min.y = m_popoutBoxMinY;
	    newLibRef->m_popoutBox.min.z = m_popoutBoxMinZ;
	    UpdateData(FALSE);

		

        for(POSITION wldPos = m_worldObjListRef->GetHeadPosition();wldPos != NULL;)
		{//world object loop
            CWldObject * wldPtr = (CWldObject *) m_worldObjListRef->GetNext(wldPos);
            if(wldPtr->m_node)
			{//valid node
               if(wldPtr->m_node->m_libraryReference == libraryRef)
			   {//found other instance

                  wldPtr->List_popoutBox.max.x = wldPtr->m_node->m_position._41 + newLibRef->m_popoutBox.max.x;//m_nodeImporterModule.m_popoutBoxMaxX;
	              wldPtr->List_popoutBox.max.y = wldPtr->m_node->m_position._42 + newLibRef->m_popoutBox.max.y;//m_nodeImporterModule.m_popoutBoxMaxY;
        	      wldPtr->List_popoutBox.max.z = wldPtr->m_node->m_position._43 + newLibRef->m_popoutBox.max.z;//m_nodeImporterModule.m_popoutBoxMaxZ;
	              wldPtr->List_popoutBox.min.x = wldPtr->m_node->m_position._41 + newLibRef->m_popoutBox.min.x;//m_nodeImporterModule.m_popoutBoxSizeMinX;
	              wldPtr->List_popoutBox.min.y = wldPtr->m_node->m_position._42 + newLibRef->m_popoutBox.min.y;//m_nodeImporterModule.m_popoutBoxSizeMinY;
	              wldPtr->List_popoutBox.min.z = wldPtr->m_node->m_position._43 + newLibRef->m_popoutBox.min.z;//m_nodeImporterModule.m_popoutBoxSizeMinZ;

			   }//end found other instance
			}//end valid node
		}//end world object loop
}

void CReferenceLibrary::OnBUTTONSaveLib() 
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
     static char BASED_CODE filter[] = "Reference Lib Files|*.RLB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "RLB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_referenceDBREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);
	
}

void CReferenceLibrary::OnBUTTONSaveUnit() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * newLibRef = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
	 //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "Reference Unit Files (*.RUI)|*.RUI|Wavefront OBJ (*.obj)|*.obj|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "RUI", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString ext = opendialog.GetFileExt();
	   ext.MakeLower();
	   if(ext == "obj")
	   {
	       BOOL exported = FALSE;
	       if(newLibRef->m_meshObject)
	           exported = newLibRef->m_meshObject->ExportToObj(opendialog.GetPathName());
	       else if(newLibRef->m_skeletalOption)
	           exported = newLibRef->m_skeletalOption->ExportToObj(opendialog.GetPathName());
	       if(!exported)
	           AfxMessageBox("Failed to export Wavefront OBJ file.");
	       SetCurrentDirectory(MainDir);
	       return;
	   }
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << newLibRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);
}

void CReferenceLibrary::OnBUTTONImportUnit() 
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

   static char BASED_CODE filter[] = "Reference Unit Files|*.RUI|All Files (*.*)|*.*||";
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
	   CReferenceObj * newLibRef = NULL;
	   the_in_Archive >> newLibRef;
	   the_in_Archive.Close();
       fl.Close();
	   m_referenceDBREF->AddTail(newLibRef);
	   int libIndex = m_referenceDBREF->GetCount()-1;
		 if(m_worldObjListRef)
		 for(POSITION wldPos = m_worldObjListRef->GetHeadPosition();wldPos != NULL;)
			{//world object loop
				CWldObject * wldPtr = (CWldObject *) m_worldObjListRef->GetNext(wldPos);
				if(wldPtr->m_node)
				{//valid node
				   if(wldPtr->m_node->m_libraryReference == libIndex)
				   {//found other instance

					  wldPtr->List_popoutBox.max.x = wldPtr->m_node->m_position._41 + newLibRef->m_popoutBox.max.x;//m_nodeImporterModule.m_popoutBoxMaxX;
					  wldPtr->List_popoutBox.max.y = wldPtr->m_node->m_position._42 + newLibRef->m_popoutBox.max.y;//m_nodeImporterModule.m_popoutBoxMaxY;
        			  wldPtr->List_popoutBox.max.z = wldPtr->m_node->m_position._43 + newLibRef->m_popoutBox.max.z;//m_nodeImporterModule.m_popoutBoxMaxZ;
					  wldPtr->List_popoutBox.min.x = wldPtr->m_node->m_position._41 + newLibRef->m_popoutBox.min.x;//m_nodeImporterModule.m_popoutBoxSizeMinX;
					  wldPtr->List_popoutBox.min.y = wldPtr->m_node->m_position._42 + newLibRef->m_popoutBox.min.y;//m_nodeImporterModule.m_popoutBoxSizeMinY;
					  wldPtr->List_popoutBox.min.z = wldPtr->m_node->m_position._43 + newLibRef->m_popoutBox.min.z;//m_nodeImporterModule.m_popoutBoxSizeMinZ;

				   }//end found other instance
				}//end valid node
			}//end world object loop
		 }
		 SetCurrentDirectory(MainDir);
		 InList((m_referenceDBREF->GetCount()-1));
 
	 
}

void CReferenceLibrary::OnBUTTONLoadLib() 
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

   static char BASED_CODE filter[] = "Reference Library Files|*.RLB|All Files (*.*)|*.*||";
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
       if(m_referenceDBREF)
	   {
	   m_referenceDBREF->SafeDelete();
       delete m_referenceDBREF;
	   m_referenceDBREF = 0;
	   }

	   the_in_Archive >> m_referenceDBREF;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InList((m_referenceDBREF->GetCount()-1));

	 int trace = 0;
	 if(m_referenceDBREF)
     for(POSITION posLoc = m_referenceDBREF->GetHeadPosition();posLoc != NULL;)
	 {
        CReferenceObj * newLibRef = (CReferenceObj *)m_referenceDBREF->GetNext(posLoc);
		 for(POSITION wldPos = m_worldObjListRef->GetHeadPosition();wldPos != NULL;)
			{//world object loop
				CWldObject * wldPtr = (CWldObject *) m_worldObjListRef->GetNext(wldPos);
				if(wldPtr->m_node)
				{//valid node
				   if(wldPtr->m_node->m_libraryReference == trace)
				   {//found other instance

					  wldPtr->List_popoutBox.max.x = wldPtr->m_node->m_position._41 + newLibRef->m_popoutBox.max.x;//m_nodeImporterModule.m_popoutBoxMaxX;
					  wldPtr->List_popoutBox.max.y = wldPtr->m_node->m_position._42 + newLibRef->m_popoutBox.max.y;//m_nodeImporterModule.m_popoutBoxMaxY;
        			  wldPtr->List_popoutBox.max.z = wldPtr->m_node->m_position._43 + newLibRef->m_popoutBox.max.z;//m_nodeImporterModule.m_popoutBoxMaxZ;
					  wldPtr->List_popoutBox.min.x = wldPtr->m_node->m_position._41 + newLibRef->m_popoutBox.min.x;//m_nodeImporterModule.m_popoutBoxSizeMinX;
					  wldPtr->List_popoutBox.min.y = wldPtr->m_node->m_position._42 + newLibRef->m_popoutBox.min.y;//m_nodeImporterModule.m_popoutBoxSizeMinY;
					  wldPtr->List_popoutBox.min.z = wldPtr->m_node->m_position._43 + newLibRef->m_popoutBox.min.z;//m_nodeImporterModule.m_popoutBoxSizeMinZ;

				   }//end found other instance
				   wldPtr->m_node->m_refPtr = NULL;
				}//end valid node
			}//end world object loop
		 trace++;
	 }


}
//-------------------------------------------------//
void CReferenceLibrary::OnBUTTONSetName() 
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * newLibRef = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
        
		UpdateData(TRUE);

		newLibRef->m_referenceName = m_nameEdit;

		UpdateData(FALSE);
	InList(libraryRef);
}
//-------------------------------------------------//

void CReferenceLibrary::OnBUTTONReinitClipBoxes() 
{
    int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
 
		//ABBOX newBox;
		//get minimum required size (largest lod)
		//refObj->m_meshObject->GetBoxWithMarix(wldPtr->m_node->m_position,&newBox);

        


	    for(POSITION wldPos = m_worldObjListRef->GetHeadPosition();wldPos != NULL;)
		{//world object loop
            CWldObject * wldPtr = (CWldObject *) m_worldObjListRef->GetNext(wldPos);
            if(wldPtr->m_node)
			{//valid node
               if(wldPtr->m_node->m_libraryReference == libraryRef)
			   {//found other instance

                  if(refObj->m_skeletalOption)
				  {
					 wldPtr->m_node->m_clipBox = refObj->m_skeletalOption->QuerryBoxExact(&wldPtr->m_node->m_position);
                     //wldPtr->m_node->m_clipBox = refObj->m_skeletalOption->m_box;

                     /*wldPtr->m_node->m_clipBox.maxX += wldPtr->m_node->m_position._41;
                     wldPtr->m_node->m_clipBox.maxY += wldPtr->m_node->m_position._42;
                     wldPtr->m_node->m_clipBox.maxZ += wldPtr->m_node->m_position._43;

					 wldPtr->m_node->m_clipBox.minX += wldPtr->m_node->m_position._41;
                     wldPtr->m_node->m_clipBox.minY += wldPtr->m_node->m_position._42;
                     wldPtr->m_node->m_clipBox.minZ += wldPtr->m_node->m_position._43;*/
				  }
			      if(refObj->m_meshObject)
				  {
					  refObj->m_meshObject->GetBoxWithMarix(wldPtr->m_node->m_position,&wldPtr->m_node->m_clipBox);
					  //adjust for group array
					  
					  //adjust based on lods
					  if(refObj->m_lodVisualList)
					  for(POSITION refPos = refObj->m_lodVisualList->GetHeadPosition();refPos != NULL;)
					  {
						  CMeshLODObject * mLodObj = (CMeshLODObject *) refObj->m_lodVisualList->GetNext(refPos);
						  ABBOX current;
						  mLodObj->m_meshLodLevel->GetBoxWithMarix(wldPtr->m_node->m_position,&current);
						  //adjust
						  if(current.maxX > wldPtr->m_node->m_clipBox.maxX)
							 wldPtr->m_node->m_clipBox.maxX = current.maxX;

		    			  if(current.maxY > wldPtr->m_node->m_clipBox.maxY)
							 wldPtr->m_node->m_clipBox.maxY = current.maxY;

						  if(current.maxZ > wldPtr->m_node->m_clipBox.maxZ)
							 wldPtr->m_node->m_clipBox.maxZ = current.maxZ;
	   
						  if(current.minX < wldPtr->m_node->m_clipBox.minX)
							 wldPtr->m_node->m_clipBox.minX = current.minX;

						  if(current.minY < wldPtr->m_node->m_clipBox.minY)
							 wldPtr->m_node->m_clipBox.minY = current.minY;

						  if(current.minZ < wldPtr->m_node->m_clipBox.minZ)
							 wldPtr->m_node->m_clipBox.minZ = current.minZ;
					  }
				  }
			   }//end found other instance
			}//end valid node
		}//end world object loop
}
//-------------------------------------------------//

void CReferenceLibrary::OnBUTTONCopyClipBoxToLods() 
{
	// TODO: Add your control notification handler code here
	
}

void CReferenceLibrary::OnBnClickedButtonAnimationsettings()
{
    int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);


	CVertexAnimSettingsDlg dlg;

	   dlg.m_animModuleRef = refObj->m_animModule;
	   dlg.g_pd3dDevice    = g_pd3dDeviceRef;
	   dlg.DoModal();
	   refObj->m_animModule = dlg.m_animModuleRef;
}

void CReferenceLibrary::OnBnClickedButtonUnifynormals()
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
		refObj->m_meshObject->UnifyNormals();
		refObj->m_meshObject->InitBuffer(g_pd3dDeviceRef,NULL);
}

void CReferenceLibrary::OnBnClickedButtonRegeneratenormals()
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
		refObj->m_meshObject->GenerateNormals(0);
		refObj->m_meshObject->InitBuffer(g_pd3dDeviceRef,NULL);
}

void CReferenceLibrary::OnBnClickedCheckFacetocam()
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);

    UpdateData(TRUE);
	refObj->m_faceCamera = m_faceCameraChk.GetCheck();
	UpdateData(FALSE);
}

void CReferenceLibrary::OnBnClickedButtonloadspt()
{
   /*CTreeImporterObj * newTree = new CTreeImporterObj();
   newTree->GetLibraryObjFromSPTFile(m_referenceDBREF,
								     m_textureSearchPathsRef,
								     m_texturePathCountRef,
								     g_pd3dDeviceRef
						             );
   delete newTree;
   newTree = 0;
   InList(m_referenceDBREF->GetCount()-1);*/
}

void CReferenceLibrary::OnBnClickedButtoneditlodmaterial()
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	int lodIndex = m_lodDB.GetCurSel();
    if(lodIndex == -1){
       AfxMessageBox("Must select a lod");
       return;
	}

    POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);


		if(!refObj->m_lodVisualList)
			return;
		
    
		POSITION lodPos = refObj->m_lodVisualList->FindIndex(lodIndex);
        if(!lodPos)
		return;

		CMeshLODObject * meshLod = (CMeshLODObject *)refObj->m_lodVisualList->GetAt(lodPos);

	    CMaterialDlg dlg;
	    dlg.m_pxShaderSystemRef			= m_renderStateObjectRef->m_pxShderSystemRef;
	    dlg.m_vxShaderSystemRef			= m_renderStateObjectRef->m_shderSystemRef;
	    dlg.m_globalShaderRef			= m_cgGlobalShaderDB;
	    dlg.GL_textureDataBase			= GL_textureDataBase;
	    dlg.meshRef						= meshLod->m_meshLodLevel;
	    dlg.DoModal();
		

}

void CReferenceLibrary::OnBnClickedButtonsquenceloadsetbtn()
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * newLibRef = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
        
		UpdateData(TRUE);

		newLibRef->m_seqLoad = m_sequenceIndex;

		UpdateData(FALSE);
	
}

void CReferenceLibrary::OnBnClickedBtnSkeletaladdbasic()
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
            static char BASED_CODE filter[] = "ARB Files|*.ARB|ARB Files|*.ARB|All Files (*.*)|*.*||";
	        CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
            if ( opendialog.DoModal() == IDOK )
			{ 
				 CReferenceObj * newLibRef      = new CReferenceObj();
				 newLibRef->m_skeletalOption	= new CSkeletonObject();
			     newLibRef->m_skeletalOption->m_SkeletalName	= "";
	             CString filenamePathandAll		= opendialog.GetPathName();
				 CString baseName				= opendialog.GetFileTitle();
				 UpdateData(TRUE);
				 newLibRef->m_referenceName   = m_nameEdit;
				 newLibRef->m_popoutBox.max.x = m_popoutBoxMaxX;
			     newLibRef->m_popoutBox.max.y = m_popoutBoxMaxY;
			     newLibRef->m_popoutBox.max.z = m_popoutBoxMaxZ;
			     newLibRef->m_popoutBox.min.x = m_popoutBoxMinX;
			     newLibRef->m_popoutBox.min.y = m_popoutBoxMinY;
			     newLibRef->m_popoutBox.min.z = m_popoutBoxMinZ;
			     newLibRef->m_faceCamera = m_faceCameraChk.GetCheck();

				 UpdateData(FALSE);
				 
	             if(!newLibRef->m_skeletalOption->InitBoneSystem(filenamePathandAll,g_pd3dDeviceRef))//init base skeletal
				 {
					AfxMessageBox("InitBoneSystem() Failed:"+filenamePathandAll);
					SetCurrentDirectory(MainDir);
					return;
				 }
				 newLibRef->m_skeletalOption->LoadAnimation(filenamePathandAll,TRUE,0,1,-1);//load defaul animation
				 if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD1.arb","_LOD1",g_pd3dDeviceRef))
				    if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD2.arb","_LOD2",g_pd3dDeviceRef))
						if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD3.arb","_LOD3",g_pd3dDeviceRef))
							if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD4.arb","_LOD4",g_pd3dDeviceRef))
								if(newLibRef->m_skeletalOption->MergeAsLod(baseName + "_LOD5.arb","_LOD5",g_pd3dDeviceRef))
								{

								}

				 m_referenceDBREF->AddTail(newLibRef);
	             InList((m_referenceDBREF->GetCount()-1));
				 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 gStatus->SkeletalTextureReassignment(newLibRef->m_skeletalOption);

				 

	             CSkeletalAnimationDlg dlg;
				 dlg.m_renderStateObjectRef		= m_renderStateObjectRef;
				 dlg.m_globalShaderRef			= m_cgGlobalShaderDB;
				 dlg.m_skillDatabaseREF			= NULL;
				 dlg.m_elementDBREF				= m_elementDBREF;
				 dlg.GL_textureDataBase			= GL_textureDataBase;
				 dlg.m_skeletonReferencePtr		= newLibRef->m_skeletalOption;
				 dlg.directSoundRef				= m_dirSndRef;
				 dlg.DoModal();

			}
            SetCurrentDirectory(MainDir);
}

void CReferenceLibrary::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CReferenceLibrary::OnBnClickedBtnSkeletaledit()
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}

    POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
		if(!refObj->m_skeletalOption)
		{
            AfxMessageBox("Reference is not a skeletal object");
			return;
		}

         CSkeletalAnimationDlg dlg;
		 dlg.m_renderStateObjectRef		= m_renderStateObjectRef;
		 dlg.m_globalShaderRef			= m_cgGlobalShaderDB;
		 dlg.m_skillDatabaseREF			= NULL;
		 dlg.m_elementDBREF				= m_elementDBREF;
		 dlg.GL_textureDataBase			= GL_textureDataBase;
		 dlg.m_skeletonReferencePtr		= refObj->m_skeletalOption;
		 dlg.directSoundRef				= m_dirSndRef;
		 dlg.DoModal();

		 if(refObj->m_skeletalOptionLive)
		 {
            refObj->m_skeletalOptionLive->SafeDelete();
			delete refObj->m_skeletalOptionLive;
			refObj->m_skeletalOptionLive = 0;
		 }
}


void CReferenceLibrary::OnBnClickedBtnSkeletallodsys()
{
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}

    POSITION posLoc = m_referenceDBREF->FindIndex(libraryRef);
	if(!posLoc)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(posLoc);
		if(!refObj->m_skeletalOption)
		{
            AfxMessageBox("Reference is not a skeletal object");
			return;
		}

        CEntityLODSystemsDlg dlg;
		dlg.m_lodRangeRefList = refObj->m_skeletalOption->m_lodRanges;
		if(dlg.DoModal()==IDOK){
		  refObj->m_skeletalOption->m_lodRanges = dlg.m_lodRangeRefList;
		  if(refObj->m_skeletalOption->m_lodRanges)
		   if(refObj->m_skeletalOption->m_lodRanges->GetCount() == 0){
			  refObj->m_skeletalOption->m_lodRanges->SafeDelete();
			  delete refObj->m_skeletalOption->m_lodRanges;
			  refObj->m_skeletalOption->m_lodRanges = NULL;
			  refObj->m_skeletalOption->m_currentLOD = 0;
		   }
		}

		if(refObj->m_skeletalOptionLive)
		{
            refObj->m_skeletalOptionLive->SafeDelete();
			delete refObj->m_skeletalOptionLive;
			refObj->m_skeletalOptionLive = 0;
		}
}

void CReferenceLibrary::OnBnClickedButtonReplaceunit()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
	int libraryRef = m_referenceLibraryDB.GetCurSel();
	if(libraryRef == -1){
       AfxMessageBox("Must select a library reference");
       return;
	}
	
   SetCurrentDirectory(gStatus->GL_pathGameFiles);//use gamefiles folder for organization

   static char BASED_CODE filter[] = "Reference Unit Files|*.RUI|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(gStatus->GL_workingDirectory);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(gStatus->GL_workingDirectory);
		  return;
	   }
	   gStatus->CacheGeneratedRenderStuff(false);

       POSITION markerPos = m_referenceDBREF->FindIndex(libraryRef);
	   if(!markerPos)
		return;
        CReferenceObj * refObj = (CReferenceObj *)m_referenceDBREF->GetAt(markerPos);
      

	   CArchive the_in_Archive(&fl,CArchive::load);
	   CReferenceObj * newLibRef = NULL;
	   the_in_Archive >> newLibRef;
	   the_in_Archive.Close();
       fl.Close();

	   refObj->CopyValues(newLibRef,g_pd3dDeviceRef);
	   if(newLibRef->m_skeletalOption)
	   if(newLibRef->m_skeletalOption->m_alternateConfigurations)
	   {
	      refObj->m_skeletalOption->m_alternateConfigurations = newLibRef->m_skeletalOption->m_alternateConfigurations;
	      newLibRef->m_skeletalOption->m_alternateConfigurations = 0;
	   }

	   if(refObj->m_skeletalOption)
	      gStatus->SkeletalTextureReassignment(refObj->m_skeletalOption);

	   if(refObj->m_meshObject)
	      gStatus->AddTextureToDatabase(refObj->m_meshObject,10);
	   
	   delete newLibRef;
	   newLibRef = 0;

	   int libIndex = libraryRef;
	   if(m_worldObjListRef)
	   for(POSITION wldPos = m_worldObjListRef->GetHeadPosition();wldPos != NULL;)
		{//world object loop
            CWldObject * wldPtr = (CWldObject *) m_worldObjListRef->GetNext(wldPos);
            if(wldPtr->m_node)
			{//valid node
               if(wldPtr->m_node->m_libraryReference == libIndex)
			   {//found other instance

                  wldPtr->List_popoutBox.max.x = wldPtr->m_node->m_position._41 + newLibRef->m_popoutBox.max.x;//m_nodeImporterModule.m_popoutBoxMaxX;
	              wldPtr->List_popoutBox.max.y = wldPtr->m_node->m_position._42 + newLibRef->m_popoutBox.max.y;//m_nodeImporterModule.m_popoutBoxMaxY;
        	      wldPtr->List_popoutBox.max.z = wldPtr->m_node->m_position._43 + newLibRef->m_popoutBox.max.z;//m_nodeImporterModule.m_popoutBoxMaxZ;
	              wldPtr->List_popoutBox.min.x = wldPtr->m_node->m_position._41 + newLibRef->m_popoutBox.min.x;//m_nodeImporterModule.m_popoutBoxSizeMinX;
	              wldPtr->List_popoutBox.min.y = wldPtr->m_node->m_position._42 + newLibRef->m_popoutBox.min.y;//m_nodeImporterModule.m_popoutBoxSizeMinY;
	              wldPtr->List_popoutBox.min.z = wldPtr->m_node->m_position._43 + newLibRef->m_popoutBox.min.z;//m_nodeImporterModule.m_popoutBoxSizeMinZ;

			   }//end found other instance
			}//end valid node
		}//end world object loop
	   
	 }
     SetCurrentDirectory(gStatus->GL_workingDirectory);
	 InList(libraryRef);
}

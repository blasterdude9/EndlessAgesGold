// ObjectLibraryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "ObjectLibraryDlg.h"
#include "direct.h"
#include "materialdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectLibraryDlg dialog


CObjectLibraryDlg::CObjectLibraryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectLibraryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectLibraryDlg)
	m_description = _T("");
	m_lodRange = 0.0f;
	m_popoutBoxBasis = 0.0f;
	m_lodBoxMaxX = 0.0f;
	m_lodBoxMaxY = 0.0f;
	m_lodBoxMaxZ = 0.0f;
	m_lodBoxMinX = 0.0f;
	m_lodBoxMinY = 0.0f;
	m_lodBoxMinZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CObjectLibraryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectLibraryDlg)
	DDX_Control(pDX, IDC_CHECK_EnableBoxType, m_checkEnableBoxType);
	DDX_Control(pDX, IDC_LIST_LodList, m_lodList);
	DDX_Control(pDX, IDC_LIST_ObjectContent, m_contentList);
	DDX_Control(pDX, IDC_LIST_LibraryObjects, m_libraryList);
	DDX_Text(pDX, IDC_EDIT_description, m_description);
	DDX_Text(pDX, IDC_EDIT_Range, m_lodRange);
	DDX_Text(pDX, IDC_EDIT_popoutBoxBasis, m_popoutBoxBasis);
	DDX_Text(pDX, IDC_EDIT_LodBoxSizeMaxX, m_lodBoxMaxX);
	DDX_Text(pDX, IDC_EDIT_LodBoxSizeMaxY, m_lodBoxMaxY);
	DDX_Text(pDX, IDC_EDIT_lodBoxSizeMaxZ, m_lodBoxMaxZ);
	DDX_Text(pDX, IDC_EDIT_lodBoxSizeMinX, m_lodBoxMinX);
	DDX_Text(pDX, IDC_EDIT_lodBoxSizeMinY, m_lodBoxMinY);
	DDX_Text(pDX, IDC_EDIT_lodBoxSizeMinZ, m_lodBoxMinZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectLibraryDlg, CDialog)
	//{{AFX_MSG_MAP(CObjectLibraryDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddLibraryObject, OnBUTTONAddLibraryObject)
	ON_BN_CLICKED(IDC_BUTTON_AddObject, OnBUTTONAddObject)
	ON_BN_CLICKED(IDC_BUTTON_Import, OnBUTTONImport)
	ON_LBN_SELCHANGE(IDC_LIST_LibraryObjects, OnSelchangeLISTLibraryObjects)
	ON_BN_CLICKED(IDC_BUTTON_SetDescription, OnBUTTONSetDescription)
	ON_BN_CLICKED(IDC_BUTTON_SetAltCollisionModel, OnBUTTONSetAltCollisionModel)
	ON_BN_CLICKED(IDC_BUTTON_DeleteLibraryObject, OnBUTTONDeleteLibraryObject)
	ON_BN_CLICKED(IDC_BUTTON_RemoveAltCollisionModel, OnBUTTONRemoveAltCollisionModel)
	ON_BN_CLICKED(IDC_BUTTON_DeleteObject, OnBUTTONDeleteObject)
	ON_BN_CLICKED(IDC_BUTTON_MaterialAccess, OnBUTTONMaterialAccess)
	ON_BN_CLICKED(IDC_BUTTON_SaveLibrary, OnBUTTONSaveLibrary)
	ON_BN_CLICKED(IDC_BUTTON_LoadLibrary, OnBUTTONLoadLibrary)
	ON_BN_CLICKED(IDC_BUTTON_AddLod, OnBUTTONAddLod)
	ON_LBN_SELCHANGE(IDC_LIST_ObjectContent, OnSelchangeLISTObjectContent)
	ON_BN_CLICKED(IDC_BUTTON_SetLodRange, OnBUTTONSetLodRange)
	ON_BN_CLICKED(IDC_BUTTON_DeleteLod, OnBUTTONDeleteLod)
	ON_BN_CLICKED(IDC_BUTTON_SetPopoutBox, OnBUTTONSetPopoutBox)
	ON_BN_CLICKED(IDC_BUTTON_Merge, OnBUTTONMerge)
	ON_BN_CLICKED(IDC_BUTTON_SetLodBoxValues, OnBUTTONSetLodBoxValues)
	ON_LBN_SELCHANGE(IDC_LIST_LodList, OnSelchangeLISTLodList)
	ON_BN_CLICKED(IDC_CHECK_EnableBoxType, OnCHECKEnableBoxType)
	ON_BN_CLICKED(IDC_BUTTON_setGlobalFade, OnBUTTONsetGlobalFade)
	ON_BN_CLICKED(IDC_BUTTON_setGlobalColor, OnBUTTONsetGlobalColor)
	ON_BN_CLICKED(IDC_BUTTON_setGlobalBoundingBox, OnBUTTONsetGlobalBoundingBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectLibraryDlg message handlers

BOOL CObjectLibraryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListLibraryObjects(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::InListLibraryObjects(int sel)
{
	UpdateData(TRUE);
    m_libraryList.ResetContent();
	for(POSITION posLoc = m_objectLibraryRef->GetHeadPosition();posLoc != NULL;)
	{
        CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetNext(posLoc);
        m_libraryList.AddString(lPtr->m_collection->m_description);
	}
    if(sel > -1)
    m_libraryList.SetCurSel(sel);
	UpdateData(FALSE);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONAddLibraryObject() 
{
    UpdateData(TRUE);
	CObjectCollection * newPtr = new CObjectCollection;
	newPtr->m_collection = new CObjectLibraryObjList;
	newPtr->m_collection->m_description = m_description;
	m_objectLibraryRef->AddTail(newPtr);
	UpdateData(FALSE);
	InListLibraryObjects(m_objectLibraryRef->GetCount()-1);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONAddObject() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       MessageBox("Must select a base library");
       return;
	}
	

    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
        CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);
        /*//use mapsmodels folder for organization
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
         if ( opendialog.DoModal() != IDOK )
		 {
		   SetCurrentDirectory(MainDir);
		   return;
		 }
		 CString idName = opendialog.GetFileTitle(); 
		 CString fileName = opendialog.GetPathName();
	     SetCurrentDirectory(MainDir);

         CObjectLibraryObj * newPtr = new CObjectLibraryObj();
         if(newPtr->InitFromFile(fileName)){//success
            lPtr->m_collection->AddTail(newPtr);
			newPtr->m_wldObject->m_fileNameRef = idName;
		 }//end success
		 else{
            MessageBox("Failed to load file");
		 }*/
		 
         //multiselect

         //char temp;
         char bigBuff[20000] = "";  // maximum common dialog buffer size
         //use mapsmodels folder for organization
         CString CurDir;
         CString MainDir;
         CString Mdir;
         char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 Mdir = CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
         //end use mapsmodels folder for organization
	     static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	     CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT |OFN_FILEMUSTEXIST, filter, this );
         opendialog.m_ofn.hInstance = AfxGetInstanceHandle();
	     opendialog.m_ofn.lpstrFile = bigBuff;
	     opendialog.m_ofn.nMaxFile = sizeof(bigBuff);
	     if ( opendialog.DoModal() == IDOK )
		 {//file chosen
	         //solisify directory
		     char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 CurDir = buffer;
		     //-------------build selected files database------------//
		     CString FileNames[2000];
	         int counter = 0;
	         int length = 0;
	         int length2 = 0;
	         for(int loop=0;loop<15000;loop++){
		        if(bigBuff[loop] == NULL)
				{
		           counter++;
		           if(bigBuff[loop+1] == NULL)
                      break; 
		              loop++;
				}
		        CString temp = (CString)bigBuff[loop];
		        FileNames[counter] = operator+(FileNames[counter],temp);
			 }
	         if(counter == 1){
                FileNames[1] = opendialog.GetFileName();
	            counter++;
			 }
		     SetCurrentDirectory(CurDir);
	         //---------end-build selected files database------------//
		     for(int loop=1;loop<counter;loop++){//begin file load loop
                 CObjectLibraryObj * newPtr = new CObjectLibraryObj();
                 if(newPtr->InitFromFile(FileNames[loop],g_pd3dDeviceRef)){//success
                    lPtr->m_collection->AddTail(newPtr);
			        newPtr->m_wldObject->m_fileNameRef = FileNames[loop];
				 }//end success
		         else{
                     MessageBox("Failed to load file");
				 }
     

			     //SetCurrentDirectory(CurDir);//set to base location
			    // LoadWorldObject(FileNames[loop],&retBasisLoc);	
			     //SetCurrentDirectory(Mdir);//set to maps and models
			 }
	  }// end file chosen		  
      SetCurrentDirectory(MainDir);












         //end multiselect


		 InListContents(lPtr->m_collection->GetCount()-1);

}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::InListContents(int sel)
{

int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       MessageBox("Must select a base library");
       return;
	}
    UpdateData(TRUE);
    m_contentList.ResetContent();
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
        CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


        for(POSITION cPos = lPtr->m_collection->GetHeadPosition();cPos != NULL;)
		{
            CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetNext(cPos);
            if(libObj->m_wldObject)
			   m_contentList.AddString(libObj->m_wldObject->m_fileNameRef);
			else
			   m_contentList.AddString("NULL");
		}
    UpdateData(FALSE);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONImport() 
{
	m_finalIndex = m_libraryList.GetCurSel();
	if(m_finalIndex == -1){
       MessageBox("Library must be selected for export");
       return;
	}
	
	
	CDialog::OnOK();
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnOK() 
{
	
	   
	
	CDialog::OnOK();
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnSelchangeLISTLibraryObjects() 
{


	InListContents(0);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONSetDescription() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       MessageBox("Must select a base library");
       return;
	}
    UpdateData(TRUE);
    m_contentList.ResetContent();
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
        CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);
        lPtr->m_collection->m_description = m_description;
    UpdateData(FALSE);
	InListLibraryObjects(baseLibrarySelection);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONSetAltCollisionModel() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       MessageBox("Must select a base library");
       return;
	}
	

    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
        CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);
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
         if ( opendialog.DoModal() != IDOK )
		 {
		   SetCurrentDirectory(MainDir);
		   return;
		 }
		 CString idName = opendialog.GetFileTitle(); 
		 CString fileName = opendialog.GetPathName();
	     SetCurrentDirectory(MainDir);
         
		 
         CObjectLibraryObj * newPtr = new CObjectLibraryObj();
         if(newPtr->InitFromFile(fileName,g_pd3dDeviceRef)){//success
			if(lPtr->m_collection->m_altCollisionModel){
              lPtr->m_collection->m_altCollisionModel->SafeDelete();
			  delete lPtr->m_collection->m_altCollisionModel;
			  lPtr->m_collection->m_altCollisionModel = 0;
			}  
			 newPtr->m_wldObject->m_fileNameRef = idName;
			 lPtr->m_collection->m_altCollisionModel = newPtr->m_wldObject;
		 }//end success
		 else{
            MessageBox("Failed to load file");
		 }
		 
		 InListContents(lPtr->m_collection->GetCount()-1);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONDeleteLibraryObject() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       MessageBox("Must select a base library");
       return;
	}
	

    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
        CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);
		lPtr->SafeDelete();
		delete lPtr;
		lPtr = 0;
		m_objectLibraryRef->RemoveAt(posLoc);
		InListLibraryObjects(baseLibrarySelection-1);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONRemoveAltCollisionModel() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       MessageBox("Must select a base library");
       return;
	}
	

    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
        CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);
        if(lPtr->m_collection->m_altCollisionModel)
		{
           lPtr->m_collection->m_altCollisionModel->SafeDelete();
           delete lPtr->m_collection->m_altCollisionModel; 
           lPtr->m_collection->m_altCollisionModel = 0;
		}
     AfxMessageBox("Removed Alternate Collision Model");
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONDeleteObject() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       MessageBox("Must select a base library");
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
       MessageBox("Must select a library content object");
       return;
	}

    UpdateData(TRUE);
    m_contentList.ResetContent();
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
        CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


        POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
		if(!cPos)
			return;
            CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
            libObj->SafeDelete();
			delete libObj;
			libObj = 0;
			lPtr->m_collection->RemoveAt(cPos);

    UpdateData(FALSE);
	InListContents(contentSelection-1);
}

//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONSaveLibrary() 
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
     static char BASED_CODE filter[] = "ODB Library Files|*.ODB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "ODB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_objectLibraryRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONLoadLibrary() 
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

   static char BASED_CODE filter[] = "ODB Files|*.ODB|All Files (*.*)|*.*||";
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
	   the_in_Archive >> m_objectLibraryRef;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListLibraryObjects(0);
	 
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONAddLod() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       MessageBox("Must select a base library");
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
       MessageBox("Must select a library content object");
       return;
	}

	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

     static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
       if(!libObj->m_wldObject->m_lodVisualList)
		   libObj->m_wldObject->m_lodVisualList = new CMeshLODObjectList;

	   libObj->m_wldObject->m_lodVisualList->AddLOD(0,
		                                            NULL,
													m_lodRange,
													fileName,
													libObj->m_wldObject->m_meshObject->List_materialObject,
													g_pd3dDeviceRef);


	   InListLods(libObj->m_wldObject->m_lodVisualList->GetCount()-1);
	 }
	 libObj->m_wldObject->UpdateLods();
	 SetCurrentDirectory(MainDir);
	 UpdateData(FALSE);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::InListLods(int sel)
{
int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
       return;
	}
    m_lodList.ResetContent();
	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	
	UpdateData(TRUE);
    m_lodBoxMaxX = libObj->m_wldObject->List_popoutBox.max.x;
	m_lodBoxMaxY = libObj->m_wldObject->List_popoutBox.max.y;
	m_lodBoxMaxZ = libObj->m_wldObject->List_popoutBox.max.z;
	m_lodBoxMinX = libObj->m_wldObject->List_popoutBox.min.x;
	m_lodBoxMinY = libObj->m_wldObject->List_popoutBox.min.y;
	m_lodBoxMinZ = libObj->m_wldObject->List_popoutBox.min.z;
	UpdateData(FALSE);



	if(libObj->m_wldObject->m_lodVisualList)
        for(POSITION lPos = libObj->m_wldObject->m_lodVisualList->GetHeadPosition();lPos != NULL;)
		{
            CMeshLODObject * mLodPtr = (CMeshLODObject *)libObj->m_wldObject->m_lodVisualList->GetNext(lPos);

            CString build = operator+("LOD:",m_cablib.ConvToString(mLodPtr->m_distance));
		
		    m_lodList.AddString(build);
		}


	 UpdateData(FALSE);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnSelchangeLISTObjectContent() 
{
	InListLods(0);
	
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONSetLodRange() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
       return;
	}


    int lodLevel = m_lodList.GetCurSel();
    if(lodLevel == -1)
	{
       return;
	}


	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	 
	if(libObj->m_wldObject->m_lodVisualList){
       POSITION lPos = libObj->m_wldObject->m_lodVisualList->FindIndex(lodLevel);
       if(lPos){
            CMeshLODObject * mLodPtr = (CMeshLODObject *)libObj->m_wldObject->m_lodVisualList->GetAt(lPos);
            mLodPtr->m_distance = m_lodRange;
	   }
	}
	InListLods(lodLevel);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONDeleteLod() 
{
    int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
       return;
	}


    int lodLevel = m_lodList.GetCurSel();
    if(lodLevel == -1)
	{
       return;
	}


	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	 
	if(libObj->m_wldObject->m_lodVisualList){
       POSITION lPos = libObj->m_wldObject->m_lodVisualList->FindIndex(lodLevel);
       if(lPos){
            CMeshLODObject * mLodPtr = (CMeshLODObject *)libObj->m_wldObject->m_lodVisualList->GetAt(lPos);
            mLodPtr->SafeDelete();
			delete mLodPtr;
			mLodPtr=0;
			libObj->m_wldObject->m_lodVisualList->RemoveAt(lPos);
	   }
	   

	   if(libObj->m_wldObject->m_lodVisualList->GetCount() == 0){
          delete libObj->m_wldObject->m_lodVisualList;
		  libObj->m_wldObject->m_lodVisualList=0;
	   }
	}
	InListLods(lodLevel-1);
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONMaterialAccess() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
       return;
	}

	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);

    CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
    dlg.GL_textureDataBase = GL_textureDataBase;
    dlg.meshRef = libObj->m_wldObject->m_meshObject;
	dlg.makeMirror = libObj->m_wldObject->m_meshObject->List_materialObject->m_mirrorEnabled;
	if(dlg.DoModal() == IDOK)
	{
       libObj->m_wldObject->m_meshObject->List_materialObject->m_mirrorEnabled = dlg.makeMirror;
	   libObj->m_wldObject->UpdateLods();
	}
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONSetPopoutBox() 
{
    int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
	   MessageBox("Must select a library");
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
	   MessageBox("Must select a content object");
       return;
	}

	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);

	UpdateData(TRUE);
    libObj->m_wldObject->List_popoutBox.max.x = m_lodBoxMaxX;
	libObj->m_wldObject->List_popoutBox.max.y = m_lodBoxMaxY;
	libObj->m_wldObject->List_popoutBox.max.z = m_lodBoxMaxZ;
	libObj->m_wldObject->List_popoutBox.min.x = m_lodBoxMinX;
	libObj->m_wldObject->List_popoutBox.min.y = m_lodBoxMinY;
	libObj->m_wldObject->List_popoutBox.min.z = m_lodBoxMinZ;
	UpdateData(FALSE);

	//libObj->m_wldObject->CalibrateCenteredBoundingBox((int)m_popoutBoxBasis);
	MessageBox("Done");
}
//------------------------------------------------//
//------------------------------------------------//
//------------------------------------------------//
void CObjectLibraryDlg::OnBUTTONMerge() 
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
   CObjectCollectionList * m_objectPatchList = NULL;
   static char BASED_CODE filter[] = "ODB Files|*.ODB|All Files (*.*)|*.*||";
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
	   the_in_Archive >> m_objectPatchList;
	   the_in_Archive.Close();
       fl.Close();
	 
	   for(POSITION ptPos = m_objectPatchList->GetHeadPosition();ptPos != NULL;)
	   {
	     CObjectCollection * lPtr = (CObjectCollection *)m_objectPatchList->GetNext(ptPos);
	     m_objectLibraryRef->AddTail(lPtr);
	   }
	 }
     SetCurrentDirectory(MainDir);
	 InListLibraryObjects(0);	
}

void CObjectLibraryDlg::OnBUTTONSetLodBoxValues() 
{
    int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
	   AfxMessageBox("You must select a library object");
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
	   AfxMessageBox("You must select a content object object");
       return;
	}


    int lodLevel = m_lodList.GetCurSel();
    if(lodLevel == -1)
	{
	   AfxMessageBox("You must select a lod level");
       return;
	}


	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	 
	if(libObj->m_wldObject->m_lodVisualList){
       POSITION lPos = libObj->m_wldObject->m_lodVisualList->FindIndex(lodLevel);
       if(lPos){
            CMeshLODObject * mLodPtr = (CMeshLODObject *)libObj->m_wldObject->m_lodVisualList->GetAt(lPos);
            

               mLodPtr->m_boxOption.maxX = m_lodBoxMaxX;
	           mLodPtr->m_boxOption.maxY = m_lodBoxMaxY;
	           mLodPtr->m_boxOption.maxZ = m_lodBoxMaxZ;
	           mLodPtr->m_boxOption.minX = m_lodBoxMinX;
	           mLodPtr->m_boxOption.minY = m_lodBoxMinY;
	           mLodPtr->m_boxOption.minZ = m_lodBoxMinZ;
	   }
	}
	UpdateData(FALSE);
}

void CObjectLibraryDlg::OnSelchangeLISTLodList() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
       return;
	}


    int lodLevel = m_lodList.GetCurSel();
    if(lodLevel == -1)
	{
       return;
	}


	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	 
	if(libObj->m_wldObject->m_lodVisualList){
       POSITION lPos = libObj->m_wldObject->m_lodVisualList->FindIndex(lodLevel);
       if(lPos){
            CMeshLODObject * mLodPtr = (CMeshLODObject *)libObj->m_wldObject->m_lodVisualList->GetAt(lPos);
            
			if(libObj->m_wldObject->m_lodVisualList->m_lodType == 0)
			{
			   m_checkEnableBoxType.SetCheck(0);
			   m_lodRange = mLodPtr->m_distance;
			   m_lodBoxMaxX = 0.0f;
	           m_lodBoxMaxY = 0.0f;
	           m_lodBoxMaxZ = 0.0f;
	           m_lodBoxMinX = 0.0f;
	           m_lodBoxMinY = 0.0f;
	           m_lodBoxMinZ = 0.0f;
			}
			else
			{
               m_checkEnableBoxType.SetCheck(1);

			   m_lodBoxMaxX = mLodPtr->m_boxOption.maxX;
	           m_lodBoxMaxY = mLodPtr->m_boxOption.maxY;
	           m_lodBoxMaxZ = mLodPtr->m_boxOption.maxZ;
	           m_lodBoxMinX = mLodPtr->m_boxOption.minX;
	           m_lodBoxMinY = mLodPtr->m_boxOption.minY;
	           m_lodBoxMinZ = mLodPtr->m_boxOption.minZ;

			}
	   }
	}
	UpdateData(FALSE);
}

void CObjectLibraryDlg::OnCHECKEnableBoxType() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
	   AfxMessageBox("You must select a library object");
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
	   AfxMessageBox("You must select a content object object");
       return;
	}


    int lodLevel = m_lodList.GetCurSel();
    if(lodLevel == -1)
	{
	   AfxMessageBox("You must select a lod level");
       return;
	}


	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	 
	if(libObj->m_wldObject->m_lodVisualList){
       POSITION lPos = libObj->m_wldObject->m_lodVisualList->FindIndex(lodLevel);
       if(lPos){
            CMeshLODObject * mLodPtr = (CMeshLODObject *)libObj->m_wldObject->m_lodVisualList->GetAt(lPos);
            
			if(libObj->m_wldObject->m_lodVisualList->m_lodType == 1)
			{
			   m_checkEnableBoxType.SetCheck(0);
			   libObj->m_wldObject->m_lodVisualList->m_lodType = 0;
			 
			}
			else
			{
               m_checkEnableBoxType.SetCheck(1);
			   libObj->m_wldObject->m_lodVisualList->m_lodType = 1;

			}
	   }
	}
	UpdateData(FALSE);
}

void CObjectLibraryDlg::OnBUTTONsetGlobalFade() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
	   AfxMessageBox("You must select a library object");
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
	   AfxMessageBox("You must select a content object object");
       return;
	}


	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	 
	libObj->m_wldObject->m_meshObject->List_materialObject;

    for(POSITION lPos = lPtr->m_collection->GetHeadPosition();lPos != NULL;)
	{
        CObjectLibraryObj * libObject = (CObjectLibraryObj *)lPtr->m_collection->GetNext(lPos);

        libObject->m_wldObject->m_meshObject->List_materialObject->m_fadeByDistance = libObj->m_wldObject->m_meshObject->List_materialObject->m_fadeByDistance;
		libObject->m_wldObject->m_meshObject->List_materialObject->m_fadeMaxDist = libObj->m_wldObject->m_meshObject->List_materialObject->m_fadeMaxDist;
        libObject->m_wldObject->m_meshObject->List_materialObject->m_fadeVisDist = libObj->m_wldObject->m_meshObject->List_materialObject->m_fadeVisDist;

	}



	UpdateData(FALSE);
	
}

void CObjectLibraryDlg::OnBUTTONsetGlobalColor() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
	   AfxMessageBox("You must select a library object");
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
	   AfxMessageBox("You must select a content object object");
       return;
	}


	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	 
	libObj->m_wldObject->m_meshObject->List_materialObject;

    for(POSITION lPos = lPtr->m_collection->GetHeadPosition();lPos != NULL;)
	{
        CObjectLibraryObj * libObject = (CObjectLibraryObj *)lPtr->m_collection->GetNext(lPos);

        libObject->m_wldObject->m_meshObject->List_materialObject->m_baseMaterial = libObj->m_wldObject->m_meshObject->List_materialObject->m_baseMaterial;
		libObject->m_wldObject->m_meshObject->List_materialObject->m_baseBlendMode = libObj->m_wldObject->m_meshObject->List_materialObject->m_baseBlendMode;
        libObject->m_wldObject->m_meshObject->List_materialObject->Reset();
	}



	UpdateData(FALSE);
}

void CObjectLibraryDlg::OnBUTTONsetGlobalBoundingBox() 
{
	int baseLibrarySelection = m_libraryList.GetCurSel();
	if(baseLibrarySelection == -1)
	{
	   AfxMessageBox("You must select a library object");
       return;
	}


    int contentSelection = m_contentList.GetCurSel();
    if(contentSelection == -1)
	{
	   AfxMessageBox("You must select a content object object");
       return;
	}


	UpdateData(TRUE);
    POSITION posLoc = m_objectLibraryRef->FindIndex(baseLibrarySelection);
	if(!posLoc)
		return;
    CObjectCollection * lPtr = (CObjectCollection *)m_objectLibraryRef->GetAt(posLoc);


    POSITION cPos = lPtr->m_collection->FindIndex(contentSelection);
    if(!cPos)
	    return;
    CObjectLibraryObj * libObj = (CObjectLibraryObj *)lPtr->m_collection->GetAt(cPos);
	 
	libObj->m_wldObject->m_meshObject->List_materialObject;

    for(POSITION lPos = lPtr->m_collection->GetHeadPosition();lPos != NULL;)
	{
        CObjectLibraryObj * libObject = (CObjectLibraryObj *)lPtr->m_collection->GetNext(lPos);

        CopyMemory(&libObject->m_wldObject->List_popoutBox,&libObj->m_wldObject->List_popoutBox,sizeof(BNDBOX));
		
	}



	UpdateData(FALSE);
}

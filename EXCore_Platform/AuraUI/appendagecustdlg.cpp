// AppendageCustDlg.cpp : implementation file
//
#define D3D_OVERLOADS
#include "stdafx.h"
#include "..\EngineMainApp\resource.h"
#include <d3d9.h>
#include <afxtempl.h>
#include "AppendageCustDlg.h"
#include "direct.h"
#include "matrixarb.h"
#include "SkeletalAnimationDlg.h"
#include "CGlobalMaterialShaderLib.h"
#include "TransformationDlg.h"
#include "PathDialog.h"
#include "globalstatusvariables.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAppendageCustDlg dialog


CAppendageCustDlg::CAppendageCustDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAppendageCustDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAppendageCustDlg)
	m_miscInt = 0;
	m_name = _T("");
	m_boneAttached = 0;
	m_functionalityAttrib = 0;
	m_miscInt2 = 0;
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	m_locationOccupancyID = 0;
	m_skillUse = 0;
	m_abilityUseType = 0;
	m_animAccessVersion = 0;
	m_miscIntText = _T("");
	m_miscInt2Text = _T("");
	m_miscInt3 = 0;
	m_miscInt3Text = _T("");
	m_type = 0;
	m_dimCfgIndex = -1;
	m_dimCfgChangeTo = -1;
	//}}AFX_DATA_INIT
	m_renderStateObjectRef = NULL;
}


void CAppendageCustDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAppendageCustDlg)
	DDX_Control(pDX, IDC_COMBO_FunctionAttribute, m_cmboFunctionAttribute);
	DDX_Control(pDX, IDC_COMBO_BonesSel, m_bonesSel);
	DDX_Control(pDX, IDC_LIST_Appendages, m_appendageList);
	DDX_Text(pDX, IDC_EDIT_App_MiscInt, m_miscInt);
	DDX_Text(pDX, IDC_EDIT_AppName, m_name);
	DDX_Text(pDX, IDC_EDIT_BoneAttached, m_boneAttached);
	DDX_Text(pDX, IDC_EDIT_FunctionalityAttribute, m_functionalityAttrib);
	DDX_Text(pDX, IDC_EDIT_MiscInt2, m_miscInt2);
	DDX_Text(pDX, IDC_EDIT_POSITIONX, m_positionX);
	DDX_Text(pDX, IDC_EDIT_POSITIONY, m_positionY);
	DDX_Text(pDX, IDC_EDIT_POSITIONZ, m_positionZ);
	DDX_Text(pDX, IDC_EDIT_LocationOccupancyID, m_locationOccupancyID);
	DDX_Text(pDX, IDC_EDIT_SkillUse, m_skillUse);
	DDX_Text(pDX, IDC_EDIT_AbilityUseType, m_abilityUseType);
	DDX_Text(pDX, IDC_EDIT_AnimAccessVersion, m_animAccessVersion);
	DDX_Text(pDX, IDC_STATIC_miscInt, m_miscIntText);
	DDX_Text(pDX, IDC_STATIC_miscInt2, m_miscInt2Text);
	DDX_Text(pDX, IDC_EDIT_MiscInt3, m_miscInt3);
	DDX_Text(pDX, IDC_STATIC_miscInt3, m_miscInt3Text);
	DDX_Text(pDX, IDC_EDIT_TypeID, m_type);
	DDX_Text(pDX, IDC_EDIT_DimChangeIndex, m_dimCfgIndex);
	DDX_Text(pDX, IDC_EDIT_dimCfgChangeTo, m_dimCfgChangeTo);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_alwaysShowFirstPerson, m_alwaysShowFirstPersonCK);
}


BEGIN_MESSAGE_MAP(CAppendageCustDlg, CDialog)
	//{{AFX_MSG_MAP(CAppendageCustDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADDAppendage, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DeleteFirstPersonSkeleton, OnBUTTONDeleteFirstPersonSkeleton)
	ON_BN_CLICKED(IDC_BUTTON_DeleteThirdPersonSkeleton, OnBUTTONDeleteThirdPersonSkeleton)
	ON_BN_CLICKED(IDC_BUTTON_EditFirstPersonSkeleton, OnBUTTONEditFirstPersonSkeleton)
	ON_BN_CLICKED(IDC_BUTTON_EditThirdPersonSkeleton, OnBUTTONEditThirdPersonSkeleton)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceValues, OnBUTTONReplaceValues)
	ON_LBN_SELCHANGE(IDC_LIST_Appendages, OnSelchangeLISTAppendages)
	ON_CBN_SELCHANGE(IDC_COMBO_FunctionAttribute, OnSelchangeCOMBOFunctionAttribute)
	ON_BN_CLICKED(IDC_BUTTON_Export, OnBUTTONExport)
	ON_BN_CLICKED(IDC_BUTTON_Import, OnBUTTONImport)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_Transform, OnBnClickedButtonTransform)
	ON_BN_CLICKED(IDC_BUTTON_ImportKeepID, OnBnClickedButtonImportkeepid)
	ON_BN_CLICKED(IDC_BUTTON_ImportReplace, &CAppendageCustDlg::OnBnClickedButtonImportreplace)
	ON_BN_CLICKED(IDC_BUTTON_ImportCompleteFromDir, &CAppendageCustDlg::OnBnClickedButtonImportcompletefromdir)
	ON_BN_CLICKED(IDC_BUTTON_ImportReplaceCompleteFromDir, &CAppendageCustDlg::OnBnClickedButtonImportreplacecompletefromdir)
	ON_BN_CLICKED(IDC_BUTTON_ImportReplaceKeepCompleteFromDir, &CAppendageCustDlg::OnBnClickedButtonImportreplacekeepcompletefromdir)
	ON_BN_CLICKED(IDC_BUTTON_AddFirstPersonThroughDir, &CAppendageCustDlg::OnBnClickedButtonAddfirstpersonthroughdir)
	ON_BN_CLICKED(IDC_BUTTON_AddthirdPersonThroughDir, &CAppendageCustDlg::OnBnClickedButtonAddthirdpersonthroughdir)
	ON_BN_CLICKED(IDC_CHECK_alwaysShowFirstPerson, &CAppendageCustDlg::OnBnClickedCheckalwaysshowfirstperson)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppendageCustDlg message handlers

void CAppendageCustDlg::OnButtonAdd() 
{
	UpdateData(TRUE);
    if(!m_armedInvenListRef)
		m_armedInvenListRef = new CArmedInventoryList;
	
    CArmedInventoryObj * armedPtr = new CArmedInventoryObj(m_name);
    armedPtr->m_type = m_armedInvenListRef->GetCount();
	m_armedInvenListRef->AddTail(armedPtr);
    
	int bonesel = m_bonesSel.GetCurSel();
	if(bonesel ==-1){
       bonesel=0;
	}
	
	armedPtr->m_boneAttached               = bonesel;
	armedPtr->m_miscInt                    = m_miscInt;
	armedPtr->m_miscInt2                   = m_miscInt2;
	armedPtr->m_miscInt3                   = m_miscInt3;
	armedPtr->m_miscPosition.x             = m_positionX;
    armedPtr->m_miscPosition.y             = m_positionY;
	armedPtr->m_miscPosition.z             = m_positionZ;
	armedPtr->m_locationOccupancy          = m_locationOccupancyID;
	armedPtr->m_skillUse                   = m_skillUse;
	armedPtr->m_abilityTypeUse             = m_abilityUseType;
	armedPtr->m_animAccessVersion          = m_animAccessVersion;
	armedPtr->m_functionalityAttribute     = m_cmboFunctionAttribute.GetCurSel();

	CBoneObject * boneRef = m_focusMovPtr->List_skeletalObject->GetBoneByIndex(bonesel);
		if(boneRef)
		{
           armedPtr->m_boneMapping = boneRef->m_boneName;
		}

		m_armedInvenListRef->CompileDirectAccess();
	UpdateData(FALSE);
	InListAppendages(-1);
}

void CAppendageCustDlg::OnButtonDelete() 
{
	if(AfxMessageBox("You should never delete attachments, are your sure ?", MB_OKCANCEL, 0) != IDOK)
		return;

	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1)
	{
		MessageBox("must select a appendage");
		return;
	}

	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;

	CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
	armedPtr->SafeDelete();
	delete armedPtr;
	armedPtr=0;
	m_armedInvenListRef->RemoveAt(posLoc);
	m_armedInvenListRef->CompileDirectAccess();
	
	if(listIndex>-1)
		InListAppendages((listIndex-1));
}

void CAppendageCustDlg::OnBUTTONDeleteFirstPersonSkeleton() 
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1)
	{
       MessageBox("must select a appendage");
       return;
	}

    if(AfxMessageBox("Are you sure you want to delete this skeletal system ?", MB_OKCANCEL, 0) != IDOK)
		return;

	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
	if(armedPtr->m_firstPersonSystem)
	{
		armedPtr->m_firstPersonSystem->SafeDelete();
		delete armedPtr->m_firstPersonSystem;
		armedPtr->m_firstPersonSystem=0;
	}
}

 void CAppendageCustDlg::OnBUTTONDeleteThirdPersonSkeleton() 
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1)
	{
		MessageBox("must select a appendage");
		return;
	}

	if(AfxMessageBox("Are you sure you want to delete this skeletal system ?", MB_OKCANCEL, 0) != IDOK)
	return;


	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
	if(armedPtr->m_thirdPersonSystem)
	{
		armedPtr->m_thirdPersonSystem->SafeDelete();
		delete armedPtr->m_thirdPersonSystem;
		armedPtr->m_thirdPersonSystem=0;
	}
}

void CAppendageCustDlg::OnBUTTONEditFirstPersonSkeleton() 
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a appendage");
       return;
	}

	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
		if(!armedPtr->m_firstPersonSystem){//init bone system
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
				 armedPtr->m_firstPersonSystem = new CSkeletonObject();
			     armedPtr->m_firstPersonSystem->m_SkeletalName = armedPtr->m_name;
	             CString filenamePathandAll = opendialog.GetPathName();
				 CString baseName = opendialog.GetFileTitle();
	             armedPtr->m_firstPersonSystem->InitBoneSystem(filenamePathandAll,g_pd3dDeviceRef);
				 if(armedPtr->m_firstPersonSystem->MergeAsLod(baseName + "_LOD1.arb","_LOD1",g_pd3dDeviceRef))
				    if(armedPtr->m_firstPersonSystem->MergeAsLod(baseName + "_LOD2.arb","_LOD2",g_pd3dDeviceRef))
						if(armedPtr->m_firstPersonSystem->MergeAsLod(baseName + "_LOD3.arb","_LOD3",g_pd3dDeviceRef))
							if(armedPtr->m_firstPersonSystem->MergeAsLod(baseName + "_LOD4.arb","_LOD4",g_pd3dDeviceRef))
								if(armedPtr->m_firstPersonSystem->MergeAsLod(baseName + "_LOD5.arb","_LOD5",g_pd3dDeviceRef))
								{

								}
	             
				 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 if(armedPtr->m_firstPersonSystem)
				    gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_firstPersonSystem);
			}
			else{


	        SetCurrentDirectory(MainDir);
			return;
			}
            SetCurrentDirectory(MainDir);
		}//end init bone system
        //pull up edit dlg
        CSkeletalAnimationDlg dlg;
		dlg.m_renderStateObjectRef = m_renderStateObjectRef;
		dlg.m_globalShaderRef = m_cgGlobalShaderDB;
		dlg.m_skillDatabaseREF = m_skillDatabaseREF;
		dlg.m_elementDBREF = m_elementDBREF;
		dlg.GL_textureDataBase = GL_textureDataBase;
		dlg.m_skeletonReferencePtr = armedPtr->m_firstPersonSystem;
		dlg.directSoundRef = m_dirSndRef;
		dlg.DoModal();
}

void CAppendageCustDlg::OnBUTTONEditThirdPersonSkeleton() 
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a appendage");
       return;
	}

	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
		if(!armedPtr->m_thirdPersonSystem){//init bone system
            
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
				 armedPtr->m_thirdPersonSystem = new CSkeletonObject();
			     armedPtr->m_thirdPersonSystem->m_SkeletalName = armedPtr->m_name;
	             CString filenamePathandAll = opendialog.GetPathName();
	             armedPtr->m_thirdPersonSystem->InitBoneSystem(filenamePathandAll,g_pd3dDeviceRef);
	             //armedPtr->m_thirdPersonSystem->OptimizeVertexAssignment();
                 CString baseName = opendialog.GetFileTitle();
				 if(armedPtr->m_thirdPersonSystem->MergeAsLod(baseName + "_LOD1.arb","_LOD1",g_pd3dDeviceRef))
				    if(armedPtr->m_thirdPersonSystem->MergeAsLod(baseName + "_LOD2.arb","_LOD2",g_pd3dDeviceRef))
						if(armedPtr->m_thirdPersonSystem->MergeAsLod(baseName + "_LOD3.arb","_LOD3",g_pd3dDeviceRef))
							if(armedPtr->m_thirdPersonSystem->MergeAsLod(baseName + "_LOD4.arb","_LOD4",g_pd3dDeviceRef))
								if(armedPtr->m_thirdPersonSystem->MergeAsLod(baseName + "_LOD5.arb","_LOD5",g_pd3dDeviceRef))
								{

								}

				 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 
	    
		         if(armedPtr->m_thirdPersonSystem)
	                gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_thirdPersonSystem);
			}
			else{
	        SetCurrentDirectory(MainDir);
			return;
			}
            SetCurrentDirectory(MainDir);
		}//end init bone system
        //pull up edit dlg
        CSkeletalAnimationDlg dlg;
		dlg.m_renderStateObjectRef = m_renderStateObjectRef;
		dlg.m_globalShaderRef = m_cgGlobalShaderDB;
        dlg.m_skillDatabaseREF = m_skillDatabaseREF;
		dlg.m_elementDBREF = m_elementDBREF;
		dlg.GL_textureDataBase = GL_textureDataBase;
		dlg.m_skeletonReferencePtr = armedPtr->m_thirdPersonSystem;
		dlg.directSoundRef = m_dirSndRef;
		dlg.DoModal();
}

void CAppendageCustDlg::OnBUTTONReplaceValues() 
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a appendage");
       return;
	}
    UpdateData(TRUE);
	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
	    
		int bonesel = m_bonesSel.GetCurSel();
	    if(bonesel ==-1){
           bonesel=0;
		}
		armedPtr->m_name						= m_name;
		armedPtr->m_boneAttached				= bonesel;
	    armedPtr->m_miscInt						= m_miscInt;
	    armedPtr->m_miscInt2					= m_miscInt2;
		armedPtr->m_miscInt3					= m_miscInt3;
	    armedPtr->m_miscPosition.x				= m_positionX;
        armedPtr->m_miscPosition.y				= m_positionY;
	    armedPtr->m_miscPosition.z				= m_positionZ;
		armedPtr->m_locationOccupancy			= m_locationOccupancyID;
		armedPtr->m_skillUse					= m_skillUse;
		armedPtr->m_abilityTypeUse				= m_abilityUseType;
		armedPtr->m_animAccessVersion			= m_animAccessVersion;
		armedPtr->m_type						= m_type;
		armedPtr->m_alwaysShowFirstPerson		= m_alwaysShowFirstPersonCK.GetCheck();

		
		CBoneObject * boneRef = m_focusMovPtr->List_skeletalObject->GetBoneByIndex(bonesel);
		if(boneRef)
		{
           armedPtr->m_boneMapping = boneRef->m_boneName;
		}

		armedPtr->m_functionalityAttribute     = m_cmboFunctionAttribute.GetCurSel();
	UpdateData(FALSE);
	InListAppendages(listIndex);
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
BOOL CAppendageCustDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListAppendages(-1);
	BonesInList();

    UpdateData(TRUE);
	m_cmboFunctionAttribute.SetCurSel(0);
	m_miscIntText  = "Not Used";
	m_miscInt2Text = "Not Used";


    

    UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CAppendageCustDlg::InListAppendages(int selHighLight) 
{
   UpdateData(TRUE);
   m_appendageList.ResetContent();
   if(m_armedInvenListRef)
	   for(POSITION posLoc = m_armedInvenListRef->GetHeadPosition();posLoc != NULL;)
	   {
           CArmedInventoryObj * armedPtr = (CArmedInventoryObj *)m_armedInvenListRef->GetNext(posLoc);
           m_appendageList.AddString(armedPtr->m_name);
	   }
   UpdateData(FALSE);
   if(selHighLight!=-1)
   m_appendageList.SetCurSel(selHighLight);
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CAppendageCustDlg::OnSelchangeLISTAppendages() 
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       return;
	}
    UpdateData(TRUE);
	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
	    m_name = armedPtr->m_name;
		m_bonesSel.SetCurSel(armedPtr->m_boneAttached);
		m_cmboFunctionAttribute.SetCurSel(armedPtr->m_functionalityAttribute);
	    m_miscInt					= armedPtr->m_miscInt;
	    m_miscInt2					= armedPtr->m_miscInt2;
		m_miscInt3					= armedPtr->m_miscInt3;
	    m_positionX					= armedPtr->m_miscPosition.x;
        m_positionY					= armedPtr->m_miscPosition.y;
	    m_positionZ					= armedPtr->m_miscPosition.z;
		m_locationOccupancyID		= armedPtr->m_locationOccupancy;
		m_skillUse					= armedPtr->m_skillUse;
		m_abilityUseType			= armedPtr->m_abilityTypeUse;
		m_animAccessVersion			= armedPtr->m_animAccessVersion;
		m_type						= armedPtr->m_type;

		m_alwaysShowFirstPersonCK.SetCheck(armedPtr->m_alwaysShowFirstPerson);

		if(armedPtr->m_boneMapping.GetLength()>0)
		{
		   m_bonesSel.SetCurSel(m_focusMovPtr->List_skeletalObject->GetBoneIndexByName(armedPtr->m_boneMapping));
		}


        if(armedPtr->m_functionalityAttribute == 0){
	       m_miscIntText  = "Not Used";
	       m_miscInt2Text = "Not Used";
		   m_miscInt3Text = "Not Used";
		}
	    if(armedPtr->m_functionalityAttribute == 1){
	       m_miscIntText  = "Fire Rate Delay Ms";
           m_miscInt2Text = "Missile DB Index  -1=none";
	       m_miscInt3Text = "Desired Distance if DB is -1";
		}
	    if(armedPtr->m_functionalityAttribute == 2){
	       m_miscIntText  = "Not Used";
	       m_miscInt2Text = "Not Used";
		   m_miscInt3Text = "Not Used";
		}
		if(armedPtr->m_functionalityAttribute == 3){
	       m_miscIntText  = "Fuel Capacity";
	       m_miscInt2Text = "Fuel Use Rate";
	       m_miscInt3Text = "Not Used";
		}

	UpdateData(FALSE);
	
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CAppendageCustDlg::BonesInList()
{
UpdateData(TRUE);
  m_bonesSel.ResetContent();
  if(m_focusMovPtr->List_skeletalObject)
  if(m_focusMovPtr->List_skeletalObject->m_bonesList)
  for(POSITION posLoc = m_focusMovPtr->List_skeletalObject->m_bonesList->GetHeadPosition();posLoc != NULL;)
  {
     CBoneObject * bonePtr = (CBoneObject *) m_focusMovPtr->List_skeletalObject->m_bonesList->GetNext(posLoc);
     m_bonesSel.AddString(bonePtr->m_boneName);
  }
UpdateData(FALSE);
}




void CAppendageCustDlg::OnSelchangeCOMBOFunctionAttribute() 
{
	
	int ref = m_cmboFunctionAttribute.GetCurSel();
	if(ref == -1)
	   return;
    UpdateData(TRUE);
	if(ref == 0){
	m_miscIntText  = "Not Used";
	m_miscInt2Text = "Not Used";
	m_miscInt3Text = "Not Used";
	}
	if(ref == 1){
	m_miscIntText  = "Fire Rate Delay Ms";
	m_miscInt2Text = "Missile DB Index  -1=none";
	m_miscInt3Text = "Desired Distance if DB is -1";
	}
	if(ref == 2){
	m_miscIntText  = "Not Used";
	m_miscInt2Text = "Not Used";
	m_miscInt3Text = "Not Used";
	}
	if(ref == 3){
	m_miscIntText  = "Fuel Capacity";
	m_miscInt2Text = "Fuel Use Rate";
	m_miscInt3Text = "Not Used";
	}
	UpdateData(FALSE);
	
}

void CAppendageCustDlg::OnBUTTONExport() 
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       return;
	}
    UpdateData(TRUE);
	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "Attachment Modules Files|*.ATM|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "ATM", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << armedPtr;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CAppendageCustDlg::OnBUTTONImport() 
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

   static char BASED_CODE filter[] = "Attachment Module Files|*.ATM|All Files (*.*)|*.*||";
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
	   CArmedInventoryObj * armedPtr = NULL;
	   the_in_Archive >> armedPtr;
	   the_in_Archive.Close();
       fl.Close();

	   armedPtr->m_type = m_armedInvenListRef->GetFreeID();
	   m_armedInvenListRef->AddTail(armedPtr);
	   InListAppendages(m_armedInvenListRef->GetCount()-1);

	   GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 
	    if(armedPtr->m_firstPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_firstPersonSystem);
		if(armedPtr->m_thirdPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_thirdPersonSystem);
	 }
     SetCurrentDirectory(MainDir);
	 
}

void CAppendageCustDlg::OnBnClickedButtonTransform()
{
	MatrixARB matrixTools;
    int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       return;
	}
    UpdateData(TRUE);
	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
	
		if(armedPtr->m_thirdPersonSystem)
		{
           CTransformationDlg dlg;
		   dlg.m_rotX = 0.0f;
		   dlg.m_rotY = 0.0f;
		   dlg.m_rotZ = 0.0f;
		   dlg.m_scaleX = 1.0f;
		   dlg.m_scaleY = 1.0f;
		   dlg.m_scaleZ = 1.0f;
		   dlg.m_transX = 0.0f;
		   dlg.m_transY = 0.0f;
		   dlg.m_transZ = 0.0f;

		   dlg.DoModal();

		   float percentage = .9f;
		   D3DMATRIX transform;
		   ZeroMemory(&transform,sizeof(D3DMATRIX));
           transform._11 = dlg.m_scaleX;
		   transform._22 = dlg.m_scaleY;
		   transform._33 = dlg.m_scaleZ;

		   transform._44 = 1.0f;

		   transform._41 = dlg.m_transX;
		   transform._42 = dlg.m_transY;
		   transform._43 = dlg.m_transZ;
           BOOL rotation = FALSE;
		   if(dlg.m_rotX != 0.0f || dlg.m_rotY != 0.0f || dlg.m_rotZ != 0.0f)
		   {
              matrixTools.Rotate ( dlg.m_rotX, dlg.m_rotY, dlg.m_rotZ, transform.m);
              rotation = TRUE;
		   }

		     armedPtr->m_thirdPersonSystem->TransformSystem(transform,g_pd3dDeviceRef,rotation);
		}

}

void CAppendageCustDlg::OnBnClickedButtonImportkeepid()
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

   static char BASED_CODE filter[] = "Attachment Module Files|*.ATM|All Files (*.*)|*.*||";
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
	   CArmedInventoryObj * armedPtr = NULL;
	   the_in_Archive >> armedPtr;
	   the_in_Archive.Close();
       fl.Close();

	   GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 
	    if(armedPtr->m_firstPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_firstPersonSystem);
		if(armedPtr->m_thirdPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_thirdPersonSystem);
	   
	   if(m_armedInvenListRef->GetByType(armedPtr->m_type))
	      AfxMessageBox("ID already exists-cannot add");
	   else
	   {
	       m_armedInvenListRef->AddTail(armedPtr);
	       InListAppendages(m_armedInvenListRef->GetCount()-1);
	   }
	 }
     SetCurrentDirectory(MainDir);
}

void CAppendageCustDlg::OnBnClickedButtonImportreplace()
{
    int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a appendage");
       return;
	}

	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	CArmedInventoryObj * armedPtrExisting = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);


	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "Attachment Module Files|*.ATM|All Files (*.*)|*.*||";
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
	   CArmedInventoryObj * armedPtr = NULL;
	   the_in_Archive >> armedPtr;
	   the_in_Archive.Close();
       fl.Close();

	   armedPtr->m_type = armedPtrExisting->m_type;

	   GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 
	    if(armedPtr->m_firstPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_firstPersonSystem);
		if(armedPtr->m_thirdPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_thirdPersonSystem);
	  
       m_armedInvenListRef->InsertAfter(posLoc,armedPtr);
	   armedPtrExisting->SafeDelete();
	   delete armedPtrExisting;
	   armedPtrExisting = 0;
	   m_armedInvenListRef->RemoveAt(posLoc);
       InListAppendages(listIndex);
	 }
     SetCurrentDirectory(MainDir);
}

void CAppendageCustDlg::OnBnClickedButtonImportcompletefromdir()
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
    char buffer[_MAX_PATH];
    _getcwd( buffer, _MAX_PATH );
    MainDir = CurDir = buffer;
    //end use gamefiles folder for organization
    
    CString strYourCaption(_T("Your caption here..."));
    CString strYourTitle(_T("Your tilte here..."));

    CPathDialog dlg("Directory Select", "This Title", MainDir);

    if (dlg.DoModal()==IDOK)
    {
        UpdateData(TRUE);

        CString baseDir = dlg.GetPathName() + "\\";
		CString firstPersonPath = baseDir + "FirstPerson\\";
		CString firstPersonPathFile = firstPersonPath + "model.arb";
		CString thirdPersonPath = baseDir + "ThirdPerson\\";
        CString thirdPersonPathFile = thirdPersonPath + "model.arb";
        
		if(!m_armedInvenListRef)
			m_armedInvenListRef = new CArmedInventoryList;
		
		CArmedInventoryObj * armedPtr = new CArmedInventoryObj(m_name);
		armedPtr->m_type = m_armedInvenListRef->GetCount();
		
		armedPtr->m_firstPersonSystem = new CSkeletonObject();
	    armedPtr->m_firstPersonSystem->m_SkeletalName = armedPtr->m_name;
        BOOL resBool = armedPtr->m_firstPersonSystem->LoadFullSkeletalFromDirectory(firstPersonPathFile,firstPersonPath,g_pd3dDeviceRef);
        if(!resBool)
		{
            armedPtr->m_firstPersonSystem->SafeDelete();
			delete armedPtr->m_firstPersonSystem;
            armedPtr->m_firstPersonSystem = 0;
		}
		armedPtr->m_thirdPersonSystem = new CSkeletonObject();
	    armedPtr->m_thirdPersonSystem->m_SkeletalName = armedPtr->m_name;
		resBool = armedPtr->m_thirdPersonSystem->LoadFullSkeletalFromDirectory(thirdPersonPathFile,thirdPersonPath,g_pd3dDeviceRef);
		if(!resBool)
		{
            armedPtr->m_thirdPersonSystem->SafeDelete();
			delete armedPtr->m_thirdPersonSystem;
            armedPtr->m_thirdPersonSystem = 0;
		}

		m_armedInvenListRef->AddTail(armedPtr);	    
		int bonesel = m_bonesSel.GetCurSel();
		if(bonesel ==-1){
		   bonesel=0;
		}
		
		armedPtr->m_boneAttached               = bonesel;
		armedPtr->m_miscInt                    = m_miscInt;
		armedPtr->m_miscInt2                   = m_miscInt2;
		armedPtr->m_miscInt3                   = m_miscInt3;
		armedPtr->m_miscPosition.x             = m_positionX;
		armedPtr->m_miscPosition.y             = m_positionY;
		armedPtr->m_miscPosition.z             = m_positionZ;
		armedPtr->m_locationOccupancy          = m_locationOccupancyID;
		armedPtr->m_skillUse                   = m_skillUse;
		armedPtr->m_abilityTypeUse             = m_abilityUseType;
		armedPtr->m_animAccessVersion          = m_animAccessVersion;
		armedPtr->m_functionalityAttribute     = m_cmboFunctionAttribute.GetCurSel();
		

		CBoneObject * boneRef = m_focusMovPtr->List_skeletalObject->GetBoneByIndex(bonesel);
			if(boneRef)
			{
			   armedPtr->m_boneMapping = boneRef->m_boneName;
			}

			m_armedInvenListRef->CompileDirectAccess();
		UpdateData(FALSE);
		InListAppendages(m_armedInvenListRef->GetCount()-1);

		GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 
	    if(armedPtr->m_firstPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_firstPersonSystem);
		if(armedPtr->m_thirdPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_thirdPersonSystem);
    }

	


	SetCurrentDirectory(MainDir);
}

void CAppendageCustDlg::OnBnClickedButtonImportreplacecompletefromdir()
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a appendage");
       return;
	}
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
    char buffer[_MAX_PATH];
    _getcwd( buffer, _MAX_PATH );
    MainDir = CurDir = buffer;
    //end use gamefiles folder for organization
    
    CString strYourCaption(_T("Your caption here..."));
    CString strYourTitle(_T("Your tilte here..."));

    CPathDialog dlg("Directory Select", "This Title", MainDir);

    if (dlg.DoModal()==IDOK)
    {
        UpdateData(TRUE);

        CString baseDir = dlg.GetPathName() + "\\";
		CString firstPersonPath = baseDir + "FirstPerson\\";
		CString firstPersonPathFile = firstPersonPath + "model.arb";
		CString thirdPersonPath = baseDir + "ThirdPerson\\";
        CString thirdPersonPathFile = thirdPersonPath + "model.arb";
        
		if(!m_armedInvenListRef)
			m_armedInvenListRef = new CArmedInventoryList;
		
		//CArmedInventoryObj * armedPtr = new CArmedInventoryObj(m_name);
		//armedPtr->m_type = m_armedInvenListRef->GetCount();
		
	    POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	    if(!posLoc)
		    return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
		
		armedPtr->m_firstPersonSystem = new CSkeletonObject();
	    armedPtr->m_firstPersonSystem->m_SkeletalName = m_name;
		armedPtr->m_name = m_name;
        BOOL resBool = armedPtr->m_firstPersonSystem->LoadFullSkeletalFromDirectory(firstPersonPathFile,firstPersonPath,g_pd3dDeviceRef);
        if(!resBool)
		{
            armedPtr->m_firstPersonSystem->SafeDelete();
			delete armedPtr->m_firstPersonSystem;
            armedPtr->m_firstPersonSystem = 0;
		}
		armedPtr->m_thirdPersonSystem = new CSkeletonObject();
	    armedPtr->m_thirdPersonSystem->m_SkeletalName = armedPtr->m_name;
		resBool = armedPtr->m_thirdPersonSystem->LoadFullSkeletalFromDirectory(thirdPersonPathFile,thirdPersonPath,g_pd3dDeviceRef);
		if(!resBool)
		{
            armedPtr->m_thirdPersonSystem->SafeDelete();
			delete armedPtr->m_thirdPersonSystem;
            armedPtr->m_thirdPersonSystem = 0;
		}

		//m_armedInvenListRef->AddTail(armedPtr);	    
		int bonesel = m_bonesSel.GetCurSel();
		if(bonesel ==-1){
		   bonesel=0;
		}
		
		armedPtr->m_boneAttached               = bonesel;
		armedPtr->m_miscInt                    = m_miscInt;
		armedPtr->m_miscInt2                   = m_miscInt2;
		armedPtr->m_miscInt3                   = m_miscInt3;
		armedPtr->m_miscPosition.x             = m_positionX;
		armedPtr->m_miscPosition.y             = m_positionY;
		armedPtr->m_miscPosition.z             = m_positionZ;
		armedPtr->m_locationOccupancy          = m_locationOccupancyID;
		armedPtr->m_skillUse                   = m_skillUse;
		armedPtr->m_abilityTypeUse             = m_abilityUseType;
		armedPtr->m_animAccessVersion          = m_animAccessVersion;
		armedPtr->m_functionalityAttribute     = m_cmboFunctionAttribute.GetCurSel();

		CBoneObject * boneRef = m_focusMovPtr->List_skeletalObject->GetBoneByIndex(bonesel);
			if(boneRef)
			{
			   armedPtr->m_boneMapping = boneRef->m_boneName;
			}

			m_armedInvenListRef->CompileDirectAccess();
		UpdateData(FALSE);
		InListAppendages(listIndex);

		GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 
	    if(armedPtr->m_firstPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_firstPersonSystem);
		if(armedPtr->m_thirdPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_thirdPersonSystem);
    }



	SetCurrentDirectory(MainDir);
}

void CAppendageCustDlg::OnBnClickedButtonImportreplacekeepcompletefromdir()
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a appendage");
       return;
	}
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
    char buffer[_MAX_PATH];
    _getcwd( buffer, _MAX_PATH );
    MainDir = CurDir = buffer;
    //end use gamefiles folder for organization
    
    CString strYourCaption(_T("Your caption here..."));
    CString strYourTitle(_T("Your tilte here..."));

    CPathDialog dlg("Directory Select", "This Title", MainDir);

    if (dlg.DoModal()==IDOK)
    {
        UpdateData(TRUE);

        CString baseDir = dlg.GetPathName() + "\\";
		CString firstPersonPath = baseDir + "FirstPerson\\";
		CString firstPersonPathFile = firstPersonPath + "model.arb";
		CString thirdPersonPath = baseDir + "ThirdPerson\\";
        CString thirdPersonPathFile = thirdPersonPath + "model.arb";
        
		if(!m_armedInvenListRef)
			m_armedInvenListRef = new CArmedInventoryList;
		
		//CArmedInventoryObj * armedPtr = new CArmedInventoryObj(m_name);
		//armedPtr->m_type = m_armedInvenListRef->GetCount();
		
	    POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	    if(!posLoc)
		    return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);
        CSkeletonObject * oldSkeletalFirst = NULL;
		if(armedPtr->m_firstPersonSystem)
		{
           armedPtr->m_firstPersonSystem->Clone(&oldSkeletalFirst,g_pd3dDeviceRef,0,1);		
           armedPtr->m_firstPersonSystem->SafeDelete();
		   delete armedPtr->m_firstPersonSystem;
		   armedPtr->m_firstPersonSystem = 0;
		}
  

		armedPtr->m_firstPersonSystem = new CSkeletonObject();
	    armedPtr->m_firstPersonSystem->m_SkeletalName = m_name;
		armedPtr->m_name = m_name;
        BOOL resBool = armedPtr->m_firstPersonSystem->LoadFullSkeletalFromDirectory(firstPersonPathFile,firstPersonPath,g_pd3dDeviceRef);
        if(!resBool)
		{
            armedPtr->m_firstPersonSystem->SafeDelete();
			delete armedPtr->m_firstPersonSystem;
            armedPtr->m_firstPersonSystem = 0;
		}
		else if(oldSkeletalFirst)
		{//add additional info
			if(oldSkeletalFirst->m_damageSphereList)
               oldSkeletalFirst->m_damageSphereList->Clone(&armedPtr->m_firstPersonSystem->m_damageSphereList);
            
			if(oldSkeletalFirst->m_fireQueList)
			   oldSkeletalFirst->m_fireQueList->Clone(&armedPtr->m_firstPersonSystem->m_fireQueList);
            
			if(oldSkeletalFirst->m_particleEmiterList)
			   oldSkeletalFirst->m_particleEmiterList->Clone(&armedPtr->m_firstPersonSystem->m_particleEmiterList);
			
			if(oldSkeletalFirst->m_sphereBodyCollisionSystem)
			   oldSkeletalFirst->m_sphereBodyCollisionSystem->Clone(&armedPtr->m_firstPersonSystem->m_sphereBodyCollisionSystem);
			
			if(oldSkeletalFirst->m_sphereLocationalDamageList)
			   oldSkeletalFirst->m_sphereLocationalDamageList->Clone(&armedPtr->m_firstPersonSystem->m_sphereLocationalDamageList);
			
   
			//try to remap sound ranges
			if(oldSkeletalFirst->m_bonesList)
			if(armedPtr->m_firstPersonSystem->m_bonesList)
			for(POSITION bnPosLoc = oldSkeletalFirst->m_bonesList->GetHeadPosition();bnPosLoc != NULL;)
	        {
                CBoneObject * bone = (CBoneObject *)oldSkeletalFirst->m_bonesList->GetNext(bnPosLoc);
				if(bone->m_animationList)
				for(POSITION animPos = bone->m_animationList->GetHeadPosition();animPos != NULL;)
				{
					CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetNext(animPos);
                    if(!animPtr->m_soundRanges)
						continue;
					BOOL locatedSoundRange = FALSE;
					for(POSITION bnPosLoc = armedPtr->m_firstPersonSystem->m_bonesList->GetHeadPosition();bnPosLoc != NULL;)
					{
						CBoneObject * boneTarget = (CBoneObject *)armedPtr->m_firstPersonSystem->m_bonesList->GetNext(bnPosLoc);
						
						if(boneTarget->m_animationList)
						for(POSITION animPos = boneTarget->m_animationList->GetHeadPosition();animPos != NULL;)
						{
							CBoneAnimationObject * animPtrTarget = (CBoneAnimationObject *)boneTarget->m_animationList->GetNext(animPos);
							if(animPtrTarget->m_type == animPtr->m_type)
							   if(animPtrTarget->m_version == animPtr->m_version)
							   {
							      animPtr->m_soundRanges->Clone(&animPtrTarget->m_soundRanges);
								  locatedSoundRange = TRUE;
								  break;
							   }
						}
						if(locatedSoundRange)
						   break;
					}
					if(!locatedSoundRange)
					{
					   POSITION bnPosLoc = armedPtr->m_firstPersonSystem->m_bonesList->GetHeadPosition();
					   if(bnPosLoc)
					   {
						CBoneObject * boneTarget = (CBoneObject *)armedPtr->m_firstPersonSystem->m_bonesList->GetAt(bnPosLoc);
						if(!boneTarget->m_animationList)
							continue;
                        POSITION animPos = boneTarget->m_animationList->GetHeadPosition();
						if(animPos)
						{
							CBoneAnimationObject * animPtrTarget = (CBoneAnimationObject *)boneTarget->m_animationList->GetAt(animPos);
							animPtr->m_soundRanges->Clone(&animPtrTarget->m_soundRanges);
						}
					   }
					}
				}
			}//end remap sound ranges
		}//end add additional info
		if(oldSkeletalFirst)
		{
           oldSkeletalFirst->SafeDelete();
		   delete oldSkeletalFirst;
		   oldSkeletalFirst = 0;
		}

		CSkeletonObject * oldSkeletalThird = NULL;
		if(armedPtr->m_thirdPersonSystem)
		{
           armedPtr->m_thirdPersonSystem->Clone(&oldSkeletalThird,g_pd3dDeviceRef,0,1);
		   armedPtr->m_thirdPersonSystem->SafeDelete();
		   delete armedPtr->m_thirdPersonSystem;
		   armedPtr->m_thirdPersonSystem = 0;
		}

		armedPtr->m_thirdPersonSystem = new CSkeletonObject();
	    armedPtr->m_thirdPersonSystem->m_SkeletalName = armedPtr->m_name;
		resBool = armedPtr->m_thirdPersonSystem->LoadFullSkeletalFromDirectory(thirdPersonPathFile,thirdPersonPath,g_pd3dDeviceRef);
		if(!resBool)
		{
            armedPtr->m_thirdPersonSystem->SafeDelete();
			delete armedPtr->m_thirdPersonSystem;
            armedPtr->m_thirdPersonSystem = 0;
		}
		else if(oldSkeletalThird)
		{//add additional info
			if(oldSkeletalThird->m_damageSphereList)
               oldSkeletalThird->m_damageSphereList->Clone(&armedPtr->m_thirdPersonSystem->m_damageSphereList);
            
			if(oldSkeletalThird->m_fireQueList)
			   oldSkeletalThird->m_fireQueList->Clone(&armedPtr->m_thirdPersonSystem->m_fireQueList);
            
			if(oldSkeletalThird->m_particleEmiterList)
			   oldSkeletalThird->m_particleEmiterList->Clone(&armedPtr->m_thirdPersonSystem->m_particleEmiterList);
			
			if(oldSkeletalThird->m_sphereBodyCollisionSystem)
			   oldSkeletalThird->m_sphereBodyCollisionSystem->Clone(&armedPtr->m_thirdPersonSystem->m_sphereBodyCollisionSystem);
			
			if(oldSkeletalThird->m_sphereLocationalDamageList)
			   oldSkeletalThird->m_sphereLocationalDamageList->Clone(&armedPtr->m_thirdPersonSystem->m_sphereLocationalDamageList);
			
   
			//try to remap sound ranges
			if(oldSkeletalThird->m_bonesList)
			if(armedPtr->m_thirdPersonSystem->m_bonesList)
			for(POSITION bnPosLoc = oldSkeletalThird->m_bonesList->GetHeadPosition();bnPosLoc != NULL;)
	        {
                CBoneObject * bone = (CBoneObject *)oldSkeletalThird->m_bonesList->GetNext(bnPosLoc);
				if(bone->m_animationList)
				for(POSITION animPos = bone->m_animationList->GetHeadPosition();animPos != NULL;)
				{
					CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetNext(animPos);
                    if(!animPtr->m_soundRanges)
						continue;
					BOOL locatedSoundRange = FALSE;
					for(POSITION bnPosLoc = armedPtr->m_thirdPersonSystem->m_bonesList->GetHeadPosition();bnPosLoc != NULL;)
					{
						CBoneObject * boneTarget = (CBoneObject *)armedPtr->m_thirdPersonSystem->m_bonesList->GetNext(bnPosLoc);
						
						if(boneTarget->m_animationList)
						for(POSITION animPos = boneTarget->m_animationList->GetHeadPosition();animPos != NULL;)
						{
							CBoneAnimationObject * animPtrTarget = (CBoneAnimationObject *)boneTarget->m_animationList->GetNext(animPos);
							if(animPtrTarget->m_type == animPtr->m_type)
							   if(animPtrTarget->m_version == animPtr->m_version)
							   {
							      animPtr->m_soundRanges->Clone(&animPtrTarget->m_soundRanges);
								  locatedSoundRange = TRUE;
								  break;
							   }
						}
						if(locatedSoundRange)
						   break;
					}
					if(!locatedSoundRange)
					{
					   POSITION bnPosLoc = armedPtr->m_thirdPersonSystem->m_bonesList->GetHeadPosition();
					   if(bnPosLoc)
					   {
						CBoneObject * boneTarget = (CBoneObject *)armedPtr->m_thirdPersonSystem->m_bonesList->GetAt(bnPosLoc);
						if(!boneTarget->m_animationList)
							continue;
                        POSITION animPos = boneTarget->m_animationList->GetHeadPosition();
						if(animPos)
						{
							CBoneAnimationObject * animPtrTarget = (CBoneAnimationObject *)boneTarget->m_animationList->GetAt(animPos);
							animPtr->m_soundRanges->Clone(&animPtrTarget->m_soundRanges);
						}
					   }
					}
				}
			}//end remap sound ranges
		}//end add additional info
		if(oldSkeletalThird)
		{
           oldSkeletalThird->SafeDelete();
		   delete oldSkeletalThird;
		   oldSkeletalThird = 0;
		}

		//m_armedInvenListRef->AddTail(armedPtr);	    
		int bonesel = m_bonesSel.GetCurSel();
		if(bonesel ==-1){
		   bonesel=0;
		}
		
		armedPtr->m_boneAttached               = bonesel;
		armedPtr->m_miscInt                    = m_miscInt;
		armedPtr->m_miscInt2                   = m_miscInt2;
		armedPtr->m_miscInt3                   = m_miscInt3;
		armedPtr->m_miscPosition.x             = m_positionX;
		armedPtr->m_miscPosition.y             = m_positionY;
		armedPtr->m_miscPosition.z             = m_positionZ;
		armedPtr->m_locationOccupancy          = m_locationOccupancyID;
		armedPtr->m_skillUse                   = m_skillUse;
		armedPtr->m_abilityTypeUse             = m_abilityUseType;
		armedPtr->m_animAccessVersion          = m_animAccessVersion;
		armedPtr->m_functionalityAttribute     = m_cmboFunctionAttribute.GetCurSel();

		CBoneObject * boneRef = m_focusMovPtr->List_skeletalObject->GetBoneByIndex(bonesel);
			if(boneRef)
			{
			   armedPtr->m_boneMapping = boneRef->m_boneName;
			}

			m_armedInvenListRef->CompileDirectAccess();
		UpdateData(FALSE);
		InListAppendages(listIndex);

		GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 
	    if(armedPtr->m_firstPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_firstPersonSystem);
		if(armedPtr->m_thirdPersonSystem)
	       gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_thirdPersonSystem);
    }



	SetCurrentDirectory(MainDir);
}

void CAppendageCustDlg::OnBnClickedButtonAddfirstpersonthroughdir()
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a appendage");
       return;
	}

	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);

		
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
                    CSkeletonObject * oldSkeletalFirst = NULL;
					if(armedPtr->m_firstPersonSystem)
					{
					   armedPtr->m_firstPersonSystem->Clone(&oldSkeletalFirst,g_pd3dDeviceRef,0,1);		
					   armedPtr->m_firstPersonSystem->SafeDelete();
					   delete armedPtr->m_firstPersonSystem;
					   armedPtr->m_firstPersonSystem = 0;
					}

				    _getcwd( buffer, _MAX_PATH );
					CString plainDir = buffer;
					plainDir = plainDir + "\\";
	                CString filenamePathandAll = opendialog.GetPathName();
	                armedPtr->m_firstPersonSystem = new CSkeletonObject();
					armedPtr->m_firstPersonSystem->m_SkeletalName = m_name;
					armedPtr->m_name = m_name;
					BOOL resBool = armedPtr->m_firstPersonSystem->LoadFullSkeletalFromDirectory(filenamePathandAll,plainDir,g_pd3dDeviceRef);
					if(!resBool)
					{
						armedPtr->m_firstPersonSystem->SafeDelete();
						delete armedPtr->m_firstPersonSystem;
						armedPtr->m_firstPersonSystem = 0;
					}
					else if(oldSkeletalFirst)
					{//add additional info
						if(oldSkeletalFirst->m_damageSphereList)
						   oldSkeletalFirst->m_damageSphereList->Clone(&armedPtr->m_firstPersonSystem->m_damageSphereList);
			            
						if(oldSkeletalFirst->m_fireQueList)
						   oldSkeletalFirst->m_fireQueList->Clone(&armedPtr->m_firstPersonSystem->m_fireQueList);
			            
						if(oldSkeletalFirst->m_particleEmiterList)
						   oldSkeletalFirst->m_particleEmiterList->Clone(&armedPtr->m_firstPersonSystem->m_particleEmiterList);
						
						if(oldSkeletalFirst->m_sphereBodyCollisionSystem)
						   oldSkeletalFirst->m_sphereBodyCollisionSystem->Clone(&armedPtr->m_firstPersonSystem->m_sphereBodyCollisionSystem);
						
						if(oldSkeletalFirst->m_sphereLocationalDamageList)
						   oldSkeletalFirst->m_sphereLocationalDamageList->Clone(&armedPtr->m_firstPersonSystem->m_sphereLocationalDamageList);
						
			   
						//try to remap sound ranges
						if(oldSkeletalFirst->m_bonesList)
						if(armedPtr->m_firstPersonSystem->m_bonesList)
						for(POSITION bnPosLoc = oldSkeletalFirst->m_bonesList->GetHeadPosition();bnPosLoc != NULL;)
						{
							CBoneObject * bone = (CBoneObject *)oldSkeletalFirst->m_bonesList->GetNext(bnPosLoc);
							if(bone->m_animationList)
							for(POSITION animPos = bone->m_animationList->GetHeadPosition();animPos != NULL;)
							{
								CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetNext(animPos);
								if(!animPtr->m_soundRanges)
									continue;
								BOOL locatedSoundRange = FALSE;
								for(POSITION bnPosLoc = armedPtr->m_firstPersonSystem->m_bonesList->GetHeadPosition();bnPosLoc != NULL;)
								{
									CBoneObject * boneTarget = (CBoneObject *)armedPtr->m_firstPersonSystem->m_bonesList->GetNext(bnPosLoc);
									
									if(boneTarget->m_animationList)
									for(POSITION animPos = boneTarget->m_animationList->GetHeadPosition();animPos != NULL;)
									{
										CBoneAnimationObject * animPtrTarget = (CBoneAnimationObject *)boneTarget->m_animationList->GetNext(animPos);
										if(animPtrTarget->m_type == animPtr->m_type)
										   if(animPtrTarget->m_version == animPtr->m_version)
										   {
											  animPtr->m_soundRanges->Clone(&animPtrTarget->m_soundRanges);
											  locatedSoundRange = TRUE;
											  break;
										   }
									}
									if(locatedSoundRange)
									   break;
								}
								if(!locatedSoundRange)
								{
								   POSITION bnPosLoc = armedPtr->m_firstPersonSystem->m_bonesList->GetHeadPosition();
								   if(bnPosLoc)
								   {
									CBoneObject * boneTarget = (CBoneObject *)armedPtr->m_firstPersonSystem->m_bonesList->GetAt(bnPosLoc);
									POSITION animPos = boneTarget->m_animationList->GetHeadPosition();
									if(animPos)
									{
										CBoneAnimationObject * animPtrTarget = (CBoneAnimationObject *)boneTarget->m_animationList->GetAt(animPos);
										animPtr->m_soundRanges->Clone(&animPtrTarget->m_soundRanges);
									}
								   }
								}
							}
						}//end remap sound ranges
					}//end add additional info
					if(oldSkeletalFirst)
					{
					   oldSkeletalFirst->SafeDelete();
					   delete oldSkeletalFirst;
					   oldSkeletalFirst = 0;
					}
					GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 
	                if(armedPtr->m_firstPersonSystem)
	                   gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_firstPersonSystem);
		
			}
			else{
	        SetCurrentDirectory(MainDir);
			return;
			}
            SetCurrentDirectory(MainDir);

}

void CAppendageCustDlg::OnBnClickedButtonAddthirdpersonthroughdir()
{
	int listIndex = m_appendageList.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a appendage");
       return;
	}

	POSITION posLoc = m_armedInvenListRef->FindIndex(listIndex);
	if(!posLoc)
		return;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) m_armedInvenListRef->GetAt(posLoc);

		
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
                    CSkeletonObject * oldSkeletalThird = NULL;
					if(armedPtr->m_thirdPersonSystem)
					{
					   armedPtr->m_thirdPersonSystem->Clone(&oldSkeletalThird,g_pd3dDeviceRef,0,1);		
					   armedPtr->m_thirdPersonSystem->SafeDelete();
					   delete armedPtr->m_thirdPersonSystem;
					   armedPtr->m_thirdPersonSystem = 0;
					}

				    _getcwd( buffer, _MAX_PATH );
					CString plainDir = buffer;
					plainDir = plainDir + "\\";
	                CString filenamePathandAll = opendialog.GetPathName();
	                armedPtr->m_thirdPersonSystem = new CSkeletonObject();
					armedPtr->m_thirdPersonSystem->m_SkeletalName = m_name;
					armedPtr->m_name = m_name;
					BOOL resBool = armedPtr->m_thirdPersonSystem->LoadFullSkeletalFromDirectory(filenamePathandAll,plainDir,g_pd3dDeviceRef);
					if(!resBool)
					{
						armedPtr->m_thirdPersonSystem->SafeDelete();
						delete armedPtr->m_thirdPersonSystem;
						armedPtr->m_thirdPersonSystem = 0;
					}
					else if(oldSkeletalThird)
					{//add additional info
						if(oldSkeletalThird->m_damageSphereList)
						   oldSkeletalThird->m_damageSphereList->Clone(&armedPtr->m_thirdPersonSystem->m_damageSphereList);
			            
						if(oldSkeletalThird->m_fireQueList)
						   oldSkeletalThird->m_fireQueList->Clone(&armedPtr->m_thirdPersonSystem->m_fireQueList);
			            
						if(oldSkeletalThird->m_particleEmiterList)
						   oldSkeletalThird->m_particleEmiterList->Clone(&armedPtr->m_thirdPersonSystem->m_particleEmiterList);
						
						if(oldSkeletalThird->m_sphereBodyCollisionSystem)
						   oldSkeletalThird->m_sphereBodyCollisionSystem->Clone(&armedPtr->m_thirdPersonSystem->m_sphereBodyCollisionSystem);
						
						if(oldSkeletalThird->m_sphereLocationalDamageList)
						   oldSkeletalThird->m_sphereLocationalDamageList->Clone(&armedPtr->m_thirdPersonSystem->m_sphereLocationalDamageList);
						
			   
						//try to remap sound ranges
						if(oldSkeletalThird->m_bonesList)
						if(armedPtr->m_thirdPersonSystem->m_bonesList)
						for(POSITION bnPosLoc = oldSkeletalThird->m_bonesList->GetHeadPosition();bnPosLoc != NULL;)
						{
							CBoneObject * bone = (CBoneObject *)oldSkeletalThird->m_bonesList->GetNext(bnPosLoc);
							if(bone->m_animationList)
							for(POSITION animPos = bone->m_animationList->GetHeadPosition();animPos != NULL;)
							{
								CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetNext(animPos);
								if(!animPtr->m_soundRanges)
									continue;
								BOOL locatedSoundRange = FALSE;
								for(POSITION bnPosLoc = armedPtr->m_thirdPersonSystem->m_bonesList->GetHeadPosition();bnPosLoc != NULL;)
								{
									CBoneObject * boneTarget = (CBoneObject *)armedPtr->m_thirdPersonSystem->m_bonesList->GetNext(bnPosLoc);
									
									if(boneTarget->m_animationList)
									for(POSITION animPos = boneTarget->m_animationList->GetHeadPosition();animPos != NULL;)
									{
										CBoneAnimationObject * animPtrTarget = (CBoneAnimationObject *)boneTarget->m_animationList->GetNext(animPos);
										if(animPtrTarget->m_type == animPtr->m_type)
										   if(animPtrTarget->m_version == animPtr->m_version)
										   {
											  animPtr->m_soundRanges->Clone(&animPtrTarget->m_soundRanges);
											  locatedSoundRange = TRUE;
											  break;
										   }
									}
									if(locatedSoundRange)
									   break;
								}
								if(!locatedSoundRange)
								{
								   POSITION bnPosLoc = armedPtr->m_thirdPersonSystem->m_bonesList->GetHeadPosition();
								   if(bnPosLoc)
								   {
									CBoneObject * boneTarget = (CBoneObject *)armedPtr->m_thirdPersonSystem->m_bonesList->GetAt(bnPosLoc);
									POSITION animPos = boneTarget->m_animationList->GetHeadPosition();
									if(animPos)
									{
										CBoneAnimationObject * animPtrTarget = (CBoneAnimationObject *)boneTarget->m_animationList->GetAt(animPos);
										animPtr->m_soundRanges->Clone(&animPtrTarget->m_soundRanges);
									}
								   }
								}
							}
						}//end remap sound ranges
					}//end add additional info
					if(oldSkeletalThird)
					{
					   oldSkeletalThird->SafeDelete();
					   delete oldSkeletalThird;
					   oldSkeletalThird = 0;
					}
					GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusRef;
				 
	
		            if(armedPtr->m_thirdPersonSystem)
	                   gStatus->SkeletalTextureReassignmentWCompile(armedPtr->m_thirdPersonSystem);
			}
			else{
	        SetCurrentDirectory(MainDir);
			return;
			}
            SetCurrentDirectory(MainDir);
}

void CAppendageCustDlg::OnBnClickedCheckalwaysshowfirstperson()
{
	// TODO: Add your control notification handler code here
}

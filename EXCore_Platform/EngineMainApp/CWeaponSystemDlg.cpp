#include "resource.h"
#include "cweaponsystemdlg.h"

///////////////////////////////////////////////////////////////////////////////
// Function:	ConvertToX(CWeaponSystemDlg&, CString*)
// Description:	Convert 3DS Max files (.3ds) to DirectX format files (.x).
//				Wrapper for a call to Microsoft's conv3ds.exe utility.
///////////////////////////////////////////////////////////////////////////////

void 
ConvertToX(CWeaponSystemDlg& dlg, CString *pFilename)
{
	CString strExtension = dlg.m_pStatus->m_cabLib.GetFileExt(*pFilename).MakeLower();

	// Begin extension conversion
	if(strExtension == "3ds")
	{
		CString final = operator+("conv3ds -x -m ", *pFilename);
		system(final);
		dlg.m_pStatus->m_cabLib.RemoveFileExt(pFilename);
		dlg.m_pStatus->m_cabLib.AddNewFileExt(pFilename,".x");
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	MACROS
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CWeaponSystemDlg, CDialog)

BEGIN_MESSAGE_MAP(CWeaponSystemDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_MIS_AddMissile, OnBTNMISAddMissile)
	ON_BN_CLICKED(IDC_BTN_MIS_BrowseFileName, OnBTNMISBrowseFileName)
	ON_BN_CLICKED(IDC_BTN_MIS_DeleteMissile, OnBTNMISDeleteMissile)
	ON_BN_CLICKED(IDC_BTN_MIS_save, OnBTNMISsave)
	ON_BN_CLICKED(IDC_BTN_MIS_Load, OnBTNMISLoad)
	ON_BN_CLICKED(IDC_BTN_MIS_ReplaceValues, OnBTNMISReplaceValues)
	ON_BN_CLICKED(IDC_BTN_MIS_EditVisual, OnBTNMISEditVisual)
	ON_BN_CLICKED(IDC_BTN_MIS_AddBillboardMissile, OnBTNMISAddBillboardMissile)
	ON_BN_CLICKED(IDC_BTN_MIS_EditDynamics, OnBTNMISEditDynamics)
	ON_BN_CLICKED(IDC_BTN_MIS_Sound, OnBTNMISSound)
	ON_BN_CLICKED(IDC_BTN_MIS_AddSkeletalMissile, OnBTNMISAddSkeletalMissile)
	ON_BN_CLICKED(IDC_BTN_MIS_AddLaser, OnBTNMISAddLaser)
	ON_BN_CLICKED(IDC_BTN_MIS_DeleteLight, OnBTNMISDeleteLight)
	ON_BN_CLICKED(IDC_BTN_MIS_LightSys, OnBTNMISLightSys)
	ON_BN_CLICKED(IDC_BTN_MIS_SaveUnit, OnBTNMISSaveUnit)
	ON_BN_CLICKED(IDC_BTN_MIS_LoadUnit, OnBTNMISLoadUnit)
	ON_BN_CLICKED(IDC_BTN_MIS_AddChannel, OnBTNAddMissileChannel)
	ON_BN_CLICKED(IDC_BTN_MIS_RemoveChannel, OnBTNRemoveMissileChannel)
	ON_LBN_SELCHANGE(IDC_LIST_MIS_missileDataBase, OnSelchangeLISTMISmissileDataBase)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CWeaponSystemDlg::CWeaponSystemDlg(CWnd* pParent) 
	: CDialog(CWeaponSystemDlg::IDD, pParent)
{

}

CWeaponSystemDlg::~CWeaponSystemDlg()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnInitDialog()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

BOOL
CWeaponSystemDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->MissileDatabaseInList(this->m_pStatus->m_missileDB->GetCount() - 1);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnOK()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnOK()
{
	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	DoDataExchange(CDataExchange*)
// Description:	Inherited from CDialog. 
//				No control data members at this time.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	InitDBMissileObject(CMissileObj*)
// Description:	Open browse dialog for the weapon system window.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CWeaponSystemDlg::InitDBMissileObject(CMissileObj* mislPtr)
{ 
  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISBrowseFileName()
// Description:	Open browse dialog for the weapon system window.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISBrowseFileName() 
{
//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\MapsModels\\";
			 SetCurrentDirectory(CurDir);
//end use mapsmodels folder for organization
static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetPathName();
	   ConvertToX(*this, &fileName);
	   this->SetDlgItemText( IDC_EDIT_MIS_FileName,fileName);
	   UpdateData(FALSE);
	 }
SetCurrentDirectory(MainDir);		
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISsave()
// Description:	Save a missile model configuration to storage.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISsave() 
{
#ifndef _LimitedEngine
//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "MIS Files|*.MIS|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "MIS", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  MessageBox(fileName);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_pStatus->m_missileDB;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISLoad()
// Description:	Load a missile model configuration from storage.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISLoad() 
{
//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
//end use gamefiles folder for organization 	 
   static char BASED_CODE filter[] = "MIS Files|*.MIS|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();  
	   SetCurrentDirectory(MainDir);
	   m_pStatus->LoadMissileDatabase(fileName);
	   MissileDatabaseInList(-1);
	 }
	 SetCurrentDirectory(MainDir);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISAddMissile()
// Description:	Add a missile object to the database.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISAddMissile() 
{ 
	CMissileObj *newMissilePtr = new CMissileObj();
	newMissilePtr->m_typeOfProjectile = 0; // custom mesh

	// load type and mesh data
	// use mapsmodels folder for organization
	CString CurDir;
	CString MainDir;
	char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	MainDir = CurDir = buffer;
	CurDir = CurDir + "\\MapsModels\\";
	SetCurrentDirectory(CurDir);
	// end use mapsmodels folder for organization

	static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
	if(opendialog.DoModal() == IDOK)
	{ 
		CString filenamePathandAll = opendialog.GetPathName();

		//load imediate mode mesh data
		newMissilePtr->m_basicMesh = new CEXMeshObj();
		newMissilePtr->m_basicMesh->List_materialObject = new CMaterialObject();

		float sightRange = 0.0f;
		BOOL result = newMissilePtr->m_basicMesh->LoadGeometryFromPlugin(filenamePathandAll,
			&sightRange,
			m_pStatus->m_dxRenderer->m_pd3dDevice,
			NULL);

		/*
		if(result == FALSE)
			m_pStatus->m_systemReadout->AddLine(operator+("Mesh Initialization Failed:", filenamePathandAll));
		else
			m_pStatus->m_systemReadout->AddLine(operator+("Mesh Initialization Success:", filenamePathandAll));
		*/

		//apply mapping and add to DB
		m_pStatus->AddTextureToDatabase(newMissilePtr->m_basicMesh);
		SetCurrentDirectory(MainDir);
	}
	else
	{
		SetCurrentDirectory(MainDir);
		SafeDelete(newMissilePtr);
		/*
		newMissilePtr->SafeDelete();
		delete newMissilePtr;
		newMissilePtr=0;
		*/
		return;
	}       
	//end 

	CString conversion;
	newMissilePtr->m_lifeTime = this->GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
	newMissilePtr->m_minDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
	newMissilePtr->m_maxDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
	newMissilePtr->m_dotRefValue = this->GetDlgItemInt(IDC_EDIT_MIS_dotRef, NULL, TRUE);
	newMissilePtr->m_particleLink = this->GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
	newMissilePtr->m_explosionLink = this->GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
	newMissilePtr->m_abilityType = this->GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
	newMissilePtr->m_skillType = this->GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);

	CSkillObject *skillPtr = m_pStatus->m_skillDatabase->GetSkillByIndex(this->SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill, CB_GETCURSEL, 0, 0)-1);

	if(skillPtr)
		newMissilePtr->m_criteriaForSkillUse = skillPtr->m_skillType;
	else
		newMissilePtr->m_criteriaForSkillUse = -1;

	//newMissilePtr->m_criteriaForSkillUse = this->SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_GETCURSEL ,0,0)-1;

	this->GetDlgItemText( IDC_EDIT_MIS_MissileName,newMissilePtr->m_missileName);

	this->GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
	newMissilePtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);

	this->GetDlgItemText( IDC_EDIT_MIS_pushPowerF,conversion);
	newMissilePtr->m_pushPower = (float)atof((LPCTSTR)conversion);

	this->GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
	newMissilePtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);

	if(this->SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
		newMissilePtr->m_forceExplodeAtEnd = TRUE;
	else
		newMissilePtr->m_forceExplodeAtEnd = FALSE;

	if(this->SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
		newMissilePtr->m_filterCollision = TRUE;
	else
		newMissilePtr->m_filterCollision = FALSE;
	newMissilePtr->m_physicDynamics = new CMovementCaps();

	m_pStatus->m_missileDB->AddTail(newMissilePtr);
	MissileDatabaseInList(m_pStatus->m_missileDB->GetCount()-1);
	//SafeDelete(newMissilePtr);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	MissileDatabaseInList(int)
// Description:	Select a missile object from the missile database.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::MissileDatabaseInList(int sel)
{

   this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_RESETCONTENT,0,0);
   int trace = 0;
   for(POSITION posLoc = m_pStatus->m_missileDB->GetHeadPosition();posLoc != NULL;)
   {
       CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetNext(posLoc);
	   CString build = m_pStatus->m_cabLib.ConvToString(trace)+" ";
	   build = build+misPtr->m_missileName;
       this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,
		                                  LB_ADDSTRING,0,
										 (LPARAM)(LPCTSTR)build);
	   trace++;
   }
   if(sel > -1)
      this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_SETCURSEL ,sel,0);


   InListMissileSkillCriteria(0);
   InListMisChannels(0);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	InListMisChannels(int)
// Description:	Select a missile object from the missile database.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::InListMisChannels(int sel) 
{

     this->SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_RESETCONTENT,0,0);	
     for(POSITION posLoc = m_pStatus->m_elementDB->GetHeadPosition();posLoc != NULL;)
	 {
       CElementObj * entLoc = (CElementObj *)m_pStatus->m_elementDB->GetNext(posLoc);
	   this->SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)entLoc->m_elementName);
	 }
     if(sel > -1)
	   this->SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_SETCURSEL ,sel,0);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	InListMissileSkillCriteria(int)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::InListMissileSkillCriteria(int sel)
{ 

   this->SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_RESETCONTENT,0,0);	
   this->SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)"NONE");
   for(POSITION posLoc = m_pStatus->m_skillDatabase->GetHeadPosition();posLoc != NULL;)
   {
       CSkillObject * skLoc = (CSkillObject *)m_pStatus->m_skillDatabase->GetNext(posLoc);
	   this->SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)skLoc->m_skillName);
   }
   if(sel > -1)
	   this->SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_SETCURSEL ,sel,0);


}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISDeleteMissile()
// Description:	Remove a missile object from the missile database.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISDeleteMissile() 
{
	int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
	if(missileIndex == -1)
	{
		MessageBox("Must Select a missile database object");
		return;
	}
	POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);
	misPtr->SafeDelete();
	delete misPtr;
	misPtr=0;
	m_pStatus->m_missileDB->RemoveAt(posLoc);
	MissileDatabaseInList(m_pStatus->m_missileDB->GetCount());
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNAddMissileChannel()
// Description:	Add a missile channel.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNAddMissileChannel()
{
	int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL, 0, 0);
     if(missileIndex == -1)
	    return;

     POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);

	 if(!misPtr->m_damageChannelList)
		 misPtr->m_damageChannelList = new CElementStatusList();
	 
	 int elementSelect = this->SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_GETCURSEL ,0,0);
	 if(elementSelect == -1)
	 {
		MessageBox("Must select an element");
		return;
	 }

	 UpdateData(TRUE);
	 CElementStatusObj * statusPtr = new CElementStatusObj();
	 statusPtr->m_currentValue = this->GetDlgItemInt(IDC_EDIT_MIS_damPercentage, NULL, TRUE);
	 statusPtr->m_elementIndex = elementSelect;
	 UpdateData(FALSE);

	 misPtr->m_damageChannelList->AddTail(statusPtr);	
	 InListElementStatus(misPtr->m_damageChannelList->GetCount()-1);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	InListElementStatus()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::InListElementStatus(int sel)
{

	int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1)
	    return;

     POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);
	 
	 this->SendDlgItemMessage(IDC_LIST_DamageChannelList, LB_RESETCONTENT, 0, 0);

	 if(!misPtr->m_damageChannelList)
		 return;

	for(POSITION posLoc = misPtr->m_damageChannelList->GetHeadPosition(); posLoc !=NULL;)
	{
		CElementStatusObj* elePtr = (CElementStatusObj*)misPtr->m_damageChannelList->GetNext(posLoc);
		CString tempName = "Unknown";
		POSITION elePos = m_pStatus->m_elementDB->FindIndex(elePtr->m_elementIndex);
		 if(elePos)
		 {
           CElementObj * ptr = (CElementObj *)m_pStatus->m_elementDB->GetAt(elePos);
           tempName = ptr->m_elementName;
		 }

		CString build = tempName + " ";
		build = build + m_pStatus->m_cabLib.ConvToString(elePtr->m_currentValue);
		this->SendDlgItemMessage(IDC_LIST_DamageChannelList, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)build);
	}
	
	if(sel > -1)
		this->SendDlgItemMessage(IDC_LIST_DamageChannelList, LB_SETCURSEL, sel, 0);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNRemoveMissileChannel()
// Description:	Remove a missile channel.
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNRemoveMissileChannel()
{
	//Get the element Index
	int elementIndex = this->SendDlgItemMessage(IDC_LIST_DamageChannelList, LB_GETCURSEL, 0, 0);
	if(elementIndex == -1)
		return;

	//Get the missile Index
	int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL, 0, 0);
     if(missileIndex == -1)
	    return;

     POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     
	 CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);

	 POSITION elePos = misPtr->m_damageChannelList->FindIndex(elementIndex);
	 if(!elePos)
		return;

	 CElementStatusObj *statusPtr = (CElementStatusObj *)misPtr->m_damageChannelList->GetAt(elePos);
	 statusPtr->SafeDelete();
	 delete statusPtr;
	 statusPtr = 0;

	 misPtr->m_damageChannelList->RemoveAt(elePos);
	 InListElementStatus(elementIndex - 1);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISReplaceValues()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISReplaceValues() 
{
     int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
		MessageBox("Must select a missile database object");
	    return;
	 }
     POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
	 CString conversion;
     CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);
     misPtr->m_lifeTime = this->GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
	 misPtr->m_minDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
	 misPtr->m_maxDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
	 misPtr->m_dotRefValue = this->GetDlgItemInt(IDC_EDIT_MIS_dotRef,NULL,TRUE);
	 misPtr->m_particleLink = this->GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
	 misPtr->m_explosionLink  = this->GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
	 misPtr->m_abilityType  = this->GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
	 misPtr->m_skillType    = this->GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);
	 misPtr->m_damageSpecific = this->GetDlgItemInt(IDC_EDIT_MIS_DamageSpecificID,NULL,TRUE);
     misPtr->m_baseKickPower = (float)this->GetDlgItemInt(IDC_EDIT_MIS_kickBaseForce,NULL,TRUE);
	 this->GetDlgItemText( IDC_EDIT_MIS_MissileName,misPtr->m_missileName);


	 CSkillObject * skillPtr = m_pStatus->m_skillDatabase->GetSkillByIndex(this->SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_GETCURSEL ,0,0)-1);
	 
	 if(skillPtr)
	 misPtr->m_criteriaForSkillUse = skillPtr->m_skillType;
	 else
	 misPtr->m_criteriaForSkillUse = -1;
	 misPtr->m_damageChannel = this->SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_GETCURSEL ,0,0);


     this->GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
     misPtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);

	 this->GetDlgItemText( IDC_EDIT_MIS_pushPowerF,conversion);
     misPtr->m_pushPower = (float)atof((LPCTSTR)conversion);
 
	 this->GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
     misPtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);
     
     if(this->SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_forceExplodeAtEnd = TRUE;
	 else
	   misPtr->m_forceExplodeAtEnd = FALSE;

    
	 if(this->SendDlgItemMessage(IDC_CHECK_MIS_FaceCamera,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_orientateToCamera = TRUE;
	 else
	   misPtr->m_orientateToCamera = FALSE;

	 if(this->SendDlgItemMessage(IDC_CHECK_MIS_BurstForceType,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_burstForceType = TRUE;
	 else
	   misPtr->m_burstForceType = FALSE;

	 if(this->SendDlgItemMessage(IDC_CHECK_MIS_UseFullPhysics,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_useFullPhysics = TRUE;
	 else
	   misPtr->m_useFullPhysics = FALSE;

	 if(this->SendDlgItemMessage(IDC_CHECK_MIS_UseElastics,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_useElastics = TRUE;
	 else
	   misPtr->m_useElastics = FALSE;

     if(this->SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_filterCollision = TRUE;
	 else
	   misPtr->m_filterCollision = FALSE;
	 
	 
	 MissileDatabaseInList(missileIndex);	
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISEditVisual()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISEditVisual() 
{
#ifndef _ClientOutput
     int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
		MessageBox("Must select a missile database object");
	    return;
	 }
     POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
         CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);

		 if(misPtr->m_typeOfProjectile == 0){//basic missile
		    CMissileBaseMeshEdit dlg;
			dlg.g_pd3dDeviceRef = m_pStatus->m_dxRenderer->m_pd3dDevice;
			dlg.GL_textureDataBase = m_pStatus->m_textureDataBase;
		    dlg.m_missileRef = misPtr;
			dlg.DoModal();
			if(misPtr->m_basicMesh)
		    m_pStatus->AddTextureToDatabase(misPtr->m_basicMesh);
		 }//end basic missile
		 if(misPtr->m_typeOfProjectile == 1){//billboard
		    CMissileAddBillboardDlg dlg;
			dlg.textureDBref = m_pStatus->m_textureDataBase;
		    dlg.m_missileRef = misPtr;
			if(dlg.DoModal()==IDOK)
			{
			 float newBillboardSize = dlg.m_billboardSize*.5f;
       	     misPtr->m_basicMesh->m_abVertexArray.m_vertexArray1[0].x  = newBillboardSize;
	         misPtr->m_basicMesh->m_abVertexArray.m_vertexArray1[0].y  = newBillboardSize;
	         misPtr->m_basicMesh->m_abVertexArray.m_vertexArray1[1].x  = newBillboardSize;
	         misPtr->m_basicMesh->m_abVertexArray.m_vertexArray1[1].y  = -newBillboardSize;
	         misPtr->m_basicMesh->m_abVertexArray.m_vertexArray1[2].x  = -newBillboardSize;
	         misPtr->m_basicMesh->m_abVertexArray.m_vertexArray1[2].y  = -newBillboardSize;
	         misPtr->m_basicMesh->m_abVertexArray.m_vertexArray1[3].x  = -newBillboardSize;
	         misPtr->m_basicMesh->m_abVertexArray.m_vertexArray1[3].y  = newBillboardSize;
			 misPtr->m_basicMesh->InitBuffer(m_pStatus->m_dxRenderer->m_pd3dDevice,NULL);
			 if(misPtr->m_basicMesh)
		     m_pStatus->AddTextureToDatabase(misPtr->m_basicMesh);
			}
		 }//end basic missile
         if(misPtr->m_typeOfProjectile == 2){//skeletal
		    CSkeletalAnimationDlg dlg;
			dlg.m_renderStateObjectRef = m_pStatus->m_renderStateObject;
			dlg.m_globalShaderRef = m_pStatus->m_cgGlobalShaderDB;
			dlg.m_elementDBREF = NULL;
			dlg.m_skillDatabaseREF = NULL;
            dlg.GL_textureDataBase = m_pStatus->m_textureDataBase;
	        dlg.m_skeletonReferencePtr = misPtr->m_skeletalVisual;
            dlg.directSoundRef = m_pStatus->m_dirSnd;
	        dlg.DoModal();
			m_pStatus->SkeletalTextureReassignmentWCompile(misPtr->m_skeletalVisual);
		 }//end //skeletal
		 if(misPtr->m_typeOfProjectile == 3){//laser object
		    CLaserDlg dlg;
	        dlg.GL_textureDataBase = m_pStatus->m_textureDataBase;
	        dlg.m_laserObjectRef = misPtr->m_laserObject;
	        dlg.DoModal();
			misPtr->m_laserObject->InitializeMesh(m_pStatus->m_dxRenderer->m_pd3dDevice);
		 }//end laser object
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISAddBillboardMissile()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISAddBillboardMissile() 
{
#ifndef _ClientOutput
	CMissileObj * newMissilePtr = new CMissileObj();
	newMissilePtr->m_typeOfProjectile = 1;//billboard
    newMissilePtr->m_basicMesh = new CEXMeshObj();  

	CMissileAddBillboardDlg dlg;
	dlg.textureDBref = m_pStatus->m_textureDataBase;
	dlg.m_missileRef = NULL;
	if(dlg.DoModal()!=IDOK)
	   return;
	CString textureName = "NONE";
	float billboardSize = dlg.m_billboardSize * .5f;

	if(dlg.m_textureIndexRet >= 0){
	POSITION texPos = m_pStatus->m_textureDataBase->FindIndex(dlg.m_textureIndexRet);
     if(texPos){
	   CTextureDBObj * texPtr = (CTextureDBObj *) m_pStatus->m_textureDataBase->GetAt(texPos);
       textureName = texPtr->List_textureName;
	 }
	}

    ABVERTEX1L * vertexArray  = new ABVERTEX1L[4];
	ZeroMemory(vertexArray, sizeof(D3DVERTEX) * 4);

	WORD *      indecieArray = new WORD[6];
	ZeroMemory(indecieArray, sizeof(WORD) * 6);

	WORD        vertexCount  = 4;
	WORD        indecieCount = 6;
    
	//vertex 1 upper left
	vertexArray[0].x  = billboardSize;
	vertexArray[0].y  = billboardSize;
	vertexArray[0].z  = 0.0f;
	vertexArray[0].tx0.tu = 1.0f;
	vertexArray[0].tx0.tv = 0.0f;
	vertexArray[0].nx = 0.0f;
	vertexArray[0].ny = 0.0f;
	vertexArray[0].nz = 1.0f;
	//vertex 2 bottom left
	vertexArray[1].x  = billboardSize;
	vertexArray[1].y  = -billboardSize;
	vertexArray[1].z  = 0.0f;
	vertexArray[1].tx0.tu = 0.0f;
	vertexArray[1].tx0.tv = 0.0f;
	vertexArray[1].nx = 0.0f;
	vertexArray[1].ny = 0.0f;
	vertexArray[1].nz = 1.0f;
	//vertex 3 bottom right
	vertexArray[2].x  = -billboardSize;
	vertexArray[2].y  = -billboardSize;
	vertexArray[2].z  = 0.0f;
	vertexArray[2].tx0.tu = 0.0f;
	vertexArray[2].tx0.tv = 1.0f;
	vertexArray[2].nx = 0.0f;
	vertexArray[2].ny = 0.0f;
	vertexArray[2].nz = 1.0f;
	//vertex 4 upper right
	vertexArray[3].x  = -billboardSize;
	vertexArray[3].y  = billboardSize;
	vertexArray[3].z  = 0.0f;
	vertexArray[3].tx0.tu = 1.0f;
	vertexArray[3].tx0.tv = 1.0f;
	vertexArray[3].nx = 0.0f;
	vertexArray[3].ny = 0.0f;
	vertexArray[3].nz = 1.0f;
	//indecies
	//no facing
	indecieArray[0] = 1;
	indecieArray[1] = 0;
	indecieArray[2] = 3;
	indecieArray[3] = 2;
	indecieArray[4] = 1;
	indecieArray[5] = 3;


    newMissilePtr->m_basicMesh->List_materialObject = new CMaterialObject();
	newMissilePtr->m_basicMesh->SetGeometry2(textureName,
							                 vertexArray,
							                 vertexCount,
							                 indecieArray,
							                 indecieCount);
     newMissilePtr->m_basicMesh->InitBuffer(m_pStatus->m_dxRenderer->m_pd3dDevice,NULL);
	 m_pStatus->AddTextureToDatabase(newMissilePtr->m_basicMesh);
     //end 
     CString conversion;
     newMissilePtr->m_lifeTime = this->GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
	 newMissilePtr->m_minDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
	 newMissilePtr->m_maxDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
	 newMissilePtr->m_dotRefValue = this->GetDlgItemInt(IDC_EDIT_MIS_dotRef, NULL, TRUE);
	 newMissilePtr->m_particleLink = this->GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
	 newMissilePtr->m_explosionLink  = this->GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
	 newMissilePtr->m_abilityType  = this->GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
	 newMissilePtr->m_skillType    = this->GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);
	 this->GetDlgItemText( IDC_EDIT_MIS_MissileName,newMissilePtr->m_missileName);
	 
     this->GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
     newMissilePtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);
 
	 this->GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
     newMissilePtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);
     
     if(this->SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_forceExplodeAtEnd = TRUE;
	 else
	   newMissilePtr->m_forceExplodeAtEnd = FALSE;

	 if(this->SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_filterCollision = TRUE;
	 else
	   newMissilePtr->m_filterCollision = FALSE;

     newMissilePtr->m_physicDynamics = new CMovementCaps();

	 m_pStatus->m_missileDB->AddTail(newMissilePtr);
	 MissileDatabaseInList(m_pStatus->m_missileDB->GetCount());
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISEditDynamics()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISEditDynamics() 
{
#ifndef _ClientOutput
     int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
		MessageBox("Must select a missile database object");
	    return;
	 }
     POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
         CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);	
	     CMotionSettingsDlg dlg;
		 dlg.movementDB = misPtr->m_physicDynamics;
		 dlg.m_aiListRef = m_pStatus->m_aiList;
		 dlg.DoModal();
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISSound()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISSound() 
{
#ifndef _ClientOutput
	int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase, LB_GETCURSEL, 0, 0);
	if(missileIndex == -1)
	{
		MessageBox("Must Select a missile database object");
		return;
	}
	POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	if(!posLoc) return;
	CMissileObj *misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);
	CSoundSettingsDlg dlg;
	dlg.enabled = misPtr->m_enableSound; 
	dlg.m_distanceFactor = misPtr->m_distanceFactor;
	dlg.m_sndID = misPtr->m_sndID;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.enabled)
		{
			misPtr->m_sndID            = dlg.m_sndID;
			misPtr->m_enableSound      = dlg.enabled;
			misPtr->m_distanceFactor   = dlg.m_distanceFactor;
		}
	}	
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISAddSkeletalMissile()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISAddSkeletalMissile() 
{
#ifndef _ClientOutput
	//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\MapsModels\\";
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization
    static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() != IDOK ){
	      SetCurrentDirectory(MainDir);
          return;
	 }
	 CString fileName = opendialog.GetPathName();
	 SetCurrentDirectory(MainDir);	
	
	 CMissileObj * newMissilePtr = new CMissileObj();
	 newMissilePtr->m_typeOfProjectile = 2;//skeletal missile
     newMissilePtr->m_skeletalVisual = new CSkeletonObject();
	 newMissilePtr->m_skeletalVisual->InitBoneSystem(fileName,m_pStatus->m_dxRenderer->m_pd3dDevice);
	 newMissilePtr->m_skeletalVisual->m_SkeletalName = fileName;
     CSkeletalAnimationDlg dlg;
	 dlg.m_renderStateObjectRef = m_pStatus->m_renderStateObject;
	 dlg.m_globalShaderRef = m_pStatus->m_cgGlobalShaderDB;
	 dlg.m_elementDBREF = NULL;
	 dlg.m_skillDatabaseREF = NULL;
     dlg.GL_textureDataBase = m_pStatus->m_textureDataBase;
	 dlg.m_skeletonReferencePtr = newMissilePtr->m_skeletalVisual;
     dlg.directSoundRef = m_pStatus->m_dirSnd;
	 dlg.DoModal();
	 m_pStatus->SkeletalTextureReassignmentWCompile(newMissilePtr->m_skeletalVisual);

     //end 
     CString conversion;
     newMissilePtr->m_lifeTime = this->GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
	 newMissilePtr->m_minDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
	 newMissilePtr->m_maxDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
	 newMissilePtr->m_dotRefValue = this->GetDlgItemInt(IDC_EDIT_MIS_dotRef, NULL, TRUE);
	 newMissilePtr->m_particleLink = this->GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
	 newMissilePtr->m_explosionLink  = this->GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
	 newMissilePtr->m_abilityType  = this->GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
	 newMissilePtr->m_skillType    = this->GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);
	 this->GetDlgItemText( IDC_EDIT_MIS_MissileName,newMissilePtr->m_missileName);
	 
     this->GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
     newMissilePtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);
 
	 this->GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
     newMissilePtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);
     
     if(this->SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_forceExplodeAtEnd = TRUE;
	 else
	   newMissilePtr->m_forceExplodeAtEnd = FALSE;

	 if(this->SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_filterCollision = TRUE;
	 else
	   newMissilePtr->m_filterCollision = FALSE;

     newMissilePtr->m_physicDynamics = new CMovementCaps();

	 m_pStatus->m_missileDB->AddTail(newMissilePtr);
	 MissileDatabaseInList(m_pStatus->m_missileDB->GetCount());	
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISAddLaser()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISAddLaser() 
{
#ifndef _ClientOutput
     CMissileObj * newMissilePtr = new CMissileObj();
	 newMissilePtr->m_typeOfProjectile = 3;//laser segment
     
     newMissilePtr->m_laserObject = new CLaserObj();
	 newMissilePtr->m_laserObject->InitializeMesh(m_pStatus->m_dxRenderer->m_pd3dDevice); 

	 CLaserDlg dlg;
	 dlg.GL_textureDataBase = m_pStatus->m_textureDataBase;
	 dlg.m_laserObjectRef = newMissilePtr->m_laserObject;
	 if(dlg.DoModal() == IDOK)
	 {
		 CString conversion;
		 newMissilePtr->m_lifeTime = this->GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
		 newMissilePtr->m_minDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
		 newMissilePtr->m_maxDamageValue = this->GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
		 newMissilePtr->m_dotRefValue = this->GetDlgItemInt(IDC_EDIT_MIS_dotRef, NULL, TRUE);
		 newMissilePtr->m_particleLink = this->GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
		 newMissilePtr->m_explosionLink  = this->GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
		 newMissilePtr->m_abilityType  = this->GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
		 newMissilePtr->m_skillType    = this->GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);
		 this->GetDlgItemText( IDC_EDIT_MIS_MissileName,newMissilePtr->m_missileName);
		 
		 this->GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
		 newMissilePtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);
	 
		 this->GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
		 newMissilePtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);
	     
		 if(this->SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
		   newMissilePtr->m_forceExplodeAtEnd = TRUE;
		 else
		   newMissilePtr->m_forceExplodeAtEnd = FALSE;

		 if(this->SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
		   newMissilePtr->m_filterCollision = TRUE;
		 else
		   newMissilePtr->m_filterCollision = FALSE;

		 newMissilePtr->m_physicDynamics = new CMovementCaps();

		 m_pStatus->m_missileDB->AddTail(newMissilePtr);
		 MissileDatabaseInList(m_pStatus->m_missileDB->GetCount()-1);	
	 }
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISDeleteLight()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISDeleteLight() 
{
	int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
	if(missileIndex == -1)
	{
		MessageBox("Must Select a missile database object");
		return;
	}
	POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	if(!posLoc)
		return;
	
	CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);	 
	if(misPtr->m_light)
	{
		if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
			return;
		
		delete misPtr->m_light;
		misPtr->m_light = 0;
		AfxMessageBox("Done");
	}
	else
		AfxMessageBox("No light existed");
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISLightSys()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISLightSys() 
{
#ifndef _ClientOutput
	int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
        MessageBox("Must Select a missile database object");
	    return;
	 }
     POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
         CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);	 
			 CLightInterfaceDlg dlg;
	         dlg.GL_textureDataBase = m_pStatus->m_textureDataBase;
	         dlg.m_lightREF = misPtr->m_light;
	         if(dlg.DoModal()==IDOK)
		        misPtr->m_light = dlg.m_lightREF;
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISSaveUnit()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISSaveUnit() 
{
#ifndef _LimitedEngine
	int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
        MessageBox("Must Select a missile database object");
	    return;
	 }
     POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
         CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);
		
        //use gamefiles folder for organization
        CString CurDir;
        CString MainDir;
        char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
        //end use gamefiles folder for organization
        static char BASED_CODE filter[] = "MIE Files|*.MIE|All Files (*.*)|*.*||";
        CFileDialog opendialog( FALSE, "MUI", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
        if ( opendialog.DoModal() == IDOK )
		{
	         CString fileName = opendialog.GetFileName();
	         CFileException exc;
             CFile fl;
             BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
             if(res == FALSE){
		       SetCurrentDirectory(MainDir);
		       MessageBox(fileName);
	           return;
			 }
             CArchive the_out_Archive(&fl,CArchive::store);
             the_out_Archive << misPtr;
             the_out_Archive.Close();
             fl.Close();
		}
	    SetCurrentDirectory(MainDir); 
#endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnBTNMISLoadUnit()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnBTNMISLoadUnit() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "MIE Files|*.MIE|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   CMissileObj* misPtr = NULL;
	   the_in_Archive >> misPtr;
	   the_in_Archive.Close();
       fl.Close();  
	   //Prep then add baby!!
	   SetCurrentDirectory(MainDir);	
	   m_pStatus->m_missileDB->AddTail(misPtr);
	   MissileDatabaseInList(m_pStatus->m_missileDB->GetCount()-1);
	 }
     SetCurrentDirectory(MainDir);	
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CWeaponSystemDlg
// Function:	OnSelchangeLISTMISmissileDataBase()
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CWeaponSystemDlg::OnSelchangeLISTMISmissileDataBase() 
{
	int missileIndex = this->SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
	    return;
	 }
     POSITION posLoc = m_pStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     CMissileObj* misPtr = (CMissileObj*)m_pStatus->m_missileDB->GetAt(posLoc);
	 this->SetDlgItemText( IDC_EDIT_MIS_MissileName,          misPtr->m_missileName);
     this->SetDlgItemInt(  IDC_EDIT_MIS_LifeTime,             misPtr->m_lifeTime,TRUE);
	 this->SetDlgItemInt(  IDC_EDIT_MIS_Damage,               misPtr->m_minDamageValue,TRUE);
	 this->SetDlgItemInt(  IDC_EDIT_MIS_DamageMax,			misPtr->m_maxDamageValue,TRUE);
	 this->SetDlgItemInt(  IDC_EDIT_MIS_dotRef,				misPtr->m_dotRefValue,TRUE);
	 this->SetDlgItemInt(  IDC_EDIT_MIS_ParticleDBlink,       misPtr->m_particleLink,TRUE);
	 this->SetDlgItemInt(  IDC_EDIT_MIS_ExplosionAttribute,   misPtr->m_explosionLink,TRUE);
     this->SetDlgItemInt(  IDC_EDIT_MIS_AbilityUseType,       misPtr->m_abilityType,TRUE);
	 this->SetDlgItemInt(  IDC_EDIT_MIS_SkillUseType,         misPtr->m_skillType,TRUE);
	 this->SetDlgItemInt(  IDC_EDIT_MIS_DamageSpecificID,     misPtr->m_damageSpecific,TRUE);
	 this->SetDlgItemInt(  IDC_EDIT_MIS_kickBaseForce,        (int)misPtr->m_baseKickPower,TRUE);
	 this->SetDlgItemText( IDC_EDIT_MIS_CollisionRadius,m_pStatus->m_cabLib.FloatToStingCnv(misPtr->m_collisionRadius));
     this->SetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,m_pStatus->m_cabLib.FloatToStingCnv(misPtr->m_heatSeekingPower));
	 this->SetDlgItemText( IDC_EDIT_MIS_pushPowerF,m_pStatus->m_cabLib.FloatToStingCnv(misPtr->m_pushPower));

	 this->SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_SETCURSEL ,misPtr->m_damageChannel,0);
     this->SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_SETCURSEL ,(m_pStatus->m_skillDatabase->GetSkillIndexByType(misPtr->m_criteriaForSkillUse)+1),0);

     if(misPtr->m_forceExplodeAtEnd == TRUE)
	    this->SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_SETCHECK,BST_CHECKED,0);
     else
	    this->SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_SETCHECK,BST_UNCHECKED,0);

	 if(misPtr->m_orientateToCamera == TRUE)
		this->SendDlgItemMessage(IDC_CHECK_MIS_FaceCamera,BM_SETCHECK,BST_CHECKED,0);
     else
	    this->SendDlgItemMessage(IDC_CHECK_MIS_FaceCamera,BM_SETCHECK,BST_UNCHECKED,0);
    
     if(misPtr->m_useElastics == TRUE)
		this->SendDlgItemMessage(IDC_CHECK_MIS_UseElastics,BM_SETCHECK,BST_CHECKED,0);
     else
	    this->SendDlgItemMessage(IDC_CHECK_MIS_UseElastics,BM_SETCHECK,BST_UNCHECKED,0);

     if(misPtr->m_useFullPhysics == TRUE)
		this->SendDlgItemMessage(IDC_CHECK_MIS_UseFullPhysics,BM_SETCHECK,BST_CHECKED,0);
     else
	    this->SendDlgItemMessage(IDC_CHECK_MIS_UseFullPhysics,BM_SETCHECK,BST_UNCHECKED,0);

	 if(misPtr->m_burstForceType == TRUE)
		this->SendDlgItemMessage(IDC_CHECK_MIS_BurstForceType,BM_SETCHECK,BST_CHECKED,0);
     else
	    this->SendDlgItemMessage(IDC_CHECK_MIS_BurstForceType,BM_SETCHECK,BST_UNCHECKED,0);

	 if(misPtr->m_filterCollision == TRUE)
		this->SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_SETCHECK,BST_CHECKED,0);
     else
	    this->SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_SETCHECK,BST_UNCHECKED,0);

	 InListElementStatus(-1);
}
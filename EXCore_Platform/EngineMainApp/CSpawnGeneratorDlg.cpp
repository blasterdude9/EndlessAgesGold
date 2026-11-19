#include "CSpawnGeneratorDlg.h"

///////////////////////////////////////////////////////////////////////////////
// Class:		CSpawnGeneratorDlg
// Function:	MACROS
///////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSpawnGeneratorDlg, CDialog)

BEGIN_MESSAGE_MAP(CSpawnGeneratorDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_Spawn_AddGen, OnBTNSpawnAddGen)
	ON_BN_CLICKED(IDC_BTN_Spawn_DeleteGen, OnBTNSpawnDeleteGen)
	ON_BN_CLICKED(IDC_BTN_Spawn_EditGen, OnBTNSpawnEditGen)
	ON_BN_CLICKED(IDC_BTN_SPN_ExportSys, OnBTNSPNExportSys)
	ON_BN_CLICKED(IDC_BTN_SPN_ImportSys, OnBTNSPNImportSys)
	ON_BN_CLICKED(IDC_CHECK_SPN_EnableSystem, OnCHECKSPNEnableSystem)
	ON_BN_CLICKED(IDC_BTN_SPN_AppendSys, OnBTNSPNAppendSys)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// Class:		CSpawnGeneratorDlg
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CSpawnGeneratorDlg::CSpawnGeneratorDlg(CWnd* pParent) 
	: CDialog(CSpawnGeneratorDlg::IDD, pParent)
{

}

CSpawnGeneratorDlg::~CSpawnGeneratorDlg()
{
	
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CSpawnGeneratorDlg
// Function:	OnInitDialog()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

BOOL
CSpawnGeneratorDlg::OnInitDialog()
{
	m_pStatus->SpawnGenDatabaseInList(0, this);
	CDialog::OnInitDialog();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CSpawnGeneratorDlg
// Function:	OnOK()
// Description:	Inherited from CDialog.
///////////////////////////////////////////////////////////////////////////////

void
CSpawnGeneratorDlg::OnOK()
{
	CDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CSpawnGeneratorDlg
// Function:	DoDataExchange(CDataExchange*)
// Description:	Inherited from CDialog. 
//				No control data members at this time.
///////////////////////////////////////////////////////////////////////////////

void
CSpawnGeneratorDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
}



void CSpawnGeneratorDlg::OnBTNSPNExportSys() 
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
        static char BASED_CODE filter[] = "SDB Files|*.SDB|All Files (*.*)|*.*||";
        CFileDialog opendialog( FALSE, "SDB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
             the_out_Archive << m_pStatus->m_spawnGeneratorDB << m_pStatus->GL_aiWebDatabase;
             the_out_Archive.Close();
             fl.Close();
		}
	    SetCurrentDirectory(MainDir);  	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CSpawnGeneratorDlg::OnBTNSPNImportSys() 
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

   static char BASED_CODE filter[] = "SDB Files|*.SDB|All Files (*.*)|*.*||";
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
       if(m_pStatus->m_spawnGeneratorDB)
	   {
          m_pStatus->m_spawnGeneratorDB->SafeDelete();
		  delete m_pStatus->m_spawnGeneratorDB;
		  m_pStatus->m_spawnGeneratorDB = 0;
	   }
	   if(m_pStatus->GL_aiWebDatabase)
	   { 
          m_pStatus->GL_aiWebDatabase->SafeDelete();
		  delete m_pStatus->GL_aiWebDatabase;
          m_pStatus->GL_aiWebDatabase = 0;
	   }

	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> m_pStatus->m_spawnGeneratorDB >> m_pStatus->GL_aiWebDatabase;
	   the_in_Archive.Close();
       fl.Close();  
		//Prep then add baby!!
		SetCurrentDirectory(MainDir);	
		m_pStatus->SpawnGenDatabaseInList(0, this);
		this->m_pAISystemDlg->AITreeInList(-1);
	 }
     SetCurrentDirectory(MainDir);	
}

void CSpawnGeneratorDlg::OnBTNSpawnAddGen() 
{
#ifndef _ClientOutput
	CAddSpawnGeneratorDlg dlg;
	dlg.m_aiCfgDatabaseRef    = m_pStatus->m_aiList;
    CSpawnObj * spawnGenPtr = new CSpawnObj();
       dlg.m_generatorName = spawnGenPtr->List_spawnName;
	   
	   dlg.m_posX = spawnGenPtr->List_spawnMatrix.m[3][0]; 
	   dlg.m_posY = spawnGenPtr->List_spawnMatrix.m[3][1]; 
	   dlg.m_posZ = spawnGenPtr->List_spawnMatrix.m[3][2]; 
	   
	   dlg.m_possibilityOfSpawn = spawnGenPtr->List_possibilityOfSpawn;
	   dlg.m_maxOutputCapacity = spawnGenPtr->List_maxOutPutCapacity;
       dlg.m_possibilityOfSpawn = spawnGenPtr->List_possibilityOfSpawn;//% out of 1000
	   dlg.m_spawnIndex = spawnGenPtr->List_spawnIndex;
	   dlg.m_spawnRadius = spawnGenPtr->List_spawnRadius;
   	   dlg.m_spawnType = spawnGenPtr->List_spawnType;
       dlg.treeList = m_pStatus->GL_aiWebDatabase;
	   dlg.spawnObj = spawnGenPtr;
	   dlg.useRuntimeEntityList = FALSE;

    if(dlg.DoModal() == IDOK){
       spawnGenPtr->List_spawnName = dlg.m_generatorName;
	   if(dlg.useRuntimeEntityList == TRUE){
		   int ENIndex = this->m_pEntityDlg->SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	       if(ENIndex == -1){
              MessageBox("Must select an entity object on the entity runtime interface");
		      return;
		   }
	       POSITION position = m_pStatus->m_entObjRuntime->FindIndex(ENIndex);
	       if(position){
            CMovementObj * mPtrtarget = (CMovementObj *)m_pStatus->m_entObjRuntime->GetAt(position);
	        mPtrtarget->List_baseFrame->GetTransform(NULL,&spawnGenPtr->List_spawnMatrix);
		   }
	   }
	   else{
	   spawnGenPtr->List_spawnMatrix.m[3][0] = dlg.m_posX; 
	   spawnGenPtr->List_spawnMatrix.m[3][1] = dlg.m_posY; 
	   spawnGenPtr->List_spawnMatrix.m[3][2] = dlg.m_posZ; 
	   }
	   spawnGenPtr->List_possibilityOfSpawn = dlg.m_possibilityOfSpawn;
	   spawnGenPtr->List_maxOutPutCapacity = dlg.m_maxOutputCapacity;
       spawnGenPtr->List_possibilityOfSpawn = dlg.m_possibilityOfSpawn;//% out of 1000
	   spawnGenPtr->List_spawnIndex = dlg.m_spawnIndex;
	   spawnGenPtr->List_spawnRadius = dlg.m_spawnRadius;
   	   spawnGenPtr->List_spawnType = dlg.m_spawnType;
	   m_pStatus->m_spawnGeneratorDB->AddTail(spawnGenPtr);
	   m_pStatus->SpawnGenDatabaseInList(m_pStatus->m_spawnGeneratorDB->GetCount()-1, this);
	   m_pStatus->m_spawnGeneratorDB->GetTotalAIOutput();
	}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CSpawnGeneratorDlg::OnBTNSpawnDeleteGen() 
{
	int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_Spawn_listDB,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select a system");
		return;
	}
	POSITION posLoc = m_pStatus->m_spawnGeneratorDB->FindIndex(runtimeIndex);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CSpawnObj * ptrLoc = ( CSpawnObj* ) m_pStatus->m_spawnGeneratorDB->GetAt(posLoc);
	delete ptrLoc;
	ptrLoc=0;
	m_pStatus->m_spawnGeneratorDB->RemoveAt(posLoc);
	m_pStatus->SpawnGenDatabaseInList(runtimeIndex-1, this);
	m_pStatus->m_spawnGeneratorDB->GetTotalAIOutput();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CSpawnGeneratorDlg::OnBTNSpawnEditGen() 
{
#ifndef _ClientOutput
int runtimeIndex = this->SendDlgItemMessage(IDC_LIST_Spawn_listDB,LB_GETCURSEL ,0,0);
 if(runtimeIndex == -1){
    MessageBox("Must select a system");
	return;
 }
 POSITION posLoc = m_pStatus->m_spawnGeneratorDB->FindIndex(runtimeIndex);
 if(!posLoc)
     return;
     CSpawnObj * spawnGenPtr = ( CSpawnObj* ) m_pStatus->m_spawnGeneratorDB->GetAt(posLoc);
     CAddSpawnGeneratorDlg dlg;
           
	   dlg.m_aiCfgDatabaseRef    = m_pStatus->m_aiList;
       dlg.m_generatorName       = spawnGenPtr->List_spawnName;
       dlg.m_posX                = spawnGenPtr->List_spawnMatrix.m[3][0]; 
	   dlg.m_posY                = spawnGenPtr->List_spawnMatrix.m[3][1]; 
	   dlg.m_posZ                = spawnGenPtr->List_spawnMatrix.m[3][2]; 
	   dlg.m_possibilityOfSpawn  = spawnGenPtr->List_possibilityOfSpawn;
	   dlg.m_maxOutputCapacity   = spawnGenPtr->List_maxOutPutCapacity;
       dlg.m_possibilityOfSpawn  = spawnGenPtr->List_possibilityOfSpawn;//% out of 1000
	   dlg.m_spawnIndex          = spawnGenPtr->List_spawnIndex;
	   dlg.m_spawnRadius         = spawnGenPtr->List_spawnRadius;
   	   dlg.m_spawnType           = spawnGenPtr->List_spawnType;
	   dlg.treeList              = m_pStatus->GL_aiWebDatabase;
	   dlg.spawnObj              = spawnGenPtr;
	   dlg.useRuntimeEntityList = FALSE;

   if(dlg.DoModal()==IDOK){
	   if(dlg.useRuntimeEntityList == TRUE){
         int ENIndex = this->m_pEntityDlg->SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	       if(ENIndex == -1){
              MessageBox("Must select an entity object on the entity runtime interface");
		      return;
		   }
	       POSITION position = m_pStatus->m_entObjRuntime->FindIndex(ENIndex);
	       if(position){
            CMovementObj * mPtrtarget = (CMovementObj *)m_pStatus->m_entObjRuntime->GetAt(position);
	        mPtrtarget->List_baseFrame->GetTransform(NULL,&spawnGenPtr->List_spawnMatrix);
		   }
	   }
	   else{
	   spawnGenPtr->List_spawnMatrix.m[3][0] = dlg.m_posX; 
	   spawnGenPtr->List_spawnMatrix.m[3][1] = dlg.m_posY; 
	   spawnGenPtr->List_spawnMatrix.m[3][2] = dlg.m_posZ; 
	   }
       spawnGenPtr->List_spawnName = dlg.m_generatorName;
	   spawnGenPtr->List_possibilityOfSpawn = dlg.m_possibilityOfSpawn;
	   spawnGenPtr->List_maxOutPutCapacity = dlg.m_maxOutputCapacity;
       spawnGenPtr->List_possibilityOfSpawn = dlg.m_possibilityOfSpawn;//% out of 1000
	   spawnGenPtr->List_spawnIndex = dlg.m_spawnIndex;
	   spawnGenPtr->List_spawnRadius = dlg.m_spawnRadius;
   	   spawnGenPtr->List_spawnType = dlg.m_spawnType;
	   m_pStatus->SpawnGenDatabaseInList(runtimeIndex, this);
   }	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CSpawnGeneratorDlg::OnCHECKSPNEnableSystem() 
{
	if(m_pStatus->m_spawnGeneratorDB->m_enable)
	   m_pStatus->m_spawnGeneratorDB->m_enable = FALSE;
	else
	   m_pStatus->m_spawnGeneratorDB->m_enable = TRUE;
	
}

void CSpawnGeneratorDlg::OnBTNSPNAppendSys() 
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

   static char BASED_CODE filter[] = "SDB Files|*.SDB|All Files (*.*)|*.*||";
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
       if(!this->m_pStatus->m_spawnGeneratorDB)
	   {
           this->m_pStatus->m_spawnGeneratorDB = new CSpawnObjList();
	   }
	   if(!this->m_pStatus->GL_aiWebDatabase)
	   { 
           this->m_pStatus->GL_aiWebDatabase = new CAIWebObjList();
	   }

	   CAIWebObjList * aiWebHolder = NULL;
	   CSpawnObjList * spawnGenHolder = NULL;

	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> spawnGenHolder >> aiWebHolder;
	   the_in_Archive.Close();
       fl.Close();  

	   int offset = this->m_pStatus->GL_aiWebDatabase->GetCount();

       if(aiWebHolder)
       for(POSITION posBuild = aiWebHolder->GetHeadPosition();posBuild != NULL;)
	   {
           CAIWebNode * webNode = (CAIWebNode *)aiWebHolder->GetNext(posBuild);
           this->m_pStatus->GL_aiWebDatabase->AddTail(webNode);
	   }
	   
	   if(spawnGenHolder)
       for(POSITION posBuild = spawnGenHolder->GetHeadPosition();posBuild != NULL;)
	   {
           CSpawnObj * spawnNode = (CSpawnObj *)spawnGenHolder->GetNext(posBuild);
		   spawnNode->m_treeUse += offset;
           this->m_pStatus->m_spawnGeneratorDB->AddTail(spawnNode);
	   }
	   
	   SetCurrentDirectory(MainDir);	
	   this->m_pStatus->SpawnGenDatabaseInList(0);
	   this->m_pAISystemDlg->AITreeInList(-1);
	 }
     SetCurrentDirectory(MainDir);	
}
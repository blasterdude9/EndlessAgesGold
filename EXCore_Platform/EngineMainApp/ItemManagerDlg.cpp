// ItemManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "..\AuraUI\resource.h"
#include "ItemManagerDlg.h"
#include "globalstatusvariables.h"
#include "direct.h"
#include "AppendageCustDlg.h"
#include "AddItemDlg.h"
#include "SqlConnectionDlg.h"


// CItemManagerDlg dialog

IMPLEMENT_DYNAMIC(CItemManagerDlg, CDialog)

CItemManagerDlg::CItemManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemManagerDlg::IDD, pParent)
{
	m_selectedEntity = 0;
}

CItemManagerDlg::~CItemManagerDlg()
{
}

void CItemManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_imItemList, m_imItemList);
}


BEGIN_MESSAGE_MAP(CItemManagerDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_imAddItem, &CItemManagerDlg::OnBnClickedButtonimadditem)
	ON_BN_CLICKED(IDC_BUTTON_imEditItem, &CItemManagerDlg::OnBnClickedButtonimedititem)
	ON_BN_CLICKED(IDC_BUTTON_imDeleteItem, &CItemManagerDlg::OnBnClickedButtonimdeleteitem)
	ON_BN_CLICKED(IDC_BUTTON_imArmItemTest, &CItemManagerDlg::OnBnClickedButtonimarmitemtest)
	ON_BN_CLICKED(IDC_BUTTON_imImportEDB, &CItemManagerDlg::OnBnClickedButtonimimportedb)
	ON_BN_CLICKED(IDC_BUTTON_imImportAPD, &CItemManagerDlg::OnBnClickedButtonimimportapd)
	ON_BN_CLICKED(IDC_BUTTON_imVisuals, &CItemManagerDlg::OnBnClickedButtonimvisuals)
	ON_BN_CLICKED(IDC_BUTTON_imSave, &CItemManagerDlg::OnBnClickedButtonimsave)
	ON_BN_CLICKED(IDC_BUTTON_imLoad, &CItemManagerDlg::OnBnClickedButtonimload)
	ON_BN_CLICKED(IDOK, &CItemManagerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_exportToSQL, &CItemManagerDlg::OnBnClickedButtonexporttosql)
END_MESSAGE_MAP()


// CItemManagerDlg message handlers

BOOL CItemManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InList(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CItemManagerDlg::OnBnClickedButtonimadditem()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
	
	
	CItemObj * itemObj = new CItemObj();
	itemObj->m_attachmentRefList = new CAttachmentRefList();
	itemObj->m_globalID = gStatus->m_itemManager->GetFreeGlobalID_safe();

    //aquire free global id
	
	CAddItemDlg dlg;
	dlg.m_skillDatabaseRef			= gStatus->m_skillDatabase;
	dlg.m_itemRef					= itemObj;
	dlg.m_armedInventoryListRef		= gStatus->m_itemManager->m_visuals;
	dlg.m_currentElements			= gStatus->m_elementDB;
	dlg.m_enitityDB					= gStatus->m_entObjList;
	if(dlg.DoModal()==IDOK)
	{
	   CItemObj * itemDuplicateNameTest = gStatus->m_itemManager->GetItemByName_LOS(itemObj->m_itemName.GetBuffer());
	   if(itemDuplicateNameTest)
	   {
	      
		  AfxMessageBox("Name already exists, cannot add error:"+itemObj->m_itemName);
		  gStatus->m_itemManager->Unlock();
		  delete itemObj;
		  itemObj = 0;
		  return;
	   }
	   gStatus->m_itemManager->Add_safe(itemObj);
	   InList(itemObj->m_itemName);
	}
}

void CItemManagerDlg::OnBnClickedButtonimedititem()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
	int sel = m_imItemList.GetCurSel();
    if(sel < 0)
	{
       AfxMessageBox("Must select a item from the list");
       return;
	}
	CString itemName;
    m_imItemList.GetText(sel,itemName);
	CItemObj * itemObj = gStatus->m_itemManager->GetItemByName_LOS(itemName.GetBuffer());
	if(!itemObj)
	{
        AfxMessageBox("Cannot find item, error 64");
		return;
	}

	int gId					= itemObj->m_globalID;
	CString lastName		= itemObj->m_itemName;
	
	
	CAddItemDlg dlg;
	dlg.m_skillDatabaseRef			= gStatus->m_skillDatabase;
	dlg.m_itemRef					= itemObj;
	dlg.m_armedInventoryListRef		= gStatus->m_itemManager->m_visuals;
	dlg.m_currentElements			= gStatus->m_elementDB;
	dlg.m_enitityDB					= gStatus->m_entObjList;
	if(dlg.DoModal()==IDOK)
	{
	     //reindex item
		 gStatus->m_itemManager->Unlock();
		 gStatus->m_itemManager->RemoveByData_safe(gId,lastName.GetBuffer());
		 gStatus->m_itemManager->Add_safe(itemObj);//reindex with new data
		 InList(itemObj->m_itemName);

	}
	else
	  gStatus->m_itemManager->Unlock();
}

void CItemManagerDlg::OnBnClickedButtonimdeleteitem()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
	int sel = m_imItemList.GetCurSel();
    if(sel < 0)
	{
       AfxMessageBox("Must select a item from the list");
       return;
	}
	CString itemName;
    m_imItemList.GetText(sel,itemName);
	CItemObj * itemObj = gStatus->m_itemManager->GetItemByName_LOS(itemName.GetBuffer());
	if(!itemObj)
	{
        AfxMessageBox("Cannot find item, error 64");
		return;
	}
    gStatus->m_itemManager->Unlock();

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	gStatus->m_itemManager->DeleteByOject(itemObj);
    InList(sel-1);
}

void CItemManagerDlg::OnBnClickedButtonimarmitemtest()
{
    GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
	int sel = m_imItemList.GetCurSel();
    if(sel < 0)
	{
       AfxMessageBox("Must select a item from the list");
       return;
	}
	CString itemName;
    m_imItemList.GetText(sel,itemName);
	CItemObj * itemObj = gStatus->m_itemManager->GetItemByName_LOS(itemName.GetBuffer());
	if(!itemObj)
	{
        AfxMessageBox("Cannot find item, error 64");
		return;
	}

	CMovementObj * mPtr = NULL;
	if(m_selectedEntity < 0)
		mPtr = gStatus->m_entObjRuntime->GetObjByIndex(0);
	else
		mPtr = gStatus->m_entObjRuntime->GetObjByIndex(m_selectedEntity);

    if(!mPtr)
	{
	    gStatus->m_itemManager->Unlock();
        AfxMessageBox("a runtime entity needs to be spawned",0,0);
        return;
	} 
	CMovementObj * DBPtr = gStatus->m_entObjList->GetByID(mPtr->m_uniqueId);
    if(!DBPtr)
	{
		gStatus->m_itemManager->Unlock();
		AfxMessageBox("Primary EDB ref cannot be found",0,0);
		return;
	}

	int gIDStore = itemObj->m_globalID;
	gStatus->m_itemManager->Unlock();
	gStatus->m_itemManager->ArmEntity(gIDStore,mPtr,DBPtr,gStatus->m_3dDevice);
}

void CItemManagerDlg::InList(int sel)
{
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
     m_imItemList.ResetContent();//clear
 
	 for(unsigned int cLoop=0;cLoop<gStatus->m_itemManager->m_cellCount;cLoop++)
	 {
		 for(CAxLinkObj * pos = gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		 {
			 CItemObj * iPtr = (CItemObj *) gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
			 m_imItemList.AddString(iPtr->m_itemName);
		 }
	 }
	//internally the rest of the lsists should auto clean themselfs
	if(sel > -1)
	   m_imItemList.SetCurSel(sel);
}

void CItemManagerDlg::InList(CString sel)
{
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
     m_imItemList.ResetContent();//clear
 
	 for(unsigned int cLoop=0;cLoop<gStatus->m_itemManager->m_cellCount;cLoop++)
	 {
		 for(CAxLinkObj * pos = gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		 {
			 CItemObj * iPtr = (CItemObj *) gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
			 m_imItemList.AddString(iPtr->m_itemName);
		 }
	 }
	//internally the rest of the lsists should auto clean themselfs

	if(sel.GetLength() > 0)
	   m_imItemList.SelectString(0,sel);
}

void CItemManagerDlg::OnBnClickedButtonimimportedb()
{
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
     //use gamefiles folder for organization
     CString MainDir;
     char buffer[_MAX_PATH];
	 _getcwd( buffer, _MAX_PATH );
	 MainDir = buffer;
	
     static char BASED_CODE filter[] = "Legacy EDB format|*.EDB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   gStatus->m_itemManager->ImportEDB(fileName.GetBuffer());
       InList(-1);
	 }
     SetCurrentDirectory(MainDir);
}

void CItemManagerDlg::OnBnClickedButtonimimportapd()
{
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
     //use gamefiles folder for organization
     CString MainDir;
     char buffer[_MAX_PATH];
	 _getcwd( buffer, _MAX_PATH );
	 MainDir = buffer;
	
     static char BASED_CODE filter[] = "Legacy APD format|*.APD|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   gStatus->m_itemManager->ImportAPD(fileName.GetBuffer());
	   if(gStatus->m_itemManager)
	   if(gStatus->m_itemManager->m_visuals)
	   for(POSITION armedPos = gStatus->m_itemManager->m_visuals->GetHeadPosition();armedPos != NULL;)
	   {//appendage Loop
		  CArmedInventoryObj* armedObj = (CArmedInventoryObj*)gStatus->m_itemManager->m_visuals->GetNext(armedPos);
		  if(armedObj->m_thirdPersonSystem)//third person
			 gStatus->SkeletalTextureReassignment(armedObj->m_thirdPersonSystem);
		   //first person
		  if(armedObj->m_firstPersonSystem)//third person
			 gStatus->SkeletalTextureReassignment(armedObj->m_firstPersonSystem);
		}//end appendage loop
	 }
     SetCurrentDirectory(MainDir);

}

void CItemManagerDlg::OnBnClickedButtonimvisuals()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
	CAppendageCustDlg dlg;
	dlg.m_gStatusRef				= gStatus;
	dlg.m_renderStateObjectRef		= gStatus->m_renderStateObject;
	dlg.m_cgGlobalShaderDB			= gStatus->m_cgGlobalShaderDB;
	dlg.m_skillDatabaseREF			= gStatus->m_skillDatabase;
	dlg.m_elementDBREF				= gStatus->m_elementDB;
	dlg.g_pd3dDeviceRef				= gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.GL_textureDataBase			= gStatus->m_textureDataBase;
	dlg.m_focusMovPtr               = NULL;
    
	CMovementObj * mPtr = gStatus->m_entObjRuntime->GetObjByIndex(0);
    if(mPtr)
       dlg.m_focusMovPtr			= mPtr;
	else
	{
       AfxMessageBox("a runtime entity needs to be spawned, preferably the one that uses the attachemnt visual",0,0);
       return;
	}

    if(!gStatus->m_itemManager->m_visuals)
	    gStatus->m_itemManager->m_visuals = new CArmedInventoryList();

	dlg.m_dirSndRef					= gStatus->m_dirSnd;
	dlg.m_armedInvenListRef			= gStatus->m_itemManager->m_visuals;
	dlg.DoModal();
	dlg.m_armedInvenListRef->CompileDirectAccess();

}

void CItemManagerDlg::OnBnClickedButtonimsave()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
	if(!gStatus || !gStatus->m_itemManager)
	{
		AfxMessageBox("No item database is loaded.", MB_ICONWARNING);
		return;
	}

	CString mainDir;
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	mainDir = buffer;
	CString gameFilesDir = gStatus->GL_pathGameFiles;
	if(gameFilesDir.IsEmpty())
		gameFilesDir = mainDir + "\\GameFiles\\";
	if(!SetCurrentDirectory(gameFilesDir))
	{
		AfxMessageBox("Could not open the GameFiles folder:\n\n" + gameFilesDir, MB_ICONWARNING);
		return;
	}

	static char BASED_CODE filter[] = "Item database (*.APX)|*.APX|All Files (*.*)|*.*||";
	CFileDialog saveDialog(FALSE, "APX", "EARevolution.APX",
	                       OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
	                       filter, this);
	if(saveDialog.DoModal() == IDOK)
	{
		CString filePath = saveDialog.GetPathName();
		char * savePath = filePath.GetBuffer();
		BOOL saved = gStatus->m_itemManager->Save(savePath);
		filePath.ReleaseBuffer();
		if(!saved)
			AfxMessageBox("Could not save the item database to:\n\n" + filePath, MB_ICONWARNING);
	}
	SetCurrentDirectory(mainDir);
}

void CItemManagerDlg::OnBnClickedButtonimload()
{
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
     //use gamefiles folder for organization
	 //use gamefiles folder for organization
     CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "APX format|*.APX|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   if(gStatus->m_itemManager)
	   delete gStatus->m_itemManager;
	   gStatus->m_itemManager = 0;
	   gStatus->m_itemManager = new CItemManager();
	   gStatus->m_itemManager->Load(fileName.GetBuffer());
	   InList(-1);

	   if(gStatus->m_itemManager)
	   if(gStatus->m_itemManager->m_visuals)
	   for(POSITION armedPos = gStatus->m_itemManager->m_visuals->GetHeadPosition();armedPos != NULL;)
	   {//appendage Loop
		  CArmedInventoryObj* armedObj = (CArmedInventoryObj*)gStatus->m_itemManager->m_visuals->GetNext(armedPos);
		  if(armedObj->m_thirdPersonSystem)//third person
			 gStatus->SkeletalTextureReassignment(armedObj->m_thirdPersonSystem);
		   //first person
		  if(armedObj->m_firstPersonSystem)//third person
			 gStatus->SkeletalTextureReassignment(armedObj->m_firstPersonSystem);
		}//end appendage loop
	 }
     SetCurrentDirectory(MainDir);
}

void CItemManagerDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CItemManagerDlg::OnBnClickedButtonexporttosql()
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)globalStatusMem;
	CSqlConnectionDlg dlg;
     if(dlg.DoModal() == IDOK)
	 {
	    //get connection
	    CDBConnectionSystem * dbConnection = new CDBConnectionSystem(dlg.m_ipAddress,
			                                                         dlg.m_database,
																     dlg.m_username,
																     dlg.m_password,
																     NULL);


		if(!dbConnection->m_connectionEstablished)
		{
			delete dbConnection;
		    dbConnection = 0;
			AfxMessageBox("Connection could not be established to the database-check your connection info");
			return;
		}
		dbConnection->ExecQuery("DELETE FROM GM_GIL");//clear existing
		dbConnection->ExecQuery("DELETE FROM gm_itemVisualDump");//clear existing

        CString addItemStencil = "INSERT INTO GM_GIL(ITEMNAME,GIL) VALUES('$ITEMNAME','$GIL')";

		CString addVisual = "INSERT INTO gm_itemVisualDump(VISUALNAME) VALUES('$VISUALNAME')";

        CString tempQry;
		
		//Static APX items
        for(unsigned int cLoop=0;cLoop<gStatus->m_itemManager->m_cellCount;cLoop++)
	    {
			 for(CAxLinkObj * pos = gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
			 {
				 CItemObj * iPtr = (CItemObj *) gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
				 tempQry = addItemStencil;

				 // Sanitize the item name
				 iPtr->m_itemName.Replace("'", "''");

				 // Populate insert string with item properties
				 tempQry.Replace("$ITEMNAME",iPtr->m_itemName);
				 tempQry.Replace("$GIL",gStatus->m_cabLib.ConvToString(iPtr->m_globalID));

				 // Execute INSERT query
				 dbConnection->ExecQuery(tempQry);
			 }
	    }

		//base visuals (building blocks of all items)
		if(gStatus->m_itemManager->m_visuals)
		for(POSITION vPos = gStatus->m_itemManager->m_visuals->GetHeadPosition();vPos != NULL;)
		{
			CArmedInventoryObj * aPtr = (CArmedInventoryObj *) gStatus->m_itemManager->m_visuals->GetNext(vPos);
			tempQry = addVisual;
			tempQry.Replace("$VISUALNAME",aPtr->m_name);
			
			dbConnection->ExecQuery(tempQry);
		}

	    delete dbConnection;
		dbConnection = 0;
		AfxMessageBox("COMPLETE");
	 }
}

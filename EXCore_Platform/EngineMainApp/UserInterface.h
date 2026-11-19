#include "CAssimpModelImporter.h"

//------------------------------------------------------------------------------------//
//------toolbar functions-------------------------------------------------------------//
//ref
//m_wndDlgBar.SetDlgItemText( IDC_EDIT_FILENAME, CSTRING);
//m_wndDlgBar.SetDlgItemInt(IDC_EDIT_FILENAME,INT,TRUE);
//char * testtemp = "HI";
//m_dlgBarObject.SendDlgItemMessage(LISTBOX ID,LB_INSERTSTRING,0,(LPARAM)testtemp)
//m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
//HTREEITEM item = TreeView_GetSelection(treeHwnd); 
//HTREEITEM child1 = AddTreeChild("COMON",IDC_OBJECT_TREE,&m_dlgBarObject,item);
//function closes engine
//------Function exits game engine----------------------------------------------------//
void EngineXWin::OnButtonQuit() 
{
    //FreeAllThenClose();	
	PostMessage( WM_CLOSE );
}
//------function opens a new toolbar and closes the others----------------------------//
void EngineXWin::OpenToolBar(CDialogBar *barLoc)
{
CloseAllToolDialogs();
barLoc->ShowWindow(SW_SHOWNORMAL);
if(m_gStatus)
	barLoc->SetWindowPos(&wndTop ,m_gStatus->m_imediateRenderRECT.right,0,1024,800,SWP_SHOWWINDOW);	
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------hides all toolbars------------------------------------------------------------//
void EngineXWin::CloseAllToolDialogs()
{
m_wndDlgBar.ShowWindow(SW_HIDE);
m_dlgBarObject.ShowWindow(SW_HIDE);
m_backgroundDlgBar.ShowWindow(SW_HIDE);
m_controlsDlgBar.ShowWindow(SW_HIDE);
m_moviesDlgBar.ShowWindow(SW_HIDE);
m_entityDlgBar.ShowWindow(SW_HIDE);
m_camVptDlgBar.ShowWindow(SW_HIDE);
m_2DinterfaceBar.ShowWindow(SW_HIDE);
//m_enviormentBar.ShowWindow(SW_HIDE);
m_textFontDlgBar.ShowWindow(SW_HIDE);
//m_explosionDlgBar.ShowWindow(SW_HIDE);
m_musicDlgBar.ShowWindow(SW_HIDE);
//m_missileDlgBar.ShowWindow(SW_HIDE);
m_bulletHoleDlgBar.ShowWindow(SW_HIDE);
m_multiPlayer.ShowWindow(SW_HIDE);
m_animatedTextureDBDlgBar.ShowWindow(SW_HIDE);
m_scriptDlgBar.ShowWindow(SW_HIDE);
//m_textureDBDlgBar.ShowWindow(SW_HIDE);
m_hudDBDlgBar.ShowWindow(SW_HIDE);
m_menuDBDlgBar.ShowWindow(SW_HIDE);
m_particleDBDlgBar.ShowWindow(SW_HIDE);
//m_spawnGenDBDlgBar.ShowWindow(SW_HIDE);
//m_haloEffectsDlgBar.ShowWindow(SW_HIDE);
//m_aiDBDlgBar.ShowWindow(SW_HIDE);
m_mouseDlgBar.ShowWindow(SW_HIDE);
m_mainMenus.ShowWindow(SW_HIDE);
m_mainCharacter.ShowWindow(SW_HIDE);
m_mainEnvironment.ShowWindow(SW_HIDE);
m_mainFX.ShowWindow(SW_HIDE);
g_editorHighlightTraceId     = -1;
g_editorHighlightGroupId     = -99999;
g_editorHighlightEntityIndex = -1;
if(m_gStatus)
	m_gStatus->m_dVectors.Clear();
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------function saves scene----------------------------------------------------------//
void EngineXWin::OnButtonSave() 
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
static char BASED_CODE filter[] = "EXG ENGIN|*.EXG|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "EXG", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SaveScene(fileName);
	   m_gStatus->m_currentWorkingFile = fileName;
	   m_wndDlgBar.SetDlgItemText( IDC_EDIT_FILENAME, m_gStatus->m_currentWorkingFile);
	 }
	 SetCurrentDirectory(MainDir);
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------loads a scene file------------------------------------------------------------//
void EngineXWin::OnButtonLoad() 
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

   static char BASED_CODE filter[] = "EXG Files|*.EXG|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadNewWorld=TRUE;
       m_gStatus->NewWorld=fileName;
	   m_gStatus->m_currentWorkingFile = fileName;
	   m_wndDlgBar.SetDlgItemText( IDC_EDIT_FILENAME, m_gStatus->m_currentWorkingFile);
	   m_refreshInterface = TRUE;
	 }
     SetCurrentDirectory(MainDir);
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------switches tool bar to object edit editor---------------------------------------//
void EngineXWin::OnButtonSwitchObject() 
{
	OpenToolBar(&m_dlgBarObject);	
}
//switch to main
void EngineXWin::OnButtonMenu() 
{
	OpenToolBar(&m_wndDlgBar);	
}
//switch to background editor
void EngineXWin::OnBUTTONBKEDITORswtch() 
{
//OpenToolBar(&m_backgroundDlgBar);
	CPatchSystemUpdateDlg dlg;
	dlg.DoModal();
}
//switch to controls
void EngineXWin::OnButtonControls() 
{
OpenToolBar(&m_controlsDlgBar);	
}
//switch to movies
void EngineXWin::OnButtonMovies() 
{
	OpenToolBar(&m_moviesDlgBar);		
}
//switch to entities
void EngineXWin::OnButtonSwEntities() 
{
    OpenToolBar(&m_entityDlgBar);	
}
//switch to Environment
void EngineXWin::OnBTNMainEnvironment() 
{
    OpenToolBar(&m_mainEnvironment);	
}
//switch to Character
void EngineXWin::OnBTNMainCharacter() 
{
    OpenToolBar(&m_mainCharacter);	
}
//switch to Menus
void EngineXWin::OnBTNMainMenus() 
{
    OpenToolBar(&m_mainMenus);	
}
//switch to FX
void EngineXWin::OnBTNMainFX() 
{
    OpenToolBar(&m_mainFX);	
}
void EngineXWin::OnBTNCameraViewportsSw() 
{
	 OpenToolBar(&m_camVptDlgBar);	
}
void EngineXWin::OnBTN2DINTERFACEsw() 
{
	#ifndef _ClientOutput 
    CShaderLibDlg * dlg = new CShaderLibDlg();
	dlg->m_gstatusMem = m_gStatus;
	dlg->m_shaderLibRef = m_gStatus->m_pxShaderSystem;
	dlg->m_pd3dDevice = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg->m_shaderTypeMode = 1;
	//dlg.DoModal();
	
     dlg->Create(IDD_DIALOG_ShaderLibEditor,NULL);
     dlg->ShowWindow(SW_SHOW);


    /*CShaderLibDlg dlg;
	dlg.m_gstatusMem = m_gStatus;
	dlg.m_shaderLibRef = m_gStatus->m_pxShaderSystem;
	dlg.m_pd3dDevice = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.m_shaderTypeMode = 1;
	dlg.DoModal();
	m_gStatus->m_pxShaderSystem = dlg.m_shaderLibRef;
    m_gStatus->m_pxShaderSystem->Compile(m_gStatus->m_dxRenderer->m_pd3dDevice);
	m_gStatus->m_renderStateObject->m_pxShderSystemRef = m_gStatus->m_pxShaderSystem;*/
    #endif
}

///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	OnBUTTONEnviormentSw()
// Description:	Show the Environment Settings dialog control.
///////////////////////////////////////////////////////////////////////////////

void
EngineXWin::OnBUTTONEnviormentSw() 
{
	/* DEPRECATED /////////////////////////////////////////////////////////////
	OpenToolBar(&m_enviormentBar);	
	RefreshEnviorment();
	/////////////////////////////////////////////////////////////////////////*/

	// Delete Environment Settings dialog if it already exists
	if(this->m_pEnvironmentSettingsDlg) 
	{
		delete this->m_pEnvironmentSettingsDlg;
		this->m_pEnvironmentSettingsDlg = 0;
	}

	// Instantiate new Environment Settings dialog
	this->m_pEnvironmentSettingsDlg = new CEnvironmentSettingsDlg();
	this->m_pEnvironmentSettingsDlg->Create(IDD_DIALOG_ENVIORMENT, 0);
	this->m_pEnvironmentSettingsDlg->Status(m_gStatus);
	this->m_pEnvironmentSettingsDlg->ObjectAddDialog(&m_dlgBarObject);
	this->m_pEnvironmentSettingsDlg->RefreshEnviorment();
	this->m_pEnvironmentSettingsDlg->ShowWindow(1);
}

void EngineXWin::OnBTNtextFontSW() 
{
     OpenToolBar(&m_textFontDlgBar);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	OnBTNExplosionDBSW()
// Description:	Show the Explosion Database dialog control.
///////////////////////////////////////////////////////////////////////////////

void 
EngineXWin::OnBTNExplosionDBSW() 
{
	/* DEPRECATED /////////////////////////////////////////////////////////////
	OpenToolBar(&m_explosionDlgBar);
	/////////////////////////////////////////////////////////////////////////*/

	// Delete Explosions Database dialog if it already exists
	if(this->m_pExplosionsDbDlg) 
	{
		delete this->m_pExplosionsDbDlg;
		this->m_pExplosionsDbDlg = 0;
	}

	// Instantiate new Explosions Database dialog
	this->m_pExplosionsDbDlg = new CExplosionsDatabaseDlg();
	this->m_pExplosionsDbDlg->Status(m_gStatus);
	this->m_pExplosionsDbDlg->Create(IDD_DIALOG_EXPLOSIONS,0);
	this->m_pExplosionsDbDlg->ShowWindow(1);
}

void EngineXWin::OnBTNUIMusic() 
{
     OpenToolBar(&m_musicDlgBar);
}
void EngineXWin::OnBTNUIMissileDatabase() 
{
	/* DEPRECATED /////////////////////////////////////////////////////////////
	OpenToolBar(&m_missileDlgBar);
	/////////////////////////////////////////////////////////////////////////*/

	// Delete Weapon System dialog if it already exists
	if(this->m_pWeaponSystemDlg) 
	{
		delete this->m_pWeaponSystemDlg;
		this->m_pWeaponSystemDlg = 0;
	}

	// Instantiate new Explosions Database dialog
	this->m_pWeaponSystemDlg = new CWeaponSystemDlg();
	this->m_pWeaponSystemDlg->Status(m_gStatus);
	this->m_pWeaponSystemDlg->Create(IDD_DIALOG_MISSILE_Interface,0);
	this->m_pWeaponSystemDlg->ShowWindow(1);
}
void EngineXWin::OnBTNUIBulletHole() 
{
     OpenToolBar(&m_bulletHoleDlgBar);	
}
void EngineXWin::OnBTNUIMultiplayer() 
{
     OpenToolBar(&m_multiPlayer);	
}
void EngineXWin::OnBTNUIAnimTextureDB() 
{
	OpenToolBar(&m_animatedTextureDBDlgBar);		
}
void EngineXWin::OnBtnUiScript() 
{
	OpenToolBar(&m_scriptDlgBar);	
}
///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	OnBTNUITextureAssign()
// Description:	Show the Texture Database dialog control.
///////////////////////////////////////////////////////////////////////////////

void EngineXWin::OnBTNUITextureAssign() 
{
	/* DEPRECATED /////////////////////////////////////////////////////////////
	OpenToolBar(&m_textureDBDlgBar);	
	/////////////////////////////////////////////////////////////////////////*/

	// Delete Texture Database dialog if it already exists
	if(this->m_pTextureDatabaseDlg) 
	{
		delete this->m_pTextureDatabaseDlg;
		this->m_pTextureDatabaseDlg = 0;
	}

	// Instantiate new AI System dialog
	this->m_pTextureDatabaseDlg = new CTextureDatabaseDlg();
	this->m_pTextureDatabaseDlg->Status(m_gStatus);
	this->m_pTextureDatabaseDlg->Create(IDD_DIALOG_TEXTUREDATA, 0);
	this->m_pTextureDatabaseDlg->ShowWindow(1);
}

void EngineXWin::OnBTNUIHudInterface() 
{
	#ifndef _ClientOutput 
	 CMaterialShaderLibDlg dlg;
     dlg.cablib = &m_gStatus->m_cabLib;
	 dlg.m_pixelShaderLibRef = m_gStatus->m_pxShaderSystem;
	 dlg.m_vertexShaderLibRef = m_gStatus->m_shaderSystem;
	 dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
     dlg.m_globalShaderRef = m_gStatus->m_cgGlobalShaderDB;
	 dlg.DoModal();
	 m_gStatus->m_cgGlobalShaderDB = dlg.m_globalShaderRef;
	 m_gStatus->m_cgGlobalShaderDB->ScaleForRuntimeCompatibility(m_gStatus->m_renderStateObject->m_supportBUMPENVMAP,m_gStatus->m_renderStateObject->m_maxPixShaderVs,m_gStatus->m_renderStateObject->m_maxVSShaderVs);

    #endif
}
void EngineXWin::OnBTNUIMenuSystem() 
{
	OpenToolBar(&m_menuDBDlgBar);
}
void EngineXWin::OnBTNUIParticleSys() 
{
	//OpenToolBar(&m_particleDBDlgBar);

	 CParticleInterfaceDlg * particleDlg = new CParticleInterfaceDlg();
	 particleDlg->m_gStatusMem = m_gStatus;
	 particleDlg->Create(IDD_DIALOG_ParticleInterface,NULL);
	 particleDlg->ShowWindow(SW_SHOW);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	OnBTNUISpawnGen()
// Description:	Show the Spawn Generator dialog control.
///////////////////////////////////////////////////////////////////////////////

void EngineXWin::OnBTNUISpawnGen() 
{
	/* DEPRECATED /////////////////////////////////////////////////////////////
	OpenToolBar(&m_spawnGenDBDlgBar);	
	/////////////////////////////////////////////////////////////////////////*/

	// Delete Spawn Generator dialog if it already exists
	if(this->m_pSpawnGeneratorDlg) 
	{
		delete this->m_pSpawnGeneratorDlg;
		this->m_pSpawnGeneratorDlg = 0;
	}

	// Instantiate new AI System dialog
	if(!this->m_pAISystemDlg) 
	{
		this->OnBtnUiAi();
		this->m_pAISystemDlg->ShowWindow(SW_HIDE);
	}

	// Instantiate new AI System dialog
	this->m_pSpawnGeneratorDlg = new CSpawnGeneratorDlg();
	this->m_pSpawnGeneratorDlg->Status(m_gStatus);
	this->m_pSpawnGeneratorDlg->EntityDlg(&this->m_entityDlgBar);
	this->m_pSpawnGeneratorDlg->AISystemDlg(this->m_pAISystemDlg);
	this->m_pSpawnGeneratorDlg->Create(IDD_DIALOG_SpawnGeneratorUI, 0);
	this->m_pSpawnGeneratorDlg->ShowWindow(1);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	OnBtnUiAi()
// Description:	Show the AI System dialog control.
///////////////////////////////////////////////////////////////////////////////

void EngineXWin::OnBtnUiAi() 
{
	/* DEPRECATED /////////////////////////////////////////////////////////////
	OpenToolBar(&m_aiDBDlgBar);
	/////////////////////////////////////////////////////////////////////////*/

	// Delete AI System dialog if it already exists
	if(this->m_pAISystemDlg) 
	{
		delete this->m_pAISystemDlg;
		this->m_pAISystemDlg = 0;
	}

	// Instantiate new AI System dialog
	this->m_pAISystemDlg = new CAISystemDlg();
	this->m_pAISystemDlg->Status(m_gStatus);
	this->m_pAISystemDlg->Create(IDD_DIALOG_AI_BOTS,0);
	this->m_pAISystemDlg->ShowWindow(1);
}

void EngineXWin::OnBTNUIHaloEffects() 
{
	CeffectSystemDlg dlg;
	dlg.m_selectionEffectDBRef = m_gStatus->m_selectionEffectDB;
	dlg.m_gStatus = m_gStatus;
	dlg.m_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.m_renderStateObjectRef = m_gStatus->m_renderStateObject;
	dlg.m_cgGlobalShaderDB = m_gStatus->m_cgGlobalShaderDB;
	dlg.m_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.m_dirSndRef = m_gStatus->m_dirSnd;
	dlg.m_elementDBREF = m_gStatus->m_elementDB;
	dlg.m_skillDatabaseREF = m_gStatus->m_skillDatabase;
	dlg.DoModal();
}

//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------adds a item as a root object--------------------------------------------------//
HTREEITEM EngineXWin::AddTreeRoot(CString name,UINT id,CDialogBar *barLoc)
{
	const char * nameLoc = (LPCTSTR)name;
	TV_INSERTSTRUCT TreeCtrlItem;
    TreeCtrlItem.hParent = TVI_ROOT;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
	TreeCtrlItem.item.pszText = (char *)nameLoc;
	
    

	//TreeCtrlItem.item.lParam = 0;
	HTREEITEM hTreeItem1 = (HTREEITEM) barLoc->SendDlgItemMessage(id,TVM_INSERTITEM,0,(LPARAM)(LPTV_INSERTSTRUCT)&TreeCtrlItem);
    return hTreeItem1;
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------function adds a child to a tree item------------------------------------------//
HTREEITEM EngineXWin::AddTreeChild(CString name,UINT id,CDialogBar *barLoc,HTREEITEM treeItem)
{
	const char * nameLoc = (LPCTSTR)name;
	TV_INSERTSTRUCT TreeCtrlItem;
	TreeCtrlItem.hInsertAfter = TVI_LAST;
	TreeCtrlItem.item.mask = TVIF_TEXT | TVIF_PARAM;
	TreeCtrlItem.hParent = treeItem;
	TreeCtrlItem.item.pszText = (char *)nameLoc;
	//TreeCtrlItem.item.lParam = 1;	
	HTREEITEM hTreeItem1 = (HTREEITEM) barLoc->SendDlgItemMessage(id,TVM_INSERTITEM,0,(LPARAM)(LPTV_INSERTSTRUCT)&TreeCtrlItem);
    barLoc->SendDlgItemMessage(id,TVM_ENSUREVISIBLE,0,(LPARAM)hTreeItem1);
	return hTreeItem1;
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------retrieves text from a selected object from a tree-----------------------------//
CString EngineXWin::GetTreeItemText(UINT id,CDialogBar *barLoc,HTREEITEM treeItem,int maxLen)
{
if(id == IDC_OBJECT_TREE && barLoc)
{
   HWND treeHwnd = NULL;
   barLoc->GetDlgItem(IDC_OBJECT_TREE, &treeHwnd);
   if(treeHwnd)
   {
      if(!treeItem)
         treeItem = TreeView_GetSelection(treeHwnd);
      if(treeItem)
      {
         HTREEITEM parentItem = TreeView_GetParent(treeHwnd, treeItem);
         if(parentItem)
            treeItem = parentItem;
      }
   }
   if(maxLen < 32)
      maxLen = 32;
}
char buffer[256];
ZeroMemory(buffer, sizeof(buffer));
if(maxLen > (int)sizeof(buffer) - 1)
   maxLen = (int)sizeof(buffer) - 1;
TVITEM tvItem;
ZeroMemory(&tvItem, sizeof(tvItem));
tvItem.mask = TVIF_TEXT;
tvItem.hItem = treeItem;
tvItem.cchTextMax = maxLen;
tvItem.pszText = buffer;
BOOL res = barLoc->SendDlgItemMessage(id,TVM_GETITEM,0,(LPARAM)(TVITEM*)&tvItem);
if(res == FALSE)
   return "NONE";
CString tempLocal = tvItem.pszText;
return tempLocal;
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------retrieves text from a selected object from a tree-----------------------------//
BOOL EngineXWin::SetTreeItemText(CString newText,UINT id,CDialogBar *barLoc,HTREEITEM treeItem,int maxLen)
{
char buffer[500];
TVITEM tvItem; 
tvItem.mask = TVIF_TEXT;
tvItem.hItem = treeItem;
tvItem.cchTextMax = maxLen;
tvItem.pszText = buffer;
BOOL res = barLoc->SendDlgItemMessage(id,TVM_GETITEM,0,(LPARAM)(TVITEM*)&tvItem);
if(res == FALSE)
   return FALSE;
const char * nameLoc = (LPCTSTR)newText;
tvItem.pszText = (char *)nameLoc;
res = barLoc->SendDlgItemMessage(id,TVM_SETITEM,0,(LPARAM)(TVITEM*)&tvItem);
return TRUE;
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------test obsolete-----------------------------------------------------------------//
void EngineXWin::OnButtonTreetest()
{
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------function adds a model from file-----------------------------------------------//
void EngineXWin::OnButtonObjectload() 
{
	//If they don't have a group selected, return
	int menuIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box, LB_GETCURSEL, 0, 0);
	if(menuIndex == -1)
	{
		MessageBox("You must select a group first.");
		return;
	}

	//char temp;
	char bigBuff[20000] = "";  // maximum common dialog buffer size
	//use mapsmodels folder for organization
	CString CurDir;
	CString MainDir;
	CString Mdir;
	char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 Mdir = CurDir = CurDir+"\\MapsModels\\";
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
		  int loop=0;
	      for(loop=0;loop<15000;loop++){
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
		  SetCurrentDirectory(MainDir);
	      //---------end-build selected files database------------//
		  for(loop=1;loop<counter;loop++){//begin file load loop
		      CWldObject * retBasisLoc = NULL;
			  SetCurrentDirectory(CurDir);//set to base location
			  LoadWorldObject(FileNames[loop],&retBasisLoc);	
			  SetCurrentDirectory(Mdir);//set to maps and models
			   if(retBasisLoc)//valid
			       m_gStatus->AddTextureToDatabase(retBasisLoc->m_meshObject);//apply mapping and add to DB
		  }
	  }// end file chosen		  
SetCurrentDirectory(MainDir);
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------deletes a object based on the selected item in the object tree----------------//
void EngineXWin::OnButtonObjectdelete()
{
HWND treeHwnd = NULL;
m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd);
HTREEITEM rootItem = (treeHwnd && item) ? TreeView_GetParent(treeHwnd, item) : NULL;
if(!rootItem) rootItem = item;
CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,rootItem,10);
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){
	if(g_editorHighlightTraceId == ptrLoc->List_identity)
	{
		g_editorHighlightTraceId = -1;
		m_gStatus->m_dVectors.Clear();
	}
	DeleteObjectByTrace(ptrLoc->List_identity);
	m_gStatus->m_resetReferences = TRUE;
	m_gStatus->m_skeletalReferenceObjCache.RemoveAll();
	m_dlgBarObject.SendDlgItemMessage(IDC_OBJECT_TREE,TVM_DELETEITEM,0,(LPARAM) (HTREEITEM)rootItem);
    //m_gStatus->m_worldRenderList->RemoveAll();
	return;
	}
  }//end Begin world object loop
  //m_gStatus->m_worldRenderList->RemoveAll();
  MessageBox("Object Not Found");
}


//-------------------------End Function-----------------------------------------------//

///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	GetTreeChildIndex(HWND, HTREEITEM) 
// Description:	Returns the index of the selected item as the number in its list.
///////////////////////////////////////////////////////////////////////////////

int
EngineXWin::GetTreeChildIndex(HWND hTree, HTREEITEM hItem) 
{
	int iLocation = 0;
	HTREEITEM hItemLoc = hItem;
	while(hItemLoc != NULL)
	{	   
		hItemLoc = TreeView_GetPrevSibling(hTree, hItemLoc);
		if(hItemLoc) ++iLocation;
	}
	return iLocation;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------this function inlists all objects in tree view of objects---------------------//
void EngineXWin::ObjectTreeViewInlist(int groupNum,int traceNum) 
{
if(!m_gStatus->m_wldObjList)
return;

  
//delete all existing enries in tree
  int traceSel=-1;
  HTREEITEM track;
  m_dlgBarObject.SendDlgItemMessage(IDC_OBJECT_TREE,TVM_DELETEITEM,0,(LPARAM) (HTREEITEM)TVI_ROOT);
  //in list all new entries
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
   CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
   BOOL logIt = TRUE;
   
   
   if(groupNum == -9998){
	  logIt = FALSE;

	  if(ptrLoc->m_filterGroup != -1)
		 logIt = TRUE;

   }
   
   
   if(groupNum == -9999){
	    logIt = FALSE;
   
		if(ptrLoc->m_particleLink != -1)
           logIt = TRUE;

		if(ptrLoc->m_filterGroup != -1)
		   logIt = TRUE;

        if(ptrLoc->List_usedAsSpawnPoint != -1)
		   logIt = TRUE;
        if(ptrLoc->List_triggerList)
		   if(ptrLoc->List_triggerList->GetCount() > 0)
		   logIt = TRUE;
		if(ptrLoc->m_lightObject)
		   logIt = TRUE;
		if(ptrLoc->List_removeEnviorment)
		   logIt = TRUE;

        if(ptrLoc->m_meshObject)
		   if(ptrLoc->m_meshObject->m_charona)
			  logIt = TRUE;

   }
   if(logIt == TRUE)
   if(ptrLoc->List_groupNumber == groupNum || groupNum == -9999 || groupNum == -9998){//filter only group
	HTREEITEM Root = AddTreeRoot(operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity))),IDC_OBJECT_TREE,&m_dlgBarObject);   
            HTREEITEM child1;
            if(ptrLoc->m_node){//node conversion
               POSITION nodeLoc = m_gStatus->m_libraryReferenceDB->FindIndex(ptrLoc->m_node->m_libraryReference);
	           if(nodeLoc)
			   {
                   CReferenceObj * refObj = (CReferenceObj *)m_gStatus->m_libraryReferenceDB->GetAt(nodeLoc);
				   CString build = "N:";
                   build = operator+(build,m_gStatus->m_cabLib.ConvToString(ptrLoc->m_node->m_libraryReference));
                   ptrLoc->m_fileNameRef = operator+(build,refObj->m_referenceName);
			   }
			}

			//optional padding
			CString extraPaddingTexture;
			if(ptrLoc->m_meshObject)
			   extraPaddingTexture = "."+ptrLoc->m_meshObject->List_storedTextureFromFile[0];

			child1 = AddTreeChild(ptrLoc->m_fileNameRef+extraPaddingTexture,IDC_OBJECT_TREE,&m_dlgBarObject,Root);
			if(traceNum == ptrLoc->List_identity){
			   traceSel = traceNum;
			   track = child1;
			}
   }//end filter only group
  }
  g_editorHighlightGroupId = groupNum;
  g_editorHighlightTraceId = -1;
  if(traceSel != -1){
			   g_editorHighlightTraceId = traceSel;
			   HWND treeHwnd;
			   m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
               TreeView_SelectItem(treeHwnd,track);
			   VisualPropertiesInList();
  }
  else
  {
      UpdateEditorObjectHighlight(m_gStatus, TRUE, FALSE);
  }

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------this function inlists all objects in tree view of objects---------------------//
void EngineXWin::GetEnviormentFilter() 
{
if(!m_gStatus->m_wldObjList)
return;

POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(0);
if(!posLoc)
    return;
    CMovementObj * entityPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
//delete all existing enries in tree
  int traceSel=-1;
  HTREEITEM track;
  m_dlgBarObject.SendDlgItemMessage(IDC_OBJECT_TREE,TVM_DELETEITEM,0,(LPARAM) (HTREEITEM)TVI_ROOT);
  //in list all new entries
  for(posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
   CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
   BOOL logIt = FALSE;
   
   if(ptrLoc->List_removeEnviorment)
   {
	   D3DVECTOR position;
       entityPtr->List_baseFrame->GetPosition(NULL,&position);
	   BNDBOX boxA,boxB;
	   boxA.max.x = position.x + 1.0f;
	   boxA.max.y = position.y + 1.0f;
	   boxA.max.z = position.z + 1.0f;
	   boxA.min.x = position.x - 1.0f;
	   boxA.min.y = position.y - 1.0f;
	   boxA.min.z = position.z - 1.0f;

	   
	   
      if(m_gStatus->m_collisionBase->BoundingBoxToBoxCheck(boxA,ptrLoc->List_popoutBox))
         logIt = TRUE;
   }


   if(logIt == TRUE){
	HTREEITEM Root = AddTreeRoot(operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity))),IDC_OBJECT_TREE,&m_dlgBarObject);   
            HTREEITEM child1;
			child1 = AddTreeChild(ptrLoc->m_fileNameRef,IDC_OBJECT_TREE,&m_dlgBarObject,Root);
		
   }//end filter only group
  }
 

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Create Background object------------------------------------------------------//
BOOL EngineXWin::CreateBackGroundObject(CString bitmapName,int xMax, int yMax,int startX,int startY,RECT usageRect,BOOL movingMap)
{//BEGIN
/*CBackGroundObj * ptrLoc = new CBackGroundObj(NULL,//main surface init
											 bitmapName,//bitmap file name
		                                     xMax,//represents the x pixel count of source image
		                                     yMax,//represents the y pixel count of source image			
		                                     usageRect,//represents the rect of what is used of the source image
	                                         startX,//position X of offset that defines location
			                                 startY,//position Y of offset that defines location
			                                 movingMap,
											 0,//animate parameter
											 0//animate parameter
											 );
m_gStatus->m_backGroundList->AddTail(ptrLoc);*/

return TRUE;
}//END
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------initialize background objects-------------------------------------------------//
BOOL EngineXWin::InitializeBackground(int numberOfInitialization)
{//BEGIN
//DirectDraw 7 background-surface initialization removed with the rest of the DDraw
//pipeline - 2D backgrounds are handled by the D3D9 renderer.
return FALSE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------------runs a background--------------------------------------------//
BOOL EngineXWin::BackgroundInLoop(int inUse)
{//BEGIN
//DirectDraw 7 back-surface blit removed with the rest of the DDraw pipeline.
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Scrolls Bacroung Right--------------------------------------------------------//
void EngineXWin::BackGroundScrollRight(int pixelIndex,int listIndex)
{//BEGIN
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Scrolls Bacroung left--------------------------------------------------------//
void EngineXWin::BackGroundScrollLeft(int pixelIndex,int listIndex)
{//BEGIN

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Scrolls Bacroung Down--------------------------------------------------------//
void EngineXWin::BackGroundScrollDown(int pixelIndex,int listIndex)
{//BEGIN
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Scrolls Background Up---------------------------------------------------------//
void EngineXWin::BackGroundScrollUp(int pixelIndex,int listIndex)
{//BEGIN

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnButtonDelete() 
{
	DeleteBackground();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----Functions are movement buttons for tesing single step background movement-------//
void EngineXWin::OnButtonMvUp() 
{
	BackGroundScrollUp(5,0);
}

void EngineXWin::OnButtonMvDown() 
{
	BackGroundScrollDown(5,0);
}

void EngineXWin::OnButtonMvLeft() 
{
	BackGroundScrollLeft(5,0);
}

void EngineXWin::OnButtonMvRight() 
{
	BackGroundScrollRight(5,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------Adds a static non movable background-------------------------------//
void EngineXWin::OnButtonAddnewstaticBk() 
{
	DeleteBackground() ;
//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\MapsModels\\";
			 SetCurrentDirectory(CurDir);
 //end use mapsmodels folder for organization
 static char BASED_CODE filter[] = "Texture Files|*.tga;*.bmp;*.png;*.dds;*.jpg;*.jpeg|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
       m_backgroundDlgBar.SetDlgItemText(IDC_EDIT_BackgroundVsFile,fileName); 
       
       RECT usageRectLoc;
       usageRectLoc.left = 0;
       usageRectLoc.top = 0;
       usageRectLoc.right = 0;
       usageRectLoc.bottom = 0;
       CreateBackGroundObject(fileName,
					          0, 
					          0,
					          0,
					          0,
					          usageRectLoc,
							  FALSE);
       SetCurrentDirectory(MainDir);
	   InitializeBackground(0);	
	 }
     SetCurrentDirectory(MainDir);
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------Used to remove a background--------------------------------------//
void EngineXWin::DeleteBackground() 
{
/*POSITION posLoc, posLoc2;
if(!m_gStatus->m_backGroundList)
return;
for(posLoc = m_gStatus->m_backGroundList->GetHeadPosition();(posLoc2 = posLoc) != NULL;){//Begin background loop
        CBackGroundObj * ptrLoc = (CBackGroundObj*) m_gStatus->m_backGroundList->GetNext(posLoc);
        if(ptrLoc->List_mainSurface){
           ptrLoc->List_mainSurface->Release();
		   ptrLoc->List_mainSurface=0;
		}
	    CBackGroundObj * delLoc = (CBackGroundObj*) m_gStatus->m_backGroundList->GetAt(posLoc2);
		delete delLoc;
		delLoc=0;
	}
	m_gStatus->m_backGroundList->RemoveAll();
	m_gStatus->m_blackOut = TRUE;
	if(m_gStatus->m_editorModeOn == FALSE)
		return;
	//update visual data
    m_backgroundDlgBar.SetDlgItemInt(IDC_EDIT_USAGERECTX,0,TRUE);
    m_backgroundDlgBar.SetDlgItemInt(IDC_EDIT_USAGERECTY,0,TRUE);
    m_backgroundDlgBar.SetDlgItemInt(IDC_EDIT_BackgroundAnimateX,0,TRUE);
    m_backgroundDlgBar.SetDlgItemInt(IDC_EDIT_BackgroundAnimateY,0,TRUE);
    m_backgroundDlgBar.SetDlgItemText(IDC_EDIT_BackgroundVsFile,"NONE"); 
    //end update visual data*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------Applys advanced settings-----------------------------------------//
void EngineXWin::OnBUTTONBKSETTINGSapply() 
{
	/*POSITION posLoc = m_gStatus->m_backGroundList->FindIndex(0);
    if(posLoc == NULL)
       return;//didnt find index
       CBackGroundObj * ptrLoc = (CBackGroundObj*) m_gStatus->m_backGroundList->GetAt(posLoc);
	   ptrLoc->List_animX = m_backgroundDlgBar.GetDlgItemInt(IDC_EDIT_BackgroundAnimateX,NULL,TRUE);
	   ptrLoc->List_animY = m_backgroundDlgBar.GetDlgItemInt(IDC_EDIT_BackgroundAnimateY,NULL,TRUE);*/
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Test a movie------------------------------------------------------------------//
void EngineXWin::OnButtonTestmovie() 
{//use mapsmodels folder for organization
 CString CurDir;
 CString MainDir;
 char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\Movies\\";
			 SetCurrentDirectory(CurDir);
 //end use mapsmodels folder for organization	
	static char BASED_CODE filter[] = "Avi Files|*.AVI|MPEG Files|*.Mpg|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {//file chosen
	      CString fileName = opendialog.GetFileName();
		  SetCurrentDirectory(MainDir);
	      m_gStatus->PlayMoveFullscreen(fileName);
	 }//end file chosen
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------updates controller stuff------------------------------------------------------//
void EngineXWin::UpdateControlList()
{
   
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------test the effect on the force controller---------------------------------------//
void EngineXWin::OnButtonTestEffect() 
{

}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------init forcefeed back joysticks-------------------------------------------------//
BOOL EngineXWin::ForceFeedbackInit() 
{
   
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Stops all effects on force feed joysticks-------------------------------------//
void EngineXWin::StopAllForceEffects() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Stops all effects on force feed joysticks-------------------------------------//
void EngineXWin::OnButtonStopEffect() 
{
 StopAllForceEffects() ;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------saves scene-------------------------------------------------------------------//
BOOL EngineXWin::SaveScene(CString fileName)
{
//preliminary wrapper
CPersistObjList * persistList = NULL;
GeneratePersistInfo(&persistList);
//end preliminary wrapper
   CFileException exc;
   CFile fl;
   CString reserved = "II";
   int reservedInt = 9;
   fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
   CArchive the_out_Archive(&fl,CArchive::store);
   the_out_Archive << m_gStatus->m_wldObjList 
	               << m_gStatus->m_backGroundList 
				   << m_gStatus->m_environmentPtr
				   << persistList
				   << m_gStatus->m_currentTrack
				   << m_gStatus->m_musicLoopFile
				   << m_gStatus->m_globalWorldShader
				   << m_gStatus->W_BufferPreffered
				   << m_gStatus->m_sceneScriptExec
				   << reserved
				   << m_gStatus->m_spawnGeneratorDB
				   << m_gStatus->m_collisionBase
				   << m_gStatus->m_worldGroupList
				   << m_gStatus->GL_aiWebDatabase
				   << m_gStatus->m_libraryReferenceDB
				   << m_gStatus->m_waterZoneDB;
  
   the_out_Archive.Close();
   fl.Close();
//delete temp wrappers
DestroyPersistInfo(&persistList);
//end delete temp wrappers
  return TRUE;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------maintain animations-----------------------------------------------------------//
BOOL EngineXWin::IndexAnimations(int currentTickCount)
{//BEGIN 

return TRUE;
}//END




//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------creates a new entity movement object----------------------------------//
void EngineXWin::OnBtnEntityCreatenew() 
{
#ifndef _ClientOutput
    CAddEntityDlg dlg;
	dlg.saveCollisionInfo = TRUE;
    if(dlg.DoModal()!=IDOK)
	   return;
    CMovementObj * mPtr = new CMovementObj(dlg.m_entityName,//name
										  (m_gStatus->GlobalTraceNumber++));//secondary collision file
										   
	m_gStatus->m_entObjList->AddTail(mPtr);
    mPtr->LoadFromDir(dlg.m_pathGlobal,dlg.m_folderPath,m_gStatus->m_dxRenderer->m_pd3dDevice,TRUE);
	InitFXSahdersOnSkeletal(mPtr->List_skeletalObject);
	m_gStatus->SkeletalTextureReassignmentWCompile(mPtr->List_skeletalObject);
	
	mPtr->m_uniqueId = m_gStatus->m_entObjList->GetNextHighID();
	  
	HTREEITEM item = AddTreeRoot(operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId)),IDC_TREE_ENTITIES,&m_entityDlgBar);
    AddTreeChild(mPtr->List_skeletalObject->m_SkeletalName,IDC_TREE_ENTITIES,&m_entityDlgBar,item);	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------init FX shader onto skeletal------------------------------------------//
void EngineXWin::InitFXSahdersOnSkeletal(CSkeletonObject * skeletal)
{
     if(!skeletal)
		 return;


	 if(skeletal->m_bonesList)
	    skeletal->m_bonesList->CompileDirectAccess();
     if(skeletal->m_alternateConfigurations)
     for(POSITION posLoc = skeletal->m_alternateConfigurations->GetHeadPosition();posLoc != NULL;)
     {
       CAlterUnitDBObject * aPtr = (CAlterUnitDBObject *)skeletal->m_alternateConfigurations->GetNext(posLoc);
       for(POSITION cfgPos = aPtr->m_configurations->GetHeadPosition();cfgPos != NULL;)
	   {
           CDeformableMesh * dMesh = (CDeformableMesh *)aPtr->m_configurations->GetNext(cfgPos);
		   for(POSITION lPos = dMesh->m_lodChain->GetHeadPosition();lPos != NULL;)
		   {
	           CDeformableVisObj * vObj = (CDeformableVisObj *) dMesh->m_lodChain->GetNext(lPos);
			   //skeletal->GenerateTangents(m_gStatus->m_dxRenderer->m_pd3dDevice,vObj);
			   
			   if(vObj->m_mesh->List_materialObject->m_peramCount > 0)
			   {
                  vObj->m_mesh->List_materialObject->m_pixelShader = m_gStatus->m_pxShaderSystem->LoadShaderByStringData(vObj->m_mesh->List_materialObject->m_fxBindString,m_gStatus->m_dxRenderer->m_pd3dDevice,vObj->m_mesh->List_materialObject->m_fxBindString);
			   }
		   }
	   }
     }
	 for(POSITION posLoc = skeletal->m_bonesList->GetHeadPosition();posLoc != NULL;)
     {
         CBoneObject * bonePtr = (CBoneObject *)skeletal->m_bonesList->GetNext(posLoc);
         if(bonePtr->m_hiarchyMesh)
		  for(POSITION hPos = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
		  {
			  CHiarcleVisObj * hVisObj = (CHiarcleVisObj *)bonePtr->m_hiarchyMesh->m_lodChain->GetNext(hPos);  
			  //hVisObj->m_mesh->m_abVertexArray.GenerateTangents(m_gStatus->m_dxRenderer->m_pd3dDevice);
			  if(hVisObj->m_mesh->List_materialObject->m_peramCount > 0)
			  {
                  hVisObj->m_mesh->List_materialObject->m_pixelShader = m_gStatus->m_pxShaderSystem->LoadShaderByStringData(hVisObj->m_mesh->List_materialObject->m_fxBindString,m_gStatus->m_dxRenderer->m_pd3dDevice,hVisObj->m_mesh->List_materialObject->m_fxBindString);
			  }
		  }
	 }




}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Activates test on entity interface------------------------------------//
void EngineXWin::OnBtnEntityActivate() 
{
#ifndef _ClientOutput
int indexLoc=0;
BOOL foundIt = FALSE; 
   //get selected index
    HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    temp.Replace("TRC_","");
	CMovementObj * mPtr = m_gStatus->m_entObjList->GetByID(atoi(temp));
    
    /*int tracerX = 0;
    for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        //if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->List_traceNumber))) == temp){
		temp.Replace("TRC_","");
		
		
		if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(tracerX))) == temp){
           foundIt = TRUE;
		   indexLoc = mPtr->m_uniqueId;
		   break;
		}
		tracerX++;
		
	}*/
	if(!mPtr){
       MessageBox("Must select a entity base object");
	   return;
	}
	indexLoc = mPtr->m_uniqueId;
   //end get selected index
   //prepare location data
   BOOL result = FALSE;
   CPlacementDlg pDlg;
   D3DMATRIX tempMatrix;
   D3DVECTOR tempPosition;
   if(pDlg.DoModal()==IDOK){//if continue
	   CFrameObj* tempFrame = new CFrameObj(NULL);
	   if(pDlg.PlacementMethod == 0){//user define
		   D3DVECTOR temp;
		   temp.x = pDlg.m_posX;
		   temp.y = pDlg.m_posY;
		   temp.z = pDlg.m_posZ;
		   tempFrame->SetPosition(NULL,temp);
           tempFrame->GetTransform(NULL,&tempMatrix);
	   }//end user define
	   if(pDlg.PlacementMethod == 1){//based on entities position 
           int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              MessageBox("Must select an entity object on the entity runtime interface");
		      return;
		   }
	       POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
               ptrLoc->List_baseFrame->GetPosition(NULL,&tempPosition);
			   tempFrame->SetPosition(NULL,tempPosition);
               tempFrame->GetTransform(NULL,&tempMatrix);
			   
	   }//end based on entities position 
       if(pDlg.PlacementMethod == 2){//based on entities matrix 
          int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              MessageBox("Must select an entity object on the entity runtime interface");
		      return;
		   }
	       POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
               ptrLoc->List_baseFrame->GetTransform(NULL,&tempMatrix);

	   }//end based on entities matrix
	   tempFrame->SafeDelete();
	   delete tempFrame;
	   tempFrame=0;
   //end prepare location data
   //place entity if evrything is ok
   int controlType=0;
   if(pDlg.m_comboEntityType == "AI")
       controlType = 5;
   if(pDlg.m_comboEntityType == "VEHICLE")
       controlType = 10;
   if(pDlg.m_comboEntityType == "STATIC")
       controlType = 15;
   if(pDlg.m_comboEntityType == "PICKUP ITEM")
       controlType = 35;

   if(controlType == 0)
   result = m_gStatus->ActivateMovementEntity(  NULL,
												0,
												tempMatrix,
												0,
												0,
												indexLoc,
												controlType,
												0,
												0,
												FALSE,
												TRUE,
												FALSE,
												-1,
												-1,
												NULL,
												NULL,
												mPtr->m_primaryPhysicsOverd,
												pDlg.m_animationOv,
									            m_gStatus->m_collisionBase,
									            m_gStatus->m_collisionDatabase,
												m_gStatus->m_entObjRuntime,
												m_gStatus->m_currentWorkingFile);
   else{
   
   result = m_gStatus->ActivateMovementEntity(NULL,
	                               pDlg.m_aiBotIndex,
	                               tempMatrix,
                                   0,
								   0,
								   indexLoc,
								   controlType,
								   0,
								   0,
								   FALSE,
								   FALSE,
								   FALSE,
								   -1,
								   -1,
								   NULL,
					               NULL,
								   -1,
								   pDlg.m_animationOv,
								   m_gStatus->m_collisionBase,
								   m_gStatus->m_collisionDatabase,
								   m_gStatus->m_entObjRuntime,
								   m_gStatus->m_currentWorkingFile);
   }
   
   }//end if continue	
	if(result == FALSE)
	   MessageBox("activate failed");


#endif 
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::InListRuntimeEntities(int sel)
{

   m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_RESETCONTENT,0,0);	
   for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc != NULL;)
   {
       CMovementObj * entLoc = (CMovementObj *)m_gStatus->m_entObjRuntime->GetNext(posLoc);
       CString build = ">";
	   build = build+m_gStatus->m_cabLib.ConvToString(entLoc->List_traceNumber);
	   build = build+" id>";
	   build = build+m_gStatus->m_cabLib.ConvToString(entLoc->List_networkDefinedID);
	   m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)(entLoc->List_name+build));
   }
   if(sel > -1)
	   m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_SETCURSEL ,sel,0);

}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Adds a child to entity/movemnt model----------------------------------//
void EngineXWin::OnBtnEntityAddchild() 
{
	   CSqlConnectionDlg dlg;
       if(dlg.DoModal() == IDOK)
	   {
        if(!m_gStatus->m_entObjList->ExportToSQL(dlg.m_ipAddress, dlg.m_database, dlg.m_username, dlg.m_password))
		    AfxMessageBox("Failed",0,0);
		else
			AfxMessageBox("Completed",0,0);
	   }
	   return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------This will put into m_gStatus->m_fullScreen then allow you to return with esc-------//
void EngineXWin::OnButtonTestfullscreen() 
{
   m_gStatus->CreateDisplayIM(TRUE,0,0,ResolutionWidth,ResolutionHieght,16);	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------This will now make it render wireframe--------------------------------//
void EngineXWin::OnBtnEntityMovement() 
{
#ifndef _LimitedEngine
#ifndef _ClientOutput
    HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);

		temp.Replace("TRC_","");

	CMovementObj * mPtr = m_gStatus->m_entObjList->GetByID(atoi(temp));
    if(!mPtr)
	{
        MessageBox("Must select a entity base object");
		return;
	}
	CMotionSettingsDlg dlg;
            dlg.movementDB = mPtr->List_movementDB;	
			dlg.m_aiListRef = m_gStatus->m_aiList;
	        if(dlg.DoModal()==IDOK)
			mPtr->List_movementDB = dlg.movementDB;	
			//mPtr->List_speedBasedAnimation = dlg.m_speedBasedAnim;
			return;

    /*for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->List_traceNumber))) == temp){
			CMotionSettingsDlg dlg;
            dlg.movementDB = mPtr->List_movementDB;	
			dlg.m_aiListRef = m_gStatus->m_aiList;
	        if(dlg.DoModal()==IDOK)
			mPtr->List_movementDB = dlg.movementDB;	
			//mPtr->List_speedBasedAnimation = dlg.m_speedBasedAnim;
			return;
		}
	}
    MessageBox("Must select a entity base object");
	return;*/
#endif
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Entity in list--------------------------------------------------------//
void EngineXWin::EnitityInList() 
{
//delete all existing enries in tree
m_entityDlgBar.SendDlgItemMessage(IDC_TREE_ENTITIES,TVM_DELETEITEM,0,(LPARAM) (HTREEITEM)TVI_ROOT);
if(m_gStatus->m_entObjList)
  for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
	HTREEITEM item = AddTreeRoot(operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId)),IDC_TREE_ENTITIES,&m_entityDlgBar);
         HTREEITEM child1 = AddTreeChild(mPtr->List_name,
				                             IDC_TREE_ENTITIES,
											 &m_entityDlgBar,
											 item);
			    /*for(POSITION posLoc3 = cPtr->List_animationList->GetHeadPosition();posLoc3 != NULL;){//visual loop2 
			        CAnimationObj * aPtr = ( CAnimationObj* ) cPtr->List_animationList->GetNext(posLoc3); 
				    AddTreeChild(aPtr->List_animationName,IDC_TREE_ENTITIES,&m_entityDlgBar,child1);
				}*/
  }
}



//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---has the ability to deactivete a selected entity----------------------------------//
void EngineXWin::OnBtnEntityDeactivate() 
{
	   int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	   if(runtimeIndex == -1){
        MessageBox("Must select an entity object");
		return;
	   }
	   POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
	   if(!posLoc)
		   return;
	       CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
		   //ptrLoc->SafeDelete(m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_spawnGeneratorDB);
		   //delete ptrLoc;
		   m_gStatus->m_entObjRuntime->RemoveAt(posLoc);
		   m_gStatus->DestroyRuntimeEntity(ptrLoc,m_gStatus->m_spawnGeneratorDB,m_gStatus->m_entObjRuntime);
		   ptrLoc = 0;
		   
		   InListRuntimeEntities(runtimeIndex-1);
		   return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Button Delete entity--------------------------------------------------//
void EngineXWin::OnBtnDeleteEntity() 
{
	HWND treeHwnd = NULL;
    m_entityDlgBar.GetDlgItem(IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    POSITION delPos;
    for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();(delPos = posLoc) != NULL;)
	{//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp)
		{
			if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
				return;

			m_gStatus->m_entObjList->RemoveAt(delPos);
			delete mPtr;
			mPtr=0;
			m_entityDlgBar.SendDlgItemMessage(IDC_TREE_ENTITIES,TVM_DELETEITEM,0,(LPARAM) (HTREEITEM)item);
			return;
		}
	}
    MessageBox("Must select a entity base object");
	return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------deletes a entities animation--------------------------------------------------//
void EngineXWin::OnBTNENTITYdeleteAnim() 
{
 HWND treeHwnd = NULL;
    m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    POSITION delPos;
    for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();(delPos = posLoc) != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp){
			static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
			CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
			if ( opendialog.DoModal() == IDOK )
			{
				mPtr->List_skeletalObject->MergeAsLod(opendialog.GetPathName(),m_gStatus->m_dxRenderer->m_pd3dDevice);
			}
			return;
		}
	}
    MessageBox("Must select a entity base object");
	return;

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Button Delete entity child--------------------------------------------//
void EngineXWin::OnBTNENTITYDeleteChld() 
{
    if(AfxMessageBox("Are you sure? this id rebuild is only for advanced users and backwards compatibility.",MB_OKCANCEL,0)!=IDOK)
       return;

	CString conversion;
	m_entityDlgBar.GetDlgItemText( IDC_EDIT_REBUILDoffset,conversion);

    m_gStatus->m_entObjList->RebuildIds(atoi(conversion));

	AfxMessageBox("Completed rebuild of ids-only save if you know what you are doing");
	EnitityInList();
	return;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------resets scene to new---------------------------------------------------//
void EngineXWin::OnBtnNewall() 
{
	OnResetAll(); 
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------will load up collision data from a mesh-------------------------------//
BOOL EngineXWin::LoadCollisionDataFromImMesh(CEXMeshObj* imMeshPtr,CPolyObj * collisionData)
{
//*faceCount = (imMeshPtr->m_abVertexArray.m_indecieCount/3);
//*collisionData = new CPolyObj[(imMeshPtr->m_abVertexArray.m_indecieCount/3)];
//HRESULT err;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Updates Entitys last position-----------------------------------------//
void EngineXWin::UpdateEntitiesLastPosition()
{
   for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc != NULL;){//Begin entity loop
    CMovementObj * entLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);
    if(entLoc->List_baseFrame){//if valid
	     D3DMATRIX matrixLoc;
		 D3DVECTOR point1Loc;
		 entLoc->List_baseFrame->GetTransform(NULL,&matrixLoc);
		 point1Loc.x = 0.001f; 
	     point1Loc.y = 0.002f;
		 point1Loc.z = 0.0001f;//entLoc->List_boundingRadius*.5f;
	     entLoc->List_lastPosition = m_gStatus->m_cabLib.UpdatePolygonVertex(matrixLoc.m,point1Loc);
	}//end if valid
   }//end entiy loop
}
//--------------Disable a viewport----------------------------------------------------//
BOOL EngineXWin::DisableVieport(int viewportIndex)
{
	 

return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Disable Camera--------------------------------------------------------//
BOOL EngineXWin::DisableCamera(int cameraIndex)
{

return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Disable all viewports-------------------------------------------------//
BOOL EngineXWin::DisableAllVieports()
{
	
	for(POSITION posLoc = m_gStatus->m_viewPortDB->GetHeadPosition();posLoc != NULL;)
	{
	     CViewportObj* mptLoc = ( CViewportObj* ) m_gStatus->m_viewPortDB->GetNext(posLoc);
		 mptLoc->List_inUse = FALSE;
	}
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------activate a viewports-------------------------------------------------//
/*BOOL EngineXWin::ActivateVieport(CViewportObj * vptPtr)
{

m_gStatus->m_blackOut = TRUE;

         float X1conv = (float)vptPtr->List_X1;
		 float Y1conv = (float)vptPtr->List_Y1;
		 float X2conv = (float)vptPtr->List_X2;
		 float Y2conv = (float)vptPtr->List_Y2;

         

		 float width = (float)m_gStatus->m_imediateRenderRECT.right;
	     int X1perc = (int)((X1conv*width)*.01);
	     int X2perc = (int)((X2conv*width)*.01);
	     float hieght = (float)m_gStatus->m_imediateRenderRECT.bottom;
	     int Y1perc = (int)((Y1conv*hieght)*.01);
	     int Y2perc = (int)((Y2conv*hieght)*.01);

	     X2perc = X2perc - X1perc;
		 Y2perc = Y2perc - Y1perc;
		 if(m_gStatus->m_dxRenderer->m_pd3dDevice){//immediate mode
			UtilizeViewPort(vptPtr);
			vptPtr->List_inUse=TRUE;
		 }//end immediate mode
return TRUE;
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------viewports-------------------------------------------------------------//
BOOL EngineXWin::CreateCustomViewPort(int X1perc,
									  int Y1perc,
									  int X2perc,
									  int Y2perc)
{			
	 CViewportObj * objLoc = new CViewportObj(
		X1perc,
	    Y1perc,
		X2perc,
		Y2perc
     );
	 m_gStatus->m_viewPortDB->AddTail(objLoc);
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------Adds a new camera to data base-----------------------------------------//
void EngineXWin::OnCAMERACreateNewCamera() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------Adds a new camera to data base-----------------------------------------//
void EngineXWin::OnBTNCreateNewViewport() 
{
//retrieve Field of view
int topPercent = m_camVptDlgBar.GetDlgItemInt( IDC_EDIT_TOPpercent,NULL);
int bottomPercent = m_camVptDlgBar.GetDlgItemInt( IDC_EDIT_BottomPercent,NULL);
int leftPercent = m_camVptDlgBar.GetDlgItemInt( IDC_EDIT_LeftPercent,NULL);
int rightPercent = m_camVptDlgBar.GetDlgItemInt( IDC_EDIT_RightPercent,NULL);

CreateCustomViewPort(leftPercent,topPercent,rightPercent,bottomPercent);
CString combinationLoc = "Vport Data Top:"+m_gStatus->m_cabLib.ConvToString(topPercent);
combinationLoc = combinationLoc+" Bottom:";
combinationLoc = combinationLoc+m_gStatus->m_cabLib.ConvToString(bottomPercent);
combinationLoc = combinationLoc+" Left:";
combinationLoc = combinationLoc+m_gStatus->m_cabLib.ConvToString(leftPercent);
combinationLoc = combinationLoc+" Right:";
combinationLoc = combinationLoc+m_gStatus->m_cabLib.ConvToString(rightPercent);

m_camVptDlgBar.SendDlgItemMessage(IDC_LIST_VIEWPORTS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)combinationLoc);

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------tests a viewport camera combo-----------------------------------------//
void EngineXWin::OnBTNCameraVportActivate() 
{
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------Deactivate cameras and vieports----------------------------------------//
void EngineXWin::OnBTNvportDeactivate() 
{
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------creates a mouse object from interface---------------------------------//
void EngineXWin::OnBTNMOUSEcreate() 
{
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------button file browse----------------------------------------------------//
void EngineXWin::OnBTNMOUSEfileBrowse() 
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
static char BASED_CODE filter[] = "Texture Files|*.tga;*.bmp;*.png;*.dds;*.jpg;*.jpeg|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileTitle();
	   m_mouseDlgBar.SetDlgItemText( IDC_EDIT_MOUSEfile,fileName);
	   UpdateData(FALSE);
	 }
SetCurrentDirectory(MainDir);	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------used for testing mouse configurations---------------------------------//
void EngineXWin::OnBTNMOUSEactivate() 
{

}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------used for testing mouse configurations---------------------------------//
void EngineXWin::OnBTNMOUSEdelete() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------used for testing mouse configurations---------------------------------//
void EngineXWin::OnBTNMOUSEdeactivate() 
{
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Creates a 2D collection-----------------------------------------------//
void EngineXWin::OnBTN2DCreateCollection() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Adds a visual item to the collection specified------------------------//
void EngineXWin::OnBTN2DADDunit() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Activates a 2D collection---------------------------------------------//
void EngineXWin::OnBtn2dActivate() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Dectivates 2D collection----------------------------------------------//
void EngineXWin::OnBtn2dDeactivate() 
{
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Dectivates 2D collection----------------------------------------------//
void EngineXWin::OnBTN2DBrowseFileUp() 
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
static char BASED_CODE filter[] = "Texture Files|*.tga;*.bmp;*.png;*.dds;*.jpg;*.jpeg|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_FileUp,fileName);
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Dectivates 2D collection----------------------------------------------//
void EngineXWin::OnBTN2DBrowseFileDown() 
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
static char BASED_CODE filter[] = "Texture Files|*.tga;*.bmp;*.png;*.dds;*.jpg;*.jpeg|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   m_2DinterfaceBar.SetDlgItemText( IDC_EDIT_2D_FileDown,fileName);
	   UpdateData(FALSE);
	 }
 SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------gets selected 2D unit and fills data----------------------------------//
void EngineXWin::OnBTN2DgetSelectedInfo() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Apply 2D Modifications to a selected unit-----------------------------//
void EngineXWin::OnBTN2DApplyModifications() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------deletes a 2D collection-----------------------------------------------//
void EngineXWin::OnBTN2DDeleteCollection() 
{
   
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------deletes a selected unit-----------------------------------------------//
void EngineXWin::OnBTN2DDeleteUnit() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------2dinterface tree inlist-----------------------------------------------//
void EngineXWin::Tree2DinterfaceInList() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Pulls up color dlg for Background color-------------------------------//
/*void EngineXWin::OnBTNENVBckRgb() 
{
#ifndef _ClientOutput 
   CColorDlg cdlg;
   cdlg.m_red = m_gStatus->m_environmentPtr->m_backRed;
   cdlg.m_green = m_gStatus->m_environmentPtr->m_backGreen;
   cdlg.m_blue = m_gStatus->m_environmentPtr->m_backBlue;
   cdlg.DoModal();

	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckRed,m_gStatus->m_cabLib.FloatToStingCnv(cdlg.m_red));
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckGreen,m_gStatus->m_cabLib.FloatToStingCnv(cdlg.m_green));
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckBlue,m_gStatus->m_cabLib.FloatToStingCnv(cdlg.m_blue));

	

	m_gStatus->m_environmentPtr->m_backRed = cdlg.m_red;
	m_gStatus->m_environmentPtr->m_backGreen = cdlg.m_green;
	m_gStatus->m_environmentPtr->m_backBlue = cdlg.m_blue;
	m_gStatus->InitializeEnvironment(TRUE);
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Pulls up color dlg for fog color--------------------------------------//
void EngineXWin::OnBTNENVFogRgb() 
{
#ifndef _ClientOutput
   CColorDlg cdlg;
   cdlg.m_red = m_gStatus->m_environmentPtr->m_fogRed;
   cdlg.m_green = m_gStatus->m_environmentPtr->m_fogGreen;
   cdlg.m_blue = m_gStatus->m_environmentPtr->m_fogBlue;
   cdlg.DoModal();

	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogRed,m_gStatus->m_cabLib.FloatToStingCnv(cdlg.m_red));
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogGreen,m_gStatus->m_cabLib.FloatToStingCnv(cdlg.m_green));
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogBlue,m_gStatus->m_cabLib.FloatToStingCnv(cdlg.m_blue));	

    m_gStatus->m_environmentPtr->m_fogRed = cdlg.m_red;
	m_gStatus->m_environmentPtr->m_fogGreen = cdlg.m_green;
	m_gStatus->m_environmentPtr->m_fogBlue = cdlg.m_blue;
	m_gStatus->InitializeEnvironment(TRUE);
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Applys any changes to the environment---------------------------------//
void EngineXWin::OnBTNENVApplyChanges() 
{

	if(m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_FogOn,BM_GETSTATE,0,0)==BST_CHECKED)
       m_gStatus->m_environmentPtr->m_fogOn = TRUE;
	else
	   m_gStatus->m_environmentPtr->m_fogOn = FALSE;

	if(m_enviormentBar.SendDlgItemMessage(IDC_CHECK_lockBackColor,BM_GETSTATE,0,0)==BST_CHECKED)
       m_gStatus->m_environmentPtr->m_lockBackColor = TRUE;
	else
	   m_gStatus->m_environmentPtr->m_lockBackColor = FALSE;
	

    CString fogStart; 
    m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_StartRange,fogStart);
    m_gStatus->m_environmentPtr->m_fogStart = (float)atof((LPCTSTR)fogStart);

    CString fogEnd; 
    m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_EndRange,fogEnd);
    m_gStatus->m_environmentPtr->m_fogRange = (float)atof((LPCTSTR)fogEnd);

    CString density; 
    m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_Density,density);
    m_gStatus->m_environmentPtr->m_fogDensity = (float)atof((LPCTSTR)density);

    CString sunPosition; 
    m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_sunPosX,sunPosition);
	m_gStatus->m_environmentPtr->m_sunPosition.x = (float)atof((LPCTSTR)sunPosition);

    m_enviormentBar.GetDlgItemText( IDC_EDIT_ENVsunPosY,sunPosition);
	m_gStatus->m_environmentPtr->m_sunPosition.y = (float)atof((LPCTSTR)sunPosition);

    m_enviormentBar.GetDlgItemText( IDC_EDIT_ENVsunPosZ,sunPosition);
	m_gStatus->m_environmentPtr->m_sunPosition.z = (float)atof((LPCTSTR)sunPosition);
     
	CString tempPosition;

	m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_MaxWindStrength,tempPosition);
	m_gStatus->m_environmentPtr->m_maxWindStrength = (float)atof((LPCTSTR)tempPosition);

	m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_WindDirX,tempPosition);
	m_gStatus->m_environmentPtr->m_windDirectionX = (float)atof((LPCTSTR)tempPosition);

	m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_WindDirZ,tempPosition);
	m_gStatus->m_environmentPtr->m_windDirectionZ = (float)atof((LPCTSTR)tempPosition);
	
	m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_Gravity,tempPosition);
	m_gStatus->m_environmentPtr->m_gravity = (float)atof((LPCTSTR)tempPosition);

	m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_SeaLevel,tempPosition);
	m_gStatus->m_environmentPtr->m_seaLevel = (float)atof((LPCTSTR)tempPosition);

	m_enviormentBar.GetDlgItemText( IDC_EDIT_ENV_SeaDrag,tempPosition);
	m_gStatus->m_environmentPtr->m_seaViscusDrag = (float)atof((LPCTSTR)tempPosition);

	m_gStatus->InitializeEnvironment(TRUE);

}
//--------------Matches background to fog---------------------------------------------//
void EngineXWin::OnBTNENVMatch() 
{
    m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckRed,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_fogRed));
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckGreen,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_fogGreen));
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckBlue,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_fogBlue));

	m_gStatus->m_environmentPtr->m_backRed = m_gStatus->m_environmentPtr->m_fogRed;
	m_gStatus->m_environmentPtr->m_backGreen = m_gStatus->m_environmentPtr->m_fogGreen;
	m_gStatus->m_environmentPtr->m_backBlue = m_gStatus->m_environmentPtr->m_fogBlue;
	m_gStatus->InitializeEnvironment(TRUE);
}
//--------------updates on fog status too---------------------------------------------//
void EngineXWin::OnCHECKENVFogOn() 
{
    if(m_gStatus->m_environmentPtr->m_fogOn == TRUE)
	   m_gStatus->m_environmentPtr->m_fogOn=FALSE;
	else
       m_gStatus->m_environmentPtr->m_fogOn=TRUE;
	m_gStatus->InitializeEnvironment(TRUE);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------browse visual file for environment------------------------------------//
void EngineXWin::OnBTNENVBrowseSurObj() 
{
   
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------callback for environment----------------------------------------------//
/*void EngineXWin::EnvironmentUpdate(CFrameObj* frame)
{
D3DVECTOR position;
frame->GetPosition(NULL,&position);//get frame position

//if(m_gStatus->m_environmentPtr->List_content->List_mainFrame)
//m_gStatus->m_environmentPtr->List_content->List_mainFrame->SetPosition(NULL,position.x,position.y,position.z);

//if(m_gStatus->m_environmentPtr->List_flareFrame)
//m_gStatus->m_environmentPtr->List_flareFrame->SetPosition(NULL,position.x,position.y,position.z);



}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------enables and disables surroundings-------------------------------------//
/*void EngineXWin::OnCHECKEnableSurrounding() 
{

}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------object material editing-----------------------------------------------//
void EngineXWin::OnBTNObjectMaterial() 
{
#ifndef _ClientOutput
HWND treeHwnd = NULL;
m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd); 
int indexOfObject = GetTreeChildIndex(treeHwnd,item);	
HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,ObjectRoot,10);
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){

         if(!ptrLoc->m_meshObject)
		 {//no mesh object available
            AfxMessageBox("No mesh object available");
            return;
		 }//end no mesh object available

	     CMaterialDlg dlg;
		 dlg.m_pxShaderSystemRef = m_gStatus->m_pxShaderSystem;
		 dlg.m_vxShaderSystemRef = m_gStatus->m_shaderSystem;
		 dlg.m_globalShaderRef = m_gStatus->m_cgGlobalShaderDB;
         dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
		 dlg.meshRef = ptrLoc->m_meshObject;
		 dlg.makeMirror = ptrLoc->m_meshObject->List_materialObject->m_mirrorEnabled;
         if(dlg.DoModal()==IDOK){
		  if(dlg.makeMirror == TRUE)	  
          if(ptrLoc->m_meshObject->m_abVertexArray.m_vertexCount!=4) {
		     MessageBox("Mirror does not have four verticies!");
		     return;
		  }
		  if(!ptrLoc->m_meshObject)
		  {
             MessageBox("This object doesnt support direct material edits");
             return;
		  }
		  if(!ptrLoc->m_meshObject->List_materialObject)
		  {
             MessageBox("This object doesnt support direct material edits.");
             return;
		  }
		  ptrLoc->UpdateLods();
		  ptrLoc->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_cgGlobalShaderDB->GetByIndex(ptrLoc->m_meshObject->List_materialObject->m_optionalShaderIndex-1));
          if(dlg.applyMaterialToGroup){//Apply to group
             for(POSITION wPos = m_gStatus->m_wldObjList->GetHeadPosition();wPos != NULL;){//w Loop2
	             CWldObject * wPtr = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(wPos);	
				 if(wPtr->List_groupNumber == ptrLoc->List_groupNumber){//1
                    if(dlg.m_includeBlendMethods)
					{
					   CopyMemory(wPtr->m_meshObject->List_BlendMethod,ptrLoc->m_meshObject->List_BlendMethod,sizeof(int)*10);
					   wPtr->m_meshObject->List_materialObject->m_pixelShader = ptrLoc->m_meshObject->List_materialObject->m_pixelShader;
					   wPtr->m_meshObject->List_materialObject->m_vertexShader = ptrLoc->m_meshObject->List_materialObject->m_vertexShader;
					}

                    CopyMemory(&wPtr->m_meshObject->List_materialObject->m_baseMaterial,&ptrLoc->m_meshObject->List_materialObject->m_baseMaterial,sizeof(D3DMATERIAL9));
                    wPtr->m_meshObject->List_materialObject->m_textureTileType  = ptrLoc->m_meshObject->List_materialObject->m_textureTileType;
                    wPtr->m_meshObject->List_materialObject->m_cullOn           = ptrLoc->m_meshObject->List_materialObject->m_cullOn;
                    wPtr->m_meshObject->List_materialObject->m_forceWireFrame   = ptrLoc->m_meshObject->List_materialObject->m_forceWireFrame;
                    wPtr->m_meshObject->List_materialObject->m_baseBlendMode    = ptrLoc->m_meshObject->List_materialObject->m_baseBlendMode;
				    wPtr->m_meshObject->List_materialObject->m_fadeMaxDist      = ptrLoc->m_meshObject->List_materialObject->m_fadeMaxDist;
					wPtr->m_meshObject->List_materialObject->m_fadeVisDist      = ptrLoc->m_meshObject->List_materialObject->m_fadeVisDist;
					wPtr->m_meshObject->List_materialObject->m_fadeByDistance   = ptrLoc->m_meshObject->List_materialObject->m_fadeByDistance;		
					wPtr->m_meshObject->List_materialObject->m_enableTextureMovement = ptrLoc->m_meshObject->List_materialObject->m_enableTextureMovement;
					wPtr->m_meshObject->List_materialObject->m_enableSet0 = ptrLoc->m_meshObject->List_materialObject->m_enableSet0;
					wPtr->m_meshObject->List_materialObject->m_enableSet1 = ptrLoc->m_meshObject->List_materialObject->m_enableSet1;
					wPtr->m_meshObject->List_materialObject->m_textureMovement[0].tu = ptrLoc->m_meshObject->List_materialObject->m_textureMovement[0].tu;
					wPtr->m_meshObject->List_materialObject->m_textureMovement[0].tv = ptrLoc->m_meshObject->List_materialObject->m_textureMovement[0].tv;
					wPtr->m_meshObject->List_materialObject->m_textureMovement[1].tu = ptrLoc->m_meshObject->List_materialObject->m_textureMovement[1].tu;
					wPtr->m_meshObject->List_materialObject->m_textureMovement[1].tv = ptrLoc->m_meshObject->List_materialObject->m_textureMovement[1].tv;
					wPtr->m_meshObject->List_materialObject->m_zBias                 = ptrLoc->m_meshObject->List_materialObject->m_zBias;
					wPtr->m_meshObject->List_materialObject->m_fogFilter             = ptrLoc->m_meshObject->List_materialObject->m_fogFilter;
					wPtr->m_meshObject->List_materialObject->m_fogOverride           = ptrLoc->m_meshObject->List_materialObject->m_fogOverride;
					wPtr->m_meshObject->List_materialObject->Reset();
                      //
                    wPtr->UpdateLods();
					wPtr->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_cgGlobalShaderDB->GetByIndex(wPtr->m_meshObject->List_materialObject->m_optionalShaderIndex-1));
				 }//end 1
			 }//end w Loop2
		  }//end apply to group

		 }
	   //end do it
	return;
	}
  }//end Begin world object loop
  MessageBox("Must Select a visual");	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------sets emissive attributes of environment object------------------------//
/*void EngineXWin::OnBTNENVemmisive() 
{
   
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Enables/disables sun environment object-------------------------------//
void EngineXWin::OnCHECKENVenableSun() 
{
	if(m_gStatus->m_environmentPtr->m_sunLightOn == TRUE){
	   m_gStatus->m_environmentPtr->m_sunLightOn = FALSE;
	}
	else{
       m_gStatus->m_environmentPtr->m_sunLightOn = TRUE;
	   }
	m_gStatus->InitializeEnvironment(TRUE);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------changes sun color------------------------------------------------------//
void EngineXWin::OnBTNENVsunColor() 
{
#ifndef _ClientOutput
   CColorDlg cdlg;
   cdlg.m_red = m_gStatus->m_environmentPtr->m_sunRed;
   cdlg.m_green = m_gStatus->m_environmentPtr->m_sunGreen;
   cdlg.m_blue = m_gStatus->m_environmentPtr->m_sunBlue;
   cdlg.DoModal();
   m_gStatus->m_environmentPtr->m_sunRed = cdlg.m_red;
   m_gStatus->m_environmentPtr->m_sunGreen = cdlg.m_green;
   m_gStatus->m_environmentPtr->m_sunBlue = cdlg.m_blue;
m_gStatus->InitializeEnvironment(TRUE);
#endif
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------changes material on entity visual-------------------------------------//
void EngineXWin::OnBTNEntityMaterial() 
{

	//note : MERGE must have the primary edb that loaded must contain most items
	 CString edbPath,apdPath;
	 CMovementObjList * edbLocal = NULL;
	 CArmedInventoryList * apdLocal = NULL;
     static char BASED_CODE filter[] = "EDB Files|*.EDB|Edb Files|*.EDB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	  
	   edbPath = opendialog.GetPathName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(edbPath,CFile::modeRead, &exc);
	   if(res == FALSE){ 
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> edbLocal;
	   the_in_Archive.Close();
       fl.Close();
	   
	 }
	 else
	   return;

	 static char BASED_CODE filter2[] = "APD Files|*.APD|APD Files|*.APD|All Files (*.*)|*.*||";
	 CFileDialog opendialog2( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter2, this );
     if ( opendialog2.DoModal() == IDOK )
	 {
	  
	   apdPath = opendialog2.GetPathName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(apdPath,CFile::modeRead, &exc);
	   if(res == FALSE){
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> apdLocal;
	   the_in_Archive.Close();
       fl.Close();
	   apdLocal->CompileDirectAccess();
	 }
	 else
	   return;
     //get last unID
	 int countEdb = edbLocal->GetCount();
	 int latestEDBID = 0;
	 int latestItemID = 0;
	 for(POSITION posLoc2 = m_gStatus->m_entObjList->GetHeadPosition();posLoc2 != NULL;)
	 {
         CMovementObj * mPtr = (CMovementObj *) m_gStatus->m_entObjList->GetNext(posLoc2);
         latestEDBID = mPtr->m_uniqueId;
		 if(mPtr->m_itemDB)
		 for(POSITION itPos = mPtr->m_itemDB->GetHeadPosition();itPos != NULL;)
		 {
             CItemObj * itemPtr = (CItemObj *) mPtr->m_itemDB->GetNext(itPos);
			 if(itemPtr->m_globalID > latestItemID)
                latestItemID = itemPtr->m_globalID+1;
		 }
	 }
	 latestEDBID++;

	 int latestAPDID = 0;
	 if(m_gStatus->m_itemManager->m_visuals)
	 for(POSITION posLoc3 = m_gStatus->m_itemManager->m_visuals->GetHeadPosition();posLoc3 != NULL;)
	 {
         CArmedInventoryObj * aPtr = (CArmedInventoryObj *) m_gStatus->m_itemManager->m_visuals->GetNext(posLoc3);
         latestAPDID = aPtr->m_type;
	 }
	 latestAPDID++;
     //Merge objects one by one
	 
	 int previousID = 0;
	 POSITION posLast = NULL;
     
	 //merge Attachments
	 for(POSITION aPos = apdLocal->GetHeadPosition();(posLast = aPos) != NULL;)
	 {//merge attachments and reID accross edb local
	     CArmedInventoryObj * aPtr = (CArmedInventoryObj *) apdLocal->GetNext(aPos);
	     previousID = aPtr->m_type;
         aPtr->m_type = latestAPDID++;
		 m_gStatus->m_itemManager->m_visuals->AddTail(aPtr);
	     apdLocal->RemoveAt(posLast);
		 edbLocal->TransitionItemAttachmentID(previousID,aPtr->m_type);//remaps merge edb
	 }
	 m_gStatus->m_itemManager->m_visuals->CompileDirectAccess();
	 //merge entities
	 for(POSITION posLoc = edbLocal->GetHeadPosition();(posLast = posLoc) != NULL;)
	 {//merge entities and reID
         CMovementObj * mPtr = (CMovementObj *) edbLocal->GetNext(posLoc);
		 previousID = mPtr->m_uniqueId;//
		 mPtr->List_traceNumber = m_gStatus->GlobalTraceNumber++;
		 mPtr->m_uniqueId = latestEDBID++;//assign to new ID

		 //assign item db reid it
		 if(mPtr->m_itemDB)
		 for(POSITION itPos = mPtr->m_itemDB->GetHeadPosition();itPos != NULL;)
		 {
             CItemObj * itemPtr = (CItemObj *) mPtr->m_itemDB->GetNext(itPos);
             itemPtr->m_globalID = latestItemID++;
		 }
		 //swap the entity to the primary list
         m_gStatus->m_entObjList->AddTail(mPtr);
		 edbLocal->RemoveAt(posLast);
         //start attachment merge and remap
	 }//end merge entities and reid
	    EnitityInList();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Creates a text collection---------------------------------------------//
void EngineXWin::CreateTextCollection(CString CollectionName,int timeOn,int fontSelection,COLORREF color)//-1 timeOn = no time limit  / constant- 0 font selection = standard
{
    CTextCollection* cTextPtr = new CTextCollection(CollectionName,
		                            NULL,
					                0,
					                timeOn,
					                fontSelection,
									FALSE,
									color);
	cTextPtr->List_textEntryList = new CTextEntryList;
	m_gStatus->m_textInterfaceList->AddTail(cTextPtr);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Adds a text object to collection--------------------------------------//
BOOL EngineXWin::AddTextEntry(int collectionIndex,CString text,int percX,int percY)
{
POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(collectionIndex);
if(!posLoc)
return FALSE;
CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
CTextEntry * entryPtr = new CTextEntry(text,percX,percY);
cTextPtr->List_textEntryList->AddTail(entryPtr);
return TRUE;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Report Message--------------------------------------------------------//
void EngineXWin::ReportError(CString errorMessageLocal)
{
m_gStatus->ErrorOutPut=TRUE;
ErrorMessage = errorMessageLocal;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Close Message---------------------------------------------------------//
void EngineXWin::CloseErrorMessage()
{
m_gStatus->ErrorOutPut=FALSE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Activate a text display-----------------------------------------------//
BOOL EngineXWin::ActivateTextDisplay(int collectionIndex)
{
POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(collectionIndex);
if(!posLoc)
return FALSE;
CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
cTextPtr->List_displayIsOn = TRUE;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------DeActivate a text display-----------------------------------------------//
BOOL EngineXWin::DeActivateTextDisplay(int collectionIndex)
{
POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(collectionIndex);
if(!posLoc)
return FALSE;
CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
cTextPtr->List_displayIsOn = FALSE;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------DeActivate All Text displays------------------------------------------//
void EngineXWin::DeActivateAllTextDisplays()
{
 for(POSITION posLoc = m_gStatus->m_textInterfaceList->GetHeadPosition();posLoc != NULL;){//loopB
	CTextCollection* collectionPtr = ( CTextCollection* ) m_gStatus->m_textInterfaceList->GetNext(posLoc);
    collectionPtr->List_displayIsOn = FALSE;
 }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Add Font To DataBase--------------------------------------------------//
void EngineXWin::AddFont(int sizeX,int sizeY)
{
CFontDialog fdlg;
fdlg.DoModal();

CFontObj*fntPtr = new CFontObj(NULL,fdlg.GetFaceName(),sizeX,sizeY);
fntPtr->List_fontObject  = CreateFont((int)(sizeX),
						   (int)(sizeY), 0, 0,
		                   1, FALSE, FALSE, FALSE,
		                   ANSI_CHARSET,
		                   OUT_DEFAULT_PRECIS,
		                   CLIP_DEFAULT_PRECIS,
		                   PROOF_QUALITY,
		                   VARIABLE_PITCH,
		                   fntPtr->List_fontName);
m_gStatus->m_fontList->AddTail(fntPtr);
m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_FONTS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)fntPtr->List_fontName);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Used to initialize fonts from file------------------------------------//
void EngineXWin::InitAllFonts()
{
m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_FONTS,LB_RESETCONTENT,0,0);
for(POSITION posLoc = m_gStatus->m_fontList->GetHeadPosition();posLoc != NULL;){//loopB
	CFontObj* fntPtr = ( CFontObj* ) m_gStatus->m_fontList->GetNext(posLoc);
    fntPtr->List_fontObject = CreateFont((int)(fntPtr->List_sizeX),
						       (int)(fntPtr->List_sizeY), 0, 0,
		                       1, FALSE, FALSE, FALSE,
		                       ANSI_CHARSET,
		                       OUT_DEFAULT_PRECIS,
		                       CLIP_DEFAULT_PRECIS,
		                       PROOF_QUALITY,
		                       VARIABLE_PITCH,
		                       fntPtr->List_fontName);
	m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_FONTS,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)fntPtr->List_fontName);
 }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------Adds a base collection object to text db list--------------------------//
void EngineXWin::OnBUTTONtextAddTextCollection() 
{
CString data;
m_textFontDlgBar.GetDlgItemText( IDC_EDIT_text_Data,data);
if(data.GetLength()==0){
MessageBox("Enter name of collection in data field and try again");
return;
}
CreateTextCollection(data,-1,0,RGB(0,0,0));
AddTreeChild(data,IDC_TREE_TEXTdisplays,&m_textFontDlgBar,TextDisplayTreeROOT);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------Adds a base collection object to text db list--------------------------//
void EngineXWin::OnBUTTONtextAddTextObject() 
{
HWND treeHwnd = NULL;

int selectionLevel = GetTreeSelectionLevel(IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
if(selectionLevel == -1 || selectionLevel != 1){
MessageBox("Must select a display collection");
return;
}

m_textFontDlgBar.GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd); 
int collectionIndex = GetTreeChildIndex(treeHwnd,item) ;

CString data;
m_textFontDlgBar.GetDlgItemText( IDC_EDIT_text_Data,data);
if(data.GetLength()==0){
MessageBox("Enter the text entry in data field and try again");
return;
}

int tempPercX = m_textFontDlgBar.GetDlgItemInt(IDC_EDIT_text_percX,NULL,TRUE);
int tempPercY = m_textFontDlgBar.GetDlgItemInt(IDC_EDIT_text_percY,NULL,TRUE);

AddTextEntry(collectionIndex,data,tempPercX,tempPercY);
AddTreeChild(data,IDC_TREE_TEXTdisplays,&m_textFontDlgBar,item);

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------gets the selection level based on roots--------------------------------//
int EngineXWin::GetTreeSelectionLevel(UINT id,CDialogBar *barLoc)
{
HWND treeHwnd = NULL;
barLoc->GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd);
if(!item)
return -1;

int counterA=0;
HTREEITEM parent = item;
while(1){
   parent = TreeView_GetParent(treeHwnd,parent);
   if(!parent)
       break;
   counterA++;
}
return counterA;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------activates specific text unit-------------------------------------------//
void EngineXWin::OnBTNtextActivate() 
{
    HWND treeHwnd = NULL;
    int selectionLevel = GetTreeSelectionLevel(IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
    if(selectionLevel == -1 || selectionLevel != 1){
    MessageBox("Must select a display collection");
    return;
	}	
	m_textFontDlgBar.GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
    int collectionIndex = GetTreeChildIndex(treeHwnd,item) ;
	ActivateTextDisplay(collectionIndex);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------deactivates specific text unit-------------------------------------------//
void EngineXWin::OnBTNtextDeactivate() 
{
	HWND treeHwnd = NULL;
    int selectionLevel = GetTreeSelectionLevel(IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
    if(selectionLevel == -1 || selectionLevel != 1){
    MessageBox("Must select a display collection");
    return;
	}	
	m_textFontDlgBar.GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
    int collectionIndex = GetTreeChildIndex(treeHwnd,item) ;
	DeActivateTextDisplay(collectionIndex);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------changes collections text color-----------------------------------------//
void EngineXWin::OnBUTTONtextTextColor() 
{
#ifndef _ClientOutput
    HWND treeHwnd = NULL;
    int selectionLevel = GetTreeSelectionLevel(IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
    if(selectionLevel == -1 || selectionLevel != 1){
    MessageBox("Must select a display collection");
    return;
	}	
	m_textFontDlgBar.GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
    int collectionIndex = GetTreeChildIndex(treeHwnd,item) ;

    POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(collectionIndex);
    if(!posLoc)
    return;
    CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
    
	CColorDlg dlg;
	dlg.m_red = (float)GetRValue(cTextPtr->List_textColor);
    dlg.m_green = (float)GetGValue(cTextPtr->List_textColor);
	dlg.m_blue = (float)GetBValue(cTextPtr->List_textColor);
	
	dlg.color255 = TRUE;
	if(dlg.DoModal()==TRUE){
       cTextPtr->List_textColor = RGB(dlg.m_red,dlg.m_green,dlg.m_blue);
	}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------this applys changes to collection and enty levels :)---------------------//
void EngineXWin::OnBTNtextApplyChanges() 
{
    HWND treeHwnd = NULL;
    int selectionLevel = GetTreeSelectionLevel(IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
    if(selectionLevel == -1){
    MessageBox("Must select a display collection or entry ;!");
    return;
	}	
	m_textFontDlgBar.GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
    int universalIndex = GetTreeChildIndex(treeHwnd,item) ;
   
    if(selectionLevel == 1){//a collection is selected
    POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(universalIndex);
    if(!posLoc)
    return;
    CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
	cTextPtr->List_timeLength = m_textFontDlgBar.GetDlgItemInt(IDC_EDIT_text_timeLimit,NULL,TRUE);
	cTextPtr->List_timeVarial = 0;
	CString data;
      m_textFontDlgBar.GetDlgItemText( IDC_EDIT_text_Data,data);
      if(data.GetLength()==0){
      MessageBox("Enter the text entry in data field and try again");
         return;
	  }
    cTextPtr->List_collectionName = data;
	SetTreeItemText(data,IDC_TREE_TEXTdisplays,&m_textFontDlgBar,item,500);
	}//end a collection is selected
	if(selectionLevel == 2){//a text entry is selected
	   HTREEITEM parent = TreeView_GetParent(treeHwnd,item);
	   int parentIndex = GetTreeChildIndex(treeHwnd,parent) ;
         POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(parentIndex);
         if(!posLoc)
             return;
         CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
	
	     posLoc = cTextPtr->List_textEntryList->FindIndex(universalIndex);
         if(!posLoc)
             return;
         CTextEntry* entryPtr = ( CTextEntry* )cTextPtr->List_textEntryList->GetAt(posLoc);
	
	     CString data;
           m_textFontDlgBar.GetDlgItemText( IDC_EDIT_text_Data,data);
             if(data.GetLength()==0){
                MessageBox("Enter the text entry in data field and try again");
                return;
			 }
		   entryPtr->List_text = data;
           entryPtr->List_percX = m_textFontDlgBar.GetDlgItemInt(IDC_EDIT_text_percX,NULL,TRUE);
           entryPtr->List_percY = m_textFontDlgBar.GetDlgItemInt(IDC_EDIT_text_percY,NULL,TRUE);
           SetTreeItemText(data,IDC_TREE_TEXTdisplays,&m_textFontDlgBar,item,500);
	}//end a text entry is selected
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Get info on text object info---------------------------------------------//
void EngineXWin::OnBTNtextGetCurrentInfo() 
{
    HWND treeHwnd = NULL;
    int selectionLevel = GetTreeSelectionLevel(IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
    if(selectionLevel == -1){
    MessageBox("Must select a display collection or entry ;!");
    return;
	}	
	m_textFontDlgBar.GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
    int universalIndex = GetTreeChildIndex(treeHwnd,item) ;
   
    if(selectionLevel == 1){//a collection is selected
    POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(universalIndex);
    if(!posLoc)
    return;
    CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
	m_textFontDlgBar.SetDlgItemText( IDC_EDIT_text_Data,cTextPtr->List_collectionName);
	m_textFontDlgBar.SetDlgItemInt( IDC_EDIT_text_timeLimit,cTextPtr->List_timeLength);
	}//end a collection is selected
	if(selectionLevel == 2){//a text entry is selected
	   HTREEITEM parent = TreeView_GetParent(treeHwnd,item);
	   int parentIndex = GetTreeChildIndex(treeHwnd,parent) ;
         POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(parentIndex);
         if(!posLoc)
             return;
         CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
	
	     posLoc = cTextPtr->List_textEntryList->FindIndex(universalIndex);
         if(!posLoc)
             return;
         CTextEntry* entryPtr = ( CTextEntry* )cTextPtr->List_textEntryList->GetAt(posLoc);
	       //CString ConvertLongToString(int number);
	       m_textFontDlgBar.SetDlgItemInt( IDC_EDIT_text_percX,entryPtr->List_percX);
	       m_textFontDlgBar.SetDlgItemInt( IDC_EDIT_text_percY,entryPtr->List_percY);
	       m_textFontDlgBar.SetDlgItemText( IDC_EDIT_text_Data,entryPtr->List_text);
	}//end a text entry is selected
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Apply Font to text collection--------------------------------------------//
void EngineXWin::OnBTNtextApplyFont() 
{
    int fontIndex = m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_FONTS,LB_GETCURSEL ,0,0);
	if(fontIndex == -1){
       MessageBox("must select a font");
	   return;
	}	
    HWND treeHwnd = NULL;
    int selectionLevel = GetTreeSelectionLevel(IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
    if(selectionLevel == -1 || selectionLevel != 1){
    MessageBox("Must select a display collection");
    return;
	}	
	m_textFontDlgBar.GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
    int collectionIndex = GetTreeChildIndex(treeHwnd,item) ;

    POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(collectionIndex);
    if(!posLoc)
    return;
    CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
    cTextPtr->List_fontSel	= fontIndex;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Adds a font to database--------------------------------------------------//
void EngineXWin::OnBUTTONtextAddFont() 
{
int szX = m_textFontDlgBar.GetDlgItemInt(IDC_EDIT_text_SizeX,NULL,TRUE);
int szY = m_textFontDlgBar.GetDlgItemInt(IDC_EDIT_text_SizeY,NULL,TRUE);	
AddFont(szX,szY);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Apply size changes to text font------------------------------------------//
void EngineXWin::OnBTNtextApplySizeChange() 
{
int fontIndex = m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_FONTS,LB_GETCURSEL ,0,0);
	if(fontIndex == -1){
       MessageBox("must select a font");
	   return;
	}	
    POSITION position = m_gStatus->m_fontList->FindIndex(fontIndex);
	if(!position){
       MessageBox("Invalid Font");
	   return;
	}
	CFontObj * fPtr = (CFontObj *)m_gStatus->m_fontList->GetAt(position);
    fPtr->List_sizeX = m_textFontDlgBar.GetDlgItemInt(IDC_EDIT_text_SizeX,NULL,TRUE);
    fPtr->List_sizeY = m_textFontDlgBar.GetDlgItemInt(IDC_EDIT_text_SizeY,NULL,TRUE);	
    DeleteObject(fPtr->List_fontObject);//standard font
	fPtr->List_fontObject = CreateFont((int)(fPtr->List_sizeX),
						       (int)(fPtr->List_sizeY), 0, 0,
		                       1, FALSE, FALSE, FALSE,
		                       ANSI_CHARSET,
		                       OUT_DEFAULT_PRECIS,
		                       CLIP_DEFAULT_PRECIS,
		                       PROOF_QUALITY,
		                       VARIABLE_PITCH,
		                       fPtr->List_fontName);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Deletes a selected font--------------------------------------------------//
void EngineXWin::OnBUTTONtextDeleteFont() 
{
int fontIndex = m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_FONTS,LB_GETCURSEL ,0,0);
	if(fontIndex == -1){
       MessageBox("must select a font");
	   return;
	}	
    POSITION position = m_gStatus->m_fontList->FindIndex(fontIndex);
	if(!position){
       MessageBox("Invalid Font");
	   return;
	}
	CFontObj * fPtr = (CFontObj *)m_gStatus->m_fontList->GetAt(position);
	m_gStatus->m_fontList->RemoveAt(position);
	DeleteObject(fPtr->List_fontObject);
	delete fPtr;
	fPtr=0;

	m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_FONTS,LB_DELETESTRING,fontIndex,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Delete selected Text Collection------------------------------------------//
void EngineXWin::OnBUTTONtextDeleteTextCollection() 
{
    HWND treeHwnd = NULL;
    int selectionLevel = GetTreeSelectionLevel(IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
    if(selectionLevel == -1 || selectionLevel != 1){
    MessageBox("Must select a display collection");
    return;
	}	
	m_textFontDlgBar.GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
    int collectionIndex = GetTreeChildIndex(treeHwnd,item) ;

    POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(collectionIndex);
    if(!posLoc)
    return;
    CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
	for(POSITION posLoc2 = cTextPtr->List_textEntryList->GetHeadPosition();posLoc2 != NULL;){
        CTextEntry* entryPtr = (CTextEntry*)cTextPtr->List_textEntryList->GetNext(posLoc2);
		delete entryPtr;
        entryPtr=0;
	}
    cTextPtr->List_textEntryList->RemoveAll();
	delete cTextPtr->List_textEntryList;
	cTextPtr->List_textEntryList=0;
	m_gStatus->m_textInterfaceList->RemoveAt(posLoc);
	delete cTextPtr;
	cTextPtr=0;
	m_textFontDlgBar.SendDlgItemMessage(IDC_TREE_TEXTdisplays,TVM_DELETEITEM,0,(LPARAM) (HTREEITEM)item);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Delete selected Text object----------------------------------------------//
void EngineXWin::OnBUTTONtextDeleteTextObject() 
{
  HWND treeHwnd = NULL;
    int selectionLevel = GetTreeSelectionLevel(IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
    if(selectionLevel == -1 || selectionLevel == 1){
    MessageBox("Must select a entry ;!");
    return;
	}	
	m_textFontDlgBar.GetDlgItem( IDC_TREE_TEXTdisplays, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
    int universalIndex = GetTreeChildIndex(treeHwnd,item) ;
  
	   HTREEITEM parent = TreeView_GetParent(treeHwnd,item);
	   int parentIndex = GetTreeChildIndex(treeHwnd,parent) ;
         POSITION posLoc = m_gStatus->m_textInterfaceList->FindIndex(parentIndex);
         if(!posLoc)
             return;
         CTextCollection* cTextPtr = ( CTextCollection* )m_gStatus->m_textInterfaceList->GetAt(posLoc);
	
	     posLoc = cTextPtr->List_textEntryList->FindIndex(universalIndex);
         if(!posLoc)
             return;
         CTextEntry* entryPtr = ( CTextEntry* )cTextPtr->List_textEntryList->GetAt(posLoc);
		 cTextPtr->List_textEntryList->RemoveAt(posLoc);
		 delete entryPtr;
		 entryPtr=0;

      m_textFontDlgBar.SendDlgItemMessage(IDC_TREE_TEXTdisplays,TVM_DELETEITEM,0,(LPARAM) (HTREEITEM)item);
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Save text font data to file-------------------------------------------//
void EngineXWin::OnBTNtestSave() 
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
     static char BASED_CODE filter[] = "ETF ENGIN|*.ETF|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "ETF", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SaveTextFontInterfaceFromFile(fileName);
	 }
	 SetCurrentDirectory(MainDir);
}
//--------------Load text font data to file-------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------transform beta code---------------------------------------------------//
void EngineXWin::OnBTNtestLoad() 
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

   static char BASED_CODE filter[] = "ETF Files|*.ETF|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadTextFontInterfaceFromFile(fileName);

	      TextFontInterfaceInlist();
	      InListCGFonts(0);
	 }
     SetCurrentDirectory(MainDir);
}
//--------------Save text font data to file-------------------------------------------//
BOOL EngineXWin::SaveTextFontInterfaceFromFile(CString fileName)
{
   CFileException exc;
   CFile fl;
   BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
   if(res == FALSE)
	  return FALSE;
   CArchive the_out_Archive(&fl,CArchive::store);
   the_out_Archive << m_gStatus->m_textInterfaceList << m_gStatus->m_fontList << m_gStatus->m_cgFontList << m_gStatus->m_symbolMapDB;
   the_out_Archive.Close();
   fl.Close();

  return TRUE;
}
//--------------END FUNCTION----------------------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------In list font and text interfaces--------------------------------------//
void EngineXWin::TextFontInterfaceInlist()
{
	m_textFontDlgBar.SendDlgItemMessage(IDC_TREE_TEXTdisplays,TVM_DELETEITEM,0,(LPARAM) (HTREEITEM)TVI_ROOT);
    TextDisplayTreeROOT = AddTreeRoot("Text Display Database",IDC_TREE_TEXTdisplays,&m_textFontDlgBar);
	InitAllFonts();
      for(POSITION posLoc = m_gStatus->m_textInterfaceList->GetHeadPosition();posLoc != NULL;){//loopB
	      CTextCollection* collectionPtr = ( CTextCollection* ) m_gStatus->m_textInterfaceList->GetNext(posLoc);
	         HTREEITEM root = AddTreeChild(collectionPtr->List_collectionName,IDC_TREE_TEXTdisplays,&m_textFontDlgBar,TextDisplayTreeROOT);
		     for(POSITION posLoc2 = collectionPtr->List_textEntryList->GetHeadPosition();posLoc2 != NULL;){//loopC
	             CTextEntry* txtItemPtr = ( CTextEntry* ) collectionPtr->List_textEntryList->GetNext(posLoc2);
                 AddTreeChild(txtItemPtr->List_text,IDC_TREE_TEXTdisplays,&m_textFontDlgBar,root);
	   
			 }//loopC
	  }//loopB
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----saves vieports and cameras------------------------------------------------------//
void EngineXWin::OnBTNcamVportSave() 
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
     static char BASED_CODE filter[] = "EVC Files|*.EVC|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "EVC", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_gStatus->m_cameraList << m_gStatus->m_viewPortDB;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------------------Loads viewports and cameras-----------------------------//
void EngineXWin::OnBTNcamVportLoad() 
{
	
}
//----deletes camera-------------------------------------------------------//
void EngineXWin::OnBTNDeleteCamera() 
{
	int cameraIndex = m_camVptDlgBar.SendDlgItemMessage(IDC_LIST_CAMERAS,LB_GETCURSEL ,0,0);
	if(cameraIndex == -1)
	{
		MessageBox("must select a camera");
		return;
	}
	POSITION posLoc = m_gStatus->m_cameraList->FindIndex(cameraIndex);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CCameraObj* camLoc = ( CCameraObj* ) m_gStatus->m_cameraList->GetAt(posLoc);
	m_gStatus->m_cameraList->RemoveAt(posLoc);
	delete camLoc;
	camLoc=0;
	m_camVptDlgBar.SendDlgItemMessage(IDC_LIST_CAMERAS,LB_DELETESTRING,cameraIndex,0);	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----delete Viewport-----------------------------------------------------------------//
void EngineXWin::OnBTNDeleteViewport() 
{
	int vportIndex = m_camVptDlgBar.SendDlgItemMessage(IDC_LIST_VIEWPORTS,LB_GETCURSEL ,0,0);
	if(vportIndex == -1)
	{
		MessageBox("must select a viewport");
		return;
	}
	POSITION posLoc = m_gStatus->m_viewPortDB->FindIndex(vportIndex);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CViewportObj* vpLoc = ( CViewportObj* ) m_gStatus->m_viewPortDB->GetAt(posLoc);
	m_gStatus->m_viewPortDB->RemoveAt(posLoc);
	delete vpLoc;
	vpLoc=0;
	m_camVptDlgBar.SendDlgItemMessage(IDC_LIST_VIEWPORTS,LB_DELETESTRING,vportIndex,0);	
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Tear down background interface but keep active data valid for restore---------//
void EngineXWin::TearDownBackgroundInterface()
{
    for(POSITION posLoc = m_gStatus->m_backGroundList->GetHeadPosition();posLoc != NULL;){//Begin background loop
        CBackGroundObj * ptrLoc = (CBackGroundObj*) m_gStatus->m_backGroundList->GetNext(posLoc);
        if(ptrLoc->List_mainSurface){
           ptrLoc->List_mainSurface->Release();
		   ptrLoc->List_mainSurface=0;
		}
	}
}
//--------------tear down all vports leave inuse varuial for restore------------------//
BOOL EngineXWin::TearDownAllVieportInterfaces()
{
return TRUE;
}
//---------------------------------saves 2D interfaces to file------------------------//
void EngineXWin::OnBTN2DSaveConfig() 
{

}
//----------------------------loads 2D interfaces from file---------------------------//
void EngineXWin::OnBTN2DLoadConfig() 
{

}
//---------------interface add explosion----------------------------------------------//
/*void EngineXWin::OnBTNEXPAddExplosion() 
{
#ifndef _ClientOutput
   CExplosionBasicDlg dlg;
   dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
   dlg.m_dirSndRef = m_gStatus->m_dirSnd;
   CExplosionObj* newExpObj = new CExplosionObj();
   dlg.m_explosionRef = newExpObj;
   dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
   dlg.m_elementDBREF = m_gStatus->m_elementDB;
   dlg.m_skillDatabaseRef = m_gStatus->m_skillDatabase;
   if(dlg.DoModal()==IDOK){
      m_gStatus->m_explosionDBList->AddTail(newExpObj);
	  InListExplosionDB((m_gStatus->m_explosionDBList->GetCount()-1));
	  if(newExpObj->m_meshObject)
	     m_gStatus->ReinitTextureUsage(newExpObj->m_meshObject);
   }
   else{
     newExpObj->SafeDelete();
     delete newExpObj;
     newExpObj=0;
   }
#endif
}*/
//--------------function resets scene to new------------------------------------------//

void EngineXWin::OnResetAll() 
{
	m_gStatus->m_blackOut = TRUE;//blacks out screen
    m_gStatus->DestroyAllTexturesFromDatabase();
	m_gStatus->DestroyEnvironment();
	m_gStatus->DestroyWorldObjects();

	int indexLocal=0;
	for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);
        m_gStatus->DestroyRuntimeEntity(mPtr,m_gStatus->m_spawnGeneratorDB,m_gStatus->m_entObjRuntime);
		mPtr=0;
		indexLocal++;
	}
	m_gStatus->m_entObjRuntime->RemoveAll();
	InListRuntimeEntities(0);
	m_gStatus->m_worldRenderList->RemoveAll();
	m_gStatus->m_worldTransferList->RemoveAll();
	if(m_gStatus->m_collisionBase)
	{
	m_gStatus->m_collisionBase->SafeDelete();
    delete m_gStatus->m_collisionBase;
	m_gStatus->m_collisionBase=0;
	}
	m_gStatus->m_collisionBase    =          new CCollisionObj();
	//reinit
	m_gStatus->m_wldObjList =                new CWldObjectList;//world objects
	m_gStatus->m_environmentPtr =            new CEnvironmentObj;//environment
	m_gStatus->m_dynamicTextureDataBase   =  new CTextureDBObjList;//
    m_gStatus->InListTextureDB(0);	
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------gets meshes actual origin and radius----------------------------------//
void EngineXWin::FindOriginAndRadius(D3DVECTOR * origin,
									 float * spereRadius,
									 CPolyObj polyArray[],
									 int polyCount,
									 CFrameObj * mainFrame)
{
      float largestX,smallestX,largestY,smallestY,largestZ,smallestZ;
      if(polyCount == 0)
		 return;

      int quantity = polyCount;
      largestX=polyArray[0].polygon[0].x;
	  smallestX=polyArray[0].polygon[0].x;
	  largestY=polyArray[0].polygon[0].y;
	  smallestY=polyArray[0].polygon[0].y;
	  largestZ=polyArray[0].polygon[0].z;
	  smallestZ=polyArray[0].polygon[0].z; 

	  D3DVECTOR frameOrigin;
      frameOrigin.x = 0.0f;
	  frameOrigin.y = 0.0f;
	  frameOrigin.z = 0.0f;

	  if(mainFrame)
	  mainFrame->GetPosition(NULL,&frameOrigin);

    for(int loop=0;loop<quantity;loop++)
	{//loop through polygons
      for(int vertIndx=0;vertIndx<3;vertIndx++){//loop through polygon vertexes max 3
       if(largestX < polyArray[loop].polygon[vertIndx].x)
          largestX = polyArray[loop].polygon[vertIndx].x;
       
	   if(smallestX > polyArray[loop].polygon[vertIndx].x)
		  smallestX = polyArray[loop].polygon[vertIndx].x;
	   
	   if(largestY < polyArray[loop].polygon[vertIndx].y)
          largestY = polyArray[loop].polygon[vertIndx].y;
       
	   if(smallestY > polyArray[loop].polygon[vertIndx].y)
		  smallestY = polyArray[loop].polygon[vertIndx].y;

	   if(largestZ < polyArray[loop].polygon[vertIndx].z)
          largestZ = polyArray[loop].polygon[vertIndx].z;
       
	   if(smallestZ > polyArray[loop].polygon[vertIndx].z)
		  smallestZ = polyArray[loop].polygon[vertIndx].z;
	  }//end loop through polygon vertexes max 3
	}//end loop through polygons
	origin->x = (largestX + smallestX)*.5f;
    origin->y = (largestY + smallestY)*.5f;
	origin->z = (largestZ + smallestZ)*.5f;

	*spereRadius = m_gStatus->m_cabLib.MagnitudeSquared(origin->x,origin->y,
									       origin->z,largestX,
									       largestY,largestZ);
	origin->x = origin->x + frameOrigin.x;
	origin->y = origin->y + frameOrigin.y;
	origin->z = origin->z + frameOrigin.z;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------updates d3dvector translation-----------------------------------------//
D3DVECTOR EngineXWin::UpdateTransD3dvector(D3DVECTOR v1,D3DVECTOR v2)
{
D3DVECTOR localVect;
localVect.x = v1.x + v2.x;
localVect.y = v1.y + v2.y;
localVect.z = v1.z + v2.z;
return localVect;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------camera advanced settings----------------------------------------------//
void EngineXWin::OnBTNCameraAdvanced() 
{
#ifndef _ClientOutput
	CCameraAdvDlg dlg;
    int cameraIndex = m_camVptDlgBar.SendDlgItemMessage(IDC_LIST_CAMERAS,LB_GETCURSEL ,0,0);
	if(cameraIndex == -1){
       MessageBox("must select a camera");
	   return;
	}
	//refrence the camera and vport
    POSITION posCamLoc = m_gStatus->m_cameraList->FindIndex(cameraIndex);
    if(!posCamLoc){
        MessageBox("Camera isnt in database");
		return;
	}
    CCameraObj * camPtr = (CCameraObj *)m_gStatus->m_cameraList->GetAt(posCamLoc);
    dlg.m_fixedAfterPosX = camPtr->List_fixedAfterPosX;
	dlg.m_fixedAfterPosY = camPtr->List_fixedAfterPosY;
	dlg.m_fixedAfterPosZ = camPtr->List_fixedAfterPosZ;
	dlg.m_InitialPosX = camPtr->List_InitialPosX;
	dlg.m_InitialPosY = camPtr->List_InitialPosY;
	dlg.m_InitialPosZ = camPtr->List_InitialPosZ;
	dlg.m_floatingMovePwr = camPtr->List_floatingMovePwr;
	dlg.m_clipPlane = camPtr->List_clipPlane;
	dlg.m_fov = camPtr->List_fovView;
	dlg.cameraTypeInt = camPtr->List_type;
	dlg.floatingUsesCollision = camPtr->List_floatingUsesCollision;
	dlg.m_zoomInSpeed = camPtr->List_zoomInSpeed;
	dlg.m_zoomOutSpeed = camPtr->List_zoomOutSpeed;
	dlg.m_zoomInStop = camPtr->List_zoomInStop;
	dlg.m_floatTargetPosX = camPtr->List_floatingCamTarget.x;
	dlg.m_floatTargetPosY = camPtr->List_floatingCamTarget.y;
	dlg.m_floatTargetPosZ = camPtr->List_floatingCamTarget.z;
	dlg.m_baseHieghtAspect = camPtr->List_hieghtBaseAspect;
	dlg.m_pitchRotation = camPtr->m_pitchRotation;
	dlg.m_miscVal1 = camPtr->m_miscVal1;
	dlg.g_filterFirstPersonModeRef = camPtr->m_filterFirstPersonMode;
	if(dlg.DoModal()==IDOK){
	camPtr->m_filterFirstPersonMode = dlg.g_filterFirstPersonModeRef;
	camPtr->m_miscVal1 = dlg.m_miscVal1;
	camPtr->m_pitchRotation = dlg.m_pitchRotation;
	camPtr->List_hieghtBaseAspect = dlg.m_baseHieghtAspect;
    camPtr->List_fixedAfterPosX = dlg.m_fixedAfterPosX;
	camPtr->List_fixedAfterPosY = dlg.m_fixedAfterPosY;
	camPtr->List_fixedAfterPosZ = dlg.m_fixedAfterPosZ;
	camPtr->List_InitialPosX = dlg.m_InitialPosX;
	camPtr->List_InitialPosY = dlg.m_InitialPosY;
	camPtr->List_InitialPosZ = dlg.m_InitialPosZ;
	camPtr->List_floatingMovePwr = dlg.m_floatingMovePwr;
	camPtr->List_clipPlane = dlg.m_clipPlane;
	camPtr->List_fovView = dlg.m_fov;
	camPtr->List_type = dlg.cameraTypeInt;
	camPtr->List_floatingUsesCollision = dlg.floatingUsesCollision;
	camPtr->List_zoomInSpeed = dlg.m_zoomInSpeed;
	camPtr->List_zoomOutSpeed = dlg.m_zoomOutSpeed;
	camPtr->List_zoomInStop = dlg.m_zoomInStop;
	camPtr->List_floatingCamTarget.x = dlg.m_floatTargetPosX;
	camPtr->List_floatingCamTarget.y = dlg.m_floatTargetPosY;
	camPtr->List_floatingCamTarget.z = dlg.m_floatTargetPosZ;
	}
#endif
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------adds a movie for runtime---------------------------------------------//
BOOL EngineXWin::AddMovieRuntimeObject(CString fileName,RECT displayRect,BOOL looping)
{
CMovieRuntimeObj*mPtr = new CMovieRuntimeObj(FALSE,
											 looping,
		                                     fileName,
					                         displayRect,
											 displayRect
				                             );
m_gStatus->MovieRList->AddTail(mPtr);
//DirectDraw 7 video-stream initialization removed - movie entries are data-only
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------UI add movie into runtime movie list -------------------------------//
void EngineXWin::OnBTNMovieAddMovie() 
{
#ifndef _ClientOutput
CMovieSettingsDlg dlg;
 dlg.m_percTop = 5;
 dlg.m_percLeft = 5;
 dlg.m_percRight = 50;
 dlg.m_percBottom = 40;
 if(dlg.DoModal()==IDOK)
 {//if ok
    RECT rectLoc;
	dlg.movieLoops = FALSE;
	rectLoc.top = dlg.m_percTop;
	rectLoc.left = dlg.m_percLeft;
	rectLoc.right = dlg.m_percRight;
	rectLoc.bottom = dlg.m_percBottom;
   AddMovieRuntimeObject(dlg.m_movieFile,rectLoc,dlg.movieLoops);
   m_moviesDlgBar.SendDlgItemMessage(IDC_LIST_MovieList,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)dlg.m_movieFile);
 
 }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------UI test movie ------------------------------------------------------//
void EngineXWin::OnBTNMovieTestMovie() 
{
  int runtimeIndex = m_moviesDlgBar.SendDlgItemMessage(IDC_LIST_MovieList,LB_GETCURSEL ,0,0);
  if(runtimeIndex == -1){
     MessageBox("Must select a movie...");
     return;
  }
  m_gStatus->RunRuntimeMovie(runtimeIndex);
}

//----------------edits a runtime movie-----------------------------------------------//
void EngineXWin::OnBTNMovieEditMovie() 
{
#ifndef _ClientOutput
CMovieSettingsDlg dlg;
  int runtimeIndex = m_moviesDlgBar.SendDlgItemMessage(IDC_LIST_MovieList,LB_GETCURSEL ,0,0);
  if(runtimeIndex == -1){
     MessageBox("Must select a movie...");
     return;
  }
  POSITION posLoc = m_gStatus->MovieRList->FindIndex(runtimeIndex);
  if(!posLoc)
  return;	
  CMovieRuntimeObj* mPtr = (CMovieRuntimeObj*) m_gStatus->MovieRList->GetAt(posLoc);	
	
 dlg.m_movieFile = "not editable";
 dlg.m_percTop = mPtr->List_rectMov.top;
 dlg.m_percLeft = mPtr->List_rectMov.left;
 dlg.m_percRight = mPtr->List_rectMov.right;
 dlg.m_percBottom = mPtr->List_rectMov.bottom;
 dlg.movieLoops = mPtr->List_looping;
 if(dlg.DoModal()==IDOK)
 {//if ok
    mPtr->List_rectMov.top = dlg.m_percTop;
    mPtr->List_rectMov.left = dlg.m_percLeft;
    mPtr->List_rectMov.right = dlg.m_percRight;
    mPtr->List_rectMov.bottom = dlg.m_percBottom;
    mPtr->List_looping = dlg.movieLoops;
 }
 m_gStatus->CalculateAllMovieDisplayRects();
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------deletes a runtime movie---------------------------------------------//
void EngineXWin::OnBTNMovieDeleteMovie() 
{
	int runtimeIndex = m_moviesDlgBar.SendDlgItemMessage(IDC_LIST_MovieList,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select a movie...");
		return;
	}
	POSITION posLoc = m_gStatus->MovieRList->FindIndex(runtimeIndex);
	if(!posLoc)
		return;	
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CMovieRuntimeObj* mPtr = (CMovieRuntimeObj*) m_gStatus->MovieRList->GetAt(posLoc);
	delete mPtr;
	mPtr=0;
	m_gStatus->MovieRList->RemoveAt(posLoc);
	m_moviesDlgBar.SendDlgItemMessage(IDC_LIST_MovieList,LB_DELETESTRING,runtimeIndex,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------saves a runtime movie DB--------------------------------------------//
//UI
void EngineXWin::OnBTNMoviesSaveDB() 
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
    static char BASED_CODE filter[] = "MDV Files|*.MDV|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "MDB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_gStatus->MovieRList;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------loads a runtime movie DB--------------------------------------------//
//UI
void EngineXWin::OnBTNMoviesLoadDB() 
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

     static char BASED_CODE filter[] = "MDV Files|*.MDV|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadMoviesRuntimeDB(fileName); 
	 }
     SetCurrentDirectory(MainDir);
}

//----------------stops all movies that are bieng tested------------------------------//
void EngineXWin::OnBTNMovieStopMovies() 
{
//no video backend anymore - just mark every movie entry stopped
for(POSITION posLoc = m_gStatus->MovieRList->GetHeadPosition();posLoc != NULL;){//movie loop
    CMovieRuntimeObj* mPtr = (CMovieRuntimeObj*) m_gStatus->MovieRList->GetNext(posLoc);
    mPtr->List_playing = FALSE;
  }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------------Generate persist info-----------------------------------------//
BOOL EngineXWin::GeneratePersistInfo(CPersistObjList ** pList)
{
  CPersistObjList *persistList = new CPersistObjList;
  for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc != NULL;){//Begin movement entity loop
	  CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);	
      if(ptrLoc->List_baseFrame != NULL && ptrLoc->List_SpawnGeneratorLink == -1){//valid
         CPersistObj *persistPtr = new CPersistObj(ptrLoc->List_refModel,
		                                           m_gStatus->m_matrixInit2,
												   ptrLoc->List_controlType);
		 ptrLoc->List_baseFrame->GetTransform(NULL,&persistPtr->List_entMatrix);

         persistList->AddTail(persistPtr);
	  }//end valid
  }
*pList = persistList;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------------Destroy persist info-----------------------------------------//
BOOL EngineXWin::DestroyPersistInfo(CPersistObjList ** pList)
{
  CPersistObjList *persistList = *pList;
  for(POSITION posLoc = persistList->GetHeadPosition();posLoc != NULL;){//Begin movement entity loop
	  CPersistObj * pstLoc = ( CPersistObj* ) persistList->GetNext(posLoc);	
      delete pstLoc;
	  pstLoc=0;
  }	
  persistList->RemoveAll();
  delete persistList;
  persistList=0;
  *pList=0;
return TRUE;
}
//----------------Add Control Config--------------------------------------------------//
void EngineXWin::AddControlConfig(CString configName,int controlBasis)
{
ControlDBObj*cPtr = new	ControlDBObj(controlBasis,
									 configName,
		                             NULL);
cPtr->List_controlInfoList = new ControlInfoList;
m_gStatus->m_controlsList->AddTail(cPtr);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Add Event to Controller obj-----------------------------------------//
void EngineXWin::AddControllerEvent(ControlDBObj* controller,
									int controlType,
									int keyLoc,
									int eventAction,
									CString description,
									float miscFloat1,
									float miscFloat2,
									float miscFloat3)
{
ControlInfoObj*cPtr = new ControlInfoObj(controlType,
		                                 keyLoc,
				                         eventAction,
										 description,
										 miscFloat1,
										 miscFloat2,
										 miscFloat3);
controller->List_controlInfoList->AddTail(cPtr);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Add Controller Object-----------------------------------------------//
void EngineXWin::OnBTNControlAddObject() 
{
#ifndef _ClientOutput
CControlAddObjectDlg dlg;
dlg.controlType=0;
dlg.m_name = "Default";
 if(dlg.DoModal()==IDOK){
    AddControlConfig(dlg.m_name,dlg.controlType);
	m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)dlg.m_name);
 }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Add Controller Event------------------------------------------------//
void EngineXWin::OnBTNControlAddEvent() 
{
#ifndef _ClientOutput
int objectIndex = m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_GETCURSEL ,0,0);
	if(objectIndex == -1){
       MessageBox("Must select a control object");
	   return;
	}

POSITION posLoc = m_gStatus->m_controlsList->FindIndex(objectIndex);
if(posLoc == NULL)
return;//didnt find index
ControlDBObj * ptrLoc = (ControlDBObj*) m_gStatus->m_controlsList->GetAt(posLoc);
CAddControlEventDlg dlg;
 if(dlg.DoModal()==IDOK){
	//CString combination = operator+(dlg.m_comboActions,operator+("|",dlg.m_comboKeyButton));
	AddControllerEvent(ptrLoc,
		               dlg.eventBasis,
		               dlg.finalKeyNum,
					   dlg.finalActionNum,
					   dlg.m_comboActions,
					   dlg.m_miscFloat1,
					   dlg.m_miscFloat2,
					   dlg.m_miscFloat3);
    m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlEvents,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)dlg.m_comboActions);
 }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Save control cnf list-----------------------------------------------//
void EngineXWin::OnBTNControlCfgSave() 
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
     static char BASED_CODE filter[] = "CNT Control File|*.CNT|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "CNT", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
        SetCurrentDirectory(MainDir);
		CString fileName = opendialog.GetFileName();
        m_gStatus->SaveControlConfig(fileName);
	 }
	 SetCurrentDirectory(MainDir);	
}
//----------------Load Event----------------------------------------------------------//
void EngineXWin::OnBTNControlCfgLoad() 
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

   static char BASED_CODE filter[] = "CNT Files|*.CNT|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadControlCfgList(fileName);  
	   //InListControlEvents();

	   m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_RESETCONTENT,0,0);
		for(POSITION posLoc = m_gStatus->m_controlsList->GetHeadPosition(); posLoc != NULL;)
		{//control loop
			ControlDBObj * conPtr = ( ControlDBObj* ) m_gStatus->m_controlsList->GetNext(posLoc);
			m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)conPtr->List_controlConfigName);
		}
	 }
     SetCurrentDirectory(MainDir);
}

//----------------Delete Control Object-----------------------------------------------//
void EngineXWin::OnBTNControlDeleteObject() 
{
	int objectIndex = m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_GETCURSEL ,0,0);
	if(objectIndex == -1)
	{
		MessageBox("Must select a control object");
		return;
	}
	POSITION posLoc = m_gStatus->m_controlsList->FindIndex(objectIndex);
	if(posLoc == NULL)
		return;//didnt find index
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	ControlDBObj * ptrLoc = (ControlDBObj*) m_gStatus->m_controlsList->GetAt(posLoc);
	delete ptrLoc;
	ptrLoc=0;
	m_gStatus->m_controlsList->RemoveAt(posLoc);
	m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_DELETESTRING ,objectIndex,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Delete Control Event------------------------------------------------//
void EngineXWin::OnBTNControlDeleteEvent() 
{
	int objectIndex = m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_GETCURSEL ,0,0);
	if(objectIndex == -1)
	{
		MessageBox("Must select a control object");
		return;
	}
	int eventIndex = m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlEvents,LB_GETCURSEL ,0,0);
	if(eventIndex == -1)
	{
		MessageBox("Must select a control event");
		return;
	}
	POSITION posLoc = m_gStatus->m_controlsList->FindIndex(objectIndex);
	if(posLoc == NULL)
		return;//didnt find index
	
	ControlDBObj * ptrLoc = (ControlDBObj*) m_gStatus->m_controlsList->GetAt(posLoc);
	posLoc = ptrLoc->List_controlInfoList->FindIndex(eventIndex);
	if(posLoc == NULL)
		return;//didnt find index

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	ControlInfoObj * infLoc = (ControlInfoObj*) ptrLoc->List_controlInfoList->GetAt(posLoc);
	delete infLoc;
	infLoc=0;
	ptrLoc->List_controlInfoList->RemoveAt(posLoc);
	m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlEvents,LB_DELETESTRING ,eventIndex,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Initialize Control List---------------------------------------------//
void EngineXWin::InListControlEvents()
{
m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlEvents,LB_RESETCONTENT,0,0);
 int objectIndex = m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_GETCURSEL ,0,0);
	if(objectIndex == -1){
	   return;
	}
    POSITION posLoc = m_gStatus->m_controlsList->FindIndex(objectIndex);
    if(posLoc == NULL)
       return;//didnt find index
       ControlDBObj * conPtr = (ControlDBObj*) m_gStatus->m_controlsList->GetAt(posLoc);
	   for(POSITION posLoc2 = conPtr->List_controlInfoList->GetHeadPosition();posLoc2 != NULL;){//info loop
          ControlInfoObj * infoPtr = ( ControlInfoObj* ) conPtr->List_controlInfoList->GetNext(posLoc2);
		  m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlEvents,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)infoPtr->List_description);
	}
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------detect when selection changes for events----------------------------//
void EngineXWin::OnSelchangeLISTControlDB() 
{
InListControlEvents();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Edits control objects-----------------------------------------------//
void EngineXWin::OnBTNControlEditObject() 
{
#ifndef _ClientOutput
int objectIndex = m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_GETCURSEL ,0,0);
	if(objectIndex == -1){
       MessageBox("Must select a control object");
	   return;
	}
POSITION posLoc = m_gStatus->m_controlsList->FindIndex(objectIndex);
if(posLoc == NULL)
return;//didnt find index
ControlDBObj * ptrLoc = (ControlDBObj*) m_gStatus->m_controlsList->GetAt(posLoc);
CControlAddObjectDlg dlg;
dlg.GL_invertMouse=ptrLoc->m_invertMouse;
dlg.controlType=ptrLoc->List_controlBasis;
dlg.m_name = ptrLoc->List_controlConfigName;
 if(dlg.DoModal()==IDOK){
  ptrLoc->List_controlBasis = dlg.controlType;
  ptrLoc->List_controlConfigName = dlg.m_name;
  ptrLoc->m_invertMouse = dlg.GL_invertMouse;
  m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_DELETESTRING ,objectIndex,0);
  m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_INSERTSTRING ,objectIndex,(LPARAM)(LPCTSTR)ptrLoc->List_controlConfigName);
 }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Edits control events------------------------------------------------//
void EngineXWin::OnBTNControlEditEvent() 
{
#ifndef _ClientOutput
int objectIndex = m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_GETCURSEL ,0,0);
	if(objectIndex == -1){
       MessageBox("Must select a control object");
	   return;
	}
int eventIndex = m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlEvents,LB_GETCURSEL ,0,0);
	if(eventIndex == -1){
       MessageBox("Must select a control event");
	   return;
	}
POSITION posLoc = m_gStatus->m_controlsList->FindIndex(objectIndex);
if(posLoc == NULL)
return;//didnt find index
ControlDBObj * ptrLoc = (ControlDBObj*) m_gStatus->m_controlsList->GetAt(posLoc);
posLoc = ptrLoc->List_controlInfoList->FindIndex(eventIndex);
if(posLoc == NULL)
return;//didnt find index
ControlInfoObj * infLoc = (ControlInfoObj*) ptrLoc->List_controlInfoList->GetAt(posLoc);
CAddControlEventDlg dlg;
    dlg.m_miscFloat1 = infLoc->List_miscFloat1;
	dlg.m_miscFloat2 = infLoc->List_miscFloat2;
	dlg.m_miscFloat3 = infLoc->List_miscFloat3;
    dlg.finalKeyNum = infLoc->List_buttonDown;
	dlg.eventBasis = infLoc->List_controlType;
    dlg.finalActionNum = infLoc->List_action;

if(dlg.DoModal()==IDOK){
	infLoc->List_miscFloat1 = dlg.m_miscFloat1;
	infLoc->List_miscFloat2 = dlg.m_miscFloat2;
	infLoc->List_miscFloat3 = dlg.m_miscFloat3;
    infLoc->List_buttonDown = dlg.finalKeyNum;
	infLoc->List_controlType = dlg.eventBasis;
    infLoc->List_action = dlg.finalActionNum;
	CString combination = operator+(dlg.m_comboActions,operator+("|",dlg.m_comboKeyButton));
    infLoc->List_description = dlg.m_comboActions;
    m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlEvents,LB_DELETESTRING ,eventIndex,0);
	m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlEvents,LB_INSERTSTRING,eventIndex,(LPARAM)(LPCTSTR)dlg.m_comboActions);
    
  }
#endif
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Fully loads an object-----------------------------------------------//
BOOL EngineXWin::LoadWorldObject(CString fileName,CWldObject ** retLoc)
{
 int groupIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_GETCURSEL ,0,0);
 if(groupIndex == -1){
	groupIndex=0;
 }
 else{
  POSITION posLoc = m_gStatus->m_worldGroupList->FindIndex(groupIndex);
  groupIndex=0;
   if(posLoc){
     CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetAt(posLoc);
     groupIndex = wldPtr->List_groupInt;
   }
 }


          CWldObject * ptrLoc = new CWldObject((m_gStatus->GlobalTraceNumber++));  
		  ptrLoc->List_groupNumber = groupIndex;
		  ptrLoc->List_triggerList = new CTriggerObjectList;
		  //load imediate mode mesh data
		     ptrLoc->m_fileNameRef = fileName;
             ptrLoc->m_meshObject = new CEXMeshObj();
			 ptrLoc->m_meshObject->List_materialObject = new CMaterialObject();
             float sightRange = 0.0f;
             ptrLoc->m_meshObject->LoadGeometryFromPlugin(fileName,&sightRange,m_gStatus->m_dxRenderer->m_pd3dDevice,&ptrLoc->List_popoutBox);

          //initialize mesh and frames and set texture correctness
          if(m_gStatus->InitWorldObject(ptrLoc)==TRUE){
		     m_gStatus->m_wldObjList->AddTail(ptrLoc);
#ifndef _ClientOutput
		     PushEditorUndoObjectTrace(ptrLoc->List_identity);
#endif
			 *retLoc = ptrLoc;
			 ptrLoc->List_popoutRadius = ptrLoc->m_meshObject->m_meshBoundingSphereRadius;

             /*ptrLoc->List_popoutBox.max.x =  ((sightRange+1)+ptrLoc->m_meshObject->m_meshCenter.x);
			 ptrLoc->List_popoutBox.max.y =  ((sightRange+2)+ptrLoc->m_meshObject->m_meshCenter.y);
			 ptrLoc->List_popoutBox.max.z =  ((sightRange+3)+ptrLoc->m_meshObject->m_meshCenter.z);
			 ptrLoc->List_popoutBox.min.x = -(sightRange+4)+ptrLoc->m_meshObject->m_meshCenter.x;
			 ptrLoc->List_popoutBox.min.y = -(sightRange+5)+ptrLoc->m_meshObject->m_meshCenter.y;
			 ptrLoc->List_popoutBox.min.z = -(sightRange+6)+ptrLoc->m_meshObject->m_meshCenter.z;*/

			 if(ptrLoc->m_meshObject->List_materialObject->m_peramCount > 0)
			 {
                ptrLoc->m_meshObject->List_materialObject->m_pixelShader = m_gStatus->m_pxShaderSystem->LoadShaderByStringData(ptrLoc->m_meshObject->List_materialObject->m_fxBindString,m_gStatus->m_dxRenderer->m_pd3dDevice,ptrLoc->m_meshObject->List_materialObject->m_fxBindString);
			 }

			 //HTREEITEM Root = AddTreeRoot(operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity))),IDC_OBJECT_TREE,&m_dlgBarObject);   
	         //AddTreeChild(ptrLoc->m_fileNameRef,IDC_OBJECT_TREE,&m_dlgBarObject,Root);  
		  }
		  else
		  return FALSE;

return TRUE;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Shpere collision------------------------------------------------------//
void EngineXWin::SphereCollideSlide(CFrameObj* frame2,CFrameObj* frame,float speed,float radiusesCombined)
{
D3DVECTOR vectorDir;
frame->GetPosition(frame2,&vectorDir);
float scale = (float)sqrt((vectorDir.x * vectorDir.x) + (vectorDir.y * vectorDir.y) + (vectorDir.z * vectorDir.z));
if(scale == 0.0f)
return;
vectorDir.x = vectorDir.x / scale;
vectorDir.y = vectorDir.y / scale;
vectorDir.z = vectorDir.z / scale;
//vectorDir.x = vectorDir.x;
//vectorDir.y = vectorDir.y;
//vectorDir.z = vectorDir.z;
float diff = radiusesCombined - scale;

float tranx = (.1f + diff) * vectorDir.x;
float trany = (.1f + diff) * vectorDir.y;
float tranz = (.1f + diff) * vectorDir.z;
frame->AddTranslationAfter( tranx,trany,tranz);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Object Array Add------------------------------------------------------//
void EngineXWin::OnBTNObjectArrayAdd() 
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
	 {//file chosen
	      CString baseName = opendialog.GetFileName();
		  //get first file then loop till done
		  CString ext = m_gStatus->m_cabLib.GetFileExt(baseName);
		  ext = operator+(".",ext);
		  m_gStatus->m_cabLib.RemoveFileExt(&baseName);
          int curCount=0;
		  while(1){ 
			CString fileName = baseName;
			CString fileNum = m_gStatus->m_cabLib.ConvToString(curCount+1);
			if(curCount>0)//append if over 0
			fileName = operator+(baseName,fileNum);
            m_gStatus->m_cabLib.AddNewFileExt(&fileName,ext);
            CWldObject * retBasisLoc = NULL;
            if(LoadWorldObject(fileName,&retBasisLoc)==FALSE)
			   break;
			curCount++;
		  }
	 }// end file chosen

SetCurrentDirectory(MainDir);	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Popout::entity optimization-------------------------------------------//
void EngineXWin::Popout_Entity(CViewportObj* mptLoc)
{



}
//--------------Advanced Rener Pipeline settings--------------------------------------//
void EngineXWin::OnBTNUIBaseAdvanced() 
{
#ifndef _ClientOutput 
	CGlobalAdvancedDlg dlg;
	dlg.mouseSelectionMethodOn = m_gStatus->m_selctionModeOn;
	dlg.m_interpolationGlobal = m_gStatus->m_globalShadowType;
	dlg.m_targetFps = (int)m_gStatus->fps_cap;
    dlg.m_resolutionHieght = ResolutionHieght;
    dlg.m_resolutionWidth = ResolutionWidth;
	dlg.w_bufferPreffered = m_gStatus->W_BufferPreffered;
	dlg.calcEntityCollision = EntityCollisionOn;
	dlg.m_shaderAvailable = m_gStatus->m_shaderSystem;
	dlg.m_currentWorldShader = m_gStatus->m_globalWorldShader;
	if(dlg.DoModal()==IDOK){
	m_gStatus->W_BufferPreffered = dlg.w_bufferPreffered;
	m_gStatus->m_globalWorldShader = dlg.m_currentWorldShader;
	m_gStatus->m_renderStateObject->SetShader(m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_shaderSystem,m_gStatus->m_globalWorldShader);
	m_gStatus->fps_cap = (float)dlg.m_targetFps;
	m_gStatus->m_globalShadowType = dlg.m_interpolationGlobal;
	ResolutionHieght = dlg.m_resolutionHieght;
    ResolutionWidth = dlg.m_resolutionWidth;
	m_gStatus->m_selctionModeOn = dlg.mouseSelectionMethodOn;
	EntityCollisionOn = dlg.calcEntityCollision;
	m_gStatus->SetAllRenderStatesIM();
	}	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Advanced Properties entity--------------------------------------------//
void EngineXWin::OnBTNENTITYProperties() 
{
#ifndef _ClientOutput
int indexLoc=0;
BOOL foundIt = FALSE; 
CMovementObj * mPtr;
   //get selected index
    POSITION posLoc;
    for(posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp){
           foundIt = TRUE;
		   break;
		}
		indexLoc++;
	}
	if(foundIt == FALSE){
       MessageBox("Must select a entity base object");
	   return;
	}
	//get database object
    posLoc = m_gStatus->m_entObjList->FindIndex(indexLoc);
    if(!posLoc)
       return;
    CMovementObj *ptrLoc = dynamic_cast<CMovementObj*>(m_gStatus->m_entObjList->GetAt(posLoc));	
    CEntityPropertiesDlg dlg;
	dlg.m_selectionEffectTest = ptrLoc->m_selectionEffectState;
	dlg.m_filterLodBalancing = ptrLoc->m_filterLodBalancing;
	dlg.m_hardPointOverd = ptrLoc->m_hardPointOverdY;
	dlg.m_shadowPref     = ptrLoc->m_showShadow;
	dlg.firstPersonModel = ptrLoc->List_firstPersonModel;
	dlg.m_popoutRadius = ptrLoc->List_popoutRadius;
	dlg.m_energy = ptrLoc->List_maxEnergy;
	dlg.m_hudLinkThirdPersonSpecific = ptrLoc->m_hudLinkThirdPerson;
	dlg.m_fullModelRef = ptrLoc->List_fullModelRef;
	dlg.m_thirdPersonCameraIndex = ptrLoc->List_camThirdPerson;
    dlg.m_firstPersonCameraIndex = ptrLoc->List_camFirstPerson;
	dlg.m_firstPersonModelRef = ptrLoc->List_firstPersonRef;
    dlg.m_deathCamIndex = ptrLoc->List_deathCam;
	dlg.m_hudLink = ptrLoc->List_hudLink;
    dlg.m_meshRadius = ptrLoc->List_boundingRadius;
    dlg.m_mountBoneIndex = ptrLoc->m_ridable;
	dlg.m_team           = ptrLoc->m_team;
	dlg.m_entityName     = ptrLoc->List_name;
	dlg.m_raceID         = ptrLoc->m_raceID;
	dlg.disableCollision = ptrLoc->m_filterCollision;
	dlg.m_svrPhysics = ptrLoc->m_primaryPhysicsOverd;
	dlg.m_collisionType = ptrLoc->m_collisionType;
    if(dlg.DoModal()==IDOK)
	{
	   ptrLoc->m_selectionEffectState = dlg.m_selectionEffectTest;
	   ptrLoc->m_primaryPhysicsOverd = dlg.m_svrPhysics;
	   ptrLoc->m_filterCollision = dlg.disableCollision;
	   ptrLoc->List_name = dlg.m_entityName;
       ptrLoc->m_team    = dlg.m_team;
       ptrLoc->m_ridable = dlg.m_mountBoneIndex;
	   ptrLoc->List_boundingRadius = dlg.m_meshRadius;
	   ptrLoc->List_deathCam = dlg.m_deathCamIndex;
	   ptrLoc->List_camThirdPerson = dlg.m_thirdPersonCameraIndex;
       ptrLoc->List_camFirstPerson = dlg.m_firstPersonCameraIndex;
	   ptrLoc->List_firstPersonRef = dlg.m_firstPersonModelRef;
	   ptrLoc->List_fullModelRef = dlg.m_fullModelRef;
	   ptrLoc->List_firstPersonModel = dlg.firstPersonModel;
	   ptrLoc->List_maxEnergy = dlg.m_energy;
	   ptrLoc->List_popoutRadius = dlg.m_popoutRadius;
	   ptrLoc->m_hudLinkThirdPerson = dlg.m_hudLinkThirdPersonSpecific;
	   ptrLoc->List_hudLink = dlg.m_hudLink;
	   ptrLoc->m_raceID = dlg.m_raceID;
	   ptrLoc->m_hardPointOverdY = dlg.m_hardPointOverd;
	   ptrLoc->m_collisionType = dlg.m_collisionType;
	   ptrLoc->m_showShadow = dlg.m_shadowPref;
	   ptrLoc->m_filterLodBalancing = dlg.m_filterLodBalancing;

	}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Advanced Properties objects-------------------------------------------//
void EngineXWin::OnBTNOBJECTProperties() 
{
WorldObjectProperties();
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------adds a light object---------------------------------------------------//
void EngineXWin::OnBtnObjAddlight() 
{
#ifndef _ClientOutput
  HWND treeHwnd = NULL;
  m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
  HTREEITEM item = TreeView_GetSelection(treeHwnd); 
  CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);

  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){
	   CLightInterfaceDlg dlg;
	   dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	   dlg.m_lightREF = ptrLoc->m_lightObject;
	   if(dlg.DoModal()==IDOK)
		  ptrLoc->m_lightObject = dlg.m_lightREF;
	return;
	}
  }//end Begin world object loop
  MessageBox("Object Not Found");
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------destroy light object--------------------------------------------------//
void EngineXWin::OnBTNOBJDeleteLight() 
{
	HWND treeHwnd = NULL;
	m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
	HTREEITEM item = TreeView_GetSelection(treeHwnd); 
	CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);

	for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;)
	{//Begin world object loop
		CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
		CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
		if(tracename == textLoc)
		{
			if(ptrLoc->m_lightObject)
			{			
				if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
					return;
				
				delete ptrLoc->m_lightObject;
				ptrLoc->m_lightObject = 0;
			}
			return;
		}
	}//end Begin world object loop
	
	MessageBox("Object Not Found");	
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------destroy a light interface---------------------------------------------//
void EngineXWin::OnBTNENTDestroyLight() 
{
	#ifndef _ClientOutput
    CAddEntityDlg dlg;
	dlg.saveCollisionInfo = TRUE;
    if(dlg.DoModal()!=IDOK)
	   return;


	HWND treeHwnd = NULL;
    m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    POSITION delPos;
    for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();(delPos = posLoc) != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp){
			
            mPtr->LoadFromDir(dlg.m_pathGlobal,dlg.m_folderPath,m_gStatus->m_dxRenderer->m_pd3dDevice,FALSE);
			InitFXSahdersOnSkeletal(mPtr->List_skeletalObject);
			m_gStatus->SkeletalTextureReassignmentWCompile(mPtr->List_skeletalObject);
		}
	}
	  //SetCurrentDirectory(MainDir);
	  //-------------------------------
	  //-------------------------------
#endif
}
//--------------assign track to scene-------------------------------------------------//
void EngineXWin::OnBTNMusicAssignTrack() 
{
}
//--------------assign wav to scene---------------------------------------------------//
void EngineXWin::OnBTNMusicAssignWav() 
{
m_musicDlgBar.GetDlgItemText( IDC_EDIT_Music_WavFile,m_gStatus->m_musicLoopFile);	
}
//--------------unassign track from scene---------------------------------------------//
void EngineXWin::OnBTNMusicUnassignTrack() 
{
}
//--------------unassign wav from world-----------------------------------------------//
void EngineXWin::OnBTNMusicUnassignWav() 
{
m_gStatus->m_musicLoopFile = "NONE";
m_musicDlgBar.SetDlgItemText(IDC_EDIT_Music_WavFile,"NONE");	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Browse for a wav file in music dir------------------------------------//
void EngineXWin::OnBTNMusicBrowseWav() 
{
	//use sounds folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\Sounds\\";
			 SetCurrentDirectory(CurDir);
    //end use sounds folder for organization
    static char BASED_CODE filter[] = "wav Files|*.wav|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   m_musicDlgBar.SetDlgItemText( IDC_EDIT_Music_WavFile,fileName);
	   UpdateData(FALSE);
	 }
    SetCurrentDirectory(MainDir);		
}



//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------adjusts entity based on collision return------------------------------//
void EngineXWin::CollisionAdjust(CMovementObj * entLoc,int howManyNormals,
								 float storedDistance[20],D3DVECTOR storedNormals[20])
{
//entLoc->List_movementDB->List_curTransY = 0.0f;
	            D3DVECTOR currentNormal;
                currentNormal.x=0;currentNormal.y=0;currentNormal.z=0;
                float tranx =  0.0f;
                float trany =  0.0f;
                float tranz =  0.0f;
	            int howManyDifferentNormals = 0;

	              if(howManyNormals > 1)
	                 for(int loop=0;loop<howManyNormals;loop++){//normal loop
	                   BOOL noDuplicateFound = TRUE;
		               BOOL validLoop = FALSE;
                         for(int index=loop;index<howManyNormals;index++){//normal loop2
		                     if(loop != index)
							 {
				                validLoop = TRUE;
			                    if(storedNormals[loop].x == storedNormals[index].x && storedNormals[loop].y == storedNormals[index].y && storedNormals[loop].z == storedNormals[index].z)
								{
                                   noDuplicateFound = FALSE;
								}
							 }
						 }//end normal loop2
		                 if(noDuplicateFound == TRUE && validLoop == TRUE)
			                howManyDifferentNormals++;
					 }//end normal loop
	 
	                 if(howManyDifferentNormals == 0){
                        tranx =  (storedDistance[0]) * storedNormals[0].x;
                        trany =  (storedDistance[0]) * storedNormals[0].y;
                        tranz =  (storedDistance[0]) * storedNormals[0].z;
						entLoc->List_baseFrame->AddTranslationAfter(tranx,trany,tranz);
					 }
	                 else{
						//float currentDistance = storedDistance[0];
						float currentDistance = 0.0f;
		                for(int loop=0;loop<howManyNormals;loop++){//normal loop
                            currentNormal.x += storedNormals[loop].x;
                            currentNormal.y += storedNormals[loop].y;
                            currentNormal.z += storedNormals[loop].z;
	                        currentDistance += storedDistance[loop];
							//if(currentDistance > storedDistance[loop])
							//   currentDistance = storedDistance[loop];
						}//end normal loop
                     float scale = (float)sqrt((currentNormal.x * currentNormal.x) + (currentNormal.y * currentNormal.y) + (currentNormal.z * currentNormal.z));
					 if(scale == 0){
						 //MessageBox("Divd by 0");
						 entLoc->List_baseFrame->SetPosition2(NULL,entLoc->List_lastPosition.x,entLoc->List_lastPosition.y,entLoc->List_lastPosition.z);
					     return;
					 }
					       currentNormal.x = currentNormal.x / scale;
                           currentNormal.y = currentNormal.y / scale;
                           currentNormal.z = currentNormal.z / scale;

                           currentDistance = currentDistance / howManyNormals;
                           tranx =  (currentDistance) * currentNormal.x;
                           trany =  (currentDistance) * currentNormal.y;
                           tranz =  (currentDistance) * currentNormal.z;
						      
						   entLoc->List_baseFrame->AddTranslationAfter(tranx,trany,tranz);
					 }
			 
}

//--------------initialize a DB missile Object in DB----------------------------------//
/*BOOL EngineXWin::InitDBMissileObject(CMissileObj* mislPtr)
{
  
  return TRUE;
}*/




//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Browse filename missile Interface---------------------------------------------//
/*void EngineXWin::OnBTNMISBrowseFileName() 
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
	   ConvertToX(&fileName);
	   m_missileDlgBar.SetDlgItemText( IDC_EDIT_MIS_FileName,fileName);
	   UpdateData(FALSE);
	 }
SetCurrentDirectory(MainDir);		
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------save missile Database----------------------------------------------------//
void EngineXWin::OnBTNMISsave() 
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
       the_out_Archive << m_gStatus->m_missileDB;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------missile object material--------------------------------------------------//
void EngineXWin::OnBTNMISLoad() 
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
	   m_gStatus->LoadMissileDatabase(fileName);
       if(m_gStatus->m_editorModeOn == TRUE)
	      MissileDatabaseInList(-1);
	 }
	 SetCurrentDirectory(MainDir);
}*/

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------interface add bullet hole---------------------------------------------//
void EngineXWin::OnBTNBHAddHole() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Browse for file bullet hole-------------------------------------------//
void EngineXWin::OnBTNBHBrowse() 
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
	   CString fileName = opendialog.GetFileName();
	   m_bulletHoleDlgBar.SetDlgItemText( IDC_EDIT_BH_FileName,fileName);
	   UpdateData(FALSE);
	 }
SetCurrentDirectory(MainDir);		
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Browse for file bullet hole-------------------------------------------//
void EngineXWin::OnBTNBHAlphaTexture() 
{
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Browse for file bullet hole-------------------------------------------//
void EngineXWin::OnBTNBHDeleteHole() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Browse for file bullet hole-------------------------------------------//
void EngineXWin::OnBTNBHMaterial() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------load a file bullet hole-----------------------------------------------//
void EngineXWin::OnBTNBHSave() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Browse for file bullet hole-------------------------------------------//
void EngineXWin::OnBTNBHload() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------load bullet hole from file--------------------------------------------//
void EngineXWin::OnBTNEntityshadow() 
{
#ifndef _ClientOutput
/*HWND treeHwnd = NULL;
    m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
    CString textLoc = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,10);
  for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//Begin entity loop
	  CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);	
	  CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_traceNumber)));
	  if(tracename == textLoc){
		  
		  CShadowDlg dlg;
		  dlg.shadowEnabled = ptrLoc->List_skeletalObject->m_shadowOn;
		  dlg.m_above = 5;
          dlg.m_depth = 20;
	      dlg.m_radius = 10;
		  dlg.m_collisionRadius = 15;
		  if(ptrLoc->List_skeletalObject->m_shadowInterface){
		  dlg.m_textureIndex = ptrLoc->List_skeletalObject->m_shadowInterface->m_textureIndex;
		  dlg.m_above = ptrLoc->List_skeletalObject->m_shadowInterface->m_extentUp;
          dlg.m_depth = ptrLoc->List_skeletalObject->m_shadowInterface->m_extentDown;
	      dlg.m_radius = ptrLoc->List_skeletalObject->m_shadowInterface->m_originalRadius;
		  dlg.m_collisionRadius = ptrLoc->List_skeletalObject->m_shadowInterface->m_collisionRadius;
		  }
		  
		  if(ptrLoc->List_skeletalObject->m_shadowOn == TRUE){
			  dlg.m_above           = ptrLoc->List_skeletalObject->m_shadowInterface->m_extentUp;
              dlg.m_depth           = ptrLoc->List_skeletalObject->m_shadowInterface->m_extentDown;
			  dlg.m_radius          = ptrLoc->List_skeletalObject->m_shadowInterface->m_originalRadius;
		      dlg.m_collisionRadius = ptrLoc->List_skeletalObject->m_shadowInterface->m_collisionRadius;
		  }
		  if(dlg.DoModal()==IDOK){
              POSITION texPositionLoc = m_gStatus->m_textureDataBase->FindIndex(dlg.m_textureIndex);
			  if(texPositionLoc){
			  CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetAt(texPositionLoc);
			  ptrLoc->List_skeletalObject->EnableShadow(dlg.shadowEnabled,
				                                         dlg.m_radius,
														 dlg.m_collisionRadius,
														 dlg.m_depth,
														 dlg.m_above,
														 dlg.m_textureIndex,
														 txtPtrLoc->List_textureName);
			  }
			  else
			  MessageBox("Invalid textureIndex");
		  }
		  return;
	  }
  }//end entity object loop
  MessageBox("Must have a trace selected...");	*/
#endif
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------get the object shadow plane-------------------------------------------//
void EngineXWin::OnBTNObjectShadow() 
{
//waiting on new shadow code
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------world objects defined as environment variables callback---------------//
void EngineXWin::WorldEnvironmentUpdate(CFrameObj* frame)
{
/*D3DVECTOR position;
frame->GetPosition(NULL,&position);//get frame position

int howMany=0;
  for(POSITION posLoc = m_gStatus->m_worldRenderList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_worldRenderList->GetNext(posLoc);	
    if(ptrLoc->List_environmentAttached == TRUE){
	  if(ptrContent->List_mainFrame)
         ptrContent->List_mainFrame->SetPosition2(NULL,position.x,position.y,position.z);
	}
  }*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------animation properies-determines run/jump-------------------------------//
void EngineXWin::OnBTNENTAnimProperties() 
{ 
	HWND treeHwnd = NULL;
    m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
	int indexOfObject = GetTreeChildIndex(treeHwnd,item);	
	HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
    CString textLoc = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,ObjectRoot,10);
    for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;)
	{//Begin entity object loop
	  CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);	
	  CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->m_uniqueId)));
	  if(tracename == textLoc){
         CStatisticsDBDlg dlg;

		 if(!ptrLoc->m_stats)
		     ptrLoc->m_stats = new CStatObjList();

         dlg.statisticDBREF = ptrLoc->m_stats;
         dlg.DoModal();
         ptrLoc->m_stats = dlg.statisticDBREF;
	  }
	}

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------utilizes the attribute action of a object see CattributeClass---------//
void EngineXWin::ExecAttributeOfObject(CWldObject * wldObjLoc)
{
//ok
if(!wldObjLoc->List_attribute)
   return;

				 //int List_otherAttributeType;
				 //int List_otherAttributeValue;
				 //int List_otherObjectSelect;
wldObjLoc->List_attribute->List_inUse = TRUE;
wldObjLoc->List_attribute->List_retractDelayVarial=0;

if(wldObjLoc->List_attribute->List_selfAttributeType == 1){
  
}
if(wldObjLoc->List_attribute->List_selfAttributeType == 2){
   
}

if(wldObjLoc->List_attribute->List_otherAttributeType == 1){
	
	///return;
}

if(wldObjLoc->List_attribute->List_otherAttributeType == 2){
   
	//return;
}

}
//-------------sound entity button stuff----------------------------------------------//
void EngineXWin::OnBTNEntitySound() 
{
#ifndef _ClientOutput
HWND treeHwnd = NULL;
    m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
	int indexOfObject = GetTreeChildIndex(treeHwnd,item);	
	HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
    CString textLoc = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,ObjectRoot,10);
  for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	  CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);	
	  CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->m_uniqueId)));
	  if(tracename == textLoc){
	     /*POSITION contPtrLoc = ptrLoc->List_visualBase->FindIndex(indexOfObject);
	     CContent * ptrContent = (CContent *) ptrLoc->List_visualBase->GetAt(contPtrLoc);
	     CSoundSettingsDlg dlg;
		  dlg.enabled = ptrContent->List_useSound;
		  dlg.m_fileName = ptrContent->List_soundFileName;
		  dlg.m_distanceFactor = ptrContent->List_soundFactor;
		  dlg.looping = ptrContent->List_soundLooping;
		  if(dlg.DoModal()==IDOK){
			 ptrContent->List_soundLooping = dlg.looping;
			 ptrContent->List_useSound = dlg.enabled;
		     ptrContent->List_soundFileName = dlg.m_fileName;
		     ptrContent->List_soundFactor = dlg.m_distanceFactor;
		     UpdateContentObject(ptrContent);
		  }*/
	return;
	}
  }//end Begin world object loop
  MessageBox("Not A Valid Object...");	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------clone sound objects----------------------------------------------------//
void EngineXWin::OnBTNEntitysetSoundCenter() 
{
  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------sets a sound to an object----------------------------------------------//
void EngineXWin::OnBTNOBJSound() 
{
#ifndef _ClientOutput
HWND treeHwnd = NULL;
m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd); 
CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);

  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){
       CSoundSettingsDlg dlg;
		  dlg.enabled = ptrLoc->List_useSound;
		     if(ptrLoc->m_soundRange){
		        dlg.m_fileName = ptrLoc->m_soundRange->m_fileName;
			 }
		     dlg.m_distanceFactor = ptrLoc->List_soundFactor;
		  if(dlg.DoModal()==IDOK){
			 
              
		  }
	return;
	}
  }//end Begin world object loop
  MessageBox("Object Not Found");	
#endif
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------applys an alpha texture to wld object----------------------------------//
void EngineXWin::OnBTNOBJAlphaTexture() 
{
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------tear down world objects------------------------------------------------//
void EngineXWin::TearDownWorldObjects()
{
for(POSITION worldPos1 = m_gStatus->m_wldObjList->GetHeadPosition();worldPos1 != NULL;){//Begin world object loop
	CWldObject * wldLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(worldPos1);	
	wldLoc->SafeDelete();
 }//end Begin world object loop
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------gives or edits a animation sound---------------------------------------//
void EngineXWin::OnBTNENTAnimSound() 
{

}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Adds other entities objects to current runtime for temp use-guns ect--//
void EngineXWin::OnBTNEntityCombineEntityVis() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------fires missile from entity---------------------------------------------//
BOOL EngineXWin::EntityFireNow(CMovementObj* ptrLocal)
{

ptrLocal->List_currentlyFireing = TRUE;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------fires missile from entity---------------------------------------------//
void EngineXWin::OnBTNENTContentProperties() 
{
 




}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------Texture Swap Object Add------------------------------------------------//
BOOL EngineXWin::AddTextureSwapCollection(CString collectionName,
										  int totalImages,
										  BOOL alphaTexture,
										  BOOL decalTex,
										  int alphaMapSize,
										  float decalRed,
										  float decalGreen,
										  float decalBlue,
										  int delay,
										  int alphaChannel)
{
//contruct basis
CTextureSwapObj* txtObjPtr = new CTextureSwapObj(NULL,//CBitmapObjList * L_bitmapList,
		                                         collectionName,
					                             totalImages,
												 alphaTexture,
												 decalTex,
					                             delay,
					                             0,
					                             alphaMapSize,
					                             decalRed,
					                             decalGreen,
					                             decalBlue,
												 alphaChannel
				                                 );
CString fileExtension =  m_gStatus->m_cabLib.GetFileExt(collectionName);
//add images
txtObjPtr->List_bitmapList = new CBitmapObjList;
for(int loop=1;loop<=totalImages;loop++){//image object load
CString finalName = collectionName;
m_gStatus->m_cabLib.RemoveFileExt(&finalName);
if(loop != 1)
finalName = finalName+m_gStatus->m_cabLib.ConvertLongToString(loop);
finalName = finalName+".";
finalName = finalName+fileExtension;
char temp[256];
lstrcpy(temp, finalName);
CBitmapObj * bmpObj = new CBitmapObj(temp);
txtObjPtr->List_bitmapList->AddTail(bmpObj);
}//end image object load
//BOOL result = InitCollection(txtObjPtr);
//if(result==TRUE)
m_gStatus->TextureSwapList->AddTail(txtObjPtr);

return TRUE;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------Add Collection----------------------------------------------------//
void EngineXWin::OnBTNTEXAddCollection() 
{
#ifndef _ClientOutput
CAnimTextureSwpAddDlg dlg;
dlg.blueSrc = 0.0f;
dlg.redSrc = 0.0f;
dlg.greenSrc = 0.0f;
dlg.m_howManyTextures = 0;
dlg.m_alphaMapSize = 64;
if(dlg.DoModal()==IDOK){
  if(dlg.m_howManyTextures == 0)
	 return;
AddTextureSwapCollection(dlg.m_textureName,
						 dlg.m_howManyTextures,
						 dlg.useAlphaTexture,
					     dlg.useDecalTransparency,
						 dlg.m_alphaMapSize,
						 dlg.redSrc,
						 dlg.greenSrc,
						 dlg.blueSrc,
						 dlg.m_delay,
						 dlg.m_alphaChannel);

   m_animatedTextureDBDlgBar.SendDlgItemMessage(IDC_LIST_TEX_Collection,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("Map Basis :: ",dlg.m_textureName));
}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------delete collection-------------------------------------------------//
void EngineXWin::OnBTNTEXDeleteCollection() 
{
	int runtimeIndex = m_animatedTextureDBDlgBar.SendDlgItemMessage(IDC_LIST_TEX_Collection,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select collection");
		return;
	}
	POSITION posLocal = m_gStatus->TextureSwapList->FindIndex(runtimeIndex);
	if(!posLocal)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CTextureSwapObj* txtObjPtr = (CTextureSwapObj*) m_gStatus->TextureSwapList->GetAt(posLocal);
	m_gStatus->DestroyTextureCollection(txtObjPtr);
	if(txtObjPtr)
		delete txtObjPtr;
	txtObjPtr=0;
	m_gStatus->TextureSwapList->RemoveAt(posLocal);
	if(m_gStatus->m_editorModeOn == TRUE)
		m_animatedTextureDBDlgBar.SendDlgItemMessage(IDC_LIST_TEX_Collection,LB_DELETESTRING,runtimeIndex,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------load collection---------------------------------------------------//
void EngineXWin::OnBTNTEXLoadCollection() 
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
   static char BASED_CODE filter[] = "TXA Files|*.TXA|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();  
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadAnimTextureSwapDB(fileName);	   
	 }
	 SetCurrentDirectory(MainDir);		
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------save collection---------------------------------------------------//
void EngineXWin::OnBTNTEXSaveCollection() 
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
     static char BASED_CODE filter[] = "TXA Files|*.TXA|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "TXA", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_gStatus->TextureSwapList;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  	
}
//-----------Add script event to a list-----------------------------------------------//
void EngineXWin::AddScriptEvent(int attribute,CString miscString,int miscInt,CEXScriptObjList * scriptList)
{
CEXScriptObj * scrPtr = new CEXScriptObj(attribute,
		                                 miscString,
		                                 miscInt
		  	                             );

scriptList->AddTail(scrPtr);
}
//-----------Run all attributes-------------------------------------------------------//
BOOL EngineXWin::SaveScriptFile(CString fileName,CEXScriptObjList * scriptList)
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
CFileException exc;
   CFile fl;
   BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
   if(res == FALSE){
	  SetCurrentDirectory(MainDir);
	  return FALSE;
   }
   CArchive the_out_Archive(&fl,CArchive::store);
   the_out_Archive << scriptList;
   the_out_Archive.Close();
   fl.Close();
   SetCurrentDirectory(MainDir);
   return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Load Entity Databse -----------------------------------------------------//
void EngineXWin::OnBTNENTLoadDB() 
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

   static char BASED_CODE filter[] = "EDB Files|*.EDB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadEntityDatabase(fileName);
	   m_quickEntityDatabasePath = fileName;

	      EnitityInList();

	 }
     SetCurrentDirectory(MainDir);	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
// Make a dated copy before a database save can replace the existing file.  A
// failed backup blocks the save, so an I/O or permissions problem cannot silently
// destroy the last good EDB.
static BOOL EngineXCreateDatabaseBackup(const CString & filePath, CString & backupPath, CString & error)
{
    backupPath.Empty();
    error.Empty();
    DWORD attributes = GetFileAttributesA(filePath);
    if(attributes == INVALID_FILE_ATTRIBUTES)
    {
        DWORD code = GetLastError();
        if(code == ERROR_FILE_NOT_FOUND || code == ERROR_PATH_NOT_FOUND)
            return TRUE; // this is the first save; there is nothing to back up
        error.Format("Could not check the existing database (Windows error %lu).", code);
        return FALSE;
    }
    if(attributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        error = "The selected database path is a folder, not a file.";
        return FALSE;
    }

    int slash = filePath.ReverseFind('\\');
    if(slash < 0)
        slash = filePath.ReverseFind('/');
    CString parent = slash >= 0 ? filePath.Left(slash) : CString(".");
    CString baseName = slash >= 0 ? filePath.Mid(slash + 1) : filePath;
    CString backupDir = parent + "\\Backups";
    if(!CreateDirectoryA(backupDir, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
    {
        error.Format("Could not create the backup folder:\n%s\n\nWindows error %lu.",
                     (const char *)backupDir, GetLastError());
        return FALSE;
    }

    SYSTEMTIME now;
    GetLocalTime(&now);
    for(int attempt = 0; attempt < 1000; ++attempt)
    {
        CString suffix;
        if(attempt > 0)
            suffix.Format("-%03d", attempt);
        backupPath.Format("%s\\%s.%04d%02d%02d-%02d%02d%02d-%03d%s.bak",
                          (const char *)backupDir, (const char *)baseName,
                          now.wYear, now.wMonth, now.wDay,
                          now.wHour, now.wMinute, now.wSecond, now.wMilliseconds,
                          (const char *)suffix);
        if(CopyFileA(filePath, backupPath, TRUE))
            return TRUE;
        DWORD code = GetLastError();
        if(code == ERROR_FILE_EXISTS || code == ERROR_ALREADY_EXISTS)
            continue; // same millisecond: choose another unique name
        error.Format("Could not back up the database to:\n%s\n\nWindows error %lu.",
                     (const char *)backupPath, code);
        return FALSE;
    }

    error = "Could not choose a unique dated backup name. The database was not saved.";
    return FALSE;
}

//-----------Save Entity Database-----------------------------------------------------//
void EngineXWin::OnBTNENTSaveDB()
{
    if(!m_gStatus || !m_gStatus->m_entObjList)
    {
        MessageBox("No entity database is loaded.", "Save Database", MB_OK | MB_ICONWARNING);
        return;
    }
    if(m_gStatus->m_entObjList->m_lodScriptHasBeenExecuted == 1)
    {
        AfxMessageBox("Cannot save EDB because LOD processing has been done, if saved it will lose lods information",0,0);
        return;
    }

    CString mainDir;
    char buffer[_MAX_PATH];
    _getcwd(buffer, _MAX_PATH);
    mainDir = buffer;
    CString gameFilesDir = m_gStatus->GL_pathGameFiles;
    if(gameFilesDir.IsEmpty())
        gameFilesDir = mainDir + "\\GameFiles\\";
    if(!SetCurrentDirectory(gameFilesDir))
    {
        MessageBox("Could not open the GameFiles folder:\n\n" + gameFilesDir,
                   "Save Database", MB_OK | MB_ICONWARNING);
        return;
    }

    static char BASED_CODE filter[] = "Entity database (*.EDB)|*.EDB|All Files (*.*)|*.*||";
    CFileDialog saveDialog(FALSE, "EDB", "EARevolution.EDB",
                           OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
                           filter, this);
    if(saveDialog.DoModal() == IDOK)
    {
        CString filePath = saveDialog.GetPathName();
        CString backupPath, backupError;
        if(!EngineXCreateDatabaseBackup(filePath, backupPath, backupError))
        {
            SetCurrentDirectory(mainDir);
            MessageBox("The database was not saved because its backup could not be made.\n\n" + backupError,
                       "Save Database", MB_OK | MB_ICONWARNING);
            return;
        }

        CFileException exception;
        CFile file;
        if(!file.Open(filePath, CFile::modeCreate | CFile::modeWrite, &exception))
        {
            char errorText[512] = { 0 };
            exception.GetErrorMessage(errorText, sizeof(errorText));
            SetCurrentDirectory(mainDir);
            CString message;
            message.Format("Could not save the database:\n%s\n\n%s",
                           (const char *)filePath, errorText);
            MessageBox(message, "Save Database", MB_OK | MB_ICONWARNING);
            return;
        }

        BOOL saved = FALSE;
        CString writeError;
        try
        {
            CArchive archive(&file, CArchive::store);
            archive << m_gStatus->m_entObjList;
            archive.Close();
            file.Close();
            saved = TRUE;
        }
        catch(CException * exceptionPtr)
        {
            char errorText[512] = { 0 };
            exceptionPtr->GetErrorMessage(errorText, sizeof(errorText));
            writeError = errorText;
            exceptionPtr->Delete();
            file.Abort();
        }
        catch(...)
        {
            writeError = "An unexpected error occurred while writing the EDB.";
            file.Abort();
        }

        if(saved)
            m_quickEntityDatabasePath = filePath;
        SetCurrentDirectory(mainDir);

        if(!saved)
        {
            CString message;
            message.Format("Could not finish saving the database:\n%s\n\n%s\n\n"
                           "The dated backup, if one was made, is still available.",
                           (const char *)filePath, (const char *)writeError);
            MessageBox(message, "Save Database", MB_OK | MB_ICONWARNING);
        }
        return;
    }

    SetCurrentDirectory(mainDir);
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------actuates retractable objects to origional position if qualify-----------------//
void EngineXWin::OnBTNSCRAddScript() 
{
#ifndef _ClientOutput
    if(!GL_scriptList)
		GL_scriptList = new CEXScriptObjList;
	    CAddScriptEventDlg dlg;
		if(dlg.DoModal()==IDOK){
			  AddScriptEvent(GetScriptEventNum(dlg.m_event),dlg.m_miscText,dlg.m_miscInt,GL_scriptList);
			//update Editor

               m_scriptDlgBar.SendDlgItemMessage(IDC_LIST_SCR_AutoExecEventList,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)dlg.m_event);
		}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------actuates retractable objects to origional position if qualify-----------------//
void EngineXWin::OnBTNSCRDeleteEvent() 
{
	if(!GL_scriptList)
	{
		MessageBox("No event to delete.");
		return;
	}

	int runtimeIndex = m_scriptDlgBar.SendDlgItemMessage(IDC_LIST_SCR_AutoExecEventList,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select a script event.");
		return;
	}
	POSITION posLoc = GL_scriptList->FindIndex(runtimeIndex);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CEXScriptObj * ptrLoc = ( CEXScriptObj* ) GL_scriptList->GetAt(posLoc);
	delete ptrLoc;
	ptrLoc=0;
	GL_scriptList->RemoveAt(posLoc);
	m_scriptDlgBar.SendDlgItemMessage(IDC_LIST_SCR_AutoExecEventList,LB_DELETESTRING,runtimeIndex,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------actuates retractable objects to origional position if qualify-----------------//
void EngineXWin::OnBTNSCRLoadExecFile() 
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
	 // pick the script to edit (any .aut, defaults to GameFiles\autoexec.aut)
	 static char BASED_CODE filter[] = "Autoexec Files|*.aut|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, "aut", "autoexec.aut", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
	 if ( opendialog.DoModal() != IDOK )
	 {
		SetCurrentDirectory(MainDir);
		return;
	 }
	 CString pickedPath = opendialog.GetPathName();
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(pickedPath,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(MainDir); 
		MessageBox("Could not open:\n" + pickedPath);
		return;
	 }
	 m_scriptFilePath = pickedPath;
	 m_gStatus->DeleteScriptList(&GL_scriptList);
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> GL_scriptList;
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(MainDir); 
	 ScriptInList(GL_scriptList);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------------SRC exec Build-----------------------------------------------//
void EngineXWin::OnBTNSCRBuild() 
{
	if(!GL_scriptList)
	{
		MessageBox("Nothing to build - load a script (Load From Exec) or add events first.");
		return;
	}
	// Build to the file that was loaded; if none was, ask where (defaults to GameFiles\autoexec.aut)
	CString target = m_scriptFilePath;
	if(target.IsEmpty())
	{
		CString MainDir; char buffer[_MAX_PATH]; _getcwd(buffer,_MAX_PATH); MainDir = buffer;
		SetCurrentDirectory(MainDir + "\\GameFiles\\");
		static char BASED_CODE filter[] = "Autoexec Files|*.aut|All Files (*.*)|*.*||";
		CFileDialog savedialog( FALSE, "aut", "autoexec.aut", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, this );
		int ok = savedialog.DoModal();
		SetCurrentDirectory(MainDir);
		if(ok != IDOK) return;
		target = savedialog.GetPathName();
	}
	CFileException exc; CFile fl;
	if(!fl.Open(target, CFile::modeCreate | CFile::modeWrite, &exc))
	{
		MessageBox("Could not write:\n" + target);
		return;
	}
	CArchive the_out_Archive(&fl,CArchive::store);
	the_out_Archive << GL_scriptList;
	the_out_Archive.Close();
	fl.Close();
	m_scriptFilePath = target;
	MessageBox("Script built:\n" + target);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------------Edit event in script-----------------------------------------//
void EngineXWin::OnBTNSCREditEvent() 
{
#ifndef _ClientOutput

    if(!GL_scriptList){
       MessageBox("No event to edit.");
	   return;
	}
   
	int runtimeIndex = m_scriptDlgBar.SendDlgItemMessage(IDC_LIST_SCR_AutoExecEventList,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1){
       MessageBox("Must select a script event.");
	   return;
	}
	   POSITION posLoc = GL_scriptList->FindIndex(runtimeIndex);
	   if(!posLoc)
		   return;
	       CEXScriptObj * ptrLoc = ( CEXScriptObj* ) GL_scriptList->GetAt(posLoc);
		   CAddScriptEventDlg dlg;
		   dlg.m_event = GetScriptEventDesc(ptrLoc->List_actionAttribute);
		   dlg.m_miscText = ptrLoc->List_miscString;
		   dlg.m_miscInt = ptrLoc->List_miscInt;
		   if(dlg.DoModal()==IDOK){
		      ptrLoc->List_actionAttribute = GetScriptEventNum(dlg.m_event);
			  ptrLoc->List_miscString = dlg.m_miscText;
			  ptrLoc->List_miscInt = dlg.m_miscInt;
			  m_scriptDlgBar.SendDlgItemMessage(IDC_LIST_SCR_AutoExecEventList,LB_INSERTSTRING ,runtimeIndex,(LPARAM)(LPCTSTR)dlg.m_event);
	          m_scriptDlgBar.SendDlgItemMessage(IDC_LIST_SCR_AutoExecEventList,LB_DELETESTRING,(runtimeIndex+1),0);
		   }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------actuates retractable objects to origional position if qualify-----------------//
int EngineXWin::GetScriptEventNum(CString txtDesc) 
{
	if("play movie m_gStatus->m_fullScreen" == txtDesc)
		return 0;
	if("open scene" == txtDesc)
		return 1;
	if("load missile database" == txtDesc)
		return 2;
	if("load explosion database" == txtDesc)
		return 3;
	if("load movie database" == txtDesc)
		return 4;
	if("load controls database" == txtDesc)
		return 5;
	if("load default cameras" == txtDesc)
		return 6;
	if("load anim textures database" == txtDesc)
		return 7;
	if("initialize mouse object" == txtDesc)
		return 8;
	if("load 2D collections database" == txtDesc)
		return 9;
	if("load text/font collections database" == txtDesc)
		return 10;
	if("load Sound Database" == txtDesc)
		return 11;
	if("start in Fullscreen 640 / 480" == txtDesc)
		return 12;
	if("load entity database" == txtDesc)
		return 14;
	if("load mouse Database" == txtDesc)
		return 15;
	if("show mouse" == txtDesc)
		return 16;
	if("init runtime movie" == txtDesc)
		return 17;
	if("init 2D display collection" == txtDesc)
		return 18;
	if("set all viewports cam to focus" == txtDesc)
		return 19;
	if("AutoInit TCP/IP Game" == txtDesc)
		return 20;
	if("Load Hud Database" == txtDesc)
		return 21;
	if("Load Menu Database" == txtDesc)
		return 22;
	if("Open Menu" == txtDesc)
		return 23;
	if("Load Particle Systems" == txtDesc)
		return 24;
	if("Load Texture Bank" == txtDesc)
		return 25;
	if("Load SoundTrack" == txtDesc)
		return 26;
	if("Load AI cfgs" == txtDesc)
		return 27;
	if("Load Global Inventory" == txtDesc)
		return 28;
	if("Load Commerce Database" == txtDesc)
		return 29;
	if("Load Bank Zones" == txtDesc)
		return 30;
	if("Load Safe Zones" == txtDesc)
		return 31;
	if("Load Item Generator" == txtDesc)
		return 32;
	if("Load Static Texture Database" == txtDesc)
		return 33;
	if("Load Water Zone Database" == txtDesc)
		return 34;
	if("Load Portal Database" == txtDesc)
		return 35;
	if("Load Element Databse" == txtDesc)
		return 36;
	if("Load Stats Database" == txtDesc)
		return 37;
	if("Load Skills Database" == txtDesc)
		return 38;
	if("Load AI Collision Database" == txtDesc)
		return 39;
	if("Load Icon Database" == txtDesc)
		return 40;
	if("Load House Zones" == txtDesc)
		return 41;
	if("Load House DB" == txtDesc)
		return 42;
	if("Load AI raid DB" == txtDesc)
		return 43;
	if("Load Comm Controls" == txtDesc)
		return 44;
	if("Load Arena Sys" == txtDesc)
		return 45;
	if("Load World Rules" == txtDesc)
		return 46;
	if("Set Username" == txtDesc)
		return 47;
	if("Set Password" == txtDesc)
		return 48;
	if("Set Aiserver Flags" == txtDesc)
		return 49;
	if("Set Connect Join" == txtDesc)
		return 50;
	if("Trigger Spawn Gen" == txtDesc)
		return 51;
	if("Load Shader Library" == txtDesc)
		return 52;
	if("Load Global Shader Lib" == txtDesc)
		return 53;
	if("Load Global Attachment DB" == txtDesc)
		return 54;
	if("Load Pixel Shader Lib" == txtDesc)
		return 55;
	if("Load Post Effects DB" == txtDesc)
		return 56;
	if("Load Temp Entities" == txtDesc)
		return 57;
	if("Load Temp Items" == txtDesc)
		return 58;

	return 99;
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------returns the object text desc--------------------------------------------------//
CString EngineXWin::GetScriptEventDesc(int num) 
{
	if(0 == num)
		return "play movie m_gStatus->m_fullScreen";
	if(1 == num)
		return "open scene";
	if(2 == num)
		return "load missile database";
	if(3 == num)
		return "load explosion database";
	if(4 == num)
		return "load movie database";
	if(5 == num)
		return "load controls database";
	if(6 == num)
		return "load default cameras";
	if(7 == num)
		return "load anim textures database";
	if(8 == num)
		return "initialize mouse object";
	if(9 == num)
		return "load 2D collections database";
	if(10 == num)
		return "load text/font collections database";
	if(11 == num)
		return "load Sound Database";
	if(12 == num)
		return "start in m_gStatus->m_fullScreen 640 / 480";
	if(14 == num)
		return "load entity database";
	if(15 == num)
		return "load mouse Database";
	if(16 == num)
		return "show mouse";
	if(17 == num)
		return "init runtime movie";
	if(18 == num)
		return "init 2D display collection";
	if(19 == num)
		return "set all viewports cam to focus";
	if(20 == num)
		return "AutoInit TCP/IP Game";
	if(21 == num)
		return "Load Hud Database";
	if(22 == num)
		return "Load Menu Database";
	if(23 == num)
		return "Open Menu";
	if(24 == num)
		return "Load Particle Systems";
	if(25 == num)
		return "Load Texture Bank";
	if(26 == num)
		return "Load SoundTrack";
	if(27 == num)
		return "Load AI cfgs";
	if(28 == num)
		return "Load Global Inventory";
	if(29 == num)
		return "Load Commerce Database";
	if(30 == num)
		return "Load Bank Zones";
	if(31 == num)
		return "Load Safe Zones";
	if(32 == num)
		return "Load Item Generator";
	if(33 == num)
		return "Load Static Texture Database";
	if(34 == num)
		return "Load Water Zone Database";
	if(35 == num)
		return "Load Portal Database";
	if(36 == num)
		return "Load Element Databse";
	if(37 == num)
		return "Load Stats Database";
	if(38 == num)
		return "Load Skills Database";
	if(39 == num)
		return "Load AI Collision Database";
	if(40 == num)
		return "Load Icon Database";
	if(41 == num)
		return "Load House Zones";
	if(42 == num)
		return "Load House DB";
	if(43 == num)
		return "Load AI raid DB";
	if(44 == num)
		return "Load Comm Controls";
	if(45 == num)
		return "Load Arena Sys";
	if(46 == num)
		return "Load World Rules";
	if(47 == num)
		return "Set Username";
	if(48 == num)
		return "Set Password";
	if(49 == num)
		return "Set Aiserver Flags";
	if(50 == num)
		return "Set Connect Join";
	if(51 == num)
		return "Trigger Spawn Gen";
	if(52 == num)
		return "Load Shader Library";
	if(53 == num)
		return "Load Global Shader Lib";
	if(54 == num)
		return "Load Global Attachment DB";
	if(55 == num)
		return "Load Pixel Shader Lib";
	if(56 == num)
		return "Load Post Effects DB";
	if(57 == num)
		return "Load Temp Entities";
	if(58 == num)
		return "Load Temp Items";

	return "FAILED";
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------fill data of script interface-------------------------------------------------//
void EngineXWin::ScriptInList(CEXScriptObjList * ptrLoc)
{
m_scriptDlgBar.SendDlgItemMessage(IDC_LIST_SCR_AutoExecEventList,LB_RESETCONTENT,0,0);
if(!ptrLoc)
return;

 for(POSITION posLoc = ptrLoc->GetHeadPosition();posLoc != NULL;)
 {
     CEXScriptObj * scrPtr = (CEXScriptObj*) ptrLoc->GetNext(posLoc);
     m_scriptDlgBar.SendDlgItemMessage(IDC_LIST_SCR_AutoExecEventList,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)GetScriptEventDesc(scrPtr->List_actionAttribute) );
 }
}

//-----------Load Mouse Database------------------------------------------------------//
void EngineXWin::OnBTNMSLoadDB() 
{
  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Load Entity obj and append it--------------------------------------------//
void EngineXWin::OnBTNENTLoadEntity() 
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

   // SIE entities load as before; modern model files (FBX / glTF / glb / OBJ /
   // Collada / 3DS) go through the 1 click auto rigger instead: pick the target
   // entity in the entity tree, answer the two questions and the mesh is
   // scaled, skinned and attached to that skeleton in one click.
   static char BASED_CODE filter[] = "All Supported (*.SIE;*.fbx;*.gltf;*.glb;*.obj;*.dae;*.3ds)|*.SIE;*.fbx;*.gltf;*.glb;*.obj;*.dae;*.3ds|Entity Files (*.SIE)|*.SIE|3D Model Files (*.fbx;*.gltf;*.glb;*.obj;*.dae;*.3ds)|*.fbx;*.gltf;*.glb;*.obj;*.dae;*.3ds|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CString pathName = opendialog.GetPathName();

	   // ------------------------------------------------------------------
	   // 1 click auto rig: an imported 3D model instead of a serialized .SIE
	   // ------------------------------------------------------------------
	   if(CAssimpModelImporter::IsSupportedFile(pathName))
	   {
	      // target entity = the one selected in the entity tree (same lookup the
	      // Save Entity button uses)
	      int indexLoc = 0;
	      BOOL foundIt = FALSE;
	      CMovementObj * mPtr = NULL;
	      POSITION posLoc = NULL;
          for(posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//entity loop
	          mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		      HWND treeHwnd = NULL;
              m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
              HTREEITEM item = TreeView_GetSelection(treeHwnd);
		      CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
              if((operator+("TRC_", m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp){
                 foundIt = TRUE;
		         break;
		      }
		      indexLoc++;
	      }//end entity loop
	      if(foundIt == FALSE || mPtr == NULL)
	      {
	         SetCurrentDirectory(MainDir);
		     MessageBox("Select the target entity in the entity tree first, then load the model.");
		     return;
	      }
	      if(mPtr->List_skeletalObject == NULL || mPtr->List_skeletalObject->m_bonesList == NULL)
	      {
	         SetCurrentDirectory(MainDir);
		     MessageBox("The selected entity has no skeleton to rig the model to.");
		     return;
	      }

	      // Two simple Yes/No questions - that is the whole user input.
	      BOOL isFemale = (MessageBox("Is this a FEMALE character?\n\nYes = female body: adds the R_boob / L_boob / R_butt / L_butt anatomy bones and sculpts the bust and glutes.\nNo  = male / standard body.",
			                      "Auto-Rig: Body Type", MB_YESNO | MB_ICONQUESTION) == IDYES);
	      BOOL isArmor = (MessageBox("Is this an ARMOR / OUTFIT variation?\n\nYes = armor / outfit piece (private variation).\nNo  = nude / base body (public variation).",
			                     "Auto-Rig: Slot Type", MB_YESNO | MB_ICONQUESTION) == IDYES);

	      // import the model with Assimp (FBX, glTF, glb, OBJ, Collada, 3DS...)
	      CEXMeshObj * importedMesh = new CEXMeshObj();
	      float sightRange = 0.0f;
	      if(!importedMesh->LoadGeometryFromPlugin(pathName,&sightRange,m_gStatus->m_dxRenderer->m_pd3dDevice,NULL))
	      {
	         importedMesh->SafeDelete();
		     delete importedMesh;
		     SetCurrentDirectory(MainDir);
		     char failMsg[1024];
		     sprintf(failMsg,"Could not import '%s'.\n\n%s",(LPCSTR)pathName,(LPCSTR)CAssimpModelImporter::GetLastError());
		     MessageBox(failMsg);
		     return;
	      }

	      // one click: scale + align, weight, sculpt (female), attach to the skeleton
	      BOOL rigged = mPtr->List_skeletalObject->AutoRigMeshToSkeleton(importedMesh,isFemale,isArmor,m_gStatus->m_dxRenderer->m_pd3dDevice);

	      importedMesh->SafeDelete();
	      delete importedMesh;

	      SetCurrentDirectory(MainDir);
	      if(rigged)
	      {
	         CString doneMsg;
		     doneMsg.Format("Auto-rig complete.\n\nThe model was attached to entity TRC_%s as %s variation.\nUse Save Entity to store it in the SIE file.",
			                (LPCSTR)m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId),
					        (LPCSTR)(isArmor ? CString("an armor / outfit") : CString("a nude / base body")));
		     MessageBox(doneMsg);
	      }
	      else
	      {
	         MessageBox("Auto-rig failed.\n\n" + CAssimpModelImporter::GetLastError());
	      }
	      return;
	   }

	   // ------------------------------------------------------------------
	   // serialized .SIE entity (original behavior)
	   // ------------------------------------------------------------------
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   CMovementObj * newPtrLoc = NULL;
	   the_in_Archive >> newPtrLoc;
	   the_in_Archive.Close();
       fl.Close();  

	   newPtrLoc->List_traceNumber = m_gStatus->GlobalTraceNumber++;
	   //Prep then add baby!!
	   SetCurrentDirectory(MainDir);	
	   m_gStatus->m_entObjList->AddTail(newPtrLoc);
	      EnitityInList();	   
	 }
     SetCurrentDirectory(MainDir);	
}

//------------------------------------------------------------------------------------//
//-----------Save Entity obj----------------------------------------------------------//
void EngineXWin::OnBTNENTSaveEntity() 
{
    if(m_gStatus->m_entObjList->m_lodScriptHasBeenExecuted == 1)
	{
	   AfxMessageBox("Cannot save SIE because LOD processing has been done, if saved it will lose lods information",0,0);
	   return;
	}

int indexLoc=0;
BOOL foundIt = FALSE; 
CMovementObj * mPtr;
   //get selected index
    POSITION posLoc;
    for(posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp){
           foundIt = TRUE;
		   break;
		}
		indexLoc++;
	}
	if(foundIt == FALSE){
       MessageBox("Must select a entity base object");
	   return;
	}
	//get database object
    posLoc = m_gStatus->m_entObjList->FindIndex(indexLoc);
    if(!posLoc)
       return;
       CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetAt(posLoc);	
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "Entity Files (*.SIE)|*.SIE|Wavefront OBJ (*.obj)|*.obj|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "SIE", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString ext = opendialog.GetFileExt();
	   ext.MakeLower();
	   if(ext == "obj")
	   {
	       if(!ptrLoc->List_skeletalObject || !ptrLoc->List_skeletalObject->ExportToObj(opendialog.GetPathName()))
	           MessageBox("Failed to export skeletal entity to Wavefront OBJ.");
	       SetCurrentDirectory(MainDir);
	       return;
	   }
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
       the_out_Archive << ptrLoc;
       the_out_Archive.Close();
       fl.Close();
       
       
	 }
	 SetCurrentDirectory(MainDir);  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Save Mouse DB------------------------------------------------------------//
void EngineXWin::OnBTNMSSaveDB() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------edits a object light-----------------------------------------------------//
void EngineXWin::OnBTNOBJEditLight() 
{
#ifndef _ClientOutput
HWND treeHwnd = NULL;
  m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
  HTREEITEM item = TreeView_GetSelection(treeHwnd); 
  CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);

  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc && ptrLoc->m_lightObject != NULL){
	   CLightInterfaceDlg dlg;
	   dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	   dlg.m_lightREF = ptrLoc->m_lightObject;
	   if(dlg.DoModal()==IDOK)
		  ptrLoc->m_lightObject = dlg.m_lightREF;

	   return;
	}
	
  }//end Begin world object loop
  MessageBox("Object Not Found or light isnt there");
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------transfers  to another entity by indexes----------------------------------//
BOOL EngineXWin::GetIntoEntityByIndexes(int indexSrc,int indexDest)
{
POSITION posLocal = m_gStatus->m_entObjRuntime->FindIndex(indexSrc);
if(!posLocal)
return FALSE;
CMovementObj * ptrSrc = ( CMovementObj* )m_gStatus->m_entObjRuntime->GetAt(posLocal);
posLocal = m_gStatus->m_entObjRuntime->FindIndex(indexDest);
if(!posLocal)
return FALSE;
CMovementObj * ptrDest = ( CMovementObj* )m_gStatus->m_entObjRuntime->GetAt(posLocal);
GetIntoEntity(ptrSrc,ptrDest);
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------transfers  to another entity---------------------------------------------//
BOOL EngineXWin::GetIntoEntity(CMovementObj * currentEntity,CMovementObj * targetEntity)
{
if(currentEntity->List_controlType != 0 || targetEntity->List_controlType != 10)
return FALSE;

int origionalEntityRefModel = currentEntity->List_refModel;
int controlAssignMent = currentEntity->List_controlType;
int targetEntityRefModel = targetEntity->List_refModel;
DPNID networkID = currentEntity->List_idFromLoc;
D3DMATRIX targetMatrix;
targetEntity->List_baseFrame->GetTransform(NULL,&targetMatrix);

int indexTraceLocal = 0;
POSITION posLast;
for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
{
    CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);
	if(ptrLoc->List_traceNumber == currentEntity->List_traceNumber || ptrLoc->List_traceNumber == targetEntity->List_traceNumber){
	   m_gStatus->m_entObjRuntime->RemoveAt(posLast);
	   m_gStatus->DestroyRuntimeEntity(ptrLoc,m_gStatus->m_spawnGeneratorDB,m_gStatus->m_entObjRuntime);
	   ptrLoc = 0;
	   
	}
}
//spawn in to replace vehicle ;)
m_gStatus->ActivateMovementEntity(NULL,
					   0,
					   targetMatrix,
					   0,//int cameraIndex,
					   0,//int vportIndex,
					   targetEntityRefModel,//int indexLoc,
					   controlAssignMent,//int controlAssignment,
					   0,//int entityType,
					   networkID,//DPNID idFromLoc,
					   FALSE,//BOOL networkEntity,
					   TRUE,
					   FALSE,//BOOL addHead)
					   -1,
					   -1,
					   NULL,
					   NULL,
					   -1,
					   -1,
					   m_gStatus->m_collisionBase,
					   m_gStatus->m_collisionDatabase,
					   m_gStatus->m_entObjRuntime,
					   m_gStatus->m_currentWorkingFile);
CMovementObj * focusLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetHead();
focusLoc->List_possesedBy = origionalEntityRefModel;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------get spawn point matrix---------------------------------------------------//
BOOL EngineXWin::GetSpawnPointMatrix(int selectMethod,D3DMATRIX * matrixLocal)
{
//selection method
//0=first available
m_gStatus->m_matLib.InitMat(matrixLocal->m);
for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
    if(ptrLoc->List_usedAsSpawnPoint == selectMethod){//found first available SPAWN IT!!
       matrixLocal->_41 = ptrLoc->m_meshObject->m_meshCenter.x;
	   matrixLocal->_42 = ptrLoc->m_meshObject->m_meshCenter.y;
	   matrixLocal->_43 = ptrLoc->m_meshObject->m_meshCenter.z;
	   return TRUE;
	}//end found first available SPAWN IT!!
}
return FALSE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------respawns entity if dead--------------------------------------------------//
BOOL EngineXWin::Respawn(CMovementObj * currentEntity)
{
/*if(currentEntity->List_energy > 0)
return FALSE;

currentEntity->List_movementDB->List_curSpeed = 0.0f;
currentEntity->List_movementDB->List_curRotX=0.0f;
currentEntity->List_movementDB->List_curRotY=0.0f;
int origionalEntityRefModel = currentEntity->List_refModel;
int controlAssignMent = currentEntity->List_controlType;
float targetMatrix[4][4];
currentEntity->List_baseFrame->GetTransform(NULL,&targetMatrix);
GetSpawnPointMatrix(0,targetMatrix);
DPID networkID = currentEntity->List_idFromLoc;
currentEntity->List_idFromLoc=NULL;
//spawn in to replace vehicle ;)
m_gStatus->ActivateMovementEntity(targetMatrix,
					   0,//int cameraIndex,
					   0,//int vportIndex,
					   currentEntity->List_refModel,//int indexLoc,
					   controlAssignMent,//int controlAssignment,
					   0,//int entityType,
					   networkID,//DPNID idFromLoc,
					   currentEntity->List_networkEntity,//BOOL networkEntity,
					   TRUE,
					   FALSE);//BOOL addHead)
//basically have the other entity become dead / empty again
//currentEntity->List_controlType = 999;
//currentEntity->List_networkEntity = FALSE;*/
return TRUE;
}
//-----------removes dead entities----------------------------------------------------//
void EngineXWin::SetFadeOnSkeletal(CSkeletonObject * skeleton,long duration)
{
      
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------destroy inventory list---------------------------------------------------//
void EngineXWin::DestroyInventorylist(CInventoryObjList * listFocus)
{
  if(!listFocus)
      return;

  for(POSITION posLoc = listFocus->GetHeadPosition();posLoc != NULL;)
  {
      CInventoryObj * ptr = (CInventoryObj *)listFocus->GetNext(posLoc);
	  delete ptr;
	  ptr=0;
  }
  listFocus->RemoveAll();
  delete listFocus;
  listFocus=0;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------tally up lights in use---------------------------------------------------//
void EngineXWin::TallyLightsInUse()
{
	LIGHTSINUSE = m_gStatus->m_runtimeLightSys->GetCount();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------get matrix conversion----------------------------------------------------//
void EngineXWin::InvFloatMatrix(float Mat[4][4])
{
for(int i=0;i<3;i++)
for(int j=0;j<3;j++){
Mat[i][j] = Mat[i][j] * -1;
}
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------get matrix conversion----------------------------------------------------//
void EngineXWin::GetFloatMatrix(D3DMATRIX mat,float Mat[4][4])
{

   Mat[0][0] = mat._11;  Mat[0][1] = mat._12;  Mat[0][2] = mat._13;  Mat[0][3] = mat._14;
   Mat[1][0] = mat._21;  Mat[1][1] = mat._22;  Mat[1][2] = mat._23;  Mat[1][3] = mat._24;
   Mat[2][0] = mat._31;  Mat[2][1] = mat._32;  Mat[2][2] = mat._33;  Mat[2][3] = mat._34;
   Mat[3][0] = mat._41;  Mat[3][1] = mat._42;  Mat[3][2] = mat._43;  Mat[3][3] = mat._44;

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------get matrix conversion----------------------------------------------------//
void EngineXWin::GetD3DMatrix(D3DMATRIX * mat,float Mat[4][4])
{
   //transfer data
   mat->_11 = Mat [0][0];  mat->_12 = Mat [0][1];  mat->_13 = Mat [0][2];  mat->_14 = Mat [0][3];
   mat->_21 = Mat [1][0];  mat->_22 = Mat [1][1];  mat->_23 = Mat [1][2];  mat->_24 = Mat [1][3];
   mat->_31 = Mat [2][0];  mat->_32 = Mat [2][1];  mat->_33 = Mat [2][2];  mat->_34 = Mat [2][3];
   mat->_41 = Mat [3][0];  mat->_42 = Mat [3][1];  mat->_43 = Mat [3][2];  mat->_44 = Mat [3][3];
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------load texture data--------------------------------------------------------//
void EngineXWin::SearchAndAddTextureDataWorldObjs()
{
  CString CurDir;
       CString MainDir;
         char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\MapsModels\\";
			 SetCurrentDirectory(CurDir);
			 
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
        if(ptrLoc->m_meshObject)
		m_gStatus->AddTextureToDatabase(ptrLoc->m_meshObject);
  }//end Begin world object loop
  SetCurrentDirectory(MainDir);
  m_gStatus->ReinitAllTexturesFromDatabase();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------delete texture-----------------------------------------------------------//
/*
void EngineXWin::OnBtnTxtdbDelete() 
{
int runtimeIndex = m_textureDBDlgBar.SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              MessageBox("Must select an texture object.");
		      return;
		   }
	       POSITION posLoc = m_gStatus->m_textureDataBase->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_gStatus->m_textureDataBase->GetAt(posLoc);	
               m_gStatus->m_textureDataBase->RemoveAt(posLoc);
			   InListTextureDB(runtimeIndex-1);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------Edit Texture-------------------------------------------------------------//
void EngineXWin::OnBtnTxtdbEdit() 
{
#ifndef _ClientOutput
int runtimeIndex = m_textureDBDlgBar.SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              MessageBox("Must select an texture object.");
		      return;
		   }
	       POSITION posLoc = m_gStatus->m_textureDataBase->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_gStatus->m_textureDataBase->GetAt(posLoc);		   

    //working
    CAddTextureDlg dlg;
	dlg.m_mipLevels = 1;
	if(dlg.DoModal()==IDOK){
		if(dlg.fileCounter > 2){
           MessageBox("Error: Must select only 1 texture when replacing.");
		   return;
		}
       
       

	   for(int loop=0;loop<dlg.fileCounter;loop++){//load all files
          if(dlg.fileCounter != 1 && loop == 0)
			 loop++;

		  CString loadContruction;
		  
             loadContruction = dlg.FileNames[0]+"\\"+dlg.FileNames[loop];

          char charbuf[256];
	      lstrcpy(charbuf,dlg.FileNames[loop]);
		  ptrLoc->m_textureObject->SafeDelete();
		  delete ptrLoc->m_textureObject;
		  ptrLoc->m_textureObject=0;
		  lstrcpy(ptrLoc->List_textureName,charbuf);
          ptrLoc->m_textureObject = new CTextureEX();
	      if(ptrLoc->m_textureObject->LoadIt(loadContruction,
			                                 dlg.m_mipLevels,
			                                 m_gStatus->m_dxRenderer->m_pd3dDevice,
											 FALSE)==TRUE){//success
             InListTextureDB(runtimeIndex);
			 m_gStatus->ReinitAllTexturesFromDatabase();
		  }//end success
	      else{
          MessageBox("failed");
		  }
	   }
	}				
				
#endif
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------in list texture database-------------------------------------------------//
/*
void EngineXWin::InListTextureDB(int sel) 
{
if(!m_gStatus->m_textureDataBase)
return;

if(m_gStatus->m_editorModeOn == FALSE)
return;

m_textureDBDlgBar.SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_RESETCONTENT,0,0);
int indexTrace = 0;
  for(POSITION texPositionLoc = m_gStatus->m_textureDataBase->GetHeadPosition();texPositionLoc != NULL;)
  {
    CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetNext(texPositionLoc);
    CString tempLocal = m_gStatus->m_cabLib.ConvToString(indexTrace)+"__";  
	CString format = "Unknown";
	CString sz = "";

	if(txtPtrLoc->m_textureObject)
	{
       format = txtPtrLoc->m_textureObject->GetFormatName();
       if(txtPtrLoc->m_textureObject->m_usage == 1)
	      format += " R-T ";
	   sz = " Size:"+m_gStatus->m_cabLib.ConvToString(txtPtrLoc->m_textureObject->m_widthBasis) + "X" + m_gStatus->m_cabLib.ConvToString(txtPtrLoc->m_textureObject->m_hieghtBasis);
	}
	m_textureDBDlgBar.SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)(tempLocal+txtPtrLoc->List_textureName+" *"+format+sz));
    indexTrace++;
  }

  if(sel != -1)
	   m_textureDBDlgBar.SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_SETCURSEL ,sel,0);
}
*/
//-----------reinit all textures from database----------------------------------------//
/*
void EngineXWin::OnBtnTxtdbReinitAll() 
{
    
    if(m_textureDBDlgBar.SendDlgItemMessage(IDC_CHECK_TEX_BltOn,BM_GETSTATE ,0,0) == BST_CHECKED)
	{
	   m_gStatus->m_dynamicTextureDataBase->m_compressRef = 1;
	   m_gStatus->m_textureDataBase->m_compressRef = 1;
	   m_gStatus->m_compressTextures = TRUE;
	}
	else
	{
       m_gStatus->m_dynamicTextureDataBase->m_compressRef = 0;
	   m_gStatus->m_textureDataBase->m_compressRef = 0;
	   m_gStatus->m_compressTextures = FALSE;
	}

	m_gStatus->ReinitAllTexturesFromDatabase();
	if(m_gStatus->m_editorModeOn == TRUE)
	   InListTextureDB(0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------texture browse-----------------------------------------------------------//
void EngineXWin::OnBTNTXTDBBrowse() 
{
     m_gStatus->ReinitAllTexturesFromDatabaseRemoveUnused();
	 InListTextureDB(0);
	 AfxMessageBox("Completed");
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------texture report-----------------------------------------------------------//
void EngineXWin::OnBTNTXTDBTextureReport() 
{
	// Get Current Location
	CString CurDir;
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	CurDir = buffer;

	// Create Command Buffer and Create TextureReport Directory
	CString command;
	command = "mkdir " + CurDir + "\\TextureReport";
	system(command.GetBuffer());

	// Delete any existing files in TextureReport Directory
	command = "del " + CurDir + "\\TextureReport\\* /Q";
	system(command.GetBuffer());

	// Clear Existing Error Log
	command = "echo Starting Texture Copying... > " + CurDir + "\\textureReportErrors.txt";
	system(command.GetBuffer());

	// Create NewLine Character Reference
	char newline = '\n';

	// Set the fileName and Open File for Writing
	CString fileName = CurDir + "\\textureReport.txt";
	FILE *m_file = fopen(fileName.GetBuffer(), "w");

	// Loop through texture DB
	for(POSITION texPositionLoc = m_gStatus->m_textureDataBase->GetHeadPosition();texPositionLoc != NULL;)
	{
		CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetNext(texPositionLoc);

		// Write Texture Name to File
		fwrite(txtPtrLoc->List_textureName, strlen(txtPtrLoc->List_textureName), 1, m_file);
		fwrite(&newline, 1, 1, m_file);

		// Copy File to Clean Directory
		command = "copy \"" + CurDir + "\\MapsModels\\" + txtPtrLoc->List_textureName + "\" \"" + CurDir + "\\TextureReport\\" + txtPtrLoc->List_textureName + "\"";
		system(command.GetBuffer());

		// Verify File Was Written
		command = "IF NOT EXIST \"" + CurDir + "\\TextureReport\\" + txtPtrLoc->List_textureName + "\" echo " + txtPtrLoc->List_textureName + " Not Copied. >> " + CurDir + "\\textureReportErrors.txt";
		system(command.GetBuffer());
	}	

	// Close the File
	fclose(m_file);

	// Tell User the report is generated.
	AfxMessageBox("Report Generated.");
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------destroy all textures from database---------------------------------------//
void EngineXWin::DestroyAllTexturesFromDatabase()
{
if(!m_gStatus->m_dynamicTextureDataBase)
return;
m_gStatus->m_dynamicTextureDataBase->InvalidateAllTextures();
m_gStatus->m_dynamicTextureDataBase->SafeDelete();
delete m_gStatus->m_dynamicTextureDataBase;
m_gStatus->m_dynamicTextureDataBase=0;
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------clear text coords--------------------------------------------------//
void EngineXWin::OnBTNOBJClearTexCoords() 
{
/*HWND treeHwnd = NULL;
m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd); 
int indexOfObject = GetTreeChildIndex(treeHwnd,item);	
HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,ObjectRoot,10);
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){
	     POSITION contPtrLoc = ptrLoc->List_objContentList->FindIndex(indexOfObject);
	     CContent * ptrContent = (CContent *) ptrLoc->List_objContentList->GetAt(contPtrLoc);
	     ptrContent->List_imMesh->DumpTextureCoordSets();
		 ptrContent->List_imMesh->List_howManyTextures = 0;
		 ptrContent->List_imMesh->InitVB(g_pD3D);
         return;
	}
  }
  MessageBox("Must Select a visual");*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------add texture coordinates--------------------------------------------------//
void EngineXWin::OnBTNOBJAddTexCoords() 
{
#ifndef _ClientOutput
    HWND treeHwnd = NULL;
  m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
  HTREEITEM item = TreeView_GetSelection(treeHwnd); 
  CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);

  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){
	   
	   CVertexAnimSettingsDlg dlg;

	   dlg.m_animModuleRef = ptrLoc->m_animModule;
	   dlg.g_pd3dDevice    = m_gStatus->m_dxRenderer->m_pd3dDevice;

	   dlg.DoModal();

	   ptrLoc->m_animModule = dlg.m_animModuleRef;

	   return;
	}
	
  }//end Begin world object loop
  MessageBox("Object Not Found or light isnt there");	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------sets script to scene-----------------------------------------------------//
void EngineXWin::OnBTNSCRSetToScene() 
{
if(!GL_scriptList)
return;
for(POSITION posLoc = GL_scriptList->GetHeadPosition();posLoc != NULL;)
 {
     CEXScriptObj * scrPtr = (CEXScriptObj*) GL_scriptList->GetNext(posLoc);
     AddScriptEvent(scrPtr->List_actionAttribute,scrPtr->List_miscString,scrPtr->List_miscInt,m_gStatus->m_sceneScriptExec);	
 }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------sets script to scene-----------------------------------------------------//
void EngineXWin::OnBTNSCRClearScene() 
{

   m_scriptDlgBar.SendDlgItemMessage(IDC_LIST_SCR_AutoExecEventList,LB_RESETCONTENT,0,0);
m_gStatus->DeleteScriptList(&m_gStatus->m_sceneScriptExec);
m_gStatus->m_sceneScriptExec =      new CEXScriptObjList;	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------sets script to scene-----------------------------------------------------//
void EngineXWin::OnBTNSCRLoadFromScene() 
{
ScriptInList(m_gStatus->m_sceneScriptExec);
}

//--------------------indexes animations based on time--------------------------------//
BOOL EngineXWin::GetKeyIfPressed(CString * key,BYTE diKeys[256])
{
*key = "5555555";//def
//----
if(diKeys[DIK_NUMPAD1] & 0x80){
*key = "1";return TRUE;}
if(diKeys[DIK_NUMPAD2] & 0x80){
*key = "2";return TRUE;}
if(diKeys[DIK_NUMPAD3] & 0x80){
*key = "3";return TRUE;}
if(diKeys[DIK_NUMPAD4] & 0x80){
*key = "4";return TRUE;}
if(diKeys[DIK_NUMPAD5] & 0x80){
*key = "5";return TRUE;}
if(diKeys[DIK_NUMPAD6] & 0x80){
*key = "6";return TRUE;}
if(diKeys[DIK_NUMPAD7] & 0x80){
*key = "7";return TRUE;}
if(diKeys[DIK_NUMPAD8] & 0x80){
*key = "8";return TRUE;}
if(diKeys[DIK_NUMPAD9] & 0x80){
*key = "9";return TRUE;}
if(diKeys[DIK_NUMPAD0] & 0x80){
*key = "0";return TRUE;}
if(diKeys[DIK_1] & 0x80){
*key = "1";return TRUE;}
if(diKeys[DIK_2] & 0x80){
*key = "2";return TRUE;}
if(diKeys[DIK_3] & 0x80){
*key = "3";return TRUE;}
if(diKeys[DIK_4] & 0x80){
*key = "4";return TRUE;}
if(diKeys[DIK_5] & 0x80){
*key = "5";return TRUE;}
if(diKeys[DIK_6] & 0x80){
*key = "6";return TRUE;}
if(diKeys[DIK_7] & 0x80){
*key = "7";return TRUE;}
if(diKeys[DIK_8] & 0x80){
*key = "8";return TRUE;}
if(diKeys[DIK_9] & 0x80){
*key = "9";return TRUE;}
if(diKeys[DIK_0] & 0x80){
*key = "0";return TRUE;}
if(diKeys[DIK_A] & 0x80){
*key = "A";return TRUE;}
if(diKeys[DIK_B] & 0x80){
*key = "B";return TRUE;}
if(diKeys[DIK_C] & 0x80){
*key = "C";return TRUE;}
if(diKeys[DIK_D] & 0x80){
*key = "D";return TRUE;}
if(diKeys[DIK_E] & 0x80){
*key = "E";return TRUE;}
if(diKeys[DIK_F] & 0x80){
*key = "F";return TRUE;}
if(diKeys[DIK_G] & 0x80){
*key = "G";return TRUE;}
if(diKeys[DIK_H] & 0x80){
*key = "H";return TRUE;}
if(diKeys[DIK_I] & 0x80){
*key = "I";return TRUE;}
if(diKeys[DIK_J] & 0x80){
*key = "J";return TRUE;}
if(diKeys[DIK_K] & 0x80){
*key = "K";return TRUE;}
if(diKeys[DIK_L] & 0x80){
*key = "L";return TRUE;}
if(diKeys[DIK_M] & 0x80){
*key = "M";return TRUE;}
if(diKeys[DIK_N] & 0x80){
*key = "N";return TRUE;}
if(diKeys[DIK_O] & 0x80){
*key = "O";return TRUE;}
if(diKeys[DIK_P] & 0x80){
*key = "P";return TRUE;}
if(diKeys[DIK_Q] & 0x80){
*key = "Q";return TRUE;}
if(diKeys[DIK_R] & 0x80){
*key = "R";return TRUE;}
if(diKeys[DIK_S] & 0x80){
*key = "S";return TRUE;}
if(diKeys[DIK_T] & 0x80){
*key = "T";return TRUE;}
if(diKeys[DIK_U] & 0x80){
*key = "U";return TRUE;}
if(diKeys[DIK_V] & 0x80){
*key = "V";return TRUE;}
if(diKeys[DIK_W] & 0x80){
*key = "W";return TRUE;}
if(diKeys[DIK_X] & 0x80){
*key = "X";return TRUE;}
if(diKeys[DIK_Y] & 0x80){
*key = "Y";return TRUE;}
if(diKeys[DIK_Z] & 0x80){
*key = "Z";return TRUE;}
if(diKeys[DIK_SPACE] & 0x80){
*key = " ";return TRUE;}
if(diKeys[DIK_ADD] & 0x80){
*key = "+";return TRUE;}
if(diKeys[DIK_SUBTRACT] & 0x80){
*key = "-";return TRUE;}
if(diKeys[DIK_EQUALS] & 0x80){
*key = "=";return TRUE;}
if(diKeys[DIK_LBRACKET] & 0x80){
*key = "[";return TRUE;}
if(diKeys[DIK_RBRACKET] & 0x80){
*key = "]";return TRUE;}
if(diKeys[DIK_SEMICOLON] & 0x80){
*key = ":";return TRUE;}
if(diKeys[DIK_APOSTROPHE] & 0x80){
*key = "'";return TRUE;}
if(diKeys[DIK_COMMA] & 0x80){
*key = ",";return TRUE;}
if(diKeys[DIK_PERIOD] & 0x80){
*key = ".";return TRUE;}
if(diKeys[DIK_BACKSLASH] & 0x80){
*key = "\\";return TRUE;}
if(diKeys[DIK_MULTIPLY] & 0x80){
*key = "*";return TRUE;}
if(diKeys[DIK_MINUS] & 0x80){
*key = "-";return TRUE;}
if(diKeys[DIK_SLASH] & 0x80){
*key = "?";return TRUE;}
if(*key == "5555555"){
return FALSE;
}
return TRUE;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//

//------------------------Advanced settings on network--------------------------------//
void EngineXWin::OnBTNNTAdvanced() 
{
#ifndef _ClientOutput
  CAdvancedNetworkDlg dlg;
  dlg.m_maxPacketIntake = m_gStatus->m_multiplayerObject->m_maxBandwithPerPerson;
  dlg.m_maxPacketOutput = m_gStatus->m_multiplayerObject->m_maxPacketSizeNonAdmin;
  dlg.m_houseVisRangeValue = (int)m_gStatus->m_multiplayerObject->m_houseSightRange;
  dlg.m_levelRestriction = m_gStatus->m_multiplayerObject->m_pkLevelRestriction;
  dlg.enableAutobackup = m_gStatus->m_multiplayerObject->m_enableAutoBackup;
  dlg.m_murdererLevel = m_gStatus->m_multiplayerObject->m_murderCountMax;
  dlg.m_durationBetweenBackups = m_gStatus->m_multiplayerObject->m_autoBackupFrequency;
  dlg.m_autoBackupFile = m_gStatus->m_multiplayerObject->m_autoBackupFile;
  dlg.NoRenderDedicated = m_gStatus->m_dedicatedServer;///////
  dlg.m_calcItemRadius = m_gStatus->m_multiplayerObject->m_serverCalculateRadiusItems;
  dlg.m_calcEntityRadius = m_gStatus->m_multiplayerObject->m_serverCalculateRadius; 
  dlg.RecordNetworkData = GL_RecordNetworkActivity;
  dlg.ClientSideFireing = m_gStatus->m_multiplayerObject->m_clientSideFireing;
  dlg.m_availableBandwidth = m_gStatus->m_multiplayerObject->m_bandwidthAvailable;
  dlg.m_maxLoggedOnPlayers = m_gStatus->m_multiplayerObject->m_maxPlayers; 
  dlg.m_frequencyIncrement = m_gStatus->m_multiplayerObject->m_frequencyIncrement;
  dlg.m_frequencyAdjustBuffer = m_gStatus->m_multiplayerObject->m_frequencyBuffer;
  dlg.persistentServerType = m_gStatus->m_multiplayerObject->m_PersistentType;
  dlg.m_frequencyLimit = m_gStatus->m_multiplayerObject->m_frequencyLimit;
  dlg.requiresPassword = m_gStatus->m_multiplayerObject->m_passwordRequired;
  //dlg.m_serverKey = m_gStatus->m_multiplayerObject->m_serverKey;
  //dlg.m_serverName = m_gStatus->m_multiplayerObject->m_servername;
  dlg.aiServer = m_gStatus->m_multiplayerObject->m_AIServer; 
  dlg.m_deadEntityDissolveTime = m_gStatus->m_multiplayerObject->m_deadEntityDissolveTime;
  dlg.m_deadEntityNPCDissolveTime = m_gStatus->m_multiplayerObject->m_deadEntityNPCDissolveTime;
  dlg.m_aiServerCycle = m_gStatus->m_multiplayerObject->m_aiserverCycleDuration;
  dlg.m_versionNumber = m_gStatus->m_multiplayerObject->m_currentVersion;
  dlg.m_sqlCatalogDB = m_gStatus->m_multiplayerObject->m_sqlServerCatalog;
  dlg.m_sqlIP = m_gStatus->m_multiplayerObject->m_sqlServerIP;
  dlg.m_sqlPassword = m_gStatus->m_multiplayerObject->m_sqlServerPassword;
  dlg.m_sqlUsername = m_gStatus->m_multiplayerObject->m_sqlServerUsername;
  dlg.m_sqlTableName = m_gStatus->m_multiplayerObject->m_sqlServerTable;
  dlg.m_murderWorkOffRate = m_gStatus->m_multiplayerObject->m_goodBehaviorDuration;
 
  if(dlg.DoModal()==IDOK){
  m_gStatus->m_multiplayerObject->m_pkLevelRestriction = dlg.m_levelRestriction;
  m_gStatus->m_multiplayerObject->m_goodBehaviorDuration = dlg.m_murderWorkOffRate;
  m_gStatus->m_multiplayerObject->m_murderCountMax = dlg.m_murdererLevel;
  m_gStatus->m_multiplayerObject->m_enableAutoBackup = dlg.enableAutobackup;
  m_gStatus->m_multiplayerObject->m_autoBackupFrequency = dlg.m_durationBetweenBackups;
  m_gStatus->m_multiplayerObject->m_autoBackupFile = dlg.m_autoBackupFile;
  m_gStatus->m_multiplayerObject->m_deadEntityDissolveTime = dlg.m_deadEntityDissolveTime;
  m_gStatus->m_multiplayerObject->m_AIServer = dlg.aiServer; 
  if(m_gStatus->m_multiplayerObject->m_AIServer)
  {//dump sounds
     m_gStatus->m_soundManager->SafeDelete();
  }//end dump sounds
  m_gStatus->m_dedicatedServer = dlg.NoRenderDedicated;
  m_gStatus->m_blackOut = TRUE;

  m_gStatus->m_multiplayerObject->m_sqlServerTable = dlg.m_sqlTableName;
  m_gStatus->m_multiplayerObject->m_sqlServerCatalog = dlg.m_sqlCatalogDB;
  m_gStatus->m_multiplayerObject->m_sqlServerIP = dlg.m_sqlIP;
  m_gStatus->m_multiplayerObject->m_sqlServerPassword = dlg.m_sqlPassword;
  m_gStatus->m_multiplayerObject->m_sqlServerUsername = dlg.m_sqlUsername;
  m_gStatus->m_multiplayerObject->m_aiserverCycleDuration = dlg.m_aiServerCycle;
  m_gStatus->m_multiplayerObject->m_serverCalculateRadiusItems = dlg.m_calcItemRadius;
  m_gStatus->m_multiplayerObject->m_serverCalculateRadius = dlg.m_calcEntityRadius;
  m_gStatus->m_multiplayerObject->m_clientSideFireing = dlg.ClientSideFireing;
  GL_RecordNetworkActivity = dlg.RecordNetworkData;
  m_gStatus->m_multiplayerObject->m_deadEntityNPCDissolveTime = dlg.m_deadEntityNPCDissolveTime;
  m_gStatus->m_multiplayerObject->m_bandwidthAvailable = dlg.m_availableBandwidth;
  m_gStatus->m_multiplayerObject->m_maxPlayers = dlg.m_maxLoggedOnPlayers;
  m_gStatus->m_multiplayerObject->m_frequencyIncrement = dlg.m_frequencyIncrement;
  m_gStatus->m_multiplayerObject->m_frequencyBuffer = dlg.m_frequencyAdjustBuffer;
  m_gStatus->m_multiplayerObject->m_PersistentType = dlg.persistentServerType;
  m_gStatus->m_multiplayerObject->m_frequencyLimit = dlg.m_frequencyLimit;
  m_gStatus->m_multiplayerObject->m_passwordRequired = dlg.requiresPassword;
  //m_gStatus->m_multiplayerObject->m_serverKey = dlg.m_serverKey;
  //m_gStatus->m_multiplayerObject->m_servername = dlg.m_serverName;
  }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------Builds a txt record--------------------------------//
void EngineXWin::OnBTNNTBuildTxtRecord() 
{
	
if(!m_gStatus->m_multiplayerObject->m_eventLog)
return;

m_gStatus->m_multiplayerObject->m_eventLog->OutPutTextFile("TEST.ONL");





}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Create AI Object------------------------------------------------------//
BOOL EngineXWin::SetCursorToSelectedObject(int trace)
{
   //OBJECTS INTERFACE
   int localIndex = 0;
   for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	if(ptrLoc->List_identity == trace){//trace found
	   
		int groupSelIndex = 0;
        for(POSITION gPos = m_gStatus->m_worldGroupList->GetHeadPosition();gPos != NULL;){
            CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetNext(gPos);
            if(wldPtr->List_groupInt == ptrLoc->List_groupNumber){
		  
	            break;
			}
            groupSelIndex++;
		}
		
		
		
	   WorldGroupInList(groupSelIndex);
	   ObjectTreeViewInlist(ptrLoc->List_groupNumber,ptrLoc->List_identity);	
       /*HTREEITEM itemLocal;
       HWND treeHwnd = NULL;
       m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
       itemLocal = TreeView_GetRoot(treeHwnd);
	   TreeView_SelectItem(treeHwnd,itemLocal);
	   if(localIndex > 0)
	   for(int loop=0;loop<localIndex;loop++){
	   itemLocal = TreeView_GetNextSibling(treeHwnd,itemLocal);		
	   TreeView_SelectItem(treeHwnd,itemLocal);
	   }*/
	   return TRUE;
	}//trace found
	localIndex++;
   }//end Begin world object loop
return FALSE;
}//end buuton down and select mode on
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------reconfigure file to X format---------------------------------------------//
void EngineXWin::ConvertToX(CString * fileString)
{
CString ext = m_gStatus->m_cabLib.GetFileExt(*fileString);
       ext.MakeLower();
       if(ext == "3ds"){//begin extension conversion
       CString final = operator+("conv3ds -x -m ",*fileString);
       system( final );
       m_gStatus->m_cabLib.RemoveFileExt(fileString);
       m_gStatus->m_cabLib.AddNewFileExt(fileString,".x");
	   }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Add Display Unit------------------------------------------------------//
void EngineXWin::OnBTNHUDAddDisplayUnit() 
{
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------------Add New Hud------------------------------------------------//
void EngineXWin::OnBTNHUDAddNewHud() 
{
/*#ifndef _ClientOutput
CAddHud dlg;
  if(dlg.DoModal()==IDOK){
   CHudObj *hudPtr = new CHudObj(dlg.m_name);
   m_gStatus->m_hudList->AddTail(hudPtr);
   m_hudDBDlgBar.SendDlgItemMessage(IDC_LIST_HUD_DB,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+(">",dlg.m_name));
  }
#endif*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------------InList Hud-------------------------------------------------//
void EngineXWin::InListHud() 
{
    /*m_hudDBDlgBar.SendDlgItemMessage(IDC_LIST_HUD_DB,LB_RESETCONTENT,0,0);
	for(POSITION posLoc = m_gStatus->m_hudList->GetHeadPosition();posLoc != NULL;){
		CHudObj *hudPtr = (CHudObj *)m_gStatus->m_hudList->GetNext(posLoc);
        m_hudDBDlgBar.SendDlgItemMessage(IDC_LIST_HUD_DB,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+(">",hudPtr->List_HudCollectionName));
	}*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------------------Save Hud-------------------------------------------------//
void EngineXWin::OnBTNHUDSaveDB() 
{
//use gamefiles folder for organization
   /* CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "HUD Files|*.HUD|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "HUD", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_gStatus->m_hudList;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  */	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------------load Hud-------------------------------------------------//
void EngineXWin::OnBTNHUDLoadDB() 
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

   static char BASED_CODE filter[] = "HUD Files|*.HUD|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadHudDatabase(fileName);
	 }
     SetCurrentDirectory(MainDir);	
}
//-------------------------sel change-------------------------------------------------//
void EngineXWin::OnSelchangeListHudDb() 
{
InListDisplayUnits();
}
//---------------------in list display units------------------------------------------//
void EngineXWin::InListDisplayUnits()
{
/*int hudIndex = m_hudDBDlgBar.SendDlgItemMessage(IDC_LIST_HUD_DB,LB_GETCURSEL ,0,0);
  if(hudIndex == -1){
   return;
  }
   POSITION posLoc = m_gStatus->m_hudList->FindIndex(hudIndex);
     if(!posLoc)
        return;
        CHudObj * hudLoc = ( CHudObj* ) m_gStatus->m_hudList->GetAt(posLoc);
    m_hudDBDlgBar.SendDlgItemMessage(IDC_LIST_HUD_unitsDB,LB_RESETCONTENT,0,0);
	for(posLoc = hudLoc->List_displayUnitList->GetHeadPosition();posLoc != NULL;){
		CHudDisplayObj *displayPtr = (CHudDisplayObj *)hudLoc->List_displayUnitList->GetNext(posLoc);
        m_hudDBDlgBar.SendDlgItemMessage(IDC_LIST_HUD_unitsDB,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("Type ->",m_gStatus->m_cabLib.ConvToString(displayPtr->List_unitType)));
	}*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------delete Hud-----------------------------------------------------//
void EngineXWin::OnBTNHUDDeleteHud() 
{
 /* int hudIndex = m_hudDBDlgBar.SendDlgItemMessage(IDC_LIST_HUD_DB,LB_GETCURSEL ,0,0);
  if(hudIndex == -1){
   MessageBox("Must select A HUD");
   return;
  }
   POSITION posLoc = m_gStatus->m_hudList->FindIndex(hudIndex);
     if(!posLoc)
        return;
        CHudObj * hudLoc = ( CHudObj* ) m_gStatus->m_hudList->GetAt(posLoc);
        hudLoc->SafeDelete();
		delete hudLoc;
		hudLoc=0;
   m_gStatus->m_hudList->RemoveAt(posLoc);
   InListHud();*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------delete display Unit--------------------------------------------//
void EngineXWin::OnBTNHUDDeleteDisplayUnit() 
{
/*int hudIndex = m_hudDBDlgBar.SendDlgItemMessage(IDC_LIST_HUD_DB,LB_GETCURSEL ,0,0);
  if(hudIndex == -1){
   MessageBox("Must select A HUD");
   return;
  }
int DisplayIndex = m_hudDBDlgBar.SendDlgItemMessage(IDC_LIST_HUD_unitsDB,LB_GETCURSEL ,0,0);
  if(DisplayIndex == -1){
     MessageBox("Must select A Units");
     return;
  }
   POSITION posLoc = m_gStatus->m_hudList->FindIndex(hudIndex);
   if(!posLoc)
       return;
       CHudObj * hudLoc = ( CHudObj* ) m_gStatus->m_hudList->GetAt(posLoc);
   posLoc = hudLoc->List_displayUnitList->FindIndex(DisplayIndex);
   if(!posLoc)
       return;
       CHudDisplayObj *displayPtr = (CHudDisplayObj *)hudLoc->List_displayUnitList->GetAt(posLoc);
       displayPtr->SafeDelete();
	   delete displayPtr;
	   displayPtr=0;
	   hudLoc->List_displayUnitList->RemoveAt(posLoc);
InListDisplayUnits();*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------delete display Unit--------------------------------------------//
void EngineXWin::OnBTNHUDReplaceUnit() 
{

}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------an amageddon scaleing routine no turnin back-------------------//
void EngineXWin::ScaleAll()
{
float factorEX = .1f;
float transitionMatrix[4][4];
m_gStatus->m_matLib.scale (factorEX,factorEX,factorEX,transitionMatrix);
//world
for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	 CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	    if(ptrLoc->m_meshObject)
			ptrLoc->m_meshObject->ScaleIt(factorEX);    
}
return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Update entity in runtime----------------------------------------------//
void EngineXWin::OnBTNENTUpdate() 
{
int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              MessageBox("Must select an entity object on the entity runtime interface");
		      return;
		   }
	       POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
   D3DMATRIX tempMatrix;
   ptrLoc->List_baseFrame->GetTransform(NULL,&tempMatrix);
   int controlBasis = ptrLoc->List_controlType;
   int refModel = ptrLoc->List_refModel;
   m_gStatus->m_entObjRuntime->RemoveAt(posLoc);
   m_gStatus->DestroyRuntimeEntity(ptrLoc,m_gStatus->m_spawnGeneratorDB,m_gStatus->m_entObjRuntime);
   ptrLoc = 0;
   

   if(controlBasis == 0)
   m_gStatus->ActivateMovementEntity(NULL,
                          0,
                          tempMatrix,
                          0,
						  0,
						  refModel,
						  controlBasis,
						  0,
						  0,
						  FALSE,
						  TRUE,
						  FALSE,
						  -1,
						  -1,
						  NULL,
					      NULL,
						  -1,
						  -1,
					      m_gStatus->m_collisionBase,
						  m_gStatus->m_collisionDatabase,
						  m_gStatus->m_entObjRuntime,
						  m_gStatus->m_currentWorkingFile);
   else
   m_gStatus->ActivateMovementEntity(NULL,
                          0,
                          tempMatrix,
                          0,
                          0,
                          refModel,
                          controlBasis,
                          0,
                          0,
                          FALSE,
                          FALSE,
                          FALSE,
                          -1,
                          -1,
						  NULL,
					      NULL,
						  -1,
						  -1,
							m_gStatus->m_collisionBase,
							m_gStatus->m_collisionDatabase,
							m_gStatus->m_entObjRuntime,
							m_gStatus->m_currentWorkingFile);
	InListRuntimeEntities(runtimeIndex);

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------------------Sel change------------------------------------------------//
/*
void EngineXWin::OnSelchangeLISTTXTDBDatabase() 
{
		int runtimeIndex = m_textureDBDlgBar.SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              return;
		   }
	       POSITION posLoc = m_gStatus->m_textureDataBase->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_gStatus->m_textureDataBase->GetAt(posLoc);
			   
			   m_textureDBDlgBar.SetDlgItemInt(  IDC_BTN_TXB_mipLevels,ptrLoc->m_textureObject->m_dwMipMapCount,TRUE);
               m_textureDBDlgBar.SetDlgItemInt(  IDC_EDIT_TXTDB_TextureName,ptrLoc->m_bump,TRUE);
			   m_textureDBDlgBar.SetDlgItemInt(  IDC_EDIT_TXTDB_TextureBumpType,ptrLoc->m_bumpType,TRUE);
			   m_textureDBDlgBar.SetDlgItemText(  IDC_EDIT_TXTDBPerameter,m_gStatus->m_cabLib.FloatToStingCnv(ptrLoc->m_peramameter1));
               m_textureDBDlgBar.SetDlgItemText(  IDC_EDIT_txtCompressedOrd,m_gStatus->m_cabLib.ConvToString((int)ptrLoc->m_textureObject->m_compressedOvrd));
			   if(ptrLoc->m_textureObject->m_usage == 0)
			      m_textureDBDlgBar.SendDlgItemMessage(  IDC_CHECK_TXB_BitmapLoadOverride,BM_SETCHECK,0,0);
			   else
			      m_textureDBDlgBar.SendDlgItemMessage(  IDC_CHECK_TXB_BitmapLoadOverride,BM_SETCHECK,1,0);
			

}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------------------------Blt on----------------------------------------------//
void EngineXWin::OnCHECKTEXBltOn() 
{
	if(m_textureDBDlgBar.SendDlgItemMessage(IDC_CHECK_TEX_BltOn,BM_GETSTATE ,0,0) == BST_CHECKED)
	{
	   m_gStatus->m_dynamicTextureDataBase->m_compressRef = 1;
	   m_gStatus->m_textureDataBase->m_compressRef = 1;
	   m_gStatus->m_compressTextures = TRUE;
	}
	else
	{
       m_gStatus->m_dynamicTextureDataBase->m_compressRef = 0;
	   m_gStatus->m_textureDataBase->m_compressRef = 0;
	   m_gStatus->m_compressTextures = FALSE;
	}
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------if returns true somthing does require the mouse---------------------------//
void EngineXWin::OnBTNMenuClone() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------------material-------------------------------------------------//
void EngineXWin::OnBTNMenuMaterial3D() 
{
   
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------------------add animation---------------------------------------------//
void EngineXWin::OnBTNMenuAddAnimation() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnSelchangeLISTMenuAnimations() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnSelchangeLISTMENUUnitList() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------maintain animations-----------------------------------------------------------//
void EngineXWin::OnBTNMenuproperties() 
{

}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*
void EngineXWin::OnBTNENVFogDay() 
{
#ifndef _ClientOutput
CColorDlg cdlg;
cdlg.m_red = m_gStatus->m_environmentPtr->m_fogRed;
cdlg.m_green = m_gStatus->m_environmentPtr->m_fogGreen;
cdlg.m_blue = m_gStatus->m_environmentPtr->m_fogBlue;
 if(cdlg.DoModal()==IDOK){
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogRed,m_gStatus->m_cabLib.FloatToStingCnv(cdlg.m_red));
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogGreen,m_gStatus->m_cabLib.FloatToStingCnv(cdlg.m_green));
	m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogBlue,m_gStatus->m_cabLib.FloatToStingCnv(cdlg.m_blue));	
    m_gStatus->m_environmentPtr->m_fogRed = cdlg.m_red;
	m_gStatus->m_environmentPtr->m_fogGreen = cdlg.m_green;
	m_gStatus->m_environmentPtr->m_fogBlue = cdlg.m_blue;
	m_gStatus->InitializeEnvironment(TRUE);
 }
#endif
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnCHECKENVAmbientLightOn() 
{
	if(m_gStatus->m_environmentPtr->m_ambientLightOn == TRUE){
       m_gStatus->m_environmentPtr->m_ambientLightOn = FALSE;   
	   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK,BST_UNCHECKED,0);
	}
	else{
       m_gStatus->m_environmentPtr->m_ambientLightOn = TRUE;
       m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK ,BST_CHECKED,0);
	}
	m_gStatus->InitializeEnvironment(TRUE);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENVambientDayLight() 
{
#ifndef _ClientOutput
 CColorDlg cdlg;
 cdlg.m_red    = m_gStatus->m_environmentPtr->m_ambientLightRedDay;
 cdlg.m_green  = m_gStatus->m_environmentPtr->m_ambientLightGreenDay;
 cdlg.m_blue   = m_gStatus->m_environmentPtr->m_ambientLightBlueDay;
 if(cdlg.DoModal()==IDOK){
    m_gStatus->m_environmentPtr->m_ambientLightRedDay     = cdlg.m_red;
	m_gStatus->m_environmentPtr->m_ambientLightGreenDay   = cdlg.m_green;
	m_gStatus->m_environmentPtr->m_ambientLightBlueDay    = cdlg.m_blue;
 }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnCHECKENVTimeSystem() 
{
  if(m_gStatus->m_environmentPtr->m_enableTimeSystem == TRUE){
       m_gStatus->m_environmentPtr->m_enableTimeSystem = FALSE;   
	   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK,BST_UNCHECKED,0);
  }
  else{
       m_gStatus->m_environmentPtr->m_enableTimeSystem = TRUE;
       m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK ,BST_CHECKED,0);
  }
  m_gStatus->InitializeEnvironment(TRUE);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnCHECKENVWindRandomizer() 
{
  if(m_gStatus->m_environmentPtr->m_enableWindRandomizer == TRUE){
       m_gStatus->m_environmentPtr->m_enableWindRandomizer = FALSE;   
	   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK,BST_UNCHECKED,0);
  }
  else{
       m_gStatus->m_environmentPtr->m_enableWindRandomizer = TRUE;
       m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK ,BST_CHECKED,0);
  }
  m_gStatus->InitializeEnvironment(TRUE);
}*/
//------------------------------------------------------------------------------------//
void EngineXWin::ParticleDatabaseInList(int sel)
{
if(!m_gStatus->m_particleDatabase)
	 return;
	
int traceIt=0;
    m_particleDBDlgBar.SendDlgItemMessage(IDC_LIST_PART_Database,LB_RESETCONTENT,0,0);	
	for(POSITION posLoc = m_gStatus->m_particleDatabase->GetHeadPosition();posLoc != NULL;){
        EnvParticleSystem * partPtr = (EnvParticleSystem *)m_gStatus->m_particleDatabase->GetNext(posLoc);
        m_particleDBDlgBar.SendDlgItemMessage(IDC_LIST_PART_Database,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+(m_gStatus->m_cabLib.ConvToString(traceIt)," "+partPtr->m_systemName));
		traceIt++;
	}
	if(sel != -1)
	   m_particleDBDlgBar.SendDlgItemMessage(IDC_LIST_PART_Database,LB_SETCURSEL ,sel,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNPARTDeleteSystem() 
{
	if(!m_gStatus->m_particleDatabase)
		return;

	int runtimeIndex = m_particleDBDlgBar.SendDlgItemMessage(IDC_LIST_PART_Database,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select an particle system");
		return;
	}
	POSITION posLoc = m_gStatus->m_particleDatabase->FindIndex(runtimeIndex);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	EnvParticleSystem * ptrLoc = ( EnvParticleSystem* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
	delete ptrLoc;
	ptrLoc=0;
	m_gStatus->m_particleDatabase->RemoveAt(posLoc);
	ParticleDatabaseInList(runtimeIndex-1);	
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNPARTLoadDatabase() 
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
   static char BASED_CODE filter[] = "PRT Files|*.PRT|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();  
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadParticleDatabase(fileName);
       ParticleDatabaseInList(-1);
	 }
	 SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNPARTSaveDatabase() 
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
     static char BASED_CODE filter[] = "PRT Files|*.PRT|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "PRT", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_gStatus->m_particleDatabase;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNPARTTestSystem() 
{
int runtimeIndex = m_particleDBDlgBar.SendDlgItemMessage(IDC_LIST_PART_Database,LB_GETCURSEL ,0,0);
 if(runtimeIndex == -1){
    MessageBox("Must select an particle system");
	return;
 }
 POSITION posLoc = m_gStatus->m_particleDatabase->FindIndex(runtimeIndex);
 if(!posLoc)
     return;
     EnvParticleSystem * ptrLoc = ( EnvParticleSystem* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
	 if(m_gStatus->m_particleTest)
	 {
		delete m_gStatus->m_particleTest;
		m_gStatus->m_particleTest = 0;
	 }
	 ptrLoc->Clone(&m_gStatus->m_particleTest,m_gStatus->m_dxRenderer->m_pd3dDevice);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNPARTStopTest() 
{
	if(m_gStatus->m_particleTest)
	 {
		delete m_gStatus->m_particleTest;
		m_gStatus->m_particleTest = 0;
	 }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------------Particle Callback---------------------------------------------//
void EngineXWin::AddParticleSystem
	(
	CString    fileName,
	float      meshScale,
	int        lifeTime,
	BOOL       sortOn,
	float      startRed,
	float      startGreen,
	float      startBlue,
	float      radius,
	BOOL       sphereType,
	float      startHieght,
	int        fadeStart,
	int        fadeEnd,
	int        scaleStart,
	int        scaleEnd,
	BOOL       endSpawn,
	int        texture,
	float      initialSpeedX,
	float      initialSpeedY,
	float      initialSpeedZ,
	float      directpartSpeed,
	float      directpartSpread,
	float      bspd,
	float      bspddr,
	float      bsprd,
    float      bsprddr,
	BOOL       isDirectional,
	BOOL       fadeTime,
	BOOL       Regenerate,
	BOOL	   VaryLifetime,
	int        howManyParticles,
	BOOL       billBoardOn,
	EnvParticleSystem ** retPtr
	)
{
	CString ext = m_gStatus->m_cabLib.GetFileExt(fileName);
	EnvParticleSystem * pParticlePtr = new EnvParticleSystem();
    //get texture name
	CString tempName = "DUMMY.TGA";
	POSITION texPositionLoc = m_gStatus->m_textureDataBase->FindIndex(texture);
	if(texPositionLoc)
	{
	   CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetNext(texPositionLoc);
	   tempName = txtPtrLoc->List_textureName;	   
	}

	

	pParticlePtr->im_mesh->List_textureSelect[0] = texture;
	//if( meshScale > 0.0f )
		//pParticlePtr->im_mesh->ScaleIt(meshScale);

	pParticlePtr->m_Sort = sortOn;
	
	D3DVECTOR speed;

	if( isDirectional==TRUE )
		{
		speed.x = directpartSpeed;
		speed.y = directpartSpread;
		}
	else
		{
		speed.x = initialSpeedX;
		speed.y = initialSpeedY;
		speed.z = initialSpeedZ;
		}
	
	D3DCOLORVALUE color;
	color.r = startRed;
	color.g = startGreen;
	color.b = startBlue;
	
	pParticlePtr->CreateField
		(
		howManyParticles,
		lifeTime,
		startHieght,
		radius,
		sphereType,
		isDirectional,
		Regenerate,
		VaryLifetime,
		speed,
		scaleStart,
		scaleEnd
		);

	if( isDirectional==TRUE )
		pParticlePtr->Burst(bspd, bspddr, bsprd, bsprddr);

	pParticlePtr->storedFileName = fileName;
	pParticlePtr->m_IsBillboard = billBoardOn;
	
	pParticlePtr->SetFade(fadeStart,fadeEnd,endSpawn,color,fadeTime);
	pParticlePtr->im_mesh->List_textureSelect[0] = texture;
      
	int indexaTracel = 0;
    for(texPositionLoc = m_gStatus->m_textureDataBase->GetHeadPosition();texPositionLoc != NULL;){
		CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetNext(texPositionLoc);
        if(indexaTracel == pParticlePtr->im_mesh->List_textureSelect[0]){
           pParticlePtr->im_mesh->List_storedTextureFromFile[0] = txtPtrLoc->List_textureName;
		}
		indexaTracel++;
	} 
	
	*retPtr = pParticlePtr;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNSpawnAddGen() 
{
#ifndef _ClientOutput
	CAddSpawnGeneratorDlg dlg;
	dlg.m_aiCfgDatabaseRef    = m_gStatus->m_aiList;
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
       dlg.treeList = m_gStatus->GL_aiWebDatabase;
	   dlg.spawnObj = spawnGenPtr;
	   dlg.useRuntimeEntityList = FALSE;

    if(dlg.DoModal() == IDOK){
       spawnGenPtr->List_spawnName = dlg.m_generatorName;
	   if(dlg.useRuntimeEntityList == TRUE){
         int ENIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	       if(ENIndex == -1){
              MessageBox("Must select an entity object on the entity runtime interface");
		      return;
		   }
	       POSITION position = m_gStatus->m_entObjRuntime->FindIndex(ENIndex);
	       if(position){
            CMovementObj * mPtrtarget = (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(position);
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
	   m_gStatus->m_spawnGeneratorDB->AddTail(spawnGenPtr);
	   SpawnGenDatabaseInList(m_gStatus->m_spawnGeneratorDB->GetCount()-1);
	   m_gStatus->m_spawnGeneratorDB->GetTotalAIOutput();
	}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNSpawnDeleteGen() 
{
int runtimeIndex = m_spawnGenDBDlgBar.SendDlgItemMessage(IDC_LIST_Spawn_listDB,LB_GETCURSEL ,0,0);
 if(runtimeIndex == -1){
    MessageBox("Must select a system");
	return;
 }
 POSITION posLoc = m_gStatus->m_spawnGeneratorDB->FindIndex(runtimeIndex);
 if(!posLoc)
     return;
     CSpawnObj * ptrLoc = ( CSpawnObj* ) m_gStatus->m_spawnGeneratorDB->GetAt(posLoc);
	 delete ptrLoc;
	 ptrLoc=0;
	 m_gStatus->m_spawnGeneratorDB->RemoveAt(posLoc);
	 SpawnGenDatabaseInList(runtimeIndex-1);
	 m_gStatus->m_spawnGeneratorDB->GetTotalAIOutput();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNSpawnEditGen() 
{
#ifndef _ClientOutput
int runtimeIndex = m_spawnGenDBDlgBar.SendDlgItemMessage(IDC_LIST_Spawn_listDB,LB_GETCURSEL ,0,0);
 if(runtimeIndex == -1){
    MessageBox("Must select a system");
	return;
 }
 POSITION posLoc = m_gStatus->m_spawnGeneratorDB->FindIndex(runtimeIndex);
 if(!posLoc)
     return;
     CSpawnObj * spawnGenPtr = ( CSpawnObj* ) m_gStatus->m_spawnGeneratorDB->GetAt(posLoc);
     CAddSpawnGeneratorDlg dlg;
           
	   dlg.m_aiCfgDatabaseRef    = m_gStatus->m_aiList;
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
	   dlg.treeList              = m_gStatus->GL_aiWebDatabase;
	   dlg.spawnObj              = spawnGenPtr;
	   dlg.useRuntimeEntityList = FALSE;

   if(dlg.DoModal()==IDOK){
	   if(dlg.useRuntimeEntityList == TRUE){
         int ENIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	       if(ENIndex == -1){
              MessageBox("Must select an entity object on the entity runtime interface");
		      return;
		   }
	       POSITION position = m_gStatus->m_entObjRuntime->FindIndex(ENIndex);
	       if(position){
            CMovementObj * mPtrtarget = (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(position);
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
	   SpawnGenDatabaseInList(runtimeIndex);
   }	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::SpawnGenDatabaseInList(int sel)
{

}
*/
//----------------------Particle Callback---------------------------------------------//
void EngineXWin::OnBTNObjectBuildCollisionMap() 
{
#ifndef _ClientOutput 

	 m_gStatus->m_collisionBase->GL_worldList = m_gStatus->m_wldObjList;
	 m_gStatus->m_collisionBase->directKeyboard = m_userControls->m_input->m_directKeyboard;
	 CCollisionResolutionDlg dlg;
	 if(m_gStatus->m_collisionBase->SizeSquaredX){
	 dlg.m_XResolution = m_gStatus->m_collisionBase->SizeSquaredX[0];
	 dlg.m_YResolution = m_gStatus->m_collisionBase->SizeSquaredY[0];
	 dlg.m_ZResolution = m_gStatus->m_collisionBase->SizeSquaredZ[0];
	 }
	 dlg.m_levelsDeep = m_gStatus->m_collisionBase->NumberOfLevels;
	 dlg.m_maxfaceResident = m_gStatus->m_collisionBase->MaxFacesInBox;
	 if(dlg.DoModal()==IDOK){
     m_gStatus->m_collisionBase->NumberOfLevels = dlg.m_levelsDeep;
	 m_gStatus->m_collisionBase->MaxFacesInBox = dlg.m_maxfaceResident;	
 
	 if(m_progressInternal)
	    delete m_progressInternal;
        m_progressInternal=0;
     m_progressInternal = new CDialog;
	 m_progressInternal->Create(IDD_DIALOG_ProgressMeter,NULL);
	 m_progressInternal->ShowWindow(SW_SHOW);

       if(m_gStatus->m_collisionBase->CalculateWorld(m_gStatus->m_wldObjList,
		                                   dlg.m_XResolution,
									       dlg.m_YResolution,
									       dlg.m_ZResolution,
										   m_gStatus->m_libraryReferenceDB,
										   m_progressInternal)== FALSE){
		m_gStatus->m_collisionBase = NULL;
        delete m_gStatus->m_collisionBase;
		m_gStatus->m_collisionBase = 0;
		m_gStatus->m_collisionBase = new CCollisionObj();
	   }
	   else
		m_gStatus->m_collisionBase->BuildObjectReferenceArray(m_gStatus->m_wldObjList);
       m_gStatus->m_worldListTemp->RemoveAll();
	   MessageBox("Done");
	   delete m_progressInternal;
	   m_progressInternal = 0;
	   //m_gStatus->m_collisionBase->AssignPopout(m_gStatus->m_wldObjList);
	 }
	 
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------------Generate Normals----------------------------------------------//
void EngineXWin::WorldNormalModifier(int process, int group)
{
BOOL flatShaded = FALSE;
int amount = 0;
for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){
    CWldObject * wldObj = (CWldObject *)m_gStatus->m_wldObjList->GetNext(posLoc);
    if(wldObj->List_groupNumber == group)
	   amount++;
}

if(m_dlgBarObject)
   m_dlgBarObject.SendDlgItemMessage(IDC_PROGRESS_OBJ,PBM_SETRANGE ,0,MAKELPARAM(0,amount));

if(process == 1){//independent normal soothing by object
amount = 0;
for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){
    CWldObject * wldObj = (CWldObject *)m_gStatus->m_wldObjList->GetNext(posLoc);
	
	if(!wldObj->m_meshObject)
	   continue;
	
	if(wldObj->List_groupNumber == group){//is group
	    wldObj->m_meshObject->GenerateNormals(0);
		wldObj->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
		if(m_dlgBarObject)
           m_dlgBarObject.SendDlgItemMessage(IDC_PROGRESS_OBJ,PBM_SETPOS,amount,0);  
		amount++;
	}//end is group
}
return;
}//end independent normal soothing by object
else if(process == 2){//group smoothing
amount = 0;
POSITION posLoc;
  for(posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;)
  {//primary loop
    CWldObject * wldObj = (CWldObject *)m_gStatus->m_wldObjList->GetNext(posLoc);
    if(!wldObj->m_meshObject)
	   continue;

	ABVERTEX *vArray = NULL;
    int vCount = 0;

	wldObj->m_meshObject->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);

	if(wldObj->List_groupNumber == group){//is group
		//check against set
        for(POSITION wldPosInside = m_gStatus->m_wldObjList->GetHeadPosition();wldPosInside != NULL;){//3
            CWldObject * wldObjInside = (CWldObject *)m_gStatus->m_wldObjList->GetNext(wldPosInside);
            if(!wldObjInside->m_meshObject)
	            continue;

			ABVERTEX *subvArray = NULL;
            int subvCount = 0;

	        wldObjInside->m_meshObject->m_abVertexArray.GetABVERTEXStyle(&subvArray,&subvCount);

			for(int loop=0;loop<wldObj->m_meshObject->m_abVertexArray.m_vertexCount;loop++){
				    for(int loop2=0;loop2<wldObjInside->m_meshObject->m_abVertexArray.m_vertexCount;loop2++){//loop through vetexes
		             if(vArray[loop].x == subvArray[loop2].x &&
			            vArray[loop].y == subvArray[loop2].y &&
			            vArray[loop].z == subvArray[loop2].z){//matches
					    
						vArray[loop].nx += subvArray[loop2].nx;
						vArray[loop].ny += subvArray[loop2].ny;
						vArray[loop].nz += subvArray[loop2].nz;
																																								  
					 }
					}//end loop through vetexes
			     
					D3DVECTOR newNormal;
					newNormal.x = vArray[loop].nx;
					newNormal.y = vArray[loop].ny;
					newNormal.z = vArray[loop].nz;

					newNormal = m_gStatus->m_matLib.Normalize( newNormal );
					
					vArray[loop].nx = newNormal.x;
					vArray[loop].ny = newNormal.y;
					vArray[loop].nz = newNormal.z;
			}
			//wldObj->m_meshObject->InitBuffer();
			if(subvArray)
			{
               delete [] subvArray;
               subvArray = 0;
			}
		}//end 3
        
		if(m_dlgBarObject)
           m_dlgBarObject.SendDlgItemMessage(IDC_PROGRESS_OBJ,PBM_SETPOS,amount,0);  
		amount++;
	}//end is group
	if(vArray)
	{
			   wldObj->m_meshObject->m_abVertexArray.SetABVERTEXStyle(NULL,
												                      vArray,
												                      vCount);
               delete [] vArray;
			   vArray = 0;
	}
  }//end primary loop
        //--------------------------//
		//--------------------------//
		//--------------------------//
		for(posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;)
		{//8
            CWldObject * wldObj = (CWldObject *)m_gStatus->m_wldObjList->GetNext(posLoc);
            if(!wldObj->m_meshObject)
	         continue;

            if(wldObj->List_groupNumber != group)
				continue;

			ABVERTEX *vArray = NULL;
            int vCount = 0;
	        wldObj->m_meshObject->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);

            for(POSITION wldPosInside = m_gStatus->m_wldObjList->GetHeadPosition();wldPosInside != NULL;){//3
            CWldObject * wldObjInside = (CWldObject *)m_gStatus->m_wldObjList->GetNext(wldPosInside);
            if(!wldObjInside->m_meshObject)
	            continue;

			    if(wldObjInside->List_groupNumber != group)
				   continue;
			
				 ABVERTEX *subvArray = NULL;
                 int subvCount = 0;
	             wldObjInside->m_meshObject->m_abVertexArray.GetABVERTEXStyle(&subvArray,&subvCount);

			     for(int loop=0;loop<wldObj->m_meshObject->m_abVertexArray.m_vertexCount;loop++){
				    for(int loop2=0;loop2<wldObjInside->m_meshObject->m_abVertexArray.m_vertexCount;loop2++){//loop through vetexes
		             if(vArray[loop].x == subvArray[loop2].x &&
			            vArray[loop].y == subvArray[loop2].y &&
			            vArray[loop].z == subvArray[loop2].z){//matches
					    
						subvArray[loop2].nx = vArray[loop].nx;
						subvArray[loop2].ny = vArray[loop].ny;
						subvArray[loop2].nz = vArray[loop].nz;																																				  
					 }
					}//end loop through vetexes
				 }
              if(subvArray)
			  {
			   wldObjInside->m_meshObject->m_abVertexArray.SetABVERTEXStyle(NULL,
												                      subvArray,
												                      subvCount);
               delete [] subvArray;
			   subvArray = 0;
			  }
			  wldObjInside->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
			}//end 3
            if(vArray)
			{
               delete [] vArray;
			   vArray = 0;
			}
			//wldObj->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice);
		}//end 8
		//--------------------------//
		//--------------------------//
		//--------------------------//
}//end group smoothing
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------------In list objects---------------------//
void EngineXWin::ObjectsInListAll() 
{
if(!m_gStatus->m_wldObjList)
return;
  /*for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	

	for(POSITION posLoc2 = ptrLoc->List_objContentList->GetHeadPosition();posLoc2 != NULL;){//object hiachy list
	        CContent * ptrLoc2 = ( CContent* ) ptrLoc->List_objContentList->GetNext(posLoc2);
	    //animation entry loop
			for(POSITION posLoc3 = ptrLoc2->List_animationList->GetHeadPosition();posLoc3 != NULL;){//loopd
	              CAnimationObj * ptrAnim = ( CAnimationObj* ) ptrLoc2->List_animationList->GetNext(posLoc3);
			      HTREEITEM child2 = AddTreeChild(ptrAnim->List_animationName,IDC_OBJECT_TREE,&m_dlgBarObject,child1);
			}
	}
  }*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------------In list objects---------------------//

///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	OnClickObjectTree(NMHDR*, LRESULT*)
// Description:	Click event handler for the Object Editor dialog Group Visuals
//				TreeView control.
///////////////////////////////////////////////////////////////////////////////

void
EngineXWin::OnClickObjectTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	HWND treeHwnd = NULL;
	m_dlgBarObject.GetDlgItem(IDC_OBJECT_TREE, &treeHwnd);
	if(treeHwnd)
	{
		POINT pt;
		if(GetCursorPos(&pt))
		{
			::ScreenToClient(treeHwnd, &pt);
			TVHITTESTINFO ht;
			ZeroMemory(&ht, sizeof(ht));
			ht.pt = pt;
			HTREEITEM hitItem = TreeView_HitTest(treeHwnd, &ht);
			if(hitItem && (ht.flags & (TVHT_ONITEM | TVHT_ONITEMRIGHT)))
			{
				TreeView_SelectItem(treeHwnd, hitItem);
			}
		}
	}
	VisualPropertiesInList();
	if(pNMHDR && pNMHDR->code == NM_RCLICK)
	{
		TeleportEditorToSelectedObject(m_gStatus);
	}
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	OnSelchangedObjectTree(NMHDR*, LRESULT*)
// Description:	Selection Changed event handler for the Object Editor dialog
//				Group Visuals TreeView control.
///////////////////////////////////////////////////////////////////////////////

void
EngineXWin::OnSelchangedObjectTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	VisualPropertiesInList();

	*pResult = 0;
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------------In list objects----------------------------------//

///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	VisualPropertiesInList()
// Description:
///////////////////////////////////////////////////////////////////////////////

void
EngineXWin::VisualPropertiesInList()
{
	HWND treeHwnd = NULL;
	m_dlgBarObject.GetDlgItem(IDC_OBJECT_TREE, &treeHwnd);
	HTREEITEM item = TreeView_GetSelection(treeHwnd);
	if(!item || !m_gStatus || !m_gStatus->m_wldObjList)
		return;
	int indexOfObject = GetTreeChildIndex(treeHwnd, item);
	HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
	if(!ObjectRoot)
	{
		ObjectRoot = item;
		TreeView_Expand(treeHwnd, ObjectRoot, TVE_EXPAND);
	}
	CString textLoc = GetTreeItemText(IDC_OBJECT_TREE, &m_dlgBarObject, ObjectRoot, 32);

	// World object loop
	for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition(); posLoc != NULL;)
	{
		CWldObject *ptrLoc = static_cast<CWldObject*>(m_gStatus->m_wldObjList->GetNext(posLoc));
		CString tracename = operator+("TRC_", (m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));

		if(tracename == textLoc)
		{
			g_editorHighlightTraceId = ptrLoc->List_identity;
			UpdateEditorObjectHighlight(m_gStatus, TRUE, FALSE);

			// Clear the Visual Attributes listbox control
			m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes, LB_RESETCONTENT, 0, 0);

			D3DVECTOR minPt, maxPt, centerPt;
			if(GetEditorWorldObjectBounds(ptrLoc, minPt, maxPt, centerPt))
			{
				CString posLine;
				posLine.Format("Pos: (%.1f, %.1f, %.1f) [RClick/Ctrl+F=GoTo]", centerPt.x, centerPt.y, centerPt.z);
				m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,
					LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)posLine);

				CString sizeLine;
				sizeLine.Format("Size: %.1f x %.1f x %.1f  (Group %d)",
					maxPt.x - minPt.x, maxPt.y - minPt.y, maxPt.z - minPt.z, ptrLoc->List_groupNumber);
				m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,
					LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)sizeLine);
			}

			if(ptrLoc->List_usedAsSpawnPoint != -1)
			{
				m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,
					LB_ADDSTRING,
					0,
					(LPARAM)(LPCTSTR)operator+("Spawn Point Index ::", m_gStatus->m_cabLib.ConvToString(ptrLoc->List_usedAsSpawnPoint)));
			}

			// Determine if a light exists for the currently selected object
			if(ptrLoc->m_lightObject)
			{
				m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,
					LB_ADDSTRING,
					0,
					(LPARAM)(LPCTSTR)"Light Exists");
			}

			// Determine if the currently selected object has a trigger
			if(ptrLoc->List_triggerList)
			{
				if(ptrLoc->List_triggerList->GetCount() > 0)
				{
					m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,
						LB_ADDSTRING,
						0,
						(LPARAM)(LPCTSTR)"Trigger Exists");
				}
			}

			// Determine if the currently selected object has LOD data
			if(ptrLoc->m_meshObject)
			{
				/* DEPRECATED /////////////////////////////////////////////////////
				if(ptrContent->List_imMesh->List_howManyTextures == 0)
				{
					m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Single Passed");
					m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,
						LB_ADDSTRING,
						0,
						(LPARAM)(LPCTSTR)operator+(
							ptrContent->List_imMesh->List_storedTextureFromFile[0],
							operator+(" index ::",
							m_gStatus->m_cabLib.ConvToString(ptrContent->List_imMesh->List_textureSelect[0]))));
				}
				else if(ptrContent->List_imMesh->List_howManyTextures == 1)
				{
					m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Multipass Passed");
					m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,
						LB_ADDSTRING,
						0,
						(LPARAM)(LPCTSTR)operator+(ptrContent->List_imMesh->List_storedTextureFromFile[0],
							operator+(" index ::",
							m_gStatus->m_cabLib.ConvToString(ptrContent->List_imMesh->List_textureSelect[0]))));
					m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_VisualAttributes,
						LB_ADDSTRING,
						0,
						(LPARAM)(LPCTSTR)operator+(ptrContent->List_imMesh->List_storedTextureFromFile[1],
							operator+(" index ::",
							m_gStatus->m_cabLib.ConvToString(ptrContent->List_imMesh->List_textureSelect[1]))));
				}
				/////////////////////////////////////////////////////////////////*/

				m_dlgBarObject.SendDlgItemMessage(IDC_LIST_OBJ_LODdata,LB_RESETCONTENT,0,0);
				int tracer = 0;

				// LOD data
				if(ptrLoc->m_lodVisualList)
				{
					for(POSITION posLoc = ptrLoc->m_lodVisualList->GetHeadPosition();posLoc != NULL;)
					{
						CMeshLODObject * lodPtr = (CMeshLODObject *)ptrLoc->m_lodVisualList->GetNext(posLoc);
						CString buildStr = m_gStatus->m_cabLib.FloatToStingCnv(lodPtr->m_distance);
						CString numBuild = m_gStatus->m_cabLib.ConvToString(tracer);
						CString finalBuild = numBuild + " : LOD LEVEL";
						finalBuild = finalBuild + "  Dist : ";
						finalBuild = finalBuild + buildStr;
						m_dlgBarObject.SendDlgItemMessage(IDC_LIST_OBJ_LODdata,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)finalBuild);
						tracer++;
					}
				}
			}
		}
	}
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::WorldGroupInList(int sel)
{

   m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_RESETCONTENT,0,0);
   for(POSITION posLoc = m_gStatus->m_worldGroupList->GetHeadPosition();posLoc != NULL;){
       CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetNext(posLoc);
       m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+(wldPtr->List_groupName,operator+(" ID::",m_gStatus->m_cabLib.ConvToString(wldPtr->List_groupInt))));
   }
if(sel != -1)
	   m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_SETCURSEL ,sel,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNObjectsAddGroup() 
{
#ifndef _ClientOutput
CWorldGroupDlg dlg;
   
   int newIndex=0;
   BOOL gotIt = FALSE;
   while(gotIt == FALSE){
	   gotIt = TRUE;
   for(POSITION posLoc = m_gStatus->m_worldGroupList->GetHeadPosition();posLoc != NULL;){
       CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetNext(posLoc);
       if(wldPtr->List_groupInt == newIndex){
		  newIndex++;
		  gotIt = FALSE;
	      break;
	   }
   }
   }

   dlg.m_groupName = "Unknown";
   dlg.m_groupID = newIndex;
   if(dlg.DoModal()==IDOK){
	  for(POSITION posLoc = m_gStatus->m_worldGroupList->GetHeadPosition();posLoc != NULL;){
       CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetNext(posLoc);
       if(wldPtr->List_groupInt == dlg.m_groupID){
		  MessageBox("ID already exists");
		  return;
	   }
	  }
      CWldGroupObj * groupPtr = new CWldGroupObj("NA",m_gStatus->m_worldGroupList->GetCount());
	  groupPtr->List_groupName = dlg.m_groupName;
      groupPtr->List_groupInt = dlg.m_groupID;
      m_gStatus->m_worldGroupList->AddTail(groupPtr);
	  WorldGroupInList(-1);
   }	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------------------In list objects----------------------------------//
void EngineXWin::OnBTNObjectsDeleteGroup() 
{
	int runtimeIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1)
	{
		MessageBox("Must select a group");
		return;
	}
	POSITION posLoc = m_gStatus->m_worldGroupList->FindIndex(runtimeIndex);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetAt(posLoc);
	POSITION posLast,wPos;
	for(wPos = m_gStatus->m_wldObjList->GetHeadPosition();(posLast = wPos) != NULL;)
	{
		CWldObject * wObj = (CWldObject *)m_gStatus->m_wldObjList->GetNext(wPos);
		if(wObj->List_groupNumber == wldPtr->List_groupInt)
		{
			wObj->SafeDelete();
			delete wObj;
			wObj=0;
			m_gStatus->m_wldObjList->RemoveAt(posLast);
		}
	}
	
	delete wldPtr;
	wldPtr=0;
	m_gStatus->m_worldGroupList->RemoveAt(posLoc);
	WorldGroupInList(runtimeIndex-1);

	for(wPos = m_gStatus->m_worldRenderList->GetHeadPosition();wPos != NULL;)
	{
		CWldObject * wObj = (CWldObject *)m_gStatus->m_worldRenderList->GetNext(wPos);
		wObj->List_poppedOut = TRUE;
	}
	m_gStatus->m_worldRenderList->RemoveAll();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNObjectsEditGroup() 
{
#ifndef _ClientOutput
int runtimeIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_GETCURSEL ,0,0);
 if(runtimeIndex == -1){
	MessageBox("Must select a group");
	return;
 }
 POSITION posLoc = m_gStatus->m_worldGroupList->FindIndex(runtimeIndex);
 if(!posLoc)
     return;
     CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetAt(posLoc);
	 CWorldGroupDlg dlg;
	 dlg.m_groupID = wldPtr->List_groupInt;
	 dlg.m_groupName = wldPtr->List_groupName;
	 if(dlg.DoModal()==IDOK){
        wldPtr->List_groupInt = dlg.m_groupID;
	    wldPtr->List_groupName = dlg.m_groupName;
	 }
	 WorldGroupInList(runtimeIndex);	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnSelchangeLISTObjectsGroupList2Box() 
{
int runtimeIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_GETCURSEL ,0,0);
 if(runtimeIndex == -1){
	return;
 }
 POSITION posLoc = m_gStatus->m_worldGroupList->FindIndex(runtimeIndex);
 if(!posLoc)
     return;
     CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetAt(posLoc);
     ObjectTreeViewInlist(wldPtr->List_groupInt,0);	
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnDblclkLISTObjectsGroupList2Box() 
{
#ifndef _ClientOutput
int runtimeIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_GETCURSEL ,0,0);
 if(runtimeIndex == -1){
	return;
 }
 POSITION posLoc = m_gStatus->m_worldGroupList->FindIndex(runtimeIndex);
 if(!posLoc)
     return;
     CWldGroupObj * groupPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetAt(posLoc);

CGroupModifierDlg       dlg;
dlg.GL_textureDataBase= m_gStatus->m_textureDataBase;
dlg.g_pd3dDeviceRef   = m_gStatus->m_dxRenderer->m_pd3dDevice;
dlg.diffuseRed        = 0.0f;
dlg.diffuseGreen      = 0.0f;
dlg.diffuseBlue       = 0.0f;
dlg.specularRed       = 0.0f;
dlg.specularGreen     = 0.0f;
dlg.specularBlue      = 0.0f;
dlg.emmisiveRed       = 0.0f;
dlg.emmisiveGreen     = 0.0f;
dlg.emmisiveBlue      = 0.0f;
dlg.m_reletiveBoxSize = 0.0f;
  if(dlg.DoModal()==IDOK){//applicate
     if(dlg.UnifyVertsByTolerance)
     UnionVertexesPositionalyByTolerance(dlg.m_tolerance,groupPtr->List_groupInt);


     if(dlg.normalModEnable == TRUE){//normal modification
				if(dlg.inipendentNormalType == TRUE){
                   WorldNormalModifier(1,groupPtr->List_groupInt);
				}
				else{
                   WorldNormalModifier(2,groupPtr->List_groupInt);
				}
	 }//end normal modification

     for(posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
      CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
      if(ptrLoc->List_groupNumber == groupPtr->List_groupInt || dlg.m_applyGlobal == TRUE){//filter only group
			if(dlg.m_diffuseEffectEnable == TRUE)
			if(ptrLoc->m_meshObject)
			if(ptrLoc->m_meshObject->List_materialObject)
			{
               ptrLoc->m_meshObject->List_materialObject->m_diffuseEffect = dlg.m_diffuseVxEffect; 
			   //ptrLoc->m_meshObject->m_abVertexArray.InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,ptrLoc->m_meshObject->List_materialObject->m_diffuseEffect);
			   if(ptrLoc->m_lodVisualList)
			   for(POSITION lodPos = ptrLoc->m_lodVisualList->GetHeadPosition();lodPos != NULL;)
			   {
                   CMeshLODObject * mLod = (CMeshLODObject *) ptrLoc->m_lodVisualList->GetNext(lodPos);
                   mLod->m_meshLodLevel->List_materialObject->m_diffuseEffect = dlg.m_diffuseVxEffect;
	               //mLod->m_meshLodLevel->m_abVertexArray.InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,mLod->m_meshLodLevel->List_materialObject->m_diffuseEffect);
			   }
			}

			if(dlg.m_renderFilterEnable == TRUE)
			{
               ptrLoc->m_renderFilter = dlg.m_renderFilterOn;
			}
		  
		    if(dlg.m_groupIdModifier == TRUE)
			{
               ptrLoc->List_groupNumber = dlg.m_modGroupID; 
			}
			if(dlg.particleEmmiterEnable == TRUE)
			{//particle assignment
               ptrLoc->m_particleLink = dlg.m_particleDBref;
			}//end particle assignment
			if(dlg.m_rangeModEnable == TRUE)
			{//particle assignment
				//dlg.m_rangeMod
              
				if(ptrLoc->m_lightObject)
				{
                   ptrLoc->List_popoutBox.max.x = ptrLoc->m_lightObject->m_wldspacePos.x + dlg.m_rangeMod;
                   ptrLoc->List_popoutBox.max.y = ptrLoc->m_lightObject->m_wldspacePos.y + dlg.m_rangeMod;
				   ptrLoc->List_popoutBox.max.z = ptrLoc->m_lightObject->m_wldspacePos.z + dlg.m_rangeMod;

				   ptrLoc->List_popoutBox.min.x = ptrLoc->m_lightObject->m_wldspacePos.x - dlg.m_rangeMod;
                   ptrLoc->List_popoutBox.min.y = ptrLoc->m_lightObject->m_wldspacePos.y - dlg.m_rangeMod;
				   ptrLoc->List_popoutBox.min.z = ptrLoc->m_lightObject->m_wldspacePos.z - dlg.m_rangeMod;
				}

				if(ptrLoc->m_meshObject)
				{
                   ptrLoc->List_popoutBox.max.x = ptrLoc->m_meshObject->m_meshCenter.x + dlg.m_rangeMod;
                   ptrLoc->List_popoutBox.max.y = ptrLoc->m_meshObject->m_meshCenter.y + dlg.m_rangeMod;
				   ptrLoc->List_popoutBox.max.z = ptrLoc->m_meshObject->m_meshCenter.z + dlg.m_rangeMod;

				   ptrLoc->List_popoutBox.min.x = ptrLoc->m_meshObject->m_meshCenter.x - dlg.m_rangeMod;
                   ptrLoc->List_popoutBox.min.y = ptrLoc->m_meshObject->m_meshCenter.y - dlg.m_rangeMod;
				   ptrLoc->List_popoutBox.min.z = ptrLoc->m_meshObject->m_meshCenter.z - dlg.m_rangeMod;
				}
			}//end particle assignment
			if(dlg.m_enableCharonaOption == TRUE)
			{//charona assignment
               if(dlg.m_charonaObject)
			   {
				  if(ptrLoc->m_meshObject)
				  if(ptrLoc->m_meshObject->m_charona)
				  {
                    ptrLoc->m_meshObject->m_charona->SafeDelete();
					delete ptrLoc->m_meshObject->m_charona;
					ptrLoc->m_meshObject->m_charona = 0;
				  }

				  if(ptrLoc->m_charona)
				  {
                    ptrLoc->m_charona->SafeDelete();
					delete ptrLoc->m_charona;
					ptrLoc->m_charona = 0;
				  }


                  if(ptrLoc->m_meshObject)
                     dlg.m_charonaObject->Clone(&ptrLoc->m_meshObject->m_charona);
				  else
                     dlg.m_charonaObject->Clone(&ptrLoc->m_charona);

				  if(ptrLoc->m_meshObject)
				  if(ptrLoc->m_meshObject->m_charona){
			         ptrLoc->m_meshObject->m_charona->InitializeVis();
			         ptrLoc->m_meshObject->m_charona->m_visual->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,ptrLoc->m_meshObject->m_charona->m_visual->m_uvCount);
				  }

				  if(ptrLoc->m_charona){
			         ptrLoc->m_charona->InitializeVis();
			         ptrLoc->m_charona->m_visual->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,ptrLoc->m_charona->m_visual->m_uvCount);
				  }
			   }
			   else
			   {
                 if(ptrLoc->m_meshObject->m_charona)
				 {
                    ptrLoc->m_meshObject->m_charona->SafeDelete();
					delete ptrLoc->m_meshObject->m_charona;
					ptrLoc->m_meshObject->m_charona = 0;
				 }
			   }
			}//end charona assignment
			if(dlg.m_enableLightOption == TRUE)
			{//light assignment
               if(dlg.m_lightObject)
			   {
                  if(dlg.m_lightObject->m_range == 0.0)
				  {
                     if(ptrLoc->m_lightObject)
                        ptrLoc->m_lightObject->m_lightOn = dlg.m_lightObject->m_lightOn;
				  }
				  else
				  {
					  if(ptrLoc->m_lightObject)
					  {
						delete ptrLoc->m_lightObject;
						ptrLoc->m_lightObject = 0;
					  }
					  dlg.m_lightObject->Clone(&ptrLoc->m_lightObject);
				  }
			   }
			   else
			   {
                 if(ptrLoc->m_lightObject)
				 {
					delete ptrLoc->m_lightObject;
					ptrLoc->m_lightObject = 0;
				 }
			   }
			}//end light assignment
		    if(dlg.materialModEnable == TRUE){//material modification
			 
	           ptrLoc->m_meshObject->List_materialObject->m_baseMaterial.Emissive.r   = dlg.emmisiveRed;
	           ptrLoc->m_meshObject->List_materialObject->m_baseMaterial.Emissive.g   = dlg.emmisiveGreen;
	           ptrLoc->m_meshObject->List_materialObject->m_baseMaterial.Emissive.b   = dlg.emmisiveBlue;	   
	           ptrLoc->m_meshObject->List_materialObject->m_baseMaterial.Specular.r   = dlg.specularRed;
	           ptrLoc->m_meshObject->List_materialObject->m_baseMaterial.Specular.g   = dlg.specularGreen;
	           ptrLoc->m_meshObject->List_materialObject->m_baseMaterial.Specular.b   = dlg.specularBlue;
	           ptrLoc->m_meshObject->List_materialObject->m_baseMaterial.Diffuse.r    = dlg.diffuseRed;
	           ptrLoc->m_meshObject->List_materialObject->m_baseMaterial.Diffuse.g    = dlg.diffuseGreen;
	           ptrLoc->m_meshObject->List_materialObject->m_baseMaterial.Diffuse.b    = dlg.diffuseBlue;
			   ptrLoc->m_meshObject->List_materialObject->Reset();
			}
			if(dlg.multipassModEnable == TRUE){//material modification
               ptrLoc->m_meshObject->List_BlendMethod[0] = dlg.blendMode;
			}
			if(dlg.textureDataSwapEnable == TRUE){//material modification
               ptrLoc->m_meshObject->ExchangeTextureSets(dlg.m_texturedataSwapIndex1,dlg.m_texturedataSwapIndex2);
			   ptrLoc->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
			}
			if(dlg.meshOptimizer == TRUE){//material modification
			   ptrLoc->m_meshObject->OptimizeMesh(&m_dlgBarObject);
		       ptrLoc->m_meshObject->GenerateNormals(0);
	           ptrLoc->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
			   if(ptrLoc->m_lodVisualList)
			   for(POSITION lodPos = ptrLoc->m_lodVisualList->GetHeadPosition();lodPos != NULL;)
			   {
                   CMeshLODObject * mLod = (CMeshLODObject *) ptrLoc->m_lodVisualList->GetNext(lodPos);
                   mLod->m_meshLodLevel->OptimizeMesh(&m_dlgBarObject);
		           mLod->m_meshLodLevel->GenerateNormals(0);
	               mLod->m_meshLodLevel->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
			   }
			}
			if(dlg.boundingBoxModifier == TRUE){//bounding box modifier
               ptrLoc->m_meshObject->GetBox();
			    ptrLoc->List_popoutBox.max.x = ptrLoc->m_meshObject->List_boundingBox.maxX + ((ptrLoc->m_meshObject->List_boundingBox.maxX-ptrLoc->m_meshObject->List_boundingBox.minX) * dlg.m_groupMultiplier);
	            ptrLoc->List_popoutBox.max.y = ptrLoc->m_meshObject->List_boundingBox.maxY + ((ptrLoc->m_meshObject->List_boundingBox.maxY-ptrLoc->m_meshObject->List_boundingBox.minY) * dlg.m_groupMultiplier);
	            ptrLoc->List_popoutBox.max.z = ptrLoc->m_meshObject->List_boundingBox.maxZ + ((ptrLoc->m_meshObject->List_boundingBox.maxZ-ptrLoc->m_meshObject->List_boundingBox.minZ) * dlg.m_groupMultiplier);
	            ptrLoc->List_popoutBox.min.x = ptrLoc->m_meshObject->List_boundingBox.minX - ((ptrLoc->m_meshObject->List_boundingBox.maxX-ptrLoc->m_meshObject->List_boundingBox.minX) * dlg.m_groupMultiplier);
	            ptrLoc->List_popoutBox.min.y = ptrLoc->m_meshObject->List_boundingBox.minY - ((ptrLoc->m_meshObject->List_boundingBox.maxY-ptrLoc->m_meshObject->List_boundingBox.minY) * dlg.m_groupMultiplier);
	            ptrLoc->List_popoutBox.min.z = ptrLoc->m_meshObject->List_boundingBox.minZ - ((ptrLoc->m_meshObject->List_boundingBox.maxZ-ptrLoc->m_meshObject->List_boundingBox.minZ) * dlg.m_groupMultiplier);
			}//end bounding box modifier
			if(dlg.boundingBoxModifierMirror == TRUE){//bounding box modifier
			    ptrLoc->List_popoutBox.max.x = dlg.m_bndBoxMaxX;
	            ptrLoc->List_popoutBox.max.y = dlg.m_bndBoxMaxY;
	            ptrLoc->List_popoutBox.max.z = dlg.m_bndBoxMaxZ;
	            ptrLoc->List_popoutBox.min.x = dlg.m_bndBoxMinX;
	            ptrLoc->List_popoutBox.min.y = dlg.m_bndBoxMinY;
	            ptrLoc->List_popoutBox.min.z = dlg.m_bndBoxMinZ;
			}//end bounding box modifier
			if(dlg.formatBoxEnable == TRUE){//bounding box modifier
				D3DVECTOR centerObject;
				ZeroMemory(&centerObject,sizeof(D3DVECTOR));
				if(ptrLoc->m_meshObject)
				{
				   centerObject = ptrLoc->m_meshObject->m_meshCenter;
				}
				else if(!ptrLoc->m_node)
				{
				     if(ptrLoc->m_lightObject)
                         centerObject = ptrLoc->m_lightObject->m_wldspacePos;

				}




			    ptrLoc->List_popoutBox.max.x = centerObject.x + dlg.m_reletiveBoxSize;
	            ptrLoc->List_popoutBox.max.y = centerObject.y + dlg.m_reletiveBoxSize;
	            ptrLoc->List_popoutBox.max.z = centerObject.z + dlg.m_reletiveBoxSize;
	            ptrLoc->List_popoutBox.min.x = centerObject.x - dlg.m_reletiveBoxSize;
	            ptrLoc->List_popoutBox.min.y = centerObject.y - dlg.m_reletiveBoxSize;
	            ptrLoc->List_popoutBox.min.z = centerObject.z - dlg.m_reletiveBoxSize;
			}//end bounding box modifier
			if(dlg.wrapModifier == TRUE){
               ptrLoc->m_meshObject->List_materialObject->m_textureTileType = dlg.wrapType;
			}
			if(dlg.filterCollisionModEnable == TRUE){
               ptrLoc->m_collisionInfoFilter = dlg.filterCollisionOn;
			}
			if(dlg.m_generateTangents == TRUE)
			{
               ptrLoc->m_meshObject->m_abVertexArray.GenerateTangentsPerfect(m_gStatus->m_dxRenderer->m_pd3dDevice,FALSE,0);
			   ptrLoc->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
			}
	  }//end filter only group
	 }//end world object loop
	  }//end applicate
#endif
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNPARTEditSystem() 
{
#ifndef _ClientOutput
 int runtimeIndex = m_particleDBDlgBar.SendDlgItemMessage(IDC_LIST_PART_Database,LB_GETCURSEL ,0,0);
 
 if(runtimeIndex == -1)
	{
	MessageBox("Must select a particle system");
	return;
	}

 POSITION posLoc = m_gStatus->m_particleDatabase->FindIndex(runtimeIndex);
 if(!posLoc)
     return;
    
 EnvParticleSystem * ptrLoc = ( EnvParticleSystem* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);

 CParticleInterfaceDlg * particleDlg = new CParticleInterfaceDlg();
 particleDlg->m_gStatusMem = m_gStatus;
 particleDlg->Create(IDD_DIALOG_ParticleInterface,NULL);//
 particleDlg->ShowWindow(SW_SHOW);

 
 
#endif
}



//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNPARTAddParticle() 
{
#ifndef _ClientOutput
CAddParticleSystemDLG dlg;
dlg.m_fileName         = "PICKME";
dlg.m_particlelifeTime = 1000; 
dlg.SortOn             = TRUE;
dlg.m_particleRed      = 1.0f;
dlg.m_particleGreen    = 1.0f;     
dlg.m_particleBlue     = 1.0f;  
dlg.m_fieldRadius      = 2.0f;
dlg.SphereType         = FALSE;  
dlg.m_startHieght      = 0.0f;
dlg.m_fadeStart        = 0;
dlg.m_fadeEnd          = 1000;
dlg.m_scaleStart       = 0;
dlg.m_scaleEnd         = 0;  
dlg.EndSpawn           = TRUE;
dlg.m_textureRef       = 5;  
dlg.m_initialSpeedX    = 0.0f;
dlg.m_initialSpeedY    = 0.0f;
dlg.m_initialSpeedZ    = 0.0f; 
dlg.m_particleCount    = 50;
dlg.BillboardOn        = TRUE;
dlg.IsDirectional      = FALSE;
dlg.FadeTime           = FALSE;
dlg.m_directpartSpeed  = 0;
dlg.m_directpartSpread = 0;
dlg.m_BSpeed		   = 0;
dlg.m_BSpeedDRate	   = 0;
dlg.m_BSpread		   = 0;
dlg.m_BSpreadDRate	   = 0;
dlg.Regenerate         = TRUE;
dlg.VaryLifetime       = TRUE;

   if(dlg.DoModal()==IDOK){
	  EnvParticleSystem * partPtr = NULL;
      AddParticleSystem
		  (
		  dlg.m_fileName,
  		  dlg.m_meshScale,
		  dlg.m_particlelifeTime,
		  dlg.SortOn,
		  dlg.m_particleRed,
		  dlg.m_particleGreen,
		  dlg.m_particleBlue,
		  dlg.m_fieldRadius,
		  dlg.SphereType,
		  dlg.m_startHieght,
		  dlg.m_fadeStart,
		  dlg.m_fadeEnd,
		  dlg.m_scaleStart,
		  dlg.m_scaleEnd,
		  dlg.EndSpawn,
		  dlg.m_textureRef,
		  dlg.m_initialSpeedX,
		  dlg.m_initialSpeedY,
		  dlg.m_initialSpeedZ,
		  dlg.m_directpartSpeed,
		  dlg.m_directpartSpread,
		  dlg.m_BSpeed,
		  dlg.m_BSpeedDRate,
		  dlg.m_BSpread,
		  dlg.m_BSpreadDRate,
		  dlg.IsDirectional,
		  dlg.FadeTime,
		  dlg.Regenerate,
		  dlg.VaryLifetime,
		  dlg.m_particleCount,
		  dlg.BillboardOn,
		  &partPtr
		  );
      
      m_gStatus->m_particleDatabase->AddTail(partPtr);
	  ParticleDatabaseInList(m_gStatus->m_particleDatabase->GetCount()-1);
   }
#endif
}

/*
void EngineXWin::RefreshEnviorment()
{
#ifndef _ClientOutput 
    if(m_gStatus->m_editorModeOn == TRUE){
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_StartRange,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_fogStart));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_EndRange,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_fogRange));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_Density,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_fogDensity));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogRed,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_fogRed));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogGreen,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_fogGreen));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogBlue,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_fogBlue));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckRed,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_backRed));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckGreen,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_backGreen));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckBlue,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_backBlue));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_sunPosX,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_sunPosition.x));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENVsunPosY,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_sunPosition.y));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENVsunPosZ,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_sunPosition.z));
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_MaxWindStrength,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_maxWindStrength));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_WindDirX,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_windDirectionX));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_WindDirZ,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_windDirectionZ));
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_Gravity,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_gravity));
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_SeaLevel,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_seaLevel));
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_SeaDrag,m_gStatus->m_cabLib.FloatToStingCnv(m_gStatus->m_environmentPtr->m_seaViscusDrag));

	   if(m_gStatus->m_environmentPtr->m_ambientLightOn==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK,BST_UNCHECKED,0);
       
	   if(m_gStatus->m_environmentPtr->m_enableTimeSystem==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK,BST_UNCHECKED,0);
	   
	   if(m_gStatus->m_environmentPtr->m_enableWindRandomizer==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK,BST_UNCHECKED,0);

	   if(m_gStatus->m_environmentPtr->m_sunLightOn==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_enableSun,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_enableSun,BM_SETCHECK,BST_UNCHECKED,0);

	   if(m_gStatus->m_environmentPtr->m_fogOn==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_FogOn,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_FogOn,BM_SETCHECK,BST_UNCHECKED,0);

	}
#endif
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------Spawn Genereator Callback-----------------------------------------//
void EngineXWin::UpgradeFileFormats()
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
char temp[2048] = "";
char bigBuff[2048] = "";  // maximum common dialog buffer size
static char BASED_CODE filter[] = "exg Files|*.exg|All Files (*.*)|*.*||";
	CFileDialog opendialog( TRUE, 0, 0,  OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
	opendialog.m_ofn.hInstance = AfxGetInstanceHandle();
	opendialog.m_ofn.lpstrFile = bigBuff;
	opendialog.m_ofn.nMaxFile = sizeof(bigBuff);
	if ( opendialog.DoModal() == IDOK )
	{
	CString FileNames[500];
	int counter = 0;
	int length = 0;
	int length2 = 0;
	int loop=0;
	for(loop=0;loop<2048;loop++){
		
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
    SetCurrentDirectory(MainDir);
    for(loop=1;loop<counter;loop++){//begin file load loop
	CString filename = FileNames[loop];
	m_gStatus->LoadScene(filename);
	SaveScene(filename);
	}	
	}
	MessageBox("Upgrade Completed without errors");
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------double click world prop-----------------------------------------//
void EngineXWin::WorldObjectProperties()
{
#ifndef _ClientOutput
HWND treeHwnd = NULL;
m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd); 
CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);
int trace = 0;
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){
    
	CWorldObjectPropertiesDlg dlg;
	dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.m_worldGroupListRef = m_gStatus->m_worldGroupList;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.m_wldObjectRef = ptrLoc;
	
	dlg.m_particlelink  = ptrLoc->m_particleLink;
	dlg.m_haloLink      = ptrLoc->m_haloLink;
	dlg.m_lensflareLink = ptrLoc->m_lensFlareType;

	if(ptrLoc->m_meshObject)
    ptrLoc->m_meshObject->GetBox();
	
	dlg.showBoundingBox = ptrLoc->List_showBox;
	if(ptrLoc->m_meshObject)
	{
	   dlg.m_actualBoxSizeMaxX = ptrLoc->m_meshObject->List_boundingBox.maxX;
	   dlg.m_actualBoxSizeMaxY = ptrLoc->m_meshObject->List_boundingBox.maxY;
	   dlg.m_actualBoxSizeMaxZ = ptrLoc->m_meshObject->List_boundingBox.maxZ;
	   dlg.m_actualBoxSizeMinX = ptrLoc->m_meshObject->List_boundingBox.minX;
	   dlg.m_actualBoxSizeMinY = ptrLoc->m_meshObject->List_boundingBox.minY;
	   dlg.m_actualBoxSizeMinZ = ptrLoc->m_meshObject->List_boundingBox.minZ;
	}
    dlg.m_popoutBoxSizeMaxX = ptrLoc->List_popoutBox.max.x;
    dlg.m_popoutBoxSizeMaxY = ptrLoc->List_popoutBox.max.y;
	dlg.m_popoutBoxSizeMaxZ = ptrLoc->List_popoutBox.max.z;
	dlg.m_popoutBoxSizeMinX = ptrLoc->List_popoutBox.min.x;
    dlg.m_popoutBoxSizeMinY = ptrLoc->List_popoutBox.min.y;
	dlg.m_popoutBoxSizeMinZ = ptrLoc->List_popoutBox.min.z;

	//if(ptrLoc->m_meshObject)
	//dlg.m_meshRadius = ptrLoc->m_meshObject->m_meshBoundingSphereRadius;

	//dlg.m_popoutRadius = ptrLoc->List_popoutRadius;
	dlg.attachedToScene = ptrLoc->List_environmentAttached;
	//dlg.m_spawnPoint = ptrLoc->List_usedAsSpawnPoint;
	dlg.removeEnvioronment = ptrLoc->List_removeEnviorment;
	dlg.m_objectGroup = ptrLoc->List_groupNumber;

    if(!ptrLoc->List_attribute){
    ptrLoc->List_attribute = new CAttributeObj(0,0,0,0,0,FALSE,FALSE,FALSE,200,0);
	}

	dlg.autoRetract = ptrLoc->List_attribute->List_autoRetract;
	dlg.vicinityBased = ptrLoc->List_attribute->List_vicinityBased;
	dlg.m_otherAttributeType = ptrLoc->List_attribute->List_otherAttributeType;
	dlg.m_otherAttributeValue = ptrLoc->List_attribute->List_otherAttributeValue;
	dlg.m_otherObjectSelect = ptrLoc->List_attribute->List_otherObjectSelect;
	dlg.m_selfAttributeType = ptrLoc->List_attribute->List_selfAttributeType;
	dlg.m_selfAttributeValue = ptrLoc->List_attribute->List_selfAttributeValue;
     if(dlg.DoModal()==IDOK)
	 {
     
    //   if(ptrLoc->m_meshObject)
	   //{//valid mesh
    //      ptrLoc->m_meshObject->m_meshBoundingSphereRadius = dlg.m_meshRadius;
		  //if(ptrLoc->m_meshObject->m_charona){
			 //ptrLoc->m_meshObject->m_charona->InitializeVis();
			 //ptrLoc->m_meshObject->m_charona->m_visual->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,ptrLoc->m_meshObject->m_charona->m_visual->m_uvCount);
		  //}
	   //}//end valid mesh
	   ptrLoc->List_showBox = dlg.showBoundingBox;
       ptrLoc->List_popoutBox.max.x = dlg.m_popoutBoxSizeMaxX;
       ptrLoc->List_popoutBox.max.y = dlg.m_popoutBoxSizeMaxY;
	   ptrLoc->List_popoutBox.max.z = dlg.m_popoutBoxSizeMaxZ;
	   ptrLoc->List_popoutBox.min.x = dlg.m_popoutBoxSizeMinX;
       ptrLoc->List_popoutBox.min.y = dlg.m_popoutBoxSizeMinY;
	   ptrLoc->List_popoutBox.min.z = dlg.m_popoutBoxSizeMinZ;



	   ptrLoc->List_environmentAttached = dlg.attachedToScene;
	   ptrLoc->List_removeEnviorment = dlg.removeEnvioronment;
	   
	   //ptrLoc->List_popoutRadius = dlg.m_popoutRadius;
	   ptrLoc->List_groupNumber = dlg.m_objectGroup;

	   ptrLoc->List_attribute->List_autoRetract = dlg.autoRetract;
	   ptrLoc->List_attribute->List_vicinityBased = dlg.vicinityBased;
	   ptrLoc->List_attribute->List_otherAttributeType = dlg.m_otherAttributeType;
	   ptrLoc->List_attribute->List_otherAttributeValue = dlg.m_otherAttributeValue;
	   ptrLoc->List_attribute->List_otherObjectSelect = dlg.m_otherObjectSelect;
	   ptrLoc->List_attribute->List_selfAttributeType = dlg.m_selfAttributeType;
       ptrLoc->List_attribute->List_selfAttributeValue = dlg.m_selfAttributeValue;
	   //ptrLoc->List_usedAsSpawnPoint = dlg.m_spawnPoint; 

	   ptrLoc->m_particleLink  = dlg.m_particlelink;
	   ptrLoc->m_haloLink      = dlg.m_haloLink;
	   ptrLoc->m_lensFlareType = dlg.m_lensflareLink;

	   //ObjectTreeViewInlist(ptrLoc->List_groupNumber);
	   for(POSITION posLocal = m_gStatus->m_wldObjList->GetHeadPosition();posLocal != NULL;){//loop through objects
	       CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLocal);
		   if(ptrLoc->List_showBox == TRUE)
			  GL_showBoundingBoxs = TRUE;
	   }//end loop through objects
	   for(POSITION objPos = m_gStatus->m_wldObjList->GetHeadPosition();objPos != NULL;)
	   {//reset particles
			CWldObject *obj = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(objPos);
		  
			if(obj->m_particleEmmiter)
			{
			   obj->m_particleEmmiter->m_Regenerate = FALSE;
			   obj->m_particleEmmiter = 0;
			}
			  
	   }//end reset particles
	   m_gStatus->m_curWldObjPrtList->RemoveAll();
	 }
	return;
	
	}
	trace++;
  }//end Begin world object loop
  MessageBox("Object Not Found");	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------double click world prop-----------------------------------------//
void EngineXWin::OnDblclkObjectTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
    WorldObjectProperties();
	*pResult = 0;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------Spawn Genereator Callback-----------------------------------------//
void EngineXWin::SpawnGeneratorCallback()//ussually utilized by server only
{
if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe() == TRUE && m_gStatus->m_multiplayerObject->m_AIServer == FALSE)
return;//dont spawn sh*t is this is a client
   int generatorIndexTrace = 0;
   if(!m_gStatus->m_spawnGeneratorDB->m_enable)
	   return;

   for(POSITION posLocal = m_gStatus->m_spawnGeneratorDB->GetHeadPosition();posLocal != NULL;)
   {//Spawn Generator loop
      CSpawnObj * spawnGenPtr = (CSpawnObj *)m_gStatus->m_spawnGeneratorDB->GetNext(posLocal);
      if(spawnGenPtr->List_currentOutPut < spawnGenPtr->List_maxOutPutCapacity)
	  {//possibility of spawn
           if(spawnGenPtr->m_aiCfg < 0)
		   {//error
			  CString aiIndexString = m_gStatus->m_cabLib.ConvToString(spawnGenPtr->m_aiCfg);
			  CString spawnGenString = m_gStatus->m_cabLib.ConvToString(generatorIndexTrace);
              MessageBox(operator+("Invalid AI configuration #",aiIndexString));
			  MessageBox(operator+("Spawn Gen Index #",spawnGenString));
			  continue;
		   }//end error



           CAIObj * aiPtr = m_gStatus->m_aiList->GetByIndex(spawnGenPtr->m_aiCfg);
           if(!aiPtr)
		   {//Invalid ai ptr
	          CString aiIndexString = m_gStatus->m_cabLib.ConvToString(spawnGenPtr->m_aiCfg);
			  CString spawnGenString = m_gStatus->m_cabLib.ConvToString(generatorIndexTrace);
              MessageBox(operator+("Invalid AI configuration #",aiIndexString));
			  MessageBox(operator+("Spawn Gen Index #",spawnGenString));
			  continue;
		   }//end invalid ai ptr
		   
		   //add conditions for not spawning
		   D3DMATRIX successMatrice = m_gStatus->m_matrixInit2;//spawnGenPtr->List_spawnMatrix;
		   int currentTreePosition = 0;
		   if((rand()%1000) < spawnGenPtr->List_possibilityOfSpawn){//conditions for not spawning
			   if(spawnGenPtr->m_treeUse != -1){//tree override in use
                  POSITION wPos = m_gStatus->GL_aiWebDatabase->FindIndex(spawnGenPtr->m_treeUse);
				  if(wPos){//
                     CAIWebObj * wObj = (CAIWebObj *)m_gStatus->GL_aiWebDatabase->GetAt(wPos);
					 POSITION trePos = NULL;
					 if(!wObj->m_web)
					 {//error- not compiled
			            CString spawnGenString = m_gStatus->m_cabLib.ConvToString(generatorIndexTrace);
			            MessageBox(operator+("invalid web->spwn gen #",spawnGenString));
			            continue;
					 }//end error - not compiled
					 
					 
					 
					 if(spawnGenPtr->m_treePosition == -1){//get random tree location
					    int locationAmount = wObj->m_web->GetCount();
                        if(locationAmount > 0){
						   int randomPosition = rand()%locationAmount;
						   trePos = wObj->m_web->FindIndex(randomPosition);
						   currentTreePosition = randomPosition;
						}
					 }//end get random tree location
					 else
					 trePos = wObj->m_web->FindIndex(spawnGenPtr->m_treePosition);
					 if(trePos){//2
                        CAIWebNode * node = (CAIWebNode *)wObj->m_web->GetAt(trePos);
						successMatrice._41 = node->m_position.x;
                        successMatrice._42 = node->m_position.y;
						successMatrice._43 = node->m_position.z;
					 }//end 2
				  }//
			   }//end tree override in use

               CMovementObj * refPtr = NULL;
		       BOOL result = m_gStatus->ActivateMovementEntity(NULL,
			                                        spawnGenPtr->m_aiCfg,
			                                        successMatrice,
		                                            0,
											        0,
											        aiPtr->m_entityCfgDBindexRef,//spawnGenPtr->List_spawnIndex,
											        5,//hardcode ai to player-spawnGenPtr->List_spawnType,
											        0,
											        0,
											        FALSE,
											        FALSE,
											        FALSE,
											        generatorIndexTrace,
											        -1,
											        &refPtr,
					                                NULL,
													-1,
													-1,
													m_gStatus->m_collisionBase,
													m_gStatus->m_collisionDatabase,
													m_gStatus->m_entObjRuntime,
													m_gStatus->m_currentWorkingFile);
		       if(result){//valid spawn
		        if(refPtr->m_aiObj){//if AI
		           refPtr->m_aiObj->m_routeCurTarget = currentTreePosition;
		           refPtr->m_aiObj->m_mapCurLocation = currentTreePosition;
                   refPtr->m_aiObj->m_curMap         = spawnGenPtr->m_treeUse;
           
                   if(refPtr->m_aiObj->m_physicsModuleOverride)
				   {//Movment ovverride valid
		              if(refPtr->List_movementDB)
					  {
                         refPtr->List_movementDB->SafeDelete();
			             delete refPtr->List_movementDB;
			             refPtr->List_movementDB=0;
					  }
                     refPtr->m_aiObj->m_physicsModuleOverride->Clone(&refPtr->List_movementDB);
			         refPtr->List_movementDB->List_lastPosition.x = successMatrice._41;
                     refPtr->List_movementDB->List_lastPosition.y = successMatrice._42;
                     refPtr->List_movementDB->List_lastPosition.z = successMatrice._43;
				   }//end Movment ovverride valid
		           //ArmNextWeapon(refPtr,TRUE);
				}
           
		        spawnGenPtr->List_currentOutPut++;
			   }//valid spawn
		       else{
                 AfxMessageBox("SpawnGen Failed");
			     AfxMessageBox("spawnGenPtr->List_spawnName");
			   }
		   }//end conditions for not spawning
	  }//end possibility of spawn
   generatorIndexTrace++;
   }//end Spawn Generator loop
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------draw Box----------------------------------------------------------//
void EngineXWin::DrawBox(LPDIRECT3DDEVICE9 pd3dDevice, 
						 D3DVECTOR max, 
						 D3DVECTOR min,
						 D3DVECTOR offset)
{
	/*D3DMATRIX newMatrix = m_gStatus->m_matrixInit2;
	
	newMatrix._41 = newMatrix._41 - offset.x;
    newMatrix._42 = newMatrix._42 - offset.y;
    newMatrix._43 = newMatrix._43 - offset.z;
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetTransform( D3DTS_WORLD , &newMatrix ); 
    D3DCOLORVALUE EmmisiveColor;EmmisiveColor.r = 1.0f;EmmisiveColor.g = 1.0f;EmmisiveColor.b = 1.0f;EmmisiveColor.a = 1.0f;
	GlobalStateMaterial.Emissive = EmmisiveColor;
    D3DCOLORVALUE DiffuseColor;DiffuseColor.r = 0.0f;DiffuseColor.g = 0.0f;DiffuseColor.b = 0.0f;DiffuseColor.a = 1.0f;
	GlobalStateMaterial.Diffuse = DiffuseColor;
    D3DCOLORVALUE SpecularColor;SpecularColor.r = 0.0f;SpecularColor.g = 0.0f;SpecularColor.b = 0.0f;
	GlobalStateMaterial.Specular = SpecularColor; 
	GlobalStateMaterial.Power = 0.0f;
	GlobalStateMaterial.Ambient = DiffuseColor;
	
	if(max.z < min.z) {
		float tmp;
		tmp   = max.z;
		max.z = min.z;
		min.z = tmp;
	}

	D3DVERTEX BoxVerts[24];
	D3DVECTOR v;
	v.x = 0.0f;
	v.y = 0.0f;
	v.z = -1.0f;
 
	//Draw top of box
	BoxVerts[0] = D3DVERTEX(D3DXVECTOR3(max.x,max.y,max.z),v,0,0);
	BoxVerts[2] = D3DVERTEX(D3DXVECTOR3(max.x,max.y,min.z),v,0,0);
	BoxVerts[4] = D3DVERTEX(D3DXVECTOR3(min.x,max.y,min.z),v,0,0);
	BoxVerts[6] = D3DVERTEX(D3DXVECTOR3(min.x,max.y,max.z),v,0,0);
	
	BoxVerts[1] = BoxVerts[2];
	BoxVerts[3] = BoxVerts[4];
	BoxVerts[5] = BoxVerts[6];
	BoxVerts[7] = BoxVerts[0];

	//Draw bottom of box
	BoxVerts[ 8] = D3DVERTEX(D3DVECTOR(max.x,min.y,max.z),v,0,0);
	BoxVerts[10] = D3DVERTEX(D3DVECTOR(max.x,min.y,min.z),v,0,0);
	BoxVerts[12] = D3DVERTEX(D3DVECTOR(min.x,min.y,min.z),v,0,0);
	BoxVerts[14] = D3DVERTEX(D3DVECTOR(min.x,min.y,max.z),v,0,0);
	
	BoxVerts[ 9] = BoxVerts[10];
	BoxVerts[11] = BoxVerts[12];
	BoxVerts[13] = BoxVerts[14];
	BoxVerts[15] = BoxVerts[8];

	//Connect top and bottom
	BoxVerts[16] = BoxVerts[8];
	BoxVerts[17] = BoxVerts[0];

	BoxVerts[18] = BoxVerts[10];
	BoxVerts[19] = BoxVerts[ 2];
	
	BoxVerts[20] = BoxVerts[12];
	BoxVerts[21] = BoxVerts[ 4];

	BoxVerts[22] = BoxVerts[14];
	BoxVerts[23] = BoxVerts[ 6];
    
	
	//draw box  
	m_gStatus->m_renderStateObject->SetTextureState(-1,0,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_dxRenderer->m_pd3dDevice,-1);
	m_gStatus->m_dxRenderer->m_pd3dDevice->SetMaterial( &GlobalStateMaterial );
    
    m_gStatus->m_dxRenderer->m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, BoxVerts, 24, NULL);
    //draw dir rep
	D3DVERTEX BoxDirX[2];
    //Draw top of box
	BoxDirX[0] = BoxVerts[0];
	BoxDirX[1] = BoxVerts[10];
	EmmisiveColor;EmmisiveColor.r = 1.0f;EmmisiveColor.g = 0.0f;EmmisiveColor.b = 0.0f;EmmisiveColor.a = 1.0f;
	GlobalStateMaterial.emissive = EmmisiveColor;
	m_gStatus->m_dxRenderer->m_pd3dDevice->SetMaterial( &GlobalStateMaterial );
	m_gStatus->m_dxRenderer->m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, BoxDirX, 2, NULL);
	
    BoxDirX[0] = BoxVerts[6];
	BoxDirX[1] = BoxVerts[8];
	EmmisiveColor;EmmisiveColor.r = 0.0f;EmmisiveColor.g = 1.0f;EmmisiveColor.b = 0.0f;EmmisiveColor.a = 1.0f;
	GlobalStateMaterial.emissive = EmmisiveColor;
	m_gStatus->m_dxRenderer->m_pd3dDevice->SetMaterial( &GlobalStateMaterial );
	m_gStatus->m_dxRenderer->m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, BoxDirX, 2, NULL);

	BoxDirX[0] = BoxVerts[0];
	BoxDirX[1] = BoxVerts[4];
	EmmisiveColor;EmmisiveColor.r = 0.0f;EmmisiveColor.g = 0.0f;EmmisiveColor.b = 1.0f;EmmisiveColor.a = 1.0f;
	GlobalStateMaterial.emissive = EmmisiveColor;
	m_gStatus->m_dxRenderer->m_pd3dDevice->SetMaterial( &GlobalStateMaterial );
	m_gStatus->m_dxRenderer->m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, BoxDirX, 2, NULL);*/

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------draw Box----------------------------------------------------------//
void EngineXWin::OnBTNUIMerge() 
{

	BOOL aiMergeMode = FALSE;

	CMergeOptionsDlg dlg;
	if(dlg.DoModal()==IDOK)
	   aiMergeMode = dlg.m_aiMergeType;
//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization 
	static char BASED_CODE filter[] = "EXG Files|*.EXG|All Files (*.*)|*.*||";
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
	 CBackGroundObjList * dumpGround;
	 CEnvironmentObj *envDump;
	 CPersistObjList * persistDump;
	 CTextureDBObjList * textureDump;
	 CEXScriptObjList * scriptDump;
	 CSpawnObjList * spawnDump;
	 CCollisionObj * collisionDump;
	 CWldGroupObjList * wldListDump;
	 CAIWebObjList * aiWebDatabaseDump;
	 CReferenceObjList * referenceDatabase = NULL;
	 int intDump;
	 CString stringDump;
     //end preliminary wrapper
	 CWldObjectList * patchList = NULL;
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> patchList 
		            >> dumpGround 
					>> envDump
					>> persistDump
					>> intDump
					>> stringDump
					>> intDump
				    >> intDump
					>> scriptDump
					>> stringDump
					>> spawnDump
					>> collisionDump
                    >> wldListDump
					>> aiWebDatabaseDump
					>> referenceDatabase;

	 /*the_in_Archive >> m_gStatus->m_wldObjList 
		            >> m_gStatus->m_backGroundList 
					>> m_gStatus->m_environmentPtr
					>> GL_ScenePersistList
					>> m_gStatus->m_currentTrack
					>> m_gStatus->m_musicLoopFile
					>> reservedInt
				    >> m_gStatus->W_BufferPreffered
					>> m_gStatus->m_sceneScriptExec
					>> reserved
					>> m_gStatus->m_spawnGeneratorDB
					>> m_gStatus->m_collisionBase
                    >> m_gStatus->m_worldGroupList
					>> m_gStatus->GL_aiWebDatabase
					>> m_gStatus->m_libraryReferenceDB
	                >> m_gStatus->m_waterZoneDB;*/


	 int offsetRefLib = m_gStatus->m_libraryReferenceDB->GetCount();

	 if(!aiMergeMode)
	 if(dlg.m_groupMode > -1)
	 {
        //world objects
	    for(POSITION posLoc = patchList->GetHeadPosition();posLoc != NULL;){
	        CWldObject * patchPtr = (CWldObject *)patchList->GetNext(posLoc);
		    if(dlg.m_groupMode != patchPtr->List_groupNumber)
			continue;
			
			patchPtr->List_groupNumber = dlg.m_transitionGroupID;
			patchPtr->List_identity = m_gStatus->GlobalTraceNumber++;

		    m_gStatus->m_wldObjList->AddTail(patchPtr);
	    }

        return;
	 }
     
	 if(!aiMergeMode)
	 if(referenceDatabase)
	 for(POSITION rPos = referenceDatabase->GetHeadPosition();rPos != NULL;)
	 {
         CReferenceObj * rPtr = (CReferenceObj *)referenceDatabase->GetNext(rPos);
         m_gStatus->m_libraryReferenceDB->AddTail(rPtr);
	 }


	 int webPreCount = m_gStatus->GL_aiWebDatabase->GetCount();

	 if(aiMergeMode)
	 if(aiWebDatabaseDump)
	 for(POSITION posLoc = aiWebDatabaseDump->GetHeadPosition();posLoc != NULL;)
	 {
        CAIWebObj * webObj = (CAIWebObj *)aiWebDatabaseDump->GetNext(posLoc);
        m_gStatus->GL_aiWebDatabase->AddTail(webObj);
	 }

	 
     
	 //spawnDump
	 if(aiMergeMode)
	 if(spawnDump)
	 for(POSITION rPos = spawnDump->GetHeadPosition();rPos != NULL;)
	 {
         CSpawnObj * ptrLoc = ( CSpawnObj* ) spawnDump->GetNext(rPos);
         ptrLoc->m_treeUse = ptrLoc->m_treeUse + webPreCount;
		 m_gStatus->m_spawnGeneratorDB->AddTail(ptrLoc);
	 }

	 WorldGroupInList(0);
	 m_gStatus->SpawnGenDatabaseInList(0);

	 if(this->m_pAISystemDlg)
		this->m_pAISystemDlg->AITreeInList(m_gStatus->GL_aiWebDatabase->GetCount()-1);

	 if(aiMergeMode)
	    return;
	 
     //get newest group number
	 int newGroup = 0;
	 POSITION wPos;
	 for(wPos = m_gStatus->m_wldObjList->GetHeadPosition();wPos != NULL;)
	 {
         CWldObject * wPtr = (CWldObject *)m_gStatus->m_wldObjList->GetNext(wPos);
         if(wPtr->List_groupNumber >= newGroup)
			newGroup = wPtr->List_groupNumber+1;
	 }

	 for(wPos = patchList->GetHeadPosition();wPos != NULL;){
	     CWldObject * patchPtr = (CWldObject *)patchList->GetNext(wPos);
	     if(patchPtr->List_groupNumber >= newGroup)
			newGroup = patchPtr->List_groupNumber+1;
	 }
     //end get newest group number
     //groups
     for(POSITION gPos = wldListDump->GetHeadPosition();gPos != NULL;)
	 {
         CWldGroupObj * gPtr = (CWldGroupObj *)wldListDump->GetNext(gPos);
		 BOOL addIt = TRUE;
		 for(POSITION wPos = m_gStatus->m_worldGroupList->GetHeadPosition();wPos != NULL;)
		 {
            CWldGroupObj * wPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetNext(wPos);
            //if(wPtr->List_groupInt == gPtr->List_groupInt)
            //addIt = FALSE;
			if(wPtr->List_groupName == gPtr->List_groupName)
			{
               addIt = FALSE;

			}
		 }
		 if(addIt){//add unknow group
		    CWldGroupObj * newPtr = new CWldGroupObj();
			int originalGroup      = gPtr->List_groupInt;
		    newPtr->List_groupInt  = newGroup;
		    newPtr->List_groupName = gPtr->List_groupName;
			//update Patch List
			for(POSITION patchPos = patchList->GetHeadPosition();patchPos != NULL;){
	            CWldObject * patchPtr = (CWldObject *)patchList->GetNext(patchPos);
		        if(patchPtr->List_groupNumber == originalGroup)
                   patchPtr->List_groupNumber = newGroup;
			}
			//end update Patch List
			newGroup++;
            m_gStatus->m_worldGroupList->AddTail(newPtr);
		 }//end add unknown group
	 }
	 //world objects
	 for(POSITION posLoc = patchList->GetHeadPosition();posLoc != NULL;){
	     CWldObject * patchPtr = (CWldObject *)patchList->GetNext(posLoc);
		 patchPtr->List_identity = m_gStatus->GlobalTraceNumber++;
	     
		 if(aiMergeMode)
         if(patchPtr->m_node)
			patchPtr->m_node->m_libraryReference += offsetRefLib;

		 m_gStatus->m_wldObjList->AddTail(patchPtr);
	 }
	 m_gStatus->InitializeOjectsIntoScene();
	 m_gStatus->RebuildTraceNumber();//rebuild trace numbers
	 m_gStatus->ReinitAllTexturesFromDatabase();
	 
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(MainDir); 
	 }
     
  return;	
}
//-----------------------------------------------------------------------------
// Name: CreatePlane()
// Desc: Helper function to get a plane equation from 3 points in space.
//-----------------------------------------------------------------------------
//plane, pt1, pt2, vEyePt
FLOAT* EngineXWin::MirrCreatePlane( FLOAT *plane, D3DXVECTOR3& a, 
								    D3DXVECTOR3& b, D3DXVECTOR3& c )
{
	D3DXVECTOR3 n = m_gStatus->m_matLib.Normalize( m_gStatus->m_matLib.CrossProduct( c-b, c-a ) );
    plane[0] = n.x;
	plane[1] = n.y;
	plane[2] = n.z; 
    plane[3] = -( n.x*c.x + n.y*c.y + n.z*c.z );

    return plane;
}

//-----------------------------------------------------------------------------
// Name: OrderVerticies()
// Desc: Orders verticies on a 4 pt. mesh according to texture coordinates
//-----------------------------------------------------------------------------
void EngineXWin::OrderVerticies(CEXMeshObj *mesh, ABVERTEX *vOut)
{
	/*int vFound[4];
	ZeroMemory(vFound, sizeof(int) * 4);
	
	ABVERTEX *tmp = mesh->m_abVertexArray.m_origVertexArray;

	for(int lpr=0; lpr < 4; lpr++) {

		int tu,tv;

		if(mesh->m_abVertexArray.m_origVertexArray[lpr].tx0.tu >= 0.5f) tu=1;
		else tu=0;
		if(mesh->m_abVertexArray.m_origVertexArray[lpr].tx0.tv >= 0.5f) tv=1;
		else tv=0;

		if(tu == 0) {

			if(tv == 0) {
				vOut[0] = tmp[lpr];
				vFound[0]    = 1;
			}
			else if(tv == 1) {
				vOut[2] = tmp[lpr];
				vFound[2]    = 1;
			}

		}
		else if(tu == 1) {
			
			if(tv == 0) {
				vOut[1] = tmp[lpr];
				vFound[1]    = 1;
			}
			else if(tv == 1) {
				vOut[3] = tmp[lpr];
				vFound[3]    = 1;
			}

		}
		
	}

	//if( (vFound[0] != 1) && (vFound[1] != 1) && (vFound[2] != 1) && (vFound[3] != 1) )
	//	MessageBox("Verticies not found:  possible incorrect texture coordinates");*/
}

//-----------------------------------------------------------------------------
// Name: GetOffset()
// Desc: Gets the x,y,z offset from the origin to the center of a 4pt. mesh
//----------------------------------------------------------------------------- 
D3DVECTOR EngineXWin::GetOffset(ABVERTEX *vIn)
{
	D3DVECTOR vRet;
	ZeroMemory(&vRet, sizeof(D3DVECTOR));

	float top, left, leftdepth;

	top        = vIn[1].x - vIn[0].x;
	left       = vIn[0].y - vIn[2].y;
	leftdepth  = vIn[0].z - vIn[2].z;

	vRet.x = vIn[1].x-(top/2.0f);
	vRet.y = vIn[0].y-(left/2.0f);
	vRet.z = vIn[0].z-(leftdepth/2.0f);
	
	return vRet;
}
//------------------------------------------------------------------------------------//
void EngineXWin::ReorderObjectsBasedOnTextureIndex()
{
int countBegin = m_gStatus->m_wldObjList->GetCount();
BOOL done=FALSE;
CWldObjectList * newDB = new CWldObjectList();

 int textureCount = m_gStatus->m_textureDataBase->GetCount();
 int wldObjCount  = m_gStatus->m_wldObjList->GetCount();

 for(int i=-1;i<textureCount;i++)
 {//1
	 POSITION posLast;
     for(POSITION posLocal = m_gStatus->m_wldObjList->GetHeadPosition();(posLast = posLocal) != NULL;)
	 {//loop through objects
         CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLocal);
		   CEXMeshObj * pMeshRef = ptrLoc->m_meshObject;
	       if(pMeshRef == NULL){//probably a ref object
		      CReferenceObj * referenceObject = m_gStatus->m_libraryReferenceDB->GetByIndex(ptrLoc->m_node->m_libraryReference);
		      if(!referenceObject)
		          continue;
		          pMeshRef = referenceObject->m_meshObject;
	   
	
		      if(i == pMeshRef->List_textureSelect[0])
			  {
			    newDB->AddTail(ptrLoc);
		        m_gStatus->m_wldObjList->RemoveAt(posLast);
			  }
		   }//end probably a ref object
	 }//end loop through objects
 }//1

 if(m_gStatus->m_wldObjList->GetCount() != 0)
 {
	 POSITION posLast;
     for(POSITION posLocal = newDB->GetHeadPosition();(posLast = posLocal) != NULL;)
	 {//loop through objects
         CWldObject * ptrLoc = ( CWldObject* ) newDB->GetNext(posLocal);
         m_gStatus->m_wldObjList->AddTail(ptrLoc);
	 }
	 newDB->RemoveAll();
	 delete newDB;
	 newDB = 0;
	 
	 MessageBox("Invalid world object count");
 }
 else{
	 POSITION posLast;
     for(POSITION posLocal = newDB->GetHeadPosition();(posLast = posLocal) != NULL;)
	 {//loop through objects
         CWldObject * ptrLoc = ( CWldObject* ) newDB->GetNext(posLocal);
         m_gStatus->m_wldObjList->AddTail(ptrLoc);
	 }
	 newDB->RemoveAll();
	 delete newDB;
	 newDB = 0;

    MessageBox("Completed Flawlessly");
 }

 if(m_gStatus->m_wldObjList->GetCount() != wldObjCount)
 {
    MessageBox("Major miscount error do not save, quit");


 }
 else
    MessageBox("Object counts verify correctly");


}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOptimizeStates() 
{   
if(AfxMessageBox("This May take awhile-will optimize states and you will have to rebuild collision map",
			  MB_OKCANCEL,
			  0)==IDOK)
ReorderObjectsBasedOnTextureIndex();	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPNewAccount() 
{
     
   
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::InListAccounts(int sel)
{
  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPDeleteAccount() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOBJAddTrigger() 
{
#ifndef _ClientOutput
HWND treeHwnd = NULL;
m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd); 
CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);
int trace = 0;
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){
        CTriggerEditorDlg dlg;
		dlg.GL_worldObject = ptrLoc;
		if(dlg.DoModal()==IDOK){
           ptrLoc = dlg.GL_worldObject;
		}
	   return;
	}
  } 
  MessageBox("Must select a trace number of a object");
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOBJUpdateObject() 
{
HWND treeHwnd = NULL;
m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd); 
int indexOfObject = GetTreeChildIndex(treeHwnd,item);	
HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,ObjectRoot,10);
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = "TRC_"+m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity);
	if(tracename == textLoc){
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
	      CString fileName = opendialog.GetFileName();
          ptrLoc->m_meshObject->SafeDelete();
		  delete ptrLoc->m_meshObject;
		  ptrLoc->m_meshObject=0;
		  ptrLoc->m_meshObject = new CEXMeshObj();
		  ptrLoc->m_meshObject->List_materialObject = new CMaterialObject();
		  float range = 0.0f;
		  ptrLoc->m_meshObject->LoadGeometryFromPlugin(fileName,&range,m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
		  
		  
		  m_gStatus->AddTextureToDatabase(ptrLoc->m_meshObject);//apply mapping and add to DB
		  ptrLoc->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
		 }//111111111111111111111
		 SetCurrentDirectory(MainDir);
	return;
	}
  }
   MessageBox("Must select a visual");	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL EngineXWin::ReplaceObjectByName(CString objectNameBasis,CString path) 
{
  CString cmp1 = objectNameBasis;
  cmp1.MakeUpper();
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;)
  {//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString cmp2 = ptrLoc->m_fileNameRef;
	cmp2.MakeUpper();
	if(cmp1 == cmp2)
	{//compare names

		  CEXMeshObj * newMesh = new CEXMeshObj();
		  newMesh->List_materialObject = new CMaterialObject();
		  float range = 0.0f;
		  
		  if(newMesh->LoadGeometryFromPlugin(path,&range,m_gStatus->m_dxRenderer->m_pd3dDevice,NULL))
		  {
             ptrLoc->m_meshObject->SafeDelete();
		     delete ptrLoc->m_meshObject;
		     ptrLoc->m_meshObject=0;
             ptrLoc->m_meshObject = newMesh;
			 m_gStatus->AddTextureToDatabase(ptrLoc->m_meshObject);//apply mapping and add to DB
		     ptrLoc->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
			 return TRUE;
		  }
          newMesh->SafeDelete();
		  delete newMesh;
		  newMesh = 0;
	}//end compare names
  }//End world object loop
  return FALSE;
}

//------------------------------------------------------------------------------------//
void EngineXWin::DumpTempDatabases()
{
m_gStatus->m_worldRenderList->RemoveAll();
m_gStatus->m_worldTriggerList->RemoveAll();
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnCHECKTCPRegistered() 
{

	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnSelchangeLISTTCPIPCurrentPlayers() 
{
  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPServerLoad() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPServerSave() 
{

}
//------------------------------------------------------------------------------------//
/*
BOOL EngineXWin::SaveTextureBank(CString fileName,int compressed,int maxTextureSize)
{
    if(m_gStatus->m_dynamicTextureDataBase->m_compressRef == 1)
      m_gStatus->m_dynamicTextureDataBase->UpdateLiveDataToBaseData();
	
	m_gStatus->m_dynamicTextureDataBase->ExportCompressedTXB(fileName,m_gStatus->m_dxRenderer->m_pd3dDevice,compressed,maxTextureSize,compressed);

   return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTXSaveTextureBank() 
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
	static char BASED_CODE filter[] = "TXB ENGIN|*.TXB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "EXG", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SaveTextureBank(fileName,0,2048);
	 }
    SetCurrentDirectory(MainDir);
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*
void EngineXWin::OnBTNTXLoadTextureBank() 
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
	static char BASED_CODE filter[] = "TXB Files|*.TXB|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadTextureBank(fileName);
	   InListTextureDB(0);
	   if(m_gStatus->m_libraryReferenceDB)
	      m_gStatus->m_libraryReferenceDB->ClearInstances();
	  
	 }
	SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::TextureRebuildWithSearch(int mipLevels)
{
  //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir,"\\MapsModels\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization  
  for(POSITION texPositionLoc = m_gStatus->m_textureDataBase->GetHeadPosition();texPositionLoc != NULL;)
  {
    CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetNext(texPositionLoc);
    CString conv = txtPtrLoc->List_textureName;
	CTextureEX * newPtr = new CTextureEX();
	if(newPtr->LoadIt(conv,
		              mipLevels,
					  m_gStatus->m_dxRenderer->m_pd3dDevice,
					  FALSE)==TRUE){//success
	   newPtr->SafeDelete();
	   delete newPtr;
	   newPtr=0;
	   txtPtrLoc->m_textureObject->SafeDelete();
	   delete txtPtrLoc->m_textureObject;
	   txtPtrLoc->m_textureObject = 0;
	   txtPtrLoc->m_textureObject = new CTextureEX();
	   if(txtPtrLoc->m_textureObject->LoadIt(conv,
		                                     mipLevels,
											 m_gStatus->m_dxRenderer->m_pd3dDevice,
											 FALSE)==TRUE){
	 
	   }
	}//end success
  }
  m_gStatus->ReinitAllTexturesFromDatabase();
  SetCurrentDirectory(MainDir);
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------add url--------------------------------------------------------------//
/*
void EngineXWin::OnBtnTxtdbAddUrl()
{
    #ifndef _ClientOutput
	//attemp multiselect
    //working
	      int mipLevels = 10;
		  CString loadContruction = "http://www.endlessagesportal.com/images/EndlessTitleRes.jpg";
          char charbuf[256];
	      lstrcpy(charbuf,loadContruction.GetBuffer());
		  CTextureDBObj * newPtr = new CTextureDBObj(charbuf);
          newPtr->m_textureObject = new CTextureEX();
	      if(newPtr->m_textureObject->LoadItFromURL(loadContruction,
													mipLevels,
													m_gStatus->m_dxRenderer->m_pd3dDevice,
													0,
													true,
													m_gStatus->m_advUrlBasic,
													m_gStatus->m_advUrlVending)==TRUE)
		  {//success
             newPtr->m_textureObject->Restore(m_gStatus->m_dxRenderer->m_pd3dDevice,
											  0,
											  newPtr->m_bump,
											  newPtr->m_bumpType,
											  m_gStatus->m_dxRenderer->m_maxTextureSizeSquared,
											  m_gStatus->m_renderStateObject->m_pCaps,
											  m_gStatus->m_compressTextures,
											  m_gStatus->m_advUrlBasic,
										      m_gStatus->m_advUrlVending);   
	         m_gStatus->m_textureDataBase->AddTail(newPtr);
             InListTextureDB(m_gStatus->m_textureDataBase->GetCount()-1);
		  }//end success
	      else{
			  MessageBox("failed to load:"+loadContruction);
			  delete newPtr;
			  newPtr = 0;
		  }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------add texture--------------------------------------------------------------//
void EngineXWin::OnBtnTxtdbAdd() 
{
#ifndef _ClientOutput
	//attemp multiselect
    //working
    CAddTextureDlg dlg;
	dlg.m_mipLevels = 1;
	dlg.fileCounter = 0;
	if(dlg.DoModal()==IDOK){
	   
       
	   for(int loop=0;loop<dlg.fileCounter;loop++){//load all files
          if(dlg.fileCounter != 1 && loop == 0)
			 loop++;

		  CString loadContruction;
		  
             loadContruction = dlg.FileNames[0]+"\\"+dlg.FileNames[loop];

          char charbuf[256];
	      lstrcpy(charbuf,dlg.FileNames[loop]);
		  CTextureDBObj * newPtr = new CTextureDBObj(charbuf);
          newPtr->m_textureObject = new CTextureEX();
	      if(newPtr->m_textureObject->LoadIt(loadContruction,
			                                 dlg.m_mipLevels,
											 m_gStatus->m_dxRenderer->m_pd3dDevice,
											 FALSE)==TRUE){//success
	         newPtr->m_textureObject->Restore(m_gStatus->m_dxRenderer->m_pd3dDevice,
											  0,
											  newPtr->m_bump,
											  newPtr->m_bumpType,
											  m_gStatus->m_dxRenderer->m_maxTextureSizeSquared,
											  m_gStatus->m_renderStateObject->m_pCaps,
											  m_gStatus->m_compressTextures,
											  m_gStatus->m_advUrlBasic,
										      m_gStatus->m_advUrlVending);
	         m_gStatus->m_textureDataBase->AddTail(newPtr);
             InListTextureDB(m_gStatus->m_textureDataBase->GetCount()-1);
		  }//end success
	      else{
          MessageBox("failed");
		  }
	   }

	   m_gStatus->m_textureDataBase->CompileDirectAccess();
	}
#endif
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMNUAnimProperties() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMNUDeleteAnimation() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOBJGetSignificant() 
{
#ifndef _ClientOutput
GetSignificantOptionsDlg dlg;
  if(dlg.DoModal()==IDOK)
  {
     if(dlg.m_getInverseOnly)
      ObjectTreeViewInlist(-9998,0);	
	 else
	  ObjectTreeViewInlist(-9999,0);	

  
  }
#endif
}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::LightmapSearchAndload() 
{

}
//-----------------------------End Function-------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOBJTextureSetMacro() 
{
	//If they don't have a group selected, return
	int menuIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box, LB_GETCURSEL, 0, 0);
	if(menuIndex == -1)
	{
		MessageBox("You must select a group first.");
		return;
	}

	LoadRNEFile();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------draw Box----------------------------------------------------------//
void EngineXWin::DrawSimpleBox(LPDIRECT3DDEVICE9 pd3dDevice, 
						       D3DVECTOR max, 
						       D3DVECTOR min,
						       D3DVECTOR offset)
{
	/*D3DMATRIX newMatrix = m_gStatus->m_matrixInit2;
	
	newMatrix._41 = newMatrix._41 - offset.x;
    newMatrix._42 = newMatrix._42 - offset.y;
    newMatrix._43 = newMatrix._43 - offset.z;
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &newMatrix ); 
    D3DCOLORVALUE EmmisiveColor;EmmisiveColor.r = 1.0f;EmmisiveColor.g = 1.0f;EmmisiveColor.b = 1.0f;EmmisiveColor.a = 1.0f;
	GlobalStateMaterial.Emissive = EmmisiveColor;
    D3DCOLORVALUE DiffuseColor;DiffuseColor.r = 0.0f;DiffuseColor.g = 0.0f;DiffuseColor.b = 0.0f;DiffuseColor.a = 1.0f;
	GlobalStateMaterial.Diffuse = DiffuseColor;
    D3DCOLORVALUE SpecularColor;SpecularColor.r = 0.0f;SpecularColor.g = 0.0f;SpecularColor.b = 0.0f;
	GlobalStateMaterial.Specular = SpecularColor; 
	GlobalStateMaterial.Power = 0.0f;
	
	if(max.z < min.z) {
		float tmp;
		tmp   = max.z;
		max.z = min.z;
		min.z = tmp;
	}

	D3DVERTEX BoxVerts[24];
	D3DVECTOR v = D3DVECTOR(0,0,-1);
 
	//Draw top of box
	BoxVerts[0] = D3DVERTEX(D3DVECTOR(max.x,max.y,max.z),v,0,0);
	BoxVerts[2] = D3DVERTEX(D3DVECTOR(max.x,max.y,min.z),v,0,0);
	BoxVerts[4] = D3DVERTEX(D3DVECTOR(min.x,max.y,min.z),v,0,0);
	BoxVerts[6] = D3DVERTEX(D3DVECTOR(min.x,max.y,max.z),v,0,0);
	
	BoxVerts[1] = BoxVerts[2];
	BoxVerts[3] = BoxVerts[4];
	BoxVerts[5] = BoxVerts[6];
	BoxVerts[7] = BoxVerts[0];

	//Draw bottom of box
	BoxVerts[ 8] = D3DVERTEX(D3DVECTOR(max.x,min.y,max.z),v,0,0);
	BoxVerts[10] = D3DVERTEX(D3DVECTOR(max.x,min.y,min.z),v,0,0);
	BoxVerts[12] = D3DVERTEX(D3DVECTOR(min.x,min.y,min.z),v,0,0);
	BoxVerts[14] = D3DVERTEX(D3DVECTOR(min.x,min.y,max.z),v,0,0);
	
	BoxVerts[ 9] = BoxVerts[10];
	BoxVerts[11] = BoxVerts[12];
	BoxVerts[13] = BoxVerts[14];
	BoxVerts[15] = BoxVerts[8];

	//Connect top and bottom
	BoxVerts[16] = BoxVerts[8];
	BoxVerts[17] = BoxVerts[0];

	BoxVerts[18] = BoxVerts[10];
	BoxVerts[19] = BoxVerts[ 2];
	
	BoxVerts[20] = BoxVerts[12];
	BoxVerts[21] = BoxVerts[ 4];

	BoxVerts[22] = BoxVerts[14];
	BoxVerts[23] = BoxVerts[ 6];
    
	
	//draw box  
	m_gStatus->m_renderStateObject->SetTextureState(-1,0,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_dxRenderer->m_pd3dDevice,-1);
	m_gStatus->m_dxRenderer->m_pd3dDevice->SetMaterial( &GlobalStateMaterial );
    
    m_gStatus->m_dxRenderer->m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, BoxVerts, 24, NULL);*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOBJEditLOD() 
{
#ifndef _ClientOutput
  HWND treeHwnd = NULL;
  m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
  HTREEITEM item = TreeView_GetSelection(treeHwnd); 
  int indexOfObject = GetTreeChildIndex(treeHwnd,item);	
  HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
  CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,ObjectRoot,10);
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){
       int lodIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_OBJ_LODdata,LB_GETCURSEL ,0,0);
	     if(lodIndex == -1){
            MessageBox("must select a LOD level");
	        return;
		 }

		 POSITION lPos = ptrLoc->m_lodVisualList->FindIndex(lodIndex);
		 if(!lPos)
			 return;
		     CMeshLODObject * mLod = (CMeshLODObject *)ptrLoc->m_lodVisualList->GetAt(lPos);

             //mLod->m_meshLodLevel->GetGeometryFromPlugin


			 CLodSystemDlg dlg;
			 dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;

			 dlg.m_maxX = mLod->m_boxOption.maxX;
			 dlg.m_maxY = mLod->m_boxOption.maxY;
			 dlg.m_maxZ = mLod->m_boxOption.maxZ;
			 dlg.m_minX = mLod->m_boxOption.minX;
		     dlg.m_minY = mLod->m_boxOption.minY;
			 dlg.m_minZ = mLod->m_boxOption.minZ;

			 dlg.m_lodType = ptrLoc->m_lodVisualList->m_lodType;

		

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
					  mLod->m_meshLodLevel->List_materialObject = new CMaterialObject();
                      BOOL result = mLod->m_meshLodLevel->LoadGeometryFromPlugin(dlg.m_fileName,&sightRange,m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
					  mLod->m_meshLodLevel->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
					}//end change mesh

                    ABBOX trnsferBx;
			        mLod->m_boxOption.maxX = dlg.m_maxX;
			        mLod->m_boxOption.maxY = dlg.m_maxY;
			        mLod->m_boxOption.maxZ = dlg.m_maxZ;
			        mLod->m_boxOption.minX = dlg.m_minX;
			        mLod->m_boxOption.minY = dlg.m_minY;
			        mLod->m_boxOption.minZ = dlg.m_minZ;

					mLod->m_distance = dlg.m_activation;

					ptrLoc->m_lodVisualList->m_lodType = dlg.m_lodType;
					
             
				  ptrLoc->UpdateLods();
			      VisualPropertiesInList();
			  }
	     return;
	}
  }//end Begin world object loop
  MessageBox("Must Select a visual");
#endif
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------draw Box----------------------------------------------------------//
void EngineXWin::DrawSegment(LPDIRECT3DDEVICE9 pd3dDevice, 
						     D3DVECTOR max, 
						     D3DVECTOR min,
						     D3DVECTOR offset)
{
	/*D3DMATRIX newMatrix = m_gStatus->m_matrixInit2;
	
	newMatrix._41 = newMatrix._41 - offset.x;
    newMatrix._42 = newMatrix._42 - offset.y;
    newMatrix._43 = newMatrix._43 - offset.z;
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &newMatrix ); 
    D3DCOLORVALUE EmmisiveColor;EmmisiveColor.r = 1.0f;EmmisiveColor.g = 1.0f;EmmisiveColor.b = 1.0f;EmmisiveColor.a = 1.0f;
	GlobalStateMaterial.emissive = EmmisiveColor;
    D3DCOLORVALUE DiffuseColor;DiffuseColor.r = 0.0f;DiffuseColor.g = 0.0f;DiffuseColor.b = 0.0f;DiffuseColor.a = 1.0f;
	GlobalStateMaterial.diffuse = DiffuseColor;
    D3DCOLORVALUE SpecularColor;SpecularColor.r = 0.0f;SpecularColor.g = 0.0f;SpecularColor.b = 0.0f;
	GlobalStateMaterial.specular = SpecularColor; 
	GlobalStateMaterial.power = 0.0f;
	

	D3DVERTEX BoxVerts[5];
	D3DVECTOR v = D3DVECTOR(0,0,-1);
 
	//Draw top of box
	BoxVerts[0] = D3DVERTEX(D3DVECTOR(max.x,max.y,max.z),v,0,0);
	BoxVerts[1] = D3DVERTEX(D3DVECTOR(min.x,min.y,min.z),v,0,0);
	
	//draw box  
    m_gStatus->m_renderStateObject->SetTextureState(-1,0,
		                                  m_gStatus->m_dxRenderer->m_pd3dDevice,
				                          m_gStatus->m_textureDataBase);


	m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_dxRenderer->m_pd3dDevice,-1);
	m_gStatus->m_dxRenderer->m_pd3dDevice->SetMaterial( &GlobalStateMaterial );
    //draw dir rep
	D3DVERTEX BoxDirX[2];
    //Draw top of box
	BoxDirX[0] = BoxVerts[0];
	BoxDirX[1] = BoxVerts[1];
	EmmisiveColor;EmmisiveColor.r = 1.0f;EmmisiveColor.g = 0.0f;EmmisiveColor.b = 0.0f;EmmisiveColor.a = 1.0f;
	GlobalStateMaterial.emissive = EmmisiveColor;
	m_gStatus->m_dxRenderer->m_pd3dDevice->SetMaterial( &GlobalStateMaterial );
	m_gStatus->m_dxRenderer->m_pd3dDevice->DrawPrimitive(D3DPT_LINELIST, D3DFVF_VERTEX, BoxDirX, 2, NULL);*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOBJRemoveLODLevel() 
{
	HWND treeHwnd = NULL;
	m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
	HTREEITEM item = TreeView_GetSelection(treeHwnd); 
	
	int indexOfObject = GetTreeChildIndex(treeHwnd,item);	
	HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
	CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,ObjectRoot,10);
	
	for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;)
	{//Begin world object loop
		CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
		CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
		if(tracename == textLoc)
		{
			int lodIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_OBJ_LODdata,LB_GETCURSEL ,0,0);
			if(lodIndex == -1)
			{
				MessageBox("must select a LOD level");
				return;
			}

			if(!ptrLoc->m_lodVisualList)
				return;

			if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
				return;
			
			ptrLoc->m_lodVisualList->RemoveLod(lodIndex);
			if(ptrLoc->m_lodVisualList->GetCount() == 0)
			{
				ptrLoc->m_lodVisualList->SafeDelete();
				delete ptrLoc->m_lodVisualList;
				ptrLoc->m_lodVisualList = 0;
			}
			VisualPropertiesInList();
			//end do it
			return;
		}
	}//end Begin world object loop
	MessageBox("Must Select a visual");	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBtnObjLodadd() 
{
#ifndef _ClientOutput
  HWND treeHwnd = NULL;
  m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
  HTREEITEM item = TreeView_GetSelection(treeHwnd); 
  int indexOfObject = GetTreeChildIndex(treeHwnd,item);	
  HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
  CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,ObjectRoot,10);
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc){
         CLodSystemDlg dlg;
		 dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;

		 if(!ptrLoc->m_lodVisualList)
				 ptrLoc->m_lodVisualList = new CMeshLODObjectList();

		 dlg.m_lodType = ptrLoc->m_lodVisualList->m_lodType;
	     if(dlg.DoModal()==IDOK){
             //ptrContent->List_imMesh->AddLODLevel(dlg.m_fileName,dlg.m_activation);
			 

             ABBOX trnsferBx;
			 trnsferBx.maxX = dlg.m_maxX;
			 trnsferBx.maxY = dlg.m_maxY;
			 trnsferBx.maxZ = dlg.m_maxZ;
			 trnsferBx.minX = dlg.m_minX;
			 trnsferBx.minY = dlg.m_minY;
			 trnsferBx.minZ = dlg.m_minZ;

               //MessageBox("AD");
             if(dlg.m_lodType == 0)
			 ptrLoc->m_lodVisualList->AddLOD(0,NULL,dlg.m_activation,dlg.m_fileName,ptrLoc->m_meshObject->List_materialObject,m_gStatus->m_dxRenderer->m_pd3dDevice);
			 else
			 ptrLoc->m_lodVisualList->AddLOD(1,&trnsferBx,dlg.m_activation,dlg.m_fileName,ptrLoc->m_meshObject->List_materialObject,m_gStatus->m_dxRenderer->m_pd3dDevice);
			 ptrLoc->m_lodVisualList->Initialize(m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_cgGlobalShaderDB);
			 //MessageBox("AD2");
			 ptrLoc->UpdateLods();
			 if(ptrLoc->m_lodVisualList->GetCount()==0){
                ptrLoc->m_lodVisualList->SafeDelete();
				delete ptrLoc->m_lodVisualList;
                ptrLoc->m_lodVisualList=0;
			 }

			 VisualPropertiesInList();
		 }
	   //end do it
	return;
	}
  }//end Begin world object loop
  MessageBox("Must Select a visual");	
#endif
}
//----------------------------------------------------------//
D3DVECTOR EngineXWin::TransformPt(D3DMATRIX transform, D3DVECTOR pt)
{
	D3DVECTOR newPt;
	ZeroMemory(&newPt, sizeof(D3DVECTOR));

	newPt.x = 
		transform.m[0][0]*pt.x +
		transform.m[1][0]*pt.y +
		transform.m[2][0]*pt.z +
		transform.m[3][0];
	newPt.y = 
		transform.m[0][1]*pt.x +
		transform.m[1][1]*pt.y +
		transform.m[2][1]*pt.z +
		transform.m[3][1];
	newPt.z = 
		transform.m[0][2]*pt.x +
		transform.m[1][2]*pt.y +
		transform.m[2][2]*pt.z +
		transform.m[3][2];

	return newPt;

}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::AddTextureFromSearchPath(CString path,CEXMeshObj * mesh)
{
              CString CurDir;
              CString MainDir;
              char buffer[_MAX_PATH];
			  _getcwd( buffer, _MAX_PATH );
			  MainDir = CurDir = buffer;
			  CurDir = operator+(CurDir,path);
			  SetCurrentDirectory(CurDir);
			  m_gStatus->AddTextureToDatabase(mesh);
			  SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTITYAnimations() 
{
#ifndef _LimitedEngine
#ifndef _ClientOutput

    //HRSRC hResource = ::FindResource(NULL, (LPCSTR)IDD_DIALOG_SkeletalAnimationEditor, RT_DIALOG);

	HWND treeHwnd = NULL;
    m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd); 
	int indexOfObject = GetTreeChildIndex(treeHwnd,item);	
	HTREEITEM ObjectRoot = TreeView_GetParent(treeHwnd, item);
    CString textLoc = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,ObjectRoot,10);
    for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;)
	{//Begin entity object loop
	  CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);	
	  CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->m_uniqueId)));
	  if(tracename == textLoc){
	     CSkeletalAnimationDlg dlg;
		 //make sure textures have been initialized
         if(m_gStatus->m_entObjList->m_texturesHaveBeenInitialized == 0)
		   if(AfxMessageBox("Textures for this interface has not been initialized yet, this may take a moment.",MB_OKCANCEL,0)==IDOK)
		   {
			  for(POSITION posEnt = m_gStatus->m_entObjList->GetHeadPosition();posEnt != NULL;){
	              CMovementObj * ptrEnt = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posEnt);	
				  if(ptrEnt->List_skeletalObject)
					 m_gStatus->SkeletalTextureReassignment(ptrEnt->List_skeletalObject);
			  }
			  m_gStatus->m_entObjList->m_texturesHaveBeenInitialized = 1;
			  m_gStatus->m_textureDataBase->CompileDirectAccess();
		   }
		 //end make sure textures have been initialized
		 dlg.m_renderStateObjectRef = m_gStatus->m_renderStateObject;
		 dlg.m_globalShaderRef = m_gStatus->m_cgGlobalShaderDB;
		 dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
		 dlg.m_skeletonReferencePtr = ptrLoc->List_skeletalObject;
		 dlg.directSoundRef = m_gStatus->m_dirSnd;
		 dlg.m_elementDBREF = m_gStatus->m_elementDB;
		 dlg.m_skillDatabaseREF = m_gStatus->m_skillDatabase;
		 dlg.DoModal();
	  return;
	}
	}//end Begin world object loop
    MessageBox("Select a skeleton Object...");	
#endif
#endif
}
//------------------------------------------------------------------------------------//
int EngineXWin::BonesSystemRenderSkeleton(CSkeletonObject * sklPtr,
										  D3DVECTOR offsetVector,
										  D3DMATRIX positionMatrix)
{
    if(sklPtr->m_bonesList){
         for(POSITION posLoc = sklPtr->m_bonesList->GetHeadPosition();posLoc != NULL;){
             CBoneObject * bone = (CBoneObject *)sklPtr->m_bonesList->GetNext(posLoc);
			 
			 
			 D3DVECTOR posBonePos,parentPos;
			 D3DMATRIX matrixTransform,newAppend;
			 
			 bone->m_boneFrame->GetTransform(NULL,&matrixTransform);
			 m_gStatus->m_matLib.fastMatrixMultiply(matrixTransform,matrixTransform,positionMatrix);
			 

			 posBonePos.x = matrixTransform._41;
			 posBonePos.y = matrixTransform._42;
			 posBonePos.z = matrixTransform._43;
			 //test render bone visual
			 D3DVECTOR max,min;
			 max.x = posBonePos.x +.1f;
			 max.y = posBonePos.y +.1f;
			 max.z = posBonePos.z +.1f;
			 min.x = posBonePos.x -.1f;
			 min.y = posBonePos.y -.1f;
			 min.z = posBonePos.z -.1f;
			 DrawSimpleBox(m_gStatus->m_dxRenderer->m_pd3dDevice,
				           max,
					       min,
				           offsetVector);
			 //segment
			 if(bone->m_boneFrame->List_parentFrame){
                //bone->m_boneFrame->List_parentFrame->GetPosition(NULL,&parentPos);
				bone->m_boneFrame->List_parentFrame->GetTransform(NULL,&matrixTransform);
			    m_gStatus->m_matLib.fastMatrixMultiply(matrixTransform,matrixTransform,positionMatrix);
				parentPos.x = matrixTransform._41;
			    parentPos.y = matrixTransform._42;
			    parentPos.z = matrixTransform._43;
                D3DVECTOR one,two;
				one.x = 0.0f;
				one.y = 0.0f;
				one.z = 0.0f;
				two.x = 0.0f;
				two.y = 5.0f;
				two.z = 0.0f;
				
				DrawSegment(m_gStatus->m_dxRenderer->m_pd3dDevice, 
						    posBonePos, 
						    parentPos,
						    offsetVector);
			 }
		 }
		     
		}
   return 1;
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNAIDelete() 
{
if(!m_gStatus->m_aiList)
	return;
	
if(m_gStatus->m_editorModeOn==FALSE)
return;
int runtimeIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_DB,LB_GETCURSEL ,0,0);
  if(runtimeIndex == -1){
     MessageBox("Must Select a bot");
     return;
  }
  POSITION posLoc = m_gStatus->m_aiList->FindIndex(runtimeIndex);
    if(!posLoc)
       return;
       CAIObj * partPtr = (CAIObj *)m_gStatus->m_aiList->GetAt(posLoc);
	   partPtr->SafeDelete();
	   delete partPtr;
	   partPtr=0;
	   m_gStatus->m_aiList->RemoveAt(posLoc);
	   AI_DBInList(runtimeIndex-1);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Pathfinding check version 1-Used in AI--------------------------------//
BOOL EngineXWin::AI_Roam(CMovementObj * ptrLoc,CAIObj * AIPtr)
{
	BOOL proceed = FALSE;
	//if(AIPtr->m_status != 0 && AIPtr->m_getSonarInfo == FALSE)
	//   proceed = TRUE;
	if(ptrLoc->List_AI_reachedDestination == TRUE)
       proceed = TRUE; 
	
	if(proceed == TRUE){
      //get new location to travel to in radius
	  D3DVECTOR locationBuild = m_gStatus->m_cabLib.GetRandomPointInRadius(AIPtr->List_AreaToRoam,ptrLoc->List_AI_origin,0.0f);
	  //application of data
      ptrLoc->List_AI_reachedDestination = FALSE;
      ptrLoc->List_AIdestination = locationBuild;
	}

return TRUE;
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::AI_DBInList(int sel)
{
if(!m_gStatus->m_aiList)
	return;
	
if(m_gStatus->m_editorModeOn==FALSE)
return;
int traceIt=0;
    m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_DB,LB_RESETCONTENT,0,0);	
	for(POSITION posLoc = m_gStatus->m_aiList->GetHeadPosition();posLoc != NULL;){
        CAIObj * partPtr = (CAIObj *)m_gStatus->m_aiList->GetNext(posLoc);
        m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_DB,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+(m_gStatus->m_cabLib.ConvToString(traceIt),operator+(" cfg:",partPtr->m_aiCfgName)));
		traceIt++;
	}
	if(sel > -1)
	   m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_DB,LB_SETCURSEL ,sel,0);
}
									 

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------Attack check version 1-Used in AI--------------------------------//
BOOL EngineXWin::AI_UseWeapon(CMovementObj * ptrLoc,CAIObj * AIPtr)
{
 
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Create AI Object------------------------------------------------------//
void EngineXWin::AddAIObject(float     closeEnoughDist,
							 float     turnTolerance,
							 int       sightType,
		                     int       sightRandVariable,
		                     float     maxSightRadius,
		                     int       difficulty,
		                     BOOL      walkingType,
		                     int       startAttackZone,
		                     int       startAttackDist,
                             int       randChillTime,
		                     int       pathfindingMethod,
		                     int       randKamakazi)
{

CAIObj* AIptr = new CAIObj();
//d3drm3->CreateFrame( scene, &AIptr->List_targetFrame);//build temp frame
AIptr->List_targetFrame = new CFrameObj(NULL);
m_gStatus->m_aiList->AddTail(AIptr);
}*/
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
BOOL EngineXWin::ArmEntitySys(CMovementObj * movObj,
							  int itemIndex,
					          CMovementObjList * referenceDatabase,
							  BOOL overrideNetwork)
{
 
   POSITION refDBpos = referenceDatabase->FindIndex(movObj->List_refModel);
   if(!refDBpos)
	   return FALSE;
       CMovementObj * refDBObj = (CMovementObj *)referenceDatabase->GetAt(refDBpos);
       if(!refDBObj->List_armedInventoryDB)//valid inventory list
       return FALSE;

       POSITION armedDBPos = refDBObj->List_armedInventoryDB->FindIndex(itemIndex);
	     if(!armedDBPos)
	     return FALSE;
	        CArmedInventoryObj * armedPos = (CArmedInventoryObj *) refDBObj->List_armedInventoryDB->GetAt(armedDBPos);
            //make sure not already armed or occupancy conflict
			if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
            if(movObj->List_armedInventoryDB)
	           for(POSITION posLoc = movObj->List_armedInventoryDB->GetHeadPosition();posLoc != NULL;)
			   {
                 CArmedInventoryObj * armedPtr = (CArmedInventoryObj *)movObj->List_armedInventoryDB->GetNext(posLoc);
                 if(armedPtr->m_id == itemIndex || armedPtr->m_locationOccupancy == armedPos->m_locationOccupancy)
			        return FALSE;
			   }
            //end make sure not already armed
            CArmedInventoryObj * appendageCloned = NULL;
			armedPos->Clone(&appendageCloned,NULL);
            appendageCloned->m_id = itemIndex;
            if(!movObj->List_armedInventoryDB)
				movObj->List_armedInventoryDB = new CArmedInventoryList();

			if(movObj->List_controlType == 5){//ai custom
				if(movObj->m_aiObj)
				if(appendageCloned->m_functionalityAttribute == 1){//weapon
                   movObj->m_aiObj->m_minAttackDistance = (float)appendageCloned->m_miscInt3;
				}//end weapon		
			}//end ai custom

            movObj->List_armedInventoryDB->AddTail(appendageCloned);

return TRUE;
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
BOOL EngineXWin::DisArmEntitySys(CMovementObj * movObj,
							     int itemIndex,
								 BOOL networkOverride)
{
    if(!movObj->List_armedInventoryDB)
		return FALSE;


	POSITION armedPosition = movObj->List_armedInventoryDB->FindIndex(itemIndex);
	if(!armedPosition)
		return FALSE;
	    CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) movObj->List_armedInventoryDB->GetAt(armedPosition);
        armedPtr->SafeDelete();
		delete armedPtr;
		armedPtr=0;
        movObj->List_armedInventoryDB->RemoveAt(armedPosition);

return TRUE;
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTAppendCustomDB() 
{
#ifndef _ClientOutput
int indexLoc=0;
BOOL foundIt = FALSE; 
CMovementObj * mPtr;
   //get selected index
    POSITION posLoc;
    for(posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp){
           foundIt = TRUE;
		   break;
		}
		indexLoc++;
	}
	if(foundIt == FALSE){
       MessageBox("Must select a entity base object");
	   return;
	}
	//get database object
    posLoc = m_gStatus->m_entObjList->FindIndex(indexLoc);
    if(!posLoc)
       return;
    CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetAt(posLoc);
    CAppendageCustDlg dlg;
	dlg.m_gStatusRef = m_gStatus;
	dlg.m_renderStateObjectRef = m_gStatus->m_renderStateObject;
	dlg.m_cgGlobalShaderDB = m_gStatus->m_cgGlobalShaderDB;
	dlg.m_skillDatabaseREF = m_gStatus->m_skillDatabase;
	dlg.m_elementDBREF = m_gStatus->m_elementDB;
	dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
    dlg.m_focusMovPtr = ptrLoc;
	dlg.m_dirSndRef = m_gStatus->m_dirSnd;
	dlg.m_armedInvenListRef = m_gStatus->m_itemManager->m_visuals;
	if(m_gStatus->m_itemManager->m_visuals)
    if(m_gStatus->m_itemManager->m_visuals->m_texturesHaveBeenInitialized == 0)
       if(AfxMessageBox("Textures for this interface has not been initialized yet, this may take a moment.",MB_OKCANCEL,0)==IDOK)
	   {
		  for(POSITION armedPosition = m_gStatus->m_itemManager->m_visuals->GetHeadPosition();armedPosition != NULL;)
	      {
              CArmedInventoryObj* armedptr = (CArmedInventoryObj*)m_gStatus->m_itemManager->m_visuals->GetNext(armedPosition);
              if(armedptr->m_firstPersonSystem)
			     m_gStatus->SkeletalTextureReassignment(armedptr->m_firstPersonSystem);
		      if(armedptr->m_thirdPersonSystem)
			     m_gStatus->SkeletalTextureReassignment(armedptr->m_thirdPersonSystem);
	      }
		  m_gStatus->m_itemManager->m_visuals->m_texturesHaveBeenInitialized = 1;
		  m_gStatus->m_textureDataBase->CompileDirectAccess();
	   }

	dlg.DoModal();
	//check for unassigned/added textures
	if(m_gStatus->m_itemManager->m_visuals)
	{
       m_gStatus->m_itemManager->m_visuals->CompileDirectAccess();
	}
    InListEntityAppendages();
#endif
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::SkeletalTextureAdd(CSkeletonObject * skeletonObj)
{
   if(skeletonObj->m_deformableMeshArrayV2)
   for(int loop=0;loop<skeletonObj->m_deformableMeshArrayCount;loop++){
       for(POSITION posLoc = skeletonObj->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
	   {
           CDeformableVisObj * visPtr = (CDeformableVisObj *)skeletonObj->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
           m_gStatus->AddTextureToDatabase(visPtr->m_mesh);
	   }
   }
   //hiarch bone objects
   for(POSITION posLocal = skeletonObj->m_bonesList->GetHeadPosition();posLocal != NULL;){
	   CBoneObject * bonePtr = (CBoneObject *)skeletonObj->m_bonesList->GetNext(posLocal);
       int indexaTracel = 0;
	   //Hiarchy mesh
	   if(bonePtr->m_hiarchyMesh){
          for(POSITION posOni = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();posOni != NULL;)
		  {
             CHiarcleVisObj * hObj = (CHiarcleVisObj *) bonePtr->m_hiarchyMesh->m_lodChain->GetNext(posOni);
		     m_gStatus->AddTextureToDatabase(hObj->m_mesh);
		  }
	   }
   }  
}
//-----------------------------------------------//
//-----------------------------------------------//

//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::InListEntityAppendages()
{
	m_entityDlgBar.SendDlgItemMessage(IDC_LIST_Appendages,LB_RESETCONTENT,0,0);
	int entIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
	   return;
	}
    POSITION posloc = m_gStatus->m_entObjRuntime->FindIndex(entIndex);
	if(!posloc)
		return;
        CMovementObj * movObj = (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(posloc);
		
		posloc = m_gStatus->m_entObjList->FindIndex(movObj->List_refModel);
        if(!posloc)
			return;
		    CMovementObj * movDBPtr = (CMovementObj *)m_gStatus->m_entObjList->GetAt(posloc);
		
		
		
		if(movDBPtr->List_armedInventoryDB)
			for(posloc = movDBPtr->List_armedInventoryDB->GetHeadPosition();posloc != NULL;)
			{
                CArmedInventoryObj * armedObj = (CArmedInventoryObj *)movDBPtr->List_armedInventoryDB->GetNext(posloc);      
				m_entityDlgBar.SendDlgItemMessage(IDC_LIST_Appendages,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+(">",armedObj->m_name));
			}
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnSelchangeLISTENTITYRuntimeList() 
{
     InListEntityAppendages();
	 InListEntityDefcfgs(0);
	 int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList, LB_GETCURSEL, 0, 0);
	 g_editorHighlightEntityIndex = runtimeIndex;
	 if(runtimeIndex != -1 && m_gStatus && m_gStatus->m_entObjRuntime)
	 {
		 POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
		 if(posLoc)
		 {
			 CMovementObj * ptrLoc = (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(posLoc);
			 if(ptrLoc && ptrLoc->List_baseFrame)
			 {
				 D3DVECTOR entPos;
				 ptrLoc->List_baseFrame->GetPosition(NULL, &entPos);
				 CString sx, sy, sz;
				 sx.Format("%.1f", entPos.x);
				 sy.Format("%.1f", entPos.y);
				 sz.Format("%.1f", entPos.z);
				 m_entityDlgBar.SetDlgItemText(IDC_EDIT_quickMoveX, sx);
				 m_entityDlgBar.SetDlgItemText(IDC_EDIT_quickMoveY, sy);
				 m_entityDlgBar.SetDlgItemText(IDC_EDIT_quickMoveZ, sz);
			 }
		 }
		 UpdateEditorObjectHighlight(m_gStatus, FALSE, TRUE);
	 }
}
//-----------------------------------------------//
// Double click runtime entity: possess/take control of it in editor
//-----------------------------------------------//
void EngineXWin::OnDblclkLISTENTITYRuntimeList()
{
	int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	if(runtimeIndex == -1) return;
	POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
	if(!posLoc) return;
	CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
	if(!ptrLoc) return;

	ptrLoc->List_controlType = 0; // Take player user control
	if(ptrLoc->List_energy <= 0)
		ptrLoc->List_energy = ptrLoc->List_maxEnergy > 0 ? ptrLoc->List_maxEnergy : 100;

	m_gStatus->m_cameraModule->m_followTrace = ptrLoc->List_traceNumber;
	m_gStatus->m_globalAvVar = TRUE;

	// Capture input and switch to 3D walk mode
	if(m_gStatus->m_dxRenderer)
		m_gStatus->m_dxRenderer->DisableMouse();
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTAppendArm()
{
	 CItemManagerDlg * iManageDlg = new CItemManagerDlg();
	 iManageDlg->m_selectedEntity = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	 iManageDlg->globalStatusMem = m_gStatus;
	 iManageDlg->Create(IDD_DIALOG_ItemInterface,NULL);
	 iManageDlg->ShowWindow(SW_SHOW);
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTAppendDisarm() 
{
int entIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
	   MessageBox("Must also select a runtime entity");
	   return;
	}
	int appendageIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_Appendages,LB_GETCURSEL ,0,0);
	if(appendageIndex == -1){
	   MessageBox("Must also select a appendage");
	   return;
	}
	//m_gStatus->m_entObjList
    POSITION posloc = m_gStatus->m_entObjRuntime->FindIndex(entIndex);
	if(!posloc)
		return;
        CMovementObj * movObj = (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(posloc);	
		if(!movObj->List_armedInventoryDB)
			return;

	posloc = m_gStatus->m_entObjList->FindIndex(entIndex);
	if(!posloc)
		return;
        CMovementObj * dbObj = (CMovementObj *)m_gStatus->m_entObjList->GetAt(posloc);	

		if(!dbObj->List_armedInventoryDB)
			return;

	posloc = dbObj->List_armedInventoryDB->FindIndex(appendageIndex);
	if(!posloc)
		return;
        CArmedInventoryObj * armedObj = (CArmedInventoryObj *)dbObj->List_armedInventoryDB->GetAt(posloc);	
		int index = movObj->List_armedInventoryDB->GetIndexByID(armedObj->m_locationOccupancy);
		if(index == -1)   
		MessageBox("Item is not armed");
		else
		DisArmEntitySys(movObj,
						index,
						FALSE);
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTLODSystems() 
{
#ifndef _ClientOutput
int indexLoc=0;
BOOL foundIt = FALSE; 
CMovementObj * mPtr;
	CEntityLODSystemsDlg dlg;
	POSITION posLoc;
    for(posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp){
           foundIt = TRUE;
		   break;
		}
		indexLoc++;
	}
	if(foundIt == FALSE){
       MessageBox("Must select a entity base object");
	   return;
	}
	//get database object
    posLoc = m_gStatus->m_entObjList->FindIndex(indexLoc);
    if(!posLoc)
       return;
    CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetAt(posLoc);	
	dlg.m_lodRangeRefList = ptrLoc->List_skeletonLODList;
	if(dlg.DoModal()==IDOK){
	  ptrLoc->List_skeletonLODList = dlg.m_lodRangeRefList;
      if(ptrLoc->List_skeletonLODList){
	   if(ptrLoc->List_skeletonLODList->GetCount() == 0){
          ptrLoc->List_skeletonLODList->SafeDelete();
		  delete ptrLoc->List_skeletonLODList;
		  ptrLoc->List_skeletonLODList = NULL;
		  ptrLoc->m_currentLOD = 0;
	   }
	  }
	}
	else
	  ptrLoc->List_skeletonLODList = dlg.m_lodRangeRefList;
#endif
}

//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTResourceBase() 
{
#ifndef _ClientOutput
 int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	   if(runtimeIndex == -1){
        MessageBox("Must select an entity object");
		return;
	   }
	   POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
	   if(!posLoc)
		   return;
	       CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
		   m_gStatus->m_cameraModule->m_followTrace = ptrLoc->List_traceNumber;
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//

void EngineXWin::OnBTNAddHalo()
{

}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//

void EngineXWin::OnBTNDeleteHalo()
{

}

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//

void EngineXWin::OnBTNEditHalo()
{

}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
void EngineXWin::UpdateHaloListbox()
{

}

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//

void EngineXWin::OnBTNAddLensFlare()
{

}

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//

void EngineXWin::OnBTNEditLensFlare()
{

}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//

void EngineXWin::OnBTNDeleteLensFlare()
{

}

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//

void EngineXWin::OnBTNHaloLoad()
{

}

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//

void EngineXWin::OnBTNHaloSave()
{

}

void EngineXWin::OnBTNGenerateMesh()
{
#ifndef _ClientOutput 
	GenerateMeshDLG gm;

	gm.DoModal();
#endif
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------Add Missile to Missile database-----------------------------------------------//
/*void EngineXWin::OnBTNMISAddMissile() 
{ 
	CMissileObj * newMissilePtr = new CMissileObj();
	newMissilePtr->m_typeOfProjectile = 0;//custom mesh
       //load type and mesh data
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
	             CString filenamePathandAll = opendialog.GetPathName();
		           //load imediate mode mesh data
                   newMissilePtr->m_basicMesh = new CEXMeshObj();
			       newMissilePtr->m_basicMesh->List_materialObject = new CMaterialObject();
                   float sightRange = 0.0f;
                   BOOL result = newMissilePtr->m_basicMesh->LoadGeometryFromPlugin(filenamePathandAll,&sightRange,m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
			       
			 
				   if(result == FALSE)
			          m_gStatus->m_systemReadout->AddLine(operator+("Mesh Initialization Failed:", filenamePathandAll));
				   else
					  m_gStatus->m_systemReadout->AddLine(operator+("Mesh Initialization Success:", filenamePathandAll));
			 
				m_gStatus->AddTextureToDatabase(newMissilePtr->m_basicMesh);//apply mapping and add to DB
			    SetCurrentDirectory(MainDir);
			 }
			 else{
				newMissilePtr->SafeDelete();
				delete newMissilePtr;
				newMissilePtr=0;

	            SetCurrentDirectory(MainDir);
			    return;
			 }       
     //end 
     CString conversion;
     newMissilePtr->m_lifeTime = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
	 newMissilePtr->m_minDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
	 newMissilePtr->m_maxDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
	 newMissilePtr->m_dotRefValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_dotRef, NULL, TRUE);
	 newMissilePtr->m_particleLink = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
	 newMissilePtr->m_explosionLink  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
	 newMissilePtr->m_abilityType  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
	 newMissilePtr->m_skillType    = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);
	 
	 CSkillObject * skillPtr = m_gStatus->m_skillDatabase->GetSkillByIndex(m_missileDlgBar.SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_GETCURSEL ,0,0)-1);
	 
	 if(skillPtr)
	 newMissilePtr->m_criteriaForSkillUse = skillPtr->m_skillType;
	 else
	 newMissilePtr->m_criteriaForSkillUse = -1;
	 
	 //newMissilePtr->m_criteriaForSkillUse = m_missileDlgBar.SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_GETCURSEL ,0,0)-1;
	 
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_MissileName,newMissilePtr->m_missileName);
	 
     m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
     newMissilePtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);

	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_pushPowerF,conversion);
     newMissilePtr->m_pushPower = (float)atof((LPCTSTR)conversion);
 
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
     newMissilePtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);
     
     if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_forceExplodeAtEnd = TRUE;
	 else
	   newMissilePtr->m_forceExplodeAtEnd = FALSE;

	 if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_filterCollision = TRUE;
	 else
	   newMissilePtr->m_filterCollision = FALSE;



     newMissilePtr->m_physicDynamics = new CMovementCaps();

	 m_gStatus->m_missileDB->AddTail(newMissilePtr);
	 MissileDatabaseInList(m_gStatus->m_missileDB->GetCount()-1);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::MissileDatabaseInList(int sel)
{
   if(!m_gStatus->m_editorModeOn)
	   return;

   m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_RESETCONTENT,0,0);
   int trace = 0;
   for(POSITION posLoc = m_gStatus->m_missileDB->GetHeadPosition();posLoc != NULL;)
   {
       CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetNext(posLoc);
	   CString build = m_gStatus->m_cabLib.ConvToString(trace)+" ";
	   build = build+misPtr->m_missileName;
       m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,
		                                  LB_ADDSTRING,0,
										 (LPARAM)(LPCTSTR)build);
	   trace++;
   }
   if(sel > -1)
      m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_SETCURSEL ,sel,0);


   InListMissileSkillCriteria(0);
   InListMisChannels(0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------delete missile object-----------------------------------------------//
void EngineXWin::OnBTNMISDeleteMissile() 
{
     int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
        MessageBox("Must Select a missile database object");
	    return;
	 }
     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);
	 misPtr->SafeDelete();
	 delete misPtr;
	 misPtr=0;
	 m_gStatus->m_missileDB->RemoveAt(posLoc);
     MissileDatabaseInList(m_gStatus->m_missileDB->GetCount());
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Add Missile Channel-------------------------------------------------//
void EngineXWin::OnBTNAddMissileChannel()
{
	int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL, 0, 0);
     if(missileIndex == -1)
	    return;

     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);

	 if(!misPtr->m_damageChannelList)
		 misPtr->m_damageChannelList = new CElementStatusList();
	 
	 int elementSelect = m_missileDlgBar.SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_GETCURSEL ,0,0);
	 if(elementSelect == -1)
	 {
		MessageBox("Must select an element");
		return;
	 }

	 UpdateData(TRUE);
	 CElementStatusObj * statusPtr = new CElementStatusObj();
	 statusPtr->m_currentValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_damPercentage, NULL, TRUE);
	 statusPtr->m_elementIndex = elementSelect;
	 UpdateData(FALSE);

	 misPtr->m_damageChannelList->AddTail(statusPtr);	
	 InListElementStatus(misPtr->m_damageChannelList->GetCount()-1);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------Remove Missile Channel----------------------------------------------//
void EngineXWin::OnBTNRemoveMissileChannel()
{
	//Get the element Index
	int elementIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_DamageChannelList, LB_GETCURSEL, 0, 0);
	if(elementIndex == -1)
		return;

	//Get the missile Index
	int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL, 0, 0);
     if(missileIndex == -1)
	    return;

     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     
	 CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);

	 POSITION elePos = misPtr->m_damageChannelList->FindIndex(elementIndex);
	 if(!elePos)
		return;

	 CElementStatusObj *statusPtr = (CElementStatusObj *)misPtr->m_damageChannelList->GetAt(elePos);
	 statusPtr->SafeDelete();
	 delete statusPtr;
	 statusPtr = 0;

	 misPtr->m_damageChannelList->RemoveAt(elePos);
	 InListElementStatus(elementIndex - 1);
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------In List of Elements-------------------------------------------------//
/*void EngineXWin::InListElementStatus(int sel)
{
	if(!m_gStatus->m_editorModeOn)
	   return;

	int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1)
	    return;

     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);
	 
	 m_missileDlgBar.SendDlgItemMessage(IDC_LIST_DamageChannelList, LB_RESETCONTENT, 0, 0);

	 if(!misPtr->m_damageChannelList)
		 return;

	for(POSITION posLoc = misPtr->m_damageChannelList->GetHeadPosition(); posLoc !=NULL;)
	{
		CElementStatusObj* elePtr = (CElementStatusObj*)misPtr->m_damageChannelList->GetNext(posLoc);
		CString tempName = "Unknown";
		POSITION elePos = m_gStatus->m_elementDB->FindIndex(elePtr->m_elementIndex);
		 if(elePos)
		 {
           CElementObj * ptr = (CElementObj *)m_gStatus->m_elementDB->GetAt(elePos);
           tempName = ptr->m_elementName;
		 }

		CString build = tempName + " ";
		build = build + m_gStatus->m_cabLib.ConvToString(elePtr->m_currentValue);
		m_missileDlgBar.SendDlgItemMessage(IDC_LIST_DamageChannelList, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)build);
	}
	
	if(sel > -1)
		m_missileDlgBar.SendDlgItemMessage(IDC_LIST_DamageChannelList, LB_SETCURSEL, sel, 0);
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnSelchangeLISTMISmissileDataBase() 
{
	int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
	    return;
	 }
     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);
	 m_missileDlgBar.SetDlgItemText( IDC_EDIT_MIS_MissileName,          misPtr->m_missileName);
     m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_LifeTime,             misPtr->m_lifeTime,TRUE);
	 m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_Damage,               misPtr->m_minDamageValue,TRUE);
	 m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_DamageMax,			misPtr->m_maxDamageValue,TRUE);
	 m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_dotRef,				misPtr->m_dotRefValue,TRUE);
	 m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_ParticleDBlink,       misPtr->m_particleLink,TRUE);
	 m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_ExplosionAttribute,   misPtr->m_explosionLink,TRUE);
     m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_AbilityUseType,       misPtr->m_abilityType,TRUE);
	 m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_SkillUseType,         misPtr->m_skillType,TRUE);
	 m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_DamageSpecificID,     misPtr->m_damageSpecific,TRUE);
	 m_missileDlgBar.SetDlgItemInt(  IDC_EDIT_MIS_kickBaseForce,        (int)misPtr->m_baseKickPower,TRUE);
	 m_missileDlgBar.SetDlgItemText( IDC_EDIT_MIS_CollisionRadius,m_gStatus->m_cabLib.FloatToStingCnv(misPtr->m_collisionRadius));
     m_missileDlgBar.SetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,m_gStatus->m_cabLib.FloatToStingCnv(misPtr->m_heatSeekingPower));
	 m_missileDlgBar.SetDlgItemText( IDC_EDIT_MIS_pushPowerF,m_gStatus->m_cabLib.FloatToStingCnv(misPtr->m_pushPower));

	 m_missileDlgBar.SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_SETCURSEL ,misPtr->m_damageChannel,0);
     m_missileDlgBar.SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_SETCURSEL ,(m_gStatus->m_skillDatabase->GetSkillIndexByType(misPtr->m_criteriaForSkillUse)+1),0);

     if(misPtr->m_forceExplodeAtEnd == TRUE)
	    m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_SETCHECK,BST_CHECKED,0);
     else
	    m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_SETCHECK,BST_UNCHECKED,0);

	 if(misPtr->m_orientateToCamera == TRUE)
		m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FaceCamera,BM_SETCHECK,BST_CHECKED,0);
     else
	    m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FaceCamera,BM_SETCHECK,BST_UNCHECKED,0);
    
     if(misPtr->m_useElastics == TRUE)
		m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_UseElastics,BM_SETCHECK,BST_CHECKED,0);
     else
	    m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_UseElastics,BM_SETCHECK,BST_UNCHECKED,0);

     if(misPtr->m_useFullPhysics == TRUE)
		m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_UseFullPhysics,BM_SETCHECK,BST_CHECKED,0);
     else
	    m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_UseFullPhysics,BM_SETCHECK,BST_UNCHECKED,0);

	 if(misPtr->m_burstForceType == TRUE)
		m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_BurstForceType,BM_SETCHECK,BST_CHECKED,0);
     else
	    m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_BurstForceType,BM_SETCHECK,BST_UNCHECKED,0);

	 if(misPtr->m_filterCollision == TRUE)
		m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_SETCHECK,BST_CHECKED,0);
     else
	    m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_SETCHECK,BST_UNCHECKED,0);

	 InListElementStatus(-1);
}*/

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNMISReplaceValues() 
{
     int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
		MessageBox("Must select a missile database object");
	    return;
	 }
     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
	 CString conversion;
     CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);
     misPtr->m_lifeTime = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
	 misPtr->m_minDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
	 misPtr->m_maxDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
	 misPtr->m_dotRefValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_dotRef,NULL,TRUE);
	 misPtr->m_particleLink = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
	 misPtr->m_explosionLink  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
	 misPtr->m_abilityType  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
	 misPtr->m_skillType    = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);
	 misPtr->m_damageSpecific = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_DamageSpecificID,NULL,TRUE);
     misPtr->m_baseKickPower = (float)m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_kickBaseForce,NULL,TRUE);
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_MissileName,misPtr->m_missileName);


	 CSkillObject * skillPtr = m_gStatus->m_skillDatabase->GetSkillByIndex(m_missileDlgBar.SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_GETCURSEL ,0,0)-1);
	 
	 if(skillPtr)
	 misPtr->m_criteriaForSkillUse = skillPtr->m_skillType;
	 else
	 misPtr->m_criteriaForSkillUse = -1;
	 misPtr->m_damageChannel = m_missileDlgBar.SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_GETCURSEL ,0,0);


     m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
     misPtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);

	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_pushPowerF,conversion);
     misPtr->m_pushPower = (float)atof((LPCTSTR)conversion);
 
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
     misPtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);
     
     if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_forceExplodeAtEnd = TRUE;
	 else
	   misPtr->m_forceExplodeAtEnd = FALSE;

    
	 if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FaceCamera,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_orientateToCamera = TRUE;
	 else
	   misPtr->m_orientateToCamera = FALSE;

	 if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_BurstForceType,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_burstForceType = TRUE;
	 else
	   misPtr->m_burstForceType = FALSE;

	 if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_UseFullPhysics,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_useFullPhysics = TRUE;
	 else
	   misPtr->m_useFullPhysics = FALSE;

	 if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_UseElastics,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_useElastics = TRUE;
	 else
	   misPtr->m_useElastics = FALSE;

     if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
       misPtr->m_filterCollision = TRUE;
	 else
	   misPtr->m_filterCollision = FALSE;
	 
	 
	 MissileDatabaseInList(missileIndex);	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMISEditVisual() 
{
#ifndef _ClientOutput
     int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
		MessageBox("Must select a missile database object");
	    return;
	 }
     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
         CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);

		 if(misPtr->m_typeOfProjectile == 0){//basic missile
		    CMissileBaseMeshEdit dlg;
			dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
			dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
		    dlg.m_missileRef = misPtr;
			dlg.DoModal();
			if(misPtr->m_basicMesh)
		    m_gStatus->AddTextureToDatabase(misPtr->m_basicMesh);
		 }//end basic missile
		 if(misPtr->m_typeOfProjectile == 1){//billboard
		    CMissileAddBillboardDlg dlg;
			dlg.textureDBref = m_gStatus->m_textureDataBase;
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
			 misPtr->m_basicMesh->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
			 if(misPtr->m_basicMesh)
		     m_gStatus->AddTextureToDatabase(misPtr->m_basicMesh);
			}
		 }//end basic missile
         if(misPtr->m_typeOfProjectile == 2){//skeletal
		    CSkeletalAnimationDlg dlg;
			dlg.m_renderStateObjectRef = m_gStatus->m_renderStateObject;
			dlg.m_globalShaderRef = m_gStatus->m_cgGlobalShaderDB;
			dlg.m_elementDBREF = NULL;
			dlg.m_skillDatabaseREF = NULL;
            dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	        dlg.m_skeletonReferencePtr = misPtr->m_skeletalVisual;
            dlg.directSoundRef = m_gStatus->m_dirSnd;
	        dlg.DoModal();
			m_gStatus->SkeletalTextureReassignmentWCompile(misPtr->m_skeletalVisual);
		 }//end //skeletal
		 if(misPtr->m_typeOfProjectile == 3){//laser object
		    CLaserDlg dlg;
	        dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	        dlg.m_laserObjectRef = misPtr->m_laserObject;
	        dlg.DoModal();
			misPtr->m_laserObject->InitializeMesh(m_gStatus->m_dxRenderer->m_pd3dDevice);
		 }//end laser object
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMISAddBillboardMissile() 
{
#ifndef _ClientOutput
	CMissileObj * newMissilePtr = new CMissileObj();
	newMissilePtr->m_typeOfProjectile = 1;//billboard
    newMissilePtr->m_basicMesh = new CEXMeshObj();  

	CMissileAddBillboardDlg dlg;
	dlg.textureDBref = m_gStatus->m_textureDataBase;
	dlg.m_missileRef = NULL;
	if(dlg.DoModal()!=IDOK)
	   return;
	CString textureName = "NONE";
	float billboardSize = dlg.m_billboardSize * .5f;

	if(dlg.m_textureIndexRet >= 0){
	POSITION texPos = m_gStatus->m_textureDataBase->FindIndex(dlg.m_textureIndexRet);
     if(texPos){
	   CTextureDBObj * texPtr = (CTextureDBObj *) m_gStatus->m_textureDataBase->GetAt(texPos);
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
     newMissilePtr->m_basicMesh->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
	 m_gStatus->AddTextureToDatabase(newMissilePtr->m_basicMesh);
     //end 
     CString conversion;
     newMissilePtr->m_lifeTime = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
	 newMissilePtr->m_minDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
	 newMissilePtr->m_maxDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
	 newMissilePtr->m_dotRefValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_dotRef, NULL, TRUE);
	 newMissilePtr->m_particleLink = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
	 newMissilePtr->m_explosionLink  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
	 newMissilePtr->m_abilityType  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
	 newMissilePtr->m_skillType    = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_MissileName,newMissilePtr->m_missileName);
	 
     m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
     newMissilePtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);
 
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
     newMissilePtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);
     
     if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_forceExplodeAtEnd = TRUE;
	 else
	   newMissilePtr->m_forceExplodeAtEnd = FALSE;

	 if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_filterCollision = TRUE;
	 else
	   newMissilePtr->m_filterCollision = FALSE;

     newMissilePtr->m_physicDynamics = new CMovementCaps();

	 m_gStatus->m_missileDB->AddTail(newMissilePtr);
	 MissileDatabaseInList(m_gStatus->m_missileDB->GetCount());
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMISEditDynamics() 
{
#ifndef _ClientOutput
     int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
		MessageBox("Must select a missile database object");
	    return;
	 }
     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
         CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);	
	     CMotionSettingsDlg dlg;
		 dlg.movementDB = misPtr->m_physicDynamics;
		 dlg.m_aiListRef = m_gStatus->m_aiList;
		 dlg.DoModal();
#endif
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTEditStartInven() 
{
#ifndef _ClientOutput
   int indexLoc=0;
   BOOL foundIt = FALSE; 
   CMovementObj * mPtr;
   //get selected index
   POSITION posLoc;
    for(posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp){
           foundIt = TRUE;
		   break;
		}
		indexLoc++;
	}
	if(foundIt == FALSE){
       MessageBox("Must select a entity base object");
	   return;
	}
	//get database object
    posLoc = m_gStatus->m_entObjList->FindIndex(indexLoc);
    if(!posLoc)
       return;
    CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetAt(posLoc);	
    CEditStartInventoryDlg dlg;
	if(!ptrLoc->m_itemDB)
	    ptrLoc->m_itemDB = new CItemObjList();
	
	
	dlg.m_gstatusMem				= m_gStatus; 
    dlg.m_itemDBRef					= ptrLoc->m_itemDB;
	dlg.m_autoCfgRef				= ptrLoc->m_autoCfg;
    dlg.m_armedInventoryDatabase	= m_gStatus->m_itemManager->m_visuals;
    dlg.DoModal();
	ptrLoc->m_autoCfg = dlg.m_autoCfgRef;
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::RenderTextureIndex(int index,CViewportObj* vptPtr)
{
 /*//set view point
 D3DMATRIX matProj,origMatrix,matView,movMatrix;
 m_gStatus->m_matLib.InitMat(origMatrix.m);
 movMatrix = origMatrix;
 D3DXVECTOR3 upVect,dirVect;
 upVect.x = 0.0f;
 upVect.y = 1.0f;
 upVect.z = 0.0f;
 dirVect.x = 0.0f;
 dirVect.y = 0.0f;
 dirVect.z = 1.0f;
 SetViewMatrixAB(matView,m_gStatus->m_d3dVectorInit,dirVect,upVect);
 movMatrix._41 += .4f;
 m_gStatus->m_dxRenderer->m_pd3dDevice->SetTransform( D3DTS_VIEW , &matView );
 m_gStatus->m_dxRenderer->m_pd3dDevice->SetTransform( D3DTS_WORLD , &movMatrix );
 m_gStatus->m_dxRenderer->m_pd3dDevice->GetTransform(D3DTS_PROJECTION,&origMatrix);
 float fAspect = ((FLOAT)vptPtr->List_imViewPort.Height) / vptPtr->List_imViewPort.Width;
 D3DUtil_SetProjectionMatrix( matProj,
				                   .3f, 
						           fAspect, 
						           1.0f,
							       2000.0f,
				                   1.0f);
 m_gStatus->m_dxRenderer->m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &matProj );

 //end set viewpoint
 float size  = .2f;
 float zDist = 10.0f;
 D3DVERTEX * verteies = new D3DVERTEX[4];
 ZeroMemory(verteies,sizeof(D3DVERTEX)*4);
 WORD * indecies = new WORD[6];
 //verticies
 verteies[0].x = -size;
 verteies[0].y = -size;
 verteies[0].z = zDist;
 verteies[0].nx = 0.0f;
 verteies[0].ny = 0.0f;
 verteies[0].nz = -1.0f;
 verteies[0].tu = 0.0f;
 verteies[0].tv = 1.0f;
 verteies[1].x = -size;
 verteies[1].y = size;
 verteies[1].z = zDist;
 verteies[1].nx = 0.0f;
 verteies[1].ny = 0.0f;
 verteies[1].nz = -1.0f;
 verteies[1].tu = 0.0f;
 verteies[1].tv = 0.0f;
 verteies[2].x = size;
 verteies[2].y = size;
 verteies[2].z = zDist;
 verteies[2].nx = 0.0f;
 verteies[2].ny = 0.0f;
 verteies[2].nz = -1.0f;
 verteies[2].tu = 1.0f;
 verteies[2].tv = 0.0f;
 verteies[3].x = size;
 verteies[3].y = -size;
 verteies[3].z = zDist;
 verteies[3].nx = 0.0f;
 verteies[3].ny = 0.0f;
 verteies[3].nz = -1.0f;
 verteies[3].tu = 1.0f;
 verteies[3].tv = 1.0f;

 //indecies
 indecies[0] = 0;
 indecies[1] = 1;
 indecies[2] = 2;
 indecies[3] = 2;
 indecies[4] = 3;
 indecies[5] = 0;
 CMaterialObject * materialTemp = new CMaterialObject();
 materialTemp->m_blendMode = 6;
 materialTemp->m_baseMaterial.Emissive.r = 1.0f;
 materialTemp->m_baseMaterial.Emissive.g = 1.0f;
 materialTemp->m_baseMaterial.Emissive.b = 1.0f;
 materialTemp->m_baseMaterial.Emissive.a = 0.5f;
 materialTemp->Reset();
 for(int loop=1;loop<m_gStatus->m_renderStateObject->m_multiTextureSupported;loop++){
     m_gStatus->m_renderStateObject->SetBlendLevel(loop,-1,m_gStatus->m_dxRenderer->m_pd3dDevice);
 }

 m_gStatus->m_dxRenderer->m_pd3dDevice->SetMaterial(&materialTemp->m_useMaterial);
 m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_dxRenderer->m_pd3dDevice,materialTemp->m_blendMode);

 m_gStatus->m_renderStateObject->SetTextureState(index,
					                  0,
					                  m_gStatus->m_dxRenderer->m_pd3dDevice,
					                  m_gStatus->m_textureDataBase);
 m_gStatus->m_dxRenderer->m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, &movMatrix );
 m_gStatus->m_dxRenderer->m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST ,  
                                     D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
									 verteies,//LPVOID lpvVertices,                   
                                     4,//DWORD  dwVertexCount,              
                                     indecies,
                                     6,//DWORD  dwIndexCount,
                                     0                        
                                     );

 m_gStatus->m_dxRenderer->m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_PROJECTION, &origMatrix );
 delete materialTemp;
 materialTemp = 0;
 delete [] verteies;
 verteies=0;
 delete [] indecies;
 indecies=0;*/
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnDropdownCMBOCGTEXFontTableTexture() 
{
	m_textFontDlgBar.SendDlgItemMessage(IDC_CMBO_CGTEX_FontTableTexture,CB_RESETCONTENT,0,0);
	int indexaTracel = 0;
	for(POSITION texPositionLoc = m_gStatus->m_textureDataBase->GetHeadPosition();texPositionLoc != NULL;){
        CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetNext(texPositionLoc);
		CString base = txtPtrLoc->List_textureName;
		base = operator+(" ",base);
		base = operator+(m_gStatus->m_cabLib.ConvToString(indexaTracel),base);
        m_textFontDlgBar.SendDlgItemMessage(IDC_CMBO_CGTEX_FontTableTexture,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)base);
        indexaTracel++;
	} 
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNCGTEXAdd() 
{
	int fontTableIndex = m_textFontDlgBar.SendDlgItemMessage(IDC_CMBO_CGTEX_FontTableTexture,CB_GETCURSEL ,0,0);
	if(fontTableIndex == -1){
	   MessageBox("Must select a font palet");
	   return;
	}

	//test font
    CGFontObj * font = new CGFontObj();
	m_textFontDlgBar.GetDlgItemText( IDC_EDIT_CGFNT_name,font->m_fontName);
    font->m_fontTableIndex       = fontTableIndex;
    POSITION texPositionLoc = m_gStatus->m_textureDataBase->FindIndex(fontTableIndex);
       CTextureDBObj * txtPtrLoc = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetAt(texPositionLoc);
	   font->m_fontTableName = txtPtrLoc->List_textureName;

    m_gStatus->m_cgFontList->AddTail(font);
	InListCGFonts(m_gStatus->m_cgFontList->GetCount()-1);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNCGTEXDelete() 
{
	int CGtextIndex = m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_CGTEX_List,CB_GETCURSEL ,0,0);
	if(CGtextIndex == -1)
	{
		MessageBox("must select a CG text Object...");
		return;
	}

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	POSITION texPositionLoc = m_gStatus->m_cgFontList->FindIndex(CGtextIndex);
	CGFontObj * fntObj = (CGFontObj *)m_gStatus->m_cgFontList->GetAt(texPositionLoc);
	delete fntObj;
	fntObj=0;
	m_gStatus->m_cgFontList->RemoveAt(texPositionLoc);
	InListCGFonts(m_gStatus->m_cgFontList->GetCount()-1);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::InListCGFonts(int sel)
{
   m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_CGTEX_List,LB_RESETCONTENT,0,0);
   for(POSITION posLoc = m_gStatus->m_cgFontList->GetHeadPosition();posLoc != NULL;)
   {
       CGFontObj * fntObj = (CGFontObj *)m_gStatus->m_cgFontList->GetNext(posLoc);
       m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_CGTEX_List,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("CG:",fntObj->m_fontName));
   }
   if(sel > -1)
	  m_textFontDlgBar.SendDlgItemMessage(IDC_LIST_CGTEX_List,LB_SETCURSEL ,sel,0);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
/*void EngineXWin::OnBTNAIAddEvent() 
{
#ifndef _ClientOutput
	int entIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
	    MessageBox("Must select a script");
		return;
	}
	CScriptEventAdd dlg;
	dlg.m_finalEventInt = 0;
	if(dlg.DoModal() == IDOK){
       POSITION posIndex = m_gStatus->m_scrList->FindIndex(entIndex);
	   if(posIndex){
          CAIScriptObj * script = (CAIScriptObj *)m_gStatus->m_scrList->GetAt(posIndex);
          if(!script->m_events)
			  script->m_events = new CAIScriptEventList();
		      script->m_events->AddEvent(dlg.m_finalEventInt,dlg.m_miscInt);
			  ScriptEventsInList(script->m_events->GetCount()-1);
	   }
	}
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNAIAddScript() 
{
#ifndef _ClientOutput
	CScriptAdd dlg;
	if(dlg.DoModal() == IDOK){
       CAIScriptObj * newScript = new CAIScriptObj();
	   newScript->scriptName        = dlg.m_scriptName;
	   newScript->scriptDescription = dlg.m_scriptDescription;

	   m_gStatus->m_scrList->AddTail(newScript);
	   ScriptInList(m_gStatus->m_scrList->GetCount()-1);
	}
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::ScriptInList(int sel)
{
	//
	m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_RESETCONTENT,0,0);
	for(POSITION posLoc = m_gStatus->m_scrList->GetHeadPosition();posLoc != NULL;)
	{
        CAIScriptObj * newScript = (CAIScriptObj *)m_gStatus->m_scrList->GetNext(posLoc);
        m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)newScript->scriptName);
	}
	if(sel != -1)
	   m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_SETCURSEL ,sel,0);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::ScriptEventsInList(int sel)
{
	int entIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
		return;
	}
	POSITION posIndex = m_gStatus->m_scrList->FindIndex(entIndex);
	 if(posIndex){
        CAIScriptObj * script = (CAIScriptObj *)m_gStatus->m_scrList->GetAt(posIndex);
	    m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_RESETCONTENT,0,0);
		if(script->m_events)
	    for(POSITION posLoc = script->m_events->GetHeadPosition();posLoc != NULL;)
		{
            CAIScriptEvent * scEvent = (CAIScriptEvent *)script->m_events->GetNext(posLoc);
			if(scEvent->m_event == 0)
               m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("GoTo :",m_gStatus->m_cabLib.ConvToString(scEvent->m_miscInt)));
		    if(scEvent->m_event == 1)
               m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("Delay :",m_gStatus->m_cabLib.ConvToString(scEvent->m_miscInt)));
		    if(scEvent->m_event == 2)
               m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("Play Animation :",m_gStatus->m_cabLib.ConvToString(scEvent->m_miscInt)));
		    if(scEvent->m_event == 3)
               m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"GoTo Random :");
		    if(scEvent->m_event == 4)
               m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Reset Script :");
		}
	    if(sel != -1)
	        m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_SETCURSEL ,sel,0);
	 }
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNAIEditScript() 
{
#ifndef _ClientOutput
    CScriptAdd dlg;
	int scrIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(scrIndex == -1){
		MessageBox("Must select a script");
		return;
	}
	POSITION posIndex = m_gStatus->m_scrList->FindIndex(scrIndex);
	 if(posIndex){
        CAIScriptObj * script = (CAIScriptObj *)m_gStatus->m_scrList->GetAt(posIndex);
		   dlg.m_scriptName = script->scriptName;
	       dlg.m_scriptDescription = script->scriptDescription;
	    if(dlg.DoModal() == IDOK){
	       script->scriptName        = dlg.m_scriptName;
	       script->scriptDescription = dlg.m_scriptDescription;
	       ScriptInList(scrIndex);
		}	
	 }
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNAIDeleteScript() 
{
	int scrIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(scrIndex == -1){
		MessageBox("Must select a script");
		return;
	}
	POSITION posIndex = m_gStatus->m_scrList->FindIndex(scrIndex);
	 if(posIndex){
        CAIScriptObj * script = (CAIScriptObj *)m_gStatus->m_scrList->GetAt(posIndex);
		script->SafeDelete();
        delete script;
		script=0;
		m_gStatus->m_scrList->RemoveAt(posIndex);
		ScriptInList(scrIndex-1);
	 }
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNAIEditEvent() 
{
#ifndef _ClientOutput
	int entIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
	    MessageBox("Must select a script");
		return;
	}
    int eventIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_GETCURSEL ,0,0);
	if(eventIndex == -1){
	    MessageBox("Must select a event");
		return;
	}


   
	CScriptEventAdd dlg;
       POSITION posIndex = m_gStatus->m_scrList->FindIndex(entIndex);
	   if(posIndex){
          CAIScriptObj * script = (CAIScriptObj *)m_gStatus->m_scrList->GetAt(posIndex);
          if(!script->m_events)
			  return;
			  POSITION eventPos = script->m_events->FindIndex(eventIndex);
		      if(eventPos){
			     CAIScriptEvent * scEvent = (CAIScriptEvent *)script->m_events->GetAt(eventPos);
			     CScriptEventAdd dlg;
				 dlg.m_finalEventInt = scEvent->m_event;
				 dlg.m_miscInt       = scEvent->m_miscInt;
				 if(dlg.DoModal()==IDOK){//1
                    scEvent->m_event         = dlg.m_finalEventInt;
				    scEvent->m_miscInt       = dlg.m_miscInt;
					ScriptEventsInList(eventIndex);
				 }//end 1
			  }
	   }
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNAIDeleteEvent() 
{
#ifndef _ClientOutput
	int entIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
	    MessageBox("Must select a script");
		return;
	}
    int eventIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_GETCURSEL ,0,0);
	if(eventIndex == -1){
	    MessageBox("Must select a event");
		return;
	}
	CScriptEventAdd dlg;
       POSITION posIndex = m_gStatus->m_scrList->FindIndex(entIndex);
	   if(posIndex){
          CAIScriptObj * script = (CAIScriptObj *)m_gStatus->m_scrList->GetAt(posIndex);
          if(!script->m_events)
			  return;
			  POSITION eventPos = script->m_events->FindIndex(eventIndex);
		      if(eventPos){
			     CAIScriptEvent * scEvent = (CAIScriptEvent *)script->m_events->GetAt(eventPos);
				 delete scEvent;
				 scEvent=0;
				 script->m_events->RemoveAt(eventPos);
				 ScriptEventsInList(eventIndex-1);
			  }
	   }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNAICreateNew() 
{
#ifndef _ClientOutput
	  CAiSettingsDlg dlg;
	  dlg.m_gStatusMem				= m_gStatus;
	  dlg.m_edbRef					= m_gStatus->m_entObjList; 
	  dlg.m_refList					= m_gStatus->m_scrList;
	  dlg.m_finalAiScriptAssign		= -1;
	  CAIObj * ai_ptr				= new CAIObj();
	  dlg.m_aiPtrRef				= ai_ptr;  
	  if(dlg.DoModal()==IDOK)
	  {
	  ai_ptr = dlg.m_aiPtrRef;
	  ai_ptr->m_spawnScript			= dlg.m_finalAiScriptAssign;
	  m_gStatus->m_aiList->AddTail(ai_ptr);
	  m_gStatus->m_aiList->ResetIndexReferences();
	  ai_ptr->List_targetFrame		= new CFrameObj(NULL);
	  AI_DBInList(m_gStatus->m_aiList->GetCount()-1);
	  }
#endif
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::AITreeInList(int sel)
{
	//
	m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_RESETCONTENT,0,0);
	for(POSITION posLoc = m_gStatus->GL_aiWebDatabase->GetHeadPosition();posLoc != NULL;)
	{
        CAIWebObj * webObj = (CAIWebObj *)m_gStatus->GL_aiWebDatabase->GetNext(posLoc);
        m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)webObj->m_name);
	}
	if(sel != -1)
	   m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_SETCURSEL ,sel,0);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNAIAddTree() 
{
	//use mapsmodels folder for organization  
	char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
    CString CurDir;
    CString MainDir;	
	MainDir = CurDir = buffer;
	CurDir = CurDir+"\\GameFiles\\";
	SetCurrentDirectory(CurDir);
	CString fileName = "<none>";
	//end use mapsmodels folder for organization
	static char BASED_CODE filter[] = "TRE Files|*.tre";
	CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
    if ( opendialog.DoModal() == IDOK )
		fileName = opendialog.GetPathName();
	else{
		SetCurrentDirectory(MainDir);
		return;
	}
	SetCurrentDirectory(MainDir);
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 CArchive the_in_Archive(&fl,CArchive::load);
	 CAIWebObj * webObj = NULL;
	 the_in_Archive >> webObj;
	 the_in_Archive.Close();
     fl.Close();
	 m_gStatus->GL_aiWebDatabase->AddTail(webObj);
	 AITreeInList(m_gStatus->GL_aiWebDatabase->GetCount()-1);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNAIDeleteTree() 
{
	int treeIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_GETCURSEL ,0,0);
	if(treeIndex == -1){
		MessageBox("Must select a tree");
		return;
	}
	POSITION posIndex = m_gStatus->GL_aiWebDatabase->FindIndex(treeIndex);
	 if(posIndex){
        CAIWebObj * webObj = (CAIWebObj *)m_gStatus->GL_aiWebDatabase->GetAt(posIndex);
		webObj->SafeDelete();
        delete webObj;
		webObj=0;
		m_gStatus->GL_aiWebDatabase->RemoveAt(posIndex);
		AITreeInList(m_gStatus->GL_aiWebDatabase->GetCount()-1);
	 }
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNAIEdit() 
{
#ifndef _ClientOutput
	int aiIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_DB,LB_GETCURSEL ,0,0);
	if(aiIndex == -1){
	    MessageBox("Must select a bot");
		return;
	}

    POSITION posLoc = m_gStatus->m_aiList->FindIndex(aiIndex);
	if(!posLoc)
		return;
	CAIObj * aiPtr = (CAIObj *) m_gStatus->m_aiList->GetAt(posLoc);

	CAiSettingsDlg dlg;
	  dlg.m_gStatusMem				= m_gStatus;
	  dlg.m_edbRef					= m_gStatus->m_entObjList; 
	  dlg.m_refList					= m_gStatus->m_scrList;
	  dlg.m_finalAiScriptAssign		= aiPtr->m_spawnScript;
	  dlg.m_aiPtrRef				= aiPtr;  
	  if(dlg.DoModal()==IDOK)
	  {
		  aiPtr						= dlg.m_aiPtrRef;
		  aiPtr->m_spawnScript		= dlg.m_finalAiScriptAssign;
		  AI_DBInList(aiIndex);
	  }
#endif
}

void EngineXWin::OnBTNAISaveAiData() 
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
	static char BASED_CODE filter[] = "AII Files|*.AII|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "AII", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_gStatus->m_aiList << m_gStatus->m_scrList;
       the_out_Archive.Close();
       fl.Close();

	 }

	SetCurrentDirectory(MainDir);
}

//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnBTNAILoadAiData() 
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

   static char BASED_CODE filter[] = "AII Files|*.AII|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadAiData(fileName);
	   if(m_gStatus->m_editorModeOn == TRUE)
	   {
	      AI_DBInList(m_gStatus->m_aiList->GetCount()-1);
	      ScriptInList(m_gStatus->m_scrList->GetCount()-1);
	   }
	 }
     SetCurrentDirectory(MainDir);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void EngineXWin::OnSelchangeLISTAIScripts() 
{
	m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Events,LB_RESETCONTENT,0,0);
	int entIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_Scripts,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
		return;
	}
       POSITION posIndex = m_gStatus->m_scrList->FindIndex(entIndex);
	   if(posIndex){
          CAIScriptObj * script = (CAIScriptObj *)m_gStatus->m_scrList->GetAt(posIndex);
          if(script->m_events)
			  ScriptEventsInList(script->m_events->GetCount()-1);
	   }
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTSaveAppendages() 
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
     static char BASED_CODE filter[] = "APX Files|*.APD|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "APX", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString filePath = opendialog.GetPathName();
       m_gStatus->m_itemManager->Load(filePath.GetBuffer());
	 }
	 SetCurrentDirectory(MainDir);  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTLoadAppendages() 
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
	  static char BASED_CODE filter[] = "APX Files|*.APX|All Files (*.*)|*.*||";
	  CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
      if ( opendialog.DoModal() != IDOK ){
           SetCurrentDirectory(MainDir); 
	  }
	  CString filePath = opendialog.GetPathName();	
	  if(m_gStatus->m_itemManager){
	     delete m_gStatus->m_itemManager;
	     m_gStatus->m_itemManager=0;
	  }
	  m_gStatus->m_itemManager = new CItemManager();
	  m_gStatus->m_itemManager->Load(filePath.GetBuffer());
      SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTAddAppendages() 
{
/*int indexLoc=0;
    BOOL foundIt = FALSE; 
    for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->List_traceNumber))) == temp){
           foundIt = TRUE;
		   break;
		}
		indexLoc++;
	}
	if(foundIt == FALSE){
       MessageBox("Must select a entity base object");
	   return;
	}
	//get database object
    posLoc = m_gStatus->m_entObjList->FindIndex(indexLoc);
    if(!posLoc)
      return;
      CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjList->GetAt(posLoc);
	  //use gamefiles folder for organization
      CString CurDir;
      CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
      //end use gamefiles folder for organization 	 
	  static char BASED_CODE filter[] = "APD Files|*.APD|All Files (*.*)|*.*||";
	  CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
      if ( opendialog.DoModal() != IDOK ){
           SetCurrentDirectory(MainDir); 
	  }
	  CString fileName = opendialog.GetFileName();	 
	  CFileException exc;
      CFile fl;
      BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	  if(res == FALSE){
		SetCurrentDirectory(MainDir); 
		return;
	  }
	  CArmedInventoryList * newAppendages = NULL;
	  CArchive the_in_Archive(&fl,CArchive::load);
	  the_in_Archive >> newAppendages;
	  the_in_Archive.Close();
      fl.Close();
      //append
      if(newAppendages){
		  for(POSITION aPos = newAppendages->GetHeadPosition();aPos != NULL;){
              CArmedInventoryObj * armedPtr = (CArmedInventoryObj *)newAppendages->GetNext(aPos);
              ptrLoc->List_armedInventoryList->AddTail(armedPtr);
		  }
		  newAppendages->RemoveAll();
		  delete newAppendages;
		  newAppendages=0;
	  }
      SetCurrentDirectory(MainDir);	*/
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBtnEntMount() 
{
	POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(0);
	if(!posLoc){
		MessageBox("no user created");
		return;
	}
	CMovementObj * hostPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
    int aiselect = m_entityDlgBar.GetDlgItemInt(IDC_EDIT_ENT_mountAiIndex,NULL,TRUE);
    

    m_gStatus->GenAndMount(hostPtr,aiselect,m_gStatus->m_entObjRuntime,m_gStatus->m_missileRuntime);
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBtnEntDemount() 
{
	POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(0);
	if(!posLoc){
		MessageBox("no user created");
		return;
	}
	CMovementObj * hostPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
    
	m_gStatus->DeMount(hostPtr,m_gStatus->m_entObjRuntime);
	
	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::RenderFeelers(CMovementObj * ptrLoc,D3DVECTOR offset)
{
if(!ptrLoc->m_aiObj)
    return;

    D3DMATRIX matrix;
    ptrLoc->List_baseFrame->GetTransform(NULL,&matrix);

	D3DVECTOR pointOne,pointTwo;
    pointOne.x = ptrLoc->m_aiObj->m_feelerSys->m_startPoint.x;
	pointOne.y = ptrLoc->m_aiObj->m_feelerSys->m_startPoint.y;
	pointOne.z = ptrLoc->m_aiObj->m_feelerSys->m_startPoint.z;
    pointTwo.x = ptrLoc->m_aiObj->m_feelerSys->m_endPoint.x;
	pointTwo.y = ptrLoc->m_aiObj->m_feelerSys->m_endPoint.y;
	pointTwo.z = ptrLoc->m_aiObj->m_feelerSys->m_endPoint.z;
   
	pointOne = m_gStatus->m_matLib.UpdatePolygonVertex(matrix.m,pointOne);
	pointTwo = m_gStatus->m_matLib.UpdatePolygonVertex(matrix.m,pointTwo);

    DrawSegment(m_gStatus->m_dxRenderer->m_pd3dDevice,pointOne,pointTwo,offset);

}



//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL EngineXWin::LoadWorldObjectFromData(ABVERTEX * vArray,
										 WORD * iArray,
										 WORD vCount,
										 WORD iCount,
										 CString texture1,
										 CString texture2,
										 CString texture3,
										 CString texture4,
										 CString texture5,
										 CString texture6,
										 CString texture7,
										 CString texture8,
										 CString objectName,
										 float sightRange,
										 BOOL smoothShading)
{
          CWldObject * ptrLoc = new CWldObject((m_gStatus->GlobalTraceNumber++)); 
		  ptrLoc->m_fileNameRef = objectName;
		  

		     ptrLoc->List_groupNumber = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box,LB_GETCURSEL ,0,0);
             if(ptrLoc->List_groupNumber == -1){
	            ptrLoc->List_groupNumber=0;
			 }

		  
		  ptrLoc->List_triggerList = new CTriggerObjectList;
		  //load imediate mode mesh data
             ptrLoc->m_meshObject = new CEXMeshObj();
			 ptrLoc->m_meshObject->m_groupReference = ptrLoc->List_groupNumber;
			 
			 ptrLoc->m_meshObject->List_storedTextureFromFile[0] = texture1;
			 ptrLoc->m_meshObject->List_storedTextureFromFile[1] = texture2;
			 ptrLoc->m_meshObject->List_storedTextureFromFile[2] = texture3;
			 ptrLoc->m_meshObject->List_storedTextureFromFile[3] = texture4;
			 ptrLoc->m_meshObject->List_storedTextureFromFile[4] = texture5;
			 ptrLoc->m_meshObject->List_storedTextureFromFile[5] = texture6;
			 ptrLoc->m_meshObject->List_storedTextureFromFile[6] = texture7;
			 ptrLoc->m_meshObject->List_storedTextureFromFile[7] = texture8;
			 m_gStatus->AddTextureToDatabase(ptrLoc->m_meshObject);//apply mapping and add to DB
			 ptrLoc->m_meshObject->List_materialObject = new CMaterialObject();

			 ptrLoc->m_meshObject->GetUVlayerCountString();
			 
			 ptrLoc->m_meshObject->SetGeometry(texture1,
				                              vArray,
											  vCount,
											  iArray,
											  iCount);

			 

			 ptrLoc->m_meshObject->OptimizeMesh(NULL);
			 
			 if(smoothShading == TRUE)
             ptrLoc->m_meshObject->GenerateSmoothNormals();
			 else
			 ptrLoc->m_meshObject->GenerateNormals(0);

          if(m_gStatus->InitWorldObject(ptrLoc)==TRUE){
		     m_gStatus->m_wldObjList->AddTail(ptrLoc);
			 ptrLoc->m_meshObject->List_materialObject->Reset();
			 ptrLoc->List_popoutRadius = ptrLoc->m_meshObject->m_meshBoundingSphereRadius * 30;

			 ptrLoc->List_popoutBox.max.x =  ((sightRange+1)+ptrLoc->m_meshObject->m_meshCenter.x);
			 ptrLoc->List_popoutBox.max.y =  ((sightRange+2)+ptrLoc->m_meshObject->m_meshCenter.y);
			 ptrLoc->List_popoutBox.max.z =  ((sightRange+3)+ptrLoc->m_meshObject->m_meshCenter.z);
			 ptrLoc->List_popoutBox.min.x = -(sightRange+4)+ptrLoc->m_meshObject->m_meshCenter.x;
			 ptrLoc->List_popoutBox.min.y = -(sightRange+5)+ptrLoc->m_meshObject->m_meshCenter.y;
			 ptrLoc->List_popoutBox.min.z = -(sightRange+6)+ptrLoc->m_meshObject->m_meshCenter.z;

			 HTREEITEM Root = AddTreeRoot(operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity))),IDC_OBJECT_TREE,&m_dlgBarObject);   
	         AddTreeChild(ptrLoc->m_fileNameRef,IDC_OBJECT_TREE,&m_dlgBarObject,Root);  
		  }
		  else
		  return FALSE;
			 

		  return TRUE;
}




//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
/*void EngineXWin::OnBTNEXPEditExplosionObj() 
{
#ifndef _ClientOutput
   int runtimeIndex = m_explosionDlgBar.SendDlgItemMessage(IDC_LIST_ExplosionList,LB_GETCURSEL ,0,0);
   if(runtimeIndex == -1){
      MessageBox("Must select an explosion");
	  return;
   }
   POSITION posLoc = m_gStatus->m_explosionDBList->FindIndex(runtimeIndex);
   if(!posLoc)
	   return;
       CExplosionObj * expObj = (CExplosionObj *)m_gStatus->m_explosionDBList->GetAt(posLoc);
   CExplosionBasicDlg dlg;
   dlg.m_dirSndRef = m_gStatus->m_dirSnd;
   dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
   dlg.m_explosionRef = expObj;
   dlg.m_elementDBREF = m_gStatus->m_elementDB;
   dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
   dlg.m_skillDatabaseRef = m_gStatus->m_skillDatabase;
   if(dlg.DoModal()==IDOK){

   }
   InListExplosionDB(runtimeIndex);
#endif
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
/*void EngineXWin::InListExplosionDB(int sel)
{
if(m_gStatus->m_editorModeOn==FALSE)
return;
int traceIt=0;
    m_explosionDlgBar.SendDlgItemMessage(IDC_LIST_ExplosionList,LB_RESETCONTENT,0,0);	
	for(POSITION posLoc = m_gStatus->m_explosionDBList->GetHeadPosition();posLoc != NULL;){
        CExplosionObj * partPtr = (CExplosionObj *)m_gStatus->m_explosionDBList->GetNext(posLoc);

        CString build = operator+(" ",partPtr->m_explosionName);
		build = operator+(m_gStatus->m_cabLib.ConvToString(traceIt),build);

        m_explosionDlgBar.SendDlgItemMessage(IDC_LIST_ExplosionList,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)build);
		traceIt++;
	}
	if(sel != -1)
	   m_explosionDlgBar.SendDlgItemMessage(IDC_LIST_ExplosionList,LB_SETCURSEL ,sel,0);
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Delete selected explosion---------------------------------------------//
/*void EngineXWin::OnBTNEXPDeleteExplosion() 
{
   int runtimeIndex = m_explosionDlgBar.SendDlgItemMessage(IDC_LIST_ExplosionList,LB_GETCURSEL ,0,0);
   if(runtimeIndex == -1){
      MessageBox("Must select an explosion");
	  return;
   }
   POSITION posLoc = m_gStatus->m_explosionDBList->FindIndex(runtimeIndex);
   if(!posLoc)
	   return;
       CExplosionObj * expObj = (CExplosionObj *)m_gStatus->m_explosionDBList->GetAt(posLoc);
	   expObj->SafeDelete();
	   delete expObj;
	   expObj=0;
	   m_gStatus->m_explosionDBList->RemoveAt(posLoc);
   
   InListExplosionDB(runtimeIndex-1);
}*/
//---------------Load explosion data--------------------------------------------------//
/*void EngineXWin::OnBTNEXPLoadExplosions() 
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

   static char BASED_CODE filter[] = "EXP Files|*.EXP|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadExplosionFile(fileName);
	   if(m_gStatus->m_editorModeOn == TRUE)
	      InListExplosionDB(0);
	 }
     SetCurrentDirectory(MainDir);
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------Save Explosion Data--------------------------------------------------//
/*void EngineXWin::OnBTNEXPSaveExplosions() 
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
     static char BASED_CODE filter[] = "EXP Files|*.EXP|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "EXP", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_gStatus->m_explosionDBList;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
#endif
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
/*void EngineXWin::OnBTNMISSound() 
{
#ifndef _ClientOutput
   int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
        MessageBox("Must Select a missile database object");
	    return;
	 }
     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
     CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);
     CSoundSettingsDlg dlg;
	  dlg.enabled          = misPtr->m_enableSound; 
	  dlg.m_distanceFactor = misPtr->m_distanceFactor;
	  dlg.m_sndID          = misPtr->m_sndID;
	  if(dlg.DoModal()==IDOK){
	   
		  if(dlg.enabled){
			 misPtr->m_sndID            = dlg.m_sndID;
			 misPtr->m_enableSound      = dlg.enabled;
             misPtr->m_distanceFactor   = dlg.m_distanceFactor;
		  }
	  }	
#endif
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMUSICAddTrack() 
{
//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\Sounds\\";
			 SetCurrentDirectory(CurDir);
//end use mapsmodels folder for organization
     static char BASED_CODE filter[] = "wav Files|*.wav|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY , filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   CString trackName = opendialog.GetFileTitle();
	   bool isUrl = false;
	   if(fileName.GetLength() > 4)
	   {
	      if(fileName.GetAt(0) == 'h')
		     if(fileName.GetAt(1) == 't')
			    if(fileName.GetAt(2) == 't')
				   if(fileName.GetAt(3) == 'p')
				      isUrl = true;
	   }
	   if(isUrl == false)
	      m_gStatus->m_cabLib.RemoveFileExt(&trackName);
	   else
		   trackName = fileName;
	   
	   SetCurrentDirectory(MainDir);
	   m_gStatus->m_soundTrackList->AddTrack(trackName,fileName,isUrl);
	   UpdateData(FALSE);
	 }
     SetCurrentDirectory(MainDir);
	 SoundTrackInList(m_gStatus->m_soundTrackList->GetCount()-1);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::SoundTrackInList(int sel)
{
    m_musicDlgBar.SendDlgItemMessage(IDC_LIST_MUSIC_SoundTracks,LB_RESETCONTENT,0,0);
	for(POSITION posLoc = m_gStatus->m_soundTrackList->GetHeadPosition();posLoc != NULL;)
	{
        CSoundTrackObj* sndObj = (CSoundTrackObj*)m_gStatus->m_soundTrackList->GetNext(posLoc);
        m_musicDlgBar.SendDlgItemMessage(IDC_LIST_MUSIC_SoundTracks,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)sndObj->m_trackName);
	}
	if(sel != -1)
	    m_musicDlgBar.SendDlgItemMessage(IDC_LIST_MUSIC_SoundTracks,LB_SETCURSEL ,sel,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMUSICDeleteTrack() 
{
	int trackIndex = m_musicDlgBar.SendDlgItemMessage(IDC_LIST_MUSIC_SoundTracks,LB_GETCURSEL ,0,0);
	if(trackIndex == -1)
	   return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

    m_gStatus->m_soundTrackList->RemoveTrack(trackIndex);
	SoundTrackInList(m_gStatus->m_soundTrackList->GetCount()-1);
	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMUSICTestTrack() 
{

}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMUSICLoadDB() 
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
	 static char BASED_CODE filter[] = "STK Files|*.STK|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   SetCurrentDirectory(MainDir);
	   CString fileName = opendialog.GetFileName();
       m_gStatus->LoadSoundTrackDB(fileName);
	      SoundTrackInList(0);//
	 }
	 SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMUSICSaveDB() 
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
     static char BASED_CODE filter[] = "SoundTrack Files|*.STK|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "STK", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_gStatus->m_soundTrackList;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMUSICStopSoundTrack() 
{
    //int trackIndex = m_musicDlgBar.SendDlgItemMessage(IDC_LIST_MUSIC_SoundTracks,LB_GETCURSEL ,0,0);
	//if(trackIndex == -1){
	   m_gStatus->m_soundTrackList->Stop();
	  // return;
	//}

}
//----------------------------------------------------------//
//----------------------------------------------------------//

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
int EngineXWin::SetJet(CMovementObj * ptrLoc,
						float forceY,
						float forceZ)
{
    if(ptrLoc->List_armedInventoryDB)
        for(POSITION posLoc = ptrLoc->List_armedInventoryDB->GetHeadPosition();posLoc != NULL;)
		{
            CArmedInventoryObj * armedPtr = (CArmedInventoryObj *)ptrLoc->List_armedInventoryDB->GetNext(posLoc);
            if(armedPtr->m_functionalityAttribute == 3)
			{//jet
			     armedPtr->m_miscPosition.x;
	             armedPtr->m_miscPosition.y = forceY;
                 armedPtr->m_miscPosition.z = forceZ;
				 return armedPtr->m_type;
			}//end jet
		}
	return -1;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
void EngineXWin::UpdateInfoEverySecond()
{
#ifdef _ClientOutput
	// client build: no editor dialog bars to feed, nothing to do
	return;
#else
	   if(!m_gStatus || !m_gStatus->m_dxRenderer)
	      return;

//Editor display
		   int currentTickCountLocal = GetTickCount();
		   if((currentTickCountLocal - updateEditorVarial) > 1000){//update every second
		   setIt = TRUE;
		   updateEditorVarial = GetTickCount();
		   if(m_gStatus->m_dxRenderer->m_fullScreenMode == FALSE)
		   {//1
		   TallyLightsInUse();
		   m_wndDlgBar.SetDlgItemInt(IDC_EDIT_UI_CurrentLightsInUse,LIGHTSINUSE,TRUE);
		   LIGHTSINUSE=0;
		   m_wndDlgBar.SetDlgItemText( IDC_EDIT_UI_DeviceInUse, m_publicNotifyDeviceType);

		   //int availTextureMem = m_gStatus->m_dxRenderer->m_pd3dDevice->GetAvailableTextureMem()/1000000;
           m_wndDlgBar.SetDlgItemInt(IDC_EDIT_UI_AvailTextureMem,(m_gStatus->m_dxRenderer->m_pd3dDevice->GetAvailableTextureMem()/1000000),TRUE);
           CString objectsFromTable = m_gStatus->m_cabLib.ConvToString(m_sceneAssembler->GL_objectsFromTable);
		   objectsFromTable = objectsFromTable+"/V:";
		   CString objectsRendered = m_gStatus->m_cabLib.ConvToString(m_gStatus->GL_objectsRendered);
           CString final = objectsFromTable+objectsRendered;
		   m_wndDlgBar.SetDlgItemText(IDC_EDIT_UI_ObjectsRendered,final);
		   if(m_rendermanagerEd->GL_polysRenderedPerFrame > 0)
		   m_wndDlgBar.SetDlgItemInt(IDC_EDIT_UI_ObjectsPoppedIn,(m_rendermanagerEd->GL_polysRenderedPerFrame/3),TRUE);
		   m_wndDlgBar.SetDlgItemInt(IDC_EDIT_UI_RndrPopoutCalcs,m_rendermanagerEd->VPORTRENDERCOUNT,TRUE);
		   m_wndDlgBar.SetDlgItemInt(IDC_EDIT_UI_FPSLIMIT,m_gStatus->m_exactFPSHiRes,TRUE);
	       m_wndDlgBar.SetDlgItemInt(IDC_EDIT_MOUSE_X,GlobalMouseCenterPoint.x,TRUE);
	       m_wndDlgBar.SetDlgItemInt(IDC_EDIT_MOUSE_Y,GlobalMouseCenterPoint.y,TRUE);
		   m_wndDlgBar.SetDlgItemInt(IDC_EDIT_UI_CollisionCalc,m_gStatus->m_collisionPolysInUse,TRUE);
		   m_wndDlgBar.SetDlgItemInt(IDC_EDIT_ActualCalcPolys,m_gStatus->m_actualPolysCalculated,TRUE);
		   m_wndDlgBar.SetDlgItemInt(IDC_EDIT_UI_TextureStateChanges,m_gStatus->m_renderStateObject->m_stateChangesPerPass,TRUE);
		   m_wndDlgBar.SetDlgItemText(IDC_EDIT_UI_RenderTime,m_gStatus->m_cabLib.FloatToStingCnv((float)m_rendermanagerEd->GL_straightRenderDuration));
		   m_wndDlgBar.SetDlgItemText(IDC_EDIT_UI_MathTime,m_gStatus->m_cabLib.FloatToStingCnv((float)m_gStatus->GL_mathDuration));
		   m_wndDlgBar.SetDlgItemText(IDC_EDIT_UI_MiscTimeStamp,m_gStatus->m_cabLib.FloatToStingCnv((float)m_gStatus->m_toRenderSurfaceDB->GetCount())); 
           m_wndDlgBar.SetDlgItemInt(IDC_EDIT_UI_ParticleCounter,m_rendermanagerEd->GL_particleCalculationCount,TRUE);
		   //m_wndDlgBar.SetDlgItemInt(IDC_EDIT_PARTICLE_ECOUNT,m_gStatus->m_runtimeParticleDatabase->GetCount(),TRUE);
		   m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCPIP_YourPing,m_cNetworkObject->CURPING,TRUE); 
		   m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCPIP_packetsSentPerPass,m_gStatus->m_multiplayerObject->m_packetsOutPerPass,TRUE); 
		   m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCPIP_MsgsRecievedPerPass,m_gStatus->m_multiplayerObject->m_packetsRecievedPerPass,TRUE);   
		   m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCPIP_LagCount,LagCount,TRUE);
		   m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCP_BandwithUse,(m_gStatus->m_multiplayerObject->m_totalBandwidthOutGoing*8),TRUE);
		   m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCP_BandwidthInPerSec,(m_gStatus->m_multiplayerObject->GetTotalBandwidthIncomming_safe()*8),TRUE);
		   m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCP_PlayerCountInGame,m_gStatus->m_multiplayerObject->m_currentPlayersOnServer,TRUE);
		   m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCP_FrequencyRestriction,m_gStatus->m_multiplayerObject->m_communicationFrequency,TRUE);


		   

		   m_gStatus->m_performanceTimeStamp = 0;
           if(m_gStatus->m_multiplayerObject->m_clientInterface)
		   {
			   DWORD numberOfMessages = 0;
			   DWORD totalSizeInBytes = 0;
               m_gStatus->m_multiplayerObject->m_clientInterface->GetSendQueueInfo(&numberOfMessages,
				                                                        &totalSizeInBytes,
                                                                        0);
			   m_multiPlayer.SetDlgItemInt(IDC_EDIT_TCP_clientSendQue,numberOfMessages,TRUE);
		   }
		   
		   }//end 1
		   m_gStatus->m_multiplayerObject->ZeroTotalBandwidthIncomming_safe();
		   m_gStatus->m_multiplayerObject->m_totalBandwidthOutGoing=0;
		   m_gStatus->m_multiplayerObject->m_packetsOutPerPass=0;
           m_gStatus->m_multiplayerObject->m_packetsRecievedPerPass=0;
		   GL_mislTimeStampTest = 0;
		   GL_counterThing = 0;
		   GL_exactFPS = 0;
		   }//end update every second
#endif // _ClientOutput
}

//----------------------------------------------------------//
void EngineXWin::HandleFPSTimeStamp()
{

}
//----------------------------------------------------------//
void EngineXWin::RenderLoopMessageHandling()
{
    if(!GL_dedicatedRenderLoop){
			
			MSG Msg;
			if(PeekMessage(&Msg,NULL,0,0,PM_NOREMOVE))
				{
				CPluginTransfer transfer;
				CString formatPathToName,objName;
				//m_gStatus->m_systemReadout->AddLine("got message");
                if(Msg.message == transfer.m_linkMSG)
				{//file target system
                   BYTE * dataRaw = NULL;
				   DWORD sizeRaw = 0;
				   transfer.ReceiveMemory(&dataRaw, &sizeRaw);
				   DWORD trace = 0;
                   while(trace < sizeRaw)
				   {//illiterate
				   DWORD strinLength = 0;
				   short specialLoadCommand = 0;
				   CopyMemory(&strinLength, &dataRaw[trace], sizeof(DWORD));
				   trace += sizeof(DWORD);
				   char * newPath = new char[strinLength+1];
				   ZeroMemory(newPath,strinLength+1);
				   CopyMemory(newPath,&dataRaw[trace],strinLength-(sizeof(DWORD)+sizeof(short)));
				   CopyMemory(&specialLoadCommand,&dataRaw[trace + (strinLength-(sizeof(short)+sizeof(DWORD)))],sizeof(short));
				   
				   
				   CWldObject * retBasisLoc = NULL;
				   formatPathToName = newPath;
				   if(formatPathToName.GetLength() > 0)
				   {//valid path
				    CString fType = m_gStatus->m_cabLib.GetFileExt(formatPathToName);
				    if(fType.MakeUpper() == "ARW" || CAssimpModelImporter::IsSupportedFile(formatPathToName))
				    {//world obj type
				     
					 
						
						
				     
                     //parse name
                     m_gStatus->m_cabLib.RemoveFileExt(&formatPathToName);
				     int trace=formatPathToName.GetLength()-1;
					 objName = "";
                     while(trace > 0)
				     {
                       char xAdd = formatPathToName.GetAt(trace);
					   if(xAdd == '\\')
					      break;
					   objName = xAdd + objName;
					   trace--;
				     }

					 if(specialLoadCommand == 0)
					 {
                         LoadWorldObject(newPath,&retBasisLoc);
						 retBasisLoc->m_fileNameRef = objName;
					 }
					 else if(specialLoadCommand == 1)
					 {
					     if(!ReplaceObjectByName(objName,newPath))
						 {
                             LoadWorldObject(newPath,&retBasisLoc);
                             retBasisLoc->m_fileNameRef = objName;
						 }
					 }
				     
				    }//end world obj type
					else if(fType.MakeUpper() == "ARB")
				    {//Entity obj type
                       CMovementObj * mPtr = new CMovementObj("NEW ENT",//name
										                     (m_gStatus->GlobalTraceNumber++));//secondary collision file
										   
	                   
                       //parse name
				       int trace=formatPathToName.GetLength()-1;
					   objName = "";
                       while(trace > 0)
				       {
                             char xAdd = formatPathToName.GetAt(trace);
					         if(xAdd == '\\')
					            break;
					         objName = xAdd + objName;
					         trace--;
				       }
					   CString folderPath = formatPathToName;
					   folderPath.Replace(objName,"");
					   if(mPtr->LoadFromDir(formatPathToName,folderPath,m_gStatus->m_dxRenderer->m_pd3dDevice,TRUE))
					   {
						 InitFXSahdersOnSkeletal(mPtr->List_skeletalObject);
					     m_gStatus->m_entObjList->AddTail(mPtr);
					     //m_gStatus->ReinitAllTexturesFromDatabase();
						 m_gStatus->SkeletalTextureReassignmentWCompile(mPtr->List_skeletalObject);
	                     HTREEITEM item = AddTreeRoot(operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId)),IDC_TREE_ENTITIES,&m_entityDlgBar);
                         AddTreeChild(mPtr->List_skeletalObject->m_SkeletalName,IDC_TREE_ENTITIES,&m_entityDlgBar,item);	
					   }
					   else
						 AfxMessageBox("Skeletal Failed to Load:"+formatPathToName);
					}//end Entity obj type
				   }//end valid path
                   trace += strinLength-(sizeof(DWORD));
				   }//end illiterate
		
			
                   if(dataRaw)
				      delete [] dataRaw;
				      dataRaw = 0;
				}//end file target system
				/*if(Msg.message == transfer.m_linkMSG)
					//message is from 3D Studio plugin
					{
				    m_gStatus->m_systemReadout->AddLine("Transfer Initialized",m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
					int trnsfrType = Msg.wParam;
					if(trnsfrType==0) 
						{
												
						if(transfer.ReceiveMeshExport()==FALSE)
							{
							DWORD err = GetLastError();
							MessageBox("ReceiveMeshExport() failed");
							}
						else
							{
							while(transfer.MeshInQueue())
								{
								ABVERTEX *vArray = NULL;
								WORD vCount;
								
								WORD *iArray = NULL;
								WORD iCount;

								CString texture = "";
								CString texture2 = "";
								CString texture3 = "";
								CString texture4 = "";
								CString texture5 = "";
								CString texture6 = "";
								CString texture7 = "";
								CString texture8 = "";
								CString fileName = "";

								BOOL smoothShading;
								float sightRange;

								transfer.GetMesh
									(
									&smoothShading,
									&sightRange,
									fileName,
									&vArray,
									&iArray,
									&vCount,
									&iCount,
									texture,
									texture2,
									texture3,
									texture4,
									texture5,
									texture6,
									texture7,
									texture8
									);

								 if(vCount != 0 && iCount != 0)
								 LoadWorldObjectFromData(vArray,
											        iArray,
											        vCount,
											        iCount,
												    texture,
													texture2,
													texture3,
													texture4,
													texture5,
													texture6,
													texture7,
													texture8,
													fileName,
													sightRange,
													smoothShading);
								}
							}
							
						}

					}*/
				}
		}
		else{
		 //do not do this it kill performance in duration
         //MSG Msg;
         //PeekMessage(&Msg,NULL,WM_MOUSEFIRST,WM_MOUSELAST,PM_REMOVE);
         //PeekMessage(&Msg,NULL,WM_KEYFIRST ,WM_KEYLAST ,PM_REMOVE);
		}
		//get all keystroke messages if requested

		
		//MSG Msg;
		//if(PeekMessage(&Msg,NULL,WM_KEYFIRST ,WM_KEYFIRST ,PM_REMOVE)!=0)
		//AfxMessageBox(m_gStatus->m_cabLib.ConvToString(Msg.wParam));
		
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
inline CString EngineXWin::TranslateCharValue(int value)
{
    
     switch(value)
	 {
     case 191:
     return "?";
     case 187:
     return "+";
	 case 219:
     return "{";
	 case 221:
     return "}";
	 case 186:
     return ":";
	 case 188:
     return "<";
	 case 190:
     return ">";
	 case 222:
     return "'";
	 case 16:
     return "";
	 }

	 return "";
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
/*void EngineXWin::OnBTNMNUGetCursor() 
{
    int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
		MessageBox("must select a menu");
	   return;
	}
  	
	POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
		if(menuObj->m_cursor){
           menuObj->m_cursor->SafeDelete();
		   delete menuObj->m_cursor;
		   menuObj->m_cursor=0;
		}

    int cloneSel = m_menuDBDlgBar.GetDlgItemInt(IDC_EDIT_MNU_MenuIndex,NULL,TRUE);
	posloc = m_gStatus->m_menuDatabase->FindIndex(cloneSel);
	if(!posloc)
		return;
	    
        CMenu3DObj * mPtr = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
		if(mPtr->m_cursor)
		   mPtr->m_cursor->Clone(&menuObj->m_cursor,m_gStatus->m_dxRenderer->m_pd3dDevice);
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
BOOL EngineXWin::GetScreenPos(int *ScrnPosHorz, 
						      int *ScrnPosVert, 
						      int HieghtRes,
                              int WidthRes, 
						      double ClpPlaneLeft, 
						      double ClpPlaneBottom,
                              double ClpPlaneRight, 
						      double ClpPlaneTop,
                              double ClpPlaneDist, 
						      D3DVECTOR My3DPnt,
						      float Aspect)
{
 double ScaleFactor;
 double dScrnPosHorz;
 double dScrnPosVert;
 
 if(My3DPnt.z<0)
	return FALSE;//behind u

 ScaleFactor = ClpPlaneDist/My3DPnt.z;

 dScrnPosHorz = ((My3DPnt.x * ScaleFactor) - ClpPlaneLeft)
              * (WidthRes / (ClpPlaneRight - ClpPlaneLeft));

 dScrnPosVert = (ClpPlaneTop - (My3DPnt.y * ScaleFactor))
              * (HieghtRes / (ClpPlaneTop - ClpPlaneBottom));

 *ScrnPosHorz = ((int)floor(dScrnPosHorz));
 *ScrnPosVert = ((int)floor(dScrnPosVert));
 
 //*ScrnPosHorz = (int)(*ScrnPosHorz+(((WidthRes*Aspect)-WidthRes)*.5f));
 if(*ScrnPosHorz >= 0 && *ScrnPosHorz <= WidthRes && *ScrnPosVert >= 0 && *ScrnPosVert <= HieghtRes)
 return TRUE;
 else
 return FALSE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL EngineXWin::GetPixelSnap(D3DMATRIX camMatrix,
							   float fovLoc,
							   D3DVECTOR targetWldPosition,
							   DWORD * pixelValue,
							   DWORD surfaceBBP,
							   BYTE* pBytes,
							   float vPortHAspect,
							   DWORD pitch
							   )
{

	 D3DVECTOR targetLocalPosition = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(camMatrix,targetWldPosition);
     //m_gStatus->m_systemReadout->AddLine(targetLocalPosition);
	    double fAspect = ((FLOAT)m_curModeHieght) / m_curModeWidth;
	    //FLOAT w = ( cosf(fovLoc/2)/sinf(fovLoc/2) );
        //FLOAT h = ( cosf(fovLoc/2)/sinf(fovLoc/2) );
	 
	    double compromizer =  (1.0+(1.0-fAspect));
		double compromizer2 = (1.0+(1.0-vPortHAspect));
		//float compromizer = fAspect;
	    //double ClpPlaneLeft = (-1* tan((0.5f * fovLoc))); 
		//double ClpPlaneRight = (   tan((0.5f * fovLoc))); 
	    double ClpPlaneLeft = (-1* tan((0.51*compromizer) * fovLoc)); 
		double ClpPlaneRight = (   tan((0.51*compromizer) * fovLoc)); 
	    //double ClpPlaneTop = tan((0.5f * fovLoc));
		double ClpPlaneTop =    (    tan(((0.51*compromizer2) * fovLoc)));
	    double ClpPlaneBottom = (-1* tan(((0.51*compromizer2) * fovLoc)));
	    int    surfacePosX,surfacePosY;
        
        
        if(GetScreenPos(&surfacePosX, 
				        &surfacePosY, 
					    m_curModeHieght,//int HieghtRes,  m_curModeHieght
                        m_curModeWidth,//int WidthRes,  m_curModeWidth
					    ClpPlaneLeft, 
					    ClpPlaneBottom,
                        ClpPlaneRight, 
					    ClpPlaneTop,
                        1.0f, 
					    targetLocalPosition,
					    fAspect)){//in view
                        

                  if( 32 == surfaceBBP ){
                      DWORD* pDstData32 = (DWORD*)pBytes;
					  
                      *pixelValue = pDstData32[(surfacePosY*(pitch/sizeof(DWORD)))+surfacePosX];
				  }
                  else{
                      WORD* pDstData32 = (WORD*)pBytes;
		
					  //*pixelValue = (DWORD)pBytes[(surfacePosY*pitch)+(surfacePosX*2)];
					  //*pixelValue += (DWORD)pBytes[(((surfacePosY*pitch)+(surfacePosX*2))+1)];
                      *pixelValue = pDstData32[(((WORD)surfacePosY)*(pitch/sizeof(WORD)))+((WORD)surfacePosX)];
				  }

				 //m_gStatus->m_systemReadout->AddLine(surfacePosX);
	             //m_gStatus->m_systemReadout->AddLine(surfacePosY);
                 return TRUE;
		}//end in view
	    //m_gStatus->m_systemReadout->AddLine("OutOfView");
        return FALSE;  
}
//------------------------------------------------------------------------------------//
BOOL EngineXWin::EvaluateAndRenderBaseCharonas(CViewportObj* mptLoc, 
					                           D3DVECTOR locationVect)
{
     return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL EngineXWin::EvaluateAndRenderCharonaOverlays(CViewportObj* mptLoc, 
					                              D3DVECTOR locationVect)
{

	return TRUE;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTXPurgeTextures() 
{
	 CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization  
	static char BASED_CODE filter[] = "TXB ENGIN|*.TXB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "EXG", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   m_gStatus->SaveTextureBank(fileName,1,2048);
	 }
    SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL EngineXWin::SphereInFovStatus(double radius,
							       double fov,
								   D3DMATRIX cameraMatrix,
								   D3DVECTOR wldPosition)
{	
	    D3DMATRIX projMatrix;
	    m_gStatus->m_dxRenderer->m_pd3dDevice->GetTransform(D3DTS_PROJECTION ,&projMatrix);
		
		double fovRadY = projMatrix._11*.5f;
        double fovRadX = projMatrix._22*.5f;


 D3DVECTOR locateMatrix = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(cameraMatrix,wldPosition);
	
 if(locateMatrix.z == 0.0)
    return TRUE;
 
 if(m_gStatus->m_matLib.BoundingSphereCheck(radius,1.0f,wldPosition.x,wldPosition.y,wldPosition.z,
	                                       cameraMatrix._41,cameraMatrix._42,cameraMatrix._43))
	return TRUE;
 //if(locateMatrix.z > 0.0f -radius)
 //	 return FALSE;//behind you
 
 double positionAngle = asin((fabs(locateMatrix.x)-radius)/fabs(locateMatrix.z));//* 57.3;
 if(fovRadX < positionAngle)//out of fov
    return FALSE;
 positionAngle = asin((fabs(locateMatrix.y)-radius)/fabs(locateMatrix.z));//* 57.3;
 if(fovRadY < positionAngle)//out of fov
    return FALSE;

 

return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::StampLog(long stampTest2)
{
	   /*GL_lastAdjust = m_gStatus->m_timeAdjust;
       float timePassed = (float)timeGetTime()-stampTest2;
	   if(timePassed == 0)
		  timePassed = 1.0f;
	      GL_exactFPS = 1000.0f/timePassed;
	      
	   float adjust = GL_exactFPS;*/

	   /*while(m_forceSpeed <= adjust){
		   float timePassed = timeGetTime()-stampTest2;
	       if(timePassed == 0)
		      timePassed = 1;
	          adjust = 1000/timePassed;
	   }*/

	   /*if(fps_cap <= adjust){//plenty of speed
		  m_gStatus->m_systemReadout->AddLine("non");
		  m_gStatus->m_timeAdjust = 1.0f;
	      while(fps_cap <= adjust){
		   //m_gStatus->m_timeAdjust = 1.0f;
		   float timePassed = timeGetTime()-stampTest2;
	       if(timePassed == 0)
		      timePassed = 1;
	          adjust = 1000.0/timePassed;
		  }
	   }//end plenty of speed
	   else{//need to caclulate adjust factor
           if(m_gStatus->m_timeAdjust == 0.0f)
			  m_gStatus->m_timeAdjust = 1.0f;
		      m_gStatus->m_timeAdjust = fps_cap/((double)adjust);
		   if(m_gStatus->m_timeAdjust > 20.0f)
			  m_gStatus->m_timeAdjust = 1.0f;
		   m_gStatus->m_systemReadout->AddLine("adjust ;)))))))))");
	   m_gStatus->m_systemReadout->AddLine(m_gStatus->m_timeAdjust);
	   }//end need to caclulate adjust factor   */
	   
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL EngineXWin::RenderTranslucentMirroredPolys(D3DVECTOR locationVect,BOOL mirrorEnable)
{
    m_gStatus->m_renderStateObject->SetWireFillState(FALSE,m_gStatus->m_dxRenderer->m_pd3dDevice);
    
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int1,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int2,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int3,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int4,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int5,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int6,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int7,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);

	BOOL matrixDone = FALSE;
	
    
	for(POSITION posLoc = m_gStatus->m_translucentMirrorPolyList->GetTailPosition();posLoc != NULL;)
	{
        CTranslucentPolyObj * mTpoly = (CTranslucentPolyObj *)m_gStatus->m_translucentMirrorPolyList->GetPrev(posLoc);
		if(!mTpoly->m_meshReference){//poly basis  
		  
		}//end poly basis
		else{
		   if(mTpoly->m_meshReference->m_visibilityTrack > 0)
              m_gStatus->HighLevelMeshRender(mTpoly->m_meshReference,locationVect,mirrorEnable);
		}
	}
	return TRUE;
}

//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//

///////////////////////////////////////////////////////////////////////////////
// Class:		EngineXWin
// Function:	InListEntityDefcfgs(int)
// Description:	Display a selected entity's deformable meshes.
///////////////////////////////////////////////////////////////////////////////

void 
EngineXWin::InListEntityDefcfgs(int sel)
{
	int					iIndex = 0,
						iLoop = 0;
	POSITION			posMovement;

	iIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList, LB_GETCURSEL, 0, 0);
	if(iIndex == -1) return;

	posMovement = m_gStatus->m_entObjRuntime->FindIndex(iIndex);
	if(!posMovement) return;

	// Get a pointer to the selected entity
	CMovementObj *movObj = (CMovementObj*)m_gStatus->m_entObjRuntime->GetAt(posMovement);		
	m_entityDlgBar.SendDlgItemMessage(IDC_LIST_defSwapOptions, LB_RESETCONTENT, 0, 0);

	// Populate "Dim Cfgs Testing" listbox with the selected entity's deformable meshes
	for(iLoop = 0; iLoop < movObj->List_skeletalObject->m_deformableMeshArrayCount; iLoop++)
	{
		m_entityDlgBar.SendDlgItemMessage(
			IDC_LIST_defSwapOptions,
			LB_ADDSTRING,
			0,
			//(LPARAM)(LPCTSTR)movObj->List_skeletalObject->m_deformableMeshArrayV2[iLoop].m_defMeshVisualUnit->m_dimName);
			(LPARAM)(LPCTSTR)movObj->List_skeletalObject->m_deformableMeshArrayV2[iLoop].m_defMeshVisualUnit->DimName());
	}
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTdefSwapNext() 
{
    int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
    if(runtimeIndex == -1){
       MessageBox("Must select an entity object on the entity runtime interface");
	   return;
	}

    int dimIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_defSwapOptions,LB_GETCURSEL ,0,0);
    if(dimIndex == -1){
       MessageBox("Must select a dim...");
	   return;
	}

	
	POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
    if(!posLoc)
		return;
	    CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);

        BOOL result = m_menuManagement->IndexUpDim(
													ptrLoc->List_skeletalObject,
													dimIndex,
													m_gStatus->m_entObjList,
													ptrLoc->List_refModel);
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTdimPrev() 
{
	int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
    if(runtimeIndex == -1){
       MessageBox("Must select an entity object on the entity runtime interface");
	   return;
	}

    int dimIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_defSwapOptions,LB_GETCURSEL ,0,0);
    if(dimIndex == -1){
       MessageBox("Must select a dim...");
	   return;
	}

	
	POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
    if(!posLoc)
		return;
	    CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);

        BOOL result = m_menuManagement->IndexDownDim(
			            ptrLoc->List_skeletalObject,
					    dimIndex,
				        m_gStatus->m_entObjList,
					    ptrLoc->List_refModel);
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::UnionVertexesPositionalyByTolerance(float tolerance,int group)
{
     for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//begin world object loop
         CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
         if(!ptrLoc->m_meshObject)
			 continue;

         ABVERTEX * vArray = NULL;
         int vCount = 0;
		 ptrLoc->m_meshObject->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);

         if(ptrLoc->List_groupNumber == group)
		 {//select by group
            for(POSITION scnPos = m_gStatus->m_wldObjList->GetHeadPosition();scnPos != NULL;)
			{//begin secondary world object loop
                CWldObject * chkLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(scnPos);	
                if(!chkLoc->m_meshObject)
			        continue;
				ABVERTEX * chkvArray = NULL;
                int chkvCount = 0;
				chkLoc->m_meshObject->m_abVertexArray.GetABVERTEXStyle(&chkvArray,&chkvCount);
				
				for(int pvLoop=0;pvLoop<ptrLoc->m_meshObject->m_abVertexArray.m_vertexCount;pvLoop++)
				{//primary mesh loop
                   for(int scndLoop=0;scndLoop<chkLoc->m_meshObject->m_abVertexArray.m_vertexCount;scndLoop++)
				   {//secondary mesh loop
					  
                      float dist = m_gStatus->m_matLib.MagnitudeSquared(vArray[pvLoop].x,
						                                               vArray[pvLoop].y,
																	   vArray[pvLoop].z,
																	   chkvArray[scndLoop].x,
																	   chkvArray[scndLoop].y,
																	   chkvArray[scndLoop].z);
                     if(dist < tolerance)
					 {//colapse
                        chkvArray[scndLoop].x = vArray[pvLoop].x;
					    chkvArray[scndLoop].y = vArray[pvLoop].y;
					    chkvArray[scndLoop].z = vArray[pvLoop].z;
					 }//end colapse
				   }//end secondary mesh loop
				}//end primary mesh loop

				if(chkvArray)
				{
                   chkLoc->m_meshObject->m_abVertexArray.SetABVERTEXStyle(NULL,chkvArray,chkvCount);
                   delete [] chkvArray;
				   chkvArray = 0;
				}
				


				chkLoc->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
				int tracer = 0;
				if(chkLoc->m_lodVisualList)
				if(ptrLoc->m_lodVisualList)
				for(POSITION lPos = ptrLoc->m_lodVisualList->GetHeadPosition();lPos != NULL;)
				{//primary LOD loop
                 CMeshLODObject * primaryLodPtr = (CMeshLODObject *)ptrLoc->m_lodVisualList->GetNext(lPos);
                  if(!primaryLodPtr->m_meshLodLevel)
					 continue;
                  ABVERTEX * subvArray = NULL;
                  int supvCount = 0;
		          primaryLodPtr->m_meshLodLevel->m_abVertexArray.GetABVERTEXStyle(&subvArray,&supvCount);


				  POSITION scndPos = chkLoc->m_lodVisualList->FindIndex(tracer);
				  if(scndPos){//secondary LOD loop
                   CMeshLODObject * secondaryLodPtr = (CMeshLODObject *)chkLoc->m_lodVisualList->GetAt(scndPos);
				   if(!secondaryLodPtr->m_meshLodLevel)
					  continue;

                   ABVERTEX * subscndryvArray = NULL;
                   int supvscndryCount = 0;
		           secondaryLodPtr->m_meshLodLevel->m_abVertexArray.GetABVERTEXStyle(&subscndryvArray,&supvscndryCount);

				   for(int pvLoop=0;pvLoop<primaryLodPtr->m_meshLodLevel->m_abVertexArray.m_vertexCount;pvLoop++)
				   {//primary mesh loop
                      for(int scndLoop=0;scndLoop<secondaryLodPtr->m_meshLodLevel->m_abVertexArray.m_vertexCount;scndLoop++)
					  {//secondary mesh loop
					  
                         float dist = m_gStatus->m_matLib.MagnitudeSquared(subvArray[pvLoop].x,
						                                                  subvArray[pvLoop].y,
																	      subvArray[pvLoop].z,
																	      subscndryvArray[scndLoop].x,
																	      subscndryvArray[scndLoop].y,
																	      subscndryvArray[scndLoop].z);
                         if(dist < tolerance)
						 {//colapse
                           subscndryvArray[scndLoop].x = subvArray[pvLoop].x;
					       subscndryvArray[scndLoop].y = subvArray[pvLoop].y;
					       subscndryvArray[scndLoop].z = subvArray[pvLoop].z;
						 }//end colapse
					  }//end secondary mesh loop
				   }//end primary mesh loop
                   if(subscndryvArray)
				   {
                     secondaryLodPtr->m_meshLodLevel->m_abVertexArray.SetABVERTEXStyle(NULL,subscndryvArray,supvscndryCount);
                     delete [] subscndryvArray;
				     subscndryvArray = 0;
				   }

				   secondaryLodPtr->m_meshLodLevel->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
				  }//secondary found ok
				  tracer++;
				  if(subvArray)
				  {
                      delete [] subvArray;
		              subvArray = 0;
				  }
				}//end primary LOD loop
			}//end secondary world object loop
		 }//end select by group
       if(vArray)
	   {
          delete [] vArray;
		  vArray = 0;
	   }
	 }//end world object loop
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
/*void EngineXWin::OnBTNAIEditAITree() 
{
#ifndef _ClientOutput
    int treeIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_WebTrees,LB_GETCURSEL ,0,0);
	if(treeIndex == -1){
		MessageBox("Must select a tree");
		return;
	}
	POSITION posIndex = m_gStatus->GL_aiWebDatabase->FindIndex(treeIndex);
	 if(posIndex){
        CAIWebObj * webObj = (CAIWebObj *)m_gStatus->GL_aiWebDatabase->GetAt(posIndex);
	    CAIPathDlg dlg;
        POSITION ePos = m_gStatus->m_entObjRuntime->FindIndex(0);
	    if(ePos)
		{
           CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(ePos);
           ptrLoc->List_baseFrame->GetPosition(NULL,&dlg.m_charCurrentPosition);
		}
		else{
           dlg.m_charCurrentPosition.x = 0.0f;
		   dlg.m_charCurrentPosition.y = 0.0f;
		   dlg.m_charCurrentPosition.z = 0.0f;
		}
	    
		dlg.m_aiObject = webObj;
        dlg.DoModal();
		webObj->m_name = dlg.m_webName;
		AITreeInList(treeIndex);
	 }
#endif
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNSystemGoingDown() 
{
  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL EngineXWin::TargetNotVisible(D3DVECTOR eyePoint,D3DVECTOR target)
{
D3DVECTOR intersection,latestNormal;
int objectHit = 0;
int polysExactCalculated = 0;
int polysConsideredCalculated = 0;
BOOL result = m_gStatus->m_collisionBase->CollisionSegmentSI(
					            eyePoint,
					            target,
					            &intersection.x,
					            &intersection.y,
					            &intersection.z,
					            &polysExactCalculated,
					            &polysConsideredCalculated,
					            &latestNormal,
					            &objectHit,
								100.0f,
								TRUE
					            );
m_gStatus->m_actualPolysCalculated += polysExactCalculated;
m_gStatus->m_collisionPolysInUse   += polysConsideredCalculated;
return result;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTAddItem() 
{
#ifndef _ClientOutput
    int indexLoc=0;
   //get selected index
    HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    temp.Replace("TRC_","");
	CMovementObj * mainPtr = m_gStatus->m_entObjList->GetByID(atoi(temp));
	if(!mainPtr){
       MessageBox("Must select a entity base object");
	   return;
	}

	 if(!mainPtr->m_itemDB)
	 mainPtr->m_itemDB = new CItemObjList();
     CItemObj * itemObj = new CItemObj();
	 itemObj->m_attachmentRefList = new CAttachmentRefList();

     CAddItemDlg dlg;
	 dlg.m_skillDatabaseRef = m_gStatus->m_skillDatabase;
	 dlg.m_itemRef = itemObj;
	 dlg.m_armedInventoryListRef = m_gStatus->m_itemManager->m_visuals;
	 dlg.m_currentElements = m_gStatus->m_elementDB;
	 dlg.m_enitityDB = m_gStatus->m_entObjList;
	 if(dlg.DoModal()==IDOK){
		mainPtr->m_itemDB->AddTail(itemObj);
	 }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTItemTest() 
{
 #ifndef _ClientOutput
int entIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
	   MessageBox("Must also select a runtime entity");
	   return;
	}

    POSITION posloc = m_gStatus->m_entObjRuntime->FindIndex(entIndex);
	if(!posloc)
		return;
        CMovementObj * mainPtr = (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(posloc);

        CMovementObj * DBPtr = m_gStatus->m_entObjList->GetByID(mainPtr->m_uniqueId);
		if(!DBPtr)
			return;

        if(!DBPtr->m_itemDB){
            MessageBox("item list does not exist");
            return;
		}

		CEntityItemTestDlg dlg;
		dlg.m_itemObjectListRef = DBPtr->m_itemDB;
        if(dlg.DoModal()==IDOK)
		{
			
			POSITION itemPos = DBPtr->m_itemDB->FindIndex(dlg.m_selection);
			if(itemPos){
               CItemObj * itemPtr = (CItemObj *)DBPtr->m_itemDB->GetAt(itemPos);
			   m_gStatus->ArmEntityItemSys(mainPtr,itemPtr->m_globalID,0);
			   //m_gStatus->m_itemManager->ArmEntity(itemPtr->m_globalID,mainPtr,DBPtr,m_gStatus->m_3dDevice);
			}
		}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTDeleteItem() 
{
 #ifndef _ClientOutput
	int indexLoc=0;
	BOOL foundIt = FALSE; 
	HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
	HTREEITEM item = TreeView_GetSelection(treeHwnd);
	CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
	temp.Replace("TRC_","");
	CMovementObj * mainPtr = m_gStatus->m_entObjList->GetByID(atoi(temp));
	if(!mainPtr)
	{
		MessageBox("Must select a entity base object");
		return;
	}

	if(!mainPtr->m_itemDB)
	{
		MessageBox("item list does not exist");
		return;
	}

	CEntityItemTestDlg dlg;
	dlg.m_selection = 0;
	dlg.m_itemObjectListRef = mainPtr->m_itemDB;
	if(dlg.DoModal()==IDOK)
	{
		POSITION posLoc = mainPtr->m_itemDB->FindIndex(dlg.m_selection);
		if(posLoc)
		{
			if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
				return;
			CItemObj * itemObj = (CItemObj *)mainPtr->m_itemDB->GetAt(posLoc);
			itemObj->SafeDelete();
			delete itemObj;
			itemObj = 0;
			mainPtr->m_itemDB->RemoveAt(posLoc);
		}
	}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTDisarmItem() 
{
	/*
#ifndef _ClientOutput
int entIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	if(entIndex == -1){
	   MessageBox("Must also select a runtime entity");
	   return;
	}

    POSITION posloc = m_gStatus->m_entObjRuntime->FindIndex(entIndex);
	if(!posloc)
		return;
        CMovementObj * mainPtr = (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(posloc);

        if(!mainPtr->m_itemDB){
            MessageBox("item list does not exist");
            return;
		}

		CEntityItemTestDlg dlg;
		dlg.m_selection = 0;
		dlg.m_itemObjectListRef = mainPtr->m_itemDB;
        if(dlg.DoModal()==IDOK)
		{
           //mainPtr->DisarmItem(dlg.m_selection,TRUE);
           POSITION itemPos = mainPtr->m_itemDB->FindIndex(dlg.m_selection);
			if(itemPos){
               CItemObj * itemPtr = (CItemObj *)mainPtr->m_itemDB->GetAt(itemPos);    
			   m_gStatus->DisarmEntityItemSys(mainPtr,itemPtr->m_globalID,0);
			}
           //mainPtr->ArmItem(dlg.m_selection,m_gStatus->m_dirSnd,DBPtr);m_userControls->DisarmEntityItemSys(CMovementObj * mainPtr,int itemID)

		}
#endif
		*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTEditItem() 
{
#ifndef _ClientOutput
int indexLoc=0;
    BOOL foundIt = FALSE; 
    HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    temp.Replace("TRC_","");
	CMovementObj * mainPtr = m_gStatus->m_entObjList->GetByID(atoi(temp));
	if(!mainPtr){
       MessageBox("Must select a entity base object");
	   return;
	}

        if(!mainPtr->m_itemDB){
            MessageBox("item list does not exist");
            return;
		}

		int selectionCur = 0;
		while(1)
		{
		 CEntityItemTestDlg dlg;
		 dlg.m_itemObjectListRef = mainPtr->m_itemDB;
		 dlg.m_selection = selectionCur;
         if(dlg.DoModal()==IDOK){
          
		   selectionCur = dlg.m_selection;
		   POSITION posLoc = mainPtr->m_itemDB->FindIndex(dlg.m_selection);
		   if(posLoc){
              CItemObj * itemObj = (CItemObj *)mainPtr->m_itemDB->GetAt(posLoc);
              
              if(!mainPtr->m_itemDB)
	              mainPtr->m_itemDB = new CItemObjList();
			     if(!itemObj->m_attachmentRefList)
	             itemObj->m_attachmentRefList = new CAttachmentRefList();
                 CAddItemDlg adlg;
				 adlg.m_skillDatabaseRef = m_gStatus->m_skillDatabase;
	             adlg.m_itemRef = itemObj;
	             adlg.m_armedInventoryListRef = m_gStatus->m_itemManager->m_visuals;
	             adlg.m_enitityDB = m_gStatus->m_entObjList;
				 adlg.m_currentElements = m_gStatus->m_elementDB;
	             if(adlg.DoModal()==IDOK){
		            
				 
				 }
				 
		   
		   }
		 }
		 else
		   break;

		}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL EngineXWin::BrushObjectCopy(D3DMATRIX newPosition,
								 D3DVECTOR rotation,
								 D3DVECTOR offset,
								 D3DVECTOR scale)
{
 int baseObjectIndex = -1;
 HWND treeHwnd = NULL;
 m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
 HTREEITEM item = TreeView_GetSelection(treeHwnd); 
 CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);
 int trace = 0;
 CWldObject * pTemp = NULL;
  for(POSITION sPos = m_gStatus->m_wldObjList->GetHeadPosition();sPos != NULL;){//Begin world object loop
    pTemp = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(sPos);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(pTemp->List_identity)));
	if(tracename == textLoc){
       baseObjectIndex = trace;
	   break;
	}
	trace++;
  }
  if(baseObjectIndex == -1){
     MessageBox("Must select a object");
     return FALSE;
  }

  CWldObject * m_wldObject = NULL;
  return BrushObjectCopyLowLevel(pTemp,
								 newPosition,
								 rotation,
								 offset,
								 scale,
								 &m_wldObject,
								 pTemp->m_meshObject->m_meshCenter);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL EngineXWin::BrushObjectCopyLowLevel(CWldObject * ptrLoc,
										 D3DMATRIX newPosition,
								         D3DVECTOR rotation,
								         D3DVECTOR offset,
								         D3DVECTOR scale,
										 CWldObject ** newPtrRef,
										 D3DVECTOR meshCenter)
{
            D3DMATRIX currentMatrix;
	        m_gStatus->m_matLib.InitMat(currentMatrix.m);

			currentMatrix._41 = meshCenter.x;
			currentMatrix._42 = meshCenter.y;// - (ptrLoc->m_meshObject->m_meshCenter.y - ptrLoc->m_meshObject->List_boundingBox.minY);
			currentMatrix._43 = meshCenter.z;

            CWldObject * newPtr = new CWldObject((m_gStatus->GlobalTraceNumber++));
            newPtr->List_groupNumber = ptrLoc->List_groupNumber;
			newPtr->List_popoutRadius = ptrLoc->List_popoutRadius;
			newPtr->m_fileNameRef = ptrLoc->m_fileNameRef;
	        newPtr->List_triggerList = new CTriggerObjectList();

            ptrLoc->m_meshObject->Clone(&newPtr->m_meshObject,m_gStatus->m_dxRenderer->m_pd3dDevice);
            CopyMemory(&newPtr->List_popoutBox,&ptrLoc->List_popoutBox,sizeof(BNDBOX));

        D3DVECTOR translation;
		translation.x = ((newPosition._41 - currentMatrix._41)+offset.x);
		translation.y = ((newPosition._42 - currentMatrix._42)+offset.y);
		translation.z = ((newPosition._43 - currentMatrix._43)+offset.z);

		newPtr->Rotate(rotation,meshCenter);
		newPtr->Scale(scale,meshCenter);
		newPtr->Move(translation,meshCenter);
		newPtr->m_meshObject->GetBox();
		newPtr->m_meshObject->GetCenterRadius();
	
		if( ptrLoc->m_lodVisualList ){//copy lod structure
            ptrLoc->m_lodVisualList->Clone(&newPtr->m_lodVisualList,m_gStatus->m_dxRenderer->m_pd3dDevice);
            for(POSITION lPos = newPtr->m_lodVisualList->GetHeadPosition();lPos != NULL;)
			{
                CMeshLODObject * lodObj = (CMeshLODObject *) newPtr->m_lodVisualList->GetNext(lPos);

				lodObj->m_meshLodLevel->Rotate(rotation,meshCenter);
				lodObj->m_meshLodLevel->Scale(scale,meshCenter);
                lodObj->m_meshLodLevel->Move(translation,meshCenter);
                lodObj->m_meshLodLevel->GetBox();
				lodObj->m_meshLodLevel->GetCenterRadius();
			   
				if(newPtr->m_lodVisualList->m_lodType == 1){//must move lod boxes
                   
				   //lodObj->m_boxOption.maxX = lodObj->m_boxOption.maxX * scale.x;
                   //lodObj->m_boxOption.maxY = lodObj->m_boxOption.maxY * scale.y;
				   //lodObj->m_boxOption.maxZ = lodObj->m_boxOption.maxZ * scale.z;
                   //lodObj->m_boxOption.minX = lodObj->m_boxOption.minX * scale.x;
                   //lodObj->m_boxOption.minY = lodObj->m_boxOption.minY * scale.y;
				   //lodObj->m_boxOption.minZ = lodObj->m_boxOption.minZ * scale.z;
				   //translate
				   lodObj->m_boxOption.maxX += translation.x;
                   lodObj->m_boxOption.maxY += translation.y;
				   lodObj->m_boxOption.maxZ += translation.z;
                   lodObj->m_boxOption.minX += translation.x;
                   lodObj->m_boxOption.minY += translation.y;
				   lodObj->m_boxOption.minZ += translation.z;
				}//end must move lod boxes
			}
			newPtr->m_lodVisualList->Initialize(m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_cgGlobalShaderDB);
		}//end copy lod structure

		if(m_gStatus->InitWorldObject(newPtr)==TRUE){
		   m_gStatus->m_wldObjList->AddTail(newPtr);	
		   m_gStatus->m_worldListTemp->AddTail(newPtr);
		   newPtr->m_meshObject->GetBox();
		   *newPtrRef = newPtr;
		   HTREEITEM Root = AddTreeRoot(operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(newPtr->List_identity))),IDC_OBJECT_TREE,&m_dlgBarObject);   
	        AddTreeChild(newPtr->m_fileNameRef,IDC_OBJECT_TREE,&m_dlgBarObject,Root);  
		}
		else
		  return FALSE;

	    return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOBJClone() 
{
#ifndef _ClientOutput
   
HWND treeHwnd = NULL;
m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd); 
CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc)
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
       static char BASED_CODE filter[] = "Mesh Wld Obj Files (*.WLE)|*.WLE|Wavefront OBJ (*.obj)|*.obj|All Files (*.*)|*.*||";
       CFileDialog opendialog( FALSE, "WLE", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, this );
       if ( opendialog.DoModal() == IDOK )
	   {
	       CString ext = opendialog.GetFileExt();
	       ext.MakeLower();
	       if(ext == "obj")
	       {
	           BOOL exported = FALSE;
	           if(ptrLoc->m_meshObject && ptrLoc->m_meshObject->m_abVertexArray.m_vertexCount > 0)
	               exported = ptrLoc->m_meshObject->ExportToObj(opendialog.GetPathName());
	           else if(ptrLoc->m_node && m_gStatus->m_libraryReferenceDB)
	           {
	               CReferenceObj * refObj = m_gStatus->m_libraryReferenceDB->GetByIndex(ptrLoc->m_node->m_libraryReference);
	               if(refObj && refObj->m_meshObject)
	                   exported = refObj->m_meshObject->ExportToObj(opendialog.GetPathName());
	           }
	           if(!exported)
	               MessageBox("Failed to export Wavefront OBJ file.");
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
           the_out_Archive << ptrLoc;
           the_out_Archive.Close();
          fl.Close();
	   }
	   SetCurrentDirectory(MainDir);	
	   return;
  }//end Begin world object loop
}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOBJTransformation() 
{
#ifndef _ClientOutput
 int baseObjectIndex = -1;
 CWldObject * pTemp = NULL;
 HWND treeHwnd = NULL;
 m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
 HTREEITEM item = TreeView_GetSelection(treeHwnd); 
 CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);
 int trace = 0;
  for(POSITION sPos = m_gStatus->m_wldObjList->GetHeadPosition();sPos != NULL;){//Begin world object loop
    pTemp = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(sPos);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(pTemp->List_identity)));
	if(tracename == textLoc){
       baseObjectIndex = trace;
	   break;
	}
	trace++;
  }
  if(baseObjectIndex == -1){
     MessageBox("Must select a object");
     return;
  }
	

   CTransformationDlg dlg;
   dlg.m_rotX    = 0.0f;
   dlg.m_rotY    = 0.0f;
   dlg.m_rotZ    = 0.0f;
   dlg.m_transX  = 0.0f;
   dlg.m_transY  = 0.0f;
   dlg.m_transZ  = 0.0f;
   dlg.m_scaleX  = 1.0f;
   dlg.m_scaleY  = 1.0f;
   dlg.m_scaleZ  = 1.0f;
   if(dlg.DoModal()==IDOK){
	  D3DVECTOR offset,rotation,scale;
	  offset.x = dlg.m_transX;
	  offset.y = dlg.m_transY;
	  offset.z = dlg.m_transZ;
	  rotation.x = dlg.m_rotX;
	  rotation.y = dlg.m_rotY;
	  rotation.z = dlg.m_rotZ;
      scale.x = dlg.m_scaleX;
	  scale.y = dlg.m_scaleY;
	  scale.z = dlg.m_scaleZ;
	  pTemp->m_meshObject->Rotate(rotation,pTemp->m_meshObject->m_meshCenter);
	  pTemp->m_meshObject->Scale(scale,pTemp->m_meshObject->m_meshCenter);
	  pTemp->m_meshObject->Move(offset,pTemp->m_meshObject->m_meshCenter);
	  pTemp->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
	  pTemp->m_meshObject->GetBox();
	  if( pTemp->m_lodVisualList ){//copy lod structure
            for(POSITION lPos = pTemp->m_lodVisualList->GetHeadPosition();lPos != NULL;)
			{
                CMeshLODObject * lodObj = (CMeshLODObject *) pTemp->m_lodVisualList->GetNext(lPos);
                lodObj->m_meshLodLevel->Rotate(rotation,pTemp->m_meshObject->m_meshCenter);
				lodObj->m_meshLodLevel->Scale(scale,pTemp->m_meshObject->m_meshCenter);
				lodObj->m_meshLodLevel->Move(offset,pTemp->m_meshObject->m_meshCenter);
                lodObj->m_meshLodLevel->GetBox();
				
			}
			pTemp->m_lodVisualList->Initialize(m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_cgGlobalShaderDB);
	  }//end copy lod structure
	  MessageBox("Complete");
   }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BNDBOX EngineXWin::BoundingBoxGroupSet(int group,
									   float maxBumpX,
									   float maxBumpY,
									   float maxBumpZ,
									   float minBumpX,
									   float minBumpY,
									   float minBumpZ)
{
    BNDBOX bndingBx;
    bndingBx.max.x = 0.0f;
    bndingBx.max.y = 0.0f;
    bndingBx.max.z = 0.0f;
    bndingBx.min.x = 0.0f;
    bndingBx.min.y = 0.0f;
    bndingBx.min.z = 0.0f;
	BOOL initialized = FALSE;
	POSITION sPos;
    for(sPos = m_gStatus->m_wldObjList->GetHeadPosition();sPos != NULL;)
	{//Begin world object loop
        CWldObject * wldPtr = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(sPos);
		if(wldPtr->List_groupNumber == group)
		{//found group
	       ABVERTEX * vArray = NULL;
		   int vCount = 0;
		   wldPtr->m_meshObject->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);
			
		   for(int vLoop=0;vLoop<wldPtr->m_meshObject->m_abVertexArray.m_vertexCount;vLoop++)
		   {//1
               if(!initialized){//set
                   bndingBx.max.x = vArray[vLoop].x;
				   bndingBx.max.y = vArray[vLoop].y;
				   bndingBx.max.z = vArray[vLoop].z;
				   bndingBx.min.x = vArray[vLoop].x;
				   bndingBx.min.y = vArray[vLoop].y;
				   bndingBx.min.z = vArray[vLoop].z;
			   }//end set
               //check for new max
               if(bndingBx.max.x < vArray[vLoop].x)
				  bndingBx.max.x = vArray[vLoop].x;

			   if(bndingBx.max.y < vArray[vLoop].y)
				  bndingBx.max.y = vArray[vLoop].y;

			   if(bndingBx.max.z < vArray[vLoop].z)
				  bndingBx.max.z = vArray[vLoop].z;
               //check for new min
			   if(bndingBx.min.x > vArray[vLoop].x)
				  bndingBx.min.x = vArray[vLoop].x;

               if(bndingBx.min.y > vArray[vLoop].y)
				  bndingBx.min.y = vArray[vLoop].y;

			   if(bndingBx.min.z > vArray[vLoop].z)
				  bndingBx.min.z = vArray[vLoop].z;

			initialized = TRUE;
		   }//end 1

		   if(vArray)
		   {
              delete [] vArray;
			  vArray = 0;
		   }

		}//end found group
	}//end world object loop
    
	
    bndingBx.max.x += maxBumpX;
	bndingBx.max.y += maxBumpY;
	bndingBx.max.z += maxBumpZ;
    bndingBx.min.x += minBumpX;
	bndingBx.min.y += minBumpY;
	bndingBx.min.z += minBumpZ;

    //set to group objects
    for(sPos = m_gStatus->m_wldObjList->GetHeadPosition();sPos != NULL;)
	{//Begin world object loop
        CWldObject * wldPtr = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(sPos);
		if(wldPtr->List_groupNumber == group)
        CopyMemory(&wldPtr->List_popoutBox,&bndingBx,sizeof(BNDBOX));
	}//end world object loop

	return bndingBx;
}
//----------------------------------------//
//----------------------------------------//
//----------------------------------------//
void EngineXWin::OnBTNOBJBoundingGroupSet() 
{
#ifndef _ClientOutput
HWND treeHwnd = NULL;
m_dlgBarObject.GetDlgItem( IDC_OBJECT_TREE, &treeHwnd);
HTREEITEM item = TreeView_GetSelection(treeHwnd); 
CString textLoc = GetTreeItemText(IDC_OBJECT_TREE,&m_dlgBarObject,item,10);
  for(POSITION posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
	CString tracename = operator+("TRC_",(m_gStatus->m_cabLib.ConvToString(ptrLoc->List_identity)));
	if(tracename == textLoc)
	{
	   ptrLoc->m_meshObject->m_abVertexArray.GenerateTangentsPerfect(m_gStatus->m_dxRenderer->m_pd3dDevice,FALSE,0);
	   ptrLoc->m_meshObject->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_cgGlobalShaderDB->GetByIndex(ptrLoc->m_meshObject->List_materialObject->m_optionalShaderIndex-1));
	return;
	}
  }//end Begin world object loop
  //m_gStatus->m_worldRenderList->RemoveAll();
  MessageBox("Object Not Found");


	/*CGroupBoundingSetDlg dlg;
    dlg.m_group       = 0;
	dlg.m_XnegBump    = 0.0f;
	dlg.m_XposBump    = 0.0f;
	dlg.m_YnegBump    = 0.0f;
	dlg.m_YposBump    = 0.0f;
	dlg.m_ZnegBump    = 0.0f;
	dlg.m_ZposBump    = 0.0f;

	if(dlg.DoModal()==IDOK)
	{
              BoundingBoxGroupSet(dlg.m_group,
								  dlg.m_XposBump,
								  dlg.m_YposBump,
								  dlg.m_ZposBump,
							      dlg.m_XnegBump,
								  dlg.m_YnegBump,
								  dlg.m_ZnegBump);
	}*/
#endif
}
//----------------------------------------//
//----------------------------------------//
//----------------------------------------//
void EngineXWin::OnBTNOBJLibraryAccess() 
{
#ifndef _ClientOutput
	//If they don't have a group selected, return
	int groupIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box, LB_GETCURSEL, 0, 0);
	if(groupIndex == -1)
	{
		MessageBox("You must select a group first.");
		return;
	}
	else
	{
		POSITION posLoc = m_gStatus->m_worldGroupList->FindIndex(groupIndex);
		groupIndex=0;
		if(posLoc)
		{
			CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetAt(posLoc);
			groupIndex = wldPtr->List_groupInt;
		}
	}


     //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

     static char BASED_CODE filter[] = "World Obj Export Files|*.WLE|All Files (*.*)|*.*||";
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
	   CWldObject * wleNewPtr = NULL;
	   the_in_Archive >> wleNewPtr;
	   the_in_Archive.Close();
       fl.Close();
	   wleNewPtr->List_groupNumber = groupIndex;
	   wleNewPtr->List_identity = m_gStatus->GlobalTraceNumber++;
	   m_gStatus->m_wldObjList->AddTail(wleNewPtr);
	 }
     SetCurrentDirectory(MainDir);
#endif
}
//----------------------------------------//
//----------------------------------------//
//----------------------------------------//
/*
void EngineXWin::OnBTNENVTimeSystem() 
{
 #ifndef _ClientOutput  
	CTimeSystemDlg dlg;
	dlg.g_pd3dDeviceRef      = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.m_enviormentObjRef   = m_gStatus->m_environmentPtr;
	dlg.m_textureDataBaseRef = m_gStatus->m_textureDataBase;
	dlg.m_directSoundRef     = m_gStatus->m_dirSnd;

    if(!m_gStatus->m_environmentPtr->m_enviormentTimeSystem)
	{
        m_gStatus->m_environmentPtr->m_enviormentTimeSystem = new CTimeSystemObj();
	}

    if(dlg.DoModal()==IDOK)
	{
	   if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState)
	   {
          m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->SafeDelete();
          delete m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState;
          m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState = 0;
	   }

	   if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_stormPtr)
	   {
          m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_stormPtr->SafeDelete();
          delete m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_stormPtr;
          m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_stormPtr = 0;
	   }
	   //m_gStatus->m_environmentPtr->StartSystem(GetTickCount(),m_gStatus->m_dirSnd);
       //m_gStatus->m_environmentPtr->InitSystem(m_gStatus->m_dirSnd);
	   if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem)
		  m_gStatus->m_environmentPtr->m_enviormentTimeSystem->StartSystem(GetTickCount(),0);
	   

         


	}
#endif
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNMISAddSkeletalMissile() 
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
	 newMissilePtr->m_skeletalVisual->InitBoneSystem(fileName,m_gStatus->m_dxRenderer->m_pd3dDevice);
	 newMissilePtr->m_skeletalVisual->m_SkeletalName = fileName;
     CSkeletalAnimationDlg dlg;
	 dlg.m_renderStateObjectRef = m_gStatus->m_renderStateObject;
	 dlg.m_globalShaderRef = m_gStatus->m_cgGlobalShaderDB;
	 dlg.m_elementDBREF = NULL;
	 dlg.m_skillDatabaseREF = NULL;
     dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	 dlg.m_skeletonReferencePtr = newMissilePtr->m_skeletalVisual;
     dlg.directSoundRef = m_gStatus->m_dirSnd;
	 dlg.DoModal();
	 m_gStatus->SkeletalTextureReassignmentWCompile(newMissilePtr->m_skeletalVisual);

     //end 
     CString conversion;
     newMissilePtr->m_lifeTime = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
	 newMissilePtr->m_minDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
	 newMissilePtr->m_maxDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
	 newMissilePtr->m_dotRefValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_dotRef, NULL, TRUE);
	 newMissilePtr->m_particleLink = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
	 newMissilePtr->m_explosionLink  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
	 newMissilePtr->m_abilityType  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
	 newMissilePtr->m_skillType    = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_MissileName,newMissilePtr->m_missileName);
	 
     m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
     newMissilePtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);
 
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
     newMissilePtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);
     
     if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_forceExplodeAtEnd = TRUE;
	 else
	   newMissilePtr->m_forceExplodeAtEnd = FALSE;

	 if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_filterCollision = TRUE;
	 else
	   newMissilePtr->m_filterCollision = FALSE;

     newMissilePtr->m_physicDynamics = new CMovementCaps();

	 m_gStatus->m_missileDB->AddTail(newMissilePtr);
	 MissileDatabaseInList(m_gStatus->m_missileDB->GetCount());	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMISAddLaser() 
{
#ifndef _ClientOutput
     CMissileObj * newMissilePtr = new CMissileObj();
	 newMissilePtr->m_typeOfProjectile = 3;//laser segment
     
     newMissilePtr->m_laserObject = new CLaserObj();
	 newMissilePtr->m_laserObject->InitializeMesh(m_gStatus->m_dxRenderer->m_pd3dDevice); 

	 CLaserDlg dlg;
	 dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	 dlg.m_laserObjectRef = newMissilePtr->m_laserObject;
	 dlg.DoModal();
     //end 
     CString conversion;
     newMissilePtr->m_lifeTime = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_LifeTime,NULL,TRUE);
	 newMissilePtr->m_minDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_Damage,NULL,TRUE);
	 newMissilePtr->m_maxDamageValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_DamageMax,NULL,TRUE);
	 newMissilePtr->m_dotRefValue = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_dotRef, NULL, TRUE);
	 newMissilePtr->m_particleLink = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ParticleDBlink,NULL,TRUE);
	 newMissilePtr->m_explosionLink  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_ExplosionAttribute,NULL,TRUE);
	 newMissilePtr->m_abilityType  = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_AbilityUseType,NULL,TRUE);
	 newMissilePtr->m_skillType    = m_missileDlgBar.GetDlgItemInt(IDC_EDIT_MIS_SkillUseType,NULL,TRUE);
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_MissileName,newMissilePtr->m_missileName);
	 
     m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_CollisionRadius,conversion);
     newMissilePtr->m_collisionRadius = (float)atof((LPCTSTR)conversion);
 
	 m_missileDlgBar.GetDlgItemText( IDC_EDIT_MIS_HeatSeekPower,conversion);
     newMissilePtr->m_heatSeekingPower = (float)atof((LPCTSTR)conversion);
     
     if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_ForceExplode,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_forceExplodeAtEnd = TRUE;
	 else
	   newMissilePtr->m_forceExplodeAtEnd = FALSE;

	 if(m_missileDlgBar.SendDlgItemMessage(IDC_CHECK_MIS_FilterWldCoillision,BM_GETSTATE,0,0)==BST_CHECKED)
       newMissilePtr->m_filterCollision = TRUE;
	 else
	   newMissilePtr->m_filterCollision = FALSE;

     newMissilePtr->m_physicDynamics = new CMovementCaps();

	 m_gStatus->m_missileDB->AddTail(newMissilePtr);
	 MissileDatabaseInList(m_gStatus->m_missileDB->GetCount()-1);	
#endif
}*/
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPSupportedWorlds() 
{
   if(m_currentWldsSpawnPointModule)
	  delete m_currentWldsSpawnPointModule;
      m_currentWldsSpawnPointModule = 0;

      m_currentWldsSpawnPointModule = new CCurrentWorldsDlg();
	  m_currentWldsSpawnPointModule->m_multiplayerObject = m_gStatus->m_multiplayerObject;
	  m_currentWldsSpawnPointModule->Create(IDD_DIALOG_CurrentWorlds,NULL);//
      m_currentWldsSpawnPointModule->ShowWindow(SW_SHOW);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTEditDeathCfg() 
{
#ifndef _ClientOutput
    HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    temp.Replace("TRC_","");
	CMovementObj * mainPtr = m_gStatus->m_entObjList->GetByID(atoi(temp));
	if(!mainPtr){
       MessageBox("Must select a entity base object");
	   return;
	}
	//get database object
    CEditStartInventoryDlg dlg;
	if(!mainPtr->m_itemDB)
	    mainPtr->m_itemDB = new CItemObjList();
	dlg.m_gstatusMem				= m_gStatus; 
    dlg.m_itemDBRef					= mainPtr->m_itemDB;
	dlg.m_autoCfgRef				= mainPtr->m_deathCfg;
    dlg.m_armedInventoryDatabase	= m_gStatus->m_itemManager->m_visuals;
    dlg.DoModal();
	mainPtr->m_deathCfg = dlg.m_autoCfgRef;
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPViewAccount() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnCHECKMNUrenderClizeZoneInfo() 
{

   if(m_gStatus->m_renderMenuClickZoneInfo)
	  m_gStatus->m_renderMenuClickZoneInfo = FALSE;
   else
      m_gStatus->m_renderMenuClickZoneInfo = TRUE;
	
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNMNUDeleteClickZone() 
{
	if(AfxMessageBox("Are you sure?",MB_OKCANCEL,0)!=IDOK)
   return;
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
		MessageBox("must select a menu");
	   return;
	}
    int clIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MNU_ClickZones,LB_GETCURSEL ,0,0);
	if(clIndex == -1){
	   MessageBox("must select a click zone");
	   return;
	}
	
	POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);

		POSITION clPos = menuObj->m_clickActions->FindIndex(clIndex);
		if(!clPos)
			return;
	        CCLickZoneObj * cPtr = ( CCLickZoneObj* )menuObj->m_clickActions->GetAt(clPos);
			delete cPtr;
			cPtr = 0;
			menuObj->m_clickActions->RemoveAt(clPos);
		    InListClickZones(clIndex-1);
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnCHECKTXBBitmapLoadOverride() 
{
	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNUISkillBuilder() 
{
#ifndef _ClientOutput
  CSkillSystemDlg dlg;
  
  dlg.m_skillDatabaseRef       = m_gStatus->m_skillDatabase;
  dlg.m_edbDatabaseRef         = m_gStatus->m_entObjList;
  dlg.m_statDatabaseRef        = m_gStatus->m_statDatabase;
  dlg.m_gStatusMem			   = m_gStatus;

   if(dlg.DoModal()==IDOK){
      m_gStatus->m_skillDatabase = dlg.m_skillDatabaseRef;
	  //InListMissileSkillCriteria(0);
   }
#endif
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTBaseSkills() 
{
#ifndef _ClientOutput
    CAddEntityDlg dlg;
	dlg.saveCollisionInfo = TRUE;
    if(dlg.DoModal()!=IDOK)
	   return;

	HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    temp.Replace("TRC_","");
	CMovementObj * mainPtr = m_gStatus->m_entObjList->GetByID(atoi(temp));
	if(!mainPtr){
       MessageBox("Must select a entity base object");
	   return;
	}

	mainPtr->LoadFromDir(dlg.m_pathGlobal,dlg.m_folderPath,m_gStatus->m_dxRenderer->m_pd3dDevice,TRUE);
	InitFXSahdersOnSkeletal(mainPtr->List_skeletalObject);
    //m_gStatus->ReinitAllTexturesFromDatabase();
	m_gStatus->SkeletalTextureReassignmentWCompile(mainPtr->List_skeletalObject);
#endif
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNOBJAddNode() 
{
#ifndef _ClientOutput
	//test
	m_nodeImporterModule.m_referenceDBREF = m_gStatus->m_libraryReferenceDB;
	if(m_nodeImporterModule.DoModal()!=IDOK)
		return;

	POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
	if(!entPosition){
		MessageBox("no runtime spawned");
		return;
	}
	CMovementObj * entityPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);

	//If they don't have a group selected, return
	int groupIndex = m_dlgBarObject.SendDlgItemMessage(IDC_LIST_Objects_GroupList2Box, LB_GETCURSEL, 0, 0);
	if(groupIndex == -1)
	{
		MessageBox("You must select a group first.");
		return;
	}
	else
	{
		POSITION wldGroupPos = m_gStatus->m_worldGroupList->FindIndex(groupIndex);
		groupIndex = 0;
		if(wldGroupPos)
		{
			CWldGroupObj * wldPtr = (CWldGroupObj *)m_gStatus->m_worldGroupList->GetAt(wldGroupPos);
			groupIndex = wldPtr->List_groupInt;
		}
	}

	for(int loop=0;loop<m_nodeImporterModule.m_selectionCount;loop++)
		AddNodeByData(groupIndex,m_nodeImporterModule.m_selectionArray[loop],entityPtr,NULL);

    /*CWldObject * wldObject                  = new CWldObject((m_gStatus->GlobalTraceNumber++));  
    wldObject->List_groupNumber             = groupIndex;
    wldObject->List_triggerList             = new CTriggerObjectList;
    wldObject->m_node                       = new CNodeObj();
    wldObject->m_node->m_libraryReference   = m_nodeImporterModule.m_selection;
    CReferenceObj * refObj = m_gStatus->m_libraryReferenceDB->GetByIndex(wldObject->m_node->m_libraryReference);
    if(!refObj){
       AfxMessageBox("Cannot reference Reference Lib Object");
       return;
	}

    entityPtr->List_baseFrame->GetTransform(NULL,&wldObject->m_node->m_position);

	D3DMATRIX scaleMat,moveMat;
	m_gStatus->m_matLib.scale( m_nodeImporterModule.m_scaleX, m_nodeImporterModule.m_scaleY, m_nodeImporterModule.m_scaleZ,scaleMat.m);
	m_gStatus->m_matLib.Translate( m_nodeImporterModule.m_moveX, m_nodeImporterModule.m_moveY, m_nodeImporterModule.m_moveZ,moveMat.m);
	m_gStatus->m_matLib.fastMatrixMultiply(wldObject->m_node->m_position,moveMat,wldObject->m_node->m_position);
	m_gStatus->m_matLib.fastMatrixMultiply(wldObject->m_node->m_position,scaleMat,wldObject->m_node->m_position);

    
	refObj->m_meshObject->GetBoxWithMarix(wldObject->m_node->m_position,&wldObject->m_node->m_clipBox);
    //set bounding box
	
	wldObject->List_popoutBox.max.x = wldObject->m_node->m_position._41 + m_nodeImporterModule.m_popoutBoxMaxX;
	wldObject->List_popoutBox.max.y = wldObject->m_node->m_position._42 + m_nodeImporterModule.m_popoutBoxMaxY;
	wldObject->List_popoutBox.max.z = wldObject->m_node->m_position._43 + m_nodeImporterModule.m_popoutBoxMaxZ;
	wldObject->List_popoutBox.min.x = wldObject->m_node->m_position._41 + m_nodeImporterModule.m_popoutBoxSizeMinX;
	wldObject->List_popoutBox.min.y = wldObject->m_node->m_position._42 + m_nodeImporterModule.m_popoutBoxSizeMinY;
	wldObject->List_popoutBox.min.z = wldObject->m_node->m_position._43 + m_nodeImporterModule.m_popoutBoxSizeMinZ;

	
    //end set bounding box
    wldObject->List_poppedOut = TRUE;
    m_gStatus->m_wldObjList->AddTail(wldObject);	
	m_gStatus->m_worldListTemp->AddTail(wldObject);*/
#endif
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::AddNodeByData(int groupIndex,
							   int selection,
							   CMovementObj * entityPtr,
							   D3DMATRIX * overrideMatrix)
{
#ifndef _ClientOutput

	int sqCount = 1;
	int sqTrace = 0;
    while(sqCount > 0)
	{

	sqCount--;
	
	CWldObject * wldObject                  = new CWldObject((m_gStatus->GlobalTraceNumber++));  
    wldObject->List_groupNumber             = groupIndex;
    wldObject->List_triggerList             = new CTriggerObjectList;
    wldObject->m_node                       = new CNodeObj();
    wldObject->m_node->m_libraryReference   = selection + sqTrace;
	sqTrace++;
    CReferenceObj * refObj = m_gStatus->m_libraryReferenceDB->GetByIndex(wldObject->m_node->m_libraryReference);
    if(!refObj){
       AfxMessageBox("Cannot reference Reference Lib Object");
       return;
	}
    sqCount += refObj->m_seqLoad;
	if(!overrideMatrix)
	{
    entityPtr->List_baseFrame->GetTransform(NULL,&wldObject->m_node->m_position);

	D3DMATRIX scaleMat,moveMat;
	m_gStatus->m_matLib.scale( m_nodeImporterModule.m_scaleX, m_nodeImporterModule.m_scaleY, m_nodeImporterModule.m_scaleZ,scaleMat.m);
	m_gStatus->m_matLib.Translate( m_nodeImporterModule.m_moveX, m_nodeImporterModule.m_moveY, m_nodeImporterModule.m_moveZ,moveMat.m);
	m_gStatus->m_matLib.fastMatrixMultiply(wldObject->m_node->m_position,moveMat,wldObject->m_node->m_position);
	m_gStatus->m_matLib.fastMatrixMultiply(wldObject->m_node->m_position,scaleMat,wldObject->m_node->m_position);
	}
	else
		CopyMemory(&wldObject->m_node->m_position,overrideMatrix,sizeof(D3DMATRIX));
    
	
	//if(refObj->m_lockPointCount > 0)
	//{
	  //wldObject->m_node->m_clipBox.maxX = refObj->m_meshObject->List_boundingBox.maxX;
	  //wldObject->m_node->m_clipBox.maxY = refObj->m_meshObject->List_boundingBox.maxY;
	  //wldObject->m_node->m_clipBox.maxZ = refObj->m_meshObject->List_boundingBox.maxZ;
	  //wldObject->m_node->m_clipBox.minX = refObj->m_meshObject->List_boundingBox.minX;
	  //wldObject->m_node->m_clipBox.minY = refObj->m_meshObject->List_boundingBox.minY;
	  //wldObject->m_node->m_clipBox.minZ = refObj->m_meshObject->List_boundingBox.minZ;

	  /*wldObject->m_node->m_clipBox.maxX = wldObject->m_node->m_position._41 + refObj->m_meshObject->List_boundingBox.maxY;
	  wldObject->m_node->m_clipBox.maxY = wldObject->m_node->m_position._42 + refObj->m_meshObject->List_boundingBox.maxY;
	  wldObject->m_node->m_clipBox.maxZ = wldObject->m_node->m_position._43 + refObj->m_meshObject->List_boundingBox.maxY;
	  wldObject->m_node->m_clipBox.minX = wldObject->m_node->m_position._41 - refObj->m_meshObject->List_boundingBox.maxY;
	  wldObject->m_node->m_clipBox.minY = wldObject->m_node->m_position._42 - refObj->m_meshObject->List_boundingBox.maxY;
	  wldObject->m_node->m_clipBox.minZ = wldObject->m_node->m_position._43 - refObj->m_meshObject->List_boundingBox.maxY;*/

	  /*D3DVECTOR tempReg;
	  tempReg.x = (wldObject->m_node->m_position.m[0][0]*wldObject->m_node->m_clipBox.maxX)+(wldObject->m_node->m_position.m[1][0]*wldObject->m_node->m_clipBox.maxY)+(wldObject->m_node->m_position.m[2][0]*wldObject->m_node->m_clipBox.maxZ)+(wldObject->m_node->m_position.m[3][0]);
      tempReg.y = (wldObject->m_node->m_position.m[0][1]*wldObject->m_node->m_clipBox.maxX)+(wldObject->m_node->m_position.m[1][1]*wldObject->m_node->m_clipBox.maxY)+(wldObject->m_node->m_position.m[2][1]*wldObject->m_node->m_clipBox.maxZ)+(wldObject->m_node->m_position.m[3][1]);
      tempReg.z = (wldObject->m_node->m_position.m[0][2]*wldObject->m_node->m_clipBox.maxX)+(wldObject->m_node->m_position.m[1][2]*wldObject->m_node->m_clipBox.maxY)+(wldObject->m_node->m_position.m[2][2]*wldObject->m_node->m_clipBox.maxZ)+(wldObject->m_node->m_position.m[3][2]);
		
	  wldObject->m_node->m_clipBox.maxX = tempReg.x;
	  wldObject->m_node->m_clipBox.maxY = tempReg.y;
	  wldObject->m_node->m_clipBox.maxZ = tempReg.z;

	  tempReg.x = (wldObject->m_node->m_position.m[0][0]*wldObject->m_node->m_clipBox.minX)+(wldObject->m_node->m_position.m[1][0]*wldObject->m_node->m_clipBox.minY)+(wldObject->m_node->m_position.m[2][0]*wldObject->m_node->m_clipBox.minZ)+(wldObject->m_node->m_position.m[3][0]);
      tempReg.y = (wldObject->m_node->m_position.m[0][1]*wldObject->m_node->m_clipBox.minX)+(wldObject->m_node->m_position.m[1][1]*wldObject->m_node->m_clipBox.minY)+(wldObject->m_node->m_position.m[2][1]*wldObject->m_node->m_clipBox.minZ)+(wldObject->m_node->m_position.m[3][1]);
      tempReg.z = (wldObject->m_node->m_position.m[0][2]*wldObject->m_node->m_clipBox.minX)+(wldObject->m_node->m_position.m[1][2]*wldObject->m_node->m_clipBox.minY)+(wldObject->m_node->m_position.m[2][2]*wldObject->m_node->m_clipBox.minZ)+(wldObject->m_node->m_position.m[3][2]);
		
	  wldObject->m_node->m_clipBox.minX = tempReg.x;
	  wldObject->m_node->m_clipBox.minY = tempReg.y;
	  wldObject->m_node->m_clipBox.minZ = tempReg.z;*/


	  //RecalBox
	  /*ABBOX newBox;
      newBox->maxX = newBox->minX = wldObject->m_node->m_position._41;
      newBox->maxY = newBox->minY = wldObject->m_node->m_position._42;
      newBox->maxZ = newBox->minZ = wldObject->m_node->m_position._43;
	  float addData[7];

          addData[0] = wldObject->m_node->m_clipBox.maxX;
	      addData[1] = wldObject->m_node->m_clipBox.maxY;
	      addData[2] = wldObject->m_node->m_clipBox.maxZ;
	      addData[3] = wldObject->m_node->m_clipBox.minX;
	      addData[4] = wldObject->m_node->m_clipBox.minY;
	      addData[5] = wldObject->m_node->m_clipBox.minZ;


      for(int loop=0;loop<6;loop++)
	  {
          if(wldObject->m_node->m_clipBox.maxX < newBox->minX)
             newBox->minX = wldObject->m_node->m_clipBox.maxX;
	      if(wldObject->m_node->m_clipBox.minX < newBox->minX)
             newBox->minX = wldObject->m_node->m_clipBox.minX;

          if(wldObject->m_node->m_clipBox.maxX > newBox->maxX)
             newBox->maxX = wldObject->m_node->m_clipBox.maxX;
	      if(wldObject->m_node->m_clipBox.minX > newBox->maxX)
             newBox->maxX = wldObject->m_node->m_clipBox.minX;


	      if(translatedPos.y < returnBox->minY)
             returnBox->minY = translatedPos.y;
	      if(translatedPos.y > returnBox->maxY)
             returnBox->maxY = translatedPos.y;

	      if(translatedPos.z < returnBox->minZ)
             returnBox->minZ = translatedPos.z;
	      if(translatedPos.z > returnBox->maxZ)
             returnBox->maxZ = translatedPos.z;

          if(returnBox->minX == returnBox->maxX)
             returnBox->maxX =  returnBox->maxX + .01f;
          if(returnBox->minY == returnBox->maxY)
             returnBox->maxY =  returnBox->maxY + .01f;
          if(returnBox->minZ == returnBox->maxZ)
             returnBox->maxZ =  returnBox->maxZ + .01f;
	  }*/



	  /*D3DVECTOR tempReg;
	  for(int vX=0;vX<refObj->m_lockPointCount;vX++)
      {
		  tempReg.x = (wldObject->m_node->m_position.m[0][0]*refObj->m_extLockPoints[vX].x)+(wldObject->m_node->m_position.m[1][0]*refObj->m_extLockPoints[vX].y)+(wldObject->m_node->m_position.m[2][0]*refObj->m_extLockPoints[vX].z)+(wldObject->m_node->m_position.m[3][0]);
          tempReg.y = (wldObject->m_node->m_position.m[0][1]*refObj->m_extLockPoints[vX].x)+(wldObject->m_node->m_position.m[1][1]*refObj->m_extLockPoints[vX].y)+(wldObject->m_node->m_position.m[2][1]*refObj->m_extLockPoints[vX].z)+(wldObject->m_node->m_position.m[3][1]);
          tempReg.z = (wldObject->m_node->m_position.m[0][2]*refObj->m_extLockPoints[vX].x)+(wldObject->m_node->m_position.m[1][2]*refObj->m_extLockPoints[vX].y)+(wldObject->m_node->m_position.m[2][2]*refObj->m_extLockPoints[vX].z)+(wldObject->m_node->m_position.m[3][2]);

					  if(tempReg.x > wldObject->m_node->m_clipBox.maxX || vX == 0)
					     wldObject->m_node->m_clipBox.maxX = tempReg.x;

					  if(tempReg.y > wldObject->m_node->m_clipBox.maxY || vX == 0)
					     wldObject->m_node->m_clipBox.maxY = tempReg.y;

					  if(tempReg.z > wldObject->m_node->m_clipBox.maxZ || vX == 0)
					     wldObject->m_node->m_clipBox.maxZ = tempReg.z;

					  if(tempReg.x < wldObject->m_node->m_clipBox.minX || vX == 0)
					     wldObject->m_node->m_clipBox.minX = tempReg.x;

					  if(tempReg.y < wldObject->m_node->m_clipBox.minY || vX == 0)
					     wldObject->m_node->m_clipBox.minY = tempReg.y;

					  if(tempReg.z < wldObject->m_node->m_clipBox.minZ || vX == 0)
					     wldObject->m_node->m_clipBox.minZ = tempReg.z;
				  
      }*/
	  //transform box
	  /*D3DVECTOR tempReg;
	  tempReg.x = (wldObject->m_node->m_position.m[0][0]*wldObject->m_node->m_clipBox.maxX)+(wldObject->m_node->m_position.m[1][0]*wldObject->m_node->m_clipBox.maxY)+(wldObject->m_node->m_position.m[2][0]*wldObject->m_node->m_clipBox.maxZ)+(wldObject->m_node->m_position.m[3][0]);
      tempReg.y = (wldObject->m_node->m_position.m[0][1]*wldObject->m_node->m_clipBox.maxX)+(wldObject->m_node->m_position.m[1][1]*wldObject->m_node->m_clipBox.maxY)+(wldObject->m_node->m_position.m[2][1]*wldObject->m_node->m_clipBox.maxZ)+(wldObject->m_node->m_position.m[3][1]);
      tempReg.z = (wldObject->m_node->m_position.m[0][2]*wldObject->m_node->m_clipBox.maxX)+(wldObject->m_node->m_position.m[1][2]*wldObject->m_node->m_clipBox.maxY)+(wldObject->m_node->m_position.m[2][2]*wldObject->m_node->m_clipBox.maxZ)+(wldObject->m_node->m_position.m[3][2]);
	  wldObject->m_node->m_clipBox.maxX = tempReg.x;
	  wldObject->m_node->m_clipBox.maxY = tempReg.y;
	  wldObject->m_node->m_clipBox.maxZ = tempReg.z;

      tempReg.x = (wldObject->m_node->m_position.m[0][0]*wldObject->m_node->m_clipBox.minX)+(wldObject->m_node->m_position.m[1][0]*wldObject->m_node->m_clipBox.minY)+(wldObject->m_node->m_position.m[2][0]*wldObject->m_node->m_clipBox.minZ)+(wldObject->m_node->m_position.m[3][0]);
      tempReg.y = (wldObject->m_node->m_position.m[0][1]*wldObject->m_node->m_clipBox.minX)+(wldObject->m_node->m_position.m[1][1]*wldObject->m_node->m_clipBox.minY)+(wldObject->m_node->m_position.m[2][1]*wldObject->m_node->m_clipBox.minZ)+(wldObject->m_node->m_position.m[3][1]);
      tempReg.z = (wldObject->m_node->m_position.m[0][2]*wldObject->m_node->m_clipBox.minX)+(wldObject->m_node->m_position.m[1][2]*wldObject->m_node->m_clipBox.minY)+(wldObject->m_node->m_position.m[2][2]*wldObject->m_node->m_clipBox.minZ)+(wldObject->m_node->m_position.m[3][2]);
	  wldObject->m_node->m_clipBox.minX = tempReg.x;
	  wldObject->m_node->m_clipBox.minY = tempReg.y;
	  wldObject->m_node->m_clipBox.minZ = tempReg.z;*/


	//}
	//else

    if(refObj->m_skeletalOption)
	{
       /*CopyMemory(&wldObject->m_node->m_clipBox,&refObj->m_skeletalOption->m_box,sizeof(ABBOX));
	   refObj->m_skeletalOption->m_box.maxX += wldObject->m_node->m_position._41;
	   refObj->m_skeletalOption->m_box.minX += wldObject->m_node->m_position._41;
	   refObj->m_skeletalOption->m_box.maxY += wldObject->m_node->m_position._42;
	   refObj->m_skeletalOption->m_box.minY += wldObject->m_node->m_position._42;
	   refObj->m_skeletalOption->m_box.maxZ += wldObject->m_node->m_position._43;
	   refObj->m_skeletalOption->m_box.minZ += wldObject->m_node->m_position._43;*/

		wldObject->m_node->m_clipBox = refObj->m_skeletalOption->QuerryBoxExact(&wldObject->m_node->m_position);
	}
    else if(refObj->m_meshObject)
	   refObj->m_meshObject->GetBoxWithMarix(wldObject->m_node->m_position,&wldObject->m_node->m_clipBox);
    //set bounding box
	if(refObj->m_lockPointCount > 0)
	{//adjust for speed tree implementation
	   //ground for billboard box change
	   if(wldObject->m_node->m_position._42 < wldObject->m_node->m_clipBox.minY)
          wldObject->m_node->m_clipBox.minY = wldObject->m_node->m_position._42;

	   //adjust for extent of small billboards
	   wldObject->m_node->m_clipBox.minX = wldObject->m_node->m_clipBox.minX - refObj->m_billboardSz;
       wldObject->m_node->m_clipBox.minZ = wldObject->m_node->m_clipBox.minX - refObj->m_billboardSz;

	   wldObject->m_node->m_clipBox.maxX = wldObject->m_node->m_clipBox.maxX + refObj->m_billboardSz;
	   wldObject->m_node->m_clipBox.maxY = wldObject->m_node->m_clipBox.maxY + refObj->m_billboardSz;
	   wldObject->m_node->m_clipBox.maxZ = wldObject->m_node->m_clipBox.maxZ + refObj->m_billboardSz;
		   
	}//end adjust for speedtree implementation
	
	wldObject->List_popoutBox.max.x = wldObject->m_node->m_position._41 + refObj->m_popoutBox.max.x;//m_nodeImporterModule.m_popoutBoxMaxX;
	wldObject->List_popoutBox.max.y = wldObject->m_node->m_position._42 + refObj->m_popoutBox.max.y;//m_nodeImporterModule.m_popoutBoxMaxY;
	wldObject->List_popoutBox.max.z = wldObject->m_node->m_position._43 + refObj->m_popoutBox.max.z;//m_nodeImporterModule.m_popoutBoxMaxZ;
	wldObject->List_popoutBox.min.x = wldObject->m_node->m_position._41 + refObj->m_popoutBox.min.x;//m_nodeImporterModule.m_popoutBoxSizeMinX;
	wldObject->List_popoutBox.min.y = wldObject->m_node->m_position._42 + refObj->m_popoutBox.min.y;//m_nodeImporterModule.m_popoutBoxSizeMinY;
	wldObject->List_popoutBox.min.z = wldObject->m_node->m_position._43 + refObj->m_popoutBox.min.z;//m_nodeImporterModule.m_popoutBoxSizeMinZ;
	
    //end set bounding box
    wldObject->List_poppedOut = TRUE;
    m_gStatus->m_wldObjList->AddTail(wldObject);
	m_gStatus->m_worldListTemp->AddTail(wldObject);
	PushEditorUndoObjectTrace(wldObject->List_identity, groupIndex, wldObject->m_node->m_libraryReference, &wldObject->m_node->m_position);

	}
#endif
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNOBJReferenceLibrary() 
{
#ifndef _LimitedEngine

#ifndef _ClientOutput
    CReferenceLibrary dlg;
	dlg.m_cgGlobalShaderDB = m_gStatus->m_cgGlobalShaderDB;
	dlg.m_dirSndRef        = m_gStatus->m_dirSnd;
	dlg.m_elementDBREF     = m_gStatus->m_elementDB;
	dlg.m_worldObjListRef  = m_gStatus->m_wldObjList;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.m_referenceDBREF   = m_gStatus->m_libraryReferenceDB;
	dlg.g_pd3dDeviceRef    = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.m_renderStateObjectRef = m_gStatus->m_renderStateObject;
	dlg.m_textureSearchPathsRef = m_gStatus->m_textureSearchPath;
	dlg.m_texturePathCountRef = m_gStatus->m_searchPathCount;
	dlg.m_gStatusRef = m_gStatus;
	dlg.DoModal();
	m_gStatus->m_searchPathCount = dlg.m_texturePathCountRef;
	m_gStatus->m_libraryReferenceDB   = dlg.m_referenceDBREF;
	m_gStatus->ReinitTextureLibraryTextures();
	m_gStatus->m_textureDataBase->CompileDirectAccess();
	m_gStatus->m_resetReferences = TRUE;
	m_gStatus->m_skeletalReferenceObjCache.RemoveAll();
#endif

#endif
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNUISoundDB() 
{
#ifndef _ClientOutput 
	CSoundDatabaseDlg dlg;
	dlg.m_soundManagerRef		= m_gStatus->m_soundManager;
	dlg.directSoundRef			= m_gStatus->m_dirSnd;
	dlg.m_gstatusRef			= m_gStatus;
	dlg.DoModal();
	m_gStatus->m_soundManager	= dlg.m_soundManagerRef;
#endif
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNUIRadarSettings()
{
#ifndef _ClientOutput 
	CSoundDatabaseDlg dlg;
	dlg.m_soundManagerRef		= m_gStatus->m_soundManager;
	dlg.m_gstatusRef			= m_gStatus;
	dlg.directSoundRef			= m_gStatus->m_dirSnd;
	dlg.DoModal();
	m_gStatus->m_soundManager	= dlg.m_soundManagerRef;
#endif
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNUIGlInventoryDB() 
{
 
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBtnUiCurrencysys() 
{
#ifndef _ClientOutput
     CCurrencyDlg dlg;
	  dlg.m_currencyName = m_currencyObject->m_currencyName;
	 if(dlg.DoModal()==IDOK)
	 {
	  m_currencyObject->m_currencyName = dlg.m_currencyName;
	 }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNUICommerceDB() 
{
#ifndef _ClientOutput
    CCommerceDlg dlg;
	dlg.m_gStatusMem         = m_gStatus;
	dlg.m_commerceObjDBRef   = m_gStatus->m_commerceDB;
	dlg.DoModal();
	m_gStatus->m_commerceDB = dlg.m_commerceObjDBRef;
#endif
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNUIBankZones() 
{
#ifndef _ClientOutput
	CBankZonesDlg dlg;
	dlg.m_bankDBRef = m_gStatus->m_bankZoneList;
	dlg.DoModal();
	m_gStatus->m_bankZoneList = dlg.m_bankDBRef;
#endif
}
//
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBtnUiSafezones() 
{
#ifndef _ClientOutput
	CSafeZoneDlg dlg;
	dlg.m_safeZoneDBREF = m_gStatus->m_safeZoneDB;
	dlg.DoModal();
	m_gStatus->m_safeZoneDB = dlg.m_safeZoneDBREF;
#endif
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTRefreshList() 
{
	InListRuntimeEntities(0);
	
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTViewRuntimeStats() 
{
	int runtimeIndex = m_entityDlgBar.SendDlgItemMessage(IDC_LIST_ENTITY_RuntimeList,LB_GETCURSEL ,0,0);
	   if(runtimeIndex == -1){
        MessageBox("Must select an entity object");
		return;
	   }
	   POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(runtimeIndex);
	   if(!posLoc)
		   return;
	       CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
	      
		   
		    if(m_entityViewerModule)
	           delete m_entityViewerModule;
               m_entityViewerModule = 0;

                  m_entityViewerModule = new CRuntimeEntityViewerDlg();
	              m_entityViewerModule->m_runtimeEntityRef = ptrLoc;
	              m_entityViewerModule->Create(IDD_DIALOG_RuntimeEntityInfo,NULL);//
                  m_entityViewerModule->ShowWindow(SW_SHOW);
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTViewRuntimeResources() 
{
	//use gamefiles folder for organization
    CString CurDir;
      char buffer[_MAX_PATH];
	  _getcwd( buffer, _MAX_PATH );
	  CurDir = buffer;
    
    //end use gamefiles folder for organization
    static char BASED_CODE filter[] = "TXT Files|*.TXT|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
		 CString fileName = opendialog.GetPathName();//
         CFile fl;
		 CFileException exc;
		   BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
		   if(res == FALSE)
		   {
		      SetCurrentDirectory(CurDir);
			  return;
		   }
		   int fileLength = fl.GetLength();
		   char * buffer = new char[fileLength+1];
		   ZeroMemory(buffer,fileLength+1);
           fl.Read(buffer,fileLength);
		   fl.Close();
           
           m_gStatus->m_entObjList->ApplyLODProcessing(buffer);
		   m_gStatus->m_itemManager->m_visuals->ApplyLODProcessing(buffer);
		   m_gStatus->m_lodBalancer.ApplyLODProcessing(buffer);
		   
		   delete [] buffer;
		   buffer = 0;
	 }
	 //m_gStatus->m_entObjList//
	 SetCurrentDirectory(CurDir);
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNENTUnifySleletalMesh() 
{
    HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    temp.Replace("TRC_","");
	CMovementObj * mainPtr = m_gStatus->m_entObjList->GetByID(atoi(temp));
	if(!mainPtr){
       MessageBox("Must select a entity base object");
	   return;
	}
	mainPtr->List_skeletalObject->UnifyAllNormalsDeformable(m_gStatus->m_dxRenderer->m_pd3dDevice);
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
/*void EngineXWin::OnBTNMISDeleteLight() 
{
     int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
        MessageBox("Must Select a missile database object");
	    return;
	 }
     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
         CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);	 
		 if(misPtr->m_light)
		 {
			delete misPtr->m_light;
			misPtr->m_light = 0;

			AfxMessageBox("Done");
		 }
		 else
			AfxMessageBox("No light existed");
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void EngineXWin::OnBTNMISLightSys() 
{
#ifndef _ClientOutput
	int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
        MessageBox("Must Select a missile database object");
	    return;
	 }
     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
         CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);	 
			 CLightInterfaceDlg dlg;
	         dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	         dlg.m_lightREF = misPtr->m_light;
	         if(dlg.DoModal()==IDOK)
		        misPtr->m_light = dlg.m_lightREF;
#endif
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPItemGeneratorDB() 
{
	  //obsolete

}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::LoadPatchDatabase(CString fileName)
{
    AfxMessageBox("This sys has been taken out.");
}

//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPUpdateCurrentOnlinePlayers() 
{
   
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTScaleEntity() 
{
    HWND treeHwnd = NULL;
	m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
    HTREEITEM item = TreeView_GetSelection(treeHwnd);
    CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
    temp.Replace("TRC_","");
	CMovementObj * mainPtr = m_gStatus->m_entObjList->GetByID(atoi(temp));
	if(!mainPtr){
       MessageBox("Must select a entity base object");
	   return;
	}

	CString conversion;
	m_entityDlgBar.GetDlgItemText( IDC_EDIT_ENT_scalePercentage,conversion);
	mainPtr->List_skeletalObject->ScaleSystem((float)atof((LPCTSTR)conversion),m_gStatus->m_dxRenderer->m_pd3dDevice);
	MessageBox("done");
	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*
void EngineXWin::OnCHECKSPNEnableSystem() 
{
	if(m_gStatus->m_spawnGeneratorDB->m_enable)
	   m_gStatus->m_spawnGeneratorDB->m_enable = FALSE;
	else
	   m_gStatus->m_spawnGeneratorDB->m_enable = TRUE;
	
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------add texture--------------------------------------------------------------//
void EngineXWin::ConvertExt(CString & currentFile,CString newExt)
{
     CString ext = m_gStatus->m_cabLib.GetFileExt(currentFile);
	 
	 ext.MakeLower();
     if(ext == "bmp")
	 {
	   m_gStatus->m_cabLib.RemoveFileExt(&currentFile);
       m_gStatus->m_cabLib.AddNewFileExt(&currentFile,newExt);
	 }
}
//------------------------------------------------------------------------------------//
/*
void EngineXWin::OnBTNTXBSetMipLevels() 
{
	int runtimeIndex = m_textureDBDlgBar.SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              return;
		   }
	       POSITION posLoc = m_gStatus->m_textureDataBase->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_gStatus->m_textureDataBase->GetAt(posLoc);
			   
               ptrLoc->m_textureObject->m_dwMipMapCount		= m_textureDBDlgBar.GetDlgItemInt(IDC_BTN_TXB_mipLevels,NULL,TRUE);
			   ptrLoc->m_bump								= m_textureDBDlgBar.GetDlgItemInt(IDC_EDIT_TXTDB_TextureName,NULL,TRUE);
			   ptrLoc->m_bumpType							= m_textureDBDlgBar.GetDlgItemInt(IDC_EDIT_TXTDB_TextureBumpType,NULL,TRUE);
			   
			   ptrLoc->m_textureObject->m_usage = m_textureDBDlgBar.SendDlgItemMessage(IDC_CHECK_TXB_BitmapLoadOverride,BM_GETSTATE,0,0);

               CString Peram1; 
               m_textureDBDlgBar.GetDlgItemText( IDC_EDIT_TXTDBPerameter,Peram1);
               ptrLoc->m_peramameter1 = (float)atof((LPCTSTR)Peram1);

			   ptrLoc->m_textureObject->m_compressedOvrd = m_textureDBDlgBar.GetDlgItemInt(IDC_EDIT_txtCompressedOrd,NULL,TRUE);

			   BOOL processImage = TRUE;
               /*ptrLoc->m_textureObject->Restore(m_gStatus->m_dxRenderer->m_pd3dDevice,
				                                0,
												ptrLoc->m_bump,
												ptrLoc->m_bumpType,
												m_gStatus->m_dxRenderer->m_maxTextureSizeSquared,
												m_gStatus->m_renderStateObject->m_pCaps,
												m_gStatus->m_compressTextures,
												processImage,
												m_gStatus->m_advUrlBasic,
												m_gStatus->m_advUrlVending);*/
			   /*if(ptrLoc->m_textureObject->Restore(	m_gStatus->m_dxRenderer->m_pd3dDevice,
													0,
													ptrLoc->m_bump,
													ptrLoc->m_bumpType,
													m_gStatus->m_dxRenderer->m_maxTextureSizeSquared,
													m_gStatus->m_renderStateObject->m_pCaps,
													m_gStatus->m_compressTextures,
													processImage,
													m_gStatus->m_advUrlBasic,
													m_gStatus->m_advUrlVending) != S_OK)
			   {
				   CString build = ptrLoc->List_textureName;
			       AfxMessageBox(build + " could not initialize",0,0);
			   }

			   

               InListTextureDB(runtimeIndex);
}
*/

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNENTReplaceEntityWSIE() 
{
    int indexLoc=0;
    BOOL foundIt = FALSE; 
    CMovementObj * mPtr;
    //get selected index
	POSITION posLast = NULL;
	int tracer = 0;
    for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();(posLast = posLoc) != NULL;){//visual loop
	    mPtr = ( CMovementObj* ) m_gStatus->m_entObjList->GetNext(posLoc);
		HWND treeHwnd = NULL;
        m_entityDlgBar.GetDlgItem( IDC_TREE_ENTITIES, &treeHwnd);
        HTREEITEM item = TreeView_GetSelection(treeHwnd);
		CString temp = GetTreeItemText(IDC_TREE_ENTITIES,&m_entityDlgBar,item,50);
        if((operator+("TRC_",m_gStatus->m_cabLib.ConvToString(mPtr->m_uniqueId))) == temp){
           foundIt = TRUE;
           //mPtr->SafeDelete(m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_spawnGeneratorDB);
           //delete mPtr;
		   //mPtr = 0;
           //m_gStatus->m_entObjList->RemoveAt(posLast);
		   break;
		}
		indexLoc++;
		tracer++;
	}
	if(foundIt == FALSE){
       MessageBox("Must select a entity base object");
	   return;
	}
	
	
	
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

    static char BASED_CODE filter[] = "SIE Files|*.SIE|All Files (*.*)|*.*||";
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
	   CMovementObj * newPtrLoc = NULL;
	   the_in_Archive >> newPtrLoc;
	   the_in_Archive.Close();
       fl.Close();  
	   newPtrLoc->List_traceNumber = m_gStatus->GlobalTraceNumber++;
	   //Prep then add baby!!
       tracer--;
	   if(tracer < 0)
	   {
          m_gStatus->m_entObjList->AddTail(newPtrLoc);
	   }
	   else{
          POSITION hubLoc = m_gStatus->m_entObjList->FindIndex(tracer);
          if(hubLoc)
		  {
             m_gStatus->m_entObjList->InsertAfter(hubLoc,newPtrLoc);
		  }
	   }

	   SetCurrentDirectory(MainDir);	
	   
	      EnitityInList();	   
	 }
     SetCurrentDirectory(MainDir);		
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------entity child animation index update-----------------------------------//
void EngineXWin::OnBTNTCPIPBlocking() 
{
/*#ifndef _ClientOutput
	CBlockIpDlg dlg;
	if(!m_gStatus->m_multiplayerObject->m_ipBlockList)
		m_gStatus->m_multiplayerObject->m_ipBlockList = new CBlockIPList();

    dlg.m_IPBlockListRef = m_gStatus->m_multiplayerObject->m_ipBlockList;
    if(dlg.DoModal()==IDOK)
	{
       m_gStatus->m_multiplayerObject->m_ipBlockList = dlg.m_IPBlockListRef;
	}
#endif*/
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBtnUiWaterzones() 
{
#ifndef _ClientOutput
	CWaterZoneDBDLG dlg;

    dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
	dlg.m_waterZnDBREF  = m_gStatus->m_waterZoneDB;
	dlg.m_waterZnDBREF_memloc = &m_gStatus->m_waterZoneDB;

	if(dlg.DoModal() == IDOK)
	{
       
       m_gStatus->m_waterZoneDB = dlg.m_waterZnDBREF;


	}	
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNMNUSaveUnit() 
{
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
   
	POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);
		
        //use gamefiles folder for organization
        CString CurDir;
        CString MainDir;
        char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
        //end use gamefiles folder for organization
        static char BASED_CODE filter[] = "MUI Files|*.MUI|All Files (*.*)|*.*||";
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
             the_out_Archive << menuObj;
             the_out_Archive.Close();
             fl.Close();
		}
	    SetCurrentDirectory(MainDir);  
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNMNUImportUnit() 
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

   static char BASED_CODE filter[] = "MUI Files|*.MUI|All Files (*.*)|*.*||";
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
	   CMenu3DObj * menuObj = NULL;
	   the_in_Archive >> menuObj;
	   the_in_Archive.Close();
       fl.Close();  
	   //Prep then add baby!!
	   SetCurrentDirectory(MainDir);	
	   m_gStatus->m_menuDatabase->AddTail(menuObj);
	   if(m_gStatus->m_editorModeOn == TRUE)
	      MenuInList(0);  
	 }
     SetCurrentDirectory(MainDir);	
	
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBtnUiPortalzonesdb() 
{
#ifndef _ClientOutput
     CPortalsDlg dlg;
	 dlg.m_portalsDBRef = m_gStatus->m_portalDatabase;
	 dlg.g_pd3dDeviceRef = m_gStatus->m_dxRenderer->m_pd3dDevice;
	 if(dlg.DoModal()==IDOK)
	 {
        m_gStatus->m_portalDatabase = dlg.m_portalsDBRef;
	 }
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNUIElementDB() 
{
#ifndef _ClientOutput
	CElementsDlg dlg;
	dlg.m_elementDBREF = m_gStatus->m_elementDB;
	if(dlg.DoModal()==IDOK)
	{
       	m_gStatus->m_elementDB = dlg.m_elementDBREF;
	}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNUIDotDB()
{
#ifndef _ClintOutput
	AddDoTdlg dlg;
	dlg.m_dotDBREF = m_gStatus->m_dotDB;
	dlg.m_currentElements = m_gStatus->m_elementDB;
	if(dlg.DoModal() == IDOK)
	{
		m_gStatus->m_dotDB = dlg.m_dotDBREF;
	}
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::InListMisChannels(int sel) 
{

     m_missileDlgBar.SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_RESETCONTENT,0,0);	
     for(POSITION posLoc = m_gStatus->m_elementDB->GetHeadPosition();posLoc != NULL;)
	 {
       CElementObj * entLoc = (CElementObj *)m_gStatus->m_elementDB->GetNext(posLoc);
	   m_missileDlgBar.SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)entLoc->m_elementName);
	 }
     if(sel > -1)
	   m_missileDlgBar.SendDlgItemMessage(IDC_BTN_MIS_DamageChannel,CB_SETCURSEL ,sel,0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPUpgradeWorm() 
{

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNEXPSaveUnit() 
{
#ifndef _LimitedEngine
   int runtimeIndex = m_explosionDlgBar.SendDlgItemMessage(IDC_LIST_ExplosionList,LB_GETCURSEL ,0,0);
   if(runtimeIndex == -1){
      MessageBox("Must select an explosion");
	  return;
   }
   POSITION posLoc = m_gStatus->m_explosionDBList->FindIndex(runtimeIndex);
   if(!posLoc)
	   return;
       CExplosionObj * expObj = (CExplosionObj *)m_gStatus->m_explosionDBList->GetAt(posLoc);
	   
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "EXU Files|*.EXU|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "EXU", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << expObj;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir); 
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNEXPLoadUnit() 
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

   static char BASED_CODE filter[] = "EXU Files|*.EXU|All Files (*.*)|*.*||";
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
	   CExplosionObj * expObj = NULL;
	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> expObj;
	   the_in_Archive.Close();
       fl.Close();
	   m_gStatus->m_explosionDBList->AddTail(expObj);
	 }
	 InListExplosionDB(m_gStatus->m_explosionDBList->GetCount()-1);
     SetCurrentDirectory(MainDir);
	
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPDisconnectAll() 
{
	
}

void EngineXWin::OnBTNPRTSaveUnit() 
{
	int runtimeIndex = m_particleDBDlgBar.SendDlgItemMessage(IDC_LIST_PART_Database,LB_GETCURSEL ,0,0);
 
    if(runtimeIndex == -1)
	{
	MessageBox("Must select a particle system");
	return;
	}

    POSITION posLoc = m_gStatus->m_particleDatabase->FindIndex(runtimeIndex);
    if(!posLoc)
        return;
    
    EnvParticleSystem * ptrLoc = ( EnvParticleSystem* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "PRU Files|*.PRU|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "PRU", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << ptrLoc;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir); 
	
}

void EngineXWin::OnBTNPRTLoadUnit() 
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

   static char BASED_CODE filter[] = "PRU Files|*.PRU|All Files (*.*)|*.*||";
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
	   EnvParticleSystem * ptrLoc = NULL;
	   the_in_Archive >> ptrLoc;
	   the_in_Archive.Close();
       fl.Close();
	   ptrLoc->im_mesh->InitBuffer(m_gStatus->m_dxRenderer->m_pd3dDevice,NULL);
	   m_gStatus->m_particleDatabase->AddTail(ptrLoc);
	 }
     SetCurrentDirectory(MainDir);
     ParticleDatabaseInList(m_gStatus->m_particleDatabase->GetCount()-1);
	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OutPutInfoFile(CString filename)
{
	CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
	//------------------------------
	//--------------Missile Database
	the_out_Archive.WriteString( "---MISSILE DATABASE-EXPLOSION DB CROSS REFRENCED---" );  
	the_out_Archive.WriteString( "\n");
	int tracer=0;
	for(POSITION posLoc = m_gStatus->m_missileDB->GetHeadPosition();posLoc != NULL;)
	{//visual loop
	   CMissileObj * mPtr = ( CMissileObj* ) m_gStatus->m_missileDB->GetNext(posLoc);
       CString build = "                               ";
	   int lengthLimit = build.GetLength()-1;
       for(int loop=0;loop<mPtr->m_missileName.GetLength();loop++)
	   { 
		   //mPtr->m_missileName
           build.SetAt(loop,mPtr->m_missileName.GetAt(loop));
		   if(loop >= lengthLimit)
              break;
	   }
	   the_out_Archive.WriteString( m_gStatus->m_cabLib.ConvToString(tracer) );
	   the_out_Archive.WriteString( ":" );
	   the_out_Archive.WriteString( build ); 
	   the_out_Archive.WriteString( "\t");
	   the_out_Archive.WriteString( "Min Dmg:");
	   the_out_Archive.WriteString( m_gStatus->m_cabLib.ConvToString(mPtr->m_minDamageValue)); 
	   the_out_Archive.WriteString( "\t");
	   the_out_Archive.WriteString( "Max Dmg:");
	   the_out_Archive.WriteString( m_gStatus->m_cabLib.ConvToString(mPtr->m_maxDamageValue));
	   the_out_Archive.WriteString( "\t");
	   the_out_Archive.WriteString( "E:");
	   the_out_Archive.WriteString( m_gStatus->m_cabLib.ConvToString(mPtr->m_damageChannel)); 
	   the_out_Archive.WriteString( "\t");
	   the_out_Archive.WriteString( "Exp-Link:"); 
	   the_out_Archive.WriteString( m_gStatus->m_cabLib.ConvToString(mPtr->m_explosionLink)); 
	   the_out_Archive.WriteString( "\t");
	   the_out_Archive.WriteString( "DoT Attachment:");
	   the_out_Archive.WriteString( m_gStatus->m_cabLib.ConvToString(mPtr->m_dotRefValue));
	   the_out_Archive.WriteString( "\t");

	   if(mPtr->m_explosionLink > -1)
	   {//EXP cross reference
	    POSITION misPos = m_gStatus->m_explosionDBList->FindIndex(mPtr->m_explosionLink);
        if(misPos)
		{
          CExplosionObj * expObj = (CExplosionObj *)m_gStatus->m_explosionDBList->GetAt(misPos);
       
	      the_out_Archive.WriteString( "Exp-Radius:"); 
	      the_out_Archive.WriteString( m_gStatus->m_cabLib.ConvToString((int)expObj->m_damageRadius)); 
	      the_out_Archive.WriteString( "\t");
	      the_out_Archive.WriteString( "Exp-Dmg:"); 
	      the_out_Archive.WriteString( m_gStatus->m_cabLib.ConvToString(expObj->m_radiusDamage)); 
		  the_out_Archive.WriteString( "\t");
	      the_out_Archive.WriteString( "E:");
	      the_out_Archive.WriteString( m_gStatus->m_cabLib.ConvToString(expObj->m_damageChannel)); 
		}
	   }//end EXP cross reference
	   the_out_Archive.WriteString( "\n");  
	   tracer++;
	}
	
	
	the_out_Archive.Close();
    fl.Close(); 
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPPrintReports() 
{
	 //SpawnGenDataSQLOutput();
	 SqlItemUpdateToServer();
	 SqlAIToServer();
	 
     /*static char BASED_CODE filter[] = "DOC|*.DOC|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "DOC", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   OutPutInfoFile(fileName);
	 }	*/
	AfxMessageBox("Completed");
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::RemoveDuplicatesThatExistFromFile() 
{
   static char BASED_CODE filter[] = "TXB|*.TXB|All Files (*.*)|*.*||";
   CFileDialog opendialog( FALSE, "TXB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
   if ( opendialog.DoModal() == IDOK )
   {
	   CString fileName = opendialog.GetPathName();

	 CFileException exc;
     CFile fl;
	 
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	
	 CTextureDBObjList * textureTemp = NULL;

	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> textureTemp;
	 the_in_Archive.Close();
     fl.Close();
	 
	 if(textureTemp)
	 {//start
	 POSITION posLast = NULL;
	   for(POSITION texPositionLoc = m_gStatus->m_textureDataBase->GetHeadPosition();(posLast = texPositionLoc) != NULL;)
	   {//loop through each one checking for duplicates
         CTextureDBObj * txbPtr = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetNext(texPositionLoc);
    
 
		 for(POSITION sPos = textureTemp->GetHeadPosition();sPos != NULL;)
		 {//secondary loop
             CTextureDBObj * filePtr = (CTextureDBObj *)textureTemp->GetNext(sPos);
	         CString testOne = txbPtr->List_textureName;
             CString textTwo = filePtr->List_textureName;
             testOne.MakeLower();
             textTwo.MakeLower();
			 if(testOne == textTwo)
			 {//remove this one already here
	            //filePtr->SafeDelete();
				//delete filePtr;
			//	filePtr = 0;
				m_gStatus->m_textureDataBase->RemoveAt(posLast);
                break;
			 }//end remove this one already here
		 }//end secondary loop
	   }//end loop through each one checking for duplicates
	 
    
	   //textureTemp->SafeDelete();
       //delete textureTemp;
       //textureTemp = 0;
	 }//end start
   }
  return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*
void EngineXWin::OnBTNTXBRemoveDuplicates() 
{
	if(AfxMessageBox("Are you sure you want to apply these changes to this texture permenantly?",MB_OKCANCEL,0)!=IDOK)
	   return;
	//RemoveDuplicatesThatExistFromFile();
	int runtimeIndex = m_textureDBDlgBar.SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_GETCURSEL ,0,0);
	       if(runtimeIndex == -1){
              return;
		   }
	       POSITION posLoc = m_gStatus->m_textureDataBase->FindIndex(runtimeIndex);
	       if(!posLoc)
		       return;
	           CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_gStatus->m_textureDataBase->GetAt(posLoc);
			   
               ptrLoc->m_textureObject->m_dwMipMapCount		= m_textureDBDlgBar.GetDlgItemInt(IDC_BTN_TXB_mipLevels,NULL,TRUE);
			   ptrLoc->m_bump								= m_textureDBDlgBar.GetDlgItemInt(IDC_EDIT_TXTDB_TextureName,NULL,TRUE);
			   ptrLoc->m_bumpType							= m_textureDBDlgBar.GetDlgItemInt(IDC_EDIT_TXTDB_TextureBumpType,NULL,TRUE);
			   
			   ptrLoc->m_textureObject->m_usage = m_textureDBDlgBar.SendDlgItemMessage(IDC_CHECK_TXB_BitmapLoadOverride,BM_GETSTATE,0,0);

               

               CString Peram1; 
               m_textureDBDlgBar.GetDlgItemText( IDC_EDIT_TXTDBPerameter,Peram1);
               ptrLoc->m_peramameter1 = (float)atof((LPCTSTR)Peram1);
			   ptrLoc->m_textureObject->m_compressedOvrd = m_textureDBDlgBar.GetDlgItemInt(IDC_EDIT_txtCompressedOrd,NULL,TRUE);
			   
			   BOOL processImage = TRUE;
               if(ptrLoc->m_textureObject->Restore(	m_gStatus->m_dxRenderer->m_pd3dDevice,
													0,
													ptrLoc->m_bump,
													ptrLoc->m_bumpType,
													m_gStatus->m_dxRenderer->m_maxTextureSizeSquared,
													m_gStatus->m_renderStateObject->m_pCaps,
													m_gStatus->m_compressTextures,
													processImage,
													m_gStatus->m_advUrlBasic,
													m_gStatus->m_advUrlVending) != S_OK)
			   {
					CString build = ptrLoc->List_textureName;
			        AfxMessageBox(build + " could not initialize",0,0);
			   }
			   else
			        ptrLoc->m_textureObject->CopyDataLiveDataToBaseData(0);

               InListTextureDB(runtimeIndex);
}
*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::InListMissileSkillCriteria(int sel)
{ 
   if(m_gStatus->m_editorModeOn == FALSE)
	  return;

   m_missileDlgBar.SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_RESETCONTENT,0,0);	
   m_missileDlgBar.SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)"NONE");
   for(POSITION posLoc = m_gStatus->m_skillDatabase->GetHeadPosition();posLoc != NULL;)
   {
       CSkillObject * skLoc = (CSkillObject *)m_gStatus->m_skillDatabase->GetNext(posLoc);
	   m_missileDlgBar.SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_ADDSTRING,0,(LPARAM)(LPCTSTR)skLoc->m_skillName);
   }
   if(sel > -1)
	   m_missileDlgBar.SendDlgItemMessage(IDC_COMBO_MIS_CriteriaFSkill,CB_SETCURSEL ,sel,0);


}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNAILoadAiUnit() 
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

   static char BASED_CODE filter[] = "AIE Files|*.AIE|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
       CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   CAIObj * partPtr = NULL;
	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> partPtr;
	   the_in_Archive.Close();
       fl.Close();
	   m_gStatus->m_aiList->AddTail(partPtr);
	   if(m_gStatus->m_editorModeOn == TRUE){//if editor is available
	      AI_DBInList(m_gStatus->m_aiList->GetCount()-1);
	      ScriptInList(m_gStatus->m_scrList->GetCount()-1);
	   }
	 }
     SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNAISaveAiUnit() 
{
    if(!m_gStatus->m_aiList)
	return;
	
    if(m_gStatus->m_editorModeOn==FALSE)
    return;
    int runtimeIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_AI_DB,LB_GETCURSEL ,0,0);
    if(runtimeIndex == -1){
     MessageBox("Must Select a bot");
     return;
	}
    POSITION posLoc = m_gStatus->m_aiList->FindIndex(runtimeIndex);
    if(!posLoc)
       return;
       CAIObj * partPtr = (CAIObj *)m_gStatus->m_aiList->GetAt(posLoc);
	   

	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization
	static char BASED_CODE filter[] = "AIE Files|*.AIE|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "AIE", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << partPtr;
       the_out_Archive.Close();
       fl.Close();

	 }

	SetCurrentDirectory(MainDir);
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNMNUExportVisual() 
{
    int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}
    int visIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_UnitList,LB_GETCURSEL ,0,0);
	if(visIndex == -1){
	   MessageBox("must select a visual");
	   return;
	}

    POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);

		posloc = menuObj->m_visualItems->FindIndex(visIndex);
	    if(!posloc)
		    return;
            CMenuVisObj * visObj = (CMenuVisObj *)menuObj->m_visualItems->GetAt(posloc);
		
        //use gamefiles folder for organization
        CString CurDir;
        CString MainDir;
        char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
        //end use gamefiles folder for organization
        static char BASED_CODE filter[] = "MVU Files|*.MVU|All Files (*.*)|*.*||";
        CFileDialog opendialog( FALSE, "MVU", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
			 CMenuVisObj * copyMem = NULL;
             visObj->Clone(&copyMem,NULL);

             CArchive the_out_Archive(&fl,CArchive::store);
             the_out_Archive << copyMem;
             the_out_Archive.Close();
             fl.Close();

		}
	    SetCurrentDirectory(MainDir);  
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNMNUImportVisual() 
{
	int menuIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_MainDB,LB_GETCURSEL ,0,0);
	if(menuIndex == -1){
	   MessageBox("must select a menu");
	   return;
	}

    POSITION posloc = m_gStatus->m_menuDatabase->FindIndex(menuIndex);
	if(!posloc)
		return;
        CMenu3DObj * menuObj = (CMenu3DObj *)m_gStatus->m_menuDatabase->GetAt(posloc);

     //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "MVU Files|*.MVU|All Files (*.*)|*.*||";
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
	   CMenuVisObj * visObj = NULL;
	   the_in_Archive >> visObj;
	   the_in_Archive.Close();
       fl.Close();  
	   //Prep then add baby!!
       CMenuVisObj * cloneCleanObj = NULL;
       visObj->Clone(&cloneCleanObj,NULL);
	   //visObj->SafeDelete();
	   //delete visObj;
	   //visObj = 0;

	   SetCurrentDirectory(MainDir);	
 
	   int visIndex = m_menuDBDlgBar.SendDlgItemMessage(IDC_LIST_MENU_UnitList,LB_GETCURSEL ,0,0);
	   if(visIndex == -1){
	      menuObj->m_visualItems->AddTail(cloneCleanObj);
	   }
	   else
	   {
          POSITION selPosVix = menuObj->m_visualItems->FindIndex(visIndex);
	      if(!posloc)
		      menuObj->m_visualItems->AddTail(cloneCleanObj);
		  else
			  menuObj->m_visualItems->InsertBefore(selPosVix,cloneCleanObj);

	   }



	   
	   if(m_gStatus->m_editorModeOn == TRUE)
	      MenuVisualsInList(menuObj->m_visualItems->GetCount()-1);  
	 }
     SetCurrentDirectory(MainDir);		
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNAIImportCollisionSys() 
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

   static char BASED_CODE filter[] = "CUI Files|*.CUI|All Files (*.*)|*.*||";
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
	   CCollisionObj * cObj = NULL;
	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> cObj;
	   the_in_Archive.Close();
       fl.Close();
	   m_gStatus->m_collisionDatabase->AddTail(cObj);
	 }
	 InListCollisionDB(m_gStatus->m_collisionDatabase->GetCount()-1);
     SetCurrentDirectory(MainDir);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNAIDeleteSys() 
{
	int runtimeIndex = m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_collisionDatabases,LB_GETCURSEL ,0,0);
    if(runtimeIndex == -1){
       MessageBox("Must select a collision unit...");
	   return;
	}

	POSITION posLoc = m_gStatus->m_collisionDatabase->FindIndex(runtimeIndex);
	if(!posLoc)
		return;

    CCollisionObj * cObj = (CCollisionObj *)m_gStatus->m_collisionDatabase->GetAt(posLoc);
	   
	cObj->SafeDelete();
	delete cObj;
	cObj = 0;
	m_gStatus->m_collisionDatabase->RemoveAt(posLoc);
	
	InListCollisionDB(runtimeIndex-1);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNAIExportCurrentCol() 
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
     static char BASED_CODE filter[] = "CUI Files|*.CUI|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "CUI", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_gStatus->m_collisionBase;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNAISaveColDB() 
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
     static char BASED_CODE filter[] = "CRD Files|*.CRD|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "CRD", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_gStatus->m_collisionDatabase;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNAILoadColDB() 
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

   static char BASED_CODE filter[] = "CRD Files|*.CRD|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   SetCurrentDirectory(MainDir);
	   m_gStatus->LoadAICollisionDB(fileName);    
	 }
     SetCurrentDirectory(MainDir);
	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::InListCollisionDB(int sel)
{
	m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_collisionDatabases,LB_RESETCONTENT,0,0);	
	for(POSITION posLoc = m_gStatus->m_collisionDatabase->GetHeadPosition();posLoc != NULL;)
	{
        CCollisionObj * cObj = (CCollisionObj *)m_gStatus->m_collisionDatabase->GetNext(posLoc);
		m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_collisionDatabases,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Collision Object");
	}

    if(sel > -1)
	   m_aiDBDlgBar.SendDlgItemMessage(IDC_LIST_collisionDatabases,LB_SETCURSEL ,sel,0);

}*/
//------------------------------------------------------------------------------------//
/*void EngineXWin::OnBTNMISSaveUnit() 
{
#ifndef _LimitedEngine
	int missileIndex = m_missileDlgBar.SendDlgItemMessage(IDC_LIST_MIS_missileDataBase,LB_GETCURSEL ,0,0);
     if(missileIndex == -1){
        MessageBox("Must Select a missile database object");
	    return;
	 }
     POSITION posLoc = m_gStatus->m_missileDB->FindIndex(missileIndex);
	 if(!posLoc)
		 return;
         CMissileObj* misPtr = (CMissileObj*)m_gStatus->m_missileDB->GetAt(posLoc);
		
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
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNMISLoadUnit() 
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
	   m_gStatus->m_missileDB->AddTail(misPtr);
	   if(m_gStatus->m_editorModeOn == TRUE)
	      MissileDatabaseInList(m_gStatus->m_missileDB->GetCount()-1);
	 }
     SetCurrentDirectory(MainDir);	
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
/*
void EngineXWin::OnBTNSPNExportSys() 
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
             the_out_Archive << m_gStatus->m_spawnGeneratorDB << m_gStatus->GL_aiWebDatabase;
             the_out_Archive.Close();
             fl.Close();
		}
	    SetCurrentDirectory(MainDir);  	
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNSPNImportSys() 
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
       if(m_gStatus->m_spawnGeneratorDB)
	   {
          m_gStatus->m_spawnGeneratorDB->SafeDelete();
		  delete m_gStatus->m_spawnGeneratorDB;
		  m_gStatus->m_spawnGeneratorDB = 0;
	   }
	   if(m_gStatus->GL_aiWebDatabase)
	   { 
          m_gStatus->GL_aiWebDatabase->SafeDelete();
		  delete m_gStatus->GL_aiWebDatabase;
          m_gStatus->GL_aiWebDatabase = 0;
	   }

	   CArchive the_in_Archive(&fl,CArchive::load);
	   the_in_Archive >> m_gStatus->m_spawnGeneratorDB >> m_gStatus->GL_aiWebDatabase;
	   the_in_Archive.Close();
       fl.Close();  
	   //Prep then add baby!!
	   SetCurrentDirectory(MainDir);	
	   if(m_gStatus->m_editorModeOn == TRUE){
	      SpawnGenDatabaseInList(0);
		  if(this->m_pAISystemDlg)
			this->m_pAISystemDlg->AITreeInList(-1);
	   }
	 }
     SetCurrentDirectory(MainDir);	
}
*/

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPSearchForAccount() 
{

}
//----------------------------------------------------------------//
void EngineXWin::OnBTNTCPRestoreBackup() 
{
	
}
//----------------------------------------------------------------//
void EngineXWin::OnBTNTXTSymbolDB() 
{
#ifndef _ClientOutput
    CSymbolMapDlg dlg;
	dlg.m_symbolMapDBREF = m_gStatus->m_symbolMapDB;
	dlg.DoModal();
	m_gStatus->m_symbolMapDB = dlg.m_symbolMapDBREF;
#endif
}
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
void EngineXWin::OnCHECKEditorTxtMode() 
{
	if(m_editorTextModeOn)
	   m_editorTextModeOn = FALSE;
	else
	   m_editorTextModeOn = TRUE;
	
}
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
void EngineXWin::FormatAllNames()
{
   for(POSITION posLoc = m_gStatus->m_entObjList->GetHeadPosition();posLoc != NULL;)
   {
       CMovementObj * entLoc = (CMovementObj *)m_gStatus->m_entObjList->GetNext(posLoc);
       if(entLoc->m_itemDB)
          for(POSITION posLoc2 = entLoc->m_itemDB->GetHeadPosition();posLoc2 != NULL;)
		  {
              CItemObj * itemPtr = (CItemObj *) entLoc->m_itemDB->GetNext(posLoc2);
			  int length = itemPtr->m_itemName.GetLength();
			  CString newName;
              BOOL firstChar = TRUE;
              for(int loop=0;loop<length;loop++)
			  {
				  CString temp = (CString)itemPtr->m_itemName.GetAt(loop);
                  if(firstChar)
                     temp.MakeUpper();
				  else
                     temp.MakeLower();

				  firstChar = FALSE;

				  if(temp == " ")
					 firstChar = TRUE;

				  if(temp == "-")
					 firstChar = TRUE;

				  if(temp == ")")
					 firstChar = TRUE;

                  newName = operator+(newName,temp);

			  }
			  itemPtr->m_itemName = newName;
		  }
   }
   MessageBox("Completed");
}
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
void EngineXWin::OnBTNENTMassFormatNames() 
{
   if(AfxMessageBox("Are you sure you want to mass format all names.",MB_OKCANCEL,0)!=IDOK)
	return;
	FormatAllNames();
}

//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
BOOL EngineXWin::DeleteAccountsBasedOnFile()
{
/*CFileException exc;
     CFile fl;
	 CArchive the_Archive(&fl,CArchive::load);
     BOOL res = fl.Open("accountclean.txt",CFile::modeRead, &exc);
     if(res == FALSE)
	 {
        AfxMessageBox("Cannot Open File");
        return FALSE;
	 }

	 while(1)
	 {
		 CString username, password;
         if(!the_Archive.ReadString(username))
		    break;
         if(!the_Archive.ReadString(password))
		    break;
  
		 DeleteAccountByInfo(username,password);

	 }

	 fl.Close();*/
	 return TRUE;
}

//------------------------------------------------------------------------------------//
void EngineXWin::OnCHKOBJEnablePlaementHotKey() 
{
     if(m_placementHotKeyEnabled)
		m_placementHotKeyEnabled = FALSE;
	 else
		m_placementHotKeyEnabled = TRUE;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::SnapShotRenderSection() 
{
	m_gStatus->m_logRenderSection = TRUE;
	m_gStatus->m_totalWorldPolys = 0;
    m_totalEntitypolys = 0;
	CString filename = "rendersection.log";
    CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
	the_out_Archive.WriteString( "Cleared...");  
    the_out_Archive.WriteString( "\n");   
    the_out_Archive.Close();
    fl.Close(); 
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNOBJReportCurRenderCache() 
{
	SnapShotRenderSection();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNUIIconDBACCESS() 
{
#ifndef _ClientOutput
	CIconSysDlg dlg;
	dlg.m_texturedatabaseREF = m_gStatus->m_textureDataBase;
	dlg.m_iconDatabaseREF    = m_gStatus->m_iconDatabase;
    dlg.DoModal();
	m_gStatus->m_iconDatabase = dlg.m_iconDatabaseREF;
#endif
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPSnapShotPacketQue() 
{
	m_snapshotPacketQue = TRUE;
	
}
//---------------------------------------------------------------//
void EngineXWin::RenderStencilShadows2(D3DVECTOR locationVect, D3DMATRIX camMatrix)
{
    if(m_gStatus->m_globalShadowType == 0)
	   return;


    if(m_gStatus->m_shadowObjectsList->GetCount() == 0)
	   return;


    

    LARGE_INTEGER StartTime, EndTime;
    if(m_gStatus->GL_frequency.QuadPart != 0)QueryPerformanceCounter(&StartTime);

    m_gStatus->m_renderStateObject->SetDiffuseSource(1,m_gStatus->m_dxRenderer->m_pd3dDevice);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int1,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int2,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int3,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int4,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int5,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int6,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int7,m_gStatus->m_dxRenderer->m_pd3dDevice,m_gStatus->m_textureDataBase);
	//m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_gStatus->m_renderStateObject->SetLightingMode(FALSE,m_gStatus->m_dxRenderer->m_pd3dDevice);



	m_gStatus->m_dxRenderer->m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_STENCIL,0xff00bfff, 1.0f, 0L );
	m_gStatus->m_renderStateObject->SetStencilMode(TRUE,m_gStatus->m_dxRenderer->m_pd3dDevice);
    //m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
	m_gStatus->m_renderStateObject->SetZWritableState(FALSE,m_gStatus->m_dxRenderer->m_pd3dDevice);
    //m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE,     D3DSHADE_FLAT );
	m_gStatus->m_renderStateObject->SetShadeMode(1,m_gStatus->m_dxRenderer->m_pd3dDevice);//flat shade
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_ALWAYS );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILFAIL,  D3DSTENCILOP_KEEP );

    // If ztest passes, inc/decrement stencil buffer value
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILREF,       0x1 );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILMASK,      0xffffffff );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0xffffffff );
    

	m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_dxRenderer->m_pd3dDevice,8);
    DWORD         vertecCount = 0;
    POSITION posLoc;
		for(posLoc = m_gStatus->m_shadowObjectsList->GetHeadPosition();posLoc != NULL;)
		{//build all shadow volumes
            CTranslucentPolyObj * pObj = (CTranslucentPolyObj *)m_gStatus->m_shadowObjectsList->GetNext(posLoc);
		
	        if(!pObj->m_meshReference)
				continue;
			  
			    if(pObj->m_zValue > m_gStatus->m_globalShadowDist)
				   continue;
			
                 //restructure mesh for shadow render
                pObj->m_location._41 = pObj->m_location._41 - locationVect.x;
	            pObj->m_location._42 = pObj->m_location._42 - locationVect.y;
	            pObj->m_location._43 = pObj->m_location._43 - locationVect.z;

		}
        
        for(posLoc = m_gStatus->m_shadowObjectsList->GetHeadPosition();posLoc != NULL;)
		{//build all shadow volumes
             CTranslucentPolyObj * pObj = (CTranslucentPolyObj *)m_gStatus->m_shadowObjectsList->GetNext(posLoc);
			
			

	        if(!pObj->m_meshReference)
				continue;
			  
			    if(pObj->m_zValue > m_gStatus->m_globalShadowDist)
				   continue;
				
                D3DXVECTOR3 vLight;
                
                vLight.x = m_gStatus->m_environmentPtr->m_sunPosition.x;// + m._41;
                vLight.y = m_gStatus->m_environmentPtr->m_sunPosition.y;// + m._42;
                vLight.z = m_gStatus->m_environmentPtr->m_sunPosition.z;// + m._43;
				if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem)
			    {
                      vLight.x = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition.x;// + m._41;
                      vLight.y = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition.y;// + m._42;
                      vLight.z = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition.z;// + m._43;
			    }
				
				if(vLight.y < .1)
				   vLight.y = 1.0f;

				vLight = m_gStatus->m_matLib.Normalize(vLight);
				vLight.x = vLight.x * ((pObj->m_meshReference->m_meshBoundingSphereRadius+5)*4);
				vLight.y = vLight.y * ((pObj->m_meshReference->m_meshBoundingSphereRadius+5)*4);
				vLight.z = vLight.z * ((pObj->m_meshReference->m_meshBoundingSphereRadius+5)*4);
				D3DXMATRIXA16 m;
				D3DXMatrixInverse( &m, NULL, (D3DXMATRIX*)&pObj->m_location );
			    FLOAT Lx =   vLight.x;//m_gStatus->m_environmentPtr->m_sunPosition.x;
                FLOAT Ly =   vLight.y;//m_gStatus->m_environmentPtr->m_sunPosition.y;
                FLOAT Lz =   vLight.z;//m_gStatus->m_environmentPtr->m_sunPosition.z;
				vLight.x = Lx*m._11 + Ly*m._21 + Lz*m._31;
                vLight.y = Lx*m._12 + Ly*m._22 + Lz*m._32;
                vLight.z = Lx*m._13 + Ly*m._23 + Lz*m._33;
                
				DWORD       faceBufferVXcount = 0;
				vertecCount = 0;

				m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&pObj->m_location,m_gStatus->m_dxRenderer->m_pd3dDevice);
				m_gStatus->m_renderStateObject->SetCullState(2,m_gStatus->m_dxRenderer->m_pd3dDevice);
	            m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILZFAIL,      D3DSTENCILOP_INCR );
                
	            m_gStatus->m_dxRenderer->m_pd3dDevice->SetFVF( D3DFVF_XYZ );
				m_gStatus->m_dxRenderer->m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, vertecCount/3,m_rendermanagerEd->bufferVx, sizeof(D3DXVECTOR3) );
				m_gStatus->m_renderStateObject->SetCullState(1,m_gStatus->m_dxRenderer->m_pd3dDevice);//Clockwise
                m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR );
				m_gStatus->m_dxRenderer->m_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, vertecCount/3,m_rendermanagerEd->bufferVx, sizeof(D3DXVECTOR3) );
		}//end build all shadow volumes

		//m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
	
	m_gStatus->m_renderStateObject->SetCullState(1,m_gStatus->m_dxRenderer->m_pd3dDevice);
    //m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,     TRUE );
	m_gStatus->m_renderStateObject->SetZWritableState(TRUE,m_gStatus->m_dxRenderer->m_pd3dDevice);
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );
	m_gStatus->m_renderStateObject->SetFogState(m_gStatus->m_dxRenderer->m_pd3dDevice,FALSE);
	m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_dxRenderer->m_pd3dDevice,7);
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_gStatus->m_renderStateObject->SetBlendLevel(m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_dxRenderer->m_pd3dDevice);
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILREF,  0x1 );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );
    // Draw a big, gray square
	m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&m_gStatus->m_matrixInit2,m_gStatus->m_dxRenderer->m_pd3dDevice);
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetStreamSource( 0, m_gStatus->m_dxRenderer->m_shadowSquareVB, 0, sizeof(SHADOWVERTEX) );
    m_gStatus->m_dxRenderer->m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
    // Restore render states
    m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
	m_gStatus->m_renderStateObject->SetStencilMode(FALSE,m_gStatus->m_dxRenderer->m_pd3dDevice);
	m_gStatus->m_renderStateObject->m_currentVertexType = 99;
	m_gStatus->m_renderStateObject->SetShadeMode(0,m_gStatus->m_dxRenderer->m_pd3dDevice);//gauroud shade
	//m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    QueryPerformanceCounter(&EndTime);
    LONG64 ElapsedTime = EndTime.QuadPart - StartTime.QuadPart;
    m_gStatus->GL_testPerformance = (double(ElapsedTime)/ m_gStatus->GL_frequency.QuadPart)*1000.0f;
	

	//m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    m_gStatus->m_renderStateObject->SetLightingMode(TRUE,m_gStatus->m_dxRenderer->m_pd3dDevice);
	m_gStatus->m_renderStateObject->SetFogState(m_gStatus->m_dxRenderer->m_pd3dDevice,TRUE);
    return;
}


//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void EngineXWin::SpawnGenDataSQLOutput()
{
     CString commandBuild = "Provider=sqloledb;Data Source=";
	    commandBuild = commandBuild+m_gStatus->m_multiplayerObject->m_sqlServerIP;
		commandBuild = commandBuild+";Initial Catalog=";
		commandBuild = commandBuild+m_gStatus->m_multiplayerObject->m_sqlServerCatalog;
		commandBuild = commandBuild+";User Id=";
        commandBuild = commandBuild+m_gStatus->m_multiplayerObject->m_sqlServerUsername;
		commandBuild = commandBuild+";Password=";
		commandBuild = commandBuild+m_gStatus->m_multiplayerObject->m_sqlServerPassword;
		commandBuild = commandBuild+";";

	 _bstr_t deleteTableString = (LPSTR)"Delete SpawnGenData";
	 _bstr_t tableConv = "SpawnGenData";
	 _RecordsetPtr  pRstClear;
	 pRstClear.CreateInstance(__uuidof(Recordset));
	 _bstr_t strCnn(commandBuild);

	 //clear existing data
	 try
	 {
	   //execute connection method
       pRstClear->Open(
                   deleteTableString,//table
                   strCnn,
                   adOpenStatic,
                   adLockOptimistic,
                   adCmdText);
		   
	  }
	  catch(_com_error &e)
	  {
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			AfxMessageBox(bstrDescription);
	  }
	  //end clear existing data
     //add all new data

     BOOL enabled = TRUE;
	 _RecordsetPtr  pRst;
	 pRst.CreateInstance(__uuidof(Recordset));
	 try
     {
        pRst->Open(
             tableConv,//table
             strCnn,
             adOpenStatic,
             adLockOptimistic,
             adCmdTable);
	 }
	 catch(_com_error &e)
	 {
	        enabled = FALSE;
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			AfxMessageBox(bstrDescription);
			return;
	 }

	 try
     {
	 int trace = 0;
       for(POSITION posLoc = m_gStatus->m_spawnGeneratorDB->GetHeadPosition();posLoc != NULL;){
         CSpawnObj * spnPtr = (CSpawnObj *)m_gStatus->m_spawnGeneratorDB->GetNext(posLoc);
		   pRst->AddNew();
           pRst->Fields->GetItem("SpawnGenerator")->Value = (_bstr_t)spnPtr->List_spawnName;//Works
	       pRst->Fields->GetItem("SpawnCount")->Value = (_bstr_t)spnPtr->List_maxOutPutCapacity;//Works
		   pRst->Fields->GetItem("PercentSpawn")->Value = (_bstr_t)spnPtr->List_possibilityOfSpawn;//Works
          
		   CAIObj * aiPtr = m_gStatus->m_aiList->GetByIndex(spnPtr->m_aiCfg);
           if(aiPtr)
		   {//Invalid ai ptr
			  CString temp = m_gStatus->m_cabLib.ConvToString(spnPtr->m_aiCfg) + ") " + aiPtr->m_aiCfgName;
              pRst->Fields->GetItem("MobName")->Value = (_bstr_t)temp;
		   }
		   else
		      pRst->Fields->GetItem("MobName")->Value = (_bstr_t)"Not Found";


		   if(spnPtr->m_treeUse != -1){//tree override in use
              POSITION wPos = m_gStatus->GL_aiWebDatabase->FindIndex(spnPtr->m_treeUse);
			  if(wPos){//
                 CAIWebObj * wObj = (CAIWebObj *)m_gStatus->GL_aiWebDatabase->GetAt(wPos);
				     CString temp = m_gStatus->m_cabLib.ConvToString(spnPtr->m_treeUse) + ") " + wObj->m_name;
                     pRst->Fields->GetItem("TreeBind")->Value = (_bstr_t)temp;
				 }
		   }
		   else
              pRst->Fields->GetItem("TreeBind")->Value = (_bstr_t)"Not Found";

		   
		   pRst->Update();
	   }
	 }
	 catch(_com_error &e)
	 {
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			AfxMessageBox(bstrDescription);
			return;
	 }

     try
     {//TRY close table
		      pRst->Close();
	 }//END TRY close table
	 catch(_com_error &e)
     {//CATCH
		     
	 }//end CATCH
}
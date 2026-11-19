//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------free all memory then shut down----------------------------------------//
void EngineXWin::FreeAllThenClose()
{
	#ifndef _ClientOutput 
    
	if(m_axmedia)
	{  
       delete m_axmedia;
	   m_axmedia = 0;
	}

	// Delete Explosions Database dialog if it exists
	if(this->m_pExplosionsDbDlg)
	{
		delete this->m_pExplosionsDbDlg;
		this->m_pExplosionsDbDlg = 0;
	}

	// Delete Weapon System dialog if it exists
	if(this->m_pWeaponSystemDlg) 
	{
		delete this->m_pWeaponSystemDlg;
		this->m_pWeaponSystemDlg = 0;
	}

	// Delete Environment Settings dialog if it exists
	if(this->m_pEnvironmentSettingsDlg) 
	{
		delete this->m_pEnvironmentSettingsDlg;
		this->m_pEnvironmentSettingsDlg = 0;
	}

	// Delete AI System dialog if it exists
	if(this->m_pAISystemDlg) 
	{
		delete this->m_pAISystemDlg;
		this->m_pAISystemDlg = 0;
	}

	// Delete Spawn Generator dialog if it exists
	if(this->m_pSpawnGeneratorDlg) 
	{
		delete this->m_pSpawnGeneratorDlg;
		this->m_pSpawnGeneratorDlg = 0;
	}

	// Delete Texture Database dialog if it exists
	if(this->m_pTextureDatabaseDlg) 
	{
		delete this->m_pTextureDatabaseDlg;
		this->m_pTextureDatabaseDlg = 0;
	}

    /*if(m_newMalInterface)
	{
	   delete m_newMalInterface;
	   m_newMalInterface=0;
	}*/
    #endif

	if(m_gStatus)
	{
		if(m_gStatus->m_multiplayerObject)
		if(m_gStatus->m_multiplayerObject->m_eventLog)
		{
		   m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Packets Dropped during session",":",m_gStatus->m_multiplayerObject->m_packetsDropped);
		   m_gStatus->m_multiplayerObject->m_eventLog->OutPutTextFile("last.log");
		   m_gStatus->m_multiplayerObject->m_eventLog->SafeDelete();
		   delete m_gStatus->m_multiplayerObject->m_eventLog;
		   m_gStatus->m_multiplayerObject->m_eventLog = 0;
		}

		m_gStatus->SetAllRenderStatesIM();
	}

	if(m_currencyObject)
	{
       delete m_currencyObject;
	   m_currencyObject = 0;
	}
    if(m_currentWldsSpawnPointModule)
	{
	   delete m_currentWldsSpawnPointModule;
       m_currentWldsSpawnPointModule = 0;
	}
	if(m_accountViewerModule)
	{
       delete m_accountViewerModule;
       m_accountViewerModule = 0;
	}
	if(m_accountModule)
	{
       delete m_accountModule;
	   m_accountModule = 0;
	}
	if(m_entityViewerModule)
	{
       delete m_entityViewerModule;
       m_entityViewerModule = 0;
	}

	if(m_gStatus)
	   m_gStatus->WriteDebugFileLive(3,6);//DEBUG
    //delete network databases
	if(NetworkRecordList){
    for(POSITION posLoc = NetworkRecordList->GetHeadPosition();posLoc != NULL;){
        CNetworkKeyObj* ntPtr = (CNetworkKeyObj*)NetworkRecordList->GetNext(posLoc);
        delete ntPtr;
		ntPtr=0;
	}
	delete NetworkRecordList;
	}
	NetworkRecordList=0;

    if(m_globalSqlHandle)
	{
	    CloseHandle(m_globalSqlHandle);
        m_globalSqlHandle = NULL;
	}


	SafeDelete(m_menuThemeDatabase);

	//----if ui is available delete it------------------//
	if(m_gStatus)
    m_gStatus->WriteDebugFileLive(3,13);//DEBUG
		     m_dlgBarObject.DestroyWindow();
	         m_wndDlgBar.DestroyWindow();
             m_backgroundDlgBar.DestroyWindow();
	         m_controlsDlgBar.DestroyWindow();
	         m_moviesDlgBar.DestroyWindow();
	         m_entityDlgBar.DestroyWindow();
	         m_camVptDlgBar.DestroyWindow();
	         m_mouseDlgBar.DestroyWindow();
	         m_2DinterfaceBar.DestroyWindow();
	         m_enviormentBar.DestroyWindow();
			 m_textFontDlgBar.DestroyWindow();
			 //m_explosionDlgBar.DestroyWindow();
             m_musicDlgBar.DestroyWindow();
             m_missileDlgBar.DestroyWindow();
	         m_bulletHoleDlgBar.DestroyWindow();
			 m_multiPlayer.DestroyWindow();
			 m_animatedTextureDBDlgBar.DestroyWindow();
			 m_scriptDlgBar.DestroyWindow();
			 m_textureDBDlgBar.DestroyWindow();
             m_hudDBDlgBar.DestroyWindow();
			 m_menuDBDlgBar.DestroyWindow();
			 m_particleDBDlgBar.DestroyWindow();
	         m_spawnGenDBDlgBar.DestroyWindow();
			 m_haloEffectsDlgBar.DestroyWindow();
			 m_aiDBDlgBar.DestroyWindow();
             m_mouseDlgBar.DestroyWindow();
			 m_mainMenus.DestroyWindow();
			 m_mainCharacter.DestroyWindow();
			 m_mainEnvironment.DestroyWindow();
			 m_mainFX.DestroyWindow();

    if(m_gStatus)
       m_gStatus->WriteDebugFileLive(3,14);//DEBUG


    if(m_clientEngine)
	   delete m_clientEngine;
	   m_clientEngine = 0;


    if(m_newMalInterface)
		delete m_newMalInterface;
	m_newMalInterface = 0;

    CoUninitialize();

}
//------deletes a object in the object list based on trace name-----------------------//
void EngineXWin::DeleteObjectByTrace(int trace)
{//BEGIN
POSITION worldPos1,worldPos2;
//clean temp list
POSITION posLast;
for(POSITION posLoc = m_gStatus->m_worldListTemp->GetHeadPosition();(posLast = posLoc) != NULL;)
{     
	CWldObject * wldLoc = ( CWldObject* )m_gStatus->m_worldListTemp->GetNext(posLoc);
    if(wldLoc->List_identity == trace){
       m_gStatus->m_worldListTemp->RemoveAt(posLast);
       break;
	}
}
//clean normal
for(worldPos1 = m_gStatus->m_wldObjList->GetHeadPosition();(worldPos2 = worldPos1) != NULL;){//Begin world object loop
	CWldObject * wldLoc = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(worldPos1);	
	if(wldLoc->List_identity == trace){//found its ass
	   
	   m_gStatus->m_wldObjList->RemoveAt(worldPos2);
	   POSITION posLast = NULL;
	   for(POSITION posLoc = m_gStatus->m_worldRenderList->GetHeadPosition();(posLast = posLoc) != NULL;){
	       CWldObject * wldLoc2 = ( CWldObject* )m_gStatus->m_worldRenderList->GetNext(posLoc);
           if(wldLoc == wldLoc2){
              m_gStatus->m_worldRenderList->RemoveAt(posLast);
			  break;
		   }
	   }
	    
	   wldLoc->SafeDelete();
       delete wldLoc;
	   wldLoc=0;

	   return;
	}//end found its ass	
 }//end Begin world object loop
}//END


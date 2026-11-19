//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPIPStartHost() 
{
#ifndef _LimitedEngine

EnableServer();//REMOVE

#endif
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPIPStopHost() 
{
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPIPJoinHost() 
{
m_gStatus->m_multiplayerObject->EnableClient(m_gStatus->m_multiplayerObject->m_IPaddress,0);//REMOVE
}
//------------------------------------------------------------------------------------//
void EngineXWin::UpdateMonitor()
{

if(!m_gStatus->m_multiplayerObject->m_eventLog)
return;
   int insertPoint = 0;
   for(POSITION posLoc = m_gStatus->m_multiplayerObject->m_eventLog->GetTailPosition();posLoc != NULL;)
   {
       CServerEventObj * sEventObj = (CServerEventObj *)m_gStatus->m_multiplayerObject->m_eventLog->GetPrev(posLoc);
       if(sEventObj->m_posted == TRUE)
		  return;//posted all new messages
	      sEventObj->m_posted = TRUE;
          m_multiPlayer.SendDlgItemMessage(IDC_LIST_TCPIP_Monitor,LB_INSERTSTRING,insertPoint,(LPARAM)(LPCTSTR)sEventObj->m_eventDescription);	
          insertPoint++;
   }
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPDisconnectPlayer() 
{

}
//------------------------------------------------------------------------------------//
BOOL EngineXWin::EnableServer()
{
	if(m_gStatus->m_multiplayerObject->m_serverStarted == TRUE){
       AfxMessageBox("Error starting :: Server Already Started");
	   return FALSE;
	}

    if(!m_gStatus->m_multiplayerObject->EnableServer())
		return FALSE;
	    
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void EngineXWin::OnChangeEDITTCPIPadress() 
{
	m_multiPlayer.GetDlgItemText( IDC_EDIT_TCP_IPadress,m_gStatus->m_multiplayerObject->m_IPaddress);
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnChangeTCPuserName() 
{
	m_multiPlayer.GetDlgItemText( IDC_TCP_userName,m_gStatus->m_multiplayerObject->m_userName);	
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnChangeTCPpassword() 
{
	m_multiPlayer.GetDlgItemText( IDC_TCP_password,m_gStatus->m_multiplayerObject->m_passWord);	
}
//------------------------------------------------------------------------------------//
void EngineXWin::OnBtnUiStatdb() 
{
#ifndef _ClientOutput
   CStatisticsDBDlg dlg;

   dlg.statisticDBREF = m_gStatus->m_statDatabase;
   dlg.DoModal();
   m_gStatus->m_statDatabase = dlg.statisticDBREF;
#endif
}
//------------------------------------------------------------------------------------//
void EngineXWin::MaintnenceOnAccounts(BOOL reclaimMemory)
{
   
}

//------------------------------------------------------------------------------------//
void EngineXWin::OnBTNTCPUpdateAllAcounts() 
{

        CheckAccountDBSyncronization();
	    MessageBox("Completed...");
	  
}
//---------------------------------------------------------------------------//
/*void EngineXWin::AIdimIntegrityCheck()
{
  for(POSITION posLoc = m_gStatus->m_aiList->GetHeadPosition();posLoc != NULL;)
  {
    CAIObj * partPtr = (CAIObj *)m_gStatus->m_aiList->GetNext(posLoc);
	
    CMovementObj * DBPtr = m_gStatus->m_entObjList->GetObjByIndex(partPtr->m_entityCfgDBindexRef);   
    if(!DBPtr)
	{
        AfxMessageBox(partPtr->m_aiCfgName + " invalid edb index");
		continue;
	}

	if(DBPtr->List_skeletalObject->m_deformableMeshArrayCount == 0)
	   continue;

	if(partPtr->m_spawnCfgCount != DBPtr->List_skeletalObject->m_deformableMeshArrayCount) 
    {//Update dim cfg arrays if damaged
	   m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("CFG Mismatch Upgrading. Correcting: "+partPtr->m_aiCfgName);
       int oldCount = partPtr->m_spawnCfgCount;
	   if(partPtr->m_spawnCfgCount > 0 && oldCount > 0){     
	      partPtr->m_spawnCfgCount = DBPtr->List_skeletalObject->m_deformableMeshArrayCount;
          int cfgSelect = partPtr->m_spawnCfg[0];
		  if(partPtr->m_spawnCfg)
		     delete [] partPtr->m_spawnCfg;
			 partPtr->m_spawnCfg = 0;
					  
		     partPtr->m_spawnCfg = new int[partPtr->m_spawnCfgCount];
		     for(int tempLoop=0;tempLoop<partPtr->m_spawnCfgCount;tempLoop++)
			 {
				 partPtr->m_spawnCfg[tempLoop] = cfgSelect;
			 }			
	   }
    }//end Update dim cfg arrays if damaged
   }
}*/





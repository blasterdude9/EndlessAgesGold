#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include "direct.h"
#include <d3d9.h>
#include "CClientCommClass.h"

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CClientCommObj::HandleEngineMessagesClientSide(LPVOID lpMessage,
													DPNID idFromLoc,
													int msgSize,
													CMultiplayerObj * multiplayerObj,
													CMovementObjList * EntObjRuntime,
													CHousingCacheObjList * m_houseingCache,
													BOOL & m_pingInQuerry,
													int & CURPING,
													long & m_pingPersistStamp,
													int & MultiplayerDelayComm,
													CMenu3DObjList * menuList,
													LPDIRECT3DDEVICE9 d3dDevice,
													CtextReadoutObj * GL_systemReadout,
													CSymbolMapperObjList * m_symbolMapDB,
													CIconSysList * m_iconDatabase,
													CViewportList * m_runtimeViewportDB,
													CSpawnObjList * spawnGeneratorDB,
													BOOL & m_speechMode)
{
	//cast into an identity
    LPEXMSG_CATAGORIE identityMsg = (LPEXMSG_CATAGORIE)lpMessage;
    if(msgSize <= 0)
	   return TRUE;

	
	
	//-----------------------------------------------------------//
	if(identityMsg->m_catagorie == 5)
	{//equip msg
       LPEQUIP_STRUCT formatedMessage = (LPEQUIP_STRUCT)lpMessage;
	   if(multiplayerObj->GetEquipCRC(formatedMessage)==formatedMessage->CRCcheck)//check packet CRC
	      HandleEquipMessageClientSide(formatedMessage);
	   else{//bad CRC
            multiplayerObj->m_eventLog->AddEvent("Invalid Equip CRC from Server");
	   }//end bad CRC
	   delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end eqip msg
	//-----------------------------------------------------------//
	if(identityMsg->m_catagorie == 6)
	{//attrib msg
       LPATTRIB_STRUCT formatedMessage = (LPATTRIB_STRUCT)lpMessage;
	   if(multiplayerObj->GetAttribCRC(formatedMessage)==formatedMessage->CRCcheck)//check packet CRC
	      HandleAttribMessageClientSide(formatedMessage,idFromLoc,multiplayerObj,EntObjRuntime);
	   else{//bad CRC
            multiplayerObj->m_eventLog->AddEvent("Invalid Equip CRC from Server");
	   }//end bad CRC
	   delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end attrib msg
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 7)
	{//house block info ai server only
       return HandleHouseingBlock((BYTE *)lpMessage,msgSize,multiplayerObj);
	}//end house block info ai server only
    //---------------------------------------------------------------//
	if(identityMsg->m_catagorie == 8)
	{//spawn msg
       LPSPAWN_STRUCT formatedMessage = (LPSPAWN_STRUCT)lpMessage;
       HandleSpawnMessagesClientSide(formatedMessage,idFromLoc,multiplayerObj);
	   delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end spawn msg
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 10)
	{//item cfg msg
       LPITEM_LOCATION itemLocationMsg = (LPITEM_LOCATION)lpMessage;
	   BYTE * dataBlock = (BYTE*)lpMessage;

	   if(!multiplayerObj->m_requestingPortal)
       HandleItemLocationalMessagesClientSide(itemLocationMsg,(BYTE*)lpMessage);

	   delete [] dataBlock;
	   dataBlock = 0;

	   return TRUE;
	}//end item cfg msg
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 12)
	{//menu build msg
       LPMENUBUILD_STRUCT formatedMessage = (LPMENUBUILD_STRUCT)lpMessage;
	   if(multiplayerObj->GetBuildMenuMsgCrC(formatedMessage)==formatedMessage->CRCcheck){//check packet CRC
          HandleMenuBuildMessagesClientSide(formatedMessage,idFromLoc,menuList);
	   }//end check packet validity
	   else{//bad CRC
          multiplayerObj->m_eventLog->AddEvent("Invalid Menu Build CRC from Server");
	   }//end bad CRC
	   delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end menu build msg
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 19)
	{//mov block
       for(POSITION tagPos = EntObjRuntime->GetHeadPosition();tagPos != NULL;)
	   {//initialize all entities
           CMovementObj * mPtr = (CMovementObj *)EntObjRuntime->GetNext(tagPos);
		   mPtr->m_validThisRound = FALSE;
	   }//end initialize all entities
	   m_houseingCache->FlagAllInvalid();//set flag on all current houses in cache
		
	   //handleping
	   m_pingInQuerry = FALSE;
	   multiplayerObj->m_lastMovMsgRecievedTime = GetTickCount();
       CURPING = GetTickCount()-m_pingPersistStamp;
	   
	   
	   if(CURPING < multiplayerObj->m_frequencyLimit){
		  MultiplayerDelayComm = multiplayerObj->m_frequencyLimit+25;
          multiplayerObj->m_communicationFrequency = multiplayerObj->m_frequencyLimit+25;
		  multiplayerObj->m_retrySendDuration = multiplayerObj->m_frequencyLimit*2;
	   }
	   else{
	   
	   MultiplayerDelayComm = CURPING+50;
       if(MultiplayerDelayComm > 1500)
		  MultiplayerDelayComm = 1500;

	   multiplayerObj->m_retrySendDuration = MultiplayerDelayComm*2;

	   multiplayerObj->m_communicationFrequency = MultiplayerDelayComm;
	   }

	   //handle mov data
	   HandleClientMovBlockData(lpMessage,msgSize,idFromLoc,multiplayerObj);

	   //remove any entities that are not valid anymore
       POSITION tagPosLast = NULL;
       for(tagPos = EntObjRuntime->GetHeadPosition();(tagPosLast = tagPos) != NULL;)
	   {
           CMovementObj * mPtr = (CMovementObj *)EntObjRuntime->GetNext(tagPos);
		   if(mPtr->m_validThisRound == FALSE && mPtr->List_networkEntity == TRUE)
		   {//remove
              
			  m_runtimeViewportDB->DeleteByEntTrace(mPtr->List_traceNumber);
			   //DestroyRuntimeEntity(mPtr,0);
			  mPtr->SafeDelete(d3dDevice,spawnGeneratorDB);
			  delete mPtr;
	          EntObjRuntime->RemoveAt(tagPosLast);
		   }//end remove
	   }
	   m_houseingCache->RemoveAllInvalid();//remove All that were not in current list


	   return TRUE;
	}//end mov block
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 20)
	{//list block data (GLIDs)
       return HandleClientListGlidBlockData(lpMessage,msgSize,idFromLoc,multiplayerObj);
	}//end list block data (GLIDs)
    //---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 21)
	{//entity cfg block
	   BYTE * byteBlockData = (BYTE*)lpMessage;
	   HandleEntityCfgBlock(lpMessage,msgSize,idFromLoc,multiplayerObj);
	   delete [] byteBlockData;
	   byteBlockData = 0;
	   return TRUE;
	}//end entity cfg block
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 24)
	{//bank block transfer
       return HandleBankDataBlock(lpMessage,msgSize,idFromLoc,multiplayerObj);
	}//end bank block transfer
    //---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 25)
	{//store block transfer
	   return HandleStoreDataBlock(lpMessage,msgSize,idFromLoc,multiplayerObj);
	}//end store block transfer
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 26)
	{//store block transfer
	   return HandleHouseStoreDataBlock(lpMessage,msgSize,idFromLoc,multiplayerObj);
	}//end store block transfer
    //---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 28)
	{//misc msg handler
       LPMSGHANDLER formatedMessage = (LPMSGHANDLER)lpMessage;

       delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end store block transfer
    //---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 29)
	{//handle loot block
       return HandleLootBlockData(lpMessage,msgSize,idFromLoc);
	}//end handle loot block
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 30)
	{//sync timing for enviorments
       LPTIMESYNC_DATA formatedMessage = (LPTIMESYNC_DATA)lpMessage;
       
       multiplayerObj->m_serverStartTime = GetTickCount() - formatedMessage->m_timePassed;
	   multiplayerObj->m_frequencyLimit = formatedMessage->m_freqState;

       delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end sync timing for enviorments
	if(identityMsg->m_catagorie == 31)
	{//recieve group data
	   return ParseGroupListData((BYTE*)lpMessage,msgSize);
	}//end recieve group data
    if(identityMsg->m_catagorie == 32)
	{//recieve group data
	   return ParseClanRosterData((BYTE*)lpMessage,msgSize);
	}//end recieve group data
	//----------------------------------
	if(identityMsg->m_catagorie == 33)
	{//recieve quests list
	   LPBLOCKQUEST_DATA formatedMessage = (LPBLOCKQUEST_DATA)lpMessage;
       BYTE * byteBlockData = (BYTE*)lpMessage;
	   //open menu by id
	   for(POSITION posLoc = menuList->GetHeadPosition();posLoc != NULL;)
	   {//1
           CMenu3DObj * menu = (CMenu3DObj *)menuList->GetNext(posLoc);
	       if(menu->m_closeOnEsc)
			  menu->CloseMenu();
	   }//end 1
	   OpenMenu(91,0,0,0,FALSE);

	   //parse out test objects
	   int tracerBuild = sizeof(BLOCKQUEST_DATA);
	   while(tracerBuild < msgSize)
	   {
		   int curStrLen = byteBlockData[tracerBuild];
		   char * listText = new char[curStrLen+1];
		   tracerBuild++;

           for(int loop=0;loop<curStrLen;loop++)
		   {
		       listText[loop] = byteBlockData[tracerBuild];
			   tracerBuild++;
		   }   
		   listText[curStrLen] = 0;//null terminator
			   
		   menuList->AddStringToMenuList(91,listText,-1,formatedMessage->m_itemAssignedID,0,listText,0,"");
	   }

       delete [] byteBlockData;
	   byteBlockData=0;	
	   return TRUE;
	}//end recieve quests list
	if(identityMsg->m_catagorie == 34)
	{//recieve abilities available
	   BYTE * byteBlockData = (BYTE*)lpMessage;
	   HandleSkillAbilitiesClientSide(byteBlockData,msgSize,idFromLoc);
	   delete [] byteBlockData;
	   byteBlockData=0;	
	   return TRUE;
	}//end recieve abilities available
    if(identityMsg->m_catagorie == 35)
	{//recieve existing skills
	   BYTE * byteBlockData = (BYTE*)lpMessage;
	   HandleSkillsOutputClientSide(byteBlockData,msgSize,idFromLoc);
	   delete [] byteBlockData;
	   byteBlockData=0;	
	   return TRUE;
	}//end recieve existing skills
    //----------------------------------
	if(identityMsg->m_catagorie == 36)
	{//recieve arena list-check world avaiability
	   LPBLOCKQUEST_DATA formatedMessage = (LPBLOCKQUEST_DATA)lpMessage;
       BYTE * byteBlockData = (BYTE*)lpMessage;
	   //open menu by id
	   for(POSITION posLoc = menuList->GetHeadPosition();posLoc != NULL;)
	   {//1
           CMenu3DObj * menu = (CMenu3DObj *)menuList->GetNext(posLoc);
	       if(menu->m_closeOnEsc)
			  menu->CloseMenu();
	   }//end 1
	   
	   CString arenaName;

	   //parse out test objects
	   int loopTrace = 0;
	   BOOL openMenuAllGood = TRUE;
	   int tracerBuild = sizeof(BLOCKQUEST_DATA);
	   while(tracerBuild < msgSize)
	   {
		   int curStrLen = byteBlockData[tracerBuild];
		   char * listText = new char[curStrLen+1];
		   tracerBuild++;

           for(int loop=0;loop<curStrLen;loop++)
		   {
		       listText[loop] = byteBlockData[tracerBuild];
			   tracerBuild++;
		   }   
		   listText[curStrLen] = 0;//null terminator
		   if(loopTrace == 0)
		      arenaName = listText;
		      
		   else
		   {//validate world
		      CString arenaWorldFile = listText;
              //use gamefiles folder for organization
              CString CurDir;
              CString MainDir;
              char buffer[_MAX_PATH];
			  _getcwd( buffer, _MAX_PATH );
			  MainDir = CurDir = buffer;
			  CurDir = operator+(CurDir,"\\GameFiles\\");
			  SetCurrentDirectory(CurDir);
              //end use gamefiles folder for organization 
              CFileException exc;
              CFile fl;
              BOOL res = fl.Open(arenaWorldFile,CFile::modeRead | CFile::shareDenyWrite, &exc);
	          if(res == FALSE){
		         GL_systemReadout->AddLine(arenaName + " Is required for Arena Competition, contact avaria@endlessages.com for information",d3dDevice,m_symbolMapDB,0.1f,1.0f,0.1f,m_speechMode);
		         openMenuAllGood = FALSE;
	          }
			  fl.Close();
              SetCurrentDirectory(MainDir);
			  break;
		   }//end validate world

		   

		   loopTrace++;
	   }

	   if(openMenuAllGood)
	   {
		      OpenMenu(91,0,0,0,FALSE);
		      menuList->AddStringToMenuList(91,arenaName,-1,formatedMessage->m_itemAssignedID,0,arenaName,0,"");
	   }

       delete [] byteBlockData;
	   byteBlockData=0;	
	   return TRUE;
	}//end recieve arena list
	if(identityMsg->m_catagorie == 37)
	{//recieve icon display
	   BYTE * memoryArray = (BYTE*)lpMessage;
       int structureSz = sizeof(ICON_POS);
 
       int iconDispCount = msgSize/structureSz;

       if(iconDispCount < 1)
	   {
          delete [] memoryArray;
		  memoryArray = 0;
          return FALSE;
	   }
       
       ICON_POS * iconList = new ICON_POS[iconDispCount];
	   BYTE * dataRefCast = (BYTE*)iconList;  
       //fill list
       for(int fillerCount=1;fillerCount<msgSize;fillerCount++)
	   {
           dataRefCast[fillerCount-1] = memoryArray[fillerCount];
	   }
	   //get menu
	   CMenu3DObj * menuObj = NULL;
	   for(POSITION rMenuPos = menuList->GetHeadPosition();rMenuPos != NULL;)
	   {
           menuObj  = (CMenu3DObj *)menuList->GetNext(rMenuPos);
		   if(menuObj->m_specialOperation != 0)
              break;
	   }
	   //fill menu buffer for icon overlays
	   if(menuObj)
	   for(int loop=0;loop<iconDispCount;loop++)
	   {
           
		   D3DVECTOR screenPos;
		   screenPos.x = iconList[loop].m_xPos;
           screenPos.y = iconList[loop].m_yPos;
		   screenPos.z = 3.0f;

           if(!menuObj->m_iconDynamicOverlays)
			   menuObj->m_iconDynamicOverlays = new CIconSysList();

		       menuObj->m_iconDynamicOverlays->AddIconFromDBtoThisDB(m_iconDatabase,
										                             iconList[loop].m_iconDBIndex,
										                             screenPos,
										                             d3dDevice);
	   }

       delete [] iconList;
	   iconList = 0;

	   delete [] memoryArray;
	   memoryArray = 0;
	   return TRUE;
	}//end recieve icon display
    if(identityMsg->m_catagorie == 38)
	{//recieve gurenteed block data
	   BYTE * blockCast = (BYTE*)lpMessage;
	   LPENTITYGUPDATE_HEADER formatedMessage = (LPENTITYGUPDATE_HEADER)lpMessage;
       int dataTracer = sizeof(ENTITYGUPDATE_HEADER);

       if(formatedMessage->m_attributeMsgCount > 0)
	   {//attribute batch coming in
		  int byteSize = sizeof(ATTRIB_STRUCT)*formatedMessage->m_attributeMsgCount;
	      ATTRIB_STRUCT * attribArray = new ATTRIB_STRUCT[formatedMessage->m_attributeMsgCount];
		  BYTE * atCast = (BYTE*)attribArray;
		  
		  for(int i=0;i<byteSize;i++)
		  {
              atCast[i] = blockCast[dataTracer];
			  dataTracer++;
		  }
		  for(int atLoop=0;atLoop<formatedMessage->m_attributeMsgCount;atLoop++)
		  {
           HandleAttribMessageClientSide(&attribArray[atLoop],idFromLoc,multiplayerObj,EntObjRuntime);
		  }
		  delete [] attribArray;
		  attribArray = 0;
	   }//end attribute batch coming in
	   if(formatedMessage->m_equipMsgCount > 0)
	   {//equip batch coming in
		  int byteSize = sizeof(EQUIP_STRUCT)*formatedMessage->m_equipMsgCount;
	      EQUIP_STRUCT * equipArray = new EQUIP_STRUCT[formatedMessage->m_equipMsgCount];
		  BYTE * atCast = (BYTE*)equipArray;
	      for(int i=0;i<byteSize;i++)
		  {
              atCast[i] = blockCast[dataTracer];
			  dataTracer++;
		  }
		  for(int atLoop=0;atLoop<formatedMessage->m_equipMsgCount;atLoop++)
		  {
           HandleEquipMessageClientSide(&equipArray[atLoop]);
		  }
		  delete [] equipArray;
		  equipArray = 0;
	   }//end equi batch coming in
	   if(formatedMessage->m_txtMsgCount > 0)
	   {//txt batch coming in
		  for(int txtLoop=0;txtLoop<formatedMessage->m_txtMsgCount;txtLoop++)
		  {//txt msg loop
             int txtMsgSz = blockCast[dataTracer];
			 dataTracer++;
			 if(txtMsgSz > 0)
			 {//valid sz
                BYTE * tempBlock = new BYTE[txtMsgSz];
                for(int i=0;i<txtMsgSz;i++)
				{
                    tempBlock[i] = blockCast[dataTracer];
			        dataTracer++;
				}
                HandleTextMessageClientSide(tempBlock,txtMsgSz,idFromLoc);
				delete [] tempBlock;
				tempBlock = 0;
			 }//end valid sz
		  }//end txt msg loop
	   }//end txt batch coming in
	   if(formatedMessage->m_updateStatMsgCount > 0)
	   {//statistic batch coming in
		  int statBlockSize = blockCast[dataTracer];
		  dataTracer++;
		  if(statBlockSize > 0)
		  {
	       BYTE * statUpdateBlock = new BYTE[statBlockSize];		  
		   for(int i=0;i<statBlockSize;i++)
		   {
              statUpdateBlock[i] = blockCast[dataTracer];
			  dataTracer++;
		   }
		   HandleClientStatBlockData(statUpdateBlock,statBlockSize,idFromLoc);
		  }
	   }//end statistic batch coming in
	   if(formatedMessage->m_miscMsgCount > 0)
	   {//misc batch coming in
		 for(int miscLoop=0;miscLoop<formatedMessage->m_miscMsgCount;miscLoop++)
		 {
		  BYTE castByteTemp[2];
          castByteTemp[0] = blockCast[dataTracer];
		  dataTracer++;
		  castByteTemp[1] = blockCast[dataTracer];
		  dataTracer++;
		  short miscBlockSize = 0;
		  CopyMemory(&miscBlockSize,castByteTemp,sizeof(short));
		  //int miscBlockSize = blockCast[dataTracer];
		  //dataTracer++;
		  if(miscBlockSize > 0)
		  {//1
	       BYTE * miscUpdateBlock = new BYTE[miscBlockSize];		  
		   for(int i=0;i<miscBlockSize;i++)
		   {
              miscUpdateBlock[i] = blockCast[dataTracer];
			  dataTracer++;
		   }
		   HandleMiscBlocksClientSide(miscUpdateBlock,miscBlockSize);
		   if(miscUpdateBlock)
			  delete [] miscUpdateBlock;
		      miscUpdateBlock = 0;
		  }//end 1
		 }
	   }//end misc batch coming in

	   delete [] blockCast;
	   blockCast=0;

       return TRUE;
	}//end recieve gurenteed block data
	if(identityMsg->m_catagorie == 40)
	{//recieve existing skills
	   LPAIDYNSPAWN_POS formattedMsg = (LPAIDYNSPAWN_POS)lpMessage;
	   D3DVECTOR currentPosCnv;
	   currentPosCnv.x = formattedMsg->m_xPos;
	   currentPosCnv.y = formattedMsg->m_yPos;
	   currentPosCnv.z = formattedMsg->m_zPos;
	   SpawnEntityBasedOnPositionAndCfg(currentPosCnv,formattedMsg->m_aiCfg,formattedMsg->m_dynaID,formattedMsg->m_lifeLine);


	   delete formattedMsg;
	   formattedMsg=0;	
	   return TRUE;
	}//end recieve existing skills

	multiplayerObj->m_eventLog->AddEvent("Unknown catagorie ","byte:",identityMsg->m_catagorie);
	//disconeect the mofo
	
	return FALSE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles Attribute messages--------------------------------------------//
BOOL CClientCommObj::HandleAttribMessageClientSide(LPATTRIB_STRUCT formatedMessage,
												   DPNID idFromLoc,
												   CMultiplayerObj * multiplayerObj,
												   CMovementObjList * EntObjRuntime)
{
       if(formatedMessage->attribute == 3){//update on players health
		  CMovementObj * attacker = NULL;
		  if(formatedMessage->uniInt2 > -1)
             attacker = EntObjRuntime->GetObjByNetworkID(formatedMessage->uniInt2);
		     

          CMovementObj * mPtr = EntObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		  if(mPtr){
			   int oldEnergy = mPtr->List_energy;
			   mPtr->List_energy = formatedMessage->uniInt;
			   mPtr->List_maxEnergy = formatedMessage->uniShort3;
			   
			   if(oldEnergy > mPtr->List_energy){//under attack
				//m_screenFlashModule->DoIt(100,g_pd3dDevice);
			     if(attacker){//log attacker trace
					 if(attacker->List_traceNumber != mPtr->List_traceNumber)
					 {//dont aggro self
					    mPtr->List_attackerTrace = attacker->List_traceNumber;
                        if(mPtr->m_aiObj)
						   mPtr->m_curHitCounter++;
					 }
				 }
			   }//end under attack
		  }
	   }//end update on players health
	   if(formatedMessage->attribute == 7){//dissipate entity that died
         
	   }//end dissipate entity that died
       if(formatedMessage->attribute == 8){//set network id for communication purposes
		   //MessageBox("Communications set");
		  for(POSITION posLoc = EntObjRuntime->GetHeadPosition();posLoc != NULL;)
		  {//e loop
              CMovementObj * selfPtr = ( CMovementObj* ) EntObjRuntime->GetNext(posLoc);
              if(selfPtr->List_traceNumber == formatedMessage->idRefrence){//found him
				 //MessageBox("Enter the dragon");
                 selfPtr->List_networkDefinedID = formatedMessage->uniInt;//set
                 //request my start up cfgs
				 multiplayerObj->SendAttribMsg(DPNSEND_GUARANTEED,
			                     DPID_SERVERPLAYER,
					             1,
				                 0,
				 	             selfPtr->List_networkDefinedID,
						         selfPtr->List_networkDefinedID,
							     0,
							     NULL);
				 selfPtr->List_energy = formatedMessage->uniInt2;
				 selfPtr->List_maxEnergy = formatedMessage->uniShort3;
				 multiplayerObj->m_eventLog->AddEvent("Energy is set:"," UniInt2 ",selfPtr->List_energy);
				 return TRUE;
			  }//end found him
		  }//end e loop
          multiplayerObj->m_eventLog->AddEvent("Comm assignment did not find trace:"," idRef ",formatedMessage->idRefrence);//output for ai server
	   }//end set network id for communication purposes
	   if(formatedMessage->attribute == 9){//ok to mount entity
          CMovementObj * mPtr = EntObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		  if(mPtr){//valid
		     POSITION posLoc = EntObjRuntime->FindIndex(0);
	         if(posLoc)
			 {//user is here
		        CMovementObj * hostPtr = ( CMovementObj* ) EntObjRuntime->GetAt(posLoc);
                if(hostPtr->List_controlType == 0){//main player entity
		           hostPtr->Mount(mPtr);
				}//end main player entity
			 }//end user is here
		  }//end valid
	   }//end ok to mount entity
	   if(formatedMessage->attribute == 10){//delete entity
          POSITION posLast;
		   for(POSITION posLoc = EntObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
		   {
               CMovementObj * mPtr = ( CMovementObj* ) EntObjRuntime->GetNext(posLoc);
               if(formatedMessage->idRefrence == mPtr->List_networkDefinedID)
			   {//found it
				  DestroyRuntimeEntity(mPtr,0);
				  mPtr=0;
                  EntObjRuntime->RemoveAt(posLast);
			   }//end found it
		   }
	   }//end delete entity
	   if(formatedMessage->attribute == 11){//demount entity
          //CMovementObj * mPtr = EntObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		  //if(mPtr){//valid
		     POSITION posLoc = EntObjRuntime->FindIndex(0);
	         if(posLoc)
			 {//user is here
		        CMovementObj * hostPtr = ( CMovementObj* ) EntObjRuntime->GetAt(posLoc);
                if(hostPtr->List_controlType == 0){//main player entity
					hostPtr->DeMount();
				}//end main player entity
			 }//end user is here
		  //}//end valid
	   }//end demount entity
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 12){//tell entity (ai) target has been eliminated
          CMovementObj * mPtr = EntObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		  if(mPtr){//valid		     
             if(mPtr->m_aiObj){//ai
			    mPtr->List_AI_target = -1;//set target to none
		        mPtr->List_targetRef = NULL;
			 }//end ai
		  }//end valid
	   }//end //tell entity (ai) target has been eliminated
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 14){//confermation of deleteion of character
          DeleteStringFromMenuList(50,formatedMessage->uniInt);
	   }//end confermation of deleteion of character
       //-----------------------------------------------------------
	   if(formatedMessage->attribute == 15){//change def cfg on entity
          CMovementObj * mPtr = EntObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		  if(mPtr){//valid		     
		     if(mPtr->List_skeletalObject->m_deformableMeshArrayCount <= formatedMessage->uniInt)
	            return FALSE;

                POSITION posRef = EntObjList->FindIndex(mPtr->List_refModel);
	            if(!posRef)
		            return FALSE;
                    CMovementObj * dbPtr = (CMovementObj *) EntObjList->GetAt(posRef);

                    if(!dbPtr->List_skeletalObject->HowSwapDim(formatedMessage->uniInt,formatedMessage->uniInt2,mPtr->List_skeletalObject,g_pd3dDevice,TRUE))
			        return FALSE;

		  }//end valid
	   }//end //change def cfg on entity
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 16){//inventory list fill
          CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(formatedMessage->uniInt);
		  if(invObj)
             AddStringToMenuList(formatedMessage->uniShort3,invObj->m_itemData->m_itemName,invObj->m_itemData->m_visualRef,formatedMessage->uniInt,0,invObj->m_itemData->m_itemName,0,"");
		  else{
             AddStringToMenuList(formatedMessage->uniShort3,"Unknown",-1,formatedMessage->uniInt,0,"Unknown",0,"");
		  }
	   }//end inventory list fill
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 17){//store list fill
          CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(formatedMessage->uniInt);
		  if(invObj){
			 CString output = operator+(invObj->m_itemData->m_itemName," cost:");
             output = operator+(output,CABLIB->ConvToString(invObj->m_itemData->m_marketCost));
             AddStringToMenuList(formatedMessage->uniShort3,output,invObj->m_itemData->m_visualRef,formatedMessage->uniInt,0,output,1,"");
		  }
		  else{
             AddStringToMenuList(formatedMessage->uniShort3,"Unknown",-1,formatedMessage->uniInt,0,"Unknown",1,"");
		  }
	   }//end store list fill
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 18){//call store menu
         POSITION entPosition = EntObjRuntime->FindIndex(0);
	     if(entPosition){//valid    
	        CMovementObj * ptrLoc = ( CMovementObj* ) EntObjRuntime->GetAt(entPosition);
		     
		    OpenMenu(formatedMessage->uniInt2,formatedMessage->uniInt,0,0,FALSE);
		 }//end valid
	   }//end call store menu
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 19){//update on cash amount
		  multiplayerObj->m_cashCount = formatedMessage->uniInt;
	   }//end update on cash amount
	   if(formatedMessage->attribute == 20){//open loot menu stencil
		  OpenMenu(formatedMessage->uniInt2,0,0,0,FALSE);
		  ClearAllFromMenuList(formatedMessage->uniInt);
	   }//end open loot menu stencil
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 21){//loot menu list fill
          CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(formatedMessage->uniInt);
		  if(invObj){
             AddStringToMenuList(formatedMessage->uniShort3,invObj->m_itemData->m_itemName,invObj->m_itemData->m_visualRef,formatedMessage->uniInt,0,invObj->m_itemData->m_itemName,0,"");
		  }
		  else{//alternate
			 if(formatedMessage->uniInt == -50){//cash
				CString output = operator+("CREDITS :",CABLIB->ConvToString(formatedMessage->uniInt2));
                AddStringToMenuList(formatedMessage->uniShort3,output,-1,formatedMessage->uniInt,0,output,0,"");
			 }//end cash
		     else//ship!
                AddStringToMenuList(formatedMessage->uniShort3,"Unknown",-1,formatedMessage->uniInt,0,"Unknown",0,"");
		  }//end alternate
	   }//end loot menu list fill
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 22){//update on bank cash amount
		  multiplayerObj->m_bankCashCount = formatedMessage->uniInt;
	   }//end update on bank cash amount
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 23){//update time sync
		  //multiplayerObj->m_serverStartTime = GetTickCount()-formatedMessage->idRefrence;
	   }//end update time sync
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 24){//basic menu open
		  if(formatedMessage->uniShort3 == -9)
		  {//close all if specified
             for(POSITION posLoc = m_menuObjectList->GetHeadPosition();posLoc != NULL;)
			 {//1
                 CMenu3DObj * menu = (CMenu3DObj *)m_menuObjectList->GetNext(posLoc);
	             if(menu->m_closeOnEsc)
				    menu->CloseMenu();
			 }//end 1
		  }//end close all if specified
		  if(formatedMessage->uniInt != 0)
		  {//trade window special instruction
		     CMovementObj * entityPtr =EntObjRuntime->GetObjByNetworkID(formatedMessage->uniInt);
		     if(entityPtr)
		     multiplayerObj->m_currentlyTradingWith = entityPtr->List_name;
		  }//end trade window special instruction
			 OpenMenu(formatedMessage->uniInt2,0,0,0,FALSE);
	   }//end basic menu open
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 25){//close menu that is specified
		  m_menuObjectList->CloseByID(formatedMessage->uniInt);
	   }//end close menu that is specified
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 26){//somone is offering 
           if(formatedMessage->idRefrence > -1)
		   {  
		     for(POSITION posLoc = m_menuObjectList->GetHeadPosition();posLoc != NULL;)
			 {//1
                 CMenu3DObj * menu = (CMenu3DObj *)m_menuObjectList->GetNext(posLoc);
	             if(menu->m_closeOnEsc)
				    menu->CloseMenu();
			 }//end 1
		  
			 multiplayerObj->m_potentialGroupJoiner = formatedMessage->idRefrence;
		     OpenMenu(formatedMessage->uniInt,multiplayerObj->m_potentialGroupJoiner,0,0,FALSE);		     
		  }
	   }//end close menu that is specified
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 27){//set charges
          multiplayerObj->m_itemGlobalMaxCharges   = formatedMessage->uniInt;
          multiplayerObj->m_itemGlobalCurCharges   = formatedMessage->uniInt2;
	   }//end set charges
	   //-----------------------------------------------------------
	   if(formatedMessage->attribute == 28){//set jets
		  gl_svrJt = TRUE;
		  gl_frceY = ((float)formatedMessage->uniInt)*.001f;
		  gl_frceZ = ((float)formatedMessage->uniInt2)*.001f;
	   }//end set jets
	   
	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles Equip specific msgs-------------------------------------------//
BOOL CClientCommObj::HandleEquipMessageClientSide(LPEQUIP_STRUCT formatedMessage,CMultiplayerObj * multiplayerObj)
{
        if(formatedMessage->m_catagorie == 40)
		{
           CMovementObj * targetPtr = (CMovementObj *)EntObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
	       if(targetPtr)
		   {
			 if(formatedMessage->index > 0)
			    GenAndMount(targetPtr,abs(formatedMessage->index));
			 else
				DeMount(targetPtr);
		   }
		   return TRUE;
		}

        if(formatedMessage->idRefrence == 0){//this is confermation
          POSITION posLoc = EntObjRuntime->FindIndex(0);
		  if(posLoc){
	           CMovementObj * selfPtr = ( CMovementObj* ) EntObjRuntime->GetAt(posLoc);
			   if(formatedMessage->index < 0){
                  int vIndex = 0;
				  if(formatedMessage->index != -9999)
                     vIndex = abs(formatedMessage->index);
				     //selfPtr->DisarmItem(vIndex,TRUE);
				     CMovementObj * DBPtr = EntObjList->GetObjByIndex(selfPtr->List_refModel);
					 if(DBPtr)
					 selfPtr->DisarmItemByGLID(vIndex,TRUE,DBPtr,NULL);
			   }
			   else{
	             CMovementObj * DBPtr = EntObjList->GetObjByIndex(selfPtr->List_refModel);
			     if(DBPtr){//valid ref model
                 //selfPtr->ArmItem(abs(formatedMessage->index),DirSnd,DBPtr);
				 selfPtr->ArmItemByGLID(abs(formatedMessage->index),DBPtr,m_globalArmedInvenList,NULL);
				 }//end valid ref model
				 else//failed
				  multiplayerObj->m_eventLog->AddEvent("Arm Item Error:","Index",abs(formatedMessage->index));
			   }
		  }
	   }//end this is confermation
	   else{//somone else
	        CMovementObj * mPtr = EntObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
            if(mPtr){//valid
			  if(formatedMessage->index < 0){
				int vIndex = 0;
				  if(formatedMessage->index != -9999)
                     vIndex = abs(formatedMessage->index);
                     //mPtr->DisarmItem(vIndex,TRUE);
				     CMovementObj * DBPtr = EntObjList->GetObjByIndex(mPtr->List_refModel);
			         if(DBPtr)//valid ref model
					    mPtr->DisarmItemByGLID(vIndex,TRUE,DBPtr,NULL);
			  }
			  else{
				 CMovementObj * DBPtr = EntObjList->GetObjByIndex(mPtr->List_refModel);
			     if(DBPtr){//valid ref model
                 //mPtr->ArmItem(abs(formatedMessage->index),DirSnd,DBPtr);
				 mPtr->ArmItemByGLID(abs(formatedMessage->index),DBPtr,m_globalArmedInvenList,NULL);
				 }//end valid ref model
				 else//failed
				  multiplayerObj->m_eventLog->AddEvent("Arm Item Error:","Index",abs(formatedMessage->index));
			  }
			}//end valid
			else{
               multiplayerObj->m_eventLog->AddEvent("Error locating player with id");
			}
	   }//end somone else
	   return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CClientCommObj::HandleHouseingBlock(BYTE * byteBlock,int msgSz,CMultiplayerObj * multiplayerObj)
{
   
	int structSz = sizeof(HOUSE_DATA);
	int houseStructureCount = msgSz/structSz;


	if(!m_housingDB)
	{
       multiplayerObj->m_eventLog->AddEvent("Invalid House Database");
       houseStructureCount = 0;
	}


	//houseStructureCount = 0;//REMOVE

	if(!multiplayerObj->m_houseingDB)
	{
       multiplayerObj->m_eventLog->AddEvent("Invalid Housing Zones Database");
       houseStructureCount = 0;
	}

	if(houseStructureCount == 0)
	{
	   delete [] byteBlock;
	   byteBlock = 0;
	   return FALSE;
	}

	

	HOUSE_DATA * houseBlockData = new HOUSE_DATA[houseStructureCount];
	BYTE * conversionBlock = (BYTE*)houseBlockData;
    
    for(int i=0;i<(msgSz-1);i++)
        conversionBlock[i] = byteBlock[i+1];

    for(int loop=0;loop<houseStructureCount;loop++)
	{
        CHousingObj * houseBasePtr = m_housingDB->GetByIndex(houseBlockData[loop].m_houseType);
	    if(houseBasePtr)
		{//valid database base
		   int houseID = 0;
           BNDBOX clipBoxCast;
		   D3DVECTOR wldPosition;
		   wldPosition.x = houseBlockData[loop].m_posX;
		   wldPosition.y = houseBlockData[loop].m_posY;
		   wldPosition.z = houseBlockData[loop].m_posZ;
           clipBoxCast.max.x = houseBasePtr->m_clipBox.maxX;
		   clipBoxCast.max.y = houseBasePtr->m_clipBox.maxY;
		   clipBoxCast.max.z = houseBasePtr->m_clipBox.maxZ;
		   clipBoxCast.min.x = houseBasePtr->m_clipBox.minX;
	       clipBoxCast.min.y = houseBasePtr->m_clipBox.minY;
		   clipBoxCast.min.z = houseBasePtr->m_clipBox.minZ;

		   int result = multiplayerObj->m_houseingDB->AddPlacementObjToTable(houseBlockData[loop].m_houseType,
                                                                                  houseBlockData[loop].m_acID,
                                                                                  clipBoxCast,
										                                          wldPosition,
											                                      &houseID,
																				  -1);
           if(result == 0)
		   {
              multiplayerObj->m_eventLog->AddEvent("House placement failed");
		   }
		   else if(result == 1)
		   {
              multiplayerObj->m_eventLog->AddEvent("House placement successful");
		   }
		   else if(result == 5)
		   {
			  multiplayerObj->m_eventLog->AddEvent("House placement failed ver 2");
		   }
		}//end valid database base
	}

    delete [] byteBlock;
    byteBlock = 0;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles spawn messages------------------------------------------------//
BOOL CClientCommObj::HandleSpawnMessagesClientSide(LPSPAWN_STRUCT formatedMessage,DPNID idFromLoc,CMultiplayerObj * multiplayerObj)
{
	if(EntObjRuntime){
	  int trace = 0;
      for(POSITION posLoc = EntObjRuntime->GetHeadPosition();posLoc != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) EntObjRuntime->GetNext(posLoc);
		DestroyRuntimeEntity(mPtr,trace);
		mPtr = 0;
		trace++;
	  }
	  EntObjRuntime->RemoveAll();
	}	
	
	//this is only for client use / not AI servers
	if(formatedMessage->worldName != CurrentWorkingFile)
	{//necessary world change
       while(!LoadScene(formatedMessage->worldName))
	   {//failed loading world
          CString build = formatedMessage->worldName;
		  build = operator+("Error opening file :",build);
		  GL_systemReadout->AddLine(build,g_pd3dDevice,m_symbolMapDB,m_speechMode);
	   }//end failed loading world
	   CurrentWorkingFile = formatedMessage->worldName;
	}//end necessary world change
    //flush unecessary
	if(multiplayerObj->m_clientCfgRequestOutbox)
       multiplayerObj->m_clientCfgRequestOutbox->SafeDelete();
	if(multiplayerObj->m_attributeOutBox)
       multiplayerObj->m_attributeOutBox->SafeDelete();
	//end flush unecessary

    D3DMATRIX locationalMatrix;
    GL_matrixFunctions.InitMat(locationalMatrix.m);
	locationalMatrix._41 = formatedMessage->uniInt1;
	locationalMatrix._42 = formatedMessage->uniInt2;
	locationalMatrix._43 = formatedMessage->uniInt3;

    BOOL result = ActivateMovementEntity(NULL,
                                         formatedMessage->currentTrace,
                                         locationalMatrix,
                                         0,
								         0,
								         formatedMessage->dbIndex,//db
								         0,//user
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
										 -1);
    if(!result)
		MessageBox("Base spawn failed");

	multiplayerObj->m_requestingPortal = FALSE;
	


	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CClientCommObj::HandleItemLocationalMessagesClientSide(LPITEM_LOCATION formatedMessage,BYTE * dataBlock)
{
	   //if(!EntObjRuntime->GetItemByNetworkID(formatedMessage->networkID))
	   if(!EntObjRuntime->GetObjByNetworkID(formatedMessage->networkID))//if already spawned forget it
	   if(formatedMessage->dbIndex > -1){//spawn the item
		  CString name;
		  int tracer = sizeof(ITEM_LOCATION);
		  if(formatedMessage->m_nameLength > 0)
		  {//unpack name
		     char * nameTemp = new char[formatedMessage->m_nameLength+1];
			 
			 for(int j=0;j<formatedMessage->m_nameLength;j++)
			 {
			     nameTemp[j] = dataBlock[tracer];
				 tracer++;
			 }
			 nameTemp[formatedMessage->m_nameLength] = 0;
			 name = nameTemp;
			 delete [] nameTemp;
			 nameTemp = 0;
		  }//end unpack name


		  int * spawnCfg = NULL;
		  if(formatedMessage->m_cfgLength > 0)
		  {
			 spawnCfg = new int[formatedMessage->m_cfgLength+10];
		     ZeroMemory(spawnCfg,sizeof(int)*(formatedMessage->m_cfgLength+10));
		  }
		  //spawn cfg
		  for(int cfg=0;cfg<formatedMessage->m_cfgLength;cfg++)
		  {
		      spawnCfg[cfg] = dataBlock[tracer];
			  tracer++;
		  }
		  short * glidArray = NULL;
		  if(formatedMessage->m_itemArmedLength > 0){//equip glid package
             glidArray = new short[formatedMessage->m_itemArmedLength];
			 BYTE * glidConvertion = (BYTE*)glidArray;
			 for(int glLoop=0;glLoop<(formatedMessage->m_itemArmedLength*sizeof(short));glLoop++)
			 {
                 glidConvertion[glLoop] = dataBlock[tracer];
			     tracer++;
			 }
		  }//end equip glid package

		  CMovementObj * mObj = NULL;
          if(ActivateMovementEntity(NULL,
			                      0,
	                              MatrixInit2,
	                              -1,
	                              -1,
	                               formatedMessage->dbIndex,
	                              35,
	                               0,
	                               formatedMessage->networkID,
	                               TRUE,
	                               FALSE,
	                               FALSE,
	                               formatedMessage->networkID,
	                               -1,
								   &mObj,
					               spawnCfg,
								   -1,
								   -1)){//valid item
              //GL_systemReadout->AddLine(mObj->List_name,g_pd3dDevice,m_symbolMapDB,0.1f,1.0f,0.1f);//REMOVE
			  if(glidArray){//equip with list
			   CMovementObj * DBPtr = EntObjList->GetObjByIndex(mObj->List_refModel);
			     if(DBPtr)
                 for(int j=0;j<formatedMessage->m_itemArmedLength;j++)
				 {//glid loop
				    mObj->ArmItemByGLID(glidArray[j],DBPtr,m_globalArmedInvenList,NULL);
				 }//end glid loop
			  }//end equip with list

		  D3DMATRIX positionalMatrix;
		  GL_matrixFunctions.Rotate (0.0f,((float)formatedMessage->m_rotation)*2.0f,0.0f,positionalMatrix.m);
		  positionalMatrix._41 = formatedMessage->Xpos;
		  positionalMatrix._42 = formatedMessage->Ypos;
		  positionalMatrix._43 = formatedMessage->Zpos;
		  mObj->List_baseFrame->AddTransformReplace(positionalMatrix);	

		  mObj->m_validThisRound = TRUE;
		  mObj->List_networkPopoutCount = 0;
		  mObj->List_name = name;
		  if(formatedMessage->curAnim < 0)//reverse animation
			   mObj->List_skeletalObject->SetAnimationDirection(abs(formatedMessage->curAnim),TRUE);
			else
			   mObj->List_skeletalObject->SetAnimationDirection(abs(formatedMessage->curAnim),FALSE);
			   mObj->List_skeletalObject->SetCurrentAnimation(abs(formatedMessage->curAnim),100);
                
			   int version=0;int attrib=0;
			   if(mObj->List_armedInventoryDB){
				   mObj->List_skeletalObject->GetAnimationStatus(&version,&attrib,abs(formatedMessage->curAnim));
				   for(POSITION posArm = mObj->List_armedInventoryDB->GetHeadPosition();posArm!= NULL;){
					   CArmedInventoryObj * armPtr = (CArmedInventoryObj *)mObj->List_armedInventoryDB->GetNext(posArm);
                       if(armPtr->m_thirdPersonSystem){
						   int indexer = armPtr->m_thirdPersonSystem->GetAnimationIndexFromAttrib(attrib);
			               armPtr->m_thirdPersonSystem->SetCurrentAnimation(indexer,100);
					   }
				   }
			   }
		  }//end valid item
		  if(glidArray)
			 delete [] glidArray;
		     glidArray = 0;
		  if(spawnCfg)
	      {
             delete [] spawnCfg;
		     spawnCfg = 0;
	      }
		  if(mObj)
          if(formatedMessage->mount > -1)
             GenAndMount(mObj,formatedMessage->mount);
	   }//end spawn the item
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//----------------HandleMenuBuildMessagesClientSide-----------------------------------//
BOOL CClientCommObj::HandleMenuBuildMessagesClientSide(LPMENUBUILD_STRUCT formatedMessage,
												       DPNID idFromLoc,
												       CMenu3DObjList * menuList)
{
	if(formatedMessage->messageType == 0)
	{//update menu control by ID
       menuList->AddStringToMenuList(formatedMessage->miscint,formatedMessage->uniText,-1,0,0,formatedMessage->uniText,0,"");
	}//end update menu control by ID
    return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
BOOL CClientCommObj::HandleClientMovBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj)
{
    BYTE * dataBlock = (BYTE *)lpMessage;
    LPENTITYPOS_HEADER header = (LPENTITYPOS_HEADER)lpMessage;  
	int headerSize = sizeof(ENTITYPOS_HEADER);
    int movStructSize = sizeof(SHRUNK_MOVE_STRUCT);
	   if(multiplayerObj->m_requestingPortal)
	   {//failed
		  delete [] dataBlock;
          dataBlock=0;
		  return TRUE;
	   }//end failed*/
       
	   int indexInto = headerSize;

       SHRUNK_MOVE_STRUCT * movBlockData               = NULL;
	   STATICENTITY_DATA * staticData                  = NULL;
	   FIRE_STRUCT * fireData                          = NULL;
	   MSGHANDLER *  msgData                           = NULL;
	   DAMAGEINFO_DATA * dmgData                       = NULL;
	   DETAILEDMSG_DATA * detMsgData                   = NULL;
	   GENERICCONFIG_STRUCT * surroundingHousesMsgData = NULL;
	   
	   if(header->m_entPosCount > 0)
	   {
	      movBlockData = new SHRUNK_MOVE_STRUCT[header->m_entPosCount];
	      int byteCount = sizeof(SHRUNK_MOVE_STRUCT)*header->m_entPosCount;
          BYTE * conversionBlock = (BYTE*)movBlockData;//new BYTE[byteCount];
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

	   if(header->m_staticCount > 0)
	   {
	      staticData = new STATICENTITY_DATA[header->m_staticCount];
	      int byteCount = (header->m_staticCount*sizeof(STATICENTITY_DATA));
          BYTE * conversionBlock = (BYTE*)staticData;//new BYTE[byteCount];
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

       if(header->m_fireMsgCount > 0)
	   {
	      fireData = new FIRE_STRUCT[header->m_fireMsgCount];
	      int byteCount = (header->m_fireMsgCount*sizeof(FIRE_STRUCT));
          BYTE * conversionBlock = (BYTE *)fireData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

	   if(header->m_generalMsgCount > 0)
	   {
	      msgData = new MSGHANDLER[header->m_generalMsgCount];
	      int byteCount = (header->m_generalMsgCount*sizeof(MSGHANDLER));
          BYTE * conversionBlock = (BYTE *)msgData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

	   if(header->m_damageInfoMsgCount > 0)
	   {
	      dmgData = new DAMAGEINFO_DATA[header->m_damageInfoMsgCount];
	      int byteCount = (header->m_damageInfoMsgCount*sizeof(DAMAGEINFO_DATA));
          BYTE * conversionBlock = (BYTE *)dmgData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

	   if(header->m_detailedMsgCount > 0)
	   {
	      detMsgData = new DETAILEDMSG_DATA[header->m_detailedMsgCount];
	      int byteCount = (header->m_detailedMsgCount*sizeof(DETAILEDMSG_DATA));
          BYTE * conversionBlock = (BYTE *)detMsgData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

       if(header->m_itemCfgMsgCount > 0)
	   {//item cfgs
		  
		  for(int itemLoop=0;itemLoop<header->m_itemCfgMsgCount;itemLoop++)
		  {
              int dataBlockSz = dataBlock[indexInto];
              indexInto++;
 
			  if(dataBlockSz > 0)
			  {
			     BYTE * byteBlock = new BYTE[dataBlockSz];
                 for(int i=0;i<dataBlockSz;i++)
				 {
                     byteBlock[i] = dataBlock[indexInto];
                     indexInto++;
				 }
				 LPITEM_LOCATION itemLocationMsg = (LPITEM_LOCATION)byteBlock;
				 HandleItemLocationalMessagesClientSide(itemLocationMsg,byteBlock);
			     delete [] byteBlock;
				 byteBlock = 0;
			  }
		  }
	   }//end item cfgs

       if(header->m_entitCfgMsgCount > 0)
	   {//entity cfgs
		  for(int itemLoop=0;itemLoop<header->m_entitCfgMsgCount;itemLoop++)
		  {
              int dataBlockSz = dataBlock[indexInto];
              indexInto++;
 
			  if(dataBlockSz > 0)
			  {
			     BYTE * byteBlock = new BYTE[dataBlockSz];
                 for(int i=0;i<dataBlockSz;i++)
				 {
                     byteBlock[i] = dataBlock[indexInto];
                     indexInto++;
				 }
                 HandleEntityCfgBlock(byteBlock,dataBlockSz,idFromLoc);
				 delete [] byteBlock;
				 byteBlock = 0;
			  }
		  }
	   }//end entity cfgs

       int currentCfgRequestCnt = 0;

	   if(header->m_houseMsgCount > 0)
	   {
		  surroundingHousesMsgData = new GENERICCONFIG_STRUCT[header->m_houseMsgCount];
		  int byteCount = (header->m_houseMsgCount*sizeof(GENERICCONFIG_STRUCT));
          BYTE * conversionBlock = (BYTE *)surroundingHousesMsgData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
			  conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];

		  int requestCfgCount = 0;
          for(int i=0;i<header->m_houseMsgCount;i++)
		  {
              if(!m_houseingCache->FlagValidByAcID(surroundingHousesMsgData[i].m_acID,surroundingHousesMsgData[i].m_open))
			  {
                 //GL_systemReadout->AddLine("Requesting house config",g_pd3dDevice,m_symbolMapDB);
				 if(requestCfgCount<10)
				    SendAttribMsgCP(38,
				                    surroundingHousesMsgData[i].m_acID,
				 	                0,
						            0,
						            0);

				 requestCfgCount++;
			  }
		  }

          delete [] surroundingHousesMsgData;
		  surroundingHousesMsgData = 0;
		  indexInto += byteCount; 
	   }
       
       

	   if(movBlockData)
	   {//parse dynamic entities
	      for(int msgParse = 0;msgParse < header->m_entPosCount;msgParse++)
		  {//parse messages
		     LPMOVE_STRUCT expandedStruct = (LPMOVE_STRUCT) new BYTE[sizeof(MOVE_STRUCT)];
	         expandedStruct = multiplayerObj->ExpandData(&movBlockData[msgParse],expandedStruct);
		     UpdateNetworkEntity(expandedStruct,FALSE,currentCfgRequestCnt);
		     delete [] expandedStruct;
		     expandedStruct=0;
		  }//end parse messages

          if(movBlockData)
		  { 
             delete [] movBlockData;
             movBlockData = 0;
		  }
	   }//end parse dynamic entities
	   
       if(staticData)
	   {//parse static entities  
	      for(int msgParse = 0;msgParse < header->m_staticCount;msgParse++)
		  {//parse messages
             CMovementObj * mPtr = NULL;
			 if(staticData[msgParse].m_networAssignedID < 0){
			    mPtr = EntObjRuntime->GetItemByNetworkID(abs(staticData[msgParse].m_networAssignedID));
			 }
		     else{
				mPtr = EntObjRuntime->GetObjByNetworkID(staticData[msgParse].m_networAssignedID);
			 }
					
		     if(mPtr)
			 {//already here refresh
                mPtr->m_validThisRound = TRUE;
				if(mPtr->m_posses)
                   mPtr->m_posses->m_validThisRound = TRUE;
			 }//end already here refresh
		     else if(currentCfgRequestCnt < multiplayerObj->m_maxCfgRequestsPerRound){//inquire about this seed
		       currentCfgRequestCnt++; 

               if(staticData[msgParse].m_networAssignedID < 0)
			   {//send item data
                SendAttribMsgCP(4,
				                abs(staticData[msgParse].m_networAssignedID),
				 	            0,
						        0,
						        0);
			   }//end send item data
			   else{//send static entity data
				SendAttribMsgCP(1,
				                0,
				 	            staticData[msgParse].m_networAssignedID,
						        staticData[msgParse].m_networAssignedID,
						        FALSE);
			   }//end send static entity data
			 }//end inquire about this seed
		  }//end parse messages

          if(staticData)
			 delete [] staticData;
		     staticData = 0;
	   }//end parse static entities

	   if(fireData)
	   {//parse fire msgs
		  for(int floop=0;floop<header->m_fireMsgCount;floop++)
              HandleFireMessagesClientSide(&fireData[floop]);

          if(fireData)
			 delete [] fireData;
		     fireData = 0;
	   }//end parse fire msgs

	   if(dmgData)
	   {//parse dmg msgs
          for(int floop=0;floop<header->m_damageInfoMsgCount;floop++)
              HandleDmgMsgHandler(&dmgData[floop]);

          if(dmgData)
			 delete [] dmgData;
		     dmgData = 0;
	   }//end parse dmg msgs

	   if(msgData)
	   {//parse dmg msgs
          for(int floop=0;floop<header->m_generalMsgCount;floop++)
              HandleGeneralMsgHandler(&msgData[floop]);

          if(msgData)
			 delete [] msgData;
		     msgData = 0;
	   }//end parse dmg msgs

	   if(detMsgData)
	   {//parse det msgs
          for(int floop=0;floop<header->m_detailedMsgCount;floop++)
              HandleDetailedMsgHandler(&detMsgData[floop]);

          if(detMsgData)
			 delete [] detMsgData;
		     detMsgData = 0;
	   }//end parse det msgs

       SendPendingCfgRequestsCP();//send all pending cfg requests
	
 
    delete [] dataBlock;
    dataBlock=0;
	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
BOOL CClientCommObj::HandleClientListGlidBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj)
{
LPBLOCKITEMLIST_STRUCT header = (LPBLOCKITEMLIST_STRUCT)lpMessage;  
BYTE * dataBlock = (BYTE *)lpMessage;

	   if((msgSize-4) < 4)
	   {//failed
          multiplayerObj->m_eventLog->AddEvent("Block Update Failed");
		  delete [] dataBlock;
          dataBlock=0;
		  return TRUE;
	   }//end failed
       //get msg count
       int msgCount = (msgSize-4)/4;
       ITEMDATA_STRUCT * itemBlockData = new ITEMDATA_STRUCT[msgCount];

	   int byteCount = (msgSize-4);
       BYTE * conversionBlock = new BYTE[byteCount];
       for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	       conversionBlock[shiftLoop] = dataBlock[shiftLoop+4];

       CopyMemory(itemBlockData,conversionBlock,byteCount);

       if(conversionBlock)
		  delete [] conversionBlock;
	      conversionBlock = 0;

       for(int msgParse = 0;msgParse < msgCount;msgParse++)
	   {//parse messages
          CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(itemBlockData[msgParse].m_GLID);
		  if(invObj){
             //AddStringToMenuList(header->m_miscInt,invObj->m_itemData->m_itemName,-1,itemBlockData[msgParse].m_GLID);
             CString output = invObj->m_itemData->m_itemName;
             if(invObj->m_itemData->m_stackable)
			    output = operator+(operator+(invObj->m_itemData->m_itemName," X"),CABLIB->ConvToString(itemBlockData[msgParse].m_quanity));
             AddStringToMenuList(header->m_miscInt,output,invObj->m_itemData->m_visualRef,itemBlockData[msgParse].m_GLID,itemBlockData[msgParse].m_quanity,invObj->m_itemData->m_itemName,1,"");
		  }
		  else{
             AddStringToMenuList(header->m_miscInt,"Unknown",-1,itemBlockData[msgParse].m_GLID,0,"Unknown",1,"");
		  }
	   }//end parse messages
	   if(itemBlockData)
		  delete [] itemBlockData;
	      itemBlockData = 0;
   
	   delete [] dataBlock;
       dataBlock=0;
	   return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
BOOL CClientCommObj::HandleEntityCfgBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj)
{
   LPENTITYCFG_HEADER header = (LPENTITYCFG_HEADER)lpMessage;  
   BYTE * dataBlock = (BYTE *)lpMessage;

   if(header->m_catagorie == 83)
   {//Housing specific
	  D3DVECTOR wldHsPosition;
      wldHsPosition.x = header->Xpos;
	  wldHsPosition.y = header->Ypos;
	  wldHsPosition.z = header->Zpos;
	  m_houseingCache->PlaceHouse(wldHsPosition,header->dbCfg,header->networkDefinedID);
      return TRUE;
   }//end Housing specific
   int headerSz = sizeof(ENTITYCFG_HEADER);
   int invenItemCount = header->invenDataSize/sizeof(SHORTTYPE_DATA);

   int * spawnCfg = NULL;
   int defCfgItemCount = header->defCfgDataSize/sizeof(SHORTTYPE_DATA);
   if(defCfgItemCount > 0)
   {//valid deformable
     SHORTTYPE_DATA * defCfgBuildBlock = new SHORTTYPE_DATA[defCfgItemCount];
     BYTE * defCfgByteAccess = (BYTE*)defCfgBuildBlock;
     int offset = headerSz + abs(header->nameDataSize) + header->invenDataSize;
     for(int nloop=0;nloop<header->defCfgDataSize;nloop++)//def cfg loop
         defCfgByteAccess[nloop] = dataBlock[nloop+offset];

     if(defCfgItemCount>0)
	 {
	    spawnCfg = new int[defCfgItemCount+10];
        ZeroMemory(spawnCfg,sizeof(int)*(defCfgItemCount+10));
	 }

	 for(int apDefLoop=0;apDefLoop<defCfgItemCount;apDefLoop++)
	     spawnCfg[apDefLoop] = defCfgBuildBlock[apDefLoop].m_data;
         //for(int apDefLoop=0;apDefLoop<defCfgItemCount;apDefLoop++)//do def cfg
             //DBPtr->List_skeletalObject->HowSwapDim(apDefLoop,defCfgBuildBlock[apDefLoop].m_data,mPtr->List_skeletalObject,g_pd3dDevice);
        
	  if(defCfgBuildBlock)
	  delete [] defCfgBuildBlock;
      defCfgBuildBlock = 0;
   }//end valid deformable
   
   
   if(!EntObjRuntime->GetObjByNetworkID(header->networkDefinedID))
   {//not found spawn it
	  CMovementObj * temp = NULL;
	  int controlType = -1;
	  if(header->m_isItem)
		 controlType = 35;
      if(ActivateMovementEntity(NULL,
			                    0,
	                            MatrixInit2,
	                            -1,
	                            -1,
	                            header->dbCfg,
	                            controlType,
	                            0,
	                            header->networkDefinedID,
	                            TRUE,
	                            FALSE,
	                            FALSE,
	                            -1,
	                            header->networkDefinedID,
							    &temp,
					            spawnCfg,
								-1,
								-1)){
	       if(spawnCfg)
           {
              delete [] spawnCfg;
              spawnCfg = 0;
           }
		   //initialize
		   temp->m_clanMember = header->m_clanMember;
		   temp->List_baseFrame->SetPosition2(NULL,header->Xpos,header->Ypos,header->Zpos);
	       temp->List_networkPopoutCount               = 0;//updated dont pop out
		   temp->List_networkInterpCurStep             = 0;  
	       temp->List_networkInterpCurStepOri          = 0;
	       StampNetworkMark(temp);
		   temp->m_validThisRound                      = TRUE;
           temp->List_baseFrame->GetPosition(NULL,&temp->List_networkLastPositionXYZ);
	       temp->List_baseFrame->GetOrientation(&temp->List_networkLastDirVect,&temp->List_networkLastUpVect); 
	       temp->List_networkFreeLookCurYawLast        = 0.0f;
	       temp->List_networkFreeLookCurPitchLast      = 0.0f;

			temp->List_networkTargetPositionXYZ.x     = header->Xpos;
	        temp->List_networkTargetPositionXYZ.y     = header->Ypos;
	        temp->List_networkTargetPositionXYZ.z     = header->Zpos;
			temp->List_networkFreeLookCurYawTarget    = 0.0f;
	        temp->List_networkFreeLookCurPitchTarget  = 0.0f;

			if(temp->List_skeletalObject->SetCurrentAnimation(abs(header->anim),100))
			{		
			  if(header->anim < 0)//reverse animation
			     temp->List_skeletalObject->SetAnimationDirection(abs(header->anim),TRUE);
		  	  else
			     temp->List_skeletalObject->SetAnimationDirection(abs(header->anim),FALSE);
			}  
		   //end initialize
		   
	  }
   }//end not found spawn it

   
   
   //retrieve entity
   CMovementObj * mPtr = EntObjRuntime->GetObjByNetworkID(header->networkDefinedID);//verify
   if(!mPtr)
   {
	   if(spawnCfg)
       {
          delete [] spawnCfg;
          spawnCfg = 0;
       }
       return FALSE;
   }

   if(mPtr->List_networkEntity == FALSE && mPtr->List_controlType == 0)
   multiplayerObj->m_requestingPortal = FALSE;
          
   CMovementObj * DBPtr = EntObjList->GetObjByIndex(mPtr->List_refModel);
   if(!DBPtr)
   {
	   if(spawnCfg)
       {
          delete [] spawnCfg;
          spawnCfg = 0;
       }
       return FALSE;
   }
   
   //

   if(header->nameDataSize < 0)
   {
      header->nameDataSize = abs(header->nameDataSize);
      mPtr->m_murderer = TRUE;
   }

   if(mPtr->m_textureRenderObject)
   {
      mPtr->m_textureRenderObject->SafeDelete();
      delete mPtr->m_textureRenderObject;
	  mPtr->m_textureRenderObject = 0;
   }
   //Set name
   if(header->nameDataSize > 0)
   {//valid name data
   char * nameString = new char[(header->nameDataSize + 1)];
   for(int nloop=0;nloop<header->nameDataSize;nloop++)
       nameString[nloop] = dataBlock[nloop+headerSz];
   nameString[header->nameDataSize] = 0;
   mPtr->List_name = nameString;//set to ptr
   
   if(nameString)
	  delete [] nameString;
      nameString = 0;
   }//end valid name data
   //set inventory Cfg

   if(spawnCfg)
   if(defCfgItemCount > 0)
   {//valid deformable
         for(int apDefLoop=0;apDefLoop<defCfgItemCount;apDefLoop++)//do def cfg
             DBPtr->List_skeletalObject->HowSwapDim(apDefLoop,spawnCfg[apDefLoop],mPtr->List_skeletalObject,g_pd3dDevice,TRUE);
   }//end valid deformable
   
   if(invenItemCount > 0)
   {//valid inven
     SHORTTYPE_DATA * invenBuildBlock = new SHORTTYPE_DATA[invenItemCount];
     BYTE * invenByteAccess = (BYTE*)invenBuildBlock;
     int offset = headerSz + header->nameDataSize;
     for(int nloop=0;nloop<header->invenDataSize;nloop++)
	 {//inventory build loop
       invenByteAccess[nloop] = dataBlock[nloop+offset];
	 }//end inventory build loop
	 int curPhyObj = 0;
     for(int invenLoop=0;invenLoop<invenItemCount;invenLoop++)
	 {//inven action loop
		 if(invenBuildBlock[invenLoop].m_data<0)
		 {//server side physics assigned
	        if(mPtr->List_movementDB)
			{
               if(curPhyObj == 0)
			   {
                  mPtr->List_movementDB->List_maxSpeed = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
				  mPtr->List_movementDB->List_minSpeed = -mPtr->List_movementDB->List_maxSpeed;
				  mPtr->List_movementDB->List_accellerate = mPtr->List_movementDB->List_maxSpeed;
				  mPtr->List_movementDB->List_decellerate = mPtr->List_movementDB->List_maxSpeed;
			   }
			   if(curPhyObj == 1)
                  mPtr->List_movementDB->List_jumpPower = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
			   if(curPhyObj == 2)
                  mPtr->List_movementDB->m_inLiquidMotionModifier = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
			   if(curPhyObj == 3)
                  mPtr->List_movementDB->m_crossSectionalArea = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
               if(curPhyObj == 4)
                  mPtr->List_movementDB->m_airControl = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
			   curPhyObj++;
			}
		 }//end server side physics assigned
		 else
		   mPtr->ArmItemByGLID(abs(invenBuildBlock[invenLoop].m_data),DBPtr,m_globalArmedInvenList,NULL);
	 }//end inven action loop
     if(invenBuildBlock)
	    delete [] invenBuildBlock;
        invenBuildBlock = 0;
   //set def cfg
   }//end valid inven
   
   /*int defCfgItemCount = header->defCfgDataSize/sizeof(SHORTTYPE_DATA);
   if(defCfgItemCount > 0)
   {//valid deformable
     SHORTTYPE_DATA * defCfgBuildBlock = new SHORTTYPE_DATA[header->defCfgDataSize];
     BYTE * defCfgByteAccess = (BYTE*)defCfgBuildBlock;
     int offset = headerSz + header->nameDataSize + header->invenDataSize;
     for(int nloop=0;nloop<header->defCfgDataSize;nloop++)//def cfg loop
         defCfgByteAccess[nloop] = dataBlock[nloop+offset];

         for(int apDefLoop=0;apDefLoop<defCfgItemCount;apDefLoop++)//do def cfg
             DBPtr->List_skeletalObject->HowSwapDim(apDefLoop,defCfgBuildBlock[apDefLoop].m_data,mPtr->List_skeletalObject,g_pd3dDevice);
        
	  if(defCfgBuildBlock)
	  delete [] defCfgBuildBlock;
      defCfgBuildBlock = 0;
   }//end valid deformable*/
   

   if(spawnCfg)
   {
      delete [] spawnCfg;
      spawnCfg = 0;
   }
   //handle mount function
   if(mPtr)
   if(header->mount > -1)
      GenAndMount(mPtr,header->mount);

   if(mPtr)
   if(multiplayerObj->m_AIServer)
      IndexEntityAnimations(mPtr,GetTickCount(),TRUE);

   return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CClientCommObj::HandleBankDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj)
{
    BYTE * dataBlock = (BYTE *)lpMessage;
    LPBANK_BLOCKHEADER header = (LPBANK_BLOCKHEADER)lpMessage;
        
   
        OpenMenu(header->m_menuBind,header->m_menuBind,0,0,FALSE);//open bank specified menu
        multiplayerObj->m_bankCashCount = header->m_bankCash;//update bank data
		multiplayerObj->m_cashCount     = header->m_onCharCash;

		int headerSize = sizeof(BANK_BLOCKHEADER);
		int itemDataBlockSize = (msgSize-headerSize);
		if(itemDataBlockSize > 0)
		{//valid
		int itemDataSize = sizeof(ITEMDATA_STRUCT);
		int invenCount = (itemDataBlockSize/itemDataSize);
        ITEMDATA_STRUCT * itemDataBlock = new ITEMDATA_STRUCT[invenCount];
		BYTE * conversionBlock = (BYTE*)itemDataBlock;
        for(int loop=0;loop<itemDataBlockSize;loop++)
            conversionBlock[loop] = dataBlock[loop+headerSize];

        //--------------------
		int tracer = 0;
		for(loop=0;loop<header->m_bankItemCount;loop++)
		{//fill in bank data
         CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(itemDataBlock[loop].m_GLID);
		  if(invObj){
             //AddStringToMenuList(header->m_menuBind,invObj->m_itemData->m_itemName,-1,itemDataBlock[loop].m_GLID);
			 CString output = invObj->m_itemData->m_itemName;
             if(invObj->m_itemData->m_stackable)
			    output = operator+(operator+(invObj->m_itemData->m_itemName," X"),CABLIB->ConvToString(itemDataBlock[loop].m_quanity));
             AddStringToMenuList(header->m_menuBind,output,invObj->m_itemData->m_visualRef,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,invObj->m_itemData->m_itemName,1,"");
		  }
		  else{//alternate
             AddStringToMenuList(header->m_menuBind,"Unknown",-1,itemDataBlock[loop].m_GLID,0,"Unknown",1,"");
		  }//end alternate
		  tracer++;
		}//end fill in bank data
		//----------------------
		
		
		for(loop=tracer;loop<invenCount;loop++)
		{//fill in player inventory
          CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(itemDataBlock[loop].m_GLID);
		  if(invObj){
             //AddStringToMenuList(51,invObj->m_itemData->m_itemName,-1,itemDataBlock[loop].m_GLID);
			 CString output = invObj->m_itemData->m_itemName;
             if(invObj->m_itemData->m_stackable)
			    output = operator+(operator+(invObj->m_itemData->m_itemName," X"),CABLIB->ConvToString(itemDataBlock[loop].m_quanity));
             AddStringToMenuList(51,output,invObj->m_itemData->m_visualRef,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,invObj->m_itemData->m_itemName,1,"");
		  }
		  else{//alternate
             AddStringToMenuList(51,"Unknown",-1,itemDataBlock[loop].m_GLID,0,"Unknown",1,"");
		  }//end alternate
		}//end fill in player inventory

        delete [] itemDataBlock;
		itemDataBlock = 0;
		}//end valid


	delete [] dataBlock;
    dataBlock=0;
	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CClientCommObj::HandleStoreDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj)
{
    BYTE * dataBlock = (BYTE *)lpMessage;
    LPSTORE_BLOCKHEADER header = (LPSTORE_BLOCKHEADER)lpMessage;

        OpenMenu(header->m_menuBind,0,0,0,FALSE);//open store specified menu
        multiplayerObj->m_cashCount = header->m_playerCash;//update bank data

		int headerSize = sizeof(STORE_BLOCKHEADER);
		int itemDataBlockSize = (msgSize-headerSize);
		if(itemDataBlockSize > 0)
		{//valid
		int itemDataSize = sizeof(ITEMDATA_STRUCT);
		int invenCount = (itemDataBlockSize/itemDataSize);
        ITEMDATA_STRUCT * itemDataBlock = new ITEMDATA_STRUCT[invenCount];
		BYTE * conversionBlock = (BYTE*)itemDataBlock;
        for(int loop=0;loop<itemDataBlockSize;loop++)
            conversionBlock[loop] = dataBlock[loop+headerSize];

        //--------------------
		int tracer = 0;
		for(loop=0;loop<header->m_storeItemCount;loop++)
		{//fill in bank data
         CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(itemDataBlock[loop].m_GLID);
		  if(invObj){
              CString output = operator+(invObj->m_itemData->m_itemName," cost:");
              output = operator+(output,CABLIB->ConvToString(invObj->m_itemData->m_marketCost));
			  AddStringToMenuList(52,output,invObj->m_itemData->m_visualRef,itemDataBlock[loop].m_GLID,0,invObj->m_itemData->m_itemName,1,(" cost:" + CABLIB->ConvToString(invObj->m_itemData->m_marketCost)));
		  }
		  else{//alternate
             AddStringToMenuList(52,"Unknown",-1,itemDataBlock[loop].m_GLID,0,"Unknown",1,"");
		  }//end alternate
		  tracer++;
		}//end fill in bank data
		//----------------------
		
		
		for(loop=tracer;loop<invenCount;loop++)
		{//fill in player inventory
          CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(itemDataBlock[loop].m_GLID);
		   if(invObj){
             //AddStringToMenuList(51,invObj->m_itemData->m_itemName,-1,itemDataBlock[loop].m_GLID);
             CString output = invObj->m_itemData->m_itemName;
             if(invObj->m_itemData->m_stackable)
			    output = operator+(operator+(invObj->m_itemData->m_itemName," X"),CABLIB->ConvToString(itemDataBlock[loop].m_quanity));
             AddStringToMenuList(51,output,invObj->m_itemData->m_visualRef,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,invObj->m_itemData->m_itemName,1,"");
		   }
		   else{//alternate
             AddStringToMenuList(51,"Unknown",-1,itemDataBlock[loop].m_GLID,1,"Unknown",1,"");
		   }//end alternate
		}//end fill in player inventory

        delete [] itemDataBlock;
		itemDataBlock = 0;
		}//end valid


	delete [] dataBlock;
    dataBlock=0;
	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CClientCommObj::HandleHouseStoreDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj)
{
    BYTE * dataBlock = (BYTE *)lpMessage;
    LPSTORE_BLOCKHEADER header = (LPSTORE_BLOCKHEADER)lpMessage;

        OpenMenu(header->m_menuBind,0,0,0,FALSE);//open store specified menu
		SetMenuTextItem(188, CABLIB->ConvToString(header->m_storeCash));
        multiplayerObj->m_cashCount = header->m_playerCash;//update bank data
		int headerSize = sizeof(STORE_BLOCKHEADER);
		int itemDataBlockSize = (msgSize-headerSize);
		if(itemDataBlockSize > 0)
		{//valid
		int itemDataSize = sizeof(ITEMDATA_STRUCTEXT);
		int invenCount = (itemDataBlockSize/itemDataSize);
        ITEMDATA_STRUCTEXT * itemDataBlock = new ITEMDATA_STRUCTEXT[invenCount];
		BYTE * conversionBlock = (BYTE*)itemDataBlock;
        for(int loop=0;loop<itemDataBlockSize;loop++)
            conversionBlock[loop] = dataBlock[loop+headerSize];
        //--------------------
		int tracer = 0;
		for(loop=0;loop<header->m_storeItemCount;loop++)
		{//fill in bank data
         CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(itemDataBlock[loop].m_GLID);
		  if(invObj){
              CString output = operator+(invObj->m_itemData->m_itemName," cost:");
              output = operator+(output,CABLIB->ConvToString(itemDataBlock[loop].m_price));
			  if(invObj->m_itemData->m_stackable)
			    output = operator+(operator+(output," X"),CABLIB->ConvToString(itemDataBlock[loop].m_quanity));
			 AddStringToMenuList(52,output,invObj->m_itemData->m_visualRef,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,invObj->m_itemData->m_itemName,1,(" cost:" + CABLIB->ConvToString(itemDataBlock[loop].m_price)));
		  }
		  else{//alternate
             AddStringToMenuList(52,"Unknown",0,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,"Unknown",1,"");
		  }//end alternate
		  tracer++;
		}//end fill in bank data
		//----------------------
		
		for(loop=tracer;loop<invenCount;loop++)
		{//fill in player inventory
          CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(itemDataBlock[loop].m_GLID);
		   if(invObj){
             //AddStringToMenuList(51,invObj->m_itemData->m_itemName,-1,itemDataBlock[loop].m_GLID);
             CString output = invObj->m_itemData->m_itemName;
			 
             if(invObj->m_itemData->m_stackable)
			    output = operator+(operator+(invObj->m_itemData->m_itemName," X"),CABLIB->ConvToString(itemDataBlock[loop].m_quanity));
             AddStringToMenuList(51,output,invObj->m_itemData->m_visualRef,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,invObj->m_itemData->m_itemName,1,"");
		   }
		   else{//alternate
             AddStringToMenuList(51,"Unknown",-1,itemDataBlock[loop].m_GLID,1,"Unknown",1,"");
		   }//end alternate
		}//end fill in player inventory

        delete [] itemDataBlock;
		itemDataBlock = 0;
		}//end valid

	delete [] dataBlock;
    dataBlock=0;
	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CClientCommObj::HandleLootBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc)
{
       BYTE * dataBlock     = (BYTE *)lpMessage;
	   int headerSize       = sizeof(LOOTBLOCK_DATA);
	   int itemDataStructSz = sizeof(ITEMDATA_STRUCT);

       LPLOOTBLOCK_DATA header = (LPLOOTBLOCK_DATA)lpMessage;
       int itemDataSize = msgSize - headerSize;

       int itemCount = itemDataSize/itemDataStructSz;
       OpenMenu(header->m_menuLink,0,0,0,FALSE);//open loot menu

	   if(itemCount > 0)
	   {//valid fill data
	     long byteItemSz = sizeof(ITEMDATA_STRUCT)*itemCount;
         ITEMDATA_STRUCT * itemBlock = new ITEMDATA_STRUCT[itemCount];
         BYTE * conversionCast = (BYTE*)itemBlock;

	     int fill = headerSize;
	     for(int i=0;i<byteItemSz;i++)
		 {
           conversionCast[i] = dataBlock[fill];
		   fill++;
		 }
       
         for(int loop=0;loop<itemCount;loop++)
		 {
	       CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(itemBlock[loop].m_GLID);
	       if(invObj){
             CString output = invObj->m_itemData->m_itemName;
             if(invObj->m_itemData->m_stackable)
			    output = operator+(operator+(invObj->m_itemData->m_itemName," X"),CABLIB->ConvToString(itemBlock[loop].m_quanity));
             AddStringToMenuList(header->m_menuLink,output,invObj->m_itemData->m_visualRef,itemBlock[loop].m_GLID,itemBlock[loop].m_quanity,invObj->m_itemData->m_itemName,1,"");
		   }
	       else{//alternate
			 if(itemBlock[loop].m_GLID == -50){//cash
				CString output = operator+("CREDITS :",CABLIB->ConvToString(itemBlock[loop].m_quanity));
                AddStringToMenuList(header->m_menuLink,output,-1,itemBlock[loop].m_GLID,itemBlock[loop].m_quanity,output,1,"");
			 }//end cash
			 else{//ship!
                AddStringToMenuList(header->m_menuLink,"Unknown",-1,itemBlock[loop].m_GLID,0,"Unknown",1,"");
			 }
		   }//end alternate
		 }
		 if(itemBlock)
		 {
          delete [] itemBlock;
	      itemBlock = 0;
		 }
	   }//end valid fill data

         

	   if(dataBlock)
	   {
          delete [] dataBlock;
          dataBlock = 0;
	   }

	   return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CClientCommObj::ParseGroupListData(BYTE * data,int size)
{
    //close runtime use Menus
    for(POSITION posLoc = m_menuObjectList->GetHeadPosition();posLoc != NULL;)
	{//1
        CMenu3DObj * menu = (CMenu3DObj *)m_menuObjectList->GetNext(posLoc);
	    if(menu->m_closeOnEsc)
		   menu->CloseMenu();
	}//end 1
	OpenMenu(99,0,0,0,FALSE);//open group menu
	//fill data
	int trace = 1;//start at one because of catagorie
    
    int handle = 0;
	
    while(trace < size)
	{//fill in player inventory
       int handle = 0;
	   int stringSize = (int)data[trace];//get string size
	   char * string = new char[stringSize+1];
	   ZeroMemory(string,(stringSize+1));
	   trace++;
       //fill in handle
       BYTE handleParse[4];
	   handleParse[0] = data[trace];trace++;
	   handleParse[1] = data[trace];trace++;
	   handleParse[2] = data[trace];trace++;
	   handleParse[3] = data[trace];trace++;
	   CopyMemory(&handle,handleParse,4);
       //fill in name
	   for(int i=0;i<stringSize;i++){
           string[i] = data[trace];
		   trace++;
	   }

       AddStringToMenuList(99,string,-1,handle,0,string,0,"");
	   if(string)
	   {
          delete [] string;
		  string = 0;
	   }
	}//end fill in player inventory

	if(data)
	{
	  delete [] data;
	  data = 0;
	}

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CClientCommObj::ParseClanRosterData(BYTE * data,int size)
{
    //close runtime use Menus
    for(POSITION posLoc = m_menuObjectList->GetHeadPosition();posLoc != NULL;)
	{//1
        CMenu3DObj * menu = (CMenu3DObj *)m_menuObjectList->GetNext(posLoc);
	    if(menu->m_closeOnEsc)
		   menu->CloseMenu();
	}//end 1
	OpenMenu(800,0,0,0,FALSE);//open group menu
	//fill data
	int trace = 1;//start at one because of catagorie
    
    int handle = 0;
	
    while(trace < size)
	{//fill in player inventory
       int handle = 0;
	   int stringSize = (int)data[trace];//get string size
	   char * string = new char[stringSize+1];
	   ZeroMemory(string,(stringSize+1));
	   trace++;
       //fill in name
	   for(int i=0;i<stringSize;i++){
           string[i] = data[trace];
		   trace++;
	   }

       AddStringToMenuList(800,string,-1,handle,0,string,0,"");
	   if(string)
	   {
          delete [] string;
		  string = 0;
	   }
	}//end fill in player inventory

	if(data)
	{
	  delete [] data;
	  data = 0;
	}

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CClientCommObj::HandleSkillAbilitiesClientSide(BYTE * byteBlock,
											        int msgSz,
											        DPNID idFromLoc)
{
    LPSKILLABILITIES_HEADER formatedMessage = (LPSKILLABILITIES_HEADER)byteBlock;
    int headerSize = sizeof(SKILLABILITIES_HEADER);
	int dataTrack = headerSize;

	//close existing
	for(POSITION posLoc = m_menuObjectList->GetHeadPosition();posLoc != NULL;)
	{//emergency close all menus
         CMenu3DObj * menu = (CMenu3DObj *)m_menuObjectList->GetNext(posLoc);
	     if(menu->m_closeOnEsc)
	     menu->CloseMenu();
	}//end emergency close all menus
    //open menu
	OpenMenu(formatedMessage->m_menuID,0,0,0,FALSE);

    while(dataTrack < msgSz)
	{
        int textSz = byteBlock[dataTrack];
		dataTrack++;

		char * textFinal = new char[textSz+1];

		for(int loop=0;loop<textSz;loop++)
		{
		    textFinal[loop] = byteBlock[dataTrack];
		    dataTrack++;
		}
		textFinal[textSz] = 0;
		//CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(formatedMessage->m_glid);
		//if(invObj)
		   AddStringToMenuList(formatedMessage->m_menuID,textFinal,-1,formatedMessage->m_glid,0,textFinal,1,"");
	}

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CClientCommObj::HandleSkillsOutputClientSide(BYTE * byteBlock,
									          int msgSz,
										      DPNID idFromLoc)
{
    LPSKILLS_HEADER formatedMessage = (LPSKILLS_HEADER)byteBlock;
    int headerSize = sizeof(SKILLS_HEADER);
	int dataTrack = headerSize;

	//close existing
	for(POSITION posLoc = m_menuObjectList->GetHeadPosition();posLoc != NULL;)
	{//emergency close all menus
         CMenu3DObj * menu = (CMenu3DObj *)m_menuObjectList->GetNext(posLoc);
	     if(menu->m_closeOnEsc)
	     menu->CloseMenu();
	}//end emergency close all menus
    //open menu
	OpenMenu(141,0,0,0,FALSE);//hardcode

    for(int loop=0;loop<formatedMessage->m_skillCount;loop++)
	{
		int type = byteBlock[dataTrack];
		dataTrack++;
		CString level = CABLIB->ConvToString(((int)byteBlock[dataTrack]));
		dataTrack++;
        CString percentageTillNextLevel = CABLIB->ConvToString(((int)byteBlock[dataTrack]));
		dataTrack++;
		
	    CSkillObject * skillResult = (CSkillObject *)m_skillDatabase->GetSkillByType(type);
        if(skillResult)
		{
           CString build = operator+(skillResult->m_skillName," Level:");
		   build = operator+(build,level);
		   build = operator+(build," %");
		   build = operator+(build,percentageTillNextLevel);
		   AddStringToMenuList(141,build,-1,0,0,build,0,"");
		}
		else
		{
           CString build = "Unknown Skill Level:";
		   build = operator+(build,level);
		   AddStringToMenuList(141,build,-1,0,0,build,0,"");
		}
	}

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
BOOL CClientCommObj::HandleTextMessageClientSide(LPVOID lpMessage,int & msgSize,DPID & idFromLoc)
{
     int stringLength = msgSize - 4;
	 if(stringLength <= 0)
		return FALSE;
	 BYTE * dataBlock = (BYTE*)lpMessage;
	 char * newString = new char[stringLength+1];
     ZeroMemory(newString,(stringLength+1));
	 //filter multiple spacing / spacing-prevent spoof
	 BOOL lastSpace = FALSE;
	 int stringTrace = 0;
	 for(int charLoop=0;charLoop<stringLength;charLoop++)
	 {
         if(lastSpace)
            if(dataBlock[charLoop+4] == ' ')//skip
			   continue;
         newString[stringTrace] = dataBlock[charLoop+4];
		 stringTrace++;
		 if(dataBlock[charLoop+4] == ' ')
		    lastSpace = TRUE;
		 else
		    lastSpace = FALSE;
	 }

	 newString[stringLength] = 0;

	 CString loader = newString;


	 if(!m_filterSpeechDB->ShouldStatmentBeBlocked(loader))
	 {
		 #ifdef _chinaClient
		 if(dataBlock[0] == 0)//normal green
		 {
	        GL_systemReadout->AddLineChina(loader,g_pd3dDevice,m_symbolMapDB,0.1f,1.0f,0.1f,m_speechMode);
		 }
		 if(dataBlock[0] == 1)//group light purple
		 {
	        GL_systemReadout->AddLineChina(loader,g_pd3dDevice,m_symbolMapDB,0.8f,0.5f,0.8f,m_speechMode);
		 }
		 if(dataBlock[0] == 2)//clan light cyan
		 {
	        GL_systemReadout->AddLineChina(loader,g_pd3dDevice,m_symbolMapDB,0.39f,.74f,0.82f,m_speechMode);
		 }
         #endif
         #ifdef _ENGLISH
		 if(dataBlock[0] == 0)//normal green
		 {
	        GL_systemReadout->AddLine(loader,g_pd3dDevice,m_symbolMapDB,0.1f,1.0f,0.1f,m_speechMode);
		 }
		 if(dataBlock[0] == 1)//group light purple
		 {
			 // SET CHAT COLOR FOR GROUP
			 //GL_systemReadout->AddLine(loader,g_pd3dDevice,m_symbolMapDB,0.8f,0.5f,0.8f,m_speechMode);
			 GL_systemReadout->AddLine(loader,g_pd3dDevice,m_symbolMapDB,GL_systemReadout->m_guildTextRed,GL_systemReadout->m_guildTextGreen,GL_systemReadout->m_guildTextBlue,m_speechMode);
		 }
		 if(dataBlock[0] == 2)//clan light cyan
		 {
			 // SET CHAT COLOR FOR CLAN
			 //GL_systemReadout->AddLine(loader,g_pd3dDevice,m_symbolMapDB,0.39f,.74f,0.82f,m_speechMode);
			 GL_systemReadout->AddLine(loader,g_pd3dDevice,m_symbolMapDB,GL_systemReadout->m_clanTextRed,GL_systemReadout->m_clanTextGreen,GL_systemReadout->m_clanTextBlue,m_speechMode);
		 }
         #endif
	 }


	 if(newString)
		  delete [] newString;
	      newString = 0;


	 //delete [] dataBlock;
     //dataBlock=0;
	 return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
BOOL CClientCommObj::HandleClientStatBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc)
{
    LPSTATBLKHEADER_DATA statBlockData = (LPSTATBLKHEADER_DATA)lpMessage;
	BYTE * dataBlock = (BYTE *)lpMessage;
	
	POSITION posLoc = EntObjRuntime->FindIndex(0);
	if(posLoc)
	{
	   CMovementObj * mPtr = (CMovementObj *)EntObjRuntime->GetAt(posLoc);
	   if(mPtr)
	   {
		  int oldEnergy = mPtr->List_energy;
	      mPtr->List_energy = statBlockData->m_hp;
          mPtr->List_maxEnergy = statBlockData->m_maxHp;

		  if(oldEnergy > mPtr->List_energy)
		  m_screenFlashModule->DoIt(100,g_pd3dDevice);

          if(statBlockData->m_hp <= 0)
		     m_globalAvVar = FALSE;

	   }
	}

	GL_mutiplayerObject->m_currentStat1 = statBlockData->m_str;
	GL_mutiplayerObject->m_currentStat2 = statBlockData->m_dex;
    GL_mutiplayerObject->m_currentStat3 = statBlockData->m_wis;
  
	if(statBlockData->m_stasis == 0)
       GL_mutiplayerObject->m_currentStasis = "NORMAL";
	if(statBlockData->m_stasis == 1)
       GL_mutiplayerObject->m_currentStasis = "INVIS";
	if(statBlockData->m_stasis == 2)
       GL_mutiplayerObject->m_currentStasis = "SHIFTED";

	if(m_skillDatabase)//get title
       GL_mutiplayerObject->m_titleLink = m_skillDatabase->GetTitleByLevelAndType(statBlockData->m_skillType,statBlockData->m_level);

	
    int parseTrace = sizeof(STATBLKHEADER_DATA);
	int elementCount = msgSize - parseTrace;

    
    if(GL_mutiplayerObject->m_elementStatus)
	{
       GL_mutiplayerObject->m_elementStatus->SafeDelete();
	   delete GL_mutiplayerObject->m_elementStatus;
	   GL_mutiplayerObject->m_elementStatus = 0;
	}
       GL_mutiplayerObject->m_elementStatus = new CElementStatusList();

	for(int eleLoop = 0;eleLoop < elementCount;eleLoop++)
	{
        CElementStatusObj * elementPtr = new CElementStatusObj();
		elementPtr->m_currentValue = dataBlock[parseTrace];
	    elementPtr->m_elementIndex = eleLoop;
		GL_mutiplayerObject->m_elementStatus->AddTail(elementPtr);
        parseTrace++;
	}
    

	delete [] dataBlock;
    dataBlock=0;

	return TRUE;
}
//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
void CClientCommObj::HandleMiscBlocksClientSide(BYTE * miscUpdateBlock, int miscBlockSize)
{
           if(miscUpdateBlock[0] == 2)
		   {//fill in client offered items
			  int headerSz = sizeof(TRADE_HEADER);
              int istructSz = sizeof(ITEMDATA_STRUCT);
		      int itemCount = (miscBlockSize-headerSz)/istructSz;
		 
			    BYTE * headerBytes = NULL;
				
				TRADE_HEADER * header = (TRADE_HEADER * )miscUpdateBlock;
				
				if(header->m_cashAmount > 0)
				{
                     CString output = operator+("CREDITS :",CABLIB->ConvToString(header->m_cashAmount));
                     AddStringToMenuList(59,output,-1,-50,0,output,0,"");
				}
				
				if(itemCount > 0)
				{
				 int valueByteCount = sizeof(ITEMDATA_STRUCT)*itemCount;
			     ITEMDATA_STRUCT * itemArray = NULL;
				 BYTE * castBytes = NULL;
				 itemArray = new ITEMDATA_STRUCT[itemCount];
				 castBytes = (BYTE *)itemArray;
			     for(int j=0;j<valueByteCount;j++)
				     castBytes[j] = miscUpdateBlock[j+headerSz];
	

				 for(int menuFillLoop=0;menuFillLoop<itemCount;menuFillLoop++)
				 {//menu fill loop
			      CGlobalInventoryObj * invObj = m_globalInventoryDB->GetByGLID(itemArray[menuFillLoop].m_GLID);
		          if(invObj){
                   if(!invObj->m_itemData->m_stackable)
				   {
					   AddStringToMenuList(59,invObj->m_itemData->m_itemName,invObj->m_itemData->m_visualRef,itemArray[menuFillLoop].m_GLID,1,invObj->m_itemData->m_itemName,1,"");
				   }
			       else if(!AddSelectedFromMenuList(59,itemArray[menuFillLoop].m_GLID,itemArray[menuFillLoop].m_quanity))
				   {
                       if(invObj->m_itemData->m_stackable){
						  CString quanityTxt = CABLIB->ConvToString(itemArray[menuFillLoop].m_quanity);
						  CString outPut = operator+(invObj->m_itemData->m_itemName,operator+(" X",quanityTxt));
						  AddStringToMenuList(59,outPut,invObj->m_itemData->m_visualRef,itemArray[menuFillLoop].m_GLID,itemArray[menuFillLoop].m_quanity,outPut,1,"");
					   }
                       else
                          AddStringToMenuList(59,invObj->m_itemData->m_itemName,invObj->m_itemData->m_visualRef,itemArray[menuFillLoop].m_GLID,1,invObj->m_itemData->m_itemName,1,"");
				   }
				  }//alternate
				 }//end menu fill loop
				 delete [] itemArray;
				 itemArray = 0;
				}
		   }//end fill in client offered items
           if(miscUpdateBlock[0] == 3)
		   {//recent quest journal entry update
		      int tracerBuild = 1;
              if(GL_mutiplayerObject->m_recentQuestPtr)
			  {
                 GL_mutiplayerObject->m_recentQuestPtr->SafeDelete();
				 delete GL_mutiplayerObject->m_recentQuestPtr;
				 GL_mutiplayerObject->m_recentQuestPtr = 0;
			  }
              GL_mutiplayerObject->m_recentQuestPtr = new CQuestJournalObj();
			  GL_mutiplayerObject->m_recentQuestPtr->m_requiredItems = new CQJRequiredItemList();
			   
			  int curStrLen = miscUpdateBlock[tracerBuild];
		      char * listText = new char[curStrLen+1];
		      tracerBuild++;
              for(int loop=0;loop<curStrLen;loop++)
		      {
		       listText[loop] = miscUpdateBlock[tracerBuild];
			   tracerBuild++;
		      }   
		      listText[curStrLen] = 0;//null terminator
		      GL_mutiplayerObject->m_recentQuestPtr->m_questDescription = listText;
			  delete [] listText;
			  listText = 0;
			  
			  curStrLen = miscUpdateBlock[tracerBuild];
		      listText = new char[curStrLen+1];
		      tracerBuild++;
              for(int loop=0;loop<curStrLen;loop++)
		      {
		       listText[loop] = miscUpdateBlock[tracerBuild];
			   tracerBuild++;
		      }   
		      listText[curStrLen] = 0;//null terminator
		      GL_mutiplayerObject->m_recentQuestPtr->m_npcName = listText;
			  delete [] listText;
			  listText = 0;

			  curStrLen = miscUpdateBlock[tracerBuild];
		      listText = new char[curStrLen+1];
		      tracerBuild++;
              for(int loop=0;loop<curStrLen;loop++)
		      {
		       listText[loop] = miscUpdateBlock[tracerBuild];
			   tracerBuild++;
		      }   
		      listText[curStrLen] = 0;//null terminator
		      GL_mutiplayerObject->m_recentQuestPtr->m_keyStatement = listText;
			  delete [] listText;
			  listText = 0;
			  
			  while(tracerBuild<miscBlockSize)
			  {

				  curStrLen = miscUpdateBlock[tracerBuild];
		          listText = new char[curStrLen+1];
		          tracerBuild++;
                  for(int loop=0;loop<curStrLen;loop++)
		          {
		              listText[loop] = miscUpdateBlock[tracerBuild];
			          tracerBuild++;
		          }   
		          listText[curStrLen] = 0;//null terminator
		          CQJRequiredItem * newItem = new CQJRequiredItem();
				  newItem->m_itemName = listText;
                  GL_mutiplayerObject->m_recentQuestPtr->m_requiredItems->AddTail(newItem);
			      delete [] listText;
			      listText = 0;
			  }
		   }//recent quest journal entry update
		   if(miscUpdateBlock[0] == 4)
		   {//recent quest dialog
              for(POSITION posLoc = m_menuObjectList->GetHeadPosition();posLoc != NULL;)
	          {//1
                  CMenu3DObj * menu = (CMenu3DObj *)m_menuObjectList->GetNext(posLoc);
	              if(menu->m_closeOnEsc)
			         menu->CloseMenu();
	          }//end 1
	          OpenMenu(92,0,0,0,FALSE);

	          //parse out test objects
			  int maxLineLength = 35;
			  int tracerBuild = 1;
			  float lineLengthMeasured = 0.0;
			  float maxLineLengthMeasured = 0.3;
			  if(m_curModeWidth > 1000)
			  {
                 maxLineLengthMeasured = maxLineLengthMeasured * .8f;
			  }
			  float characterSpacing = GL_systemReadout->m_cgFontSizeX*2.0f;
			  CString tempCS = "";
              while(tracerBuild < miscBlockSize)
	          {
		          //int curStrLen = miscUpdateBlock[tracerBuild];
		          //char * listText = new char[curStrLen+1];
		          //tracerBuild++;

				  CString curBuild = "";
                  lineLengthMeasured = 0.0f;
                  char additionTemp;
                  for(int loop=0;loop<20000;loop++)
		          {
					  if(tracerBuild >= miscBlockSize)
					     break;
		              //listText[loop] = miscUpdateBlock[tracerBuild];
					  additionTemp = miscUpdateBlock[tracerBuild];
					  curBuild = curBuild + additionTemp;
					  tempCS = additionTemp;
                      CSymbolMapperObj * tempref = m_symbolMapDB->GetByID(tempCS);
					  lineLengthMeasured += characterSpacing * tempref->m_percentageX;

			          tracerBuild++;
					  if(lineLengthMeasured > maxLineLengthMeasured)
					  {
					    //tracerBuild--;
                        for(int j=0;j<tracerBuild;j++)
						{
							
							additionTemp = miscUpdateBlock[tracerBuild];
							if(additionTemp == ' ')
							   break;

                            tracerBuild--;
							curBuild.Delete(curBuild.GetLength()-1,1);
						}
					    break;
					  }

					  
		          }   
		          //listText[curStrLen] = 0;//null terminator
			   
		          AddStringToMenuList(92,curBuild,-1,0,0,curBuild,0,"");
	          }
		   }//end recent quest dialog
		   if(miscUpdateBlock[0] == 5)
		   {//report murder option

			  int tracerBuild = 3;
			  int menuID = miscUpdateBlock[1];
			  int nameLength = miscUpdateBlock[2];
			  int handleTemp = 0;

              CString m_murdererName = "";
		      char * listText = new char[nameLength+1];
              for(int loop=0;loop<nameLength;loop++)
		      {
		       listText[loop] = miscUpdateBlock[tracerBuild++];
		      }   
		      listText[nameLength] = 0;//null terminator
			  m_murdererName = listText;
			  delete [] listText;
			  listText = 0;

			  BYTE xFloatCast[4];
			  for(loop = 0;loop<4;loop++) 
				  xFloatCast[loop] = miscUpdateBlock[tracerBuild++];
			  CopyMemory(&handleTemp,xFloatCast,sizeof(int));

			  OpenMenu(menuID,0,handleTemp,0,FALSE);
			  SetMenuTextItem(10,"You have been murdered by "+m_murdererName);
		   }//end report murder option
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
BOOL EngineXWin::SpawnEntityBasedOnPositionAndCfg(D3DVECTOR position, 
												  int aiCfg,
												  int dynamicSpawnId,
												  long lifeMaxTime)
{

	 CAIObj * aiPtr = AI_List->GetByIndex(aiCfg);
     if(!aiPtr)
	 {//Invalid ai ptr
	   return FALSE;
	 }//end invalid ai ptr


     //idiot protection
	 if(lifeMaxTime > 2700000)
		lifeMaxTime = 2700000;


	 if(dynamicSpawnId > -1)
	 {//this spawn cannot preexist in the world-check to see if already spawned
        //if(EntObjRuntime->GetObjectByDynaID(dynamicSpawnId))
		   //return FALSE;

		if(EntObjRuntime->GetObjectCountByDynaID(dynamicSpawnId) > 2)
		   return FALSE;
	 }//end this spawn cannot preexist in the world
		   
     //add conditions for not spawning
     D3DMATRIX successMatrice = MatrixInit2;//spawnGenPtr->List_spawnMatrix;
	 successMatrice._41 = position.x;
	 successMatrice._42 = position.y;
	 successMatrice._43 = position.z;
     CMovementObj * refPtr = NULL;
     BOOL result = ActivateMovementEntity(NULL,
			                              aiCfg,
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
										  -1,
										  -1,
										  &refPtr,
					                      NULL,
										  -1,
										  -1);
	  if(result){//valid spawn
	   if(refPtr->m_aiObj){//if AI
		  refPtr->m_aiObj->m_routeCurTarget = 0;
		  refPtr->m_aiObj->m_mapCurLocation = 0;
          //refPtr->m_aiObj->m_curMap         = spawnGenPtr->m_treeUse;
		  refPtr->m_dynaSpawnID = dynamicSpawnId;
		  refPtr->m_lifeTimeMax = lifeMaxTime;
           
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
	   }
	  }
	  return TRUE;
}
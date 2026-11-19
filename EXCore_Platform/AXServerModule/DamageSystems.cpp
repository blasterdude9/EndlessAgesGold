#include "StdAfx.h"
#include "damagesystems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDamageSystems::CDamageSystems(CSharedNet * sharedInterface)
{
	 m_sharedInterfaceRef = sharedInterface;
}

CDamageSystems::~CDamageSystems(void)
{
}
//------------------------------------------------------------------------------------//
BOOL CDamageSystems::HandleDamageMsgServerSide_safe(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,int & curSimutanius)
{
	LPHITPLAYER_DATA formatedMessage = (LPHITPLAYER_DATA)lpMessage;
	m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Lock();//must be before runtimeplayeers, future redesign indexer on damage system to be by player object 
    CPlayerObject * playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->m_attackerNetowkID);
	if(playerObjectData == NULL)
	{//invalid player
	   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
	   m_sharedInterfaceRef->m_eventLog->LogByPriority("Player attempted damage and is not in game-or victim is not on server anymore",2);
	   return FALSE;
	}//end invalid player

	HandleDamageMsgServerSide((LPHITPLAYER_DATA)lpMessage,msgSize,idFromLoc,curSimutanius,playerObjectData);

	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CDamageSystems::HandleDamageMsgServerSide(LPHITPLAYER_DATA formatedMessage,int & msgSize,DPID & idFromLoc,int & curSimutanius,CPlayerObject * playerObjectData)
{
	//runtime entity list is already locked
	CString attackerAccountName = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountNameByDPNID(idFromLoc);

	DWORD currentTime = GetTickCount();	
	//test CRC
		float serverSideMsgCRC = m_sharedInterfaceRef->m_netComObject->GetDamageMsgCrC(formatedMessage);
		if(((int)serverSideMsgCRC) != ((int)formatedMessage->CRCCHECK)){//verify CRC integrity
           m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid CRC from account dm :",attackerAccountName);
		   m_sharedInterfaceRef->m_eventLog->AddEvent("SVR calculated CRC :",serverSideMsgCRC);
		   m_sharedInterfaceRef->m_eventLog->AddEvent("Client calculated CRC :",formatedMessage->CRCCHECK);
           if(msgSize > 0)
		   {
		   
		   }	
           m_sharedInterfaceRef->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
		   return FALSE;
		}//end verify CRC integrity

		
		float damageAmount = 0;
		int skillTypeUsed      = -1;
		if(formatedMessage->m_type == 0)
		{
			CMissileObj * misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(formatedMessage->m_miscValue);//missile index
			if(misPtr)
			   skillTypeUsed = misPtr->m_criteriaForSkillUse;
		}
		else if(formatedMessage->m_type == 1)
		{
			CArmedInventoryObj * visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(formatedMessage->m_miscValue);
			if(visObj)
			{//valid attachment reference
				if(visObj->m_thirdPersonSystem)
					if(visObj->m_thirdPersonSystem->m_damageSphereList)
						for(POSITION dPos = visObj->m_thirdPersonSystem->m_damageSphereList->GetHeadPosition();dPos != NULL;)
						{//damage paths
							CDamagePathObj* damagePathObj = (CDamagePathObj*)visObj->m_thirdPersonSystem->m_damageSphereList->GetNext(dPos);
							skillTypeUsed = damagePathObj->m_criteriaForSkillUse;
						}//end damage paths
			}
		}
		else if(formatedMessage->m_type == 2)
		{
			CExplosionObj* expPtr = m_sharedInterfaceRef->m_explosionDBList->GetPtr(formatedMessage->m_miscValue);
			if(expPtr)
			   skillTypeUsed = expPtr->m_criteriaForSkillUse;
		}

        int scIncre = 0;
		BOOL proceedAfterInc = TRUE;

		if(m_sharedInterfaceRef->m_damageMode == 1)
			damageAmount = ChronicleDamageCalculation(formatedMessage, msgSize, idFromLoc, curSimutanius, playerObjectData);
		else if(m_sharedInterfaceRef->m_damageMode == 2)
			damageAmount = EATwoDamageCalculation(formatedMessage, msgSize, idFromLoc, curSimutanius, playerObjectData);
		else
			proceedAfterInc = EADamageCalculation(damageAmount,formatedMessage, msgSize, idFromLoc, curSimutanius, playerObjectData, scIncre);

		//generate index
	    int securityIndexer = 0;

		if(!m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->IndexSecurityIndexerByDPNID(idFromLoc,scIncre,&securityIndexer))//account sys already locked at this point
           m_sharedInterfaceRef->m_eventLog->AddEvent(attackerAccountName +" -IndexSecurityIndexerByDPNID() cant locate account");

		if(playerObjectData->m_admin)//admin passes always
		   securityIndexer = formatedMessage->m_securityIndexer;

		if(securityIndexer != formatedMessage->m_securityIndexer)
		{//test indexed data
		   m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid Index from player-disconnecting :",playerObjectData->m_charName);
		   m_sharedInterfaceRef->m_eventLog->AddEvent("SVR index :",((float)formatedMessage->m_securityIndexer));
		   m_sharedInterfaceRef->m_eventLog->AddEvent("Client index :",((float)formatedMessage->m_securityIndexer));
		   m_sharedInterfaceRef->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
		   if(msgSize > 0)
		   {

		   }
		   return FALSE;
		}//end test indexed data

		CPlayerObject * victimObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->m_playerThatGotHitID);//aquire player that was damaged
		if(!victimObjectData)
			return FALSE;	

		if(victimObjectData->m_energy < 1)
			return FALSE;//cant shoot already dead entities


        //exploit check
		if(playerObjectData->m_currentWorldID != victimObjectData->m_currentWorldID)//cannot damage from different zones
		{
		   m_sharedInterfaceRef->m_eventLog->AddEvent("***Cross zone damage illegal :",playerObjectData->m_charName);
		   return FALSE;
		}

		//rule will make any hit instant kill
		if(damageAmount > 0)//damage hit
		if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObjectData->m_stencilChannel,playerObjectData->m_currentWorldID,WORLDRULE_INSTANTKILL))
			damageAmount = victimObjectData->m_energy + 1;
		if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObjectData->m_stencilChannel,playerObjectData->m_currentWorldID,WORLDRULE_THREE_HIT_KILL))
			damageAmount = (float)victimObjectData->m_maxEnergy * 0.4f;
        //check if invunerable zone
		if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObjectData->m_stencilChannel,playerObjectData->m_currentWorldID,WORLDRULE_FILTERALLDMG))
		{
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
					                          idFromLoc,
					                          "Cannot attack this player you are in a invunerable zone",		
						                      0,
											  playerObjectData,
											  3);
			return FALSE;	
		}

		//check PC safe zone
		if(playerObjectData->m_aiControlled == FALSE && victimObjectData->m_aiControlled == FALSE)
		if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObjectData->m_stencilChannel,playerObjectData->m_currentWorldID,WORLDRULE_FILTERPCDMG))
		{
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
					                          idFromLoc,
					                          "Cannot attack this player you are in a safe zone",
						                      0,
											  playerObjectData,
											  3);
			return FALSE;	
		}

		

		if(!proceedAfterInc)
			return FALSE;

		if(damageAmount == 0.0f)
		{
			m_sharedInterfaceRef->m_eventLog->LogByPriority(playerObjectData->m_charName + " zero damage inflicted error",2);
			return FALSE;
		}

		//if(!playerObjectData->m_aiControlled)
		if(victimObjectData->m_reflectLevel > 0.0f)
		{//refect will turn damage around onto the attacker
		   float reflectedDamage = damageAmount * victimObjectData->m_reflectLevel;
           playerObjectData->m_energy -= reflectedDamage;
		   if(playerObjectData->m_energy < 1)
		   {//handle death
		      playerObjectData->m_energy = 0;
			  if(!playerObjectData->m_aiControlled)
					m_sharedInterfaceRef->UpdateClientStatistics(playerObjectData,playerObjectData->m_currentID);
			  else
					m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
														  playerObjectData->m_currentID,
														  3,
														  playerObjectData->m_energy,
														  playerObjectData->m_networkAssignedID,
														  victimObjectData->m_networkAssignedID,//attacker assigned id
														  playerObjectData->m_maxEnergy,
														  playerObjectData);// this lets the ai know hes it being jacked up and whos doing it
			  
			  
			  m_sharedInterfaceRef->HandleDeath( playerObjectData,victimObjectData);//killed self
			  return FALSE;//dont passthrough
		   }//end handle death
           damageAmount = damageAmount - reflectedDamage;
		   if(damageAmount < 1)
		      return FALSE;//dont passthrough
		}


		if(victimObjectData->m_teamTemp == playerObjectData->m_teamTemp)
		   if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObjectData->m_stencilChannel,playerObjectData->m_currentWorldID,WORLDRULE_FILTERTEAMFIRE))
				return FALSE;	

		//Remove ability to deal damage to group members when not in an arena
		//This will still allow to heal group members though
		if(damageAmount > 0)
		{
			if(playerObjectData->m_groupID && victimObjectData->m_groupID && !playerObjectData->m_aiControlled && !victimObjectData->m_aiControlled)
				if(playerObjectData->m_groupID == victimObjectData->m_groupID && !playerObjectData->m_inArena && !victimObjectData->m_inArena)
					return FALSE;
		}

		if(damageAmount > 0)
		{//damageing hit
		  if(!(int)damageAmount)
			  damageAmount = 1;
			victimObjectData->m_energy -= (int)damageAmount;
			
		  if(!victimObjectData->m_aiControlled)
		  {
			CString msgBuild = playerObjectData->m_charName + " has damaged you for " + m_sharedInterfaceRef->m_cabLib.ConvToString(damageAmount) + ".";
			m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
												victimObjectData->m_currentID,
												msgBuild,
												0,
												victimObjectData,
												3);//confermation
		  }

		  //Arena Tracking Info
		  if(victimObjectData->m_inArena && playerObjectData->m_inArena && !playerObjectData->m_aiControlled)
			  if(victimObjectData->m_currentID != playerObjectData->m_currentID)
				  playerObjectData->m_arenaInfo[4] += damageAmount;

		  if(victimObjectData->m_aiControlled)
		  {//inform ai of attacker and health
             m_sharedInterfaceRef->SendAttribMsg( DPNSEND_GUARANTEED,
												  victimObjectData->m_currentID,
												  3,
												  victimObjectData->m_energy,
												  victimObjectData->m_networkAssignedID,
												  formatedMessage->m_attackerNetowkID,//attacker assigned id
												  victimObjectData->m_maxEnergy,
												  victimObjectData);// this lets the ai know hes it being jacked up and whos doing it

		  }//end inform ai of attacker and health
		  
		  playerObjectData->Attack(victimObjectData,350000,(int)damageAmount);//for murder agressor lists

		  if(!playerObjectData->m_aiControlled)
		  {
			 if(victimObjectData->m_energy > 0)
				 m_sharedInterfaceRef->SendDamageInfoMessageHandler((int)damageAmount,
																	victimObjectData->m_networkAssignedID,
																	playerObjectData);
			 else
			 {
				m_sharedInterfaceRef->SendDamageInfoMessageHandler((int)damageAmount,
																   victimObjectData->m_networkAssignedID,
																   playerObjectData,
																   true);
			 }
		  }
		  victimObjectData->m_skillUseTimeStamp = GetTickCount();//interupt his skill use
		  if(damageAmount < 1)
			 return FALSE;//dont pass  no damage done
			 
	   }//end damageing hit
	   else
	   {//healing hit
	      if(!m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(victimObjectData->m_stencilChannel,victimObjectData->m_currentWorldID,WORLDRULE_NOHEAL))
		  {
	        victimObjectData->m_energy -= (int)damageAmount;
			
			  if(!victimObjectData->m_aiControlled)
			  {
				CString msgBuild = playerObjectData->m_charName + " has healed you for " + m_sharedInterfaceRef->m_cabLib.ConvToString(abs(damageAmount)) + ".";
				m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
													victimObjectData->m_currentID,
													msgBuild,
													0,
													victimObjectData,
													3);//confermation
			  }

			if(victimObjectData->m_energy > victimObjectData->m_maxEnergy)
			   victimObjectData->m_energy = victimObjectData->m_maxEnergy;//cap it

		    if(!playerObjectData->m_aiControlled)
		    m_sharedInterfaceRef->SendDamageInfoMessageHandler((int)damageAmount,
								         victimObjectData->m_networkAssignedID,
								         playerObjectData);
		  }
		  else
		    m_sharedInterfaceRef->SendGeneralMessageHandler(89,playerObjectData);//inform of problem with use
	   }//healing hit
	   
       
	   if(damageAmount > 0)//must be a damage hit no healing
	   if(!playerObjectData->m_aiControlled)
       if(playerObjectData->m_stats)
	   {//valid statistical data

          if(playerObjectData->m_stasis == 1){//if you use item when hiding unhide
			 m_sharedInterfaceRef->Unhide(playerObjectData);
		  }//end if you use item when hiding unhide

		  BOOL updateStats = FALSE;//gl area
		  //STATS SECTION
		  CInventoryObj * weaponLink = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
		  if(weaponLink)
		  {//aquire weapon used
		     if(playerObjectData->m_stats->UseStatByID((float)weaponLink->m_itemData->m_requiredStr,2,playerObjectData->m_currentInventory))
			 {//you have increased the strength stat
              //calculate max energy
			  CMovementObj * DBPtr = m_sharedInterfaceRef->m_entObjList->GetObjByIndex(playerObjectData->m_curEDBIndex);
			  if(DBPtr)
			  {//valid db ref
                 int finalMaxEnergyBase = DBPtr->List_maxEnergy;
                 if(playerObjectData->m_overridedEnergy > 0)
		            finalMaxEnergyBase = playerObjectData->m_overridedEnergy;

                 playerObjectData->m_maxEnergy = playerObjectData->m_stats->GetBenefitBonus(1,playerObjectData->m_currentInventory) + finalMaxEnergyBase;
			     m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                          idFromLoc,
						                          "<STR UP> +1",			
							                       0,
												   playerObjectData,
												   0);//confermation
				 m_sharedInterfaceRef->SendDetailedMsgToClient(6,
															   145,
															   playerObjectData->m_networkAssignedID,
															   0,
															   playerObjectData);
				 updateStats = TRUE;
				 
			  }//end valid db ref
			 }//end you have increased the strength stat
			 if(playerObjectData->m_stats->UseStatByID((float)weaponLink->m_itemData->m_requiredDex,3,playerObjectData->m_currentInventory))
			 {//you have increased the Dex stat
			     m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                          idFromLoc,
						                          "<DEX UP> +1",		// For Chronicle
							                       0,
												   playerObjectData,
												   0);//confermation
				 m_sharedInterfaceRef->SendDetailedMsgToClient(6,
															   145,
															   playerObjectData->m_networkAssignedID,
															   0,
															   playerObjectData);
				 updateStats = TRUE; 
			 }//end you have increased the Dex stat
			 if(playerObjectData->m_stats->UseStatByID((float)weaponLink->m_itemData->m_requiredKnowledge,4,playerObjectData->m_currentInventory))
			 {//you have increased the Wis stat
			     m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                          idFromLoc,
						                          "<WIS UP> +1",		// For Chronicle
							                       0,
												   playerObjectData,
												   0);//confermation
				 m_sharedInterfaceRef->SendDetailedMsgToClient(6,
															   145,
															   playerObjectData->m_networkAssignedID,
															   0,
															   playerObjectData);
				 updateStats = TRUE; 
			 }//end you have increased the Wis stat
		  }//end aquire weapon used
          //SKILLS SECTION
		  if(skillTypeUsed != -1)
		  {//skill is used
             if(!playerObjectData->m_skills)
                 playerObjectData->m_skills = new CSkillObjectList();

			 int specialInstruction = 0;
             float expOfTarget = 1.0f;
			 if(victimObjectData->m_aiControlled)
			 {//get exp AI style
                expOfTarget = victimObjectData->m_expTempAIRef;
			 }//end get exp AI style
			 else{//get exp by other players best skill
				  expOfTarget = victimObjectData->m_currentHighestExperience*.001f;	 
			 }//end get exp by other players best skill

			 int miscInt = 0;
			 CSkillObject * newAddedSkillRetPtr = NULL;
			 CLevelRangeObject * levelUpObj = NULL;
			 CSkillObject * skillPtrReference = NULL;
			 int effectBind = 0;
			 long forcedNoDelay = 0;
			 if(victimObjectData->m_aiControlled)//added so only xp given on ai, not players
				 playerObjectData->m_skills->UseSkillByType( skillTypeUsed,//int type,
															 m_sharedInterfaceRef->m_skillDatabase,
															 (long)expOfTarget,
															 -1,//skill level use not valid useing this method
															 -1,//ability not valid here
															 playerObjectData->m_curEDBIndex,
															 playerObjectData->m_currentInventory,
															 &specialInstruction,
															 &newAddedSkillRetPtr,
															 playerObjectData->m_stats,
															 &updateStats,
															 &levelUpObj,
															 &skillPtrReference,
															 &miscInt,
															 m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar,
															 &effectBind,
															 forcedNoDelay,
															 playerObjectData->m_skillUseTimeStamp);


			 if(newAddedSkillRetPtr)
			 {//Notify new skill added
			    CString msgBuild = "You begin to train the skill-";
				CSkillObject * skillPtrRef = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(newAddedSkillRetPtr->m_skillType);
				if(skillPtrRef)
			       msgBuild = operator+(msgBuild,skillPtrRef->m_skillName);
			    m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,msgBuild,0,playerObjectData,0);//confermation
			 }//end Notify new skill added

			 if(skillPtrReference)
			 if(levelUpObj != NULL){//notify leveled up
				int levelNumber = 1;
				CString newTitle;
				CSkillObject * skillPtrRef = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(skillPtrReference->m_skillType);
		        if(skillPtrRef)
				{
				   skillPtrReference->EvaluateLevelAndTitle(newTitle,levelNumber,skillPtrRef);
                   CString msgBuild = "You are now Level "+m_sharedInterfaceRef->m_cabLib.ConvToString(levelNumber);
			       msgBuild = msgBuild+".";
//				   msgBuild = msgBuild+skillPtrRef->m_skillName;
			       m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,msgBuild,0,playerObjectData,3);//confermation
				   //play an effect on the player
				   m_sharedInterfaceRef->SendDetailedMsgToClient(6,
															     24,
															     playerObjectData->m_networkAssignedID,
															     0,
															     playerObjectData);
				}
			 }//end notify leveled up

		  }//end skill is used
          if(updateStats)
		     m_sharedInterfaceRef->UpdateClientStatistics(playerObjectData,idFromLoc);
	   }//end valid statistical data

	   //send damage player current energy
	   if(victimObjectData->m_inGame)
		  if(victimObjectData->m_currentID != 0 && victimObjectData->m_timeDied == -1)
		  {//dont send messages to a non network player ;) or player that already dead  
			  if(!victimObjectData->m_aiControlled)
				  m_sharedInterfaceRef->UpdateClientStatistics(victimObjectData,victimObjectData->m_currentID);	    
		  }//end dont send messages to a non network player ;) or player that already dead  

       //Flag the player for group update if necessary.
	   if(!victimObjectData->m_aiControlled && victimObjectData->m_groupID > 0)
		   victimObjectData->m_groupStruct.m_squadDataIsDirty = 1;

	   // Handle death if valid
	   if(damageAmount > 0 && victimObjectData->m_energy <= 0)
			m_sharedInterfaceRef->HandleDeath( victimObjectData,playerObjectData);

	   if(!playerObjectData->m_aiControlled)
			m_sharedInterfaceRef->UpdateClientStatistics(playerObjectData,idFromLoc);
	   
    return TRUE;
}
//------------------------------------------------------------------------------------//
int CDamageSystems::AIXVerifyWeaponProjectile(CPlayerObject * playerObjectData, int fireQueIndex, int fireQueOvrd)
{
	if(playerObjectData->m_aiControlled)
		return 1;

	if(fireQueOvrd > 0)
		if(fireQueIndex == fireQueOvrd)
		{
			return 1;
		}
	
	if(playerObjectData->m_weaponPass == fireQueIndex)
	{
		playerObjectData->m_weaponPass = -1;
		return 1;
	}
	playerObjectData->m_weaponPass = -1;
	/*if(playerObjectData->m_entityAddressRef)
	{//local firepower
		CMovementObj * mPtr = (CMovementObj *)playerObjectData->m_entityAddressRef;
		if(mPtr->List_skeletalObject)
			if(mPtr->List_skeletalObject->m_fireQueList)
				if(mPtr->List_skeletalObject->m_fireQueList->GetByMissileIndex(fireQueIndex))
					return 1;
	}//end local firepower*/
	   
	CInventoryObj * weaponLink = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
	if(weaponLink)
	{    
		if(!weaponLink->m_itemData)
			return 2;

		if(!weaponLink->m_itemData->m_attachmentRefList)
			return 3;

		for(POSITION posLoc = weaponLink->m_itemData->m_attachmentRefList->GetHeadPosition();posLoc != NULL;)
		{
			CAttachmentRefObj * ref = (CAttachmentRefObj *)weaponLink->m_itemData->m_attachmentRefList->GetNext(posLoc);
			CArmedInventoryObj * visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(ref->m_attachmentType);
			if(visObj)
			{//try to verify fire que
				if(visObj->m_firstPersonSystem)
					if(visObj->m_firstPersonSystem->m_fireQueList)
						if(visObj->m_firstPersonSystem->m_fireQueList->GetByMissileIndex(fireQueIndex))
							return 1;

				if(visObj->m_thirdPersonSystem)
					if(visObj->m_thirdPersonSystem->m_fireQueList)
						if(visObj->m_thirdPersonSystem->m_fireQueList->GetByMissileIndex(fireQueIndex))
							return 1;
			}//end try to verify fire que
		}
		//cannot find fire que and therefore
		return 4;
	}
	return 0;//cant find weapons
}

//------------------------------------------------------------------------------------//
int CDamageSystems::AIXVerifyDotAttachmentMissile(CPlayerObject *playerObjectData, int dotIndex)
{
	if(playerObjectData->m_aiControlled)
		return 1;

	CInventoryObj *weaponLink = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
	if(weaponLink)
	{
		if(!weaponLink->m_itemData)
			return 2;

		if(!weaponLink->m_itemData->m_attachmentRefList)
			return 3;

		for(POSITION posLoc = weaponLink->m_itemData->m_attachmentRefList->GetHeadPosition(); posLoc != NULL;)
		{
			CAttachmentRefObj * ref = (CAttachmentRefObj *)weaponLink->m_itemData->m_attachmentRefList->GetNext(posLoc);
		    CArmedInventoryObj * visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(ref->m_attachmentType);
			if(visObj)
			{//Verify the Dot attachment
				if(visObj->m_firstPersonSystem)
				{
					if(visObj->m_firstPersonSystem->m_fireQueList)
					{
						for(POSITION posLoc = visObj->m_firstPersonSystem->m_fireQueList->GetHeadPosition(); posLoc != NULL;)
						{
							CFireRangeObject *fPtr = (CFireRangeObject *)visObj->m_firstPersonSystem->m_fireQueList->GetNext(posLoc);
							CMissileObj *misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(fPtr->m_missileIndex);
							if(misPtr)
								if(misPtr->m_dotRefValue == dotIndex)
									return 1;
						}
					}
				}
				if(visObj->m_thirdPersonSystem)
				{
					if(visObj->m_thirdPersonSystem->m_fireQueList)
					{
						for(POSITION posLoc = visObj->m_thirdPersonSystem->m_fireQueList->GetHeadPosition(); posLoc != NULL;)
						{
							CFireRangeObject *fPtr = (CFireRangeObject *)visObj->m_thirdPersonSystem->m_fireQueList->GetNext(posLoc);
							CMissileObj *misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(fPtr->m_missileIndex);
							if(misPtr)
								if(misPtr->m_dotRefValue == dotIndex)
									return 1;
						}
					}
				}
			}//End verify the dot attachment
		}
		//cannot find dot
		return 4;
	}
	//Cannot find weapon
	return 0;
}

//------------------------------------------------------------------------------------//
int CDamageSystems::AIXVerifyDotAttachmentMelee(CPlayerObject *playerObjectData, int dotIndex)
{
	if(playerObjectData->m_aiControlled)
		return 1;

	CInventoryObj *weaponLink = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
	if(weaponLink)
	{
		if(!weaponLink->m_itemData)
			return 2;

		if(!weaponLink->m_itemData->m_attachmentRefList)
			return 3;

		for(POSITION posLoc = weaponLink->m_itemData->m_attachmentRefList->GetHeadPosition(); posLoc != NULL;)
		{
			CAttachmentRefObj * ref = (CAttachmentRefObj *)weaponLink->m_itemData->m_attachmentRefList->GetNext(posLoc);
		    CArmedInventoryObj * visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(ref->m_attachmentType);
			if(visObj)
			{//Verify the Dot attachment
				if(visObj->m_firstPersonSystem)
				{
					if(visObj->m_firstPersonSystem->m_damageSphereList)
					{
						for(POSITION posLoc = visObj->m_firstPersonSystem->m_damageSphereList->GetHeadPosition(); posLoc != NULL;)
						{
							CDamagePathObj *damagePathObj = (CDamagePathObj*)visObj->m_firstPersonSystem->m_damageSphereList->GetNext(posLoc);
							if(damagePathObj)
								if(damagePathObj->m_damageDotValue == dotIndex)
									return 1;
						}
					}
				}
				if(visObj->m_thirdPersonSystem)
				{
					if(visObj->m_thirdPersonSystem->m_damageSphereList)
					{
						for(POSITION posLoc = visObj->m_thirdPersonSystem->m_damageSphereList->GetHeadPosition(); posLoc != NULL;)
						{
							CDamagePathObj *damagePathObj = (CDamagePathObj*)visObj->m_firstPersonSystem->m_damageSphereList->GetNext(posLoc);
							if(damagePathObj)
								if(damagePathObj->m_damageDotValue == dotIndex)
									return 1;
						}
					}
				}
			}//End verify the dot attachment
		}
		//cannot find dot
		return 4;
	}
	//Cannot find weapon
	return 0;
}

//------------------------------------------------------------------------------------//
int CDamageSystems::AIXVerifyWeaponExplosion(CPlayerObject * playerObjectData, int explosionQueIndex)
{
     if(playerObjectData->m_aiControlled)
		return 1;

	 /*if(playerObjectData->m_entityAddressRef)
	 {//local firepower
        CMovementObj * mPtr = (CMovementObj *)playerObjectData->m_entityAddressRef;
        if(mPtr->List_skeletalObject)
           if(mPtr->List_skeletalObject->m_fireQueList)
		   {
              for(POSITION posLoc = mPtr->List_skeletalObject->m_fireQueList->GetHeadPosition();posLoc != NULL;)
			  {
                  CFireRangeObject * fPtr = (CFireRangeObject *)mPtr->List_skeletalObject->m_fireQueList->GetNext(posLoc);
				  CMissileObj * misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(fPtr->m_missileIndex);
				  if(misPtr->m_explosionLink == explosionQueIndex)
				     return 1;
			  }
		   }
              
	 }//end local firepower*/
	   

     CInventoryObj * weaponLink = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
     if(weaponLink)
	 {
        if(!weaponLink->m_itemData)
			return 2;

		if(!weaponLink->m_itemData->m_attachmentRefList)
		    return 3;

        for(POSITION posLoc = weaponLink->m_itemData->m_attachmentRefList->GetHeadPosition();posLoc != NULL;)
		{
			CAttachmentRefObj * ref = (CAttachmentRefObj *)weaponLink->m_itemData->m_attachmentRefList->GetNext(posLoc);
		    CArmedInventoryObj * visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(ref->m_attachmentType);
            if(visObj)
			{//try to verify fire que
			   if(visObj->m_firstPersonSystem)
			      if(visObj->m_firstPersonSystem->m_fireQueList)
                  {
					  for(POSITION posLoc = visObj->m_firstPersonSystem->m_fireQueList->GetHeadPosition();posLoc != NULL;)
					  {
						CFireRangeObject * fPtr = (CFireRangeObject *)visObj->m_firstPersonSystem->m_fireQueList->GetNext(posLoc);
						CMissileObj * misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(fPtr->m_missileIndex);
						if(misPtr)
						  if(misPtr->m_explosionLink == explosionQueIndex)
							return 1;
					  }
				  }

			   if(visObj->m_thirdPersonSystem)
			      if(visObj->m_thirdPersonSystem->m_fireQueList)
                  {
					  for(POSITION posLoc = visObj->m_thirdPersonSystem->m_fireQueList->GetHeadPosition();posLoc != NULL;)
					  {
						CFireRangeObject * fPtr = (CFireRangeObject *)visObj->m_thirdPersonSystem->m_fireQueList->GetNext(posLoc);
						CMissileObj * misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(fPtr->m_missileIndex);
						if(misPtr)
						  if(misPtr->m_explosionLink == explosionQueIndex)
							return 1;
					  }
				  }
			}//end try to verify fire que
		}
		//cannot find fire que and therefore
		return 4;
	 }
	 return 0;//cant find weapons
}
//------------------------------------------------------------------------------------//
int CDamageSystems::AIXVerifyAttachment(CPlayerObject * playerObjectData, int attachment)
{
     if(playerObjectData->m_aiControlled)
		return 1;

	 /*if(playerObjectData->m_entityAddressRef)
	 {//local firepower
        CMovementObj * mPtr = (CMovementObj *)playerObjectData->m_entityAddressRef;
        if(mPtr->List_skeletalObject)
           if(mPtr->List_skeletalObject->m_fireQueList)
              if(mPtr->List_skeletalObject->m_fireQueList->GetByMissileIndex(fireQueIndex))
			     return 1;
	 }//end local firepower*/
	   

     CInventoryObj * weaponLink = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
     if(weaponLink)
	 {
        if(!weaponLink->m_itemData)
			return 2;

		if(!weaponLink->m_itemData->m_attachmentRefList)
		    return 3;

        for(POSITION posLoc = weaponLink->m_itemData->m_attachmentRefList->GetHeadPosition();posLoc != NULL;)
		{
			CAttachmentRefObj * ref = (CAttachmentRefObj *)weaponLink->m_itemData->m_attachmentRefList->GetNext(posLoc);
		    if(attachment == ref->m_attachmentType)
			   return 1;
		}
		//cannot find fire que and therefore
		return 4;
	 }
	 return 0;//cant find weapons
}
//------------------------------------------------------------------------------------//
float CDamageSystems::GetFinalDamageTweened(float baseDamage,
											int perfective,
											int profficiency,
											int damageRangeBaseMinPerc,
											int damageRangeBaseMaxPerc)
{
    int maxPerc = damageRangeBaseMaxPerc + profficiency;
    int negPerc = damageRangeBaseMinPerc + perfective;

	if(maxPerc == 0 && negPerc == 0)
	   return baseDamage;

    int range = maxPerc - negPerc;
	if(range == 0)
	   return baseDamage;

    float resultPercentage = (float)(rand()%range)+negPerc;

	return ((resultPercentage*.01f)*baseDamage)+baseDamage;
}

//------------------------------------------------------------------------------------//
int CDamageSystems::WoundChannelCalculation(CPlayerObject *playerObjectData, int damageAmount, int damageChannel)
{
	//Check if its an AI, if it is wound channels do not apply
	if(playerObjectData->m_aiControlled)
		return damageAmount;

	//Don't apply healing hits into the wound channel
	if(damageAmount <= 0)
		return damageAmount;

	//If player has maxxed on wound channel, send damage to kill player
	if(playerObjectData->m_woundChannel[damageChannel] == 100)
		return 30000;
	
	//Setup for a new calculation
	int newDamageAmount = damageAmount;

	//For every 10 points in the wound channel, increase the damage by 5%
	newDamageAmount += (int)(newDamageAmount * ((playerObjectData->m_woundChannel[damageChannel] * 0.1) * 0.05));

	//Increase the woundChannel amount by 3% of the original damage
	int amount = (int)damageAmount * 0.03;
	if(amount > 100)
		amount = 100;
	
	playerObjectData->m_woundChannel[damageChannel] += (unsigned char)amount;

	//Make sure we set the woundChannel to max if it goes over
	if(playerObjectData->m_woundChannel[damageChannel] > 100)
		playerObjectData->m_woundChannel[damageChannel] = 100;

	//Return our new amount
	return newDamageAmount;
}

int CDamageSystems::AddInPowerupResist(CPlayerObject *playerObjectData, int element, int armourRating)
{
	if(element == 0 && (playerObjectData->m_bonus[2].m_bonusType == 6 || playerObjectData->m_bonus[2].m_bonusType == 14))
		armourRating += playerObjectData->m_bonus[2].m_bonusAmount;
	else if(element == 1 && (playerObjectData->m_bonus[2].m_bonusType == 7 || playerObjectData->m_bonus[2].m_bonusType == 14))
		armourRating += playerObjectData->m_bonus[2].m_bonusAmount;
	else if(element == 2 && (playerObjectData->m_bonus[2].m_bonusType == 8 || playerObjectData->m_bonus[2].m_bonusType == 14))
		armourRating += playerObjectData->m_bonus[2].m_bonusAmount;
	else if(element == 3 && (playerObjectData->m_bonus[2].m_bonusType == 9 || playerObjectData->m_bonus[2].m_bonusType == 14))
		armourRating += playerObjectData->m_bonus[2].m_bonusAmount;
	else if(element == 4 && (playerObjectData->m_bonus[2].m_bonusType == 10 || playerObjectData->m_bonus[2].m_bonusType == 14))
		armourRating += playerObjectData->m_bonus[2].m_bonusAmount;
	else if(element == 5 && (playerObjectData->m_bonus[2].m_bonusType == 11 || playerObjectData->m_bonus[2].m_bonusType == 14))
		armourRating += playerObjectData->m_bonus[2].m_bonusAmount;
	else if(element == 6 && (playerObjectData->m_bonus[2].m_bonusType == 12 || playerObjectData->m_bonus[2].m_bonusType == 14))
		armourRating += playerObjectData->m_bonus[2].m_bonusAmount;
	else if(element == 7 && (playerObjectData->m_bonus[2].m_bonusType == 13 || playerObjectData->m_bonus[2].m_bonusType == 14))
		armourRating += playerObjectData->m_bonus[2].m_bonusAmount;

	if(armourRating > 95)
		armourRating = 95;

	return armourRating;
}

BOOL CDamageSystems::EADamageCalculation(float & damageAmount, LPHITPLAYER_DATA formatedMessage, int msgSize, DPID idFromLoc, int curSimutanius, CPlayerObject *playerObjectData,int & scIncre)
{
	//Get bottom line damage
	DWORD currentTime = GetTickCount();
	BOOL foundDamageValue	= FALSE;
	damageAmount			= 0.0f;
	int damageChannel		= 0;
	int skillTypeUsed		= -1;
	int specifiedDamage		= -1;
	BOOL filter				= 0;
	float damageMult		= 1.0f;

	CInventoryObj * weaponLink1 = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
	int damageOvrd,elementOvrd,fireQueOvrd = 0;
	int testValue = 0;
	if(weaponLink1)
	{
		
	   m_sharedInterfaceRef->m_itemManager->GetItemDynOvrds_safe(weaponLink1->m_itemData->m_globalID,&damageOvrd,&elementOvrd,&fireQueOvrd);

	   if(weaponLink1->m_itemData->m_hp == 1)
	   {
		  m_sharedInterfaceRef->SendTextMsg(idFromLoc,weaponLink1->m_itemData->m_itemName + " is broken, cannot attack with it!",playerObjectData);

		  return FALSE;
	   }


	   CAttachmentRefObj * ref;
	   for(POSITION posLoc = weaponLink1->m_itemData->m_attachmentRefList->GetHeadPosition();posLoc != NULL;)
			ref = (CAttachmentRefObj *)weaponLink1->m_itemData->m_attachmentRefList->GetNext(posLoc);

	   if(ref)
	   {
			CArmedInventoryObj * visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(ref->m_attachmentType);
			if(visObj)
			{//valid attachment reference
				if(visObj->m_thirdPersonSystem)
				{
					if(visObj->m_thirdPersonSystem->m_damageSphereList)
						for(POSITION dPos = visObj->m_thirdPersonSystem->m_damageSphereList->GetHeadPosition();dPos != NULL;)
						{//damage paths
						  CDamagePathObj* damagePathObj = (CDamagePathObj*)visObj->m_thirdPersonSystem->m_damageSphereList->GetNext(dPos);
						  //(removed: 'testValue = m_damageMinValue = rand...' overwrote this weapon's minimum damage
						  // in shared memory on every attack, scrambling melee damage; testValue isn't used here)
						  (void)damagePathObj;
						}
				}
			}
	   }
	    
	   if(formatedMessage->m_type == 1)
	   {//mele id (appendage type)
			 if(m_sharedInterfaceRef->m_itemManager->m_visuals)
			 {//valid attachment db
				int rResX = AIXVerifyAttachment(playerObjectData, (int)formatedMessage->m_miscValue);	
				if(rResX != 1)
				{//exploit
					m_sharedInterfaceRef->m_eventLog->LogByPriority(" -Weapon MELEE Exploit",(DWORD)formatedMessage->m_miscValue,3);
					filter = 1;
				}//end exploit

				CArmedInventoryObj * visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(formatedMessage->m_miscValue);
				if(visObj)
				{//valid attachment reference
				   if(visObj->m_thirdPersonSystem)
				   if(visObj->m_thirdPersonSystem->m_damageSphereList)
				   for(POSITION dPos = visObj->m_thirdPersonSystem->m_damageSphereList->GetHeadPosition();dPos != NULL;)
				   {//damage paths
					  CDamagePathObj* damagePathObj = (CDamagePathObj*)visObj->m_thirdPersonSystem->m_damageSphereList->GetNext(dPos);
					  damageAmount = (float)damagePathObj->m_damageMinValue;
					  if((damagePathObj->m_damageMaxValue - damagePathObj->m_damageMinValue) > 0)//fixed-damage weapons (min == max) crashed the server
						  damageAmount += (rand() % (damagePathObj->m_damageMaxValue - damagePathObj->m_damageMinValue));
					  foundDamageValue = TRUE;
					  damageChannel = damagePathObj->m_damageChannel;
					  skillTypeUsed = damagePathObj->m_criteriaForSkillUse;
					  specifiedDamage = damagePathObj->m_damageSpecific;
					  scIncre = 8;
				   }//end damage paths
				}//end valid attachment reference
			 }//end valid attachment db

			 //HANDLE WEAR
			 if(weaponLink1)
			 if(weaponLink1->ApplyWear(1))
			 {//somthing in inventory just broke
				m_sharedInterfaceRef->SendTextMsg(idFromLoc,
												  weaponLink1->m_itemData->m_itemName + " just broke!!",		
												  playerObjectData);
				if(weaponLink1->m_itemData->m_destroyOnBreak)
				{//remove item from inventory and broadcast inv change (disarm)
					m_sharedInterfaceRef->BroadcastInventoryChange(playerObjectData,-weaponLink1->m_itemData->m_globalID,0,TRUE,weaponLink1->m_itemData);
					playerObjectData->m_currentInventory->DeleteByGLIDBroken(weaponLink1->m_itemData->m_globalID,1);
					return FALSE;
				}
			 }//end somthing in inventory just broke
		}//end mele id (appendage type)
	}

	if(formatedMessage->m_type == 0)
	{//missile type

	  int rResX = AIXVerifyWeaponProjectile(playerObjectData, (int)formatedMessage->m_miscValue, fireQueOvrd);	
	  if(rResX != 1)
	  {//exploit
		 m_sharedInterfaceRef->m_eventLog->LogByPriority(" -Weapon Exploit",(DWORD)formatedMessage->m_miscValue,3);
		 filter = 1;
	  }//end exploit
		CMissileObj * misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(formatedMessage->m_miscValue);//missile index
		if(misPtr)
		{//valid
			damageAmount = (float)misPtr->m_minDamageValue;
			if((misPtr->m_maxDamageValue - misPtr->m_minDamageValue) > 0)
				damageAmount = damageAmount + (rand() % (misPtr->m_maxDamageValue - misPtr->m_minDamageValue));
		
			foundDamageValue = TRUE;
			if(misPtr->m_damageChannel > -1)
				damageChannel = misPtr->m_damageChannel;
			if(misPtr->m_damageChannelList)
			{
				POSITION damageChannelPos = misPtr->m_damageChannelList->GetHeadPosition();
				CElementStatusObj *elementStatusPtr = NULL;
				if(damageChannelPos)
					elementStatusPtr = (CElementStatusObj*)misPtr->m_damageChannelList->GetNext(damageChannelPos);
				if(elementStatusPtr)
					damageChannel = elementStatusPtr->m_elementIndex;
			}
			
			skillTypeUsed = misPtr->m_criteriaForSkillUse;
			specifiedDamage = misPtr->m_damageSpecific;
			scIncre = ((int)misPtr->m_collisionRadius+5);
		}//end valid
	}//end missile type
	else if(formatedMessage->m_type == 2)
	{//explosion index
	  int rResX = AIXVerifyWeaponExplosion(playerObjectData, (int)formatedMessage->m_miscValue);	
	  if(rResX != 1)
	  {//exploit
		 m_sharedInterfaceRef->m_eventLog->AddEvent(" -Weapon EXP Exploit",(DWORD)formatedMessage->m_miscValue);
		 filter = 1;
	  }//end exploit
	  CExplosionObj* expPtr = m_sharedInterfaceRef->m_explosionDBList->GetPtr(formatedMessage->m_miscValue);
	  if(expPtr)
	  {
		 damageAmount     = (float)expPtr->m_radiusDamage;
		 foundDamageValue = TRUE;
		 damageChannel    = expPtr->m_damageChannel;
		 specifiedDamage  = expPtr->m_damageSpecific;
		 skillTypeUsed    = expPtr->m_criteriaForSkillUse;
		 playerObjectData->m_timeStampOfLastAttack = 0; //Added so explosions will bypass the damage timer limit
		 scIncre = ((int)expPtr->m_damageRadius+6);
	  }
	}//end explosion index

	CPlayerObject * victimObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->m_playerThatGotHitID);//aquire player that was damaged
	if(victimObjectData == NULL)
	{
	   m_sharedInterfaceRef->m_eventLog->AddEvent("Victim is not on server anymore  Attacker net ID:",(DWORD)formatedMessage->m_playerThatGotHitID);
	   return FALSE;
	}

	if(filter == 1)
	   return FALSE;

	
	//dynamic overrides
	
	if(weaponLink1)
	{
		if(damageOvrd > 0)
		{
			damageAmount = damageOvrd;
			//m_sharedInterfaceRef->m_eventLog->LogByPriority("***Damage Ovrd:",(DWORD)damageOvrd,2);
		}
		if(elementOvrd > -1)
		{
			//m_sharedInterfaceRef->m_eventLog->LogByPriority("***Damage Element Ovrd:",(DWORD)elementOvrd,2);
			damageChannel = elementOvrd;
		}
	}
	//end dynamic overrides


	if(!playerObjectData->m_aiControlled)
	if((GetTickCount()-playerObjectData->m_timeLastUpdated) > m_sharedInterfaceRef->m_netComObject->m_lagLimitOnSceneEffect)
	{//cannot lag and kill
	 
	   return FALSE;
	}//cannot lag and kill
	//aquire player object

	if((currentTime - playerObjectData->m_timeStampOfLastAttack) < 200)//todo get a max fire rate from apx
	{//hit is too fast ingnor it
		return FALSE;
	}//end hit is too fast ingnor it


	//in area code (vicinity verification)
	if(!playerObjectData->m_aiControlled)
	{
	  DWORD interpVar = playerObjectData->m_interpolationVar;
	  if(interpVar > 1200)
		 interpVar = 1200;

	  D3DVECTOR latentPos = victimObjectData->GetDatedPosition(interpVar);
	  int vTolerance = (int)(((float)m_sharedInterfaceRef->m_netComObject->m_vicinityTolerance) + (m_sharedInterfaceRef->m_matLib.MagnitudeSquared(victimObjectData->m_Last3dPosition.x,victimObjectData->m_Last3dPosition.y,victimObjectData->m_Last3dPosition.z,victimObjectData->m_Target3dPosition.x,victimObjectData->m_Target3dPosition.y,victimObjectData->m_Target3dPosition.z)*.6f));
	  
	  if(victimObjectData->m_aiControlled)
		 vTolerance = vTolerance * 2;

	  BOOL vResult = m_sharedInterfaceRef->m_cabLib.BoundingSphereCheck((float)vTolerance,
												  5.0f,
												  latentPos.x,
												  latentPos.y,
												  latentPos.z,
												  (float)formatedMessage->m_targetX,
												  (float)formatedMessage->m_targetY,
												  (float)formatedMessage->m_targetZ);
	  if(!vResult)
	  {
		 return FALSE;
	  }
	}
	//end area code

	if(!playerObjectData->m_admin)//admins not applicable
	   if(playerObjectData->m_gm == TRUE || victimObjectData->m_gm == TRUE)//this makes gms invunerable and unable to deliver damage
		  return FALSE;

	if(victimObjectData->m_invunerableTemp)
	{//somone is attacking somone invunerable
	  if(!victimObjectData->Vulnerable())
	  {
		  if(!victimObjectData->m_aiControlled)
			 m_sharedInterfaceRef->SendGeneralMessageHandler(80,playerObjectData);
		  return FALSE;//filter damage for now
	  }
	}//end somone is attacking somone invunerable

	if(playerObjectData->m_invunerableTemp)
	{//attacker is attempting tol do damage while invunerable
	  if(!playerObjectData->Vulnerable())
	  {
		  if(!playerObjectData->m_aiControlled)
			 m_sharedInterfaceRef->SendGeneralMessageHandler(87,playerObjectData);
		  return FALSE;//filter damage for now
	  }
	}//end attacker is attempting tol do damage while invunerable

	victimObjectData->m_networkIDLastAttacker = playerObjectData->m_networkAssignedID;
	victimObjectData->m_timeStampOfLastAttack = currentTime;
	playerObjectData->m_timeStampOfLastAttack = currentTime;

	if(playerObjectData->m_energy < 1)
	{//cannot do damage while dead
	  return FALSE;
	}//end cannot do damage while dead

	if(damageAmount > 0)//only valid for damage hits
	{//we have damage
		if(!m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(victimObjectData->m_stencilChannel,victimObjectData->m_currentWorldID,WORLDRULE_NOLEVELRESTRICTIONS))
		{//no level restrictions
			if(playerObjectData->m_aiControlled == FALSE)
			{//player not AI
				if(victimObjectData->m_aiControlled == FALSE)
				{//level restrictions
					if(playerObjectData->m_currentHighestLevel < m_sharedInterfaceRef->m_netComObject->m_pkLevelRestriction)
					{
						  m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
														   playerObjectData->m_currentID,
														  "You must be at least level "+m_sharedInterfaceRef->m_cabLib.ConvToString(m_sharedInterfaceRef->m_netComObject->m_pkLevelRestriction)+" to attack this player.",
														   0,
														   playerObjectData,
														   3);
						  return FALSE;
					}
					
					if(victimObjectData->m_currentHighestLevel < m_sharedInterfaceRef->m_netComObject->m_pkLevelRestriction)
					{
						  m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
														   playerObjectData->m_currentID,
														  "You cannot attack a player that is not level "+m_sharedInterfaceRef->m_cabLib.ConvToString(m_sharedInterfaceRef->m_netComObject->m_pkLevelRestriction)+".",
														   0,
														   playerObjectData,
														   3);
						  return FALSE;
					}
				}//end level restrictions
			}//end player not AI
		}//end no level restrictions
	}//end we haev damage

	//verify ammo
	int damageModifier = 0;
	if(weaponLink1)
	{//aquire weapon used
	  //get damage modifier
	  damageModifier = (int)weaponLink1->m_itemData->m_damageModifier;

	  //calc ammo
	  if(weaponLink1->m_itemData->m_maxCharges != 0)
	  {//does require ammo of some sort
		 if(weaponLink1->m_itemData->m_chargeCurrent <= 0)
		 {//1a
	        
//			return FALSE;
		 }//end 1a
	  }//end does require ammo of some sort
	}//end aquire weapon used

	if(victimObjectData->m_energy < 1)
	{//shooting dead bodies has no effect
	   if(msgSize > 0)
	   {
			return FALSE;
	   }	
	   return FALSE; 
	}//end shooting dead bodies has no effect

	if(!foundDamageValue)
	{//failed aquire damage value
//	   m_sharedInterfaceRef->m_eventLog->AddEvent("Unable to aquire correct damage db index :",accountTrackPtr->m_accountName);
	   m_sharedInterfaceRef->m_eventLog->AddEvent("value :",((float)formatedMessage->m_miscValue));
	   m_sharedInterfaceRef->m_eventLog->AddEvent("type :",((float)formatedMessage->m_type));
	   if(msgSize > 0)
	   {
	    
	   }	
	   return FALSE;
	}//end failed aquire damage value	   
	//EVALUATE SAFE ZONES

	BOOL evaluateSafezone = TRUE;
	if(victimObjectData->m_noteriety)
	  if(victimObjectData->m_noteriety->m_currentMurders >= m_sharedInterfaceRef->m_netComObject->m_murderCountMax)
		 evaluateSafezone = FALSE;


	if(playerObjectData->m_admin)
	  evaluateSafezone = FALSE;

	if(!playerObjectData->m_aiControlled)
		if(damageAmount > 0)
			if(curSimutanius > 3)//ignore (EXPLOIT KILLER)
			   return FALSE;

	curSimutanius++;

	if(damageAmount > 0)
	{//only if a damage message you should eval safe area
		if(!playerObjectData->m_aiControlled)
		{//pc attacker
			if(!victimObjectData->m_aiControlled)
			{//pc target
				if(playerObjectData->m_networkAssignedID != victimObjectData->m_networkAssignedID)
				{//not same ID
					if(evaluateSafezone)
					{//do safe zone 
						if(victimObjectData->m_inSafeZone)
						{//damage in safe zone- a crime
							   //inform attacker of 
							   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
															  playerObjectData->m_currentID,
															  "Weapon ineffective in protected area",
															   0,
															   playerObjectData,
															   0);
							   return FALSE;
						}//end damage in safe zone- a crime
						else
						{//victim isnt in safe zone maybe attacker is
							 if(playerObjectData->m_inSafeZone)
							 {
								m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
															   playerObjectData->m_currentID,
															  "Weapon ineffective in protected area",
															   0,
															   playerObjectData,
															   0);
								return FALSE;
							 }
						}//end victim isnt in safe zone maybe attacker is
					}//end do safe zone
				}//end not same ID
			}//end non AI target
		}//end non AI attacker
	}//end valid damage amount

	//Add faction bonus
	if((m_sharedInterfaceRef->m_pvpFactionBonus > -1) && (playerObjectData->m_factionStanding == m_sharedInterfaceRef->m_pvpFactionBonus))
		damageAmount += 10;

	//factor in armour rating
	//float armourRating = (float)victimObjectData->m_currentInventory->GetArmourRating(damageChannel,FALSE,specifiedDamage);

	//factor in armour rating and compound armor wear
	float armourRating = 0;
	POSITION ePosLast;
	int wearAmnt = 0;
	for(POSITION eLoc = victimObjectData->m_currentInventory->GetHeadPosition(); (ePosLast = eLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)victimObjectData->m_currentInventory->GetNext(eLoc);

		if(	!invObj->m_itemData || !invObj->m_itemData->m_elementDefenses || !invObj->m_armed || invObj->m_itemData->m_hp == 1)
			continue;

		if(	invObj->m_itemData->m_defensesSpecific != -1)
			continue;

		if(	invObj->m_itemData->m_defensesSpecific != -1)
			continue;
		
		float def = 0.0f;
		for(POSITION defPos = invObj->m_itemData->m_elementDefenses->GetHeadPosition(); defPos != NULL;)
		{
			CElementStatusObj *elementStatusPtr = (CElementStatusObj*)invObj->m_itemData->m_elementDefenses->GetNext(defPos);
			if(damageChannel == elementStatusPtr->m_elementIndex)
			{
				armourRating += elementStatusPtr->m_currentValue;
				def = 1.0f - (((float)elementStatusPtr->m_currentValue) * .01f);
				wearAmnt = def * damageAmount;
				break;
			}
		}

		//HANDLE WEAR
		if(wearAmnt > 0)
		if(invObj->ApplyWear(wearAmnt))
		{//somthing in inventory just broke
			m_sharedInterfaceRef->SendTextMsg(idFromLoc,
					                          invObj->m_itemData->m_itemName + " just broke!",		
											  victimObjectData);
			if(invObj->m_itemData->m_destroyOnBreak)
			{//remove item from inventory and broadcast inv change (disarm)
				m_sharedInterfaceRef->BroadcastInventoryChange(victimObjectData,-invObj->m_itemData->m_globalID,0,TRUE,invObj->m_itemData);
				invObj->SafeDelete();
				delete invObj;
				invObj = 0;
				victimObjectData->m_currentInventory->RemoveAt(ePosLast);
			}
		}//end somthing in inventory just broke
	}



	///////////////////////////////
	// 03/24/2006 update         //
	// Check for Reistance Bonus //
	///////////////////////////////

	if(damageChannel == 0 && (victimObjectData->m_bonus[2].m_bonusType == 6 || victimObjectData->m_bonus[2].m_bonusType == 14))
	 armourRating += victimObjectData->m_bonus[2].m_bonusAmount;
	else if(damageChannel == 1 && (victimObjectData->m_bonus[2].m_bonusType == 7 || victimObjectData->m_bonus[2].m_bonusType == 14))
	 armourRating += victimObjectData->m_bonus[2].m_bonusAmount;
	else if(damageChannel == 2 && (victimObjectData->m_bonus[2].m_bonusType == 8 || victimObjectData->m_bonus[2].m_bonusType == 14))
	 armourRating += victimObjectData->m_bonus[2].m_bonusAmount;
	else if(damageChannel == 3 && (victimObjectData->m_bonus[2].m_bonusType == 9 || victimObjectData->m_bonus[2].m_bonusType == 14))
	 armourRating += victimObjectData->m_bonus[2].m_bonusAmount;
	else if(damageChannel == 4 && (victimObjectData->m_bonus[2].m_bonusType == 10 || victimObjectData->m_bonus[2].m_bonusType == 14))
	 armourRating += victimObjectData->m_bonus[2].m_bonusAmount;
	else if(damageChannel == 5 && (victimObjectData->m_bonus[2].m_bonusType == 11 || victimObjectData->m_bonus[2].m_bonusType == 14))
	 armourRating += victimObjectData->m_bonus[2].m_bonusAmount;
	else if(damageChannel == 6 && (victimObjectData->m_bonus[2].m_bonusType == 12 || victimObjectData->m_bonus[2].m_bonusType == 14))
	 armourRating += victimObjectData->m_bonus[2].m_bonusAmount;
	else if(damageChannel == 7 && (victimObjectData->m_bonus[2].m_bonusType == 13 || victimObjectData->m_bonus[2].m_bonusType == 14))
	 armourRating += victimObjectData->m_bonus[2].m_bonusAmount;

	//Emblem Resist Bonus
	armourRating = victimObjectData->m_currentInventory->GetEmblemResistBonus(armourRating);
	
	if(armourRating > 95)
	 armourRating = 95;

	////////////////
	// End Update //
	////////////////

	armourRating = 1.0f - (armourRating * .01f);
	if(armourRating <= 0.0f)
	{//error
		armourRating = 0.0f;
		m_sharedInterfaceRef->m_eventLog->AddEvent("Players armour class is bad,possible cheat:",victimObjectData->m_charName,idFromLoc);
	}//end error

	if(damageAmount > 0)
	{//damageing hit
		//10/31/02
		int perfective = 0;
		int profficiency = 0;
		if(!playerObjectData->m_aiControlled)
		{
			m_sharedInterfaceRef->m_skillDatabase->GetEffeciencyByTypeAndLevel(skillTypeUsed,playerObjectData->m_skills,&profficiency,&perfective);
			//m_sharedInterfaceRef->m_eventLog->AddEvent(profficiency);
			//m_sharedInterfaceRef->m_eventLog->AddEvent(perfective);
		}
		else
		{
			perfective = 10;
			profficiency = 10;
		}
		damageAmount = GetFinalDamageTweened(damageAmount+damageModifier,
											 perfective,//int perfective,
											 profficiency,//int profficiency,
											 m_sharedInterfaceRef->m_netComObject->m_dmgRngBaseMinPerc,
											 m_sharedInterfaceRef->m_netComObject->m_dmgRngBaseMaxPerc);

		////////////////////////////////
		// Check for Damage Modifiers //
		////////////////////////////////
		//Bonus Damage
		/*if(playerObjectData->m_bonus[0].m_bonusType == 5)
		{
			// If we have a damage modifier, apply it
			damageAmount *= playerObjectData->m_bonus[0].m_damageMult;
		}*/
	}

	if(damageAmount > 10000)
		damageAmount = 10000;

	//Emblem Damage Reduction
	if(!victimObjectData->m_aiControlled)
	{
		//Make sure we don't get a healing bonus with damage reduction
		if(damageAmount > 0)
		{
			int reduction = victimObjectData->m_currentInventory->GetEmblemReductionBonus();
			//Faction bonus
			if((m_sharedInterfaceRef->m_clanFactionBonus > -1) && (m_sharedInterfaceRef->m_clanFactionBonus == victimObjectData->m_factionStanding))
				reduction += 5;
			damageAmount -= reduction;
			if(damageAmount < 0)
				damageAmount = 0;
		}
	}
		  
	damageAmount = (((float)damageAmount)*armourRating);//damage amount
	  
	return TRUE;
}

float CDamageSystems::ChronicleDamageCalculation(LPHITPLAYER_DATA formatedMessage, int msgSize, DPID idFromLoc, int curSimutanius, CPlayerObject *playerObjectData)
{
	DWORD currentTime = GetTickCount();
	BOOL foundDamageValue	= FALSE;
	float damageAmount		= 0.0f;
	int damageChannel		= 0;
	int skillTypeUsed		= -1;
	int specifiedDamage		= -1;
	int scIncre				= 0;
	BOOL filter				= 0;
	CPlayerObject * victimObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->m_playerThatGotHitID);//aquire player that was damaged
	
	//////////////////////////////////
	// Chronicle Damage Calculation //
	//////////////////////////////////

	////////////////////////
	// Damage Acquisition //
	////////////////////////

	// 1.  Aquire the Base Damage from the weapon in question
	float newDamageAmount = 0.0f;
	damageChannel = -1;
	CInventoryObj * weaponLink1 = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
	int testValue = 0;
	if(weaponLink1)
	{
		CAttachmentRefObj *ref;
		for(POSITION posLoc = weaponLink1->m_itemData->m_attachmentRefList->GetHeadPosition();posLoc != NULL;)
			ref = (CAttachmentRefObj *)weaponLink1->m_itemData->m_attachmentRefList->GetNext(posLoc);

		if(ref)
		{
			CArmedInventoryObj * visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(ref->m_attachmentType);
			if(visObj)
			{//valid attachment reference
				if(visObj->m_thirdPersonSystem)
				if(visObj->m_thirdPersonSystem->m_damageSphereList)
				for(POSITION dPos = visObj->m_thirdPersonSystem->m_damageSphereList->GetHeadPosition();dPos != NULL;)
				{//damage paths
					CDamagePathObj* damagePathObj = (CDamagePathObj*)visObj->m_thirdPersonSystem->m_damageSphereList->GetNext(dPos);
					testValue = damagePathObj->m_damageMinValue;//was 'testValue = m_damageMinValue = rand...' - overwrote the weapon's minimum
					if((damagePathObj->m_damageMaxValue - damagePathObj->m_damageMinValue) > 0)
						testValue += (rand() % (damagePathObj->m_damageMaxValue - damagePathObj->m_damageMinValue));
					foundDamageValue = TRUE;
					damageChannel = damagePathObj->m_damageChannel;
					skillTypeUsed = damagePathObj->m_criteriaForSkillUse;
					specifiedDamage = damagePathObj->m_damageSpecific;
					scIncre = 8;
				}
			}
		}
	}

	newDamageAmount += (float)testValue;

	// 2.  Add that Base Damage now to a range from Skill Damage (if applicable) (TODO)
	if(formatedMessage->m_type == 0)
	{//missile type
		CMissileObj * misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(formatedMessage->m_miscValue);//missile index
		if(misPtr)
		{//valid
			newDamageAmount += (float)misPtr->m_minDamageValue;
			if((misPtr->m_maxDamageValue - misPtr->m_minDamageValue) > 0)//min == max crashed the server
				newDamageAmount += (rand() % (misPtr->m_maxDamageValue - misPtr->m_minDamageValue));
			foundDamageValue = TRUE;
			if(damageChannel == -1)
				damageChannel = misPtr->m_damageChannel;
			skillTypeUsed = misPtr->m_criteriaForSkillUse;
			specifiedDamage = misPtr->m_damageSpecific;
			scIncre = ((int)misPtr->m_collisionRadius+5);
		}//end valid
	}//end missile type

	else if(formatedMessage->m_type == 2)
	{//explosion index
		CExplosionObj* expPtr = m_sharedInterfaceRef->m_explosionDBList->GetPtr(formatedMessage->m_miscValue);
		if(expPtr)
		{
			newDamageAmount  += (float)expPtr->m_radiusDamage;
			foundDamageValue = TRUE;
			if(damageChannel == -1)
				damageChannel    = expPtr->m_damageChannel;
			specifiedDamage  = expPtr->m_damageSpecific;
			skillTypeUsed    = expPtr->m_criteriaForSkillUse;
			scIncre = ((int)expPtr->m_damageRadius+6);
		}
	}//end explosion index

	// 3.  Add that Base Damage to a rand() % Modifier where Modifier is the level of your skill + 1
	float skillMult = 0.5f;
	if(!playerObjectData->m_aiControlled)
	{
		CSkillObject *skill = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(skillTypeUsed);
		CSkillObject *playerSkill = playerObjectData->m_skills->GetSkillByType(skillTypeUsed);
	  
		if(playerSkill)
		{
			int level = 1;
			playerSkill->EvaluateLevel(level, skill);
			if(level > 0)//untrained skill (level 0) crashed the server
				newDamageAmount += rand() % level + 1;
			skillMult = (((float)level) * 0.05f) + 1.0f;
			if(skillTypeUsed == 20)
				newDamageAmount -= testValue;
		}
	}

	// 4.  Add in damage based off of the stat that goes alongside the weapon
	//		if the amount from the stat increase is greater then 50% of the stat amount, cap it at 50%
	if(playerObjectData->m_stasis == 0 && !playerObjectData->m_aiControlled)
	{
		int ability = 0;
		if(weaponLink1)
			if(weaponLink1->m_itemData)
				ability = weaponLink1->m_itemData->m_abilityType;
		if(skillTypeUsed == 20)
			ability = 3;
		if(ability)
		{
			POSITION posLoc = playerObjectData->m_stats->FindIndex(ability - 1);
			if(posLoc)
			{
				CStatObj *statPtr = (CStatObj *) playerObjectData->m_stats->GetAt(posLoc);
				float statDamage = 0.0f;
				int statAmount = (int)statPtr->GetValueIncludingBonusItems(playerObjectData->m_currentInventory);
				statAmount = (float)statAmount * 0.5f;

				if(statAmount > 0)//a stat of 0 or 1 halves to 0 - crashed the server
					statDamage = (rand() % statAmount);

				newDamageAmount += statDamage;
			}
		}
	}
	else
	{
		int randomAmount = rand() % 10;
		newDamageAmount += randomAmount;
	}

	// 5.  Multiply all this by  1 + 0.05 for every level of base skill.
	if(!playerObjectData->m_aiControlled)
		newDamageAmount *= skillMult;

	// Lastly, add on powerups
	if(playerObjectData->m_bonus[0].m_bonusType == 5)
	{
		// If we have a damage modifier, apply it
		newDamageAmount *= playerObjectData->m_bonus[0].m_damageMult;
		// Must be a melee attack
		if(formatedMessage->m_type == 1)
		{
			// Decrement the counter
			playerObjectData->m_bonus[0].m_bonusDuration--;
		  
			// Send update for visual icon display icon side
			m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
												  playerObjectData->m_currentID,
												  30,												
												  playerObjectData->m_bonus[0].m_bonusDuration,	
												  playerObjectData->m_bonus[0].m_bonusType,
												  0,
												  1,
												  playerObjectData);

			// If we have reached 0, time to nullify powerup
			if(playerObjectData->m_bonus[0].m_bonusDuration == 0)
			{
				playerObjectData->m_bonus[0].m_bonusAmount = 0;
				playerObjectData->m_bonus[0].m_bonusDuration = 0;
				playerObjectData->m_bonus[0].m_bonusType = 0;
				playerObjectData->m_bonus[0].m_damageMult = 0;
				playerObjectData->m_bonus[0].m_maxDuration = 0;

				playerObjectData->m_DamageModifier = 0;
				playerObjectData->m_DamageChannelOverride = -1;
			}
		}
	}

	// Update Damage Channel here if needed (greater than -1)
	if(playerObjectData->m_DamageChannelOverride > -1)
		damageChannel = playerObjectData->m_DamageChannelOverride;

	//////////////////////
	// Damage Reduction //
	//////////////////////

	// 1.  Subtract the Armour Value (AC) of all armour pieces from overall damage (MAX 75 ATM)
	int armourClass = 0;
	armourClass = (int)victimObjectData->m_currentInventory->GetArmourClass(FALSE);
	newDamageAmount -= armourClass;
		  
	// 2.  Multiply the amount left by Resistance of damage channel, which cannot be greater than 90 currently
	float armourRating = (float)victimObjectData->m_currentInventory->GetArmourRating(damageChannel,FALSE,specifiedDamage);
	armourRating = AddInPowerupResist(victimObjectData, damageChannel, armourRating);	   
	armourRating = 1.0f - (armourRating * .01f);
	if(armourRating <= 0.0f)
	{//error
		armourRating = 0.0f;
		m_sharedInterfaceRef->m_eventLog->AddEvent("Players armour class is bad,possible cheat:",victimObjectData->m_charName,idFromLoc);
	}//end error

	if(newDamageAmount > 30000)
		newDamageAmount = 30000;

	damageAmount = newDamageAmount;

	//Even in full armour, we have a chance to take some damage
	if(newDamageAmount < 0)
	{
		int chanceToDamage = rand() % 5 + 1;
		newDamageAmount = chanceToDamage;
	}
	////////////////////////////////////////
	// End Testing New Damage Calculation //
	////////////////////////////////////////

	return newDamageAmount;
}

float CDamageSystems::EATwoDamageCalculation(LPHITPLAYER_DATA formatedMessage, int msgSize, DPID idFromLoc, int curSimutanius, CPlayerObject *playerObjectData)
{
	DWORD currentTime = GetTickCount();
	BOOL foundDamageValue	= FALSE;
	float damageAmount		= 0.0f;
	int damageChannel		= 0;
	int skillTypeUsed		= -1;
	int specifiedDamage		= -1;
	int scIncre				= 0;
	BOOL filter				= 0;
	CPlayerObject * victimObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->m_playerThatGotHitID);//aquire player that was damaged
	////////////////////////////
	// EA2 Damage Calculation //
	////////////////////////////
	int overAllDamage = 0;
	if(m_sharedInterfaceRef->m_damageMode == 2)
	{
		  int baseDamage = 0;
		  int Ea2Channel = 0;
		  int channelAmount = 0;
		  BOOL addDot = FALSE;
		  CDotClass *dotToAdd = NULL;
		  
		  // 1. Aquire the base Damage from the weapon we used
		  //		damage = Minimum + (random % max - min)
		  if(formatedMessage->m_type == 0)
		  {//Missile Type
			  CMissileObj *misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(formatedMessage->m_miscValue);
			  if(misPtr)
			  {
				  int difference = misPtr->m_maxDamageValue - misPtr->m_minDamageValue;
				  if(difference)
					baseDamage += (float)misPtr->m_minDamageValue + (rand() % (difference));
				  else
					  baseDamage += (float)misPtr->m_minDamageValue;
				  foundDamageValue = TRUE;
				  scIncre = ((int)misPtr->m_collisionRadius + 5);
				  if(misPtr->m_damageChannelList)
				     channelAmount = misPtr->m_damageChannelList->GetCount();
				  specifiedDamage = misPtr->m_damageSpecific;
				  if(misPtr->m_dotRefValue > -1)
				  {
					  int axDotVerify = AIXVerifyDotAttachmentMissile(playerObjectData, misPtr->m_dotRefValue);
					  if(axDotVerify == 1)
					  {
						  dotToAdd = m_sharedInterfaceRef->m_dotDB->GetDotByID(misPtr->m_dotRefValue);
						  int addChance = rand() % 100;
						  if(dotToAdd)
						  {
							  if(addChance > dotToAdd->m_dotApplyPer)
								  dotToAdd = NULL;
						  }
					  }
				  }
			  }
		  }//End Missile Type
		  
		  else if(formatedMessage->m_type == 1)
		  {//Melee type
			  if(m_sharedInterfaceRef->m_itemManager->m_visuals)
			  {
				  int rResX = AIXVerifyAttachment(playerObjectData, (int)formatedMessage->m_miscValue);
				  if(rResX != 1)
				  {
					  m_sharedInterfaceRef->m_eventLog->LogByPriority(" -Weapon MELEE Exploit", (DWORD)formatedMessage->m_miscValue,3);
					  filter = 1;
				  }
				  CArmedInventoryObj *visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(formatedMessage->m_miscValue);
				  if(visObj)
				  {//valid
					  if(visObj->m_thirdPersonSystem)
					  {
						  if(visObj->m_thirdPersonSystem->m_damageSphereList)
						  {
							  for(POSITION dPos = visObj->m_thirdPersonSystem->m_damageSphereList->GetHeadPosition(); dPos != NULL;)
							  {//Acquire base damage elements
								  CDamagePathObj *damagePathObj = (CDamagePathObj *)visObj->m_thirdPersonSystem->m_damageSphereList->GetNext(dPos);
								  int difference2 = damagePathObj->m_damageMaxValue - damagePathObj->m_damageMinValue;
								  if(difference2)
									  baseDamage = (float)damagePathObj->m_damageMinValue + (rand() % (difference2));
								  else
									  baseDamage = (float)damagePathObj->m_damageMinValue;
								  foundDamageValue = TRUE;
								  Ea2Channel = damagePathObj->m_damageChannel;
								  specifiedDamage = damagePathObj->m_damageSpecific;
								  scIncre = 8;
								  channelAmount = 1;
								  if(damagePathObj->m_damageDotValue > -1)
								  {//Check for Dot Chance to add
									  int axDotVerify = AIXVerifyDotAttachmentMelee(playerObjectData, damagePathObj->m_damageDotValue);
									  if(axDotVerify == 1)
									  {
										  dotToAdd = m_sharedInterfaceRef->m_dotDB->GetDotByID(damagePathObj->m_damageDotValue);
										  int addChance = rand() % 100;
										  if(dotToAdd)
										  {
											  if(addChance > dotToAdd->m_dotApplyPer)
												  dotToAdd = NULL;
										  }
									  }
								  }//End Dot Chance to Add
							  }//End acquire base damage elements
						  }
					  }
				  }//End Valid
			  }
		  }//End Melee type
		  
		  else if(formatedMessage->m_type == 2)
		  {//Explosion Type
			  int rResX = AIXVerifyWeaponExplosion(playerObjectData, (int)formatedMessage->m_miscValue);
			  if(rResX != 1)
			  {//Exploit
				  m_sharedInterfaceRef->m_eventLog->AddEvent(" -Weapon EXP Exploit",(DWORD)formatedMessage->m_miscValue);
				  filter = 1;
			  }//End Exploit
			  CExplosionObj *expPtr = m_sharedInterfaceRef->m_explosionDBList->GetPtr(formatedMessage->m_miscValue);
			  if(expPtr)
			  {
				  baseDamage = (float)expPtr->m_radiusDamage;
				  foundDamageValue = TRUE;
				  Ea2Channel = expPtr->m_damageChannel;
				  specifiedDamage = expPtr->m_damageSpecific;
				  scIncre = ((int)expPtr->m_damageRadius + 6);
				  channelAmount = 1;
			  }
		  }//End Explosion Type

		  // 2. Aquire the offensive stat that is used for the attack
		  //		value = random % 50% of max stat
		  //		bonus to damage is (value * 1.5)
		  if(playerObjectData->m_stasis == 0 && !playerObjectData->m_aiControlled && playerObjectData->m_stats)
		  {
			  int ability = 0;
			  CInventoryObj * weaponLink1 = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
			  if(weaponLink1)
				  if(weaponLink1->m_itemData)
					ability = weaponLink1->m_itemData->m_abilityType;
			  if(ability)
			  {
				  POSITION posLoc = playerObjectData->m_stats->FindIndex(ability - 1);
				  if(posLoc)
				  {
					  CStatObj *statPtr = (CStatObj *) playerObjectData->m_stats->GetAt(posLoc);
					  float statDamage = 0.0f;
					  int statAmount = (int)statPtr->GetValueIncludingBonusItems(playerObjectData->m_currentInventory);
					  statAmount = (float)statAmount * 0.5f;
					  if(statAmount > 0)//a stat of 0 or 1 halves to 0 - crashed the server
						  statDamage = (rand() % statAmount);

					  overAllDamage = baseDamage + (statDamage * 1.5);
				  }
			  }
		  }
		  else
		  {
			  int randomAmount = rand() % 10;
			  overAllDamage = baseDamage + randomAmount;
		  }

		  // 3. Aquire the defensive stat (if available)
		  //		value = random % 50% of max stat
		  //		minus from damage is (value * 0.5)
		  //		if we go below 0, check to see if we should always do damage or not
		  if(victimObjectData->m_stasis == 0 && !victimObjectData->m_aiControlled && victimObjectData->m_stats)
		  {
			  int ability = 0;
			  int defAbility = 0;
			  //Base the adjustment off of the ability attachment to the weapon
			  CInventoryObj * weaponLink1 = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
			  if(weaponLink1)
				  if(weaponLink1->m_itemData)
					ability = weaponLink1->m_itemData->m_abilityType;
			  
			  //If Str, counter with Dex
			  if(ability == 1)
				  defAbility = 2;
			  //If Dex, counter with Str
			  if(ability == 2)
				  defAbility = 1;
			  //Otherwise use Wis
			  if(ability == 3)
				  defAbility = 3;
			  
			  if(defAbility)
			  {
				  POSITION posLoc = victimObjectData->m_stats->FindIndex(defAbility - 1);
				  if(posLoc)
				  {
					  CStatObj *statPtr = (CStatObj *) victimObjectData->m_stats->GetAt(posLoc);
					  float statDamage = 0.0f;
					  int statAmount = (int)statPtr->GetValueIncludingBonusItems(victimObjectData->m_currentInventory);
					  statAmount = (float)statAmount * 0.5f;
					  if(statAmount > 0)//a stat of 0 or 1 halves to 0 - crashed the server
						  statDamage = (rand() % statAmount);

					  overAllDamage -= (statDamage * 0.5);
					  baseDamage = overAllDamage;
				  }
			  }
		  }
		  else
		  {
			  int randomAmount = rand() % 10;
			  overAllDamage -= randomAmount;
			  baseDamage = overAllDamage;
		  }

		  // Steps 4 - 7 need to repeat for every channel
		  for(int i = channelAmount; i > 0; i--)
		  {
			  if(channelAmount > 1)
			  {
				  //If we are a missile Type, we need to divide the damage by percentage
				  if(formatedMessage->m_type == 0)
				  {
					  CMissileObj *misPtr = m_sharedInterfaceRef->m_missileDB->GetPtr(formatedMessage->m_miscValue);
					  int damage = 0;
					  if(misPtr)
					  {
						  BOOL noChannelOnObject = TRUE;
						  if(misPtr->m_damageChannelList)
						  for(POSITION posLoc = misPtr->m_damageChannelList->GetHeadPosition(); posLoc != NULL;)
						  {
							  // Get the damage value per channel
							  CElementStatusObj *eleObj = (CElementStatusObj *) misPtr->m_damageChannelList->GetNext(posLoc);
							  damage = (eleObj->m_currentValue * 0.01f) * baseDamage;
							  
							  // 4. Calculate the resistances that are in the channel
							  //		this is where we will need to retrieve the amount of damage channels the attack had and do
							  //			one for each channel
							  //		shields - 35%		armour - 50%		jetpack - 5%
							  float armourRating = (float)victimObjectData->m_currentInventory->GetArmourRating(eleObj->m_elementIndex, FALSE, specifiedDamage);
							  armourRating = AddInPowerupResist(victimObjectData, eleObj->m_elementIndex, (int)armourRating);
							  armourRating = 1.0f - (armourRating * .01f);
							  if(armourRating <= 0.0f)
							  {//error
								armourRating = 0.0f;
								m_sharedInterfaceRef->m_eventLog->AddEvent("Players armour class is bad,possible cheat:",victimObjectData->m_charName,idFromLoc);
							  }//end error
							  damage *= armourRating;

							  // 5. Increase Dot Dmg
							  //		static value that is saved in player object for damage increase in a channel
							  damage += victimObjectData->m_dotDamageTakeIncrease[eleObj->m_elementIndex];

							  // 6. Decrease Dot Dmg
							  //		max reduction of 50% from damage being done ( 1 - 50 )
							  if(playerObjectData->m_dotDamageReduction[eleObj->m_elementIndex] > 50)
								  playerObjectData->m_dotDamageReduction[eleObj->m_elementIndex] = 50;
							  damage -= (damage * (playerObjectData->m_dotDamageReduction[eleObj->m_elementIndex] * 0.01f));

							  // 7. Wound Channels
							  //		Calculate the amount of damage wounds will increase and apply wound channel damage
							  damage = WoundChannelCalculation(victimObjectData, damage, eleObj->m_elementIndex);

							  //Increase the damage
							  overAllDamage += damage;
							  noChannelOnObject = FALSE;
						  }
						  //Error Checking for element channel
						  if(noChannelOnObject)
						  {
							  float armourRating = (float)victimObjectData->m_currentInventory->GetArmourRating(0, FALSE, specifiedDamage);
							  armourRating = AddInPowerupResist(victimObjectData, 0, (int)armourRating);
							  armourRating = 1.0f - (armourRating * .01f);
							  if(armourRating <= 0.0f)
							  {//error
								armourRating = 0.0f;
								m_sharedInterfaceRef->m_eventLog->AddEvent("Players armour class is bad,possible cheat:",victimObjectData->m_charName,idFromLoc);
							  }//end error
							  damage *= armourRating;

							  // 5. Increase Dot Dmg
							  //		static value that is saved in player object for damage increase in a channel
							  damage += victimObjectData->m_dotDamageTakeIncrease[0];

							  // 6. Decrease Dot Dmg
							  //		max reduction of 50% from damage being done ( 1 - 50 )
							  if(playerObjectData->m_dotDamageReduction[0] > 50)
								  playerObjectData->m_dotDamageReduction[0] = 50;
							  damage -= (damage * (playerObjectData->m_dotDamageReduction[0] * 0.01f));

							  // 7. Wound Channels
							  //		Calculate the amount of damage wounds will increase and apply wound channel damage
							  damage = WoundChannelCalculation(victimObjectData, damage, 0);

							  //Increase the damage
							  overAllDamage += damage;
						  }
					  }
				  }
			  }

			  else
			  {
				  // 4. Calculate the resistances that are in the channel
				  //		this is where we will need to retrieve the amount of damage channels the attack had and do
				  //			one for each channel
				  //		shields - 35%		armour - 50%		jetpack - 5%
				  float armourRating = (float)victimObjectData->m_currentInventory->GetArmourRating(Ea2Channel, FALSE, specifiedDamage);
				  armourRating = (float)AddInPowerupResist(victimObjectData, Ea2Channel, (int)armourRating);
				  armourRating = 1.0f - (armourRating * .01f);
				  if(armourRating <= 0.0f)
				  {//error
					  armourRating = 0.0f;
					  m_sharedInterfaceRef->m_eventLog->AddEvent("Players armour class is bad,possible cheat:",victimObjectData->m_charName,idFromLoc);
				  }//end error
				  baseDamage = (int)(baseDamage * armourRating);//was baseDamage *= (int)armourRating, which rounded any armour down to 0 damage
				  
				  // 5. Increase Dot Dmg
				  //		static value that is saved in player object for damage increase in a channel
				  baseDamage += victimObjectData->m_dotDamageTakeIncrease[Ea2Channel];

				  // 6. Decrease Dot Dmg
				  //		max reduction of 50% from damage being done ( 1 - 50 )
				  if(playerObjectData->m_dotDamageReduction[Ea2Channel] > 50)
					  playerObjectData->m_dotDamageReduction[Ea2Channel] = 50;
				  baseDamage -= (int)(baseDamage * (playerObjectData->m_dotDamageReduction[Ea2Channel] * 0.01f));

				  // 7. Wound Channels
				  //		Calculate the amount of damage wounds will increase and apply wound channel damage
				  overAllDamage = WoundChannelCalculation(victimObjectData, baseDamage, Ea2Channel);
			  }
		  }

		  // 8. DoT adding
		  //		Calculate to see if a DoT has the chance of being attached base on the 
		  //			attack that was used
		  //		(DoTs go on the VICTIM - they used to land on the attacker - and each victim
		  //		 gets its own copy: the old code added the shared master from the DoT database,
		  //		 which was then counted down for everyone and deleted when it ran out)
		  if(dotToAdd && victimObjectData && victimObjectData->m_dotList
		  && victimObjectData->m_dotList->m_currentDotAmount <= victimObjectData->m_dotList->m_dotLimit)
		  {
			  CDotClass * dotCopy = NULL;
			  dotToAdd->Clone(&dotCopy);
			  dotCopy->m_dotPlayerID  = (short)playerObjectData->m_networkAssignedID;//who cast it
			  dotCopy->m_dotTimeStamp = GetTickCount();
			  victimObjectData->m_dotList->m_currentDotAmount++;
			  victimObjectData->m_dotList->AddTail(dotCopy);
			  if(dotCopy->m_dotType == DOTTYPE_AMP_DAMAGE)
				  victimObjectData->m_dotDamageTakeIncrease[dotCopy->m_dotChannel] += dotCopy->m_dotAmount;
			  else if(dotCopy->m_dotType == DOTTYPE_LOWER_DAMAGE)
				  victimObjectData->m_dotDamageReduction[dotCopy->m_dotChannel] += dotCopy->m_dotAmount;
			  else if(dotCopy->m_dotType == DOTTYPE_LOWER_MAXHP)
				  victimObjectData->m_dotLowerMaxHP += dotCopy->m_dotAmount;
			  else if(dotCopy->m_dotType == DOTTYPE_LOWER_MAXMP)
				  victimObjectData->m_dotLowerMaxMP += dotCopy->m_dotAmount;
			  else if(dotCopy->m_dotType == DOTTYPE_NULL_JETS)
				  victimObjectData->m_dotJetEffect = 1;
			  else if(dotCopy->m_dotType == DOTTYPE_NULL_SHIELDS)
				  victimObjectData->m_dotShieldEffect = 1;
		  }
	  }

	  ////////////////////////////////
	  // End EA2 Damage Calculation //
	  ////////////////////////////////
	return (float)overAllDamage;
}
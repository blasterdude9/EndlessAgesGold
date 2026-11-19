#include "StdAfx.h"
#include "questsystems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CQuestSystems::CQuestSystems(CSharedNet * sharedInterface)
{
    m_sharedInterfaceRef = sharedInterface;

}

CQuestSystems::~CQuestSystems(void)
{
}

//------------------------------------------------------------------------------------//
BOOL CQuestSystems::HandleQuestSys(CPlayerObject * currentPlayer,
								   CAccountObject * accountPtrLoc,
								   CAICollectionQuestObj * questPtr,
								   CServerItemObj * svrPtr,
								   DPNID idFromLoc,
								   int & questSelection)
{
	CString errorCatch;
	float expGained = 0.0f;
    
	if(currentPlayer == NULL || questPtr == NULL || svrPtr == NULL)
		return FALSE;

	if(!svrPtr->m_collectionQuestDB)
		return FALSE;

	int maxProgressions = (int)svrPtr->m_collectionQuestDB->GetCount();
	// A quest part the player cannot start yet (skill too low) still goes in the journal, with a hint
	// that says what is needed - otherwise the journal keeps showing the old part and players think
	// the quest restarted.
	auto logBlockedPart = [&](const CString & needText)
	{
		if(questPtr->m_completedKeyEnabled <= 0 || !currentPlayer->m_playerOrigRef)
			return;
		if(!currentPlayer->m_playerOrigRef->m_questJournal)
			currentPlayer->m_playerOrigRef->m_questJournal = new CQuestJournalList();
		currentPlayer->m_playerOrigRef->m_questJournal->LogQuest(questPtr->m_title,
		                                                        (questPtr->m_completedKeyEnabled == 2) ? NULL : questPtr->m_requiredItems,
		                                                        svrPtr->m_displayedName,
		                                                        needText,
		                                                        m_sharedInterfaceRef->m_itemManager,
		                                                        FALSE,
		                                                        questSelection,
		                                                        questPtr->m_completedKeyEnabled,
		                                                        questPtr->m_mobToHuntID,
		                                                        questPtr->m_maxDropCount,
		                                                        questPtr->m_dropGlid,
		                                                        questPtr->m_dropPercentage,
		                                                        questPtr->m_autocomplete,
		                                                        questPtr->m_skillTypeBonus,
		                                                        questPtr->m_expBonus,
		                                                        questPtr->m_maxExpLimit,
		                                                        questPtr->m_progessBasedByQuestJournal,
		                                                        questPtr->m_undeletable,
		                                                        questPtr->m_closeJournalEntryByName,
		                                                        questPtr->m_closeJournalOnSuccess,
		                                                        maxProgressions);
		m_sharedInterfaceRef->UpdateClientRecentQuestJournalEntry(currentPlayer);
	};

	if(questPtr->m_reward)
	{
		if(currentPlayer->m_currentInventory)
			if(!currentPlayer->m_currentInventory->CanTakeObjects(m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar,1))
			{//overloaded
				CString questLimitTxt = svrPtr->m_displayedName + ">" + "You must have more room in your inventory to do this.";
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
											  idFromLoc,
											  questLimitTxt,
											  0,
											  currentPlayer,
											  0);//confermation
				return FALSE;
			}//end overloaded	 
	}

	//questPtr->m_requiredSkillType
	//questPtr->m_requiredSkillLevel
	if(questPtr->m_requiredSkillLevel > 0)
	{
		if(currentPlayer->m_playerOrigRef)
		{//EVALUATE SKILL/LEVE REQ
			CSkillObject * sklRefPtr = currentPlayer->m_playerOrigRef->m_skills->GetSkillByType(questPtr->m_requiredSkillType);
			if(sklRefPtr)
			{//have the skill
				CString titleTemp;
				int curLevel = 1;

				CSkillObject * skillPtrRef = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(sklRefPtr->m_skillType);
				if(skillPtrRef)
				{
					sklRefPtr->EvaluateLevelAndTitle(titleTemp,curLevel,skillPtrRef);
					if(curLevel < questPtr->m_requiredSkillLevel)
					{
						{
							CString needText;
							needText.Format("Needs %s level %d (you have %d) - press F1 > Skills",
							                (const char*)skillPtrRef->m_skillName, questPtr->m_requiredSkillLevel, curLevel);
							logBlockedPart(needText);
						}
						CString skillName = "You must be at least Level "+m_sharedInterfaceRef->m_cabLib.ConvToString(questPtr->m_requiredSkillLevel);
						skillName = skillName+" ";
						skillName = skillName+skillPtrRef->m_skillName;
						skillName = skillName+" to begin this quest. Continue training this skill and you can check your skill levels by pressing F1, then select SKILLS from the menu bar on the top of the screen.";
						skillName = svrPtr->m_displayedName + ">" + skillName;
						m_sharedInterfaceRef->SendTextMsg( DPNSEND_GUARANTEED,
														   idFromLoc,
														   skillName,
														   0,
														   currentPlayer,
														   0);//confermation
						return FALSE;
					}	  
				}
			}//end have a skill
			else
			{//dont have the skill at all	
				CSkillObject * sklRefPtr = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(questPtr->m_requiredSkillType);
				if(sklRefPtr)
				{//known skill
					{
						CString needText;
						needText.Format("Needs %s level %d (you do not have it yet) - press F1 > Skills",
						                (const char*)sklRefPtr->m_skillName, questPtr->m_requiredSkillLevel);
						logBlockedPart(needText);
					}
					CString skillName = "You must be at least Level "+m_sharedInterfaceRef->m_cabLib.ConvToString(questPtr->m_requiredSkillLevel);
					skillName = skillName+" ";
					skillName = skillName+sklRefPtr->m_skillName;
					skillName = skillName+" to begin this quest. Continue training this skill and you can check your skill levels by pressing F1, then select SKILLS from the menu bar on the top of the screen.";
					skillName = skillName+sklRefPtr->m_skillName;
					skillName = svrPtr->m_displayedName + ">" + skillName;
					m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED,
														idFromLoc,
														skillName,
														0,
														currentPlayer,
														0);//confermation
					return FALSE;
				}//end skill know skill
				else
				{//unknown skill
					CString skillName = "You lack the Unknown level and skill required to attemp this...";
					m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED,
														idFromLoc,
														skillName,
														0,
														currentPlayer,
														0);//confermation
					return FALSE;
				}//end unknown skill
				return FALSE;
			}//end dont have the skill at all
		}//END EVALUATE SKILL LEVEL REQ
	}


				 int optError = 0;
				 if(questPtr->EvaluateQuest(currentPlayer->m_currentInventory,
					                        currentPlayer->m_skills,
											m_sharedInterfaceRef->m_skillDatabase,
											&expGained,
											currentPlayer,
											&optError,
											m_sharedInterfaceRef->m_currencySystem,
											m_sharedInterfaceRef->m_itemManager))
				 {//success  

                    if(questPtr->m_flagHostileOnSuccess > 0)
					{//validate hostile flags
					   if(currentPlayer->m_playerOrigRef)
					   {
					      if(questPtr->m_flagHostileOnSuccess == 2)//1= passive 2=hostile
						  {
                             currentPlayer->m_playerOrigRef->m_hostileFlag = 1;
							 currentPlayer->m_hostileFlag = 1;
							 m_sharedInterfaceRef->m_eventLog->AddEvent("Flag as hostile "+currentPlayer->m_charName);
						  }
						  else
						  {
						     currentPlayer->m_playerOrigRef->m_hostileFlag = 0;
							 currentPlayer->m_hostileFlag = 0;
							 m_sharedInterfaceRef->m_eventLog->AddEvent("Remove hostile Flag "+currentPlayer->m_charName);
						  }
					   }
					}//end validate hostile flags
 

				    if(questPtr->m_completedKeyEnabled > 0)
				    if(currentPlayer->m_playerOrigRef)
					{//quest log
						/*if (currentPlayer->m_playerOrigRef->m_questJournal->GetCount() > MAXJOURNALITEMS)
						{
							CString questLimitTxt = "You have too many quests in your journal to begin a new one!";
							m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
							  idFromLoc,
							  questLimitTxt,
							  0,
							  currentPlayer,
							  3);//confermation
							return FALSE;
						}*/

                       if(!currentPlayer->m_playerOrigRef->m_questJournal)
                           currentPlayer->m_playerOrigRef->m_questJournal = new CQuestJournalList();

					   if(questPtr->m_completedKeyEnabled == 2)
					   {//keep quest uncomplete show only hint
                          currentPlayer->m_playerOrigRef->m_questJournal->LogQuest(questPtr->m_title,
						                                                        NULL,
																				svrPtr->m_displayedName,
						                                                        questPtr->m_optionalIntruction,
																				m_sharedInterfaceRef->m_itemManager,
																				FALSE,
																				questSelection,
																				questPtr->m_completedKeyEnabled,
																				questPtr->m_mobToHuntID,
																				questPtr->m_maxDropCount,
																				questPtr->m_dropGlid,
																				questPtr->m_dropPercentage,
																				questPtr->m_autocomplete,
																				questPtr->m_skillTypeBonus,
																				questPtr->m_expBonus,
																				questPtr->m_maxExpLimit,
																				questPtr->m_progessBasedByQuestJournal,
																				questPtr->m_undeletable,
																				questPtr->m_closeJournalEntryByName,
																				questPtr->m_closeJournalOnSuccess,
																				maxProgressions);
					   }//end keep quest uncomplete show only hint
					   else{//normal success
					   currentPlayer->m_playerOrigRef->m_questJournal->LogQuest(questPtr->m_title,
						                                                        questPtr->m_requiredItems,
																				svrPtr->m_displayedName,
						                                                        questPtr->m_optionalIntruction,
																				m_sharedInterfaceRef->m_itemManager,
																				TRUE,
																				questSelection,
																				questPtr->m_completedKeyEnabled,
																				questPtr->m_mobToHuntID,
																				questPtr->m_maxDropCount,
																				questPtr->m_dropGlid,
																				questPtr->m_dropPercentage,
																				questPtr->m_autocomplete,
																				questPtr->m_skillTypeBonus,
																				questPtr->m_expBonus,
																				questPtr->m_maxExpLimit,
																				questPtr->m_progessBasedByQuestJournal,
																				questPtr->m_undeletable,
																				questPtr->m_closeJournalEntryByName,
																				questPtr->m_closeJournalOnSuccess,
																				maxProgressions);
					   }//end normal success
					   m_sharedInterfaceRef->UpdateClientRecentQuestJournalEntry(currentPlayer);
					}//end quest log

					
					
                    CString completedMsgRaw = "";
						
					if(questPtr->m_questCompletionMessage.GetLength() > 0)
					{
						if(svrPtr->m_displayedName.GetLength() > 0)
							completedMsgRaw = svrPtr->m_displayedName + ": " + questPtr->m_questCompletionMessage;
						else
							completedMsgRaw = questPtr->m_questCompletionMessage;
					}

					if(questPtr->m_cashReward > 0)
					{//cash reward
					   m_sharedInterfaceRef->m_currencySystem->AddCurrency(m_sharedInterfaceRef->m_itemManager,currentPlayer->m_currentInventory,-1,questPtr->m_cashReward);
					}//end cash reward
					if(expGained > 0.0f)
					{//appened exp gained to the end of it
                       completedMsgRaw = completedMsgRaw+" --you have gained experience--";
					}//end appened exp gained to the end of it
                    if(questPtr->m_miscAction > 0)
					{//execute command
					     if(questPtr->m_miscAction == 1)
						 {//request Store data
		                    if(currentPlayer->m_stencilChannel < 0)//cannot find stencil
		                       m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find establishment-store currentPlayer->m_stencilChannel handle quest sys");

						    CCommerceObj * cmPtr = m_sharedInterfaceRef->m_commerceDB->GetEstablishmentByPosition(currentPlayer->m_3dPosition,currentPlayer->m_stencilChannel);
                            if(cmPtr)
				            {//found store zone
				               m_sharedInterfaceRef->SendStoreData(cmPtr->m_uniqueID,currentPlayer);
				            }//end found store zone
						 }//end request Store data
						 else if(questPtr->m_miscAction == 2)
						 {//request bank data
                            m_sharedInterfaceRef->BankRequest(currentPlayer,idFromLoc);
						 }//end request bank data
					}//end execute command
					if(questPtr->m_spawnAiCfg > -1 && questPtr->m_spawnOnFail == FALSE)
					{//spawn ai	
					   if((GetTickCount()-questPtr->m_spawnStamp) > 5000)
					   {
                           m_sharedInterfaceRef->SpawnDynamicAIToAvailAiServer(  questPtr->m_spawnAiCfg,
																				 svrPtr->m_world, 
																				 questPtr->m_spawnPosition,
																				 svrPtr->m_networkAssignedID,
																				 questPtr->m_spawnedAiLifeline,//life time
																				 -1,
																				 questPtr->m_spawnArrayCountOption+20,
																				 questPtr->m_spawnArrayCountOption,
																				 questPtr->m_spawnArrayOffset,
																				 questPtr->m_spawnRotationRange,
																				 questPtr->m_spawnToWorldName);

                           questPtr->m_spawnStamp = GetTickCount();
					   }
					   else{//too soon to respawn
						    m_sharedInterfaceRef->m_eventLog->LogByPriority("HandleQuest spawn requested but NPC already has maxed spanwed AI.",2);
							return FALSE;
					   }//end too soon to respawn
					}//end spawn ai

					CString clientScriptCurrent = questPtr->m_clientScript;
					CString rewardScriptCurrent = questPtr->m_rewardScript;

                    m_sharedInterfaceRef->m_serverScripts->MacroHandler(completedMsgRaw,currentPlayer,m_sharedInterfaceRef);
					m_sharedInterfaceRef->m_serverScripts->MacroHandler(clientScriptCurrent,currentPlayer,m_sharedInterfaceRef);
					m_sharedInterfaceRef->m_serverScripts->MacroHandler(rewardScriptCurrent,currentPlayer,m_sharedInterfaceRef);

					if(clientScriptCurrent.GetLength() > 0)
					{//send script
                       m_sharedInterfaceRef->SendClientScript(currentPlayer,clientScriptCurrent,completedMsgRaw);
					}//end send script
					else if(questPtr->m_openDialogWindow)
                       m_sharedInterfaceRef->SendQuestDialog(currentPlayer,completedMsgRaw);
					else
					   m_sharedInterfaceRef->SendExtendedTextToChatWindow(currentPlayer,completedMsgRaw);

					if(rewardScriptCurrent.GetLength() > 0)
					{//send script
					   //process server specific functions
					   if(m_sharedInterfaceRef->m_serverScripts->ScriptProcessor(currentPlayer,sizeof(CPlayerObject), svrPtr,sizeof(CServerItemObj), rewardScriptCurrent, errorCatch,m_sharedInterfaceRef)==false)
                          m_sharedInterfaceRef->m_eventLog->AddEvent(errorCatch);
                       //send the rest to client
                       m_sharedInterfaceRef->SendClientScript(currentPlayer,rewardScriptCurrent,completedMsgRaw);//send client local scripts
					}//end send script

					if(questPtr->m_flagTimer)
					   currentPlayer->m_questTimer = GetTickCount();

					if(currentPlayer->m_playerOrigRef->m_questJournal)
	                if(questPtr->m_closeJournalEntryByName.GetLength() > 0 && questPtr->m_closeJournalEntryByName != "Reserved")
					{
	                      currentPlayer->m_playerOrigRef->m_questJournal->DeleteEntryByNpcName(questPtr->m_closeJournalEntryByName);
					      m_sharedInterfaceRef->UpdateClientRecentQuestJournalEntry(currentPlayer);
					}
					
					if(questPtr->m_spawnToLocationOverride == TRUE && questPtr->m_spawnOnFail == FALSE)
					{//must be called last for player
						BOOL continuePortaling = TRUE;
                        if(accountPtrLoc)
						{//account verifications
						  if(questPtr->m_portalLimitMsg != "Reserved" && questPtr->m_portalLimitMsg.GetLength() > 0)
						  {//evaluate local client content-he may not have the world to load
						   if(accountPtrLoc->m_localClientContentVersion < questPtr->m_portalLimitID)
						   {
                              m_sharedInterfaceRef->SendExtendedTextToChatWindow(currentPlayer,questPtr->m_portalLimitMsg);
						      continuePortaling = FALSE;							
						   }
						  }//end evaluate local client content-he may not have the world to load
						  if(continuePortaling)
						  if(questPtr->m_expansionIDReq > accountPtrLoc->m_expansionID)
						  {//doesnt have expansion clearance
						     if(questPtr->m_miscString.CompareNoCase("respawn") != 0)
							 {//give information
                                m_sharedInterfaceRef->SendExtendedTextToChatWindow(currentPlayer,questPtr->m_miscString);
						        continuePortaling = FALSE;			
							 }//end give information
							 else
							 {//redirect them
                                
								if(m_sharedInterfaceRef->m_serverScripts->ScriptProcessor(currentPlayer,sizeof(CPlayerObject),m_sharedInterfaceRef->m_serverScripts->GetUIDByName(accountPtrLoc->m_currentlyPlayingGameName),errorCatch,accountPtrLoc->m_currentlyPlayingGameName,-1,m_sharedInterfaceRef)==false)
								{
									m_sharedInterfaceRef->m_eventLog->AddEvent(errorCatch);
								}
								else
								{
						            m_sharedInterfaceRef->SpawnToPointInWld(    currentPlayer,
																				currentPlayer->m_respawnPosition,
																				currentPlayer->m_respawnWorldName);
									
								}

								return TRUE;
							 }//end redirect them
						  }//end doesnt have expansion clearance
						}//end account verifications
						if(continuePortaling)
						{//continue portalling
					     switch(questPtr->m_instanceGenerateType)
						 {
							case 0://standard
							 if(questPtr->m_makeTargetRebirthPoint)
								 m_sharedInterfaceRef->HandleReassignRespawnByDetails(  currentPlayer,
																						accountPtrLoc,
																						idFromLoc,
																						questPtr->m_spawnPosition,
																						questPtr->m_spawnToWorldName);
							
							  m_sharedInterfaceRef->SpawnToPointInWld(  currentPlayer,
																		questPtr->m_spawnPosition,
																		questPtr->m_spawnToWorldName);
							break;
							case 1://group portal
							    HandleGroupInstancing( currentPlayer,questPtr,svrPtr,idFromLoc);
							break;
							case 2://arena portal
								{
									this->m_sharedInterfaceRef->m_eventLog->LogByPriority(
										"Call to HandleArena() 01!",
										2);
									HandleArena(currentPlayer,questPtr,svrPtr,idFromLoc);
								}
							break;
						 }//end switch
						}//end continue portalling
					}//end must be called last

					if(questPtr->m_despawnOnReward)
				    {
                       if(svrPtr->m_spawnID > -1)
					   {
                          if(!m_sharedInterfaceRef->m_serverItemGenerator->InformGeneratorThatItCanRespawn(svrPtr->m_spawnID))
							  m_sharedInterfaceRef->m_eventLog->LogByPriority("InformGeneratorThatItCanRespawn() failed to locate spawn ID",(DWORD)svrPtr->m_spawnID,2);
						  else
						      m_sharedInterfaceRef->m_eventLog->LogByPriority("InformGeneratorThatItCanRespawn() sucessfully informed",(DWORD)svrPtr->m_spawnID,2);
					   }

				       m_sharedInterfaceRef->m_netComObject->m_runtimeItems->DeleteByID(svrPtr->m_networkAssignedID);
				    }

					return TRUE;
				 }//end success
				 else{//send info on quest

                 
                    if(questPtr->m_spawnAiCfg > -1 && questPtr->m_spawnOnFail == TRUE)
					{//spawn ai	
					   if((GetTickCount()-questPtr->m_spawnStamp) > 5000)
					   {
                           m_sharedInterfaceRef->SpawnDynamicAIToAvailAiServer(questPtr->m_spawnAiCfg,
											             svrPtr->m_world, 
											             questPtr->m_spawnPosition,
											             svrPtr->m_networkAssignedID,
											             questPtr->m_spawnedAiLifeline,
														 -1,
														 questPtr->m_spawnArrayCountOption+20,
														 questPtr->m_spawnArrayCountOption,
														 questPtr->m_spawnArrayOffset,
														 questPtr->m_spawnRotationRange,
														 questPtr->m_spawnToWorldName);

                           questPtr->m_spawnStamp = GetTickCount();
					   }
					   else{//too soon to respawn
/*						   CString buildTemp = svrPtr->m_displayedName+">";
						   buildTemp = buildTemp+"Theres too much going on right now..come back later.";
					        m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                                     idFromLoc,
						                                     buildTemp,
							                                 0,
													         currentPlayer,
														     0);//confermation
*/							return FALSE;
					   }//end too soon to respawn
					}//end spawn ai

				    if(questPtr->m_completedKeyEnabled == 3 || questPtr->m_completedKeyEnabled == 1 || questPtr->m_completedKeyEnabled == 4)
				    if(currentPlayer->m_playerOrigRef)
					{//quest log
                       if(!currentPlayer->m_playerOrigRef->m_questJournal)
                           currentPlayer->m_playerOrigRef->m_questJournal = new CQuestJournalList();
					   
					   currentPlayer->m_playerOrigRef->m_questJournal->LogQuest(questPtr->m_title,
						                                                        questPtr->m_requiredItems,
																				svrPtr->m_displayedName,
						                                                        questPtr->m_optionalIntruction,
																				m_sharedInterfaceRef->m_itemManager,
																				FALSE,
																				questSelection,
																				questPtr->m_completedKeyEnabled,
																				questPtr->m_mobToHuntID,
																				questPtr->m_maxDropCount,
																				questPtr->m_dropGlid,
																				questPtr->m_dropPercentage,
																				questPtr->m_autocomplete,
																				questPtr->m_skillTypeBonus,
																				questPtr->m_expBonus,
																				questPtr->m_maxExpLimit,
																				questPtr->m_progessBasedByQuestJournal,
																				questPtr->m_undeletable,
																				questPtr->m_closeJournalEntryByName,
																				questPtr->m_closeJournalOnSuccess,
																				maxProgressions);
					   m_sharedInterfaceRef->UpdateClientRecentQuestJournalEntry(currentPlayer);

					}//end quest log
					//int stringLength = questPtr->m_questDesription.GetLength();        
					
					CString completedMsgRaw = "";
						
					if(questPtr->m_questDesription.GetLength() > 0)
					{
						if(svrPtr->m_displayedName.GetLength() > 0)
							completedMsgRaw = svrPtr->m_displayedName + ": " + questPtr->m_questDesription;
						else
							completedMsgRaw = questPtr->m_questDesription;
					}

					CString clientScriptCurrent = questPtr->m_clientScript;
					CString requiredScriptCurrent = questPtr->m_requiredScript;

                    m_sharedInterfaceRef->m_serverScripts->MacroHandler(completedMsgRaw,currentPlayer,m_sharedInterfaceRef);
					m_sharedInterfaceRef->m_serverScripts->MacroHandler(clientScriptCurrent,currentPlayer,m_sharedInterfaceRef);
					m_sharedInterfaceRef->m_serverScripts->MacroHandler(requiredScriptCurrent,currentPlayer,m_sharedInterfaceRef);

					if(clientScriptCurrent.GetLength() > 0)
					{//send script
                       m_sharedInterfaceRef->SendClientScript(currentPlayer,clientScriptCurrent,completedMsgRaw);
					}//end send script
					else if(questPtr->m_openDialogWindow)
                       m_sharedInterfaceRef->SendQuestDialog(currentPlayer,completedMsgRaw);
					else
					   m_sharedInterfaceRef->SendExtendedTextToChatWindow(currentPlayer,completedMsgRaw);

					if(requiredScriptCurrent.GetLength() > 0)
					{//send script
					   //process server specific functions
                       if(m_sharedInterfaceRef->m_serverScripts->ScriptProcessor(currentPlayer,sizeof(CPlayerObject), svrPtr,sizeof(CServerItemObj), requiredScriptCurrent, errorCatch,m_sharedInterfaceRef)==false)
                          m_sharedInterfaceRef->m_eventLog->AddEvent(errorCatch);
                       //send the rest to client
                       m_sharedInterfaceRef->SendClientScript(currentPlayer,requiredScriptCurrent,completedMsgRaw);
					}//end send script

					if(currentPlayer->m_playerOrigRef->m_questJournal)
	                if(questPtr->m_closeJournalEntryByName.GetLength() > 0 && questPtr->m_closeJournalEntryByName != "Reserved")
					{
	                   currentPlayer->m_playerOrigRef->m_questJournal->DeleteEntryByNpcName(questPtr->m_closeJournalEntryByName);
					   m_sharedInterfaceRef->UpdateClientRecentQuestJournalEntry(currentPlayer);
					}
               
 
					if(questPtr->m_spawnToLocationOverride == TRUE && questPtr->m_spawnOnFail == TRUE)
					{//must be called last
						this->m_sharedInterfaceRef->m_eventLog->LogByPriority("Inside Handle Quest Sys for Handle Arena: 1", 2);
						BOOL continuePortaling = TRUE;
                        if(accountPtrLoc)
						{//account verifications
						  if(questPtr->m_portalLimitMsg != "Reserved" && questPtr->m_portalLimitMsg.GetLength() > 0)
						  {//evaluate local client content-he may not have the world to load
						   if(accountPtrLoc->m_localClientContentVersion < questPtr->m_portalLimitID)
						   {
                              m_sharedInterfaceRef->SendExtendedTextToChatWindow(currentPlayer,questPtr->m_portalLimitMsg);
						      continuePortaling = FALSE;
						   }
						  }//end evaluate local client content-he may not have the world to load
						  if(continuePortaling)
						  if(questPtr->m_expansionIDReq > accountPtrLoc->m_expansionID)
						  {//doesnt have expansion clearance
						     //continuePortaling = FALSE;
							 this->m_sharedInterfaceRef->m_eventLog->LogByPriority("Inside Handle Quest Sys for Handle Arena: 2", 2);
                             if(questPtr->m_miscString.CompareNoCase("respawn") != 0)
							 {//give information
                                m_sharedInterfaceRef->SendExtendedTextToChatWindow(currentPlayer,questPtr->m_miscString);
						        continuePortaling = FALSE;
							 }//end give information
							 else
							 {//redirect them
								if(m_sharedInterfaceRef->m_serverScripts->ScriptProcessor(currentPlayer,sizeof(CPlayerObject),m_sharedInterfaceRef->m_serverScripts->GetUIDByName(accountPtrLoc->m_currentlyPlayingGameName),errorCatch,accountPtrLoc->m_currentlyPlayingGameName,-1,m_sharedInterfaceRef)==false)
								{
									m_sharedInterfaceRef->m_eventLog->AddEvent(errorCatch);
								}
								else
								{
						            m_sharedInterfaceRef->SpawnToPointInWld(    currentPlayer,
																				currentPlayer->m_respawnPosition,
																				currentPlayer->m_respawnWorldName);
								}

								return TRUE;
							 }//end redirect them

						  this->m_sharedInterfaceRef->m_eventLog->LogByPriority("Inside Handle Quest Sys for Handle Arena: 3", 2);
						  }//end doesnt have expansion clearance
						}//end account verifications
						if(continuePortaling)
						{
						 switch(questPtr->m_instanceGenerateType)
						 {
						this->m_sharedInterfaceRef->m_eventLog->LogByPriority("Inside Handle Quest Sys for Handle Arena: 4", 2);	
						 case 0://standard
							 if(questPtr->m_makeTargetRebirthPoint)
								 m_sharedInterfaceRef->HandleReassignRespawnByDetails(  currentPlayer,
																						accountPtrLoc,
																						idFromLoc,
																						questPtr->m_spawnPosition,
																						questPtr->m_spawnToWorldName);
							
							  m_sharedInterfaceRef->SpawnToPointInWld(  currentPlayer,
																		questPtr->m_spawnPosition,
																		questPtr->m_spawnToWorldName);
							break;
							case 1://group portal
							     HandleGroupInstancing( currentPlayer,questPtr,svrPtr,idFromLoc);
							break;
							case 2://arena portal
								{
									this->m_sharedInterfaceRef->m_eventLog->LogByPriority(
										"Call to HandleArena() 02!",
										2);
								 HandleArena(currentPlayer,questPtr,svrPtr,idFromLoc);
								}
							break;
						 }//end switch
						}
					}//end must be called last


				 return FALSE;
				 }//end send info on quest	
	 this->m_sharedInterfaceRef->m_eventLog->LogByPriority("Returning Out From Number: 4", 2);
	 return TRUE;
}

BOOL CQuestSystems::HandleArena(CPlayerObject * currentPlayer,
								CAICollectionQuestObj * questPtr,
								CServerItemObj * svrPtr,
								DPNID idFromLoc)
{
	// Add the player to CAICollectionQuestList members collection
	int	iResult = questPtr->AddToDWMEM(questPtr->m_instanceGenerateParam, currentPlayer->m_handle);
	int iNumCurrentPlayers = 0;

	this->m_sharedInterfaceRef->m_eventLog->LogByPriority("MEMCOUNT (HandleArena): %i", questPtr->m_dwMemCount, 2);

	// Registration FAIL
	if(iResult == 0)
	{
		this->m_sharedInterfaceRef->m_eventLog->LogByPriority(
			"HandleArena() ERROR AddToDWMEM returned 0.",
			2);
		return FALSE;
	}
	// Registration SUCCESS, arena IS full
	//	NOTE: Return out of the function here to prevent messaging to final
	//	registered player, who will already be spawning into the arena.
	else if (iResult == 1)
	{
		this->StartArena_requiresRuntimeEntityLock(questPtr);
		return TRUE;
	}
	// Registration SUCCESS, arena is NOT full
	else if(iResult == 2)
	{
		iNumCurrentPlayers = questPtr->DWMEM_getCount();

		if(iNumCurrentPlayers >= questPtr->m_instanceGenerateParam3 && !questPtr->m_arenaStartTimerStamp)
		{
			// m_arenaStartTimerStamp > 0, enabled callback will have to check this
			questPtr->m_arenaStartTimerStamp = GetTickCount();

			this->m_sharedInterfaceRef->m_eventLog->LogByPriority(
				"HandleArena() has SET the arena autostart timestamp: ",
				questPtr->m_arenaStartTimerStamp,
				2);

			// Handle fluke
			if(questPtr->m_arenaStartTimerStamp == 0)
				questPtr->m_arenaStartTimerStamp = 1;

			// Tell the whole server the arena is about to start
			CString arenaName = questPtr->m_title;
			arenaName.Trim();
			if(arenaName.IsEmpty())
				arenaName = "The arena";
			CString arenaMsg;
			if(questPtr->m_instanceGenerateParam4 > 0)
			{
				int arenaMins = (int)((questPtr->m_instanceGenerateParam4 + 59999) / 60000);
				arenaMsg.Format("[ARENA] %s starts in %d minute%s - sign up now!", (LPCSTR)arenaName, arenaMins, arenaMins == 1 ? "" : "s");
			}
			else
				arenaMsg.Format("[ARENA] %s is filling up - sign up now!", (LPCSTR)arenaName);
			this->m_sharedInterfaceRef->BroadcastToAll(arenaMsg);
		}
	}
	// Double signup error
	else if(iResult == 9)
	{
		this->m_sharedInterfaceRef->m_eventLog->LogByPriority(
			"***HandleArena() ERROR AddToDWMEM returned 9 - double signup.",
			2);
		return FALSE;
	}

	// CREATE the ARENA TYPE REQUEST response packet
	BLOCKQUEST_DATA HeaderPacket;
	HeaderPacket.m_catagorie = 33;
	HeaderPacket.m_itemAssignedID = 0;

	// COPY message header into packet
	//SafeDeleteArray(pData);
	BYTE *pData = NULL;
	int iPos = 0;
	int iSize = sizeof(BLOCKQUEST_DATA)	// Header 
		+ sizeof(ARENA_SEVERITY)		// ArenaResult struct
		+ sizeof(ARENA_RESULTS)
		+ sizeof(char)					// Packet ID
		+ sizeof(int);					// Result code
	pData = new BYTE[iSize];
	::memcpy(&pData[iPos], &HeaderPacket, sizeof(BLOCKQUEST_DATA));
	iPos = sizeof(BLOCKQUEST_DATA);

	// COPY packet ID into the packet
	unsigned char iPacketID = ARENA_JOIN_ID;
	::memcpy(&pData[iPos], &iPacketID, sizeof(char));
	iPos += sizeof(char);

	// COPY arena results into the packet
	ArenaResult result;
	::memcpy(&pData[iPos], &result.Severity, sizeof(ARENA_SEVERITY));
	iPos += sizeof(ARENA_SEVERITY);
	::memcpy(&pData[iPos], &result.Result, sizeof(ARENA_RESULTS));
	iPos += sizeof(ARENA_RESULTS);

	// ENCODE final response packet: Arena bot ID
	//	NOTE: Returning any non -1 integer value indicates success
	iResult = svrPtr->m_spawnID;
	::memcpy(&pData[iPos], &iResult, sizeof(int));
	iPos += sizeof(int);

	// SEND response packet to requesting player
	this->m_sharedInterfaceRef->m_netComObject->SendData(pData,
		 iSize,
		 DPNSEND_GUARANTEED,
		 idFromLoc,
		 m_sharedInterfaceRef->m_netComObject->m_packetTimeout);
	delete [] pData;//SendData copies the packet - free ours (was leaked every arena reply)
	pData = 0;
	return TRUE;
}

BOOL CQuestSystems::StartArena_requiresRuntimeEntityLock(CAICollectionQuestObj * questPtr)
{
	int currentSignUps = questPtr->DWMEM_getCount();
	if(currentSignUps == 0)
	{
		m_sharedInterfaceRef->m_eventLog->LogByPriority("StartArena_requiresRuntimeEntityLock() starting arena:cannot start arena with 0 signups.",2);
		return FALSE;
	}

	int handleToInstanceID = 0;
	
	BOOL spawnedWorld = FALSE;
	m_sharedInterfaceRef->m_eventLog->LogByPriority("StartArena_requiresRuntimeEntityLock() starting arena:rule count.",(DWORD)questPtr->m_ruleCount,2);
    //loop through handle
    for(int hLoop=0;hLoop<questPtr->m_dwMemCount;hLoop++)
	{
            if(questPtr->m_dwMem[hLoop] < 0)
			   continue;//arena may not be full filter -1

		    CPlayerObject *pPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(questPtr->m_dwMem[hLoop]);
            if(pPlayer)
			{
				if(!spawnedWorld)
				{
					//request asnc pending zone directive
					handleToInstanceID = pPlayer->m_handle;//must have live handle as controller-todo make close world function that turns off identifier to no-join. just in case primary handle leaves and starts new arena
					m_sharedInterfaceRef->m_threadedDataProcessor->AddToQue(0,questPtr->m_spawnToWorldName,handleToInstanceID,0,29,(byte*)questPtr->m_rules,sizeof(DYNRULES)*questPtr->m_ruleCount);//misc data proc async creation of zone
				    spawnedWorld = TRUE;
				}

				if(questPtr->m_makeTargetRebirthPoint)
				{
				   CAccountObject * acPtrTemp = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID(pPlayer->m_currentID);
                   if(acPtrTemp)
					   m_sharedInterfaceRef->HandleReassignRespawnByDetails(    pPlayer,
																				acPtrTemp,
																				pPlayer->m_currentID,
																				questPtr->m_spawnPosition,
																				questPtr->m_spawnToWorldName);
				}
	
				m_sharedInterfaceRef->SpawnToPointInWld(  pPlayer,
														  questPtr->m_spawnPosition,
														  questPtr->m_spawnToWorldName,
														  &handleToInstanceID);
			}
	}
	if(spawnedWorld)
	{
		CString arenaName = questPtr->m_title;
		arenaName.Trim();
		if(arenaName.IsEmpty())
			arenaName = "The arena";
		m_sharedInterfaceRef->BroadcastToAll("[ARENA] " + arenaName + " has begun!");
	}

    //clear signups and get ready for next arena batch
	questPtr->m_arenaBetPool			= 0;
    questPtr->ClearDWMEM();
	questPtr->m_arenaStartTimerStamp	= 0;
	
	return TRUE;
}

BOOL CQuestSystems::HandleGroupInstancing(  CPlayerObject * currentPlayer,
											CAICollectionQuestObj * questPtr,
											CServerItemObj * svrPtr,
											DPNID idFromLoc)
{
        int copyGroupID = currentPlayer->m_groupID;
		int handleToInstanceID = currentPlayer->m_handle;
		if(copyGroupID < 1)
		{//not part of a group
		   CString questReply = svrPtr->m_displayedName + ">" + "Must be part of a group to continue.";
           m_sharedInterfaceRef->SendTextMsg( DPNSEND_GUARANTEED,
											  idFromLoc,
											  questReply,
											  0,
											  currentPlayer,
											  0);//confermation
           return FALSE;
		}//end not part of a group

		//request asnc pending zone directive
		m_sharedInterfaceRef->m_threadedDataProcessor->AddToQue(0,questPtr->m_spawnToWorldName,handleToInstanceID,0,29,0,0);//misc data proc async creation of zone
        //ARB TODO generate instance type ID and assign to players before spawn message-and generate target pending zone
        CAxList * gList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(copyGroupID);
		if(gList)
		{
           for(CAxLinkObj *posLoc = gList->GetHeadPosition_unsafe(); posLoc != NULL;)
		   {
				CPlayerObject *player = (CPlayerObject *)gList->GetNext_unsafe(&posLoc);
				
				if(player->m_energy < 1)
				   continue;//filter dead players

				if(player->m_aiControlled)
				   continue;

				
				if(questPtr->m_makeTargetRebirthPoint)
				{
                   CAccountObject * acPtrTemp = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID(player->m_currentID);
                   if(acPtrTemp)
						m_sharedInterfaceRef->HandleReassignRespawnByDetails(       player,
																					acPtrTemp,
																					idFromLoc,
																					questPtr->m_spawnPosition,
																					questPtr->m_spawnToWorldName);


				}
	
			    m_sharedInterfaceRef->SpawnToPointInWld(  player,
														  questPtr->m_spawnPosition,
														  questPtr->m_spawnToWorldName,
														  &handleToInstanceID);
		   }
		}
		return TRUE;
}

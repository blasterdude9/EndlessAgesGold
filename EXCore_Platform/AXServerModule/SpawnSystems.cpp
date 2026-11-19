#include "StdAfx.h"
#include "spawnsystems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSpawnSystems::CSpawnSystems(CSharedNet * sharedInterface)
{
    m_sharedInterfaceRef = sharedInterface;
}

CSpawnSystems::~CSpawnSystems(void)
{
}

//--------------Handles system messages-----------------------------------------------//
BOOL CSpawnSystems::HandleSpawnMessagesServerSide(LPSPAWN_STRUCT formatedMessage,DPNID idFromLoc,CAccountObject * accountPtr,int channelOverride)
{
		accountPtr->m_lastELtime = (DWORD)-9999999999999;//reset time creep
		accountPtr->m_syncTrackingTime = 0;
	
		accountPtr->m_currentcharInUse = 0;//always 0
        CPlayerObject * plSearch = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_Alive_LOS(idFromLoc);
		if(plSearch)
		{
			m_sharedInterfaceRef->m_eventLog->AddEvent("::Player already on server::",accountPtr->m_accountName);
			m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			return FALSE;
		}
		//end this player should not be spawned already ignore if true
		CPlayerObject * newPobj = new CPlayerObject();
		newPobj->m_spawnTimeStamp = GetTickCount();
		newPobj->m_murderWearTimeStamp = GetTickCount();
		
		newPobj->m_firemsgsoutbox					= new CPacketObjList();
		newPobj->m_playerChangeInvenOutbox			= new CPacketObjList();
        newPobj->m_playerItemCfgOutbox				= new CPacketObjList();
		newPobj->m_playerEntityCfgOutbox			= new CPacketObjList();
		newPobj->m_playerEntityDamageMgsOutbox		= new CPacketObjList();//lossy Mov pack
   	    newPobj->m_playerEntityGeneralMgsOutbox		= new CPacketObjList();//lossy Mov pack
		newPobj->m_playerEntityDetailedMgsOutbox	= new CPacketObjList();//lossy Mov pack
		newPobj->m_attributeOutbox					= new CPacketObjList();
		newPobj->m_equipMsgOutbox					= new CPacketObjList();//non loss
	    newPobj->m_txtMsgOutbox						= new CPacketObjList();//non loss
		newPobj->m_statisticsOutbox					= new CPacketObjList();//non loss
		newPobj->m_miscOutBox						= new CPacketObjList();
		newPobj->m_entityAddressRef					= NULL;

		newPobj->m_currentID	      = idFromLoc;//use for routeing messages only
		newPobj->m_visualDistanceRef  = m_sharedInterfaceRef->m_netComObject->m_serverCalculateRadius;
   
        //get database and give the start inventory to player if first time spawning/creating character
        CMovementObj * DBPtr = m_sharedInterfaceRef->m_entObjList->GetObjByIndex(formatedMessage->dbIndex);
		if(!DBPtr){//invalid spawn index
			delete newPobj;
			newPobj = 0;

            m_sharedInterfaceRef->m_eventLog->AddEvent("Player attempted to spawn unknow databse entity");
            return FALSE;
		}//end invalid spawn index
		else{//valid attach start inventory
				if(!accountPtr->m_characterOnAccount){//invalid character selection
					delete newPobj;
			        newPobj = 0;
                    m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid character selection: Account:",accountPtr->m_accountName);
				    return FALSE;
				}//end invalid character selection
                CPlayerObject * playerObj = accountPtr->m_characterOnAccount;
                if(playerObj->m_stats)
				   playerObj->m_stats->Clone(&newPobj->m_stats);

				newPobj->m_invunerableDuration				= playerObj->m_invunerableDuration;
				newPobj->m_invunerableTemp					= playerObj->m_invunerableTemp;
				newPobj->m_invunerableStamp					= playerObj->m_invunerableStamp;
				newPobj->m_invunOptionTemp					= playerObj->m_invunOptionTemp;
				newPobj->m_hostileFlag						= playerObj->m_hostileFlag;
                newPobj->m_entityAddressRef					= (CObject*)DBPtr;
				newPobj->m_phyRef							= DBPtr->List_movementDB;
				newPobj->m_dynamicRadius					= DBPtr->List_boundingRadius;

				for(int i = 0; i < MAX_BONUS; i++)
				{
					newPobj->m_bonus[i].m_bonusAmount		= playerObj->m_bonus[i].m_bonusAmount;
					newPobj->m_bonus[i].m_bonusDuration		= playerObj->m_bonus[i].m_bonusDuration;
					newPobj->m_bonus[i].m_bonusType			= playerObj->m_bonus[i].m_bonusType;
					newPobj->m_bonus[i].m_damageMult		= playerObj->m_bonus[i].m_damageMult;
					newPobj->m_bonus[i].m_maxDuration		= playerObj->m_bonus[i].m_maxDuration;
				}
				
				for(int wound = 0; wound < 8; wound++)
					newPobj->m_woundChannel[wound] = playerObj->m_woundChannel[wound];

				newPobj->m_mute = playerObj->m_mute;
				if(accountPtr->m_deafMute)
				{
				newPobj->m_deaf               = TRUE; 
				newPobj->m_mute               = TRUE;
				newPobj->m_deafMuteNotify     = TRUE;
				}
				
                newPobj->m_currentWorldMap     = playerObj->m_currentWorldMap;
				
				
				if(channelOverride > -1)
                   newPobj->m_currentWorldID = channelOverride;
				else//get defaul from world if one isnt specified
				   newPobj->m_currentWorldID      = m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->GetChannelByName_safe(newPobj->m_currentWorldMap);
				
				
				if(newPobj->m_currentWorldID < 0)
				{
                    m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid world load:"+newPobj->m_currentWorldMap,accountPtr->m_accountName);
                    newPobj->m_currentWorldID = 0;
				}
				newPobj->m_stencilChannel = m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->GetStencilChannelByLiveChannel_safe(newPobj->m_currentWorldID);//translate instance channel to stencil channel
				
				DWORD param = 0;
				if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,newPobj->m_currentWorldID,WORLDRULE_SPAWN_INVUNERABLE,&param))
				{//invunerable settings
				   newPobj->SetInvunerableDuration(param);
				}
				else
				   newPobj->SetInvunerableDuration(5000);
				
				playerObj->m_invunerableTemp  = FALSE;
				playerObj->m_invunerableStamp = GetTickCount();
				playerObj->m_invunOptionTemp  = 0;

				newPobj->m_skillDBisReferenced = TRUE;
				newPobj->m_skills = playerObj->m_skills;
				playerObj->m_currentInventory->Clone(&newPobj->m_currentInventory);


				if(playerObj->m_reconfigInven)
				   playerObj->m_reconfigInven->Clone(&newPobj->m_reconfigInven);
				newPobj->m_playerOrigRef = playerObj;

                newPobj->m_deformableMeshCount = playerObj->m_deformableMeshCount; 
				if(newPobj->m_deformableMeshCount > 0){
                   if(newPobj->m_deformableCfg)
					  delete [] newPobj->m_deformableCfg;
				      newPobj->m_deformableCfg = 0;

				   newPobj->m_deformableCfg = new int[newPobj->m_deformableMeshCount];
				   CopyMemory(newPobj->m_deformableCfg,playerObj->m_deformableCfg,(sizeof(int)*newPobj->m_deformableMeshCount));
				}
			
			    if(newPobj->m_stats)
				{
				   newPobj->m_maxEnergy        = newPobj->m_stats->GetBenefitBonus(1,newPobj->m_currentInventory) + DBPtr->List_maxEnergy;
				   playerObj->m_maxEnergy      = newPobj->m_stats->GetBenefitBonus(1,newPobj->m_currentInventory) + DBPtr->List_maxEnergy;
				}
				else
                newPobj->m_maxEnergy			= playerObj->m_maxEnergy;

				newPobj->m_handle				= playerObj->m_handle;
                newPobj->m_energy				= playerObj->m_energy;
				newPobj->m_clanHandle			= playerObj->m_clanHandle;
				newPobj->m_charName				= playerObj->m_charName;
				newPobj->m_originalEDBEnergy	= playerObj->m_originalEDBEnergy;

				newPobj->m_admin				= accountPtr->m_administratorAccess;
				newPobj->m_gm					= accountPtr->m_gm;
				newPobj->m_spawnAbility			= accountPtr->m_canSpawn;
				FlagBodyAsPlaced(newPobj->m_handle);//flag any pending bodies as placed

                if(newPobj->m_energy <= 0)
                   newPobj->m_energy = (newPobj->m_maxEnergy/2);

				if(newPobj->m_energy > newPobj->m_maxEnergy)
				   newPobj->m_energy = newPobj->m_maxEnergy;
				
				//Get this player's name and skill title to display
				bool skillTitleFound = false;
				newPobj->m_charName = playerObj->m_charName;
				/*if(playerObj->m_skills->m_skillToDisplay != -1)
				{
					CSkillObject *skill = playerObj->m_skills->GetSkillByType(playerObj->m_skills->m_skillToDisplay);
					CSkillObject *baseSkill = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(playerObj->m_skills->m_skillToDisplay);

					if(skill)
					{
						POSITION posLevel = baseSkill->m_levelRangeDB->GetHeadPosition();
						while(posLevel)
						{
							CLevelRangeObject *level = (CLevelRangeObject*)baseSkill->m_levelRangeDB->GetNext(posLevel);
							if(level->m_rangeStart < skill->m_currentExperience && level->m_rangeEnd > skill->m_currentExperience)
							{
								newPobj->m_visName = level->m_levelTitle + ":" + playerObj->m_charName;
								skillTitleFound = true;
								break;
							}
						}
					}
				}*/

				/*if(!skillTitleFound)
					newPobj->m_visName  = playerObj->m_charName;

				newPobj->m_clanAbbrev				= playerObj->m_clanAbbrev;
				if(newPobj->m_clanAbbrev.GetLength() > 0)
					newPobj->m_visName = newPobj->m_clanAbbrev + "-" + newPobj->m_visName;*/

				newPobj->m_clanAbbrev				= playerObj->m_clanAbbrev;
				newPobj->ReassembleName(m_sharedInterfaceRef);
			
				newPobj->m_arenaBasePoints			= playerObj->m_arenaBasePoints;
				newPobj->m_pvpKills					= playerObj->m_pvpKills;
				newPobj->m_arenaKills				= playerObj->m_arenaKills;
				newPobj->m_arenaTotalKills			= playerObj->m_arenaTotalKills;
				newPobj->m_accountNumberRef			= accountPtr->m_accountNumber;
				newPobj->m_stasis					= playerObj->m_stasis;
	            newPobj->m_altCfgIndex				= playerObj->m_altCfgIndex;
				newPobj->m_curEDBIndex				= playerObj->m_curEDBIndex;
				newPobj->m_originalEDBCfg			= playerObj->m_originalEDBCfg;
				newPobj->m_dbCfg					= playerObj->m_dbCfg;
				newPobj->m_aiCfgOfMounted			= playerObj->m_aiCfgOfMounted;
				newPobj->m_countDownToMurderPardon	= playerObj->m_countDownToMurderPardon;
				newPobj->m_deathCount				= playerObj->m_deathCount;
				newPobj->m_groupID					= playerObj->m_groupID;
				newPobj->m_instanceType				= playerObj->m_instanceType;
				newPobj->m_factionStanding			= playerObj->m_factionStanding;
				newPobj->m_arenaItemsGiven			= playerObj->m_arenaItemsGiven;
				newPobj->m_teamTemp					= -1;
				newPobj->m_arenaBetAmount			= playerObj->m_arenaBetAmount;
				
				newPobj->m_groupStruct.m_squadDataIsDirty		= 0;
				newPobj->m_groupStruct.m_groupPlacementIndex	= playerObj->m_groupStruct.m_groupPlacementIndex;
				newPobj->m_groupStruct.m_groupLeadership		= playerObj->m_groupStruct.m_groupLeadership;
				newPobj->m_squadName							= playerObj->m_squadName;

				newPobj->ReassembleName(m_sharedInterfaceRef);
				m_sharedInterfaceRef->SendClientScript(newPobj, "UpdateCharName("+newPobj->m_visName+")","");				
				
				newPobj->m_dotList				   = new CDotClassList();
				newPobj->m_dotList->m_dotLimit	   = m_sharedInterfaceRef->m_maxDotTrack;


				if(newPobj->m_hostileFlag == 1)
				   m_sharedInterfaceRef->m_eventLog->AddEvent("Spawned Hostile"+newPobj->m_charName);

				if(playerObj->m_noteriety)
				   playerObj->m_noteriety->Clone(&newPobj->m_noteriety);
				else
				   newPobj->m_noteriety = new CNoterietyObj();
				
				if(!newPobj->m_noteriety->m_currentAgressors)
					newPobj->m_noteriety->m_currentAgressors = new CAggressorObjList();
				
				// SETUP for arena apawn: Team assignment has already been handled, 
				//	so copy over the team ID to the new CPlayerObj
				DWORD param1 = 0;
				newPobj->m_teamTemp = playerObj->m_teamTemp;

				if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,newPobj->m_currentWorldID,WORLDRULE_TEAM_ELIMINATION,&param1))
				{//TEAM ELIMINATION RULE SPECIFIC
				  // BOOL res = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->SortToTeam(newPobj->m_currentWorldID,param1,newPobj);//player shouldnt be added yet-param is team count
				   //if(res)

					// Players should already be assigned to teams
					if (newPobj->m_teamTemp >= 0) 
				   {
					  // m_sharedInterfaceRef->m_eventLog->LogByPriority("SortToTeam() success:"+newPobj->m_charName+" to team:",(DWORD)newPobj->m_teamTemp,2);
					   //sort to team location
					   if(!m_sharedInterfaceRef->m_poiSystem->Get3DPositionInOrderInChannel_safe(newPobj->m_currentWorldID,newPobj->m_teamTemp,&playerObj->m_3dPosition))
						   m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionInOrderInChannel_safe() FAILED Team Elimination:",(DWORD)newPobj->m_teamTemp,2);
					   else
					   {
					       newPobj->m_allowLargeMovement = true;
					       CString dynScript;
						   dynScript.Format("LocalTeleport(%i,%i,%i)",(int)playerObj->m_3dPosition.x,(int)(playerObj->m_3dPosition.y+1.0f),(int)playerObj->m_3dPosition.z);
						   m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionInOrderInChannel_safe() success sending script:"+dynScript,(DWORD)newPobj->m_teamTemp,2);
						   m_sharedInterfaceRef->SendClientScript(newPobj,dynScript,"");//move client script
					   }
				   }
				   else
					  m_sharedInterfaceRef->m_eventLog->LogByPriority("SortToTeam() FAILED player:"+newPobj->m_charName,2);
				}//END TEAM ELIMINATION RULE SPECIFIC
				if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,newPobj->m_currentWorldID,WORLDRULE_ELIMINATION,&param1))
				{//FFA ARENA RULE SPECIFIC
					if(!m_sharedInterfaceRef->m_poiSystem->Get3DPositionRandomInChannel_safe(newPobj->m_currentWorldID,&playerObj->m_3dPosition))
							m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionRandomInChannel_safe() FAILED Elimination:"+playerObj->m_charName,2);
					else
					{
							newPobj->m_allowLargeMovement = true;
							CString dynScript;
							dynScript.Format("LocalTeleport(%i,%i,%i)",(int)playerObj->m_3dPosition.x,(int)(playerObj->m_3dPosition.y+1.0f),(int)playerObj->m_3dPosition.z);
							m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionRandomInChannel_safe() Elimination success sending script:"+dynScript,2);
							m_sharedInterfaceRef->SendClientScript(newPobj,dynScript,"");//move client script
					}
				}//end FFA ARENA RULE SPECIFIC
				if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,newPobj->m_currentWorldID,WORLDRULE_DEATHMATCH,&param1))
				{//DEATHMATCH ARENA RULE SPECIFIC
					if(!m_sharedInterfaceRef->m_poiSystem->Get3DPositionRandomInChannel_safe(newPobj->m_currentWorldID, &playerObj->m_3dPosition))
						m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionRandomInChannel_safe() FAILED DM: " + playerObj->m_charName, 2);
					else
					{
						newPobj->m_allowLargeMovement = true;
						CString dynScript;
						dynScript.Format("LocalTeleport(%i, %i, %i)", (int)playerObj->m_3dPosition.x, (int)(playerObj->m_3dPosition.y + 1.0f), (int)playerObj->m_3dPosition.z);
						m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionRandomInChannel_safe() DM success sending script: " + dynScript, 2);
						m_sharedInterfaceRef->SendClientScript(newPobj, dynScript, "");//move client script
						CopyMemory(&newPobj->m_arenaRespawnPosition, &playerObj->m_3dPosition, sizeof(D3DVECTOR));
						newPobj->m_arenaRespawnWorldName = playerObj->m_currentWorldMap;
						for(int i = 0; i < ARENA_TRACKING; i++)
							newPobj->m_arenaInfo[i] = playerObj->m_arenaInfo[i];
					}
				}//end DEATHMATCH ARENA RULE SPECIFIC
				if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,newPobj->m_currentWorldID,WORLDRULE_TEAM_DEATHMATCH,&param1))
				{//TEAM DEATHMATCH ARENA RULE SPECIFIC
					//BOOL res = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->SortToTeam(newPobj->m_currentWorldID, param1, newPobj);//player shouldn't be added yet
					//if(res)

					// Players should already be assigned to teams
					if (newPobj->m_teamTemp >= 0)
					{
						//m_sharedInterfaceRef->m_eventLog->LogByPriority("SortToTeam() success: " + newPobj->m_charName + " to team: ", (DWORD)newPobj->m_teamTemp, 2);
						//sort to team location
						if(!m_sharedInterfaceRef->m_poiSystem->Get3DPositionInOrderInChannel_safe(newPobj->m_currentWorldID, newPobj->m_teamTemp, &playerObj->m_3dPosition))
							m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionInOrderInChannel_safe() FAILED teamDM: ", (DWORD)newPobj->m_teamTemp, 2);
						else
						{
							newPobj->m_allowLargeMovement = true;
							CString dynScript;
							dynScript.Format("LocalTeleport(%i, %i, %i)", (int)playerObj->m_3dPosition.x, (int)(playerObj->m_3dPosition.y + 1.0f), (int)playerObj->m_3dPosition.z);
							m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionInOrderInChannel_safe() success sending script: " + dynScript, (DWORD)newPobj->m_teamTemp,2);
							m_sharedInterfaceRef->SendClientScript(newPobj, dynScript, "");//move client script
							CopyMemory(&newPobj->m_arenaRespawnPosition, &playerObj->m_3dPosition, sizeof(D3DVECTOR));
							newPobj->m_arenaRespawnWorldName = playerObj->m_currentWorldMap;
							for(int i = 0; i < ARENA_TRACKING; i++)
								newPobj->m_arenaInfo[i] = playerObj->m_arenaInfo[i];
						}
					}
					else
						m_sharedInterfaceRef->m_eventLog->LogByPriority("SortToTeam() FAILED player: " + newPobj->m_charName, 2);
				}//end TEAM DEATHMATCH ARENA RULE SPECIFIC
				if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,newPobj->m_currentWorldID,WORLDRULE_DOMINATION,&param1))
				{//DOMINATION ARENA RULE SPECIFIC
					if(!m_sharedInterfaceRef->m_poiSystem->Get3DPositionRandomInChannel_safe(newPobj->m_currentWorldID, &playerObj->m_3dPosition))
						m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionRandomInChannel_safe() FAILED Domination: " + playerObj->m_charName, 2);
					else
					{
						newPobj->m_allowLargeMovement = true;
						CString dynScript;
						dynScript.Format("LocalTeleport(%i, %i, %i)", (int)playerObj->m_3dPosition.x, (int)(playerObj->m_3dPosition.y + 1.0f), (int)playerObj->m_3dPosition.z);
						m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionRandomInChannel_safe() Domination success sending script: " + dynScript, 2);
						m_sharedInterfaceRef->SendClientScript(newPobj, dynScript, "");//move client script
						CopyMemory(&newPobj->m_arenaRespawnPosition, &playerObj->m_3dPosition, sizeof(D3DVECTOR));
						newPobj->m_arenaRespawnWorldName = playerObj->m_currentWorldMap;
						for(int i = 0; i < ARENA_TRACKING; i++)
							newPobj->m_arenaInfo[i] = playerObj->m_arenaInfo[i];
					}
				}//end DOMINATION ARENA RULE SPECIFIC
				if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,newPobj->m_currentWorldID,WORLDRULE_TEAM_DOMINATION,&param1))
				{//TEAM DOMINATION ARENA RULE SPECIFIC
					//BOOL res = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->SortToTeam(newPobj->m_currentWorldID, param1, newPobj);//player shouldn't be added yet
					//if(res)
					if (newPobj->m_teamTemp >= 0) 
					{
						//m_sharedInterfaceRef->m_eventLog->LogByPriority("SortToTeam() success: " + newPobj->m_charName + " to team: ", (DWORD)newPobj->m_teamTemp, 2);
						//sort to team location
						if(!m_sharedInterfaceRef->m_poiSystem->Get3DPositionInOrderInChannel_safe(newPobj->m_currentWorldID, newPobj->m_teamTemp, &playerObj->m_3dPosition))
							m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionInOrderInChannel_safe() FAILED teamDomination: ", (DWORD)newPobj->m_teamTemp, 2);
						else
						{
							newPobj->m_allowLargeMovement = true;
							CString dynScript;
							dynScript.Format("LocalTeleport(%i, %i, %i)", (int)playerObj->m_3dPosition.x, (int)(playerObj->m_3dPosition.y + 1.0f), (int)playerObj->m_3dPosition.z);
							m_sharedInterfaceRef->m_eventLog->LogByPriority("Get3DPositionInOrderInChannel_safe() success sending script: " + dynScript, (DWORD)newPobj->m_teamTemp,2);
							m_sharedInterfaceRef->SendClientScript(newPobj, dynScript, "");//move client script
							CopyMemory(&newPobj->m_arenaRespawnPosition, &playerObj->m_3dPosition, sizeof(D3DVECTOR));
							newPobj->m_arenaRespawnWorldName = playerObj->m_currentWorldMap;
							for(int i = 0; i < ARENA_TRACKING; i++)
								newPobj->m_arenaInfo[i] = playerObj->m_arenaInfo[i];
						}
					}
					else
						m_sharedInterfaceRef->m_eventLog->LogByPriority("SortToTeam() FAILED player: " + newPobj->m_charName, 2);
				}//end TEAM DOMINATION ARENA RULE SPECIFIC

				CopyMemory(&newPobj->m_3dPosition,&playerObj->m_3dPosition,sizeof(D3DVECTOR));
				CopyMemory(&newPobj->m_Last3dPosition,&playerObj->m_3dPosition,sizeof(D3DVECTOR));
				CopyMemory(&newPobj->m_Target3dPosition,&playerObj->m_3dPosition,sizeof(D3DVECTOR));

				  if(newPobj->m_stasis == 2)
				  {//aquire alternat internal cfg
                    //disarm all
                    CAIObj * aiReferencePtr = m_sharedInterfaceRef->m_aiList->GetByIndex(newPobj->m_altCfgIndex);
					if(aiReferencePtr)
					{//reconfig energy
                        if(aiReferencePtr->m_physicsModuleOverride)
                           newPobj->m_phyRef = aiReferencePtr->m_physicsModuleOverride;

                        if(aiReferencePtr->m_overriderMaxEngery > 0){//energy overide
						    newPobj->m_overridedEnergy = aiReferencePtr->m_overriderMaxEngery;
                            if(newPobj->m_stats)
				               newPobj->m_maxEnergy        = newPobj->m_stats->GetBenefitBonus(1,newPobj->m_currentInventory) + aiReferencePtr->m_overriderMaxEngery;
				            else
                               newPobj->m_maxEnergy        = aiReferencePtr->m_overriderMaxEngery;
							
							if(newPobj->m_energy > newPobj->m_maxEnergy)
				               newPobj->m_energy = newPobj->m_maxEnergy;
						}//end energy overide
					    //DIM CFG
				        if(DBPtr->List_skeletalObject->m_deformableMeshArrayCount>0)
						{//valid cfg
				           newPobj->m_deformableMeshCount   = DBPtr->List_skeletalObject->m_deformableMeshArrayCount;
				           if(newPobj->m_deformableCfg)
					          delete [] newPobj->m_deformableCfg;
				              newPobj->m_deformableCfg = 0;
						   newPobj->m_deformableCfg = new int[DBPtr->List_skeletalObject->m_deformableMeshArrayCount];
                           for(int dloop=0;dloop<newPobj->m_deformableMeshCount;dloop++)
						   {//loop
                              if(dloop < aiReferencePtr->m_spawnCfgCount)
				                 newPobj->m_deformableCfg[dloop] = aiReferencePtr->m_spawnCfg[dloop];
						   }//end loop
						}//end valid cfg
					}//end reconfig energy

                    if(!newPobj->m_reconfigInven)
					{//need to build reconfig menu because i shifted into not pewrsisting partal respawn or regen
						 newPobj->m_reconfigInven = new CInventoryObjList();
					
					 if(newPobj->m_currentInventory)
					 for(POSITION iPos = newPobj->m_currentInventory->GetHeadPosition();iPos != NULL;)
					 {
					     CInventoryObj * invPtr = (CInventoryObj *) newPobj->m_currentInventory->GetNext(iPos);
						 if(invPtr->m_armed)
						 {
							CInventoryObj * newInvPtr = NULL;
							invPtr->Clone(&newInvPtr);
							newPobj->m_reconfigInven->AddTail(newInvPtr);
						 }
						 invPtr->m_armed = FALSE;
					 }
                     if(aiReferencePtr)
					 {//configure through bot reference
						if(aiReferencePtr->m_inventoryCfg)
                        for(POSITION aiPosLoc = aiReferencePtr->m_inventoryCfg->GetHeadPosition();aiPosLoc != NULL;)
						{
							CInventoryObj * invPtr = (CInventoryObj *)aiReferencePtr->m_inventoryCfg->GetNext(aiPosLoc);
							if(invPtr->m_armed){
							   CInventoryObj * existingPtr = newPobj->m_currentInventory->VerifyInventory(invPtr->m_itemData->m_globalID);
							   if(!existingPtr)
							       newPobj->m_currentInventory->AddInventoryItem(1,TRUE,invPtr->m_itemData);  
							   else
								   existingPtr->m_armed = TRUE;
							}
						}
					 }//end configure through bot reference
					}//need to build reconfig menu because i shifted into not pewrsisting partal respawn or regen
				  }//end aquire alternat internal cfg

				int durationSave0 = newPobj->m_bonus[0].m_bonusDuration;
				int durationSave1 = newPobj->m_bonus[1].m_bonusDuration;
				int durationSave2 = newPobj->m_bonus[2].m_bonusDuration;

			    m_sharedInterfaceRef->UpdateClientStatistics(newPobj,idFromLoc);
				
				newPobj->m_bonus[0].m_bonusDuration = durationSave0;
				newPobj->m_bonus[1].m_bonusDuration = durationSave1;
				newPobj->m_bonus[2].m_bonusDuration = durationSave2;

				m_sharedInterfaceRef->UpdateClientRecentQuestJournalEntry(newPobj);
				if(playerObj->m_justDied == 1)
				{
				   m_sharedInterfaceRef->RequestMurdersToReport(newPobj);//finish up any pending reports
				   playerObj->m_justDied = 0;
				}
				//update ammo
				CInventoryObj * weaponLink = newPobj->m_currentInventory->GetArmedInvenItemByLocationID(12);
	            if(weaponLink)
				{//update current armed status
					if(weaponLink->m_itemData->m_secsToGo)
					{
						weaponLink->m_itemData->m_weaponDepletionTimer = weaponLink->m_itemData->m_secsToGo;
						weaponLink->m_itemData->m_tempDepletionHolder = GetTickCount();
					}
				      m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
															idFromLoc,
															27,//attribute
															weaponLink->m_itemData->m_maxCharges,//
															0,
															weaponLink->m_itemData->m_chargeCurrent,
															weaponLink->m_itemData->m_shotsPerFire,
															newPobj);
					  newPobj->m_rechargeTimer = weaponLink->m_itemData->m_weaponDepletionTimer;
					  /*m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
															idFromLoc,
															41,//attribute
															weaponLink->m_itemData->m_weaponDepletionTimer,//
															0,
															0,
															0,
															newPobj);*/
				}//end update current armed status 
                //increment for user connection status on zone
				if(!m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->IncrementZoneConnections_safe(newPobj->m_currentWorldID))
				{
					m_sharedInterfaceRef->m_eventLog->LogByPriority("IncrementZoneConnections_safe() m_currentWorldMap:"+newPobj->m_currentWorldMap+" override:"+CSToString(channelOverride)+" FAILED channel:",(DWORD)newPobj->m_currentWorldID,2);
					return FALSE;//sever error
				}
		}//end valid attach start inventory

		// IF this world rule is present, we must hide all of the player's non-permanent 
		//	inventory items until they leave this world...
		/*DWORD param1 = 0;
		if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(
			newPobj->m_stencilChannel,
			newPobj->m_currentWorldID,
			WORLDRULE_HIDE_INVENTORY_ON_SPAWN,
			&param1))
		{
			if(!newPobj->m_reconfigInven)
				newPobj->m_reconfigInven = new CInventoryObjList();

			// COPY all non-permanent items into the player's hidden inventory
			if(newPobj->m_reconfigInven->GetCount() == 0)
			{
				POSITION pos = NULL;
				for (pos = newPobj->m_currentInventory->GetHeadPosition(); pos != NULL;)
				{
					CInventoryObj *pItem = dynamic_cast<CInventoryObj*>(newPobj->m_currentInventory->GetNext(pos));
					//Don't save out arena items
					BOOL arenaItem = FALSE;
					for(int i = 0; i < m_sharedInterfaceRef->m_arenaItemCount; i++)
					{
						if(pItem->m_itemData->m_globalID == m_sharedInterfaceRef->m_arenaItems[i])
						{
							arenaItem = TRUE;
							break;
						}
					}
					if (!pItem || arenaItem)
						continue;
					newPobj->m_reconfigInven->AddInventoryItem(pItem->m_quanity, pItem->m_armed, pItem->m_itemData);
				}
				//Remove all by permanent and arena based items
				POSITION posLast = NULL;
				for(POSITION posLoc = newPobj->m_currentInventory->GetHeadPosition(); (posLast = posLoc) != NULL;)
				{
					CInventoryObj *invObj = (CInventoryObj*)newPobj->m_currentInventory->GetNext(posLoc);
					BOOL arenaItem = FALSE;
					for(int i = 0; i < m_sharedInterfaceRef->m_arenaItemCount; i++)
					{
						if(invObj->m_itemData->m_globalID == m_sharedInterfaceRef->m_arenaItems[i])
						{
							arenaItem = TRUE;
							break;
						}
					}
					if(!arenaItem)
					{
						invObj->SafeDelete();
						delete invObj;
						invObj = 0;
						newPobj->m_currentInventory->RemoveAt(posLast);
					}
				}

				if(!newPobj->m_arenaItemsGiven)
					m_sharedInterfaceRef->m_threadedDataProcessor->AddToQue(newPobj->m_currentWorldID,newPobj->m_stencilChannel,35,0);//process closing rules
				
				// REMOVE all the non-permanent items from the player's inventory
			//	newPobj->m_currentInventory->RemoveAllExceptPerminent();
			}
		}
		// OTHERWISE, replace all the player's hidden items back into their regular inventory
		else
		{
			if(newPobj->m_reconfigInven && newPobj->m_reconfigInven->GetCount() > 0)
			{
				// IF there is anything in the hidden inventory, THEN remove all items in the player's current inventory.
				//	We don't want them keeping anything they obtained from the WORLDRULE_HIDE_INVENTORY_ON_SPAWN world.
				//	NOTE: Except for arena rewards OR arena tokens (GLID: 324 & 325)!
				for(POSITION pos = newPobj->m_currentInventory->GetHeadPosition(); pos != NULL;)
				{
					CInventoryObj *pItem = (CInventoryObj*)newPobj->m_currentInventory->GetNext(pos);
					if(pItem)
					{
						if((pItem->m_itemData->m_globalID == 324) || (pItem->m_itemData->m_globalID == 325))
							newPobj->m_reconfigInven->AddInventoryItem(pItem->m_quanity, pItem->m_armed, pItem->m_itemData);
					}
				}
				// Added this here since we aren't putting perm items into the reconfig
				// then we should remove them from the current inventory
				POSITION posLast = NULL;
				for(POSITION posLoc = newPobj->m_currentInventory->GetHeadPosition(); (posLast = posLoc) != NULL;)
				{
					CInventoryObj *invObj = (CInventoryObj*)newPobj->m_currentInventory->GetNext(posLoc);
					if(invObj)
					{
						invObj->SafeDelete();
						delete invObj;
						invObj = 0;
						newPobj->m_currentInventory->RemoveAt(posLast);
					}
				}
				newPobj->m_currentInventory->RemoveAll();

				// COPY all hidden items into the player's accessible inventory
				for(POSITION pos = newPobj->m_reconfigInven->GetHeadPosition(); pos != NULL;)
				{
					CInventoryObj *pItem = dynamic_cast<CInventoryObj*>(newPobj->m_reconfigInven->GetNext(pos));
					if(!pItem) 
						continue;
					newPobj->m_currentInventory->AddInventoryItem(pItem->m_quanity, pItem->m_armed, pItem->m_itemData);
					pItem->SafeDelete();
					SafeDelete(pItem);
				}
				// REMOVE all items from the player's hidden inventory
				newPobj->m_reconfigInven->RemoveAll();
			}
			else if(newPobj->m_arenaItemsGiven)
				newPobj->m_currentInventory->RemoveAll();

			newPobj->m_arenaItemsGiven = FALSE;
		}*/
               
		FinalizeSpawn(newPobj,accountPtr,formatedMessage->currentTrace);
		return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CSpawnSystems::FlagBodyAsPlaced(int handle)
{
  CPlayerObject * pData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(handle);
  if(pData)
  {
     pData->m_bodyNeedToBePlaced = FALSE;
     return TRUE;
  }
  return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL CSpawnSystems::HandleAISpawnMessagesServerSide_safe(LPSPAWN_STRUCT formatedMessage,DPNID idFromLoc)
{
		
	    CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
		if(!accountPtr)
		{
            m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find account by DPNID HandleAISpawnMessagesServerSide_safe()::",(DWORD)idFromLoc);
		    return FALSE;
		}
	    
	    accountPtr->m_lastELtime = (DWORD)-9999999999999;//reset time creep
		accountPtr->m_syncTrackingTime = 0;
        //if ai server dont let him spawn twice either
        
        int worldChannel = formatedMessage->uniInt2;//can trust this, its server side (AI->Management) 
	    if(worldChannel < 0)
		{
			  m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid world selection HandleAISpawnMessagesServerSide_safe()::",accountPtr->m_accountName);
	    }
	   
		accountPtr->m_worldChannelTemp = worldChannel;

        CPlayerObject * aiSearch = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetAIObjectByAliveAndIDandTrace(idFromLoc,formatedMessage->currentTrace,worldChannel);
        if(aiSearch)
	    {
		   m_sharedInterfaceRef->m_eventLog->AddEvent("::AI entity already on server double spawn prevention:","WLDID:"+m_sharedInterfaceRef->m_cabLib.ConvToString(worldChannel) + " " + accountPtr->m_accountName + " Net ID:",(DWORD)formatedMessage->currentTrace);
           m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		   return FALSE;
		}
		//end this player should not be spawned already ignore if true
	    
		CPlayerObject * newPobj = new CPlayerObject();
		newPobj->m_spawnTimeStamp = GetTickCount();
		newPobj->m_murderWearTimeStamp = GetTickCount();
	
		newPobj->m_firemsgsoutbox					= new CPacketObjList();
		newPobj->m_playerChangeInvenOutbox			= new CPacketObjList();
        newPobj->m_playerItemCfgOutbox				= new CPacketObjList();
		newPobj->m_playerEntityCfgOutbox			= new CPacketObjList();
		newPobj->m_playerEntityDamageMgsOutbox		= new CPacketObjList();//lossy Mov pack
   	    newPobj->m_playerEntityGeneralMgsOutbox		= new CPacketObjList();//lossy Mov pack
		newPobj->m_playerEntityDetailedMgsOutbox	= new CPacketObjList();//lossy Mov pack
		newPobj->m_attributeOutbox					= new CPacketObjList();
		newPobj->m_equipMsgOutbox					= new CPacketObjList();//non loss
	    newPobj->m_txtMsgOutbox						= new CPacketObjList();//non loss
		newPobj->m_statisticsOutbox					= new CPacketObjList();//non loss
		newPobj->m_miscOutBox						= new CPacketObjList();
		newPobj->m_entityAddressRef					= NULL;

		newPobj->m_currentID	      = idFromLoc;//use for routeing messages only
		newPobj->m_visualDistanceRef  = m_sharedInterfaceRef->m_netComObject->m_serverCalculateRadius;

        //get database and give the start inventory to player if first time spawning/creating character
        CMovementObj * DBPtr = m_sharedInterfaceRef->m_entObjList->GetObjByIndex(formatedMessage->dbIndex);
		if(!DBPtr){//invalid spawn index
			SafeDelete(newPobj);

            m_sharedInterfaceRef->m_eventLog->AddEvent("Player attempted to spawn unknown databse entity");
			m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
            return FALSE;
		}//end invalid spawn index
		else{//valid attach start inventory
			if(accountPtr->m_aiServer)
			{//ai server spawn
			  newPobj->m_dynamicRadius     = DBPtr->List_boundingRadius;
			  newPobj->m_admin             = accountPtr->m_administratorAccess;
			  newPobj->m_gm                = accountPtr->m_gm;
			  newPobj->m_spawnAbility      = accountPtr->m_canSpawn;
			  newPobj->m_dbCfg             = formatedMessage->dbIndex;
			  newPobj->m_aiTraceTemp       = formatedMessage->currentTrace;
			  newPobj->m_aiControlled      = TRUE;
              newPobj->m_maxEnergy         = DBPtr->List_maxEnergy;
              newPobj->m_energy            = DBPtr->List_maxEnergy;
			  newPobj->m_currentWorldID    = accountPtr->m_worldChannelTemp;
              newPobj->m_currentWorldMap   = formatedMessage->worldName;
			  if(DBPtr->m_skills)
			     DBPtr->m_skills->Clone(&newPobj->m_skills);//assign start skills
			  else
			  newPobj->m_skills             = new CSkillObjectList();//give default
			  newPobj->m_currentInventory   = new CInventoryObjList();
			  newPobj->m_charName           = DBPtr->List_name;
			  newPobj->m_curEDBIndex        = newPobj->m_dbCfg;

			  newPobj->m_phyRef             = DBPtr->List_movementDB;

			  newPobj->m_noteriety = new CNoterietyObj();
			  newPobj->m_noteriety->m_currentAgressors = new CAggressorObjList();

              CAIObj * aiReferencePtr = m_sharedInterfaceRef->m_aiList->GetByIndex(formatedMessage->uniInt3);
              newPobj->m_aiCfgReference = formatedMessage->uniInt3;
			  
			  if(aiReferencePtr)
			  {//configure through bot reference
			     if(aiReferencePtr->m_physicsModuleOverride)
                    newPobj->m_phyRef = aiReferencePtr->m_physicsModuleOverride;


				 if(aiReferencePtr->List_maxSightRadius < 4000)
                    newPobj->m_visualDistanceRef = aiReferencePtr->List_maxSightRadius;//specific distancing
				 else
				 {
					m_sharedInterfaceRef->m_eventLog->AddEvent("AI CFG sight sys is greater than 4000?? WTF:"," ",formatedMessage->uniInt3);
                    aiReferencePtr->List_maxSightRadius = 4000;
				 }

				 if(aiReferencePtr->m_overriderMaxEngery > 0){//energy overide
					newPobj->m_overridedEnergy = aiReferencePtr->m_overriderMaxEngery;
                    if(newPobj->m_stats)
				       newPobj->m_maxEnergy        = newPobj->m_stats->GetBenefitBonus(1,newPobj->m_currentInventory) + aiReferencePtr->m_overriderMaxEngery;
				    else
                       newPobj->m_maxEnergy        = aiReferencePtr->m_overriderMaxEngery;
							
				       newPobj->m_energy = newPobj->m_maxEnergy;
				 }//end energy overide
                 //NAME
				 newPobj->m_expTempAIRef = aiReferencePtr->m_curExperienceLevel;
                 newPobj->m_charName = aiReferencePtr->m_nameOvveride;
				 newPobj->m_visName = aiReferencePtr->m_nameOvveride;
				 //CURRENCY
				 if(aiReferencePtr->m_enableCashApplied)
				 {//enabled cash
                    int range = aiReferencePtr->m_maxCash - aiReferencePtr->m_minCash;
                    if(range > 0)
					{//valid range
		               int final = rand()%range;
					   m_sharedInterfaceRef->m_currencySystem->AddCurrency(m_sharedInterfaceRef->m_itemManager,newPobj->m_currentInventory,-1,aiReferencePtr->m_minCash+final);
					}//end valid range
		         }//end enabled cash
                 //INVENTORY
				 BOOL useOvveride = FALSE;
				 
                 if(aiReferencePtr->m_inventoryCfg)
                    if(aiReferencePtr->m_inventoryCfg->GetCount() > 0)
					   useOvveride = TRUE;

				 if(useOvveride)
				 {// valid inventory db
                    for(POSITION invCfgPos = aiReferencePtr->m_inventoryCfg->GetHeadPosition();invCfgPos != NULL;)
					{//inv loop
						CInventoryObj * invObj = (CInventoryObj *) aiReferencePtr->m_inventoryCfg->GetNext(invCfgPos);
				        if(invObj->m_armed)
						   newPobj->m_currentInventory->AddInventoryItem(1,invObj->m_armed,invObj->m_itemData);//add inven obj
						else
						{
                           int resultThrow = rand()%100;
                           if(resultThrow < aiReferencePtr->m_percentageOfDropping)
						      newPobj->m_currentInventory->AddInventoryItem(1,invObj->m_armed,invObj->m_itemData);//add inven obj	
						}
					}//end inv loop
				 }//end valid inventory db
				 else if(DBPtr->m_autoCfg)
				 {
					newPobj->m_charName = DBPtr->List_name;
				    for(POSITION posLoc = DBPtr->m_autoCfg->GetHeadPosition();posLoc != NULL;)
					{//inital cfg list
                       CCfgObj * cfgPtr = (CCfgObj *)DBPtr->m_autoCfg->GetNext(posLoc);
                       if(cfgPtr->m_quanity > 0)
					   {//set inventory		
				        CItemObj * invObj = m_sharedInterfaceRef->m_itemManager->GetItemByID_LOS(cfgPtr->m_globalCFG);
                        if(!invObj)
					         continue;

				        for(int iLoop=0;iLoop<cfgPtr->m_quanity;iLoop++)
                             newPobj->m_currentInventory->AddInventoryItem(1,cfgPtr->m_armed,invObj);

                        m_sharedInterfaceRef->m_itemManager->Unlock();
                         

					   }//end set inventory
					}//end inital cfg list
				 }
                 //DIM CFG
				 if(DBPtr->List_skeletalObject->m_deformableMeshArrayCount>0)
				 {//valid cfg
				    newPobj->m_deformableMeshCount   = DBPtr->List_skeletalObject->m_deformableMeshArrayCount;
                    if(newPobj->m_deformableCfg)
					   delete [] newPobj->m_deformableCfg;
					   newPobj->m_deformableCfg = 0;

				    newPobj->m_deformableCfg = new int[DBPtr->List_skeletalObject->m_deformableMeshArrayCount];
                    for(int dloop=0;dloop<newPobj->m_deformableMeshCount;dloop++)
					{//loop
                       if(dloop < aiReferencePtr->m_spawnCfgCount)
				          newPobj->m_deformableCfg[dloop] = aiReferencePtr->m_spawnCfg[dloop];
					}//end loop
				 }//end valid cfg
	          }//end configure through bot reference
			  else if(DBPtr->m_autoCfg)//configure through edb old school
			  for(POSITION posLoc = DBPtr->m_autoCfg->GetHeadPosition();posLoc != NULL;)
			  {//inital cfg list
                CCfgObj * cfgPtr = (CCfgObj *)DBPtr->m_autoCfg->GetNext(posLoc);
                if(cfgPtr->m_quanity > 0)
				{//set inventory		
				   //get inventory item
				   CItemObj * invObj = m_sharedInterfaceRef->m_itemManager->GetItemByID_LOS(cfgPtr->m_globalCFG);
                   if(!invObj)
					   continue;

				   for(int iLoop=0;iLoop<cfgPtr->m_quanity;iLoop++)
                       newPobj->m_currentInventory->AddInventoryItem(1,cfgPtr->m_armed,invObj);  

                   m_sharedInterfaceRef->m_itemManager->Unlock();
				}//end set inventory
			  }//end inital cfg list
			}//end ai server spawn
		}//end valid attach start inventory
      
	    FinalizeSpawn(newPobj,accountPtr,formatedMessage->currentTrace);
	
		m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();

	    return TRUE;
}

//------------------------------------------------------------------------------------//
void CSpawnSystems::FinalizeSpawn(CPlayerObject * newPobj,CAccountObject * accountPtr,int trace)
{
       newPobj->m_networkAssignedID = m_sharedInterfaceRef->m_netComObject->GetFreeRuntimeEntityNetworkID();
	   m_sharedInterfaceRef->m_eventLog->LogByPriority("Spawned PC-AI() NetID:",(DWORD)newPobj->m_networkAssignedID,3);
	   assert(newPobj->m_networkAssignedID != -1);  
	   newPobj->m_inGame            = TRUE;
	   newPobj->m_skillUseTimeStamp = GetTickCount();//interupt his skill use
	   newPobj->m_timeLastUpdated   = GetTickCount();//

	   
       
	   //send communication data / networkassigned id
	       m_sharedInterfaceRef->SendAttribMsg( DPNSEND_GUARANTEED,
												newPobj->m_currentID,
												8,
												newPobj->m_networkAssignedID,
												trace,
												newPobj->m_energy,
												newPobj->m_maxEnergy,
												newPobj);

		     if(newPobj->m_bonus[1].m_bonusType > 0)
			 {
				 // Send the Update to Client Side for Visual Display
				 m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
													   newPobj->m_currentID,					// Player ID
												       31,										// Attribute (Stat)
													   newPobj->m_bonus[1].m_bonusDuration,		// Current Timer
													   newPobj->m_bonus[1].m_bonusType - 1,		// Visual Display
													   newPobj->m_bonus[1].m_maxDuration,		// Max Timer
													   14,										// Menu to Use
													   newPobj);								// Player
				 newPobj->m_StatStamp = GetTickCount();
			 }

			 if(newPobj->m_bonus[2].m_bonusType > 0)
			 {
				 // Send the Update to Client Side for Visual Display
				 m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
													   newPobj->m_currentID,					// Player ID
												       32,										// Attribute (Resist)
													   newPobj->m_bonus[2].m_bonusDuration,		// Current Timer
													   newPobj->m_bonus[2].m_bonusType - 6,		// Visual Display
													   newPobj->m_bonus[2].m_maxDuration,		// Max Timer
													   15,										// Menu to Use
													   newPobj);								// Player
				 newPobj->m_ResistStamp = GetTickCount();
			 }

			 if(newPobj->m_bonus[0].m_bonusType > 0)
			 {
				 // Send the Update to Client Side for Visual Display
				 m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
													   newPobj->m_currentID,					// Player ID
												       30,										// Attribute (Damage)
													   newPobj->m_bonus[0].m_bonusDuration,		// Current Timer
													   newPobj->m_bonus[0].m_bonusType,			// Visual Display
													   newPobj->m_bonus[0].m_maxDuration,		// Max Timer
													   0,										// Menu to Use
													   newPobj);								// Player
			 }
	 
	   int totalAssets = m_sharedInterfaceRef->m_currencySystem->EvalCurrency(newPobj->m_currentInventory);
	   m_sharedInterfaceRef->SendAttribMsg( DPNSEND_GUARANTEED,
											newPobj->m_currentID,
											19,
											totalAssets,
											0,
											0,
											0,
											newPobj);
	   
	   m_sharedInterfaceRef->SendGuerenteedCycleMsgToClient(newPobj);//send cache

	   DWORD dwParam = 0;
	   int iWorldRuleType = -1;
	   if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,
			newPobj->m_currentWorldID,
			WORLDRULE_TEAM_ELIMINATION,
			&dwParam))
		{
			newPobj->m_inArena = WORLDRULE_TEAM_ELIMINATION;
			iWorldRuleType = WORLDRULE_TEAM_ELIMINATION;
		}
		else if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,
			newPobj->m_currentWorldID,
			WORLDRULE_ELIMINATION,
			&dwParam))
		{
			newPobj->m_inArena = WORLDRULE_ELIMINATION;
			iWorldRuleType = WORLDRULE_ELIMINATION;
		}
	    else if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,
			newPobj->m_currentWorldID,
			WORLDRULE_DEATHMATCH,
			&dwParam))
		{
			newPobj->m_inArena = WORLDRULE_DEATHMATCH;
			iWorldRuleType = WORLDRULE_DEATHMATCH;
		}
		else if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,
			newPobj->m_currentWorldID,
			WORLDRULE_TEAM_DEATHMATCH,
			&dwParam))
		{
			newPobj->m_inArena = WORLDRULE_TEAM_DEATHMATCH;
			iWorldRuleType = WORLDRULE_TEAM_DEATHMATCH;
		}
		else if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,
			newPobj->m_currentWorldID,
			WORLDRULE_DOMINATION,
			&dwParam))
		{
			newPobj->m_inArena = WORLDRULE_DOMINATION;
			iWorldRuleType = WORLDRULE_DOMINATION;
		}
		else if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,
			newPobj->m_currentWorldID,
			WORLDRULE_TEAM_DOMINATION,
			&dwParam))
		{
			newPobj->m_inArena = WORLDRULE_TEAM_DOMINATION;
			iWorldRuleType = WORLDRULE_TEAM_DOMINATION;
		}
		else
		{
			//Reset all valid items for arenas
			for(int i=0; i<ARENA_TRACKING; i++)
			{
				if(accountPtr && accountPtr->m_characterOnAccount)
					accountPtr->m_characterOnAccount->m_arenaInfo[i] = 0;
				newPobj->m_arenaInfo[i] = 0;
			}
			if(accountPtr && accountPtr->m_characterOnAccount)
				accountPtr->m_characterOnAccount->m_teamTemp = -1;
			newPobj->m_teamTemp = -1;
		}

		// Determine the synchro start time (UTC) for arena battle
		FILETIME ft;
		::GetSystemTimeAsFileTime(&ft);
		DWORD dwArenaDuration = 0;

		// Clear arena info for this player character for this spawn event
		if(!newPobj->m_arenaInfo[3] && newPobj->m_arenaInfo[0])
		{
			for(int i=0; i<ARENA_TRACKING; i++)
				newPobj->m_arenaInfo[i] = 0;
		}
		
		// Get the arena duration if there is a WORLDRULE_TIMELIMIT...
		BOOL bResult = m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(newPobj->m_stencilChannel,
			newPobj->m_currentWorldID,
			WORLDRULE_TIMELIMIT,
			&dwArenaDuration);

		// Set the arena type in the player's arena info array
		if((iWorldRuleType == WORLDRULE_TEAM_ELIMINATION)
		|| (iWorldRuleType == WORLDRULE_ELIMINATION))
		{
			// GET world available object
		   CWorldAvailableObj *pWorld = m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(newPobj->m_currentWorldID);

			ARENA_MESSAGE msg;
			newPobj->m_arenaInfo[0] = iWorldRuleType;
			msg.ArenaMessage = ARENA_BEGIN;
			msg.ArenaType = newPobj->m_arenaInfo[0];
			msg.Score = ft.dwHighDateTime; // Server Time (UTC, high 32 bits)
			msg.Kills = ft.dwLowDateTime; // Server Time (UTC, low 32 bits)
			msg.Deaths = static_cast<int>(dwArenaDuration); // Arena Duration (in milliseconds)
			msg.Damage = 0;
			msg.Reward = 0;
			m_sharedInterfaceRef->SendArenaMessage(newPobj, msg);

			// HANDLE arena betting ...
			if (pWorld && (newPobj->m_arenaBetAmount > 0))
			{
				// IF player is a bettor, REMOVE bet amount from player's currency
				//	IF this fails, the player reverts to a non-bettor
				if (m_sharedInterfaceRef->m_currencySystem->RemoveCurrencyIfEnoughExists(
					m_sharedInterfaceRef->m_itemManager,
					newPobj->m_currentInventory,
					-1,
					newPobj->m_arenaBetAmount) == 0)
					newPobj->m_arenaBetAmount = 0;

				// REMOVE the player's arena bet from their total currency and
				//	ADD it to the arena betting pool
				pWorld->m_arenaBetPool += newPobj->m_arenaBetAmount;
				++pWorld->m_numArenaBettors;
			}
			else
				newPobj->m_arenaBetAmount = 0;
		}
		else if(((iWorldRuleType == WORLDRULE_DEATHMATCH)
			|| (iWorldRuleType == WORLDRULE_TEAM_DEATHMATCH)
			|| (iWorldRuleType == WORLDRULE_DOMINATION)
			|| (iWorldRuleType == WORLDRULE_TEAM_DOMINATION))
			&& !newPobj->m_arenaInfo[3] /* Deaths */)
		{
			// GET world available object
		   CWorldAvailableObj *pWorld = m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(newPobj->m_currentWorldID);

			ARENA_MESSAGE msg;
			newPobj->m_arenaInfo[0] = iWorldRuleType;
			msg.ArenaMessage = ARENA_BEGIN;
			msg.ArenaType = newPobj->m_arenaInfo[0];
			msg.Score = ft.dwHighDateTime; // Server Time (UTC, high 32 bits)
			msg.Kills = ft.dwLowDateTime; // Server Time (UTC, low 32 bits)
			msg.Deaths = static_cast<int>(dwArenaDuration); // Arena Duration (in milliseconds)
			msg.Damage = 0;
			msg.Reward = 0;
			m_sharedInterfaceRef->SendArenaMessage(newPobj, msg);

			// HANDLE arena betting ...
			if (pWorld && (newPobj->m_arenaBetAmount > 0))
			{
				// IF player is a bettor, REMOVE bet amount from player's currency
				//	IF this fails, the player reverts to a non-bettor
				if (m_sharedInterfaceRef->m_currencySystem->RemoveCurrencyIfEnoughExists(
					m_sharedInterfaceRef->m_itemManager,
					newPobj->m_currentInventory,
					-1,
					newPobj->m_arenaBetAmount) == 0)
					newPobj->m_arenaBetAmount = 0;
				
				// REMOVE the player's arena bet from their total currency and
				//	ADD it to the arena betting pool
				pWorld->m_arenaBetPool += newPobj->m_arenaBetAmount;
				++pWorld->m_numArenaBettors;
			}
		}

       if(!m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Add_safe(newPobj))
		   m_sharedInterfaceRef->m_eventLog->AddEvent("***extreme (m_runtimePlayersOnServer) Add() error in spawn spawnsystems.cpp ln:548:"+accountPtr->m_accountName);	
}

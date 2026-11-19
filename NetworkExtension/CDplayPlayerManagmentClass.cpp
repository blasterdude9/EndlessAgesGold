#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CDplayPlayerClass.h"
#include "sharednet.h"
//Notes
//DPNID = DWORD value range
//0 to 4,294,967,295
//
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------//
//-----class is for persisting characters over a network-----//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CPlayerObject::CPlayerObject()
{
	m_tradeOfferTimeStamp	= GetTickCount();
	m_tradeOffer			= NULL;
	m_invunerableDuration	= 0;
	m_tradeTargetID			= -1; 
	m_tradeAccepted			= FALSE;
	m_tradePendingTarget	= -1;
	m_tradePendingRequest   = -1;
	m_teamTemp				= -1;
	m_instanceType			= 0;
	m_phyRef				= NULL;
	m_deaf					= 0; 
	m_mute					= 0;
	m_deafMuteNotify		= 0;
	m_weaponPass			= 0;
	m_entityAddressRef		= NULL;
    DWORD curTime			= GetTickCount();
	m_reflectLevel			= 0.0f;
    m_upVector.x			= 0.0f;
	m_upVector.y			= 1.0f;
	m_upVector.z			= 0.0f;
    m_dirVector.x			= 0.0f;
	m_dirVector.y			= 0.0f;
	m_dirVector.z			= 1.0f;
    m_3dPosition.x			= 0.0f;
	m_3dPosition.y			= 0.0f;
	m_3dPosition.z			= 0.0f;
	m_LastLast3dPosition.x	= 0.0f;
	m_LastLast3dPosition.y	= 0.0f;
	m_LastLast3dPosition.z	= 0.0f;
    m_freeLookYaw			= 0.0f;
	m_freeLookPitch			= 0.0f;
	m_LastUpVector.x		= 0.0f;
	m_LastUpVector.y		= 1.0f;
	m_LastUpVector.z		= 0.0f;
	m_LastDirVector.x		= 0.0f;
	m_LastDirVector.y		= 0.0f;
	m_LastDirVector.z		= 1.0f;
	m_Last3dPosition.x		= 0.0f;
	m_Last3dPosition.y		= 0.0f;
	m_Last3dPosition.z		= 0.0f;
	m_TargetUpVector.x		= 0.0f;
	m_TargetUpVector.y		= 1.0f;
	m_TargetUpVector.z		= 0.0f;
	m_TargetDirVector.x		= 0.0f;
	m_TargetDirVector.y		= 0.0f;
	m_TargetDirVector.z		= 1.0f;
    m_Target3dPosition.x	= 0.0f;
	m_Target3dPosition.y	= 0.0f;
	m_Target3dPosition.z	= 0.0f;
    m_currentID				= 0;
    m_animationState		= 0;
    m_currentWorldMap		= "Unknown";
	m_arenaRespawnWorldName = "Unknown";
	m_arenaRespawnPosition.x	= 0;
	m_arenaRespawnPosition.y	= 0;
	m_arenaRespawnPosition.z	= 0;
	m_currentInventory		= NULL;
	//m_bankInventory			= NULL;
	m_dbCfg					= -1;//-1 = none
	m_curAnim				= -1;
	m_secondaryAnim			= -1;
	m_inGame				= FALSE;
	m_currentWorldID		= -1;//none
    m_energy				= 100;
	m_originalEDBEnergy		= 10;
    m_maxEnergy				= 100;
	m_networkAssignedID		= -1;//-1 = none
	m_timeDied				= -1;
	m_justSpawned			= TRUE;
	m_timeLastUpdated		= GetTickCount();
	m_regenStamp          = GetTickCount();
	m_ammoRegenStamp	  = GetTickCount();
    m_mountedOnPlayerID   = -1;//-1 not riding anything
    m_mountedByPlayerID   = -1;//-1 == not mounted
	m_accountNumberRef    = -1;
	m_accountNumberRefOld = -1;
	m_alive               = TRUE;
	m_bornSpawnCfgIndex   = 0;
	m_aiControlled        = FALSE;
	m_charName         = "Unknown";
	m_interpolationVar = 1000;
    m_deformableMeshCount = 0;
    m_deformableCfg       = NULL;
	m_skills              = NULL;
	m_dynamicRadius       = 0.0f;
	m_armourClass         = 0;
	m_stats               = NULL;
	m_lastUseItemStamp    = curTime;
	m_raceID              = 0;
	m_inSafeZone          = FALSE;
	m_respawnPosition.x   = 0.0f;
	m_respawnPosition.y   = 0.0f;
	m_respawnPosition.z   = 0.0f;
	m_arenaRespawnPosition.x = 0.0f;
	m_arenaRespawnPosition.y = 0.0f;
	m_arenaRespawnPosition.z = 0.0f;
	m_handle              = -1;
	m_groupDatabase       = NULL;
	m_playerOrigRef       = NULL;
	m_weaponSwitchStamp   = 0;
	m_expTempAIRef        = 1.0f;
	m_stasis              = 0;
	m_positionLock.x      = 0.0f;
    m_positionLock.y      = 0.0f;
    m_positionLock.z      = 0.0f;
	m_rechargeTimer		  = 0;

	m_altCfgIndex         = 0;
	m_curEDBIndex         = 0;
	m_originalEDBCfg      = 0;
	m_reconfigInven       = NULL;
	m_skillUseTimeStamp   = 0;
	m_overridedEnergy     = 0;
	m_lastSurroundingsUpdateTime = 0;
	m_visualDistanceRef   = 0.0f;
	m_aiCfgOfMounted      = -1;
	m_admin               = FALSE;
	//m_accountReference    = NULL;
	m_firemsgsoutbox      = NULL;
	m_playerChangeInvenOutbox  = NULL;
	m_playerItemCfgOutbox = NULL;//lossy Mov pack
	m_playerEntityCfgOutbox = NULL;//lossy Mov pack
	m_playerEntityDamageMgsOutbox = NULL;//lossy Mov pack
	m_playerEntityGeneralMgsOutbox = NULL;//lossy Mov pack
	m_playerEntityDetailedMgsOutbox= NULL;//lossy Mov pack
	m_attributeOutbox = NULL;//Non loss
	m_equipMsgOutbox = NULL;//non loss
	m_txtMsgOutbox = NULL;//non loss
	m_statisticsOutbox = NULL;
	m_miscOutBox       = NULL;
	m_noteriety        = NULL;
	m_clanHandle       = -1;
	m_currentRequestToJoinClanHandle = -1;
	m_countDownToMurderPardon = 0;
	m_spawnTimeStamp   = 0;
	m_lastMsgTimeStamp = 0;
	m_violationsInARow = 0;
	ZeroMemory(&m_movementTrackPosStamp,sizeof(D3DVECTOR));
	m_movementTrackTimeStamp = 0;
    m_networkIDLastAttacker = -1;
    m_timeStampOfLastAttack = curTime;
	m_skillDBisReferenced = FALSE;
	m_currentHighestLevel = 1;
	m_currentHighestExperience = 1.0f;
	m_bodyNeedToBePlaced = FALSE;
	m_currentRequestForAWar = -1;
	m_gm = FALSE;
	m_spawnAbility = FALSE;
	m_invunerableStamp = curTime;
	m_murderWearTimeStamp = curTime;
	m_interpUpdateTemp = curTime;
	m_invunerableTemp = FALSE;
	m_inArena = 0;
	m_factionStanding = -1;
	m_aiTraceTemp = 0;
	m_arenaBasePoints = 0;
	m_pvpKills = 0;
	m_arenaKills = 0;
	m_arenaTotalKills = 0;
	m_physicsOverdTemp = 0;
	m_invunOptionTemp = 0;
	m_currentBounty = 0;
	m_questJournal = NULL;
	m_aiCfgReference = -1;
	m_currentBounty = 0;
	m_justDied = 0;
	m_hostileFlag = 0;
	m_claimedTemp = 0;
	//m_backupStamp = GetTickCount();
	m_questTimer = GetTickCount(); - 60000000;
	m_envDamageStamp = GetTickCount();
	m_speedRating = 350;
	m_throttleStamp = GetTickCount();
	//m_spawnStamp = GetTickCount();
	m_StatStamp = GetTickCount();
	m_ResistStamp = GetTickCount();
	m_deathCount = 0;
	//Misc Damage Variables
	m_DamageModifier = 0; //Actual Amount
	m_DamageChannelOverride = -1; //0-7 resistance Channels
	m_DamageType = 0; //Health (0) or Energy (1) Both(2)
	m_DamageDuration = 0;
	m_DamageStamp = 0;
	m_groupID = 0;
	m_groupStruct.m_squadDataIsDirty = 0;
	m_groupStruct.m_groupLeadership = 0;
	m_groupStruct.m_groupPlacementIndex = 0;
	m_lastGroupUpdate = GetTickCount();
	m_groupBlockTracker = -1;
	m_stencilChannel = 0;
	m_arenaItemsGiven = FALSE;
	//m_arenaInfo = NULL;
	m_arenaInfo = new int[ARENA_TRACKING];
	for(int ai = 0; ai < ARENA_TRACKING; ai++)
		m_arenaInfo[ai] = 0;
	m_bMostWanted = false;

	for(int i = 0; i < 8; i++)
	{
		m_woundChannel[i] = 0;
		m_dotDamageReduction[i] = 0;
		m_dotDamageTakeIncrease[i] = 0;
	}

	// INIT quick slots
	if (this->m_QuickSlots)
	{
		for (int i=0; i<NUM_QUICKBARS; i++)
		{
			for (int j=0; j<NUM_QUICKBAR_SLOTS; j++)
			{
				this->m_QuickSlots[i][j] = -1;
			}
		}
	}
	
	m_allowLargeMovement = false;
	m_dotList = NULL;
	m_dotShieldEffect = 0;
	m_dotJetEffect = 0;
	m_dotLowerMaxHP = 0;
	m_dotLowerMaxMP = 0;

	m_suicideTimeStamp = 0;
	m_suicideTimer = -1;
}


void CPlayerObject::Clone(CPlayerObject ** clone)
{
     CPlayerObject * copyLocal = new CPlayerObject();
     

     CopyMemory(&copyLocal->m_upVector,&m_upVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_dirVector,&m_dirVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_3dPosition,&m_3dPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_LastUpVector,&m_LastUpVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_LastDirVector,&m_LastDirVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_Last3dPosition,&m_Last3dPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_TargetUpVector,&m_TargetUpVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_TargetDirVector,&m_TargetDirVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_Target3dPosition,&m_Target3dPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_respawnPosition,&m_respawnPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_LastLast3dPosition,&m_LastLast3dPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_arenaRespawnPosition, &m_arenaRespawnPosition,sizeof(D3DVECTOR));
	 
	 copyLocal->m_groupStruct		 = m_groupStruct;
	 copyLocal->m_squadName			 = m_squadName;
	 copyLocal->m_groupID			 = m_groupID;
     copyLocal->m_freeLookYaw        = m_freeLookYaw;
	 copyLocal->m_freeLookPitch      = m_freeLookPitch;
     copyLocal->m_animationState     = m_animationState;
     copyLocal->m_currentWorldMap    = m_currentWorldMap;
	 copyLocal->m_dbCfg              = m_dbCfg;
	 copyLocal->m_curAnim            = m_curAnim;
	 copyLocal->m_inGame             = m_inGame;
	 copyLocal->m_currentWorldID     = m_currentWorldID;
     copyLocal->m_energy             = m_energy;
	 copyLocal->m_originalEDBEnergy	 = m_originalEDBEnergy;
     copyLocal->m_maxEnergy          = m_maxEnergy;
	 copyLocal->m_networkAssignedID  = m_networkAssignedID;
	 copyLocal->m_timeDied           = m_timeDied;
	 copyLocal->m_rechargeTimer		 = m_rechargeTimer;
	 copyLocal->m_justSpawned        = m_justSpawned;
	 copyLocal->m_timeLastUpdated    = m_timeLastUpdated;
     copyLocal->m_handle             = m_handle;
     copyLocal->m_mountedByPlayerID  = m_mountedByPlayerID;
	 copyLocal->m_bornSpawnCfgIndex  = m_bornSpawnCfgIndex;
	 copyLocal->m_charName           = m_charName;
	 copyLocal->m_deformableMeshCount= m_deformableMeshCount;
	 copyLocal->m_gm                 = m_gm;
	 copyLocal->m_instanceType		 = m_instanceType;
	 copyLocal->m_stasis             = m_stasis;
     copyLocal->m_altCfgIndex        = m_altCfgIndex;
	 copyLocal->m_curEDBIndex        = m_curEDBIndex;
	 copyLocal->m_originalEDBCfg     = m_originalEDBCfg;
	 //copyLocal->m_reconfigInven      = m_reconfigInven;
	 copyLocal->m_countDownToMurderPardon = m_countDownToMurderPardon;
	 copyLocal->m_clanHandle         = m_clanHandle;
	 copyLocal->m_spawnAbility       = m_spawnAbility;
	 copyLocal->m_invunerableTemp    = m_invunerableTemp;
	 copyLocal->m_arenaBasePoints    = m_arenaBasePoints;
	 copyLocal->m_pvpKills			 = m_pvpKills;
	 copyLocal->m_arenaKills		 = m_arenaKills;
	 copyLocal->m_arenaTotalKills    = m_arenaTotalKills;
	 copyLocal->m_physicsOverdTemp   = m_physicsOverdTemp;
	 copyLocal->m_aiCfgReference     = m_aiCfgReference;
	 copyLocal->m_currentBounty      = m_currentBounty;
	 copyLocal->m_hostileFlag		 = m_hostileFlag;
	 copyLocal->m_claimedTemp        = m_claimedTemp;
	 copyLocal->m_deaf               = m_deaf; 
	 copyLocal->m_mute               = m_mute;
	 copyLocal->m_deafMuteNotify     = m_deafMuteNotify;
	 copyLocal->m_deathCount		 = m_deathCount;
	 copyLocal->m_stencilChannel	 = m_stencilChannel;
	 copyLocal->m_respawnWorldName	 = m_respawnWorldName;
	 copyLocal->m_arenaRespawnWorldName = m_arenaRespawnWorldName;
	 copyLocal->m_teamTemp			 = m_teamTemp;
	 copyLocal->m_reflectLevel		 = m_reflectLevel;
	 copyLocal->m_invunerableDuration= m_invunerableDuration;
	 copyLocal->m_factionStanding	 = m_factionStanding;

	 if(m_reconfigInven)
		m_reconfigInven->Clone(&copyLocal->m_reconfigInven);

	 if(m_currentInventory)
		m_currentInventory->Clone(&copyLocal->m_currentInventory);

	 /*
	 if(m_bankInventory)
		 m_bankInventory->Clone(&copyLocal->m_bankInventory);
		 */
	 for (int i=0; i<this->m_banks.size(); i++)
	 {
		 this->m_banks[i]->Clone(&copyLocal->m_banks[i]);
	 }

	 if(m_stats)
		m_stats->Clone(&copyLocal->m_stats);

	 if(m_noteriety)
		m_noteriety->Clone(&copyLocal->m_noteriety);

	 if(m_skills)
		m_skills->Clone(&copyLocal->m_skills);

	 if(m_dotList)
		m_dotList->Clone(&copyLocal->m_dotList);

	 //Copy Bonus List
	 for(int i = 0; i < MAX_BONUS; i++)
	 {
		 copyLocal->m_bonus[i].m_bonusAmount = m_bonus[i].m_bonusAmount;
		 copyLocal->m_bonus[i].m_bonusDuration = m_bonus[i].m_bonusDuration;
		 copyLocal->m_bonus[i].m_bonusType = m_bonus[i].m_bonusType;
		 copyLocal->m_bonus[i].m_damageMult = m_bonus[i].m_damageMult;
		 copyLocal->m_bonus[i].m_maxDuration = m_bonus[i].m_maxDuration;
	 }

	 if(m_groupDatabase)
		m_groupDatabase->Clone(&copyLocal->m_groupDatabase);

	 if(m_questJournal)
        m_questJournal->Clone(&copyLocal->m_questJournal);

     if(m_deformableMeshCount > 0)
	 {
        copyLocal->m_deformableCfg = new int[m_deformableMeshCount];
        CopyMemory(copyLocal->m_deformableCfg,m_deformableCfg,sizeof(int)*m_deformableMeshCount);
	 }

	 // Arena stuff
	 if(m_arenaInfo)
	 {
		 CopyMemory(copyLocal->m_arenaInfo, m_arenaInfo, sizeof(int) * ARENA_TRACKING);//TODO:  CHANGE TO LOADED FROM SQL TABLE
	 }
	 copyLocal->m_arenaBetAmount = this->m_arenaBetAmount;

	 *clone = copyLocal;
}

void CPlayerObject::CloneMinimum(CPlayerObject ** clone)
{
     CPlayerObject * copyLocal = new CPlayerObject();
     

     CopyMemory(&copyLocal->m_upVector,&m_upVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_dirVector,&m_dirVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_3dPosition,&m_3dPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_LastUpVector,&m_LastUpVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_LastDirVector,&m_LastDirVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_Last3dPosition,&m_Last3dPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_TargetUpVector,&m_TargetUpVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_TargetDirVector,&m_TargetDirVector,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_Target3dPosition,&m_Target3dPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_respawnPosition,&m_respawnPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_LastLast3dPosition,&m_LastLast3dPosition,sizeof(D3DVECTOR));
	 CopyMemory(&copyLocal->m_arenaRespawnPosition, &m_arenaRespawnPosition,sizeof(D3DVECTOR));
	 
	 copyLocal->m_groupStruct		 = m_groupStruct;
	 copyLocal->m_squadName			 = m_squadName;
	 copyLocal->m_groupID			 = m_groupID;
     copyLocal->m_freeLookYaw        = m_freeLookYaw;
	 copyLocal->m_freeLookPitch      = m_freeLookPitch;
     copyLocal->m_animationState     = m_animationState;
     copyLocal->m_currentWorldMap    = m_currentWorldMap;
	 copyLocal->m_dbCfg              = m_dbCfg;
	 copyLocal->m_curAnim            = m_curAnim;
	 copyLocal->m_inGame             = m_inGame;
	 copyLocal->m_currentWorldID     = m_currentWorldID;
     copyLocal->m_energy             = m_energy;
	 copyLocal->m_originalEDBEnergy	 = m_originalEDBEnergy;
     copyLocal->m_maxEnergy          = m_maxEnergy;
	 copyLocal->m_networkAssignedID  = m_networkAssignedID;
	 copyLocal->m_timeDied           = m_timeDied;
	 copyLocal->m_rechargeTimer		 = m_rechargeTimer;
	 copyLocal->m_justSpawned        = m_justSpawned;
	 copyLocal->m_timeLastUpdated    = m_timeLastUpdated;
     copyLocal->m_handle             = m_handle;
     copyLocal->m_mountedByPlayerID  = m_mountedByPlayerID;
	 copyLocal->m_bornSpawnCfgIndex  = m_bornSpawnCfgIndex;
	 copyLocal->m_charName           = m_charName;
	 copyLocal->m_deformableMeshCount= m_deformableMeshCount;
	 copyLocal->m_stasis             = m_stasis;
     copyLocal->m_altCfgIndex        = m_altCfgIndex;
	 copyLocal->m_curEDBIndex        = m_curEDBIndex;
	 copyLocal->m_originalEDBCfg     = m_originalEDBCfg;
	 //copyLocal->m_reconfigInven      = m_reconfigInven;
	 copyLocal->m_countDownToMurderPardon = m_countDownToMurderPardon;
	 copyLocal->m_clanHandle         = m_clanHandle;
	 copyLocal->m_gm                 = m_gm;
	 copyLocal->m_instanceType		 = m_instanceType;
	 copyLocal->m_spawnAbility       = m_spawnAbility;
	 copyLocal->m_arenaBasePoints    = m_arenaBasePoints;
	 copyLocal->m_pvpKills			 = m_pvpKills;
	 copyLocal->m_arenaKills		 = m_arenaKills;
	 copyLocal->m_arenaTotalKills    = m_arenaTotalKills;
	 copyLocal->m_physicsOverdTemp   = m_physicsOverdTemp;
	 copyLocal->m_aiCfgReference     = m_aiCfgReference;
	 copyLocal->m_currentBounty      = m_currentBounty;
	 copyLocal->m_hostileFlag		 = m_hostileFlag;
	 copyLocal->m_claimedTemp        = m_claimedTemp;
	 copyLocal->m_deaf               = m_deaf; 
	 copyLocal->m_mute               = m_mute;
	 copyLocal->m_deafMuteNotify     = m_deafMuteNotify;
	 copyLocal->m_deathCount		 = m_deathCount;
	 copyLocal->m_stencilChannel	 = m_stencilChannel;
	 copyLocal->m_respawnWorldName	 = m_respawnWorldName;
	 copyLocal->m_arenaRespawnWorldName = m_arenaRespawnWorldName;
	 copyLocal->m_reflectLevel		 = m_reflectLevel;
	 copyLocal->m_invunerableDuration= m_invunerableDuration;
	 copyLocal->m_factionStanding	 = m_factionStanding;


	 if(m_reconfigInven)
		m_reconfigInven->Clone(&copyLocal->m_reconfigInven);

	 if(m_currentInventory)
		m_currentInventory->CloneMinimum(&copyLocal->m_currentInventory);

	 /*if(m_bankInventory)
		 m_bankInventory->CloneMinimum(&copyLocal->m_bankInventory);*/
	 for (int i=0; i<this->m_banks.size(); i++)
	 {
		 CInventoryObjList *pCloneInventory = NULL;
		 this->m_banks[i]->CloneMinimum(&pCloneInventory);
		 copyLocal->m_banks.push_back(pCloneInventory);
	 }

	 if(m_stats)
		m_stats->Clone(&copyLocal->m_stats);

	 if(m_noteriety)
		m_noteriety->Clone(&copyLocal->m_noteriety);

	 if(m_skills)
		m_skills->Clone(&copyLocal->m_skills);

	 if(m_dotList)
		m_dotList->Clone(&copyLocal->m_dotList);

	 //Copy Bonus array
	 for(int i = 0; i < MAX_BONUS; i++)
	 {
		 copyLocal->m_bonus[i].m_bonusAmount = m_bonus[i].m_bonusAmount;
		 copyLocal->m_bonus[i].m_bonusDuration = m_bonus[i].m_bonusDuration;
		 copyLocal->m_bonus[i].m_bonusType = m_bonus[i].m_bonusType;
		 copyLocal->m_bonus[i].m_damageMult = m_bonus[i].m_damageMult;
		 copyLocal->m_bonus[i].m_maxDuration = m_bonus[i].m_maxDuration;
	 }

	 if(m_groupDatabase)
		m_groupDatabase->Clone(&copyLocal->m_groupDatabase);

	 if(m_questJournal)
        m_questJournal->Clone(&copyLocal->m_questJournal);


     if(m_deformableMeshCount > 0)
	 {
        copyLocal->m_deformableCfg = new int[m_deformableMeshCount];
        CopyMemory(copyLocal->m_deformableCfg,m_deformableCfg,sizeof(int)*m_deformableMeshCount);
	 }

	 if(m_arenaInfo)
	 {
		 CopyMemory(copyLocal->m_arenaInfo, m_arenaInfo, sizeof(int) * ARENA_TRACKING);
	 }
	 copyLocal->m_arenaBetAmount = this->m_arenaBetAmount;

	 *clone = copyLocal;
}



CPlayerObject::~CPlayerObject()
{
    if(m_currentInventory){
	   m_currentInventory->SafeDelete();
	   delete m_currentInventory;
	   m_currentInventory=0;
	}

	if(m_arenaInfo)
	{
		delete [] m_arenaInfo;
		m_arenaInfo = 0;
	}

	if(m_deformableCfg){
	   delete [] m_deformableCfg;
	   m_deformableCfg = 0;
	}

	if(!m_skillDBisReferenced)
	if(m_skills)
	{
       m_skills->SafeDelete();
	   delete m_skills;
	}
	m_skills=0;

	if(m_dotList)
	{
		m_dotList->SafeDelete();
		delete m_dotList;
		m_dotList = 0;
	}

	/*if(m_bankInventory)
	{
       m_bankInventory->SafeDelete();
	   delete m_bankInventory;
	   m_bankInventory=0;
	}*/
	for (int i=0; i<this->m_banks.size(); i++)
	{
		SafeDelete(this->m_banks[i]);
	}

	if(m_stats)
	{
       m_stats->SafeDelete();
	   delete m_stats;
       m_stats = 0;
	}

	if(m_groupDatabase)
	{
       m_groupDatabase->SafeDelete();
	   delete m_groupDatabase;
	   m_groupDatabase = 0;
	}

	if(m_reconfigInven)
	{
       m_reconfigInven->SafeDelete();
	   delete m_reconfigInven;
	   m_reconfigInven = 0;
	}

	if(m_firemsgsoutbox)
	{
	   delete m_firemsgsoutbox;
	   m_firemsgsoutbox = 0;
	}

	if(m_playerChangeInvenOutbox)
	{
	   delete m_playerChangeInvenOutbox;
	   m_playerChangeInvenOutbox = 0;
	}

	if(m_playerItemCfgOutbox)
	{
	   delete m_playerItemCfgOutbox;
	   m_playerItemCfgOutbox = 0;
	}

	if(m_playerEntityCfgOutbox)
	{
	   delete m_playerEntityCfgOutbox;
	   m_playerEntityCfgOutbox = 0;
	}

	if(m_playerEntityDamageMgsOutbox)
	{
	   delete m_playerEntityDamageMgsOutbox;
	   m_playerEntityDamageMgsOutbox = 0;
	}

	if(m_playerEntityGeneralMgsOutbox)
	{
	   delete m_playerEntityGeneralMgsOutbox;
	   m_playerEntityGeneralMgsOutbox = 0;
	}

    if(m_playerEntityDetailedMgsOutbox)
	{
	   delete m_playerEntityDetailedMgsOutbox;
	   m_playerEntityDetailedMgsOutbox = 0;
	}

	if(m_attributeOutbox)
	{
	   delete m_attributeOutbox;
	   m_attributeOutbox = 0;
	}

	if(m_equipMsgOutbox)
	{
	   delete m_equipMsgOutbox;
	   m_equipMsgOutbox = 0;
	}

	if(m_txtMsgOutbox)
	{
	   delete m_txtMsgOutbox;
	   m_txtMsgOutbox = 0;
	}

	if(m_statisticsOutbox)
	{
	   delete m_statisticsOutbox;
	   m_statisticsOutbox = 0;
	}

	if(m_miscOutBox)
	{
	   delete m_miscOutBox;
	   m_miscOutBox = 0;
	}

	if(m_noteriety)
	{
	   delete m_noteriety;
	   m_noteriety = 0;
	}

	if(m_questJournal)
	{
       m_questJournal->SafeDelete();
       delete m_questJournal;
	   m_questJournal = 0;
	}
}

BOOL CPlayerObject::Vulnerable()
{
	if((GetTickCount() - m_invunerableStamp) < m_invunerableDuration)
		return FALSE;
	
	m_invunerableTemp = FALSE;
	return TRUE;
}

void CPlayerObject::SetInvunerableDuration(DWORD duration)
{
	m_invunerableDuration = duration;
}

void CPlayerObject::InvunerableTimeStamp()
{
	m_invunerableStamp = GetTickCount();
}

CString CPlayerObject::ReassembleName(void * sif)
{
	m_visName = m_charName + "\n";
	if(m_clanAbbrev.GetLength() > 0)
		m_visName += m_clanAbbrev + "\n";
	if(m_squadName.GetLength() > 0)
		m_visName += "[" + m_squadName + "]" + "\n";

	//best skill name
	if(sif)
	if(m_skills)
	if(m_skills->m_skillToDisplay != -1)
	{
		CSkillObject *skill = m_skills->GetSkillByType(m_skills->m_skillToDisplay);
		CSkillObject *baseSkill = ((CSharedNet *)sif)->m_skillDatabase->GetSkillByType(m_skills->m_skillToDisplay);

		if(skill)
		if(baseSkill)
		{
			POSITION posLevel = baseSkill->m_levelRangeDB->GetHeadPosition();
			while(posLevel)
			{
				CLevelRangeObject *level = (CLevelRangeObject*)baseSkill->m_levelRangeDB->GetNext(posLevel);
				if(level->m_rangeStart < skill->m_currentExperience && level->m_rangeEnd > skill->m_currentExperience)
				{
					m_visName += level->m_levelTitle;
					break;
				}
			}
		}
	}
	
	//m_visName += m_charName;
	return m_visName;
}

BOOL CPlayerObject::AllowAsyncQue()
{
   DWORD curTime = GetTickCount();
   if(curTime-m_throttleStamp > 1000)
   {
      m_throttleStamp = curTime;
	  return TRUE;
   }
   return FALSE;
}

BOOL CPlayerObject::AllowAsyncQueMini()
{
   DWORD curTime = GetTickCount();
   if(curTime-m_throttleStamp > 200)
   {
      m_throttleStamp = curTime;
	  return TRUE;
   }
   return FALSE;
}

void CPlayerObject::Attack(CPlayerObject * victim, long timeout, int damageAmount)
{ 
	 if(victim->m_handle == m_handle)//dont m self
		return;

	 if(victim->m_noteriety->m_currentAgressors)
	 {
        if(m_groupID > 0)// part of a group
	       victim->m_noteriety->m_currentAgressors->UpdateAgressorState(-m_groupID, damageAmount, victim->m_currentWorldID);
		else
		   victim->m_noteriety->m_currentAgressors->UpdateAgressorState(m_handle, damageAmount, victim->m_currentWorldID);
	 }
	
}

BOOL CPlayerObject::LootAccessCheck(CPlayerObject * lootRequestedBy,int & serverLootMode)
{ 
    if(lootRequestedBy->m_handle == m_handle)//self bypass
       return TRUE;


	switch(serverLootMode)
	{
	    case 0://simple mode
          return TRUE;
		break;
		case 1://group and handle majority damage
           if(m_noteriety)
		   if(m_noteriety->m_currentAgressors)
		   {
			  if(lootRequestedBy->m_groupID > 0)
			     return m_noteriety->m_currentAgressors->LootDamageBasedAccessCheck(-lootRequestedBy->m_groupID);
			  else
			     return m_noteriety->m_currentAgressors->LootDamageBasedAccessCheck(lootRequestedBy->m_handle);
		   }
		break;
	}
	return FALSE;
}

D3DVECTOR CPlayerObject::GetDatedPosition(DWORD oldTime)
{
    DWORD timePassed = GetTickCount()-m_timeLastUpdated;
    if(timePassed>=oldTime)
	{
	   DWORD interpolationCurStep = timePassed - oldTime;
	   if(interpolationCurStep > m_interpolationVar)
	      interpolationCurStep = m_interpolationVar;
       return InterpolatePoints(m_Last3dPosition, 
						        m_Target3dPosition,
					            (int)m_interpolationVar, 
					            (int)interpolationCurStep);
	}

	DWORD interpolationCurStep = m_interpolationVar-(oldTime - timePassed);
	if(interpolationCurStep > m_interpolationVar)
	   interpolationCurStep = m_interpolationVar;

	return InterpolatePoints(m_LastLast3dPosition, 
						     m_Last3dPosition,
					         (int)m_interpolationVar, 
					         (int)interpolationCurStep);

}

D3DVECTOR CPlayerObject::InterpolatePoints(D3DVECTOR StartVec, 
						                   D3DVECTOR EndVec,
					                       int NumOfStages, 
					                       int ThisStage)
{
  D3DVECTOR StageVec;
  float propfactor;
  if (NumOfStages <= 0)
  NumOfStages = 1;
  propfactor = (float)ThisStage/NumOfStages;
  StageVec.x = StartVec.x + propfactor * (EndVec.x - StartVec.x);
  StageVec.y = StartVec.y + propfactor * (EndVec.y - StartVec.y);
  StageVec.z = StartVec.z + propfactor * (EndVec.z - StartVec.z);
  return StageVec;
}

void CPlayerObject::LogPositionalMsg(LPMOVE_STRUCT msg)
{
	//end positional data
	if(m_justSpawned){//just spawned
       m_upVector.x          = msg->OrientateUpX;
	   m_upVector.y          = msg->OrientateUpY;
	   m_upVector.z          = msg->OrientateUpZ;
       m_dirVector.x         = msg->OrientateDirX;
	   m_dirVector.y         = msg->OrientateDirY;
	   m_dirVector.z         = msg->OrientateDirZ;
       m_3dPosition.x        = msg->Xpos;
	   m_3dPosition.y        = msg->Ypos;
	   m_3dPosition.z        = msg->Zpos;
	   m_Target3dPosition.x    = msg->Xpos;
	   m_Target3dPosition.y    = msg->Ypos;
	   m_Target3dPosition.z    = msg->Zpos;
       CopyMemory(&m_Last3dPosition,&m_3dPosition,sizeof(D3DVECTOR));

       m_justSpawned         = FALSE;
	}//end just spawned
	   //last last position
	   m_LastLast3dPosition.x = m_Last3dPosition.x;
	   m_LastLast3dPosition.y = m_Last3dPosition.y;
	   m_LastLast3dPosition.z = m_Last3dPosition.z;
	   //last position
	   m_LastUpVector.x      = m_upVector.x;
	   m_LastUpVector.y      = m_upVector.y;
	   m_LastUpVector.z      = m_upVector.z;
	   m_LastDirVector.x     = m_dirVector.x;
	   m_LastDirVector.y     = m_dirVector.y;
	   m_LastDirVector.z     = m_dirVector.z;
   	   m_Last3dPosition.x    = m_Target3dPosition.x;//m_3dPosition.x;
	   m_Last3dPosition.y    = m_Target3dPosition.y;//m_3dPosition.y;
	   m_Last3dPosition.z    = m_Target3dPosition.z;//m_3dPosition.z;
       //target position
	   m_TargetUpVector.x    = msg->OrientateUpX;
	   m_TargetUpVector.y    = msg->OrientateUpY;
	   m_TargetUpVector.z    = msg->OrientateUpZ;
       m_TargetDirVector.x   = msg->OrientateDirX;
	   m_TargetDirVector.y   = msg->OrientateDirY;
	   m_TargetDirVector.z   = msg->OrientateDirZ;
       m_Target3dPosition.x  = msg->Xpos;
  	   m_Target3dPosition.y  = msg->Ypos;
	   m_Target3dPosition.z  = msg->Zpos;

	   CopyMemory(&m_3dPosition,&m_Target3dPosition,sizeof(D3DVECTOR));
	   CopyMemory(&m_dirVector,&m_TargetDirVector,sizeof(D3DVECTOR));
	   CopyMemory(&m_upVector,&m_TargetUpVector,sizeof(D3DVECTOR));

	   DWORD lastUpdatedTimeReference = m_timeLastUpdated;

	   m_timeLastUpdated     = GetTickCount();

	m_freeLookYaw         = msg->FreeLookYaw;
	m_freeLookPitch       = msg->FreeLookPitch;
	if(m_energy>0){//only log anim if alive
	m_curAnim             = msg->curAnim;
	m_secondaryAnim       = msg->secondaryAnim;
	}//only log anim if alive
	m_inGame              = TRUE;

	m_interpolationVar = (DWORD)(((float)(m_timeLastUpdated - lastUpdatedTimeReference))*1.0f);

	  if(m_interpolationVar > 3000)
	  {//interpolation epsilon
         m_interpolationVar = 3000;
	  }//end interpolation epsilon

}

void CPlayerObject::LogPositionalMsg(LPSHRUNK_TOSVRMOV_STRUCT msg)
{
	//end positional data
	if(m_justSpawned){//just spawned
       m_upVector.x          = msg->OrientateUpX / 127.0f;
	   m_upVector.y          = msg->OrientateUpY / 127.0f;
	   m_upVector.z          = msg->OrientateUpZ / 127.0f;
       m_dirVector.x         = msg->OrientateDirX / 127.0f;
	   m_dirVector.y         = msg->OrientateDirY / 127.0f;
	   m_dirVector.z         = msg->OrientateDirZ / 127.0f;
       m_3dPosition.x        = msg->Xpos;
	   m_3dPosition.y        = msg->Ypos;
	   m_3dPosition.z        = msg->Zpos;
	   m_Target3dPosition.x    = msg->Xpos;
	   m_Target3dPosition.y    = msg->Ypos;
	   m_Target3dPosition.z    = msg->Zpos;
       CopyMemory(&m_Last3dPosition,&m_3dPosition,sizeof(D3DVECTOR));

       m_justSpawned         = FALSE;
	}//end just spawned
	   //last last position
	   m_LastLast3dPosition.x = m_Last3dPosition.x;
	   m_LastLast3dPosition.y = m_Last3dPosition.y;
	   m_LastLast3dPosition.z = m_Last3dPosition.z;
	   //last position
	   m_LastUpVector.x      = m_upVector.x;
	   m_LastUpVector.y      = m_upVector.y;
	   m_LastUpVector.z      = m_upVector.z;
	   m_LastDirVector.x     = m_dirVector.x;
	   m_LastDirVector.y     = m_dirVector.y;
	   m_LastDirVector.z     = m_dirVector.z;
   	   m_Last3dPosition.x    = m_Target3dPosition.x;//m_3dPosition.x;
	   m_Last3dPosition.y    = m_Target3dPosition.y;//m_3dPosition.y;
	   m_Last3dPosition.z    = m_Target3dPosition.z;//m_3dPosition.z;
       //target position
	   m_TargetUpVector.x    = msg->OrientateUpX / 127.0f;
	   m_TargetUpVector.y    = msg->OrientateUpY / 127.0f;
	   m_TargetUpVector.z    = msg->OrientateUpZ / 127.0f;
       m_TargetDirVector.x   = msg->OrientateDirX / 127.0f;
	   m_TargetDirVector.y   = msg->OrientateDirY / 127.0f;
	   m_TargetDirVector.z   = msg->OrientateDirZ / 127.0f;
       m_Target3dPosition.x  = msg->Xpos;
  	   m_Target3dPosition.y  = msg->Ypos;
	   m_Target3dPosition.z  = msg->Zpos;

	   CopyMemory(&m_3dPosition,&m_Target3dPosition,sizeof(D3DVECTOR));
	   CopyMemory(&m_dirVector,&m_TargetDirVector,sizeof(D3DVECTOR));
	   CopyMemory(&m_upVector,&m_TargetUpVector,sizeof(D3DVECTOR));

	   long lastUpdatedTimeReference = m_timeLastUpdated;

	   m_timeLastUpdated     = GetTickCount();

	m_freeLookYaw         = 0.0f;//msg->FreeLookYaw;
	m_freeLookPitch       = msg->FreeLookPitch;
	if(m_energy>0){//only log anim if alive
	m_curAnim             = msg->curAnim;
	m_secondaryAnim       = msg->secondaryAnim;
	}//only log anim if alive
	m_inGame              = TRUE;

	m_interpolationVar = (long)(((float)(m_timeLastUpdated - lastUpdatedTimeReference))*1.0f);

	  if(m_interpolationVar > 3000)
	  {//interpolation epsilon
         m_interpolationVar = 3000;
	  }//end interpolation epsilon
}

bool				  
CPlayerObject::IsInQuickSlots(int iGLID)
{
	if (!this->m_QuickSlots) return false;
	for (int i=0; i<NUM_QUICKBARS; i++)
	{
		for (int j=0; j<NUM_QUICKBAR_SLOTS; j++)
		{
			if (this->m_QuickSlots[i][j] == iGLID)
				return true;
		}
	}
	return false;
}

int
CPlayerObject::QuickSlots(int iBar, int iSlot, int iGLID)
{
	// Bounds Check
	if ((iBar >= NUM_QUICKBARS)
	|| (iBar < 0)
	|| (iSlot >= NUM_QUICKBAR_SLOTS)
	|| (iSlot < 0)) 
	{
		return -1;
	}
	else
	{
		// Read Access
		if(iGLID == INT_MIN) 
		{
			return this->m_QuickSlots[iBar][iSlot];
		}
		// Write Access
		else
		{
			this->m_QuickSlots[iBar][iSlot] = iGLID;
			CInventoryObj *pInventoryObj = this->m_currentInventory->GetByGLID(iGLID);
			if (pInventoryObj)
			{
				if (iGLID == -1)
					pInventoryObj->m_itemData->m_isInQuickSlots = false;
				else
					pInventoryObj->m_itemData->m_isInQuickSlots = true;
			}
			return iGLID;			
		}
	}
}

CPlayerObject * CPlayerObjectList::GetAIObjectByAliveAndIDandTrace(DPNID id, int trace, int worldChannelID)
{
   CAxList * list = GetListByChannel(worldChannelID);
   if(!list)
	   return NULL;

   for(CAxLinkObj * posLoc = list->GetHeadPosition_unsafe();posLoc!=NULL;){//account list
       CPlayerObject * pPtr = (CPlayerObject *)list->GetNext_unsafe(&posLoc);
	   if(pPtr->m_energy > 0)
	   if(pPtr->m_currentID == id)
       if(pPtr->m_aiTraceTemp == trace)
	   if(pPtr->m_currentWorldID == worldChannelID)
	      return pPtr;
   }
 return NULL;
}

CPlayerObject * CPlayerObjectList::GetObjectAICfgUnclaimed(int cfg)
{//TODO
   /*for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
       CPlayerObject * pPtr = (CPlayerObject *)GetNext(posLoc);
	   if(pPtr->m_aiCfgReference == cfg)
	   if(pPtr->m_energy > 0)
	   if(pPtr->m_claimedTemp == 0)
	      return pPtr;
   }*/
   return NULL;
}

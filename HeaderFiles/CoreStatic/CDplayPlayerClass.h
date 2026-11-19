#ifndef CDPLAYPLAYERMANAGEMENTCLASS_H
#define CDPLAYPLAYERMANAGEMENTCLASS_H

#include "Common.h"
#include "CMaterialClass.h"
#include "CStructuresMisl.h"
#include "CInventoryClass.h"
#include "dplay8.h"
#include "CSkillClass.h"
#include "CCurrencyClass.h"
#include "CStatClass.h"
#include "CCommGroupClass.h"
#include "CPacketQueClass.h"
#include "CNoterietyClass.h"
#include "CQuestJournal.h"
#include "CBonusClass.h"
#include "CAxList.h"
#include "CDotClass.h"
#include <vector>

using namespace std;

class CAICollectionQuestObj;

#define MAX_BONUS		3
#define ARENA_TRACKING	6

struct CGroupStruct
{
public:
	CGroupStruct()
	{
		this->m_groupPlacementIndex = 0;
		this->m_squadDataIsDirty = 0;
		this->m_groupLeadership = 0;
	}

	CGroupStruct& operator=(const CGroupStruct &rhs)
	{
		if (this == &rhs) return *this; // CHECK for self-assignment
		this->m_groupPlacementIndex = rhs.m_groupPlacementIndex;
		this->m_squadDataIsDirty = rhs.m_squadDataIsDirty;
		this->m_groupLeadership = rhs.m_groupLeadership;
		return *this;
	}

	unsigned char m_groupPlacementIndex;
	unsigned char m_squadDataIsDirty;
	unsigned char m_groupLeadership;
};

class CPlayerObject
{

public:
	
	CPlayerObject();
	~CPlayerObject();

    //
	CString               m_visName;
	CString               m_charName;

    //POSITIONAL management on server
	D3DVECTOR             m_upVector;
    D3DVECTOR             m_dirVector;
	D3DVECTOR             m_3dPosition;
	D3DVECTOR             m_LastUpVector;
    D3DVECTOR             m_LastDirVector;
	D3DVECTOR             m_LastLast3dPosition;
	D3DVECTOR             m_Last3dPosition;
	D3DVECTOR             m_TargetUpVector;
    D3DVECTOR             m_TargetDirVector;
	D3DVECTOR             m_Target3dPosition;
	float                 m_freeLookYaw;
	float                 m_freeLookPitch;
	CString               m_currentWorldMap;
	int                   m_currentWorldID;//dynamically assigned
	int					  m_stencilChannel;
	int                   m_accountNumberRef;
	int                   m_accountNumberRefOld;
	DWORD                 m_interpUpdateTemp;
	BOOL				  m_arenaItemsGiven;

	//IDs
	DPNID                 m_currentID;//communication ID
	int                   m_networkAssignedID;//network assigned ID (TRACE)
	//Quest Journal
    CQuestJournalList    *m_questJournal;

	//Animation state
	int                   m_animationState;
	int                   m_curAnim;//- is reverse
	int                   m_secondaryAnim;

    //TEMP states
	BOOL                  m_inGame;
	BOOL                  m_justSpawned;
	DWORD                 m_timeLastUpdated;
	long                  m_timeDied;//-1 n/a > ms when dies
	int					  m_rechargeTimer;
    
	//Vehicle/PET mounts
	int                   m_mountedOnPlayerID;//-1 not riding anything
	int                   m_mountedByPlayerID;//-1 == not mounted
	                                          //if mounted will be moved to appropriate position
	//cfg current/used for rebuild
	CInventoryObjList   * m_currentInventory;
	//CInventoryObjList   * m_bankInventory;//secure banking
	vector<CInventoryObjList*>	m_banks; 
private:
	int					  m_QuickSlots[NUM_QUICKBARS][NUM_QUICKBAR_SLOTS];
	
public:
	int                   m_dbCfg;
	int                   m_energy;
	int                   m_maxEnergy;
	BOOL                  m_alive;
	int                   m_armourClass;//100 max
	int					  m_originalEDBEnergy;

	//Initial Spawn when began, also death spawn
	int                   m_bornSpawnCfgIndex;

    //tag AI if spawned by an AI manager
	BOOL                  m_aiControlled;
	DWORD                 m_interpolationVar;

	//deformable cfg (initialy done)
	int                   m_deformableMeshCount;
	int                  *m_deformableCfg;
	float                 m_dynamicRadius;

	//skills/experience points
	CSkillObjectList     *m_skills;
	CStatObjList         *m_stats;
	long                  m_lastUseItemStamp;
	int                   m_raceID;
	BOOL                  m_inSafeZone;//track for notification reasons

	//respawn config
	D3DVECTOR             m_respawnPosition;
	//int                   m_respawnWorld;
	CString				  m_respawnWorldName;

	//arena Respawn
	D3DVECTOR			  m_arenaRespawnPosition;
	CString				  m_arenaRespawnWorldName;

	//handle for chat and clans
	int                   m_handle;
	int					  m_groupID;
	CString				  m_squadName;
	CGroupStruct		  m_groupStruct;
	DWORD				  m_lastGroupUpdate;
	char				  m_groupBlockTracker;

	//trade 
	DWORD				  m_tradeOfferTimeStamp;
	CInventoryObjList	  * m_tradeOffer;//will be emptied when a new trade starts-locked once submitted.
	int					  m_tradeTargetID; //-1 none
	BOOL				  m_tradeAccepted; //reset to false when new trade starts
	int					  m_tradePendingTarget; //-1 none
	int					  m_tradePendingRequest; //-1 none

	//Dot Tracking
	CDotClassList		 *m_dotList;
	char				  m_dotShieldEffect;
	char				  m_dotJetEffect;
	int					  m_dotDamageReduction[8];
	int					  m_dotDamageTakeIncrease[8];
	int					  m_dotLowerMaxHP;
	int					  m_dotLowerMaxMP;

	//Suicide Timer
	DWORD				  m_suicideTimeStamp;
	char				  m_suicideTimer;

	//Information for DamageWounds
	unsigned char		  m_woundChannel[8];

	//group comm object
    CCommGroupList       *m_groupDatabase;
	//reference
	CPlayerObject        *m_playerOrigRef;

	long                  m_weaponSwitchStamp;

    float                 m_expTempAIRef;
	float                 m_visualDistanceRef;

	int                   m_aiCfgReference;


	//stasis
	int                   m_stasis;//0=none 1=hidden 2=shapeshifted
	D3DVECTOR             m_positionLock;//multi used (hiding)
	int                   m_altCfgIndex;//shapeshifted alternate cfg
	int                   m_curEDBIndex;//
	int                   m_originalEDBCfg;
	CInventoryObjList   * m_reconfigInven;//use when unshifting


	//temp Timers Sytems
	long                  m_skillUseTimeStamp;//time last used skill
	int                   m_overridedEnergy;
    DWORD                 m_lastSurroundingsUpdateTime;
	
	//int flags for ridable pets/vehicles
	int                   m_aiCfgOfMounted;//-1 = none
	BOOL                  m_admin;
	BOOL                  m_gm;
	BOOL                  m_spawnAbility;
	float				  m_reflectLevel;
	//CObject             * m_accountReference;
	int                   m_violationsInARow;

    //
	int                   m_networkIDLastAttacker;
	long                  m_timeStampOfLastAttack;
    BOOL                  m_skillDBisReferenced;


	int                   m_currentHighestLevel;
	float                 m_currentHighestExperience;
	BOOL                  m_bodyNeedToBePlaced;

	DWORD                 m_invunerableStamp;//used for 
	DWORD				  m_invunerableDuration;
	BOOL                  m_invunerableTemp;
	BOOL                  m_invunOptionTemp;

	int                   m_aiTraceTemp;

	//POINTS
	int                   m_arenaBasePoints;
	
	//WebsiteTracking
	int					  m_pvpKills;
	int					  m_arenaKills;

	// Arena and Tracking Info
	bool				  m_bMostWanted;
	int                   m_inArena;
	int                   m_arenaTotalKills;
	int					  m_deathCount;
	short				  m_teamTemp;//team arena vars
	int					  m_arenaBetAmount;
	int					  m_factionStanding;
	int					  *m_arenaInfo;		/*Will hold the following:	0 - Arena Type
																		1 - Score Tracker
																		2 - Kill Tracker
																		3 - Death Tracker
																		4 - Damage Tracker
																		5 - Bet Reward | NOTE: < 0 is a loss, > 0 is a win
																	*/

	//physics select
	short                 m_physicsOverdTemp;

	//bounty
	int                   m_currentBounty;

	//flags temporary
	char                  m_justDied;
	char                  m_hostileFlag;
	char                  m_claimedTemp;
	bool                  m_allowLargeMovement;//one this is used once the srever will set it to zero again, ussually used in fast portal situations.

	CObject             * m_entityAddressRef;
    //DWORD                 m_backupStamp;
	DWORD                 m_questTimer;
	DWORD                 m_envDamageStamp;
	int                   m_weaponPass;
	float                 m_speedRating;
	BOOL                  m_deaf; 
	BOOL                  m_mute;
	BOOL                  m_deafMuteNotify;
	DWORD                 m_throttleStamp;//for qued calls
	DWORD                 m_regenStamp;
	DWORD				  m_ammoRegenStamp;

	void                * m_phyRef;

	//instance specifier
	int					  m_instanceType;


	//outbox packets
	CPacketObjList      * m_firemsgsoutbox;//lossy Mov pack
	CPacketObjList      * m_playerItemCfgOutbox;//lossy Mov pack
	CPacketObjList      * m_playerEntityCfgOutbox;//lossy Mov pack
	CPacketObjList      * m_playerEntityDamageMgsOutbox;//lossy Mov pack
	CPacketObjList      * m_playerEntityGeneralMgsOutbox;//lossy Mov pack
	CPacketObjList      * m_playerEntityDetailedMgsOutbox;//lossy Mov pack
	CPacketObjList      * m_playerChangeInvenOutbox;//non loss
	CPacketObjList      * m_attributeOutbox;//non loss
	CPacketObjList      * m_equipMsgOutbox;//non loss
	CPacketObjList      * m_txtMsgOutbox;//non loss
	CPacketObjList      * m_statisticsOutbox;//non loss
	CPacketObjList      * m_miscOutBox;

	//
	CNoterietyObj       * m_noteriety;

	CString               m_additionalChatTemp;

	//clan binds
	int                   m_clanHandle;//-1 = none
	CString				  m_clanAbbrev;
	int                   m_currentRequestToJoinClanHandle;//-1 = none
	DWORD                 m_countDownToMurderPardon;
	DWORD                 m_spawnTimeStamp;
	DWORD                 m_lastMsgTimeStamp;
	DWORD                 m_murderWearTimeStamp;

    D3DVECTOR             m_movementTrackPosStamp;
	DWORD                 m_movementTrackTimeStamp;
	int                   m_currentRequestForAWar;
	
	//Powerup Goodnesss
	CBonus				  m_bonus[MAX_BONUS];
	DWORD				  m_StatStamp;
	DWORD				  m_ResistStamp;

	//Misc Damage Variables
	int					  m_DamageModifier; //Actual Amount
	signed char			  m_DamageChannelOverride; //0-7 resistance Channels
	unsigned char         m_DamageType; //Health (0) or Energy (1) Both(2)
	int					  m_DamageDuration; //duration
	DWORD				  m_DamageStamp;

	//FUNCTIONS
	BOOL				  LootAccessCheck(CPlayerObject * lootRequestedBy,int & serverLootMode);
    void                  Clone(CPlayerObject ** copy);
	void                  CloneMinimum(CPlayerObject ** clone);
	void                  LogPositionalMsg(LPMOVE_STRUCT msg);
	void				  LogPositionalMsg(LPSHRUNK_TOSVRMOV_STRUCT msg);
	void                  Attack(CPlayerObject * victim,long timeout, int damageAmount);
	D3DVECTOR             InterpolatePoints(D3DVECTOR StartVec, D3DVECTOR EndVec,int NumOfStages, int ThisStage);
	D3DVECTOR             GetDatedPosition(DWORD oldTime);
	BOOL                  AllowAsyncQue();
	BOOL                  AllowAsyncQueMini();
	void				  SetInvunerableDuration(DWORD duration);
	BOOL				  Vulnerable();
	void				  InvunerableTimeStamp();
	bool				  IsInQuickSlots(int iGLID);
	int					  QuickSlots(int iBar, int iSlot, int iGLID = INT_MIN);
	CString				  ReassembleName(void * sif = NULL);
};

//Notes
//DPNID = DWORD value range
//0 to 4,294,967,295
//note:if this class is used multi threaded we have to extend the cell locks-TODO
class CPlayerObjectList : public CAxDB
{
public:
    CPlayerObjectList()
    {
      Init(2000,7);//determines memory commit and speed of query
      AddIndexVariable(0,"net_id");//m_networkAssignedID unique
      AddIndexVariable(1,"dpnid");//m_currentID unique
      AddIndexVariable(2,"channel");//m_currentWorldID-non unique
	  AddIndexVariable(3,"handle");//m_handle unique
	  AddIndexVariable(4,"group_id");//m_groupID
	  AddIndexVariable(5,"name");//m_charName
	  AddIndexVariable(6,"clan_handle");//m_charName
    };
    ~CPlayerObjectList()
    {
      //traverse list manually to delete core class objects, the list will clear itself when deconstructed
	  Lock();
	  this->WriteStatus("cleaned_CPlayerObjectList.txt");
	  if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
	     for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
	     {
             for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
			 {
		         CPlayerObject * delPtr = (CPlayerObject *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
				 delete delPtr;
				 delPtr = 0;
			 }
		 }
	  //internally the rest of the lsists should auto clean themselfs
	  Unlock();
    };
   
    //FUNCTIONS
	CPlayerObject * GetAIObjectByAliveAndIDandTrace(DPNID id,int trace,int worldChannelID);
	CPlayerObject * GetObjectAICfgUnclaimed(int cfg);

	bool Add_safe(CPlayerObject * playerMem)
	{
         if(!playerMem)//assert on add index-return false here
             return false;

		 Lock();
         AddIndexed((void*)playerMem, playerMem->m_networkAssignedID, playerMem->m_networkAssignedID,"net_id");
         AddIndexed((void*)playerMem, playerMem->m_currentID, playerMem->m_networkAssignedID,"dpnid");
		 AddIndexed((void*)playerMem, playerMem->m_currentWorldID, playerMem->m_networkAssignedID,"channel");//channel
		 AddIndexed((void*)playerMem, playerMem->m_handle, playerMem->m_networkAssignedID,"handle");//channel
		 if(playerMem->m_groupID > 0)
			AddIndexed((void*)playerMem, playerMem->m_groupID, playerMem->m_networkAssignedID,"group_id");//channel
		 AddIndexed((void*)playerMem, GetSimpleHash(playerMem->m_charName.GetBuffer()), playerMem->m_networkAssignedID,"name");

		 if(playerMem->m_clanHandle > 0)//filter so that clan db isnt large, we would not search by non clan bind
			AddIndexed((void*)playerMem, playerMem->m_clanHandle, playerMem->m_networkAssignedID,"clan_handle");

	     Unlock();
		 return true;
	}

    CPlayerObject * GetObjectByName(char * name)
	{
		 CAxList * obj = GetAllOnIndex(GetSimpleHash(name),"name");
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_charName.CompareNoCase(name)==0)
			    return plObj;
		 }

	     return NULL;
	}

	CPlayerObject * GetObjectByNameAlive(char * name)
	{
		 CAxList * obj = GetAllOnIndex(GetSimpleHash(name),"name");
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
			 if(plObj->m_energy > 0)
             if(plObj->m_charName.CompareNoCase(name)==0)
			    return plObj;
		 }

	     return NULL;
	}

	CPlayerObject * GetObjectByName_LOS(char * name)
	{
		 Lock();
		 CAxList * obj = GetAllOnIndex(GetSimpleHash(name),"name");
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_charName.CompareNoCase(name)==0)
			    return plObj;
		 }
		 Unlock();

	     return NULL;
	}

	CPlayerObject * GetByNetID(unsigned int networkAsID)
	{
         CAxLinkObj * obj = GetByUniqueID(networkAsID,networkAsID,"net_id");
		 if(obj)
		    return (CPlayerObject *)obj->m_thisObject;
	     return NULL;
	}

	CPlayerObject * GetByNetID_LOS(unsigned int networkAsID)
	{
		 Lock();
         CAxLinkObj * obj = GetByUniqueID(networkAsID,networkAsID,"net_id");
		 if(obj)
		    return (CPlayerObject *)obj->m_thisObject;
		 Unlock();
	     return NULL;
	}

	BOOL VerifyValidByNetID_safe(int networkAsID)
   {
		 CAxList * obj = GetAllOnIndex(networkAsID,"net_id");

		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_networkAssignedID==networkAsID)
			 {
			    Unlock();
			    return TRUE;
			 }
		 }
		 Unlock();
	     return FALSE;
   }

	CPlayerObject * GetByDPNID(DPNID dpnid)
	{
		 CAxList * obj = GetAllOnIndex(dpnid,"dpnid");
		 if(!obj)
		    return NULL;

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_currentID==dpnid)
			    return plObj;
		 }
	     return NULL;
	}

	CPlayerObject * GetByClanID(int clanID)
	{
		 CAxList * obj = GetAllOnIndex(clanID,"clan_handle");
		 if(!obj)
		    return NULL;

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_clanHandle==clanID)
			    return plObj;
		 }
	     return NULL;
	}

	CAxList * GetListByClanID(int clanID)
	{
		CAxList * obj = GetAllOnIndex(clanID, "clan_handle");
		if(obj)
			return (CAxList *)obj;
		return NULL;
	}

	CAxList * GetListByChannel(unsigned int channel)
	{
         CAxList * obj = GetAllOnIndex(channel,"channel");
		 if(obj)
		    return (CAxList *)obj;
		return NULL;
	}


	CPlayerObject * GetFirstLiveOnChannelNoAI(unsigned int channel)
	{
         CAxList * obj = GetAllOnIndex(channel,"channel");
		
		 for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_energy > 0)
			 if(!plObj->m_aiControlled)
			 if(plObj->m_currentWorldID==channel)
			    return plObj;
		 }
		 return NULL;
	}



	CAxList *GetListByGroupID(unsigned int groupID)
	{
		CAxList *obj = GetAllOnIndex(groupID, "group_id");
		if(obj)
			return (CAxList *)obj;
		return NULL;
	}

	CAxList *GetListByGroupID_LOS(unsigned int groupID)
	{
		Lock();
		CAxList *obj = GetAllOnIndex(groupID, "group_id");
		if(obj)
			return (CAxList *)obj;
		Unlock();
		return NULL;
	}

	CPlayerObject * GetObjectByGroupID(unsigned int groupID)
	{
		 CAxList * obj = GetAllOnIndex(groupID,"group_id");
		 if(!obj)
		    return NULL;

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_groupID==groupID)
			    return plObj;
		 }
	     return NULL;
	}

	CPlayerObject * GetObjectByHandle(unsigned int handle)
	{
		 CAxList * obj = GetAllOnIndex(handle,"handle");
		 if(!obj)
		    return NULL;

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_handle==handle)
			    return plObj;
		 }
	     return NULL;
	}

	CPlayerObject * GetObjectByHandleAlive(unsigned int handle)
	{
		 CAxList * obj = GetAllOnIndex(handle,"handle");
		 if(!obj)
		    return NULL;

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
			 if(plObj->m_energy > 0)
             if(plObj->m_handle==handle)
			    return plObj;
		 }
	     return NULL;
	}

	CPlayerObject * GetObjectByHandle_LOS(unsigned int handle)
	{
		 CAxList * obj = GetAllOnIndex(handle,"handle");

         Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_handle==handle)
			    return plObj;
		 }
		 Unlock();
	     return NULL;
	}

	CPlayerObject * GetByDPNID_Alive(DPNID dpnid)
	{
         CAxList * obj = GetAllOnIndex(dpnid,"dpnid");
		 if(!obj)
		    return NULL;

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
			 if(plObj->m_energy > 0)
             if(plObj->m_currentID==dpnid)
			    return plObj;
		 }
	     return NULL;
	}

	CPlayerObject * GetByDPNID_Alive_LOS(DPNID dpnid)
	{
         CAxList * obj = GetAllOnIndex(dpnid,"dpnid");
		
         Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
			 if(plObj->m_energy > 0)
             if(plObj->m_currentID==dpnid)
			    return plObj;
		 }
		 Unlock();
	     return NULL;
	}


	CPlayerObject * GetByDPNID_PrefAlive(DPNID dpnid)
	{
         CAxList * obj = GetAllOnIndex(dpnid,"dpnid");

		 CPlayerObject * tempPtr = NULL;

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
			 if(plObj->m_currentID==dpnid)
			 {
				 if(!tempPtr)
					 tempPtr = plObj;
				 
				 if(plObj->m_energy > 0)
					return plObj;
			 }
		 }

         if(tempPtr)
		    return tempPtr;

	     return NULL;
	}

	CPlayerObject * GetByNetID_Alive_LOS(unsigned int networkAsID)
	{
		 Lock();
         CAxLinkObj * obj = GetByUniqueID(networkAsID,networkAsID,"net_id");
		 if(obj)
		 {

		    CPlayerObject * temp = (CPlayerObject *)obj->m_thisObject;
		    if(temp->m_energy > 0)
		       return (CPlayerObject *)obj->m_thisObject;
		 }
		 Unlock();
		 return NULL;
	}

	CPlayerObject * GetByNetID_Alive(unsigned int networkAsID)
	{
         CAxLinkObj * obj = GetByUniqueID(networkAsID,networkAsID,"net_id");
		 if(obj)
		 {

		    CPlayerObject * temp = (CPlayerObject *)obj->m_thisObject;
		    if(temp->m_energy > 0)
		       return (CPlayerObject *)obj->m_thisObject;
		 }
		 return NULL;
	}

	CPlayerObject * GetByNetID_GetAliveStatus_LOS(unsigned int networkAsID, BOOL * alive)
	{
         CAxList * obj = GetAllOnIndex(networkAsID,"net_id");
		 Lock();
		 for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_networkAssignedID==networkAsID)
			 {
                 *alive = TRUE;
				 if(plObj->m_energy < 1)
					*alive = FALSE;
			    return plObj;
			 }
		 }
		 Unlock();
	     return NULL;
	}

	BOOL DeleteByOject(CPlayerObject * playerMem)
	{
         if(!playerMem)
			 return FALSE;//should assert when used

		 RemoveIndexed(playerMem->m_networkAssignedID,playerMem->m_networkAssignedID, "net_id");
	     RemoveIndexed(playerMem->m_currentID,playerMem->m_networkAssignedID, "dpnid");
	     RemoveIndexed(playerMem->m_currentWorldID,playerMem->m_networkAssignedID,"channel");
		 RemoveIndexed(playerMem->m_handle,playerMem->m_networkAssignedID,"handle");
		 RemoveIndexed(playerMem->m_groupID,playerMem->m_networkAssignedID,"group_id");
		 RemoveIndexed(GetSimpleHash(playerMem->m_charName.GetBuffer()),playerMem->m_networkAssignedID,"name");

		 if(playerMem->m_clanHandle > 0)
			RemoveIndexed(playerMem->m_clanHandle,playerMem->m_networkAssignedID,"clan_handle");
         
		 delete playerMem;
		 playerMem = 0;

		 return TRUE;
	}

	BOOL ReIndexByGroupID_safe(CPlayerObject *player, int oldID, int newID)
	{
		if(!player)
			return FALSE;
 
		Lock();

		if(oldID > 0)
		{
			if(!RemoveIndexed(oldID, player->m_networkAssignedID, "group_id"))
			{
				Unlock();
				return FALSE;
			}
		}
		player->m_groupID = newID;
		if(newID < 1)
		{
			Unlock();
			return TRUE;
		}

		if(!AddIndexed((void*)player, newID, player->m_networkAssignedID,"group_id"))//channel
		{
			Unlock();
			return FALSE;
		}
		Unlock();

		return TRUE;
	}

	BOOL ReIndexByGroupID(CPlayerObject *player, int oldID, int newID)
	{
		if(!player)
			return FALSE;

		if(oldID > 0)
		{
			if(!RemoveIndexed(oldID, player->m_networkAssignedID, "group_id"))
				return FALSE;
		}

		player->m_groupID = newID;
		if(newID < 1)
			return TRUE;
		
		if(!AddIndexed((void*)player, newID, player->m_networkAssignedID,"group_id"))//channel
			return FALSE;

		return TRUE;
	}

	BOOL ReIndexByClanHandle(CPlayerObject *player, int newHandle)
	{
		if(!player)
			return FALSE;

		RemoveIndexed(player->m_clanHandle, player->m_networkAssignedID, "clan_handle");//will fail somtimes on purpose

		player->m_clanHandle = newHandle;
        if(newHandle < 1)
		   return TRUE;//filter non bind

		if(!AddIndexed((void*)player, newHandle, player->m_networkAssignedID,"clan_handle"))//channel
			return FALSE;

		return TRUE;
	}

	BOOL ReIndexByClanHandle(int playerClanHandle, int newHandle)
	{
		CPlayerObject * player = GetByClanID(playerClanHandle);
		if(!player)
			return FALSE;

		RemoveIndexed(player->m_clanHandle, player->m_networkAssignedID, "clan_handle");//will fail somtimes on purpose

		player->m_clanHandle = newHandle;
        if(newHandle < 1)
		   return TRUE;//filter non bind

		if(!AddIndexed((void*)player, newHandle, player->m_networkAssignedID,"clan_handle"))//channel
			return FALSE;

		return TRUE;
	}


	BOOL ResortGroupList(unsigned int groupID, unsigned char placementHolder)
	{
		CAxList *playerList = GetListByGroupID(groupID);
		CPlayerObject *playerObjectData = NULL;
		int counter = 0;
		Lock();
		for(CAxLinkObj *posLoc = playerList->GetHeadPosition_unsafe();posLoc != NULL;)
		{
			if(!posLoc) 
			{
				Unlock();
				return FALSE;
			}
			CPlayerObject * pPtr = (CPlayerObject *)playerList->GetNext_unsafe(&posLoc);
			if(pPtr->m_groupID == groupID)
			{	
				if(pPtr->m_groupStruct.m_groupPlacementIndex > placementHolder)
					pPtr->m_groupStruct.m_groupPlacementIndex--;
				if(pPtr->m_groupStruct.m_groupPlacementIndex == 1)
				{
					pPtr->m_groupStruct.m_groupLeadership = 1;
					playerObjectData = pPtr;
				}
				counter++;
			}
		}

		//Disband group if only 1 is left in group
		/*if(counter == 1)
		{
			ReIndexByGroupID(playerObjectData, playerObjectData->m_groupID, 0);
			playerObjectData->m_groupStruct.m_squadDataIsDirty = 0;
			playerObjectData->m_groupStruct.m_groupLeadership = 0;
			playerObjectData->m_groupStruct.m_groupPlacementIndex = 0;
			playerObjectData->m_groupBlockTracker = -1;
		}*/

		Unlock();
		return TRUE;
	}

	BOOL RemoveByNetID(int networkAsID)//crash
	{
		 CAxLinkObj * obj = GetByUniqueID(networkAsID,networkAsID,"net_id");
		 if(!obj)
		    return FALSE;
         
		 DeleteByOject((CPlayerObject *)obj->m_thisObject);
		 
		 return TRUE;
	}

	BOOL RemoveByDPNID(DPNID dpnid)//crash
	{
		 CAxList * obj = GetAllOnIndex(dpnid,"dpnid");

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
             if(plObj->m_currentID==dpnid)
			 {
                DeleteByOject(plObj);
                return TRUE;
			 }
		 }
	     return FALSE;
	}

	BOOL RemoveByNetID_Alive(unsigned int networkAsID)
	{
         CAxLinkObj * obj = GetByUniqueID(networkAsID,networkAsID,"net_id");
		 if(!obj)
		    return FALSE;

		 CPlayerObject * temp = (CPlayerObject *)obj->m_thisObject;
		 if(temp->m_energy <= 0)
		    return FALSE;
         
		 DeleteByOject((CPlayerObject *)obj->m_thisObject);
		 return TRUE;
	}

	BOOL SortToTeam(unsigned int channel, int teamCount, CPlayerObject * player)
	{//should be done before player is added
		if(teamCount < 1)
		   return FALSE;

        CAxList * obj = GetAllOnIndex(channel,"channel");
		if(!obj)
		   return FALSE;
		 
		int * teamCountAr = new int[teamCount];
		ZeroMemory(teamCountAr,sizeof(int)*teamCount);
		for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		{
             CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
			 if(plObj->m_teamTemp < teamCount && plObj->m_teamTemp > -1)//bounds check
				teamCountAr[plObj->m_teamTemp]++;
		}
        int teamSel = 0;
		int lowestMembers = 9999999;
        for(int i=0;i<teamCount;i++)
		{
            if(teamCountAr[i] <= lowestMembers) 
			{
               teamSel = i;
               lowestMembers = teamCountAr[i];
			}
		}
		player->m_teamTemp = teamSel;

		delete [] teamCountAr;
		teamCountAr = 0;

		return TRUE;
	}

	
};

#endif

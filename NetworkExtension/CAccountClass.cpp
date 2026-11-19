#include "stdafx.h"
#include "AuraNetHost.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include  <afxtempl.h>
#include <direct.h>
#include "CAccountClass.h"
#include "ServerScripts.h"
#include "cmultiplayerclass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------------//
//-----class is for persisting characters over a network-----//
//-----------------------------------------------------------//
CAccountObject::CAccountObject(CString name,
				               CString password
		                       )
{
m_currentCharRequestCount    = 0;
m_deafMute                   = 0;
m_backupStampX               = GetTickCount();
m_maxCharactersTemp          = 0;
m_worldChannelTemp           = 0;
m_playerEntityCfgOutbox      = NULL;
m_attributeOutbox            = NULL;
m_detailMsgOutbox            = NULL;
m_localClientContentVersion  = 0;
m_accountName                = name;
m_accountPassword            = password;
m_loggedOn                   = FALSE;//online status
m_accountStatus              = 0;//0=ok 1=suspended for payment reasons 2=suspended for violation 
m_suspentionNotes            = "None";//description of suspension reasons
m_currentID                  = 0;//
m_registeredVersion          = FALSE;
m_timePlayedMinutes          = 0;
m_tempPlayedTimeMilliseconds = 0;
m_logonTimeStamp             = 0;
m_characterOnAccount         = NULL;
m_currentcharInUse           = -1;
m_accountNumber              = 0;
m_aiServer                   = FALSE;
m_securityIndexer            = 0;
m_administratorAccess        = FALSE;

m_realName                   = "Unknown";//as apears on credit card
m_email                      = "unknown@unknown.com";
m_yearOfLastLogon            = 0;
m_monthOfLastLogon           = 0;
m_dayOfLastLogon             = 0;
m_hourOfLastLogon            = 0;
m_minuteOfLastLogon          = 0;
m_refreshPaymentYear         = 0;
m_refreshPaymentMonth        = 0;
m_refreshPaymentDay          = 0;
m_paidMonths                 = 0;
m_housingBind                = 0;//0 = none
m_initializedHousingStates   = FALSE;
m_gm                         = FALSE;
m_canSpawn                   = FALSE;
m_syncTrackingTime           = 0;
m_syncViolations             = 0;
m_lastELtime                 = (DWORD)-9999999999999;
StampLogOn();
m_expansionID                = 0;
m_syncTestReset              = 0;
m_charsCountDMMode		     = 0;
m_throttleStamp              = GetTickCount();
m_connectedTemp              = FALSE;

}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CAccountObject::Clone(CAccountObject ** clone)
{
	 CAccountObject * copyLocal = new CAccountObject(m_accountName,m_accountPassword);     
  
     copyLocal->m_loggedOn                   = m_loggedOn;//online status
     copyLocal->m_accountStatus              = m_accountStatus;//0=ok 1=suspended for payment reasons 2=suspended for violation 
     copyLocal->m_suspentionNotes            = m_suspentionNotes;//description of suspension reasons
     copyLocal->m_currentID                  = m_currentID;//
     copyLocal->m_registeredVersion          = m_registeredVersion;
     copyLocal->m_timePlayedMinutes          = m_timePlayedMinutes;
     copyLocal->m_tempPlayedTimeMilliseconds = m_tempPlayedTimeMilliseconds;
     copyLocal->m_logonTimeStamp             = m_logonTimeStamp;
     copyLocal->m_currentcharInUse           = m_currentcharInUse;
     copyLocal->m_accountNumber              = m_accountNumber;
     copyLocal->m_aiServer                   = m_aiServer;
     copyLocal->m_securityIndexer            = m_securityIndexer;
     copyLocal->m_administratorAccess        = m_administratorAccess;
     copyLocal->m_realName                   = m_realName;//as apears on credit card
     copyLocal->m_email                      = m_email;
     copyLocal->m_yearOfLastLogon            = m_yearOfLastLogon;
     copyLocal->m_monthOfLastLogon           = m_monthOfLastLogon;
     copyLocal->m_dayOfLastLogon             = m_dayOfLastLogon;
     copyLocal->m_hourOfLastLogon            = m_hourOfLastLogon;
     copyLocal->m_minuteOfLastLogon          = m_minuteOfLastLogon;
     copyLocal->m_refreshPaymentYear         = m_refreshPaymentYear;
     copyLocal->m_refreshPaymentMonth        = m_refreshPaymentMonth;
     copyLocal->m_refreshPaymentDay          = m_refreshPaymentDay;
     copyLocal->m_paidMonths                 = m_paidMonths;
     copyLocal->m_housingBind                = m_housingBind;//0 = none
     copyLocal->m_initializedHousingStates   = m_initializedHousingStates;
	 copyLocal->m_gm                         = m_gm;
	 copyLocal->m_canSpawn                   = m_canSpawn;
	 copyLocal->m_worldChannelTemp           = m_worldChannelTemp;
	 copyLocal->m_currentlyPlayingGameName   = m_currentlyPlayingGameName;

	 if(m_characterOnAccount)
		m_characterOnAccount->Clone(&copyLocal->m_characterOnAccount);

     *clone = copyLocal;

}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
BOOL CAccountObject::AllowAsyncQue()
{
   DWORD curTime = GetTickCount();
   if(curTime-m_throttleStamp > 1000)
   {
      m_throttleStamp = curTime;
	  return TRUE;
   }
   return FALSE;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
BOOL CAccountObject::AllowAsyncQueMini()
{
   DWORD curTime = GetTickCount();
   if(curTime-m_throttleStamp > 200)
   {
      m_throttleStamp = curTime;
	  return TRUE;
   }
   return FALSE;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CAccountObject::CloneMinimum(CAccountObject ** clone)
{
	 CAccountObject * copyLocal = new CAccountObject(m_accountName,m_accountPassword);     
  
     copyLocal->m_loggedOn                   = m_loggedOn;//online status
     copyLocal->m_accountStatus              = m_accountStatus;//0=ok 1=suspended for payment reasons 2=suspended for violation 
     copyLocal->m_suspentionNotes            = m_suspentionNotes;//description of suspension reasons
     copyLocal->m_currentID                  = m_currentID;//
     copyLocal->m_registeredVersion          = m_registeredVersion;
     copyLocal->m_timePlayedMinutes          = m_timePlayedMinutes;
     copyLocal->m_tempPlayedTimeMilliseconds = m_tempPlayedTimeMilliseconds;
     copyLocal->m_logonTimeStamp             = m_logonTimeStamp;
     copyLocal->m_currentcharInUse           = m_currentcharInUse;
     copyLocal->m_accountNumber              = m_accountNumber;
     copyLocal->m_aiServer                   = m_aiServer;
     copyLocal->m_securityIndexer            = m_securityIndexer;
     copyLocal->m_administratorAccess        = m_administratorAccess;
     copyLocal->m_realName                   = m_realName;//as apears on credit card
     copyLocal->m_email                      = m_email;
     copyLocal->m_yearOfLastLogon            = m_yearOfLastLogon;
     copyLocal->m_monthOfLastLogon           = m_monthOfLastLogon;
     copyLocal->m_dayOfLastLogon             = m_dayOfLastLogon;
     copyLocal->m_hourOfLastLogon            = m_hourOfLastLogon;
     copyLocal->m_minuteOfLastLogon          = m_minuteOfLastLogon;
     copyLocal->m_refreshPaymentYear         = m_refreshPaymentYear;
     copyLocal->m_refreshPaymentMonth        = m_refreshPaymentMonth;
     copyLocal->m_refreshPaymentDay          = m_refreshPaymentDay;
     copyLocal->m_paidMonths                 = m_paidMonths;
     copyLocal->m_housingBind                = m_housingBind;//0 = none
     copyLocal->m_initializedHousingStates   = m_initializedHousingStates;
	 copyLocal->m_gm                         = m_gm;
	 copyLocal->m_canSpawn                   = m_canSpawn;
	 copyLocal->m_worldChannelTemp           = m_worldChannelTemp;

	 if(m_characterOnAccount)
		m_characterOnAccount->Clone(&copyLocal->m_characterOnAccount);

     *clone = copyLocal;

}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CAccountObject::WriteDebugFile(CString filename,CString debugString)
{
	CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
       the_out_Archive.WriteString( debugString );  
	   //the_out_Archive.WriteString( "\n");  
	
	 the_out_Archive.Close();
     fl.Close(); 
}
//---------------------------------------------------//
//---------------------------------------------------//
//---------------------------------------------------//
CString CAccountObject::ConvLongToString(DWORD number)
{ 
CString temp;	 
char buffer[20];
_ultoa_s( number,buffer,10);
temp = buffer;
return temp;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CPlayerObject * CAccountObject::GetCurrentCharacter()
{
    //if(m_currentcharInUse < 0)
	  // return NULL;

	if(!m_characterOnAccount)
		return NULL;

	return m_characterOnAccount;
}
//-----------------------------------------------------------//
void CAccountObject::AddEntityCfgMsg(int size,BYTE * data,DPNID id,int channelID)
{
	if(!m_playerEntityCfgOutbox)
		m_playerEntityCfgOutbox = new CPacketObjList();
	
    m_playerEntityCfgOutbox->AddPacket(size,data,id,channelID);
}
//-----------------------------------------------------------//
void CAccountObject::AddAttribMsg(int size,BYTE * data,DPNID id,int channelID)
{
	if(!m_attributeOutbox)
		m_attributeOutbox = new CPacketObjList();
	
    m_attributeOutbox->AddPacket(size,data,id,channelID);
}
//-----------------------------------------------------------//
void CAccountObject::AddDetailMsg(int size,BYTE * data,DPNID id,int channelID)
{
	if(!m_detailMsgOutbox)
		m_detailMsgOutbox = new CPacketObjList();
	
    m_detailMsgOutbox->AddPacket(size,data,id,channelID);
}
//-----------------------------------------------------------//
void CAccountObject::StampLogOn()
{
	/*CTime t = CTime::GetCurrentTime();
	
	m_yearOfLastLogon            = t.GetYear();
    m_monthOfLastLogon           = t.GetMonth();
    m_dayOfLastLogon             = t.GetDay();
    m_hourOfLastLogon            = t.GetHour();
	m_minuteOfLastLogon          = t.GetMinute();*/

}
//---------------------------------------------------//
CString CAccountObjectList::ConvLongToString(DWORD number)
{ 
CString temp;	 
char buffer[20];
_ultoa_s( number,buffer,10);
temp = buffer;
return temp;
}
//-----------------------------------------------------------//
bool CAccountObjectList::ToggleGMStatus_safe(CString username)
{
	CAccountObject * acPtr = GetAccountByUsername_LOS(username.GetBuffer());//LOS means locked on success
	if(acPtr)
	{
	   if(acPtr->m_gm)
		  acPtr->m_gm = FALSE;
	   else
          acPtr->m_gm = TRUE;

	   //locks by username must unlock by username
	   Unlock();
	}
	return false;
}
//-----------------------------------------------------------//
bool CAccountObjectList::SetDPNID(CString userName,
								  CString password,
								  DPNID currentDPNID)
{
     CAccountObject * acPtr = GetAccountByUsername_LOS(userName.GetBuffer());
	 if(acPtr)
	 {
		 acPtr->m_currentID			= currentDPNID;
		 acPtr->m_loggedOn			= TRUE;
		 acPtr->m_syncTrackingTime	= 0;
		 acPtr->m_syncViolations	= 0;
		 //if(cAccntPtr)
		   //*cAccntPtr = acPtr;//this would make it unsafe

		 Unlock();
	 }

	 return true;
}
//-----------------------------------------------------------//
int CAccountObjectList::LogOutRuntime_safe(  CString username,
										     int * totalMinutesPlayed)
{
    int minPlayed = RemoveByName_safe(username.GetBuffer());
	if(minPlayed < 0)
	   return 1;//account not found
	
    *totalMinutesPlayed = minPlayed;

	return 0;//success-removed account
}
//-----------------------------------------------------------//
BOOL CAccountObjectList::UpdateAccount(CPlayerObject  * runtimePlayerObj,
									   CAccountObject * accountPtr)
{
	DWORD goodBehaviorDuration              = 300000;

	if(!accountPtr)
		return FALSE;

	if(!runtimePlayerObj)
		return FALSE;
	
	if(accountPtr->m_currentcharInUse < 0)
	   return FALSE;

    if(!accountPtr->m_characterOnAccount)
	{
        accountPtr->m_currentcharInUse  = -1;
	    return FALSE;	
	}

		CPlayerObject * playerObject    = accountPtr->m_characterOnAccount;

		playerObject->m_countDownToMurderPardon = runtimePlayerObj->m_countDownToMurderPardon;
		playerObject->m_3dPosition.x            = runtimePlayerObj->m_3dPosition.x;
		playerObject->m_3dPosition.y            = runtimePlayerObj->m_3dPosition.y;
		playerObject->m_3dPosition.z            = runtimePlayerObj->m_3dPosition.z;
		playerObject->m_energy                  = runtimePlayerObj->m_energy;
		playerObject->m_maxEnergy               = runtimePlayerObj->m_maxEnergy;
		playerObject->m_aiCfgOfMounted          = runtimePlayerObj->m_aiCfgOfMounted;
		playerObject->m_clanHandle              = runtimePlayerObj->m_clanHandle;
		playerObject->m_arenaBasePoints         = runtimePlayerObj->m_arenaBasePoints;
        playerObject->m_arenaTotalKills         = runtimePlayerObj->m_arenaTotalKills;
		playerObject->m_instanceType			= runtimePlayerObj->m_instanceType;
		playerObject->m_groupID					= runtimePlayerObj->m_groupID;
		playerObject->m_teamTemp				= runtimePlayerObj->m_teamTemp;
		playerObject->m_clanAbbrev				= runtimePlayerObj->m_clanAbbrev;
		playerObject->m_arenaBetAmount			= runtimePlayerObj->m_arenaBetAmount;
		playerObject->m_factionStanding			= runtimePlayerObj->m_factionStanding;
		playerObject->m_arenaItemsGiven			= runtimePlayerObj->m_arenaItemsGiven;

		playerObject->m_groupStruct.m_groupLeadership		= runtimePlayerObj->m_groupStruct.m_groupLeadership;
		playerObject->m_groupStruct.m_groupPlacementIndex	= runtimePlayerObj->m_groupStruct.m_groupPlacementIndex;
		
		//update inventory
		if(playerObject->m_currentInventory)
		{
		   playerObject->m_currentInventory->SafeDelete();
		   delete playerObject->m_currentInventory;
		   playerObject->m_currentInventory=0;
		}
		runtimePlayerObj->m_currentInventory->Clone(&playerObject->m_currentInventory);	

        if(playerObject->m_reconfigInven)
		{
		   playerObject->m_reconfigInven->SafeDelete();
		   delete playerObject->m_reconfigInven;
		   playerObject->m_reconfigInven=0;
		}
		if(runtimePlayerObj->m_reconfigInven)
		runtimePlayerObj->m_reconfigInven->Clone(&playerObject->m_reconfigInven);	

		if(playerObject->m_noteriety)
		{
		   delete playerObject->m_noteriety;
		   playerObject->m_noteriety=0;
		}
		runtimePlayerObj->m_noteriety->Clone(&playerObject->m_noteriety);	

		//update skills gained
        if(playerObject->m_stats != NULL && runtimePlayerObj->m_stats != NULL){//valid for stats
           playerObject->m_stats->SafeDelete();
	       delete playerObject->m_stats;
		   playerObject->m_stats=0;
	       runtimePlayerObj->m_stats->Clone(&playerObject->m_stats);
		}//end valid for stats
		
	return TRUE;
}
//------------------------------------------------------------------//
void CAccountObjectList::DisconnectAll_safe(void * serverInterfaceMem)
{
	 AuraNetHost * serverInterface = (AuraNetHost *)serverInterfaceMem;   // ENet host (was IDirectPlay8Server)
	 if(!serverInterface) return;
	 Lock();
     if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
	   for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
	   {
				 for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
				 {
					 CAccountObject * delPtr = (CAccountObject *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
					 serverInterface->DisconnectPlayer((AuraNetID)delPtr->m_currentID);
				 }
	   }
     Unlock();
}
//------------------------------------------------------------------//
bool CAccountObjectList::Add_safe(CAccountObject * playerMem)
{
     if(!playerMem)//assert on add index-return false here
         return false;

	 Lock();
	 bool res = true;
     if(!AddIndexed((void*)playerMem, playerMem->m_accountNumber, playerMem->m_accountNumber,"account_num"))//int
		 res = false;
     if(!AddIndexed((void*)playerMem, playerMem->m_currentID, playerMem->m_currentID,"dpnid"))//dpnid dword/unsigned int
		 res = false;
     if(!AddIndexed((void*)playerMem, GetSimpleHash(playerMem->m_accountName.GetBuffer()), playerMem->m_currentID,"username"))//username text
		 res = false;

	 if(playerMem->m_aiServer)//selective add
	   if(!AddIndexed((void*)playerMem, playerMem->m_aiServer, playerMem->m_currentID,"aiserver"))//non unique
		 res = false;
     Unlock();
	 return res;
}
//------------------------------------------------------------------//
BOOL CAccountObjectList::DeleteByOject(CAccountObject * playerMem)//must be manually locked
{
     if(!playerMem)
		 return FALSE;//should assert when used

	 BOOL res = FALSE;
	 if(RemoveIndexed(playerMem->m_accountNumber,playerMem->m_accountNumber, "account_num"))
        if(RemoveIndexed(playerMem->m_currentID,playerMem->m_currentID, "dpnid"))
           if(RemoveIndexed(GetSimpleHash(playerMem->m_accountName.GetBuffer()),playerMem->m_currentID,"username"))
	          res = TRUE;
			   
	 if(playerMem->m_aiServer)
	    if(!RemoveIndexed(playerMem->m_aiServer,playerMem->m_currentID, "aiserver"))
			res = FALSE;

	 delete playerMem;
	 playerMem = 0;

	 return res;
}
//------------------------------------------------------------------//
int CAccountObjectList::RemoveByName_safe(char * name)//return minutes connected -1 = not found
{
	 CAxList * obj = GetAllOnIndex(GetSimpleHash(name),"username");

	 Lock();
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_accountName.CompareNoCase(name)==0)
		 {
		     int minutesConnected = plObj->m_timePlayedMinutes;
			 DeleteByOject(plObj);
			 Unlock();
			 return minutesConnected;
		 }
	 }
	 Unlock();
     return -1;
}
//------------------------------------------------------------------//
CAccountObject * CAccountObjectList::GetAccountByUsername_LOS(char * name)
{
	 CAxList * obj = GetAllOnIndex(GetSimpleHash(name),"username");
	 if(!obj)
	    return NULL;

	 Lock();
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_accountName.CompareNoCase(name)==0)
		 {
		    return plObj;
		 }
	 }
	 Unlock();
     return NULL;
}
//------------------------------------------------------------------//
BOOL CAccountObjectList::VerifyAdmin_safe(DPNID dpnid)
{
	 CAxList * obj = GetAllOnIndex(dpnid,"dpnid");

	 BOOL res = FALSE;
	 Lock();
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_currentID==dpnid)
		  if(plObj->m_administratorAccess)
		  {
		    res = TRUE;
			break;
		  }
	 }
	 Unlock();
     return res;
}
//------------------------------------------------------------------//
BOOL CAccountObjectList::VerifyAIserver_safe(DPNID dpnid,BOOL & isAIserver)
{
	 CAxList * obj = GetAllOnIndex(dpnid,"dpnid");

	 Lock();
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_currentID==dpnid)
		  {
		    isAIserver = plObj->m_aiServer;
			Unlock();
			return TRUE;
		  }
	 }
	 Unlock();
     return FALSE;
}
//------------------------------------------------------------------//
BOOL CAccountObjectList::IsAdministrator_safe(DPNID id)
{
     CAxList * obj = GetAllOnIndex(id,"dpnid");

	 Lock();
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_currentID==id)
		 if(plObj->m_administratorAccess)
		 {
			Unlock();
			return TRUE;
		 }
	 }
	 Unlock();
     return FALSE;
}
//------------------------------------------------------------------//
CAccountObject * CAccountObjectList::GetFirstAIServer()
{
	 CAxList * obj = GetAllOnIndex(1,"aiserver");

     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_aiServer)
			return plObj;//return locked
	 }
     return FALSE;
}
//------------------------------------------------------------------//
CAccountObject * CAccountObjectList::GetAccountByID_LOS(DPNID dpnid)
{
	 Lock();
	 CAxLinkObj * obj = GetByUniqueID(dpnid,dpnid,"dpnid");
	 if(obj)
	    return (CAccountObject *)obj->m_thisObject;//returns locked
	 Unlock();
     return NULL;
}
//------------------------------------------------------------------//
CAccountObject * CAccountObjectList::GetAccountByID(DPNID dpnid)
{
	 CAxLinkObj * obj = GetByUniqueID(dpnid,dpnid,"dpnid");
	 if(obj)
	    return (CAccountObject *)obj->m_thisObject;//returns locked
     return NULL;
}
//------------------------------------------------------------------//
BOOL CAccountObjectList::IndexSecurityIndexerByDPNID_safe(DPNID & dpnid,int & indexAmount,int * result)
{
	 CAxList * obj = GetAllOnIndex(dpnid,"dpnid");
	 Lock();
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_currentID==dpnid)
		 {
		    plObj->m_securityIndexer += indexAmount;
            if(plObj->m_securityIndexer > 30000)
		       plObj->m_securityIndexer = 0;

			if(result)
			   *result = plObj->m_securityIndexer;
			Unlock();
		    return TRUE;
		 }
	 }
	 Unlock();
     return FALSE;
}
//------------------------------------------------------------------//
BOOL CAccountObjectList::IndexSecurityIndexerByDPNID(DPNID & dpnid,int & indexAmount,int * result)
{
	 CAxList * obj = GetAllOnIndex(dpnid,"dpnid");
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_currentID==dpnid)
		 {
		    plObj->m_securityIndexer += indexAmount;
            if(plObj->m_securityIndexer > 30000)
		       plObj->m_securityIndexer = 0;

			if(result)
			   *result = plObj->m_securityIndexer;
		    return TRUE;
		 }
	 }
     return FALSE;
}
//------------------------------------------------------------------//
CString CAccountObjectList::GetAccountNameByDPNID_safe(DPNID dpnid)
{
	 CAxList * obj = GetAllOnIndex(dpnid,"dpnid");
	 Lock();
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_currentID==dpnid)
		 {
		    CString nameRet = plObj->m_accountName;
			Unlock();
		    return nameRet;
		 }
	 }
	 Unlock();
     return "**GetAccountNameByDPNID() Cant Find Account**";
}
//------------------------------------------------------------------//
CString CAccountObjectList::GetAccountNameByDPNID(DPNID dpnid)
{
	 CAxList * obj = GetAllOnIndex(dpnid,"dpnid");
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_currentID==dpnid)
		 {
		    CString nameRet = plObj->m_accountName;
		    return nameRet;
		 }
	 }
     return "**GetAccountNameByDPNID() Cant Find Account**";
}
//------------------------------------------------------------------//
CPlayerObject * CAccountObjectList::GetCurrentCharacterByID(DPNID dpnid)
{
	 CAxList * obj = GetAllOnIndex(dpnid,"dpnid");
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_currentID==dpnid)
		 {
			return plObj->m_characterOnAccount;
		 }
	 }
     return NULL;
}
//------------------------------------------------------------------------------------//
BOOL CAccountObjectList::HandlePcInventoryOnDeath(	CPlayerObject * requestedData,
											        CPlayerObject * characterOnAccount,
											        CMovementObj * entObjDB,
											        BOOL & specialNonDropZone,
													BOOL noDisarm)
{
    if(!entObjDB)
		return FALSE;

	if(!characterOnAccount)
		return FALSE;

	
	//handle inventory loss switch or reset
    if(characterOnAccount->m_currentInventory)
	{//valid inventory
	   POSITION invPosLast = NULL;
       for(POSITION invPos = characterOnAccount->m_currentInventory->GetHeadPosition();(invPosLast = invPos) != NULL;)
	   {//inventory loop
         CInventoryObj * invObj = (CInventoryObj *)characterOnAccount->m_currentInventory->GetNext(invPos);
		 //if(!invObj->m_itemData->m_permanent)
		 //{//dont deal with perminent
		  if(invObj->m_itemData->m_disarmable == TRUE || invObj->m_armed == FALSE || invObj->m_itemData->m_nonDrop == TRUE)
		  {//disarmable so remove it from account
			  if(invObj->m_itemData->m_permanent)
				  continue;

			  invObj->SafeDelete();
			  delete invObj;
			  invObj = 0;
              characterOnAccount->m_currentInventory->RemoveAt(invPosLast);	
		  }//end disarmable so remove it from account
	      else{//non disarmable, check death cfg before switching out to default (death robes)
			   BOOL foundOneTemp = FALSE;
               if(entObjDB)
			   {//DB entity ref valid
				   if(entObjDB->m_deathCfg)
                      for(POSITION dPos = entObjDB->m_deathCfg->GetHeadPosition();dPos != NULL;)
					  {//death cfg loop
                         CCfgObj * deathCfg = (CCfgObj *)entObjDB->m_deathCfg->GetNext(dPos);
					     CItemObj * dItemRef = entObjDB->GetItemByID(deathCfg->m_globalCFG);
						 if(dItemRef)
					     if(dItemRef->m_locationID == invObj->m_itemData->m_locationID)
						 {//found item on death cfg
						    invObj->SafeDelete();
                            delete invObj;
						    invObj = 0;
                            characterOnAccount->m_currentInventory->RemoveAt(invPosLast); 
							foundOneTemp = TRUE;
						    break;
						 }//end found item on death cfg
					  }//end death cfg loop
			   }//end DB entity ref valid
			   if(!foundOneTemp)
			   {
                   if(!invObj->m_itemData->m_permanent || !invObj->m_itemData->m_isInQuickSlots)
				   {
			          invObj->SafeDelete();
			          delete invObj;
			          invObj = 0;
                      characterOnAccount->m_currentInventory->RemoveAt(invPosLast);	
				   }
			   }
		  }//end non disarmable, check death cfg before switching out to default (death robes)
		 //}//end dont deal with perminent
	   }//end inventory loop
		  POSITION lastInvNonDropLoc;
		  for(POSITION invNonDropLoc = requestedData->m_currentInventory->GetHeadPosition();(lastInvNonDropLoc = invNonDropLoc) != NULL;)
		  {//replace items that are non drop
             CInventoryObj * invObj = (CInventoryObj *)requestedData->m_currentInventory->GetNext(invNonDropLoc);
		     if(invObj->m_itemData->m_nonDrop == TRUE 
			 || specialNonDropZone == TRUE
			 || invObj->m_itemData->m_isInQuickSlots)
			 {//handle non drop replacement
                 if(invObj->m_itemData->m_permanent)
					 continue;

				 if(noDisarm)
					characterOnAccount->m_currentInventory->AddInventoryItem(invObj->m_quanity,invObj->m_armed,invObj->m_itemData);
				 else
					characterOnAccount->m_currentInventory->AddInventoryItem(invObj->m_quanity,FALSE,invObj->m_itemData);

                 invObj->SafeDelete();
			     delete invObj;
				 invObj = 0;
                 requestedData->m_currentInventory->RemoveAt(lastInvNonDropLoc);
			 }//end handle non drop replacement
		  }//end replace items that are non drop
		  //add death cfg
	   if(entObjDB->m_deathCfg)
          for(POSITION dPos = entObjDB->m_deathCfg->GetHeadPosition();dPos != NULL;)
		  {//death cfg loop
            CCfgObj * deathCfg = (CCfgObj *)entObjDB->m_deathCfg->GetNext(dPos);
		    CItemObj * dItemRef = entObjDB->GetItemByID(deathCfg->m_globalCFG);
			if(dItemRef)
			{
			 CInventoryObj * querryIPtr = NULL;
			 
			 if(!dItemRef->m_permanent)
			 if(characterOnAccount->m_stats)
			 querryIPtr = characterOnAccount->m_currentInventory->GetInvenItemByLocationIDArmable(dItemRef->m_locationID,
			                                                                                      characterOnAccount->m_stats->GetStatValueByID(2,characterOnAccount->m_currentInventory),
																								  characterOnAccount->m_stats->GetStatValueByID(3,characterOnAccount->m_currentInventory),
																								  characterOnAccount->m_stats->GetStatValueByID(4,characterOnAccount->m_currentInventory));

			 if(!querryIPtr)
			     querryIPtr = characterOnAccount->m_currentInventory->GetByGLID(dItemRef->m_globalID);
			 
			 if(!querryIPtr)
			 {
			    characterOnAccount->m_currentInventory->AddInventoryItem(1,
					                                                     TRUE,
					                                                     dItemRef);
			 }
			 else
			 {
                if(!characterOnAccount->m_currentInventory->DoesArmedLocationIDExist(querryIPtr->m_itemData->m_locationID))
			        querryIPtr->m_armed = TRUE;
			 }
			} 
		  }//end death cfg loop
	      //end add death cfg
	}//end valid inventory
	
	//Save the reconfig inventory on death
	if(!characterOnAccount->m_reconfigInven)
		characterOnAccount->m_reconfigInven = new CInventoryObjList();
	if(characterOnAccount->m_reconfigInven)
	{//valid character account reconfig
		if(requestedData->m_reconfigInven && (requestedData->m_reconfigInven->GetCount() > 0))
		{//valid player reconfig
			if(characterOnAccount->m_reconfigInven->GetCount() > 0)
			{
				POSITION lastPos;
				for(POSITION pos = characterOnAccount->m_reconfigInven->GetHeadPosition(); (lastPos = pos) != NULL;)
				{
					CInventoryObj *pItem = dynamic_cast<CInventoryObj*>(characterOnAccount->m_reconfigInven->GetNext(pos));
						if(!pItem) 
							continue;
					pItem->SafeDelete();
					SafeDelete(pItem);
					characterOnAccount->m_reconfigInven->RemoveAt(lastPos);
				}
			}
			if(characterOnAccount->m_reconfigInven->GetCount() == 0)
			{
				POSITION lastPosLoc;
				for(POSITION posLoc = requestedData->m_reconfigInven->GetHeadPosition(); (lastPosLoc = posLoc) != NULL;)
				{//replace items
					CInventoryObj * invObj = (CInventoryObj *)requestedData->m_reconfigInven->GetNext(posLoc);
					if(invObj && invObj->m_itemData)
					{//valid item
						characterOnAccount->m_reconfigInven->AddInventoryItem(invObj->m_quanity,invObj->m_armed,invObj->m_itemData);
						invObj->SafeDelete();
						delete invObj;
						invObj = 0;
						requestedData->m_reconfigInven->RemoveAt(lastPosLoc);
					}//end valid item
				}//end replace items
			}
		}//end valid player reconfig 
	}//end valid character account reconfig
	return TRUE;
}



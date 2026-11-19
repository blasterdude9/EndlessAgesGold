#ifndef CACCOUNTCLASS_H
#define CACCOUNTCLASS_H

#include "dplay8.h"
#include "CDplayPlayerClass.h"
#include "CInventoryClass.h"
#include "CMovementModel.h"

class CAccountObject
{
	CAccountObject(){m_currentCharRequestCount=0;}
	 
public:

	~CAccountObject()
	{
        if(m_characterOnAccount){
	       delete m_characterOnAccount;
	       m_characterOnAccount=0;
		}
		if(m_playerEntityCfgOutbox)
		{
		   delete m_playerEntityCfgOutbox;
		   m_playerEntityCfgOutbox = 0;
		}
		if(m_attributeOutbox)
		{
		   delete m_attributeOutbox;
		   m_attributeOutbox = 0;
		}
		if(m_detailMsgOutbox)
		{
		   delete m_detailMsgOutbox;
		   m_detailMsgOutbox = 0;
		}
	}
	
	CAccountObject(CString name,
				   CString password
		          );

	CString                 m_accountName;//plaers name
    CString                 m_accountPassword;
	BOOL                    m_loggedOn;//online status
	int                     m_accountStatus;//0=ok 1=suspended for payment reasons 2=suspended for violation 
	CString                 m_suspentionNotes;//description of suspension reasons
	DPNID                   m_currentID;//
	BOOL                    m_registeredVersion;
	long                    m_timePlayedMinutes;
	long                    m_tempPlayedTimeMilliseconds;
	CString                 m_currentIP;
	long                    m_logonTimeStamp;
	int                     m_currentcharInUse;//-1 = none
	int                     m_accountNumber;//
	BOOL                    m_deafMute;

	BOOL                    m_aiServer;
	
	CPlayerObject          *m_characterOnAccount;
	short                   m_securityIndexer;

	BOOL                    m_administratorAccess;
	BOOL                    m_gm;
	BOOL                    m_canSpawn;

	int                     m_housingBind;

	BOOL                    m_initializedHousingStates;

	//extra vars
	int                     m_localClientContentVersion;

	
	//Functions

	//ai server only storage for channeling
	CPacketObjList        * m_playerEntityCfgOutbox;
	CPacketObjList        * m_attributeOutbox;
	CPacketObjList        * m_detailMsgOutbox;


	//status info
	//CTime t = CTime::GetCurrentTime();
    //CString s = t.Format( "%A, %H %M, %S" );
	CString                 m_realName;//as apears on credit card
    CString                 m_email;
	int                     m_yearOfLastLogon;
	int                     m_monthOfLastLogon;
	int                     m_dayOfLastLogon;
	int                     m_hourOfLastLogon;
	int                     m_minuteOfLastLogon;

	//validity-to play
	int                     m_refreshPaymentYear;
	int                     m_refreshPaymentMonth;
	int                     m_refreshPaymentDay;
	int                     m_paidMonths;

	int                     m_charsCountDMMode;//only valid in DB mode

	//misc timers
	DWORD                   m_syncTrackingTime;
	int                     m_syncViolations;
	DWORD                   m_lastELtime;
	DWORD                   m_syncTestReset;
	DWORD                   m_throttleStamp;

	//Expansion Index
	int                     m_expansionID;

	int                     m_worldChannelTemp;
	int                     m_maxCharactersTemp;
	DWORD                   m_backupStampX;

	CString                 m_currentlyPlayingGameName;
	//limitors and throttles
	int                     m_currentCharRequestCount;
	BOOL                    m_connectedTemp;

	//FUNCTIONS
    void                    Clone(CAccountObject ** clone);
	void                    CloneMinimum(CAccountObject ** clone);
	void                    StampLogOn();
	CPlayerObject *         GetCurrentCharacter();
	void                    WriteDebugFile(CString filename,CString debugString);
	CString                 ConvLongToString(DWORD number);
	void					AddEntityCfgMsg(int size,BYTE * data,DPNID id,int channelID);
	void					AddAttribMsg(int size,BYTE * data,DPNID id,int channelID);
	void					AddDetailMsg(int size,BYTE * data,DPNID id,int channelID);
	BOOL                    AllowAsyncQue();
	BOOL                    AllowAsyncQueMini();

};

class CAccountObjectList : public CAxDB
{
public:
   CAccountObjectList()
   {
		  Init(2000,4);//determines memory commit and speed of query
		  AddIndexVariable(0,"account_num");//account_num unique
		  AddIndexVariable(1,"dpnid");//dpnid unique in account situation-good news
		  AddIndexVariable(2,"username");//username-unique but hash id non-unique and can cause stacking somtimes
		  AddIndexVariable(3,"aiserver");
   };

   ~CAccountObjectList()
   {
          //traverse list manually to delete core class objects, the list will clear itself when deconstructed
		  Lock();
		  this->WriteStatus("cleaned_CAccountObjectList.txt");
		  if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
			 for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
			 {
				 for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
				 {
					 CAccountObject * delPtr = (CAccountObject *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
					 delete delPtr;
					 delPtr = 0;
				 }
			 }
		  //internally the rest of the lsists should auto clean themselfs
		  Unlock();
   };

   //variables

   //Functions
   BOOL             IsAdministrator_safe(DPNID id);
   bool				Add_safe(CAccountObject * playerMem);
   BOOL				DeleteByOject(CAccountObject * playerMem);
   int				RemoveByName_safe(char * name);
   CAccountObject * GetAccountByUsername_LOS(char * name);
   CAccountObject * GetAccountByID_LOS(DPNID dpnid);
   CAccountObject * GetAccountByID(DPNID dpnid);
   CString			ConvLongToString(DWORD number);
   BOOL				UpdateAccount(CPlayerObject  * runtimePlayerObj,CAccountObject * accountPtr);
   bool				ToggleGMStatus_safe(CString username);
   int				LogOutRuntime_safe(CString username,int * totalMinutesPlayed);
   bool				SetDPNID(CString userName,CString password,DPNID currentDPNID);  
   void				DisconnectAll_safe(void * serverInterfaceMem);
   BOOL             VerifyAdmin_safe(DPNID dpnid);
   BOOL				VerifyAIserver_safe(DPNID dpnid,BOOL & isAIserver);
   CPlayerObject *  GetCurrentCharacterByID(DPNID dpnid);
   CAccountObject * GetFirstAIServer();
   CString			GetAccountNameByDPNID_safe(DPNID dpnid);
   CString			GetAccountNameByDPNID(DPNID dpnid);
   BOOL				IndexSecurityIndexerByDPNID_safe(DPNID & dpnid,int & indexAmount,int * result);
   BOOL				IndexSecurityIndexerByDPNID(DPNID & dpnid,int & indexAmount,int * result);
   BOOL				HandlePcInventoryOnDeath(CPlayerObject * requestedData,CPlayerObject * characterOnAccount,CMovementObj * entObjDB,BOOL & specialNonDropZone,BOOL noDisarm);
};

#endif
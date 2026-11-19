#pragma once
#include "CAxList.h"
#include "DBConnectionSystem.h"

//maintains a non-spammable que for realtime coross node critical data
//all data reacts with the db as fast as it can async
//---------------------------------------------------
//CLAN_REQUESTTYPES 
//CLAN_CREATE this will check to see if it already exists, then creates it, the creators handle makes 
			//him the leader and unique handle for the clan, if a character is deleted so is the clan and all handles set to -1 on gmCharacters
//CLAN_DELETE this will delete the core object on the db that shared, if at anytime a clan 
			//id is attempted to be accessed and doesnt exist it will clear the clan handle on the db characters
enum CLAN_REQUESTTYPES
{
	CLAN_CREATE = 1,	
	CLAN_DELETE,
	CLAN_CHAT,
	CLAN_INVITE,
	CLAN_REMOVEMEMBER,
	CLAN_ACCEPT,
	CLAN_REQUESTMEMBERS,
	CLAN_MAX
};

class CAxClanCommand
{
	public:
		CAxClanCommand(int & command, int & requestHandle, CString & param1, CString & param2)
		{
			m_requestType		= command;
			m_fromhandle		= requestHandle;
			m_param1			= param1;
			m_param2			= param2;
		}
		~CAxClanCommand(){}

	int				m_requestType;
	int				m_fromhandle;
	CString			m_param1;
	CString			m_param2;
};

class CAxClanSystemComm : public CAxList
{
	public:
		CAxClanSystemComm()
		{
			InitializeCriticalSection(&m_sect);

			m_addClan				= "INSERT INTO GMCLANDATA (ABBREV, HANDLE) Values('$ABBREV','$HANDLE')";
			m_deleteClan			= "DELETE FROM GMCLANDATA WHERE ABBREV = '$ABBREV'";
			m_doesClanExist			= "SELECT ABBREV FROM GMCLANDATA WHERE ABBREV = '$ABBREV'";
			m_doesClanExistByHandle	= "SELECT ABBREV FROM GMCLANDATA WHERE HANDLE = '$HANDLE'";
			m_doescharExist			= "SELECT CLANID FROM GMCHARACTER WHERE CHARNAME = '$CHARNAME'";
			m_removeFromClan		= "UPDATE GMCHARACTERS SET CLANID = '$CLANID' WHERE CHARNAME = '$CHARNAME'";
		}
		~CAxClanSystemComm()
		{
			for(CAxLinkObj * pos = GetHeadPosition_unsafe();pos != NULL;)
			{//in this case we do have to run this loop the CAxClanCommand isnt derive from CAxLinkObj
				CAxClanCommand * pData = (CAxClanCommand *)GetNext_unsafe(&pos);
				delete pData;
				pData = 0;
			}
			RemoveAll();
			DeleteCriticalSection(&m_sect);
		}

		CRITICAL_SECTION m_sect;
		CString m_addClan,m_deleteClan,m_doesClanExist,m_doesClanExistByHandle,m_doescharExist,m_removeFromClan;


		int DeleteClanMember(CString &clanID,CString & charName,CDBConnectionSystem * connection)
		{
            CString temp = m_doescharExist;
			temp.Replace("$CHARNAME",charName);
			_RecordsetPtr rcdPtr = connection->Query(temp);
			if(connection->GetCount(rcdPtr) > 0)
			{//found player
				if(connection->GetStringData("CLANID",rcdPtr).CompareNoCase(clanID)!=0)
				{
					connection->Close(rcdPtr);
					return 2;//not on clan to begin with :O
				}
				connection->Close(rcdPtr);
				//execute clan removal by setting the CLANID to -1
				temp = m_removeFromClan;
				temp.Replace("$CHARNAME",charName);
				temp.Replace("$CLANID",clanID);
				connection->ExecQuery(temp);
			    return 1;//success
			}//end found player
			connection->Close(rcdPtr);
			return 0;//cant even find the player
		}


		BOOL IsThisHandleAlreadyRequesting(int & requestHandle)
		{
			EnterCriticalSection(&m_sect);
			for(CAxLinkObj * pos = GetHeadPosition_unsafe();pos != NULL;)
			{//ordered from greatest to least
				CAxClanCommand * pData = (CAxClanCommand *)GetNext_unsafe(&pos);
				if(pData->m_fromhandle == requestHandle)
				{
					LeaveCriticalSection(&m_sect);
					return TRUE;
				}
			}
			LeaveCriticalSection(&m_sect);
			return FALSE;
		}
		 

		BOOL AddCommand_safe(int command, int requestHandle, CString param1, CString param2)
		{
			if(IsThisHandleAlreadyRequesting(requestHandle))
			   return FALSE;

			//add it to the que
			EnterCriticalSection(&m_sect);
			CAxClanCommand * cmd = new CAxClanCommand(command,requestHandle,param1,param2);
			CAxLinkObj * newPtr		= new CAxLinkObj();
			newPtr->m_uniqueID		= requestHandle;
			newPtr->m_thisObject	= cmd;
			AddTail(newPtr);
			LeaveCriticalSection(&m_sect);

			return TRUE;
		}

		CAxClanCommand * GetNextCommand()
		{
			EnterCriticalSection(&m_sect);
			CAxClanCommand * cmd = NULL;
			CAxLinkObj * pos = this->GetHeadPosition_unsafe();
			if(pos)
			{  
			   cmd = (CAxClanCommand *)pos->m_thisObject;
			   this->RemoveAt(cmd->m_fromhandle);
			   LeaveCriticalSection(&m_sect);
			   return cmd;//the reciever should delete memory of command
			}
			LeaveCriticalSection(&m_sect);
			return NULL;
		}

		BOOL DoesClanExist_safe(CString clanHandle,CDBConnectionSystem * connection)
		{
			CString temp = m_doesClanExistByHandle;
			temp.Replace("$HANDLE",clanHandle);
			_RecordsetPtr rcdPtr = connection->Query(temp);
			if(connection->GetCount(rcdPtr) > 0)
			{
				connection->Close(rcdPtr);
				return TRUE;//yes
			}
			connection->Close(rcdPtr);
			return FALSE;//no
		}

		BOOL DoesClanExist_safe(CString clanHandle,CString * clanNameReturn,CDBConnectionSystem * connection)
		{
			CString temp = m_doesClanExistByHandle;
			temp.Replace("$HANDLE",clanHandle);
			_RecordsetPtr rcdPtr = connection->Query(temp);
			if(connection->GetCount(rcdPtr) > 0)
			{
				*clanNameReturn = connection->GetStringData("ABBREV",rcdPtr);
				connection->Close(rcdPtr);
				return TRUE;//yes
			}
			connection->Close(rcdPtr);
			return FALSE;//no
		}

		BOOL AddClanIfNone_safe(CString abbrev,CString uniqueHandle,CDBConnectionSystem * connection)
		{
            CString temp = m_doesClanExist;
			temp.Replace("$ABBREV",abbrev);
			_RecordsetPtr rcdPtr = connection->Query(temp);
			if(connection->GetCount(rcdPtr) > 0)
			{
				connection->Close(rcdPtr);
				return FALSE;
			}
			connection->Close(rcdPtr);
		
			temp = m_addClan;
			temp.Replace("$ABBREV",abbrev);
			temp.Replace("$HANDLE",uniqueHandle);

			if(connection->ExecQuery(temp))
				return TRUE;//success		

			return FALSE;//failed
		}

		BOOL DeleteClan_safe(CString abbrev,CDBConnectionSystem * connection)
		{
            CString temp = m_deleteClan;
			temp.Replace("$ABBREV",abbrev);
			connection->ExecQuery(temp);
			return TRUE;
		}

};


#include "stdafx.h"
#include "cnetworkcommands.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Class:		CNetworkCommands
// Description:	Static Member Initialization
///////////////////////////////////////////////////////////////////////////////

/*
bool CNetworkCommands::m_bInitialized = false;
COMMAND_MAP* CNetworkCommands::ClientCommands = new COMMAND_MAP();
COMMAND_MAP* CNetworkCommands::ServerCommands = new COMMAND_MAP();
COMMAND_MAP_INVERSE* CNetworkCommands::ServerCommandsInverse = new COMMAND_MAP_INVERSE();
*/

///////////////////////////////////////////////////////////////////////////////
// Class:		CNetworkCommands
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CNetworkCommands::CNetworkCommands()
{
	// Initialize command mappings
	//if(!CNetworkCommands::m_bInitialized) 
	//	CNetworkCommands::Init();

	/*COMMAND_MAP*/ ClientCommands = new COMMAND_MAP();
	/*COMMAND_MAP*/ ServerCommands = new COMMAND_MAP();
	/*COMMAND_MAP_INVERSE*/ ServerCommandsInverse = new COMMAND_MAP_INVERSE();

	this->Init();
}

CNetworkCommands::~CNetworkCommands()
{
	/*
	// Clear command containers
	if(CNetworkCommands::m_bInitialized)
		CNetworkCommands::Cleanup();

	// Release dynamically allocated memory
	SafeDelete(CNetworkCommands::ClientCommands);
	SafeDelete(CNetworkCommands::ServerCommands);
	SafeDelete(CNetworkCommands::ServerCommandsInverse);
	*/
	// Clear command containers
	this->Cleanup();

	// Release dynamically allocated memory
	SafeDelete(ClientCommands);
	SafeDelete(ServerCommands);
	SafeDelete(ServerCommandsInverse);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CNetworkCommands
// Function:	Init()
// Description:	Initialize command -> command ID mappings.
///////////////////////////////////////////////////////////////////////////////

void CNetworkCommands::Init()
{
	// Clear containers
	//ClientCommands->clear();
	//ServerCommands->clear();
	//ServerCommandsInverse->clear();

	// Initialize client-side command mappings
//	ClientCommands->insert(COMMAND_PAIR("INVITE", CCMD_INVITE));
//	ClientCommands->insert(COMMAND_PAIR("REMOVE", CCMD_REMOVE));
//	ClientCommands->insert(COMMAND_PAIR("GIVEAUTHORITY", CCMD_GIVEAUTHORITY));
//	ClientCommands->insert(COMMAND_PAIR("MAKELEADER", CCMD_MAKELEADER));
//	ClientCommands->insert(COMMAND_PAIR("CREATESQUAD", CCMD_CREATESQUAD));
//	ClientCommands->insert(COMMAND_PAIR("LEAVEGROUP", CCMD_LEAVEGROUP));
	ClientCommands->insert(COMMAND_PAIR("GROUPMENU", CCMD_SQUADMENU));
	ClientCommands->insert(COMMAND_PAIR("SQUELCH", CCMD_SQUELCH));
	ClientCommands->insert(COMMAND_PAIR("DESQUELCH", CCMD_DESQUELCH));
	ClientCommands->insert(COMMAND_PAIR("SQUAD", CCMD_SQUAD));
	ClientCommands->insert(COMMAND_PAIR("GROUP", CCMD_GROUP));
	ClientCommands->insert(COMMAND_PAIR("DANCE", CCMD_DANCE));
	ClientCommands->insert(COMMAND_PAIR("SIT", CCMD_SIT));
	ClientCommands->insert(COMMAND_PAIR("ALL", CCMD_ALL));

	// Initialize server-side command mappings
	ServerCommands->insert(COMMAND_PAIR("WHISPER", SCMD_WHISPER));
	ServerCommands->insert(COMMAND_PAIR("TALK", SCMD_TALK));
	ServerCommands->insert(COMMAND_PAIR("T", SCMD_T));
	ServerCommands->insert(COMMAND_PAIR("BUY", SCMD_BUY));
	ServerCommands->insert(COMMAND_PAIR("MANAGE", SCMD_MANAGE));
	ServerCommands->insert(COMMAND_PAIR("GAG", SCMD_GAG));
	ServerCommands->insert(COMMAND_PAIR("VUP", SCMD_VUP));
	ServerCommands->insert(COMMAND_PAIR("VDOWN", SCMD_VDOWN));
	ServerCommands->insert(COMMAND_PAIR("ARENASTATS", SCMD_ARENASTATS));
	ServerCommands->insert(COMMAND_PAIR("ROLL", SCMD_ROLL));
	ServerCommands->insert(COMMAND_PAIR("REMOVEHOUSE", SCMD_REMOVEFROMCLANHOUSE));
	ServerCommands->insert(COMMAND_PAIR("WHERE", SCMD_WHERE));
	ServerCommands->insert(COMMAND_PAIR("LOCKHOUSE", SCMD_LOCKHOUSE));
	ServerCommands->insert(COMMAND_PAIR("UNLOCKHOUSE", SCMD_UNLOCKHOUSE));
	ServerCommands->insert(COMMAND_PAIR("GROUPSHARINGON", SCMD_GROUPSHARINGON));
	ServerCommands->insert(COMMAND_PAIR("GROUPSHARINGOFF", SCMD_GROUPSHARINGOFF));
	ServerCommands->insert(COMMAND_PAIR("LOOT", SCMD_LOOT));
	ServerCommands->insert(COMMAND_PAIR("REBIRTH", SCMD_REBIRTH));
	ServerCommands->insert(COMMAND_PAIR("TRADE", SCMD_TRADE));
	ServerCommands->insert(COMMAND_PAIR("BANK", SCMD_BANK));
	ServerCommands->insert(COMMAND_PAIR("PAGEGM", SCMD_PAGEGM));
	ServerCommands->insert(COMMAND_PAIR("ZEROMURDERS", SCMD_ZEROMURDERS));
	ServerCommands->insert(COMMAND_PAIR("ANSWERNEXTPAGE", SCMD_ANSWERNEXTPAGE));
	ServerCommands->insert(COMMAND_PAIR("WITHDRAWAL", SCMD_WITHDRAWAL));
	ServerCommands->insert(COMMAND_PAIR("DEPOSIT", SCMD_DEPOSIT));
	ServerCommands->insert(COMMAND_PAIR("CREATEGROUP", SCMD_CREATEGROUP));
	ServerCommands->insert(COMMAND_PAIR("DISBANDGROUP", SCMD_DISBANDGROUP));
	ServerCommands->insert(COMMAND_PAIR("REQUESTJOINGROUP", SCMD_REQUESTJOINGROUP));
	ServerCommands->insert(COMMAND_PAIR("APPROVECLANRECRUIT", SCMD_APPROVECLANRECRUIT));
	ServerCommands->insert(COMMAND_PAIR("DESTROYMYSKILL", SCMD_DESTROYMYSKILL));
	ServerCommands->insert(COMMAND_PAIR("DESTROYJNLENTRY", SCMD_DESTROYJNLENTRY));
	ServerCommands->insert(COMMAND_PAIR("DECLAREWAR", SCMD_DECLAREWAR));
	ServerCommands->insert(COMMAND_PAIR("ACCEPTWAR", SCMD_ACCEPTWAR));
	ServerCommands->insert(COMMAND_PAIR("SURRENDER", SCMD_SURRENDER));
	ServerCommands->insert(COMMAND_PAIR("SPAWNGEN", SCMD_SPAWNGEN));
	ServerCommands->insert(COMMAND_PAIR("BROADCAST", SCMD_BROADCAST));
	ServerCommands->insert(COMMAND_PAIR("INVITETOGROUP", SCMD_INVITETOGROUP));
	ServerCommands->insert(COMMAND_PAIR("LEAVEGROUP", SCMD_LEAVEGROUP));
	ServerCommands->insert(COMMAND_PAIR("CREATECLAN", SCMD_CREATECLAN));
	ServerCommands->insert(COMMAND_PAIR("REQUESTAREACLANLEADERS", SCMD_REQUESTAREACLANLEADERS));
	ServerCommands->insert(COMMAND_PAIR("REQUESTCHARSKILLS", SCMD_REQUESTCHARSKILLS));
	ServerCommands->insert(COMMAND_PAIR("REQUESTAREATRADE", SCMD_REQUESTAREATRADE));
	ServerCommands->insert(COMMAND_PAIR("REQUESTCLANMEMBERS", SCMD_REQUESTCLANMEMBERS));
	ServerCommands->insert(COMMAND_PAIR("REQUESTAREAPC", SCMD_REQUESTAREAPC));
	ServerCommands->insert(COMMAND_PAIR("DISBANCLAN", SCMD_DISBANCLAN));
	ServerCommands->insert(COMMAND_PAIR("REMOVECLANMEMBER", SCMD_REMOVECLANMEMBER));
	ServerCommands->insert(COMMAND_PAIR("REQUESTJOINCLAN", SCMD_REQUESTJOINCLAN));
	ServerCommands->insert(COMMAND_PAIR("VIEWROSTER", SCMD_VIEWROSTER));
	ServerCommands->insert(COMMAND_PAIR("VIEWWARLIST", SCMD_VIEWWARLIST));
	ServerCommands->insert(COMMAND_PAIR("COMBINESTACK", SCMD_COMBINESTACKS));
	ServerCommands->insert(COMMAND_PAIR("SPLITSTACK", SCMD_SPLITSTACK));
	ServerCommands->insert(COMMAND_PAIR("DROPSTACK", SCMD_DROPSTACK));
	ServerCommands->insert(COMMAND_PAIR("PICKUPSTACK", SCMD_PICKUPSTACK));
	ServerCommands->insert(COMMAND_PAIR("DELETESTACK", SCMD_DELETESTACK));
	ServerCommands->insert(COMMAND_PAIR("MOVESTACKTOPACK", SCMD_MOVESTACKTOPACK));
	ServerCommands->insert(COMMAND_PAIR("ARMSTACK", SCMD_ARMSTACK));
	ServerCommands->insert(COMMAND_PAIR("UNARMSTACK", SCMD_UNARMSTACK));
	ServerCommands->insert(COMMAND_PAIR("MURDERCOUNT", SCMD_MURDERCOUNT));
	ServerCommands->insert(COMMAND_PAIR("MURDERWEAR", SCMD_MURDERWEAR));
	ServerCommands->insert(COMMAND_PAIR("FORCEGROUPID", SCMD_FORCEGROUPID));
	ServerCommands->insert(COMMAND_PAIR("INFO", SCMD_INFO));
	ServerCommands->insert(COMMAND_PAIR("ADMINRF", SCMD_ADMINRF));
    ServerCommands->insert(COMMAND_PAIR("ADMINEXPLODE", SCMD_ADMINEXPLODE));
	ServerCommands->insert(COMMAND_PAIR("ADMINSLAP", SCMD_ADMINSLAP));
	ServerCommands->insert(COMMAND_PAIR("TRADEOFFER", SCMD_TRADEOFFER));
	ServerCommands->insert(COMMAND_PAIR("TRADEACCEPT", SCMD_TRADEACCEPT));
	ServerCommands->insert(COMMAND_PAIR("TRADECANCEL", SCMD_TRADECANCEL));
	ServerCommands->insert(COMMAND_PAIR("TRADECONFIRM", SCMD_TRADECONFIRM));
	ServerCommands->insert(COMMAND_PAIR("INTERACTION", SCMD_INTERACTION));
	ServerCommands->insert(COMMAND_PAIR("REQUESTARENATYPES", SCMD_REQUESTARENATYPES));
	ServerCommands->insert(COMMAND_PAIR("REQUESTARENAINFO", SCMD_REQUESTARENAINFO));
	ServerCommands->insert(COMMAND_PAIR("GMTOPLAYER", SCMD_GMTOPLAYER));
	ServerCommands->insert(COMMAND_PAIR("PLAYERTOGM", SCMD_PLAYERTOGM));
	ServerCommands->insert(COMMAND_PAIR("TRADERESET", SCMD_TRADERESET));
	ServerCommands->insert(COMMAND_PAIR("INVITETOSQUAD", SCMD_INVITETOSQUAD));
	ServerCommands->insert(COMMAND_PAIR("REMOVEFROMSQUAD", SCMD_REMOVEFROMSQUAD));
	ServerCommands->insert(COMMAND_PAIR("GIVEAUTHORITYOFSQUAD", SCMD_GIVEAUTHORITYOFSQUAD));
	ServerCommands->insert(COMMAND_PAIR("MAKELEADEROFSQUAD", SCMD_MAKELEADEROFSQUAD));
	ServerCommands->insert(COMMAND_PAIR("CREATESQUAD", SCMD_CREATESQUAD));
	ServerCommands->insert(COMMAND_PAIR("LEAVESQUAD", SCMD_LEAVESQUAD));
	ServerCommands->insert(COMMAND_PAIR("REQUESTSQUADINFO", SCMD_REQUESTSQUADINFO));
	ServerCommands->insert(COMMAND_PAIR("C", SCMD_C));
	ServerCommands->insert(COMMAND_PAIR("REQUESTINVENTORY", SCMD_REQUESTINVENTORY));
	ServerCommands->insert(COMMAND_PAIR("REQUESTCLANINIT", SCMD_REQUESTCLANINIT));
	ServerCommands->insert(COMMAND_PAIR("&SUICIDE&", SCMD_SUICIDE));
	ServerCommands->insert(COMMAND_PAIR("UC_LISTRECENT",	SCMD_UC_LISTRECENT));
	ServerCommands->insert(COMMAND_PAIR("UC_LISTBYITEM",	SCMD_UC_LISTBYITEM));
	ServerCommands->insert(COMMAND_PAIR("UC_SELLITEM",		SCMD_UC_SELLITEM));
	ServerCommands->insert(COMMAND_PAIR("UC_BUYITEM",		SCMD_UC_BUYITEM));
	ServerCommands->insert(COMMAND_PAIR("UC_CASHOUT",		SCMD_UC_CASHOUT));
	ServerCommands->insert(COMMAND_PAIR("UC_SALESSTATS",	SCMD_UC_SALESSTATS));

	// Initialize inverse server commands map
	COMMAND_MAP::const_iterator iter;
	iter = ServerCommands->begin();
	while(iter != ServerCommands->end())
	{
		ServerCommandsInverse->insert(COMMAND_PAIR_INVERSE(iter->second, iter->first));
		iter++;
	}

	// Set the initialized flag
	//m_bInitialized = true;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CNetworkCommands
// Function:	Cleanup()
// Description:	Clear command mapping containers.
///////////////////////////////////////////////////////////////////////////////

void CNetworkCommands::Cleanup()
{
	// Clear containers
	ServerCommands->clear();
	ClientCommands->clear();
}

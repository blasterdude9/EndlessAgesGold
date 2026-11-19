#include "stdafx.h"
#include "CMenuScript.h"
#include "direct.h"
#include "GlobalStatusVariables.h"
#include "renderingmanagement.h"
#include <vector>
#include <map>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern GlobalStatusVariables * globalStatus;
extern CRenderingManagement * rendermanager;

/*CMenuScript& 
CMenuScript::operator=(const CMenuScript &rhs)
{
	 this->m_reference = TRUE;    
	 this->m_scriptText = rhs.m_scriptText;
	 this->m_waitOnInputFlag = rhs.m_waitOnInputFlag;
	 this->m_pFirstLine = rhs.m_pFirstLine;

	 // Allow for chained assignments
     return *this;
}*/

void CMenuScript::Clone(CMenuScript **pClone)
{
	SafeDelete(*pClone);
	(*pClone) = new CMenuScript();
	(*pClone)->m_reference = TRUE;

	(*pClone)->m_scriptText = m_scriptText;
	(*pClone)->m_waitOnInputFlag = m_waitOnInputFlag;
	(*pClone)->m_pFirstLine = m_pFirstLine;
}

void CMenuScript::AddLine(char *newLine)
{
	int stringLength = strlen(newLine);
	char **params = NULL;
	CScriptLine *curLine = m_pFirstLine;

	//Add the line to the script string
	m_scriptText += newLine;

	//Get the current script line(create one of we need to)
	if(!m_pFirstLine)
		curLine = m_pFirstLine = new CScriptLine();
	else
	{
		while(curLine->m_pNextLine)
			curLine = curLine->m_pNextLine;

		curLine->m_pNextLine				= new CScriptLine();
		curLine->m_pNextLine->m_pPrevLine	= curLine;
		curLine								= curLine->m_pNextLine;
	}

	//Find the beginning of the parameters
	int paramStart = 1;
	while(newLine[paramStart] != '(' && paramStart < stringLength - 1)
		paramStart++;

	//NULL terminate the function
	newLine[paramStart++] = '\0';
	char *paramString = newLine + paramStart;
	int curChar = 0;

	// NOTE: the else-if chain below is at MSVC's nested-block limit (C1061,
	// 128 levels).  Do NOT add more "else if" branches to it - add new script
	// commands here as separate early-return blocks instead.
	if(strcmp(newLine, "Login") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_LOGIN;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("Login-MENU_SCRIPTFUNC_LOGIN Parameter Mismatch (Login(userTextBoxID,passwordTextBoxID))");
		return;
	}
	if(strcmp(newLine, "SetServerAddress") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETSERVERADDRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("SetServerAddress-MENU_SCRIPTFUNC_SETSERVERADDRESS Parameter Mismatch");
		return;
	}
	if(strcmp(newLine, "UpdateElementProgressBar") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEELEMENTPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("UpdateElementProgressBar-MENU_SCRIPTFUNC_UPDATEELEMENTPROGRESS Parameter Mismatch (UpdateElementProgressBar(barID,element))");
		return;
	}

	//Figure out what function we're on
	if(strcmp(newLine, "QuitGame") == 0)
	{
		//Store the function type
		curLine->m_function	 = MENU_SCRIPTFUNC_QUIT;
	}	
	else if(strcmp(newLine, "InitQuantity") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INITQUANTITY;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("InitQuantity-MENU_SCRIPTFUNC_INITQUANTITY Parameter Mismatch");
	}
	else if(strcmp(newLine, "BuyItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_BUY;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("BuyItem-MENU_SCRIPTFUNC_BUY Parameter Mismatch");
	}
	else if(strcmp(newLine, "SellItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SELL;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("SellItem-MENU_SCRIPTFUNC_SELL Parameter Mismatch");
	}
	else if(strcmp(newLine, "LootItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_LOOT;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("LootItem-MENU_SCRIPTFUNC_LOOT Parameter Mismatch");
	}
	else if(strcmp(newLine, "LootAllItems") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_LOOTALL;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("LootAllItems-MENU_SCRIPTFUNC_LOOTALL Parameter Mismatch");
	}
	else if(strcmp(newLine, "CloseMenu") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_CLOSE;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("CloseMenu-MENU_SCRIPTFUNC_CLOSE Parameter Mismatch");
	}
	else if(strcmp(newLine, "CloseAllMenus") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_CLOSEALL;
	}
	else if(strcmp(newLine, "QuantityPopup") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_QUANTITY;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("QuantityPopup-MENU_SCRIPTFUNC_QUANTITY Parameter Mismatch");
	}
	else if(strcmp(newLine, "PlayCharacter") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_PLAYCHARACTER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("PlayCharacter-MENU_SCRIPTFUNC_PLAYCHARACTER Parameter Mismatch");
	}
	else if(strcmp(newLine, "DeleteCharacter") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DELETECHARACTER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("DeleteCharacter-MENU_SCRIPTFUNC_DELETECHARACTER Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestCharacters") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTCHARACTERS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("RequestCharacters-MENU_SCRIPTFUNC_REQUESTCHARACTERS Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestInventory") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTINVENTORY;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("RequestInventory-MENU_SCRIPTFUNC_REQUESTINVENTORY Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetSystemOutput") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETSYSTEMOUTPUT;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("SetSystemOutput-MENU_SCRIPTFUNC_SETSYSTEMOUTPUT Parameter Mismatch",paramString);
	}
	else if(strcmp(newLine, "SetChatInput") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETCHATINPUT;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("SetChatInput-MENU_SCRIPTFUNC_SETCHATINPUT Parameter Mismatch");
	}
	else if(strcmp(newLine, "SendChatText") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_SENDCHATTEXT;
	}
	else if(strcmp(newLine, "RequestStoreInventory") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTSTOREINVENTORY;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("RequestStoreInventory-MENU_SCRIPTFUNC_REQUESTSTOREINVENTORY Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestLootInventory") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTLOOTINVENTORY;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("RequestLootInventory-MENU_SCRIPTFUNC_REQUESTLOOTINVENTORY Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestInteraction") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTINTERACTION;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("RequestInteraction-MENU_SCRIPTFUNC_REQUESTINTERACTION Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplayCredits") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYCREDITS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("DisplayCredits-MENU_SCRIPTFUNC_DISPLAYCREDITS Parameter Mismatch");
		
	}
	else if(strcmp(newLine, "DisplayHealth") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYHEALTH;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("DisplayHealth-MENU_SCRIPTFUNC_DISPLAYHEALTH Parameter Mismatch");
	}
	else if(strcmp(newLine, "ActivateItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_ACTIVATEITEM;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("ActivateItem-MENU_SCRIPTFUNC_ACTIVATEITEM Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisarmItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISARM;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("DisarmItem-MENU_SCRIPTFUNC_DISARM Parameter Mismatch");
	}
	else if(strcmp(newLine, "DeleteItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DELETEITEM;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("DeleteItem-MENU_SCRIPTFUNC_DELETEITEM Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestSkills") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTSKILLSINVENTORY;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("RequestSkills-MENU_SCRIPTFUNC_REQUESTSKILLSINVENTORY Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestSpells") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTSPELLSINVENTORY;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("RequestSpells-MENU_SCRIPTFUNC_REQUESTSPELLSINVENTORY Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplayEnergy") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYENERGY;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("DisplayEnergy-MENU_SCRIPTFUNC_DISPLAYENERGY Parameter Mismatch");
	}
	else if(strcmp(newLine, "SendServerCommand") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SENDSERVERCOMMAND;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("SendServerCommand-MENU_SCRIPTFUNC_SENDSERVERCOMMAND Parameter Mismatch");
	}
	else if(strcmp(newLine, "Teleport") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_TELEPORT;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("Teleport-MENU_SCRIPTFUNC_TELEPORT Parameter Mismatch");
	}
	else if(strcmp(newLine, "OpenMenu") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_OPENMENU;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("OpenMenu-MENU_SCRIPTFUNC_OPENMENU Parameter Mismatch");
	}
	else if(strcmp(newLine, "ToggleMenu") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_TOGGLEMENU;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("ToggleMenu-MENU_SCRIPTFUNC_TOGGLEMENU Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateCompass") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATECOMPASS;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("UpdateCompass-MENU_SCRIPTFUNC_UPDATECOMPASS Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateHealthProgressBar") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEHEALTHPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("UpdateHealthProgressBar-MENU_SCRIPTFUNC_UPDATEHEALTHPROGRESS Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateJetProgress") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEJETPACK;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("UpdateJetProgress-MENU_SCRIPTFUNC_UPDATEJETPACK Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateEnergyProgress") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEENERGYPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("UpdateEnergyProgress-MENU_SCRIPTFUNC_UPDATEENERGYPROGRESS Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplayCrystals") == 0)
	{
	    curLine->m_function = MENU_SCRIPTFUNC_DISPLAYCRYSTALS;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("DisplayCrystals-MENU_SCRIPTFUNC_DISPLAYCRYSTALS Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdatePowerupStatProgressBar") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEPOWERUPSTATPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("UpdatePowerupStatProgressBar-MENU_SCRIPTFUNC_UPDATEPOWERUPSTATPROGRESS Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdatePowerupResistProgressBar") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEPOWERUPRESISTPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("UpdatePowerupResistProgressBar-MENU_SCRIPTFUNC_UPDATEPOWERUPRESISTPROGRESS Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdatePowerupDamageProgressBar") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEPOWERUPDAMAGEPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("UpdatePowerupDamageProgressBar-MENU_SCRIPTFUNC_UPDATEPOWERUPDAMAGEPROGRESS Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestCash") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTCASH;
	}
	else if(strcmp(newLine, "UpdateTextInt") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATETEXTINT;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("UpdateTextInt-MENU_SCRIPTFUNC_UPDATETEXTINT Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateTextString") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATETEXTSTRING;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("UpdateTextString-MENU_SCRIPTFUNC_UPDATETEXTSTRING Parameter Mismatch");
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_UPDATETEXTSTRING;
	}
	else if(strcmp(newLine, "AcceptSelectedQuest") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_ACCEPTSELECTEDQUEST;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("AcceptSelectedQuest-MENU_SCRIPTFUNC_ACCEPTSELECTEDQUEST Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetToggleState") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETTOGGLESTATE;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("SetToggleState-MENU_SCRIPTFUNC_SETTOGGLESTATE Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetVisualVisibleState") == 0)
	{
        curLine->m_function = MENU_SCRIPTFUNC_SETVISUALVISIBLESTATE;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("SetVisualVisibleState-MENU_SCRIPTFUNC_SETVISUALVISIBLESTATE Parameter Mismatch");
	}
	else if(strcmp(newLine, "AcceptSquadInvite") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_ACCEPTSQUADINVITE;
	}
	else if(strcmp(newLine, "DeclineSquadInvite") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_DECLINESQUADINVITE;
	}
	else if(strcmp(newLine, "DepositCredits") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_DEPOSITCREDITS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("SetVisualVisibleState-MENU_SCRIPTFUNC_DEPOSITCREDITS Parameter Mismatch");
	}
	else if(strcmp(newLine, "WithdrawlCredits") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_WITHDRAWLCREDITS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("SetVisualVisibleState-MENU_SCRIPTFUNC_WITHDRAWLCREDITS Parameter Mismatch");
	}
	else if(strcmp(newLine, "InitRadar") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INITRADAR;
		if(!GetParametersSafe(paramString,stringLength,curChar,12,curLine))
			globalStatus->UI_Log("InitRadar-MENU_SCRIPTFUNC_INITRADAR Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateRadar") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_UPDATERADAR;
	}
	else if(strcmp(newLine, "ZoomRadarIn") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_ZOOMRADARIN;
	}
	else if(strcmp(newLine, "ZoomRadarOut") == 0)
	{
		//Store the function type
		curLine->m_function = MENU_SCRIPTFUNC_ZOOMRADAROUT;
	}
	else if(strcmp(newLine, "RequestSkillTitles") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTSKILLTITLES;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("RequestSkillTitles-MENU_SCRIPTFUNC_REQUESTSKILLTITLES Parameter Mismatch");
	}
	else if(strcmp(newLine, "SendSelectedSkillTitle") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SENDSELECTEDSKILLTITLE;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("SendSelectedSkillTitle-MENU_SCRIPTFUNC_SENDSELECTEDSKILLTITLE Parameter Mismatch");
	}
	/*else if(strcmp(newLine, "RequestControls") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTCONTROLSETTINGS;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("RequestControls-MENU_SCRIPTFUNC_REQUESTCONTROLSETTINGS Parameter Mismatch");
	}*/
	else if(strcmp(newLine, "OnSelectedSetKeyMapListener") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_ONSELECTEDSETKEYMAPLISTENER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("SetKeyMap-MENU_SCRIPTFUNC_ONSELECTEDSETKEYMAPLISTENER Parameter Mismatch");
	}
	else if(strcmp(newLine, "RenderMainCharacter") == 0)
	{
        //params
		//0=image id
		//1=camera dist
		//2=camera yaw
		//3=camera pitch
		//4=rezX
		//5=resY
		//6=Red background
		//7=Green background
		//8=Blue background
		curLine->m_function = MENU_SCRIPTFUNC_RENDERMAINCHARACTER;
		if(!GetParametersSafe(paramString,stringLength,curChar,9,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_RENDERMAINCHARACTER Parameter Mismatch");
	}
	else if(strcmp(newLine, "RenderListGlidToImage") == 0)
	{
        //params
		//0=image id
		//1=glid list
		//2=camera dist
		//3=camera pitch
		//4=rezX
		//5=resY
		//6=Red background
		//7=Green background
		//8=Blue background
		curLine->m_function = MENU_SCRIPTFUNC_RenderListGlidToImage;
		if(!GetParametersSafe(paramString,stringLength,curChar,9,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_RenderListGlidToImage Parameter Mismatch");
	}
	else if(strcmp(newLine, "IfOfflineFillTestGlids") == 0)
	{
        //params
		//0=list
		curLine->m_function = MENU_SCRIPTFUNC_IfOfflineFillTestGlids;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_IfOfflineFillTestGlids Parameter Mismatch");
	}
	else if(strcmp(newLine, "IndexDimUp") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INDEXDIMUP;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INDEXDIMUP Parameter Mismatch");
	}
	else if(strcmp(newLine, "IndexDimDown") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INDEXDIMDOWN;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INDEXDIMDOWN Parameter Mismatch");
	}
	else if(strcmp(newLine, "CreateCharacter") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_CREATECHARACTER;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_CREATECHARACTER Parameter Mismatch");
	}
	else if(strcmp(newLine, "InitEntityFromDBID") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INITENTITYFROMDBID;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INITENTITYFROMDBID Parameter Mismatch");
	}
	else if(strcmp(newLine, "RenderEntityObject") == 0)
	{
        //params
		//0=image id
		//1=glid list
		//2=camera dist
		//3=camera pitch
		//4=rezX
		//5=resY
		//6=Red background
		//7=Green background
		//8=Blue background
		//9=entity id
		curLine->m_function = MENU_SCRIPTFUNC_RENDERENTITYOBJECT;
		if(!GetParametersSafe(paramString,stringLength,curChar,10,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_RENDERENTITYOBJECT Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateEntityAnimation") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEENTITYANIMATION;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEENTITYANIMATION Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetEntityAnimation") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETANIMATION;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETANIMATION Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplayStat") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYSTAT;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DISPLAYSTAT Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplayResist") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYRESIST;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DISPLAYRESIST Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplayWound") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYWOUND;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DISPLAYWOUND Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplayCharName") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYCHARNAME;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DISPLAYCHARNAME Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateResistProgress") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATERESISTPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATERESISTPROGRESS Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateAmmoProgress") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEAMMOPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEAMMOPROGRESS Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateAmmoRecharge") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEAMMORECHARGE;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEAMMORECHARGE Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateHPLight") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEHPLIGHT;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEHPLIGHT Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateResistLight") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATERESISTLIGHT;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATERESISTLIGHT Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateBuffIcon") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEBUFFMARKER;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEBUFFMARKER Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateStoreEntity") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_EDITSTOREENTITY;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_EDITSTOREENTITY Parameter Mismatch");
	}
	else if(strcmp(newLine, "SelectQuest") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SELECTQUEST;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SELECTQUEST Parameter Mismatch");
	}
	else if(strcmp(newLine, "ClearList") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_CLEARLIST;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_CLEARLIST Parameter Mismatch");
	}
	else if(strcmp(newLine, "InvertMouse") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INVERTMOUSE;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INVERTMOUSE Parameter Mismatch");
	}
	else if(strcmp(newLine, "IncreaseMouseSensitivity") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INCREASEMOUSESENSE;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INCREASEMOUSESENSE Parameter Mismatch");
	}
	else if(strcmp(newLine, "DecreaseMouseSensitivity") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DECREASEMOUSESENSE;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DECREASEMOUSESENSE Parameter Mismatch");
	}
	else if(strcmp(newLine, "IncreaseMusicVolume") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INCREASEMUSICVOLUME;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INCREASEMUSICVOLUME Parameter Mismatch");
	}
	else if(strcmp(newLine, "DecreaseMusicVolume") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DECREASEMUSICVOLUME;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DECREASEMUSICVOLUME Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetKeyControl") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETKEYCONTROL;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETKEYCONTROL Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetDefaultKeyConfigs") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETDEFAULTKEYCONFIGS;
		if(!GetParametersSafe(paramString,stringLength,curChar,4,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETDEFAULTKEYCONFIGS Parameter Mismatch");
	}
	else if(strcmp(newLine, "WriteKeyConfigToFile") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_WRITEKEYCONFIGTOFILE;
		if(!GetParametersSafe(paramString,stringLength,curChar,4,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_WRITEKEYCONFIGTOFILE Parameter Mismatch");
	}
	else if(strcmp(newLine, "ReadKeyConfigFromFile") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_READKEYCONFIGFROMFILE;
		if(!GetParametersSafe(paramString,stringLength,curChar,5,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_READKEYCONFIGFROMFILE Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowQuickbarHover") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SHOWQUICKBARHOVER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SHOWQUICKBARHOVER Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowEmblemHover") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SHOWEMBLEMHOVER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SHOWEMBLEMHOVER Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowHPHover") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_HPHOVER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_HPHOVER Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowBasicHover") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_BASICHOVER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_BASICHOVER Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowXPHover") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_XPHOVER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_XPHOVER Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowFactionHover") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_FACTIONHOVER;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_FACTIONHOVER Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowInventoryHover") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SHOWINVENTORYHOVER;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SHOWINVENTORYHOVER Parameter Mismatch");
	}
	else if(strcmp(newLine, "SelectDragItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SELECTDRAGITEM;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SELECTDRAGITEM Parameter Mismatch");
	}
	else if(strcmp(newLine, "DragQuickItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DRAGQUICKITEM;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DRAGQUICKITEM Parameter Mismatch");
	}
	else if(strcmp(newLine, "LoadQuickbarConfig") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_LOADQUICKSLOTSCONFIG;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_LOADQUICKSLOTSCONFIG Parameter Mismatch");
	}
	else if(strcmp(newLine, "AddItemToQuickSlot") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_ADDITEMTOQUICKSLOT;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_ADDITEMTOQUICKSLOT Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetQuickItemVisual") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETQUICKITEMVISUAL;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETQUICKITEMVISUAL Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateQuickslotHighlight") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEQUICKSLOTHIGHLIGHT;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEQUICKSLOTHIGHLIGHT Parameter Mismatch");
	}
	else if(strcmp(newLine, "PlaySong") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_PLAYSONG;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_PLAYSONG Parameter Mismatch");
	}
	else if(strcmp(newLine, "InitArenaBanners") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INITARENABANNERS;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INITARENABANNERS Parameter Mismatch");
	}
	else if(strcmp(newLine, "InitArenaBadges") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INITARENABADGES;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INITARENABADGES Parameter Mismatch");
	}
	else if(strcmp(newLine, "FadeImage") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_FADEIMAGE;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_FADEIMAGE Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetNotificationString") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETNOTIFICATIONSTRING;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETNOTIFICATIONSTRING Parameter Mismatch");
	}
	else if(strcmp(newLine, "CheckNPCCollision") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_CHECKNPCCOLLISION;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_CHECKNPCCOLLISION Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateArenaTimer") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEARENATIMER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEARENATIMER Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetRGB") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETRGB;
		if(!GetParametersSafe(paramString,stringLength,curChar,4,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETRGB Parameter Mismatch");
	}
	else if(strcmp(newLine, "TradeCancel") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_TRADECANCEL;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_TRADECANCEL Parameter Mismatch");
	}
	else if(strcmp(newLine, "TradeConfirm") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_TRADECONFIRM;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_TRADECONFIRM Parameter Mismatch");
	}
	else if(strcmp(newLine, "TradeOffer") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_TRADEOFFER;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_TRADEOFFER Parameter Mismatch");
	}
	else if(strcmp(newLine, "DropDragItemToList") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DROPDRAGITEMTOLIST;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DROPDRAGITEMTOLIST Parameter Mismatch");
	}
	else if(strcmp(newLine, "InitArenaReg") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INITARENAREG;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INITARENAREG Parameter Mismatch");
	}
	else if(strcmp(newLine, "EnableMenuButton") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_ENABLEMENUBUTTON;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_ENABLEMENUBUTTON Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetGroupedToggle") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETGROUPEDTOGGLE;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETGROUPEDTOGGLE Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestArenaTypes") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTARENATYPES;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_REQUESTARENATYPES Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestArenaInfo") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTARENAINFO;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_REQUESTARENAINFO Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestArenaLeave") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTARENALEAVE;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_REQUESTARENALEAVE Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowArenaTypesByMode") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SHOWARENATYPESBYMODE;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SHOWARENATYPESBYMODE Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplaySelectedArenaInfo") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYSELECTEDARENAINFO;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DISPLAYSELECTEDARENAINFO Parameter Mismatch");
	}
	if(strcmp(newLine, "UpdateArenaCountdown") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEARENACOUNTDOWN;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEARENACOUNTDOWN Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowImage") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SHOWIMAGE;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SHOWIMAGE Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateArenaTypeDescription") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEARENATYPEDESCRIPTION;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEARENATYPEDESCRIPTION Parameter Mismatch");
	}
	else if(strcmp(newLine, "JoinArena") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_JOINARENA;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_JOINARENA Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateResistImage") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATERESISTIMAGE;
		if(!GetParametersSafe(paramString,stringLength,curChar,4,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATERESISTIMAGE Parameter Mismatch");
	}
	else if(strcmp(newLine, "EvaluateLevel") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_EVALUATELEVEL;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_EVALUATELEVEL Parameter Mismatch");
	}
	else if(strcmp(newLine, "EvaluateXP") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_EVALUATEXP;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_EVALUATEXP Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateProgress") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_XPPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_XPPROGRESS Parameter Mismatch");
	}
	else if(strcmp(newLine, "InviteToGroup") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INVITETOGROUP;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INVITETOGROUP Parameter Mismatch");
	}
	else if(strcmp(newLine, "InviteToSquad") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INVITETOSQUAD;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INVITETOSQUAD Parameter Mismatch");
	}
	else if(strcmp(newLine, "RemoveFromSquad") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REMOVEFROMSQUAD;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_REMOVEFROMSQUAD Parameter Mismatch");
	}
	else if(strcmp(newLine, "LeaveSquad") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_LEAVESQUAD;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_LEAVESQUAD Parameter Mismatch");
	}
	else if(strcmp(newLine, "MakeLeaderOfSquad") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_MAKELEADEROFSQUAD;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_MAKELEADEROFSQUAD Parameter Mismatch");
	}
	else if(strcmp(newLine, "InitTrade") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INITTRADE;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INITTRADE Parameter Mismatch");
	}
	else if(strcmp(newLine, "SaveLastDragItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SAVELASTDRAGITEM;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SAVELASTDRAGITEM Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateRecentJournalItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATE_RECENTJOURNALITEM;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATE_RECENTJOURNALITEM Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestBadges") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTBADGES;
		if(!GetParametersSafe(paramString,stringLength,curChar,0,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_REQUESTBADGES Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateBadgeView") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SHOWBADGE;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SHOWBADGE Parameter Mismatch");
	}	
	else if(strcmp(newLine, "RequestFullJournal") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REQUESTFULLJOURNAL;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))//param 0 will be the return target list
			globalStatus->UI_Log("MENU_SCRIPTFUNC_REQUESTFULLJOURNAL Parameter Mismatch");
	}	
	else if(strcmp(newLine, "TextFlash") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_TEXTFLASH;
		if(!GetParametersSafe(paramString,stringLength,curChar,4,curLine))//textID,alphaStart,alphaend,duration
			globalStatus->UI_Log("MENU_SCRIPTFUNC_TEXTFLASH Parameter Mismatch");
	}
	else if(strcmp(newLine, "ButtonFlash") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_FLASHBUTTON;
		if(!GetParametersSafe(paramString,stringLength,curChar,4,curLine))//textID,alphaStart,alphaend,duration
			globalStatus->UI_Log("MENU_SCRIPTFUNC_FLASHBUTTON Parameter Mismatch");
	}
	else if(strcmp(newLine, "ProgressFlash") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_FLASHPROGRESS;
		if(!GetParametersSafe(paramString,stringLength,curChar,4,curLine))//textID,alphaStart,alphaend,duration
			globalStatus->UI_Log("MENU_SCRIPTFUNC_FLASHPROGRESS Parameter Mismatch");
	}
	else if(strcmp(newLine, "RemoveItemForTrade") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REMOVEITEMFORTRADE;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_REMOVEITEMFORTRADE Parameter Mismatch");
	}
	else if(strcmp(newLine, "AddItemForTrade") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_ADDITEMFORTRADE;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_ADDITEMFORTRADE Parameter Mismatch");
	}
	else if(strcmp(newLine, "ResetList") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_RESETLIST;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))//listboxID
			globalStatus->UI_Log("MENU_SCRIPTFUNC_RESETLIST Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplayQuestJournalDetails") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYQUESTJOURNALDETAILS;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))//listboxID
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DISPLAYQUESTJOURNALDETAILS Parameter Mismatch");
	}
	else if(strcmp(newLine, "RefreshQuestJournal") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REFRESHQUESTJOURNAL;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))//listboxID
			globalStatus->UI_Log("MENU_SCRIPTFUNC_REFRESHQUESTJOURNAL Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateLoadingItems") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATELOADINGITEMS;
		if(!GetParametersSafe(paramString,stringLength,curChar,2,curLine))//listboxID
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATELOADINGITEMS Parameter Mismatch");
	}
	else if(strcmp(newLine, "HideHoverMenu") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_HIDEHOVERMENU;
		if(!GetParametersSafe(paramString, stringLength, curChar, 0, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_HIDEHOVERMENU Parameter Mismatch");
	}
	else if(strcmp(newLine, "InitMenu") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_INITMENU;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_INITMENU Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetActiveInventory") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETACTIVEINVENTORY;
		if(!GetParametersSafe(paramString, stringLength, curChar, 2, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETACTIVEINVENTORY Parameter Mismatch");
	}
	else if(strcmp(newLine, "SelectInventoryItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SELECTINVENTORYITEM;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SELECTINVENTORYITEM Parameter Mismatch");
	}
	else if(strcmp(newLine, "AddToQuickbar") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_ADDTOQUICKBAR;
		if(!GetParametersSafe(paramString, stringLength, curChar, 0, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_ADDTOQUICKBAR Parameter Mismatch");
	}
	else if(strcmp(newLine, "RemoveFromQuickbar") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_REMOVEFROMQUICKBAR;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_REMOVEFROMQUICKBAR Parameter Mismatch");
	}
	else if(strcmp(newLine, "DepositItemToBank") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DEPOSITITEMTOBANK;
		if(!GetParametersSafe(paramString, stringLength, curChar, 2, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DEPOSITITEMTOBANK Parameter Mismatch");
	}
	else if(strcmp(newLine, "WithdrawItemFromBank") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_WITHDRAWITEMFROMBANK;
		if(!GetParametersSafe(paramString, stringLength, curChar, 2, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_WITHDRAWITEMFROMBANK Parameter Mismatch");
	}
	else if(strcmp(newLine, "SelectQuickbarItem") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SELECTQUICKBARITEM;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SELECTQUICKBARITEM Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateFactionBonus") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEFACTIONBONUS;
		if(!GetParametersSafe(paramString, stringLength, curChar, 2, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEFACTIONBONUS Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateFactionTag") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEFACTIONTAG;
		if(!GetParametersSafe(paramString, stringLength, curChar, 2, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEFACTIONTAG Parameter Mismatch");
	}
	else if(strcmp(newLine, "PageGM") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_PAGEGM;
		if(!GetParametersSafe(paramString, stringLength, curChar, 0, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_PAGEGM Parameter Mismatch");
	}
	else if(strcmp(newLine, "SendToMessageChannel") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SENDMESSAGETOCHANNEL;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SENDMESSAGETOCHANNEL Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateMessageChannel") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEMESSAGECHANNEL;
		if(!GetParametersSafe(paramString, stringLength, curChar, 2, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEMESSAGECHANNEL Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetScrollBar") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETSCROLLBAR;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETSCROLLBAR Parameter Mismatch");
	}
	else if(strcmp(newLine, "StartTrade") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_STARTTRADE;
		if(!GetParametersSafe(paramString, stringLength, curChar, 0, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_STARTTRADE Parameter Mismatch");
	}
	else if(strcmp(newLine, "SeeGroupInvite") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SEEGROUPINVITE;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SEEGROUPINVITE Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateVisibilityOn") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEVISIBILITYON;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEVISIBILITYON Parameter Mismatch");
	}
	else if(strcmp(newLine, "UpdateVisibilityOff") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_UPDATEVISIBILITYOFF;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATEVISIBILITYOFF Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowStoreHover") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SHOWSTOREHOVER;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SHOWSTOREHOVER Parameter Mismatch");
	}
	else if(strcmp(newLine, "DisplayNearbyPlayers") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_DISPLAYNEARBYPLAYERS;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_DISPLAYNEARBYPLAYERS Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetVisible") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETVISIBLE;
		if(!GetParametersSafe(paramString, stringLength, curChar, 2, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETVISIBLE Parameter Mismatch");
	}
	else if(strcmp(newLine, "SetActiveBankLocker") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SETACTIVEBANKLOCKER;
		if(!GetParametersSafe(paramString, stringLength, curChar, 3, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SETACTIVEBANKLOCKER Parameter Mismatch");
	}
	else if(strcmp(newLine, "HideChatMenu") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_HIDECHATMENU;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_HIDECHATMENU Parameter Mismatch");
	}
	else if(strcmp(newLine, "ShowChatMenu") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SHOWCHATMENU;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SHOWCHATMENU Parameter Mismatch");
	}
	else if(strcmp(newLine, "OpenSquadMenu") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_OPENSQUADMENU;
		if(!GetParametersSafe(paramString, stringLength, curChar, 0, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_OPENSQUADMENU Parameter Mismatch");
	}
	else if(strcmp(newLine, "CreateSquad") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_CREATESQUAD;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_CREATESQUAD Parameter Mismatch");
	}
	else if(strcmp(newLine, "UseSkill") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_USESKILL;
		if(!GetParametersSafe(paramString, stringLength, curChar, 3, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_USESKILL Parameter Mismatch");
	}
	else if(strcmp(newLine, "SendCommand") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SENDCOMMAND;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SENDCOMMAND Parameter Mismatch");
	}
	else if(strcmp(newLine, "RequestJoinClan") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_CLANJOINREQUEST;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_CLANJOINREQUEST Parameter Mismatch");
	}
	else if(strcmp(newLine, "CreateClan") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_CLANCREATE;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_CLANCREATE Parameter Mismatch");
	}
	else if(strcmp(newLine, "RemoveClanMember") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_CLANREMOVEMEMBER;
		if(!GetParametersSafe(paramString, stringLength, curChar, 1, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_CLANREMOVEMEMBER Parameter Mismatch");
	}
	else if(strcmp(newLine, "SwapMenus") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_SWAPMENUS;
		if(!GetParametersSafe(paramString, stringLength, curChar, 2, curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_SWAPMENUS Parameter Mismatch");
	}
	else if(strcmp(newLine, "InvertMulti") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_TOGGLEMULTI;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_TOGGLEMULTI Parameter Mismatch");
	}
	else if(strcmp(newLine, "TestAH") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_TESTAH;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_TESTAH Parameter Mismatch");
	}
	else if(strcmp(newLine, "BuyFromAH") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_BUYFROMAH;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_BUYFROMAH Parameter Mismatch");
	}
	else if(strcmp(newLine, "AddToAH") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_ADDTOAH;
		if(!GetParametersSafe(paramString,stringLength,curChar,3,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_ADDTOAH Parameter Mismatch");
	}
	else if(strcmp(newLine, "PopulateStatsAH") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_POPULATEAHSTATS;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_POPULATEAHSTATS Parameter Mismatch");
	}
	else if(strcmp(newLine, "ListByItemAH") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_LISTBYITEMAH;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_LISTBYITEMAH Parameter Mismatch");
	}
	else if(strcmp(newLine, "NextItemsAH") == 0)
	{
		curLine->m_function = MENU_SCRIPTFUNC_NEXTITEMSAH;
		if(!GetParametersSafe(paramString,stringLength,curChar,1,curLine))
			globalStatus->UI_Log("MENU_SCRIPTFUNC_NEXTITEMSAH Parameter Mismatch");
	}
}

void CMenuScript::ClearScript()
{
	CScriptLine *curLine = m_pFirstLine;

	if(curLine)
	{
		//Loop through the script
		while(curLine && curLine->m_pNextLine)
		{
			//Move to the next pointer and use its previous to delete the current object
			curLine = curLine->m_pNextLine;
			SafeDelete(curLine->m_pPrevLine);
		}

		//Delete the only remaining line
		SafeDelete(curLine);
		m_pFirstLine = NULL;
	}

	m_scriptText = "";
}

BOOL CMenuScript::GetParametersSafe(char *line, int lineLength, int &curChar,int paramCount,CScriptLine * curLine)
{
	// NOTE: We are allowing zero parameter scripts here to clean up log spam in GeneralClientLog.txt!
    if(paramCount < 1)
	   return TRUE;

	//if(!curLine->m_strParams)
	if(curLine->m_strParams)
	{
        delete [] curLine->m_strParams;
		curLine->m_strParams = 0;
	}
	
	curLine->m_strParams = new CString[paramCount];
    curLine->m_allocatedParamCount = paramCount;
    int iLoop = 0;
	//Get the parameters
	for(iLoop = 0;iLoop < paramCount;iLoop++)
	{
	    if(GetParameterString(line, curChar, lineLength, m_paramConversion))
           curLine->m_strParams[iLoop] = m_paramConversion;
		else 
		   break;
	}

	if(iLoop != paramCount)
       return FALSE;
	
	return TRUE;
}

BOOL CMenuScript::GetParameterString(char *line, int &curChar, int & bufLength, CString & returnParam)
{
	ZeroMemory(&m_paramTemp,sizeof(MAX_BUFFER));
	//char *param = new char[MAX_BUFFER];
	int i = 0;
	bool foundBeginning = false;

	//Loop until we hit a comma or closing parentheses, which is the end of this parameter
	while(curChar < bufLength)
	{
        if(line[curChar] == ',')
		   break;

		if(line[curChar] == ')')
		   break;

		//If we haven't yet found a " or $ which is the beginning of the parameter
		/*if(!foundBeginning)
		{
			if(line[curChar] == '"')
				foundBeginning = true;

			if(line[curChar] == '$')
			{
				foundBeginning = true;
				continue;
			}

			curChar++;
			continue;
		}

		//If we hit the closing ", break from the loop
		if(line[curChar] == '"')
			break;*/

		m_paramTemp[i++] = line[curChar++];
	}

	//NULL terminate the parameter string
	m_paramTemp[i] = '\0';

	//Skip over the , or )
	curChar++;

	//If this is a variable
	/*if(m_paramTemp[0] == '$')
	{
		if(strcmp(m_paramTemp + 1, "FPS") == 0)
			returnParam = globalStatus->m_cabLib.ConvToString(globalStatus->m_exactFPSHiRes);
		if(strcmp(m_paramTemp + 1, "monkey") == 0)
			returnParam = globalStatus->m_currentWorkingFile;
		else
			returnParam = m_paramTemp;
	}
	else*/
		returnParam = m_paramTemp;


	if(i > 0)
	   return TRUE;

	return FALSE;
}

void CMenuScript::ExecuteScript()
{
	if (m_isBeingEdited)
		return;

	CScriptLine *curLine = m_pFirstLine;
	char buffer[10];

	//Loop through the script, line by line
	while(curLine)
	{
		switch(curLine->m_function)
		{
			case MENU_SCRIPTFUNC_QUIT:
			{
				globalStatus->m_closeDownCall = TRUE;
				break;
			}
			case MENU_SCRIPTFUNC_CLOSE:
			{
				globalStatus->m_runtimeMenuDatabase->CloseByID(curLine->GetIntByParam(0), &globalStatus->m_showMenuCursor);
				break;
			}
			case MENU_SCRIPTFUNC_CLOSEALL:
			{
				globalStatus->m_runtimeMenuDatabase->CloseAll(&globalStatus->m_showMenuCursor);
				break;
			}
			case MENU_SCRIPTFUNC_INITQUANTITY:
			{
				CMenuTextBox *pQuantityText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(pQuantityText)
				{
					// Give the textbox focus and set the cursor position
					pQuantityText->m_isFocused = true;
					pQuantityText->m_showCursor = true;

					// Clear pre-existing text
					pQuantityText->m_fullText.SetString("");

					// Initialize the quantity displayed in the textbox
					int iQuantity = curLine->GetIntByParam(1);
					if(iQuantity > 0)
						pQuantityText->SetText(curLine->GetStrByParam(1));
					else
						pQuantityText->SetText("1");
				}
				break;
			}
			case MENU_SCRIPTFUNC_BUY:
			{
				// ERROR checking: Make sure we're in the game client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				// GET references to menu visuals
				int quantity = 1;
				CMenuTextBox *pBuyAmountText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				CMenuInventory *pStoreInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true));
				CMenuInventory *pPlayerInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
				if (!pBuyAmountText 
				|| !pStoreInventory
				|| !pStoreInventory->SelectedItem()
				|| !pPlayerInventory)
					break;

				// GET the amount specified for purchase
				int iAmount = atoi(pBuyAmountText->m_linesOfText[0].m_text);
				if(iAmount < 1)
				{
					globalStatus->ShowMessageBox("Buy Item", "Please specify an amount greater than 0 to purchase!");
					break;
				}
				else if(iAmount > 30000)
				{
					globalStatus->ShowMessageBox("Buy Item", "Please specify an amount less than 30,000 to purchase!");
					break;
				}
	
				// GET the client-side player object
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if(!pos) break;
				CMovementObj *pMovementObject = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(pos);

				// GET the item the player is attempting to purchase
				CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(pStoreInventory->SelectedItem()->m_data.m_GLID);
				if (!pItem) break;

				// ERROR checking: Make sure the player can afford their potential purchase
				if (globalStatus->m_multiplayerObject->m_cashCount < (pItem->m_marketCost * iAmount)) 
					break;

				// ADD the purchase to the player's inventory on the client
				for (int i=0; i < iAmount; i++)
				{
					// NOTE: We have to set the "quantity" data member here before
					//	adding it to the player's inventory because it is 
					//	being used to denote item cost for the store!
					pStoreInventory->SelectedItem()->m_data.m_quantity = 1; //iAmount;
					pPlayerInventory->AddItem(pStoreInventory->SelectedItem()->m_data);
				}
				
				// ADD the purchase to the player's inventory on the server
				globalStatus->m_netGameLogicObject->SendAttribMsg(
					DPSEND_GUARANTEED,
					DPID_SERVERPLAYER,
					21,
					pStoreInventory->SelectedItem()->m_data.m_GLID,
					pMovementObject->List_networkDefinedID,
					0,
					iAmount,
					NULL);
				break;
			}			
			case MENU_SCRIPTFUNC_SELL:
			{
				// ERROR checking: Make sure we're in the game client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				// GET the client-side player object
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if (!pos) break;
				CMovementObj *pMovementObject = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(pos);
				if (!pMovementObject) break;

				// GET references to menu visuals
				CMenuTextBox *pSellAmountText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true));
				if (!pSellAmountText
				|| !pInventory
				|| !pInventory->SelectedItem())
					break;

				// GET the amount the player is trying to sell
				int iAmount = atoi(pSellAmountText->m_linesOfText[0].m_text);
				if(iAmount < 1)
				{
					globalStatus->ShowMessageBox("Sell Item", "Please specify an amount greater than 0 to sell!");
					break;
				}
				else if(iAmount > 30000)
				{
					globalStatus->ShowMessageBox("Sell Item", "Please specify an amount less than 30,000 to sell!");
					break;
				}

				// ERROR checking: Can't sell equipped items!
				if(pInventory->SelectedItem()->m_data.m_armed)
				{
					globalStatus->ShowMessageBox("Sell Item", "Please unequip that item before trying to sell it!");
					break;
				}
				
				// ERROR checking: Make sure the player has enough to sell!
				ITEMDATA_STRUCT_EX item = pInventory->SelectedItem()->m_data;
				if (iAmount > item.m_quantity)
					iAmount = item.m_quantity;

				// REMOVE the specified amount of the item from the player's client-side inventory
				pInventory->RemoveItem(item.m_GLID, iAmount);

				// REMOVE the specified amount of the item from the player's server-side inventory
				globalStatus->m_netGameLogicObject->SendAttribMsg(
					DPSEND_GUARANTEED,
					DPID_SERVERPLAYER,
					22,
					item.m_GLID,
					pMovementObject->List_networkDefinedID,
					0,
					iAmount,
					NULL);
				break;
			}
			case MENU_SCRIPTFUNC_PLAYCHARACTER:
			{
				CMenuListBox *visual = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				if(globalStatus->m_multiplayerObject->GetEnabledClient_safe() && visual->m_currentSelection)
					if(visual->m_currentSelection->m_index > -1)
					{ //Enabled Client and has a Selection
						if (visual->m_currentSelection->m_index == 0)
						{ //Index is 0: Check for Character
							POSITION posCurVisual = visual->m_currentSelection->m_textList.GetHeadPosition();
							CMenuText *textObject = (CMenuText*)visual->m_currentSelection->m_textList.GetNext(posCurVisual);
							
							if(textObject)
								if(strcmp(textObject->m_text, "No characters Created Yet") == 0)
									break;
						} //End Index is 0: Check for Character

						//Player has a valid selection, Send to Server	
						globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																		DPID_SERVERPLAYER,
																		9,
																		visual->m_currentSelection->m_index,
																		0,
																		globalStatus->m_localContentClientVersion,
																		15,//VERSION EXPLOIT
																		NULL);
						
						//Set Current Character even though it's pending
						globalStatus->m_multiplayerObject->m_currentCharacterInUse = visual->m_currentSelection->m_index;
						
						//Disable the button to prevent multiple clicks
						m_callingVisual->m_enabled = false;

					} //End Enabled Client and has a Selection
				break;
			}
			case MENU_SCRIPTFUNC_DELETECHARACTER:
			{
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				CMenuListBox *visual = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				if(!visual)
					break;
				if(!visual->m_currentSelection)
					break;

				globalStatus->m_netGameLogicObject->SendAttribMsg( DPNSEND_GUARANTEED,
																   DPID_SERVERPLAYER,
																   10,
																   visual->m_currentSelection->m_index,
																   0,
																   0,
																   0,
																   NULL);
				break;
			}
			case MENU_SCRIPTFUNC_LOOT:
			{
				// ERROR checking: Make sure we are in the game client!
				if (!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
			
				// GET references to menu visuals
				int quantity = 1;
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if (!pInventory || !pInventory->SelectedItem())
				{
					// CLOSE the loot menu on fail
					globalStatus->m_runtimeMenuDatabase->CloseByID(ID_LOOT_MENU, &globalStatus->m_showMenuCursor);
					break;
				}

				// GET the client-side player object
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if (!pos) break;
				CMovementObj *pMovementObject = (CMovementObj *)globalStatus->m_entObjRuntime->GetAt(pos);
				if(!pMovementObject) break;

				// LOOT the selected item in the loot inventory
				CMenuInventoryItem *pItem = pInventory->SelectedItem();
				globalStatus->m_netGameLogicObject->SendAttribMsg(
					DPNSEND_GUARANTEED,
					DPID_SERVERPLAYER,
					23,
					pItem->m_data.m_GLID,
					pMovementObject->List_networkDefinedID,
					0,
					pItem->m_data.m_quantity,
					NULL);

				pInventory->RemoveItem(pItem->m_data.m_GLID, pItem->m_data.m_quantity);

				// CLOSE the loot menu
				if(pInventory->Count() == 0)
					globalStatus->m_runtimeMenuDatabase->CloseByID(ID_LOOT_MENU, &globalStatus->m_showMenuCursor);
				break;
			}
			case MENU_SCRIPTFUNC_LOOTALL:
			{
				// ERROR checking: Make sure we are in the game client!
				if (!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
			
				// GET references to menu visuals
				int quantity = 1;
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if (!pInventory || (pInventory->Count() <= 0))
				{
					// CLOSE the loot menu on fail
					globalStatus->m_runtimeMenuDatabase->CloseByID(ID_LOOT_MENU, &globalStatus->m_showMenuCursor);
					break;
				}

				// GET the client-side player object
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if (!pos) break;
				CMovementObj *pMovementObject = (CMovementObj *)globalStatus->m_entObjRuntime->GetAt(pos);
				if(!pMovementObject) break;

				// LOOP and LOOT through all items in the loot inventory
				//for (int i=0; i<pInventory->Count(); i++)
				//{
					//CMenuInventoryItem *pItem = pInventory->m_items[i];
					//if (!pItem) continue;
					globalStatus->m_netGameLogicObject->SendAttribMsg(
						DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						48,
						0,//pItem->m_data.m_GLID,
						pMovementObject->List_networkDefinedID,
						0,
						1,//pItem->m_data.m_quantity,
						NULL);
				//}

				// CLOSE the loot menu
				globalStatus->m_runtimeMenuDatabase->CloseByID(ID_LOOT_MENU, &globalStatus->m_showMenuCursor);
				break;
			}
			case MENU_SCRIPTFUNC_QUANTITY:
			{
				// PARSE script params
				int iMenuID = curLine->GetIntByParam(0);

				// GET reference to menu visual
				CMenuInventory *pInventory = globalStatus->GetActiveInventory((MENU_IDS)iMenuID);
				if (!pInventory || !pInventory->SelectedItem()) break;

				// GET the selected item object
				CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(pInventory->SelectedItem()->m_data.m_GLID);
				if ((pItem->m_useType == 11) 
			/*	|| (pItem->m_useType == 12)*/)
					break;

				// OPEN the quantity menu
				globalStatus->OpenMenu(curLine->GetIntByParam(1), false);
				break;
			}
			case MENU_SCRIPTFUNC_REQUESTCHARACTERS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				CMenuListBox* visualReference = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				//Create Queue Entry
				CAttribQueueEntry *curEntry = new CAttribQueueEntry();
				curEntry->m_requestedAttrib = 50;
				curEntry->m_requester = visualReference;
				globalStatus->m_attribQueue->AddTail(curEntry);

				if(visualReference)
				{
					//Clear Existing List
					while(visualReference->m_pFirstItem != NULL)
						visualReference->RemoveItem(0);
				}

				//Send Attrib Message (Request Character List)
				globalStatus->m_netGameLogicObject->SendAttribMsg( DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																14,
																50, //Requested Attrib ID
																0,
																0,
																0,
																NULL);



				break;
			}
			case MENU_SCRIPTFUNC_REQUESTINVENTORY:
			{
				// GET params
				int iInventoryVisualID = curLine->GetIntByParam(0);

				// ERROR checking: Make sure this is an enabled client!
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create Visual Reference and posLoc
				/*CMenuListBox* visualReference = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				CMenuWindow* visual = (CMenuWindow*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				//Confirm VisualReference and posLoc
				if(!visualReference || !posLoc)
					break;*/

				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);

				// GET a reference to the menu visual and
				//	CLEAR the local copy of the player's inventory
				CMenuInventory *pMenu = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iInventoryVisualID, true));
				if (!pMenu)
					break;
				pMenu->ClearItems();

				//Create Attrib Queue Entry
				/*CAttribQueueEntry *curEntry = new CAttribQueueEntry();
				curEntry->m_requestedAttrib = 51;
				curEntry->m_requester = visualReference;
				globalStatus->m_attribQueue->AddTail(curEntry);
				
				//Clear Existing List
				while(visualReference->m_pFirstItem != NULL)
					visualReference->RemoveItem(0);*/

				//Send Attrib Message for Inventory List
				globalStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
					DPID_SERVERPLAYER,
					19,
					iInventoryVisualID,//Requested Attrib ID
					ptrLoc->List_networkDefinedID,
					0,
					3, //Filter Type 0 = none, 1 = skills only, 2 = spells only, 3 = no skills / spells
					NULL);//request character inventory
				
				break;
			}
			case MENU_SCRIPTFUNC_REQUESTSPELLSINVENTORY:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create Visual Reference and posLoc
				CMenuListBox* visualReference = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Confirm VisualReference and posLoc
				if(!visualReference || !posLoc)
					break;

				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);

				//Create Attrib Queue Entry
				CAttribQueueEntry *curEntry = new CAttribQueueEntry();
				curEntry->m_requestedAttrib = 52;
				curEntry->m_requester = visualReference;
				globalStatus->m_attribQueue->AddTail(curEntry);
				
				//Clear Existing List
				while(visualReference->m_pFirstItem != NULL)
					visualReference->RemoveItem(0);

				//Send Attrib Message for Inventory List
				globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																19,
																52,//Requested Attrib ID
																ptrLoc->List_networkDefinedID,
																0,
																2, //Filter Type 0 = none, 1 = skills only, 2 = spells only, 3 = no skills / spells
																NULL);//request character inventory
				
				break;
			}
			case MENU_SCRIPTFUNC_REQUESTSKILLSINVENTORY:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create Visual Reference and posLoc
				CMenuListBox* visualReference = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Confirm VisualReference and posLoc
				if(!visualReference || !posLoc)
					break;

				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);

				//Create Attrib Queue Entry
				CAttribQueueEntry *curEntry = new CAttribQueueEntry();
				curEntry->m_requestedAttrib = 53;
				curEntry->m_requester = visualReference;
				globalStatus->m_attribQueue->AddTail(curEntry);
				
				//Clear Existing List
				while(visualReference->m_pFirstItem != NULL)
					visualReference->RemoveItem(0);

				//Send Attrib Message for Inventory List
				globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																19,
																53,//Requested Attrib ID
																ptrLoc->List_networkDefinedID,
																0,
																1, //Filter Type 0 = none, 1 = skills only, 2 = spells only, 3 = no skills / spells
																NULL);//request character inventory
				
				
				break;
			}
			case MENU_SCRIPTFUNC_REQUESTSTOREINVENTORY:
			{
				// ERROR checking: Only execute if this is the game client!
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// PARSE out script params
				int iMenuID = curLine->GetIntByParam(0);

				// GET references to menu visuals
				CMenuInventory *pInventory = globalStatus->GetActiveInventory((MENU_IDS)iMenuID);
				if (!pInventory) break;

				// GET a reference to the client-side player object
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if (!pos) break;
				CMovementObj *pMovementObject = dynamic_cast<CMovementObj*>(globalStatus->m_entObjRuntime->GetAt(pos));
				if (!pMovementObject) break;

				// CLEAR the store inventory of old items
				pInventory->ClearItems();

				// SEND the store inventory request to the server
				if ((curLine->GetIntByParam(1) == 1)
				&& (pMovementObject->List_controlType == 0))
				{
					globalStatus->m_netGameLogicObject->SendTextMsg(
						DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						pMovementObject->List_networkDefinedID,
						NULL,
						0,
						"",
						SCMD_BUY);
				}
				break;
			}
			case MENU_SCRIPTFUNC_REQUESTLOOTINVENTORY:
			{
				// ERROR checking: Make sure we are in the game client!
				if (!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// GET references to menu visuals
				CMenuInventory* pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if (!pInventory) break;

				// GET a reference to the client-side player object
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if (!pos) break;
				CMovementObj *pMovementObject = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(pos);
				if (!pMovementObject) break;

				// CLEAR the loot inventory of old items
				pInventory->ClearItems();

				// SEND loot inventory request to server
				if ((curLine->GetIntByParam(1) == 1)
				&& (pMovementObject->List_controlType == 0))
					globalStatus->SendChatString(globalStatus->m_globalInteractCommand);
				break;
			}
			case MENU_SCRIPTFUNC_REQUESTINTERACTION:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create Visual Reference and position
				CMenuText *visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);
				
				//Confirm VisualReference and entPosition
				if(!visual || !entPosition)
					break;

				//Look for existing attrib queue entries
				bool existingQueue = false;
				POSITION posCurVisual = globalStatus->m_attribQueue->GetHeadPosition();
				while(posCurVisual != NULL)
				{
					CAttribQueueEntry *entry = (CAttribQueueEntry*)globalStatus->m_attribQueue->GetNext(posCurVisual);
					if(entry->m_requestedAttrib == 55)
						existingQueue = true;
				}
				
				//Create Attrib Queue Entry if Needed
				if (!existingQueue)
				{
					CAttribQueueEntry *curEntry = new CAttribQueueEntry();
					curEntry->m_requestedAttrib = 55;
					curEntry->m_requester = visual;
					globalStatus->m_attribQueue->AddTail(curEntry);
				}

				//Needs to send the interaction request to server
				if(curLine->GetIntByParam(1) == 1)
				{
	
					CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(entPosition);
					if(ptrLoc)
						if(ptrLoc->List_controlType == 0)
						   globalStatus->SendChatString(globalStatus->m_globalInteractCommand);
				}
				
				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYCREDITS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				//Create Visual Reference and Set Text
				CMenuText* visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				
				m_tempRegString = globalStatus->m_cabLib.ConvertLongToString(globalStatus->m_multiplayerObject->m_cashCount);
				{// 209000070 -> 209,000,070
					CString digits = m_tempRegString, sign = "";
					if (digits.GetLength() > 0 && digits[0] == '-') { sign = "-"; digits = digits.Mid(1); }
					CString grouped = "";
					int n = digits.GetLength();
					for (int i = 0; i < n; i++)
					{
						if (i > 0 && (n - i) % 3 == 0)
							grouped += ",";
						grouped += digits[i];
					}
					m_tempRegString = sign + grouped;
				}
				
				if(visual)//valid
				if(visual->m_text != m_tempRegString)
				{//lets not recreate unless changed
					visual->m_text = m_tempRegString;

					//Delete the Text Mesh if Exists so it will re-create
					if(visual->m_textMesh)
					{
						visual->m_textMesh->SafeDelete();
						delete visual->m_textMesh;
						visual->m_textMesh = NULL;
					}
				}//lets not recreate unless changed
				
				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYSTAT:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				//Create Visual Reference and Set Text
				CMenuText* visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				if(curLine->GetIntByParam(1) == 0)
					m_tempRegString = globalStatus->m_cabLib.ConvToString(globalStatus->m_multiplayerObject->m_currentStat1); //Str
				else if(curLine->GetIntByParam(1) == 1)
					m_tempRegString = globalStatus->m_cabLib.ConvToString(globalStatus->m_multiplayerObject->m_currentStat2); //Dex
				else if(curLine->GetIntByParam(1) == 2)
					m_tempRegString = globalStatus->m_cabLib.ConvToString(globalStatus->m_multiplayerObject->m_currentStat3); //Wis
				
				if(visual)//valid
				if(visual->m_text != m_tempRegString)
				{//lets not recreate unless changed
					visual->m_text = m_tempRegString;

					//Delete the Text Mesh if Exists so it will re-create
					if(visual->m_textMesh)
					{
						visual->m_textMesh->SafeDelete();
						delete visual->m_textMesh;
						visual->m_textMesh = NULL;
					}
				}//lets not recreate unless changed

				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYRESIST:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create Visual Reference and Set Text
				CMenuText* visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				int resistIndex = curLine->GetIntByParam(1);


				if(!globalStatus->m_multiplayerObject->m_elementStatus)
					break;

				for(POSITION posLoc = globalStatus->m_multiplayerObject->m_elementStatus->GetHeadPosition(); posLoc != NULL;)
				{
					CElementStatusObj *eleObject = (CElementStatusObj *)globalStatus->m_multiplayerObject->m_elementStatus->GetNext(posLoc);
					if(eleObject->m_elementIndex == resistIndex)
					{
						m_tempRegString = globalStatus->m_cabLib.ConvToString((int)eleObject->m_currentValue);
						m_tempRegString = m_tempRegString;
						break;
					}
				}
				
				if(visual)
				{//valid
					if(visual->m_text != m_tempRegString)
					{//lets not recreate unless changed
						visual->m_text = m_tempRegString;

						//Delete the Text Mesh if Exists so it will re-create
						if(visual->m_textMesh)
						{//valid textMesh
							visual->m_textMesh->SafeDelete();
							delete visual->m_textMesh;
							visual->m_textMesh = NULL;
						}//end valid textMesh
					}//lets not recreate unless changed
				}//end valid

				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYWOUND:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create Visual Reference and Set Text
				CMenuText* visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);
				CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(entPosition);

				m_tempRegString = globalStatus->m_cabLib.ConvToString((int)ptrLoc->m_woundChannel[curLine->GetIntByParam(1)]);
				
				if(visual)//valid
				if(visual->m_text != m_tempRegString)
				{//lets not recreate unless changed
					visual->m_text = m_tempRegString;

					//Delete the Text Mesh if Exists so it will re-create
					if(visual->m_textMesh)
					{
						visual->m_textMesh->SafeDelete();
						delete visual->m_textMesh;
						visual->m_textMesh = NULL;
					}
				}//lets not recreate unless changed

				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYCHARNAME:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create Visual Reference and Set Text
				CMenuText* visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);
				CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(entPosition);

				m_tempRegString = ptrLoc->List_name;
				
				if(visual)//valid
				if(visual->m_text != m_tempRegString)
				{//lets not recreate unless changed
					visual->m_text = m_tempRegString;

					//Delete the Text Mesh if Exists so it will re-create
					if(visual->m_textMesh)
					{
						visual->m_textMesh->SafeDelete();
						delete visual->m_textMesh;
						visual->m_textMesh = NULL;
					}
				}//lets not recreate unless changed
				
				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYHEALTH:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				//Create position and Visual Reference
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				CMenuText* visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				//Make sure there is a valid posLoc and Visual Reference
				if(!visual || !posLoc)
				    break;

				//Set Text to Health Ratio
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);
				
				
				m_tempRegString = globalStatus->m_cabLib.ConvToString(ptrLoc->List_energy) +  " / " + globalStatus->m_cabLib.ConvToString(ptrLoc->List_maxEnergy);
                if(m_tempRegString != visual->m_text)
				{//lets not recreate unless changed
					visual->m_text = m_tempRegString;

					//Delete the Text Mesh if Exists so it will re-create
					if(visual->m_textMesh)
					{
						visual->m_textMesh->SafeDelete();
						delete visual->m_textMesh;
						visual->m_textMesh = NULL;
					}
				}//end lets not recreate unless changed
			
					
				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYENERGY:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuText *visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Make sure there is a valid posLoc and Visual
				if(!visual || !posLoc)
					break;

				CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(posLoc);
				
				
				/*m_tempRegString = globalStatus->m_cabLib.ConvToString(ptrLoc->m_energyCurrent) + " / " + globalStatus->m_cabLib.ConvToString(ptrLoc->m_energyMax);*/
				m_tempRegString = globalStatus->m_cabLib.ConvToString((int)ptrLoc->m_energyCurrent) + " / " + globalStatus->m_cabLib.ConvToString((int)ptrLoc->m_energyMax);
				//WARNING: C4244 --> float to int (ConvToString func)

				if(visual->m_text != m_tempRegString)
				{//lets not recreate unless changed
					visual->m_text = m_tempRegString;

					//Delete the Text Mesh if Exists so it will re-create
					if(visual->m_textMesh)
					{
						visual->m_textMesh->SafeDelete();
						delete visual->m_textMesh;
						visual->m_textMesh = NULL;
					}
				}//end lets not recreate unless changed

				break;
			}
			case MENU_SCRIPTFUNC_SETSYSTEMOUTPUT:
			{
				globalStatus->m_systemReadout = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				globalStatus->m_chatReadout = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true));
				break;
			}
			case MENU_SCRIPTFUNC_SETCHATINPUT:
			{
				globalStatus->m_menuChat		= (CMenuWindow*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), false);
				if(globalStatus->m_menuChat)
				   globalStatus->m_menuChatInput	= (CMenuTextBox*)globalStatus->m_menuChat->m_childList->GetVisualByID(curLine->GetIntByParam(1), false);
				break;
			}
			case MENU_SCRIPTFUNC_SENDCHATTEXT:
			{
				//Convert the calling visual to a text box
				CMenuTextBox *visual = (CMenuTextBox*)m_callingVisual;
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				//Make sure we found the items were looking for
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pEntity)
					break;

				CString newString;
				CSimpleString::XCHAR chFirstChar = visual->m_fullText.GetAt(0); // Check first char for escape character 
				// If this string is not escaped, it's just a regular chat message
				if(chFirstChar != ':' && chFirstChar != '/')
				{
					if(strcmp(pEntity->m_lastTextArea, "/C") == 0)
						newString = "/C " + visual->m_fullText;
					else if(strcmp(pEntity->m_lastTextArea, "/GROUP") == 0)
						newString = "/GROUP " + visual->m_fullText;
					else if(strcmp(pEntity->m_lastTextArea, "/WHISPER") == 0)
						newString = "/WHISPER " + visual->m_fullText;
					else
						newString = visual->m_fullText;
				}
				else
					newString = visual->m_fullText;

				//Send the chat text(the first line of text)
				globalStatus->SendChatString(newString);

				//Clear out the string
				visual->m_fullText.Empty();
				visual->WrapLine();

				//Make it rebuild the mesh
				if(visual->m_linesOfText[0].m_textMesh)
				{
					visual->m_linesOfText[0].m_textMesh->SafeDelete();
					delete visual->m_linesOfText[0].m_textMesh;
					visual->m_linesOfText[0].m_textMesh = 0;
				}

				//Kill the focus on the text box
				visual->m_isFocused  = false;
				visual->m_showCursor = false;

				break;
			}
			case MENU_SCRIPTFUNC_SENDSERVERCOMMAND:
			{
				//Make sure this is an enabled client and has a command
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe() || curLine->GetIntByParam(0) < 1)
					break;

				//Determine what type of message to send based on params
				CString serverCommand;
				switch(curLine->GetIntByParam(0))
				{
					case 54:
					{
						//Send Request for Store Inv
						serverCommand = ":BUY";
						break;
					}
				}
				
				if(!serverCommand)
					break;

				//See if we need to add a queue entry (have a reference)
				if(curLine->GetIntByParam(1) > 0)
				{
					//Create Visual Reference
					CMenuListBox* visualReference = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);

					//Confirm VisualReference and entPosition
					if(!visualReference)
						break;

					//Look for existing attrib queue entries
					bool existingQueue = false;
					POSITION posCurVisual = globalStatus->m_attribQueue->GetHeadPosition();
					while(posCurVisual != NULL)
					{
						CAttribQueueEntry *entry = (CAttribQueueEntry*)globalStatus->m_attribQueue->GetNext(posCurVisual);
						if(entry->m_requestedAttrib == curLine->GetIntByParam(0))
							existingQueue = true;
					}
					
					//Create Attrib Queue Entry if Needed
					if (!existingQueue)
					{
						CAttribQueueEntry *curEntry = new CAttribQueueEntry();
						curEntry->m_requestedAttrib = curLine->GetIntByParam(0);
						curEntry->m_requester = visualReference;
						globalStatus->m_attribQueue->AddTail(curEntry);
					}
				}
	
				//Create Position
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);

				if (entPosition)
				{
					CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(entPosition);
					if(ptrLoc)
						if(ptrLoc->List_controlType == 0)
							globalStatus->SendChatString(serverCommand);
				}
				break;
			}
			case MENU_SCRIPTFUNC_TELEPORT:
			{
				//Create Position
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);

				if(entPosition)
				{
					CMovementObj *ptrLoc = (CMovementObj*) globalStatus->m_entObjRuntime->GetAt(entPosition);

					if(!ptrLoc)
						break;

					//Set for large player movement
					globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,
																	60,
																	0,
																	ptrLoc->List_networkDefinedID,
																	0,
																	0,
																	NULL);

					//Set the new player's position
					ptrLoc->List_baseFrame->SetPosition2(NULL, curLine->GetFloatByParam(0), curLine->GetFloatByParam(1), curLine->GetFloatByParam(2));
					ptrLoc->List_baseFrame->GetPosition(NULL, &ptrLoc->List_movementDB->List_lastPosition);
					ptrLoc->m_collisionRaySys->InitOnSpawn(ptrLoc->List_baseFrame, ptrLoc->List_skeletalObject, &globalStatus->m_matLib);
				}

				break;
			}
			case MENU_SCRIPTFUNC_UPDATEELEMENTPROGRESS:
			{
				// Gold-style element gauge: how much protection the player's ARMED gear
				// gives on one damage channel (0 Plasma,1 Thermal,2 Hydro,3 Electric,
				// 4 Methane,5 Corrode,6 Physical,7 Ethereal). Same number the server's
				// damage handler uses (CInventoryObjList::GetArmourRating, capped at 95).
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);
				if(!visual || !posLoc)
					break;
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);
				visual->m_curPercentage = 0.0f;
				if(!ptrLoc->m_inventory)
					break;
				int channel = curLine->GetIntByParam(1);
				int rating = ptrLoc->m_inventory->GetArmourRating(channel, FALSE, -1);
				visual->m_curPercentage = (float)rating / 95.0f;
				break;
			}
			case MENU_SCRIPTFUNC_LOGIN:
			{
				// In-game login: credentials come from two TextBox visuals on the login
				// window, then connect exactly as the start-up script's "connect" step does.
				CMenuVisual *userVis = globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				CMenuVisual *passVis = globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);
				if(!userVis || !passVis || userVis->GetType() != MENU_VISTYPE_TEXTBOX || passVis->GetType() != MENU_VISTYPE_TEXTBOX)
				{
					globalStatus->UI_Log("Login(): user/password visual IDs are not TextBoxes");
					break;
				}
				CString user = ((CMenuTextBox*)userVis)->m_fullText; user.TrimLeft(); user.TrimRight();
				CString pass = ((CMenuTextBox*)passVis)->m_fullText;
				if(user.IsEmpty() || pass.IsEmpty())
				{
					globalStatus->UI_Log("Login(): username or password empty");
					break;
				}
				if(globalStatus->m_multiplayerObject)
				{
					globalStatus->m_multiplayerObject->m_userName = user;
					globalStatus->m_multiplayerObject->m_passWord = pass;
					if(globalStatus->m_systemReadout)
						globalStatus->m_systemReadout->AddLine("Connecting to " + globalStatus->m_multiplayerObject->m_IPaddress + " ...");
					globalStatus->m_multiplayerObject->EnableClient(globalStatus->m_multiplayerObject->m_IPaddress, globalStatus->m_multiplayerObject->m_currentVersion);
				}
				break;
			}
			case MENU_SCRIPTFUNC_SETSERVERADDRESS:
			{
				CMenuVisual *srvVis = globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				if(srvVis && srvVis->GetType() == MENU_VISTYPE_TEXTBOX && globalStatus->m_multiplayerObject)
				{
					CString srv = ((CMenuTextBox*)srvVis)->m_fullText; srv.TrimLeft(); srv.TrimRight();
					if(!srv.IsEmpty()) globalStatus->m_multiplayerObject->m_IPaddress = srv;
				}
				break;
			}
			case MENU_SCRIPTFUNC_OPENMENU:
			{
				globalStatus->OpenMenu(curLine->GetIntByParam(0), false);

				break;
			}
			case MENU_SCRIPTFUNC_UPDATECOMPASS:
			{
				//Compass only works with images
				CMenuImage *menuImage = (CMenuImage*)m_callingVisual;

				//Get the player entity
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);
				if(entPosition)
			    {
					CMovementObj *entity = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(entPosition);
			 
					D3DMATRIX orientateMat;
					D3DVECTOR normal;
					normal.x = 0.0f;
					normal.y = 0.0f;
					normal.z = -1.0f;
					entity->List_baseFrame->GetTransform(NULL,&orientateMat);
					normal = globalStatus->m_cabLib.UpdatePolygonVertexRotationOnly(orientateMat.m,normal);

					normal.y = 0.0f;
					normal = globalStatus->m_matLib.Normalize(normal);

					float rotationOnY = atan(fabs(normal.x) / fabs(normal.z)) * 57.3f;

					float param0 = curLine->GetFloatByParam(0);//TextureStartX
					float param1 = curLine->GetFloatByParam(1);//TextureFullWidth,
					float param2 = curLine->GetFloatByParam(2);//VisibleWidth

					
					float translatedRotation = rotationOnY;
					//If they're facing North
					if(normal.z > 0)
					{
						//If they're facing North East
						if(normal.x > 0.0f)
						   translatedRotation = rotationOnY;
						else //They're facing North West
						   translatedRotation = 360.0f - rotationOnY;
					}
					else //They're facing South
					{
						//If they're facing South East
						if(normal.x > 0.0f)
						   translatedRotation = 180.0f - rotationOnY;
						else //They're facing South West
						   translatedRotation = 180.0f + rotationOnY;
					}
    
					
					menuImage->m_texturePosX = globalStatus->m_matLib.InterpolateFloatsHiRes(param0,param1,360.0f,translatedRotation);

					//menuImage->m_texturePosX = param1;

					//Set the texture width to be the width of the mesh
					menuImage->m_textureWidth = menuImage->m_width;

					//Update the image
					menuImage->UpdateTextureCoords();
				}

				break;
			}
			case MENU_SCRIPTFUNC_UPDATEHEALTHPROGRESS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual || !posLoc)
					break;

				//Set Text to Health Ratio
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);
				visual->m_curPercentage = (float)ptrLoc->List_energy / (float)ptrLoc->List_maxEnergy;

				break;
			}
			case MENU_SCRIPTFUNC_UPDATEJETPACK:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual || !posLoc)
					break;
				
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);
				
				if(!ptrLoc->List_armedInventoryDB)
					break;

				visual->m_curPercentage = 0.0f;

				float max = 1.0f;
				float cur = 0.0f;
				BOOL noJetpack = TRUE;
				for(POSITION posLoc = ptrLoc->List_armedInventoryDB->GetHeadPosition();posLoc != NULL;)
				{
					CArmedInventoryObj * armedPtr = (CArmedInventoryObj *)ptrLoc->List_armedInventoryDB->GetNext(posLoc);
					if(armedPtr->m_functionalityAttribute == 3)
					{//jet
						/*max = armedPtr->m_miscInt;*/
						max = (float)armedPtr->m_miscInt;
						//WARNING: C4244 --> int to float

						cur = 0;//armedPtr->m_miscInt2;
						if(armedPtr->m_obfusionMiscInt)
						/*cur = *armedPtr->m_obfusionMiscInt;*/
						cur = (float)*armedPtr->m_obfusionMiscInt;
						//WARNING: C4244 --> int to float

						visual->m_curPercentage = cur / max;
						noJetpack = FALSE;
					}
				}
				if(noJetpack)
					visual->m_curPercentage = 0.0f;
				break;
			}
			case MENU_SCRIPTFUNC_UPDATERESISTPROGRESS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				int resistIndex = curLine->GetIntByParam(1);
				int value = 0;

				// No element status yet (HUD opens before the server has sent it) - show empty, don't crash
				if(!globalStatus->m_multiplayerObject->m_elementStatus)
				{
					if(visual) visual->m_curPercentage = 0.0f;
					break;
				}

				for(POSITION posLoc2 = globalStatus->m_multiplayerObject->m_elementStatus->GetHeadPosition(); posLoc2 != NULL;)
				{
					CElementStatusObj *eleObject = (CElementStatusObj *)globalStatus->m_multiplayerObject->m_elementStatus->GetNext(posLoc2);
					if(eleObject->m_elementIndex == resistIndex)
						value = (int)eleObject->m_currentValue;
				}

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual || !posLoc)
					break;

				//Set Text to Health Ratio
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);
				float fVal = /*100.0f -*/ (float)value;
				visual->m_curPercentage = (float)fVal / 100;

				break;
			}
			case MENU_SCRIPTFUNC_UPDATEWOUNDPROGRESS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual || !posLoc)
					break;

				//Set Text to Health Ratio
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);
				visual->m_curPercentage = (float)ptrLoc->List_energy / 100;

				break;
			}
			case MENU_SCRIPTFUNC_UPDATEENERGYPROGRESS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual || !posLoc)
					break;

				//Set Text to Health Ratio
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);
				visual->m_curPercentage = (float)ptrLoc->m_energyCurrent / (float)ptrLoc->m_energyMax;
				
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEAMMOPROGRESS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual)
					break;
				
				//Set Ammo Ratio
				if(globalStatus->m_multiplayerObject->m_itemGlobalMaxCharges)
					visual->m_curPercentage = (float)globalStatus->m_multiplayerObject->m_itemGlobalCurCharges / (float)globalStatus->m_multiplayerObject->m_itemGlobalMaxCharges;
				else
					visual->m_curPercentage = 0.0f;
				
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEAMMORECHARGE:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual)
					break;

				//Default the visual to 0
				visual->m_curPercentage = 0.0f;

				if(globalStatus->m_multiplayerObject->m_rechargeTimer)
				{
					float baseValue = (float)(GetTickCount() - globalStatus->m_multiplayerObject->m_rechargeTimerStart); //How many seconds have we gone
					float swappedValue = (float)globalStatus->m_multiplayerObject->m_rechargeTimer - baseValue; //convert this to how many seconds left
					if(swappedValue < 0.0f)
						swappedValue = 0.0f;
					visual->m_curPercentage = swappedValue / (float)globalStatus->m_multiplayerObject->m_rechargeTimer;
					if(!swappedValue)
					{
						globalStatus->m_multiplayerObject->m_rechargeTimer = 0;
						globalStatus->m_multiplayerObject->m_rechargeTimerStart = 0;
					}
				}

				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYCRYSTALS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				//Create Visual Reference and Set Text
				CMenuText* visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				int crystalCount = 0;
				//Determine Crystal Type
				if(curLine->GetIntByParam(1) == 1) //Blue
					crystalCount = globalStatus->m_multiplayerObject->m_cashCount % 100;									
				else if(curLine->GetIntByParam(1) == 2) //Green
					crystalCount = ((globalStatus->m_multiplayerObject->m_cashCount - (globalStatus->m_multiplayerObject->m_cashCount % 100)) % 10000) / 100;		
				else if(curLine->GetIntByParam(1) == 3) //Red
					crystalCount = ((globalStatus->m_multiplayerObject->m_cashCount - ((globalStatus->m_multiplayerObject->m_cashCount - (globalStatus->m_multiplayerObject->m_cashCount % 100)) % 10000)) % 10000000) / 10000;
				else if(curLine->GetIntByParam(1) == 4) //Yellow - Not implemented yet because it steps outside int bounds.
					crystalCount = 0;
				else if(curLine->GetIntByParam(1) == 5) //White - Not implemented yet because it steps outside int bounds.
					crystalCount = 0;

				m_tempRegString = globalStatus->m_cabLib.ConvToString(crystalCount);
				if(visual)//valid
				if(visual->m_text != m_tempRegString)
				{//recreate if changed
                    visual->m_text = m_tempRegString;
					//Delete the Text Mesh if Exists so it will re-create
					if(visual->m_textMesh)
					{
						visual->m_textMesh->SafeDelete();
						delete visual->m_textMesh;
						visual->m_textMesh = NULL;
					}
				}//end recreate if changed
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEPOWERUPSTATPROGRESS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual)
					break;

				//Update Timer values
				long currentTime = GetTickCount();
				long lastTime = currentTime - globalStatus->m_multiplayerObject->m_StatTimeStamp;
				globalStatus->m_multiplayerObject->m_StatTimeStamp = currentTime;
				
				//Set the progression bar
				if(globalStatus->m_multiplayerObject->m_CurBonusTimeStat > 0)
				{
					visual->m_curPercentage = (float)globalStatus->m_multiplayerObject->m_CurBonusTimeStat / (float)globalStatus->m_multiplayerObject->m_MaxBonusTimeStat;
					globalStatus->m_multiplayerObject->m_CurBonusTimeStat -= lastTime;
				}
				else
				{
					globalStatus->m_multiplayerObject->m_CurBonusTimeStat = 0;
					globalStatus->m_multiplayerObject->m_MaxBonusTimeStat = 0;
					globalStatus->m_multiplayerObject->m_StatIcon = -1;
					globalStatus->m_multiplayerObject->m_StatTimeStamp = 0;
					globalStatus->m_runtimeMenuDatabase->CloseByID(207, &globalStatus->m_showMenuCursor);
				}
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEPOWERUPRESISTPROGRESS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual)
					break;

				//Update Timer values
				long currentTime = GetTickCount();
				long lastTime = currentTime - globalStatus->m_multiplayerObject->m_ResistTimeStamp;
				globalStatus->m_multiplayerObject->m_ResistTimeStamp = currentTime;
				
				//Set the progression bar
				if(globalStatus->m_multiplayerObject->m_CurBonusTimeResist > 0)
				{
					visual->m_curPercentage = (float)globalStatus->m_multiplayerObject->m_CurBonusTimeResist / (float)globalStatus->m_multiplayerObject->m_MaxBonusTimeResist;
					globalStatus->m_multiplayerObject->m_CurBonusTimeResist -= lastTime;
				}
				else
				{
					globalStatus->m_multiplayerObject->m_CurBonusTimeResist = 0;
					globalStatus->m_multiplayerObject->m_MaxBonusTimeResist = 0;
					globalStatus->m_multiplayerObject->m_ResistIcon = -1;
					globalStatus->m_multiplayerObject->m_ResistTimeStamp = 0;
					globalStatus->m_runtimeMenuDatabase->CloseByID(203, &globalStatus->m_showMenuCursor);
				}
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEPOWERUPDAMAGEPROGRESS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual)
					break;

				//Set the progression Bar
				visual->m_curPercentage = (float)globalStatus->m_multiplayerObject->m_CurBonusTimeDamage / (float)globalStatus->m_multiplayerObject->m_MaxBonusTimeDamage;

				if(globalStatus->m_multiplayerObject->m_CurBonusTimeDamage == 0)
				{
					globalStatus->m_multiplayerObject->m_MaxBonusTimeDamage = 0;
					globalStatus->m_multiplayerObject->m_DamageIcon = -1;
					globalStatus->m_runtimeMenuDatabase->CloseByID(204, &globalStatus->m_showMenuCursor);
				}
				break;
			}
			case MENU_SCRIPTFUNC_INVERTMOUSE:
			{				
				//Get the control position
				POSITION pos = globalStatus->m_controlsList->FindIndex(0);
				if(pos)
				{
					//Retrieve object and visual
					ControlDBObj *cObj = (ControlDBObj *)globalStatus->m_controlsList->GetAt(pos);
					CMenuToggle *toggle = (CMenuToggle *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
					
					//Change mouse invert variable and the toggle control
					if(cObj->m_invertMouse)
					{
						cObj->m_invertMouse = FALSE;
						toggle->m_isSet = FALSE;
						toggle->m_pDisplayText->SetText("False");
					}
					else
					{
						cObj->m_invertMouse = TRUE;
						toggle->m_isSet = TRUE;
						toggle->m_pDisplayText->SetText("True");
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_TOGGLEMULTI:
			{
				//Get the control position
				POSITION pos = globalStatus->m_controlsList->FindIndex(0);
				if(pos)
				{
					//Retrieve object and visual
					ControlDBObj *cObj = (ControlDBObj *)globalStatus->m_controlsList->GetAt(pos);
					CMenuToggle *toggle = (CMenuToggle *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
					
					//Change mouse invert variable and the toggle control
					if(cObj->m_allowMultiBind)
					{
						cObj->m_allowMultiBind = FALSE;
						toggle->m_isSet = FALSE;
						toggle->m_pDisplayText->SetText("False");

						static char strCurrentValue[4];
						BOOL bResult = FALSE;

						// Retrieve object and visuals
						int iListBoxID = 282;
						int iToggleID = 290;
						int iTextID = 292;
						int iMusicVolumeID = 84;
						int iMultiID = 1459;
						CMenuListBox *pListBox = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iListBoxID, true);
						CMenuToggle *pToggle = (CMenuToggle*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iToggleID, true);
						CMenuText *pSensitivityValue = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iTextID, true);
						CMenuText *pMusicVolumeText = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMusicVolumeID, true);
						CMenuToggle *pMulti = (CMenuToggle*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMultiID, true);

						// Load the movement object
						POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
						if(!pos) break;
						CMovementObj *pMovement = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(pos);

						// Create filename for writing
						CString strFileName = KEY_CONFIG_FILE;
						strFileName += ".";
						strFileName += globalStatus->m_cabLib.ConvToString(pMovement->m_uniqueId);

						// Init key configs from disk or default
						bResult = globalStatus->LoadControlCfgList(strFileName, pListBox);	
						
						if(bResult)
						{
							// Get the ControlDBObj with key and mouse settings
							POSITION pos = globalStatus->m_controlsList->FindIndex(0);
							if(!pos) break;
							ControlDBObj *pControlDB = (ControlDBObj*)globalStatus->m_controlsList->GetAt(pos);

							float fStepAmount = ((MAX_MOUSE_SENSITIVITY - MIN_MOUSE_SENSITIVITY) / MAX_MOUSE_SENSITIVITY_STEPS);
							float fCurrentValue = (pControlDB->m_mouseSensitivity) / fStepAmount;
							int iCurrentValue = static_cast<int>(fCurrentValue);

							// Clamp the display values for mouse sensitivity
							if (pSensitivityValue)
							{
								if(iCurrentValue <= 0) 
									iCurrentValue++;
								else if(iCurrentValue >= MAX_MOUSE_SENSITIVITY_STEPS) 
									iCurrentValue = (int)MAX_MOUSE_SENSITIVITY_STEPS;
								itoa(iCurrentValue, strCurrentValue, 10);
								pSensitivityValue->SetText(strCurrentValue);
							}

							// Update the display value for Invert Mouse
							if (pToggle)
							{
								pToggle->m_isSet = pControlDB->m_invertMouse;
								if (pToggle->m_isSet)
									pToggle->m_pDisplayText->SetText("True");
								else
									pToggle->m_pDisplayText->SetText("False");
							}

							// Update the display value for Multibind
							if(pMulti)
							{
								pMulti->m_isSet = pControlDB->m_allowMultiBind;
								if (pMulti->m_isSet)
									pMulti->m_pDisplayText->SetText("True");
								else
									pMulti->m_pDisplayText->SetText("False");
							}

							// Update the display and value for Music Volume
							iCurrentValue = (int)pControlDB->m_musicVolume;
							if (pMusicVolumeText)
								pMusicVolumeText->SetText(globalStatus->m_cabLib.ConvToString(iCurrentValue));

							// SET the music volume
							if (globalStatus->m_soundTrackList)
								globalStatus->m_soundTrackList->SetVolume(iCurrentValue * VOLUME_SCALED_MAX);
						}//*/
					}
					else
					{
						cObj->m_allowMultiBind = TRUE;
						toggle->m_isSet = TRUE;
						toggle->m_pDisplayText->SetText("True");
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_REQUESTCASH:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				//Determine If Player Needs new Update
				if(GetTickCount() - globalStatus->m_lastCashUpdate < globalStatus->m_cashUpdateFrequency)
					break;

				//Create and Confirm posLoc
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);
				if(!posLoc)
					break;

				//Get a pointer to the 
				CMovementObj * ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(posLoc);

				//Confirm NetworkID
				if(ptrLoc->List_networkDefinedID <= 0)
					break;

				//Send Attrib Message for Player Cash
				globalStatus->m_lastCashUpdate = GetTickCount(); 
				globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,
																	61,
																	0,//Requested Attrib ID
																	ptrLoc->List_networkDefinedID,
																	0,
																	0,
																	NULL);//Request Character Cash

				break;
			}
			case MENU_SCRIPTFUNC_TOGGLEMENU:
			{
				//Attempt to Close Menu
				bool closedMenu = globalStatus->m_runtimeMenuDatabase->CloseByID(curLine->GetIntByParam(0), &globalStatus->m_showMenuCursor);
				
				//Open Menu if Wasn't Just Closed
				if(!closedMenu)
					globalStatus->OpenMenu(curLine->GetIntByParam(0), false);

				break;
			}
			case MENU_SCRIPTFUNC_UPDATETEXTINT:
			{
				//Get Calling Visual
				CMenuText *visual = (CMenuText*)m_callingVisual;
				if(!visual)
					break;
				
				//See if the string has changed
				m_tempRegString = globalStatus->m_cabLib.ConvToString(curLine->GetIntByParam(0));
				if(visual->m_text != m_tempRegString)
				{
					//Copy over the new string
					visual->m_text = m_tempRegString;
					
					//Delete the Text Mesh if Exists so it will re-create
					if(visual->m_textMesh)
					{
						visual->m_textMesh->SafeDelete();
						delete visual->m_textMesh;
						visual->m_textMesh = NULL;
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_UPDATETEXTSTRING:
			{
				//Get Calling Visual
				CMenuText *visual = (CMenuText*)m_callingVisual;
				if(!visual)
					break;
				
				//See if the string has changed
				if(visual->m_text != *curLine->m_strParams[0])
				{
					//Copy over the new string
					visual->m_text = m_tempRegString;
					
					//Delete the Text Mesh if Exists so it will re-create
					if(visual->m_textMesh)
					{
						visual->m_textMesh->SafeDelete();
						delete visual->m_textMesh;
						visual->m_textMesh = NULL;
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_LOADQUICKSLOTSCONFIG:
			{
				/*/ Store the client's entity pointer for later use
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos) break;

				// Get player entity
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pEntity) break;

				// Send Attrib Message for Inventory List
				globalStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
					DPID_SERVERPLAYER,
					19,
					51,//Requested Attrib ID
					pEntity->List_networkDefinedID,
					0,
					3, //Filter Type 0 = none, 1 = skills only, 2 = spells only, 3 = no skills / spells
					NULL);//request character inventory//*/
				break;
			}
			/*case MENU_SCRIPTFUNC_SHOWHOVER:
			{
				//Open the Hover Menu and Get Visual
				globalStatus->OpenMenu(curLine->GetIntByParam(0), false);
				CMenuWindow *menuVisual = (CMenuWindow*) globalStatus->m_menuDatabase->GetVisualByID(curLine->GetIntByParam(0), false);

				//Get The Visual Of the Text
				CMenuText *visual = (CMenuText *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);

				//Set the Menu's Position Based on Mouse
				menuVisual->m_posX = globalStatus->m_mouseCursors[globalStatus->m_currentCursor]->m_posX + globalStatus->m_mouseCursors[globalStatus->m_currentCursor]->m_width + 5;
				menuVisual->m_posY = globalStatus->m_mouseCursors[globalStatus->m_currentCursor]->m_posY + globalStatus->m_mouseCursors[globalStatus->m_currentCursor]->m_height + 5;

				//Set The Text
				visual->m_text = *curLine->m_strParams[0];

				//Delete the Text Mesh if Exists so it will re-create
				if(visual->m_textMesh)
				{
					visual->m_textMesh->SafeDelete();
					delete visual->m_textMesh;
					visual->m_textMesh = NULL;
				}

				//Set The Menu's Width Based on Text Size
				menuVisual->m_width = visual->m_textWidth + 10;

				break;
			}*/
			case MENU_SCRIPTFUNC_ACCEPTSELECTEDQUEST:
			{
				//obsolete
				/*//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Get the position of the local player's movement object(safety check)
				POSITION posEntity = globalStatus->m_entObjRuntime->FindIndex(0);
				if(!posEntity)
					break;

				//Get a pointer to the local player's movement object
				CMovementObj *ptrEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(posEntity);

				//Get a pointer to the specified Quest list box
				CMenuListBox *questList = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);
				if(!questList->m_currentSelection)
					break;

				//Send the selected quest to the server
				globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,
																	32,
																	questList->m_currentSelection->m_index,
																	ptrEntity->List_networkDefinedID,
																	questList->m_currentSelection->m_miscInt1,
																	0,
																	NULL);

				//Clear The Quest List
				while(questList->m_pFirstItem != NULL)
					questList->RemoveItem(0);

				//Close the specified menu
				globalStatus->m_runtimeMenuDatabase->CloseByID(curLine->GetIntByParam(0), &globalStatus->m_showMenuCursor);*/

				break;
			}
			case MENU_SCRIPTFUNC_SETTOGGLESTATE:
			{
				//Get the toggle whose state we're setting
				CMenuToggle *toggle = (CMenuToggle*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				//Set the toggle to the specified state
				toggle->m_isSet = (bool)curLine->GetIntByParam(1);

				break;
			}
			case MENU_SCRIPTFUNC_SETVISUALVISIBLESTATE:
			{
				//Get the visual who's visible state we're setting
				CMenuVisual *visual = globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				//Set the visual's visible state to the specified value
				visual->m_visible = (bool)curLine->GetIntByParam(1);

				break;
			}
			case MENU_SCRIPTFUNC_ACCEPTSQUADINVITE:
			{
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);
				if(globalStatus->m_multiplayerObject->GetEnabledClient_safe())
				{
					if(entPosition)
					{
						CMovementObj *ptrLoc = (CMovementObj *) globalStatus->m_entObjRuntime->GetAt(entPosition);
						if(ptrLoc->List_controlType == 0)
						{
							COMMAND_DATA dataPacket;
							int messageSize = sizeof(COMMAND_DATA);
							dataPacket.m_catagorie = 18;
							dataPacket.m_data      = 2;
							dataPacket.m_miscInt   = ptrLoc->List_networkDefinedID;
							dataPacket.m_networkID = globalStatus->m_multiplayerObject->m_potentialGroupJoiner;

							globalStatus->m_multiplayerObject->SendData((BYTE *)&dataPacket,
								messageSize,
								DPNSEND_GUARANTEED,
								NULL,
								globalStatus->m_multiplayerObject->m_packetTimeout);
						}
					}

					// Open group status window
					globalStatus->OpenMenu(ID_SQUAD_STATUS_MENU, false);
				}				
				break;
			}
			case MENU_SCRIPTFUNC_DECLINESQUADINVITE:
			{
				if(globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					globalStatus->m_multiplayerObject->m_potentialGroupJoiner = -1;
				break;
			}
			case MENU_SCRIPTFUNC_INITRADAR:
			{
				//If we've already initialized the radar, we don't need to do it again
				if(globalStatus->m_radar.m_menu || !m_callingMenu || !m_callingMenu->m_childList)
					break;

				//Get pointers to all needed objects
				globalStatus->m_radar.m_menu		= m_callingMenu;
				globalStatus->m_radar.m_region		= (CMenuText*) m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(0), true);
				globalStatus->m_radar.m_map			= (CMenuImage*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(1), true);
				globalStatus->m_radar.m_compass		= (CMenuImage*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(2), true);
				globalStatus->m_radar.m_coordinateX = (CMenuText*) m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(3), true);
				globalStatus->m_radar.m_coordinateY = (CMenuText*) m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(4), true);
				globalStatus->m_radar.m_zoomRadius	= (CMenuText*) m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(5), true);
				globalStatus->m_radar.m_groupDot	= (CMenuImage*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(6), true);
				globalStatus->m_radar.m_playerDot	= (CMenuImage*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(7), true);
				globalStatus->m_radar.m_npcDot		= (CMenuImage*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(8), true);
				globalStatus->m_radar.m_mobDot		= (CMenuImage*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(9), true);

				//Get the positon of the player dot
				globalStatus->m_radar.m_dotStart = globalStatus->m_radar.m_menu->m_childList->GetHeadPosition();

				//Temperary - TODO: Move to map settings somewhere
				globalStatus->m_radar.m_mapScale = (float)curLine->GetIntByParam(10);
				globalStatus->m_radar.m_mapScale /= 100000;

				globalStatus->m_radar.m_curZoom	 = (float)curLine->GetIntByParam(11);

				if(globalStatus->m_radar.m_map)
				{
					//Update the map's texture width and height(actually performs the map zooming)[6.5616 = radius to diameter(2) * meters to feet(3.2808)]
					globalStatus->m_radar.m_map->m_textureWidth	 = globalStatus->m_radar.m_curZoom * 6.5616f * globalStatus->m_radar.m_mapScale;
					globalStatus->m_radar.m_map->m_textureHeight = globalStatus->m_radar.m_curZoom * 6.5616f * globalStatus->m_radar.m_mapScale;

					//Update the zoom scale
					if(globalStatus->m_radar.m_map->m_textureWidth > 0.0001f)
						globalStatus->m_radar.m_zoomScale = globalStatus->m_radar.m_map->m_width / globalStatus->m_radar.m_map->m_textureWidth;
					else
						globalStatus->m_radar.m_zoomScale = 1.0f;
				}

				if(globalStatus->m_radar.m_zoomRadius)
				{
					//Update the zoom radius text
					globalStatus->m_radar.m_zoomRadius->m_text	 = itoa((int)globalStatus->m_radar.m_curZoom, buffer, 10);
					globalStatus->m_radar.m_zoomRadius->m_text	+= "m";
				}

				break;
			}
			case MENU_SCRIPTFUNC_UPDATERADAR:
			{
				if(!globalStatus->m_radar.m_menu || !globalStatus->m_radar.m_menu->m_childList ||
				   !globalStatus->m_entObjRuntime || globalStatus->m_entObjRuntime->IsEmpty())
					break;

				//Get the local player's movement object
				CMovementObj *ptrPlayer = (CMovementObj*)globalStatus->m_entObjRuntime->GetHead();
				if(!ptrPlayer || !ptrPlayer->List_baseFrame)
					break;

				//Calculate rotation for map, compass, and dots
				D3DVECTOR direction, up;
				ptrPlayer->List_baseFrame->GetOrientation(&direction, &up);
				float rotation = atan2f(direction.x, direction.z);

				//Update region label if available
				if(globalStatus->m_radar.m_region && !globalStatus->m_currentWorkingFile.IsEmpty())
				{
					CString zoneName = globalStatus->m_currentWorkingFile;
					int dotIdx = zoneName.ReverseFind('.');
					if(dotIdx > 0)
						zoneName = zoneName.Left(dotIdx);
					if(globalStatus->m_radar.m_region->m_text != zoneName)
					{
						globalStatus->m_radar.m_region->m_text = zoneName;
						if(globalStatus->m_radar.m_region->m_textMesh)
						{
							globalStatus->m_radar.m_region->m_textMesh->SafeDelete();
							delete globalStatus->m_radar.m_region->m_textMesh;
							globalStatus->m_radar.m_region->m_textMesh = NULL;
						}
					}
				}

				//Rotate and Translate map and compass
				if(globalStatus->m_radar.m_map)
				{
					globalStatus->m_radar.m_map->m_texturePosX = -1 * ptrPlayer->List_lastPosition.x * globalStatus->m_radar.m_mapScale + 358.0f; //358 is a temperary offset
					globalStatus->m_radar.m_map->m_texturePosY = ptrPlayer->List_lastPosition.z * globalStatus->m_radar.m_mapScale + 336.0f; //336 is a temperary offset
					globalStatus->m_radar.m_map->RotateTexture(rotation);
				}
				if(globalStatus->m_radar.m_compass)
					globalStatus->m_radar.m_compass->RotateTexture(rotation);

				//Update dots for all entities
				POSITION posEnt = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(posEnt)
					globalStatus->m_entObjRuntime->GetNext(posEnt); //The first one is the local player, get the second one
				POSITION posDot = globalStatus->m_radar.m_dotStart;
				if(posDot)
					globalStatus->m_radar.m_menu->m_childList->GetPrev(posDot);
				CMovementObj *ptrEnt;
				CMenuImage *ptrDot;
				float distance, diffX, diffY, entRotation;
				while(posEnt && globalStatus->m_radar.m_playerDot && globalStatus->m_radar.m_npcDot && globalStatus->m_radar.m_mobDot && globalStatus->m_radar.m_map)
				{
					ptrEnt = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(posEnt);
					if(!ptrEnt)
						continue;

					//Get the directional vector for this entity
					diffX = ptrPlayer->List_lastPosition.x - ptrEnt->List_lastPosition.x;
					diffY = ptrPlayer->List_lastPosition.z - ptrEnt->List_lastPosition.z;

					//Calculate the rotation to this entity
					entRotation = atan2f(diffY, diffX);

					//Calculate the distance to this entity
					distance = sqrtf(diffX * diffX + diffY * diffY);

					//Get a dot for this entity
					if(posDot)
					{
						//Get the dot
						ptrDot = (CMenuImage*)globalStatus->m_radar.m_menu->m_childList->GetPrev(posDot);

						//If this is a player entity
						if(ptrEnt->List_controlType == 0)
						{
							if(ptrDot->m_visualID != globalStatus->m_radar.m_playerDot->m_visualID)
								globalStatus->m_radar.m_playerDot->Copy(ptrDot, globalStatus->m_dxRenderer->m_pd3dDevice);
						}
						//Otherwise this is an AI entity
						else
						{
							//If this is an interactable AI
							if(ptrEnt->List_controlType == 35)// || ptrEnt->List_energy <= 0)
							{
								if(ptrDot->m_visualID != globalStatus->m_radar.m_npcDot->m_visualID)
									globalStatus->m_radar.m_npcDot->Copy(ptrDot, globalStatus->m_dxRenderer->m_pd3dDevice);
							}
							//Otherwise this is an enemy AI
							else
							{
								if(ptrDot->m_visualID != globalStatus->m_radar.m_mobDot->m_visualID)
									globalStatus->m_radar.m_mobDot->Copy(ptrDot, globalStatus->m_dxRenderer->m_pd3dDevice);
							}
						}
					}
					else
					{
						//If this is a player entity
						if(ptrEnt->List_controlType == 0)
						{
							globalStatus->m_radar.m_playerDot->Clone((CMenuVisual**)&ptrDot, globalStatus->m_dxRenderer->m_pd3dDevice);
						}
						//Otherwise this is an AI entity
						else
						{
							//If this is an interactable AI
							if(ptrEnt->List_controlType == 35)// || ptrEnt->List_energy <= 0)
								globalStatus->m_radar.m_npcDot->Clone((CMenuVisual**)&ptrDot, globalStatus->m_dxRenderer->m_pd3dDevice);
							//Otherwise this is an enemy AI
							else
								globalStatus->m_radar.m_mobDot->Clone((CMenuVisual**)&ptrDot, globalStatus->m_dxRenderer->m_pd3dDevice);
						}

						//Add the dot to the beginning of the list because menu objects render in reverse order
						globalStatus->m_radar.m_menu->m_childList->AddHead(ptrDot);
					}

					//Set the coordinates for this entity's dot
					ptrDot->m_posX = globalStatus->m_radar.m_playerDot->m_posX + distance * cosf(entRotation + rotation) * globalStatus->m_radar.m_mapScale * globalStatus->m_radar.m_zoomScale;
					ptrDot->m_posY = globalStatus->m_radar.m_playerDot->m_posY + distance * sinf(entRotation + rotation) * globalStatus->m_radar.m_mapScale * globalStatus->m_radar.m_zoomScale;

					//If it's outside our mini-map's render square, hide it
					if(	ptrDot->m_posX < globalStatus->m_radar.m_map->m_posX ||
						ptrDot->m_posY < globalStatus->m_radar.m_map->m_posY ||
						ptrDot->m_posX + ptrDot->m_width > globalStatus->m_radar.m_map->m_posX + globalStatus->m_radar.m_map->m_width ||
						ptrDot->m_posY + ptrDot->m_height > globalStatus->m_radar.m_map->m_posY + globalStatus->m_radar.m_map->m_height)
					{
						ptrDot->m_visible = false;
					}
					//Otherwise, show it
					else
						ptrDot->m_visible = true;
				}

				//Hide excess dots
				while(posDot)
				{
					ptrDot = (CMenuImage*)globalStatus->m_radar.m_menu->m_childList->GetPrev(posDot);
					if(ptrDot)
						ptrDot->m_visible = false;
				}

				//Update the player's X and Y Coordinates
				if(globalStatus->m_radar.m_coordinateX)
				{
					globalStatus->m_radar.m_coordinateX->m_text = itoa((int)ptrPlayer->List_lastPosition.x, buffer, 10);
					if(globalStatus->m_radar.m_coordinateX->m_textMesh)
					{
						globalStatus->m_radar.m_coordinateX->m_textMesh->SafeDelete();
						delete globalStatus->m_radar.m_coordinateX->m_textMesh;
						globalStatus->m_radar.m_coordinateX->m_textMesh = NULL;
					}
				}
				if(globalStatus->m_radar.m_coordinateY)
				{
					globalStatus->m_radar.m_coordinateY->m_text = itoa((int)ptrPlayer->List_lastPosition.z, buffer, 10);
					if(globalStatus->m_radar.m_coordinateY->m_textMesh)
					{
						globalStatus->m_radar.m_coordinateY->m_textMesh->SafeDelete();
						delete globalStatus->m_radar.m_coordinateY->m_textMesh;
						globalStatus->m_radar.m_coordinateY->m_textMesh = NULL;
					}
				}

				break;
			}
			case MENU_SCRIPTFUNC_ZOOMRADARIN:
			{
				if(globalStatus->m_radar.m_curZoom > 25.0f)
				{
					globalStatus->m_radar.m_curZoom	 -= 25.0f;

					if(globalStatus->m_radar.m_map)
					{
						//Update the map's texture width and height(actually performs the map zooming)[6.5616 = radius to diameter(2) * meters to feet(3.2808)]
						globalStatus->m_radar.m_map->m_textureWidth	 = globalStatus->m_radar.m_curZoom * 6.5616f * globalStatus->m_radar.m_mapScale;
						globalStatus->m_radar.m_map->m_textureHeight = globalStatus->m_radar.m_curZoom * 6.5616f * globalStatus->m_radar.m_mapScale;

						//Update the zoom scale
						if(globalStatus->m_radar.m_map->m_textureWidth > 0.0001f)
							globalStatus->m_radar.m_zoomScale = globalStatus->m_radar.m_map->m_width / globalStatus->m_radar.m_map->m_textureWidth;
					}

					if(globalStatus->m_radar.m_zoomRadius)
					{
						//Update the zoom radius text
						globalStatus->m_radar.m_zoomRadius->m_text	 = itoa((int)globalStatus->m_radar.m_curZoom, buffer, 10);
						globalStatus->m_radar.m_zoomRadius->m_text	+= "m";

						//Delete the Text Mesh if Exists so it will be updated
						if(globalStatus->m_radar.m_zoomRadius->m_textMesh)
						{
							globalStatus->m_radar.m_zoomRadius->m_textMesh->SafeDelete();
							delete globalStatus->m_radar.m_zoomRadius->m_textMesh;
							globalStatus->m_radar.m_zoomRadius->m_textMesh = NULL;
						}
					}
				}

				break;
			}
			case MENU_SCRIPTFUNC_ZOOMRADAROUT:
			{
				if(globalStatus->m_radar.m_curZoom < 150.0f)
				{
					globalStatus->m_radar.m_curZoom += 25.0f;

					if(globalStatus->m_radar.m_map)
					{
						//Update the map's texture width and height(actually performs the map zooming)[6.5616 = radius to diameter(2) * meters to feet(3.2808)]
						globalStatus->m_radar.m_map->m_textureWidth	 = globalStatus->m_radar.m_curZoom * 6.5616f * globalStatus->m_radar.m_mapScale;
						globalStatus->m_radar.m_map->m_textureHeight = globalStatus->m_radar.m_curZoom * 6.5616f * globalStatus->m_radar.m_mapScale;

						//Update the zoom scale
						if(globalStatus->m_radar.m_map->m_textureWidth > 0.0001f)
							globalStatus->m_radar.m_zoomScale = globalStatus->m_radar.m_map->m_width / globalStatus->m_radar.m_map->m_textureWidth;
					}

					if(globalStatus->m_radar.m_zoomRadius)
					{
						//Update the zoom radius text
						globalStatus->m_radar.m_zoomRadius->m_text	 = itoa((int)globalStatus->m_radar.m_curZoom, buffer, 10);
						globalStatus->m_radar.m_zoomRadius->m_text	+= "m";

						//Delete the Text Mesh if Exists so it will be updated
						if(globalStatus->m_radar.m_zoomRadius->m_textMesh)
						{
							globalStatus->m_radar.m_zoomRadius->m_textMesh->SafeDelete();
							delete globalStatus->m_radar.m_zoomRadius->m_textMesh;
							globalStatus->m_radar.m_zoomRadius->m_textMesh = NULL;
						}
					}
				}

				break;
			}
			case MENU_SCRIPTFUNC_REQUESTSKILLTITLES:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create an attribute queue entry for this request
				CAttribQueueEntry *curEntry = new CAttribQueueEntry();
				curEntry->m_requestedAttrib = 43;
				curEntry->m_miscInt1 = curLine->GetIntByParam(0);
				globalStatus->m_attribQueue->AddTail(curEntry);

				//Find the player object so we can send the message
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);
				if(entPosition)
				{
					CMovementObj *ptrLoc = (CMovementObj *)globalStatus->m_entObjRuntime->GetAt(entPosition);
					globalStatus->m_netGameLogicObject->SendCommandMsg(14, 0, ptrLoc->List_networkDefinedID);
				}

				break;
			}
			case MENU_SCRIPTFUNC_SENDSELECTEDSKILLTITLE:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Get the position of the local player's movement object(safety check)
				POSITION posEntity = globalStatus->m_entObjRuntime->FindIndex(0);
				if(!posEntity)
					break;

				//Get a pointer to the local player's movement object
				CMovementObj *ptrEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(posEntity);

				//Get a pointer to the specified Quest list box
				CMenuListBox *skillTitleList = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				if(!skillTitleList->m_currentSelection)
					break;

				//Send the selected skill title to the server
				globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,
																	62,
																	skillTitleList->m_currentSelection->m_miscInt1,
																	ptrEntity->List_networkDefinedID,
																	0,
																	0,
																	NULL);

				break;
			}
			case MENU_SCRIPTFUNC_RENDERMAINCHARACTER:
			{

                 CMenuImage *visual = (CMenuImage*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
                 if(!visual)
				 {
                     globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RENDERMAINCHARACTER Cannot find image visual ID:"+curLine->GetStrByParam(0));
                     break;
				 }
				 if(!visual->m_optionalDynTexture)
				 {//create render target is NULL
                     HRESULT err = globalStatus->m_dxRenderer->m_pd3dDevice->CreateTexture( curLine->GetIntByParam(4),
																							curLine->GetIntByParam(5),
																							1,
																							D3DUSAGE_RENDERTARGET,
																							globalStatus->m_dxRenderer->m_renderTargetTextFormat,
																							D3DPOOL_DEFAULT,
																							&visual->m_optionalDynTexture,
																							NULL );
					 if(err != S_OK)
					 {
                        globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RENDERMAINCHARACTER Cannot create Render Target");
					    break;
					 }
				 }//end create render target is NULL
			     if(!visual->m_surfaceController)
				 {	
				   if(visual->m_optionalDynTexture->GetSurfaceLevel(0,&visual->m_surfaceController)!=S_OK)
				   {
					  globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RENDERMAINCHARACTER Cannot create Render Surface");
					  break;
				   }
				   //init to color
				 }

				 D3DCOLOR backGround = D3DCOLOR_ARGB(0,curLine->GetIntByParam(6),curLine->GetIntByParam(7),curLine->GetIntByParam(8)); // alpha 0: BlendMode 1 images show the model solid on a clear background (additive BlendMode 2 ignores alpha)
				 
				 CMovementObj * mPtr = globalStatus->m_entObjRuntime->GetObjByIndex(0);
				 if(mPtr)
				   if(mPtr->List_controlType == 0)
					 rendermanager->RenderEntityToSurface(   mPtr,//CMovementObj * mPtr,
															 visual->m_surfaceController, 
															 curLine->GetFloatByParam(3),//float camPitch, 
															 curLine->GetFloatByParam(2),//float camYaw, 
															 curLine->GetFloatByParam(1),//float camDist,
															 curLine->GetFloatByParam(4),//fov
															 &backGround,
															 (DWORD)curLine->GetIntByParam(4),
															 (DWORD)curLine->GetIntByParam(5));
				 break;
			}
			case MENU_SCRIPTFUNC_RenderListGlidToImage:
			{

                 CMenuImage *visual = (CMenuImage*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
                 if(!visual)
				 {
                     globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RenderListGlidToImage Cannot find image visual ID:"+curLine->GetStrByParam(0));
                     break;
				 }
                 CMenuListBox *targetList = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);
                 if(!targetList)
				 {
                     globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RenderListGlidToImage Cannot find glid list:"+curLine->GetStrByParam(1));
                     break;
				 }

				 if(!visual->m_optionalDynTexture)
				 {//create render target is NULL
                     HRESULT err = globalStatus->m_dxRenderer->m_pd3dDevice->CreateTexture( curLine->GetIntByParam(4),
																							curLine->GetIntByParam(5),
																							1,
																							D3DUSAGE_RENDERTARGET,
																							globalStatus->m_dxRenderer->m_renderTargetTextFormat,
																							D3DPOOL_DEFAULT,
																							&visual->m_optionalDynTexture,
																							NULL );
					 if(err != S_OK)
					 {
                        globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RenderListGlidToImage Cannot create Render Target");
					    break;
					 }
				 }//end create render target is NULL
			     if(!visual->m_surfaceController)
				 {	
				   if(visual->m_optionalDynTexture->GetSurfaceLevel(0,&visual->m_surfaceController)!=S_OK)
				   {
					  globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RenderListGlidToImage Cannot create Render Surface");
					  break;
				   }
				   //init to color
				 }

				 D3DCOLOR backGround = D3DCOLOR_ARGB(0,curLine->GetIntByParam(6),curLine->GetIntByParam(7),curLine->GetIntByParam(8)); // alpha 0: BlendMode 1 images show the model solid on a clear background (additive BlendMode 2 ignores alpha)
				 if(targetList->m_currentSelection)
				 {
				    CItemObj * iPtr = globalStatus->m_itemManager->GetItemByID(targetList->m_currentSelection->m_miscInt1);
                    //iPtr->m_attachmentRefList
					rendermanager->RenderItemToSurface(      iPtr,
															 visual->m_surfaceController, 
															 curLine->GetFloatByParam(3),//float camPitch, 
															 0.0f,//float camYaw, 
															 curLine->GetFloatByParam(2),//float camDist,
															 curLine->GetFloatByParam(4),//fov
															 &backGround,
															 (DWORD)curLine->GetIntByParam(4),
															 (DWORD)curLine->GetIntByParam(5));


			     }
				 else
				 {//clear selection
                    rendermanager->RenderItemToSurface(      NULL,
															 visual->m_surfaceController, 
															 curLine->GetFloatByParam(3),//float camPitch, 
															 0.0f,//float camYaw, 
															 curLine->GetFloatByParam(2),//float camDist,
															 curLine->GetFloatByParam(4),//fov
															 &backGround,
															 (DWORD)curLine->GetIntByParam(4),
															 (DWORD)curLine->GetIntByParam(5));
				 }//end clear selection
				 break;
				 
			}
			case MENU_SCRIPTFUNC_IfOfflineFillTestGlids:
			{
                 CMenuListBox *targetList = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
                 if(!targetList)
				 {
                     globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_IfOfflineFillTestGlids Cannot find list ID:"+curLine->GetStrByParam(0));
                     break;
				 }
				 int maxTestCount = 100;

				 int currentVis = 0;
				 for(unsigned int cLoop=0;cLoop<globalStatus->m_itemManager->m_cellCount;cLoop++)
				 {
					 for(CAxLinkObj * pos = globalStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
					 {
						 CItemObj * iPtr = (CItemObj *) globalStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
						 if(!iPtr->m_attachmentRefList)
							 continue;

						 currentVis = 0;
						 for(POSITION rPos = iPtr->m_attachmentRefList->GetHeadPosition();rPos != NULL;)
						 {
                             CAttachmentRefObj * attachmentPtr = (CAttachmentRefObj *)iPtr->m_attachmentRefList->GetNext(rPos);
							 CArmedInventoryObj *visObj = globalStatus->m_itemManager->m_visuals->GetByTypeDirectAccess(attachmentPtr->m_attachmentType);
							 if(!visObj)
								 continue;
							 if(visObj->m_thirdPersonSystem)
							    currentVis++;

						 }
						 if(currentVis == 0)
						    continue;


						 CMenuListItem *curItem	 = new CMenuListItem();
						 CMenuText *text		 = new CMenuText();
						 text->m_text			 = iPtr->m_itemName;
						 curItem->m_miscInt1     = iPtr->m_globalID;//glid
						 curItem->m_miscInt2     = 0;
						 curItem->m_miscInt3     = 0;
						 curItem->m_miscInt4     = 0;
						 curItem->m_textList.AddTail(text);
						 targetList->AddItem(curItem);

						 maxTestCount--;
					 }
					 if(maxTestCount < 0)
					    break;
				 }
				break;
			}
			case MENU_SCRIPTFUNC_INDEXDIMUP:
			{
                CMenuEntity *menuEntity = (CMenuEntity*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(0), false);
				if(!menuEntity)
				{
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_INDEXDIMUP Cannot find entity ID:"+curLine->GetStrByParam(0));
                    break;
				}

				menuEntity->IndexDimUp_PublicOnly(curLine->GetIntByParam(1));
			
				break;
			}
			case MENU_SCRIPTFUNC_INDEXDIMDOWN:
			{
                CMenuEntity *menuEntity = (CMenuEntity*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(0), false);
				if(!menuEntity)
				{
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_INDEXDIMDOWN Cannot find entity ID:"+curLine->GetStrByParam(0));
                    break;
				}

				menuEntity->IndexDimDown_PublicOnly(curLine->GetIntByParam(1));
			
				break;
			}
			case MENU_SCRIPTFUNC_EDITSTOREENTITY:
			{
				// GET references to menu visuals
				int iMenuID = curLine->GetIntByParam(0);
				CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMenuID, true));
				if (!pMenu) break;
				CMenuInventory *pInventory = globalStatus->GetActiveInventory((MENU_IDS)pMenu->m_visualID);
				if (!pInventory || !pInventory->HoverItem()) break;

				// GET a reference to the entity menu and make it visible
				CMenuImage *pEntityImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_ENTITY, true));
				CMenuEntity *menuEntity = (CMenuEntity*)pMenu->m_childList->GetVisualByID(curLine->GetIntByParam(1), false);
				if (!menuEntity || !pEntityImage)
				{
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_INDEXDIMDOWN Cannot find entity ID:"+curLine->GetStrByParam(1));
                    break;
				}

				// SET entity visibility to TRUE
				menuEntity->m_visible = true;
				pEntityImage->m_visible = true;

				// UPDATE the visual of the menu entity
				CMovementObj *mPtr = menuEntity->m_entityObj;
				if(!mPtr) break;
				CMovementObj *DBPtr = globalStatus->m_entObjList->GetByID(mPtr->List_refModel);
				mPtr->ArmItemByGLID(
					pInventory->HoverItem()->m_data.m_GLID,
					DBPtr,
					globalStatus->m_itemManager,
					NULL);
				break;
			}
			case MENU_SCRIPTFUNC_CREATECHARACTER:
			{
                CMenuEntity *menuEntity = (CMenuEntity*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(0), false);
				if(!menuEntity)
				{
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_CREATECHARACTER Cannot find entity ID:"+curLine->GetStrByParam(0));
                    break;
				}

				if(!menuEntity->m_entityObj)
				{
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_CREATECHARACTER entity visual not initialized ID:"+curLine->GetStrByParam(0));
                    break;
				}

				CMenuTextBox *nameTextBox = (CMenuTextBox *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);
				if(!nameTextBox)
				{
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_CREATECHARACTER Cannot find name text box ID:"+curLine->GetStrByParam(1));
                    break;
				}

				int spawnPosIndex = curLine->GetIntByParam(2);
				int result = globalStatus->CreateCharacter_Send(spawnPosIndex,nameTextBox->m_fullText,menuEntity->m_entityObj);

					switch(result)
					{
					   case 2://not connected to server
                          globalStatus->UI_Log("DEBUG-CreateCharacter_Send not connected to server:");
					   break;
					   case 3://Invalid name
                          globalStatus->UI_Log("DEBUG-CreateCharacter_Send Invalid Name:");
					   break;
					}
				break;
			}
			case MENU_SCRIPTFUNC_INITENTITYFROMDBID:
			{
				// GET references to menu visuals
				CMenuWindow *pMenu = NULL;
				int iMenuID = curLine->GetIntByParam(0);
				if (iMenuID > 0)
				{
					pMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMenuID, true));
					if (!pMenu) break;
				}

				// GET a reference to the entity menu
				CMenuEntity *menuEntity = NULL;
				if (pMenu)
					menuEntity = (CMenuEntity*)pMenu->m_childList->GetVisualByID(curLine->GetIntByParam(1), false);
				else
					menuEntity = (CMenuEntity*)this->m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(1), false);

				if(!menuEntity)
					globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_INITENTITYFROMDBID Cannot find entity ID:"+curLine->GetStrByParam(0));

				// INIT the entity menu
				if(!menuEntity->InitFromDBID(curLine->GetIntByParam(2)))
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_INITENTITYFROMDBID InitFromDBID Failed:"+curLine->GetStrByParam(1));
				break;
			}
			case MENU_SCRIPTFUNC_RENDERENTITYOBJECT:
			{   
				CMenuImage *visual = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
                 if(!visual)
				 {
                     globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RENDERENTITYOBJECT Cannot find image visual ID:"+curLine->GetStrByParam(0));
                     break;
				 }
				 visual->m_visible = true;

                 CMenuEntity *menuEntity = dynamic_cast<CMenuEntity*>(m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(9), false));
				 if(!menuEntity)
				 {
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RENDERENTITYOBJECT Cannot find entity ID:"+curLine->GetStrByParam(9));
                    break;
				 } 

				 if(!visual->m_optionalDynTexture)
				 {//create render target is NULL
                     HRESULT err = globalStatus->m_dxRenderer->m_pd3dDevice->CreateTexture( curLine->GetIntByParam(4),
																							curLine->GetIntByParam(5),
																							1,
																							D3DUSAGE_RENDERTARGET,
																							globalStatus->m_dxRenderer->m_renderTargetTextFormat,
																							D3DPOOL_DEFAULT,
																							&visual->m_optionalDynTexture,
																							NULL );
					 if(err != S_OK)
					 {
                        globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RENDERENTITYOBJECT Cannot create Render Target");
					    break;
					 }
				 }//end create render target is NULL
			     if(!visual->m_surfaceController)
				 {	
				   if(visual->m_optionalDynTexture->GetSurfaceLevel(0,&visual->m_surfaceController)!=S_OK)
				   {
					  globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_RENDERENTITYOBJECT Cannot create Render Surface");
					  break;
				   }
				   //init to color
				 }

				 D3DCOLOR backGround = D3DCOLOR_ARGB(0,curLine->GetIntByParam(6),curLine->GetIntByParam(7),curLine->GetIntByParam(8)); // alpha 0: BlendMode 1 images show the model solid on a clear background (additive BlendMode 2 ignores alpha)
				 
				 if(menuEntity->m_entityObj)
					 rendermanager->RenderEntityToSurface(   menuEntity->m_entityObj,//CMovementObj * mPtr,
															 visual->m_surfaceController, 
															 curLine->GetFloatByParam(3),//float camPitch, 
															 curLine->GetFloatByParam(2),//float camYaw, 
															 curLine->GetFloatByParam(1),//float camDist,
															 curLine->GetFloatByParam(4),//fov
															 &backGround,
															 (DWORD)curLine->GetIntByParam(4),
															 (DWORD)curLine->GetIntByParam(5));
				 break;
			}
			case MENU_SCRIPTFUNC_UPDATEENTITYANIMATION:
			{
				// GET references to menu visuals
				//CMenuImage *pEntityImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_ENTITY, true));
                CMenuEntity *pEntity = (CMenuEntity*)m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(0), false);
				if (!pEntity)
				{
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_UPDATEENTITYANIMATION Cannot find entity ID:"+curLine->GetStrByParam(0));
                    break;
				}

				// SET image visibility to TRUE
				//if (pEntityImage)
				//	pEntityImage->m_visible = true;

				// UPDATE the entity animation
				pEntity->UpdateAnimation();
				break;
			}
			case MENU_SCRIPTFUNC_SELECTQUEST:
			{
				CMenuListBox *visual = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				if(!visual)
				{
					globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_SELECTQUEST Cannot find List ID:"+curLine->GetStrByParam(0));
					break;
				}

				if(globalStatus->m_multiplayerObject->GetEnabledClient_safe() != FALSE && visual->m_currentSelection != NULL)
				{
					globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																		DPID_SERVERPLAYER,
																		32,
																		visual->m_currentSelection->m_index,
																		0,
																		0,
																		0,
																		NULL);
					//note to close menu after selection use scripting
				}
				else if(!visual->m_currentSelection)
				   globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_SELECTQUEST Cannot find selection ID:"+curLine->GetStrByParam(0));
				else
				   globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_SELECTQUEST Client cannot execute this function while not connected to server:"+curLine->GetStrByParam(0));
                   
                break;
			}
			case MENU_SCRIPTFUNC_CLEARLIST:
			{
				CMenuListBox *visual = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				if(!visual)
				{
					globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_SELECTQUEST Cannot find List ID:"+curLine->GetStrByParam(0));
					break;
				}

				//Clear Existing List
				while(visual->m_pFirstItem != NULL)
					  visual->RemoveItem(0);
				
				break;
			}
			case MENU_SCRIPTFUNC_SETANIMATION:
			{
				// GET references to menu visuals
				CMenuWindow *pMenu = NULL;
				int iMenuID = curLine->GetIntByParam(0);
				if (iMenuID > 0)
				{
					pMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMenuID, true));
					if (!pMenu) break;
				}

				// GET a reference to the entity menu
				CMenuEntity *menuEntity = NULL;
				if (pMenu)
					menuEntity = (CMenuEntity*)pMenu->m_childList->GetVisualByID(curLine->GetIntByParam(1), false);
				else
					menuEntity = (CMenuEntity*)this->m_callingMenu->m_childList->GetVisualByID(curLine->GetIntByParam(1), false);

				if (!menuEntity)
                    globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_INITENTITYFROMDBID Cannot find entity ID:"+curLine->GetStrByParam(0));

				// SET the the entity menu animation
				menuEntity->SetAnimationIndex(curLine->GetIntByParam(2));
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEHPLIGHT:
			{
				CMenuImage *visual = (CMenuImage*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
                if(!visual)
				{
					globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_UPDATEHPLIGHT Cannot find image visual ID:"+curLine->GetStrByParam(0));
					break;
				}
				//Create position and Visual Reference
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);
				//Set Text to Health Ratio
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);

				if(curLine->GetIntByParam(1) == 0)
				{
					if(((float)ptrLoc->List_energy / (float)ptrLoc->List_maxEnergy) > 0.3f)
						visual->m_visible = FALSE;
					else
						visual->m_visible = TRUE;
				}
				else
				{
					if(((float)ptrLoc->List_energy / (float)ptrLoc->List_maxEnergy) > 0.3f)
						visual->m_visible = FALSE;
					else
					{
						static DWORD counter = GetTickCount();
						if(GetTickCount() - counter > 1000)
						{
							counter = GetTickCount();
							if(visual->m_visible == TRUE)
								visual->m_visible = FALSE;
							else
								visual->m_visible = TRUE;
						}
					}
				}
				 
				break;
			}
			case MENU_SCRIPTFUNC_UPDATERESISTLIGHT:
			{
				CMenuImage *visual = (CMenuImage*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
                if(!visual)
				{
					globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_UPDATERESISTLIGHT Cannot find image visual ID:"+curLine->GetStrByParam(0));
					break;
				}
				//Create position and Visual Reference
				CMovementObj * ptrLoc = globalStatus->m_entObjRuntime->GetObjByIndex(0);
				if(!ptrLoc)
				    break;
				//Set Text to Health Ratio


				if(curLine->GetIntByParam(2) == 0)
				{
					if(ptrLoc->m_woundChannel[curLine->GetIntByParam(1)] >= 70 ||
					   ptrLoc->m_badDots[curLine->GetIntByParam(1)] > ptrLoc->m_goodDots[curLine->GetIntByParam(1)])
						visual->m_visible = TRUE;
					else
						visual->m_visible = FALSE;
				}
				else
				{
					if(ptrLoc->m_woundChannel[curLine->GetIntByParam(1)] >= 70 ||
					   ptrLoc->m_badDots[curLine->GetIntByParam(1)] > ptrLoc->m_goodDots[curLine->GetIntByParam(1)])
						visual->m_visible = TRUE;
					else
					{
						static DWORD counter = GetTickCount();
						if(GetTickCount() - counter > 1000)
						{
							counter = GetTickCount();
							if(visual->m_visible == TRUE)
								visual->m_visible = FALSE;
							else
								visual->m_visible = TRUE;
						}
					}
				}
				
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEBUFFMARKER:
			{
				CMenuImage *visual = (CMenuImage*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
                if(!visual)
				{
					globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_UPDATEHPLIGHT Cannot find image visual ID:"+curLine->GetStrByParam(0));
					break;
				}
				//Get Our Player
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);

				if(curLine->GetIntByParam(1) == 0)
				{
					static float defaultPosX1 = visual->m_posX;
					visual->m_posX = defaultPosX1 - ((float)ptrLoc->m_goodDots[curLine->GetIntByParam(2)] * 3);
				}
				else if(curLine->GetIntByParam(1) == 1)
				{
					static float defaultPosX2 = visual->m_posX;
					visual->m_posX = defaultPosX2 + ((float)ptrLoc->m_badDots[curLine->GetIntByParam(2)] * 3);
				}
				
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEQUICKSLOTHIGHLIGHT:
			{
				CMenuImage *visual = (CMenuImage*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				if(!visual)
				{
					globalStatus->UI_Log("DEBUG-MENU_SCRIPTFUNC_UPDATEQUICKSLOTHIGHLIGHT Cannot find image visual ID:"+curLine->GetStrByParam(0));
					break;
				}
				//Get Our Player
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);

				if(ptrLoc->m_quickSlotNumber == -1)
					visual->m_visible = FALSE;
				else
					visual->m_visible = TRUE;

				break;
			}
			case MENU_SCRIPTFUNC_ONSELECTEDSETKEYMAPLISTENER:
			{
				POSITION pos = globalStatus->m_controlsList->FindIndex(0);
				if(pos && globalStatus->m_inputReference)
				{
					// Retrieve object and visuals
					ControlDBObj *pControlDB = (ControlDBObj*)globalStatus->m_controlsList->GetAt(pos);
                    CMenuListBox *pListBox = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
					
					// Create the update script string
					char strVisualID[4];
					itoa(curLine->GetIntByParam(0), strVisualID, 10);
					CString strUpdateScript = "SetKeyControl(";
					strUpdateScript += strVisualID;
					strUpdateScript += ")";
						
					// Create and attach the update script object to 
					// begin the loop waiting for the user's next button/key input
					CMenuScript *pUpdateScript = new CMenuScript();
					pUpdateScript->AddLine(strUpdateScript.GetBuffer());
					strUpdateScript.ReleaseBuffer();
					pListBox->m_updateScript = pUpdateScript;
				}
				break;
			}
			case MENU_SCRIPTFUNC_SETKEYCONTROL:
			{
				// Retrieve menu listbox
				int iKeyCode = -1;
				InputTypes eType = VT_ALL;
                CMenuListBox *pListBox = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				if(pListBox && pListBox->m_currentSelection)
				{
					// Retrieve ControlDBObj
					POSITION pos = globalStatus->m_controlsList->FindIndex(0);
					if(!pos) break;
					ControlDBObj *pControlDB = (ControlDBObj*)globalStatus->m_controlsList->GetAt(pos);

					CString keystring = globalStatus->m_inputReference->GetInputAsString(globalStatus->GetTimeStamp_DW(), iKeyCode, eType);
					if(!m_waitOnInputFlag && (keystring != ""))
					{
						m_waitOnInputFlag = TRUE;
					}

					pos = pListBox->m_currentSelection->m_textList.GetHeadPosition();
					CMenuText *pNewAction = (CMenuText*)pListBox->m_currentSelection->m_textList.GetNext(pos);
					CMenuText *pNewKey = (CMenuText*)pListBox->m_currentSelection->m_textList.GetAt(pos);

					if(m_waitOnInputFlag && pos)
					{	
						// This function automaticaly handles add and update functionality
						keystring = globalStatus->m_inputReference->GetInputAsString(globalStatus->GetTimeStamp_DW(), iKeyCode);
						if (keystring != "")
						{
							// HANDLE updating the new key config binding!
							if (iKeyCode != DIK_ESCAPE)
							{
								// Update the listbox display
								ControlInfoObj *pControlInfo = pControlDB->List_controlInfoList->GetControlInfoObjectByKey(keystring);
								if(pControlInfo) 
								{
									// Find the list item to which our target key is currently bound
									CMenuListItem *pOldItem = NULL;
									CMenuText *pOldAction = NULL, *pOldKey = NULL;
									for(pOldItem = pListBox->m_pFirstItem; pOldItem != NULL; pOldItem = pOldItem->m_pNext)
									{
										pos = pOldItem->m_textList.GetHeadPosition();
										pOldAction = (CMenuText*)pOldItem->m_textList.GetNext(pos);
										pOldKey = (CMenuText*)pOldItem->m_textList.GetAt(pos);

										// We've found our old target, 
										// so update the display and BREAK
										if(pOldAction->m_text == pControlInfo->List_description)
										{
											if(pOldAction == pNewAction) break;
//											pOldKey->SetText("Unassigned");
											break;
										}
									}
								}

								// GET the display string for the key config menu
								std::map<int, CString>::const_iterator iter;
								CString strDisplay;
								if (eType == VT_ALPHANUMERIC || eType == VT_COMMAND || eType == VT_ALL)
								{
									iter = KeyMap.find(iKeyCode);
									if (iter != KeyMap.end()) 
										strDisplay = iter->second;
								}
								else if (eType == VT_MOUSE)
								{
									iter = MouseMap.find(iKeyCode);
									if (iter != MouseMap.end()) 
										strDisplay = iter->second;
								}

								// Setup in case Multibind is in affect
								BOOL Multi = TRUE;
								if(pControlDB->m_allowMultiBind)
									Multi = FALSE;

								CString strDisplayStorage;
								// Set new key assignment display
								if (strDisplay != "")
								{
									strDisplayStorage = iter->second;
									if(pControlDB->m_allowMultiBind)
										strDisplay = pNewKey->m_text + ", " + iter->second;
									else
										strDisplay = iter->second;
									pNewKey->SetText(strDisplay.GetBuffer());
									strDisplay.ReleaseBuffer();
								}

								// Add event listener for the new key mapping and
								// unassign any previous mapping
								pControlDB->List_controlInfoList->AddControlEvent(strDisplayStorage, //keystring,
									pNewAction->m_text,
									pListBox->m_currentSelection->m_miscInt1,
									pListBox->m_currentSelection->m_miscInt2,
									pListBox->m_currentSelection->m_miscInt3,
									pListBox->m_currentSelection->m_miscInt4,
									FALSE,
									Multi);
							}

							// DESELECT any currently selected list item
							if (keystring != "Left Click")
								pListBox->m_currentSelection = NULL;

							// Set function to a silly value to indicate that 
							// the update script should be deleted
							pListBox->m_updateScript->m_pFirstLine->m_function = -1;
							m_waitOnInputFlag = FALSE;
						}
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_INCREASEMOUSESENSE:
			case MENU_SCRIPTFUNC_DECREASEMOUSESENSE:
			{
				// Retrieve the control and menu objects
				POSITION pos = globalStatus->m_controlsList->FindIndex(0);
				if(!pos) break;
				ControlDBObj *cObj = (ControlDBObj *)globalStatus->m_controlsList->GetAt(pos);
				CMenuText *visual = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				
				// Get current sensitivity display value
				int iCurrSensitivity = atoi(visual->m_text.GetBuffer());
				visual->m_text.ReleaseBuffer();

				// Increase the mouse senstivity
				if(curLine->m_function == MENU_SCRIPTFUNC_INCREASEMOUSESENSE)
				{
					cObj->m_mouseSensitivity += (MAX_MOUSE_SENSITIVITY - MIN_MOUSE_SENSITIVITY) / MAX_MOUSE_SENSITIVITY_STEPS;
					if(cObj->m_mouseSensitivity > (MAX_MOUSE_SENSITIVITY + EPSILON))
						cObj->m_mouseSensitivity = MAX_MOUSE_SENSITIVITY;
					else
						iCurrSensitivity++;
				}
				// Decrease the mouse sensitivity
				else
				{
					cObj->m_mouseSensitivity -= ((MAX_MOUSE_SENSITIVITY - MIN_MOUSE_SENSITIVITY) / MAX_MOUSE_SENSITIVITY_STEPS);
					if(cObj->m_mouseSensitivity < (MIN_MOUSE_SENSITIVITY + EPSILON))
						cObj->m_mouseSensitivity = MIN_MOUSE_SENSITIVITY + EPSILON;
					else
						iCurrSensitivity--;
				}

				// Retrieve visual and set the new text
				CString strText = globalStatus->m_cabLib.ConvToString(iCurrSensitivity);
				visual->SetText(strText.GetBuffer());
				strText.ReleaseBuffer();
				break;
			}
			case MENU_SCRIPTFUNC_INCREASEMUSICVOLUME:
			case MENU_SCRIPTFUNC_DECREASEMUSICVOLUME:
			{
				//Get the control position
				POSITION pos = globalStatus->m_controlsList->FindIndex(0);
				if(!pos) break;

				//Retrieve object and visual
				ControlDBObj *pControl = (ControlDBObj*)globalStatus->m_controlsList->GetAt(pos);
				if (!pControl) break;

				// GET the control and menu objects
				CMenuText *pVolumeText = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				
				// GET current volume display value
				int iCurrentVolume = (int)pControl->m_musicVolume;
				if (pVolumeText)
				{
					iCurrentVolume = ::atoi(pVolumeText->m_text.GetBuffer());
					pVolumeText->m_text.ReleaseBuffer();
				}

				// INCREASE the music volume
				if(curLine->m_function == MENU_SCRIPTFUNC_INCREASEMUSICVOLUME)
				{
					if(iCurrentVolume >= VOLUME_SCALED_MAX)
						iCurrentVolume = VOLUME_SCALED_MAX;
					else
						++iCurrentVolume;
				}
				// DECREASE the music volume
				else
				{
					if(iCurrentVolume <= VOLUME_SCALED_MIN)
						iCurrentVolume = VOLUME_SCALED_MIN;
					else
						--iCurrentVolume;
				}

				// SET the music volume
				if (globalStatus->m_soundTrackList)
					globalStatus->m_soundTrackList->SetVolume(iCurrentVolume * VOLUME_SCALED_MAX);

				// CLAMP music volume value
				if (globalStatus->m_soundTrackList->m_musicPlayer->m_volumeSetting < VOLUME_MIN)
					globalStatus->m_soundTrackList->SetVolume(VOLUME_MIN);
				else if (globalStatus->m_soundTrackList->m_musicPlayer->m_volumeSetting > VOLUME_MAX)
					globalStatus->m_soundTrackList->SetVolume(VOLUME_MAX);

				// SET the new display text value
				if (pVolumeText)
				{
					CString strText = globalStatus->m_cabLib.ConvToString(iCurrentVolume);
					pVolumeText->SetText(strText.GetBuffer());
					strText.ReleaseBuffer();
				}

				// STORE music volume level in the control DB object
				pControl->m_musicVolume = (float)iCurrentVolume;
				break;
			}
			case MENU_SCRIPTFUNC_SETDEFAULTKEYCONFIGS:
			case MENU_SCRIPTFUNC_READKEYCONFIGFROMFILE:
			{
				static char strCurrentValue[4];
				BOOL bResult = FALSE;

				// Retrieve object and visuals
				int iListBoxID = curLine->GetIntByParam(0);
				int iToggleID = curLine->GetIntByParam(1);
				int iTextID = curLine->GetIntByParam(2);
				int iMusicVolumeID = curLine->GetIntByParam(3);
				int iMultiID = curLine->GetIntByParam(4);
				CMenuListBox *pListBox = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iListBoxID, true);
				CMenuToggle *pToggle = (CMenuToggle*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iToggleID, true);
				CMenuText *pSensitivityValue = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iTextID, true);
				CMenuText *pMusicVolumeText = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMusicVolumeID, true);
				CMenuToggle *pMulti = (CMenuToggle*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMultiID, true);

				// Load the movement object
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if(!pos) break;
				CMovementObj *pMovement = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(pos);

				// Create filename for writing
				CString strFileName = KEY_CONFIG_FILE;
				strFileName += ".";
				strFileName += globalStatus->m_cabLib.ConvToString(pMovement->m_uniqueId);

				// Init key configs from disk or default
				if (curLine->m_function == MENU_SCRIPTFUNC_SETDEFAULTKEYCONFIGS)
					bResult = globalStatus->LoadControlCfgList(strFileName, pListBox);	
				else if (curLine->m_function == MENU_SCRIPTFUNC_READKEYCONFIGFROMFILE)
					bResult = globalStatus->LoadControlCfgFromDisk(strFileName, pListBox);
				
				if(bResult)
				{
					// Get the ControlDBObj with key and mouse settings
					POSITION pos = globalStatus->m_controlsList->FindIndex(0);
					if(!pos) break;
					ControlDBObj *pControlDB = (ControlDBObj*)globalStatus->m_controlsList->GetAt(pos);

					float fStepAmount = ((MAX_MOUSE_SENSITIVITY - MIN_MOUSE_SENSITIVITY) / MAX_MOUSE_SENSITIVITY_STEPS);
					float fCurrentValue = (pControlDB->m_mouseSensitivity) / fStepAmount;
					int iCurrentValue = static_cast<int>(fCurrentValue);

					// Clamp the display values for mouse sensitivity
					if (pSensitivityValue)
					{
						if(iCurrentValue <= 0) 
							iCurrentValue++;
						else if(iCurrentValue >= MAX_MOUSE_SENSITIVITY_STEPS) 
							iCurrentValue = (int)MAX_MOUSE_SENSITIVITY_STEPS;
						itoa(iCurrentValue, strCurrentValue, 10);
						pSensitivityValue->SetText(strCurrentValue);
					}

					// Update the display value for Invert Mouse
					if (pToggle)
					{
						pToggle->m_isSet = pControlDB->m_invertMouse;
						if (pToggle->m_isSet)
							pToggle->m_pDisplayText->SetText("True");
						else
							pToggle->m_pDisplayText->SetText("False");
					}

					// Update the display value for Multibind
					if(pMulti)
					{
						pMulti->m_isSet = pControlDB->m_allowMultiBind;
						if (pMulti->m_isSet)
							pMulti->m_pDisplayText->SetText("True");
						else
							pMulti->m_pDisplayText->SetText("False");
					}

					// Update the display and value for Music Volume
					iCurrentValue = (int)pControlDB->m_musicVolume;
					if (pMusicVolumeText)
						pMusicVolumeText->SetText(globalStatus->m_cabLib.ConvToString(iCurrentValue));

					// SET the music volume
					if (globalStatus->m_soundTrackList)
						globalStatus->m_soundTrackList->SetVolume(iCurrentValue * VOLUME_SCALED_MAX);
				}//*/
				break;
			}
			case MENU_SCRIPTFUNC_WRITEKEYCONFIGTOFILE:
			{
				CFileException exc;
				CFile fl;
				POSITION pos = NULL;

				// Get the control position
				pos = globalStatus->m_controlsList->FindIndex(0);
				if(!pos) break;

				// Retrieve object and visuals
				ControlDBObj *pControlDB = (ControlDBObj*)globalStatus->m_controlsList->GetAt(pos);
				CMenuListBox *pListBox = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				CMenuToggle *pToggle = (CMenuToggle*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);
				CMenuText *pSensitivityValue = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(2), true);
				CMenuToggle *pMulti = (CMenuToggle*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(3), true);

				// Make sure we got our controls
				if(!pListBox || !pToggle || !pSensitivityValue || !pMulti)
				{
					globalStatus->UI_Log("MENU_SCRIPTFUNC_WRITEKEYCONFIGTOFILE Error");
					break;
				}

				// Load the movement object
				pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if(!pos) break;
				CMovementObj *pMovement = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(pos);

				// Create filename for writing
				CString strFileName = KEY_CONFIG_FILE;
				strFileName += ".";
				strFileName += globalStatus->m_cabLib.ConvToString(pMovement->m_uniqueId);

				// Open the control config file for writing. 
				// If it doesn't exist, then create it.
				SetCurrentDirectory(globalStatus->GL_pathGameFiles);				
				BOOL res = fl.Open(strFileName,
					CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite, 
					&exc);

				// If we got our file, write the key configs out to disk.
				if(res)
				{
					CArchive the_out_Archive(&fl,CArchive::store);
					the_out_Archive << globalStatus->m_controlsList;
					the_out_Archive.Close();
					fl.Close();
				}
				SetCurrentDirectory(globalStatus->GL_workingDirectory);

				break;
			}
			case MENU_SCRIPTFUNC_PLAYSONG:
			{
				if (!globalStatus->m_soundTrackList) break;

				// PLAY the music
				globalStatus->m_soundTrackList->Play(curLine->GetIntByParam(0),globalStatus->GL_pathGameSounds, VARIANT_TRUE);
				SetCurrentDirectory(globalStatus->GL_workingDirectory);
				break;
			}
			case MENU_SCRIPTFUNC_DEPOSITCREDITS:
			{
				//Check to make sure client enabled
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Retrieve player
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);
				if(entPosition)
				{
					CMenuTextBox *visual = (CMenuTextBox *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
					if(visual)
					{
						int amount = abs(atoi(visual->m_linesOfText[0].m_text));
						CMovementObj *ptrLoc = (CMovementObj *)globalStatus->m_entObjRuntime->GetAt(entPosition);
						COMMAND_DATA dataPacket;
						int messageSize			= sizeof(COMMAND_DATA);
						dataPacket.m_catagorie	= 18;
						dataPacket.m_data		= 1;
						dataPacket.m_miscInt	= amount;
						dataPacket.m_networkID	= ptrLoc->List_networkDefinedID;

						globalStatus->m_multiplayerObject->SendData((BYTE *)&dataPacket,
																  messageSize,
																  DPNSEND_GUARANTEED,
																  NULL,
																  globalStatus->m_multiplayerObject->m_packetTimeout);
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_WITHDRAWLCREDITS:
			{
				//Check to make sure client enabled
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Retrieve player
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);
				if(entPosition)
				{
					CMenuTextBox *visual = (CMenuTextBox *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
					if(visual)
					{
						int amount = abs(atoi(visual->m_linesOfText[0].m_text));
						CMovementObj *ptrLoc = (CMovementObj *)globalStatus->m_entObjRuntime->GetAt(entPosition);
						COMMAND_DATA dataPacket;
						int messageSize			= sizeof(COMMAND_DATA);
						dataPacket.m_catagorie	= 18;
						dataPacket.m_data		= 0;
						dataPacket.m_miscInt	= amount;
						dataPacket.m_networkID	= ptrLoc->List_networkDefinedID;

						globalStatus->m_multiplayerObject->SendData((BYTE *)&dataPacket,
																  messageSize,
																  DPNSEND_GUARANTEED,
																  NULL,
																  globalStatus->m_multiplayerObject->m_packetTimeout);
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_SELECTDRAGITEM:
			{
				/*
				if(!globalStatus->m_bIsDragging || globalStatus->m_draggingItem) break;

				CMenuListBox *pListBox = NULL;
				CMenuListItem *pListItem = NULL;
				CMenuImage *pImage = NULL;

				// Retrieve visual controls
				pListBox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pListBox) 
				{
					pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
					if(!pImage)
					{
						globalStatus->UI_Log("MENU_SCRIPTFUNC_SELECTDRAGITEM Error");
						break;
					}
				}

				// Create the update script string
				if(!curLine) break;
				int iQuickBar = curLine->GetIntByParam(1),
					iQuickSlot = curLine->GetIntByParam(2);
				char strParam[4];
				CString strUpdateScript = "DragQuickItem(";
				itoa(curLine->GetIntByParam(0), strParam, 10);
				strUpdateScript += strParam;
				// Quick Bar Index
				if(iQuickBar >= 0)
				{
					strUpdateScript += ",";
					itoa(iQuickBar, strParam, 10);
					strUpdateScript += strParam;
				}
				// Quick Bar Slot Index
				if(iQuickSlot >= 0)
				{
					strUpdateScript += ",";
					itoa(iQuickSlot, strParam, 10);
					strUpdateScript += strParam;
				}
				strUpdateScript += ")";
				
				// If we have a listbox...
				if(pListBox)
				{
					if(globalStatus->m_bIsDragging 
					&& pListBox->m_currentSelection
					&& !pListBox->m_itemJustSelected)
					{
						// Set the global dragging item pointer
						globalStatus->m_draggingItem = new CMenuListItem();
						if(pListBox->m_currentSelection->m_icon)
							pListBox->m_currentSelection->m_icon->Clone((CMenuVisual**)(&globalStatus->m_draggingItem->m_icon), globalStatus->m_3dDevice);
						if(globalStatus->m_draggingItem->m_icon)
						{
							globalStatus->m_draggingItem->m_icon->Init(globalStatus->m_3dDevice);
							globalStatus->m_draggingItem->m_icon->UpdateTexture(globalStatus->m_draggingItem->m_icon->m_textureSelect);
							globalStatus->m_draggingItem->m_icon->UpdateTextureCoords();
						}
						globalStatus->m_draggingItem->m_miscInt1 = pListBox->m_currentSelection->m_miscInt1;
						globalStatus->m_draggingItem->m_miscInt2 = pListBox->m_currentSelection->m_miscInt2;

						// Create and attach the update script object to 
						// begin the loop waiting for the user's next button/key input
						if(!pListBox->m_updateScript) pListBox->m_updateScript = new CMenuScript();
						pListBox->m_updateScript->ClearScript();
						pListBox->m_updateScript->AddLine(strUpdateScript.GetBuffer());
						strUpdateScript.ReleaseBuffer();
						pListBox->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_DRAGQUICKITEM;

						// Update current line
						curLine = pListBox->m_updateScript->m_pFirstLine;
					}
				}
				// Otherwise, this is an image...
				else 
				{
					if(globalStatus->m_bIsDragging 
					&& pImage->m_mousedOver 
					&& pImage->m_mousedDown 
					&& pImage->m_bDraggable)
					{
						// Grab the current coords for image position reset
						pImage->m_fLocalOriginX = pImage->m_posX;
						pImage->m_fLocalOriginY = pImage->m_posY;
						pImage->m_fGlobalOriginX = globalStatus->m_mouseCursors[globalStatus->m_currentCursor]->m_posX;
						pImage->m_fGlobalOriginY = globalStatus->m_mouseCursors[globalStatus->m_currentCursor]->m_posY;

						// Set the global dragging item pointer
						globalStatus->m_draggingItem = new CMenuListItem();
						pImage->Clone((CMenuVisual**)&globalStatus->m_draggingItem->m_icon, globalStatus->m_3dDevice);
						globalStatus->m_draggingItem->m_icon->Init(globalStatus->m_3dDevice);
						globalStatus->m_draggingItem->m_icon->UpdateTexture(globalStatus->m_draggingItem->m_icon->m_textureSelect);
						globalStatus->m_draggingItem->m_icon->UpdateTextureCoords();

						globalStatus->m_draggingItem->m_miscInt1 = globalStatus->m_multiplayerObject->Quickbar(iQuickBar, iQuickSlot);
						
						// Create and attach the update script object to 
		 				// begin the loop waiting for the user's next button/key input
						if(!pImage->m_updateScript) pImage->m_updateScript = new CMenuScript();
						pImage->m_updateScript->ClearScript();
						pImage->m_updateScript->AddLine(strUpdateScript.GetBuffer());
						strUpdateScript.ReleaseBuffer();
						pImage->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_DRAGQUICKITEM;

						// Update current line
						curLine = pImage->m_updateScript->m_pFirstLine;
					}
				}*/
				break;
			}
			case MENU_SCRIPTFUNC_DRAGQUICKITEM:
			{
				/*
				// Get a pointer to our control
				CMenuListBox *pListBox = NULL;
				CMenuImage *pImage = NULL;
				int iQuickBar = 0,
					iQuickSlot = 0;

				pListBox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pListBox)
				{
					pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
					if(!pImage)
					{
						globalStatus->UI_Log("MENU_SCRIPTFUNC_DRAGQUICKITEM Error");
						break;
					}
				}
				else
				{
					if(pListBox->m_currentSelection)
						pImage = pListBox->m_currentSelection->m_icon;
					if(!pImage)
					{
						globalStatus->UI_Log("MENU_SCRIPTFUNC_DRAGQUICKITEM Error");
						break;
					}
				}

				// Create the update script string
				if(!curLine) break;
				char strParam[4];
				CString strUpdateScript = "SelectDragItem(";
				itoa(curLine->GetIntByParam(0), strParam, 10);
				strUpdateScript += strParam;
				// Quick Bar Index
				if(curLine->GetIntByParam(1) >= 0)
				{
					strUpdateScript += ",";
					itoa(curLine->GetIntByParam(1), strParam, 10);
					strUpdateScript += strParam;
				}
				// Quick Bar Slot Index
				if(curLine->GetIntByParam(2) >= 0)
				{
					strUpdateScript += ",";
					itoa(curLine->GetIntByParam(2), strParam, 10);
					strUpdateScript += strParam;
				}
				strUpdateScript += ")";

				// We're here while the user is DRAGGING the mouse
				if (globalStatus->m_bIsDragging)
				{
					if(globalStatus->m_draggingItem && globalStatus->m_draggingItem->m_icon)
					{	
						float halfWidth	 = globalStatus->m_dxRenderer->m_currentWidth / 2.0f;
						float halfHeight = globalStatus->m_dxRenderer->m_currentHeight / 2.0f;
						float fCursorPosX = globalStatus->m_mouseCursors[globalStatus->m_currentCursor]->m_posX;						
						float fCursorPosY = globalStatus->m_mouseCursors[globalStatus->m_currentCursor]->m_posY;
						fCursorPosX -= halfWidth + (globalStatus->m_draggingItem->m_icon->m_width / 2.0f);
						fCursorPosY -= halfHeight - (globalStatus->m_draggingItem->m_icon->m_height / 2.0f);

						// Update position of the dragging item's icon
						globalStatus->m_draggingItem->m_icon->m_posX = fCursorPosX;
						globalStatus->m_draggingItem->m_icon->m_posY = fCursorPosY;
					}
				}
				// We're here when the user RELEASES the left mouse button
				else
				{
					// Reset list box properties
					if(pListBox && pListBox->m_updateScript) 
					{
						// Return the icon to its home
						pImage->m_posX = 0.0f; 
						pImage->m_posY = 0.0f; 
						pImage->m_fLocalOriginX = std::numeric_limits<float>::infinity();
						pImage->m_fLocalOriginY = std::numeric_limits<float>::infinity();

						// Disassociate global dragging item
						globalStatus->m_deleteDraggingItem = true;
						pListBox->m_currentSelection = NULL;

						// Reset update script
						if(!pListBox->m_updateScript) pListBox->m_updateScript = new CMenuScript();
						pListBox->m_updateScript->ClearScript();
						pListBox->m_updateScript->AddLine(strUpdateScript.GetBuffer());
						strUpdateScript.ReleaseBuffer();
						pListBox->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_SELECTDRAGITEM;

						// Update current line
						curLine = pListBox->m_updateScript->m_pFirstLine;
					}
					// Reset image properties
					else if(pImage && pImage->m_updateScript)
					{						
						// If image is outside of 1.5x the original bounds, remove it from the quick items inventory
						if((pImage->m_posX < (pImage->m_fGlobalOriginX - (pImage->m_width / 2.0f))
						|| (pImage->m_posX > (pImage->m_fGlobalOriginX + (1.5f * pImage->m_width)))
						|| (pImage->m_posY < pImage->m_fGlobalOriginY - (pImage->m_height / 2.0f))
						|| (pImage->m_posY > (pImage->m_fGlobalOriginY + (1.5f * pImage->m_height)))))
						{
							// Remove the slot GLID
							iQuickBar = curLine->GetIntByParam(1);
							iQuickSlot = curLine->GetIntByParam(2);
							globalStatus->m_multiplayerObject->Quickbar(iQuickBar, iQuickSlot, -1);

							// Clear the image from the hot slot
							pImage->m_texturePosX = 510.0f;
							pImage->m_texturePosY = 510.0f;
							pImage->m_textureWidth = 1.0f;
							pImage->m_textureHeight = 1.0f;
							pImage->m_blendMode = 1;
							pImage->m_bDraggable = false;
							pImage->Init(globalStatus->m_3dDevice);
							pImage->UpdateTexture(pImage->m_textureSelect);
							pImage->UpdateTextureCoords();

							// Save the quick bar system back out
							globalStatus->SaveQuickbarConfig();
						}

						// Disassociate global dragging item
						globalStatus->m_deleteDraggingItem = true;

						// Return the icon to its home
						pImage->m_posX = pImage->m_fLocalOriginX;
						pImage->m_posY = pImage->m_fLocalOriginY;
						pImage->m_fLocalOriginX = std::numeric_limits<float>::infinity();
						pImage->m_fLocalOriginY = std::numeric_limits<float>::infinity();
						pImage->m_fGlobalOriginX = std::numeric_limits<float>::infinity();
						pImage->m_fGlobalOriginY = std::numeric_limits<float>::infinity();

						if(!pImage->m_updateScript) pImage->m_updateScript = new CMenuScript();
						pImage->m_updateScript->ClearScript();
						pImage->m_updateScript->AddLine(strUpdateScript.GetBuffer());
						strUpdateScript.ReleaseBuffer();
						pImage->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_SELECTDRAGITEM;

						// Update current line
						curLine = pImage->m_updateScript->m_pFirstLine;
					}
				}*/
				break;
			}
			case MENU_SCRIPTFUNC_ADDITEMTOQUICKSLOT:
			{
				/*
				// Get the list visual
				CMenuImage *visual = (CMenuImage*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);

				// Make sure there was a dragging item and visual
				if(!globalStatus->m_draggingItem || !visual)
					break;

				// If the item being added is already in the quick items inventory,
				// remove the pre-existing item entry
				CMenuImage *pTemp = NULL;
				int iQuickBar = curLine->GetIntByParam(1);
				int iSlotID = visual->m_visualID - curLine->GetIntByParam(2); // First slot image visual ID
				for(int i=0; i<NUM_QUICKBAR_SLOTS; i++)				
				{
					// Get the current quick item
					pTemp = (CMenuImage*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iSlotID++, true);
					
					// If the GLID are equivalent for the current quick item and the dragging one,
					// remove the current quick item
					if(globalStatus->m_multiplayerObject->Quickbar(iQuickBar, i) == globalStatus->m_draggingItem->m_miscInt1)
					{
						// Remove this slot's GLID
						iQuickBar = curLine->GetIntByParam(1);
						globalStatus->m_multiplayerObject->Quickbar(iQuickBar, i, -1);

						// Clear the image from the hot slot
						pTemp->m_texturePosX = 0.0f;
						pTemp->m_texturePosY = 0.0f;
						pTemp->m_textureWidth = 0.0f;
						pTemp->m_textureHeight = 0.0f;
						pTemp->m_blendMode = 1;
						pTemp->m_bDraggable = false;
						pTemp->Init(globalStatus->m_3dDevice);
						pTemp->UpdateTexture(pTemp->m_textureSelect);
						pTemp->UpdateTextureCoords();
						break;
					}
				}

				// Update the texture position
				visual->m_texturePosX		= globalStatus->m_draggingItem->m_icon->m_texturePosX;
				visual->m_texturePosY		= globalStatus->m_draggingItem->m_icon->m_texturePosY;
				visual->m_textureWidth		= globalStatus->m_draggingItem->m_icon->m_textureWidth;
				visual->m_textureHeight		= globalStatus->m_draggingItem->m_icon->m_textureHeight;
				visual->m_fullTextureWidth	= globalStatus->m_draggingItem->m_icon->m_fullTextureWidth;
				visual->m_fullTextureHeight	= globalStatus->m_draggingItem->m_icon->m_fullTextureHeight;
				visual->m_blendMode			= globalStatus->m_draggingItem->m_icon->m_blendMode;

				// Update the texture for render
				visual->Init(globalStatus->m_3dDevice);
				visual->UpdateTexture(globalStatus->m_draggingItem->m_icon->m_textureSelect);
				visual->UpdateTextureCoords();

				// This quick slot image is now draggable
				visual->m_bDraggable = true;

				// Store the item GLID
				if(globalStatus->m_draggingItem->m_miscInt1 > -1 && curLine->GetIntByParam(1) > -1 && curLine->GetIntByParam(2) > -1)
					globalStatus->m_multiplayerObject->Quickbar(curLine->GetIntByParam(1), curLine->GetIntByParam(2), globalStatus->m_draggingItem->m_miscInt1);

				// Create the update script string
				if(!curLine) break;
				char strParam[4];
				CString strUpdateScript = "SelectDragItem(";
				itoa(curLine->GetIntByParam(0), strParam, 10);
				strUpdateScript += strParam;
				// Quick Bar Index
				if(curLine->GetIntByParam(1) >= 0)
				{
					strUpdateScript += ",";
					itoa(curLine->GetIntByParam(1), strParam, 10);
					strUpdateScript += strParam;
				}
				// Quick Bar Slot Index
				if(curLine->GetIntByParam(2) >= 0)
				{
					strUpdateScript += ",";
					itoa(curLine->GetIntByParam(2), strParam, 10);
					strUpdateScript += strParam;
				}
				strUpdateScript += ")";

				// Assign the new update script to this menu image
				if(!visual->m_updateScript) visual->m_updateScript = new CMenuScript();
				visual->m_updateScript->ClearScript();
				visual->m_updateScript->AddLine(strUpdateScript.GetBuffer());
				strUpdateScript.ReleaseBuffer();
				visual->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_SELECTDRAGITEM;

				// Save the quick bar system back out
				globalStatus->SaveQuickbarConfig();*/
				break;
			}
			/*case MENU_SCRIPTFUNC_SETQUICKITEMVISUAL:
			{
				globalStatus->m_multiplayerObject->m_ActiveItem[0] = curLine->GetIntByParam(0);
				break;
			}*/
			case MENU_SCRIPTFUNC_HPHOVER:
			{
				CMenuProgressBar *pProgress = dynamic_cast<CMenuProgressBar*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pProgress)
					break;

				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Make sure there is a valid posLoc
				if(!posLoc)
					break;
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);

				// SHOW the hover text
				CString strMessage = globalStatus->m_cabLib.ConvToString(ptrLoc->List_energy) + " / " + globalStatus->m_cabLib.ConvToString(ptrLoc->List_maxEnergy);
				globalStatus->ShowHoverText(ID_HOVER_TEXT_MENU, strMessage);

				break;
			}
			case MENU_SCRIPTFUNC_BASICHOVER:
			{
				CString strMessage = curLine->GetStrByParam(0);
				globalStatus->ShowHoverText(ID_HOVER_TEXT_MENU, strMessage);
				break;
			}
			case MENU_SCRIPTFUNC_XPHOVER:
			{
				CMenuImage *pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pImage)
					break;

				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Make sure there is a valid posLoc
				if(!posLoc)
					break;
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);

				// SHOW the hover text
				CString strMessage = globalStatus->m_cabLib.ConvToString(ptrLoc->m_curXP) + " / " + globalStatus->m_cabLib.ConvToString(ptrLoc->m_nextLvlXP);
				globalStatus->ShowHoverText(ID_HOVER_TEXT_MENU, strMessage);
				
				break;
			}
			case MENU_SCRIPTFUNC_FACTIONHOVER:
			{
				CMenuImage *pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pImage)
					break;

				CString strMessage = "";
				if(curLine->GetIntByParam(1) == 0)
					strMessage = "+50 health bonus";
				else if(curLine->GetIntByParam(1) == 1)
					strMessage = "+10 extra damage";
				else if(curLine->GetIntByParam(1) == 2)
					strMessage = "+5 damage reduction";

				// SHOW the hover text
				globalStatus->ShowHoverText(ID_HOVER_TEXT_MENU, strMessage);
				break;
			}
			case MENU_SCRIPTFUNC_SHOWEMBLEMHOVER:
			{
				int iImageID = curLine->GetIntByParam(0),
					iGLID = 0;
				CMenuImage *pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iImageID, true));
				if (!pImage) break;

				iGLID = pImage->m_GLID;
				CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(iGLID);
				if (!pItem) break;

				// SET emblem name in the hover text
				CString strMessage = pItem->m_itemName;

				// APPEND emblem attributes in the hover text
				for (int i=0; i<MAX_EMBLEM; i++)
				{
					if (!pItem->m_emblemEffects[i])
						continue;

					switch (i)
					{
						case HEALTH_P10:
						{
							strMessage.Append("\nHealth +10");
							break;
						}
						case HEALTH_P20:
						{
							strMessage.Append("\nHealth +20");
							break;
						}
						case RESIST_P01:
						{
							strMessage.Append("\nResist +1");
							break;
						}
						case DAMAGE_TAKEN_M05:
						{
							strMessage.Append("\nDamage Taken -5");
							break;
						}
						case AMMO_REGEN_P02:
						{
							strMessage.Append("\nAmmo Regen +2");
							break;
						}
						default: break;
					}
				}

				// SHOW the hover text
				globalStatus->ShowHoverText(ID_HOVER_TEXT_MENU, strMessage);
				break;
			}
			/*case MENU_SCRIPTFUNC_SHOWINVENTORYHOVER:
			{
				// GET menu visuals
				CMenuWindow *pStoreMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(STORE_MENU_ID, true));
				CMenuWindow *pInventoryMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_MENU, true));
				CMenuListBox *pInventoryList = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(INVENTORY_LISTBOX_ID, true));
				if (!pInventoryMenu || !pInventoryList) break;

				// GET the currently 
				// IF the store menu is open...
				CString strMessage;
				if (!pInventoryList->m_currentHover) break;
				if (pStoreMenu)
				{
					// GET the hover list item
					int iGLID = pInventoryList->m_currentHover->m_miscInt1;
					CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(iGLID);
					if (!pItem) break;

					// DISPLAY the item resale value
					strMessage.Append("Resale Value: ");
					strMessage.Append(globalStatus->m_cabLib.ConvToString(pItem->m_marketCost / 4.0f));
					strMessage.Append(" MDB");
					globalStatus->ShowHoverText(strMessage);
				}
				// ELSE, handle regular inventory hover text
				//	NOTE: Currently only displaying element defenses for armor items.
				else
				{					
					int iGLID = pInventoryList->m_currentHover->m_miscInt1;
					CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(iGLID);
					if (!pItem || !pItem->m_elementDefenses) 
						break;

					POSITION pos = NULL;
					for (pos = pItem->m_elementDefenses->GetHeadPosition(); pos != NULL;)
					{
						CElementStatusObj *pDefense = dynamic_cast<CElementStatusObj*>(pItem->m_elementDefenses->GetNext(pos));
						if (!pDefense || (pDefense->m_currentValue == 0))
							continue;

						std::map<int, CString>::const_iterator iter;
						iter = ElementDefenseMap.find((ELEMENT_DEFENSES)pDefense->m_elementIndex);
						if (iter == ElementDefenseMap.end())
							continue;

						strMessage.Append(iter->second);
						strMessage.Append(": ");
						strMessage.Append(globalStatus->m_cabLib.ConvToString(pDefense->m_currentValue));
						if (pos != NULL) strMessage.Append("\n");
					}
					globalStatus->ShowHoverText(strMessage);
				}
				break;
			}*/
			case MENU_SCRIPTFUNC_INITARENABANNERS:
			{
				// Get all arena banner images
				CMenuImage *pBegin = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BANNER_BEGIN_ID, true));
				CMenuImage *pVictory = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BANNER_VICTORY_ID, true));
				CMenuImage *pDefeat = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BANNER_DEFEAT_ID, true));

				// Make menu images fully transparent
				if(pBegin)
				{
					pBegin->m_alpha = 0.0f;
					pBegin->Init(globalStatus->m_3dDevice);
					pBegin->m_imageMesh->List_blendType = 1;
					pBegin->UpdateTexture(pVictory->m_textureSelect);
					pBegin->UpdateTextureCoords();
				}

				if(pVictory)
				{
					pVictory->m_alpha = 0.0f;
					pVictory->Init(globalStatus->m_3dDevice);
					pVictory->m_imageMesh->List_blendType = 1;
					pVictory->UpdateTexture(pVictory->m_textureSelect);
					pVictory->UpdateTextureCoords();
				}

				if(pDefeat)
				{
					pDefeat->m_alpha = 0.0f;
					pDefeat->Init(globalStatus->m_3dDevice);
					pDefeat->m_imageMesh->List_blendType = 1;
					pDefeat->UpdateTexture(pDefeat->m_textureSelect);
					pDefeat->UpdateTextureCoords();
				}
				break;
			}
			case MENU_SCRIPTFUNC_INITARENABADGES:
			{
				// Get all arena badge images
				CMenuImage *pMostWanted = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BADGE_MOST_WANTED_ID, true));
				CMenuImage *pKillingBlow = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BADGE_KILLING_BLOW_ID, true));
				CMenuImage *pSoleSurvivor = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BADGE_SOLE_SURVIVOR_ID, true));

				// Make menu images fully transparent
				if(pMostWanted)
				{
					pMostWanted->m_alpha = 0.0f;
					pMostWanted->Init(globalStatus->m_3dDevice);
					pMostWanted->m_imageMesh->List_blendType = 1;
					pMostWanted->UpdateTexture(pMostWanted->m_textureSelect);
					pMostWanted->UpdateTextureCoords();
				}

				if(pKillingBlow)
				{
					pKillingBlow->m_alpha = 0.0f;
					pKillingBlow->Init(globalStatus->m_3dDevice);
					pKillingBlow->m_imageMesh->List_blendType = 1;
					pKillingBlow->UpdateTexture(pKillingBlow->m_textureSelect);
					pKillingBlow->UpdateTextureCoords();
				}

				if(pSoleSurvivor)
				{
					pSoleSurvivor->m_alpha = 0.0f;
					pSoleSurvivor->Init(globalStatus->m_3dDevice);
					pSoleSurvivor->m_imageMesh->List_blendType = 1;
					pSoleSurvivor->UpdateTexture(pSoleSurvivor->m_textureSelect);
					pSoleSurvivor->UpdateTextureCoords();
				}
				break;
			}
			case MENU_SCRIPTFUNC_FADEIMAGE:
			{
				// Open the image visual
				int iVisualID = curLine->GetIntByParam(0);
				CMenuImage *pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iVisualID, true));
				float FADE_RATE = curLine->GetFloatByParam(1);
				float fadeDec = curLine->GetFloatByParam(2);
				if(!pImage || FADE_RATE == 0.0f) break;

				if(!m_fadeRate)
					m_fadeRate = FADE_RATE;
				float fadeCnv = m_fadeRate - 1.0f;
				if(fadeDec)
					fadeCnv *= fadeDec;
				else
					fadeCnv *= 0.5f;
				if(fadeCnv < 0.01f)
					fadeCnv = 0.01f;

				// Reduce the image alpha by the fade rate amount
				pImage->m_alpha /= m_fadeRate;
				m_fadeRate = fadeCnv + 1.0f;

				pImage->Init(globalStatus->m_3dDevice);
				pImage->m_imageMesh->List_blendType = 1;
				pImage->UpdateTexture(pImage->m_textureSelect);
				pImage->UpdateTextureCoords();

				// If the alpha is a very small amount,
				// make the image invisible and remove the fade script
				if(abs(pImage->m_alpha) <= EPSILON)
				{
					m_fadeRate = 0.0f;
					pImage->m_alpha = 0.0f;
					pImage->Init(globalStatus->m_3dDevice);
					pImage->m_imageMesh->List_blendType = 1;
					pImage->UpdateTexture(pImage->m_textureSelect);
					pImage->UpdateTextureCoords();

					// Remove the update script
					if(pImage->m_updateScript)
					{
						pImage->m_updateScript->m_pFirstLine->m_function = -1;
						curLine = NULL;
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_SETNOTIFICATIONSTRING:
			{
				CMenuText *notificationTextOutput = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
                if(notificationTextOutput)
				{
			       if(notificationTextOutput->m_text != globalStatus->gl_helperString)
				   {
					   notificationTextOutput->m_text = globalStatus->gl_helperString;
					   if(notificationTextOutput->m_textMesh)
					   {
							notificationTextOutput->m_textMesh->SafeDelete();
							delete notificationTextOutput->m_textMesh;
							notificationTextOutput->m_textMesh = NULL;
					   }
				   }
				}
				break;
			}
			case MENU_SCRIPTFUNC_CHECKNPCCOLLISION:
			{
				int iMenuID = curLine->GetIntByParam(0);
				CMenuVisual *pMenu = (CMenuVisual*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMenuID, true);

				// If the player character is outside the collsion radius of the NPC character,
				// then close the specified menu.
				if(pMenu && (globalStatus->gl_helperString.GetLength() == 0))
				{
					globalStatus->m_runtimeMenuDatabase->CloseByID(iMenuID, &globalStatus->m_showMenuCursor);
				}
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEARENATIMER:
			{
				int iTextID = curLine->GetIntByParam(0);
				CMenuText *pText = (CMenuText*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iTextID, true);
				if(!pText) break;

				// Update GSV arena related variables
				CString strDisplay;
				DWORD dwElapsed = globalStatus->GetElaspeTimeMS_DW(globalStatus->m_iArenaTimeStamp);

				// Update the GSV arena timestamp
				if(globalStatus->m_highResTimerEnabled)
					globalStatus->m_iArenaTimeStamp = timeGetTime();
				else
					globalStatus->m_iArenaTimeStamp = GetTickCount();
				globalStatus->m_iArenaTimer -= dwElapsed;

				// Bounds checking for the arena timer
				if (((int)globalStatus->m_iArenaTimer) < 0) break;

				// Calculate for display
				DWORD dwTimeRemaining = globalStatus->m_iArenaTimer / 1000;
				int iMinute = dwTimeRemaining / 60;
				int iSecond = dwTimeRemaining % 60;
				
				// Create display string for time remaining
				strDisplay += "Time remaining_ ";
				if(iMinute < 10) 
					strDisplay += "0";
				strDisplay += globalStatus->m_cabLib.ConvToString(iMinute);
				strDisplay += ":";
				if(iSecond < 10) 
					strDisplay += "0";
				strDisplay += globalStatus->m_cabLib.ConvToString(iSecond);

				// Update the display text
				pText->SetText(strDisplay.GetBuffer());
				strDisplay.ReleaseBuffer();
				break;
			}
			case MENU_SCRIPTFUNC_SETRGB:
			{
				int iImageID = curLine->GetIntByParam(0);
				float fRed = curLine->GetFloatByParam(1);
				float fGreen = curLine->GetFloatByParam(2);
				float fBlue = curLine->GetFloatByParam(3);
				CMenuImage *pImage = (CMenuImage*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(iImageID, true);
				if(!pImage) break;

				pImage->m_red = fRed;
				pImage->m_green = fGreen;
				pImage->m_blue = fBlue;
				break;
			}
			case MENU_SCRIPTFUNC_TRADECONFIRM:
			{
				// GET the client-side player object
				CMovementObj *ptrLoc = dynamic_cast<CMovementObj*>(globalStatus->m_entObjRuntime->GetObjByIndex(0));
				if (!ptrLoc || (ptrLoc->List_controlType != 0)) break;
					
				// GET references to menu visuals
				CMenuInventory *pYourInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_YOUR_INVENTORY, true));
				CMenuInventory *pTheirInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_THEIR_INVENTORY, true));
				CMenuButton *pOfferButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_OFFER_BUTTON, true));
				CMenuButton *pConfirmButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CONFIRM_BUTTON, true));
				CMenuButton *pCancelButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CANCEL_BUTTON, true));
				if (!pYourInventory
				|| !pTheirInventory
				|| !pOfferButton
				|| !pConfirmButton
				|| !pCancelButton)
					break;

				// ENABLE/DISABLE buttons
				pOfferButton->SetEnabled(false);
				pConfirmButton->SetEnabled(false);
				pCancelButton->SetEnabled(false);

				// SEND the Confirm Trade command string, IF we're in the game client
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe()) 
					globalStatus->SendChatString("/TRADECONFIRM");

				// CLEAR the trade inventories
				pYourInventory->ClearItems();
				pTheirInventory->ClearItems();

				// SHOW message box for trade confirmation
				globalStatus->ShowMessageBox("Trade Success", "Congratulations! Your trade offer has been confirmed.");
				break;
				/*
				CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetObjByIndex(0);
                if(ptrLoc)
				if(ptrLoc->List_controlType == 0)
					globalStatus->SendChatString(":TRADECONFIRM");

				CMenuListBox *visual = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(TRADE_LIST, true);
				if(visual)
				{
					while(visual->m_pFirstItem != NULL)
						visual->RemoveItem(0);
				}

				break;*/
			}
			case MENU_SCRIPTFUNC_TRADEOFFER:
			{
				// PARSE script params
				MENU_IDS eMenuID = (MENU_IDS)curLine->GetIntByParam(0);

				// GET references to menu visuals
				CMenuInventory *pInventory = globalStatus->GetActiveInventory(eMenuID);
				CMenuButton *pOfferButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_OFFER_BUTTON, true));
				CMenuButton *pConfirmButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CONFIRM_BUTTON, true));
				CMenuButton *pCancelButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CANCEL_BUTTON, true));
				if (!pInventory 
				|| (pInventory->Count() <= 0)
				|| !pOfferButton
				|| !pConfirmButton
				|| !pCancelButton)
					break;

				// ENABLE/DISABLE buttons
				pConfirmButton->SetEnabled();
				pCancelButton->SetEnabled();
				pOfferButton->SetEnabled(false);

				// GET the client-side player object
				CMovementObj *pMovementObject = dynamic_cast<CMovementObj*>(globalStatus->m_entObjRuntime->GetObjByIndex(0));
                if (!pMovementObject || (pMovementObject->List_controlType != 0)) break;

				// CREATE the server command string
				CString strMessage = "/TRADEOFFER ";
				for (int i=0; i<pInventory->Count(); i++)
				{
					// Offer Trade command string format:
					//	"/TRADEOFFER [GLID]:[QUANTITY];[...]"
					CMenuInventoryItem *pItem = pInventory->m_items[i];
					if (!pItem) continue;
					strMessage.Append(globalStatus->m_cabLib.ConvToString(pItem->m_data.m_GLID));
					strMessage.Append(":");
					strMessage.Append(globalStatus->m_cabLib.ConvToString(pItem->m_data.m_quantity));
					strMessage.Append(";");
				}

				// IF we're in the game client, SEND the command string
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe()) 
					globalStatus->SendChatString(strMessage);
				break;
                /*CMenuListBox *visual = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				if(!visual)
				{
					globalStatus->UI_Log("MENU_SCRIPTFUNC_TRADEOFFER error:cannot find visual list");
					break;
				}


				CString svrOfferBuild,tempSeg;
				svrOfferBuild = "/tradeoffer ";
				CMenuListItem *curItem = visual->m_pFirstItem;
				while(curItem)
				{
					tempSeg.Format("%i:%i;",curItem->m_miscInt1,curItem->m_miscInt2);
					svrOfferBuild += tempSeg;
					curItem = curItem->m_pNext;
				}
				if(svrOfferBuild.GetLength() == 0)
				   break;

				CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetObjByIndex(0);
                if(ptrLoc)
				if(ptrLoc->List_controlType == 0)
					globalStatus->SendChatString(svrOfferBuild);

				break;*/
			}
			case MENU_SCRIPTFUNC_TRADECANCEL:
			{
				// GET the client-side player object
				CMovementObj *ptrLoc = dynamic_cast<CMovementObj*>(globalStatus->m_entObjRuntime->GetObjByIndex(0));
				if (!ptrLoc || (ptrLoc->List_controlType != 0)) break;
					
				// GET references to menu visuals
				CMenuInventory *pYourInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_YOUR_INVENTORY, true));
				CMenuInventory *pTheirInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_THEIR_INVENTORY, true));
				CMenuInventory *pPlayerInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
				CMenuButton *pOfferButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_OFFER_BUTTON, true));
				CMenuButton *pConfirmButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CONFIRM_BUTTON, true));
				CMenuButton *pCancelButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CANCEL_BUTTON, true));
				if (!pYourInventory
				|| !pTheirInventory
				|| !pOfferButton
				|| !pConfirmButton
				|| !pCancelButton)
					break;

				// ENABLE/DISABLE buttons
				pOfferButton->SetEnabled(false);
				pConfirmButton->SetEnabled(false);
				pCancelButton->SetEnabled(false);

				// SEND the Cancel Trade command string, IF we're in the game client
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe()) 
					globalStatus->SendChatString("/TRADECANCEL");

				// TRANSFER all trade items back to the player's inventory
				for (int i=0; i<pYourInventory->Count(); i++)
				{
					if (!pYourInventory->m_items[i]) continue;
						ITEMDATA_STRUCT_EX item = pYourInventory->m_items[i]->m_data;
					if(pPlayerInventory)
						pPlayerInventory->AddItem(item);
				}

				// CLEAR the trade inventories
				pYourInventory->ClearItems();
				pTheirInventory->ClearItems();
				break;
				/*
                CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetObjByIndex(0);
                if(ptrLoc)
				if(ptrLoc->List_controlType == 0)
					globalStatus->SendChatString(":TRADECANCEL");

				CMenuListBox *visual = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(TRADE_LIST, true);
				if(visual)
				{
					while(visual->m_pFirstItem != NULL)
						visual->RemoveItem(0);
				}

				break;*/
			}
			case MENU_SCRIPTFUNC_DROPDRAGITEMTOLIST:
			{
				if(!globalStatus->m_draggingItem && !globalStatus->m_lastItemID)
				{
					globalStatus->UI_Log("MENU_SCRIPTFUNC_DROPDRAGITEMTOLIST error:dragging visual is invalid");
					break;
				}

				CMenuTextBox *editVisual = (CMenuTextBox *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);

				int glID = globalStatus->m_lastItemID;
				int quantity = 0;
				
				if(editVisual)
					if(editVisual->m_linesOfText)
					{//valid
						int quanityCnv = atoi(editVisual->m_linesOfText[0].m_text);
						if(quanityCnv > globalStatus->m_lastItemAmount)
							quanityCnv = globalStatus->m_lastItemAmount;
						if(quanityCnv < 0)
							quanityCnv = 1;
						quantity = quanityCnv;
					}//end valid
				else
					quantity = globalStatus->m_lastItemAmount;
			    CItemObj * invObj = globalStatus->m_itemManager->GetItemByID(glID);
			    if(invObj)
				{//valid
						CString quantityText = globalStatus->m_cabLib.ConvToString(quantity);
						CString outPut = operator+(invObj->m_itemName,operator+(" X", quantityText));
						globalStatus->AddStringToMenuList(  curLine->GetIntByParam(0),
															outPut,
															invObj->m_visualRef,
															glID,
															quantity,
															invObj->m_itemName,
															1,
															"");
				}
				else
					globalStatus->UI_Log("MENU_SCRIPTFUNC_DROPDRAGITEMTOLIST error:glid is not found");

				globalStatus->m_lastItemID = 0;
				globalStatus->m_lastItemAmount = 0;
				break;
			}
			case MENU_SCRIPTFUNC_INITARENAREG:
			{
				/*/ IF we're not in the client, then BREAK
				if (!globalStatus->m_multiplayerObject->GetEnabledClient_safe()) 
					break;

				// Get the menu ID
				int iMenuID = curLine->GetIntByParam(0);
				if (iMenuID <= 0) break;

				// Get a reference to the window
				CMenuWindow *pWindow = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMenuID, true));
				if (!pWindow) break;

				// Handle arena reg init by page
				POSITION posCurrent = NULL, posPrevious = NULL;
				switch (iMenuID)
				{
					case ID_ARENA_REGISTRATION_01_MENU:
					{
						CMenuToggle *pToggle = NULL;
						CMenuToggle *pSoloToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_SOLO_TOGGLE, true));
						CMenuToggle *pTeamToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_TEAM_TOGGLE, true));
						CMenuToggle *pTypeToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_TOGGLE, true));
						CMenuImage *pEliminationImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_ELIMINATION_SPLASH_IMAGE, true));
						CMenuImage *pDeathmatchImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_DEATHMATCH_SPLASH_IMAGE, true));
						CMenuButton *pNextButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REGISTRATION_NEXT_BUTTON, true));

						// DISABLE the navigation buttons
						if (pNextButton) pNextButton->SetEnabled(false);

						// HIDE the template arena TYPE toggle control
						if (pTypeToggle) pTypeToggle->m_visible = false;

						// HIDE the arena type splash images
						if (pEliminationImage) pEliminationImage->m_visible = false;
						if (pDeathmatchImage) pDeathmatchImage->m_visible = false;
						
						// LOOP through all the child controls of the open window
						for (posCurrent = pWindow->m_childList->GetHeadPosition(); posCurrent != NULL;)
						{
							// DELETE all toggle controls whose ID is > the template toggle control's ID
							posPrevious = posCurrent;
							pToggle = dynamic_cast<CMenuToggle*>(pWindow->m_childList->GetNext(posCurrent));

							if (pToggle && (pToggle->m_visualID > pTypeToggle->m_visualID))
							{
								pWindow->m_childList->RemoveAt(posPrevious);
								SafeDelete(pToggle);
							}
						}

						// INIT selected arena MODE
						//	and execute the toggle click event
						if (pSoloToggle) pSoloToggle->SetEnabled();
						if (pTeamToggle) pTeamToggle->SetEnabled();
						if (pSoloToggle->m_pData && pSoloToggle->m_clickScript) 
							pSoloToggle->m_clickScript->ExecuteScript();
						else if (pTeamToggle->m_pData && pTeamToggle->m_clickScript) 
							pTeamToggle->m_clickScript->ExecuteScript();
						else if (pSoloToggle->m_clickScript)
							pSoloToggle->m_clickScript->ExecuteScript();
						break;
					}
					case ID_ARENA_REGISTRATION_02_MENU:
					{
						// GET the client's entity
						POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
						if (!pos) break;
						CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);

						CMenuToggle *pToggle = NULL;
						CMenuButton *pPreviousButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REGISTRATION_PREVIOUS_BUTTON, true));
						CMenuButton *pJoinButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_JOIN_BUTTON, true));
						CMenuButton *pRefreshButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REFRESH_BUTTON, true));
						CMenuButton *pLeaveButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_LEAVE_BUTTON, true));
						
						// Disable the JOIN and LEAVE navigation button
						if (pJoinButton) pJoinButton->SetEnabled(false);
						if (pLeaveButton 
						&& pEntity->m_hasJoinedArena 
						&& pEntity->m_arenaBotID) 
							pLeaveButton->SetEnabled();
						else 
							pLeaveButton->SetEnabled(false);

						// Enable the PREVIOUS, REFRESH and CLOSE buttons
						if (pRefreshButton) pRefreshButton->SetEnabled();
						if (pPreviousButton) pPreviousButton->SetEnabled();

						// Loop through all the child controls of the open window
						for (posCurrent = pWindow->m_childList->GetHeadPosition(); posCurrent != NULL;)
						{
							// Disable all toggle controls
							pToggle = dynamic_cast<CMenuToggle*>(pWindow->m_childList->GetNext(posCurrent));
							if (pToggle)
								pToggle->SetEnabled(false);
						}
						break;
					}
					default: break;
				}*/
				break;
			}
			case MENU_SCRIPTFUNC_ENABLEMENUBUTTON:
			{
				// Parse script params
				bool bEnabled = false;
				int iButtonID = curLine->GetIntByParam(0);
				CString strEnabled = curLine->GetStrByParam(1);
				if (strEnabled.Find("true") != -1) 
					bEnabled = true;
				if (iButtonID <= 0) break;

				// Get references to the menu visuals
				CMenuButton *pButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iButtonID, true));
				if (!pButton) break;

				// Enable/Disable menu visuals
				pButton->SetEnabled(bEnabled);
				break;
			}
			case MENU_SCRIPTFUNC_SETGROUPEDTOGGLE:
			{
				// Get visual IDs
				int iMenuID = curLine->GetIntByParam(0);
				int iToggleID = curLine->GetIntByParam(1);
				if((iMenuID <= 0) || (iToggleID <= 0)) break;

				// Get references to the menu visuals
				CMenuWindow *pWindow = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMenuID, true));
				CMenuToggle *pToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iToggleID, true));
				if (!pWindow || !pToggle) break;

				if (!pToggle->m_isSet) 
				{
					pToggle->m_isSet = true;
					//break;
				}

				// Unset all toggles in the same group as the newly selected toggle
				POSITION pos = NULL;
				CMenuToggle *pOther = NULL;
				for (pos = pWindow->m_childList->GetHeadPosition(); pos != NULL;)
				{
					pOther = dynamic_cast<CMenuToggle*>(pWindow->m_childList->GetNext(pos));
					if (!pOther) 
						continue;
					else if ((pOther != pToggle) && (pOther->m_iGroupID == pToggle->m_iGroupID))
						pOther->m_isSet = false;
				}
				break;
			}
			case MENU_SCRIPTFUNC_REQUESTARENATYPES:
			{
				// Get the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if (!pos) break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				
				// Request available arena types (CLIENT ONLY)
				// NOTE: This is an asynchronous call, so we're done after the request
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
				{
					/*
					globalStatus->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						SCMD_REQUESTARENATYPES,
						NULL,
						pEntity->List_networkDefinedID,
						NULL,
						TMC_COMMAND);
						*/
					globalStatus->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						pEntity->List_networkDefinedID,
						NULL,
						TMC_COMMAND,
						NULL,
						SCMD_REQUESTARENATYPES);
				}
				break;
			}
			case MENU_SCRIPTFUNC_REQUESTARENAINFO:
			{
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if (!pos) break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);

				// GET references to menu visuals
				CMenuWindow *pParentMenu01 = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ARENA_REGISTRATION_01_MENU, true));
				CMenuWindow *pParentMenu02 = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ARENA_REGISTRATION_02_MENU, true));
			
				// ERROR checking
				if (!pParentMenu01 && !pParentMenu02)
					break;

				// SETUP for determining selected arena MODE and TYPE
				CString strParams;
				pos = NULL;

				// IF the first registration menu is open...
				if (pParentMenu01)
				{		
					// GET references to menu visuals
					CMenuToggle *pToggle = NULL;
					CMenuToggle *pSoloArenasToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_SOLO_TOGGLE, true));
					CMenuToggle *pTeamArenasToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_TEAM_TOGGLE, true));
					CMenuToggle *pTypeToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_TOGGLE, true));

					// Get the arena MODE 				
					if (pSoloArenasToggle->m_isSet) pToggle = pSoloArenasToggle;
					else pToggle = pTeamArenasToggle;
					strParams = pToggle->m_strValue + " ";

					// Get the arena TYPE
					// Loop through all the parent window's child controls
					// Find the toggle with the same Group ID as the template TYPE toggle control
					// Find the one that's set and you've got your selected arena TYPE
					for (pos = pParentMenu01->m_childList->GetHeadPosition(); pos != NULL;)
					{
						pToggle = dynamic_cast<CMenuToggle*>(pParentMenu01->m_childList->GetNext(pos));
						if (pToggle 
						&& pToggle->m_isSet
						&& (pToggle->m_iGroupID == pTypeToggle->m_iGroupID))
						{
							strParams += pToggle->m_strValue;
							break;
						}
					}
				}
				// ELSE, if the the second registration screen is open...
				else if (pParentMenu02)
				{
					// GET references to menu visuals
					CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MAPS_LIST, true));

					// ERROR checking
					if (!pListbox 
					|| !pListbox->m_pFirstItem
					|| !pListbox->m_pFirstItem->m_pData) 
						break;

					// APPEND the arena MODE and TYPE params
					ArenaInfoEX *pArena = (ArenaInfoEX*)(pListbox->m_pFirstItem->m_pData);
					if (!pArena) break;
					strParams = globalStatus->m_cabLib.ConvToString(pArena->Mode);
					strParams += " ";
					strParams += globalStatus->m_cabLib.ConvToString(pArena->Type);
				}

				// Request arena info (CLIENT ONLY)
				// NOTE: This is an asynchronous call, so we're done after the request
				//	HEADER (x1)
				//		-> Category (char)
				//		-> Player's network assigned ID (short)
				//	PACKET ID (x1)
				//		-> Packet type ID (char)
				//	ARENA INFO (xN, where N is the number of arenas available via this arena bot)
				//		-> Arena Index (unsigned short)
				//		-> Arena Mode (unsigned char)
				//		-> Arena Type (unsigned char)
				//		-> Arena Name Length (unsigned int)
				//		-> Arena Name (char x [Arena Name Length])
				//		-> Arena World Name Length (unsigned int)
				//		-> Arena World Name (char x [Arena World Name Length])
				//		-> Arena Min Players (unsigned short)
				//		-> Arena Max Players (unsigned short)
				//		-> Arena Current Players (unsigned short)
				//		-> Arena Time Limit, in ms (unsigned int)
				//		-> Time Remaining Until Arena Autostart, in ms (unsigned int)
				//		-> Arena Num Teams (unsigned char)
				//		-> Arena Players Per Team (unsigned short x [Arena Num Teams])
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
				{
					/*
					globalStatus->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						SCMD_REQUESTARENAINFO,
						strParams.GetBuffer(),
						pEntity->List_networkDefinedID,
						NULL,
						TMC_COMMAND);
						*/
					globalStatus->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						pEntity->List_networkDefinedID,
						NULL,
						TMC_COMMAND,
						strParams.GetBuffer(),
						SCMD_REQUESTARENAINFO);
					strParams.ReleaseBuffer();
				}
				break;
			}
			case MENU_SCRIPTFUNC_SHOWARENATYPESBYMODE:
			{
				// Get references to menu visuals
				CMenuWindow *pParentMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ARENA_REGISTRATION_01_MENU, true));
				CMenuToggle *pSoloArenasToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_SOLO_TOGGLE, true));
				CMenuToggle *pTeamArenasToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_TEAM_TOGGLE, true));
				CMenuToggle *pTypeToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_TOGGLE, true));
				CMenuButton *pNextButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REGISTRATION_NEXT_BUTTON, true));
				CMenuText *pArenaDescriptionText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_DESCRIPTION_TEXT, true));

				// HIDE all arena TYPE splash images
				POSITION pos = NULL;
				CMenuImage *pImage = NULL;
				for (pos = pParentMenu->m_childList->GetHeadPosition(); pos != NULL;)
				{
					pImage = dynamic_cast<CMenuImage*>(pParentMenu->m_childList->GetNext(pos));
					if (!pImage) continue;
					switch (pImage->m_visualID)
					{
						case ARENA_TYPE_ELIMINATION_SPLASH_IMAGE:
						case ARENA_TYPE_DEATHMATCH_SPLASH_IMAGE:
						case ARENA_TYPE_DOMINATION_SPLASH_IMAGE:
						{
							pImage->m_visible = false;
							break;
						}
						default: break;
					}
				}

				// Error checking
				if (!pParentMenu
				|| !pSoloArenasToggle 
				|| !pTeamArenasToggle 
				|| !pTypeToggle
				|| !pNextButton
				|| !pArenaDescriptionText) 
				{
					// TRY TO DISABLE the next button if we failed to load any visuals
					if (pNextButton) 
						pNextButton->SetEnabled(false);

					// TRY TO SET a default arena description message
					if (pArenaDescriptionText)
						pArenaDescriptionText->SetText(ARENA_TYPES_DESCRIPTIONS_MAP[ARENATYPE_UNSPECIFIED]);

					// TRY TO SET a load fail message on the template toggle
					if (pTypeToggle)
					{
						pTypeToggle->SetText(ARENA_TYPES_MAP[ARENATYPE_UNSPECIFIED]);
						pTypeToggle->m_visible = true;
						pTypeToggle->m_enabled = false;
					}
					break;
				}
				else 
					// Hide the template toggle
					pTypeToggle->m_visible = false;

				// Set the arena MODE toggle values
				pSoloArenasToggle->m_strValue = globalStatus->m_cabLib.ConvToString((int)ARENAMODE_SOLO);
				pTeamArenasToggle->m_strValue = globalStatus->m_cabLib.ConvToString((int)ARENAMODE_TEAM);

				// Loop through all the child controls of the open window
				POSITION posCurrent = NULL, posPrevious = NULL;
				CMenuToggle *pToggle = NULL;
				for (posCurrent = pParentMenu->m_childList->GetHeadPosition(); posCurrent != NULL;)
				{					
					// Remove all toggle controls whose ID is > the template toggle control's ID
					posPrevious = posCurrent;
					pToggle = dynamic_cast<CMenuToggle*>(pParentMenu->m_childList->GetNext(posCurrent));
					if (pToggle 
					&& (pToggle->m_visualID != ARENA_MODE_SOLO_TOGGLE)
					&& (pToggle->m_visualID != ARENA_MODE_TEAM_TOGGLE)
					&& (pToggle->m_visualID != ARENA_TYPE_TOGGLE))
					{
						pParentMenu->m_childList->RemoveAt(posPrevious);
						SafeDelete(pToggle);
					}
				}

				// Let's make sure the MODE toggle controls have their arena TYPE data
				vector<ArenaInfo*> *pArenaTypes = NULL;
				vector<ArenaInfo*> *SoloArenas = NULL, *TeamArenas = NULL;
				if (pSoloArenasToggle->m_isSet && pSoloArenasToggle->m_pData)
				{
					SoloArenas = reinterpret_cast<vector<ArenaInfo*>* >(pSoloArenasToggle->m_pData);
					pArenaTypes = SoloArenas;
				}
				else if (pTeamArenasToggle->m_isSet && pTeamArenasToggle->m_pData)
				{
					TeamArenas = reinterpret_cast<vector<ArenaInfo*>* >(pTeamArenasToggle->m_pData);
					pArenaTypes = TeamArenas;
				}
				// What?! No arena TYPEs for either arena MODE? Bye!
				else
				{
					// Disable the next button
					pNextButton->SetEnabled(false);

					// Set a load fail message on the template toggle
					pTypeToggle->SetText(ARENA_TYPES_MAP[ARENATYPE_UNSPECIFIED]);
					pTypeToggle->m_visible = true;
					pTypeToggle->m_enabled = false;

					// CLEAR the arena type description text
					pArenaDescriptionText->SetText("");
					break;
				}

				// SETUP for dynamically instantiating the new CMenuToggle controls
				CMenuScript *pClickScript = NULL;
				vector<ArenaInfo*>::iterator iter;
				map<ARENA_TYPES, CString>::const_iterator c_iter;
				CString strArenaName;
				int iCount = 0; 
				const int VERTICAL_SPACING = 30;

				// CREATE CMenuToggle controls to display all arena TYPEs 
				//	for the selected arena MODE
				if (pArenaTypes)
				{
					for(iter = pArenaTypes->begin(); iter != pArenaTypes->end(); iter++)
					{
						// RETRIEVE next ArenaInfo object
						ArenaInfo *pInfo = static_cast<ArenaInfo*>(*iter);

						// CLONE new CMenuToggle from the template toggle control
						pToggle = new CMenuToggle();
						pTypeToggle->Copy((CMenuVisual*)pToggle, globalStatus->m_3dDevice);
						pToggle->m_visible = true;
						pToggle->m_enabled = true;

						// ASSIGN a free visual ID
						// NOTE: Since these are temp controls, just give them the biggest IDs possible
						//	to ensure there are no conflicts with visual IDS.
						pToggle->m_visualID = INT_MAX - iCount;

						// ASSIGN the toggle group ID
						pToggle->m_iGroupID = pTypeToggle->m_iGroupID;

						// ASSIGN control name
						pToggle->m_name = "ARENA_TYPE_TOGGLE_" + globalStatus->m_cabLib.ConvToString(pToggle->m_visualID);

						// SET display text
						strArenaName = ARENA_TYPES_MAP[pInfo->Type];
						if (pSoloArenasToggle->m_isSet)
							strArenaName.Insert(0, "Solo ");
						else
							strArenaName.Insert(0, "Team ");
						pToggle->m_pDisplayText->SetText(strArenaName.GetBuffer());
						strArenaName.ReleaseBuffer();

						// SET toggle value (arena TYPE enum value)
						pToggle->m_strValue = globalStatus->m_cabLib.ConvToString(pInfo->Type);

						// SET position
						pToggle->m_posX = pTypeToggle->m_posX;
						pToggle->m_posY = pTypeToggle->m_posY - (iCount * VERTICAL_SPACING);

						// SET dimensions
						pToggle->m_width = pTypeToggle->m_width;
						pToggle->m_height = pTypeToggle->m_height;

						// INIT control selection and enabled state
						if (iCount == 0) pToggle->m_isSet = true;
						else pToggle->m_isSet = false;
						pToggle->SetEnabled();

						// ADD new toggle control to the parent menu's list of children controls
						pParentMenu->m_childList->AddHead(pToggle);

						// CREATE the click script string (SetGroupedToggle)
						CString strClickScript = "SetGroupedToggle(";
						strClickScript += globalStatus->m_cabLib.ConvToString(ID_ARENA_REGISTRATION_01_MENU); // Param 1: Parent visual ID
						strClickScript += ",";
						strClickScript += globalStatus->m_cabLib.ConvToString(pToggle->m_visualID); // Param 2: Toggle visual ID
						strClickScript += ")";

						// ADD the SetGroupedToggle click script
						if(!pToggle->m_clickScript) 
							pToggle->m_clickScript = new CMenuScript();
						pToggle->m_clickScript->ClearScript();
						pToggle->m_clickScript->AddLine(strClickScript.GetBuffer());
						strClickScript.ReleaseBuffer();

						// CREATE the click script string (UpdateArenaTypeDescription)
						strClickScript.Empty();
						strClickScript = "UpdateArenaTypeDescription(";
						strClickScript += globalStatus->m_cabLib.ConvToString(pToggle->m_visualID); // Param 1: Toggle visual ID
						strClickScript += ")";

						// ADD the UpdateArenaTypeDescription click script
						pToggle->m_clickScript->AddLine(strClickScript.GetBuffer());
						strClickScript.ReleaseBuffer();

						// IF this is the first instantiated toggle control
						//	INIT the menu by selecting it
						if (iter == pArenaTypes->begin())
						{
							pToggle->m_clickScript->ExecuteScript();
						}

						// INCREMENT the controls count var
						iCount++;
					}
				}

				// Enable the NEXT button
				pNextButton->SetEnabled();
				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYSELECTEDARENAINFO:
			{
				// Get references to menu visuals
				int iListboxID = curLine->GetIntByParam(0);
				CMenuWindow *pWindow = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ARENA_REGISTRATION_02_MENU, true));
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iListboxID, true));
				CMenuButton *pJoinButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_JOIN_BUTTON, true));
				CMenuText *pCountdownText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_AUTOSTART_COUNTDOWN_TEXT, true));
				CMenuText *pMinmaxSizeText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MINMAX_SIZE_TEXT, true));
				CMenuText *pCurrentSizeText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_CURRENT_SIZE_TEXT, true));
				CMenuText *pCurrentBetPoolText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_CURRENT_BET_POOL_TEXT, true));

				// Error checking
				if (!pListbox
				|| !pJoinButton
				|| !pCountdownText
				|| !pMinmaxSizeText
				|| !pCurrentSizeText
				|| !pCurrentBetPoolText
				|| !pListbox->m_currentSelection 
				|| !pListbox->m_currentSelection->m_pData)
				{
					// DISABLE the JOIN ARENA button
					if (pJoinButton) 
						pJoinButton->SetEnabled(false);
					break;
				}

				// RETRIEVE the arena info data
				ArenaInfoEX *pArenaInfo = (ArenaInfoEX*)(pListbox->m_currentSelection->m_pData);
				if (!pArenaInfo)
				{
					// DISABLE the JOIN ARENA button
					if (pJoinButton) 
						pJoinButton->SetEnabled(false);
					break;
				}

				// SETUP for updating the arena map image
				map<CString, int>::const_iterator iter;
				CMenuImage *pArenaImage = NULL;

				// HIDE all arena map images
				for (iter = ArenaImageMap.begin(); iter != ArenaImageMap.end(); iter++)
				{
					pArenaImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iter->second, true));
					if (pArenaImage)
						pArenaImage->m_visible = false;
				}

				// UPDATE the arena map image
				iter = ArenaImageMap.find(pArenaInfo->WorldName.MakeLower());
				if (iter != ArenaImageMap.end())
				{
					pArenaImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iter->second, true));
					if (pArenaImage)
						pArenaImage->m_visible = true;
				}

				// UPDATE min/max players info
				CString strResult;
				strResult = globalStatus->m_cabLib.ConvToString(pArenaInfo->NumMinPlayers);
				strResult += "/";
				strResult += globalStatus->m_cabLib.ConvToString(pArenaInfo->NumMaxPlayers);
				pMinmaxSizeText->SetText(strResult.GetBuffer());
				strResult.ReleaseBuffer();
				strResult.Empty();

				// UPDATE current players info
				strResult = globalStatus->m_cabLib.ConvToString(pArenaInfo->NumCurrentPlayers);
				pCurrentSizeText->SetText(strResult.GetBuffer());
				strResult.ReleaseBuffer();
				strResult.Empty();

				// UPDATE current bet pool info
				strResult = globalStatus->m_cabLib.ConvToString(pArenaInfo->BetPool);
				strResult.Append(" Credits");
				pCurrentBetPoolText->SetText(strResult.GetBuffer());
				strResult.ReleaseBuffer();
				strResult.Empty();				

				// INIT toggle controls to disabled
				POSITION pos = NULL;
				CMenuToggle *pToggle = NULL;
				for (pos = pWindow->m_childList->GetHeadPosition(); pos != NULL;)
				{
					// DISABLE all toggle controls
					pToggle = dynamic_cast<CMenuToggle*>(pWindow->m_childList->GetNext(pos));
					if (pToggle)
					{
						if (pToggle->m_visualID == ARENA_BET_TOGGLE)
							pToggle->SetEnabled();
						else
						{
							pToggle->SetEnabled(false);

							// SETUP for updating the toggle display text
							// NOTE: This strategy assumes the display text to be in the following format =>
							//	"Team # (00)"
							CString strText = pToggle->m_pDisplayText->m_text.Mid(0, 1);
							strText.Append(" (00)");

							// SET the toggle display text
							pToggle->SetText(strText.GetBuffer());
							strText.ReleaseBuffer();
						}
					}
				}

				// CHECK for teams and UPDATE the teams display
				if (pArenaInfo->NumTeams > 0)
				{
					for (pos = pWindow->m_childList->GetHeadPosition(); pos != NULL;)
					{
						// ENABLE the toggle controls whose value is <= the number of teams
						pToggle = dynamic_cast<CMenuToggle*>(pWindow->m_childList->GetNext(pos));
						if (pToggle)
						{
							int iIndex = ::atoi(pToggle->m_strValue.GetBuffer());
							--iIndex; // NOTE: The toggle team control values are off by +1, hence this decrement
							if (iIndex < pArenaInfo->NumTeams)
							{
								pToggle->SetEnabled();

								// SETUP for updating the toggle display text
								// NOTE: This strategy assumes the display text to be in the following format =>
								//	"Team # (##)"
								//	UPDATE: New format is now -> # (##)
								CString strText = pToggle->m_pDisplayText->m_text.Mid(0, 1);
								strText.Append("(");

								// GET the number of players on this team
								int iNumPlayers = pArenaInfo->NumPlayersOnTeam[iIndex];
								if (iNumPlayers < 10)
									strText.Append("0");
								strText.Append(globalStatus->m_cabLib.ConvToString(iNumPlayers));
								strText.Append(")");

								// SET the toggle display text
								pToggle->SetText(strText.GetBuffer());
								strText.ReleaseBuffer();
							}

							// INIT the first toggle control's selected state
							if (iIndex == 0)
								pToggle->m_isSet = true;
						}
					}
				}
				
				// GET the client's entity
				CMovementObj *pEntity = NULL;
				pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if (pos)
					pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				
				// ENABLE the JOIN ARENA button if this player has not already joined one
				if (pEntity 
				/*&& !pEntity->m_hasJoinedArena*/
				&& (pEntity->m_arenaBotID <= 0))
					pJoinButton->SetEnabled();

				// DEBUG: TEMP WRITE FILE!
				/*globalStatus->m_axLogger.WriteDebugFile("\nDisplaySelectedArenaInfo() =====", 0, 0);
				globalStatus->m_axLogger.WriteDebugFile("\nARENA BOT ID: ", 0, 0);
				globalStatus->m_axLogger.WriteDebugFile(globalStatus->m_cabLib.ConvToString(pEntity->m_arenaBotID).GetBuffer(), 0, 0);
				globalStatus->m_axLogger.WriteDebugFile("\nHAS JOINED ARENA? ", 0, 0);
				if (pEntity->m_hasJoinedArena)
					globalStatus->m_axLogger.WriteDebugFile("TRUE", 0, 1);
				else
					globalStatus->m_axLogger.WriteDebugFile("FALSE", 0, 1);*/

				
				// NOTE: We have an intentional fallthrough here!
				//	This is done to prevent having arena autostart 
				//	countdown logic included in this code block as well.
				//break;
			}
			case MENU_SCRIPTFUNC_UPDATEARENACOUNTDOWN:
			{
				int iListboxID = curLine->GetIntByParam(0);
				int iTextID = curLine->GetIntByParam(1);

				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iListboxID, true));
				CMenuText *pText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iTextID, true));

				// Error checking
				if (!pText
				|| !pListbox
				|| !pListbox->m_currentSelection 
				|| !pListbox->m_currentSelection->m_pData)
				{
					break;
				}

				// RETRIEVE the arena info data
				CString strResult;
				ArenaInfoEX *pArenaInfo = (ArenaInfoEX*)(pListbox->m_currentSelection->m_pData);
				if (!pArenaInfo) break;

				// Calculate time remaining, in ms
				int iTimeElapsed = ::GetTickCount() - pArenaInfo->TimeReceived;
				int iTimeRemaining = pArenaInfo->TimeRemainingForAutostart - iTimeElapsed;

				// CONVERT time to [mm:ss] ready values
				int iMinutes = iTimeRemaining / 60000;
				iTimeRemaining %= 60000;
				int iSeconds = iTimeRemaining / 1000;

				// IF less than the min number of players registered for arena autostart...
				if (pArenaInfo->NumCurrentPlayers < pArenaInfo->NumMinPlayers)
				{
					strResult = "--:--";
				}
				// ELSE IF less than a minute remains...
				else if ((iMinutes <= 0) && (iSeconds <= 59))
				{
					strResult = "< 1 min";
				}
				// ELSE do a regular update of the timer display in [mm:ss] format
				else
				{
					if (iMinutes < 10) strResult += "0";
					strResult += globalStatus->m_cabLib.ConvToString(iMinutes);
					strResult += ":";
					if (iSeconds < 10) strResult += "0";
					strResult += globalStatus->m_cabLib.ConvToString(iSeconds);
				}
				pText->SetText(strResult.GetBuffer());
				strResult.ReleaseBuffer();
				strResult.Empty();
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEARENATYPEDESCRIPTION:
			{
				// GET references to menu visuals
				int iToggleID = curLine->GetIntByParam(0);
				CMenuImage *pArenaTypeSplashImage = NULL;
				CMenuToggle *pToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iToggleID, true));
				CMenuText *pText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_DESCRIPTION_TEXT, true));

				// ERROR checking: Did we get references to all the menu visuals?
				if (!pToggle || !pText) 
				{
					// One last try to display an error description...
					if (pText) 
					{
						pText->m_text.Empty(); // Out with the old text...
						pText->SetText(ARENA_TYPES_DESCRIPTIONS_MAP[ARENATYPE_UNSPECIFIED]); // ...in with the new text!
					}
					break;
				}

				// SET all arena TYPE splash images to invisible
				for (int i=0; i<ARENATYPE_MAX; i++)
				{
					CMenuImage *pArenaTypeSplashImage = dynamic_cast<CMenuImage*>(
						globalStatus->m_runtimeMenuDatabase->GetVisualByID(
						ARENATYPES_SPLASH_IMAGES_ID_LOOKUP[i], true));
					if (pArenaTypeSplashImage)
					{
						pArenaTypeSplashImage->m_visible = false;
					}
				}

				// SET the textbox text to the correct arena type description
				ARENA_TYPES eType = (ARENA_TYPES)::atoi(pToggle->m_strValue);

				// ERROR checking: Do we have a valid ARENATYPES value?
				if ((eType > ARENATYPE_MAX) 
				|| (eType < ARENATYPE_UNSPECIFIED))
					eType = ARENATYPE_UNSPECIFIED;

				LPTSTR strDescription = ARENA_TYPES_DESCRIPTIONS_MAP[eType];
				pText->m_text.Empty(); // Out with the old text...
				pText->SetText(strDescription); // ...in with the new text!

				// SET the correct arena TYPE splash image to visible
				pArenaTypeSplashImage = dynamic_cast<CMenuImage*>(
					globalStatus->m_runtimeMenuDatabase->GetVisualByID(
					ARENATYPES_SPLASH_IMAGES_ID_LOOKUP[eType], true));
				if (pArenaTypeSplashImage)
					pArenaTypeSplashImage->m_visible = true;
				break;
			}
			case MENU_SCRIPTFUNC_JOINARENA:
			{
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if (!pos) break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if (!pEntity) break;

				// GET references to menu visuals
				CMenuWindow *pWindow = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ARENA_REGISTRATION_02_MENU, true));
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MAPS_LIST, true));
				CMenuToggle *pBetToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BET_TOGGLE, true));

				// ERROR checking
				if (!pWindow 
				|| !pListbox
				|| !pListbox->m_currentSelection
				|| !pListbox->m_currentSelection->m_pData
				|| !pBetToggle)
					break;
			
				// SETUP for creating the request packet
				pos = NULL;
				int iTeamIndex = 0;
				CMenuToggle *pToggle = NULL;

				// Get the selected arena
				ArenaInfoEX *pArenaInfo = (ArenaInfoEX*)pListbox->m_currentSelection->m_pData;

				if (pArenaInfo->NumTeams > 0)
				{
					// Determine the selected team index
					for (pos = pWindow->m_childList->GetHeadPosition(); pos != NULL;)
					{
						pToggle = dynamic_cast<CMenuToggle*>(pWindow->m_childList->GetNext(pos));
						if (pToggle && pToggle->m_isSet)
						{
							iTeamIndex = ::atoi(pToggle->m_strValue.GetBuffer());
							pToggle->m_strValue.ReleaseBuffer();
						}
					}
				}

				// ENCODE stored arena bot ID 
				//	NOTE: Indicates if this player is registered for another arena
				CString strParams;
				strParams.Append(globalStatus->m_cabLib.ConvToString(pEntity->m_arenaBotID));
				strParams.Append(" ");

				// ENCODE selected arena index
				strParams.Append(globalStatus->m_cabLib.ConvToString(pArenaInfo->Index));
				strParams.Append(" ");

				// ENCODE selected arena team index
				strParams.Append(globalStatus->m_cabLib.ConvToString(iTeamIndex));
				strParams.Append(" ");

				// ENCODE bet amount
				const int BET_AMOUNT = 100;
				int iBetAmount = 0;
				if (pBetToggle->m_isSet)
					iBetAmount = BET_AMOUNT;
				strParams.Append(globalStatus->m_cabLib.ConvToString(iBetAmount));

				// REQUEST arena join (CLIENT ONLY)
				// NOTE: This is an asynchronous call, so we're done after the request
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
				{
					globalStatus->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						pEntity->List_networkDefinedID,
						NULL,
						TMC_COMMAND,
						strParams.GetBuffer(),
						SCMD_REQUESTARENAJOIN);
					strParams.ReleaseBuffer();
				}
				break;
			}
			case MENU_SCRIPTFUNC_SHOWIMAGE:
			{
				// PARSE script params
				bool bVisible = true;
				int iImageID = curLine->GetIntByParam(0);
				CString strVisible = curLine->GetStrByParam(1);

				if (strVisible.MakeLower() == "true") bVisible = true;
				else bVisible = false;

				// GET reference to menu visual
				CMenuImage *pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iImageID, true));
				if (!pImage) break;

				// SET the image visibility state
				if (bVisible)
					pImage->m_visible = true;
				else
					pImage->m_visible = false;
				break;
			}
			case MENU_SCRIPTFUNC_UPDATERESISTIMAGE:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Set up our values
				int iChannel = curLine->GetIntByParam(0);
				int value = 0;
				CMenuImage *plowVis = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true));
				CMenuImage *pmidVis = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(2), true));
				CMenuImage *phiVis = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(3), true));
				//Retrieve the player
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Search for the players appropriate resistance value
				if(!globalStatus->m_multiplayerObject->m_elementStatus)
					break;
				for(POSITION posLoc2 = globalStatus->m_multiplayerObject->m_elementStatus->GetHeadPosition(); posLoc2 != NULL;)
				{
					CElementStatusObj *eleObject = (CElementStatusObj *)globalStatus->m_multiplayerObject->m_elementStatus->GetNext(posLoc2);
					if(eleObject->m_elementIndex == iChannel)
						value = (int)eleObject->m_currentValue;
				}

				//Check to see if its on the low end
				if(plowVis)
				{
					if(value < 30)
						plowVis->m_visible = true;
					else
						plowVis->m_visible = false;
				}
				//Check to see if its on the mid grade
				if(pmidVis)
				{
					if(value > 29 && value < 61)
						pmidVis->m_visible = true;
					else
						pmidVis->m_visible = false;
				}
				//Check to see if its on the high end
				if(phiVis)
				{
					if(value > 60)
						phiVis->m_visible = true;
					else
						phiVis->m_visible = false;
				}

				break;
			}
			case MENU_SCRIPTFUNC_REQUESTARENALEAVE:
			{
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if (!pos) break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);

				// REQUEST to leave an arena (CLIENT ONLY)
				// NOTE: This is an asynchronous call, so we're done after the request
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
				{
					globalStatus->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						pEntity->List_networkDefinedID,
						NULL,
						TMC_COMMAND,
						NULL,
						SCMD_REQUESTARENALEAVE);
				}
				break;
			}
			case MENU_SCRIPTFUNC_EVALUATELEVEL:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				//Retrieve the player
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);
				if (!posLoc) break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(posLoc);

				int iTextID = curLine->GetIntByParam(0);
				CMenuText *pText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iTextID, true));
				if(pText)
				{
					pText->m_text.Empty(); // Out with the old text...
					pText->SetText(globalStatus->m_cabLib.ConvToString(pEntity->m_level)); // ...in with the new text!
				}
				break;
			}
			case MENU_SCRIPTFUNC_EVALUATEXP:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				//Retrieve the player
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);
				if (!posLoc) break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(posLoc);

				int iTextID = curLine->GetIntByParam(0);
				CMenuText *pText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iTextID, true));
				if(pText)
				{
					pText->m_text.Empty(); // Out with the old text...
					CString newString = globalStatus->m_cabLib.ConvToString(pEntity->m_curXP) + " / " + globalStatus->m_cabLib.ConvToString(pEntity->m_nextLvlXP) + "  (" + globalStatus->m_cabLib.ConvToString(pEntity->m_percXP) + "%)";
					pText->SetText(newString); // ...in with the new text!
				}
				
				break;
			}
			case MENU_SCRIPTFUNC_XPPROGRESS:
			{
				//Make sure this is an enabled client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				//Create position and Visual Reference
				CMenuProgressBar *visual = (CMenuProgressBar*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				POSITION posLoc = globalStatus->m_entObjRuntime->FindIndex(0);

				//Make sure there is a valid posLoc and Visual Reference
				if(!visual || !posLoc)
					break;

				//Set Text to Health Ratio
				CMovementObj * ptrLoc = ( CMovementObj* ) globalStatus->m_entObjRuntime->GetAt(posLoc);
				visual->m_curPercentage = (float)ptrLoc->m_percXP / 100.0f;
				
				break;
			}
			case MENU_SCRIPTFUNC_INVITETOGROUP:
			{
				// PARSE script params
				int iListboxID = curLine->GetIntByParam(0);

				// GET references to menu visuals
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iListboxID, true));
				if(!pListbox || !pListbox->m_currentSelection)
					break;

				// GET the player's name
				CMenuText *pPlayerName = dynamic_cast<CMenuText*>(pListbox->m_currentSelection->m_textList.GetHead());
				if (!pPlayerName) break;

				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if (!pos) break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if (!pEntity || (pEntity->List_controlType != 0)) break;

				// SEND party invite server command
				CString strCommand = "/INVITETOGROUP " + pPlayerName->m_text;
				globalStatus->SendChatString(strCommand);
				break;
			}
			case MENU_SCRIPTFUNC_INVITETOSQUAD:
			{
				// PARSE script params
				int iListboxID = curLine->GetIntByParam(0);

				// GET references to menu visuals
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iListboxID, true));
				CMenuTextBox *pEditbox = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iListboxID, true));
				if(!pEditbox)
				{
					if(!pListbox || !pListbox->m_currentSelection)
						break;

					// GET the player's name
					CMenuText *pPlayerName = dynamic_cast<CMenuText*>(pListbox->m_currentSelection->m_textList.GetHead());
					if (!pPlayerName) break;

					// GET the client's entity
					POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
					if (!pos) break;
					CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
					if (!pEntity || (pEntity->List_controlType != 0)) break;

					// SEND party invite server command
					CString strCommand = "/INVITETOSQUAD " + pPlayerName->m_text;
					globalStatus->SendChatString(strCommand);
				}
				else
				{
					// GET the client's entity
					POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
					if (!pos) break;
					CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
					if (!pEntity || (pEntity->List_controlType != 0)) break;

					// SEND party invite server command
					CString strCommand = "/INVITETOSQUAD " + pEditbox->m_fullText;
					globalStatus->SendChatString(strCommand);
				}

				break;
			}
			case MENU_SCRIPTFUNC_REMOVEFROMSQUAD:
			{
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);

				CMenuListBox *pListBox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pListBox || !pListBox->m_currentSelection)
					break;
				
				CMenuText *text = (CMenuText *)pListBox->m_currentSelection->m_textList.GetHead();
				if(!text->m_text)
					break;

				CString svrRemoveBuild = "/REMOVEFROMSQUAD " + text->m_text;
				// Remove Entity
				if(pEntity)
					if(pEntity->List_controlType == 0)
						globalStatus->SendChatString(svrRemoveBuild);

				break;
			}
			case MENU_SCRIPTFUNC_LEAVESQUAD:
			{
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos) break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if (!pEntity || (pEntity->List_controlType != 0)) break;

				// LEAVE the squad
				globalStatus->SendChatString("/LEAVESQUAD");

				// CLOSE the squad status menu
				globalStatus->m_runtimeMenuDatabase->CloseByID(ID_SQUAD_STATUS_MENU, &globalStatus->m_showMenuCursor);
				break;
			}
			case MENU_SCRIPTFUNC_MAKELEADEROFSQUAD:
			{
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);

				CMenuListBox *pListBox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pListBox || !pListBox->m_currentSelection)
					break;

				CMenuText *text = (CMenuText *)pListBox->m_currentSelection->m_textList.GetHead();
				if(!text->m_text)
					break;

				// Make entity a leader
				CString svrLeaderBuild = "/MAKELEADEROFSQUAD " + text->m_text;
				// Remove Entity
				if(pEntity)
					if(pEntity->List_controlType == 0)
						globalStatus->SendChatString(svrLeaderBuild);

				break;
			}
			case MENU_SCRIPTFUNC_INITTRADE:
			{
				// PARSE script params
				int iListboxID = curLine->GetIntByParam(0);

				// GET the client side player object
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if (!pos) break;
				CMovementObj *pEntity = dynamic_cast<CMovementObj*>(globalStatus->m_entObjRuntime->GetNext(pos));
				if (!pEntity) break;

				// GET references to menu visuals
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iListboxID, true));
				if (!pListbox || !pListbox->m_currentSelection) break;
				CMenuText *pText = dynamic_cast<CMenuText*>(pListbox->m_currentSelection->m_textList.GetHead());
				if (!pText) break;
				
				// IF we're in the game client, SEND the Trade Request server command
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					globalStatus->m_netGameLogicObject->SendTextMsg(
						DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						pEntity->List_networkDefinedID,
						NULL,
						TMC_COMMAND,
						pText->m_text,
						SCMD_TRADE);
				break;
				/*
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);

				CMenuListBox *pListBox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pListBox || !pListBox->m_currentSelection)
					break;
				
				CMenuText *text = (CMenuText *)pListBox->m_currentSelection->m_textList.GetHead();
				if(!text->m_text)
					break;
				
				globalStatus->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																pEntity->List_networkDefinedID,
																NULL,
																TMC_COMMAND,
																text->m_text,
																SCMD_TRADE);
				
				break;*/
			}
			case MENU_SCRIPTFUNC_REQUESTBADGES:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pEntity)
					break;

				//Reset Badges before requesting new update
				for(int i = 0; i < BADGE_TRACKING; i++)
					pEntity->m_badges[i] = 0;

				//Send Attrib Message for Badges
				globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,
																	63,
																	0,
																	pEntity->List_networkDefinedID,
																	0,
																	0, 
																	NULL);//request badge inventory
				
				break;
			}
			case MENU_SCRIPTFUNC_SHOWBADGE:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				int glID = curLine->GetIntByParam(0);
				CMenuImage *pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true));
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				//Make sure we found the items were looking for
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pEntity || !pImage)
					break;

				//set visible state to false by default
				pImage->m_visible = false;

				//Check the ID tracking
				Emblem *pEmblem = NULL;
				CMenuImage *pEmblemImage = NULL;
				std::map<int, Emblem*>::const_iterator iter;
				for(int i = 0; i < BADGE_TRACKING; i++)
				{
					//Break out if we find no more ID's
					if(pEntity->m_badges[i] == 0)
						break;
					//Otherwise check to see if we found the right one
					else if(pEntity->m_badges[i] == glID)
					{
						//Remove the glass alpha so we can see the badge
						pImage->m_visible = true;

						// Store the badge GLID in the image's miscellaneous data member
						iter = EmblemMap.find(glID);
						if(iter != EmblemMap.end())
						{
							pEmblem = iter->second;
							pEmblemImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(pEmblem->VisualID, true));
							if (pEmblemImage)
								pEmblemImage->m_GLID = glID;
						}
						break;
					}
				}
				
				break;
			}
			case MENU_SCRIPTFUNC_SAVELASTDRAGITEM:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				if(globalStatus->m_draggingItem && !globalStatus->m_lastItemID)
				{
					globalStatus->m_lastItemID	 = globalStatus->m_draggingItem->m_miscInt1;
					globalStatus->m_lastItemAmount = globalStatus->m_draggingItem->m_miscInt2;
				}
				break;
			}
			case MENU_SCRIPTFUNC_UPDATE_RECENTJOURNALITEM:
			{
				if(!globalStatus->m_multiplayerObject->m_recentQuestPtr)
					break;

				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pListbox)
				{
					globalStatus->UI_Log("MENU_SCRIPTFUNC_UPDATE_RECENTJOURNALITEM cannot find list ID:", curLine->GetStrByParam(0));
					break;
				}

				float newAlpha = 1.0;
				if(pListbox->m_pFirstItem)
				if(GetTickCount()-pListbox->m_miscTimeStamp < 30000)
				{//if the quest journal was just changed it should be flashing
					float newAlpha = 1.0;
					
					if(GetTickCount()-pListbox->m_miscTimeStamp < 4000)
				    {//within the 4 seconds do the more extensive calc after that give time to lock, small optimization
						DWORD fadeDuration = 250;
						float alpha1 = 1.0f;
						float alpha2 = 0.1f;
				
						DWORD currentTime = GetTickCount();
						DWORD timesInto = (currentTime-pListbox->m_miscTimeStamp)/fadeDuration;
						DWORD progress = (currentTime-pListbox->m_miscTimeStamp)-(timesInto * fadeDuration);

						if (timesInto & 0x1)//odd
							newAlpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha1,(float)alpha2,(float)fadeDuration,(float)progress);
						else//even
							newAlpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha2,(float)alpha1,(float)fadeDuration,(float)progress);
					}
					
					pListbox->m_selectedAlpha	= newAlpha;
					pListbox->m_normalAlpha		= newAlpha;
					pListbox->m_specialAlpha	= newAlpha;
				}

				if (!globalStatus->m_multiplayerObject->m_bRecentJournalUpdate)
					break;
				else
					globalStatus->m_multiplayerObject->m_bRecentJournalUpdate = false;

				//journal fills these paramns on fail in a quest where its waiting on items, the key statement is extra info dialog in the quest tools in the speech dialog
				//this comes in a list form, only to be updated on change todo:check all other menu functions/updates for effeciency on updating only on change
				/*
	            if(globalStatus->m_jnlLastKeyStatement.CompareNoCase(globalStatus->m_multiplayerObject->m_recentQuestPtr->m_keyStatement)==0)
				    if(globalStatus->m_jnlLastQuestDescription.CompareNoCase(globalStatus->m_multiplayerObject->m_recentQuestPtr->m_questDescription)==0)
					    if(globalStatus->m_jnlLastNpcName.CompareNoCase(globalStatus->m_multiplayerObject->m_recentQuestPtr->m_npcName)==0)
							break;//did not change
							*/

				//update required

				pListbox->m_miscTimeStamp = GetTickCount();

				/*
				//pListbox->ClearList();//clear
				
                CMenuListItem	* journalItem		= new CMenuListItem();
				CMenuText		* npcText			= new CMenuText();
				//CMenuText		* keyText			= new CMenuText();
				//CMenuText		* descText			= new CMenuText();

				npcText->m_text = "Seer:" + globalStatus->m_multiplayerObject->m_recentQuestPtr->m_npcName;
				//keyText->m_text = globalStatus->m_multiplayerObject->m_recentQuestPtr->m_keyStatement;
				//descText->m_text = globalStatus->m_multiplayerObject->m_recentQuestPtr->m_questDescription;
			
				journalItem->m_textList.AddHead(npcText);
				//journalItem->m_textList.AddTail(descText);
				//journalItem->m_textList.AddTail(keyText);				
				pListbox->AddItem(journalItem);
				*/
				globalStatus->RefreshQuestJournal(ID_QUEST_JOURNAL_DESCRIPTION_LIST, false);
								
				//log this info as last set so we dont constanly burn cycles for no reason
				globalStatus->m_jnlLastNpcName = globalStatus->m_multiplayerObject->m_recentQuestPtr->m_npcName;
				globalStatus->m_jnlLastQuestDescription = globalStatus->m_multiplayerObject->m_recentQuestPtr->m_questDescription;
				globalStatus->m_jnlLastKeyStatement = globalStatus->m_multiplayerObject->m_recentQuestPtr->m_keyStatement;

				break;
			}
			case MENU_SCRIPTFUNC_REQUESTFULLJOURNAL:
			{
				if (!globalStatus->m_multiplayerObject->GetEnabledClient_safe()) break;

				CMovementObj *pEntity = globalStatus->m_entObjRuntime->GetObjByIndex(0);
				if(!pEntity)
					break;

				globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,
																	45,
																	curLine->GetIntByParam(0),
																	pEntity->List_networkDefinedID,
																	0,
																	0,
																	NULL);//request full journal and return list ID
				break;
			}
			case MENU_SCRIPTFUNC_TEXTFLASH:
			{

				CMenuText *pMenuText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pMenuText)
					break;

				if(!pMenuText->m_textMesh)
					break;

				if(!pMenuText->m_textMesh->List_materialObject)
					break;

				if(pMenuText->m_text.GetLength() < 1)
				   break;//optimize non visible

				DWORD fadeDuration = (DWORD)curLine->GetIntByParam(3);
				float alpha1 = curLine->GetFloatByParam(1);
				float alpha2 = curLine->GetFloatByParam(2);
				if(fadeDuration < 1)
				   break;//must be greater than zero or this will cause divide by zero

				DWORD currentTime = GetTickCount();
				DWORD timesInto = (currentTime-pMenuText->m_miscTimeStamp)/fadeDuration;
				DWORD progress = (currentTime-pMenuText->m_miscTimeStamp)-(timesInto * fadeDuration);


				if (timesInto & 0x1)//odd
					pMenuText->m_textMesh->List_materialObject->m_useMaterial.Diffuse.a = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha1,(float)alpha2,(float)fadeDuration,(float)progress);
				else//even
					pMenuText->m_textMesh->List_materialObject->m_useMaterial.Diffuse.a = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha2,(float)alpha1,(float)fadeDuration,(float)progress);
					
				break;
			}
			case MENU_SCRIPTFUNC_FLASHBUTTON:
			{
				CMenuButton *pMenuButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pMenuButton)
					break;

				if(!pMenuButton->m_stateImages)
					break;

				if(!pMenuButton->m_visible)
					break;

				DWORD fadeDuration = (DWORD)curLine->GetIntByParam(3);
				float alpha1 = curLine->GetFloatByParam(1);
				float alpha2 = curLine->GetFloatByParam(2);
				if(fadeDuration < 1)
				   break;//must be greater than zero or this will cause divide by zero

				DWORD currentTime = GetTickCount();
				DWORD timesInto = (currentTime-pMenuButton->m_miscTimeStamp)/fadeDuration;
				DWORD progress = (currentTime-pMenuButton->m_miscTimeStamp)-(timesInto * fadeDuration);

				if(timesInto & 0x1)//odd
				{
					for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
					{
						for(int j = 0; j < MENU_BUTTONIMAGE_MAX; j++)
						{
							if(pMenuButton->m_stateImages[i][j])
							{
								pMenuButton->m_stateImages[i][j]->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha1, (float)alpha2, (float)fadeDuration, (float)progress);
								pMenuButton->m_stateImages[i][j]->UpdateTexture(pMenuButton->m_stateImages[i][j]->m_textureSelect);
								pMenuButton->m_stateImages[i][j]->UpdateTextureCoords();
							}
						}
					}
				}
				else//even
				{
					for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
					{
						for(int j = 0; j < MENU_BUTTONIMAGE_MAX; j++)
						{
							if(pMenuButton->m_stateImages[i][j])
							{
								pMenuButton->m_stateImages[i][j]->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha2, (float)alpha1, (float)fadeDuration, (float)progress);
								pMenuButton->m_stateImages[i][j]->UpdateTexture(pMenuButton->m_stateImages[i][j]->m_textureSelect);
								pMenuButton->m_stateImages[i][j]->UpdateTextureCoords();
							}
						}
					}
				}
					
				break;
			}
			case MENU_SCRIPTFUNC_FLASHPROGRESS:
			{
				CMenuProgressBar *pMenuVisual = dynamic_cast<CMenuProgressBar*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pMenuVisual)
					break;

				DWORD fadeDuration = (DWORD)curLine->GetIntByParam(3);
				float alpha1 = curLine->GetFloatByParam(1);
				float alpha2 = curLine->GetFloatByParam(2);
				if(fadeDuration < 1)
				   break;//must be greater than zero or this will cause divide by zero

				DWORD currentTime = GetTickCount();
				DWORD timesInto = (currentTime-pMenuVisual->m_miscTimeStamp)/fadeDuration;
				DWORD progress = (currentTime-pMenuVisual->m_miscTimeStamp)-(timesInto * fadeDuration);

				if(timesInto & 0x1)//odd
				{
					if(pMenuVisual->m_backgroundLeft)
					{
						pMenuVisual->m_backgroundLeft->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha1, (float)alpha2, (float)fadeDuration, (float)progress);
						pMenuVisual->m_backgroundLeft->UpdateTexture(pMenuVisual->m_backgroundLeft->m_textureSelect);
						pMenuVisual->m_backgroundLeft->UpdateTextureCoords();
					}
					if(pMenuVisual->m_backgroundMiddle)
					{
						pMenuVisual->m_backgroundMiddle->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha1, (float)alpha2, (float)fadeDuration, (float)progress);
						pMenuVisual->m_backgroundMiddle->UpdateTexture(pMenuVisual->m_backgroundMiddle->m_textureSelect);
						pMenuVisual->m_backgroundMiddle->UpdateTextureCoords();
					}
					if(pMenuVisual->m_backgroundRight)
					{
						pMenuVisual->m_backgroundRight->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha1, (float)alpha2, (float)fadeDuration, (float)progress);
						pMenuVisual->m_backgroundRight->UpdateTexture(pMenuVisual->m_backgroundRight->m_textureSelect);
						pMenuVisual->m_backgroundRight->UpdateTextureCoords();
					}
					if(pMenuVisual->m_foregroundLeft)		
					{
						pMenuVisual->m_foregroundLeft->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha1, (float)alpha2, (float)fadeDuration, (float)progress);
						pMenuVisual->m_foregroundLeft->UpdateTexture(pMenuVisual->m_foregroundLeft->m_textureSelect);
						pMenuVisual->m_foregroundLeft->UpdateTextureCoords();
					}
					if(pMenuVisual->m_foregroundMiddle)
					{
						pMenuVisual->m_foregroundMiddle->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha1, (float)alpha2, (float)fadeDuration, (float)progress);
						pMenuVisual->m_foregroundMiddle->UpdateTexture(pMenuVisual->m_foregroundMiddle->m_textureSelect);
						pMenuVisual->m_foregroundMiddle->UpdateTextureCoords();
					}
					if(pMenuVisual->m_foregroundRight)
					{
						pMenuVisual->m_foregroundRight->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha1, (float)alpha2, (float)fadeDuration, (float)progress);
						pMenuVisual->m_foregroundRight->UpdateTexture(pMenuVisual->m_foregroundRight->m_textureSelect);
						pMenuVisual->m_foregroundRight->UpdateTextureCoords();
					}
				}
				else//even
				{
					if(pMenuVisual->m_backgroundLeft)
					{
						pMenuVisual->m_backgroundLeft->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha2, (float)alpha1, (float)fadeDuration, (float)progress);
						pMenuVisual->m_backgroundLeft->UpdateTexture(pMenuVisual->m_backgroundLeft->m_textureSelect);
						pMenuVisual->m_backgroundLeft->UpdateTextureCoords();
					}
					if(pMenuVisual->m_backgroundMiddle)
					{
						pMenuVisual->m_backgroundMiddle->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha2, (float)alpha1, (float)fadeDuration, (float)progress);
						pMenuVisual->m_backgroundMiddle->UpdateTexture(pMenuVisual->m_backgroundMiddle->m_textureSelect);
						pMenuVisual->m_backgroundMiddle->UpdateTextureCoords();
					}
					if(pMenuVisual->m_backgroundRight)
					{
						pMenuVisual->m_backgroundRight->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha2, (float)alpha1, (float)fadeDuration, (float)progress);
						pMenuVisual->m_backgroundRight->UpdateTexture(pMenuVisual->m_backgroundRight->m_textureSelect);
						pMenuVisual->m_backgroundRight->UpdateTextureCoords();
					}
					if(pMenuVisual->m_foregroundLeft)		
					{
						pMenuVisual->m_foregroundLeft->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha2, (float)alpha1, (float)fadeDuration, (float)progress);
						pMenuVisual->m_foregroundLeft->UpdateTexture(pMenuVisual->m_foregroundLeft->m_textureSelect);
						pMenuVisual->m_foregroundLeft->UpdateTextureCoords();
					}
					if(pMenuVisual->m_foregroundMiddle)
					{
						pMenuVisual->m_foregroundMiddle->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha2, (float)alpha1, (float)fadeDuration, (float)progress);
						pMenuVisual->m_foregroundMiddle->UpdateTexture(pMenuVisual->m_foregroundMiddle->m_textureSelect);
						pMenuVisual->m_foregroundMiddle->UpdateTextureCoords();
					}
					if(pMenuVisual->m_foregroundRight)
					{
						pMenuVisual->m_foregroundRight->m_alpha = globalStatus->m_matLib.InterpolateFloatsHiRes((float)alpha2, (float)alpha1, (float)fadeDuration, (float)progress);
						pMenuVisual->m_foregroundRight->UpdateTexture(pMenuVisual->m_foregroundRight->m_textureSelect);
						pMenuVisual->m_foregroundRight->UpdateTextureCoords();
					}
				}
				
				break;
			}
			case MENU_SCRIPTFUNC_REMOVEITEMFORTRADE:
			{
				// PARSE out script params
				MENU_IDS eTradeMenuID = (MENU_IDS)curLine->GetIntByParam(0);
				MENU_IDS eInventoryMenuID = (MENU_IDS)curLine->GetIntByParam(1);

				// GET references to menu visuals
				CMenuTextBox *pTradeAmountText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_REMOVE_QUANTITY_TEXT, true));
				CMenuInventory *pPlayerInventory = globalStatus->GetActiveInventory(eInventoryMenuID);
				CMenuInventory *pTradeInventory = globalStatus->GetActiveInventory(eTradeMenuID);
				CMenuButton *pOfferButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_OFFER_BUTTON, true));
				CMenuButton *pConfirmButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CONFIRM_BUTTON, true));
				CMenuButton *pCancelButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CANCEL_BUTTON, true));
				if (!pPlayerInventory
				|| !pTradeInventory
				|| !pTradeInventory->SelectedItem()
				|| !pOfferButton
				|| !pConfirmButton
				|| !pCancelButton)
					break;

				// CHECK if we need to DISABLE buttons
				if (pTradeInventory->Count() <= 0)
				{
					pOfferButton->SetEnabled(false);
					pConfirmButton->SetEnabled(false);
					pCancelButton->SetEnabled(false);
				}
				else
				{
					pOfferButton->SetEnabled();
					pCancelButton->SetEnabled();
					pConfirmButton->SetEnabled(false);
				}

				// CHECK if we need to display the quantity popup menu
				int iAmount = 1;
				if (pTradeInventory->SelectedItem()->m_data.m_quantity > 1)
				{
					if (!pTradeAmountText
					|| !pTradeAmountText->m_linesOfText)
					{
						globalStatus->OpenMenu(ID_TRADE_REMOVE_QUANTITY_MENU, false);
						break;
					}
					else
					{
						// GET the amount specified for purchase
						iAmount = atoi(pTradeAmountText->m_linesOfText[0].m_text);
						if(iAmount < 1)
						{
							globalStatus->ShowMessageBox("Trade", "Please specify an amount greater than 0 to purchase!");
							break;
						}
						else if(iAmount > 30000)
						{
							globalStatus->ShowMessageBox("Trade", "Please specify an amount less than 30,000 to purchase!");
							break;
						}
					}
				}

				// ERROR checking: Check amount specified against actual trade inventory stock.
				//	IF we remove all copies of the item, REMOVE the selection indicator.
				if (iAmount > pTradeInventory->SelectedItem()->m_data.m_quantity)
					iAmount = pTradeInventory->SelectedItem()->m_data.m_quantity;

				// ADD the specified amount of the selected item for trade to the player's inventory
				ITEMDATA_STRUCT_EX item = pTradeInventory->SelectedItem()->m_data;
				item.m_quantity = iAmount;
				pPlayerInventory->AddItem(item);

				// REMOVE the specified amount of the selected item for trade from the trade inventory
				pTradeInventory->RemoveItem(item.m_GLID, item.m_quantity);
				break;

				// UPDATE offer to reset
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
				{
					CMovementObj *pMovementObject = (CMovementObj*)globalStatus->m_entObjRuntime->GetObjByIndex(0);
					if (pMovementObject
					&& (pMovementObject->List_controlType == 0))
						globalStatus->SendChatString("/TRADERESET");
				}
				break;
				/*//Check for enabled Client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetObjByIndex(0);
                if(!ptrLoc)
					break;

				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pListbox)
				{
					globalStatus->UI_Log("MENU_SCRIPTFUNC_REMOVEITEMFORTRADE cannot find list ID:", curLine->GetStrByParam(0));
					break;
				}
				if(!pListbox->m_currentSelection)
				{
					globalStatus->UI_Log("No Item Selected from listbox to remove: ", curLine->GetStrByParam(0));
					break;
				}

				//Remove the selected item from the list
				pListbox->RemoveItem(pListbox->m_currentSelection->m_index);
				//Reset the current selection variable to NULL
				pListbox->m_currentSelection = NULL;

				//Update offer to reset
				if(ptrLoc->List_controlType == 0)
					globalStatus->SendChatString(":TRADERESET");
				
				break;*/
			}
			case MENU_SCRIPTFUNC_ADDITEMFORTRADE:
			{
				// PARSE out script params
				MENU_IDS eTradeMenuID = (MENU_IDS)curLine->GetIntByParam(0);
				MENU_IDS eInventoryMenuID = (MENU_IDS)curLine->GetIntByParam(1);

				// GET references to menu visuals
				CMenuTextBox *pTradeAmountText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_ADD_QUANTITY_TEXT, true));
				CMenuInventory *pPlayerInventory = globalStatus->GetActiveInventory(eInventoryMenuID);
				CMenuInventory *pTradeInventory = globalStatus->GetActiveInventory(eTradeMenuID);
				CMenuButton *pOfferButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_OFFER_BUTTON, true));
				CMenuButton *pConfirmButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CONFIRM_BUTTON, true));
				CMenuButton *pCancelButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_CANCEL_BUTTON, true));
				if (!pPlayerInventory
				|| !pPlayerInventory->SelectedItem()
				|| !pTradeInventory
				|| !pOfferButton
				|| !pConfirmButton
				|| !pCancelButton)
					break;

				// ENABLE/DISABLE buttons
				pOfferButton->SetEnabled();
				pCancelButton->SetEnabled();
				pConfirmButton->SetEnabled(false);

				// CHECK if we need to display the quantity popup menu
				int iAmount = 1;
				if (pPlayerInventory->SelectedItem()->m_data.m_quantity > 1)
				{
					if (!pTradeAmountText
					|| !pTradeAmountText->m_linesOfText)
					{
						globalStatus->OpenMenu(ID_TRADE_ADD_QUANTITY_MENU, false);
						break;
					}
					else
					{
						// GET the amount specified for purchase
						iAmount = atoi(pTradeAmountText->m_linesOfText[0].m_text);
						if(iAmount < 1)
						{
							globalStatus->ShowMessageBox("Trade", "Please specify an amount greater than 0 to purchase!");
							break;
						}
						else if(iAmount > 30000)
						{
							globalStatus->ShowMessageBox("Trade", "Please specify an amount less than 30,000 to purchase!");
							break;
						}
					}
				}

				// ERROR checking: Check amount specified against actual inventory stock.
				//	IF we remove all copies of the item, REMOVE the selection indicator.
				if (iAmount > pPlayerInventory->SelectedItem()->m_data.m_quantity)
					iAmount = pPlayerInventory->SelectedItem()->m_data.m_quantity;

				// ADD the specified amount of the selected item for trade to the trade inventory
				ITEMDATA_STRUCT_EX item = pPlayerInventory->SelectedItem()->m_data;
				item.m_quantity = iAmount;
				pTradeInventory->AddItem(item);

				// REMOVE the specified amount of the selected item for trade from the player's inventory
				pPlayerInventory->RemoveItem(item.m_GLID, item.m_quantity);
				break;
				/*
				POSITION entPosition = globalStatus->m_entObjRuntime->FindIndex(0);
				if(entPosition)
				{//valid		
					int quantity = 1;
					CMenuTextBox *editVisual = (CMenuTextBox *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
					if(editVisual)
						if(editVisual->m_linesOfText)
						{//valid
							int quanityCnv = atoi(editVisual->m_linesOfText[0].m_text);
							if(quanityCnv > 0 && quanityCnv < 30000)
								quantity = quanityCnv;
							else
								globalStatus->UI_Log("Invalid quanity-defaulting to 1 ");
						}//end valid

					CMovementObj *ptrLoc = (CMovementObj *) globalStatus->m_entObjRuntime->GetAt(entPosition);
					CMenuListBox *inventoryList = (CMenuListBox *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);
					CMenuListBox *tradeList = (CMenuListBox *)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(2), true);

					if(!inventoryList || !tradeList)
						break;
					if(!inventoryList->m_currentSelection)
						break;

					int glID = inventoryList->m_currentSelection->m_miscInt1;
					
					//Error check
					if(quantity > inventoryList->m_currentSelection->m_miscInt2 || 
					   quantity < 1)
						quantity = 1;

					//Get the item we are trying to Deposit
					CItemObj * invObj = globalStatus->m_itemManager->GetItemByID(glID);
					if(invObj)
					{//valid
						if(!invObj->m_stackable)
						{
							for(int count = 0; count < quantity; count++)
								globalStatus->AddStringToMenuList( tradeList->m_visualID,
																invObj->m_itemName,
																invObj->m_visualRef,
																glID,
																1,
																invObj->m_itemName,
																1,
																"");

						}
						else
						{
							if(invObj->m_stackable)
							{								
								CMenuListItem *item = NULL;
								for(item = tradeList->m_pFirstItem; item != NULL;)
								{
									if(item->m_miscInt1 == glID)
									{
										item->m_miscInt2 += quantity;
										int possibleDif = 0;
										if(item->m_miscInt2 > 30000)
											possibleDif = item->m_miscInt2 - 30000;
										//Update the quantity that we show in the list
										CMenuText *text = (CMenuText *)item->m_textList.GetTail();
										if(text)
										{
											text->m_text = " Quantity: ";
											char buffer[20];
											text->m_text += itoa(item->m_miscInt2, buffer, 10);
											
											text->m_textMesh->SafeDelete();
											delete text->m_textMesh;
											text->m_textMesh = NULL;
										}
										if(possibleDif)
										{
											CString quantityText = globalStatus->m_cabLib.ConvToString(quantity);
											CString outPut = operator+(invObj->m_itemName,operator+(" X", quantityText));
											globalStatus->AddStringToMenuList( tradeList->m_visualID,
																			outPut,
																			invObj->m_visualRef,
																			glID,
																			quantity,
																			invObj->m_itemName,
																			1,
																			"");
										}
										break;
									}
									item = item->m_pNext;
								}
								if(!item)
								{
									CString quantityText = globalStatus->m_cabLib.ConvToString(quantity);
									CString outPut = operator+(invObj->m_itemName,operator+(" X", quantityText));
									globalStatus->AddStringToMenuList( tradeList->m_visualID,
																	outPut,
																	invObj->m_visualRef,
																	glID,
																	quantity,
																	invObj->m_itemName,
																	1,
																	"");
								}
							}
							else
							{
								for(int count = 0; count < quantity; count++)
									globalStatus->AddStringToMenuList(	tradeList->m_visualID,
																	invObj->m_itemName,
																	invObj->m_visualRef,
																	glID,
																	1,
																	invObj->m_itemName,
																	1,
																	"");
							}
						}
					}//End Valid item object
					inventoryList->m_currentSelection = NULL;
				}//End Valid Entity Position
				break;*/
			}
			case MENU_SCRIPTFUNC_RESETLIST:
			{
				//Check for enabled Client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetObjByIndex(0);
                if(!ptrLoc)
					break;

				CMenuListBox *visual = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				if(visual)
					visual->ClearList();

				break;
			}
			case MENU_SCRIPTFUNC_REFRESHQUESTJOURNAL:
			{
				// REFRESH the quest journal list
				globalStatus->RefreshQuestJournal(curLine->GetIntByParam(0), true);
				break;
			}
			case MENU_SCRIPTFUNC_UPDATELOADINGITEMS:
			{
				// PARSE script params
				int iInventoryID = curLine->GetIntByParam(0);
				DWORD iUpdatePeriod = (DWORD)curLine->GetIntByParam(1);

				// CHECK if the update period has elapsed
				if (iUpdatePeriod < 1) 
					iUpdatePeriod = 2000;
				if ((GetTickCount() - this->m_timeStamp) < iUpdatePeriod)
					break;

				// UPDATE the timestamp
				this->m_timeStamp = GetTickCount();

				// GET references to menu visuals
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iInventoryID, true));
				if (!pInventory) break;

				// ITERATE through the inventory items and
				//	CHECK for any incompletely received items that now have data available
				ITEMLIST::iterator iter;
				for (iter = pInventory->m_items.begin(); iter != pInventory->m_items.end(); iter++)
				{
					ITEMDATA_STRUCT_EX item = (*iter)->m_data;
					if (!item.m_received)
					{
						CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(item.m_GLID);
						if (!pItem) continue;
						pInventory->RefreshItem(item);
					}
				}
				break;
				/*
				DWORD cycleDuration = (DWORD)curLine->GetIntByParam(1);
				if(cycleDuration < 1)
				   cycleDuration = 2000;
				//cycle time
				if(GetTickCount()-m_timeStamp < cycleDuration)
					break;

				m_timeStamp = GetTickCount();

				CMenuListBox *visual = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				if(!visual)
					break;

				//check for items in the list that have streamed it, they will be called loading but the glid is correct

				CMenuListItem * curItem = visual->m_pFirstItem;
				while(curItem)
				{
					CMenuText *cText = dynamic_cast<CMenuText*>(curItem->m_textList.GetHead());
					if(!cText)
						continue;
					
					if(cText->m_text.CompareNoCase("Loading")==0)
					{//see if this item is waiting on data
						CItemObj * invObj = globalStatus->m_itemManager->GetItemByID(curItem->m_miscInt1);
						if(invObj)
						{//aquired it
							cText->m_text = invObj->m_itemName;
							cText->m_textMesh->SafeDelete();
							delete cText->m_textMesh;//delete so hardware refreshes
							cText->m_textMesh = 0;
						}
					}//end see if this item is waiting on data
					curItem = curItem->m_pNext;
				}
				*/
			}
			case MENU_SCRIPTFUNC_DISPLAYQUESTJOURNALDETAILS:
			{
				// CHECK if we're in the game client
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// GET a reference to the necessary menu visuals
				CMenuListBox *pQuestListbox = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true);
				CMenuListBox *pDescriptionListbox = (CMenuListBox*)globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true);
				if(!pQuestListbox
				|| !pQuestListbox->m_currentSelection
				|| !pDescriptionListbox
				|| !globalStatus->m_multiplayerObject->m_journalEntries)
					break;

				// CLEAR the quest description listbox
				pDescriptionListbox->ClearList();

				// GET the selected quest NPC name
				CMenuListItem *pSelectedQuest = pQuestListbox->m_currentSelection;
				CMenuText *pSelectedQuestText = dynamic_cast<CMenuText*>(pSelectedQuest->m_textList.GetHead());

				if (!pSelectedQuestText
				|| (pSelectedQuestText->m_text == ""))
					break;

				CString selectedNpcName = pSelectedQuest->m_miscString;
				if (selectedNpcName.IsEmpty())
				{
					selectedNpcName = pSelectedQuestText->m_text;
					if (selectedNpcName.Left(7).CompareNoCase("[Done] ") == 0)
						selectedNpcName = selectedNpcName.Mid(7);
				}

				CMenuInventory *pPlayerInventory = dynamic_cast<CMenuInventory*>(
					globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));

				POSITION pos = NULL;
				for (pos = globalStatus->m_multiplayerObject->m_journalEntries->GetHeadPosition(); pos != NULL;)
				{
					CMenuText *pLineBreak = NULL;

					CQuestJournalObj *pJournalEntry = dynamic_cast<CQuestJournalObj*>(globalStatus->m_multiplayerObject->m_journalEntries->GetNext(pos));
					if (!pJournalEntry
					|| (pJournalEntry->m_npcName.CompareNoCase(selectedNpcName) != 0))
						continue;

					// 1) Quest Giver & Status line
					{
						CMenuListItem *pItemStatus = new CMenuListItem();
						pItemStatus->m_special = (pJournalEntry->m_status != 0);
						CMenuText *pStatusText = new CMenuText();
						pStatusText->m_multilined = true;
						pStatusText->m_text = "Quest Giver: ";
						pStatusText->m_text.Append(pJournalEntry->m_npcName);
						pStatusText->m_text.Append((pJournalEntry->m_status != 0) ? "  [Completed]" : "  [Active]");
						pItemStatus->AddText(pStatusText, pDescriptionListbox->m_width);
						pDescriptionListbox->AddItem(pItemStatus);
					}

					// 2) Full Quest Description / Story (previously omitted!)
					if (!pJournalEntry->m_questDescription.IsEmpty())
					{
						CMenuListItem *pItemDesc = new CMenuListItem();
						CMenuText *pDescText = new CMenuText();
						pDescText->m_multilined = true;
						pDescText->m_text = "Objective: ";
						pDescText->m_text.Append(pJournalEntry->m_questDescription);
						pItemDesc->AddText(pDescText, pDescriptionListbox->m_width);
						pDescriptionListbox->AddItem(pItemDesc);
					}

					// 3) Quest Hint / Key Statement
					if (!pJournalEntry->m_keyStatement.IsEmpty())
					{
						CMenuListItem *pItemKeyStatement = new CMenuListItem();
						CMenuText *pKeyStatement = new CMenuText();
						pKeyStatement->m_multilined = true;
						pKeyStatement->m_text.Append("Hint: ");
						pKeyStatement->m_text.Append(pJournalEntry->m_keyStatement);
						pItemKeyStatement->AddText(pKeyStatement, pDescriptionListbox->m_width);
						pDescriptionListbox->AddItem(pItemKeyStatement);
					}

					// ADD a line break
					CMenuListItem *pItemLineBreak = new CMenuListItem();
					pLineBreak = new CMenuText();
					pLineBreak->m_text.Append("");
					pItemLineBreak->AddText(pLineBreak, pDescriptionListbox->m_width);
					pDescriptionListbox->AddItem(pItemLineBreak);

					// 4) Required Items with live inventory counts (have / needed)
					CMenuListItem *pItemRequiredItems = new CMenuListItem();
					CMenuText *pRequiredItems = new CMenuText();
					pRequiredItems->m_multilined = true;
					pRequiredItems->m_text = "Required items:";
					pItemRequiredItems->AddText(pRequiredItems, pDescriptionListbox->m_width);
					pDescriptionListbox->AddItem(pItemRequiredItems);

					if (pJournalEntry->m_requiredItems && pJournalEntry->m_requiredItems->GetCount() > 0)
					{
						for (POSITION reqPos = pJournalEntry->m_requiredItems->GetHeadPosition(); reqPos != NULL;)
						{
							CQJRequiredItem *pItem = dynamic_cast<CQJRequiredItem*>(pJournalEntry->m_requiredItems->GetNext(reqPos));
							if (!pItem)
								continue;

							int neededQty = (pItem->m_quanity > 0) ? pItem->m_quanity : 1;
							int haveQty = 0;
							if (pPlayerInventory && globalStatus->m_itemManager)
							{
								for (size_t invIdx = 0; invIdx < pPlayerInventory->m_items.size(); ++invIdx)
								{
									CMenuInventoryItem *invSlot = pPlayerInventory->m_items[invIdx];
									if (!invSlot || invSlot->m_data.m_GLID <= 0)
										continue;
									CItemObj *itemDef = globalStatus->m_itemManager->GetItemByID(invSlot->m_data.m_GLID);
									if (itemDef && itemDef->m_itemName.CompareNoCase(pItem->m_itemName) == 0)
									{
										int slotQty = (invSlot->m_data.m_quantity > 0) ? invSlot->m_data.m_quantity : 1;
										haveQty += slotQty;
									}
								}
							}

							CMenuListItem *pItemRequiredItem = new CMenuListItem();
							pItemRequiredItem->m_special = (haveQty >= neededQty || pJournalEntry->m_status != 0);
							CMenuText *pRequiredItem = new CMenuText();
							pRequiredItem->m_multilined = true;
							pRequiredItem->m_text.Format("  - %s (%d/%d)%s",
								(const char*)pItem->m_itemName,
								haveQty,
								neededQty,
								(haveQty >= neededQty) ? " [Ready]" : "");
							pItemRequiredItem->AddText(pRequiredItem, pDescriptionListbox->m_width);
							pDescriptionListbox->AddItem(pItemRequiredItem);
						}
					}
					else
					{
						CMenuListItem *pNoItems = new CMenuListItem();
						CMenuText *pNoItemsText = new CMenuText();
						pNoItemsText->m_multilined = true;
						pNoItemsText->m_text = "  - None";
						pNoItems->AddText(pNoItemsText, pDescriptionListbox->m_width);
						pDescriptionListbox->AddItem(pNoItems);
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_ACTIVATEITEM:
			{
				// GET references to the menu visuals
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if (!pInventory || !pInventory->SelectedItem())
					break;

				// IF we are in the game client, ARM the selected item
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					globalStatus->ActivateItem(pInventory->SelectedItem());
				break;
			}
			case MENU_SCRIPTFUNC_DISARM:
			{
				// GET a reference to the menu visual
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				CMenuButton *pEquipButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_EQUIP_BUTTON, true));
				CMenuButton *pUnequipButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_UNEQUIP_BUTTON, true));
				if (!pInventory || !pInventory->SelectedItem())
					break;

				// IF this is an enabled client, DISARM the selected item
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					globalStatus->DeactivateItem(pInventory->SelectedItem());
				break;
			}
			case MENU_SCRIPTFUNC_DELETEITEM:
			{
				// GET a reference to the client side player and menu visuals
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				CMovementObj *ptrLoc = (CMovementObj*)globalStatus->m_entObjRuntime->GetAt(pos);
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				
				// ERROR checking
				if(!pInventory || !pInventory->SelectedItem() || !ptrLoc)
					break;

				// SEND the network message only if we are in the game client
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
				{
					// CREATE packet to remove this item on the server-side
					LPATTRIB_STRUCT lpChatMessage = NULL;
					DWORD messageSize;
					messageSize = sizeof(ATTRIB_STRUCT);
					lpChatMessage = (LPATTRIB_STRUCT) new BYTE[messageSize];

					// BUILD packet information
					lpChatMessage->m_catagorie = 6;
					lpChatMessage->attribute   = 31;
					lpChatMessage->uniInt      = pInventory->SelectedItem()->m_data.m_GLID;
					lpChatMessage->idRefrence  = (short)ptrLoc->List_networkDefinedID;
					lpChatMessage->uniInt2     = 0;
					lpChatMessage->uniShort3   = 0;
					lpChatMessage->CRCcheck    = globalStatus->m_multiplayerObject->GetAttribCRC(lpChatMessage);
				    
					// SEND packet
					globalStatus->m_multiplayerObject->m_clientCfgRequestOutbox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,0);

					// RELEASE resources
					SafeDeleteArray(lpChatMessage);
				}
				
				// REMOVE the item from the inventory
				pInventory->RemoveItem(pInventory->SelectedItem()->m_data.m_GLID, pInventory->SelectedItem()->m_data.m_quantity);

				// INVALIDATE current selections
				pInventory->ClearSelectedItem();
				break;
			}
			case MENU_SCRIPTFUNC_INITMENU:
			{
				int iVisualID = curLine->GetIntByParam(0);
				CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if (!pMenu) break;

				switch (iVisualID)
				{
					case ID_EXAMINE_MENU:
					{
						// GET references to menu visuals
						CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_EXAMINE_INVENTORY_SELECTION_IMAGE, true));
						CMenuButton *pLootButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_EXAMINE_LOOT_BUTTON, true));
						CMenuButton *pLootAllButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_EXAMINE_LOOT_ALL_BUTTON, true));
						
						CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_LOOT_INVENTORY, true));
						if(!pInventory) break;

						pInventory->ClearItems();

						// HIDE the selection indicator
						if (pSelectionIndicator)
							pSelectionIndicator->m_visible = false;

						// DISABLE the loot buttons
						if (pLootButton) pLootButton->SetEnabled(false);
						if (pLootAllButton) pLootAllButton->SetEnabled(false);
						break;
					}
					case ID_SQUAD_STATUS_MENU:
					{
						// CENTER justify the title text
						if(pMenu->m_titleText)
							pMenu->m_titleText->m_alignment = MENU_TEXTALIGN_CENTER;
						break;	
					}
					case ID_ARENA_REGISTRATION_01_MENU:
					{
						CMenuToggle *pToggle = NULL;
						CMenuToggle *pSoloToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_SOLO_TOGGLE, true));
						CMenuToggle *pTeamToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_TEAM_TOGGLE, true));
						CMenuToggle *pTypeToggle = dynamic_cast<CMenuToggle*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_TOGGLE, true));
						CMenuImage *pEliminationImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_ELIMINATION_SPLASH_IMAGE, true));
						CMenuImage *pDeathmatchImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_DEATHMATCH_SPLASH_IMAGE, true));
						CMenuButton *pNextButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REGISTRATION_NEXT_BUTTON, true));

						// DISABLE the navigation buttons
						if (pNextButton) pNextButton->SetEnabled(false);

						// HIDE the template arena TYPE toggle control
						if (pTypeToggle) pTypeToggle->m_visible = false;

						// HIDE the arena type splash images
						if (pEliminationImage) pEliminationImage->m_visible = false;
						if (pDeathmatchImage) pDeathmatchImage->m_visible = false;
						
						// LOOP through all the child controls of the open window
						POSITION posCurrent = NULL, posPrevious = NULL;
						for (posCurrent = pMenu->m_childList->GetHeadPosition(); posCurrent != NULL;)
						{
							// DELETE all toggle controls whose ID is > the template toggle control's ID
							posPrevious = posCurrent;
							pToggle = dynamic_cast<CMenuToggle*>(pMenu->m_childList->GetNext(posCurrent));

							if (pToggle && (pToggle->m_visualID > pTypeToggle->m_visualID))
							{
								pMenu->m_childList->RemoveAt(posPrevious);
								SafeDelete(pToggle);
							}
						}

						// INIT selected arena MODE
						//	and execute the toggle click event
						if (pSoloToggle) pSoloToggle->SetEnabled();
						if (pTeamToggle) pTeamToggle->SetEnabled();
						if (pSoloToggle->m_pData && pSoloToggle->m_clickScript) 
							pSoloToggle->m_clickScript->ExecuteScript();
						else if (pTeamToggle->m_pData && pTeamToggle->m_clickScript) 
							pTeamToggle->m_clickScript->ExecuteScript();
						else if (pSoloToggle->m_clickScript)
							pSoloToggle->m_clickScript->ExecuteScript();
						break;
					}
					case ID_ARENA_REGISTRATION_02_MENU:
					{
						// GET the client's entity
						POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
						if (!pos) break;
						CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);

						CMenuToggle *pToggle = NULL;
						CMenuButton *pPreviousButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REGISTRATION_PREVIOUS_BUTTON, true));
						CMenuButton *pJoinButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_JOIN_BUTTON, true));
						CMenuButton *pRefreshButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REFRESH_BUTTON, true));
						CMenuButton *pLeaveButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_LEAVE_BUTTON, true));
						
						// Disable the JOIN and LEAVE navigation button
						if (pJoinButton) pJoinButton->SetEnabled(true);
						if (pLeaveButton 
						/*&& pEntity->m_hasJoinedArena */
						&& (pEntity->m_arenaBotID > 0))
						{
							pLeaveButton->SetEnabled(true);
							if (pJoinButton) pJoinButton->SetEnabled(false);
						}
						else 
							pLeaveButton->SetEnabled(false);

						// Enable the PREVIOUS, REFRESH and CLOSE buttons
						if (pRefreshButton) pRefreshButton->SetEnabled();
						if (pPreviousButton) pPreviousButton->SetEnabled();

						// Loop through all the child controls of the open window
						POSITION posCurrent = NULL, posPrevious = NULL;
						for (posCurrent = pMenu->m_childList->GetHeadPosition(); posCurrent != NULL;)
						{
							// Disable all toggle controls
							pToggle = dynamic_cast<CMenuToggle*>(pMenu->m_childList->GetNext(posCurrent));
							if (pToggle)
								pToggle->SetEnabled(false);
						}
						break;
					}
					case ID_SYSTEM_CHAT_MENU:
					{
						// GET references to menu visuals
						CMenuButton *pHideButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SYSTEM_CHAT_HIDE_BUTTON, true));
						CMenuButton *pShowButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SYSTEM_CHAT_SHOW_BUTTON, true));
						if (!pHideButton || !pShowButton) break;
						
						// SET button enabled status
						pHideButton->SetEnabled();
						pShowButton->SetEnabled(false);
						break;
					}
					case ID_PLAYER_CHAT_MENU:
					{
						// GET references to menu visuals
						CMenuButton *pHideButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_CHAT_HIDE_BUTTON, true));
						CMenuButton *pShowButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_CHAT_SHOW_BUTTON, true));
						if (!pHideButton || !pShowButton) break;

						// SET button enabled status
						pHideButton->SetEnabled();
						pShowButton->SetEnabled(false);
						break;
					}
					case ID_TRADE_MENU:
					{
						// GET references to menu visuals
						CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_INVENTORY_SELECTION_IMAGE, true));
						CMenuInventory *pYourInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_YOUR_INVENTORY, true));
						CMenuInventory *pTheirInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_THEIR_INVENTORY, true));
						if (!pYourInventory || !pTheirInventory) break;

						pYourInventory->ClearItems();
						pTheirInventory->ClearItems();

						// SET default active states for the trade inventories
						pYourInventory->m_bActive = true;
						pTheirInventory->m_bActive = false;

						// HIDE the inventory selection indicator
						if (pSelectionIndicator)
							pSelectionIndicator->m_visible = false;
						break;
					}
					case ID_STORE_MENU:
					{
						// GET references to menu visuals
						CMenuImage *pEntity = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_ENTITY, true));
						CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_INVENTORY_SELECTION_IMAGE, true));
						CMenuButton *pBuyButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_BUY_BUTTON, true));
						CMenuButton *pSellButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_SELL_BUTTON, true));
						if (!pBuyButton || !pSellButton)
							break;

						// HIDE the selection indicator and store entity
						if (pSelectionIndicator)
							pSelectionIndicator->m_visible = false;
						if (pEntity)
							pEntity->m_visible = false;

						// DISABLE the buy and sell buttons
						pBuyButton->SetEnabled(false);
						pSellButton->SetEnabled(false);
						break;
					}
					case ID_BANK_INVENTORY_MENU:
					{
						// DISABLE all buttons EXCEPT the locker and close buttons
						POSITION pos = NULL;
						CMenuButton *pSelectedButton = NULL;
						for (pos = pMenu->m_childList->GetHeadPosition(); pos != NULL;)
						{
							CObject *pChild = pMenu->m_childList->GetNext(pos);
							CMenuButton *pButton = dynamic_cast<CMenuButton*>(pChild);
							CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(pChild);
							if (pButton)
							{
								CString name = pButton->m_name;
								name = name.MakeLower();

								// DEPRECATED: CHECK for the selected locker
								// NOTE: Now we are always defaulting to the first locker as intially selected
								/*if ((name.Find("locker") == 0) 
								&& !pButton->m_enabled)
									pSelectedButton = pButton;*/

								// INIT enabled state of all bank menu buttons
								if ((name.Find("locker") == -1)
								&& (name.Find("close") == -1))
									pButton->SetEnabled(false);
								else
									pButton->SetEnabled();
							}
							else if (pInventory)
							{
								// HIDE the item selection indicator
								pInventory->ClearSelectedItem();
							}
						}

						// INIT the currently selected locker button
						if (!pSelectedButton)
						{
							pSelectedButton = dynamic_cast<CMenuButton*>(pMenu->m_childList->GetVisualByID(ID_BANK_LOCKER_BUTTON_01, true));
							if (!pSelectedButton) break;
						}
						pSelectedButton->SetEnabled(false);
						pSelectedButton->SetButtonState(MENU_BUTTONSTATE_HOVER);						
						break;
					}
					case ID_PLAYER_INVENTORY_MENU:
					{
						///////////////////////////////////////////////////////
						// GENERAL
						///////////////////////////////////////////////////////

						// GET references to menu visuals
						CMenuWindow *pBankMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_INVENTORY_MENU, true));
						CMenuWindow *pStoreMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_MENU, true));
						CMenuWindow *pTradeMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_MENU, true));

						// DISABLE all buttons EXCEPT the close button
						POSITION pos = NULL;
						for (pos = pMenu->m_childList->GetHeadPosition(); pos != NULL;)
						{
							CObject *pControl = pMenu->m_childList->GetNext(pos);

							// INIT button control
							CMenuButton *pButton = dynamic_cast<CMenuButton*>(pControl);
							if (pButton) 
							{
								CString name = pButton->m_name;
								name = name.MakeLower();
								if (name.Find("close") == -1)
									pButton->SetEnabled(false);
								continue;
							}
							
							// INIT inventory control
							bool bFoundActive = false;
							CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(pControl);
							if (pInventory)
							{
								if (pInventory->m_bActive && !bFoundActive)
								{
									bFoundActive = true;
									pInventory->m_visible = true;
								}
								else
									pInventory->m_visible = false;
								continue;
							}
						}

						// INIT visibility of selection indicators
						CMenuImage *pQuickbarIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_QUICKBAR_SELECTION_IMAGE, true));
						CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_SELECTION_IMAGE, true));
						CMenuImage *pEquippedIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_EQUIPPED_SELECTION_IMAGE, true));
						if (pQuickbarIndicator)
							pQuickbarIndicator->m_visible = false;
						if (pSelectionIndicator)
							pSelectionIndicator->m_visible = false;
						if (pEquippedIndicator)
							pEquippedIndicator->m_visible = false;

						///////////////////////////////////////////////////////
						// QUICKBAR
						///////////////////////////////////////////////////////

						// INIT the visibility of the Add/Remove To Quickbar buttons
						CMenuButton *pAddToQuickbarButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ADD_TO_QUICKBAR_BUTTON, true));
						CMenuButton *pRemoveFromQuickbarButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_REMOVE_FROM_QUICKBAR_BUTTON, true));

						// INIT Quickbar buttons
						if (pBankMenu || pStoreMenu || pTradeMenu)
						{
							if (pRemoveFromQuickbarButton) pRemoveFromQuickbarButton->SetEnabled(false);
							if (pAddToQuickbarButton) pAddToQuickbarButton->SetEnabled(false);
						}
						else
						{// "add to quickbar" and "remove from quickbar" sit side by side, always shown
							if (pRemoveFromQuickbarButton)
							{
								pRemoveFromQuickbarButton->m_visible = true;
								pRemoveFromQuickbarButton->SetEnabled();
							}
							if (pAddToQuickbarButton)
							{
								pAddToQuickbarButton->m_visible = true;
								pAddToQuickbarButton->SetEnabled();
							}
						}

						///////////////////////////////////////////////////////
						// TRADE
						///////////////////////////////////////////////////////

						// INIT the enabled status of all trade buttons
						if (pTradeMenu && pTradeMenu->m_childList)
						{
							for (POSITION pos = pTradeMenu->m_childList->GetHeadPosition(); pos != NULL;)
							{
								CMenuButton *pButton = dynamic_cast<CMenuButton*>(pTradeMenu->m_childList->GetNext(pos));
								if (!pButton || pButton->m_visualID == ID_TRADE_CLOSE_BUTTON) continue;
								pButton->SetEnabled(false);
							}
						}

					}
					default: break;
				};
				break;
			}
			case MENU_SCRIPTFUNC_SETACTIVEINVENTORY:
			{
				globalStatus->SetActiveInventory(
					(MENU_IDS)curLine->GetIntByParam(0), 
					(MENU_IDS)curLine->GetIntByParam(1), 
					true);
				break;
			}
			case MENU_SCRIPTFUNC_SELECTINVENTORYITEM:
			{
				// PARSE script params
				MENU_IDS eMenuID = (MENU_IDS)curLine->GetIntByParam(0);

				// GET reference to menu visual
				CMenuInventory *pInventory = globalStatus->GetActiveInventory(eMenuID);
				if (!pInventory) break;

				// HANDLE inventory item selection event
				globalStatus->OnInventoryItemSelected((MENU_IDS)pInventory->m_visualID);
				break;
			}
			case MENU_SCRIPTFUNC_DEPOSITITEMTOBANK:
			{
				// PARSE out script params
				int iFromMenuID = curLine->GetIntByParam(0);
				int iToMenuID = curLine->GetIntByParam(1);

				// GET references to menu visuals
				CMenuInventory *pPlayerInventory = globalStatus->GetActiveInventory((MENU_IDS)iFromMenuID);
				CMenuInventory *pBankInventory = globalStatus->GetActiveInventory((MENU_IDS)iToMenuID);
				CMenuTextBox *pDepositText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_DEPOSIT_TEXTBOX, true));
				if (!pPlayerInventory
				|| !pBankInventory
				|| !pPlayerInventory->SelectedItem())
					break;

				// GET the bank index
				int iBankIndex = -1;
				switch (pBankInventory->m_visualID)
				{
					default: 
					case ID_BANK_INVENTORY_01: iBankIndex = 0; break;
					case ID_BANK_INVENTORY_02: iBankIndex = 1; break;
					case ID_BANK_INVENTORY_03: iBankIndex = 2; break;
					case ID_BANK_INVENTORY_04: iBankIndex = 3; break;
				}

				// IF there is more than one of an item that can be stored in the bank
				//	AND the quantity menu isn't open,
				//	THEN OPEN it and BREAK
				//	ELSE GET the quantity to transfer and CLOSE the menu
				short iQuantity = 1;
				CMenuInventoryItem *pInventoryItem = pPlayerInventory->SelectedItem();
				ITEMDATA_STRUCT_EX item = pInventoryItem->m_data;

				// ERROR checking: PREVENT deposits of armed items!
				if (pInventoryItem->m_data.m_armed)
				{
					globalStatus->ShowMessageBox("Stronghold Deposit", "You cannot deposit an armed item into the stronghold! Please unequip the item and try again.");
					break;
				}

				if (pInventoryItem
				&& (pInventoryItem->m_data.m_quantity > 1))
				{
					if (!pDepositText)
					{
						// OPEN the quantity menu
						//	NOTE: This script will be called again when that menu is OK'd
						globalStatus->OpenMenu(ID_BANK_DEPOSIT_MENU, false);
						break;
					}
					else 
					{
						// GET the quantity to deposit
						iQuantity = atoi(pDepositText->m_fullText.GetBuffer());
						pDepositText->m_fullText.ReleaseBuffer();

						// CLOSE the quantity menu
						globalStatus->m_runtimeMenuDatabase->CloseByID(ID_BANK_DEPOSIT_MENU, &globalStatus->m_showMenuCursor);

						// ERROR checking: Is a valid quantity specified for deposit?
						if (iQuantity == 0)
						{
							globalStatus->ShowMessageBox("Bank Deposit", "Please specify an amount greater than 0!");
							break;
						}
						else if ((iQuantity > item.m_quantity) || (iQuantity > 30000))
						{
							iQuantity = item.m_quantity;
						}
					}
				}

				// GET a reference to the client-side player object
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if(!pos) break;
				CMovementObj *pMovementObject = dynamic_cast<CMovementObj*>(globalStatus->m_entObjRuntime->GetAt(pos));

				// PERFORM a local update of the bank change
				//	NOTE: This is only temporary - real changes are made server-side!
				item.m_quantity = iQuantity;
				bool bResult = pBankInventory->AddItem(item);
				if (!bResult)
				{
					// IF we couldn't add the item, we can assume the bank locker is at its capacity...
					globalStatus->ShowMessageBox("Bank Inventory", "This bank locker is full, please try another.");
					break;
				}

				// PERFORM a local update of the inventory change
				//	NOTE: This is only temporary - real changes are made server-side!
				pPlayerInventory->RemoveItem(item.m_GLID, iQuantity);

				// IF we're in the client, then send network message
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
				{					
					// UPDATE the server regarding the item deposit
					globalStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						24,
						item.m_GLID,
						pMovementObject->List_networkDefinedID,
						iBankIndex,
						iQuantity,
						NULL);
				}

				// REMOVE the deposited item from the player's Quickbar
				globalStatus->RemoveFromQuickbar(item);
				break;
			}
			case MENU_SCRIPTFUNC_WITHDRAWITEMFROMBANK:
			{
				// PARSE out script params
				int iFromMenuID = curLine->GetIntByParam(0);
				int iToMenuID = curLine->GetIntByParam(1);

				// GET references to menu visuals
				CMenuInventory *pBankInventory = globalStatus->GetActiveInventory((MENU_IDS)iFromMenuID);
				CMenuInventory *pPlayerInventory = globalStatus->GetActiveInventory((MENU_IDS)iToMenuID);
				CMenuTextBox *pWithdrawText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_WITHDRAW_TEXTBOX, true));
				if (!pPlayerInventory
				|| !pBankInventory
				|| !pBankInventory->SelectedItem())
					break;

				// GET the bank index
				int iBankIndex = -1;
				switch (pBankInventory->m_visualID)
				{
					default: 
					case ID_BANK_INVENTORY_01: iBankIndex = 0; break;
					case ID_BANK_INVENTORY_02: iBankIndex = 1; break;
					case ID_BANK_INVENTORY_03: iBankIndex = 2; break;
					case ID_BANK_INVENTORY_04: iBankIndex = 3; break;
				}

				// IF there is more than one of an item that can be removed from the bank
				//	AND the quantity menu isn't open,
				//	THEN OPEN it and BREAK
				//	ELSE GET the quantity to transfer and CLOSE the menu
				int iQuantity = 1;
				CMenuInventoryItem *pBankItem = pBankInventory->SelectedItem();
				ITEMDATA_STRUCT_EX item = pBankItem->m_data;

				if (pBankItem
				&& (pBankItem->m_data.m_quantity > 1))
				{
					if (!pWithdrawText)
					{
						// OPEN the quantity menu
						//	NOTE: This script will be called again when that menu is OK'd
						globalStatus->OpenMenu(ID_BANK_WITHDRAW_MENU, false);
						break;
					}
					else 
					{
						// GET the quantity to deposit
						iQuantity = atoi(pWithdrawText->m_fullText.GetBuffer());
						pWithdrawText->m_fullText.ReleaseBuffer();

						// CLOSE the quantity menu
						globalStatus->m_runtimeMenuDatabase->CloseByID(ID_BANK_WITHDRAW_MENU, &globalStatus->m_showMenuCursor);

						// ERROR checking: Is a valid quantity specified for withdrawal?
						if (iQuantity <= 0)
						{
							globalStatus->ShowMessageBox("Bank Withdrawal", "Please specify an amount greater than 0!");
							break;
						}
						else if ((iQuantity > item.m_quantity) || (iQuantity > 30000))
						{
							iQuantity = item.m_quantity;
						}
					}
				}

				// GET a reference to the client-side player object
				POSITION pos = globalStatus->m_entObjRuntime->FindIndex(0);
				if(!pos) break;
				CMovementObj *pMovementObject = dynamic_cast<CMovementObj*>(globalStatus->m_entObjRuntime->GetAt(pos));

				// PERFORM a local update of the inventory change
				//	NOTE: This is only temporary - real changes are made server-side!
				pBankInventory->RemoveItem(pBankItem->m_data.m_GLID, iQuantity);

				// PERFORM a local update of the bank change
				//	NOTE: This is only temporary - real changes are made server-side!
				item.m_quantity = iQuantity;
				pPlayerInventory->AddItem(item);

				// IF we're in the client, then send the network message
				if (globalStatus->m_multiplayerObject->GetEnabledClient_safe())
				{					
					// UPDATE the server regarding the item deposit
					globalStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
						DPID_SERVERPLAYER,
						25,
						item.m_GLID,
						pMovementObject->List_networkDefinedID,
						iBankIndex,
						iQuantity,
						NULL);
				}
				break;
			}
			case MENU_SCRIPTFUNC_ADDTOQUICKBAR:
			{
				// GET references to menu visuals
				CMenuInventory *pPlayerInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
				CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_QUICKBAR_SELECTION_IMAGE, true));
				CMenuButton *pAddToQuickbarButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ADD_TO_QUICKBAR_BUTTON, true));
				if (!pPlayerInventory
				|| !pSelectionIndicator
				|| !pPlayerInventory->SelectedItem())
					break;

				// ADD the selected item to the player's Quickbar
				globalStatus->AddToQuickbar(pPlayerInventory->SelectedItem());

				// CHECK if we need to disable the "Add To Quickbar" button
				if (pAddToQuickbarButton
				&& globalStatus->m_multiplayerObject->IsQuickbarFull())
				{
					pAddToQuickbarButton->SetEnabled(false);
					pAddToQuickbarButton->m_visibleState = MENU_BUTTONSTATE_UP;
				}
				break;
			}
			case MENU_SCRIPTFUNC_REMOVEFROMQUICKBAR:
			{
				// REMOVE the quickbar slot the player clicked, or else the item selected in the inventory
				CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_QUICKBAR_SELECTION_IMAGE, true));
				CMenuInventory *pPlayerInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
				CMenuButton *pAddToQuickbarButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ADD_TO_QUICKBAR_BUTTON, true));
				int iIndex = globalStatus->m_multiplayerObject->m_iQuickbarSelectedIndex;
				bool bRemoved = false;
				if (iIndex >= 0 && iIndex < NUM_QUICKBAR_SLOTS && pSelectionIndicator && pSelectionIndicator->m_visible)
					bRemoved = globalStatus->RemoveFromQuickbar(0, iIndex);
				else if (pPlayerInventory && pPlayerInventory->SelectedItem())
					bRemoved = globalStatus->RemoveFromQuickbar(pPlayerInventory->SelectedItem()->m_data);

				// HIDE the quickbar selection indicator
				if (pSelectionIndicator)
					pSelectionIndicator->m_visible = false;
				globalStatus->m_multiplayerObject->m_iQuickbarSelectedIndex = -1;

				// a slot is free again, so "add to quickbar" can be used
				if (bRemoved && pAddToQuickbarButton)
				{
					pAddToQuickbarButton->m_visible = true;
					pAddToQuickbarButton->SetEnabled();
				}
				break;
			}
			case MENU_SCRIPTFUNC_SELECTQUICKBARITEM:
			{
				// GET params
				int iQuickbarSlotID = curLine->GetIntByParam(0);

				// GET references to menu visuals
				CMenuImage *pQuickbarSlot = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iQuickbarSlotID, true));
				CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_QUICKBAR_SELECTION_IMAGE, true));
				CMenuButton *pAddToQuickbarButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ADD_TO_QUICKBAR_BUTTON, true));
				CMenuButton *pRemoveFromQuickbarButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_REMOVE_FROM_QUICKBAR_BUTTON, true));
				if (!pQuickbarSlot 
				|| !pSelectionIndicator)
					break;

				// GET the Quickbar selected index
				bool bWasSelected = false;
				CMenuInventoryItem *pItem = NULL;
				for (int i=0; i<NUM_QUICKBAR_SLOTS; i++)
				{
					if (QUICKBAR_SLOT_IDS[0][i] == iQuickbarSlotID)
					{
						if (i == globalStatus->m_multiplayerObject->m_iQuickbarSelectedIndex)
							bWasSelected = true;
						globalStatus->m_multiplayerObject->m_iQuickbarSelectedIndex = i;
						pItem = *(globalStatus->m_multiplayerObject->Quickbar(0, i));
						break;
					}
				}

				if (!pItem 
				|| (bWasSelected && pSelectionIndicator->m_visible))
				{
					// HIDE the image indicator if there's no data in the selected Quickbar slot
					//	or TOGGLE the visibility if it was already selected
					pSelectionIndicator->m_visible = false;
					break;
				}
				else
				{
					// SET the position and visibility of the selection indicator 
					//	to that of the selected item
					pSelectionIndicator->m_visible = true;
					pSelectionIndicator->m_posX = pQuickbarSlot->m_posX;
					pSelectionIndicator->m_posY = pQuickbarSlot->m_posY;

					if (pRemoveFromQuickbarButton)
					{
						pRemoveFromQuickbarButton->m_visible = true;
						pRemoveFromQuickbarButton->SetEnabled();
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_SHOWQUICKBARHOVER:
			{
				int iQuickbarIndex = curLine->GetIntByParam(0);

				// GET item info
				CString strInfo;
				CMenuInventoryItem **ppHoverSlot = globalStatus->m_multiplayerObject->Quickbar(0, iQuickbarIndex);
				if (!ppHoverSlot) break; // index out of range
				CMenuInventoryItem *pInventoryItem = *ppHoverSlot;
				if (!pInventoryItem) break;				
				CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(pInventoryItem->m_data.m_GLID);
				if (pItem)
				{				
					strInfo.Append("Name: " + pItem->m_itemName);
					if (pInventoryItem->m_data.m_dMod > 0)
						strInfo.Append(" +" + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_dMod));
					if (pInventoryItem->m_data.m_quantity > 1)
						strInfo.Append("\r\nQuantity: " + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_quantity));
				}
				else
				{
					strInfo.Append("Name: Unknown Item");
					if (pInventoryItem->m_data.m_dMod > 0)
						strInfo.Append(" +" + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_dMod));
					if (pInventoryItem->m_data.m_quantity > 1)
						strInfo.Append("\r\nQuantity: " + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_quantity));
				}

				// SHOW the hover text window
				globalStatus->ShowHoverText(ID_HOVER_TEXT_MENU, strInfo);
				break;
			}
			case MENU_SCRIPTFUNC_SHOWINVENTORYHOVER:
			{
				// PARSE script params
				int iMenuVisualID = curLine->GetIntByParam(0);
				CString strIsParent = curLine->GetStrByParam(1);
				bool bIsParent = true;
				if (strIsParent.CompareNoCase("false") == 0) bIsParent = false;

				// GET references to menu and inventory objects
				CMenuInventory *pInventory = NULL;
				if (bIsParent)
					pInventory = globalStatus->GetActiveInventory((MENU_IDS)iMenuVisualID);
				else
					pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMenuVisualID, true));
				CMenuWindow *pHoverText = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_HOVER_TEXT_MENU, true));
				if (!pInventory || pInventory->m_items.empty())
					break;

				// GET the inventory hover item
				CMenuInventoryItem *pInventoryItem = pInventory->HoverItem();
				if (!pInventoryItem)
					break;

				// CREATE the hover text message string
				CString strInfo;
				CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(pInventoryItem->m_data.m_GLID);
				if (pItem)
				{	
					strInfo.Append("Name: " + pItem->m_itemName);
					if (pInventoryItem->m_data.m_dMod > 0)
						strInfo.Append(" +" + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_dMod));
					if (pInventoryItem->m_data.m_quantity > 1)
						strInfo.Append("\r\nQuantity: " + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_quantity));
				}
				else
				{
					strInfo.Append("Name: Unknown Item");
					if (pInventoryItem->m_data.m_dMod > 0)
						strInfo.Append(" +" + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_dMod));
					if (pInventoryItem->m_data.m_quantity > 1)
						strInfo.Append("\r\nQuantity: " + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_quantity));
				}

				// SHOW the hover text window
				globalStatus->ShowHoverText(ID_HOVER_TEXT_MENU, strInfo);
				break;
			}
			case MENU_SCRIPTFUNC_HIDEHOVERMENU:
			{
				globalStatus->ShowHoverText(ID_HOVER_TEXT_MENU, "");
				globalStatus->m_runtimeMenuDatabase->CloseByID(ID_HOVER_TEXT_MENU, &globalStatus->m_showMenuCursor);
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEFACTIONBONUS:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				CMenuImage *pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				//Make sure we found the items were looking for
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pEntity || !pImage)
					break;

				//set visible state to false by default
				pImage->m_visible = FALSE;

				if(curLine->GetIntByParam(1) == 0)
				{
					if(pEntity->m_factionBonus1 == TRUE)
						pImage->m_visible = TRUE;
				}
				else if(curLine->GetIntByParam(1) == 1)
				{
					if(pEntity->m_factionBonus2 == TRUE)
						pImage->m_visible = TRUE;
				}
				else if(curLine->GetIntByParam(1) == 2)
				{
					if(pEntity->m_factionBonus3 == TRUE)
						pImage->m_visible = TRUE;
				}

				break;
			}
			case MENU_SCRIPTFUNC_UPDATEFACTIONTAG:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				CMenuImage *pImage = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				//Make sure we found the items were looking for
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pEntity || !pImage)
					break;

				//set visible state to false by default
				pImage->m_visible = FALSE;

				//Don't do anything else if players faction is below 0
				if(pEntity->m_curFaction < 0)
					break;

				if(pEntity->m_curFaction == curLine->GetIntByParam(1))
					pImage->m_visible = TRUE;
				else if(curLine->GetIntByParam(1) > 7 && pEntity->m_curFaction > 7)
					pImage->m_visible = TRUE;

				break;
			}
			case MENU_SCRIPTFUNC_PAGEGM:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				globalStatus->SendChatString("/PAGEGM");
				break;
			}
			case MENU_SCRIPTFUNC_STARTTRADE:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				globalStatus->SendChatString("/TRADEACCEPT");
				break;
			}
			case MENU_SCRIPTFUNC_SEEGROUPINVITE:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				globalStatus->OpenMenu(curLine->GetIntByParam(0), false);
				break;
			}
			case MENU_SCRIPTFUNC_DISPLAYNEARBYPLAYERS:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// GET references to menu visuals
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));;
				if (!pListbox) break;

				// CLEAR the listbox items
				pListbox->ClearList();

				// GET the client side player object
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos) break;
				CMovementObj *pClient = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pClient) break;

				// ADD valid players to the listbox menu
				for (pos = globalStatus->m_entObjRuntime->GetHeadPosition(); pos != NULL;)
				{
					// GET the next player object
					CMovementObj *pMovementObject = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
					if(pMovementObject == pClient) continue;

					// ADD valid player
					if(!pMovementObject->m_isAI)
					{
						CMenuListItem *pItem = new CMenuListItem();
						CMenuText *pName = new CMenuText();

						CString strName = pMovementObject->List_name;
						pName->SetText(strName.GetBuffer());
						strName.ReleaseBuffer();
						pItem->AddText(pName, pListbox->m_width);
						pListbox->AddItem(pItem, 0);
					}
				}
				
				break;
			}
			case MENU_SCRIPTFUNC_SENDMESSAGETOCHANNEL:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				//Make sure we found the items were looking for
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pEntity)
					break;

				if(curLine->GetIntByParam(0) == 0)
					pEntity->m_lastTextArea = "/WHISPER";
				else if(curLine->GetIntByParam(0) == 1)
					pEntity->m_lastTextArea = "/C";
				else if(curLine->GetIntByParam(0) == 2)
					pEntity->m_lastTextArea = "/GROUP";
				else if(curLine->GetIntByParam(0) == 3)
					pEntity->m_lastTextArea = "All";
				
				break;
			}
			case MENU_SCRIPTFUNC_UPDATEMESSAGECHANNEL:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// Get our button were trying to update
				CMenuButton *pChatButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pChatButton)
					break;

				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				//Make sure we found the items were looking for
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pEntity)
					break;

				//Reset the button state
				pChatButton->SetEnabled(true);
				pChatButton->SetButtonState(MENU_BUTTONSTATE_UP);

				if(curLine->GetIntByParam(1) == 0)
				{
					if(strcmp(pEntity->m_lastTextArea, "/WHISPER") == 0)
					{
						// SET the selected button as active
						pChatButton->SetEnabled(false);
						pChatButton->SetButtonState(MENU_BUTTONSTATE_HOVER);
					}
				}
				else if(curLine->GetIntByParam(1) == 1)
				{
					if(strcmp(pEntity->m_lastTextArea, "/C") == 0)
					{
						// SET the selected button as active
						pChatButton->SetEnabled(false);
						pChatButton->SetButtonState(MENU_BUTTONSTATE_HOVER);
					}
				}
				else if(curLine->GetIntByParam(1) == 2)
				{
					if(strcmp(pEntity->m_lastTextArea, "/GROUP") == 0)
					{
						// SET the selected button as active
						pChatButton->SetEnabled(false);
						pChatButton->SetButtonState(MENU_BUTTONSTATE_HOVER);
					}
				}
				else if(curLine->GetIntByParam(1) == 3)
				{
					if(strcmp(pEntity->m_lastTextArea, "All") == 0)
					{
						// SET the selected button as active
						pChatButton->SetEnabled(false);
						pChatButton->SetButtonState(MENU_BUTTONSTATE_HOVER);
					}
				}

				break;
			}
			case MENU_SCRIPTFUNC_SETSCROLLBAR:
			{
				//Convert the calling visual to a text box
				CMenuTextBox *visual = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));;
				if(!visual)
					break;

				if(!visual->m_scrollBar)
					break;

				visual->m_scrollBar->m_curPosition = (int)(visual->m_scrollBar->m_fullSize - visual->m_scrollBar->m_sliderSize) + 2;
				visual->m_scrollBar->Update();

				break;
			}
			case MENU_SCRIPTFUNC_UPDATEVISIBILITYON:
			{
				CMenuButton *pButtonVisual = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(pButtonVisual)
				{
					pButtonVisual->m_visible = TRUE;					
					break;
				}

				CMenuTextBox *pTextboxVisual = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(pTextboxVisual)
				{
					pTextboxVisual->m_visible = TRUE;
					break;
				}

				CMenuImage *pImageVisual = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(pImageVisual)
				{
					pImageVisual->m_visible = TRUE;					
					break;
				}

				break;
			}
			case MENU_SCRIPTFUNC_UPDATEVISIBILITYOFF:
			{
				CMenuButton *pButtonVisual = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(pButtonVisual)
				{
					pButtonVisual->m_visible = FALSE;					
					break;
				}

				CMenuTextBox *pTextboxVisual = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(pTextboxVisual)
				{
					pTextboxVisual->m_visible = FALSE;
					break;
				}

				CMenuImage *pImageVisual = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(pImageVisual)
				{
					pImageVisual->m_visible = FALSE;					
					break;
				}

				break;
			}
			case MENU_SCRIPTFUNC_SHOWSTOREHOVER:
			{
				// GET script params
				int iMenuID = curLine->GetIntByParam(0);

				// GET references to menu and inventory objects
				CMenuInventory *pInventory = globalStatus->GetActiveInventory((MENU_IDS)iMenuID);
				CMenuWindow *pHoverText = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_HOVER_TEXT_MENU, true));
				if (!pInventory || pInventory->m_items.empty())
					break;

				// GET the item that the mouse is currently hovering over
				CMenuInventoryItem *pInventoryItem = pInventory->HoverItem();
				if (!pInventoryItem)
					break;

				// CREATE the hover text message string
				CString strInfo;
				CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(pInventoryItem->m_data.m_GLID);
				if (pItem)
				{	
					strInfo.Append("Name: " + pItem->m_itemName);
					if (pInventoryItem->m_data.m_dMod > 0)
						strInfo.Append(" +" + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_dMod));
					if (pInventoryItem->m_data.m_quantity > 1)
						strInfo.Append("\r\nCost: " + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_quantity));
				}
				else
				{
					strInfo.Append("Name: Unknown Item");
					if (pInventoryItem->m_data.m_dMod > 0)
						strInfo.Append(" +" + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_dMod));
					if (pInventoryItem->m_data.m_quantity > 1)
						strInfo.Append("\r\nCost: " + globalStatus->m_cabLib.ConvToString(pInventoryItem->m_data.m_quantity));
				}

				// SHOW the hover text window
				globalStatus->ShowHoverText(ID_HOVER_TEXT_MENU, strInfo);
				break;
			}
			case MENU_SCRIPTFUNC_SETVISIBLE:
			{
				// PARSE script params
				int iMenuID = curLine->GetIntByParam(0);
				bool bIsVisible = true;
				CString strIsVisible = curLine->GetStrByParam(1);
				if (strIsVisible.CompareNoCase("false") == 0) bIsVisible = false;

				// GET references to menu visuals
				CMenuVisual *pVisual = dynamic_cast<CMenuVisual*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iMenuID, true));
				if (!pVisual) break;

				// SET the visual visibility
				pVisual->m_visible = bIsVisible;
				break;
			}
			case MENU_SCRIPTFUNC_SETACTIVEBANKLOCKER:
			{
				// PARSE script params
				MENU_IDS eLockerButton = (MENU_IDS)curLine->GetIntByParam(0);
				MENU_IDS eInventoryParent = (MENU_IDS)curLine->GetIntByParam(1);
				MENU_IDS eInventory = (MENU_IDS)curLine->GetIntByParam(2);

				// GET references to menu visuals
				CMenuWindow *pBankMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_INVENTORY_MENU, true));
				CMenuButton *pLockerButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(eLockerButton, true));
				if (!pBankMenu || !pLockerButton)
					break;

				// GET the index of the selected locker
				CString strCommand = "/BANK ";
				switch (eLockerButton)
				{
					default:
					case ID_BANK_LOCKER_BUTTON_01: strCommand.Append("0"); break;
					case ID_BANK_LOCKER_BUTTON_02: strCommand.Append("1"); break;
					case ID_BANK_LOCKER_BUTTON_03: strCommand.Append("2"); break;
					case ID_BANK_LOCKER_BUTTON_04: strCommand.Append("3"); break;
				}

				for (POSITION pos = pBankMenu->m_childList->GetHeadPosition(); pos != NULL;)
				{
					CMenuButton *pButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetNext(pos));
					if (!pButton) continue;

					// RESET all button states
					pButton->SetButtonState(MENU_BUTTONSTATE_UP);

					// ENABLE all locker buttons
					CString strName = pButton->m_name;
					if (strName.MakeLower().Find("locker") == 0)
						pButton->SetEnabled();
				}

				// SET the selected button as active
				pLockerButton->SetEnabled(false);
				pLockerButton->SetButtonState(MENU_BUTTONSTATE_HOVER);

				// SEND bank inventory request server command
				globalStatus->SendChatString(strCommand);

				// SET the active inventory for the specified menu
				globalStatus->SetActiveInventory(
					eInventoryParent, 
					eInventory, 
					true);
				break;
			}
			case MENU_SCRIPTFUNC_HIDECHATMENU:
			{
				// PARSE script params
				const int DELTA_Y = 135;
				MENU_IDS eChatMenu = (MENU_IDS)curLine->GetIntByParam(0);

				// GET references to menu visuals
				CMenuButton *pHideButton = NULL, *pShowButton = NULL;
				CMenuWindow *pWindow = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(eChatMenu, true));
				switch (eChatMenu)
				{
					default:
					case ID_SYSTEM_CHAT_MENU:
					{
						pHideButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SYSTEM_CHAT_HIDE_BUTTON, true));
						pShowButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SYSTEM_CHAT_SHOW_BUTTON, true));
						break;
					}
					case ID_PLAYER_CHAT_MENU:
					{
						pHideButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_CHAT_HIDE_BUTTON, true));
						pShowButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_CHAT_SHOW_BUTTON, true));
						break;
					}
				}
				if (!pWindow
				|| !pHideButton 
				|| !pShowButton) 
					break;

				// RETRACT the chat window
				pWindow->m_posY += DELTA_Y;

				// SET enabled status of the chat's hide/show buttons
				pHideButton->SetEnabled(false);
				pShowButton->SetEnabled();
				break;
			}
			case MENU_SCRIPTFUNC_SHOWCHATMENU:
			{
				// PARSE script params
				const int DELTA_Y = 135;
				MENU_IDS eChatMenu = (MENU_IDS)curLine->GetIntByParam(0);

				// GET references to menu visuals
				CMenuButton *pHideButton = NULL, *pShowButton = NULL;
				CMenuWindow *pWindow = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(eChatMenu, true));
				switch (eChatMenu)
				{
					default:
					case ID_SYSTEM_CHAT_MENU:
					{
						pHideButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SYSTEM_CHAT_HIDE_BUTTON, true));
						pShowButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SYSTEM_CHAT_SHOW_BUTTON, true));
						break;
					}
					case ID_PLAYER_CHAT_MENU:
					{
						pHideButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_CHAT_HIDE_BUTTON, true));
						pShowButton = dynamic_cast<CMenuButton*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_CHAT_SHOW_BUTTON, true));
						break;
					}
				}
				if (!pWindow
				|| !pHideButton 
				|| !pShowButton) 
					break;

				// RETRACT the chat window
				pWindow->m_posY -= DELTA_Y;

				// SET enabled status of the chat's hide/show buttons
				pHideButton->SetEnabled();
				pShowButton->SetEnabled(false);
				break;
			}
			case MENU_SCRIPTFUNC_OPENSQUADMENU:
			{
				// IF this player is not part of a squad...
				if (globalStatus->m_multiplayerObject->m_squadName.GetLength() <= 0)
				{
					// SHOW the create squad menu
					globalStatus->OpenMenu(ID_CREATE_SQUAD_MENU, false);

					// GET references to menu visuals
					CMenuTextBox *pSquadNameText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_CREATE_SQUAD_TEXTBOX, true));
					if (!pSquadNameText) break;

					// CLEAR the text box and RESET the cursor position
					pSquadNameText->SetText("");
					pSquadNameText->WrapLine();
				}
				else
				{
					// SHOW the squad status menu
					if (!globalStatus->OpenMenu(ID_SQUAD_STATUS_MENU, false))
						break;

					// SEND squad info request server command
					globalStatus->SendChatString("/REQUESTSQUADINFO");

					// GET references to menu visuals
					CMenuWindow *pSquadMenu = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SQUAD_STATUS_MENU, true));
					if (!pSquadMenu) break;

					// SET the title of the menu to the squad name
					if (globalStatus->m_multiplayerObject->m_squadName.GetLength() == 0)
					{
						if(pSquadMenu->m_titleText)
							pSquadMenu->m_titleText->SetText("No Squad");
					}
					else
					{
						if(pSquadMenu->m_titleText)
							pSquadMenu->m_titleText->SetText(globalStatus->m_multiplayerObject->m_squadName + " Squad");
					}
				}
				break;
			}
			case MENU_SCRIPTFUNC_CREATESQUAD:
			{
				// PARSE script params
				int iEditBoxID = curLine->GetIntByParam(0);

				// GET references to menu visuals
				CMenuTextBox *pSquadName = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iEditBoxID, true));
				if (!pSquadName) break;

				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if (!pos) break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if (!pEntity || (pEntity->List_controlType != 0)) break;

				// ERROR checking: Bounds checking on name length
				if (pSquadName->m_fullText.GetLength() <= 0)
				{
					// OPEN the menu back up for them to try again
					globalStatus->ShowMessageBox("Create Squad", 
						"Please enter a squad name to create a squad.");
					globalStatus->OpenMenu(ID_CREATE_SQUAD_MENU, false);
					break;
				}
				else if (pSquadName->m_fullText.GetLength() > SQUAD_NAME_MAX_LENGTH)
				{
					// OPEN the menu back up for them to try again
					globalStatus->ShowMessageBox("Create Squad", 
						"Squad names must be less than "
						+ globalStatus->m_cabLib.ConvToString(SQUAD_NAME_MAX_LENGTH+1)
						+ " characters in length!");
					globalStatus->OpenMenu(ID_CREATE_SQUAD_MENU, false);
					break;
				}

				// SEND party invite server command
				CString strCommand = "/CREATESQUAD " + pSquadName->m_fullText;
				globalStatus->SendChatString(strCommand);
				break;
			}
			case MENU_SCRIPTFUNC_USESKILL:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// GET the client's entity
				POSITION pos = globalStatus->m_entObjRuntime->GetHeadPosition();
				if(!pos)
					break;
				CMovementObj *pEntity = (CMovementObj*)globalStatus->m_entObjRuntime->GetNext(pos);
				if(!pEntity || (pEntity->List_controlType != 0))
					break;

				int abilityVal = curLine->GetIntByParam(0);
				int skillLevel = curLine->GetIntByParam(1);
				int skillVal = curLine->GetIntByParam(2);

				globalStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,
																	33,
																	abilityVal,
																	pEntity->List_networkDefinedID,
																	skillLevel,
																	skillVal,
																	NULL);

				break;
			}
			case MENU_SCRIPTFUNC_SENDCOMMAND:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;
				
				globalStatus->SendChatString(curLine->GetStrByParam(0));
				break;
			}
			case MENU_SCRIPTFUNC_CLANJOINREQUEST:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// GET references to menu visuals
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if (!pListbox || !pListbox->m_currentSelection) break;
				CMenuText *pText = dynamic_cast<CMenuText*>(pListbox->m_currentSelection->m_textList.GetHead());
				if (!pText) break;
				
				// SEND clancreate server command
				CString strCommand = "/REQUESTJOINCLAN " + pText->m_text;
				globalStatus->SendChatString(strCommand);

				break;
			}
			case MENU_SCRIPTFUNC_CLANCREATE:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// PARSE script params
				int iEditBoxID = curLine->GetIntByParam(0);
				// GET references to menu visuals
				CMenuTextBox *pName = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(iEditBoxID, true));
				
				// SEND clancreate server command
				CString strCommand = "/CREATECLAN " + pName->m_fullText;
				globalStatus->SendChatString(strCommand);

				break;
			}
			case MENU_SCRIPTFUNC_CLANREMOVEMEMBER:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// GET references to menu visuals
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if (!pListbox || !pListbox->m_currentSelection) break;
				CMenuText *pText = dynamic_cast<CMenuText*>(pListbox->m_currentSelection->m_textList.GetHead());
				if (!pText) break;
				
				// SEND clancreate server command
				CString strCommand = "/REMOVECLANMEMBER " + pText->m_text;
				globalStatus->SendChatString(strCommand);
			
				break;
			}
			case MENU_SCRIPTFUNC_SWAPMENUS:
			{
				CMenuWindow *pShowWindow = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				CMenuWindow *pHideWindow = dynamic_cast<CMenuWindow*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true));
				if(!pShowWindow || !pHideWindow)
					break;

				pShowWindow->m_posX = -4;
				pHideWindow->m_posX = -304;

				break;
			}
			case MENU_SCRIPTFUNC_TESTAH:
			{
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));

				if(pListbox)
				{
					CMenuListItem *pItem = NULL;
					CMenuText *pItemName = NULL, *pSellValue = NULL, *pQuantity = NULL, *pPlayerName = NULL;

					pItem = new CMenuListItem();
					pItemName = new CMenuText();
					pSellValue = new CMenuText();
					pQuantity = new CMenuText();
					pPlayerName = new CMenuText();

					pItemName->m_text = "Very long item name for test reference";
					pSellValue->m_text = "                                          10000000";
					pQuantity->m_text = "                          100";
					pPlayerName->m_text = "Test Character Name";

					// Add menu item to CMenuListBox
					pItem->m_textList.AddHead(pPlayerName);
					pItem->m_textList.AddHead(pQuantity);
					pItem->m_textList.AddHead(pSellValue);
					pItem->m_textList.AddHead(pItemName);

					pListbox->AddItem(pItem);
				}

				break;
			}
			case MENU_SCRIPTFUNC_BUYFROMAH:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// Get the listbox and make sure we have a selected item in it
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pListbox || !pListbox->m_currentSelection)
					break;

				CMenuText *pTextPlayer = NULL;
				CMenuText *pTextItem = NULL;
				CMenuText *pTextPrice = NULL;
				CMenuText *pTextQuantity = NULL;

				for(POSITION pos = pListbox->m_currentSelection->m_textList.GetHeadPosition(); pos != NULL;)
				{
					if(!pTextItem)
						pTextItem = (CMenuText *)pListbox->m_currentSelection->m_textList.GetNext(pos);
					else if(!pTextPrice)
						pTextPrice = (CMenuText *)pListbox->m_currentSelection->m_textList.GetNext(pos);
					else if(!pTextQuantity)
						pTextQuantity = (CMenuText *)pListbox->m_currentSelection->m_textList.GetNext(pos);
					else if(!pTextPlayer)
						pTextPlayer = (CMenuText *)pListbox->m_currentSelection->m_textList.GetNext(pos);
				}

				CString  strPrice,strQuantity; 
				const char *buf = pTextPrice->m_text.GetBuffer();
				unsigned int length = pTextPrice->m_text.GetLength();
				unsigned int dpos = 0;

				//Get rid of all the spaces for the price
				for(dpos = 1; dpos != 0;)
				{
					ParseByDelimiter(dpos,buf,length,' ',strPrice);
					if(strPrice != "")
						dpos = 0;
				}

				//Get rid of all the spaces for the quantity
				buf = pTextQuantity->m_text.GetBuffer();
				length = pTextQuantity->m_text.GetLength();
				for(dpos = 1; dpos != 0;)
				{
					ParseByDelimiter(dpos,buf,length,' ',strQuantity);
					if(strQuantity != "")
						dpos = 0;
				}

				//Send the message across the server
				CString strMessage = "/UC_BUYITEM " + pTextPlayer->m_text + "," + pTextItem->m_text + "," + strPrice + "," + strQuantity;
				globalStatus->SendChatString(strMessage);

				pListbox->RemoveItem(pListbox->m_currentSelection->m_index);

				break;
			}
			case MENU_SCRIPTFUNC_ADDTOAH:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// GET references to menu visuals
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				CMenuTextBox *pAmountText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(1), true));
				CMenuTextBox *pQuantityText = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(2), true));
				if (!pAmountText
				|| !pQuantityText
				|| !pInventory
				|| !pInventory->SelectedItem())
					break;

				// GET the amount the player is trying to sell
				int iAmount = atoi(pAmountText->m_linesOfText[0].m_text);
				int iQuantity = atoi(pQuantityText->m_linesOfText[0].m_text);
				CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(pInventory->SelectedItem()->m_data.m_GLID);
				//Unable to locat item check
				if(!pItem) break;

				if(iQuantity < 1)
				{
					globalStatus->ShowMessageBox("Auction Item", "Please specify a quantity greater than 0 to auction!");
					break;
				}
				else if(iQuantity > 30000)
				{
					globalStatus->ShowMessageBox("Auction Item", "Please specify a quantity less than 30,000 to auction!");
					break;
				}
				else if(iAmount < 1)
				{
					globalStatus->ShowMessageBox("Auction Item", "Please specify an amount greater than 0 to auction!");
					break;
				}

				// ERROR checking: Can't auction equipped items!
				if(pInventory->SelectedItem()->m_data.m_armed)
				{
					globalStatus->ShowMessageBox("Auction Item", "Please unequip that item before trying to auction it!");
					break;
				}

				//[ITEMNAME] [QUANTITY] [PRICE]
				//Send the message across the server
				CString strMessage = "/UC_SELLITEM " + pItem->m_itemName + "," + pQuantityText->m_linesOfText[0].m_text + "," + pAmountText->m_linesOfText[0].m_text;
				globalStatus->SendChatString(strMessage);
				
				break;
			}
			case MENU_SCRIPTFUNC_POPULATEAHSTATS:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				// Get the listbox and make sure we have a selected item in it
				CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pListbox || !pListbox->m_currentSelection)
					break;

				CMenuText *pTextItem = NULL;
				POSITION pos = pListbox->m_currentSelection->m_textList.GetHeadPosition();
				pTextItem = (CMenuText *)pListbox->m_currentSelection->m_textList.GetNext(pos);
				if(!pTextItem) break;

				//[ITEMNAME]
				//Send the message across the server
				CString strMessage = "/UC_SALESSTATS " + pTextItem->m_text;
				globalStatus->SendChatString(strMessage);

				break;
			}
			case MENU_SCRIPTFUNC_LISTBYITEMAH:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				CMenuTextBox *pItemNameEdit = dynamic_cast<CMenuTextBox*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pItemNameEdit)
					break;

				//Send the message across the server
				CString strMessage = "/UC_LISTBYITEM " + pItemNameEdit->m_fullText;
				globalStatus->SendChatString(strMessage);

				break;
			}
			case MENU_SCRIPTFUNC_NEXTITEMSAH:
			{
				// IF we're not in the client, then BREAK
				if(!globalStatus->m_multiplayerObject->GetEnabledClient_safe())
					break;

				CMenuText *pDateText = dynamic_cast<CMenuText*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(curLine->GetIntByParam(0), true));
				if(!pDateText)
					break;

				//Send the message across the server
				CString strMessage = "/UC_LISTRECENT " + pDateText->m_text;
				globalStatus->SendChatString(strMessage);

				break;
			}
		}

		//Goto the next line
		if(curLine)
			curLine = curLine->m_pNextLine;
	}
}

CMenuScript::~CMenuScript()
{
	if(!m_reference)
	{
		CScriptLine *curLine = m_pFirstLine;

		if(curLine)
		{
			//Loop through the script
			while(curLine->m_pNextLine)
			{
				//Move to the next pointer and use its previous to delete the current object
				curLine = curLine->m_pNextLine;
				SafeDelete(curLine->m_pPrevLine);
			}

			//Delete the only remaining line
			SafeDelete(curLine);
			m_pFirstLine = NULL;
		}

		
	}
}

int CScriptLine::GetIntByParam(int paramNum)
{
    if(paramNum >= m_allocatedParamCount || paramNum < 0)
	   return -1;

	if(m_strParams[paramNum].CompareNoCase("$FPS")==0)
	   return globalStatus->m_exactFPSHiRes;
	else if(m_strParams[paramNum].CompareNoCase("$Ping")==0)
		return rendermanager->m_clientNet->CURPING;

    return atoi(m_strParams[paramNum]);
}

CString CScriptLine::GetStrByParam(int paramNum)
{
    if(paramNum >= m_allocatedParamCount || paramNum < 0)
	   return "";

	if(m_strParams[paramNum].CompareNoCase("$FPS")==0)
	   return globalStatus->m_cabLib.ConvToString(globalStatus->m_exactFPSHiRes);
	else if(m_strParams[paramNum].CompareNoCase("$MONKEY")==0)
	   return globalStatus->m_currentWorkingFile;

    return m_strParams[paramNum];
}

float CScriptLine::GetFloatByParam(int paramNum)
{
    if(paramNum >= m_allocatedParamCount || paramNum < 0)
	   return 0;

	if(m_strParams[paramNum].CompareNoCase("$FPS")==0)
	   return (float)globalStatus->m_exactFPSHiRes;

    return atof(m_strParams[paramNum]);
}

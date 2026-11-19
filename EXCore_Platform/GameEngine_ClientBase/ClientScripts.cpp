#include "StdAfx.h"
#include "clientscripts.h"
#include "globalstatusvariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString SanitizeIn(CString text)
{
	text.Replace("[OpenParan]","(");
	text.Replace("[CloseParan]",")");
    return text;
}

CClientScripts::CClientScripts(void)
{
}

CClientScripts::~CClientScripts(void)
{
}

bool CClientScripts::ScriptProcessor(CString script, 
									 void * gStatusMem, 
									 CString & error, 
									 CString miscText)
{
	int cursorPosition = 0;
	int scrLength = script.GetLength();
    if(scrLength < 2)
	   return false;

    char * cArray = script.GetBuffer();
	
	while(cursorPosition < scrLength)
	{
          CString command,paramsblock;

		  if(GetNextFunctionData(cursorPosition,scrLength, cArray, command, paramsblock)==true)
		  {//aquired function data
             if(DoFunction(command,paramsblock,gStatusMem,miscText)==false)
			 {//parse error
				error = "DoFunction() Failed:"+command+" Params:"+paramsblock;
			    return false;
			 }//end parse error
		  }//end aquired function data
		  else
		  {//report bad format (parse error)
			  error = "GetNextFunctionData() Failed";
		      return false;
		  }//end report bad format (parse error)
	}
    return true;
}

bool CClientScripts::GetNextFunctionData(int & currentPosition,int & scriptLength, char * cArray, CString & cmd, CString & paramsBlock)
{
   int flag = 0;
   while(currentPosition < scriptLength)
   {
      if(cArray[currentPosition] == '(')
	  {
	     flag = 1;
		 currentPosition++;
         continue;
	  }

	  if(cArray[currentPosition] == ')')
	  {
	     currentPosition++;
		 if(cmd.GetLength() == 0)
		    return false;

	     return true;
	  }

      if(cArray[currentPosition] == ' ' && cmd.GetLength() == 0)
	  {
         currentPosition++;
         continue;
	  }

	  if(cArray[currentPosition] != 13/* && cArray[currentPosition] != 10*/)
	  {
	     if(flag == 0)
		 {
			if(cArray[currentPosition] != 10)
				cmd += cArray[currentPosition];
		 }
		 else
		 {
		         paramsBlock += cArray[currentPosition];
		 }
	  }
	  currentPosition++;
   }

   return false;
}


bool CClientScripts::DoFunction(CString & command, 
								CString & params,
								void * memoryObj,
								CString & miscText)
{
	GlobalStatusVariables * gstatus = (GlobalStatusVariables *)memoryObj;

	FillParams(params);
	
	if(command.CompareNoCase("OpenMenu")==0)
	{//jounal entry 19 vars
		gstatus->OpenMenu(atoi(m_params[0]), false);
	}//end journal entry 
	else if(command.CompareNoCase("CloseAllMenus")==0)
	{//CloseAllMenus
		for(POSITION posLoc = gstatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
		{//clear existing menus
			CMenu3DObj * menu = (CMenu3DObj *)gstatus->m_runtimeMenuDataBase->GetNext(posLoc);
	        if(menu->m_closeOnEsc)
				menu->CloseMenu(&gstatus->m_globalCursorPos);
		}//end clear existing menus
	}//end CloseAllMenus
	else if(command.CompareNoCase("SetMenuObjectTexture")==0)
	{//set SetMenuObjectTexture
		CMenu3DObj * menu = gstatus->m_runtimeMenuDataBase->SetTextureToObjectByName(m_params[0],m_params[1],atoi(m_params[2]));
		if(menu)
			gstatus->LoadTexturesForMenu(menu);
	}//end SetMenuObjectTexture
	else if(command.CompareNoCase("AddMiscTextToList")==0)
	{//THIS IS NOT CHAR MONEY, THIS IS QUEST DATA
	  CMenuText* visual = (CMenuText*)gstatus->m_runtimeMenuDatabase->GetVisualByID(91, true);
	  if(visual)
	  {
		  //Copy the new text
		  visual->m_text = miscText;

		  //Delete Text Mesh
		  if(visual->m_textMesh)
		  {
			  visual->m_textMesh->SafeDelete();
			  delete visual->m_textMesh;
			  visual->m_textMesh = NULL;
		  }
	  }
	}//END SEE ABOVE COMMENT
	else if(command.CompareNoCase("FillAH")==0)
	{
		//open menu
		if(atoi(m_params[0]) > 0)
			gstatus->OpenMenu(atoi(m_params[0]), false);

		CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(gstatus->m_runtimeMenuDatabase->GetVisualByID(atoi(m_params[1]), true));
		if(pListbox)
		{
			if(pListbox->m_scrollBar)
			{
				pListbox->m_scrollBar->m_curPosition = 0;//(int)(pListbox->m_scrollBar->m_fullSize - pListbox->m_scrollBar->m_sliderSize) + 2;
				pListbox->m_scrollBar->Update();
			}

			pListbox->ClearList();
			unsigned int len = m_params[2].GetLength();
			char *buf = m_params[2].GetBuffer();
			unsigned int dpos = 0;
			CMenuText *pFirstDate = dynamic_cast<CMenuText*>(gstatus->m_runtimeMenuDatabase->GetVisualByID(2778, true));
			CMenuText *pLastDate = dynamic_cast<CMenuText*>(gstatus->m_runtimeMenuDatabase->GetVisualByID(2779, true));
			
			CString itemName = NULL, quantity = NULL, price = NULL, playerName = NULL, date = NULL;
			ParseByDelimiter(dpos,buf,len,'&',itemName);
			ParseByDelimiter(dpos,buf,len,'&',quantity);
			ParseByDelimiter(dpos,buf,len,'&',price);
			ParseByDelimiter(dpos,buf,len,'&',playerName);
			ParseByDelimiter(dpos,buf,len,'&',date);
			CMenuListItem *pItem = new CMenuListItem();
			CMenuText *pItemName = new CMenuText();
			CMenuText *pQuantity = new CMenuText();
			CMenuText *pPrice = new CMenuText();
			CMenuText *pPlayerName = new CMenuText();

			pItemName->m_text = SanitizeIn(itemName);
			pQuantity->m_text = "                          " + quantity;
			pPrice->m_text = "                                          " + price;
			pPlayerName->m_text = playerName;
			pLastDate->m_text = pFirstDate->m_text = date;

			// Add first item to the listbox
			pItem->m_textList.AddHead(pPlayerName);
			pItem->m_textList.AddHead(pQuantity);
			pItem->m_textList.AddHead(pPrice);
			pItem->m_textList.AddHead(pItemName);

			pListbox->AddItem(pItem);

			while(dpos < len)
			{
				CString LoopitemName = NULL, Loopquantity = NULL, Loopprice = NULL, LoopplayerName = NULL, Loopdate = NULL;
				ParseByDelimiter(dpos,buf,len,'&',LoopitemName);
				ParseByDelimiter(dpos,buf,len,'&',Loopquantity);
				ParseByDelimiter(dpos,buf,len,'&',Loopprice);
				ParseByDelimiter(dpos,buf,len,'&',LoopplayerName);
				ParseByDelimiter(dpos,buf,len,'&',Loopdate);
				CMenuListItem *pLoopItem = new CMenuListItem();
				CMenuText *pLoopItemName = new CMenuText();
				CMenuText *pLoopQuantity = new CMenuText();
				CMenuText *pLoopPrice = new CMenuText();
				CMenuText *pLoopPlayerName = new CMenuText();

				pLoopItemName->m_text = SanitizeIn(LoopitemName);
				pLoopQuantity->m_text = "                          " + Loopquantity;
				pLoopPrice->m_text = "                                          " + Loopprice;
				pLoopPlayerName->m_text = LoopplayerName;
				pLastDate->m_text = Loopdate;

				// Add first item to the listbox
				pLoopItem->m_textList.AddHead(pLoopPlayerName);
				pLoopItem->m_textList.AddHead(pLoopQuantity);
				pLoopItem->m_textList.AddHead(pLoopPrice);
				pLoopItem->m_textList.AddHead(pLoopItemName);

				pListbox->AddItem(pLoopItem);
			}
		}
	}
	else if(command.CompareNoCase("FillList")==0)
	{//camma dilimited
	  //open menu if > 0
	  if(atoi(m_params[0]) > 0)
	  {//open menu
			gstatus->OpenMenu(atoi(m_params[0]), false);

	  }
		
		
	  CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(gstatus->m_runtimeMenuDatabase->GetVisualByID(atoi(m_params[1]), true));
	  if(pListbox)
	  {
		  int		len		= m_params[2].GetLength();
		  char	*	buf		= m_params[2].GetBuffer();
		
		  CString temp;
		  for(int iLoop=0;iLoop<len;iLoop++)
		  {
				if(buf[iLoop] == '&')
				{
					if(temp.GetLength() > 0)
					{//add to list
						CMenuListItem	* pItem	= new CMenuListItem();
						CMenuText		* pName	= new CMenuText();

						temp = SanitizeIn(temp);

						// Read in
						pName->m_text			= temp;

						// Add menu item to CMenuListBox
						pItem->m_textList.AddHead(pName);

						pListbox->AddItem(pItem);
					}
					temp = "";
					continue;
				}
				temp += buf[iLoop];
		  }
		  if(temp.GetLength() > 0)
		  {//add to list
			  CMenuListItem	* pItem	= new CMenuListItem();
			  CMenuText		* pName	= new CMenuText();

			  temp = SanitizeIn(temp);

			  // Read in
			  pName->m_text			= temp;

			  // Add menu item to CMenuListBox
			  pItem->m_textList.AddHead(pName);

			  pListbox->AddItem(pItem);
		  }
		  temp = "";
	  }
	}//END SEE ABOVE COMMENT
	else if(command.CompareNoCase("StartSoundtrack")==0)
	{//start sound track
		gstatus->m_soundTrackList->Play(atoi(m_params[0]),gstatus->GL_pathGameSounds);
	}//end start sound track
	else if(command.CompareNoCase("ForcePad")==0)
	{//start sound track
		CMovementObj * cmovPtr = gstatus->m_entObjRuntime->GetObjByControlType(0);
		if(cmovPtr)
		{
			cmovPtr->List_externalVectorTemp.x = atof(m_params[0]);
			cmovPtr->List_externalVectorTemp.y = atof(m_params[1]); 
			cmovPtr->List_externalVectorTemp.z = atof(m_params[2]); 
			//cmovPtr->List_externalVectorTemp = gstatus->m_matLib.Normalize(cmovPtr->List_externalVectorTemp);
		}
	}//end start sound track
	else if(command.CompareNoCase("ArmItem")==0)
	{//arm item
		CMovementObj * cmovPtr = gstatus->m_entObjRuntime->GetObjByControlType(0);
        if(cmovPtr)
			if(cmovPtr->List_controlType == 0)//verify primary
				gstatus->ArmEntityItemSys(cmovPtr,atoi(m_params[0]),0);
	}//end arm item
    else if(command.CompareNoCase("DoExplosionEffect")==0)
	{//do exp effect
		CMovementObj * cmovPtr = gstatus->m_entObjRuntime->GetObjByControlType(0);
		if(cmovPtr)
			if(cmovPtr->List_controlType == 0)//verify primary
				gstatus->DoExpEffect(atoi(m_params[0]),cmovPtr->List_networkDefinedID,gstatus->m_explosionRuntimeList,atof(m_params[1]),atof(m_params[2]));
	}//end do exp effect
	else if(command.CompareNoCase("Console")==0)
	{//console command
	   CMovementObj * cmovPtr = gstatus->m_entObjRuntime->GetObjByControlType(0);
       if(cmovPtr)
	      if(cmovPtr->List_controlType == 0)//verify primary
			 gstatus->m_netGameLogicObject->SendTextMsg(   DPNSEND_GUARANTEED,
														   DPID_SERVERPLAYER,//can be NULL if client
														   cmovPtr->List_networkDefinedID,
													       NULL,
														   0,
														   m_params[0]);
	}//end console command
	else if(command.CompareNoCase("UseItem")==0)
	{//use item command to server
		CMovementObj * cmovPtr = gstatus->m_entObjRuntime->GetObjByControlType(0);
		if(cmovPtr)
		{//valid movement object
			if(cmovPtr->List_controlType == 0)
			{//verify primary
				gstatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
															 DPID_SERVERPLAYER,
															 27,//use item
															 atoi(m_params[0]),//glid
															 cmovPtr->List_networkDefinedID,
															 0,
															 0,
															 NULL);
			}//end verify primary
		}//end valid movement object
	}//end use item command to server
	else if(command.CompareNoCase("SetCountDownTimer")==0)
	{//set timer
        gstatus->m_globalCountDownDuration = atoi(m_params[0]);
        gstatus->m_globalCountDownNotifierStamp = GetTickCount();
	}//end set timer
	else if(command.CompareNoCase("LocalTeleport")==0)
	{//move is local within world, but if done from the server a server function must be called to give player permision to move the distance
		CMovementObj * cmovPtr = gstatus->m_entObjRuntime->GetObjByControlType(0);
        if(cmovPtr)
		  if(cmovPtr->List_controlType == 0)//verify primary
		  {//teleport
			 cmovPtr->List_baseFrame->SetPosition2(NULL,atof(m_params[0]),atof(m_params[1]),atof(m_params[2]));
			 cmovPtr->List_baseFrame->GetPosition(NULL,&cmovPtr->List_movementDB->List_lastPosition);
			 cmovPtr->m_collisionRaySys->InitOnSpawn(cmovPtr->List_baseFrame,cmovPtr->List_skeletalObject,&gstatus->m_matLib);
		  }//end teleport
	}//end local teleport
	else if(command.CompareNoCase("EnableMenuVisual")==0)
	{//enable visual
		CMenuVisual* visual = gstatus->m_runtimeMenuDatabase->GetVisualByName(m_params[0], true);
        if(visual)
		   visual->m_visible = (bool)atoi(m_params[1]);
	}//end enable visual
	else if(command.CompareNoCase("UpdateCharName")==0)
	{//update char name
		CMovementObj * cmovPtr = gstatus->m_entObjRuntime->GetObjByControlType(0);
		if(cmovPtr)
		{//movement object
			if(cmovPtr->List_controlType == 0)//verify primary
			{//update name
				cmovPtr->List_name = m_params[0];
				if(cmovPtr->m_optionalTextMesh)
				{//refresh please
					cmovPtr->m_optionalTextMesh->SafeDelete();
					delete cmovPtr->m_optionalTextMesh;
					cmovPtr->m_optionalTextMesh = 0;
				}//end refresh
			}//end update name
		}//end movement object
	}//end update char name
	else if(command.CompareNoCase("UpdateGroupCFG")==0)
	{//updage group cfg
		CMovementObj * cmovPtr = gstatus->m_entObjRuntime->GetObjByNetworkID(atoi(m_params[0]));
        if(cmovPtr)
		{//update cfg
			CMovementObj *DBPtr = gstatus->m_entObjList->GetByID(cmovPtr->List_refModel);
			if(DBPtr)
			{//valid DBptr
				if(atoi(m_params[1]) > 0)
					cmovPtr->ArmItemByGLID(atoi(m_params[1]),DBPtr,gstatus->m_itemManager,NULL);
				else
					cmovPtr->DisarmItemByGLID(abs(atoi(m_params[1])), TRUE, DBPtr, NULL);
			}//end valid DBptr
		}//end update cfg
	}//end update group cfg

	return true;
}


bool CClientScripts::FillParams(CString & paramBlock)
{
     int count = paramBlock.GetLength();
	 char * arrayLoc = paramBlock.GetBuffer();
     int trace = 0;
	 int paramIndex = 0;
	 m_params[paramIndex] = "";
     while(trace < count)
	 {
         if(arrayLoc[trace] == ' ' && m_params[paramIndex].GetLength()==0)
		 {
            trace++;
		    continue;
		 }

		 if(arrayLoc[trace] == 13)
		 {
		    trace++;
			continue;
		 }

	     if(arrayLoc[trace] == ',')
		 {
            trace++;
		    paramIndex++;
			m_params[paramIndex] = "";
            continue;
		 }
		 
		 m_params[paramIndex] += arrayLoc[trace];
		 trace++;
	 }
	 return true;
}

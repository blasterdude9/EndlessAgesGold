#include "StdAfx.h"
#include "direct.h"
#include "menumanagement.h"
#include "renderingmanagement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GlobalStatusVariables * globalStatus;


CMenuManagement::CMenuManagement(GlobalStatusVariables * gStatus,
								 SceneAssembly * sceneAssembly,
								 CClientNetworkingLogic * clientNet)
{
	m_gStatus					= gStatus;
	globalStatus				= gStatus;
	m_sceneAssembly				= sceneAssembly;
	m_clientNet					= clientNet;
	long currentTime			= GetTickCount();
	m_clickHandlerStamp			= currentTime;
	m_clickStamp                = currentTime;
	m_groupLastRequest          = currentTime;
	m_durationToServerStamp     = currentTime;
	m_durationToServer			= 2500;

	m_hudFontSmall				= NULL;
	m_hudFontTitle				= NULL;
	m_hudFontDevice				= NULL;
	m_minimapMode				= 0;
	m_minimapZoomRadius			= 600.0f;
	m_minimapNorthUp			= false;
	m_questTrackerVisible		= true;
	m_questJournalSynced		= false;
	m_hudInvRequestStamp		= 0;
	m_lastHudKeyTime			= 0;
	m_lastMouseScreenX			= 0.0f;
	m_lastMouseScreenY			= 0.0f;
	m_hudToastTime				= 0;
	m_zoneMapTex				= NULL;
	m_zoneMapDevice				= NULL;
	m_zoneMapLoaded				= false;
	m_zoneMapHasBounds			= false;
	m_zoneMapMinX = m_zoneMapMaxX = m_zoneMapMinZ = m_zoneMapMaxZ = 0.0f;
	m_zoneMapQuestStamp			= 0;
	m_zoneMapLevels				= 0;
	for (int lv = 0; lv < 8; lv++) { m_zoneMapLevelTex[lv] = NULL; m_zoneMapLevelLo[lv] = m_zoneMapLevelHi[lv] = 0.0f; m_wmFloorT[lv] = m_wmFloorB[lv] = 0.0f; }
	m_zoneMapPlayerLevel		= 0;
	m_worldMapViewLevel			= -1;
	m_wmFloorL = m_wmFloorR = 0.0f;
	m_worldMapOpen				= false;
	m_worldMapZoom				= 1.0f;
	m_worldMapCenterX = m_worldMapCenterZ = 0.0f;
	m_worldMapDragging			= false;
	m_wmDragX = m_wmDragY = 0.0f;
	m_wmLeft = m_wmTop = m_wmSize = 0.0f;
	m_wmViewMinX = m_wmViewMaxZ = 0.0f;
	m_wmScale = m_wmFull = m_wmMidX = m_wmMidZ = 0.0f;
	m_wmCloseL = m_wmCloseT = m_wmCloseR = m_wmCloseB = 0.0f;
	m_wmListL = m_wmListR = 0.0f;
}

CMenuManagement::~CMenuManagement(void)
{
	ReleaseHudFonts();
	ReleaseZoneMap();
}


//------------------------------------------------------------------------------------//
int CMenuManagement::ClickActionHandler(CCLickZoneObj* clickObj,
										CMenu3DObj* menu,
										BOOL overrideDelay)
{
	long currentTime = GetTickCount();
	if(!overrideDelay)
		if((currentTime - m_clickHandlerStamp) < 500)
			return TRUE;
          
	if(clickObj->m_action == 1)
	{//close menu
		menu->CloseMenu(NULL);
	}//end close menu         
	if(clickObj->m_action == 2)
	{//editable text
		int trace = 0;
		for(POSITION visItemPos = menu->m_visualItems->GetHeadPosition();visItemPos != NULL;)
		{
			CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visItemPos);
			if(trace == clickObj->m_visualIndex){
				//visPtr->m_editable = TRUE;
				visPtr->m_currentlyBiengEdited = TRUE;
			}
			else if(visPtr->m_editable){
				//visPtr->m_editable = FALSE;
				visPtr->m_currentlyBiengEdited = FALSE;
			}
			trace++;
		}
		return TRUE;
	}//end editable text
	if(clickObj->m_action == 3)
	{//connect to tcpip useing global settings
		m_gStatus->m_multiplayerObject->EnableClient(m_gStatus->m_multiplayerObject->m_IPaddress,0);
		menu->CloseMenu(&m_gStatus->m_globalCursorPos);
	}//connect to tcpip useing global settings
	if(clickObj->m_action == 4)
	{//choose character and play
		CMenuVisObj * vObjTemp = NULL;
		int currentlySelectedItem = GetCurSelFromMenuList(50,&vObjTemp);
		if(vObjTemp)
		{
			CSliderContentObj * slPtr = vObjTemp->m_sliderSystem->GetByIndex(currentlySelectedItem);
			if(slPtr)
			{
				if(slPtr->m_text.CompareNoCase("No characters Created Yet")==0)
				{
					if(m_gStatus->m_systemReadout)
						m_gStatus->m_systemReadout->AddLine("You must first select or Create a charcater.");
				}
				else
				{
					m_clientNet->PlayCharacterOnAccount(currentlySelectedItem);
					menu->CloseMenu(&m_gStatus->m_globalCursorPos);
				}
			}
			else if(m_gStatus->m_systemReadout)
				m_gStatus->m_systemReadout->AddLine("You must first select or Create a charcater.");
		}
		else if(m_gStatus->m_systemReadout)
			m_gStatus->m_systemReadout->AddLine("You must first select or Create a charcater.");
	}//end choose character and play
	if(clickObj->m_action == 5)
	{//load world and spawn
		D3DVECTOR position;
		position.x = (float)clickObj->m_miscInt;
		position.y = (float)clickObj->m_miscInt2;
		position.z = (float)clickObj->m_miscInt3;
		m_gStatus->LoadScene(clickObj->m_miscString);
		m_clientNet->RespawnOnClient(0,position);
	}//end load world and spawn
	if(clickObj->m_action == 6)
	{//set resolution
		int indexOfVisual = clickObj->m_visualIndex;
		if(menu->m_visualItems)
		if(indexOfVisual > -1){//valid index
			POSITION visPos = menu->m_visualItems->FindIndex(indexOfVisual);
			if(visPos)
			{//valid pos
				CMenuVisObj * vObj = (CMenuVisObj *)menu->m_visualItems->GetAt(visPos);
				if(vObj->m_type == 7)
				{//slider model
					if(vObj->m_sliderSystem)
					{//v ptr
						if(m_gStatus->m_dxRenderer->m_vmodeWidth[vObj->m_sliderSystem->m_currentSelectedContentItem] == -2)
						{//force original window
						m_gStatus->CreateDisplayIM(FALSE,0,0,m_gStatus->m_dxRenderer->m_initialCreateRect.right,m_gStatus->m_dxRenderer->m_initialCreateRect.bottom,32);
						}//end force original window
						else
						m_gStatus->CreateDisplayIM(TRUE,0,0,m_gStatus->m_dxRenderer->m_vmodeWidth[vObj->m_sliderSystem->m_currentSelectedContentItem],m_gStatus->m_dxRenderer->m_vmodeHeight[vObj->m_sliderSystem->m_currentSelectedContentItem],m_gStatus->m_dxRenderer->m_vmodeDepth[vObj->m_sliderSystem->m_currentSelectedContentItem]);
					
					return 2;
					}//end v ptr
				}//end slider model
			}//end valid pos
		}//end valid index
	}//end set resolution
	if(clickObj->m_action == 7)
	{//mouse invert on/off
		if((GetTickCount()-m_clickStamp) > 1000)
		{
			clickObj->m_switchOverride = TRUE;
			POSITION cntPos = m_gStatus->m_controlsList->FindIndex(0);
			if(cntPos)
			{//1
				ControlDBObj* cObj = (ControlDBObj*)m_gStatus->m_controlsList->GetAt(cntPos);
				if(cObj->m_invertMouse)
				{
					cObj->m_invertMouse = FALSE;
					clickObj->m_switchOverrideState = 0;
				}
				else
				{
					cObj->m_invertMouse = TRUE;
					clickObj->m_switchOverrideState = 3;
				}
				m_clickStamp = GetTickCount();
			}//end 1
		}
	}//end mouse invert on/off
	if(clickObj->m_action == 8)
	{//quit game
		m_gStatus->m_closeDownCall = TRUE;
	}//end quit game
	if(clickObj->m_action == 9)
	{//open menu
		int varTraslation = clickObj->m_miscInt3;
		if(varTraslation < 0)
		{//selection from list command
			varTraslation = GetCurSelFromMenuList(abs(varTraslation),NULL);
		}//end selection from list command

		if(menu->m_cursor)
		{
			m_gStatus->m_globalCursorPos.x = menu->m_cursor->m_position.x;//- menu->m_offsetMenu.x;
			m_gStatus->m_globalCursorPos.y = menu->m_cursor->m_position.y;//- menu->m_offsetMenu.y;
		}
		m_gStatus->OpenMenu(clickObj->m_miscInt, false); //0,varTraslation,clickObj->m_miscInt2,FALSE);

		if(clickObj->m_miscString != "NA")
		{//Send Server Command
			POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
			if(entPosition)
			{//valid    
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				if(ptrLoc)
				if(ptrLoc->List_controlType == 0)
					/*
					m_gStatus->m_netGameLogicObject->SendTextMsg(   DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,//can be NULL if client
																	clickObj->m_miscString,
																	ptrLoc->List_networkDefinedID,
																	NULL,
																	0);
																	*/
					m_gStatus->m_netGameLogicObject->SendTextMsg(   DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,//can be NULL if client
																	ptrLoc->List_networkDefinedID,
																	NULL,
																	0,
																	clickObj->m_miscString);
			}//end valid
		}//Send Server Command
		
		menu->CloseMenu(&m_gStatus->m_globalCursorPos);
		m_clickHandlerStamp = currentTime;
	}//end open 
	if(clickObj->m_action == 10)
	{//delete char on account
		m_clientNet->DeleteCharacter(GetCurSelFromMenuList(50,NULL));
		m_clickHandlerStamp = currentTime;
	}//end delete char on account 
	if(clickObj->m_action == 11)
	{//change entity item by id
		CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visPtr)
		if(visPtr->m_entityObjectLink)
		{//found entity visual
			CMovementObj * dbPtr = m_gStatus->m_entObjList->GetByID(visPtr->m_entityObjectLink->List_refModel);
			if(!dbPtr)
				return FALSE;
		       
				visPtr->m_entityObjectLink->ArmNextLocationId(clickObj->m_miscInt2,dbPtr,m_gStatus->m_itemManager,m_gStatus->m_3dDevice);
		}//end found entity visual
		visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt+1);
		if(visPtr)
		if(visPtr->m_entityObjectLink)
		{//found entity visual
			CMovementObj * dbPtr = m_gStatus->m_entObjList->GetByID(visPtr->m_entityObjectLink->List_refModel);
			if(!dbPtr)
				return FALSE;
		       
			visPtr->m_entityObjectLink->ArmNextLocationId(clickObj->m_miscInt2,dbPtr,m_gStatus->m_itemManager,m_gStatus->m_3dDevice);
		}//end found entity visual
		m_clickHandlerStamp = currentTime;
	}//end change entity item by id
	if(clickObj->m_action == 12)
	{//create character from ids
		int spawnIndex = atoi(clickObj->m_miscString);
		CreateCharacterThroughMenus(clickObj->m_miscInt,clickObj->m_miscInt2,spawnIndex);
		m_clickHandlerStamp = currentTime;
		m_gStatus->OpenMenu(clickObj->m_miscInt3, false);
	}//end create character from ids
	if(clickObj->m_action == 13)
	{//change Next deformable cfg
		CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visPtr)
		if(visPtr->m_entityObjectLink)
		{//found entity visual
			BOOL result = IndexUpDim(	visPtr->m_entityObjectLink->List_skeletalObject,
										clickObj->m_miscInt2,
										m_gStatus->m_entObjList,
										visPtr->m_entityObjectLink->List_refModel);
			m_clickHandlerStamp = currentTime;
		}//end entity Next found visual
		visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt+1);
		if(visPtr)
		if(visPtr->m_entityObjectLink)
		{//found entity visual
			BOOL result = IndexUpDim(	visPtr->m_entityObjectLink->List_skeletalObject,
										clickObj->m_miscInt2,
										m_gStatus->m_entObjList,
										visPtr->m_entityObjectLink->List_refModel);
			m_clickHandlerStamp = currentTime;
		}//end entity Next found visual
	}//end change deformable cfg
	if(clickObj->m_action == 14)
	{//change Prev deformable cfg
		CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visPtr)
		if(visPtr->m_entityObjectLink)
		{//found entity visual
			BOOL result = IndexDownDim(
			visPtr->m_entityObjectLink->List_skeletalObject,
			clickObj->m_miscInt2,
			m_gStatus->m_entObjList,
			visPtr->m_entityObjectLink->List_refModel);
			m_clickHandlerStamp = currentTime;
		}//end entity found visual
		visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt+1);
		if(visPtr)
		if(visPtr->m_entityObjectLink)
		{//found entity visual
			BOOL result = IndexDownDim(
			visPtr->m_entityObjectLink->List_skeletalObject,
			clickObj->m_miscInt2,
			m_gStatus->m_entObjList,
			visPtr->m_entityObjectLink->List_refModel);
			m_clickHandlerStamp = currentTime;
		}//end entity found visual
	}//end change Prev deformable cfg
	
	/*
	**
	** Removing Action 15 (Arm Item) to replace it with a single Activate Action 70
	** Please make sure you use 70 from now on. This applies to Use Item as well.
	**
	if(clickObj->m_action == 15){//arm item in inventory list
		int dupPassed = 0;
		int glID = GetMiscIntFromMenuList(clickObj->m_miscInt,dupPassed);

		if(glID != -1)
		{//somthing is selected
			POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(entPosition){//valid    
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				if(ptrLoc->List_controlType == 0)
				{//verify primary
					 m_gStatus->ArmEntityItemSys(ptrLoc,glID,dupPassed);
				}//end verify primary
			}//end valid
		}//end somthing is selected
		m_clickHandlerStamp = currentTime;
	}//end arm item in inventory list
	*/
	if(clickObj->m_action == 16){//disarm item in inventory list
		/*
		int dupPassed = 0;
		int glID = GetMiscIntFromMenuList(clickObj->m_miscInt,dupPassed);

		if(glID != -1)
		{//somthing is selected
			POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(entPosition)
			{//valid    
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				
				if(ptrLoc->List_controlType == 0)//verify primary
				{
					m_gStatus->DisarmEntityItemSys(ptrLoc,glID,dupPassed);
				}
			}//end valid
		}//end somthing is selected
		*/
		m_clickHandlerStamp = currentTime;
	}//end disarm item in inventory list

	if(clickObj->m_action == 17){//Buy Item From Store
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(entPosition)
			{//valid entity   
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			      
				int quanity = 1;
				CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
				if(visPtr)
				if(visPtr->m_textObject)
				{
					int quanityCnv = atoi(visPtr->m_textObject);
					if(quanityCnv > 0 && quanityCnv < 30000)
						quanity = quanityCnv;
					else if(m_gStatus->m_systemReadout)
						m_gStatus->m_systemReadout->AddLine("Invalid quantity-defaulting to 1 ");
				}

				int glID = GetMiscIntFromMenuList(clickObj->m_miscInt);
				CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
				if(invObj)
				{//valid
					if(m_gStatus->m_multiplayerObject->m_cashCount >= (invObj->m_marketCost*quanity))
					{//can afford this so visually add to player list
						if(!invObj->m_stackable)
						{
							for(int count=0;count<quanity;count++)
								m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,invObj->m_itemName,invObj->m_visualRef,glID,1,invObj->m_itemName,1,"");
						}
						else if(!m_gStatus->AddSelectedFromMenuList(clickObj->m_miscInt2,glID,quanity))
						{
							if(invObj->m_stackable)
							{
								 CString quanityTxt = m_gStatus->m_cabLib.ConvToString(quanity);
								 CString outPut = operator+(invObj->m_itemName,operator+(" X",quanityTxt));
								 m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,outPut,invObj->m_visualRef,glID,quanity,invObj->m_itemName,1,"");
							}
							else
							{
								 for(int count=0;count<quanity;count++)
									 m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,invObj->m_itemName,invObj->m_visualRef,glID,1,invObj->m_itemName,1,"");
							}
						}
						//if(!m_gStatus->AddSelectedFromMenuList(clickObj->m_miscInt2,glID))//bump counter or add 
						//   m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,invObj->m_itemName,-1,glID,1);
					}//end can afford this so visually add to player list
				

					m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
																   DPID_SERVERPLAYER,
																   21,
																   glID,
																   ptrLoc->List_networkDefinedID,
																   0,
																   quanity,
																   NULL);//request purchase of a store item
					m_clickHandlerStamp = currentTime;
			}//end valid
		}//end valid entity
	}//End Buy item from store
	if(clickObj->m_action == 18)
	{//Sell Item To Store
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid   
			int quanity = 1;
			CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
			if(visPtr)
			if(visPtr->m_textObject)
			{
				int quanityCnv = atoi(visPtr->m_textObject);
				if(quanityCnv > 0 && quanityCnv < 30000)
				{
					quanity = quanityCnv;
				}
				else
				m_gStatus->m_systemReadout->AddLine("Invalid quanity-defaulting to 1 ");
			}


			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			int glID = GetMiscIntFromMenuList(clickObj->m_miscInt);
			//RemoveSelectedFromMenuList(clickObj->m_miscInt);
			MinusSelectedFromMenuList(clickObj->m_miscInt,glID,quanity);
			m_gStatus->m_netGameLogicObject->SendAttribMsg( DPNSEND_GUARANTEED,
															DPID_SERVERPLAYER,
															22,
															glID,
															ptrLoc->List_networkDefinedID,
															0,
															quanity,
															NULL);//request sale of a item to store
			m_clickHandlerStamp = currentTime;
		}//End valid
	}//End Sell Item To Store
	if(clickObj->m_action == 19)
	{//loot item
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		{
			POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(entPosition)
			{//valid    
				int quanity = 1;
				CMenuVisObj * qPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
				if(qPtr)
				if(qPtr->m_textObject)
				{
					int quanityCnv = atoi(qPtr->m_textObject);
					if(quanityCnv > 0 && quanityCnv < 30000)
						quanity = quanityCnv;
					else if(m_gStatus->m_systemReadout)
						m_gStatus->m_systemReadout->AddLine("Invalid quanity-defaulting to 1 ");
				}
				
				
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				int glID = GetMiscIntFromMenuList(clickObj->m_miscInt);
				if(glID == -50)
					RemoveSelectedFromMenuList(clickObj->m_miscInt);
				else
					MinusSelectedFromMenuList(clickObj->m_miscInt,glID,quanity);
				m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																23,
																glID,
																ptrLoc->List_networkDefinedID,
																0,
																quanity,
																NULL);//request aquisition of item into inventory
				m_clickHandlerStamp = currentTime;
				//close menu if list is empty
				CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
				if(visPtr)
				if(visPtr->m_sliderSystem)
				if(visPtr->m_sliderSystem->m_contentList)
				if(visPtr->m_sliderSystem->m_contentList->GetCount()==0)
					 menu->CloseMenu(&m_gStatus->m_globalCursorPos);
			}//end valid		
		}
	}//End loot item
	if(clickObj->m_action == 20)
	{//Bank Item
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid		
			int quanity = 1;
			CMenuVisObj * qPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
			if(qPtr)
			if(qPtr->m_textObject)
			{//valid
				int quanityCnv = atoi(qPtr->m_textObject);
				if(quanityCnv > 0 && quanityCnv < 30000)
					quanity = quanityCnv;
				else if(m_gStatus->m_systemReadout)
					m_gStatus->m_systemReadout->AddLine("Invalid quanity-defaulting to 1 ");
			}//end valid

			CMovementObj *ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			int dupTrace = 0;
			int glID = GetMiscIntFromMenuList(clickObj->m_miscInt,dupTrace);
			//RemoveSelectedFromMenuList(clickObj->m_miscInt);//remove from player inventory
			
			CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
			CString existingText;
			quanity = MinusSelectedFromMenuList(clickObj->m_miscInt,glID,quanity,existingText);
			//AfxMessageBox(existingText);
			if(invObj)
			{
				if(!invObj->m_stackable)
					m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,existingText,invObj->m_visualRef,glID,1,existingText,1,"");
				else if(!m_gStatus->AddSelectedFromMenuList(clickObj->m_miscInt2,glID,quanity))
				{
					if(invObj->m_stackable)
					{	
						 CString quanityTxt = m_gStatus->m_cabLib.ConvToString(quanity);
						 CString outPut = operator+(existingText,operator+(" X",quanityTxt));
						 m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,outPut,invObj->m_visualRef,glID,quanity,existingText,1,"");
					}
					else
						 m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,existingText,invObj->m_visualRef,glID,1,existingText,1,"");
				}
				//m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,invObj->m_itemName,-1,glID);
			      
				m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																24,
																glID,
																ptrLoc->List_networkDefinedID,
																dupTrace,
																quanity,
																NULL);//request aquisition of item into inventory
				m_clickHandlerStamp = currentTime;
			}
		}//end valid
	}//End Bank item
	if(clickObj->m_action == 21)
	{//Take Item From Bank
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid   	
			int quanity = 1;
			CMenuVisObj * qPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
			if(qPtr)
			if(qPtr->m_textObject)
			{//valid
				int quanityCnv = atoi(qPtr->m_textObject);
				if(quanityCnv > 0 && quanityCnv < 30000)
					quanity = quanityCnv;
				else if(m_gStatus->m_systemReadout)
					m_gStatus->m_systemReadout->AddLine("Invalid quanity-defaulting to 1 ");
			}//end valid

			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			int dupPassed = 0;
			int glID = GetMiscIntFromMenuList(clickObj->m_miscInt,dupPassed);
			//RemoveSelectedFromMenuList(clickObj->m_miscInt);
			CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
			CString lastItemName = "";
			quanity = MinusSelectedFromMenuList(clickObj->m_miscInt,glID,quanity,lastItemName);
			if(quanity < 1)
				 quanity = 1;
			if(invObj)
			{
				 if(!invObj->m_stackable)
				 {
					m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,lastItemName,invObj->m_visualRef,glID,1,lastItemName,1,"");
				 }
				 else if(!m_gStatus->AddSelectedFromMenuList(clickObj->m_miscInt2,glID,quanity))
				 {
						 //CString outPut = operator+(invObj->m_itemName," X1");
						 //m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,outPut,-1,glID,1);
						 CString quanityTxt = m_gStatus->m_cabLib.ConvToString(quanity);
						 CString outPut = operator+(invObj->m_itemName,operator+(" X",quanityTxt));
						 m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,outPut,invObj->m_visualRef,glID,quanity,lastItemName,1,"");
				 }

				 //m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,invObj->m_itemName,-1,glID);
				 m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
								DPID_SERVERPLAYER,
								25,
								glID,
								ptrLoc->List_networkDefinedID,
								dupPassed,
								quanity,
								NULL);//request aquisition of item into inventory
				 m_clickHandlerStamp = currentTime;
			}
		}//end valid
	}//End Take Item From Bank
	
	/*
	**
	** Removing Action 22 (Use Item) to replace it with a single Activate Action 70
	** Please make sure you use 70 from now on. This applies to Arm Item as well.
	**
	if(clickObj->m_action == 22)
	{//Use Item
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid    
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			int dupPassed = 0;
			int glID = GetMiscIntFromMenuList(clickObj->m_miscInt,dupPassed);
			CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
			if(invObj)
			{//valid
				if(invObj->m_useType != 0)
				{//dont use important shit
					//RemoveSelectedFromMenuList(clickObj->m_miscInt);
					
					BOOL useItemNow = TRUE;
					if(invObj->m_useType == 6)
					{//house item verify placement validity locally before even using this item
						D3DVECTOR positionWld;
						ptrLoc->List_baseFrame->GetPosition(NULL,&positionWld);
						CHousingObj * houseBasePtr = m_gStatus->m_housingDB->GetByIndex(invObj->m_miscUseValue);
						if(houseBasePtr)
						{//valid database base
							if(!HousePlacementValidCheck(positionWld,houseBasePtr->m_clipBox))
							{
								useItemNow = FALSE;
								m_gStatus->m_systemReadout->AddLine("Ground would not support your house well here...");
							}
						}//end valid database base
						else
						{//cannot find house DB ref
							 useItemNow = FALSE;
							 m_gStatus->m_systemReadout->AddLine("Invalid database error 23464...");
						}//end cannot find house DB ref
						m_clickHandlerStamp = currentTime;
					}//end house item verify placement validity locally before even using this item
					if(useItemNow)
					{
						MinusSelectedFromMenuList(clickObj->m_miscInt,glID,1);
						if(invObj->m_reloadTime == 0)
						{
							m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
								DPID_SERVERPLAYER,
								27,//use item
								glID,
								ptrLoc->List_networkDefinedID,
								dupPassed,
								0,
								NULL);
						}
						else
							 m_gStatus->m_pendingObject->AddCommand(1,glID,ptrLoc->List_networkDefinedID,invObj->m_reloadTime,"");
					}
					m_clickHandlerStamp = currentTime;
				}//end dont use important shit
			}//end valid
		}//end valid
	}//End Use Item
	*/
	if(clickObj->m_action == 23)
	{//make controls Default
		m_gStatus->SaveControlConfig("KeyConfig.cnt");
		m_clickHandlerStamp = currentTime;
	}//End make controls Default
	if(clickObj->m_action == 24)
	{//index slider sys up
		CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visPtr)
		if(visPtr->m_sliderSystem)
		{//valid slider system
			visPtr->m_sliderSystem->IndexUp((int)m_gStatus->m_timeAdjust);
			//m_clickHandlerStamp = currentTime;
		}//end valid slider system
	}//End index slider sys up
	if(clickObj->m_action == 25)
	{//index slider sys down
		CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visPtr)
		if(visPtr->m_sliderSystem)
		{//valid slider system
			visPtr->m_sliderSystem->IndexDown((int)m_gStatus->m_timeAdjust);
			//m_clickHandlerStamp = currentTime;
		}//end valid slider system
	}//End index slider sys down
	//--------------------------------------------
	if(clickObj->m_action == 26)
	{//send trade offer
		if(!clickObj->m_previouslyActuated)
		{//actuate only once
			POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(entPosition)
			{//valid    
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
				if(visPtr)
				if(visPtr->m_sliderSystem)
				{//valid slider system
					//send offer to server
					int cashAmount = 0;
					int cashItemCount = 0;
					CMenuVisObj * cashPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt2);
					if(cashPtr)
					{//send cash
						cashAmount = atoi(cashPtr->m_textObject);
					}//end send cash

					int listCount = 0;
					if(visPtr->m_sliderSystem->m_contentList)
						listCount = (int)visPtr->m_sliderSystem->m_contentList->GetCount();

					if(listCount > 50)
					{
						if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine("Too many items in trade");
					}
					else if(listCount < 1 && cashAmount == 0)
					{
						if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine("Nothing to submit");
					}
					else
					{//do it
						int itemCount = listCount/2;
						int headerSize = sizeof(TRADE_HEADER);
						int byteSize = (sizeof(ITEMDATA_STRUCT)*itemCount) + sizeof(TRADE_HEADER);
						
						BYTE * dataBlock = new BYTE[byteSize];
						
						TRADE_HEADER header;
						header.m_catagorie = 2;
						header.m_cashAmount = cashAmount;

						CopyMemory(dataBlock,&header,sizeof(TRADE_HEADER));
					
						if(itemCount > 0)
						{
							ITEMDATA_STRUCT * itemBlock = new ITEMDATA_STRUCT[itemCount];
							int tracerFill = 0;
							BOOL switchForMode1 = TRUE;
							if(visPtr->m_sliderSystem->m_contentList)
							for(POSITION posLoc = visPtr->m_sliderSystem->m_contentList->GetHeadPosition();posLoc != NULL;)
							{
								CSliderContentObj * cObj = (CSliderContentObj *)visPtr->m_sliderSystem->m_contentList->GetNext(posLoc);
								if(!switchForMode1)
								{
									switchForMode1 = TRUE;
									continue;
								}
							
								itemBlock[tracerFill].m_GLID    = cObj->m_miscInt;
								itemBlock[tracerFill].m_quantity = cObj->m_miscInt2;
								itemBlock[tracerFill].m_dMod    = cObj->m_miscInt3;//override for duptrace
								tracerFill++;
								switchForMode1 = FALSE;
							}

							//fill data
							BYTE * itemCast = (BYTE*)itemBlock;
							int transferSz = (sizeof(ITEMDATA_STRUCT)*itemCount);
							for(int i=0;i<transferSz;i++)
								dataBlock[i+headerSize] = itemCast[i];
							
							if(itemBlock)
								delete [] itemBlock;
							
							itemBlock = 0;
						}

						m_gStatus->m_multiplayerObject->m_miscOutBox->AddPacket(byteSize,(BYTE*)dataBlock,0,0);
						delete [] dataBlock;
						dataBlock = 0;
					}//end do it
				}//end valid slider system
				m_clickHandlerStamp = currentTime;
			}//end valid
			clickObj->m_previouslyActuated = TRUE;
		}//end clickObj->m_previouslyActuated 
	}//End send trade offer
	//--------------------------------------------
	if(clickObj->m_action == 27){//transfer from list to list
		CMenuVisObj * srcList =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		CMenuVisObj * destList = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt2);
		int glID = GetMiscIntFromMenuList(clickObj->m_miscInt);
		if(srcList != NULL && destList != NULL)
		if(srcList->m_sliderSystem != NULL && destList->m_sliderSystem != NULL)
		if(srcList->m_sliderSystem->m_currentSelectedContentItem > -1)
		{//valid
			CSliderContentObj * sliderObj = srcList->m_sliderSystem->GetByIndex(srcList->m_sliderSystem->m_currentSelectedContentItem);
			if(sliderObj)
			{//v2
				 CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
				 if(invObj)
				 {
					destList->m_sliderSystem->AddString(invObj->m_itemName,sliderObj->m_visualRef,sliderObj->m_miscInt,1,0,0);
					/*if(!invObj->m_stackable)
					else if(!m_gStatus->AddSelectedFromMenuList(clickObj->m_miscInt2,sliderObj->m_miscInt))
							destList->m_sliderSystem->AddString(sliderObj->m_text,sliderObj->m_visualRef,sliderObj->m_miscInt,1);*/

					MinusSelectedFromMenuList(clickObj->m_miscInt,glID,1);
				}
				//destList->m_sliderSystem->AddString(sliderObj->m_text,sliderObj->m_visualRef,sliderObj->m_miscInt);
				//srcList->m_sliderSystem->DeleteString(srcList->m_sliderSystem->m_currentSelectedContentItem);
			}//end v2
		}//end valid
		m_clickHandlerStamp = currentTime;
	}//transfer from list to list
	//--------------------------------------------
	if(clickObj->m_action == 28)
	{//Accept Trade Offer
		if(!clickObj->m_previouslyActuated)
		{//actuate only once
			POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(entPosition)
			{//valid    
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
							DPID_SERVERPLAYER,
							30,
							0,
							ptrLoc->List_networkDefinedID,
							0,
							0,
							NULL);

			}//end valid
		}
		clickObj->m_previouslyActuated = TRUE;
	}//end accept trade offer
	//--------------------------------------------
	if(clickObj->m_action == 29){//Cancel Trade
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid    
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
															DPID_SERVERPLAYER,
															29,
															0,
															ptrLoc->List_networkDefinedID,
															0,
															0,
															NULL);

		}//end valid
	}//end Cancel Trade
	//--------------------------------------------
	if(clickObj->m_action == 30)
	{//withdrawal
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid  
			CMenuVisObj * refText =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
			if(refText)
			{
				int amount = abs(atoi(refText->m_textObject));
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				m_clientNet->SendCommandMsg(0, amount, ptrLoc->List_networkDefinedID);
			}
			m_clickHandlerStamp = currentTime;
		}//end valid
	}//end withdrawal
	//--------------------------------------------
	if(clickObj->m_action == 31)
	{//Deposit
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid    
			CMenuVisObj * refText =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
			if(refText)
			{
				int amount = abs(atoi(refText->m_textObject));
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				m_clientNet->SendCommandMsg(1, amount, ptrLoc->List_networkDefinedID);
			}
			m_clickHandlerStamp = currentTime;
		}//end valid
	}//end Deposit
	//--------------------------------------------
	if(clickObj->m_action == 32 && m_gStatus->m_multiplayerObject->GetEnabledClient_safe() == TRUE)
	{//Add Control Event
		CString actionString;//,keySelectString;
		int inventoryGlid = -1;
		int miscInt2      =  0;
		int miscInt3      =  0;
		int miscInt4      =  0;
		
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visual)
		if(visual->m_sliderSystem)
		{
			CSliderContentObj * slPtr = (CSliderContentObj *)visual->m_sliderSystem->GetByIndex(visual->m_sliderSystem->m_currentSelectedContentItem);
			if(slPtr)
			{
				if((slPtr->m_text.Left(8) == "Use Item" || slPtr->m_text.Left(10) == "Equip Item") && clickObj->m_miscInt3 == 0)
				{//Get an inventory item to use/equip
					/*CMenu3DObj * menuObj = m_gStatus->m_menuDatabase->GetByID(1125);
					if(menuObj)
					{//Found menu
						CMenu3DObj * runtimeTwo =  NULL;
						menuObj->Clone(&runtimeTwo,m_gStatus->m_3dDevice);
						m_gStatus->ReinitMenuSys(runtimeTwo,0);
						
						//Merge Menu and Load Textures
						menu->MergeMenu(runtimeTwo,m_gStatus->m_3dDevice,1);
						m_gStatus->LoadTexturesForMenu(menu);
						
						//Delete object
						runtimeTwo->SafeDelete();
						delete runtimeTwo;
						runtimeTwo = 0;

						//Request Inventory From Server
						POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
						if(entPosition)
						{//valid
							CMovementObj * ptrLoc = ( CMovementObj* )m_gStatus->m_entObjRuntime->GetAt(entPosition);

							m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																19,
																51,
																ptrLoc->List_networkDefinedID,
																0,
																0,
																NULL);
						}//end valid
						
						//menu->m_lockedForChildMenuResponse = TRUE;
					}//End found menu
					*/m_durationToServerStamp = currentTime;

					return TRUE;
				}//End get an inventory item to use/equip

				actionString = slPtr->m_text;
				inventoryGlid = slPtr->m_miscInt;
				miscInt2 = slPtr->m_miscInt2;
				miscInt3 = slPtr->m_miscInt3;
				miscInt4 = slPtr->m_miscInt4;
			}
		}
		/*visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt2);
		if(visual)
		if(visual->m_sliderSystem)
		{
			CSliderContentObj * slPtr = (CSliderContentObj *)visual->m_sliderSystem->GetByIndex(visual->m_sliderSystem->m_currentSelectedContentItem);
			if(slPtr)
				keySelectString = slPtr->m_text;
		}*/
		visual = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
		if(visual)
		if(visual->m_sliderSystem)
		{
			CSliderContentObj * slPtr = (CSliderContentObj *)visual->m_sliderSystem->GetByIndex(visual->m_sliderSystem->m_currentSelectedContentItem);
			if(slPtr)
			if(miscInt2 == 0)
				inventoryGlid = slPtr->m_miscInt;
		}
		POSITION cntPos = m_gStatus->m_controlsList->FindIndex(0);
		if(cntPos)
		{//1
			ControlDBObj* cObj = (ControlDBObj*)m_gStatus->m_controlsList->GetAt(cntPos);
			cObj->List_controlInfoList->AddControlEvent("Num 1",//keySelectString,
														actionString,
														inventoryGlid,
														miscInt2,
														miscInt3,
														miscInt4,
														FALSE,
														FALSE);
		}
		//reinlist control config
		visual =  (CMenuVisObj *) GetVisualByID(5);
		if(visual)
		if(visual->m_sliderSystem)
		{//valid
			//visual->m_sliderSystem->FlushAll();
			POSITION cntPos = m_gStatus->m_controlsList->FindIndex(0);
			if(cntPos)
			{//1
				ControlDBObj* cObj = (ControlDBObj*)m_gStatus->m_controlsList->GetAt(cntPos);
				for(POSITION cPos = cObj->List_controlInfoList->GetHeadPosition();cPos != NULL;)
				{
					ControlInfoObj * infoObj = (ControlInfoObj *)cObj->List_controlInfoList->GetNext(cPos);
					if(!cPos)
					{
						CString build;
						if(infoObj->List_miscFloat2 == 0)
							build = infoObj->GetActionDescription();
						else
						{
							 CSkillObject * skillPtrRef = m_gStatus->m_skillDatabase->GetSkillByType((int)infoObj->List_miscFloat3);
							 build = "Skill Bind";
							 if(skillPtrRef)
							 if(skillPtrRef->m_levelRangeDB)
							 {
								int tempindex = (int)(infoObj->List_miscFloat2-1.0f);
								POSITION lvlPos = skillPtrRef->m_levelRangeDB->FindIndex(tempindex);
								if(lvlPos)
								{
									CLevelRangeObject * lvlPtr = (CLevelRangeObject *)skillPtrRef->m_levelRangeDB->GetAt(lvlPos);
									if(lvlPtr->m_abilityDB)
									{
										CSkillAbilityObject * abilityPtr = lvlPtr->m_abilityDB->GetObjByIndex((int)infoObj->List_miscFloat1);
										if(abilityPtr)
										build = abilityPtr->m_abilityName;
									}
								}
							}
						}

						//CString assignmnt = cObj->GetButtonVis(infoObj->List_buttonDown,infoObj->List_controlType);
						//build = assignmnt + " :" + build;
						visual->m_sliderSystem->AddString(build, -1, 0, 0, 0, 0);
						visual->m_sliderSystem->AddString(cObj->GetButtonVis(infoObj->List_buttonDown,infoObj->List_controlType), -1, 0, 0, 0, 0);
						visual->m_sliderSystem->m_currentSelectedContentItem = 0;
					}
				}
			}//end 1
		}//end valid
		m_gStatus->SaveControlConfig("KeyConfig.cnt");
		m_clickHandlerStamp = currentTime;
	}//end Add Control Event
	//--------------------------------------------
	if(clickObj->m_action == 33)
	{//restore default controls
		RestoreFromDefaults();
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visual)
		if(visual->m_sliderSystem)
		{
			if(visual->m_type == 73)
			   m_gStatus->InListActions(0,visual->m_sliderSystem, TRUE);
			else
               m_gStatus->InListActions(0,visual->m_sliderSystem, FALSE);
		}
		m_clickHandlerStamp = currentTime;
	}//end restore default controls
	if(clickObj->m_action == 34)
	{//delete a control macro
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(5);
		if(visual)
		if(visual->m_sliderSystem)
		{//valid
			visual->m_sliderSystem->FlushAll();
			POSITION cntPos = m_gStatus->m_controlsList->FindIndex(0);
			if(cntPos)
			{//1
				ControlDBObj* cObj = (ControlDBObj*)m_gStatus->m_controlsList->GetAt(cntPos);
				cObj->List_controlInfoList->RemoveByIndex(visual->m_sliderSystem->m_currentSelectedContentItem / 2);
				for(POSITION cPos = cObj->List_controlInfoList->GetHeadPosition();cPos != NULL;)
				{
					ControlInfoObj * infoObj = (ControlInfoObj *)cObj->List_controlInfoList->GetNext(cPos);
					CString build;//= infoObj->GetActionDescription();

					if(infoObj->List_miscFloat2 == 0)
						build = infoObj->GetActionDescription();
					else
					{
						CSkillObject * skillPtrRef = m_gStatus->m_skillDatabase->GetSkillByType((int)infoObj->List_miscFloat3);
						build = "Skill Bind";
						if(skillPtrRef)
						if(skillPtrRef->m_levelRangeDB)
						{
							POSITION lvlPos = skillPtrRef->m_levelRangeDB->FindIndex((int)(infoObj->List_miscFloat2-1.0f));
							if(lvlPos)
							{
								CLevelRangeObject * lvlPtr = (CLevelRangeObject *)skillPtrRef->m_levelRangeDB->GetAt(lvlPos);
								if(lvlPtr->m_abilityDB)
								{
									CSkillAbilityObject * abilityPtr = lvlPtr->m_abilityDB->GetObjByIndex((int)infoObj->List_miscFloat1);
									if(abilityPtr)
										build = abilityPtr->m_abilityName;
								}
							}
						}
					}
					//CString assignmnt = cObj->GetButtonVis(infoObj->List_buttonDown,infoObj->List_controlType);
					//build = assignmnt+" :"+build;
					visual->m_sliderSystem->AddString(build, -1, 0, 0, 0, 0);
					visual->m_sliderSystem->AddString(cObj->GetButtonVis(infoObj->List_buttonDown,infoObj->List_controlType), -1, 0, 0, 0, 0);
				}
				visual->m_sliderSystem->m_currentSelectedContentItem -= 2;
			}//end 1
		}//end valid
		m_gStatus->SaveControlConfig("KeyConfig.cnt");
		m_clickHandlerStamp = currentTime;
	}//end delete a control macro
	if(clickObj->m_action == 35)
	{//Replace Control macro
		CString actionString,keySelectString;
		int inventoryGlid = -1;
		int miscInt2 = 0;
		int miscInt3 = 0;
		int miscInt4 = 0;
		
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visual)
		if(visual->m_sliderSystem)
		{
			CSliderContentObj * slPtr = (CSliderContentObj *)visual->m_sliderSystem->GetByIndex(visual->m_sliderSystem->m_currentSelectedContentItem);
			if(slPtr)
			{
				actionString = slPtr->m_text;
				inventoryGlid = slPtr->m_miscInt;
				miscInt2 = slPtr->m_miscInt2;
				miscInt3 = slPtr->m_miscInt3;
				miscInt4 = slPtr->m_miscInt4;
			}
		}
		visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt2);
		if(visual)
		if(visual->m_sliderSystem)
		{
			CSliderContentObj * slPtr = (CSliderContentObj *)visual->m_sliderSystem->GetByIndex(visual->m_sliderSystem->m_currentSelectedContentItem);
			if(slPtr)
			keySelectString = slPtr->m_text;
		}
		visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
		if(visual)
		if(visual->m_sliderSystem)
		{
			CSliderContentObj * slPtr = (CSliderContentObj *)visual->m_sliderSystem->GetByIndex(visual->m_sliderSystem->m_currentSelectedContentItem);
			if(slPtr)
			if(miscInt2 == 0)
				inventoryGlid = slPtr->m_miscInt;
		}
		
		//reinlist control config
		visual =  (CMenuVisObj *) GetVisualByID(5);
		if(visual)
		if(visual->m_sliderSystem)
		{//valid
			visual->m_sliderSystem->FlushAll();
			POSITION cntPos = m_gStatus->m_controlsList->FindIndex(0);
			if(cntPos)
			{//1
				ControlDBObj* cObj = (ControlDBObj*)m_gStatus->m_controlsList->GetAt(cntPos);
				cObj->List_controlInfoList->ReplaceControlEvent(visual->m_sliderSystem->m_currentSelectedContentItem / 2,
																keySelectString,
																actionString,
																inventoryGlid,
																miscInt2,
																miscInt3,
																miscInt4);

				for(POSITION cPos = cObj->List_controlInfoList->GetHeadPosition();cPos != NULL;)
				{
					ControlInfoObj * infoObj = (ControlInfoObj *)cObj->List_controlInfoList->GetNext(cPos);
					//CString build = infoObj->GetActionDescription();
					CString build;
					if(infoObj->List_miscFloat2 == 0)
						build = infoObj->GetActionDescription();
					else
					{
						CSkillObject * skillPtrRef = m_gStatus->m_skillDatabase->GetSkillByType((int)infoObj->List_miscFloat3);
						build = "Skill Bind";
						if(skillPtrRef)
						if(skillPtrRef->m_levelRangeDB)
						{
							POSITION lvlPos = skillPtrRef->m_levelRangeDB->FindIndex((int)(infoObj->List_miscFloat2-1.0f));
							if(lvlPos)
							{
								CLevelRangeObject * lvlPtr = (CLevelRangeObject *)skillPtrRef->m_levelRangeDB->GetAt(lvlPos);
								if(lvlPtr->m_abilityDB)
								{
									CSkillAbilityObject * abilityPtr = lvlPtr->m_abilityDB->GetObjByIndex((int)infoObj->List_miscFloat1);
									if(abilityPtr)
										build = abilityPtr->m_abilityName;
								}
							}
						}
					}
					//CString assignmnt = cObj->GetButtonVis(infoObj->List_buttonDown,infoObj->List_controlType);
					//build = assignmnt+" :"+build;
					visual->m_sliderSystem->AddString(build, -1, 0, 0, 0, 0);
					visual->m_sliderSystem->AddString(cObj->GetButtonVis(infoObj->List_buttonDown,infoObj->List_controlType), -1, 0, 0, 0, 0);
				}
			}//end 1
		}//end valid
		m_clickHandlerStamp = currentTime;
	}//end Replace Control Event
	if(clickObj->m_action == 36)
	{//send server command
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid    
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc)
			if(ptrLoc->List_controlType == 0)
			{//user define
				CMenuVisObj * visualX =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
				CString additionalText = "";
				if(visualX)
					if(visualX->m_textObject)
						additionalText = " " + visualX->m_textObject;

				/*
				m_gStatus->m_netGameLogicObject->SendTextMsg(   DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,//can be NULL if client
																clickObj->m_miscString + additionalText,
																ptrLoc->List_networkDefinedID,
																NULL,
																0);
																*/
				m_gStatus->m_netGameLogicObject->SendTextMsg(   DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,//can be NULL if client
																ptrLoc->List_networkDefinedID,
																NULL,
																0,
																clickObj->m_miscString + additionalText);
			
			}
			for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
			{//close all menus
				CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
				if(menu->m_closeOnEsc)
				menu->CloseMenu(&m_gStatus->m_globalCursorPos);
			}//end close all menus
			m_clickHandlerStamp = currentTime;
		}
	}//end send server command
	if(clickObj->m_action == 37)
	{//Accept Potential Group Joiner
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid    
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)//user define
				m_clientNet->SendCommandMsg(2,
											m_gStatus->m_multiplayerObject->m_potentialGroupJoiner,
											ptrLoc->List_networkDefinedID);//ask to join our group
		}
		menu->CloseMenu(&m_gStatus->m_globalCursorPos);
		m_clickHandlerStamp = currentTime;
	}//end Accept Potential Group Joiner
	if(clickObj->m_action == 38)
	{//Decline Potential Group Joiner
		m_gStatus->m_multiplayerObject->m_potentialGroupJoiner = -1;
		m_clickHandlerStamp = currentTime;
		menu->CloseMenu(&m_gStatus->m_globalCursorPos);
	}//end send server command
	if(clickObj->m_action == 39)
	{//Delete player from Group
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
			{//user define 		 
				CMenuVisObj * srcList =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
				if(srcList != NULL)
				if(srcList->m_sliderSystem != NULL)
				if(srcList->m_sliderSystem->m_currentSelectedContentItem > -1)
				{//valid
					CSliderContentObj * sliderObj = srcList->m_sliderSystem->GetByIndex(srcList->m_sliderSystem->m_currentSelectedContentItem);
					if(sliderObj)
					{//v2
						m_clientNet->SendCommandMsg(5,
													sliderObj->m_miscInt,//handle
													ptrLoc->List_networkDefinedID);//
						srcList->m_sliderSystem->DeleteString(srcList->m_sliderSystem->m_currentSelectedContentItem);
					}//end v2
				}//end valid
			}//end user define
		}//end valid Alpha
		m_clickHandlerStamp = currentTime;
	}//end delete player from Group
	if(clickObj->m_action == 40)
	{//Invite Player by local area select
		if((GetTickCount() - m_groupLastRequest) < 10000)
			return FALSE;
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
			{//user define 
				CMenuVisObj * srcList =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
				if(srcList != NULL)
				if(srcList->m_sliderSystem != NULL)
				if(srcList->m_sliderSystem->m_currentSelectedContentItem > -1)
				{//valid
					CSliderContentObj * sliderObj = srcList->m_sliderSystem->GetByIndex(srcList->m_sliderSystem->m_currentSelectedContentItem);
					if(sliderObj)
					{//v2
						m_clientNet->SendCommandMsg(3,
													sliderObj->m_miscInt,
													ptrLoc->List_networkDefinedID);//ask to join our group
						srcList->m_sliderSystem->DeleteString(srcList->m_sliderSystem->m_currentSelectedContentItem);
						srcList->m_sliderSystem->m_currentSelectedContentItem -= 1;
					}//end v2
				}//end valid
			}//end user define
		}//end valid Alpha
		m_clickHandlerStamp = currentTime;
	}
	if(clickObj->m_action == 41)
	{//request group list
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
				m_clientNet->SendCommandMsg(4, 0, ptrLoc->List_networkDefinedID);//request group list
		 }//end valid alpha
		 m_clickHandlerStamp = currentTime;
	}//end request group list
	if(clickObj->m_action == 42)
	{//send quest selection over
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
			{//user define 
				CMenuVisObj * srcList =  (CMenuVisObj *) GetVisualByID(91);
				if(srcList)
				if(srcList->m_sliderSystem)
				{//valid visual
					int itemNetAssignedID = GetMiscIntFromMenuList(91);
					if(itemNetAssignedID != -1)
					{//valid selection
						m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																		DPID_SERVERPLAYER,
																		32,
																		srcList->m_sliderSystem->m_currentSelectedContentItem,
																		ptrLoc->List_networkDefinedID,
																		itemNetAssignedID,
																		0,
																		NULL);
				
						for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
						{//close all menus
							CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
							if(menu->m_closeOnEsc)
							menu->CloseMenu(&m_gStatus->m_globalCursorPos);
						}//end close all menus
					}//end valid selection
				}//end valid visual
			}//end user define 
		 }//end valid alpha
		 m_clickHandlerStamp = currentTime;
	}//end send quest selection over
	if(clickObj->m_action == 43)
	{//use ability
		if((currentTime - m_durationToServerStamp) > m_durationToServer)
		{//1
			POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
			if(entPosition)
			{//valid Alpha  
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				if(ptrLoc->List_controlType == 0)
				{//user define    
					CMenuVisObj * srcList =  (CMenuVisObj *) GetVisualByID(menu->m_menuAccessID);
					if(srcList != NULL)
					if(srcList->m_sliderSystem != NULL)
					if(srcList->m_sliderSystem->m_currentSelectedContentItem > -1)
					{//valid
						CSliderContentObj * sliderObj = srcList->m_sliderSystem->GetByIndex(srcList->m_sliderSystem->m_currentSelectedContentItem);
						if(sliderObj)
						{//v2
							CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(sliderObj->m_miscInt);
							if(invObj)
							{//found global inventory item
								int selectedItem = srcList->m_sliderSystem->m_currentSelectedContentItem;
								if(srcList->m_sliderSystem->m_mode == 1)
									selectedItem = selectedItem/2;

								m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																				 DPID_SERVERPLAYER,
																				 33,
																				 selectedItem,
																				 ptrLoc->List_networkDefinedID,
																				 invObj->m_requiredSkillLevel,
																				 invObj->m_requiredSkillType,
																				 NULL);
							}//end found global inventory item
						}//end v2
					}//end valid
				}//end user define
			}//end valid alpha
			m_durationToServerStamp = currentTime;
		}//end 1
		else
		{
			if(m_gStatus->m_systemReadout)
				m_gStatus->m_systemReadout->AddLine("Cannot use another skill yet...");
		}

		m_clickHandlerStamp = currentTime;
	}//end use ability
	if(clickObj->m_action == 44)
	{//call for skills menu
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
				m_clientNet->SendCommandMsg(7,0,ptrLoc->List_networkDefinedID);
		}//end valid Alpha  
		m_clickHandlerStamp = currentTime;
	}//end call fir skills menu
	if(clickObj->m_action == 45)
	{//actuate skill by level and ability index 
	if((currentTime - m_durationToServerStamp) > m_durationToServer)
	{//1
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
			{
				if(m_gStatus->m_systemReadout)m_gStatus->m_systemReadout->AddLine("Ability Selection: ", clickObj->m_miscInt);
				if(m_gStatus->m_systemReadout)m_gStatus->m_systemReadout->AddLine("Required Skill Level: ", clickObj->m_miscInt2);
				if(m_gStatus->m_systemReadout)m_gStatus->m_systemReadout->AddLine("Skill Type: ", clickObj->m_miscInt3);
				m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																33,
																clickObj->m_miscInt,//ability selection
																ptrLoc->List_networkDefinedID,
																clickObj->m_miscInt2,//invObj->m_requiredSkillLevel,
																clickObj->m_miscInt3,
																NULL);//invObj->m_requiredSkillType);
			}
		}//end valid Alpha  
		m_durationToServerStamp = currentTime;
	}//end 1
	else if(m_gStatus->m_systemReadout)
		m_gStatus->m_systemReadout->AddLine("Cannot use another skill yet...");

		if(clickObj->m_miscString == "CLOSE")
			menu->CloseMenu(NULL);

		m_clickHandlerStamp = currentTime;
	}//end actuate skill by level and ability index 
	if(clickObj->m_action == 46)
	{//call for clan roster
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
				m_clientNet->SendCommandMsg(8,0,ptrLoc->List_networkDefinedID);
		}//end valid Alpha  
		m_clickHandlerStamp = currentTime;
	}//end call for clan roster
	if(clickObj->m_action == 47)
	{//execute passed in commands
		if(menu->m_passedInCommand == 1)
		{//delete char command
			m_clientNet->DeleteCharacter(menu->m_passedInVar);
			m_gStatus->OpenMenu(50, false); //TODO: Switch this to use a value from the "important menu" array
			menu->CloseMenu(&m_gStatus->m_globalCursorPos);
		}//end delete char command

		if(menu->m_passedInCommand == 2)
		{//delete char command
			POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
			if(entPosition)
			{//valid Alpha  
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				if(ptrLoc->List_controlType == 0)
					m_clientNet->SendAttribMsgCP(	31,
													menu->m_passedInVar,
													ptrLoc->List_networkDefinedID,//GLID of item to destroy
													0,
													0);
				menu->CloseMenu(&m_gStatus->m_globalCursorPos);
			}//end valid Alpha  
		}//end delete char command
		m_clickHandlerStamp = currentTime;
	}//end execute passed in commands
	if(clickObj->m_action == 48)
	{//destroy selected item
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid     
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			int dupCount = 0;
			int glID = GetMiscIntFromMenuList(ID_PLAYER_INVENTORY_MENU,dupCount);
			if(glID > -1)
			if(ptrLoc->List_controlType == 0)
			{
				//m_gStatus->OpenMenu(689,0,glID,2);//lets do this
				//menu->CloseMenu();
				RemoveSelectedFromMenuList(ID_PLAYER_INVENTORY_MENU);
				m_clientNet->SendAttribMsgCP(	31,
												glID,
												ptrLoc->List_networkDefinedID,//GLID of item to destroy
												dupCount,
												0);
			
			}
		}		
		m_clickHandlerStamp = currentTime;
		menu->m_clearTempMenus = TRUE;
		//menu->ClearTempMenus();
		return TRUE;
	}//end destroy selected item
	if(clickObj->m_action == 49)
	{//add item to trade
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid   
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			int quanity = 1;
			CMenuVisObj * qPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
			if(qPtr)
			if(qPtr->m_textObject)
			{
				 int quanityCnv = atoi(qPtr->m_textObject);
				 if(quanityCnv > 0 && quanityCnv < 30000)
					quanity = quanityCnv;
				 else if(m_gStatus->m_systemReadout)
					m_gStatus->m_systemReadout->AddLine("Invalid quanity-defaulting to 1 ");
			}

			int dupTrace = 0;
			int glID = GetMiscIntFromMenuList(clickObj->m_miscInt,dupTrace);
			CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
			CString lastStringName = "";
			quanity = MinusSelectedFromMenuList(clickObj->m_miscInt,glID,quanity,lastStringName);
			if(invObj)
			if(!invObj->m_stackable)
					m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,invObj->m_itemName,invObj->m_visualRef,glID,1,lastStringName,1,"",dupTrace,0);
			else if(!m_gStatus->AddSelectedFromMenuList(clickObj->m_miscInt2,glID,quanity))
			{
					if(invObj->m_stackable)
					{
						 CString quanityTxt = m_gStatus->m_cabLib.ConvToString(quanity);
						 CString outPut = operator+(invObj->m_itemName,operator+(" X",quanityTxt));
						 m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,outPut,invObj->m_visualRef,glID,quanity,lastStringName,1,"",dupTrace,0);
					}
					else
						 m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,invObj->m_itemName,invObj->m_visualRef,glID,1,lastStringName,1,"",dupTrace,0);
			}
		}//end valid
		m_clickHandlerStamp = currentTime;
	}//end add item to trade
	if(clickObj->m_action == 50)
	{//Add Item To House Store
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
			{//user type
				int quanity = 1;
				int setCost = 1;
				CMenuVisObj * qPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
				if(qPtr)
				if(qPtr->m_textObject)
				{//valid
					int quanityCnv = atoi(qPtr->m_textObject);
					if(quanityCnv > 0 && quanityCnv < 30000)
					{
						quanity = quanityCnv;
					}
					else if(m_gStatus->m_systemReadout)
						m_gStatus->m_systemReadout->AddLine("Invalid quanity-defaulting to 1 ");
				}//end valid
				CMenuVisObj * setCostVisObj = (CMenuVisObj *) GetVisualByID(atoi(clickObj->m_miscString));
				if(setCostVisObj)
				if(setCostVisObj->m_textObject)
				{//valid
					setCost = atoi(setCostVisObj->m_textObject);
					if(setCost < 1)
						setCost = 1;
					if(setCost > 9000000)
						setCost = 9000000;
				}//end valid
				int dupTrace = 0;
				int glID = GetMiscIntFromMenuList(clickObj->m_miscInt,dupTrace);
				CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
				CString lastItemName;
				quanity = MinusSelectedFromMenuList(clickObj->m_miscInt,glID,quanity,lastItemName);
				if(invObj)
				if(!invObj->m_stackable)
					m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,invObj->m_itemName,invObj->m_visualRef,glID,1,lastItemName,1," cost: " + m_gStatus->m_cabLib.ConvToString(setCost));
				else if(!m_gStatus->AddSelectedFromMenuList(clickObj->m_miscInt2,glID,quanity))
				{
					if(invObj->m_stackable)
					{
						 CString quanityTxt = m_gStatus->m_cabLib.ConvToString(quanity);
						 CString outPut =   invObj->m_itemName + " cost: " + m_gStatus->m_cabLib.ConvToString(setCost);
						 outPut = operator+(outPut,operator+(" X",quanityTxt));
						 m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,outPut,invObj->m_visualRef,glID,quanity,lastItemName,1," cost: " + m_gStatus->m_cabLib.ConvToString(setCost));
					}
					else
						 m_gStatus->AddStringToMenuList(clickObj->m_miscInt2,invObj->m_itemName,invObj->m_visualRef,glID,1,lastItemName,1,"");
				 }
				 m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																40,//attrib
																glID,//uniint Glid Choice of product
																dupTrace,
																setCost,//uniint2
																quanity,//unishort
																NULL);//invObj->m_requiredSkillType);
			}//end user type
		}//end valid Alpha  
		m_durationToServerStamp = currentTime;
	}//end Add Item To House Store
	if(clickObj->m_action == 51)
	{//Buy Item From House Store
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			int quanity = 1;
			CMenuVisObj * qPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
			if(qPtr)
			if(qPtr->m_textObject)
			{//valid
				int quanityCnv = atoi(qPtr->m_textObject);
				if(quanityCnv > 0 && quanityCnv < 30000)
					quanity = quanityCnv;
				else if(m_gStatus->m_systemReadout)
					m_gStatus->m_systemReadout->AddLine("Invalid quanity-defaulting to 1 ");
			}//end valid
			int dupTrace = 0;
			int glID = GetMiscIntFromMenuList(clickObj->m_miscInt2,dupTrace);
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
			CString lastSelectedItem;
			quanity = MinusSelectedFromMenuList(clickObj->m_miscInt2,glID,quanity,lastSelectedItem);
			if(invObj)
			if(!invObj->m_stackable)
					m_gStatus->AddStringToMenuList(clickObj->m_miscInt,invObj->m_itemName,invObj->m_visualRef,glID,1,lastSelectedItem,1,"");
			else if(!m_gStatus->AddSelectedFromMenuList(clickObj->m_miscInt,glID,quanity))
			{
				if(invObj->m_stackable){
					 CString quanityTxt = m_gStatus->m_cabLib.ConvToString(quanity);
					 CString outPut = operator+(invObj->m_itemName,operator+(" X",quanityTxt));
					 m_gStatus->AddStringToMenuList(clickObj->m_miscInt,outPut,invObj->m_visualRef,glID,quanity,lastSelectedItem,1,"");
				}
				else
					 m_gStatus->AddStringToMenuList(clickObj->m_miscInt,invObj->m_itemName,invObj->m_visualRef,glID,1,lastSelectedItem,1,"");
			}
					
			if(ptrLoc->List_controlType == 0)
			{//user type
				m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																 DPID_SERVERPLAYER,
																 39,//attrib
																 glID,//uniint Glid Choice of product
																 ptrLoc->List_networkDefinedID,
																 dupTrace,//uniint2
																 quanity,//unishort
																 NULL);//invObj->m_requiredSkillType);
						
			}//end user type
		}//end valid alpha
		m_durationToServerStamp = currentTime;
	}//end Buy Item From House Store
	if(clickObj->m_action == 52)
	{//Withdrawal Cash From Store
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
			{//user type
				CMenuVisObj * qPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
				if(qPtr)
					qPtr->m_textObject = "0";
			 

				m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																42,//attrib
																0,//uniint Glid Choice of product
																ptrLoc->List_networkDefinedID,
																0,//uniint2
																0,//unishort
																NULL);//invObj->m_requiredSkillType);
				m_durationToServerStamp = currentTime;
			}//end user type
		}//end valid alpha
	}//end Withdrawal Cash From Store
	if(clickObj->m_action == 53)
	{//Remove Item From Store
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
			{//user type
				int quanity = 1;
				int setCost = 1;
				CMenuVisObj * qPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
				if(qPtr)
				if(qPtr->m_textObject)
				{//valid
					int quanityCnv = atoi(qPtr->m_textObject);
					if(quanityCnv > 0 && quanityCnv < 30000)
						quanity = quanityCnv;
					else if(m_gStatus->m_systemReadout)
						m_gStatus->m_systemReadout->AddLine("Invalid quanity-defaulting to 1 ");
				}//end valid

				CMenuVisObj * setCostVisObj = (CMenuVisObj *) GetVisualByID(atoi(clickObj->m_miscString));
				if(setCostVisObj)
				if(setCostVisObj->m_textObject)
				{//valid
					setCost = atoi(setCostVisObj->m_textObject);
					if(setCost < 1)
						setCost = 1;
					if(setCost > 9000000)
						setCost = 9000000;
				}//end valid

				int dupeTrace = 0;
				int glID = GetMiscIntFromMenuList(clickObj->m_miscInt2,dupeTrace);
				CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
				CString lastSelectedText;
				quanity = MinusSelectedFromMenuList(clickObj->m_miscInt2,glID,quanity,lastSelectedText);
				if(invObj)
				if(!invObj->m_stackable)
					m_gStatus->AddStringToMenuList(clickObj->m_miscInt,invObj->m_itemName,invObj->m_visualRef,glID,1,lastSelectedText,1,"");
				else if(!m_gStatus->AddSelectedFromMenuList(clickObj->m_miscInt,glID,quanity))
				{
					if(invObj->m_stackable)
					{
						 CString quanityTxt = m_gStatus->m_cabLib.ConvToString(quanity);
						 CString outPut = operator+(invObj->m_itemName,operator+(" X",quanityTxt));
						 m_gStatus->AddStringToMenuList(clickObj->m_miscInt,outPut,invObj->m_visualRef,glID,quanity,lastSelectedText,1,"");
					}
					else
						 m_gStatus->AddStringToMenuList(clickObj->m_miscInt,invObj->m_itemName,invObj->m_visualRef,glID,1,lastSelectedText,1,"");
				 }
				 m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
																DPID_SERVERPLAYER,
																41,//attrib
																glID,//uniint Glid Choice of product
																dupeTrace,//dupeTrace
																setCost,//uniint2
																quanity,//unishort
																NULL);//invObj->m_requiredSkillType);
			}//end user type
		}//end valid Alpha  
		m_durationToServerStamp = currentTime;
	}//end Remove Item From Store
	if(clickObj->m_action == 54)
	{//pending control Assignment
		menu->m_captureControlQuerry  = TRUE;
		menu->m_controlActionListID   = clickObj->m_miscInt;
/*		menu->m_controlInvID          = clickObj->m_miscInt2;
		menu->m_controlStatusID       = clickObj->m_miscInt3;
		
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(menu->m_controlStatusID);
		if(visual)
			visual->m_textObject = "The next key you pressed will be bound"; 
*/
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(5);
		m_durationToServerStamp = currentTime;
	}//end pending control Assignment
	if(clickObj->m_action == 55)
	{//mereg menu as temp
		/*CMenu3DObj * menuObj = m_gStatus->m_menuDatabase->GetByID(clickObj->m_miscInt);
		if(menuObj)
		{
			CMenu3DObj * runtimeTwo =  NULL;
			menuObj->Clone(&runtimeTwo,m_gStatus->m_3dDevice);
			if(runtimeTwo->m_centerMenu)
			{
				runtimeTwo->m_offsetMenu.x += menu->m_cursor->m_position.x;
				runtimeTwo->m_offsetMenu.y += menu->m_cursor->m_position.y;
				runtimeTwo->ScaleOffset(runtimeTwo->m_offsetMenu,runtimeTwo->m_scale);
			}

			
			m_gStatus->ReinitMenuSys(runtimeTwo,0);
			
			menu->MergeMenu(runtimeTwo,m_gStatus->m_3dDevice,1);
			m_gStatus->LoadTexturesForMenu(menu);
			runtimeTwo->SafeDelete();
			delete runtimeTwo;
			runtimeTwo = 0;
			menu->m_lockedForChildMenuResponse = TRUE;
		}*/
		m_durationToServerStamp = currentTime;
	}//end mereg menu as temp
	if(clickObj->m_action == 56)
	{//close temp menus
		m_durationToServerStamp = currentTime;
		menu->m_clearTempMenus = TRUE;
		//menu->ClearTempMenus();
		return TRUE;
	}//end close temp menus
	if(clickObj->m_action == 57)
	{//increment up
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visual)
		{
			int currentTempInt = atoi(visual->m_textObject)+1;
			if(currentTempInt < 1)
				currentTempInt = 1;
			visual->m_textObject = m_gStatus->m_cabLib.ConvToString(currentTempInt);
		}
	}//end increment up
	if(clickObj->m_action == 58)
	{//increment down
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visual)
		{
			int currentTempInt = atoi(visual->m_textObject)-1;
			if(currentTempInt < 1)
				currentTempInt = 1;
			visual->m_textObject = m_gStatus->m_cabLib.ConvToString(currentTempInt);
		}
	}//end increment down
	if(clickObj->m_action == 59)
	{//transfer text
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		CMenuVisObj * visual2 =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt2);
		if(visual2)
		if(visual)
			visual->m_textObject = visual2->m_textObject;
		m_durationToServerStamp = currentTime;
	}//end transfer text
	if(clickObj->m_action == 60)
	{//set Macro
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visual)
		{
			menu->m_macroRepeatCount = atoi(visual->m_textObject);
			if(menu->m_macroRepeatCount < 1)
				menu->m_macroRepeatCount = 1;
			menu->m_macroRunning = TRUE;
			menu->m_macroActionType = clickObj->m_miscInt2;
			menu->m_macroCurrent = 0;
			menu->m_macroLastActionTimeStamp = GetTickCount();
			menu->m_macroStartStamp = GetTickCount();
			menu->m_macroManditoryWait = 10000;
		}
		m_durationToServerStamp = currentTime;
	}//end set Macro
	if(clickObj->m_action == 61)
	{//abort Macro
		 menu->m_macroRunning = FALSE;
		 m_durationToServerStamp = currentTime;
	}//end abort Macro
	if(clickObj->m_action == 62)
	{//multi Bind On Off
		if((GetTickCount()-m_clickStamp) > 1000)
		{
			clickObj->m_switchOverride = TRUE;
			POSITION cntPos = m_gStatus->m_controlsList->FindIndex(0);
			if(cntPos)
			{//1
				ControlDBObj* cObj = (ControlDBObj*)m_gStatus->m_controlsList->GetAt(cntPos);
				if(cObj->m_allowMultiBind)
				{
					cObj->m_allowMultiBind = FALSE;
					clickObj->m_switchOverrideState = 0;
				}
				else
				{
					cObj->m_allowMultiBind = TRUE;
					clickObj->m_switchOverrideState = 3;
				}
				m_clickStamp = GetTickCount();
			}//end 1
		}
	}//end multi Bind On Off
	//--------------------------------------------
	if(clickObj->m_action == 63)
	{//restore default controls 2
		RestoreFromDefaults();
		CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visual)
		if(visual->m_sliderSystem)
		{
			if(visual->m_type == 73)
			   m_gStatus->InListActions(0,visual->m_sliderSystem, TRUE);
			else
               m_gStatus->InListActions(0,visual->m_sliderSystem, FALSE);
		}
		/*CMenuVisObj * visual =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
		if(visual)
		if(visual->m_sliderSystem)
		{//valid
			//m_gStatus->InListActions(0,visual->m_sliderSystem);
			visual->m_sliderSystem->FlushAll();
			POSITION cntPos = m_gStatus->m_controlsList->FindIndex(0);
			if(cntPos)
			{//1
				ControlDBObj* cObj = (ControlDBObj*)m_gStatus->m_controlsList->GetAt(cntPos);
				for(POSITION cPos = cObj->List_controlInfoList->GetHeadPosition();cPos != NULL;)
				{
					ControlInfoObj * infoObj = (ControlInfoObj *)cObj->List_controlInfoList->GetNext(cPos);
					CString build = infoObj->GetActionDescription();
					CString assignmnt = cObj->GetButtonVis(infoObj->List_buttonDown,infoObj->List_controlType);
					//build = assignmnt + " :" + build;
					visual->m_sliderSystem->AddString(build, -1, 0, 0, 0, 0);
					visual->m_sliderSystem->AddString(assignmnt, -1, 0, 0, 0, 0);
				}
				visual->m_sliderSystem->m_currentSelectedContentItem = -1;
			}//end 1
		}//end valid*/
		m_gStatus->SaveControlConfig("KeyConfig.cnt");
		m_clickHandlerStamp = currentTime;
	}//end restore default controls 2
	//--------------------------------------------
	if(clickObj->m_action == 64)
	{//call quest journal
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
		 
			m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
															DPID_SERVERPLAYER,
															45,
															0,
															ptrLoc->List_networkDefinedID,
															0,
															0,
															NULL);//request purchase of a store item
		}//end valid
		m_clickHandlerStamp = currentTime;
	}//end call quest journal
	//------------------------------------------
	if(clickObj->m_action == 65)
	{//Report Murder
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		if(entPosition)
		{//valid Alpha  
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(ptrLoc->List_controlType == 0)
			{//user define 
				CMenuVisObj * visObj =  (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
				if(visObj != NULL)
				{//valid
					int bountyLoc = atoi(visObj->m_textObject);
					if(bountyLoc > -1 && bountyLoc < 30000000){}
					else
					{
						if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine("Invalid amount-defaulting to 1 ");
						bountyLoc = 1;
					}
					m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																	DPID_SERVERPLAYER,
																	46,
																	menu->m_passedInVar,//handle
																	ptrLoc->List_networkDefinedID,
																	bountyLoc,//bounty amount
																	0,
																	NULL);//request purchase of a store item
					m_clickHandlerStamp = currentTime;
					menu->CloseMenu(&m_gStatus->m_globalCursorPos);
				}//end valid
			}//end user define
		}//end valid Alpha
		m_clickHandlerStamp = currentTime;
	}//End Report Murder
	//------------------------------------------
	if(clickObj->m_action == 66)
	{//Enable disable music
		if((GetTickCount()-m_clickStamp) > 1000)
		{
			clickObj->m_switchOverride = TRUE;
			if(m_gStatus->m_soundTrackList->m_musicPlayer->m_disabled)
			{
				m_gStatus->m_soundTrackList->m_musicPlayer->m_disabled = FALSE;
				clickObj->m_switchOverrideState = 0;

			}
			else
			{
				m_gStatus->m_soundTrackList->m_musicPlayer->m_disabled = TRUE;
				clickObj->m_switchOverrideState = 3;
			}
			m_clickStamp = GetTickCount();
		}
	}//end Enable disable music
	//------------------------------------------
	if(clickObj->m_action == 67)
	{//Open menu X and Close Menu Y
		m_gStatus->m_runtimeMenuDataBase->CloseByID(clickObj->m_miscInt2);
		m_gStatus->OpenMenu(clickObj->m_miscInt, false);
	}//end Open menu X and Close Menu Y
	//------------------------------------------
	if(clickObj->m_action == 68)
	{//Toggle Menu
		if(CMenu3DObj *menu = m_gStatus->m_runtimeMenuDataBase->GetByID(clickObj->m_miscInt))
			menu->CloseMenu(NULL);
		else
			m_gStatus->OpenMenu(clickObj->m_miscInt, false);
	}//end Toggle Menu

	if(clickObj->m_action == 69)
	{//Loot All Items
		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
		{
			POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
			if(entPosition)
			{//valid    
				int quanity = 1;
				CMenuVisObj * qPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt3);
				if(qPtr)
				if(qPtr->m_textObject)
				{
					int quanityCnv = atoi(qPtr->m_textObject);
					if(quanityCnv > 0 && quanityCnv < 30000)
						quanity = quanityCnv;
					else if(m_gStatus->m_systemReadout)
						m_gStatus->m_systemReadout->AddLine("Invalid quanity-defaulting to 1 ");
				}
				
				
				CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
				int glID = GetMiscIntFromMenuList(clickObj->m_miscInt);

				/* if(glID == -50)
					RemoveSelectedFromMenuList(clickObj->m_miscInt);
				else
					MinusSelectedFromMenuList(clickObj->m_miscInt,glID,quanity); */

				//Send Attibute Message to place all items into inventory
				m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,DPID_SERVERPLAYER,48,glID,ptrLoc->List_networkDefinedID,0,quanity,NULL);
				m_clickHandlerStamp = currentTime;

				//Always close menu list
				CMenuVisObj * visPtr = (CMenuVisObj *) GetVisualByID(clickObj->m_miscInt);
				
				menu->CloseMenu(&m_gStatus->m_globalCursorPos);
			}//end valid		
		}
	}//End Loot All Items
	//Teleport
	if(clickObj->m_action == 71)
	{
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);

		if(entPosition)
		{//Valid entPosition
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			if(!ptrLoc)
				return FALSE;

			//setting the new player's position
			ptrLoc->List_baseFrame->SetPosition2(NULL,(float)clickObj->m_miscInt, (float)clickObj->m_miscInt2, (float)clickObj->m_miscInt3);
			ptrLoc->List_baseFrame->GetPosition(NULL,&ptrLoc->List_movementDB->List_lastPosition);
			ptrLoc->m_collisionRaySys->InitOnSpawn(ptrLoc->List_baseFrame,ptrLoc->List_skeletalObject,&m_gStatus->m_matLib);
		}

	} //End Teleport

	if(clickObj->m_action == 70 || clickObj->m_action == 22 || clickObj->m_action == 15)
	{//Activate Item
		POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);

		if(entPosition)
		{//Valid entPosition
			CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
			int dupPassed = 0;
			int glID = GetMiscIntFromMenuList(clickObj->m_miscInt,dupPassed);
			CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(glID);
			
			if(invObj)
			{//Valid Inventory Object
				
				if(invObj->m_useType != 0 && invObj->m_useType != 11) //11 will signify that its a faction item attachment
				{//Dont Use Items Without a Use Type
					//RemoveSelectedFromMenuList(clickObj->m_miscInt);
					BOOL useItemNow = TRUE;
					if(invObj->m_useType == 6)
					{//house item verify placement validity locally before even using this item
						D3DVECTOR positionWld;
						ptrLoc->List_baseFrame->GetPosition(NULL,&positionWld);
						CHousingObj * houseBasePtr = m_gStatus->m_housingDB->GetByIndex(invObj->m_miscUseValue);
						if(houseBasePtr)
						{//valid database base
							if(!HousePlacementValidCheck(positionWld,houseBasePtr->m_clipBox))
							{
								useItemNow = FALSE;
								if(m_gStatus->m_systemReadout)
									m_gStatus->m_systemReadout->AddLine("Ground would not support your house well here...");
							}
						}//end valid database base
						else
						{//cannot find house DB ref
							 useItemNow = FALSE;
							 if(m_gStatus->m_systemReadout)
								m_gStatus->m_systemReadout->AddLine("Invalid database error 23464...");
						}//end cannot find house DB ref
						m_clickHandlerStamp = currentTime;
					}//end house item verify placement validity locally before even using this item
					if(useItemNow)
					{
						MinusSelectedFromMenuList(clickObj->m_miscInt,glID,1);
						if(invObj->m_reloadTime == 0)
						{
													
							//For Conjugation Energy Reduction
							if(invObj->m_useType == 12)
							{
								//Check to make sure the skill we are going to use is conjugation
								if(invObj->m_requiredSkillType == 13)
								{
									//Retrieve the entity
									POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
									if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
									if(entPosition)
									{//valid Alpha  
										CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
										if(ptrLoc->List_controlType == 0)
										{
											int EnergyCost = 0;
											POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(0);
											if(posLoc)
											{														
												EnergyCost = 10; //hardcoded for the moment
												CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(posLoc);
												if(ptrLoc)
												{
													//Reduce the current energy by the energy cost
													ptrLoc->m_energyCurrent -= EnergyCost;
													if(ptrLoc->m_energyCurrent < 0)
													{//If we go below 0, we don't have enough energy, return Negator
														ptrLoc->m_energyCurrent = 0;
														if(m_gStatus->m_systemReadout)
															m_gStatus->m_systemReadout->AddLine("Not enough energy!");
														return FALSE;
													}
												}
											}
										}
									}//End valid alpha
								}
							}//End conjugation Specific

							m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
								DPID_SERVERPLAYER,27,//use item
								glID,ptrLoc->List_networkDefinedID,
								dupPassed,0,NULL); 
						}
						else
							 m_gStatus->m_pendingObject->AddCommand(1,glID,ptrLoc->List_networkDefinedID,invObj->m_reloadTime,"");
					}
				}//End Dont Use Items Without a Use Type
				else
				{//Attempt to Arm Item
					if(glID != -1)
					{//Valid Object Selected						
						if(ptrLoc->List_controlType == 0)
						{//Attempt to Arm Item
							 m_gStatus->ArmEntityItemSys(ptrLoc,glID,dupPassed);
						}//End Attempt to Arm Item
					}//End Valid Object Selected
				}//End Attempt to Arm Item
			}//End Valid Inventory Object
		}//End Valid entPosition

		//Set Click Handler Timestamp
		m_clickHandlerStamp = currentTime;
	}//End Activate Item


	if(clickObj->m_action != 55  && (menu->m_menuAccessID != 1123 || (clickObj->m_action != 24 && clickObj->m_action != 25)))
		menu->m_clearTempMenus = TRUE;
    
	return TRUE;
}
//------------------------------------------------------------------------------------//
int CMenuManagement::GetCurSelFromMenuList(int controlID,CMenuVisObj ** visRef)
{
	for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//loop through menus
		CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
		if(menu->m_visualItems)
		for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
		{//visual items loop
			CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
			if(visPtr->m_sliderSystem)
			if(visPtr->m_sliderSystem->m_controlID == controlID)
			{//correct control ID
				if(visRef)
				   *visRef = visPtr;
				return visPtr->m_sliderSystem->m_currentSelectedContentItem;
			}//end correct control ID
		}//end visual items loop
	}//end loop through menus
	return -1;
}
//------------------------------------------------------------------------------------//
CMenuVisObj * CMenuManagement::GetVisualByID(int controlID)
{
	for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//loop through menus
		CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
		if(menu->m_visualItems)
		for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
		{//visual items loop
			CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
			if(visPtr->m_visualID == controlID)
				return visPtr;
		}//end visual items loop
	}//end loop through menus
	return NULL;
}
//------------------------------------------------------------------------------------//
BOOL CMenuManagement::CreateCharacterThroughMenus(int nameID, int entityID, int spawnIndex)
{
    CMenuVisObj * entityReference = GetVisualByID(entityID);
    if(!entityReference)
		return FALSE;
	if(!entityReference->m_entityObjectLink)
		return FALSE;

    CMenuVisObj * entityName = GetVisualByID(nameID);
	if(!entityName)
		return FALSE;
		
	m_clientNet->CreateCharacterOnServer(	entityReference->m_entityObjectLink->List_refModel,
											spawnIndex,
											entityName->m_textObject,
											entityReference);
    
	return TRUE;
}
//------------------------------------------------------------------//
BOOL CMenuManagement::IndexUpDim(CSkeletonObject * runtimeSkeletal,
								 int cfg,
								 CMovementObjList * entityDB,
								 int refModel)
{
    if(runtimeSkeletal->m_deformableMeshArrayCount <= cfg)
		 return FALSE;

    CMovementObj * dbPtr = entityDB->GetByID(refModel);
	if(!dbPtr)
		return FALSE;
    //CMovementObj * dbPtr = (CMovementObj *) entityDB->GetAt(posRef);

	int offset = 1;
    if(!dbPtr->List_skeletalObject->m_alternateConfigurations)
		  return FALSE;
     
	while(1)
	{
		POSITION alPos = dbPtr->List_skeletalObject->m_alternateConfigurations->FindIndex(cfg);
		if(!alPos)
			return FALSE;

		CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) dbPtr->List_skeletalObject->m_alternateConfigurations->GetAt(alPos);
		POSITION curCfgPos = altPtr->m_configurations->FindIndex((runtimeSkeletal->m_spawnCfg[cfg]+offset));
		if(!curCfgPos)
			return FALSE;

		CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
        if(dMesh->m_publicDim)
            break;
		offset++;
	}
	if(!dbPtr->List_skeletalObject->HotSwapDim(cfg,(runtimeSkeletal->m_spawnCfg[cfg]+offset),runtimeSkeletal,m_gStatus->m_3dDevice,TRUE))
		return FALSE;
      
	return TRUE;
}
//------------------------------------------------------------------//
BOOL CMenuManagement::IndexDownDim(CSkeletonObject * runtimeSkeletal,
								     int cfg,
								     CMovementObjList * entityDB,
								     int refModel)
{

    if(runtimeSkeletal->m_deformableMeshArrayCount <= cfg)
		 return FALSE;

    CMovementObj * dbPtr = entityDB->GetByID(refModel);
	if(!dbPtr)
		return FALSE;
        //CMovementObj * dbPtr = (CMovementObj *) entityDB->GetAt(posRef);

        int offset = 1;
        if(!dbPtr->List_skeletalObject->m_alternateConfigurations)
			  return FALSE;

     
		while(1)
		{

		  if((runtimeSkeletal->m_spawnCfg[cfg]-offset) < 0)
			return FALSE;

		POSITION alPos = dbPtr->List_skeletalObject->m_alternateConfigurations->FindIndex(cfg);
		  if(!alPos)
			  return FALSE;
		CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) dbPtr->List_skeletalObject->m_alternateConfigurations->GetAt(alPos);
		POSITION curCfgPos = altPtr->m_configurations->FindIndex((runtimeSkeletal->m_spawnCfg[cfg]-offset));
		if(!curCfgPos)
			  return FALSE;
		CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
        if(dMesh->m_publicDim)
            break;
		offset++;
		}
		
		if((runtimeSkeletal->m_spawnCfg[cfg]-offset) < 0)
			return FALSE;
		
		if(!dbPtr->List_skeletalObject->HotSwapDim(cfg,(runtimeSkeletal->m_spawnCfg[cfg]-offset),runtimeSkeletal,m_gStatus->m_3dDevice,TRUE))
			return FALSE;
      
	return TRUE;
}
//------------------------------------------------------------------------------------//
int CMenuManagement::GetMiscIntFromMenuList(int controlID)
{
   for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
		if(menu->m_visualItems)
			for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
			{//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
				if(visPtr->m_sliderSystem)
				{//valid slider system
				if(visPtr->m_sliderSystem->m_contentList)
                if(visPtr->m_sliderSystem->m_controlID == controlID)
				{//correct control ID
                    int miscIntRet = 0;
					int dupPassed = 0;
					if(visPtr->m_sliderSystem->GetInfoByIndex(visPtr->m_sliderSystem->m_currentSelectedContentItem, &miscIntRet, &dupPassed,NULL))
					{

						 return miscIntRet;
					}
					else
						  return -1;
				}//end correct control ID
				}//end valid slider system
			}//end visual items loop
   }//end loop through menus
   return -1;
}

//------------------------------------------------------------------------------------//
int CMenuManagement::GetMiscIntFromMenuList(int controlID, int & dupPassed)
{
   for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
		if(menu->m_visualItems)
			for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
			{//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
				if(visPtr->m_sliderSystem)
				{//valid slider system
				if(visPtr->m_sliderSystem->m_contentList)
                if(visPtr->m_sliderSystem->m_controlID == controlID)
				{//correct control ID
                    int miscIntRet = 0;
					dupPassed = 0;
					if(visPtr->m_sliderSystem->GetInfoByIndex(visPtr->m_sliderSystem->m_currentSelectedContentItem, &miscIntRet, &dupPassed,NULL))
					{

						 return miscIntRet;
					}
					else
						  return -1;
				}//end correct control ID
				}//end valid slider system
			}//end visual items loop
   }//end loop through menus
   return -1;
}
//------------------------------------------------------------------------------------//
int CMenuManagement::MinusSelectedFromMenuList(int controlID,int glid,int quanity)
{
   for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
		if(menu->m_visualItems)
			for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
			{//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
				if(visPtr->m_sliderSystem)
				{//valid slider system
				if(visPtr->m_sliderSystem->m_contentList)
                if(visPtr->m_sliderSystem->m_controlID == controlID)
				{//correct control ID
					CSliderContentObj * slObjPtr = NULL;
					if(visPtr->m_sliderSystem->m_mode == 0)
                       slObjPtr = visPtr->m_sliderSystem->m_contentList->GetByIndex(visPtr->m_sliderSystem->m_currentSelectedContentItem);
					else if(visPtr->m_sliderSystem->m_mode == 1)
						slObjPtr = visPtr->m_sliderSystem->m_contentList->GetByIndex(visPtr->m_sliderSystem->m_currentSelectedContentItem+1);
					
					int trueCountOfQuanityRemoved = 1;
                    if(!slObjPtr)
						return 1;
						  int originalQuanity = slObjPtr->m_miscInt2;
						if(quanity > slObjPtr->m_miscInt2)
							 quanity = slObjPtr->m_miscInt2;
						
						trueCountOfQuanityRemoved = quanity;						
						slObjPtr->m_miscInt2-=quanity;

						  if(slObjPtr->m_miscInt2 < 1)
						{//depleted
							 RemoveSelectedFromMenuList(controlID); 
						}//end depleted
						else{//rename
                           //CString lengthString = m_gStatus->m_cabLib.ConvToString(originalQuanity);
                           //int remAmnt = lengthString.GetLength();
							 CString oldString = m_gStatus->m_cabLib.ConvToString(originalQuanity);
							 //CString newString = m_gStatus->m_cabLib.ConvToString(slObjPtr->m_miscInt2);
							 
                           //int diff = oldString.GetLength() - newString.GetLength();
						
                           int strOrigLength = slObjPtr->m_text.GetLength();
							 int removeCount = oldString.GetLength();

							 if(strOrigLength > removeCount)
                           slObjPtr->m_text.Delete((strOrigLength-removeCount),removeCount);

                           slObjPtr->m_text = operator+(slObjPtr->m_text,m_gStatus->m_cabLib.ConvToString(slObjPtr->m_miscInt2));
						}//end rename
						return trueCountOfQuanityRemoved;
				}//end correct control ID
				}//end valid slider system
			}//end visual items loop
   }//end loop through menus
   return 1;
}
//------------------------------------------------------------------------------------//
int CMenuManagement::MinusSelectedFromMenuList(int controlID,int glid,int quanity,CString & lastItemName)
{
   for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
		if(menu->m_visualItems)
			for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
			{//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
				if(visPtr->m_sliderSystem)
				{//valid slider system
				if(visPtr->m_sliderSystem->m_contentList)
                if(visPtr->m_sliderSystem->m_controlID == controlID)
				{//correct control ID
					CSliderContentObj * slObjPtr = NULL;
					if(visPtr->m_sliderSystem->m_mode == 0)
                       slObjPtr = visPtr->m_sliderSystem->m_contentList->GetByIndex(visPtr->m_sliderSystem->m_currentSelectedContentItem);
					else if(visPtr->m_sliderSystem->m_mode == 1)
						slObjPtr = visPtr->m_sliderSystem->m_contentList->GetByIndex(visPtr->m_sliderSystem->m_currentSelectedContentItem+1);
					
					int trueCountOfQuanityRemoved = 1;
                    if(!slObjPtr)
						return 1;
						  int originalQuanity = slObjPtr->m_miscInt2;
						//lastItemName = slObjPtr->m_textVisual->m_text;
						if(quanity > slObjPtr->m_miscInt2)
							 quanity = slObjPtr->m_miscInt2;
						
						trueCountOfQuanityRemoved = quanity;						
						slObjPtr->m_miscInt2-=quanity;

						  if(slObjPtr->m_miscInt2 < 1)
						{//depleted
							 lastItemName = RemoveSelectedFromMenuList(controlID); 
						}//end depleted
						else{//rename
                           //CString lengthString = m_gStatus->m_cabLib.ConvToString(originalQuanity);
                           //int remAmnt = lengthString.GetLength();
							 CString oldString = m_gStatus->m_cabLib.ConvToString(originalQuanity);
							 //CString newString = m_gStatus->m_cabLib.ConvToString(slObjPtr->m_miscInt2);
							 
                           //int diff = oldString.GetLength() - newString.GetLength();
						
                           int strOrigLength = slObjPtr->m_text.GetLength();
							 int removeCount = oldString.GetLength();

							 if(strOrigLength > removeCount)
                           slObjPtr->m_text.Delete((strOrigLength-removeCount),removeCount);

                           slObjPtr->m_text = operator+(slObjPtr->m_text,m_gStatus->m_cabLib.ConvToString(slObjPtr->m_miscInt2));
						}//end rename
						return trueCountOfQuanityRemoved;
				}//end correct control ID
				}//end valid slider system
			}//end visual items loop
   }//end loop through menus
   return 1;
}
//------------------------------------------------------------------------------------//
CString CMenuManagement::RemoveSelectedFromMenuList(int controlID)
{
   CString removedText = "";
   for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
		if(menu->m_visualItems)
			for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
			{//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
				if(visPtr->m_sliderSystem)
				{//valid slider system
				if(visPtr->m_sliderSystem->m_contentList)
                if(visPtr->m_sliderSystem->m_controlID == controlID)
				{//correct control ID
					if(visPtr->m_sliderSystem->m_mode == 0)
					{
						POSITION posObjContent = visPtr->m_sliderSystem->m_contentList->FindIndex(visPtr->m_sliderSystem->m_currentSelectedContentItem);
						if(!posObjContent)
							return removedText;
						
						CSliderContentObj * slObjPtr = (CSliderContentObj *)visPtr->m_sliderSystem->m_contentList->GetAt(posObjContent);
						removedText = slObjPtr->m_text;
						slObjPtr->SafeDelete();
						delete slObjPtr;
						slObjPtr = 0;
						
						visPtr->m_sliderSystem->m_contentList->RemoveAt(posObjContent);
						visPtr->m_sliderSystem->m_itemCount--;

						visPtr->m_sliderSystem->m_currentSelectedContentItem -= 1;
						if(visPtr->m_sliderSystem->m_currentSelectedContentItem < 0)
							 visPtr->m_sliderSystem->m_currentSelectedContentItem = 0;
						return removedText;
					}
					else if(visPtr->m_sliderSystem->m_mode == 1)
					{
                        for(int loopOpt=0;loopOpt<2;loopOpt++)
						{
							if(visPtr->m_sliderSystem->m_currentSelectedContentItem < 0)
							continue;

							POSITION posObjContent = visPtr->m_sliderSystem->m_contentList->FindIndex(visPtr->m_sliderSystem->m_currentSelectedContentItem);
							if(!posObjContent)
								return removedText;
						    
							CSliderContentObj * slObjPtr = (CSliderContentObj *)visPtr->m_sliderSystem->m_contentList->GetAt(posObjContent);
						    if(slObjPtr)
							{
								if(removedText.GetLength() == 0)
									removedText = slObjPtr->m_text;
								
								slObjPtr->SafeDelete();
								delete slObjPtr;
								slObjPtr = 0;
								
								visPtr->m_sliderSystem->m_contentList->RemoveAt(posObjContent);
								visPtr->m_sliderSystem->m_itemCount--;
							}
						}

						visPtr->m_sliderSystem->m_currentSelectedContentItem -= 2;
						if(visPtr->m_sliderSystem->m_currentSelectedContentItem < 0)
							 visPtr->m_sliderSystem->m_currentSelectedContentItem = 0;
						return removedText;
					}
				}//end correct control ID
				}//end valid slider system
			}//end visual items loop
   }//end loop through menus
   return removedText;
}
//-----------------------------------------------------------//
BOOL CMenuManagement::HousePlacementValidCheck(D3DVECTOR charPosition,ABBOX box)
{

  D3DVECTOR collisionIntersect,normalRet;
  int polysExactChecked = 0;
  int polysConsidered = 0;
  int ObjectIndexOfCollision = 0;
  BOOL result = FALSE;

  D3DVECTOR currentPosition,anchorPosition;

  //box seg 1
  currentPosition.x = box.minX + charPosition.x;
  currentPosition.y = box.maxY + charPosition.y;
  currentPosition.z = box.maxZ + charPosition.z;
  anchorPosition.x  = box.minX + charPosition.x;
  anchorPosition.y  = box.minY + charPosition.y;
  anchorPosition.z  = box.maxZ + charPosition.z;
     
			result = m_gStatus->m_collisionBase->CollisionSegmentSI(
						               currentPosition,
						               anchorPosition,
						               &collisionIntersect.x,
						               &collisionIntersect.y,
						               &collisionIntersect.z,
						               &polysExactChecked,
						               &polysConsidered,
						               &normalRet,
						               &ObjectIndexOfCollision,
						               20.0f,
						               TRUE);
			if(!result)
				return FALSE;//not in ground

  //box seg 2
  currentPosition.x = box.maxX + charPosition.x;
  currentPosition.y = box.maxY + charPosition.y;
  currentPosition.z = box.maxZ + charPosition.z;
  anchorPosition.x  = box.maxX + charPosition.x;
  anchorPosition.y  = box.minY + charPosition.y;
  anchorPosition.z  = box.maxZ + charPosition.z;
     
			result = m_gStatus->m_collisionBase->CollisionSegmentSI(
						               currentPosition,
						               anchorPosition,
						               &collisionIntersect.x,
						               &collisionIntersect.y,
						               &collisionIntersect.z,
						               &polysExactChecked,
						               &polysConsidered,
						               &normalRet,
						               &ObjectIndexOfCollision,
						               20.0f,
						               TRUE);
			if(!result)
				return FALSE;//not in ground

  //box seg 3
  currentPosition.x = box.maxX + charPosition.x;
  currentPosition.y = box.maxY + charPosition.y;
  currentPosition.z = box.minZ + charPosition.z;
  anchorPosition.x  = box.maxX + charPosition.x;
  anchorPosition.y  = box.minY + charPosition.y;
  anchorPosition.z  = box.minZ + charPosition.z;
     
			result = m_gStatus->m_collisionBase->CollisionSegmentSI(
						               currentPosition,
						               anchorPosition,
						               &collisionIntersect.x,
						               &collisionIntersect.y,
						               &collisionIntersect.z,
						               &polysExactChecked,
						               &polysConsidered,
						               &normalRet,
						               &ObjectIndexOfCollision,
						               20.0f,
						               TRUE);
			if(!result)
				return FALSE;//not in ground

  //box seg 4
  currentPosition.x = box.minX + charPosition.x;
  currentPosition.y = box.maxY + charPosition.y;
  currentPosition.z = box.minZ + charPosition.z;
  anchorPosition.x  = box.minX + charPosition.x;
  anchorPosition.y  = box.minY + charPosition.y;
  anchorPosition.z  = box.minZ + charPosition.z;
     
			result = m_gStatus->m_collisionBase->CollisionSegmentSI(
						               currentPosition,
						               anchorPosition,
						               &collisionIntersect.x,
						               &collisionIntersect.y,
						               &collisionIntersect.z,
						               &polysExactChecked,
						               &polysConsidered,
						               &normalRet,
						               &ObjectIndexOfCollision,
						               20.0f,
						               TRUE);
			if(!result)
				return FALSE;//not in ground

  return TRUE;
}
//----------------Load control Config list--------------------------------------------//
void CMenuManagement::RestoreFromDefaults()
{
	SetCurrentDirectory(m_gStatus->GL_pathGameFiles);//use gamefiles folder for organization

	CFileException exc;
	CFile fl;
	BOOL res = fl.Open("KEYCONFIGDEFAULTS.CNT",CFile::modeRead, &exc);
	if(!res)
	    res = fl.Open("DefaultBak.CNT",CFile::modeRead, &exc);
	if(res == FALSE)
	{
	   SetCurrentDirectory(m_gStatus->GL_workingDirectory);
	   return;
	}
	m_gStatus->DestroyControlList();
	CArchive the_in_Archive(&fl,CArchive::load);
	the_in_Archive >> m_gStatus->m_controlsList;
	the_in_Archive.Close();
	fl.Close();
	SetCurrentDirectory(m_gStatus->GL_workingDirectory); 
	m_gStatus->InitializeControlDB();
}

bool CMenuManagement::HandleInput(CInput *pInput)
{
	CMenuWindow *pHandlerWindow = NULL;

	// If the mouse cursor shouldn't be shown, or
	// If we're editing a scroll bar, button, or toggle, don't handle input
	if(/*!m_gStatus->m_showMenuCursor || */m_gStatus->m_runtimeMenuDatabase->m_freezeInput)
		return false;

	//Keep track of the focused state of the chat box
	bool chatHadFocus = false;
	if(m_gStatus->m_menuChatInput)
		chatHadFocus = m_gStatus->m_menuChatInput->m_isFocused;

	//Calculate half the screen dimensions for use later
	float halfWidth	 = m_gStatus->m_dxRenderer->m_currentWidth * 0.5f;
	float halfHeight = m_gStatus->m_dxRenderer->m_currentHeight * 0.5f;

	// IF we have a valid game cursor...
	if (m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor])
	{
		// IF it's visible, MOVE it to its updated location
		if (m_gStatus->m_showMenuCursor)
		{
			m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor]->m_posX = pInput->m_mouseAbsPosX;
			m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor]->m_posY = pInput->m_mouseAbsPosY - m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor]->m_height;
		}
		// ELSE, anchor the cursor to the center of the game screen
		else
		{
			pInput->m_mouseAbsPosX = halfWidth;
			pInput->m_mouseAbsPosY = halfHeight;
		}
	}

	//Loop through every menu that's currently being rendered
	POSITION posLast;
	CMenuWindow *menu = NULL;
	bool inputHandled = false;
	for (POSITION posLoc = m_gStatus->m_runtimeMenuDatabase->GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
		try
		{
			//If the menu's not visible or not enabled, then it can't take input
			menu = dynamic_cast<CMenuWindow*>(m_gStatus->m_runtimeMenuDatabase->GetNext(posLoc));
			if(!menu || !menu->m_visible || !menu->m_enabled)
				continue;
		}
		catch(std::bad_typeid)
		{
			TRACE("dynamic_cast<CMenuWindow*> FAILED in CMenuManagement::HandleInput(CInput *pInput).\n");
			continue;
		}

		//Store the old focused setting to know if we
		bool hadFocus = menu->m_isFocused;

		// If we have an item we're currently dragging...
		if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown())
		{
			// Toggle the dragging flag
			m_gStatus->m_bIsDragging = true;
		}
		// ...but the left mouse button is no longer down...	
		else if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased())
		{
			// Toggle the dragging flag
			m_gStatus->m_bIsDragging = false;
		}
		
		//See if this menu can handle the input
		if(!inputHandled)
		{
			inputHandled = menu->HandleInput(pInput, 
											 halfWidth + halfWidth * menu->m_horizontalAlignment,
											 halfHeight + halfHeight * menu->m_verticalAlignment,
											 m_gStatus->GetTimeStamp_DW(),
											 m_gStatus->m_3dDevice);
			if(inputHandled) pHandlerWindow = menu;

			//If the menu has focus, but it didn't used to
			if(menu->m_isFocused && !hadFocus)
			{
				//Move it to the front of the menu list
				m_gStatus->m_runtimeMenuDatabase->RemoveAt(posLast);
				m_gStatus->m_runtimeMenuDatabase->AddHead(menu);
			}
		}
	}

	//If the chat box had focus and lost it, hide the mouse
	if(chatHadFocus && !m_gStatus->m_menuChatInput->m_isFocused)
		m_gStatus->m_showMenuCursor--;

	// Handle interactive Minimap / Quest Tracker / F12 Screenshot HUD input
	if(!inputHandled)
		inputHandled = HandleHudOverlayInput(pInput);

	//If the menu system ate the input, return true
	if(inputHandled)
		return true;

	return false;
}

namespace
{
	struct HudVertex2D
	{
		float x, y, z, rhw;
		DWORD color;
	};
	const DWORD HUD_FVF_2D = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;

	void DrawHudRectFill(LPDIRECT3DDEVICE9 dev, float left, float top, float right, float bottom, DWORD colorTop, DWORD colorBottom)
	{
		HudVertex2D v[4] = {
			{ left,  top,    0.0f, 1.0f, colorTop },
			{ right, top,    0.0f, 1.0f, colorTop },
			{ left,  bottom, 0.0f, 1.0f, colorBottom },
			{ right, bottom, 0.0f, 1.0f, colorBottom }
		};
		dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(HudVertex2D));
	}

	void DrawHudRectBorder(LPDIRECT3DDEVICE9 dev, float left, float top, float right, float bottom, DWORD color)
	{
		HudVertex2D v[5] = {
			{ left,  top,    0.0f, 1.0f, color },
			{ right, top,    0.0f, 1.0f, color },
			{ right, bottom, 0.0f, 1.0f, color },
			{ left,  bottom, 0.0f, 1.0f, color },
			{ left,  top,    0.0f, 1.0f, color }
		};
		dev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, v, sizeof(HudVertex2D));
	}

	void DrawHudDisc(LPDIRECT3DDEVICE9 dev, float cx, float cy, float radius, DWORD centerColor, DWORD edgeColor, int segments = 32)
	{
		if (segments < 8) segments = 8;
		if (segments > 64) segments = 64;
		HudVertex2D v[66];
		v[0].x = cx;
		v[0].y = cy;
		v[0].z = 0.0f;
		v[0].rhw = 1.0f;
		v[0].color = centerColor;
		for (int i = 0; i <= segments; ++i)
		{
			float ang = (float)i * (6.2831853f / (float)segments);
			v[i + 1].x = cx + cosf(ang) * radius;
			v[i + 1].y = cy + sinf(ang) * radius;
			v[i + 1].z = 0.0f;
			v[i + 1].rhw = 1.0f;
			v[i + 1].color = edgeColor;
		}
		dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, segments, v, sizeof(HudVertex2D));
	}

	void DrawHudCircleRing(LPDIRECT3DDEVICE9 dev, float cx, float cy, float radius, DWORD color, int segments = 32)
	{
		if (segments < 8) segments = 8;
		if (segments > 64) segments = 64;
		HudVertex2D v[65];
		for (int i = 0; i <= segments; ++i)
		{
			float ang = (float)i * (6.2831853f / (float)segments);
			v[i].x = cx + cosf(ang) * radius;
			v[i].y = cy + sinf(ang) * radius;
			v[i].z = 0.0f;
			v[i].rhw = 1.0f;
			v[i].color = color;
		}
		dev->DrawPrimitiveUP(D3DPT_LINESTRIP, segments, v, sizeof(HudVertex2D));
	}

	void DrawHudDiamond(LPDIRECT3DDEVICE9 dev, float cx, float cy, float r, DWORD fillColor, DWORD borderColor)
	{
		HudVertex2D fill[4] = {
			{ cx,     cy - r, 0.0f, 1.0f, fillColor },
			{ cx + r, cy,     0.0f, 1.0f, fillColor },
			{ cx - r, cy,     0.0f, 1.0f, fillColor },
			{ cx,     cy + r, 0.0f, 1.0f, fillColor }
		};
		dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, fill, sizeof(HudVertex2D));
		HudVertex2D ring[5] = {
			{ cx,     cy - r, 0.0f, 1.0f, borderColor },
			{ cx + r, cy,     0.0f, 1.0f, borderColor },
			{ cx,     cy + r, 0.0f, 1.0f, borderColor },
			{ cx - r, cy,     0.0f, 1.0f, borderColor },
			{ cx,     cy - r, 0.0f, 1.0f, borderColor }
		};
		dev->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, ring, sizeof(HudVertex2D));
	}

	void DrawHudLine(LPDIRECT3DDEVICE9 dev, float x1, float y1, float x2, float y2, DWORD color)
	{
		HudVertex2D v[2] = {
			{ x1, y1, 0.0f, 1.0f, color },
			{ x2, y2, 0.0f, 1.0f, color }
		};
		dev->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(HudVertex2D));
	}

	void DrawHudTextShadowed(LPD3DXFONT font, const char* text, int left, int top, int right, int bottom, DWORD format, DWORD color)
	{
		if (!font || !text || !text[0])
			return;
		// ID3DXFont does not support the ellipsis flags - with them it draws nothing
		// (that is why the quest tracker and minimap titles were blank).
		format &= ~(DT_END_ELLIPSIS | DT_PATH_ELLIPSIS | DT_WORD_ELLIPSIS | DT_MODIFYSTRING);
		RECT rcShadow = { left + 1, top + 1, right + 1, bottom + 1 };
		font->DrawTextA(NULL, text, -1, &rcShadow, format, D3DCOLOR_ARGB(200, 0, 0, 0));
		RECT rcMain = { left, top, right, bottom };
		font->DrawTextA(NULL, text, -1, &rcMain, format, color);
	}

	bool PointInRect2D(float px, float py, float left, float top, float right, float bottom)
	{
		return (px >= left && px <= right && py >= top && py <= bottom);
	}

	// textured 2D vertices for the zone map picture
	struct HudVertexTex
	{
		float x, y, z, rhw;
		DWORD color;
		float u, v;
	};
	const DWORD HUD_FVF_TEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

	void BeginHudTexture(LPDIRECT3DDEVICE9 dev, LPDIRECT3DTEXTURE9 tex)
	{
		dev->SetTexture(0, tex);
		dev->SetFVF(HUD_FVF_TEX);
		dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		dev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		dev->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		dev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		dev->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		dev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		dev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		dev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	}

	void EndHudTexture(LPDIRECT3DDEVICE9 dev)
	{
		dev->SetTexture(0, NULL);
		dev->SetFVF(HUD_FVF_2D);
		dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	}

	// journal NPC name vs. the name in the .poi list ("Opot-Plorb" matches "Opot-Plorb (Items)")
	bool HudNameMatches(const CString &a, const CString &b)
	{
		if (a.IsEmpty() || b.IsEmpty())
			return false;
		if (a.CompareNoCase(b) == 0)
			return true;
		const CString &longer  = (a.GetLength() > b.GetLength()) ? a : b;
		const CString &shorter = (a.GetLength() > b.GetLength()) ? b : a;
		int n = shorter.GetLength();
		if (n < 3 || longer.Left(n).CompareNoCase(shorter) != 0)
			return false;
		char c = longer[n];
		return (c == ' ' || c == '(' || c == '-' || c == '[');
	}
}

void CMenuManagement::EnsureHudFonts(LPDIRECT3DDEVICE9 pDevice)
{
	if (!pDevice)
		return;
	if (m_hudFontDevice != pDevice)
	{
		ReleaseHudFonts();
		m_hudFontDevice = pDevice;
	}
	if (!m_hudFontSmall)
	{
		D3DXCreateFontA(
			pDevice,
			13, 0, FW_SEMIBOLD, 1, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Tahoma",
			&m_hudFontSmall);
	}
	if (!m_hudFontTitle)
	{
		D3DXCreateFontA(
			pDevice,
			14, 0, FW_BOLD, 1, FALSE,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "Tahoma",
			&m_hudFontTitle);
	}
}

void CMenuManagement::ReleaseHudFonts()
{
	if (m_hudFontSmall)
	{
		m_hudFontSmall->Release();
		m_hudFontSmall = NULL;
	}
	if (m_hudFontTitle)
	{
		m_hudFontTitle->Release();
		m_hudFontTitle = NULL;
	}
	m_hudFontDevice = NULL;
}

void CMenuManagement::ToggleQuestJournalWindow()
{
	if (!m_gStatus || !m_gStatus->m_runtimeMenuDatabase)
		return;

	CMenuWindow *pJournalMenu = dynamic_cast<CMenuWindow*>(
		m_gStatus->m_runtimeMenuDatabase->GetVisualByID(QUEST_JOURNAL_MENU, false));
	if (pJournalMenu && pJournalMenu->m_visible)
	{
		m_gStatus->m_runtimeMenuDatabase->CloseByID(QUEST_JOURNAL_MENU, &m_gStatus->m_showMenuCursor);
	}
	else
	{
		m_gStatus->OpenMenu(QUEST_JOURNAL_MENU, true);
		m_gStatus->RefreshQuestJournal(ID_QUEST_JOURNAL_MAIN_LIST, true);
	}
}

bool CMenuManagement::CaptureScreenshot()
{
	if (!m_gStatus || !m_gStatus->m_3dDevice)
		return false;

	CreateDirectoryA("Screenshots", NULL);

	SYSTEMTIME st;
	GetLocalTime(&st);
	CString fileName;
	fileName.Format("Screenshots\\EndlessAges_%04d%02d%02d_%02d%02d%02d_%03d.bmp",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	HRESULT hr = m_gStatus->m_3dDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	if (FAILED(hr) || !pBackBuffer)
		return false;

	hr = D3DXSaveSurfaceToFileA(fileName, D3DXIFF_BMP, pBackBuffer, NULL, NULL);
	pBackBuffer->Release();
	if (FAILED(hr))
		return false;

	m_hudToastText.Format("Screenshot saved: %s", (const char*)fileName);
	m_hudToastTime = GetTickCount();
	if (m_gStatus->m_systemReadout)
		m_gStatus->m_systemReadout->AddLine(m_hudToastText);
	return true;
}

bool CMenuManagement::HandleHudOverlayInput(CInput *pInput)
{
	if (!pInput || !m_gStatus || !m_gStatus->m_dxRenderer)
		return false;

	float screenW = (float)m_gStatus->m_dxRenderer->m_currentWidth;
	float screenH = (float)m_gStatus->m_dxRenderer->m_currentHeight;
	m_lastMouseScreenX = pInput->m_mouseAbsPosX;
	m_lastMouseScreenY = screenH - pInput->m_mouseAbsPosY;

	bool chatFocused = (m_gStatus->m_menuChatInput && m_gStatus->m_menuChatInput->m_isFocused);
	DWORD now = GetTickCount();

	// Big world map: while it is open it takes all input (no walking / shooting / camera)
	if (m_worldMapOpen)
	{
		if (!IsHudWorldActive())
		{
			SetWorldMapOpen(false);
			return false;
		}
		if (chatFocused)
			return false;
		if ((pInput->m_keyboardKeys[DIK_M].JustPressed() && !pInput->m_shiftOn) || pInput->m_keyboardKeys[DIK_ESCAPE].JustPressed())
		{
			m_lastHudKeyTime = now;
			SetWorldMapOpen(false);
			return true;
		}
		float mx = m_lastMouseScreenX, my = m_lastMouseScreenY;
		bool overMap = (m_wmSize > 0.0f) && PointInRect2D(mx, my, m_wmLeft, m_wmTop, m_wmLeft + m_wmSize, m_wmTop + m_wmSize);

		// zoom: mouse wheel over the map, [ ] or - =
		float zoomBy = 1.0f;
		if (overMap && pInput->m_mouseWheelZ > 0.0f) zoomBy = 1.25f;
		else if (overMap && pInput->m_mouseWheelZ < 0.0f) zoomBy = 0.8f;
		if (pInput->m_keyboardKeys[DIK_RBRACKET].JustPressed() || pInput->m_keyboardKeys[DIK_EQUALS].JustPressed() || pInput->m_keyboardKeys[DIK_ADD].JustPressed())
			zoomBy = 1.5f;
		if (pInput->m_keyboardKeys[DIK_LBRACKET].JustPressed() || pInput->m_keyboardKeys[DIK_MINUS].JustPressed() || pInput->m_keyboardKeys[DIK_SUBTRACT].JustPressed())
			zoomBy = 1.0f / 1.5f;
		if (zoomBy != 1.0f && m_wmSize > 0.0f && m_wmScale > 0.0f && m_wmFull > 0.0f)
		{
			float newZoom = min(8.0f, max(1.0f, m_worldMapZoom * zoomBy));
			if (newZoom != m_worldMapZoom)
			{
				// keep the spot under the mouse in place (keys zoom on the middle)
				bool useMouse = overMap && (pInput->m_mouseWheelZ != 0.0f);
				float fx = useMouse ? (mx - m_wmLeft) / m_wmSize : 0.5f;
				float fy = useMouse ? (my - m_wmTop) / m_wmSize : 0.5f;
				float wx = m_wmViewMinX + fx * m_wmSize / m_wmScale;
				float wz = m_wmViewMaxZ - fy * m_wmSize / m_wmScale;
				float span = m_wmFull / newZoom;
				m_worldMapZoom = newZoom;
				m_worldMapCenterX = wx - (fx - 0.5f) * span;
				m_worldMapCenterZ = wz + (fy - 0.5f) * span;
				ClampWorldMapCenter();
			}
		}

		// PgUp / PgDn: look at the floor above / below
		if (m_zoneMapLevels >= 2)
		{
			int cur = (m_worldMapViewLevel >= 0) ? m_worldMapViewLevel : m_zoneMapPlayerLevel;
			if (pInput->m_keyboardKeys[DIK_PRIOR].JustPressed() && cur < m_zoneMapLevels - 1)
				m_worldMapViewLevel = cur + 1;
			if (pInput->m_keyboardKeys[DIK_NEXT].JustPressed() && cur > 0)
				m_worldMapViewLevel = cur - 1;
		}

		// Space: zoom in on yourself (and your floor)
		if (pInput->m_keyboardKeys[DIK_SPACE].JustPressed() && m_gStatus->m_entObjRuntime && !m_gStatus->m_entObjRuntime->IsEmpty())
		{
			CMovementObj *me = (CMovementObj*)m_gStatus->m_entObjRuntime->GetHead();
			if (me)
			{
				m_worldMapViewLevel = -1;
				m_worldMapZoom = max(m_worldMapZoom, 3.0f);
				m_worldMapCenterX = me->List_lastPosition.x;
				m_worldMapCenterZ = me->List_lastPosition.z;
				ClampWorldMapCenter();
			}
		}

		if (pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
		{
			if (PointInRect2D(mx, my, m_wmCloseL, m_wmCloseT, m_wmCloseR, m_wmCloseB))
			{
				SetWorldMapOpen(false);
				return true;
			}
			if (m_zoneMapLevels >= 2 && mx >= m_wmFloorL && mx <= m_wmFloorR)
			{
				for (int lv = 0; lv < m_zoneMapLevels; lv++)
					if (my >= m_wmFloorT[lv] && my <= m_wmFloorB[lv])
					{
						m_worldMapViewLevel = (lv == m_zoneMapPlayerLevel) ? -1 : lv;
						return true;
					}
			}
			if (mx >= m_wmListL && mx <= m_wmListR)
			{
				for (size_t r = 0; r < m_wmQuestRows.size(); r++)
				{
					if (my < m_wmQuestRows[r].top || my > m_wmQuestRows[r].bottom)
						continue;
					int pi = m_wmQuestRows[r].poi;
					if (pi >= 0 && pi < (int)m_zoneMapPois.size())
					{
						m_worldMapZoom = max(m_worldMapZoom, 3.0f);
						m_worldMapCenterX = m_zoneMapPois[pi].x;
						m_worldMapCenterZ = m_zoneMapPois[pi].z;
						if (m_zoneMapLevels >= 2 && m_zoneMapPois[pi].hasY)
						{
							int lv = ZoneMapLevelOf(m_zoneMapPois[pi].y);
							m_worldMapViewLevel = (lv == m_zoneMapPlayerLevel) ? -1 : lv;
						}
						ClampWorldMapCenter();
					}
					else
					{
						m_hudToastText = "That quest NPC is not in this zone";
						m_hudToastTime = now;
					}
					return true;
				}
			}
			if (overMap)
			{
				m_worldMapDragging = true;
				m_wmDragX = mx;
				m_wmDragY = my;
			}
		}
		if (m_worldMapDragging)
		{
			if (pInput->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown() && m_wmScale > 0.0f)
			{
				m_worldMapCenterX -= (mx - m_wmDragX) / m_wmScale;
				m_worldMapCenterZ += (my - m_wmDragY) / m_wmScale;
				m_wmDragX = mx;
				m_wmDragY = my;
				ClampWorldMapCenter();
			}
			else
				m_worldMapDragging = false;
		}
		return true;
	}

	// Keyboard shortcuts when chat is not focused
	if (!chatFocused && (now - m_lastHudKeyTime > 180))
	{
		if (pInput->m_keyboardKeys[DIK_F12].JustPressed())
		{
			m_lastHudKeyTime = now;
			CaptureScreenshot();
			return true;
		}
		if (!pInput->m_ctrlOn && !pInput->m_altOn)
		{
			if (pInput->m_keyboardKeys[DIK_M].JustPressed())
			{
				if (pInput->m_shiftOn)
				{// Shift+M: minimap size
					m_lastHudKeyTime = now;
					m_minimapMode = (m_minimapMode + 1) % 3;
					m_hudToastText = (m_minimapMode == 0) ? "Minimap: Standard" :
					                 (m_minimapMode == 1) ? "Minimap: Large" : "Minimap: Compact";
					m_hudToastTime = now;
					return true;
				}
				if (IsHudWorldActive())
				{// M: big world map
					m_lastHudKeyTime = now;
					SetWorldMapOpen(true);
					return true;
				}
			}
			if (pInput->m_keyboardKeys[DIK_N].JustPressed())
			{
				m_lastHudKeyTime = now;
				m_minimapNorthUp = !m_minimapNorthUp;
				m_hudToastText = m_minimapNorthUp ? "Minimap: North-Up Locked" : "Minimap: Heading-Up";
				m_hudToastTime = now;
				return true;
			}
			if (pInput->m_keyboardKeys[DIK_LBRACKET].JustPressed())
			{
				m_lastHudKeyTime = now;
				m_minimapZoomRadius = max(40.0f, m_minimapZoomRadius * 0.75f);
				return true;
			}
			if (pInput->m_keyboardKeys[DIK_RBRACKET].JustPressed())
			{
				m_lastHudKeyTime = now;
				m_minimapZoomRadius = min(4000.0f, m_minimapZoomRadius * 1.33f);
				return true;
			}
			if (pInput->m_keyboardKeys[DIK_L].JustPressed())
			{
				m_lastHudKeyTime = now;
				m_questTrackerVisible = !m_questTrackerVisible;
				m_hudToastText = m_questTrackerVisible ? "Quest Tracker: Shown" : "Quest Tracker: Collapsed";
				m_hudToastTime = now;
				return true;
			}
		}
	}

	// Only handle HUD mouse clicks if local player is in-world and cursor is shown
	if (!m_gStatus->m_showMenuCursor || !m_gStatus->m_entObjRuntime || m_gStatus->m_entObjRuntime->IsEmpty())
		return false;

	if (!pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
		return false;

	float panelW = (m_minimapMode == 1) ? 260.0f : 196.0f;
	float panelRight = screenW - 12.0f;
	float panelLeft = panelRight - panelW;
	float headerTop = 12.0f;
	float headerBottom = headerTop + 22.0f;

	// Header buttons: [M] [N] [J] on right side of minimap header
	float btnW = 20.0f;
	float btnJLeft = panelRight - 4.0f - btnW;
	float btnNLeft = btnJLeft - 3.0f - btnW;
	float btnMLeft = btnNLeft - 3.0f - btnW;

	if (PointInRect2D(m_lastMouseScreenX, m_lastMouseScreenY, btnMLeft, headerTop + 2.0f, btnMLeft + btnW, headerBottom - 2.0f))
	{
		SetWorldMapOpen(true);
		return true;
	}
	if (PointInRect2D(m_lastMouseScreenX, m_lastMouseScreenY, btnNLeft, headerTop + 2.0f, btnNLeft + btnW, headerBottom - 2.0f))
	{
		m_minimapNorthUp = !m_minimapNorthUp;
		return true;
	}
	if (PointInRect2D(m_lastMouseScreenX, m_lastMouseScreenY, btnJLeft, headerTop + 2.0f, btnJLeft + btnW, headerBottom - 2.0f))
	{
		ToggleQuestJournalWindow();
		return true;
	}

	float bodyHeight = (m_minimapMode == 2) ? 0.0f : ((m_minimapMode == 1) ? 248.0f : 176.0f);
	float footerTop = headerBottom + bodyHeight;
	float footerBottom = footerTop + 22.0f;

	// Footer zoom [-] and [+] buttons
	if (m_minimapMode != 2)
	{
		float zoomPlusLeft = panelRight - 22.0f;
		float zoomMinusLeft = panelRight - 86.0f;   // room for "4000m"
		if (PointInRect2D(m_lastMouseScreenX, m_lastMouseScreenY, zoomPlusLeft, footerTop + 2.0f, zoomPlusLeft + 18.0f, footerBottom - 2.0f))
		{
			m_minimapZoomRadius = max(40.0f, m_minimapZoomRadius * 0.75f);
			return true;
		}
		if (PointInRect2D(m_lastMouseScreenX, m_lastMouseScreenY, zoomMinusLeft, footerTop + 2.0f, zoomMinusLeft + 18.0f, footerBottom - 2.0f))
		{
			m_minimapZoomRadius = min(4000.0f, m_minimapZoomRadius * 1.33f);
			return true;
		}
	}

	// Quest Tracker header bar buttons
	float qtWidth = 236.0f;
	float qtRight = QuestTrackerRight(screenW);
	float qtLeft = qtRight - qtWidth;
	float qtHeaderTop = footerBottom + 8.0f;
	float qtHeaderBottom = qtHeaderTop + 22.0f;

	float qtToggleLeft = qtRight - 24.0f;
	float qtJournalLeft = qtToggleLeft - 52.0f;
	if (PointInRect2D(m_lastMouseScreenX, m_lastMouseScreenY, qtToggleLeft, qtHeaderTop + 2.0f, qtRight - 4.0f, qtHeaderBottom - 2.0f))
	{
		m_questTrackerVisible = !m_questTrackerVisible;
		return true;
	}
	if (PointInRect2D(m_lastMouseScreenX, m_lastMouseScreenY, qtJournalLeft, qtHeaderTop + 2.0f, qtToggleLeft - 3.0f, qtHeaderBottom - 2.0f))
	{
		ToggleQuestJournalWindow();
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------------//
// Zone map: Maps\<zone>.png (picture), .txt (minX maxX minZ maxZ) and .poi (points of interest)
// in the game folder. The picture and .txt come from EngineX Display > Capture Zone Map.
//------------------------------------------------------------------------------------//
void CMenuManagement::ReleaseZoneMap()
{
	if (m_zoneMapTex)
	{
		m_zoneMapTex->Release();
		m_zoneMapTex = NULL;
	}
	for (int lv = 0; lv < 8; lv++)
		if (m_zoneMapLevelTex[lv])
		{
			m_zoneMapLevelTex[lv]->Release();
			m_zoneMapLevelTex[lv] = NULL;
		}
	m_zoneMapLevels = 0;
	m_zoneMapPlayerLevel = 0;
	m_zoneMapDevice = NULL;
	m_zoneMapHasBounds = false;
	m_zoneMapLoaded = false;
	m_zoneMapPois.clear();
	m_zoneMapPoiQuest.clear();
	m_zoneMapQuestStamp = 0;
}

void CMenuManagement::EnsureZoneMap(LPDIRECT3DDEVICE9 dev, const CString &zoneName)
{
	if (m_zoneMapLoaded && m_zoneMapDevice == dev && zoneName.CompareNoCase(m_zoneMapName) == 0)
		return;

	bool zoneChanged = (zoneName.CompareNoCase(m_zoneMapName) != 0);
	ReleaseZoneMap();
	if (zoneChanged)
	{
		SetWorldMapOpen(false);
		m_worldMapZoom = 1.0f;
	}
	m_zoneMapName = zoneName;
	m_zoneMapLoaded = true;
	m_zoneMapDevice = dev;

	CString dir = m_gStatus->GL_workingDirectory;
	if (!dir.IsEmpty() && dir.Right(1) != "\\" && dir.Right(1) != "/")
		dir += "\\";
	CString base = dir + "Maps\\" + zoneName;

	// bounds of the picture in world units
	FILE *f = NULL;
	CString txtFile = base + ".txt";
	if (fopen_s(&f, txtFile, "r") == 0 && f)
	{
		char line[256] = { 0 };
		if (fgets(line, sizeof(line), f))
		{
			float v[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			char *pos = line;
			int n = 0;
			for (; n < 4; n++)
			{
				char *end = NULL;
				v[n] = (float)strtod(pos, &end);
				if (end == pos)
					break;
				pos = end;
			}
			if (n == 4 && v[1] - v[0] > 1.0f && v[3] - v[2] > 1.0f)
			{
				m_zoneMapMinX = v[0]; m_zoneMapMaxX = v[1];
				m_zoneMapMinZ = v[2]; m_zoneMapMaxZ = v[3];
				m_zoneMapHasBounds = true;
			}
		}
		// optional floors: "levels N" then N lines "lowY highY" (floor 1 = lowest)
		int want = 0, got = 0;
		while (fgets(line, sizeof(line), f))
		{
			if (want == 0)
			{
				if (strncmp(line, "levels", 6) == 0)
				{
					want = atoi(line + 6);
					if (want > 8) want = 8;
					if (want < 2) break;
				}
				continue;
			}
			char *end1 = NULL, *end2 = NULL;
			float lo = (float)strtod(line, &end1);
			if (end1 == line)
				break;
			float hi = (float)strtod(end1, &end2);
			if (end2 == end1)
				break;
			m_zoneMapLevelLo[got] = lo;
			m_zoneMapLevelHi[got] = hi;
			if (++got >= want)
				break;
		}
		if (want >= 2 && got == want && m_zoneMapHasBounds)
			m_zoneMapLevels = got;
		fclose(f);
		f = NULL;
	}

	// floor pictures, all loaded now so changing floors never stalls the game
	bool allFloors = (m_zoneMapLevels >= 2);
	for (int lv = 0; lv < m_zoneMapLevels && dev; lv++)
	{
		CString lvBase;
		lvBase.Format("%s_L%d", (const char*)base, lv + 1);
		const char *lvExts[3] = { ".dds", ".jpg", ".png" };
		for (int e = 0; e < 3 && !m_zoneMapLevelTex[lv]; e++)
		{
			CString picFile = lvBase + lvExts[e];
			if (GetFileAttributesA(picFile) == INVALID_FILE_ATTRIBUTES)
				continue;
			D3DFORMAT fmts[3] = { D3DFMT_DXT1, D3DFMT_X8R8G8B8, D3DFMT_UNKNOWN };
			for (int i = 0; i < 3 && !m_zoneMapLevelTex[lv]; i++)
				if (FAILED(D3DXCreateTextureFromFileExA(dev, picFile, 2048, 2048, D3DX_DEFAULT, 0, fmts[i],
					D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_zoneMapLevelTex[lv])))
					m_zoneMapLevelTex[lv] = NULL;
		}
		if (!m_zoneMapLevelTex[lv])
			allFloors = false;
	}

	// the picture (.dds loads fastest if one is shipped, .png is what EngineX saves)
	if (m_zoneMapHasBounds && dev && !allFloors)
	{
		const char *exts[3] = { ".dds", ".jpg", ".png" };
		for (int e = 0; e < 3 && !m_zoneMapTex; e++)
		{
			CString picFile = base + exts[e];
			if (GetFileAttributesA(picFile) == INVALID_FILE_ATTRIBUTES)
				continue;
			// DXT1 keeps the 4096x4096 picture at ~11MB with mipmaps
			D3DFORMAT fmts[3] = { D3DFMT_DXT1, D3DFMT_X8R8G8B8, D3DFMT_UNKNOWN };
			for (int i = 0; i < 3 && !m_zoneMapTex; i++)
			{
				if (FAILED(D3DXCreateTextureFromFileExA(dev, picFile, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, fmts[i],
					D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_zoneMapTex)))
					m_zoneMapTex = NULL;
			}
		}
	}

	// points of interest
	CString poiFile = base + ".poi";
	if (fopen_s(&f, poiFile, "r") == 0 && f)
	{
		char line[512];
		while (fgets(line, sizeof(line), f))
		{
			char *t = line;
			while (*t == ' ' || *t == '\t')
				t++;
			if (!t[0] || t[0] == '#' || t[1] != '|')
				continue;
			HudMapPoi poi;
			poi.type = (char)toupper((unsigned char)t[0]);
			char *pos = t + 2, *end = NULL;
			poi.x = (float)strtod(pos, &end);
			if (end == pos || *end != '|')
				continue;
			pos = end + 1;
			poi.z = (float)strtod(pos, &end);
			if (end == pos || *end != '|')
				continue;
			pos = end + 1;
			poi.y = 0.0f;
			poi.hasY = false;
			{
				char *yEnd = NULL;
				float yv = (float)strtod(pos, &yEnd);
				if (yEnd != pos && *yEnd == '|')
				{
					poi.y = yv;
					poi.hasY = true;
					pos = yEnd + 1;
				}
			}
			size_t len = strlen(pos);
			while (len && (pos[len - 1] == '\r' || pos[len - 1] == '\n' || pos[len - 1] == ' '))
				pos[--len] = 0;
			poi.name = pos;
			m_zoneMapPois.push_back(poi);
		}
		fclose(f);
	}
	m_zoneMapPoiQuest.assign(m_zoneMapPois.size(), 0);
}

// right edge of the quest tracker: it moves left while the ESC button menu (Inventory, Trade,
// Group...) is open on the right side, so the two never sit on top of each other
float CMenuManagement::QuestTrackerRight(float screenW)
{
	float right = screenW - 12.0f;
	if (m_gStatus && m_gStatus->m_runtimeMenuDatabase)
	{
		CMenuWindow *escMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(686, false));
		if (escMenu && escMenu->m_visible)
			right -= 96.0f;
	}
	return right;
}

int CMenuManagement::ZoneMapLevelOf(float y)
{
	if (m_zoneMapLevels < 2)
		return 0;
	for (int lv = 0; lv < m_zoneMapLevels; lv++)
		if (y >= m_zoneMapLevelLo[lv] && y < m_zoneMapLevelHi[lv])
			return lv;
	return (y < m_zoneMapLevelLo[0]) ? 0 : m_zoneMapLevels - 1;
}

LPDIRECT3DTEXTURE9 CMenuManagement::ZoneMapTexture(int level)
{
	if (m_zoneMapLevels >= 2 && level >= 0 && level < m_zoneMapLevels && m_zoneMapLevelTex[level])
		return m_zoneMapLevelTex[level];
	return m_zoneMapTex;
}

void CMenuManagement::UpdatePoiQuestStates()
{
	DWORD now = GetTickCount();
	if (m_zoneMapQuestStamp && now - m_zoneMapQuestStamp < 500 && m_zoneMapPoiQuest.size() == m_zoneMapPois.size())
		return;
	m_zoneMapQuestStamp = now;
	m_zoneMapPoiQuest.assign(m_zoneMapPois.size(), 0);
	if (!m_gStatus->m_multiplayerObject || !m_gStatus->m_multiplayerObject->m_journalEntries)
		return;
	for (POSITION qPos = m_gStatus->m_multiplayerObject->m_journalEntries->GetHeadPosition(); qPos != NULL;)
	{
		CQuestJournalObj *q = (CQuestJournalObj*)m_gStatus->m_multiplayerObject->m_journalEntries->GetNext(qPos);
		if (!q || q->m_npcName.IsEmpty())
			continue;
		char state = (q->m_status == 0) ? 1 : 2;
		for (size_t i = 0; i < m_zoneMapPois.size(); i++)
		{
			if (m_zoneMapPois[i].type == 'P')
				continue;
			if (HudNameMatches(m_zoneMapPois[i].name, q->m_npcName))
				if (m_zoneMapPoiQuest[i] == 0 || state == 1)
					m_zoneMapPoiQuest[i] = state;
		}
	}
}

bool CMenuManagement::IsHudWorldActive()
{
	if (!m_gStatus || !m_gStatus->m_entObjRuntime || m_gStatus->m_entObjRuntime->IsEmpty())
		return false;
	if (m_gStatus->m_runtimeMenuDatabase)
	{
		CMenuWindow *loginWin = dynamic_cast<CMenuWindow*>(
			m_gStatus->m_runtimeMenuDatabase->GetVisualByID(EA_LOGIN_MENU_ID, false));
		if (loginWin && loginWin->m_visible)
			return false;
	}
	return true;
}

void CMenuManagement::SetWorldMapOpen(bool open)
{
	if (open == m_worldMapOpen)
		return;
	m_worldMapOpen = open;
	m_worldMapDragging = false;
	if (open)
	{
		m_worldMapZoom = 1.0f;
		m_worldMapViewLevel = -1;
		m_gStatus->m_showMenuCursor++;          // mouse cursor on while the map is up
	}
	else if (m_gStatus->m_showMenuCursor > 0)
		m_gStatus->m_showMenuCursor--;
}

void CMenuManagement::ClampWorldMapCenter()
{
	if (m_wmFull <= 0.0f)
		return;
	if (m_worldMapZoom <= 1.0f)
	{
		m_worldMapZoom = 1.0f;
		m_worldMapCenterX = m_wmMidX;
		m_worldMapCenterZ = m_wmMidZ;
		return;
	}
	float slack = m_wmFull * 0.5f - m_wmFull * 0.5f / m_worldMapZoom;
	m_worldMapCenterX = max(m_wmMidX - slack, min(m_wmMidX + slack, m_worldMapCenterX));
	m_worldMapCenterZ = max(m_wmMidZ - slack, min(m_wmMidZ + slack, m_worldMapCenterZ));
}

void CMenuManagement::RenderWorldMap(LPDIRECT3DDEVICE9 dev, CMovementObj *ptrPlayer, float screenW, float screenH, float playerYawRad)
{
	const DWORD colGold      = D3DCOLOR_ARGB(255, 255, 215, 40);
	const DWORD colGoldEdge  = D3DCOLOR_ARGB(255, 120, 80, 0);
	const DWORD colTurnIn    = D3DCOLOR_ARGB(255, 110, 235, 130);
	const DWORD colStore     = D3DCOLOR_ARGB(255, 255, 160, 40);
	const DWORD colStoreEdge = D3DCOLOR_ARGB(255, 130, 70, 0);
	const DWORD colBank      = D3DCOLOR_ARGB(255, 80, 210, 110);
	const DWORD colPortal    = D3DCOLOR_ARGB(255, 195, 85, 255);
	const DWORD colPortalEdge= D3DCOLOR_ARGB(255, 245, 200, 255);
	const DWORD colQuestDot  = D3DCOLOR_ARGB(210, 235, 220, 120);
	const DWORD colText      = D3DCOLOR_ARGB(255, 225, 235, 245);

	float mx = m_lastMouseScreenX, my = m_lastMouseScreenY;
	float px = ptrPlayer->List_lastPosition.x, pz = ptrPlayer->List_lastPosition.z;
	bool floors = (m_zoneMapLevels >= 2);
	if (m_worldMapViewLevel >= m_zoneMapLevels)
		m_worldMapViewLevel = -1;
	int viewLevel = (floors && m_worldMapViewLevel >= 0) ? m_worldMapViewLevel : m_zoneMapPlayerLevel;
	LPDIRECT3DTEXTURE9 viewTex = ZoneMapTexture(viewLevel);

	// dim the game behind the map
	DrawHudRectFill(dev, 0.0f, 0.0f, screenW, screenH, D3DCOLOR_ARGB(165, 0, 0, 0), D3DCOLOR_ARGB(165, 0, 0, 0));

	// layout: square map on the left, quest list on the right
	float listW = 250.0f;
	float S = min(screenH - 110.0f, screenW - 60.0f - listW - 12.0f);
	if (S < 240.0f)
		S = 240.0f;
	float left = floorf((screenW - (S + 12.0f + listW)) * 0.5f);
	if (left < 4.0f)
		left = 4.0f;
	float top = floorf((screenH - S) * 0.5f + 12.0f);
	float right = left + S, bottom = top + S;
	m_wmLeft = left; m_wmTop = top; m_wmSize = S;

	// area of the world being shown
	float bMinX, bMaxX, bMinZ, bMaxZ;
	if (m_zoneMapHasBounds)
	{
		bMinX = m_zoneMapMinX; bMaxX = m_zoneMapMaxX;
		bMinZ = m_zoneMapMinZ; bMaxZ = m_zoneMapMaxZ;
	}
	else
	{
		bMinX = px - 4000.0f; bMaxX = px + 4000.0f;
		bMinZ = pz - 4000.0f; bMaxZ = pz + 4000.0f;
	}
	m_wmFull = max(bMaxX - bMinX, bMaxZ - bMinZ);
	m_wmMidX = (bMinX + bMaxX) * 0.5f;
	m_wmMidZ = (bMinZ + bMaxZ) * 0.5f;
	ClampWorldMapCenter();
	float half = m_wmFull * 0.5f / m_worldMapZoom;
	float viewMinX = m_worldMapCenterX - half;
	float viewMaxZ = m_worldMapCenterZ + half;
	float scale = S / (2.0f * half);
	m_wmViewMinX = viewMinX; m_wmViewMaxZ = viewMaxZ; m_wmScale = scale;

	// ---- title bar
	float titleTop = top - 30.0f, titleBottom = top - 4.0f;
	DrawHudRectFill(dev, left, titleTop, right, titleBottom, D3DCOLOR_ARGB(235, 22, 30, 46), D3DCOLOR_ARGB(235, 12, 17, 28));
	DrawHudRectBorder(dev, left, titleTop, right, titleBottom, D3DCOLOR_ARGB(220, 190, 150, 70));
	m_wmCloseR = right - 4.0f; m_wmCloseL = m_wmCloseR - 22.0f;
	m_wmCloseT = titleTop + 3.0f; m_wmCloseB = titleBottom - 3.0f;
	bool closeHot = PointInRect2D(mx, my, m_wmCloseL, m_wmCloseT, m_wmCloseR, m_wmCloseB);
	DrawHudRectFill(dev, m_wmCloseL, m_wmCloseT, m_wmCloseR, m_wmCloseB,
		closeHot ? D3DCOLOR_ARGB(230, 150, 40, 30) : D3DCOLOR_ARGB(210, 70, 30, 25),
		closeHot ? D3DCOLOR_ARGB(230, 110, 25, 20) : D3DCOLOR_ARGB(210, 45, 18, 15));
	DrawHudRectBorder(dev, m_wmCloseL, m_wmCloseT, m_wmCloseR, m_wmCloseB, D3DCOLOR_ARGB(220, 230, 170, 120));
	CString title;
	if (floors)
		title.Format("%s  -  Floor %d of %d        %d, %d", (const char*)m_zoneMapName, viewLevel + 1, m_zoneMapLevels, (int)px, (int)pz);
	else
		title.Format("%s        %d, %d", (const char*)m_zoneMapName, (int)px, (int)pz);
	DrawHudTextShadowed(m_hudFontTitle, title, (int)left + 8, (int)titleTop + 2, (int)m_wmCloseL - 6, (int)titleBottom - 2,
		DT_LEFT | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 240, 210, 130));
	DrawHudTextShadowed(m_hudFontSmall, "X", (int)m_wmCloseL, (int)m_wmCloseT, (int)m_wmCloseR, (int)m_wmCloseB,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 230, 210));
	DrawHudTextShadowed(m_hudFontSmall, (m_zoneMapLevels >= 2) ? "Wheel / [ ]: zoom     Drag: move     PgUp / PgDn: floor     Space: find me     M / Esc: close" : "Wheel / [ ]: zoom     Drag: move     Space: find me     M / Esc: close",
		(int)left, (int)bottom + 4, (int)right, (int)bottom + 22, DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(230, 190, 200, 215));
	dev->SetTexture(0, NULL);
	dev->SetFVF(HUD_FVF_2D);

	// ---- the map picture
	DrawHudRectFill(dev, left, top, right, bottom, D3DCOLOR_ARGB(255, 16, 34, 48), D3DCOLOR_ARGB(255, 8, 18, 30));
	bool havePicture = (viewTex != NULL && m_zoneMapHasBounds);
	if (havePicture)
	{
		float w = m_zoneMapMaxX - m_zoneMapMinX, h = m_zoneMapMaxZ - m_zoneMapMinZ;
		float u0 = (viewMinX - m_zoneMapMinX) / w, u1 = (viewMinX + 2.0f * half - m_zoneMapMinX) / w;
		float v0 = (m_zoneMapMaxZ - viewMaxZ) / h, v1 = (m_zoneMapMaxZ - (viewMaxZ - 2.0f * half)) / h;
		DWORD white = D3DCOLOR_ARGB(255, 255, 255, 255);
		HudVertexTex quad[4] = {
			{ left,  top,    0.0f, 1.0f, white, u0, v0 },
			{ right, top,    0.0f, 1.0f, white, u1, v0 },
			{ left,  bottom, 0.0f, 1.0f, white, u0, v1 },
			{ right, bottom, 0.0f, 1.0f, white, u1, v1 }
		};
		BeginHudTexture(dev, viewTex);
		dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, quad, sizeof(HudVertexTex));
		EndHudTexture(dev);
	}
	else
	{
		for (int g = 1; g < 8; g++)
		{
			float gx = left + S * g / 8.0f, gy = top + S * g / 8.0f;
			DrawHudLine(dev, gx, top, gx, bottom, D3DCOLOR_ARGB(50, 100, 160, 210));
			DrawHudLine(dev, left, gy, right, gy, D3DCOLOR_ARGB(50, 100, 160, 210));
		}
		DrawHudTextShadowed(m_hudFontTitle, "No map picture for this zone yet",
			(int)left, (int)top + 12, (int)right, (int)top + 40, DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(230, 220, 210, 175));
		dev->SetTexture(0, NULL);
		dev->SetFVF(HUD_FVF_2D);
	}
	DrawHudRectBorder(dev, left, top, right, bottom, D3DCOLOR_ARGB(230, 195, 160, 80));
	DrawHudRectBorder(dev, left - 2.0f, top - 2.0f, right + 2.0f, bottom + 2.0f, D3DCOLOR_ARGB(140, 80, 140, 195));

	// ---- markers
	CString hoverText;
	float hoverBest = 10.0f * 10.0f;
	struct Label { float x, y; CString text; DWORD color; };
	std::vector<Label> labels;
	bool showLabels = (m_worldMapZoom >= 2.4f);

	// quest objects / quest givers not in your journal: small dots
	for (size_t i = 0; i < m_zoneMapPois.size(); i++)
	{
		const HudMapPoi &poi = m_zoneMapPois[i];
		if (poi.type != 'Q' || (i < m_zoneMapPoiQuest.size() && m_zoneMapPoiQuest[i]))
			continue;
		if (floors && poi.hasY && ZoneMapLevelOf(poi.y) != viewLevel)
			continue;
		float sx = left + (poi.x - viewMinX) * scale, sy = top + (viewMaxZ - poi.z) * scale;
		if (sx < left + 3.0f || sx > right - 3.0f || sy < top + 3.0f || sy > bottom - 3.0f)
			continue;
		DrawHudDisc(dev, sx, sy, 2.4f, colQuestDot, D3DCOLOR_ARGB(210, 120, 100, 30), 8);
		float d2 = (sx - mx) * (sx - mx) + (sy - my) * (sy - my);
		if (d2 < hoverBest) { hoverBest = d2; hoverText = poi.name; }
	}

	// merchants, banks, portals
	bool havePoiPortals = false;
	for (size_t i = 0; i < m_zoneMapPois.size(); i++)
	{
		const HudMapPoi &poi = m_zoneMapPois[i];
		if (poi.type == 'P')
			havePoiPortals = true;
		if (poi.type != 'S' && poi.type != 'B' && poi.type != 'P')
			continue;
		if (floors && poi.hasY && ZoneMapLevelOf(poi.y) != viewLevel)
			continue;
		float sx = left + (poi.x - viewMinX) * scale, sy = top + (viewMaxZ - poi.z) * scale;
		if (sx < left + 5.0f || sx > right - 5.0f || sy < top + 5.0f || sy > bottom - 5.0f)
			continue;
		CString tip;
		DWORD labelColor;
		if (poi.type == 'S')
		{
			DrawHudDisc(dev, sx, sy, 5.0f, colStore, colStoreEdge, 12);
			DrawHudCircleRing(dev, sx, sy, 5.0f, D3DCOLOR_ARGB(255, 60, 30, 0), 12);
			tip.Format("Merchant: %s", (const char*)poi.name);
			labelColor = D3DCOLOR_ARGB(255, 255, 200, 120);
		}
		else if (poi.type == 'B')
		{
			DrawHudRectFill(dev, sx - 4.5f, sy - 4.5f, sx + 4.5f, sy + 4.5f, colBank, D3DCOLOR_ARGB(255, 30, 130, 60));
			DrawHudRectBorder(dev, sx - 4.5f, sy - 4.5f, sx + 4.5f, sy + 4.5f, D3DCOLOR_ARGB(255, 10, 50, 20));
			tip.Format("Bank: %s", (const char*)poi.name);
			labelColor = D3DCOLOR_ARGB(255, 150, 240, 170);
		}
		else
		{
			DrawHudDiamond(dev, sx, sy, 6.0f, colPortal, colPortalEdge);
			tip.Format("Portal: %s", (const char*)poi.name);
			labelColor = D3DCOLOR_ARGB(255, 225, 180, 255);
		}
		float d2 = (sx - mx) * (sx - mx) + (sy - my) * (sy - my);
		if (d2 < hoverBest) { hoverBest = d2; hoverText = tip; }
		if (showLabels)
		{
			Label l = { sx + 8.0f, sy, poi.name, labelColor };
			labels.push_back(l);
		}
	}
	// zones without a .poi file still get their portals from the live portal list
	if (!havePoiPortals && m_gStatus->m_portalDatabase)
	{
		for (POSITION pPos = m_gStatus->m_portalDatabase->GetHeadPosition(); pPos != NULL;)
		{
			CPortalSysObj *portalObj = (CPortalSysObj*)m_gStatus->m_portalDatabase->GetNext(pPos);
			if (!portalObj)
				continue;
			if (floors && ZoneMapLevelOf(portalObj->m_wldPosition.y) != viewLevel)
				continue;
			float sx = left + (portalObj->m_wldPosition.x - viewMinX) * scale;
			float sy = top + (viewMaxZ - portalObj->m_wldPosition.z) * scale;
			if (sx < left + 5.0f || sx > right - 5.0f || sy < top + 5.0f || sy > bottom - 5.0f)
				continue;
			DrawHudDiamond(dev, sx, sy, 6.0f, colPortal, colPortalEdge);
			float d2 = (sx - mx) * (sx - mx) + (sy - my) * (sy - my);
			if (d2 < hoverBest)
			{
				hoverBest = d2;
				hoverText.Format("Portal: %s", portalObj->m_portalName.IsEmpty() ? "Zone Exit" : (const char*)portalObj->m_portalName);
			}
		}
	}

	// live entities near you (players, NPCs, monsters)
	POSITION posEnt = m_gStatus->m_entObjRuntime->GetHeadPosition();
	if (posEnt)
		m_gStatus->m_entObjRuntime->GetNext(posEnt); // skip local player at head
	while (posEnt)
	{
		CMovementObj *ptrEnt = (CMovementObj*)m_gStatus->m_entObjRuntime->GetNext(posEnt);
		if (!ptrEnt)
			continue;
		if (floors && ZoneMapLevelOf(ptrEnt->List_lastPosition.y) != viewLevel)
			continue;
		float sx = left + (ptrEnt->List_lastPosition.x - viewMinX) * scale;
		float sy = top + (viewMaxZ - ptrEnt->List_lastPosition.z) * scale;
		if (sx < left + 3.0f || sx > right - 3.0f || sy < top + 3.0f || sy > bottom - 3.0f)
			continue;
		if (ptrEnt->List_controlType == 0)
			DrawHudDisc(dev, sx, sy, 3.8f, D3DCOLOR_ARGB(255, 70, 235, 255), D3DCOLOR_ARGB(255, 20, 140, 210), 10);
		else if (ptrEnt->List_controlType == 35)
			DrawHudDisc(dev, sx, sy, 3.2f, D3DCOLOR_ARGB(255, 255, 205, 65), D3DCOLOR_ARGB(255, 180, 120, 20), 10);
		else if (ptrEnt->List_energy > 0)
			DrawHudDisc(dev, sx, sy, 2.8f, D3DCOLOR_ARGB(250, 245, 65, 65), D3DCOLOR_ARGB(255, 130, 15, 15), 8);
		else
			continue;
		float d2 = (sx - mx) * (sx - mx) + (sy - my) * (sy - my);
		if (d2 < hoverBest)
		{
			hoverBest = d2;
			hoverText = ptrEnt->List_name.IsEmpty() ? CString("Unknown") : ptrEnt->List_name;
		}
	}

	// quest NPCs from your journal (on top): gold = active quest, green = other journal NPC
	DWORD pulse = (DWORD)(GetTickCount() % 1200);
	float pulseR = 7.0f + 5.0f * (float)pulse / 1200.0f;
	DWORD pulseA = (DWORD)(200 - 200 * pulse / 1200);
	for (size_t i = 0; i < m_zoneMapPois.size() && i < m_zoneMapPoiQuest.size(); i++)
	{
		char qs = m_zoneMapPoiQuest[i];
		if (!qs)
			continue;
		const HudMapPoi &poi = m_zoneMapPois[i];
		if (floors && poi.hasY && ZoneMapLevelOf(poi.y) != viewLevel)
			continue;
		float sx = left + (poi.x - viewMinX) * scale, sy = top + (viewMaxZ - poi.z) * scale;
		if (sx < left + 6.0f || sx > right - 6.0f || sy < top + 6.0f || sy > bottom - 6.0f)
			continue;
		if (qs == 1)
			DrawHudCircleRing(dev, sx, sy, pulseR, D3DCOLOR_ARGB(pulseA, 255, 215, 40), 18);
		DrawHudDiamond(dev, sx, sy, 7.0f, qs == 1 ? colGold : colTurnIn, colGoldEdge);
		float d2 = (sx - mx) * (sx - mx) + (sy - my) * (sy - my);
		if (d2 < 12.0f * 12.0f && d2 <= hoverBest + 30.0f)
		{
			hoverBest = d2;
			hoverText.Format("[Quest] %s", (const char*)poi.name);
		}
		Label l = { sx - 3.0f, sy, qs == 1 ? CString("!") : CString("?"), D3DCOLOR_ARGB(255, 60, 30, 0) };
		labels.push_back(l);
	}

	// you
	{
		float sx = left + (px - viewMinX) * scale, sy = top + (viewMaxZ - pz) * scale;
		sx = max(left + 6.0f, min(right - 6.0f, sx));
		sy = max(top + 6.0f, min(bottom - 6.0f, sy));
		float sinA = sinf(playerYawRad), cosA = cosf(playerYawRad);
		float k = 1.6f;
		float ax[4] = { 0.0f, 4.8f * k, 0.0f, -4.8f * k }, ay[4] = { -6.5f * k, 5.0f * k, 2.2f * k, 5.0f * k };
		HudVertex2D v[4];
		DWORD cols[4] = { D3DCOLOR_ARGB(255, 255, 255, 255), D3DCOLOR_ARGB(255, 80, 220, 255), D3DCOLOR_ARGB(255, 180, 245, 255), D3DCOLOR_ARGB(255, 80, 220, 255) };
		for (int i = 0; i < 4; i++)
		{
			v[i].x = sx + (ax[i] * cosA - ay[i] * sinA);
			v[i].y = sy + (ax[i] * sinA + ay[i] * cosA);
			v[i].z = 0.0f; v[i].rhw = 1.0f; v[i].color = cols[i];
		}
		DrawHudDisc(dev, sx, sy, 13.0f, D3DCOLOR_ARGB(90, 0, 0, 0), D3DCOLOR_ARGB(0, 0, 0, 0), 20);
		HudVertex2D tris[6] = { v[0], v[1], v[2], v[0], v[2], v[3] };
		dev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, tris, sizeof(HudVertex2D));
		float d2 = (sx - mx) * (sx - mx) + (sy - my) * (sy - my);
		if (d2 < hoverBest) { hoverBest = d2; hoverText = "You"; }
	}

	// labels (text last so the vertex state is only rebound once)
	for (size_t i = 0; i < labels.size(); i++)
	{
		const Label &l = labels[i];
		if (l.text == "!" || l.text == "?")
			DrawHudTextShadowed(m_hudFontSmall, l.text, (int)l.x - 6, (int)l.y - 9, (int)l.x + 12, (int)l.y + 9,
				DT_CENTER | DT_VCENTER | DT_SINGLELINE, l.color);
		else
			DrawHudTextShadowed(m_hudFontSmall, l.text, (int)l.x, (int)l.y - 9, (int)min(right - 2.0f, l.x + 220.0f), (int)l.y + 9,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE, l.color);
	}
	dev->SetTexture(0, NULL);
	dev->SetFVF(HUD_FVF_2D);

	// ---- floor buttons (top-left of the map), highest floor at the top
	m_wmFloorL = m_wmFloorR = 0.0f;
	if (floors)
	{
		float bl = left + 6.0f, br = bl + 92.0f;
		m_wmFloorL = bl; m_wmFloorR = br;
		int row = 0;
		for (int lv = m_zoneMapLevels - 1; lv >= 0; lv--, row++)
		{
			float bt = top + 6.0f + row * 23.0f, bb = bt + 20.0f;
			m_wmFloorT[lv] = bt; m_wmFloorB[lv] = bb;
			bool sel = (lv == viewLevel), hot = PointInRect2D(mx, my, bl, bt, br, bb);
			DrawHudRectFill(dev, bl, bt, br, bb,
				sel ? D3DCOLOR_ARGB(235, 120, 90, 25) : (hot ? D3DCOLOR_ARGB(225, 45, 60, 85) : D3DCOLOR_ARGB(205, 20, 28, 42)),
				sel ? D3DCOLOR_ARGB(235, 80, 58, 12) : (hot ? D3DCOLOR_ARGB(225, 30, 42, 60) : D3DCOLOR_ARGB(205, 12, 17, 28)));
			DrawHudRectBorder(dev, bl, bt, br, bb, sel ? D3DCOLOR_ARGB(255, 245, 205, 100) : D3DCOLOR_ARGB(200, 120, 150, 190));
		}
		row = 0;
		for (int lv = m_zoneMapLevels - 1; lv >= 0; lv--, row++)
		{
			CString ft;
			if (lv == m_zoneMapPlayerLevel)
				ft.Format("Floor %d  (you)", lv + 1);
			else
				ft.Format("Floor %d", lv + 1);
			DrawHudTextShadowed(m_hudFontSmall, ft, (int)m_wmFloorL + 6, (int)m_wmFloorT[lv], (int)m_wmFloorR - 4, (int)m_wmFloorB[lv],
				DT_LEFT | DT_VCENTER | DT_SINGLELINE, (lv == viewLevel) ? D3DCOLOR_ARGB(255, 255, 235, 160) : D3DCOLOR_ARGB(255, 210, 225, 240));
		}
		dev->SetTexture(0, NULL);
		dev->SetFVF(HUD_FVF_2D);
	}

	// ---- quest list + legend on the right
	float listL = right + 12.0f, listR = listL + listW;
	m_wmListL = listL; m_wmListR = listR;
	DrawHudRectFill(dev, listL, titleTop, listR, bottom, D3DCOLOR_ARGB(225, 16, 22, 34), D3DCOLOR_ARGB(225, 9, 13, 22));
	DrawHudRectBorder(dev, listL, titleTop, listR, bottom, D3DCOLOR_ARGB(210, 180, 145, 70));
	DrawHudLine(dev, listL + 1.0f, titleTop + 1.0f, listR - 1.0f, titleTop + 1.0f, D3DCOLOR_ARGB(240, 235, 195, 95));
	DrawHudTextShadowed(m_hudFontTitle, "Quests", (int)listL + 8, (int)titleTop + 2, (int)listR - 8, (int)titleBottom - 2,
		DT_LEFT | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 240, 210, 130));
	dev->SetTexture(0, NULL);
	dev->SetFVF(HUD_FVF_2D);

	float legendH = 7 * 17.0f + 12.0f;
	float rowsTop = titleBottom + 4.0f, rowsBottom = bottom - legendH - 6.0f;
	const float rowH = 36.0f;
	m_wmQuestRows.clear();
	int shown = 0;
	if (m_gStatus->m_multiplayerObject && m_gStatus->m_multiplayerObject->m_journalEntries)
	{
		for (POSITION qPos = m_gStatus->m_multiplayerObject->m_journalEntries->GetHeadPosition(); qPos != NULL;)
		{
			CQuestJournalObj *q = (CQuestJournalObj*)m_gStatus->m_multiplayerObject->m_journalEntries->GetNext(qPos);
			if (!q)
				continue;
			float rt = rowsTop + shown * rowH, rb = rt + rowH - 2.0f;
			if (rb > rowsBottom)
				break;
			shown++;
			int poiIndex = -1;
			for (size_t i = 0; i < m_zoneMapPois.size(); i++)
				if (m_zoneMapPois[i].type != 'P' && HudNameMatches(m_zoneMapPois[i].name, q->m_npcName))
				{
					poiIndex = (int)i;
					break;
				}
			HudMapQuestRow row = { rt, rb, poiIndex };
			m_wmQuestRows.push_back(row);
			bool hot = PointInRect2D(mx, my, listL + 3.0f, rt, listR - 3.0f, rb);
			DrawHudRectFill(dev, listL + 3.0f, rt, listR - 3.0f, rb,
				hot ? D3DCOLOR_ARGB(200, 55, 60, 30) : D3DCOLOR_ARGB(150, 26, 34, 50),
				hot ? D3DCOLOR_ARGB(200, 40, 42, 20) : D3DCOLOR_ARGB(150, 18, 24, 36));
			DrawHudDiamond(dev, listL + 12.0f, rt + 10.0f, 5.0f, (q->m_status == 0) ? colGold : colTurnIn, colGoldEdge);
		}
	}
	// row text
	{
		int idx = 0;
		if (m_gStatus->m_multiplayerObject && m_gStatus->m_multiplayerObject->m_journalEntries)
		{
			for (POSITION qPos = m_gStatus->m_multiplayerObject->m_journalEntries->GetHeadPosition(); qPos != NULL && idx < (int)m_wmQuestRows.size();)
			{
				CQuestJournalObj *q = (CQuestJournalObj*)m_gStatus->m_multiplayerObject->m_journalEntries->GetNext(qPos);
				if (!q)
					continue;
				const HudMapQuestRow &row = m_wmQuestRows[idx++];
				CString who = q->m_npcName.IsEmpty() ? CString("Quest") : q->m_npcName;
				CString where;
				if (row.poi >= 0 && floors && m_zoneMapPois[row.poi].hasY && ZoneMapLevelOf(m_zoneMapPois[row.poi].y) != m_zoneMapPlayerLevel)
					where.Format("Floor %d", ZoneMapLevelOf(m_zoneMapPois[row.poi].y) + 1);
				else if (row.poi >= 0)
				{
					float dx = m_zoneMapPois[row.poi].x - px, dz = m_zoneMapPois[row.poi].z - pz;
					where.Format("%dm", (int)sqrtf(dx * dx + dz * dz));
				}
				else
					where = "other zone";
				DrawHudTextShadowed(m_hudFontSmall, who, (int)listL + 22, (int)row.top + 1, (int)listR - 64, (int)row.top + 18,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE, (q->m_status == 0) ? D3DCOLOR_ARGB(255, 255, 220, 110) : D3DCOLOR_ARGB(255, 150, 235, 160));
				DrawHudTextShadowed(m_hudFontSmall, where, (int)listR - 64, (int)row.top + 1, (int)listR - 8, (int)row.top + 18,
					DT_RIGHT | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(220, 170, 185, 205));
				DrawHudTextShadowed(m_hudFontSmall, q->m_questDescription, (int)listL + 22, (int)row.top + 17, (int)listR - 8, (int)row.bottom,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(220, 175, 185, 200));
			}
		}
		if (m_wmQuestRows.empty())
			DrawHudTextShadowed(m_hudFontSmall, "No quests in your journal", (int)listL + 10, (int)rowsTop + 4, (int)listR - 8, (int)rowsTop + 24,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(220, 175, 185, 200));
		dev->SetTexture(0, NULL);
		dev->SetFVF(HUD_FVF_2D);
	}

	// legend
	float lgTop = bottom - legendH;
	DrawHudLine(dev, listL + 6.0f, lgTop - 3.0f, listR - 6.0f, lgTop - 3.0f, D3DCOLOR_ARGB(150, 180, 145, 70));
	const char *legendText[7] = { "You", "Quest (active)", "Quest (other)", "Merchant", "Bank", "Portal", "Player / Monster" };
	for (int i = 0; i < 7; i++)
	{
		float cy = lgTop + 8.0f + i * 17.0f, cx = listL + 16.0f;
		switch (i)
		{
		case 0: DrawHudDisc(dev, cx, cy, 5.0f, D3DCOLOR_ARGB(255, 180, 245, 255), D3DCOLOR_ARGB(255, 80, 220, 255), 10); break;
		case 1: DrawHudDiamond(dev, cx, cy, 6.0f, colGold, colGoldEdge); break;
		case 2: DrawHudDiamond(dev, cx, cy, 6.0f, colTurnIn, colGoldEdge); break;
		case 3: DrawHudDisc(dev, cx, cy, 5.0f, colStore, colStoreEdge, 12); break;
		case 4: DrawHudRectFill(dev, cx - 4.5f, cy - 4.5f, cx + 4.5f, cy + 4.5f, colBank, D3DCOLOR_ARGB(255, 30, 130, 60)); break;
		case 5: DrawHudDiamond(dev, cx, cy, 6.0f, colPortal, colPortalEdge); break;
		case 6:
			DrawHudDisc(dev, cx - 4.0f, cy, 3.5f, D3DCOLOR_ARGB(255, 70, 235, 255), D3DCOLOR_ARGB(255, 20, 140, 210), 10);
			DrawHudDisc(dev, cx + 5.0f, cy, 3.0f, D3DCOLOR_ARGB(250, 245, 65, 65), D3DCOLOR_ARGB(255, 130, 15, 15), 8);
			break;
		}
	}
	for (int i = 0; i < 7; i++)
	{
		float cy = lgTop + 8.0f + i * 17.0f;
		DrawHudTextShadowed(m_hudFontSmall, legendText[i], (int)listL + 30, (int)cy - 8, (int)listR - 8, (int)cy + 8,
			DT_LEFT | DT_VCENTER | DT_SINGLELINE, colText);
	}
	dev->SetTexture(0, NULL);
	dev->SetFVF(HUD_FVF_2D);

	// hover tooltip
	if (!hoverText.IsEmpty())
	{
		float tipW = min(320.0f, (float)(hoverText.GetLength() * 7 + 16));
		float tipLeft = max(8.0f, min(mx + 14.0f, screenW - tipW - 8.0f));
		float tipTop = max(8.0f, min(my - 26.0f, screenH - 30.0f));
		DrawHudRectFill(dev, tipLeft, tipTop, tipLeft + tipW, tipTop + 22.0f,
			D3DCOLOR_ARGB(240, 16, 22, 34), D3DCOLOR_ARGB(240, 10, 14, 22));
		DrawHudRectBorder(dev, tipLeft, tipTop, tipLeft + tipW, tipTop + 22.0f, D3DCOLOR_ARGB(230, 235, 195, 90));
		DrawHudTextShadowed(m_hudFontSmall, hoverText, (int)tipLeft + 6, (int)tipTop + 2, (int)(tipLeft + tipW - 6), (int)(tipTop + 20.0f),
			DT_LEFT | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 240, 185));
		dev->SetTexture(0, NULL);
		dev->SetFVF(HUD_FVF_2D);
	}
}

void CMenuManagement::RenderModernHudOverlays(long currentTime)
{
	if (!m_gStatus || !m_gStatus->m_3dDevice || !m_gStatus->m_dxRenderer)
		return;

	// Do not draw in-world HUD while the login menu is open or when no player entity exists
	if (m_gStatus->m_runtimeMenuDatabase)
	{
		CMenuWindow *loginWin = dynamic_cast<CMenuWindow*>(
			m_gStatus->m_runtimeMenuDatabase->GetVisualByID(EA_LOGIN_MENU_ID, false));
		if (loginWin && loginWin->m_visible)
		{
			SetWorldMapOpen(false);
			return;
		}
	}

	if (!m_gStatus->m_entObjRuntime || m_gStatus->m_entObjRuntime->IsEmpty())
	{
		m_questJournalSynced = false;
		SetWorldMapOpen(false);
		return;
	}

	CMovementObj *ptrPlayer = (CMovementObj*)m_gStatus->m_entObjRuntime->GetHead();
	if (!ptrPlayer || !ptrPlayer->List_baseFrame)
	{
		SetWorldMapOpen(false);
		return;
	}

	// Automatically request a silent quest journal sync once upon entering the world
	if (!m_questJournalSynced &&
		m_gStatus->m_multiplayerObject &&
		m_gStatus->m_multiplayerObject->GetEnabledClient_safe() &&
		m_gStatus->m_netGameLogicObject)
	{
		m_questJournalSynced = true;
		m_gStatus->m_netGameLogicObject->SendAttribMsg(
			DPNSEND_GUARANTEED,
			DPID_SERVERPLAYER,
			45,
			ID_QUEST_JOURNAL_MAIN_LIST,
			ptrPlayer->List_networkDefinedID,
			0,
			1, // uniShort3 = 1 -> silent sync (no chat spam if quest log is empty)
			NULL);
	}

	// keep the quest tracker's item counts current: while an active quest needs items, ask the
	// server what you carry every 3 seconds (the answer is kept even with the inventory closed)
	if (m_gStatus->m_multiplayerObject &&
		m_gStatus->m_multiplayerObject->GetEnabledClient_safe() &&
		m_gStatus->m_netGameLogicObject &&
		m_gStatus->m_multiplayerObject->m_journalEntries)
	{
		bool needItems = false;
		for (POSITION qPos = m_gStatus->m_multiplayerObject->m_journalEntries->GetHeadPosition(); qPos != NULL && !needItems;)
		{
			CQuestJournalObj *q = (CQuestJournalObj*)m_gStatus->m_multiplayerObject->m_journalEntries->GetNext(qPos);
			if (q && q->m_status == 0 && q->m_requiredItems && q->m_requiredItems->GetCount() > 0)
				needItems = true;
		}
		DWORD nowInv = GetTickCount();
		if (needItems && (m_hudInvRequestStamp == 0 || nowInv - m_hudInvRequestStamp > 3000))
		{
			m_hudInvRequestStamp = nowInv;
			m_gStatus->m_netGameLogicObject->SendAttribMsg(
				DPNSEND_GUARANTEED,
				DPID_SERVERPLAYER,
				19,                     // request character inventory
				HUD_QUEST_INV_REQ,      // answer goes to the tracker, not a menu
				ptrPlayer->List_networkDefinedID,
				0,
				3,                      // no skills / spells
				NULL);
		}
	}

	LPDIRECT3DDEVICE9 dev = m_gStatus->m_3dDevice;
	EnsureHudFonts(dev);

	// Capture current D3D9 state so we leave the pipeline untouched for the cursor & next frame
	IDirect3DStateBlock9 *pStateBlock = NULL;
	dev->CreateStateBlock(D3DSBT_ALL, &pStateBlock);

	dev->SetVertexShader(NULL);
	dev->SetPixelShader(NULL);
	dev->SetTexture(0, NULL);
	dev->SetTexture(1, NULL);
	dev->SetFVF(HUD_FVF_2D);
	dev->SetRenderState(D3DRS_ZENABLE, FALSE);
	dev->SetRenderState(D3DRS_LIGHTING, FALSE);
	dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	dev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

	float screenW = (float)m_gStatus->m_dxRenderer->m_currentWidth;
	float screenH = (float)m_gStatus->m_dxRenderer->m_currentHeight;

	// =========================================================================
	// 1. INTERACTIVE MINIMAP / RADAR WIDGET (Top-Right Corner)
	// =========================================================================
	float panelW = (m_minimapMode == 1) ? 260.0f : 196.0f;
	float panelRight = screenW - 12.0f;
	float panelLeft = panelRight - panelW;
	float headerTop = 12.0f;
	float headerBottom = headerTop + 22.0f;

	// Clean zone name
	CString zoneName = m_gStatus->m_currentWorkingFile;
	if (zoneName.IsEmpty())
		zoneName = "Endless Ages";
	else
	{
		int slashIdx = max(zoneName.ReverseFind('\\'), zoneName.ReverseFind('/'));
		if (slashIdx >= 0)
			zoneName = zoneName.Mid(slashIdx + 1);
		int dotIdx = zoneName.ReverseFind('.');
		if (dotIdx > 0)
			zoneName = zoneName.Left(dotIdx);
	}
	EnsureZoneMap(dev, zoneName);
	UpdatePoiQuestStates();
	m_zoneMapPlayerLevel = ZoneMapLevelOf(ptrPlayer->List_lastPosition.y);
	if (m_zoneMapLevels >= 2)
	{
		CString floorTxt;
		floorTxt.Format(" - Floor %d", m_zoneMapPlayerLevel + 1);
		zoneName += floorTxt;
	}

	// Header background + gold top trim
	DrawHudRectFill(dev, panelLeft, headerTop, panelRight, headerBottom,
		D3DCOLOR_ARGB(220, 18, 26, 38), D3DCOLOR_ARGB(220, 10, 15, 24));
	DrawHudRectBorder(dev, panelLeft, headerTop, panelRight, headerBottom, D3DCOLOR_ARGB(210, 180, 145, 70));
	DrawHudLine(dev, panelLeft + 1.0f, headerTop + 1.0f, panelRight - 1.0f, headerTop + 1.0f, D3DCOLOR_ARGB(240, 235, 195, 95));

	// Header buttons [M] [N] [J]
	float btnW = 20.0f;
	float btnJLeft = panelRight - 4.0f - btnW;
	float btnNLeft = btnJLeft - 3.0f - btnW;
	float btnMLeft = btnNLeft - 3.0f - btnW;

	DrawHudRectFill(dev, btnMLeft, headerTop + 3.0f, btnMLeft + btnW, headerBottom - 3.0f,
		D3DCOLOR_ARGB(200, 35, 50, 72), D3DCOLOR_ARGB(200, 20, 30, 45));
	DrawHudRectBorder(dev, btnMLeft, headerTop + 3.0f, btnMLeft + btnW, headerBottom - 3.0f, D3DCOLOR_ARGB(180, 120, 160, 210));

	DrawHudRectFill(dev, btnNLeft, headerTop + 3.0f, btnNLeft + btnW, headerBottom - 3.0f,
		m_minimapNorthUp ? D3DCOLOR_ARGB(220, 70, 55, 20) : D3DCOLOR_ARGB(200, 35, 50, 72),
		m_minimapNorthUp ? D3DCOLOR_ARGB(220, 45, 35, 10) : D3DCOLOR_ARGB(200, 20, 30, 45));
	DrawHudRectBorder(dev, btnNLeft, headerTop + 3.0f, btnNLeft + btnW, headerBottom - 3.0f,
		m_minimapNorthUp ? D3DCOLOR_ARGB(230, 240, 195, 80) : D3DCOLOR_ARGB(180, 120, 160, 210));

	DrawHudRectFill(dev, btnJLeft, headerTop + 3.0f, btnJLeft + btnW, headerBottom - 3.0f,
		D3DCOLOR_ARGB(200, 55, 42, 18), D3DCOLOR_ARGB(200, 35, 25, 10));
	DrawHudRectBorder(dev, btnJLeft, headerTop + 3.0f, btnJLeft + btnW, headerBottom - 3.0f, D3DCOLOR_ARGB(210, 220, 175, 75));

	DrawHudTextShadowed(m_hudFontTitle, zoneName,
		(int)panelLeft + 6, (int)headerTop + 3, (int)btnMLeft - 4, (int)headerBottom - 2,
		DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, D3DCOLOR_ARGB(255, 240, 210, 130));
	DrawHudTextShadowed(m_hudFontSmall, "M",
		(int)btnMLeft, (int)headerTop + 3, (int)(btnMLeft + btnW), (int)headerBottom - 3,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 190, 225, 255));
	DrawHudTextShadowed(m_hudFontSmall, "N",
		(int)btnNLeft, (int)headerTop + 3, (int)(btnNLeft + btnW), (int)headerBottom - 3,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE,
		m_minimapNorthUp ? D3DCOLOR_ARGB(255, 255, 215, 90) : D3DCOLOR_ARGB(255, 190, 225, 255));
	DrawHudTextShadowed(m_hudFontSmall, "J",
		(int)btnJLeft, (int)headerTop + 3, (int)(btnJLeft + btnW), (int)headerBottom - 3,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 215, 100));

	// Re-bind untextured FVF after ID3DXFont::DrawTextA
	dev->SetTexture(0, NULL);
	dev->SetFVF(HUD_FVF_2D);

	// Player orientation & heading
	// facing worked out the same way as the game's own compass (GetCompasDir): the model faces its local -Z
	D3DMATRIX orientMat;
	ptrPlayer->List_baseFrame->GetTransform(NULL, &orientMat);
	D3DVECTOR facing = { 0.0f, 0.0f, -1.0f };
	facing = m_gStatus->m_cabLib.UpdatePolygonVertexRotationOnly(orientMat.m, facing);
	float playerYawRad = atan2f(facing.x, facing.z); // 0 = North (+Z), +pi/2 = East (+X)
	float radarRotRad = m_minimapNorthUp ? 0.0f : playerYawRad;

	float bodyHeight = (m_minimapMode == 2) ? 0.0f : ((m_minimapMode == 1) ? 248.0f : 176.0f);
	float bodyTop = headerBottom;
	float bodyBottom = bodyTop + bodyHeight;
	CString hoveredBlipText;

	if (m_minimapMode != 2)
	{
		// Radar backdrop panel
		DrawHudRectFill(dev, panelLeft, bodyTop, panelRight, bodyBottom,
			D3DCOLOR_ARGB(175, 10, 16, 26), D3DCOLOR_ARGB(195, 6, 10, 18));
		DrawHudRectBorder(dev, panelLeft, bodyTop, panelRight, bodyBottom, D3DCOLOR_ARGB(180, 120, 100, 55));

		float mapCx = (panelLeft + panelRight) * 0.5f;
		float mapCy = (bodyTop + bodyBottom) * 0.5f;
		float mapRadius = min(panelW, bodyHeight) * 0.5f - 10.0f;

		// Radar circular disc & range rings
		DrawHudDisc(dev, mapCx, mapCy, mapRadius,
			D3DCOLOR_ARGB(215, 16, 28, 44), D3DCOLOR_ARGB(235, 8, 14, 24), 36);

		float scalePx = (mapRadius - 6.0f) / max(10.0f, m_minimapZoomRadius);   // same scale as the blips
		float plX = ptrPlayer->List_lastPosition.x, plZ = ptrPlayer->List_lastPosition.z;
		float rotC = cosf(radarRotRad), rotS = sinf(radarRotRad);
		bool zoneMapShown = false;
		LPDIRECT3DTEXTURE9 mmTex = ZoneMapTexture(m_zoneMapPlayerLevel);
		if (mmTex && m_zoneMapHasBounds)
		{// the zone picture under you (your floor), turned with the radar
			const int SEG = 48;
			HudVertexTex fan[SEG + 2];
			float r = mapRadius - 1.0f;
			float invW = 1.0f / (m_zoneMapMaxX - m_zoneMapMinX), invH = 1.0f / (m_zoneMapMaxZ - m_zoneMapMinZ);
			for (int i = 0; i < SEG + 2; i++)
			{
				float ox = 0.0f, oy = 0.0f;
				if (i > 0)
				{
					float a = (float)(i - 1) / (float)SEG * 6.2831853f;
					ox = cosf(a) * r;
					oy = sinf(a) * r;
				}
				// screen offset -> radar space -> world (inverse of the blip transform)
				float rx = ox / scalePx, rz = -oy / scalePx;
				float wx = plX + rx * rotC + rz * rotS;
				float wz = plZ - rx * rotS + rz * rotC;
				fan[i].x = mapCx + ox; fan[i].y = mapCy + oy; fan[i].z = 0.0f; fan[i].rhw = 1.0f;
				fan[i].color = D3DCOLOR_ARGB(255, 255, 255, 255);
				fan[i].u = (wx - m_zoneMapMinX) * invW;
				fan[i].v = (m_zoneMapMaxZ - wz) * invH;
			}
			BeginHudTexture(dev, mmTex);
			dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, SEG, fan, sizeof(HudVertexTex));
			EndHudTexture(dev);
			zoneMapShown = true;
		}
		if (!zoneMapShown)
		{
			DrawHudCircleRing(dev, mapCx, mapCy, mapRadius * 0.33f, D3DCOLOR_ARGB(65, 90, 150, 200), 28);
			DrawHudCircleRing(dev, mapCx, mapCy, mapRadius * 0.66f, D3DCOLOR_ARGB(75, 90, 150, 200), 32);

			// Subtle radar crosshairs
			DrawHudLine(dev, mapCx - mapRadius, mapCy, mapCx + mapRadius, mapCy, D3DCOLOR_ARGB(55, 100, 160, 210));
			DrawHudLine(dev, mapCx, mapCy - mapRadius, mapCx, mapCy + mapRadius, D3DCOLOR_ARGB(55, 100, 160, 210));
		}

		// Merchants, banks and the NPCs of your journal quests (from Maps\<zone>.poi).
		// Quest NPCs further away than the radar range stay pinned to the edge, pointing the way.
		for (size_t i = 0; i < m_zoneMapPois.size(); i++)
		{
			const HudMapPoi &poi = m_zoneMapPois[i];
			char qs = (i < m_zoneMapPoiQuest.size()) ? m_zoneMapPoiQuest[i] : 0;
			if (!qs && poi.type != 'S' && poi.type != 'B')
				continue;
			bool otherFloor = (m_zoneMapLevels >= 2 && poi.hasY && ZoneMapLevelOf(poi.y) != m_zoneMapPlayerLevel);
			if (otherFloor && !qs)
				continue;
			float dx = poi.x - plX, dz = poi.z - plZ;
			float dist = sqrtf(dx * dx + dz * dz);
			float rx = dx * rotC - dz * rotS;
			float rz = dx * rotS + dz * rotC;
			float bx = mapCx + rx * scalePx, by = mapCy - rz * scalePx;
			float off = sqrtf((bx - mapCx) * (bx - mapCx) + (by - mapCy) * (by - mapCy));
			float rimR = mapRadius - 6.0f;
			bool onRim = false;
			if (off > rimR)
			{
				if (!qs)
					continue;
				bx = mapCx + (bx - mapCx) * rimR / off;
				by = mapCy + (by - mapCy) * rimR / off;
				onRim = true;
			}
			if (qs)
				DrawHudDiamond(dev, bx, by, onRim ? 4.0f : 5.5f,
					qs == 1 ? D3DCOLOR_ARGB(otherFloor ? 120 : 255, 255, 215, 40) : D3DCOLOR_ARGB(otherFloor ? 110 : 235, 110, 235, 130),
					D3DCOLOR_ARGB(otherFloor ? 120 : 255, 120, 80, 0));
			else if (poi.type == 'S')
				DrawHudDisc(dev, bx, by, 3.6f, D3DCOLOR_ARGB(255, 255, 160, 40), D3DCOLOR_ARGB(255, 130, 70, 0), 10);
			else
			{
				DrawHudRectFill(dev, bx - 3.2f, by - 3.2f, bx + 3.2f, by + 3.2f, D3DCOLOR_ARGB(255, 80, 210, 110), D3DCOLOR_ARGB(255, 30, 130, 60));
				DrawHudRectBorder(dev, bx - 3.2f, by - 3.2f, bx + 3.2f, by + 3.2f, D3DCOLOR_ARGB(255, 10, 50, 20));
			}
			if (fabsf(m_lastMouseScreenX - bx) < 6.5f && fabsf(m_lastMouseScreenY - by) < 6.5f)
			{
				const char *kind = qs ? "[Quest] " : (poi.type == 'S' ? "Merchant: " : "Bank: ");
				if (otherFloor)
					hoveredBlipText.Format("%s%s (Floor %d)", kind, (const char*)poi.name, ZoneMapLevelOf(poi.y) + 1);
				else
					hoveredBlipText.Format("%s%s (%dm)", kind, (const char*)poi.name, (int)dist);
			}
		}

		// Player View Cone (FOV wedge)
		float fovCenterAngle = m_minimapNorthUp ? (playerYawRad - 1.5707963f) : -1.5707963f;
		float fovHalfWidth = 0.45f;
		HudVertex2D fovFan[10];
		fovFan[0].x = mapCx;
		fovFan[0].y = mapCy;
		fovFan[0].z = 0.0f;
		fovFan[0].rhw = 1.0f;
		fovFan[0].color = D3DCOLOR_ARGB(55, 100, 210, 255);
		for (int i = 0; i <= 8; ++i)
		{
			float a = fovCenterAngle - fovHalfWidth + (2.0f * fovHalfWidth * (float)i / 8.0f);
			fovFan[i + 1].x = mapCx + cosf(a) * (mapRadius * 0.85f);
			fovFan[i + 1].y = mapCy + sinf(a) * (mapRadius * 0.85f);
			fovFan[i + 1].z = 0.0f;
			fovFan[i + 1].rhw = 1.0f;
			fovFan[i + 1].color = D3DCOLOR_ARGB(4, 100, 210, 255);
		}
		dev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 8, fovFan, sizeof(HudVertex2D));

		// Outer gold/cyan radar bezel rings
		DrawHudCircleRing(dev, mapCx, mapCy, mapRadius, D3DCOLOR_ARGB(220, 195, 160, 80), 36);
		DrawHudCircleRing(dev, mapCx, mapCy, mapRadius + 2.0f, D3DCOLOR_ARGB(140, 80, 140, 195), 36);

		float scalePxPerUnit = (mapRadius - 6.0f) / max(10.0f, m_minimapZoomRadius);

		// Plot Portals in current zone (purple diamonds)
		if (m_gStatus->m_portalDatabase)
		{
			for (POSITION pPos = m_gStatus->m_portalDatabase->GetHeadPosition(); pPos != NULL;)
			{
				CPortalSysObj *portalObj = (CPortalSysObj*)m_gStatus->m_portalDatabase->GetNext(pPos);
				if (!portalObj)
					continue;
				float dx = portalObj->m_wldPosition.x - ptrPlayer->List_lastPosition.x;
				float dz = portalObj->m_wldPosition.z - ptrPlayer->List_lastPosition.z;
				float dist = sqrtf(dx * dx + dz * dz);
				if (dist > m_minimapZoomRadius * 1.05f)
					continue;
				if (m_zoneMapLevels >= 2 && ZoneMapLevelOf(portalObj->m_wldPosition.y) != m_zoneMapPlayerLevel)
					continue;

				float rx = dx * cosf(radarRotRad) - dz * sinf(radarRotRad);
				float rz = dx * sinf(radarRotRad) + dz * cosf(radarRotRad);
				float bx = mapCx + rx * scalePxPerUnit;
				float by = mapCy - rz * scalePxPerUnit;

				DrawHudDiamond(dev, bx, by, 4.5f, D3DCOLOR_ARGB(240, 195, 85, 255), D3DCOLOR_ARGB(255, 245, 200, 255));
				if (fabsf(m_lastMouseScreenX - bx) < 7.0f && fabsf(m_lastMouseScreenY - by) < 7.0f)
				{
					hoveredBlipText.Format("Portal: %s (%dm)",
						portalObj->m_portalName.IsEmpty() ? "Zone Exit" : (const char*)portalObj->m_portalName,
						(int)dist);
				}
			}
		}

		// Plot Entities (Players, Friendly NPCs, Quest NPCs, Hostile Mobs)
		POSITION posEnt = m_gStatus->m_entObjRuntime->GetHeadPosition();
		if (posEnt)
			m_gStatus->m_entObjRuntime->GetNext(posEnt); // skip local player at head

		while (posEnt)
		{
			CMovementObj *ptrEnt = (CMovementObj*)m_gStatus->m_entObjRuntime->GetNext(posEnt);
			if (!ptrEnt)
				continue;

			float dx = ptrEnt->List_lastPosition.x - ptrPlayer->List_lastPosition.x;
			float dz = ptrEnt->List_lastPosition.z - ptrPlayer->List_lastPosition.z;
			float dist = sqrtf(dx * dx + dz * dz);
			if (dist > m_minimapZoomRadius)
				continue;
			if (m_zoneMapLevels >= 2 && ZoneMapLevelOf(ptrEnt->List_lastPosition.y) != m_zoneMapPlayerLevel)
				continue;   // on another floor

			float rx = dx * cosf(radarRotRad) - dz * sinf(radarRotRad);
			float rz = dx * sinf(radarRotRad) + dz * cosf(radarRotRad);
			float bx = mapCx + rx * scalePxPerUnit;
			float by = mapCy - rz * scalePxPerUnit;

			// Check if this entity is a Quest Giver for one of the player's quests
			bool isQuestNpc = false;
			bool isActiveQuestNpc = false;
			if (!ptrEnt->List_name.IsEmpty() &&
				m_gStatus->m_multiplayerObject &&
				m_gStatus->m_multiplayerObject->m_journalEntries)
			{
				for (POSITION qPos = m_gStatus->m_multiplayerObject->m_journalEntries->GetHeadPosition(); qPos != NULL;)
				{
					CQuestJournalObj *qEntry = (CQuestJournalObj*)m_gStatus->m_multiplayerObject->m_journalEntries->GetNext(qPos);
					if (qEntry && qEntry->m_npcName.CompareNoCase(ptrEnt->List_name) == 0)
					{
						isQuestNpc = true;
						if (qEntry->m_status == 0)
							isActiveQuestNpc = true;
						break;
					}
				}
			}

			// Highlight currently targeted entity
			if (m_gStatus->m_tgSys_entitySelected == ptrEnt->List_traceNumber && ptrEnt->List_traceNumber >= 0)
			{
				DrawHudCircleRing(dev, bx, by, 6.5f, D3DCOLOR_ARGB(255, 255, 255, 255), 14);
			}

			if (isQuestNpc)
			{
				DWORD qFill = isActiveQuestNpc ? D3DCOLOR_ARGB(255, 255, 215, 40) : D3DCOLOR_ARGB(235, 110, 235, 130);
				DrawHudDiamond(dev, bx, by, 5.0f, qFill, D3DCOLOR_ARGB(255, 255, 250, 200));
			}
			else if (ptrEnt->List_controlType == 0)
			{
				// Other Player
				DrawHudDisc(dev, bx, by, 3.6f, D3DCOLOR_ARGB(255, 70, 235, 255), D3DCOLOR_ARGB(255, 20, 140, 210), 10);
			}
			else if (ptrEnt->List_controlType == 35)
			{
				// Friendly NPC / Merchant
				DrawHudDisc(dev, bx, by, 3.4f, D3DCOLOR_ARGB(255, 255, 205, 65), D3DCOLOR_ARGB(255, 180, 120, 20), 10);
			}
			else
			{
				// Hostile Mob / AI Entity
				DWORD mobColor = (ptrEnt->List_energy <= 0)
					? D3DCOLOR_ARGB(170, 140, 140, 140)
					: D3DCOLOR_ARGB(250, 245, 65, 65);
				DrawHudDisc(dev, bx, by, 3.3f, mobColor, D3DCOLOR_ARGB(255, 130, 15, 15), 10);
			}

			if (fabsf(m_lastMouseScreenX - bx) < 6.5f && fabsf(m_lastMouseScreenY - by) < 6.5f)
			{
				CString entName = ptrEnt->List_name.IsEmpty() ? "Entity" : ptrEnt->List_name;
				if (isActiveQuestNpc)
					hoveredBlipText.Format("[Quest] %s (%dm)", (const char*)entName, (int)dist);
				else
					hoveredBlipText.Format("%s (%dm)", (const char*)entName, (int)dist);
			}
		}

		// Local Player Center Arrow / Chevron
		float arrowAngle = m_minimapNorthUp ? playerYawRad : 0.0f;
		float sinA = sinf(arrowAngle);
		float cosA = cosf(arrowAngle);
		auto rotPtX = [&](float lx, float ly) { return mapCx + (lx * cosA - ly * sinA); };
		auto rotPtY = [&](float lx, float ly) { return mapCy + (lx * sinA + ly * cosA); };

		HudVertex2D playerArrow[4] = {
			{ rotPtX(0.0f, -6.5f), rotPtY(0.0f, -6.5f), 0.0f, 1.0f, D3DCOLOR_ARGB(255, 255, 255, 255) },
			{ rotPtX(4.8f,  5.0f), rotPtY(4.8f,  5.0f), 0.0f, 1.0f, D3DCOLOR_ARGB(255, 80, 220, 255) },
			{ rotPtX(0.0f,  2.2f), rotPtY(0.0f,  2.2f), 0.0f, 1.0f, D3DCOLOR_ARGB(255, 180, 245, 255) },
			{ rotPtX(-4.8f, 5.0f), rotPtY(-4.8f, 5.0f), 0.0f, 1.0f, D3DCOLOR_ARGB(255, 80, 220, 255) }
		};
		HudVertex2D arrowTris[6] = {
			playerArrow[0], playerArrow[1], playerArrow[2],
			playerArrow[0], playerArrow[2], playerArrow[3]
		};
		dev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, arrowTris, sizeof(HudVertex2D));

		// Cardinal Compass Labels (N, E, S, W) around the radar ring
		const char* cardLabels[4] = { "N", "E", "S", "W" };
		const float cardAngles[4] = { 0.0f, 1.5707963f, 3.1415926f, 4.7123889f };
		for (int c = 0; c < 4; ++c)
		{
			float ang = cardAngles[c] - radarRotRad;
			float lx = mapCx + sinf(ang) * (mapRadius - 8.0f);
			float ly = mapCy - cosf(ang) * (mapRadius - 8.0f);
			DWORD cColor = (c == 0) ? D3DCOLOR_ARGB(255, 255, 95, 80) : D3DCOLOR_ARGB(230, 220, 210, 175);
			DrawHudTextShadowed(m_hudFontSmall, cardLabels[c],
				(int)(lx - 10.0f), (int)(ly - 8.0f), (int)(lx + 10.0f), (int)(ly + 8.0f),
				DT_CENTER | DT_VCENTER | DT_SINGLELINE, cColor);
		}
		dev->SetTexture(0, NULL);
		dev->SetFVF(HUD_FVF_2D);
	}

	// Footer Bar: Coordinates + Compass Heading + Zoom Controls
	float footerTop = bodyBottom;
	float footerBottom = footerTop + 22.0f;
	DrawHudRectFill(dev, panelLeft, footerTop, panelRight, footerBottom,
		D3DCOLOR_ARGB(220, 14, 20, 32), D3DCOLOR_ARGB(230, 8, 12, 20));
	DrawHudRectBorder(dev, panelLeft, footerTop, panelRight, footerBottom, D3DCOLOR_ARGB(195, 150, 120, 60));

	CString compassDir = GetCompasDir(ptrPlayer);
	CString coordStr;
	coordStr.Format("%s  %d, %d",
		(const char*)compassDir,
		(int)ptrPlayer->List_lastPosition.x,
		(int)ptrPlayer->List_lastPosition.z);

	if (m_minimapMode != 2)
	{
		float zoomPlusLeft = panelRight - 22.0f;
		float zoomMinusLeft = panelRight - 86.0f;   // room for "4000m"
		DrawHudRectFill(dev, zoomMinusLeft, footerTop + 3.0f, zoomMinusLeft + 16.0f, footerBottom - 3.0f,
			D3DCOLOR_ARGB(200, 30, 44, 65), D3DCOLOR_ARGB(200, 18, 26, 40));
		DrawHudRectBorder(dev, zoomMinusLeft, footerTop + 3.0f, zoomMinusLeft + 16.0f, footerBottom - 3.0f, D3DCOLOR_ARGB(180, 120, 155, 205));

		DrawHudRectFill(dev, zoomPlusLeft, footerTop + 3.0f, zoomPlusLeft + 16.0f, footerBottom - 3.0f,
			D3DCOLOR_ARGB(200, 30, 44, 65), D3DCOLOR_ARGB(200, 18, 26, 40));
		DrawHudRectBorder(dev, zoomPlusLeft, footerTop + 3.0f, zoomPlusLeft + 16.0f, footerBottom - 3.0f, D3DCOLOR_ARGB(180, 120, 155, 205));

		CString zoomStr;
		zoomStr.Format("%dm", (int)m_minimapZoomRadius);
		DrawHudTextShadowed(m_hudFontSmall, "-",
			(int)zoomMinusLeft, (int)footerTop + 2, (int)zoomMinusLeft + 16, (int)footerBottom - 2,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 210, 235, 255));
		DrawHudTextShadowed(m_hudFontSmall, zoomStr,
			(int)zoomMinusLeft + 17, (int)footerTop + 2, (int)zoomPlusLeft - 1, (int)footerBottom - 2,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 185, 205, 230));
		DrawHudTextShadowed(m_hudFontSmall, "+",
			(int)zoomPlusLeft, (int)footerTop + 2, (int)zoomPlusLeft + 16, (int)footerBottom - 2,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 210, 235, 255));
		DrawHudTextShadowed(m_hudFontSmall, coordStr,
			(int)panelLeft + 6, (int)footerTop + 2, (int)zoomMinusLeft - 4, (int)footerBottom - 2,
			DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, D3DCOLOR_ARGB(255, 225, 235, 245));
	}
	else
	{
		DrawHudTextShadowed(m_hudFontSmall, coordStr,
			(int)panelLeft + 6, (int)footerTop + 2, (int)panelRight - 6, (int)footerBottom - 2,
			DT_LEFT | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 225, 235, 245));
	}
	dev->SetTexture(0, NULL);
	dev->SetFVF(HUD_FVF_2D);

	// Hover tooltip over radar blips
	if (!hoveredBlipText.IsEmpty())
	{
		float tipW = min(240.0f, (float)(hoveredBlipText.GetLength() * 7 + 16));
		float tipLeft = max(8.0f, min(m_lastMouseScreenX - tipW - 8.0f, screenW - tipW - 8.0f));
		float tipTop = max(8.0f, min(m_lastMouseScreenY - 12.0f, screenH - 30.0f));
		DrawHudRectFill(dev, tipLeft, tipTop, tipLeft + tipW, tipTop + 22.0f,
			D3DCOLOR_ARGB(235, 16, 22, 34), D3DCOLOR_ARGB(235, 10, 14, 22));
		DrawHudRectBorder(dev, tipLeft, tipTop, tipLeft + tipW, tipTop + 22.0f, D3DCOLOR_ARGB(230, 235, 195, 90));
		DrawHudTextShadowed(m_hudFontSmall, hoveredBlipText,
			(int)tipLeft + 6, (int)tipTop + 2, (int)(tipLeft + tipW - 6), (int)(tipTop + 20.0f),
			DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, D3DCOLOR_ARGB(255, 255, 240, 185));
		dev->SetTexture(0, NULL);
		dev->SetFVF(HUD_FVF_2D);
	}

	// =========================================================================
	// 2. ON-SCREEN ACTIVE QUEST TRACKER PANEL (Right Side, Below Minimap)
	// =========================================================================
	int totalQuests = 0;
	int activeQuests = 0;
	if (m_gStatus->m_multiplayerObject && m_gStatus->m_multiplayerObject->m_journalEntries)
	{
		for (POSITION qPos = m_gStatus->m_multiplayerObject->m_journalEntries->GetHeadPosition(); qPos != NULL;)
		{
			CQuestJournalObj *qEntry = (CQuestJournalObj*)m_gStatus->m_multiplayerObject->m_journalEntries->GetNext(qPos);
			if (!qEntry)
				continue;
			totalQuests++;
			if (qEntry->m_status == 0)
				activeQuests++;
		}
	}

	float qtWidth = 236.0f;
	float qtRight = QuestTrackerRight(screenW);
	float qtLeft = qtRight - qtWidth;
	float qtHeaderTop = footerBottom + 8.0f;
	float qtHeaderBottom = qtHeaderTop + 22.0f;

	DrawHudRectFill(dev, qtLeft, qtHeaderTop, qtRight, qtHeaderBottom,
		D3DCOLOR_ARGB(215, 22, 28, 42), D3DCOLOR_ARGB(215, 12, 16, 26));
	DrawHudRectBorder(dev, qtLeft, qtHeaderTop, qtRight, qtHeaderBottom, D3DCOLOR_ARGB(200, 180, 145, 70));

	float qtToggleLeft = qtRight - 24.0f;
	float qtJournalLeft = qtToggleLeft - 52.0f;
	DrawHudRectFill(dev, qtJournalLeft, qtHeaderTop + 3.0f, qtToggleLeft - 3.0f, qtHeaderBottom - 3.0f,
		D3DCOLOR_ARGB(200, 52, 40, 18), D3DCOLOR_ARGB(200, 32, 24, 10));
	DrawHudRectBorder(dev, qtJournalLeft, qtHeaderTop + 3.0f, qtToggleLeft - 3.0f, qtHeaderBottom - 3.0f, D3DCOLOR_ARGB(195, 215, 170, 75));

	DrawHudRectFill(dev, qtToggleLeft, qtHeaderTop + 3.0f, qtRight - 4.0f, qtHeaderBottom - 3.0f,
		D3DCOLOR_ARGB(200, 32, 45, 68), D3DCOLOR_ARGB(200, 20, 28, 44));
	DrawHudRectBorder(dev, qtToggleLeft, qtHeaderTop + 3.0f, qtRight - 4.0f, qtHeaderBottom - 3.0f, D3DCOLOR_ARGB(180, 120, 160, 210));

	CString qtHeaderTitle;
	if (totalQuests > 0)
		qtHeaderTitle.Format("QUESTS (%d Active / %d)", activeQuests, totalQuests);
	else
		qtHeaderTitle = "QUEST TRACKER";

	DrawHudTextShadowed(m_hudFontTitle, qtHeaderTitle,
		(int)qtLeft + 6, (int)qtHeaderTop + 2, (int)qtJournalLeft - 4, (int)qtHeaderBottom - 2,
		DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS, D3DCOLOR_ARGB(255, 240, 205, 115));
	DrawHudTextShadowed(m_hudFontSmall, "Log",
		(int)qtJournalLeft, (int)qtHeaderTop + 2, (int)qtToggleLeft - 3, (int)qtHeaderBottom - 2,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 220, 120));
	DrawHudTextShadowed(m_hudFontSmall, m_questTrackerVisible ? "-" : "+",
		(int)qtToggleLeft, (int)qtHeaderTop + 2, (int)qtRight - 4, (int)qtHeaderBottom - 2,
		DT_CENTER | DT_VCENTER | DT_SINGLELINE, D3DCOLOR_ARGB(255, 195, 230, 255));
	dev->SetTexture(0, NULL);
	dev->SetFVF(HUD_FVF_2D);

	if (m_questTrackerVisible)
	{
		CMenuInventory *pPlayerInventory = m_gStatus->m_runtimeMenuDatabase
			? dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true))
			: NULL;

		float curY = qtHeaderBottom + 2.0f;
		int shownQuests = 0;
		const int maxShownQuests = 3;

		if (totalQuests == 0)
		{
			float boxBottom = curY + 38.0f;
			DrawHudRectFill(dev, qtLeft, curY, qtRight, boxBottom,
				D3DCOLOR_ARGB(165, 10, 14, 22), D3DCOLOR_ARGB(175, 6, 10, 16));
			DrawHudRectBorder(dev, qtLeft, curY, qtRight, boxBottom, D3DCOLOR_ARGB(130, 110, 95, 55));
			DrawHudTextShadowed(m_hudFontSmall,
				"No active quests logged.\nSpeak to NPCs or click [Log].",
				(int)qtLeft + 8, (int)curY + 4, (int)qtRight - 8, (int)boxBottom - 4,
				DT_LEFT | DT_TOP | DT_WORDBREAK, D3DCOLOR_ARGB(215, 185, 195, 210));
			dev->SetTexture(0, NULL);
			dev->SetFVF(HUD_FVF_2D);
		}
		else if (m_gStatus->m_multiplayerObject && m_gStatus->m_multiplayerObject->m_journalEntries)
		{
			// Show active quests first; if all quests are completed, show the most recent completed ones
			bool showCompletedFallback = (activeQuests == 0);
			for (POSITION qPos = m_gStatus->m_multiplayerObject->m_journalEntries->GetHeadPosition();
				 qPos != NULL && shownQuests < maxShownQuests;)
			{
				CQuestJournalObj *qEntry = (CQuestJournalObj*)m_gStatus->m_multiplayerObject->m_journalEntries->GetNext(qPos);
				if (!qEntry)
					continue;
				if (!showCompletedFallback && qEntry->m_status != 0)
					continue;

				int reqItemCount = (qEntry->m_requiredItems) ? min(4, (int)qEntry->m_requiredItems->GetCount()) : 0;
				bool hasDesc = !qEntry->m_questDescription.IsEmpty();
				bool hasHint = !qEntry->m_keyStatement.IsEmpty();
				float cardH = 22.0f + (hasDesc ? 30.0f : 0.0f) + (hasHint ? 18.0f : 0.0f) + (float)(reqItemCount * 16) + 6.0f;

				DrawHudRectFill(dev, qtLeft, curY, qtRight, curY + cardH,
					D3DCOLOR_ARGB(175, 12, 18, 28), D3DCOLOR_ARGB(190, 8, 12, 20));
				DrawHudRectBorder(dev, qtLeft, curY, qtRight, curY + cardH,
					(qEntry->m_status == 0) ? D3DCOLOR_ARGB(165, 165, 135, 65) : D3DCOLOR_ARGB(150, 75, 155, 95));

				// Check if this quest's NPC is nearby in m_entObjRuntime so we can show distance & bearing
				CString npcHeader = qEntry->m_npcName;
				for (POSITION ePos = m_gStatus->m_entObjRuntime->GetHeadPosition(); ePos != NULL;)
				{
					CMovementObj *entObj = (CMovementObj*)m_gStatus->m_entObjRuntime->GetNext(ePos);
					if (entObj && entObj != ptrPlayer && entObj->List_name.CompareNoCase(qEntry->m_npcName) == 0)
					{
						float dx = entObj->List_lastPosition.x - ptrPlayer->List_lastPosition.x;
						float dz = entObj->List_lastPosition.z - ptrPlayer->List_lastPosition.z;
						int distM = (int)sqrtf(dx * dx + dz * dz);
						npcHeader.Format("%s  [%dm]", (const char*)qEntry->m_npcName, distM);
						break;
					}
				}
				if (qEntry->m_status != 0)
					npcHeader = "[Done] " + npcHeader;

				float textY = curY + 4.0f;
				DrawHudTextShadowed(m_hudFontTitle, npcHeader,
					(int)qtLeft + 7, (int)textY, (int)qtRight - 7, (int)(textY + 16.0f),
					DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS,
					(qEntry->m_status == 0) ? D3DCOLOR_ARGB(255, 255, 215, 95) : D3DCOLOR_ARGB(255, 130, 235, 145));
				textY += 18.0f;

				if (hasDesc)
				{
					DrawHudTextShadowed(m_hudFontSmall, qEntry->m_questDescription,
						(int)qtLeft + 9, (int)textY, (int)qtRight - 7, (int)(textY + 28.0f),
						DT_LEFT | DT_TOP | DT_WORDBREAK | DT_END_ELLIPSIS,
						D3DCOLOR_ARGB(240, 225, 230, 240));
					textY += 30.0f;
				}

				if (hasHint)
				{
					CString hintLine = "Hint: " + qEntry->m_keyStatement;
					DrawHudTextShadowed(m_hudFontSmall, hintLine,
						(int)qtLeft + 9, (int)textY, (int)qtRight - 7, (int)(textY + 16.0f),
						DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS,
						D3DCOLOR_ARGB(240, 135, 215, 250));
					textY += 18.0f;
				}

				if (qEntry->m_requiredItems && reqItemCount > 0)
				{
					int itemDrawn = 0;
					for (POSITION rPos = qEntry->m_requiredItems->GetHeadPosition();
						 rPos != NULL && itemDrawn < reqItemCount;)
					{
						CQJRequiredItem *reqItem = (CQJRequiredItem*)qEntry->m_requiredItems->GetNext(rPos);
						if (!reqItem)
							continue;

						int neededQty = (reqItem->m_quanity > 0) ? reqItem->m_quanity : 1;
						int haveQty = 0;
						// the journal stores names as "     (10)Clegurk Tongue" - match and show the plain item name
						CString reqName = reqItem->m_itemName;
						reqName.TrimLeft();
						if (reqName.GetLength() > 2 && reqName[0] == '(')
						{
							int close = reqName.Find(')');
							bool digits = close > 1;
							for (int c = 1; c < close && digits; c++)
								digits = (reqName[c] >= '0' && reqName[c] <= '9');
							if (digits)
							{
								reqName = reqName.Mid(close + 1);
								reqName.TrimLeft();
							}
						}
						if (m_gStatus->m_hudInvCount >= 0 && m_gStatus->m_itemManager)
						{// what you carry (kept current by the tracker's own inventory requests)
							for (int iv = 0; iv < m_gStatus->m_hudInvCount; iv++)
							{
								int glid = m_gStatus->m_hudInvGlid[iv];
								bool match = (reqItem->m_glid > 0 && glid == reqItem->m_glid);
								if (!match)
								{
									CItemObj *itemDef = m_gStatus->m_itemManager->GetItemByID(glid);
									match = (itemDef && itemDef->m_itemName.CompareNoCase(reqName) == 0);
								}
								if (match)
									haveQty += m_gStatus->m_hudInvQty[iv];
							}
						}
						else if (pPlayerInventory && m_gStatus->m_itemManager)
						{
							for (size_t invIdx = 0; invIdx < pPlayerInventory->m_items.size(); ++invIdx)
							{
								CMenuInventoryItem *invSlot = pPlayerInventory->m_items[invIdx];
								if (!invSlot || invSlot->m_data.m_GLID <= 0)
									continue;
								CItemObj *itemDef = m_gStatus->m_itemManager->GetItemByID(invSlot->m_data.m_GLID);
								if ((reqItem->m_glid > 0 && invSlot->m_data.m_GLID == reqItem->m_glid)
								 || (itemDef && itemDef->m_itemName.CompareNoCase(reqName) == 0))
								{
									int slotQty = (invSlot->m_data.m_quantity > 0) ? invSlot->m_data.m_quantity : 1;
									haveQty += slotQty;
								}
							}
						}

						bool itemMet = (haveQty >= neededQty || qEntry->m_status != 0);
						CString itemLine;
						itemLine.Format("%s %s (%d/%d)",
							itemMet ? "[x]" : "[ ]",
							(const char*)reqName,
							haveQty,
							neededQty);
						DrawHudTextShadowed(m_hudFontSmall, itemLine,
							(int)qtLeft + 12, (int)textY, (int)qtRight - 7, (int)(textY + 15.0f),
							DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS,
							itemMet ? D3DCOLOR_ARGB(255, 125, 240, 140) : D3DCOLOR_ARGB(235, 215, 215, 220));
						textY += 16.0f;
						itemDrawn++;
					}
				}

				dev->SetTexture(0, NULL);
				dev->SetFVF(HUD_FVF_2D);
				curY += cardH + 4.0f;
				shownQuests++;
			}
		}
	}

	// =========================================================================
	// BIG WORLD MAP (M)
	// =========================================================================
	if (m_worldMapOpen)
		RenderWorldMap(dev, ptrPlayer, screenW, screenH, playerYawRad);

	// =========================================================================
	// 3. HUD TOAST NOTIFICATION BANNER (F12 Screenshot / Mode Toggles)
	// =========================================================================
	DWORD now = GetTickCount();
	if (!m_hudToastText.IsEmpty() && (now - m_hudToastTime < 3500))
	{
		float toastW = min(460.0f, (float)(m_hudToastText.GetLength() * 7 + 28));
		float toastLeft = (screenW - toastW) * 0.5f;
		float toastTop = 18.0f;
		DrawHudRectFill(dev, toastLeft, toastTop, toastLeft + toastW, toastTop + 26.0f,
			D3DCOLOR_ARGB(225, 18, 26, 40), D3DCOLOR_ARGB(225, 10, 14, 24));
		DrawHudRectBorder(dev, toastLeft, toastTop, toastLeft + toastW, toastTop + 26.0f,
			D3DCOLOR_ARGB(230, 235, 195, 90));
		DrawHudTextShadowed(m_hudFontTitle, m_hudToastText,
			(int)toastLeft + 10, (int)toastTop + 3, (int)(toastLeft + toastW - 10), (int)(toastTop + 23.0f),
			DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS,
			D3DCOLOR_ARGB(255, 255, 230, 140));
	}

	// Restore D3D9 state block and invalidate CStateManagementObj cached state
	if (pStateBlock)
	{
		pStateBlock->Apply();
		pStateBlock->Release();
	}
	if (m_gStatus->m_renderStateObject)
	{
		m_gStatus->m_renderStateObject->m_currentVertexType = -1;
		m_gStatus->m_renderStateObject->m_globalBlend = -1;
		m_gStatus->m_renderStateObject->m_currentDiffuseSource = -1;
		m_gStatus->m_renderStateObject->GlobalTextureIndex[0] = -1;
	}
}

//------------------------------------------------------------------------------------//
BOOL CMenuManagement::RenderCGMenus(long currentTime)
{
	if(!m_gStatus->m_runtimeMenuDatabase)
	    return FALSE;

	//Calculate half the screen dimensions for use later
	float halfWidth	 = m_gStatus->m_dxRenderer->m_currentWidth / 2.0f;
	float halfHeight = m_gStatus->m_dxRenderer->m_currentHeight / 2.0f;

	//Make sure persistent shaders are disabled before entering menu system
	m_gStatus->m_renderStateObject->SetPxShader(m_gStatus->m_3dDevice, m_gStatus->m_renderStateObject->m_pxShderSystemRef, -1);
	m_gStatus->m_renderStateObject->SetShader(m_gStatus->m_3dDevice, m_gStatus->m_shaderSystem, -1);

	//Render all menus from back to front(because Z is turned off...AND they're 2D)
	for(POSITION posLoc = m_gStatus->m_runtimeMenuDatabase->GetTailPosition();posLoc != NULL;)
	{
		CMenuWindow *menu = (CMenuWindow*)m_gStatus->m_runtimeMenuDatabase->GetPrev(posLoc);

		//If the menu is visible, render it
		if(menu->m_visible)
			menu->Render(m_gStatus->m_3dDevice,
						 m_gStatus->m_renderStateObject,
						 m_gStatus->m_textureDataBase,
						 halfWidth * menu->m_horizontalAlignment,
						 halfHeight * menu->m_verticalAlignment);
	}

	// Render the interactive Minimap / Radar & Active Quest Tracker HUD before the mouse cursor
	RenderModernHudOverlays(currentTime);

	if(m_gStatus->m_showMenuCursor)
	{
		//If we have a dragging item, render it
		if(m_gStatus->m_draggingItem && m_gStatus->m_draggingItem->m_icon)
		{
			m_gStatus->m_draggingItem->m_icon->Render(	m_gStatus->m_3dDevice,
														m_gStatus->m_renderStateObject,
														m_gStatus->m_textureDataBase,
														0.0f, //halfWidth * -1.0f + m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor]->m_posX - (m_gStatus->m_draggingItem->m_icon->m_width / 2.0f),
														0.0f); //halfHeight * -1.0f + m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor]->m_posY + (m_gStatus->m_draggingItem->m_icon->m_height / 2.0f));
			
			// Honor any requests to free the dragging item pointer
			if(m_gStatus->m_releaseDraggingItem) 
			{
				m_gStatus->m_draggingItem = NULL;
				m_gStatus->m_releaseDraggingItem = false;
			}
			else if(m_gStatus->m_deleteDraggingItem)
			{
				SafeDelete(m_gStatus->m_draggingItem->m_icon);
				SafeDelete(m_gStatus->m_draggingItem);
				m_gStatus->m_deleteDraggingItem = false;
			}
		}

		//Render the mouse cursor
		m_gStatus->m_mouseCursors[m_gStatus->m_currentCursor]->Render(	m_gStatus->m_3dDevice,
																		m_gStatus->m_renderStateObject,
																		m_gStatus->m_textureDataBase,
																		halfWidth * -1.0f,
																		halfHeight * -1.0f);
	}

   return TRUE;
}
//----------------------------------------------------------//
BOOL CMenuManagement::GetCurrentJetData(CMovementObj * ptrLoc,int & cur,int & max)
{
	if(!ptrLoc->List_armedInventoryDB)
		return FALSE;
	for(POSITION posLoc = ptrLoc->List_armedInventoryDB->GetHeadPosition();posLoc != NULL;)
	{
		CArmedInventoryObj * armedPtr = (CArmedInventoryObj *)ptrLoc->List_armedInventoryDB->GetNext(posLoc);
		if(armedPtr->m_functionalityAttribute == 3){//jet
			max = armedPtr->m_miscInt;
			cur = 0;//armedPtr->m_miscInt2;
			if(armedPtr->m_obfusionMiscInt)
				cur = *armedPtr->m_obfusionMiscInt;
			return TRUE;
		}//end jet
	}
	return TRUE;
}
//------------------------------------------------------------------------------------//
CString CMenuManagement::GetCompasDir(CMovementObj * dbPtr)
{
	if(!dbPtr)
		return "Error";
 
	D3DMATRIX orientateMat;
	D3DVECTOR normal;
	normal.x = 0.0f;
	normal.y = 0.0f;
	normal.z = -1.0f;
	dbPtr->List_baseFrame->GetTransform(NULL,&orientateMat);
	normal = m_gStatus->m_cabLib.UpdatePolygonVertexRotationOnly(orientateMat.m,normal);

	normal.y = 0.0f;
	normal = m_gStatus->m_matLib.Normalize(normal);

	float rotationOnY = atan(fabs(normal.x)/fabs(normal.z))* 57.3f;
	//m_gStatus->m_systemReadout->AddLine(rotationOnY,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);

	if(normal.z > 0)
	{//something northern
		if(normal.x > 0.0f)
		{
			if(rotationOnY < 22.5)
				return "N";
			else if(rotationOnY < 67.5)
				return "NE";
			else
				return "E";
		}
		else
		{
			if(rotationOnY < 22.5)
				return "N";
			else if(rotationOnY < 67.5)
				return "NW";
			else
				return "W";
		}
	}//end something northern
	else
	{//something southern
		if(normal.x > 0.0f)
		{
			if(rotationOnY < 22.5)
				return "S";
			else if(rotationOnY < 67.5)
				return "SE";
			else
				return "E";
		}
		else
		{
			if(rotationOnY < 22.5)
				return "S";
			else if(rotationOnY < 67.5)
				return "SW";
			else
				return "W";
		}
	}//end something southern

	return "UN";
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
float CMenuManagement::GetEntityDir(CMovementObj * dbPtr)
{
	if(!dbPtr)
		return 0.0f;
 
	D3DMATRIX orientateMat;
	D3DVECTOR normal;
	normal.x = 0.0f;
	normal.y = 0.0f;
	normal.z = -1.0f;
	dbPtr->List_baseFrame->GetTransform(NULL,&orientateMat);
	normal = m_gStatus->m_cabLib.UpdatePolygonVertexRotationOnly(orientateMat.m,normal);
	normal.y = 0.0f;
	normal = m_gStatus->m_matLib.Normalize(normal);
	float rotationOnY = atan(fabs(normal.x)/fabs(normal.z))* 57.3f;

	if(normal.z > 0)
	{//north
		if(normal.x > 0.0f)
			return rotationOnY;
		else
			return 360.0f - rotationOnY;
	}
	else
	{//south
		if(normal.x > 0.0f)
			return 180.0f - rotationOnY;
		else
			return 180.0f + rotationOnY;
	}
}
//------------------------------------------------------------------------------------//
BOOL CMenuManagement::RenderOverlays()
{
	//---------render overlays-----------//
	//get primary vport     
	long currentTime = GetTickCount();
	m_gStatus->m_renderStateObject->SetFogFilter(m_gStatus->m_3dDevice,TRUE);
	//m_gStatus->m_3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER,NULL, 1.0f, 0L );//flatten scene
 
	m_gStatus->m_3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	//RenderTextCache(m_gStatus->m_textRenderList,mainVport,0);
	RenderCGMenus(currentTime);

	/*for(POSITION ovrPos = m_gStatus->m_inViewEntityList->GetHeadPosition();ovrPos != NULL;)
	{
		CDynamicOverlayObj * newPtr = (CDynamicOverlayObj *)m_gStatus->m_inViewEntityList->GetNext(ovrPos);
		D3DMATRIX initMatCopy = m_gStatus->m_matrixInit2;
		initMatCopy._41 = newPtr->m_screenPos.x;
		initMatCopy._42 = newPtr->m_screenPos.y;
		initMatCopy._43 = newPtr->m_screenPos.z;
		D3DMATRIX scaleMatrix;
		m_gStatus->m_matLib.scale(newPtr->m_multiplier,newPtr->m_multiplier,newPtr->m_multiplier,scaleMatrix.m);
		m_gStatus->m_matLib.fastMatrixMultiply(initMatCopy,scaleMatrix,initMatCopy); 
		//initMatCopy._43 = newPtr->m_screenPos.z;
		m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&initMatCopy,m_gStatus->m_3dDevice);
		POSITION iconPos = m_gStatus->m_iconDatabase->FindIndex(5);
		if(iconPos)
		{
			CIconSysObj * iconSys = (CIconSysObj *)m_gStatus->m_iconDatabase->GetAt(iconPos);
			iconSys->Render(m_gStatus->m_3dDevice,m_gStatus->m_renderStateObject,m_gStatus->m_textureDataBase);
		}
	}*/
				

	m_gStatus->m_3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	return TRUE;
}
//-----------------------------------------------------------------------//
void CMenuManagement::RenderTextCache(CTextureRenderObjList*textRList,
									    CViewportObj* vptPtr,
									    int fontSysDB)
{
	POSITION fntPos = m_gStatus->m_cgFontList->FindIndex(fontSysDB);
	if(!fntPos)
		return;
    CGFontObj * cg_fntObject = (CGFontObj *)m_gStatus->m_cgFontList->GetAt(fntPos);
	m_gStatus->m_renderStateObject->SetTextureState(cg_fntObject->m_fontTableIndex,
													m_gStatus->m_renderStateObject->m_intZero,
													m_gStatus->m_3dDevice,
													m_gStatus->m_textureDataBase);


    
	POSITION fntBase = m_gStatus->m_textureDataBase->FindIndex(cg_fntObject->m_fontTableIndex);
	if(!fntBase)
		return;
	
	CTextureDBObj * fontTexture = (CTextureDBObj *)m_gStatus->m_textureDataBase->GetAt(fntBase);
	D3DMATRIX movMatrix;
	m_gStatus->m_matLib.InitMat(movMatrix.m); 
	m_gStatus->m_renderStateObject->SetPxShader(m_gStatus->m_3dDevice,m_gStatus->m_pxShaderSystem,-1);
	m_gStatus->m_renderStateObject->SetShader(m_gStatus->m_3dDevice,m_gStatus->m_shaderSystem,-1);
	m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&movMatrix,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->SetBlendLevel(m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_3dDevice);
	for(int loop=1;loop<m_gStatus->m_renderStateObject->m_multiTextureSupported;loop++)
	{
		m_gStatus->m_renderStateObject->SetBlendLevel(loop,m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_3dDevice);
		if(loop>7)
			 break;
	}
	//readout
	m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_3dDevice,2);
	float scalar = m_gStatus->m_dxRenderer->GetWidthAspectMultiplier();
	if(m_gStatus->m_forceScalarForAspect != 0.0f)
		scalar = m_gStatus->m_forceScalarForAspect;
	//m_gStatus->m_systemReadout->m_cgVertSpacing = m_gStatus->m_readoutFontSpacing * scalar;
	//m_gStatus->m_systemReadout->m_cgFontSizeX   = m_gStatus->m_readoutFontX * scalar;
	//m_gStatus->m_systemReadout->m_cgFontSizeY   = m_gStatus->m_readoutFontY * scalar;
	//m_gStatus->m_systemReadout->m_maxMeasuredLength = m_gStatus->m_readoutMaxMeasuredLength * scalar;
				
	//m_gStatus->m_systemReadout->RenderTextCG(m_gStatus->m_renderStateObject,m_gStatus->m_3dDevice);

	if(textRList)
	for(POSITION posLoc = textRList->GetHeadPosition();posLoc != NULL;)
	{//text loop
		CTextureRenderObj * textPtr = (CTextureRenderObj *)textRList->GetNext(posLoc);
				 
		m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_3dDevice,textPtr->m_blendMode);
		m_gStatus->m_renderStateObject->SetMaterial(m_gStatus->m_3dDevice,&textPtr->m_materialObj->m_useMaterial);
		textPtr->Render(m_gStatus->m_3dDevice,m_gStatus->m_renderStateObject);
	}//end text loop
}
//------------------------------------------------------------------------------------//
void CMenuManagement::RenderCGMenuCursors(long currentTime)
{
    //prioritize menu cursors over Gameplay cursors, also this will replicate off position aquisition so that only one cursor is used
	CBoneCursorObj * finalCursor = NULL;
	//ZeroMemory(&finalCurPositionRet,sizeof(D3DVECTOR));
    for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//loop through menus
        CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
        if(menu->m_cursor)
		{
		   if(finalCursor)
            if(finalCursor->m_cursorType == 0)//already found a menu based cursor
              continue;

		   //CopyMemory(&finalCurPositionRet,&menu->m_cursor->m_position,sizeof(D3DVECTOR));
		   finalCursor = menu->m_cursor;
		   if(m_gStatus->m_tgSys_hoverState > 0)
		   if(menu->m_additionalCursors)
		   {//base on hover state possibly select a optional Cursor
			  CBoneCursorObj * optionalCursor = menu->m_additionalCursors->GetByUseTypeID(m_gStatus->m_tgSys_hoverState);
			  if(optionalCursor)
			  { 
				 CopyMemory(&optionalCursor->m_position,&finalCursor->m_position,sizeof(D3DVECTOR));//position by controller cursor
				 finalCursor = optionalCursor;
			  }
		   }//end base on hover state possibly select a optional Cursor
		}
	}
	if(finalCursor)
	   CGCursorRender(finalCursor);


	/*CMenu3DObj * finalMenu = NULL;
	CBoneCursorObj * finalCursor = NULL;
    for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//loop through menus
        CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
        if(menu->m_cursor)
		{
            if(menu->m_cursor->m_cursorType == 0)//already found a menu based cursor
			{
              if(!finalCursor)
                  finalCursor = menu->m_cursor;
              continue;
			}

		   //finalCursor = menu->m_cursor;
		   finalMenu = menu;
		}
	}

	
	if(finalMenu)
	{
       finalCursor = finalMenu->m_cursor;//default cursor
       if(m_gStatus->m_tgSys_hoverState > 0)
		   if(finalMenu->m_additionalCursors)
		   {//base on hover state possibly select a optional Cursor
			  CBoneCursorObj * optionalCursor = finalMenu->m_additionalCursors->GetByUseTypeID(m_gStatus->m_tgSys_hoverState);
			  if(optionalCursor)
			  { 
				 CopyMemory(&optionalCursor->m_position,&finalCursor->m_position,sizeof(D3DVECTOR));//position by controller cursor
				 finalCursor = optionalCursor;
			  }
		   }//end base on hover state possibly select a optional Cursor	   
	}
    if(finalCursor)
	   CGCursorRender(finalCursor);*/
}
//------------------------------------------------------------------------------------//
/*void CMenuManagement::EvaluateFinalCursorPosition(D3DVECTOR & finalCurPositionRet)
{
    //prioritize menu cursors over Gameplay cursors, also this will replicate off position aquisition so that only one cursor is used
    DWORD currentTime = GetTickCount();
	CBoneCursorObj * finalCursor = NULL;
	ZeroMemory(&finalCurPositionRet,sizeof(D3DVECTOR));
    for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//loop through menus
        CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
        if(menu->m_cursor)
		{
			if(finalCursor)
            if(finalCursor->m_cursorType == 0)//already found a menu based cursor
				continue;

			CopyMemory(&finalCurPositionRet,&menu->m_cursor->m_position,sizeof(D3DVECTOR));
			finalCursor = menu->m_cursor;
		}
	}
	if(finalCursor)
		finalCursor->AnimationManagement(currentTime, m_userControls->m_input->m_mousepointTarget, m_gStatus->m_timeAdjustFloat);
}*/
//------------------------------------------------------------------------------------//
BOOL CMenuManagement::CGCursorRender(CBoneCursorObj * cursor)
{
	if(!cursor)
		return FALSE;

	if(!cursor->m_skeletonObject)
		return FALSE;

	long currentTime = GetTickCount();

	//cursor->AnimationManagement(currentTime,m_userControls->m_input->m_mousepointTarget);

	D3DVECTOR offsetVector;
	offsetVector.x = 0.0f;
	offsetVector.y = 0.0f;
	offsetVector.z = 0.0f;

	cursor->m_skeletonObject->AnimationCallback(currentTime,m_gStatus->m_matrixInit2,cursor->m_position,1.0f,FALSE,m_gStatus->m_soundManager,FALSE,m_gStatus->m_multiplayerObject->m_AIServer,m_gStatus->m_3dDevice,m_gStatus->m_d3dVectorInit,m_gStatus->m_oneVal,m_gStatus->m_lodOverride);

	D3DMATRIX positional = m_gStatus->m_matrixInit2;
	positional._41 = cursor->m_position.x;
	positional._42 = cursor->m_position.y;
	positional._43 = cursor->m_position.z;
	D3DMATRIX scaler;
  
	m_gStatus->m_matLib.scale(cursor->m_scaleFactor,cursor->m_scaleFactor,cursor->m_scaleFactor,scaler.m);
	m_gStatus->m_matLib.fastMatrixMultiply(positional,scaler,positional); 

	if(m_gStatus->m_dxRenderer->m_currentWidth > m_gStatus->m_dxRenderer->m_aspectBasis)
	{
		float adScl = m_gStatus->m_dxRenderer->GetWidthAspectMultiplier();
		if(m_gStatus->m_forceScalarForAspect != 0.0f)
			adScl = m_gStatus->m_forceScalarForAspect;
		m_gStatus->m_matLib.scale(adScl,adScl,1.0f,scaler.m);
		m_gStatus->m_matLib.fastMatrixMultiply(positional,scaler,positional); 
	}
	m_gStatus->BonesSystemRender(cursor->m_skeletonObject,offsetVector,positional);

	return TRUE;
}

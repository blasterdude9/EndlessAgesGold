#include "StdAfx.h"
#include "attacksystemsmanagement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAttackSystemsManagement::CAttackSystemsManagement(CSharedNet * sharedInterface)
{
	m_sharedInterfaceRef = sharedInterface;
}

CAttackSystemsManagement::~CAttackSystemsManagement(void)
{
}

//--------------Handles system messages-----------------------------------------------//
BOOL CAttackSystemsManagement::HandleFireMessagesServerSide_safe(LPFIRE_STRUCT formatedMessage,
															     DPNID messageFromDPID)
{
	CPlayerObject * playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
	if(!playerObjectData)
	{
		return FALSE;
	}
	
	//manage ammo
	if(!playerObjectData->m_aiControlled)
	{//ai is excempt
		if(playerObjectData->m_stasis == 1){//if you fire when hiding unhide
		   m_sharedInterfaceRef->Unhide(playerObjectData);
		}//end if you fire when hiding unhide

      CInventoryObj * weaponLink = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
	  if(weaponLink)
	  {//aquire weapon used
		  if(weaponLink->m_itemData->m_maxCharges != 0)//max changes at zero would be unlimited ammo
		  {//does require ammo of some sort
			int amountOfCharges = 1;
			for(POSITION posLoc = weaponLink->m_itemData->m_attachmentRefList->GetHeadPosition();posLoc != NULL;)
			{
				CAttachmentRefObj * ref = (CAttachmentRefObj *)weaponLink->m_itemData->m_attachmentRefList->GetNext(posLoc);
				CArmedInventoryObj * visObj = m_sharedInterfaceRef->m_itemManager->m_visuals->GetByTypeDirectAccess(ref->m_attachmentType);
				if(visObj)
				{//try to verify fire que
					if(visObj->m_firstPersonSystem)
					{	
						if(visObj->m_firstPersonSystem->m_fireQueList)
						{
							amountOfCharges = visObj->m_firstPersonSystem->m_fireQueList->GetCount();
							break;
						}
					}

					if(visObj->m_thirdPersonSystem)
					{
						if(visObj->m_thirdPersonSystem->m_fireQueList)
						{
							amountOfCharges = visObj->m_thirdPersonSystem->m_fireQueList->GetCount();
							break;
						}
					}
				}//end try to verify fire que
			}

			//Check to see if they can fire
			if(weaponLink->m_itemData->m_tempDepletionHolder)
				if((GetTickCount() - weaponLink->m_itemData->m_tempDepletionHolder) > weaponLink->m_itemData->m_weaponDepletionTimer)
					weaponLink->m_itemData->m_tempDepletionHolder = 0;

			if(weaponLink->m_itemData->m_tempDepletionHolder)
			{
				int secConversion = (weaponLink->m_itemData->m_weaponDepletionTimer - (GetTickCount() - weaponLink->m_itemData->m_tempDepletionHolder)) * 0.001;
				CString message = "This weapon has " + m_sharedInterfaceRef->m_cabLib.ConvToString(secConversion) + " seconds left for recharge.";
				m_sharedInterfaceRef->SendTextMsg(messageFromDPID,
												  message,		
												  playerObjectData,
												  3);
				m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
				return FALSE;
			}

//			 if(weaponLink->m_itemData->m_chargeCurrent > weaponLink->m_itemData->m_shotsPerFire)
//			 {//1a
				weaponLink->m_itemData->m_chargeCurrent -= weaponLink->m_itemData->m_shotsPerFire;
/*			 }//end 1a
			 else
			 {//empty- handler silent
				m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
				return FALSE;
			 }//end empty-handler silent
*/
			  //Time for the new ammo check
			  //First we check to see if the fire que fired was the last bullet in a sequence
			  if((formatedMessage->fireQueIndex + 1) == amountOfCharges)
			  {//We are the last bullet fired in sequance
				  if(weaponLink->m_itemData->m_chargeCurrent < 0)
				  {//after the shots our ammo is depleted below 0
					  //Retrieve how far below 0 we are to determine how long the gun needs to recharge
					  int amountBelowZero = (weaponLink->m_itemData->m_chargeCurrent * -1) * 1000; //Converted to milliseconds to add to time
					  //Next we need to see how long it will take the gun in seconds 
					  //to recharge to a minimum fire capacity on its own
					  int chargesNeeded = amountOfCharges * weaponLink->m_itemData->m_shotsPerFire;
					  int sequences = 1;
					  if(weaponLink->m_itemData->m_ammoRegenRate > 0)
						sequences = (chargesNeeded / weaponLink->m_itemData->m_ammoRegenRate) + 1;
					  int secondsNeeded = (sequences * (weaponLink->m_itemData->m_ammoSecTick * 1000)) + amountBelowZero;
					  //Set the weapon depletion timer to secondsNeeded
					  weaponLink->m_itemData->m_weaponDepletionTimer = secondsNeeded;
					  //Set the counter to start from current tick count
					  weaponLink->m_itemData->m_tempDepletionHolder = GetTickCount();
					  //Set the chargeCurrent to 0 to avoid errors
					  weaponLink->m_itemData->m_chargeCurrent = 0;
					  m_sharedInterfaceRef->SendAttribMsg(	DPNSEND_GUARANTEED,
															playerObjectData->m_currentID,
															27,
															weaponLink->m_itemData->m_maxCharges,
															0,
															weaponLink->m_itemData->m_chargeCurrent,
															weaponLink->m_itemData->m_shotsPerFire,
															playerObjectData);
					  playerObjectData->m_rechargeTimer = secondsNeeded;
					  m_sharedInterfaceRef->UpdateClientStatistics(playerObjectData, playerObjectData->m_currentID);
/*					  m_sharedInterfaceRef->SendAttribMsg(	DPNSEND_GUARANTEED,
															playerObjectData->m_currentID,
															41,
															secondsNeeded,
															0,
															0,
															0,
															playerObjectData);
*/				  }//end after the shots our ammo is depleted below 0
			  }//End we are the last bullet fired in sequence
		  }//end does require ammo of some sort
		  CInventoryObj * brokenInventoryVar = NULL;
		  if(playerObjectData->m_currentInventory->ApplyWearSpecific(12,&brokenInventoryVar,1))
		  {//somthing in inventory just broke
			m_sharedInterfaceRef->SendTextMsg(messageFromDPID,
					                          brokenInventoryVar->m_itemData->m_itemName + " just broke!",		
											  playerObjectData);
			if(brokenInventoryVar->m_itemData->m_destroyOnBreak)
			{//remove item from inventory and broadcast inv change (disarm)
				m_sharedInterfaceRef->BroadcastInventoryChange(playerObjectData,-brokenInventoryVar->m_itemData->m_globalID,0,TRUE,brokenInventoryVar->m_itemData);
				playerObjectData->m_currentInventory->DeleteByGLIDBroken(brokenInventoryVar->m_itemData->m_globalID,1);
				weaponLink = NULL;//this has been deleted internally
			}
		  }//end somthing in inventory just broke
	  }//end aquire weapon used
	}//end ai is excempt
	
	
	    //broadcast to all around player that fired
	    //precreatePacket
	    FIRE_STRUCT firePacket;
	    DWORD messageSize			= sizeof(FIRE_STRUCT);
		firePacket.m_catagorie		= 3;
		firePacket.lookatPosX		= (short)formatedMessage->lookatPosX;
		firePacket.lookatPosY		= (short)formatedMessage->lookatPosY;
		firePacket.lookatPosZ		= (short)formatedMessage->lookatPosZ;
		firePacket.idRefrence		= playerObjectData->m_networkAssignedID;
		firePacket.fireQueIndex		= formatedMessage->fireQueIndex;
        
		CAxList * channelList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(playerObjectData->m_currentWorldID);//does a query for only objects on the specified channel/zone
		
		if(channelList)
	    for(CAxLinkObj * firePos = channelList->GetHeadPosition_unsafe();firePos != NULL;)
	    {//player loop
           CPlayerObject * pData = (CPlayerObject *)channelList->GetNext_unsafe(&firePos);
           if(!pData->m_inGame)
			  continue;
		   
		   if(pData->m_currentID == 0)
			  continue;
		   
		   if(pData->m_aiControlled)
			  continue;//ai server does not need these messages
		   if(pData->m_networkAssignedID != playerObjectData->m_networkAssignedID)
				 if(m_sharedInterfaceRef->m_cabLib.BoundingSphereCheck((float)m_sharedInterfaceRef->m_netComObject->m_serverCalculateRadius,0.0f,
					                             playerObjectData->m_3dPosition.x,playerObjectData->m_3dPosition.y,playerObjectData->m_3dPosition.z,
											     pData->m_3dPosition.x,pData->m_3dPosition.y,pData->m_3dPosition.z))
				 {//bounding check
					 if(pData->m_firemsgsoutbox->GetCount() < 20)
					 {
					    pData->m_firemsgsoutbox->AddPacket(messageSize,(BYTE*)&firePacket,pData->m_currentID,pData->m_currentWorldID);
					 }
				 }
	   }//end player loop
	   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	   return TRUE;
}
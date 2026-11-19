#include "StdAfx.h"
#include "networkgamelogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


NetworkGameLogic::NetworkGameLogic(CMultiplayerObj * mObj)
{
     m_mNObjectRef							= mObj;
	long curTimeInit						= GetTickCount();
	m_useItemTimeStamp						= curTimeInit;
	m_durationToServerStamp					= curTimeInit;
	m_durationToServer						= 2500;
}

NetworkGameLogic::~NetworkGameLogic(void)
{


}
//------------------------------------------------------------------------------------//
BOOL NetworkGameLogic::PlayCharacterOnAccount(int index,
											  CMultiplayerObj * mNObject,
											  int & localContentClientVersion)
{
  if(index < 0)
	 return FALSE;

  if(!mNObject->GetEnabledClient_safe())
      return FALSE;

      SendAttribMsg(DPNSEND_GUARANTEED,
			        DPID_SERVERPLAYER,
			        9,
			        index,
			        0,
			        localContentClientVersion,
					15,//VERSION EXPLOIT
					NULL);

	  mNObject->m_currentCharacterInUse = index;//pending though

      return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL NetworkGameLogic::SendAttribMsg(DWORD flags,
									 DPNID sendTo,
									 int attribute,
									 int uniInt,
									 long refrenceID,
								 	 int uniInt2,
									 short uniShort3,
									 CPlayerObject * runtimePlOption) 
{
    LPATTRIB_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(ATTRIB_STRUCT);
    lpChatMessage = (LPATTRIB_STRUCT) new BYTE[messageSize];

	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie = 6;
	lpChatMessage->attribute   = attribute;
	lpChatMessage->uniInt      = uniInt;
    lpChatMessage->idRefrence  = (short)refrenceID;
	lpChatMessage->uniInt2     = uniInt2;
	lpChatMessage->uniShort3   = uniShort3;
	lpChatMessage->CRCcheck    = m_mNObjectRef->GetAttribCRC(lpChatMessage);
    
	
    BOOL storeIt = TRUE;
	if(runtimePlOption)
	{
	   if(runtimePlOption->m_aiControlled)
		  storeIt = FALSE;
	}
	else
	   storeIt = FALSE;
    
	if(sendTo == DPID_SERVERPLAYER)
	{
	   if(m_mNObjectRef->m_attributeOutBox->GetCount() < MAXATTRIBUTESEND)
			m_mNObjectRef->m_attributeOutBox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,0);
	}
	else if(storeIt)
       runtimePlOption->m_attributeOutbox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,runtimePlOption->m_currentWorldID);
	else
       m_mNObjectRef->SendData((BYTE *)lpChatMessage,
				                  messageSize,
				                  flags,
					              sendTo,
								  m_mNObjectRef->m_packetTimeout);
    

	if (lpChatMessage){
		delete lpChatMessage;
		lpChatMessage=0;
	}

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL NetworkGameLogic::SendCommandMsg(  int msgType,
										int miscInt,
										int networkAssignedID)
{

	COMMAND_DATA dataPacket;
	int messageSize = sizeof(COMMAND_DATA);
    dataPacket.m_catagorie = 18;
	dataPacket.m_data      = msgType;
	dataPacket.m_miscInt   = miscInt;
	dataPacket.m_networkID = networkAssignedID;

    m_mNObjectRef->SendData((BYTE *)&dataPacket,
				                          messageSize,
				                          DPNSEND_GUARANTEED,
				                          NULL,
										  m_mNObjectRef->m_packetTimeout);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		NetworkGameLogic
// Function:	SendTextMsg(DWORD, DPNID, CString, int, CPlayerObject*, int)
// Description:	This is used to encode regular text chat messages as packets
//				for transmission to the server application.
///////////////////////////////////////////////////////////////////////////////

/*
BOOL 
NetworkGameLogic::SendTextMsg(DWORD iFlags,
							  DPNID iSendTo,
							  CString strMessage,
							  int iReferenceID,
							  CPlayerObject *pPlayer,
							  int iCategory)
{
    const unsigned char MAX_MESSAGE_SIZE = 250;
	int	iPacketSize = 0,
		iMessageSize = strMessage.GetLength();

	// Constrain the size of packets
	if((iMessageSize < 0) || (iMessageSize > MAX_MESSAGE_SIZE))return FALSE;

	// Allocate memory for message content + header
	iPacketSize = sizeof(TXT_MSG) + iMessageSize;
    BYTE *pPacket = new BYTE[iPacketSize];
	if(pPacket == NULL) return FALSE;

	// Encode reference ID
	// BYTE *shortConv = (BYTE*)iReferenceID;

	//*shortConv ??? not being used
	//WARNING: C4312 --> conversion from int to BYTE*, of greater size
	
	// Create packet header
	LPTXT_MSG castEntry = (LPTXT_MSG)pPacket;
	castEntry->m_catagorie = iCategory;
	castEntry->referenceID = iReferenceID;

	// Encode text message into packet body
    for(int i = 0; i < iMessageSize; i++)
		pPacket[i + 4] = strMessage.GetAt(i);

	// Add packet to appropriate message queue
	if(iSendTo == DPID_SERVERPLAYER)
	{
		if(m_mNObjectRef->m_txtOutBox->GetCount() < 5)
			m_mNObjectRef->m_txtOutBox->AddPacket(iPacketSize,(BYTE*)pPacket,0,0);
	}
	else if(pPlayer)
	{
		if(pPlayer->m_txtMsgOutbox->GetCount() < 20)
			pPlayer->m_txtMsgOutbox->AddPacket(iPacketSize,(BYTE*)pPacket,0,pPlayer->m_currentWorldID);
	}

	// Packet has been sent; release memory on the heap
	if (pPacket)
	{
		delete [] pPacket;
	    pPacket=0;
	}
	return TRUE;
}
*/
///////////////////////////////////////////////////////////////////////////////
// Class:		NetworkGameLogic
// Function:	SendTextMsg(DWORD, DPNID, int, CPlayerObject*, int, CString, CMDID)
// Description:	This function handles packaging a command string issued
//				through text chat as a packet to send to the server application
//				for further processing.
///////////////////////////////////////////////////////////////////////////////

BOOL 
NetworkGameLogic::SendTextMsg(DWORD iFlags,
							  DPNID iSendTo,
							  int iReferenceID,
							  CPlayerObject *pPlayer,
							  int iCategory,
							  CString strArgs,
							  CMDID iCommandID)
{
	const unsigned char MAX_ARGS_SIZE = 250;
	int	iIndex = 0,
		iMessageSize = 0,
		iArgsSize = strArgs.GetLength();

	// Constrain the size of packets
	if((iArgsSize < 0) || (iArgsSize > MAX_ARGS_SIZE)) return FALSE;

	// Allocate memory for message content + header
	iMessageSize = sizeof(TXT_MSG) + sizeof(iCommandID) + iArgsSize;
    BYTE *pMessagePacket = new BYTE[iMessageSize];
	if(pMessagePacket == NULL) return FALSE;

	// Create packet header
	LPTXT_MSG castEntry = (LPTXT_MSG)pMessagePacket;
	castEntry->m_catagorie = iCategory;
    castEntry->referenceID = iReferenceID;
	iIndex += sizeof(TXT_MSG);

	// Encode command ID into packet body
	pMessagePacket[iIndex] = iCommandID;
	iIndex += sizeof(iCommandID);

	// Encode argument string into packet body
    for(int i = 0; i < iArgsSize; i++)
        pMessagePacket[i + iIndex] = strArgs.GetAt(i);

	// Add packet to appropriate message queue
	if(iSendTo == DPID_SERVERPLAYER)
	{
		if(m_mNObjectRef->m_txtOutBox->GetCount() < 5)
			m_mNObjectRef->m_txtOutBox->AddPacket(iMessageSize, (BYTE*)pMessagePacket, 0, 0);
	}
	else if(pPlayer)
	{
		if(pPlayer->m_txtMsgOutbox->GetCount() < 20)
			pPlayer->m_txtMsgOutbox->AddPacket(iMessageSize, (BYTE*)pMessagePacket, 0, pPlayer->m_currentWorldID);
	}

	// Packet has been sent; release memory on the heap
	if(pMessagePacket)
	{
		delete[] pMessagePacket;
	    pMessagePacket=0;
	}
	return TRUE;
}

//------------------------------------------------------------------------------------
void NetworkGameLogic::UseItemByGLID(int glID,CMovementObjList * entObjRuntime)
{
	if(!m_mNObjectRef->GetEnabledClient_safe())
    return;

	long currentTime = GetTickCount();
     if((currentTime-m_useItemTimeStamp) > 1000)
	 {

	    POSITION posLoc = entObjRuntime->FindIndex(0);
        if(!posLoc)
	    return;
	    CMovementObj * mainPtr = ( CMovementObj* ) entObjRuntime->GetAt(posLoc);

	    if(mainPtr->List_controlType != 0)//user?
		   return;

	    if(m_mNObjectRef->m_requestingPortal == TRUE)
		   return;

	    if(mainPtr->List_networkDefinedID < 1)//has no way of defining who he is
		   return;
		 
		 
		 m_mNObjectRef->SendAttribMsg(DPNSEND_GUARANTEED,
				                      DPID_SERVERPLAYER,
									  27,//use item
									  glID,
									  mainPtr->List_networkDefinedID,//myNetworkID,
									  0,
									  0,
									  NULL);
		 m_useItemTimeStamp = currentTime;
	 }
}
//--------------send message----------------------------------------------------------//
BOOL NetworkGameLogic::SendEquipMsg(int index,
						      int refrenceID,
							  CPlayerObject * playerObjectData,
							  int catagorie,
							  int dupCount) 
{
    LPEQUIP_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(EQUIP_STRUCT);
    lpChatMessage = (LPEQUIP_STRUCT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie= catagorie;
	lpChatMessage->index      = index;
    lpChatMessage->idRefrence = (short)refrenceID;
    lpChatMessage->CRCcheck   = m_mNObjectRef->GetEquipCRC(lpChatMessage);
	lpChatMessage->dupCount   = dupCount;
	
    

	//newPobj->m_equipMsgOutbox
    if(!playerObjectData)
	{
       /*GL_mutiplayerObject->SendData((BYTE *)lpChatMessage,
				                             messageSize,
				                             flags,
					                         sendTo,
										     GL_mutiplayerObject->m_packetTimeout);*/
	    if(m_mNObjectRef->m_equipOutBox)
	    if(m_mNObjectRef->m_equipOutBox->GetCount() < 20)
	      m_mNObjectRef->m_equipOutBox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,0);
	}
	else if(playerObjectData){
	   if(playerObjectData->m_equipMsgOutbox->GetCount() < 30)
       playerObjectData->m_equipMsgOutbox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,playerObjectData->m_currentWorldID);
	}


	if (lpChatMessage){
		delete lpChatMessage;
		lpChatMessage=0;
	}

	return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL NetworkGameLogic::SendFireMsgCP(   DWORD flags,
										D3DVECTOR lookat,
										short firQueIndex,
										DPNID sendTo,
										long refrenceID) 
{
	
	
	LPFIRE_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(FIRE_STRUCT);
    lpChatMessage = (LPFIRE_STRUCT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie     = 3;
	lpChatMessage->lookatPosX      = (short)lookat.x;
	lpChatMessage->lookatPosY      = (short)lookat.y;
	lpChatMessage->lookatPosZ      = (short)lookat.z;
    lpChatMessage->idRefrence      = (short)refrenceID;
	lpChatMessage->fireQueIndex    = firQueIndex;
	lpChatMessage->indexer         = 0;//mG_status->m_multiplayerObject->m_localCLSecurityIndexer;
    
	
	m_mNObjectRef->SendData((BYTE *)lpChatMessage,
				                    messageSize,
				                    flags,
					                sendTo,
									m_mNObjectRef->m_clientImediatePacketTimeout);
	

	if (lpChatMessage){
		delete lpChatMessage;
		lpChatMessage;
	}

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL NetworkGameLogic::SendFireMsg(DWORD flags,
										D3DVECTOR lookat,
										short firQueIndex,
										DPNID sendTo,
										long refrenceID) 
{
    if(m_mNObjectRef->m_fireOutBox->GetCount()>2)
	   return FALSE;
	
	
	LPFIRE_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(FIRE_STRUCT);
    lpChatMessage = (LPFIRE_STRUCT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}
    
	lpChatMessage->m_catagorie     = 3;
	lpChatMessage->lookatPosX      = (short)lookat.x;
	lpChatMessage->lookatPosY      = (short)lookat.y;
	lpChatMessage->lookatPosZ      = (short)lookat.z;
    lpChatMessage->idRefrence      = refrenceID;
	lpChatMessage->fireQueIndex    = firQueIndex;
    	
	m_mNObjectRef->m_fireOutBox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,0);

	if (lpChatMessage){
		delete lpChatMessage;
		lpChatMessage;
	}

	return TRUE;
}
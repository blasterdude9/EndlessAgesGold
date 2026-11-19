#pragma once
#include "matrixarb.h"
#include "CCameraClass.h"
#include "Viewportclass.h"
#include "dxdeviceclass.h"
#include "CMenu3DClass.h"
#include "CMultiplayerClass.h"
#include "CMovementModel.h"
#include "networkgamelogic.h"
#include "CABFunctionLib.h"
#include "CtextReadoutClass.h"
#include "CFilterSpeechClass.h"
#include "CTextRenderClass.h"
#include "CWaterZoneClass.h"
#include "CEnvironmentClass.h"
#include "AIclass.h"
#include "GlobalDefines.h"
#include "CNetworkCommands.h"


class NetworkGameLogic
{
public:
	NetworkGameLogic(CMultiplayerObj * mObj);
	~NetworkGameLogic(void);

	//Variables
	CMultiplayerObj * m_mNObjectRef;
    long m_useItemTimeStamp;
	long m_durationToServerStamp;
	long m_durationToServer;


    //Functions
	BOOL PlayCharacterOnAccount(int index,CMultiplayerObj * mNObject,int & localContentClientVersion);
	BOOL SendAttribMsg( DWORD flags,
						DPNID sendTo,
						int attribute,
						int uniInt,
						long refrenceID,
						int uniInt2,
						short uniShort3,
						CPlayerObject * runtimePlOption);
	BOOL SendCommandMsg(int msgType,
						int miscInt,
						int networkAssignedID);
	/*BOOL SendTextMsg(DWORD iFlags,
					 DPNID iSendTo, // Can be NULL if client
					 CString strMessage,
					 int iReferenceID,
					 CPlayerObject *pPlayer,
					 int iType);*/
	BOOL SendTextMsg(DWORD iFlags,
					 DPNID iSendTo,
					 int iReferenceID,
					 CPlayerObject *pPlayer,
					 int iType,
					 CString strArgs,
					 CMDID iCommandID = SCMD_UNKNOWN);
	void UseItemByGLID(int glID,CMovementObjList * entObjRuntime);
	BOOL SendEquipMsg(  int index,
						int refrenceID,
						CPlayerObject * playerObjectData,
						int catagorie,
						int dupCount);
	BOOL SendFireMsgCP( DWORD flags,
						D3DVECTOR lookat,
						short firQueIndex,
						DPNID sendTo,
						long refrenceID) ;
	BOOL SendFireMsg(DWORD flags,
					D3DVECTOR lookat,
					short firQueIndex,
					DPNID sendTo,
					long refrenceID);

};

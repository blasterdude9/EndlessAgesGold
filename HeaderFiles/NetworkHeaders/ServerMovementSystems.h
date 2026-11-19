#pragma once
#include "sharednet.h"

class CServerMovementSystems
{
public:
	CServerMovementSystems(CSharedNet * sharedInterface);
	~CServerMovementSystems(void);


	//Variables
	CSharedNet * m_sharedInterfaceRef;



	//Functions
	void InterpolateNetworkEntiesServerSide(CPlayerObject * pData,DWORD & CurrentTickCount);
};

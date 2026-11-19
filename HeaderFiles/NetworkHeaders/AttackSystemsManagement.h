#pragma once
#include "sharednet.h"

class CAttackSystemsManagement
{
public:
	CAttackSystemsManagement(CSharedNet * sharedInterface);
	~CAttackSystemsManagement(void);


    //Variables
	CSharedNet * m_sharedInterfaceRef;


	//Functions
    BOOL HandleFireMessagesServerSide_safe(LPFIRE_STRUCT formatedMessage,DPNID messageFromDPID);

};

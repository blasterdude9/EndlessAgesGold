#include "CAxList.h"
#pragma once

class CCharacterPendingObj
{
public:
	CCharacterPendingObj(void){};
	~CCharacterPendingObj(void){};

	CString m_username;
	int m_charIndex;
	void * m_objectOption;
};

class CCharacterPendingLoader : public CAxList
{
public:
	CCharacterPendingLoader(void);
	~CCharacterPendingLoader(void);
	//Vars
	CRITICAL_SECTION m_sect;
    //Functions
    bool AddCharacterToPendingList(CString username,int charIndex);
	bool GetNextCharacterToLoad(CCharacterPendingObj ** retPtr);

};

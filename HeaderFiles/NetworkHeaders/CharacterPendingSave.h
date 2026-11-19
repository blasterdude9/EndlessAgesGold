#pragma once
#include "CDplayPlayerClass.h"


class CCharacterPendingSaveObj : public CObject
{
public:
	CCharacterPendingSaveObj(void){playerRef = NULL;m_logOff=0;};
	~CCharacterPendingSaveObj(void);
    
    CPlayerObject * playerRef;
	int m_logOff;
	CString m_text;
	int m_optionalHandle;

	
};


class CCharacterPendingSave : public CObList
{
public:
	CCharacterPendingSave(void);
	~CCharacterPendingSave(void);


	bool AddCharacterToPendingList(CPlayerObject * playerRef,int logOff,CString text,int handleOverride);
    bool GetNextCharacterToLoad(CCharacterPendingSaveObj ** retPtr);
    

	CRITICAL_SECTION m_cSection;
};

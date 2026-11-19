#pragma once
#include "CSkeletalClass.h"

class CSelectionEffect : public CObject
{
	DECLARE_SERIAL( CSelectionEffect );
public:
	CSelectionEffect(void);
	~CSelectionEffect(void);

    CSkeletonObject * m_skeletalPtr;
	BOOL              m_usedThisRound;
	CString           m_effectName;

	//functions
	void Serialize( CArchive& ar );	
};

class CSelectionEffectList : public CObList
{
	DECLARE_SERIAL( CSelectionEffectList );
public:
	CSelectionEffectList(void){};
public:
	~CSelectionEffectList(void)
	{
		for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
            CSelectionEffect * sPtr = (CSelectionEffect *)GetNext(posLoc);
			delete sPtr;
			sPtr = 0;
		}
		RemoveAll();
	}

	void Reset()
	{
        for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
            CSelectionEffect * sPtr = (CSelectionEffect *)GetNext(posLoc);
			sPtr->m_usedThisRound = FALSE;
		}
	}

	CSelectionEffect * GetByIndex(int index)
	{
        if(index < 0)
		   return NULL;

        POSITION posLoc = FindIndex(index);
		if(!posLoc)
			return NULL;
    
		CSelectionEffect * sPtr = (CSelectionEffect *)GetAt(posLoc);
		return sPtr;
	}

    
};



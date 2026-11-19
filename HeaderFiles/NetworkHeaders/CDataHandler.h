#pragma once
#include "dplay8.h"

class CCDataHandler : public CObject
{
public:
	CCDataHandler(void);
	~CCDataHandler(void);

    //Vars
    CString 	    m_text;
	CString         m_text2;
	int             m_int;
	int             m_int2;
	int             m_int3;
	DWORD           m_dword;
	DWORD           m_dword2;
	int             m_type;
	DPNID           m_dpnid;
	int             m_bufSz;
	byte          * m_miscData;
	DWORD           m_timeAdded;
	DWORD			m_durationTillExecution;

	//Functions
};



class CCDataHandlerDB : public CObList
{
public:
	CCDataHandlerDB(void){InitializeCriticalSection(&m_cSection);m_queCount=0;};
	~CCDataHandlerDB(void)
	{
        for(POSITION posDel = GetHeadPosition();posDel != NULL;)
		{
            CCDataHandler * dataHandler = (CCDataHandler *)GetNext(posDel);
			delete dataHandler;
			dataHandler = 0;
		}
		RemoveAll();

		DeleteCriticalSection(&m_cSection);
	};

    //Vars
    CRITICAL_SECTION m_cSection;
	long             m_queCount;
	//Functions
	bool AddToQue( DWORD dwordType, 
				   CString text, 
				   int intMisc, 
				   DPNID dpnidMisc, 
				   int type, 
				   byte * data, 
				   int bufSize);
	bool AddToQue( DWORD dwordType,
				   CString text,
				   int intMisc, 
				   DPNID dpnidMisc, 
				   int type, 
				   byte * data, 
				   int bufSize,
				   DWORD dwordType2,
				   int intMisc2,
				   int intMisc3,
				   CString text2);
	bool AddToQue( int miscInt,
				   int miscInt2,
				   int type, 
				   DWORD delay);
	bool CheckForDuplicate(CString & text, int & type);
	bool GetNextObject(CCDataHandler ** dataObj);
	long GetQueCount();

};

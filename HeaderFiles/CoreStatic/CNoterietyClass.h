#include "matrixarb.h"
#ifndef CNOTERIETYOBJCLASS_H
#define CNOTERIETYOBJCLASS_H

class   CAggressorObj : public CObject
{
public:
    CAggressorObj();

	void Clone(CAggressorObj ** clone);
    
	int		m_wldID;
    int		m_handle;
    long	m_timeStamp;
	int		m_curDamageDone;
};

class   CAggressorObjList : public CObList
{
public:
   CAggressorObjList()
   {
       m_agressorTimeout					= 350000;
	   m_maxAgressors						= 10;
   };

   ~CAggressorObjList()
   {
       for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	   {
           CAggressorObj * aPtr = (CAggressorObj *)GetNext(posLoc);
		   delete aPtr;
		   aPtr=0;
	   }
	   RemoveAll();
   };

   void Flush()
   {
       for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	   {
           CAggressorObj * aPtr = (CAggressorObj *)GetNext(posLoc);
		   delete aPtr;
		   aPtr=0;
	   }
	   RemoveAll();
   }

   //vars
   DWORD			m_agressorTimeout;
   int              m_maxAgressors;
   //Functions
   BOOL UpdateAgressorState(int handle, int damageAmount, int & channel);
   BOOL LootDamageBasedAccessCheck(int looterHandleOrGroup);
   void Clone(CAggressorObjList ** clone);
   void CacheTimedOutAgressors(long timeout);
   BOOL DoesHandleExist(int & handle,long timeout);
   void FilterByWldID(int id);
};

class   CNoterietyObj : public CObject
{
	
     
	DECLARE_SERIAL( CNoterietyObj );
	
    
public:
    CNoterietyObj();
	~CNoterietyObj()
	{
		if(m_currentAgressors)
		{
		   delete m_currentAgressors;
		   m_currentAgressors = 0;
		}
	}
    
	int                 m_currentMurders;
	long                m_currentMinuteCountSinceLastMurder;
	int					m_murderUpdateTick;
	CAggressorObjList * m_currentAgressors; 
	
    
	void Clone(CNoterietyObj ** clone);
	void Serialize( CArchive& ar );
	void SerializeAlloc( CArchive& ar );

};


#endif
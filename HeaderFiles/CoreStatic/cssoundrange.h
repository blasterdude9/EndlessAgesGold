
#ifndef CSSOUNDRANGE_H
#define CSSOUNDRANGE_H
#include "CSoundManagerClass.h"

//Base skeleton Class
class CSoundRangeObject : public CObject
{
	
     
    DECLARE_SERIAL( CSoundRangeObject )
    
    
public:
	
	CSoundRangeObject();
    long                  m_startRange;
	long                  m_endRange;
	BOOL                  m_inRange;
	int                   m_id;
	CString               m_fileName;
	float                 m_soundFactor;
	float                 m_radiusClipping;
	BOOL                  m_velocityBased;
	BOOL                  m_looping;
	CSoundSampleObj      *m_soundRef;

	void                  SafeDelete();
	void                  Clone(CSoundRangeObject ** clone);
	void                  Serialize( CArchive& ar );
    void                  SetRanges(long start, long end);
};

class CSoundRangeObjectList : public CObList
{
public:
   void SafeDelete();
   void Clone(CSoundRangeObjectList ** clone);
   CSoundRangeObjectList(){};
    
     
    DECLARE_SERIAL( CSoundRangeObjectList )
    
    
};

#endif
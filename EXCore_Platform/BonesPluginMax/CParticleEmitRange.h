#include "EnvParticleSystem.h"
#ifndef CPARTICLEEMITRANGE_H
#define CPARTICLEEMITRANGE_H



class  CParticleEmitRangeObj : public CObject
{
	
     
	DECLARE_SERIAL( CParticleEmitRangeObj );
	
    
public:
	
	CParticleEmitRangeObj();
   
	BOOL                   m_useExactRange;
    int                    m_animationQue;
	int                    m_startRange;
	int                    m_endRange;
	int                    m_particleDBIndex;
	BOOL                   m_inRange;
	int                    m_boneLink;
	EnvParticleSystem     *m_particleEmmiter;


	void                  SafeDelete();
	void                  Clone(CParticleEmitRangeObj ** clone);
	void                  Serialize( CArchive& ar );
    void                  SetRanges(long start, long end);
};

class  CParticleEmitRangeObjList : public CObList
{
public:
   void SafeDelete();
   void Clone(CParticleEmitRangeObjList ** clone);
   CParticleEmitRangeObjList(){};
   
     
	DECLARE_SERIAL( CParticleEmitRangeObjList );
	
    
};

#endif
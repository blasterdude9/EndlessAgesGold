#include "matrixarb.h"
#ifndef CSAFEZONES_H
#define CSAFEZONES_H

class   CSafeZoneObj : public CObject
{
	
     
	DECLARE_SERIAL( CSafeZoneObj );
	
    
public:
    CSafeZoneObj();
    
	
	CString                    m_zoneName;
    int                        m_worldID;
	D3DVECTOR                  m_boundingBoxMax;
	D3DVECTOR                  m_boundingBoxMin;
	BOOL                       m_rebirthableZone;

	void Serialize( CArchive& ar );
	
};

class   CSafeZoneObjList : public CObList
{
public:
   CSafeZoneObjList(){InitializeCriticalSection(&m_section);};
   ~CSafeZoneObjList(){DeleteCriticalSection(&m_section);};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CSafeZoneObjList );
	
    
     CRITICAL_SECTION m_section;
	  MatrixARB                  GL_matrixFunctions;
	 CSafeZoneObj * GetSafeZoneByPosition(D3DVECTOR & position,int worldID);
	 BOOL BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max);
};

#endif
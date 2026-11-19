#include "CAdvancedVertexClass.h"

#ifndef CCONTAINMENTMENT_H
#define CCONTAINMENTMENT_H

class  CContainmentObject : public CObject
{
	
     
    DECLARE_SERIAL( CContainmentObject )
    
    
public:
	
	CContainmentObject();
	
	int            m_containmentBasis;
	int            m_miscType;
	long           m_respawnTime;
	long           m_lastRespawnTime;
	int            m_currentAmount;
	int            m_maxAmount;
	int            m_dispersalAmount;
	
    int            UseIt();
	void           Clone(CContainmentObject ** copy);
	void           Serialize( CArchive& ar );

};

class  CContainmentObjectList : public CObList
{
public:
   CContainmentObjectList(){};
   void SafeDelete();
    
     
    DECLARE_SERIAL( CContainmentObjectList )
    
    
};

#endif
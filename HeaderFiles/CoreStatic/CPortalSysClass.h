#include "CStructuresMisl.h"
#ifndef CPORTALSYS_H
#define CPORTALSYS_H



class   CPortalSysObj : public CObject
{
	
     
	DECLARE_SERIAL( CPortalSysObj );
	
    
public:
    CPortalSysObj();
    

	CString    m_portalName;
    int        m_keyRequired;//0=no key required
    //location
	int        m_targetWorld;
	D3DVECTOR  m_wldPosition;
	//area
	ABBOX      m_portalZone;
	int        m_zoneWldID;

	 BOOL BoundBoxPointCheck(D3DVECTOR & point,ABBOX & box);
	BOOL InZone(D3DVECTOR currentLocation,int curWorld);
	void Serialize( CArchive& ar );
	
};

class   CPortalSysList : public CObList
{
public:
	CPortalSysList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CPortalSysList );
	
    

	CPortalSysObj * ReturnPortal(D3DVECTOR currentPosition,int curWorld);
};

#endif
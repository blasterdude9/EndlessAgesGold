#include "dplay8.h"
#ifndef CCLIENTOBJCLASS_H
#define CCLIENTOBJCLASS_H

class CClientObj : public CObject
{
   
	//DECLARE_SERIAL( CClientObj );


public:
    CClientObj();

	
    
	DPNID        m_dpnidCom;
	BYTE *       m_packetBuild;
	long         m_finalPacketSize;
	long         m_currentAmountDownloaded;
	
	//void Serialize( CArchive& ar );
	void SafeDelete();
	
};

class CClientObjList : public CObList
{
public:
   CClientObjList(){};
  
   //DECLARE_SERIAL( CPacketObjList );
   void SafeDelete();
   void LogOut(DPNID dpnid);
   CClientObj * GetObjByDPNID(DPNID dpnid);
};

#endif
#include "dplay8.h"
#ifndef CPACKETQUECLASS_H
#define CPACKETQUECLASS_H



class CPacketObj : public CObject
{
   
	DECLARE_SERIAL( CPacketObj );


public:
    CPacketObj();
	void SafeDelete();
    
	BYTE *       m_data;
	int          m_size;
	DPNID        m_communicationID;
  
	void Serialize( CArchive& ar );
	
};

class CPacketObjList : public CObList
{
public:
   CPacketObjList(){};
   void AddPacket(int size,BYTE * data,DPNID id);
   void SafeDelete();
  
  
	DECLARE_SERIAL( CPacketObjList );


};

#endif
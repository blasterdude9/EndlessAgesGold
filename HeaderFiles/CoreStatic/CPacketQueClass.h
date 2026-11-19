#include "dplay8.h"
#ifndef CPACKETQUECLASS_H
#define CPACKETQUECLASS_H



class CPacketObj : public CObject
{
    
     
	DECLARE_SERIAL( CPacketObj );
	
    

public:
    CPacketObj();
	~CPacketObj();
    
	BYTE *       m_data;
	int          m_size;
	int          m_channelID;
	DPNID        m_communicationID;
  
	void Serialize( CArchive& ar );
	
};

class CPacketObjList : public CObList
{
public:
   CPacketObjList(){m_criticalSectionOption=NULL;m_criticalSectionEnabled=FALSE;};
   ~CPacketObjList();
   CRITICAL_SECTION * m_criticalSectionOption;
   BOOL m_criticalSectionEnabled;

   void AddPacket(int size,BYTE * data,DPNID id,int channelID);
   void PurgeAllByID(DPNID id,BOOL logPurged);
   void SendToTextFile();

   CString ConvToString(int number);
   CString ConvLongToString(DWORD number);
   int GetCountBaseOnChannel(int channel);
   int GetDataSizeByChannel(int channel,int buffer);
   BOOL EnableCriticalSectionUse();
   void EnterCriticalSectionEXA();
   void LeaveCriticalSectionEXA();
   CPacketObj * GetOldestPacket();
   int GetCountProtected();
   BOOL GetNextPacket_safe(int * size,BYTE ** data,DPNID * id);
   void Flush(int cnt=0);
    
   DECLARE_SERIAL( CPacketObjList );

};

#endif
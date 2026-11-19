#pragma once
#include <winsock2.h>

#define ACCOUNT_BUFFER_MAX_SIZE 50

struct AuthPacket
{
	char type;
    char username[ACCOUNT_BUFFER_MAX_SIZE];
    char password[ACCOUNT_BUFFER_MAX_SIZE];
};

struct SignalPacket
{
	char type;
};

struct Genheader
{
	char type;
	long longType;
	short shortType;
};

struct fileCRC
{
	char type;
	long length;
	long crc;
	short fileIndex;
};

class CAXSsPacket : public CObject
{
    public:
	CAXSsPacket(){};
    ~CAXSsPacket()
	{
	   if(data)
	   if(length > 0)
	      delete [] data;
	   data = 0;
	   length = 0;
	};

    //vars
    BYTE * data;
	int    length;
	SOCKET socketWS;
	int    connectionId;
};

class CAXSsPacketQue : public CObList
{
    public:
		CAXSsPacketQue(){InitializeCriticalSection(&m_section);}
		~CAXSsPacketQue()
		{
			EnterCriticalSection(&m_section);
			for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
			{
                CAXSsPacket * pcObj = (CAXSsPacket *)GetNext(posLoc);
				delete pcObj;
				pcObj = 0;
			}
			RemoveAll();
			LeaveCriticalSection(&m_section);
			DeleteCriticalSection(&m_section);
		    
		}

	//VARS
	CRITICAL_SECTION m_section;
	//Functions
	bool GetNextPacket(CAXSsPacket ** packet)
	{
	     EnterCriticalSection(&m_section);
         POSITION posHead = GetHeadPosition();
		 if(posHead)
		 {
            *packet = (CAXSsPacket *)GetAt(posHead);
			 RemoveAt(posHead);
			 LeaveCriticalSection(&m_section);
			 return true;
		 }
	     LeaveCriticalSection(&m_section);
		 return false;
	}
	bool AddPacket(BYTE * data, int length, SOCKET socketLoc, int connectionID)
	{
         if(length < 1)
		    return false;

		 CAXSsPacket * packet = new CAXSsPacket();
		 packet->connectionId = connectionID;
	     packet->socketWS = socketLoc;
		 packet->data = new BYTE[length+1];
		 packet->data[length] = 0;
		 CopyMemory(packet->data,data,length);
		 packet->length = length;
	     EnterCriticalSection(&m_section);
		 AddTail(packet);
	     LeaveCriticalSection(&m_section);
		 return true;
	}
};
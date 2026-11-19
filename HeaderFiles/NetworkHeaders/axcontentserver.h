#include "AXSocketServer.h"
#include "DBConnectionSystem.h"
#include "CABFunctionLib.h"
#include "CServerEventLogClass.h"
#include "axencryptionlib.h"

#pragma once

struct fileBlockReq
{
	char type;
    char objectName[25];
    long startPoint;
};

class CActiveConnection : public CObject
{
    public:
	CActiveConnection(){m_virtualIndex=0;m_inUse = false;m_validAfter=GetTickCount();};
	~CActiveConnection(){};
    
	int m_virtualIndex;
	bool m_inUse;
	DWORD m_validAfter;
    


};

class CActiveConnectionDB : public CObList
{
    public:
    CRITICAL_SECTION m_sect;
	CAXSocketServer		            * m_sServerRef;
	int								  m_rotation;

	CActiveConnectionDB(){InitializeCriticalSection(&m_sect);m_sServerRef = NULL;m_rotation=0;};
	~CActiveConnectionDB()
	{
	   EnterCriticalSection(&m_sect);
	   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	   {
           CActiveConnection * acPtr = (CActiveConnection *)GetNext(posLoc);
		   delete acPtr;
		   acPtr = 0;
	   }
	   RemoveAll();
	   LeaveCriticalSection(&m_sect);
	   DeleteCriticalSection(&m_sect);
	};

	void AddConnection(int conIndex)
	{
		EnterCriticalSection(&m_sect);
		CActiveConnection * acPtr = new CActiveConnection();
		acPtr->m_virtualIndex = conIndex;
        AddTail(acPtr);
		LeaveCriticalSection(&m_sect);
	}

	int GetNextConnectionToService()
	{
       //exp
	   DWORD currentTime = GetTickCount();
 
	   EnterCriticalSection(&m_sect);
	   
       POSITION posLoc = FindIndex(m_rotation);
	   if(!posLoc)
	   {
	       posLoc = GetHeadPosition();
		   m_rotation = 0;
	   }

	   while(posLoc)
	   {
			CActiveConnection * acPtr = (CActiveConnection *)GetNext(posLoc);
			if(acPtr->m_inUse == false && currentTime > acPtr->m_validAfter)
			{
				acPtr->m_inUse = true;
				int temp = acPtr->m_virtualIndex;
				LeaveCriticalSection(&m_sect);
				return temp;
			}
			m_rotation++;
	   }
	   
	   LeaveCriticalSection(&m_sect);

       //end exp
	   return -1;
	}

	void FinishWithConnection(int con)
	{
       EnterCriticalSection(&m_sect);
	   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	   {
           CActiveConnection * acPtr = (CActiveConnection *)GetNext(posLoc);
		   if(acPtr->m_virtualIndex == con)
		   {
		      acPtr->m_inUse = false;
		      LeaveCriticalSection(&m_sect);
			  return;
		   }
	   }
	   LeaveCriticalSection(&m_sect);
	}

	void FinishWithConnection(int con,DWORD waitTime)
	{
       EnterCriticalSection(&m_sect);
	   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	   {
           CActiveConnection * acPtr = (CActiveConnection *)GetNext(posLoc);
		   if(acPtr->m_virtualIndex == con)
		   {
		      acPtr->m_inUse = false;
			  acPtr->m_validAfter = GetTickCount() + waitTime;
		      LeaveCriticalSection(&m_sect);
			  return;
		   }
	   }
	   LeaveCriticalSection(&m_sect);
	}

	void RemoveConnection(int conIndex)
	{
	   EnterCriticalSection(&m_sect);
	   POSITION posLast = NULL;
	   for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	   {
           CActiveConnection * acPtr = (CActiveConnection *)GetNext(posLoc);
		   if(acPtr->m_virtualIndex == conIndex)
		   {
		      delete acPtr;
		      acPtr = 0;
			  RemoveAt(posLast);
		   }
	   }
	   LeaveCriticalSection(&m_sect);
	}

};

class CAXFileObject : public CObject
{
	public:
	CAXFileObject()
	{
	   m_initialized = false;
	   m_fileLength = 0;
	   m_data = NULL;
	   m_fileIndex = 0;
	}
	~CAXFileObject()
	{
	   if(m_data)
	      delete [] m_data;
	   m_data = 0;
	   m_fileLength = 0;
	}
    

	CString			  m_publicName;
	CString			  m_actualDataPath;
	BYTE			* m_data;
	long              m_fileLength;
	bool              m_initialized;
	int               m_fileIndex;
};


class CAXFileObjectDB : public CObList
{
	public:
	CAXFileObjectDB(){InitializeCriticalSection(&m_section);}
	
	~CAXFileObjectDB()
	{
		for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
            CAXFileObject * fileObject = (CAXFileObject *)GetNext(posLoc);
			delete fileObject;
			fileObject = 0;
		}
		RemoveAll();
		DeleteCriticalSection(&m_section);
	}
    

	CRITICAL_SECTION m_section;

    bool AddFile(CString name, CString path)
	{
		 EnterCriticalSection(&m_section);
         if(GetFileObjectByNameULK(name))
		 {//watch for a double slip violation
            
            LeaveCriticalSection(&m_section);
			return true;
		 }//watch for a double slip violation

		 int currentIndex = (int)GetCount();
		 CFileException exc;
         CFile fl;
		 BOOL res = fl.Open(path,CFile::modeRead, &exc);
	     if(res == FALSE){
	        return false;
	     }
	  
		 CAXFileObject * newPtr = new CAXFileObject();
		 newPtr->m_actualDataPath = path;
		 newPtr->m_publicName = name;
		 newPtr->m_fileIndex = currentIndex;
         
		 if(newPtr->m_data)
		    delete [] newPtr->m_data;
		 newPtr->m_data = 0;
		 newPtr->m_fileLength = (long)fl.GetLength();
	      if(newPtr->m_fileLength > 0)
		  {
		     newPtr->m_data = new BYTE[newPtr->m_fileLength];
	         fl.Read(newPtr->m_data,newPtr->m_fileLength);
		  }
	     fl.Close();
		 AddTail(newPtr);
		 LeaveCriticalSection(&m_section);
		 return true;
	}

	CAXFileObject * GetFileIndex(int index)
	{
		 int trace = 0;
		 EnterCriticalSection(&m_section);
         for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		 {
             CAXFileObject * mPtr = (CAXFileObject *)GetNext(posLoc);
             LeaveCriticalSection(&m_section);
			 if(trace==index)
			 {
			    return mPtr;
			 }
			 Sleep(0);
			 EnterCriticalSection(&m_section);
             trace++;
		 }
		 LeaveCriticalSection(&m_section);
		 return NULL;
	}

	CAXFileObject * GetFileObjectByName(CString name)
	{
		 EnterCriticalSection(&m_section);
         for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		 {
             CAXFileObject * mPtr = (CAXFileObject *)GetNext(posLoc);
             LeaveCriticalSection(&m_section);
			 if(mPtr->m_publicName.CompareNoCase(name)==0)
			 {
			    return mPtr;
			 }
			 Sleep(0);
			 EnterCriticalSection(&m_section);

		 }
		 LeaveCriticalSection(&m_section);
		 return NULL;
	}

	CAXFileObject * GetFileObjectByNameULK(CString name)
	{
		 
         for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		 {
             CAXFileObject * mPtr = (CAXFileObject *)GetNext(posLoc);
			 if(mPtr->m_publicName.CompareNoCase(name)==0)
			 {
			    return mPtr;
			 }
		 }
		 return NULL;
	}

};


class CAXContentServer
{
public:
	CAXContentServer(void);
	~CAXContentServer(void);

	//Vars
	CAXSocketServer		            * m_sServer;
    CDBConnectionSystem				* m_dbConnection;
	CDBConnectionSystem				* m_authConnection;
	CDBConnectionSystem				* m_axWebConnection;
	CAXFileObjectDB                 * m_dataStorage;
	DWORD							* m_threadIds;
	HANDLE							* m_threadHandles;
	CServerEventList                * m_eventLog;
	
	int                               m_threadCount;
	CABFunctionLib                    m_cabLib;
    BOOL                              m_shutDown;
	CRITICAL_SECTION                  m_cSection;
	int                               m_theadShutDownCount;
	CAXEncryptionLib                * m_encryptor;
	CRITICAL_SECTION                  m_cStreamerSection;
	CObList                         * m_streamerConnectionList;
	CActiveConnectionDB             * m_activeConnections;


	//Functions
    //bool RemoveConnection(CString ip, long socket);
	bool RemoveConnection(int connectionIndex);
	bool AddConnection(CString ip, long socket, int connectionIndex);
    bool AuthenticateConnectionInDB(CString ip, long socket, CString username);
	bool UpdateKBS(long kbs, long socket, CString ip, CString currentFileName, long currentPosition);
	

};

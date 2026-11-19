// Class supports DB hybrid link list indexed database style searching-cross platform
#include <assert.h>
#pragma once

class CAxLinkObj
{
	public:
		CAxLinkObj();
		 ~CAxLinkObj();
	
	    //Variables
	    CAxLinkObj 			* m_prevObject;
	    CAxLinkObj 			* m_nextObject;
		void 			    * m_thisObject;
	    unsigned int        m_dbIndex; 
	    unsigned int        m_uniqueID;
	
		void SetDBIndex(unsigned int index){m_dbIndex = index;}
		void SetDBIndex(unsigned char index){m_dbIndex = (unsigned int)index;}
};

class CAxList
{
	public:
		CAxList();
		 ~CAxList();
	
		// CAxList variables
	    CAxLinkObj * m_listHead;
	    CAxLinkObj * m_currentTemp;
	    CAxLinkObj * m_lastTemp;
	
	    int 		 m_count;
	
	public:
		// CAxList functions
	    void		RemoveAll();
	    void		AddTail(CAxLinkObj * object);
	    void		AddHead(CAxLinkObj * object);
		CAxLinkObj* GetHeadPosition_unsafe();
		void*		GetNext_unsafe(CAxLinkObj** cPtr);
		void*		GetPrev_unsafe(CAxLinkObj** cPtr);
	    CAxLinkObj* FindIndex(int index);
	    void		InsertAfter(CAxLinkObj *pos, CAxLinkObj * object);
	    bool		RemoveAt(unsigned int uniqueID);
		bool		RemoveAt_unsafe(CAxLinkObj * object);
	    CAxLinkObj* GetByUniqueID(unsigned int uniqueID);
		bool		RemoveAt_unsafe(unsigned int uniqueID);
	    int			GetCount();
		bool		UpdateMemory_unsafe(unsigned int uniqueID,void * newMem);

		// Editor UI enumerates these lists single-threaded; ManualLock/UnLock
		// are no-op guards so the _unsafe traversal call sites compile.
		void		ManualLock(){}
		void		ManualUnLock(){}
	
};

class axListHolder
{
	
	public:
	axListHolder()
	{
		m_axList = NULL;
		memset(m_indexedBy,0,50);
	}
	~axListHolder()
	{
		if(m_axList)
		   delete [] m_axList;
		   m_axList = 0;
	}	
	
	char m_indexedBy[50];//description later used to do searches
	CAxList * m_axList;
};

class CAxDB
{
	public:
		CAxDB()
	    {
			m_listArray = NULL;
			InitializeCriticalSection(&m_sect);
		};
		 ~CAxDB()
		{
			 if(m_listArray)
				delete [] m_listArray;
			 m_listArray = 0;
			 DeleteCriticalSection(&m_sect);
		};
	
			
		// CAxDB variables
        unsigned int m_indexedVarCount;
		unsigned int m_cellCount;
		axListHolder * m_listArray;//per index type
		CRITICAL_SECTION m_sect;
		// Functions
		void Lock()
		{
             EnterCriticalSection(&m_sect);
		}

		void Unlock()
		{
             LeaveCriticalSection(&m_sect);
		}

		void ManualLock()   { Lock();   }  // aliases used by editor UI code
		void ManualUnLock() { Unlock(); }

		void Init(unsigned int cellCount,unsigned int indexedVarCount)
	    {
			if(cellCount < 1)
			   cellCount = 1;
			
			if(indexedVarCount < 1)
			   indexedVarCount = 1;
			m_cellCount     	= cellCount;
			m_indexedVarCount 	= indexedVarCount;
			m_listArray = new axListHolder[m_indexedVarCount+1];
			for(unsigned int loop=0;loop<m_indexedVarCount;loop++)
			{
				m_listArray[loop].m_axList = new CAxList[m_cellCount+1];
			}
		};
		
		CAxList *GetVarBasedAXList(char * var)
		{
			for(unsigned int loop=0;loop<m_indexedVarCount;loop++)
			{
				if(strcmp(m_listArray[loop].m_indexedBy,var)==0)
				   return m_listArray[loop].m_axList;
			}
			return NULL;
		}
		bool AddIndexVariable(unsigned int indexPos,char * varName)
		{
			 if(indexPos >= m_indexedVarCount)
			    return false;
			 
			 if(strlen(varName) < 1)
				return false;
			 
			 memcpy(m_listArray[indexPos].m_indexedBy,varName,strlen(varName));
			 return true;
		}
		/*void AddIndexed(CAxLinkObj * object, unsigned int indexID, unsigned int uniqueID,int indexArrayPos)
		{
			 object->SetDBIndex(indexID);
			 object->m_uniqueID = uniqueID;
			 m_listArray[indexArrayPos].m_axList[object->m_dbIndex%m_cellCount].AddHead(object);			
		}*/
        

		bool AddIndexed(void * ptrAny, unsigned int indexID, unsigned int uniqueID, char * searchBasedOnName)
		{
			 CAxList * indexedList = GetVarBasedAXList(searchBasedOnName);
			 if(!indexedList)
				return false;

			 CAxLinkObj * object = new CAxLinkObj();
			
			 object->m_thisObject = ptrAny;
			 object->SetDBIndex(indexID);
			 object->m_uniqueID = uniqueID;
			 indexedList[object->m_dbIndex%m_cellCount].AddHead(object);	
			 return true;			 
		}
		bool RemoveIndexed(unsigned int indexID,unsigned int uniqueID, char * searchBasedOnName)
		{
			 CAxList * indexedList = GetVarBasedAXList(searchBasedOnName);
			 if(!indexedList)
				 return false;
			
			 return indexedList[indexID%m_cellCount].RemoveAt(uniqueID); 
		}

		bool UpdateMemory_unsafe(unsigned int indexID,unsigned int uniqueID, char * searchBasedOnName,void * mem)
		{
			 CAxList * indexedList = GetVarBasedAXList(searchBasedOnName);
			 if(!indexedList)
				 return false;
			
			 return indexedList[indexID%m_cellCount].UpdateMemory_unsafe(uniqueID,mem); 
		}



        bool RemoveIndexed_unsafe(unsigned int indexID,unsigned int uniqueID, char * searchBasedOnName)
		{
			 CAxList * indexedList = GetVarBasedAXList(searchBasedOnName);
			 if(!indexedList)
				 return false;
			
			 return indexedList[indexID%m_cellCount].RemoveAt_unsafe(uniqueID); 
		}

		CAxLinkObj * GetByUniqueID(unsigned int indexID,unsigned int uniqueID)
		{
		    return m_listArray[0].m_axList[indexID%m_cellCount].GetByUniqueID(uniqueID);
		}
		
		CAxLinkObj * GetByUniqueID(unsigned int indexID,unsigned int uniqueID, char * searchBasedOnName)
		{
			CAxList * indexedList = GetVarBasedAXList(searchBasedOnName);
			if(!indexedList)
				return NULL;
			
		    return indexedList[indexID%m_cellCount].GetByUniqueID(uniqueID);
		}
		
		CAxList * GetAllOnIndex(unsigned int indexID, char * searchBasedOnName)
		{
			 CAxList * indexedList = GetVarBasedAXList(searchBasedOnName);
			 if(!indexedList)
				  return NULL;
			 return &indexedList[indexID%m_cellCount];
		}
		bool WriteStatus(char * fileName);

		unsigned int GetSimpleHash(char * charString)
		{
			 int length = strlen(charString);
			 int hash = length;
			 for(int loop=0;loop<length;loop++)
                 hash += toupper(charString[loop]);//this enables all searches by text to be non-case sensitive

			 return hash;
		}


};

//#endif	//_CAXLIST_H_

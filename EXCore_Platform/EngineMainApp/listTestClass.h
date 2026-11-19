
#include "CAxList.h"
#pragma once



class testObj
{
      public:
      testObj(unsigned int id,unsigned int trace, unsigned int channel)
	  {
          m_id				= id;
		  m_trace			= trace;
		  m_channel			= channel;
	  }
	  ~testObj()
	  {


	  }
	 

      unsigned int m_id;
	  unsigned int m_trace;
	  unsigned int m_channel;
};

class testObjList : public CAxDB
{
      public:

		  testObjList()
		  {
              Init(200,3);
              assert(AddIndexVariable(0,"id"));//unique example
              assert(AddIndexVariable(1,"trace"));
              assert(AddIndexVariable(2,"channel"));//non-unique
		  }
		  ~testObjList()
		  {
              //traverse list manually to delete core class objects, the list will clear itself when deconstructed
			  Lock();
			  if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
			     for(int cLoop=0;cLoop<m_cellCount;cLoop++)
			     {
                     for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
					 {
				         testObj * delPtr = (testObj *)m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
						 delete delPtr;
						 delPtr = 0;
					 }
				 }
			  //internally the rest of the lsists should auto clean themselfs
			  Unlock();
		  }
		  CRITICAL_SECTION addDelSect;//section just for add and delete , does not effect searches, db supports simutanious searches without global lock

		  //extended
		  void Add(testObj * object) 
          {
			  EnterCriticalSection(&addDelSect);
                  assert(AddIndexed((void*)object, object->m_id, object->m_id,"id"));//unique
                  assert(AddIndexed((void*)object, object->m_trace, object->m_trace,"trace"));//trace
                  assert(AddIndexed((void*)object, object->m_channel, object->m_id,"channel"));//channel
			  LeaveCriticalSection(&addDelSect);
		  }

		  testObj * GetByID(int id) 
          {
                 CAxLinkObj * obj = GetByUniqueID(id,id,"id");
				 if(obj)
					 return (testObj *)obj->m_thisObject;
				 return NULL;
		  }

		  testObj * GetByTrace(int trace) 
          {
                 CAxLinkObj * obj = GetByUniqueID(trace,trace,"trace");
				 if(obj)
					 return (testObj *)obj->m_thisObject;
				 return NULL;
		  }

		  CAxList * GetResultSetbyChannelTrace(int channel) 
          {
                 CAxList * obj = GetAllOnIndex(channel,"channel");
				 if(obj)
					 return (CAxList *)obj;
				 return NULL;
		  }

		  void DeleteObject(testObj * object)
		  {		  
			   EnterCriticalSection(&addDelSect);
		       assert(RemoveIndexed(object->m_id,object->m_id, "id"));
			   assert(RemoveIndexed(object->m_trace,object->m_trace, "trace"));
			   assert(RemoveIndexed(object->m_channel,object->m_id,"channel"));
               delete object;
			   object = 0;
		       LeaveCriticalSection(&addDelSect);
	      }

		  void DeleteByID(unsigned int id)
		  {		  
			   EnterCriticalSection(&addDelSect);
               testObj * ptr = GetByID(id);
			   if(ptr)
		          DeleteObject(ptr);
		       LeaveCriticalSection(&addDelSect);
	      }



};



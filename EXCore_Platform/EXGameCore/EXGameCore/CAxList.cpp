
#include "StdAfx.h"
#include <iostream>
#include <fstream>
#include "CAxList.h"
#include "assert.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAxLinkObj::CAxLinkObj()
{
	m_prevObject		= NULL;
	m_nextObject		= NULL;
	m_thisObject        = NULL;
	m_uniqueID          = 0;
	m_dbIndex           = 0;
}

CAxLinkObj::~CAxLinkObj()
{
	

}

CAxList::CAxList()
{
	m_listHead 			= NULL;
	m_count             = 0;
}


CAxList::~CAxList()
{
	RemoveAll();
}

void CAxList::RemoveAll()
{
   
   m_currentTemp = m_listHead;

   CAxLinkObj * lastObj = NULL;
   if(m_currentTemp)
   {
      while(m_currentTemp->m_nextObject)
      {
	        lastObj = m_currentTemp;
	        m_currentTemp = m_currentTemp->m_nextObject;
			delete lastObj;
			lastObj = 0;
      }
	  delete m_currentTemp;
	  m_currentTemp = 0;
   }
   m_listHead		= NULL;
   m_count			= 0;
   

}

bool CAxList::RemoveAt(unsigned int uniqueID)
{	
	

	if(!m_listHead)
	{
		
		return false;
	}
	
	bool foundDel = false;
	m_currentTemp = m_listHead;
	CAxLinkObj * delObj = NULL;
	while(m_currentTemp)
	{
	  if(m_currentTemp->m_uniqueID == uniqueID)
	  {
		 if(m_currentTemp->m_prevObject)
			m_currentTemp->m_prevObject->m_nextObject = m_currentTemp->m_nextObject;
		 
		 if(m_currentTemp->m_nextObject)
			m_currentTemp->m_nextObject->m_prevObject = m_currentTemp->m_prevObject;
		 m_count--;
		 foundDel = true;
         delObj = m_currentTemp;
         m_currentTemp = m_currentTemp->m_nextObject;
		 if(m_currentTemp)
         if(!m_currentTemp->m_prevObject)//check for new list head
             m_listHead = m_currentTemp;

		 if(m_listHead->m_uniqueID == delObj->m_uniqueID)
		    m_listHead = NULL;

		 delete delObj;
		 delObj = 0;
		 break;
	  }
	  else
	     m_currentTemp = m_currentTemp->m_nextObject;
	}
	
	return foundDel;
}

bool CAxList::UpdateMemory_unsafe(unsigned int uniqueID,void * newMem)
{	
	if(!m_listHead)
		return false;
	
	bool foundDel = false;
	m_currentTemp = m_listHead;
	while(m_currentTemp)
	{
	  if(m_currentTemp->m_uniqueID == uniqueID)
	  {
		 m_currentTemp->m_thisObject = newMem;
		 return true;
	  }
	  else
	     m_currentTemp = m_currentTemp->m_nextObject;
	}
	return false;
}



bool CAxList::RemoveAt_unsafe(unsigned int uniqueID)
{	

	if(!m_listHead)
	{
		return false;
	}
	
	bool foundDel = false;
	m_currentTemp = m_listHead;
	CAxLinkObj * delObj = NULL;
	while(m_currentTemp)
	{
	  if(m_currentTemp->m_uniqueID == uniqueID)
	  {
		 if(m_currentTemp->m_prevObject)
			m_currentTemp->m_prevObject->m_nextObject = m_currentTemp->m_nextObject;
		 
		 if(m_currentTemp->m_nextObject)
			m_currentTemp->m_nextObject->m_prevObject = m_currentTemp->m_prevObject;
		 m_count--;
		 foundDel = true;
         delObj = m_currentTemp;
         m_currentTemp = m_currentTemp->m_nextObject;
		 if(m_currentTemp)
         if(!m_currentTemp->m_prevObject)//check for new list head
             m_listHead = m_currentTemp;

		 if(m_listHead->m_uniqueID == delObj->m_uniqueID)
		    m_listHead = NULL;

		 delete delObj;
		 delObj = 0;
		 break;
	  }
	  else
	     m_currentTemp = m_currentTemp->m_nextObject;
	}
	return foundDel;
}

bool CAxList::RemoveAt_unsafe(CAxLinkObj * object)
{	
	if(!m_listHead)
	{
		return false;
	}
	
	bool foundDel = false;
	m_currentTemp = m_listHead;
	CAxLinkObj * delObj = NULL;
	while(m_currentTemp)
	{
	  if(m_currentTemp == object)
	  {
		 if(m_currentTemp->m_prevObject)
			m_currentTemp->m_prevObject->m_nextObject = m_currentTemp->m_nextObject;
		 
		 if(m_currentTemp->m_nextObject)
			m_currentTemp->m_nextObject->m_prevObject = m_currentTemp->m_prevObject;
		 m_count--;
		 foundDel = true;
         delObj = m_currentTemp;
         m_currentTemp = m_currentTemp->m_nextObject;
		 if(m_currentTemp)
         if(!m_currentTemp->m_prevObject)//check for new list head
             m_listHead = m_currentTemp;

		 if(m_listHead == delObj)
		    m_listHead = NULL;

		 delete delObj;
		 delObj = 0;
		 break;
	  }
	  else
	     m_currentTemp = m_currentTemp->m_nextObject;
	}
	return foundDel;
}


void CAxList::AddTail(CAxLinkObj * object)
{
	 
	 if(!m_listHead)
	 {
		 m_listHead = object;
		 m_count++;
		 
		 return;
	 }
	 m_currentTemp = m_listHead;
	 m_lastTemp = NULL;
	 while(m_currentTemp->m_nextObject)
	 {
          assert(object!=m_currentTemp);//Attempt was made to insert duplicate pointer into list
	      m_lastTemp = m_currentTemp;
		  m_currentTemp = m_currentTemp->m_nextObject;
	 }
     assert(object!=m_currentTemp);//Attempt was made to insert duplicate pointer into list

	 m_currentTemp->m_nextObject = object;
	 object->m_prevObject = m_currentTemp;
	 m_count++;
	 
}

void CAxList::AddHead(CAxLinkObj * object)
{
	 
	 if(!m_listHead)
	 {
		 m_listHead = object;
		 m_count++;
		 
		 return;
	 }
	 
	 object->m_nextObject = m_listHead;
	 m_listHead->m_prevObject = object;
	 m_listHead = object;
	 m_count++;
	 
}

CAxLinkObj* CAxList::GetHeadPosition_unsafe()//must be manually locked
{
    return m_listHead;
}

void * CAxList::GetNext_unsafe(CAxLinkObj** cPtr)
{
	CAxLinkObj * thisPoint = *cPtr;
    *cPtr = thisPoint->m_nextObject;

    return thisPoint->m_thisObject;
}

void * CAxList::GetPrev_unsafe(CAxLinkObj** cPtr)
{
    CAxLinkObj* thisPoint = *cPtr;
    *cPtr = thisPoint->m_prevObject;

    return thisPoint->m_thisObject;
}

CAxLinkObj* CAxList::FindIndex(int index)
{
	 
	 if(!m_listHead)
	 {
		 
		 return NULL;
	 }
	 int traceTemp = 0;
	 m_currentTemp = m_listHead;
	 m_lastTemp = NULL;
	 while(m_currentTemp)
	 {
		  if(traceTemp == index)
		  {
			 
			 return m_currentTemp;
		  }
		 
	      m_lastTemp = m_currentTemp;
		  m_currentTemp = m_currentTemp->m_nextObject;
	      traceTemp++;
	 }
	 
	 return NULL;
}

int CAxList::GetCount()
{
	
	int threadSafeTransfer = m_count;
	
	return threadSafeTransfer;
}


void CAxList::InsertAfter(CAxLinkObj *pos, CAxLinkObj * object)
{
	 
	 if(!m_listHead)
	 {
		 m_listHead = object;
		 m_count++;
		 
		 return;
	 }
	 object->m_nextObject = pos->m_nextObject;
	 pos->m_nextObject = object;
	 m_count++;
	 
}

CAxLinkObj* CAxList::GetByUniqueID(unsigned int uniqueID)
{
//note lock must be carried out and then unlocked when finished with this section
	 
	 if(!m_listHead)
	 {
		 
		 return NULL;
	 }
	
	 m_currentTemp = m_listHead;
	 while(m_currentTemp->m_nextObject)
	 {
		  if(m_currentTemp->m_uniqueID == uniqueID)
		  {
			 
		     return m_currentTemp;//returns locked-application must unlock it extremally when done with the pointer-VALID only if return true
		  }
		  m_currentTemp = m_currentTemp->m_nextObject;
	 }
	 
	 if(m_currentTemp->m_uniqueID == uniqueID)
	 {
		
		return m_currentTemp;//returns locked-application must unlock it extremally when done with the pointer-VALID only if return true
	 }
	 
	 return NULL;
}

bool CAxDB::WriteStatus(char * fileName)
{
	FILE *outFile = NULL;
	outFile = fopen(fileName, "w");
	if(!outFile)
	    return false;
    fprintf(outFile, "cell Count : %d\n", m_cellCount);
	fprintf(outFile, "\n");
    
	for(unsigned int loop=0;loop<m_indexedVarCount;loop++)
	{
	
		fprintf(outFile, " List # : %s\n", m_listArray[loop].m_indexedBy);
		
		for(unsigned int cellLoop=0;cellLoop<m_cellCount;cellLoop++)
	    {
			if(m_listArray[loop].m_axList[cellLoop].GetCount() > 0)
			{
			   fprintf(outFile, "\t List # : %d\t", cellLoop);
			   fprintf(outFile, " Objects : %d\n", m_listArray[loop].m_axList[cellLoop].GetCount());
			}
		}
		fprintf(outFile, "\n");
			
	}
	
	fclose(outFile);
	
	
	//works dont delete
	/*ofstream fout("file.dat", ios::binary);

    if (fout.fail())
	{	
	    
		return false;
	}
	
	int number = 30;
    fout.write((char *)(&number), sizeof(number));
	
	fout.close();*/
	
	
	return true;
}

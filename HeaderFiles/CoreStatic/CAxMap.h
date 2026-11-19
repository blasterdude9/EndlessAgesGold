#pragma once
#include "CAxList.h"

class CAXMap : public CAxDB
{//hashed search class ussually only one element ends up in a cell depending on hash size-this is ussually very fasy with large amounts of data
	public:

    CAXMap()
	{
			Init(2000,1);//determines memory commit and speed of query
			AddIndexVariable(0,"func");
	}

	CAXMap(int hashSize)
	{
		    if(hashSize < 10)
			   hashSize = 10;
			Init(hashSize,1);//determines memory commit and speed of query
			AddIndexVariable(0,"func");
	}

	~CAXMap()
	{
		Lock();
		if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
		 for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
		 {
			 for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
			 {
				 CString * delPtr = (CString *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
				 delete delPtr;
				 delPtr = 0;
			 }
		 }
		Unlock();
	}

	void AddItem(unsigned int uniqueID, CString functionName)
	{
        CString * ptr = new CString(functionName);
		AddIndexed((void*)ptr, GetSimpleHash(functionName.GetBuffer()), uniqueID,"func");
	}
 
	unsigned int GetIDFromFunctionName(CString & functionName)
	{
			 CAxList * obj = GetAllOnIndex(GetSimpleHash(functionName.GetBuffer()),"func");
			 unsigned int retVal = 0;
			 for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
			 {//ussually theres only one element in a loop so dont worry about that
				 retVal = pos->m_uniqueID;
				 CString * ptr = (CString *) obj->GetNext_unsafe(&pos);
				 if(ptr->CompareNoCase(functionName)==0)
					return retVal;
			 }
			 return 0;//not found
	}
   
};
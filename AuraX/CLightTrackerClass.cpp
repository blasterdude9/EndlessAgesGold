#include "stdafx.h"
#include "CLightTrackerClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------Main object list------------------//
CLightTrackerObj::CLightTrackerObj()
{
List_hardwareIndex=0;
}

void CLightTrackerObjList::SafeDelete()
{
 for(POSITION posLoc=GetHeadPosition();posLoc!=NULL;){
     CLightTrackerObj * objPtr = (CLightTrackerObj *)GetNext(posLoc);
     delete objPtr;
	 objPtr=0;
 }
 RemoveAll();
}



int CLightTrackerObjList::AddLight()
{
int indexFind=2;
for(POSITION posLoc=GetHeadPosition();posLoc!=NULL;){
    CLightTrackerObj * objPtr = (CLightTrackerObj *)GetNext(posLoc);
    if(objPtr->List_hardwareIndex != indexFind){//found open hardware int
       CLightTrackerObj * objPtr = new CLightTrackerObj();
	   objPtr->List_hardwareIndex = indexFind;
	   AddTail(objPtr);
	   return indexFind;
	}//end found open hardware int
	indexFind++;
}
return indexFind;
}


void CLightTrackerObjList::LightHasBeenRemoved(int hardwareIndex)
{
POSITION posLast;
for(POSITION posLoc=GetHeadPosition();(posLast = posLoc) != NULL;){
    CLightTrackerObj * objPtr = (CLightTrackerObj *)GetNext(posLoc);
    if(objPtr->List_hardwareIndex == hardwareIndex){//
       CLightTrackerObj * delPtr = (CLightTrackerObj *)GetAt(posLast);
	   delete delPtr;
	   delPtr=0;
       RemoveAt(posLast);
	   return;
	}//
}
}
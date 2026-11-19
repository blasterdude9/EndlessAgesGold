#include "stdafx.h"
#include <afxtempl.h>
#include "CAIWebClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------//
//-----------------Route To Object---------------------//
//-----------------------------------------------------//
CAIRouteToObj::CAIRouteToObj()
{
   m_mapNodeCount = 0;
    m_map          = NULL;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIRouteToObj::SafeDelete()
{
	if(m_map){
       delete [] m_map;
	   m_map = 0;
	}
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIRouteToObj::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
		
	   ar << m_mapNodeCount; 
	   for(int loop=0;loop<m_mapNodeCount;loop++)
	       ar << m_map[loop];
	}
	else
	{
       ar >> m_mapNodeCount; 
	   if(m_mapNodeCount>0)
          m_map = new int[m_mapNodeCount];
	   for(int loop=0;loop<m_mapNodeCount;loop++)
	       ar >> m_map[loop];
       
	}
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIRouteToObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAIRouteToObj*rtPtr=(CAIRouteToObj*)GetNext(posLoc);
		rtPtr->SafeDelete();
		delete rtPtr;
		rtPtr=0;
	}
    RemoveAll();
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
IMPLEMENT_SERIAL(CAIRouteToObj, CObject, 0)
IMPLEMENT_SERIAL(CAIRouteToObjList, CObList, 0)

//-----------------------------------------------------//
//-------------End Route To Object---------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
//------------------web Object-------------------------//
//-----------------------------------------------------//
CAIWebObj::CAIWebObj()
{
   m_name = "NA";
   m_web = NULL;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIWebObj::SafeDelete()
{
	if(m_web){
	   m_web->SafeDelete();
       delete m_web;
	   m_web=0;
	}
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
int * CAIWebObj::GetRoute(int currentPos,int dest,int * nodeCount)
{
	if(!m_web)
		return NULL;
   
	if(currentPos < 0)
	   return NULL;

    POSITION wPos = m_web->FindIndex(currentPos);
	if(!wPos)
		return NULL;
	       CAIWebNode * locationNode = (CAIWebNode *)m_web->GetAt(wPos);
           return locationNode->GetRoute(dest,nodeCount);
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIWebObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
		
	   ar << m_name 
		  << m_web;
	}
	else
	{
       ar >> m_name 
		  >> m_web;
	}
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIWebObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAIWebObj * wObj = (CAIWebObj *)GetNext(posLoc);
		wObj->SafeDelete();
		delete wObj;
		wObj = 0;
	}
	RemoveAll();
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
CAIWebObj * CAIWebObjList::GetByIndex(int index)
{
    if(index < 0)
	   return NULL;

    POSITION posLoc = FindIndex(index);
	if(posLoc){
        CAIWebObj * wObj = (CAIWebObj *)GetNext(posLoc);
		return wObj;
	}
	return NULL;
}

IMPLEMENT_SERIAL(CAIWebObj, CObject, 0)
IMPLEMENT_SERIAL(CAIWebObjList, CObList, 0)
//-----------------------------------------------------//
//------------------End web Object---------------------//
//-----------------------------------------------------//
CAIWebNode::CAIWebNode()
{
    m_locationName                = "NONE";
    m_routeToAllFromLocationList  = NULL;
	m_directLinks                 = NULL;
    m_directLinkCount             = 0;
	m_position.x                  = 0.0f;
	m_position.y                  = 0.0f;
	m_position.z                  = 0.0f;
	m_radius                      = 20.0f;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
int * CAIWebNode::GetRoute(int index,int * mapNodeCount)
{
    if(index < 0)
	   return NULL;

	if(!m_routeToAllFromLocationList)
	{
		return NULL;
	}
	 
    POSITION posLoc = m_routeToAllFromLocationList->FindIndex(index);
    if(!posLoc){
		*mapNodeCount = 0;
		return NULL;
	}
	    CAIRouteToObj * routePtr = (CAIRouteToObj *)m_routeToAllFromLocationList->GetAt(posLoc);
        *mapNodeCount = routePtr->m_mapNodeCount;
		if(routePtr->m_mapNodeCount > 0){
			int * mapNew = new int[routePtr->m_mapNodeCount];
		    CopyMemory(mapNew,routePtr->m_map,sizeof(int)*routePtr->m_mapNodeCount);
		    return mapNew;
		}
		return NULL;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIWebNode::SafeDelete()
{
	if(m_routeToAllFromLocationList){
       m_routeToAllFromLocationList->SafeDelete();
       delete m_routeToAllFromLocationList;
	   m_routeToAllFromLocationList=0;
	}
	if(m_directLinks)
	{
       delete [] m_directLinks;
       m_directLinks = 0;
	}
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIWebNode::AddDirectLink(int newInt)
{
int count = m_directLinkCount;
int * array = new int[(count+1)];  

  for(int loop=0;loop<count;loop++)
      array[loop] = m_directLinks[loop];

  array[count] = newInt;
  count++;
  if(m_directLinks)
     delete [] m_directLinks;
     m_directLinks   = 0; 
     m_directLinkCount = count;
	 m_directLinks = array;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIWebNode::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
		
	   ar << m_locationName 
		  << m_routeToAllFromLocationList
          << m_directLinkCount
		  << m_position.x
		  << m_position.y
		  << m_position.z
		  << m_radius;
	  
	   for(int loop=0;loop<m_directLinkCount;loop++)
	       ar << m_directLinks[loop];
	}
	else
	{
       ar >> m_locationName 
		  >> m_routeToAllFromLocationList
		  >> m_directLinkCount
		  >> m_position.x
		  >> m_position.y
		  >> m_position.z
		  >> m_radius;
	   
	   if(m_directLinkCount > 0)
	      m_directLinks = new int[m_directLinkCount];
	   for(int loop=0;loop<m_directLinkCount;loop++)
	       ar >> m_directLinks[loop];
	       
	}
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
float CAIWebNodeList::GetRadius(int index)
{
    POSITION posLoc = FindIndex(index);
    if(!posLoc){
		return 0.0f;
	}
	    CAIWebNode * webPtr = (CAIWebNode *)GetAt(posLoc);
		return webPtr->m_radius;
}

CString CAIWebNodeList::ConvertLongToString(long number)
{    
	          if(number==0)
				 return "0";

			  char buffer[67];

			  _ltoa_s(number,buffer,10);
		
			  return buffer;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
D3DVECTOR CAIWebNodeList::GetPosition(int index)
{
    POSITION posLoc = FindIndex(index);
    if(!posLoc){
		D3DVECTOR nullVect;
		nullVect.x = 0.0f;
		nullVect.y = 0.0f;
		nullVect.z = 0.0f;
		AfxMessageBox("GetPosition() aiweb -returned NULL Vector:"+ConvertLongToString(index));
		return nullVect;
	}
	    CAIWebNode * webPtr = (CAIWebNode *)GetAt(posLoc);
		return webPtr->m_position;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIWebNodeList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAIWebNode*aiPtr=(CAIWebNode*)GetNext(posLoc);
		aiPtr->SafeDelete();
		delete aiPtr;
		aiPtr=0;
	}
    RemoveAll();
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
int * CAIWebNodeList::AddIntegerIntoArray(int * intArray,int * arrayCount,int newInt)
{
int count = *arrayCount;
int * array = new int[(count+1)];  

  for(int loop=0;loop<count;loop++)
      array[loop] = intArray[loop];

  array[count] = newInt;
  count++;
  if(intArray)
     delete [] intArray;
     intArray   = 0; 
     *arrayCount = count;
	 return array;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
int CAIWebNodeList::GetNearestNode(D3DVECTOR location)
{
	 int   trace=0;
	 int   winner=0;
	 float distancePersist = 0;
     for(POSITION posLoc=GetHeadPosition();posLoc != NULL;)
	 {//Node loop
        CAIWebNode* wMainObj = (CAIWebNode*)GetNext(posLoc);
        float distance = GL_matrixFunctions.MagnitudeSquared(wMainObj->m_position.x,
			                                                  wMainObj->m_position.y,
			                                                  wMainObj->m_position.z,
											                  location.x,
											                  location.y,
											                  location.z);
        if(trace==0){
		   distancePersist = distance;
           winner = trace;
		}

        if(distance < distancePersist){
		   distancePersist = distance;
		   winner = trace;
		}

		trace++;
	 }//end Node loop
	 return winner;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
BOOL CAIWebNodeList::CompileAll(int querryDuration)
{
  srand( (unsigned)time( NULL ) );
  int primaryTrace = 0;
  int secondaryTrace = 0;
  for(POSITION posLoc=GetHeadPosition();posLoc != NULL;)
  {//Node loop
   CAIWebNode* wMainObj = (CAIWebNode*)GetNext(posLoc);
   if(!wMainObj->m_routeToAllFromLocationList)
       wMainObj->m_routeToAllFromLocationList = new CAIRouteToObjList();
       wMainObj->m_routeToAllFromLocationList->SafeDelete(); 
	   secondaryTrace = 0;
       for(POSITION desPos=GetHeadPosition();desPos != NULL;)
	   {//destination loop
           CAIWebNode* destObj = (CAIWebNode*)GetNext(desPos);
           CAIRouteToObj * routePtr = new CAIRouteToObj();
		   CompileNodeLight(wMainObj,querryDuration,destObj,routePtr,primaryTrace,secondaryTrace,(int)GetCount());
		   wMainObj->m_routeToAllFromLocationList->AddTail(routePtr);//add
		   secondaryTrace++;
	   }//end destrination loop
	   primaryTrace++;
   }//End Node loop
return TRUE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
BOOL CAIWebNodeList::UntraveledPathExists(int * currentLinksTraveled,
										  int & currentLinkTraveledCount,
										  int & directLinkCount,
										  int * pathDirectLinks,
										  int & randomPathSelection
										  )
{
   
	int untraveledPathCount = 0;
	if(directLinkCount == 0)
	   return FALSE;
	int * goodPathArray = new int[directLinkCount];

	for(int loop=0;loop<directLinkCount;loop++)
	{
        int possiblePath = pathDirectLinks[loop];
		BOOL traveled = FALSE;
		for(int traveledPathLoop=0;traveledPathLoop<currentLinkTraveledCount;traveledPathLoop++)
		{
            if(currentLinksTraveled[traveledPathLoop] == possiblePath)
               traveled = TRUE;
		}

        if(!traveled)
		{
			goodPathArray[untraveledPathCount] = possiblePath;
			untraveledPathCount++;
            //return TRUE;
		}
	}
  
   

   if(untraveledPathCount > 0)
   {
	  randomPathSelection = goodPathArray[(rand()%untraveledPathCount)];
      delete [] goodPathArray;
	  goodPathArray=0;
	  return TRUE;
   }
   else{
      delete [] goodPathArray;
	  goodPathArray=0;
   }
   return FALSE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIWebNodeList::CompileNodeLight(  CAIWebNode* wMainObj,
										long querryDuration,
										CAIWebNode* destObj,
										CAIRouteToObj * routePtr,
										int primaryIndex,
										int secondaryIndex,
										int totalnodeCount)
{
           //find fastest route up or down
	       int upCount = 0;
		   int downCount = 0;
		   int thisIndex = primaryIndex;
		   while(thisIndex != secondaryIndex)
		   {
               upCount++;
			   thisIndex++;
			   if(thisIndex == totalnodeCount)
			      thisIndex = 0;
		   }
           thisIndex = primaryIndex;
		   while(thisIndex != secondaryIndex)
		   {
               downCount++;
			   thisIndex--;
			   if(thisIndex < 0)
			      thisIndex = totalnodeCount-1;
		   }

		   if(upCount < downCount)
		   {//up is faster
              if(upCount > 0)
			  {
                 routePtr->m_map = new int[upCount];
				 int trace = 0;
				 routePtr->m_mapNodeCount = upCount;
				 thisIndex = primaryIndex;
				 while(thisIndex != secondaryIndex)
				 {
					upCount++;
					thisIndex++;
					if(thisIndex == totalnodeCount)
					   thisIndex = 0;
                    routePtr->m_map[trace] = thisIndex;
					trace++;
				 }
			  }
		   }//end up is faster
		   else
		   {
              if(downCount > 0)
			  {
                 routePtr->m_map = new int[downCount];
				 int trace = 0;
				 routePtr->m_mapNodeCount = downCount;
				 thisIndex = primaryIndex;
				 while(thisIndex != secondaryIndex)
				 {
					downCount++;
					thisIndex--;
					if(thisIndex < 0)
						thisIndex = totalnodeCount-1;
                    routePtr->m_map[trace] = thisIndex;
					trace++;
				 }
			  }


		   }


}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
void CAIWebNodeList::CompileNode(CAIWebNode* wMainObj,
								 long querryDuration,
								 CAIWebNode* destObj,
								 CAIRouteToObj * routePtr)
{
           
	        
	
	       CAIWebNode* currentObj = wMainObj;
		   CAIWebNode* lastObj    = wMainObj;
		   //find route to dest object
           long timeStamp          = GetTickCount();
		   long thoughtDuration    = querryDuration;

		   int * currentRouteBuild      = NULL;
		   int   currentRouteNodeCount  = 0;
		   BOOL foundRoute = FALSE;
           long timePassed = 0;
		   int lastMapNode = -1;
		   int curMapIndexPoint = 0;
		   while(((long)GetTickCount()-timeStamp) < thoughtDuration){//cycle for duration till u find fastest dest objects from main
			  int linkCount = currentObj->m_directLinkCount;
              BOOL hasLinks = TRUE;
			  if(linkCount <= 1 && currentRouteNodeCount != 0)
				 hasLinks = FALSE;

			  //currentRouteNodeCount
			  //currentRouteBuild
			  int selectedPathLink = 0;
              if(!UntraveledPathExists(currentRouteBuild,
								       currentRouteNodeCount,
								       linkCount,
								       currentObj->m_directLinks,
									   selectedPathLink
								       ))
								       hasLinks = FALSE;

			  
			  if(hasLinks == TRUE && linkCount != 0){//has links
				if(currentObj == destObj)//already there
                    foundRoute = TRUE;
				else{//go through process

                 int selectedPath = rand()%(linkCount);
				 
				 int currentProspect = selectedPathLink;
                 
				    lastMapNode = curMapIndexPoint;
				    curMapIndexPoint = currentProspect;
                    //add
				    lastObj = currentObj;
				    POSITION posLastObj = FindIndex(curMapIndexPoint);
				    currentObj = (CAIWebNode*)GetAt(posLastObj);
                    currentRouteBuild = AddIntegerIntoArray(currentRouteBuild,&currentRouteNodeCount,curMapIndexPoint);
                    if(currentObj == destObj)
                      foundRoute = TRUE;
				}//end go through process
			  }//end has links
			  else{//end of the road?
					if(currentObj == destObj)//already there
                       foundRoute = TRUE;
					else{//nada
				       lastObj     = wMainObj;
                       currentObj  = wMainObj;
					   lastMapNode = -1;
					   curMapIndexPoint = 0;
					   if(currentRouteBuild){
					      delete [] currentRouteBuild;
                          currentRouteBuild = 0;
					   }
					   currentRouteNodeCount = 0;
					}//end nada
			  }//end end of the road?
			  if(foundRoute == TRUE){//shift and set
				 if(currentRouteNodeCount < routePtr->m_mapNodeCount || routePtr->m_map == NULL)
				 {//found better path
					 if(routePtr->m_map){
                        delete [] routePtr->m_map;
						routePtr->m_map=0;
					 }

					 routePtr->m_mapNodeCount = currentRouteNodeCount;
					 if(currentRouteNodeCount > 0){
					 routePtr->m_map          = new int[currentRouteNodeCount];
					 CopyMemory(routePtr->m_map,currentRouteBuild,(sizeof(int)*currentRouteNodeCount));
					 delete [] currentRouteBuild;
					 }
					 currentRouteBuild=0;
					 currentRouteNodeCount    = 0;
					 lastMapNode              = -1;
					 curMapIndexPoint         = 0;
					 lastObj                  = wMainObj;
					 currentObj               = wMainObj;//reset
					 foundRoute               = FALSE;
				 }//end found better path
			  }//end shift and set
		   }//end cycle for duration till u find fastest dest objects from main
           //end find route to dest object
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
BOOL CAIWebNodeList::GetShortestRoute(int current,int target)
{
  //start tree-build lowest level root
   CAITree * m_baseTree = new CAITree();
   int backPath = -1;
   GenerateNodeInfo(backPath,
				    current,
				    m_baseTree);//build web
 
  return TRUE;//found route successfully (is possible)
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
BOOL CAIWebNodeList::BeenThereAlready(CAITree * m_baseTree,
									  int destination)
{
   for(POSITION posLoc = m_baseTree->GetHeadPosition();posLoc != NULL;)
   {
       CAITreeNode * node = (CAITreeNode *)m_baseTree->GetNext(posLoc);
       if(node->m_currentNode == destination)
		  return TRUE;
   }
   return FALSE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
BOOL CAIWebNodeList::GenerateNodeInfo(int & backPath,
									  int & currentIndex,
									  CAITree * m_currentTree)
{
 
  return TRUE;
}

//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
CAIWebNode* CAIWebNodeList::GetByIndex(int index)
{
  if(index < 0)
     return NULL;

  POSITION rPos = FindIndex(index);
  if(!rPos)
	  return NULL;
      return(CAIWebNode*)GetAt(rPos);
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
int CAIWebNodeList::GetIndexFromPointer(CAIWebNode* wbPtr)
{
	int trace = 0;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAIWebNode*cmpPtr=(CAIWebNode*)GetNext(posLoc);
		if(wbPtr == cmpPtr)
           return trace;
        trace++;
	}
    return -1;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
BOOL CAIWebNodeList::VerifyMoreInList(CAIWebNode* wbPtr)
{
	int trace = 0;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAIWebNode*cmpPtr=(CAIWebNode*)GetNext(posLoc);
		if(wbPtr != cmpPtr)
           return TRUE;
        trace++;
	}
    return FALSE;
}

IMPLEMENT_SERIAL(CAIWebNode, CObject, 0)
IMPLEMENT_SERIAL(CAIWebNodeList, CObList, 0)



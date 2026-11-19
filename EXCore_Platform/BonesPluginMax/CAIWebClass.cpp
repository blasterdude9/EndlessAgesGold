#include "afxwin.h"
#include "afxext.h"
#include "CAIWebClass.h"

//-----------------------------------------------------//
//-----------------Route To Object---------------------//
//-----------------------------------------------------//
CAIRouteToObj::CAIRouteToObj()
{
   m_mapNodeCount = 0;
   m_map          = NULL;
}

void CAIRouteToObj::SafeDelete()
{
	if(m_map){
       delete [] m_map;
	   m_map = 0;
	}
}

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

void CAIWebObj::SafeDelete()
{
	if(m_web){
	   m_web->SafeDelete();
       delete m_web;
	   m_web=0;
	}
}

int * CAIWebObj::GetRoute(int currentPos,int dest,int * nodeCount)
{
	if(!m_web)
		return NULL;
    POSITION wPos = m_web->FindIndex(currentPos);
	if(!wPos)
		return NULL;
	       CAIWebNode * locationNode = (CAIWebNode *)m_web->GetAt(wPos);
           return locationNode->GetRoute(dest,nodeCount);
}

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

int * CAIWebNode::GetRoute(int index,int * mapNodeCount)
{
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



void CAIWebNode::SafeDelete()
{
	if(m_routeToAllFromLocationList){
       m_routeToAllFromLocationList->SafeDelete();
       delete m_routeToAllFromLocationList;
	   m_routeToAllFromLocationList=0;
	}

}

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

float CAIWebNodeList::GetRadius(int index)
{
    POSITION posLoc = FindIndex(index);
    if(!posLoc){
		return 0.0f;
	}
	    CAIWebNode * webPtr = (CAIWebNode *)GetAt(posLoc);
		return webPtr->m_radius;
}

D3DVECTOR CAIWebNodeList::GetPosition(int index)
{
    POSITION posLoc = FindIndex(index);
    if(!posLoc){
		D3DVECTOR nullVect;
		nullVect.x = 0.0f;
		nullVect.y = 0.0f;
		nullVect.z = 0.0f;
		return nullVect;
	}
	    CAIWebNode * webPtr = (CAIWebNode *)GetAt(posLoc);
		return webPtr->m_position;
}


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


BOOL CAIWebNodeList::CompileAll(int querryDuration)
{
  srand( (unsigned)time( NULL ) );
  for(POSITION posLoc=GetHeadPosition();posLoc != NULL;)
  {//Node loop
   CAIWebNode* wMainObj = (CAIWebNode*)GetNext(posLoc);
   if(!wMainObj->m_routeToAllFromLocationList)
       wMainObj->m_routeToAllFromLocationList = new CAIRouteToObjList();
       wMainObj->m_routeToAllFromLocationList->SafeDelete(); 
	   
       for(POSITION desPos=GetHeadPosition();desPos != NULL;)
	   {//destination loop
           CAIWebNode* destObj = (CAIWebNode*)GetNext(desPos);
           CAIRouteToObj * routePtr = new CAIRouteToObj();
		   CAIWebNode* currentObj = wMainObj;
		   CAIWebNode* lastObj    = wMainObj;
		   //find route to dest object
           long timeStamp          = GetTickCount();
		   long thoughtDuration    = querryDuration;
		   //routePtr->m_map
		   int * currentRouteBuild      = NULL;
		   int   currentRouteNodeCount  = 0;
		   BOOL foundRoute = FALSE;
           long timePassed = 0;
		   int lastMapNode = -1;
		   int curMapIndexPoint = 0;
		   while((GetTickCount()-timeStamp) < thoughtDuration){//cycle for duration till u find fastest dest objects from main
			  int linkCount = currentObj->m_directLinkCount;
              BOOL hasLinks = TRUE;
			  if(linkCount == 1 && currentRouteNodeCount != 0)
				 hasLinks = FALSE;
			  //hasLinks = VerifyMoreInList(lastObj);
			  if(hasLinks == TRUE && linkCount != 0){//has links
				if(currentObj == destObj)//already there
                    foundRoute = TRUE;
				else{//go through process
				 //linkCount--;
                 int selectedPath = rand()%(linkCount);
				 //while(selectedPath == lastMapNode){//dont go back ;		       
			     //selectedPath = rand()%(linkCount);
				 //}
                 int currentProspect = currentObj->m_directLinks[selectedPath];
				 while(currentProspect == lastMapNode){//dont go back ;		       
			           selectedPath = rand()%(linkCount);
					   currentProspect = currentObj->m_directLinks[selectedPath];
					   if(linkCount == 1 && currentProspect == lastMapNode){
                          //dead end
						   AfxMessageBox("Dead end loop infinit");
						   break;
					   }
				 }
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
			  }
			  else{//end of the road?
                    //didnt find anything // Reset
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
		   wMainObj->m_routeToAllFromLocationList->AddTail(routePtr);//add
	   }//end destrination loop
   }//End Node loop
       //CAIWebObj* ptr = (CAIWebObj*)GetHead();
       //int * cow = NULL;
	   //int count = 0;
	   //cow = ptr->GetMap(4,&count);
	   //cow = ptr->GetMap(3,&count);
	   //cow = ptr->GetMap(2,&count);
	   //cow = ptr->GetMap(1,&count);
      // cow = ptr->GetMap(0,&count);
return TRUE;
}

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



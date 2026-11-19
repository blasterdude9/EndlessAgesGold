#include "stdafx.h"

#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CArmedInventoryClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//----------------------------------------------//
//------------Main object list------------------//
//----------------------------------------------//
CArmedInventoryObj::CArmedInventoryObj(CString name)
{
   m_name						= name;
   m_firstPersonSystem			= NULL;
   m_thirdPersonSystem			= NULL;
   m_boneAttached				= 0;
   m_functionalityAttribute		= 0;
   m_miscInt					= 0;
   m_miscInt2					= 0;
   m_miscInt3					= 0;
   m_miscPosition.x				= 0;
   m_miscPosition.y				= 0;
   m_miscPosition.z				= 0;
   m_id							= 0;
   m_lastUpdatedTime			= 0;
   m_locationOccupancy			= 0;
   m_skillUse					= -1;
   m_abilityTypeUse				= 0;
   m_animAccessVersion			= 0;
   m_type						= -1;
   m_obfusionMiscInt			= NULL;
   m_reloadDelay				= 0;
   m_tempItem					= 0;
   m_alwaysShowFirstPerson		= FALSE;
   m_fireQueOvrd				= 0;
}
//----------------------------------------------//
//------------Memory Deleteion------------------//
//----------------------------------------------//
void CArmedInventoryObj::Invalidate()
{
	if(m_firstPersonSystem){
	   m_firstPersonSystem->Invalidate();
	}

	if(m_thirdPersonSystem){
	   m_thirdPersonSystem->Invalidate();
	}      
}
//----------------------------------------------//
//------------Memory Deleteion------------------//
//----------------------------------------------//
void CArmedInventoryObj::SafeDelete()
{
	if(m_firstPersonSystem){
	   m_firstPersonSystem->SafeDelete();
	   delete m_firstPersonSystem;
	   m_firstPersonSystem=0;
	}

	if(m_thirdPersonSystem){
	   m_thirdPersonSystem->SafeDelete();
	   delete m_thirdPersonSystem;
	   m_thirdPersonSystem=0;
	}     

	if(m_obfusionMiscInt)
	{ 
       delete m_obfusionMiscInt;
	   m_obfusionMiscInt = 0;
	}
}
//----------------------------------------------//
//-------------------Clone----------------------//
//----------------------------------------------//
void CArmedInventoryObj::Clone(CArmedInventoryObj **copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
CArmedInventoryObj * copyLocal = new CArmedInventoryObj(m_name);
 
   if(  m_firstPersonSystem)
		m_firstPersonSystem->Clone(&copyLocal->m_firstPersonSystem,g_pd3dDevice,NULL);
   if(  m_thirdPersonSystem)
		m_thirdPersonSystem->Clone(&copyLocal->m_thirdPersonSystem,g_pd3dDevice,NULL);
   copyLocal->m_boneAttached				= m_boneAttached;
   copyLocal->m_functionalityAttribute		= m_functionalityAttribute;
   copyLocal->m_miscInt						= m_miscInt;
   copyLocal->m_miscInt2					= m_miscInt2;
   copyLocal->m_miscInt3					= m_miscInt3;
   copyLocal->m_miscPosition.x				= m_miscPosition.x;
   copyLocal->m_miscPosition.y				= m_miscPosition.y;
   copyLocal->m_miscPosition.z				= m_miscPosition.z;
   copyLocal->m_locationOccupancy			= m_locationOccupancy;
   copyLocal->m_skillUse					= m_skillUse;
   copyLocal->m_abilityTypeUse				= m_abilityTypeUse;
   copyLocal->m_animAccessVersion			= m_animAccessVersion;
   copyLocal->m_type						= m_type;
   copyLocal->m_reloadDelay					= m_reloadDelay;
   copyLocal->m_boneMapping					= m_boneMapping;
   copyLocal->m_tempItem					= m_tempItem;
   copyLocal->m_alwaysShowFirstPerson		= m_alwaysShowFirstPerson;
   copyLocal->m_fireQueOvrd					= m_fireQueOvrd;

*copy = copyLocal;
}
//----------------------------------------------//
//----------------------------------------------//
//----------------------------------------------//
void CArmedInventoryObj::Serialize( CArchive& ar )
{

BOOL reserved = FALSE;

CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar.SetObjectSchema(4);
       ASSERT(ar.GetObjectSchema() == 4);

	    ar << m_name
           << m_firstPersonSystem
           << m_thirdPersonSystem
           << m_boneAttached  
           << m_functionalityAttribute 
           << m_miscInt     
           << m_miscInt2    
           << m_miscPosition.x   
           << m_miscPosition.y 
           << m_miscPosition.z
		   << m_locationOccupancy
		   << m_skillUse
		   << m_reloadDelay
		   << m_animAccessVersion
		   << m_miscInt3
		   << m_type;


		ar << m_boneMapping
		   << m_tempItem
		   << m_alwaysShowFirstPerson
		   << m_fireQueOvrd
		   << reserved;

	}
	else
	{
        int version = ar.GetObjectSchema();

        ar >> m_name
           >> m_firstPersonSystem
           >> m_thirdPersonSystem
           >> m_boneAttached  
           >> m_functionalityAttribute 
           >> m_miscInt     
           >> m_miscInt2    
           >> m_miscPosition.x   
           >> m_miscPosition.y 
           >> m_miscPosition.z
		   >> m_locationOccupancy
		   >> m_skillUse
		   >> m_reloadDelay
		   >> m_animAccessVersion
		   >> m_miscInt3
		   >> m_type;

        switch(version)
		{
		  case 3:
              ar >> m_boneMapping
		         >> m_tempItem
		         >> reserved;
			 
		  break;
		  case 4:
              ar >> m_boneMapping
		         >> m_tempItem
		         >> m_alwaysShowFirstPerson
				 >> m_fireQueOvrd
				 >> reserved;

		  break;
		}

		m_id                       = 0;
		m_lastUpdatedTime          = 0;
		m_obfusionMiscInt          = NULL;
	}
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
CArmedInventoryObj * CArmedInventoryList::GetByType(int type)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CArmedInventoryObj * visObj = (CArmedInventoryObj *)GetNext(posLoc);
		if(visObj->m_type == type)
		   return visObj;
	 }
	 return NULL;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CArmedInventoryList::Clone(CArmedInventoryList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CArmedInventoryList * copyLocal = new CArmedInventoryList();
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CArmedInventoryObj * visObj = (CArmedInventoryObj *)GetNext(posLoc);
        CArmedInventoryObj * clonedPtr = NULL; 
		visObj->Clone(&clonedPtr,g_pd3dDevice);
		copyLocal->AddTail(clonedPtr);
	 }
	 *copy = copyLocal;
}

int CArmedInventoryList::GetIndexByID(int id)
{
	POSITION posLast;
	int trace = 0;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
	    CArmedInventoryObj * armedObj = (CArmedInventoryObj *)GetNext(posLoc);
		if(armedObj->m_locationOccupancy == id){
		   return trace;
		}
		trace++;
	}
return -1;
}

int CArmedInventoryList::GetIndexByType(int type)
{
	POSITION posLast;
	int trace = 0;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
	    CArmedInventoryObj * armedObj = (CArmedInventoryObj *)GetNext(posLoc);
		if(armedObj->m_type == type){
		   return trace;
		}
		trace++;
	}
return -1;
}

CArmedInventoryObj * CArmedInventoryList::GetByName(CString name)
{
	POSITION posLast;
	int trace = 0;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
	    CArmedInventoryObj * armedObj = (CArmedInventoryObj *)GetNext(posLoc);
		if(armedObj->m_name.CompareNoCase(name)==0)
		   return armedObj;
		trace++;
	}
return NULL;
}

BOOL CArmedInventoryList::RemoveByType(int type,BOOL deleteMem)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
	    CArmedInventoryObj * armedObj = (CArmedInventoryObj *)GetNext(posLoc);
		if(armedObj->m_type == type){
		  if(deleteMem)
		  {
		     armedObj->SafeDelete();
		     delete armedObj;
	   	     armedObj=0;
		  }
		  RemoveAt(posLast);
		return TRUE;
		}
	}
return FALSE;
}

BOOL CArmedInventoryList::RemoveByID(int id)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
	    CArmedInventoryObj * armedObj = (CArmedInventoryObj *)GetNext(posLoc);
		if(armedObj->m_locationOccupancy == id){
		armedObj->SafeDelete();
		delete armedObj;
		armedObj=0;
		RemoveAt(posLast);
		return TRUE;
		}
	}
return FALSE;
}
void CArmedInventoryList::Invalidate()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
	    CArmedInventoryObj * armedObj = (CArmedInventoryObj *)GetNext(posLoc);
		armedObj->Invalidate();
	}
}

void CArmedInventoryList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
	    CArmedInventoryObj * armedObj = (CArmedInventoryObj *)GetNext(posLoc);
		armedObj->SafeDelete();
		delete armedObj;
		armedObj=0;
	}
	RemoveAll();
	if(directAccess)
	{
       delete [] directAccess;
       directAccess = 0;
	}
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
CArmedInventoryObj * CArmedInventoryList::GetByTypeDirectAccess(int type)
{
     if(!directAccess)
		 return NULL;

     if(type < 0)
	    return NULL;

     if(type >= directAccessObjectCount)
	    return NULL;


	 return directAccess[type].object;
}

//------------------------------------------------------------------//
BOOL CArmedInventoryList::MergeAsTempItems(CArmedInventoryList * tempItemsDB)
{
	POSITION posLast = NULL;
     for(POSITION posLoc = tempItemsDB->GetHeadPosition();(posLast=posLoc) != NULL;)
	 {
        CArmedInventoryObj * entObj  = (CArmedInventoryObj *)tempItemsDB->GetNext(posLoc);
		if(!GetByType(entObj->m_type))
		{//is not prestent already so i can be added
			entObj->m_tempItem = TRUE;
            AddTail(entObj);
			tempItemsDB->RemoveAt(posLast);
		}//end is not prestent already so i can be added
	 }
	 CompileDirectAccess();
	 return TRUE;
}
//------------------------------------------------------------------//
void CArmedInventoryList::ClearTempItems()
{
	POSITION posLast = NULL;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
        CArmedInventoryObj * entObj  = (CArmedInventoryObj *)GetNext(posLoc);
		if(entObj->m_tempItem)
		{
           entObj->SafeDelete();
		   delete entObj;
		   entObj = 0;
		   RemoveAt(posLast);
		}
	 }
	 CompileDirectAccess();
}

int CArmedInventoryList::GetHighestNumber()
{
   int count = 0;
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
        CArmedInventoryObj * entObj = (CArmedInventoryObj *)GetNext(posLoc);
		if(entObj->m_type > count)
		   count = entObj->m_type;
   }
   return count+1;
}

void CArmedInventoryList::CompileDirectAccess()
{
    int textureCount = (int)GetHighestNumber();
	if(textureCount == 0)
	   return;

	if(directAccess)
	{
       delete [] directAccess;
       directAccess = 0;
	}
    directAccess = new DirectAccessStructAIO[textureCount];
	ZeroMemory(directAccess,sizeof(DirectAccessStructAIO)*textureCount);
	//int tracer = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        
		CArmedInventoryObj * armPtr = (CArmedInventoryObj *)GetNext(posLoc);
		directAccess[armPtr->m_type].object = armPtr;
		//tracer++;  
	}
	directAccessObjectCount = textureCount;
}

int CArmedInventoryList::GetFreeID()
{//slow but doesnt matter
	int trace = 1;
	BOOL done = FALSE;
	while(!done)
	{
	  done = TRUE;
	  for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
		  CArmedInventoryObj * nPtr = (CArmedInventoryObj *)GetNext(posLoc);
		  if(nPtr->m_type == trace)
		  {
			  done = FALSE;
			  trace++;
			  break;
		  }
	  }
	}
	return trace;
}
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
//--------------------------------------------------------------------------//
void CArmedInventoryList::ApplyLODProcessing(CString strLodScript)
{//slow but doesnt matter
	m_lodScriptHasBeenExecuted = 1;
	//If current LOD count, remove lod #s zero based //example: 10000,6000,3000,1500,500 0,1,2,3,4
    //4,4,2,0;3,0,2;3,0; example of string that gets parsed to generate thse structures in an array form
    //always make sure lod that you will keep are listed greatest to least, this will keep proper lod removal
    //second line/section defines by a \r\n(newline) of the script includes the comminted LOD ranges for entities based on its height, 
	//this is critical because you can have only one set of ranges per entity and if a lod doesnt exist it just wont render ie: weapon 
	//attachment that may only have 2 lods and the entity is on lod 3 the attachment wont render this is a advantage
	//however if not understood while designing lods or adding lods this can be a major problem
	//PROBLEM : lod ranges can be different on every entity, tailored by size genrally-if range isnt in script will keep whats currently there
	//script example: 
	//4,4,2,0;3,0,2;3,0
	//0-4,7,30,300
	//4-6,10,50,600
	//6-12,20,90,800
	//LOD LIMITS (LOAD BALANCING) LOD INDEX,HOW MANY ALLOWED
    //0,2
    //1,5
    
	LODOptCondition lodOptArray[100];
    ZeroMemory(lodOptArray,sizeof(LODOptCondition)*100);

	int scriptLength = strLodScript.GetLength();
	char * scriptArray = strLodScript.GetBuffer();
	int trace = 0;
	int curArray = 0;
	int curRangeArray = 0;
	int mode = 0;
	CString temp;
	int stage = 0;
	int lodIndexArray = 0;
	
    while(trace < scriptLength)
	{
            if(mode == 0)
			{//processing lod elimination structures
               while(scriptArray[trace] != '\r' && scriptArray[trace] != '\n' && trace < scriptLength)
		       {//per line

			      if(scriptArray[trace] == 'L')
				     break;
              
                  while(scriptArray[trace] != ',' && scriptArray[trace] != ';')
				  {
					    if(scriptArray[trace] != ' ')
                           temp += scriptArray[trace];
						trace++;
				  }

				  if(stage == 0)//lod match count
                     lodOptArray[curArray].lodMatch = atoi(temp);
				  else
				  {
                     lodOptArray[curArray].removalArray[lodIndexArray] = atoi(temp);
                     lodOptArray[curArray].removalCount++;
					 lodIndexArray++;
				  }
				  temp = "";
				  stage++;
				  stage = 1;
				  if(scriptArray[trace] == ';')
				  {
                     curArray++;
                     stage = 0;
					 lodIndexArray = 0;
				  }
				  trace++;
				  
			   }//end per line
			   stage = 0;
			   lodIndexArray = 0;
			}//end processing lod elimination structures
			break;
		    mode = 1;
		trace++;
	}
	
	
	
	  for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
		  CArmedInventoryObj * nPtr = (CArmedInventoryObj *)GetNext(posLoc);

		  nPtr->m_thirdPersonSystem->RemoveLodsByStruct(curArray,lodOptArray);
		  
	  }
}



IMPLEMENT_SERIAL(CArmedInventoryObj, CObject, VERSIONABLE_SCHEMA|4)
IMPLEMENT_SERIAL(CArmedInventoryList, CObList, 0)

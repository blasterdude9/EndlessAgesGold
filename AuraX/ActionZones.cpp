 #include "StdAfx.h"
#include "actionzones.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//------------------------------------------------------------//
void CActionBox::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){

       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);

		 
	   ar << m_zoneLabel
		  << m_action
		  << m_actionValue
		  << boxMin.x
		  << boxMin.y
		  << boxMin.z
		  << boxMax.x
		  << boxMax.y
		  << boxMax.z;

	   ar << m_actionAttrib
		  << reserved
		  << reserved
		  << reserved
		  << reserved;
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_zoneLabel
		  >> m_action
		  >> m_actionValue
		  >> boxMin.x
		  >> boxMin.y
		  >> boxMin.z
		  >> boxMax.x
		  >> boxMax.y
		  >> boxMax.z;

	   m_actionAttrib = -1;

       if(version > 1)
	   {

	   ar >> m_actionAttrib
		  >> reserved
		  >> reserved
		  >> reserved
		  >> reserved;

	   }
	}
}
//------------------------------------------------------------//
void CActionZone::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << (int)m_boxList.GetCount();
	   for(POSITION posLoc = m_boxList.GetHeadPosition();posLoc != NULL;)
	   {
           CActionBox * m_box = (CActionBox *) m_boxList.GetNext(posLoc);
           ar << m_box;
	   }
	}
	else
	{
       int count = 0;
       ar >> count;
       for(int loop=0;loop<count;loop++)
	   {
           CActionBox * m_box = NULL;
           ar >> m_box;
		   m_boxList.AddTail(m_box);
	   }
	}
}
//----------------------Zone----------------------------------//
void CActionZone::ClearAll()
{
    for(POSITION posLoc = m_boxList.GetHeadPosition();posLoc != NULL;)
	{
		CActionBox * xPtr = (CActionBox *)m_boxList.GetNext(posLoc);
		delete xPtr;
		xPtr = 0; 
	}
	m_boxList.RemoveAll();
}
//----------------------Zone----------------------------------//
void CActionZone::AddBox(D3DVECTOR min, 
						 D3DVECTOR max, 
						 int action, 
						 int actionValue, 
						 int attrib,
						 CString label)
{
     CActionBox * newPtr = new CActionBox();
	 CopyMemory(&newPtr->boxMax, &max, sizeof(D3DVECTOR));
	 CopyMemory(&newPtr->boxMin, &min, sizeof(D3DVECTOR));
	 newPtr->m_action			= action;
	 newPtr->m_actionValue	    = actionValue;
	 newPtr->m_zoneLabel        = label;
	 newPtr->m_actionAttrib     = attrib;

	 m_boxList.AddTail(newPtr);
}
//----------------------Zone----------------------------------//
bool CActionZone::DeleteBox(int index)
{
     if(index < 0)
	    return false;

	 POSITION posLoc = m_boxList.FindIndex(index);
	 if(posLoc == NULL)
	    return false;

	 CActionBox * dPtr = (CActionBox *)m_boxList.GetAt(posLoc);
	 delete dPtr;
	 dPtr = 0;
	 m_boxList.RemoveAt(posLoc);
	 return true;
}
//----------------------Zone----------------------------------//
bool CActionZone::EditBox(int index,
						  D3DVECTOR min, 
						  D3DVECTOR max, 
						  int action, 
						  int actionValue, 
						  int attrib,
						  CString label)
{
     if(index < 0)
	    return false;

	 POSITION posLoc = m_boxList.FindIndex(index);
	 if(posLoc == NULL)
	    return false;

	 CActionBox * dPtr = (CActionBox *)m_boxList.GetAt(posLoc);
	 CopyMemory(&dPtr->boxMax, &max, sizeof(D3DVECTOR));
	 CopyMemory(&dPtr->boxMin, &min, sizeof(D3DVECTOR));
	 dPtr->m_action					= action;
	 dPtr->m_actionValue			= actionValue;
	 dPtr->m_actionAttrib           = attrib;
	 dPtr->m_zoneLabel				= label;

	 return true;
}
//-----------------------Zone DB------------------------------//
void CActionZoneDB::CompileDirectAccess()
{
    if(m_directAccess)
	{
       delete [] m_directAccess;
       m_directAccess = 0;
	}
	
	m_directAccessObjectCount = (int)GetCount();
	if(m_directAccessObjectCount == 0)
	   return;


    m_directAccess = new DirectAccessStructAZ[m_directAccessObjectCount];
	ZeroMemory(m_directAccess,sizeof(DirectAccessStructAZ)*m_directAccessObjectCount);
	int tracer = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        
		CActionZone * xPtr = (CActionZone *)GetNext(posLoc);
		m_directAccess[tracer].object = xPtr;
		tracer++;  
	}
}

int CActionZoneDB::GetActionBoxByPosition(D3DVECTOR & position, int channel)
{
    if(channel < 0)
	   return 0;

    if(channel >= m_directAccessObjectCount)
	   return 0;

    CActionZone * zonePtr = m_directAccess[channel].object;
	if(zonePtr)
	for(POSITION posLoc = zonePtr->m_boxList.GetHeadPosition();posLoc != NULL;)
	{    
		CActionBox * xBox = (CActionBox *)zonePtr->m_boxList.GetNext(posLoc);
		if(m_matLib.BoundingBoxPointCheck(position,xBox->boxMin,xBox->boxMax))
		   return xBox->m_action;
	}
	return 0;
}

CActionBox * CActionZoneDB::GetActionBoxByType(D3DVECTOR & position, int channel, int type)
{
    if(channel < 0)
	   return 0;

    if(channel >= m_directAccessObjectCount)
	   return 0;

    CActionZone * zonePtr = m_directAccess[channel].object;
	if(zonePtr)
	for(POSITION posLoc = zonePtr->m_boxList.GetHeadPosition();posLoc != NULL;)
	{    
		CActionBox * xBox = (CActionBox *)zonePtr->m_boxList.GetNext(posLoc);
		if(type == xBox->m_action)
		if(m_matLib.BoundingBoxPointCheck(position,xBox->boxMin,xBox->boxMax))
		   return xBox;
	}
	return 0;
}

CActionZone * CActionZoneDB::GetByIndex(int type)
{
    if(type < 0)
	   return 0;

    if(type >= m_directAccessObjectCount)
	   return 0;

    return (CActionZone *)m_directAccess[type].object;
}

BOOL CActionZoneDB::GetAllApplicableActions(D3DVECTOR & position, int channel, ActionListStructure * actionsList)
{
	ZeroMemory(actionsList,sizeof(ActionListStructure));
	actionsList->maxActions = 8;
    
	if(channel < 0)
	   return 0;

    if(channel >= m_directAccessObjectCount)
	   return 0;

    CActionZone * zonePtr = m_directAccess[channel].object;
	if(zonePtr)
	for(POSITION posLoc = zonePtr->m_boxList.GetHeadPosition();posLoc != NULL;)
	{    
		CActionBox * xBox = (CActionBox *)zonePtr->m_boxList.GetNext(posLoc);
		if(m_matLib.BoundingBoxPointCheck(position,xBox->boxMin,xBox->boxMax))
		{
           actionsList->actionArray[actionsList->actionReturnCount] = xBox->m_action;
		   actionsList->actionAmountArray[actionsList->actionReturnCount] = xBox->m_actionValue;
		   actionsList->attribArray[actionsList->actionReturnCount] = xBox->m_actionAttrib;
		   actionsList->actionReturnCount++;
		   if(actionsList->actionReturnCount > actionsList->maxActions)
		      break;
		}
	}

    if(actionsList->actionReturnCount > 0)
	   return TRUE;

	return 0;
}



void CActionZoneDB::AddZone()
{
    CActionZone * newZone = new CActionZone();
    AddTail(newZone);
    CompileDirectAccess();
}

bool CActionZoneDB::DeleteZone(int index)
{
	 int trace = 0;
	 POSITION posLast = NULL;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
		CActionZone * xPtr = (CActionZone *)GetNext(posLoc);
		if(trace == index)
		{
		   delete xPtr;
		   xPtr = 0;
           RemoveAt(posLast);
		   CompileDirectAccess();
           return true;
		}
		trace++;
	 }
	 return false;
}



void CActionZoneDB::ClearAll()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CActionZone * xPtr = (CActionZone *)GetNext(posLoc);
		delete xPtr;
		xPtr = 0; 
	}
	RemoveAll();
    CompileDirectAccess();
}

IMPLEMENT_SERIAL(CActionBox, CObject,VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CActionZone, CObject, 0)
IMPLEMENT_SERIAL(CActionZoneDB, CObList, 0)
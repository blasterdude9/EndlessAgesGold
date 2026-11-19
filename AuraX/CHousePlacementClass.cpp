#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CHousePlacementClass.h"
#include "CHousingClass.h"
#include "CServerEventLogClass.h"
#include "dbconnectionsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CHPlacementObj::CHPlacementObj()
{
    m_type                = 0;//DB reference
    m_accountBind         = 0;//one placement per accout allowed
    m_wldPosition.x       = 0.0f;
	m_wldPosition.y       = 0.0f;
	m_wldPosition.z       = 0.0f;
	m_id                  = 0;
	m_open                = 0;
    m_vaultInstalled      = FALSE;
    m_vaultItemCapacity   = 0;
    m_vaultCurrentCash    = 0;
	m_vaultInventory      = NULL;
	m_storeItemCapacity   = 0;
	m_storeCurrentCash    = 0;
	m_storeInventory      = NULL;
}


void CHPlacementObj::SafeDelete()
{
	if(m_storeInventory)
	{
       m_storeInventory->SafeDelete();
	   delete m_storeInventory;
	   m_storeInventory = 0;
	}
    if(m_vaultInventory)
	{
       m_vaultInventory->SafeDelete();
	   delete m_vaultInventory;
	   m_vaultInventory = 0;
	}

}

void CHPlacementObj::Clone(CHPlacementObj ** clone)
{

	CHPlacementObj * copyLocal = new CHPlacementObj();
	copyLocal->m_type                = m_type;//DB reference
    copyLocal->m_accountBind         = m_accountBind;//one placement per accout allowed
    CopyMemory(&copyLocal->m_wldPosition,&m_wldPosition,sizeof(D3DVECTOR));
	copyLocal->m_id                  = m_id;
	copyLocal->m_open                = m_open;
    copyLocal->m_vaultInstalled      = m_vaultInstalled;
    copyLocal->m_vaultItemCapacity   = m_vaultItemCapacity;
    copyLocal->m_vaultCurrentCash    = m_vaultCurrentCash;
	
	
	if(m_vaultInventory)
	   m_vaultInventory->Clone(&copyLocal->m_vaultInventory);

	copyLocal->m_storeItemCapacity   = m_storeItemCapacity;
	copyLocal->m_storeCurrentCash    = m_storeCurrentCash;

	if(m_storeInventory)
	   m_storeInventory->Clone(&copyLocal->m_storeInventory);

    *clone = copyLocal;

}

void CHPlacementObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHPlacementObj * spnPtr = (CHPlacementObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

CHPlacementObj* CHPlacementObjList::GetByAccountBind(int accountBind)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHPlacementObj * spnPtr = (CHPlacementObj *)GetNext(posLoc);
		if(spnPtr->m_accountBind == accountBind)
           return spnPtr;
	}
	return NULL;
}

CHPlacementObj* CHPlacementObjList::GetHouseByBox(D3DVECTOR position)
{
    BNDBOX clipBoxTranslated;
	
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHPlacementObj * spnPtr = (CHPlacementObj *)GetNext(posLoc);
		clipBoxTranslated.max.x = 150.0f + spnPtr->m_wldPosition.x;
	    clipBoxTranslated.max.y = 150.0f + spnPtr->m_wldPosition.y;
	    clipBoxTranslated.max.z = 150.0f + spnPtr->m_wldPosition.z;
	    clipBoxTranslated.min.x = spnPtr->m_wldPosition.x - 150.0f;
	    clipBoxTranslated.min.y = spnPtr->m_wldPosition.y - 150.0f;
	    clipBoxTranslated.min.z = spnPtr->m_wldPosition.z - 150.0f;
		if(BoundBoxPointCheck(position,clipBoxTranslated.min,clipBoxTranslated.max))
		{
		   
           return spnPtr;
		}
	}
	return NULL;
}

 BOOL CHPlacementObjList::BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max)
{
 if(point.x >= min.x &&
    point.x <= max.x &&
    point.y >= min.y &&
    point.y <= max.y &&
    point.z >= min.z &&
    point.z <= max.z)
  return TRUE;
 return FALSE;
}

BOOL CHPlacementObjList::RemoveByAccountBind(int accountBind)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
        CHPlacementObj * spnPtr = (CHPlacementObj *)GetNext(posLoc);
		if(spnPtr->m_accountBind == accountBind)
		{
           delete spnPtr;
		   spnPtr = 0;
           RemoveAt(posLast);
           return TRUE;
		}
	}
	return FALSE;
}

BOOL CHPlacementObjList::AddPlacement(int type,
									  int accountBind,
									  D3DVECTOR position,
									  int id)
{
     if(GetByAccountBind(accountBind))
		return FALSE;


     CHPlacementObj * m_placementObj = new CHPlacementObj();

	 m_placementObj->m_id             = id;
	 m_placementObj->m_type           = type;
	 m_placementObj->m_accountBind    = accountBind;
     CopyMemory(&m_placementObj->m_wldPosition,&position,sizeof(D3DVECTOR));

	 AddTail(m_placementObj);
     return TRUE;
}


void CHPlacementObjList::Clone(CHPlacementObjList ** clone)
{
     CHPlacementObjList * copyLocal = new CHPlacementObjList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CHPlacementObj * skPtr = (CHPlacementObj *)GetNext(posLoc);
		 CHPlacementObj * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}

void CHPlacementObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() )
	{	
	   ar << m_type
	      << m_accountBind
	      << m_wldPosition.x
	      << m_wldPosition.y
	      << m_wldPosition.z
		  << m_open
		  << m_id
		  << m_vaultInstalled
          << m_vaultItemCapacity
          << m_vaultCurrentCash
	      << m_vaultInventory
		  << m_storeItemCapacity
	      << m_storeCurrentCash
	      << m_storeInventory;

	}
	else
	{
       ar >> m_type
	      >> m_accountBind
	      >> m_wldPosition.x
	      >> m_wldPosition.y
	      >> m_wldPosition.z
		  >> m_open
		  >> m_id
		  >> m_vaultInstalled
          >> m_vaultItemCapacity
          >> m_vaultCurrentCash
	      >> m_vaultInventory
		  >> m_storeItemCapacity
	      >> m_storeCurrentCash
	      >> m_storeInventory;

	   /*m_vaultInstalled      = FALSE;
       m_vaultItemCapacity   = 0;
       m_vaultCurrentCash    = 0;
	   m_vaultInventory      = NULL;*/
	   /*m_storeItemCapacity   = 0;
	   m_storeCurrentCash    = 0;
	   m_storeInventory      = NULL;*/
	}
}     
//--------------------------
//--------------------------
CHousingZone::CHousingZone()
{
    ZeroMemory(&m_visibleBox,sizeof(BNDBOX));
	ZeroMemory(&m_zoneBox,sizeof(BNDBOX));
    m_subDivisionX  = 2;
    m_subDivisionY  = 2;
	m_subDivisionZ  = 2;
	m_wldID         = 0;
	m_densityAllowance = 300.0f;

	m_hsTable       = NULL;
	m_houseDatabase = NULL;
	m_optimizedSave = FALSE;
}

void CHousingZone::Clone(CHousingZone ** clone)
{
	CHousingZone * copyLocal = new CHousingZone();

	CopyMemory(&copyLocal->m_visibleBox,&m_visibleBox,sizeof(BNDBOX));
	CopyMemory(&copyLocal->m_zoneBox,&m_zoneBox,sizeof(BNDBOX));
    copyLocal->m_subDivisionX  = m_subDivisionX;
    copyLocal->m_subDivisionY  = m_subDivisionY;
	copyLocal->m_subDivisionZ  = m_subDivisionZ;
	copyLocal->m_wldID         = m_wldID;
	copyLocal->m_densityAllowance = m_densityAllowance;

	
	if(m_hsTable)
	{
	  copyLocal->m_hsTable = new Placement**[(int)m_subDivisionX];
      for (int m=0; m<m_subDivisionX; m++)
	  {
         copyLocal->m_hsTable[m] = new Placement*[(int)m_subDivisionY];
         for (int n=0; n<m_subDivisionY; n++)
		 {
           copyLocal->m_hsTable[m][n] = new Placement[(int)m_subDivisionZ];
		 }
	  }

	
	    for (int xPos=0; xPos<m_subDivisionX; xPos++){
         for (int yPos=0; yPos<m_subDivisionY; yPos++){
		  for (int zPos=0; zPos<m_subDivisionZ; zPos++){    
			   m_hsTable[xPos][yPos][zPos].m_placementList->Clone(&copyLocal->m_hsTable[xPos][yPos][zPos].m_placementList);
		  }
		 }
		}
	}
	
	if(!m_optimizedSave)
	if(m_houseDatabase)
	   m_houseDatabase->Clone(&copyLocal->m_houseDatabase);

	m_optimizedSave = FALSE;

	*clone = copyLocal;

}

void CHousingZone::SafeDelete()
{
	if(m_hsTable)
	{
	 for (int xPos=0; xPos<m_subDivisionX; xPos++){
      for (int yPos=0; yPos<m_subDivisionY; yPos++){
		  for (int zPos=0; zPos<m_subDivisionZ; zPos++){
		       m_hsTable[xPos][yPos][zPos].m_placementList->SafeDelete();
			   delete m_hsTable[xPos][yPos][zPos].m_placementList;
			   m_hsTable[xPos][yPos][zPos].m_placementList = 0;
		  }
		  delete [] m_hsTable[xPos][yPos];
		  m_hsTable[xPos][yPos]=0;
	  }
      delete [] m_hsTable[xPos];
      m_hsTable[xPos]=0;
	 }
	 delete [] m_hsTable;
     m_hsTable=0;
	}

	if(m_houseDatabase)
	{
       m_houseDatabase->SafeDelete();
	   delete m_houseDatabase;
	   m_houseDatabase = 0;
	}
}

void CHousingZoneDB::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
	DeleteCriticalSection(  &m_cSection );
}

void CHousingZoneDB::ClearAllPlacements()
{
	this->m_currentIDCount = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
		
		if(spnPtr->m_houseDatabase)
           spnPtr->m_houseDatabase->SafeDelete();
	}
}

BOOL CHousingZone::RemoveByAccountBind(int accountBind)
{
    if(!m_houseDatabase)
	   return FALSE;

	   m_houseDatabase->RemoveByAccountBind(accountBind);
  
       if(m_hsTable)
	   {
	    for (int xPos=0; xPos<m_subDivisionX; xPos++){
         for (int yPos=0; yPos<m_subDivisionY; yPos++){
		  for (int zPos=0; zPos<m_subDivisionZ; zPos++){    
			   m_hsTable[xPos][yPos][zPos].m_placementList->RemoveByAccountBind(accountBind);
		  }
		 }
		}
	   }

	   return TRUE;
}


void CHousingZone::CreateNewHouseingZone(CString zoneName,
										 float subX,
										 float subY,
										 float subZ,
										 int wldID,
										 BNDBOX box,
										 BNDBOX visibleBox)
{

     SafeDelete();

	 m_zoneLabel     = zoneName;
	 m_subDivisionX  = subX;
	 m_subDivisionY  = subY;
     m_subDivisionZ  = subZ;
	 m_wldID         = wldID;

	 CopyMemory(&m_zoneBox,&box,sizeof(BNDBOX));
	 CopyMemory(&m_visibleBox,&visibleBox,sizeof(BNDBOX));
	   
	  m_hsTable = new Placement**[(int)m_subDivisionX];
      for (int m=0; m<m_subDivisionX; m++)
	  {
         m_hsTable[m] = new Placement*[(int)m_subDivisionY];
         for (int n=0; n<m_subDivisionY; n++)
		 {
           m_hsTable[m][n] = new Placement[(int)m_subDivisionZ];
		   for (int o=0; o<m_subDivisionZ; o++)
		   {
              m_hsTable[m][n][o].m_placementList = new CHPlacementObjList();
		   }
		 }
	  }
}

 BOOL CHousingZone::BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max)
{
 if(point.x >= min.x &&
    point.x <= max.x &&
    point.y >= min.y &&
    point.y <= max.y &&
    point.z >= min.z &&
    point.z <= max.z)
  return TRUE;
 return FALSE;
}

float CHousingZone::MagnitudeSquared(float &Mx1,float &My1,
								         float &Mz1,float &Mx2,
								         float &My2,float &Mz2){
float temp_Mxyz = (((Mx1 - Mx2) * (Mx1 - Mx2)) + ((My1 - My2) * (My1 - My2)) + ((Mz1 - Mz2) * (Mz1 - Mz2)));

return (float)sqrt(temp_Mxyz);
}

CHPlacementObjList * CHousingZone::GetVisibleList(D3DVECTOR wldPosition,float range)
{
	if(!m_houseDatabase)
		return NULL;

    CHPlacementObjList * visibleList = new CHPlacementObjList();

    for(POSITION posLoc = m_houseDatabase->GetHeadPosition();posLoc != NULL;)
	{
        CHPlacementObj * houseOnTable = (CHPlacementObj *) m_houseDatabase->GetNext(posLoc);
        if(houseOnTable)
		{
		   float dist = MagnitudeSquared(wldPosition.x,wldPosition.y,wldPosition.z,houseOnTable->m_wldPosition.x,houseOnTable->m_wldPosition.y,houseOnTable->m_wldPosition.z);
           if(dist < range)
              visibleList->AddTail(houseOnTable);
		}
	}
    
	if(visibleList->GetCount()==0){//none found
       delete visibleList;
	   visibleList = 0;
     
	   return visibleList;
	}
	else
	return visibleList;
}


CHPlacementObjList * CHousingZone::GetContainerByWldPosition(D3DVECTOR position)
{
     if(!BoundBoxPointCheck(position,m_zoneBox.min,m_zoneBox.max))
         return NULL;
                                //current position              //Container Length                                  
	 int positionXtable = (int)((m_zoneBox.max.x - position.x)/((m_zoneBox.max.x - m_zoneBox.min.x)/m_subDivisionX));
     int positionYtable = (int)((m_zoneBox.max.y - position.y)/((m_zoneBox.max.y - m_zoneBox.min.y)/m_subDivisionY));
	 int positionZtable = (int)((m_zoneBox.max.z - position.z)/((m_zoneBox.max.z - m_zoneBox.min.z)/m_subDivisionZ));

     if(positionXtable >= m_subDivisionX || positionXtable < 0)
		 return NULL;
	 if(positionYtable >= m_subDivisionY || positionYtable < 0)
		 return NULL;
	 if(positionZtable >= m_subDivisionZ || positionZtable < 0)
		 return NULL;

	 //int count = m_hsTable[positionXtable][positionYtable][positionZtable].m_placementList->GetCount();

     return m_hsTable[positionXtable][positionYtable][positionZtable].m_placementList;
}


BOOL CHousingZone::AnyHousesWithinArea(D3DVECTOR position,float radius)
{

    if(!m_houseDatabase)
		return FALSE;

      for(POSITION posLoc = m_houseDatabase->GetHeadPosition();posLoc != NULL;)
	  {
           CHPlacementObj * house = (CHPlacementObj *)m_houseDatabase->GetNext(posLoc);
		   
	       float dist = MagnitudeSquared(house->m_wldPosition.x,house->m_wldPosition.y,
								         house->m_wldPosition.z,position.x,
								         position.y,position.z);

		   if(dist < radius)
	          return TRUE;
	  
	  }
	  return FALSE;
}

int CHousingZone::AddPlacementObjToTable(int type,
                                          int m_accountBind,
                                          BNDBOX m_clipBoxOfObjectPlaced,
										  D3DVECTOR wldPosition,
										  int id)
{

    if(!BoundBoxPointCheck(wldPosition,m_zoneBox.min,m_zoneBox.max))
		return 0;//out of housing zone


	if(AnyHousesWithinArea(wldPosition,m_densityAllowance))
        return 5;//too close to another house

    BNDBOX clipBoxTranslated;
	clipBoxTranslated.max.x = m_clipBoxOfObjectPlaced.max.x + wldPosition.x;
	clipBoxTranslated.max.y = m_clipBoxOfObjectPlaced.max.y + wldPosition.y;
	clipBoxTranslated.max.z = m_clipBoxOfObjectPlaced.max.z + wldPosition.z;
	clipBoxTranslated.min.x = m_clipBoxOfObjectPlaced.min.x + wldPosition.x;
	clipBoxTranslated.min.y = m_clipBoxOfObjectPlaced.min.y + wldPosition.y;
	clipBoxTranslated.min.z = m_clipBoxOfObjectPlaced.min.z + wldPosition.z;


    D3DVECTOR point1,point2,point3,point4,point5,point6,point7,point8;  
    //point 1
	point1.x = clipBoxTranslated.min.x;
	point1.y = clipBoxTranslated.max.y;
	point1.z = clipBoxTranslated.max.z;
	//point 2
	point2.x = clipBoxTranslated.max.x;
	point2.y = clipBoxTranslated.max.y;
	point2.z = clipBoxTranslated.max.z;
	//point 3
	point3.x = clipBoxTranslated.max.x;
	point3.y = clipBoxTranslated.max.y;
	point3.z = clipBoxTranslated.min.z;
	//point 4
	point4.x = clipBoxTranslated.min.x;
	point4.y = clipBoxTranslated.max.y;
	point4.z = clipBoxTranslated.min.z;
    //point 5
	point5.x = clipBoxTranslated.min.x;
	point5.y = clipBoxTranslated.min.y;
	point5.z = clipBoxTranslated.max.z;
	//point 6
	point6.x = clipBoxTranslated.max.x;
	point6.y = clipBoxTranslated.min.y;
	point6.z = clipBoxTranslated.max.z;
    //point 7
	point7.x = clipBoxTranslated.max.x;
	point7.y = clipBoxTranslated.min.y;
	point7.z = clipBoxTranslated.min.z;
    //point 8
	point8.x = clipBoxTranslated.min.x;
	point8.y = clipBoxTranslated.min.y;
	point8.z = clipBoxTranslated.min.z;


    CHPlacementObjList * container1 = GetContainerByWldPosition(point1);
	if(!container1)
		return 0;
    CHPlacementObjList * container2 = GetContainerByWldPosition(point2);
	if(!container2)
		return 0;
	CHPlacementObjList * container3 = GetContainerByWldPosition(point3);
	if(!container3)
		return 0;
	CHPlacementObjList * container4 = GetContainerByWldPosition(point4);
	if(!container4)
		return 0;
	CHPlacementObjList * container5 = GetContainerByWldPosition(point5);
	if(!container5)
		return 0;
	CHPlacementObjList * container6 = GetContainerByWldPosition(point6);
	if(!container6)
		return 0;
	CHPlacementObjList * container7 = GetContainerByWldPosition(point7);
	if(!container7)
		return 0;
    CHPlacementObjList * container8 = GetContainerByWldPosition(point8);
	if(!container8)
		return 0;

	

	container1->AddPlacement(type,m_accountBind,wldPosition,id);
	container2->AddPlacement(type,m_accountBind,wldPosition,id);
	container3->AddPlacement(type,m_accountBind,wldPosition,id);
	container4->AddPlacement(type,m_accountBind,wldPosition,id);
	container5->AddPlacement(type,m_accountBind,wldPosition,id);
	container6->AddPlacement(type,m_accountBind,wldPosition,id);
	container7->AddPlacement(type,m_accountBind,wldPosition,id);
	container8->AddPlacement(type,m_accountBind,wldPosition,id);

	if(!m_houseDatabase)
	    m_houseDatabase = new CHPlacementObjList();
	

	m_houseDatabase->AddPlacement(type,m_accountBind,wldPosition,id);//Tracking database


return 1;
}




void CHousingZone::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() )
	{	
	   ar << m_zoneLabel
	      << m_subDivisionX
	      << m_subDivisionY
	      << m_subDivisionZ
	      << m_wldID
		  << m_visibleBox.max.x
		  << m_visibleBox.max.y
		  << m_visibleBox.max.z
		  << m_visibleBox.min.x
		  << m_visibleBox.min.y
		  << m_visibleBox.min.z
		  << m_zoneBox.max.x
		  << m_zoneBox.max.y
		  << m_zoneBox.max.z
		  << m_zoneBox.min.x
		  << m_zoneBox.min.y
		  << m_zoneBox.min.z
		  << m_densityAllowance;
	   if(!m_optimizedSave)
	       ar << m_houseDatabase;
	   else
	   {
		   CHPlacementObjList * temp = NULL;
		   ar << temp;
	   }

	   m_optimizedSave = FALSE;
	   
      if(m_hsTable)
	  {
	    for (int xPos=0; xPos<m_subDivisionX; xPos++){
         for (int yPos=0; yPos<m_subDivisionY; yPos++){
		  for (int zPos=0; zPos<m_subDivisionZ; zPos++){    
			   ar <<m_hsTable[xPos][yPos][zPos].m_placementList;
		  }
		 }
		}
	  }
	}
	else
	{
       ar >> m_zoneLabel
	      >> m_subDivisionX
	      >> m_subDivisionY
	      >> m_subDivisionZ
	      >> m_wldID
		  >> m_visibleBox.max.x
		  >> m_visibleBox.max.y
		  >> m_visibleBox.max.z
		  >> m_visibleBox.min.x
		  >> m_visibleBox.min.y
		  >> m_visibleBox.min.z
		  >> m_zoneBox.max.x
		  >> m_zoneBox.max.y
		  >> m_zoneBox.max.z
		  >> m_zoneBox.min.x
		  >> m_zoneBox.min.y
		  >> m_zoneBox.min.z
		  >> m_densityAllowance
		  >> m_houseDatabase;

	   m_optimizedSave = FALSE;

	   //m_densityAllowance = 300.0f;

	   
	   m_hsTable = new Placement**[(int)m_subDivisionX];
       for (int m=0; m<m_subDivisionX; m++)
	   {
         m_hsTable[m] = new Placement*[(int)m_subDivisionY];
         for (int n=0; n<m_subDivisionY; n++)
		 {
           m_hsTable[m][n] = new Placement[(int)m_subDivisionZ];
		 }
	   }
	   
	   if(m_hsTable)
	   {
	    for (int xPos=0; xPos<m_subDivisionX; xPos++){
         for (int yPos=0; yPos<m_subDivisionY; yPos++){
		  for (int zPos=0; zPos<m_subDivisionZ; zPos++){
		       ar >> m_hsTable[xPos][yPos][zPos].m_placementList;
		  }
		 }
		}
	   }
	}
}     


void CHousingZone::SerializeAlloc(CArchive& ar , CRITICAL_SECTION * cSection)
{
	if(ar.IsStoring() )
	{	
	   //LeaveCriticalSection(cSection);
	   ar << m_zoneLabel
	      << m_subDivisionX
	      << m_subDivisionY
	      << m_subDivisionZ
	      << m_wldID
		  << m_visibleBox.max.x
		  << m_visibleBox.max.y
		  << m_visibleBox.max.z
		  << m_visibleBox.min.x
		  << m_visibleBox.min.y
		  << m_visibleBox.min.z
		  << m_zoneBox.max.x
		  << m_zoneBox.max.y
		  << m_zoneBox.max.z
		  << m_zoneBox.min.x
		  << m_zoneBox.min.y
		  << m_zoneBox.min.z
		  << m_densityAllowance;
	   if(!m_optimizedSave)
	       ar << m_houseDatabase;
	   else
	   {
		   CHPlacementObjList * temp = NULL;
		   ar << temp;
	   }

	   m_optimizedSave = FALSE;
	   
      if(m_hsTable)
	  {
	    for (int xPos=0; xPos<m_subDivisionX; xPos++){
         for (int yPos=0; yPos<m_subDivisionY; yPos++){
		  for (int zPos=0; zPos<m_subDivisionZ; zPos++){  
               //LeaveCriticalSection(cSection);
               Sleep(0);
			   //EnterCriticalSection(cSection);
			   ar << m_hsTable[xPos][yPos][zPos].m_placementList;
		  }
		 }
		}
	  }
	}
	else
	{
       ar >> m_zoneLabel
	      >> m_subDivisionX
	      >> m_subDivisionY
	      >> m_subDivisionZ
	      >> m_wldID
		  >> m_visibleBox.max.x
		  >> m_visibleBox.max.y
		  >> m_visibleBox.max.z
		  >> m_visibleBox.min.x
		  >> m_visibleBox.min.y
		  >> m_visibleBox.min.z
		  >> m_zoneBox.max.x
		  >> m_zoneBox.max.y
		  >> m_zoneBox.max.z
		  >> m_zoneBox.min.x
		  >> m_zoneBox.min.y
		  >> m_zoneBox.min.z
		  >> m_densityAllowance
		  >> m_houseDatabase;

	   m_optimizedSave = FALSE;

	   //m_densityAllowance = 300.0f;

	   
	   m_hsTable = new Placement**[(int)m_subDivisionX];
       for (int m=0; m<m_subDivisionX; m++)
	   {
         m_hsTable[m] = new Placement*[(int)m_subDivisionY];
         for (int n=0; n<m_subDivisionY; n++)
		 {
           m_hsTable[m][n] = new Placement[(int)m_subDivisionZ];
		 }
	   }
	   
	   if(m_hsTable)
	   {
	    for (int xPos=0; xPos<m_subDivisionX; xPos++){
         for (int yPos=0; yPos<m_subDivisionY; yPos++){
		  for (int zPos=0; zPos<m_subDivisionZ; zPos++){
		       ar >> m_hsTable[xPos][yPos][zPos].m_placementList;
		  }
		 }
		}
	   }
	}
}


void CHousingZoneDB::SerializeAlloc(CArchive& ar , CRITICAL_SECTION * cSection)
{
    if(ar.IsStoring() )
	{	
		EnterCriticalSection(cSection);
		m_lock = TRUE;
		LeaveCriticalSection(cSection);
		int count = (int)GetCount();
		ar << count;
		for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
			CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
			spnPtr->SerializeAlloc(ar, cSection);
			Sleep(0);
		}
		EnterCriticalSection(cSection);
		m_lock = FALSE;
		LeaveCriticalSection(cSection);
	}
	else
	{
	   int count = 0;
       ar >> count;
       for(int loop=0;loop<count;loop++)
	   {
			CHousingZone * spnPtr = new CHousingZone();
			//ar >> spnPtr;
			spnPtr->SerializeAlloc(ar, cSection);
			AddTail(spnPtr);
	   }
	}
}

CHousingZoneDB::CHousingZoneDB()
{
   m_currentIDCount = 1;
   m_lock = FALSE;

   InitializeCriticalSection(  &m_cSection );
}

int CHousingZoneDB::AddPlacementObjToTable(int type,
                                            int m_accountBind,
                                            BNDBOX m_clipBoxOfObjectPlaced,
										    D3DVECTOR wldPosition,
											int * idReturn,
											int wldID)
{
    

    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
		if(wldID != -1)
		if(wldID != spnPtr->m_wldID)
		   continue;
        int result = spnPtr->AddPlacementObjToTable(type,m_accountBind,m_clipBoxOfObjectPlaced,wldPosition,m_currentIDCount);
        if(result != 0)
		{
           if(result == 5)
			  return 5;

           *idReturn = m_currentIDCount;
           m_currentIDCount++;
		   return 1;
		}
	}
	return FALSE;
}

 BOOL CHousingZoneDB::BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max)
{
 if(point.x >= min.x &&
    point.x <= max.x &&
    point.y >= min.y &&
    point.y <= max.y &&
    point.z >= min.z &&
    point.z <= max.z)
  return TRUE;
 return FALSE;
}

CHPlacementObjList * CHousingZoneDB::GetVisibleList(D3DVECTOR wldPosition,float range,int wldID)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
        if(spnPtr->m_wldID != wldID)
		   continue;
 
        if(BoundBoxPointCheck(wldPosition,spnPtr->m_visibleBox.min,spnPtr->m_visibleBox.max))
		{//in this zones visible range
           return spnPtr->GetVisibleList(wldPosition,range);
		}//end in this zones visible range
	}
	return NULL;
}

CHPlacementObjList * CHousingZoneDB::GetContainer(D3DVECTOR wldPosition)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
        if(BoundBoxPointCheck(wldPosition,spnPtr->m_zoneBox.min,spnPtr->m_zoneBox.max))
		{//in this zones visible range
           return spnPtr->GetContainerByWldPosition(wldPosition);
		}//end in this zones visible range
	}
	return NULL;
}

CHPlacementObjList * CHousingZoneDB::GetContainerAndZone(D3DVECTOR wldPosition,CHousingZone ** zone)
{
	if(m_lock)
	   return NULL;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
        if(BoundBoxPointCheck(wldPosition,spnPtr->m_zoneBox.min,spnPtr->m_zoneBox.max))
		{//in this zones visible range
		   *zone = spnPtr;
           return spnPtr->GetContainerByWldPosition(wldPosition);
		}//end in this zones visible range
	}
	return NULL;
}

CHPlacementObj * CHousingZoneDB::GetHousePlacementByPosAcSpecial(D3DVECTOR wldPosition,int acID,int wldID)
{
    if(m_lock)
	   return NULL;

	CHPlacementObj * tempRet = NULL;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
		if(spnPtr->m_wldID == wldID)
		   if(spnPtr->m_houseDatabase)
              for(POSITION posLoc2 = spnPtr->m_houseDatabase->GetHeadPosition();posLoc2 != NULL;)
		      {
               CHPlacementObj * houseOnTable = (CHPlacementObj *) spnPtr->m_houseDatabase->GetNext(posLoc2);
               if(houseOnTable)
			   if(houseOnTable->m_accountBind == acID)
			   {
			     
				  D3DVECTOR max,min;
				  max.x = houseOnTable->m_wldPosition.x + 200.0f;
				  max.y = houseOnTable->m_wldPosition.y + 200.0f;
				  max.z = houseOnTable->m_wldPosition.z + 200.0f;
				  min.x = houseOnTable->m_wldPosition.x - 200.0f;
				  min.y = houseOnTable->m_wldPosition.y - 200.0f;
				  min.z = houseOnTable->m_wldPosition.z - 200.0f;
                  if(BoundBoxPointCheck(wldPosition,min,max))
				  {//in this zones visible range
					 return houseOnTable;
				  }//end in this zones visible range
			   }
		     }
	}
	return NULL;
}

CHPlacementObj * CHousingZoneDB::GetHousePlacementByPosAc(D3DVECTOR wldPosition,int acID)
{
    if(m_lock)
	   return NULL;

	CHPlacementObj * tempRet = NULL;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
        if(BoundBoxPointCheck(wldPosition,spnPtr->m_visibleBox.min,spnPtr->m_visibleBox.max))
		{//in this zones visible range
		   if(spnPtr->m_houseDatabase)
              tempRet = spnPtr->m_houseDatabase->GetByAccountBind(acID);

		   if(tempRet)
			  return tempRet;
		}//end in this zones visible range
	}
	return NULL;
}

CHPlacementObj * CHousingZoneDB::GetHousePlacementByPosAc(D3DVECTOR wldPosition,int acID,int * retWldID)
{
    if(m_lock)
	   return NULL;


	CHPlacementObj * tempRet = NULL;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
        if(BoundBoxPointCheck(wldPosition,spnPtr->m_visibleBox.min,spnPtr->m_visibleBox.max))
		{//in this zones visible range
		   if(spnPtr->m_houseDatabase)
		   {
              tempRet = spnPtr->m_houseDatabase->GetByAccountBind(acID);
			  *retWldID = spnPtr->m_wldID;
		   }

		   if(tempRet)
			  return tempRet;
		}//end in this zones visible range
	}
	return NULL;
}


CHPlacementObj * CHousingZoneDB::GetHousePlacementByPos_area(D3DVECTOR wldPosition)
{
    //CHPlacementObjList * containerList = GetContainer(wldPosition);
	CHousingZone * zone = NULL;
	CHPlacementObjList * containerList = GetContainerAndZone(wldPosition,&zone);
    if(containerList){

	   CHPlacementObj * hsPtr = containerList->GetHouseByBox(wldPosition);
	   if(hsPtr)
	   {
		   if(zone->m_houseDatabase)
	       return zone->m_houseDatabase->GetByAccountBind(hsPtr->m_accountBind);
	   }
	}

	return NULL;
}


void CHousingZoneDB::RemoveHouseByAccountBind(int acID)
{
	if(m_lock)
	   return;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
        spnPtr->RemoveByAccountBind(acID);
	}
}

CHPlacementObj * CHousingZoneDB::GetHouseByAccountBind(int acID)
{
	if(m_lock)
	   return NULL;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
		if(spnPtr->m_houseDatabase)
		{
		  CHPlacementObj * housePlPtr = spnPtr->m_houseDatabase->GetByAccountBind(acID);
		  if(housePlPtr)
		     return housePlPtr;
		}
	}

	return NULL;
}

int CHousingZoneDB::RemoveHouseByPosition(D3DVECTOR wldPosition,int wldID)
{
    if(m_lock)
	   return -1;
 

    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
        
		if(spnPtr->m_wldID == wldID)
        if(BoundBoxPointCheck(wldPosition,spnPtr->m_visibleBox.min,spnPtr->m_visibleBox.max))
		{//in this zones visible range
		   if(spnPtr->m_houseDatabase)
		   for(POSITION posLoc2 = spnPtr->m_houseDatabase->GetHeadPosition();posLoc2 != NULL;)
		   {
               CHPlacementObj * houseOnTable = (CHPlacementObj *) spnPtr->m_houseDatabase->GetNext(posLoc2);
               if(houseOnTable)
			   {
				  D3DVECTOR max,min;
				  max.x = houseOnTable->m_wldPosition.x + 200.0f;
				  max.y = houseOnTable->m_wldPosition.y + 200.0f;
				  max.z = houseOnTable->m_wldPosition.z + 200.0f;
				  min.x = houseOnTable->m_wldPosition.x - 200.0f;
				  min.y = houseOnTable->m_wldPosition.y - 200.0f;
				  min.z = houseOnTable->m_wldPosition.z - 200.0f;
                  if(BoundBoxPointCheck(wldPosition,min,max))
				  {//in this zones visible range
				     int acID = houseOnTable->m_accountBind;
                     spnPtr->RemoveByAccountBind(houseOnTable->m_accountBind);
					 return acID;
				  }//end in this zones visible range
			   }
		   }
		}//end in this zones visible range
	}
	return -1;
}

CHPlacementObj *  CHousingZoneDB::GetHouseByPositionAndWldID(D3DVECTOR wldPosition,int wldID)
{
    if(m_lock)
	   return NULL;

    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)GetNext(posLoc);
		if(spnPtr->m_wldID == wldID)
        if(BoundBoxPointCheck(wldPosition,spnPtr->m_visibleBox.min,spnPtr->m_visibleBox.max))
		{//in this zones visible range
		   if(spnPtr->m_houseDatabase)
		   for(POSITION posLoc2 = spnPtr->m_houseDatabase->GetHeadPosition();posLoc2 != NULL;)
		   {
               CHPlacementObj * houseOnTable = (CHPlacementObj *) spnPtr->m_houseDatabase->GetNext(posLoc2);
               if(houseOnTable)
			   {
				  D3DVECTOR max,min;
				  max.x = houseOnTable->m_wldPosition.x + 200.0f;
				  max.y = houseOnTable->m_wldPosition.y + 200.0f;
				  max.z = houseOnTable->m_wldPosition.z + 200.0f;
				  min.x = houseOnTable->m_wldPosition.x - 200.0f;
				  min.y = houseOnTable->m_wldPosition.y - 200.0f;
				  min.z = houseOnTable->m_wldPosition.z - 200.0f;
                  if(BoundBoxPointCheck(wldPosition,min,max))
				  {//in this zones visible range
					 return houseOnTable;
				  }//end in this zones visible range
			   }
		   }
		}//end in this zones visible range
	}
	return NULL;
}

void CHousingZoneDB::Clone(CHousingZoneDB ** clone)
{
     CHousingZoneDB * copyLocal = new CHousingZoneDB();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CHousingZone * skPtr = (CHousingZone *)GetNext(posLoc);
		 CHousingZone * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}


BOOL CHousingZoneDB::ImportFromSql(void *eventLogMem,void *houseDBMem,void * globalInvenMem)
{
    

    /*CServerEventList			* eventLog			= (CServerEventList *)eventLogMem;
	CHousingObjList             * hsObjList         = (CHousingObjList *)houseDBMem;
	CGlobalInventoryObjList		* glInventoryRef	= (CGlobalInventoryObjList *)globalInvenMem;

	CABFunctionLib cabLib;
	ConnectionStructureClass connectionStructTemp;
    connectionStructTemp.AquireFromCfgFile("baseAccount.cfg");
    CDBConnectionSystem * dbConnection = new CDBConnectionSystem(connectionStructTemp.m_sqlServerIP,
			                                                    connectionStructTemp.m_sqlServerCatalog,
																connectionStructTemp.m_sqlServerUsername,
																connectionStructTemp.m_sqlServerPassword,
																eventLog);


	if(!dbConnection->m_connectionEstablished)
	{
	    if(eventLog)
	       eventLog->AddEvent("ImportFromSql() HOUSEING Aborted due to connection not being established");
        return FALSE;
	}

	//clear existing
	this->ClearAllPlacements();

	CString getAllPlacments = "SELECT * FROM GM_SERVERPLACEMENTS";
    CString getAllInventoryForPlacement = "SELECT * FROM GM_PLACEMENTINVENTORY WHERE ACCOUNTBIND = '$ACCOUNTBIND' ORDER BY ORDERINDEX";



	_RecordsetPtr plcReader = dbConnection->Query(getAllPlacments);
    int plcCount = dbConnection->GetCount(plcReader);

	if(eventLog)
	   eventLog->AddEvent("Placing Houses from DB count:",(DWORD)plcCount);
  
	CString tempQuerry;
	D3DVECTOR tempVect;
	for(int plcLoop = 0;plcLoop < plcCount;plcLoop++)
	{
        
        int accountBind			= dbConnection->GetIntData("ACCOUNTBIND",plcReader);
		//int worldChannel		= dbConnection->GetIntData("WORLDCHANNEL",plcReader);
		tempVect.x				= dbConnection->GetfloatData("POSX",plcReader);
		tempVect.y				= dbConnection->GetfloatData("POSY",plcReader);
		tempVect.z				= dbConnection->GetfloatData("POSZ",plcReader);
		int type				= dbConnection->GetIntData("TYPE",plcReader);
		int vaultInstalled		= dbConnection->GetIntData("VAULTINSTALLED",plcReader);
		int vaultCapacity		= dbConnection->GetIntData("VAULTCAPACITY",plcReader);
		int vaultCash			= dbConnection->GetIntData("VAULTCURRENTCASH",plcReader);
		int storeCapacity		= dbConnection->GetIntData("STOREITEMCAPACITY",plcReader);
		int storeCash			= dbConnection->GetIntData("STORECURRENTCASH",plcReader);

		//aquire house meodel
        CHousingObj * hsModelPtr = hsObjList->GetByIndex(type);
		if(!hsModelPtr)
		{
            if(eventLog)
			   eventLog->AddEvent("ImportFromSql() Invalid house type AC ID:",(DWORD)accountBind);

			dbConnection->NextRecord(plcReader);
            continue;
		}


		BNDBOX newBox;
		newBox.min.x = hsModelPtr->m_clipBox.minX;
		newBox.min.y = hsModelPtr->m_clipBox.minY;
		newBox.min.z = hsModelPtr->m_clipBox.minZ;
		newBox.max.x = hsModelPtr->m_clipBox.maxX;
		newBox.max.y = hsModelPtr->m_clipBox.maxY;
		newBox.max.z = hsModelPtr->m_clipBox.maxZ;
		//add to live table
		int idReturn = 0;

        if(GetHouseByAccountBind(accountBind))
		{//already placed a house
           eventLog->AddEvent("ImportFromSql() DUPLICATE AC ID(refuse placement):",(DWORD)accountBind);
		   dbConnection->NextRecord(plcReader);
           continue;
		}//end already placed a house

		int result		= AddPlacementObjToTable(type,//int type,
                                                 accountBind,//int m_accountBind,
                                                 newBox,//BNDBOX m_clipBoxOfObjectPlaced,
										         tempVect,//D3DVECTOR wldPosition,
											     &idReturn,//int * idReturn,
											     -1);//int wldID);

		CHPlacementObj * houesThatwasPlaced = GetHouseByAccountBind(accountBind);
		if(!houesThatwasPlaced)
		{//error
           if(eventLog)
			   eventLog->AddEvent("ImportFromSql() HOUSEING House failed to place AC ID:",(DWORD)accountBind);

		   dbConnection->NextRecord(plcReader);
           continue;
		}//end error
        //fill misc data
      
		houesThatwasPlaced->m_vaultInstalled			= vaultInstalled;
	    houesThatwasPlaced->m_vaultItemCapacity			= vaultCapacity;
	    houesThatwasPlaced->m_vaultCurrentCash			= vaultCash;
	    houesThatwasPlaced->m_vaultInventory			= new CInventoryObjList();
	    houesThatwasPlaced->m_storeItemCapacity			= storeCapacity;
	    houesThatwasPlaced->m_storeCurrentCash			= storeCash;
	    houesThatwasPlaced->m_storeInventory			= new CInventoryObjList();
		//fill Inventory
		tempQuerry = getAllInventoryForPlacement;
		tempQuerry.Replace("$ACCOUNTBIND",cabLib.ConvToString(accountBind));
        _RecordsetPtr invReader = dbConnection->Query(tempQuerry);
        int invenCount = dbConnection->GetCount(invReader);
        for(int loopInv = 0;loopInv < invenCount; loopInv++)
		{
			int glid					= dbConnection->GetIntData("ITEMGIL",invReader);
			int price					= dbConnection->GetIntData("PRICE",invReader);
			int storageLocation			= dbConnection->GetIntData("STORAGELOCATION",invReader);
			int quanity					= dbConnection->GetIntData("QUANITY",invReader);
			int dmgMod					= dbConnection->GetIntData("DMGMOD",invReader);

		    CGlobalInventoryObj * invGLPtr = glInventoryRef->GetByGLID(glid);
		    if(invGLPtr)
			{
			   if(invGLPtr->m_itemData->m_stackable)
			   {
			      CInventoryObj * newItem = new CInventoryObj();
			      invGLPtr->m_itemData->Clone(&newItem->m_itemData);
			      newItem->m_itemData->m_marketCost				= price;
			      newItem->m_quanity							= quanity;
				  newItem->m_itemData->m_damageModifier		    = dmgMod;
                  if(storageLocation == 0)
			         houesThatwasPlaced->m_storeInventory->AddTail(newItem);
			      else
				     houesThatwasPlaced->m_vaultInventory->AddTail(newItem);
			   }
			   else
			   {
                  for(int QXloop=0;QXloop < quanity;QXloop++)
				  {
					  CInventoryObj * newItem = new CInventoryObj();
			          invGLPtr->m_itemData->Clone(&newItem->m_itemData);
			          newItem->m_itemData->m_marketCost				= price;
			          newItem->m_quanity							= 1;
					  newItem->m_itemData->m_damageModifier			= dmgMod;
                      if(storageLocation == 0)
			             houesThatwasPlaced->m_storeInventory->AddTail(newItem);
			          else
				         houesThatwasPlaced->m_vaultInventory->AddTail(newItem);
				  }
			   }
			}
            dbConnection->NextRecord(invReader);
		}
		dbConnection->Close(invReader);
		dbConnection->NextRecord(plcReader);
	}
	dbConnection->Close(plcReader);

	delete dbConnection;
	dbConnection = 0;

	if(eventLog)
	   eventLog->AddEvent("Placing Houses from DB completed");*/

	return TRUE;
}



BOOL CHousingZoneDB::ExportToSQL(void *eventLogMem)
{
     CServerEventList			* eventLog			= (CServerEventList *)eventLogMem;
	 //CGlobalInventoryObjList	* glInventoryRef	= (CGlobalInventoryObjList *)globalInvenMem;


     CABFunctionLib cabLib;
	 ConnectionStructureClass connectionStructTemp;
     connectionStructTemp.AquireFromCfgFile("baseAccount.cfg");
     CDBConnectionSystem * dbConnection = new CDBConnectionSystem(connectionStructTemp.m_sqlServerIP,
			                                                     connectionStructTemp.m_sqlServerCatalog,
																 connectionStructTemp.m_sqlServerUsername,
																 connectionStructTemp.m_sqlServerPassword,
																 eventLog);


	 if(!dbConnection->m_connectionEstablished)
	 {
		if(eventLog)
	       eventLog->AddEvent("GetFromSQL() Aborted due to connection not being established");
        return FALSE;
	 }

	 //clear existing
	 dbConnection->ExecQuery("DELETE FROM GM_SERVERPLACEMENTS");
	 dbConnection->ExecQuery("DELETE FROM GM_PLACEMENTINVENTORY");

	 //setup querry
	 CString addPlacement = "INSERT INTO GM_SERVERPLACEMENTS (ACCOUNTBIND, WORLDCHANNEL, POSX, POSY, POSZ, TYPE, VAULTINSTALLED, VAULTCAPACITY, VAULTCURRENTCASH, STOREITEMCAPACITY, STORECURRENTCASH) Values('$ACCOUNTBIND','$WORLDCHANNEL','$POSX','$POSY','$POSZ','$TYPE','$VAULTINSTALLED','$VAULTCAPACITY','$VAULTCURRENTCASH','$STOREITEMCAPACITY','$STORECURRENTCASH')";
     CString addInventory = "INSERT INTO GM_PLACEMENTINVENTORY (ACCOUNTBIND, ORDERINDEX, ITEMGIL, QUANITY, PRICE, STORAGELOCATION, ITEMNAME) Values('$ACCOUNTBIND','$ORDERINDEX','$ITEMGIL','$QUANITY','$PRICE','$STORAGELOCATION','$ITEMNAME')";


	 CString tempQuerry;
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CHousingZone * housingZone = (CHousingZone *)GetNext(posLoc);
		
         if(housingZone->m_houseDatabase)
         for(POSITION pPos = housingZone->m_houseDatabase->GetHeadPosition();pPos != NULL;)
		 {
             CHPlacementObj * houseOnTable = (CHPlacementObj *)housingZone->m_houseDatabase->GetNext(pPos);//inner list, not this zone list
             tempQuerry = addPlacement;
			 tempQuerry.Replace("$ACCOUNTBIND",cabLib.ConvToString(houseOnTable->m_accountBind));
             tempQuerry.Replace("$WORLDCHANNEL","0");
             tempQuerry.Replace("$POSX",cabLib.FloatToStingCnv(houseOnTable->m_wldPosition.x));
             tempQuerry.Replace("$POSY",cabLib.FloatToStingCnv(houseOnTable->m_wldPosition.y));
			 tempQuerry.Replace("$POSZ",cabLib.FloatToStingCnv(houseOnTable->m_wldPosition.z));
			 tempQuerry.Replace("$TYPE",cabLib.ConvToString(houseOnTable->m_type));
             tempQuerry.Replace("$VAULTINSTALLED",cabLib.ConvToString(houseOnTable->m_vaultInstalled));
			 tempQuerry.Replace("$VAULTCAPACITY",cabLib.ConvToString(houseOnTable->m_vaultItemCapacity));
			 tempQuerry.Replace("$VAULTCURRENTCASH",cabLib.ConvToString(houseOnTable->m_vaultCurrentCash));
			 tempQuerry.Replace("$STOREITEMCAPACITY",cabLib.ConvToString(houseOnTable->m_storeItemCapacity));
			 tempQuerry.Replace("$STORECURRENTCASH",cabLib.ConvToString(houseOnTable->m_storeCurrentCash));
             dbConnection->ExecQuery(tempQuerry);
			 //add bound inventory
			 int orderIndex = 0;
             if(houseOnTable->m_storeInventory)
			 for(POSITION posLoc = houseOnTable->m_storeInventory->GetHeadPosition();posLoc != NULL;)
			 {
                 CInventoryObj * invPtr = (CInventoryObj *) houseOnTable->m_storeInventory->GetNext(posLoc);
				 tempQuerry = addInventory;
			     tempQuerry.Replace("$ACCOUNTBIND",cabLib.ConvToString(houseOnTable->m_accountBind));
				 tempQuerry.Replace("$ITEMNAME",dbConnection->SanitizeFormatIn(invPtr->m_itemData->m_itemName));
                 tempQuerry.Replace("$ITEMGIL",cabLib.ConvToString(invPtr->m_itemData->m_globalID));
				 tempQuerry.Replace("$QUANITY",cabLib.ConvToString(invPtr->m_quanity));
				 tempQuerry.Replace("$ORDERINDEX",cabLib.ConvToString(orderIndex));
                 tempQuerry.Replace("$PRICE",cabLib.ConvToString(invPtr->m_itemData->m_marketCost));
				 tempQuerry.Replace("$STORAGELOCATION","0");

				 dbConnection->ExecQuery(tempQuerry);
				 orderIndex++;
			 }

			 if(houseOnTable->m_vaultInventory)
			 for(POSITION posLoc = houseOnTable->m_vaultInventory->GetHeadPosition();posLoc != NULL;)
			 {
                 CInventoryObj * invPtr = (CInventoryObj *) houseOnTable->m_vaultInventory->GetNext(posLoc);
				 tempQuerry = addInventory;
			     tempQuerry.Replace("$ACCOUNTBIND",cabLib.ConvToString(houseOnTable->m_accountBind));
				 tempQuerry.Replace("$ITEMNAME",dbConnection->SanitizeFormatIn(invPtr->m_itemData->m_itemName));
                 tempQuerry.Replace("$ITEMGIL",cabLib.ConvToString(invPtr->m_itemData->m_globalID));
				 tempQuerry.Replace("$QUANITY",cabLib.ConvToString(invPtr->m_quanity));
				 tempQuerry.Replace("$ORDERINDEX",cabLib.ConvToString(orderIndex));
                 tempQuerry.Replace("$PRICE",cabLib.ConvToString(invPtr->m_itemData->m_marketCost));
				 tempQuerry.Replace("$STORAGELOCATION","1");

				 dbConnection->ExecQuery(tempQuerry);
				 orderIndex++;
			 }
		 }
	 }

	 delete dbConnection;
	 dbConnection = 0;
	 return TRUE;
}



IMPLEMENT_SERIAL(CHousingZone, CObject, 0)
IMPLEMENT_SERIAL(CHousingZoneDB, CObList, 0)
IMPLEMENT_SERIAL(CHPlacementObj, CObject, 0)
IMPLEMENT_SERIAL(CHPlacementObjList, CObList, 0)

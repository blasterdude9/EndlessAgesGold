#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CSkeletalLODClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------------//
//-----class is for persisting characters over a network-----//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkeletalLODObject::CSkeletalLODObject()
{
   m_distance = 0;
   m_flagTemp = 0;
}

void CSkeletalLODObject::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_distance;
	   
	}
	else
	{
    ar >> m_distance;

	}
}

void CSkeletalLODObject::SafeDelete()
{
	
}

void CSkeletalLODObject::Clone(CSkeletalLODObject ** copy)
{
  CSkeletalLODObject * copyLocal = new CSkeletalLODObject();
  copyLocal->m_distance = m_distance;
  
  *copy = copyLocal;
}

void CSkeletalLODObjectList::Clone(CSkeletalLODObjectList ** copy)
{
	CSkeletalLODObjectList * copyLocal = new CSkeletalLODObjectList();
    for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CSkeletalLODObject * skPtr = (CSkeletalLODObject *)GetNext(posLoc);
	    CSkeletalLODObject * newPtr = NULL;
	    skPtr->Clone(&newPtr);
	    copyLocal->AddTail(newPtr);
	}
    *copy = copyLocal;
}

int CSkeletalLODObjectList::GetLodIndex(float & currentDistance)
{
	int final = 0;
	int index = 0;
    for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CSkeletalLODObject * skPtr = (CSkeletalLODObject *)GetNext(posLoc);
		index++;
		if(currentDistance > skPtr->m_distance)
		{
           final = index;
		}
		
	}
	return final;
}

int CSkeletalLODObjectList::GetLod(D3DMATRIX & cam, D3DMATRIX & oPos)
{
	int final = 0;
	int index = 0;
	float temp = 0.0f;
	//CSkeletalLODObject * skPtrTemp = NULL;
    for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CSkeletalLODObject * skPtr = (CSkeletalLODObject *)GetNext(posLoc);
		index++;
		
		if(!BoundingSphereTest(skPtr->m_distance,temp,
                               cam._41,cam._42,cam._43,
							   oPos._41,oPos._42,oPos._43))
		{
           final = index;
		   //skPtrTemp = skPtr;
		}
		
	}
    /*if(skPtrTemp)
	{
	   if(skPtrTemp->m_flagTemp == 0)
          skPtrTemp->m_flagTemp = 1;
	   else
	      return -final;
	}*/

	return final;
}
//-------------------------------------------------------------//
void CSkeletalLODObjectList::ResetFlags()
{
    for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;)
	{//account list
        CSkeletalLODObject * skPtr = (CSkeletalLODObject *)GetNext(posLoc);
		skPtr->m_flagTemp = 0;
	}
}

//---This Function will determine bounding sphere collision----//
BOOL CSkeletalLODObjectList::BoundingSphereTest(float & radius1,float & radius2,
								   float & x1,float & y1,float & z1,
								   float & x2,float & y2,float & z2)
{
float MinDist,dist;
MinDist = radius1+radius2;
MinDist = MinDist * MinDist;
dist = (x1-x2)*(x1-x2);
   if(dist <= MinDist)
   {
      dist = dist+(y1-y2)*(y1-y2);
	  if(dist <= MinDist)
	  {
       dist = dist+(z1-z2)*(z1-z2);
	   if(dist <= MinDist)
	      return TRUE;
	  }
   }
return FALSE;
}


void CSkeletalLODObjectList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CSkeletalLODObject * skPtr = (CSkeletalLODObject *)GetNext(posLoc);
		skPtr->SafeDelete();
		delete skPtr;
		skPtr=0;
	}
	RemoveAll();
}


IMPLEMENT_SERIAL(CSkeletalLODObject, CObject, 0)
IMPLEMENT_SERIAL(CSkeletalLODObjectList, CObList, 0)


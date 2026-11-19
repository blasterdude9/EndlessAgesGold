#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include "CStructuresMisl.h"
#include <afxtempl.h>
#include "CNodeObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNodeObj::CNodeObj()
{
   m_libraryReference = 0;
   m_refPtr = NULL;
   ZeroMemory(&m_clipBox,sizeof(ABBOX));
}

void CNodeObj::SafeDelete()
{
  
}


void CNodeObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_libraryReference
		  << m_clipBox.minX
		  << m_clipBox.minY
		  << m_clipBox.minZ
		  << m_clipBox.maxX
		  << m_clipBox.maxY
		  << m_clipBox.maxZ;

       for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		 ar << m_position.m[i][j];

	}
	else
	{
       ar >> m_libraryReference
		  >> m_clipBox.minX
		  >> m_clipBox.minY
		  >> m_clipBox.minZ
		  >> m_clipBox.maxX
		  >> m_clipBox.maxY
		  >> m_clipBox.maxZ;

	   for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
		 ar >> m_position.m[i][j];
	}
}     

void CNodeObjList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CNodeObj * nodePtr = (CNodeObj *)GetNext(posLoc);
         nodePtr->SafeDelete();
		 delete nodePtr;
		 nodePtr = 0;
	 }
	 RemoveAll();
}


IMPLEMENT_SERIAL(CNodeObj, CObject, 0)
IMPLEMENT_SERIAL(CNodeObjList, CObList, 0)


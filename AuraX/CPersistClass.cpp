#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CPersistClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------//
//-------------------------Movement object/list------------------//
//---------------------------------------------------------------//
//contains the mesh / file name ect  list of possible animations-//

CPersistObj::CPersistObj(int             L_entDBIndex,
		                 D3DMATRIX       L_entMatrix,
						 int             L_entControlType
			             )
{
List_entControlType = L_entControlType;
List_entDBIndex = L_entDBIndex;
List_entMatrix = L_entMatrix;
}

void CPersistObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar  << List_entDBIndex
		   << List_entControlType;
	   //arrays ect
	   for(int loop=0;loop<4;loop++)
       for(int loop2=0;loop2<4;loop2++)
           ar << List_entMatrix.m[loop][loop2];
		
	}
	else
	{
       ar  >> List_entDBIndex
		   >> List_entControlType;
	   //arrays ect
	   for(int loop=0;loop<4;loop++)
       for(int loop2=0;loop2<4;loop2++)
           ar >> List_entMatrix.m[loop][loop2];
	}
}

//-------------------Deconstructor----------------------------------//
void CPersistObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CPersistObj * ptRang = (CPersistObj *)GetNext(posLoc);
		delete ptRang;
		ptRang = 0;
	}
	RemoveAll();
}

IMPLEMENT_SERIAL(CPersistObj, CObject, 0)
IMPLEMENT_SERIAL(CPersistObjList, CObList, 0)


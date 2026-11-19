#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CSpawnClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSpawnObj::CSpawnObj()
{
GL_matrixFunctions.InitMat(List_spawnMatrix.m);
List_spawnName          = "Unamed Spawn Point";
List_maxOutPutCapacity  = 1;
List_currentOutPut      = 0;
List_possibilityOfSpawn = 500;//% out of 1000 once per minute evaluation
List_spawnIndex         = 0;
List_spawnRadius        = 0.0f;
List_spawnType          = 5;
m_treeUse               = -1;//-1 = none
m_treePosition          = -1;//-1 = rendom
m_aiCfg                 = 0;
}


void CSpawnObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CSpawnObj * spnPtr = (CSpawnObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CSpawnObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << List_spawnName
	      << List_maxOutPutCapacity
	      << m_aiCfg
	      << List_possibilityOfSpawn
	      << List_spawnIndex
		  << List_spawnRadius
		  << List_spawnType
	      << m_treeUse
          << m_treePosition;

	   for(int loop=0;loop<4;loop++)
           for(int loop2=0;loop2<4;loop2++)
       ar << List_spawnMatrix.m[loop][loop2];
	}
	else
	{
       ar >> List_spawnName
	      >> List_maxOutPutCapacity
	      >> m_aiCfg
	      >> List_possibilityOfSpawn
	      >> List_spawnIndex
		  >> List_spawnRadius
		  >> List_spawnType
		  >> m_treeUse
          >> m_treePosition;

	   for(int loop=0;loop<4;loop++)
           for(int loop2=0;loop2<4;loop2++)
       ar >> List_spawnMatrix.m[loop][loop2];

		   List_currentOutPut=0;
	}
}     

CSpawnObjList::CSpawnObjList()
{
     m_recentTotalOutput = 0;
	 m_enable            = FALSE;
}

long CSpawnObjList::GetTotalAIOutput()
{
    long total = 0;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CSpawnObj * spwnPtr = (CSpawnObj *) GetNext(posLoc);
        total += spwnPtr->List_maxOutPutCapacity;
	}
	m_recentTotalOutput = total;
	return total;
}

IMPLEMENT_SERIAL(CSpawnObj, CObject, 0)
IMPLEMENT_SERIAL(CSpawnObjList, CObList, 0)


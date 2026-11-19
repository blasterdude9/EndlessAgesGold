
#include "matrixarb.h"
#ifndef CSPAWNOBJCLASS_H
#define CSPAWNOBJCLASS_H



class CSpawnObj : public CObject
{
	
     
	DECLARE_SERIAL( CSpawnObj );
	
    
public:
    CSpawnObj();
    MatrixARB                  GL_matrixFunctions;
	CString                    List_spawnName;
    D3DMATRIX                  List_spawnMatrix;
	int                        List_maxOutPutCapacity;
	int                        List_currentOutPut;
    int                        List_possibilityOfSpawn;//% out of 1000
	int                        List_spawnIndex;
	float                      List_spawnRadius;
	int                        List_spawnType;
	int                        m_aiCfg;//if used
 
	//functions//

	//tree use
	int                        m_treeUse;//-1 = none
	int                        m_treePosition;//-1 = rendom


  
	void Serialize( CArchive& ar );
	
};

class CSpawnObjList : public CObList
{
public:
   CSpawnObjList();
   void SafeDelete();
    
     
	DECLARE_SERIAL( CSpawnObjList );
	
    

    long m_recentTotalOutput;

	long GetTotalAIOutput();
	BOOL m_enable;
};

#endif
#pragma once
#include "sharednet.h"

class CSystemsLoader
{
public:
	CSystemsLoader(CSharedNet * sharedInterface);
	~CSystemsLoader(void);

	//Vars
	CSharedNet			* m_sharedInterfaceRef;
	CString				  m_dataPath;
	
	

	//Functions
	BOOL LoadDataByType(CString filename);
	void ProcessLoadDataTable(CString tableName);
	void ProcessMainCfg(CString tableName);
	BOOL LoadSupportedWorldsFromDB();
	BOOL LoadSpawnPointsFromDB();
	BOOL LoadSupportedArenaItems();
};

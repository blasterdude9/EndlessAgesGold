#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CServerItemClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CServerItemObj::CServerItemObj()
{
    m_world					= 0;
	m_dbRef					= 0;
    m_position.x			= 0.0f;
    m_position.y			= 0.0f;
    m_position.z			= 0.0f;
    m_curAnim				=  -1;
    m_glid					=  0;
    m_spawnID				= -1;//-1 = non generator spawned
	m_refSpawnId			= -1;
	m_networkAssignedID		= 0;
	m_requiredKey			= 0;//0=none
	m_contents				= NULL;
	m_spawnCfg				= NULL;
    m_spawnCfgCount			= 0;
	m_staticItemType		= LOOTABLE_CHEST;
	m_rotationAdjust		= 0;
	m_chatStrings			= NULL;//
	m_chatStringCount		= 0;//
	m_chatStringCurIndex	= 0;
	m_collectionQuestDB		= NULL;

	m_randomAnimation = -1;//-1 = none
	m_randomExpEffect = -1;//-1 = none
	m_mountOption     = -1;//-1 = none
	m_randomInterval  = 1000;
	m_organizeGroupID = 0;
	m_ranTimeStamp = GetTickCount();
	m_noResponseMsg = "...";
	m_sightRange = 1000.0f;
	m_interactRadius = 0.0f;
	m_autoTrigger = 0;
	m_animDirection = 0;

	m_useExactOrientation = false;
	ZeroMemory(&m_upVector,sizeof(D3DVECTOR));
	ZeroMemory(&m_dirVector,sizeof(D3DVECTOR));
}

CServerItemObj::~CServerItemObj()
{
        if(m_contents)
		{
		   m_contents->SafeDelete();
		   delete m_contents;
		   m_contents = 0;
		}

		if(m_spawnCfg)
		{
		   delete [] m_spawnCfg;
		   m_spawnCfg = 0;
		}

		if(m_chatStrings)
		{
		   delete [] m_chatStrings;
		   m_chatStrings = 0;
		}

		if(m_collectionQuestDB)
		{
		   m_collectionQuestDB->SafeDelete();
		   delete m_collectionQuestDB;
		   m_collectionQuestDB = 0;
		}
}

void CServerItemObj::Clone(CServerItemObj ** clone)
{
     CServerItemObj * copyLocal     = new CServerItemObj();
	 copyLocal->m_curAnim           = m_curAnim;
     copyLocal->m_dbRef             = m_dbRef;
	 copyLocal->m_position.x        = m_position.x;
	 copyLocal->m_position.y        = m_position.y;
	 copyLocal->m_position.z        = m_position.z;
     copyLocal->m_glid              = m_glid;
     copyLocal->m_spawnID           = m_spawnID;//-1 = non generator spawned
	 copyLocal->m_networkAssignedID = m_networkAssignedID;
	 copyLocal->m_requiredKey       = m_requiredKey;
	 copyLocal->m_world             = m_world;
	 copyLocal->m_spawnCfgCount     = m_spawnCfgCount;
	 copyLocal->m_displayedName     = m_displayedName;
	 copyLocal->m_staticItemType    = m_staticItemType;
	 copyLocal->m_rotationAdjust    = m_rotationAdjust;
	 copyLocal->m_chatStringCount   = m_chatStringCount;

	 copyLocal->m_randomAnimation   = m_randomAnimation;//-1 = none
	 copyLocal->m_randomExpEffect   = m_randomExpEffect;//-1 = none
	 copyLocal->m_mountOption       = m_mountOption;//-1 = none
	 copyLocal->m_randomInterval    = m_randomInterval;
	 copyLocal->m_organizeGroupID   = m_organizeGroupID;
	 copyLocal->m_noResponseMsg     = m_noResponseMsg;
	 copyLocal->m_sightRange        = m_sightRange;
	 copyLocal->m_interactRadius    = m_interactRadius;
	 copyLocal->m_autoTrigger		= m_autoTrigger;
	 copyLocal->m_botBrain          = m_botBrain;

	 copyLocal->m_useExactOrientation	= m_useExactOrientation;
	 copyLocal->m_upVector				= m_upVector;
	 copyLocal->m_dirVector				= m_dirVector;
	 copyLocal->m_animDirection			= m_animDirection;


	 if(m_spawnCfg)
	 {
        copyLocal->m_spawnCfg = new int[m_spawnCfgCount];
        if(m_spawnCfgCount > 0)
			CopyMemory(copyLocal->m_spawnCfg,m_spawnCfg,(sizeof(int)*m_spawnCfgCount));
	 }

	 if(m_chatStringCount > 0)
	 if(m_chatStrings)
	 {
        copyLocal->m_chatStrings = new CString[m_chatStringCount];
        for(int i=0;i<m_chatStringCount;i++)
	        copyLocal->m_chatStrings[i] = m_chatStrings[i];
	 }

     if(m_contents)
		m_contents->Clone(&copyLocal->m_contents);

	 if(m_collectionQuestDB)
		m_collectionQuestDB->Clone(&copyLocal->m_collectionQuestDB);

	 *clone = copyLocal;
}

CServerItemObj * CServerItemList::GetItemInRadius_LOSchannel(D3DVECTOR entPos,float radius, int worldChannel)
{
    CAxList * obj = GetAllOnIndex(worldChannel,"channel");

	Lock();
    for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	{
        CServerItemObj * spnPtr = (CServerItemObj *)obj->GetNext_unsafe(&pos);
        if(worldChannel == spnPtr->m_world)
        if(GL_matrixFunctions.BoundingSphereCheck(radius,spnPtr->m_interactRadius,
			                                      entPos.x,entPos.y,entPos.z,
											      spnPtr->m_position.x,
											      spnPtr->m_position.y,
											      spnPtr->m_position.z))
		{
           return spnPtr;//beware it returns locked, must unlock
		}
	}
    Unlock();
	return NULL;
}

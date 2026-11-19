#include "stdafx.h"
#include "direct.h"
#include "CMenuEntity.h"
#include "GlobalStatusVariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern GlobalStatusVariables *globalStatus;

CMenuEntity::CMenuEntity()
{
	m_posZ			 = 0.0f;
	m_scale			 = 1.0f;
	m_entityID		 = -1;
	m_entityObj		 = NULL;
	m_animationIndex = 0;
	ZeroMemory(&m_centerTemp,sizeof(D3DVECTOR));
}

CMenuEntity::~CMenuEntity()
{
	if(m_entityObj)
	{ 
	   delete m_entityObj;
	   m_entityObj = 0;
	}
}

static BOOL ApplyGenderToMenuEntity(CMenuEntity *menuEnt, int baseModel, BOOL wantFemale, BOOL toggleIfSame)
{
	if (!menuEnt || !menuEnt->m_entityObj || !menuEnt->m_entityObj->List_skeletalObject || !menuEnt->m_entityObj->List_skeletalObject->m_spawnCfg)
		return FALSE;

	CMovementObj *dbPtr = (CMovementObj*)globalStatus->m_entObjList->GetByID(baseModel);
	if (!dbPtr || !dbPtr->List_skeletalObject || !dbPtr->List_skeletalObject->m_alternateConfigurations)
		return FALSE;

	int headSlot = (baseModel == 8) ? 5 : ((baseModel == 18) ? 9 : -1);
	int slotCount = menuEnt->m_entityObj->List_skeletalObject->m_deformableMeshArrayCount;

	// If toggling, check whether slot 0 is currently on a _Fem configuration
	if (toggleIfSame)
	{
		BOOL currentlyFemale = FALSE;
		POSITION pos0 = dbPtr->List_skeletalObject->m_alternateConfigurations->FindIndex(0);
		if (pos0)
		{
			CAlterUnitDBObject *alt0 = (CAlterUnitDBObject*)dbPtr->List_skeletalObject->m_alternateConfigurations->GetAt(pos0);
			if (alt0 && alt0->m_configurations)
			{
				POSITION cfgPos0 = alt0->m_configurations->FindIndex(menuEnt->m_entityObj->List_skeletalObject->m_spawnCfg[0]);
				if (cfgPos0)
				{
					CDeformableMesh *dm0 = (CDeformableMesh*)alt0->m_configurations->GetAt(cfgPos0);
					if (dm0 && dm0->m_dimName.Find("_Fem") != -1)
						currentlyFemale = TRUE;
				}
			}
		}
		wantFemale = !currentlyFemale;
	}

	for (int s = 0; s < slotCount; s++)
	{
		if (s == headSlot)
			continue;

		POSITION altPos = dbPtr->List_skeletalObject->m_alternateConfigurations->FindIndex(s);
		if (!altPos)
			continue;
		CAlterUnitDBObject *altUnit = (CAlterUnitDBObject*)dbPtr->List_skeletalObject->m_alternateConfigurations->GetAt(altPos);
		if (!altUnit || !altUnit->m_configurations)
			continue;

		CDWordArray malePublicIndices;
		CDWordArray femPublicIndices;
		int cfgIdx = 0;
		for (POSITION cfgPos = altUnit->m_configurations->GetHeadPosition(); cfgPos != NULL; ++cfgIdx)
		{
			CDeformableMesh *dm = (CDeformableMesh*)altUnit->m_configurations->GetNext(cfgPos);
			if (!dm)
				continue;
			if (dm->m_publicDim && dm->m_dimName.Find("_Fem") != -1)
				femPublicIndices.Add((DWORD)cfgIdx);
			else if ((cfgIdx == 0 || dm->m_publicDim) && dm->m_dimName.Find("_Fem") == -1)
				malePublicIndices.Add((DWORD)cfgIdx);
		}

		if (femPublicIndices.GetSize() == 0 || malePublicIndices.GetSize() == 0)
			continue;

		int curCfg = menuEnt->m_entityObj->List_skeletalObject->m_spawnCfg[s];
		INT_PTR skinOrdinal = 0;
		for (INT_PTR i = 0; i < malePublicIndices.GetSize(); i++)
		{
			if ((int)malePublicIndices.GetAt(i) == curCfg)
			{
				skinOrdinal = i;
				break;
			}
		}
		for (INT_PTR i = 0; i < femPublicIndices.GetSize(); i++)
		{
			if ((int)femPublicIndices.GetAt(i) == curCfg)
			{
				skinOrdinal = i;
				break;
			}
		}

		int targetCfg = wantFemale
			? (int)femPublicIndices.GetAt(skinOrdinal % femPublicIndices.GetSize())
			: (int)malePublicIndices.GetAt(skinOrdinal % malePublicIndices.GetSize());

		menuEnt->m_entityObj->List_skeletalObject->m_spawnCfg[s] = targetCfg;
		dbPtr->List_skeletalObject->HotSwapDim(
			s, targetCfg, menuEnt->m_entityObj->List_skeletalObject, globalStatus->m_3dDevice, TRUE);
	}

	menuEnt->UpdateAnimation();
	return TRUE;
}

BOOL CMenuEntity::InitFromDBID(int index)
{
	// Support direct female race IDs (58 = Bloblic Female, 63 = Amphibious Female) without duplicating full skeletons in RAM
	BOOL forceFemaleOnLoad = FALSE;
	if (index == 58)
	{
		index = 8;
		forceFemaleOnLoad = TRUE;
	}
	else if (index == 63)
	{
		index = 18;
		forceFemaleOnLoad = TRUE;
	}

	// Clicking the already-selected Bloblic (8) or Amphibious (18) button (or gender 1=Male / 2=Female) toggles Male <-> Female body configs
	if (m_entityObj && m_entityObj->List_skeletalObject && m_entityObj->List_skeletalObject->m_spawnCfg)
	{
		int curModel = m_entityObj->List_refModel;
		if (index == 1 && curModel == 51)
		{
			index = 39; // Switch Human Female -> Human Male
		}
		else if (index == 2 && curModel == 39)
		{
			index = 51; // Switch Human Male -> Human Female
		}
		else if ((index == 1 || index == 2) && (curModel == 8 || curModel == 18))
		{
			return ApplyGenderToMenuEntity(this, curModel, (index == 2), FALSE);
		}
		else if (index == curModel && (curModel == 8 || curModel == 18) && !forceFemaleOnLoad)
		{
			return ApplyGenderToMenuEntity(this, curModel, TRUE, TRUE);
		}
	}

    if(m_entityObj)
	{
       delete m_entityObj;
	   m_entityObj = 0;
	}

	CMovementObj * mPtr = (CMovementObj *)globalStatus->m_entObjList->GetByID(index);
    if(mPtr)
	{
	   mPtr->Clone(&m_entityObj,globalStatus->m_3dDevice,NULL);
	   m_entityObj->List_refModel = index;
	   m_entityID = index;
	   if (forceFemaleOnLoad && (index == 8 || index == 18))
	   {
		   ApplyGenderToMenuEntity(this, index, TRUE, FALSE);
	   }
       return TRUE;
	}
    return FALSE;
}

BOOL CMenuEntity::SetAnimationIndex(int animIndex)
{
     if(!m_entityObj)
		return FALSE;

	 m_entityObj->List_skeletalObject->SetCurrentAnimation(animIndex,100);

	 return TRUE;
}

BOOL CMenuEntity::IndexDimUp(int dimIndex)
{
     if(!m_entityObj)
		return FALSE;

	 
	 if(!m_entityObj->List_skeletalObject->m_spawnCfg)
	    return FALSE;

	 CMovementObj * dbPtr = globalStatus->m_entObjList->GetByID(m_entityObj->List_refModel);
	 if(!dbPtr)
	    return FALSE;

     m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex]++;
	 
	 
	 
	 
	 if(!dbPtr->List_skeletalObject->HotSwapDim( dimIndex,
					                             m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex],
					                             m_entityObj->List_skeletalObject,
					                             globalStatus->m_3dDevice,
										         FALSE))
	 {
         m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex] = 0;//loop back to 0
         if(!dbPtr->List_skeletalObject->HotSwapDim( dimIndex,
					                                 m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex],
					                                 m_entityObj->List_skeletalObject,
					                                 globalStatus->m_3dDevice,
										             FALSE))
		 {
             return FALSE;
		 }
	 }
	 UpdateAnimation();

	 return TRUE;
}

BOOL CMenuEntity::IndexDimDown(int dimIndex)
{
     if(!m_entityObj)
		return FALSE;
	 
	 if(!m_entityObj->List_skeletalObject->m_spawnCfg)
	    return FALSE;

	 CMovementObj * dbPtr = globalStatus->m_entObjList->GetByID(m_entityObj->List_refModel);
	 if(!dbPtr)
	    return FALSE;

     m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex]--;
	 if(m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex] < 0)
        m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex] = dbPtr->List_skeletalObject->GetDimCount(dimIndex);
	 

	 if(!dbPtr->List_skeletalObject->HotSwapDim( dimIndex,
					                             m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex],
					                             m_entityObj->List_skeletalObject,
					                             globalStatus->m_3dDevice,
										         FALSE))
	                                             return FALSE;

	 UpdateAnimation();

	 return TRUE;
}

BOOL CMenuEntity::UpdateAnimation()
{
    if(!m_entityObj)
		return FALSE;

	DWORD currentTime = GetTickCount();
	
	m_entityObj->List_skeletalObject->AnimationCallback( currentTime,
														 globalStatus->m_soundCenterMatrix,
														 m_centerTemp,
														 globalStatus->GL_globalSoundFactor,
														 FALSE,
														 globalStatus->m_soundManager,
														 FALSE,
														 globalStatus->m_multiplayerObject->m_AIServer,
														 globalStatus->m_3dDevice,
														 globalStatus->m_d3dVectorInit,
														 globalStatus->m_oneVal,
														 globalStatus->m_lodOverride);
	return TRUE;
}

void CMenuEntity::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	if(!pClone)
		return;

	*pClone = new CMenuEntity();
	Copy(*pClone, pD3DDevice);
}

void CMenuEntity::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	if(!pCopy)
		return;

	CMenuVisual::Copy(pCopy, pD3DDevice);

	CMenuEntity *entityCopy = dynamic_cast<CMenuEntity*>(pCopy);
	if(!entityCopy)
		return;

	// The displayed model is runtime-owned state, not menu data.  A cloned
	// character-creation window initializes its own model from its load script.
	if(entityCopy->m_entityObj)
	{
		delete entityCopy->m_entityObj;
		entityCopy->m_entityObj = NULL;
	}
	entityCopy->m_entityID = m_entityID;
	entityCopy->m_animationIndex = m_animationIndex;
	entityCopy->m_posZ = m_posZ;
	entityCopy->m_scale = m_scale;
	entityCopy->m_centerTemp = m_centerTemp;
}

void CMenuEntity::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		//Settings
		xml.WriteAttribute("Version", 1);
		xml.WriteAttribute("EntityID", m_entityID);
		xml.WriteAttribute("AnimationIndex", m_animationIndex);
		xml.WriteAttribute("PositionZ", (int)(m_posZ * 100.0f));
		xml.WriteAttribute("Scale", (int)(m_scale * 100.0f));
	}
	else
	{
		bool closing;
		char buffer[MAX_BUFFER];
		int  version;

		xml.ReadAttribute(version);
		xml.ReadAttribute(m_entityID);
		xml.ReadAttribute(m_animationIndex);
		xml.ReadAttribute(m_posZ);  m_posZ  /= 100.0f;
		xml.ReadAttribute(m_scale); m_scale /= 100.0f;

		xml.ReadTag(buffer, MAX_BUFFER, closing); //Read </Entity>
	}
}

//------------------------------------------------------------------//
BOOL CMenuEntity::IndexDimUp_PublicOnly(int dimIndex)
{
    if(!m_entityObj)
		return FALSE;
	 
	 if(!m_entityObj->List_skeletalObject->m_spawnCfg)
	    return FALSE;

	 CMovementObj * dbPtr = globalStatus->m_entObjList->GetByID(m_entityObj->List_refModel);
	 if(!dbPtr)
	    return FALSE;
 

    if(m_entityObj->List_skeletalObject->m_deformableMeshArrayCount <= dimIndex)
	   dimIndex = 0;

    if(!dbPtr->List_skeletalObject->m_alternateConfigurations)
		  return FALSE;
     
	BOOL looped = FALSE;
	int cfg = m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex]+1;

	POSITION alPos = dbPtr->List_skeletalObject->m_alternateConfigurations->FindIndex(dimIndex);
	if(!alPos)
		return FALSE;

	CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) dbPtr->List_skeletalObject->m_alternateConfigurations->GetAt(alPos);
	
	while(1)
	{
		POSITION curCfgPos = altPtr->m_configurations->FindIndex(cfg);
		if(!curCfgPos)
		{
			if(looped)
			   return FALSE;

			looped = TRUE;
            cfg = 0;
			continue;
		}

		CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
        if(dMesh->m_publicDim)
            break;
		cfg++;
	}
	if(!dbPtr->List_skeletalObject->HotSwapDim(dimIndex,cfg,m_entityObj->List_skeletalObject,globalStatus->m_3dDevice,TRUE))
		return FALSE;

	UpdateAnimation();
      
	return TRUE;
}
//------------------------------------------------------------------//
BOOL CMenuEntity::IndexDimDown_PublicOnly(int dimIndex)
{

	if(!m_entityObj)
	return FALSE;

	if(!m_entityObj->List_skeletalObject->m_spawnCfg)
	return FALSE;

	CMovementObj * dbPtr = globalStatus->m_entObjList->GetByID(m_entityObj->List_refModel);
	if(!dbPtr)
	return FALSE;


	if(m_entityObj->List_skeletalObject->m_deformableMeshArrayCount <= dimIndex)
	   return FALSE;

    BOOL looped = FALSE;
	int cfg = m_entityObj->List_skeletalObject->m_spawnCfg[dimIndex]-1;
	if(!dbPtr->List_skeletalObject->m_alternateConfigurations)
		return FALSE;

	POSITION alPos = dbPtr->List_skeletalObject->m_alternateConfigurations->FindIndex(dimIndex);
	if(!alPos)
		return FALSE;
	CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) dbPtr->List_skeletalObject->m_alternateConfigurations->GetAt(alPos);


	while(1)
	{
		  if(cfg < 0)
		  {
            if(looped)
			   return FALSE;

			looped = TRUE;
            cfg = altPtr->m_configurations->GetCount()-1;
			continue;
		  }
		  
		  POSITION curCfgPos = altPtr->m_configurations->FindIndex(cfg);
		  if(!curCfgPos)
				return FALSE;
		  CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
		  if(dMesh->m_publicDim)
				break;
		  cfg--;
    }

	if(cfg < 0)
	    return FALSE;

	if(!dbPtr->List_skeletalObject->HotSwapDim(dimIndex,cfg,m_entityObj->List_skeletalObject,globalStatus->m_3dDevice,TRUE))
		return FALSE;

	UpdateAnimation();
	      
	return TRUE;
}

#include "StdAfx.h"
#include "ItemManager.h"
#include "CStructuresMisl.h"

static CString CItemManagerGetAbsolutePath(const char *fileName)
{
	if(!fileName || !fileName[0])
		return CString();
	char fullPath[MAX_PATH] = { 0 };
	DWORD length = GetFullPathNameA(fileName, MAX_PATH, fullPath, NULL);
	if(length == 0 || length >= MAX_PATH)
		return CString(fileName);
	return CString(fullPath);
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CItemManager::CItemManager(void)
{
		//note item type is the unique
		m_count			= 0;
		m_visuals		= NULL;
		Init(3000,2);//determines memory commit and speed of query
		AddIndexVariable(0,"global_id");//unique ID m_globalID of item class serves as a unique
		AddIndexVariable(1,"name");//unique as well
}
CItemManager::~CItemManager(void)
{
		//traverse list manually to delete core class objects, the list will clear itself when deconstructed
		Lock();
		this->WriteStatus("cleaned_CItemManager.txt");
		if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
		 for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
		 {
			 for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
			 {
				 CItemObj * delPtr = (CItemObj *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
				 delPtr->SafeDelete();
				 delete delPtr;
				 delPtr = 0;
			 }
		 }
		//internally the rest of the lists should auto clean themselfs
		Unlock();

        if(m_visuals)
		{
			m_visuals->SafeDelete();
			delete m_visuals;
			m_visuals = 0;
		}
}

bool CItemManager::Add_safe(CItemObj * itemMem)
{
     if(!itemMem)//assert on add index-return false here
         return false;

	 Lock();
     if(AddIndexed((void*)itemMem, itemMem->m_globalID, itemMem->m_globalID,"global_id"))
		if(AddIndexed((void*)itemMem, GetSimpleHash(itemMem->m_itemName.GetBuffer()), itemMem->m_globalID,"name"))
		{
			m_count++;
		    Unlock();
			return true;
		}
	 
     Unlock();
	 return false;
}

int CItemManager::ImportAllDynamicItems(	CElementObjList * elementDB,
											CStatObjList * statDB,
											CDBConnectionSystem * dbConnection)
{
		int dynamicItemsAdded = 0;
		CString query = "SELECT GLID FROM GM_DYNAMICITEMS ORDER BY GLID";
		_RecordsetPtr dynItemReader = dbConnection->Query(query);
		int acCount = dbConnection->GetCount(dynItemReader);
		for(int i=0;i<acCount;i++)
		{
			CString glidText = dbConnection->GetStringData("GLID",dynItemReader);

			AquireByGlid(	glidText,
							elementDB,
							statDB,
							dbConnection);

			dbConnection->NextRecord(dynItemReader);
			dynamicItemsAdded++;
		}
		dbConnection->Close(dynItemReader);
		return dynamicItemsAdded;
}

bool CItemManager::GenerateDynItemPacket(byte ** data,int & dataSize, int glid, int optNetID)
{
	CItemObj * oPtr = GetItemByID_LOS(glid);
	if(!oPtr)
		return false;

	int visCount = oPtr->m_attachmentRefList->GetCount();
	int packetSize = sizeof(DYNITEMCREATE)+(sizeof(ITEMVISUAL)*visCount);
	dataSize = packetSize;

	byte * packetGen = new byte[packetSize];
	ZeroMemory(packetGen,packetSize);
	//build header
	DYNITEMCREATE header;
	ZeroMemory(&header,sizeof(DYNITEMCREATE));

	

	int nameSize = oPtr->m_itemName.GetLength();
	if(nameSize >= 23)
		nameSize = 23;//watch overrun

	CopyMemory(header.m_itemName,oPtr->m_itemName.GetBuffer(),nameSize);
	
	header.m_catagorie				= 14;//routes
	header.m_glid					= oPtr->m_globalID;
	header.m_animationAccess		= oPtr->m_animAccessValue;
	header.m_defaultAnimation		= oPtr->m_animAccessDefault;
	header.m_maxCharges				= oPtr->m_maxCharges;
	header.m_chargeType				= oPtr->m_chargeType;
	header.m_useType				= oPtr->m_useType;
	header.m_shotsPerFire			= oPtr->m_shotsPerFire;
	header.m_locationID				= oPtr->m_locationID;
	header.m_optNetID				= optNetID;
	header.m_attachmentCount		= visCount;
	header.m_visualRef				= oPtr->m_visualRef;

	header.m_damageElementOvrd		= oPtr->m_dynDamageElementOvrd;
	header.m_fireQueOvrd			= oPtr->m_dynDamageFireQueOvrd;
	header.m_damageOvrd				= oPtr->m_dynDamageOvrd;

	//tack on item visuals
	CopyMemory(packetGen,&header,sizeof(DYNITEMCREATE));

	ITEMVISUAL itemVis;
	int trace = sizeof(DYNITEMCREATE);
	for(POSITION posLoc = oPtr->m_attachmentRefList->GetHeadPosition();posLoc != NULL;)
	{
		CAttachmentRefObj *ref = (CAttachmentRefObj *)oPtr->m_attachmentRefList->GetNext(posLoc);

		itemVis.m_fireQueOvrd		= ref->m_fireQueOvrd;
		itemVis.m_visualID			= ref->m_attachmentType;
		itemVis.m_raceBind			= ref->m_raceBind;
		itemVis.m_dimIndex			= ref->m_dimIndex;
		itemVis.m_dimSwitchTo		= ref->m_dimSwitchTo;

		CopyMemory(&packetGen[trace],&itemVis,sizeof(ITEMVISUAL));

		trace+=sizeof(ITEMVISUAL);
	}

	*data = packetGen;
	Unlock();

	return true;
}

bool CItemManager::AddItemByDataPacket(byte * data)
{
	int headerSize = sizeof(DYNITEMCREATE);
	DYNITEMCREATE * header = (DYNITEMCREATE *)data;

	if(DoesItemExist((int)header->m_glid))
		return false;//dont dyn create twice


	CItemObj * oPtr = new CItemObj();
	oPtr->m_attachmentRefList = new CAttachmentRefList();


	oPtr->m_globalID				= header->m_glid;
	oPtr->m_itemName				= header->m_itemName;
	oPtr->m_animAccessValue			= header->m_animationAccess;
	oPtr->m_animAccessDefault		= header->m_defaultAnimation;
	oPtr->m_maxCharges				= header->m_maxCharges;
	oPtr->m_chargeType				= header->m_chargeType;
	oPtr->m_useType					= header->m_useType;
	oPtr->m_shotsPerFire			= header->m_shotsPerFire;
	oPtr->m_locationID				= header->m_locationID;
	oPtr->m_visualRef				= header->m_visualRef;

	oPtr->m_dynDamageElementOvrd	= header->m_damageElementOvrd;
	oPtr->m_dynDamageFireQueOvrd	= header->m_fireQueOvrd;
	oPtr->m_dynDamageOvrd			= header->m_damageOvrd;
	
	int itemSize = sizeof(ITEMVISUAL);

	int trace = headerSize;
	for(int i=0;i<(int)header->m_attachmentCount;i++)
	{
		ITEMVISUAL visual;
		CopyMemory(&visual,&data[trace],sizeof(ITEMVISUAL));

		CAttachmentRefObj * attObj	= new CAttachmentRefObj();
		attObj->m_attachmentType	= visual.m_visualID;
		attObj->m_fireQueOvrd		= oPtr->m_dynDamageFireQueOvrd;
		attObj->m_raceBind			= visual.m_raceBind;
		attObj->m_dimIndex			= visual.m_dimIndex;
		attObj->m_dimSwitchTo		= visual.m_dimSwitchTo;
		oPtr->m_attachmentRefList->AddTail(attObj);

		trace += sizeof(ITEMVISUAL);
	}
	Add_safe(oPtr);

	return true;
}

bool CItemManager::AquireByGlid(CString glid,
								CElementObjList * elementDB,
								CStatObjList * statDB,
								CDBConnectionSystem * dbConnection)
{
		CString query = "SELECT * FROM GM_DYNAMICITEMS WHERE GLID = '"+glid+"'";

		

		_RecordsetPtr dynItemReader = dbConnection->Query(query);
		int acCount = dbConnection->GetCount(dynItemReader);
		if(acCount > 0)
		{//item exists in DB
			CString itemNameTest = dbConnection->GetStringData("ItemName",dynItemReader);
			CItemObj * oPtr = new CItemObj();
			oPtr->m_attachmentRefList = new CAttachmentRefList();

			oPtr->m_globalID					= atoi(glid);
			oPtr->m_itemName					= dbConnection->GetStringData("ItemName",dynItemReader);
			oPtr->m_marketCost					= dbConnection->GetIntData("MarketCost",dynItemReader);
			oPtr->m_locationID					= dbConnection->GetIntData("LocationID",dynItemReader);
			oPtr->m_animAccessValue				= dbConnection->GetIntData("AnimationPref",dynItemReader);
			oPtr->m_animAccessDefault			= dbConnection->GetIntData("AnimationDefault",dynItemReader);
			oPtr->m_nonDrop						= dbConnection->GetIntData("NonLootable",dynItemReader);
			oPtr->m_nonTransferable				= dbConnection->GetIntData("NonTransferable",dynItemReader);
			oPtr->m_destroyOnUse				= dbConnection->GetIntData("DestroyOnUse",dynItemReader);
			oPtr->m_stackable					= dbConnection->GetIntData("Stackable",dynItemReader);
			oPtr->m_chargeCurrent				= dbConnection->GetIntData("CurrentCharges",dynItemReader);
			oPtr->m_maxCharges					= dbConnection->GetIntData("MaxCharges",dynItemReader);
			oPtr->m_chargeType					= dbConnection->GetIntData("ChargeType",dynItemReader);
			oPtr->m_shotsPerFire				= dbConnection->GetIntData("ShotsPerFire",dynItemReader);
			oPtr->m_ammoRegenRate				= dbConnection->GetIntData("AmmoRegenRate",dynItemReader);
			oPtr->m_useType						= dbConnection->GetIntData("UseType",dynItemReader);
			oPtr->m_requiredSkillType			= dbConnection->GetIntData("requiredSkill",dynItemReader);
			oPtr->m_requiredSkillLevel			= dbConnection->GetIntData("RequiredSkillLevel",dynItemReader);
			oPtr->m_hp							= dbConnection->GetIntData("HitPoints",dynItemReader);
			//oPtr->m_maxHP						= dbConnection->GetIntData("MaxHitPoints",dynItemReader);
			oPtr->m_destroyOnBreak				= dbConnection->GetIntData("DestroyOnBreak",dynItemReader);
			oPtr->m_requiredDex					= dbConnection->GetIntData("RequiredDex",dynItemReader);
			oPtr->m_requiredStr					= dbConnection->GetIntData("RequiredStr",dynItemReader);
			oPtr->m_requiredKnowledge			= dbConnection->GetIntData("RequiredWiz",dynItemReader);
			oPtr->m_visualRef					= dbConnection->GetIntData("VisualRef",dynItemReader);

			oPtr->m_dynDamageFireQueOvrd		= dbConnection->GetIntData("FireQueIndexOvrd",dynItemReader);
			oPtr->m_dynDamageOvrd				= dbConnection->GetIntData("DamageOvrd",dynItemReader);
			oPtr->m_dynDamageElementOvrd		= dbConnection->GetIntData("DamageElementOvrd",dynItemReader)-1;
			
			CString attachmentParse				= dbConnection->GetStringData("AttachmentParse",dynItemReader);
			CString defenseParse				= dbConnection->GetStringData("DefenseParse",dynItemReader);
			CString statBonusParse				= dbConnection->GetStringData("StatBonusParse",dynItemReader);

			//dbConnection->m_eventLogRef->LogByPriority(oPtr->m_itemName + " Dynamic Loaded",2);
			//dbConnection->m_eventLogRef->LogByPriority(oPtr->m_itemName + " HitPoints",(DWORD)oPtr->m_hp,2);
			//dbConnection->m_eventLogRef->LogByPriority(oPtr->m_itemName + " Destroy On Break:",(DWORD)oPtr->m_destroyOnBreak,2);
			//dbConnection->m_eventLogRef->LogByPriority(oPtr->m_itemName + " Fire Que Ovrd:",(DWORD)oPtr->m_dynDamageFireQueOvrd,2);
			dbConnection->m_eventLogRef->LogByPriority(oPtr->m_itemName + " Damage Ovrd:",(DWORD)oPtr->m_dynDamageOvrd,2);
			//dbConnection->m_eventLogRef->LogByPriority(oPtr->m_itemName + " Damage Element Ovrd:",(DWORD)oPtr->m_dynDamageElementOvrd,2);
			
			
			//attachment parse
			char * attachBuffer		= attachmentParse.GetBuffer();
			int bufLength			= attachmentParse.GetLength();
			CString nameBuild,fireQueOvrd,indexBuild,raceSpecific,dimIndex,dimSwitchTo;
			int buffSwitch = 0;
			//dbConnection->m_eventLogRef->LogByPriority(attachmentParse + " Attachment Parsing",2);
			for(int iP=0;iP<bufLength;iP++)
			{
				if(attachBuffer[iP] == ';')
				{
					CArmedInventoryObj * aPtr = m_visuals->GetByName(nameBuild);
					if(aPtr)
					{
						CAttachmentRefObj * attObj = new CAttachmentRefObj();
						attObj->m_attachmentType = aPtr->m_type;
						//fire que ovrd
						/*dbConnection->m_eventLogRef->LogByPriority("Attachment item add",2);
						dbConnection->m_eventLogRef->LogByPriority("Visual Name "+nameBuild,2);
						dbConnection->m_eventLogRef->LogByPriority("Race "+raceSpecific,2);
						dbConnection->m_eventLogRef->LogByPriority("Dim Index "+dimIndex,2);
						dbConnection->m_eventLogRef->LogByPriority("Dim Switch "+dimSwitchTo,2);*/


						attObj->m_raceBind		= atoi(raceSpecific);
						attObj->m_dimIndex		= atoi(dimIndex);
						attObj->m_dimSwitchTo	= atoi(dimSwitchTo);
						oPtr->m_attachmentRefList->AddTail(attObj);
					}
					else
						dbConnection->m_eventLogRef->LogByPriority("Dyn Apx Error-Visual Does not Exist:"+nameBuild,2);
				
					fireQueOvrd			= "";
					nameBuild			= "";
					raceSpecific		= "";
					dimIndex			= "";
					dimSwitchTo			= "";
					buffSwitch			= 0;
				}
				else if(attachBuffer[iP] == '|')
				{
					buffSwitch++;
				}
				else if(buffSwitch == 4)
				{
					dimSwitchTo += attachBuffer[iP];
				}
				else if(buffSwitch == 3)
				{
					dimIndex += attachBuffer[iP];
				}
				else if(buffSwitch == 2)
				{
					raceSpecific += attachBuffer[iP];
				}
				else if(buffSwitch == 1)
				{
					fireQueOvrd += attachBuffer[iP];
				}
				else if(buffSwitch == 0)
				{
					nameBuild += attachBuffer[iP];
				}
			}
			//defense parse
			char * defenseBuffer		= defenseParse.GetBuffer();
			bufLength					= defenseParse.GetLength();
			indexBuild					= "";
			nameBuild					= "";
			for(int iP=0;iP<bufLength;iP++)
			{
				if(attachBuffer[iP] == ';')
				{
					
					int index = elementDB->GetIndexByName(nameBuild);
					if(index > -1)
					{//valid
						CElementStatusObj * elePtr		= new CElementStatusObj();
						elePtr->m_currentValue			= atoi(indexBuild);
						elePtr->m_elementIndex			= index;

						oPtr->m_elementDefenses->AddTail(elePtr);
					}//end valid
				
					nameBuild			= "";
					indexBuild			= "";
					buffSwitch			= 0;
				}
				else if(attachBuffer[iP] == '|')
				{
					buffSwitch = 1;
				}
				else if(buffSwitch == 1)
				{
					indexBuild += attachBuffer[iP];
				}
				else if(buffSwitch == 0)
				{
					nameBuild += attachBuffer[iP];
				}
			}
			//stat parse
			char * statBuffer			= statBonusParse.GetBuffer();
			bufLength					= statBonusParse.GetLength();
			indexBuild					= "";
			nameBuild					= "";
			for(int iP=0;iP<bufLength;iP++)
			{
				if(attachBuffer[iP] == ';')
				{
					int index = -1;
					
					if(statDB)
						index = statDB->GetStatIdByName(nameBuild);
					if(index > -1)
					{//valid
						CStatBonusObj * statPtr		= new CStatBonusObj();
						statPtr->m_statBonus		= atoi(indexBuild);
						statPtr->m_statID			= index;

						oPtr->m_statBonuses->AddTail(statPtr);
					}//end valid
				
					nameBuild			= "";
					indexBuild			= "";
					buffSwitch			= 0;
				}
				else if(attachBuffer[iP] == '|')
				{
					buffSwitch = 1;
				}
				else if(buffSwitch == 1)
				{
					indexBuild += attachBuffer[iP];
				}
				else if(buffSwitch == 0)
				{
					nameBuild += attachBuffer[iP];
				}
			}

			//if already exists update it
			Lock();
			CItemObj * existingPtr = GetItemByName(oPtr->m_itemName.GetBuffer());//see if we need to modify things
			if(existingPtr)
			{//if exists remove
				DeleteByOject(existingPtr);
			}//end if exists remove
			Unlock();

			//add it to item DB
			Add_safe(oPtr);
		
		}
		dbConnection->Close(dynItemReader);

	 return true;
}


int CItemManager::GetFreeGlobalID_safe()
{
    int newID = 1;
	while(Exists_safe(newID))
	{
       newID++;
	}
	return newID;
}

BOOL CItemManager::Exists_safe(int globalID)
{ 
     CAxList * objList = GetAllOnIndex(globalID,"global_id");
	 Lock();
     for(CAxLinkObj * pos = objList->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CItemObj * iObj = (CItemObj *)objList->GetNext_unsafe(&pos);
         if(iObj->m_globalID==globalID)
		 {
		    Unlock();
		    return TRUE;
		 }
	 }
	 Unlock();
	 return FALSE;
}


BOOL CItemManager::DeleteByOject(CItemObj * itemMem)
{
     if(!itemMem)
		 return FALSE;//should assert when used

	 BOOL res = FALSE;
	 if(RemoveIndexed(itemMem->m_globalID,itemMem->m_globalID, "global_id"))
	    res = TRUE;
     if(RemoveIndexed(GetSimpleHash(itemMem->m_itemName.GetBuffer()),itemMem->m_globalID, "name"))
	    res = TRUE;
	 m_count--;
	 itemMem->SafeDelete();
	 delete itemMem;
	 itemMem = 0;

	 return res;
}

BOOL CItemManager::RemoveByData_safe(int globalID, char * name)
{
	 BOOL res = FALSE;
	 Lock();
	 if(RemoveIndexed(globalID,globalID, "global_id"))
	    res = TRUE;
     if(RemoveIndexed(GetSimpleHash(name),globalID, "name"))
	    res = TRUE;
	 m_count--;
	 Unlock();

	 return res;
}

CItemObj * CItemManager::GetItemByID_LOS(int globalID)
{
	 Lock();
	 CAxLinkObj * obj = GetByUniqueID(globalID,globalID,"global_id");
	 if(obj)
		return (CItemObj *)obj->m_thisObject;//returns locked
	 Unlock();
	 return NULL;
}

BOOL CItemManager::GetItemDynOvrds_safe(int & globalID,int * damage,int * element,int * fireQue)
{
	 Lock();
	 CAxLinkObj * obj = GetByUniqueID(globalID,globalID,"global_id");
	 if(obj)
	 {
		CItemObj * item = (CItemObj *)obj->m_thisObject;
		if(damage)
			*damage	= item->m_dynDamageOvrd;
		if(element)
			*element = item->m_dynDamageElementOvrd;
		if(fireQue)
			*fireQue = item->m_dynDamageFireQueOvrd;

		Unlock();
		return TRUE;
	 }
	 Unlock();
	 return FALSE;
}

BOOL CItemManager::DoesItemExist(int globalID)
{
	 Lock();
	 CAxLinkObj * obj = GetByUniqueID(globalID,globalID,"global_id");
	 if(obj)
	 {
			Unlock();
			return TRUE;//returns locked
	 }
	 Unlock();
	 return FALSE;
}

CItemObj * CItemManager::GetItemByID(int globalID)
{
	 CAxList * objList = GetAllOnIndex(globalID,"global_id");
     for(CAxLinkObj * pos = objList->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CItemObj * iObj = (CItemObj *)objList->GetNext_unsafe(&pos);
         if(iObj->m_globalID==globalID)
		    return iObj;
	 }
	 return NULL;
}

CItemObj * CItemManager::GetItemByName_LOS(char * name)
{
	 CAxList * objList = GetAllOnIndex(GetSimpleHash(name),"name");
	 Lock();
     for(CAxLinkObj * pos = objList->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CItemObj * iObj = (CItemObj *)objList->GetNext_unsafe(&pos);
         if(iObj->m_itemName.CompareNoCase(name)==0)
		    return iObj;
	 }
	 Unlock();
	 return NULL;
}

CItemObj * CItemManager::GetItemByName(char * name)
{
	 CAxList * objList = GetAllOnIndex(GetSimpleHash(name),"name");
     for(CAxLinkObj * pos = objList->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CItemObj * iObj = (CItemObj *)objList->GetNext_unsafe(&pos);
         if(iObj->m_itemName.CompareNoCase(name)==0)
		    return iObj;
	 }
	 return NULL;
}

int CItemManager::GetGLIDByName_safe(char * name)
{
	int result = -1;
	
	CAxList * objList = GetAllOnIndex(GetSimpleHash(name),"name");
	Lock();
    for(CAxLinkObj * pos = objList->GetHeadPosition_unsafe();pos != NULL;)
	{
		CItemObj * iObj = (CItemObj *)objList->GetNext_unsafe(&pos);
		if(iObj->m_itemName.CompareNoCase(name)==0)
		{
			result = iObj->m_globalID;
			break;
		}
	}
	Unlock();
	return result;
}

BOOL CItemManager::GetIsStackableByName_safe(char * name)
{
	int result = FALSE;
	
	CAxList * objList = GetAllOnIndex(GetSimpleHash(name),"name");
	Lock();
    for(CAxLinkObj * pos = objList->GetHeadPosition_unsafe();pos != NULL;)
	{
		CItemObj * iObj = (CItemObj *)objList->GetNext_unsafe(&pos);
		if(iObj->m_itemName.CompareNoCase(name)==0)
		{
			result = iObj->m_stackable;
			break;
		}
	}
	Unlock();
	return result;
}

BOOL CItemManager::VerifyRaceCanArm_safe(int raceBind, int globalID)
{
	 Lock();
     CAxList * objList = GetAllOnIndex(globalID,"global_id");
     for(CAxLinkObj * pos = objList->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CItemObj * iObj = (CItemObj *)objList->GetNext_unsafe(&pos);
		 if(iObj->m_globalID==globalID)
		  if(iObj->m_attachmentRefList)
		    for(POSITION atPos = iObj->m_attachmentRefList->GetHeadPosition();atPos != NULL;)
			{
			    CAttachmentRefObj * ref = (CAttachmentRefObj *)iObj->m_attachmentRefList->GetNext(atPos);
				if(ref->m_raceBind == raceBind || ref->m_raceBind == -1)
				{
                   Unlock();
				   return TRUE;
				}
			}
	 }
	 Unlock();
	 return FALSE;
}

BOOL CItemManager::Save(char * fileName)
{
     CFileException exc;
	 CFile fl;
	 BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
	 if(!res)
		 return FALSE;
	 CArchive the_out_Archive(&fl,CArchive::store);

	 the_out_Archive << m_count;
	 the_out_Archive << m_visuals;
	 if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
	 for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
	 {
		 for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		 {
			 CItemObj * itemPtr = (CItemObj *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
			 the_out_Archive << itemPtr;
		 }
	 }
	 //internally the rest of the lists should auto clean themselfs

	 the_out_Archive.Close();
	 fl.Close();
	 m_filePath = CItemManagerGetAbsolutePath(fileName);
	 return TRUE;
}

BOOL CItemManager::ImportAPD(char * fileName)
{
     CFileException exc;
	 CFile fl;
	 BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE)
		return FALSE;
	 CArchive the_in_Archive(&fl,CArchive::load);

     if(m_visuals)
	 {
        m_visuals->SafeDelete();
		delete m_visuals;
		m_visuals = 0;
	 }

	 the_in_Archive >> m_visuals;

	 m_visuals->CompileDirectAccess();
	 
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
}

BOOL CItemManager::ImportEDB(char * fileName)
{
     CFileException exc;
	 CFile fl;
	 BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE)
		return FALSE;
	 CArchive the_in_Archive(&fl,CArchive::load);

	 CMovementObjList * edbTemp  = NULL;
	 the_in_Archive >> edbTemp;
	 
     //extract items
	 if(edbTemp)
	 {
		 
		 for(POSITION posLoc = edbTemp->GetHeadPosition();posLoc != NULL;)
		 {
			 CMovementObj * mPtr = (CMovementObj *) edbTemp->GetNext(posLoc);
			 if(mPtr->m_itemDB)
			 for(POSITION iPos = mPtr->m_itemDB->GetHeadPosition();iPos != NULL;)
			 {
				CItemObj * iPtr = (CItemObj *) mPtr->m_itemDB->GetNext(iPos);
				CItemObj * clonePtr = NULL;
				iPtr->Clone(&clonePtr);
                //format to be bound to specific edb entity
                if(clonePtr->m_attachmentRefList)
				   for(POSITION atPos = clonePtr->m_attachmentRefList->GetHeadPosition();atPos != NULL;)
				   {
                       CAttachmentRefObj * ref = (CAttachmentRefObj *)clonePtr->m_attachmentRefList->GetNext(atPos);
                       ref->m_raceBind = mPtr->m_uniqueId;
				   }

				Add_safe(clonePtr);
			 }
		 }
		 edbTemp->SafeDelete();
         delete edbTemp;
	     edbTemp = 0;
	 }

	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
}

BOOL CItemManager::Load(char * fileName)
{
     CFileException exc;
	 CFile fl;
	 BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE)
		return FALSE;
	 CArchive the_in_Archive(&fl,CArchive::load);

	 if(m_visuals)
	 {
        m_visuals->SafeDelete();
		delete m_visuals;
		m_visuals = 0;
	 }

	 int countToLoad = 0;
	 the_in_Archive >> countToLoad;
	 the_in_Archive >> m_visuals;
	 for(int loop = 0;loop<countToLoad;loop++)
	 {
         CItemObj * itemPtr = NULL;
		 the_in_Archive >> itemPtr;
         Add_safe(itemPtr);//reconstructs indexes from file
	 }
	 //would load apd visuals
	 m_visuals->CompileDirectAccess();

	 the_in_Archive.Close();
     fl.Close();
	 m_filePath = CItemManagerGetAbsolutePath(fileName);
	 return TRUE;
}


BOOL CItemManager::ArmEntity(int globalID, 
							 CMovementObj * entityLive, 
							 CMovementObj * refDBEntity,
							 LPDIRECT3DDEVICE9 g_pd3dDevice)
{
		CItemObj * itemPtr = GetItemByID_LOS(globalID);
		if(!itemPtr)
			return FALSE;

		POSITION dbPosLast;
		for(POSITION dbPos = entityLive->m_itemDB->GetHeadPosition();(dbPosLast = dbPos) != NULL;)
		{
            CItemObj * itemCHK = (CItemObj *)entityLive->m_itemDB->GetNext(dbPos);
			if(itemCHK->m_locationID == itemPtr->m_locationID)
			{
               entityLive->DisarmItem(itemCHK,TRUE,refDBEntity,g_pd3dDevice);
			   itemCHK->SafeDelete();
			   delete itemCHK;
			   itemCHK = 0;
			   entityLive->m_itemDB->RemoveAt(dbPosLast);
			}
		}

        CItemObj * itemPtrClone = NULL;
		itemPtr->Clone(&itemPtrClone);
		itemPtrClone->m_equiped = TRUE;
		entityLive->m_itemDB->AddTail(itemPtrClone);
		if(itemPtrClone->m_dimCfgChangeTo > -1)
		{
           refDBEntity->List_skeletalObject->HotSwapDim(    (int)itemPtrClone->m_dimCfgIndex,
													        (int)itemPtrClone->m_dimCfgChangeTo,
															entityLive->List_skeletalObject,
															g_pd3dDevice,
															FALSE);
		}


	    //end verify manditory swap
		if(itemPtr->m_attachmentRefList)
		{
		 for(POSITION atPos = itemPtr->m_attachmentRefList->GetHeadPosition();atPos != NULL;)
		 {
		   CAttachmentRefObj * attObj = (CAttachmentRefObj *)itemPtr->m_attachmentRefList->GetNext(atPos);
		   
		   
		   if(attObj->m_raceBind == -1 || attObj->m_raceBind == entityLive->m_uniqueId)
		   {
               if(attObj->m_dimSwitchTo > -1)
			   {
				   refDBEntity->List_skeletalObject->HotSwapDim(    (int)attObj->m_dimIndex,
																	(int)attObj->m_dimSwitchTo,
																	entityLive->List_skeletalObject,
																	g_pd3dDevice,
																	FALSE);
			   }

			   //attObj->m_fireQueOvrd;
			   if(entityLive->ArmAttachment(attObj->m_attachmentType,
											attObj->m_fireQueOvrd,
											m_visuals,
											g_pd3dDevice,
											itemPtr->m_locationID,
											refDBEntity)==0)
			   {
		   
			   }
		   }
		 }
		}
		//itemPtr->m_equiped = TRUE;
		if(itemPtr->m_locationID == 12)
		if(itemPtr->m_animAccessValue != -1)
		{
		   entityLive->List_skeletalObject->m_animPreference = itemPtr->m_animAccessValue;
           entityLive->List_skeletalObject->m_animPrefDefault = itemPtr->m_animAccessDefault;
		}

		Unlock();
		return TRUE;
}

BOOL CItemManager::DisarmEntity(int globalID, 
							    CMovementObj * entityLive, 
							    CMovementObj * refDBEntity,
							    LPDIRECT3DDEVICE9 g_pd3dDevice)
{
		CItemObj * itemPtr = GetItemByID_LOS(globalID);
		if(!itemPtr)
			return FALSE;

		if(itemPtr->m_dimCfgChangeTo > -1)
		{
		   if(entityLive->List_skeletalObject->m_spawnCfg)
           refDBEntity->List_skeletalObject->HotSwapDim((int)itemPtr->m_dimCfgIndex,
					                                    entityLive->List_skeletalObject->m_spawnCfg[itemPtr->m_dimCfgIndex],
					                                    entityLive->List_skeletalObject,
					                                    g_pd3dDevice,
													    FALSE);
		}

		if(itemPtr->m_attachmentRefList){
		   for(POSITION atPos = itemPtr->m_attachmentRefList->GetHeadPosition();atPos != NULL;)
		   {
		       CAttachmentRefObj * attObj = (CAttachmentRefObj *)itemPtr->m_attachmentRefList->GetNext(atPos);
			   if(attObj->m_dimSwitchTo > -1)
			   {
				   if(entityLive->List_skeletalObject->m_spawnCfg)
				      refDBEntity->List_skeletalObject->HotSwapDim((int)attObj->m_dimIndex,
																        entityLive->List_skeletalObject->m_spawnCfg[attObj->m_dimIndex],
																        entityLive->List_skeletalObject,
																        g_pd3dDevice,
																        FALSE);
			   }
			   
			   if(entityLive->DisarmAttachment(attObj->m_attachmentType,g_pd3dDevice,refDBEntity)==0)
			   {
                  //could not find but proceed
			   }
		   }
		}
		//itemPtr->m_equiped = FALSE;
		if(itemPtr->m_animAccessValue != -1)
		{
		   entityLive->List_skeletalObject->m_animPreference = 0;
		   entityLive->List_skeletalObject->m_animPrefDefault = 0;
		}

		Unlock();
		return TRUE;
}
#include <d3d9.h>
#include <afxtempl.h>
#include "stdafx.h"
#include "CInventoryClass.h"
//#include "CStructuresMisl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CInventoryObj, CObject, 0)
IMPLEMENT_SERIAL(CInventoryObjList, CObList, 0)

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObj
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CInventoryObj::CInventoryObj()
{
	m_quanity = 0;
	m_armed = FALSE;
	m_itemData = NULL;
	m_optimizedSave = FALSE;
	m_tempUniqueSync = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObj
// Function:	SafeDelete()
// Description:	Safely release resources held by this inventory object.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObj::SafeDelete()
{
    if(m_itemData)
	{
       m_itemData->SafeDelete();
	   delete m_itemData;
	   m_itemData = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObj
// Function:	Clone(CInventoryObj**)
// Description:	Perform a deep copy of one inventory object to another.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObj::Clone(CInventoryObj **copy)
{
     CInventoryObj *copyLocal = new CInventoryObj();
	 if(m_itemData) m_itemData->Clone(&copyLocal->m_itemData);
	 copyLocal->m_quanity = m_quanity;
	 copyLocal->m_armed = m_armed;
	 *copy = copyLocal;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObj
// Function:	ApplyWear(int wearAmount)
// Description:	direct wear function
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObj::ApplyWear(int wearAmount)
{
	if(m_itemData->m_hp > 1)//has energy left
	{//item has wear rate
		m_itemData->m_hp -= wearAmount;//decrement health of item
		if( m_itemData->m_hp < 1)
			m_itemData->m_hp = 1;

		if(m_itemData->m_hp == 1)//just broke-notify (we know this will stop wear this rount on other prospects but thats ok
		{
			return TRUE;//one did break this round notify-outside this function will handle delete and notification
		}
	}//end item has wear rate
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObj
// Function:	PhysicChanging()
// Description:	Check if this inventory object changes the physics of 
//				the player character.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObj::PhysicChanging()
{
	if(m_armed && m_itemData && m_itemData->m_statBonuses)
	{
		for(POSITION posLocX = m_itemData->m_statBonuses->GetHeadPosition(); posLocX != NULL;)
		{
			CStatBonusObj *statBns = (CStatBonusObj*)m_itemData->m_statBonuses->GetNext(posLocX);
			if(statBns->m_statID >= 1000) return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObj
// Function:	CloneMinimum(CInventoryObj**)
// Description:	Copy minimum required properties from one inventory object
//				to another.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObj::CloneMinimum(CInventoryObj **copy)
{
     CInventoryObj *copyLocal = new CInventoryObj();
	 if(m_itemData)
	 {
	    copyLocal->m_itemData = new CItemObj();
        copyLocal->m_itemData->m_globalID = m_itemData->m_globalID;
        copyLocal->m_itemData->m_chargeCurrent = m_itemData->m_chargeCurrent;
		copyLocal->m_itemData->m_missileLoaded = m_itemData->m_missileLoaded;
		copyLocal->m_itemData->m_damageModifier = m_itemData->m_damageModifier;
		if(m_itemData->m_expansiveCollection)
		   m_itemData->m_expansiveCollection->Clone(&copyLocal->m_itemData->m_expansiveCollection);
	 }
	 copyLocal->m_quanity = m_quanity;
	 copyLocal->m_armed = m_armed;
	 *copy = copyLocal;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	SerializeAlloc(CArchive &)
// Description:	Read/Write this inventory object to/from storage.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObj::SerializeAlloc(CArchive &ar)
{
	if(ar.IsStoring())
	{      
		ar	<< m_itemData->m_globalID
			<< m_itemData->m_chargeCurrent
			<< m_quanity
			<< m_armed;
		     
		if(!m_itemData->m_expansiveCollection)
		{
		  ar << -1;
		}
		else
		{
		  ar << (int)m_itemData->m_expansiveCollection->GetCount();
		  m_itemData->m_expansiveCollection->SerializeAlloc(ar,0);
		}
	}
	else
	{
		m_itemData = new CItemObj();
		ar >> m_itemData->m_globalID
			>> m_itemData->m_chargeCurrent
			>> m_quanity
			>> m_armed;

		int expCount = 0;
		ar >> expCount;

		if(expCount > -1)
		{
			m_itemData->m_expansiveCollection = new CCExpansiveObjList();
			m_itemData->m_expansiveCollection->SerializeAlloc(ar,expCount);
		}
	}
}     

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObj
// Function:	Serialize(CArchive &)
// Description:	Read/Write this inventory object to/from storage.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObj::Serialize(CArchive &ar)
{
	BOOL reserved = 0;
	char version  = 0;
	CObject::Serialize( ar );

	if(ar.IsStoring())
	{
		if(!m_optimizedSave)
		{
			version = 0;
			ar	<< version
				<< m_itemData
				<< m_quanity
				<< m_armed
				<< reserved
				<< reserved;
		}
		else
		{
			version = 1;
			ar  << version
				<< m_itemData->m_globalID
				<< m_itemData->m_chargeCurrent
				<< m_quanity
				<< m_armed
				<< m_itemData->m_expansiveCollection;	// Migrate

			/* DEPRECATED /////////////////////////////////////////////////////////
			CItemObj item;
			 
			ar  << version;
			ar  << &item;

			ar	<< m_quanity
				<< m_armed
				<< m_itemData->m_globalID
				<< m_itemData->m_chargeCurrent
				<< m_itemData->m_expansiveCollection;
			/////////////////////////////////////////////////////////////////////*/
		}
		m_optimizedSave = FALSE;		// Reset
	}
	else
	{
		ar >> version;
		//version = 0;

		if(version == 0)
		{
			ar  >> m_itemData
				>> m_quanity
				>> m_armed
				>> reserved
				>> reserved;
		}
		// Expand
		else
		{
			this->m_itemData = new CItemObj();
			ar	>> m_itemData->m_globalID
				>> m_itemData->m_chargeCurrent
				>> m_quanity
				>> m_armed
				>> m_itemData->m_expansiveCollection;	// Migrate

			/* DEPRECATED /////////////////////////////////////////////////////////
			ar  >> m_itemData;

			ar  >> m_quanity
				>> m_armed
				>> m_itemData->m_globalID
				>> m_itemData->m_chargeCurrent
				>> m_itemData->m_expansiveCollection;
			/////////////////////////////////////////////////////////////////////*/
		}
		m_optimizedSave = FALSE; // Reset
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CInventoryObjList::CInventoryObjList()
{
       m_totalWieght = 0;
       m_itemCountLimit = 0;
       m_wieghtLimit = 0;
       m_itemTotal = 0;
	   m_lastArmedWeapon = -1;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	SafeDelete()
// Description:	Safely delete all inventory object nodes from this linked list.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		invObj->SafeDelete();
		
		if (invObj) delete invObj;
		invObj = 0;
	}
	RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	RemoveAllExceptPerminentNONdrop()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObjList::RemoveAllExceptPerminentNONdrop()
{
	POSITION posLast = NULL;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(	((invObj->m_itemData->m_permanent == FALSE) && 
			(invObj->m_itemData->m_nonDrop == FALSE)) ||
			(!invObj->m_itemData->m_isInQuickSlots))
		{
			invObj->SafeDelete();
			delete invObj;
			invObj = 0;
			RemoveAt(posLast);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	RemoveAllExceptPerminent()
// Description:	
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObjList::RemoveAllExceptPerminent()
{
	POSITION posLast = NULL;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(!invObj->m_itemData->m_permanent || !invObj->m_itemData->m_isInQuickSlots)
		{
			invObj->SafeDelete();
			delete invObj;
			invObj = 0;
			RemoveAt(posLast);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetByGLID(int)
// Description:	Return a pointer to an inventory object in this list 
//				based on a unique ID.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::GetByGLID(int glID)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(glID == invObj->m_itemData->m_globalID) 
			return invObj;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	IsItemOfValuePresent()
// Description:	
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::IsItemOfValuePresent()
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);		
		if(!invObj->m_itemData->m_permanent) 
			return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetInvenCountFilterUnseeable()
// Description:	
///////////////////////////////////////////////////////////////////////////////

int 
CInventoryObjList::GetInvenCountFilterUnseeable()
{
	int count = 0;
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj * invObj = (CInventoryObj *)GetNext(posLoc);
		if(invObj->m_itemData->m_permanent)
		   continue;
		/* DEPRECATED /////////////////////////////////////////////////////////
		//Also adding in that spells/skills do not add to your inventory count
		if(invObj->m_itemData->m_useType == 11 || invObj->m_itemData->m_useType == 12)
		continue;
		/////////////////////////////////////////////////////////////////////*/
        count++;
	}
	return count;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	CanTakeObjects(int, int)
// Description:	Determine if the player can possess a given number of 
//				additional items.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::CanTakeObjects(int maxCount, int itemCountToAdd)
{
	int currentCount = GetInvenCountFilterUnseeable();
	if((currentCount + itemCountToAdd) < maxCount)
		return TRUE;
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	IsExpItemObjectPresent(int, int)
// Description:	
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::IsExpItemObjectPresent(int glid, int expID)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(	(invObj->m_itemData->m_globalID == glid) &&
			(invObj->m_itemData->m_expansiveCollection) &&
			(invObj->m_itemData->m_expansiveCollection->GetByMisc2(expID)))
		{
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	IsExpItemObjectPresent(CArchive&)
// Description:	Read/Write an inventory list to/from storage.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObjList::Serialize(CArchive &ar)
{
	CObList::Serialize(ar);
	if(ar.IsStoring())
	{	
		ar  << m_totalWieght
			<< m_itemCountLimit
			<< m_wieghtLimit
			<< m_itemTotal;
	}
	else
	{
		ar  >> m_totalWieght
			>> m_itemCountLimit
			>> m_wieghtLimit
			>> m_itemTotal;
		m_lastArmedWeapon = -1;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	Clone(CInventoryObjList**)
// Description:	Perform a deep copy of all inventory items in this list.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObjList::Clone(CInventoryObjList **copy)
{
	// Create and init the clone list
	CInventoryObjList* copyLocal = new CInventoryObjList();
	copyLocal->m_totalWieght = m_totalWieght;
	copyLocal->m_itemCountLimit = m_itemCountLimit;
	copyLocal->m_wieghtLimit = m_wieghtLimit;
	copyLocal->m_itemTotal = m_itemTotal;
	copyLocal->m_lastArmedWeapon = m_lastArmedWeapon;

	// Copy each inventory node from this list to the clone
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		CInventoryObj *cpyPtr = NULL;
		invObj->Clone(&cpyPtr);
		copyLocal->AddTail(cpyPtr);
	}

	// Set the in param to point to our instantiated clone
	*copy = copyLocal;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	CloneMinimum(CInventoryObjList**)
// Description:	Perform a deep copy of all inventory items in this list.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObjList::CloneMinimum(CInventoryObjList **copy)
{
	CInventoryObjList* copyLocal = new CInventoryObjList();
	copyLocal->m_totalWieght = m_totalWieght;
	copyLocal->m_itemCountLimit = m_itemCountLimit;
	copyLocal->m_wieghtLimit = m_wieghtLimit;
	copyLocal->m_itemTotal = m_itemTotal;
	copyLocal->m_lastArmedWeapon = m_lastArmedWeapon;

	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CInventoryObj* invObj = (CInventoryObj*)GetNext(posLoc);
		CInventoryObj* cpyPtr = NULL;
		invObj->CloneMinimum(&cpyPtr);
		copyLocal->AddTail(cpyPtr);
	}
	*copy = copyLocal;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	AddInventoryItem(int, BOOL, CItemObj*)
// Description:	Attempt to add an item to our inventory. If the item was 
//				added succesfully, we return a pointer to it. Otherwise, return
//				NULL to indicate failure.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::AddInventoryItem(int quanity,
									BOOL armed,
									CItemObj *itemData)
{
	// If we are at maximum capacity, return NULL
	int cnt = (int)GetCount();
	if(cnt > 700) return NULL;

	// See if we already have a similar object in inventory
	CInventoryObj *existingSimilar = VerifyInventory(itemData->m_globalID);

	if(existingSimilar)
	{ // Begin Similar Found
		if(existingSimilar->m_itemData->m_stackable)
		{ // Begin Stackable
			existingSimilar->m_quanity += quanity;
			if(existingSimilar->m_quanity > 30000)
				existingSimilar->m_quanity = 30000;
			return existingSimilar;
		} // End Stackable
		else 
		{ // Begin Not Stackable
			CInventoryObj *invObj = NULL;
			for(int j=0;j<quanity;j++)
			{
				invObj = new CInventoryObj();
				itemData->Clone(&invObj->m_itemData);
				invObj->m_quanity = 1;
				invObj->m_armed = armed;
				AddTail(invObj);
			}
			return invObj;
		} // End Not Stackable
	} // End Similar Found
	else
	{ // Add New Item
		if(itemData->m_stackable)
		{ // Begin Stackable
			CInventoryObj *invObj = new CInventoryObj();
			itemData->Clone(&invObj->m_itemData);
			invObj->m_quanity = quanity;
			invObj->m_armed   = armed;
			if(invObj->m_quanity > 30000)
				invObj->m_quanity = 30000;
			AddTail(invObj);
			return invObj;
		} // End Stackable
		else
		{ // Not Stackable
			CInventoryObj *invObj = NULL;
			for(int j=0; j<quanity; j++)
			{
				invObj = new CInventoryObj();
				itemData->Clone(&invObj->m_itemData);
				invObj->m_quanity = 1;
				invObj->m_armed = armed;
				if(invObj->m_quanity > 30000)
				   invObj->m_quanity = 30000;
				AddTail(invObj);	
			}
			return invObj;
		} // End Not Stackable
	} // End New Item
    
	// Item added successfully
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	IsAnythingInInventoryValuable()
// Description:	Check to see if there are any items in inventory with market
//				cost that are NOT non-droppable AND NOT permanent.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::IsAnythingInInventoryValuable()
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(	((!invObj->m_itemData->m_permanent) || (!invObj->m_itemData->m_nonDrop)) 
			&& (invObj->m_itemData->m_marketCost > 0) )
		   return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetTotalValue()
// Description:	Return total value of all items in inventory that are
//				NOT non-droppable AND NOT permanent.
///////////////////////////////////////////////////////////////////////////////

int 
CInventoryObjList::GetTotalValue()
{
	int total = 0;
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(	((!invObj->m_itemData->m_permanent) || (!invObj->m_itemData->m_nonDrop)) 
			&& (invObj->m_itemData->m_marketCost > 0) )
		   total += (invObj->m_itemData->m_marketCost * invObj->m_quanity);
	}
   return total;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyInventory(int)
// Description:	Retrieve an item from inventory based on a unique ID. 
//				If found, return a pointer to the item. Otherwise, return NULL.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::VerifyInventory(int GL_ID)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(invObj->m_itemData->m_globalID == GL_ID)
		   return invObj;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	OptimizeBestConfig()
// Description:	Equip player with the most valuable item in inventory.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObjList::OptimizeBestConfig()
{
	int startID = 0;
	CInventoryObj *bestPtr = NULL;
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		bestPtr = invObj;
		bestPtr->m_armed = TRUE;
        for(POSITION posLoc2 = GetHeadPosition(); posLoc2 != NULL;)
	    {
            CInventoryObj *invObjX = (CInventoryObj*)GetNext(posLoc2);
            if(	(invObj->m_itemData->m_locationID == invObjX->m_itemData->m_locationID)
				&& (invObjX->m_itemData->m_marketCost > invObj->m_itemData->m_marketCost))
			{
                bestPtr->m_armed = FALSE;
				bestPtr = invObjX;
                bestPtr->m_armed = TRUE;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyInventoryUnarmed(int, int)
// Description:	Verify that a certain item (or group of items) in inventory
//				is  not currently armed, based on a unique ID. If not armed,
//				return a pointer to that item (or group of items).
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::VerifyInventoryUnarmed(int GL_ID, int quanity)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if( (!invObj->m_armed)
			&& (invObj->m_itemData->m_globalID == GL_ID)
			&& (invObj->m_quanity >= quanity))
		{
		   return invObj;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyInventoryUnarmed(int)
// Description:	Verify that a certain item (or group of items) in inventory
//				is  allowed to be transferable
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::VerifyInventoryTransferable(int GL_ID, int quanity)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if( (!invObj->m_armed)
			&& (invObj->m_itemData->m_globalID == GL_ID)
			&& (invObj->m_quanity >= quanity)
			&& (invObj->m_itemData->m_nonTransferable == FALSE))
		{
		   return invObj;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyInventoryUnarmed(int, int, int)
// Description:	Verify that a certain item (or group of items) in inventory
//				is not currently armed, based on a unique ID and the
//				specified damage modifier amount. If not armed, return a 
//				pointer to that item (or group of items).
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::VerifyInventoryUnarmed(int GL_ID, int quanity, int dmod)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if( (!invObj->m_armed)
			&& (invObj->m_itemData->m_globalID == GL_ID)
			&& ((int)invObj->m_itemData->m_damageModifier == dmod)
			&& (invObj->m_quanity >= quanity))
		{
		   return invObj;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyItemIsArmed(CString)
// Description:	Verify that an item is armed based on its name.
///////////////////////////////////////////////////////////////////////////////

bool 
CInventoryObjList::VerifyItemIsArmed(CString itemName)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(	(invObj->m_armed)
			&& (itemName.CompareNoCase(invObj->m_itemData->m_itemName)))
			return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyItemIsArmed(int)
// Description:	Verify that an item is armed based on a unique ID.
///////////////////////////////////////////////////////////////////////////////

bool 
CInventoryObjList::VerifyItemIsArmed(int iGLID)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(	(invObj->m_armed)
			&& (invObj->m_itemData->m_globalID == iGLID))
			return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyPlayerHasItem(CString)
// Description:	Verify that the player has an item, provided its name.
///////////////////////////////////////////////////////////////////////////////

bool 
CInventoryObjList::VerifyPlayerHasItem(CString itemName)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(itemName.CompareNoCase(invObj->m_itemData->m_itemName))
			return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyPlayerHasItem(int)
// Description:	Verify that the player has an item, provided a unique ID.
///////////////////////////////////////////////////////////////////////////////

bool 
CInventoryObjList::VerifyPlayerHasItem(int iGLID)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(invObj->m_itemData->m_globalID == iGLID)
			return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyInventory(int, int)
// Description:	Verify that a player has a minimum of some item in inventory 
//				based on a unique ID.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::VerifyInventory(int GL_ID, int quanity)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(	(invObj->m_itemData->m_globalID == GL_ID)
			&& (invObj->m_quanity >= quanity))
		{
		   return invObj;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyInventory(int, int, int)
// Description:	Verify that a player has a minimum amount of a duplicate item
//				in inventory based on a unique ID.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::VerifyInventoryDUPH(int GL_ID, int dupCount, int quanity)
{
	int dupTrace = 0;
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(invObj->m_itemData->m_globalID == GL_ID)
		{
		   if(dupCount == dupTrace)
		   {
              if(invObj->m_quanity < quanity)
			     return NULL;
		      return invObj;
		   }
		   else dupTrace++;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	VerifyInventory(int, int)
// Description:	Verify that a player has a duplicate item in inventory 
//				based on a unique ID.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::VerifyInventoryDUPH(int GL_ID, int dupCount)
{
	int dupTrace = 0;
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(invObj->m_itemData->m_globalID == GL_ID)
		{
		   if(dupCount == dupTrace)
		      return invObj;
		   else
		      dupTrace++;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DoesArmedLocationIDExist(int)
// Description:	Check to see if an item at some given location is currently armed.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DoesArmedLocationIDExist(int id)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(	(invObj->m_armed)
			&& (invObj->m_itemData->m_locationID == id))
			return TRUE;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	ApplyWearOverAll(int)
// Description:	in the even item has a wear rate, this is called in placed like 
//				attack or handle damage, only one will break max-this will optimize flow of data but create a slight unbalance
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::ApplyWearOverAll(CInventoryObj ** brokenRef,int wearAmount)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(invObj->m_armed)
			if(invObj->m_itemData->m_hp > 1)//has energy left
				{//item has wear rate
					invObj->m_itemData->m_hp -= wearAmount;//decrement health of item
					if( invObj->m_itemData->m_hp < 1)
						invObj->m_itemData->m_hp = 1;

					if(invObj->m_itemData->m_hp == 1)//just broke-notify (we know this will stop wear this rount on other prospects but thats ok
					{
						*brokenRef = invObj;
						return TRUE;//one did break this round notify-outside this function will handle delete and notification
					}
				}//end item has wear rate
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Function:	ApplyWearSpecific(int)
// Description:	in the even item has a wear rate, this is called in placed like 
//				attack or handle damage, only one will break max-this will optimize flow of data but create a slight unbalance
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::ApplyWearSpecific(int locationID,CInventoryObj ** brokenRef,int wearAmount)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(invObj->m_armed)
			if(invObj->m_itemData->m_locationID == locationID)
				if(invObj->m_itemData->m_hp > 1)
				{//item has wear rate
					invObj->m_itemData->m_hp-=wearAmount;//decrement health of item
					if( invObj->m_itemData->m_hp < 1)
						invObj->m_itemData->m_hp = 1;

					if(invObj->m_itemData->m_hp == 1)//just broke-notify (we know this will stop wear this rount on other prospects but thats ok
					{
						*brokenRef = invObj;
						return TRUE;//one did break this round notify-outside this function will handle delete and notification
					}
				}//end item has wear rate
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetAndRemoveByGlid(int)
// Description:	Retrieve and remove an item from inventory given a unique ID.
//				If found, return a pointer to the removed item.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::GetAndRemoveByGlid(int GL_ID)
{
	POSITION posLast = NULL;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(invObj->m_itemData->m_globalID == GL_ID)
		{
		   RemoveAt(posLast);
		   return invObj;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetAndRemoveAmountByGlid(int)
// Description:	Retrieve and remove an item from inventory given a unique ID and specified amount.
//				If found, return a pointer to the removed item.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj*
CInventoryObjList::GetAndRemoveAmountByGlid(int GL_ID, int amount)
{
	POSITION posLast = NULL;
	for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(invObj->m_itemData->m_globalID == GL_ID)
		{
			invObj->m_quanity -= amount;
			if(invObj->m_quanity <= 0)
				RemoveAt(posLast);
			return invObj;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	CombineToExpObj(int, int)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

int CInventoryObjList::CombineToExpObj(int glid_combineItem, int glid_targetObject)
{
	POSITION posLast = NULL;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{//3
        CInventoryObj *itemToCombine = (CInventoryObj*)GetNext(posLoc);
		if(itemToCombine->m_itemData->m_globalID == glid_combineItem)
		{//found the item to combine 
           if(!itemToCombine->m_itemData->m_expansiveCollection)
			   return 5;//no contents to add error

           for(POSITION posLoc2 = GetHeadPosition(); posLoc2 != NULL;)
		   {
               CInventoryObj *targetItemPtr = (CInventoryObj*)GetNext(posLoc2);
               if(targetItemPtr->m_itemData->m_globalID == glid_targetObject)
			   {//found target
                  if(!targetItemPtr->m_itemData->m_expansiveCollection)
                      targetItemPtr->m_itemData->m_expansiveCollection = new CCExpansiveObjList();

				  POSITION expPosLast = NULL;
				  for(POSITION expPos = itemToCombine->m_itemData->m_expansiveCollection->GetHeadPosition(); (expPosLast = expPos) != NULL;)
				  {
                      CExpansiveObj *expObj = (CExpansiveObj*)itemToCombine->m_itemData->m_expansiveCollection->GetNext(expPos);
                      if(!targetItemPtr->m_itemData->m_expansiveCollection->GetByMisc2(expObj->m_miscInt2))
					  {//doesnt exist already
                         targetItemPtr->m_itemData->m_expansiveCollection->AddTail(expObj);
                         itemToCombine->m_itemData->m_expansiveCollection->RemoveAt(expPosLast);
					  }//end doesnt exist already
					  else
						return 8;//already has been added error code
				  }

			      itemToCombine->SafeDelete();
				  delete itemToCombine;
				  itemToCombine = 0;
			      RemoveAt(posLast);
				  return 1;
			   }//end found target
		   }
		   return 2;//could not find the target combine item
		}//end found the item to combine 
	}//end 3
  return 0;//didnt have the target item (nothing to combine to)
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	Combine(CItemObj*, CItemObj*)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

int 
CInventoryObjList::Combine(CItemObj *itemToCombine, CItemObj *targetItemPtr)
{	
     if(!itemToCombine->m_expansiveCollection)
	     return 5; // No contents to add error code

     if(!targetItemPtr->m_expansiveCollection)
         targetItemPtr->m_expansiveCollection = new CCExpansiveObjList();

	 POSITION expPosLast = NULL;
	 for(POSITION expPos = itemToCombine->m_expansiveCollection->GetHeadPosition(); (expPosLast = expPos) != NULL;)
	 {
		CExpansiveObj *expObj = (CExpansiveObj*)itemToCombine->m_expansiveCollection->GetNext(expPos);
		if(!targetItemPtr->m_expansiveCollection->GetByMisc2(expObj->m_miscInt2))	// Doesn't exist already
		{
            CExpansiveObj *cloneExpObj = NULL;
			expObj->Clone(&cloneExpObj);
			targetItemPtr->m_expansiveCollection->AddTail(cloneExpObj);
		}
		else
		    return 8; // Already has been added error code
	 }
     return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetArmourClass(BOOL)
// Description:	Return the combined armour class for all items in inventory that
//				are currently armed.
///////////////////////////////////////////////////////////////////////////////

int 
CInventoryObjList::GetArmourClass(BOOL forVisualOnly)
{
	int armourClass = 0;
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		/* OLD ////////////////////////////////////////////////////////////////
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(!invObj->m_itemData)
			continue;

		if(forVisualOnly == TRUE)
			if(invObj->m_itemData->m_defensesSpecific != -1)
				continue;

		if(invObj->m_armed)
			armourClass += invObj->m_itemData->m_miscUseValue;
		/////////////////////////////////////////////////////////////////////*/

		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);

		// IF this inventory object has an associated item 
		// AND (it is NOT only for looks AND it is NOT an item with a defense specific characteristic)
		// AND it is armed
		// THEN tally it into the armour class total
		if(	(invObj->m_itemData) 
			&& ((forVisualOnly == FALSE) && (invObj->m_itemData->m_defensesSpecific == -1))
			&& (invObj->m_armed) )
		{
			armourClass += invObj->m_itemData->m_miscUseValue;
		}
	}

	// Clamp armour class total to 75
	if(armourClass > 75)
		armourClass = 75;
	return armourClass;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	CheckForSpecifiedDamage(int)
// Description:	Goes through armed inventory to find a specific defense to
//				an attack
///////////////////////////////////////////////////////////////////////////////
BOOL
CInventoryObjList::CheckForSpecifiedDamage(int specified)
{
	//Loop through our inventory for that value
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(invObj->m_armed)
			if(invObj->m_itemData)
				if(invObj->m_itemData->m_defensesSpecific == specified)
					return TRUE;
	}

	//None found
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetArmourRating(int, BOOL, int)
// Description:	Calculate the total armour rating for all armed items in the
//				player's inventory.
///////////////////////////////////////////////////////////////////////////////

int 
CInventoryObjList::GetArmourRating(int damageChannel,
								   BOOL forVisualOnly,
								   int attackSpecified)
{
	int armourRating = 0;
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		// IF this inventory object does NOT have an item associated with it
		// OR it does NOT have element defenses
		// OR it is NOT armed
		// OR is broken
		// THEN CONTINUE
		if(	!invObj->m_itemData
			|| !invObj->m_itemData->m_elementDefenses
			|| !invObj->m_armed
			|| invObj->m_itemData->m_hp == 1)
			continue;

		// IF this item is for looks only 
		// AND it doesn't have a defense specific characteristic
		// THEN CONTINUE
		if(	(forVisualOnly == TRUE) 
			&& (invObj->m_itemData->m_defensesSpecific != -1))
			continue;

		// IF it doesn't have any defense specific characteristics
		// AND the attack specified isn't the same as defense specific characteristic
		// THEN CONTINUE
		if(	(invObj->m_itemData->m_defensesSpecific != -1)
			&& (attackSpecified != invObj->m_itemData->m_defensesSpecific))
			continue;
		
		// We've made it through all the checks so
		// tally this item 
		for(POSITION defPos = invObj->m_itemData->m_elementDefenses->GetHeadPosition(); defPos != NULL;)
		{
			CElementStatusObj *elementStatusPtr = (CElementStatusObj*)invObj->m_itemData->m_elementDefenses->GetNext(defPos);
			if(damageChannel == elementStatusPtr->m_elementIndex)
				armourRating += elementStatusPtr->m_currentValue;
		}
	}

	// Clamp armour ratings to 95
    if(armourRating > 95)
		armourRating = 95;
	// Make sure we don't go below 0
	else if(armourRating < 0)
		armourRating = 0;
	return armourRating;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetEmblemResistBonus(int)
// Description:	Adds in a bonus based off of emblem effects.
///////////////////////////////////////////////////////////////////////////////

int					
CInventoryObjList::GetEmblemResistBonus(int armourValue)
{
	int armourRating = armourValue;

	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);

		if(invObj->m_itemData)
		{
			if(invObj->m_itemData->m_emblemEffects[RESIST_P01/*2*/])
				armourRating++;
		}
	}

	// Clamp armour ratings to 95
    if(armourRating > 95)
		armourRating = 95;
	else if(armourRating < 0)
		armourRating = 0;
	
	return armourRating;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetEmblemDamageBonus(int)
// Description:	Adds in a bonus based off of emblem effects.
///////////////////////////////////////////////////////////////////////////////

/*
float
CInventoryObjList::GetEmblemDamageBonus(float damage)
{
	float damageValue = damage;
	int temp = 0;
	
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);

		if(invObj->m_itemData)
		{
			if(invObj->m_itemData->m_emblemEffects[DAMAGE_P10/*1/])
				temp = 10;
			if(temp)
			{
				damageValue = damageValue + (rand() % temp);
				temp = 0;
			}
		}
	}

	return damageValue;
}
*/

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetEmblemDamageBonus(int)
// Description:	Adds in a bonus based off of emblem effects.
///////////////////////////////////////////////////////////////////////////////

int
CInventoryObjList::GetEmblemAmmoBonus()
{
	int ammoIncrease = 0;

	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);

		if(invObj->m_itemData)
		{
			if(invObj->m_itemData->m_emblemEffects[AMMO_REGEN_P02/*4*/])
				ammoIncrease += 2;
		}
	}

	return ammoIncrease;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetEmblemHealthBonus(int)
// Description:	Adds in a bonus based off of emblem effects.
///////////////////////////////////////////////////////////////////////////////

int
CInventoryObjList::GetEmblemHealthBonus()
{
	int healthIncrease = 0;

	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);

		if(invObj->m_itemData)
		{
			if(invObj->m_itemData->m_emblemEffects[HEALTH_P10/*0*/])
				healthIncrease += 10;
			if(invObj->m_itemData->m_emblemEffects[HEALTH_P20/*1*/])
				healthIncrease += 20;
		}
	}

	return healthIncrease;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetEmblemReductionBonus(int)
// Description:	Adds in a bonus based off of emblem effects.
///////////////////////////////////////////////////////////////////////////////

int
CInventoryObjList::GetEmblemReductionBonus()
{
	int reductionIncrease = 0;

	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);

		if(invObj->m_itemData)
		{
			if(invObj->m_itemData->m_emblemEffects[RESIST_P01/*3*/])
				reductionIncrease += 1;
		}
	}

	return reductionIncrease;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetInvenItemByLocationID(int)
// Description:	Retrieve an inventory item provided a location ID.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::GetInvenItemByLocationID(int id)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(invObj->m_itemData->m_locationID == id)
		   return invObj;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetInvenItemByLocationIDArmable(int, float, float, float)
// Description:	Retrieve an inventory item provided a location ID. Values
//				for strength, dexterity and wisdom are passed to check for 
//				the capability to arm the target item. If all checks are passed,
//				return a pointer to the found item.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::GetInvenItemByLocationIDArmable(int id,
												   float rStr,
												   float rDex,
												   float rWis)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		/* OLD ////////////////////////////////////////////////////////////////
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(invObj->m_itemData->m_locationID == id)
			if(invObj->m_itemData->m_requiredStr <= rStr && invObj->m_itemData->m_requiredDex <= rDex &&  invObj->m_itemData->m_requiredKnowledge <= rWis)
			{
			   return invObj;
			}
		/////////////////////////////////////////////////////////////////////*/

        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(	(invObj->m_itemData->m_locationID == id)
			&& (invObj->m_itemData->m_requiredStr <= rStr)
			&& (invObj->m_itemData->m_requiredDex <= rDex)
			&& (invObj->m_itemData->m_requiredKnowledge <= rWis))
		{
		   return invObj;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetArmedInvenItemByLocationID(int)
// Description:	Retrieve an item from inventory provided a location ID.
//				Only return a pointer to the item if it is currently armed.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::GetArmedInvenItemByLocationID(int id)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if((invObj->m_armed == TRUE) && (invObj->m_itemData->m_locationID == id))
		   return invObj;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetUnarmedWeapon()
// Description:	Return a pointer to any unarmed weapon in location ID 12.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::GetUnarmedWeapon()
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(	invObj->m_itemData->m_permanent 
			&& invObj->m_itemData->m_unarmedAttack 
			&& invObj->m_itemData->m_locationID == 12)
		{
			return invObj;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	UseIfKeyInInventory(int)
// Description:	Remove an item from the inventory specified by the key ID.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::UseIfKeyInInventory(int keyID)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(invObj->m_itemData->m_keyID == keyID) // Found key!
		{
           if(invObj->m_itemData->m_destroyOnUse)
		   {
              --invObj->m_quanity;
              if(invObj->m_quanity < 1)
			  {
		         invObj->SafeDelete();
		         delete invObj;
		         invObj = 0;
	             RemoveAt(posLast);
			  }
		   }
		   return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLIDAll(int)
// Description:	Remove all items from inventory with the specified GLID.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLIDAll(int GLID)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(GLID == invObj->m_itemData->m_globalID)
		{
			invObj->SafeDelete();
			delete invObj;
			invObj = 0;
			RemoveAt(posLast);
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByLOCIDAll(int)
// Description:	Remove all items from inventory with the specified location ID.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByLOCIDAll(int locationID)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(locationID == invObj->m_itemData->m_locationID)
		{
			if(invObj->m_armed)
				invObj->m_armed = FALSE;
			invObj->SafeDelete();
		      delete invObj;
		      invObj = 0;
	          RemoveAt(posLast);
		}
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLID(int)
// Description:	Remove the first item from inventory with the specified GLID.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLID(int GLID)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(GLID == invObj->m_itemData->m_globalID)
		{
           --invObj->m_quanity;
           if(invObj->m_quanity < 1)
		   {
		      invObj->SafeDelete();
		      delete invObj;
		      invObj = 0;
	          RemoveAt(posLast);
		   }
		   return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLID_DPTRACE(int, int, int)
// Description:	
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLID_DPTRACE(int GLID, int dupTrace, int quanity)
{
	POSITION p1 = NULL, p2 = NULL;
	int dupTraceLocal = 0;
    for(p1 = p2 = GetHeadPosition(); (p2 = p1) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(p1);
        if(GLID == invObj->m_itemData->m_globalID)
		{
           if(dupTraceLocal != dupTrace)
		   {
			   ++dupTraceLocal;
			   continue;
		   }

           invObj->m_quanity -= quanity;
           if(invObj->m_quanity < 1)
		   {
		      invObj->SafeDelete();
		      delete invObj;
		      invObj = 0;
	          RemoveAt(p2);
		   }
		   return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLID_DPTRACE(int, int)
// Description:	
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLID_DPTRACE(int GLID, int dupTrace)
{
	POSITION posLast;
	int dupTraceLocal = 0;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(GLID == invObj->m_itemData->m_globalID)
		{
           if(dupTraceLocal != dupTrace)
		   {
			   ++dupTraceLocal;
			   continue;
		   }

           --invObj->m_quanity;
           if(invObj->m_quanity < 1)
		   {
		      invObj->SafeDelete();
		      delete invObj;
		      invObj = 0;
	          RemoveAt(posLast);
		   }
		   return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByName(CString)
// Description:	Remove one instance of the first item from inventory with 
//				the specified name.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByName(CString itemName)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(itemName.CompareNoCase(invObj->m_itemData->m_itemName) == 0)
		{
           --invObj->m_quanity;
           if(invObj->m_quanity < 1)
		   {
		      invObj->SafeDelete();
		      delete invObj;
		      invObj = 0;
	          RemoveAt(posLast);
		   }
		   return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLID(int, int)
// Description:	Remove one instance of the first item from inventory with 
//				the specified GLID.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLID(int GLID, int quanity)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(GLID == invObj->m_itemData->m_globalID)
		{
           invObj->m_quanity -= quanity;
           if(invObj->m_quanity < 1)
		   {
		      invObj->SafeDelete();
		      delete invObj;
		      invObj = 0;
	          RemoveAt(posLast);
		   }
		   return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLIDBroken(int, int)
// Description:	Remove one instance of the first broken item from inventory with 
//				the specified GLID.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLIDBroken(int GLID, int quanity)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(GLID == invObj->m_itemData->m_globalID && invObj->m_itemData->m_hp == 1)
		{
           invObj->m_quanity -= quanity;
           if(invObj->m_quanity < 1)
		   {
		      invObj->SafeDelete();
		      delete invObj;
		      invObj = 0;
	          RemoveAt(posLast);
		   }
		   return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLIDX(int, int)
// Description:	Remove the specified quantity of the item with the
//				specified GLID from inventory.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLIDX(int GLID, int quanity)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(GLID == invObj->m_itemData->m_globalID)
		{
           invObj->m_quanity -= quanity;
		   int tempRemainder = invObj->m_quanity;
           if(invObj->m_quanity < 1)
		   {
		      invObj->SafeDelete();
		      delete invObj;
		      invObj = 0;
	          RemoveAt(posLast);
		   }
           if(tempRemainder >= 0)
		      return TRUE;
		   else
		      return FALSE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLIDUnarmedOnly(int)
// Description:	Remove one instance of the inventory item with the specified
//				GLID only if it's not currently armed.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLIDUnarmedOnly(int GLID)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if((GLID == invObj->m_itemData->m_globalID) && (invObj->m_armed == FALSE))
		{
		   --invObj->m_quanity;
           if(invObj->m_quanity < 1)
		   {
			  invObj->SafeDelete();
		      delete invObj;
		      invObj = 0;
	          RemoveAt(posLast);
		   }
		   return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetByUniqueSync(long)
// Description:	Return a pointer to the object whose unique sync matches the
//				ID parameter passed in.
///////////////////////////////////////////////////////////////////////////////

CInventoryObj* 
CInventoryObjList::GetByUniqueSync(long id)
{
    for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(id == invObj->m_tempUniqueSync)
		   return invObj;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLIDUnarmedOnlyWQuanity(int, int)
// Description:	Remove the specified quantity of the item corresponding to the
//				passed in GLID only if it's currently unarmed.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLIDUnarmedOnlyWQuanity(int GLID, int quanity)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(GLID == invObj->m_itemData->m_globalID && invObj->m_armed == FALSE)
		{
			/* DEPRECATED /////////////////////////////////////////////////////
			if(quanity > invObj->m_quanity)
			  return FALSE;
			/////////////////////////////////////////////////////////////////*/

			invObj->m_quanity -= quanity;
			if(invObj->m_quanity < 1)
			{
			  invObj->SafeDelete();
			  delete invObj;
			  invObj = 0;
			  RemoveAt(posLast);
			}
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	DeleteByGLIDUnarmedOnlyWQuanity(int, int, int)
// Description:	Remove the specified quantity of the item corresponding to the
//				passed in GLID only if it's currently unarmed and it's damage
//				modifier is equivalent to the specified damage modifier value.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::DeleteByGLIDUnarmedOnlyWQuanity(int GLID, int quanity, int damageMod)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
		/* OLD ////////////////////////////////////////////////////////////////
		CInventoryObj * invObj = (CInventoryObj *)GetNext(posLoc);
        if(GLID == invObj->m_itemData->m_globalID)
			if(invObj->m_armed == FALSE)
				if(damageMod == (int)invObj->m_itemData->m_damageModifier)
		/////////////////////////////////////////////////////////////////////*/

        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
        if(	(GLID == invObj->m_itemData->m_globalID)
			&& (invObj->m_armed == FALSE)
		    && (damageMod == (int)invObj->m_itemData->m_damageModifier))
		{
			invObj->m_quanity -= quanity;
			if(invObj->m_quanity < 1)
			{
				invObj->SafeDelete();
				delete invObj;
				invObj = 0;
				RemoveAt(posLast);
			}
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	ReorderInventory(int, int, int)
// Description:	Re-order an item in the inventory given an index for which item
//				to move and an index for its target location.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::ReorderInventory(int objectToMoveIndex, 
									int targetIndex,
									int filterType)
{

	// Error checking for silly conditions
	if(	(objectToMoveIndex == targetIndex)
		|| (objectToMoveIndex < 0)
		|| (targetIndex < 0))
	   return FALSE;

	/* DEPRECATED /////////////////////////////////////////////////////////////
	if(targetIndex > 0)
	   targetIndex = targetIndex - 1;
	/////////////////////////////////////////////////////////////////////////*/
	
	POSITION posObjectToMove = NULL;	// FindIndex(objectToMoveIndex);
	int trace = 0;
	POSITION posLoc;

    for(posLoc = GetHeadPosition(); (posObjectToMove = posLoc) != NULL;)
    {
        CInventoryObj *invPtrTemp = (CInventoryObj*)GetNext(posLoc);
	    if(	(filterType == 1)
			&& (invPtrTemp->m_itemData)
			&& (invPtrTemp->m_itemData->m_permanent == TRUE))
				 continue;

		if(trace == objectToMoveIndex) break;
	    trace++;
	}

	// If we are at the end of the list, we couldn't find the object
	if(posObjectToMove == NULL)
	   return FALSE;

	// Get a pointer to the item we want to move and yank it out of the list
	CInventoryObj *invObj = (CInventoryObj*)GetAt(posObjectToMove);
	RemoveAt(posObjectToMove);

	// If new index is 0, just pop it on the front
	if(targetIndex == 0)
	{
		AddHead(invObj);
		return TRUE;
	}
	else
	{
		trace = 0;
		for(posLoc = GetHeadPosition(); posLoc != NULL;)
		{
			CInventoryObj *invPtrTemp = (CInventoryObj*)GetAt(posLoc);

			// If there's an object with an associated item at this location
			// and it's permanent, move along...
			if(	(filterType == 1)
				&& (invPtrTemp->m_itemData)
				&& (invPtrTemp->m_itemData->m_permanent == TRUE))
			{
				GetNext(posLoc);
				continue;
			}

			// We've hit our target, insert the item
			if(trace == targetIndex)
			{
				InsertBefore(posLoc,invObj);
				return TRUE;
			}
			GetNext(posLoc);
			++trace;
		}
	}

	// We haven't found a new pos for the item? 
	// Just pop it on the back!
	AddTail(invObj);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GetStatBenefit(int)
// Description:	Retrieve the modifier value of the first armed item with a 
//				bonus modifiers.
///////////////////////////////////////////////////////////////////////////////

int 
CInventoryObjList::GetStatBenefit(int statID)
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		/* OLD ////////////////////////////////////////////////////////////////
		CInventoryObj * tempObj = (CInventoryObj *)GetNext(posLoc);
		if(tempObj->m_armed)
			if(tempObj->m_itemData)
				if(tempObj->m_itemData->m_statBonuses)
		/////////////////////////////////////////////////////////////////////*/

		CInventoryObj *tempObj = (CInventoryObj*)GetNext(posLoc);
		if( (tempObj->m_armed)
			&& (tempObj->m_itemData)
			&& (tempObj->m_itemData->m_statBonuses))
		{
			return tempObj->m_itemData->m_statBonuses->GetBonusByID(statID);
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	PhysicChanging()
// Description:	If there is in object in the inventory with modifiers greater
//				than a threshold value, i.e. >= 1000, return TRUE.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::PhysicChanging()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		/* OLD ////////////////////////////////////////////////////////////////
		CInventoryObj * temoObj = (CInventoryObj *)GetNext(posLoc);
		if(temoObj->m_armed)
			if(temoObj->m_itemData)
				if(temoObj->m_itemData->m_statBonuses)
		/////////////////////////////////////////////////////////////////////*/

		CInventoryObj *tempObj = (CInventoryObj*)GetNext(posLoc);
		if( (tempObj->m_armed)
			&& (tempObj->m_itemData)
			&& (tempObj->m_itemData->m_statBonuses))
		{
			for(POSITION posLocX = tempObj->m_itemData->m_statBonuses->GetHeadPosition(); posLocX != NULL;)
			{
				CStatBonusObj *statBns = (CStatBonusObj*)tempObj->m_itemData->m_statBonuses->GetNext(posLocX);
				if(statBns->m_statBonus >= 1000)
					return TRUE;
			}
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	SerializeAlloc(CArchive&, int)
// Description:	Read/Write the inventory list to/from storage.
///////////////////////////////////////////////////////////////////////////////

void 
CInventoryObjList::SerializeAlloc(CArchive& ar, int loadCount)
{
	if(ar.IsStoring())
	{	
	   for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	   {
           CInventoryObj *tempObj = (CInventoryObj*)GetNext(posLoc);
		   tempObj->SerializeAlloc(ar);
	   }
	}
	else
	{
       for(int i=0; i<loadCount; i++)
	   {
		   CInventoryObj *tempObj = new CInventoryObj();
           tempObj->SerializeAlloc(ar);
		   AddTail(tempObj);
	   }
	}
}

BOOL
CInventoryObjList::GenerateBadgeDataBlock(BYTE **dataGLIDBlock, int &blockSize, int &datasize)
{
	blockSize = 0;
	short headerSize = sizeof(short);
	POSITION posLoc;
	for(posLoc = GetHeadPosition(); posLoc != NULL;)
	{//Size Loop
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(invObj->m_itemData->m_useType != 13)
			continue;
		blockSize++;
	}//End size loop

	if(!blockSize)
		return FALSE;

	//Setup the size of the info
	datasize = (sizeof(short) * blockSize) + headerSize + 1;
	BYTE *dataArray = new BYTE[datasize];
	ZeroMemory(dataArray,datasize);

	int trace = headerSize + 1;
	dataArray[0] = 12; //Setup the catagory for HandleMiscBlocksClientSide
	dataArray[1] = (short)blockSize;
	
	short glID = 0;
	short structSize = sizeof(short);
	for(posLoc = GetHeadPosition(); posLoc != NULL;)
	{//Fill data Loop
		CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);
		if(invObj->m_itemData->m_useType != 13)
			continue;

		glID = (short)invObj->m_itemData->m_globalID;
        CopyMemory(&dataArray[trace],&glID,structSize);
		trace += structSize;
	}//End fill data loop
	
	*dataGLIDBlock = (BYTE*)dataArray;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GenerateInventoryDataBlock(int, BYTE**, int, int, BOOL, int, int)
// Description:	Convert an inventory list into a stream of bytes to transfer
//				over the network.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::GenerateInventoryDataBlock(int includeArmed,
											  BYTE **dataGLIDBlock,
											  int &blockSize,
											  int &datasize,
											  BOOL filterNonTransferable,
											  int headerSize,
											  int filter)
{
	blockSize = 0;
	POSITION posLoc;
	//test
	filter = 0;
    for(posLoc = GetHeadPosition(); posLoc != NULL;)
	{ // Size Loop
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);

		// Do not display permanent items
		if(invObj->m_itemData->m_permanent)
		   continue;

		// Filter Non-Transferable Items
        if(filterNonTransferable && invObj->m_itemData->m_nonTransferable)
			continue;

		// Filter Items that are Armed and cannot be disarmed
		if(invObj->m_itemData->m_disarmable == FALSE && invObj->m_armed == TRUE)
		   continue;
		
		// Only Show Armed Items
		if(includeArmed == 2 && !invObj->m_armed)
			continue;
		
		// Filter Armed Items
		if(includeArmed == 0 && invObj->m_armed)
			continue;

		// Filter Types:
		// 0 = none
		// 1 = skills only
		// 2 = spells only
		// 3 = no skills / spells

		// Only Show Skills
		if(filter == 1 && invObj->m_itemData->m_useType != 12)
			continue;

		// Only Show Spells
		if(filter == 2 && invObj->m_itemData->m_useType != 11)
			continue;

		// Filter Spell and Skill Items Out (Don't show spells / skills)
		if(filter == 3 && invObj->m_itemData->m_useType == 11)
			continue;

		if(filter == 3 && invObj->m_itemData->m_useType == 12)
			continue;

		blockSize++;
	} // End size loop

    if(blockSize == 0)
	   return FALSE;

	datasize = (sizeof(ITEMDATA_STRUCT) * blockSize) + headerSize;
	BYTE *dataArray = new BYTE[datasize];
	ZeroMemory(dataArray,datasize);
	//ITEMDATA_STRUCT * dataArray = new ITEMDATA_STRUCT[blockSize]+headerSize;

	int trace = headerSize;
	ITEMDATA_STRUCT tempStruct;
	int structSize = sizeof(ITEMDATA_STRUCT);
	for(posLoc = GetHeadPosition(); posLoc != NULL;)
	{ // Size Loop
        CInventoryObj *invObj = (CInventoryObj*)GetNext(posLoc);

        /* DEPRECATED /////////////////////////////////////////////////////////
		if(invObj->m_itemData->m_permanent)
		   continue;

        if(filterNonTransferable)
		   if(invObj->m_itemData->m_nonTransferable)
			  continue;

        if(invObj->m_itemData->m_disarmable == FALSE && invObj->m_armed == TRUE)
		   continue;

		if(!includeArmed)
			if(invObj->m_armed)
			   continue;
		/////////////////////////////////////////////////////////////////////*/

		//Do not display permanent items
		if(invObj->m_itemData->m_permanent)
		   continue;

		// Filter Non-Transferable Items
        if(filterNonTransferable && invObj->m_itemData->m_nonTransferable)
			continue;

		// Filter Items that are Armed and cannot be disarmed
		if(invObj->m_itemData->m_disarmable == FALSE && invObj->m_armed == TRUE)
		   continue;
		
		// Only Show Armed Items
		if(includeArmed == 2 && !invObj->m_armed)
			continue;
		
		// Filter Armed Items
		if(includeArmed == 0 && invObj->m_armed)
			continue;

		// Filter Types:
		// 0 = none
		// 1 = skills only
		// 2 = spells only
		// 3 = no skills / spells

		// Only Show Skills
		if(filter == 1 && invObj->m_itemData->m_useType != 12)
			continue;

		// Only Show Spells
		if(filter == 2 && invObj->m_itemData->m_useType != 11)
			continue;

		// Filter Spell and Skill Items Out (Don't show spells / skills)
		if(filter == 3 && invObj->m_itemData->m_useType == 11)
			continue;

		if(filter == 3 && invObj->m_itemData->m_useType == 12)
			continue;

        tempStruct.m_GLID    = invObj->m_itemData->m_globalID;
		tempStruct.m_quantity = invObj->m_quanity;
		tempStruct.m_dMod    = invObj->m_itemData->m_damageModifier;
		tempStruct.m_armed	 = invObj->m_armed;
        CopyMemory(&dataArray[trace], &tempStruct,structSize);

		trace += structSize;
	} // End Size Loop

	*dataGLIDBlock = (BYTE*)dataArray;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Function:	GeneratePartialInventoryDataBlock(BLOCKITEMLIST_STRUCT, BYTE**, int, int, BOOL, int, int)
// Description:	Convert an inventory list into a stream of bytes to transfer
//				over the network.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CInventoryObjList::GeneratePartialInventoryDataBlock(std::vector<short> &indices,
													 BLOCKITEMLIST_STRUCT header,
													 BYTE **pInventoryData,
													 int &iSize)
{
	// SETUP network packet
	iSize = sizeof(header) + (sizeof(ITEMDATA_STRUCT) * (sizeof(short) * indices.size()));
	BYTE *pData = new BYTE[iSize];
	ZeroMemory(pData, iSize);

	// ENCODE header into the network packet
	CopyMemory(pData, &header, sizeof(BLOCKITEMLIST_STRUCT));//was copied over the caller's pointer (stack smash crash)

	int iCount = 0, iTrace = sizeof(header);
	ITEMDATA_STRUCT item;
	std::vector<short>::iterator iter;
	for (iter = indices.begin(); iter != indices.end(); iter++)
	{
		// GET the item at the specified index
		POSITION p = this->FindIndex(*iter);
		CInventoryObj *pItem = (CInventoryObj*)this->GetAt(p);
		if (!pItem) continue;

		// ENCODE item info into network packet
        item.m_GLID = pItem->m_itemData->m_globalID;
		item.m_quantity = pItem->m_quanity;
		item.m_dMod = pItem->m_itemData->m_damageModifier;
		item.m_armed = pItem->m_armed;
        CopyMemory(&pData[iTrace], &item, sizeof(ITEMDATA_STRUCT));

		// INCREMENT packet size tracker
		iTrace += sizeof(ITEMDATA_STRUCT);
	}

	*pInventoryData = (BYTE*)pData;
	return TRUE;
}

/* DEPRECATED /////////////////////////////////////////////////////////////////
int CInventoryObjList::AddInventoryItem(int  GLID,
										int  quanity,
										BOOL armed,
										BOOL disarmable,
										int  locationID,
										BOOL permanent,
										int  armourClass,
										BOOL nonDrop)
{
	CInventoryObj * invObj = new CInventoryObj();
	invObj->m_itemData = new CItemObj();
	invObj->m_itemData->m_globalID             = GLID;
	invObj->m_itemData->m_disarmable           = disarmable;
	invObj->m_itemData->m_locationID           = locationID;
	invObj->m_itemData->m_permanent            = permanent;
	invObj->m_quanity                          = quanity;
	invObj->m_armed                            = armed;
	invObj->m_itemData->m_armourClass          = armourClass;
	invObj->m_itemData->m_nonDrop              = nonDrop;

    AddTail(invObj);
return 1;//added item
}
/////////////////////////////////////////////////////////////////////////////*/


#define D3D_OVERLOADS

#include <afxtempl.h>
#include "stdafx.h"
#include "CItemClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CStatBonusObj, CObject, 0)
IMPLEMENT_SERIAL(CStatBonusList, CObList, 0)
IMPLEMENT_SERIAL(CAttachmentRefObj, CObject, VERSIONABLE_SCHEMA|4)
IMPLEMENT_SERIAL(CAttachmentRefList, CObList, 0)
IMPLEMENT_SERIAL(CItemObj, CObject, VERSIONABLE_SCHEMA|13)
IMPLEMENT_SERIAL(CItemObjList, CObList, 0)

///////////////////////////////////////////////////////////////////////////////
// Class:		CAttachmentRefObj
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CAttachmentRefObj::CAttachmentRefObj()
{
    m_attachmentType		= -1;
	m_raceBind				= -1; // -1 will give any access
	m_dimIndex				= -1;
    m_dimSwitchTo			= -1;
	m_fireQueOvrd			= 0;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CAttachmentRefObj
// Function:	Serialize(CArchive&)
// Description:	Serialize this node for storage.
///////////////////////////////////////////////////////////////////////////////

void 
CAttachmentRefObj::Serialize(CArchive& ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring())
	{	
		ar.SetObjectSchema(4);
		ASSERT(ar.GetObjectSchema() == 4);

		ar	<< m_attachmentType
			<< m_raceBind
			<< m_dimIndex
			<< m_dimSwitchTo
			<< m_fireQueOvrd;
	}
	else
	{
		int version = ar.GetObjectSchema();
		m_fireQueOvrd = 0;

		switch(version)
		{
		case 2:
			ar	>> m_attachmentType
				>> m_raceBind;
			break;
		case 3:
			ar	>> m_attachmentType
				>> m_raceBind
				>> m_dimIndex
				>> m_dimSwitchTo;
			break;
		case 4:
			ar	>> m_attachmentType
				>> m_raceBind
				>> m_dimIndex
				>> m_dimSwitchTo
				>> m_fireQueOvrd;
			break;
		default:
			ar	>> m_attachmentType;
			break;
		}
	}
}     

///////////////////////////////////////////////////////////////////////////////
// Class:		CAttachmentRefList
// Function:	SafeDelete(CArchive&)
// Description:	Safely delete all nodes in this list of attachment objects.
///////////////////////////////////////////////////////////////////////////////

void 
CAttachmentRefList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAttachmentRefObj *spnPtr = (CAttachmentRefObj*)GetNext(posLoc);
		delete spnPtr;
		spnPtr = 0;
	}
	RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CAttachmentRefList
// Function:	Clone(CAttachmentRefList**)
// Description:	Perform a deep copy of all nodes in this list of attachment
//				objects.
///////////////////////////////////////////////////////////////////////////////

void 
CAttachmentRefList::Clone(CAttachmentRefList **clone)
{
	CAttachmentRefList *copyLocal = new CAttachmentRefList();
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CAttachmentRefObj *bnPtr		= (CAttachmentRefObj*)GetNext(posLoc);
		CAttachmentRefObj *newObject	= new CAttachmentRefObj();  
		newObject->m_attachmentType		= bnPtr->m_attachmentType;
		newObject->m_dimIndex			= bnPtr->m_dimIndex;
		newObject->m_dimSwitchTo		= bnPtr->m_dimSwitchTo;
		newObject->m_raceBind			= bnPtr->m_raceBind;
		newObject->m_fireQueOvrd		= bnPtr->m_fireQueOvrd;
	
		copyLocal->AddTail(newObject);
	}
	*clone = copyLocal;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObj
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CItemObj::CItemObj()
{
	m_isInQuickSlots	   = false;
	m_animAccessDefault	   = 0;
    m_itemName             = "None";
    m_attachmentRefList    = NULL;
    m_marketCost           = 0;
    m_requiredDex          = 0;
	m_requiredStr          = 0;
	m_requiredKnowledge    = 0;
	m_equiped              = FALSE;
	m_locationID           = -1;
	m_disarmable           = TRUE;
	m_globalID             = -1;
    m_animAccessValue      = -1;
	m_skillUseRef          = -1;
    m_minimumLevel         = 0;
    m_masteredLevel        = 0;
	m_permanent            = FALSE;
	m_useType              = 0;
	m_miscUseValue         = 0;
	m_armourClass          = 0;
	m_raceID               = 0;
	m_nonDrop              = FALSE;
	m_statBonuses          = 0;
	m_dbSrcIndex           = 0;
	m_chargeType           = 0;
	m_chargeCurrent        = 0;
	m_missileLoaded		   = 0;
    m_maxCharges           = 0;
	m_keyID                = 0;
	m_elementDefenses      = NULL;
    m_nonTransferable      = 0;
	m_unarmedAttack		   = FALSE;
    m_destroyOnUse         = 0;
    m_requiredSkillType    = 0;
    m_requiredSkillLevel   = 0;
	m_stackable            = FALSE;
	m_expansiveCollection  = NULL;
	m_defensesSpecific     = -1;	// -1 = normal
	m_creationTimeStamp    = 0;
    m_timoutValue          = 0;
	m_visualRef            = 0;
    m_yJetforce            = 0.0f;
    m_zJetforce            = 0.0f;
	m_dimCfgIndex          = -1;
	m_dimCfgChangeTo       = -1;
	m_dimCfgChangeBack     = -1;
	m_reloadTime           = 0;
	m_damageModifier       = 0;
	m_bonusDuration		   = 0;
	m_damageMultiplier	   = 1.0f;
	m_bonusType			   = 0;
	m_abilityType		   = 0;
	m_explosionRef		   = -1;
	m_shotsPerFire		   = 1;
	m_tempDepletionHolder  = 0;
	m_weaponDepletionTimer = 0;
	m_secsToGo			   = 0;
	m_ammoRegenRate		   = 1;
	m_ammoSecTick		   = 3;
	m_hp				   = 0;
	m_maxHP				   = 2000;
	m_destroyOnBreak	   = false;
	m_dynDamageOvrd			= -1;//alters bottome line damage on server damage handler
	m_dynDamageElementOvrd	= -1;//this alters the channel on the fly during the damage handler
	m_dynDamageFireQueOvrd  = -1;//alters fire que on server fire effect distribution and local client

	for(int i = 0; i < (int)(MAX_EMBLEM); i++)
		m_emblemEffects[i] = 0;

	/* DEPRECATED =============================================================
	//m_setCost              = 0;
	=========================================================================*/
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObj
// Function:	SafeDelete()
// Description:	Safely release system resources utilized by this item object.
///////////////////////////////////////////////////////////////////////////////

void 
CItemObj::SafeDelete()
{
	if(m_attachmentRefList)
	{
		m_attachmentRefList->SafeDelete();
		delete m_attachmentRefList;
		m_attachmentRefList=0;
	}

	if(m_elementDefenses)
	{
		m_elementDefenses->SafeDelete();
		delete m_elementDefenses;
		m_elementDefenses = 0;
	}

	if(m_statBonuses)
	{
		m_statBonuses->SafeDelete();
		delete m_statBonuses;
		m_statBonuses = 0;
	}

	if(m_expansiveCollection)
	{
		m_expansiveCollection->SafeDelete();
		delete m_expansiveCollection;
		m_expansiveCollection = 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObj
// Function:	Clone(CItemObj**)
// Description:	Perform a deep copy of one game item's properties to another. 
///////////////////////////////////////////////////////////////////////////////

void 
CItemObj::Clone(CItemObj **clone)
{
	// Instantiate new item object for cloning 
    CItemObj *copyObj				= new CItemObj();

	// Copy primitive data type values to clone
	copyObj->m_itemName				= m_itemName;
	copyObj->m_marketCost			= m_marketCost;
	copyObj->m_requiredDex			= m_requiredDex;
	copyObj->m_requiredKnowledge	= m_requiredKnowledge;
	copyObj->m_requiredStr			= m_requiredStr;
	copyObj->m_equiped				= FALSE;
    copyObj->m_locationID			= m_locationID;
	copyObj->m_disarmable			= m_disarmable;
	copyObj->m_unarmedAttack		= m_unarmedAttack;
	copyObj->m_globalID				= m_globalID;
	copyObj->m_animAccessValue		= m_animAccessValue;
	copyObj->m_animAccessDefault	= m_animAccessDefault;
	copyObj->m_skillUseRef			= m_skillUseRef;
	copyObj->m_minimumLevel			= m_minimumLevel;
    copyObj->m_masteredLevel		= m_masteredLevel;
	copyObj->m_permanent			= m_permanent;
	copyObj->m_isInQuickSlots		= m_isInQuickSlots;
	copyObj->m_useType				= m_useType;
	copyObj->m_miscUseValue			= m_miscUseValue;
	copyObj->m_armourClass			= m_armourClass;
	copyObj->m_raceID				= m_raceID;
	copyObj->m_nonDrop				= m_nonDrop;
	copyObj->m_dbSrcIndex			= m_dbSrcIndex;
	copyObj->m_chargeType			= m_chargeType;
	copyObj->m_chargeCurrent		= m_chargeCurrent;
	copyObj->m_missileLoaded		= m_missileLoaded;
    copyObj->m_maxCharges			= m_maxCharges;
	copyObj->m_keyID				= m_keyID;
	copyObj->m_nonTransferable		= m_nonTransferable;
    copyObj->m_destroyOnUse			= m_destroyOnUse;
	copyObj->m_requiredSkillType	= m_requiredSkillType;
    copyObj->m_requiredSkillLevel	= m_requiredSkillLevel;
	copyObj->m_stackable			= m_stackable;
	copyObj->m_defensesSpecific		= m_defensesSpecific;
	copyObj->m_timoutValue			= m_timoutValue;
	copyObj->m_visualRef			= m_visualRef;
	copyObj->m_yJetforce			= m_yJetforce;
    copyObj->m_zJetforce			= m_zJetforce;
	copyObj->m_dimCfgIndex			= m_dimCfgIndex;
	copyObj->m_dimCfgChangeTo		= m_dimCfgChangeTo;
	copyObj->m_dimCfgChangeBack		= m_dimCfgChangeBack;
	copyObj->m_creationTimeStamp	= GetTickCount();
	copyObj->m_reloadTime			= m_reloadTime;
	copyObj->m_damageModifier		= m_damageModifier;
	copyObj->m_bonusDuration		= m_bonusDuration;
	copyObj->m_damageMultiplier		= m_damageMultiplier;
	copyObj->m_bonusType			= m_bonusType;
	copyObj->m_abilityType			= m_abilityType;
	copyObj->m_explosionRef			= m_explosionRef;
	copyObj->m_shotsPerFire			= m_shotsPerFire;
	copyObj->m_tempDepletionHolder  = m_tempDepletionHolder;
	copyObj->m_weaponDepletionTimer = m_weaponDepletionTimer;
	copyObj->m_secsToGo				= m_secsToGo;
	copyObj->m_ammoRegenRate		= m_ammoRegenRate;
	copyObj->m_ammoSecTick			= m_ammoSecTick;
	copyObj->m_hp					= m_hp;
	copyObj->m_maxHP				= m_maxHP;
	copyObj->m_destroyOnBreak		= m_destroyOnBreak;
	copyObj->m_dynDamageOvrd		= m_dynDamageOvrd;//alters bottome line damage on server damage handler
	copyObj->m_dynDamageElementOvrd	= m_dynDamageElementOvrd;//this alters the channel on the fly during the damage handler
	copyObj->m_dynDamageFireQueOvrd = m_dynDamageFireQueOvrd;//alters fire que on server fire effect distribution and local client

	for(int i = 0; i < MAX_EMBLEM; i++)
		copyObj->m_emblemEffects[i] = m_emblemEffects[i];

	// Copy custom object data member values to clone
	if(m_statBonuses)
        m_statBonuses->Clone(&copyObj->m_statBonuses);
     
	if(m_attachmentRefList)
		m_attachmentRefList->Clone(&copyObj->m_attachmentRefList);

	if(m_elementDefenses)
		m_elementDefenses->Clone(&copyObj->m_elementDefenses);

	if(m_expansiveCollection)
		m_expansiveCollection->Clone(&copyObj->m_expansiveCollection);

	// Set in param to point to our clone instance
	*clone = copyObj;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObj
// Function:	SerializeAlloc(CArchive&)
// Description:	Read/Write an item's expansive collection to/from storage.
///////////////////////////////////////////////////////////////////////////////

void 
CItemObj::SerializeAlloc(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar	<< m_globalID
			<< m_expansiveCollection;		// Migrated
	}
	else
	{
		ar	>> m_globalID
			>> m_expansiveCollection;		// Migrated
	}
}     

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObj
// Function:	Serialize(CArchive&)
// Description:	Read/Write an item object to/from storage.
//				Current serialization version is 8.
///////////////////////////////////////////////////////////////////////////////

void 
CItemObj::Serialize(CArchive& ar)
{
	BOOL reserved = 0;
	char reservedByte = 0;
	CObject::Serialize(ar);

	// Write item to storage
	if(ar.IsStoring())
	{
	   ar.SetObjectSchema(13);
	   ASSERT(ar.GetObjectSchema() == 13);

	   ar << m_itemName
		  << m_marketCost
		  << m_requiredDex
		  << m_requiredStr
		  << m_requiredKnowledge
		  << m_attachmentRefList
		  << m_equiped
		  << m_locationID
		  << m_disarmable
		  << m_globalID
		  << m_animAccessValue
		  << m_permanent
		  << m_useType
		  << m_miscUseValue
		  << m_armourClass
		  << m_raceID
		  << m_nonDrop
		  << m_dbSrcIndex
		  << m_statBonuses
		  << m_nonTransferable
		  << m_destroyOnUse
		  << m_requiredSkillType
		  << m_requiredSkillLevel
		  << m_stackable
		  << m_timoutValue
		  << m_elementDefenses
		  << m_chargeType
		  << m_chargeCurrent
		  << m_maxCharges
		  << m_keyID
		  << m_expansiveCollection		// migrated
		  << m_defensesSpecific
		  << m_visualRef
		  << m_yJetforce
		  << m_zJetforce
		  << m_dimCfgIndex
		  << m_dimCfgChangeTo
		  << m_dimCfgChangeBack
		  << m_damageModifier
		  << m_reloadTime
		  << m_bonusDuration
		  << m_damageMultiplier
		  << m_bonusType
		  << m_abilityType
		  << m_unarmedAttack
		  << m_explosionRef
		  << m_missileLoaded
		  << m_shotsPerFire
		  << m_ammoRegenRate
		  << m_ammoSecTick
		  << m_animAccessDefault
		  << m_hp
		  << m_destroyOnBreak;
		for(int i = 0; i < (int)MAX_EMBLEM; i++)
			ar << m_emblemEffects[i];
		  ar << m_maxHP;
	}
	// Read item from storage
	else
	{
	   int version = ar.GetObjectSchema();
	   
	   ar >> m_itemName
		  >> m_marketCost
		  >> m_requiredDex
		  >> m_requiredStr
		  >> m_requiredKnowledge
		  >> m_attachmentRefList
		  >> m_equiped
		  >> m_locationID
		  >> m_disarmable
		  >> m_globalID
		  >> m_animAccessValue
		  >> m_permanent
		  >> m_useType
		  >> m_miscUseValue
		  >> m_armourClass
		  >> m_raceID
		  >> m_nonDrop
		  >> m_dbSrcIndex
		  >> m_statBonuses
		  >> m_nonTransferable		// updated from here down
		  >> m_destroyOnUse
		  >> m_requiredSkillType
		  >> m_requiredSkillLevel
		  >> m_stackable
		  >> m_timoutValue
		  >> m_elementDefenses
		  >> m_chargeType
		  >> m_chargeCurrent
		  >> m_maxCharges
		  >> m_keyID
		  >> m_expansiveCollection	// migrated
		  >> m_defensesSpecific;	// migrated

		m_hp					= 0;
		m_maxHP					= 2000;
		m_destroyOnBreak		= false;
		m_visualRef				= 0;
		m_animAccessDefault		= 0;
		m_reloadTime			= 0;
		m_damageModifier		= 0;
		m_bonusDuration			= 0;
		m_damageMultiplier		= 1.0f;
		m_bonusType				= 0;
		m_abilityType			= 0;
		m_explosionRef			= -1;
		m_missileLoaded			= 0;
		m_shotsPerFire			= 1;
		m_tempDepletionHolder   = 0;
		m_weaponDepletionTimer  = 0;
		m_secsToGo				= 0;
		m_ammoRegenRate			= 1;
		m_ammoSecTick			= 3;
		
		m_dynDamageOvrd			= -1;//alters bottome line damage on server damage handler
		m_dynDamageElementOvrd	= -1;//this alters the channel on the fly during the damage handler
		m_dynDamageFireQueOvrd  = -1;//alters fire que on server fire effect distribution and local client

		for(int i = 0; i < (int)MAX_EMBLEM; i++)
			m_emblemEffects[i] = 0;

		// Item retrieval is contingent on current version
		switch(version)
		{
			case 2:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce;

				m_dimCfgIndex          = -1;
				m_dimCfgChangeTo       = -1;
				m_dimCfgChangeBack     = -1;
				break;
			case 3:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime;
				break;
			case 4:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType;
				break;
			case 5:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType
				   >> m_abilityType;
				break;
			case 6:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType
				   >> m_abilityType
				   >> m_unarmedAttack;
				break;
			case 7:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType
				   >> m_abilityType
				   >> m_unarmedAttack
				   >> m_explosionRef;
				break;
			case 8:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType
				   >> m_abilityType
				   >> m_unarmedAttack
				   >> m_explosionRef
				   >> m_missileLoaded;
				break;
			    case 9:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType
				   >> m_abilityType
				   >> m_unarmedAttack
				   >> m_explosionRef
				   >> m_missileLoaded
				   >> m_shotsPerFire
				   >> m_ammoRegenRate
				   >> m_ammoSecTick;
				break;
				case 10:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType
				   >> m_abilityType
				   >> m_unarmedAttack
				   >> m_explosionRef
				   >> m_missileLoaded
				   >> m_shotsPerFire
				   >> m_ammoRegenRate
				   >> m_ammoSecTick
				   >> m_animAccessDefault;
				break;
				case 11:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType
				   >> m_abilityType
				   >> m_unarmedAttack
				   >> m_explosionRef
				   >> m_missileLoaded
				   >> m_shotsPerFire
				   >> m_ammoRegenRate
				   >> m_ammoSecTick
				   >> m_animAccessDefault;
				for(int i = 0; i < (int)MAX_EMBLEM; i++)
					ar >> m_emblemEffects[i];
				break;
				case 12:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType
				   >> m_abilityType
				   >> m_unarmedAttack
				   >> m_explosionRef
				   >> m_missileLoaded
				   >> m_shotsPerFire
				   >> m_ammoRegenRate
				   >> m_ammoSecTick
				   >> m_animAccessDefault
				   >> m_hp
				   >> m_destroyOnBreak;
				for(int i = 0; i < (int)MAX_EMBLEM; i++)
					ar >> m_emblemEffects[i];
				break;
				case 13:
				ar >> m_visualRef
				   >> m_yJetforce
				   >> m_zJetforce
				   >> m_dimCfgIndex
				   >> m_dimCfgChangeTo
				   >> m_dimCfgChangeBack
				   >> m_damageModifier
				   >> m_reloadTime
				   >> m_bonusDuration
				   >> m_damageMultiplier
				   >> m_bonusType
				   >> m_abilityType
				   >> m_unarmedAttack
				   >> m_explosionRef
				   >> m_missileLoaded
				   >> m_shotsPerFire
				   >> m_ammoRegenRate
				   >> m_ammoSecTick
				   >> m_animAccessDefault
				   >> m_hp
				   >> m_destroyOnBreak;
				for(int i = 0; i < (int)MAX_EMBLEM; i++)
					ar >> m_emblemEffects[i];
				   ar >> m_maxHP;
				break;
		}

		m_skillUseRef          = -1;
		m_minimumLevel         = 0;
		m_masteredLevel        = 0;
		m_creationTimeStamp    = 0; 
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObjList
// Function:	SafeDelete()
// Description:	Safely delete a linked list of CItemObj nodes.
///////////////////////////////////////////////////////////////////////////////

void 
CItemObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CItemObj *spnPtr = (CItemObj*)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr = 0;
	}
	RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObjList
// Function:	Clone(CItemObjList**)
// Description:	Perform a deep copy of all items from one CItemObjList to
//				another.
///////////////////////////////////////////////////////////////////////////////

void 
CItemObjList::Clone(CItemObjList **clone)
{
	CItemObjList *newList = new CItemObjList();
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CItemObj *bnPtr = (CItemObj*)GetNext(posLoc);
		CItemObj *newObject = NULL;  
		bnPtr->Clone(&newObject);
		newList->AddTail(newObject);
	}
	*clone = newList;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObjList
// Function:	GetByGLID(int)
// Description:	Return an item from a list of items by unique ID.
///////////////////////////////////////////////////////////////////////////////

CItemObj* 
CItemObjList::GetByGLID(int GLID)
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CItemObj *bnPtr = (CItemObj*)GetNext(posLoc);
		if(bnPtr->m_globalID == GLID)
		   return bnPtr;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CStatBonusObj
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CStatBonusObj::CStatBonusObj()
{
	m_statID = 0;
	m_statBonus = 0;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CStatBonusObj
// Function:	Clone(CStatBonusObj**)
// Description:	Perform a deep copy of one CStatBonusObj's properties to another.
///////////////////////////////////////////////////////////////////////////////

void 
CStatBonusObj::Clone(CStatBonusObj **clone)
{
	CStatBonusObj *copyObj = new CStatBonusObj();
	copyObj->m_statID = m_statID;
	copyObj->m_statBonus = m_statBonus;
	*clone = copyObj;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CStatBonusObj
// Function:	Serialize(CArchive &)
// Description:	Read/Write a CStatBonusObj to/from storage.
///////////////////////////////////////////////////////////////////////////////

void 
CStatBonusObj::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring())
	{	
	   ar << m_statID
		  << m_statBonus;
	}
	else
	{
       ar >> m_statID
		  >> m_statBonus;
	}
}     

///////////////////////////////////////////////////////////////////////////////
// Class:		CStatBonusList
// Function:	GetBonusByID(int)
// Description:	Retrieve a stat bonus modifier by unique ID.
///////////////////////////////////////////////////////////////////////////////

int 
CStatBonusList::GetBonusByID(int id)
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{ 
		CStatBonusObj *bPtr = (CStatBonusObj*)GetNext(posLoc);
		if(bPtr->m_statID == id)
			return bPtr->m_statBonus;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CStatBonusList
// Function:	SafeDelete()
// Description:	Safely delete all CStatBonusObj nodes from a list.
///////////////////////////////////////////////////////////////////////////////

void 
CStatBonusList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CStatBonusObj *bPtr = (CStatBonusObj*)GetNext(posLoc);
		delete bPtr;
		bPtr = 0;
	}
	RemoveAll();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CStatBonusList
// Function:	Clone(CStatBonusList**)
// Description:	Perform a deep copy of all CStatBonusObj nodes in a list to
//				another CStatBonusList.
///////////////////////////////////////////////////////////////////////////////

void 
CStatBonusList::Clone(CStatBonusList **clone)
{
	CStatBonusList *copyLocal = new CStatBonusList();
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CStatBonusObj *bnPtr = (CStatBonusObj*)GetNext(posLoc);
		CStatBonusObj *newObject = NULL;  
		bnPtr->Clone(&newObject);
		copyLocal->AddTail(newObject);
	}
	*clone = copyLocal;
}

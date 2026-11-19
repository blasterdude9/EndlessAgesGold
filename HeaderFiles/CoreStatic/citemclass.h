#ifndef CITEMCLASS_H
#define CITEMCLASS_H

#include "CElementStatusClass.h"
#include "CItemExpanseObj.h"
#include <map>
#include "Common.h"


//#define MAX_EMBLEM_EFFECT 5

///////////////////////////////////////////////////////////////////////////////
// Enum:		Emblems
// Description:	Represents a modifier that adjust player stats.
//	P## indicates PLUS. M## indicates MINUS. Used as indexers into
//	CItemObj::m_emblemEffects[].
///////////////////////////////////////////////////////////////////////////////

enum EmblemEffects : unsigned char
{
	HEALTH_P10 = 0,
	HEALTH_P20,
	RESIST_P01,
	DAMAGE_TAKEN_M05,
	AMMO_REGEN_P02,
	MAX_EMBLEM
};

struct Emblem
{
	CString Name;
	int GLID;
	int VisualID;

	Emblem(CString strName, int iGLID, int iVisualID)
	{
		Name = strName;
		GLID = iGLID;
		VisualID = iVisualID;
	}
};


typedef std::pair<int, Emblem*> EMBLEM_ID_PAIR;

const Emblem bountyHunter("Bounty Hunter", 218, 514);
const Emblem chiefExplorer("Chief Explorer", 217, 515);
const Emblem elitewrecker("Elite Wrecker", 501, 516);
const Emblem emissary("Emissary", 39, 517);
const Emblem extractionengineer("Extraction Engineer", 502, 518);
const Emblem fabricationexpert("Fabrication Expert", 504, 519);
const Emblem geneticzoologist("Genetic Zoologist", 583, 520);
const Emblem honorableservice("Honorable Service", 503, 521);
const Emblem medic("Medic", 584, 522);
const Emblem officialsurveyor("Official Surveyor", 499, 523);
const Emblem signaloperator("Signal Operator", 500, 524);
const Emblem nexusonesecurityspecops("Nexus One Security Spec Ops", 651, 525);
const Emblem exgspy("EXG Spy", 690, 526);
const Emblem txbbioEngineer("TXB Bio-Engineer", 691, 527);
const Emblem bceironchef("BCE Iron Chef", 692, 528);
const Emblem terranloyalist("Terran Loyalist", 693, 529);
const Emblem amphibianhonorguard("Amphibian Honor Guard", 694, 530);
const Emblem assault("Assault", 695, 531);
const Emblem mechanic("Mechanic", 696, 532);
const Emblem scientificdiscovery("Scientific Discovery", 697, 533);
const Emblem testpilot("Test Pilot", 698, 534);
const Emblem ampassault("A.M.P. Assault", 699, 535);
const Emblem inventor("Inventor", 700, 536);
const Emblem hacker("Hacker", 701, 537);
const Emblem survivalist("Survivalist", 702, 538);
const Emblem outlaws("Outlaws", 703, 539);
const Emblem conqueror("Conqueror", 704, 540);

const EMBLEM_ID_PAIR EmblemIdPairs[BADGE_TRACKING] =
{
	EMBLEM_ID_PAIR(218, (Emblem*)&bountyHunter),
	EMBLEM_ID_PAIR(217, (Emblem*)&chiefExplorer),
	EMBLEM_ID_PAIR(501, (Emblem*)&elitewrecker),
	EMBLEM_ID_PAIR(39, (Emblem*)&emissary),
	EMBLEM_ID_PAIR(502, (Emblem*)&extractionengineer),
	EMBLEM_ID_PAIR(504, (Emblem*)&fabricationexpert),
	EMBLEM_ID_PAIR(583, (Emblem*)&geneticzoologist),
	EMBLEM_ID_PAIR(503, (Emblem*)&honorableservice),
	EMBLEM_ID_PAIR(584, (Emblem*)&medic),
	EMBLEM_ID_PAIR(499, (Emblem*)&officialsurveyor),
	EMBLEM_ID_PAIR(500, (Emblem*)&signaloperator),
	EMBLEM_ID_PAIR(651, (Emblem*)&nexusonesecurityspecops),
	EMBLEM_ID_PAIR(690, (Emblem*)&exgspy),
	EMBLEM_ID_PAIR(691, (Emblem*)&txbbioEngineer),
	EMBLEM_ID_PAIR(692, (Emblem*)&bceironchef),
	EMBLEM_ID_PAIR(693, (Emblem*)&terranloyalist),
	EMBLEM_ID_PAIR(694, (Emblem*)&amphibianhonorguard),
	EMBLEM_ID_PAIR(695, (Emblem*)&assault),
	EMBLEM_ID_PAIR(696, (Emblem*)&mechanic),
	EMBLEM_ID_PAIR(697, (Emblem*)&scientificdiscovery),
	EMBLEM_ID_PAIR(698, (Emblem*)&testpilot),
	EMBLEM_ID_PAIR(699, (Emblem*)&ampassault),
	EMBLEM_ID_PAIR(700, (Emblem*)&inventor),
	EMBLEM_ID_PAIR(701, (Emblem*)&hacker),
	EMBLEM_ID_PAIR(702, (Emblem*)&survivalist),
	EMBLEM_ID_PAIR(703, (Emblem*)&outlaws),
	EMBLEM_ID_PAIR(704, (Emblem*)&conqueror)

	//BELOW LEAKS!!!
	//EMBLEM_ID_PAIR(218, new Emblem("Bounty Hunter", 218, 514)),
	/*EMBLEM_ID_PAIR(217, new Emblem("Chief Explorer", 217, 515)),
	EMBLEM_ID_PAIR(501, new Emblem("Elite Wrecker", 501, 516)),
	EMBLEM_ID_PAIR(39, new Emblem("Emissary", 39, 517)),
	EMBLEM_ID_PAIR(502, new Emblem("Extraction Engineer", 502, 518)),
	EMBLEM_ID_PAIR(504, new Emblem("Fabrication Expert", 504, 519)),
	EMBLEM_ID_PAIR(583, new Emblem("Genetic Zoologist", 583, 520)),
	EMBLEM_ID_PAIR(503, new Emblem("Honorable Service", 503, 521)),
	EMBLEM_ID_PAIR(584, new Emblem("Medic", 584, 522)),
	EMBLEM_ID_PAIR(499, new Emblem("Official Surveyor", 499, 523)),
	EMBLEM_ID_PAIR(500, new Emblem("Signal Operator", 500, 524)),
	EMBLEM_ID_PAIR(651, new Emblem("Nexus One Security Spec Ops", 651, 525)),
	EMBLEM_ID_PAIR(690, new Emblem("EXG Spy", 690, 526)),
	EMBLEM_ID_PAIR(691, new Emblem("TXB Bio-Engineer", 691, 527)),
	EMBLEM_ID_PAIR(692, new Emblem("BCE Iron Chef", 692, 528)),
	EMBLEM_ID_PAIR(693, new Emblem("Terran Loyalist", 693, 529)),
	EMBLEM_ID_PAIR(694, new Emblem("Amphibian Honor Guard", 694, 530)),
	EMBLEM_ID_PAIR(695, new Emblem("Assault", 695, 531)),
	EMBLEM_ID_PAIR(696, new Emblem("Mechanic", 696, 532)),
	EMBLEM_ID_PAIR(697, new Emblem("Scientific Discovery", 697, 533)),
	EMBLEM_ID_PAIR(698, new Emblem("Test Pilot", 698, 534)),
	EMBLEM_ID_PAIR(699, new Emblem("A.M.P. Assault", 699, 535)),
	EMBLEM_ID_PAIR(700, new Emblem("Inventor", 700, 536)),
	EMBLEM_ID_PAIR(701, new Emblem("Hacker", 701, 537)),
	EMBLEM_ID_PAIR(702, new Emblem("Survivalist", 702, 538)),
	EMBLEM_ID_PAIR(703, new Emblem("Outlaws", 703, 539)),
	EMBLEM_ID_PAIR(704, new Emblem("Conqueror", 704, 540))//*/
};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Member:		EmblemMap
// Description:	The hashmap containers for emblem pairs.
///////////////////////////////////////////////////////////////////////////////

const std::map<int, Emblem*> 
EmblemMap(EmblemIdPairs, EmblemIdPairs + sizeof(EmblemIdPairs) / sizeof(EmblemIdPairs[0]));

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Member:		ELEMENT_DEFENSES
///////////////////////////////////////////////////////////////////////////////

const enum ELEMENT_DEFENSES
{
	ED_PLASMA = 0,
	ED_THERMAL,
	ED_HYDRO,
	ED_ELECTRIC,
	ED_METHANE,
	ED_CORRODE,
	ED_PHYSICAL,
	ED_ETHEREAL,
	ED_MAX
};

typedef std::pair<int, CString> ELEMENT_DEFENSE_PAIR;
const ELEMENT_DEFENSE_PAIR ElementDefensePairs[ED_MAX] =
{
	ELEMENT_DEFENSE_PAIR(ED_PLASMA, "Plasma"),
	ELEMENT_DEFENSE_PAIR(ED_THERMAL, "Thermal"),
	ELEMENT_DEFENSE_PAIR(ED_HYDRO, "Hydro"),
	ELEMENT_DEFENSE_PAIR(ED_ELECTRIC, "Electric"),
	ELEMENT_DEFENSE_PAIR(ED_METHANE, "Methane"),
	ELEMENT_DEFENSE_PAIR(ED_CORRODE, "Corrode"),
	ELEMENT_DEFENSE_PAIR(ED_PHYSICAL, "Physical"),
	ELEMENT_DEFENSE_PAIR(ED_ETHEREAL, "Ethereal")
};

const std::map<int, CString> 
ElementDefenseMap(ElementDefensePairs, ElementDefensePairs + sizeof(ElementDefensePairs) / sizeof(ElementDefensePairs[0]));

///////////////////////////////////////////////////////////////////////////////
// Class:		CStatBonusObj
// Description:	Represents a modifier that adjust player stats up/down.
///////////////////////////////////////////////////////////////////////////////

class CStatBonusObj : public CObject
{    
public:
	int     m_statID;
	int     m_statBonus;

    CStatBonusObj();	
	void Serialize( CArchive& ar );
	void Clone(CStatBonusObj **clone);
	DECLARE_SERIAL( CStatBonusObj );	
};

///////////////////////////////////////////////////////////////////////////////
// Class:		CStatBonusList
// Description:	A linked list of CStatBonusObj.
///////////////////////////////////////////////////////////////////////////////

class CStatBonusList : public CObList
{
public:	
   CStatBonusList() {};   
   void		SafeDelete();
   int		GetBonusByID(int id);
   void		Clone(CStatBonusList **clone);
   DECLARE_SERIAL( CStatBonusList );
};

///////////////////////////////////////////////////////////////////////////////
// Class:		CAttachmentRefObj
// Description:	Represents a single node in a linked list of attachment objects.
///////////////////////////////////////////////////////////////////////////////

class CAttachmentRefObj : public CObject
{	
public:
	int		m_attachmentType;
	int		m_raceBind;			//	-1 = none
	int     m_dimIndex;			//	-1 = none
    int     m_dimSwitchTo;		//	-1 = none
	int		m_fireQueOvrd;		//	0=none

    CAttachmentRefObj();
	void	Serialize(CArchive &ar);
	DECLARE_SERIAL(CAttachmentRefObj);
};

///////////////////////////////////////////////////////////////////////////////
// Class:		CAttachmentRefList
// Description:	A linked list of attachment objects.
///////////////////////////////////////////////////////////////////////////////

class CAttachmentRefList : public CObList
{
public:
	CAttachmentRefList() {};
   void		SafeDelete();
   void		Clone(CAttachmentRefList **clone);
   DECLARE_SERIAL(CAttachmentRefList);
};

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObj
// Description:	Common properties for describing a generic game item.
///////////////////////////////////////////////////////////////////////////////

class CItemObj : public CObject
{
public:
	bool					m_isInQuickSlots;
	CString					m_itemName;
	CAttachmentRefList		*m_attachmentRefList;
	int						m_marketCost;
	int						m_requiredDex;
	int						m_requiredStr;
	int						m_requiredKnowledge;
	BOOL					m_equiped;
	int						m_locationID;
	BOOL					m_disarmable;
	BOOL					m_unarmedAttack;
	int						m_globalID;
	int						m_useType;
	int						m_miscUseValue;
	int						m_armourClass;

	// Charges
    int						m_chargeType;		// Specify the missile index
	int						m_chargeCurrent;	// Current charges
    int						m_maxCharges;		// Maximum charges on item
	char					m_missileLoaded;	// What missile we are to fire

	int						m_skillUseRef;
	int						m_minimumLevel;
	int						m_masteredLevel;

	int						m_animAccessValue;
	int						m_animAccessDefault;

	BOOL					m_permanent;
	BOOL					m_raceID;			// Any race can arm it (race id == 0) other must match

	int						m_dbSrcIndex;
    BOOL					m_nonDrop;
	BOOL					m_nonTransferable;
	BOOL					m_destroyOnUse;

	int						m_keyID;
	BOOL					m_stackable;

	int						m_defensesSpecific;	// Reference to missile DB

	int						m_hp;//0=infinite 1=broken >1=valid with energy
	int						m_maxHP;//Needed in order to repair items
	bool					m_destroyOnBreak;

    // Additional skills 
	int						m_requiredSkillType;
	int						m_requiredSkillLevel;

	long					m_creationTimeStamp;
	int						m_timoutValue;
	//int                   m_setCost;

	int						m_visualRef;
	float					m_yJetforce;
	float					m_zJetforce;

	// Dim cfg control
	char					m_dimCfgIndex;
	char					m_dimCfgChangeTo;
	char					m_dimCfgChangeBack;

	int						m_reloadTime;
	char					m_damageModifier;

	int						m_bonusDuration;
	float					m_damageMultiplier;
	int						m_bonusType;

	int						m_abilityType;		// Ability type
	int						m_explosionRef;		// Melee explosion hits

	//Ammo & Gun features
	int						m_shotsPerFire;
	int						m_ammoRegenRate;
	int						m_ammoSecTick;

	//overrides
	int						m_dynDamageOvrd;//alters bottome line damage on server damage handler
	int						m_dynDamageElementOvrd;//this alters the channel on the fly during the damage handler
	int						m_dynDamageFireQueOvrd;//alters fire que on server fire effect distribution and local client
	DWORD					m_tempDepletionHolder;//overheat type of ability on weapons
	DWORD					m_weaponDepletionTimer;
	int						m_secsToGo;


	
	//Emblem effects
	char					m_emblemEffects[MAX_EMBLEM];
	//char					m_emblemEffects[MAX_EMBLEM_EFFECT]; 
	/* 0 - Health +10			1 - Health +20			 2 - Resist +1
	   3 - DamageTaken -5		4 - AmmoRegen +2
	*/

	CStatBonusList			*m_statBonuses;
	CElementStatusList		*m_elementDefenses;
	CCExpansiveObjList		*m_expansiveCollection;

    CItemObj();
	void					SafeDelete();
    void					SerializeAlloc(CArchive &ar);
	void					Serialize(CArchive &ar);
    void					Clone(CItemObj **clone);
	DECLARE_SERIAL(CItemObj);
};

///////////////////////////////////////////////////////////////////////////////
// Class:		CItemObjList
// Description:	A linked list of generic game items.
///////////////////////////////////////////////////////////////////////////////

class CItemObjList : public CObList
{
public:
   CItemObjList(){};   
   void			SafeDelete();
   CItemObj*	GetByGLID(int GLID);
   void			Clone(CItemObjList **clone);
   DECLARE_SERIAL( CItemObjList );
};

#endif

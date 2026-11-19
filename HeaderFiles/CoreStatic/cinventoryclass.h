#ifndef CINVENTORYCLASS_H
#define CINVENTORYCLASS_H

#include "CStructuresMisl.h"
#include "CItemClass.h"
#include <vector>

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObj
// Description:	This class wraps properties and routines related to a single
//				inventory object for a player.
///////////////////////////////////////////////////////////////////////////////

class CInventoryObj : public CObject
{	
public:
	BOOL			m_armed,					// Quanity must be 1 for armed
					m_optimizedSave;
	int				m_quanity;
	long			m_tempUniqueSync;			// Used for relationship items combinations in sync for database use
	CItemObj		*m_itemData;

	CInventoryObj();
	void			Clone(CInventoryObj **copy);
	void			CloneMinimum(CInventoryObj **copy);
	void			Serialize(CArchive &ar);	// Visual rep needs to be added
	void			SerializeAlloc(CArchive &ar);
	BOOL			PhysicChanging();
	BOOL			ApplyWear(int wearAmount);
	void			SafeDelete();
	DECLARE_SERIAL(CInventoryObj);
};

///////////////////////////////////////////////////////////////////////////////
// Class:		CInventoryObjList
// Description:	A linked list of CInventoryObj nodes.
///////////////////////////////////////////////////////////////////////////////

class CInventoryObjList : public CObList
{
public:
	int	m_wieghtLimit,
		m_totalWieght,
		m_itemCountLimit,
		m_itemTotal,
		m_lastArmedWeapon;

	DECLARE_SERIAL(CInventoryObjList);	

	CInventoryObjList();
	~CInventoryObjList() { this->SafeDelete(); }
	void				SafeDelete();
	void				Serialize(CArchive &ar);
	void				Clone(CInventoryObjList **copy);
	void				CloneMinimum(CInventoryObjList **copy);
	CInventoryObj		*AddInventoryItem(	int quanity,
											BOOL armed,
											CItemObj *itemData);
	BOOL				DeleteByGLIDUnarmedOnly(int GLID);
	CInventoryObj		*VerifyInventory(int GL_ID);
	CInventoryObj		*GetByGLID(int glID);
	BOOL				DeleteByGLID(int GLID);
	BOOL				DeleteByGLIDBroken(int GLID, int quanity);
	BOOL				DeleteByGLID(int GLID,int quanity);
	BOOL				DeleteByGLIDUnarmedOnlyWQuanity(int GLID, int quanity);
	int					GetArmourClass(BOOL forVisualOnly);
	int					GetArmourRating(int damageChannel, BOOL forVisualOnly, int attackSpecified);
	int					GetEmblemResistBonus(int armourValue);
	float				GetEmblemDamageBonus(float damage);
	int					GetEmblemAmmoBonus();
	int					GetEmblemHealthBonus();
	int					GetEmblemReductionBonus();
	BOOL				GenerateInventoryDataBlock(	int includeArmed,
													BYTE **dataGLIDBlock,
													int &blockSize,
													int &datasize,
													BOOL filterNonTransferable,
													int headerSize,
													int filter);
	BOOL				GenerateBadgeDataBlock(BYTE **dataGLIDBlock, int &blockSize, int &datasize);
	BOOL				DeleteByGLIDUnarmedOnlyWQuanity(int GLID, int quanity, int damageMod);
	BOOL				IsItemOfValuePresent();
	int					GetInvenCountFilterUnseeable();
	BOOL				DoesArmedLocationIDExist(int id);
	CInventoryObj		*GetArmedInvenItemByLocationID(int id);
	BOOL				UseIfKeyInInventory(int keyID);
	BOOL				IsAnythingInInventoryValuable();
	BOOL				IsExpItemObjectPresent(int glid,int expID);
	int					CombineToExpObj(int glid_combineItem,int glid_targetObject);
	CInventoryObj		*GetAndRemoveByGlid(int GL_ID);
	CInventoryObj		*GetAndRemoveAmountByGlid(int GL_ID, int amount);
	BOOL				CanTakeObjects(int maxCount,int itemCountToAdd);
	int					GetTotalValue();
	CInventoryObj		*VerifyInventory(int GL_ID, int quanity);
	CInventoryObj		*VerifyInventoryUnarmed(int GL_ID, int quanity);
	CInventoryObj		*VerifyInventoryTransferable(int GL_ID, int quanity);
	void				RemoveAllExceptPerminent();
	void				SerializeAlloc(CArchive& ar,int loadCount);
	CInventoryObj		*GetInvenItemByLocationID(int id);
	CInventoryObj		*GetInvenItemByLocationIDArmable(int id, float rStr, float rDex, float rWis);
	BOOL				ReorderInventory(int objectToMoveIndex, int targetIndex, int filterType);
	BOOL				DeleteByName(CString itemName);
	CInventoryObj		*GetByUniqueSync(long id);
	int					Combine(CItemObj *itemToCombine, CItemObj *targetItemPtr);
	BOOL				DeleteByGLIDX(int GLID, int quanity);
	BOOL				DeleteByGLIDAll(int GLID);
	void				RemoveAllExceptPerminentNONdrop();
	int					GetStatBenefit(int statID);
	BOOL				DeleteByLOCIDAll(int locationID);
	BOOL				PhysicChanging();
	void				OptimizeBestConfig();
	CInventoryObj		*VerifyInventoryDUPH(int GL_ID, int dupCount);
	BOOL				DeleteByGLID_DPTRACE(int GLID, int dupTrace);
	BOOL				DeleteByGLID_DPTRACE(int GLID, int dupTrace, int quanity);
	CInventoryObj		*VerifyInventoryDUPH(int GL_ID, int dupCount, int quanity);
	CInventoryObj		*VerifyInventoryUnarmed(int GL_ID, int quanity, int dmod);
	bool				VerifyItemIsArmed(CString itemName);
	bool				VerifyItemIsArmed(int iGLID);
	bool				VerifyPlayerHasItem(CString itemName);
	bool				VerifyPlayerHasItem(int iGLID);
	CInventoryObj		*GetUnarmedWeapon();
	BOOL				CheckForSpecifiedDamage(int specified);
	BOOL				ApplyWearSpecific(int locationID,CInventoryObj ** brokenRef,int wearAmount);
	BOOL				ApplyWearOverAll(CInventoryObj ** brokenRef,int wearAmount);
	BOOL				GeneratePartialInventoryDataBlock(std::vector<short> &indices,
							BLOCKITEMLIST_STRUCT header,
							BYTE **pInventoryData,
							int &iSize);

   /* DEPRECATED //////////////////////////////////////////////////////////////
   int			GetNextWeaponToArm(CSkillObjectList * skills,CStatObjList * stats);
   int			GetPrevWeaponToArm(CSkillObjectList * skills,CStatObjList * stats);
   int          AddInventoryItem(int  GLID,
							     int  quanity,
								 BOOL armed,
							     BOOL disarmable,
								 int  locationID,
								 BOOL permanent,
								 int  armourClass,
								 BOOL nonDrop);
	/////////////////////////////////////////////////////////////////////////*/
};

#endif
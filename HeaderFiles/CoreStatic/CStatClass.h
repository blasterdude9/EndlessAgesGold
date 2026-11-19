#include "matrixarb.h"
#include "CInventoryClass.h"
#ifndef CSTATOBJCLASS_H
#define CSTATOBJCLASS_H

class   CStatObj : public CObject
{
	
     
	DECLARE_SERIAL( CStatObj );
	
    
public:
    CStatObj();
    MatrixARB                  GL_matrixFunctions;

	CString              m_statName;
	float                m_capValue;
	float                m_currentValue;
	int                  m_id;
	float                m_gainBasis;
	int                  m_benefitType;//0 = no benefit  1=health benefit  2=speed benefit
	

	float  GetValueIncludingBonusItems(CInventoryObjList * inventory);
     BOOL UseStat(float useLevel,CInventoryObjList * inventory);
	void   SerializeAlloc(CArchive& ar);
	void   Clone(CStatObj ** clone);
  
	void Serialize( CArchive& ar );
	
};

class   CStatObjList : public CObList
{
public:
   CStatObjList(){};
   void SafeDelete();

    
     
	DECLARE_SERIAL( CStatObjList );
	
    


	float GetStatValueByID(int id,CInventoryObjList * inventory);
	BOOL  UseStatByID(float useLevel,int id,CInventoryObjList * inventory);
	void  Clone(CStatObjList ** clone);
	int   GetBenefitBonus(int benefit,CInventoryObjList * inventory);
	BOOL  VerifyStatRequirement(float minimumValue,int id,CInventoryObjList * inventory);
	int   GetBonusByID(int id);
	int	GetStatIdByName(CString name);
	CStatObj * GetStatByID(int id);
	void SerializeAlloc(CArchive& ar,int loadCount);
};

#endif
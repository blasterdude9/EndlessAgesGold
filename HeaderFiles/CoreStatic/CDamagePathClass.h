#ifndef CDAMAGEPATH_H
#define CDAMAGEPATH_H

class  CDamagePathObj : public CObject
{
	
     
    DECLARE_SERIAL( CDamagePathObj )
    
    
public:
	
	CDamagePathObj();
	
    //functions
    int          m_boneIndex;
	D3DVECTOR    m_lastPosition;
	float        m_radius;
	int          m_enableOnAnimationIndex;
	int          m_damageMinValue;
	int          m_damageMaxValue;
	int          m_damageDotValue;
	BOOL         m_contactMade;
	int          m_damageDelay;//minimum wait on redamaging in swing
	long         m_damageDelayStamp;//last hit
	int          m_damageChannel;
	int          m_criteriaForSkillUse;//skill type
	int          m_damageSpecific;

	void Clone(CDamagePathObj ** clone);
	void Serialize( CArchive& ar );
};

class  CDamagePathObjList : public CObList
{
public:
   CDamagePathObjList(){};
   void Clone(CDamagePathObjList ** clone);
   void SafeDelete();
    
     
    DECLARE_SERIAL( CDamagePathObjList )
    
    
};

#endif
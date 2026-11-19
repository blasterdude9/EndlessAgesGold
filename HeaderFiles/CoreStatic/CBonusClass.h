//CBonusClass.h
///////////////////////////////////

#include "CItemClass.h"

#ifndef _CBONUSCLASS_H_
#define _CBONUSCLASS_H_

class CBonus : public CObject
{
		DECLARE_SERIAL(CBonus);

	public:
		CBonus();
		
		int		m_bonusType;			//What type of bonus are we (stat, dmg, resist)
		float	m_damageMult;			//How much more damage are we doing
		int		m_bonusDuration;		//How long should this bonus last for
		int		m_bonusAmount;			//What is the amount the bonus adds
		int		m_maxDuration;			//How much time the duration has to begin with

		//Enumerating the bonus types
		enum { NONE = 0, STAT_STR, STAT_DEX, STAT_WIS, STAT_ALL, DAMAGE,
			   RESIST_NORM, RESIST_FIRE, RESIST_WATER, RESIST_RUNE, 
			   RESIST_RELIC, RESIST_EARTH, RESIST_AIR, RESIST_DEATH, RESIST_ALL };

		//Functions
		void Clone(CBonus * clone);
		void Serialize(CArchive &ar);
		void SerializeAlloc(CArchive &ar);
};

#endif
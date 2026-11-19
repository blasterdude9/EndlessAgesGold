#include "stdafx.h"

#include <afxtempl.h>
#include "CBonusClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////
//
//CBonus Default Copy Constructor
//Sets up based Bonus to NULL
//
//////////////////////////////////////////
CBonus::CBonus()
{
	m_bonusType			= 0;
	m_damageMult		= 0;
	m_bonusDuration		= 0;
	m_bonusAmount		= 0;
	m_maxDuration		= 0;
}

//////////////////////////////////////////
//
//CBonus Clone Function
//Creates a copy of the Bonus object
//
//////////////////////////////////////////
void CBonus::Clone(CBonus * clone)
{
	CBonus *copyObj			= new CBonus();
	
	copyObj->m_bonusType		= m_bonusType;
	copyObj->m_damageMult		= m_damageMult;
	copyObj->m_bonusDuration	= m_bonusDuration;
	copyObj->m_bonusAmount		= m_bonusAmount;
	copyObj->m_maxDuration		= m_maxDuration;

	clone = copyObj;    
}

//////////////////////////////////////////
//
//CBonus Serialize function
//Saves and loads the Bonuses of a player
//
//////////////////////////////////////////
void CBonus::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);

	if(ar.IsStoring())
	{
		ar  << m_bonusType
			<< m_damageMult
			<< m_bonusDuration
			<< m_bonusAmount
			<< m_maxDuration;
	}

	else
	{
		ar	>> m_bonusType
			>> m_damageMult
			>> m_bonusDuration
			>> m_bonusAmount
			>> m_maxDuration;
	}
}

//////////////////////////////////////////
//
//CBonus SerializeAlloc Function
//Make sure we have enough memory to save out
//   correctly
//
//////////////////////////////////////////
void CBonus::SerializeAlloc(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar  << m_bonusType
			<< m_damageMult
			<< m_bonusDuration
			<< m_bonusAmount
			<< m_maxDuration;
	}

	else
	{
		ar	>> m_bonusType
			>> m_damageMult
			>> m_bonusDuration
			>> m_bonusAmount
			>> m_maxDuration;
	}	
}

IMPLEMENT_SERIAL(CBonus, CObject, 0);
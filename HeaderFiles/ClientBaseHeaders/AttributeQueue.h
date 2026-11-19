#pragma once

#include "CMenuVisual.h"

class CAttribQueueEntry : public CObject
{
public:
	int			 m_requestedAttrib;
	CMenuVisual	*m_requester;

	int		m_miscInt1;
	int		m_miscInt2;
	int		m_miscInt3;
	int		m_miscInt4;
	float	m_miscFloat;
	CString	m_miscString;
};

class CAttribQueue : public CObList	
{
};
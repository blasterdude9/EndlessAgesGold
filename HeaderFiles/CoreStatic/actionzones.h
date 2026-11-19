#include "matrixarb.h"

#pragma once


class CActionBox : public CObject
{
public:
	DECLARE_SERIAL( CActionBox )
	CActionBox()
	{
	   m_action			= 0;
	   m_actionValue	= 0;
	   m_actionAttrib   = -1;
	   ZeroMemory(&boxMin,sizeof(D3DVECTOR));
	   ZeroMemory(&boxMax,sizeof(D3DVECTOR));
	};
	~CActionBox(){};
  
    //variables
	CString m_zoneLabel;
	int m_action;//0=none 1=damage 2=safe zone 3=safe zone w rebirth
	int m_actionValue;//valid for damage zones
	int m_actionAttrib;
	D3DVECTOR boxMin, boxMax;

	//functions
	void Serialize( CArchive& ar );
};


class CActionZone : public CObject
{
public:
	DECLARE_SERIAL( CActionZone )
	CActionZone(void){};
	~CActionZone(void)
	{
	    ClearAll();
	};

	CObList m_boxList;

	//functions
	void Serialize( CArchive& ar );
	void ClearAll();
	void AddBox(D3DVECTOR min, 
				D3DVECTOR max, 
				int action, 
				int actionValue, 
				int attrib,
				CString label);
	bool DeleteBox(int index);
	bool EditBox(   int index,
					D3DVECTOR min, 
					D3DVECTOR max, 
					int action, 
					int actionValue, 
					int attrib,
					CString label);
};

struct DirectAccessStructAZ
{
    CActionZone * object;
};

struct ActionListStructure
{
    int actionReturnCount;
	int actionArray[10];
	int actionAmountArray[10];
	int attribArray[10];
	int maxActions;
};

class CActionZoneDB : public CObList
{
public:
	DECLARE_SERIAL( CActionZoneDB )
	CActionZoneDB(){m_directAccess=0;m_directAccessObjectCount=0;};
	~CActionZoneDB(void)
	{
		ClearAll();
	}

	DirectAccessStructAZ *  m_directAccess;
    int                     m_directAccessObjectCount;
	MatrixARB               m_matLib;

	//Functions
	void ClearAll();
	void AddZone();
	void CompileDirectAccess();
	bool DeleteZone(int index);
	int GetActionBoxByPosition(D3DVECTOR & position, int channel);
	CActionBox * GetActionBoxByType(D3DVECTOR & position, int channel, int type);
	BOOL GetAllApplicableActions(D3DVECTOR & position, int channel, ActionListStructure * actionsList);
	CActionZone * GetByIndex(int type);
};

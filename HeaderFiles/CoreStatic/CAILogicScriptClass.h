#ifndef AILOGICSCRIPTCLASS_H
#define AILOGICSCRIPTCLASS_H

//---------------------------------------------------------//
//---------------- Logic Script Class----------------------//
//---------------------------------------------------------//
class CAILogicScriptObj : public CObject
{     
	DECLARE_SERIAL(CAILogicScriptObj)    

	public:	
		CAILogicScriptObj();

	CString			scriptName;//Name of the script
	CString			scriptDescription;//Description of script
	DWORD			lastUseTimeStamp;//When did we last use the script
	DWORD			howOftenTimer;//How often can we do this script
	unsigned int	scriptAction;//What is the basis of this script
	int				whenToUse;//When do we use this script
	int				miscInt1;//Misc value for scripts
	int				miscInt2;//Misc value for scripts
	int				miscInt3;//Misc value for scripts
	CString			emoteMessage;//Message to send when script is played
	int				listPlacement;//which list do we need to be in

	void Serialize(CArchive& ar);
	void Clone(CAILogicScriptObj **clone);
};

class CAILogicScriptObjList : public CObList
{
	public:
		CAILogicScriptObjList() {};    

	DECLARE_SERIAL(CAILogicScriptObjList)      

	CAILogicScriptObj *GetScriptObject(int index);
	void SafeDelete();
	void Clone(CAILogicScriptObjList **copy);
};

#endif
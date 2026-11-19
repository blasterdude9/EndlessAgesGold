#ifndef AISCRIPTCLASS_H
#define AISCRIPTCLASS_H
//---------------------------------------------------------//
//-------------------Event Class---------------------------//
//---------------------------------------------------------//
class CAIScriptEvent : public CObject
{
	//CAIObj(){}
	
     
    DECLARE_SERIAL( CAIScriptEvent )
    
    
public:
	
	CAIScriptEvent();

	int m_event;//0=goTo location on tree 
	            //1=delay  
	            //2=play anim by version  
	            //3=goTo Random location on tree
	            //4=loop to begginig of script
	int m_miscInt;
	void Serialize( CArchive& ar );
	void Clone(CAIScriptEvent ** clone);
};

class CAIScriptEventList : public CObList
{
public:
   CAIScriptEventList(){};
   
    
     
    DECLARE_SERIAL( CAIScriptEventList )
    
    

    void SafeDelete();
	CAIScriptEvent* GetEvent(int index);
	void AddEvent(int event,int miscInt);
	//functions
	void Clone(CAIScriptEventList ** clone);
};
//---------------------------------------------------------//
//-------------------Script Class--------------------------//
//---------------------------------------------------------//
class CAIScriptObj : public CObject
{
	//CAIObj(){}
	
     
    DECLARE_SERIAL( CAIScriptObj )
    
    
public:
	
	CAIScriptObj();

	CString scriptName;
    CString scriptDescription;

	CAIScriptEventList *  m_events;
	int                   m_currentEvent;
	long                  m_delayDuration;
	long                  m_delayStamp;
	       
    
	CAIScriptEvent* GetEvent(int index);
	void Serialize( CArchive& ar );
    void SafeDelete();
	void Clone(CAIScriptObj ** clone);

};

class CAIScriptObjList : public CObList
{
public:
   CAIScriptObjList(){};
    
     
    DECLARE_SERIAL( CAIScriptObjList )
    
    

    CAIScriptObj * GetScriptObject(int index);
	void SafeDelete();
};
#endif




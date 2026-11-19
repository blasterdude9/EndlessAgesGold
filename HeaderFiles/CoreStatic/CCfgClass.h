#ifndef CFGCLASS_H
#define CFGCLASS_H

class  CCfgObj : public CObject
{
	
     
	DECLARE_SERIAL( CCfgObj );
	
    
public:
    CCfgObj();
    
    int m_globalCFG;//tracer to global inven for inventory selling buying ect
	int m_armed;//see if need to attach
	int m_quanity;

	void Clone(CCfgObj ** clone);
	void Serialize( CArchive& ar );
	
};

class  CCfgObjList : public CObList
{
public:
   CCfgObjList(){};
   void SafeDelete();
   void Clone(CCfgObjList ** clone);
    
     
	DECLARE_SERIAL( CCfgObjList );
	
    
};

#endif
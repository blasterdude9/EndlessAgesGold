#ifndef CELEMENTSTATUSCLASS_H
#define CELEMENTSTATUSCLASS_H

class  CElementStatusObj : public CObject
{
	
     
	DECLARE_SERIAL( CElementStatusObj );
	
    
public:
    CElementStatusObj();

	short m_elementIndex;//corilates to index zero based
	short m_currentValue;//current value

	void SafeDelete();
	void Clone(CElementStatusObj ** clone);
	void SerializeAlloc(CArchive& ar);


	void Serialize( CArchive& ar );
};

class  CElementStatusList : public CObList
{
public:
   CElementStatusList(){};
   
    
     
	DECLARE_SERIAL( CElementStatusList );
	
    

	void SafeDelete();
	void Clone(CElementStatusList ** clone);
	int  GetDefenseForSpecifiedChannel(int damageChannel);
	void SerializeAlloc(CArchive& ar,int loadCount);
};

#endif
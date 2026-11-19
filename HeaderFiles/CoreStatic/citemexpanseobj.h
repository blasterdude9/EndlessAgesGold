#ifndef CEXPANSIVEOBJCLASS_H
#define CEXPANSIVEOBJCLASS_H

class CExpansiveObj : public CObject
{
	
     
	DECLARE_SERIAL( CExpansiveObj );
	
    
public:
    CExpansiveObj();
    
    int    m_iconIndex;
    float  m_xPos;
	float  m_yPos;
	int    m_miscInt;
	int    m_miscInt2;

	
	void Clone(CExpansiveObj ** clone);
	void SerializeAlloc(CArchive& ar);
	void Serialize( CArchive& ar );	
};

class CCExpansiveObjList : public CObList
{
public:
   CCExpansiveObjList(){};
   void SafeDelete();
   void Clone(CCExpansiveObjList ** clone);

    
     
	DECLARE_SERIAL( CCExpansiveObjList ); 
	
    

	CExpansiveObj * GetByMisc2(int miscInt2);
	void SerializeAlloc(CArchive& ar,int loadCount);

};

#endif
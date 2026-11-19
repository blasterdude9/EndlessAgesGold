#ifndef CGFONTCLASS_H
#define CGFONTCLASS_H

class CGFontObj : public CObject
{
	
     
	DECLARE_SERIAL( CGFontObj );
	
    

public:
	
	CGFontObj();
	
	CString        m_fontName;
	int            m_fontTableIndex;
	CString        m_fontTableName;

	void        Serialize( CArchive& ar );

};

class CGFontObjList : public CObList
{
public:
   CGFontObjList(){};
   void SafeDelete();
   
    
   DECLARE_SERIAL( CGFontObjList )
   
   
};


#endif
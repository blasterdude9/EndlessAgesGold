#ifndef CTEXTCLASS_H
#define CTEXTCLASS_H
//Font stuff
class CFontObj : public CObject
{
	CFontObj(){}
	
     
    DECLARE_SERIAL( CFontObj )
    
    
public:
	
	CFontObj(HFONT L_fontObject,
		     CString L_fontName,
			 int L_sizeX,
			 int L_sizeY);

             HFONT List_fontObject;
	         CString List_fontName;
			 int List_sizeX;
			 int List_sizeY;
	

	void Serialize( CArchive& ar );

};

class CFontObjList : public CObList
{
public:
   CFontObjList(){};
    
   void SafeDelete();
   DECLARE_SERIAL( CFontObjList )
    
    
};

//text and font stuff
class CTextEntry : public CObject
{
	CTextEntry(){}
	
     
    DECLARE_SERIAL( CTextEntry )
    
    
public:
	
	CTextEntry(CString L_text,
		       int L_percX,
			   int L_percY);

    
	CString List_text;
	int List_percX;
	int List_percY;

	void Serialize( CArchive& ar );

};

class CTextEntryList : public CObList
{
public:
   CTextEntryList(){};
   
     
    DECLARE_SERIAL( CTextEntryList )
    
    
};
//main class   list of objects defines objects i scene
//each object can have multiple animations and harcy
class CTextCollection : public CObject
{
	CTextCollection(){}
	
     
    DECLARE_SERIAL( CTextCollection )
    
    
public:
	
	CTextCollection(CString L_collectionName,
		            CTextEntryList * L_textEntryList,
					int L_timeVarial,
					int L_timeLength,
					int L_fontSel,
					BOOL L_displayIsOn,
					COLORREF L_textColor);

	COLORREF List_textColor;
	BOOL List_displayIsOn;
	CString List_collectionName;
	CTextEntryList * List_textEntryList;
	int List_timeVarial;
	int List_timeLength;
	int List_fontSel;

	void Serialize( CArchive& ar );

};

class CTextCollectionList : public CObList
{
public:
   CTextCollectionList(){};
   void SafeDelete();

     
    DECLARE_SERIAL( CTextCollectionList )
    
    
};



#endif




#include "stdafx.h"
#include <afxtempl.h>
#include "CTextClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------text entry------------------//
CTextEntry::CTextEntry(CString L_text,
		               int L_percX,
			           int L_percY)
{
List_text = L_text;
List_percX = L_percX;
List_percY = L_percY;
}

void CTextEntry::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << List_text
		  << List_percX
		  << List_percY;
		
	}
	else
	{
        ar >> List_text
		   >> List_percX
		   >> List_percY;
	}
}

IMPLEMENT_SERIAL(CTextEntry, CObject, 0)
IMPLEMENT_SERIAL(CTextEntryList, CObList, 0)


//text collection
CTextCollection::CTextCollection(CString L_collectionName,
		                         CTextEntryList * L_textEntryList,
					             int L_timeVarial,
					             int L_timeLength,
					             int L_fontSel,
								 BOOL L_displayIsOn,
								 COLORREF L_textColor)
{
List_textColor = L_textColor;
List_displayIsOn = L_displayIsOn;
List_collectionName = L_collectionName;
List_textEntryList = L_textEntryList;
List_timeVarial = L_timeVarial;
List_timeLength = L_timeLength;
List_fontSel = L_fontSel;
}

void CTextCollection::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
		ar << List_collectionName
		   << List_textEntryList
		   << List_timeVarial
		   << List_timeLength
		   << List_fontSel
		   << List_textColor;
		
	}
	else
	{
       ar >> List_collectionName
		  >> List_textEntryList
		  >> List_timeVarial
		  >> List_timeLength
		  >> List_fontSel
		  >> List_textColor;

	   List_displayIsOn = FALSE;
	}
}

void CTextCollectionList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
	    CTextCollection* mptLoc = ( CTextCollection* ) GetNext(posLoc);
         for(POSITION posLoc2 = mptLoc->List_textEntryList->GetHeadPosition();posLoc2 != NULL;)
		 { 
             CTextEntry* entryPtrLoc = ( CTextEntry* ) mptLoc->List_textEntryList->GetNext(posLoc2);
			 delete entryPtrLoc;
             entryPtrLoc=0;
		 }
		 mptLoc->List_textEntryList->RemoveAll();
		 delete mptLoc->List_textEntryList;
		 mptLoc->List_textEntryList=0;
		 delete mptLoc;
		 mptLoc=0;
	}
	RemoveAll();
}


IMPLEMENT_SERIAL(CTextCollection, CObject, 0)
IMPLEMENT_SERIAL(CTextCollectionList, CObList, 0)
//font list
CFontObj::CFontObj(HFONT L_fontObject,
		           CString L_fontName,
				   int L_sizeX,
				   int L_sizeY)
{
List_sizeX = L_sizeX;
List_sizeY = L_sizeY;
List_fontObject = L_fontObject;
List_fontName = L_fontName;
}
	
void CFontObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << List_fontName
		  << List_sizeX
		  << List_sizeY;
	}
	else
	{
       ar >> List_fontName
		  >> List_sizeX
		  >> List_sizeY;
	}
}

void CFontObjList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CFontObj * skPtr = (CFontObj *)GetNext(posLoc);
		 if(skPtr->List_fontObject)
            DeleteObject(skPtr->List_fontObject);
		 delete skPtr;
		 skPtr=0;
	 }
	 RemoveAll();
}



IMPLEMENT_SERIAL(CFontObj, CObject, 0)
IMPLEMENT_SERIAL(CFontObjList, CObList, 0)
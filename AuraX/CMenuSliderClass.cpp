#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CMenuSliderClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSliderBarObj::CSliderBarObj()
{
    m_boxClickSize            = 0.0f;
    m_center.x                = 0.0f;
	m_center.y                = 0.0f;
	m_center.z                = 0.0f;
    m_locStart.x              = 0.0f;
	m_locStart.y              = 0.0f;
	m_locStart.z              = 0.0f;
    m_locEnd.x                = 0.0f;
	m_locEnd.y                = 0.0f;
	m_locEnd.z                = 0.0f;
    m_current.x               = 0.0f;
	m_current.y               = 0.0f;
	m_current.z               = 0.0f;
    m_visStart                = NULL;
    m_visMov                  = NULL;
    m_visEnd                  = NULL;
    m_itemCount               = 0;
    m_currentItem             = 0;
    m_itemsThatAreVisible     = 4;
    m_visOffsetX              = .1f;
	m_visOffsetY			  = 0.0f;//.02f; //Hard Code Y Offset to Allow For Larger Buttons on Scroll Bars
    m_visSpacing              = .05f;
	m_acceptingMouseMovements = FALSE;
	m_contentList             = NULL;
	m_clickWidth              = .3f;
	m_currentSelectedContentItem = 0;
	m_selectStamp             = 0;
    m_fontSizeX               = .01f;
    m_fontSizeY               = .01f;
	m_controlID               = 0;
	m_mode                    = 0;
	m_toggleTemp              = 0;
	m_dragDropEnabled         = FALSE;

	m_red						= 1.0f;
    m_green						= 1.0f;
    m_blue						= 1.0f;
    m_selectionDisabled			= 0;
}

void CSliderBarObj::Clone(CSliderBarObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CSliderBarObj * copyLocal = new CSliderBarObj();
     copyLocal->m_boxClickSize            = m_boxClickSize;
     copyLocal->m_center.x                = m_center.x;
	 copyLocal->m_center.y                = m_center.y;
	 copyLocal->m_center.z                = m_center.z;
     copyLocal->m_locStart.x              = m_locStart.x;
	 copyLocal->m_locStart.y              = m_locStart.y;
	 copyLocal->m_locStart.z              = m_locStart.z;
     copyLocal->m_locEnd.x                = m_locEnd.x;
	 copyLocal->m_locEnd.y                = m_locEnd.y;
	 copyLocal->m_locEnd.z                = m_locEnd.z;
     copyLocal->m_current.x               = m_current.x;
	 copyLocal->m_current.y               = m_current.y;
	 copyLocal->m_current.z               = m_current.z;
     copyLocal->m_itemCount               = m_itemCount;
     copyLocal->m_currentItem             = m_currentItem;
     copyLocal->m_itemsThatAreVisible     = m_itemsThatAreVisible;
     copyLocal->m_visOffsetX              = m_visOffsetX;
	 copyLocal->m_visOffsetY			  = m_visOffsetY;
     copyLocal->m_visSpacing              = m_visSpacing;
	 copyLocal->m_acceptingMouseMovements = FALSE;
	 copyLocal->m_clickWidth              = m_clickWidth;
	 copyLocal->m_currentSelectedContentItem = 0;
	 copyLocal->m_selectStamp             = m_selectStamp;
	 copyLocal->m_fontSizeX               = m_fontSizeX;
     copyLocal->m_fontSizeY               = m_fontSizeY;
	 copyLocal->m_controlID               = m_controlID;
	 copyLocal->m_mode                    = m_mode;
	 copyLocal->m_toggleTemp              = m_toggleTemp;
	 copyLocal->m_dragDropEnabled         = m_dragDropEnabled;

	 copyLocal->m_red						= m_red;
     copyLocal->m_green						= m_green;
     copyLocal->m_blue						= m_blue;
     copyLocal->m_selectionDisabled			= m_selectionDisabled;
	
	 
	 if(m_visStart)
	    m_visStart->Clone(&copyLocal->m_visStart,g_pd3dDevice,NULL);
	 if(m_visMov)
	    m_visMov->Clone(&copyLocal->m_visMov,g_pd3dDevice,NULL);
	 if(m_visEnd)
	    m_visEnd->Clone(&copyLocal->m_visEnd,g_pd3dDevice,NULL);

	 if(m_contentList)
		m_contentList->Clone(&copyLocal->m_contentList);

	 *copy = copyLocal;
}


int CSliderBarObj::GetListDisplayBeginning()
{
	if(m_itemCount < m_itemsThatAreVisible)
		return m_currentItem = 0;

	float range		= m_locStart.y - m_locEnd.y;
	float position	= m_locStart.y - m_current.y;
	float itemCount = (float)m_itemCount;
	float visItems	= (float)m_itemsThatAreVisible;

	m_currentItem = 0;
	if(position == 0.0f)
		return 0;
	if(range == 0.0f)
		return 0;

	float percentage = position / range;
	m_currentItem = (int)((itemCount - visItems) * percentage);//+ 1;
	int convIndex = 0;
	if(m_mode != 0)
		m_currentItem -= m_currentItem % 2;

	return m_currentItem;
}

BOOL CSliderBarObj::InsertBefore(CString text,
								 int indexInList,
							     int visIndex,
								 int miscInt,
								 int miscInt2,
								 int miscInt3,
							     int miscInt4)
{
    if(!m_contentList)
		return FALSE;

	POSITION posLast = m_contentList->FindIndex(indexInList);
    if(posLast)
	{//1
       CSliderContentObj * cPtr = new CSliderContentObj();
	   cPtr->m_text        = text;
	   cPtr->m_visualRef   = visIndex;
	   cPtr->m_miscInt     = miscInt;
	   cPtr->m_miscInt2    = miscInt2;
	   cPtr->m_miscInt3     = miscInt3;
	   cPtr->m_miscInt4    = miscInt4;
	   m_contentList->InsertBefore(posLast,cPtr);
	   m_itemCount = (int)m_contentList->GetCount();
	   return TRUE;
	}//end 1
	return FALSE;
}

BOOL CSliderBarObj::InsertAfter(CString text,
								int indexInList,
							    int visIndex,
								int miscInt,
								int miscInt2,
								int miscInt3,
							    int miscInt4)
{
    if(!m_contentList)
		return FALSE;

	POSITION posLast = m_contentList->FindIndex(indexInList);
    if(posLast)
	{//1
       CSliderContentObj * cPtr = new CSliderContentObj();
	   cPtr->m_text        = text;
	   cPtr->m_visualRef   = visIndex;
       cPtr->m_miscInt     = miscInt;
	   cPtr->m_miscInt2    = miscInt2;
	   cPtr->m_miscInt3     = miscInt3;
	   cPtr->m_miscInt4    = miscInt4;
	   m_contentList->InsertAfter(posLast,cPtr);
	   m_itemCount = (int)m_contentList->GetCount();
	   return TRUE;
	}//end 1
	return FALSE;
}

BOOL CSliderBarObj::DeleteString(int index)
{
    if(!m_contentList)
		return FALSE;

    POSITION posLast = m_contentList->FindIndex(index);
    if(posLast)
	{//1
      CSliderContentObj * cPtr = (CSliderContentObj *)m_contentList->GetAt(posLast);
      delete cPtr;
	  cPtr = 0;
      m_contentList->RemoveAt(posLast);
      return TRUE;
	}//end 1 
	return FALSE;
}

void CSliderBarObj::AddString(CString text,
							  int visIndex,
							  int miscInt,
							  int miscInt2,
							  int miscInt3,
							  int miscInt4)
{
    if(!m_contentList)
		m_contentList = new CSliderContentObjList();

        CSliderContentObj * cPtr = new CSliderContentObj();
	    cPtr->m_text        = text;
	    cPtr->m_visualRef   = visIndex;
		cPtr->m_miscInt     = miscInt;
		cPtr->m_miscInt2    = miscInt2;
		cPtr->m_miscInt3    = miscInt3;
		cPtr->m_miscInt4    = miscInt4;
	    m_contentList->AddTail(cPtr);
	    m_itemCount = (int)m_contentList->GetCount();	
}

CSliderContentObj * CSliderBarObj::GetByIndex(int index)
{
    if(!m_contentList)
		return NULL;

	    POSITION posLoc = m_contentList->FindIndex(index);
		if(!posLoc)
			return NULL;
	    CSliderContentObj * ptrSel = (CSliderContentObj *)m_contentList->GetAt(posLoc);
      
		return ptrSel;
}

void CSliderBarObj::FlushAll()
{
    if(!m_contentList)
		return;

	    for(POSITION posLoc = m_contentList->GetHeadPosition();posLoc != NULL;)
		{
		    CSliderContentObj * ptrSel = (CSliderContentObj *)m_contentList->GetNext(posLoc);
			ptrSel->SafeDelete();
			delete ptrSel;
			ptrSel = 0;
		}
		m_contentList->RemoveAll();
}


void CSliderBarObj::SafeDelete()
{
	if(m_visStart){
       m_visStart->SafeDelete();
	   delete m_visStart;
       m_visStart=0;
	}
	if(m_visMov){
       m_visMov->SafeDelete();
	   delete m_visMov;
	   m_visMov=0;	
	}
    if(m_visEnd){
       m_visEnd->SafeDelete();
	   delete m_visEnd;
	   m_visEnd=0;	
	}

	if(m_contentList){
       m_contentList->SafeDelete();
       delete m_contentList;
	   m_contentList=0;
	}
}


void CSliderBarObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
 
	   ar.SetObjectSchema(3);
       ASSERT(ar.GetObjectSchema() == 3);

	   ar << m_boxClickSize
          << m_center.x
		  << m_center.y
		  << m_center.z
          << m_locStart.x
		  << m_locStart.y
		  << m_locStart.z
          << m_locEnd.x
		  << m_locEnd.y
		  << m_locEnd.z
          << m_current.x
		  << m_current.y
		  << m_current.z
          << m_visStart
          << m_visMov
          << m_visEnd
          << m_itemCount
          << m_currentItem
          << m_itemsThatAreVisible
          << m_visOffsetX
          << m_visSpacing
		  << m_contentList
		  << m_clickWidth
		  << m_fontSizeX
		  << m_fontSizeY
		  << m_controlID;	

	   ar << m_red
	      << m_green
	      << m_blue
	      << m_selectionDisabled;
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_boxClickSize
          >> m_center.x
		  >> m_center.y
		  >> m_center.z
          >> m_locStart.x
		  >> m_locStart.y
		  >> m_locStart.z
          >> m_locEnd.x
		  >> m_locEnd.y
		  >> m_locEnd.z
          >> m_current.x
		  >> m_current.y
		  >> m_current.z
          >> m_visStart
          >> m_visMov
          >> m_visEnd
          >> m_itemCount
          >> m_currentItem
          >> m_itemsThatAreVisible
          >> m_visOffsetX
          >> m_visSpacing
		  >> m_contentList
		  >> m_clickWidth
		  >> m_fontSizeX
		  >> m_fontSizeY
		  >> m_controlID;

	      m_red = 1.0f;
	      m_green = 1.0f;
	      m_blue = 1.0f;
	      m_selectionDisabled = 0;

	   if(version > 1)
	   {
	   ar >> m_red
	      >> m_green
	      >> m_blue
	      >> m_selectionDisabled;
	   }
	   m_visOffsetY = 0.0f;
	   if(version > 2)
	   {
	      m_visOffsetY = .02f; //Hard Code Y Offset to Allow For Larger Buttons on Scroll Bars
	   }

	   //m_visOffsetY = .02f;

	   m_acceptingMouseMovements = FALSE;
	   m_currentSelectedContentItem = 0;
	   m_selectStamp = 0;
	   m_mode        = 0;
	   m_toggleTemp  = 0;
	   m_dragDropEnabled = FALSE;
	   //m_currentItem = 0;
	   //m_controlID = 0;
	}
}     

BOOL CSliderContentObjList::ReorderInventory(int objectToMoveIndex,int targetIndex,int mode)
{
	if(objectToMoveIndex == targetIndex)
	   return FALSE;

	if(objectToMoveIndex < 0 || targetIndex < 0)
	   return FALSE;

	  /*if(mode == 1)
	   targetIndex = targetIndex - 2;
	  else
	   targetIndex = targetIndex - 1;*/

	if(targetIndex < 0)
	   targetIndex = 0;
	
	POSITION posObjectToMove = FindIndex(objectToMoveIndex);
	POSITION posObjectToMove2 = FindIndex(objectToMoveIndex+1);
	POSITION posTargetIndex  = FindIndex(targetIndex);
	POSITION posTargetIndex2  = FindIndex(targetIndex+1);

	
	if(mode == 1)
	{

	   if(posObjectToMove2 == NULL || posTargetIndex2 == NULL || posObjectToMove == NULL || posTargetIndex == NULL)
	   return FALSE;

	   CSliderBarObj * invObj = (CSliderBarObj *)GetAt(posObjectToMove);
	   CSliderBarObj * invObj2 = (CSliderBarObj *)GetAt(posObjectToMove2);
	   RemoveAt(posObjectToMove);
	   RemoveAt(posObjectToMove2);
	   if(targetIndex == 0)
	   {
		   AddHead(invObj2);
		   AddHead(invObj);
            
		  return TRUE;
	   }
	   else
	   {
          int trace = 0;
	      for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		  {
             if(trace == targetIndex)
			 {
				InsertBefore(posLoc,invObj);
			    InsertBefore(posLoc,invObj2);
	            
                return TRUE;
			 }
			 GetNext(posLoc);
			 trace++;
		  }
	      
	   }
	   AddTail(invObj);
	   AddTail(invObj2);
       
       return TRUE;
	}
	else{
	
	if(posObjectToMove == NULL || posTargetIndex == NULL)
	   return FALSE;

	CSliderBarObj * invObj = (CSliderBarObj *)GetAt(posObjectToMove);
	
	RemoveAt(posObjectToMove);
	   if(targetIndex == 0)
	   {
		  AddHead(invObj);
		  return TRUE;
	   }
	   else
	   {
		 int trace = 0;
	     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		 {
             if(trace == targetIndex)
			 {
			    InsertBefore(posLoc,invObj);
                return TRUE;
			 }
			 GetNext(posLoc);
			 trace++;
		 }
	   }

    AddHead(invObj);
	return TRUE;
	}

return FALSE;
}


void CSliderBarObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CSliderBarObj * sldObj = (CSliderBarObj *)GetNext(posLoc);
		sldObj->SafeDelete();
		delete sldObj;
		sldObj=0;
	}
	RemoveAll();
}


//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
BOOL CSliderBarObj::Update(DIMOUSESTATE mouseState,D3DVECTOR cursorPosition)
{

    if(mouseState.rgbButtons[0] & 0x80){//pressed
	  m_selectStamp = GetTickCount();
	  if(m_acceptingMouseMovements == FALSE){//not already set
       if(cursorPosition.x < (m_current.x + m_boxClickSize) && 
          cursorPosition.x > (m_current.x - m_boxClickSize) && 
		  cursorPosition.y < (m_current.y + m_boxClickSize) &&
		  cursorPosition.y > (m_current.y - m_boxClickSize)){//lock
          m_acceptingMouseMovements = TRUE;//locking focus
		 
	   }//end lock
	  }//end not already set
	}//end pressed
	else{//not pressed
       m_acceptingMouseMovements = FALSE;//unlock
	}//end not pressed

    if(m_acceptingMouseMovements){//adjust
       m_current.y = cursorPosition.y;
	   if(m_current.y > m_locStart.y)
		  m_current.y = m_locStart.y;

	   if(m_current.y < m_locEnd.y)
		  m_current.y = m_locEnd.y;
		return FALSE;
	}//end adjust

    //evaluate current list selection
	if(mouseState.rgbButtons[0] & 0x80)
	{//clicked
		int newIndex = GetListIndexByPosition(cursorPosition);
		if(newIndex != -1)
		{
			if(newIndex > m_itemCount)
				m_currentSelectedContentItem = -2;
			else
				m_currentSelectedContentItem = newIndex;
			return TRUE;
		}
	}

    return FALSE;
}
//---------------------------------------------------------------/*/
//---------------------------------------------------------------//
//---------------------------------------------------------------//
int CSliderBarObj::GetListIndexByPosition(D3DVECTOR cursorPosition)
{
	int itemSelected = 0;
    float flItemCount = (float)m_itemsThatAreVisible;
	if(m_mode == 2) flItemCount /= 2.0f;
	float range = m_visSpacing * flItemCount;
    if((m_current.x + m_boxClickSize + m_visOffsetX) < cursorPosition.x &&
	   (m_locStart.y - range + m_visOffsetY) < cursorPosition.y &&
       (m_locStart.y + m_visSpacing + m_visOffsetY) > cursorPosition.y && 
	   (m_locStart.x + m_clickWidth + m_visOffsetX) > cursorPosition.x)
	{//valid    
		float position = m_locStart.y + m_visOffsetY - cursorPosition.y + (m_visSpacing * 0.5f);
		if(range == 0.0f)
	       itemSelected = 0;
		else
		{
			//float percentage = (position -(m_visSpacing * .5f)) / range;
			//itemSelected = (((int)(flItemCount * percentage) + 1) + m_currentItem);
			itemSelected = (int)(position / m_visSpacing) + m_currentItem;
			if(m_mode != 0)
			{
				if(m_mode == 2)
					itemSelected = (int)(position / m_visSpacing) * 2 + m_currentItem;

				itemSelected -= itemSelected % 2;
			}
			//m_currentSelectedContentItem = itemSelected;
		}
		return itemSelected;
	}//end valid
    return -1;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
void CSliderBarObj::IndexUp(int glAdjust)
{
	m_current.y += (m_locStart.y - m_locEnd.y) / m_itemCount;
	if(m_current.y > m_locStart.y)
		m_current.y = m_locStart.y;

	if(m_current.y < m_locEnd.y)
		m_current.y = m_locEnd.y;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
void CSliderBarObj::IndexDown(int glAdjust)
{
	m_current.y -= (m_locStart.y - m_locEnd.y) / m_itemCount;
	if(m_current.y > m_locStart.y)
		m_current.y = m_locStart.y;

	if(m_current.y < m_locEnd.y)
		m_current.y = m_locEnd.y;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CSliderBarObj::ConvToString(int number)
{    
              CString temp;
			  temp.Format("%i",number);
	
	          /*if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);*/
		
			  return temp;
}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
BOOL CSliderBarObj::BuildSystem(CString startVisualFile,
								CString midVisFile,
								CString endVisFile,
								D3DVECTOR centerOfSystem,
								D3DVECTOR endPos,
								D3DVECTOR startPos,
								float clickZoneSize,
								int visibleItems,
								float visOffset,
								float visSpacing,
								float clickWidth,
								float fontSizeX,
								float fontSizeY,
								LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	m_fontSizeX       = fontSizeX;
    m_fontSizeY       = fontSizeY;
    m_boxClickSize    = clickZoneSize;
	m_center.x        = centerOfSystem.x;
	m_center.y        = centerOfSystem.y;
	m_center.z        = centerOfSystem.z;
    m_locStart.x      = startPos.x;
	m_locStart.y      = startPos.y;
	m_locStart.z      = startPos.z;
    m_locEnd.x        = endPos.x;
	m_locEnd.y        = endPos.y;
	m_locEnd.z        = endPos.z;
	m_current.x       = centerOfSystem.x;
	m_current.y       = centerOfSystem.y;
	m_current.z       = centerOfSystem.z;
	m_itemsThatAreVisible     = visibleItems;
    m_visOffsetX              = visOffset;
    m_visSpacing              = visSpacing;
	m_clickWidth              = clickWidth;
    //start slider visual
	if(m_visStart){
       m_visStart->SafeDelete();
       delete m_visStart;
	   m_visStart=0;
	}
    m_visStart = new CSkeletonObject();
    if(!m_visStart->InitBoneSystem(startVisualFile,g_pd3dDevice)){//failed
        m_visStart->SafeDelete();
	    delete m_visStart;
	    m_visStart=0;
        return FALSE;
	}//end failed
    //mid mov slider visual
	if(m_visMov){
       m_visMov->SafeDelete();
       delete m_visMov;
	   m_visMov=0;
	}
    m_visMov = new CSkeletonObject();
    if(!m_visMov->InitBoneSystem(midVisFile,g_pd3dDevice)){//failed
        m_visMov->SafeDelete();
	    delete m_visMov;
	    m_visMov=0;
        return FALSE;
	}//end failed
    //end slider visual
	if(m_visEnd){
       m_visEnd->SafeDelete();
       delete m_visEnd;
	   m_visEnd=0;
	}
    m_visEnd = new CSkeletonObject();
    if(!m_visEnd->InitBoneSystem(endVisFile,g_pd3dDevice)){//failed
        m_visEnd->SafeDelete();
	    delete m_visEnd;
	    m_visEnd=0;
        return FALSE;
	}//end failed
	return TRUE;
}
//---------------
//content objects
//---------------
CSliderContentObj::CSliderContentObj()
{

    m_text;
    m_visualRef = 0;
	m_miscInt   = 0;
	m_miscInt2  = 0;
	m_miscInt3  = 0;
	m_miscInt4  = 0;
	m_textVisual = NULL;
}

void CSliderContentObj::SafeDelete()
{

	if(m_textVisual)
	{
       m_textVisual->SafeDelete();
	   delete m_textVisual;
	   m_textVisual = 0;
	}
}

void CSliderContentObj::Clone(CSliderContentObj ** copy)
{
    CSliderContentObj * copyLocal = new CSliderContentObj();
    copyLocal->m_text             = m_text;
    copyLocal->m_visualRef        = m_visualRef;
	copyLocal->m_miscInt          = m_miscInt;
	copyLocal->m_miscInt2         = m_miscInt2;
	copyLocal->m_miscInt3         = m_miscInt3;
	copyLocal->m_miscInt4         = m_miscInt4;

	*copy = copyLocal;
}

void CSliderContentObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_text
          << m_visualRef
		  << m_miscInt;
	}
	else
	{
       ar >> m_text
          >> m_visualRef
		  >> m_miscInt;



	   m_miscInt2   = 0;
	   m_miscInt3   = 0;
	   m_miscInt4   = 0;
	   m_textVisual = NULL;
	}
}     

void CSliderContentObjList::Clone(CSliderContentObjList ** copy)
{
   CSliderContentObjList * copyLocal = new CSliderContentObjList();
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CSliderContentObj * ptr = (CSliderContentObj *)GetNext(posLoc);
       CSliderContentObj * cloned = NULL; 
	   ptr->Clone(&cloned);
	   copyLocal->AddTail(cloned);
   }
   *copy = copyLocal;
}

void CSliderContentObjList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CSliderContentObj * ptr = (CSliderContentObj *)GetNext(posLoc);
       ptr->SafeDelete();
	   delete ptr;
	   ptr=0;
   }
   RemoveAll();

}

CSliderContentObj * CSliderContentObjList::GetByMiscInt(int miscl)
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CSliderContentObj * ptr = (CSliderContentObj *)GetNext(posLoc);
	   if(ptr->m_miscInt == miscl)
	   {
          return ptr;

	   }
   }
   return NULL;

}

CSliderContentObj * CSliderContentObjList::GetByMiscIntQuanity(int miscl)
{
   BOOL found = FALSE;
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CSliderContentObj * ptr = (CSliderContentObj *)GetNext(posLoc);
	   if(!found)
	   if(ptr->m_miscInt == miscl)
	   {
		   found = TRUE;
           continue;
	   }
	   if(found)
	      return ptr;
   }
   return NULL;

}





BOOL CSliderBarObj::GetInfoByIndex(int index, 
									int * miscInt, 
									int * duplicatePassed,
									CSliderContentObj ** ptrReference)
{
   if(!this->m_contentList)
      return FALSE;
	
	if(index < 0)
	  return FALSE;
  
   POSITION posLoc = this->m_contentList->FindIndex(index);
   if(!posLoc)
	  return FALSE;

   int dupLocal = 0;
       
   CSliderContentObj * ptr = (CSliderContentObj *)this->m_contentList->GetAt(posLoc);

   if(miscInt)
      *miscInt = ptr->m_miscInt;

   if(ptrReference)
      *ptrReference = ptr;

   int trace = 0;
   BOOL skip = 0;
   if(duplicatePassed)
   for(posLoc = this->m_contentList->GetHeadPosition();posLoc != NULL;)
   {
       CSliderContentObj * testPtr = (CSliderContentObj *)this->m_contentList->GetNext(posLoc);
       if(trace==index)
	   {
		   if(duplicatePassed)
             *duplicatePassed = dupLocal;
		   return TRUE;
	   }
	   
	   if(testPtr->m_miscInt == ptr->m_miscInt)
	   {
	      if(skip)
		     skip = 0;
	      else
		  {
             dupLocal++;
             if(m_mode == 1 || m_mode == 2)
                skip = 1;
		  }
	   }
       trace++;
   }

   

   return FALSE;
}

CSliderContentObj * CSliderContentObjList::GetByIndex(int index)
{
   if(index < 0)
	  return NULL;
  

   POSITION posLoc = FindIndex(index);
   if(!posLoc)
	  return NULL;
       
   CSliderContentObj * ptr = (CSliderContentObj *)GetAt(posLoc);
	  
   return ptr;

}

IMPLEMENT_SERIAL(CSliderContentObj, CObject, 0)
IMPLEMENT_SERIAL(CSliderContentObjList, CObList, 0)
IMPLEMENT_SERIAL(CSliderBarObj, CObject, VERSIONABLE_SCHEMA|3)
IMPLEMENT_SERIAL(CSliderBarObjList, CObList, 0)


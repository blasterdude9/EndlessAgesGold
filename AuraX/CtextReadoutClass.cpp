#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CTextReadoutClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CStringObj::CStringObj()
{
   m_textVisual = NULL; 
   m_red        = 1.0f;
   m_green      = 1.0f;
   m_blue       = 1.0f;	 
}

void CStringObj::Clone(CStringObj ** copy)
{
   CStringObj * copyPtr = new CStringObj();
   copyPtr->m_text = m_text;

   *copy = copyPtr;
}

void CStringObj::SafeDelete()
{
   if(m_textVisual)
   {
      m_textVisual->SafeDelete();
	  delete m_textVisual;
      m_textVisual = 0;
   }
}


void CStringObj::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_text;
	}
	else
	{
    ar >> m_text;
	  m_red        = 1.0f;
      m_green      = 1.0f;
      m_blue       = 1.0f;	 
	}
}


void CStringObjList::Clone(CStringObjList ** copy)
{
     CStringObjList * copyLocal = new CStringObjList();

     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CStringObj * objPtr = (CStringObj *)GetNext(posLoc);
         CStringObj * cpyPtr = NULL;
		 objPtr->Clone(&cpyPtr);
		 copyLocal->AddTail(cpyPtr);   
	 }
   *copy = copyLocal;
}

//text 

CtextReadoutObj::CtextReadoutObj()
{
   m_vertSpacing            = 5;
   m_historyCount           = 35;
   m_maxLineLength          = 65;
   m_verticleLineVistCount  = 3;
   m_timeOption             = 4000;
   m_locationX              = 1000;
   m_locationY              = 1000;
   m_textLines              = NULL;
   m_cgLocationX            = -.39f;
   m_cgLocationY            = -.4f;
   m_cgVertSpacing          = .02f;
   m_cgBlendMode            = 2;
   m_cgFontSizeX            = .007f;
   m_cgFontSizeY            = .009f;
   m_currentOffset          = 0;
   m_scrollStamp            = GetTickCount();
   m_maxMeasuredLength      = .54f;
   m_reinit                 = false;
   m_speechMode             = 0;
   m_symbolMapSysRef        = NULL;
}


void CtextReadoutObj::SafeDelete()
{
	if(m_textLines){
       for(POSITION posLoc = m_textLines->GetHeadPosition();posLoc != NULL;)
	   {
		   CStringObj * strPtr = (CStringObj *)m_textLines->GetNext(posLoc);
		   strPtr->SafeDelete();
		   delete strPtr;
		   strPtr=0;
	   }
	   m_textLines->RemoveAll();
	   delete m_textLines;
	   m_textLines=0;
	}
}

void CtextReadoutObj::Flush()
{
	if(m_textLines){
       for(POSITION posLoc = m_textLines->GetHeadPosition();posLoc != NULL;)
	   {
		   CStringObj * strPtr = (CStringObj *)m_textLines->GetNext(posLoc);
		   strPtr->SafeDelete();
		   delete strPtr;
		   strPtr=0;
	   }
	   m_textLines->RemoveAll();
	}
}



void CtextReadoutObj::Clone(CtextReadoutObj ** copy)
{
   CtextReadoutObj * copyPtr = new CtextReadoutObj();
   copyPtr->m_vertSpacing            = m_vertSpacing;
   copyPtr->m_historyCount           = m_historyCount;
   copyPtr->m_maxLineLength          = m_maxLineLength;
   copyPtr->m_verticleLineVistCount  = m_verticleLineVistCount;
   copyPtr->m_timeOption             = m_timeOption;
   copyPtr->m_locationX              = m_locationX;
   copyPtr->m_locationY              = m_locationY;
   copyPtr->m_maxMeasuredLength      = m_maxMeasuredLength;

   copyPtr->m_textLines              = NULL;
   if(m_textLines)
   m_textLines->Clone(&copyPtr->m_textLines);

   *copy = copyPtr;
}

CString CtextReadoutObj::FloatToStingCnv(float number)
{	
   CString final;
   final.Format("%f",number);
   return final;
}

void CtextReadoutObj::AddLine(CString text)
{
	AddLine(text,1.0f,1.0f,1.0f);
}

void CtextReadoutObj::AddLine(D3DVECTOR vector)
{
    CString build = FloatToStingCnv(vector.x) + " ";
	build = build + FloatToStingCnv(vector.y);
	build = build + " ";
	build = build + FloatToStingCnv(vector.z);
	AddLine(build,1.0f,1.0f,1.0f);
}

void CtextReadoutObj::AddLine(AXDVECTOR vector)
{
    CString build = FloatToStingCnv((float)vector.x)+" ";
	build = build + FloatToStingCnv((float)vector.y);
	build = build + " ";
	build = build + FloatToStingCnv((float)vector.z);
	AddLine(build,1.0f,1.0f,1.0f);
}

void CtextReadoutObj::AddLine(CString label,D3DVECTOR vector)
{
    CString build = FloatToStingCnv(vector.x) + " ";
	build = build + FloatToStingCnv(vector.y);
	build = build + " ";
	build = build + FloatToStingCnv(vector.z);
	AddLine(label + ":" + build,1.0f,1.0f,1.0f);
}

void CtextReadoutObj::AddLine(CString label,AXDVECTOR vector)
{
    CString build = FloatToStingCnv((float)vector.x) + " ";
	build = build + FloatToStingCnv((float)vector.y);
	build = build + " ";
	build = build + FloatToStingCnv((float)vector.z);
	AddLine(label + ":" + build,1.0f,1.0f,1.0f);
}

void CtextReadoutObj::AddLine(float floatNumber)
{
    AddLine(FloatToStingCnv(floatNumber),1.0f,1.0f,1.0f);
}

void CtextReadoutObj::AddLine(CString label,float floatNumber)
{
	AddLine(label + ":" + FloatToStingCnv(floatNumber),1.0f,1.0f,1.0f);
}

void CtextReadoutObj::AddLine(CString textLine,
							  float red,
							  float green,
							  float blue)
{
   if(!m_textLines)
	   m_textLines = new CStringObjList;

   //float m_maxMeasuredLength = .54f;
   int charCount = textLine.GetLength();
   if(charCount < 1)
	  return;
   float characterSpacing = m_cgFontSizeX*2.0f;

   float curLengthBuilt = 0.0f;
   CString buildTemp;
   int seeker2 = 0;
   for(int seeker = 0;seeker<charCount;seeker++)
   {//looper main
       
       CString letter = (CString)textLine.GetAt(seeker);
	   CSymbolMapperObj * symPtr = m_symbolMapSysRef->GetByID((CString)textLine.GetAt(seeker));
       if(symPtr)
	   {//add this
		  curLengthBuilt += characterSpacing * symPtr->m_percentageX;
		  buildTemp = operator+(buildTemp,letter);
          if(curLengthBuilt >= m_maxMeasuredLength)
		  {
		     CStringObj * textLinePtr = new CStringObj();
             textLinePtr->m_red = red;
             textLinePtr->m_green = green;
             textLinePtr->m_blue = blue;
             
			 int charCount = buildTemp.GetLength();
			 //regress to nearest space
             int regressionCount = 0;
			 char currentChar = 'i';
			 int currentPosition = charCount-1;
			 BOOL foundBlank = TRUE;
             while(currentChar != ' ')
			 {
			     currentChar = buildTemp.GetAt(currentPosition);
				 currentPosition--;

                 regressionCount++;
                 if(regressionCount > charCount){
					foundBlank = FALSE;
                    break;
				 }

                 if(regressionCount > 15){
					foundBlank = FALSE;
					break;
				 }
			 }
			 if(foundBlank){//do regression
                seeker -= regressionCount;
				seeker2 -= regressionCount;
				if((seeker2+1) > regressionCount)
				buildTemp.Delete(seeker2+1,regressionCount);
			 }//do regression
             //end regress to nearest space 
             textLinePtr->m_text = buildTemp;
			 m_textLines->AddTail(textLinePtr);

			 buildTemp.Empty();
			 curLengthBuilt = 0.0f;
			 seeker2 = 0;
		  }
		  
	   }//end add this
	   seeker2++;
   }//end looper main

   if(buildTemp.GetLength() > 0)
   {//post left over
      CStringObj * textLinePtr = new CStringObj();
      textLinePtr->m_red = red;
      textLinePtr->m_green = green;
      textLinePtr->m_blue = blue;
      textLinePtr->m_text = buildTemp;
      m_textLines->AddTail(textLinePtr);
   }//end post left over
   /*if(m_maxLineLength > textLine.GetLength())
   {
      CStringObj * textLinePtr = new CStringObj();
	  textLinePtr->m_red = red;
	  textLinePtr->m_green = green;
	  textLinePtr->m_blue = blue;
	  textLinePtr->m_text = textLine;
      m_textLines->AddTail(textLinePtr);
   }
   else{
       


       while(textLine.GetLength() > m_maxLineLength)
	   {
		   CStringObj * textLinePtr = new CStringObj();
		   textLinePtr->m_red = red;
	       textLinePtr->m_green = green;
	       textLinePtr->m_blue = blue;
	       int length = textLine.GetLength();
		   CString editString = textLine;
           editString.Delete(m_maxLineLength,(textLine.GetLength()-m_maxLineLength));
		   textLinePtr->m_text = editString;
           m_textLines->AddTail(textLinePtr);
		   textLine.Delete(0,m_maxLineLength);
	   }
	   CStringObj * textLinePtr = new CStringObj();
	   textLinePtr->m_text = textLine;
	   m_textLines->AddTail(textLinePtr);
   }*/
   //trim any history over limit
   int historyObjectCount = (int)m_textLines->GetCount();
   if(historyObjectCount > m_historyCount){//trim
      int trimCount = historyObjectCount - m_historyCount;
		POSITION posLast;
		int track=0;
        for(POSITION posLoc = m_textLines->GetHeadPosition();(posLast = posLoc) != NULL;)
		{
            CStringObj * strPtr = (CStringObj *)m_textLines->GetNext(posLoc);
			strPtr->SafeDelete();
			delete strPtr;
			strPtr=0;
			m_textLines->RemoveAt(posLast);
			track++;
			if(track >= trimCount)
			   break;
		}
   }//end trim
   //ReinitVisualSection(g_pd3dDevice,symbolMapSys,speechMode);
   m_reinit = true;
}




void CtextReadoutObj::AddLineChina(CString textLine,
							       float red,
							       float green,
							       float blue)
{
   if(!m_textLines)
	   m_textLines = new CStringObjList;

   //float m_maxMeasuredLength = .54f;
   int charCount = textLine.GetLength();
   if(charCount < 1)
	  return;
   float characterSpacing = m_cgFontSizeX*2.0f;

   float curLengthBuilt = 0.0f;
   CString buildTemp;
   int seeker2 = 0;
   for(int seeker = 0;seeker<charCount;seeker++)
   {//looper main
       
       CString letter = (CString)textLine.GetAt(seeker);
	   CSymbolMapperObj * symPtr = m_symbolMapSysRef->GetByID((CString)textLine.GetAt(seeker));
       if(symPtr)
	   {//add this
		  curLengthBuilt += characterSpacing * symPtr->m_percentageX;
		  if(letter == "~")
             curLengthBuilt -= characterSpacing * 3;

		  buildTemp = operator+(buildTemp,letter);
          if(curLengthBuilt >= m_maxMeasuredLength)
		  {
		     CStringObj * textLinePtr = new CStringObj();
             textLinePtr->m_red = red;
             textLinePtr->m_green = green;
             textLinePtr->m_blue = blue;
             
			 int charCount = buildTemp.GetLength();
			 //regress to nearest space
             int regressionCount = 0;
			 char currentChar = 'i';
			 int currentPosition = charCount-1;
			 BOOL foundBlank = TRUE;
             while(currentChar != ' ')
			 {
			     currentChar = buildTemp.GetAt(currentPosition);
				 currentPosition--;

                 regressionCount++;
                 if(regressionCount > charCount){
					foundBlank = FALSE;
                    break;
				 }

                 if(regressionCount > 15){
					foundBlank = FALSE;
					break;
				 }
			 }
			 if(foundBlank){//do regression
                seeker -= regressionCount;
				seeker2 -= regressionCount;
				if((seeker2+1) > regressionCount)
				buildTemp.Delete(seeker2+1,regressionCount);
			 }//do regression
             //end regress to nearest space 
             textLinePtr->m_text = buildTemp;
			 m_textLines->AddTail(textLinePtr);

			 buildTemp.Empty();
			 curLengthBuilt = 0.0f;
			 seeker2 = 0;
		  }
		  
	   }//end add this
	   seeker2++;
   }//end looper main

   if(buildTemp.GetLength() > 0)
   {//post left over
      CStringObj * textLinePtr = new CStringObj();
      textLinePtr->m_red = red;
      textLinePtr->m_green = green;
      textLinePtr->m_blue = blue;
      textLinePtr->m_text = buildTemp;
      m_textLines->AddTail(textLinePtr);
   }//end post left over
   /*if(m_maxLineLength > textLine.GetLength())
   {
      CStringObj * textLinePtr = new CStringObj();
	  textLinePtr->m_red = red;
	  textLinePtr->m_green = green;
	  textLinePtr->m_blue = blue;
	  textLinePtr->m_text = textLine;
      m_textLines->AddTail(textLinePtr);
   }
   else{
       


       while(textLine.GetLength() > m_maxLineLength)
	   {
		   CStringObj * textLinePtr = new CStringObj();
		   textLinePtr->m_red = red;
	       textLinePtr->m_green = green;
	       textLinePtr->m_blue = blue;
	       int length = textLine.GetLength();
		   CString editString = textLine;
           editString.Delete(m_maxLineLength,(textLine.GetLength()-m_maxLineLength));
		   textLinePtr->m_text = editString;
           m_textLines->AddTail(textLinePtr);
		   textLine.Delete(0,m_maxLineLength);
	   }
	   CStringObj * textLinePtr = new CStringObj();
	   textLinePtr->m_text = textLine;
	   m_textLines->AddTail(textLinePtr);
   }*/
   //trim any history over limit
   int historyObjectCount = (int)m_textLines->GetCount();
   if(historyObjectCount > m_historyCount){//trim
      int trimCount = historyObjectCount - m_historyCount;
		POSITION posLast;
		int track=0;
        for(POSITION posLoc = m_textLines->GetHeadPosition();(posLast = posLoc) != NULL;)
		{
            CStringObj * strPtr = (CStringObj *)m_textLines->GetNext(posLoc);
			strPtr->SafeDelete();
			delete strPtr;
			strPtr=0;
			m_textLines->RemoveAt(posLast);
			track++;
			if(track >= trimCount)
			   break;
		}
   }//end trim
   //ReinitVisualSection(g_pd3dDevice,symbolMapSys,speechMode);
   m_reinit = true;
}


void CtextReadoutObj::RenderTextCG(CStateManagementObj * statemanager,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    if(!m_textLines)
		return;
	if(m_reinit == true)
	   ReinitVisualSection(g_pd3dDevice,m_symbolMapSysRef,m_speechMode);

	m_reinit = false;

	int displayTrack = 0;
    BOOL reinitializeAbove = 0;

	int totalCount = 0;

	if(m_textLines)
    totalCount = (int)m_textLines->GetCount();

	/*int offsetFloating = m_currentOffset-m_verticleLineVistCount;
	if(offsetFloating < 0)
	   offsetFloating = 0;*/

    int startPosition = (totalCount - m_currentOffset)-1;

	if(startPosition < 0)
	   startPosition = 0;

	

	
    for(POSITION posLoc = m_textLines->FindIndex(startPosition);posLoc != NULL;)
	{
        CStringObj * strPtr = (CStringObj *)m_textLines->GetPrev(posLoc);

        if(!strPtr->m_textVisual)
		{
			reinitializeAbove = TRUE;
		    /*strPtr->m_textVisual = new CTextureRenderObj(strPtr->m_text,
									                     1,
									                     m_cgLocationX,
									                    (m_cgLocationY + (m_cgVertSpacing * displayTrack)),
									                     m_cgFontSizeX,
									                     m_cgFontSizeY,
									                     TRUE);*/
			//strPtr->m_textVisual->Initialize(g_pd3dDevice);

            //strPtr->m_textVisual->m_vertexObject->Render(g_pd3dDevice);
		}	
		else{
		    g_pd3dDevice->SetMaterial(&strPtr->m_textVisual->m_materialObj->m_useMaterial);
			statemanager->SetDiffuseSource(strPtr->m_textVisual->m_vertexObject->m_advancedFixedMode,g_pd3dDevice);
			statemanager->SetVertexType(strPtr->m_textVisual->m_vertexObject->m_uvCount,g_pd3dDevice);
			strPtr->m_textVisual->m_vertexObject->Render(g_pd3dDevice,statemanager->m_currentVertexType);
		}
		/*textureRenderList->AddTextObject(strPtr->m_text,m_cgBlendMode,m_cgLocationX,
			                            (m_cgLocationY + (m_cgVertSpacing * displayTrack)),
										 m_cgFontSizeX,m_cgFontSizeY,TRUE);*/

		displayTrack++;	
		if(displayTrack >= m_verticleLineVistCount)
		   break;
	}

	//if(reinitializeAbove)
    //ReinitVisualSection(g_pd3dDevice);

}


void CtextReadoutObj::ScrollUp(LPDIRECT3DDEVICE9 g_pd3dDevice,CSymbolMapperObjList * symbolMapSys,int speechMode)
{
	long currentTime = GetTickCount();
    if((currentTime-m_scrollStamp) < 100)
		return;

	m_scrollStamp = currentTime;

	int histCountLoc = m_historyCount;

    if(m_textLines)
    m_historyCount = (int)m_textLines->GetCount();

    if(m_historyCount > m_currentOffset){
       m_currentOffset++;

       

       ReinitVisualSection(g_pd3dDevice,symbolMapSys,speechMode);
	}
}

void CtextReadoutObj::ScrollDown(LPDIRECT3DDEVICE9 g_pd3dDevice,CSymbolMapperObjList * symbolMapSys,int speechMode)
{
    long currentTime = GetTickCount();
    if((currentTime-m_scrollStamp) < 100)
		return;

	m_scrollStamp = currentTime;

    if(m_currentOffset > 0){
	   m_currentOffset--;
	   ReinitVisualSection(g_pd3dDevice,symbolMapSys,speechMode);
	}
}

void CtextReadoutObj::ReinitVisualSection(LPDIRECT3DDEVICE9 g_pd3dDevice,CSymbolMapperObjList * symbolMapSys,int speechMode)
{
	int displayTrack = 0;
    BOOL reinitializeAbove = 0;

    int totalCount = 0;

	if(m_textLines)
       totalCount = (int)m_textLines->GetCount();

    /*int offsetFloating = m_currentOffset-m_verticleLineVistCount;
	if(offsetFloating < 0)
	   offsetFloating = 0;*/

    int startPosition = (totalCount - m_currentOffset)-1;

	if(startPosition < 0)
	   startPosition = 0;
    

	if(m_textLines)
    for(POSITION posLoc = m_textLines->FindIndex(startPosition);posLoc != NULL;)
	{
        CStringObj * strPtr = (CStringObj *)m_textLines->GetPrev(posLoc);

        if(strPtr->m_textVisual){
	       strPtr->m_textVisual->SafeDelete();
		   delete strPtr->m_textVisual;
		   strPtr->m_textVisual = 0;
		}
		   reinitializeAbove = TRUE;
		   
		   strPtr->m_textVisual = new CTextureRenderObj("",
									                    1,
									                    m_cgLocationX,
									                   (m_cgLocationY + (m_cgVertSpacing * displayTrack)),
									                    m_cgFontSizeX,
									                    m_cgFontSizeY,
									                    TRUE,
														strPtr->m_red,
														strPtr->m_green,
														strPtr->m_blue);
		   strPtr->m_textVisual->Initialize(g_pd3dDevice,strPtr->m_text,FALSE,symbolMapSys,speechMode,0.0f);

   
		/*textureRenderList->AddTextObject(strPtr->m_text,m_cgBlendMode,m_cgLocationX,
			                            (m_cgLocationY + (m_cgVertSpacing * displayTrack)),
										 m_cgFontSizeX,m_cgFontSizeY,TRUE);*/

		displayTrack++;	
		if(displayTrack >= m_verticleLineVistCount)
		   break;
	}
}


CStringObj * CtextReadoutObj::GetHeadVisual()
{
	if(m_textLines)
    for(POSITION posLoc = m_textLines->GetTailPosition();posLoc != NULL;)
	{
        CStringObj * strPtr = (CStringObj *)m_textLines->GetPrev(posLoc);
		if(strPtr->m_textVisual)
		   return strPtr;

	}
	return NULL;
}


void CtextReadoutObj::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_vertSpacing
	   << m_historyCount
	   << m_maxLineLength
	   << m_verticleLineVistCount
	   << m_timeOption
	   << m_locationX
	   << m_locationY
	   << m_textLines
	   << m_cgLocationX 
	   << m_cgLocationY
	   << m_cgVertSpacing
	   << m_cgBlendMode
       << m_cgFontSizeX
       << m_cgFontSizeY;
	}
	else
	{
    ar >> m_vertSpacing
	   >> m_historyCount
	   >> m_maxLineLength
	   >> m_verticleLineVistCount
	   >> m_timeOption
	   >> m_locationX
	   >> m_locationY
	   >> m_textLines
	   >> m_cgLocationX 
	   >> m_cgLocationY
	   >> m_cgVertSpacing
	   >> m_cgBlendMode
       >> m_cgFontSizeX
       >> m_cgFontSizeY;

	  m_scrollStamp            = GetTickCount();
	  m_maxMeasuredLength = .54f;

	}
}


void CtextReadoutObjList::Clone(CtextReadoutObjList ** copy)
{
     CtextReadoutObjList * copyLocal = new CtextReadoutObjList();

     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CtextReadoutObj * objPtr = (CtextReadoutObj *)GetNext(posLoc);
         CtextReadoutObj * cpyPtr = NULL;
		 objPtr->Clone(&cpyPtr);
		 copyLocal->AddTail(cpyPtr);
         
	 }
   *copy = copyLocal;
}



IMPLEMENT_SERIAL(CtextReadoutObj, CObject, 0)
IMPLEMENT_SERIAL(CStringObj, CObject, 0)
IMPLEMENT_SERIAL(CtextReadoutObjList, CObject, 0)
IMPLEMENT_SERIAL(CStringObjList, CObject, 0)
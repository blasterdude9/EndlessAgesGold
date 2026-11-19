#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
#include "math.h"
#include <afxtempl.h>

#include "CHudClass.h"

//DISPLAY UNIT CLASS
CHudDisplayObj::CHudDisplayObj()
{
List_unitType = 0;
List_vectorObj = NULL;
List_posX = 0;
List_posY = 0;
List_fontIndex = 0;
List_uniFloat = 0.0f;
List_uniInt=0;

List_penRed = 255;
List_penGreen = 255;
List_penBlue = 255;
List_textRed = 255;
List_textGreen = 255;
List_textBlue = 255;
List_brushRed = 0;
List_brushGreen = 0;
List_brushBlue = 0;
List_lineThickness = 1;
}

void CHudDisplayObj::SafeDelete()
{
	if(List_vectorObj){
       List_vectorObj->SafeDelete();
       delete List_vectorObj;
	   List_vectorObj=0;
	}
}



void CHudDisplayObj::Serialize( CArchive& ar )
{
int dump = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar  << List_unitType
           << List_vectorObj
           << List_posX
           << List_posY
           << List_fontIndex
		   << List_uniInt
		   << List_penRed
		   << List_penGreen
		   << List_penBlue
		   << List_textRed
		   << List_textGreen
		   << List_textBlue
		   << List_brushRed
		   << List_brushGreen
		   << List_brushBlue
		   << List_uniFloat
		   << List_lineThickness
		   << List_uniText;
	}
	else
	{
       ar  >> List_unitType
           >> List_vectorObj
           >> List_posX
           >> List_posY
           >> List_fontIndex
		   >> List_uniInt
		   >> List_penRed
		   >> List_penGreen
		   >> List_penBlue
		   >> List_textRed
		   >> List_textGreen
		   >> List_textBlue
		   >> List_brushRed
		   >> List_brushGreen
		   >> List_brushBlue
		   >> List_uniFloat
	       >> List_lineThickness
		   >> List_uniText;
	}
}

//MAIN HUD CLASS
CHudObj::CHudObj(CString hudName)
{
   List_HudCollectionName = hudName;
   List_displayUnitList = new CHudDisplayList;
}
//add display unit
BOOL CHudObj::AddUnit(int uniInt,
					  int type,
					  int posX,
					  int posY,
					  int fontIndex,
					  float uniFloat,
					  CString uniString,
					  BOOL LoadDxfFile,
					  int penRed,
		              int penGreen,
		              int penBlue,
		              int textRed,
		              int textGreen,
		              int textBlue,
		              int brushRed,
		              int brushGreen,
		              int brushBlue,
					  int LineThickness
					  )
{
CHudDisplayObj * displPtr = new CHudDisplayObj();
List_displayUnitList->AddTail(displPtr);
displPtr->List_unitType = type;
displPtr->List_fontIndex = fontIndex;
displPtr->List_posX = posX;
displPtr->List_posY = posY;
displPtr->List_uniText = uniString;
displPtr->List_uniFloat = uniFloat;
displPtr->List_vectorObj = NULL;
displPtr->List_uniInt = uniInt;
displPtr->List_penRed = penRed;
displPtr->List_penGreen = penGreen;
displPtr->List_penBlue = penBlue;
displPtr->List_textRed = textRed;
displPtr->List_textGreen = textGreen;
displPtr->List_textBlue = textBlue;
displPtr->List_brushRed = brushRed;
displPtr->List_brushGreen = brushGreen;
displPtr->List_brushBlue = brushBlue;
displPtr->List_lineThickness = LineThickness;
    //vector graphics
	if(LoadDxfFile == TRUE){//gunna be a vector display
       displPtr->List_vectorObj = new CEX2DVectObj();
       displPtr->List_vectorObj->LoadDxfFile(uniString);
    return TRUE;
	}//end gunna be a vector display
 
	return TRUE;
}

//deletion mem
void CHudObj::SafeDelete()
{
    if(List_displayUnitList){
	for(POSITION posLoc = List_displayUnitList->GetHeadPosition();posLoc != NULL;){
        CHudDisplayObj * ptrLocal = (CHudDisplayObj *)List_displayUnitList->GetNext(posLoc);
		ptrLocal->SafeDelete();
		delete ptrLocal;
		ptrLocal=0;
	}
    List_displayUnitList->RemoveAll();
	delete List_displayUnitList;
	List_displayUnitList=0;
	}
}

void CHudObj::Serialize( CArchive& ar )
{
int dump = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar  << List_HudCollectionName
		   << List_displayUnitList;
	}
	else
	{
       ar  >> List_HudCollectionName
		   >> List_displayUnitList;  
	}
}

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float CHudObj::MagnitudeSquared(float Mx1,float My1,
									  float Mz1,float Mx2,
									  float My2,float Mz2){
float temp_Mxyz = (((Mx1 - Mx2) * (Mx1 - Mx2)) + ((My1 - My2) * (My1 - My2)) + ((Mz1 - Mz2) * (Mz1 - Mz2)));

return (float)sqrt(temp_Mxyz);
}


CString CHudObj::ConvToString(int number)
{    
              

			  CString temp;
			  temp.Format("%i",number);
	 
			  /*if(number==0)
				 return "0";
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;*/
			  return temp;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------converts float cnvert---------------------------//
CString CHudObj::FloatToStingCnv(float number)
{	
   CString final;
   final.Format("%f",number);
   return final;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//---This Function will determine bounding sphere collision----//
BOOL CHudObj::BoundingSphereCheck(float radius1,float radius2,
								  float x1,float y1,float z1,
								  float x2,float y2,float z2)
{
float MinDist,dist;
MinDist = radius1+radius2;
MinDist = MinDist * MinDist;
dist = (x1-x2)*(x1-x2);
   if(dist <= MinDist)
   {
      dist = dist+(y1-y2)*(y1-y2);
	  if(dist <= MinDist)
	  {
       dist = dist+(z1-z2)*(z1-z2);
	   if(dist <= MinDist)
	   return TRUE;
	  }
   }
return FALSE;
}
//---------------------------------------------------------------------------
// Note the following routine requires the include file <math.h>
//
// ScrnPosHorz is the returned horizontal pixel location
// ScrnPosVert is the returned vertical pixel location
// HieghtRes is the heigth of the view area in pixels
// WidthRes is the width of the view area in pixels
// ClpPlaneLeft is the left position of the cliping plane returned from GetField
// ClpPlaneBottom is the bottom position of the cliping plane returned from GetField
// ClpPlaneRight is the right position of the cliping plane returned from GetField
// ClpPlaneTop is the top position of the cliping plane returned from GetField
// ClpPlaneDist is the distance from the camera to the front clipping plane from GetFront
// My3Dpnt is the 3D point for wich the screen position is desired.
BOOL CHudObj::GetScreenPos(int *ScrnPosHorz, int *ScrnPosVert, int HieghtRes,
                           int WidthRes, double ClpPlaneLeft, double ClpPlaneBottom,
                           double ClpPlaneRight, double ClpPlaneTop,
                           double ClpPlaneDist, D3DVECTOR My3DPnt,float Aspect)
{
 double ScaleFactor;
 double dScrnPosHorz;
 double dScrnPosVert;




 ScaleFactor = ClpPlaneDist/My3DPnt.z;

 dScrnPosHorz = ((My3DPnt.x * ScaleFactor) - ClpPlaneLeft)
              * (WidthRes / (ClpPlaneRight - ClpPlaneLeft));

 dScrnPosVert = (ClpPlaneTop - (My3DPnt.y * ScaleFactor))
              * (HieghtRes / (ClpPlaneTop - ClpPlaneBottom));



 *ScrnPosHorz = ((int)floor(dScrnPosHorz));
 *ScrnPosVert = ((int)floor(dScrnPosVert));
 
 *ScrnPosHorz = (int)(*ScrnPosHorz+(((WidthRes*Aspect)-WidthRes)*.5f));

 if(*ScrnPosHorz > 0 && *ScrnPosHorz < (WidthRes*Aspect) && *ScrnPosVert > 0 && *ScrnPosVert < HieghtRes)
 return TRUE;
 else
 return FALSE;
}
//----------------------------------------------------------------------------
void CHudObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHudObj * spnPtr = (CHudObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}



//---------------------------------------------------------------------------
//serializeGlobals
IMPLEMENT_SERIAL(CHudObj, CObject, 0)
IMPLEMENT_SERIAL(CHudObjList, CObList, 0)
IMPLEMENT_SERIAL(CHudDisplayObj, CObject, 0)
IMPLEMENT_SERIAL(CHudDisplayList, CObList, 0)





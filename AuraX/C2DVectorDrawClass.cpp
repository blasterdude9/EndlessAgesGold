#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
#include "math.h"
#include  <afxtempl.h>

 #include "C2DVectorDrawClass.h"


CEX2DVectObj::CEX2DVectObj()
{
List_xyPointCount = 0;
List_xyPointArray = NULL;
List_centerPointX = 0.0f;
List_centerPointY = 0.0f;
List_radius = 0.0f;
}

void CEX2DVectObj::SafeDelete()
{
if(List_xyPointCount > 0)
delete [] List_xyPointArray;
List_xyPointArray=0;
}

BOOL CEX2DVectObj::Clone(CEX2DVectObj **copy)
{
CEX2DVectObj * copyLocal = new CEX2DVectObj();

*copy = copyLocal;
return TRUE;
}

BOOL CEX2DVectObj::Scale(float size)
{
if(List_radius == 0 || size == 0 || List_xyPointCount == 0)
return FALSE;

float scaler = (size / List_radius);

List_radius = List_radius*scaler;
for(int loop=0;loop<List_xyPointCount;loop++)
	{//loop through polygons
       List_xyPointArray[loop].startX*=scaler;
       List_xyPointArray[loop].startX*=scaler;    
	   List_xyPointArray[loop].startX*=scaler;
       List_xyPointArray[loop].startX*=scaler;
	   List_xyPointArray[loop].endX*=scaler;
       List_xyPointArray[loop].endX*=scaler;    
	   List_xyPointArray[loop].endX*=scaler;
	   List_xyPointArray[loop].endX*=scaler;
	   
	   List_xyPointArray[loop].startY*=scaler;
       List_xyPointArray[loop].startY*=scaler;
	   List_xyPointArray[loop].startY*=scaler;
	   List_xyPointArray[loop].startY*=scaler;
	   List_xyPointArray[loop].endY*=scaler;
       List_xyPointArray[loop].endY*=scaler;    
	   List_xyPointArray[loop].endY*=scaler;
	   List_xyPointArray[loop].endY*=scaler;
	}//end loop through polygons

return TRUE;
}

BOOL CEX2DVectObj::GetCenterAndRadius()
{
if(List_xyPointCount == 0)
   return FALSE;
   float largestX,smallestX,largestY,smallestY,largestZ,smallestZ;    

      int quantity = List_xyPointCount;
      largestX=List_xyPointArray[0].startX;
	  smallestX=List_xyPointArray[0].startX;
	  largestY=List_xyPointArray[0].startY;
	  smallestY=List_xyPointArray[0].startY;
	  largestZ=0.0f;
	  smallestZ=0.0f; 

	  D3DVECTOR frameOrigin;
      frameOrigin.x = 0.0f;
	  frameOrigin.y = 0.0f;
	  frameOrigin.z = 0.0f;

    for(int loop=0;loop<quantity;loop++)
	{//loop through polygons
       if(largestX < List_xyPointArray[loop].startX)
          largestX = List_xyPointArray[loop].startX;     
	   if(smallestX > List_xyPointArray[loop].startX)
		  smallestX = List_xyPointArray[loop].startX;
	   if(largestX < List_xyPointArray[loop].endX)
          largestX = List_xyPointArray[loop].endX;     
	   if(smallestX > List_xyPointArray[loop].endX)
		  smallestX = List_xyPointArray[loop].endX;
	   
	   if(largestY < List_xyPointArray[loop].startY)
          largestY = List_xyPointArray[loop].startY; 
	   if(smallestY > List_xyPointArray[loop].startY)
		  smallestY = List_xyPointArray[loop].startY;
	   if(largestX < List_xyPointArray[loop].endY)
          largestX = List_xyPointArray[loop].endY;     
	   if(smallestX > List_xyPointArray[loop].endY)
		  smallestX = List_xyPointArray[loop].endY;
	}//end loop through polygons

	List_centerPointX = (largestX + smallestX)*.5f;
    List_centerPointY = (largestY + smallestY)*.5f;

	List_radius = MagnitudeSquared(List_centerPointX,
		                                    List_centerPointY,
									        0.0f,
											largestX,
									        largestY,
											0.0f);
    /*//CENTER THEN RECALC
    for(loop=0;loop<List_xyPointCount;loop++)
	{//loop through polygons
       List_xyPointArray[loop].startX-=List_centerPointX;
	   List_xyPointArray[loop].endX-=List_centerPointX;
	   
	   List_xyPointArray[loop].startY-=List_centerPointY;
	   List_xyPointArray[loop].endY-=List_centerPointY;
	}//end loop through polygons
    for(loop=0;loop<quantity;loop++)
	{//loop through polygons
       if(largestX < List_xyPointArray[loop].startX)
          largestX = List_xyPointArray[loop].startX;     
	   if(smallestX > List_xyPointArray[loop].startX)
		  smallestX = List_xyPointArray[loop].startX;
	   if(largestX < List_xyPointArray[loop].endX)
          largestX = List_xyPointArray[loop].endX;     
	   if(smallestX > List_xyPointArray[loop].endX)
		  smallestX = List_xyPointArray[loop].endX;
	   
	   if(largestY < List_xyPointArray[loop].startY)
          largestY = List_xyPointArray[loop].startY; 
	   if(smallestY > List_xyPointArray[loop].startY)
		  smallestY = List_xyPointArray[loop].startY;
	   if(largestX < List_xyPointArray[loop].endY)
          largestX = List_xyPointArray[loop].endY;     
	   if(smallestX > List_xyPointArray[loop].endY)
		  smallestX = List_xyPointArray[loop].endY;
	}//end loop through polygons

	List_centerPointX = (largestX + smallestX)*.5f;
    List_centerPointY = (largestY + smallestY)*.5f;*/

	return TRUE;
}


void CEX2DVectObj::Serialize( CArchive& ar )
{
int dump = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){
		
	   ar  << List_xyPointCount;
	   for(int loop=0;loop<List_xyPointCount;loop++){//store vertex array
       ar  << List_xyPointArray[loop].startX
		   << List_xyPointArray[loop].startY
		   << List_xyPointArray[loop].endX
		   << List_xyPointArray[loop].endY
		   << List_xyPointArray[loop].circV;
	   }//end store vertex array

	}
	else
	{
       ar  >> List_xyPointCount;  
	   if(List_xyPointCount > 0)
       List_xyPointArray = new LINEEXG[List_xyPointCount];
	   for(int loop=0;loop<List_xyPointCount;loop++){//store vertex array
       ar  >> List_xyPointArray[loop].startX
		   >> List_xyPointArray[loop].startY
		   >> List_xyPointArray[loop].endX
		   >> List_xyPointArray[loop].endY
		   >> List_xyPointArray[loop].circV;
	   }//end store vertex array
	}
}
//-------------------Draw IT---------------------------//
BOOL CEX2DVectObj::LoadDxfFile(CString filename)
{
char tempFileName[256];
lstrcpy(tempFileName, filename);
CFile xfile(tempFileName,CFile::modeRead);
int bufferSize = (int)xfile.GetLength( ); 
char *pbuf = new char[bufferSize];
UINT nBytesRead = xfile.Read( pbuf, bufferSize );
CString append;
for(int loopForSize=0;loopForSize<2;loopForSize++){
  if(loopForSize==1)//allocate ?
     List_xyPointArray = new LINEEXG[List_xyPointCount];
int indexTraceCounter = 0;
for(int loop=0;loop<bufferSize;loop++){//loop through buffer
	//GET Vector Data
	//AcDbLine = tag for line
	if(pbuf[loop] == 'A' && 
	   pbuf[loop+1] == 'c' && 
	   pbuf[loop+2] == 'D' && 
	   pbuf[loop+3] == 'b' &&
	   pbuf[loop+4] == 'L' &&
	   pbuf[loop+5] == 'i' &&
	   pbuf[loop+6] == 'n' &&
	   pbuf[loop+7] == 'e' &&
	   pbuf[loop+8] != 't')
	{//found vector data
    loop = loop+7;//skip past garbage
	//ENTERDATA IF U KNOW SIZE
	if(loopForSize == 0)
    List_xyPointCount++;
	else{//Do IT
	while(pbuf[loop] == ' ' || pbuf[loop] == '\n' || pbuf[loop] == '{' || pbuf[loop] == '}')//get next data
	   loop++;
	   loop+=8;
	 for(int indexer=0;indexer<5;indexer++){//loop A   	 
            CString tempBuild;
	        while(pbuf[loop] != ' ' && pbuf[loop] != '\n'){//build float
	          tempBuild = operator+(tempBuild,pbuf[loop]);
	          loop++;
			}//end build float
			if(indexer != 2){//skip Z
	           if(indexer == 0)
				  List_xyPointArray[indexTraceCounter].startX = (float)atof(tempBuild);//RECORD;
			   if(indexer == 1)
				  List_xyPointArray[indexTraceCounter].startY = (float)atof(tempBuild);//RECORD;
			   if(indexer == 3)
				  List_xyPointArray[indexTraceCounter].endX = (float)atof(tempBuild);//RECORD;
			   if(indexer == 4)
				  List_xyPointArray[indexTraceCounter].endY = (float)atof(tempBuild);//RECORD;
			   List_xyPointArray[indexTraceCounter].circV=0.0f;
			}//END skip Z
	     while(pbuf[loop] == ' ' || pbuf[loop] == '\n')//get next data
	     loop++;
		 loop+=4;
	 }//end loop A
	 indexTraceCounter++;
	}//end do it
	}//found vector data
    //HANDLE CIRCLES
	//AcDbCircle = tag for line
	if(pbuf[loop] == 'A' && 
	   pbuf[loop+1] == 'c' && 
	   pbuf[loop+2] == 'D' && 
	   pbuf[loop+3] == 'b' &&
	   pbuf[loop+4] == 'C' &&
	   pbuf[loop+5] == 'i' &&
	   pbuf[loop+6] == 'r' &&
	   pbuf[loop+7] == 'c' &&
	   pbuf[loop+8] == 'l' && 
	   pbuf[loop+9] == 'e')
	{//found circle data
    if(loopForSize == 0)
    List_xyPointCount++;
	else{//Do IT 3
    while(pbuf[loop] == ' ' || pbuf[loop] == '\n' || pbuf[loop] == '{' || pbuf[loop] == '}')//get next data
	   loop++;
	   loop+=17;
       for(int indexer=0;indexer<4;indexer++){//loop A   	 
            CString tempBuild;
	        while(pbuf[loop] != ' ' && pbuf[loop] != '\n'){//build float
	          tempBuild = operator+(tempBuild,pbuf[loop]);
	          loop++;
			}//end build float
	           if(indexer == 0)
				  List_xyPointArray[indexTraceCounter].startX = (float)atof(tempBuild);//RECORD;
			   if(indexer == 1)
				  List_xyPointArray[indexTraceCounter].startY = (float)atof(tempBuild);//RECORD;
			   if(indexer == 3)
				  List_xyPointArray[indexTraceCounter].circV = (float)(atof(tempBuild)*2.0f);//RECORD radius;
				  List_xyPointArray[indexTraceCounter].endX = 0.0f;//Init;
				  List_xyPointArray[indexTraceCounter].endY = 0.0f;//Init;
	     while(pbuf[loop] == ' ' || pbuf[loop] == '\n')//get next data
	     loop++;
		 loop+=4;
	 }//end loop A
	 indexTraceCounter++;
	}//end Do It 2
	}//end found circle data
//RECTANGLE
//AcDbPolyline = filled areas for line
/*	if(pbuf[loop] == 'A' && 
	   pbuf[loop+1] == 'c' && 
	   pbuf[loop+2] == 'D' && 
	   pbuf[loop+3] == 'b' &&
	   pbuf[loop+4] == 'P' &&
	   pbuf[loop+5] == 'o' &&
	   pbuf[loop+6] == 'l' &&
	   pbuf[loop+7] == 'y' &&
	   pbuf[loop+8] == 'l' && 
	   pbuf[loop+9] == 'i' && 
	   pbuf[loop+10] == 'n' && 
	   pbuf[loop+11] == 'e' && 
	   )
	{//found pline data
    if(loopForSize == 0)
    List_xyPointCount++;
	else{//Do IT 3
    while(pbuf[loop] == ' ' || pbuf[loop] == '\n' || pbuf[loop] == '{' || pbuf[loop] == '}')//get next data
	   loop++;
	   loop+=19;
	 indexTraceCounter++;
	}//end Do It 2
	}//end found pline data*/
//END ENTERDATA IF U KNOW SIZE
}//end loop through buffer
}
xfile.Close();
delete [] pbuf;
pbuf=0; 

GetCenterAndRadius();

return TRUE;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float CEX2DVectObj::MagnitudeSquared(float Mx1,float My1,
									  float Mz1,float Mx2,
									  float My2,float Mz2){
float temp_Mxyz = (((Mx1 - Mx2) * (Mx1 - Mx2)) + ((My1 - My2) * (My1 - My2)) + ((Mz1 - Mz2) * (Mz1 - Mz2)));

return (float)sqrt(temp_Mxyz);
}
IMPLEMENT_SERIAL(CEX2DVectObj, CObject, 0)
IMPLEMENT_SERIAL(CEX2DVectObjList, CObList, 0)


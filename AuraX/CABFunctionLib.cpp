//----This Library of functions is the base foundation for a game engine----//
//--CABFunctionLib.cpp / CABFunctionLib.h--MOD---11-24-98-------------------//
//----LIBRARY CREATED BY ARB AND MILLER-------------------------------------//
//----Includes----//
#include "stdafx.h"
#define  D3D_OVERLOADS
#include <afxtempl.h>
#include <math.h>
#include "d3d9.h"
#include "mmsystem.h"
#include "CABFunctionLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//----Functions---//
//-------------------------------------------------------------//
//-----This Function Verifys if a file exists on a CDrom-------//
//-----Used in making sure the CD is in the computer-----------//
//--CURCDROM = CheckForFileOnCDROM("dsetup.dll","\\*.*",FALSE);//
//-------------------------------------------------------------//
CString CABFunctionLib::CheckForFileOnCDROM(CString filename,CString directoryExtended,BOOL Priority)
{
//validate CDROM is present :)
     /*int drive;
    
     for( drive = 1; drive <= 26; drive++ ){ 
          CString letterLoc = (CString)(drive + 'A' - 1);
		  letterLoc = operator+(letterLoc,":");
		     if(GetDriveType(letterLoc) == DRIVE_CDROM){
				 letterLoc = operator+(letterLoc,directoryExtended);
				 if(ValidateFileIsThere(filename,letterLoc) == TRUE){
                    good = TRUE;
                    return letterLoc;
					break;
				 }
			 }
		 }
if(good == FALSE && Priority == TRUE){
   AfxMessageBox("ERROR - CDROM NOT FOUND - REQUIRES CDROM TO RUN");
   exit(1);
}*/
return "NONE";
//end validate CDROM is present
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----This Function Checks for a file in a given directory----//
BOOL CABFunctionLib::ValidateFileIsThere(CString fileName,CString directory)
{
WIN32_FIND_DATA FindData;
HANDLE FileHandle;
DWORD Attrib;
CString name = "";
fileName.MakeUpper();
    FileHandle = FindFirstFile(directory, &FindData);
	do{
	  Attrib = FindData.dwFileAttributes;
	  if(Attrib != 16){//if valid file
	  name = FindData.cFileName;
	  //compare
	  name.MakeUpper();
	  if(fileName == name)
		 return TRUE;
	  }//end if valid file
	} while (FindNextFile(FileHandle, &FindData));
return FALSE;//file or CDROM not there
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//----------this function converts long to cstring-------------//
CString CABFunctionLib::ConvertLongToString(long number)
{    
	          if(number==0)
				 return "0";

			  char buffer[67];

			  _ltoa_s(number,buffer,10);
		
			  return buffer;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//---This Function will determine bounding sphere collision----//
BOOL CABFunctionLib::BoundingSphereCheck(float radius1,float radius2,float x1,float y1,float z1,
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
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//This Function will update a polygons transfore based on matrix//
void CABFunctionLib::UpdatePolygonTransform(float matrixLoc[4][4],D3DVECTOR polygonLoc[3])
{
D3DVECTOR tempP[3];
tempP[0].x = (matrixLoc[0][0]*polygonLoc[0].x)+(matrixLoc[1][0]*polygonLoc[0].y)+(matrixLoc[2][0]*polygonLoc[0].z)+(matrixLoc[3][0] * 1);
tempP[0].y = (matrixLoc[0][1]*polygonLoc[0].x)+(matrixLoc[1][1]*polygonLoc[0].y)+(matrixLoc[2][1]*polygonLoc[0].z)+(matrixLoc[3][1] * 1);
tempP[0].z = (matrixLoc[0][2]*polygonLoc[0].x)+(matrixLoc[1][2]*polygonLoc[0].y)+(matrixLoc[2][2]*polygonLoc[0].z)+(matrixLoc[3][2] * 1);

tempP[1].x = (matrixLoc[0][0]*polygonLoc[1].x)+(matrixLoc[1][0]*polygonLoc[1].y)+(matrixLoc[2][0]*polygonLoc[1].z)+(matrixLoc[3][0] * 1);
tempP[1].y = (matrixLoc[0][1]*polygonLoc[1].x)+(matrixLoc[1][1]*polygonLoc[1].y)+(matrixLoc[2][1]*polygonLoc[1].z)+(matrixLoc[3][1] * 1);
tempP[1].z = (matrixLoc[0][2]*polygonLoc[1].x)+(matrixLoc[1][2]*polygonLoc[1].y)+(matrixLoc[2][2]*polygonLoc[1].z)+(matrixLoc[3][2] * 1);

tempP[2].x = (matrixLoc[0][0]*polygonLoc[2].x)+(matrixLoc[1][0]*polygonLoc[2].y)+(matrixLoc[2][0]*polygonLoc[2].z)+(matrixLoc[3][0] * 1);
tempP[2].y = (matrixLoc[0][1]*polygonLoc[2].x)+(matrixLoc[1][1]*polygonLoc[2].y)+(matrixLoc[2][1]*polygonLoc[2].z)+(matrixLoc[3][1] * 1);
tempP[2].z = (matrixLoc[0][2]*polygonLoc[2].x)+(matrixLoc[1][2]*polygonLoc[2].y)+(matrixLoc[2][2]*polygonLoc[2].z)+(matrixLoc[3][2] * 1);

polygonLoc[0].x = tempP[0].x;
polygonLoc[0].y = tempP[0].y;
polygonLoc[0].z = tempP[0].z;
polygonLoc[1].x = tempP[1].x;
polygonLoc[1].y = tempP[1].y;
polygonLoc[1].z = tempP[1].z;
polygonLoc[2].x = tempP[2].x;
polygonLoc[2].y = tempP[2].y;
polygonLoc[2].z = tempP[2].z;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//This Function will update a vertex transfore based on matrix-//
D3DVECTOR CABFunctionLib::UpdatePolygonVertex(float matrixLoc[4][4],D3DVECTOR polygonLoc)
{
D3DVECTOR tempP;
tempP.x = (matrixLoc[0][0]*polygonLoc.x)+(matrixLoc[1][0]*polygonLoc.y)+(matrixLoc[2][0]*polygonLoc.z)+(matrixLoc[3][0] * 1);
tempP.y = (matrixLoc[0][1]*polygonLoc.x)+(matrixLoc[1][1]*polygonLoc.y)+(matrixLoc[2][1]*polygonLoc.z)+(matrixLoc[3][1] * 1);
tempP.z = (matrixLoc[0][2]*polygonLoc.x)+(matrixLoc[1][2]*polygonLoc.y)+(matrixLoc[2][2]*polygonLoc.z)+(matrixLoc[3][2] * 1);

polygonLoc.x = tempP.x;
polygonLoc.y = tempP.y;
polygonLoc.z = tempP.z;
return polygonLoc;
}


//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//This Function will update a vertex transfore based on matrix Rotation only-//
D3DVECTOR CABFunctionLib::UpdatePolygonVertexRotationOnly(float matrixLoc[4][4],D3DVECTOR polygonLoc)
{
D3DVECTOR tempP;
tempP.x = (matrixLoc[0][0]*polygonLoc.x)+(matrixLoc[1][0]*polygonLoc.y)+(matrixLoc[2][0]*polygonLoc.z);
tempP.y = (matrixLoc[0][1]*polygonLoc.x)+(matrixLoc[1][1]*polygonLoc.y)+(matrixLoc[2][1]*polygonLoc.z);
tempP.z = (matrixLoc[0][2]*polygonLoc.x)+(matrixLoc[1][2]*polygonLoc.y)+(matrixLoc[2][2]*polygonLoc.z);

polygonLoc.x = tempP.x;
polygonLoc.y = tempP.y;
polygonLoc.z = tempP.z;
return polygonLoc;
}

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//---This Function will return the cross product---------------//
/*D3DVECTOR CABFunctionLib::CrossProduct (const D3DVECTOR& v1, const D3DVECTOR& v2)
{
    D3DVECTOR result;

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;

    return result;
}*/
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//---This Function will return the cross product---------------//
float CABFunctionLib::CrossProductFloat (float v1, float v2)
{
    float result;

    result = v1 * v2 - v1 * v2;
   
    return result;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-This Function will determine the angle between two vectors--//
float CABFunctionLib::ContactAngle(D3DVECTOR OriginOfShipNormal, D3DVECTOR TerminusOfShipNormal,D3DVECTOR OriginOfWallNormal, D3DVECTOR TerminusOfWallNormal)
{
 double AngleBetweenVectors;

 AngleBetweenVectors =
	   acos( ( (TerminusOfShipNormal.x - OriginOfShipNormal.x)
			  *(TerminusOfWallNormal.x - OriginOfWallNormal.x))
			+( (TerminusOfShipNormal.y - OriginOfShipNormal.y)
			  *(TerminusOfWallNormal.y - OriginOfWallNormal.y))
			+( (TerminusOfShipNormal.z - OriginOfShipNormal.z)
			  *(TerminusOfWallNormal.z - OriginOfWallNormal.z)));

 float degrees = (float)((AngleBetweenVectors * 180)/3.14159265359);

 return degrees;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--This Function is a component of ExactCollision()-----------//
float CABFunctionLib::WherePt(D3DVECTOR polygon[3],D3DVECTOR ptloc)
{
float a[3],b[3];
float zTest=0.0f;
a[0] = polygon[1].x - polygon[0].x;
a[1] = polygon[1].y - polygon[0].y;
a[2] = polygon[1].z - polygon[0].z;
b[0] = polygon[2].x - polygon[0].x;
b[1] = polygon[2].y - polygon[0].y;
b[2] = polygon[2].z - polygon[0].z;




if((a[0] != 0.0f) && ((b[0] * a[2] - b[2]*a[0]) != 0.0f))
{
      zTest = polygon[0].y - ptloc.y
              +((b[1]*a[0] - b[0]*a[1])/(b[0]*a[2] - b[2]*a[0]))
              *(polygon[0].z - ptloc.z)
              +((a[1]/a[0])+(b[1]*a[0]*a[2]-b[0]*a[1]*a[2])
              /(a[0]*(b[0]*a[2]-b[2]*a[0])))*(ptloc.x-polygon[0].x);
	  return zTest;
}

if((a[2] != 0.0f) && ((b[2]*a[0] - b[0]*a[2])!= 0.0f))
{
zTest = (polygon[0].y - ptloc.y)
        +((b[1]*a[2] - b[2]*a[1])/(b[2]*a[0] - b[0] * a[2])) * (polygon[0].x - ptloc.x)
		+((a[1]/a[2])+(b[1]*a[0]*a[2] - b[2]*a[0]*a[1])
		/(a[2] *(b[2]*a[0] - b[0]*a[2]))) * (ptloc.z - polygon[0].z);

      return zTest;
}


if((a[1] != 0.0f)&&((b[1]*a[2]-b[2]*a[1]) != 0.0f))
{
zTest = (polygon[0].x - ptloc.x) + (( a[0]/a[1])+(b[0]*a[2]*a[1]-b[1]*a[2]*a[0]) / 
		(a[1] * (b[1] * a[2] -b[2] * a[1]))) * (ptloc.y-polygon[0].y) +
		((b[0]*a[1]-b[1]* a[0])/(b[1]*a[2]-b[2]*a[1]))
		*(polygon[0].z-ptloc.z);


      return zTest;
}


if(((b[2]*a[1]-b[1]*a[2])!=0.0f)&&(a[2]!=0.0f))
{
zTest = (polygon[0].x-ptloc.x)+((b[0]*a[2]-b[2]*a[0])
								/(b[2]*a[1]-b[1]*a[2]))*(polygon[0].y-ptloc.y)+
								((a[0]/a[2])+(b[0]*a[1]*a[2]-b[2]*a[1]*a[0])/
								(a[2]*(a[1]*b[2]-b[1]*a[2])))*(ptloc.z-polygon[0].z);

      return zTest;
}

if(( a[0] != 0.0f )&&((b[0]*a[1]-b[1]*a[0])!= 0.0f))
{
	zTest = (polygon[0].z - ptloc.z)
	        +(( b[2]*a[0] - b[0]*a[2])/(b[0]*a[1]-b[1]*a[0]))
			*(polygon[0].y - ptloc.y)
			+((a[2]/a[0]) + (b[2]*a[1]*a[0]-b[0]*a[1]*a[2])
			/(a[0]*(b[0]*a[1] - b[1]*a[0]))) 
			*(ptloc.x - polygon[0].x);
	  return zTest;
}

if((a[1]!=0.0f)&&((b[1]*a[0]-b[0]*a[1]) != 0.0f))
{
    zTest = polygon[0].z - ptloc.z
		    + ((a[2]/a[1])+(b[2]*a[0]*a[1] - b[1]*a[0]*a[2])
			/ (a[1]*(b[1]*a[0]-b[0]*a[1])))*(ptloc.y-polygon[0].y)
			+((b[2]*a[0] - b[0]*a[2])/(b[1]*a[0]-b[0]*a[1]))
			*(polygon[0].x - ptloc.x);
	  return zTest;
}



return zTest;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//This Function is a component of exact collision--------------//
void CABFunctionLib::FindInterPt(D3DVECTOR polygon[3],D3DVECTOR pt1,D3DVECTOR pt2,D3DVECTOR * interPtLoc)
{
float a[3][3],z[3],deta,t;

a[0][0] = polygon[1].x - polygon[0].x;
a[1][0] = polygon[1].y - polygon[0].y;
a[2][0] = polygon[1].z - polygon[0].z;
a[0][1] = polygon[2].x - polygon[0].x;
a[1][1] = polygon[2].y - polygon[0].y;
a[2][1] = polygon[2].z - polygon[0].z;
a[0][2] = pt1.x - pt2.x;
a[1][2] = pt1.y - pt2.y;
a[2][2] = pt1.z - pt2.z;
z[0] = pt1.x - polygon[0].x;
z[1] = pt1.y - polygon[0].y;
z[2] = pt1.z - polygon[0].z;


deta = (a[0][0]*a[1][1]*a[2][2])+
       (a[0][1]*a[1][2]*a[2][0])+
       (a[0][2]*a[1][0]*a[2][1])-
	   (a[0][0]*a[1][2]*a[2][1])-
	   (a[0][1]*a[1][0]*a[2][2])-
	   (a[0][2]*a[1][1]*a[2][0]);


t = (1 / deta)*(((a[1][0]*a[2][1] - a[1][1]*a[2][0])*z[0])
	-((a[0][0]*a[2][1] - a[0][1]*a[2][0])*z[1])
	+((a[0][0]*a[1][1] - a[0][1]*a[1][0])*z[2]));

interPtLoc->x = pt1.x - t * a[0][2];
interPtLoc->y = pt1.y - t * a[1][2];
interPtLoc->z = pt1.z - t * a[2][2];
return;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//This Function is a component of exact collision--------------//
BOOL CABFunctionLib::SegOverlap(D3DVECTOR pt1,D3DVECTOR pt2,D3DVECTOR pt3,D3DVECTOR pt4)
{
float t1,t2;
    if((pt2.x - pt1.x)!= 0.0f){
	    t1 = (pt3.x - pt1.x)/(pt2.x - pt1.x);
        t2 = (pt4.x - pt1.x)/(pt2.x - pt1.x);
	}
	else if((pt2.y-pt1.y)!=0.0f){
		t1 = (pt3.y - pt1.y)/(pt2.y - pt1.y);
		t2 = (pt4.y - pt1.y)/(pt2.y - pt1.y);
	}
	else if((pt2.z-pt1.z)!=0.0f){
        t1 = (pt3.z - pt1.z)/(pt2.z - pt1.z);
		t2 = (pt4.z - pt1.z)/(pt2.z - pt1.z);
	}
	else
	return FALSE;


    if( ((t1 < 0)&&(t2<0)) || ((t1>1)&&(t2>1)))
        return FALSE;

    return TRUE;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----This function determines Exact collision between two----//
//-----Polygons - also will get exact location of intersection-//
//-----so you could have somthing happen there-----------------//
BOOL CABFunctionLib::ExactColision(D3DVECTOR polygon1[3],D3DVECTOR polygon2[3],float *avgX,float *avgY,float *avgZ)
{
    D3DVECTOR interPtLoc1,interPtLoc2,interPtLoc3,interPtLoc4;
	
	float relativePtLoc1 = WherePt(polygon1,polygon2[0]);
	float relativePtLoc2 = WherePt(polygon1,polygon2[1]);  
	float relativePtLoc3 = WherePt(polygon1,polygon2[2]);
	
	if((relativePtLoc1==0.0f)&&(relativePtLoc2==0.0f)&&(relativePtLoc3==0.0f))
	   return FALSE;

    if((relativePtLoc1==0.0f)||(relativePtLoc2==0.0f)||(relativePtLoc3==0.0f))
	   return FALSE;

	if(((relativePtLoc1>0.0f)&&(relativePtLoc2>0.0f)&&(relativePtLoc3>0.0f)) ||
	  ((relativePtLoc1<0.0f)&&(relativePtLoc2<0.0f)&&(relativePtLoc3<0.0f)))
      return FALSE;		   

    float relativePtLoc4 = WherePt(polygon2,polygon1[0]);
	float relativePtLoc5 = WherePt(polygon2,polygon1[1]);  
	float relativePtLoc6 = WherePt(polygon2,polygon1[2]);

    if(((relativePtLoc4>0.0f)&&(relativePtLoc5>0.0f)&&(relativePtLoc6>0.0f))||
	   ((relativePtLoc4<0.0f)&&(relativePtLoc5<0.0f)&&(relativePtLoc6<0.0f)))
	   return FALSE;

	if(((relativePtLoc1>0.0f)&&(relativePtLoc2>0.0f))||
	   ((relativePtLoc1<0.0)&&(relativePtLoc2<0.0f)))
    {
       FindInterPt(polygon1,polygon2[2],polygon2[0],&interPtLoc1);
       FindInterPt(polygon1,polygon2[2],polygon2[1],&interPtLoc2);
	}
    else if(((relativePtLoc1>0.0f)&&(relativePtLoc3>0.0f))||
		    ((relativePtLoc1<0.0f)&&(relativePtLoc3<0.0f)))
	{
       FindInterPt(polygon1,polygon2[1],polygon2[0],&interPtLoc1);
       FindInterPt(polygon1,polygon2[1],polygon2[2],&interPtLoc2);
	}
    else
	{
       FindInterPt(polygon1,polygon2[0],polygon2[1],&interPtLoc1);
       FindInterPt(polygon1,polygon2[0],polygon2[2],&interPtLoc2);
	}
    if(((relativePtLoc4>0.0f)&&(relativePtLoc5>0.0f))||
	   ((relativePtLoc4<0.0f)&&(relativePtLoc5<0.0f)))
	{
       FindInterPt(polygon2,polygon1[2],polygon1[0],&interPtLoc3);
       FindInterPt(polygon2,polygon1[2],polygon1[1],&interPtLoc4);
	}
    else if(((relativePtLoc4>0.0f)&&(relativePtLoc6>0.0f))||
		    ((relativePtLoc4<0.0f)&&(relativePtLoc6<0.0f)))
	{
       FindInterPt(polygon2,polygon1[1],polygon1[0],&interPtLoc3);
       FindInterPt(polygon2,polygon1[1],polygon1[2],&interPtLoc4);
	}
    else
	{
       FindInterPt(polygon2,polygon1[0],polygon1[1],&interPtLoc3);
       FindInterPt(polygon2,polygon1[0],polygon1[2],&interPtLoc4);
	}
//--Find exact intersection XYZ 
	if(avgX != NULL){
      *avgX = ((interPtLoc1.x + interPtLoc2.x)*.5f);
	}
	if(avgY != NULL){
      *avgY = ((interPtLoc1.y + interPtLoc2.y)*.5f);
	}
	if(avgZ != NULL){
      *avgZ = ((interPtLoc1.z + interPtLoc2.z)*.5f);
	}

return SegOverlap(interPtLoc1,interPtLoc2,interPtLoc3,interPtLoc4);
 
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--This Function gets the maximum radius of a polygon---------//
float CABFunctionLib::GetRadius(D3DVECTOR center,D3DVECTOR vert1,D3DVECTOR vert2,D3DVECTOR vert3)
{
float radius1 = MagnitudeSquared(center.x,
								 center.y,
				                 center.z,
				                 vert2.x,
				                 vert2.y,
				                 vert2.z);
float radius2 = MagnitudeSquared(center.x,
								 center.y,
				                 center.z,
				                 vert3.x,
				                 vert3.y,
				                 vert3.z);
float radius3 = MagnitudeSquared(center.x,
								 center.y,
				                 center.z,
				                 vert1.x,
				                 vert1.y,
				                 vert1.z);

if(radius1>=radius2){
	if(radius1>=radius3){
        return (radius1);
}}
if(radius2>=radius1){
	if(radius2>=radius3){
        return (radius2);
}}

if(radius3>=radius1){
	if(radius3>=radius2){
        return (radius3);
}}
return 0;

}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float CABFunctionLib::MagnitudeSquared(float Mx1,float My1,
									  float Mz1,float Mx2,
									  float My2,float Mz2){
float temp_Mxyz = (((Mx1 - Mx2) * (Mx1 - Mx2)) + ((My1 - My2) * (My1 - My2)) + ((Mz1 - Mz2) * (Mz1 - Mz2)));

return (float)sqrt(temp_Mxyz);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float CABFunctionLib::MagnitudeSquared2(float Mx1,float My1,
									    float Mz1,float Mx2,
									    float My2,float Mz2){
float temp_Mxyz = (((Mx1 - Mx2) * (Mx1 - Mx2)) + ((My1 - My2) * (My1 - My2)) + ((Mz1 - Mz2) * (Mz1 - Mz2)));

return (float)GL_matrixFunctions.sqrt_5((unsigned long)temp_Mxyz);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//---this function will calculate the new position of your-----//
//---vehicle/person after a colision - supports multi poly-----//
//---correct adjustment----------Max 20 polys colid------------//
D3DVECTOR CABFunctionLib::GetNewPositionAfterCollision(int howManyNormals,D3DVECTOR storedNormals[20],float storedDistance[20])
{
	D3DVECTOR translation,currentNormal;
	translation.x=0.0f;
	translation.y=0.0f;
	translation.z=0.0f;
	currentNormal.x=0.0f;
	currentNormal.y=0.0f;
	currentNormal.z=0.0f;
	float currentDistance = 0.0f;
	int howManyDifferentNormals = 0;

    if(howManyNormals > 1)
	 for(int loop=0;loop<howManyNormals;loop++){//normal loop
	     BOOL noDuplicateFound = TRUE;
		 BOOL validLoop = FALSE;
         for(int index=loop;index<howManyNormals;index++){//normal loop2
		      if(loop != index)
			  {
				 validLoop = TRUE;
			     if(storedNormals[loop].x == storedNormals[index].x && storedNormals[loop].y == storedNormals[index].y && storedNormals[loop].z == storedNormals[index].z)
				 {
                    noDuplicateFound = FALSE;
				 }
			  }
		 }//end normal loop2
		    if(noDuplicateFound == TRUE && validLoop == TRUE)
			howManyDifferentNormals++;
	 }//end normal loop



     if(howManyDifferentNormals == 0){//if you only hit one
        translation.x =  (storedDistance[0] + .001f) * storedNormals[0].x;
        translation.y =  (storedDistance[0] + .001f) * storedNormals[0].y;
        translation.z =  (storedDistance[0] + .001f) * storedNormals[0].z;
        return translation;
	 }//end if you only hit one
	 else{//if you colised into multi polygons
		for(int loop=0;loop<howManyNormals;loop++){//normal loop
              currentNormal.x += storedNormals[loop].x;
              currentNormal.y += storedNormals[loop].y;
              currentNormal.z += storedNormals[loop].z;
	          currentDistance += storedDistance[loop];
	    }//end normal loop
     
        float scale = (float)sqrt((currentNormal.x * currentNormal.x) + (currentNormal.y * currentNormal.y) + (currentNormal.z * currentNormal.z));
        currentNormal.x = currentNormal.x / scale;
        currentNormal.y = currentNormal.y / scale;
        currentNormal.z = currentNormal.z / scale;

        currentDistance = currentDistance / howManyNormals;
        translation.x =  (currentDistance + .001f) * currentNormal.x;
        translation.y =  (currentDistance + .001f) * currentNormal.y;
        translation.z =  (currentDistance + .001f) * currentNormal.z;
		return translation;
	 }//end if you collided into multipolygons
	 return translation;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----------calculate distance of transform based-------------//
//----------------on norm on ship and poly---------------------//
float CABFunctionLib::SlideTranslation( D3DVECTOR Normal, D3DVECTOR NormalOrigin,
									   D3DVECTOR Polygon[3])
{
 float dist1,dist2,dist3;

 dist1 = (Polygon[0].x - NormalOrigin.x) * Normal.x
       + (Polygon[0].y - NormalOrigin.y) * Normal.y
       + (Polygon[0].z - NormalOrigin.z) * Normal.z;

 dist2 = (Polygon[1].x - NormalOrigin.x) * Normal.x
       + (Polygon[1].y - NormalOrigin.y) * Normal.y
       + (Polygon[1].z - NormalOrigin.z) * Normal.z;

 dist3 = (Polygon[2].x - NormalOrigin.x) * Normal.x
       + (Polygon[2].y - NormalOrigin.y) * Normal.y
       + (Polygon[2].z - NormalOrigin.z) * Normal.z;


 if ((dist1 <= dist2) && (dist1 <= dist3))
  return (-dist1);
 if ((dist2 <= dist1) && (dist2 <= dist3))
  return (-dist2);
return (-dist3);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----the function gets the center of a polygon---------------//
D3DVECTOR CABFunctionLib::FindCenter( D3DVECTOR P1, D3DVECTOR P2, D3DVECTOR P3 )
{
 D3DVECTOR P4;
 D3DVECTOR P5;
 D3DVECTOR PC;
 float div;
 float parametric;
 PC.x = 0.0f;
 PC.y = 0.0f;
 PC.z = 0.0f;

 P4.x = (P1.x + P2.x)/2.0f;
 P4.y = (P1.y + P2.y)/2.0f;
 P4.z = (P1.z + P2.z)/2.0f;
 P5.x = (P1.x + P3.x)/2.0f;
 P5.y = (P1.y + P3.y)/2.0f;
 P5.z = (P1.z + P3.z)/2.0f;

 div = P5.x - P2.x - P4.x + P3.x;
 if (div != 0.0)
 {
  parametric = (P3.x - P2.x) / div;
  PC.x = P2.x + (parametric * (P5.x - P2.x));
  PC.y = P2.y + (parametric * (P5.y - P2.y));
  PC.x = P2.z + (parametric * (P5.z - P2.z));
  return (PC);
 }
 else
 {
  div = P5.y - P2.y - P4.y + P3.y;
  if (div != 0.0)
  {
   parametric = (P3.y - P2.y) / div;
   PC.x = P2.x + (parametric * (P5.x - P2.x));
   PC.y = P2.y + (parametric * (P5.y - P2.y));
   PC.x = P2.z + (parametric * (P5.z - P2.z));
   return (PC);
  }
  else
  {
   div = P5.z - P2.z - P4.z + P3.z;
   if (div != 0.0)
   {
    parametric = (P3.z - P2.z) / div;
    PC.x = P2.x + (parametric * (P5.x - P2.x));
    PC.y = P2.y + (parametric * (P5.y - P2.y));
    PC.x = P2.z + (parametric * (P5.z - P2.z));
    return (PC);
   }
  }
 }
 return (PC);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CABFunctionLib::ConvToString(int number)
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
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----add a new file ext--------------------------------------//
void CABFunctionLib::AddNewFileExt(CString *currentName,CString newExt)
{
CString tempName = *currentName;
CString builtName;
int length = tempName.GetLength();
BOOL done = FALSE;
 for(int loop=0;loop<length;loop++){     
  CString chr = (CString)tempName.GetAt(loop);
  builtName = operator+( builtName, chr);  
  if(chr == ".")
	 break;
 }
builtName = operator+(builtName,newExt);
*currentName = builtName;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----remove a new file ext-----------------------------------//
void CABFunctionLib::RemoveFileExt(CString *currentName)
{
CString tempName = *currentName;
CString builtName;
int length = tempName.GetLength();
BOOL done = FALSE;
 for(int loop=0;loop<length;loop++){     
  CString chr = (CString)tempName.GetAt(loop);
  if(chr == ".")
	 break;
  builtName = operator+( builtName, chr);  
 }
*currentName = builtName;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----Get File Ext--------------------------------------------//
CString CABFunctionLib::GetFileExt(CString currentName)
{
CString ext;
int length = currentName.GetLength();
if(length == 0)
   return "";

BOOL done = FALSE;
 while(done == FALSE){
  CString chr = (CString)currentName.GetAt(length-1);
  
  if(chr == ".")
	 break;

  ext = operator+(chr,ext);
  length = length -1;
  
  if(length == 0)
  break;
 }
return ext;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----Get File Ext--------------------------------------------//
CString CABFunctionLib::FloatToStingCnv(float number)
{	
   CString final;
   final.Format("%f",number);
   return final;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----Is the point in rect------------------------------------//
BOOL CABFunctionLib::IsPointInRect(RECT rect, int pX,int pY)
{
if(rect.left < pX && rect.right > pX && rect.top < pY && rect.bottom > pY)
return TRUE; 

return FALSE;
}


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------get matrix conversion----------------------------------------------------//
/*D3DVECTOR CABFunctionLib::Normalize (const _D3DVECTOR& v)
{
   return v / Magnitude(v);
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------get matrix conversion----------------------------------------------------//
/*D3DVECTOR CABFunctionLib::Magnitude (const _D3DVECTOR& v)
{
   return (D3DVALUE) sqrt(SquareMagnitude(v));
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------get matrix conversion----------------------------------------------------//
/*D3DVECTOR CABFunctionLib::CrossProduct (const _D3DVECTOR& v1, const _D3DVECTOR& v2)
{
    _D3DVECTOR result;
 
    result[0] = v1[1] * v2[2] - v1[2] * v2[1];
    result[1] = v1[2] * v2[0] - v1[0] * v2[2];
    result[2] = v1[0] * v2[1] - v1[1] * v2[0];
 
    return result;
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------append write to cfile----------------------------------------------------//
void CABFunctionLib::WriteToCArchive(int dataType,
								     CArchive * fileLoc,
								     CString stringLoc,
								     int intLoc,
								     float floatLoc)//0=text 1=int 2=float
{
    if(dataType == 0)
    fileLoc->WriteString(stringLoc);
	if(dataType == 1)
    fileLoc->WriteString(ConvToString(intLoc));
	if(dataType == 2)
    fileLoc->WriteString(FloatToStingCnv(floatLoc));
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------get random point in radius--------------------------------------------//
D3DVECTOR CABFunctionLib::GetRandomPointInRadius(float radius,D3DVECTOR origin,float minimum)
{
      D3DVECTOR locationBuild;
	  locationBuild.x = 0.0f;
	  locationBuild.y = 0.0f;
	  locationBuild.z = 0.0f;
	  float matrixDistloc[4][4];
	  float matrixOriginloc[4][4];
	  float matrixRatationloc[4][4];
      float RadiusDist = (float)( rand()%((int)radius) );
	  if(RadiusDist < minimum)
		 RadiusDist = minimum;
      float RotationDist =(float)( rand()%360 );
	  GL_matrixFunctions.Translate ( origin.x,origin.y,origin.z,matrixOriginloc );//origin of roam area
	  GL_matrixFunctions.Translate ( 0.0f,0.0f,RadiusDist,matrixDistloc );//z dist
      GL_matrixFunctions.Rotate ( 0.0f,RotationDist,0.0f,matrixRatationloc );//yaw rotation
      //end get new location to travel to in radius
      //move point
	  locationBuild = UpdatePolygonVertex(matrixDistloc ,locationBuild);//translation
      locationBuild = UpdatePolygonVertex(matrixRatationloc ,locationBuild);//rotation
	  locationBuild = UpdatePolygonVertex(matrixOriginloc ,locationBuild);//offset from origin
      return locationBuild;
}


//-----------------------------------------------------------------------------
//Name: OutOfFov()
//Desc: Checks to see if the point is out of view
//-----------------------------------------------------------------------------
BOOL CABFunctionLib::OutOfFov(float camFov,D3DVECTOR vectLoc)
{
	float wDist;
	if(vectLoc.z == 0.0f)
	return FALSE;
	vectLoc.x = fabs(vectLoc.x);
	vectLoc.y = fabs(vectLoc.y);

	//AfxMessageBox(FloatToStingCnv(vectLoc.x));
	if(vectLoc.x > vectLoc.y)
		wDist = vectLoc.x;
	else
		wDist = vectLoc.y;

	if(wDist == 0.0f)
	return FALSE;
	//float angle = fabs(sin(wDist/vectLoc.z));
	float angle = fabs(atan(wDist/vectLoc.z));

	if(angle > (camFov*.65f))
		return TRUE;
	else
		return FALSE;
}
//-----------------------------------------------------------------------------
//Name: Zcheck()
//Desc: Checks to see if the particle is behind the view
//-----------------------------------------------------------------------------
double CABFunctionLib::RandFlotingPoint(double variable)
{
   int focus = (int)(variable * 100.0);
   int resultRand = rand()%focus;
   return ((double)resultRand)/100.0;
}
//-----------------------------------------------------------------------------
//Name: Zcheck()
//Desc: Checks to see if the particle is behind the view
//-----------------------------------------------------------------------------
BOOL CABFunctionLib::Zcheck(D3DMATRIX matrixLoc,D3DVECTOR point,float fov)
{
	D3DVECTOR zeroedPos;

		zeroedPos.x = point.x - matrixLoc.m[3][0];
		zeroedPos.y = point.y - matrixLoc.m[3][1];
		zeroedPos.z = point.z - matrixLoc.m[3][2];

	D3DVECTOR tempFinal;
	D3DMATRIX invMatrix;
	GL_matrixFunctions.TrnsfrMtrxInvrs(matrixLoc.m,invMatrix.m);
	//D3DMath_MatrixInvert(invMatrix,matrixLoc);
	tempFinal.z = (invMatrix.m[0][2]*zeroedPos.x)+(invMatrix.m[1][2]*zeroedPos.y)+(invMatrix.m[2][2]*zeroedPos.z);

    if(tempFinal.z < 0){
		return FALSE;
	}
	else{
		tempFinal.x = (invMatrix.m[0][0]*zeroedPos.x)+(invMatrix.m[1][0]*zeroedPos.y)+(invMatrix.m[2][0]*zeroedPos.z);
		tempFinal.y = (invMatrix.m[0][1]*zeroedPos.x)+(invMatrix.m[1][1]*zeroedPos.y)+(invMatrix.m[2][1]*zeroedPos.z);
		if(OutOfFov(fov,tempFinal)==TRUE)
			return FALSE;
		else
			return TRUE;
	}

}

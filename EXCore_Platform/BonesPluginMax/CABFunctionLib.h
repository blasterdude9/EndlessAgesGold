#include "matrixarb.h"
#ifndef CABFUNCTIONLIB_H
#define CABFUNCTIONLIB_H

//LIB STRUCTURES



//LIB CLASS
class  CABFunctionLib
{

public:
MatrixARB GL_matrixFunctions;
//--CURCDROM = CheckForFileOnCDROM("dsetup.dll","\\*.*",FALSE);//--provides CD verification/Lock
CString CheckForFileOnCDROM(CString filename,CString directoryExtended,BOOL Priority);
//-----This Function Checks for a file in a given directory----//
BOOL ValidateFileIsThere(CString fileName,CString directory);
//----------this function converts long to cstring-------------//
CString ConvertLongToString(long number);
//---This Function will determine bounding sphere collision----//
BOOL BoundingSphereCheck(float radius1,float radius2,float x1,float y1,float z1,float x2,float y2,float z2);
//---This Function will return the cross product---------------//
//D3DVECTOR CrossProduct (const D3DVECTOR& v1, const D3DVECTOR& v2);
float CrossProductFloat (float v1, float v2);
//-This Function will determine the angle between two vectors--//
float ContactAngle(D3DVECTOR OriginOfShipNormal, D3DVECTOR TerminusOfShipNormal,D3DVECTOR OriginOfWallNormal, D3DVECTOR TerminusOfWallNormal);
//This Function will update a polygons transfore based on matrix//
void UpdatePolygonTransform(float matrixLoc[4][4],D3DVECTOR polygonLoc[3]);
//This Function will update a vertex transfore based on matrix-//
D3DVECTOR UpdatePolygonVertex(float matrixLoc[4][4],D3DVECTOR polygonLoc);
D3DVECTOR UpdatePolygonVertexRotationOnly(float matrixLoc[4][4],D3DVECTOR polygonLoc);
//Collision Functions------------------------------------------//
float WherePt(D3DVECTOR polygon[3],D3DVECTOR ptloc);
void  FindInterPt(D3DVECTOR polygon[3],D3DVECTOR pt1,D3DVECTOR pt2,D3DVECTOR * interPtLoc);
BOOL SegOverlap(D3DVECTOR pt1,D3DVECTOR pt2,D3DVECTOR pt3,D3DVECTOR pt4);
BOOL ExactColision(D3DVECTOR polygon1[3],D3DVECTOR polygon2[3],float *avgX,float *avgY,float *avgZ);
//-------------------------------------------------------------//
//-------Function gets the max radius of a polygon-------------//
float GetRadius(D3DVECTOR center,D3DVECTOR vert1,D3DVECTOR vert2,D3DVECTOR vert3);
//----gets the distance between two 3D points------------------//
float MagnitudeSquared(float Mx1,float My1,float Mz1,float Mx2,float My2,float Mz2);
float MagnitudeSquared2(float Mx1,float My1,float Mz1,float Mx2,float My2,float Mz2);
//----Caculate slide translation based on colision-------------//
D3DVECTOR GetNewPositionAfterCollision(int howManyNormals,D3DVECTOR storedNormals[20],float storedDistance[20]);
//calculate distance of transform based on norm on ship and poly//
float SlideTranslation( D3DVECTOR Normal, D3DVECTOR NormalOrigin,D3DVECTOR Polygon[3]);
//----get center of polygon-------------------------------------//
D3DVECTOR FindCenter( D3DVECTOR P1, D3DVECTOR P2, D3DVECTOR P3 );
//-----converts a long into string-----------------------------//
CString ConvToString(int number);
//-----converts a float into string-----------------------------//
CString FloatToStingCnv(float number);
//-----add a new file ext--------------------------------------//
void AddNewFileExt(CString *currentName,CString newExt);
//-----remove a new file ext-----------------------------------//
void RemoveFileExt(CString *currentName);
CString GetFileExt(CString currentName);
//-----play CD Track-------------------------------------------//
//-----is the point in the rect--------------------------------//
BOOL IsPointInRect(RECT rect, int pX,int pY);
//--
BOOL OutOfFov(float camFov,D3DVECTOR vectLoc);
D3DVECTOR GetRandomPointInRadius(float radius,D3DVECTOR origin,float minimum);
BOOL Zcheck(D3DMATRIX matrixLoc,D3DVECTOR point,float fov);


void WriteToCArchive(int dataType,
								     CArchive * fileLoc,
								     CString stringLoc,
								     int intLoc,
								     float floatLoc);

double RandFlotingPoint(double variable);
//_D3DVECTOR Normalize (const _D3DVECTOR& v);
//_D3DVECTOR Magnitude (const _D3DVECTOR& v);
//_D3DVECTOR CrossProduct (const _D3DVECTOR& v1, const _D3DVECTOR& v2);

};
#endif
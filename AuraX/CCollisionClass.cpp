#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "matrixarb.h"
#include "math.h"

#include "CCollisionClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCollisionObj::CCollisionObj()
{
 GL_barSwitch = FALSE;
 //m_progressInternal = NULL;
 //m_subLevelProgressInternal = NULL;
 //int cntr;
 BoxZoneAdjustManual = 2;
 GL_barTrace = 0;
 FaceDatabank = NULL;
 //GL_worldList = NULL;
 NumberOfLevels =  4;
 MaxFacesInBox = 10;
 FaceDataBankCount = 0;
 BaseBox.max.x = 0.0f;
 BaseBox.max.y = 0.0f;
 BaseBox.max.z = 0.0f;
 BaseBox.min.x = 0.0f;
 BaseBox.min.y = 0.0f;
 BaseBox.min.z = 0.0f;
 refBoxCollectionCount = 0;
 GL_lastBarUpdate = 0;
 SizeSquaredX = NULL;
 SizeSquaredY = NULL;
 SizeSquaredZ = NULL;
 m_wldObjectReferenceArray = NULL;
 boxSizeX = NULL;
 boxSizeY = NULL;
 boxSizeZ = NULL;
 GL_worldList = NULL;
 
 m_updateWnd = NULL;
 directKeyboard = NULL;
 boxCollection = NULL;
 m_smallestTableRes = 1.0f;
}
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
void CCollisionObj::BuildObjectReferenceArray(CWldObjectList * wList)
{
if(m_wldObjectReferenceArray)
   delete [] m_wldObjectReferenceArray;
   m_wldObjectReferenceArray = 0;
   m_wldObjectReferenceArray = new WorldObjectReference[wList->GetCount()];

   int trace = 0;
   for(POSITION posLoc = wList->GetHeadPosition();posLoc != NULL;)
   {
	   m_wldObjectReferenceArray[trace].worldObjectReference = (CWldObject *)wList->GetNext(posLoc);
       trace++;
   }
}
//-----------------------------------------------------------------------------
//Name: Zcheck()
//Desc: Checks to see if the particle is behind the view
//-----------------------------------------------------------------------------
BOOL CCollisionObj::Zcheck(D3DMATRIX matrixLoc,D3DVECTOR point,float fov)
{
	D3DVECTOR zeroedPos;

		zeroedPos.x = point.x - matrixLoc.m[3][0];
		zeroedPos.y = point.y - matrixLoc.m[3][1];
		zeroedPos.z = point.z - matrixLoc.m[3][2];

	D3DVECTOR tempFinal;
	D3DMATRIX invMatrix;
	abMath.TrnsfrMtrxInvrs(matrixLoc.m,invMatrix.m);
	//D3DMath_MatrixInvert(invMatrix,matrixLoc);
	tempFinal.z = (invMatrix.m[0][2]*zeroedPos.x)+(invMatrix.m[1][2]*zeroedPos.y)+(invMatrix.m[2][2]*zeroedPos.z);

	if(tempFinal.z < 0){
		return FALSE;
	}
	return TRUE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::PolyCollisionTest(D3DVECTOR polyLoc[3],
				      float * intersectX,
				      float * intersectY,
				      float * intersectZ,
				      D3DVECTOR * normal,
				      int *actualCollisions,
				      int *consideredCollision,
					  int *indexOfObjectHit
				     )
{
 if(!boxCollection)
  return FALSE;

 int Xpos,Ypos,Zpos;
 int oldXpos = -1;
 int oldYpos = -1;
 int oldZpos = -1;
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX,oldMinX;
 float minY,oldMinY;
 float minZ,oldMinZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 oldMinX = BaseBox.min.x - 1.0f;
 oldMinY = BaseBox.min.y - 1.0f;
 oldMinZ = BaseBox.min.z - 1.0f;

 BNDBOX polyPass = GetPolyBoundingBox(polyLoc);
 for(int loop=0;loop<3;loop++)
 {
  if(BoundBoxPointCheck(polyLoc[loop],BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   Xpos = ConvertPositionToIndex(polyLoc[loop].x,boxSizeX[0],BaseBox.min.x,SizeSquaredX[0]);
   Ypos = ConvertPositionToIndex(polyLoc[loop].y,boxSizeY[0],BaseBox.min.y,SizeSquaredY[0]);
   Zpos = ConvertPositionToIndex(polyLoc[loop].z,boxSizeZ[0],BaseBox.min.z,SizeSquaredZ[0]);
   CurSubLevel = 0;
   GoodSubBox = &boxCollection[Xpos][Ypos][Zpos];
   CurrentSubBox = boxCollection[Xpos][Ypos][Zpos].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   while (CurrentSubBox != NULL)
   {
	
    minX = minX + (Xpos*boxSizeX[CurSubLevel]);
    minY = minY + (Ypos*boxSizeY[CurSubLevel]);
    minZ = minZ + (Zpos*boxSizeZ[CurSubLevel]);
    CurSubLevel++;
    Xpos = ConvertPositionToIndex(polyLoc[loop].x,boxSizeX[CurSubLevel],minX,SizeSquaredX[0]);
    Ypos = ConvertPositionToIndex(polyLoc[loop].y,boxSizeY[CurSubLevel],minY,SizeSquaredY[0]);
    Zpos = ConvertPositionToIndex(polyLoc[loop].z,boxSizeZ[CurSubLevel],minZ,SizeSquaredZ[0]);
    GoodSubBox = &CurrentSubBox[Xpos][Ypos][Zpos];
    CurrentSubBox = CurrentSubBox[Xpos][Ypos][Zpos].SubBoxCollection;
   }

   if(Xpos != oldXpos && Ypos != oldYpos && Zpos != oldZpos && CurSubLevel != oldCurSubLevel
      && minX != oldMinX && minY != oldMinY && minZ != oldMinZ)
   {
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {
     collisionConsideration++;
     //if(BoundingBoxToBoxCheck(polyPass,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
     //{//bounding collision check
      if(ExactColision(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,polyLoc,intersectX,intersectY,intersectZ)==TRUE)
      {
        *normal = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
        if(actualCollisions)
        *actualCollisions = collisionCheckCount;
	    *indexOfObjectHit = FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef;
       return TRUE;
      }
      collisionCheckCount++;
     //}
    }
   }
   oldXpos = Xpos;
   oldYpos = Ypos;
   oldZpos = Zpos;
   oldMinX = minX;
   oldMinY = minY;
   oldMinZ = minZ;
   oldCurSubLevel = CurSubLevel;
  }//end if in bounds
 }
 if(actualCollisions)
  *actualCollisions = collisionCheckCount;
 if(consideredCollision)
  *consideredCollision = collisionConsideration;
 return FALSE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
 BOOL CCollisionObj::CollisionSegmentSIValidPath(
					                D3DVECTOR & collisionPathStart,
					                D3DVECTOR & collisionPathEnd,
					                int *actualCollisions,
					                int *consideredCollision,
					                int tolerance,
									float floorTolerance,
							        float wallMinimumHieght,
							        float currentYpos,
							        D3DVECTOR origin,
							        BOOL *wallInWayRet,
							        BOOL *floorExistRet,
									D3DVECTOR * wallNormal
					               )
{
 BOOL wallInWay = FALSE;
 BOOL floorExist = FALSE;

 if(!boxCollection)
     return FALSE;
 
     D3DVECTOR segmentStart,segmentEnd;
     CopyMemory(&segmentStart,&collisionPathStart,sizeof(D3DVECTOR));
     CopyMemory(&segmentEnd,&collisionPathEnd,sizeof(D3DVECTOR));
     BOOL everColided = FALSE;
     BNDBOX cZone = GetSegmentBoundingBox(collisionPathStart,collisionPathEnd);
     float closestIntersection = 100000.0f;
     INVECTOR posXYZ,oldposXYZ;
     oldposXYZ.x = -1;
     oldposXYZ.y = -1;
     oldposXYZ.z = -1;
     int sizeINVECTOR = sizeof(INVECTOR);
     int oldCurSubLevel=-1;
      //vertex one
     int collisionCheckCount = 0;
     int collisionConsideration = 0;
     int CurSubLevel;
     float minX;
     float minY;
     float minZ;
     REFBOX ***CurrentSubBox;
     REFBOX *GoodSubBox;
     float distanceTraveled = abMath.MagnitudeSquared(segmentStart.x,segmentStart.y,segmentStart.z,
	                                                   segmentEnd.x,segmentEnd.y,segmentEnd.z);
     long possibilities = (long)ceil(distanceTraveled/tolerance);

     for(int prog=0;prog<possibilities;prog++)
	 {//mainloop

        D3DVECTOR currentPosition = abMath.InterpolatePoints(segmentStart,
															 segmentEnd,
					                                         possibilities, 
					                                         prog);


        if(BoundBoxPointCheck(currentPosition,BaseBox.min,BaseBox.max)==TRUE)
		{//if in bounds
            posXYZ.x = (int)((currentPosition.x-BaseBox.min.x) / boxSizeX[0]);
            posXYZ.y = (int)((currentPosition.y-BaseBox.min.y) / boxSizeY[0]);
            posXYZ.z = (int)((currentPosition.z-BaseBox.min.z) / boxSizeZ[0]);

            if(posXYZ.x >= SizeSquaredX[0])
               posXYZ.x = SizeSquaredX[0]-1;
            if(posXYZ.y >= SizeSquaredY[0])
               posXYZ.y = SizeSquaredY[0]-1;
            if(posXYZ.z >= SizeSquaredZ[0])
               posXYZ.z = SizeSquaredZ[0]-1;

            CurSubLevel = 0;
            GoodSubBox = &boxCollection[posXYZ.x][posXYZ.y][posXYZ.z];
            CurrentSubBox = boxCollection[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
            minX = BaseBox.min.x;
            minY = BaseBox.min.y;
            minZ = BaseBox.min.z;
   
            while (CurrentSubBox)
			{
	
              minX = minX + (posXYZ.x*boxSizeX[CurSubLevel]);
              minY = minY + (posXYZ.y*boxSizeY[CurSubLevel]);
              minZ = minZ + (posXYZ.z*boxSizeZ[CurSubLevel]);
              CurSubLevel++;
               
              posXYZ.x =  (int) ((currentPosition.x-minX) / boxSizeX[CurSubLevel]);
              posXYZ.y = (int) ((currentPosition.y-minY) / boxSizeY[CurSubLevel]);
              posXYZ.z = (int) ((currentPosition.z-minZ) / boxSizeZ[CurSubLevel]);

	          if(posXYZ.x >= SizeSquaredX[CurSubLevel])
                 posXYZ.x = SizeSquaredX[CurSubLevel]-1;
              if(posXYZ.y >= SizeSquaredY[CurSubLevel])
                 posXYZ.y = SizeSquaredY[CurSubLevel]-1;
              if(posXYZ.z >= SizeSquaredZ[CurSubLevel])
                 posXYZ.z = SizeSquaredZ[CurSubLevel]-1;
	
	             GoodSubBox = &CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z];
                 CurrentSubBox = CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
			}
 
         if(memcmp(&posXYZ,&oldposXYZ,sizeINVECTOR)==0 && oldCurSubLevel == CurSubLevel)  
	        continue;
    
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
      //if(BoundingBoxToBoxCheck(cZone,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
      //{//bounding collision check
       D3DVECTOR intersectLocalTest;
        if(SegPolyCheck(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,
							                            collisionPathStart,
							                            collisionPathEnd,
							                            &intersectLocalTest)==TRUE)
		{//Has collided
		 float distance = 0.0f;
        
	     everColided = TRUE;

		   if(FaceDatabank[GoodSubBox->refrenceList[loop]].normal.y > floorTolerance)
              floorExist = TRUE;
		   else if(wallInWay==FALSE){
			  wallInWay = TRUE;

			  wallNormal->x = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.x;
              wallNormal->y = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.y;
			  wallNormal->z = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.z;
		
		   }
		}//end Has collided
	    collisionCheckCount++;
      //}//end bounding collision check
    }//pointcheck

	CopyMemory(&oldposXYZ,&posXYZ,sizeINVECTOR);
    oldCurSubLevel = CurSubLevel;
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
 if(actualCollisions)
  *actualCollisions = collisionCheckCount;
 if(consideredCollision)
  *consideredCollision = collisionConsideration;

 *wallInWayRet      = wallInWay;
 *floorExistRet     = floorExist;

 if(wallInWay == TRUE)
	return FALSE;
 if(floorExist == FALSE)
	return FALSE;
 
  return TRUE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::ValidPath(D3DVECTOR polyLoc[3],
				              int *actualCollisions,
				              int *consideredCollision,
							  float floorTolerance,
							  float wallMinimumHieght,
							  float currentYpos,
							  D3DVECTOR origin,
							  D3DVECTOR virtualNormal,
							  BOOL *wallInWayRet,
							  BOOL *floorExistRet
				              )
{
 if(!boxCollection)
  return FALSE;

 int Xpos,Ypos,Zpos;
 int oldXpos = -1;
 int oldYpos = -1;
 int oldZpos = -1;
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX,oldMinX;
 float minY,oldMinY;
 float minZ,oldMinZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;
 BOOL wallInWay = FALSE;
 BOOL floorExist = FALSE;

 oldMinX = BaseBox.min.x - 1.0f;
 oldMinY = BaseBox.min.y - 1.0f;
 oldMinZ = BaseBox.min.z - 1.0f;

 BNDBOX polyPass = GetPolyBoundingBox(polyLoc);
 for(int loop=0;loop<3;loop++)
 {
  if(BoundBoxPointCheck(polyLoc[loop],BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   Xpos = ConvertPositionToIndex(polyLoc[loop].x,boxSizeX[0],BaseBox.min.x,SizeSquaredX[0]);
   Ypos = ConvertPositionToIndex(polyLoc[loop].y,boxSizeY[0],BaseBox.min.y,SizeSquaredY[0]);
   Zpos = ConvertPositionToIndex(polyLoc[loop].z,boxSizeZ[0],BaseBox.min.z,SizeSquaredZ[0]);
   CurSubLevel = 0;
   GoodSubBox = &boxCollection[Xpos][Ypos][Zpos];
   CurrentSubBox = boxCollection[Xpos][Ypos][Zpos].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   while (CurrentSubBox != NULL)
   {
	
    minX = minX + (Xpos*boxSizeX[CurSubLevel]);
    minY = minY + (Ypos*boxSizeY[CurSubLevel]);
    minZ = minZ + (Zpos*boxSizeZ[CurSubLevel]);
    CurSubLevel++;
    Xpos = ConvertPositionToIndex(polyLoc[loop].x,boxSizeX[CurSubLevel],minX,SizeSquaredX[0]);
    Ypos = ConvertPositionToIndex(polyLoc[loop].y,boxSizeY[CurSubLevel],minY,SizeSquaredY[0]);
    Zpos = ConvertPositionToIndex(polyLoc[loop].z,boxSizeZ[CurSubLevel],minZ,SizeSquaredZ[0]);
    GoodSubBox = &CurrentSubBox[Xpos][Ypos][Zpos];
    CurrentSubBox = CurrentSubBox[Xpos][Ypos][Zpos].SubBoxCollection;
   }

   if(Xpos != oldXpos && Ypos != oldYpos && Zpos != oldZpos && CurSubLevel != oldCurSubLevel
      && minX != oldMinX && minY != oldMinY && minZ != oldMinZ)
   {
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {
     collisionConsideration++;
     //if(BoundingBoxToBoxCheck(polyPass,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
     //{//bounding collision check
      D3DVECTOR intersection;

      if(ExactColision(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,polyLoc,&intersection.x,&intersection.y,&intersection.z)==TRUE)
      {
       //floorExist = TRUE;
        if(FaceDatabank[GoodSubBox->refrenceList[loop]].normal.y > floorTolerance)
           floorExist = TRUE;
		 else if(wallInWay==FALSE){
	      //if(FaceDatabank[GoodSubBox->refrenceList[loop]].normal.y > -.8){
		   //verify wall is high enough
           if((FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].y - currentYpos) > wallMinimumHieght)
			   wallInWay = TRUE;
		   if((FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[1].y - currentYpos) > wallMinimumHieght)
			   wallInWay = TRUE;
		   if((FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[2].y - currentYpos) > wallMinimumHieght)
			   wallInWay = TRUE;
		 }
	
        if(actualCollisions)
        *actualCollisions += collisionCheckCount;
      }
      collisionCheckCount++;
    //}
    }
   }
   oldXpos = Xpos;
   oldYpos = Ypos;
   oldZpos = Zpos;
   oldMinX = minX;
   oldMinY = minY;
   oldMinZ = minZ;
   oldCurSubLevel = CurSubLevel;
  }//end if in bounds
 }
 if(actualCollisions)
  *actualCollisions = collisionCheckCount;
 if(consideredCollision)
  *consideredCollision = collisionConsideration;
 

 *wallInWayRet      = wallInWay;
 *floorExistRet     = floorExist;

 if(wallInWay == TRUE)
	return FALSE;
 if(floorExist == FALSE)
	return FALSE;
 
 return TRUE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
void CCollisionObj::BarColorOcsilate()
{
int currentTime = GetTickCount();
       if(currentTime > (GL_lastBarUpdate + 1000)){
		   if(GL_barSwitch == TRUE)
			  GL_barSwitch = FALSE;
		   else
			  GL_barSwitch = TRUE;
           GL_lastBarUpdate = GetTickCount();
           if(GL_barSwitch == TRUE){ 
			  MessageBeep(1);
			  //if(m_updateWnd)
              //  m_updateWnd->SendDlgItemMessage(IDC_PROGRESS_OBJ,PBM_SETBARCOLOR,0,RGB(0,255,0));
		      //Sleep(100);
		   }
		   else{
			  MessageBeep(1);
              //if(m_updateWnd)
              //  m_updateWnd->SendDlgItemMessage(IDC_PROGRESS_OBJ,PBM_SETBARCOLOR,0,RGB(255,0,0));
		      //Sleep(100);
		   }
		   //if(m_updateWnd)
           //  m_updateWnd->SendDlgItemMessage(IDC_PROGRESS_OBJ,PBM_SETBARCOLOR,0,RGB(255,0,0));
	   }
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
 BOOL CCollisionObj::ObjectCollisionAdjustSegment(
					  D3DVECTOR & collisionPathStart,
					  D3DVECTOR & collisionPathEnd,
					  D3DVECTOR & segmentStart,
					  D3DVECTOR & segmentEnd,
					  float * intersectX,
					  float * intersectY,
					  float * intersectZ,
					  int *actualCollisions,
					  int *consideredCollision,
					  float * highestYcontactPoint,
					  D3DVECTOR * latestNormal,
					  BOOL useAngle,
					  D3DVECTOR *adjustVector,
					  float normalYHieghtFilter,//-1 is no hight filter
					  float normalYHieghtFilterDW,
					  int * indexOfObjectHit
					 )
{
 if(!boxCollection)
  return FALSE;

 *highestYcontactPoint = -100000.0f;

 BNDBOX cZone = GetSegmentBoundingBox(collisionPathStart,collisionPathEnd);
 BOOL everColided = FALSE;

 INVECTOR posXYZ,oldposXYZ;
 oldposXYZ.x = -1;
 oldposXYZ.y = -1;
 oldposXYZ.z = -1;
 int oldCurSubLevel=-1;
 int sizeINVECTOR = sizeof(INVECTOR);
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX;
 float minY;
 float minZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 float deepestIntersection = 100000.0f;

 BOOL done = FALSE;

 float distanceTraveled = abMath.MagnitudeSquared(segmentStart.x,segmentStart.y,segmentStart.z,
	                                                   segmentEnd.x,segmentEnd.y,segmentEnd.z);

 float tolerance = 2.0f;

 long possibilities = (long)ceil(distanceTraveled/tolerance);

 for(int prog=0;prog<possibilities;prog++)
 {//mainloop

   D3DVECTOR currentPosition = abMath.InterpolatePoints(segmentStart,
	                                                    segmentEnd,
					                                    possibilities, 
					                                    prog);


  if(BoundBoxPointCheck(currentPosition,BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   posXYZ.x = (int)((currentPosition.x-BaseBox.min.x) / boxSizeX[0]);
   posXYZ.y = (int)((currentPosition.y-BaseBox.min.y) / boxSizeY[0]);
   posXYZ.z = (int)((currentPosition.z-BaseBox.min.z) / boxSizeZ[0]);

   if(posXYZ.x >= SizeSquaredX[0])
           posXYZ.x = SizeSquaredX[0]-1;
   if(posXYZ.y >= SizeSquaredY[0])
           posXYZ.y = SizeSquaredY[0]-1;
   if(posXYZ.z >= SizeSquaredZ[0])
           posXYZ.z = SizeSquaredZ[0]-1;

   CurSubLevel = 0;
   GoodSubBox = &boxCollection[posXYZ.x][posXYZ.y][posXYZ.z];
   CurrentSubBox = boxCollection[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   
   while (CurrentSubBox != NULL)
   {
	
    minX = minX + (posXYZ.x*boxSizeX[CurSubLevel]);
    minY = minY + (posXYZ.y*boxSizeY[CurSubLevel]);
    minZ = minZ + (posXYZ.z*boxSizeZ[CurSubLevel]);
    CurSubLevel++;

    posXYZ.x = (int)((currentPosition.x-minX) / boxSizeX[CurSubLevel]);
    posXYZ.y = (int)((currentPosition.y-minY) / boxSizeY[CurSubLevel]);
    posXYZ.z = (int)((currentPosition.z-minZ) / boxSizeZ[CurSubLevel]);

	if(posXYZ.x >= SizeSquaredX[CurSubLevel])
           posXYZ.x = SizeSquaredX[CurSubLevel]-1;
    if(posXYZ.y >= SizeSquaredY[CurSubLevel])
           posXYZ.y = SizeSquaredY[CurSubLevel]-1;
    if(posXYZ.z >= SizeSquaredZ[CurSubLevel])
           posXYZ.z = SizeSquaredZ[CurSubLevel]-1;

	
	GoodSubBox = &CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z];
    CurrentSubBox = CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   }
 
   if(memcmp(&posXYZ,&oldposXYZ,sizeINVECTOR)==0 && oldCurSubLevel == CurSubLevel) 
	  continue;
    
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
     //if(BoundingBoxToBoxCheck(cZone,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
     //{//bounding collision check
       
       D3DVECTOR intersectLocalTest;
	   if(FaceDatabank[GoodSubBox->refrenceList[loop]].normal.y >= normalYHieghtFilter && 
		  FaceDatabank[GoodSubBox->refrenceList[loop]].normal.y <= normalYHieghtFilterDW)
        if(SegPolyCheck(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,
							                            collisionPathStart,
							                            collisionPathEnd,
							                            &intersectLocalTest)==TRUE)
		{//Has collided
		 float distance = 0.0f;
         if(GL_matrixFunctions.GetSubDistance(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,
						                      FaceDatabank[GoodSubBox->refrenceList[loop]].normal,
						                      collisionPathEnd,
						                      &distance)){
            
		 
		 
		  if(distance < deepestIntersection){

			 deepestIntersection = distance;

             *adjustVector = GL_matrixFunctions.GetAdjustVector(distance,FaceDatabank[GoodSubBox->refrenceList[loop]].normal);

             *intersectX = intersectLocalTest.x;
	         *intersectY = intersectLocalTest.y;
	         *intersectZ = intersectLocalTest.z;
	         *latestNormal = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
			 if(*highestYcontactPoint < intersectLocalTest.y)
	            *highestYcontactPoint = intersectLocalTest.y;

			 *indexOfObjectHit = FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef;
	     
	         everColided = TRUE;
		  }//end Has collided
		  else{
             if(*highestYcontactPoint < intersectLocalTest.y)
	            *highestYcontactPoint = intersectLocalTest.y;
		  }
		 }
		}
	    collisionCheckCount++;
	 //}//end bounding collision check
    }//pointcheck
    CopyMemory(&oldposXYZ,&posXYZ,sizeINVECTOR);
    oldCurSubLevel = CurSubLevel;
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
 if(actualCollisions)
  *actualCollisions = collisionCheckCount;
 if(consideredCollision)
  *consideredCollision = collisionConsideration;
 if(everColided == TRUE)
  return TRUE;
 else
  return FALSE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::GetClosesIntersectionUseSeg(
					  D3DVECTOR & collisionPathStart,
					  D3DVECTOR & collisionPathEnd,
					  D3DVECTOR & segmentStart,
					  D3DVECTOR & segmentEnd,
					  float * intersectX,
					  float * intersectY,
					  float * intersectZ,
					  int * indexOfObjectHit,
					  D3DVECTOR * latestNormal,
					  bool useangle,
					  float & dist,
					  byte & type
					 )
{
 if(!boxCollection)
  return FALSE;

 if(memcmp(&segmentStart,&segmentEnd,sizeof(D3DVECTOR))==0)
    return FALSE;

	D3DVECTOR virtualNormal;
	virtualNormal.x = collisionPathEnd.x - collisionPathStart.x;
	virtualNormal.y = collisionPathEnd.y - collisionPathStart.y;
	virtualNormal.z = collisionPathEnd.z - collisionPathStart.z;

	virtualNormal = abMath.Normalize(virtualNormal);

 BNDBOX cZone = GetSegmentBoundingBox(collisionPathStart,collisionPathEnd);
 BOOL everColided = FALSE;

 INVECTOR posXYZ,oldposXYZ;
 oldposXYZ.x = -1;
 oldposXYZ.y = -1;
 oldposXYZ.z = -1;
 int oldCurSubLevel=-1;
 int sizeINVECTOR = sizeof(INVECTOR);
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX;
 float minY;
 float minZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 float deepestIntersection = 100000.0f;

 BOOL done = FALSE;

 

 float distanceTraveled = abMath.MagnitudeSquared(segmentStart.x,segmentStart.y,segmentStart.z,
	                                                   segmentEnd.x,segmentEnd.y,segmentEnd.z);

 float tolerance = 2.0f;

 long possibilities = (long)ceil(distanceTraveled/tolerance);

 for(int prog=0;prog<possibilities;prog++)
 {//mainloop

   D3DVECTOR currentPosition = abMath.InterpolatePoints(segmentStart,
	                                                    segmentEnd,
					                                    possibilities, 
					                                    prog);


  if(BoundBoxPointCheck(currentPosition,BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   posXYZ.x = (int)((currentPosition.x-BaseBox.min.x) / boxSizeX[0]);
   posXYZ.y = (int)((currentPosition.y-BaseBox.min.y) / boxSizeY[0]);
   posXYZ.z = (int)((currentPosition.z-BaseBox.min.z) / boxSizeZ[0]);

   if(posXYZ.x >= SizeSquaredX[0])
           posXYZ.x = SizeSquaredX[0]-1;
   if(posXYZ.y >= SizeSquaredY[0])
           posXYZ.y = SizeSquaredY[0]-1;
   if(posXYZ.z >= SizeSquaredZ[0])
           posXYZ.z = SizeSquaredZ[0]-1;

   CurSubLevel = 0;
   GoodSubBox = &boxCollection[posXYZ.x][posXYZ.y][posXYZ.z];
   CurrentSubBox = boxCollection[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   
   while (CurrentSubBox != NULL)
   {
	
    minX = minX + (posXYZ.x*boxSizeX[CurSubLevel]);
    minY = minY + (posXYZ.y*boxSizeY[CurSubLevel]);
    minZ = minZ + (posXYZ.z*boxSizeZ[CurSubLevel]);
    CurSubLevel++;

    posXYZ.x = (int)((currentPosition.x-minX) / boxSizeX[CurSubLevel]);
    posXYZ.y = (int)((currentPosition.y-minY) / boxSizeY[CurSubLevel]);
    posXYZ.z = (int)((currentPosition.z-minZ) / boxSizeZ[CurSubLevel]);

	if(posXYZ.x >= SizeSquaredX[CurSubLevel])
           posXYZ.x = SizeSquaredX[CurSubLevel]-1;
    if(posXYZ.y >= SizeSquaredY[CurSubLevel])
           posXYZ.y = SizeSquaredY[CurSubLevel]-1;
    if(posXYZ.z >= SizeSquaredZ[CurSubLevel])
           posXYZ.z = SizeSquaredZ[CurSubLevel]-1;

	
	GoodSubBox = &CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z];
    CurrentSubBox = CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   }
 
   if(memcmp(&posXYZ,&oldposXYZ,sizeINVECTOR)==0 && oldCurSubLevel == CurSubLevel) 
	  continue;
    
    D3DVECTOR intersectLocalTest;
	float angle = 0.0f;
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
     //if(BoundingBoxToBoxCheck(cZone,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==FALSE)
     //  continue;
	 //{//bounding collision check
        
           
		//continue;//filter by angle
       
        if(SegPolyCheck(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,
							                            collisionPathStart,
							                            collisionPathEnd,
							                            &intersectLocalTest)==TRUE)
		{//Has collided
         if(useangle == true)
		 {
			angle = GL_matrixFunctions.GetAngleBetweenTwoVectorsFast(virtualNormal,FaceDatabank[GoodSubBox->refrenceList[loop]].normal);
			if(angle < 89)
			   continue;
		 }

		 float distance = 0.0f;
        
		 if(memcmp(&segmentStart,&intersectLocalTest,sizeINVECTOR)==0) 
			continue;
		 
            if(type == 0)
		       distance = GL_matrixFunctions.MagnitudeSquared(segmentStart.x,segmentStart.y,segmentStart.z,intersectLocalTest.x,intersectLocalTest.y,intersectLocalTest.z);
			else
               GL_matrixFunctions.GetSubDistance(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,FaceDatabank[GoodSubBox->refrenceList[loop]].normal,collisionPathEnd,&distance);
		


		  if(distance < deepestIntersection)
		  {
             

             //dist = distance;
			 deepestIntersection = distance;

             *intersectX = intersectLocalTest.x;
	         *intersectY = intersectLocalTest.y;
	         *intersectZ = intersectLocalTest.z;
	         *latestNormal = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
			 *indexOfObjectHit = FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef;
	     
	         everColided = TRUE;
		  }
		}//end Has collided
	    collisionCheckCount++;
	 //}//end bounding collision check
    }//pointcheck
    CopyMemory(&oldposXYZ,&posXYZ,sizeINVECTOR);
    oldCurSubLevel = CurSubLevel;
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
 if(everColided == TRUE)
 {
  dist = GL_matrixFunctions.MagnitudeSquared(segmentEnd.x,segmentEnd.y,segmentEnd.z,*intersectX,*intersectY,*intersectZ);
  return TRUE;
 }
 else
  return FALSE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::CollisionSegmentSItranslated(
					  D3DVECTOR collisionPathStart,
					  D3DVECTOR collisionPathEnd,
					  float * intersectX,
					  float * intersectY,
					  float * intersectZ,
					  int *actualCollisions,
					  int *consideredCollision,
					  D3DVECTOR * latestNormal,
					  int * indexOfObjectHit,
					  float tolerance,
					  BOOL returnAfterFirstContact,
					  D3DMATRIX & position,
					  D3DMATRIX & invPosition
					 )
{
 if(!boxCollection)
  return FALSE;
 

 //translation
 collisionPathStart.x += invPosition._41;
 collisionPathStart.y += invPosition._42;
 collisionPathStart.z += invPosition._43;

 collisionPathEnd.x += invPosition._41;
 collisionPathEnd.y += invPosition._42;
 collisionPathEnd.z += invPosition._43;
 //end translation
 D3DVECTOR segmentStart,segmentEnd;
 CopyMemory(&segmentStart,&collisionPathStart,sizeof(D3DVECTOR));
 CopyMemory(&segmentEnd,&collisionPathEnd,sizeof(D3DVECTOR));


 BOOL everColided = FALSE;

 BNDBOX cZone = GetSegmentBoundingBox(collisionPathStart,collisionPathEnd);
 
 float closestIntersection = 100000.0f;


 INVECTOR posXYZ,oldposXYZ;
 oldposXYZ.x = -1;
 oldposXYZ.y = -1;
 oldposXYZ.z = -1;
 int sizeINVECTOR = sizeof(INVECTOR);
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX;
 float minY;
 float minZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 float distanceTraveled = abMath.MagnitudeSquared(segmentStart.x,segmentStart.y,segmentStart.z,
	                                                   segmentEnd.x,segmentEnd.y,segmentEnd.z);

 


 long possibilities = (long)ceil(distanceTraveled/tolerance);

 for(int prog=0;prog<possibilities;prog++)
 {//mainloop

   D3DVECTOR currentPosition = abMath.InterpolatePoints(segmentStart,
															 segmentEnd,
					                                         possibilities, 
					                                         prog);


  if(BoundBoxPointCheck(currentPosition,BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   posXYZ.x = (int)((currentPosition.x-BaseBox.min.x) / boxSizeX[0]);
   posXYZ.y = (int)((currentPosition.y-BaseBox.min.y) / boxSizeY[0]);
   posXYZ.z = (int)((currentPosition.z-BaseBox.min.z) / boxSizeZ[0]);

   if(posXYZ.x >= SizeSquaredX[0])
           posXYZ.x = SizeSquaredX[0]-1;
    if(posXYZ.y >= SizeSquaredY[0])
           posXYZ.y = SizeSquaredY[0]-1;
    if(posXYZ.z >= SizeSquaredZ[0])
           posXYZ.z = SizeSquaredZ[0]-1;

   CurSubLevel = 0;
   GoodSubBox = &boxCollection[posXYZ.x][posXYZ.y][posXYZ.z];
   CurrentSubBox = boxCollection[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   
   while (CurrentSubBox)
   {
	
    minX = minX + (posXYZ.x*boxSizeX[CurSubLevel]);
    minY = minY + (posXYZ.y*boxSizeY[CurSubLevel]);
    minZ = minZ + (posXYZ.z*boxSizeZ[CurSubLevel]);
    CurSubLevel++;
               
    posXYZ.x = (int)((currentPosition.x-minX) / boxSizeX[CurSubLevel]);
    posXYZ.y = (int)((currentPosition.y-minY) / boxSizeY[CurSubLevel]);
    posXYZ.z = (int)((currentPosition.z-minZ) / boxSizeZ[CurSubLevel]);

	if(posXYZ.x >= SizeSquaredX[CurSubLevel])
           posXYZ.x = SizeSquaredX[CurSubLevel]-1;
    if(posXYZ.y >= SizeSquaredY[CurSubLevel])
           posXYZ.y = SizeSquaredY[CurSubLevel]-1;
    if(posXYZ.z >= SizeSquaredZ[CurSubLevel])
           posXYZ.z = SizeSquaredZ[CurSubLevel]-1;
	
	GoodSubBox = &CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z];
    CurrentSubBox = CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   }
 
   if(memcmp(&posXYZ,&oldposXYZ,sizeINVECTOR)==0 && oldCurSubLevel == CurSubLevel) 
	  continue;
    
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
      //if(BoundingBoxToBoxCheck(cZone,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
      //{//bounding collision check
       D3DVECTOR intersectLocalTest;
        if(SegPolyCheck(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,
							                            collisionPathStart,
							                            collisionPathEnd,
							                            &intersectLocalTest)==TRUE)
		{//Has collided
		 float distance = 0.0f;
        
	     everColided = TRUE;
		 if(returnAfterFirstContact){
			 if(actualCollisions)
                *actualCollisions = collisionCheckCount;
             if(consideredCollision)
                *consideredCollision = collisionConsideration;
		     *intersectX = intersectLocalTest.x + position._41;
	         *intersectY = intersectLocalTest.y + position._42;
	         *intersectZ = intersectLocalTest.z + position._43;
			 *latestNormal = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
			 *indexOfObjectHit = FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef;
			 return TRUE;
		 }

         float curdistance = GL_matrixFunctions.MagnitudeSquared(collisionPathStart.x,
			                                                     collisionPathStart.y,
																 collisionPathStart.z,
																 intersectLocalTest.x,
																 intersectLocalTest.y,
																 intersectLocalTest.z);


		 if(curdistance < closestIntersection){
			closestIntersection = curdistance;

         *intersectX = intersectLocalTest.x + position._41;
	     *intersectY = intersectLocalTest.y + position._42;
	     *intersectZ = intersectLocalTest.z + position._43;
	     *latestNormal = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
		 *indexOfObjectHit = FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef;

		 }


		}//end Has collided
	    collisionCheckCount++;
      //}//end bounding collision check
    }//pointcheck

	CopyMemory(&oldposXYZ,&posXYZ,sizeINVECTOR);
    oldCurSubLevel = CurSubLevel;
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
 if(actualCollisions)
  *actualCollisions = collisionCheckCount;
 if(consideredCollision)
  *consideredCollision = collisionConsideration;
 if(everColided == TRUE)
  return TRUE;
 else
  return FALSE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::CollisionSegmentSI(
					  D3DVECTOR collisionPathStart,
					  D3DVECTOR collisionPathEnd,
					  float * intersectX,
					  float * intersectY,
					  float * intersectZ,
					  int *actualCollisions,
					  int *consideredCollision,
					  D3DVECTOR * latestNormal,
					  int * indexOfObjectHit,
					  float tolerance,
					  BOOL returnAfterFirstContact
					 )
{
 if(!boxCollection)
  return FALSE;
 
 D3DVECTOR segmentStart,segmentEnd;
 CopyMemory(&segmentStart,&collisionPathStart,sizeof(D3DVECTOR));
 CopyMemory(&segmentEnd,&collisionPathEnd,sizeof(D3DVECTOR));


 BOOL everColided = FALSE;

 BNDBOX cZone = GetSegmentBoundingBox(collisionPathStart,collisionPathEnd);
 
 float closestIntersection = 100000.0f;


 INVECTOR posXYZ,oldposXYZ;
 oldposXYZ.x = -1;
 oldposXYZ.y = -1;
 oldposXYZ.z = -1;
 int sizeINVECTOR = sizeof(INVECTOR);
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX;
 float minY;
 float minZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 float distanceTraveled = abMath.MagnitudeSquared(segmentStart.x,segmentStart.y,segmentStart.z,
	                                                   segmentEnd.x,segmentEnd.y,segmentEnd.z);

 


 long possibilities = (long)ceil(distanceTraveled/tolerance);

 for(int prog=0;prog<possibilities;prog++)
 {//mainloop

   D3DVECTOR currentPosition = abMath.InterpolatePoints(segmentStart,
															 segmentEnd,
					                                         possibilities, 
					                                         prog);


  if(BoundBoxPointCheck(currentPosition,BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   posXYZ.x = (int)((currentPosition.x-BaseBox.min.x) / boxSizeX[0]);
   posXYZ.y = (int)((currentPosition.y-BaseBox.min.y) / boxSizeY[0]);
   posXYZ.z = (int)((currentPosition.z-BaseBox.min.z) / boxSizeZ[0]);

   if(posXYZ.x >= SizeSquaredX[0])
           posXYZ.x = SizeSquaredX[0]-1;
    if(posXYZ.y >= SizeSquaredY[0])
           posXYZ.y = SizeSquaredY[0]-1;
    if(posXYZ.z >= SizeSquaredZ[0])
           posXYZ.z = SizeSquaredZ[0]-1;

   CurSubLevel = 0;
   GoodSubBox = &boxCollection[posXYZ.x][posXYZ.y][posXYZ.z];
   CurrentSubBox = boxCollection[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   
   while (CurrentSubBox)
   {
	
    minX = minX + (posXYZ.x*boxSizeX[CurSubLevel]);
    minY = minY + (posXYZ.y*boxSizeY[CurSubLevel]);
    minZ = minZ + (posXYZ.z*boxSizeZ[CurSubLevel]);
    CurSubLevel++;
               
    posXYZ.x = (int)((currentPosition.x-minX) / boxSizeX[CurSubLevel]);
    posXYZ.y = (int)((currentPosition.y-minY) / boxSizeY[CurSubLevel]);
    posXYZ.z = (int)((currentPosition.z-minZ) / boxSizeZ[CurSubLevel]);

	if(posXYZ.x >= SizeSquaredX[CurSubLevel])
           posXYZ.x = SizeSquaredX[CurSubLevel]-1;
    if(posXYZ.y >= SizeSquaredY[CurSubLevel])
           posXYZ.y = SizeSquaredY[CurSubLevel]-1;
    if(posXYZ.z >= SizeSquaredZ[CurSubLevel])
           posXYZ.z = SizeSquaredZ[CurSubLevel]-1;
	
	GoodSubBox = &CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z];
    CurrentSubBox = CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   }
 
   if(memcmp(&posXYZ,&oldposXYZ,sizeINVECTOR)==0 && oldCurSubLevel == CurSubLevel) 
	  continue;
    
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
      //if(BoundingBoxToBoxCheck(cZone,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
      //{//bounding collision check
       D3DVECTOR intersectLocalTest;
        if(SegPolyCheck(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,
							                            collisionPathStart,
							                            collisionPathEnd,
							                            &intersectLocalTest)==TRUE)
		{//Has collided
		 float distance = 0.0f;
        
	     everColided = TRUE;
		 if(returnAfterFirstContact){
			 if(actualCollisions)
                *actualCollisions = collisionCheckCount;
             if(consideredCollision)
                *consideredCollision = collisionConsideration;
		     *intersectX = intersectLocalTest.x;
	         *intersectY = intersectLocalTest.y;
	         *intersectZ = intersectLocalTest.z;
			 *latestNormal = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
			 *indexOfObjectHit = FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef;
			 return TRUE;
		 }

         float curdistance = GL_matrixFunctions.MagnitudeSquared(collisionPathStart.x,
			                                                     collisionPathStart.y,
																 collisionPathStart.z,
																 intersectLocalTest.x,
																 intersectLocalTest.y,
																 intersectLocalTest.z);


		 if(curdistance < closestIntersection){
			closestIntersection = curdistance;

         *intersectX = intersectLocalTest.x;
	     *intersectY = intersectLocalTest.y;
	     *intersectZ = intersectLocalTest.z;
	     *latestNormal = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
		 *indexOfObjectHit = FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef;

		 }


		}//end Has collided
	    collisionCheckCount++;
      //}//end bounding collision check
    }//pointcheck

	CopyMemory(&oldposXYZ,&posXYZ,sizeINVECTOR);
    oldCurSubLevel = CurSubLevel;
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
 if(actualCollisions)
  *actualCollisions = collisionCheckCount;
 if(consideredCollision)
  *consideredCollision = collisionConsideration;
 if(everColided == TRUE)
  return TRUE;
 else
  return FALSE;
}

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::CollisionSegmentSIFilterCharona(
					  D3DVECTOR collisionPathStart,
					  D3DVECTOR collisionPathEnd,
					  float tolerance
					)
{
 if(!boxCollection)
  return FALSE;
 
  BOOL doCheck = TRUE;
 D3DVECTOR segmentStart,segmentEnd;
 CopyMemory(&segmentStart,&collisionPathStart,sizeof(D3DVECTOR));
 CopyMemory(&segmentEnd,&collisionPathEnd,sizeof(D3DVECTOR));


 BOOL everColided = FALSE;

 BNDBOX cZone = GetSegmentBoundingBox(collisionPathStart,collisionPathEnd);
 
 float closestIntersection = 100000.0f;


 INVECTOR posXYZ,oldposXYZ;
 oldposXYZ.x = -1;
 oldposXYZ.y = -1;
 oldposXYZ.z = -1;
 int sizeINVECTOR = sizeof(INVECTOR);
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX;
 float minY;
 float minZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 float distanceTraveled = abMath.MagnitudeSquared(segmentStart.x,segmentStart.y,segmentStart.z,
	                                                   segmentEnd.x,segmentEnd.y,segmentEnd.z);

 


 long possibilities = (long)ceil(distanceTraveled/tolerance);

 for(int prog=0;prog<possibilities;prog++)
 {//mainloop

   D3DVECTOR currentPosition = abMath.InterpolatePoints(segmentStart,
															 segmentEnd,
					                                         possibilities, 
					                                         prog);


  if(BoundBoxPointCheck(currentPosition,BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   posXYZ.x = (int)((currentPosition.x-BaseBox.min.x) / boxSizeX[0]);
   posXYZ.y = (int)((currentPosition.y-BaseBox.min.y) / boxSizeY[0]);
   posXYZ.z = (int)((currentPosition.z-BaseBox.min.z) / boxSizeZ[0]);

   if(posXYZ.x >= SizeSquaredX[0])
           posXYZ.x = SizeSquaredX[0]-1;
    if(posXYZ.y >= SizeSquaredY[0])
           posXYZ.y = SizeSquaredY[0]-1;
    if(posXYZ.z >= SizeSquaredZ[0])
           posXYZ.z = SizeSquaredZ[0]-1;

   CurSubLevel = 0;
   GoodSubBox = &boxCollection[posXYZ.x][posXYZ.y][posXYZ.z];
   CurrentSubBox = boxCollection[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   
   while (CurrentSubBox)
   {
	
    minX = minX + (posXYZ.x*boxSizeX[CurSubLevel]);
    minY = minY + (posXYZ.y*boxSizeY[CurSubLevel]);
    minZ = minZ + (posXYZ.z*boxSizeZ[CurSubLevel]);
    CurSubLevel++;
               
    posXYZ.x = (int)((currentPosition.x-minX) / boxSizeX[CurSubLevel]);
    posXYZ.y = (int)((currentPosition.y-minY) / boxSizeY[CurSubLevel]);
    posXYZ.z = (int)((currentPosition.z-minZ) / boxSizeZ[CurSubLevel]);

	if(posXYZ.x >= SizeSquaredX[CurSubLevel])
           posXYZ.x = SizeSquaredX[CurSubLevel]-1;
    if(posXYZ.y >= SizeSquaredY[CurSubLevel])
           posXYZ.y = SizeSquaredY[CurSubLevel]-1;
    if(posXYZ.z >= SizeSquaredZ[CurSubLevel])
           posXYZ.z = SizeSquaredZ[CurSubLevel]-1;
	
	GoodSubBox = &CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z];
    CurrentSubBox = CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   }
 
   if(memcmp(&posXYZ,&oldposXYZ,sizeINVECTOR)==0 && oldCurSubLevel == CurSubLevel) 
	  continue;
    
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
      //if(BoundingBoxToBoxCheck(cZone,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
      //{//bounding collision check
       D3DVECTOR intersectLocalTest;
	   doCheck = TRUE;
	    if(m_wldObjectReferenceArray[FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef].worldObjectReference->m_meshObject)
		{
	       if(m_wldObjectReferenceArray[FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef].worldObjectReference->m_meshObject->m_charona)//dont check charona objects
            doCheck = FALSE;
		   
		   if(m_wldObjectReferenceArray[FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef].worldObjectReference->m_meshObject->List_materialObject)
		   if(m_wldObjectReferenceArray[FaceDatabank[GoodSubBox->refrenceList[loop]].m_objectIndexRef].worldObjectReference->m_meshObject->List_materialObject->m_blendMode > -1)
			doCheck = FALSE;
		}
		


		if(doCheck)
		if(SegPolyCheck(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,
							                            collisionPathStart,
							                            collisionPathEnd,
							                            &intersectLocalTest)==TRUE)
		{//Has collided
          
		 float distance = 0.0f;
	     everColided = TRUE;
		 return TRUE;
		}//end Has collided
	    collisionCheckCount++;
	  //}//end bounding collision check
	}//pointcheck

	CopyMemory(&oldposXYZ,&posXYZ,sizeINVECTOR);
    oldCurSubLevel = CurSubLevel;
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
  return FALSE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::CollisionSegmentSIBasic(
					  D3DVECTOR &collisionPathStart,
					  D3DVECTOR &collisionPathEnd,
					  float tolerance
					)
{
 if(!boxCollection)
  return FALSE;

 
 
  BOOL doCheck = TRUE;
 D3DVECTOR segmentStart,segmentEnd;
 CopyMemory(&segmentStart,&collisionPathStart,sizeof(D3DVECTOR));
 CopyMemory(&segmentEnd,&collisionPathEnd,sizeof(D3DVECTOR));


 BOOL everColided = FALSE;

 BNDBOX cZone = GetSegmentBoundingBox(collisionPathStart,collisionPathEnd);
 
 float closestIntersection = 100000.0f;


 INVECTOR posXYZ,oldposXYZ;
 oldposXYZ.x = -1;
 oldposXYZ.y = -1;
 oldposXYZ.z = -1;
 int sizeINVECTOR = sizeof(INVECTOR);
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX;
 float minY;
 float minZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 float distanceTraveled = abMath.MagnitudeSquared(segmentStart.x,segmentStart.y,segmentStart.z,
	                                                   segmentEnd.x,segmentEnd.y,segmentEnd.z);

 


 long possibilities = (long)ceil(distanceTraveled/tolerance);

 for(int prog=0;prog<possibilities;prog++)
 {//mainloop

   D3DVECTOR currentPosition = abMath.InterpolatePoints(segmentStart,
															 segmentEnd,
					                                         possibilities, 
					                                         prog);


  if(BoundBoxPointCheck(currentPosition,BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   posXYZ.x = (int)((currentPosition.x-BaseBox.min.x) / boxSizeX[0]);
   posXYZ.y = (int)((currentPosition.y-BaseBox.min.y) / boxSizeY[0]);
   posXYZ.z = (int)((currentPosition.z-BaseBox.min.z) / boxSizeZ[0]);

   if(posXYZ.x >= SizeSquaredX[0])
           posXYZ.x = SizeSquaredX[0]-1;
    if(posXYZ.y >= SizeSquaredY[0])
           posXYZ.y = SizeSquaredY[0]-1;
    if(posXYZ.z >= SizeSquaredZ[0])
           posXYZ.z = SizeSquaredZ[0]-1;

   CurSubLevel = 0;
   GoodSubBox = &boxCollection[posXYZ.x][posXYZ.y][posXYZ.z];
   CurrentSubBox = boxCollection[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   
   while (CurrentSubBox)
   {
	
    minX = minX + (posXYZ.x*boxSizeX[CurSubLevel]);
    minY = minY + (posXYZ.y*boxSizeY[CurSubLevel]);
    minZ = minZ + (posXYZ.z*boxSizeZ[CurSubLevel]);
    CurSubLevel++;
               
    posXYZ.x = (int)((currentPosition.x-minX) / boxSizeX[CurSubLevel]);
    posXYZ.y = (int)((currentPosition.y-minY) / boxSizeY[CurSubLevel]);
    posXYZ.z = (int)((currentPosition.z-minZ) / boxSizeZ[CurSubLevel]);

	if(posXYZ.x >= SizeSquaredX[CurSubLevel])
           posXYZ.x = SizeSquaredX[CurSubLevel]-1;
    if(posXYZ.y >= SizeSquaredY[CurSubLevel])
           posXYZ.y = SizeSquaredY[CurSubLevel]-1;
    if(posXYZ.z >= SizeSquaredZ[CurSubLevel])
           posXYZ.z = SizeSquaredZ[CurSubLevel]-1;
	
	GoodSubBox = &CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z];
    CurrentSubBox = CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   }
 
   if(memcmp(&posXYZ,&oldposXYZ,sizeINVECTOR)==0 && oldCurSubLevel == CurSubLevel) 
	  continue;
    
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
      //if(BoundingBoxToBoxCheck(cZone,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
      //{//bounding collision check
       D3DVECTOR intersectLocalTest;
		if(SegPolyCheck(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,
							                            collisionPathStart,
							                            collisionPathEnd,
							                            &intersectLocalTest)==TRUE)
		{//Has collided
			float distance = 0.0f;
			everColided = TRUE;
			return TRUE;
		}//end Has collided
	    collisionCheckCount++;
	  //}//end bounding collision check
	}//pointcheck

	CopyMemory(&oldposXYZ,&posXYZ,sizeINVECTOR);
    oldCurSubLevel = CurSubLevel;
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
  return FALSE;
}

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::CollisionSegmentSIBasicNormal(
					  D3DVECTOR &collisionPathStart,
					  D3DVECTOR &collisionPathEnd,
					  float tolerance
					)
{
 if(!boxCollection)
  return FALSE;

 D3DVECTOR virtualNormal;
 virtualNormal.x = collisionPathEnd.x - collisionPathStart.x;
 virtualNormal.y = collisionPathEnd.y - collisionPathStart.y;
 virtualNormal.z = collisionPathEnd.z - collisionPathStart.z;
 virtualNormal = abMath.Normalize(virtualNormal);
 
  BOOL doCheck = TRUE;
 D3DVECTOR segmentStart,segmentEnd;
 CopyMemory(&segmentStart,&collisionPathStart,sizeof(D3DVECTOR));
 CopyMemory(&segmentEnd,&collisionPathEnd,sizeof(D3DVECTOR));


 BOOL everColided = FALSE;

 BNDBOX cZone = GetSegmentBoundingBox(collisionPathStart,collisionPathEnd);
 
 float closestIntersection = 100000.0f;


 INVECTOR posXYZ,oldposXYZ;
 oldposXYZ.x = -1;
 oldposXYZ.y = -1;
 oldposXYZ.z = -1;
 int sizeINVECTOR = sizeof(INVECTOR);
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX;
 float minY;
 float minZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 float distanceTraveled = abMath.MagnitudeSquared(segmentStart.x,segmentStart.y,segmentStart.z,
	                                                   segmentEnd.x,segmentEnd.y,segmentEnd.z);

 


 long possibilities = (long)ceil(distanceTraveled/tolerance);

 for(int prog=0;prog<possibilities;prog++)
 {//mainloop

   D3DVECTOR currentPosition = abMath.InterpolatePoints(segmentStart,
															 segmentEnd,
					                                         possibilities, 
					                                         prog);


  if(BoundBoxPointCheck(currentPosition,BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   posXYZ.x = (int)((currentPosition.x-BaseBox.min.x) / boxSizeX[0]);
   posXYZ.y = (int)((currentPosition.y-BaseBox.min.y) / boxSizeY[0]);
   posXYZ.z = (int)((currentPosition.z-BaseBox.min.z) / boxSizeZ[0]);

   if(posXYZ.x >= SizeSquaredX[0])
           posXYZ.x = SizeSquaredX[0]-1;
    if(posXYZ.y >= SizeSquaredY[0])
           posXYZ.y = SizeSquaredY[0]-1;
    if(posXYZ.z >= SizeSquaredZ[0])
           posXYZ.z = SizeSquaredZ[0]-1;

   CurSubLevel = 0;
   GoodSubBox = &boxCollection[posXYZ.x][posXYZ.y][posXYZ.z];
   CurrentSubBox = boxCollection[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   
   while (CurrentSubBox)
   {
	
    minX = minX + (posXYZ.x*boxSizeX[CurSubLevel]);
    minY = minY + (posXYZ.y*boxSizeY[CurSubLevel]);
    minZ = minZ + (posXYZ.z*boxSizeZ[CurSubLevel]);
    CurSubLevel++;
               
    posXYZ.x = (int)((currentPosition.x-minX) / boxSizeX[CurSubLevel]);
    posXYZ.y = (int)((currentPosition.y-minY) / boxSizeY[CurSubLevel]);
    posXYZ.z = (int)((currentPosition.z-minZ) / boxSizeZ[CurSubLevel]);

	if(posXYZ.x >= SizeSquaredX[CurSubLevel])
           posXYZ.x = SizeSquaredX[CurSubLevel]-1;
    if(posXYZ.y >= SizeSquaredY[CurSubLevel])
           posXYZ.y = SizeSquaredY[CurSubLevel]-1;
    if(posXYZ.z >= SizeSquaredZ[CurSubLevel])
           posXYZ.z = SizeSquaredZ[CurSubLevel]-1;
	
	GoodSubBox = &CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z];
    CurrentSubBox = CurrentSubBox[posXYZ.x][posXYZ.y][posXYZ.z].SubBoxCollection;
   }
 
   if(memcmp(&posXYZ,&oldposXYZ,sizeINVECTOR)==0 && oldCurSubLevel == CurSubLevel) 
	  continue;
    
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
      //if(BoundingBoxToBoxCheck(cZone,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
      //{//bounding collision check
       D3DVECTOR intersectLocalTest;
		if(SegPolyCheck(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,
							                            collisionPathStart,
							                            collisionPathEnd,
							                            &intersectLocalTest)==TRUE)
		{//Has collided
         float deg = 91.0f;
      
	        D3DVECTOR newNormalLoc;
	        newNormalLoc.x = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.x
                       + FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].x;
            newNormalLoc.y = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.y
                       + FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].y;
            newNormalLoc.z = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.z
                       + FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].z;
	        deg = ContactAngle(collisionPathStart,virtualNormal,FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0],newNormalLoc);
        
         if(deg > 89)
         {

			float distance = 0.0f;
			everColided = TRUE;
			return TRUE;
		 }
		}//end Has collided
	    collisionCheckCount++;
	  //}//end bounding collision check
	}//pointcheck

	CopyMemory(&oldposXYZ,&posXYZ,sizeINVECTOR);
    oldCurSubLevel = CurSubLevel;
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
  return FALSE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::ObjectCollisionAdjust(D3DVECTOR polyLoc[3],
					  float * intersectX,
					  float * intersectY,
					  float * intersectZ,
					  int *actualCollisions,
					  int *consideredCollision,
					  D3DVECTOR storedNormals[20],
					  float storedDistance[20],
                                          int & howManyNormals,
					  float * highestYcontactPoint,
					  BOOL recordAllInfo,
					  D3DVECTOR * latestNormal,
					  D3DVECTOR virtualNormal,
					  D3DVECTOR origin,
					  BOOL useAngle
					 )
{
 if(!boxCollection)
  return FALSE;

 BOOL everColided = FALSE;

 int Xpos,Ypos,Zpos;
 int oldXpos = -1;
 int oldYpos = -1;
 int oldZpos = -1;
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX,oldMinX;
 float minY,oldMinY;
 float minZ,oldMinZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 oldMinX = BaseBox.min.x - 1.0f;
 oldMinY = BaseBox.min.y - 1.0f;
 oldMinZ = BaseBox.min.z - 1.0f;



 BNDBOX polyPass = GetPolyBoundingBox(polyLoc);
 for(int loop=0;loop<3;loop++)
 {//mainloop
  if(BoundBoxPointCheck(polyLoc[loop],BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   Xpos = ConvertPositionToIndex(polyLoc[loop].x,boxSizeX[0],BaseBox.min.x,SizeSquaredX[0]);
   Ypos = ConvertPositionToIndex(polyLoc[loop].y,boxSizeY[0],BaseBox.min.y,SizeSquaredY[0]);
   Zpos = ConvertPositionToIndex(polyLoc[loop].z,boxSizeZ[0],BaseBox.min.z,SizeSquaredZ[0]);
   CurSubLevel = 0;
   GoodSubBox = &boxCollection[Xpos][Ypos][Zpos];
   CurrentSubBox = boxCollection[Xpos][Ypos][Zpos].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   
   while (CurrentSubBox != NULL)
   {
	
    minX = minX + (Xpos*boxSizeX[CurSubLevel]);
    minY = minY + (Ypos*boxSizeY[CurSubLevel]);
    minZ = minZ + (Zpos*boxSizeZ[CurSubLevel]);
    CurSubLevel++;

    Xpos = ConvertPositionToIndex(polyLoc[loop].x,boxSizeX[CurSubLevel],minX,SizeSquaredX[0]);
    Ypos = ConvertPositionToIndex(polyLoc[loop].y,boxSizeY[CurSubLevel],minY,SizeSquaredY[0]);
    Zpos = ConvertPositionToIndex(polyLoc[loop].z,boxSizeZ[CurSubLevel],minZ,SizeSquaredZ[0]);
	
	GoodSubBox = &CurrentSubBox[Xpos][Ypos][Zpos];
    CurrentSubBox = CurrentSubBox[Xpos][Ypos][Zpos].SubBoxCollection;
   }

   if(Xpos != oldXpos && Ypos != oldYpos && Zpos != oldZpos && CurSubLevel != oldCurSubLevel
      && minX != oldMinX && minY != oldMinY && minZ != oldMinZ)
   {//corner check
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
     if(howManyNormals < 10)
     {//howmanynormals
      //if(BoundingBoxToBoxCheck(polyPass,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
      //{//bounding collision check
       //filter by poly degree
       float deg = 91.0f;
       if(useAngle == TRUE)
       {
	    D3DVECTOR newNormalLoc;
	    newNormalLoc.x = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.x
                       + FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].x;
        newNormalLoc.y = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.y
                       + FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].y;
        newNormalLoc.z = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.z
                       + FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].z;
	    deg = ContactAngle(origin,virtualNormal,FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0],newNormalLoc);
       }
       //end filter by poly degree

		if(deg > 89)
		{
       	if(ExactColision(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,polyLoc,intersectX,intersectY,intersectZ)==TRUE)
        {//Has collided
	     *latestNormal = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
	     if(*highestYcontactPoint < *intersectY)
	      *highestYcontactPoint = *intersectY;

	     if(recordAllInfo == TRUE)
         {//record hit data
	      D3DVECTOR tempVectorLoc;
	      tempVectorLoc.x = FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].x;
          tempVectorLoc.y = FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].y;
          tempVectorLoc.z = FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].z;
	      storedDistance[howManyNormals] =
           SlideTranslation(FaceDatabank[GoodSubBox->refrenceList[loop]].normal,tempVectorLoc,polyLoc);
	      storedNormals[howManyNormals] = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
	      howManyNormals++;
	     }//end record hit data
	     everColided = TRUE;
	    }//end Has collided
	    collisionCheckCount++;
       }
      //}//end bounding collision check
     }//end howmanynormals
    }//pointcheck
    oldXpos = Xpos;
    oldYpos = Ypos;
    oldZpos = Zpos;
    oldMinX = minX;
    oldMinY = minY;
    oldMinZ = minZ;
    oldCurSubLevel = CurSubLevel;
   }//end corner check
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
 if(actualCollisions)
  *actualCollisions = collisionCheckCount;
 if(consideredCollision)
  *consideredCollision = collisionConsideration;
 if(everColided == TRUE)
  return TRUE;
 else
  return FALSE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::CollisionPolyRetrieval(
										  D3DVECTOR polyLoc[3],
					  float * intersectX,
					  float * intersectY,
					  float * intersectZ,
					  int *actualCollisions,
					  int *consideredCollision,
					  D3DVECTOR storedNormals[20],
					  float storedDistance[20],
                                          int & howManyNormals,
					  float * highestYcontactPoint,
					  BOOL recordAllInfo,
					  D3DVECTOR * latestNormal,
					  D3DVECTOR virtualNormal,
					  D3DVECTOR origin,
					  BOOL useAngle,
					  CPolyRetStruct polyRet[20]
					 )
{
 if(!boxCollection)
  return FALSE;
  
 int tracePolyLoc=0;

 BOOL everColided = FALSE;

 int Xpos,Ypos,Zpos;
 int oldXpos = -1;
 int oldYpos = -1;
 int oldZpos = -1;
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX,oldMinX;
 float minY,oldMinY;
 float minZ,oldMinZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 oldMinX = BaseBox.min.x - 1.0f;
 oldMinY = BaseBox.min.y - 1.0f;
 oldMinZ = BaseBox.min.z - 1.0f;



 BNDBOX polyPass = GetPolyBoundingBox(polyLoc);
 for(int loop=0;loop<3;loop++)
 {//mainloop
  if(BoundBoxPointCheck(polyLoc[loop],BaseBox.min,BaseBox.max)==TRUE)
  {//if in bounds
   Xpos = ConvertPositionToIndex(polyLoc[loop].x,boxSizeX[0],BaseBox.min.x,SizeSquaredX[0]);
   Ypos = ConvertPositionToIndex(polyLoc[loop].y,boxSizeY[0],BaseBox.min.y,SizeSquaredY[0]);
   Zpos = ConvertPositionToIndex(polyLoc[loop].z,boxSizeZ[0],BaseBox.min.z,SizeSquaredZ[0]);
   CurSubLevel = 0;
   GoodSubBox = &boxCollection[Xpos][Ypos][Zpos];
   CurrentSubBox = boxCollection[Xpos][Ypos][Zpos].SubBoxCollection;
   minX = BaseBox.min.x;
   minY = BaseBox.min.y;
   minZ = BaseBox.min.z;
   
   while (CurrentSubBox != NULL)
   {
	
    minX = minX + (Xpos*boxSizeX[CurSubLevel]);
    minY = minY + (Ypos*boxSizeY[CurSubLevel]);
    minZ = minZ + (Zpos*boxSizeZ[CurSubLevel]);
    CurSubLevel++;

    Xpos = ConvertPositionToIndex(polyLoc[loop].x,boxSizeX[CurSubLevel],minX,SizeSquaredX[0]);
    Ypos = ConvertPositionToIndex(polyLoc[loop].y,boxSizeY[CurSubLevel],minY,SizeSquaredY[0]);
    Zpos = ConvertPositionToIndex(polyLoc[loop].z,boxSizeZ[CurSubLevel],minZ,SizeSquaredZ[0]);
   	   
	/*if(Xpos > (SizeSquaredX[0]-1) || Ypos > (SizeSquaredY[0]-1) || Zpos > (SizeSquaredZ[0]-1)){
	   Xpos = (SizeSquaredX[0]-1);
	   Ypos = (SizeSquaredY[0]-1);
	   Zpos = (SizeSquaredZ[0]-1);

	}*/
	
	
	GoodSubBox = &CurrentSubBox[Xpos][Ypos][Zpos];
    CurrentSubBox = CurrentSubBox[Xpos][Ypos][Zpos].SubBoxCollection;
   }

   if(Xpos != oldXpos && Ypos != oldYpos && Zpos != oldZpos && CurSubLevel != oldCurSubLevel
      && minX != oldMinX && minY != oldMinY && minZ != oldMinZ)
   {//corner check
    for(int loop=0;loop<GoodSubBox->count;loop++)
    {//pointcheck
     collisionConsideration++;
     if(howManyNormals < 10)
     {//howmanynormals
      //if(BoundingBoxToBoxCheck(polyPass,FaceDatabank[GoodSubBox->refrenceList[loop]].box)==TRUE)
      //{//bounding collision check
       //filter by poly degree
       float deg = 91.0f;
       if(useAngle == TRUE)
       {
	    D3DVECTOR newNormalLoc;
	    newNormalLoc.x = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.x
                       + FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].x;
        newNormalLoc.y = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.y
                       + FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].y;
        newNormalLoc.z = FaceDatabank[GoodSubBox->refrenceList[loop]].normal.z
                       + FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].z;
	    deg = ContactAngle(origin,virtualNormal,FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0],newNormalLoc);
       }
       //end filter by poly degree

       if(deg > 89)
       {
       	if(ExactColision(FaceDatabank[GoodSubBox->refrenceList[loop]].polygon,polyLoc,intersectX,intersectY,intersectZ)==TRUE)
        {//Has collided
	     *latestNormal = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
	     if(*highestYcontactPoint < *intersectY)
	      *highestYcontactPoint = *intersectY;


         //output polys for shadowing
		 //GoodSubBox->refrenceList[loop]].polygon;
		 polyRet[tracePolyLoc].polygon[0] = FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0];
		 polyRet[tracePolyLoc].polygon[1] = FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[1];
		 polyRet[tracePolyLoc].polygon[2] = FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[2];
		 tracePolyLoc++;

	     if(recordAllInfo == TRUE)
         {//record hit data
	      D3DVECTOR tempVectorLoc;
	      tempVectorLoc.x = FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].x;
          tempVectorLoc.y = FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].y;
          tempVectorLoc.z = FaceDatabank[GoodSubBox->refrenceList[loop]].polygon[0].z;
	      storedDistance[howManyNormals] =
           SlideTranslation(FaceDatabank[GoodSubBox->refrenceList[loop]].normal,tempVectorLoc,polyLoc);
	      storedNormals[howManyNormals] = FaceDatabank[GoodSubBox->refrenceList[loop]].normal;
	      howManyNormals++;
	     }//end record hit data
	     everColided = TRUE;
	    }//end Has collided
	    collisionCheckCount++;
       }
      //}//end bounding collision check
     }//end howmanynormals
    }//pointcheck
    oldXpos = Xpos;
    oldYpos = Ypos;
    oldZpos = Zpos;
    oldMinX = minX;
    oldMinY = minY;
    oldMinZ = minZ;
    oldCurSubLevel = CurSubLevel;
   }//end corner check
  }//end if in bounds
//tally final info then output final resolutions
 }//end main loop
 if(actualCollisions)
  *actualCollisions = collisionCheckCount;
 if(consideredCollision)
  *consideredCollision = collisionConsideration;
 if(everColided == TRUE)
  return TRUE;
 else
  return FALSE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
void CCollisionObj::SafeDelete()
{
 if(m_wldObjectReferenceArray)
   delete [] m_wldObjectReferenceArray;
   m_wldObjectReferenceArray = 0;


 if(FaceDatabank)
  delete [] FaceDatabank;
 FaceDatabank=0;
 if(!SizeSquaredX)
	 return;
 
 int trace = 0;
         for (int xPtr=0; xPtr<SizeSquaredX[0]; xPtr++)
           for (int yPtr=0; yPtr<SizeSquaredY[0]; yPtr++)
            for (int zPtr=0; zPtr<SizeSquaredZ[0]; zPtr++)
			{//loop through all boxes in list
	         int oldCurSubLevel=-1;
             int CurSubLevel;
             //float minX,oldMinX;
             //float minY,oldMinY;
             //float minZ,oldMinZ;
             REFBOX ***CurrentSubBox;
	         CurSubLevel = 0;
             //save root boxes
		
			     if(boxCollection[xPtr][yPtr][zPtr].refrenceList)
					delete [] boxCollection[xPtr][yPtr][zPtr].refrenceList;
				    boxCollection[xPtr][yPtr][zPtr].refrenceList=0;
                 trace++;
			     if(boxCollection[xPtr][yPtr][zPtr].objectReferenceList)
				    delete [] boxCollection[xPtr][yPtr][zPtr].objectReferenceList;
				    boxCollection[xPtr][yPtr][zPtr].objectReferenceList=0;
				  
			 //end save root boxes
             CurrentSubBox = boxCollection[xPtr][yPtr][zPtr].SubBoxCollection;

			 if(CurrentSubBox != NULL){
                SubLevelDelete(CurrentSubBox);
			 }
                
			}//end loop through all boxes in list
   //delete 
   for (int xPos=0; xPos<SizeSquaredX[0]; xPos++){
     for (int yPos=0; yPos<SizeSquaredY[0]; yPos++){
		  delete [] boxCollection[xPos][yPos];
		  boxCollection[xPos][yPos]=0;
	 }
     delete [] boxCollection[xPos];
     boxCollection[xPos]=0;
   }
   delete [] boxCollection;
   boxCollection=0;



   
   if(SizeSquaredX)
   delete [] SizeSquaredX;
   SizeSquaredX=0;
   if(SizeSquaredY)
   delete [] SizeSquaredY;
   SizeSquaredY=0;
   if(SizeSquaredZ)
   delete [] SizeSquaredZ;
   SizeSquaredZ=0;
   if(boxSizeX)
   delete [] boxSizeX;
   boxSizeX=0;
   if(boxSizeY)
   delete [] boxSizeY;
   boxSizeY=0;
   if(boxSizeZ)
   delete [] boxSizeZ;
   boxSizeZ=0;

}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
void CCollisionObj::SubLevelDelete(REFBOX ***CurrentSubBox)
{
	int xPos,yPos,zPos;
                 for (xPos=0; xPos<SizeSquaredX[0]; xPos++)
                  for (yPos=0; yPos<SizeSquaredY[0]; yPos++)
                   for (zPos=0; zPos<SizeSquaredZ[0]; zPos++)
				   {//loop through all boxes inside list 	   
                      //save box data
			
				         if(CurrentSubBox[xPos][yPos][zPos].refrenceList)
							delete [] CurrentSubBox[xPos][yPos][zPos].refrenceList;
						    CurrentSubBox[xPos][yPos][zPos].refrenceList=0;

						 if(CurrentSubBox[xPos][yPos][zPos].objectReferenceList)
							delete [] CurrentSubBox[xPos][yPos][zPos].objectReferenceList;
						    CurrentSubBox[xPos][yPos][zPos].objectReferenceList=0;
					      
					     if(CurrentSubBox[xPos][yPos][zPos].SubBoxCollection != NULL)
						 SubLevelDelete(CurrentSubBox[xPos][yPos][zPos].SubBoxCollection);
						 //CurrentSubBox = CurrentSubBox[xPos][yPos][zPos].SubBoxCollection;
				   }//end loop through all boxes inside list 

				   for (xPos=0; xPos<SizeSquaredX[0]; xPos++){
					 for (int yPos=0; yPos<SizeSquaredY[0]; yPos++){
					      delete [] CurrentSubBox[xPos][yPos];
						  CurrentSubBox[xPos][yPos]=0;
					 }
					 delete [] CurrentSubBox[xPos];
					 CurrentSubBox[xPos]=0;
				   }
				   delete [] CurrentSubBox;
				   CurrentSubBox=0;

}

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::GenerateTable(CDialog * m_progressInternal)
{
 int xPtr;
 int yPtr;
 int zPtr;
 int faceCntr;
 int barSwitch = 0;
 //int * BoxPtrArray;

 boxSizeX[0] = (BaseBox.max.x - BaseBox.min.x) / SizeSquaredX[0];
 boxSizeY[0] = (BaseBox.max.y - BaseBox.min.y) / SizeSquaredY[0];
 boxSizeZ[0] = (BaseBox.max.z - BaseBox.min.z) / SizeSquaredZ[0];

 boxCollection = new REFBOX**[SizeSquaredX[0]];
 for (int m=0; m<SizeSquaredX[0]; m++)
 {
  boxCollection[m] = new REFBOX*[SizeSquaredY[0]];
  for (int n=0; n<SizeSquaredY[0]; n++)
  {
   boxCollection[m][n] = new REFBOX[SizeSquaredZ[0]];
  }
 }

 int posCntrolTotal = SizeSquaredX[0]*SizeSquaredY[0]*SizeSquaredZ[0];
 
 


 m_progressInternal->SendDlgItemMessage(IDC_PROGRESS_METER,PBM_SETRANGE ,0,MAKELPARAM(0,posCntrolTotal));
 m_progressInternal->SendDlgItemMessage(IDC_PROGRESS_METER,PBM_SETBARCOLOR,0,RGB(0,100,0));
 m_progressInternal->SendDlgItemMessage(IDC_PROGRESS_SUBLEVEL,PBM_SETBARCOLOR,0,RGB(255,0,255));


 GL_barTrace=0;
 for (xPtr=0; xPtr<SizeSquaredX[0]; xPtr++)
  for (yPtr=0; yPtr<SizeSquaredY[0]; yPtr++)
   for (zPtr=0; zPtr<SizeSquaredZ[0]; zPtr++)
   {//box illiteration
    GL_barTrace++;
    
	m_progressInternal->SendDlgItemMessage(IDC_PROGRESS_METER,PBM_SETPOS,GL_barTrace,0);
    //allocate link list
    //boxCollection[xPtr][yPtr][zPtr].refrenceList = new CBoxObjList;
    boxCollection[xPtr][yPtr][zPtr].refrenceList = new int[2];
	boxCollection[xPtr][yPtr][zPtr].objectReferenceCount = 0;
	boxCollection[xPtr][yPtr][zPtr].objectReferenceList=new WORD[2];
    boxCollection[xPtr][yPtr][zPtr].count = 0;
    boxCollection[xPtr][yPtr][zPtr].SubTableLevel = 0;
	boxCollection[xPtr][yPtr][zPtr].SubBoxCollection = NULL;
    BNDBOX tempBoxOla;
    tempBoxOla.max.x =  (xPtr+1)*boxSizeX[0] + BaseBox.min.x;//
    tempBoxOla.min.x =  (xPtr)*boxSizeX[0]   + BaseBox.min.x;//
    tempBoxOla.max.y =  (yPtr+1)*boxSizeY[0] + BaseBox.min.y;//
    tempBoxOla.min.y =  (yPtr)*boxSizeY[0]   + BaseBox.min.y;//
    tempBoxOla.max.z =  (zPtr+1)*boxSizeZ[0] + BaseBox.min.z;//
    tempBoxOla.min.z =  (zPtr)*boxSizeZ[0]   + BaseBox.min.z;//


	//---------------//
     //------------------------
     //------------------------
     //assign popout assignment
     if(GL_worldList){//valid worldist Global
        int indexTrace = 0;
		int count = 0;
           for(POSITION posLoc = GL_worldList->GetHeadPosition();posLoc != NULL;){//1
               CWldObject * wldObj = (CWldObject *)GL_worldList->GetNext(posLoc);		
	           if(BoundingBoxToBoxCheck(wldObj->List_popoutBox,tempBoxOla)==TRUE || 
				  wldObj->List_environmentAttached == TRUE)
			   {//bounding collision check
		          //check list if object exists in the list
                  BOOL itAlreadyExists = FALSE;
		          for(count=0;count<boxCollection[xPtr][yPtr][zPtr].objectReferenceCount;count++){
			        if(boxCollection[xPtr][yPtr][zPtr].objectReferenceList[count] == indexTrace){//found a copy
                    itAlreadyExists = TRUE;
				    break;
					}//end found a copy
				  }
				  if(itAlreadyExists == FALSE){
		          //reallocate delete and add ;)
                  WORD * newList = new WORD[(boxCollection[xPtr][yPtr][zPtr].objectReferenceCount+1)];
		          for(count=0;count<boxCollection[xPtr][yPtr][zPtr].objectReferenceCount;count++)
			        newList[count] = boxCollection[xPtr][yPtr][zPtr].objectReferenceList[count];
		            delete [] boxCollection[xPtr][yPtr][zPtr].objectReferenceList;
		            boxCollection[xPtr][yPtr][zPtr].objectReferenceList=0;
                    boxCollection[xPtr][yPtr][zPtr].objectReferenceCount++;
		            newList[(boxCollection[xPtr][yPtr][zPtr].objectReferenceCount-1)] = indexTrace;//index of object in db
		            boxCollection[xPtr][yPtr][zPtr].objectReferenceList = newList;  
				  }
			   }//end bounding collision check
	           indexTrace++;
		   }//end 1
	 }//end valid worldist Global
     //end assign popout assignment
	 //------------------------
	 //------------------------


    for (faceCntr=0; faceCntr<FaceDataBankCount; faceCntr++)
    {//loop through all polys
     //---------------//
	 if(directKeyboard != NULL)
	 {//valid
     BYTE  globalForceKeys[256]; 
		if ( directKeyboard->GetDeviceState(256, globalForceKeys) == DI_OK)
		{
			if(globalForceKeys[DIK_ESCAPE] & 0x80){
				AfxMessageBox("Aborted");
				boxCollection = NULL;
				//boxCollection->SubBoxCollection=NULL;
				return FALSE;
			}
		}
	 }//valid

	 
	 
	 if(BoundingBoxToBoxCheck(GetPolyBoundingBox(FaceDatabank[faceCntr].polygon),tempBoxOla)==TRUE)
     {
        
	  boxCollection[xPtr][yPtr][zPtr].count++;
      int * newList = new int[boxCollection[xPtr][yPtr][zPtr].count];
      for(int loop=0;loop<(boxCollection[xPtr][yPtr][zPtr].count-1);loop++)
       newList[loop] = boxCollection[xPtr][yPtr][zPtr].refrenceList[loop];
      newList[(boxCollection[xPtr][yPtr][zPtr].count-1)] = faceCntr;
      delete [] boxCollection[xPtr][yPtr][zPtr].refrenceList;
      boxCollection[xPtr][yPtr][zPtr].refrenceList = 0;
      boxCollection[xPtr][yPtr][zPtr].refrenceList = newList;
	 }
    }//end loop through all polys
    if(boxCollection[xPtr][yPtr][zPtr].count > MaxFacesInBox)
    {
     //If the number of faces assigned to the box is greater than MaxFacesInBox
     //then create a subset of boxes.
	 if(GenerateSubTable(&boxCollection[xPtr][yPtr][zPtr], &tempBoxOla,m_progressInternal)==FALSE)
	 return FALSE;
    }
   }

   return TRUE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::GenerateSubTable(REFBOX *SubWorld,
                                     BNDBOX *SubWorldSize,
									 CDialog * m_subLevelProgressInternal
                                    )
{
 int barSwitch = 0;
 int xPtr;
 int yPtr;
 int zPtr;
 int faceCntr;
 int facePtr;
 int SubLevel;
 //int * BoxPtrArray;
 SubLevel = SubWorld->SubTableLevel + 1;

 boxSizeX[SubLevel] = (SubWorldSize->max.x - SubWorldSize->min.x) / SizeSquaredX[SubLevel];
 boxSizeY[SubLevel] = (SubWorldSize->max.y - SubWorldSize->min.y) / SizeSquaredY[SubLevel];
 boxSizeZ[SubLevel] = (SubWorldSize->max.z - SubWorldSize->min.z) / SizeSquaredZ[SubLevel];

 SubWorld->SubBoxCollection = new REFBOX**[SizeSquaredX[SubLevel]];
 for (int m=0; m<SizeSquaredX[SubLevel]; m++)
 {
  SubWorld->SubBoxCollection[m] = new REFBOX*[SizeSquaredY[SubLevel]];
  for (int n=0; n<SizeSquaredY[0]; n++)
  {
   SubWorld->SubBoxCollection[m][n] = new REFBOX[SizeSquaredZ[SubLevel]];
  }
 }

 int posCntrolTotal = SizeSquaredX[0]*SizeSquaredY[0]*SizeSquaredZ[0];



 m_subLevelProgressInternal->SendDlgItemMessage(IDC_PROGRESS_SUBLEVEL,PBM_SETRANGE ,0,MAKELPARAM(0,posCntrolTotal));

 int tracer = 0;

 for (xPtr=0; xPtr<SizeSquaredX[SubLevel]; xPtr++)
  for (yPtr=0; yPtr<SizeSquaredY[SubLevel]; yPtr++)
   for (zPtr=0; zPtr<SizeSquaredZ[SubLevel]; zPtr++)
   {//box illiteration
    tracer++;

	m_subLevelProgressInternal->SendDlgItemMessage(IDC_PROGRESS_SUBLEVEL,PBM_SETPOS,tracer,0);
    //allocate link list
	SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceCount = 0;
	SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceList = NULL;
    SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].refrenceList = new int[2];
    SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].count = 0;
    SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].SubTableLevel = SubLevel;
	SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].SubBoxCollection = NULL;
    BNDBOX tempBoxOla;
    tempBoxOla.max.x =  (xPtr+1)*boxSizeX[SubLevel] + SubWorldSize->min.x;//
    tempBoxOla.min.x =  (xPtr)*boxSizeX[SubLevel]   + SubWorldSize->min.x;//
    tempBoxOla.max.y =  (yPtr+1)*boxSizeY[SubLevel] + SubWorldSize->min.y;//
    tempBoxOla.min.y =  (yPtr)*boxSizeY[SubLevel]   + SubWorldSize->min.y;//
    tempBoxOla.max.z =  (zPtr+1)*boxSizeZ[SubLevel] + SubWorldSize->min.z;//
    tempBoxOla.min.z =  (zPtr)*boxSizeZ[SubLevel]   + SubWorldSize->min.z;//
    
	//---------------//
     //------------------------
     //------------------------
     //assign popout assignment
     if(GL_worldList){//valid worldist Global
        int indexTrace = 0;
		int count = 0;

           for(POSITION posLoc = GL_worldList->GetHeadPosition();posLoc != NULL;){//1
               CWldObject * wldObj = (CWldObject *)GL_worldList->GetNext(posLoc);		
	           if(BoundingBoxToBoxCheck(wldObj->List_popoutBox,tempBoxOla)==TRUE || 
				  wldObj->List_environmentAttached == TRUE)
			   {//bounding collision check
		          //check list if object exists in the list
                  BOOL itAlreadyExists = FALSE;
		           for(count=0;count<SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceCount;count++){
			        if(SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceList[count] == indexTrace){//found a copy
                    itAlreadyExists = TRUE;
					break;
					}//end found a copy
				   }
				  if(itAlreadyExists == FALSE){
		          //reallocate delete and add ;)
                  WORD * newList = new WORD[(SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceCount+1)];
		          for(count=0;count<SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceCount;count++)
			        newList[count] = SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceList[count];
		            delete [] SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceList;
		            SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceList=0;
                    SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceCount++;
		            newList[(SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceCount-1)] = indexTrace;//index of object in db
		            SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].objectReferenceList = newList;   
				  }
			   }//end bounding collision check
	           indexTrace++;
		   }//end 1
	 }//end valid worldist Global
     //end assign popout assignment
	 //------------------------
	 //------------------------
	
	
	
	
	for (facePtr=0; facePtr<SubWorld->count; facePtr++)
    {//loop through all polys
     faceCntr = SubWorld->refrenceList[facePtr];
	 if(directKeyboard != NULL){//valid
     BYTE  globalForceKeys[256]; 
	   if ( directKeyboard->GetDeviceState(256, globalForceKeys) == DI_OK){
         if(globalForceKeys[DIK_ESCAPE] & 0x80){
	          AfxMessageBox("Aborted");
			  SubWorld->SubBoxCollection=NULL;
			  SubWorld = NULL;
			  return FALSE;
		 }
	   }
	 }//valid

	 
     /*float xAdjust = boxSizeX[SubLevel]*.2f;
     float yAdjust = boxSizeY[SubLevel]*.2f;
     float zAdjust = boxSizeZ[SubLevel]*.2f;*/
     BNDBOX crazedOla;
	 crazedOla.min.x = tempBoxOla.min.x - BoxZoneAdjustManual;
	 crazedOla.min.y = tempBoxOla.min.y - BoxZoneAdjustManual;
	 crazedOla.min.z = tempBoxOla.min.z - BoxZoneAdjustManual;
	 crazedOla.max.x = tempBoxOla.max.x + BoxZoneAdjustManual;
	 crazedOla.max.y = tempBoxOla.max.y + BoxZoneAdjustManual;
	 crazedOla.max.z = tempBoxOla.max.z + BoxZoneAdjustManual;
  
     if(BoundingBoxToBoxCheck(GetPolyBoundingBox(FaceDatabank[faceCntr].polygon),crazedOla)==TRUE)
     {
      SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].count++;
      int * newList = new int[SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].count];
      for(int loop=0;loop<(SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].count-1);loop++)
       newList[loop] = SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].refrenceList[loop];
      newList[(SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].count-1)] = faceCntr;
      delete [] SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].refrenceList;
      SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].refrenceList = 0;
      SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].refrenceList = newList;
	 }
    }//end loop through all polys
	
    if((SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].count > MaxFacesInBox)
       && (SubWorld->SubBoxCollection[xPtr][yPtr][zPtr].SubTableLevel < (NumberOfLevels-1)))
    {
     //If the number of faces assigned to the box is greater than MaxFacesInBox
     //then create a subset of boxes.
		if(GenerateSubTable(&SubWorld->SubBoxCollection[xPtr][yPtr][zPtr],&tempBoxOla,m_subLevelProgressInternal)==FALSE){
			return FALSE;
		}
    }
   }
   return TRUE;

}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
 int CCollisionObj::ConvertPositionToIndex(float & position,float & dimension,float & startPoint,int & endPoint)
{
	if(dimension != 0){
		int indexer = (int)(((position-startPoint) / dimension));

        if(indexer >= endPoint)
           indexer = endPoint-1;

		return indexer;
	}
	return 0;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
BOOL CCollisionObj::CalculateWorld(CWldObjectList * worldList,
				                   int squaredX,
				                   int squaredY,
				                   int squaredZ,
				                   CReferenceObjList * reference,
								   CDialog * m_subLevelProgressInternal)
{
 if(boxCollection)
 {
  SafeDelete();
 }


 SizeSquaredX = new int[NumberOfLevels];
 SizeSquaredY = new int[NumberOfLevels];
 SizeSquaredZ = new int[NumberOfLevels];
 boxSizeX = new float[NumberOfLevels];
 boxSizeY = new float[NumberOfLevels];
 boxSizeZ = new float[NumberOfLevels];
 for(int cntr=0; cntr<NumberOfLevels; cntr++)
 {
  SizeSquaredX[cntr] = squaredX;
  SizeSquaredY[cntr] = squaredY;
  SizeSquaredZ[cntr] = squaredZ;
 }

 int sizeTrack = 0;
 BOOL initedWorldBox = FALSE;
 int faceDataVetexTrace = 0;
 int barSwitch = 0;
 for(int loop=0;loop<2;loop++)
 {//size track loop
  int countTraceLocal = 0;
  for(POSITION posLoc = worldList->GetHeadPosition();posLoc != NULL;)
  {//world object loop
   CWldObject * wldObj = (CWldObject *)worldList->GetNext(posLoc);
   CEXMeshObj * meshReference = NULL;
   ABVERTEX * vArray = NULL;
   

   D3DMATRIX m_adjustmentMatrix;
   GL_matrixFunctions.InitMat(m_adjustmentMatrix.m);
   
   if(wldObj->m_meshObject)
   {
      meshReference = wldObj->m_meshObject;
   }
   else if(wldObj->m_meshObject == NULL && wldObj->m_node != NULL)
   {
       CReferenceObj * libraryref = reference->GetByIndex(wldObj->m_node->m_libraryReference);
       if(libraryref){//valid referenece
		   if(libraryref->m_collisionModel){
              meshReference = libraryref->m_collisionModel;
			  m_adjustmentMatrix = wldObj->m_node->m_position;
		   }
		   else{
		    countTraceLocal++;
			continue;
		   }
	   }//end valid referenece
	   else{
          AfxMessageBox("Invalid reference");
	   }
   }
   else{
	   countTraceLocal++;
	    continue; 
   }

   int countV = 0;
   meshReference->m_abVertexArray.GetABVERTEXStyle(&vArray,&countV);

   if(wldObj->List_environmentAttached == FALSE && wldObj->m_collisionInfoFilter == FALSE) 
   {//has no animations
      int verTraceIndex = 0;
      int faceCount = 0;
      
      if(meshReference->m_abVertexArray.m_indecieCount > 0)
       faceCount = meshReference->m_abVertexArray.m_indecieCount/3;
      for(int indecieIndex=0;indecieIndex<faceCount;indecieIndex++)
      {//face loop
       if(loop == 0)//count size
        sizeTrack++;
       else{//add faces
        for(int index=0;index<3;index++)
        {//begin face loop
         //D3DVERTEX vetXloc = wldObj->m_meshObject->m_abVertexArray.m_origVertexArray[wldObj->m_meshObject->m_abVertexArray.m_indecieArray[verTraceIndex]];
         D3DVECTOR vertex;
		  
         vertex.x = vArray[meshReference->m_abVertexArray.m_indecieArray[verTraceIndex]].x;
         vertex.y = vArray[meshReference->m_abVertexArray.m_indecieArray[verTraceIndex]].y;
         vertex.z = vArray[meshReference->m_abVertexArray.m_indecieArray[verTraceIndex]].z;
         
		 vertex = GL_matrixFunctions.UpdatePolygonVertex(m_adjustmentMatrix.m,vertex);
		 
		 
		 //define world box
         if(initedWorldBox == FALSE)
         {//init them
	  BaseBox.max.x = vertex.x;
	  BaseBox.max.y = vertex.y;
	  BaseBox.max.z = vertex.z;
	  BaseBox.min.x = vertex.x;
	  BaseBox.min.y = vertex.y;
	  BaseBox.min.z = vertex.z;
	  initedWorldBox = TRUE;
	 }//end init them
	 else
         {//adjust
          if(vertex.x > BaseBox.max.x)
	   BaseBox.max.x = vertex.x;
	   if(vertex.y > BaseBox.max.y)
	    BaseBox.max.y = vertex.y;
	   if(vertex.z > BaseBox.max.z)
	    BaseBox.max.z = vertex.z;
           if(vertex.x < BaseBox.min.x)
	    BaseBox.min.x = vertex.x;
	   if(vertex.y < BaseBox.min.y)
	    BaseBox.min.y = vertex.y;
	   if(vertex.z < BaseBox.min.z)
	    BaseBox.min.z = vertex.z;
	   }//end adjust
	    //end define world box
       FaceDatabank[faceDataVetexTrace].m_objectIndexRef = countTraceLocal;
	   FaceDatabank[faceDataVetexTrace].polygon[index] = vertex;
	   //FaceDatabank[faceDataVetexTrace].box = GetPolyBoundingBox(FaceDatabank[faceDataVetexTrace].polygon);
	   verTraceIndex++;
	   
	  }//end verticie loop
	  //create perp normal for collision not rendering ;)
          D3DVECTOR XVEC,AXVECT,VEC1,VEC2;
	  float dTemp;

	  VEC1.x = FaceDatabank[faceDataVetexTrace].polygon[1].x
                 - FaceDatabank[faceDataVetexTrace].polygon[0].x;
	  VEC1.y = FaceDatabank[faceDataVetexTrace].polygon[1].y
           	 - FaceDatabank[faceDataVetexTrace].polygon[0].y;
          VEC1.z = FaceDatabank[faceDataVetexTrace].polygon[1].z
           	 - FaceDatabank[faceDataVetexTrace].polygon[0].z;

	  VEC2.x = FaceDatabank[faceDataVetexTrace].polygon[2].x
           	 - FaceDatabank[faceDataVetexTrace].polygon[0].x;
	  VEC2.y = FaceDatabank[faceDataVetexTrace].polygon[2].y
           	 - FaceDatabank[faceDataVetexTrace].polygon[0].y;
	  VEC2.z = FaceDatabank[faceDataVetexTrace].polygon[2].z
           	 - FaceDatabank[faceDataVetexTrace].polygon[0].z;

	  XVEC = GL_matrixFunctions.CrossProduct(VEC1,VEC2);
	  dTemp = (float)sqrt(XVEC.x * XVEC.x + XVEC.y
            	 * XVEC.y + XVEC.z * XVEC.z);
          AXVECT.x = XVEC.x/dTemp;
	  AXVECT.y = XVEC.y/dTemp;
	  AXVECT.z = XVEC.z/dTemp;
	  FaceDatabank[faceDataVetexTrace].normal = AXVECT;
	  faceDataVetexTrace++;
	 }//end add faces
	}//end face loop
   }//has no animations
	 countTraceLocal++;
     if(vArray)
        delete [] vArray;
	    vArray = 0;
     }//end world object loop
     if(FaceDatabank != NULL && loop == 0)
     {
      delete [] FaceDatabank;
      FaceDatabank = 0;
      FaceDatabank = new CFaceStruct[sizeTrack];
      FaceDataBankCount = sizeTrack;
     }
     else if(loop == 0)
     {
      FaceDatabank = new CFaceStruct[sizeTrack];
      FaceDataBankCount = sizeTrack;
     }
    }//end size track loop

    if(GenerateTable(m_subLevelProgressInternal)==FALSE)//Generate table
       return FALSE;


	//if(m_updateWnd)
    //m_updateWnd->SendDlgItemMessage(IDC_PROGRESS_OBJ,PBM_SETPOS,0,0);
    return TRUE;
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
void CCollisionObj::SubLevelSave(REFBOX ***CurrentSubBox,CArchive& ar)
{
	             int loop=0;
                 for (int xPos=0; xPos<SizeSquaredX[0]; xPos++)
                  for (int yPos=0; yPos<SizeSquaredY[0]; yPos++)
                   for (int zPos=0; zPos<SizeSquaredZ[0]; zPos++)
				   {//loop through all boxes inside list 	   
                      //save box data	  
					     if(CurrentSubBox[xPos][yPos][zPos].SubBoxCollection == NULL){//if bottom level save data
					     ar << 1;
						 ar << CurrentSubBox[xPos][yPos][zPos].count;
			             ar << CurrentSubBox[xPos][yPos][zPos].SubTableLevel;
						 ar << CurrentSubBox[xPos][yPos][zPos].objectReferenceCount;
							 for(loop=0;loop<CurrentSubBox[xPos][yPos][zPos].count;loop++)
					         ar << CurrentSubBox[xPos][yPos][zPos].refrenceList[loop];
							 for(loop=0;loop<CurrentSubBox[xPos][yPos][zPos].objectReferenceCount;loop++)
					         ar << CurrentSubBox[xPos][yPos][zPos].objectReferenceList[loop];
						 }//end if bottom level save data
						 else{//not bottom level
						 ar << 0;
						 SubLevelSave(CurrentSubBox[xPos][yPos][zPos].SubBoxCollection,ar);
						 }
						 //CurrentSubBox = CurrentSubBox[xPos][yPos][zPos].SubBoxCollection;
				   }//end loop through all boxes inside list 
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
void CCollisionObj::SubLevelLoad(REFBOX ***CurrentSubBox,CArchive& ar)
{
	             int loop=0;
                 for (int xPos=0; xPos<SizeSquaredX[0]; xPos++)
                  for (int yPos=0; yPos<SizeSquaredY[0]; yPos++)
                   for (int zPos=0; zPos<SizeSquaredZ[0]; zPos++)
				   {//loop through all boxes inside list 	   
                      //save box data
					     //if(CurrentSubBox[xPos][yPos][zPos].SubTableLevel == (NumberOfLevels+1)){
					     int bottomLine = 1; 
						     ar >> bottomLine;
						 if(bottomLine == 1){
							 //int cast = 0;
						     ar >> CurrentSubBox[xPos][yPos][zPos].count;
			                 ar >> CurrentSubBox[xPos][yPos][zPos].SubTableLevel;
							 ar >> CurrentSubBox[xPos][yPos][zPos].objectReferenceCount;
					        if(CurrentSubBox[xPos][yPos][zPos].count > 0)
					           CurrentSubBox[xPos][yPos][zPos].refrenceList = new int[CurrentSubBox[xPos][yPos][zPos].count]; 
						    else
						       CurrentSubBox[xPos][yPos][zPos].refrenceList = new int[2];

							if(CurrentSubBox[xPos][yPos][zPos].objectReferenceCount > 0){
					           CurrentSubBox[xPos][yPos][zPos].objectReferenceList = new WORD[CurrentSubBox[xPos][yPos][zPos].objectReferenceCount]; 
							}
						    else{
					           CurrentSubBox[xPos][yPos][zPos].objectReferenceList = new WORD[2];
							}
                            //CurrentSubBox[xPos][yPos][zPos].objectReferenceCount=0;


							    for(loop=0;loop<CurrentSubBox[xPos][yPos][zPos].count;loop++){
				                    ar >> CurrentSubBox[xPos][yPos][zPos].refrenceList[loop];
								}
                                
								for(loop=0;loop<CurrentSubBox[xPos][yPos][zPos].objectReferenceCount;loop++){
				                    ar >> CurrentSubBox[xPos][yPos][zPos].objectReferenceList[loop];
								}
							        //CurrentSubBox[xPos][yPos][zPos].objectReferenceList=0;

								CurrentSubBox[xPos][yPos][zPos].SubBoxCollection = NULL;
								//CurrentSubBox[xPos][yPos][zPos].objectReferenceCount = 0;
								//CurrentSubBox[xPos][yPos][zPos].objectReferenceList = 0;
						 }
						 else{
					     CurrentSubBox[xPos][yPos][zPos].refrenceList = new int[2];
						 CurrentSubBox[xPos][yPos][zPos].objectReferenceList = new WORD[2];
						 CurrentSubBox[xPos][yPos][zPos].objectReferenceCount=0;
						 CurrentSubBox[xPos][yPos][zPos].count=0;
                           CurrentSubBox[xPos][yPos][zPos].SubBoxCollection = new REFBOX**[SizeSquaredX[0]];
                            for (int m=0; m<SizeSquaredX[0]; m++)
							{
                              CurrentSubBox[xPos][yPos][zPos].SubBoxCollection[m] = new REFBOX*[SizeSquaredY[0]];
                              for (int n=0; n<SizeSquaredY[0]; n++)
                              CurrentSubBox[xPos][yPos][zPos].SubBoxCollection[m][n] = new REFBOX[SizeSquaredZ[0]];
							}
						    SubLevelLoad(CurrentSubBox[xPos][yPos][zPos].SubBoxCollection,ar);
						 }
						 //CurrentSubBox = CurrentSubBox[xPos][yPos][zPos].SubBoxCollection;
				   }//end loop through all boxes inside list 
}
//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//
void CCollisionObj::Serialize( CArchive& ar )
{
 int dump = 0;
 CObject::Serialize( ar );
  if(ar.IsStoring() )
  {
   int hasContent = 0;
   if(SizeSquaredX)
       hasContent = 1;
   ar  << FaceDataBankCount
	   << NumberOfLevels
       << MaxFacesInBox
       << BaseBox.max.x
       << BaseBox.max.y
       << BaseBox.max.z
       << BaseBox.min.x
       << BaseBox.min.y
       << BaseBox.min.z
	   << hasContent;
   //save poly array
   for(int i=0;i<FaceDataBankCount;i++)
   {
    ar  << FaceDatabank[i].normal.x
        << FaceDatabank[i].normal.y
        << FaceDatabank[i].normal.z
        //poly
        << FaceDatabank[i].polygon[0].x
        << FaceDatabank[i].polygon[0].y
        << FaceDatabank[i].polygon[0].z
        << FaceDatabank[i].polygon[1].x
        << FaceDatabank[i].polygon[1].y
        << FaceDatabank[i].polygon[1].z
        << FaceDatabank[i].polygon[2].x
        << FaceDatabank[i].polygon[2].y
        << FaceDatabank[i].polygon[2].z
		//object reference
		<< FaceDatabank[i].m_objectIndexRef;
   }
   

   //--------save the shiat arrays------//
     if(hasContent == 1){//has content
	   for(int aIndex=0;aIndex<NumberOfLevels;aIndex++){
           ar  << SizeSquaredX[aIndex]
	           << SizeSquaredY[aIndex]
	           << SizeSquaredZ[aIndex]
		       << boxSizeX[aIndex]
               << boxSizeY[aIndex]
               << boxSizeZ[aIndex];
	   }
   
          //---------------------//
	      //---------------------//
	      int loop=0;
          for (int xPtr=0; xPtr<SizeSquaredX[0]; xPtr++)
           for (int yPtr=0; yPtr<SizeSquaredY[0]; yPtr++)
            for (int zPtr=0; zPtr<SizeSquaredZ[0]; zPtr++)
			{//loop through all boxes in list
	         int oldCurSubLevel=-1;
             int CurSubLevel;
             //float minX,oldMinX;
             //float minY,oldMinY;
             //float minZ,oldMinZ;
             REFBOX ***CurrentSubBox;
	         CurSubLevel = 0;
             //save root boxes
			 
			 if(boxCollection[xPtr][yPtr][zPtr].SubBoxCollection == NULL){//bottom level save data
			 ar << 1;
			 ar << boxCollection[xPtr][yPtr][zPtr].count;
			 ar << boxCollection[xPtr][yPtr][zPtr].SubTableLevel;
			 ar << boxCollection[xPtr][yPtr][zPtr].objectReferenceCount;
				 for(loop=0;loop<boxCollection[xPtr][yPtr][zPtr].count;loop++)
				    ar << boxCollection[xPtr][yPtr][zPtr].refrenceList[loop];

				 for(loop=0;loop<boxCollection[xPtr][yPtr][zPtr].objectReferenceCount;loop++)
					 ar << boxCollection[xPtr][yPtr][zPtr].objectReferenceList[loop];
			 }//end bottom level save data
			 else{//flag as not bottom level
			 ar << 0;
			 CurrentSubBox = boxCollection[xPtr][yPtr][zPtr].SubBoxCollection;
             SubLevelSave(CurrentSubBox,ar);
			 }
			 //end save root boxes
			}//end loop through all boxes in list
	 }//end has content
   
    }
    else
    {//loading
	 GL_barSwitch = FALSE;
     GL_barTrace = 0;
	 BoxZoneAdjustManual = 2;
     FaceDatabank = NULL;
     NumberOfLevels =  4;
     MaxFacesInBox = 10;
     FaceDataBankCount = 0;
     refBoxCollectionCount = 0;
     GL_lastBarUpdate = 0;
     SizeSquaredX = NULL;
     SizeSquaredY = NULL;
     SizeSquaredZ = NULL;
     boxSizeX = NULL;
     boxSizeY = NULL;
     boxSizeZ = NULL;
     GL_worldList = NULL;
     m_updateWnd = NULL;
     directKeyboard = NULL;
     boxCollection = NULL;

	 int hasContent = 0;
     ar  >> FaceDataBankCount
	     >> NumberOfLevels
         >> MaxFacesInBox
         >> BaseBox.max.x
         >> BaseBox.max.y
         >> BaseBox.max.z
         >> BaseBox.min.x
         >> BaseBox.min.y
         >> BaseBox.min.z
		 >> hasContent;
	 //load poly array
     if(FaceDataBankCount > 0)
        FaceDatabank = new CFaceStruct[FaceDataBankCount];

     for(int i=0;i<FaceDataBankCount;i++)
     {
      ar  >> FaceDatabank[i].normal.x
          >> FaceDatabank[i].normal.y
          >> FaceDatabank[i].normal.z
          //poly
          >> FaceDatabank[i].polygon[0].x
          >> FaceDatabank[i].polygon[0].y
          >> FaceDatabank[i].polygon[0].z
          >> FaceDatabank[i].polygon[1].x
          >> FaceDatabank[i].polygon[1].y
          >> FaceDatabank[i].polygon[1].z
          >> FaceDatabank[i].polygon[2].x
          >> FaceDatabank[i].polygon[2].y
          >> FaceDatabank[i].polygon[2].z
		  //object reference
		  >> FaceDatabank[i].m_objectIndexRef;



      //FaceDatabank[i].box = GetPolyBoundingBox(FaceDatabank[i].polygon);
     }

    //--------load the shiat arrays------//
    if(hasContent == 1){//has content
     SizeSquaredX = new int[NumberOfLevels];
     SizeSquaredY = new int[NumberOfLevels];
     SizeSquaredZ = new int[NumberOfLevels];
     boxSizeX = new float[NumberOfLevels];
     boxSizeY = new float[NumberOfLevels];
     boxSizeZ = new float[NumberOfLevels];
	 for(int aIndex=0;aIndex<NumberOfLevels;aIndex++){
           ar  >> SizeSquaredX[aIndex]
	           >> SizeSquaredY[aIndex]
	           >> SizeSquaredZ[aIndex]
		       >> boxSizeX[aIndex]
               >> boxSizeY[aIndex]
               >> boxSizeZ[aIndex];
	 }
	 //---------------------//
	 //---------------------//
	 //allocate for root boxes
	   boxCollection = new REFBOX**[SizeSquaredX[0]];
       for (int m=0; m<SizeSquaredX[0]; m++)
	   {
         boxCollection[m] = new REFBOX*[SizeSquaredY[0]];
         for (int n=0; n<SizeSquaredY[0]; n++)
		 {
           boxCollection[m][n] = new REFBOX[SizeSquaredZ[0]];
		 }
	   }
	   //---------
	      int loop = 0;
	      for (int xPtr=0; xPtr<SizeSquaredX[0]; xPtr++)
           for (int yPtr=0; yPtr<SizeSquaredY[0]; yPtr++)
            for (int zPtr=0; zPtr<SizeSquaredZ[0]; zPtr++)
			{//loop through all boxes in list
	         int oldCurSubLevel=-1;
             int CurSubLevel;
             //float minX,oldMinX;
             //float minY,oldMinY;
             //float minZ,oldMinZ;
             REFBOX ***CurrentSubBox;
	         CurSubLevel = 0;
             //save root boxes
			 
			 
			             //if(boxCollection[xPtr][yPtr][zPtr].SubTableLevel == (NumberOfLevels-1)){
			            int bottomLine = 1; 
						ar >> bottomLine;
						//int cast = 0;
			            if(bottomLine == 1){
						 ar >> boxCollection[xPtr][yPtr][zPtr].count;
			             ar	>> boxCollection[xPtr][yPtr][zPtr].SubTableLevel;
						 ar	>> boxCollection[xPtr][yPtr][zPtr].objectReferenceCount;
						 if(boxCollection[xPtr][yPtr][zPtr].count > 0)
					     boxCollection[xPtr][yPtr][zPtr].refrenceList = new int[boxCollection[xPtr][yPtr][zPtr].count]; 
						 else
					     boxCollection[xPtr][yPtr][zPtr].refrenceList = new int[2];
							
						 //boxCollection[xPtr][yPtr][zPtr].objectReferenceCount=0;
						 if(boxCollection[xPtr][yPtr][zPtr].objectReferenceCount > 0){
					     boxCollection[xPtr][yPtr][zPtr].objectReferenceList = new WORD[boxCollection[xPtr][yPtr][zPtr].objectReferenceCount]; 
						 }
						 else{
					     boxCollection[xPtr][yPtr][zPtr].objectReferenceList = new WORD[2];
						 }
						 
						 for(loop=0;loop<boxCollection[xPtr][yPtr][zPtr].count;loop++){
				               ar >> boxCollection[xPtr][yPtr][zPtr].refrenceList[loop];
						 }

                         
						 for(loop=0;loop<boxCollection[xPtr][yPtr][zPtr].objectReferenceCount;loop++)
						 {
				               ar >> boxCollection[xPtr][yPtr][zPtr].objectReferenceList[loop];
						 }
						boxCollection[xPtr][yPtr][zPtr].SubBoxCollection = NULL;  
						//boxCollection[xPtr][yPtr][zPtr].objectReferenceCount=0;
						//boxCollection[xPtr][yPtr][zPtr].objectReferenceList=0;
						}
						else{
					     boxCollection[xPtr][yPtr][zPtr].refrenceList = new int[2];
						 boxCollection[xPtr][yPtr][zPtr].objectReferenceList = new WORD[2];
						 boxCollection[xPtr][yPtr][zPtr].objectReferenceCount = 0;
						 boxCollection[xPtr][yPtr][zPtr].count=0;
			             boxCollection[xPtr][yPtr][zPtr].SubBoxCollection = new REFBOX**[SizeSquaredX[0]];
                         for (int m=0; m<SizeSquaredX[0]; m++)
						 {
                           boxCollection[xPtr][yPtr][zPtr].SubBoxCollection[m] = new REFBOX*[SizeSquaredY[0]];
                           for (int n=0; n<SizeSquaredY[0]; n++)
                              boxCollection[xPtr][yPtr][zPtr].SubBoxCollection[m][n] = new REFBOX[SizeSquaredZ[0]];
						 }
				         CurrentSubBox = boxCollection[xPtr][yPtr][zPtr].SubBoxCollection;
			             SubLevelLoad(CurrentSubBox,ar);
						}
			}//end loop through all boxes in list
	}//end has content
	
   m_wldObjectReferenceArray = NULL;
   }//end loading
	//AfxMessageBox("Completed");
	
}

BNDBOX CCollisionObj::GetPolyBoundingBox(D3DVECTOR poly[3])
{
 BNDBOX tweekedFinal;
 for(int loop=0;loop<3;loop++)
 {
  if(loop == 0)
  {//init them
   tweekedFinal.max.x = poly[0].x;
   tweekedFinal.max.y = poly[0].y;
   tweekedFinal.max.z = poly[0].z;
   tweekedFinal.min.x = poly[0].x;
   tweekedFinal.min.y = poly[0].y;
   tweekedFinal.min.z = poly[0].z;
  }//end init them
  else
  {//adjust
   if(poly[loop].x > tweekedFinal.max.x)
    tweekedFinal.max.x = poly[loop].x;
   if(poly[loop].y > tweekedFinal.max.y)
    tweekedFinal.max.y = poly[loop].y;
   if(poly[loop].z > tweekedFinal.max.z)
    tweekedFinal.max.z = poly[loop].z;

   if(poly[loop].x < tweekedFinal.min.x)
    tweekedFinal.min.x = poly[loop].x;
   if(poly[loop].y < tweekedFinal.min.y)
    tweekedFinal.min.y = poly[loop].y;
   if(poly[loop].z < tweekedFinal.min.z)
    tweekedFinal.min.z = poly[loop].z;
  }//end adjust
 }
 if(tweekedFinal.max.x == tweekedFinal.max.x)
    tweekedFinal.max.x = tweekedFinal.max.x + .01f;
 if(tweekedFinal.max.y == tweekedFinal.max.y)
    tweekedFinal.max.y = tweekedFinal.max.y + .01f;
 if(tweekedFinal.max.z == tweekedFinal.max.z)
    tweekedFinal.max.z = tweekedFinal.max.z + .01f;

 return tweekedFinal;
}
//--------------------------------------------------------//
//--------------------------------------------------------//
//--------------------------------------------------------//
 BNDBOX CCollisionObj::GetSegmentBoundingBox(D3DVECTOR & segmentStart,
											D3DVECTOR & segmentEnd)
{
 BNDBOX tweekedFinal;
  
  //init them
   tweekedFinal.max.x = segmentStart.x;
   tweekedFinal.max.y = segmentStart.y;
   tweekedFinal.max.z = segmentStart.z;
   tweekedFinal.min.x = segmentStart.x;
   tweekedFinal.min.y = segmentStart.y;
   tweekedFinal.min.z = segmentStart.z;
  //end init them
 
   if(segmentEnd.x > tweekedFinal.max.x)
    tweekedFinal.max.x = segmentEnd.x;
   if(segmentEnd.y > tweekedFinal.max.y)
    tweekedFinal.max.y = segmentEnd.y;
   if(segmentEnd.z > tweekedFinal.max.z)
    tweekedFinal.max.z = segmentEnd.z;

   if(segmentEnd.x < tweekedFinal.min.x)
    tweekedFinal.min.x = segmentEnd.x;
   if(segmentEnd.y < tweekedFinal.min.y)
    tweekedFinal.min.y = segmentEnd.y;
   if(segmentEnd.z < tweekedFinal.min.z)
    tweekedFinal.min.z = segmentEnd.z;

 if(tweekedFinal.max.x == tweekedFinal.max.x)
    tweekedFinal.max.x = tweekedFinal.max.x + .01f;
 if(tweekedFinal.max.y == tweekedFinal.max.y)
    tweekedFinal.max.y = tweekedFinal.max.y + .01f;
 if(tweekedFinal.max.z == tweekedFinal.max.z)
    tweekedFinal.max.z = tweekedFinal.max.z + .01f;

 return tweekedFinal;
}




CBoxObj::CBoxObj()
{
faceID = -1;

}


void CBoxObj::Serialize( CArchive& ar )
{
 int dump = 0;
 CObject::Serialize( ar );
 if(ar.IsStoring() )
 {
  ar  << faceID;
 }
 else
 {
  ar  >> faceID;
 }
}
///-------------------------------collision extented--------------------------------------//
//--
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--This Function is a component of ExactCollision()-----------//
int CCollisionObj::LeeRound(float x)
{
 float testVal;
 testVal = fmod(x,1.0f);
 if((x >= 0) && (testVal >= 0.5))
  x = x + 1.0f;
 else if((x<0)&&(testVal<=-0.5f))
  x=x-1.0f;
 return (int)x;
}

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--This Function is a component of ExactCollision()-----------//
float CCollisionObj::WherePt(D3DVECTOR polygon[3],D3DVECTOR ptloc)
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
void CCollisionObj::FindInterPt(D3DVECTOR polygon[3],
                                D3DVECTOR pt1,
                                D3DVECTOR pt2,
                                D3DVECTOR * interPtLoc
                               )
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
BOOL CCollisionObj::SegOverlap(D3DVECTOR pt1,
                               D3DVECTOR pt2,
                               D3DVECTOR pt3,
                               D3DVECTOR pt4
                              )
{
 float t1,t2;

 if((pt2.x - pt1.x)!= 0.0f)
 {
  t1 = (pt3.x - pt1.x)/(pt2.x - pt1.x);
  t2 = (pt4.x - pt1.x)/(pt2.x - pt1.x);
 }
 else if((pt2.y-pt1.y)!=0.0f)
 {
  t1 = (pt3.y - pt1.y)/(pt2.y - pt1.y);
  t2 = (pt4.y - pt1.y)/(pt2.y - pt1.y);
 }
 else if((pt2.z-pt1.z)!=0.0f)
 {
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
BOOL CCollisionObj::ExactColision(D3DVECTOR polygon1[3],
                                  D3DVECTOR polygon2[3],
                                  float *avgX,
                                  float *avgY,
                                  float *avgZ
                                 )
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
 if(avgX != NULL)
 {
  *avgX = ((interPtLoc1.x + interPtLoc2.x)*.5f);
 }
 if(avgY != NULL)
 {
  *avgY = ((interPtLoc1.y + interPtLoc2.y)*.5f);
 }
 if(avgZ != NULL)
 {
  *avgZ = ((interPtLoc1.z + interPtLoc2.z)*.5f);
 }

 return SegOverlap(interPtLoc1,interPtLoc2,interPtLoc3,interPtLoc4);

}

 BOOL CCollisionObj::BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max)
{
 if(point.x >= min.x &&
    point.x <= max.x &&
    point.y >= min.y &&
    point.y <= max.y &&
    point.z >= min.z &&
    point.z <= max.z)
  return TRUE;
 return FALSE;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//------segment check------------------------------------------//
 BOOL CCollisionObj::SegStraightOverlap(float & segPt1min,
				       float & segPt1max,
				       float & segPt2min,
				       float & segPt2max
				      )
{
 if(segPt1min >= segPt2min && segPt1min <= segPt2max)
  return TRUE;
 if(segPt1max >= segPt2min && segPt1max <= segPt2max)
  return TRUE;
 if(segPt1min <= segPt2min && segPt1max >= segPt2max)
  return TRUE;

 return FALSE;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//------Bounding box to box check------------------------------------------//
 BOOL CCollisionObj::BoundingBoxToBoxCheck(BNDBOX & box1,BNDBOX & box2)
{
 if(SegStraightOverlap(box1.min.x,box1.max.x,box2.min.x,box2.max.x)==FALSE)
  return FALSE;
 if(SegStraightOverlap(box1.min.y,box1.max.y,box2.min.y,box2.max.y)==FALSE)
  return FALSE;
 if(SegStraightOverlap(box1.min.z,box1.max.z,box2.min.z,box2.max.z)==FALSE)
  return FALSE;

 return TRUE;//is collided
}
//-------------------------------------------------------------//
//-----------calculate distance of transform based-------------//
//----------------on norm on ship and poly---------------------//
float CCollisionObj::SlideTranslation( 
				       D3DVECTOR Normal,
				       D3DVECTOR NormalOrigin,
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
//-This Function will determine the angle between two vectors--//
float CCollisionObj::ContactAngle(D3DVECTOR OriginOfShipNormal,
                                  D3DVECTOR TerminusOfShipNormal,
                                  D3DVECTOR OriginOfWallNormal,
                                  D3DVECTOR TerminusOfWallNormal
                                 )
{
 double AngleBetweenVectors;

 AngleBetweenVectors = acos( ( (TerminusOfShipNormal.x - OriginOfShipNormal.x)
		     *(TerminusOfWallNormal.x - OriginOfWallNormal.x))
		     +( (TerminusOfShipNormal.y - OriginOfShipNormal.y)
		     *(TerminusOfWallNormal.y - OriginOfWallNormal.y))
		     +( (TerminusOfShipNormal.z - OriginOfShipNormal.z)
		     *(TerminusOfWallNormal.z - OriginOfWallNormal.z)));

 float degrees = (float)((AngleBetweenVectors * 180)/3.14159265359);

 return degrees;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Poly Collision R2-----------------------------------------------------//
BOOL CCollisionObj::SegPolyCheck(D3DVECTOR polygon[3],
							     D3DVECTOR & startPoint,
							     D3DVECTOR & endPoint,
							     D3DVECTOR *intersectionPoint)
{
	
	
	if(startPoint.x > polygon[0].x &&
	   startPoint.x > polygon[1].x &&
	   startPoint.x > polygon[2].x &&
	   endPoint.x > polygon[0].x &&
	   endPoint.x > polygon[1].x &&
	   endPoint.x > polygon[2].x)
	{//filter
       return FALSE;
	}

	if(startPoint.x < polygon[0].x &&
	   startPoint.x < polygon[1].x &&
	   startPoint.x < polygon[2].x &&
	   endPoint.x < polygon[0].x &&
	   endPoint.x < polygon[1].x &&
	   endPoint.x < polygon[2].x)
	{//filter
       return FALSE;
	}

	if(startPoint.y > polygon[0].y &&
	   startPoint.y > polygon[1].y &&
	   startPoint.y > polygon[2].y &&
	   endPoint.y > polygon[0].y &&
	   endPoint.y > polygon[1].y &&
	   endPoint.y > polygon[2].y)
	{//filter
       return FALSE;
	}

	if(startPoint.y < polygon[0].y &&
	   startPoint.y < polygon[1].y &&
	   startPoint.y < polygon[2].y &&
	   endPoint.y < polygon[0].y &&
	   endPoint.y < polygon[1].y &&
	   endPoint.y < polygon[2].y)
	{//filter
       return FALSE;
	}

    if(startPoint.z > polygon[0].z &&
	   startPoint.z > polygon[1].z &&
	   startPoint.z > polygon[2].z &&
	   endPoint.z > polygon[0].z &&
	   endPoint.z > polygon[1].z &&
	   endPoint.z > polygon[2].z)
	{//filter
       return FALSE;
	}

	if(startPoint.z < polygon[0].z &&
	   startPoint.z < polygon[1].z &&
	   startPoint.z < polygon[2].z &&
	   endPoint.z < polygon[0].z &&
	   endPoint.z < polygon[1].z &&
	   endPoint.z < polygon[2].z)
	{//filter
       return FALSE;
	}
	
	m_polyspace.m[0][3] = 0.0f;
    m_polyspace.m[1][3] = 0.0f;
    m_polyspace.m[2][3] = 0.0f;
    m_polyspace.m[3][3] = 1.0f;

    //D3DVECTOR directionalVect,directionalVect2;
	m_directionalVect.x = polygon[1].x - polygon[0].x;
	m_directionalVect.y = polygon[1].y - polygon[0].y;
	m_directionalVect.z = polygon[1].z - polygon[0].z;

	m_directionalVect2.x = polygon[2].x - polygon[0].x;
	m_directionalVect2.y = polygon[2].y - polygon[0].y;
	m_directionalVect2.z = polygon[2].z - polygon[0].z;

    m_polySpaceUpVector = GL_matrixFunctions.Normalize (GL_matrixFunctions.CrossProduct(m_directionalVect,m_directionalVect2));
	m_directionalVector = GL_matrixFunctions.Normalize (m_directionalVect);
	//m_polySpaceUpVector = GL_matrixFunctions.CrossProduct(m_directionalVect,m_directionalVect2);
	//m_directionalVector = m_directionalVect;

    GL_matrixFunctions.SetOrientation(&m_polyspace,m_directionalVector,m_polySpaceUpVector);

  
	m_polyspace._41 = polygon[0].x;
	m_polyspace._42 = polygon[0].y;
	m_polyspace._43 = polygon[0].z;
    GL_matrixFunctions.D3DMath_MatrixInvert2( m_tempMatGLB, m_polyspace);

    m_localStartPoint = GL_matrixFunctions.GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,startPoint);
    m_localEndPoint   = GL_matrixFunctions.GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,endPoint);

    if(m_localStartPoint.y > 0.0f && m_localEndPoint.y > 0.0f)
	   return FALSE;
    
	if(m_localStartPoint.y <= 0.0f && m_localEndPoint.y <= 0.0f)
	   return FALSE;


	m_localPolygon[0] = GL_matrixFunctions.GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,polygon[0]);
    m_localPolygon[1] = GL_matrixFunctions.GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,polygon[1]);
	m_localPolygon[2] = GL_matrixFunctions.GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,polygon[2]);

    m_segDistance = fabs(m_localStartPoint.y) + fabs(m_localEndPoint.y);//seg distance

	m_tempXYZ                         = GL_matrixFunctions.InterpolatePointsHiRes(m_localStartPoint, 
						                                                          m_localEndPoint,
					                                                              m_segDistance, 
					                                                              fabs(m_localStartPoint.y));

    if(GL_matrixFunctions.PointInPolyXZOpt(m_tempXYZ,m_localPolygon))
	{
	   *intersectionPoint = GL_matrixFunctions.UpdatePolygonVertex(m_polyspace.m,m_tempXYZ);
       return TRUE;
	}


	return FALSE;
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
CCollisionObj * CCollisionObjList::GetByIndex(int index)
{
   if(index < 0)
	  return NULL;
	
      POSITION posLoc = FindIndex(index);
      if(!posLoc)
	      return NULL;

      CCollisionObj * cPtr = (CCollisionObj *)GetAt(posLoc);
	  return cPtr;
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CCollisionObjList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CCollisionObj * delPtr = (CCollisionObj *)GetNext(posLoc);
	   delPtr->SafeDelete();
       delete delPtr;
	   delPtr=0;
   }
   RemoveAll();
}


IMPLEMENT_SERIAL(CBoxObj, CObject, 0)
IMPLEMENT_SERIAL(CBoxObjList, CObList, 0)
IMPLEMENT_SERIAL(CCollisionObj, CObject, 0)
IMPLEMENT_SERIAL(CCollisionObjList, CObList, 0)


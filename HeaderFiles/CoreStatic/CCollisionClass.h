

#ifndef CCOLLISION_H
#define CCOLLISION_H

#include "CWldObject.h"
#include <dinput.h>
#include "CStructuresMisl.h"
#include "CTextureDBClass.h"
#include "MatrixArb.h"
#include "CReferenceLibrary.h"
#include "..\..\AuraX\resource.h"

struct WorldObjectReference
{
    CWldObject * worldObjectReference;
};


struct REFBOX
{
	WORD objectReferenceCount;
	WORD * objectReferenceList;
    int *  refrenceList;
    int    count;
    int    SubTableLevel;
    REFBOX ***SubBoxCollection;
};


class CBoxObj : public CObject
{
	
     
	DECLARE_SERIAL( CBoxObj );
	
    
 public:
	CBoxObj();
	int faceID;
	void Serialize( CArchive& ar );

};

class CBoxObjList : public CObList
{
public:
    CBoxObjList(){};
	
     
    DECLARE_SERIAL( CBoxObjList )
    
    
};

class CCollisionObj : public CObject
{
	
     
    DECLARE_SERIAL( CCollisionObj );
	
    
public:
	MatrixARB abMath;
	//CDialog * m_progressInternal;
	//CDialog * m_subLevelProgressInternal;
	CWldObjectList * GL_worldList;
	int  GL_barTrace;
	BOOL GL_barSwitch;
	int GL_lastBarUpdate;
    int *SizeSquaredX;
    int *SizeSquaredY;
    int *SizeSquaredZ;
    int MaxFacesInBox;
	float BoxZoneAdjustManual;
    int NumberOfLevels;
    MatrixARB GL_matrixFunctions;
    CCollisionObj();
    REFBOX ***boxCollection;
    CDialogBar *m_updateWnd;
	IDirectInputDevice8 * directKeyboard;
    float *boxSizeX;
    float *boxSizeY;
    float *boxSizeZ;
    int refBoxCollectionCount;
    CFaceStruct * FaceDatabank;
	WorldObjectReference * m_wldObjectReferenceArray;
    int FaceDataBankCount;
    BNDBOX BaseBox;
    int xGlobalOffset,yGlobalOffset,zGlobalOffset;
	float m_smallestTableRes;
	//global temps to class object-Two threads cannot use this object simutanius
	D3DMATRIX m_tempMatGLB;
	D3DMATRIX m_polyspace;
	D3DVECTOR m_directionalVect;
	D3DVECTOR m_directionalVect2;
	D3DVECTOR m_polySpaceUpVector;
    D3DVECTOR m_directionalVector;
    float     m_segDistance;
	D3DVECTOR m_localPolygon[3];
	D3DVECTOR m_tempXYZ;
	D3DVECTOR m_localStartPoint,m_localEndPoint;

	BOOL SegPolyCheck(D3DVECTOR polygon[3],
							 D3DVECTOR & startPoint,
							 D3DVECTOR & endPoint,
							 D3DVECTOR *intersectionPoint);
    BOOL CalculateWorld(CWldObjectList * worldList,
			int squaredX,
			int squaredY,
			int squaredZ,
			CReferenceObjList * reference,
			CDialog * m_subLevelProgressInternal);
    BOOL GenerateTable(CDialog * m_progressInternal);
    BOOL GenerateSubTable(REFBOX *SubWorld, BNDBOX *SubWorldSize,CDialog * m_subLevelProgressInternal);
    int LeeRound(float x);
    void Serialize( CArchive& ar );

    float WherePt(D3DVECTOR polygon[3],D3DVECTOR ptloc);
    void FindInterPt(D3DVECTOR polygon[3],D3DVECTOR pt1,D3DVECTOR pt2,D3DVECTOR * interPtLoc);
    BOOL SegOverlap(D3DVECTOR pt1,D3DVECTOR pt2,D3DVECTOR pt3,D3DVECTOR pt4);
    BOOL ExactColision(D3DVECTOR polygon1[3],D3DVECTOR polygon2[3],float *avgX,float *avgY,float *avgZ);
     BOOL BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max);
     BOOL SegStraightOverlap(float & segPt1min,
		            float & segPt1max,
			        float & segPt2min,
			        float & segPt2max
			        );
    BOOL PolyCollisionTest(D3DVECTOR polyLoc[3],
			   float * intersectX,
			   float * intersectY,
			   float * intersectZ,
			   D3DVECTOR * normal,
			   int *actualCollisions,
			   int *consideredCollision,
			   int *indexOfObjectHit
			  );
    BOOL ObjectCollisionAdjust(D3DVECTOR polyLoc[3],
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
			      );
    
    BOOL CollisionPolyRetrieval(D3DVECTOR polyLoc[3],
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
					 );
	BOOL ValidPath(D3DVECTOR polyLoc[3],
				   int *actualCollisions,
				   int *consideredCollision,
				   float floorTolerance,
				   float wallMinimumHieght,
				   float currentYpos,
				   D3DVECTOR origin,
				   D3DVECTOR virtualNormal,
				   BOOL *wallInWayRet,
				   BOOL *floorExistRet
				   );


	 BOOL ObjectCollisionAdjustSegment(
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
					  float normalYHieghtFilter,
					  float normalYHieghtFilterDW,
					  int * indexOfObjectHit
					 );

    int ConvertPositionToIndex(float & position,float & dimension,float & startPoint,int & endPoint);
    BNDBOX GetPolyBoundingBox(D3DVECTOR poly[3]);
     BOOL BoundingBoxToBoxCheck(BNDBOX & box1,BNDBOX & box2);
    float SlideTranslation( D3DVECTOR Normal,D3DVECTOR NormalOrigin,D3DVECTOR Polygon[3]);
    float ContactAngle(D3DVECTOR OriginOfShipNormal, D3DVECTOR TerminusOfShipNormal,D3DVECTOR OriginOfWallNormal, D3DVECTOR TerminusOfWallNormal);
    void SafeDelete();
	void BarColorOcsilate();
	void SubLevelSave(REFBOX ***CurrentSubBox,CArchive& ar);
	void SubLevelLoad(REFBOX ***CurrentSubBox,CArchive& ar);
	void SubLevelDelete(REFBOX ***CurrentSubBox);

    BOOL CollisionSegmentSI(
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
					 );
	BOOL CollisionSegmentSItranslated(
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
					 );
     BOOL CollisionSegmentSIValidPath(
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
					               );
	BOOL CollisionSegmentSIBasic(
					  D3DVECTOR & collisionPathStart,
					  D3DVECTOR & collisionPathEnd,
					  float tolerance
					);

    BOOL CollisionSegmentSIFilterCharona(
					  D3DVECTOR collisionPathStart,
					  D3DVECTOR collisionPathEnd,
					  float tolerance
					  );

	BOOL CollisionSegmentSIBasicNormal(
					  D3DVECTOR &collisionPathStart,
					  D3DVECTOR &collisionPathEnd,
					  float tolerance
					);

	BOOL GetClosesIntersectionUseSeg(
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
					  byte & type);
	BOOL Zcheck(D3DMATRIX matrixLoc,D3DVECTOR point,float fov);
	BNDBOX GetSegmentBoundingBox(D3DVECTOR & segmentStart,D3DVECTOR & segmentEnd);
    void BuildObjectReferenceArray(CWldObjectList * wList);
};

class CCollisionObjList : public CObList
{
public:
    CCollisionObjList(){};
	
     
    DECLARE_SERIAL( CCollisionObjList )
    
    

	CCollisionObj * GetByIndex(int index);
	void SafeDelete();
};

#endif
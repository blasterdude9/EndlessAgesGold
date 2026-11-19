#include <d3dx9.h>
#include <d3d9.h>

#pragma once
//#ifndef CARBMATRIX_H
//#define CARBMATRIX_H

//#ifndef MATRIXSTRUCTURESAX
//#define MATRIXSTRUCTURESAX
class AXDXMATRIX {
    
	public:
	union {
        struct {
            double        _11, _12, _13, _14;
            double        _21, _22, _23, _24;
            double        _31, _32, _33, _34;
            double        _41, _42, _43, _44;

        };
        double m[4][4];
    };

    
		AXDXMATRIX(){}
		~AXDXMATRIX(){}
	
		AXDXMATRIX & operator=( D3DMATRIX temp )
		{
			_11 = (double)temp._11;
			_12 = (double)temp._12;
			_13 = (double)temp._13;
			_14 = (double)temp._14;

			_21 = (double)temp._21;
			_22 = (double)temp._22;
			_23 = (double)temp._23;
			_24 = (double)temp._24;

			_31 = (double)temp._31;
			_32 = (double)temp._32;
			_33 = (double)temp._33;
			_34 = (double)temp._34;

			_41 = (double)temp._41;
			_42 = (double)temp._42;
			_43 = (double)temp._43;
			_44 = (double)temp._44;

			return *this;
		}

		void SetD3dMatrix(D3DMATRIX & tempMat)
		{
            tempMat._11 = (float)_11;
			tempMat._12 = (float)_12;
			tempMat._13 = (float)_13;
			tempMat._14 = (float)_14;

			tempMat._21 = (float)_21;
			tempMat._22 = (float)_22;
			tempMat._23 = (float)_23;
			tempMat._24 = (float)_24;

			tempMat._31 = (float)_31;
			tempMat._32 = (float)_32;
			tempMat._33 = (float)_33;
			tempMat._34 = (float)_34;

			tempMat._41 = (float)_41;
			tempMat._42 = (float)_42;
			tempMat._43 = (float)_43;
			tempMat._44 = (float)_44;

		}

		void RotateYri ( double Ya )
		{
			//Generate 3D rotation matrix:
			double TempEqY = Ya;
			  
			double cy = cos(TempEqY);
			double sy = sin(TempEqY);

			//Initialize Y rotation matrix:
			_11=cy;   _12=0.0;   _13=-sy; _14=0.0;
			_21=0.0;  _22=1.0;   _23=0.0; _24=0.0;
			_31=sy;   _32=0.0;   _33=cy;  _34=0.0;
			_41=0.0;  _42=0.0;   _43=0.0; _44=1.0;
		}

		void AddTransformAfter(double & xLoc, double & yLoc, double & zLoc)
		{
			_41 = xLoc + _41;
			_42 = yLoc + _42;
			_43 = zLoc + _43;
		}
		
};


class AXDVECTOR {
    
public:

	AXDVECTOR(){}
	~AXDVECTOR(){}
	
	double x;
    double y;
    double z;

	AXDVECTOR & operator=( D3DVECTOR temp )
	{
        x = temp.x;
		y = temp.y;
		z = temp.z;
		return *this;
	}

	void ZeroIt()
	{
        ZeroMemory(this,sizeof(AXDVECTOR));
	}


    double MagnitudeSquared(double xPos, double yPos, double zPos)
	{
      double temp_Mxyz = (((x - xPos) * (x - xPos)) + ((y - yPos) * (y - yPos)) + ((z - zPos) * (z - zPos)));
      return sqrt(temp_Mxyz);
    }

	double MagnitudeSquared(double & xPos, double & yPos, double & zPos)
	{
      double temp_Mxyz = (((x - xPos) * (x - xPos)) + ((y - yPos) * (y - yPos)) + ((z - zPos) * (z - zPos)));
      return sqrt(temp_Mxyz);
    }
};

//#endif


class MatrixARB 
{


public:

/*D3DMATRIX m_tempMatGLB;
D3DVECTOR m_tempXYZ;
float m_t1;
float m_t2;
float m_t3;

//public collision pre alloc
D3DMATRIX m_polyspace;
D3DVECTOR m_localPolygon[3];
D3DVECTOR m_directionalVect;
D3DVECTOR m_directionalVect2;
D3DVECTOR m_polySpaceUpVector;
D3DVECTOR m_directionalVector;
float     m_segDistance;*/


MatrixARB();
float PI;
void MatrixToPLaneZ(D3DMATRIX & matrix,float plane[4]);
D3DMATRIX GetDifferenceMatrix(D3DMATRIX & startMatrix,D3DMATRIX & afterMatrix);
void AngleAxisTrnsfrm(D3DMATRIX& q, D3DVECTOR& Axis, float Angle);
void InitMat ( float Mat [ 4 ] [ 4 ] );
void Rotate ( float Xa, float Ya, float Za, float TMatrix [ 4 ] [ 4 ] );
void  RotateX ( float Xa,float TMatrix [ 4 ] [ 4 ]);
void  RotateZ ( float & Za,float TMatrix [ 4 ] [ 4 ]);
D3DVECTOR RotateY_WLD ( float & Ya, D3DVECTOR & curPos);
void Translate ( float Xt, float Yt, float Zt,float TMatrix [ 4 ] [ 4 ] );
void scale ( float Xt, float Yt, float Zt,float TMatrix [ 4 ] [ 4 ] );
void _fastcall TrnsfrMtrxInvrs(float Matrix[4][4], float InvMatrix[4][4]);
D3DVECTOR UpdatePolygonVertex(float matrixLoc[4][4],D3DVECTOR & polygonLoc);
D3DVECTOR UpdatePolygonVertexRotationOnly(float matrixLoc[4][4],D3DVECTOR polygonLoc);
D3DVECTOR GetLocalPositionFromMatrixView(D3DMATRIX & matrixLoc,D3DVECTOR & objVect);
void _fastcall TrnsfrMtrxMult(float MultOut[4][4], float A[4][4], float B[4][4]);
void _fastcall MergeMatrices1( float Dest[4][4], float Source[4][4]);
void MergeMatrices ( float Dest [ 4 ] [ 4 ], float Source [ 4 ] [ 4 ] );
unsigned long int sqrt_5(unsigned long int x);
float CrossProductFloat (float v1, float v2);
D3DVECTOR CrossProduct (const _D3DVECTOR& v1, const _D3DVECTOR& v2);
float DotProduct (const _D3DVECTOR& v1, const _D3DVECTOR& v2);
void FaceTowardPoint(D3DVECTOR & targetVect,D3DMATRIX * inputMatrix);
void GetOrientation(D3DMATRIX & input,D3DVECTOR *dirVect,D3DVECTOR *upVect);
void GetDirVect(D3DMATRIX & input,D3DVECTOR *dirVect);
void SetOrientation(D3DMATRIX * matrixInOut,D3DVECTOR & dirVect,D3DVECTOR & upVect);
int RoundNext(float x);
int Round(float x);
void D3DMath_MatrixInvert( D3DMATRIX& q, D3DMATRIX& a );
D3DVECTOR GetPointFromSpace(D3DMATRIX invFocusMatrix,D3DVECTOR point);
D3DVECTOR InterpolatePoints(D3DVECTOR & StartVec,
								   D3DVECTOR & EndVec,
					               int NumOfStages, 
					               int ThisStage);
D3DVECTOR InterpolatePointsExp(D3DVECTOR StartVec, 
						       D3DVECTOR EndVec,
					           int NumOfStages, 
					           int ThisStage,
							   float exp1,
							   float exp2);
D3DVECTOR Normalize (D3DVECTOR & v);
float MagnitudeSquaredFast3(float Mx1,float My1,float Mz1,float Mx2,float My2,float Mz2);
float MagnitudeSquared(float &Mx1,float &My1,float &Mz1,
							  float &Mx2,float &My2,float &Mz2);
float MagnitudeSquared2(float Mx1,float My1,float Mz1,float Mx2,float My2,float Mz2);
 void fastMatrixMultiply(D3DMATRIX & R,D3DMATRIX A, D3DMATRIX B);
BOOL CollisionSphereSeg(D3DVECTOR & startPoint,
					    D3DVECTOR & endPoint,
					    float & sphereRadius,
					    D3DVECTOR & center,
					    BOOL  getAdjustInfo,
					    D3DVECTOR *adjust,
					    float & objectRadius,
						D3DVECTOR * intersection);
float InterpolateFloats(
						 float StartVec, 
						 float EndVec,
					     int NumOfStages, 
					     int ThisStage);
BOOL BoundingSphereCheck(float radius1,float radius2,float x1,float y1,float z1,float x2,float y2,float z2);
float GetLocalZpos (D3DMATRIX  invViewMatrix,
				    D3DMATRIX viewMatrix,
				    float posX,
				    float posY,
				    float posZ);
D3DMATRIX AxisAngleRotationV2( D3DVECTOR axis, float angle );
D3DVECTOR InterpolatePointsHiRes(D3DVECTOR StartVec, 
						                    D3DVECTOR EndVec,
					                        float NumOfStages, 
					                        float ThisStage);
BOOL SegPolyCheck(D3DVECTOR polygon[3],
							 D3DVECTOR &startPoint,
							 D3DVECTOR &endPoint,
							 D3DVECTOR *intersectionPoint);
void D3DMath_MatrixInvert2( D3DMATRIX& q, D3DMATRIX& a );
float InterpolateFloatsHiRes(
						            float StartVec, 
						            float EndVec,
					                float NumOfStages, 
					                float ThisStage);
BOOL PointInPolyXZ(D3DVECTOR & point,D3DVECTOR polyGon[3]);
BOOL TestLevel1(float & val,float & h1,float & h2);
BOOL PointInPolyXZOpt(D3DVECTOR & point,D3DVECTOR polyGon[3]);
D3DVECTOR GetLocalPositionFromMatrixView2(D3DMATRIX & invMatrix,D3DMATRIX & matrixLoc,D3DVECTOR & objVect);
BOOL GetSubDistance(D3DVECTOR polyGon[3],
							   D3DVECTOR uniNormal,
							   D3DVECTOR point,
							   float * distanceToMoveAlongNormal);

D3DVECTOR GetAdjustVector(float & distance,D3DVECTOR & uniDirection);
D3DVECTOR GetAdjustVector(float & distance,D3DVECTOR & uniDirection,float & factor);

D3DMATRIX LookAtPoint( D3DVECTOR loc, 
							      D3DVECTOR lookAt, 
							      D3DVECTOR inUp );
 D3DMATRIX             GetShadowMeshTransform(D3DMATRIX currentMatrix,
											 D3DVECTOR & orientationNormal,
											 float offsetFromFloor);
D3DMATRIX GetShadowMeshTransformSpcified(D3DMATRIX currentMatrix,
												D3DMATRIX & differenceMatrix);
BOOL ScaleSystem(float percentage);
float Magnitude (const _D3DVECTOR& v);
 float SquareMagnitude (const _D3DVECTOR& v);
 D3DVECTOR Rotate_WLD ( float & Ya,D3DVECTOR & offset,D3DVECTOR & curPos);
 float GetDifferenceAngleY(D3DVECTOR vector1,D3DVECTOR vector2);
 D3DMATRIX GetAccuracyAdjust(float maxGroupingAngle);
 float GetAngleBetweenTwoVectors(D3DVECTOR vOne,D3DVECTOR vTwo);
 HRESULT SetProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,FLOAT fNearPlane, FLOAT fFarPlane,float hieghtBase );
 BOOL SetViewMatrixAB( D3DMATRIX& mat, D3DXVECTOR3& vFrom, D3DXVECTOR3& vAt, D3DXVECTOR3& vWorldUp );
 BOOL BoundingSphereCheckX(float & radius1,float & radius2,float & x1,float & y1,float & z1,float & x2,float & y2,float & z2);
 BOOL SegPolyCheckInLine(   D3DVECTOR & polygonPt1,
							D3DVECTOR & polygonPt2,
							D3DVECTOR & polygonPt3,
							D3DVECTOR & startPoint,
							D3DVECTOR & endPoint,
							D3DVECTOR *intersectionPoint);
 float GetAngleBetweenTwoVectorsFast(D3DVECTOR & vOne,D3DVECTOR & vTwo);
 D3DXMATRIX GetDifferenceMatrix2(D3DXMATRIX startMatrix,D3DXMATRIX afterMatrix);
 D3DVECTOR GetLocalPositionFromMatrixViewRotOnly(D3DMATRIX & matrixLoc,D3DVECTOR & objVect);
 void  RotateY ( float Ya,float TMatrix [ 4 ] [ 4 ]);
 void  RotateYri ( float Ya,float TMatrix [ 4 ] [ 4 ]);
 float GetAngleBetweenTwoVectorsRadian(D3DVECTOR vOne,D3DVECTOR vTwo);
 void D3DMath_MatrixInvertD( AXDXMATRIX& q, AXDXMATRIX& a );
 AXDVECTOR GetLocalPositionFromMatrixViewRotOnlyD(AXDXMATRIX & matrixLoc,AXDVECTOR & objVect);
 AXDVECTOR UpdatePolygonVertexRotationOnlyD(AXDXMATRIX matrixLoc,AXDVECTOR polygonLoc);
 void fastMatrixMultiplyD(AXDXMATRIX & R,AXDXMATRIX A, AXDXMATRIX B);
 BOOL BoundingBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max);
 BOOL BoundingBoxPointCheck(float & pointX,float & pointY,float & pointZ,float & maxX,float & maxY,float & maxZ,float & minX,float & minY,float & minZ);
 float MagnitudeSquared(D3DMATRIX & matrix,D3DMATRIX & matrix2);
 float MagnitudeSquared(D3DMATRIX & matrix,D3DVECTOR & pos);
 float MagnitudeSquared(D3DVECTOR & po,D3DVECTOR & pos);
 D3DVECTOR InterpolatePointsX(D3DVECTOR & StartVec, 
						      D3DVECTOR & EndVec,
					          double NumOfStages, 
					          double ThisStage);
 void  RotateXri ( float Xr,float TMatrix [ 4 ] [ 4 ]);
 BOOL Generate2Dto3DVector(  float & screenX, 
							 float & screenY, 
							 float & screenZ,
							 float Zdist, 
							 D3DVECTOR & startPos, 
							 D3DVECTOR & endPos, 
							 D3DMATRIX & cameraMatrix,
							 float & fov,
							 float projectionCursorFov,
							 float & multiplierRamp);
 BOOL BoundingBoxPointCheck(D3DVECTOR & point,
									  float & maxX,
									  float & maxY,
									  float & maxZ,
									  float & minX,
									  float & minY,
									  float & minZ);
 BOOL BoundingBoxToBoxCheck(float & b1maxX,
									  float & b1maxY,
									  float & b1maxZ,
									  float & b1minX,
									  float & b1minY,
									  float & b1minZ,
									  float & b2maxX,
									  float & b2maxY,
									  float & b2maxZ,
									  float & b2minX,
									  float & b2minY,
									  float & b2minZ);
 BOOL SegStraightOverlap(float & segPt1min,float & segPt1max,float & segPt2min,float & segPt2max);
 BOOL GenerateReflectiveCameraMatrix(	D3DVECTOR & mirrorCenter, 
										D3DVECTOR & mirrorNormal, 
										D3DMATRIX & cameraMatrix, 
										D3DMATRIX * reflectiveMatrix,
										D3DMATRIX & invReflectiveMatrix,
										D3DVECTOR & widthPoint,
										D3DVECTOR & widthPointN,
										D3DVECTOR & heightPoint,
										D3DVECTOR & heightPointN,
										D3DMATRIX & perspectiveMatRet,
										D3DMATRIX & objectMatrix,
										D3DMATRIX * fovMatrix,
										float & distClipPlane);
 float GetAngleBetweenTwoVectorsRadian(D3DMATRIX & pivot,D3DVECTOR vOne,D3DVECTOR vTwo);
 BOOL GetViewMatrixAB( D3DMATRIX& mat, D3DMATRIX& cameraMat );
 BOOL SetViewMatrixAB( D3DMATRIX& mat, D3DVECTOR& vAt, D3DVECTOR& vWorldUp );
 void SegmentToBox(D3DVECTOR & start, D3DVECTOR & end, D3DVECTOR & max, D3DVECTOR & min);
};

//#endif

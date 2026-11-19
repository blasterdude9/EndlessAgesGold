#include "stdafx.h"
#define D3D_OVERLOADS

#include <afxtempl.h>
#include "math.h"

#include <d3d9.h>

#include "matrixarb.h"

#define N_BITS 32
#define MAX_BIT ((N_BITS + 1) / 2 - 1)
#define M_PI 3.14159

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MatrixARB::MatrixARB()
{
	PI = 3.141592654f;

}

//--This Function is a component of ExactCollision()-----------//
int MatrixARB::Round(float x)
{
	float testVal;
	testVal = fmod(x,1.0f);
	if((x >= 0) && (testVal >= 0.5))
		x = x + 1.0f;
	else if((x<0)&&(testVal<=-0.5f))
		x=x-1.0f;
	return (int)x;
}

//--This Function is a component of ExactCollision()-----------//
int MatrixARB::RoundNext(float x)
{
	if(x > 0)
		x += 1.0f;


	else if(x < 0)

		x -= 1.0f;

	return (int)x;
}



void MatrixARB::InitMat ( float Mat [ 4 ] [ 4 ] )
{
   //Initializes a specific matrix to the identity matrix:
   Mat [0][0] = 1.0f;  Mat [0][1] = 0.0f;  Mat [0][2] = 0.0f;  Mat [0][3] = 0.0f;
   Mat [1][0] = 0.0f;  Mat [1][1] = 1.0f;  Mat [1][2] = 0.0f;  Mat [1][3] = 0.0f;
   Mat [2][0] = 0.0f;  Mat [2][1] = 0.0f;  Mat [2][2] = 1.0f;  Mat [2][3] = 0.0f;
   Mat [3][0] = 0.0f;  Mat [3][1] = 0.0f;  Mat [3][2] = 0.0f;  Mat [3][3] = 1.0f;
}
   

void  MatrixARB::Rotate ( float Xa, float Ya, float Za, float TMatrix [ 4 ] [ 4 ])
{
	//Generate 3D rotation matrix:
   
	float Rmat [ 4 ] [ 4 ];
	InitMat ( TMatrix );
	float TempEqX = 0.0f;
	float TempEqY = 0.0f;
	float TempEqZ = 0.0f;
   
	if(Xa != 0.0f)
		TempEqX = (float)PI * ( Xa / 180.0f);
	if(Ya != 0.0f)
		TempEqY = (float)PI * ( Ya / 180.0f);
	if(Za != 0.0f)
		TempEqZ = (float)PI * ( Za / 180.0f);
  
	float cx = (float)cos(TempEqX);
	float sx = (float)sin(TempEqX);
  
	float cy = (float)cos(TempEqY);
	float sy = (float)sin(TempEqY);
  
	float cz = (float)cos(TempEqZ);
	float sz = (float)sin(TempEqZ);

	//Initialize Z rotation matrix - Note: we perform Z
	//rotation first to align the 3D Z axis with the 2D Z axis.
	Rmat[0][0]=cz;  Rmat[0][1]=sz;  Rmat[0][2]=0;    Rmat[0][3]=0;
	Rmat[1][0]=-sz; Rmat[1][1]=cz;  Rmat[1][2]=0;    Rmat[1][3]=0;
	Rmat[2][0]=0;   Rmat[2][1]=0;   Rmat[2][2]=1;    Rmat[2][3]=0;
	Rmat[3][0]=0;   Rmat[3][1]=0;   Rmat[3][2]=0;    Rmat[3][3]=1;

	//Merge matrix with master matrix:
	MergeMatrices (Rmat, TMatrix );

	//Initialize X rotation matrix:
	Rmat[0][0]=1;  Rmat[0][1]=0;        Rmat[0][2]=0;       Rmat[0][3]=0;
	Rmat[1][0]=0;  Rmat[1][1]=cx;       Rmat[1][2]=sx;      Rmat[1][3]=0;
	Rmat[2][0]=0;  Rmat[2][1]=-sx;      Rmat[2][2]=cx;      Rmat[2][3]=0;
	Rmat[3][0]=0;  Rmat[3][1]=0;        Rmat[3][2]=0;       Rmat[3][3]=1;

	//Merge matrix with master matrix:
	MergeMatrices ( Rmat, TMatrix );

	//Initialize Y rotation matrix:
	Rmat[0][0]=cy; Rmat[0][1]=0;   Rmat[0][2]=-sy; Rmat[0][3]=0;
	Rmat[1][0]=0;  Rmat[1][1]=1;   Rmat[1][2]=0;   Rmat[1][3]=0;
	Rmat[2][0]=sy; Rmat[2][1]=0;   Rmat[2][2]=cy;  Rmat[2][3]=0;
	Rmat[3][0]=0;  Rmat[3][1]=0;   Rmat[3][2]=0;   Rmat[3][3]=1;

	//Merge matrix with master matrix:
	MergeMatrices ( Rmat, TMatrix );
}

void  MatrixARB::RotateX ( float Xa,float TMatrix [ 4 ] [ 4 ])
{
	//Generate 3D rotation matrix:
	float TempEqX = 0.0f;
   
	if(Xa != 0.0f)
		TempEqX = (float)PI * ( Xa / 180.0f);
  
	float cx = (float)cos(TempEqX);
	float sx = (float)sin(TempEqX);

	//Initialize X rotation matrix:
	TMatrix[0][0]=1;  TMatrix[0][1]=0;        TMatrix[0][2]=0;       TMatrix[0][3]=0;
	TMatrix[1][0]=0;  TMatrix[1][1]=cx;       TMatrix[1][2]=sx;      TMatrix[1][3]=0;
	TMatrix[2][0]=0;  TMatrix[2][1]=-sx;      TMatrix[2][2]=cx;      TMatrix[2][3]=0;
	TMatrix[3][0]=0;  TMatrix[3][1]=0;        TMatrix[3][2]=0;       TMatrix[3][3]=1;
}

void  MatrixARB::RotateY ( float Ya,float TMatrix [ 4 ] [ 4 ])
{
	//Generate 3D rotation matrix:
	float TempEqY = 0.0f;

	if(Ya != 0.0f)
	TempEqY = (float)PI * ( Ya / 180.0f);

	float cy = (float)cos(TempEqY);
	float sy = (float)sin(TempEqY);

	//Initialize Y rotation matrix:
	TMatrix[0][0]=cy; TMatrix[0][1]=0;   TMatrix[0][2]=-sy; TMatrix[0][3]=0;
	TMatrix[1][0]=0;  TMatrix[1][1]=1;   TMatrix[1][2]=0;   TMatrix[1][3]=0;
	TMatrix[2][0]=sy; TMatrix[2][1]=0;   TMatrix[2][2]=cy;  TMatrix[2][3]=0;
	TMatrix[3][0]=0;  TMatrix[3][1]=0;   TMatrix[3][2]=0;   TMatrix[3][3]=1;
}

void  MatrixARB::RotateXri ( float Xr,float TMatrix [ 4 ] [ 4 ])
{
	//Generate 3D rotation matrix:
	float TempEqX = Xr;

	float cx = (float)cos(TempEqX);
	float sx = (float)sin(TempEqX);

	//Initialize X rotation matrix:
	TMatrix[0][0]=1;  TMatrix[0][1]=0;        TMatrix[0][2]=0;       TMatrix[0][3]=0;
	TMatrix[1][0]=0;  TMatrix[1][1]=cx;       TMatrix[1][2]=sx;      TMatrix[1][3]=0;
	TMatrix[2][0]=0;  TMatrix[2][1]=-sx;      TMatrix[2][2]=cx;      TMatrix[2][3]=0;
	TMatrix[3][0]=0;  TMatrix[3][1]=0;        TMatrix[3][2]=0;       TMatrix[3][3]=1;
}

void  MatrixARB::RotateYri ( float Ya,float TMatrix [ 4 ] [ 4 ])
{
	//Generate 3D rotation matrix:
	float TempEqY = Ya;

	float cy = (float)cos(TempEqY);
	float sy = (float)sin(TempEqY);

	//Initialize Y rotation matrix:
	TMatrix[0][0]=cy; TMatrix[0][1]=0;   TMatrix[0][2]=-sy; TMatrix[0][3]=0;
	TMatrix[1][0]=0;  TMatrix[1][1]=1;   TMatrix[1][2]=0;   TMatrix[1][3]=0;
	TMatrix[2][0]=sy; TMatrix[2][1]=0;   TMatrix[2][2]=cy;  TMatrix[2][3]=0;
	TMatrix[3][0]=0;  TMatrix[3][1]=0;   TMatrix[3][2]=0;   TMatrix[3][3]=1;
}

void  MatrixARB::RotateZ ( float & Za,float TMatrix [ 4 ] [ 4 ])
{
	//Generate 3D rotation matrix:
	float TempEqZ = 0.0f;

	if(Za != 0.0f)
	TempEqZ = (float)PI * ( Za / 180.0f);

	float cz = (float)cos(TempEqZ);
	float sz = (float)sin(TempEqZ);

	//rotation first to align the 3D Z axis with the 2D Z axis.
	TMatrix[0][0]=cz;  TMatrix[0][1]=sz;  TMatrix[0][2]=0;    TMatrix[0][3]=0;
	TMatrix[1][0]=-sz; TMatrix[1][1]=cz;  TMatrix[1][2]=0;    TMatrix[1][3]=0;
	TMatrix[2][0]=0;   TMatrix[2][1]=0;   TMatrix[2][2]=1;    TMatrix[2][3]=0;
	TMatrix[3][0]=0;   TMatrix[3][1]=0;   TMatrix[3][2]=0;    TMatrix[3][3]=1;
}

D3DVECTOR MatrixARB::RotateY_WLD ( float & Ya, D3DVECTOR & curPos)
{
	D3DMATRIX m_tempMatGLB;
	D3DVECTOR m_tempXYZ;
	//Generate 3D rotation matrix:

	float m_t1 = (float)PI * ( Ya / 180.0f);

	float m_t2 = (float)cos(m_t1);//cy
	float m_t3 = (float)sin(m_t1);//sy

	//Initialize Y rotation matrix:
	m_tempMatGLB.m[0][0]=m_t2; m_tempMatGLB.m[0][1]=0;   m_tempMatGLB.m[0][2]=-m_t3; m_tempMatGLB.m[0][3]=0;
	m_tempMatGLB.m[1][0]=0;    m_tempMatGLB.m[1][1]=1;   m_tempMatGLB.m[1][2]=0;     m_tempMatGLB.m[1][3]=0;
	m_tempMatGLB.m[2][0]=m_t3; m_tempMatGLB.m[2][1]=0;   m_tempMatGLB.m[2][2]=m_t2;  m_tempMatGLB.m[2][3]=0;
	m_tempMatGLB.m[3][0]=0;    m_tempMatGLB.m[3][1]=0;   m_tempMatGLB.m[3][2]=0;     m_tempMatGLB.m[3][3]=1;


	//output vertex
	m_tempXYZ.x = (m_tempMatGLB.m[0][0]*curPos.x)+(m_tempMatGLB.m[1][0]*curPos.y)+(m_tempMatGLB.m[2][0]*curPos.z);
	m_tempXYZ.y = (m_tempMatGLB.m[0][1]*curPos.x)+(m_tempMatGLB.m[1][1]*curPos.y)+(m_tempMatGLB.m[2][1]*curPos.z);
	m_tempXYZ.z = (m_tempMatGLB.m[0][2]*curPos.x)+(m_tempMatGLB.m[1][2]*curPos.y)+(m_tempMatGLB.m[2][2]*curPos.z);

	return m_tempXYZ;
}

D3DVECTOR MatrixARB::Rotate_WLD ( float & Ya,D3DVECTOR & offset,D3DVECTOR & curPos)
{
	//Generate 3D rotation matrix:
	D3DMATRIX m_tempMatGLB;
	D3DVECTOR m_tempXYZ;

	float m_t1 = (float)PI * ( Ya / 180.0f);

	float m_t2 = (float)cos(m_t1);//cy
	float m_t3 = (float)sin(m_t1);//sy

	//Initialize Y rotation matrix:
	m_tempMatGLB.m[0][0]=m_t2; m_tempMatGLB.m[0][1]=0;   m_tempMatGLB.m[0][2]=-m_t3; m_tempMatGLB.m[0][3]=0;
	m_tempMatGLB.m[1][0]=0;    m_tempMatGLB.m[1][1]=1;   m_tempMatGLB.m[1][2]=0;     m_tempMatGLB.m[1][3]=0;
	m_tempMatGLB.m[2][0]=m_t3; m_tempMatGLB.m[2][1]=0;   m_tempMatGLB.m[2][2]=m_t2;  m_tempMatGLB.m[2][3]=0;
	m_tempMatGLB.m[3][0]=0;    m_tempMatGLB.m[3][1]=0;   m_tempMatGLB.m[3][2]=0;     m_tempMatGLB.m[3][3]=1;

	curPos.x = curPos.x + offset.x;
	curPos.y = curPos.y + offset.y;
	curPos.z = curPos.z + offset.z;

	//output vertex
	m_tempXYZ.x = (m_tempMatGLB.m[0][0]*curPos.x)+(m_tempMatGLB.m[1][0]*curPos.y)+(m_tempMatGLB.m[2][0]*curPos.z);
	m_tempXYZ.y = (m_tempMatGLB.m[0][1]*curPos.x)+(m_tempMatGLB.m[1][1]*curPos.y)+(m_tempMatGLB.m[2][1]*curPos.z);
	m_tempXYZ.z = (m_tempMatGLB.m[0][2]*curPos.x)+(m_tempMatGLB.m[1][2]*curPos.y)+(m_tempMatGLB.m[2][2]*curPos.z);

	return m_tempXYZ;
}
   
void MatrixARB::Translate(float Xt, float Yt, float Zt, float TMatrix[4][4])
{
	//Create 3D translation matrix:
	InitMat ( TMatrix );
	//Declare translation matrix:
	float Tmat [ 4 ] [ 4 ];

	//Save translation values:
	//XTrans = Xt; YTrans = Yt; ZTrans = Zt;

	//Initialize translation matrix:
	Tmat[0][0]=1;  Tmat[0][1]=0;  Tmat[0][2]=0;  Tmat[0][3]=0;
	Tmat[1][0]=0;  Tmat[1][1]=1;  Tmat[1][2]=0;  Tmat[1][3]=0;
	Tmat[2][0]=0;  Tmat[2][1]=0;  Tmat[2][2]=1;  Tmat[2][3]=0;
	Tmat[3][0]=Xt; Tmat[3][1]=Yt; Tmat[3][2]=Zt; Tmat[3][3]=1;

	//Merge matrix with master matrix:
	MergeMatrices( Tmat, TMatrix );
	//TrnsfrMtrxMult(TMatrix, TMatrix, Tmat);
}

void MatrixARB::scale (float Xt, float Yt, float Zt, float TMatrix[4][4])
{
	//Create 3D translation matrix:
	InitMat ( TMatrix );
	//Declare translation matrix:
	float Tmat [ 4 ] [ 4 ];

	//Save translation values:
	//XTrans = Xt; YTrans = Yt; ZTrans = Zt;

	//Initialize translation matrix:
	Tmat[0][0]=Xt;  Tmat[0][1]=0;  Tmat[0][2]=0;  Tmat[0][3]=0;
	Tmat[1][0]=0;  Tmat[1][1]=Yt;  Tmat[1][2]=0;  Tmat[1][3]=0;
	Tmat[2][0]=0;  Tmat[2][1]=0;  Tmat[2][2]=Zt;  Tmat[2][3]=0;
	Tmat[3][0]=0;  Tmat[3][1]=0;  Tmat[3][2]=0;  Tmat[3][3]=1;

	//Merge matrix with master matrix:
	MergeMatrices ( Tmat, TMatrix );
}

void _fastcall MatrixARB::TrnsfrMtrxInvrs(float Matrix[4][4], float InvMatrix[4][4])
{
	//This code should be fixed to work with CFrameObj Class LWM09FEB2000
	//All matrixes are stored in column row format i.e. A[column][row]
	InvMatrix[0][0] = Matrix[0][0];
	InvMatrix[1][0] = Matrix[0][1];
	InvMatrix[2][0] = Matrix[0][2];
	InvMatrix[3][0] = -(Matrix[0][0] * Matrix[3][0]
					  + Matrix[0][1] * Matrix[3][1]
					  + Matrix[0][2] * Matrix[3][2]);

	InvMatrix[0][1] = Matrix[1][0];
	InvMatrix[1][1] = Matrix[1][1];
	InvMatrix[2][1] = Matrix[1][2];
	InvMatrix[3][1] = -(Matrix[1][0] * Matrix[3][0]
					  + Matrix[1][1] * Matrix[3][1]
					  + Matrix[1][2] * Matrix[3][2]);

	InvMatrix[0][2] = Matrix[2][0];
	InvMatrix[1][2] = Matrix[2][1];
	InvMatrix[2][2] = Matrix[2][2];
	InvMatrix[3][2] = -(Matrix[0][2] * Matrix[3][0]
					  + Matrix[1][2] * Matrix[3][1]
					  + Matrix[2][2] * Matrix[3][2]);

	InvMatrix[0][3] = 0.0;
	InvMatrix[1][3] = 0.0;
	InvMatrix[2][3] = 0.0;
	InvMatrix[3][3] = 1.0;

}


D3DVECTOR MatrixARB::UpdatePolygonVertex(float matrixLoc[4][4],D3DVECTOR & polygonLoc)
{
	D3DVECTOR tempP;
	tempP.x = (matrixLoc[0][0]*polygonLoc.x)+(matrixLoc[1][0]*polygonLoc.y)+(matrixLoc[2][0]*polygonLoc.z)+(matrixLoc[3][0]);
	tempP.y = (matrixLoc[0][1]*polygonLoc.x)+(matrixLoc[1][1]*polygonLoc.y)+(matrixLoc[2][1]*polygonLoc.z)+(matrixLoc[3][1]);
	tempP.z = (matrixLoc[0][2]*polygonLoc.x)+(matrixLoc[1][2]*polygonLoc.y)+(matrixLoc[2][2]*polygonLoc.z)+(matrixLoc[3][2]);
	return tempP;
}

D3DVECTOR MatrixARB::UpdatePolygonVertexRotationOnly(float matrixLoc[4][4],D3DVECTOR polygonLoc)
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

AXDVECTOR MatrixARB::UpdatePolygonVertexRotationOnlyD(AXDXMATRIX matrixLoc,AXDVECTOR polygonLoc)
{
	AXDVECTOR tempP;
	tempP.x = (matrixLoc.m[0][0]*polygonLoc.x)+(matrixLoc.m[1][0]*polygonLoc.y)+(matrixLoc.m[2][0]*polygonLoc.z);
	tempP.y = (matrixLoc.m[0][1]*polygonLoc.x)+(matrixLoc.m[1][1]*polygonLoc.y)+(matrixLoc.m[2][1]*polygonLoc.z);
	tempP.z = (matrixLoc.m[0][2]*polygonLoc.x)+(matrixLoc.m[1][2]*polygonLoc.y)+(matrixLoc.m[2][2]*polygonLoc.z);

	polygonLoc.x = tempP.x;
	polygonLoc.y = tempP.y;
	polygonLoc.z = tempP.z;
	return polygonLoc;
}

/*D3DVECTOR MatrixARB::UpdatePolygonVertexRotationOnlyD(float matrixLoc[4][4],D3DVECTOR polygonLoc)
{
	D3DVECTOR tempP;
	tempP.x = (matrixLoc[0][0]*polygonLoc.x)+(matrixLoc[1][0]*polygonLoc.y)+(matrixLoc[2][0]*polygonLoc.z);
	tempP.y = (matrixLoc[0][1]*polygonLoc.x)+(matrixLoc[1][1]*polygonLoc.y)+(matrixLoc[2][1]*polygonLoc.z);
	tempP.z = (matrixLoc[0][2]*polygonLoc.x)+(matrixLoc[1][2]*polygonLoc.y)+(matrixLoc[2][2]*polygonLoc.z);

	polygonLoc.x = tempP.x;
	polygonLoc.y = tempP.y;
	polygonLoc.z = tempP.z;
	return polygonLoc;
}*/


D3DVECTOR MatrixARB::GetLocalPositionFromMatrixView2(D3DMATRIX & invMatrix,D3DMATRIX & matrixLoc,D3DVECTOR & objVect)
{
	D3DVECTOR tempP;
	D3DVECTOR zeroedPos;
	zeroedPos.x = objVect.x - matrixLoc._41;
	zeroedPos.y = objVect.y - matrixLoc._42;
	zeroedPos.z = objVect.z - matrixLoc._43;
	tempP.x = (invMatrix.m[0][0]*zeroedPos.x)+(invMatrix.m[1][0]*zeroedPos.y)+(invMatrix.m[2][0]*zeroedPos.z);
	tempP.y = (invMatrix.m[0][1]*zeroedPos.x)+(invMatrix.m[1][1]*zeroedPos.y)+(invMatrix.m[2][1]*zeroedPos.z);
	tempP.z = (invMatrix.m[0][2]*zeroedPos.x)+(invMatrix.m[1][2]*zeroedPos.y)+(invMatrix.m[2][2]*zeroedPos.z);
	return tempP;
}


D3DVECTOR MatrixARB::GetLocalPositionFromMatrixView(D3DMATRIX & matrixLoc,D3DVECTOR & objVect)
{
	D3DVECTOR tempP;
	D3DVECTOR zeroedPos;
	zeroedPos.x = objVect.x - matrixLoc._41;
	zeroedPos.y = objVect.y - matrixLoc._42;
	zeroedPos.z = objVect.z - matrixLoc._43;
	D3DMATRIX invMatrix;
	D3DMath_MatrixInvert(invMatrix,matrixLoc);
	tempP.x = (invMatrix.m[0][0]*zeroedPos.x)+(invMatrix.m[1][0]*zeroedPos.y)+(invMatrix.m[2][0]*zeroedPos.z);
	tempP.y = (invMatrix.m[0][1]*zeroedPos.x)+(invMatrix.m[1][1]*zeroedPos.y)+(invMatrix.m[2][1]*zeroedPos.z);
	tempP.z = (invMatrix.m[0][2]*zeroedPos.x)+(invMatrix.m[1][2]*zeroedPos.y)+(invMatrix.m[2][2]*zeroedPos.z);

	return tempP;
}


D3DVECTOR MatrixARB::GetLocalPositionFromMatrixViewRotOnly(D3DMATRIX & matrixLoc,D3DVECTOR & objVect)
{
	D3DVECTOR tempP;
	D3DMATRIX invMatrix;
	D3DMath_MatrixInvert(invMatrix,matrixLoc);
	tempP.x = (invMatrix.m[0][0]*objVect.x)+(invMatrix.m[1][0]*objVect.y)+(invMatrix.m[2][0]*objVect.z);
	tempP.y = (invMatrix.m[0][1]*objVect.x)+(invMatrix.m[1][1]*objVect.y)+(invMatrix.m[2][1]*objVect.z);
	tempP.z = (invMatrix.m[0][2]*objVect.x)+(invMatrix.m[1][2]*objVect.y)+(invMatrix.m[2][2]*objVect.z);

	return tempP;
}

AXDVECTOR MatrixARB::GetLocalPositionFromMatrixViewRotOnlyD(AXDXMATRIX & matrixLoc,AXDVECTOR & objVect)
{
	AXDVECTOR tempP;
	AXDXMATRIX invMatrix;
	D3DMath_MatrixInvertD(invMatrix,matrixLoc);
	tempP.x = (invMatrix.m[0][0]*objVect.x)+(invMatrix.m[1][0]*objVect.y)+(invMatrix.m[2][0]*objVect.z);
	tempP.y = (invMatrix.m[0][1]*objVect.x)+(invMatrix.m[1][1]*objVect.y)+(invMatrix.m[2][1]*objVect.z);
	tempP.z = (invMatrix.m[0][2]*objVect.x)+(invMatrix.m[1][2]*objVect.y)+(invMatrix.m[2][2]*objVect.z);

	return tempP;
}

void _fastcall MatrixARB::TrnsfrMtrxMult(float MultOut[4][4], float A[4][4], float B[4][4])
{
	MultOut[0][0] = A[0][0]*B[0][0] + A[0][1]*B[1][0] + A[0][2]*B[2][0] + A[0][3]*B[3][0];
	MultOut[0][1] = A[0][0]*B[0][1] + A[0][1]*B[1][1] + A[0][2]*B[2][1] + A[0][3]*B[3][1];
	MultOut[0][2] = A[0][0]*B[0][2] + A[0][1]*B[1][2] + A[0][2]*B[2][2] + A[0][3]*B[3][2];
	MultOut[0][3] = A[0][0]*B[0][3] + A[0][1]*B[1][3] + A[0][2]*B[2][3] + A[0][3]*B[3][3];

	MultOut[1][0] = A[1][0]*B[0][0] + A[1][1]*B[1][0] + A[1][2]*B[2][0] + A[1][3]*B[3][0];
	MultOut[1][1] = A[1][0]*B[0][1] + A[1][1]*B[1][1] + A[1][2]*B[2][1] + A[1][3]*B[3][1];
	MultOut[1][2] = A[1][0]*B[0][2] + A[1][1]*B[1][2] + A[1][2]*B[2][2] + A[1][3]*B[3][2];
	MultOut[1][3] = A[1][0]*B[0][3] + A[1][1]*B[1][3] + A[1][2]*B[2][3] + A[1][3]*B[3][3];

	MultOut[2][0] = A[2][0]*B[0][0] + A[2][1]*B[1][0] + A[2][2]*B[2][0] + A[2][3]*B[3][0];
	MultOut[2][1] = A[2][0]*B[0][1] + A[2][1]*B[1][1] + A[2][2]*B[2][1] + A[2][3]*B[3][1];
	MultOut[2][2] = A[2][0]*B[0][2] + A[2][1]*B[1][2] + A[2][2]*B[2][2] + A[2][3]*B[3][2];
	MultOut[2][3] = A[2][0]*B[0][3] + A[2][1]*B[1][3] + A[2][2]*B[2][3] + A[2][3]*B[3][3];

	MultOut[3][0] = A[3][0]*B[0][0] + A[3][1]*B[1][0] + A[3][2]*B[2][0] + A[3][3]*B[3][0];
	MultOut[3][1] = A[3][0]*B[0][1] + A[3][1]*B[1][1] + A[3][2]*B[2][1] + A[3][3]*B[3][1];
	MultOut[3][2] = A[3][0]*B[0][2] + A[3][1]*B[1][2] + A[3][2]*B[2][2] + A[3][3]*B[3][2];
	MultOut[3][3] = A[3][0]*B[0][3] + A[3][1]*B[1][3] + A[3][2]*B[2][3] + A[3][3]*B[3][3];

}

void _fastcall MatrixARB::MergeMatrices1( float Dest[4][4], float Source[4][4])
{

	float temp[4][4];

	temp[0][0] = Source[0][0]*Dest[0][0] + Source[0][1]*Dest[1][0] + Source[0][2]*Dest[2][0] + Source[0][3]*Dest[3][0];
	temp[0][1] = Source[0][0]*Dest[0][1] + Source[0][1]*Dest[1][1] + Source[0][2]*Dest[2][1] + Source[0][3]*Dest[3][1];
	temp[0][2] = Source[0][0]*Dest[0][2] + Source[0][1]*Dest[1][2] + Source[0][2]*Dest[2][2] + Source[0][3]*Dest[3][2];
	temp[0][3] = Source[0][0]*Dest[0][3] + Source[0][1]*Dest[1][3] + Source[0][2]*Dest[2][3] + Source[0][3]*Dest[3][3];

	temp[1][0] = Source[1][0]*Dest[0][0] + Source[1][1]*Dest[1][0] + Source[1][2]*Dest[2][0] + Source[1][3]*Dest[3][0];
	temp[1][1] = Source[1][0]*Dest[0][1] + Source[1][1]*Dest[1][1] + Source[1][2]*Dest[2][1] + Source[1][3]*Dest[3][1];
	temp[1][2] = Source[1][0]*Dest[0][2] + Source[1][1]*Dest[1][2] + Source[1][2]*Dest[2][2] + Source[1][3]*Dest[3][2];
	temp[1][3] = Source[1][0]*Dest[0][3] + Source[1][1]*Dest[1][3] + Source[1][2]*Dest[2][3] + Source[1][3]*Dest[3][3];

	temp[2][0] = Source[2][0]*Dest[0][0] + Source[2][1]*Dest[1][0] + Source[2][2]*Dest[2][0] + Source[2][3]*Dest[3][0];
	temp[2][1] = Source[2][0]*Dest[0][1] + Source[2][1]*Dest[1][1] + Source[2][2]*Dest[2][1] + Source[2][3]*Dest[3][1];
	temp[2][2] = Source[2][0]*Dest[0][2] + Source[2][1]*Dest[1][2] + Source[2][2]*Dest[2][2] + Source[2][3]*Dest[3][2];
	temp[2][3] = Source[2][0]*Dest[0][3] + Source[2][1]*Dest[1][3] + Source[2][2]*Dest[2][3] + Source[2][3]*Dest[3][3];

	temp[3][0] = Source[3][0]*Dest[0][0] + Source[3][1]*Dest[1][0] + Source[3][2]*Dest[2][0] + Source[3][3]*Dest[3][0];
	temp[3][1] = Source[3][0]*Dest[0][1] + Source[3][1]*Dest[1][1] + Source[3][2]*Dest[2][1] + Source[3][3]*Dest[3][1];
	temp[3][2] = Source[3][0]*Dest[0][2] + Source[3][1]*Dest[1][2] + Source[3][2]*Dest[2][2] + Source[3][3]*Dest[3][2];
	temp[3][3] = Source[3][0]*Dest[0][3] + Source[3][1]*Dest[1][3] + Source[3][2]*Dest[2][3] + Source[3][3]*Dest[3][3];

	Source[0][0] = temp[0][0];
	Source[0][1] = temp[0][1];
	Source[0][2] = temp[0][2];
	Source[0][3] = temp[0][3];

	Source[1][0] = temp[1][0];
	Source[1][1] = temp[1][1];
	Source[1][2] = temp[1][2];
	Source[1][3] = temp[1][3];

	Source[2][0] = temp[2][0];
	Source[2][1] = temp[2][1];
	Source[2][2] = temp[2][2];
	Source[2][3] = temp[2][3];

	Source[3][0] = temp[3][0];
	Source[3][1] = temp[3][1];
	Source[3][2] = temp[3][2];
	Source[3][3] = temp[3][3];
}

void MatrixARB::MergeMatrices ( float Dest [ 4 ] [ 4 ], float Source [ 4 ] [ 4 ] )
{
	//Multiply Source by Dest; store result in Temp:
	short unsigned int i = 0;
	float Temp [4][4];
	for(i = 0; i < 4; i++)
		for(short unsigned int j = 0; j < 4; j++)
		{
			Temp [ i ] [ j ] = (Source[i][0] * Dest[0][j])
							 + (Source[i][1] * Dest[1][j])
							 + (Source[i][2] * Dest[2][j])
							 + (Source[i][3] * Dest[3][j]);
		}
	//Copy Temp to Dest:
	for(i = 0; i < 4; i++)
	{
		Source[i][0] = Temp[i][0];
		Source[i][1] = Temp[i][1];
		Source[i][2] = Temp[i][2];
		Source[i][3] = Temp[i][3];
	}
}

unsigned long int MatrixARB::sqrt_5(unsigned long int x)
{
	register unsigned long int xroot, m2, x2;

	xroot = 0;  m2 = 1 << MAX_BIT * 2;
	do
	{
		x2 = xroot + m2;
		xroot >>= 1;
		if (x2 <= x)
		{
			x -= x2;  xroot += m2;
		}
	}while (m2 >>= 2);
	
	if(xroot < x)
		return xroot + 1;

	return xroot;
}



float MatrixARB::CrossProductFloat(float v1, float v2)
{
	return v1 * v2 - v1 * v2;




}


D3DVECTOR MatrixARB::CrossProduct(const _D3DVECTOR& v1, const _D3DVECTOR& v2)
{
	_D3DVECTOR result;
 
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
 
	return result;
}

float MatrixARB::DotProduct(const _D3DVECTOR& v1, const _D3DVECTOR& v2)
{
	float dot = v1.x*v2.x + v1.y * v2.y + v1.z*v2.z;
	if(dot > 1.0f)
		return 1.0f;

	return dot;
}

//---------------------------------------------------------//
//------------------------look at--------------------------//
//---------------------------------------------------------//
void MatrixARB::FaceTowardPoint(D3DVECTOR & targetVect,D3DMATRIX * inputMatrix)
{
    targetVect.y += .001f;

	float opp   = 0;
	float adj   = 0;
	float yaw   = 0; 
	float pitch = 0;
		
    D3DVECTOR thisVect;
	thisVect.x = inputMatrix->_41;
	thisVect.y = inputMatrix->_42;
	thisVect.z = inputMatrix->_43;
    //GetPosition(NULL,&thisVect);//get position and respect hiarchy


	opp = thisVect.z - targetVect.z;
	adj = thisVect.x - targetVect.x;
	yaw = atan2(adj,opp);

	adj   = thisVect.y - targetVect.y;
	
	opp = sqrt((targetVect.x-thisVect.x)*(targetVect.x-thisVect.x) + 
		       (targetVect.z-thisVect.z)*(targetVect.z-thisVect.z));
	pitch = atan2(adj,opp);
	yaw   = -yaw;
	pitch = pitch;

	float cosyaw = cos(yaw);
	float sinyaw = sin(yaw);
	float cospitch = cos(pitch);
	float sinpitch = sin(pitch);
     if(yaw && pitch) {
		inputMatrix->_11 = cosyaw;
		inputMatrix->_21 = sinpitch*sinyaw;
		inputMatrix->_31 = -cospitch*sinyaw;
		inputMatrix->_22 = cospitch;
		inputMatrix->_32 = sinpitch;
		inputMatrix->_13 = sinyaw;       
		inputMatrix->_23 = -sinpitch*cosyaw;
		inputMatrix->_33 = cospitch*cosyaw;

	 }
	 else if(yaw)
     {
		inputMatrix->_11 =  cosyaw;
		inputMatrix->_13 = -sinyaw;
		inputMatrix->_31 =  sinyaw;
		inputMatrix->_33 =  cosyaw;
	 }
	 else if(pitch)
	 {
		inputMatrix->_22 = cospitch;
		inputMatrix->_32 = sinpitch;
		inputMatrix->_23 = -sinpitch;
		inputMatrix->_33 = cospitch;
	 }
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void MatrixARB::AngleAxisTrnsfrm(D3DMATRIX& q, D3DVECTOR& Axis, float Angle)
{
	float V;
	float S;
	float C;
	float KX;
	float KY;
	float KZ;

	S = sin(Angle);
	V = 1 - cos(Angle);
	C = cos(Angle);
	KX = Axis.x;
	KY = Axis.y;
	KZ = Axis.z;

	q._11 = KX*KX*V + C;
	q._12 = KX*KY*V + KZ*S;
	q._13 = KX*KZ*V - KY*S;
	q._14 = 0.0f;

	q._21 = KY*KX*V - KZ*S;
	q._22 = KY*KY*V + C;
	q._23 = KY*KZ*V +KX*S;
	q._24 = 0.0f;

	q._31 = KZ*KX*V + KY*S;
	q._32 = KZ*KY*V - KX*S;
	q._33 = KZ*KZ*V + C;
	q._34 = 0.0f;
	//q._41 = 0.0f;
	//q._42 = 0.0f;
	//q._43 = 0.0f;
	//q._44 = 1.0f;
}
//---------------------------------------------------------//
//---------------------GetOrientation----------------------//
//---------------------------------------------------------//
void MatrixARB::GetOrientation(D3DMATRIX & input,D3DVECTOR *dirVect,D3DVECTOR *upVect)
{
	upVect->x = 0.0f;
	upVect->y = 1.0f;
	upVect->z = 0.0f;
	dirVect->x = 0.0f;
	dirVect->y = 0.0f;
	dirVect->z = 1.0f;
	*upVect = UpdatePolygonVertexRotationOnly(input.m,*upVect);//get position in world space
	*dirVect = UpdatePolygonVertexRotationOnly(input.m,*dirVect);//get position in world space
}
//---------------------------------------------------------//
//---------------------GetOrientation----------------------//
//---------------------------------------------------------//
void MatrixARB::GetDirVect(D3DMATRIX & input,D3DVECTOR *dirVect)
{
	dirVect->x = 0.0f;
	dirVect->y = 0.0f;
	dirVect->z = 1.0f;
	*dirVect = UpdatePolygonVertexRotationOnly(input.m,*dirVect);//get position in world space
}
//---------------------------------------------------------//
//---------------------SetOrientation----------------------//
//---------------------------------------------------------//
void MatrixARB::SetOrientation(D3DMATRIX * matrixInOut,
							   D3DVECTOR & dirVect,
							   D3DVECTOR & upVect)
{

	matrixInOut->_11 = upVect.y*dirVect.z - upVect.z*dirVect.y;
	matrixInOut->_12 = upVect.z*dirVect.x - upVect.x*dirVect.z;
	matrixInOut->_13 = upVect.x*dirVect.y - upVect.y*dirVect.x;

	matrixInOut->_21 = upVect.x;
	matrixInOut->_22 = upVect.y;
	matrixInOut->_23 = upVect.z;

	matrixInOut->_31 = dirVect.x;
	matrixInOut->_32 = dirVect.y;
	matrixInOut->_33 = dirVect.z;

	//List_frameMatrix._44 = 1.0;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between points BABY!!-----------------------------------//
D3DVECTOR MatrixARB::InterpolatePointsHiRes(D3DVECTOR StartVec, 
						                    D3DVECTOR EndVec,
					                        float NumOfStages, 
					                        float ThisStage)
{
	D3DVECTOR StageVec;
	float propfactor;
	if (NumOfStages <= 0)
		NumOfStages = 1;

	propfactor = (float)ThisStage/NumOfStages;
	StageVec.x = StartVec.x + propfactor * (EndVec.x - StartVec.x);
	StageVec.y = StartVec.y + propfactor * (EndVec.y - StartVec.y);
	StageVec.z = StartVec.z + propfactor * (EndVec.z - StartVec.z);
	
	return StageVec;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between points BABY!!-----------------------------------//
D3DVECTOR MatrixARB::InterpolatePoints(D3DVECTOR & StartVec, 
						               D3DVECTOR & EndVec,
					                   int NumOfStages, 
					                   int ThisStage)
{
	D3DVECTOR StageVec;
	float propfactor;
	if (NumOfStages <= 0)
		NumOfStages = 1;

	propfactor = (float)ThisStage/NumOfStages;
	StageVec.x = StartVec.x + propfactor * (EndVec.x - StartVec.x);
	StageVec.y = StartVec.y + propfactor * (EndVec.y - StartVec.y);
	StageVec.z = StartVec.z + propfactor * (EndVec.z - StartVec.z);
	
	return StageVec;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between points BABY!!-----------------------------------//
D3DVECTOR MatrixARB::InterpolatePointsX(D3DVECTOR & StartVec, 
						                D3DVECTOR & EndVec,
					                    double NumOfStages, 
					                    double ThisStage)
{
	D3DVECTOR StageVec;
	float propfactor;
	if (NumOfStages <= 0)
		NumOfStages = 1;

	propfactor = (float)(ThisStage/NumOfStages);
	StageVec.x = StartVec.x + propfactor * (EndVec.x - StartVec.x);
	StageVec.y = StartVec.y + propfactor * (EndVec.y - StartVec.y);
	StageVec.z = StartVec.z + propfactor * (EndVec.z - StartVec.z);
	
	return StageVec;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between points BABY!!-----------------------------------//
D3DVECTOR MatrixARB::InterpolatePointsExp(D3DVECTOR StartVec, 
						                       D3DVECTOR EndVec,
					                           int NumOfStages, 
					                           int ThisStage,
											   float exp1,
											   float exp2)
{
	D3DVECTOR StageVec;
	float propfactor;
	if (NumOfStages <= 0)
		NumOfStages = 1;

	float numStagesFloat = (float)NumOfStages;
	float thisStageFloat = (float)ThisStage;
	propfactor = (float)thisStageFloat/numStagesFloat;
	propfactor = (thisStageFloat/numStagesFloat)*((exp1-(propfactor+exp2)));
	StageVec.x = (StartVec.x + propfactor * (EndVec.x - StartVec.x));
	StageVec.y = (StartVec.y + propfactor * (EndVec.y - StartVec.y));
	StageVec.z = (StartVec.z + propfactor * (EndVec.z - StartVec.z));
	
	return StageVec;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------get matrix conversion----------------------------------------------------//
D3DVECTOR MatrixARB::Normalize (D3DVECTOR & v)
{
	float dist = Magnitude(v);

	if(dist == 0.0f)
		dist = .001f;
	  

	v.x = v.x / dist;
	v.y = v.y / dist;
	v.z = v.z / dist;

	return v;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float MatrixARB::MagnitudeSquared(float &Mx1,float &My1, float &Mz1,float &Mx2, float &My2,float &Mz2)
{

	float temp_Mxyz = (((Mx1 - Mx2) * (Mx1 - Mx2)) + ((My1 - My2) * (My1 - My2)) + ((Mz1 - Mz2) * (Mz1 - Mz2)));

	return (float)sqrt(temp_Mxyz);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float MatrixARB::MagnitudeSquared(D3DMATRIX & matrix, D3DMATRIX & matrix2)

{
	float temp_Mxyz = (((matrix._41 - matrix2._41) * (matrix._41 - matrix2._41)) + ((matrix._42 - matrix2._42) * (matrix._42 - matrix2._42)) + ((matrix._43 - matrix2._43) * (matrix._43 - matrix2._43)));

	return (float)sqrt(temp_Mxyz);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float MatrixARB::MagnitudeSquared(D3DMATRIX & matrix, D3DVECTOR & pos)

{
	float temp_Mxyz = (((matrix._41 - pos.x) * (matrix._41 - pos.x)) + ((matrix._42 - pos.y) * (matrix._42 - pos.y)) + ((matrix._43 - pos.z) * (matrix._43 - pos.z)));

	return (float)sqrt(temp_Mxyz);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float MatrixARB::MagnitudeSquared(D3DVECTOR & po, D3DVECTOR & pos)

{
	float temp_Mxyz = (((po.x - pos.x) * (po.x - pos.x)) + ((po.y - pos.y) * (po.y - pos.y)) + ((po.z - pos.z) * (po.z - pos.z)));

	return (float)sqrt(temp_Mxyz);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float MatrixARB::MagnitudeSquaredFast3(float Mx1,float My1, float Mz1,float Mx2, float My2,float Mz2)


{

	return 0.0f;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float MatrixARB::MagnitudeSquared2(float Mx1,float My1, float Mz1,float Mx2, float My2,float Mz2)
{

	float temp_Mxyz = (((Mx1 - Mx2) * (Mx1 - Mx2)) + ((My1 - My2) * (My1 - My2)) + ((Mz1 - Mz2) * (Mz1 - Mz2)));

	return (float)sqrt_5((unsigned long)temp_Mxyz);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
D3DVECTOR MatrixARB::GetPointFromSpace(D3DMATRIX invFocusMatrix,D3DVECTOR point)
{
	D3DVECTOR retVect;
	float xpos, ypos, zpos;

	xpos = point.x;
	ypos = point.y;
	zpos = point.z;

	xpos -= invFocusMatrix._41;
	ypos -= invFocusMatrix._42;
	zpos -= invFocusMatrix._43;

	retVect.x = invFocusMatrix.m[0][0] * xpos +

				invFocusMatrix.m[1][0] * ypos +
				invFocusMatrix.m[2][0] * zpos;
		
	retVect.y = invFocusMatrix.m[0][1] * xpos +

				invFocusMatrix.m[1][1] * ypos +
				invFocusMatrix.m[2][1] * zpos;
		
	retVect.z = invFocusMatrix.m[0][2] * xpos +

				invFocusMatrix.m[1][2] * ypos +
				invFocusMatrix.m[2][2] * zpos;
	return retVect;
}

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
 void MatrixARB::fastMatrixMultiply(D3DMATRIX & R,D3DMATRIX A, D3DMATRIX B)
{
	D3DMATRIX result;
	register D3DMATRIX *result_pointer = &result;
	register D3DMATRIX *A_pointer = &A;
	register D3DMATRIX *B_pointer = &B;

	
	result_pointer->_11 = 	A_pointer->_11 * B_pointer->_11 + 
						        A_pointer->_12 *	B_pointer->_21 + 
							    A_pointer->_13 * B_pointer->_31 + 
								A_pointer->_14 * B_pointer->_41;
	result_pointer->_12 = 	A_pointer->_11 * B_pointer->_12 + 
								A_pointer->_12 *	B_pointer->_22 + 
								A_pointer->_13 * B_pointer->_32 + 
								A_pointer->_14 * B_pointer->_42;
	result_pointer->_13 = 	A_pointer->_11 * B_pointer->_13 + 
								A_pointer->_12 *	B_pointer->_23 + 
								A_pointer->_13 * B_pointer->_33 + 
								A_pointer->_14 * B_pointer->_43;
	result_pointer->_14 = 	A_pointer->_11 * B_pointer->_14 + 
								A_pointer->_12 *	B_pointer->_24 + 
								A_pointer->_13 * B_pointer->_34 + 
								A_pointer->_14 * B_pointer->_44;
	result_pointer->_21 = 	A_pointer->_21 * B_pointer->_11 + 
								A_pointer->_22 *	B_pointer->_21 + 
								A_pointer->_23 * B_pointer->_31 + 
								A_pointer->_24 * B_pointer->_41;
	result_pointer->_22 = 	A_pointer->_21 * B_pointer->_12 + 
								A_pointer->_22 *	B_pointer->_22 + 
								A_pointer->_23 * B_pointer->_32 + 
								A_pointer->_24 * B_pointer->_42;
	result_pointer->_23 = 	A_pointer->_21 * B_pointer->_13 + 
								A_pointer->_22 *	B_pointer->_23 + 
								A_pointer->_23 * B_pointer->_33 + 
								A_pointer->_24 * B_pointer->_43;
	result_pointer->_24 = 	A_pointer->_21 * B_pointer->_14 + 
								A_pointer->_22 *	B_pointer->_24 + 
								A_pointer->_23 * B_pointer->_34 + 
								A_pointer->_24 * B_pointer->_44;
	result_pointer->_31 = 	A_pointer->_31 * B_pointer->_11 + 
								A_pointer->_32 *	B_pointer->_21 + 
								A_pointer->_33 * B_pointer->_31 + 
								A_pointer->_34 * B_pointer->_41;
	result_pointer->_32 = 	A_pointer->_31 * B_pointer->_12 + 
								A_pointer->_32 *	B_pointer->_22 + 
								A_pointer->_33 * B_pointer->_32 + 
								A_pointer->_34 * B_pointer->_42;
	result_pointer->_33 = 	A_pointer->_31 * B_pointer->_13 + 
								A_pointer->_32 *	B_pointer->_23 + 
								A_pointer->_33 * B_pointer->_33 + 
								A_pointer->_34 * B_pointer->_43;
	result_pointer->_34 = 	A_pointer->_31 * B_pointer->_14 + 
								A_pointer->_32 *	B_pointer->_24 + 
								A_pointer->_33 * B_pointer->_34 + 
								A_pointer->_34 * B_pointer->_44;
	result_pointer->_41 = 	A_pointer->_41 * B_pointer->_11 + 
								A_pointer->_42 *	B_pointer->_21 + 
								A_pointer->_43 * B_pointer->_31 + 
								A_pointer->_44 * B_pointer->_41;
	result_pointer->_42 = 	A_pointer->_41 * B_pointer->_12 + 
								A_pointer->_42 *	B_pointer->_22 + 
								A_pointer->_43 * B_pointer->_32 + 
								A_pointer->_44 * B_pointer->_42;
	result_pointer->_43 = 	A_pointer->_41 * B_pointer->_13 + 
								A_pointer->_42 *	B_pointer->_23 + 
								A_pointer->_43 * B_pointer->_33 + 
								A_pointer->_44 * B_pointer->_43;
	result_pointer->_44 = 	A_pointer->_41 * B_pointer->_14 + 
								A_pointer->_42 *	B_pointer->_24 + 
								A_pointer->_43 * B_pointer->_34 + 
								A_pointer->_44 * B_pointer->_44;
	R = result;
}
//-------------------------------------------------------------//
void MatrixARB::fastMatrixMultiplyD(AXDXMATRIX & R,AXDXMATRIX A, AXDXMATRIX B)
{
	AXDXMATRIX result;
	register AXDXMATRIX *result_pointer = &result;
	register AXDXMATRIX *A_pointer = &A;
	register AXDXMATRIX *B_pointer = &B;

	
	result_pointer->_11 = 	A_pointer->_11 * B_pointer->_11 + 
						        A_pointer->_12 *	B_pointer->_21 + 
							    A_pointer->_13 * B_pointer->_31 + 
								A_pointer->_14 * B_pointer->_41;
	result_pointer->_12 = 	A_pointer->_11 * B_pointer->_12 + 
								A_pointer->_12 *	B_pointer->_22 + 
								A_pointer->_13 * B_pointer->_32 + 
								A_pointer->_14 * B_pointer->_42;
	result_pointer->_13 = 	A_pointer->_11 * B_pointer->_13 + 
								A_pointer->_12 *	B_pointer->_23 + 
								A_pointer->_13 * B_pointer->_33 + 
								A_pointer->_14 * B_pointer->_43;
	result_pointer->_14 = 	A_pointer->_11 * B_pointer->_14 + 
								A_pointer->_12 *	B_pointer->_24 + 
								A_pointer->_13 * B_pointer->_34 + 
								A_pointer->_14 * B_pointer->_44;
	result_pointer->_21 = 	A_pointer->_21 * B_pointer->_11 + 
								A_pointer->_22 *	B_pointer->_21 + 
								A_pointer->_23 * B_pointer->_31 + 
								A_pointer->_24 * B_pointer->_41;
	result_pointer->_22 = 	A_pointer->_21 * B_pointer->_12 + 
								A_pointer->_22 *	B_pointer->_22 + 
								A_pointer->_23 * B_pointer->_32 + 
								A_pointer->_24 * B_pointer->_42;
	result_pointer->_23 = 	A_pointer->_21 * B_pointer->_13 + 
								A_pointer->_22 *	B_pointer->_23 + 
								A_pointer->_23 * B_pointer->_33 + 
								A_pointer->_24 * B_pointer->_43;
	result_pointer->_24 = 	A_pointer->_21 * B_pointer->_14 + 
								A_pointer->_22 *	B_pointer->_24 + 
								A_pointer->_23 * B_pointer->_34 + 
								A_pointer->_24 * B_pointer->_44;
	result_pointer->_31 = 	A_pointer->_31 * B_pointer->_11 + 
								A_pointer->_32 *	B_pointer->_21 + 
								A_pointer->_33 * B_pointer->_31 + 
								A_pointer->_34 * B_pointer->_41;
	result_pointer->_32 = 	A_pointer->_31 * B_pointer->_12 + 
								A_pointer->_32 *	B_pointer->_22 + 
								A_pointer->_33 * B_pointer->_32 + 
								A_pointer->_34 * B_pointer->_42;
	result_pointer->_33 = 	A_pointer->_31 * B_pointer->_13 + 
								A_pointer->_32 *	B_pointer->_23 + 
								A_pointer->_33 * B_pointer->_33 + 
								A_pointer->_34 * B_pointer->_43;
	result_pointer->_34 = 	A_pointer->_31 * B_pointer->_14 + 
								A_pointer->_32 *	B_pointer->_24 + 
								A_pointer->_33 * B_pointer->_34 + 
								A_pointer->_34 * B_pointer->_44;
	result_pointer->_41 = 	A_pointer->_41 * B_pointer->_11 + 
								A_pointer->_42 *	B_pointer->_21 + 
								A_pointer->_43 * B_pointer->_31 + 
								A_pointer->_44 * B_pointer->_41;
	result_pointer->_42 = 	A_pointer->_41 * B_pointer->_12 + 
								A_pointer->_42 *	B_pointer->_22 + 
								A_pointer->_43 * B_pointer->_32 + 
								A_pointer->_44 * B_pointer->_42;
	result_pointer->_43 = 	A_pointer->_41 * B_pointer->_13 + 
								A_pointer->_42 *	B_pointer->_23 + 
								A_pointer->_43 * B_pointer->_33 + 
								A_pointer->_44 * B_pointer->_43;
	result_pointer->_44 = 	A_pointer->_41 * B_pointer->_14 + 
								A_pointer->_42 *	B_pointer->_24 + 
								A_pointer->_43 * B_pointer->_34 + 
								A_pointer->_44 * B_pointer->_44;
	R = result;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
BOOL MatrixARB::CollisionSphereSeg(D3DVECTOR & startPoint,
								   D3DVECTOR & endPoint,
								   float & sphereRadius,
								   D3DVECTOR & center,
								   BOOL getAdjustInfo,
								   D3DVECTOR * adjust,
								   float & objectRadius,
								   D3DVECTOR * intersection)
{
   D3DMATRIX mainMatrix;
   InitMat(mainMatrix.m);
   D3DVECTOR axis;
   axis.x = endPoint.x - startPoint.x;
   axis.y = endPoint.y - startPoint.y;
   axis.z = endPoint.z - startPoint.z;
   if(axis.x == 0.0f && axis.y == 0.0f && axis.z == 0.0f)
	  return FALSE;

   axis = Normalize(axis);
   FaceTowardPoint(axis,&mainMatrix);
   mainMatrix._41 = startPoint.x;
   mainMatrix._42 = startPoint.y;
   mainMatrix._43 = startPoint.z;
   D3DVECTOR inSpacePoint;
   inSpacePoint = GetLocalPositionFromMatrixView(mainMatrix,center);
   float iniTemp = 0.0f;
   float radiusCurrent = MagnitudeSquared(inSpacePoint.x,inSpacePoint.y,iniTemp,
	                                      iniTemp,iniTemp,iniTemp);
   if(radiusCurrent > (sphereRadius+objectRadius))
	  return FALSE;//out of radius
   //check distance
   if(inSpacePoint.z > (sphereRadius+objectRadius))
	  return FALSE;//too far behind-remember missiles fly foward in negative direction

   float distance = MagnitudeSquared(startPoint.x,startPoint.y,startPoint.z,
	                                 endPoint.x,endPoint.y,endPoint.z);

   if((distance + (sphereRadius+objectRadius))<-inSpacePoint.z)
	  return FALSE;

   //if((distance + (sphereRadius+objectRadius))<inSpacePoint.z)
	  //return FALSE;
 
   if(intersection){
   float distanceToTarget = MagnitudeSquared(startPoint.x,startPoint.y,startPoint.z,
	                                         center.x    ,center.y    ,center.z);
   intersection->x = startPoint.x + (axis.x*distanceToTarget);
   intersection->y = startPoint.y + (axis.y*distanceToTarget);
   intersection->z = startPoint.z + (axis.z*distanceToTarget);
   }

   if(getAdjustInfo){//find adjust movement
	  D3DVECTOR pushVect;
      float mDist = MagnitudeSquared(startPoint.x,startPoint.y,startPoint.z,
	                                    center.x,center.y,center.z);
	  pushVect.x = startPoint.x - center.x;
	  pushVect.y = startPoint.y - center.y;
	  pushVect.z = startPoint.z - center.z;
	  if(pushVect.x == 0.0f &&
		 pushVect.y == 0.0f &&
		 pushVect.z == 0.0f){
	     adjust->x = 0.0f;
	     adjust->y = 0.0f;
	     adjust->z = 0.0f;
	  }
	  else{
		pushVect = Normalize(pushVect);
	    mDist = (sphereRadius+objectRadius) - mDist;//
		if(mDist > 0){
		adjust->x = pushVect.x * mDist;
	    adjust->y = pushVect.y * mDist;
	    adjust->z = pushVect.z * mDist;
		}
		else{
        adjust->x = 0.0f;
	    adjust->y = 0.0f;
	    adjust->z = 0.0f;
		}
	  }
   }//end find adjust movement


   return TRUE;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
void MatrixARB::D3DMath_MatrixInvert( D3DMATRIX& q, D3DMATRIX& a )
{
    FLOAT fDetInv = 1.0f / ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
                             a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
                             a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

    q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
    q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
    q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
    q._14 = 0.0f;

    q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
    q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
    q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
    q._24 = 0.0f;

    q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
    q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
    q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
    q._34 = 0.0f;

    q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
    q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
    q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
    q._44 = 1.0f;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
void MatrixARB::D3DMath_MatrixInvertD( AXDXMATRIX& q, AXDXMATRIX& a )
{
    double fDetInv = 1.0 / ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
                              a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
                              a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

    q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
    q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
    q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
    q._14 = 0.0;

    q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
    q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
    q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
    q._24 = 0.0;

    q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
    q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
    q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
    q._34 = 0.0;

    q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
    q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
    q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
    q._44 = 1.0;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
void MatrixARB::D3DMath_MatrixInvert2( D3DMATRIX& q, D3DMATRIX& a )
{
    //first transpose the rotation matrix
	q._11 = a._11;
	q._12 = a._21;
	q._13 = a._31;
	q._21 = a._12;
	q._22 = a._22;
	q._23 = a._32;
	q._31 = a._13;
	q._32 = a._23;
	q._33 = a._33;

	//fix right column
	q._14 = 0;
	q._24 = 0;
	q._34 = 0;
	q._44 = 1;

	//now get the new translation vector
	//D3DVECTOR temp;
	//temp.x = a._41;
	//temp.y = a._42;
	//temp.z = a._43;

	q._41 = -(a._41 * a._11 + a._42 * a._12 + a._43 * a._13);
	q._42 = -(a._41 * a._21 + a._42 * a._22 + a._43 * a._23);
	q._43 = -(a._41 * a._31 + a._42 * a._32 + a._43 * a._33);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between animations BABY!! Imediate mode style-----------//
float MatrixARB::InterpolateFloats(
						 float StartVec, 
						 float EndVec,
					     int NumOfStages, 
					     int ThisStage)
{
 float propfactor;
 if (NumOfStages <= 0)
  NumOfStages = 1;
  propfactor = (float)ThisStage/NumOfStages;
  float StageVec = StartVec + propfactor * (EndVec - StartVec);

 return StageVec;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between animations BABY!! Imediate mode style-----------//
float MatrixARB::InterpolateFloatsHiRes(
						            float StartVec, 
						            float EndVec,
					                float NumOfStages, 
					                float ThisStage)
{
 float propfactor;
 if (NumOfStages <= 0.0f)
  NumOfStages = 1.0f;
  propfactor = ThisStage/NumOfStages;
  float StageVec = StartVec + propfactor * (EndVec - StartVec);

 return StageVec;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//---This Function will determine bounding sphere collision----//
BOOL MatrixARB::BoundingSphereCheck(float radius1,float radius2,float x1,float y1,float z1,
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
//---This Function will determine bounding sphere collision----//
BOOL MatrixARB::BoundingSphereCheckX(float & radius1,float & radius2,
									 float & x1,float & y1,float & z1,
									 float & x2,float & y2,float & z2)
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
//---This Function will determine bounding sphere collision----//
float MatrixARB::GetLocalZpos (D3DMATRIX  invViewMatrix,
						      D3DMATRIX viewMatrix,
							  float posX,
						      float posY,
						      float posZ)
{
    D3DVECTOR zeroedPos;
	zeroedPos.x = posX - viewMatrix._41;
    zeroedPos.y = posY - viewMatrix._42;
    zeroedPos.z = posZ - viewMatrix._43;

	return       (invViewMatrix.m[0][2]*zeroedPos.x)+
	             (invViewMatrix.m[1][2]*zeroedPos.y)+
		         (invViewMatrix.m[2][2]*zeroedPos.z);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
D3DMATRIX MatrixARB::AxisAngleRotationV2( D3DVECTOR axis, float angle )
{
	float s = (float)sin( angle );
	float c = (float)cos( angle );
	float x = axis.x, y = axis.y, z = axis.z;

    D3DMATRIX matrixLocal;

	matrixLocal._11 = x*x*(1-c)+c;
	matrixLocal._21 = x*y*(1-c)-(z*s);
	matrixLocal._31 = x*z*(1-c)+(y*s);
	matrixLocal._41 = 0;
	matrixLocal._12 = y*x*(1-c)+(z*s);
	matrixLocal._22 = y*y*(1-c)+c;
	matrixLocal._32 = y*z*(1-c)-(x*s);
	matrixLocal._42 = 0;
	matrixLocal._13 = z*x*(1-c)-(y*s);
	matrixLocal._23 = z*y*(1-c)+(x*s);
	matrixLocal._33 = z*z*(1-c)+c;
	matrixLocal._43 = 0;
	matrixLocal._14 = 0;
	matrixLocal._24 = 0;
	matrixLocal._34 = 0;
	matrixLocal._44 = 1;

	return matrixLocal;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Poly Collision R2-----------------------------------------------------//
BOOL MatrixARB::SegPolyCheck(D3DVECTOR polygon[3],
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

    m_polySpaceUpVector = Normalize (CrossProduct(m_directionalVect,m_directionalVect2));
	m_directionalVector = Normalize (m_directionalVect);

    SetOrientation(&m_polyspace,m_directionalVector,m_polySpaceUpVector);

  
	m_polyspace._41 = polygon[0].x;
	m_polyspace._42 = polygon[0].y;
	m_polyspace._43 = polygon[0].z;
    D3DMath_MatrixInvert2( m_tempMatGLB, m_polyspace);

    m_localStartPoint = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,startPoint);
    m_localEndPoint   = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,endPoint);

    if(m_localStartPoint.y > 0.0f && m_localEndPoint.y > 0.0f)
	   return FALSE;
    
	if(m_localStartPoint.y <= 0.0f && m_localEndPoint.y <= 0.0f)
	   return FALSE;


	m_localPolygon[0] = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,polygon[0]);
    m_localPolygon[1] = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,polygon[1]);
	m_localPolygon[2] = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,polygon[2]);

    m_segDistance = fabs(m_localStartPoint.y) + fabs(m_localEndPoint.y);//seg distance

	m_tempXYZ                         = InterpolatePointsHiRes(m_localStartPoint, 
						                                       m_localEndPoint,
					                                           m_segDistance, 
					                                           fabs(m_localStartPoint.y));

    if(PointInPolyXZOpt(m_tempXYZ,m_localPolygon))
	{
	   *intersectionPoint = UpdatePolygonVertex(m_polyspace.m,m_tempXYZ);
       return TRUE;
	}


	return FALSE;
}
//--------------Poly Collision R2-----------------------------------------------------//
BOOL MatrixARB::SegPolyCheckInLine( D3DVECTOR & polygonPt1,
								    D3DVECTOR & polygonPt2,
									D3DVECTOR & polygonPt3,
									D3DVECTOR & startPoint,
									D3DVECTOR & endPoint,
									D3DVECTOR *intersectionPoint)
{
	//D3DMATRIX polyspace;
	//InitMat(polyspace.m);
	D3DMATRIX m_polyspace;
	D3DVECTOR m_directionalVect;
	D3DVECTOR m_directionalVect2;
	D3DVECTOR m_polySpaceUpVector;
    D3DVECTOR m_directionalVector;
    float     m_segDistance;
	D3DMATRIX m_tempMatGLB;
	D3DVECTOR m_tempXYZ;
	D3DVECTOR m_localPolygon[3];

	m_polyspace.m[0][3] = 0.0f;
    m_polyspace.m[1][3] = 0.0f;
    m_polyspace.m[2][3] = 0.0f;
    m_polyspace.m[3][3] = 1.0f;

    //D3DVECTOR directionalVect,directionalVect2;
	m_directionalVect.x = polygonPt2.x - polygonPt1.x;
	m_directionalVect.y = polygonPt2.y - polygonPt1.y;
	m_directionalVect.z = polygonPt2.z - polygonPt1.z;

	m_directionalVect2.x = polygonPt3.x - polygonPt1.x;
	m_directionalVect2.y = polygonPt3.y - polygonPt1.y;
	m_directionalVect2.z = polygonPt3.z - polygonPt1.z;

    m_polySpaceUpVector = Normalize (CrossProduct(m_directionalVect,m_directionalVect2));
	m_directionalVector = Normalize (m_directionalVect);

    SetOrientation(&m_polyspace,m_directionalVector,m_polySpaceUpVector);

  
	m_polyspace._41 = polygonPt1.x;
	m_polyspace._42 = polygonPt1.y;
	m_polyspace._43 = polygonPt1.z;
    D3DMath_MatrixInvert2( m_tempMatGLB, m_polyspace);

    D3DVECTOR localStartPoint = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,startPoint);
    D3DVECTOR localEndPoint   = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,endPoint);

    if(localStartPoint.y > 0.0f && localEndPoint.y > 0.0f)
	   return FALSE;
    
	if(localStartPoint.y <= 0.0f && localEndPoint.y <= 0.0f)
	   return FALSE;


	m_localPolygon[0] = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,polygonPt1);
    m_localPolygon[1] = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,polygonPt2);
	m_localPolygon[2] = GetLocalPositionFromMatrixView2(m_tempMatGLB,m_polyspace,polygonPt3);

    m_segDistance = fabs(localStartPoint.y) + fabs(localEndPoint.y);//seg distance

	m_tempXYZ                         = InterpolatePointsHiRes(localStartPoint, 
						                                       localEndPoint,
					                                           m_segDistance, 
					                                           fabs(localStartPoint.y));

    *intersectionPoint = UpdatePolygonVertex(m_polyspace.m,m_tempXYZ);
    
    if(PointInPolyXZOpt(m_tempXYZ,m_localPolygon))
       return TRUE;


	return FALSE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Poly Collision R2-----------------------------------------------------//
BOOL MatrixARB::PointInPolyXZ(D3DVECTOR & point,
							  D3DVECTOR polyGon[3])
{
    int intersectionCount = 0;
	//segment 1   poly points 0 1
	for(int loop=0;loop<3;loop++){//loop
    int positionOne = loop;
	int positionTwo = loop+1;
	if(positionTwo == 3)
	   positionTwo = 0;


	if(TestLevel1(point.z,polyGon[positionOne].z,polyGon[positionTwo].z)){//possible
		if(polyGon[positionOne].x > point.x || polyGon[positionTwo].x > point.x){//possible 2
		   if(polyGon[positionOne].x > point.x && polyGon[positionTwo].x > point.x)  
		      intersectionCount++;//definite intersection
		   else{//aquire greater value and resolve intersection
			   if(polyGon[positionOne].z > polyGon[positionTwo].z){//1
                  float range      = polyGon[positionOne].z - polyGon[positionTwo].z;
                  float position   = polyGon[positionOne].z - point.z;
				  float x          = InterpolateFloatsHiRes(
						                                    polyGon[positionOne].x, 
						                                    polyGon[positionTwo].x,
					                                        range, 
					                                        position);
				  if(point.x < x)
                     intersectionCount++;
			   }//end 1
			   else{//2
                  float range      = polyGon[positionTwo].z - polyGon[positionOne].z;
                  float position   = polyGon[positionTwo].z - point.z;
				  float x          = InterpolateFloatsHiRes(
						                                    polyGon[positionTwo].x, 
						                                    polyGon[positionOne].x,
					                                        range, 
					                                        position);
				  if(point.x < x)
                     intersectionCount++;
			   }//end 2
		   }//aquire greater value and resolve intersection
        }//end possible 2
	}//end possible
	}//end loop

	if(intersectionCount == 1)
	   return TRUE;

return FALSE;
}
//--------------Poly Collision R2-----------------------------------------------------//
BOOL MatrixARB::PointInPolyXZOpt(D3DVECTOR & point,D3DVECTOR polyGon[3])
{
    int i, j, c = 0;
      for (i = 0, j = 2; i < 3; j = i++) 
	  {
        if ((((polyGon[i].z <= point.z) && (point.z < polyGon[j].z)) ||
             ((polyGon[j].z <= point.z) && (point.z < polyGon[i].z))) &&
            (point.x < (polyGon[j].x - polyGon[i].x) * (point.z - polyGon[i].z) / (polyGon[j].z - polyGon[i].z) + polyGon[i].x))
          c = !c;
      }
      return c;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Poly Collision R2-----------------------------------------------------//
BOOL MatrixARB::TestLevel1(float & val,float & h1,float & h2)
{
     if(val < h1 && val > h2)
		return TRUE;

     if(val > h1 && val < h2)
		return TRUE;

     return FALSE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Poly Collision R2-----------------------------------------------------//
BOOL MatrixARB::GetSubDistance(D3DVECTOR polyGon[3],
							   D3DVECTOR uniNormal,
							   D3DVECTOR point,
							   float * distanceToMoveAlongNormal)
{

    D3DMATRIX polyspace;
	InitMat(polyspace.m);

	D3DVECTOR directionalVector;

	directionalVector.x = polyGon[1].x - polyGon[0].x;
	directionalVector.y = polyGon[1].y - polyGon[0].y;
	directionalVector.z = polyGon[1].z - polyGon[0].z;

	D3DVECTOR polySpaceUpVector  = Normalize (uniNormal);
	D3DVECTOR polySpaceDirVector = Normalize (directionalVector);

    SetOrientation(&polyspace,polySpaceDirVector,polySpaceUpVector);

    D3DMATRIX invMatrix;
	polyspace._41 = polyGon[0].x;
	polyspace._42 = polyGon[0].y;
	polyspace._43 = polyGon[0].z;
    D3DMath_MatrixInvert2(invMatrix,polyspace);
 
    D3DVECTOR localPoint = GetLocalPositionFromMatrixView2(invMatrix,polyspace,point);

    *distanceToMoveAlongNormal = fabs(localPoint.y);

    if(localPoint.y >= 0.0f)
	   return FALSE;

return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Poly Collision R2-----------------------------------------------------//
D3DVECTOR MatrixARB::GetAdjustVector(float & distance,
									 D3DVECTOR & uniDirection)
{
   D3DVECTOR vectBuild;
 
   vectBuild.x = uniDirection.x * distance;
   vectBuild.y = uniDirection.y * distance;
   vectBuild.z = uniDirection.z * distance;

   return vectBuild;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Poly Collision R2-----------------------------------------------------//
D3DVECTOR MatrixARB::GetAdjustVector(float & distance,
									 D3DVECTOR & uniDirection,
									 float & factor)
{
   D3DVECTOR vectBuild;
 
   vectBuild.x = uniDirection.x * (distance*factor);
   vectBuild.y = uniDirection.y * (distance*factor);
   vectBuild.z = uniDirection.z * (distance*factor);

   return vectBuild;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
D3DMATRIX MatrixARB::LookAtPoint( D3DVECTOR loc, 
							      D3DVECTOR lookAt, 
							      D3DVECTOR inUp )
{

    D3DVECTOR viewVec;
    viewVec.x = lookAt.x - loc.x;
	viewVec.y = lookAt.y - loc.y;
	viewVec.z = lookAt.z - loc.z;
  

 
    float mag = Magnitude(viewVec);
 
    //viewVec /= mag;
	viewVec.x /= mag;
	viewVec.y /= mag;
	viewVec.z /= mag;
 
    float fDot = DotProduct (inUp,viewVec);
 
    D3DVECTOR upVec;//= inUp - fDot * viewVec;
	upVec.x = inUp.x - fDot * viewVec.x;
	upVec.y = inUp.y - fDot * viewVec.y;
	upVec.z = inUp.z - fDot * viewVec.z;
 
	Normalize(upVec);

    D3DVECTOR rightVec = CrossProduct (upVec,viewVec);

	D3DMATRIX lookatRet;

    //Start building the matrix. The first three rows contains the basis
    //vectors used to rotate the view to point at the lookat point
    lookatRet._11 = rightVec.x;    lookatRet._21 = upVec.x;    lookatRet._31 = viewVec.x;
    lookatRet._12 = rightVec.y;    lookatRet._22 = upVec.y;    lookatRet._32 = viewVec.y;
    lookatRet._13 = rightVec.z;    lookatRet._23 = upVec.z;    lookatRet._33 = viewVec.z;
 
    //Do the translation values (rotations are still about the eyepoint)
    lookatRet._41 = loc.x;
    lookatRet._42 = loc.y;
    lookatRet._43 = loc.z;

    lookatRet._14 = 0;
    lookatRet._24 = 0;
    lookatRet._34 = 0;
    lookatRet._44 = 1;

	return lookatRet;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
D3DMATRIX MatrixARB::GetShadowMeshTransformSpcified(D3DMATRIX currentMatrix,
														   D3DMATRIX & differenceMatrix)
{
	D3DVECTOR centerRef;
	centerRef.x = currentMatrix._41;
	centerRef.y = currentMatrix._42;
	centerRef.z = currentMatrix._43;
	currentMatrix._41 = 0.0f;
	currentMatrix._42 = 0.0f;
	currentMatrix._43 = 0.0f;
    //flatten
	D3DMATRIX scaleit;
	scale ( 1.0f, 0.0f, 1.0f,scaleit.m);
	fastMatrixMultiply(currentMatrix,currentMatrix,scaleit);

    currentMatrix._41 = centerRef.x;
	currentMatrix._42 = centerRef.y;
	currentMatrix._43 = centerRef.z;

	fastMatrixMultiply(currentMatrix,currentMatrix,differenceMatrix);

	return currentMatrix;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 D3DMATRIX MatrixARB::GetShadowMeshTransform(D3DMATRIX currentMatrix,
											       D3DVECTOR & orientationNormal,
											       float offsetFromFloor)
{
	D3DVECTOR centerRef;
	centerRef.x = currentMatrix._41;
	centerRef.y = currentMatrix._42;
	centerRef.z = currentMatrix._43;
	currentMatrix._41 = 0.0f;
	currentMatrix._42 = 0.0f;
	currentMatrix._43 = 0.0f;
    //flatten
	D3DMATRIX scaleit;
	scale ( 1.0f, 0.0f, 1.0f,scaleit.m);
	fastMatrixMultiply(currentMatrix,currentMatrix,scaleit);
	//generatePitchTriange
    D3DVECTOR UpVect,backPoint;
	UpVect.x = 0.0f;
    UpVect.y = orientationNormal.y;
	UpVect.z = 0.0;

	backPoint.x = 0.0f;
    backPoint.y = orientationNormal.y;
	backPoint.z = orientationNormal.z;

    float dist = MagnitudeSquared(backPoint.x,backPoint.y,
							       backPoint.z,UpVect.x,
								   UpVect.y,UpVect.z);

    float turnReq = atan(fabs(dist)/fabs(UpVect.y))* 57.3f;
  
	D3DMATRIX rotationReq;
	if(orientationNormal.z <= 0.0f)
    Rotate ( -turnReq, 0.0f, 0.0f,rotationReq.m);
	else
	Rotate ( turnReq, 0.0f, 0.0f,rotationReq.m);

    fastMatrixMultiply(currentMatrix,currentMatrix,rotationReq);
    //generateRollTriange
	UpVect.x = 0.0f;
    UpVect.y = orientationNormal.y;
	UpVect.z = 0.0;

	backPoint.x = orientationNormal.x;
    backPoint.y = orientationNormal.y;
	backPoint.z = 0.0f;
	
	dist = MagnitudeSquared(backPoint.x,backPoint.y,
									                    backPoint.z,UpVect.x,
									                    UpVect.y,UpVect.z);

	turnReq = atan(fabs(dist)/fabs(UpVect.y))* 57.3f;
	if(orientationNormal.x < 0.0f)
	Rotate ( 0.0f, 0.0f, turnReq,rotationReq.m);
	else
	Rotate ( 0.0f, 0.0f, -turnReq,rotationReq.m);

    fastMatrixMultiply(currentMatrix,currentMatrix,rotationReq);
	
   
	currentMatrix._41 = centerRef.x;
	currentMatrix._42 = centerRef.y + offsetFromFloor;
	currentMatrix._43 = centerRef.z;

	return currentMatrix;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
D3DMATRIX MatrixARB::GetDifferenceMatrix(D3DMATRIX & startMatrix,D3DMATRIX & afterMatrix)
{
	     D3DMATRIX inverseMatrix,differenceMatrix;
	     D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	     fastMatrixMultiply(differenceMatrix,inverseMatrix,afterMatrix);
		 return differenceMatrix;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
D3DXMATRIX MatrixARB::GetDifferenceMatrix2(D3DXMATRIX startMatrix,D3DXMATRIX afterMatrix)
{
	     D3DXMATRIX differenceMatrix;

		 differenceMatrix = startMatrix - afterMatrix;

		 return differenceMatrix;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float MatrixARB::Magnitude (const _D3DVECTOR& v)
{
   float xLoc = SquareMagnitude(v);
   if(xLoc == 0.0f)
      return 0.0f;

   return (float) sqrt(xLoc);
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 float MatrixARB::SquareMagnitude (const _D3DVECTOR& v)
{
   return v.x*v.x + v.y*v.y + v.z*v.z;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float MatrixARB::GetDifferenceAngleY(D3DVECTOR vector1,D3DVECTOR vector2)
{
   if(vector1.z == 0.0f)
      vector1.z = .0001f;

   if(vector2.z == 0.0f)
      vector2.z = .0001f;


   float rotationVect1 = atan(fabs(vector1.x)/fabs(vector1.z))* 57.3f;
   if(vector1.z < 0.0f)
      rotationVect1 = 180.0f-rotationVect1;

   if(vector1.x < 0.0f)
      rotationVect1 = -rotationVect1;

   rotationVect1 = -rotationVect1;

   float rotationVect2 = atan(fabs(vector2.x)/fabs(vector2.z))* 57.3f;

   vector2 = RotateY_WLD ( rotationVect1, vector2);


   rotationVect2 = atan(fabs(vector2.x)/fabs(vector2.z))* 57.3f;
   if(vector2.z < 0.0f)
      rotationVect2 = 180.0f-rotationVect2;

   if(vector2.x < 0.0f)
      rotationVect2 = -rotationVect2;


   return rotationVect2;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
D3DMATRIX MatrixARB::GetAccuracyAdjust(float maxGroupingAngle)
{
    D3DMATRIX matLoc,matLoc2;
    float xRotationFinal = (rand()%((int)(maxGroupingAngle*1000.0f)))*.001f;
    float zRotationFinal = (rand()%((int)(360.0f*100.0f)))*.01f;
   
    Rotate ( xRotationFinal, 0.0f, 0.0f, matLoc.m);
	Rotate ( 0.0f, 0.0f, zRotationFinal, matLoc2.m);
    
	fastMatrixMultiply(matLoc,matLoc,matLoc2);
	
	return matLoc;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float MatrixARB::GetAngleBetweenTwoVectorsRadian(D3DVECTOR vOne,D3DVECTOR vTwo)
{
    vOne = Normalize(vOne);
	vTwo = Normalize(vTwo);
	return fabs(acos(DotProduct(vOne,vTwo)));
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float MatrixARB::GetAngleBetweenTwoVectorsRadian(D3DMATRIX & pivot,D3DVECTOR vOne,D3DVECTOR vTwo)
{
    vOne.x = vOne.x - pivot._41;
	vOne.y = vOne.y - pivot._42;
	vOne.z = vOne.z - pivot._43;

	vTwo.x = vTwo.x - pivot._41;
	vTwo.y = vTwo.y - pivot._42;
	vTwo.z = vTwo.z - pivot._43;


    vOne = Normalize(vOne);
	vTwo = Normalize(vTwo);
	return fabs(acos(DotProduct(vOne,vTwo)));
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float MatrixARB::GetAngleBetweenTwoVectors(D3DVECTOR vOne,D3DVECTOR vTwo)
{
	vOne = Normalize(vOne);
	vTwo = Normalize(vTwo);
	return fabs(acos(DotProduct(vOne,vTwo)) * 57.3f);
}
//-----------------------------------------------------------------------------//
float MatrixARB::GetAngleBetweenTwoVectorsFast(D3DVECTOR & vOne,D3DVECTOR & vTwo)
{

	return fabs(acos(DotProduct(vOne,vTwo)) * 57.3f);
}
//-----------------------------------------------------------------------------
HRESULT MatrixARB::SetProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,FLOAT fNearPlane, FLOAT fFarPlane,float hieghtBase )
{
 
	if(fabs(fFarPlane - fNearPlane) < 0.01f)
        return E_INVALIDARG;
    if(fabs(sin(fFOV / 2)) < 0.01f)
        return E_INVALIDARG;

    FLOAT w = fAspect * ( cosf(fFOV/2)/sinf(fFOV/2) );
    FLOAT h = hieghtBase  * ( cosf(fFOV/2)/sinf(fFOV/2) );
    FLOAT Q = fFarPlane / ( fFarPlane - fNearPlane );

    ZeroMemory( &mat, sizeof(D3DMATRIX) );
    mat._11 = w;
    mat._22 = h;
    mat._33 = Q;
    mat._34 = 1.0f;
    mat._43 = -Q*fNearPlane;

    return S_OK;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------delete display Unit--------------------------------------------//
BOOL MatrixARB::SetViewMatrixAB( D3DMATRIX& mat, D3DXVECTOR3& vFrom, D3DXVECTOR3& vAt, D3DXVECTOR3& vWorldUp )
{
    D3DVECTOR vView = vAt;//- vFrom;
    //Get the dot product, and calculate the projection of the z basis
    //vector onto the up vector. The projection is the y basis vector.
    FLOAT fDotProduct = DotProduct( vWorldUp, vView );
    D3DXVECTOR3 vUp = vWorldUp - fDotProduct * vView;
    D3DXVECTOR3 vRight = CrossProduct( vUp, vView );

    //Start building the matrix. The first three rows contains the basis
    //vectors used to rotate the view to point at the lookat point
    InitMat(mat.m);
    mat._11 = vRight.x;    mat._12 = vUp.x;    mat._13 = vView.x;
    mat._21 = vRight.y;    mat._22 = vUp.y;    mat._23 = vView.y;
    mat._31 = vRight.z;    mat._32 = vUp.z;    mat._33 = vView.z;

    //Do the translation values (rotations are still about the eyepoint)
    mat._41 = - DotProduct( vFrom, vRight );
    mat._42 = - DotProduct( vFrom, vUp );
    mat._43 = - DotProduct( vFrom, vView );
return TRUE;
}
//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------//
BOOL MatrixARB::SetViewMatrixAB( D3DMATRIX& mat, D3DVECTOR& vAt, D3DVECTOR& vWorldUp )
{
 
    D3DVECTOR vView = vAt;//- vFrom;
    //Get the dot product, and calculate the projection of the z basis
    //vector onto the up vector. The projection is the y basis vector.
    FLOAT fDotProduct = DotProduct( vWorldUp, vView );
    D3DVECTOR vUp;
	vUp.x = vWorldUp.x - fDotProduct * vView.x;
	vUp.y = vWorldUp.y - fDotProduct * vView.y;
	vUp.z = vWorldUp.z - fDotProduct * vView.z;
    D3DVECTOR vRight = CrossProduct( vUp, vView );

    //Start building the matrix. The first three rows contains the basis
    //vectors used to rotate the view to point at the lookat point
    InitMat(mat.m);
    mat._11 = vRight.x;    mat._12 = vUp.x;    mat._13 = vView.x;
    mat._21 = vRight.y;    mat._22 = vUp.y;    mat._23 = vView.y;
    mat._31 = vRight.z;    mat._32 = vUp.z;    mat._33 = vView.z;

    //Do the translation values (rotations are still about the eyepoint)
    mat._41 = 0.0f;//- m_matLib.DotProduct( 0.0f, vRight );
    mat._42 = 0.0f;//- m_matLib.DotProduct( vFrom, vUp );
    mat._43 = 0.0f;//- m_matLib.DotProduct( vFrom, vView );
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL MatrixARB::GetViewMatrixAB( D3DMATRIX& viewMat, D3DMATRIX& cameraMat )
{
	        D3DVECTOR upVect;
			upVect.x = 0.0;
			upVect.y = 1.0;
			upVect.z = 0.0;
			D3DVECTOR dirVect;
			dirVect.x = 0.0;
			dirVect.y = 0.0;
			dirVect.z = 1.0;
			upVect = UpdatePolygonVertexRotationOnly(cameraMat.m,upVect);
			dirVect = UpdatePolygonVertexRotationOnly(cameraMat.m,dirVect);
			SetViewMatrixAB(viewMat,dirVect,upVect);
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------delete display Unit--------------------------------------------//
BOOL MatrixARB::BoundingBoxPointCheck(D3DVECTOR & point,
											 D3DVECTOR & min,
										     D3DVECTOR & max)
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
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------delete display Unit--------------------------------------------//
BOOL MatrixARB::BoundingBoxPointCheck(float & pointX,
									  float & pointY,
									  float & pointZ,
									  float & maxX,
									  float & maxY,
									  float & maxZ,
									  float & minX,
									  float & minY,
									  float & minZ)
{
 if(pointX >= minX &&
    pointX <= maxX &&
    pointY >= minY &&
    pointY <= maxY &&
    pointZ >= minZ &&
    pointZ <= maxZ)
  return TRUE;
 return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL MatrixARB::BoundingBoxToBoxCheck(float & b1maxX,
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
									  float & b2minZ)
{
 if(SegStraightOverlap(b1minX,b1maxX,b2minX,b2maxX)==FALSE)
  return FALSE;
 if(SegStraightOverlap(b1minY,b1maxY,b2minY,b2maxY)==FALSE)
  return FALSE;
 if(SegStraightOverlap(b1minZ,b1maxZ,b2minZ,b2maxZ)==FALSE)
  return FALSE;

return TRUE;//is collided
}
//------segment check------------------------------------------//
BOOL MatrixARB::SegStraightOverlap(    float & segPt1min,
										   float & segPt1max,
										   float & segPt2min,
										   float & segPt2max)
{
 if(segPt1min >= segPt2min && segPt1min <= segPt2max)
  return TRUE;
 if(segPt1max >= segPt2min && segPt1max <= segPt2max)
  return TRUE;
 if(segPt1min <= segPt2min && segPt1max >= segPt2max)
  return TRUE;

 return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL MatrixARB::Generate2Dto3DVector(float & screenX, 
									 float & screenY, 
									 float & screenZ,
									 float Zdist, //ray defined length
									 D3DVECTOR & startPos, 
									 D3DVECTOR & endPos, 
									 D3DMATRIX & cameraMatrix,
									 float & fov,
									 float projectionCursorFov,
									 float & multiplierRamp)
{
    //note: the longer zdist is the more calculational this will be
	//generate start point
	ZeroMemory(&startPos,sizeof(D3DVECTOR));
	ZeroMemory(&endPos,sizeof(D3DVECTOR));
	
    //recalibrate if fov has changed
    
	/*float additionalMultiplierRamp = InterpolateFloatsHiRes(1.0f, 1.025f,.3f, fov-.3f);
	if(fov >= .6)
	   additionalMultiplierRamp = InterpolateFloatsHiRes(1.025f, 1.0675f,.6f, fov-.6f);//1.025f;//correct .0425 to .9 from .6
	else if(fov >= .9)
	   additionalMultiplierRamp = InterpolateFloatsHiRes(1.0675f, 1.13f,.9f, fov-.9f);//1.0675f;//correct .0625 to 1.2 from .9
	else if(fov >= 1.2)
	   additionalMultiplierRamp = InterpolateFloatsHiRes(1.13f, 1.23f,1.2f, fov-1.2f);//1.13f;//correct .1 to 1.5 from 1.2 
    else if(fov >= 1.5)
	   additionalMultiplierRamp = 1.23f;//1.23f;//correct */ 
	
	
	
	float multiplier = (fov / projectionCursorFov)*multiplierRamp;

	//adjust somehow based on view ratio
	//menu cursor is .3 at 6.8 ratio
	//screenX
	//screenY
    
    //float angleX = (atan(screenX / screenZ) * 57.3f) * multiplier;//convert to 360 based angle
    //float angleY = (atan(screenY / screenZ) * 57.3f) * multiplier;

	/*float angleX = atan(screenX / screenZ)* multiplier;//keep it in radians
    float angleY = -(atan(screenY / screenZ)* multiplier);
    
	D3DMATRIX applicationMat;
    D3DMATRIX curPosMat;
	InitMat(curPosMat.m);
	RotateYri(angleX,applicationMat.m);
	fastMatrixMultiply(curPosMat,applicationMat,curPosMat);
	RotateXri(angleY,applicationMat.m);
	fastMatrixMultiply(curPosMat,applicationMat,curPosMat);
	//endPos = UpdatePolygonVertex(rotateApplication.m,endPos);
	Translate(0.0,0.0,Zdist,applicationMat.m);
	fastMatrixMultiply(curPosMat,applicationMat,curPosMat);
	endPos.x = curPosMat._41;
	endPos.y = curPosMat._42;
	endPos.z = curPosMat._43;
	//endPos = UpdatePolygonVertex(rotateApplication.m,endPos);
	

	/*RotateY(angleX,rotateApplication.m);
	endPos = UpdatePolygonVertex(rotateApplication.m,endPos);
	RotateX(angleY,rotateApplication.m);
	endPos = UpdatePolygonVertex(rotateApplication.m,endPos);*/

	//transform out to worls coordinates
	//startPos = UpdatePolygonVertex(cameraMatrix.m,startPos);
	//vector basis
	endPos.x = (screenX* multiplier);
	endPos.y = (screenY* multiplier);
	endPos.z = screenZ;
	endPos = Normalize(endPos);
	endPos.x = endPos.x * Zdist;
	endPos.y = endPos.y * Zdist;
	endPos.z = endPos.z * Zdist;
	
	startPos.x = cameraMatrix._41;
	startPos.y = cameraMatrix._42;
	startPos.z = cameraMatrix._43;
	endPos = UpdatePolygonVertex(cameraMatrix.m,endPos);

	return TRUE;
}

//------------------------------------------------------------------------------------//
BOOL MatrixARB::GenerateReflectiveCameraMatrix(D3DVECTOR & mirrorCenter, 
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
											   float & distClipPlane)
{
	D3DMATRIX returnReflectMatrix = cameraMatrix;
	D3DVECTOR cNormal,cp1,cp2_axis,upVector,camDir;//, centerV,;
    
	upVector.x = 0.0f;
	upVector.y = 1.0f;
	upVector.z = 0.0f;
	upVector = UpdatePolygonVertexRotationOnly(objectMatrix.m,upVector);

	//create refective cam matrix
	cNormal.x = cameraMatrix._41 - mirrorCenter.x;
	cNormal.y = cameraMatrix._42 - mirrorCenter.y;
	cNormal.z = cameraMatrix._43 - mirrorCenter.z;
	cNormal = Normalize(cNormal);

    if(GetAngleBetweenTwoVectors(cNormal,mirrorNormal) <= 90.0f)
	   return FALSE;

	//end behind mirror omit
	
	cp1 = Normalize (CrossProduct(mirrorNormal,cNormal));//setup for axis
	cp2_axis = Normalize (CrossProduct(mirrorNormal,cp1));//rotation axis
	
	//localize current cam materix for axis rotation
	returnReflectMatrix._41 -= mirrorCenter.x;
	returnReflectMatrix._42 -= mirrorCenter.y;
	returnReflectMatrix._43 -= mirrorCenter.z;
	//rotate matrix on generated axis
	D3DMATRIX rotationMatrix = AxisAngleRotationV2( cp2_axis, 180.0f/57.3f );
	fastMatrixMultiply(returnReflectMatrix,returnReflectMatrix,rotationMatrix);
	
	//move back to wld
    returnReflectMatrix._41 += mirrorCenter.x;
	returnReflectMatrix._42 += mirrorCenter.y;
	returnReflectMatrix._43 += mirrorCenter.z;

    //get optimal fov
    if(fovMatrix)
	   *fovMatrix = returnReflectMatrix;


	D3DVECTOR dirVect;
	dirVect.x = -mirrorNormal.x;
	dirVect.y = -mirrorNormal.y;
	dirVect.z = -mirrorNormal.z;
	
	SetOrientation(&returnReflectMatrix,
				   dirVect,
				   upVector);

    //caclulate projection matrix vars
	float vwW,vwWN,vwH,vwHN,fclip,bclip;

	fclip = 1000000.0f;

	D3DMath_MatrixInvert(invReflectiveMatrix,returnReflectMatrix);
    D3DVECTOR temp;
    //create view matrix
	D3DMATRIX viewMatrix;
	D3DVECTOR cameraPos,reflectPos;
	cameraPos.x = cameraMatrix._41;
	cameraPos.y = cameraMatrix._42;
	cameraPos.z = cameraMatrix._43;
	reflectPos.x = returnReflectMatrix._41;
	reflectPos.y = returnReflectMatrix._42;
	reflectPos.z = returnReflectMatrix._43;
	
	D3DXMatrixLookAtLH(	(D3DXMATRIX *)&viewMatrix,
						(D3DXVECTOR3 *)&reflectPos,
						(D3DXVECTOR3 *)&cameraPos,
						(D3DXVECTOR3 *)&upVector
						);

	temp = UpdatePolygonVertex(viewMatrix.m,widthPointN);
	vwW = temp.x;//width of projection matrix
    temp = UpdatePolygonVertex(viewMatrix.m,heightPoint);
	vwH = temp.y;//height of projection matrix

	temp = UpdatePolygonVertex(viewMatrix.m,widthPoint);
	vwWN = temp.x;//width of projection matrix
    temp = UpdatePolygonVertex(viewMatrix.m,heightPointN);
	vwHN = temp.y;//height of projection matrix

	temp = GetLocalPositionFromMatrixView2(invReflectiveMatrix,returnReflectMatrix,widthPoint);
	fclip = fabs(temp.z);
	
	ZeroMemory( &perspectiveMatRet, sizeof(D3DMATRIX) );
	D3DXMatrixPerspectiveOffCenterLH( (D3DXMATRIX*)&perspectiveMatRet,vwWN,vwW,vwHN,vwH,fclip,distClipPlane);

	*reflectiveMatrix = returnReflectMatrix;//copy over to return mem

	return TRUE;
}
//------------------------------------------------------------------------------------//
void MatrixARB::MatrixToPLaneZ(D3DMATRIX & matrix,float plane[4])
{
     
	D3DVECTOR normal;
	ZeroMemory(&normal,sizeof(D3DVECTOR));
    normal.z = 1.0f;

    normal = UpdatePolygonVertexRotationOnly(matrix.m,normal);//reorientates plane normal
	//get offset from zero
	D3DVECTOR zeroPos,viewBasedPosition;
	ZeroMemory(&zeroPos,sizeof(D3DVECTOR));
	viewBasedPosition = GetLocalPositionFromMatrixView(matrix,zeroPos);

	plane[0] = normal.x;
	plane[1] = normal.y;
	plane[2] = normal.z;
	plane[3] = viewBasedPosition.z;
}
//------------------------------------------------------------------------------------//
void MatrixARB::SegmentToBox(D3DVECTOR & start, D3DVECTOR & end, D3DVECTOR & max, D3DVECTOR & min)
{
   if(start.x > end.x)
   {
      max.x = start.x;
      min.x = end.x;
   }
   else
   {
      max.x = end.x;
      min.x = start.x;
   }

   if(start.y > end.y)
   {
      max.y = start.y;
      min.y = end.y;
   }
   else
   {
      max.y = end.y;
      min.y = start.y;
   }

   if(start.z > end.z)
   {
      max.z = start.z;
      min.z = end.z;
   }
   else
   {
      max.z = end.z;
      min.z = start.z;
   }


}
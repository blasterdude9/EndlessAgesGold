
#include "CStructuresMisl.h"
#include "d3d9.h"
#include "d3dx9.h"
#include "matrixarb.h"
#pragma once

class CFrustumObject
{
	public:
		CFrustumObject()
		{
			m_right			= 0;
			m_left			= 1;
			m_bottom		= 2;
			m_top			= 3;
			m_back			= 4;
			m_front			= 5;
			m_a				= 0;
			m_b				= 1;
			m_c				= 2;
			m_d				= 3;
			m_bypass        = false;

			ZeroMemory(m_modelView,16*sizeof(float));
			ZeroMemory(m_projection,16*sizeof(float));
		}

	MatrixARB m_matLib;
	bool m_bypass;

    float m_modelView[16];
    float m_projection[16];
    float m_Frustum[6][4];

    int m_right;	//The RIGHT side of the frustum
	int m_left;		//The LEFT   side of the frustum
	int m_bottom;	//The BOTTOM side of the frustum
	int m_top;		//The TOP side of the frustum
	int m_back;		//The RIGHT side of the frustum
	int m_front;	//The FRONT side of the frustum
	int m_a;		//The X value of the plane's normal
	int m_b;        //The Y value of the plane's normal
	int m_c;		//The Z value of the plane's normal
	int m_d;		//The distance the plane is from the origin
                    

	//Functions
    void NormalizePlane(float frustum[6][4], int side);
	void CalculateFrustum(D3DMATRIX proj, D3DMATRIX modl);
	BOOL CubeInFrustum( float x, float y, float z, float size );
	BOOL SphereInFrustum( float x, float y, float z, float radius );
	BOOL BoundingBoxOutOfFrustum( D3DVECTOR & min, D3DVECTOR & max );
	BOOL BoundingBoxOutOfFrustum( D3DXVECTOR3 & min, D3DXVECTOR3 & max );
	BOOL BoundingBoxInView( ABBOX & cl_box );
};
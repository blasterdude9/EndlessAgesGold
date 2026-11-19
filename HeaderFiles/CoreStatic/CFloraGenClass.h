#include "CEXMeshClass.h"
#include "matrixarb.h"
#ifndef CFLORAGEN_H
#define CFLORAGEN_H



class  CFloraObj : public CObject
{
	
    
public:
	CFloraObj()
	{
       m_trunkRadius1 = 0.5f;
	   m_trunkRadius2 = 0.2f;
	   m_TrunkHieght  = 15.0f;
	   m_trunkVSegments = 9;
	   m_trunkHSegments = 12;
	   m_rootCount = 3;
	   m_rootLength = 1.0f;
	   m_rootHeight = .2f;
	   m_trunkNoise = 0.4f;//0 -= none
	   //m_trunkTaper = .
	   m_meshObj = NULL;
	};
    MatrixARB                  GL_matrixFunctions;

	CEXMeshObj * m_meshObj;
	
	//stem/trunk
	float m_trunkNoise;
	float m_trunkRadius1;
	float m_trunkRadius2;
	//float m_trunkTaper;
	float m_TrunkHieght;
	int   m_trunkVSegments;
    int   m_trunkHSegments;
	int   m_rootCount;
	float m_rootLength;
	float m_rootHeight;

	//functions
	void SafeDelete();
	CEXMeshObj * GenerateMesh();
	
};


#endif
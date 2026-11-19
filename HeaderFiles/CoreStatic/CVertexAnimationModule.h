#include "matrixarb.h"
#include "CStructuresMisl.h"
#include "CAdvancedVertexClass.h"
#ifndef CVERTEXANIMMODULECLASS_H
#define CVERTEXANIMMODULECLASS_H

struct VertexPositionsFrame
{
   ABVERTEX0L                *m_vertexs;
   int                        m_vertexCount;
};

class CVertexAnimationModule : public CObject
{
	
     
	DECLARE_SERIAL( CVertexAnimationModule );
	
    
public:
    CVertexAnimationModule();
    MatrixARB                  GL_matrixFunctions;
	
	int                        m_frameCount;
    VertexPositionsFrame      *m_vertexFrames;
	int                        m_curFrame;
	int                        m_targetFrame;
	long                       m_timeBetweenFrames;
	BOOL                       m_looping;
	long                       m_lastUpdatedFrameChngTime;
	long                       m_currentFrameTime;


	void Serialize( CArchive& ar );
	void Clone(CVertexAnimationModule ** clone);
	void AddKeyFrame(CAdvancedVertexObj * vertexData);
	void ApplyAnimation(CAdvancedVertexObj * vertexData,LPDIRECT3DDEVICE9 pd3dDevice);
	void SafeDelete();
	
};

class CVertexAnimationModuleList : public CObList
{
public:
   CVertexAnimationModuleList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CVertexAnimationModuleList );
	
    
};

#endif
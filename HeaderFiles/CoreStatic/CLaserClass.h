#include "matrixarb.h"
#include "CEXMeshClass.h"


#ifndef CLASERCLASS_H
#define CLASERCLASS_H

class  CLaserObj : public CObject
{
	
     
	DECLARE_SERIAL( CLaserObj );
	
    
public:
    CLaserObj();
    void Clone(CLaserObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);

    MatrixARB                  GL_matrixFunctions;
	
    float                      m_speed;
	float                      m_currentLength;
	float                      m_currentWidth;
	BOOL                       m_lockDown;
	long                       m_millisecondsOfFade;
	BOOL                       m_fadeOut;
	BOOL                       m_terminated;//used for lockdown lasers

	CEXMeshObj                *m_meshObject;

	D3DMATRIX                  m_orientationAndOrigin;
	D3DMATRIX                  m_fireFromLocation;

	void Serialize( CArchive& ar );
	void SafeDelete();
	BOOL UpdateLazerMesh(D3DMATRIX cameraMatrix);
	void InitializeMesh(LPDIRECT3DDEVICE9 g_pd3dDevice);
	
};

class  CLaserObjList : public CObList
{
public:
   CLaserObjList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CLaserObjList );
	
    
};

#endif
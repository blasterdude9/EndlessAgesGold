#include "matrixarb.h"
#include "CEXMeshClass.h"
#include "CStateManagementClass.h"
#include "CSSoundRange.h"
#ifndef CLIGHTNINGEFFECTCLASS_H
#define CLIGHTNINGEFFECTCLASS_H


class  CLightningObj : public CObject
{
public:
    CLightningObj();
    
	CEXMeshObj                 *m_lightningMesh;

	D3DVECTOR                   m_wldPos;
    float                       m_width;
    float                       m_depth;
	long                        m_duration;
	long                        m_timeStamp;
	BOOL                        m_enviormentMode;
	
	


	void SafeDelete();
};

class  CLightningObjList : public CObList
{
public:
   CLightningObjList(){};
   MatrixARB                  GL_matrixFunctions;
                      
    void AddLightningObject(D3DVECTOR & spawnPosition,
						          float     width,
						          float     depth,
						          int textureIndex,
						          long duration,
						          BOOL enviormentModeOn
						          );

    void RenderSystem(D3DVECTOR           & cameraPosition,
				            LPDIRECT3DDEVICE9     g_pd3dDevice7,
					        D3DVECTOR           & vOffset,
					        CStateManagementObj * stateSystem,
					        CTextureDBObjList   * textureDatabase);
   
   void SafeDelete();
};

#endif
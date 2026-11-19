#include "CEXMeshClass.h"
#include "CStateManagementClass.h"
#ifndef CSCREENFLASH_H
#define CSCREENFLASH_H

class   CScreenFlashObj : public CObject
{
public:
    CScreenFlashObj();
    

    CEXMeshObj * m_visual;	
	BOOL         m_on;
	DWORD        m_duration;
	DWORD        m_stamp;
	

	void         SafeDelete();
	void         DoIt(DWORD duration,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void         Callback(DWORD currentTime,
						  LPDIRECT3DDEVICE9 g_pd3dDevice,
						  CStateManagementObj * m_stateManager,
						  CTextureDBObjList * TextureDataBase,
						  D3DMATRIX & camMatrix);
    void         Render(LPDIRECT3DDEVICE9 g_pd3dDevice,
					    CStateManagementObj * m_stateManager,
					    CTextureDBObjList * TextureDataBase,
						D3DMATRIX camMatrix);


};

#endif
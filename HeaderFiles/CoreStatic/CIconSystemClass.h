#include "CTextureDBClass.h"
#include "CStateManagementClass.h"
#include "CEXMeshClass.h"
#ifndef CICONSYSTEMCLASS_H
#define CICONSYSTEMCLASS_H

class CIconSysObj : public CObject
{
	
     
	DECLARE_SERIAL( CIconSysObj );
	
    
public:
    CIconSysObj();
    
	CString              m_iconName;
	CAdvancedVertexObj * m_vertexObject;
	CMaterialObject    * m_materialObj;
	float                m_gridDensity;
	float                m_xIndex;
	float                m_yIndex;
	float                m_size;
	int                  m_textureIndex;
	CString              m_textureName;
	int                  m_controlIDRef;
	int                  m_controlContentIDRef;
	D3DVECTOR            m_refPosition;
	BOOL                 m_refreshed;
	int                  m_indexRef;
	BOOL				 m_tempIcon;
	
  
	void Serialize( CArchive& ar );
	void SafeDelete();
	void Initialize(D3DVECTOR position,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Render(LPDIRECT3DDEVICE9 g_pd3dDevice,
			    CStateManagementObj * stateManager,
			    CTextureDBObjList * textureDatabase);
	void Clone(CIconSysObj ** clone);
};

class CIconSysList : public CObList
{
public:
   CIconSysList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CIconSysList );
	
    

	CIconSysObj * GetByIndex(int index);
	BOOL AddIconFromDBtoThisDB(CIconSysList * primaryDB,
							   int index,
							   D3DVECTOR screenPos,
							   LPDIRECT3DDEVICE9 g_pd3dDevice);
	void RenderAll(LPDIRECT3DDEVICE9 g_pd3dDevice,
						     CStateManagementObj * stateManager,
						     CTextureDBObjList * textureDatabase);
	void Initialize(D3DVECTOR position,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL AddIconFromDBtoThisDBWspecs(CIconSysList * primaryDB,
										       int index,
										       D3DVECTOR screenPos,
										       LPDIRECT3DDEVICE9 g_pd3dDevice,
											   int controlIDRef,
                                               int controlContentIDRef,
											   float scale,
											   BOOL temp = FALSE);
	void ClearTempIcons();
	BOOL TagAllRefreshed(LPDIRECT3DDEVICE9 g_pd3dDevice,
							       int controlIDRef,
								   BOOL refreshState);
	BOOL RemoveAllNonRefreshed(LPDIRECT3DDEVICE9 g_pd3dDevice,
							             int controlIDRef);
	CIconSysObj * GetIconByCriteria(LPDIRECT3DDEVICE9 g_pd3dDevice,
							                          int controlIDRef,
                                                      int controlContentIDRef,
											          D3DVECTOR position,
													  int indexRef);

};

#endif
#include "CEXMeshClass.h"
#include "CMaterialClass.h"
#include "CStateManagementClass.h"
#ifndef CMENUBARCLASS_H
#define CMENUBARCLASS_H

class CMenuBarObj : public CObject
{
	
     
	DECLARE_SERIAL( CMenuBarObj );
	
    
public:
    CMenuBarObj();
    void Clone(CMenuBarObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);

	BOOL             m_verticleStyle;
	BOOL             m_flip;//left right up down option of flow
	float            m_max;
	float            m_min;
	float            m_barLength;
	float            m_barWidth;
	float            m_xRoot;
	float            m_yRoot;
	float            m_cur;//percentage based 0.0f - 1.0f
	int              m_visualReference;//mainly for text object reference
	int              m_barUseType;//0= energy 1=jet pack fuel

	CEXMeshObj      * m_barDynamic;
    CEXMeshObj      * m_barStatic;
  
	void SetStateByDirectValue(float cur);
	void SetStateByTwoAmounts(float cur,float max);
	void Initialize(LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Serialize( CArchive& ar );
	void Render(LPDIRECT3DDEVICE9 g_pd3dDevice,CStateManagementObj * m_stateManager,CTextureDBObjList * TextureDataBase,float gl_adjust);
	void SafeDelete();
	void SetSizePosPorperties(float length, float width, float xPos, float yPos);
	void SetBackColor(float r, float g, float b, float a);
	void SetForeColor(float r, float g, float b, float a);
};

class CMenuBarObjList : public CObList
{
public:
	CMenuBarObjList(){};
    void SafeDelete();
    
     
	DECLARE_SERIAL( CMenuBarObjList );
	
    
};

#endif
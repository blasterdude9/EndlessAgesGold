#pragma once
#include "d3d9.h"
#include "d3dx9.h"
#include "dxdeviceclass.h"
#include "CMaterialClass.h"
#include "CStateManagementClass.h"
#include "CTextureDBClass.h"

class  CPostProcessModule : public CObject
{

	DECLARE_SERIAL( CPostProcessModule );
public:
	CPostProcessModule(void);
	~CPostProcessModule(void);


	//Vars
	CString                 m_processName;
    CString					m_textureAssignName[25];
    int						m_assignIndex[25];//-5 = screen render target -1 = none >-1 = texture from txb
    CMaterialObject		  * m_material;
	


    //Functions
	void Serialize(CArchive &ar);

};

class  CPostProcessModuleManager : public CObList
{
	DECLARE_SERIAL( CPostProcessModuleManager );
    public:
	CPostProcessModuleManager();
	~CPostProcessModuleManager(void);

	//Vars
	D3DMATRIX               m_matrixInit2;
	IDirect3DSurface9     * m_primaryRenderTarget;
	CPostProcessModule    * m_currentEffect;
	int                     m_indexOfCurrentEffect;
	CMaterialObject		  * m_material;
	

	void SetEffect(int effectIndex);
	bool EnableRenderTarget(DxDeviceClass * rnaxDevice);
    void RenderPostEffectToView(CTextureDBObjList   * textureDataBase,
								DxDeviceClass       * rnaxDevice,
								CStateManagementObj * renderStateObject);
	void RestorePrimaryRenderTarget(DxDeviceClass * rnaxDevice);
	void GetPrimaryRenderTarget(DxDeviceClass * rnaxDevice);
	CPostProcessModule * GetByIndex(int indexID);


};

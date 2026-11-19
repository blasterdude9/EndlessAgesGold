	#ifndef CHALOCLASS_H
#define CHALOCLASS_H

#include "CEXMeshClass.h"
#include "CTextureDBClass.h"

struct HALOTYPE : public CObject
	{
	CEXMeshObj *mesh;
	CString fileName;
	float growthRate;
	float maxSize;
	CString haloName;
	CString textureFileName;
	CTextureDBObj *texture;
	int textureIndex;
	};

struct HALO : public CObject
	{
	int haloType;
	D3DVECTOR position;
	D3DVECTOR camPosition;
	};

struct LENS : public CObject
	{
	int textureIndex;
	CString textureFileName;
	CTextureDBObj *texture;
	float distAlongAxis;
	float scale;
	};

struct LENSFLARETYPE : public CObject
	{
	CObList lensList;
	CEXMeshObj *mesh;
	CString lensFlareName;
	CString fileName;
	D3DVECTOR focalPoint;
	};

struct LENSFLARE : public CObject
	{
	int lensFlareType;
	D3DVECTOR position;
	D3DVECTOR camPosition;
	};

class AFX_EXT_CLASS CHaloClass : public CObject
{
	
     
	DECLARE_SERIAL_EXTDLL( CHaloClass );
	
    

	public:
	CHaloClass();
	~CHaloClass();
	void SafeDelete();

	void Serialize( CArchive& ar );

	int FindTextureAndLoad
		(
		CString texName, 
		CTextureDBObjList *texList,
		LPDIRECT3D7 g_pD3D7,
		LPDIRECT3DDEVICE7 pd3dDevice
		);

	void InitMeshes
		(
		LPDIRECT3D7 g_pD3D,
		LPDIRECT3DDEVICE7 pd3dDevice,
		CTextureDBObjList *texList
		);

	CEXMeshObj *LoadMeshObject(CString fileName, LPDIRECT3D7 g_pD3D, CABFunctionLib *CABLIB);
	//------------------------
	//list maintenance
	//------------------------
	void AddHalo(int haloType, D3DVECTOR *haloPos);
	void AddHaloType
		(
		CTextureDBObj *texture,
		CString textureName,
		CString haloName,
		CString filename,
		CEXMeshObj *mesh,
		int textureIndex,
		float growthRate,
		float maxSize
		);
	void DeleteHalo(int index);
	
	void AddLensFlare( int lensFlareType, D3DVECTOR *lensFlarePos );
	void AddLensFlareType
		(
		CString lensFlareName,
		CString fileName, 
		CEXMeshObj *mesh
		);
	void DeleteLensFlare( int index );
	
	void AddLens
		(
		int flareIndex, 
		int textureIndex,
		float growthRate,
		float axisDistance
		);

	void DeleteLens( int flareIndex, int lensIndex );

	CObList m_haloTypeList;
	CObList m_haloRenderList;
	
	CObList m_lensFlareList;
	CObList m_lensFlareRenderList;
	
	BOOL m_renderHalos;
	BOOL m_renderLensFlares;
	
	//------------------------
	//core functionality
	//------------------------
	void Render(LPDIRECT3DDEVICE7 pd3dDevice7, D3DMATRIX camMatrix, float divFactor);
	void RenderHalos(LPDIRECT3DDEVICE7 pd3dDevice7, float divFactor);
	void RenderLensFlares(LPDIRECT3DDEVICE7 pd3dDevice7);
    
	//------------------------
	//miscellany
	//------------------------
	MatrixARB m_abMath;
	D3DMATRIX m_identityMat;

	int m_texturestatechanges;

};

#endif
#include "matrixarb.h"
#include "CAdvancedVertexClass.h"
#include "CabFunctionLib.h"
#include "CStructuresMisl.h"
#include "CMaterialClass.h"
#include "CCharonaClass.h"
#include "CGlobalMaterialShaderLib.h"
#include "afxext.h"


#ifndef CEXMESHCLASS_H
#define CEXMESHCLASS_H

//TextureSetList
class CTextureSetObj : public CObject
{
    
     
    DECLARE_SERIAL( CTextureSetObj )
    
    
public:
	CTextureSetObj();
	~CTextureSetObj()
	{
		SafeDeleteArray(this->List_textureSet);
	}

	TXTUV * List_textureSet;
	int List_textureCoordCount;

	void Serialize( CArchive& ar );

};

class CTextureSetObjList : public CObList
{
public:
   CTextureSetObjList(){};
   
    
     
    DECLARE_SERIAL( CTextureSetObjList )
    
    
};

struct ConstantObj
{
   byte type;//0=float integer 1=integer register 2=time 0-1 calculation
   int registerIndex;
   int register1[4];
   float register2[4];
   DWORD timeStamp;
   DWORD duration;
};

class ConstantBlock
{
public:
	ConstantBlock()
	{
		this->constantArray = NULL;
	}

	~ConstantBlock()
	{
		SafeDeleteArray(this->constantArray);
	}

   int constantCount;
   ConstantObj * constantArray;
};



class CEXMeshObj : public CObject
{
    
     
    DECLARE_SERIAL( CEXMeshObj )
    
    
public:
    CEXMeshObj();
	~CEXMeshObj() { this->SafeDelete(); }
    CABFunctionLib ABEXT;
    
public:    
	 
               
	           MatrixARB                  List_matrixFunctions;
	           CString                    List_storedTextureFromFile[10];
	           CTextureSetObjList *       List_textureSetList;//additional texture sets
			   int                        List_blendType;//-1 = none 1 = even opacity 2 = opacity map darker is more transparent
			   int                        List_textureSelect[10];
			   ABBOX                      List_boundingBox;
			   float                      List_boundingRadius;
			   int                        List_BlendMethod[10];
			   CMaterialObject           *List_materialObject;
			   float                      m_visibilityTrack;
			   D3DMATRIX                  m_currentMatrix;//
			   float                      m_tempDistance;
			   D3DVECTOR                  m_meshCenter;
			   float                      m_meshBoundingSphereRadius;
			   CAdvancedVertexObj         m_abVertexArray;

               
               //Mesh effects
			   BOOL                       m_chromeWrap;
			   int                        m_chromeUvSet;

               //time hooks
			   long                       m_lastTimeStamp;

			   //effects extra
               CCharonaObj               *m_charona;
			   int                        m_groupReference;

			   CEXMeshObj                *m_shadowTemp;

               int                        m_validUvCount;

			   char                       m_byteFlagTemp;
			   //state block
			   ConstantBlock             *m_constantBlock;


			   //int                        m_textureTileType;

	//CEXMeshObj& operator=(const CEXMeshObj &rhs);
    int GetUVlayerCountString();
	BOOL UpdateShadowMesh(D3DMATRIX & objectMatrix,
						  D3DMATRIX & shadowDifference,
						  LPDIRECT3DDEVICE9 g_pd3dDevice);
    BOOL GetGeometryFromPlugin(CString fileName,
									   WORD *vertexCount,
									   ABVERTEX ** vertexArray,
									   WORD *indecieCount,
									   WORD **indecieArray,
									   CString texture[10]);
	void Rotate(D3DVECTOR rotation,D3DVECTOR meshCenter);
	void Move(D3DVECTOR translation,D3DVECTOR meshCenter);
	void Scale(D3DVECTOR scale,D3DVECTOR meshCenter);
	void Transform(float MatrixLocal[4][4]);
    void SafeDelete();
	void ConvertToX(CString * fileString);
	BOOL GetBox();
	void ScaleIt(float factor);
	BOOL CalculateBoundingRadius();
    //BOOL SetVerticies(ABVERTEX *vetexArray);
     int Render(LPDIRECT3DDEVICE9 g_pd3dDevice,int & vertexType,int uvSet);
	 int RenderFast(LPDIRECT3DDEVICE9 g_pd3dDevice,int & vertexType,int & uvSet);
	BOOL Clone(CEXMeshObj **copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL SetGeometry(CString textureFilename,
							 ABVERTEX * vertexArray,
							 WORD        vertexCount,
							 WORD *      indecieArray,
							 WORD        indecieCount);
	BOOL SetGeometry2(CString textureFilename,
					  ABVERTEX1L  * vertexArray,
					  WORD        vertexCount,
					  WORD *      indecieArray,
					  WORD        indecieCount);
	void GenerateSmoothNormals();
	void SmoothNormalsBasedOnArray(ABVERTEX *originalVertexArray);
	 BOOL ApplySphereMappingToObject(LPDIRECT3DDEVICE9 g_pd3dDevice, int & vertexType, int uvSet);
	void DumpTextureCoordSets();
	//void MoveTexture(float &u,float &v,int textureSet,float &adjustfactor);
	D3DVECTOR UpdatePolygonVertex(float matrixLoc[4][4],D3DVECTOR polygonLoc);
	void GenerateNormals(float smoothBasisAngle);
	void scaleMat ( float Xt, float Yt, float Zt,float TMatrix [ 4 ] [ 4 ] );
	void Serialize( CArchive& ar );
	BOOL LoadGeometryFromPlugin(CString fileName,float * sightRange,LPDIRECT3DDEVICE9 g_pd3dDevice,BNDBOX * boxOption);
	//Assimp import path: modern formats (FBX, OBJ, glTF, ...) - dispatched
	//from LoadGeometryFromPlugin by extension. Implemented in CAssimpModelImporter.cpp
	BOOL LoadGeometryFromAssimp(CString fileName,float * sightRange,LPDIRECT3DDEVICE9 g_pd3dDevice,BNDBOX * boxOption);
	//Shared finalization for the native and Assimp load paths (takes ownership of abVertex)
	BOOL FinalizeLoadedGeometry(ABVERTEX * abVertex,float sightBasis,BOOL gnerateNormals,float smoothAngle,LPDIRECT3DDEVICE9 g_pd3dDevice,BNDBOX * boxOption,BOOL boxOptionUsed,float * sightRange);
	BOOL ExportToObj(CString fileName);
	int Round(float x);
	void OptimizeMesh(CDialogBar *wndUpdate);
	void OptimizeMesh(CDialogBar *wndUpdate, float smoothShadeNormalReqAngle);
	void GetCenterRadius();
	void ExchangeTextureSets(int level,int level2);
	//BOOL UpdateVB();	
	 void InitBuffer(LPDIRECT3DDEVICE9 g_pd3dDevice,CGlobalMaterialShader * optionalShader);
	 int  GetUVlayerCount();
	 void TextureMovementCallback(float & gl_adjust,int & curVertexType);
	 BOOL GetBoxWithMarix(D3DMATRIX & positionWld,ABBOX * returnBox);
	void CenteredScaleIt(float factor);
	void UnifyNormals();
	void UnifyNormals(float angleTolerance);
	void AVGNormals(float smoothBasisAngle);
};

class CEXMeshObjList : public CObList
{
public:
   CEXMeshObjList(){};
   int InvalidateZeroTemp();
   void FlagAllZero();
   void AddAllocatedMeshOrRefresh(CEXMeshObj * mesh);
   BOOL ExportToObj(CString fileName);
     
    DECLARE_SERIAL( CEXMeshObjList )
	
    
   
	void SafeDelete();
};

#endif
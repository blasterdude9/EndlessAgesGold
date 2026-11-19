#include "CAdvancedVertexClass.h"
#include "CMaterialClass.h"
#include "CStateManagementClass.h"
#ifndef CTEXTRENDERCLASS_H
#define CTEXTRENDERCLASS_H

class CSymbolMapperObj : public CObject
{
	
     
	DECLARE_SERIAL( CSymbolMapperObj );
	
    
public:
    CSymbolMapperObj();

    float             m_percentageX;
	CString           m_uniqueID;//used to assign char reference
    float             m_uvIndexLeft;
	float             m_uvIndexRight;
	float             m_uvIndexTop;
	float             m_uvIndexBottom;
	int               m_gridDensity;
	float             m_origGridIndexHoriz;
    float             m_origGridIndexVert;
	CString           m_group;
	CString           m_extendedOption;
	

    void Calculate(int gridDensity,
				   float horizOffset,
				   float vertOffset,
				   CString id,
				   float xPercentage);
	void Serialize( CArchive& ar );
};

class CSymbolMapperObjList : public CObList
{
public:
   DECLARE_SERIAL( CSymbolMapperObjList );

   CSymbolMapperObjList(){};
   void SafeDelete();
   CSymbolMapperObj * GetByID(CString id);
   float GetUnitLength(int startPoint,CString string,float fontSize);
};



class CTextureRenderObj : public CObject
{
	CTextureRenderObj(){}
public:
    CTextureRenderObj(CString text,
					  int     mode,
					  float   startPosX,
					  float   startPosY,
					  float   fontSizeX,
					  float   fontSizeY,
					  BOOL    tempObj,
					  float   red,
					  float   green,
					  float   blue);

    CAdvancedVertexObj * m_vertexObject;
	CString              m_text;
	CMaterialObject    * m_materialObj;
	int                  m_blendMode;
	float                m_startPosX;
	float                m_startPosY;
	float                m_lastStartPosX;
	float                m_lastStartPosY;
	float                m_fontSizeX;
	float                m_fontSizeY;
	BOOL                 m_selfDeleteIt;
	float                m_measuredDistanceRef;
	CString              m_unicodeTempSection;


	void Initialize(LPDIRECT3DDEVICE9 g_pd3dDevice,CString textObject,BOOL constantlyChanging,CSymbolMapperObjList * symbolMapSys,int speechMode,float justifyPerc);
	void Render(LPDIRECT3DDEVICE9 g_pd3dDevice,CStateManagementObj * stateManager);
	void SafeDelete();
};

class CTextureRenderObjList : public CObList
{
public:
   CTextureRenderObjList(){};
   void SafeDelete();
   void DeleteFinishedTextDisplays();
   
   bool InitializeExternalMesh(LPDIRECT3DDEVICE9 g_pd3dDevice,
							   CString textObject,
							   float fontSizeX,
							   float fontSizeY,
							   CSymbolMapperObjList * symbolMapSys,
							   int speechMode,
							   float justifyPerc,
							   void ** meshRef,
							   int mode,
							   float red,
							   float green,
							   float blue,
							   float newLineScaleOpt);
   void AddTextObject(CString text,
				      int     mode,
			          float   startPosX,
				      float   startPosY,
				      float   fontSizeX,
				      float   fontSizeY,
					  BOOL    tempObj);

   void Flush();
};

#endif
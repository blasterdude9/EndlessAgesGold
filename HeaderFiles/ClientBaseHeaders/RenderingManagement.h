#pragma once
#include "d3d9.h"
#include "clientnetworkinglogic.h"
#include "sceneassembly.h"
#include "globalstatusvariables.h"
#include "usercontroller.h"
#include "menumanagement.h"


struct RenderParamPacket
	{
	CViewportObj* viewPort;
	D3DVECTOR     locationVect;
	D3DMATRIX     viewMat;
	D3DMATRIX     baseMat;
	};

class CRenderingManagement
{
public:
	CRenderingManagement(GlobalStatusVariables        * gStatus,
		                 SceneAssembly                * sceneAssembly,
						 CClientNetworkingLogic       * clientNet, 
						 UserController               * userControls,
						 CMenuManagement              * menuManagement);
	~CRenderingManagement(void);
	CRenderingManagement(void){};

	//Variables
	GlobalStatusVariables		* m_gStatus;
	SceneAssembly				* m_sceneAssembly;
	CClientNetworkingLogic		* m_clientNet;
	UserController				* m_userControls;
	CMenuManagement			    * m_menuManagement;
	HRESULT						  m_glerr;
	IDirect3DSurface9           * m_originalRenderTarget;
	
	//menu specific render cache
	CTranslucentPolyList        * m_nonTransRenderList;
    CTranslucentPolyList        * m_transRenderList;

	double                        GL_straightRenderDuration;
	int                           GL_polysRenderedPerFrame;
	int							  VPORTRENDERCOUNT;
	int                           MATHTIME;
	int                           GL_particleCalculationCount;
	int							  RENDERTIME;
	WORD                          g_indecieBuffer[30000];
	ABSPECFACE					  faceBufferVx[30000];
	D3DXVECTOR3                   bufferVx[30000];
	edge_struct					  edgeBuffer2[30000];
	ABVERTEX1LNT                  vxBufferLoc[300];
	D3DVIEWPORT9				  m_tempViewPort;

	// zone map capture (EngineX Display > Capture Zone Map): renders the zone from above in
	// tiles and saves Maps\<zone>.png + Maps\<zone>.txt (world bounds) for the minimap/world map
	int                           m_mapCapStep;        // -1 = idle
	int                           m_mapCapTiles;
	int                           m_mapCapTilePx;
	float                         m_mapCapTileWorld;
	D3DVECTOR                     m_mapCapMin, m_mapCapMax;
	IDirect3DSurface9           * m_mapCapImage;
	IDirect3DSurface9           * m_mapCapScratch;
	float                       * m_mapCapTileTop;     // highest / lowest geometry in each tile
	float                       * m_mapCapTileBottom;
	BOOL                          m_mapCapSavedFog;
	BOOL                          m_mapCapRefined;      // second pass done on just the land
	BOOL                          m_noClear;            // RenderVisualsBody skips its clear (map capture layering)
	void MapCapSetupTiles();
	// floors (zones with levels stacked on top of each other, like the Trendor city)
	int                           m_mapCapLevels;       // 0/1 = single map, 2+ = one picture per floor
	int                           m_mapCapLevel;        // floor being drawn, -1 = the normal top-down picture
	float                         m_mapCapLevelLo[8];   // lowest / highest walkable floor of each level
	float                         m_mapCapLevelHi[8];
	D3DCOLOR                      m_mapCapSavedBg;
	HRESULT                       m_mapCapMainHr;
	int  MapCapFindLevels();
	bool MapCapSaveImage(const CString & path, int size);
	CString                       m_mapCapZone;
	void MapCaptureFrame();
	


	//functions
	void RenderLoopBeginningChecks();
	void RenderVisualsImediateMode();
	void Popout_WldObject(CViewportObj* mptLoc);
	void RenderVisualsBody(D3DVECTOR locationVect,D3DMATRIX matrixBase,int visibilityMode,IDirect3DSurface9 * optionalRenderTarget,float & fov,D3DMATRIX * compoundMatrix);
	BOOL EvaluateAndRenderCharonaOverlaysV3(D3DVECTOR locationVect,D3DMATRIX & matrixBase,float & fov);
	void RenderEnviormentAttachedObjects(BOOL mirrorModeOn,D3DMATRIX & cameraMatrix,D3DMATRIX * compoundMatrix);
	void RenderStencilShadows3(D3DVECTOR locationVect, D3DMATRIX camMatrix);
	void RenderSpotShadows(D3DVECTOR locationVect);
	BOOL RealtimeSegmentTestUseCache(D3DVECTOR & lastPos,D3DVECTOR & curPos);
	void GetShadowPolyMeshSimple(   CCollisionObj *collisionObject, 
									ABVERTEX1LNT * buffer, 
									int & returnVCount,
									D3DXVECTOR3 objectPos,
									float radius,
									float extentUp,
									float extentDown);
	float Sgn_f(float & num);
	void UtilizeViewPort(CViewportObj* vptPtr,D3DMATRIX & matProj);
	void UtilizeViewPort(CViewportObj* vptPtr);
	void ReadoutsCallback();
	void OrthoModeBegin();
	void OrthoModeEnd();
	D3DXMATRIX GetClipProjectionMatrix(D3DXMATRIX matView, D3DXMATRIX matProj,D3DXPLANE & clip_plane);
	D3DMATRIX ProjectionMatrixToFrontClipProjection(D3DMATRIX projection, float * plane);
	void SetViewPortGetProjection( int & width,
								   int & hieght,
								   D3DMATRIX & matProj,
								   float & clipPlane,
								   float & fov);
	BOOL RenderEntityToSurface(  CMovementObj * mPtr,
								 IDirect3DSurface9 * surface, 
								 float camPitch, 
								 float camYaw, 
								 float camDist,
								 float fov,
								 D3DCOLOR * optionalBackgroundColor,
								 DWORD viewWidth,
								 DWORD viewHeight);
    BOOL RenderItemToSurface(	CItemObj * item,
								IDirect3DSurface9 * surface, 
								float camPitch, 
								float camYaw, 
								float camDist,
								float fov,
								D3DCOLOR * optionalBackgroundColor,
								DWORD viewWidth,
								DWORD viewHeight);
	
};

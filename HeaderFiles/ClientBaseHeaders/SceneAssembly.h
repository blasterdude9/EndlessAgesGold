#pragma once
#include "globalstatusvariables.h"
#include "CFrustumClass.h"
#ifndef FLOATEQ0
#define FLOATEQ0(n) ( n >= -0.01f && n <= 0.01f )
#endif

class SceneAssembly
{
public:
	SceneAssembly(GlobalStatusVariables * gStatus);
	~SceneAssembly(void);


	//variables
	GlobalStatusVariables * m_gStatus;

	//D3DVECTOR               m_refPt;
	//D3DVALUE                m_frustPlanes[20];
	int						GL_objectsFromTable;
	BOOL                    m_showAllNames;
	int                     m_groupFilterCount;
	int                     m_groupFilterArray[2048];//maximum group filter
	long					m_triggerDelayStamp;
	CLaserObj             * m_laserPointer;//used for test visual pointer
	CFrustumObject          m_frustum;
	D3DMATRIX			  * m_compoundMatrix;
	D3DMATRIX               m_compoundTemp;
	D3DVECTOR               m_compoundCenterTemp;

	//Scene preallocated constants, fullstrum and inview functions called so many times
	//float					m_frustumPlanes[5][4];
	//D3DXVECTOR3				m_planeNormal;
	//D3DXVECTOR3				m_points[8];
	D3DVECTOR               m_maxpt; 
	D3DVECTOR               m_minpt;
	D3DVECTOR               m_tempCenter;
	float                   m_refValue;
	float                   m_minValue;
	float                   m_maxValue;
	D3DXVECTOR3             m_boxmin; 
	D3DXVECTOR3             m_boxmax;
	BOOL                    m_false;
	BOOL                    m_true;
	D3DMATRIX				m_matrixTemp;
	BOOL                    m_proceedTemp;
	int                     m_mirrorBased;
	D3DMATRIX				m_appenAppliedMatrix;

	//reset list
	CReferenceObjList       m_refSkeletalResetList;
	
	//Functions
	void ZeroGroupFilters();
    void CatigorizeMesh(CEXMeshObj * imMesh,
						D3DMATRIX & cameraMatrix,
						D3DVECTOR & actualCenter,
						D3DMATRIX & invCameraMatrix,
						D3DVECTOR & fadeCenter,
						CReferenceObj * refOpt,
						CTranslucentPolyList * transRenderList,
						CTranslucentPolyList * renderList,
						int & mirrorBased);
	BOOL GeneratePopoutList(    D3DVECTOR pointLocation,
								D3DMATRIX camMatrix,
								D3DMATRIX invCamGeneratedMatrix,
								BOOL * renderEnviorment,
								DWORD * timeTaken,
								int mirrorBased,
								float fovView,
								float viewWidth,
								float viewHieght,
								float heightAspect,
								D3DMATRIX & fovMatrix,
								D3DMATRIX * compoundMatrix
								);
	/*void CreateFrustumPlanes(   D3DVECTOR *refPt, 
											 D3DVALUE *frustPlanes,
											 float & fov,
											 D3DMATRIX & camMatrix,
											 float & viewWidth,
											 float & viewHieght,
											 float & heightAspect);*/
	BOOL RenderTempListPreviews(D3DMATRIX camGeneratedMatrix,
								D3DMATRIX invCamGeneratedMatrix,
								D3DVECTOR pointLocation,
								D3DMATRIX camMatrix,
								float fovView);
	void AddGroupFilter(int & groupNumber);
	BOOL ValidateGroup(int & groupNumber);
	/*BOOL IsBoxInView(   ABBOX & cl_box,
						D3DMATRIX & camGenMatrix
						);*/
	BOOL                IsMeshInView(CEXMeshObj * mesh,
							         D3DMATRIX & camGenMatrix
							         );
	void ManageEnvEffects(CWldObject * wldObj,CEXMeshObj * mObj);
	void              ParseExplosionObjects(D3DMATRIX & camGeneratedMatrix,
											D3DMATRIX & camMatrix,
											D3DMATRIX & invCamGeneratedMatrix);
	void MissileParse(  D3DMATRIX &   cameraMatrix,
						D3DMATRIX &   inverseCameraMatrix);
	BOOL OutOfFovUntransformed( D3DMATRIX & matrix,
								ABBOX & bndBox,
								D3DMATRIX & cameraMatrix);
	void CachEntityForShadow(CMovementObj * entity);
	BOOL GetScreen3dPosition(D3DVECTOR & wldSpcPos,
										D3DMATRIX & cameraMatrix,
										float * multiplier,
										D3DVECTOR * returnScreenPos,
										float maxDist,
										float minDist,
										float & fov,
										float & viewHeightBasedAspect
							 );
	void DisplayTextBasedOn3dPosition(  D3DVECTOR & wldSpcPos,
										CViewportObj* ViewPort,
										D3DMATRIX & cameraMatrix,
										CString & textString,
										float maxDistance,
										float minDistance,
										float fontSize,
										CTextureRenderObj* &textureRenderObject,
										float red,
										float green,
										float blue,
										float screenYOffset); //kind of a hack for clan names
	
	void LogRenderObj(CString & objectname,int polyCount,int indecieCount, int vertexCount);
	void LogRenderObj(CString & objectname,D3DVECTOR position);
	void SceneAssembly::LogRenderObj(CString misc,int intMisc);
	void AppendInfoToFile(CString filename, CString data);
	void CreatePlane(D3DVALUE *plane, 
					 D3DXVECTOR3 & a, 
					 D3DXVECTOR3 & b, 
					 D3DXVECTOR3 & c);
	BOOL            OutOfFOV(   D3DXVECTOR3 & min, 
								D3DXVECTOR3 & max,
								D3DMATRIX & cameraMatrix);
    BOOL GetScreenXY(   double localZpos,
						double clipPlane,
						D3DVECTOR localizePosition,
						double * xScreenPosition,
						double * yScreenPosition,
						double   fov,
						double   vPortHAspect,
						float    screenWidth,
						float    screenHieght);
	void TriggerCheckCallback();
	int EvaluateTriggers(D3DVECTOR focusPosition,
						 CFrameObj * frame,
						 int controlType,
					     CMovementObj * entityPtr);
	BOOL IsPointInView(D3DVECTOR & cl_point,D3DMATRIX & camGenMatrix);
	void CatigorizeMesh(CEXMeshObj * imMesh,
						D3DMATRIX & cameraMatrix,
						D3DVECTOR & actualCenter,
						D3DMATRIX & invCameraMatrix,
						D3DVECTOR & fadeCenter,
						CReferenceObj * refOpt,
						CTranslucentPolyList * transRenderList,
						CTranslucentPolyList * renderList,
						int & mirrorBased,
						D3DMATRIX * compoundMatrix);
    void SkeletalDeformableMeshExtraction(	CSkeletonObject * skObj,
											D3DMATRIX & positionMatrix,
											BOOL & firstPersonMode,
											D3DMATRIX & camMatrix,
											BOOL & forceAdd,
											BOOL & renderInMirror,
											D3DMATRIX & invCameraMatrix,
											D3DMATRIX & shadowTransform,
											BOOL & enableModelShadowing,
						                    int & mirrorBased,
						                    D3DMATRIX * compoundMatrix,
											CTranslucentPolyList * transRenderList,
						                    CTranslucentPolyList * renderList);
	void SkeletalHiarcleMeshExtraction( CSkeletonObject * skObj,
										D3DMATRIX &positionMatrix,
										BOOL & firstPersonMode,
										D3DMATRIX &camMatrix,
										BOOL & forceAdd,
										BOOL & renderInMirror,
										D3DMATRIX &invCameraMatrix,
										D3DMATRIX & shadowTransform,
										BOOL & enableModelShadowing,
										int & mirrorBased,
										D3DMATRIX * compoundMatrix,
										CTranslucentPolyList * transRenderList,
										CTranslucentPolyList * renderList);
	void SkeletalEmitterCallback(   CSkeletonObject * skeletalPtr,
									D3DMATRIX * positionMatrix,
									D3DMATRIX & camMatrix,
									D3DMATRIX * compoundMatrix,
									CTranslucentPolyList * transRenderList);
};

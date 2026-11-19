//-----------------------------------------------------------------------------
//File  : EnvParticleSystem.cpp
//
//Desc  : Contains the implementation of the EnvParticleSystem class
//
//Author: Nathan McDaniel
//Date  : January 2000
//-----------------------------------------------------------------------------
 #include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "math.h"
#include "matrixarb.h"
#include <stdlib.h>
#include "EnvParticleSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------------------------
//Name: EnvParticleSystem()
//Desc: Class constructor
//-----------------------------------------------------------------------------
EnvParticleSystem::EnvParticleSystem()
{
	m_blendMode       = 1;
	m_textureIndexDyn = 0;
	m_paParticles     = NULL;
	m_renderParticles = NULL;
	im_mesh           = NULL;
	m_vshaderMesh     = NULL;

	m_baseMeshSize    = 2.0f;
    m_textureGrid	  = 1;
	m_influence       = .8f;
	

	m_crenderParticles = 0;
	m_NumParticles     = 0;

	m_IsBillboard     = TRUE;
	m_FirstTime       = TRUE;
	m_Regenerate      = TRUE;
	m_Fade            = FALSE;
	m_EndSpawn        = FALSE;
	m_IsSphere        = FALSE;
	m_Sort			  = FALSE;
	m_Burst           = FALSE;
	m_VaryLifetime    = FALSE;
	
	//Directional particle field
	m_IsDirectional   = FALSE;

	m_xvfactor        = 0;
	m_yvfactor        = 0;
	m_zvfactor        = 0;

	m_BurstSpeed       = 0;
	m_BurstSpeedDRate  = 0;
	m_BurstSpread      = 0;
	m_BurstSpreadDRate = 0;

    //timing
	tickLast          = GetTickCount();
	tickCurrent       = GetTickCount();

	m_systemStartTime = GetTickCount();

	m_lifetime        = 1000;
	m_FadeEnd		  = 1000;
	m_FadeStart		  = 0;
	m_FadeColor.r = m_FadeColor.g = m_FadeColor.b = 1;

	//for directional particle field
	m_sprdveloc		  = 0;
	m_partveloc		  = 1;
	
    m_compoundXfactor = 0.0f;
    m_compoundYfactor = 0.0f;
    m_compoundZfactor = 0.0f;

	m_camPos.x        = 0.0f;
	m_camPos.y        = 0.0f;
	m_camPos.z        = 0.0f;
	SystemOn          = FALSE;
	m_on              = TRUE;
	ZeroMemory(&matrixInit,  sizeof(D3DMATRIX));
	matrixInit._11    = matrixInit._22  = matrixInit._33  = matrixInit._44  = 1.0f;
	m_passedInMatrix = matrixInit;
	srand(GetTickCount()); //seed the random number generator
}






//-----------------------------------------------------------------------------
//Name: ~EnvParticleSystem()
//Desc: Class destructor
//-----------------------------------------------------------------------------
EnvParticleSystem::~EnvParticleSystem()
{
	if(im_mesh) { im_mesh->SafeDelete(); delete im_mesh; im_mesh=NULL; }
	if(m_paParticles) { delete [] m_paParticles; m_paParticles = NULL; }
	if(m_renderParticles) { delete [] m_renderParticles; m_renderParticles = NULL; }
	if(m_vshaderMesh) { m_vshaderMesh->SafeDelete(); delete m_vshaderMesh; m_vshaderMesh=NULL; }
	
}
//-----------------------------------------------------------------------------
BOOL EnvParticleSystem::ConstructVSPMesh(float baseParticleSize,
										 float influenceMin,
										 float influenceMax,
										 int textureGrid)
{
	 //prep a lockless vertx buffer for complete shader driven particles
	 /*if(m_vshaderMesh)
	 {
        m_vshaderMesh->SafeDelete();
		delete m_vshaderMesh;
		m_vshaderMesh = 0;
	 }*/
	 m_baseMeshSize = baseParticleSize;
	 m_textureGrid = textureGrid;
	 m_influence = influenceMin;
	 if(!m_vshaderMesh)
	 {
         m_vshaderMesh = new CEXMeshObj();
	     m_vshaderMesh->List_materialObject = new CMaterialObject();
	 }
	 m_vshaderMesh->List_materialObject->m_pixelShader = 1;
	 m_vshaderMesh->List_materialObject->m_vertexShader = 1;
	 m_vshaderMesh->List_blendType = m_blendMode;

	 //TEST REMOVE
	 if(m_keyFrames.GetCount()==0)
	 {
		 float testTempPosScale[4];
		 float testTempColorAlpha[4];
		 ZeroMemory(testTempPosScale,sizeof(float)*4);
		 ZeroMemory(testTempColorAlpha,sizeof(float)*4);

		 

		 m_keyFrames.AddKeyFrame(0.0f,testTempPosScale, testTempColorAlpha);//start anchor
		 testTempPosScale[0] = 1.0f;
		 testTempPosScale[1] = 2.0f;
		 testTempPosScale[3] = 0.2f;
		 testTempColorAlpha[0] = 0.5f;
		 testTempColorAlpha[1] = 1.0f;
		 testTempColorAlpha[3] = 1.0f;
		 m_keyFrames.AddKeyFrame(0.5f,testTempPosScale, testTempColorAlpha);//start anchor
		 testTempPosScale[0] = 0.0f;
		 testTempPosScale[1] = 4.0f;
		 testTempPosScale[3] = 0.0f;
		 testTempColorAlpha[0] = 0.0f;
		 testTempColorAlpha[1] = 0.0f;
		 testTempColorAlpha[3] = 0.0f;
		 m_keyFrames.AddKeyFrame(1.0f,testTempPosScale, testTempColorAlpha);//start anchor
	 }
	 //TEST REMOVE

     if(m_vshaderMesh->m_abVertexArray.m_vertexArray3)
	    delete [] m_vshaderMesh->m_abVertexArray.m_vertexArray3;
	    m_vshaderMesh->m_abVertexArray.m_vertexArray3 = 0;

	 if(m_vshaderMesh->m_abVertexArray.m_indecieArray)
	    delete [] m_vshaderMesh->m_abVertexArray.m_indecieArray;
	    m_vshaderMesh->m_abVertexArray.m_indecieArray = 0;

	 m_systemStartTime = GetTickCount();
	 m_cycleDuration = (DWORD)this->m_lifetime;

	 m_vshaderMesh->m_abVertexArray.m_uvCount = 3;

	 m_vshaderMesh->m_abVertexArray.m_vertexCount = m_NumParticles * 4;
	 m_vshaderMesh->m_abVertexArray.m_indecieCount = m_NumParticles * 6;
	 m_vshaderMesh->m_abVertexArray.m_vertexArray3 = new ABVERTEX3L[m_vshaderMesh->m_abVertexArray.m_vertexCount];
	 ZeroMemory(m_vshaderMesh->m_abVertexArray.m_vertexArray3,sizeof(ABVERTEX3L)*m_vshaderMesh->m_abVertexArray.m_vertexCount);

	 m_vshaderMesh->m_abVertexArray.m_indecieArray = new WORD[m_vshaderMesh->m_abVertexArray.m_indecieCount];
	 ZeroMemory(m_vshaderMesh->m_abVertexArray.m_indecieArray,sizeof(WORD)*m_vshaderMesh->m_abVertexArray.m_indecieCount);

	 //---custom vertex use definition---//
     //vertex=pos
	 //normal=axis dir around particle axis
	 //UV0 = texture uvs
	 //UV1 = panel offsets
	 //UV2 u=finalInfluence v=time offset 0-1

	 //control constants assigned before render
   
	 baseParticleSize *= .5f;
     D3DVECTOR axisTemp;
	 D3DVECTOR axisFinal;
	 ZeroMemory(&axisFinal,sizeof(D3DVECTOR));
	 
     float shiftXY = 1.0f / (float)textureGrid;

     int randStart = 0;
	 int randStartY = 0;

	 int axisMax = 360;
	 int axisCurrent = 0;
     for(int loop=0;loop<m_NumParticles;loop++)
	 {
		 randStart = rand()%textureGrid;
		 randStartY = rand()%textureGrid;
		 if(randStart == textureGrid)
		    randStart -= 1;
         //UVs
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx0.tu		= (float)randStart * shiftXY;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx0.tv		= (float)randStartY * shiftXY;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx0.tu		= (float)(randStart+1) * shiftXY;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx0.tv		= (float)randStartY * shiftXY;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx0.tu		= (float)(randStart+1) * shiftXY;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx0.tv		= (float)(randStartY+1) * shiftXY;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx0.tu		= (float)randStart * shiftXY;//lower left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx0.tv		= (float)(randStartY+1) * shiftXY;//lower left

		 //clockwise particle panel-use in offset in screen space
         m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx1.tu		= -baseParticleSize;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx1.tv		= baseParticleSize;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx1.tu		= baseParticleSize;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx1.tv		= baseParticleSize;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx1.tu		= baseParticleSize;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx1.tv		= -baseParticleSize;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx1.tu		= -baseParticleSize;//lower left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx1.tv		= -baseParticleSize;//lower left

		 //test raw position
		 /*m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].x		= -baseParticleSize;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].y		= baseParticleSize;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].x		= baseParticleSize;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].y		= baseParticleSize;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].x		= baseParticleSize;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].y		= -baseParticleSize;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].x		= -baseParticleSize;//lower left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].y		= -baseParticleSize;//lower left*/

         //influence
         float posMod = ((float)(rand()%100))*.01f;
		 posMod = influenceMin + (influenceMax - influenceMin)*posMod;

		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx2.tu		= posMod;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx2.tu		= posMod;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx2.tu		= posMod;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx2.tu		= posMod;

		 //time offset
		 float timeOffset = ((float)loop/(float)m_NumParticles)*1.0f;


		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx2.tv		= timeOffset;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx2.tv		= timeOffset;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx2.tv		= timeOffset;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx2.tv		= timeOffset;

		 //indicies
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6]				= loop*4;
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+1]			= loop*4+1;
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+2]			= loop*4+3;

		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+3]			= loop*4+3;
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+4]			= loop*4+1;
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+5]			= loop*4+2;

		 //axis
		 ZeroMemory(&axisTemp,sizeof(D3DVECTOR));
		 axisTemp.z = 1.0f;//already transposed 
		 axisCurrent = rand()%axisMax;
		 //if(axisCurrent > 0)
		 //{//rotate
               D3DMATRIX rotationMat;
			   //Generate 2D rotation matrix:
               float TempEqY = 0.0f;
   
               axisCurrent -= 180;
			   TempEqY = 3.141592f * ( (float)axisCurrent / 180.0f);
			  
			   float cy = (float)cos(TempEqY);
			   float sy = (float)sin(TempEqY);

			   //Initialize Y rotation matrix:
			   rotationMat.m[0][0]=cy; rotationMat.m[0][1]=0;   rotationMat.m[0][2]=-sy; rotationMat.m[0][3]=0;
			   rotationMat.m[1][0]=0;  rotationMat.m[1][1]=1;   rotationMat.m[1][2]=0;   rotationMat.m[1][3]=0;
			   rotationMat.m[2][0]=sy; rotationMat.m[2][1]=0;   rotationMat.m[2][2]=cy;  rotationMat.m[2][3]=0;

			   axisFinal.x = (rotationMat.m[0][0]*axisTemp.x)+(rotationMat.m[1][0]*axisTemp.y)+(rotationMat.m[2][0]*axisTemp.z);
               axisFinal.y = (rotationMat.m[0][1]*axisTemp.x)+(rotationMat.m[1][1]*axisTemp.y)+(rotationMat.m[2][1]*axisTemp.z);
			   axisFinal.z = (rotationMat.m[0][2]*axisTemp.x)+(rotationMat.m[1][2]*axisTemp.y)+(rotationMat.m[2][2]*axisTemp.z);
		 //}//endrotate
		 for(int vax=0;vax<4;vax++)
		 {//embed in vertex
			m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+vax].nx = axisFinal.x;
			m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+vax].ny = axisFinal.y;
			m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+vax].nz = axisFinal.z;
		 }//embed in vertex
	 }
	 

    return TRUE;
}
//-----------------------------------------------------------------------------
void EnvParticleSystem::InitInline(LPDIRECT3DDEVICE9 device)
{
	 if(m_vshaderMesh)
        return;


     m_vshaderMesh = new CEXMeshObj();
	 m_vshaderMesh->List_materialObject						= new CMaterialObject();
	 m_vshaderMesh->List_materialObject->m_pixelShader		= 1;
	 m_vshaderMesh->List_materialObject->m_vertexShader		= 1;
	 m_vshaderMesh->List_blendType							= m_blendMode;
	 m_vshaderMesh->List_textureSelect[0]					= m_textureIndexDyn;

	 //TEST REMOVE
	 if(m_keyFrames.GetCount()==0)
	 {
		 float testTempPosScale[4];
		 float testTempColorAlpha[4];
		 ZeroMemory(testTempPosScale,sizeof(float)*4);
		 ZeroMemory(testTempColorAlpha,sizeof(float)*4);

		 

		 m_keyFrames.AddKeyFrame(0.0f,testTempPosScale, testTempColorAlpha);//start anchor
		 testTempPosScale[0] = 1.0f;
		 testTempPosScale[1] = 2.0f;
		 testTempPosScale[3] = 0.2f;
		 testTempColorAlpha[0] = 0.5f;
		 testTempColorAlpha[1] = 1.0f;
		 testTempColorAlpha[3] = 1.0f;
		 m_keyFrames.AddKeyFrame(0.5f,testTempPosScale, testTempColorAlpha);//start anchor
		 testTempPosScale[0] = 0.0f;
		 testTempPosScale[1] = 4.0f;
		 testTempPosScale[3] = 0.0f;
		 testTempColorAlpha[0] = 0.0f;
		 testTempColorAlpha[1] = 0.0f;
		 testTempColorAlpha[3] = 0.0f;
		 m_keyFrames.AddKeyFrame(1.0f,testTempPosScale, testTempColorAlpha);//start anchor
	 }
	 //TEST REMOVE

     if(m_vshaderMesh->m_abVertexArray.m_vertexArray3)
	    delete [] m_vshaderMesh->m_abVertexArray.m_vertexArray3;
	    m_vshaderMesh->m_abVertexArray.m_vertexArray3 = 0;

	 if(m_vshaderMesh->m_abVertexArray.m_indecieArray)
	    delete [] m_vshaderMesh->m_abVertexArray.m_indecieArray;
	    m_vshaderMesh->m_abVertexArray.m_indecieArray = 0;

	 m_systemStartTime = GetTickCount();
	 m_cycleDuration = (DWORD)this->m_lifetime;

	 m_vshaderMesh->m_abVertexArray.m_uvCount = 3;

	 m_vshaderMesh->m_abVertexArray.m_vertexCount = m_NumParticles * 4;
	 m_vshaderMesh->m_abVertexArray.m_indecieCount = m_NumParticles * 6;
	 m_vshaderMesh->m_abVertexArray.m_vertexArray3 = new ABVERTEX3L[m_vshaderMesh->m_abVertexArray.m_vertexCount];
	 ZeroMemory(m_vshaderMesh->m_abVertexArray.m_vertexArray3,sizeof(ABVERTEX3L)*m_vshaderMesh->m_abVertexArray.m_vertexCount);

	 m_vshaderMesh->m_abVertexArray.m_indecieArray = new WORD[m_vshaderMesh->m_abVertexArray.m_indecieCount];
	 ZeroMemory(m_vshaderMesh->m_abVertexArray.m_indecieArray,sizeof(WORD)*m_vshaderMesh->m_abVertexArray.m_indecieCount);

	 //---custom vertex use definition---//
     //vertex=pos
	 //normal=axis dir around particle axis
	 //UV0 = texture uvs
	 //UV1 = panel offsets
	 //UV2 u=finalInfluence v=time offset 0-1

	 float baseConversion = m_baseMeshSize * .5f;
	 //control constants assigned before render
     D3DVECTOR axisTemp;
	 D3DVECTOR axisFinal;
	 ZeroMemory(&axisFinal,sizeof(D3DVECTOR));
	 
     float shiftXY = 1.0f / (float)m_textureGrid;

     int randStart = 0;
	 int randStartY = 0;

	 int axisMax = 360;
	 int axisCurrent = 0;
     for(int loop=0;loop<m_NumParticles;loop++)
	 {
		 randStart = rand()%m_textureGrid;
		 randStartY = rand()%m_textureGrid;
		 if(randStart == m_textureGrid)
		    randStart -= 1;
         //UVs
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx0.tu		= (float)randStart * shiftXY;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx0.tv		= (float)randStartY * shiftXY;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx0.tu		= (float)(randStart+1) * shiftXY;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx0.tv		= (float)randStartY * shiftXY;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx0.tu		= (float)(randStart+1) * shiftXY;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx0.tv		= (float)(randStartY+1) * shiftXY;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx0.tu		= (float)randStart * shiftXY;//lower left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx0.tv		= (float)(randStartY+1) * shiftXY;//lower left

		 //clockwise particle panel-use in offset in screen space
         m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx1.tu		= -baseConversion;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx1.tv		= baseConversion;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx1.tu		= baseConversion;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx1.tv		= baseConversion;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx1.tu		= baseConversion;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx1.tv		= -baseConversion;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx1.tu		= -baseConversion;//lower left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx1.tv		= -baseConversion;//lower left

		 //test raw position
		 /*m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].x		= -baseParticleSize;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].y		= baseParticleSize;//upper left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].x		= baseParticleSize;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].y		= baseParticleSize;//upper right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].x		= baseParticleSize;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].y		= -baseParticleSize;//lower right
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].x		= -baseParticleSize;//lower left
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].y		= -baseParticleSize;//lower left*/

         //influence
         float posMod = ((float)(rand()%100))*.01f;
		 posMod = m_influence + (1.0f - m_influence)*posMod;

		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx2.tu		= posMod;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx2.tu		= posMod;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx2.tu		= posMod;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx2.tu		= posMod;

		 //time offset
		 float timeOffset = ((float)loop/(float)m_NumParticles)*1.0f;


		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4].tx2.tv		= timeOffset;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+1].tx2.tv		= timeOffset;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+2].tx2.tv		= timeOffset;
		 m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+3].tx2.tv		= timeOffset;

		 //indicies
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6]				= loop*4;
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+1]			= loop*4+1;
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+2]			= loop*4+3;

		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+3]			= loop*4+3;
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+4]			= loop*4+1;
		 m_vshaderMesh->m_abVertexArray.m_indecieArray[loop*6+5]			= loop*4+2;

		 //axis
		 ZeroMemory(&axisTemp,sizeof(D3DVECTOR));
		 axisTemp.z = 1.0f;//already transposed 
		 axisCurrent = rand()%axisMax;
		 //if(axisCurrent > 0)
		 //{//rotate
               D3DMATRIX rotationMat;
			   //Generate 2D rotation matrix:
               float TempEqY = 0.0f;
   
               axisCurrent -= 180;
			   TempEqY = 3.141592f * ( (float)axisCurrent / 180.0f);
			  
			   float cy = (float)cos(TempEqY);
			   float sy = (float)sin(TempEqY);

			   //Initialize Y rotation matrix:
			   rotationMat.m[0][0]=cy; rotationMat.m[0][1]=0;   rotationMat.m[0][2]=-sy; rotationMat.m[0][3]=0;
			   rotationMat.m[1][0]=0;  rotationMat.m[1][1]=1;   rotationMat.m[1][2]=0;   rotationMat.m[1][3]=0;
			   rotationMat.m[2][0]=sy; rotationMat.m[2][1]=0;   rotationMat.m[2][2]=cy;  rotationMat.m[2][3]=0;

			   axisFinal.x = (rotationMat.m[0][0]*axisTemp.x)+(rotationMat.m[1][0]*axisTemp.y)+(rotationMat.m[2][0]*axisTemp.z);
               axisFinal.y = (rotationMat.m[0][1]*axisTemp.x)+(rotationMat.m[1][1]*axisTemp.y)+(rotationMat.m[2][1]*axisTemp.z);
			   axisFinal.z = (rotationMat.m[0][2]*axisTemp.x)+(rotationMat.m[1][2]*axisTemp.y)+(rotationMat.m[2][2]*axisTemp.z);
		 //}//endrotate
		 for(int vax=0;vax<4;vax++)
		 {//embed in vertex
			m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+vax].nx = axisFinal.x;
			m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+vax].ny = axisFinal.y;
			m_vshaderMesh->m_abVertexArray.m_vertexArray3[loop*4+vax].nz = axisFinal.z;
		 }//embed in vertex
	 }
	 m_vshaderMesh->InitBuffer(device,NULL);
}
//-----------------------------------------------------------------------------
int EnvParticleSystem::VSPRender(CStateManagementObj * renderStateObject,
								  LPDIRECT3DDEVICE9 device,
								  D3DVECTOR & camPos,
							      D3DMATRIX objMat,
								  void * textureDdatabase)
{
     InitInline(device);

	 //if(!m_vshaderMesh->m_abVertexArray.m_vertexBuffer)
	   //  m_vshaderMesh->m_abVertexArray.InitBuffer(device,NULL);

     //states
     renderStateObject->SetBlendType(device,m_vshaderMesh->List_blendType);
	 renderStateObject->SetFogFilter(device,TRUE);
	 renderStateObject->SetCullState(0,device);
	 renderStateObject->SetAlphaPixelTest(FALSE,device);
	 renderStateObject->SetWireFillState(FALSE,device);
	 renderStateObject->SetTextureState(this->m_textureIndexDyn,renderStateObject->m_intZero,device,(CTextureDBObjList*)textureDdatabase);
	 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int1,device,(CTextureDBObjList*)textureDdatabase);
	 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int2,device,(CTextureDBObjList*)textureDdatabase);
	 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int3,device,(CTextureDBObjList*)textureDdatabase);
	 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int4,device,(CTextureDBObjList*)textureDdatabase);
	 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int5,device,(CTextureDBObjList*)textureDdatabase);
	 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int6,device,(CTextureDBObjList*)textureDdatabase);
	 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int7,device,(CTextureDBObjList*)textureDdatabase);

	 renderStateObject->SetMaterial(device,&m_vshaderMesh->List_materialObject->m_useMaterial);
	 renderStateObject->SetShader(device,renderStateObject->m_shderSystemRef,m_vshaderMesh->List_materialObject->m_vertexShader);
	 renderStateObject->SetPxShader(device,renderStateObject->m_pxShderSystemRef,m_vshaderMesh->List_materialObject->m_pixelShader);

	 //positional :O
	 objMat._41 -= camPos.x;
	 objMat._42 -= camPos.y;
	 objMat._43 -= camPos.z;
   
	 //custom constants
     m_timeRegister = GetTickCount();
	 m_dwTempRegister = m_timeRegister-m_systemStartTime;//time passed
	 if(m_dwTempRegister != 0)
	 {
	    m_dwTempRegister = (m_dwTempRegister/(m_cycleDuration+1))*(m_cycleDuration+1);//floored offset
        m_systemStartTime += m_dwTempRegister;//bring closer to avoid large numbers
        m_dwTempRegister = m_timeRegister-m_systemStartTime;//get current time in cycle, normalizes withing cycle

        //m_dwTempRegister = .5f;

		m_vRegisterTemp.x = (float)m_cycleDuration;//store cycle duration in component X
		m_vRegisterTemp.y = (float)m_dwTempRegister;//store current system position in component Y
		//if(m_vRegisterTemp.y <= 0.0f);
		   //m_vRegisterTemp.y = .5f;
		
		m_vRegisterTemp.z = (float)m_dwTempRegister / (float)m_cycleDuration;//store 0.0-1.0 in component Z
		
		m_vRegisterTemp.w = 0.0f;//store ?? in component W
		device->SetVertexShaderConstantF( 90, (float*)&m_vRegisterTemp, 1 );//set timer info
        //set keyframe loop info
		/*int loopReg[4];
		loopReg[0] = 2;//key frame count
        loopReg[1] = 0;//initial value of AL internal vertex register
		loopReg[2] = 1;//loop stride
		loopReg[3] = 0;//reserved
		device->SetVertexShaderConstantI(1,loopReg,1);*/
        m_keyFrames.SetConstants(device);


	 }
	 else
	    return 0;
	 


     
	 renderStateObject->SetWorldMatrix((D3DXMATRIX*)&objMat,device);
     //render
	 renderStateObject->SetDiffuseSource(m_vshaderMesh->m_abVertexArray.m_advancedFixedMode,device);
	 renderStateObject->SetVertexType(m_vshaderMesh->m_abVertexArray.m_uvCount,device);
	 return m_vshaderMesh->m_abVertexArray.Render(device,renderStateObject->m_currentVertexType);
}
//-----------------------------------------------------------------------------
//Name: CreateField(n)
//Desc: Creates n particles and initializes the particle system field
//-----------------------------------------------------------------------------
int EnvParticleSystem::CreateField(int n,
								   int lifetime,
								   float fHeight,
								   float fRadius,
								   BOOL Sphere,
								   BOOL isDirect,
								   BOOL Regenerate,
								   BOOL VaryLifetime,
								   D3DVECTOR initialSpeed,
								   int scaleStart,
								   int scaleEnd
								   )
{
	m_IsDirectional = isDirect;
	m_Regenerate    = Regenerate;
	m_rcalls        = 0;
	m_VaryLifetime  = VaryLifetime;

	m_FieldHeight = fHeight;
	m_FieldRadius = fRadius;
	m_lifetime    = lifetime;

	m_paParticles = new particleStruct[n];
	if(m_paParticles==NULL) return ERR_PARTICLE_NOMEMORY;
	ZeroMemory(m_paParticles,sizeof(particleStruct)*n);

	m_renderParticles = new particleStruct[n];
	ZeroMemory(m_renderParticles,sizeof(particleStruct)*n);

	if( isDirect == FALSE )
		{
		m_xvfactor = initialSpeed.x;
		m_yvfactor = initialSpeed.y;
		m_zvfactor = initialSpeed.z;
		}
	else
		{
		m_partveloc = initialSpeed.x;
		m_sprdveloc = initialSpeed.y;
		}

	m_scaleStart = scaleStart;
	m_scaleEnd   = scaleEnd;

	m_NumParticles = n;
	m_ActiveParticles = n;

	m_FirstTime       = TRUE;
	return PARTICLE_OK;	
}

//-----------------------------------------------------------------------------
//Name: StopSpawning()
//Desc: Stops particles from spawning
//-----------------------------------------------------------------------------
void EnvParticleSystem::StopSpawning()
{
	m_Regenerate = FALSE;

	for(int i=0; i < m_NumParticles; i++) {
		m_paParticles[i].b_spawnagain = FALSE;
	}
}

//-----------------------------------------------------------------------------
//Name: StopSpawning()
//Desc: Stops particles from spawning
//-----------------------------------------------------------------------------
void EnvParticleSystem::RestartSpawning()
{
	m_Regenerate = TRUE;

	for(int i=0; i < m_NumParticles; i++) {
		m_paParticles[i].b_spawnagain = TRUE;
	}
}
//-----------------------------------------------------------------------------
//Name: CreateRenderList()
//Desc: creates a list of particles that are in the current view
//-----------------------------------------------------------------------------
void EnvParticleSystem::CreateRenderList()
{
	m_crenderParticles = 0;

	for(int i=0; i < m_NumParticles; i++) {

		if(m_paParticles[i].b_render == FALSE/* || InView(i)==FALSE*/)
			continue;
		else
			m_renderParticles[m_crenderParticles++] = m_paParticles[i];	
	}
}

//-----------------------------------------------------------------------------
//Name: InView()
//Desc: Checks to see if the particle is in the view
//-----------------------------------------------------------------------------
BOOL EnvParticleSystem::InView(int particleIndex)
{

	if(m_paParticles[particleIndex].camSpacePos.z <= 0.0f)
		return FALSE;

	float x = (float)fabs(m_paParticles[particleIndex].camSpacePos.x);
	float y = (float)fabs(m_paParticles[particleIndex].camSpacePos.y);
	float z = m_paParticles[particleIndex].camSpacePos.z;

	float wDist;

	if(x > y)
		wDist = x;
	else
		wDist = y;

	if(wDist == 0.0f)
		return TRUE;

	float angle = (float)fabs(atan(wDist/z));

	if(angle > (m_fov*.65f))
		return FALSE;
	else
		return TRUE;

}

//-----------------------------------------------------------------------------
//Name: OrderParticle(n)
//Desc: Orders the particles from largest to smallest distance from the camera
//-----------------------------------------------------------------------------
void EnvParticleSystem::OrderParticles()
{

	particleStruct *sortArray = NULL;

	sortArray = new particleStruct[m_crenderParticles];

	if(m_crenderParticles > 0)
		CopyMemory(sortArray, m_renderParticles, sizeof(particleStruct)*m_crenderParticles);

	int maxdist=0;
	
	for(int i=0; i < m_crenderParticles; i++) {

		for(int innerloop=0; innerloop < m_crenderParticles; innerloop++)
		{
			if(sortArray[innerloop].camSpacePos.z > sortArray[maxdist].camSpacePos.z)
				maxdist = innerloop;
		}

		m_renderParticles[i] = sortArray[maxdist];
		sortArray[maxdist].camSpacePos.z = -1.0f;

	}

	delete [] sortArray; 
	sortArray = NULL;

}

//-----------------------------------------------------------------------------
//Name: Burst(float, float, float, float)
//Desc: Adds burst of speed to a directional particle field
//-----------------------------------------------------------------------------
void EnvParticleSystem::Burst(float speed, float speeddrate, float spread, float spreaddrate)
{
	
	if(m_Burst==FALSE) {

		if(speed > 0 && speeddrate > 0) { 
			m_Burst            = TRUE;
			m_BurstSpeed       = speed;
			m_BurstSpeedDRate  = speeddrate;
			m_BurstSpread      = 0;
			m_BurstSpreadDRate = 0;
		}

		if(spread > 0 && spreaddrate > 0) {
			if(m_Burst==FALSE) {
				m_BurstSpeed      = 0;
				m_BurstSpeedDRate = 0;
			}
			m_Burst            = TRUE;
			m_BurstSpread      = spread;
			m_BurstSpreadDRate = spreaddrate;	
		}

		m_BSPD    = m_BurstSpeed;
		m_BSPDDR  = m_BurstSpeedDRate;
		m_BSPRD   = m_BurstSpread;
		m_BSPRDDR = m_BurstSpreadDRate;

	}
}

//-----------------------------------------------------------------------------
//Name: Clone(EnvParticleSystem *, LPDIRECT3D7)
//Desc: Associates a mesh with the class
//-----------------------------------------------------------------------------
void EnvParticleSystem::Clone(EnvParticleSystem ** clone,LPDIRECT3DDEVICE9 pD3Ddevice)
{

	EnvParticleSystem * midClone = new EnvParticleSystem();
	if(im_mesh)
	{
	   im_mesh->Clone(&midClone->im_mesh,pD3Ddevice);
	   midClone->im_mesh->InitBuffer(pD3Ddevice,NULL);
	}

	if(this->m_vshaderMesh)
	{
	   this->m_vshaderMesh->Clone(&midClone->m_vshaderMesh,pD3Ddevice);
	   midClone->m_vshaderMesh->InitBuffer(pD3Ddevice,NULL);
	}

	m_keyFrames.Clone(midClone->m_keyFrames);

	midClone->m_EndSpawn		  = m_EndSpawn;
	midClone->m_IsSphere		  = m_IsSphere;
	midClone->m_Sort			  = m_Sort;
	midClone->m_IsBillboard	  = m_IsBillboard;
	midClone->m_IsDirectional	  = m_IsDirectional;
	midClone->m_FadeTime		  = m_FadeTime;
	midClone->m_Fade			  = m_Fade;
	midClone->m_FadeColor.r	  = m_FadeColor.r;
	midClone->m_FadeColor.g	  = m_FadeColor.g;
	midClone->m_FadeColor.b	  = m_FadeColor.b;
	midClone->m_FadeStart		  = m_FadeStart;
	midClone->m_FadeEnd		  = m_FadeEnd;
	midClone->m_scaleStart    = m_scaleStart;
	midClone->m_scaleEnd      = m_scaleEnd;
	midClone->m_NumParticles	  = m_NumParticles;
	midClone->m_FieldRadius	  = m_FieldRadius;
	midClone->m_FieldHeight	  = m_FieldHeight;
	midClone->m_xvfactor		  = m_xvfactor;
	midClone->m_yvfactor		  = m_yvfactor;
	midClone->m_zvfactor		  = m_zvfactor;
	midClone->m_sprdveloc		  = m_sprdveloc;
	midClone->m_partveloc 		  = m_partveloc;
    midClone->m_lifetime  		  = m_lifetime;
    midClone->m_Burst            = m_Burst;
	midClone->m_Regenerate       = m_Regenerate;
	midClone->m_BurstSpeed       = m_BurstSpeed;
	midClone->m_BurstSpeedDRate  = m_BurstSpeedDRate;
	midClone->m_BurstSpread      = m_BurstSpread;
	midClone->m_BurstSpreadDRate = m_BurstSpreadDRate;  
	midClone->m_blendMode		 = m_blendMode;
	midClone->m_cycleDuration    = m_cycleDuration;
	midClone->m_baseMeshSize	 = m_baseMeshSize;
    midClone->m_textureGrid	     = m_textureGrid;
	midClone->m_influence	     = m_influence;
	midClone->m_textureName      = m_textureName;
	midClone->m_textureIndexDyn  = m_textureIndexDyn;
	
	if(m_NumParticles > 0)
	{
	   midClone->m_paParticles      = new particleStruct[m_NumParticles];
	   ZeroMemory(midClone->m_paParticles,sizeof(particleStruct)*m_NumParticles);

    
	   midClone->m_renderParticles = new particleStruct[m_NumParticles];
	   ZeroMemory(midClone->m_renderParticles,sizeof(particleStruct)*m_NumParticles);
	}
	midClone->m_FirstTime   = TRUE;

	midClone->m_ActiveParticles = m_NumParticles;

	midClone->m_VaryLifetime = m_VaryLifetime;
	midClone->m_BSPD    = m_BurstSpeed;
	midClone->m_BSPDDR  = m_BurstSpeedDRate;
	midClone->m_BSPRD   = m_BurstSpread;
	midClone->m_BSPRDDR = m_BurstSpreadDRate;
	*clone = midClone;
}
//-----------------------------------------------------------------------------
//Name: SetFade(float, float, BOOL, D3DCOLOR, BOOL)
//Desc: Sets the fading
//-----------------------------------------------------------------------------
void EnvParticleSystem::SetFade(int Start,int End, BOOL EndSpawn, D3DCOLORVALUE color, BOOL FadeTime)
{
	m_EndSpawn = EndSpawn;
	m_FadeTime = FadeTime;

	if(m_IsDirectional==TRUE) m_FadeTime = TRUE;
		
	if( End > 0 && End > Start) {
	
		m_Fade = TRUE;
		
		if(m_FadeTime==TRUE) {
			
			m_FadeStart = Start;
			m_FadeEnd   = End;

				 if(m_lifetime > m_FadeEnd) m_lifetime = m_FadeEnd;
			else if(m_lifetime < m_FadeEnd) m_FadeEnd  = m_lifetime;
			
			m_FadeStart = Start;
			m_FadeEnd   = m_FadeEnd;
		}
		else {
			m_FadeStart = Start;
			m_FadeEnd   = End;
		}
				
		m_FadeColor     = color;
	}
	else m_Fade = FALSE;
			

}

void EnvParticleSystem::Serialize( CArchive& ar )
{
int dump = 0;

BOOL  ReservedBOOL  = 0;
float ReservedFloat = 0;
int   ReservedInt   = 0;

int version = 4;

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << m_EndSpawn
	      << m_IsSphere
		  << m_Sort
	      << m_IsBillboard
		  << m_IsDirectional
		  << m_FadeTime
		  << m_Fade
		  << m_FadeColor.r
		  << m_FadeColor.g
		  << m_FadeColor.b
		  << m_FadeStart
		  << m_FadeEnd
		  << m_scaleStart
		  << m_scaleEnd
		  << m_NumParticles
		  << m_FieldRadius
		  << m_FieldHeight
		  << m_xvfactor
		  << m_yvfactor
		  << m_zvfactor
		  << m_sprdveloc
		  << m_partveloc
          << m_lifetime
		  << m_Burst
		  << m_Regenerate
		  << m_VaryLifetime
		  << m_BurstSpeed
		  << m_BurstSpeedDRate
		  << m_BurstSpread
		  << m_BurstSpreadDRate
		  << storedFileName
		  << im_mesh
		  << m_blendMode
		  << m_baseMeshSize
		  << m_textureGrid
		  << m_influence
		  << ReservedFloat
		  << ReservedFloat
		  << version
		  << ReservedInt
		  << ReservedInt;

	   ar << m_systemName;
	   
	   ar << m_keyFrames.GetCount()
		  << m_textureName;
	   for(POSITION lPos = m_keyFrames.GetHeadPosition();lPos != NULL;)
	   {
           HprtKeyFrame * kPtr = (HprtKeyFrame *) m_keyFrames.GetNext(lPos);
	       ar << kPtr;
	   }
	}
	else
	{
	   int loadedVersion = 0;
	   ar >> m_EndSpawn
	      >> m_IsSphere
		  >> m_Sort
	      >> m_IsBillboard
		  >> m_IsDirectional
		  >> m_FadeTime
		  >> m_Fade
		  >> m_FadeColor.r
		  >> m_FadeColor.g
		  >> m_FadeColor.b
		  >> m_FadeStart
		  >> m_FadeEnd
		  >> m_scaleStart
		  >> m_scaleEnd
		  >> m_NumParticles
		  >> m_FieldRadius
		  >> m_FieldHeight
		  >> m_xvfactor
		  >> m_yvfactor
		  >> m_zvfactor
		  >> m_sprdveloc
		  >> m_partveloc
          >> m_lifetime
		  >> m_Burst
		  >> m_Regenerate
		  >> m_VaryLifetime
		  >> m_BurstSpeed
		  >> m_BurstSpeedDRate
		  >> m_BurstSpread
		  >> m_BurstSpreadDRate
		  >> storedFileName
		  >> im_mesh
	      >> m_blendMode
		  >> m_baseMeshSize
		  >> m_textureGrid
		  >> m_influence
		  >> ReservedFloat
		  >> ReservedFloat
		  >> loadedVersion
		  >> ReservedInt
		  >> ReservedInt;

	   m_cycleDuration = (DWORD)this->m_lifetime;
	   int kCount = 0;
	   switch(loadedVersion)
	   {
	      case 2:
              ar >> m_systemName;

		      m_blendMode = 2;
		      m_textureGrid = 1;
		      m_baseMeshSize = 1.0f;

		  break;
		  case 3:
              ar >> m_systemName;
			  ar >> m_vshaderMesh;
			  ar >> kCount;
			  for(int iC=0;iC<kCount;iC++)
			  {
				  HprtKeyFrame * kPtr = NULL;
				  ar >> kPtr;
				  m_keyFrames.AddTail(kPtr);
			  }
              if(m_vshaderMesh)
			  {//port
			     m_textureName = m_vshaderMesh->List_storedTextureFromFile[0];//port
				 m_vshaderMesh->SafeDelete();
                 delete m_vshaderMesh;
				 m_vshaderMesh = 0;
		      }//end port
		  break;
		  case 4:
              ar >> m_systemName;
			  ar >> kCount
				 >> m_textureName;

			  for(int iC=0;iC<kCount;iC++)
			  {
				  HprtKeyFrame * kPtr = NULL;
				  ar >> kPtr;
				  m_keyFrames.AddTail(kPtr);
			  }
		  break;

	   }
          
	  m_paParticles = new particleStruct[m_NumParticles];
	  ZeroMemory(m_paParticles,sizeof(particleStruct)*m_NumParticles);
	  
	  m_renderParticles = new particleStruct[m_NumParticles];
	  ZeroMemory(m_renderParticles,sizeof(particleStruct)*m_NumParticles);
	  
	  m_ActiveParticles = m_NumParticles;

	  m_FirstTime   = TRUE;
	  m_rcalls      = 0;
	  m_on          = TRUE;

	}
}
void EnvParticleSystemList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        EnvParticleSystem * ptrLoc = (EnvParticleSystem *) GetNext(posLoc);   
		delete ptrLoc;
		ptrLoc=0;
	}
	RemoveAll();
}

BOOL EnvParticleSystemList::DestroyParticleSystem(EnvParticleSystem ** envPtr)
{
	POSITION poslast;
	for(POSITION posLoc = GetHeadPosition();(poslast = posLoc) != NULL;){
        EnvParticleSystem * ptrLoc = (EnvParticleSystem *) GetNext(posLoc);       
        if(ptrLoc == *envPtr){
           delete ptrLoc;
		   ptrLoc = 0;
           *envPtr = NULL;
		   RemoveAt(poslast);
		   return TRUE;
		}
	}
	return FALSE;
}

IMPLEMENT_SERIAL(EnvParticleSystem, CObject, 0)
IMPLEMENT_SERIAL(EnvParticleSystemList, CObList, 0)
IMPLEMENT_SERIAL(HprtKeyFrame, CObject, 0)
IMPLEMENT_SERIAL(HprtKeyFrameSystem, CObList, 0)

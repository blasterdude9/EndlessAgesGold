//-----------------------------------------------------------------------------
// File  : EnvParticleSystem.cpp
//
// Desc  : Contains the implementation of the EnvParticleSystem class
//
// Author: Nathan McDaniel
// Date  : January 2000
//-----------------------------------------------------------------------------
//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include <d3d9.h>
//#include <afxtempl.h>
#include "math.h"
#include "matrixarb.h"

//These two used for random number generator
#include <stdlib.h>

#include "EnvParticleSystem.h"

//-----------------------------------------------------------------------------
// Name: EnvParticleSystem()
// Desc: Class constructor
//-----------------------------------------------------------------------------
EnvParticleSystem::EnvParticleSystem()
{
	m_paParticles     = NULL;
	m_renderParticles = NULL;
	im_mesh           = NULL;
	

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
	
	// Directional particle field
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
	srand(GetTickCount()); // seed the random number generator
}






//-----------------------------------------------------------------------------
// Name: ~EnvParticleSystem()
// Desc: Class destructor
//-----------------------------------------------------------------------------
EnvParticleSystem::~EnvParticleSystem()
{
	if(im_mesh) { im_mesh->SafeDelete(); delete im_mesh; im_mesh=NULL; }
	if(m_paParticles) { delete [] m_paParticles; m_paParticles = NULL; }
	if(m_renderParticles) { delete [] m_renderParticles; m_renderParticles = NULL; }
}
//-----------------------------------------------------------------------------
// Name: CreateField(n)
// Desc: Creates n particles and initializes the particle system field
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
// Name: Render() - DIRECTIONAL PARTICLE SYSTEM
// Desc: Renders a scene
//-----------------------------------------------------------------------------
 int EnvParticleSystem::Render(LPDIRECT3DDEVICE9 pd3dDevice,
							         D3DVECTOR & camPos,
							         D3DMATRIX & objMat,
							         D3DMATRIX & viewMatrix,
							         float & fov,
									 CStateManagementObj * stateManager
							  )
{
	m_rcalls++;

	long currentTickCount = GetTickCount();
	m_fov = fov;

	m_camPos = camPos;
	m_objMatrix = objMat;

	//Is it the first time through the loop?
	if(m_FirstTime==TRUE)
		{

		int oldlife = m_lifetime;

		for(int i=0; i < m_NumParticles; i++) {
			m_paParticles[i].b_spawnagain = TRUE;
			SpawnParticle(i);
			m_paParticles[i].b_render = FALSE;
			m_paParticles[i].b_spawnagain = TRUE;
			m_paParticles[i].directMat = objMat;
		}

		m_lifetime = oldlife;

		tickLast = GetTickCount();
		m_FirstTime = FALSE;

		}
		
	MoveParticles(objMat);

	abMath.TrnsfrMtrxInvrs(viewMatrix.m,m_invCamMatrix.m);

	PointsToCamSpace();
	CreateRenderList();

	if(m_Fade==TRUE) Fade();
	//if(m_Sort==TRUE) OrderParticles();

	D3DMATERIAL9 particleMaterial;
	ZeroMemory(&particleMaterial, sizeof(D3DMATERIAL9));
    
	D3DMATRIX worldTransform;
	ZeroMemory(&worldTransform, sizeof(D3DMATRIX));

	D3DMATRIX oldViewTransform;
	pd3dDevice->GetTransform(D3DTS_VIEW , &oldViewTransform);
	pd3dDevice->SetTransform(D3DTS_VIEW , &matrixInit);
	stateManager->SetViewMatrix((D3DXMATRIX*)&matrixInit,pd3dDevice);

    ZeroMemory(&worldTransform, sizeof(D3DMATRIX));
		worldTransform._11 = worldTransform._22 = worldTransform._33 = worldTransform._44 = 1.0f;
 
    //pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) );
	 stateManager->SetDiffuseSource(im_mesh->m_abVertexArray.m_advancedFixedMode,pd3dDevice);
	 stateManager->SetVertexType(1,pd3dDevice);
	pd3dDevice->SetStreamSource( 0, im_mesh->m_abVertexArray.m_vertexBuffer, 0,sizeof(ABVERTEX1L) );
    
    pd3dDevice->SetIndices(im_mesh->m_abVertexArray.m_indicieBuffer);

	stateManager->SetBlendType(pd3dDevice,im_mesh->List_blendType);

    float mod = 0.0f;
    float timePassed = 0.0f;
	float pcnt = 1.0;	
	for(int i=0; i < m_crenderParticles;i++){

		particleMaterial.Emissive = m_renderParticles[i].color;
        
		//pd3dDevice->SetMaterial( &particleMaterial );
		stateManager->SetMaterial(pd3dDevice,&particleMaterial );
	    
		if( m_scaleEnd != m_scaleStart )
	    {
			
			
			    timePassed = (float)(currentTickCount - (m_renderParticles[i].startTime+this->m_scaleStart));
		       
				     pcnt = 1.0f;
				     mod = (float)(m_renderParticles[i].lifelength-this->m_scaleStart);
                     if(mod >= timePassed && mod != 0)
					 {
					    pcnt = 1.0f - (timePassed / mod);
                        if(pcnt < 0.0f || pcnt > 1.0f)
						   pcnt = 1.0f;
					 }
					 else if(timePassed < m_renderParticles[i].lifelength)
					    pcnt = 0.0f;    

				
			    worldTransform._11 = pcnt;
			    worldTransform._22 = pcnt;
		}

		worldTransform._41 = m_renderParticles[i].camSpacePos.x;
	    worldTransform._42 = m_renderParticles[i].camSpacePos.y;
	    worldTransform._43 = m_renderParticles[i].camSpacePos.z;
		
		stateManager->SetWorldMatrix((D3DXMATRIX*)&worldTransform,pd3dDevice);



		//render manually

		    
			pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               im_mesh->m_abVertexArray.m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (im_mesh->m_abVertexArray.m_indecieCount/3)//UINT PrimitiveCount
                                               );
		//im_mesh->Render(pd3dDevice);
	}

	pd3dDevice->SetTransform(D3DTS_VIEW , &oldViewTransform);
	stateManager->SetViewMatrix((D3DXMATRIX*)&oldViewTransform,pd3dDevice);

	return m_crenderParticles;
}






//-----------------------------------------------------------------------------
// Name: Render() - REGULAR PARTICLE SYSTEM
// Desc: Renders a scene
//-----------------------------------------------------------------------------
 int EnvParticleSystem::Render(LPDIRECT3DDEVICE9 pd3dDevice,
							         D3DVECTOR & camPos,
							         D3DMATRIX & viewMatrix,
							         float & fov,
									 CStateManagementObj * stateManager
							         )
{
	m_rcalls++;

	m_fov = fov;

	m_camPos = camPos;

	if(m_FirstTime==TRUE) {
		
		for(int i=0; i < m_NumParticles; i++) {
			m_paParticles[i].b_spawnagain = TRUE;
			SpawnParticle(i);
		}
		
		tickLast = GetTickCount()-50;
		m_FirstTime = FALSE;
       
	}

	MoveParticles();

	abMath.TrnsfrMtrxInvrs(viewMatrix.m,m_invCamMatrix.m);

	PointsToCamSpace();

	CreateRenderList();

	if(m_Fade==TRUE) Fade();
	//if(m_Sort==TRUE) OrderParticles();

	D3DMATERIAL9 particleMaterial;
	ZeroMemory(&particleMaterial, sizeof(D3DMATERIAL9));
	
	D3DMATRIX worldTransform;
	ZeroMemory(&worldTransform, sizeof(D3DMATRIX));
	worldTransform._11 = worldTransform._22 = worldTransform._33 = worldTransform._44 = 1.0f;

	D3DMATRIX oldViewTransform;
	pd3dDevice->GetTransform(D3DTS_VIEW , &oldViewTransform);
	pd3dDevice->SetTransform(D3DTS_VIEW , &matrixInit);
	stateManager->SetViewMatrix((D3DXMATRIX*)&matrixInit,pd3dDevice);

     //pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) );
	 stateManager->SetDiffuseSource(im_mesh->m_abVertexArray.m_advancedFixedMode,pd3dDevice);
	 stateManager->SetVertexType(1,pd3dDevice);

	 stateManager->SetBlendType(pd3dDevice,im_mesh->List_blendType);
	 pd3dDevice->SetStreamSource( 0, im_mesh->m_abVertexArray.m_vertexBuffer, 0,sizeof(ABVERTEX1L) );
    
    pd3dDevice->SetIndices(im_mesh->m_abVertexArray.m_indicieBuffer);

	for(int i=0; i < m_crenderParticles; i++) {
		
		particleMaterial.Emissive = m_renderParticles[i].color;
        
		//pd3dDevice->SetMaterial( &particleMaterial );
		stateManager->SetMaterial(pd3dDevice,&particleMaterial );

		

        worldTransform._41 = m_renderParticles[i].camSpacePos.x;
	    worldTransform._42 = m_renderParticles[i].camSpacePos.y;
	    worldTransform._43 = m_renderParticles[i].camSpacePos.z;
		
		stateManager->SetWorldMatrix((D3DXMATRIX*)&worldTransform,pd3dDevice);

		
		pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
			                                   0,
                                               0,//UINT MinIndex,
                                               im_mesh->m_abVertexArray.m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (im_mesh->m_abVertexArray.m_indecieCount/3)//UINT PrimitiveCount
                                               );
		//im_mesh->Render(pd3dDevice);
	}

	pd3dDevice->SetTransform(D3DTS_VIEW , &oldViewTransform);
	stateManager->SetViewMatrix((D3DXMATRIX*)&oldViewTransform,pd3dDevice);

return m_crenderParticles;
};

//-----------------------------------------------------------------------------
// Name: PointsToCamSpace()
// Desc: transforms points to camera space
//-----------------------------------------------------------------------------
void EnvParticleSystem::PointsToCamSpace()
{
	
	float xpos, ypos, zpos;
 
	for(int i=0; i < m_NumParticles; i++)
		{

		if(m_IsDirectional==TRUE)
			{
			xpos = m_paParticles[i].ax;
			ypos = m_paParticles[i].ay;
			zpos = m_paParticles[i].az;
			}
		else
			{
			xpos = m_paParticles[i].x;
			ypos = m_paParticles[i].y;
			zpos = m_paParticles[i].z;
			}

		xpos -= m_camPos.x;
		ypos -= m_camPos.y;
		zpos -= m_camPos.z;

		m_paParticles[i].camSpacePos.x = 
			m_invCamMatrix.m[0][0]*xpos +
			m_invCamMatrix.m[1][0]*ypos +
			m_invCamMatrix.m[2][0]*zpos;
			
		m_paParticles[i].camSpacePos.y =  
			m_invCamMatrix.m[0][1]*xpos +
			m_invCamMatrix.m[1][1]*ypos +
			m_invCamMatrix.m[2][1]*zpos;
			
		m_paParticles[i].camSpacePos.z = 
			m_invCamMatrix.m[0][2]*xpos +
			m_invCamMatrix.m[1][2]*ypos +
			m_invCamMatrix.m[2][2]*zpos;
		
		}

}

//-----------------------------------------------------------------------------
// Name: StopSpawning()
// Desc: Stops particles from spawning
//-----------------------------------------------------------------------------
void EnvParticleSystem::StopSpawning()
{
	m_Regenerate = FALSE;

	for(int i=0; i < m_NumParticles; i++) {
		m_paParticles[i].b_spawnagain = FALSE;
	}
}

//-----------------------------------------------------------------------------
// Name: StopSpawning()
// Desc: Stops particles from spawning
//-----------------------------------------------------------------------------
void EnvParticleSystem::RestartSpawning()
{
	m_Regenerate = TRUE;

	for(int i=0; i < m_NumParticles; i++) {
		m_paParticles[i].b_spawnagain = TRUE;
	}
}



//-----------------------------------------------------------------------------
// Name: SpawnParticle(n)
// Desc: Spawns a new particle at the nth index
//-----------------------------------------------------------------------------
 void EnvParticleSystem::SpawnParticle(int i)
{
	int xdist=0,zdist=0;
	long currentTime = GetTickCount();

	/*if(!m_paParticles[i].b_spawnagain)
    if(m_Regenerate==TRUE){
	   m_paParticles[i].b_spawnagain = TRUE;
       m_paParticles[i].b_render     = TRUE;
	   //return;
	}*/

	if(m_paParticles[i].b_spawnagain == FALSE && m_paParticles[i].b_render == TRUE) {
		m_ActiveParticles--;
	}

	
	
	if(m_paParticles[i].b_spawnagain == FALSE) {
		m_paParticles[i].b_render = FALSE;
		//return;
	}
	
	if(m_Regenerate==FALSE) m_paParticles[i].b_spawnagain = FALSE;
	else m_paParticles[i].b_spawnagain = TRUE;

	m_paParticles[i].worldTransform = matrixInit;

	//IF PARTICLE FIELD IS A DIRECTIONAL PARTICLE SYSTEM
	if(m_IsDirectional==TRUE) {
		
		//SQUARE GENERATION FIELD
		//m_paParticles[i].x       = float(-m_FieldRadius+rand()%(m_FieldRadius*2));
		//m_paParticles[i].y	 = float(-m_FieldRadius+rand()%(m_FieldRadius*2));
		m_paParticles[i].x   = float( m_FieldRadius*((rand()%1000)*0.001f));
		m_paParticles[i].y	 = float(m_FieldRadius-m_paParticles[i].x);
		
		switch((rand()%4)) {
			case 0: break;
			case 1: m_paParticles[i].x = -m_paParticles[i].x;
					break;
			case 2: m_paParticles[i].x = -m_paParticles[i].x;
					m_paParticles[i].y = -m_paParticles[i].y;
					break;
			case 3: m_paParticles[i].y = -m_paParticles[i].y;
				  break;
		}
		
		m_paParticles[i].z       = float(0);
		m_paParticles[i].zv      = float(m_partveloc*((rand()%1000)*0.001f));
		
		m_paParticles[i].spreadx = float(m_sprdveloc*((rand()%1000)*0.001f));
		m_paParticles[i].spready = float(m_sprdveloc*((rand()%1000)*0.001f));
					
	}
	//PARTICLE FIELD IS A REGULAR PARTICLE SYSTEM
	else {
		
		int denominator = 1;
	
		if( m_FieldRadius > 0 && m_FieldRadius < 1 )
			denominator = 1;
		else
			denominator = int(m_FieldRadius);

		if( denominator == 0 )
			denominator = 1;
		
		m_paParticles[i].x        = float(m_camPos.x-m_FieldRadius+rand()%int(denominator*2));
		m_paParticles[i].xv       = float(m_xvfactor*((rand()%1000)*0.001f));
        		
		if(m_IsSphere==TRUE) m_paParticles[i].y  = float(m_camPos.y-m_FieldRadius+rand()%int(denominator*2));
		else                 m_paParticles[i].y  = float(m_camPos.y+m_FieldHeight);

		m_paParticles[i].yv       = float(m_yvfactor*((rand()%1000)*0.001f));
        
		m_paParticles[i].z        = float(m_camPos.z-m_FieldRadius+rand()%int(denominator*2));
		m_paParticles[i].zv       = float(m_zvfactor*((rand()%1000)*0.001f));
	
	}

	if(m_VaryLifetime==FALSE) 
		m_paParticles[i].lifelength = m_lifetime;
	else 
		m_paParticles[i].lifelength = int(m_lifetime*((rand()%1000)*0.001f));
	
	m_paParticles[i].lifetime   = currentTime + ((long)m_paParticles[i].lifelength);
	
	m_paParticles[i].b_render   = TRUE;
	m_paParticles[i].color      = m_FadeColor;
	m_paParticles[i].startTime  = currentTime;

    
}

//-----------------------------------------------------------------------------
// Name: CreateRenderList()
// Desc: creates a list of particles that are in the current view
//-----------------------------------------------------------------------------
void EnvParticleSystem::CreateRenderList()
{
	m_crenderParticles = 0;

	for(int i=0; i < m_NumParticles; i++) {

		if(m_paParticles[i].b_render == FALSE || InView(i)==FALSE)
			continue;
		else
			m_renderParticles[m_crenderParticles++] = m_paParticles[i];
			
	}

}



//-----------------------------------------------------------------------------
// Name: MoveParticles() - Directional Particle System
// Desc: Moves and interpolates particles' positions
//-----------------------------------------------------------------------------
void EnvParticleSystem::MoveParticles(D3DMATRIX objMat)
{

	double interpolate = 1;
	tickCurrent        = GetTickCount();
	double delay        = (double)(tickCurrent - tickLast); 
	if(delay > 100.0)
	   delay = 100.0;

	tickLast           = tickCurrent;

	interpolate = (delay * 0.01);
   
	for(int i=0; i < m_NumParticles; i++) {		
		
		//Check particle life
		if(m_lifetime >= 0 && m_paParticles[i].lifetime < tickCurrent) {
			m_paParticles[i].directMat = objMat;
			SpawnParticle(i);
		}
				
		/*THIS CODE PRODUCES PARTICLES THAT SPREAD INTO FOUR COLUMNS
		------------------------------------------------------------------------------------
		if(m_paParticles[i].y<0) { m_paParticles[i].y -= m_sprdveloc*interpolate; }
		else { m_paParticles[i].y += m_sprdveloc*interpolate;	}
		if(m_paParticles[i].x<0) { m_paParticles[i].x -= m_sprdveloc*interpolate; }
		else { m_paParticles[i].x += m_sprdveloc*interpolate;	}
		------------------------------------------------------------------------------------
		*/
	
		//Spread Particles
		if(m_paParticles[i].y<0) { 
			m_paParticles[i].y -= (m_paParticles[i].spready + m_BSPRD)*interpolate;
		}
		else { 
			m_paParticles[i].y += (m_paParticles[i].spready + m_BSPRD)*interpolate;
		}
		if(m_paParticles[i].x<0) {
			m_paParticles[i].x -= (m_paParticles[i].spreadx + m_BSPRD)*interpolate;
		}
		else {
			m_paParticles[i].x += (m_paParticles[i].spreadx + m_BSPRD)*interpolate;
		}
		
		//Move Particles
		m_paParticles[i].z += (m_paParticles[i].zv + m_BSPD)*interpolate;


		if(m_Burst==TRUE) {
			
			m_BSPD  -= m_BSPDDR;
			m_BSPRD -= m_BSPRDDR;
			
			if(m_BSPD < 0)  m_BSPD  = 0;
			if(m_BSPRD < 0) m_BSPRD = 0;
			
			if(m_BSPD < 0 && m_BSPRD < 0) m_Burst = FALSE;

		}

		/*[row][column]*/
		//Translate particle into world coordinates
		m_paParticles[i].ax = (m_paParticles[i].directMat.m[0][0]*m_paParticles[i].x) + 
							  (m_paParticles[i].directMat.m[1][0]*m_paParticles[i].y) +
							  (m_paParticles[i].directMat.m[2][0]*m_paParticles[i].z) + 
							  (m_paParticles[i].directMat.m[3][0]);
		
		m_paParticles[i].ay = (m_paParticles[i].directMat.m[0][1]*m_paParticles[i].x) + 
							  (m_paParticles[i].directMat.m[1][1]*m_paParticles[i].y) +
							  (m_paParticles[i].directMat.m[2][1]*m_paParticles[i].z) +
							  (m_paParticles[i].directMat.m[3][1]);
		
		m_paParticles[i].az = (m_paParticles[i].directMat.m[0][2]*m_paParticles[i].x) + 
							  (m_paParticles[i].directMat.m[1][2]*m_paParticles[i].y) +
							  (m_paParticles[i].directMat.m[2][2]*m_paParticles[i].z) +
							  (m_paParticles[i].directMat.m[3][2]);
	
	}

}






//-----------------------------------------------------------------------------
// Name: MoveParticles() - Regular Particle System
// Desc: Moves and interpolates particles' positions
//-----------------------------------------------------------------------------
 void EnvParticleSystem::MoveParticles()
{

	double interpolate = 1.0;
	tickCurrent        = GetTickCount();
	double delay        = double(tickCurrent - tickLast); 
	tickLast           = tickCurrent;
	if(delay != 0) interpolate = (delay * 0.01);
	double sidex = 0, sidez = 0, sidey = 0;
    
	//check particle life and calculate particle distance
	for(int i=0; i < m_NumParticles; i++) {	
		
		if(m_lifetime  != -1  && m_paParticles[i].lifetime < tickCurrent) SpawnParticle(i);
				
		m_paParticles[i].x += (m_paParticles[i].xv+m_compoundXfactor)*interpolate;
		m_paParticles[i].y += (m_paParticles[i].yv+m_compoundYfactor)*interpolate;
		m_paParticles[i].z += (m_paParticles[i].zv+m_compoundZfactor)*interpolate;

	}

}

//-----------------------------------------------------------------------------
// Name: Fade()
// Desc: Fades the particles
//-----------------------------------------------------------------------------
void EnvParticleSystem::Fade()
{
 	
	double pcnt=1.0;
    
	//FADE THE PARTICLE ACCORDING TO TIME
	if(m_FadeTime==TRUE) 
	{
		long lifeinto = 0;
				
		long currTick = GetTickCount();
		double timePassed = 0.0f;
		double mod = 0.0f;
        if(	m_FadeStart != m_FadeEnd )
		for(int i=0; i < m_crenderParticles; i++) 
		{		
			    timePassed = (float)(currTick - (m_renderParticles[i].startTime+this->m_FadeStart));
		       
				     pcnt = 1.0;
				     mod = (float)(m_renderParticles[i].lifelength-this->m_FadeStart);
                     if(mod >= timePassed && mod != 0)
					 {
					    pcnt = 1.0 - (timePassed / mod);
						if(pcnt < 0.0f || pcnt > 1.0f)
						   pcnt = 1.0f;
					 }
					 else if(timePassed < m_renderParticles[i].lifelength)
					    pcnt = 0.0;    

				
			   

			
				
				//pcnt = 1.0 - (float)((lifeinto-m_FadeStart)/(m_FadeEnd-m_FadeStart));
				m_renderParticles[i].color.r = m_FadeColor.r * pcnt;
				m_renderParticles[i].color.g = m_FadeColor.g * pcnt;
				m_renderParticles[i].color.b = m_FadeColor.b * pcnt;
				
			//else m_renderParticles[i].color = m_FadeColor;
		}
	}
	//FADE THE PARTICLE ACCORDING TO DISTANCE
	else {
		double dist;

		for(int i=0; i < m_crenderParticles; i++) {
		
			dist = m_renderParticles[i].camSpacePos.z;

			if( m_FadeEnd != m_FadeStart )
				{
				pcnt = 1.0f - ((double)((dist-m_FadeStart)/(m_FadeEnd-m_FadeStart)));

				m_renderParticles[i].color.r = m_FadeColor.r * pcnt;
				m_renderParticles[i].color.g = m_FadeColor.g * pcnt;
				m_renderParticles[i].color.b = m_FadeColor.b * pcnt;
				}
			else m_renderParticles[i].color = m_FadeColor;

			if(dist >= m_FadeEnd) {
				if(m_EndSpawn==TRUE) SpawnParticle(i);
				m_renderParticles[i].color.r = 0;
				m_renderParticles[i].color.g = 0;
				m_renderParticles[i].color.b = 0;
			}
		}

	}//END FADE PARTICLE ACCORDING TO DISTANCE
	

}



//-----------------------------------------------------------------------------
// Name: InView()
// Desc: Checks to see if the particle is in the view
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
// Name: OrderParticle(n)
// Desc: Orders the particles from largest to smallest distance from the camera
//-----------------------------------------------------------------------------
void EnvParticleSystem::OrderParticles()
{

	particleStruct *sortArray = NULL;

	sortArray = new particleStruct[m_crenderParticles];

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
// Name: Burst(float, float, float, float)
// Desc: Adds burst of speed to a directional particle field
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
// Name: Clone(EnvParticleSystem *, LPDIRECT3D7)
// Desc: Associates a mesh with the class
//-----------------------------------------------------------------------------
void EnvParticleSystem::Clone(EnvParticleSystem ** clone,LPDIRECT3DDEVICE9 pD3Ddevice)
{

	EnvParticleSystem * midClone = new EnvParticleSystem();
	im_mesh->Clone(&midClone->im_mesh,pD3Ddevice);
	midClone->im_mesh->InitBuffer(pD3Ddevice,NULL);

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
	
	midClone->m_paParticles      = new particleStruct[m_NumParticles];
	ZeroMemory(midClone->m_paParticles,sizeof(particleStruct)*m_NumParticles);

	midClone->m_renderParticles = new particleStruct[m_NumParticles];
	ZeroMemory(m_renderParticles,sizeof(particleStruct)*m_NumParticles);

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
// Name: Load mesh() - one time only
// Desc: Associates a mesh with the class
//-----------------------------------------------------------------------------
int EnvParticleSystem::LoadMesh(CString filename,
								LPDIRECT3DDEVICE9 g_pd3dDevice,
								CString textureFileName,
								float scale)
{
	if(!im_mesh){
		im_mesh = new CEXMeshObj();
	}
	else{
        im_mesh->SafeDelete();
		delete im_mesh;
		im_mesh=0;
		im_mesh = new CEXMeshObj();
	}
	    float range = 0;
	    //hardcode billboard im_mesh->LoadGeometryFromPlugin(filename,&range,g_pd3dDevice);
		
		ABVERTEX1L * vertexArray  = new ABVERTEX1L[4];
	    ZeroMemory(vertexArray, sizeof(D3DVERTEX) * 4);

  	    WORD *      indecieArray = new WORD[6];
	    ZeroMemory(indecieArray, sizeof(WORD) * 6);

	    WORD        vertexCount  = 4;
	    WORD        indecieCount = 6;

		float billboardSize = scale;
    
	    //vertex 1 upper left
	    vertexArray[0].x  = billboardSize;
	    vertexArray[0].y  = billboardSize;
	    vertexArray[0].z  = 0.0f;
	    vertexArray[0].tx0.tu = 1.0f;
	    vertexArray[0].tx0.tv = 0.0f;
	    vertexArray[0].nx = 0.0f;
	    vertexArray[0].ny = 0.0f;
	    vertexArray[0].nz = -1.0f;
	    //vertex 2 bottom left
	    vertexArray[1].x  = billboardSize;
	    vertexArray[1].y  = -billboardSize;
	    vertexArray[1].z  = 0.0f;
	    vertexArray[1].tx0.tu = 0.0f;
	    vertexArray[1].tx0.tv = 0.0f;
	    vertexArray[1].nx = 0.0f;
	    vertexArray[1].ny = 0.0f;
	    vertexArray[1].nz = -1.0f;
	    //vertex 3 bottom right
	    vertexArray[2].x  = -billboardSize;
	    vertexArray[2].y  = -billboardSize;
	    vertexArray[2].z  = 0.0f;
	    vertexArray[2].tx0.tu = 0.0f;
	    vertexArray[2].tx0.tv = 1.0f;
	    vertexArray[2].nx = 0.0f;
	    vertexArray[2].ny = 0.0f;
	    vertexArray[2].nz = -1.0f;
	    //vertex 4 upper right
	    vertexArray[3].x  = -billboardSize;
	    vertexArray[3].y  = billboardSize;
	    vertexArray[3].z  = 0.0f;
	    vertexArray[3].tx0.tu = 1.0f;
	    vertexArray[3].tx0.tv = 1.0f;
	    vertexArray[3].nx = 0.0f;
	    vertexArray[3].ny = 0.0f;
	    vertexArray[3].nz = -1.0f;
	    //indecies
	    //no facing
	    /*indecieArray[0] = 1;
	    indecieArray[1] = 0;
	    indecieArray[2] = 3;
	    indecieArray[3] = 2;
	    indecieArray[4] = 1;
	    indecieArray[5] = 3;*/

		indecieArray[0] = 3;
	    indecieArray[1] = 0;
	    indecieArray[2] = 1;
	    indecieArray[3] = 3;
	    indecieArray[4] = 1;
	    indecieArray[5] = 2;


        im_mesh->List_materialObject = new CMaterialObject();
	    im_mesh->SetGeometry2(textureFileName,
							  vertexArray,
							  vertexCount,
							  indecieArray,
							  indecieCount);
		
		im_mesh->m_abVertexArray.m_uvCount = 1;
		im_mesh->InitBuffer(g_pd3dDevice,NULL);

	return PARTICLE_OK;
}


//-----------------------------------------------------------------------------
// Name: SetFade(float, float, BOOL, D3DCOLOR, BOOL)
// Desc: Sets the fading
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

int version = 2;

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
		  << ReservedBOOL
		  << ReservedBOOL
		  << ReservedBOOL
		  << ReservedFloat
		  << ReservedFloat
		  << ReservedFloat
		  << version
		  << ReservedInt
		  << ReservedInt;

	   ar << m_systemName;
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
	      >> ReservedBOOL
		  >> ReservedBOOL
		  >> ReservedBOOL
		  >> ReservedFloat
		  >> ReservedFloat
		  >> ReservedFloat
		  >> loadedVersion
		  >> ReservedInt
		  >> ReservedInt;

	   if(loadedVersion == 2)
	   {
          ar >> m_systemName;

	   }

      //im_mesh->List_blendType = 2;
          
	  m_paParticles = new particleStruct[m_NumParticles];
	  ZeroMemory(m_paParticles,sizeof(particleStruct)*m_NumParticles);
	  
	  m_renderParticles = new particleStruct[m_NumParticles];
	  ZeroMemory(m_renderParticles,sizeof(particleStruct)*m_NumParticles);
	  
	  m_ActiveParticles = m_NumParticles;

	  m_BSPD    = m_BurstSpeed;
	  m_BSPDDR  = m_BurstSpeedDRate;
	  m_BSPRD   = m_BurstSpread;
	  m_BSPRDDR = m_BurstSpreadDRate;

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
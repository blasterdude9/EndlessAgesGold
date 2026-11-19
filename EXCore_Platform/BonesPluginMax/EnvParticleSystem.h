//-----------------------------------------------------------------------------
// File  : EnvParticleSystem.h
//
// Desc  : Contains the declaration of the EnvParticleSystem class
//
// Author: Nathan McDaniel
// Date  : January 2000
//-----------------------------------------------------------------------------
#include "CStateManagementClass.h"
#ifndef ENV_PARTICLESYSTEM_H
#define ENV_PARTICLESYSTEM_H
#include "CEXMeshClass.h"
//-----------------------------------------------------------------------------

#define D3D_OVERLOADS

//-----------------------------------------------------------------------------

struct particleStruct {

	double x;
	double xv;
	double ax;// directional particle system -- holds the actual render position in world space

	double y;
	double yv;
	double ay; // directional particle system -- holds the actual render position in world space
	
	double z;
	double zv;
	double az; // directional particle system -- holds the actual render position in world space
	
	D3DVECTOR camSpacePos;

	double spreadx; // directional particle system
	double spready; // directional particle system

	long lifetime;
	int   lifelength;  

	double dist;
	long startTime;
		
	D3DMATRIX directMat; // directional particle system

	D3DMATRIX worldTransform; // particle's world transformation

	BOOL b_render;		 // should render particle or not?
	BOOL b_spawnagain;   // spawn the particle more than once?

	D3DCOLORVALUE color;
};

//-----------------------------------------------------------------------------

class  EnvParticleSystem : public CObject {
	  
       
      DECLARE_SERIAL( EnvParticleSystem )
      
      
	public:
		MatrixARB abMath;
        CEXMeshObj * im_mesh;

		CString m_systemName;
		
		ABVERTEXONE * m_virtualVertexArray;
        // used to determine whether or not to rotate the mesh
		BOOL m_IsBillboard;
        BOOL SystemOn;
		CString storedFileName;

		int CreateField
			(
			int n,
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
			);

		int LoadMesh(CString filename,
				     LPDIRECT3DDEVICE9 g_pd3dDevice,
				     CString textureFileName,
					 float scale);	
		void SetFade(int Start,int End, BOOL EndSpawn, D3DCOLORVALUE color, BOOL FadeTime);
		void Clone(EnvParticleSystem **clone,LPDIRECT3DDEVICE9 pD3Ddevice);

		void CreateRenderList();
		void PointsToCamSpace();

        BOOL InView( int particleIndex );
			
		//functions
		void Serialize( CArchive& ar );
		//FOR REGULAR PARTICLE SYSTEMS
		 int Render
			(
			LPDIRECT3DDEVICE9 pd3dDevice,
			D3DVECTOR & camPos,
			D3DMATRIX & viewMatrix,
			float & fov,
			CStateManagementObj * stateManager
			);
		//FOR DIRECTIONAL PARTICLE SYSTEMS
		 int Render
			(
			LPDIRECT3DDEVICE9 pd3dDevice,
			D3DVECTOR & camPos,
			D3DMATRIX & objMat,
			D3DMATRIX & viewMatrix,
			float & fov,
			CStateManagementObj * stateManager
			);

		 EnvParticleSystem();
		~EnvParticleSystem();
		
		//FOR DIRECTIONAL PARTICLE SYSTEMS
		void MoveParticles(D3DMATRIX objMat);
		void Burst(float speed, float speeddrate, float spread, float spreaddrate);
		//FOR REGULAR PARTICLE SYSTEMS
		 void SpawnParticle(int i);
		void MoveParticles();
		void StopSpawning();
		void RestartSpawning();
		void OrderParticles();
		void Fade();
		
        D3DMATRIX m_passedInMatrix;

		BOOL m_EndSpawn;
		BOOL m_IsSphere;
		BOOL m_Sort;
		BOOL m_FirstTime; // First time through render loop?
		BOOL m_Fade;
		BOOL m_FadeTime;
		BOOL m_IsDirectional;
		BOOL m_Burst;
		BOOL m_Regenerate;
		BOOL m_VaryLifetime;
		BOOL m_on;
		
		D3DMATRIX matrixInit;
		D3DMATRIX m_invCamMatrix;
		D3DMATRIX m_camMatrix;
		D3DMATRIX m_objMatrix;
		
		particleStruct *m_paParticles; // pointer to an array of particles
		int m_NumParticles; // number of particles in the system

		particleStruct *m_renderParticles;
		int m_crenderParticles;
	
		// the camera coordinates
		D3DVECTOR m_camPos;

		//field of view
		float m_fov;

		// starting fade color
		D3DCOLORVALUE m_FadeColor;

		float m_zBaseDist;
		// start & end distance/time of fading
		int m_FadeStart;
		int m_FadeEnd;

		// start & end distance/time of scaling
		int m_scaleStart;
		int m_scaleEnd;		
		
		int m_ActiveParticles;
		long tickLast,tickCurrent;

		// radius of generation field
		float m_FieldRadius;
		float m_FieldHeight;
		
		// Factors used for speed
		float m_xvfactor;
		float m_yvfactor;
		float m_zvfactor;

		double m_compoundXfactor;
		double m_compoundYfactor;
		double m_compoundZfactor;

		float m_BurstSpeed;
		float m_BurstSpeedDRate;
		float m_BurstSpread;
		float m_BurstSpreadDRate;

		double m_BSPD;
		double m_BSPDDR;
		double m_BSPRD;
		double m_BSPRDDR;

		// Reference used for lifetime (in milliseconds)
		int m_lifetime;

		// Number of calls to Render
		int m_rcalls;

		// DIRECTIONAL PARTICLE FIELD
		float m_sprdveloc;
		float m_partveloc;

};

class  EnvParticleSystemList : public CObList
{
public:
   EnvParticleSystemList(){};
   void SafeDelete();
   BOOL DestroyParticleSystem(EnvParticleSystem ** envPtr);
      
       
      DECLARE_SERIAL( EnvParticleSystemList )
      
      
};
//-----------------------------------------------------------------------------

#define ERR_PARTICLE_NOMEMORY (-1)
#define PARTICLE_OK (0)

//-----------------------------------------------------------------------------
#endif //ENV_PARTICLESYSTEM_H
//-----------------------------------------------------------------------------
//File  : EnvParticleSystem.h
//
//Desc  : Contains the declaration of the EnvParticleSystem class
//
//Author: Nathan McDaniel
//Date  : January 2000
//-----------------------------------------------------------------------------
#include "CStateManagementClass.h"
#ifndef ENV_PARTICLESYSTEM_H
#define ENV_PARTICLESYSTEM_H
#include "CEXMeshClass.h"
//-----------------------------------------------------------------------------

#define D3D_OVERLOADS


class HprtKeyFrame : public CObject
{
    DECLARE_SERIAL( HprtKeyFrame )
	public:
		HprtKeyFrame()
		{
		    ZeroMemory(m_header,sizeof(float)*4);
		    ZeroMemory(m_startPosScale,sizeof(float)*4);
			ZeroMemory(m_startColorAlpha,sizeof(float)*4);
		};
		~HprtKeyFrame(){};

	//var-constants
	float m_header[4];//0= start time
	float m_startPosScale[4];
	float m_startColorAlpha[4];

	//functions
    void Clone(HprtKeyFrame ** clone)
    {
         HprtKeyFrame * newPtr = new HprtKeyFrame();
		 CopyMemory(newPtr->m_header,m_header,sizeof(float)*4);
		 CopyMemory(newPtr->m_startPosScale,m_startPosScale,sizeof(float)*4);
		 CopyMemory(newPtr->m_startColorAlpha,m_startColorAlpha,sizeof(float)*4);
         *clone = newPtr;
	}
    //IO
	void Serialize( CArchive& ar )
	{
		CObject::Serialize( ar );
			if(ar.IsStoring() )
			{
	           for(int loop=0;loop<4;loop++)
			   {
                  ar << m_header[loop]
				     << m_startPosScale[loop]
					 << m_startColorAlpha[loop];
			   }
			}
			else
			{
	           for(int loop=0;loop<4;loop++)
			   {
                  ar >> m_header[loop]
				     >> m_startPosScale[loop]
					 >> m_startColorAlpha[loop];
			   }
			}
	}
};

class HprtKeyFrameSystem : public CObList
{
   DECLARE_SERIAL( HprtKeyFrameSystem )
   public:
		HprtKeyFrameSystem()
		{
			m_keyFrameCount = 0;
		};
		~HprtKeyFrameSystem()
		{
		    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
			{
				HprtKeyFrame * hPtr = (HprtKeyFrame *)GetNext(posLoc);
				delete hPtr;
				hPtr = 0;
			}
			RemoveAll();
		};
  
   //vars
   int m_keyFrameCount;


   //Functions 
   void Clone(HprtKeyFrameSystem & clone)
   {
		for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
			HprtKeyFrame * hPtr = (HprtKeyFrame *)GetNext(posLoc);
            HprtKeyFrame * cPtr = NULL;
            hPtr->Clone(&cPtr);
            clone.AddTail(cPtr);
		}
   }
   
   BOOL AddKeyFrame(float keyPos,float * posScale, float *colorAlpha)
   {
        HprtKeyFrame * hPtr = new HprtKeyFrame();
        float tempHeader[4];
		ZeroMemory(tempHeader,sizeof(float)*4);
		tempHeader[0] = keyPos;//start time
		if(keyPos > 0.0f)
		   tempHeader[2] = 1.0f/keyPos;//start time

		CopyMemory(hPtr->m_header,tempHeader,sizeof(float)*4);
		CopyMemory(hPtr->m_startColorAlpha,colorAlpha,sizeof(float)*4);
        CopyMemory(hPtr->m_startPosScale,posScale,sizeof(float)*4);
		AddTail(hPtr);
		ResetKeyRanges();
		return TRUE;
   }

   BOOL AddKeyFrame(float keyPos)
   {
	    POSITION posAt = NULL;
		HprtKeyFrame * pPrev = NULL;
        for(POSITION posLoc = GetHeadPosition();(posAt = posLoc)!= NULL;)
		{
            HprtKeyFrame * hPtr = (HprtKeyFrame *)GetNext(posLoc);
			if(keyPos < hPtr->m_header[0])
			{
			   HprtKeyFrame * cloneP = NULL;
			   hPtr->Clone(&cloneP);
			   cloneP->m_header[0] = keyPos;
			   if(keyPos > 0.0f)
		          cloneP->m_header[2] = 1.0f/keyPos;//start time
               //interpolate
			   cloneP->m_startColorAlpha[0] = (hPtr->m_startColorAlpha[0] + pPrev->m_startColorAlpha[0])/2.0f;
			   cloneP->m_startColorAlpha[1] = (hPtr->m_startColorAlpha[1] + pPrev->m_startColorAlpha[1])/2.0f;
			   cloneP->m_startColorAlpha[2] = (hPtr->m_startColorAlpha[2] + pPrev->m_startColorAlpha[2])/2.0f;
			   cloneP->m_startColorAlpha[3] = (hPtr->m_startColorAlpha[3] + pPrev->m_startColorAlpha[3])/2.0f;

			   cloneP->m_startPosScale[0] = (hPtr->m_startPosScale[0] + pPrev->m_startPosScale[0])/2.0f;
			   cloneP->m_startPosScale[1] = (hPtr->m_startPosScale[1] + pPrev->m_startPosScale[1])/2.0f;
			   cloneP->m_startPosScale[2] = (hPtr->m_startPosScale[2] + pPrev->m_startPosScale[2])/2.0f;
			   cloneP->m_startPosScale[3] = (hPtr->m_startPosScale[3] + pPrev->m_startPosScale[3])/2.0f;

               InsertBefore(posAt,cloneP);
			   ResetKeyRanges();
			   return TRUE;
			}
			pPrev = hPtr;
		}
		
        return FALSE;
   }

   void ResetKeyRanges()
   {
		HprtKeyFrame * pPrev = NULL;
        for(POSITION posLoc = GetTailPosition();posLoc!= NULL;)
		{
            HprtKeyFrame * hPtr = (HprtKeyFrame *)GetPrev(posLoc);
		
			if(pPrev)
			{
		       hPtr->m_header[2] = 1.0f/(pPrev->m_header[0]-hPtr->m_header[0]);//full time/range
			}
               
			pPrev = hPtr;
		}
   }

   BOOL SetConstants(LPDIRECT3DDEVICE9 device)
   {
        m_keyFrameCount = GetCount()-1;    
		if(m_keyFrameCount < 1)
		   return FALSE;
		//loop constants
		int loopReg[4];
		loopReg[0] = m_keyFrameCount;//key frame count
        loopReg[1] = 99;//initial value of AL internal vertex register
		loopReg[2] = 3;//loop stride
		loopReg[3] = 0;//reserved
		device->SetVertexShaderConstantI(1,loopReg,1);


		//key frames
		int constantStartPoint = 99;
        
        for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
			HprtKeyFrame * hPtr = (HprtKeyFrame *)GetNext(posLoc);
			
			device->SetVertexShaderConstantF( constantStartPoint++, (float*)&hPtr->m_header, 1 );//start stop header x=range start y=range end z=range div int0 1
			device->SetVertexShaderConstantF( constantStartPoint++, (float*)&hPtr->m_startPosScale, 1 );
			device->SetVertexShaderConstantF( constantStartPoint++, (float*)&hPtr->m_startColorAlpha, 1 );

		}
		return TRUE;
   }  
};

struct particleStruct {

	double x;
	double xv;
	double ax;//directional particle system -- holds the actual render position in world space

	double y;
	double yv;
	double ay; //directional particle system -- holds the actual render position in world space
	
	double z;
	double zv;
	double az; //directional particle system -- holds the actual render position in world space
	
	D3DVECTOR camSpacePos;

	double spreadx; //directional particle system
	double spready; //directional particle system

	long lifetime;
	int   lifelength;  

	double dist;
	long startTime;
		
	D3DMATRIX directMat; //directional particle system

	D3DMATRIX worldTransform; //particle's world transformation

	BOOL b_render;		 //should render particle or not?
	BOOL b_spawnagain;   //spawn the particle more than once?

	D3DCOLORVALUE color;
};

//-----------------------------------------------------------------------------

class   EnvParticleSystem : public CObject {
	  
       
      DECLARE_SERIAL( EnvParticleSystem )
      
      
	public:
		MatrixARB abMath;

		HprtKeyFrameSystem m_keyFrames;
        CEXMeshObj * im_mesh;
		CEXMeshObj * m_vshaderMesh;//vertex shader driven particle system 
		DWORD m_systemStartTime;
		DWORD m_cycleDuration;
		D3DXVECTOR4 m_vRegisterTemp;
		DWORD m_dwTempRegister;
		DWORD m_timeRegister;
		int m_blendMode;
		float m_baseMeshSize;
		int m_textureGrid;
		float m_influence;
		CString m_textureName;
		int m_textureIndexDyn;

		CString m_systemName;
		
		ABVERTEXONE * m_virtualVertexArray;
        //used to determine whether or not to rotate the mesh
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

		void SetFade(int Start,int End, BOOL EndSpawn, D3DCOLORVALUE color, BOOL FadeTime);
		void Clone(EnvParticleSystem **clone,LPDIRECT3DDEVICE9 pD3Ddevice);

		void CreateRenderList();

        BOOL InView( int particleIndex );
			
		//functions
		void Serialize( CArchive& ar );

		 EnvParticleSystem();
		~EnvParticleSystem();
		
		//FOR DIRECTIONAL PARTICLE SYSTEMS
		void Burst(float speed, float speeddrate, float spread, float spreaddrate);
		//FOR REGULAR PARTICLE SYSTEMS
		void StopSpawning();
		void RestartSpawning();
		void OrderParticles();
		
        D3DMATRIX m_passedInMatrix;

		BOOL m_EndSpawn;
		BOOL m_IsSphere;
		BOOL m_Sort;
		BOOL m_FirstTime; //First time through render loop?
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
		
		particleStruct *m_paParticles; //pointer to an array of particles
		int m_NumParticles; //number of particles in the system

		particleStruct *m_renderParticles;
		int m_crenderParticles;
	
		//the camera coordinates
		D3DVECTOR m_camPos;

		//field of view
		float m_fov;

		//starting fade color
		D3DCOLORVALUE m_FadeColor;

		float m_zBaseDist;
		//start & end distance/time of fading
		int m_FadeStart;
		int m_FadeEnd;

		//start & end distance/time of scaling
		int m_scaleStart;
		int m_scaleEnd;		
		
		int m_ActiveParticles;
		long tickLast,tickCurrent;

		//radius of generation field
		float m_FieldRadius;
		float m_FieldHeight;
		
		//Factors used for speed
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

		//Reference used for lifetime (in milliseconds)
		int m_lifetime;

		//Number of calls to Render
		int m_rcalls;

		//DIRECTIONAL PARTICLE FIELD
		float m_sprdveloc;
		float m_partveloc;

		BOOL ConstructVSPMesh(float baseParticleSize,float influenceMin,float influenceMax,int textureGrid);
		int VSPRender(CStateManagementObj * renderStateObject,LPDIRECT3DDEVICE9 device,D3DVECTOR & camPos,D3DMATRIX objMat,void * textureDdatabase);
        void InitInline(LPDIRECT3DDEVICE9 device);
};

class   EnvParticleSystemList : public CObList
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
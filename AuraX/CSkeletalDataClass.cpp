#define D3D_OVERLOADS
#include "afxwin.h"
#include "afxext.h"
#include "resource.h"
#include <d3d8.h>
#include "d3dmath.h"
#include "matrixarb.h"

#include "CSkeletalDataClass.h"
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
CSkeletalDataObj::CSkeletalDataObj()
{
    m_SkeletalName             = "NONE SET";
	m_frameSpeed               = 10;
	m_ticksPerFrame            = 50;
    m_bonesList                = NULL;
	m_deformableMeshArrayV2    = NULL;
    m_deformableMeshArrayCount = 0;
	m_physiqueModCount         = 0;//defines how many def meshes
	m_firstFrame               = 0;
	m_lastFrame                = 0;
	m_frameCount               = 0;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Serialize-------------------------------------//
void CSkeletalDataObj::SafeDelete()
{
     if(m_deformableMeshArrayCount > 0)
	 {
	   if(m_deformableMeshArrayV2)
       for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
           if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit){
              m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->SafeDelete();
			  delete m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
			  m_deformableMeshArrayV2[loop].m_defMeshVisualUnit=0;
		   }
	   }
	 }
	 if(m_deformableMeshArrayV2){
	     delete [] m_deformableMeshArrayV2;
	     m_deformableMeshArrayV2=0;
	 }
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Serialize-------------------------------------//
void CSkeletalDataObj::Serialize( CArchive& ar )
{
int nullDump = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){


	   ar << m_SkeletalName
	      << m_frameSpeed
	      << m_ticksPerFrame
          << m_bonesList
	      << m_deformableMeshArrayCount
          << m_physiqueModCount
	      << m_firstFrame
          << m_lastFrame
	      << m_frameCount;

	   
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
              ar << m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
		  }    
	}
	else
	{
       ar >> m_SkeletalName
		  >> m_frameSpeed
		  >> m_ticksPerFrame
		  >> m_bonesList
		  >> m_deformableMeshArrayCount
		  >> m_physiqueModCount
	      >> m_firstFrame
	      >> m_lastFrame
		  >> m_frameCount;

       if(m_deformableMeshArrayCount > 0){
	      m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
	      for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
              ar >> m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
		  }
	   }

	}
}


IMPLEMENT_SERIAL(CSkeletalDataObj,  CObject, 0)


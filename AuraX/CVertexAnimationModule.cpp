#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CVertexAnimationModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CVertexAnimationModule::CVertexAnimationModule()
{
    m_vertexFrames      = NULL;
    m_frameCount        = 0;
    m_curFrame          = 0;
    m_targetFrame       = 1;
    m_timeBetweenFrames = 1000;
    m_looping           = TRUE;
	m_currentFrameTime  = 0;
	m_lastUpdatedFrameChngTime = GetTickCount();
}

void CVertexAnimationModule::SafeDelete()
{
	   for(int i=0;i<m_frameCount;i++)
	   {  
		   if(m_vertexFrames[i].m_vertexs)
		      delete [] m_vertexFrames[i].m_vertexs;
	   } 
	   if(m_vertexFrames)
	   {
          delete [] m_vertexFrames;
		  m_vertexFrames = 0;
	   }

	   m_frameCount        = 0;
       m_curFrame          = 0;
       m_targetFrame       = 1;
}

void CVertexAnimationModule::AddKeyFrame(CAdvancedVertexObj * vertexData)
{
     VertexPositionsFrame * m_newFrames = new VertexPositionsFrame[m_frameCount+1];

     int count=0;
	 for(count=0;count<m_frameCount;count++)
	 {
         m_newFrames[count].m_vertexCount = m_vertexFrames[count].m_vertexCount;
		 m_newFrames[count].m_vertexs = new ABVERTEX0L[m_newFrames[count].m_vertexCount];
		 CopyMemory(m_newFrames[count].m_vertexs,m_vertexFrames[count].m_vertexs,sizeof(ABVERTEX0L)*m_vertexFrames[count].m_vertexCount);
	 }
	 for(int i=0;i<m_frameCount;i++)
	 {  
		   if(m_vertexFrames[i].m_vertexs)
		      delete [] m_vertexFrames[i].m_vertexs;
	 } 
	 if(m_vertexFrames)
	 {
          delete [] m_vertexFrames;
		  m_vertexFrames = 0;
	 }
     //add new one

     m_newFrames[count].m_vertexCount = vertexData->m_vertexCount;
	 m_newFrames[count].m_vertexs = new ABVERTEX0L[m_newFrames[count].m_vertexCount];
	 vertexData->Fill0UVArrayData(m_newFrames[count].m_vertexs);

	 m_frameCount++;

     m_vertexFrames = m_newFrames;
}


void CVertexAnimationModule::ApplyAnimation(CAdvancedVertexObj * vertexData,LPDIRECT3DDEVICE9 pd3dDevice)
{
     
     if(m_frameCount == 0)
	    return;
     
	
	long timeDiff = GetTickCount() - m_lastUpdatedFrameChngTime;
     if(timeDiff > m_timeBetweenFrames)
	 {
	    timeDiff = 0;
        m_curFrame = m_targetFrame;
        m_targetFrame++;
		m_lastUpdatedFrameChngTime = GetTickCount();
		if(m_targetFrame >= m_frameCount)
		{
           m_targetFrame = 0;
		}
	 }

     for(int vCount = 0;vCount<m_vertexFrames[m_curFrame].m_vertexCount;vCount++)
	 {

		 vertexData->SetPositionXYZNXNYNZMEM(vCount,
                                          GL_matrixFunctions.InterpolateFloats(m_vertexFrames[m_curFrame].m_vertexs[vCount].x,m_vertexFrames[m_targetFrame].m_vertexs[vCount].x, m_timeBetweenFrames,timeDiff),
                                          GL_matrixFunctions.InterpolateFloats(m_vertexFrames[m_curFrame].m_vertexs[vCount].y,m_vertexFrames[m_targetFrame].m_vertexs[vCount].y, m_timeBetweenFrames,timeDiff),
										  GL_matrixFunctions.InterpolateFloats(m_vertexFrames[m_curFrame].m_vertexs[vCount].z,m_vertexFrames[m_targetFrame].m_vertexs[vCount].z, m_timeBetweenFrames,timeDiff),
										  GL_matrixFunctions.InterpolateFloats(m_vertexFrames[m_curFrame].m_vertexs[vCount].nx,m_vertexFrames[m_targetFrame].m_vertexs[vCount].nx, m_timeBetweenFrames,timeDiff),
										  GL_matrixFunctions.InterpolateFloats(m_vertexFrames[m_curFrame].m_vertexs[vCount].ny,m_vertexFrames[m_targetFrame].m_vertexs[vCount].ny, m_timeBetweenFrames,timeDiff),
										  GL_matrixFunctions.InterpolateFloats(m_vertexFrames[m_curFrame].m_vertexs[vCount].nz,m_vertexFrames[m_targetFrame].m_vertexs[vCount].nz, m_timeBetweenFrames,timeDiff));

	 }
	 vertexData->ReinitializeDxVbuffersPositionOnly(pd3dDevice);
}


void CVertexAnimationModule::Clone(CVertexAnimationModule ** clone)
{
     CVertexAnimationModule * copyLocal = new CVertexAnimationModule();


    
    copyLocal->m_frameCount        = m_frameCount;
    copyLocal->m_curFrame          = m_curFrame;
    copyLocal->m_targetFrame       = m_targetFrame;
    copyLocal->m_timeBetweenFrames = m_timeBetweenFrames;
    copyLocal->m_looping           = m_looping;


    if(m_frameCount > 0)
       copyLocal->m_vertexFrames = new VertexPositionsFrame[m_frameCount];

	   for(int i=0;i<m_frameCount;i++)
	   {  
		   if(m_vertexFrames[i].m_vertexCount>0)
		      copyLocal->m_vertexFrames[i].m_vertexs = new ABVERTEX0L[m_vertexFrames[i].m_vertexCount];

           for(int vCount = 0;vCount<m_vertexFrames[i].m_vertexCount;vCount++)
		   {
               copyLocal->m_vertexFrames[i].m_vertexs[vCount].x = m_vertexFrames[i].m_vertexs[vCount].x;
			   copyLocal->m_vertexFrames[i].m_vertexs[vCount].y = m_vertexFrames[i].m_vertexs[vCount].y;
			   copyLocal->m_vertexFrames[i].m_vertexs[vCount].z = m_vertexFrames[i].m_vertexs[vCount].z;
			   copyLocal->m_vertexFrames[i].m_vertexs[vCount].nx = m_vertexFrames[i].m_vertexs[vCount].nx;
			   copyLocal->m_vertexFrames[i].m_vertexs[vCount].ny = m_vertexFrames[i].m_vertexs[vCount].ny;
			   copyLocal->m_vertexFrames[i].m_vertexs[vCount].nz = m_vertexFrames[i].m_vertexs[vCount].nz;
		   }
	   } 


}

void CVertexAnimationModuleList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CVertexAnimationModule * spnPtr = (CVertexAnimationModule *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CVertexAnimationModule::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_frameCount << m_curFrame << m_targetFrame << m_timeBetweenFrames << m_looping;


	   for(int i=0;i<m_frameCount;i++)
	   {  
		   ar << m_vertexFrames[i].m_vertexCount;
           for(int vCount = 0;vCount<m_vertexFrames[i].m_vertexCount;vCount++)
		   {
               ar << m_vertexFrames[i].m_vertexs[vCount].x
			      << m_vertexFrames[i].m_vertexs[vCount].y
				  << m_vertexFrames[i].m_vertexs[vCount].z
				  << m_vertexFrames[i].m_vertexs[vCount].nx
				  << m_vertexFrames[i].m_vertexs[vCount].ny
				  << m_vertexFrames[i].m_vertexs[vCount].nz;
		   }
	   }
	}
	else
	{
       ar >> m_frameCount >> m_curFrame >> m_targetFrame >> m_timeBetweenFrames >> m_looping;



       if(m_frameCount > 0)
          m_vertexFrames = new VertexPositionsFrame[m_frameCount];

	   for(int i=0;i<m_frameCount;i++)
	   {  
		   ar >> m_vertexFrames[i].m_vertexCount;
		   if(m_vertexFrames[i].m_vertexCount>0)
		      m_vertexFrames[i].m_vertexs = new ABVERTEX0L[m_vertexFrames[i].m_vertexCount];

           for(int vCount = 0;vCount<m_vertexFrames[i].m_vertexCount;vCount++)
		   {
               ar >> m_vertexFrames[i].m_vertexs[vCount].x
			      >> m_vertexFrames[i].m_vertexs[vCount].y
				  >> m_vertexFrames[i].m_vertexs[vCount].z
				  >> m_vertexFrames[i].m_vertexs[vCount].nx
				  >> m_vertexFrames[i].m_vertexs[vCount].ny
				  >> m_vertexFrames[i].m_vertexs[vCount].nz;
		   }
	   } 
	   m_lastUpdatedFrameChngTime = GetTickCount();
	}
}     



IMPLEMENT_SERIAL(CVertexAnimationModule, CObject, 0)
IMPLEMENT_SERIAL(CVertexAnimationModuleList, CObList, 0)


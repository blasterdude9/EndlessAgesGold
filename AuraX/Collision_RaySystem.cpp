#include "StdAfx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include "collision_raysystem.h"
#include "CPhysicsEmuClass.h"
#include "CTextReadoutClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------------//
CShockClass::CShockClass(float maxY,float minY,float tightness,D3DVECTOR origPosition)
{
	    m_tightness		= tightness;
		m_maxY			= maxY;
		m_minY			= minY;
	
	    CopyMemory(&m_originPosition,&origPosition,sizeof(D3DVECTOR));
		CopyMemory(&m_maxCompressPosition,&origPosition,sizeof(D3DVECTOR));
		CopyMemory(&m_maxUnCompressPosition,&origPosition,sizeof(D3DVECTOR));
		CopyMemory(&m_currentPosition,&origPosition,sizeof(D3DVECTOR));
		
		m_maxCompressPosition.y += m_maxY;
		m_maxUnCompressPosition.y += minY;

		m_currentPosition = m_maxUnCompressPosition;
		
}
//--------------------------------------------------------------------------------------//
CShockClass::~CShockClass(void)
{
  
}
//--------------------------------------------------------------------------------------//
CWheelClass::CWheelClass(void)
{
	
}
//--------------------------------------------------------------------------------------//
CWheelClass::CWheelClass(int axleIndex,float radius,CFrameObj * parentFrame)
{
   m_axleIndex			= axleIndex;
   m_radius				= radius;
   m_wheelBase          = 2.0f;
   m_frame = new CFrameObj(NULL);
   m_frame->SetParent(parentFrame);
   m_pivotFrameRef		= NULL;
   m_wheelFrameRef		= NULL;
   m_shock				= NULL;
   m_wheelHolder		= NULL;

   m_lengthBetweenFBWheels				= 1.0f;
   m_currentSteeringRotation			= 0.0f;
   m_steeringRotationMax                = 0.0f;
   m_canSteerRotate						= false;
   //m_shock         = new CShockClass(1.0f,-1.0f,.1f,

}
//--------------------------------------------------------------------------------------//
CWheelClass::~CWheelClass(void)
{
   if(m_frame)
   {
      m_frame->SafeDelete();
	  delete m_frame;
	  m_frame = 0;
   }

   if(m_wheelHolder)
   {
      m_wheelHolder->SafeDelete();
	  delete m_wheelHolder;
	  m_wheelHolder = 0;
   }
}
//--------------------------------------------------------------------------------------//
CCollision_Ray::CCollision_Ray(void)
{
	Reset();
	m_wheel = NULL;
	m_indexRef = 0;
}
//--------------------------------------------------------------------------------------//
void CCollision_Ray::Reset()
{
    //ZeroMemory(&m_startPoint,sizeof(D3DVECTOR));
    m_startPoint.x = .001f;
	m_startPoint.y = .002f;
	m_startPoint.z = .003f;
	//ZeroMemory(&m_endPoint,sizeof(D3DVECTOR));
	m_endPoint.x = .0035f;
	m_endPoint.y = .0021f;
	m_endPoint.z = .0011f;
	ZeroMemory(&m_dynStartPoint,sizeof(D3DVECTOR));
	ZeroMemory(&m_dynEndPoint,sizeof(D3DVECTOR));
    ZeroMemory(&m_lastIntersect,sizeof(D3DVECTOR));
	ZeroMemory(&m_prevIntersect,sizeof(D3DVECTOR));
	ZeroMemory(&m_lastNormal,sizeof(D3DVECTOR));
	m_depth = 0.0f;
	
	m_pivotRef = NULL;
	
	
    m_collidedThisRound				= false;
	m_collidedLastRound             = false;
	m_directionalGen = 0;
	m_lastRotation = 0;
	m_stopVelVector = 0;
	
}
//--------------------------------------------------------------------------------------//
D3DVECTOR CCollision_Ray::GetCorrectivePosition(MatrixARB * matLib, int calctype)
{
	if(m_collidedThisRound == true)
	{
	  
		float modDepth = m_depth;
		D3DVECTOR offset;
		ZeroMemory(&offset,sizeof(offset));
			   
		if(calctype == 0)
		{
			offset.x = this->m_lastIntersect.x - m_dynEndPoint.x;
			offset.y = this->m_lastIntersect.y - m_dynEndPoint.y;
			offset.z = this->m_lastIntersect.z - m_dynEndPoint.z;
			offset.x += m_dynEndPoint.x;
			offset.y += m_dynEndPoint.y;
			offset.z += m_dynEndPoint.z;

			offset.y -= .01f;

			//offset.y = offset.y - .01f;
		}
		else if(calctype == 1)
		{
			offset = matLib->GetAdjustVector(modDepth,m_lastNormal);
			offset.x += m_dynEndPoint.x;
			offset.y += m_dynEndPoint.y;
			offset.z += m_dynEndPoint.z;

			offset.y -= .01f;
		}
		return offset;
	}
	return m_dynEndPoint;
}
//--------------------------------------------------------------------------------------//
CCollision_Ray::~CCollision_Ray(void)
{
    if(m_wheel)
	{
       delete m_wheel;
	   m_wheel = 0;
	}
}
//--------------------------------------------------------------------------------------//
void CCollision_Ray::ShockCallback()
{ 
     if(m_wheel == NULL)
	    return;

	 if(m_wheel->m_shock == NULL)
	    return;

     if(this->m_collidedThisRound == true)
	 {//handle collision state
        if(this->m_depth < .2f)
		{//within shock tolerence	
			if(m_wheel->m_shock->m_currentPosition.y < m_wheel->m_shock->m_originPosition.y)
			{
				m_wheel->m_shock->m_currentPosition.y += m_wheel->m_shock->m_tightness;
				if(m_wheel->m_shock->m_currentPosition.y > m_wheel->m_shock->m_originPosition.y)
					m_wheel->m_shock->m_currentPosition.y = m_wheel->m_shock->m_originPosition.y;


			}
			else if(m_wheel->m_shock->m_currentPosition.y > m_wheel->m_shock->m_originPosition.y)
			{
                m_wheel->m_shock->m_currentPosition.y -= m_wheel->m_shock->m_tightness;
				if(m_wheel->m_shock->m_currentPosition.y < m_wheel->m_shock->m_originPosition.y)
					m_wheel->m_shock->m_currentPosition.y = m_wheel->m_shock->m_originPosition.y;
			}
		}//end within shock tolerence
		else
		{
			m_wheel->m_shock->m_currentPosition.y += (this->m_depth - .02f);
			if(m_wheel->m_shock->m_currentPosition.y > m_wheel->m_shock->m_maxCompressPosition.y)
				m_wheel->m_shock->m_currentPosition.y = m_wheel->m_shock->m_maxCompressPosition.y;
		}
	 }//end handle collision state
	 else
	 {//handle non- collision state
		if(m_lastRotation > 0.0f)
		{
            m_lastRotation = m_lastRotation * .98f;
			m_wheel->m_wheelFrameRef->RotateOnAxisXBasedOnDistCirc(m_wheel->m_radius*2.0f,m_lastRotation);
		}
		if(m_wheel)
		if(m_wheel->m_wheelFrameRef)
		{
			
            //expand shock if possible
            
			m_wheel->m_shock->m_currentPosition.y -= m_wheel->m_shock->m_tightness;
			if(m_wheel->m_shock->m_currentPosition.y < m_wheel->m_shock->m_maxUnCompressPosition.y)
				m_wheel->m_shock->m_currentPosition.y = m_wheel->m_shock->m_maxUnCompressPosition.y;

			/*entLoc->m_collisionRaySys->RecalculateByIndex(  cloop,entLoc->List_baseFrame, entLoc->List_skeletalObject,matLib,&position,&entLoc->List_movementDB->List_lastPosition);
			//check collision
			D3DVECTOR intersect;
            if(collisionObject->GetClosesIntersectionUseSeg(
															m_dynStartPoint,
															m_dynEndPoint,
															m_dynStartPoint,
															m_dynEndPoint,
															&intersect.x,
															&intersect.y,
															&intersect.z,
															&indexOfObject,
															&latestNormal,
															true,
															dist,
															m_directionalGen
															)==TRUE)
			{
                m_wheel->m_shock->m_currentPosition.y += (dist - .02f);
			}*/
		}
	 }//end handle non collisoion state
}
//--------------------------------------------------------------------------------------//
void CCollision_Ray::HandleTireRotation(MatrixARB * matLib,D3DMATRIX & baseFrameMatrix)
{
	    if(m_wheel)
        if(m_collidedLastRound == true)
		{//handle tire rotation
			D3DMATRIX bottomLine,invMatrix;
			matLib->fastMatrixMultiply(bottomLine,m_wheel->m_wheelHolder->List_frameMatrix,baseFrameMatrix);
			matLib->D3DMath_MatrixInvert(invMatrix,bottomLine);
			
            D3DMATRIX rWheelMat;
			m_wheel->m_wheelFrameRef->GetTransform(NULL,&rWheelMat);
			D3DVECTOR localizedVectPrev = matLib->GetLocalPositionFromMatrixView2(invMatrix,bottomLine,m_prevIntersect);
			D3DVECTOR localizedVect		= matLib->GetLocalPositionFromMatrixView2(invMatrix,bottomLine,m_lastIntersect);
			//D3DVECTOR localizedVectPrev = matLib->GetLocalPositionFromMatrixView(m_rays[rayIndex].m_wheel->m_wheelHolder->List_frameMatrix,m_rays[rayIndex].m_prevIntersect);
			//D3DVECTOR localizedVect		= matLib->GetLocalPositionFromMatrixView(m_rays[rayIndex].m_wheel->m_wheelHolder->List_frameMatrix,m_rays[rayIndex].m_lastIntersect);
			m_lastRotation = localizedVectPrev.z - localizedVect.z;
			m_wheel->m_wheelFrameRef->RotateOnAxisXBasedOnDistCirc(m_wheel->m_radius*2.0f,m_lastRotation);
		}//end handle tire rotation
}
//----------------------------------------------------------------------------------------//
void CCollision_Ray::HandleTireSteeringRotation(MatrixARB * matLib)
{
	    if(m_wheel)
		if(m_wheel->m_wheelHolder)
		if(m_wheel->m_canSteerRotate == true)
		{//handle tire rotation
			D3DMATRIX rotation;
			matLib->Rotate(0.0f,m_wheel->m_currentSteeringRotation,0.0f,rotation.m);
			rotation._41 = m_wheel->m_wheelHolder->List_frameMatrix._41;
			rotation._42 = m_wheel->m_wheelHolder->List_frameMatrix._42;
			rotation._43 = m_wheel->m_wheelHolder->List_frameMatrix._43;
			m_wheel->m_wheelHolder->AddTransformReplace(rotation);
		}//end handle tire rotation
}
//--------------------------------------------------------------------------------------//
CCollision_RaySystem::CCollision_RaySystem(void)
{
	m_rays								= NULL;
	m_rayCount							= 0;
	m_staticWorldCollisionChecked		= 0;
	m_worldCorrected                    = 0;
	m_specialOperation                  = 0;
	m_elevationBasis                    = 0.5f;
	m_type                              = 0;
	ZeroMemory(&m_centerMassPosition,sizeof(D3DVECTOR));
	ZeroMemory(&m_lastCenterMassPosition,sizeof(D3DVECTOR));
	m_stamp  = GetTickCount();
}
//--------------------------------------------------------------------------------------//
CCollision_RaySystem::CCollision_RaySystem(CSkeletonObject * skeletonPtr,int type,MatrixARB * matLib)
{
	
	m_rays								= NULL;
	m_rayCount							= 0;
	m_staticWorldCollisionChecked		= 0;
	m_centerOfMassCollided				= false;
	m_type                              = type;
	m_elevationBasis                    = 0.5f;
	ZeroMemory(&m_lastCenterMassPosition,sizeof(D3DVECTOR));
    //test type as walking
    //6 rays for this type canned
	m_rayCount = 0;
	switch(m_type)
	{
	   case 0:
	      m_rayCount = 5;
	      if(m_rayCount > 0)
	         m_rays = new CCollision_Ray[m_rayCount];
          
	   break;
	   case 1:
	      m_rayCount = 2;
	      if(m_rayCount > 0)
	         m_rays = new CCollision_Ray[m_rayCount];
          
	   break;
	   case 2:
	      m_rayCount = 6;
	      if(m_rayCount > 0)
	         m_rays = new CCollision_Ray[m_rayCount];
          
	   break;
	   case 3:
	      m_rayCount = 1;
	      if(m_rayCount > 0)
	         m_rays = new CCollision_Ray[m_rayCount];
          
	   break;
	   case 4://4 point collision setup
	      m_rayCount = 9;
	      if(m_rayCount > 0)
		  {
	          m_rays = new CCollision_Ray[m_rayCount];
		      //find axles and assign them if avail
  
			    int wTrace = 0;
				for(int loop=0;loop<4;loop++)
				{//generate base wheels
					//CBoneObject * axlePtr = NULL;
					CBoneObject * wheelPtr = NULL;
				    //if(skeletonPtr->GetBoneByName("axle"+ConvToString(loop+1),&axlePtr))
					if(skeletonPtr->GetBoneByName("wheel"+ConvToString(wTrace+1),&wheelPtr))
					{//create wheel
						float wheelRadius = 2.0f;
                        if(wheelPtr->m_hiarchyMesh)
						{//get wheel radius
						   CHiarcleVisObj * hvis = wheelPtr->m_hiarchyMesh->GetByIndex(0);
						   if(hvis)
                              wheelRadius = (hvis->m_mesh->List_boundingBox.maxY - hvis->m_mesh->List_boundingBox.minY)/2.0f;  
						}//end get wheel radius
						if(m_rays[loop].m_wheel)
							delete m_rays[loop].m_wheel;
						D3DVECTOR newPos;

						//axlePtr->m_boneFrame->GetPosition(NULL,&newPos);
						//mod code
						/*wheelPtr->m_boneFrame->GetPosition(NULL,&newPos);
                        CFrameObj * axleFrame = new CFrameObj(NULL);
						axleFrame->SetPosition(NULL,newPos);*/
						//end mod code
						wheelPtr->m_boneFrame->GetPosition(NULL,&newPos);
						
						m_rays[loop].m_wheel = 0;
						m_rays[loop].m_wheel = new CWheelClass(loop,wheelRadius,NULL/*axlePtr->m_boneFrame*/);
						m_rays[loop].m_wheel->m_wheelHolder = new CFrameObj(NULL);
						m_rays[loop].m_wheel->m_wheelHolder->SetPosition(NULL,newPos);
						m_rays[loop].m_wheel->m_wheelFrameRef = wheelPtr->m_boneFrame;
						m_rays[loop].m_wheel->m_wheelFrameRef->SetParent(m_rays[loop].m_wheel->m_wheelHolder);

						if(m_rays[loop].m_wheel->m_shock)
						{
                           delete m_rays[loop].m_wheel->m_shock;
						   m_rays[loop].m_wheel->m_shock = 0;
						}
						m_rays[loop].m_wheel->m_shock         = new CShockClass(0.5f,-0.5f,.01f,newPos);

						newPos.y -= wheelRadius;
						m_rays[loop].m_wheel->m_frame->SetPosition(NULL,newPos);
						
					}//end create wheel
					wTrace++;
				}//end generate base wheels
                //assign pivots for collision correctives
				int offset = 0;
				if(m_rays[offset+0].m_wheel)
					if(m_rays[offset+1].m_wheel)
						if(m_rays[offset+2].m_wheel)
							if(m_rays[offset+3].m_wheel)
							{ 
                               
                               m_rays[offset+0].m_wheel->m_pivotFrameRef = m_rays[offset+3].m_wheel->m_frame;
							   m_rays[offset+1].m_wheel->m_pivotFrameRef = m_rays[offset+2].m_wheel->m_frame;
							   m_rays[offset+2].m_wheel->m_pivotFrameRef = m_rays[offset+1].m_wheel->m_frame;
							   m_rays[offset+3].m_wheel->m_pivotFrameRef = m_rays[offset+0].m_wheel->m_frame;

							   m_rays[offset+0].m_crossPivot	= &m_rays[offset+1];
							   m_rays[offset+0].m_dirPivot		= &m_rays[offset+2];
							   //m_rays[0].m_xPivot       = &m_rays[2];
							   m_rays[offset+0].m_xPivot       = &m_rays[offset+3];
							   m_rays[offset+0].m_indexRef     = offset+0;
							   m_rays[offset+0].m_wheel->m_lengthBetweenFBWheels = fabs(m_rays[offset+0].m_wheel->m_frame->List_frameMatrix._43 - m_rays[offset+2].m_wheel->m_frame->List_frameMatrix._43);
							   m_rays[offset+0].m_wheel->m_canSteerRotate = true;
							   m_rays[offset+0].m_wheel->m_steeringRotationMax = 60;
							   


							   m_rays[offset+1].m_crossPivot	= &m_rays[offset+0];
							   m_rays[offset+1].m_dirPivot		= &m_rays[offset+3];
							   //m_rays[1].m_xPivot       = &m_rays[3];
							   m_rays[offset+1].m_xPivot       = &m_rays[offset+2];
							   m_rays[offset+1].m_indexRef     = offset+1;
							   m_rays[offset+1].m_wheel->m_lengthBetweenFBWheels = m_rays[offset+0].m_wheel->m_lengthBetweenFBWheels;
							   m_rays[offset+1].m_wheel->m_canSteerRotate = true;
							   m_rays[offset+1].m_wheel->m_steeringRotationMax = 60;

							   m_rays[offset+2].m_crossPivot	= &m_rays[offset+3];
							   m_rays[offset+2].m_dirPivot		= &m_rays[offset+0];
							   //m_rays[2].m_xPivot       = &m_rays[0];
							   m_rays[offset+2].m_xPivot       = &m_rays[offset+1];
							   m_rays[offset+2].m_indexRef     = offset+2;

							   m_rays[offset+3].m_crossPivot	= &m_rays[offset+2];
							   m_rays[offset+3].m_dirPivot		= &m_rays[offset+1];
							   //m_rays[3].m_xPivot		= &m_rays[1];
							   m_rays[offset+3].m_xPivot       = &m_rays[offset+0];
							   m_rays[offset+3].m_indexRef     = offset+3;
							  
							   m_rays[offset+0].m_pivotRef = &m_rays[offset+3];
							   m_rays[offset+1].m_pivotRef = &m_rays[offset+2];
							   m_rays[offset+2].m_pivotRef = &m_rays[offset+1];
							   m_rays[offset+3].m_pivotRef = &m_rays[offset+0];

							   CrossOrientate(m_rays[offset+0].m_wheel->m_frame,m_rays[offset+0].m_wheel->m_pivotFrameRef,matLib);
							   CrossOrientate(m_rays[offset+1].m_wheel->m_frame,m_rays[offset+1].m_wheel->m_pivotFrameRef,matLib);
							   CrossOrientate(m_rays[offset+2].m_wheel->m_frame,m_rays[offset+2].m_wheel->m_pivotFrameRef,matLib);
							   CrossOrientate(m_rays[offset+3].m_wheel->m_frame,m_rays[offset+3].m_wheel->m_pivotFrameRef,matLib);
							   


							}

		  }
	   break;

	}

	
	//set up static Rays
	//UpdateRays(skeletonPtr);
	
}

//-----converts a long into string-----------------------------//
void CCollision_RaySystem::CrossOrientate(CFrameObj * baseFrame, CFrameObj * pivotFrame,MatrixARB * matLib)
{
	D3DVECTOR upVectTemp,dirTemp;
    upVectTemp.x = 0.0f;
	upVectTemp.y = 1.0f;
	upVectTemp.z = 0.0f;

	dirTemp.x = pivotFrame->List_frameMatrix._41 - baseFrame->List_frameMatrix._41;
	dirTemp.y = pivotFrame->List_frameMatrix._42 - baseFrame->List_frameMatrix._42;
	dirTemp.z = pivotFrame->List_frameMatrix._43 - baseFrame->List_frameMatrix._43;
	dirTemp = matLib->Normalize(dirTemp);
	baseFrame->SetOrientation(dirTemp,upVectTemp);
}
//-----converts a long into string-----------------------------//
CString CCollision_RaySystem::ConvToString(int number)
{    
              
	          CString temp;
			  temp.Format("%i",number);
	          /*if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);*/
		
			  return temp;
}
//--------------------------------------------------------------------------------------//
void CCollision_RaySystem::UpdateRays(CSkeletonObject * skeletonPtr)
{
     switch(m_type)
	 {
	     case 0://walking type 6 rays adv users
            //no definitions available use box for reference
			m_elevationBasis = .7f;
		    //leg ray
			m_rays[0].Reset();
          
            //m_centerMassPosition.x;
	        //m_rays[0].m_startPoint.y			= skeletonPtr->m_box.maxY * (m_elevationBasis + .02f);
			if(skeletonPtr->m_box.maxY < 3.0f)
              m_rays[0].m_startPoint.y			= 3.0f * (m_elevationBasis + .02f);
			else
	           m_rays[0].m_startPoint.y			= skeletonPtr->m_box.maxY * (m_elevationBasis + .02f);
			//shoulder ray 1 + X
			m_rays[1].Reset();
	        m_rays[1].m_startPoint.y			= skeletonPtr->m_box.maxY * m_elevationBasis;
			m_rays[1].m_endPoint.y				= skeletonPtr->m_box.maxY * m_elevationBasis;
			m_rays[1].m_endPoint.x				= skeletonPtr->m_box.maxX;
			//shoulder ray 2 - X
			m_rays[2].Reset();
			m_rays[2].m_startPoint.y			= skeletonPtr->m_box.maxY * m_elevationBasis;
	        m_rays[2].m_endPoint.y			    = skeletonPtr->m_box.maxY * m_elevationBasis;
			m_rays[2].m_endPoint.x              = -skeletonPtr->m_box.maxX;
			//1 mid foward ray
			m_rays[3].Reset();
			m_rays[3].m_startPoint.y			= skeletonPtr->m_box.maxY * m_elevationBasis;
	        m_rays[3].m_endPoint.y			    = skeletonPtr->m_box.maxY * m_elevationBasis;
			m_rays[3].m_endPoint.z              = -skeletonPtr->m_box.maxZ;
			if(m_rays[3].m_endPoint.z > -2.0f)
               m_rays[3].m_endPoint.z = -2.0f;//clear camera
			m_rays[3].m_directionalGen          = 1;
			//1 head upward ray
			m_rays[4].Reset();
			m_rays[4].m_startPoint.y			= skeletonPtr->m_box.maxY * (m_elevationBasis - .02f);
	        m_rays[4].m_endPoint.y			    = skeletonPtr->m_box.maxY;
			m_rays[4].m_stopVelVector           = 1;
			
			m_specialOperation                  = 1;

		 break;
		 case 1:
		    //no definitions available use box for reference
		    
		    m_elevationBasis = .7f;
            //leg ray
			m_rays[0].Reset();

            //m_centerMassPosition.x;
			if(skeletonPtr->m_box.maxY < 3.0f)
              m_rays[0].m_startPoint.y			= 3.0f * (m_elevationBasis + .02f);
			else
	           m_rays[0].m_startPoint.y			= skeletonPtr->m_box.maxY * (m_elevationBasis + .02f);
			//1 mid foward ray
			m_rays[1].Reset();
			
			m_rays[1].m_startPoint.y			= skeletonPtr->m_box.maxY * m_elevationBasis;
	        m_rays[1].m_endPoint.y			    = skeletonPtr->m_box.maxY * m_elevationBasis;
			m_rays[1].m_endPoint.z              = -skeletonPtr->m_box.maxZ;
			if(m_rays[1].m_endPoint.z > -2.0f)
               m_rays[1].m_endPoint.z = -2.0f;//clear camera
			m_rays[1].m_directionalGen          = 1;
			
			m_specialOperation                  = 1;

		 break;
		 case 2:
		    //leg ray
			//no definitions available use box for reference
            m_elevationBasis = .7f;
		    //leg ray
			m_rays[0].Reset();
			m_rays[0].m_startPoint.y            = (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY)* (m_elevationBasis + .02f);
	        m_rays[0].m_endPoint.y			    = skeletonPtr->m_box.minY;
			//shoulder ray 1 + X
			m_rays[1].Reset();
	        m_rays[1].m_startPoint.y			= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[1].m_endPoint.y				= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[1].m_endPoint.x				= skeletonPtr->m_box.maxX;
			//shoulder ray 2 - X
			m_rays[2].Reset();
	        m_rays[2].m_startPoint.y			= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[2].m_endPoint.y				= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[2].m_endPoint.x				= -skeletonPtr->m_box.maxX;
			//1 mid foward ray
			m_rays[3].Reset();
			m_rays[3].m_startPoint.y			= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[3].m_endPoint.y				= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[3].m_endPoint.z              = -skeletonPtr->m_box.maxZ;
			if(m_rays[3].m_endPoint.z > -2.0f)
			   m_rays[3].m_endPoint.z = -2.0f;
			//1 head upward ray
			m_rays[4].Reset();
			m_rays[4].m_startPoint.y			= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * (m_elevationBasis - .02f);
	        m_rays[4].m_endPoint.y			    = skeletonPtr->m_box.maxY;
			m_rays[4].m_stopVelVector           = 1;
			//1 mid backward ray
			m_rays[5].Reset();
			m_rays[5].m_startPoint.y			= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[5].m_endPoint.y				= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[5].m_endPoint.z              = skeletonPtr->m_box.maxZ;
			if(m_rays[5].m_endPoint.z < 2.0f)
			   m_rays[5].m_endPoint.z = 2.0f;

			m_specialOperation                  = 1;

		 break;
		 case 3:
		    m_elevationBasis = .5f;
		    //no definitions available use box for reference
			//1 mid foward ray
			m_rays[0].Reset();
			//m_rays[0].m_startPoint.y			= skeletonPtr->m_box.maxY * m_elevationBasis;
	        //m_rays[0].m_endPoint.y			    = skeletonPtr->m_box.maxY * m_elevationBasis;
			m_rays[0].m_endPoint.z              = skeletonPtr->m_box.maxZ;
			if(m_rays[0].m_endPoint.z >  -2.0f)
               m_rays[0].m_endPoint.z =  -2.0f;//clear camera
			m_rays[0].m_directionalGen          = 2;
			
			m_specialOperation                  = 2;

		 break;
		 case 4://4 point collision
			 m_specialOperation                  = 0;
			 m_elevationBasis = .8f;
			 for(int loop=0;loop<4;loop++)
			 {
				 if(m_rays[loop].m_wheel)
				 {
                    m_rays[loop].m_wheel->m_frame->GetPosition(NULL,&m_rays[loop].m_endPoint);
                    CopyMemory(&m_rays[loop].m_startPoint,&m_rays[loop].m_endPoint,sizeof(D3DVECTOR));
					//m_rays[loop].m_startPoint.y += m_rays[loop].m_wheel->m_radius*2;//option 1 straight up
					
					
					m_rays[loop].m_startPoint.y = skeletonPtr->m_box.maxY;
					
					
				    m_rays[loop].m_directionalGen          = 3;
				 }
			 }
             //define car body

			 //----foward ray----//
			 m_rays[4].Reset();
			 m_rays[4].m_startPoint.y			= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			 m_rays[4].m_endPoint.y				= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			 m_rays[4].m_endPoint.z              = -fabs(skeletonPtr->m_box.maxZ+4);
			 m_rays[4].m_directionalGen          = 4;
			 if(m_rays[4].m_endPoint.z > -2.0f)
			    m_rays[4].m_endPoint.z = -2.0f;
			 //shoulder ray 1 + X
			m_rays[5].Reset();
			m_rays[5].m_startPoint.y			= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[5].m_endPoint.y				= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[5].m_endPoint.x				= skeletonPtr->m_box.maxX;
			m_rays[5].m_directionalGen          = 4;
			//shoulder ray 2 - X
			m_rays[6].Reset();
			m_rays[6].m_startPoint.y			= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[6].m_endPoint.y				= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[6].m_endPoint.x				= -skeletonPtr->m_box.maxX;
			m_rays[6].m_directionalGen          = 4;
			//1 head upward ray
			m_rays[7].Reset();
			m_rays[7].m_startPoint.y			= skeletonPtr->m_box.maxY * (m_elevationBasis - .02f);
			m_rays[7].m_endPoint.y			    = skeletonPtr->m_box.maxY;
			m_rays[7].m_stopVelVector           = 1;
			m_rays[7].m_directionalGen          = 4;
			//----foward ray----//
			m_rays[8].Reset();
			m_rays[8].m_startPoint.y			= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[8].m_endPoint.y				= (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY) * m_elevationBasis;
			m_rays[8].m_endPoint.z              = fabs(skeletonPtr->m_box.minZ);
			m_rays[8].m_directionalGen          = 4;
			if(m_rays[8].m_endPoint.z < 2.0f)
				m_rays[8].m_endPoint.z = 2.0f;


		 break;//end 4 point collision
	 }
}
//--------------------------------------------------------------------------------------//
CCollision_RaySystem::~CCollision_RaySystem(void)
{
	if(m_rays)
	{
       delete [] m_rays;
	   m_rays = 0;
	}

}
//--------------------------------------------------------------------------------------//
void CCollision_RaySystem::InitOnSpawn(CFrameObj * frame,CSkeletonObject * skeletonPtr,MatrixARB * matLib)
{
	UpdateLastPosition(frame,skeletonPtr,matLib);
	UpdateCurrentPosition(frame,skeletonPtr,matLib);
}
//--------------------------------------------------------------------------------------//
void CCollision_RaySystem::UpdateLastPosition(CFrameObj * frame,CSkeletonObject * skeletonPtr,MatrixARB * matLib)
{
	 if(m_specialOperation == 0)
	 {
       frame->GetPosition(NULL,&m_lastCenterMassPosition);
	   m_lastCenterMassPosition.y += (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY)* m_elevationBasis;
	 }
	 else if(m_specialOperation == 1)
	 {
       frame->GetPosition(NULL,&m_lastCenterMassPosition);
	   if(skeletonPtr->m_box.maxY < 3.0f)
	      m_lastCenterMassPosition.y += 3.0f * m_elevationBasis;
	   else
	      m_lastCenterMassPosition.y += skeletonPtr->m_box.maxY * m_elevationBasis;
	 }
	 else
	 {
	   frame->GetPosition(NULL,&m_lastCenterMassPosition);
	 }
}
//--------------------------------------------------------------------------------------//
void CCollision_RaySystem::UpdateCurrentPosition(CFrameObj * frame,CSkeletonObject * skeletonPtr,MatrixARB * matLib)
{
	 if(m_specialOperation == 0)
	 {
       frame->GetPosition(NULL,&m_centerMassPosition);
	   m_centerMassPosition.y += (skeletonPtr->m_box.maxY - skeletonPtr->m_box.minY)* m_elevationBasis;
	 }
	 else if(m_specialOperation == 1)
	 {
       frame->GetPosition(NULL,&m_centerMassPosition);
	   if(skeletonPtr->m_box.maxY < 3.0f)
	      m_centerMassPosition.y += 3.0f * m_elevationBasis;
	   else
	      m_centerMassPosition.y += skeletonPtr->m_box.maxY * m_elevationBasis;
	 }
	 else
	 {
	   frame->GetPosition(NULL,&m_centerMassPosition);
	 }
	
}
//--------------------------------------------------------------------------------------//
void CCollision_RaySystem::Recalculate(CFrameObj * frame, 
									   CSkeletonObject * skeletonPtr,
									   MatrixARB * matLib)
{
    UpdateRays(skeletonPtr);//update rays again primarily for hight perameters
    //get orientation
	D3DMATRIX matrix;
    frame->GetTransform(NULL,&matrix);
    for(int loop=0;loop<m_rayCount;loop++)
	{
        m_rays[loop].m_dynStartPoint	= matLib->UpdatePolygonVertex(matrix.m,m_rays[loop].m_startPoint);
		m_rays[loop].m_dynEndPoint		= matLib->UpdatePolygonVertex(matrix.m,m_rays[loop].m_endPoint);
	}
}
//--------------------------------------------------------------------------------------//
bool CCollision_RaySystem::RecalculateByIndex(  int index,
												CFrameObj * frame, 
												CSkeletonObject * skeletonPtr,
												MatrixARB * matLib,
												D3DVECTOR * currentPosition,
												D3DVECTOR * lastPosition)
{
    //UpdateRays(skeletonPtr);//update rays again primarily for hight perameters
    //get orientation
	if(m_rays[index].m_directionalGen == 0)
	{
		D3DMATRIX matrix;
		frame->GetTransform(NULL,&matrix);
		m_rays[index].m_dynStartPoint	= matLib->UpdatePolygonVertex(matrix.m,m_rays[index].m_startPoint);
		m_rays[index].m_dynEndPoint		= matLib->UpdatePolygonVertex(matrix.m,m_rays[index].m_endPoint);
		//return true;
	}
	else if(m_rays[index].m_directionalGen == 1)
	{//generate dir vector
		
		
		
		m_rays[index].m_dynStartPoint.x = 0.0f;
        m_rays[index].m_dynStartPoint.y = 0.0f;
		m_rays[index].m_dynStartPoint.z = 0.0f;

        m_rays[index].m_dynEndPoint.x = currentPosition->x - lastPosition->x;
        m_rays[index].m_dynEndPoint.y = 0.0f;
		m_rays[index].m_dynEndPoint.z = currentPosition->z - lastPosition->z;

		if(memcmp(&m_rays[index].m_dynEndPoint,&m_rays[index].m_dynStartPoint,sizeof(D3DVECTOR))==0) 
		   return false;

		m_rays[index].m_dynEndPoint = matLib->Normalize(m_rays[index].m_dynEndPoint);
		m_rays[index].m_dynEndPoint.x = m_rays[index].m_dynEndPoint.x * fabs(m_rays[index].m_endPoint.z);
		//m_rays[index].m_dynEndPoint.y = m_rays[index].m_dynEndPoint.y * m_rays[index].m_endPoint.z;
		m_rays[index].m_dynEndPoint.z = m_rays[index].m_dynEndPoint.z * fabs(m_rays[index].m_endPoint.z);

		m_rays[index].m_dynStartPoint.x += currentPosition->x;
		m_rays[index].m_dynStartPoint.y = currentPosition->y + m_rays[index].m_startPoint.y;
		m_rays[index].m_dynStartPoint.z += currentPosition->z;

		m_rays[index].m_dynEndPoint.x += currentPosition->x;
		m_rays[index].m_dynEndPoint.y = currentPosition->y + m_rays[index].m_startPoint.y;
		m_rays[index].m_dynEndPoint.z += currentPosition->z;


		//return true;
	}//end generate dir vector
	else if(m_rays[index].m_directionalGen == 2)
	{//generate dir vector
		D3DMATRIX matrix;
		frame->GetTransform(NULL,&matrix);

		m_rays[index].m_dynStartPoint.x = 0.0f;
        m_rays[index].m_dynStartPoint.y = 0.0f;
		m_rays[index].m_dynStartPoint.z = 0.0f;

        m_rays[index].m_dynEndPoint.x = currentPosition->x - lastPosition->x;
        m_rays[index].m_dynEndPoint.y = currentPosition->y - lastPosition->y;
		m_rays[index].m_dynEndPoint.z = currentPosition->z - lastPosition->z;

		if(memcmp(&m_rays[index].m_dynEndPoint,&m_rays[index].m_dynStartPoint,sizeof(D3DVECTOR))==0) 
		   return false;

		m_rays[index].m_dynEndPoint = matLib->Normalize(m_rays[index].m_dynEndPoint);
		m_rays[index].m_dynEndPoint.x = m_rays[index].m_dynEndPoint.x * fabs(m_rays[index].m_endPoint.z);
		m_rays[index].m_dynEndPoint.y = m_rays[index].m_dynEndPoint.y * fabs(m_rays[index].m_endPoint.z);
		m_rays[index].m_dynEndPoint.z = m_rays[index].m_dynEndPoint.z * fabs(m_rays[index].m_endPoint.z);

		m_rays[index].m_dynStartPoint.x += currentPosition->x;
		m_rays[index].m_dynStartPoint.y += currentPosition->y;
		m_rays[index].m_dynStartPoint.z += currentPosition->z;

		m_rays[index].m_dynEndPoint.x += currentPosition->x;
		m_rays[index].m_dynEndPoint.y += currentPosition->y;
		m_rays[index].m_dynEndPoint.z += currentPosition->z;

        //max extents
		/*if(m_rays[index].m_dynEndPoint.y < (matrix._42 + skeletonPtr->m_box.minY))
		    m_rays[index].m_dynEndPoint.y = matrix._42 + skeletonPtr->m_box.minY;*/
       
		//return true;
	}//end generate dir vector
	else if(m_rays[index].m_directionalGen == 3)
	{//generate wheel vector based on shocks
        D3DMATRIX matrix;
		frame->GetTransform(NULL,&matrix);
		m_rays[index].m_dynStartPoint	= matLib->UpdatePolygonVertex(matrix.m,m_rays[index].m_startPoint);
        
		D3DVECTOR final;
		final.x = m_rays[index].m_wheel->m_shock->m_currentPosition.x;
		final.y = m_rays[index].m_wheel->m_shock->m_currentPosition.y - m_rays[index].m_wheel->m_radius;
		final.z = m_rays[index].m_wheel->m_shock->m_currentPosition.z;
		m_rays[index].m_dynEndPoint		= matLib->UpdatePolygonVertex(matrix.m,final);
		
		//m_rays[index].m_dynEndPoint		= matLib->UpdatePolygonVertex(matrix.m,m_rays[index].m_endPoint);
	}//end generate wheel vector based on shocks
	else if(m_rays[index].m_directionalGen == 4)
	{//
        D3DMATRIX matrix;
		frame->GetTransform(NULL,&matrix);
		m_rays[index].m_dynStartPoint	= matLib->UpdatePolygonVertex(matrix.m,m_rays[index].m_startPoint);
		m_rays[index].m_dynEndPoint		= matLib->UpdatePolygonVertex(matrix.m,m_rays[index].m_endPoint);
	}
   return true;
}
//--------------------------------------------------------------------------------------//
void CCollision_RaySystem::ResetCollisionFlags()
{
    for(int loop=0;loop<m_rayCount;loop++)
	{
        if(m_rays[loop].m_collidedThisRound == true)
		{
		   m_rays[loop].m_collidedLastRound = true;
           CopyMemory(&m_rays[loop].m_prevIntersect,&m_rays[loop].m_lastIntersect,sizeof(D3DVECTOR));
		}
		else
		   m_rays[loop].m_collidedLastRound = false;

		m_rays[loop].m_collidedThisRound = false;
	}
}
//--------------------------------------------------------------------------------------//
int CCollision_RaySystem::GetCollisionCount()
{
	int colcount = 0;
    for(int loop=0;loop<m_rayCount;loop++)
	{
        if(m_rays[loop].m_collidedThisRound == true)
		   colcount++;
	}
	return colcount;
}
//--------------------------------------------------------------------------------------//
int CCollision_RaySystem::GetCollisionCountWheelSys()
{
	int colcount = 0;
    for(int loop=0;loop<m_rayCount;loop++)
	{
		if(m_rays[loop].m_wheel)
        if(m_rays[loop].m_collidedThisRound == true)
		   colcount++;
	}
	return colcount;
}
//--------------------------------------------------------------------------------------//
CCollision_Ray CCollision_RaySystem::GetRayByVirtualIndex(int virtualIndex)
{
	int colcount = 0;
    for(int loop=0;loop<m_rayCount;loop++)
	{
        if(m_rays[loop].m_collidedThisRound == true)
		if(m_rays[loop].m_depth > .02f)
		{
		   if(virtualIndex == colcount)
			  return m_rays[loop];
		   colcount++;
		}
	}
	return m_rays[0];
}
//--------------------------------------------------------------------------------------//
CCollision_Ray * CCollision_RaySystem::GetDeepestRay(int byDirGen)
{	
	CCollision_Ray * reference = NULL;
	float curDepth = 0.0f;
    for(int loop=0;loop<m_rayCount;loop++)
	{
		if(m_rays[loop].m_directionalGen == byDirGen)
		if(m_rays[loop].m_collidedThisRound == true)
		{
           if(m_rays[loop].m_depth > curDepth)
		   {
              curDepth = m_rays[loop].m_depth;
              reference = &m_rays[loop];
		   }
		}
	}
	return reference;
}
//----------------------------------------------------------------------------------------//
void CCollision_RaySystem::HandleTireRotation(MatrixARB * matLib,int rayIndex,D3DMATRIX & baseFrameMatrix)
{
	    if(m_rays[rayIndex].m_wheel)
        if(m_rays[rayIndex].m_collidedLastRound == true && m_rays[rayIndex].m_collidedThisRound == true)
		{//handle tire rotation
			D3DMATRIX bottomLine,invMatrix;
			matLib->fastMatrixMultiply(bottomLine,m_rays[rayIndex].m_wheel->m_wheelHolder->List_frameMatrix,baseFrameMatrix);
			matLib->D3DMath_MatrixInvert(invMatrix,bottomLine);
			
            D3DMATRIX rWheelMat;
			m_rays[rayIndex].m_wheel->m_wheelFrameRef->GetTransform(NULL,&rWheelMat);
			D3DVECTOR localizedVectPrev = matLib->GetLocalPositionFromMatrixView2(invMatrix,bottomLine,m_rays[rayIndex].m_prevIntersect);
			D3DVECTOR localizedVect		= matLib->GetLocalPositionFromMatrixView2(invMatrix,bottomLine,m_rays[rayIndex].m_lastIntersect);
			//D3DVECTOR localizedVectPrev = matLib->GetLocalPositionFromMatrixView(m_rays[rayIndex].m_wheel->m_wheelHolder->List_frameMatrix,m_rays[rayIndex].m_prevIntersect);
			//D3DVECTOR localizedVect		= matLib->GetLocalPositionFromMatrixView(m_rays[rayIndex].m_wheel->m_wheelHolder->List_frameMatrix,m_rays[rayIndex].m_lastIntersect);
			m_rays[rayIndex].m_lastRotation = localizedVectPrev.z - localizedVect.z;
			m_rays[rayIndex].m_wheel->m_wheelFrameRef->RotateOnAxisXBasedOnDistCirc(m_rays[rayIndex].m_wheel->m_radius*2.0f,m_rays[rayIndex].m_lastRotation);
		}//end handle tire rotation
}
//----------------------------------------------------------------------------------------//
void CCollision_RaySystem::HandleTireSteeringRotation(MatrixARB * matLib,int rayIndex)
{
	    m_rays[rayIndex].HandleTireSteeringRotation(matLib);
}
//----------------------------------------------------------------------------------------//
D3DMATRIX CCollision_RaySystem::Get4pointOrientationMatrixBasedDPRay(MatrixARB * matLib,CCollision_Ray * deepestRay)
{
          D3DVECTOR curOriDirVect,curOriCrossVect,curOriUpVect,projOriDirVect,projOriCrossVect,projOriUpVect;
		  D3DMATRIX origMatrix,newMatrix;
          matLib->InitMat(origMatrix.m);
		  matLib->InitMat(newMatrix.m);

		  //create current matrix
          origMatrix._41 = deepestRay->m_dynEndPoint.x;
		  origMatrix._42 = deepestRay->m_dynEndPoint.y;
		  origMatrix._43 = deepestRay->m_dynEndPoint.z;

		  curOriDirVect.x = deepestRay->m_dirPivot->m_dynEndPoint.x - deepestRay->m_dynEndPoint.x;
		  curOriDirVect.y = deepestRay->m_dirPivot->m_dynEndPoint.y - deepestRay->m_dynEndPoint.y;
		  curOriDirVect.z = deepestRay->m_dirPivot->m_dynEndPoint.z - deepestRay->m_dynEndPoint.z;

		  curOriCrossVect.x = deepestRay->m_crossPivot->m_dynEndPoint.x - deepestRay->m_dynEndPoint.x;
		  curOriCrossVect.y = deepestRay->m_crossPivot->m_dynEndPoint.y - deepestRay->m_dynEndPoint.y;
		  curOriCrossVect.z = deepestRay->m_crossPivot->m_dynEndPoint.z - deepestRay->m_dynEndPoint.z;

          curOriDirVect = matLib->Normalize(curOriDirVect);
		  curOriCrossVect = matLib->Normalize(curOriCrossVect);
		  curOriUpVect = matLib->CrossProduct(curOriDirVect,curOriCrossVect);
		  curOriUpVect = matLib->Normalize(curOriUpVect);
		  matLib->SetOrientation(&origMatrix,curOriDirVect,curOriUpVect);
		  //create corrected matrix
		  deepestRay->m_dynEndPoint = deepestRay->GetCorrectivePosition(matLib,1);
		  deepestRay->m_depth = .0f;
		  deepestRay->m_lastIntersect = deepestRay->m_dynEndPoint;
		  newMatrix._41 = deepestRay->m_dynEndPoint.x;
		  newMatrix._42 = deepestRay->m_dynEndPoint.y;
		  newMatrix._43 = deepestRay->m_dynEndPoint.z;


		  deepestRay->m_dirPivot->m_dynEndPoint = deepestRay->m_dirPivot->GetCorrectivePosition(matLib,1);
		  deepestRay->m_dirPivot->m_depth = .0f;
		  deepestRay->m_dirPivot->m_lastIntersect = deepestRay->m_dirPivot->m_dynEndPoint;
		  projOriDirVect.x = deepestRay->m_dirPivot->m_dynEndPoint.x - newMatrix._41;
		  projOriDirVect.y = deepestRay->m_dirPivot->m_dynEndPoint.y - newMatrix._42;
		  projOriDirVect.z = deepestRay->m_dirPivot->m_dynEndPoint.z - newMatrix._43;

		  deepestRay->m_crossPivot->m_dynEndPoint = deepestRay->m_crossPivot->GetCorrectivePosition(matLib,1);
		  deepestRay->m_crossPivot->m_depth = .0f;
		  deepestRay->m_crossPivot->m_lastIntersect = deepestRay->m_crossPivot->m_dynEndPoint;
		  projOriCrossVect.x = deepestRay->m_crossPivot->m_dynEndPoint.x - newMatrix._41;
		  projOriCrossVect.y = deepestRay->m_crossPivot->m_dynEndPoint.y - newMatrix._42;
		  projOriCrossVect.z = deepestRay->m_crossPivot->m_dynEndPoint.z - newMatrix._43;

          projOriDirVect = matLib->Normalize(projOriDirVect);
		  projOriCrossVect = matLib->Normalize(projOriCrossVect);
		  projOriUpVect = matLib->CrossProduct(projOriDirVect,projOriCrossVect);
		  projOriUpVect = matLib->Normalize(projOriUpVect);
		  matLib->SetOrientation(&newMatrix,projOriDirVect,projOriUpVect);


		  D3DMATRIX diff = matLib->GetDifferenceMatrix(origMatrix,newMatrix);
          return diff;
}
//----------------------------------------------------------------------------------------//
void CCollision_RaySystem::WheelCallback(MatrixARB * matLib,D3DMATRIX & entMatrix)
{
       for(int wLoop = 0;wLoop<m_rayCount;wLoop++)
	   {
           if(!m_rays[wLoop].m_wheel)
			   continue;

	          m_rays[wLoop].HandleTireRotation(matLib,entMatrix); 
			  if(m_rays[wLoop].m_wheel)
			     if(m_rays[wLoop].m_wheel->m_shock)
				 {
				    if(m_rays[wLoop].m_wheel->m_wheelFrameRef)
                       m_rays[wLoop].m_wheel->m_wheelFrameRef->SetPosition(NULL,m_rays[wLoop].m_wheel->m_shock->m_currentPosition);

				 }
		}
}
//----------------------------------------------------------------------------------------//
void CCollision_RaySystem::GetCurSteeringRotationData(float & curWheelRotation,float & lengthBetweenTires)
{
       for(int wLoop = 0;wLoop<m_rayCount;wLoop++)
	   {
           if(!m_rays[wLoop].m_wheel)
			   continue;

				if(m_rays[wLoop].m_wheel->m_canSteerRotate == true)
				{
                   lengthBetweenTires = m_rays[wLoop].m_wheel->m_lengthBetweenFBWheels;
				   curWheelRotation = m_rays[wLoop].m_wheel->m_currentSteeringRotation;
				   return;
				}
		}
}
//----------------------------------------------------------------------------------------//
void CCollision_RaySystem::GetCurSteeringRotationData(double & curWheelRotation,double & lengthBetweenTires)
{
       for(int wLoop = 0;wLoop<m_rayCount;wLoop++)
	   {
           if(!m_rays[wLoop].m_wheel)
			   continue;

				if(m_rays[wLoop].m_wheel->m_canSteerRotate == true)
				{
                   lengthBetweenTires = (double)m_rays[wLoop].m_wheel->m_lengthBetweenFBWheels;
				   curWheelRotation = (double)m_rays[wLoop].m_wheel->m_currentSteeringRotation;
				   return;
				}
		}
}
//----------------------------------------------------------------------------------------//
void CCollision_RaySystem::SetCurSteeringRotationData(float curWheelRotation)
{
       for(int wLoop = 0;wLoop<m_rayCount;wLoop++)
	   {
           if(!m_rays[wLoop].m_wheel)
			   continue;

				if(m_rays[wLoop].m_wheel->m_canSteerRotate == true)
				{
				   curWheelRotation = m_rays[wLoop].m_wheel->m_currentSteeringRotation = curWheelRotation;
				}
		}
}
//----------------------------------------------------------------------------------------//
bool CCollision_RaySystem::ApplyCurSteeringRotationData(float curWheelRotation)
{
	   bool applied = false;
       for(int wLoop = 0;wLoop<m_rayCount;wLoop++)
	   {
           if(!m_rays[wLoop].m_wheel)
			   continue;

				if(m_rays[wLoop].m_wheel->m_canSteerRotate == true)
				{
                   m_rays[wLoop].m_wheel->m_currentSteeringRotation += curWheelRotation;

				   if(m_rays[wLoop].m_wheel->m_currentSteeringRotation > m_rays[wLoop].m_wheel->m_steeringRotationMax)
				      m_rays[wLoop].m_wheel->m_currentSteeringRotation = m_rays[wLoop].m_wheel->m_steeringRotationMax;

				   if(m_rays[wLoop].m_wheel->m_currentSteeringRotation < -m_rays[wLoop].m_wheel->m_steeringRotationMax)
				      m_rays[wLoop].m_wheel->m_currentSteeringRotation = -m_rays[wLoop].m_wheel->m_steeringRotationMax;

				   //curWheelRotation = m_rays[wLoop].m_wheel->m_currentSteeringRotation = curWheelRotation;
				   applied = true;
				}
		}
	   return applied;
}
//----------------------------------------------------------------------------------------//
D3DMATRIX CCollision_RaySystem::MultiPointCalculatePosition(double force,
															D3DMATRIX & bFrameMatrix,
															MatrixARB * matLib,
															void * physicsObject,
															double & envGravity,
															double & drag,
															D3DVECTOR & slipVectRet,
															void * textSysVoid)
{
      
      CtextReadoutObj * textSys = (CtextReadoutObj *)textSysVoid;

	  D3DMATRIX matTemp1,matTemp2App;
	  CMovementCaps * phyObj = (CMovementCaps *)physicsObject;
      
	  float curWheelRotation = 0.0f;
	  float distFBTires = 0.0f;
      GetCurSteeringRotationData(curWheelRotation,distFBTires);
	  if(curWheelRotation == 0.0f)
 	  {//simple foward or backwards force
	     matLib->InitMat(matTemp2App.m);
		 /*matTemp2App._43 = force;
         matTemp1 = bFrameMatrix;
		 matLib->fastMatrixMultiply(matTemp1,matTemp2App,matTemp1);*/
         return matLib->GetDifferenceMatrix(bFrameMatrix,matTemp1);
	  }//end simple foward or backwards force

	  //test constants
	  float tireSpeedRating = .1f;
	  float tireDrag = .98f;
		  
	  //make velocity unit vector
	  D3DVECTOR vvUnitVector,tireUnitVector,startTireUnitVector,forceVect;
	  vvUnitVector = matLib->Normalize(phyObj->List_curVelocityVect);
	  //generate Tire unit Vector
	  ZeroMemory(&startTireUnitVector,sizeof(D3DVECTOR));
	  tireUnitVector.x = 0.0f;
	  tireUnitVector.y = 0.0f;
      tireUnitVector.z = -1.0f;
	  tireUnitVector = matLib->UpdatePolygonVertex(this->m_rays[1].m_wheel->m_wheelHolder->List_frameMatrix.m,tireUnitVector);
	  tireUnitVector = matLib->UpdatePolygonVertex(bFrameMatrix.m,tireUnitVector);
	  startTireUnitVector = matLib->UpdatePolygonVertex(this->m_rays[1].m_wheel->m_wheelHolder->List_frameMatrix.m,startTireUnitVector);
	  startTireUnitVector = matLib->UpdatePolygonVertex(bFrameMatrix.m,startTireUnitVector);
	  tireUnitVector.x = startTireUnitVector.x - tireUnitVector.x;
	  tireUnitVector.y = startTireUnitVector.y - tireUnitVector.y;
      tireUnitVector.z = startTireUnitVector.z - tireUnitVector.z;
	  tireUnitVector = matLib->Normalize(tireUnitVector);
	  //get deter angle
      float deterAngle = matLib->GetAngleBetweenTwoVectorsFast(tireUnitVector,vvUnitVector);
	  //determine final slip speed rating
	  float finalTireSpeedRating = fabs((1.0f - (fabs(deterAngle)/90))*tireSpeedRating);
	  float dragFromTires = matLib->InterpolateFloatsHiRes(1.0f,tireDrag,90.0f,fabs(deterAngle));
	  //generate force vect
	  forceVect.x = tireUnitVector.x * (float)force;
	  forceVect.y = tireUnitVector.y * (float)force;
	  forceVect.z = tireUnitVector.z * (float)force;


      D3DVECTOR currentVelocityVector = phyObj->List_curVelocityVect;
	  currentVelocityVector.x = ((currentVelocityVector.x + forceVect.x)* dragFromTires);
      currentVelocityVector.y = ((currentVelocityVector.y + forceVect.y)* dragFromTires); 
	  currentVelocityVector.z = ((currentVelocityVector.z + forceVect.z)* dragFromTires);

	  //find current speed
	  float zeroTemp = 0.0f;
	  float currentSpeed = matLib->MagnitudeSquared(zeroTemp,zeroTemp,zeroTemp,currentVelocityVector.x,currentVelocityVector.y,currentVelocityVector.z);
	  textSys->AddLine("Speed",currentSpeed);
	  textSys->AddLine("Cur Velocity Vect",currentVelocityVector);
	  textSys->AddLine("Drag from tires",dragFromTires);
	  //generate split vectors
	  if(finalTireSpeedRating == 0.0f)
	     finalTireSpeedRating = .0001f;//eliminate bs div 0
	  float pC = finalTireSpeedRating/currentSpeed;
      if(pC > 1.0f)
	     pC = 1.0f;

	  if(currentSpeed > 0.0f && pC != 0.0f)
	  {
      //gen slip vector  
	  slipVectRet.x = currentVelocityVector.x * pC;
	  slipVectRet.y = currentVelocityVector.y * pC;
	  slipVectRet.z = currentVelocityVector.z * pC;

	  }
	  //generate final rotation
	  float finalRotationForce = (1.0f - pC) * currentSpeed;

	  float topTriAngle = 90.0f - fabs(curWheelRotation);
	  float axisAngle = 180.0f - (topTriAngle + 90.0f);
	  float axisAngleRadians = (float)matLib->PI * ( axisAngle / 180.0f);
	  if(axisAngleRadians > 1.0f)
	     axisAngleRadians = 1.0f;

	  if(axisAngleRadians <= 0.0f)
	     axisAngleRadians = 0.01f;
	  
	  float radius = distFBTires/sin(axisAngleRadians);
      float circumference = (radius * 2.0f)*matLib->PI;
	  float angleToRotate = (fabs(finalRotationForce)/circumference)*360.0f;

	  D3DMATRIX axisPoint,newAxis,diffMatTemp;
	  
	  if(curWheelRotation > 0.0f)
	  {
			matLib->InitMat(axisPoint.m);
			axisPoint._41 = -radius;
			matLib->fastMatrixMultiply(axisPoint,axisPoint,this->m_rays[1].m_wheel->m_wheelHolder->List_frameMatrix);
			matLib->fastMatrixMultiply(axisPoint,axisPoint,bFrameMatrix);
			newAxis = axisPoint;
			if(force > 0.0f)
		       matLib->RotateY(-angleToRotate,matTemp2App.m);
		    else
		       matLib->RotateY(angleToRotate,matTemp2App.m);


			matLib->fastMatrixMultiply(newAxis,matTemp2App,newAxis);

			diffMatTemp = matLib->GetDifferenceMatrix(axisPoint,newAxis);
	     
			return diffMatTemp;
	  }
	 
		matLib->InitMat(axisPoint.m);
		axisPoint._41 = radius;
		matLib->fastMatrixMultiply(axisPoint,axisPoint,this->m_rays[0].m_wheel->m_wheelHolder->List_frameMatrix);
		matLib->fastMatrixMultiply(axisPoint,axisPoint,bFrameMatrix);
		newAxis = axisPoint;
		if(force > 0.0f)
		   matLib->RotateY(angleToRotate,matTemp2App.m);
		else
		   matLib->RotateY(-angleToRotate,matTemp2App.m);

		matLib->fastMatrixMultiply(newAxis,matTemp2App,newAxis);

	    diffMatTemp = matLib->GetDifferenceMatrix(axisPoint,newAxis);
	     
		return diffMatTemp;
}


//----------------------------------------------------------------------------------------//
double CCollision_RaySystem::GetWheelBase()
{
    for(int loop=0;loop<this->m_rayCount;loop++)
	{
		if(this->m_rays[loop].m_wheel)
           return this->m_rays[loop].m_wheel->m_wheelBase;
	}
    return 0.0;
}

float CCollision_RaySystem::Round(const float &number, const int num_digits)
{
    float doComplete5i, doComplete5(number * powf(10.0f, (float) (num_digits + 1)));
    
    if(number < 0.0f)
        doComplete5 -= 5.0f;
    else
        doComplete5 += 5.0f;
    
    doComplete5 /= 10.0f;
    modff(doComplete5, &doComplete5i);
    
    return doComplete5i / powf(10.0f, (float) num_digits);
}

double CCollision_RaySystem::RoundDouble(double doValue, int nPrecision)
{
    static const double doBase = 10.0;
    double doComplete5, doComplete5i;
    
    doComplete5 = doValue * pow(doBase, (double) (nPrecision + 1));
    
    if(doValue < 0.0)
        doComplete5 -= 5.0;
    else
        doComplete5 += 5.0;
    
    doComplete5 /= doBase;
    modf(doComplete5, &doComplete5i);
    
    return doComplete5i / pow(doBase, (double) nPrecision);
}

//----------------------------------------------------------------------------------------//
void CCollision_RaySystem::DoCarPhysics(    double force,
										    double brakeForce,
											void * frameObjVoid,
											MatrixARB * matLib,
											void * physicsObject,
											double & envGravity,
											void * textSysVoid,
											double adjustTime,
											double & lastAdjustTime,
											double & extForceVectX,
											double & extForceVectY,
											double & extForceVectZ,
											double & additionalForceVectX,
											double & additionalForceVectY,
											double & additionalForceVectZ,
											double & externalModifier)
{

	
	adjustTime = adjustTime * (1.0+((adjustTime-1.0)*.5f));
	

    CFrameObj * frameObj = (CFrameObj *)frameObjVoid;
	CMovementCaps * phyObj = (CMovementCaps *)physicsObject;
	CtextReadoutObj * textSys = (CtextReadoutObj *)textSysVoid;

	double scalarShift = 10.0;
	//transform velocity in world reference frame to velocity in car reference frame
	
	AXDXMATRIX frameMatD;
	frameMatD = frameObj->List_frameMatrix;
	AXDVECTOR velocityVectD;
	velocityVectD = phyObj->List_curVelocityVect;

    double   gravityForce = .04;
	gravityForce = -envGravity;

     if(!phyObj->List_doGravity)
	    gravityForce = 0.0;

	 velocityVectD.x -= (extForceVectX)*adjustTime;
	 velocityVectD.y -= ((gravityForce + extForceVectY)*adjustTime);
	 velocityVectD.z -= (extForceVectZ)*adjustTime;

	AXDVECTOR velocityTest = matLib->GetLocalPositionFromMatrixViewRotOnlyD(frameMatD,velocityVectD);


	/*double adjustLocal = adjustTime / lastAdjustTime;
   
    velocityTest.x = (velocityTest.x*adjustLocal);
    velocityTest.y = (velocityTest.y*adjustLocal);
    velocityTest.z = (velocityTest.z*adjustLocal);*/
	


	velocityTest.x += (additionalForceVectX * adjustTime);
	velocityTest.y += (additionalForceVectY * adjustTime);
	velocityTest.z += (additionalForceVectZ * adjustTime);


	velocityTest.x = velocityTest.x / adjustTime;
	velocityTest.y = velocityTest.y / adjustTime;
	velocityTest.z = velocityTest.z / adjustTime;
	
	double dist = velocityTest.MagnitudeSquared(0.0,velocityTest.y,0.0);

	double curWheelRotation = 0.0f;
	double distFBTires = 0.0f;
    GetCurSteeringRotationData(curWheelRotation,distFBTires);

	double velocityX = -velocityTest.x * scalarShift;
	double velocityY = velocityTest.y * scalarShift;
    double velocityZ = -velocityTest.z * scalarShift;

	//phyObj->m_angularvelocity = 0.0f;
       //curWheelRotation = 0.0f;
	//Resulting velocity of the wheels as result of the yaw rate of the car body
	//v = yawrate * r where r is distance of wheel to CG (approx. half wheel base)
	//yawrate (ang.velocity) must be in rad/s
	//
	double	 DRAG	      = phyObj->m_crossSectionalArea;   //5.0;	//factor for air resistance (drag) 
    double	 RESISTANCE	  = phyObj->List_groundDrag;//30.0		    //factor for rolling resistance  **tire based
    double	 CA_R		  = phyObj->m_corneringStiffnessR;//-5.2	    //cornering stiffness **tire based
    double	 CA_F		  = phyObj->m_corneringStiffnessF;//-5.0	    //cornering stiffness **tire based
    double	 MAX_GRIP	  = phyObj->m_maxGrip;//2.0f				    //MAX GRIP **tire based
	double   mass         = phyObj->List_mass;						//default 1500
	double   inertia      = phyObj->m_inertia;						//1500
	double   DELTA_T	  = 0.01;//* adjustTime;
	double   DELTA_Tx	  = 0.01;
	double   throttle     = force;
	double   brake        = brakeForce;
	

    
	if(phyObj->List_doGravity)
	{
        brake = 0.0;
        if(phyObj->m_inLiquid)
           throttle = throttle * externalModifier;
		else
		{
		    throttle = 0.0;
			curWheelRotation = 0.0;
			RESISTANCE = 2.0;
			CA_R = 0.0;
			CA_F = 0.0;
			DRAG = DRAG * .8;

			MAX_GRIP = 0.0;
		}
	}

	double velocityXangleUse = velocityX;
	double velocityYangleUse = velocityY;
    double velocityZangleUse = velocityZ;
	//curWheelRotation = 1.0;//TEMP

	if(fabs(velocityZangleUse) < 0.5)
	   velocityZangleUse = 0.0;


	double yawspeed = GetWheelBase() * 0.5 * phyObj->m_angularvelocity;//yawspeed = car->cartype->wheelbase * 0.5 * car->angularvelocity;	
	if(!phyObj->List_doGravity)
	if(velocityZangleUse == 0.0)
	{
	   yawspeed = 0.0;
	   phyObj->m_angularvelocity = 0.0;
	   curWheelRotation = 0.0;
	   velocityX = 0.0;
	   //velocityZ = 0.0;
	}
	
	double rot_angle = 0.0;
	if( velocityZangleUse > 0.0 )		//TODO: fix singularity
		rot_angle = atan2( yawspeed, velocityZangleUse);
	else if(velocityZangleUse < 0.0)
	    rot_angle = atan2( yawspeed, -velocityZangleUse);

	//Calculate the side slip angle of the car (a.k.a. beta)
	double sideslip = 0.0;

	if( velocityZangleUse > 0.0 )		//TODO: fix singularity
		sideslip = atan2( velocityXangleUse, velocityZangleUse);
	else if(velocityZangleUse < 0.0)
	    sideslip = atan2( velocityXangleUse, -velocityZangleUse);

	double steeringAngle = curWheelRotation / 57.3;
	if(velocityZ < 0.0f)
	   steeringAngle = -steeringAngle;

	double slipanglefront = sideslip + rot_angle - steeringAngle;
	double slipanglerear  = sideslip - rot_angle;

     
	//textSys->AddLine("slip angle front:",(float)slipanglefront);
	//slipanglefront=0.0;//temp
	//slipanglerear = 0.0;//temp

	//weight per axle = half car mass times 1G (=9.8m/s^2) 
	//weight = car->cartype->mass * 9.8 * 0.5;	
	double weight = mass * 9.8 * 0.5;
	//lateral force on front wheels = (Ca * slip angle) capped to friction circle * load

    bool front_slip = false;
	bool rear_slip = false;

	double flatX,flatY;
	flatX = 0.0;
	flatY = CA_F * slipanglefront;
	flatY = min(MAX_GRIP, flatY);
	flatY = max(-MAX_GRIP, flatY);
	flatY *= weight;
	if(front_slip)
		flatY *= 0.5;

	//lateral force on rear wheels

    double flatrX,flatrY;
	flatrX = 0.0;
	flatrY = CA_R * slipanglerear;
	flatrY = min(MAX_GRIP, flatrY);
	flatrY = max(-MAX_GRIP, flatrY);
	flatrY *= weight;
	if(rear_slip)
		flatrY *= 0.5;

	//longtitudinal force on rear wheels - very simple traction model
	//ftraction.x = 100*(car->throttle - car->brake*SGN(velocity.x));	
	//ftraction.y = 0;
	//if(rear_slip)
	//	ftraction.x *= 0.5; 
	double ftractionZ = 100.0f*(throttle - brake*-velocityZ);

	//float ftractionY = 100.0f*(gravityForce*_chgsign(velocity.y));	
	//float ftractionZ = 100.0f*(throttle - brake*velocity.z);
	double ftractionX = 0.0f;
	if(rear_slip)
	   ftractionZ *= 0.5f;

//Forces and torque on body
	
	//drag and rolling resistance
	double resistanceX = -( RESISTANCE*velocityX + DRAG*velocityX*abs(velocityX) );
	double resistanceZ = -( RESISTANCE*velocityZ + DRAG*velocityZ*abs(velocityZ) );

	//sum forces
	double forceX = (ftractionX + cos(steeringAngle) * flatY + flatrY + resistanceX);//Good
	double forceZ = (ftractionZ + sin(steeringAngle) * flatX + flatrX + resistanceZ);//good
	
	//torque on body from lateral forces
	//torque = car->cartype->b * flatf.y - car->cartype->c * flatr.y;

	double torque = 1.0f * flatY - 1.0f * flatrY;
    //Acceleration
	//Newton F = m.a, therefore a = F/m
	//acceleration.x = force.x/car->cartype->mass;
	//acceleration.y = force.y/car->cartype->mass;
	//angular_acceleration = torque / car->cartype->inertia;
	AXDVECTOR accelerationVector;
	accelerationVector.x = (forceX/mass);
	//accelerationVector.y = -gravityForce;//(forceY/mass);
	accelerationVector.z = (forceZ/mass);
	double angular_acceleration = torque / inertia;

    //Velocity and position
	AXDVECTOR transformLocal;
	//D3DVECTOR newVelocityVect = velocity;
    //double newVelocityVectX = velocityX;


	velocityX += DELTA_T * accelerationVector.x;
	velocityY = velocityY;
	velocityZ += DELTA_T * accelerationVector.z;

    


	transformLocal.x = -velocityX / scalarShift;
	transformLocal.y =  velocityY / scalarShift;
	transformLocal.z = -velocityZ / scalarShift;

	//time adjust
    //transformLocal.x *= adjustTime;
    //transformLocal.y *= adjustTime;
	//transformLocal.z *= adjustTime;
	//end time adjust

    transformLocal = matLib->UpdatePolygonVertexRotationOnlyD(frameMatD,transformLocal);
	
	//integrate angular acceleration to get angular velocity
	if(phyObj->List_doGravity)//in air
	{
       phyObj->m_angularvelocity = phyObj->m_angularvelocity; //* drag
	   transformLocal = matLib->UpdatePolygonVertexRotationOnlyD(frameMatD,velocityTest);
	}
	else//on ground
	{
	   phyObj->m_angularvelocity += DELTA_T * angular_acceleration;
	}

	if(phyObj->m_inLiquid)
	{
       transformLocal.x = transformLocal.x * externalModifier;
	   transformLocal.y = transformLocal.y * externalModifier;
	   transformLocal.z = transformLocal.z * externalModifier;
	}
	

    transformLocal.x = transformLocal.x * adjustTime;
	transformLocal.y = transformLocal.y * adjustTime;
	transformLocal.z = transformLocal.z * adjustTime;
	
	
	frameMatD.AddTransformAfter(transformLocal.x,transformLocal.y,transformLocal.z);
	

	if(phyObj->m_inLiquid)
	{
       phyObj->m_angularvelocity = phyObj->m_angularvelocity * externalModifier;
	}

	double curRotationY = (DELTA_Tx * phyObj->m_angularvelocity)*adjustTime;

	//textSys->AddLine("ang velocity:",phyObj->m_angularvelocity);

	if(curRotationY != 0.0)
	{
	   AXDXMATRIX rotCur;
	   rotCur.RotateYri(curRotationY);
       matLib->fastMatrixMultiplyD(frameMatD,rotCur,frameMatD);
	   //frameObj->AddTransformBefore(rotCur);
	}
	
	frameMatD.SetD3dMatrix(frameObj->List_frameMatrix);

	if(GetTickCount()-m_stamp > 300)
	{
        m_stamp = GetTickCount();

	    //textSys->AddLine("Local Velocity Vect",phyObj->m_velocity_wc);
		//textSys->AddLine("Trans Vect",transformLocal);
		//textSys->AddLine("FORCE:",force);
	}
}

//----------------------------------------------------------------------------------------//
/*//----------------------------------------------------------------------------------------//
void CCollision_RaySystem::DoCarPhysics(    double force,
										    double brakeForce,
											void * frameObjVoid,
											MatrixARB * matLib,
											void * physicsObject,
											double & envGravity,
											void * textSysVoid,
											double & adjustTime,
											double & lastAdjustTime,
											double & extForceVectX,
											double & extForceVectY,
											double & extForceVectZ,
											double & additionalForceVectX,
											double & additionalForceVectY,
											double & additionalForceVectZ,
											double & externalModifier)
{


    CFrameObj * frameObj = (CFrameObj *)frameObjVoid;
	CMovementCaps * phyObj = (CMovementCaps *)physicsObject;
	CtextReadoutObj * textSys = (CtextReadoutObj *)textSysVoid;

	double scalarShift = 10.0;
	//transform velocity in world reference frame to velocity in car reference frame
	
	AXDXMATRIX frameMatD;
	frameMatD = frameObj->List_frameMatrix;
	AXDVECTOR velocityVectD;
	velocityVectD = phyObj->List_curVelocityVect;

	AXDVECTOR velocityTest = matLib->GetLocalPositionFromMatrixViewRotOnlyD(frameMatD,velocityVectD);

	velocityTest.x += additionalForceVectX;
	velocityTest.z += additionalForceVectZ;
	
	double dist = velocityTest.MagnitudeSquared(0.0,velocityTest.y,0.0);

	double curWheelRotation = 0.0f;
	double distFBTires = 0.0f;
    GetCurSteeringRotationData(curWheelRotation,distFBTires);

	double velocityX = -velocityTest.x * scalarShift;
	double velocityY = velocityTest.y * scalarShift;
    double velocityZ = -velocityTest.z * scalarShift;

	//phyObj->m_angularvelocity = 0.0f;
       //curWheelRotation = 0.0f;
	//Resulting velocity of the wheels as result of the yaw rate of the car body
	//v = yawrate * r where r is distance of wheel to CG (approx. half wheel base)
	//yawrate (ang.velocity) must be in rad/s
	//
	double	 DRAG	      = phyObj->m_crossSectionalArea;   //5.0;	//factor for air resistance (drag) 
    double	 RESISTANCE	  = phyObj->List_groundDrag;//30.0		    //factor for rolling resistance  **tire based
    double	 CA_R		  = phyObj->m_corneringStiffnessR;//-5.2	    //cornering stiffness **tire based
    double	 CA_F		  = phyObj->m_corneringStiffnessF;//-5.0	    //cornering stiffness **tire based
    double	 MAX_GRIP	  = phyObj->m_maxGrip;//2.0f				    //MAX GRIP **tire based
	double   mass         = phyObj->List_mass;						//default 1500
	double   inertia      = phyObj->m_inertia;						//1500
	double   DELTA_T	  = 0.01 * adjustTime;
	double   throttle     = force;
	double   brake        = brakeForce;
	double   gravityForce = .04;

	gravityForce = -envGravity;

    if(!phyObj->List_doGravity)
	{
	    gravityForce = 0.0f;
	}
	else
	{
        brake = 0.0;
        if(phyObj->m_inLiquid)
           throttle = throttle * externalModifier;
		else
		{
		    throttle = 0.0;
			curWheelRotation = 0.0;
			RESISTANCE = 2.0;
			CA_R = 0.0;
			CA_F = 0.0;
			DRAG = DRAG * .8;

			MAX_GRIP = 0.0;
		}
	}

	double velocityXangleUse = velocityX;
	double velocityYangleUse = velocityY;
    double velocityZangleUse = velocityZ;
	//curWheelRotation = 1.0;//TEMP

	if(fabs(velocityZangleUse) < 0.5)
	   velocityZangleUse = 0.0;


	double yawspeed = GetWheelBase() * 0.5 * phyObj->m_angularvelocity;//yawspeed = car->cartype->wheelbase * 0.5 * car->angularvelocity;	
	if(velocityZangleUse == 0.0)
	{
	   yawspeed = 0.0;
	   phyObj->m_angularvelocity = 0.0;
	   curWheelRotation = 0.0;
	   velocityX = 0.0;
	   //velocityZ = 0.0;
	}
	
	double rot_angle = 0.0;
	if( velocityZangleUse > 0.0 )		//TODO: fix singularity
		rot_angle = atan2( yawspeed, velocityZangleUse);
	else if(velocityZangleUse < 0.0)
	    rot_angle = atan2( yawspeed, -velocityZangleUse);

	//Calculate the side slip angle of the car (a.k.a. beta)
	double sideslip = 0.0;

	if( velocityZangleUse > 0.0 )		//TODO: fix singularity
		sideslip = atan2( velocityXangleUse, velocityZangleUse);
	else if(velocityZangleUse < 0.0)
	    sideslip = atan2( velocityXangleUse, -velocityZangleUse);

	double steeringAngle = curWheelRotation / 57.3;
	if(velocityZ < 0.0f)
	   steeringAngle = -steeringAngle;

	double slipanglefront = sideslip + rot_angle - steeringAngle;
	double slipanglerear  = sideslip - rot_angle;

     
	//textSys->AddLine("slip angle front:",(float)slipanglefront);
	//slipanglefront=0.0;//temp
	//slipanglerear = 0.0;//temp

	//weight per axle = half car mass times 1G (=9.8m/s^2) 
	//weight = car->cartype->mass * 9.8 * 0.5;	
	double weight = mass * 9.8 * 0.5;
	//lateral force on front wheels = (Ca * slip angle) capped to friction circle * load

    bool front_slip = false;
	bool rear_slip = false;

	double flatX,flatY;
	flatX = 0.0;
	flatY = CA_F * slipanglefront;
	flatY = min(MAX_GRIP, flatY);
	flatY = max(-MAX_GRIP, flatY);
	flatY *= weight;
	if(front_slip)
		flatY *= 0.5;

	//lateral force on rear wheels

    double flatrX,flatrY;
	flatrX = 0.0;
	flatrY = CA_R * slipanglerear;
	flatrY = min(MAX_GRIP, flatrY);
	flatrY = max(-MAX_GRIP, flatrY);
	flatrY *= weight;
	if(rear_slip)
		flatrY *= 0.5;

	//longtitudinal force on rear wheels - very simple traction model
	//ftraction.x = 100*(car->throttle - car->brake*SGN(velocity.x));	
	//ftraction.y = 0;
	//if(rear_slip)
	//	ftraction.x *= 0.5; 
	double ftractionZ = 100.0f*(throttle - brake*_chgsign(velocityZ));

	//float ftractionY = 100.0f*(gravityForce*_chgsign(velocity.y));	
	//float ftractionZ = 100.0f*(throttle - brake*velocity.z);
	double ftractionX = 0.0f;
	if(rear_slip)
	   ftractionZ *= 0.5f;

//Forces and torque on body
	
	//drag and rolling resistance
	double resistanceX = -( RESISTANCE*velocityX + DRAG*velocityX*abs(velocityX) );
	double resistanceZ = -( RESISTANCE*velocityZ + DRAG*velocityZ*abs(velocityZ) );

	//sum forces
	double forceX = ftractionX + cos(steeringAngle) * flatY + flatrY + resistanceX;//Good
	double forceZ = ftractionZ + sin(steeringAngle) * flatX + flatrX + resistanceZ;//good
	
	//torque on body from lateral forces
	//torque = car->cartype->b * flatf.y - car->cartype->c * flatr.y;

	double torque = 1.0f * flatY - 1.0f * flatrY;
    //Acceleration
	//Newton F = m.a, therefore a = F/m
	//acceleration.x = force.x/car->cartype->mass;
	//acceleration.y = force.y/car->cartype->mass;
	//angular_acceleration = torque / car->cartype->inertia;
	AXDVECTOR accelerationVector;
	accelerationVector.x = (forceX/mass);
	//accelerationVector.y = -gravityForce;//(forceY/mass);
	accelerationVector.z = (forceZ/mass);
	double angular_acceleration = torque / inertia;

    //Velocity and position
	AXDVECTOR transformLocal;
	//D3DVECTOR newVelocityVect = velocity;
    //double newVelocityVectX = velocityX;


	velocityX += DELTA_T * accelerationVector.x;
	//newVelocityVect.y += DELTA_T * accelerationVector.y;
	velocityZ += DELTA_T * accelerationVector.z;

	transformLocal.x = -velocityX / scalarShift;
	transformLocal.y =  velocityY / scalarShift;
	transformLocal.z = -velocityZ / scalarShift;

    transformLocal = matLib->UpdatePolygonVertexRotationOnlyD(frameMatD,transformLocal);
	transformLocal.y = transformLocal.y - ((gravityForce+extForceVectY)*adjustTime);
	
	if(phyObj->m_inLiquid)
	{
       transformLocal.x = transformLocal.x * externalModifier;
	   transformLocal.y = transformLocal.y * externalModifier;
	   transformLocal.z = transformLocal.z * externalModifier;
	}
	
	frameMatD.AddTransformAfter(transformLocal.x,transformLocal.y,transformLocal.z);
	
	//textSys->AddLine("transVect:",transformLocal);

	//integrate angular acceleration to get angular velocity
	phyObj->m_angularvelocity += DELTA_T * angular_acceleration;

	//integrate angular velocity to get angular orientation
	//double curRotationY = DELTA_T * phyObj->m_angularvelocity;

	if(phyObj->m_inLiquid)
	{
       phyObj->m_angularvelocity = phyObj->m_angularvelocity * externalModifier;
	}

	double curRotationY = DELTA_T * phyObj->m_angularvelocity;

	//textSys->AddLine("ang velocity:",phyObj->m_angularvelocity);

	if(curRotationY != 0.0)
	{
	   AXDXMATRIX rotCur;
	   rotCur.RotateYri(curRotationY);
       matLib->fastMatrixMultiplyD(frameMatD,rotCur,frameMatD);
	   //frameObj->AddTransformBefore(rotCur);
	}
	
	frameMatD.SetD3dMatrix(frameObj->List_frameMatrix);

	if(GetTickCount()-m_stamp > 300)
	{
        m_stamp = GetTickCount();

	    //textSys->AddLine("Local Velocity Vect",phyObj->m_velocity_wc);
		//textSys->AddLine("Trans Vect",transformLocal);
		//textSys->AddLine("FORCE:",force);
	}
}*/


//----------------------------------------------------------------------------------------//
/*void CCollision_RaySystem::DoCarPhysics(    double force,
											void * frameObjVoid,
											MatrixARB * matLib,
											void * physicsObject,
											double & envGravity,
											void * textSysVoid)
{

    CFrameObj * frameObj = (CFrameObj *)frameObjVoid;
	CMovementCaps * phyObj = (CMovementCaps *)physicsObject;
	CtextReadoutObj * textSys = (CtextReadoutObj *)textSysVoid;

    //float result = atan2(8.0,2.0);//75 X/Z
	//float result = atan2(-8.0,2.0);//-75 X/Z

    //sn = sin(car->angle);//used to transform vectors to local frame view
	//cs = cos(car->angle);//used to transform vectors to local frame view

	//SAE convention: x is to the front of the car, y is to the right, z is down
	
	//transform velocity in world reference frame to velocity in car reference frame
	D3DVECTOR velocity = matLib->GetLocalPositionFromMatrixViewRotOnly(frameObj->List_frameMatrix,phyObj->List_curVelocityVect);

	velocity.x = -velocity.x;
	velocity.z = -velocity.z;

	//D3DVECTOR wldVelocity = phyObj->List_curVelocityVect;
	//velocity.x =  cs * car->velocity_wc.y + sn * car->velocity_wc.x;
	//velocity.y = -sn * car->velocity_wc.y + cs * car->velocity_wc.x;

//Lateral force on wheels
//	
	//Resulting velocity of the wheels as result of the yaw rate of the car body
	//v = yawrate * r where r is distance of wheel to CG (approx. half wheel base)
	//yawrate (ang.velocity) must be in rad/s
	//
	float	DRAG		= 5.0;		 	//factor for air resistance (drag) 
    float	RESISTANCE	= 30.0;			//factor for rolling resistance 
    float	CA_R		= -5.20;		//cornering stiffness 
    float	CA_F		= -5.0;			//cornering stiffness 
    float	MAX_GRIP	= 2.0;
	float   mass        = 1500.0f;
	float   inertia     = 1500.0f;
	float   DELTA_T		= 0.01f;

	float curWheelRotation = 0.0f;
	float distFBTires = 0.0f;
    GetCurSteeringRotationData(curWheelRotation,distFBTires);
	
	float yawspeed = this->m_rays[0].m_wheel->m_wheelBase * 0.5 * phyObj->m_angularvelocity;//yawspeed = car->cartype->wheelbase * 0.5 * car->angularvelocity;		
	
    
	//if( velocity.x == 0 )		//TODO: fix singularity
	//	rot_angle = 0;
	//else
	//	rot_angle = atan2( yawspeed, velocity.x);

	float rot_angle = 0.0f;
	if( velocity.x != 0.0f )		//TODO: fix singularity
		rot_angle = atan2( yawspeed, velocity.z);
   

	//Calculate the side slip angle of the car (a.k.a. beta)
	//if( velocity.x == 0 )		//TODO: fix singularity
	//	sideslip = 0;
	//else
	//	sideslip = atan2( velocity.y, velocity.x);

	float sideslip = 0.0f;
	if( velocity.x != 0.0f )		//TODO: fix singularity
		sideslip = atan2( velocity.x, velocity.z);

	//Calculate slip angles for front and rear wheels (a.k.a. alpha)
	//slipanglefront = sideslip + rot_angle - car->steerangle;
	//slipanglerear  = sideslip - rot_angle;
	float steeringAngle = curWheelRotation / 57.3;
	//steeringAngle = -steeringAngle;

	float slipanglefront = sideslip + rot_angle - steeringAngle;
	float slipanglerear  = sideslip - rot_angle;

	//weight per axle = half car mass times 1G (=9.8m/s^2) 
	//weight = car->cartype->mass * 9.8 * 0.5;	

	float weight = mass * 9.8 * 0.5;
	
	//lateral force on front wheels = (Ca * slip angle) capped to friction circle * load
	//flatf.x = 0;
	//flatf.y = CA_F * slipanglefront;
	//flatf.y = MIN(MAX_GRIP, flatf.y);
	//flatf.y = MAX(-MAX_GRIP, flatf.y);
	//flatf.y *= weight;
	//if(front_slip)
	//flatf.y *= 0.5;

    bool front_slip = false;
	bool rear_slip = false;

	float flatX,float flatY;
	flatX = 0;
	flatY = CA_F * slipanglefront;
	flatY = min(MAX_GRIP, flatY);
	flatY = max(-MAX_GRIP, flatY);
	flatY *= weight;
	if(front_slip)
		flatY *= 0.5;

	//lateral force on rear wheels
	//flatr.x = 0;
	//flatr.y = CA_R * slipanglerear;
	//flatr.y = MIN(MAX_GRIP, flatr.y);
	//flatr.y = MAX(-MAX_GRIP, flatr.y);
	//flatr.y *= weight;
	//if(rear_slip)
	//	flatr.y *= 0.5;

    float flatrX,float flatrY;
	flatrX = 0;
	flatrY = CA_R * slipanglerear;
	flatrY = min(MAX_GRIP, flatrY);
	flatrY = max(-MAX_GRIP, flatrY);
	flatrY *= weight;
	if(rear_slip)
		flatrY *= 0.5;

	//longtitudinal force on rear wheels - very simple traction model
	//ftraction.x = 100*(car->throttle - car->brake*SGN(velocity.x));	
	//ftraction.y = 0;
	//if(rear_slip)
	//	ftraction.x *= 0.5;

	float throttle = 60.0f;
	float brake = 0;
	 
	float ftractionZ = 100.0f*(throttle - brake*_chgsign(velocity.z));	
	//float ftractionZ = 100.0f*(throttle - brake*velocity.z);
	float ftractionX = 0.0f;
	if(rear_slip)
	   ftractionZ *= 0.5f;

//Forces and torque on body
	
	//drag and rolling resistance
	//resistance.x = -( RESISTANCE*velocity.x + DRAG*velocity.x*ABS(velocity.x) );
	//resistance.y = -( RESISTANCE*velocity.y + DRAG*velocity.y*ABS(velocity.y) );
	float resistanceX = -( RESISTANCE*velocity.x + DRAG*velocity.x*abs(velocity.x) );
	float resistanceZ = -( RESISTANCE*velocity.z + DRAG*velocity.z*abs(velocity.z) );

	//sum forces
	//force.x = ftraction.x + sin(car->steerangle) * flatf.x + flatr.x + resistance.x;
	//force.y = ftraction.y + cos(car->steerangle) * flatf.y + flatr.y + resistance.y;

	float forceZ = ftractionZ + sin(steeringAngle) * flatX + flatrX + resistanceZ;
	float forceX = ftractionX + cos(steeringAngle) * flatY + flatrY + resistanceX;

	//torque on body from lateral forces
	//torque = car->cartype->b * flatf.y - car->cartype->c * flatr.y;

	float torque = 1.0f * flatY - 1.0f * flatrY;

    //Acceleration
	
	//Newton F = m.a, therefore a = F/m
	//acceleration.x = force.x/car->cartype->mass;
	//acceleration.y = force.y/car->cartype->mass;
	
	//angular_acceleration = torque / car->cartype->inertia;

	D3DVECTOR accelerationVector;
	accelerationVector.x = forceX/mass;
	accelerationVector.y = 0.0f;
	accelerationVector.z = forceZ/mass;
	
	float angular_acceleration = torque / inertia;

    //Velocity and position
	
	//transform acceleration from car reference frame to world reference frame
	//acceleration_wc.x =  cs * acceleration.y + sn * acceleration.x;
	//acceleration_wc.y = -sn * acceleration.y + cs * acceleration.x;

	//accelVect = matLib->UpdatePolygonVertexRotationOnly(frameObj->List_frameMatrix.m,accelVect);//local velocity vector

	//velocity is integrated acceleration
	//
	//car->velocity_wc.x += delta_t * acceleration_wc.x;
	//car->velocity_wc.y += delta_t * acceleration_wc.y;

	D3DVECTOR transformLocal;
	D3DVECTOR newVelocityVect = velocity;

	newVelocityVect.x += DELTA_T * accelerationVector.x;
	newVelocityVect.z += DELTA_T * accelerationVector.z;

	//position is integrated velocity
	//
	//car->position_wc.x += delta_t * car->velocity_wc.x;
	//car->position_wc.y += delta_t * car->velocity_wc.y;

	transformLocal.x = -newVelocityVect.x;
	transformLocal.y = 0.0f;
	transformLocal.z = -newVelocityVect.z;

	frameObj->AddTranslationBefore(transformLocal.x,transformLocal.y,transformLocal.z);

	//D3DMATRIX invMat;
	//frameObj->GetInverseTransform(&invMat);
	//newVelocityVect = matLib->UpdatePolygonVertexRotationOnly(invMat.m,newVelocityVect);

    /*textSys->AddLine("Before Last Pos",phyObj->List_lastPosition);

	phyObj->List_lastPosition.x = frameObj->List_frameMatrix._41 - newVelocityVect.x;
    phyObj->List_lastPosition.y = frameObj->List_frameMatrix._42 - newVelocityVect.y;
    phyObj->List_lastPosition.z = frameObj->List_frameMatrix._43 - newVelocityVect.z;
	
	textSys->AddLine("after Last Pos",phyObj->List_lastPosition);


//Angular velocity and heading

	//integrate angular acceleration to get angular velocity
	//
	//car->angularvelocity += delta_t * angular_acceleration;

	phyObj->m_angularvelocity += DELTA_T * angular_acceleration;

	//integrate angular velocity to get angular orientation
	//
	//car->angle += delta_t * car->angularvelocity ;

	float curRotationY = DELTA_T * phyObj->m_angularvelocity;

	D3DMATRIX rotCur;
	matLib->RotateY(curRotationY * 57.3f,rotCur.m);
	frameObj->AddTransformBefore(rotCur);


	textSys->AddLine("Yaw Speed",yawspeed);
	textSys->AddLine("Trans Local Vect",transformLocal);
	textSys->AddLine("Local Velocity Vect",velocity);
	textSys->AddLine("CurRotation Radians",curRotationY);
	textSys->AddLine("CurRotation Angle",curRotationY * 57.3f);
}*/
/*//----------------------------------------------------------------------------------------//
void CCollision_RaySystem::DoCarPhysics(    double force,
											void * frameObjVoid,
											MatrixARB * matLib,
											void * physicsObject,
											double & envGravity,
											void * textSysVoid)
{

    CFrameObj * frameObj = (CFrameObj *)frameObjVoid;
	CMovementCaps * phyObj = (CMovementCaps *)physicsObject;
	CtextReadoutObj * textSys = (CtextReadoutObj *)textSysVoid;

    //float result = atan2(8.0,2.0);//75 X/Z
	//float result = atan2(-8.0,2.0);//-75 X/Z

    //sn = sin(car->angle);//used to transform vectors to local frame view
	//cs = cos(car->angle);//used to transform vectors to local frame view

	//SAE convention: x is to the front of the car, y is to the right, z is down
	
	//transform velocity in world reference frame to velocity in car reference frame
	D3DVECTOR velocity = matLib->GetLocalPositionFromMatrixViewRotOnly(frameObj->List_frameMatrix,phyObj->List_curVelocityVect);
	//D3DVECTOR wldVelocity = phyObj->List_curVelocityVect;
	//velocity.x =  cs * car->velocity_wc.y + sn * car->velocity_wc.x;
	//velocity.y = -sn * car->velocity_wc.y + cs * car->velocity_wc.x;

//Lateral force on wheels
//	
	//Resulting velocity of the wheels as result of the yaw rate of the car body
	//v = yawrate * r where r is distance of wheel to CG (approx. half wheel base)
	//yawrate (ang.velocity) must be in rad/s
	//
	float	DRAG		= 5.0;		 	//factor for air resistance (drag) 
    float	RESISTANCE	= 30.0;			//factor for rolling resistance 
    float	CA_R		= -5.20;		//cornering stiffness 
    float	CA_F		= -5.0;			//cornering stiffness 
    float	MAX_GRIP	= 2.0;
	float   mass        = 1500.0f;
	float   inertia     = 1500.0f;
	float   DELTA_T		= 0.01f;

	float curWheelRotation = 0.0f;
	float distFBTires = 0.0f;
    GetCurSteeringRotationData(curWheelRotation,distFBTires);
	
	float yawspeed = this->m_rays[0].m_wheel->m_wheelBase * 0.5 * phyObj->m_angularvelocity;//yawspeed = car->cartype->wheelbase * 0.5 * car->angularvelocity;		
	
    
	//if( velocity.x == 0 )		//TODO: fix singularity
	//	rot_angle = 0;
	//else
	//	rot_angle = atan2( yawspeed, velocity.x);

	float rot_angle = 0.0f;
	if( velocity.x != 0.0f )		//TODO: fix singularity
		rot_angle = atan2( yawspeed, velocity.z);
   

	//Calculate the side slip angle of the car (a.k.a. beta)
	//if( velocity.x == 0 )		//TODO: fix singularity
	//	sideslip = 0;
	//else
	//	sideslip = atan2( velocity.y, velocity.x);

	float sideslip = 0.0f;
	if( velocity.x != 0.0f )		//TODO: fix singularity
		sideslip = atan2( velocity.x, velocity.z);

	//Calculate slip angles for front and rear wheels (a.k.a. alpha)
	//slipanglefront = sideslip + rot_angle - car->steerangle;
	//slipanglerear  = sideslip - rot_angle;
	float steeringAngle = curWheelRotation / 57.3;
	steeringAngle = -steeringAngle;

	float slipanglefront = sideslip + rot_angle - steeringAngle;
	float slipanglerear  = sideslip - rot_angle;

	//weight per axle = half car mass times 1G (=9.8m/s^2) 
	//weight = car->cartype->mass * 9.8 * 0.5;	

	float weight = mass * 9.8 * 0.5;
	
	//lateral force on front wheels = (Ca * slip angle) capped to friction circle * load
	//flatf.x = 0;
	//flatf.y = CA_F * slipanglefront;
	//flatf.y = MIN(MAX_GRIP, flatf.y);
	//flatf.y = MAX(-MAX_GRIP, flatf.y);
	//flatf.y *= weight;
	//if(front_slip)
	//flatf.y *= 0.5;

    bool front_slip = false;
	bool rear_slip = false;

	float flatX,float flatY;
	flatX = 0;
	flatY = CA_F * slipanglefront;
	flatY = min(MAX_GRIP, flatY);
	flatY = max(-MAX_GRIP, flatY);
	flatY *= weight;
	if(front_slip)
		flatY *= 0.5;

	//lateral force on rear wheels
	//flatr.x = 0;
	//flatr.y = CA_R * slipanglerear;
	//flatr.y = MIN(MAX_GRIP, flatr.y);
	//flatr.y = MAX(-MAX_GRIP, flatr.y);
	//flatr.y *= weight;
	//if(rear_slip)
	//	flatr.y *= 0.5;

    float flatrX,float flatrY;
	flatrX = 0;
	flatrY = CA_R * slipanglerear;
	flatrY = min(MAX_GRIP, flatrY);
	flatrY = max(-MAX_GRIP, flatrY);
	flatrY *= weight;
	if(rear_slip)
		flatrY *= 0.5;

	//longtitudinal force on rear wheels - very simple traction model
	//ftraction.x = 100*(car->throttle - car->brake*SGN(velocity.x));	
	//ftraction.y = 0;
	//if(rear_slip)
	//	ftraction.x *= 0.5;

	float throttle = 60.0f;
	float brake = 0;
	 
	float ftractionZ = 100.0f*(throttle - brake*_chgsign(velocity.z));	
	float ftractionX = 0.0f;
	if(rear_slip)
	   ftractionZ *= 0.5f;

//Forces and torque on body
	
	//drag and rolling resistance
	//resistance.x = -( RESISTANCE*velocity.x + DRAG*velocity.x*ABS(velocity.x) );
	//resistance.y = -( RESISTANCE*velocity.y + DRAG*velocity.y*ABS(velocity.y) );
	float resistanceX = -( RESISTANCE*velocity.x + DRAG*velocity.x*abs(velocity.x) );
	float resistanceZ = -( RESISTANCE*velocity.z + DRAG*velocity.z*abs(velocity.z) );

	//sum forces
	//force.x = ftraction.x + sin(car->steerangle) * flatf.x + flatr.x + resistance.x;
	//force.y = ftraction.y + cos(car->steerangle) * flatf.y + flatr.y + resistance.y;

	float forceZ = ftractionZ + sin(steeringAngle) * flatX + flatrX + resistanceZ;
	float forceX = ftractionX + cos(steeringAngle) * flatY + flatrY + resistanceX;

	//torque on body from lateral forces
	//torque = car->cartype->b * flatf.y - car->cartype->c * flatr.y;

	float torque = 1.0f * flatY - 1.0f * flatrY;

    //Acceleration
	
	//Newton F = m.a, therefore a = F/m
	//acceleration.x = force.x/car->cartype->mass;
	//acceleration.y = force.y/car->cartype->mass;
	
	//angular_acceleration = torque / car->cartype->inertia;

	float accelerationZ = forceZ/mass;
	float accelerationX = forceX/mass;
	
	float angular_acceleration = torque / inertia;

    //Velocity and position
	
	//transform acceleration from car reference frame to world reference frame
	//acceleration_wc.x =  cs * acceleration.y + sn * acceleration.x;
	//acceleration_wc.y = -sn * acceleration.y + cs * acceleration.x;

	//accelVect = matLib->UpdatePolygonVertexRotationOnly(frameObj->List_frameMatrix.m,accelVect);//local velocity vector

	//velocity is integrated acceleration
	//
	//car->velocity_wc.x += delta_t * acceleration_wc.x;
	//car->velocity_wc.y += delta_t * acceleration_wc.y;

	D3DVECTOR transformLocal;

	transformLocal.x = DELTA_T * (velocity.x + DELTA_T * accelerationX);
	transformLocal.y = 0.0f;
	transformLocal.z = DELTA_T * (velocity.z + DELTA_T * accelerationZ);
	

	//position is integrated velocity
	//
	//car->position_wc.x += delta_t * car->velocity_wc.x;
	//car->position_wc.y += delta_t * car->velocity_wc.y;

	frameObj->AddTranslationBefore(transformLocal.x,transformLocal.y,transformLocal.z);

	


//Angular velocity and heading

	//integrate angular acceleration to get angular velocity
	//
	//car->angularvelocity += delta_t * angular_acceleration;

	phyObj->m_angularvelocity += DELTA_T * angular_acceleration;

	//integrate angular velocity to get angular orientation
	//
	//car->angle += delta_t * car->angularvelocity ;

	float curRotationY = DELTA_T * phyObj->m_angularvelocity;

	D3DMATRIX rotCur;
	matLib->RotateY(curRotationY * 57.3f,rotCur.m);
	frameObj->AddTransformBefore(rotCur);


	textSys->AddLine("Yaw Speed",yawspeed);
	textSys->AddLine("Trans Local Vect",transformLocal);
	textSys->AddLine("Local Velocity Vect",velocity);
	textSys->AddLine("CurRotation Radians",curRotationY);
	textSys->AddLine("CurRotation Angle",curRotationY * 57.3f);
}*/

//----------------------------------------------------------------------------------------//
	void CCollision_RaySystem::DoCarPhysics2D(  double forceZ,
												void * frameObjVoid,
												MatrixARB * matLib,
												void * physicsObject,
												double & envGravity,
												void * textSysVoid)
{

    CFrameObj * frameObj = (CFrameObj *)frameObjVoid;
	CMovementCaps * phyObj = (CMovementCaps *)physicsObject;
	CtextReadoutObj * textSys = (CtextReadoutObj *)textSysVoid;

	float curWheelRotation = 0.0f;
	float distFBTires = 0.0f;
    GetCurSteeringRotationData(curWheelRotation,distFBTires);

	float	DRAG		= 5.0f;		 		/* factor for air resistance (drag) 	*/
    float	RESISTANCE	= 30.0f;			/* factor for rolling resistance */
    float	CA_R		= -5.20f;			/* cornering stiffness */
    float	CA_F		= -5.0f;			/* cornering stiffness */
    float	MAX_GRIP	= 2.0f;
	float   mass        = 1500.0f;
	float   inertia     = 1500.0f;
	float   DELTA_T		= 0.01f;
	float   throttle    = (float)forceZ;
	float   brake       = 0.0f;
	float   c_b         = 3.0f;//in m, distance from CG to front axle
	float   c_c         = 3.0f;//in m, idem to rear axle

    float sn = sin(phyObj->m_angleTemp);
	float cs = cos(phyObj->m_angleTemp);

	//SAE convention: x is to the front of the car, y is to the right, z is down
	
	//transform velocity in world reference frame to velocity in car reference frame
	D3DVECTOR velocity;
	velocity.x =  cs * phyObj->m_velocity_wc.y + sn * phyObj->m_velocity_wc.x;
	velocity.y = -sn * phyObj->m_velocity_wc.y + cs * phyObj->m_velocity_wc.x;

//Lateral force on wheels
//	
	//Resulting velocity of the wheels as result of the yaw rate of the car body
	//v = yawrate * r where r is distance of wheel to CG (approx. half wheel base)
	//yawrate (ang.velocity) must be in rad/s
	//
	float yawspeed = this->m_rays[0].m_wheel->m_wheelBase * 0.5f * (float)phyObj->m_angularvelocity;	
    float rot_angle = 0.0f;

	if( velocity.x == 0 )		//TODO: fix singularity
		rot_angle = 0;
	else
		rot_angle = atan2( yawspeed, velocity.x);

	//Calculate the side slip angle of the car (a.k.a. beta)
	float sideslip = 0.0f;
	if( velocity.x == 0 )		//TODO: fix singularity
		sideslip = 0;
	else
		sideslip = atan2( velocity.y, velocity.x);		

	//Calculate slip angles for front and rear wheels (a.k.a. alpha)
    float steeringAngle = curWheelRotation/57.3f;
	//steeringAngle = 0.0f;
    textSys->AddLine("PRE steering Angle",steeringAngle);
	
	/*if( curWheelRotation < -20.0f )
	    steeringAngle = -(matLib->PI/32.0);
	else if( curWheelRotation >  20.0f )
		steeringAngle = matLib->PI/32.0;
	else
		steeringAngle = 0.0f;*/


	float slipanglefront = sideslip + rot_angle - steeringAngle;//car->steerangle;
	float slipanglerear  = sideslip - rot_angle;

	//weight per axle = half car mass times 1G (=9.8m/s^2) 
	float weight = mass * 9.8f * 0.5f;	
	
	//lateral force on front wheels = (Ca * slip angle) capped to friction circle * load
	D3DVECTOR flatf,flatr;
    bool front_slip = false;
	bool rear_slip = false;


	flatf.x = 0;
	flatf.y = CA_F * slipanglefront;
	flatf.y = min(MAX_GRIP, flatf.y);
	flatf.y = max(-MAX_GRIP, flatf.y);
	flatf.y *= weight;
	if(front_slip)
		flatf.y *= 0.5;

	//lateral force on rear wheels
	flatr.x = 0;
	flatr.y = CA_R * slipanglerear;
	flatr.y = min(MAX_GRIP, flatr.y);
	flatr.y = max(-MAX_GRIP, flatr.y);
	flatr.y *= weight;
	if(rear_slip)
		flatr.y *= 0.5;

	//longtitudinal force on rear wheels - very simple traction model
	D3DVECTOR ftraction;
	ftraction.x = 100*(throttle - brake*velocity.x);	
	ftraction.y = 0;
	if(rear_slip)
		ftraction.x *= 0.5;

//Forces and torque on body
	
	//drag and rolling resistance
	D3DVECTOR resistance,force;
	resistance.x = -( RESISTANCE*velocity.x + DRAG*velocity.x*abs(velocity.x) );
	resistance.y = -( RESISTANCE*velocity.y + DRAG*velocity.y*abs(velocity.y) );

	//sum forces
	force.x = ftraction.x + sin(steeringAngle) * flatf.x + flatr.x + resistance.x;
	force.y = ftraction.y + cos(steeringAngle) * flatf.y + flatr.y + resistance.y;	

	//torque on body from lateral forces
	float torque = c_b * flatf.y - c_c * flatr.y;

//Acceleration
	
	//Newton F = m.a, therefore a = F/m
	D3DVECTOR acceleration;
	acceleration.x = force.x/mass;
	acceleration.y = force.y/mass;
	
	float angular_acceleration = torque / inertia;

//Velocity and position
	
	//transform acceleration from car reference frame to world reference frame
	D3DVECTOR acceleration_wc;
	acceleration_wc.x =  cs * acceleration.y + sn * acceleration.x;
	acceleration_wc.y = -sn * acceleration.y + cs * acceleration.x;

	//velocity is integrated acceleration
	//
	phyObj->m_velocity_wc.x += DELTA_T * acceleration_wc.x;
	phyObj->m_velocity_wc.y += DELTA_T * acceleration_wc.y;

	//position is integrated velocity
	//
	//car->position_wc.x += DELTA_T * car->velocity_wc.x;
	//car->position_wc.y += DELTA_T * car->velocity_wc.y;

	frameObj->List_frameMatrix._41 += DELTA_T * phyObj->m_velocity_wc.x;
	frameObj->List_frameMatrix._43 += DELTA_T * phyObj->m_velocity_wc.y;

	//frameObj->AddTranslationAfter(translationY,0.0f,translationX);


//Angular velocity and heading

	//integrate angular acceleration to get angular velocity
	//
	phyObj->m_angularvelocity += DELTA_T * angular_acceleration;

	//integrate angular velocity to get angular orientation
	//
	phyObj->m_angleTemp += DELTA_T * (float)phyObj->m_angularvelocity;
    float rotateY = DELTA_T * (float)phyObj->m_angularvelocity;

	/*float nmo = 360/57.3;
    if(fabs(phyObj->m_angleTemp) > nmo)
	{
       int count = fabs(phyObj->m_angleTemp) / nmo;
 
	   if(phyObj->m_angleTemp < 0.0f)
	   {
          phyObj->m_angleTemp = phyObj->m_angleTemp + nmo*count;

	   }
	   else
	   {
          phyObj->m_angleTemp = phyObj->m_angleTemp - nmo*count;


	   }


	}*/

	//D3DVECTOR position;
	//frameObj->GetPosition(NULL,&position);
	D3DMATRIX rotCur;
	matLib->RotateY(phyObj->m_angleTemp * 57.3f,rotCur.m);
	rotCur._41 = frameObj->List_frameMatrix._41;
	rotCur._42 = frameObj->List_frameMatrix._42;
	rotCur._43 = frameObj->List_frameMatrix._43;
	frameObj->List_frameMatrix = rotCur;
	//frameObj->AddTransformBefore(rotCur);
	/*frameObj->AddTransformReplace(rotCur);
	frameObj->List_frameMatrix._41 = position.x;
	frameObj->List_frameMatrix._42 = position.y;
	frameObj->List_frameMatrix._43 = position.z;*/

    if(GetTickCount()-m_stamp > 1000)
	{
	m_stamp = GetTickCount();
    textSys->AddLine("steering Angle",steeringAngle);
	textSys->AddLine("car Angle",phyObj->m_angleTemp);
	textSys->AddLine("Force Vect",force);
	textSys->AddLine("Angular accel",(float)phyObj->m_angularvelocity);
	textSys->AddLine("wldVelocity",phyObj->m_velocity_wc);
	}
}

//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
//#include <afxtempl.h>
#include "matrixarb.h"
#include "CBoneClass.h"
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
CBoneObject::CBoneObject()
{
    m_boneFrame                = NULL;
	m_animationList            = NULL;
	m_hiarchyMesh              = NULL;
	m_secondaryOverridable     = TRUE;
	m_referencedAnimList       = FALSE;

    ZeroMemory(&m_lastUpVector,sizeof(D3DVECTOR));
	ZeroMemory(&m_lastDirVector,sizeof(D3DVECTOR));
	ZeroMemory(&m_lastPosition,sizeof(D3DVECTOR));

	ZeroMemory(&m_targetPosition,sizeof(D3DVECTOR));
	ZeroMemory(&m_targetUpVector,sizeof(D3DVECTOR));
	ZeroMemory(&m_targetDirVector,sizeof(D3DVECTOR));
	m_lastToTargetDuration  = 0;
	m_lastToTargetTimeStamp = 0;

	m_lastPositionsAreInitialized = FALSE;

	GL_matrixFunctions.InitMat(m_startPosition.m); 
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Deconstructor---------------------------------//
void CBoneObject::SafeDelete()
{
  if(m_boneFrame){
     m_boneFrame->SafeDelete();
     delete m_boneFrame;
     m_boneFrame=0;
  }
  if(m_hiarchyMesh){
     m_hiarchyMesh->SafeDelete();
     delete m_hiarchyMesh;
     m_hiarchyMesh=0;
  }

  if(!m_referencedAnimList)
  if(m_animationList){
     m_animationList->SafeDelete();
     delete m_animationList;
	 m_animationList=0;
  }

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
CBoneObject * CBoneList::GetBoneObjectByName(CString name,int * index)
{
  int trace = 0;
  for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
  {
      CBoneObject * boneObject = (CBoneObject *)GetNext(posLoc);
      *index = trace;
	  if(boneObject->m_boneName == name)
		 return boneObject;
	     trace++;
  }
return NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
CBoneObject * CBoneList::GetBoneByIndex(int index)
{
  if(index < 0)
     return NULL;

  POSITION posLoc = FindIndex(index);
  if(!posLoc)
	  return NULL;

  CBoneObject * boneObject = (CBoneObject *)GetAt(posLoc);
  return boneObject;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CBoneObject::Clone(CBoneObject ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
  CBoneObject * cloneReturn = new CBoneObject();
  //frame
  cloneReturn->m_boneFrame = new CFrameObj(NULL);
  cloneReturn->m_boneFrame->AddTransformReplace(m_startPosition);
  cloneReturn->m_boneFrame->GetTransform(NULL,&cloneReturn->m_startPosition);
  cloneReturn->m_parentName = m_parentName;
  cloneReturn->m_boneName = m_boneName;
  cloneReturn->m_secondaryOverridable = m_secondaryOverridable;
  //hiarch mesh
  if(m_hiarchyMesh)
     m_hiarchyMesh->Clone(&cloneReturn->m_hiarchyMesh,g_pd3dDevice);
  //animation data
  if(m_animationList){
     //m_animationList->Clone(&cloneReturn->m_animationList);
	 cloneReturn->m_animationList = m_animationList;
	 cloneReturn->m_referencedAnimList = TRUE;
  }

  *clone = cloneReturn;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CBoneObject::SetTransformDataForAnimation(long duration,
											          D3DVECTOR & lastPosition,
											          D3DVECTOR & lastUpVector,
											          D3DVECTOR & lastDirVector,
											          D3DVECTOR & targetPosition,
											          D3DVECTOR & targetUpVector,
											          D3DVECTOR & targetDirVector,
											          long & currentTime)
{
    CopyMemory(&m_lastUpVector,&lastUpVector,sizeof(D3DVECTOR));
	CopyMemory(&m_lastPosition,&lastPosition,sizeof(D3DVECTOR));
	CopyMemory(&m_lastDirVector,&lastDirVector,sizeof(D3DVECTOR));

	CopyMemory(&m_targetUpVector,&targetUpVector,sizeof(D3DVECTOR));
	CopyMemory(&m_targetPosition,&targetPosition,sizeof(D3DVECTOR));
	CopyMemory(&m_targetDirVector,&targetDirVector,sizeof(D3DVECTOR));

	m_lastToTargetDuration        = duration;
	m_lastToTargetTimeStamp       = currentTime;
	m_lastPositionsAreInitialized = TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
BOOL CBoneObject::ApplyAnimation(int animation,int frame)
{
if(!m_animationList)//no animation on bone
    return FALSE;

POSITION posLocal = m_animationList->FindIndex(animation);
if(!posLocal)
    return FALSE;
CBoneAnimationObject * animPtr = (CBoneAnimationObject *)m_animationList->GetAt(posLocal);

GL_matrixFunctions.SetOrientation(&m_boneFrame->List_frameMatrix,
								   GL_matrixFunctions.Normalize(animPtr->m_animationData[frame].dirVect.GetD3DVECTOR()),
								   GL_matrixFunctions.Normalize(animPtr->m_animationData[frame].upVect.GetD3DVECTOR()));
m_boneFrame->List_frameMatrix._41 = animPtr->m_animationData[frame].position.x;
m_boneFrame->List_frameMatrix._42 = animPtr->m_animationData[frame].position.y;
m_boneFrame->List_frameMatrix._43 = animPtr->m_animationData[frame].position.z;

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
BOOL CBoneObject::ApplyAnimationByTime(int animation,long time)
{
/*if(!m_animationList)//no animation on bone
    return FALSE;

POSITION posLocal = m_animationList->FindIndex(animation);
if(!posLocal)
    return FALSE;
CBoneAnimationObject * animPtr = (CBoneAnimationObject *)m_animationList->GetAt(posLocal);
//interpolated variables
animPtr->m_baseFrame = time/animPtr->m_millisecondsPerFrame;
animPtr->m_targetFrame = animPtr->m_baseFrame+1;
if(animPtr->m_targetFrame >= animPtr->m_totalFrames)
   animPtr->m_targetFrame=0;
animPtr->m_currentMS = time - (animPtr->m_baseFrame*animPtr->m_millisecondsPerFrame);
//UpdateBoneByPosition(animation,animPtr->m_currentMS,animPtr->m_baseFrame,animPtr->m_targetFrame);
UpdateBone(animPtr);*/
return TRUE;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
void CBoneObject::UpdateBone(CBoneAnimationObject * animPtr)
{
/*D3DVECTOR intPosition,intDir,intUp;

intDir = GL_matrixFunctions.InterpolatePoints(
		                                        GL_matrixFunctions.Normalize(animPtr->m_animationData[animPtr->m_baseFrame].dirVect.GetD3DVECTOR()), 
						                        GL_matrixFunctions.Normalize(animPtr->m_animationData[animPtr->m_targetFrame].dirVect.GetD3DVECTOR()),
					                            animPtr->m_millisecondsPerFrame, 
					                            animPtr->m_currentMS);
intUp = GL_matrixFunctions.InterpolatePoints(
		                                        GL_matrixFunctions.Normalize(animPtr->m_animationData[animPtr->m_baseFrame].upVect.GetD3DVECTOR()), 
						                        GL_matrixFunctions.Normalize(animPtr->m_animationData[animPtr->m_targetFrame].upVect.GetD3DVECTOR()),
					                            animPtr->m_millisecondsPerFrame, 
					                            animPtr->m_currentMS);

intPosition = GL_matrixFunctions.InterpolatePoints(
		                                        animPtr->m_animationData[animPtr->m_baseFrame].position, 
						                        animPtr->m_animationData[animPtr->m_targetFrame].position, 
					                            animPtr->m_millisecondsPerFrame, 
					                            animPtr->m_currentMS);

intUp = GL_matrixFunctions.Normalize(intUp);
intDir = GL_matrixFunctions.Normalize(intDir);

GL_matrixFunctions.SetOrientation(&m_boneFrame->List_frameMatrix,
								   intDir,
								   intUp);
m_boneFrame->List_frameMatrix._41 = intPosition.x;
m_boneFrame->List_frameMatrix._42 = intPosition.y;
m_boneFrame->List_frameMatrix._43 = intPosition.z;*/
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
void CBoneObject::SetBoneToAnimationFrame(int currentAnimation,
										  int targetFrame,
										  long transitionDuration,
										  long currentTime)
{

   POSITION posLoc = m_animationList->FindIndex(currentAnimation);
    if(!posLoc)
		return;
        CBoneAnimationObject * animBase = (CBoneAnimationObject *)m_animationList->GetAt(posLoc);

		//animBase->m_currentTimePassed = animBase->m_millisecondsPerFrame * animBase->m_baseFrame;//Experiment

        if(targetFrame >= animBase->m_totalFrames)
		   targetFrame = animBase->m_totalFrames-1;
		   
		if(targetFrame < 0)
		   targetFrame = 0;


		if(!m_lastPositionsAreInitialized)
		{//need to be initialized
		          SetTransformDataForAnimation(transitionDuration,
											   animBase->m_animationData[targetFrame].position,
											   animBase->m_animationData[targetFrame].upVect.GetD3DVECTOR(),
											   animBase->m_animationData[targetFrame].dirVect.GetD3DVECTOR(),
											   animBase->m_animationData[targetFrame].position,
											   animBase->m_animationData[targetFrame].upVect.GetD3DVECTOR(),
											   animBase->m_animationData[targetFrame].dirVect.GetD3DVECTOR(),
											   currentTime);
		}//end need to be initialized
		else{//progress
			      D3DVECTOR lastUp,LastDir,lastPosition;
			      m_boneFrame->GetOrientation(&LastDir,&lastUp);
				  m_boneFrame->GetPosition(NULL,&lastPosition);
                  SetTransformDataForAnimation(transitionDuration,
											   lastPosition,
											   lastUp,
											   LastDir,
											   animBase->m_animationData[targetFrame].position,
											   animBase->m_animationData[targetFrame].upVect.GetD3DVECTOR(),
											   animBase->m_animationData[targetFrame].dirVect.GetD3DVECTOR(),
											   currentTime);
		}//end progress
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
void CBoneObject::UpdateBoneByPosition(int baseAnim,
									   int focusAnim,
									   long millisecondInto,
									   int base,
									   int target,
									   int milisecondsPerFrame)
{

/*POSITION posLoc = m_animationList->FindIndex(baseAnim);
POSITION posLoc2 = m_animationList->FindIndex(focusAnim);
if(posLoc != NULL && posLoc2 != NULL){//valid
CBoneAnimationObject * animBase = (CBoneAnimationObject *)m_animationList->GetAt(posLoc);
CBoneAnimationObject * animFocus = (CBoneAnimationObject *)m_animationList->GetAt(posLoc2);
D3DVECTOR intPosition,intDir,intUp;

if(millisecondInto > milisecondsPerFrame)
   millisecondInto = milisecondsPerFrame;


intDir = GL_matrixFunctions.InterpolatePoints(
		                                        GL_matrixFunctions.Normalize(animBase->m_animationData[animBase->m_baseFrame].dirVect.GetD3DVECTOR()), 
						                        GL_matrixFunctions.Normalize(animFocus->m_animationData[animFocus->m_targetFrame].dirVect.GetD3DVECTOR()),
					                            milisecondsPerFrame, 
					                            millisecondInto);
intUp = GL_matrixFunctions.InterpolatePoints(
		                                        GL_matrixFunctions.Normalize(animBase->m_animationData[animBase->m_baseFrame].upVect.GetD3DVECTOR()), 
						                        GL_matrixFunctions.Normalize(animFocus->m_animationData[animFocus->m_targetFrame].upVect.GetD3DVECTOR()),
					                            milisecondsPerFrame, 
					                            millisecondInto);

intPosition = GL_matrixFunctions.InterpolatePoints(
		                                        animBase->m_animationData[animBase->m_baseFrame].position, 
						                        animFocus->m_animationData[animFocus->m_targetFrame].position, 
					                            milisecondsPerFrame, 
					                            millisecondInto);


//intDir = animFocus->m_animationData[animFocus->m_targetFrame].dirVect;
//intUp = animFocus->m_animationData[animFocus->m_targetFrame].upVect;
//intPosition = animFocus->m_animationData[animFocus->m_targetFrame].position;


GL_matrixFunctions.SetOrientation(&m_boneFrame->List_frameMatrix,
								   intDir,
								   intUp);
m_boneFrame->List_frameMatrix._41 = intPosition.x;
m_boneFrame->List_frameMatrix._42 = intPosition.y;
m_boneFrame->List_frameMatrix._43 = intPosition.z;
}//end valid*/

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
/*void CBoneObject::UpdateBoneByMs(int baseAnim,
							     int focusAnim,
								 long baseAnimTime,
								 long focusAnimTime,
								 long stepCount,
								 long currentTime)
{

POSITION posLoc = m_animationList->FindIndex(baseAnim);
POSITION posLoc2 = m_animationList->FindIndex(focusAnim);

 if(!posLoc)
	 return;
 if(!posLoc2)
	 return;

     CBoneAnimationObject * animBase = (CBoneAnimationObject *)m_animationList->GetAt(posLoc);
     CBoneAnimationObject * animFocus = (CBoneAnimationObject *)m_animationList->GetAt(posLoc2);
     D3DVECTOR startPosition,startDir,startUp;
     D3DVECTOR targetPosition,targetDir,targetUp;
	 D3DVECTOR intPosition,intDir,intUp;


     animBase->GetBonePositionalDataByTime(baseAnimTime,
									       startPosition,
									       startUp,
									       startDir);

	 animFocus->GetBonePositionalDataByTime(focusAnimTime,
									        targetPosition,
									        targetUp,
									        targetDir);

	 intDir = GL_matrixFunctions.InterpolatePoints(
		                                        startDir, 
						                        targetDir,
					                            stepCount, 
					                            currentTime);
     intUp = GL_matrixFunctions.InterpolatePoints(
		                                        startUp, 
						                        targetUp,
					                            stepCount, 
					                            currentTime);

     intPosition = GL_matrixFunctions.InterpolatePoints(
		                                        startPosition, 
						                        targetPosition, 
					                            stepCount, 
					                            currentTime);


GL_matrixFunctions.SetOrientation(&m_boneFrame->List_frameMatrix,
								   intDir,
								   intUp);
m_boneFrame->List_frameMatrix._41 = intPosition.x;
m_boneFrame->List_frameMatrix._42 = intPosition.y;
m_boneFrame->List_frameMatrix._43 = intPosition.z;
}*/
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Serialize-------------------------------------//
void CBoneObject::Serialize( CArchive& ar )
{
int miscInt = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << m_parentName
		  << m_animationList
		  << m_secondaryOverridable
		  << m_boneName
		  << m_hiarchyMesh;
	      
	      for(int loop=0;loop<4;loop++)
              for(int loop2=0;loop2<4;loop2++)
              ar << m_startPosition.m[loop][loop2];
	     
	}
	else
	{
	  
       ar >> m_parentName
		  >> m_animationList
		  >> m_secondaryOverridable
		  >> m_boneName
		  >> m_hiarchyMesh;

       for(int loop=0;loop<4;loop++)
           for(int loop2=0;loop2<4;loop2++)
               ar >> m_startPosition.m[loop][loop2];

	   m_boneFrame = new CFrameObj(NULL);
	   m_boneFrame->AddTransformReplace(m_startPosition);

	   ZeroMemory(&m_lastUpVector,sizeof(D3DVECTOR));
	   ZeroMemory(&m_lastDirVector,sizeof(D3DVECTOR));
	   ZeroMemory(&m_lastPosition,sizeof(D3DVECTOR));
	   ZeroMemory(&m_targetPosition,sizeof(D3DVECTOR));
	   ZeroMemory(&m_targetUpVector,sizeof(D3DVECTOR));
	   ZeroMemory(&m_targetDirVector,sizeof(D3DVECTOR));
	   m_lastToTargetDuration  = 0;
	   m_lastToTargetTimeStamp = 0;
	   m_lastPositionsAreInitialized = FALSE;
	   m_referencedAnimList       = FALSE;

	   //m_secondaryOverridable = TRUE;
       
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CBoneList::Clone(CBoneList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   CBoneList * newList = new CBoneList();
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBoneObject * bnPtr = (CBoneObject *)GetNext(posLoc);
        CBoneObject * newObject = NULL;  
		bnPtr->Clone(&newObject,g_pd3dDevice);
		newList->AddTail(newObject);
	}
   *clone = newList;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CBoneList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBoneObject * bnPtr = (CBoneObject *)GetNext(posLoc);
        bnPtr->SafeDelete();
		delete bnPtr;
		bnPtr = 0;
	}
    RemoveAll();
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CBoneList::Invalidate()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBoneObject * bnPtr = (CBoneObject *)GetNext(posLoc);
        if(bnPtr->m_hiarchyMesh)
           bnPtr->m_hiarchyMesh->Invalidate();
	}
}




IMPLEMENT_SERIAL(CBoneObject,  CObject, 0)
IMPLEMENT_SERIAL(CBoneList,   CObject, 0)
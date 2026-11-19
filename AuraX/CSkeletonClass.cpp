#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "matrixarb.h"
#include "math.h"

#include "CSkeletalClass.h"
#include "CSkeletonClassFileIO.h"
#include "CAssimpModelImporter.h"   // SetLastError: tell EngineX why an auto-rig failed
#include <vector>
#include <string>
#include <d3dx9.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------Female anatomy bone helpers---------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
// Shared helpers for the R_boob / L_boob / R_butt / L_butt anatomy bones and
// for the 1 click auto rigger.  They only touch the bone list / frame
// structures, hence file scope.

// Maximum share of a vertex weight the anatomy bones are allowed to take, so
// the bust / glutes always keep a connection to the chest / pelvis bones and
// the surface can never tear at the edge of the sculpted region.
static const float FEMALE_ANATOMY_MAX_SKIN_WEIGHT = 0.72f;

// Fallback feminine shaping applied to a freshly auto rigged female body.
static const float FEMALE_AUTORIG_SCULPT_SCALE = 1.15f;

// Case insensitive bone lookup on a raw bone list.
static CBoneObject * FindBoneOnListByName(CBoneList * bonesList, const CString & boneName)
{
	if(!bonesList)
		return NULL;

	for(POSITION posLoc = bonesList->GetHeadPosition(); posLoc != NULL;)
	{
		CBoneObject * bonePtr = (CBoneObject *)bonesList->GetNext(posLoc);
		if(bonePtr && bonePtr->m_boneName.CompareNoCase(boneName) == 0)
			return bonePtr;
	}
	return NULL;
}

// Index inside m_bonesList (== the index the vertex assignments use), -1 when
// the bone does not exist.
static int FindBoneListIndexByName(CBoneList * bonesList, const CString & boneName)
{
	if(!bonesList)
		return -1;

	int trace = 0;
	for(POSITION posLoc = bonesList->GetHeadPosition(); posLoc != NULL;)
	{
		CBoneObject * bonePtr = (CBoneObject *)bonesList->GetNext(posLoc);
		if(bonePtr && bonePtr->m_boneName.CompareNoCase(boneName) == 0)
			return trace;
		trace++;
	}
	return -1;
}

// Tries the preferred bone name first and then every fallback, so rigs that
// name their chest / pelvis bones differently still get anatomy bones.
static CBoneObject * ResolveBoneByCandidateNames(CBoneList * bonesList,
												 const CString & preferred,
												 const char * const * fallbacks,
												 int fallbackCount)
{
	CBoneObject * found = FindBoneOnListByName(bonesList, preferred);
	if(found)
		return found;

	for(int loop = 0; loop < fallbackCount; loop++)
	{
		found = FindBoneOnListByName(bonesList, CString(fallbacks[loop]));
		if(found)
			return found;
	}
	return NULL;
}

// TRUE for bones that actually deform skin: the auto rigger must not weight
// vertices to root helpers / footsteps / dummy bones.
static bool IsSkinningCandidateBone(const CString & rawBoneName)
{
	CString name = rawBoneName;
	name.MakeUpper();

	if(name.Find("DUMMY") != -1)
		return false;
	if(name.Find("STEPS") != -1)
		return false;
	if(name.Find("NUB") != -1)
		return false;

	return true;
}

static void GetBoneMatrixTranslation(const D3DMATRIX & matrix, D3DVECTOR & outVector)
{
	outVector.x = matrix._41;
	outVector.y = matrix._42;
	outVector.z = matrix._43;
}

// Model space point -> bone rest space.  This is exactly what the engine's
// skinning multiplies by the animated bone matrix again, so at the bind pose
// every bone reconstructs the rest position and nothing moves.
static void MakeBoneLocalOffset(const D3DMATRIX & boneRestMatrix,
								float positionX, float positionY, float positionZ,
								float normalX, float normalY, float normalZ,
								ABVERTEX0L & outOffset)
{
	MatrixARB math;
	D3DMATRIX restCopy = boneRestMatrix;
	D3DMATRIX invRest;
	math.D3DMath_MatrixInvert(invRest, restCopy);

	outOffset.x = invRest.m[0][0] * positionX + invRest.m[1][0] * positionY + invRest.m[2][0] * positionZ + invRest.m[3][0];
	outOffset.y = invRest.m[0][1] * positionX + invRest.m[1][1] * positionY + invRest.m[2][1] * positionZ + invRest.m[3][1];
	outOffset.z = invRest.m[0][2] * positionX + invRest.m[1][2] * positionY + invRest.m[2][2] * positionZ + invRest.m[3][2];

	outOffset.nx = invRest.m[0][0] * normalX + invRest.m[1][0] * normalY + invRest.m[2][0] * normalZ;
	outOffset.ny = invRest.m[0][1] * normalX + invRest.m[1][1] * normalY + invRest.m[2][1] * normalZ;
	outOffset.nz = invRest.m[0][2] * normalX + invRest.m[1][2] * normalY + invRest.m[2][2] * normalZ;
}

// Links a frame into its parent's child list exactly once.  BuildHiarchy() is
// never called a second time from here: re-running it would push another copy
// of every bone into every parent's child list.
static void AddChildFrameOnce(CFrameObj * parentFrame, CFrameObj * childFrame)
{
	if(!parentFrame || !childFrame || parentFrame == childFrame)
		return;

	// Detach from a previous parent first (never twice in the same list).
	if(childFrame->List_parentFrame && childFrame->List_parentFrame != parentFrame)
	{
		CFrameObj * oldParent = childFrame->List_parentFrame;
		if(oldParent->List_childFrameList)
		{
			POSITION posLast = NULL;
			for(POSITION posLoc = oldParent->List_childFrameList->GetHeadPosition(); (posLast = posLoc) != NULL;)
			{
				CFrameObj * existing = (CFrameObj *)oldParent->List_childFrameList->GetNext(posLoc);
				if(existing == childFrame)
				{
					oldParent->List_childFrameList->RemoveAt(posLast);
					break;
				}
			}
		}
		childFrame->List_parentFrame = NULL;
	}

	if(!parentFrame->List_childFrameList)
		parentFrame->List_childFrameList = new CFrameObjList();

	for(POSITION posLoc = parentFrame->List_childFrameList->GetHeadPosition(); posLoc != NULL;)
	{
		CFrameObj * existing = (CFrameObj *)parentFrame->List_childFrameList->GetNext(posLoc);
		if(existing == childFrame)
		{
			childFrame->List_parentFrame = parentFrame;
			return;                       // already a child of this frame
		}
	}

	childFrame->List_parentFrame = parentFrame;
	parentFrame->List_childFrameList->AddTail(childFrame);
}

// Creates one anchored helper bone: same orientation as its parent, positioned
// at the given model space point.  The bone therefore only differs from its
// parent by a translation, so its deformations follow the parent exactly.
static CBoneObject * CreateAnchoredHelperBone(CBoneList * bonesList,
											  const CString & boneName,
											  CBoneObject * parentBone,
											  const D3DVECTOR & position)
{
	if(!bonesList || !parentBone)
		return NULL;

	CBoneObject * newBone = new CBoneObject();
	newBone->m_boneFrame = new CFrameObj(NULL);      // linked once, further down
	newBone->m_boneName = boneName;
	newBone->m_parentName = parentBone->m_boneName;
	newBone->m_secondaryOverridable = FALSE;         // pure helper, never overridden
	newBone->m_animationList = NULL;                 // 0 bytes of animation RAM

	D3DMATRIX restMatrix = parentBone->m_startPosition;
	restMatrix._41 = position.x;
	restMatrix._42 = position.y;
	restMatrix._43 = position.z;

	newBone->m_boneFrame->AddTransformReplace(restMatrix);
	newBone->m_boneFrame->GetTransform(NULL, &newBone->m_startPosition);

	AddChildFrameOnce(parentBone->m_boneFrame, newBone->m_boneFrame);
	bonesList->AddTail(newBone);                     // appended: existing vertex assignment indices stay valid

	return newBone;
}

// Bounding box of the skeleton's bind pose, used when a character has no
// deformable mesh to measure yet (fresh skeleton before its first import).
static BOOL ComputeBoneRestBounds(CBoneList * bonesList,
								  float & outMinX, float & outMaxX,
								  float & outMinY, float & outMaxY,
								  float & outMinZ, float & outMaxZ)
{
	if(!bonesList || bonesList->GetCount() == 0)
		return FALSE;

	BOOL any = FALSE;
	for(POSITION posLoc = bonesList->GetHeadPosition(); posLoc != NULL;)
	{
		CBoneObject * bonePtr = (CBoneObject *)bonesList->GetNext(posLoc);
		if(!bonePtr || !IsSkinningCandidateBone(bonePtr->m_boneName))
			continue;

		D3DVECTOR position;
		GetBoneMatrixTranslation(bonePtr->m_startPosition, position);

		if(!any)
		{
			outMinX = outMaxX = position.x;
			outMinY = outMaxY = position.y;
			outMinZ = outMaxZ = position.z;
			any = TRUE;
			continue;
		}

		if(position.x < outMinX) outMinX = position.x;
		if(position.x > outMaxX) outMaxX = position.x;
		if(position.y < outMinY) outMinY = position.y;
		if(position.y > outMaxY) outMaxY = position.y;
		if(position.z < outMinZ) outMinZ = position.z;
		if(position.z > outMaxZ) outMaxZ = position.z;
	}
	return any;
}

// Squared distance from a point to a bone segment (a bone spans from its
// parent's rest position to its own rest position).
static float PointToSegmentDistanceSquared(const D3DVECTOR & point,
										   const D3DVECTOR & segmentStart,
										   const D3DVECTOR & segmentEnd)
{
	float abX = segmentEnd.x - segmentStart.x;
	float abY = segmentEnd.y - segmentStart.y;
	float abZ = segmentEnd.z - segmentStart.z;
	float apX = point.x - segmentStart.x;
	float apY = point.y - segmentStart.y;
	float apZ = point.z - segmentStart.z;

	float abLengthSq = abX * abX + abY * abY + abZ * abZ;
	float t = 0.0f;
	if(abLengthSq > 1e-8f)
		t = (apX * abX + apY * abY + apZ * abZ) / abLengthSq;
	if(t < 0.0f)
		t = 0.0f;
	if(t > 1.0f)
		t = 1.0f;

	float dx = apX - abX * t;
	float dy = apY - abY * t;
	float dz = apZ - abZ * t;

	return dx * dx + dy * dy + dz * dz;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
CSkeletonObject::CSkeletonObject()
{
m_hasPerPolyCollisionsTemp = false;
m_minSaveVerTimeStamp = GetTickCount();
m_minSameVersionAnimaPlay = 10000;
m_currentAnimVersion = -1;//unset
m_currentAnimType = -1;
m_animPrefDefault = 0;
m_curLod = 0;
m_bonesList = NULL;
m_lastUpdateTime=0;
m_SkeletalName = "BASE";
m_frameSpeed = 0;
m_ticksPerFrame = 0;
m_animationInUse = -1;
m_lastAnimationInUse = -1;

m_secondaryAnimationInUse  = -1;
m_secondaryTransitionStamp = 0;
m_inSecondaryTransition    = FALSE;

m_deformableMeshArrayCount = 0;
m_shadowInterface = NULL;
m_shadowOn = FALSE;
m_box.maxX = 0.0f;
m_box.maxY = 0.0f;
m_box.maxZ = 0.0f;
m_box.minX = 0.0f;
m_box.minY = 0.0f;
m_box.minZ = 0.0f;
m_requireUpdate = FALSE;
m_freeLookBone = -1;
m_freeLookPitch = 0;
m_freeLookYaw = 0;
m_inTransition = FALSE;
m_transitionTime = 0;
m_transitionStartTime = 0;
m_transitionBaseFrame = 0;

m_lastDurationUpdateTime = 0;
m_durationCurrentLength = 0;

m_animPreference = 0;

m_flagForDeactivateAdjust = FALSE;


m_sphereLocationalDamageList = NULL;
m_calulationSphere           = NULL;
m_sphereBodyCollisionSystem  = NULL;
m_damageSphereList           = NULL;
m_particleEmiterList         = NULL;
m_deformableMeshArrayV2      = NULL;
m_alternateConfigurations    = NULL;
m_spawnCfg                   = NULL;
m_fireQueList                = NULL;
m_visibilityTrackList        = NULL;
m_lodRanges					 = NULL;


m_waistBone     = -1;
m_waistTurn     = 0.0f;
m_waistTurnMax  = 50.0f;
m_waistTurnRate = 5.0f;
m_currentLOD    = 0;
m_secondaryCOMadjust.x        = 0.0f;
m_secondaryCOMadjust.y        = 0.0f;
m_secondaryCOMadjust.z        = 0.0f;
m_secondaryCOMadjustApply     = FALSE;
m_soundRef                    = NULL;
m_sndVelocityBased			  = FALSE;
m_sndID					      = -1;
m_clipRadius				  = 400.0f;
m_soundFactor				  = 0.0f;
m_startFrequency			  = 1.0f;
m_sndVelocityMaxDist          = 5.0f;


m_secondaryTransitionDuration = 200;

m_secondaryCOMInheritBone = -1;

m_transitionOut = FALSE;
m_lastScndDurationUpdateTime  = GetTickCount();
m_scndDurationCurrentLength   = 0;

m_primaryAnimLastUpdatedTime  = m_lastScndDurationUpdateTime;
m_secondaryAnimLastUpdatedTime = m_lastScndDurationUpdateTime;
m_inReverse = FALSE;
m_currentTimePassedPrimary = 0;
m_currentTimePassedSecondary = 0;
m_currentBaseFrame = 0;
m_secondaryBaseFrame = 0;
m_currentBaseSoundFrame = -1;
m_secondaryBaseSoundFrame = -1;
m_scndEndCap = 0;//reset end cap

m_inSecondaryTransition       = FALSE;
m_inSecondaryTransitionTime   = m_lastScndDurationUpdateTime;

m_enableJigglePhysics  = FALSE;
m_bjCount              = -1;
m_bjHaveWorld          = FALSE;
m_bjHadWorld           = FALSE;
m_bjWorldY = m_bjLastY = m_bjLastVy = 0.0f;
m_bjTick               = 0;
m_jiggleBoundsValid    = FALSE;
m_jiggleMinX           = -5.0f;
m_jiggleMaxX           = 5.0f;
m_jiggleMinY           = 0.0f;
m_jiggleMaxY           = 20.0f;
m_jiggleMinZ           = -5.0f;
m_jiggleMaxZ           = 5.0f;
m_jiggleLastTick       = 0;
m_jiggleLastAnimType   = -1;
m_jiggleLastBoneX      = 0.0f;
m_jiggleLastBoneY      = 0.0f;
m_jiggleLastBoneZ      = 0.0f;
m_jiggleBustPosY       = 0.0f;
m_jiggleBustVelY       = 0.0f;
m_jiggleBustPosX       = 0.0f;
m_jiggleBustVelX       = 0.0f;
m_jiggleBustPosZ       = 0.0f;
m_jiggleBustVelZ       = 0.0f;
m_jiggleButtPosY       = 0.0f;
m_jiggleButtVelY       = 0.0f;
m_jiggleButtPosX       = 0.0f;
m_jiggleButtVelX       = 0.0f;
m_jigglePhase          = 0.0f;
}

BOOL CSkeletonObject::Visible()
{
      if(m_visibilityTrackList)
	  {
	   if(!m_visibilityTrackList->IsInsideARange(m_animationInUse,m_currentBaseFrame))
	   {
           if(m_secondaryAnimationInUse != -1)
		   {
	          if(!m_visibilityTrackList->IsInsideARange(m_secondaryAnimationInUse,m_secondaryBaseFrame))
	              return FALSE;

		   }
		   else
	          return FALSE;
	   }
	  }
	  return TRUE;
}
//------------------------------------------------------------------//
//clear lod
//------------------------------------------------------------------//
void CSkeletonObject::RemoveLodsByStruct(int strCount,LODOptCondition * lodProcArray)
{
	 int lodCount = 0;//will include primary
	 if(m_bonesList)
	 for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
     {
         CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
         if(bonePtr->m_hiarchyMesh)
		 {//valid mesh
	         lodCount = (int)bonePtr->m_hiarchyMesh->m_lodChain->GetCount();
			 for(int i=0;i<strCount;i++)
			 {//find lod count match or dont do anything
				 if((lodProcArray[i].lodMatch+1) == lodCount)//found candidate
				   for(int j=0;j<lodProcArray[i].removalCount;j++)
				   {//
					   bonePtr->m_hiarchyMesh->m_lodChain->DeleteByIndex(lodProcArray[i].removalArray[j]);
				   }//
			 }//end find lod count match or dont do anything
		 }//end valid mesh
     }

	 if(m_deformableMeshArrayCount > 0)
	  {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  if(altPtr->m_configurations)
					  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
					  {//deformable unit same array ie head0 head1
						     CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
						     //dMesh->m_lodChain->DeleteByIndex(index);
						     lodCount = (int)dMesh->m_lodChain->GetCount();
							 for(int i=0;i<strCount;i++)
							 {//find lod count match or dont do anything
								 if((lodProcArray[i].lodMatch+1) == lodCount)//found candidate
								   for(int j=0;j<lodProcArray[i].removalCount;j++)
								   {//
									   dMesh->m_lodChain->DeleteByIndex(lodProcArray[i].removalArray[j]);
								   }//
							 }//end find lod count match or dont do anything
					  }
				}
			  }//alternate exists - this is based off of DB gen
		  }//end deformable mesh loop
	  }//end deformable meshs
}
//------------------------------------------------------------------//
//clear lod
//------------------------------------------------------------------//
void CSkeletonObject::RemoveLODFromChain(int index)
{
   for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
   {
      CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);

      if(bonePtr->m_hiarchyMesh)
	     bonePtr->m_hiarchyMesh->m_lodChain->DeleteByIndex(index);
   }

   //def mes
      if(m_deformableMeshArrayCount > 0)
	  {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  if(altPtr->m_configurations)
					  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
					  {//deformable unit same array ie head0 head1
						  CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
						  dMesh->m_lodChain->DeleteByIndex(index);
					  }
				}
			  }//alternate exists - this is based off of DB gen
		  }//end deformable mesh loop
	  }//end deformable meshs
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::ScaleSystem(float percentage,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
  D3DMATRIX scaleMatrix;
 
  GL_matrixFunctions.scale (percentage,percentage,percentage,scaleMatrix.m );



  for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
  {
      CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);

	  D3DVECTOR tempPos;
	  tempPos.x = bonePtr->m_startPosition._41;
	  tempPos.y = bonePtr->m_startPosition._42;
	  tempPos.z = bonePtr->m_startPosition._43;
	  //GL_matrixFunctions.fastMatrixMultiply(bonePtr->m_startPosition,bonePtr->m_startPosition,scaleMatrix);
	  tempPos = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,tempPos);
	  bonePtr->m_startPosition._41 = tempPos.x;
	  bonePtr->m_startPosition._42 = tempPos.y;
	  bonePtr->m_startPosition._43 = tempPos.z;

	  // anatomy / helper bones carry no animation list of their own
	  if(bonePtr->m_animationList)
	  for(POSITION posLocal = bonePtr->m_animationList->GetHeadPosition();posLocal != NULL;)
	  {
          CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bonePtr->m_animationList->GetNext(posLocal);
          for(int loop=0;loop<animPtr->m_totalFrames;loop++)
		  {
			  animPtr->m_animationData[loop].position = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,animPtr->m_animationData[loop].position);
		  }
	  }

      if(bonePtr->m_hiarchyMesh)
		  for(POSITION hPos = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
		  {
			  CHiarcleVisObj * hVisObj = (CHiarcleVisObj *)bonePtr->m_hiarchyMesh->m_lodChain->GetNext(hPos);
			  
			  hVisObj->m_mesh->ScaleIt(percentage);
		      hVisObj->m_mesh->InitBuffer(g_pd3dDevice,NULL);
		  }
  }



  //def mes
  if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
                          
						  ABVERTEX * vArray = NULL;
						  int count = 0;
						  dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&count);
						  
						  for(int vertexCompLoop = 0;vertexCompLoop < dVis->m_mesh->m_abVertexArray.m_vertexCount;vertexCompLoop++)
						  {//primary vertex loop
                             D3DVECTOR vertexTemp;
							 vertexTemp.x = vArray[vertexCompLoop].x;
							 vertexTemp.y = vArray[vertexCompLoop].y;
							 vertexTemp.z = vArray[vertexCompLoop].z;
                             vertexTemp = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,vertexTemp);
							 vArray[vertexCompLoop].x = vertexTemp.x;
							 vArray[vertexCompLoop].y = vertexTemp.y;
							 vArray[vertexCompLoop].z = vertexTemp.z;
							 
						  }//end primary vertex loop

						  dVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,count);
                          if(vArray)
						  delete [] vArray;
						  vArray = 0;

                          for(int x=0;x<dVis->m_mesh->m_abVertexArray.m_vertexCount;x++){
							  D3DVECTOR vertexTemp;
                              vertexTemp.x = dVis->m_originalArrayVector[x].x;
							  vertexTemp.y = dVis->m_originalArrayVector[x].y;
							  vertexTemp.z = dVis->m_originalArrayVector[x].z;
                              vertexTemp = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,vertexTemp);
							  dVis->m_originalArrayVector[x].x = vertexTemp.x;
							  dVis->m_originalArrayVector[x].y = vertexTemp.y;
							  dVis->m_originalArrayVector[x].z = vertexTemp.z;

						  }


						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
					  }
				  }//end deformable unit same array
	
				}//end valid configuration
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
	 }//end deformable meshes


    if(m_sphereLocationalDamageList)
       for(POSITION posLoc = m_sphereLocationalDamageList->GetHeadPosition();posLoc !=NULL;)
	   {//damage point loop
           CSphereCollisionObj * sphereObj = (CSphereCollisionObj *)m_sphereLocationalDamageList->GetNext(posLoc);
		   sphereObj->m_sphereRadius = sphereObj->m_sphereRadius * percentage;
	   }//end damage point loop

	if(m_calulationSphere)
       m_calulationSphere->m_sphereRadius = m_calulationSphere->m_sphereRadius * percentage;


	if(m_sphereBodyCollisionSystem)
       for(POSITION csysPos = m_sphereBodyCollisionSystem->GetHeadPosition();csysPos != NULL;)
	   {//g system loop
		  CSphereCollisionObj * mainBdSphPtr = (CSphereCollisionObj *)m_sphereBodyCollisionSystem->GetNext(csysPos);
		  mainBdSphPtr->m_sphereRadius = mainBdSphPtr->m_sphereRadius * percentage;

	   }

	   if(m_damageSphereList)
       for(POSITION csysPos = m_damageSphereList->GetHeadPosition();csysPos != NULL;)
	   {//g system loop
		  CDamagePathObj* damagePathObj = (CDamagePathObj*)m_damageSphereList->GetNext(csysPos);
		  damagePathObj->m_radius = damagePathObj->m_radius * percentage;

	   }

	   if(m_fireQueList)
		for(POSITION firePos = m_fireQueList->GetHeadPosition();firePos != NULL;)
		{
            CFireRangeObject * fireRange = (CFireRangeObject *)m_fireQueList->GetNext(firePos);
            fireRange->m_hardPointAdjust.x = fireRange->m_hardPointAdjust.x * percentage;
			fireRange->m_hardPointAdjust.y = fireRange->m_hardPointAdjust.y * percentage;
			fireRange->m_hardPointAdjust.z = fireRange->m_hardPointAdjust.z * percentage;

		}

  return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::TransformSystem(D3DMATRIX scaleMatrix,LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL rotationOnly)
{
  
 
  //GL_matrixFunctions.scale (percentage,percentage,percentage,scaleMatrix.m );
   //this->

  


  for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
  {
      CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
	  //tempPos.x = bonePtr->m_startPosition._41;
	  //tempPos.y = bonePtr->m_startPosition._42;
	  //tempPos.z = bonePtr->m_startPosition._43;
	  GL_matrixFunctions.fastMatrixMultiply(bonePtr->m_startPosition,bonePtr->m_startPosition,scaleMatrix);
	  //tempPos = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,tempPos);
	  //bonePtr->m_startPosition._41 = tempPos.x;
	  //bonePtr->m_startPosition._42 = tempPos.y;
	  //bonePtr->m_startPosition._43 = tempPos.z;

	  //bonePtr->m_targetPosition.x = bonePtr->m_startPosition._41;
	  //bonePtr->m_targetPosition.y = bonePtr->m_startPosition._42;
	  //bonePtr->m_targetPosition.z = bonePtr->m_startPosition._43;

	  //GL_matrixFunctions.fastMatrixMultiply(bonePtr->m_boneFrame->List_frameMatrix,scaleMatrix,bonePtr->m_boneFrame->List_frameMatrix);
	 
	  /*tempPos.x = bonePtr->m_boneFrame->List_frameMatrix._41;
	  tempPos.y = bonePtr->m_boneFrame->List_frameMatrix._42;
	  tempPos.z = bonePtr->m_boneFrame->List_frameMatrix._43;
	  tempPos = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,tempPos);
	  bonePtr->m_boneFrame->List_frameMatrix._41 = tempPos.x;
	  bonePtr->m_boneFrame->List_frameMatrix._42 = tempPos.y;
	  bonePtr->m_boneFrame->List_frameMatrix._43 = tempPos.z;*/
      //D3DVECTOR upVect,dirVect,positionTemp;
	  //upVect = bonePtr->m_targetUpVector;
	  //dirVect = bonePtr->m_targetDirVector;
	  //positionTemp = bonePtr->m_targetPosition
      //bonePtr->m_startPosition.



			  //D3DVECTOR dirVect = animPtr->m_animationData[loop].dirVect;
	  /*dirVect = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(scaleMatrix.m,dirVect);
	  upVect = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(scaleMatrix.m,upVect);

	  dirVect = GL_matrixFunctions.Normalize(dirVect);
	  upVect = GL_matrixFunctions.Normalize(upVect);

      animPtr->m_animationData[loop].upVect = upVect;
	  animPtr->m_animationData[loop].dirVect = dirVect;*/



	  // anatomy / helper bones carry no animation list of their own
	  if(bonePtr->m_animationList)
	  for(POSITION posLocal = bonePtr->m_animationList->GetHeadPosition();posLocal != NULL;)
	  {
          CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bonePtr->m_animationList->GetNext(posLocal);
          for(int loop=0;loop<animPtr->m_totalFrames;loop++)
		  {
			  animPtr->m_animationData[loop].position = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,animPtr->m_animationData[loop].position);
			  D3DVECTOR upVect,dirVect;
			  upVect = animPtr->m_animationData[loop].upVect.GetD3DVECTOR();
			  dirVect = animPtr->m_animationData[loop].dirVect.GetD3DVECTOR();;
			  //D3DVECTOR dirVect = animPtr->m_animationData[loop].dirVect;
			  
			  dirVect = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(scaleMatrix.m,dirVect);
			  upVect = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(scaleMatrix.m,upVect);

			  dirVect = GL_matrixFunctions.Normalize(dirVect);
			  upVect = GL_matrixFunctions.Normalize(upVect);

			  animPtr->m_animationData[loop].upVect = upVect;
			  animPtr->m_animationData[loop].dirVect = dirVect;
		  } 
	  }

	  if(!rotationOnly)
	  if(bonePtr->m_hiarchyMesh)
		  for(POSITION hPos = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
		  {
			  CHiarcleVisObj * hVisObj = (CHiarcleVisObj *)bonePtr->m_hiarchyMesh->m_lodChain->GetNext(hPos);
			  
			  hVisObj->m_mesh->ScaleIt(scaleMatrix._33);
			  //hVisObj->m_mesh->Transform(scaleMatrix.m);
		      hVisObj->m_mesh->InitBuffer(g_pd3dDevice,NULL);
		  }
  }



  //def mes
  if(!rotationOnly)
  if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
                          
						  ABVERTEX * vArray = NULL;
						  int count = 0;
						  //dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&count);
						  dVis->m_mesh->ScaleIt(scaleMatrix._33);
						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
						  
						  /*for(int vertexCompLoop = 0;vertexCompLoop < dVis->m_mesh->m_abVertexArray.m_vertexCount;vertexCompLoop++)
						  {//primary vertex loop
                             D3DVECTOR vertexTemp;
							 vertexTemp.x = vArray[vertexCompLoop].x;
							 vertexTemp.y = vArray[vertexCompLoop].y;
							 vertexTemp.z = vArray[vertexCompLoop].z;
                             vertexTemp = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,vertexTemp);
							 vArray[vertexCompLoop].x = vertexTemp.x;
							 vArray[vertexCompLoop].y = vertexTemp.y;
							 vArray[vertexCompLoop].z = vertexTemp.z;
							 
						  }//end primary vertex loop/

						  dVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,count);
                          if(vArray)
						  delete [] vArray;
						  vArray = 0;*/

						  
					  }
				  }//end deformable unit same array
	
				}//end valid configuration
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
	 }//end deformable meshes

    if(rotationOnly)
	   return TRUE;

    if(m_sphereLocationalDamageList)
       for(POSITION posLoc = m_sphereLocationalDamageList->GetHeadPosition();posLoc !=NULL;)
	   {//damage point loop
           CSphereCollisionObj * sphereObj = (CSphereCollisionObj *)m_sphereLocationalDamageList->GetNext(posLoc);
		   sphereObj->m_sphereRadius = sphereObj->m_sphereRadius * scaleMatrix._33;
	   }//end damage point loop

	if(m_calulationSphere)
       m_calulationSphere->m_sphereRadius = m_calulationSphere->m_sphereRadius * scaleMatrix._33;


	if(m_sphereBodyCollisionSystem)
       for(POSITION csysPos = m_sphereBodyCollisionSystem->GetHeadPosition();csysPos != NULL;)
	   {//g system loop
		  CSphereCollisionObj * mainBdSphPtr = (CSphereCollisionObj *)m_sphereBodyCollisionSystem->GetNext(csysPos);
		  mainBdSphPtr->m_sphereRadius = mainBdSphPtr->m_sphereRadius * scaleMatrix._33;

	   }

	   if(m_damageSphereList)
       for(POSITION csysPos = m_damageSphereList->GetHeadPosition();csysPos != NULL;)
	   {//g system loop
		  CDamagePathObj* damagePathObj = (CDamagePathObj*)m_damageSphereList->GetNext(csysPos);
		  damagePathObj->m_radius = damagePathObj->m_radius * scaleMatrix._33;

	   }

	   if(m_fireQueList)
		for(POSITION firePos = m_fireQueList->GetHeadPosition();firePos != NULL;)
		{
            CFireRangeObject * fireRange = (CFireRangeObject *)m_fireQueList->GetNext(firePos);
            fireRange->m_hardPointAdjust.x = fireRange->m_hardPointAdjust.x * scaleMatrix._33;
			fireRange->m_hardPointAdjust.y = fireRange->m_hardPointAdjust.y * scaleMatrix._33;
			fireRange->m_hardPointAdjust.z = fireRange->m_hardPointAdjust.z * scaleMatrix._33;

		}

  return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
D3DMATRIX CSkeletonObject::GetBoneMatrixByIndex(int index)
{
D3DMATRIX boneMatrix;
POSITION posLoc = m_bonesList->FindIndex(index);
if(!posLoc){
	GL_matrixFunctions.InitMat(boneMatrix.m);
    return boneMatrix;
}
    CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
    bonePtr->m_boneFrame->GetTransform(NULL,&boneMatrix);
    return boneMatrix;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
CFrameObj * CSkeletonObject::GetBoneFrameByIndex(int index)
{
if(index < 0)
   return NULL;

POSITION posLoc = m_bonesList->FindIndex(index);
  if(!posLoc)
      return NULL;
      CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
      return bonePtr->m_boneFrame;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
D3DVECTOR CSkeletonObject::GetBonePositionByIndex(int index)
{
D3DVECTOR bonePosition;
bonePosition.x = 0.0f;
bonePosition.y = 0.0f;
bonePosition.z = 0.0f;
POSITION posLoc = m_bonesList->FindIndex(index);
if(!posLoc)
    return bonePosition;
    CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
    bonePtr->m_boneFrame->GetPosition(NULL,&bonePosition);
    return bonePosition;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::UnifyAllNormalsDeformable(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     

     
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  //ResetSkeletalNormalsDeformable(g_pd3dDevice);//reset
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(int secondaryLoop=0;secondaryLoop<m_deformableMeshArrayCount;secondaryLoop++)
					  {//secondary loop  
						 if(secondaryLoop == loop)
							continue;
                            POSITION secondaryAltPos = m_alternateConfigurations->FindIndex(secondaryLoop);
			                if(alPos){//valid secondary configuration
			                   CAlterUnitDBObject * secondaryAltPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(secondaryAltPos);
                               for(POSITION scndCurCfgPos = secondaryAltPtr->m_configurations->GetHeadPosition();scndCurCfgPos != NULL;)
							   {//secondary deformable unit same array ie head0 head1
			                      CDeformableMesh * secondarydMesh = (CDeformableMesh *)secondaryAltPtr->m_configurations->GetNext(scndCurCfgPos);
                                  
								  BOOL done = FALSE;
								  int loop = 0;
								  while(!done)
								  {//lod loop

                                    POSITION scndLodPos = secondarydMesh->m_lodChain->FindIndex(loop);
								    POSITION lodPos = dMesh->m_lodChain->FindIndex(loop);

								    if(scndLodPos == NULL || lodPos == NULL)
								       break;
                                     CDeformableVisObj * scndDVis = (CDeformableVisObj *)secondarydMesh->m_lodChain->GetAt(scndLodPos);
                                     CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetAt(lodPos);
									 
									 if(dVis != NULL && scndDVis != NULL)
									 {//unify
                                         ABVERTEX * vArray = NULL;
										 int vCount = 0;
										 dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);


                                         for(int vertexCompLoop = 0;vertexCompLoop < dVis->m_mesh->m_abVertexArray.m_vertexCount;vertexCompLoop++)
										 {//primary vertex loop
										   
										   ABVERTEX * vArrayScndary = NULL;
										   int vCountScndry = 0;
										   scndDVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArrayScndary,&vCountScndry);
											 
										   for(int scndVertexCompLoop = 0;scndVertexCompLoop < scndDVis->m_mesh->m_abVertexArray.m_vertexCount;scndVertexCompLoop++)
										   {//secondary vertex loop
                                            float xDiff = fabs(vArray[vertexCompLoop].x - vArrayScndary[scndVertexCompLoop].x);
                                            float yDiff = fabs(vArray[vertexCompLoop].y - vArrayScndary[scndVertexCompLoop].y);
											float zDiff = fabs(vArray[vertexCompLoop].z - vArrayScndary[scndVertexCompLoop].z);

										    if(xDiff < .01f &&
                                               yDiff < .01f &&
											   zDiff < .01f)
											{//same position unify the normals
											   //if(secondaryLoop < loop)
											   //{//copy normals
                                                 vArrayScndary[scndVertexCompLoop].nx = vArray[vertexCompLoop].nx;
											     vArrayScndary[scndVertexCompLoop].ny = vArray[vertexCompLoop].ny;
											     vArrayScndary[scndVertexCompLoop].nz = vArray[vertexCompLoop].nz;
											   /*}//end copy normals
											   else
											   {//blend vert normals
                                                 D3DVECTOR normal;
												 normal.x = vArrayScndary[scndVertexCompLoop].nx + vArray[vertexCompLoop].nx;
											     normal.y = vArrayScndary[scndVertexCompLoop].ny + vArray[vertexCompLoop].ny;
											     normal.z = vArrayScndary[scndVertexCompLoop].nz + vArray[vertexCompLoop].nz;
											     //normal = GL_matrixFunctions.Normalize(normal);
											     vArrayScndary[scndVertexCompLoop].nx = normal.x;
												 vArrayScndary[scndVertexCompLoop].ny = normal.y;
												 vArrayScndary[scndVertexCompLoop].nz = normal.z;
												 vArray[vertexCompLoop].nx         = normal.x;
												 vArray[vertexCompLoop].ny         = normal.y;
												 vArray[vertexCompLoop].nz         = normal.z;

											   }//end blend vert normals*/
											}//end same position unify the normals
										   }//end secondary vertex loop
                                           
										   if(vArrayScndary)
										   {
											   scndDVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArrayScndary,vCountScndry);
											   delete [] vArrayScndary;
											   vArrayScndary = 0;
										   }
										   
										 }//end primary vertex loop

										 if(vArray)
										 {
											   dVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,vCount);
											   delete [] vArray;
											   vArray = 0;
										 }
										 //scndDVis->m_mesh->m_abVertexArray.InitBuffer();
										 //dVis->m_mesh->m_abVertexArray.InitBuffer();
									 }//end unify
									 loop++;
								  }//end lod loop
								  
							   }//secondary deformable unit same array ie head0 head1
							}//end valid secondary configuration
					  }//end secondary loop  
				  }//end deformable unit same array
	
				}//end valid configuration
				
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
		  
	 }//end deformable meshes
     //NormalizeAllNormals(g_pd3dDevice);

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::ResetSkeletalNormalsDeformable(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
                          dVis->m_mesh->GenerateNormals(0);
						  dVis->m_mesh->GenerateSmoothNormals();
						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
					  }
				  }//end deformable unit same array
	
				}//end valid configuration
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
	 }//end deformable meshes

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::Reinit(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
					  }
				  }//end deformable unit same array
	
				}//end valid configuration
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
	 }//end deformable meshes
	 if(m_bonesList){//bones
        for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
		{
            CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
			if(bonePtr->m_hiarchyMesh)
			if(bonePtr->m_hiarchyMesh->m_lodChain)
			{
			   for(POSITION posLod = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();posLod != NULL;)
			   {
				   CHiarcleVisObj * hPtr = (CHiarcleVisObj *)bonePtr->m_hiarchyMesh->m_lodChain->GetNext(posLod);
                   hPtr->m_mesh->InitBuffer(g_pd3dDevice,NULL);
			   }
			}
		}
     }//end bones

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::NormalizeAllNormals(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
                          
						  ABVERTEX * vArray = NULL;
						  int vCount = 0;
						  dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);
						  
						  for(int vertexCompLoop = 0;vertexCompLoop < dVis->m_mesh->m_abVertexArray.m_vertexCount;vertexCompLoop++)
						  {//primary vertex loop
                             D3DVECTOR normalMod;
							 normalMod.x = vArray[vertexCompLoop].nx;
							 normalMod.y = vArray[vertexCompLoop].ny;
							 normalMod.z = vArray[vertexCompLoop].nz;
                             normalMod = GL_matrixFunctions.Normalize(normalMod);
							 vArray[vertexCompLoop].nx = normalMod.x;
							 vArray[vertexCompLoop].ny = normalMod.y;
							 vArray[vertexCompLoop].nz = normalMod.z;
						  }//end primary vertex loop

                          if(vArray)
						  {
                             dVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,vCount);
                             delete [] vArray;
							 vArray = 0;
						  }


						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
					  }
				  }//end deformable unit same array
	
				}//end valid configuration
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
	 }//end deformable meshes

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::Clone(CSkeletonObject ** clone,
							LPDIRECT3DDEVICE9 g_pd3dDevice,
							int * spawnCfgOptional)
{	
CSkeletonObject * cloneReturn = new CSkeletonObject();

cloneReturn->m_animPrefDefault			  = m_animPrefDefault;
cloneReturn->m_frameSpeed                 = m_frameSpeed;
cloneReturn->m_SkeletalName               = m_SkeletalName;
cloneReturn->m_ticksPerFrame              = m_ticksPerFrame;
cloneReturn->m_deformableMeshArrayCount   = m_deformableMeshArrayCount;
cloneReturn->m_shadowOn                   = m_shadowOn;
cloneReturn->m_box.maxX                   = m_box.maxX;
cloneReturn->m_box.maxY                   = m_box.maxY;
cloneReturn->m_box.maxZ                   = m_box.maxZ;
cloneReturn->m_box.minX                   = m_box.minX;
cloneReturn->m_box.minY                   = m_box.minY;
cloneReturn->m_box.minZ                   = m_box.minZ;
cloneReturn->m_freeLookBone               = m_freeLookBone;
cloneReturn->m_freeLookPitch              = m_freeLookPitch;
cloneReturn->m_freeLookYaw                = m_freeLookYaw;
cloneReturn->m_waistBone                  = m_waistBone;
cloneReturn->m_waistTurnMax               = m_waistTurnMax;
cloneReturn->m_waistTurnRate              = m_waistTurnRate;
cloneReturn->m_secondaryCOMInheritBone    = m_secondaryCOMInheritBone;
cloneReturn->m_freeLookBone2              = m_freeLookBone2;
cloneReturn->m_sndVelocityBased			  = m_sndVelocityBased;
cloneReturn->m_sndID					  = m_sndID;
cloneReturn->m_clipRadius				  = m_clipRadius;
cloneReturn->m_soundFactor				  = m_soundFactor;
cloneReturn->m_startFrequency			  = m_startFrequency;
cloneReturn->m_sndVelocityMaxDist         = m_sndVelocityMaxDist;


cloneReturn->m_animPreference             = 0;
cloneReturn->m_lastDurationUpdateTime     = 0;
cloneReturn->m_durationCurrentLength      = 0;
cloneReturn->m_enableJigglePhysics        = m_enableJigglePhysics;
cloneReturn->m_jiggleBoundsValid          = m_jiggleBoundsValid;
cloneReturn->m_jiggleMinX                 = m_jiggleMinX;
cloneReturn->m_jiggleMaxX                 = m_jiggleMaxX;
cloneReturn->m_jiggleMinY                 = m_jiggleMinY;
cloneReturn->m_jiggleMaxY                 = m_jiggleMaxY;
cloneReturn->m_jiggleMinZ                 = m_jiggleMinZ;
cloneReturn->m_jiggleMaxZ                 = m_jiggleMaxZ;

   if(m_lodRanges)
      m_lodRanges->Clone(&cloneReturn->m_lodRanges);

   if(m_shadowOn==TRUE)//shadows
      m_shadowInterface->Clone(&cloneReturn->m_shadowInterface);

   if(m_visibilityTrackList)
      m_visibilityTrackList->Clone(&cloneReturn->m_visibilityTrackList);

   cloneReturn->m_particleEmiterList = NULL;
   if(m_particleEmiterList)
      m_particleEmiterList->Clone(&cloneReturn->m_particleEmiterList);

   cloneReturn->m_calulationSphere = NULL;
   if(m_calulationSphere)
   m_calulationSphere->Clone(&cloneReturn->m_calulationSphere);

   cloneReturn->m_damageSphereList = NULL;
   if(m_damageSphereList)
	  m_damageSphereList->Clone(&cloneReturn->m_damageSphereList);

   cloneReturn->m_sphereBodyCollisionSystem = NULL;
   if(m_sphereBodyCollisionSystem)
      m_sphereBodyCollisionSystem->Clone(&cloneReturn->m_sphereBodyCollisionSystem);

   cloneReturn->m_sphereLocationalDamageList = NULL;
   if(m_sphereLocationalDamageList)
      m_sphereLocationalDamageList->Clone(&cloneReturn->m_sphereLocationalDamageList);

   if(m_bonesList){//bones
      m_bonesList->Clone(&cloneReturn->m_bonesList,g_pd3dDevice,cloneReturn->m_hasPerPolyCollisionsTemp);
	  cloneReturn->BuildHiarchy();
   }//end bones

   if(m_fireQueList)
      m_fireQueList->Clone(&cloneReturn->m_fireQueList);


   //cloneReturn->m_deformableMeshArrayCount = 0;//REMOVE
   if(cloneReturn->m_deformableMeshArrayCount > 0){//deformable meshs
          cloneReturn->m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
		  
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen

                cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit = NULL;

			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
			      
				  POSITION curCfgPos = NULL;
				  if(!spawnCfgOptional)
				  {
				      curCfgPos = altPtr->m_configurations->FindIndex(abs(m_spawnCfg[loop]));
				  }
				  else
				  {
				      curCfgPos = altPtr->m_configurations->FindIndex(abs(spawnCfgOptional[loop]));
				  }
			  
			       if(curCfgPos){
			         CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
					 if(dMesh->m_collisionFiltered == false)
			            cloneReturn->m_hasPerPolyCollisionsTemp = true;
					 dMesh->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit,g_pd3dDevice);
				   }
				   else
				   cloneReturn->m_deformableMeshArrayCount--;
				}
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit,g_pd3dDevice);
			  }//end based on runtime gen
		  }

		  if(m_deformableMeshArrayCount > 0)
		  {
		    cloneReturn->m_spawnCfg = new int[m_deformableMeshArrayCount];
            //CopyMemory(cloneReturn->m_spawnCfg,m_spawnCfg,(sizeof(int)*m_deformableMeshArrayCount));

			if(!spawnCfgOptional)
				CopyMemory(cloneReturn->m_spawnCfg,m_spawnCfg,(sizeof(int)*m_deformableMeshArrayCount));
		    else
				CopyMemory(cloneReturn->m_spawnCfg,spawnCfgOptional,(sizeof(int)*m_deformableMeshArrayCount));
		  }
   }//end deformable meshs
            
   *clone = cloneReturn;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::Clone(CSkeletonObject ** clone,
							LPDIRECT3DDEVICE9 g_pd3dDevice,
							int * spawnCfgOptional,
							int copyAnimations)
{	
CSkeletonObject * cloneReturn = new CSkeletonObject();

cloneReturn->m_animPrefDefault			  = m_animPrefDefault;
cloneReturn->m_frameSpeed                 = m_frameSpeed;
cloneReturn->m_SkeletalName               = m_SkeletalName;
cloneReturn->m_ticksPerFrame              = m_ticksPerFrame;
cloneReturn->m_deformableMeshArrayCount   = m_deformableMeshArrayCount;
cloneReturn->m_shadowOn                   = m_shadowOn;
cloneReturn->m_box.maxX                   = m_box.maxX;
cloneReturn->m_box.maxY                   = m_box.maxY;
cloneReturn->m_box.maxZ                   = m_box.maxZ;
cloneReturn->m_box.minX                   = m_box.minX;
cloneReturn->m_box.minY                   = m_box.minY;
cloneReturn->m_box.minZ                   = m_box.minZ;
cloneReturn->m_freeLookBone               = m_freeLookBone;
cloneReturn->m_freeLookPitch              = m_freeLookPitch;
cloneReturn->m_freeLookYaw                = m_freeLookYaw;
cloneReturn->m_waistBone                  = m_waistBone;
cloneReturn->m_waistTurnMax               = m_waistTurnMax;
cloneReturn->m_waistTurnRate              = m_waistTurnRate;
cloneReturn->m_secondaryCOMInheritBone    = m_secondaryCOMInheritBone;
cloneReturn->m_freeLookBone2              = m_freeLookBone2;
cloneReturn->m_sndVelocityBased			  = m_sndVelocityBased;
cloneReturn->m_sndID					  = m_sndID;
cloneReturn->m_clipRadius				  = m_clipRadius;
cloneReturn->m_soundFactor				  = m_soundFactor;
cloneReturn->m_startFrequency			  = m_startFrequency;
cloneReturn->m_sndVelocityMaxDist         = m_sndVelocityMaxDist;


cloneReturn->m_animPreference             = 0;
cloneReturn->m_lastDurationUpdateTime     = 0;
cloneReturn->m_durationCurrentLength      = 0;
cloneReturn->m_enableJigglePhysics        = m_enableJigglePhysics;
cloneReturn->m_jiggleBoundsValid          = m_jiggleBoundsValid;
cloneReturn->m_jiggleMinX                 = m_jiggleMinX;
cloneReturn->m_jiggleMaxX                 = m_jiggleMaxX;
cloneReturn->m_jiggleMinY                 = m_jiggleMinY;
cloneReturn->m_jiggleMaxY                 = m_jiggleMaxY;
cloneReturn->m_jiggleMinZ                 = m_jiggleMinZ;
cloneReturn->m_jiggleMaxZ                 = m_jiggleMaxZ;

   if(m_lodRanges)
      m_lodRanges->Clone(&cloneReturn->m_lodRanges);

   if(m_shadowOn==TRUE)//shadows
      m_shadowInterface->Clone(&cloneReturn->m_shadowInterface);

   if(m_visibilityTrackList)
      m_visibilityTrackList->Clone(&cloneReturn->m_visibilityTrackList);

   cloneReturn->m_particleEmiterList = NULL;
   if(m_particleEmiterList)
      m_particleEmiterList->Clone(&cloneReturn->m_particleEmiterList);

   cloneReturn->m_calulationSphere = NULL;
   if(m_calulationSphere)
   m_calulationSphere->Clone(&cloneReturn->m_calulationSphere);

   cloneReturn->m_damageSphereList = NULL;
   if(m_damageSphereList)
	  m_damageSphereList->Clone(&cloneReturn->m_damageSphereList);

   cloneReturn->m_sphereBodyCollisionSystem = NULL;
   if(m_sphereBodyCollisionSystem)
      m_sphereBodyCollisionSystem->Clone(&cloneReturn->m_sphereBodyCollisionSystem);

   cloneReturn->m_sphereLocationalDamageList = NULL;
   if(m_sphereLocationalDamageList)
      m_sphereLocationalDamageList->Clone(&cloneReturn->m_sphereLocationalDamageList);

   if(m_bonesList){//bones
      if(copyAnimations == 1)
      m_bonesList->CloneCopyAnimations(&cloneReturn->m_bonesList,g_pd3dDevice);
	  else
      m_bonesList->Clone(&cloneReturn->m_bonesList,g_pd3dDevice);
	  cloneReturn->BuildHiarchy();
   }//end bones

   if(m_fireQueList)
      m_fireQueList->Clone(&cloneReturn->m_fireQueList);


   //cloneReturn->m_deformableMeshArrayCount = 0;//REMOVE
   if(cloneReturn->m_deformableMeshArrayCount > 0){//deformable meshs
          cloneReturn->m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
		  
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen

                cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit = NULL;

			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
			      
				  POSITION curCfgPos = NULL;
				  if(!spawnCfgOptional)
				  {
				      curCfgPos = altPtr->m_configurations->FindIndex(abs(m_spawnCfg[loop]));
				  }
				  else
				  {
				      curCfgPos = altPtr->m_configurations->FindIndex(abs(spawnCfgOptional[loop]));
				  }
			  
			       if(curCfgPos){
			         CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
			         dMesh->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit,g_pd3dDevice);
				   }
				   else
				   cloneReturn->m_deformableMeshArrayCount--;
				}
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit,g_pd3dDevice);
			  }//end based on runtime gen
		  }

		  if(m_deformableMeshArrayCount > 0)
		  {
		    cloneReturn->m_spawnCfg = new int[m_deformableMeshArrayCount];
            //CopyMemory(cloneReturn->m_spawnCfg,m_spawnCfg,(sizeof(int)*m_deformableMeshArrayCount));

			if(!spawnCfgOptional)
				CopyMemory(cloneReturn->m_spawnCfg,m_spawnCfg,(sizeof(int)*m_deformableMeshArrayCount));
		    else
				CopyMemory(cloneReturn->m_spawnCfg,spawnCfgOptional,(sizeof(int)*m_deformableMeshArrayCount));
		  }
   }//end deformable meshs
            
   *clone = cloneReturn;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CSkeletonObject::InitBasicDeformables(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
      if(m_deformableMeshArrayCount > 0){//deformable meshs
          m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
		  
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
                m_deformableMeshArrayV2[loop].m_defMeshVisualUnit = NULL;
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
			      
				   POSITION curCfgPos = altPtr->m_configurations->FindIndex(0);
			       if(curCfgPos){
			         CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
			         dMesh->Clone(&m_deformableMeshArrayV2[loop].m_defMeshVisualUnit,g_pd3dDevice);
				   }
				   else
				    m_deformableMeshArrayCount--;
				}
			  }//end alternate exists - this is based off of DB gen
		  }
   }//end deformable meshs
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSkeletonObject::EnableShadow(BOOL enable,
								   float radius,
								   float collisionRadius,
								   float depth,
								   float hieght,
								   int   textureIndex,
								   CString textureSearchName)
{
	if(enable == FALSE){
	   m_shadowOn = FALSE;
	   if(m_shadowInterface){
          delete m_shadowInterface;
		  m_shadowInterface=0;
	   }
	}
	else{
       if(m_shadowInterface){
          delete m_shadowInterface;
		  m_shadowInterface=0;
	   }
        m_shadowInterface = new CShadowClass(radius,
			                                 collisionRadius,
											 hieght,
											 depth,
											 TRUE,
											 textureIndex,
											 textureSearchName);
		m_shadowOn = TRUE;
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSkeletonObject::Invalidate()
{
    if(m_bonesList)
	   m_bonesList->Invalidate();


    if(m_deformableMeshArrayCount > 0){
	   if(m_deformableMeshArrayV2)
       for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
           if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit){
              m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Invalidate();
		   }
	   }
	}

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSkeletonObject::SafeDelete()
{
    if(m_soundRef)
	{
       m_soundRef->SetToDelete();
       m_soundRef = NULL;
	}

	if(m_lodRanges)
	{
       m_lodRanges->SafeDelete();
	   delete m_lodRanges;
	   m_lodRanges = 0;
	}

	if(m_bonesList){
	   m_bonesList->SafeDelete();
       delete m_bonesList;
       m_bonesList=0;
	}
	if(m_shadowInterface){
		  m_shadowInterface->SafeDelete();
          delete m_shadowInterface;
		  m_shadowInterface=0;
	}
	if(m_deformableMeshArrayCount > 0){
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

	if(m_sphereLocationalDamageList){
       m_sphereLocationalDamageList->SafeDelete();
	   delete m_sphereLocationalDamageList;
	   m_sphereLocationalDamageList=0;
	}

    if(m_particleEmiterList){
       m_particleEmiterList->SafeDelete();
	   delete m_particleEmiterList;
	   m_particleEmiterList=0;
	}

    if(m_calulationSphere){
       delete m_calulationSphere;
	   m_calulationSphere = 0;
	}
   
    if(m_damageSphereList){
	   m_damageSphereList->SafeDelete();
	   delete m_damageSphereList;
	   m_damageSphereList=0;
	}

    if(m_sphereBodyCollisionSystem){
       m_sphereBodyCollisionSystem->SafeDelete();
	   delete m_sphereBodyCollisionSystem;
	   m_sphereBodyCollisionSystem=0;
	}

	if(m_alternateConfigurations){
       m_alternateConfigurations->SafeDelete();
	   delete m_alternateConfigurations;
	   m_alternateConfigurations=0;
	}

	if(m_fireQueList)
	{
       m_fireQueList->SafeDelete();
	   delete m_fireQueList;
	   m_fireQueList = 0;
	}

	if(m_visibilityTrackList)
	{
       m_visibilityTrackList->SafeDelete();
	   delete m_visibilityTrackList;
	   m_visibilityTrackList = 0;
	}

	if(m_spawnCfg)
	{
       delete [] m_spawnCfg;
       m_spawnCfg = 0;
	}
}
//cacheUnneeded server stuff like meshes and animdata
void CSkeletonObject::DeleteUneededServerObjects()
{
	if(m_lodRanges)
	{
       m_lodRanges->SafeDelete();
	   delete m_lodRanges;
	   m_lodRanges = 0;
	}

	if(m_bonesList)
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
	{
		CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
	    if(!bonePtr || !bonePtr->m_animationList)
		    continue;//bones without animation frames simply have nothing to free
       for(POSITION animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	   {
	       CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
		   if(animPtr && animPtr->m_animationData)
		   {
				delete [] animPtr->m_animationData;
				animPtr->m_animationData = 0;
		   }
	   }
	}



	if(m_shadowInterface){
		  m_shadowInterface->SafeDelete();
          delete m_shadowInterface;
		  m_shadowInterface=0;
	}
	if(m_deformableMeshArrayCount > 0){
	   if(m_deformableMeshArrayV2)
       for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
           if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit){
              m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->SafeDelete();
			  delete m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
			  m_deformableMeshArrayV2[loop].m_defMeshVisualUnit=0;
		   }
	   }
	}

    if(m_particleEmiterList){
       m_particleEmiterList->SafeDelete();
	   delete m_particleEmiterList;
	   m_particleEmiterList=0;
	}

	if(m_alternateConfigurations)
	for(POSITION alPos = m_alternateConfigurations->GetHeadPosition();alPos != NULL;)
    {
	   CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetNext(alPos);
	   if(altPtr->m_configurations)
	   for(POSITION defListPos = altPtr->m_configurations->GetHeadPosition();defListPos != NULL;)
	   {//def list loop
			CDeformableMesh * dMeshHolder = (CDeformableMesh *)altPtr->m_configurations->GetNext(defListPos);
            dMeshHolder->m_lodChain->SafeDelete();
			delete dMeshHolder->m_lodChain;
			dMeshHolder->m_lodChain = 0;
	   }
    }
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Serialize-------------------------------------//
void CSkeletonObject::Serialize( CArchive& ar )
{
BOOL reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){

       ar.SetObjectSchema(5);
       ASSERT(ar.GetObjectSchema() == 5);

	   ar << m_SkeletalName
		  << m_bonesList
          << m_frameSpeed
          << m_ticksPerFrame
		  << m_deformableMeshArrayCount
		  << m_shadowOn
		  << m_shadowInterface
		  << m_freeLookBone
		  << m_sphereLocationalDamageList
		  << m_calulationSphere
		  << m_sphereBodyCollisionSystem
		  << m_damageSphereList
		  << m_particleEmiterList
		  << m_waistBone
	      << m_waistTurn
	      << m_waistTurnMax
	      << m_waistTurnRate
		  << m_fireQueList;

	   ar << m_alternateConfigurations
		  << m_secondaryCOMInheritBone
		  << m_freeLookBone2
		  << m_animPrefDefault
		  << reserved;

	   for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
           ar << m_spawnCfg[loop];
	   }

	   ar << m_sndVelocityBased
	      << m_sndID
		  << m_clipRadius
		  << m_soundFactor
		  << m_startFrequency
          << m_sndVelocityMaxDist;

	   ar << m_visibilityTrackList;

	   ar << m_lodRanges;
	  
	}
	else
	{

       int version = ar.GetObjectSchema();

       ar >> m_SkeletalName
		  >> m_bonesList
		  >> m_frameSpeed
		  >> m_ticksPerFrame
		  >> m_deformableMeshArrayCount
		  >> m_shadowOn
		  >> m_shadowInterface
		  >> m_freeLookBone
		  >> m_sphereLocationalDamageList
		  >> m_calulationSphere
		  >> m_sphereBodyCollisionSystem
		  >> m_damageSphereList
		  >> m_particleEmiterList
	      >> m_waistBone
	      >> m_waistTurn
	      >> m_waistTurnMax
	      >> m_waistTurnRate
		  >> m_fireQueList;

	   BuildHiarchy();

	   m_deformableMeshArrayV2 = NULL;

	   ar >> m_alternateConfigurations
		  >> m_secondaryCOMInheritBone
		  >> m_freeLookBone2
		  >> m_animPrefDefault
		  >> reserved;

	   if(m_deformableMeshArrayCount > 0)
	   m_spawnCfg = new int[m_deformableMeshArrayCount];
	   for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
           ar >> m_spawnCfg[loop];
	   }

	   m_sndVelocityBased   = FALSE;
	   m_sndID				= -1;
	   m_clipRadius         = 500.0f;
	   m_soundFactor		= 0.0f;
	   m_soundRef           = NULL;
	   m_startFrequency		= 1.0f;
       m_sndVelocityMaxDist = 5.0f;

	   if(version > 2)
	   {
		  ar >> m_sndVelocityBased
			 >> m_sndID
			 >> m_clipRadius
			 >> m_soundFactor
			 >> m_startFrequency
			 >> m_sndVelocityMaxDist;
	   }

	   m_visibilityTrackList = NULL;

	   if(version > 3)
	   {
          ar >> m_visibilityTrackList;
	   }

	   if(version > 4)
	   {
	      ar >> m_lodRanges;
	   }

       
	   m_currentLOD = 0;
	   m_lastUpdateTime            = 0;
	   m_animationInUse            = -1;
       m_lastAnimationInUse        = -1;
	   m_requireUpdate             = FALSE;
	   m_freeLookPitch             = 0;
       m_freeLookYaw               = 0;
	   m_inTransition              = FALSE;
       m_transitionTime            = 0;
       m_transitionStartTime       = 0;
	   m_transitionBaseFrame       = 0;
	   m_lastDurationUpdateTime    = 0;
       m_durationCurrentLength     = 0;
	   m_lastScndDurationUpdateTime= GetTickCount();
	   m_primaryAnimLastUpdatedTime  = m_lastScndDurationUpdateTime;
       m_secondaryAnimLastUpdatedTime = m_lastScndDurationUpdateTime;
       m_scndDurationCurrentLength = 0;
	   
	   m_inSecondaryTransition     = 0;
	   m_secondaryTransitionStamp  = 0;
	   m_inSecondaryTransition     = FALSE;
	   m_transitionOut             = FALSE;
	   m_secondaryTransitionDuration = 200;
	   m_secondaryCOMadjust.x        = 0.0f;
       m_secondaryCOMadjust.y        = 0.0f;
       m_secondaryCOMadjust.z        = 0.0f;
	   m_animPreference              = 0;
	   m_inReverse                   = FALSE;
	   m_currentTimePassedPrimary    = 0;
       m_currentTimePassedSecondary  = 0;
	   m_currentBaseFrame = 0;
       m_secondaryBaseFrame = 0;
	   m_currentBaseSoundFrame = -1;
       m_secondaryBaseSoundFrame = -1;
	   

	   if(version < 2)
	      InvertNormals();//PORT
	   
	   QuerryBox();

       #ifdef _AXSERVER
	   DeleteUneededServerObjects();
	   #endif

	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::InitBoneSystemFromPlugin(CString fileName,
											   LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);

     int physiqueModCount = 0;
	 int firstFrame = 0;
	 int lastFrame = 0;
	 int frameCount = 0;

	 int mod = 0;

	 the_in_Archive >> m_SkeletalName
		            >> m_frameSpeed
		            >> m_ticksPerFrame
		            >> m_bonesList
	          	    >> m_deformableMeshArrayCount
		            >> physiqueModCount
	                >> firstFrame
                    >> lastFrame
	                >> frameCount;
     
       if(m_deformableMeshArrayCount > 0){
	      m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
	      for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
              the_in_Archive >> m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
		  }
	   }

     

	 m_alternateConfigurations = NULL;
	 //port configurations
     m_alternateConfigurations = new CAlterUnitDBObjectList();
	 if(m_deformableMeshArrayCount > 0)
	 {//valid deformables
		m_spawnCfg = new int[m_deformableMeshArrayCount];
	    ZeroMemory(m_spawnCfg,(sizeof(int)*m_deformableMeshArrayCount));
	    for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		{
           CAlterUnitDBObject * newCfg = new CAlterUnitDBObject();
		   newCfg->m_configurations    = new CDeformableMeshList();
           CDeformableMesh    * dMesh = NULL;

           m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->CloneAllRaw(&dMesh,g_pd3dDevice);
		   //newCfg->m_alterUnitName = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_dimName;
		   newCfg->m_alterUnitName = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->DimName();
		   newCfg->m_configurations->AddTail(dMesh);

		   //CDeformableMesh    * dMesh2 = NULL;
		   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&dMesh2,g_pd3dDevice);
           //newCfg->m_configurations->AddTail(dMesh2);
		   
		   m_alternateConfigurations->AddTail(newCfg);
		   m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->SafeDelete();
		   delete m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
		   m_deformableMeshArrayV2[loop].m_defMeshVisualUnit = 0;

		}
		delete [] m_deformableMeshArrayV2;
	    m_deformableMeshArrayV2 = 0;
	 }//end valid deformables

	 

	 //return FALSE;
     //strip animation from bone list
     if(m_bonesList){
        for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
		{
            CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
			if(bonePtr->m_animationList){
			   bonePtr->m_animationList->SafeDelete();
			   delete bonePtr->m_animationList;
			   bonePtr->m_animationList=0;
			   bonePtr->m_animationList = new CBoneAnimationList();
			}
		}
	 }
	 
	 the_in_Archive.Close();
     fl.Close();
   
	 BuildHiarchy();
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::InitBoneSystem(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
BOOL res = InitBoneSystemFromPlugin(fileName,g_pd3dDevice);
QuerryBox();
//m_box = QuerryBoxExact(NULL);

return res;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::MergeAsAltCfg(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

CSkeletonObject * tempSkeletal = new CSkeletonObject();

tempSkeletal->InitBoneSystemFromPlugin(fileName,g_pd3dDevice);
tempSkeletal->QuerryBox();
if(tempSkeletal->m_alternateConfigurations)
if(m_alternateConfigurations)
{//alternate exists - this is based off of DB gen
   int indexTrace = 0;
   for(POSITION alPos = m_alternateConfigurations->GetHeadPosition();alPos != NULL;)
   {
	   CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetNext(alPos);
	   
	   POSITION mergePos = tempSkeletal->m_alternateConfigurations->FindIndex(indexTrace);
	   if(!mergePos)
		   continue;
	   CAlterUnitDBObject * altPtrMerge = (CAlterUnitDBObject *)tempSkeletal->m_alternateConfigurations->GetAt(mergePos);
	   

	   POSITION mergePos2 = altPtrMerge->m_configurations->FindIndex(0);
	   if(!mergePos2)
		   continue;
	   CDeformableMesh * dMeshHolder = (CDeformableMesh *)altPtrMerge->m_configurations->GetAt(mergePos2);

	   CDeformableMesh * dMeshHolderClone = NULL; 
	   dMeshHolder->CloneAllRaw(&dMeshHolderClone,g_pd3dDevice);
	   altPtr->m_configurations->AddTail(dMeshHolderClone);

	   //altPtr->m_configurations->AddTail(dMeshHolder);

       //altPtrMerge->m_configurations->RemoveAll();
       
	   indexTrace++;
   }
}

tempSkeletal->SafeDelete();
delete tempSkeletal;
tempSkeletal = 0;

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::MergeAsAltCfg2(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

CSkeletonObject * tempSkeletal = new CSkeletonObject();

tempSkeletal->InitBoneSystemFromPlugin(fileName,g_pd3dDevice);
tempSkeletal->QuerryBox();
if(tempSkeletal->m_alternateConfigurations)
if(m_alternateConfigurations)
{//alternate exists - this is based off of DB gen
   int indexTrace = 0;
   for(POSITION alPos = m_alternateConfigurations->GetHeadPosition();alPos != NULL;)
   {
	   CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetNext(alPos);
	   //altPtr->m_alterUnitName
	   
	   //POSITION mergePos = tempSkeletal->m_alternateConfigurations->FindIndex(indexTrace);
	   
	   CAlterUnitDBObject * altPtrMerge = tempSkeletal->m_alternateConfigurations->GetByName(altPtr->m_alterUnitName);
	   if(!altPtrMerge)
		   continue;
	   

	   POSITION mergePos2 = altPtrMerge->m_configurations->FindIndex(0);
	   if(!mergePos2)
		   continue;
	   CDeformableMesh * dMeshHolder = (CDeformableMesh *)altPtrMerge->m_configurations->GetAt(mergePos2);
       //dMeshHolder->m_lodChain
	   CDeformableMesh * dMeshHolderClone = NULL; 
	   dMeshHolder->CloneAllRaw(&dMeshHolderClone,g_pd3dDevice);
	   altPtr->m_configurations->AddTail(dMeshHolderClone);

	   //altPtr->m_configurations->AddTail(dMeshHolder);

       //altPtrMerge->m_configurations->RemoveAll();
       
	   indexTrace++;
   }
}

tempSkeletal->SafeDelete();
delete tempSkeletal;
tempSkeletal = 0;

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::MergeAsLod(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     return MergeAsLod(fileName,"",g_pd3dDevice);
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::MergeAsLod(CString fileName,CString ext,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
CSkeletonObject * tempSkeletal = new CSkeletonObject();

if(!tempSkeletal->InitBoneSystemFromPlugin(fileName,g_pd3dDevice))
{
	tempSkeletal->SafeDelete();
    delete tempSkeletal;
	tempSkeletal = 0;
    return FALSE;
}
tempSkeletal->QuerryBox();
if(tempSkeletal->m_alternateConfigurations)
if(m_alternateConfigurations)
{//alternate exists - this is based off of DB gen
   int indexTrace = 0;
   for(POSITION alPos = m_alternateConfigurations->GetHeadPosition();alPos != NULL;)
   {
	   CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetNext(alPos);
	   
	   CAlterUnitDBObject * altPtrMerge = tempSkeletal->m_alternateConfigurations->GetByName(altPtr->m_alterUnitName);
	   if(!altPtrMerge)
		   continue;
	   

	   POSITION mergePos2 = altPtrMerge->m_configurations->FindIndex(0);
	   if(!mergePos2)
		   continue;
	   CDeformableMesh * dMeshHolder = (CDeformableMesh *)altPtrMerge->m_configurations->GetAt(mergePos2);
       //dMeshHolder->m_lodChain
       CDeformableVisObj * newLodMesh = dMeshHolder->m_lodChain->GetByIndex(0);
	   if(newLodMesh)
	   {
          //CDeformableMesh * mainLodChain = altPtr->m_configurations->GetByName(dMeshHolder->m_dimName);
		   CDeformableMesh * mainLodChain = altPtr->m_configurations->GetByName(dMeshHolder->DimName());
		  if(mainLodChain)
		  {
             CDeformableVisObj * clone = NULL;
		     newLodMesh->CloneAllRaw(&clone,g_pd3dDevice);
		     mainLodChain->m_lodChain->AddTail(clone);
		  }
	   }
	   indexTrace++;
   }
}

//non deformable
CBoneObject * localBoneQuivelent = NULL;
int bnTrace = 0;
if(m_bonesList)
 if(tempSkeletal->m_bonesList)
  for(POSITION posLoc = tempSkeletal->m_bonesList->GetHeadPosition();posLoc != NULL;)
  {
    CBoneObject * bonePtr = (CBoneObject *)tempSkeletal->m_bonesList->GetNext(posLoc);
	if(!bonePtr->m_hiarchyMesh)
	{
		bnTrace++;
        continue;
	}

	CString boneNameCmp = bonePtr->m_boneName;
	if(ext.GetLength() > 0)
	   boneNameCmp = boneNameCmp.Mid(0,boneNameCmp.GetLength()-ext.GetLength());

	localBoneQuivelent = NULL;
	if(GetBoneByName(boneNameCmp,&localBoneQuivelent)==FALSE)
	   if(GetBoneByName(bonePtr->m_boneName,&localBoneQuivelent)==FALSE)
	      localBoneQuivelent = this->GetBoneByIndex(bnTrace);
	if(localBoneQuivelent)
	if(localBoneQuivelent->m_hiarchyMesh)
	{
		CHiarcleVisObj * cloneVMesh = NULL;
		CHiarcleVisObj * visPtr = (CHiarcleVisObj *)bonePtr->m_hiarchyMesh->GetByIndex(0);
        if(visPtr)
		{
		   visPtr->Clone(&cloneVMesh,g_pd3dDevice);
	       localBoneQuivelent->m_hiarchyMesh->m_lodChain->AddTail(cloneVMesh);
		}
	}
	bnTrace++;
  }






tempSkeletal->SafeDelete();
delete tempSkeletal;
tempSkeletal = 0;

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::ApplyAnimation(int animation,int frame)
{
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		bonePtr->ApplyAnimation(animation,frame);
	}
	return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::ApplyAnimationByTime(int animation,int time)
{
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		bonePtr->ApplyAnimationByTime(animation,time);
	}
	return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetAnimationSpeedBasedOnCap(int animNum,float percentage)
{
     //ofsset	 
     int currentTime = GetTickCount();
     BOOL calced = FALSE;
     if(m_bonesList)
     for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
         CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		 if(bonePtr->m_animationList){
		 POSITION posAnim = bonePtr->m_animationList->FindIndex(animNum);
		  if(posAnim){
			CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(posAnim);
            int newspeed = (int)((animPtr->m_timingCapMax * percentage)+animPtr->m_timingCapMin);
		    SetAnimationSpeed(animNum,newspeed);
		  }
		 }
	 }

	 return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::FadeOut(long duration)
{
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->SetFadeOutAlpha(duration);
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->SetFadeOutAlpha(duration);
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::FadeIn(long duration)
{

		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit)
			  if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain)
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->SetFadeInAlpha(duration);
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

		  if(m_bonesList)
          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->SetFadeInAlpha(duration);
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetOverrideTrans(float percentage)
{

		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit)
			  if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain)
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->SetOverrideTrans(percentage);
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

		  if(m_bonesList)
          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->SetOverrideTrans(percentage);
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::ResetMaterial()
{

		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit)
			  if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain)
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->Reset();
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

		  if(m_bonesList)
          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->Reset();
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::FadeInAmplified(long duration)
{
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->SetFadeIn(duration,TRUE);
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->SetFadeIn(duration,TRUE);
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::FadeOutAmplified(long duration)
{
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->SetFadeOut(duration);
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->SetFadeOut(duration);
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetAnimationSpeed(int animNum,long newSpeed)
{
     //ofsset	 
     DWORD currentTime = GetTickCount();
     BOOL calced = FALSE;

     for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
         CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
	     if(!bonePtr->m_animationList)//anatomy / helper bones have no keys of their own
			 continue;
	     POSITION posAnim = bonePtr->m_animationList->FindIndex(animNum);
		  if(posAnim){
			CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(posAnim);
            
			if(calced == FALSE){//do once
			DWORD animLength = animPtr->m_totalFrames*animPtr->m_millisecondsPerFrame;
			DWORD currentTimePassed = currentTime - m_lastUpdateTime;
			animPtr->m_millisecondsPerFrame = newSpeed;
		    DWORD animLength2 = animPtr->m_totalFrames*animPtr->m_millisecondsPerFrame;
			float var1 = (float)animLength;
			float var2 = (float)animLength2;
			float percentage = var2 / var1;
			currentTimePassed = (long)(currentTimePassed*percentage);
			m_lastUpdateTime = currentTime - currentTimePassed;
			calced = TRUE;
			}//end do once
			else
			animPtr->m_millisecondsPerFrame = newSpeed;
		  }
	 }

	 return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int CSkeletonObject::GetAnimationIndexFromAttrib(int attrib)
{
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(!posLoc)
	   return -1;
       CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
	   if(!bonePtr->m_animationList)
		   return -1;
	       int trace=0; 
	       for(posLoc = bonePtr->m_animationList->GetHeadPosition();posLoc != NULL;)
		   {
		       CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(posLoc);
	           if(animPtr->m_type == attrib)
				  return trace;
		       trace++;
		   }
	return -1;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::GetAnimationStatus(int *version,int *attrib,int animIndex)
{
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(!posLoc)
	   return FALSE;
       CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
	   if(!bonePtr->m_animationList)
		   return FALSE;
	       POSITION animPos = bonePtr->m_animationList->FindIndex(animIndex);
		   if(!animPos)
			   return FALSE;
		       CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(animPos);
	   
               *version = animPtr->m_version;
			   *attrib = animPtr->m_type;

	return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetAnimationBasedOnAttrib(int attrib,
												BOOL reverse,
												int versionPref,
												int transitionTime)
{
   int trace = 0;
   if(!m_bonesList)
	   return FALSE;

   //same version randomizing handler
   DWORD currentTime = GetTickCount();
   if(attrib == m_currentAnimType)
    if(m_currentAnimVersion == versionPref)
     if(currentTime-m_minSaveVerTimeStamp < m_minSameVersionAnimaPlay)
     {
        if(m_inReverse != reverse && m_animationInUse >= 0)
           SetAnimationDirection(m_animationInUse, reverse);
        return TRUE;//prevents switch
     }
   //end same version randomizing handler

   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){
      CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
      if(!bonePtr->m_animationList)
		  return FALSE;
      //randomize?


	  int versionDuplicateCount = 0;
	  POSITION animPos;
      for(animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_type == attrib && animPtr->m_version == versionPref){
	       versionDuplicateCount++;
		}
	  }
	  int randomWinner = 0;
	  if(versionDuplicateCount > 0)
		 randomWinner = rand()%versionDuplicateCount;
      //end randomize
      int randTrace = 0;
	  for(animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_type == attrib && animPtr->m_version == versionPref){
		  if(randomWinner == randTrace){//1
		   

			if(!SetCurrentAnimation(trace,transitionTime))
				return FALSE;

			m_durationCurrentLength = animPtr->m_minPlayDuration;
		    m_lastDurationUpdateTime = GetTickCount();
					
		    SetAnimationDirection(trace,reverse); 
		   
            return TRUE;
		  }//end 1
		  randTrace++;
		}
	    trace++;
	  }
   }
return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int  CSkeletonObject::GetAnimationBasedOnAttribAndPref(int attrib,
												       int versionPref,
													   CBoneAnimationObject ** returnPtr)
{
   int trace = 0;
   if(!m_bonesList)
	   return -1;

   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){
      CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
      if(!bonePtr->m_animationList)
		  return -1;
      //randomize?
	  int versionDuplicateCount = 0;
	  POSITION animPos;
      for(animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_type == attrib && animPtr->m_version == versionPref){
	       versionDuplicateCount++;
		}
	  }
	  int randomWinner = 0;
	  if(versionDuplicateCount > 0)
		 randomWinner = rand()%versionDuplicateCount;
	  else
		 return -1;
      //end randomize
      int randTrace = 0;
	  for(animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_type == attrib && animPtr->m_version == versionPref){
		  if(randomWinner == randTrace){//1
			 if(returnPtr)
			   *returnPtr = animPtr;
             return trace;
		  }//end 1
		  randTrace++;
		}
	    trace++;
	  }
   }
return -1;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetCurrentAnimation(int anim,int transitionTime)
{
if(anim == m_animationInUse)
   return TRUE;

POSITION posLoc = m_bonesList->FindIndex(0);
   if(!posLoc)
	   return FALSE;
       CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
       if(!bonePtr->m_animationList)
	   return FALSE;
          POSITION animPos = bonePtr->m_animationList->FindIndex(anim);
		  if(!animPos)
              return FALSE;
	          CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(animPos);

			  
			  m_lastUpdateTime              = GetTickCount();
			  
			  m_currentAnimType				= animPtr->m_type;
			  m_currentAnimVersion			= animPtr->m_version;
              m_primaryAnimLastUpdatedTime	= m_lastUpdateTime;
			  m_minSaveVerTimeStamp         = GetTickCount();
			  if(!animPtr->m_looping)
			    if(m_animationInUse == -1)
			    {//first spawn
                   m_primaryAnimLastUpdatedTime -= animPtr->m_millisecondsPerFrame * animPtr->m_totalFrames;
			    }//end first spawn

			  m_animationInUse              = anim;
			  if(m_animationInUse == 7)
				  m_animationInUse = 7;

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetCurrentAnimationByVersion(int version,int transitionTime)
{
   int trace = 0;
   if(!m_bonesList)
	   return FALSE;
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){
      CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
      if(bonePtr->m_animationList)
	  for(POSITION animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_version == version){
	       m_durationCurrentLength = animPtr->m_minPlayDuration;
		   m_lastDurationUpdateTime = GetTickCount();
		   //m_primaryAnimLastUpdatedTime = GetTickCount();//EXPERIMENT
			SetAnimationDirection(trace,FALSE);
			SetCurrentAnimation(trace,transitionTime);
           return TRUE;
		}
	    trace++;
	  }
   }
return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetAnimationDirection(int anim,BOOL reverse)
{
   if(!m_bonesList)
	   return FALSE;
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){//valid
    CBoneObject * bone = (CBoneObject *)m_bonesList->GetAt(posLoc);
	if(!bone->m_animationList)
        return FALSE; 
	POSITION animPos = bone->m_animationList->FindIndex(anim);
	  if(animPos){//valid 2
       
	   CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetAt(animPos);
	   if(m_inReverse != reverse)
	   {//affect
		DWORD currentTime = GetTickCount();
        DWORD timePassed = currentTime - m_primaryAnimLastUpdatedTime;
        DWORD animLength = (DWORD)animPtr->m_millisecondsPerFrame * (DWORD)animPtr->m_totalFrames;
		if(timePassed > 0 && timePassed < animLength)
		{
		   DWORD timeLeft = animLength - timePassed;
		   m_primaryAnimLastUpdatedTime = currentTime - timeLeft;//must mirror this
		}
		else
		{
		   m_primaryAnimLastUpdatedTime = currentTime;
		}
		
		m_inReverse = reverse;
		return TRUE;
	   }//end valid 2
	  }//end effect
   }//end valid
   return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::GetAnimationDirection(int anim)//forward = false
{
   if(!m_bonesList)
	   return FALSE;
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){//valid
    CBoneObject * bone = (CBoneObject *)m_bonesList->GetAt(posLoc);
	if(!bone->m_animationList)
        return FALSE; 
	POSITION animPos = bone->m_animationList->FindIndex(anim);
	  if(animPos){//valid 2
       
	   CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetAt(animPos);
	   if(m_inReverse == TRUE)
		  return TRUE;
	   else 
		  return FALSE;
	  }//end valid 2
   }//end valid
   return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int CSkeletonObject::GetAnimationAttribByIndex(int anim)//forward = false
{
   if(!m_bonesList)
	   return -1;

   if(anim < 0)
      return -1;


   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){//valid
    CBoneObject * bone = (CBoneObject *)m_bonesList->GetAt(posLoc);
	if(!bone->m_animationList)
        return FALSE; 
	POSITION animPos = bone->m_animationList->FindIndex(anim);
	  if(animPos){//valid 2
       
	   CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetAt(animPos);
	   return animPtr->m_type;
	  }//end valid 2
   }//end valid
   return -1;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::UpdateAnimMS(CBoneAnimationObject * animPtr,
								    DWORD currentTime,
									BOOL secondary)
{
        DWORD miliPerFrame = animPtr->m_millisecondsPerFrame;
	    
			if(miliPerFrame < 1)
			   miliPerFrame = 1;
			//interpolated variables
			DWORD animLength = (DWORD)animPtr->m_totalFrames*miliPerFrame;
			
			DWORD currentTimePassed = 1;

			if(!secondary)
			    currentTimePassed = currentTime - m_primaryAnimLastUpdatedTime;
			else
				currentTimePassed = currentTime - m_secondaryAnimLastUpdatedTime;

			BOOL isLocomotionLoop = (animPtr->m_looping != FALSE) ||
			                        (m_inReverse == TRUE && secondary == FALSE) ||
			                        (animPtr->m_type == 1 || animPtr->m_type == 2 ||
			                         animPtr->m_type == 3 || animPtr->m_type == 4 ||
			                         animPtr->m_type == 9 || animPtr->m_type == 10 ||
			                         animPtr->m_type == 11 || animPtr->m_type == 12 ||
			                         animPtr->m_type == 20 || animPtr->m_type == 21);

			if(currentTimePassed >= animLength){
			   if(!isLocomotionLoop)
			   {
                  currentTimePassed = animLength-1;
				  m_primaryAnimLastUpdatedTime = GetTickCount()-animLength;
                  if(secondary)
				    m_scndEndCap = 1;
			   }
			   else{
			      
				  if(!secondary)
				      m_primaryAnimLastUpdatedTime = currentTime;
				  else
                      m_secondaryAnimLastUpdatedTime = currentTime;
			      
				  
				  currentTimePassed = currentTimePassed - animLength;
			      if(currentTimePassed>animLength)
				  {
				     //currentTimePassed = 0;
				     currentTimePassed = currentTimePassed - animLength;//reset
                     if(currentTimePassed > animLength)
					 {//rescale
					    if(animLength > 0)
                           currentTimePassed -= (currentTimePassed/animLength)*animLength;
					 }//end rescale
				  }
			   }
			}
           

			//forward
               if(!secondary)
			       m_currentBaseFrame = currentTimePassed/miliPerFrame;
			   else
			       m_secondaryBaseFrame = currentTimePassed/miliPerFrame;
               
			//REVERSE //WORKING
		    if(m_inReverse == TRUE && secondary == FALSE){//switch animation to reverse-dont let weapons do this
               //animPtr->m_baseFrame = (animPtr->m_totalFrames - animPtr->m_baseFrame)-1;
			   //if(!secondary)
			       m_currentBaseFrame = (animPtr->m_totalFrames - m_currentBaseFrame)-1;
			   //else
			   //    m_secondaryBaseFrame = (animPtr->m_totalFrames - m_secondaryBaseFrame)-1;
			}//end switch animation to reverse  			
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::HandleSoundRangesForAnimation(CBoneAnimationObject * animPtr,
													 D3DVECTOR & wldSpacePos,
													 float & globalSoundFactor,
													 CSoundBankList * m_soundManager,
													 BOOL secondary,
													 D3DVECTOR & sndVelocityVect)
{
//sound activation ranges

	int baseFrameLoc = m_currentBaseFrame;
    if(secondary)
	   baseFrameLoc = m_secondaryBaseFrame;

    if(!secondary)
	{
	    if(m_currentBaseSoundFrame == m_currentBaseFrame)
		   return;

		m_currentBaseSoundFrame = m_currentBaseFrame;
	}
	else{
        if(m_secondaryBaseSoundFrame == m_secondaryBaseFrame)
		   return;

		m_secondaryBaseSoundFrame = m_secondaryBaseFrame;
	}
			if(animPtr->m_soundRanges)
				for(POSITION posSound = animPtr->m_soundRanges->GetHeadPosition();posSound != NULL;)
				{//SOUND LOOP
                   CSoundRangeObject* sndRange = (CSoundRangeObject*)animPtr->m_soundRanges->GetNext(posSound);
				   if(baseFrameLoc >= sndRange->m_startRange && baseFrameLoc <= sndRange->m_endRange){//in range
                       if(sndRange->m_inRange == FALSE){
						    if(sndRange->m_looping)
							{
								sndRange->m_soundRef = m_soundManager->PlayByID(sndRange->m_id,
																				TRUE,
																				wldSpacePos,
																				sndRange->m_soundFactor,
																				sndRange->m_radiusClipping);
							}
							else
							{
								m_soundManager->PlayByID(sndRange->m_id,
														 FALSE,
														 wldSpacePos,
														 sndRange->m_soundFactor,
														 sndRange->m_radiusClipping);
							}
								 
							sndRange->m_inRange = TRUE;
							//AfxMessageBox("play");
					   }

					   if(sndRange->m_soundRef)
					   if(sndRange->m_velocityBased)
					   {//maintain valocity if specified
                          //sndRange->m_soundRef->UpdateFreqBasedOnSpeed(sndVelocityVect);
					      float temp_Mxyz = ((sndVelocityVect.x * sndVelocityVect.x) + (sndVelocityVect.y * sndVelocityVect.y) + (sndVelocityVect.z * sndVelocityVect.z));
						  sndRange->m_soundRef->UpdateFreqBasedOnSpeed(1.0f,sqrt(temp_Mxyz),1.0f);
					   }//end maintain valocity if specified
				   }//end range
				   else
				   {//tag out of range
				      sndRange->m_inRange = FALSE;
                      if(sndRange->m_soundRef)
					  {
                         sndRange->m_soundRef->SetToDelete();
						 sndRange->m_soundRef = NULL;
					  }
				   }
				}//END SOUND LOOP
            //end sound activation ranges
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::ApplyFreeLook()
{
  if(m_freeLookBone > -1){//valid freelook bone
     
     float IntermPitch = m_freeLookPitch;
	 if(m_freeLookBone2 > 0)
        IntermPitch = IntermPitch * .5f;

     POSITION secondaryPos = m_bonesList->FindIndex(m_freeLookBone);
	 if(secondaryPos){//freelook secondary
	    CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
        D3DMATRIX applyMatrix;
	    GL_matrixFunctions.Rotate(0.0f,0.0f,IntermPitch,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformBefore(applyMatrix);
	    GL_matrixFunctions.Rotate(0.0f,m_freeLookYaw,0.0f,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
		if(m_freeLookBone2 > 0)
		{
		   secondaryPos = m_bonesList->FindIndex(m_freeLookBone2);
	       if(secondaryPos){//freelook secondary
	          boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
              D3DMATRIX applyMatrix;
	          GL_matrixFunctions.Rotate(0.0f,0.0f,IntermPitch,applyMatrix.m);
	          boneObject->m_boneFrame->AddTransformBefore(applyMatrix);
		   }
		}
	 }//end freelook secondary
  }//end valid freelook bone
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::ApplySecondaryAdjust(int secondaryAdjustBone)
{
  if(secondaryAdjustBone != -1){//valid bone
     POSITION secondaryPos = m_bonesList->FindIndex(m_freeLookBone);
	 if(secondaryPos){//1
	    CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
        D3DMATRIX applyMatrix;
	    GL_matrixFunctions.Translate(m_secondaryCOMadjust.x,m_secondaryCOMadjust.y,m_secondaryCOMadjust.z,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
	 }//end 1
  }//end bone
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::RemoveSecondaryAdjust(int secondaryAdjustBone)
{
  /*if(secondaryAdjustBone != -1){//valid bone
     POSITION secondaryPos = m_bonesList->FindIndex(m_freeLookBone);
	 if(secondaryPos){//1
	    CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
        D3DMATRIX applyMatrix;
	    GL_matrixFunctions.Translate(-m_secondaryCOMadjust.x,-m_secondaryCOMadjust.y,-m_secondaryCOMadjust.z,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
		
	 }//end 1
  }//end bone*/
  m_secondaryCOMadjust.x = 0.0f;
  m_secondaryCOMadjust.y = 0.0f;
  m_secondaryCOMadjust.z = 0.0f;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::RemoveFreeLook()
{
  if(m_freeLookBone > -1){//valid freelook bone
     
     float IntermPitch = m_freeLookPitch;
	 if(m_freeLookBone2 > 0)
        IntermPitch = IntermPitch * .5f;

     POSITION secondaryPos = m_bonesList->FindIndex(m_freeLookBone);
	 if(secondaryPos){//freelook secondary
	    CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
        D3DMATRIX applyMatrix;
	    GL_matrixFunctions.Rotate(0.0f,0.0f,-IntermPitch,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformBefore(applyMatrix);
	    GL_matrixFunctions.Rotate(0.0f,-m_freeLookYaw,0.0f,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
		if(m_freeLookBone2 > 0)
		{
		   secondaryPos = m_bonesList->FindIndex(m_freeLookBone2);
	       if(secondaryPos){//freelook secondary
	          boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
              D3DMATRIX applyMatrix;
	          GL_matrixFunctions.Rotate(0.0f,0.0f,-IntermPitch,applyMatrix.m);
	          boneObject->m_boneFrame->AddTransformBefore(applyMatrix);
		   }
		}
	 }//end freelook secondary
  }//end valid freelook bone
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::ApplyWaisteControl()
{
               if(m_waistBone != -1){//waiste control
				 POSITION secondaryPos = m_bonesList->FindIndex(m_waistBone);
				 if(secondaryPos){//freelook secondary
                  CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
                   D3DMATRIX applyMatrix;
				   GL_matrixFunctions.Rotate(0.0f,m_waistTurn,0.0f,applyMatrix.m);
	               boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
				   secondaryPos = m_bonesList->FindIndex(m_waistBone+2);
				   if(secondaryPos){//unhook
                      CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
                      D3DMATRIX applyMatrix;
				      GL_matrixFunctions.Rotate(0.0f,-(m_waistTurn*.8f),0.0f,applyMatrix.m);
	                  boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
				   }//end unhook
				 }//end freelook secondary
			   }//end waiste control*
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::RemoveWaisteControl()
{
               if(m_waistBone != -1){//waiste control
				 POSITION secondaryPos = m_bonesList->FindIndex(m_waistBone);
				 if(secondaryPos){//freelook secondary
                  CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
                   D3DMATRIX applyMatrix;
				   GL_matrixFunctions.Rotate(0.0f,-m_waistTurn,0.0f,applyMatrix.m);
	               boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
				   secondaryPos = m_bonesList->FindIndex(m_waistBone+2);
				   if(secondaryPos){//unhook
                      CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
                      D3DMATRIX applyMatrix;
				      GL_matrixFunctions.Rotate(0.0f,(m_waistTurn*.8f),0.0f,applyMatrix.m);
	                  boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
				   }//end unhook
				 }//end freelook secondary
			   }//end waiste control*
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::MoveBoneMangement(DWORD & currentTime,
										 long & millisecondsPerFrame,
										 CBoneObject * bonePtr,
				                         int & locAnimInUse,
			                             int & locanimationFrame,
										 BOOL & update,
										 long & overtimeReturn,
										 BOOL & secondaryInUse,
										 BOOL & newFrame
										 )
{
	// ------------------------------------------------------------------
	// Bones without animation keys of their own.
	//
	// The female anatomy bones (R_boob / L_boob / R_butt / L_butt) are added to
	// a skeleton at runtime and exist in no animation file, so their
	// m_animationList is empty / has no entry for the animation that is playing.
	// SetBoneToAnimationFrame() bails out on those bones and leaves
	// m_lastPosition and m_targetPosition at (0,0,0); the interpolation below
	// would then hand every weighted vertex a matrix built from the origin,
	// which collapses the bust / glutes onto the character's feet.
	//
	// Instead derive the frame matrix directly from the parent bone:
	//     frame = startPosition * ( inverse(parentStart) * parentFrame )
	// The bone is therefore 100% locked to the chest / pelvis for every
	// animation, needs no keyframes (0 bytes of animation RAM) and can never
	// pull its vertices to (0,0,0).
	// ------------------------------------------------------------------
	if(!BoneHasAnimationFrame(bonePtr, locAnimInUse))
	{
		ApplyParentDrivenBoneFrame(bonePtr);
		return;
	}

                 if(!bonePtr->m_lastPositionsAreInitialized)
				 {
					 bonePtr->SetBoneToAnimationFrame(locAnimInUse,
										              locanimationFrame,
										              millisecondsPerFrame,
										              currentTime);
					 if(newFrame)
					    m_secondaryCOMadjustApply = FALSE;
				 }

				 DWORD boneTimePassed = currentTime - bonePtr->m_lastToTargetTimeStamp;
				 if(boneTimePassed >= millisecondsPerFrame){
					overtimeReturn = boneTimePassed - millisecondsPerFrame;
                    bonePtr->SetBoneToAnimationFrame(locAnimInUse,
										             locanimationFrame,
										             millisecondsPerFrame,
										             currentTime);

					if(overtimeReturn > 0 && overtimeReturn <= millisecondsPerFrame)
					{
                       bonePtr->m_lastToTargetTimeStamp -= overtimeReturn;
                       boneTimePassed = overtimeReturn;//overtime;
					}
					else
					boneTimePassed = 0;

					if(secondaryInUse)
				    {
                      m_secondaryCOMadjustApply = TRUE;
					}
					if(newFrame)
					m_secondaryCOMadjustApply = FALSE;
				 }


				 if(boneTimePassed > millisecondsPerFrame)
					boneTimePassed = millisecondsPerFrame;


				  D3DVECTOR targetPos;
				    targetPos.x = bonePtr->m_targetPosition.x;
				    targetPos.y = bonePtr->m_targetPosition.y;
				    targetPos.z = bonePtr->m_targetPosition.z;
				    if(bonePtr->m_secondaryOverridable)
					if(m_secondaryCOMadjustApply)
				    {
                     targetPos.x += m_secondaryCOMadjust.x;
                     targetPos.y += m_secondaryCOMadjust.y;
					 targetPos.z += m_secondaryCOMadjust.z;
					 //bonePtr->m_lastPosition.x = targetPos.x;
					 //bonePtr->m_lastPosition.y = targetPos.y;
					 //bonePtr->m_lastPosition.z = targetPos.z;
				    }
				 
			
                 
                    D3DVECTOR newPosition = GL_matrixFunctions.InterpolatePoints(
		                                                               bonePtr->m_lastPosition, 
						                                               targetPos, 
					                                                   millisecondsPerFrame, 
					                                                   boneTimePassed);
					D3DVECTOR upVector    = GL_matrixFunctions.InterpolatePoints(
		                                                               bonePtr->m_lastUpVector, 
						                                               bonePtr->m_targetUpVector, 
					                                                   millisecondsPerFrame, 
					                                                   boneTimePassed);
					D3DVECTOR dirVector   = GL_matrixFunctions.InterpolatePoints(
		                                                               bonePtr->m_lastDirVector, 
						                                               bonePtr->m_targetDirVector, 
					                                                   millisecondsPerFrame, 
					                                                   boneTimePassed);

					bonePtr->m_currentVisibility = GL_matrixFunctions.InterpolateFloatsHiRes(bonePtr->m_lastVisibility,
						                                                                     bonePtr->m_targetVisibility,
                                                                                             (float)millisecondsPerFrame,
																							 (float)boneTimePassed);


					

					upVector  = GL_matrixFunctions.Normalize(upVector);
                    dirVector = GL_matrixFunctions.Normalize(dirVector);

                    GL_matrixFunctions.SetOrientation(&bonePtr->m_boneFrame->List_frameMatrix,
								                       dirVector,
								                       upVector);
                    bonePtr->m_boneFrame->List_frameMatrix._41 = newPosition.x;
                    bonePtr->m_boneFrame->List_frameMatrix._42 = newPosition.y;
                    bonePtr->m_boneFrame->List_frameMatrix._43 = newPosition.z;
					
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 BOOL CSkeletonObject::AnimationCallback(   DWORD currentTime,
											D3DMATRIX & earCenter,
											D3DVECTOR & wldSpacePos,
											float globalSoundFactor,
											BOOL soundOnly,
											CSoundBankList * m_soundManager,
											BOOL disableSoundOption,
											BOOL & disableDeformbleCalc,
											LPDIRECT3DDEVICE9 g_pd3dDevice,
											D3DVECTOR & velocityVect,
											int & controlType,
											int & lodOverride)
{
    m_bjWorldY = wldSpacePos.y;   // bone jiggle feels jumps/landings
    m_bjHaveWorld = TRUE;
    m_curLod = m_currentLOD;
    if(lodOverride > -1)
	   m_curLod = lodOverride;

	//sound base
	if(!disableSoundOption)
	if(this->m_sndID > -1)
	{
	   D3DVECTOR tempXr;
	   tempXr.x = earCenter._41;
	   tempXr.y = earCenter._42;
	   tempXr.z = earCenter._43;
       //check clipping
       if(GL_matrixFunctions.MagnitudeSquared(tempXr.x,tempXr.y,tempXr.z,wldSpacePos.x,wldSpacePos.y,wldSpacePos.z)>this->m_clipRadius)
	   {//clip if valid
          if(this->m_soundRef)
             this->m_soundRef->SetToDelete();
		     this->m_soundRef = NULL;//detach
	   }
	   else
	   {//in range
		//end check clipping
		if(!this->m_soundRef)
		{
			this->m_soundRef = m_soundManager->PlayByID( this->m_sndID,
															TRUE,
															wldSpacePos,
															this->m_soundFactor,
															this->m_clipRadius);
		}
		else
		{//maintain positional sound	
			     this->m_soundRef->UpdatePosition(wldSpacePos);
		}//end maintain positional sound
		//maintain
		if(this->m_soundRef)
		if(this->m_sndVelocityBased)
		{//velocity based
			float temp_Mxyz = ((velocityVect.x * velocityVect.x) + (velocityVect.y * velocityVect.y) + (velocityVect.z * velocityVect.z));
			this->m_soundRef->UpdateFreqBasedOnSpeed(this->m_sndVelocityMaxDist,sqrt(temp_Mxyz),this->m_startFrequency);
		}
	   }//end in range
	}
	
	m_scndEndCap = 0;//reset end cap
    if(m_animationInUse == -1)//no animation current
	{
	   if(!disableDeformbleCalc)//optimization for AISERVER
           UpdateBasedDeformationByVertexAssignments(m_curLod,g_pd3dDevice);//VERTEX assignments
       return FALSE;
	}
	BOOL loopingAnim = FALSE;
    if(!m_bonesList)
	    return FALSE;
	//convert to ear space
	//earCenter
	//D3DVECTOR localSpacePos;
	//ZeroMemory(&localSpacePos,sizeof(D3DVECTOR));
    //if(!disableSoundOption)
    //localSpacePos = GL_matrixFunctions.GetLocalPositionFromMatrixView(earCenter,wldSpacePos);


	POSITION posLoc = m_bonesList->FindIndex(0);
     if(!posLoc)
		 return FALSE;
	    CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
		if(!bonePtr->m_animationList)
		{
			if(!disableDeformbleCalc)//optimization for AISERVER
               UpdateBasedDeformationByVertexAssignments(m_curLod,g_pd3dDevice);//VERTEX assignments
			return FALSE;
		}
		POSITION posAnim = bonePtr->m_animationList->FindIndex(m_animationInUse);
		if(!posAnim)
		{
			if(!disableDeformbleCalc)//optimization for AISERVER
               UpdateBasedDeformationByVertexAssignments(m_curLod,g_pd3dDevice);//VERTEX assignments
			return FALSE;
		}

		CBoneAnimationObject *animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(posAnim);


		

			
            long scndryTimePassed = 0;
		
		CBoneAnimationObject* secondaryAnim = NULL;
		if(m_secondaryAnimationInUse != -1)
		{
		  POSITION posAnim2 = bonePtr->m_animationList->FindIndex(m_secondaryAnimationInUse);
		  if(!posAnim)
			return FALSE;
			secondaryAnim = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(posAnim2);
		}	
			
			
            if(!secondaryAnim)
				ZeroMemory(&m_secondaryCOMadjust,sizeof(D3DVECTOR));

			BOOL updatePrimary    = FALSE;
			BOOL updateSecondary  = FALSE;
			int  lastFrame     = m_currentBaseFrame;


			
			//interpolated variables
			UpdateAnimMS(animPtr,currentTime,FALSE);

            if(secondaryAnim){
			  int lastFrameSecondary = m_secondaryBaseFrame;
			  UpdateAnimMS(secondaryAnim,currentTime,TRUE);
			  if(m_secondaryBaseFrame != lastFrameSecondary)
                 updateSecondary = TRUE;
			}

              if(m_currentBaseFrame != lastFrame)
                 updatePrimary = TRUE;

		    /*RemoveSecondaryAdjust(m_secondaryCOMInheritBone);
		    D3DVECTOR bonePosition;
			bonePtr->m_boneFrame->GetPosition(NULL,&bonePosition);
            m_secondaryCOMadjust = animPtr->GetBonePositionalDifference(bonePosition);*/
			
			if(!disableSoundOption)
			{
			  if(secondaryAnim)
			     HandleSoundRangesForAnimation(secondaryAnim,wldSpacePos,globalSoundFactor,m_soundManager,TRUE,velocityVect);//sound range callback
			
			     HandleSoundRangesForAnimation(animPtr,wldSpacePos,globalSoundFactor,m_soundManager,FALSE,velocityVect);//sound range callback
			}
			
			if(soundOnly)
			   return TRUE;

			//updatepositions
			 int locAnimInUse          = 0;
			 int locanimationFrame     = 0;
			 long millisecondsPerFrame = 0;
			 long secondaryOvertime    = 0;
			 long primaryOvertime      = 0;
			 long currentOvertime      = 0;
			 BOOL updateBone           = FALSE;
			 BOOL secondaryInUse       = FALSE;
			 BOOL secondaryEverUsed    = FALSE;
			 long tempMiliPerFrame     = 1000;
			 BOOL secondOffsetInitialized = FALSE;
			 BOOL deactivate = FALSE;

			 			 
			 //RemoveSecondaryAdjust(m_secondaryCOMInheritBone);
			 RemoveFreeLook();//unload free look
			 RemoveWaisteControl();//unload waise control
			 
			 if(!secondaryAnim)
			     deactivate = TRUE;



             if(disableDeformbleCalc)
			 if(m_fireQueList)
		     for(POSITION firePos = m_fireQueList->GetHeadPosition();firePos != NULL;)
		     {
				CFireRangeObject * fireRange = (CFireRangeObject *)m_fireQueList->GetNext(firePos);
				if(fireRange->m_boneIndex < 0)
				   continue;
				
				POSITION posLoc = m_bonesList->FindIndex(fireRange->m_boneIndex);
				if(!posLoc)
				    continue;
				CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
				UpdateBone(bonePtr,
							animPtr,
							currentOvertime,
							currentTime,
							deactivate,
							locAnimInUse,
			                locanimationFrame,
							millisecondsPerFrame,
							updateBone,
							secondaryInUse,
							secondaryEverUsed,
							secondOffsetInitialized,
							updatePrimary,
							updateSecondary,
							primaryOvertime,
							secondaryOvertime,
							tempMiliPerFrame,
							secondaryAnim);

		     }

             if(!disableDeformbleCalc)
			 for(POSITION bPos = m_bonesList->GetHeadPosition();bPos != NULL;){//bone loop
                 CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(bPos);
				 UpdateBone(bonePtr,
							animPtr,
							currentOvertime,
							currentTime,
							deactivate,
							locAnimInUse,
			                locanimationFrame,
							millisecondsPerFrame,
							updateBone,
							secondaryInUse,
							secondaryEverUsed,
							secondOffsetInitialized,
							updatePrimary,
							updateSecondary,
							primaryOvertime,
							secondaryOvertime,
							tempMiliPerFrame,
							secondaryAnim);
				
			 }//end bone loop
			 //secondOffsetInitialized = FALSE;

			 m_primaryAnimLastUpdatedTime -= primaryOvertime;
			 if(secondaryAnim)
                m_secondaryAnimLastUpdatedTime -= secondaryOvertime;

			 ApplyFreeLook();//Implement Secondary free look
			 ApplyWaisteControl();//waiste control

             if(!disableDeformbleCalc)//optimization for AISERVER
             UpdateBasedDeformationByVertexAssignments(m_curLod,g_pd3dDevice);//VERTEX assignments
	m_requireUpdate = TRUE;
	return TRUE;
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void CSkeletonObject::UpdateBone(CBoneObject * bonePtr,
								 CBoneAnimationObject* animPtr,
								 long & currentOvertime,
								 DWORD & currentTime,
								 BOOL & deactivate,
								 int & locAnimInUse,
			                     int & locanimationFrame,
								 long & millisecondsPerFrame,
								 BOOL & updateBone,
								 BOOL & secondaryInUse,
								 BOOL & secondaryEverUsed,
								 BOOL & secondOffsetInitialized,
								 BOOL & updatePrimary,
								 BOOL & updateSecondary,
								 long & primaryOvertime,
								 long & secondaryOvertime,
								 long & tempMiliPerFrame,
								 CBoneAnimationObject* secondaryAnim)
{
                 locAnimInUse			= m_animationInUse;
			     locanimationFrame		= m_currentBaseFrame;
				 millisecondsPerFrame	= animPtr->m_millisecondsPerFrame;
				 tempMiliPerFrame		= animPtr->m_millisecondsPerFrame;
				 updateBone				= updatePrimary;
				 secondaryInUse			= FALSE;
				 if(bonePtr->m_secondaryOverridable)
				 {//secondaryAnimation
					if(!secondOffsetInitialized)
					{
						  if(m_secondaryCOMInheritBone > -1)
			              if(m_secondaryCOMInheritBone < m_bonesList->m_directAccessArrayCount)
						  {
					       CBoneObject * bonePtrOverrode = m_bonesList->m_directAccessArray[m_secondaryCOMInheritBone].object;
				           D3DVECTOR currentPos;
				           bonePtrOverrode->m_boneFrame->GetPosition(NULL,&currentPos);
                           m_secondaryCOMadjust = animPtr->GetBonePositionalDifference(currentPos);
                           secondOffsetInitialized = TRUE;
						  }
				    }
                    if(secondaryAnim)
					{
					   
                       

                       locAnimInUse = m_secondaryAnimationInUse;
			           locanimationFrame = m_secondaryBaseFrame;
                       millisecondsPerFrame = secondaryAnim->m_millisecondsPerFrame;
					   updateBone = updateSecondary;
					   secondaryInUse = TRUE;
					   secondaryEverUsed = TRUE;
					}
				 }//secondaryAnimation
                 MoveBoneMangement(currentTime,
								   millisecondsPerFrame,
								   bonePtr,
				                   locAnimInUse,
			                       locanimationFrame,
								   updateBone,
								   currentOvertime,
								   secondaryInUse,
								   deactivate
								  );

				 if(updateBone)
				 {
                  if(secondaryInUse)
                    secondaryOvertime = currentOvertime;
				  else
				    primaryOvertime = currentOvertime;
				 }
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------load animation--------------------------------//

//------------------------------------------------------------------//
// Bone jiggle: bones the artists already rigged for it (names with
// "boob", "breast" or "butt") lag behind the animation on a spring.
// The mesh follows those bones through its own painted weights, so the
// shape stays right - only the bone moves, nothing is guessed.
//------------------------------------------------------------------//
void CSkeletonObject::BoneJiggleApply()
{
	if(!m_bonesList || !m_bonesList->m_directAccessArray)
		return;
	const int count = m_bonesList->m_directAccessArrayCount;
	if(m_bjCount < 0)
	{// first time: find the jiggle bones
		m_bjCount = 0;
		for(int b = 0; b < count && m_bjCount < 4; b++)
		{
			CBoneObject * bp = m_bonesList->m_directAccessArray[b].object;
			if(!bp) continue;
			CString nm = bp->m_boneName;
			nm.MakeLower();
			if(nm.Find("boob") < 0 && nm.Find("breast") < 0 && nm.Find("butt") < 0)
				continue;
			int k = m_bjCount++;
			m_bjBone[k] = b;
			m_bjInit[k] = FALSE;
			// how far it may swing: a share of its distance to the parent bone
			float reach = 1.0f;
			for(int p = 0; p < count; p++)
			{
				CBoneObject * pp = m_bonesList->m_directAccessArray[p].object;
				if(pp && p != b && pp->m_boneName.CompareNoCase(bp->m_parentName) == 0)
				{
					float dx = bp->m_startPosition._41 - pp->m_startPosition._41;
					float dy = bp->m_startPosition._42 - pp->m_startPosition._42;
					float dz = bp->m_startPosition._43 - pp->m_startPosition._43;
					reach = sqrtf(dx * dx + dy * dy + dz * dz);
					break;
				}
			}
			m_bjMaxOff[k] = reach * 0.15f;
		}
	}
	if(m_bjCount == 0)
		return;

	DWORD now = GetTickCount();
	float dt = (m_bjTick == 0) ? 0.0f : (now - m_bjTick) * 0.001f;
	m_bjTick = now;
	if(dt > 0.05f) dt = 0.05f;

	// how hard the body itself is speeding up / slowing down up-down (jump, land, running bob)
	float bodyAccY = 0.0f;
	if(m_bjHaveWorld && dt > 0.0f)
	{
		float vy = (m_bjWorldY - m_bjLastY) / dt;
		if(m_bjHadWorld)
			bodyAccY = (vy - m_bjLastVy) / dt;
		if(bodyAccY > 4000.0f) bodyAccY = 4000.0f;
		if(bodyAccY < -4000.0f) bodyAccY = -4000.0f;
		m_bjLastVy = vy;
		m_bjHadWorld = TRUE;
	}
	m_bjLastY = m_bjWorldY;
	const float stiff = 160.0f, damp = 9.0f;
	for(int k = 0; k < m_bjCount; k++)
	{
		int b = m_bjBone[k];
		if(b < 0 || b >= count) continue;
		CBoneObject * bp = m_bonesList->m_directAccessArray[b].object;
		if(!bp || !bp->m_boneFrame) continue;
		D3DMATRIX & m = bp->m_boneFrame->List_frameMatrix;
		m_bjSaved[k] = m;
		float target[3] = { m._41, m._42, m._43 };
		if(!m_bjInit[k])
		{
			for(int a = 0; a < 3; a++) { m_bjPos[k][a] = target[a]; m_bjVel[k][a] = 0.0f; }
			m_bjInit[k] = TRUE;
			continue;
		}
		float off[3], len2 = 0.0f;
		for(int a = 0; a < 3; a++)
		{
			float acc = stiff * (target[a] - m_bjPos[k][a]) - damp * m_bjVel[k][a];
			if(a == 1) acc -= bodyAccY * 0.35f;   // the body jumps, the soft part lags
			m_bjVel[k][a] += acc * dt;
			m_bjPos[k][a] += m_bjVel[k][a] * dt;
			off[a] = m_bjPos[k][a] - target[a];
			len2 += off[a] * off[a];
		}
		float len = sqrtf(len2);
		if(len > m_bjMaxOff[k] && len > 0.0f)
		{// never let it fly off: pull the spring back inside its reach
			float sc = m_bjMaxOff[k] / len;
			for(int a = 0; a < 3; a++) { off[a] *= sc; m_bjPos[k][a] = target[a] + off[a]; }
		}
		m._41 += off[0];
		m._42 += off[1];
		m._43 += off[2];
	}
}

void CSkeletonObject::BoneJiggleRestore()
{
	if(!m_bonesList || !m_bonesList->m_directAccessArray || m_bjCount <= 0)
		return;
	for(int k = 0; k < m_bjCount; k++)
	{
		int b = m_bjBone[k];
		if(b < 0 || b >= m_bonesList->m_directAccessArrayCount) continue;
		CBoneObject * bp = m_bonesList->m_directAccessArray[b].object;
		if(bp && bp->m_boneFrame && m_bjInit[k])
			bp->m_boneFrame->List_frameMatrix = m_bjSaved[k];
	}
}

void CSkeletonObject::UpdateBasedDeformationByVertexAssignments(int & dmIndex,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	   BoneJiggleApply();   // chest/butt bones swing on a spring; restored at the end

	   int tempInt = 0;
	   ABVERTEX1Lb * tempV = NULL;
	   ABVERTEX1Lb * tempV2 = NULL;
	   int minSize = sizeof(ABVERTEX0L);
	   byte * vData = NULL;
	   for(int i=0;i<m_deformableMeshArrayCount;i++)
	   {
               if(m_deformableMeshArrayV2[i].m_defMeshVisualUnit){//p
                  CDeformableVisObj * deformPtr = NULL;
				  if(m_deformableMeshArrayV2[i].m_defMeshVisualUnit->m_lodChain)
					  deformPtr = (CDeformableVisObj*)m_deformableMeshArrayV2[i].m_defMeshVisualUnit->m_lodChain->GetByIndex(dmIndex);
                  if(deformPtr){//1
				     int vSize = deformPtr->m_mesh->m_abVertexArray.GetVertexSize();
					 vData = (byte *)deformPtr->m_mesh->m_abVertexArray.GetVertexBuf();
				    
						if(deformPtr->m_blendedType == 0)
						 for(int x=0;x<deformPtr->m_assignmentCount;x++)
						 {//blended
							tempV = (ABVERTEX1Lb *)&vData[deformPtr->m_vertexAssignments[x].vertexIndex * vSize];
							ZeroMemory(tempV,sizeof(ABVERTEX0L));

							CBoneObject * bonePtr = m_bonesList->m_directAccessArray[deformPtr->m_vertexAssignments[x].boneIndex].object;	
							 if(bonePtr)
	                         {//valid bone
								 if(deformPtr->m_mesh)
					                deformPtr->m_mesh->m_visibilityTrack = bonePtr->m_currentVisibility;

								 
								 

								 tempV->x += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][0]));
                                 tempV->y += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][1]));
                                 tempV->z += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][2]));
								 //normal
								 tempV->nx += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].nz));
                                 tempV->ny += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].nz));
                                 tempV->nz += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].nz));
								
							 }//end valid bone

							 for(int ownedLoop=0;ownedLoop<deformPtr->m_vertexAssignments[x].carriedCount;ownedLoop++)
							 {
							   tempV2 = (ABVERTEX1Lb *)&vData[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop] * vSize];
							   
							   CopyMemory(tempV2,tempV,minSize);
							 }

						 }


						 if(deformPtr->m_blendedType == 1)
						 for(int x=0;x<deformPtr->m_assignmentCount;x++)
						 {//blended
							 
							 tempV = (ABVERTEX1Lb *)&vData[deformPtr->m_blendedVertexAssignments[x].vertexIndex * vSize];
							 ZeroMemory(tempV,sizeof(ABVERTEX0L));
							 for(int xBonesLoop=0;xBonesLoop<deformPtr->m_blendedVertexAssignments[x].boneIndexesCount;xBonesLoop++)
	                         {//vertex/bone blend calc
								 if(deformPtr->m_blendedVertexAssignments[x].boneIndexes[xBonesLoop] >= m_bonesList->m_directAccessArrayCount)
						            continue;
                                 CBoneObject * bonePtr = m_bonesList->m_directAccessArray[deformPtr->m_blendedVertexAssignments[x].boneIndexes[xBonesLoop]].object;	
								 if(bonePtr)
		                         {//valid bone
                                    if(deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop] == 0.0f)
		                               continue;

									 if(deformPtr->m_mesh)
						                deformPtr->m_mesh->m_visibilityTrack = bonePtr->m_currentVisibility;

									 tempV->x += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][0]))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
                                     tempV->y += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][1]))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
                                     tempV->z += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][2]))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
									 //normal
									 tempV->nx += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nz))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
                                     tempV->ny += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nz))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
                                     tempV->nz += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nz))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
									 //tangent
								 }//end valid bone
	                        }//end vertex/bone blend calc
                         
							 for(int ownedLoop=0;ownedLoop<deformPtr->m_blendedVertexAssignments[x].carriedCount;ownedLoop++)
							 {
							   tempV2 = (ABVERTEX1Lb *)&vData[deformPtr->m_blendedVertexAssignments[x].vertexesCarried[ownedLoop] * vSize];
							   
							   CopyMemory(tempV2,tempV,minSize);
							 }
						 }//end blended
						 
						 
						 if(deformPtr->m_blendedType == 2)
						 {//blended type 2
						   ABVERTEX1Lb * varBuild = new ABVERTEX1Lb; 
						   int shiftTempVar = sizeof(ABVERTEX1Lb)-12;
						   int miniShift = vSize-12;
						   byte * tempBytArray = NULL;
						   byte * primaryArray = NULL;
							 for(int x=0;x<deformPtr->m_assignmentCount;x++)
							 {//blended
								 ZeroMemory(varBuild,sizeof(ABVERTEX1Lb));//reset the build
								 for(int xBonesLoop=0;xBonesLoop<deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexesCount;xBonesLoop++)
								 {//vertex/bone blend calc
									 if(deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexes[xBonesLoop] >= m_bonesList->m_directAccessArrayCount)
										continue;
									 CBoneObject * bonePtr = m_bonesList->m_directAccessArray[deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexes[xBonesLoop]].object;	
									 if(bonePtr)
									 {//valid bone
										if(deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop] == 0.0f)
										   continue;

										 if(deformPtr->m_mesh)
											deformPtr->m_mesh->m_visibilityTrack = bonePtr->m_currentVisibility;

										 varBuild->x += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][0]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][0]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][0]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][0]))*deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop];
										 varBuild->y += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][1]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][1]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][1]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][1]))*deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop];
										 varBuild->z += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][2]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][2]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][2]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][2]))*deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop];
										 //normal
										 varBuild->nx += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][0]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][0]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][0]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].nz))*deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop];
										 varBuild->ny += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][1]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][1]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][1]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].nz))*deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop];
										 varBuild->nz += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][2]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][2]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][2]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].nz))*deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop];
										 //tangen
											varBuild->tX += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][0]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tX)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][0]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tY)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][0]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tZ))*deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop];
											varBuild->tY += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][1]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tX)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][1]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tY)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][1]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tZ))*deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop];
											varBuild->tZ += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][2]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tX)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][2]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tY)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][2]*deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tZ))*deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone[xBonesLoop];
									 }//end valid bone
								}//end vertex/bone blend calc

								primaryArray = &vData[deformPtr->m_blendedVertexAssignmentsWt[x].vertexIndex * vSize];
								CopyMemory(primaryArray,varBuild,minSize);
								tempBytArray = (byte*)varBuild;
								CopyMemory(&primaryArray[miniShift],&tempBytArray[shiftTempVar],12);
	                            
								 for(int ownedLoop=0;ownedLoop<deformPtr->m_blendedVertexAssignmentsWt[x].carriedCount;ownedLoop++)
								 {
								   primaryArray = &vData[deformPtr->m_blendedVertexAssignmentsWt[x].vertexesCarried[ownedLoop] * vSize];
								   CopyMemory(primaryArray,varBuild,minSize);
								   CopyMemory(&primaryArray[miniShift],&tempBytArray[shiftTempVar],12);
								 }
							 }//end blended
							 delete varBuild;
							 varBuild = 0;
						 }//end blended type 2
				  }//end 1
			   }//end p
		   }

       for(int r=0;r<m_deformableMeshArrayCount;r++)
	   {//reinit
               if(m_deformableMeshArrayV2[r].m_defMeshVisualUnit
			   && m_deformableMeshArrayV2[r].m_defMeshVisualUnit->m_lodChain)
			   {//p
                  CDeformableVisObj * deformPtr = (CDeformableVisObj*)m_deformableMeshArrayV2[r].m_defMeshVisualUnit->m_lodChain->GetByIndex(dmIndex);
			      if(deformPtr)
				     deformPtr->m_mesh->m_abVertexArray.ReinitializeDxVbuffersFS(g_pd3dDevice);
					 //deformPtr->m_mesh->m_abVertexArray.ReinitializeDxVbuffersPositionOnly(g_pd3dDevice);
				  //deformPtr->m_mesh->InitBuffer(g_pd3dDevice);
			   }
			   
	   }//end reinit
	   BoneJiggleRestore();
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::InvertNormals()
{
	    int tracer = 0;
		D3DMATRIX boneMatrix;
		float posX,posY,posZ,normX,normY,normZ;
        if(m_bonesList)
        for(POSITION bPos = m_bonesList->GetHeadPosition();bPos != NULL;)
		{//bone loop
           CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(bPos);	

           
		   bonePtr->m_boneFrame->GetInverseTransform(&boneMatrix);

           //for(int i=0;i<m_deformableMeshArrayCount;i++)
		   //{//def loop

	        if(m_alternateConfigurations)
            for(POSITION alPos = m_alternateConfigurations->GetHeadPosition();alPos != NULL;)
			{//cfgs loop
			  CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetNext(alPos);
              
			  if(altPtr->m_configurations)
			  for(POSITION defListPos = altPtr->m_configurations->GetHeadPosition();defListPos != NULL;)
			  {//def list loop
			   CDeformableMesh * dMeshHolder = (CDeformableMesh *)altPtr->m_configurations->GetNext(defListPos);
			   
			   if(dMeshHolder->m_lodChain)
			   for(POSITION posLod = dMeshHolder->m_lodChain->GetHeadPosition();posLod != NULL;)
			   {//1
			      CDeformableVisObj * deformPtr = (CDeformableVisObj*)dMeshHolder->m_lodChain->GetNext(posLod);
				     for(int x=0;x<deformPtr->m_assignmentCount;x++)
					 {//as
                         if(deformPtr->m_vertexAssignments[x].boneIndex == tracer)
						 {//found bone
							
							//position
							 
                            //posX = (boneMatrix.m[0][0]*deformPtr->m_mesh->m_abVertexArray.GetVertexX(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[1][0]*deformPtr->m_mesh->m_abVertexArray.GetVertexY(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[2][0]*deformPtr->m_mesh->m_abVertexArray.GetVertexZ(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[3][0]);
                            //posY = (boneMatrix.m[0][1]*deformPtr->m_mesh->m_abVertexArray.GetVertexX(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[1][1]*deformPtr->m_mesh->m_abVertexArray.GetVertexY(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[2][1]*deformPtr->m_mesh->m_abVertexArray.GetVertexZ(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[3][1]);
                            //posZ = (boneMatrix.m[0][2]*deformPtr->m_mesh->m_abVertexArray.GetVertexX(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[1][2]*deformPtr->m_mesh->m_abVertexArray.GetVertexY(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[2][2]*deformPtr->m_mesh->m_abVertexArray.GetVertexZ(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[3][2]);
						    
							posX = deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x;//(boneMatrix.m[0][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][0]*deformPtr->m_mesh->m_abVertexArray.GetVertexY(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[2][0]*deformPtr->m_mesh->m_abVertexArray.GetVertexZ(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[3][0]);
                            posY = deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y;//(boneMatrix.m[0][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][1]*deformPtr->m_mesh->m_abVertexArray.GetVertexY(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[2][1]*deformPtr->m_mesh->m_abVertexArray.GetVertexZ(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[3][1]);
                            posZ = deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z;//(boneMatrix.m[0][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][2]*deformPtr->m_mesh->m_abVertexArray.GetVertexY(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[2][2]*deformPtr->m_mesh->m_abVertexArray.GetVertexZ(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[3][2]);
					        
							//normal
                            normX = (boneMatrix.m[0][0]*deformPtr->m_mesh->m_abVertexArray.GetVertexNX(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[1][0]*deformPtr->m_mesh->m_abVertexArray.GetVertexNY(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[2][0]*deformPtr->m_mesh->m_abVertexArray.GetVertexNZ(deformPtr->m_vertexAssignments[x].vertexIndex));
                            normY = (boneMatrix.m[0][1]*deformPtr->m_mesh->m_abVertexArray.GetVertexNX(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[1][1]*deformPtr->m_mesh->m_abVertexArray.GetVertexNY(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[2][1]*deformPtr->m_mesh->m_abVertexArray.GetVertexNZ(deformPtr->m_vertexAssignments[x].vertexIndex));
                            normZ = (boneMatrix.m[0][2]*deformPtr->m_mesh->m_abVertexArray.GetVertexNX(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[1][2]*deformPtr->m_mesh->m_abVertexArray.GetVertexNY(deformPtr->m_vertexAssignments[x].vertexIndex))+(boneMatrix.m[2][2]*deformPtr->m_mesh->m_abVertexArray.GetVertexNZ(deformPtr->m_vertexAssignments[x].vertexIndex));


                            int tempInt = deformPtr->m_vertexAssignments[x].vertexIndex;

							deformPtr->m_mesh->m_abVertexArray.SetPositionXYZNXNYNZ(tempInt,
								                                                    posX,
							                                                        posY,
																			        posZ,
																					normX,
																					normY,
																					normZ);
 							

							 for(int ownedLoop=0;ownedLoop<deformPtr->m_vertexAssignments[x].carriedCount;ownedLoop++)
							 {
							   //deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop]].x = deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexIndex].x;
							   //deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop]].y = deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexIndex].y;
							   //deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop]].z = deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexIndex].z;
							   tempInt = deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop];
							   
							   deformPtr->m_mesh->m_abVertexArray.SetPositionXYZNXNYNZ(tempInt,
								                                                       posX,
							                                                           posY,
																			           posZ,
																					   normX,
																					   normY,
																					   normZ);
							 }
						 }//end found bone
					 }//end as
					 //deformPtr->m_mesh->m_abVertexArray.Fill0UVArrayData(deformPtr->m_originalArrayVector);
				  }//end 1
			  }//end def list loop
			}//end cfgs loop
		   //}//end def loop
		   tracer++;
		}//end bone loop


		if(m_alternateConfigurations)
            for(POSITION alPos = m_alternateConfigurations->GetHeadPosition();alPos != NULL;)
			{//cfgs loop
			  CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetNext(alPos);
              
			  if(altPtr->m_configurations)
			  for(POSITION defListPos = altPtr->m_configurations->GetHeadPosition();defListPos != NULL;)
			  {//def list loop
			   CDeformableMesh * dMeshHolder = (CDeformableMesh *)altPtr->m_configurations->GetNext(defListPos);
			   
			   if(dMeshHolder->m_lodChain)
			   for(POSITION posLod = dMeshHolder->m_lodChain->GetHeadPosition();posLod != NULL;)
			   {//1
			      CDeformableVisObj * deformPtr = (CDeformableVisObj*)dMeshHolder->m_lodChain->GetNext(posLod);
				     
				  deformPtr->m_mesh->m_abVertexArray.Fill0UVArrayData(deformPtr->m_originalArrayVector);
			   }//end 1
			  }//end def list loop
			}//end cfgs loop





   return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------load animation--------------------------------//
BOOL CSkeletonObject::DeleteAnimationByIndex(int index)
{
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		  if(bonePtr->m_animationList){
			   POSITION animLast;
		       int indexTrace = 0;
			   for(POSITION animPos = bonePtr->m_animationList->GetHeadPosition();(animLast = animPos) != NULL;)
			   {//
				   CBoneAnimationObject * animationObject = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
				   if(indexTrace == index){//found type
		              CBoneAnimationObject * animationDel = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(animLast);
				      animationDel->SafeDelete();
					  delete animationDel;
					  animationDel=0;
				      bonePtr->m_animationList->RemoveAt(animLast);
				   }//end found type
				   indexTrace++;
			   }
		  }
	}	
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------load animation--------------------------------//
BOOL CSkeletonObject::DeleteAnimationByType(int type)
{
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		  if(bonePtr->m_animationList){
			   POSITION animLast;
		       for(POSITION animPos = bonePtr->m_animationList->GetHeadPosition();(animLast = animPos) != NULL;)
			   {//
				   CBoneAnimationObject * animationObject = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
				   if(animationObject->m_type == type){//found type
		              CBoneAnimationObject * animationDel = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(animLast);
				      animationDel->SafeDelete();
					  delete animationDel;
					  animationDel=0;
				      bonePtr->m_animationList->RemoveAt(animLast);
				   }//end found type
			   }
		  }
	}	
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------load animation--------------------------------//
BOOL CSkeletonObject::LoadAnimation(CString fileName,
									BOOL looping,
									int type,
									int duration,
									int replaceIndex)
{
    CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 //this->
	 CSkeletonObject * ghostSkeletalPtr = new CSkeletonObject();
     int physiqueModCount = 0;
	 int firstFrame = 0;
	 int lastFrame = 0;
	 int frameCount = 0;
	 
	 the_in_Archive >> ghostSkeletalPtr->m_SkeletalName
		            >> ghostSkeletalPtr->m_frameSpeed
		            >> ghostSkeletalPtr->m_ticksPerFrame
		            >> ghostSkeletalPtr->m_bonesList
	          	    >> ghostSkeletalPtr->m_deformableMeshArrayCount
		            >> physiqueModCount
	                >> firstFrame
                    >> lastFrame
	                >> frameCount;

       if(ghostSkeletalPtr->m_deformableMeshArrayCount > 0){
	      ghostSkeletalPtr->m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
	      for(int loop=0;loop<ghostSkeletalPtr->m_deformableMeshArrayCount;loop++){
              the_in_Archive >> ghostSkeletalPtr->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
		  }
	   }
	 
	 the_in_Archive.Close();
     fl.Close();
     //apply animation data
	 for(POSITION posLoc = ghostSkeletalPtr->m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) ghostSkeletalPtr->m_bonesList->GetNext(posLoc);
		CBoneObject * boneOnSkeletal = NULL;
		  if(GetBoneByName(bonePtr->m_boneName,&boneOnSkeletal)==TRUE){//found bone
			 POSITION animPos = bonePtr->m_animationList->FindIndex(0);
			 if(animPos){//1
		        CBoneAnimationObject * animationObject = (CBoneAnimationObject *)bonePtr->m_animationList->GetAt(animPos);
                CBoneAnimationObject * animObjectClone = NULL; 
				animationObject->CloneRaw(&animObjectClone);
				animObjectClone->m_looping                 = looping;
		        animObjectClone->m_minPlayDuration         = duration;
				animObjectClone->m_millisecondsPerFrame    = ghostSkeletalPtr->m_ticksPerFrame;

                animObjectClone->m_millisecondsPerFrame = 1000/10;//default to 10

	            animObjectClone->m_totalFrames             = lastFrame;
				animObjectClone->m_type                    = type;
				if(!boneOnSkeletal->m_animationList)
					boneOnSkeletal->m_animationList = new CBoneAnimationList;
				    
				
				
				    if(replaceIndex < 0){//add tail
						//boneOnSkeletal->m_animationList->GetB
					   int version = 0;
					   CBoneAnimationObject * tempObj = boneOnSkeletal->m_animationList->GetByType(type);
					   if(tempObj)
					      version = tempObj->m_version + 1;
				      
						   
						   boneOnSkeletal->m_animationList->AddTail(animObjectClone);
						   animObjectClone->m_version = version;
                           

				    }
					else{
                       POSITION animToReplacePos = boneOnSkeletal->m_animationList->FindIndex(replaceIndex);
                       if(animToReplacePos)
					   {
                          CBoneAnimationObject * animationObjectExist = (CBoneAnimationObject *)boneOnSkeletal->m_animationList->GetAt(animToReplacePos);
						  animationObjectExist->SafeDelete();
						  delete animationObjectExist;
						  animationObjectExist = 0;
					    
						  boneOnSkeletal->m_animationList->InsertAfter(animToReplacePos,animObjectClone);
						  boneOnSkeletal->m_animationList->RemoveAt(animToReplacePos);
					   
					   }
					}
			 }//end 1
		  }//end found bone
	 }
	 ghostSkeletalPtr->SafeDelete();
	 delete ghostSkeletalPtr;
	 ghostSkeletalPtr=0;
	
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------load animation--------------------------------//
BOOL CSkeletonObject::LoadLod(CString fileName,int configIndex)
{
    CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE || configIndex < 0){
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 //this->
	 CSkeletonObject * ghostSkeletalPtr = new CSkeletonObject();
     int physiqueModCount = 0;
	 int firstFrame = 0;
	 int lastFrame = 0;
	 int frameCount = 0;
	 
	 the_in_Archive >> ghostSkeletalPtr->m_SkeletalName
		            >> ghostSkeletalPtr->m_frameSpeed
		            >> ghostSkeletalPtr->m_ticksPerFrame
		            >> ghostSkeletalPtr->m_bonesList
	          	    >> ghostSkeletalPtr->m_deformableMeshArrayCount
		            >> physiqueModCount
	                >> firstFrame
                    >> lastFrame
	                >> frameCount;

          if(ghostSkeletalPtr->m_deformableMeshArrayCount > 0)
		  {
		   int traceLoop=0;
           for(POSITION posLoc = m_alternateConfigurations->GetHeadPosition();posLoc != NULL;)
		   {
             CAlterUnitDBObject * aPtr = (CAlterUnitDBObject *)m_alternateConfigurations->GetNext(posLoc);
             
			 POSITION cfgPos = aPtr->m_configurations->FindIndex(configIndex);
	         if(cfgPos){
				 CDeformableMesh * dMesh = (CDeformableMesh *)aPtr->m_configurations->GetAt(cfgPos);
                
	                ghostSkeletalPtr->m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
	      
		  
		            //for(int loop=0;loop<ghostSkeletalPtr->m_deformableMeshArrayCount;loop++)
					//{
                        
                        the_in_Archive >> ghostSkeletalPtr->m_deformableMeshArrayV2[traceLoop].m_defMeshVisualUnit;

						dMesh->m_lodChain->AddTail(ghostSkeletalPtr->m_deformableMeshArrayV2[traceLoop].m_defMeshVisualUnit->m_lodChain->GetHead());
						ghostSkeletalPtr->m_deformableMeshArrayV2[traceLoop].m_defMeshVisualUnit->m_lodChain->RemoveAll();
						delete ghostSkeletalPtr->m_deformableMeshArrayV2[traceLoop].m_defMeshVisualUnit->m_lodChain;
						ghostSkeletalPtr->m_deformableMeshArrayV2[traceLoop].m_defMeshVisualUnit->m_lodChain = 0;
						traceLoop++;
					//}
			 }
		   }
		  }
		  if(ghostSkeletalPtr->m_deformableMeshArrayV2)
		  {
             delete [] ghostSkeletalPtr->m_deformableMeshArrayV2;
             ghostSkeletalPtr->m_deformableMeshArrayV2 = 0;
		  }
	   //parse hiarch  ghostSkeletalPtr->m_bonesList


	   //

	   ghostSkeletalPtr->SafeDelete();
	   delete ghostSkeletalPtr;
	   ghostSkeletalPtr = 0;
	 
	 the_in_Archive.Close();
     fl.Close();
	
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::AddBone(CString       boneName,
							  CString       parentName,
							  D3DVECTOR     dirVect,
							  D3DVECTOR     upVect,
							  D3DVECTOR     position,
							  D3DVECTOR     scale,
							  CBoneObject ** boneNodeReturn)
{
     CBoneObject * bone = NULL;
     bone = new CBoneObject();
     bone->m_boneFrame = new CFrameObj(NULL);
     bone->m_boneName = boneName;
	 bone->m_parentName = parentName;
	 
	 bone->m_boneFrame->SetOrientation(dirVect,upVect);
     bone->m_boneFrame->SetPosition(NULL,position);
	 bone->m_boneFrame->GetTransform(NULL,&bone->m_startPosition);

	 m_bonesList->AddTail(bone);
	 
	 *boneNodeReturn = bone;
	 return TRUE;
  
return FALSE;
}
//--------------------GetDimCountOnIndex----------------------------//
int CSkeletonObject::GetDimCount(int index)
{
	if(m_alternateConfigurations)
	{
	   POSITION alPos = m_alternateConfigurations->FindIndex(index);
       if(!alPos)
		   return 0;
		   CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
           if(altPtr->m_configurations)
              return (int)altPtr->m_configurations->GetCount();
	}
    return 0;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Hot swap--------------------------------------//
BOOL CSkeletonObject::HotSwapDim(int deformableIndex,
								 int newCfg,
								 CSkeletonObject * targetRuntimeEntity,
								 LPDIRECT3DDEVICE9 g_pd3dDevice,
								 BOOL changeBaseCfg)
{
   //return FALSE;//REMOVE

   if(m_deformableMeshArrayCount <= 0)
	  return FALSE;

   if(newCfg < 0)
      return FALSE;

   if(deformableIndex < 0)
      return FALSE;
      
		 
			  POSITION alPos = m_alternateConfigurations->FindIndex(deformableIndex);
			  if(!alPos)
				  return FALSE;
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  
				  
				  POSITION curCfgPos = altPtr->m_configurations->FindIndex(newCfg);
			      if(!curCfgPos)
					  return FALSE;
				      if(changeBaseCfg)
				         targetRuntimeEntity->m_spawnCfg[deformableIndex] = newCfg;
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
                      if(targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit)
					  {
						 targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit->SafeDelete();
                         delete targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit;
						 targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit = 0;
					  }
			          dMesh->Clone(&targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit,NULL);
      return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::GetBoneByName(CString       boneName,
							        CBoneObject ** boneObject)
{
	for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		if(bonePtr->m_boneName.CompareNoCase(boneName)==0){
           *boneObject = bonePtr;
		   return TRUE;
		}
	}
return FALSE;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
int CSkeletonObject::GetBoneIndexByName(CString & boneName)
{
	int trace = 0;
	for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		if(bonePtr->m_boneName.CompareNoCase(boneName)==0){
		   return trace;
		}
		trace++;
	}
return 0;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::GetBoneMatrixByName(CString & boneName,D3DMATRIX & outPut)
{
	for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		if(bonePtr->m_boneName.CompareNoCase(boneName)==0)
		{
           outPut = bonePtr->m_boneFrame->List_frameMatrix;
		   return TRUE;
		}
	}
return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
CBoneObject * CSkeletonObject::GetBoneByIndex(int & index)
{
	int trace = 0;
	for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		if(trace == index)
		{
		   return bonePtr;
		}
		trace++;
	}
return NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::GetBoneMatrixByIndex(int & index,D3DMATRIX & outPut)
{
	int trace = 0;
	for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		if(trace == index)
		{
		   outPut = bonePtr->m_boneFrame->List_frameMatrix;
		   return TRUE;
		}
		trace++;
	}
return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::QuerryBox()
{
    int indexTrace = 0;
	if(m_bonesList)
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		   D3DVECTOR vector;
		   bonePtr->m_boneFrame->GetPosition(NULL,&vector);
		   if(indexTrace == 0){ //start
              m_box.maxX = vector.x;
			  m_box.maxY = vector.y;
			  m_box.maxZ = vector.z;
              m_box.minX = vector.x;
			  m_box.minY = vector.y;
			  m_box.minZ = vector.z;
		   }//end start
           else{
              if(m_box.maxX < vector.x)
				 m_box.maxX = vector.x;
              if(m_box.maxY < vector.y)
				 m_box.maxY = vector.y;
			  if(m_box.maxZ < vector.z)
				 m_box.maxZ = vector.z;
			  
              if(m_box.minX > vector.x)
				 m_box.minX = vector.x;
              if(m_box.minY > vector.y)
				 m_box.minY = vector.y;
			  if(m_box.minZ > vector.z)
				 m_box.minZ = vector.z;
		   }
		indexTrace++;
	}




return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
ABBOX CSkeletonObject::QuerryBoxExact(D3DMATRIX * compoundMatrix)
{
	ABBOX finalBox;
	ZeroMemory(&finalBox,sizeof(ABBOX));
    int indexTrace = 0;
	D3DVECTOR temp;
	if(m_bonesList)
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
	{
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		
		D3DVECTOR vector;
		bonePtr->m_boneFrame->GetPosition(NULL,&vector);
		if(compoundMatrix)
		  vector = GL_matrixFunctions.UpdatePolygonVertex(compoundMatrix->m,vector);
		  
		   if(indexTrace == 0){ //start
              finalBox.maxX = vector.x;
			  finalBox.maxY = vector.y;
			  finalBox.maxZ = vector.z;
              finalBox.minX = vector.x;
			  finalBox.minY = vector.y;
			  finalBox.minZ = vector.z;
		   }//end start
		   indexTrace++;
		
		
		
		
		  if(finalBox.maxX < vector.x)
			 finalBox.maxX = vector.x;
		  if(finalBox.maxY < vector.y)
			 finalBox.maxY = vector.y;
		  if(finalBox.maxZ < vector.z)
			 finalBox.maxZ = vector.z;
		  
		  if(finalBox.minX > vector.x)
			 finalBox.minX = vector.x;
		  if(finalBox.minY > vector.y)
			 finalBox.minY = vector.y;
		  if(finalBox.minZ > vector.z)
			 finalBox.minZ = vector.z;

        if(bonePtr->m_hiarchyMesh)
		{//only deal with visuals
           CHiarcleVisObj * hVis = bonePtr->m_hiarchyMesh->m_lodChain->GetByIndex(0);
           if(hVis)
		   {//found visual
		      ABVERTEX *vArray = NULL;
              int vCount = 0;
	          hVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);
              for(int vi=0;vi<vCount;vi++)
			  {//vertex loop
				  
                  temp.x = vArray[vi].x;
                  temp.y = vArray[vi].y;
				  temp.z = vArray[vi].z;
                  temp = GL_matrixFunctions.UpdatePolygonVertex(bonePtr->m_boneFrame->List_frameMatrix.m,temp);
				  if(compoundMatrix)
				     temp = GL_matrixFunctions.UpdatePolygonVertex(compoundMatrix->m,temp);

				  if(temp.x > fabs(finalBox.maxX))
				     finalBox.maxX = temp.x;

				  if(temp.x > finalBox.maxX)
				     finalBox.maxX = temp.x;

				  if(temp.y > finalBox.maxY)
				     finalBox.maxY = temp.y;

				  if(temp.z > finalBox.maxZ)
				     finalBox.maxZ = temp.z;

				  if(temp.x < finalBox.minX)
				     finalBox.minX = temp.x;

				  if(temp.y < finalBox.minY)
				     finalBox.minY = temp.y;

				  if(temp.z < finalBox.minZ)
				     finalBox.minZ = temp.z;

			  }//end vertex loop
			  delete [] vArray;
              vArray = 0;
		   }//end found visual
		}//end only deal with visuals
	}
	//deformable mesh
	for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
	{//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos)
				{//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
                          
						  ABVERTEX * vArray = NULL;
						  int count = 0;
						  dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&count);
						  for(int vi=0;vi<count;vi++)
						  {//vertex loop
							  
							  temp.x = vArray[vi].x;
							  temp.y = vArray[vi].y;
							  temp.z = vArray[vi].z;
							  if(compoundMatrix)
								 temp = GL_matrixFunctions.UpdatePolygonVertex(compoundMatrix->m,temp);

							  if(temp.x > fabs(finalBox.maxX))
								 finalBox.maxX = temp.x;

							  if(temp.x > finalBox.maxX)
								 finalBox.maxX = temp.x;

							  if(temp.y > finalBox.maxY)
								 finalBox.maxY = temp.y;

							  if(temp.z > finalBox.maxZ)
								 finalBox.maxZ = temp.z;

							  if(temp.x < finalBox.minX)
								 finalBox.minX = temp.x;

							  if(temp.y < finalBox.minY)
								 finalBox.minY = temp.y;

							  if(temp.z < finalBox.minZ)
								 finalBox.minZ = temp.z;

						  }//end vertex loop
						  delete [] vArray;
              vArray = 0;
					  }
				  }
				}
			  }
	}


//CopyMemory(&m_box,&finalBox,sizeof(ABBOX));

return finalBox;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::BuildHiarchy()
{
     for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * frmPtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
        for(POSITION posLoc2 = m_bonesList->GetHeadPosition();posLoc2 != NULL;){
        CBoneObject * bonesPtr = (CBoneObject *)m_bonesList->GetNext(posLoc2);
          if(frmPtr->m_parentName == bonesPtr->m_boneName){
	         frmPtr->m_boneFrame->SetParent(bonesPtr->m_boneFrame);
			 //AfxMessageBox(operator+(frmPtr->m_boneName,operator+("  parent::",bonesPtr->m_boneName)));
		  }
		}
	 }
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------secondary animation---------------------------//
void CSkeletonObject::ApplyFreeLookRotation(float pitch,float yaw)
{
    //if(m_freeLookBone == -1)
	   //return;
    if(!m_bonesList)
       return;

    m_freeLookPitch = pitch;
	m_freeLookYaw = yaw;
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void CSkeletonObject::SkeletalTextureReassignment(CTextureDBObjList * textureDatabase,
												  CTextureDBObjList * m_dynamicTextureDataBase)
{
   //hiarch bone objects
   for(POSITION posLocal = m_bonesList->GetHeadPosition();posLocal != NULL;){
	   CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLocal);
	   if(bonePtr->m_hiarchyMesh){
          for(POSITION posOni = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();posOni != NULL;)
		  {
             CHiarcleVisObj * hObj = (CHiarcleVisObj *) bonePtr->m_hiarchyMesh->m_lodChain->GetNext(posOni);
		     //ReinitTextureUsage(hObj->m_mesh);
			 
			 textureDatabase->AddTextureToDatabase(hObj->m_mesh->List_storedTextureFromFile,//CString * storedTextures,
											       hObj->m_mesh->List_textureSelect,//int * assignmentArray,
											       m_dynamicTextureDataBase);
		  }
	   }
   } 
   //spawn deformable cfg
   if(m_alternateConfigurations)
   for(POSITION posLoc = m_alternateConfigurations->GetHeadPosition();posLoc != NULL;)
   {
       CAlterUnitDBObject * aPtr = (CAlterUnitDBObject *)m_alternateConfigurations->GetNext(posLoc);
       for(POSITION cfgPos = aPtr->m_configurations->GetHeadPosition();cfgPos != NULL;)
	   {
           CDeformableMesh * dMesh = (CDeformableMesh *)aPtr->m_configurations->GetNext(cfgPos);
		   for(POSITION lPos = dMesh->m_lodChain->GetHeadPosition();lPos != NULL;)
		   {
	           CDeformableVisObj * vObj = (CDeformableVisObj *) dMesh->m_lodChain->GetNext(lPos);
			   //ReinitTextureUsage(vObj->m_mesh);
			   textureDatabase->AddTextureToDatabase(vObj->m_mesh->List_storedTextureFromFile,//CString * storedTextures,
											         vObj->m_mesh->List_textureSelect,//int * assignmentArray,
											         m_dynamicTextureDataBase);
		   }
	   }
   }
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
void CSkeletonObject::SkeletalTextureReassignmentTemp(int tempChannel,
													  CTextureDBObjList * textureDatabase)
{
   //hiarch bone objects
   for(POSITION posLocal = m_bonesList->GetHeadPosition();posLocal != NULL;){
	   CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLocal);
	   if(bonePtr->m_hiarchyMesh){
          for(POSITION posOni = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();posOni != NULL;)
		  {
             CHiarcleVisObj * hObj = (CHiarcleVisObj *) bonePtr->m_hiarchyMesh->m_lodChain->GetNext(posOni);
		     textureDatabase->AddTextureToDatabaseTemp(hObj->m_mesh->List_storedTextureFromFile,//CString * storedTextures,
											           hObj->m_mesh->List_textureSelect,
													   tempChannel);
		  }
	   }
   } 
   //spawn deformable cfg
   if(m_alternateConfigurations)
   for(POSITION posLoc = m_alternateConfigurations->GetHeadPosition();posLoc != NULL;)
   {
       CAlterUnitDBObject * aPtr = (CAlterUnitDBObject *)m_alternateConfigurations->GetNext(posLoc);
       for(POSITION cfgPos = aPtr->m_configurations->GetHeadPosition();cfgPos != NULL;)
	   {
           CDeformableMesh * dMesh = (CDeformableMesh *)aPtr->m_configurations->GetNext(cfgPos);
		   for(POSITION lPos = dMesh->m_lodChain->GetHeadPosition();lPos != NULL;)
		   {
	           CDeformableVisObj * vObj = (CDeformableVisObj *) dMesh->m_lodChain->GetNext(lPos);
			   textureDatabase->AddTextureToDatabaseTemp(vObj->m_mesh->List_storedTextureFromFile,//CString * storedTextures,
											             vObj->m_mesh->List_textureSelect,
													     tempChannel);
		   }
	   }
   }
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Generate tangents-----------------------------//
void CSkeletonObject::GenerateTangents(LPDIRECT3DDEVICE9 g_pd3dDevice)
{  
	if(this->m_bonesList)
	   this->m_bonesList->CompileDirectAccess();
     if(m_alternateConfigurations)
     for(POSITION posLoc = m_alternateConfigurations->GetHeadPosition();posLoc != NULL;)
     {
       CAlterUnitDBObject * aPtr = (CAlterUnitDBObject *)m_alternateConfigurations->GetNext(posLoc);
       for(POSITION cfgPos = aPtr->m_configurations->GetHeadPosition();cfgPos != NULL;)
	   {
           CDeformableMesh * dMesh = (CDeformableMesh *)aPtr->m_configurations->GetNext(cfgPos);
		   for(POSITION lPos = dMesh->m_lodChain->GetHeadPosition();lPos != NULL;)
		   {
	           CDeformableVisObj * vObj = (CDeformableVisObj *) dMesh->m_lodChain->GetNext(lPos);
			   
			   
			   GenerateTangents(g_pd3dDevice,vObj);
		   }
	   }
     }
	 for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
     {
         CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
         if(bonePtr->m_hiarchyMesh)
		  for(POSITION hPos = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
		  {
			  CHiarcleVisObj * hVisObj = (CHiarcleVisObj *)bonePtr->m_hiarchyMesh->m_lodChain->GetNext(hPos);  
			  hVisObj->m_mesh->m_abVertexArray.GenerateTangentsPerfect(g_pd3dDevice,FALSE,0);
		  }
	 }
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Generate tangents-----------------------------//
void CSkeletonObject::GenerateTangents(LPDIRECT3DDEVICE9 g_pd3dDevice,CDeformableVisObj * deformPtr)
{      
       //D3DMATRIX boneMatrix;
	   int tempInt = 0;
	   D3DVECTOR finalPosition,normalPosition,tangentPosition;
	   if(deformPtr->m_mesh)
	   if(deformPtr->m_mesh->m_abVertexArray.m_uvCount > 9)
	      return;
	  		
	                 int x=0;
				     for(x=0;x<deformPtr->m_assignmentCount;x++)
					 {//blended formation loop
						 ZeroMemory(&finalPosition,sizeof(D3DVECTOR));
						 ZeroMemory(&normalPosition,sizeof(D3DVECTOR));
						  if(deformPtr->m_blendedType == 1)
						  {
							 for(int xBonesLoop=0;xBonesLoop<deformPtr->m_blendedVertexAssignments[x].boneIndexesCount;xBonesLoop++)
	                         {//vertex/bone blend calc
								 if(deformPtr->m_blendedVertexAssignments[x].boneIndexes[xBonesLoop] >= m_bonesList->m_directAccessArrayCount)
						            continue;
                                 CBoneObject * bonePtr = m_bonesList->m_directAccessArray[deformPtr->m_blendedVertexAssignments[x].boneIndexes[xBonesLoop]].object;	
								 if(bonePtr)
		                         {//valid bone
 
									 finalPosition.x += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][0]))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
                                     finalPosition.y += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][1]))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
                                     finalPosition.z += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].x)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].y)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].z)+(bonePtr->m_boneFrame->List_frameMatrix.m[3][2]))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
									 //normal
									 normalPosition.x += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][0]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nz))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
                                     normalPosition.y += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][1]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nz))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
                                     normalPosition.z += ((bonePtr->m_boneFrame->List_frameMatrix.m[0][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nx)+(bonePtr->m_boneFrame->List_frameMatrix.m[1][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].ny)+(bonePtr->m_boneFrame->List_frameMatrix.m[2][2]*deformPtr->m_blendedVertexAssignments[x].offsetVects[xBonesLoop].nz))*deformPtr->m_blendedVertexAssignments[x].weightsToBone[xBonesLoop];
								 }//end valid bone
	                        }//end vertex/bone blend calc
                         
							 tempInt = deformPtr->m_blendedVertexAssignments[x].vertexIndex;

							 deformPtr->m_mesh->m_abVertexArray.SetPositionXYZNXNYNZ(tempInt,
								                                                    finalPosition.x,
							                                                        finalPosition.y,
																			        finalPosition.z,
																					normalPosition.x,
																					normalPosition.y,
																					normalPosition.z);

							 for(int ownedLoop=0;ownedLoop<deformPtr->m_blendedVertexAssignments[x].carriedCount;ownedLoop++)
							 {
							   tempInt = deformPtr->m_blendedVertexAssignments[x].vertexesCarried[ownedLoop];
							   deformPtr->m_mesh->m_abVertexArray.SetPositionXYZNXNYNZ(tempInt,
								                                                       finalPosition.x,
							                                                           finalPosition.y,
																			           finalPosition.z,
																					   normalPosition.x,
																					   normalPosition.y,
																					   normalPosition.z);
							 }
						 
						  }
						 
					 }//end blended formation loop

					 //convert to tangent
                         deformPtr->m_mesh->m_abVertexArray.GenerateTangentsPerfect(g_pd3dDevice,FALSE,0);

					 //get tangents and invert then save to new data structures
					 deformPtr->m_blendedType = 2;
					 deformPtr->m_blendedVertexAssignmentsWt = new VertexAssignmentBldTgt[deformPtr->m_assignmentCount];
					 ZeroMemory(deformPtr->m_blendedVertexAssignmentsWt,sizeof(VertexAssignmentBldTgt)*deformPtr->m_assignmentCount);
                     for(x=0;x<deformPtr->m_assignmentCount;x++)
					 {//transform loop
                        deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexesCount = deformPtr->m_blendedVertexAssignments[x].boneIndexesCount;
						deformPtr->m_blendedVertexAssignmentsWt[x].carriedCount = deformPtr->m_blendedVertexAssignments[x].carriedCount;
						deformPtr->m_blendedVertexAssignmentsWt[x].vertexIndex = deformPtr->m_blendedVertexAssignments[x].vertexIndex;

						if(deformPtr->m_blendedVertexAssignmentsWt[x].carriedCount > 0)
						{
						deformPtr->m_blendedVertexAssignmentsWt[x].vertexesCarried = new short[deformPtr->m_blendedVertexAssignmentsWt[x].carriedCount];
						CopyMemory(deformPtr->m_blendedVertexAssignmentsWt[x].vertexesCarried,deformPtr->m_blendedVertexAssignments[x].vertexesCarried,sizeof(short)*deformPtr->m_blendedVertexAssignments[x].carriedCount);
						}
						
						if(deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexesCount > 0)
						{
						deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexes = new byte[deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexesCount];
						deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects = new ABVERTEX0Lb[deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexesCount];
						deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone = new float[deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexesCount];
						CopyMemory(deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexes,deformPtr->m_blendedVertexAssignments[x].boneIndexes,deformPtr->m_blendedVertexAssignments[x].boneIndexesCount);
						CopyMemory(deformPtr->m_blendedVertexAssignmentsWt[x].weightsToBone,deformPtr->m_blendedVertexAssignments[x].weightsToBone,sizeof(float)*deformPtr->m_blendedVertexAssignments[x].boneIndexesCount);
						}

						for(int vL=0;vL<deformPtr->m_blendedVertexAssignmentsWt[x].boneIndexesCount;vL++)
						{
                            deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[vL].x = deformPtr->m_blendedVertexAssignments[x].offsetVects[vL].x;
							deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[vL].y = deformPtr->m_blendedVertexAssignments[x].offsetVects[vL].y;
							deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[vL].z = deformPtr->m_blendedVertexAssignments[x].offsetVects[vL].z;

							deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[vL].nx = deformPtr->m_blendedVertexAssignments[x].offsetVects[vL].nx;
							deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[vL].ny = deformPtr->m_blendedVertexAssignments[x].offsetVects[vL].ny;
							deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[vL].nz = deformPtr->m_blendedVertexAssignments[x].offsetVects[vL].nz;
						}
					 }//end transform loop

                     for(x=0;x<deformPtr->m_assignmentCount;x++)
					 {//blended formation loop
						 ZeroMemory(&finalPosition,sizeof(D3DVECTOR));
						 ZeroMemory(&normalPosition,sizeof(D3DVECTOR));
                         ZeroMemory(&tangentPosition,sizeof(D3DVECTOR));
						 
						  if(deformPtr->m_blendedType == 2)
						  {
							 for(int xBonesLoop=0;xBonesLoop<deformPtr->m_blendedVertexAssignments[x].boneIndexesCount;xBonesLoop++)
	                         {//vertex/bone blend calc
								 if(deformPtr->m_blendedVertexAssignments[x].boneIndexes[xBonesLoop] >= m_bonesList->m_directAccessArrayCount)
						            continue;
                                 CBoneObject * bonePtr = m_bonesList->m_directAccessArray[deformPtr->m_blendedVertexAssignments[x].boneIndexes[xBonesLoop]].object;	
								 D3DMATRIX boneMatrix;
								 bonePtr->m_boneFrame->GetInverseTransform(&boneMatrix);
								 
								 if(bonePtr)
		                         {//valid bone
                                    
		                             D3DVECTOR inPosTangent = deformPtr->m_mesh->m_abVertexArray.GetTangentVector(deformPtr->m_blendedVertexAssignments[x].vertexIndex);
		
			                         deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tX = (boneMatrix.m[0][0]*inPosTangent.x)+(boneMatrix.m[1][0]*inPosTangent.y)+(boneMatrix.m[2][0]*inPosTangent.z);
                                     deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tY = (boneMatrix.m[0][1]*inPosTangent.x)+(boneMatrix.m[1][1]*inPosTangent.y)+(boneMatrix.m[2][1]*inPosTangent.z);
                                     deformPtr->m_blendedVertexAssignmentsWt[x].offsetVects[xBonesLoop].tZ = (boneMatrix.m[0][2]*inPosTangent.x)+(boneMatrix.m[1][2]*inPosTangent.y)+(boneMatrix.m[2][2]*inPosTangent.z);

								 }//end valid bone
	                        }//end vertex/bone blend calc  
						  }
					 }//end blended formation loop
					 delete [] deformPtr->m_blendedVertexAssignments;
					 deformPtr->m_blendedVertexAssignments = NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
CAlterUnitDBObject::CAlterUnitDBObject()
{
   m_alterUnitName = "";
   m_configurations = NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CAlterUnitDBObject::SafeDelete()
{
   if(m_configurations)
   {
	  m_configurations->SafeDelete();
      delete m_configurations;
      m_configurations = 0;
   }
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CAlterUnitDBObject::Serialize( CArchive& ar )
{
 CObject::Serialize( ar );
	if(ar.IsStoring() ){	
       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);

	   ar << m_configurations;

	   ar << m_alterUnitName;
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_configurations;

	   if(version > 1)
	   {
          ar >> m_alterUnitName;
	   }
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CAlterUnitDBObjectList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
        CAlterUnitDBObject * dPtr = (CAlterUnitDBObject *)GetNext(posLoc);
		dPtr->SafeDelete();
		delete dPtr;
		dPtr=0;
   }
   RemoveAll();
}
//------------------------------------------------------------------//
CAlterUnitDBObject * CAlterUnitDBObjectList::GetByName(CString name)
{
	name = AltStripper(name);
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CAlterUnitDBObject * dPtr = (CAlterUnitDBObject *)GetNext(posLoc);
		if(AltStripper(dPtr->m_alterUnitName).CompareNoCase(name)==0)
			return dPtr;
    }
	return NULL;
}
//------------------------------------------------------------------//
CString CAlterUnitDBObjectList::AltStripper(CString & current)
{
    if(current.GetLength() < 4)
		return current;

	if(current.GetAt(0) == 'A')
		if(current.GetAt(1) == 'L')
		    if(current.GetAt(2) == 'T')
		    {
                CString build = "";
				bool record = false;
                for(int i=2;i<current.GetLength();i++)
				{
                    if(current.GetAt(i) == '_')
					{
                    record = true;
					continue;
					}
					if(record)
                    build += current.GetAt(i);
				}
				return build;
		    }


		return current;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::LoadFullSkeletalFromDirectory(CString path,CString directory,LPDIRECT3DDEVICE9 device)
{
	CABFunctionLib cablib;
    WIN32_FIND_DATA FindData;
    HANDLE FileHandle;
    DWORD Attrib;
    CString name;
	
	if(!this->InitBoneSystem(path,device))
		return FALSE;
	this->m_SkeletalName = "New Skeletal";

	CString cfgFileToApply = "";
	//first initialize skeleton List_skeletalObject
    
    FileHandle = FindFirstFile(directory+"*.*", &FindData);
	do{
	  Attrib = FindData.dwFileAttributes;
	  if(Attrib != 16){//if valid file
	  name = FindData.cFileName;
	  CString ext = cablib.GetFileExt(name);
	  name.MakeUpper();

	  if(ext.MakeUpper() == "ALT")
	  {
		if(!this->MergeAsAltCfg2((directory+name),device))
		{
           AfxMessageBox("Failed to load Alt Cfg file onto skeletal object:"+name);
           break;
		}
		else
		   continue;
	  }

	  if(ext.MakeUpper() == "ARB")
	  {
	    CString limited = name.Mid(0,3);

        int type = -1;
	    if(limited == "WAL")//walk
		   type = 3;
		else if(limited == "RUN")//run
		   type = 3;
		else if(limited == "STA")//stand
		   type = 1;
		else if(limited == "DIE")//die
		   type = 8;
		else if(limited == "JUM")//jump
		   type = 5;
		else if(limited == "CRO")//crouch
		   type = 11;
		else if(limited == "FIR")//fire
		   type = 12;
		else if(limited == "PAI")//pain
		   type = 17;
		else if(limited == "JET")//jet
		   type = 16;
		else if(name == "SWIM IDLE.ARB" || name == "SWIM IDLE1.ARB" || name == "SWIM IDLE2.ARB" || name == "SWIM IDLE3.ARB" || name == "SWIM IDLE4.ARB" || name == "SWIM IDLE5.ARB" || name == "SWIM IDLE6.ARB" || name == "SWIM IDLE7.ARB" || name == "SWIM IDLE8.ARB")
	       type = 14;
		else if(name == "SWIM ACTION.ARB" || name == "SWIM ACTION1.ARB" || name == "SWIM ACTION2.ARB" || name == "SWIM ACTION3.ARB" || name == "SWIM ACTION4.ARB" || name == "SWIM ACTION5.ARB" || name == "SWIM ACTION6.ARB" || name == "SWIM ACTION7.ARB" || name == "SWIM ACTION8.ARB")
	       type = 15;
		else if(name == "TAUNT1.ARB")
	       type = 18;
	    else if(name == "TAUNT2.ARB")
	       type = 19;
	    else if(name == "TAUNT3.ARB")
	       type = 20;
	    else if(name == "DANCE1.ARB")
	       type = 21;
	    else if(name == "DANCE2.ARB")
	       type = 22;
	    else if(name == "DANCE3.ARB")
	       type = 23;
	    else if(name == "WAVE.ARB")
	       type = 24;
	    else if(name == "SALUTE.ARB")
	       type = 25;
	    else if(name == "BOW.ARB")
	       type = 26;
	    else if(name == "LAUGH.ARB")
	       type = 27;
		else if(name == "SIT.ARB")
	       type = 28;
		else if(name == "CROUCH.ARB")
	       type = 29;
		else if(name == "MOUNT.ARB")
	       type = 30;
		else if(name == "CAST.ARB")//cast
			type = 31;
		else if(name == "CAST1.ARB")//cast
			type = 32;
		else if(name == "CAST2.ARB")//cast
			type = 33;

		if(type > -1)
	    if(this->LoadAnimation((directory+name),TRUE,type,0,-1)==FALSE)
		{
		   AfxMessageBox("Failed to load animation file onto skeletal object:"+name);
		   break;
		}
	  }

	  }//end if valid file
	} while (FindNextFile(FileHandle, &FindData));



    //do lod directories if they exist
    
    CString lodExt = "LOD";
	int curLOD = 1;
	while(LoadLODFRomDir(directory+lodExt+cablib.ConvToString(curLOD)+"\\",device))
	{
		curLOD++;
	}

		

    return TRUE;
}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
BOOL CSkeletonObject::LoadLODFRomDir(CString directory,LPDIRECT3DDEVICE9 device)
{
   
    CABFunctionLib cablib;
	WIN32_FIND_DATA FindData;
    HANDLE FileHandle;
    DWORD Attrib;
    CString name;
	CString cfgFileToApply = "";
    bool doneArb = false;
	bool validDir = false;
    
    FileHandle = FindFirstFile(directory+"*.*", &FindData);
	do{
	  
	  Attrib = FindData.dwFileAttributes;
	  if(Attrib != 16){//if valid file
	  name = FindData.cFileName;
	  CString ext = cablib.GetFileExt(name);
	  name.MakeUpper();

	  if(ext.MakeUpper() == "ALT")
	  {
	     validDir = true;
	     this->MergeAsLod((directory+name),device);
	  }

	  if(doneArb == false)
	  if(ext.MakeUpper() == "ARB")
	  {
	     validDir = true;
         this->MergeAsLod((directory+name),device);
		 doneArb = true;
	  }

	  }//end if valid file
	} while (FindNextFile(FileHandle, &FindData));

    if(validDir == true)
       return TRUE;
	else
	   return FALSE;

}

BOOL CSkeletonObject::ExportToObj(CString fileName)
{
	CEXMeshObjList exportList;
	auto addUniqueMesh = [&](CEXMeshObj * m) {
		if (!m || m->m_abVertexArray.m_vertexCount <= 0 || m->m_abVertexArray.m_indecieCount <= 0)
			return;
		for (POSITION p = exportList.GetHeadPosition(); p != NULL;)
		{
			if ((CEXMeshObj*)exportList.GetNext(p) == m)
				return;
		}
		exportList.AddTail(m);
	};

	// 1. Collect active/default deformable meshes from m_alternateConfigurations
	if (m_alternateConfigurations)
	{
		int slotIdx = 0;
		for (POSITION altPos = m_alternateConfigurations->GetHeadPosition(); altPos != NULL; ++slotIdx)
		{
			CAlterUnitDBObject *altUnit = (CAlterUnitDBObject*)m_alternateConfigurations->GetNext(altPos);
			if (!altUnit || !altUnit->m_configurations || altUnit->m_configurations->IsEmpty())
				continue;

			int cfgIdx = (m_spawnCfg && slotIdx < m_deformableMeshArrayCount && m_spawnCfg[slotIdx] >= 0)
				? m_spawnCfg[slotIdx] : 0;
			POSITION cfgPos = altUnit->m_configurations->FindIndex(cfgIdx);
			if (!cfgPos)
				cfgPos = altUnit->m_configurations->GetHeadPosition();
			if (!cfgPos)
				continue;

			CDeformableMesh *defMesh = (CDeformableMesh*)altUnit->m_configurations->GetAt(cfgPos);
			if (defMesh && defMesh->m_lodChain && !defMesh->m_lodChain->IsEmpty())
			{
				CDeformableVisObj *visObj = (CDeformableVisObj*)defMesh->m_lodChain->GetHead();
				if (visObj && visObj->m_mesh)
					addUniqueMesh(visObj->m_mesh);
			}
		}
	}

	// 2. Collect deformable meshes from m_deformableMeshArrayV2
	if (m_deformableMeshArrayV2 && m_deformableMeshArrayCount > 0)
	{
		for (int i = 0; i < m_deformableMeshArrayCount; ++i)
		{
			CDeformableMesh *defMesh = m_deformableMeshArrayV2[i].m_defMeshVisualUnit;
			if (defMesh && defMesh->m_lodChain && !defMesh->m_lodChain->IsEmpty())
			{
				CDeformableVisObj *visObj = (CDeformableVisObj*)defMesh->m_lodChain->GetHead();
				if (visObj && visObj->m_mesh)
					addUniqueMesh(visObj->m_mesh);
			}
		}
	}

	// 3. Collect rigid hierarchy meshes attached to bones
	if (m_bonesList)
	{
		for (POSITION bPos = m_bonesList->GetHeadPosition(); bPos != NULL;)
		{
			CBoneObject *boneObj = (CBoneObject*)m_bonesList->GetNext(bPos);
			if (boneObj && boneObj->m_hiarchyMesh && boneObj->m_hiarchyMesh->m_lodChain &&
				!boneObj->m_hiarchyMesh->m_lodChain->IsEmpty())
			{
				CHiarcleVisObj *hVis = (CHiarcleVisObj*)boneObj->m_hiarchyMesh->m_lodChain->GetHead();
				if (hVis && hVis->m_mesh)
					addUniqueMesh(hVis->m_mesh);
			}
		}
	}

	if (exportList.IsEmpty())
		return FALSE;

	BOOL ok = exportList.ExportToObj(fileName);
	exportList.RemoveAll(); // Do not delete owned meshes
	return ok;
}

static void ClassifyBoneForFemaleSculpt(const CString &rawBoneName, bool &outStrictLimbHead, bool &outBoundExclude, bool &outCoreTorso)
{
	CString name = rawBoneName;
	name.MakeUpper();

	// Strict limb/head bones: vertices weighted to these must NEVER be morphed (even if arms hang down next to the torso)
	outStrictLimbHead = (
		name.Find("UPPERARM") != -1 ||
		name.Find("FOREARM")  != -1 ||
		name.Find("HAND")     != -1 ||
		name.Find("FINGER")   != -1 ||
		name.Find("THUMB")    != -1 ||
		name.Find("WRIST")    != -1 ||
		name.Find("HEAD")     != -1 ||
		name.Find("NECK")     != -1 ||
		name.Find("JAW")      != -1 ||
		name.Find("MOUTH")    != -1 ||
		name.Find("EYE")      != -1 ||
		name.Find("EAR")      != -1 ||
		name.Find("PONY")     != -1 ||
		name.Find("HAIR")     != -1 ||
		name.Find("CALF")     != -1 ||
		name.Find("FOOT")     != -1 ||
		name.Find("TOE")      != -1 ||
		name.Find("TAIL")     != -1 ||
		name.Find("STEPS")    != -1 ||
		name.Find("DUMMY")    != -1 ||
		name.Find("BROW")     != -1 ||
		name.Find("TEETH")    != -1 ||
		name.Find("CHEEK")    != -1 ||
		name.Find("TOUNGE")   != -1 ||
		name.Find("TONGUE")   != -1 ||
		name.Find("LIP")      != -1 ||
		name.Find("WEAPON")   != -1 ||
		name.Find("MOUNT")    != -1 ||
		name.Find("JET")      != -1);

	// Additional bones excluded when measuring the core trunk (Pelvis + Spine) bounding box
	outBoundExclude = outStrictLimbHead || (
		name.Find("ARM")      != -1 ||
		name.Find("CLAVICLE") != -1 ||
		name.Find("SHOULDER") != -1 ||
		name.Find("THIGH")    != -1 ||
		name.Find("LEG")      != -1);

	outCoreTorso = !outBoundExclude && (
		name.Find("SPINE")  != -1 ||
		name.Find("PELVIS") != -1 ||
		name.Find("CHEST")  != -1 ||
		name.Find("TORSO")  != -1 ||
		name.Find("WAIST")  != -1 ||
		name.Find("HIP")    != -1 ||
		name.Find("RIB")    != -1 ||
		name.Find("BELLY")  != -1 ||
		name.Find("BUTT")   != -1 ||
		name == "BIP01");
}

BOOL CSkeletonObject::ComputeSkinBounds(float &outMinX, float &outMaxX, float &outMinY, float &outMaxY, float &outMinZ, float &outMaxZ)
{
	if (m_bonesList && !m_bonesList->m_directAccessArray)
		m_bonesList->CompileDirectAccess();

	int boneCount = (m_bonesList && m_bonesList->m_directAccessArray) ? m_bonesList->m_directAccessArrayCount : 0;
	bool *isBoundExclude = (boneCount > 0) ? new bool[boneCount] : NULL;
	bool *isCoreTorso    = (boneCount > 0) ? new bool[boneCount] : NULL;
	for (int b = 0; b < boneCount; b++)
	{
		bool strictLimb = false;
		CBoneObject *bPtr = m_bonesList->m_directAccessArray[b].object;
		ClassifyBoneForFemaleSculpt(bPtr ? bPtr->m_boneName : CString(""), strictLimb, isBoundExclude[b], isCoreTorso[b]);
	}

	bool hasTorso = false;
	float tMinX = 1e30f, tMaxX = -1e30f;
	float tMinY = 1e30f, tMaxY = -1e30f;
	float tMinZ = 1e30f, tMaxZ = -1e30f;

	bool hasFallback = false;
	float fMinX = 1e30f, fMaxX = -1e30f;
	float fMinY = 1e30f, fMaxY = -1e30f;
	float fMinZ = 1e30f, fMaxZ = -1e30f;

	auto addPoint = [&](float px, float py, float pz, float coreW, float exclW) {
		if (exclW < 0.25f)
		{
			if (px < fMinX) fMinX = px; if (px > fMaxX) fMaxX = px;
			if (py < fMinY) fMinY = py; if (py > fMaxY) fMaxY = py;
			if (pz < fMinZ) fMinZ = pz; if (pz > fMaxZ) fMaxZ = pz;
			hasFallback = true;
		}
		if (coreW >= 0.45f && exclW < 0.15f)
		{
			if (px < tMinX) tMinX = px; if (px > tMaxX) tMaxX = px;
			if (py < tMinY) tMinY = py; if (py > tMaxY) tMaxY = py;
			if (pz < tMinZ) tMinZ = pz; if (pz > tMaxZ) tMaxZ = pz;
			hasTorso = true;
		}
	};

	auto accumulateVisObj = [&](CDeformableVisObj *dVis) {
		if (!dVis || !dVis->m_mesh)
			return;
		if (boneCount > 0 && dVis->m_assignmentCount > 0)
		{
			if (dVis->m_blendedType == 0 && dVis->m_vertexAssignments && dVis->m_originalArrayVector)
			{
				for (int x = 0; x < dVis->m_assignmentCount; x++)
				{
					int bIdx = dVis->m_vertexAssignments[x].boneIndex;
					int vIdx = dVis->m_vertexAssignments[x].vertexIndex;
					if (bIdx < 0 || bIdx >= boneCount)
						continue;
					CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
					if (!bPtr)
						continue;
					const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
					const ABVERTEX0L &orig = dVis->m_originalArrayVector[vIdx];
					float px = M.m[0][0] * orig.x + M.m[1][0] * orig.y + M.m[2][0] * orig.z + M.m[3][0];
					float py = M.m[0][1] * orig.x + M.m[1][1] * orig.y + M.m[2][1] * orig.z + M.m[3][1];
					float pz = M.m[0][2] * orig.x + M.m[1][2] * orig.y + M.m[2][2] * orig.z + M.m[3][2];
					addPoint(px, py, pz, isCoreTorso[bIdx] ? 1.0f : 0.0f, isBoundExclude[bIdx] ? 1.0f : 0.0f);
				}
				return;
			}
			else if (dVis->m_blendedType == 1 && dVis->m_blendedVertexAssignments)
			{
				for (int x = 0; x < dVis->m_assignmentCount; x++)
				{
					float px = 0.0f, py = 0.0f, pz = 0.0f, coreW = 0.0f, exclW = 0.0f;
					for (int b = 0; b < dVis->m_blendedVertexAssignments[x].boneIndexesCount; b++)
					{
						int bIdx = dVis->m_blendedVertexAssignments[x].boneIndexes[b];
						float w  = dVis->m_blendedVertexAssignments[x].weightsToBone[b];
						if (bIdx < 0 || bIdx >= boneCount || w == 0.0f)
							continue;
						CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
						if (!bPtr)
							continue;
						const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
						const ABVERTEX0L &off = dVis->m_blendedVertexAssignments[x].offsetVects[b];
						px += (M.m[0][0] * off.x + M.m[1][0] * off.y + M.m[2][0] * off.z + M.m[3][0]) * w;
						py += (M.m[0][1] * off.x + M.m[1][1] * off.y + M.m[2][1] * off.z + M.m[3][1]) * w;
						pz += (M.m[0][2] * off.x + M.m[1][2] * off.y + M.m[2][2] * off.z + M.m[3][2]) * w;
						if (isCoreTorso[bIdx])    coreW += w;
						if (isBoundExclude[bIdx]) exclW += w;
					}
					addPoint(px, py, pz, coreW, exclW);
				}
				return;
			}
			else if (dVis->m_blendedType == 2 && dVis->m_blendedVertexAssignmentsWt)
			{
				for (int x = 0; x < dVis->m_assignmentCount; x++)
				{
					float px = 0.0f, py = 0.0f, pz = 0.0f, coreW = 0.0f, exclW = 0.0f;
					for (int b = 0; b < dVis->m_blendedVertexAssignmentsWt[x].boneIndexesCount; b++)
					{
						int bIdx = dVis->m_blendedVertexAssignmentsWt[x].boneIndexes[b];
						float w  = dVis->m_blendedVertexAssignmentsWt[x].weightsToBone[b];
						if (bIdx < 0 || bIdx >= boneCount || w == 0.0f)
							continue;
						CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
						if (!bPtr)
							continue;
						const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
						const ABVERTEX0Lb &off = dVis->m_blendedVertexAssignmentsWt[x].offsetVects[b];
						px += (M.m[0][0] * off.x + M.m[1][0] * off.y + M.m[2][0] * off.z + M.m[3][0]) * w;
						py += (M.m[0][1] * off.x + M.m[1][1] * off.y + M.m[2][1] * off.z + M.m[3][1]) * w;
						pz += (M.m[0][2] * off.x + M.m[1][2] * off.y + M.m[2][2] * off.z + M.m[3][2]) * w;
						if (isCoreTorso[bIdx])    coreW += w;
						if (isBoundExclude[bIdx]) exclW += w;
					}
					addPoint(px, py, pz, coreW, exclW);
				}
				return;
			}
		}
	};

	if (m_alternateConfigurations)
	{
		for (POSITION altPos = m_alternateConfigurations->GetHeadPosition(); altPos != NULL;)
		{
			CAlterUnitDBObject *altUnit = (CAlterUnitDBObject*)m_alternateConfigurations->GetNext(altPos);
			if (!altUnit || !altUnit->m_configurations || altUnit->m_configurations->IsEmpty())
				continue;
			CDeformableMesh *dMesh = (CDeformableMesh*)altUnit->m_configurations->GetHead();
			if (dMesh && dMesh->m_lodChain && !dMesh->m_lodChain->IsEmpty())
				accumulateVisObj((CDeformableVisObj*)dMesh->m_lodChain->GetHead());
		}
	}
	else if (m_deformableMeshArrayV2 && m_deformableMeshArrayCount > 0)
	{
		for (int i = 0; i < m_deformableMeshArrayCount; i++)
		{
			CDeformableMesh *dMesh = m_deformableMeshArrayV2[i].m_defMeshVisualUnit;
			if (dMesh && dMesh->m_lodChain && !dMesh->m_lodChain->IsEmpty())
				accumulateVisObj((CDeformableVisObj*)dMesh->m_lodChain->GetHead());
		}
	}

	delete [] isBoundExclude;
	delete [] isCoreTorso;

	if (hasTorso)
	{
		outMinX = tMinX;
		outMaxX = tMaxX;
		outMinY = tMinY;
		outMaxY = tMaxY;
		outMinZ = tMinZ;
		outMaxZ = tMaxZ;
		m_jiggleMinX = 0.0f;
		m_jiggleMaxX = 0.0f;
		m_jiggleBoundsValid = TRUE;
		return TRUE;
	}
	if (hasFallback)
	{
		outMinX = fMinX;
		outMaxX = fMaxX;
		outMinY = fMinY;
		outMaxY = fMaxY;
		outMinZ = fMinZ;
		outMaxZ = fMaxZ;
		m_jiggleMinX = 0.0f;
		m_jiggleMaxX = 0.0f;
		m_jiggleBoundsValid = TRUE;
		return TRUE;
	}
	return FALSE;
}

void CSkeletonObject::SculptFemaleDeformableMesh(
	CDeformableMesh *defMesh,
	float scaleFactor,
	float minX, float maxX,
	float minY, float maxY,
	float minZ, float maxZ)
{
	if (!defMesh || !defMesh->m_lodChain || !m_bonesList)
		return;

	// Female skeletons carry the R_boob / L_boob / R_butt / L_butt anatomy bones
	// (parented to the chest / pelvis).  They must exist before the per bone
	// classification arrays below are filled, so the sculpted bust / glute
	// vertices can be weighted to them further down.
	EnsureFemaleAnatomyBones();

	if (!m_bonesList->m_directAccessArray)
		m_bonesList->CompileDirectAccess();

	int boneCount = m_bonesList->m_directAccessArrayCount;
	if (boneCount <= 0)
		return;

	bool *isStrictLimbHead = new bool[boneCount];
	bool *isBoundExclude   = new bool[boneCount];
	bool *isCoreTorso      = new bool[boneCount];

	struct SpineNode { float x, y, z; };
	SpineNode spineChain[8];
	int spineCount = 0;

	bool hasPelvis = false, hasSpine0 = false, hasSpine1 = false, hasSpine2 = false, hasSpine3 = false, hasNeck = false;
	SpineNode pelvisP = { 0, 0, 0 }, spine0P = { 0, 0, 0 }, spine1P = { 0, 0, 0 };
	SpineNode spine2P = { 0, 0, 0 }, spine3P = { 0, 0, 0 }, neckP   = { 0, 0, 0 };

	for (int b = 0; b < boneCount; b++)
	{
		CBoneObject *bPtr = m_bonesList->m_directAccessArray[b].object;
		CString bName = bPtr ? bPtr->m_boneName : CString("");
		ClassifyBoneForFemaleSculpt(bName, isStrictLimbHead[b], isBoundExclude[b], isCoreTorso[b]);
		if (!bPtr)
			continue;

		const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
		SpineNode node = { M.m[3][0], M.m[3][1], M.m[3][2] };
		CString uName = bName;
		uName.MakeUpper();
		if (uName == "BIP01 PELVIS")      { pelvisP = node; hasPelvis = true; spineChain[spineCount++] = node; }
		else if (uName == "BIP01 SPINE")  { spine0P = node; hasSpine0 = true; spineChain[spineCount++] = node; }
		else if (uName == "BIP01 SPINE1") { spine1P = node; hasSpine1 = true; spineChain[spineCount++] = node; }
		else if (uName == "BIP01 SPINE2") { spine2P = node; hasSpine2 = true; spineChain[spineCount++] = node; }
		else if (uName == "BIP01 SPINE3") { spine3P = node; hasSpine3 = true; spineChain[spineCount++] = node; }
		else if (uName == "BIP01 NECK")   { neckP   = node; hasNeck   = true; spineChain[spineCount++] = node; }
	}

	// Sort spineChain by Y ascending
	for (int i = 0; i < spineCount - 1; i++)
	{
		for (int j = i + 1; j < spineCount; j++)
		{
			if (spineChain[j].y < spineChain[i].y)
			{
				SpineNode tmp = spineChain[i];
				spineChain[i] = spineChain[j];
				spineChain[j] = tmp;
			}
		}
	}

	float fallbackCX = 0.5f * (minX + maxX);
	float fallbackCZ = 0.5f * (minZ + maxZ);

	auto spineXZAtY = [&](float py, float &outSX, float &outSZ) {
		if (spineCount <= 0)
		{
			outSX = fallbackCX;
			outSZ = fallbackCZ;
			return;
		}
		if (py <= spineChain[0].y)
		{
			outSX = spineChain[0].x;
			outSZ = spineChain[0].z;
			return;
		}
		if (py >= spineChain[spineCount - 1].y)
		{
			outSX = spineChain[spineCount - 1].x;
			outSZ = spineChain[spineCount - 1].z;
			return;
		}
		for (int i = 0; i < spineCount - 1; i++)
		{
			if (py >= spineChain[i].y && py <= spineChain[i + 1].y)
			{
				float dy = spineChain[i + 1].y - spineChain[i].y;
				float t  = (dy > 1e-5f) ? ((py - spineChain[i].y) / dy) : 0.0f;
				outSX = spineChain[i].x + t * (spineChain[i + 1].x - spineChain[i].x);
				outSZ = spineChain[i].z + t * (spineChain[i + 1].z - spineChain[i].z);
				return;
			}
		}
		outSX = fallbackCX;
		outSZ = fallbackCZ;
	};

	float pelvisY = hasPelvis ? pelvisP.y : minY;
	float neckY   = hasNeck   ? neckP.y   : maxY;
	float spineH  = (neckY - pelvisY > 0.1f) ? (neckY - pelvisY) : (maxY - minY);
	if (spineH <= 0.1f)
	{
		delete [] isStrictLimbHead;
		delete [] isBoundExclude;
		delete [] isCoreTorso;
		return;
	}

	float topSpineY = hasSpine3 ? spine3P.y : (hasSpine2 ? spine2P.y : (hasSpine1 ? spine1P.y : (pelvisY + 0.78f * spineH)));
	float bustY     = topSpineY + (hasSpine3 ? 0.06f * spineH : -0.02f * spineH);
	float waistY    = (hasSpine0 && hasSpine1) ? (0.5f * (spine0P.y + spine1P.y)) : (pelvisY + 0.34f * spineH);
	float buttY     = pelvisY - 0.03f * spineH;

	// Measure spine-relative chest & hip cross-sections across all slots (cached in m_jiggleMin/Max on first call)
	if (m_jiggleMinX <= 0.01f || m_jiggleMaxX <= 0.01f)
	{
		float cMinX = 1e30f, cMaxX = -1e30f, cMinZ = 1e30f, cMaxZ = -1e30f;
		float hMinX = 1e30f, hMaxX = -1e30f, hMinZ = 1e30f, hMaxZ = -1e30f;
		bool hasChest = false, hasHip = false;

		auto sampleCrossSection = [&](float px, float py, float pz, float coreW, float exclW) {
			if (coreW < 0.45f || exclW >= 0.15f)
				return;
			float sx, sz;
			spineXZAtY(py, sx, sz);
			float rx = px - sx;
			float rz = pz - sz;
			if (fabsf(py - bustY) <= 0.25f * spineH)
			{
				if (rx < cMinX) cMinX = rx; if (rx > cMaxX) cMaxX = rx;
				if (rz < cMinZ) cMinZ = rz; if (rz > cMaxZ) cMaxZ = rz;
				hasChest = true;
			}
			if (fabsf(py - buttY) <= 0.25f * spineH)
			{
				if (rx < hMinX) hMinX = rx; if (rx > hMaxX) hMaxX = rx;
				if (rz < hMinZ) hMinZ = rz; if (rz > hMaxZ) hMaxZ = rz;
				hasHip = true;
			}
		};

		if (m_alternateConfigurations)
		{
			for (POSITION altPos = m_alternateConfigurations->GetHeadPosition(); altPos != NULL;)
			{
				CAlterUnitDBObject *altUnit = (CAlterUnitDBObject*)m_alternateConfigurations->GetNext(altPos);
				if (!altUnit || !altUnit->m_configurations || altUnit->m_configurations->IsEmpty())
					continue;
				CDeformableMesh *dMesh = (CDeformableMesh*)altUnit->m_configurations->GetHead();
				if (!dMesh || !dMesh->m_lodChain || dMesh->m_lodChain->IsEmpty())
					continue;
				CDeformableVisObj *dVis = (CDeformableVisObj*)dMesh->m_lodChain->GetHead();
				if (!dVis || !dVis->m_mesh || dVis->m_assignmentCount <= 0)
					continue;

				if (dVis->m_blendedType == 0 && dVis->m_vertexAssignments && dVis->m_originalArrayVector)
				{
					for (int x = 0; x < dVis->m_assignmentCount; x++)
					{
						int bIdx = dVis->m_vertexAssignments[x].boneIndex;
						int vIdx = dVis->m_vertexAssignments[x].vertexIndex;
						if (bIdx < 0 || bIdx >= boneCount) continue;
						CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
						if (!bPtr) continue;
						const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
						const ABVERTEX0L &orig = dVis->m_originalArrayVector[vIdx];
						float px = M.m[0][0] * orig.x + M.m[1][0] * orig.y + M.m[2][0] * orig.z + M.m[3][0];
						float py = M.m[0][1] * orig.x + M.m[1][1] * orig.y + M.m[2][1] * orig.z + M.m[3][1];
						float pz = M.m[0][2] * orig.x + M.m[1][2] * orig.y + M.m[2][2] * orig.z + M.m[3][2];
						sampleCrossSection(px, py, pz, isCoreTorso[bIdx] ? 1.0f : 0.0f, isBoundExclude[bIdx] ? 1.0f : 0.0f);
					}
				}
				else if (dVis->m_blendedType == 1 && dVis->m_blendedVertexAssignments)
				{
					for (int x = 0; x < dVis->m_assignmentCount; x++)
					{
						float px = 0.0f, py = 0.0f, pz = 0.0f, coreW = 0.0f, exclW = 0.0f;
						for (int b = 0; b < dVis->m_blendedVertexAssignments[x].boneIndexesCount; b++)
						{
							int bIdx = dVis->m_blendedVertexAssignments[x].boneIndexes[b];
							float w  = dVis->m_blendedVertexAssignments[x].weightsToBone[b];
							if (bIdx < 0 || bIdx >= boneCount || w == 0.0f) continue;
							CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
							if (!bPtr) continue;
							const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
							const ABVERTEX0L &off = dVis->m_blendedVertexAssignments[x].offsetVects[b];
							px += (M.m[0][0] * off.x + M.m[1][0] * off.y + M.m[2][0] * off.z + M.m[3][0]) * w;
							py += (M.m[0][1] * off.x + M.m[1][1] * off.y + M.m[2][1] * off.z + M.m[3][1]) * w;
							pz += (M.m[0][2] * off.x + M.m[1][2] * off.y + M.m[2][2] * off.z + M.m[3][2]) * w;
							if (isCoreTorso[bIdx])    coreW += w;
							if (isBoundExclude[bIdx]) exclW += w;
						}
						sampleCrossSection(px, py, pz, coreW, exclW);
					}
				}
				else if (dVis->m_blendedType == 2 && dVis->m_blendedVertexAssignmentsWt)
				{
					for (int x = 0; x < dVis->m_assignmentCount; x++)
					{
						float px = 0.0f, py = 0.0f, pz = 0.0f, coreW = 0.0f, exclW = 0.0f;
						for (int b = 0; b < dVis->m_blendedVertexAssignmentsWt[x].boneIndexesCount; b++)
						{
							int bIdx = dVis->m_blendedVertexAssignmentsWt[x].boneIndexes[b];
							float w  = dVis->m_blendedVertexAssignmentsWt[x].weightsToBone[b];
							if (bIdx < 0 || bIdx >= boneCount || w == 0.0f) continue;
							CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
							if (!bPtr) continue;
							const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
							const ABVERTEX0Lb &off = dVis->m_blendedVertexAssignmentsWt[x].offsetVects[b];
							px += (M.m[0][0] * off.x + M.m[1][0] * off.y + M.m[2][0] * off.z + M.m[3][0]) * w;
							py += (M.m[0][1] * off.x + M.m[1][1] * off.y + M.m[2][1] * off.z + M.m[3][1]) * w;
							pz += (M.m[0][2] * off.x + M.m[1][2] * off.y + M.m[2][2] * off.z + M.m[3][2]) * w;
							if (isCoreTorso[bIdx])    coreW += w;
							if (isBoundExclude[bIdx]) exclW += w;
						}
						sampleCrossSection(px, py, pz, coreW, exclW);
					}
				}
			}
		}

		m_jiggleMinX = hasChest ? (cMaxX - cMinX) : (maxX - minX);
		m_jiggleMaxX = hasChest ? (cMaxZ - cMinZ) : (maxZ - minZ);
		m_jiggleMinZ = hasHip   ? (hMaxX - hMinX) : m_jiggleMinX;
		m_jiggleMaxZ = hasHip   ? (hMaxZ - hMinZ) : m_jiggleMaxX;
	}

	float chestW = m_jiggleMinX;
	float chestD = m_jiggleMaxX;
	float hipW   = m_jiggleMinZ;
	float hipD   = m_jiggleMaxZ;

	float bustRadY  = (hasSpine3 ? 0.24f : 0.23f) * spineH;
	float waistRadY = 0.20f * spineH;
	float buttRadY  = 0.25f * spineH;

	// Seam-safe, C1-continuous bone-chain female morph (+Z = Front/Chest, -Z = Back/Glutes)
	// bustGate / gluteGate report the sculpted region gradients (0 .. 1) so the
	// skin weighting can bind exactly the vertices this morph pushes out.
	auto evalFemaleMorph = [&](float px, float py, float pz, float torsoFactor,
	                           float &outDX, float &outDY, float &outDZ,
	                           float &outDNX, float &outDNY, float &outDNZ,
	                           float * outBustGate = NULL, float * outGluteGate = NULL)
	{
		outDX = outDY = outDZ = 0.0f;
		outDNX = outDNY = outDNZ = 0.0f;
		if (outBustGate)
			*outBustGate = 0.0f;
		if (outGluteGate)
			*outGluteGate = 0.0f;
		if (torsoFactor <= 0.001f)
			return;

		float sx, sz;
		spineXZAtY(py, sx, sz);
		float rx = px - sx;
		float rz = pz - sz;
		float absX = fabsf(rx);
		float s = torsoFactor * scaleFactor;

		// 1. Smooth Rounded Breasts / Bust on Front Chest (+Z: rz > 0)
		float uyB = (py - bustY) / bustRadY;
		if (uyB > -1.0f && uyB < 1.0f && rz > 0.02f * chestD && absX < 0.52f * chestW)
		{
			float tFront = (rz - 0.02f * chestD) / (0.24f * chestD);
			if (tFront > 1.0f) tFront = 1.0f;
			if (tFront > 0.0f)
			{
				float frontGate = tFront * tFront * (3.0f - 2.0f * tFront);

				float cupCenter = 0.24f * chestW;
				float cupHalfW  = 0.27f * chestW;
				float ux = (absX - cupCenter) / cupHalfW;
				float ux2 = ux * ux;
				float profX = (fabsf(ux) < 1.0f) ? (1.0f - ux2 * ux2) : 0.0f;

				float cNorm = rx / (0.045f * chestW);
				float cleavage = 1.0f - 0.42f * expf(-cNorm * cNorm);

				float uyB2  = uyB * uyB;
				float profY = (1.0f - uyB2 * uyB2) * (1.0f - 0.35f * uyB);
				float dome  = profX * profY * cleavage;

				if (outBustGate)
					*outBustGate = dome * frontGate;

				if (dome > 0.0f)
				{
					float w = dome * frontGate * s;
					outDZ += 0.21f * chestD * w;
					outDY -= 0.024f * spineH * w;
					outDX += rx * 0.07f * w;

					outDNX += ((rx >= 0.0f) ? 0.18f : -0.18f) * ux * w;
					outDNY += 0.20f * uyB * w;
					outDNZ += 0.25f * w;
				}
			}
		}

		// 2. Hourglass Waist Taper at waistY
		float uyW = (py - waistY) / waistRadY;
		if (uyW > -1.0f && uyW < 1.0f && absX < 0.50f * chestW)
		{
			float tW = 1.0f - uyW * uyW;
			float wWaist = tW * tW;
			float wx = absX / (0.50f * chestW);
			float xGate = 1.0f - wx * wx;
			outDX -= rx * 0.25f * wWaist * xGate * s;
		}

		// 3. Feminine Hips & Rounded Rear Glutes / Butt at buttY (-Z: rz < 0)
		float uyH = (py - buttY) / buttRadY;
		if (uyH > -1.0f && uyH < 1.0f && absX < 0.58f * hipW)
		{
			float tH = 1.0f - uyH * uyH;
			float wHip = tH * tH;
			float hx = absX / (0.58f * hipW);
			float hipGate = 1.0f - hx * hx;
			outDX += rx * 0.20f * wHip * hipGate * s;

			if (rz < -0.02f * hipD)
			{
				float tRear = (-0.02f * hipD - rz) / (0.32f * hipD);
				if (tRear > 1.0f) tRear = 1.0f;
				if (tRear > 0.0f)
				{
					float rearGate = tRear * tRear * (3.0f - 2.0f * tRear);

					float uxB = (absX - 0.19f * hipW) / (0.25f * hipW);
					float uxB2 = uxB * uxB;
					float pX = (fabsf(uxB) < 1.0f) ? (1.0f - uxB2 * uxB2) : 0.0f;
					float domeB = pX * wHip;

					if (outGluteGate)
						*outGluteGate = domeB * rearGate;

					if (domeB > 0.0f)
					{
						float wB = domeB * rearGate * s;
						outDZ -= 0.26f * hipD * wB;
						outDY += 0.015f * spineH * wB;
						outDX += rx * 0.08f * wB;

						outDNX += ((rx >= 0.0f) ? 0.16f : -0.16f) * uxB * wB;
						outDNY += 0.18f * uyH * wB;
						outDNZ -= 0.25f * wB;
					}
				}
			}
		}
	};

	// ------------------------------------------------------------------
	// Female anatomy bones: give the sculpted bust / glute vertices their
	// own bone weights (R_boob / L_boob / R_butt / L_butt).  The region
	// gates are the very same dome gradients that drive the morph above, so
	// the weighted region and the sculpted region are identical and the
	// surface can never tear along the edge of the shape.  Both skinning
	// flavours are handled in place: m_blendedType == 0 visual units are
	// upgraded to weighted assignments, existing m_blendedType == 1 weights
	// are merged with the anatomy bone.  All weights stay normalised, so the
	// bind pose (M_world * inverse(M_rest) == identity) is left untouched.
	// ------------------------------------------------------------------
	const int boobRIndex = FindBoneListIndexByName(m_bonesList, "R_boob");
	const int boobLIndex = FindBoneListIndexByName(m_bonesList, "L_boob");
	const int buttRIndex = FindBoneListIndexByName(m_bonesList, "R_butt");
	const int buttLIndex = FindBoneListIndexByName(m_bonesList, "L_butt");
	const bool anatomyBonesReady = (boobRIndex >= 0 && boobLIndex >= 0 && buttRIndex >= 0 && buttLIndex >= 0);

	if (anatomyBonesReady)
	for (POSITION bindPos = defMesh->m_lodChain->GetHeadPosition(); bindPos != NULL;)
	{
		CDeformableVisObj *bindVis = (CDeformableVisObj*)defMesh->m_lodChain->GetNext(bindPos);
		if (!bindVis || !bindVis->m_mesh || bindVis->m_assignmentCount <= 0)
			continue;
		if (bindVis->m_blendedType == 2)
			continue;                                  // tangent space flavour keeps its own weights
		if (bindVis->m_blendedType == 0 && (!bindVis->m_vertexAssignments || !bindVis->m_originalArrayVector))
			continue;
		if (bindVis->m_blendedType == 1 && !bindVis->m_blendedVertexAssignments)
			continue;

		const int bindVertexCount = bindVis->m_mesh->m_abVertexArray.m_vertexCount;
		if (bindVertexCount <= 0)
			continue;

		int   * targetBone   = new int[bindVis->m_assignmentCount];
		float * targetWeight = new float[bindVis->m_assignmentCount];
		float * restPosX = new float[bindVis->m_assignmentCount];
		float * restPosY = new float[bindVis->m_assignmentCount];
		float * restPosZ = new float[bindVis->m_assignmentCount];
		float * restNorX = new float[bindVis->m_assignmentCount];
		float * restNorY = new float[bindVis->m_assignmentCount];
		float * restNorZ = new float[bindVis->m_assignmentCount];

		bool anyTarget = false;
		for (int bindLoop = 0; bindLoop < bindVis->m_assignmentCount; bindLoop++)
		{
			targetBone[bindLoop]   = -1;
			targetWeight[bindLoop] = 0.0f;
			restPosX[bindLoop] = restPosY[bindLoop] = restPosZ[bindLoop] = 0.0f;
			restNorX[bindLoop] = restNorY[bindLoop] = restNorZ[bindLoop] = 0.0f;

			float px = 0.0f, py = 0.0f, pz = 0.0f;
			float nx = 0.0f, ny = 0.0f, nz = 0.0f;
			float limbWeight = 0.0f;
			bool  usable = false;

			if (bindVis->m_blendedType == 0)
			{
				const int bIdx = bindVis->m_vertexAssignments[bindLoop].boneIndex;
				const int vIdx = bindVis->m_vertexAssignments[bindLoop].vertexIndex;
				if (bIdx < 0 || bIdx >= boneCount || vIdx < 0 || vIdx >= bindVertexCount)
					continue;
				if (isStrictLimbHead[bIdx])
					continue;                          // arms / legs / head never ride the anatomy bones
				CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
				if (!bPtr)
					continue;

				// rest pose: the stored vertex is in that bone's own rest space
				const ABVERTEX0L & stored = bindVis->m_originalArrayVector[vIdx];
				const D3DMATRIX & M = bPtr->m_startPosition;
				px = M.m[0][0] * stored.x + M.m[1][0] * stored.y + M.m[2][0] * stored.z + M.m[3][0];
				py = M.m[0][1] * stored.x + M.m[1][1] * stored.y + M.m[2][1] * stored.z + M.m[3][1];
				pz = M.m[0][2] * stored.x + M.m[1][2] * stored.y + M.m[2][2] * stored.z + M.m[3][2];
				nx = M.m[0][0] * stored.nx + M.m[1][0] * stored.ny + M.m[2][0] * stored.nz;
				ny = M.m[0][1] * stored.nx + M.m[1][1] * stored.ny + M.m[2][1] * stored.nz;
				nz = M.m[0][2] * stored.nx + M.m[1][2] * stored.ny + M.m[2][2] * stored.nz;
				usable = true;
			}
			else
			{
				VertexAssignmentBlended & entry = bindVis->m_blendedVertexAssignments[bindLoop];
				if (entry.boneIndexesCount <= 0 || !entry.boneIndexes || !entry.weightsToBone || !entry.offsetVects)
					continue;

				for (int boneLoop = 0; boneLoop < entry.boneIndexesCount; boneLoop++)
				{
					const int bIdx = entry.boneIndexes[boneLoop];
					const float w = entry.weightsToBone[boneLoop];
					if (bIdx < 0 || bIdx >= boneCount || w == 0.0f)
						continue;
					if (isStrictLimbHead[bIdx])
						limbWeight += w;
					CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
					if (!bPtr)
						continue;

					const ABVERTEX0L & off = entry.offsetVects[boneLoop];
					const D3DMATRIX & M = bPtr->m_startPosition;
					px += (M.m[0][0] * off.x + M.m[1][0] * off.y + M.m[2][0] * off.z + M.m[3][0]) * w;
					py += (M.m[0][1] * off.x + M.m[1][1] * off.y + M.m[2][1] * off.z + M.m[3][1]) * w;
					pz += (M.m[0][2] * off.x + M.m[1][2] * off.y + M.m[2][2] * off.z + M.m[3][2]) * w;
					nx += (M.m[0][0] * off.nx + M.m[1][0] * off.ny + M.m[2][0] * off.nz) * w;
					ny += (M.m[0][1] * off.nx + M.m[1][1] * off.ny + M.m[2][1] * off.nz) * w;
					nz += (M.m[0][2] * off.nx + M.m[1][2] * off.ny + M.m[2][2] * off.nz) * w;
				}

				if (limbWeight >= 0.5f)
					continue;                          // hanging arms / legs must not follow the chest
				usable = true;
			}

			if (!usable)
				continue;

			float morphDX, morphDY, morphDZ, morphNX, morphNY, morphNZ;
			float bustGate = 0.0f, gluteGate = 0.0f;
			evalFemaleMorph(px, py, pz, 1.0f - limbWeight,
							morphDX, morphDY, morphDZ, morphNX, morphNY, morphNZ,
							&bustGate, &gluteGate);

			float gate = (bustGate > gluteGate) ? bustGate : gluteGate;
			if (gate <= 0.02f)
				continue;                              // not part of the sculpted bust / glute volume
			if (gate > 1.0f)
				gate = 1.0f;

			const bool bustRegion = (bustGate >= gluteGate);
			float spineSampleX, spineSampleZ;
			spineXZAtY(py, spineSampleX, spineSampleZ);
			const bool leftSide = ((px - spineSampleX) >= 0.0f);

			targetBone[bindLoop] = bustRegion ? (leftSide ? boobLIndex : boobRIndex)
											  : (leftSide ? buttLIndex : buttRIndex);
			targetWeight[bindLoop] = FEMALE_ANATOMY_MAX_SKIN_WEIGHT * gate;

			restPosX[bindLoop] = px;
			restPosY[bindLoop] = py;
			restPosZ[bindLoop] = pz;
			restNorX[bindLoop] = nx;
			restNorY[bindLoop] = ny;
			restNorZ[bindLoop] = nz;

			anyTarget = true;
		}

		if (anyTarget)
			BindFemaleAnatomyWeightsOnVisObj(bindVis,
											 targetBone, targetWeight,
											 restPosX, restPosY, restPosZ,
											 restNorX, restNorY, restNorZ);

		delete [] targetBone;
		delete [] targetWeight;
		delete [] restPosX;
		delete [] restPosY;
		delete [] restPosZ;
		delete [] restNorX;
		delete [] restNorY;
		delete [] restNorZ;
	}

	for (POSITION lodPos = defMesh->m_lodChain->GetHeadPosition(); lodPos != NULL;)
	{
		CDeformableVisObj *dVis = (CDeformableVisObj*)defMesh->m_lodChain->GetNext(lodPos);
		if (!dVis || !dVis->m_mesh)
			continue;

		int vCount = dVis->m_mesh->m_abVertexArray.m_vertexCount;
		int vSize  = dVis->m_mesh->m_abVertexArray.GetVertexSize();
		byte *vBuf = (byte*)dVis->m_mesh->m_abVertexArray.GetVertexBuf();

		auto updateMeshVertex = [&](int vIdx, float dx, float dy, float dz, float dnx, float dny, float dnz) {
			if (!vBuf || vIdx < 0 || vIdx >= vCount || vSize <= 0)
				return;
			ABVERTEX1Lb *vtx = (ABVERTEX1Lb*)&vBuf[vIdx * vSize];
			vtx->x += dx;
			vtx->y += dy;
			vtx->z += dz;
			vtx->nx += dnx;
			vtx->ny += dny;
			vtx->nz += dnz;
			float nLen = sqrtf(vtx->nx * vtx->nx + vtx->ny * vtx->ny + vtx->nz * vtx->nz);
			if (nLen > 1e-5f)
			{
				vtx->nx /= nLen;
				vtx->ny /= nLen;
				vtx->nz /= nLen;
			}
		};

		if (dVis->m_blendedType == 0 && dVis->m_vertexAssignments && dVis->m_originalArrayVector)
		{
			for (int x = 0; x < dVis->m_assignmentCount; x++)
			{
				int bIdx = dVis->m_vertexAssignments[x].boneIndex;
				int vIdx = dVis->m_vertexAssignments[x].vertexIndex;
				if (bIdx < 0 || bIdx >= boneCount || vIdx < 0 || vIdx >= vCount)
					continue;
				if (isStrictLimbHead[bIdx])
					continue;
				CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
				if (!bPtr)
					continue;

				const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
				ABVERTEX0L &orig = dVis->m_originalArrayVector[vIdx];
				float px = M.m[0][0] * orig.x + M.m[1][0] * orig.y + M.m[2][0] * orig.z + M.m[3][0];
				float py = M.m[0][1] * orig.x + M.m[1][1] * orig.y + M.m[2][1] * orig.z + M.m[3][1];
				float pz = M.m[0][2] * orig.x + M.m[1][2] * orig.y + M.m[2][2] * orig.z + M.m[3][2];

				float dx, dy, dz, dnx, dny, dnz;
				evalFemaleMorph(px, py, pz, 1.0f, dx, dy, dz, dnx, dny, dnz);
				if (fabs(dx) < 1e-5f && fabs(dy) < 1e-5f && fabs(dz) < 1e-5f)
					continue;

				D3DMATRIX invM;
				D3DMATRIX srcM = M;
				GL_matrixFunctions.D3DMath_MatrixInvert(invM, srcM);
				float ldx  = invM.m[0][0] * dx  + invM.m[1][0] * dy  + invM.m[2][0] * dz;
				float ldy  = invM.m[0][1] * dx  + invM.m[1][1] * dy  + invM.m[2][1] * dz;
				float ldz  = invM.m[0][2] * dx  + invM.m[1][2] * dy  + invM.m[2][2] * dz;
				float ldnx = invM.m[0][0] * dnx + invM.m[1][0] * dny + invM.m[2][0] * dnz;
				float ldny = invM.m[0][1] * dnx + invM.m[1][1] * dny + invM.m[2][1] * dnz;
				float ldnz = invM.m[0][2] * dnx + invM.m[1][2] * dny + invM.m[2][2] * dnz;

				orig.x += ldx; orig.y += ldy; orig.z += ldz;
				orig.nx += ldnx; orig.ny += ldny; orig.nz += ldnz;
				float nLen = sqrtf(orig.nx * orig.nx + orig.ny * orig.ny + orig.nz * orig.nz);
				if (nLen > 1e-5f) { orig.nx /= nLen; orig.ny /= nLen; orig.nz /= nLen; }

				updateMeshVertex(vIdx, dx, dy, dz, dnx, dny, dnz);
				for (int c = 0; c < dVis->m_vertexAssignments[x].carriedCount; c++)
				{
					int cIdx = dVis->m_vertexAssignments[x].vertexesCarried[c];
					if (cIdx >= 0 && cIdx < vCount && cIdx != vIdx)
					{
						dVis->m_originalArrayVector[cIdx] = orig;
						updateMeshVertex(cIdx, dx, dy, dz, dnx, dny, dnz);
					}
				}
			}
		}
		else if (dVis->m_blendedType == 1 && dVis->m_blendedVertexAssignments)
		{
			for (int x = 0; x < dVis->m_assignmentCount; x++)
			{
				float px = 0.0f, py = 0.0f, pz = 0.0f, limbW = 0.0f;
				for (int b = 0; b < dVis->m_blendedVertexAssignments[x].boneIndexesCount; b++)
				{
					int bIdx = dVis->m_blendedVertexAssignments[x].boneIndexes[b];
					float w  = dVis->m_blendedVertexAssignments[x].weightsToBone[b];
					if (bIdx < 0 || bIdx >= boneCount || w == 0.0f)
						continue;
					if (isStrictLimbHead[bIdx])
						limbW += w;
					CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
					if (!bPtr)
						continue;
					const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
					const ABVERTEX0L &off = dVis->m_blendedVertexAssignments[x].offsetVects[b];
					px += (M.m[0][0] * off.x + M.m[1][0] * off.y + M.m[2][0] * off.z + M.m[3][0]) * w;
					py += (M.m[0][1] * off.x + M.m[1][1] * off.y + M.m[2][1] * off.z + M.m[3][1]) * w;
					pz += (M.m[0][2] * off.x + M.m[1][2] * off.y + M.m[2][2] * off.z + M.m[3][2]) * w;
				}
				if (limbW >= 0.99f)
					continue;

				float dx, dy, dz, dnx, dny, dnz;
				evalFemaleMorph(px, py, pz, 1.0f - limbW, dx, dy, dz, dnx, dny, dnz);
				if (fabs(dx) < 1e-5f && fabs(dy) < 1e-5f && fabs(dz) < 1e-5f)
					continue;

				for (int b = 0; b < dVis->m_blendedVertexAssignments[x].boneIndexesCount; b++)
				{
					int bIdx = dVis->m_blendedVertexAssignments[x].boneIndexes[b];
					if (bIdx < 0 || bIdx >= boneCount)
						continue;
					CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
					if (!bPtr)
						continue;
					D3DMATRIX invM;
					D3DMATRIX srcM = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
					GL_matrixFunctions.D3DMath_MatrixInvert(invM, srcM);
					ABVERTEX0L &off = dVis->m_blendedVertexAssignments[x].offsetVects[b];
					off.x  += invM.m[0][0] * dx  + invM.m[1][0] * dy  + invM.m[2][0] * dz;
					off.y  += invM.m[0][1] * dx  + invM.m[1][1] * dy  + invM.m[2][1] * dz;
					off.z  += invM.m[0][2] * dx  + invM.m[1][2] * dy  + invM.m[2][2] * dz;
					off.nx += invM.m[0][0] * dnx + invM.m[1][0] * dny + invM.m[2][0] * dnz;
					off.ny += invM.m[0][1] * dnx + invM.m[1][1] * dny + invM.m[2][1] * dnz;
					off.nz += invM.m[0][2] * dnx + invM.m[1][2] * dny + invM.m[2][2] * dnz;
					float nLen = sqrtf(off.nx * off.nx + off.ny * off.ny + off.nz * off.nz);
					if (nLen > 1e-5f) { off.nx /= nLen; off.ny /= nLen; off.nz /= nLen; }
				}

				int vIdx = dVis->m_blendedVertexAssignments[x].vertexIndex;
				updateMeshVertex(vIdx, dx, dy, dz, dnx, dny, dnz);
				for (int c = 0; c < dVis->m_blendedVertexAssignments[x].carriedCount; c++)
				{
					updateMeshVertex(dVis->m_blendedVertexAssignments[x].vertexesCarried[c], dx, dy, dz, dnx, dny, dnz);
				}
			}
		}
		else if (dVis->m_blendedType == 2 && dVis->m_blendedVertexAssignmentsWt)
		{
			for (int x = 0; x < dVis->m_assignmentCount; x++)
			{
				float px = 0.0f, py = 0.0f, pz = 0.0f, limbW = 0.0f;
				for (int b = 0; b < dVis->m_blendedVertexAssignmentsWt[x].boneIndexesCount; b++)
				{
					int bIdx = dVis->m_blendedVertexAssignmentsWt[x].boneIndexes[b];
					float w  = dVis->m_blendedVertexAssignmentsWt[x].weightsToBone[b];
					if (bIdx < 0 || bIdx >= boneCount || w == 0.0f)
						continue;
					if (isStrictLimbHead[bIdx])
						limbW += w;
					CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
					if (!bPtr)
						continue;
					const D3DMATRIX &M = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
					const ABVERTEX0Lb &off = dVis->m_blendedVertexAssignmentsWt[x].offsetVects[b];
					px += (M.m[0][0] * off.x + M.m[1][0] * off.y + M.m[2][0] * off.z + M.m[3][0]) * w;
					py += (M.m[0][1] * off.x + M.m[1][1] * off.y + M.m[2][1] * off.z + M.m[3][1]) * w;
					pz += (M.m[0][2] * off.x + M.m[1][2] * off.y + M.m[2][2] * off.z + M.m[3][2]) * w;
				}
				if (limbW >= 0.99f)
					continue;

				float dx, dy, dz, dnx, dny, dnz;
				evalFemaleMorph(px, py, pz, 1.0f - limbW, dx, dy, dz, dnx, dny, dnz);
				if (fabs(dx) < 1e-5f && fabs(dy) < 1e-5f && fabs(dz) < 1e-5f)
					continue;

				for (int b = 0; b < dVis->m_blendedVertexAssignmentsWt[x].boneIndexesCount; b++)
				{
					int bIdx = dVis->m_blendedVertexAssignmentsWt[x].boneIndexes[b];
					if (bIdx < 0 || bIdx >= boneCount)
						continue;
					CBoneObject *bPtr = m_bonesList->m_directAccessArray[bIdx].object;
					if (!bPtr)
						continue;
					D3DMATRIX invM;
					D3DMATRIX srcM = bPtr->m_boneFrame ? bPtr->m_boneFrame->List_frameMatrix : bPtr->m_startPosition;
					GL_matrixFunctions.D3DMath_MatrixInvert(invM, srcM);
					ABVERTEX0Lb &off = dVis->m_blendedVertexAssignmentsWt[x].offsetVects[b];
					off.x  += invM.m[0][0] * dx  + invM.m[1][0] * dy  + invM.m[2][0] * dz;
					off.y  += invM.m[0][1] * dx  + invM.m[1][1] * dy  + invM.m[2][1] * dz;
					off.z  += invM.m[0][2] * dx  + invM.m[1][2] * dy  + invM.m[2][2] * dz;
					off.nx += invM.m[0][0] * dnx + invM.m[1][0] * dny + invM.m[2][0] * dnz;
					off.ny += invM.m[0][1] * dnx + invM.m[1][1] * dny + invM.m[2][1] * dnz;
					off.nz += invM.m[0][2] * dnx + invM.m[1][2] * dny + invM.m[2][2] * dnz;
					float nLen = sqrtf(off.nx * off.nx + off.ny * off.ny + off.nz * off.nz);
					if (nLen > 1e-5f) { off.nx /= nLen; off.ny /= nLen; off.nz /= nLen; }
				}

				int vIdx = dVis->m_blendedVertexAssignmentsWt[x].vertexIndex;
				updateMeshVertex(vIdx, dx, dy, dz, dnx, dny, dnz);
				for (int c = 0; c < dVis->m_blendedVertexAssignmentsWt[x].carriedCount; c++)
				{
					updateMeshVertex(dVis->m_blendedVertexAssignmentsWt[x].vertexesCarried[c], dx, dy, dz, dnx, dny, dnz);
				}
			}
		}
	}

	delete [] isStrictLimbHead;
}

void CSkeletonObject::GenerateFemaleBodyVariations(int headSlotIndex, BOOL sculptExistingConfigs, float existingScaleFactor)
{
	if (!m_alternateConfigurations || !m_bonesList)
		return;

	if (!m_bonesList->m_directAccessArray)
		m_bonesList->CompileDirectAccess();

	float minX, maxX, minY, maxY, minZ, maxZ;
	if (!ComputeSkinBounds(minX, maxX, minY, maxY, minZ, maxZ))
		return;

	// Guard against running twice on the same skeleton
	for (POSITION altPos = m_alternateConfigurations->GetHeadPosition(); altPos != NULL;)
	{
		CAlterUnitDBObject *altUnit = (CAlterUnitDBObject*)m_alternateConfigurations->GetNext(altPos);
		if (!altUnit || !altUnit->m_configurations)
			continue;
		for (POSITION cfgPos = altUnit->m_configurations->GetHeadPosition(); cfgPos != NULL;)
		{
			CDeformableMesh *dm = (CDeformableMesh*)altUnit->m_configurations->GetNext(cfgPos);
			if (dm && dm->m_dimName.Find("_Fem") != -1)
				return;
		}
	}

	int slotIdx = 0;
	for (POSITION altPos = m_alternateConfigurations->GetHeadPosition(); altPos != NULL; ++slotIdx)
	{
		CAlterUnitDBObject *altUnit = (CAlterUnitDBObject*)m_alternateConfigurations->GetNext(altPos);
		if (!altUnit || !altUnit->m_configurations || altUnit->m_configurations->IsEmpty())
			continue;

		if (slotIdx == headSlotIndex)
			continue;

		// Collect ONLY genuinely public base configurations (never grab m_publicDim == FALSE armor pieces!)
		CPtrArray basePublicMeshes;
		int cfgIdx = 0;
		for (POSITION cfgPos = altUnit->m_configurations->GetHeadPosition(); cfgPos != NULL; ++cfgIdx)
		{
			CDeformableMesh *dm = (CDeformableMesh*)altUnit->m_configurations->GetNext(cfgPos);
			if (dm && (cfgIdx == 0 || dm->m_publicDim) && dm->m_dimName.Find("_Fem") == -1)
			{
				basePublicMeshes.Add(dm);
			}
		}

		if (!sculptExistingConfigs)
		{
			// Append 1 set of Female public variations (_FemCurvy) per base public skin color
			for (INT_PTR i = 0; i < basePublicMeshes.GetSize(); i++)
			{
				CDeformableMesh *srcMesh = (CDeformableMesh*)basePublicMeshes.GetAt(i);
				CDeformableMesh *curvyMesh = NULL;
				srcMesh->CloneAllRaw(&curvyMesh, NULL);
				if (curvyMesh)
				{
					curvyMesh->m_publicDim = TRUE;
					curvyMesh->m_dimName = srcMesh->m_dimName + "_FemCurvy";
					SculptFemaleDeformableMesh(curvyMesh, existingScaleFactor, minX, maxX, minY, maxY, minZ, maxZ);
					altUnit->m_configurations->AddTail(curvyMesh);
				}
			}
		}
		else if (existingScaleFactor > 0.01f)
		{
			for (INT_PTR i = 0; i < basePublicMeshes.GetSize(); i++)
			{
				CDeformableMesh *dm = (CDeformableMesh*)basePublicMeshes.GetAt(i);
				if (dm)
					SculptFemaleDeformableMesh(dm, existingScaleFactor, minX, maxX, minY, maxY, minZ, maxZ);
			}
		}
	}
}


//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------EnsureFemaleAnatomyBones------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
// Adds R_boob / L_boob (parented to Bip01 Spine3, or to Bip01 Spine2 when a rig
// has no Spine3) and R_butt / L_butt (parented to Bip01 Pelvis) to a female
// skeleton.  The call is idempotent: a second call never adds a duplicate bone.
//
// The bones intentionally carry no animation list: MoveBoneMangement() derives
// their frame matrices from the parent bone, which keeps them locked to the
// chest / pelvis through every animation while costing 0 bytes of keyframe RAM.
BOOL CSkeletonObject::EnsureFemaleAnatomyBones(CString spineParentName, CString pelvisParentName)
{
	if(!m_bonesList)
		return FALSE;

	if(!m_bonesList->m_directAccessArray)
		m_bonesList->CompileDirectAccess();

	// already installed?
	if(FindBoneOnListByName(m_bonesList, "R_boob") &&
	   FindBoneOnListByName(m_bonesList, "L_boob") &&
	   FindBoneOnListByName(m_bonesList, "R_butt") &&
	   FindBoneOnListByName(m_bonesList, "L_butt"))
	{
		m_bonesList->CompileDirectAccess();
		return TRUE;
	}

	// chest parent: Bip01 Spine3 -> Bip01 Spine2 -> Bip01 Spine1 -> Bip01 Spine
	static const char * spineFallbacks[] = { "Bip01 Spine2", "Bip01 Spine1", "Bip01 Spine", "Bip01 Chest" };
	CBoneObject * spineParent = ResolveBoneByCandidateNames(m_bonesList, spineParentName, spineFallbacks, 4);

	// pelvis parent: Bip01 Pelvis -> Bip01 Spine -> Bip01
	static const char * pelvisFallbacks[] = { "Bip01 Spine", "Bip01" };
	CBoneObject * pelvisParent = ResolveBoneByCandidateNames(m_bonesList, pelvisParentName, pelvisFallbacks, 2);

	if(!spineParent || !pelvisParent || spineParent == pelvisParent)
		return FALSE;                       // not a biped style skeleton we understand

	// ------------------------------------------------------------------
	// Anatomical reference frame: +X is the model's LEFT side (matching the
	// rig's L / R bone naming), +Z is the front (the sculpt uses the same
	// convention), Y is up.
	// ------------------------------------------------------------------
	D3DVECTOR pelvisPos, spinePos, neckPos;
	GetBoneMatrixTranslation(pelvisParent->m_startPosition, pelvisPos);
	GetBoneMatrixTranslation(spineParent->m_startPosition, spinePos);

	CBoneObject * neckBone = FindBoneOnListByName(m_bonesList, "Bip01 Neck");
	if(!neckBone)
		neckBone = FindBoneOnListByName(m_bonesList, "Bip01 Head");
	GetBoneMatrixTranslation(neckBone ? neckBone->m_startPosition : spineParent->m_startPosition, neckPos);

	float spineHeight = neckPos.y - pelvisPos.y;
	if(spineHeight < 0.1f)
		spineHeight = fabsf(neckPos.y - pelvisPos.y);
	if(spineHeight < 0.1f)
		spineHeight = 1.0f;                 // degenerate rig: keep the maths finite

	float leftSign = 1.0f;                  // direction of the model's left side
	float hipHalfSpan = 0.0f;
	CBoneObject * leftThigh  = FindBoneOnListByName(m_bonesList, "Bip01 L Thigh");
	CBoneObject * rightThigh = FindBoneOnListByName(m_bonesList, "Bip01 R Thigh");
	if(leftThigh && rightThigh)
	{
		D3DVECTOR leftPos, rightPos;
		GetBoneMatrixTranslation(leftThigh->m_startPosition, leftPos);
		GetBoneMatrixTranslation(rightThigh->m_startPosition, rightPos);
		hipHalfSpan = 0.5f * fabsf(leftPos.x - rightPos.x);
		if((leftPos.x - rightPos.x) < 0.0f)
			leftSign = -1.0f;
	}
	if(hipHalfSpan < 0.05f * spineHeight)
		hipHalfSpan = 0.22f * spineHeight;  // human-ish proportion fallback

	// Same heights the female sculpt uses for the bust and the glutes.
	float bustY  = spinePos.y  + 0.06f * spineHeight;
	float gluteY = pelvisPos.y - 0.03f * spineHeight;

	float boobSpan  = 0.34f * hipHalfSpan;
	float boobFront = 0.12f * spineHeight;
	float gluteSpan = 0.30f * hipHalfSpan;
	float gluteBack = 0.12f * spineHeight;

	D3DVECTOR boobLeftPos, boobRightPos, buttLeftPos, buttRightPos;
	boobLeftPos.x  = spinePos.x + leftSign * boobSpan;   boobLeftPos.y  = bustY;   boobLeftPos.z  = spinePos.z + boobFront;
	boobRightPos.x = spinePos.x - leftSign * boobSpan;   boobRightPos.y = bustY;   boobRightPos.z = spinePos.z + boobFront;
	buttLeftPos.x  = pelvisPos.x + leftSign * gluteSpan; buttLeftPos.y  = gluteY;  buttLeftPos.z  = pelvisPos.z - gluteBack;
	buttRightPos.x = pelvisPos.x - leftSign * gluteSpan; buttRightPos.y = gluteY;  buttRightPos.z = pelvisPos.z - gluteBack;

	// ------------------------------------------------------------------
	// Create the missing bones (a partial set - e.g. only the boobs - is
	// completed without touching the bones that are already there).
	// ------------------------------------------------------------------
	if(!FindBoneOnListByName(m_bonesList, "R_boob"))
		CreateAnchoredHelperBone(m_bonesList, "R_boob", spineParent, boobRightPos);
	if(!FindBoneOnListByName(m_bonesList, "L_boob"))
		CreateAnchoredHelperBone(m_bonesList, "L_boob", spineParent, boobLeftPos);
	if(!FindBoneOnListByName(m_bonesList, "R_butt"))
		CreateAnchoredHelperBone(m_bonesList, "R_butt", pelvisParent, buttRightPos);
	if(!FindBoneOnListByName(m_bonesList, "L_butt"))
		CreateAnchoredHelperBone(m_bonesList, "L_butt", pelvisParent, buttLeftPos);

	m_bonesList->CompileDirectAccess();

	return TRUE;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------Parent driven (keyless) bone frames------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
// TRUE when the bone owns animation data for the requested animation index,
// i.e. SetBoneToAnimationFrame() would find a frame to interpolate towards.
bool CSkeletonObject::BoneHasAnimationFrame(CBoneObject * bonePtr, int animationIndex)
{
	if(!bonePtr || !bonePtr->m_animationList)
		return false;

	if(bonePtr->m_animationList->IsEmpty())
		return false;

	if(bonePtr->m_animationList->FindIndex(animationIndex) == NULL)
		return false;

	return true;
}

// Finds the bone a keyless bone hangs off (used by the parent driven frame).
CBoneObject * CSkeletonObject::GetAnatomyParentBone(CBoneObject * bonePtr)
{
	if(!bonePtr || !m_bonesList)
		return NULL;

	if(!m_bonesList->m_directAccessArray)
		m_bonesList->CompileDirectAccess();

	// The hiarchy link is the fast path: the parent frame was set by
	// BuildHiarchy() or by EnsureFemaleAnatomyBones().
	if(bonePtr->m_boneFrame && bonePtr->m_boneFrame->List_parentFrame)
	{
		CFrameObj * parentFrame = bonePtr->m_boneFrame->List_parentFrame;
		for(int boneLoop = 0; boneLoop < m_bonesList->m_directAccessArrayCount; boneLoop++)
		{
			CBoneObject * candidate = m_bonesList->m_directAccessArray[boneLoop].object;
			if(candidate && candidate != bonePtr && candidate->m_boneFrame == parentFrame)
				return candidate;
		}
	}

	// Fall back to the parent name carried by the bone.
	if(bonePtr->m_parentName.GetLength() > 0)
	{
		CBoneObject * named = FindBoneOnListByName(m_bonesList, bonePtr->m_parentName);
		if(named && named != bonePtr)
			return named;
	}

	return NULL;
}

// Drives a bone that has no keyframes of its own from its parent bone:
//     frame = startPosition * ( inverse(parentStart) * parentFrame )
// The bone rigidly follows the parent (chest / pelvis) for every animation and
// its vertices can never be dragged to the origin.
void CSkeletonObject::ApplyParentDrivenBoneFrame(CBoneObject * bonePtr)
{
	if(!bonePtr || !bonePtr->m_boneFrame)
		return;

	CBoneObject * parentBone = GetAnatomyParentBone(bonePtr);
	if(!parentBone || !parentBone->m_boneFrame)
	{
		// No parent to follow: hold the bind pose instead of collapsing.
		bonePtr->m_boneFrame->List_frameMatrix = bonePtr->m_startPosition;
	}
	else
	{
		D3DMATRIX invParentStart, parentDelta;
		D3DMATRIX parentStart = parentBone->m_startPosition;
		GL_matrixFunctions.D3DMath_MatrixInvert(invParentStart, parentStart);
		GL_matrixFunctions.fastMatrixMultiply(parentDelta, invParentStart, parentBone->m_boneFrame->List_frameMatrix);
		GL_matrixFunctions.fastMatrixMultiply(bonePtr->m_boneFrame->List_frameMatrix, bonePtr->m_startPosition, parentDelta);
		bonePtr->m_boneFrame->List_initMatrix = bonePtr->m_boneFrame->List_frameMatrix;
	}

	// Keep the interpolation state coherent for anything that reads it back.
	bonePtr->m_lastPosition.x = bonePtr->m_targetPosition.x = bonePtr->m_boneFrame->List_frameMatrix._41;
	bonePtr->m_lastPosition.y = bonePtr->m_targetPosition.y = bonePtr->m_boneFrame->List_frameMatrix._42;
	bonePtr->m_lastPosition.z = bonePtr->m_targetPosition.z = bonePtr->m_boneFrame->List_frameMatrix._43;
	bonePtr->m_lastPositionsAreInitialized = TRUE;
	bonePtr->m_currentVisibility = bonePtr->m_targetVisibility;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//----------Skin weighting to the female anatomy bones--------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
// Rewrites the vertex assignments of one visual unit so the vertices listed in
// targetBone[] / targetWeight[] additionally ride R_boob / L_boob / R_butt /
// L_butt.  Both skinning flavours are handled in place:
//   m_blendedType == 0 : every vertex keeps its rigid bone (weight 1 - w) and
//                        gains the anatomy bone (weight w); the whole visual
//                        unit is upgraded to the blended (type 1) system.
//   m_blendedType == 1 : the anatomy bone is merged into the existing weights,
//                        all other bones are renormalised to (1 - w).
// Weights always sum up to exactly 1.0 and the offsets are built from the rest
// matrices, so at the bind pose every bone reconstructs the rest position
// (M_world * inverse(M_rest) == identity for every bone) and the mesh does not
// move a millimetre.
int CSkeletonObject::BindFemaleAnatomyWeightsOnVisObj(CDeformableVisObj * defVis,
													  const int * targetBone,
													  const float * targetWeight,
													  const float * restX,
													  const float * restY,
													  const float * restZ,
													  const float * restNX,
													  const float * restNY,
													  const float * restNZ)
{
	if(!defVis || !targetBone || !targetWeight || defVis->m_assignmentCount <= 0)
		return 0;

	if(!m_bonesList)
		return 0;

	if(!m_bonesList->m_directAccessArray)
		m_bonesList->CompileDirectAccess();

	const int boneCount = m_bonesList->m_directAccessArrayCount;

	if(defVis->m_blendedType == 0)
	{
		// ------------------------------------------------------------------
		// Rigid (one bone per vertex) -> blended weights
		// ------------------------------------------------------------------
		if(!defVis->m_vertexAssignments)
			return 0;

		VertexAssignmentBlended * newAssignments = new VertexAssignmentBlended[defVis->m_assignmentCount];
		ZeroMemory(newAssignments, sizeof(VertexAssignmentBlended) * defVis->m_assignmentCount);

		int reboundCount = 0;
		for(int assignLoop = 0; assignLoop < defVis->m_assignmentCount; assignLoop++)
		{
			VertexAssignment & source = defVis->m_vertexAssignments[assignLoop];
			VertexAssignmentBlended & destination = newAssignments[assignLoop];

			int anatomyBoneIndex = targetBone[assignLoop];
			float anatomyWeight = (anatomyBoneIndex >= 0) ? targetWeight[assignLoop] : 0.0f;
			if(anatomyWeight < 0.0f)
				anatomyWeight = 0.0f;
			if(anatomyWeight > 0.995f)
				anatomyWeight = 0.995f;

			const int influenceCount = (anatomyWeight > 0.0001f) ? 2 : 1;

			destination.boneIndexesCount = (byte)influenceCount;
			destination.boneIndexes      = new byte[influenceCount];
			destination.weightsToBone    = new float[influenceCount];
			destination.offsetVects      = new ABVERTEX0L[influenceCount];
			ZeroMemory(destination.offsetVects, sizeof(ABVERTEX0L) * influenceCount);
			destination.vertexIndex      = source.vertexIndex;
			destination.carriedCount     = source.carriedCount;
			destination.vertexesCarried  = NULL;
			if(source.carriedCount > 0 && source.vertexesCarried)
			{
				destination.vertexesCarried = new short[source.carriedCount];
				CopyMemory(destination.vertexesCarried, source.vertexesCarried, sizeof(short) * source.carriedCount);
			}

			// The rigid bone's offset is exactly the stored bone local vertex.
			destination.boneIndexes[0]   = (byte)((source.boneIndex >= 0) ? source.boneIndex : 0);
			destination.weightsToBone[0] = 1.0f - anatomyWeight;
			if(defVis->m_originalArrayVector && source.vertexIndex >= 0)
			{
				const ABVERTEX0L & stored = defVis->m_originalArrayVector[source.vertexIndex];
				destination.offsetVects[0].x  = stored.x;
				destination.offsetVects[0].y  = stored.y;
				destination.offsetVects[0].z  = stored.z;
				destination.offsetVects[0].nx = stored.nx;
				destination.offsetVects[0].ny = stored.ny;
				destination.offsetVects[0].nz = stored.nz;
			}

			if(influenceCount > 1 && anatomyBoneIndex >= 0 && anatomyBoneIndex < boneCount)
			{
				CBoneObject * anatomyPtr = m_bonesList->m_directAccessArray[anatomyBoneIndex].object;
				if(anatomyPtr)
				{
					destination.boneIndexes[1]   = (byte)anatomyBoneIndex;
					destination.weightsToBone[1] = anatomyWeight;
					MakeBoneLocalOffset(anatomyPtr->m_startPosition,
										restX[assignLoop], restY[assignLoop], restZ[assignLoop],
										restNX[assignLoop], restNY[assignLoop], restNZ[assignLoop],
										destination.offsetVects[1]);
					reboundCount++;
				}
				else
				{
					destination.boneIndexesCount = 1;
					destination.weightsToBone[0] = 1.0f;
				}
			}
		}

		// Free the rigid storage, then switch the unit over to blended weights.
		for(int assignLoop = 0; assignLoop < defVis->m_assignmentCount; assignLoop++)
		{
			if(defVis->m_vertexAssignments[assignLoop].vertexesCarried)
			{
				delete [] defVis->m_vertexAssignments[assignLoop].vertexesCarried;
				defVis->m_vertexAssignments[assignLoop].vertexesCarried = 0;
			}
		}
		delete [] defVis->m_vertexAssignments;
		defVis->m_vertexAssignments = NULL;

		defVis->m_blendedVertexAssignments = newAssignments;
		defVis->m_blendedType = 1;

		return reboundCount;
	}

	if(defVis->m_blendedType == 1)
	{
		// ------------------------------------------------------------------
		// Blended weights -> merge the anatomy bone in
		// ------------------------------------------------------------------
		if(!defVis->m_blendedVertexAssignments)
			return 0;

		int reboundCount = 0;
		for(int assignLoop = 0; assignLoop < defVis->m_assignmentCount; assignLoop++)
		{
			VertexAssignmentBlended & entry = defVis->m_blendedVertexAssignments[assignLoop];
			if(entry.boneIndexesCount <= 0 || !entry.boneIndexes || !entry.weightsToBone || !entry.offsetVects)
				continue;

			int anatomyBoneIndex = targetBone[assignLoop];
			float anatomyWeight = (anatomyBoneIndex >= 0) ? targetWeight[assignLoop] : 0.0f;
			if(anatomyBoneIndex < 0 || anatomyBoneIndex >= boneCount || anatomyWeight <= 0.0001f)
				continue;

			CBoneObject * anatomyPtr = m_bonesList->m_directAccessArray[anatomyBoneIndex].object;
			if(!anatomyPtr)
				continue;

			if(anatomyWeight > 0.995f)
				anatomyWeight = 0.995f;

			// already weighted to this anatomy bone (e.g. the auto rigger got
			// there first)?  Then only its weight is refreshed.
			int existingSlot = -1;
			float otherWeightSum = 0.0f;
			for(int boneLoop = 0; boneLoop < entry.boneIndexesCount; boneLoop++)
			{
				if(entry.boneIndexes[boneLoop] == (byte)anatomyBoneIndex)
					existingSlot = boneLoop;
				else
					otherWeightSum += entry.weightsToBone[boneLoop];
			}

			const int newInfluenceCount = (existingSlot >= 0) ? entry.boneIndexesCount : (entry.boneIndexesCount + 1);
			if(newInfluenceCount > 250)
				continue;

			byte       * newBones   = new byte[newInfluenceCount];
			float      * newWeights = new float[newInfluenceCount];
			ABVERTEX0L * newOffsets = new ABVERTEX0L[newInfluenceCount];
			ZeroMemory(newOffsets, sizeof(ABVERTEX0L) * newInfluenceCount);

			// Normalise the remaining bones so the total stays exactly 1.0.
			float otherScale = 0.0f;
			if(otherWeightSum > 0.0001f)
				otherScale = (1.0f - anatomyWeight) / otherWeightSum;

			int destinationSlot = 0;
			for(int boneLoop = 0; boneLoop < entry.boneIndexesCount; boneLoop++)
			{
				if(boneLoop == existingSlot)
					continue;               // rewritten below

				newBones[destinationSlot]   = entry.boneIndexes[boneLoop];
				newWeights[destinationSlot] = entry.weightsToBone[boneLoop] * otherScale;
				newOffsets[destinationSlot] = entry.offsetVects[boneLoop];
				destinationSlot++;
			}

			newBones[destinationSlot]   = (byte)anatomyBoneIndex;
			newWeights[destinationSlot] = (otherWeightSum > 0.0001f) ? anatomyWeight : 1.0f;
			MakeBoneLocalOffset(anatomyPtr->m_startPosition,
								restX[assignLoop], restY[assignLoop], restZ[assignLoop],
								restNX[assignLoop], restNY[assignLoop], restNZ[assignLoop],
								newOffsets[destinationSlot]);
			destinationSlot++;

			delete [] entry.boneIndexes;
			delete [] entry.weightsToBone;
			delete [] entry.offsetVects;

			entry.boneIndexes      = newBones;
			entry.weightsToBone    = newWeights;
			entry.offsetVects      = newOffsets;
			entry.boneIndexesCount = (byte)newInfluenceCount;

			reboundCount++;
		}

		return reboundCount;
	}

	return 0;                                 // tangent space variation (type 2) is left alone
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//----------------1-click auto rig for imports----------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
// Auto scales / aligns a freshly imported mesh to this character skeleton.
// Full body imports (at least a quarter of the skeleton's height) are scaled
// and centred on the character; small props and armour pieces are assumed to be
// authored in the character's own space and are only re-skinned.
BOOL CSkeletonObject::FitImportedMeshToSkeleton(CEXMeshObj * mesh)
{
	if(!mesh || !m_bonesList || m_bonesList->IsEmpty())
		{ CAssimpModelImporter::SetLastError("Fit step: no model or no skeleton."); return FALSE; }

	ABVERTEX * vertexArray = NULL;
	int vertexCount = 0;
	mesh->m_abVertexArray.GetABVERTEXStyle(&vertexArray, &vertexCount);
	if(!vertexArray || vertexCount <= 0)
	{
		if(vertexArray)
			delete [] vertexArray;
		{ CAssimpModelImporter::SetLastError("Fit step: the imported model has no readable vertices."); return FALSE; }
	}

	// ---- character reference box -------------------------------------
	float skeletonMinX, skeletonMaxX, skeletonMinY, skeletonMaxY, skeletonMinZ, skeletonMaxZ;
	if(!ComputeSkinBounds(skeletonMinX, skeletonMaxX, skeletonMinY, skeletonMaxY, skeletonMinZ, skeletonMaxZ))
	{
		if(!ComputeBoneRestBounds(m_bonesList, skeletonMinX, skeletonMaxX, skeletonMinY, skeletonMaxY, skeletonMinZ, skeletonMaxZ))
		{
			delete [] vertexArray;
			{ CAssimpModelImporter::SetLastError("Fit step: could not measure the character skeleton (no skin and no bone rest positions)."); return FALSE; }
		}
	}

	const float skeletonHeight = skeletonMaxY - skeletonMinY;
	if(skeletonHeight <= 0.001f)
	{
		delete [] vertexArray;
		{ CAssimpModelImporter::SetLastError("Fit step: the character skeleton has no height."); return FALSE; }
	}

	// ---- imported mesh box -------------------------------------------
	float meshMinX = vertexArray[0].x, meshMaxX = vertexArray[0].x;
	float meshMinY = vertexArray[0].y, meshMaxY = vertexArray[0].y;
	float meshMinZ = vertexArray[0].z, meshMaxZ = vertexArray[0].z;
	for(int vertexLoop = 1; vertexLoop < vertexCount; vertexLoop++)
	{
		const ABVERTEX & vertex = vertexArray[vertexLoop];
		if(vertex.x < meshMinX) meshMinX = vertex.x;
		if(vertex.x > meshMaxX) meshMaxX = vertex.x;
		if(vertex.y < meshMinY) meshMinY = vertex.y;
		if(vertex.y > meshMaxY) meshMaxY = vertex.y;
		if(vertex.z < meshMinZ) meshMinZ = vertex.z;
		if(vertex.z > meshMaxZ) meshMaxZ = vertex.z;
	}

	const float meshHeight = meshMaxY - meshMinY;
	if(meshHeight <= 0.0001f)
	{
		delete [] vertexArray;
		{ CAssimpModelImporter::SetLastError("Fit step: the imported model has no height (it is flat on the up axis)."); return FALSE; }
	}

	float scale = 1.0f;
	float offsetX = 0.0f, offsetY = 0.0f, offsetZ = 0.0f;
	if(meshHeight >= 0.25f * skeletonHeight)
	{
		// full body import: match the character's height and centre it
		scale = skeletonHeight / meshHeight;
		if(scale < 0.01f)  scale = 0.01f;
		if(scale > 100.0f) scale = 100.0f;

		const float meshCenterX     = 0.5f * (meshMinX + meshMaxX);
		const float meshCenterZ     = 0.5f * (meshMinZ + meshMaxZ);
		const float skeletonCenterX = 0.5f * (skeletonMinX + skeletonMaxX);
		const float skeletonCenterZ = 0.5f * (skeletonMinZ + skeletonMaxZ);

		offsetX = skeletonCenterX - meshCenterX * scale;
		offsetZ = skeletonCenterZ - meshCenterZ * scale;
		offsetY = skeletonMinY - meshMinY * scale;     // stand on the same floor
	}

	// ---- transform the geometry in place -----------------------------
	int   vertexSize   = mesh->m_abVertexArray.GetVertexSize();
	byte * vertexBuffer = (byte *)mesh->m_abVertexArray.GetVertexBuf();
	if(!vertexBuffer || vertexSize < (int)sizeof(ABVERTEX0L))
	{
		delete [] vertexArray;
		{ CAssimpModelImporter::SetLastError("Fit step: the imported model has no vertex data in memory to move."); return FALSE; }
	}

	for(int vertexLoop = 0; vertexLoop < vertexCount; vertexLoop++)
	{
		ABVERTEX0L * vertex = (ABVERTEX0L *)&vertexBuffer[vertexLoop * vertexSize];
		vertex->x = vertex->x * scale + offsetX;
		vertex->y = vertex->y * scale + offsetY;
		vertex->z = vertex->z * scale + offsetZ;

		// uniform scale + translation: the normal only needs to stay unit length
		const float normalLength = sqrtf(vertex->nx * vertex->nx + vertex->ny * vertex->ny + vertex->nz * vertex->nz);
		if(normalLength > 1e-5f)
		{
			vertex->nx /= normalLength;
			vertex->ny /= normalLength;
			vertex->nz /= normalLength;
		}
	}

	delete [] vertexArray;

	// ---- refresh the cached geometry state ---------------------------
	mesh->GetBox();
	mesh->CalculateBoundingRadius();
	mesh->m_abVertexArray.Invalidate();

	return TRUE;
}

// Builds the 4 bone inverse distance skin weights of an auto rigged mesh: every
// vertex is weighted to its four closest bone segments, with the weight of a
// bone falling off as 1 / distance.  The offsets are stored in each bone's own
// rest space, which is what the engine's blended deformation expects
// (m_blendedType == 1, VertexAssignmentBlended::offsetVects).
CDeformableVisObj * CSkeletonObject::BuildAutoRigSkinWeights(CEXMeshObj * mesh)
{
	if(!mesh || !m_bonesList)
		{ CAssimpModelImporter::SetLastError("Skin step: the model has no readable vertices, or more than 32000 vertices, or the skeleton has 0 or over 250 bones."); return NULL; }

	ABVERTEX * vertexArray = NULL;
	int vertexCount = 0;
	mesh->m_abVertexArray.GetABVERTEXStyle(&vertexArray, &vertexCount);
	if(!vertexArray || vertexCount <= 0)
	{
		if(vertexArray)
			delete [] vertexArray;
		{ CAssimpModelImporter::SetLastError("Skin step: the model has no readable vertices, or more than 32000 vertices, or the skeleton has 0 or over 250 bones."); return NULL; }
	}

	if(!m_bonesList->m_directAccessArray)
		m_bonesList->CompileDirectAccess();

	const int boneCount = m_bonesList->m_directAccessArrayCount;
	if(boneCount <= 0 || boneCount > 250 || vertexCount > 32000)
	{
		// The engine stores bone indices in a byte and vertex indices in a short.
		delete [] vertexArray;
		{ CAssimpModelImporter::SetLastError("Skin step: the model has no readable vertices, or more than 32000 vertices, or the skeleton has 0 or over 250 bones."); return NULL; }
	}

	// ---- rest segments of every usable bone --------------------------
	D3DVECTOR * segmentStart = new D3DVECTOR[boneCount];
	D3DVECTOR * segmentEnd   = new D3DVECTOR[boneCount];
	bool      * boneUsable   = new bool[boneCount];
	for(int boneLoop = 0; boneLoop < boneCount; boneLoop++)
	{
		boneUsable[boneLoop] = false;
		segmentStart[boneLoop].x = segmentStart[boneLoop].y = segmentStart[boneLoop].z = 0.0f;
		segmentEnd[boneLoop].x   = segmentEnd[boneLoop].y   = segmentEnd[boneLoop].z   = 0.0f;

		CBoneObject * bonePtr = m_bonesList->m_directAccessArray[boneLoop].object;
		if(!bonePtr || !IsSkinningCandidateBone(bonePtr->m_boneName))
			continue;

		GetBoneMatrixTranslation(bonePtr->m_startPosition, segmentEnd[boneLoop]);

		CBoneObject * parentBone = GetAnatomyParentBone(bonePtr);
		if(parentBone)
			GetBoneMatrixTranslation(parentBone->m_startPosition, segmentStart[boneLoop]);
		else
			segmentStart[boneLoop] = segmentEnd[boneLoop];

		boneUsable[boneLoop] = true;
	}

	// ---- per vertex influences ---------------------------------------
	CDeformableVisObj * skinObj = new CDeformableVisObj();
	skinObj->m_mesh = mesh;                    // the visual unit owns the mesh
	skinObj->m_blendedType = 1;
	skinObj->m_assignmentCount = vertexCount;
	skinObj->m_blendedVertexAssignments = new VertexAssignmentBlended[vertexCount];
	ZeroMemory(skinObj->m_blendedVertexAssignments, sizeof(VertexAssignmentBlended) * vertexCount);
	skinObj->m_referencedMemory = FALSE;

	const int maxInfluences = 4;
	int   closestBone[4];
	float closestDistance[4];
	float inverseWeight[4];

	for(int vertexLoop = 0; vertexLoop < vertexCount; vertexLoop++)
	{
		D3DVECTOR point;
		point.x = vertexArray[vertexLoop].x;
		point.y = vertexArray[vertexLoop].y;
		point.z = vertexArray[vertexLoop].z;

		for(int influenceLoop = 0; influenceLoop < maxInfluences; influenceLoop++)
		{
			closestBone[influenceLoop] = -1;
			closestDistance[influenceLoop] = 1e30f;
		}

		for(int boneLoop = 0; boneLoop < boneCount; boneLoop++)
		{
			if(!boneUsable[boneLoop])
				continue;

			const float distance = PointToSegmentDistanceSquared(point, segmentStart[boneLoop], segmentEnd[boneLoop]);

			for(int influenceLoop = 0; influenceLoop < maxInfluences; influenceLoop++)
			{
				if(distance < closestDistance[influenceLoop])
				{
					for(int shiftLoop = maxInfluences - 1; shiftLoop > influenceLoop; shiftLoop--)
					{
						closestDistance[shiftLoop] = closestDistance[shiftLoop - 1];
						closestBone[shiftLoop]     = closestBone[shiftLoop - 1];
					}
					closestDistance[influenceLoop] = distance;
					closestBone[influenceLoop]     = boneLoop;
					break;
				}
			}
		}

		// inverse distance weights, normalised so they sum up to exactly 1.0
		float inverseWeightSum = 0.0f;
		int   influenceCount = 0;
		for(int influenceLoop = 0; influenceLoop < maxInfluences; influenceLoop++)
		{
			inverseWeight[influenceLoop] = 0.0f;
			if(closestBone[influenceLoop] < 0)
				continue;

			inverseWeight[influenceLoop] = 1.0f / (closestDistance[influenceLoop] + 1e-4f);
			inverseWeightSum += inverseWeight[influenceLoop];
			influenceCount++;
		}

		VertexAssignmentBlended & entry = skinObj->m_blendedVertexAssignments[vertexLoop];
		entry.vertexIndex     = (short)vertexLoop;
		entry.carriedCount    = 0;
		entry.vertexesCarried = NULL;

		if(influenceCount <= 0 || inverseWeightSum <= 0.0f)
		{
			// Should never happen (the skeleton always has usable bones), but a
			// vertex without an assignment would collapse to the origin, so bind
			// it rigidly to the first usable bone instead.
			entry.boneIndexesCount = 1;
			entry.boneIndexes   = new byte[1];
			entry.weightsToBone = new float[1];
			entry.offsetVects   = new ABVERTEX0L[1];
			ZeroMemory(entry.offsetVects, sizeof(ABVERTEX0L));

			int fallbackBone = 0;
			for(int boneLoop = 0; boneLoop < boneCount; boneLoop++)
				if(boneUsable[boneLoop])
				{
					fallbackBone = boneLoop;
					break;
				}

			entry.boneIndexes[0]   = (byte)fallbackBone;
			entry.weightsToBone[0] = 1.0f;

			CBoneObject * fallbackPtr = m_bonesList->m_directAccessArray[fallbackBone].object;
			if(fallbackPtr)
				MakeBoneLocalOffset(fallbackPtr->m_startPosition,
									point.x, point.y, point.z,
									vertexArray[vertexLoop].nx, vertexArray[vertexLoop].ny, vertexArray[vertexLoop].nz,
									entry.offsetVects[0]);
			continue;
		}

		entry.boneIndexesCount = (byte)influenceCount;
		entry.boneIndexes   = new byte[influenceCount];
		entry.weightsToBone = new float[influenceCount];
		entry.offsetVects   = new ABVERTEX0L[influenceCount];
		ZeroMemory(entry.offsetVects, sizeof(ABVERTEX0L) * influenceCount);

		int destinationSlot = 0;
		for(int influenceLoop = 0; influenceLoop < maxInfluences; influenceLoop++)
		{
			if(closestBone[influenceLoop] < 0)
				continue;

			CBoneObject * bonePtr = m_bonesList->m_directAccessArray[closestBone[influenceLoop]].object;
			if(!bonePtr)
				continue;

			entry.boneIndexes[destinationSlot]   = (byte)closestBone[influenceLoop];
			entry.weightsToBone[destinationSlot] = inverseWeight[influenceLoop] / inverseWeightSum;

			MakeBoneLocalOffset(bonePtr->m_startPosition,
								point.x, point.y, point.z,
								vertexArray[vertexLoop].nx, vertexArray[vertexLoop].ny, vertexArray[vertexLoop].nz,
								entry.offsetVects[destinationSlot]);
			destinationSlot++;
		}

		if(destinationSlot != influenceCount)
		{
			// Re-normalise if a bone vanished between the two passes.
			float weightSum = 0.0f;
			for(int slotLoop = 0; slotLoop < destinationSlot; slotLoop++)
				weightSum += entry.weightsToBone[slotLoop];
			if(weightSum > 0.0001f)
				for(int slotLoop = 0; slotLoop < destinationSlot; slotLoop++)
					entry.weightsToBone[slotLoop] /= weightSum;
			entry.boneIndexesCount = (byte)destinationSlot;
		}
	}

	delete [] vertexArray;
	delete [] segmentStart;
	delete [] segmentEnd;
	delete [] boneUsable;

	return skinObj;
}

// Appends a deformable mesh (a new configuration) to the skeleton's alternate
// configuration list.  Returns the configuration index or -1.
int CSkeletonObject::AttachDeformableMeshToSlot(CDeformableMesh * defMesh, BOOL isArmorVariation)
{
	if(!defMesh)
		return -1;

	if(!m_alternateConfigurations)
		m_alternateConfigurations = new CAlterUnitDBObjectList();

	POSITION slotPos = m_alternateConfigurations->FindIndex(0);
	CAlterUnitDBObject * slot = NULL;
	if(slotPos)
		slot = (CAlterUnitDBObject *)m_alternateConfigurations->GetAt(slotPos);

	if(!slot)
	{
		slot = new CAlterUnitDBObject();
		slot->m_alterUnitName = "AutoRig";
		slot->m_configurations = new CDeformableMeshList();
		m_alternateConfigurations->AddTail(slot);
	}
	if(!slot->m_configurations)
		slot->m_configurations = new CDeformableMeshList();

	const int configurationIndex = (int)slot->m_configurations->GetCount();
	slot->m_configurations->AddTail(defMesh);
	return configurationIndex;
}

// One click auto rig: scale / align, weight, sculpt and attach an imported mesh
// to this character skeleton.  The call is the whole "Load Entity" ->
// auto rig pipeline: no manual weighting, no bone list editing.
BOOL CSkeletonObject::AutoRigMeshToSkeleton(CEXMeshObj * sourceMesh,
											BOOL isFemale,
											BOOL isArmorVariation,
											LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	if(!sourceMesh || !m_bonesList || m_bonesList->IsEmpty())
		{ CAssimpModelImporter::SetLastError("This entity has no skeleton (bones) to rig onto."); return FALSE; }

	if(sourceMesh->m_abVertexArray.m_vertexCount < 3)
		{ CAssimpModelImporter::SetLastError("The model has no vertices."); return FALSE; }

	if(!m_bonesList->m_directAccessArray)
		m_bonesList->CompileDirectAccess();

	// 1. work on a copy - the caller keeps ownership of its imported mesh
	CEXMeshObj * riggedMesh = NULL;
	if(!sourceMesh->Clone(&riggedMesh, g_pd3dDevice) || !riggedMesh)
		{ CAssimpModelImporter::SetLastError("Could not copy the imported model."); return FALSE; }

	// 2. auto scale + align to the character skeleton
	if(!FitImportedMeshToSkeleton(riggedMesh))
	{
		riggedMesh->SafeDelete();
		delete riggedMesh;
		return FALSE;
	}

	// 3. female skeletons get the anatomy bones before any weight is computed,
	//    so the bust / glute vertices can be weighted to them straight away
	if(isFemale)
		EnsureFemaleAnatomyBones();

	// 4. 4 bone inverse distance skin weights + bone local offsets
	CDeformableVisObj * skinObj = BuildAutoRigSkinWeights(riggedMesh);
	if(!skinObj)
	{
		riggedMesh->SafeDelete();
		delete riggedMesh;
		return FALSE;
	}

	// 5. wrap it into a deformable mesh and attach it to the skeleton
	CDeformableMesh * deformableMesh = new CDeformableMesh();
	deformableMesh->m_lodChain = new CDeformableVisList();
	deformableMesh->m_lodChain->AddTail(skinObj);
	deformableMesh->m_publicDim = (isArmorVariation == FALSE);
	deformableMesh->m_collisionFiltered = TRUE;

	const int configurationIndex = AttachDeformableMeshToSlot(deformableMesh, isArmorVariation);
	if(configurationIndex < 0)
	{
		deformableMesh->SafeDelete();
		delete deformableMesh;
		return FALSE;
	}

	CString dimensionName;
	dimensionName.Format(isArmorVariation ? "AutoRigArmor%d" : "AutoRigBody%d", configurationIndex + 1);
	deformableMesh->m_dimName = dimensionName;

	// 6. give the imported body the feminine shape (bones + weights included)
	if(isFemale)
	{
		float minX, maxX, minY, maxY, minZ, maxZ;
		if(ComputeSkinBounds(minX, maxX, minY, maxY, minZ, maxZ))
			SculptFemaleDeformableMesh(deformableMesh,
									   FEMALE_AUTORIG_SCULPT_SCALE,
									   minX, maxX, minY, maxY, minZ, maxZ);
	}

	// 7. show it right away when the skeleton is the live entity (the editor
	//    already swapped slots for this mesh before, so reuse that path)
	if(m_deformableMeshArrayCount > 0 && m_deformableMeshArrayV2 && g_pd3dDevice)
	{
		if(m_spawnCfg)
			m_spawnCfg[0] = configurationIndex;
		HotSwapDim(0, configurationIndex, this, g_pd3dDevice, FALSE);
	}

	return TRUE;
}

//------------------------------------------------------------------------------------//
// CSkeletonObject::ExportToGltf - characters out to Blender (easy-editor branch)
//
// Writes <name>.gltf + <name>.bin: every bone (named, with its parent), the current
// body parts as skinned meshes (vertex -> bone weights, the same blend the game uses)
// and a material per part that points at "<texture>.png" next to the file.
// Pose = the skeleton's start pose (m_startPosition). Left-handed engine space is
// mirrored on Z into glTF's right-handed space (triangles flipped to match).
// textureNames returns the texture files the caller should save beside the .gltf.
//------------------------------------------------------------------------------------//
namespace
{
	struct GltfSkinVert { float p[3]; float n[3]; float uv[2]; unsigned short j[4]; float w[4]; };
	struct GltfPart { std::vector<GltfSkinVert> v; std::vector<unsigned short> idx; std::string tex; std::string name; int rigidBone; };

	// engine (left-handed, row vectors) -> glTF (right-handed): mirror Z on both sides
	static void GltfMirror(const D3DXMATRIX & in, float out[16])
	{
		static const float s[4] = { 1.0f, 1.0f, -1.0f, 1.0f };
		for(int r = 0; r < 4; r++)
			for(int c = 0; c < 4; c++)
				out[r * 4 + c] = in.m[r][c] * s[r] * s[c];   // row-major row-vector == glTF column-major
	}
	static void GltfXform(const D3DXMATRIX & m, float x, float y, float z, float w, float out[3])
	{
		out[0] = x * m._11 + y * m._21 + z * m._31 + w * m._41;
		out[1] = x * m._12 + y * m._22 + z * m._32 + w * m._42;
		out[2] = x * m._13 + y * m._23 + z * m._33 + w * m._43;
	}
	static std::string GltfEsc(const char * s)
	{
		std::string o;
		for(; *s; s++)
		{
			if(*s == '"' || *s == '\\') o += '\\';
			if((unsigned char)*s < 32) continue;
			o += *s;
		}
		return o;
	}
	static std::string GltfTexBase(const CString & tex)
	{
		CString t = tex;
		int slash = t.ReverseFind('\\'); if(slash < 0) slash = t.ReverseFind('/');
		if(slash >= 0) t = t.Mid(slash + 1);
		int dot = t.ReverseFind('.');
		if(dot > 0) t = t.Left(dot);
		return std::string((const char *)t);
	}
	static void GltfAddWeight(GltfSkinVert & v, int bone, float wt)
	{
		if(wt <= 0.0f || bone < 0) return;
		for(int k = 0; k < 4; k++) if(v.w[k] > 0.0f && v.j[k] == bone) { v.w[k] += wt; return; }
		int lo = 0;
		for(int k = 1; k < 4; k++) if(v.w[k] < v.w[lo]) lo = k;
		if(wt > v.w[lo]) { v.j[lo] = (unsigned short)bone; v.w[lo] = wt; }
	}
}

BOOL CSkeletonObject::ExportToGltf(CString fileName, CStringArray & textureNames, CString & error, BOOL allOutfits)
{
	if(!m_bonesList || m_bonesList->GetCount() == 0)
	{
		error = "This character has no skeleton.";
		return FALSE;
	}
	m_bonesList->CompileDirectAccess();
	const int boneCount = m_bonesList->m_directAccessArrayCount;
	if(boneCount <= 0)
	{
		error = "This character has no bones.";
		return FALSE;
	}

	// bone start-pose matrices (character space) and parents
	std::vector<D3DXMATRIX> boneM(boneCount);
	std::vector<int> parent(boneCount, -1);
	std::vector<CString> names(boneCount);
	for(int b = 0; b < boneCount; b++)
	{
		CBoneObject * bp = m_bonesList->m_directAccessArray[b].object;
		D3DXMatrixIdentity(&boneM[b]);
		if(!bp) continue;
		boneM[b] = D3DXMATRIX((const float *)&bp->m_startPosition.m[0][0]);
		names[b] = bp->m_boneName;
		if(names[b].IsEmpty()) names[b].Format("bone_%d", b);
	}
	for(int b = 0; b < boneCount; b++)
	{
		CBoneObject * bp = m_bonesList->m_directAccessArray[b].object;
		if(!bp || bp->m_parentName.IsEmpty()) continue;
		for(int p = 0; p < boneCount; p++)
			if(p != b && names[p].CompareNoCase(bp->m_parentName) == 0) { parent[b] = p; break; }
	}

	// collect the body parts the character wears now (same choice as ExportToObj)
	// allOutfits: every outfit in every slot, named slot<S>_cfg<C> (+ "_now" for the one worn)
	std::vector<CDeformableVisObj *> visParts;
	std::vector<std::string> visNames;
	auto addVis = [&](CDeformableVisObj * vo, const std::string & nm) {
		if(!vo || !vo->m_mesh || vo->m_mesh->m_abVertexArray.m_vertexCount == 0 || vo->m_mesh->m_abVertexArray.m_indecieCount < 3) return;
		for(size_t i = 0; i < visParts.size(); i++) if(visParts[i]->m_mesh == vo->m_mesh) return;
		visParts.push_back(vo);
		visNames.push_back(nm);
	};
	if(m_alternateConfigurations)
	{
		int slotIdx = 0;
		for(POSITION altPos = m_alternateConfigurations->GetHeadPosition(); altPos != NULL; ++slotIdx)
		{
			CAlterUnitDBObject * altUnit = (CAlterUnitDBObject *)m_alternateConfigurations->GetNext(altPos);
			if(!altUnit || !altUnit->m_configurations || altUnit->m_configurations->IsEmpty()) continue;
			int cfgIdx = (m_spawnCfg && slotIdx < m_deformableMeshArrayCount && m_spawnCfg[slotIdx] >= 0) ? m_spawnCfg[slotIdx] : 0;
			if(allOutfits)
			{
				int c = 0;
				for(POSITION cp = altUnit->m_configurations->GetHeadPosition(); cp != NULL; ++c)
				{
					CDeformableMesh * dmc = (CDeformableMesh *)altUnit->m_configurations->GetNext(cp);
					if(!dmc || !dmc->m_lodChain || dmc->m_lodChain->IsEmpty()) continue;
					char nm[64];
					snprintf(nm, sizeof(nm), "slot%d_cfg%d%s", slotIdx, c, c == cfgIdx ? "_now" : "");
					addVis((CDeformableVisObj *)dmc->m_lodChain->GetHead(), nm);
				}
				continue;
			}
			POSITION cfgPos = altUnit->m_configurations->FindIndex(cfgIdx);
			if(!cfgPos) cfgPos = altUnit->m_configurations->GetHeadPosition();
			CDeformableMesh * dm = cfgPos ? (CDeformableMesh *)altUnit->m_configurations->GetAt(cfgPos) : NULL;
			if(dm && dm->m_lodChain && !dm->m_lodChain->IsEmpty())
				addVis((CDeformableVisObj *)dm->m_lodChain->GetHead(), "");
		}
	}
	if(m_deformableMeshArrayV2)
		for(int i = 0; i < m_deformableMeshArrayCount; i++)
		{
			CDeformableMesh * dm = m_deformableMeshArrayV2[i].m_defMeshVisualUnit;
			if(dm && dm->m_lodChain && !dm->m_lodChain->IsEmpty())
				addVis((CDeformableVisObj *)dm->m_lodChain->GetHead(), "");
		}

	std::vector<GltfPart> parts;
	for(size_t pi = 0; pi < visParts.size(); pi++)
	{
		CDeformableVisObj * vo = visParts[pi];
		CEXMeshObj * mesh = vo->m_mesh;
		ABVERTEX * abv = NULL; int vc = 0;
		mesh->m_abVertexArray.GetABVERTEXStyle(&abv, &vc);
		if(!abv || vc <= 0) continue;

		GltfPart part;
		part.rigidBone = -1;
		part.name = visNames[pi].empty() ? std::string("part_") + std::to_string((int)pi) : visNames[pi];
		part.tex = GltfTexBase(mesh->List_storedTextureFromFile[0]);
		if(mesh->List_storedTextureFromFile[0].IsEmpty() || mesh->List_storedTextureFromFile[0].CompareNoCase("NONE") == 0)
			part.tex.clear();
		part.v.resize(vc);
		std::vector<int> done(vc, 0);
		for(int i = 0; i < vc; i++)
		{
			GltfSkinVert & g = part.v[i];
			memset(&g, 0, sizeof(g));
			g.p[0] = abv[i].x; g.p[1] = abv[i].y; g.p[2] = abv[i].z;   // fallback: as drawn last
			g.n[0] = abv[i].nx; g.n[1] = abv[i].ny; g.n[2] = abv[i].nz;
			g.uv[0] = abv[i].tx0.tu; g.uv[1] = abv[i].tx0.tv;
		}

		// the game's own blend: position = sum(weight * offset * boneMatrix)
		auto apply = [&](int vIndex, short * carried, int carriedCount, int nb, const int * bones, const float * wts,
		                 const float * ox, const float * oy, const float * oz, const float * onx, const float * ony, const float * onz) {
			if(vIndex < 0 || vIndex >= vc) return;
			GltfSkinVert acc;
			memset(&acc, 0, sizeof(acc));
			float total = 0.0f;
			for(int k = 0; k < nb; k++)
			{
				if(bones[k] < 0 || bones[k] >= boneCount || wts[k] <= 0.0f) continue;
				float p[3], n[3];
				GltfXform(boneM[bones[k]], ox[k], oy[k], oz[k], 1.0f, p);
				GltfXform(boneM[bones[k]], onx[k], ony[k], onz[k], 0.0f, n);
				for(int a = 0; a < 3; a++) { acc.p[a] += p[a] * wts[k]; acc.n[a] += n[a] * wts[k]; }
				GltfAddWeight(acc, bones[k], wts[k]);
				total += wts[k];
			}
			if(total <= 0.0f) return;
			auto put = [&](int idx) {
				if(idx < 0 || idx >= vc) return;
				GltfSkinVert & g = part.v[idx];
				memcpy(g.p, acc.p, sizeof(g.p));
				memcpy(g.n, acc.n, sizeof(g.n));
				memcpy(g.j, acc.j, sizeof(g.j));
				memcpy(g.w, acc.w, sizeof(g.w));
				done[idx] = 1;
			};
			put(vIndex);
			for(int c = 0; c < carriedCount; c++) put(carried[c]);
		};

		for(int x = 0; x < vo->m_assignmentCount; x++)
		{
			if(vo->m_blendedType == 0 && vo->m_vertexAssignments && vo->m_originalArrayVector)
			{
				VertexAssignment & a = vo->m_vertexAssignments[x];
				ABVERTEX0L & o = vo->m_originalArrayVector[a.vertexIndex];
				int b = a.boneIndex; float w = 1.0f;
				apply(a.vertexIndex, a.vertexesCarried, a.carriedCount, 1, &b, &w, &o.x, &o.y, &o.z, &o.nx, &o.ny, &o.nz);
			}
			else if(vo->m_blendedType == 1 && vo->m_blendedVertexAssignments)
			{
				VertexAssignmentBlended & a = vo->m_blendedVertexAssignments[x];
				int nb = a.boneIndexesCount;
				std::vector<int> b(nb); std::vector<float> w(nb), ox(nb), oy(nb), oz(nb), onx(nb), ony(nb), onz(nb);
				for(int k = 0; k < nb; k++)
				{
					b[k] = a.boneIndexes[k]; w[k] = a.weightsToBone[k];
					ox[k] = a.offsetVects[k].x; oy[k] = a.offsetVects[k].y; oz[k] = a.offsetVects[k].z;
					onx[k] = a.offsetVects[k].nx; ony[k] = a.offsetVects[k].ny; onz[k] = a.offsetVects[k].nz;
				}
				if(nb > 0) apply(a.vertexIndex, a.vertexesCarried, a.carriedCount, nb, &b[0], &w[0], &ox[0], &oy[0], &oz[0], &onx[0], &ony[0], &onz[0]);
			}
			else if(vo->m_blendedType == 2 && vo->m_blendedVertexAssignmentsWt)
			{
				VertexAssignmentBldTgt & a = vo->m_blendedVertexAssignmentsWt[x];
				int nb = a.boneIndexesCount;
				std::vector<int> b(nb); std::vector<float> w(nb), ox(nb), oy(nb), oz(nb), onx(nb), ony(nb), onz(nb);
				for(int k = 0; k < nb; k++)
				{
					b[k] = a.boneIndexes[k]; w[k] = a.weightsToBone[k];
					ox[k] = a.offsetVects[k].x; oy[k] = a.offsetVects[k].y; oz[k] = a.offsetVects[k].z;
					onx[k] = a.offsetVects[k].nx; ony[k] = a.offsetVects[k].ny; onz[k] = a.offsetVects[k].nz;
				}
				if(nb > 0) apply(a.vertexIndex, a.vertexesCarried, a.carriedCount, nb, &b[0], &w[0], &ox[0], &oy[0], &oz[0], &onx[0], &ony[0], &onz[0]);
			}
		}
		delete [] abv;

		// anything the blend did not reach follows bone 0 (glTF needs a weight on every vertex)
		for(int i = 0; i < vc; i++)
		{
			GltfSkinVert & g = part.v[i];
			float sum = g.w[0] + g.w[1] + g.w[2] + g.w[3];
			if(!done[i] || sum <= 0.0f) { memset(g.j, 0, sizeof(g.j)); memset(g.w, 0, sizeof(g.w)); g.w[0] = 1.0f; continue; }
			for(int k = 0; k < 4; k++) g.w[k] /= sum;
			float len = sqrtf(g.n[0] * g.n[0] + g.n[1] * g.n[1] + g.n[2] * g.n[2]);
			if(len > 0.00001f) for(int a = 0; a < 3; a++) g.n[a] /= len;
		}
		int ic = mesh->m_abVertexArray.m_indecieCount;
		part.idx.assign(mesh->m_abVertexArray.m_indecieArray, mesh->m_abVertexArray.m_indecieArray + ic);
		parts.push_back(part);
	}

	// rigid pieces bolted to a bone (weapons, helmets ...): their vertices are in the bone's space
	for(int b = 0; b < boneCount; b++)
	{
		CBoneObject * bp = m_bonesList->m_directAccessArray[b].object;
		if(!bp || !bp->m_hiarchyMesh || !bp->m_hiarchyMesh->m_lodChain || bp->m_hiarchyMesh->m_lodChain->IsEmpty()) continue;
		CHiarcleVisObj * hv = (CHiarcleVisObj *)bp->m_hiarchyMesh->m_lodChain->GetHead();
		if(!hv || !hv->m_mesh || hv->m_mesh->m_abVertexArray.m_indecieCount < 3) continue;
		ABVERTEX * abv = NULL; int vc = 0;
		hv->m_mesh->m_abVertexArray.GetABVERTEXStyle(&abv, &vc);
		if(!abv || vc <= 0) continue;
		GltfPart part;
		part.rigidBone = b;
		part.name = std::string((const char *)names[b]) + "_piece";
		part.tex = GltfTexBase(hv->m_mesh->List_storedTextureFromFile[0]);
		if(hv->m_mesh->List_storedTextureFromFile[0].IsEmpty() || hv->m_mesh->List_storedTextureFromFile[0].CompareNoCase("NONE") == 0)
			part.tex.clear();
		part.v.resize(vc);
		for(int i = 0; i < vc; i++)
		{
			GltfSkinVert & g = part.v[i];
			memset(&g, 0, sizeof(g));
			GltfXform(boneM[b], abv[i].x, abv[i].y, abv[i].z, 1.0f, g.p);
			GltfXform(boneM[b], abv[i].nx, abv[i].ny, abv[i].nz, 0.0f, g.n);
			g.uv[0] = abv[i].tx0.tu; g.uv[1] = abv[i].tx0.tv;
			g.j[0] = (unsigned short)b; g.w[0] = 1.0f;
		}
		delete [] abv;
		int ic = hv->m_mesh->m_abVertexArray.m_indecieCount;
		part.idx.assign(hv->m_mesh->m_abVertexArray.m_indecieArray, hv->m_mesh->m_abVertexArray.m_indecieArray + ic);
		parts.push_back(part);
	}
	if(parts.empty())
	{
		error = "This character has no body parts to export.";
		return FALSE;
	}

	// ---------------- binary buffer ----------------
	std::vector<unsigned char> bin;
	auto align4 = [&]() { while(bin.size() & 3) bin.push_back(0); };
	auto putBytes = [&](const void * p, size_t n) { const unsigned char * c = (const unsigned char *)p; bin.insert(bin.end(), c, c + n); };
	std::string views, accessors, meshes, materials, textures, images, nodes;
	int viewCount = 0, accCount = 0;
	auto addView = [&](size_t off, size_t len, int target) {
		char b[160];
		if(target) snprintf(b, sizeof(b), "%s{\"buffer\":0,\"byteOffset\":%u,\"byteLength\":%u,\"target\":%d}", viewCount ? "," : "", (unsigned)off, (unsigned)len, target);
		else       snprintf(b, sizeof(b), "%s{\"buffer\":0,\"byteOffset\":%u,\"byteLength\":%u}", viewCount ? "," : "", (unsigned)off, (unsigned)len);
		views += b;
		return viewCount++;
	};
	auto addAcc = [&](int view, int compType, size_t count, const char * type, const char * extra) {
		char b[512];
		snprintf(b, sizeof(b), "%s{\"bufferView\":%d,\"componentType\":%d,\"count\":%u,\"type\":\"%s\"%s}", accCount ? "," : "", view, compType, (unsigned)count, type, extra ? extra : "");
		accessors += b;
		return accCount++;
	};

	// inverse bind matrices
	align4();
	size_t ibmOff = bin.size();
	for(int b = 0; b < boneCount; b++)
	{
		D3DXMATRIX inv;
		if(!D3DXMatrixInverse(&inv, NULL, &boneM[b])) D3DXMatrixIdentity(&inv);
		float m[16]; GltfMirror(inv, m);
		putBytes(m, sizeof(m));
	}
	int ibmAcc = addAcc(addView(ibmOff, bin.size() - ibmOff, 0), 5126, boneCount, "MAT4", NULL);

	// materials: one per distinct texture
	std::vector<std::string> texList;
	auto materialFor = [&](const std::string & tex) -> int {
		if(tex.empty()) return -1;
		for(size_t t = 0; t < texList.size(); t++) if(_stricmp(texList[t].c_str(), tex.c_str()) == 0) return (int)t;
		texList.push_back(tex);
		return (int)texList.size() - 1;
	};

	int meshCount = 0;
	for(size_t pi = 0; pi < parts.size(); pi++)
	{
		GltfPart & part = parts[pi];
		size_t vc = part.v.size();
		float mn[3] = { 1e30f, 1e30f, 1e30f }, mx[3] = { -1e30f, -1e30f, -1e30f };
		align4(); size_t posOff = bin.size();
		for(size_t i = 0; i < vc; i++)
		{
			float p[3] = { part.v[i].p[0], part.v[i].p[1], -part.v[i].p[2] };
			for(int a = 0; a < 3; a++) { if(p[a] < mn[a]) mn[a] = p[a]; if(p[a] > mx[a]) mx[a] = p[a]; }
			putBytes(p, sizeof(p));
		}
		char mm[200];
		snprintf(mm, sizeof(mm), ",\"min\":[%g,%g,%g],\"max\":[%g,%g,%g]", mn[0], mn[1], mn[2], mx[0], mx[1], mx[2]);
		int posAcc = addAcc(addView(posOff, bin.size() - posOff, 34962), 5126, vc, "VEC3", mm);
		align4(); size_t nOff = bin.size();
		for(size_t i = 0; i < vc; i++) { float n[3] = { part.v[i].n[0], part.v[i].n[1], -part.v[i].n[2] }; putBytes(n, sizeof(n)); }
		int nAcc = addAcc(addView(nOff, bin.size() - nOff, 34962), 5126, vc, "VEC3", NULL);
		align4(); size_t uvOff = bin.size();
		for(size_t i = 0; i < vc; i++) putBytes(part.v[i].uv, sizeof(part.v[i].uv));
		int uvAcc = addAcc(addView(uvOff, bin.size() - uvOff, 34962), 5126, vc, "VEC2", NULL);
		align4(); size_t jOff = bin.size();
		for(size_t i = 0; i < vc; i++) putBytes(part.v[i].j, sizeof(part.v[i].j));
		int jAcc = addAcc(addView(jOff, bin.size() - jOff, 34962), 5123, vc, "VEC4", NULL);
		align4(); size_t wOff = bin.size();
		for(size_t i = 0; i < vc; i++) putBytes(part.v[i].w, sizeof(part.v[i].w));
		int wAcc = addAcc(addView(wOff, bin.size() - wOff, 34962), 5126, vc, "VEC4", NULL);
		align4(); size_t iOff = bin.size();
		size_t triCount = part.idx.size() / 3;
		for(size_t t = 0; t < triCount; t++)
		{// mirrored on Z, so flip each triangle to keep the outside facing out
			unsigned short tri[3] = { part.idx[t * 3], part.idx[t * 3 + 2], part.idx[t * 3 + 1] };
			putBytes(tri, sizeof(tri));
		}
		int iAcc = addAcc(addView(iOff, bin.size() - iOff, 34963), 5123, triCount * 3, "SCALAR", NULL);

		int mat = materialFor(part.tex);
		char b[400];
		if(mat >= 0)
			snprintf(b, sizeof(b), "%s{\"name\":\"%s\",\"primitives\":[{\"attributes\":{\"POSITION\":%d,\"NORMAL\":%d,\"TEXCOORD_0\":%d,\"JOINTS_0\":%d,\"WEIGHTS_0\":%d},\"indices\":%d,\"material\":%d}]}",
			         meshCount ? "," : "", GltfEsc(part.name.c_str()).c_str(), posAcc, nAcc, uvAcc, jAcc, wAcc, iAcc, mat);
		else
			snprintf(b, sizeof(b), "%s{\"name\":\"%s\",\"primitives\":[{\"attributes\":{\"POSITION\":%d,\"NORMAL\":%d,\"TEXCOORD_0\":%d,\"JOINTS_0\":%d,\"WEIGHTS_0\":%d},\"indices\":%d}]}",
			         meshCount ? "," : "", GltfEsc(part.name.c_str()).c_str(), posAcc, nAcc, uvAcc, jAcc, wAcc, iAcc);
		meshes += b;
		meshCount++;
	}

	for(size_t t = 0; t < texList.size(); t++)
	{
		char b[400];
		snprintf(b, sizeof(b), "%s{\"name\":\"%s\",\"pbrMetallicRoughness\":{\"baseColorTexture\":{\"index\":%d},\"metallicFactor\":0,\"roughnessFactor\":1}}",
		         t ? "," : "", GltfEsc(texList[t].c_str()).c_str(), (int)t);
		materials += b;
		snprintf(b, sizeof(b), "%s{\"sampler\":0,\"source\":%d}", t ? "," : "", (int)t);
		textures += b;
		snprintf(b, sizeof(b), "%s{\"uri\":\"%s.png\"}", t ? "," : "", GltfEsc(texList[t].c_str()).c_str());
		images += b;
		textureNames.Add(CString(texList[t].c_str()));
	}

	// nodes: 0..boneCount-1 bones, then one node per mesh, then the root
	for(int b = 0; b < boneCount; b++)
	{
		D3DXMATRIX local = boneM[b];
		if(parent[b] >= 0)
		{
			D3DXMATRIX invP;
			if(D3DXMatrixInverse(&invP, NULL, &boneM[parent[b]]))
				local = boneM[b] * invP;   // row vectors: world = local * parentWorld
		}
		float m[16]; GltfMirror(local, m);
		std::string kids;
		for(int c = 0; c < boneCount; c++)
			if(parent[c] == b) { if(!kids.empty()) kids += ","; kids += std::to_string(c); }
		char b2[900];
		snprintf(b2, sizeof(b2), "%s{\"name\":\"%s\",\"matrix\":[%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g]%s%s%s}",
		         b ? "," : "", GltfEsc((const char *)names[b]).c_str(),
		         m[0], m[1], m[2], m[3], m[4], m[5], m[6], m[7], m[8], m[9], m[10], m[11], m[12], m[13], m[14], m[15],
		         kids.empty() ? "" : ",\"children\":[", kids.c_str(), kids.empty() ? "" : "]");
		nodes += b2;
	}
	std::string rootKids;
	for(int b = 0; b < boneCount; b++) if(parent[b] < 0) { if(!rootKids.empty()) rootKids += ","; rootKids += std::to_string(b); }
	for(int m = 0; m < meshCount; m++)
	{
		char b[120];
		snprintf(b, sizeof(b), ",{\"name\":\"%s\",\"mesh\":%d,\"skin\":0}", GltfEsc(parts[m].name.c_str()).c_str(), m);
		nodes += b;
		rootKids += "," + std::to_string(boneCount + m);
	}
	nodes += ",{\"name\":\"EndlessAgesCharacter\",\"children\":[" + rootKids + "]}";
	int rootNode = boneCount + meshCount;

	std::string joints;
	for(int b = 0; b < boneCount; b++) { if(b) joints += ","; joints += std::to_string(b); }

	// ---------------- write files ----------------
	CString binPath = fileName;
	int dot = binPath.ReverseFind('.');
	int slash = binPath.ReverseFind('\\');
	if(dot > slash) binPath = binPath.Left(dot);
	binPath += ".bin";
	CString binName = binPath.Mid(binPath.ReverseFind('\\') + 1);

	FILE * f = NULL;
	if(fopen_s(&f, (const char *)binPath, "wb") != 0 || !f) { error = "Could not write " + binPath; return FALSE; }
	if(!bin.empty()) fwrite(&bin[0], 1, bin.size(), f);
	fclose(f);

	std::string json = "{\"asset\":{\"version\":\"2.0\",\"generator\":\"EngineX (Endless Ages)\"},\"scene\":0,";
	json += "\"scenes\":[{\"nodes\":[" + std::to_string(rootNode) + "]}],";
	json += "\"nodes\":[" + nodes + "],";
	json += "\"skins\":[{\"name\":\"EndlessAgesSkeleton\",\"inverseBindMatrices\":" + std::to_string(ibmAcc) + ",\"joints\":[" + joints + "]}],";
	json += "\"meshes\":[" + meshes + "],";
	if(!texList.empty())
	{
		json += "\"materials\":[" + materials + "],";
		json += "\"textures\":[" + textures + "],";
		json += "\"images\":[" + images + "],";
		json += "\"samplers\":[{\"magFilter\":9729,\"minFilter\":9987,\"wrapS\":10497,\"wrapT\":10497}],";
	}
	json += "\"buffers\":[{\"uri\":\"" + GltfEsc((const char *)binName) + "\",\"byteLength\":" + std::to_string(bin.size()) + "}],";
	json += "\"bufferViews\":[" + views + "],";
	json += "\"accessors\":[" + accessors + "]}";

	if(fopen_s(&f, (const char *)fileName, "wb") != 0 || !f) { error = "Could not write " + fileName; return FALSE; }
	fwrite(json.c_str(), 1, json.size(), f);
	fclose(f);
	return TRUE;
}


//------------------------------------------------------------------//
// "Give All Tops Jiggle": copy the jiggle-bone weights from the part
// that has them (e.g. the bikini top) onto every other piece that can be
// worn in the same body slot (armor, shirts ...). Each vertex takes the
// weights of the closest vertex on the source, fading out with distance -
// the same idea as Blender's weight transfer. Editor copy only.
//------------------------------------------------------------------//
typedef std::vector<std::pair<int, float> > BJWeights;

static void BJ_ReadSkin(CDeformableVisObj * vo, const std::vector<D3DXMATRIX> & boneM,
                        std::vector<D3DXVECTOR3> & pos, std::vector<D3DXVECTOR3> & nrm, std::vector<BJWeights> & w)
{
	const int boneCount = (int)boneM.size();
	int vc = vo->m_mesh->m_abVertexArray.m_vertexCount;
	pos.assign(vc, D3DXVECTOR3(0, 0, 0));
	nrm.assign(vc, D3DXVECTOR3(0, 1, 0));
	w.assign(vc, BJWeights());
	ABVERTEX * abv = NULL; int got = 0;
	vo->m_mesh->m_abVertexArray.GetABVERTEXStyle(&abv, &got);
	for(int i = 0; i < vc && i < got && abv; i++)
	{
		pos[i] = D3DXVECTOR3(abv[i].x, abv[i].y, abv[i].z);
		nrm[i] = D3DXVECTOR3(abv[i].nx, abv[i].ny, abv[i].nz);
	}
	if(abv) delete [] abv;

	auto put = [&](int vIndex, short * carried, int carriedCount, int nb, const int * bones, const float * wts,
	               const float * ox, const float * oy, const float * oz, const float * onx, const float * ony, const float * onz) {
		if(vIndex < 0 || vIndex >= vc) return;
		D3DXVECTOR3 p(0, 0, 0), n(0, 0, 0);
		BJWeights list;
		for(int k = 0; k < nb; k++)
		{
			if(bones[k] < 0 || bones[k] >= boneCount || wts[k] <= 0.0f) continue;
			const D3DXMATRIX & m = boneM[bones[k]];
			p.x += (ox[k] * m._11 + oy[k] * m._21 + oz[k] * m._31 + m._41) * wts[k];
			p.y += (ox[k] * m._12 + oy[k] * m._22 + oz[k] * m._32 + m._42) * wts[k];
			p.z += (ox[k] * m._13 + oy[k] * m._23 + oz[k] * m._33 + m._43) * wts[k];
			n.x += (onx[k] * m._11 + ony[k] * m._21 + onz[k] * m._31) * wts[k];
			n.y += (onx[k] * m._12 + ony[k] * m._22 + onz[k] * m._32) * wts[k];
			n.z += (onx[k] * m._13 + ony[k] * m._23 + onz[k] * m._33) * wts[k];
			list.push_back(std::make_pair(bones[k], wts[k]));
		}
		if(list.empty()) return;
		pos[vIndex] = p; nrm[vIndex] = n; w[vIndex] = list;
		for(int c = 0; c < carriedCount; c++)
			if(carried[c] >= 0 && carried[c] < vc) { pos[carried[c]] = p; nrm[carried[c]] = n; w[carried[c]] = list; }
	};
	for(int x = 0; x < vo->m_assignmentCount; x++)
	{
		if(vo->m_blendedType == 0 && vo->m_vertexAssignments && vo->m_originalArrayVector)
		{
			VertexAssignment & a = vo->m_vertexAssignments[x];
			ABVERTEX0L & o = vo->m_originalArrayVector[a.vertexIndex];
			int b = a.boneIndex; float wt = 1.0f;
			put(a.vertexIndex, a.vertexesCarried, a.carriedCount, 1, &b, &wt, &o.x, &o.y, &o.z, &o.nx, &o.ny, &o.nz);
		}
		else if((vo->m_blendedType == 1 && vo->m_blendedVertexAssignments) || (vo->m_blendedType == 2 && vo->m_blendedVertexAssignmentsWt))
		{
			int nb, vIndex, carriedCount; short * carried; byte * bi; float * bw;
			std::vector<float> ox, oy, oz, onx, ony, onz;
			if(vo->m_blendedType == 1)
			{
				VertexAssignmentBlended & a = vo->m_blendedVertexAssignments[x];
				nb = a.boneIndexesCount; vIndex = a.vertexIndex; carried = a.vertexesCarried; carriedCount = a.carriedCount; bi = a.boneIndexes; bw = a.weightsToBone;
				for(int k = 0; k < nb; k++) { ox.push_back(a.offsetVects[k].x); oy.push_back(a.offsetVects[k].y); oz.push_back(a.offsetVects[k].z); onx.push_back(a.offsetVects[k].nx); ony.push_back(a.offsetVects[k].ny); onz.push_back(a.offsetVects[k].nz); }
			}
			else
			{
				VertexAssignmentBldTgt & a = vo->m_blendedVertexAssignmentsWt[x];
				nb = a.boneIndexesCount; vIndex = a.vertexIndex; carried = a.vertexesCarried; carriedCount = a.carriedCount; bi = a.boneIndexes; bw = a.weightsToBone;
				for(int k = 0; k < nb; k++) { ox.push_back(a.offsetVects[k].x); oy.push_back(a.offsetVects[k].y); oz.push_back(a.offsetVects[k].z); onx.push_back(a.offsetVects[k].nx); ony.push_back(a.offsetVects[k].ny); onz.push_back(a.offsetVects[k].nz); }
			}
			if(nb <= 0) continue;
			std::vector<int> b(nb); std::vector<float> wt(nb);
			for(int k = 0; k < nb; k++) { b[k] = bi[k]; wt[k] = bw[k]; }
			put(vIndex, carried, carriedCount, nb, &b[0], &wt[0], &ox[0], &oy[0], &oz[0], &onx[0], &ony[0], &onz[0]);
		}
	}
	for(int i = 0; i < vc; i++)
		if(w[i].empty()) w[i].push_back(std::make_pair(0, 1.0f));
}

static void BJ_WriteSkin(CDeformableVisObj * vo, const std::vector<D3DXMATRIX> & invBind,
                         const std::vector<D3DXVECTOR3> & pos, const std::vector<D3DXVECTOR3> & nrm, const std::vector<BJWeights> & w)
{
	int vc = (int)pos.size();
	VertexAssignmentBlended * va = new VertexAssignmentBlended[vc];
	ABVERTEX0L * orig = new ABVERTEX0L[vc];
	for(int v = 0; v < vc; v++)
	{
		orig[v].x = pos[v].x; orig[v].y = pos[v].y; orig[v].z = pos[v].z;
		orig[v].nx = nrm[v].x; orig[v].ny = nrm[v].y; orig[v].nz = nrm[v].z;
		float sum = 0.0f;
		for(size_t q = 0; q < w[v].size(); q++) sum += w[v][q].second;
		if(sum <= 0.0f) sum = 1.0f;
		int nb = (int)w[v].size();
		VertexAssignmentBlended & a = va[v];
		a.boneIndexesCount = (byte)nb;
		a.boneIndexes = new byte[nb];
		a.weightsToBone = new float[nb];
		a.offsetVects = new ABVERTEX0L[nb];
		a.vertexIndex = (short)v;
		a.vertexesCarried = NULL;
		a.carriedCount = 0;
		for(int q = 0; q < nb; q++)
		{
			int b = w[v][q].first;
			const D3DXMATRIX & inv = invBind[b];
			a.boneIndexes[q] = (byte)b;
			a.weightsToBone[q] = w[v][q].second / sum;
			a.offsetVects[q].x  = pos[v].x * inv._11 + pos[v].y * inv._21 + pos[v].z * inv._31 + inv._41;
			a.offsetVects[q].y  = pos[v].x * inv._12 + pos[v].y * inv._22 + pos[v].z * inv._32 + inv._42;
			a.offsetVects[q].z  = pos[v].x * inv._13 + pos[v].y * inv._23 + pos[v].z * inv._33 + inv._43;
			a.offsetVects[q].nx = nrm[v].x * inv._11 + nrm[v].y * inv._21 + nrm[v].z * inv._31;
			a.offsetVects[q].ny = nrm[v].x * inv._12 + nrm[v].y * inv._22 + nrm[v].z * inv._32;
			a.offsetVects[q].nz = nrm[v].x * inv._13 + nrm[v].y * inv._23 + nrm[v].z * inv._33;
		}
	}
	// old arrays are left alone (another copy may still point at them)
	vo->m_vertexAssignments = NULL;
	vo->m_blendedVertexAssignmentsWt = NULL;
	vo->m_blendedVertexAssignments = va;
	vo->m_originalArrayVector = orig;
	vo->m_assignmentCount = vc;
	vo->m_blendedType = 1;
}

BOOL CSkeletonObject::GiveTopsJiggle(CString & report)
{
	report.Empty();
	if(!m_bonesList || !m_alternateConfigurations)
	{
		report = "This character has no body-part slots.";
		return FALSE;
	}
	m_bonesList->CompileDirectAccess();
	const int boneCount = m_bonesList->m_directAccessArrayCount;
	std::vector<D3DXMATRIX> boneM(boneCount), invBind(boneCount);
	std::vector<bool> isJiggle(boneCount, false);
	float reach = 0.0f;
	for(int b = 0; b < boneCount; b++)
	{
		D3DXMatrixIdentity(&boneM[b]); D3DXMatrixIdentity(&invBind[b]);
		CBoneObject * bp = m_bonesList->m_directAccessArray[b].object;
		if(!bp) continue;
		boneM[b] = D3DXMATRIX((const float *)&bp->m_startPosition.m[0][0]);
		if(!D3DXMatrixInverse(&invBind[b], NULL, &boneM[b])) D3DXMatrixIdentity(&invBind[b]);
		CString nm = bp->m_boneName; nm.MakeLower();
		if(nm.Find("boob") >= 0 || nm.Find("breast") >= 0 || nm.Find("butt") >= 0)
		{
			isJiggle[b] = true;
			for(int p = 0; p < boneCount; p++)
			{
				CBoneObject * pp = m_bonesList->m_directAccessArray[p].object;
				if(pp && p != b && pp->m_boneName.CompareNoCase(bp->m_parentName) == 0)
				{
					float dx = bp->m_startPosition._41 - pp->m_startPosition._41;
					float dy = bp->m_startPosition._42 - pp->m_startPosition._42;
					float dz = bp->m_startPosition._43 - pp->m_startPosition._43;
					float r = sqrtf(dx * dx + dy * dy + dz * dz);
					if(r > reach) reach = r;
				}
			}
		}
	}
	bool any = false;
	for(int b = 0; b < boneCount; b++) if(isJiggle[b]) any = true;
	if(!any)
	{
		report = "This character has no jiggle bones (bones named boob / breast / butt).";
		return FALSE;
	}

	int changed = 0, slotIdx = 0;
	CString details;
	for(POSITION altPos = m_alternateConfigurations->GetHeadPosition(); altPos != NULL; ++slotIdx)
	{
		CAlterUnitDBObject * altUnit = (CAlterUnitDBObject *)m_alternateConfigurations->GetNext(altPos);
		if(!altUnit || !altUnit->m_configurations) continue;
		// every mesh (all looks, all detail levels) in this slot
		std::vector<CDeformableVisObj *> parts;
		auto addList = [&](CDeformableMesh * dm) {
			if(!dm || !dm->m_lodChain) return;
			for(POSITION lp = dm->m_lodChain->GetHeadPosition(); lp != NULL;)
			{
				CDeformableVisObj * vo = (CDeformableVisObj *)dm->m_lodChain->GetNext(lp);
				if(!vo || !vo->m_mesh || vo->m_mesh->m_abVertexArray.m_vertexCount == 0) continue;
				bool dup = false;
				for(size_t i = 0; i < parts.size(); i++) if(parts[i] == vo) dup = true;
				if(!dup) parts.push_back(vo);
			}
		};
		for(POSITION cp = altUnit->m_configurations->GetHeadPosition(); cp != NULL;)
			addList((CDeformableMesh *)altUnit->m_configurations->GetNext(cp));
		if(m_deformableMeshArrayV2 && slotIdx < m_deformableMeshArrayCount)
			addList(m_deformableMeshArrayV2[slotIdx].m_defMeshVisualUnit);

		// read every skin, find the ones that already jiggle
		std::vector<std::vector<D3DXVECTOR3> > P(parts.size()), N(parts.size());
		std::vector<std::vector<BJWeights> > W(parts.size());
		std::vector<D3DXVECTOR3> srcPos;
		std::vector<BJWeights> srcW;
		std::vector<bool> hasJiggle(parts.size(), false);
		for(size_t i = 0; i < parts.size(); i++)
		{
			BJ_ReadSkin(parts[i], boneM, P[i], N[i], W[i]);
			for(size_t v = 0; v < W[i].size(); v++)
			{
				BJWeights jw;
				for(size_t q = 0; q < W[i][v].size(); q++)
					if(isJiggle[W[i][v][q].first] && W[i][v][q].second > 0.01f) jw.push_back(W[i][v][q]);
				if(!jw.empty()) { hasJiggle[i] = true; srcPos.push_back(P[i][v]); srcW.push_back(jw); }
			}
		}
		if(srcPos.empty()) continue;
		float maxDist = reach * 0.6f;
		if(maxDist <= 0.0f) maxDist = 1.0f;

		for(size_t i = 0; i < parts.size(); i++)
		{
			if(hasJiggle[i]) continue;
			int touched = 0;
			for(size_t v = 0; v < P[i].size(); v++)
			{
				float best = 1e30f; size_t bi = 0;
				for(size_t s = 0; s < srcPos.size(); s++)
				{
					D3DXVECTOR3 d = P[i][v] - srcPos[s];
					float dd = d.x * d.x + d.y * d.y + d.z * d.z;
					if(dd < best) { best = dd; bi = s; }
				}
				float dist = sqrtf(best);
				if(dist >= maxDist) continue;
				float f = 1.0f - dist / maxDist;
				f = f * f * (3.0f - 2.0f * f);   // smooth fade at the edge
				float jsum = 0.0f;
				BJWeights add;
				for(size_t q = 0; q < srcW[bi].size(); q++) { float wq = srcW[bi][q].second * f; add.push_back(std::make_pair(srcW[bi][q].first, wq)); jsum += wq; }
				if(jsum <= 0.001f) continue;
				if(jsum > 1.0f) jsum = 1.0f;
				BJWeights out;
				for(size_t q = 0; q < W[i][v].size(); q++) out.push_back(std::make_pair(W[i][v][q].first, W[i][v][q].second * (1.0f - jsum)));
				for(size_t q = 0; q < add.size(); q++) out.push_back(add[q]);
				W[i][v] = out;
				touched++;
			}
			if(touched > 0)
			{
				BJ_WriteSkin(parts[i], invBind, P[i], N[i], W[i]);
				changed++;
				CString line;
				line.Format("\n  slot %d: a piece got jiggle on %d points", slotIdx, touched);
				details += line;
			}
		}
	}
	if(changed == 0)
	{
		report = "No other tops were close enough to the jiggle part to copy it.";
		return FALSE;
	}
	report.Format("%d piece(s) now jiggle:", changed);
	report += details;
	return TRUE;
}

////////Macros

IMPLEMENT_SERIAL(CAlterUnitDBObjectList, CObject, 0)
IMPLEMENT_SERIAL(CAlterUnitDBObject, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CSkeletonObject, CObject, VERSIONABLE_SCHEMA|5)



//-------------------




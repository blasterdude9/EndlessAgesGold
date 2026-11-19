#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "matrixarb.h"

#include "CBoneAnimationClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
CBoneAnimationObject::CBoneAnimationObject()
{
m_totalFrames          = 0;
//m_currentPosition      = 0;
m_millisecondsPerFrame = 0;
//m_baseFrame            = 0;
//m_targetFrame          = 0;
//m_currentMS            = 0;
m_animationData        = NULL;
m_soundRanges          = NULL;
m_looping              = TRUE;
m_timingBased          = 0;
m_timingCapMax         = 60;
m_timingCapMin         = 10;
//m_inReverse            = FALSE;
m_animationName        = "NONE";
m_type                 = 0;
//m_lastAnimBaseFrame    = 0;
//m_lastAnimTargetFrame  = 0;
m_minPlayDuration      = 0;
m_version              = 0;
//m_currentTimePassed    = 0;
//m_lastUpdatedTime      = 0;
m_deleteAnimData       = TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Deconstructor---------------------------------//
void CBoneAnimationObject::SafeDelete()
{
	if(m_soundRanges){
       m_soundRanges->SafeDelete();
       delete m_soundRanges;
	   m_soundRanges=0;
	}

	if(m_deleteAnimData)
	if(m_animationData){
       delete [] m_animationData;
       m_animationData = 0;
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CBoneAnimationObject::Clone(CBoneAnimationObject ** clone)
{
     CBoneAnimationObject * animPtr = new CBoneAnimationObject();
     //base data
     animPtr->m_totalFrames          = m_totalFrames;
     animPtr->m_millisecondsPerFrame = m_millisecondsPerFrame;
     animPtr->m_looping              = m_looping;
     animPtr->m_timingBased          = m_timingBased;
     animPtr->m_timingCapMax         = m_timingCapMax;
     animPtr->m_timingCapMin         = m_timingCapMin;
     animPtr->m_animationName        = m_animationName;
     animPtr->m_type                 = m_type;
     //animPtr->m_lastAnimBaseFrame    = m_lastAnimBaseFrame;
     //animPtr->m_lastAnimTargetFrame  = m_lastAnimTargetFrame;
	 animPtr->m_minPlayDuration      = m_minPlayDuration;
	 animPtr->m_version              = m_version;
	 animPtr->m_soundRanges          = NULL;
	 //sound ranges
	 if(m_soundRanges){
        m_soundRanges->Clone(&animPtr->m_soundRanges);
	 }
     //animation 
	 if(m_animationData != NULL){
		animPtr->m_deleteAnimData = FALSE;
		animPtr->m_animationData = m_animationData;
		/*animPtr->m_animationData = new AnimationContent[m_totalFrames];
        CopyMemory(animPtr->m_animationData,
			       m_animationData,
			      (sizeof(AnimationContent)*m_totalFrames));*/
	 }


     *clone = animPtr;
}
 //------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CBoneAnimationObject::CloneRaw(CBoneAnimationObject ** clone)
{
     CBoneAnimationObject * animPtr = new CBoneAnimationObject();
     //base data
     animPtr->m_totalFrames          = m_totalFrames;
     animPtr->m_millisecondsPerFrame = m_millisecondsPerFrame;
     animPtr->m_looping              = m_looping;
     animPtr->m_timingBased          = m_timingBased;
     animPtr->m_timingCapMax         = m_timingCapMax;
     animPtr->m_timingCapMin         = m_timingCapMin;
     animPtr->m_animationName        = m_animationName;
     animPtr->m_type                 = m_type;
     //animPtr->m_lastAnimBaseFrame    = m_lastAnimBaseFrame;
     //animPtr->m_lastAnimTargetFrame  = m_lastAnimTargetFrame;
	 animPtr->m_minPlayDuration      = m_minPlayDuration;
	 animPtr->m_version              = m_version;
	 animPtr->m_soundRanges          = NULL;
	 //sound ranges
	 if(m_soundRanges){
        m_soundRanges->Clone(&animPtr->m_soundRanges);
	 }
     //animation 
	 if(m_animationData != NULL){
		animPtr->m_deleteAnimData = TRUE;
		//animPtr->m_animationData = m_animationData;
		animPtr->m_animationData = new AnimationContent[m_totalFrames];
        CopyMemory(animPtr->m_animationData,
			       m_animationData,
			      (sizeof(AnimationContent)*m_totalFrames));
	 }


     *clone = animPtr;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
D3DVECTOR CBoneAnimationObject::GetBonePositionalDifference(D3DVECTOR currentBonePosition)
{
   if(!m_animationData)
       return currentBonePosition;


   D3DVECTOR startPosition = m_animationData[0].position;//start Point

   currentBonePosition.x = currentBonePosition.x - startPosition.x;
   currentBonePosition.y = currentBonePosition.y - startPosition.y;
   currentBonePosition.z = currentBonePosition.z - startPosition.z;
   return currentBonePosition;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CBoneAnimationObject::GetBonePositionalDataByTime(long        & currentPosition,
													          D3DVECTOR   & position,
															  D3DVECTOR   & upVector,
															  D3DVECTOR   & dirVector)
{

   /*position = GL_matrixFunctions.InterpolatePoints(
		                                           m_animationData[m_baseFrame].position, 
						                           m_animationData[m_targetFrame].position, 
					                               m_millisecondsPerFrame, 
					                               currentPosition);

   upVector = GL_matrixFunctions.InterpolatePoints(
		                                           m_animationData[m_baseFrame].upVect.GetD3DVECTOR(), 
						                           m_animationData[m_targetFrame].upVect.GetD3DVECTOR(), 
					                               m_millisecondsPerFrame, 
					                               currentPosition);

   dirVector = GL_matrixFunctions.InterpolatePoints(
		                                           m_animationData[m_baseFrame].dirVect.GetD3DVECTOR(), 
						                           m_animationData[m_targetFrame].dirVect.GetD3DVECTOR(), 
					                               m_millisecondsPerFrame, 
					                               currentPosition);*/
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CBoneAnimationObject::SetSoundActivationEvent(CString & fileName,
												   long start,
												   long end,
												   int id,
												   float soundRange,
												   float soundClipRadius,
												   BOOL velocityBased,
												   BOOL looping)
{
	if(!m_soundRanges)
		m_soundRanges = new CSoundRangeObjectList();
    CSoundRangeObject * sndPtr = new CSoundRangeObject();
    sndPtr->m_id = id;
	sndPtr->m_looping = looping;
	sndPtr->m_velocityBased = velocityBased;
	sndPtr->m_radiusClipping = soundClipRadius;
	sndPtr->m_soundFactor = soundRange;
	sndPtr->SetRanges(start,end);
	m_soundRanges->AddTail(sndPtr);
	return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CBoneAnimationObject::LoadAnimationData(CString FileName,CString nodename)
{
	if(RetrieveAnimationSpeedLength(FileName)==TRUE){
       //allocate
	   //m_matrixCurrent        = new D3DMATRIX[m_totalFrames+1];
	   m_animationData        = new AnimationContent[m_totalFrames+1];
       //get data
	   BOOL positionDataFound = FALSE;
	   BOOL rotationDataFound = FALSE;
       //--------------------------------------
       //--------------------------------------
       CFileException exc;
       CFile fl;
       BOOL res = fl.Open(FileName,CFile::modeRead, &exc);
	   if(res == FALSE)
		  return FALSE;
          CArchive the_in_Archive(&fl,CArchive::load);
	      CString string = "Begin";
	      CString tempSt;
	      while(string.GetLength() != 0){
	            the_in_Archive.ReadString(string);
	            tempSt = string;
	            if(tempSt.GetLength() > 10)
                   tempSt.Delete(10,(tempSt.GetLength()-10));
	               if(tempSt == "\t*NODE_TM "){//animation section check name     
			          the_in_Archive.ReadString(string);
				      tempSt = string;
	                  tempSt.Delete(0,14); 
					  tempSt.Remove('"');
						 if(tempSt == nodename){//match
                            int currentFrame = -1;
							the_in_Archive.ReadString(string);
							while(string != "\t\t}" && string.GetLength()>0){//loop through animations
						    
							tempSt = string;
							if(tempSt.GetLength() > 8)
                               tempSt.Delete(8,(tempSt.GetLength()-8));
							   D3DMATRIX matrixCurrent;
							   GL_matrixFunctions.InitMat(matrixCurrent.m);
							   if(tempSt == "\t\t*FRAME"){//found a frame
								  currentFrame++;
							      the_in_Archive.ReadString(string);
                                  
								  for(int animCount=0;animCount<4;animCount++){
								  D3DVECTOR parsedVect = GetPositionFromString(string);
                                  matrixCurrent.m[animCount][0] = parsedVect.x;
								  matrixCurrent.m[animCount][1] = parsedVect.y;
								  matrixCurrent.m[animCount][2] = parsedVect.z;
							        
								  //
								  the_in_Archive.ReadString(string);
								  }
								  GL_matrixFunctions.GetOrientation(matrixCurrent,
									                                &GL_matrixFunctions.Normalize(m_animationData[currentFrame].dirVect.GetD3DVECTOR()),
                                                                    &GL_matrixFunctions.Normalize(m_animationData[currentFrame].upVect.GetD3DVECTOR()));
								  m_animationData[currentFrame].dirVect = GL_matrixFunctions.Normalize(m_animationData[currentFrame].dirVect.GetD3DVECTOR());
								  m_animationData[currentFrame].upVect = GL_matrixFunctions.Normalize(m_animationData[currentFrame].upVect.GetD3DVECTOR());
								  m_animationData[currentFrame].position.x = matrixCurrent._41;
								  m_animationData[currentFrame].position.y = matrixCurrent._42;
								  m_animationData[currentFrame].position.z = matrixCurrent._43;
							   }//end found a frame
							   else
								the_in_Archive.ReadString(string);

                                  


							   if(currentFrame == m_totalFrames){
                                  the_in_Archive.Close();
                                  fl.Close();
                                  return TRUE;
							   }
						 }//end loop through animations

						 }//end match
				   }//end animation section check name      
		  }
	   the_in_Archive.Close();
       fl.Close();
       //--------------------------------------
       //--------------------------------------
	}
	else
	AfxMessageBox(operator+("cant find ",FileName));


return TRUE;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CBoneAnimationObject::RetrieveAnimationSpeedLength(CString fileName)
{
CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE)
		return FALSE;
     CArchive the_in_Archive(&fl,CArchive::load);
     
	 CString ticksPerFrame,animationLength;
	 CString string = "Begin";
	 CString tempSt;
	 while(string.GetLength() != 0){
	   the_in_Archive.ReadString(string);
	   tempSt = string;
	   
       tempSt = string;
	   if(tempSt.GetLength() > 18)//detect framespeed
          tempSt.Delete(18,(tempSt.GetLength()-18));
	      if(tempSt == "\t*SCENE_LASTFRAME "){	          
			 int index = 18;
			 animationLength.Empty();
			  for(int loop=index;loop<string.GetLength();loop++){
                  CString ind = (CString)string.GetAt(loop);
				  if(ind == "\t" || ind == " "){
					  loop++;
					  index = loop;
					  break;
				  }
				  animationLength = operator+(animationLength,string.GetAt(loop));
			  }
		  }
      
	   tempSt = string;
	   if(tempSt.GetLength() > 22)//detect framespeed
          tempSt.Delete(22,(tempSt.GetLength()-22));
	      if(tempSt == "\t*SCENE_TICKSPERFRAME "){	          
			 int index = 22;
			 ticksPerFrame.Empty();
			  for(int loop=index;loop<string.GetLength();loop++){
                  CString ind = (CString)string.GetAt(loop);
				  if(ind == "\t" || ind == " "){
					  loop++;
					  index = loop;
					  break;
				  }
				  ticksPerFrame = operator+(ticksPerFrame,string.GetAt(loop));
			  }
		  break;
		  }
	 }
	 m_millisecondsPerFrame    = (long)atof(ticksPerFrame);
	 m_totalFrames             = (long)atof(animationLength);
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
D3DVECTOR CBoneAnimationObject::GetPositionFromString(CString string)
{
             D3DVECTOR outPut;
			  long timeSample=0;
              CString x,y,z,time;
			  int index = 11;
			  int loop=0;
			  for(loop=index;loop<string.GetLength();loop++){
                  CString ind = (CString)string.GetAt(loop);
				  if(ind == "\t" || ind == " "){
					  loop++;
					  index = loop;
					  break;
				  }
				  x = operator+(x,string.GetAt(loop));
			  }
              for(loop=index;loop<string.GetLength();loop++){
                  CString ind = (CString)string.GetAt(loop);
				  if(ind == "\t" || ind == " "){
					  loop++;
					  index = loop;
					  break;
				  }
				  y = operator+(y,string.GetAt(loop));
			  }
			  for(loop=index;loop<string.GetLength();loop++){
                  CString ind = (CString)string.GetAt(loop);
				  if(ind == "\t" || ind == " "){
					  loop++;
					  index = loop;
					  break;
				  }
				  z = operator+(z,string.GetAt(loop));
			  }
              
              outPut.x = (float)atof(x);
			  outPut.y = (float)atof(y);
			  outPut.z = (float)atof(z);
return outPut;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Serialize-------------------------------------//
void CBoneAnimationObject::Serialize( CArchive& ar )
{
int reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){

       ar.SetObjectSchema(3);
       ASSERT(ar.GetObjectSchema() == 3);


	   ar << m_totalFrames 
		  << reserved
		  << m_millisecondsPerFrame
		  << m_looping
		  << m_timingCapMax
		  << m_timingCapMin
	      << m_timingBased
		  << m_animationName
		  << m_type
		  << m_soundRanges
		  << reserved
		  << reserved
		  << reserved
		  << reserved
		  << m_minPlayDuration
		  << m_version;


	   for(int loop=0;loop<m_totalFrames;loop++){
          ar << m_animationData[loop].upVect.x.m_cVal 
		     << m_animationData[loop].upVect.y.m_cVal
		     << m_animationData[loop].upVect.z.m_cVal 
		     << m_animationData[loop].dirVect.x.m_cVal 
		     << m_animationData[loop].dirVect.y.m_cVal
		     << m_animationData[loop].dirVect.z.m_cVal
		     << m_animationData[loop].position.x 
		     << m_animationData[loop].position.y
		     << m_animationData[loop].position.z
			 << m_animationData[loop].visibility;
	   }
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_totalFrames 
		  >> reserved
		  >> m_millisecondsPerFrame
		  >> m_looping
		  >> m_timingCapMax
		  >> m_timingCapMin
		  >> m_timingBased
		  >> m_animationName
		  >> m_type
		  >> m_soundRanges
		  >> reserved
		  >> reserved
		  >> reserved
		  >> reserved
		  >> m_minPlayDuration
		  >> m_version;

       if(m_totalFrames>0)
       m_animationData = new AnimationContent[m_totalFrames];
       
	   if(version < 2)
	   {
	    float temp=0;
	    for(int loop=0;loop<m_totalFrames;loop++){
           ar >> temp; m_animationData[loop].upVect.x = temp;
		   ar >> temp; m_animationData[loop].upVect.y = temp;
		   ar >> temp; m_animationData[loop].upVect.z = temp; 
		   ar >> temp; m_animationData[loop].dirVect.x = temp; 
		   ar >> temp; m_animationData[loop].dirVect.y = temp;
		   ar >> temp; m_animationData[loop].dirVect.z = temp;
		   ar >> m_animationData[loop].position.x; 
		   ar >> m_animationData[loop].position.y;
		   ar >> m_animationData[loop].position.z;

		   m_animationData[loop].visibility = 1.0f;
	    }
	   }
	   else if(version == 2)
	   {
          for(int loop=0;loop<m_totalFrames;loop++)
		  {
           ar >> m_animationData[loop].upVect.x.m_cVal
		      >> m_animationData[loop].upVect.y.m_cVal
		      >> m_animationData[loop].upVect.z.m_cVal
		      >> m_animationData[loop].dirVect.x.m_cVal
		      >> m_animationData[loop].dirVect.y.m_cVal
		      >> m_animationData[loop].dirVect.z.m_cVal
		      >> m_animationData[loop].position.x 
		      >> m_animationData[loop].position.y
		      >> m_animationData[loop].position.z;

		   m_animationData[loop].visibility = 1.0f;//default for legacy file formats is visible
	     }
	   }
	   else if(version == 3)
	   {
          for(int loop=0;loop<m_totalFrames;loop++)
		  {
           ar >> m_animationData[loop].upVect.x.m_cVal
		      >> m_animationData[loop].upVect.y.m_cVal
		      >> m_animationData[loop].upVect.z.m_cVal
		      >> m_animationData[loop].dirVect.x.m_cVal
		      >> m_animationData[loop].dirVect.y.m_cVal
		      >> m_animationData[loop].dirVect.z.m_cVal
		      >> m_animationData[loop].position.x 
		      >> m_animationData[loop].position.y
		      >> m_animationData[loop].position.z
			  >> m_animationData[loop].visibility;
	     }
	   }
	   //m_currentPosition      = 0;
	   //m_baseFrame            = 0;
	   //m_targetFrame          = 0;
	   //m_lastAnimBaseFrame    = 0;
	   //m_lastAnimTargetFrame  = 0;

       //m_currentMS            = 0;
	   //m_inReverse            = FALSE;

	   //m_currentTimePassed    = 0;
       //m_lastUpdatedTime      = 0;
	   m_deleteAnimData       = TRUE;
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------clone-------------------------------------//
 void CBoneAnimationList::Clone(CBoneAnimationList ** clone)
{
    CBoneAnimationList * newList = new CBoneAnimationList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBoneAnimationObject * animPtr = (CBoneAnimationObject *) GetNext(posLoc);
        CBoneAnimationObject * cloneObject = NULL;  
		animPtr->Clone(&cloneObject);
		newList->AddTail(cloneObject);
	}
    *clone = newList;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Deconstructor---------------------------------//
void CBoneAnimationList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBoneAnimationObject * animPtr = (CBoneAnimationObject *) GetNext(posLoc);
        animPtr->SafeDelete();
		delete animPtr;
		animPtr=0;
	}
    RemoveAll();
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Deconstructor---------------------------------//
CBoneAnimationObject * CBoneAnimationList::GetByType(int type)
{
	for(POSITION posLoc = GetTailPosition();posLoc != NULL;){
        CBoneAnimationObject * animPtr = (CBoneAnimationObject *) GetPrev(posLoc);
		if(type == animPtr->m_type)
		{
           return animPtr;
		}
	}
	return NULL;
}


IMPLEMENT_SERIAL(CBoneAnimationObject,  CObject, VERSIONABLE_SCHEMA|3)
IMPLEMENT_SERIAL(CBoneAnimationList,    CObject, 0)
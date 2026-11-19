#define D3D_OVERLOADS
#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>


#include "CMovementModel.h"
#include "CABFunctionLib.h"
#include "dbconnectionsystem.h"
#include "ItemManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//---------------------------------------------------------------//
//-------------------------Movement object/list------------------//
//---------------------------------------------------------------//
CMovementObj::~CMovementObj()
{
    if(m_optionalTextMesh)
	{
       m_optionalTextMesh->SafeDelete();
	   delete m_optionalTextMesh;
	   m_optionalTextMesh = 0;
	}

    if(m_textureRenderObject)
	{
       m_textureRenderObject->SafeDelete();
       delete m_textureRenderObject;
	   m_textureRenderObject = 0;
	}
	
	if(m_textureRenderObject2)
	{
       m_textureRenderObject2->SafeDelete();
       delete m_textureRenderObject2;
	   m_textureRenderObject2 = 0;
	}

    if(m_skills)
	{
       m_skills->SafeDelete();
       delete m_skills;
	   m_skills=0;
	}

    if(m_deathCfg)
	{
	   m_deathCfg->SafeDelete();
       delete m_deathCfg;
       m_deathCfg = 0;
	}

    if(m_autoCfg){
	   m_autoCfg->SafeDelete();
       delete m_autoCfg;
       m_autoCfg = 0;
	}

    if(m_itemDB){
       m_itemDB->SafeDelete();
	   delete m_itemDB;
	   m_itemDB=0;
	}


    if(List_skeletonLODList){
       List_skeletonLODList->SafeDelete();
	   delete List_skeletonLODList;
       List_skeletonLODList = 0;
	}


    if(List_armedInventoryDB){
       List_armedInventoryDB->SafeDelete();
	   delete List_armedInventoryDB;
	   List_armedInventoryDB=0;
	}

    

	if(List_baseFrame){
	   List_baseFrame->SafeDelete();
       delete List_baseFrame;
	   List_baseFrame=NULL;
	}
	  

	if(List_movementDB){
	   List_movementDB->SafeDelete();
       delete List_movementDB;
	   List_movementDB=0;
	}

   if(List_skeletalObject){
      List_skeletalObject->SafeDelete();
	  delete List_skeletalObject;
	  List_skeletalObject=0;
   }

   if(m_resources){
      m_resources->SafeDelete();
      delete m_resources;
	  m_resources=0;
   }

   if(m_inventory){
      m_inventory->SafeDelete();
      delete m_inventory;
	  m_inventory = 0;
   }
   if(m_aiObj){
	  m_aiObj->SafeDelete();
      delete m_aiObj;
      m_aiObj=0;
   }
   //posses stuff
   if(m_posses)
   {
      m_posses->m_possedBy = NULL;	
	  m_posses = NULL;
   }
   if(m_possedBy){
      m_possedBy->m_posses = NULL;
   }

   if(m_stats)
   {
      m_stats->SafeDelete();
	  delete m_stats;
	  m_stats = 0;
   }

   if(m_collisionRaySys)
   {
      delete m_collisionRaySys;
	  m_collisionRaySys = 0;
   }

   if(m_badges)
   {
	   delete [] m_badges;
	   m_badges = 0;
   }

   

   return;
}

//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
BOOL CMovementObj::LoadLODFRomDir(CString directory,LPDIRECT3DDEVICE9 device)
{
   
	
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
	  CString ext = GetFileExt(name);
	  name.MakeUpper();

	  if(ext.MakeUpper() == "ALT")
	  {
	     validDir = true;
	     List_skeletalObject->MergeAsLod((directory+name),device);
	  }

	  if(doneArb == false)
	  if(ext.MakeUpper() == "ARB")
	  {
	     validDir = true;
         List_skeletalObject->MergeAsLod((directory+name),device);
		 doneArb = true;
	  }

	  }//end if valid file
	} while (FindNextFile(FileHandle, &FindData));

    if(validDir == true)
       return TRUE;
	else
	   return FALSE;

}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
BOOL CMovementObj::LoadFromDir(CString path,CString directory,LPDIRECT3DDEVICE9 device,BOOL resetBaseSkeleton)
{
    


    WIN32_FIND_DATA FindData;
    HANDLE FileHandle;
    DWORD Attrib;
    CString name;
	if(resetBaseSkeleton)
	{
	  if(List_skeletalObject)
	  {
       List_skeletalObject->SafeDelete();
	   delete List_skeletalObject;
	   List_skeletalObject = 0;
	  }
      List_skeletalObject = new CSkeletonObject();
	  if(!List_skeletalObject->InitBoneSystem(path,device))
		  return FALSE;
	  List_skeletalObject->m_SkeletalName = "New Skeletal";
	  List_movementDB = new CMovementCaps();
	}
	CString cfgFileToApply = "";
	//first initialize skeleton List_skeletalObject
    
    FileHandle = FindFirstFile(directory+"*.*", &FindData);
	do{
	  Attrib = FindData.dwFileAttributes;
	  if(Attrib != 16){//if valid file
	  name = FindData.cFileName;
	  CString ext = GetFileExt(name);
	  name.MakeUpper();

      if(ext.MakeUpper() == "CFG")
	  {//Config file
	     cfgFileToApply = (directory+name);
	  }//end config file

	  if(ext.MakeUpper() == "MDB")
	  {
         CFileException exc;
         CFile fl;
	     CArchive the_Archive(&fl,CArchive::load);
         BOOL res = fl.Open((directory+name),CFile::modeRead, &exc);
	     
         if(List_movementDB)
		 { 
            List_movementDB->SafeDelete();
            delete List_movementDB;
			List_movementDB = 0;
		 }

         the_Archive >> List_movementDB;

         the_Archive.Close();
	     fl.Close();
	  }

	  if(ext.MakeUpper() == "ALT")
	  {
		if(!List_skeletalObject->MergeAsAltCfg2((directory+name),device))
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
	    /*if(name == "WALK.ARB" || name == "WALK1.ARB" || name == "WALK2.ARB" || name == "WALK3.ARB" || name == "WALK4.ARB"|| name == "WALK5.ARB" || name == "WALK6.ARB" || name == "WALK7.ARB" || name == "WALK8.ARB" || name == "WALK9.ARB")
	       type = 3;
        if(name == "RUN.ARB" || name == "RUN1.ARB" || name == "RUN2.ARB" || name == "RUN3.ARB" || name == "RUN4.ARB" || name == "RUN5.ARB" || name == "RUN6.ARB" || name == "RUN7.ARB" || name == "RUN8.ARB" || name == "RUN9.ARB" || name == "RUN10.ARB")
	       type = 3;//type = 2;
        if(name == "STAND.ARB" || name == "STAND1.ARB" || name == "STAND2.ARB" || name == "STAND3.ARB" || name == "STAND4.ARB" || name == "STAND5.ARB" || name == "STAND6.ARB" || name == "STAND7.ARB" || name == "STAND8.ARB" || name == "STAND9.ARB" || name == "STAND10.ARB")
	       type = 1;
        if(name == "DIE.ARB" || name == "DIE1.ARB" || name == "DIE2.ARB" )
           type = 8;
        if(name == "JUMP.ARB" || name == "JUMP1.ARB" || name == "JUMP2.ARB" || )
	       type = 5;
	    if(name == "CROUCH.ARB" || name == "CROUCH1.ARB" || name == "CROUCH2.ARB")
	       type = 11;
	    if(name == "FIRE.ARB" || name == "FIRE1.ARB" || name == "FIRE2.ARB" || name == "FIRE3.ARB" || name == "FIRE4.ARB" || name == "FIRE5.ARB" || name == "FIRE6.ARB" || name == "FIRE7.ARB" || name == "FIRE8.ARB" || name == "FIRE9.ARB")
	       type = 12;
        if(name == "SWIM IDLE.ARB" || name == "SWIM IDLE1.ARB" || name == "SWIM IDLE2.ARB")
	       type = 14;
	    if(name == "SWIM ACTION.ARB" || name == "SWIM ACTION1.ARB" || name == "SWIM ACTION2.ARB")
	       type = 15;
	    if(name == "JET PACK.ARB" || name == "JET PACK1.ARB" || name == "JET PACK2.ARB")
	       type = 16;
	    if(name == "PAIN.ARB" || name == "PAIN1.ARB" || name == "PAIN2.ARB")
	       type = 17;*/
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
	    if(List_skeletalObject->LoadAnimation((directory+name),TRUE,type,0,-1)==FALSE)
		{
		   AfxMessageBox("Failed to load animation file onto skeletal object:"+name);
		   break;
		}
	  }

	  }//end if valid file
	} while (FindNextFile(FileHandle, &FindData));

    //if cfg file needed load and exec now post build
	if(cfgFileToApply.GetLength() > 0)
	{//DO CFG
	   CFileException exc;
       CFile fl;
	   CArchive the_Archive(&fl,CArchive::load);
       BOOL res = fl.Open(cfgFileToApply,CFile::modeRead, &exc);
	   CString tempBuffer,secondaryBuffer;
	   while(the_Archive.ReadString(tempBuffer))
	   {      
	     the_Archive.ReadString(secondaryBuffer);
            
	     if(tempBuffer == "First Person Camera")
			List_camFirstPerson = atoi(secondaryBuffer);

		 if(tempBuffer == "Third Person Camera")
			List_camThirdPerson = atoi(secondaryBuffer);
		   
		 if(tempBuffer == "Radius")
			List_boundingRadius = (float)atof(secondaryBuffer);

		 if(tempBuffer == "Base Name")
		 {
			List_skeletalObject->m_SkeletalName = secondaryBuffer;
            List_name = secondaryBuffer;
		 }

		 if(tempBuffer == "Bone Look Lock")
		    List_skeletalObject->m_freeLookBone = atoi(secondaryBuffer);

		 if(tempBuffer == "Bone Look Lock2")
		    List_skeletalObject->m_freeLookBone2 = atoi(secondaryBuffer);

		 if(tempBuffer == "Bone Waist Lock")
		    List_skeletalObject->m_waistBone = atoi(secondaryBuffer);

		 if(tempBuffer == "Unlock Secondary Index")
		 {
            POSITION posLoc = List_skeletalObject->m_bonesList->FindIndex(atoi(secondaryBuffer));
            if(posLoc)
			{
               CBoneObject * bn = (CBoneObject *)List_skeletalObject->m_bonesList->GetAt(posLoc);
			   bn->m_secondaryOverridable = FALSE;
			}
	     }

		 if(tempBuffer == "Center Of Mass Link")
		    List_skeletalObject->m_secondaryCOMInheritBone = atoi(secondaryBuffer);

		 if(tempBuffer == "Make Public Cfg")
		 {
			 
            if(List_skeletalObject->m_deformableMeshArrayCount > 0)
			      if(List_skeletalObject->m_alternateConfigurations)
			      {//alternate exists - this is based off of DB gen
				    for(int countTemp = 0;countTemp<List_skeletalObject->m_deformableMeshArrayCount;countTemp++)
					{//base loop
			         POSITION alPos = List_skeletalObject->m_alternateConfigurations->FindIndex(countTemp);
			         if(alPos)
					 {//valid configuration
						int traceTemp = 0;
			            CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) List_skeletalObject->m_alternateConfigurations->GetAt(alPos);
				        for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				        {//deformable unit same array ie head0 head1
			               CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
	                       
						   if(atoi(secondaryBuffer) == traceTemp)
						      dMesh->m_publicDim = TRUE;
						   
						   traceTemp++;
						}
					 }
					}//end base loop
				  }
		 }

		 //DPD LOD
         if(tempBuffer == "LOD Distance")
		 {
	        if(!List_skeletonLODList)
	            List_skeletonLODList = new CSkeletalLODObjectList();

	        CSkeletalLODObject * newLODEntity = new CSkeletalLODObject();
	        newLODEntity->m_distance = (float)atof(secondaryBuffer);
	        List_skeletonLODList->AddTail(newLODEntity);
         }
	   }
       the_Archive.Close();
	   fl.Close();
	   }



	    //do lod directories if they exist
	    CABFunctionLib cablib;
	    CString lodExt = "LOD";
		int curLOD = 1;
		while(LoadLODFRomDir(directory+lodExt+cablib.ConvToString(curLOD)+"\\",device))
		{
			curLOD++;
		}

		

       return TRUE;

}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----Get File Ext--------------------------------------------//
CString CMovementObj::GetFileExt(CString currentName)
{
CString ext;
int length = currentName.GetLength();
if(length == 0)
   return "";

BOOL done = FALSE;
 while(done == FALSE){
  CString chr = (CString)currentName.GetAt(length-1);
  
  if(chr == ".")
	 break;

  ext = operator+(chr,ext);
  length = length -1;
  
  if(length == 0)
  break;
 }
return ext;
}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
int CMovementObj::QuerryLOD(float & distance)
{
    if(!List_skeletonLODList){
		m_currentLOD = 0;
		return 0;
	}

	m_currentLOD = List_skeletonLODList->GetLodIndex(distance);
    List_skeletalObject->m_currentLOD = m_currentLOD;

    return List_skeletalObject->m_currentLOD;
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
BOOL CMovementObj::GetFireTransform(CString boneName,CArmedInventoryObj * armedPtr, D3DMATRIX & output)
{
	if(!List_skeletalObject)
		return FALSE;

	if(!armedPtr)
	    return FALSE;

	if(!armedPtr->m_thirdPersonSystem)
		return FALSE;

	D3DMATRIX baseAttachBone,localWeaponBone;

    if(!List_skeletalObject->GetBoneMatrixByName(armedPtr->m_boneMapping,baseAttachBone))
        return FALSE;
	
    
	if(!armedPtr->m_thirdPersonSystem->GetBoneMatrixByName(boneName,localWeaponBone))
		return FALSE;

	List_baseFrame->GL_matrixFunctions.fastMatrixMultiply(output,localWeaponBone,baseAttachBone);

	List_baseFrame->GL_matrixFunctions.fastMatrixMultiply(output,output,List_baseFrame->List_frameMatrix);


	return TRUE;
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
BOOL CMovementObj::GetFireTransform(int boneIndex,CArmedInventoryObj * armedPtr, D3DMATRIX & output)
{
	if(!List_skeletalObject)
		return FALSE;

	if(!armedPtr)
	    return FALSE;

	if(!armedPtr->m_thirdPersonSystem)
		return FALSE;

	D3DMATRIX baseAttachBone,localWeaponBone;

    if(!List_skeletalObject->GetBoneMatrixByName(armedPtr->m_boneMapping,baseAttachBone))
        return FALSE;
	
    
	if(!armedPtr->m_thirdPersonSystem->GetBoneMatrixByIndex(boneIndex,localWeaponBone))
		return FALSE;

	List_baseFrame->GL_matrixFunctions.fastMatrixMultiply(output,localWeaponBone,baseAttachBone);

	List_baseFrame->GL_matrixFunctions.fastMatrixMultiply(output,output,List_baseFrame->List_frameMatrix);


	return TRUE;
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
CFireRangeObject * CMovementObj::GetNextActiveFireQue(int & optIndexRet,CArmedInventoryObj	* armedPtr, BOOL & firstPersonMode)
{
    CSkeletonObject * skeletalFocus = armedPtr->m_firstPersonSystem;
    if(!firstPersonMode)
	    skeletalFocus = armedPtr->m_thirdPersonSystem;

    if(!skeletalFocus)
	    return NULL;

	if(!skeletalFocus->m_fireQueList)
		return NULL;
  
	if(!skeletalFocus->m_bonesList)
		return NULL;//no skeleton bones => no animation frames to scan

	optIndexRet = 0;
	for(POSITION firePos = skeletalFocus->m_fireQueList->GetHeadPosition();firePos != NULL;)
	{
		CFireRangeObject * fireRange = (CFireRangeObject *) skeletalFocus->m_fireQueList->GetNext(firePos);
		if(!fireRange)
			continue;

		if(skeletalFocus->m_secondaryAnimationInUse != fireRange->m_animationIndex)
			continue;

		POSITION bonePos = skeletalFocus->m_bonesList->FindIndex(0);
		if(!bonePos)
			continue;
		CBoneObject * bonePtr = (CBoneObject *)skeletalFocus->m_bonesList->GetAt(bonePos);
		if(!bonePtr || !bonePtr->m_animationList)
			continue;//keyless bones (anatomy bones, imported helpers) own no frames

		POSITION animPos = bonePtr->m_animationList->FindIndex(skeletalFocus->m_secondaryAnimationInUse);
		if(!animPos)
			continue;
		CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bonePtr->m_animationList->GetAt(animPos);
		if(!animPtr)
			continue;

		if(fireRange->m_animationFrameRangeStart <= skeletalFocus->m_secondaryBaseFrame &&
           fireRange->m_animationFrameRangeEnd   >= skeletalFocus->m_secondaryBaseFrame)
		{//within range
			if(!fireRange->m_inRangeCurrently)
			{
				fireRange->m_inRangeCurrently = TRUE;
				return fireRange;
			}
		}
		else
			fireRange->m_inRangeCurrently = FALSE;

		optIndexRet++;
	}
	return NULL;
}
//-----------------------------------------------//
//-----------------------------------------------//
//-----------------------------------------------//
BOOL CMovementObj::ApplyStatisticalData(CMovementObj ** target)
{
CMovementObj * midTermPtr = *target;
if(midTermPtr->List_movementDB)
   delete midTermPtr->List_movementDB;
   midTermPtr->List_movementDB=0;
List_movementDB->Clone(&midTermPtr->List_movementDB);
midTermPtr->List_energy                         = List_energy;
midTermPtr->List_maxEnergy                      = List_maxEnergy;
midTermPtr->List_name							= List_name;

return TRUE;
}
//-----------------------------------------------//
//-----------------------------------------------//
//------------entity Clone------------------//
BOOL CMovementObj::Clone(CMovementObj ** clone,
						 LPDIRECT3DDEVICE9 g_pd3dDevice,
						 int * spawnCfg)
{
    CMovementObj * midTermPtr = new CMovementObj(this->List_name,-1);
    //Start Cloneing
    CopyMemory(&midTermPtr->m_matrix,&m_matrix,sizeof(D3DMATRIX));
    if(m_autoCfg)
       m_autoCfg->Clone(&midTermPtr->m_autoCfg);

	midTermPtr->m_itemDB = new CItemObjList();

    if(List_skeletalObject)
       List_skeletalObject->Clone(&midTermPtr->List_skeletalObject,g_pd3dDevice,spawnCfg);

    if(List_skeletonLODList)
       List_skeletonLODList->Clone(&midTermPtr->List_skeletonLODList);
  
    if(List_movementDB)
       List_movementDB->Clone(&midTermPtr->List_movementDB);

       midTermPtr->List_idFromLoc                      = List_idFromLoc;
       midTermPtr->m_ridable                           = m_ridable;
       midTermPtr->List_controlType                    = List_controlType;
       midTermPtr->List_name                           = List_name;
       midTermPtr->List_traceNumber                    = List_traceNumber;
       midTermPtr->List_lastPosition                   = List_lastPosition;
       /*midTermPtr->List_effCollisionPoly1[0]           = List_effCollisionPoly1[0];
       midTermPtr->List_effCollisionPoly2[0]           = List_effCollisionPoly2[0];
       midTermPtr->List_effCollisionPoly1[1]           = List_effCollisionPoly1[1];
       midTermPtr->List_effCollisionPoly2[1]           = List_effCollisionPoly2[1];
       midTermPtr->List_effCollisionPoly1[2]           = List_effCollisionPoly1[2];
       midTermPtr->List_effCollisionPoly2[2]           = List_effCollisionPoly2[2];*/
       midTermPtr->List_boundingRadius                 = List_boundingRadius;
       midTermPtr->List_speedBasedAnimation            = List_speedBasedAnimation;
       midTermPtr->List_refModel                       = List_refModel;
       midTermPtr->List_location                       = List_location;
       midTermPtr->List_controlConfig                  = List_controlConfig;
       midTermPtr->List_AIconfig                       = List_AIconfig;
       midTermPtr->List_exactCollision                 = List_exactCollision;
       midTermPtr->List_popoutRadius                   = List_popoutRadius;
       midTermPtr->List_poppedOut                      = List_poppedOut;
       //midTermPtr->List_lightObject                    = List_lightObject;
       midTermPtr->List_collisionInfoCount             = List_collisionInfoCount;
       midTermPtr->List_collisionInfoFile              = List_collisionInfoFile;
       midTermPtr->List_isPlaying                      = List_isPlaying;
       midTermPtr->List_validRange                     = List_validRange;
       midTermPtr->List_curAnimAttrib                  = List_curAnimAttrib;
       midTermPtr->List_curAnimPlayDir                 = List_curAnimPlayDir;
       midTermPtr->List_networkEntity                  = List_networkEntity;
       midTermPtr->List_networkFireWeaponQue           = List_networkFireWeaponQue;
       midTermPtr->List_energy                         = List_energy;
       midTermPtr->List_maxEnergy                      = List_maxEnergy;
       midTermPtr->List_networkInterpCurStep           = List_networkInterpCurStep;
       midTermPtr->List_networkInterpCurStepOri        = List_networkInterpCurStepOri;
       midTermPtr->m_hudLinkThirdPerson                = m_hudLinkThirdPerson;           
       midTermPtr->List_miscFloat                      = List_miscFloat;
       midTermPtr->List_miscInt                        = List_miscInt;
       midTermPtr->List_firstPersonModel               = List_firstPersonModel;
       midTermPtr->List_fullModelRef                   = List_fullModelRef;
       midTermPtr->List_networkLastPositionXYZ         = List_networkLastPositionXYZ;
       midTermPtr->List_networkRotateX                 = List_networkRotateX;
       midTermPtr->List_networkRotateY                 = List_networkRotateY;
       midTermPtr->List_networkTargetPositionXYZ       = List_networkTargetPositionXYZ; 
       midTermPtr->List_networkLastDirVect             = List_networkLastDirVect;
       midTermPtr->List_networkTargetDirVect           = List_networkTargetDirVect;
       midTermPtr->List_networkLastUpVect              = List_networkLastUpVect;
       midTermPtr->List_networkTargetUpVect            = List_networkTargetUpVect;
       midTermPtr->List_networkFocus                   = List_networkFocus;
       midTermPtr->List_camFirstPerson                 = List_camFirstPerson;
       midTermPtr->List_camThirdPerson                 = List_camThirdPerson;
       midTermPtr->List_firstPersonRef                 = List_firstPersonRef;
       midTermPtr->List_possesedBy                     = List_possesedBy;
       midTermPtr->List_deathCam                       = List_deathCam;
       midTermPtr->List_deathTimeVarial                = List_deathTimeVarial;
       midTermPtr->List_networkWeaponChangeQue         = List_networkWeaponChangeQue;
       midTermPtr->List_networkAnimReq                 = List_networkAnimReq;
       midTermPtr->List_networkAnimDirReq              = List_networkAnimDirReq;
       midTermPtr->List_networkDefinedID               = List_networkDefinedID;
       midTermPtr->List_hudLink                        = List_hudLink;
       midTermPtr->List_AI_origin                      = List_AI_origin;
       midTermPtr->List_AI_reachedDestination          = List_AI_reachedDestination;
       midTermPtr->m_ridable                           = m_ridable;
       midTermPtr->m_team                              = m_team;
	   midTermPtr->m_raceID                            = m_raceID;
	   midTermPtr->m_filterCollision                   = m_filterCollision;
	   midTermPtr->m_murderer                          = m_murderer;
	   midTermPtr->m_lifeTimeMax                       = m_lifeTimeMax;
	   midTermPtr->m_dynaSpawnID                       = m_dynaSpawnID;
	   midTermPtr->m_clanMember                        = m_clanMember;
	   midTermPtr->m_groupMember					   = m_groupMember;
	   midTermPtr->m_isAI							   = m_isAI;
	   midTermPtr->m_primaryPhysicsOverd               = m_primaryPhysicsOverd;
	   midTermPtr->m_hardPointOverdY                   = m_hardPointOverdY;
	   midTermPtr->m_networkClanID                     = m_networkClanID;
	   midTermPtr->m_autoTriggerTemp                   = m_autoTriggerTemp;
	   midTermPtr->m_spawnTimeStamp                    = GetTickCount();
	   midTermPtr->m_collisionType					   = m_collisionType;
	   midTermPtr->m_uniqueId                          = m_uniqueId;
	   midTermPtr->m_tempEntity                        = m_tempEntity;
	   midTermPtr->m_showShadow						   = m_showShadow;
	   midTermPtr->m_selectionEffectState			   = m_selectionEffectState;
	   midTermPtr->m_filterLodBalancing                = m_filterLodBalancing;

	   midTermPtr->m_hasJoinedArena		               = false;//m_hasJoinedArena;
	   midTermPtr->m_arenaBotID						   = -1;//m_arenaBotID;

	   if(m_badges)
		 CopyMemory(midTermPtr->m_badges, m_badges, sizeof(int) * BADGE_TRACKING);

	   *clone = midTermPtr;

return TRUE;
}
//contains the mesh / file name ect  list of possible animations-//
CMovementObj::CMovementObj(CString EntityName, 
						   int traceNumber)
{
m_selectionEffectState = -1;
D3DVECTOR d3dVectorInit;
d3dVectorInit.x = 0.0f;
d3dVectorInit.y = 0.0f;
d3dVectorInit.z = 0.0f;

ZeroMemory(&m_matrix,sizeof(D3DMATRIX));

m_renderBodyFilter					= 0;
m_hudLinkThirdPerson                = 0;
m_tempChannel                       = 0;
m_animationsDone					= false;
m_collisionRaySys                   = NULL;
m_lifeTimeMax                       = 0;
m_dynaSpawnID                       = -1;
m_networkLastPositionSent.x         = 0.0f;
m_networkLastPositionSent.y         = 0.0f;
m_networkLastPositionSent.z         = 0.0f;
m_networkFirstTimeSendingPos        = TRUE;
m_skills                            = NULL;
m_autoCfg                           = NULL;
m_itemDB                            = NULL;
m_aiObj                             = NULL;
m_currentLOD                        = -1;
List_skeletonLODList                = NULL;
List_armedInventoryDB               = NULL;
List_noInterpolationOverride        = FALSE;
List_idFromLoc                      = NULL;
m_recentSufaceNormal.x              = 0.0f;
m_recentSufaceNormal.y              = 1.0f;
m_recentSufaceNormal.z              = 0.0f;
//List_visualBase                     = NULL;
List_skeletalObject                 = NULL;
List_baseFrame                      = NULL;
m_ridable                           = -1;
List_controlType                    = -1;
List_movementDB                     = NULL;
List_name                           = EntityName;
List_traceNumber                    = traceNumber;
List_lastPosition                   = d3dVectorInit;
List_boundingRadius                 = 0.0f;
List_speedBasedAnimation            = 0.0f;
List_refModel                       = 0;
List_location                       = d3dVectorInit;
List_controlConfig                  = 0;
List_AIconfig                       = 0;
List_exactCollision                 = FALSE;
List_popoutRadius                   = 10000;
List_poppedOut                      = TRUE;
//List_lightObject                    = NULL;
List_collisionInfoCount             = 0;
//List_collisionInfo                  = NULL;
List_collisionInfoFile              = "Unknown";

List_isPlaying                      = FALSE;
List_validRange                     = 200.0f;
List_curAnimAttrib                  = 0;
List_curAnimPlayDir                 = 0;
List_networkEntity                  = FALSE;
List_networkFireWeaponQue           = FALSE;
List_energy                         = 10;
List_maxEnergy                      = 10;
List_networkInterpCurStep           = 0;
List_networkInterpCurStepOri        = 0;
//List_modelType                      = 0;           
List_miscFloat                      = 0.0f;
List_miscInt                        = 0;
List_firstPersonModel               = FALSE;
List_fullModelRef                   = -1;
List_networkLastPositionXYZ         = d3dVectorInit;
List_networkRotateX                 = 0.0f;
List_networkRotateY                 = 0.0f;
List_networkTargetPositionXYZ       = d3dVectorInit; 
List_networkLastDirVect             = d3dVectorInit;
List_networkLastDirVect.z           = 1.0f;
List_networkTargetDirVect           = d3dVectorInit;
List_networkTargetDirVect.z         = 1.0f;
List_networkLastUpVect              = d3dVectorInit;
List_networkLastUpVect.y            = 1.0f;
List_networkTargetUpVect            = d3dVectorInit;
List_networkTargetUpVect.y          = 1.0f;
List_networkFocus                   = FALSE;
List_camFirstPerson                 = -1;
List_camThirdPerson                 = -1;
List_firstPersonRef                 = -1;
List_possesedBy                     = -1;
List_deathCam                       = -1;
List_deathTimeVarial                = 0;
List_networkWeaponChangeQue         = -1;
List_networkAnimReq                 = -1;
List_networkAnimDirReq              = -1;
List_networkDefinedID               = -1;
List_hudLink                        = -1;
List_AIdestination                  = d3dVectorInit;
List_AI_origin                      = d3dVectorInit;
List_AI_reachedDestination          = FALSE;
//init hard code
List_externalVectorTemp.x           = 0.0f;
List_externalVectorTemp.y           = 0.0f;
List_externalVectorTemp.z           = 0.0f;
List_NetworkInterpolateVar          = 0;
List_NetworkUpdateTime              = 0;
List_targetRef                      = NULL;
List_AI_target                      = -1;
List_networkPopoutCount             = 0;
List_collisionInfoBuffer            = 0;
List_networkTextString              = "";
List_currentlyFireing               = FALSE;
List_AI_currentlyHittingWall        = FALSE;
List_SpawnGeneratorLink             = -1;
List_attackerTrace                  = -1;
m_firstPersonMode                   = FALSE;
m_resources                         = NULL;
m_inventory                         = NULL;
m_directionalVector.x               = 0.0f;
m_directionalVector.y               = 0.0f;
m_directionalVector.z               = 1.0f;
m_ridable                           = -1;
m_posses                            = NULL;
m_possedBy                          = NULL;
m_team                              = 0;
m_filterLodBalancing                = FALSE;
m_deathCfg                          = NULL;
m_stats                             = NULL;
m_raceID                            = 0;
m_textureRenderObject               = 0;
m_textureRenderObject2              = 0;
List_networkFreeLookCurPitchLast    = 0.0f;
List_networkFreeLookCurYawLast      = 0.0f;
List_networkFreeLookCurPitchTarget  = 0.0f;
List_networkFreeLookCurYawTarget    = 0.0f;
m_validThisRound                    = TRUE;
m_ovverrideAnimation                = -1;
m_ovverrideStamp                    = 0;
m_ovverrideDuration                 = 0;
m_filterCollision                   = FALSE;
m_collisionMovRefPtr                = NULL;
m_collisionVisionRefPtr             = NULL;
m_murderer                          = FALSE;
m_spawnTimeStamp                    = GetTickCount();
m_clanMember                        = 0;
m_groupMember						= 0;
m_isAI								= true;
m_curHitCounter                     = 0;
m_distanceToCamTemp                 = 0.0f;
m_hardPointOverdY                   = 0.0f;
m_networkClanID                     = 0;
m_aiCfgRef                          = 0;
m_autoTriggerTemp                   = 0;
m_collisionType                     = 1;
m_uniqueId                          = 0;
m_tempEntity                        = 0;
m_showShadow                        = 0;
m_optionalTextMesh					= 0;
m_GotHit							= FALSE;
m_level								= 1;
m_curXP								= 0;
m_nextLvlXP							= 100;
m_percXP							= 0;
m_factionBonus1						= FALSE;
m_factionBonus2						= FALSE;
m_factionBonus3						= FALSE;
m_curFaction						= -1;
m_lastTextArea						= "All";

//Energy Chron Demo
//CHRIS HACK
m_energyMax							= 100.0f;
m_energyCurrent						= 100.0f;
m_energyRate						= 1.0f;

//Multiple missile overrides
m_fireQueIndexOverride				= -1;
m_missileLoaded						= -1;

m_attackAnimOverride				= -1;
m_firstPersonAnimOverride			= -1;
m_quickSlotNumber					= -1;
for(int i = 0; i < 8; i++)
{
	m_woundChannel[i]	= 0;
	m_goodDots[i]		= 0;
	m_badDots[i]		= 0;
}

m_badges = new int[BADGE_TRACKING];
for(int iBa = 0; iBa < BADGE_TRACKING; iBa++)
	m_badges[iBa] = 0;

m_hasJoinedArena = false;
m_arenaBotID = -1;
}



BOOL CMovementObj::GenerateCollisionPolys()
{
/*if(!List_skeletalObject)
    return FALSE;
  
		  List_effCollisionPoly1[0].x = List_skeletalObject->m_box.maxX;
          List_effCollisionPoly1[0].y = List_skeletalObject->m_box.maxY * .6f;
          List_effCollisionPoly1[0].z = 0.0001f;
          List_effCollisionPoly1[1].x = List_skeletalObject->m_box.minX;
          List_effCollisionPoly1[1].y = List_skeletalObject->m_box.maxY * .61f;
          List_effCollisionPoly1[1].z = 0.0002f;
          List_effCollisionPoly1[2].x = 0.0f; 
          List_effCollisionPoly1[2].y = List_skeletalObject->m_box.maxY * .611f;
          List_effCollisionPoly1[2].z = 0.0f;
          List_effCollisionPoly2[0].x = 0.0005f;
          List_effCollisionPoly2[0].y = 0.0001f;
          List_effCollisionPoly2[0].z = 0.0007f;
          List_effCollisionPoly2[1].x = 0.0003f;
          List_effCollisionPoly2[1].y = 0.0009f;
          List_effCollisionPoly2[1].z = -(List_skeletalObject->m_box.maxZ*.7f);
          List_effCollisionPoly2[2].x = 0.0008f; 
          List_effCollisionPoly2[2].y = (List_skeletalObject->m_box.maxY *.4f);
          List_effCollisionPoly2[2].z = List_skeletalObject->m_box.minZ * .3f; */


return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CMovementObj::SetSecondaryTimingIntro(CSkeletonObject * skeletalFocus,long & currentTime)
{
          /*POSITION posLoc = skeletalFocus->m_bonesList->FindIndex(0);
          if(posLoc)
		  {
            CBoneObject * bonePtr = (CBoneObject *) skeletalFocus->m_bonesList->GetAt(posLoc);
            if(bonePtr->m_animationList)
			{
              POSITION animPos = bonePtr->m_animationList->FindIndex(skeletalFocus->m_secondaryAnimationInUse);
		      if(animPos)
			  {
	            CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(animPos);*/
                skeletalFocus->m_secondaryAnimLastUpdatedTime = currentTime;
			  /*}
			}
		  }*/
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CMovementObj::CharacterAnimationManagement(CSkeletonObject * skeletalFocus,float adjustFactor)
{
  //return;

  int transitionTime = 175;
  long currentTime = GetTickCount();
  float cur = List_movementDB->List_curTransZ;
  BOOL reverse = FALSE;
  int animationSelect = 0;
  int animVersionPref = skeletalFocus->m_animPreference;
  
	
  if(m_ovverrideAnimation != -1)
	 List_currentlyFireing = FALSE;
		
	if(List_movementDB->List_curTransZ>=0)
	   reverse = FALSE;
    else
	   reverse = TRUE;
       if(List_movementDB->List_curTransZ == 0){
		  if(!List_movementDB->m_inLiquid)
		  {
		    animationSelect = 1;
		    //stand
		  }
		  else
		  animationSelect = 14;
	   }
	   else{
	      m_ovverrideAnimation = -1;
		  if(!List_movementDB->m_inLiquid)
		  {
		    animationSelect = 3;
		  }
		  else
		  animationSelect = 15;
	   }
    //--------------------------------------------------------//
	//---------------Side step alteration---------------------//
	//--------------------------------------------------------//
    float turnRate = skeletalFocus->m_waistTurnRate*adjustFactor; 
	float cTransX = List_movementDB->List_curTransX;
	if(List_movementDB->List_curTransZ < 0)
    cTransX = -cTransX;
	
	if(cTransX > 0.0f){
       //reverse = FALSE;
	   //skeletalFocus->m_waistBone = 1;
       if(skeletalFocus->m_waistTurn < skeletalFocus->m_waistTurnMax) 
          skeletalFocus->m_waistTurn += turnRate;
	   if(skeletalFocus->m_waistTurn > skeletalFocus->m_waistTurnMax)
	      skeletalFocus->m_waistTurn = skeletalFocus->m_waistTurnMax;
	   if(!List_movementDB->m_inLiquid)
		  {
		    animationSelect = 3;
		  }
		  else
		  animationSelect = 15;

	   m_ovverrideAnimation = -1;
	}
	else if(cTransX < 0.0f){
       //reverse = FALSE;
       //skeletalFocus->m_waistBone = 1;
	   if(skeletalFocus->m_waistTurn > -skeletalFocus->m_waistTurnMax) 
          skeletalFocus->m_waistTurn -= turnRate;
	   if(skeletalFocus->m_waistTurn < -skeletalFocus->m_waistTurnMax)
	      skeletalFocus->m_waistTurn = -skeletalFocus->m_waistTurnMax;
	   if(!List_movementDB->m_inLiquid)
		  {
		    animationSelect = 3;
		  }
		  else
		  animationSelect = 15;

	   m_ovverrideAnimation = -1;
	}
	else{
		if(skeletalFocus->m_waistTurn>0.0f){
		   skeletalFocus->m_waistTurn -= turnRate;
		   if(skeletalFocus->m_waistTurn < turnRate)
			  skeletalFocus->m_waistTurn = 0.0f;
		}
		if(skeletalFocus->m_waistTurn<0.0f){
		   skeletalFocus->m_waistTurn += turnRate;
		   if(skeletalFocus->m_waistTurn > -turnRate)
			  skeletalFocus->m_waistTurn = 0.0f;
		}
	}
    //--------------------------------------------------------//
	//-----------End Side step alteration---------------------//
	//--------------------------------------------------------//

    if(List_movementDB->List_currentlyJumping)
	{
       animationSelect = 5;
	   reverse = FALSE;
	   m_ovverrideAnimation = -1;
	}

	if(List_movementDB->m_jetPackInUse)
	{
       animationSelect = 16;
	   reverse = FALSE;
	   //m_ovverrideAnimation = -1;
	}

    BOOL res = TRUE;
	if((currentTime - skeletalFocus->m_lastScndDurationUpdateTime) > skeletalFocus->m_scndDurationCurrentLength)
	{
	   if(List_currentlyFireing == TRUE)
	   {
         if(skeletalFocus->m_secondaryAnimationInUse == -1)
		 {
		  CBoneAnimationObject* animPtrRet = NULL;
	      skeletalFocus->m_secondaryAnimationInUse = skeletalFocus->GetAnimationBasedOnAttribAndPref(12,skeletalFocus->m_animPreference,&animPtrRet);
          
		  SetSecondaryTimingIntro(skeletalFocus,currentTime);
		  if(animPtrRet)
		  skeletalFocus->m_scndDurationCurrentLength = animPtrRet->m_minPlayDuration;
		  skeletalFocus->m_lastScndDurationUpdateTime = currentTime;
		  //go here
		 }
	   }
       else
	    skeletalFocus->m_secondaryAnimationInUse = -1;
	}


    if(m_ovverrideAnimation != -1)
	{
	   if(m_ovverrideDuration != 0)
	   {
	     if((currentTime-m_ovverrideStamp)>m_ovverrideDuration)
             m_ovverrideAnimation = -1;
		 else{
             skeletalFocus->m_secondaryAnimationInUse = -1;
             animationSelect = m_ovverrideAnimation;
	         reverse = FALSE;
		 }
	   }
	   else{
	   skeletalFocus->m_secondaryAnimationInUse = -1;
       animationSelect = m_ovverrideAnimation;
	   reverse = FALSE;
	   }
	}


	if(this->m_posses){//mount override
       animationSelect = 30;
	   m_ovverrideAnimation = -1;
	   ///skeletalFocus->m_secondaryAnimationInUse = -1;
	   reverse = FALSE;
	}//end mount override


	if(List_energy <= 0){//death override
       animationSelect = 8;
	   skeletalFocus->m_secondaryAnimationInUse = -1;
	   reverse = FALSE;
	}//end death override

	if(skeletalFocus->m_animationInUse == 1)
		skeletalFocus->m_animationInUse = skeletalFocus->m_animationInUse;

	if(skeletalFocus->m_durationCurrentLength != 0){
		if((GetTickCount()-skeletalFocus->m_lastDurationUpdateTime)>skeletalFocus->m_durationCurrentLength){
			
			if(!skeletalFocus->SetAnimationBasedOnAttrib(animationSelect,reverse,animVersionPref,transitionTime))
				skeletalFocus->SetAnimationBasedOnAttrib(animationSelect,reverse,skeletalFocus->m_animPrefDefault,transitionTime);
		}
	}
    else if(!skeletalFocus->SetAnimationBasedOnAttrib(animationSelect,reverse,animVersionPref,transitionTime))
		     skeletalFocus->SetAnimationBasedOnAttrib(animationSelect,reverse,skeletalFocus->m_animPrefDefault,transitionTime);


    if(m_ovverrideAnimation != -1){
       if(m_ovverrideDuration == 0)
		  m_ovverrideDuration = skeletalFocus->m_durationCurrentLength;
	}
}


/*void CMovementObjList::Serialize( CArchive& ar )
{
	CObject::Serialize( ar );
	if(ar.IsStoring() )
	{
	    POSITION pos = GetHeadPosition();
        while (pos)
        {
            CMovementObj * ePtr = (CMovementObj*)GetNext(pos);
			ar.WriteObject(ePtr);
        }
	}
	else
	{
		DWORD_PTR nNewCount = ar.ReadCount();
		while (nNewCount--)
		{
			CMovementObj * newData = NULL;
			newData = (CMovementObj *)ar.ReadObject(NULL);
			AddTail(newData);
			if(version == 50)
		       newData->List_skeletalObject->DeleteUneededServerObjects();
		}
	}
}*/


void CMovementObj::Serialize( CArchive& ar )
{
BOOL fillBool = TRUE;
float fillFloat = 0.0f;
CString fillCstring;
D3DVECTOR reserverdVector;
reserverdVector.x = 0.0f;
reserverdVector.y = 0.0f;
reserverdVector.z = 0.0f;

CObject::Serialize( ar );
	if(ar.IsStoring() ){
       
	//CObject * pointer = NULL;

	   ar  << List_name 
		   //<< List_visualBase 
		   << m_ridable 
		   << List_controlType 
		   << List_movementDB 
		   << List_traceNumber 
		   << m_collisionType 
		   << List_boundingRadius 
		   << List_speedBasedAnimation
		   << List_location.x
		   << List_location.y
		   << List_location.z
		   << List_controlConfig
		   << List_AIconfig
		   << List_exactCollision
		   << List_popoutRadius
		   << m_uniqueId//List_poppedOut;
		   << NULL//List_lightObject
		   << List_collisionInfoFile
		   << m_filterLodBalancing //m_team
		   << m_filterCollision
		   << m_raceID
           << fillCstring
		   << m_primaryPhysicsOverd
           << m_hardPointOverdY
		   << fillCstring
		   << m_tempEntity
		   << m_showShadow
           //<< fillFloat
		   << List_energy
           << List_maxEnergy
		   << m_hudLinkThirdPerson//List_modelType
		   << List_firstPersonModel
		   << List_fullModelRef
		   << List_camFirstPerson
           << List_camThirdPerson
           << List_firstPersonRef
		   << List_deathCam
		   << List_hudLink
		   << List_skeletalObject
		   << List_armedInventoryDB
		   << List_skeletonLODList
		   << m_stats
		   << m_resources
		   << m_inventory
		   << m_itemDB
		   << m_autoCfg
		   << m_skills
		   << m_deathCfg;
		
	   //arrays ect
	   for(int loop=0;loop<4;loop++)
           for(int loop2=0;loop2<4;loop2++)
       ar << m_matrix.m[loop][loop2];
		
	}
	else
	{
	   int test = 0;
       /*ar  >> List_name;
		   //>> List_visualBase 
	   ar  >> m_ridable 
		   >> List_controlType 
		   >> List_movementDB 
		   >> List_traceNumber 
		   >> m_collisionType 
		   >> List_boundingRadius 
		   >> List_speedBasedAnimation
		   >> List_location.x
		   >> List_location.y
		   >> List_location.z
		   >> List_controlConfig
		   >> List_AIconfig
		   >> List_exactCollision
		   >> List_popoutRadius
		   >> m_uniqueId
		   >> test//List_lightObject
		   >> List_collisionInfoFile
		   >> m_filterLodBalancing //m_team
		   >> m_filterCollision
		   >> m_raceID
           >> fillCstring
		   >> m_primaryPhysicsOverd
           >> m_hardPointOverdY
		   >> fillCstring
		   >> m_tempEntity
		   >> m_showShadow
           //>> fillFloat
		   >> List_energy
           >> List_maxEnergy
		   >> m_hudLinkThirdPerson//List_modelTypedsdsds
		   >> List_firstPersonModel
		   >> List_fullModelRef
		   >> List_camFirstPerson
           >> List_camThirdPerson
           >> List_firstPersonRef
		   >> List_deathCam
		   >> List_hudLink
		   >> List_skeletalObject
		   >> List_armedInventoryDB
		   >> List_skeletonLODList
		   >> m_stats
		   >> m_resources
		   >> m_inventory
		   >> m_itemDB
		   >> m_autoCfg
		   >> m_skills
		   >> m_deathCfg;*/

	   ar  >> List_name;
		   //>> List_visualBase 
	   ar  >> m_ridable ;
		   ar  >> List_controlType ;
		   ar  >> List_movementDB ;
		   ar  >> List_traceNumber ;
		   ar  >> m_collisionType ;
		   ar  >> List_boundingRadius ;
		   ar  >> List_speedBasedAnimation;
		   ar  >> List_location.x;
		   ar  >> List_location.y;
		   ar  >> List_location.z;
		   ar  >> List_controlConfig;
		   ar  >> List_AIconfig;
		   ar  >> List_exactCollision;
		   ar  >> List_popoutRadius;
		   ar  >> m_uniqueId;
		   ar  >> test;//List_lightObject
		   ar  >> List_collisionInfoFile;
		   ar  >> m_filterLodBalancing; //m_team
		   ar  >> m_filterCollision;
		  ar   >> m_raceID;
          ar   >> fillCstring;
		   ar  >> m_primaryPhysicsOverd;
           ar  >> m_hardPointOverdY;
		   ar  >> fillCstring;
		   ar  >> m_tempEntity;
		   ar  >> m_showShadow;
           //>> fillFloat
		   ar  >> List_energy;
           ar  >> List_maxEnergy;
		   ar  >> m_hudLinkThirdPerson;//List_modelTypedsdsds
		   ar  >> List_firstPersonModel;
		   ar  >> List_fullModelRef;
		   ar  >> List_camFirstPerson;
           ar  >> List_camThirdPerson;
           ar  >> List_firstPersonRef;
		   ar  >> List_deathCam;
		   ar  >> List_hudLink;
		   ar  >> List_skeletalObject;
		   ar  >> List_armedInventoryDB;
		   ar  >> List_skeletonLODList;
		   ar  >> m_stats;
		   ar  >> m_resources;
		   ar  >> m_inventory;
		  ar   >> m_itemDB;
		  ar   >> m_autoCfg;
		  ar   >> m_skills;
		  ar   >> m_deathCfg;


	   //arrays ect
	   for(int loop=0;loop<4;loop++)
           for(int loop2=0;loop2<4;loop2++)
       ar >> m_matrix.m[loop][loop2];



	   List_poppedOut		   = TRUE;
       m_curHitCounter         = 0;
	   m_aiCfgRef                          = 0;
       m_posses                = NULL;
	   m_possedBy              = NULL;
       List_SpawnGeneratorLink = -1;
	   m_lifeTimeMax                       = 0;
       m_dynaSpawnID                       = -1;

	   m_spawnTimeStamp                    = GetTickCount();
	   m_validThisRound                    = TRUE;
	   m_networkLastPositionSent.x         = 0.0f;
       m_networkLastPositionSent.y         = 0.0f;
       m_networkLastPositionSent.z         = 0.0f;
       m_networkFirstTimeSendingPos        = TRUE;
	   List_noInterpolationOverride = FALSE;
	   List_NetworkInterpolateVar = 0;
	   List_networkDefinedID = -1;
	   List_networkWeaponChangeQue = -1;
	   List_networkAnimReq = 0;
	   List_networkAnimDirReq = 0;

	   List_AI_target = -1;
	   List_AI_currentlyHittingWall = FALSE;
	   List_AI_reachedDestination = FALSE;
	   List_AI_origin.x = 0.0f;
	   List_AI_origin.y = 0.0f;
	   List_AI_origin.z = 0.0f;
	   List_AIdestination.x = 0.0f;
	   List_AIdestination.y = 0.0f;
	   List_AIdestination.z = 0.0f;
       List_networkLastPositionXYZ.x = 0.0f;
       List_networkLastPositionXYZ.y = 0.0f;
	   List_networkLastPositionXYZ.z = 0.0f;
	   List_NetworkUpdateTime = 0;
       List_networkRotateX = 0.0f;
       List_networkRotateY = 0.0f;
	   List_networkTargetPositionXYZ.x = 0.0f;
	   List_networkTargetPositionXYZ.y = 0.0f;
	   List_networkTargetPositionXYZ.z = 0.0f;
       List_networkLastDirVect = List_networkLastPositionXYZ;
       List_networkTargetDirVect = List_networkLastPositionXYZ;
	   List_networkLastUpVect = List_networkLastPositionXYZ;
       List_networkTargetUpVect = List_networkLastPositionXYZ;
       List_networkLastDirVect.z           = 1.0f;
       List_networkTargetDirVect.z         = 1.0f;
       List_networkLastUpVect.y            = 1.0f;
       List_networkTargetUpVect.y          = 1.0f;
 
	   List_currentlyFireing = FALSE;
       List_poppedOut = TRUE;
       List_deathTimeVarial = 0;
       List_possesedBy = -1;
	   List_networkFocus = FALSE;
	   List_miscInt = 0;
	   List_miscFloat = 0.0f;
	   List_networkFireWeaponQue=0;
	   List_idFromLoc=0;
	   List_baseFrame=0;
	   List_lastPosition.x = 0.0f;
	   List_lastPosition.y = 0.0f;
	   List_lastPosition.z = 0.0f;
	   List_isPlaying                      = TRUE;
	   List_externalVectorTemp.x           = 0.0f;
       List_externalVectorTemp.y           = 0.0f;
       List_externalVectorTemp.z           = 0.0f;
	   List_networkInterpCurStep           = 0;
	   List_networkInterpCurStepOri        = 0;
	   List_collisionInfoCount             = 0;
	   //List_collisionInfo                  = 0;
	   List_refModel                       = 0;
	   List_curAnimAttrib                  = 0;
	   List_curAnimPlayDir                 = 0;
	   List_networkEntity                  = FALSE;
	   List_collisionInfoBuffer            = 0;
       List_networkFreeLookCurPitchLast    = 0.0f;
       List_networkFreeLookCurYawLast      = 0.0f;
       List_networkFreeLookCurPitchTarget  = 0.0f;
       List_networkFreeLookCurYawTarget    = 0.0f;
	   List_networkTextString              = "";
       List_targetRef                      = NULL;
	   List_attackerTrace                  = -1;
	   m_cameraTypeInUse                   = -1;
	   m_firstPersonMode                   = FALSE;
	   m_currentLOD                        = -1;
	   m_aiObj                             = NULL;
	   m_recentSufaceNormal.x              = 0.0f;
       m_recentSufaceNormal.y              = 1.0f;
       m_recentSufaceNormal.z              = 0.0f;
	   m_textureRenderObject               = 0;
	   m_textureRenderObject2              = 0;
	   m_ovverrideAnimation                = -1;
	   m_ovverrideStamp                    = 0;
	   m_ovverrideDuration                 = 0;
	   m_collisionMovRefPtr                = NULL;
       m_collisionVisionRefPtr             = NULL;
	   m_murderer                          = FALSE;
       m_networkClanID                     = 0;
	   m_clanMember                        = 0;
	   m_groupMember					   = 0;
	   m_isAI							   = true;
	   m_distanceToCamTemp                 = 0.0f;
	   m_autoTriggerTemp                   = 0;
	   m_GotHit							   = FALSE;
	   m_level							   = 1;
	   m_curXP							   = 0;
	   m_nextLvlXP						   = 100;
	   m_percXP							   = 0;
	   m_badges							   = 0;
	   m_factionBonus1					   = FALSE;
	   m_factionBonus2					   = FALSE;
	   m_factionBonus3					   = FALSE;
	   m_curFaction						   = -1;  
	   m_lastTextArea					   = "All";
	}
}

BOOL CMovementObj::Mount(CMovementObj * target)
{
	//if(!target->m_aiObj)
	//	return FALSE;
	    
	    if(target->m_aiObj)
	    target->m_aiObj->m_controlled = TRUE;
		target->List_possesedBy = List_traceNumber;
        m_posses = target;
	    target->m_possedBy = this;	
		this->List_movementDB->List_freeLookCurPitch = 0.0f;
		this->List_movementDB->List_freeLookCurYaw = 0.0f;
		this->List_movementDB->List_curTransX = 0.0f;
	    this->List_movementDB->List_curTransY = 0.0f;
	    this->List_movementDB->List_curTransZ = 0.0f;
		return TRUE;
}

BOOL CMovementObj::DeMount()
{
	if(!m_posses)
		return FALSE;

        D3DVECTOR lastTangentVect = m_posses->m_collisionRaySys->m_lastCenterMassPosition;

        D3DVECTOR position;
		m_posses->List_baseFrame->GetPosition(NULL,&position);
		m_posses->List_possesedBy = -1;
	    m_posses->m_possedBy = NULL;	
        D3DMATRIX newMatrix;
		List_baseFrame->GL_matrixFunctions.InitMat(newMatrix.m);
		newMatrix._41 = position.x;
		newMatrix._42 = position.y;
		newMatrix._43 = position.z;
		List_baseFrame->AddTransformReplace(newMatrix);	
		this->List_movementDB->List_freeLookCurPitch = 0.0f;
		this->List_movementDB->List_freeLookCurYaw = 0.0f;
		this->List_movementDB->List_curTransX = 0.0f;
	    this->List_movementDB->List_curTransY = 0.0f;
	    this->List_movementDB->List_curTransZ = 0.0f;
		m_posses = NULL;
	    this->List_movementDB->List_lastPosition.x = newMatrix._41;
        this->List_movementDB->List_lastPosition.y = newMatrix._42;
        this->List_movementDB->List_lastPosition.z = newMatrix._43;

		if(m_collisionRaySys)
		{
		   m_collisionRaySys->m_lastCenterMassPosition = lastTangentVect;
		   //delete m_collisionRaySys;
		   //m_collisionRaySys = 0;
		}


		return TRUE;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
CMovementObj * CMovementObjList::GetObjByNetworkID(int id)
{
    if(m_directAccessArrayNID)
	{//speed
	   if(id < m_directAccessCount && id > -1)
	      if(m_directAccessArrayNID[id].m_inUse == 1)
		     return m_directAccessArrayNID[id].m_mPtr;
	}//end speed
	
	
	
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		if(mPtr->List_networkDefinedID == id)
           return mPtr;
	}
	return NULL;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
CMovementObj * CMovementObjList::GetItemByNetworkID(int id)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		if(mPtr->List_controlType == 35)
		if(mPtr->List_networkDefinedID == id)
           return mPtr;
	}
	return NULL;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
CMovementObj * CMovementObjList::GetObjectByDynaID(int id)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		if(mPtr->m_dynaSpawnID == id)
           return mPtr;
	}
	return NULL;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
int CMovementObjList::GetObjectCountByDynaID(int id)
{
	int count = 0;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		if(mPtr->List_energy > 0)
		if(mPtr->m_dynaSpawnID == id)
           count++;
	}
	return count;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
int CMovementObjList::GetFreeID()
{
    int trace = m_idStartIndex;

    if(m_directAccessArray)
	{//direct access mode-dam fast
       while(trace < 30000)
	   { 
           if(m_directAccessArray[trace].m_inUse == 0)
		   {
		      m_idStartIndex = trace+1;
	          if(m_idStartIndex > 30000)
                 m_idStartIndex = 1;
		      return trace;
		   }

		   trace++;
	   }
       trace = 1;
	   m_idStartIndex = 1;
	   while(trace < 30000)
	   { 
           if(m_directAccessArray[trace].m_inUse == 0)
		   {
		      m_idStartIndex = trace+1;
	          if(m_idStartIndex > 30000)
                 m_idStartIndex = 1;
		      return trace;
		   }

		   trace++;
	   }
       m_idStartIndex = trace+1;
	   if(m_idStartIndex > 30000)
          m_idStartIndex = 1;
       return trace;
	}//end direct access mode-dam fast


	BOOL done = FALSE;
    while(!done)
	{
      done = TRUE;
	  for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
          CMovementObj * spnPtr = (CMovementObj *)GetNext(posLoc);
	      if(spnPtr->List_traceNumber == trace)
		  {
		      done = FALSE;
			  trace++;
			  break;
		  }
	  }

	}

	m_idStartIndex = trace+1;
	if(m_idStartIndex > 30000)
       m_idStartIndex = 1;
    return trace;
}
//------------------------------------------------------------//
int CMovementObjList::GetNextHighID()
{
	int idCurrent = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
	    CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		if(mPtr->m_uniqueId > idCurrent)
		   idCurrent = mPtr->m_uniqueId;
	}
	return idCurrent + 1;
}
//------------------------------------------------------------//
void CMovementObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
	    CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		InvalidateIndexedByID(mPtr);
		delete mPtr;
		mPtr = 0;
	}
	RemoveAll();
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
CMovementObj * CMovementObjList::GetObjByIndex(int index)
{
    if(index < 0)
	return NULL;
 

	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;
	    CMovementObj * mPtr = (CMovementObj *)GetAt(posLoc);
	return mPtr;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
CMovementObj * CMovementObjList::GetObjByName(CString name)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		//if(mPtr->List_name == name)
		if(mPtr->List_name.CompareNoCase(name) == 0)
           return mPtr;
	}
	return NULL;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
CMovementObj * CMovementObjList::GetObjByTrace(int trace)
{
    if(m_directAccessArray)
	{//speed
	   if(trace < m_directAccessCount && trace > -1)
	      if(m_directAccessArray[trace].m_inUse == 1)
		     return m_directAccessArray[trace].m_mPtr;
	}//end speed
	
	
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		if(mPtr->List_traceNumber == trace)
           return mPtr;
	}
	return NULL;
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
CMovementObj * CMovementObjList::GetObjByControlType(int ctype)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		if(mPtr->List_controlType == ctype)
           return mPtr;
	}
	return NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CMovementObj::ArmAttachment(int type,
								 int fireQueOvrd,
								 CArmedInventoryList * globalArmedInvenList,
								 LPDIRECT3DDEVICE9 g_pd3dDevice,
								 int locationID,
								 CMovementObj * refDBEntity)
{
       if(!globalArmedInvenList)//valid inventory list
       return FALSE;

	   CArmedInventoryObj * armedPos = globalArmedInvenList->GetByTypeDirectAccess(type);
	   if(!armedPos)
	       return FALSE;

       //handle dynamic remap
	   if(List_skeletalObject)
       if(armedPos->m_boneMapping.GetLength() > 0)
	   {
          armedPos->m_boneAttached = List_skeletalObject->GetBoneIndexByName(armedPos->m_boneMapping);
	   }

            CArmedInventoryObj * appendageCloned = NULL;
			armedPos->Clone(&appendageCloned,g_pd3dDevice);
            if(!List_armedInventoryDB)
				List_armedInventoryDB = new CArmedInventoryList();

			appendageCloned->m_abilityTypeUse	= locationID;
			appendageCloned->m_fireQueOvrd		= fireQueOvrd;

			if(List_controlType == 5){//ai custom
				if(m_aiObj)
				if(appendageCloned->m_functionalityAttribute == 1){//weapon
                   m_aiObj->m_minAttackDistance = (float)appendageCloned->m_miscInt3;
				}//end weapon		
			}//end ai custom

            List_armedInventoryDB->AddTail(appendageCloned);
		return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int CMovementObj::ArmItemByGLID(int gl_id,
						        CMovementObj * refDBObj,
								void * itemManagerMem,
								LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   if(!refDBObj)
	   return 0;

   CItemManager * iManager = (CItemManager *)itemManagerMem;

   return iManager->ArmEntity(gl_id,this,refDBObj,g_pd3dDevice);
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
CItemObj * CMovementObj::GetItemByID(int gl_id)
{
   for(POSITION posLoc = m_itemDB->GetHeadPosition();posLoc != NULL;)
   {
       CItemObj * itemPtr = (CItemObj *)m_itemDB->GetNext(posLoc);
       if(itemPtr->m_globalID == gl_id)
          return itemPtr;
   }
  return NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int CMovementObj::DisarmItemByGLID(int gl_id,
								   BOOL override,
								   CMovementObj * refDBObj,
								   LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   if(!m_itemDB)
	   return 0;

   int trace = 0;
   for(POSITION posLoc = m_itemDB->GetHeadPosition();posLoc != NULL;)
   {
       CItemObj * itemPtr = (CItemObj *)m_itemDB->GetNext(posLoc);
       if(itemPtr->m_globalID == gl_id)
	   {
		   if(DisarmItem(itemPtr,override,refDBObj,g_pd3dDevice)==1)
			  return 1;

		  return 0;
	   }
	   trace++;
   }
   return 0;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int CMovementObj::ArmNextLocationId(int locationID,
						            CMovementObj * refDBObj,
									void * itemManagerMem,
									LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    if(!m_itemDB)
        return 0;
        
	int glid = -1;

    for(POSITION armedPos = m_itemDB->GetHeadPosition();armedPos != NULL;)
	{
        CItemObj * itemPtr = (CItemObj *)m_itemDB->GetNext(armedPos);
        if(itemPtr->m_locationID == locationID)
		{//id
		   glid = itemPtr->m_globalID;
		   break;
 
		}//end not id		
	}

	if(glid == -1)//get first avail
	for(POSITION bPos = refDBObj->m_itemDB->GetHeadPosition();bPos != NULL;)
	{
        CItemObj * itemPtr = (CItemObj *)refDBObj->m_itemDB->GetNext(bPos);
        if(itemPtr->m_locationID == locationID)
		{
           ArmItemByGLID(itemPtr->m_globalID,
						 refDBObj,
						 itemManagerMem,
						 g_pd3dDevice);
		   return 1;
		}
	}

	BOOL foundcuurentPoint = FALSE;
	POSITION bPos;
	for(bPos = refDBObj->m_itemDB->GetHeadPosition();bPos != NULL;)
	{
        CItemObj * itemPtr = (CItemObj *)refDBObj->m_itemDB->GetNext(bPos);
        
		if(glid == itemPtr->m_globalID)
		{
           foundcuurentPoint = TRUE;
           continue;
		}

		if(foundcuurentPoint)
		if(itemPtr->m_locationID == locationID)
		{
           ArmItemByGLID(itemPtr->m_globalID,
						 refDBObj,
						 itemManagerMem,
						 g_pd3dDevice);
		   return 1;
		}
	}

	for(bPos = refDBObj->m_itemDB->GetHeadPosition();bPos != NULL;)
	{
        CItemObj * itemPtr = (CItemObj *)refDBObj->m_itemDB->GetNext(bPos);
        if(itemPtr->m_locationID == locationID)
		{
           ArmItemByGLID(itemPtr->m_globalID,
						 refDBObj,
						 itemManagerMem,
						 g_pd3dDevice);
		   return 1;
		}
	}

return 1;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CMovementObj::DisarmAttachment(int type,
									LPDIRECT3DDEVICE9 g_pd3dDevice,
								    CMovementObj * refDBEntity)
{
       if(!List_armedInventoryDB)
		return FALSE;

	   POSITION posLast;
       for(POSITION posLoc = List_armedInventoryDB->GetHeadPosition();(posLast = posLoc) != NULL;)
	   {
           CArmedInventoryObj * armedPtr = (CArmedInventoryObj *)List_armedInventoryDB->GetNext(posLoc);
           if(armedPtr->m_type == type){
			  armedPtr->SafeDelete();
	      	  delete armedPtr;
		      armedPtr=0;
              List_armedInventoryDB->RemoveAt(posLast);
              return TRUE;
		   }
	   }
	return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CMovementObj::ChangeMissileIndexByLoc(int firequeIndex, int locationID, int missileIndex)
{
	if(!List_armedInventoryDB)
		return;

	for(POSITION posLoc = List_armedInventoryDB->GetHeadPosition(); posLoc != NULL;)
	{
		CArmedInventoryObj *armedPtr = (CArmedInventoryObj *)List_armedInventoryDB->GetNext(posLoc);
		CFireRangeObject *fireObj = NULL;
		if(armedPtr)
		{
			if(armedPtr->m_firstPersonSystem)
				if(armedPtr->m_firstPersonSystem->m_fireQueList)
					fireObj = armedPtr->m_firstPersonSystem->m_fireQueList->GetByFireQueIndex(firequeIndex);
			if(armedPtr->m_thirdPersonSystem)
				if(armedPtr->m_thirdPersonSystem->m_fireQueList)
					fireObj = armedPtr->m_thirdPersonSystem->m_fireQueList->GetByFireQueIndex(firequeIndex);
		}

		if(fireObj)
			fireObj->m_missileIndex = missileIndex;
	}

/*	if(!m_inventory)
		return;

	CInventoryObj *weaponLink = m_inventory->GetArmedInvenItemByLocationID(locationID);

	if(weaponLink)
	{
		if(weaponLink->m_itemData)
		{
			if(weaponLink->m_itemData->m_attachmentRefList)
			{
				for(POSITION posLoc = weaponLink->m_itemData->m_attachmentRefList->GetHeadPosition(); posLoc != NULL;)
				{
					CAttachmentRefObj *ref = (CAttachmentRefObj *)weaponLink->m_itemData->m_attachmentRefList->GetNext(posLoc);
					CArmedInventoryObj *visObj = List_armedInventoryDB->GetByTypeDirectAccess(ref->m_attachmentType);
					CFireRangeObject *fireObj = NULL;
					if(visObj)
					{
						if(visObj->m_firstPersonSystem)
							if(visObj->m_firstPersonSystem->m_fireQueList)
								fireObj = visObj->m_firstPersonSystem->m_fireQueList->GetByFireQueIndex(firequeIndex);
						if(visObj->m_thirdPersonSystem)
							if(visObj->m_thirdPersonSystem->m_fireQueList)
								fireObj = visObj->m_thirdPersonSystem->m_fireQueList->GetByFireQueIndex(firequeIndex);
					}

					if(fireObj)
						fireObj->m_missileIndex = missileIndex;
				}
			}
		}
	}
*/
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int CMovementObj::DisarmItem(CItemObj * itemPtr,BOOL override,CMovementObj * refDBObj,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    if(!itemPtr)
        return 0;

	        if(itemPtr->m_dimCfgChangeTo > -1)
			{
			   if(List_skeletalObject->m_spawnCfg)
               refDBObj->List_skeletalObject->HotSwapDim((int)itemPtr->m_dimCfgIndex,
						                                 List_skeletalObject->m_spawnCfg[itemPtr->m_dimCfgIndex],
						                                 List_skeletalObject,
						                                 g_pd3dDevice,
														 FALSE);
			}

			if(itemPtr->m_attachmentRefList){
			   for(POSITION atPos = itemPtr->m_attachmentRefList->GetHeadPosition();atPos != NULL;)
			   {
			       CAttachmentRefObj * attObj = (CAttachmentRefObj *)itemPtr->m_attachmentRefList->GetNext(atPos);
				  
				   if(attObj->m_raceBind == -1 || attObj->m_raceBind == m_uniqueId)
				   {
					   if(attObj->m_dimSwitchTo > -1)
					   {
						   if(List_skeletalObject->m_spawnCfg)
						   refDBObj->List_skeletalObject->HotSwapDim((int)attObj->m_dimIndex,
																		List_skeletalObject->m_spawnCfg[attObj->m_dimIndex],
																		List_skeletalObject,
																		g_pd3dDevice,
																		FALSE);
					   }
					   
					   if(DisarmAttachment(attObj->m_attachmentType,g_pd3dDevice,refDBObj)==0)
					   {
						  //could not find but proceed
					   }
				   }
			   }
			}
			//itemPtr->m_equiped = FALSE;
			if(itemPtr->m_animAccessValue != -1)
			   List_skeletalObject->m_animPreference = 0;
		//}
return 1;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CMovementObj::ExecCfg(CCfgObjList * cfg,
						   CMovementObj * refDBObj,
						   void * itemManagerMem,
						   LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   if(!cfg)
	   return FALSE;

   for(POSITION posLoc = cfg->GetHeadPosition();posLoc != NULL;)
   {
      CCfgObj * cfgPtr = (CCfgObj *)cfg->GetNext(posLoc);
	  if(cfgPtr->m_armed == TRUE && cfgPtr->m_quanity > 0)
      if(ArmItemByGLID(cfgPtr->m_globalCFG,
					   refDBObj,
					   itemManagerMem,
					   g_pd3dDevice)!= 1){


	  }
   }


return TRUE;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CMovementObj::ApplyLODProcessing(int strCount,LODOptCondition * lodProcArray, int rangeCount, lodRangeBasis * lodRange)
{
        //setup ranges
        for(int i=0;i<rangeCount;i++)
		{ 
            if(List_skeletalObject->m_box.maxY >= lodRange[i].min && List_skeletalObject->m_box.maxY <= lodRange[i].max)
			{//found range
               //clobber existing
               if(List_skeletonLODList)
			   {
                  List_skeletonLODList->SafeDelete();
				  delete List_skeletonLODList;
				  List_skeletonLODList = 0;
			   }
			   List_skeletonLODList = new CSkeletalLODObjectList();
                //CSkeletalLODObject
               for(int j=0;j<lodRange[i].lodStepCount;j++)
			   {
			       CSkeletalLODObject * lodRangePtr = new CSkeletalLODObject();
                   lodRangePtr->m_distance = lodRange[i].lodSteps[j];
				   List_skeletonLODList->AddTail(lodRangePtr);
			   }
			   break;
			}//end found range
		}

        List_skeletalObject->RemoveLodsByStruct(strCount,lodProcArray);
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CMovementObj::ApplyLODProcessing(int removeFrontLods)
{
	for(int loop=0;loop<removeFrontLods;loop++)
	{
        List_skeletalObject->RemoveLODFromChain(0);
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CMovementObj::FadeOut(long duration)
{
	if(List_skeletalObject)
		List_skeletalObject->FadeOut(duration);
	if(List_armedInventoryDB)
				for(POSITION attPos = List_armedInventoryDB->GetHeadPosition();attPos != NULL;)
				{
				  CArmedInventoryObj * armedInvObj = (CArmedInventoryObj *)List_armedInventoryDB->GetNext(attPos);
				  if(!armedInvObj->m_thirdPersonSystem)
					  continue;
					  armedInvObj->m_thirdPersonSystem->FadeOut(duration); 
				}

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CMovementObj::ResetMaterial()
{
	if(List_skeletalObject)
		List_skeletalObject->ResetMaterial();
	if(List_armedInventoryDB)
				for(POSITION attPos = List_armedInventoryDB->GetHeadPosition();attPos != NULL;)
				{
				  CArmedInventoryObj * armedInvObj = (CArmedInventoryObj *)List_armedInventoryDB->GetNext(attPos);
				  if(!armedInvObj->m_thirdPersonSystem)
					  continue;
					  armedInvObj->m_thirdPersonSystem->ResetMaterial(); 
				}

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CMovementObj::SetOverrideTrans(float percentage)
{
	if(List_skeletalObject)
		List_skeletalObject->SetOverrideTrans(percentage);
	if(List_armedInventoryDB)
				for(POSITION attPos = List_armedInventoryDB->GetHeadPosition();attPos != NULL;)
				{
				  CArmedInventoryObj * armedInvObj = (CArmedInventoryObj *)List_armedInventoryDB->GetNext(attPos);
				  if(!armedInvObj->m_thirdPersonSystem)
					  continue;
					  armedInvObj->m_thirdPersonSystem->SetOverrideTrans(percentage); 
				}

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CMovementObj::FadeIn(long duration)
{

    List_skeletalObject->FadeIn(duration);
	if(List_armedInventoryDB)
				for(POSITION attPos = List_armedInventoryDB->GetHeadPosition();attPos != NULL;)
				{
				  CArmedInventoryObj * armedInvObj = (CArmedInventoryObj *)List_armedInventoryDB->GetNext(attPos);				  
				  if(!armedInvObj->m_thirdPersonSystem)
					  continue;
					  armedInvObj->m_thirdPersonSystem->FadeIn(duration); 
				}

return TRUE;
}
//------------------------------------------------------------------//
BOOL CMovementObjList::RebuildIds(int offset)
{
	 int idGen = offset;
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMovementObj * entObj  = (CMovementObj *)GetNext(posLoc);
		entObj->m_uniqueId = idGen;
		idGen++;
	 }
	 return TRUE;
}
//------------------------------------------------------------------//
void CMovementObjList::ClearTempEntities()
{
	POSITION posLast = NULL;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
        CMovementObj * entObj  = (CMovementObj *)GetNext(posLoc);
		if(entObj->m_tempEntity)
		{
		   RemoveAt(posLast);
           InvalidateIndexedByID(entObj);
		   delete entObj;
		   entObj = 0;
		}
	 }
}
//------------------------------------------------------------------//
BOOL CMovementObjList::MergeAsTempEntities(CMovementObjList * tempEntitiesDB)
{
	POSITION posLast = NULL;
     for(POSITION posLoc = tempEntitiesDB->GetHeadPosition();(posLast=posLoc) != NULL;)
	 {
        CMovementObj * entObj  = (CMovementObj *)tempEntitiesDB->GetNext(posLoc);
		if(!GetByID(entObj->m_uniqueId))
		{//is not prestent already so i can be added
			entObj->m_tempEntity = TRUE;
            AddTail_Indexed(entObj);
			tempEntitiesDB->RemoveAt(posLast);
		}//end is not prestent already so i can be added
	 }
	 return TRUE;
}
//------------------------------------------------------------------//
CMovementObj * CMovementObjList::GetByID(int uniqueID)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMovementObj * entObj  = (CMovementObj *)GetNext(posLoc);
		if(entObj->m_uniqueId == uniqueID)
		   return entObj;
	 }
	 return NULL;
}
//------------------------------------------------------------------//
CMovementObj * CMovementObjList::GetByTraceAlive(int trace, int filterTrace)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMovementObj * entObj  = (CMovementObj *)GetNext(posLoc);
		if(entObj->List_traceNumber == trace)
		  if(entObj->List_energy > 0)
		   if(entObj->List_traceNumber != filterTrace)
		      return entObj;
	 }
	 return NULL;
}
//------------------------------------------------------------------//
CMovementObj * CMovementObjList::GetByName(CString name)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMovementObj * entObj  = (CMovementObj *)GetNext(posLoc);
		if(entObj->List_name.CompareNoCase(name)==0)
		   return entObj;
	 }
	 return NULL;
}
//------------------------------------------------------------------//
CFrameObj * CMovementObjList::GetFrameByTraceAlive(int trace, int filterTrace)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMovementObj * entObj  = (CMovementObj *)GetNext(posLoc);
		if(entObj->List_traceNumber == trace)
		  if(entObj->List_energy > 0)
		   if(entObj->List_traceNumber != filterTrace)
		      return entObj->List_baseFrame;
	 }
	 return NULL;
}
//------------------------------------------------------------------//
int CMovementObjList::GetPhysicalIndexByUniqueID(int uniqueID)
{
	 int trace = 0;
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMovementObj * entObj  = (CMovementObj *)GetNext(posLoc);
		if(entObj->m_uniqueId == uniqueID)
		   return trace;
		trace++;
	 }
	 return -1;
}
//----------------transition attachment id---------------------------//
void CMovementObjList::TransitionItemAttachmentID(int currentAttachmentID,int newAttachmentID)
{//
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMovementObj * entObj  = (CMovementObj *)GetNext(posLoc);
		if(entObj->m_itemDB)
		for(POSITION posEnt = entObj->m_itemDB->GetHeadPosition();posEnt != NULL;)
		{
            CItemObj * itemPtr = (CItemObj *)entObj->m_itemDB->GetNext(posEnt);
			if(itemPtr->m_attachmentRefList)
			   for(POSITION posAttach = itemPtr->m_attachmentRefList->GetHeadPosition();posAttach != NULL;)
			   {
                   CAttachmentRefObj * atPtr = (CAttachmentRefObj *)itemPtr->m_attachmentRefList->GetNext(posAttach);
				   if(atPtr->m_attachmentType == currentAttachmentID)
				      atPtr->m_attachmentType = newAttachmentID;
			   }
		}
	 }
}
//-------------------------------------------------------------------//
void CMovementObjList::ExportToSQL()
{
    ConnectionStructureClass connectionStructTemp;
    connectionStructTemp.AquireFromCfgFile("baseAccount.cfg");
	
    CDBConnectionSystem * dbConnection = new CDBConnectionSystem(connectionStructTemp.m_sqlServerIP,
			                                                     connectionStructTemp.m_sqlServerCatalog,
																 connectionStructTemp.m_sqlServerUsername,
																 connectionStructTemp.m_sqlServerPassword,
																 NULL);


	if(!dbConnection->m_connectionEstablished)
	{
		//if(eventLog)
	    //  eventLog->AddEvent("ExportToSQL() Aborted due to connection not being established");
        return;
	}

    CString clearFromGil		= "DELETE FROM GM_EDB";
    CString addObject           = "INSERT INTO GM_EDB (ENTITYNAME, EDB_ID, DIMCOUNT) Values('$ENTITYNAME','$EDB_ID','$DIMCOUNT')";

	CABFunctionLib cablib;
	CString temp;
	dbConnection->ExecQuery(clearFromGil);

	int trace = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CMovementObj * aiObj  = (CMovementObj *)GetNext(posLoc);
		
        temp = addObject;
		temp.Replace("$ENTITYNAME",aiObj->List_name);
		temp.Replace("$EDB_ID",cablib.ConvToString(trace));
		if(aiObj->List_skeletalObject)
		   temp.Replace("$DIMCOUNT",cablib.ConvToString(aiObj->List_skeletalObject->m_deformableMeshArrayCount));
		else
		   temp.Replace("$DIMCOUNT","0");
		dbConnection->ExecQuery(temp);
		trace++;
	}

	delete dbConnection;
	dbConnection=0;
}
//-------------------------------------------------------------------//
BOOL CMovementObjList::ExportToSQL(CString ip, CString database, CString usename, CString password)
{
	
    CDBConnectionSystem * dbConnection = new CDBConnectionSystem(ip,
			                                                     database,
																 usename,
																 password,
																 NULL);


	if(!dbConnection->m_connectionEstablished)
	{
		//if(eventLog)
	    //  eventLog->AddEvent("ExportToSQL() Aborted due to connection not being established");
        return FALSE;
	}

    CString clearFromGil		= "DELETE FROM GM_EDB";
    CString addObject           = "INSERT INTO GM_EDB (ENTITYNAME, EDB_ID, DIMCOUNT) Values('$ENTITYNAME','$EDB_ID','$DIMCOUNT')";

	CABFunctionLib cablib;
	CString temp;
	dbConnection->ExecQuery(clearFromGil);

	int trace = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CMovementObj * aiObj  = (CMovementObj *)GetNext(posLoc);
		
        temp = addObject;
		temp.Replace("$ENTITYNAME",aiObj->List_name);
		temp.Replace("$EDB_ID",cablib.ConvToString(trace));
		if(aiObj->List_skeletalObject)
		   temp.Replace("$DIMCOUNT",cablib.ConvToString(aiObj->List_skeletalObject->m_deformableMeshArrayCount));
		else
		   temp.Replace("$DIMCOUNT","0");
		dbConnection->ExecQuery(temp);
		trace++;
	}

	delete dbConnection;
	dbConnection=0;
	return TRUE;
}
//-----------------------------------------------------------------------//
POSITION CMovementObjList::AddTail_Indexed(CObject * newElement)
{
	if(!m_directAccessArray)
	    return AddTail(newElement);

    CMovementObj * mPtr = (CMovementObj *)newElement;
	if(mPtr->List_traceNumber >= m_directAccessCount || mPtr->List_traceNumber < 0)
        return NULL;

	m_directAccessArray[mPtr->List_traceNumber].m_inUse = 1;
	m_directAccessArray[mPtr->List_traceNumber].m_mPtr = (CMovementObj *)newElement;
	m_directAccessArray[mPtr->List_traceNumber].m_posInList = AddTail(newElement);

    if(mPtr->List_networkDefinedID < m_directAccessCount && mPtr->List_networkDefinedID > -1)
	{//index networkID
        m_directAccessArrayNID[mPtr->List_networkDefinedID].m_inUse = 1;
	    m_directAccessArrayNID[mPtr->List_networkDefinedID].m_mPtr = (CMovementObj *)newElement;
	}//end index networkID


	return m_directAccessArray[mPtr->List_traceNumber].m_posInList;
}
//-----------------------------------------------------------------------//
POSITION CMovementObjList::AddHead_Indexed(CObject * newElement)
{
	if(!m_directAccessArray)
	    return AddHead(newElement);

    CMovementObj * mPtr = (CMovementObj *)newElement;
	if(mPtr->List_traceNumber >= m_directAccessCount || mPtr->List_traceNumber < 0)
        return NULL;

	m_directAccessArray[mPtr->List_traceNumber].m_inUse = 1;
	m_directAccessArray[mPtr->List_traceNumber].m_mPtr = (CMovementObj *)newElement;
	m_directAccessArray[mPtr->List_traceNumber].m_posInList = AddHead(newElement);

    if(mPtr->List_networkDefinedID < m_directAccessCount && mPtr->List_networkDefinedID > -1)
	{//index networkID
        m_directAccessArrayNID[mPtr->List_networkDefinedID].m_inUse = 1;
	    m_directAccessArrayNID[mPtr->List_networkDefinedID].m_mPtr = (CMovementObj *)newElement;
	}//end index networkID

	return m_directAccessArray[mPtr->List_traceNumber].m_posInList;
	
}
//-----------------------------------------------------------------------//
BOOL CMovementObjList::DeleteByPtr(CMovementObj * mPtr)
{
    return DeleteByID(mPtr->List_traceNumber);
}
//-----------------------------------------------------------------------//
void CMovementObjList::SetNetID(CMovementObj * mPtr, int netID)
{
	mPtr->List_networkDefinedID = netID;
    if(mPtr->List_networkDefinedID < m_directAccessCount && mPtr->List_networkDefinedID > -1)
	{//index networkID
        m_directAccessArrayNID[mPtr->List_networkDefinedID].m_inUse = 1;
	    m_directAccessArrayNID[mPtr->List_networkDefinedID].m_mPtr = mPtr;
	}//end index networkID
}
//-----------------------------------------------------------------------//
BOOL CMovementObjList::DeleteByID(int id)
{
    if(!m_directAccessArray)
	{//non direct access redundancy
		POSITION posLast;
        for(POSITION posLoc = GetHeadPosition();(posLast=posLoc) != NULL;)
		{
            CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
			if(mPtr->List_traceNumber == id)
			{
				RemoveAt(posLast);
                delete mPtr;
				mPtr = 0;
				return TRUE;
			}
		}
		return FALSE;
	}//end non direct access redundancy
    if(id > m_directAccessCount || id < 0)
        return FALSE;

	if(m_directAccessArray[id].m_inUse == 0)
        return FALSE;

	if(m_directAccessArray[id].m_mPtr->List_networkDefinedID < m_directAccessCount && m_directAccessArray[id].m_mPtr->List_networkDefinedID > -1)
        m_directAccessArrayNID[m_directAccessArray[id].m_mPtr->List_networkDefinedID].m_inUse = 0;

	m_directAccessArray[id].m_inUse = 0;
    RemoveAt(m_directAccessArray[id].m_posInList);//Make sure this is not done twicw
	//m_directAccessArray[id].m_mPtr->SafeDelete();
	delete m_directAccessArray[id].m_mPtr;
	m_directAccessArray[id].m_mPtr = 0;

	return TRUE;
}
//-----------------------------------------------------------------------//
void CMovementObjList::NotifySpawnList(CSpawnObjList * spawnList,CMovementObj * mPtr)
{
    if(spawnList)
	{//add 
		if(mPtr->List_SpawnGeneratorLink > -1)
		{//was generated
		    POSITION posLocal = spawnList->FindIndex(mPtr->List_SpawnGeneratorLink);
		    if(posLocal)
			{
		    CSpawnObj*spwnObj = (CSpawnObj*)spawnList->GetAt(posLocal);
			spwnObj->List_currentOutPut--;//
			  if(spwnObj->List_currentOutPut < 0)
			     spwnObj->List_currentOutPut = 0;
		    }
		}
	}
}



//-----------------------------------------------------------------------//
void CMovementObjList::EnableDirectAccess(int maxID)
{
	m_directAccessCount = maxID;
    m_directAccessArray = new cmovmMemObj[m_directAccessCount];
	m_directAccessArrayNID = new cmovmMemObj[m_directAccessCount];
	ZeroMemory(m_directAccessArray,sizeof(cmovmMemObj)*m_directAccessCount);
	ZeroMemory(m_directAccessArrayNID,sizeof(cmovmMemObj)*m_directAccessCount);
}
//-----------------------------------------------------------------------//
BOOL CMovementObjList::InvalidateIndexedByID(CMovementObj * mPtr)
{
    if(!m_directAccessArray)
		return FALSE;

	if(mPtr->List_traceNumber >= m_directAccessCount || mPtr->List_traceNumber < 0)
	return FALSE;

    m_directAccessArray[mPtr->List_traceNumber].m_inUse = 0;
	if(mPtr->List_networkDefinedID > -1 && mPtr->List_networkDefinedID < m_directAccessCount)
	   m_directAccessArrayNID[mPtr->List_networkDefinedID].m_inUse = 0;

	return TRUE;
}
//------------------------------------------------------------------//
void CMovementObjList::ApplyLODProcessing(int removeFrontLods)
{
	m_lodScriptHasBeenExecuted = 1;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * aiObj  = (CMovementObj *)GetNext(posLoc);
	    aiObj->ApplyLODProcessing(removeFrontLods);
	}
}
//------------------------------------------------------------------//
void CMovementObjList::ApplyLODProcessing(int strCount,
										  LODOptCondition * lodProcArray,
										  int rangeCount,
										  lodRangeBasis * lodRange)
{
	m_lodScriptHasBeenExecuted = 1;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * aiObj  = (CMovementObj *)GetNext(posLoc);
	    aiObj->ApplyLODProcessing(strCount,lodProcArray,rangeCount,lodRange);
	}
}
//--------------Collision between entities--------------------------//
BOOL CMovementObjList::SegmentToEntity( D3DVECTOR & startPoint,
										D3DVECTOR & endPoint,
										CSkeletonObject * skeletalCmpare,
										MatrixARB * mathLib,
										D3DVECTOR * intersectRet,
										D3DMATRIX * boneMatrix,
										CFrameObj ** frameObject,
										D3DMATRIX & compareBaseMatrix,
										D3DVECTOR & latestNormal,
										float & dist
									)
{
	*frameObject = NULL;
	m_doAdjust = FALSE;
	//dynamically generate virtual directon based on segment-used to eliminate backface collisions by degree
    m_virtualNormal.x = endPoint.x - startPoint.x;
    m_virtualNormal.y = endPoint.y - startPoint.y;
    m_virtualNormal.z = endPoint.z - startPoint.z;
	m_virtualNormal = mathLib->Normalize(m_virtualNormal);
	dist = 1000000;
	
	if(!skeletalCmpare)
		return FALSE;

    if(skeletalCmpare->m_bonesList)
           for(POSITION posLoc = skeletalCmpare->m_bonesList->GetHeadPosition();posLoc != NULL;)
		   {
               CBoneObject * bonePtr = (CBoneObject *) skeletalCmpare->m_bonesList->GetNext(posLoc);
	           if(bonePtr->m_hiarchyMesh)
			   if(!bonePtr->m_hiarchyMesh->m_collisionFiltered)//see if collision should even be checked by this thing
			   {
                   CHiarcleVisObj * hObj = bonePtr->m_hiarchyMesh->GetByIndex(0);
                   if(hObj)
				   if(hObj->m_mesh)
				   { 
				      bonePtr->m_boneFrame->GetTransform(NULL,&m_wldBoneMatrix);//put ray into corrective space
				      mathLib->fastMatrixMultiply(m_wldBoneMatrix,m_wldBoneMatrix,compareBaseMatrix);
					  mathLib->D3DMath_MatrixInvert(m_invBoneMatrix,m_wldBoneMatrix);//requires full inverse cannot use fast calc matrix inv2
					  
					  m_invlegRayStart = mathLib->UpdatePolygonVertex(m_invBoneMatrix.m,startPoint);
                      m_invlegRayEnd   = mathLib->UpdatePolygonVertex(m_invBoneMatrix.m,endPoint);
					  m_invVirtualNormal  = mathLib->UpdatePolygonVertexRotationOnly(m_invBoneMatrix.m,m_virtualNormal);
					  
			          for(m_iLoop=0;m_iLoop < hObj->m_mesh->m_abVertexArray.m_indecieCount;m_iLoop+=3)
					  {//check polygons
                          hObj->m_mesh->m_abVertexArray.GetPolygon(m_iLoop,m_polygon);
                       
                          if(mathLib->SegPolyCheck(m_polygon,m_invlegRayStart,m_invlegRayEnd,&m_intersect))
						  {//collided
						     //generate normal
                             m_v1.x = m_polygon[1].x - m_polygon[0].x;
							 m_v1.y = m_polygon[1].y - m_polygon[0].y;
							 m_v1.z = m_polygon[1].z - m_polygon[0].z;
							 m_v2.x = m_polygon[2].x - m_polygon[0].x;
							 m_v2.y = m_polygon[2].y - m_polygon[0].y;
							 m_v2.z = m_polygon[2].z - m_polygon[0].z;
							 m_normalTemp = mathLib->Normalize(mathLib->CrossProduct(m_v1,m_v2));
							  
							 m_angle = mathLib->GetAngleBetweenTwoVectorsFast(m_invVirtualNormal,m_normalTemp);
		                     if(m_angle < 89)
		                        continue;
 
                             m_tempDist = mathLib->MagnitudeSquared(m_invlegRayEnd.x,m_invlegRayEnd.y,m_invlegRayEnd.z,m_intersect.x,m_intersect.y,m_intersect.z);
                             if(m_tempDist < dist)
							 {
                                CopyMemory(intersectRet,&m_intersect,sizeof(D3DVECTOR));
                                dist = m_tempDist;
								m_doAdjust = TRUE;
								*boneMatrix			= m_wldBoneMatrix;
								*frameObject		= bonePtr->m_boneFrame;
								latestNormal		= m_normalTemp;
							 }
						  }//end collided
					  }//end check polygons
				   }
			   }
		   }
		   //Deformable meshs
		   if(skeletalCmpare->m_deformableMeshArrayCount > 0)
		   {//valid def count
		    mathLib->D3DMath_MatrixInvert(m_invBoneMatrix,compareBaseMatrix);//only needs one basis of transform-this is good news
		    m_invlegRayStart = mathLib->UpdatePolygonVertex(m_invBoneMatrix.m,startPoint);
		    m_invlegRayEnd   = mathLib->UpdatePolygonVertex(m_invBoneMatrix.m,endPoint);
		    m_invVirtualNormal  = mathLib->UpdatePolygonVertexRotationOnly(m_invBoneMatrix.m,m_virtualNormal);
			   for(m_mCount=0;m_mCount<skeletalCmpare->m_deformableMeshArrayCount;m_mCount++)
			   {
                       if(skeletalCmpare->m_deformableMeshArrayV2[m_mCount].m_defMeshVisualUnit->m_collisionFiltered)
					      continue;

					   POSITION vLpos = skeletalCmpare->m_deformableMeshArrayV2[m_mCount].m_defMeshVisualUnit->m_lodChain->FindIndex(0);
					   if(!vLpos)
						  continue;

					   CDeformableVisObj * dVisPtr = (CDeformableVisObj *)skeletalCmpare->m_deformableMeshArrayV2[m_mCount].m_defMeshVisualUnit->m_lodChain->GetAt(vLpos);
					   if(!dVisPtr)
						   continue;

					   

					   //dVisPtr->m_mesh
                       for(m_iLoop=0;m_iLoop < dVisPtr->m_mesh->m_abVertexArray.m_indecieCount;m_iLoop+=3)
					   {//check polygons
                          dVisPtr->m_mesh->m_abVertexArray.GetPolygon(m_iLoop,m_polygon);
                       
                          if(mathLib->SegPolyCheck(m_polygon,m_invlegRayStart,m_invlegRayEnd,&m_intersect))
						  {//collided
						     //generate normal
                             m_v1.x = m_polygon[1].x - m_polygon[0].x;
							 m_v1.y = m_polygon[1].y - m_polygon[0].y;
							 m_v1.z = m_polygon[1].z - m_polygon[0].z;
							 m_v2.x = m_polygon[2].x - m_polygon[0].x;
							 m_v2.y = m_polygon[2].y - m_polygon[0].y;
							 m_v2.z = m_polygon[2].z - m_polygon[0].z;
							 m_normalTemp = mathLib->Normalize(mathLib->CrossProduct(m_v1,m_v2));
							  
							 m_angle = mathLib->GetAngleBetweenTwoVectorsFast(m_invVirtualNormal,m_normalTemp);
		                     if(m_angle < 89)
		                        continue;
 
                             m_tempDist = mathLib->MagnitudeSquared(m_invlegRayEnd.x,m_invlegRayEnd.y,m_invlegRayEnd.z,m_intersect.x,m_intersect.y,m_intersect.z);
                             if(m_tempDist < dist)
							 {
                                CopyMemory(intersectRet,&m_intersect,sizeof(D3DVECTOR));
                                dist = m_tempDist;
								m_doAdjust = TRUE;
								*boneMatrix			= compareBaseMatrix;
								//*frameObject		= bonePtr->m_boneFrame;
								latestNormal		= m_normalTemp;
							 }
						  }//end collided
					   }//end check polygons
			   }
		   }//end valid def count

		   if(m_doAdjust)
		      return TRUE;

		   return FALSE;
}
//--------------Collision between entities--------------------------//
BOOL CMovementObjList::SegmentToEntityDistPersistence(  D3DVECTOR & startPoint,
													    D3DVECTOR & endPoint,
													    CSkeletonObject * skeletalCmpare,
													    MatrixARB * mathLib,
													    D3DVECTOR * intersectRet,
														D3DMATRIX & compareBaseMatrix,
														D3DVECTOR & latestNormal,
														D3DMATRIX * boneMatrix,
														float & dist
													)
{
	m_doAdjust = FALSE;
	//dynamically generate virtual directon based on segment-used to eliminate backface collisions by degree
    m_virtualNormal.x = endPoint.x - startPoint.x;
    m_virtualNormal.y = endPoint.y - startPoint.y;
    m_virtualNormal.z = endPoint.z - startPoint.z;
	m_virtualNormal = mathLib->Normalize(m_virtualNormal);
	
	if(!skeletalCmpare)
		return FALSE;

    if(skeletalCmpare->m_bonesList)
           for(POSITION posLoc = skeletalCmpare->m_bonesList->GetHeadPosition();posLoc != NULL;)
		   {
               CBoneObject * bonePtr = (CBoneObject *) skeletalCmpare->m_bonesList->GetNext(posLoc);
	           if(bonePtr->m_hiarchyMesh)
			   if(!bonePtr->m_hiarchyMesh->m_collisionFiltered)//see if collision should even be checked by this thing
			   {
                   CHiarcleVisObj * hObj = bonePtr->m_hiarchyMesh->GetByIndex(0);
                   if(hObj)
				   if(hObj->m_mesh)
				   { 
				      bonePtr->m_boneFrame->GetTransform(NULL,&m_wldBoneMatrix);//put ray into corrective space
				      mathLib->fastMatrixMultiply(m_wldBoneMatrix,m_wldBoneMatrix,compareBaseMatrix);
					  mathLib->D3DMath_MatrixInvert(m_invBoneMatrix,m_wldBoneMatrix);//requires full inverse cannot use fast calc matrix inv2
					  
					  m_invlegRayStart = mathLib->UpdatePolygonVertex(m_invBoneMatrix.m,startPoint);
                      m_invlegRayEnd   = mathLib->UpdatePolygonVertex(m_invBoneMatrix.m,endPoint);
					  m_invVirtualNormal  = mathLib->UpdatePolygonVertexRotationOnly(m_invBoneMatrix.m,m_virtualNormal);
					  
			          for(m_iLoop=0;m_iLoop < hObj->m_mesh->m_abVertexArray.m_indecieCount;m_iLoop+=3)
					  {//check polygons
                          hObj->m_mesh->m_abVertexArray.GetPolygon(m_iLoop,m_polygon);
                       
                          if(mathLib->SegPolyCheck(m_polygon,m_invlegRayStart,m_invlegRayEnd,&m_intersect))
						  {//collided
						     //generate normal
                             m_v1.x = m_polygon[1].x - m_polygon[0].x;
							 m_v1.y = m_polygon[1].y - m_polygon[0].y;
							 m_v1.z = m_polygon[1].z - m_polygon[0].z;
							 m_v2.x = m_polygon[2].x - m_polygon[0].x;
							 m_v2.y = m_polygon[2].y - m_polygon[0].y;
							 m_v2.z = m_polygon[2].z - m_polygon[0].z;
							 m_normalTemp = mathLib->Normalize(mathLib->CrossProduct(m_v1,m_v2));
							  
							 m_angle = mathLib->GetAngleBetweenTwoVectorsFast(m_invVirtualNormal,m_normalTemp);
		                     if(m_angle < 89)
		                        continue;
 
                             m_tempDist = mathLib->MagnitudeSquared(m_invlegRayEnd.x,m_invlegRayEnd.y,m_invlegRayEnd.z,m_intersect.x,m_intersect.y,m_intersect.z);
                             if(m_tempDist < dist)
							 {
                                CopyMemory(intersectRet,&m_intersect,sizeof(D3DVECTOR));
                                dist = m_tempDist;
								m_doAdjust = TRUE;
								latestNormal		= m_normalTemp;
								*boneMatrix			= m_wldBoneMatrix;
							 }
						  }//end collided
					  }//end check polygons
				   }
			   }
		   }
		   //Deformable meshs
		   if(skeletalCmpare->m_deformableMeshArrayCount > 0)
		   {//valid def count
		    mathLib->D3DMath_MatrixInvert(m_invBoneMatrix,compareBaseMatrix);//only needs one basis of transform-this is good news
		    m_invlegRayStart = mathLib->UpdatePolygonVertex(m_invBoneMatrix.m,startPoint);
		    m_invlegRayEnd   = mathLib->UpdatePolygonVertex(m_invBoneMatrix.m,endPoint);
		    m_invVirtualNormal  = mathLib->UpdatePolygonVertexRotationOnly(m_invBoneMatrix.m,m_virtualNormal);
			   for(m_mCount=0;m_mCount<skeletalCmpare->m_deformableMeshArrayCount;m_mCount++)
			   {
                       if(skeletalCmpare->m_deformableMeshArrayV2[m_mCount].m_defMeshVisualUnit->m_collisionFiltered)
					      continue;

					   POSITION vLpos = skeletalCmpare->m_deformableMeshArrayV2[m_mCount].m_defMeshVisualUnit->m_lodChain->FindIndex(0);
					   if(!vLpos)
						  continue;

					   CDeformableVisObj * dVisPtr = (CDeformableVisObj *)skeletalCmpare->m_deformableMeshArrayV2[m_mCount].m_defMeshVisualUnit->m_lodChain->GetAt(vLpos);
					   if(!dVisPtr)
						   continue;

					   

					   //dVisPtr->m_mesh
                       for(m_iLoop=0;m_iLoop < dVisPtr->m_mesh->m_abVertexArray.m_indecieCount;m_iLoop+=3)
					   {//check polygons
                          dVisPtr->m_mesh->m_abVertexArray.GetPolygon(m_iLoop,m_polygon);
                       
                          if(mathLib->SegPolyCheck(m_polygon,m_invlegRayStart,m_invlegRayEnd,&m_intersect))
						  {//collided
						     //generate normal
                             m_v1.x = m_polygon[1].x - m_polygon[0].x;
							 m_v1.y = m_polygon[1].y - m_polygon[0].y;
							 m_v1.z = m_polygon[1].z - m_polygon[0].z;
							 m_v2.x = m_polygon[2].x - m_polygon[0].x;
							 m_v2.y = m_polygon[2].y - m_polygon[0].y;
							 m_v2.z = m_polygon[2].z - m_polygon[0].z;
							 m_normalTemp = mathLib->Normalize(mathLib->CrossProduct(m_v1,m_v2));
							  
							 m_angle = mathLib->GetAngleBetweenTwoVectorsFast(m_invVirtualNormal,m_normalTemp);
		                     if(m_angle < 89)
		                        continue;
 
                             m_tempDist = mathLib->MagnitudeSquared(m_invlegRayEnd.x,m_invlegRayEnd.y,m_invlegRayEnd.z,m_intersect.x,m_intersect.y,m_intersect.z);
                             if(m_tempDist < dist)
							 {
                                CopyMemory(intersectRet,&m_intersect,sizeof(D3DVECTOR));
                                dist = m_tempDist;
								m_doAdjust = TRUE;
								//*frameObject		= bonePtr->m_boneFrame;
								latestNormal		= m_normalTemp;
								*boneMatrix			= compareBaseMatrix;
							 }
						  }//end collided
					   }//end check polygons
			   }
		   }//end valid def count

		   if(m_doAdjust)
		      return TRUE;

		   return FALSE;
}
//--------------Collision between entities--------------------------------------------//
void CMovementObjList::DoExactPerPolyCollisionProcess(CMovementObj * entityFocus,
													   CSkeletonObject * skeletalCompare,
													   bool & correctiveOnly,
													   D3DVECTOR & intersection,
	                                                   D3DMATRIX & boneMatrix,
	                                                   CFrameObj ** framePtr,
													   MatrixARB * mathLib,
													   D3DMATRIX & matrixEntityCmp,
													   int & traceOption)
{
		if(correctiveOnly == false)
		if(SegmentToEntity(entityFocus->m_collisionRaySys->m_lastCenterMassPosition,
						   entityFocus->m_collisionRaySys->m_centerMassPosition,
						   skeletalCompare,//mPtr->List_skeletalObject,
						   mathLib,
						   &intersection,
						   &boneMatrix,
						   framePtr,
						   matrixEntityCmp,
						   m_latestNormal,
						   m_dist
						   )==TRUE)
	    {//collided
				intersection = mathLib->UpdatePolygonVertex(boneMatrix.m,intersection);
				entityFocus->List_baseFrame->GetPosition(NULL,&m_position);
				m_offset.x = intersection.x - entityFocus->m_collisionRaySys->m_centerMassPosition.x;
				m_offset.y = intersection.y - entityFocus->m_collisionRaySys->m_centerMassPosition.y;
				m_offset.z = intersection.z - entityFocus->m_collisionRaySys->m_centerMassPosition.z;
				m_position.x += m_offset.x;
				m_position.y += m_offset.y;
				m_position.z += m_offset.z;
				entityFocus->List_baseFrame->SetPosition(NULL,m_position);
				entityFocus->m_collisionRaySys->m_centerOfMassCollided = true;
	    }//end collided

		//TUNING SEGMENT
		entityFocus->List_baseFrame->GetPosition(NULL,&m_position);
        entityFocus->m_collisionRaySys->UpdateRays(entityFocus->List_skeletalObject);
		for(int cloop=(entityFocus->m_collisionRaySys->m_rayCount-1);cloop>-1;cloop--)
		{//poly ray collision
			if(entityFocus->m_collisionRaySys->RecalculateByIndex(  cloop,
															        entityFocus->List_baseFrame, 
															        entityFocus->List_skeletalObject,
															        mathLib,
																	&m_position,
																	&entityFocus->List_movementDB->List_lastPosition)==false)
																	continue;

			if(SegmentToEntity( entityFocus->m_collisionRaySys->m_rays[cloop].m_dynStartPoint,
								entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint,
								skeletalCompare,
								mathLib,
								&entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect,
								&boneMatrix,
								framePtr,
								matrixEntityCmp,
								m_latestNormal,
								m_dist
								)==TRUE)
			{//collided
				entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect = mathLib->UpdatePolygonVertex(boneMatrix.m,entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect);
				entityFocus->List_baseFrame->GetPosition(NULL,&m_position);
				entityFocus->m_collisionRaySys->m_rays[cloop].m_lastNormal = mathLib->UpdatePolygonVertexRotationOnly(boneMatrix.m,m_latestNormal);
				
				if(cloop == 0)
				{
                    m_offset.x = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.x - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.x;
				    m_offset.y = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.y - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.y;
				    m_offset.z = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.z - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.z;
				
				    m_position.x += m_offset.x;
				    m_position.y += m_offset.y;
				    m_position.z += m_offset.z;

					entityFocus->List_baseFrame->SetPosition(NULL,m_position);
					entityFocus->List_baseFrame->List_frameMatrix._42 -= .02f;
					entityFocus->List_movementDB->List_doGravity = FALSE;
					entityFocus->List_movementDB->List_currentlyJumping = FALSE;
					//lock up entity
					if(*framePtr)
					{
					entityFocus->List_movementDB->m_useLockPoint	= 1;//0=currently unlocked   1=currently locked 
					entityFocus->List_movementDB->m_lastLock		= boneMatrix;//
					entityFocus->List_movementDB->m_optFramePtr		= *framePtr;
					//entityFocus->List_movementDB->m_optLockTrace	= mPtr->List_traceNumber;
					//if(traceOption > -1)
				    entityFocus->List_movementDB->m_optLockTrace	= traceOption;
				    }
				}
				else
				{
					m_projectedPos = m_position;
                    m_offset = mathLib->GetAdjustVector(m_dist,entityFocus->m_collisionRaySys->m_rays[cloop].m_lastNormal);
                    m_projectedPos.x += m_offset.x;
					m_projectedPos.y += m_offset.y;
					m_projectedPos.z += m_offset.z;

                    m_additionalDist = fabs(entityFocus->List_skeletalObject->m_box.maxZ);
				    if(m_additionalDist < 2.0f)
					   m_additionalDist = 2.0f;

					
				    m_dirtemp.x = m_projectedPos.x - entityFocus->List_lastPosition.x; 
				    m_dirtemp.y = m_projectedPos.y - entityFocus->List_lastPosition.y; 
				    m_dirtemp.z = m_projectedPos.z - entityFocus->List_lastPosition.z;

				    m_dirtemp = mathLib->Normalize(m_dirtemp);
				    m_dirtemp.x = m_dirtemp.x * m_additionalDist;
				    m_dirtemp.y = m_dirtemp.y * m_additionalDist;
				    m_dirtemp.z = m_dirtemp.z * m_additionalDist;
				    m_projectedPos.x += m_dirtemp.x;
				    m_projectedPos.y += m_dirtemp.y;
				    m_projectedPos.z += m_dirtemp.z;
					CopyMemory(&m_dirtemp,&entityFocus->List_lastPosition,sizeof(D3DVECTOR));
 
                    m_dirtemp.y = entityFocus->m_collisionRaySys->m_rays[cloop].m_dynStartPoint.y;
			        m_projectedPos.y = entityFocus->m_collisionRaySys->m_rays[cloop].m_dynStartPoint.y;
					
					if(SegmentToEntity(  m_dirtemp,
										 m_projectedPos,
										 skeletalCompare,
										 mathLib,
										 &m_tempIntersect,
										 &boneMatrix,
										 framePtr,
										 matrixEntityCmp,
										 m_latestNormal,
										 m_dist
										 )==TRUE)
					   {//Handle corners 

							m_offset.x = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.x - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.x;
							m_offset.y = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.y - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.y;
							m_offset.z = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.z - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.z;

							m_position.x += m_offset.x;
							m_position.y += m_offset.y;
							m_position.z += m_offset.z; 

							entityFocus->List_movementDB->List_lastPosition.x = m_position.x;
							entityFocus->List_movementDB->List_lastPosition.y = m_position.y;
							entityFocus->List_movementDB->List_lastPosition.z = m_position.z;
					   }//end Handle corners 
					   else
					   {//handle slide
							m_position.x += m_offset.x;
							m_position.y += m_offset.y;
							m_position.z += m_offset.z; 
			           }//end handle slide
					entityFocus->List_baseFrame->SetPosition(NULL,m_position);
				}
			}//end collided
		}//end poly ray collision
}



//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CMovementObjList::ApplyLODProcessing(CString strLodScript)
{
	m_lodScriptHasBeenExecuted = 1;
	//If current LOD count, remove lod #s zero based //example: 10000,6000,3000,1500,500 0,1,2,3,4
    //4,4,2,0;3,0,2;3,0; example of string that gets parsed to generate thse structures in an array form
    //always make sure lod that you will keep are listed greatest to least, this will keep proper lod removal
    //second line/section defines by a \r\n(newline) of the script includes the comminted LOD ranges for entities based on its height, 
	//this is critical because you can have only one set of ranges per entity and if a lod doesnt exist it just wont render ie: weapon 
	//attachment that may only have 2 lods and the entity is on lod 3 the attachment wont render this is a advantage
	//however if not understood while designing lods or adding lods this can be a major problem
	//PROBLEM : lod ranges can be different on every entity, tailored by size genrally-if range isnt in script will keep whats currently there
	//script example: 
	//4,4,2,0;3,0,2;3,0
	//0-4,7,30,300
	//4-6,10,50,600
	//6-12,20,90,800
    
	lodRangeBasis lodRange[100];
	LODOptCondition lodOptArray[100];
	ZeroMemory(lodRange,sizeof(lodRangeBasis)*100);
    ZeroMemory(lodOptArray,sizeof(LODOptCondition)*100);

	int scriptLength = strLodScript.GetLength();
	char * scriptArray = strLodScript.GetBuffer();
	int trace = 0;
	int curArray = 0;
	int curRangeArray = 0;
	int mode = 0;
	CString temp;
	int stage = 0;
	int lodIndexArray = 0;
	
    while(trace < scriptLength)
	{
            if(mode == 0)
			{//processing lod elimination structures
               while(scriptArray[trace] != '\r' && scriptArray[trace] != '\n' && trace < scriptLength)
		       {//per line

			      if(scriptArray[trace] == 'L')
				     break;
              
                  while(scriptArray[trace] != ',' && scriptArray[trace] != ';')
				  {
					    if(scriptArray[trace] != ' ')
                           temp += scriptArray[trace];
						trace++;
				  }

				  if(stage == 0)//lod match count
                     lodOptArray[curArray].lodMatch = atoi(temp);
				  else
				  {
                     lodOptArray[curArray].removalArray[lodIndexArray] = atoi(temp);
                     lodOptArray[curArray].removalCount++;
					 lodIndexArray++;
				  }
				  temp = "";
				  stage++;
				  stage = 1;
				  if(scriptArray[trace] == ';')
				  {
                     curArray++;
                     stage = 0;
					 lodIndexArray = 0;
				  }
				  trace++;
				  
			   }//end per line
			   stage = 0;
			   lodIndexArray = 0;
			}//end processing lod elimination structures
			else
			{//processing actual lod ranges
               while(scriptArray[trace] != '\r' && scriptArray[trace] != '\n' && trace < scriptLength)
		       {//per line lod range //0-4,7,30,300

                  if(scriptArray[trace] == 'L')
				     break;

                  while(scriptArray[trace] != '-' && trace < scriptLength)
				  {//start Height range
				     if(scriptArray[trace] != ' ')
				        temp += scriptArray[trace];
					 trace++;
				  }//end start Height range
				  lodRange[curRangeArray].min = (float)atof(temp);
			      trace++;
				  temp = "";
				  while(scriptArray[trace] != ',' && trace < scriptLength)
				  {//start Height range
				     if(scriptArray[trace] != ' ')
				        temp += scriptArray[trace];
					 trace++;
				  }//end start Height range
				  lodRange[curRangeArray].max = (float)atof(temp);
				  temp = "";
			      trace++;
				  lodIndexArray = 0;
				  while(scriptArray[trace] != '\r' && scriptArray[trace] != '\n' && trace < scriptLength)
				  {
                      while(scriptArray[trace] != ',' && trace < scriptLength && scriptArray[trace] != '\r')
					  {//start Height range
						 if(scriptArray[trace] != ' ')
							temp += scriptArray[trace];
						 trace++;
					  }//end start Height range
                      lodRange[curRangeArray].lodSteps[lodIndexArray] = (float)atof(temp);
					  lodRange[curRangeArray].lodStepCount++;
					  temp = "";
                      lodIndexArray++;
                      trace++;
				  }
				  curRangeArray++;
			      trace++;
			   }//end per line lod range
			}//end processing actual lod ranges
		    mode = 1;
		trace++;
	}
	//process what we Read
	ApplyLODProcessing(curArray,lodOptArray,curRangeArray,lodRange);
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CMovementObj::ApplyForce(D3DVECTOR forceOrigin,float force)
{
     D3DVECTOR forceVect;
	 forceVect.x = List_baseFrame->List_frameMatrix._41 - forceOrigin.x;
	 forceVect.y = List_baseFrame->List_frameMatrix._42 - forceOrigin.y;
	 forceVect.z = List_baseFrame->List_frameMatrix._43 - forceOrigin.z;


     forceVect = List_baseFrame->GL_matrixFunctions.Normalize(forceVect);

	 List_externalVectorTemp.x += forceVect.x * force;
	 List_externalVectorTemp.y += forceVect.y * force;
	 List_externalVectorTemp.z += forceVect.z * force;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CMovementObj::ApplyForce(D3DVECTOR forceOrigin,float force,float range)
{
	 float dist = List_baseFrame->GL_matrixFunctions.MagnitudeSquared(forceOrigin.x,forceOrigin.y,forceOrigin.z,List_baseFrame->List_frameMatrix._41,List_baseFrame->List_frameMatrix._42,List_baseFrame->List_frameMatrix._43);
     if(dist > range)
	    return;

	 force = List_baseFrame->GL_matrixFunctions.InterpolateFloatsHiRes(force,0.0f,range,dist);

     ApplyForce(forceOrigin,force);
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
void CMovementObjList::ApplyForce(D3DVECTOR forceOrigin,float force,float range)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		if(!mPtr->List_networkEntity)
            mPtr->ApplyForce(forceOrigin,force,range);
	}
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
void CMovementObjList::ApplyForce(D3DVECTOR forceOrigin,float force)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)GetNext(posLoc);
		if(!mPtr->List_networkEntity)
            mPtr->ApplyForce(forceOrigin,force);
	}
}

void CMovementObjList::ApplyFemaleVariationsAndJigglePhysics()
{
	CMovementObj *blobMale = GetByID(8);
	CMovementObj *amphMale = GetByID(18);

	// 1. Append Female public body variations (_FemCurvy) to TRC_8 (Bloblic) and TRC_18 (Amphibious)
	// Note: TRC_51 (Human Female) is already the native female model in entitydatabase.edb and is left untouched.
	if (blobMale && blobMale->List_skeletalObject)
		blobMale->List_skeletalObject->GenerateFemaleBodyVariations(-1, FALSE, 1.15f);

	if (amphMale && amphMale->List_skeletalObject)
		amphMale->List_skeletalObject->GenerateFemaleBodyVariations(-1, FALSE, 1.35f);

	// 2. Upgrade backward movement speed across all entities to smooth modern 85% of forward max speed
	for (POSITION pos = GetHeadPosition(); pos != NULL;)
	{
		CMovementObj *mPtr = (CMovementObj*)GetNext(pos);
		if (mPtr && mPtr->List_movementDB && mPtr->List_movementDB->List_maxSpeed > 0.0f)
		{
			float modernMinSpeed = -mPtr->List_movementDB->List_maxSpeed * 0.85f;
			if (mPtr->List_movementDB->List_minSpeed > modernMinSpeed)
				mPtr->List_movementDB->List_minSpeed = modernMinSpeed;
		}
	}
}

void CMovementObjList::Serialize( CArchive& ar )
{
	CObList::Serialize( ar );
	if (!ar.IsStoring())
	{
		// ApplyFemaleVariationsAndJigglePhysics();  // off: it reshaped bodies with a box guess (warped chest)
	}
}



IMPLEMENT_SERIAL(CMovementObj,     CObject, 0)
IMPLEMENT_SERIAL(CMovementObjList, CObList, 0)


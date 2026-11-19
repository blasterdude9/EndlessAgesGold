#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "math.h"
#include "CFrameObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CFrameObj::CFrameObj(CFrameObj * parent)
{
List_parentFrame = parent;
   if(parent){//add itself to parent
      if(!List_parentFrame->List_childFrameList) 
          List_parentFrame->List_childFrameList = new CFrameObjList();

      List_frameMatrix = parent->List_frameMatrix;
      List_parentFrame->List_childFrameList->AddTail(this);
   }//end add itself to parent
   else{
      GL_matrixFunctions.InitMat(List_frameMatrix.m);
   }

   List_childFrameList = NULL;
   
   GL_matrixFunctions.InitMat(List_initMatrix.m);
}

void CFrameObj::SafeDelete()
{
if(List_parentFrame){//remove yourself from parents list
   POSITION posLast;
   if(List_parentFrame->List_childFrameList)
   for(POSITION posLoc = List_parentFrame->List_childFrameList->GetHeadPosition();(posLast = posLoc) != NULL;){//update children
       CFrameObj * ptrLoc = (CFrameObj *) List_parentFrame->List_childFrameList->GetNext(posLoc);
       if(ptrLoc == this){
          List_parentFrame->List_childFrameList->RemoveAt(posLast);
		  break;
	   }
   }
}//end remove yourself from parents list

    if(List_childFrameList){//if there are children attached move them to scene dependancy
	   for(POSITION posLoc = List_childFrameList->GetHeadPosition();posLoc != NULL;){
           CFrameObj * ptrLoc = (CFrameObj *) List_childFrameList->GetNext(posLoc);
           ptrLoc->List_parentFrame = NULL;//Invalidate directly
	   }
	   List_childFrameList->RemoveAll();
	   delete List_childFrameList;
	   List_childFrameList=0;
    }
}
//-----------------------------------------------------------------------------
//Desc: Class destructor
//-----------------------------------------------------------------------------
CFrameObj::~CFrameObj()
{	
	   
}
//---------------------------------------------------------//
//---------------------Set Parent--------------------------//
//---------------------------------------------------------//
void CFrameObj::SetParent(CFrameObj * parent)
{
if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();

if(List_parentFrame){//remove yourself from parents list
   POSITION posLast;
   for(POSITION posLoc = List_parentFrame->List_childFrameList->GetHeadPosition();(posLast = posLoc) != NULL;){//update children
       CFrameObj * ptrLoc = (CFrameObj *) List_parentFrame->List_childFrameList->GetNext(posLoc);
       if(ptrLoc == this){
          List_parentFrame->List_childFrameList->RemoveAt(posLast);
		  break;
	   }
   }
}//end remove yourself from parents list
List_parentFrame = parent;
 if(parent){//if valid parent add yourself
    if(!parent->List_childFrameList) 
        parent->List_childFrameList = new CFrameObjList();
    parent->List_childFrameList->AddTail(this);
 }//end if valid parent add yourself
}
//---------------------------------------------------------//
//--------------------UpdateChildren-----------------------//
//---------------------------------------------------------//
void CFrameObj::UpdateChildren(D3DMATRIX & diffMatrix)
{
  if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();

  for(POSITION posLoc = List_childFrameList->GetHeadPosition();posLoc != NULL;){//update children
      CFrameObj * ptrLoc = (CFrameObj *) List_childFrameList->GetNext(posLoc);
	  GL_matrixFunctions.fastMatrixMultiply(ptrLoc->List_frameMatrix,ptrLoc->List_frameMatrix,diffMatrix);
	  ptrLoc->UpdateChildren(diffMatrix);
  }
}
//---------------------------------------------------------//
//--------------------AddTransformBefore-------------------//
//---------------------------------------------------------//
void CFrameObj::AddTransformBefore2(CFrameObj * ref,D3DMATRIX & matrix)
{
  if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
  //transforms applied before makes object move local to itself
  D3DMATRIX startMatrix = List_frameMatrix;
  if(List_childFrameList->GetCount() > 0){//Has Children
	 if(!ref){//set to world space
         D3DMATRIX curMatrix = List_frameMatrix;
	     GL_matrixFunctions.fastMatrixMultiply(curMatrix,matrix,curMatrix); 
	     D3DMATRIX inverseMatrix,differenceMatrix;
	     GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	     GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	     GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	     UpdateChildren(differenceMatrix);
	 }//end set to world space
     else{//use ref
		 D3DMATRIX wheelmatrix;
		 ref->GetTransform(NULL,&wheelmatrix);
         GL_matrixFunctions.fastMatrixMultiply(wheelmatrix,wheelmatrix,List_frameMatrix);
         D3DMATRIX curMatrix;//= List_frameMatrix;
		 GL_matrixFunctions.fastMatrixMultiply(curMatrix,matrix,wheelmatrix); 
		 D3DMATRIX inverseMatrix,differenceMatrix;
		 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,wheelmatrix);
		 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
		 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	     UpdateChildren(differenceMatrix);
	 }//end use ref 
  }//end Has Children
  else{//no children
	 if(!ref){
        GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,matrix,List_frameMatrix);
	 }
	 else{//use ref
     //GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,matrix,ref->List_frameMatrix);
	    D3DMATRIX wheelmatrix;
		ref->GetTransform(NULL,&wheelmatrix);
        GL_matrixFunctions.fastMatrixMultiply(wheelmatrix,wheelmatrix,List_frameMatrix);
        D3DMATRIX curMatrix;//= List_frameMatrix;
		GL_matrixFunctions.fastMatrixMultiply(curMatrix,matrix,wheelmatrix); 
		D3DMATRIX inverseMatrix,differenceMatrix;
		GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,wheelmatrix);
		GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
		GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 }//end use ref
  }//end no children
}
//---------------------------------------------------------//
//--------------------AddTransformBefore-------------------//
//---------------------------------------------------------//
D3DMATRIX CFrameObj::AddTransformBefore3(CFrameObj * ref,D3DMATRIX & matrix)
{
  if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();

  D3DMATRIX inverseMatrix,differenceMatrix;
  //transforms applied before makes object move local to itself
  D3DMATRIX startMatrix = List_frameMatrix;
  if(List_childFrameList->GetCount() > 0){//Has Children
	 if(!ref){//set to world space
         D3DMATRIX curMatrix = List_frameMatrix;
	     GL_matrixFunctions.fastMatrixMultiply(curMatrix,matrix,curMatrix); 
	     GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	     GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	     GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	     UpdateChildren(differenceMatrix);
	 }//end set to world space
     else{//use ref
		 D3DMATRIX wheelmatrix;
		 ref->GetTransform(NULL,&wheelmatrix);
         GL_matrixFunctions.fastMatrixMultiply(wheelmatrix,wheelmatrix,List_frameMatrix);
         D3DMATRIX curMatrix;//= List_frameMatrix;
		 GL_matrixFunctions.fastMatrixMultiply(curMatrix,matrix,wheelmatrix); 
		 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,wheelmatrix);
		 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
		 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	     UpdateChildren(differenceMatrix);
	 }//end use ref 
  }//end Has Children
  else{//no children
	 if(!ref){
        GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,matrix,List_frameMatrix);
	 }
	 else{//use ref
     //GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,matrix,ref->List_frameMatrix);
	    D3DMATRIX wheelmatrix;
		ref->GetTransform(NULL,&wheelmatrix);
        GL_matrixFunctions.fastMatrixMultiply(wheelmatrix,wheelmatrix,List_frameMatrix);
        D3DMATRIX curMatrix;//= List_frameMatrix;
		GL_matrixFunctions.fastMatrixMultiply(curMatrix,matrix,wheelmatrix); 
		GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,wheelmatrix);
		GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
		GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
		
	 }//end use ref
  }//end no children
  return differenceMatrix;
}
//---------------------------------------------------------//
//--------------------AddTransformBefore-------------------//
//---------------------------------------------------------//
void CFrameObj::AddTransformBefore(D3DMATRIX & matrix)
{
  if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
  //transforms applied before makes object move local to itself
  D3DMATRIX startMatrix = List_frameMatrix;
  if(List_childFrameList->GetCount() > 0){//Has Children
	 D3DMATRIX curMatrix = List_frameMatrix;
	 GL_matrixFunctions.fastMatrixMultiply(curMatrix,matrix,curMatrix); 
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
  }//end Has Children
  else{//no children
     GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,matrix,List_frameMatrix);
  }//end no children
}
//---------------------------------------------------------//
//-----Add transfor after- ussually world transforms-------//
//---------------------------------------------------------//
void CFrameObj::AddTransformAfter(D3DMATRIX matrix)
{
	if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();

D3DMATRIX startMatrix = List_frameMatrix;

//transforms applied before makes object move local to itself
  if(List_childFrameList->GetCount() > 0){//has children
	 D3DMATRIX curMatrix = List_frameMatrix;
	 D3DVECTOR storeVect;
     storeVect.x = List_frameMatrix.m[3][0];
     storeVect.y = List_frameMatrix.m[3][1];
     storeVect.z = List_frameMatrix.m[3][2];
     curMatrix.m[3][0] = 0.0f;
     curMatrix.m[3][1] = 0.0f;
     curMatrix.m[3][2] = 0.0f;
     GL_matrixFunctions.fastMatrixMultiply(curMatrix,curMatrix,matrix);
     curMatrix.m[3][0] = storeVect.x + matrix.m[3][0];
     curMatrix.m[3][1] = storeVect.y + matrix.m[3][1];
     curMatrix.m[3][2] = storeVect.z + matrix.m[3][2]; 
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
  }//end has children
  else{//has no children
    D3DVECTOR storeVect;
    storeVect.x = List_frameMatrix.m[3][0];
    storeVect.y = List_frameMatrix.m[3][1];
    storeVect.z = List_frameMatrix.m[3][2];
    List_frameMatrix.m[3][0] = 0.0f;
    List_frameMatrix.m[3][1] = 0.0f;
    List_frameMatrix.m[3][2] = 0.0f;
    GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,matrix);
    List_frameMatrix.m[3][0] = storeVect.x + matrix.m[3][0];
    List_frameMatrix.m[3][1] = storeVect.y + matrix.m[3][1];
    List_frameMatrix.m[3][2] = storeVect.z + matrix.m[3][2];
  }//end has no children
}
//---------------------------------------------------------//
//------------Replaces transform in database---------------//
//---------------------------------------------------------//
void CFrameObj::AddTransformReplace(D3DMATRIX matrix)
{
if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
D3DMATRIX startMatrix = List_frameMatrix;
  if(List_childFrameList->GetCount() > 0){//Update children
	 D3DMATRIX curMatrix = List_frameMatrix;
	 curMatrix = matrix;
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
  }//end Update children
  else{
     List_frameMatrix = matrix;
  }
}
//---------------------------------------------------------//
//------------Replaces transform in database---------------//
//---------------------------------------------------------//
void CFrameObj::AddTransformReplaceNoChildAdjust(D3DMATRIX matrix)
{
     List_frameMatrix = matrix;
}
//---------------------------------------------------------//
//------------Replaces transform in database---------------//
//---------------------------------------------------------//
void CFrameObj::AddTransformReplaceRotationOnly(D3DMATRIX matrix)
{
if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
D3DMATRIX startMatrix = List_frameMatrix;
  if(List_childFrameList->GetCount() > 0){//Update children
	 D3DMATRIX curMatrix = List_frameMatrix;
	 D3DVECTOR cPosition;
	 cPosition.x = curMatrix._41;
	 cPosition.y = curMatrix._42;
	 cPosition.z = curMatrix._43;
	 curMatrix = matrix;
	 curMatrix._41 = cPosition.x;
	 curMatrix._42 = cPosition.y;
	 curMatrix._43 = cPosition.z;
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
  }//end Update children
  else{
     D3DVECTOR cPosition;
	 cPosition.x = List_frameMatrix._41;
	 cPosition.y = List_frameMatrix._42;
	 cPosition.z = List_frameMatrix._43;
	 List_frameMatrix = matrix;
	 List_frameMatrix._41 = cPosition.x;
	 List_frameMatrix._42 = cPosition.y;
	 List_frameMatrix._43 = cPosition.z;
  }
}
//---------------------------------------------------------//
//Gets Transform in respect to hiarchy and returns the value
//---------------------------------------------------------//
void CFrameObj::GetTransform(CFrameObj * ref,D3DMATRIX *matrix)
{
if(!ref){//if refrence frame is world coordinate sys (NULL)
*matrix = List_frameMatrix;
}//end if refrence frame is world coordinate sys (NULL)
else{//if based on other frame matrix- i dont know how to do this one
*matrix = List_frameMatrix;
}//if based on other frame matrix- i dont know how to do this one
}
//---------------------------------------------------------//
//Gets Transform in respect to hiarchy and returns the value
//---------------------------------------------------------//
void CFrameObj::GetInverseTransform(D3DMATRIX *matrix)
{
GL_matrixFunctions.D3DMath_MatrixInvert(*matrix,List_frameMatrix);
}
//---------------------------------------------------------//
//------Get Position in respect to hiarchy or reference----//
//---------------------------------------------------------//
void CFrameObj::GetPosition(CFrameObj *ref,D3DVECTOR * retVect)
{
      retVect->x = List_frameMatrix._41;
      retVect->y = List_frameMatrix._42;
      retVect->z = List_frameMatrix._43;
if(!ref)//no reference to it
return;
//has a ref frame to it- localize the point from world space to local of ref frame
*retVect = GL_matrixFunctions.GetLocalPositionFromMatrixView(ref->List_frameMatrix,*retVect);
}
//---------------------------------------------------------//
//------Set Position in respect to hiarchy or reference----//
//---------------------------------------------------------//
void CFrameObj::SetPosition(CFrameObj *ref,D3DVECTOR setVect)
{
if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
D3DMATRIX startMatrix = List_frameMatrix;
  if(List_childFrameList->GetCount() > 0){//Has Children
	 //set up
	 D3DMATRIX curMatrix = List_frameMatrix;
	 if(!ref){//set to world space
        curMatrix._41 = setVect.x;
        curMatrix._42 = setVect.y;
        curMatrix._43 = setVect.z;
	 }//end set to world space
     else{//use ref
        D3DMATRIX matrixApply;
        GL_matrixFunctions.Translate (setVect.x,setVect.y,setVect.z,matrixApply.m);
        GL_matrixFunctions.fastMatrixMultiply(matrixApply,matrixApply,ref->List_frameMatrix);
        curMatrix.m[3][0] = matrixApply.m[3][0];
        curMatrix.m[3][1] = matrixApply.m[3][1];
        curMatrix.m[3][2] = matrixApply.m[3][2];
	 }//end use ref
	  
	 //end set up
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
  }//end Has Children
  else{//no children
     if(!ref){//set to world space
         List_frameMatrix._41 = setVect.x;
         List_frameMatrix._42 = setVect.y;
         List_frameMatrix._43 = setVect.z;
	 }//end set to world space
     else{//use ref
         D3DMATRIX matrixApply;
         GL_matrixFunctions.Translate (setVect.x,setVect.y,setVect.z,matrixApply.m);
         GL_matrixFunctions.fastMatrixMultiply(matrixApply,matrixApply,ref->List_frameMatrix);
         List_frameMatrix.m[3][0] = matrixApply.m[3][0];
         List_frameMatrix.m[3][1] = matrixApply.m[3][1];
         List_frameMatrix.m[3][2] = matrixApply.m[3][2];
	 }//end use ref
  }//end else no children
}
//---------------------------------------------------------//
//----------------------SetPosition2-----------------------//
//---------------------------------------------------------//
void CFrameObj::SetPosition2(CFrameObj *ref,float setVectX,float setVectY,float setVectZ)
{
	D3DVECTOR pos;
	pos.x = setVectX;
	pos.y = setVectY;
	pos.z = setVectZ;
	SetPosition(ref,pos);
}
//---------------------------------------------------------//
//---------------------GetOrientation----------------------//
//---------------------------------------------------------//
void CFrameObj::GetOrientation(D3DVECTOR *dirVect,D3DVECTOR *upVect)
{

upVect->x = 0.0f;
upVect->y = 1.0f;
upVect->z = 0.0f;
dirVect->x = 0.0f;
dirVect->y = 0.0f;
dirVect->z = 1.0f;
*upVect = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(List_frameMatrix.m,*upVect);//get position in world space
*dirVect = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(List_frameMatrix.m,*dirVect);//get position in world space
}
//---------------------------------------------------------//
//---------------------SetOrientation----------------------//
//---------------------------------------------------------//
void CFrameObj::SetOrientation(D3DVECTOR dirVect,D3DVECTOR upVect)
{
	if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
D3DMATRIX startMatrix = List_frameMatrix;
  if(List_childFrameList->GetCount() > 0){//Has Children
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 D3DMATRIX curMatrix = List_frameMatrix;
	 //--set up
	 curMatrix._11 = upVect.y*dirVect.z - upVect.z*dirVect.y;
     curMatrix._12 = upVect.z*dirVect.x - upVect.x*dirVect.z;
     curMatrix._13 = upVect.x*dirVect.y - upVect.y*dirVect.x;

     curMatrix._21 = upVect.x;
     curMatrix._22 = upVect.y;
     curMatrix._23 = upVect.z;

     curMatrix._31 = dirVect.x;
     curMatrix._32 = dirVect.y;
     curMatrix._33 = dirVect.z;

     curMatrix._44 = 1.0;
	 //--end set up
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
  }//end Has Children
  else{//no children
     List_frameMatrix._11 = upVect.y*dirVect.z - upVect.z*dirVect.y;
     List_frameMatrix._12 = upVect.z*dirVect.x - upVect.x*dirVect.z;
     List_frameMatrix._13 = upVect.x*dirVect.y - upVect.y*dirVect.x;

     List_frameMatrix._21 = upVect.x;
     List_frameMatrix._22 = upVect.y;
     List_frameMatrix._23 = upVect.z;

     List_frameMatrix._31 = dirVect.x;
     List_frameMatrix._32 = dirVect.y;
     List_frameMatrix._33 = dirVect.z;

     List_frameMatrix._44 = 1.0;
  }//end no children
}

void CFrameObj::GetOrientationEff(float * roll,D3DVECTOR * dirVect)
{

}

void CFrameObj::SetOrientationEff(float roll,D3DVECTOR dirVect)
{

}
//---------------------------------------------------------//
//------------------------look at--------------------------//
//---------------------------------------------------------//
void CFrameObj::LookAt(CFrameObj * target)
{
D3DVECTOR VectTarget;
target->GetPosition(NULL,&VectTarget);

FaceTowardPoint(VectTarget);
}

void CFrameObj::FaceTowardPoint(D3DVECTOR targetVect)
{
	if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
    targetVect.y += .001f;

	D3DMATRIX startMatrix = List_frameMatrix;

	float opp   = 0;
	float adj   = 0;
	float yaw   = 0; 
	float pitch = 0;
		
    D3DVECTOR thisVect;
    GetPosition(NULL,&thisVect);//get position and respect hiarchy


	opp = thisVect.z - targetVect.z;
	adj = thisVect.x - targetVect.x;
	yaw = atan2(adj,opp);

	adj   = thisVect.y - targetVect.y;
	
	opp = sqrt((targetVect.x-thisVect.x)*(targetVect.x-thisVect.x) + 
		       (targetVect.z-thisVect.z)*(targetVect.z-thisVect.z));
	pitch = atan2(adj,opp);
	yaw   = -yaw;
	pitch = pitch;

	float cosyaw = cos(yaw);
	float sinyaw = sin(yaw);
	float cospitch = cos(pitch);
	float sinpitch = sin(pitch);

	if(List_childFrameList->GetCount() > 0){//Has children
	 D3DMATRIX curMatrix = List_frameMatrix;
	 //-set up
	 if(yaw && pitch) {
		curMatrix._11 = cosyaw;
		curMatrix._21 = sinpitch*sinyaw;
		curMatrix._31 = -cospitch*sinyaw;
		curMatrix._22 = cospitch;
		curMatrix._32 = sinpitch;
		curMatrix._13 = sinyaw;       
		curMatrix._23 = -sinpitch*cosyaw;
		curMatrix._33 = cospitch*cosyaw;

	 }
	 else if(yaw)
     {
		curMatrix._11 =  cosyaw;
		curMatrix._13 = -sinyaw;
		curMatrix._31 =  sinyaw;
		curMatrix._33 =  cosyaw;
	 }
	 else if(pitch)
	 {
		curMatrix._22 = cospitch;
		curMatrix._32 = sinpitch;
		curMatrix._23 = -sinpitch;
		curMatrix._33 = cospitch;
	 }
	 //-end set up
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
	}//end Has children
	else{//no children
     if(yaw && pitch) {
		List_frameMatrix._11 = cosyaw;
		List_frameMatrix._21 = sinpitch*sinyaw;
		List_frameMatrix._31 = -cospitch*sinyaw;
		List_frameMatrix._22 = cospitch;
		List_frameMatrix._32 = sinpitch;
		List_frameMatrix._13 = sinyaw;       
		List_frameMatrix._23 = -sinpitch*cosyaw;
		List_frameMatrix._33 = cospitch*cosyaw;

	 }
	 else if(yaw)
     {
		List_frameMatrix._11 =  cosyaw;
		List_frameMatrix._13 = -sinyaw;
		List_frameMatrix._31 =  sinyaw;
		List_frameMatrix._33 =  cosyaw;
	 }
	 else if(pitch)
	 {
		List_frameMatrix._22 = cospitch;
		List_frameMatrix._32 = sinpitch;
		List_frameMatrix._23 = -sinpitch;
		List_frameMatrix._33 = cospitch;
	 }
	}//end no children
}

//---------------------------------------------------------//
//-----------ussually for local Translation------------------//
//---------------------------------------------------------//
void CFrameObj::AddTranslationBefore(float Xloc,float Yloc,float Zloc)
{
	if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
    D3DMATRIX startMatrix = List_frameMatrix;
	D3DMATRIX matrixApply;
    GL_matrixFunctions.Translate (Xloc,Yloc,Zloc,matrixApply.m);
	
	if(List_childFrameList->GetCount() > 0){//Has Children
	 //-set up
	 D3DMATRIX curMatrix = List_frameMatrix;
     GL_matrixFunctions.fastMatrixMultiply(curMatrix,matrixApply,curMatrix);	
	 //end set up	
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
	}//end Has Children
	else{//has no children
     GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,matrixApply,List_frameMatrix);
	}//end has no children
}
//---------------------------------------------------------//
//-----Add Translation after- ussually world transforms-------//
//---------------------------------------------------------//
void CFrameObj::AddTranslationAfter(float Xloc,float Yloc,float Zloc)
{
	if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
D3DMATRIX startMatrix = List_frameMatrix;
  if(List_childFrameList->GetCount() > 0){//Has Children
	 D3DMATRIX curMatrix = List_frameMatrix;
	 //-set up
	 curMatrix.m[3][0] = Xloc + curMatrix.m[3][0];
     curMatrix.m[3][1] = Yloc + curMatrix.m[3][1];
     curMatrix.m[3][2] = Zloc + curMatrix.m[3][2];
	 //-end set up 
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
  }//end Has Children
  else{//has children
  List_frameMatrix.m[3][0] = Xloc + List_frameMatrix.m[3][0];
  List_frameMatrix.m[3][1] = Yloc + List_frameMatrix.m[3][1];
  List_frameMatrix.m[3][2] = Zloc + List_frameMatrix.m[3][2];
  }//end has children
}

//-------------------------------------------------------------------------------------------//
//------------Rotate on axis based on distance traveled and tire circumference---------------//
//-------------------------------------------------------------------------------------------//
void CFrameObj::RotateOnAxisXBasedOnDistCirc(float tireDiameter,float distTraveled)
{
     if(tireDiameter == 0.0f)
	    tireDiameter = 1.0f;
	 
     float circMeasurement = tireDiameter * GL_matrixFunctions.PI;
	 float turnAmount = (distTraveled / circMeasurement) * 360.0f;
     D3DMATRIX rotMat;
	 GL_matrixFunctions.RotateX(turnAmount,rotMat.m);
	 AddTransformBefore(rotMat);
}
//---------------------------------------------------------//
//------------Replaces Translation in database---------------//
//---------------------------------------------------------//
void CFrameObj::AddTranslationReplace(float Xloc,float Yloc,float Zloc)
{
  if(!List_childFrameList) 
    List_childFrameList = new CFrameObjList();
D3DMATRIX startMatrix = List_frameMatrix;

  if(List_childFrameList->GetCount() > 0){//Has children
	 D3DMATRIX curMatrix = List_frameMatrix;
	 //-set up
	 curMatrix.m[3][0] = Xloc;
     curMatrix.m[3][1] = Yloc;
     curMatrix.m[3][2] = Zloc;
	 //-end set up
	 D3DMATRIX inverseMatrix,differenceMatrix;
	 GL_matrixFunctions.D3DMath_MatrixInvert(inverseMatrix,startMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(differenceMatrix,inverseMatrix,curMatrix);
	 GL_matrixFunctions.fastMatrixMultiply(List_frameMatrix,List_frameMatrix,differenceMatrix);
	 UpdateChildren(differenceMatrix);
  }//end Has Children
  else{//no children
   List_frameMatrix.m[3][0] = Xloc;
   List_frameMatrix.m[3][1] = Yloc;
   List_frameMatrix.m[3][2] = Zloc;
  }//end no children
}


void CFrameObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << List_frameName;
	}
	else
	{
       ar >> List_frameName;
	}
} 

void CFrameObjList::SafeDelete()
{
       for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
           CFrameObj * ptrLoc = (CFrameObj *) GetNext(posLoc);
           ptrLoc->List_parentFrame = NULL;//Invalidate directly
	   }
	   RemoveAll();
}



IMPLEMENT_SERIAL(CFrameObj, CObject, 0)
IMPLEMENT_SERIAL(CFrameObjList, CObList, 0)


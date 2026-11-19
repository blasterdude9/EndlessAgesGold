#include "matrixarb.h"

#ifndef CFRAMEOBJCLASS_H
#define CFRAMEOBJCLASS_H

typedef struct _D3DRMBOX2 {
    D3DVECTOR min, max;
}D3DRMBOX2;
typedef D3DRMBOX2 *LPD3DRMBOX2;



class CFrameObjList : public CObList
{
public:
   CFrameObjList(){};
   void SafeDelete();
    

   DECLARE_SERIAL( CFrameObjList ) 
};


class CFrameObj : public CObject
{
	
	
     
    DECLARE_SERIAL( CFrameObj )
    
    
	CFrameObj(){}
	~CFrameObj();
public:
    
    CFrameObj(CFrameObj * parent);
 

    MatrixARB                  GL_matrixFunctions;
	CString                    List_frameName;
    D3DMATRIX                  List_frameMatrix;
	D3DMATRIX                  List_initMatrix;
	CFrameObj *                List_parentFrame;
    CFrameObjList *            List_childFrameList;
	//tramsforms
	void GetOrientation(D3DVECTOR *dirVect,D3DVECTOR *upVect);
	//BOOL MatrixCompare(D3DMATRIX matrix,D3DMATRIX matrix2);
	//BOOL MatrixCompare2(D3DMATRIX matrix,D3DMATRIX matrix2);
	void AddTransformBefore(D3DMATRIX & matrix);
	void AddTransformBefore2(CFrameObj * ref,D3DMATRIX & matrix);
    void AddTransformAfter(D3DMATRIX matrix);
    void AddTransformReplace(D3DMATRIX matrix);	
	void GetPosition(CFrameObj *ref,D3DVECTOR * retVect);
	void FaceTowardPoint(D3DVECTOR targetVect);
	void SetOrientation(D3DVECTOR dirVect,D3DVECTOR upVect);
	void GetOrientationEff(float * roll,D3DVECTOR * dirVect);
    void SetOrientationEff(float roll,D3DVECTOR dirVect);
    void SetPosition(CFrameObj *ref,D3DVECTOR setVect);
	void SetPosition2(CFrameObj *ref,float setVectX,float setVectY,float setVectZ);
	void GetTransform(CFrameObj * ref,D3DMATRIX *matrix);
	void LookAt(CFrameObj * target);
    void AddTranslationBefore(float Xloc,float Yloc,float Zloc);
    void AddTranslationAfter(float Xloc,float Yloc,float Zloc);
    void AddTranslationReplace(float Xloc,float Yloc,float Zloc);
	void SetParent(CFrameObj * parent);
	void UpdateChildren(D3DMATRIX & diffMatrix);
	void AddTransformReplaceRotationOnly(D3DMATRIX matrix);
	void AddTransformReplaceNoChildAdjust(D3DMATRIX matrix);
	void GetInverseTransform(D3DMATRIX *matrix);
	void SafeDelete();
	void RotateOnAxisXBasedOnDistCirc(float tireDiameter,float distTraveled);
	D3DMATRIX AddTransformBefore3(CFrameObj * ref,D3DMATRIX & matrix);


	void Serialize( CArchive& ar );
	
};



#endif
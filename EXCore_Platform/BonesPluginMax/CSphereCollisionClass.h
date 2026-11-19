#include "matrixarb.h"

#ifndef CSPHERECOLLISIONCLASS_H
#define CSPHERECOLLISIONCLASS_H



class  CSphereCollisionObj : public CObject
{
	
     
    DECLARE_SERIAL( CSphereCollisionObj )
    
    
public:
	CSphereCollisionObj();

    int     m_boneIndex;
	float   m_sphereRadius;
    float   m_damageMultiplier;
	CString m_comment;
    void Clone(CSphereCollisionObj ** copy);
	void Serialize( CArchive& ar );
	
};

class  CSphereCollisionObjList : public CObList
{
public:
   CSphereCollisionObjList(){};
   void SafeDelete();
   void Clone(CSphereCollisionObjList ** copy);
    
     
    DECLARE_SERIAL( CSphereCollisionObjList )
    
    
};

#endif
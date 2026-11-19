
#ifndef CSKELETONLODCLASS_H
#define CSKELETONLODCLASS_H

class   CSkeletalLODObject : public CObject
{
	
     
    DECLARE_SERIAL( CSkeletalLODObject )
    
    
public:
	
	CSkeletalLODObject();
	float m_distance;//greater than activate it
	char  m_flagTemp;
	
	void        Clone(CSkeletalLODObject ** copy);
	void        SafeDelete();
	void        Serialize( CArchive& ar );

};

class   CSkeletalLODObjectList : public CObList
{
public:
   CSkeletalLODObjectList(){};
   void SafeDelete();
   void ResetFlags();
   void Clone(CSkeletalLODObjectList ** copy);
   int GetLodIndex(float & currentDistance);
   int GetLod(D3DMATRIX & cam, D3DMATRIX & oPos);
   BOOL BoundingSphereTest(float & radius1,float & radius2,float & x1,float & y1,float & z1,float & x2,float & y2,float & z2);
    
     
   DECLARE_SERIAL( CSkeletalLODObjectList )
    
    
};

#endif
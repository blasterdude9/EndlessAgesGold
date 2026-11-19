#ifndef CFIRERANGE_H
#define CFIRERANGE_H

class  CFireRangeObject : public CObject
{
    DECLARE_SERIAL( CFireRangeObject )
    
public:
	
	CFireRangeObject();
    
	int          m_animationIndex;
	int          m_animationFrameRangeStart;
	int          m_animationFrameRangeEnd;
	int          m_boneIndex;
	int          m_missileIndex;
	BOOL         m_inRangeCurrently;
	D3DVECTOR    m_hardPointAdjust;
	float        m_accuracyRating;
 
    void Clone(CFireRangeObject ** clone);
    void Serialize( CArchive& ar );
};

class  CFireRangeObjectList : public CObList
{
public:
   void SafeDelete();
   void Clone(CFireRangeObjectList ** clone);
   CFireRangeObjectList(){};
    
   CFireRangeObject * GetByMissileIndex(int & missileIndex);
   CFireRangeObject *GetByFireQueIndex(int &queIndex);
     
   DECLARE_SERIAL( CFireRangeObjectList )
    
   BOOL IsInsideARange(int & animation,int & currentFrame)
   {
        for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
            CFireRangeObject * fPtr = (CFireRangeObject *)GetNext(posLoc);
            if(fPtr->m_animationIndex == animation)
			 if(currentFrame >= fPtr->m_animationFrameRangeStart && currentFrame <= fPtr->m_animationFrameRangeEnd)
			    return TRUE;
		}
		return FALSE;
   }
    
};

#endif
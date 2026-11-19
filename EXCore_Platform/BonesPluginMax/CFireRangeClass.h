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
 
    void Clone(CFireRangeObject ** clone);

    void Serialize( CArchive& ar );
};

class  CFireRangeObjectList : public CObList
{
public:
   void SafeDelete();
   void Clone(CFireRangeObjectList ** clone);
   CFireRangeObjectList(){};
    
     
    DECLARE_SERIAL( CFireRangeObjectList )
    
    
};

#endif
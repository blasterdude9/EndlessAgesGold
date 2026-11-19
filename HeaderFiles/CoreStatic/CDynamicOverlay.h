#ifndef CDYNAMICOVERLAY_H
#define CDYNAMICOVERLAY_H

class  CDynamicOverlayObj : public CObject
{

public:
    CDynamicOverlayObj();
    
	D3DVECTOR m_screenPos;
	float     m_multiplier;
 
	
};

class  CDynamicOverlayList : public CObList
{
public:
	CDynamicOverlayList(){};
   void SafeDelete();
};

#endif
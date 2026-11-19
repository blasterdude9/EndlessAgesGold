#include "CMaterialClass.h"
#include "CAdvancedVertexClass.h"
#ifndef CCHARONACLASS_H
#define CCHARONACLASS_H

class  CCharonaObj : public CObject
{
    
     
	DECLARE_SERIAL( CCharonaObj );
	
    

public:
    CCharonaObj();
	
    
    float             m_size;
	float             m_scaleFactor;
	DWORD             m_centerPixelSnapShot;
	BOOL              m_validThisRound;
	long              m_fadeTime;
	CMaterialObject * m_material;
	CString           m_textureName;
	int               m_textureIndex;
	BOOL              m_currentlyVisible;
	long              m_changeStamp;
	float             m_startDistance;
	float             m_endDistance;
	float             m_visibleDistance;
	float             m_currentDistance;
	CAdvancedVertexObj * m_visual;
	
    WORD        vertexCount;
    ABVERTEX    vertexArray[4];
	WORD        indecieCount;
    WORD        indecieArray[6];

	
	void SafeDelete();
	void Clone(CCharonaObj ** clone);
	BOOL InitializeVis();

	void Serialize( CArchive& ar );	
};

#endif
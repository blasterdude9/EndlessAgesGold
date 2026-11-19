#include "CMaterialClass.h"
#include "CAdvancedVertexClass.h"
#ifndef CCHARONACLASS_H
#define CCHARONACLASS_H

class CCharonaObj : public CObject
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

	D3DMATRIX         m_currentMatrix;
	D3DVECTOR         m_center;
	
    WORD        vertexCount;
    ABVERTEX    vertexArray[4];
	WORD        indecieCount;
    WORD        indecieArray[6];

	
	void SafeDelete();
	void Clone(CCharonaObj ** clone);
	BOOL InitializeVis();

	void Serialize( CArchive& ar );	
};

class CCharonaDB : public CObList
{
     public:
	 CCharonaDB(){};
	 ~CCharonaDB(){};
     
	 void AddCharona(CCharonaObj * charona,D3DMATRIX & matrix,D3DVECTOR & center)
	 {
		  CopyMemory(&charona->m_currentMatrix,&matrix,sizeof(D3DMATRIX));
		  CopyMemory(&charona->m_center,&center,sizeof(D3DVECTOR));
          AddTail(charona);
	 }
     
     void ClearAll()
	 {
        /*for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
            CCharonaObj * cPtr = (CCharonaObj *)GetNext(posLoc);
			cPtr->SafeDelete();
			delete cPtr;
			cPtr = 0;
		}*/
		RemoveAll();
	 }

};

#endif
#include "matrixarb.h"
#include <dinput.h>
#include "CTextRenderClass.h"
#include "CSkeletalClass.h"
#ifndef CMENUSLIDERCLASS_H
#define CMENUSLIDERCLASS_H

class   CSliderContentObj : public CObject
{
	
     
	DECLARE_SERIAL( CSliderContentObj );
	
    
public:
    CSliderContentObj();

	CString				m_text;
	int					m_visualRef;
	int					m_miscInt;
	int					m_miscInt2;
	int					m_miscInt3;
	int					m_miscInt4;
	CTextureRenderObj * m_textVisual;
	
	
    
  
	void SafeDelete();
	void Clone(CSliderContentObj ** copy);
	
	void Serialize( CArchive& ar );
	
};

class   CSliderContentObjList : public CObList
{
public:
   CSliderContentObjList(){};
   void SafeDelete();
   void Clone(CSliderContentObjList ** copy);
   BOOL ReorderInventory(int objectToMoveIndex,int targetIndex,int mode);
    
     
	DECLARE_SERIAL( CSliderContentObjList );
	
    

	CSliderContentObj * GetByMiscInt(int miscl);
	CSliderContentObj * GetByMiscIntQuanity(int miscl);
	CSliderContentObj * GetByIndex(int index);
	
};



class   CSliderBarObj : public CObject
{
	
     
	DECLARE_SERIAL( CSliderBarObj );
	
    
public:
    CSliderBarObj();
    MatrixARB                  GL_matrixFunctions;
	float                      m_boxClickSize;
	D3DVECTOR                  m_center;
	D3DVECTOR                  m_locStart;
	D3DVECTOR                  m_locEnd;
	D3DVECTOR                  m_current;
	CSkeletonObject           *m_visStart;
    CSkeletonObject           *m_visMov;
	CSkeletonObject           *m_visEnd;
	int                        m_itemCount;
	int                        m_currentItem;
	int                        m_itemsThatAreVisible;
	float                      m_visOffsetX;
	float                      m_visOffsetY;
	float                      m_visSpacing;
	BOOL                       m_acceptingMouseMovements;
	CSliderContentObjList     *m_contentList;
	float                      m_clickWidth;
	int                        m_currentSelectedContentItem;
	long                       m_selectStamp;
	float                      m_fontSizeX;
	float                      m_fontSizeY;
    
	int                        m_controlID;
	int                        m_mode;
	BOOL                       m_toggleTemp;
	BOOL                       m_dragDropEnabled;
	float					   m_red;
	float					   m_green;
	float					   m_blue;

	BOOL                       m_selectionDisabled;



    CSliderContentObj * GetByIndex(int index);
    void IndexUp(int glAdjust);
    void IndexDown(int glAdjust);
    BOOL DeleteString(int index);
	int GetListDisplayBeginning();
	void FlushAll();
	BOOL InsertAfter(CString text,int indexInList,int visIndex,int miscInt,int miscInt2,int miscInt3,int miscInt4);
	BOOL InsertBefore(CString text,int indexInList,int visIndex,int miscInt,int miscInt2,int miscInt3,int miscInt4);
	void AddString(CString text,int visIndex,int miscInt,int miscInt2,int miscInt3,int miscInt4);
	void Clone(CSliderBarObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL Update(DIMOUSESTATE mouseState,D3DVECTOR cursorPosition);
	int GetListIndexByPosition(D3DVECTOR cursorPosition);
	void Serialize( CArchive& ar );
	BOOL BuildSystem(CString startVisualFile,
								CString midVisFile,
								CString endVisFile,
								D3DVECTOR centerOfSystem,
								D3DVECTOR endPos,
								D3DVECTOR startPos,
								float clickZoneSize,
								int visibleItems,
								float visOffset,
								float visSpacing,
								float clickWidth,
								float fontSizeX,
								float fontSizeY,
								LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL GetInfoByIndex(int index,int * miscInt,int * duplicatePassed, CSliderContentObj ** ptrReference);
	CString ConvToString(int number);
	void SafeDelete();
	
};

class   CSliderBarObjList : public CObList
{
public:
   CSliderBarObjList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CSliderBarObjList );
	
    

	
};

#endif
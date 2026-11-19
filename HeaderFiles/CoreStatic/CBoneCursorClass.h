#include "matrixarb.h"
#include <dinput.h>
#include "CSkeletalClass.h"

#ifndef CBONECURSOROBJCLASS_H
#define CBONECURSOROBJCLASS_H


class  CBoneCursorObj : public CObject
{
	
     
	DECLARE_SERIAL( CBoneCursorObj );
	
    
public:
    CBoneCursorObj();



    MatrixARB                  GL_matrixFunctions;
	D3DVECTOR                  m_position;
	float                      m_limitX;
	float                      m_limitY;
	BOOL                       m_fadeInProcess;
	BOOL                       m_fadingIn;
	BOOL                       m_fadingOut;
	long                       m_fadeTimeStamp;
	long                       m_fadeDuration;
	float                      m_scaleFactor;

	//
	long                       m_cursorType;//0=normal 1=cursor can select entities/world object intersections 2=cursor can select entities/world object intersections
	int                        m_expEffectOnClick;
 
	int                        m_useTypeID;//0=over NPC 1=OVER pc etc ect definable


	BOOL                       m_mouseButton1pressed;

	CSkeletonObject           *m_skeletonObject;
	//drag drop vars
	BOOL m_dragging;
	int  m_dragGlid;
	int  m_dragVisualRef;
	int  m_dragVisualID;
	int  m_dragDupPassed;

    void SafeDelete();
	void Clone(CBoneCursorObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
    BOOL BuildMouse(CString arbFile,
		            float limitX,
					float limitY,
					D3DVECTOR startPosition,
					LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL AnimationManagement(long currentTime,DIMOUSESTATE mouseState, float adjust);

	void Serialize( CArchive& ar );
	
};

class  CBoneCursorObjList : public CObList
{
public:
   CBoneCursorObjList(){};
   ~CBoneCursorObjList()
   {
      for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	  {
          CBoneCursorObj * bObj = (CBoneCursorObj *) GetNext(posLoc);
          bObj->SafeDelete();
		  delete bObj;
		  bObj = 0;
	  }
	  RemoveAll();
   };
   //void SafeDelete();
   void Clone(CBoneCursorObjList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
   CBoneCursorObj * GetByUseTypeID(int id);
     
	DECLARE_SERIAL( CBoneCursorObjList );
	
    
};

#endif
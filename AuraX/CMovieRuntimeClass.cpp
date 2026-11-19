#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>

#include <mmsystem.h>
#include "dsound.h"
#include "CMovieRuntimeClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------------//
//-------------------------Movement object/list------------------//
//---------------------------------------------------------------//
//contains the mesh / file name ect  list of possible animations-//

CMovieRuntimeObj::CMovieRuntimeObj(BOOL L_playing,
								   BOOL L_looping,
								   CString L_fileName,
					 RECT L_rectMov,
					 RECT L_changingRect
				     )
{
List_changingRect = L_changingRect;
List_looping = L_looping;
List_playing = L_playing;
List_fileName = L_fileName;
List_rectMov = L_rectMov;
}

void CMovieRuntimeObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar  << List_fileName
		   << List_looping
		   << List_rectMov.top
		   << List_rectMov.left
		   << List_rectMov.right
		   << List_rectMov.bottom; 
		  
		
	}
	else
	{
       ar  >> List_fileName
		   >> List_looping
		   >> List_rectMov.top
		   >> List_rectMov.left
		   >> List_rectMov.right
		   >> List_rectMov.bottom; 
		      
			  List_playing = FALSE;
			  List_changingRect.top = 10;
			  List_changingRect.bottom = 20;
			  List_changingRect.right = 20;
			  List_changingRect.left = 10;
	}
}

IMPLEMENT_SERIAL(CMovieRuntimeObj, CObject, 0)
IMPLEMENT_SERIAL(CMovieRuntimeObjList, CObList, 0)


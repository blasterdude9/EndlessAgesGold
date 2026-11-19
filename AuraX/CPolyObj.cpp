#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>
#include "CPolyObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPolyObj::CPolyObj()
{
polygon[0].x = 0.0f;
polygon[0].y = 0.0f; 
polygon[0].z = 0.0f;
polygon[1].x = 0.0f; 
polygon[1].y = 0.0f; 
polygon[1].z = 0.0f; 
polygon[2].x = 0.0f; 
polygon[2].y = 0.0f; 
polygon[2].z = 0.0f; 
normal.x = 0.0f;
normal.y = 0.0f;
normal.z = 0.0f;
center.x = 0.0f;
center.y = 0.0f;
center.z = 0.0f;
radius = 0.0f;

}

/*void CPolyObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
		ar << polygon[0].x 
		   << polygon[0].y 
		   << polygon[0].z
		   << polygon[1].x 
		   << polygon[1].y 
		   << polygon[1].z 
		   << polygon[2].x 
		   << polygon[2].y 
		   << polygon[2].z 
		   << normal.x
		   << normal.y
		   << normal.z
		   << center.x
		   << center.y
		   << center.z
		   << radius;
	}
	else
	{
        ar >> polygon[0].x 
		   >> polygon[0].y 
		   >> polygon[0].z
		   >> polygon[1].x 
		   >> polygon[1].y 
		   >> polygon[1].z 
		   >> polygon[2].x 
		   >> polygon[2].y 
		   >> polygon[2].z 
		   >> normal.x
		   >> normal.y
		   >> normal.z
		   >> center.x
		   >> center.y
		   >> center.z
		   >> radius;
	}
}

IMPLEMENT_SERIAL(CPolyObj, CObject, 0)*/

#include "D3D9.h"
#ifndef CPOLYOBJ_H
#define CPOLYOBJ_H

class   CPolyObj : public CObject
{	
	//DECLARE_SERIAL( CPolyObj );
	public:
	CPolyObj();
	D3DVECTOR polygon[3];
    D3DVECTOR normal;
	float     radius;
    D3DVECTOR center;
  
	//void Serialize( CArchive& ar );

};

#endif

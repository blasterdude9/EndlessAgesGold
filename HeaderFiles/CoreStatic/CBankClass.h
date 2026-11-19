#include "matrixarb.h"
#ifndef CBANKOBJCLASS_H
#define CBANKOBJCLASS_H

class  CBankObj : public CObject
{
	
     
	DECLARE_SERIAL( CBankObj );
	
    
public:
    CBankObj();
    
	CString                    m_bankName;
    int                        m_worldID;
	D3DVECTOR                  m_boundingBoxMax;
	D3DVECTOR                  m_boundingBoxMin;
	int                        m_menuBind;
	BOOL                       m_willDealWithCriminals;
	
	void Serialize( CArchive& ar );	
};

class  CBankObjList : public CObList
{
public:
   CBankObjList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CBankObjList );
	
    

    CBankObj * GetBankByPosition(D3DVECTOR position,int worldID);
	 BOOL BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max);
};

#endif
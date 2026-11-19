#ifndef GETDXVERCLASS_H
#define GETDXVERCLASS_H


//object attributes for animation managment
class GETDXverObj
{
  public:
	GETDXverObj(){};
	HRESULT GetDXVersionPub( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );
	
};


#endif
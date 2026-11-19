#include <d3dx9.h>
#ifndef CSHADEROBJCLASS_H
#define CSHADEROBJCLASS_H


class CShaderObj : public CObject
{
	
     
	DECLARE_SERIAL( CShaderObj );
	
    
public:
    CShaderObj();
	CString                    m_shaderName;
    CString                    m_shaderCode;
	LPDIRECT3DVERTEXSHADER9    m_shader;
	LPDIRECT3DPIXELSHADER9     m_pShader;
	LPD3DXEFFECT               m_pEffect;
	D3DXHANDLE                 m_shaderValidHandle;
	short                      m_effectPeramCount;
	int                        m_shaderType;
	int                        m_compiler;//0=assemly 1=fx compiler
	byte                       m_shaderUsedOnce;


 
	BOOL Compile(LPDIRECT3DDEVICE9 pd3dDevice,CString * error);
	void SafeDelete();
	void Serialize( CArchive& ar );	
};

class CShaderObjList : public CObList
{
public:
	CShaderObjList(){};
   
    
     
	DECLARE_SERIAL( CShaderObjList );
	
    

	void SafeDelete();
	void Compile(LPDIRECT3DDEVICE9 pd3dDevice);
    void AddShader(CString shaderName, CString code, int shaderType);
	CString ConvToString(int number);
    BOOL DeleteByIndex(int index);
	CShaderObj* GetByIndex(int index);
	CShaderObj* GetByName(CString shaderName,int & index);
	int LoadShaderByStringData(CString dataStr,LPDIRECT3DDEVICE9 pd3dDevice, CString & fxNameRet);
	BOOL SetShaderByIndex(LPDIRECT3DDEVICE9 pd3dDevice,
		                  int index,
						  int mode,
						  LPD3DXEFFECT * effectRet,
						  int & correctPeramCount,
						  CString * retCode,
						  CShaderObj ** retPtr);
};

#endif
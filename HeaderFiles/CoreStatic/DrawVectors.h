#include <d3d9.h>
#include "CStateManagementClass.h"
#include "matrixarb.h"
#pragma once

class CDrawVectors
{
public:
	CDrawVectors(void){m_vCount=0;};
public:
	~CDrawVectors(void);

    D3DVECTOR m_lineBuffer[1000];
    int m_vCount;
	MatrixARB m_matLib;

	bool AddLineWLD(D3DVECTOR & start, D3DVECTOR & end)
	{
		if(m_vCount > 998)
		   return false;
         
		   m_lineBuffer[m_vCount++] = start;
           m_lineBuffer[m_vCount++] = end;

		   return true;
	}

	void Clear()
	{
         m_vCount = 0;
	}

	void Render(CStateManagementObj  * renderStateObject,D3DVECTOR & locationVect,LPDIRECT3DDEVICE9 device)
	{
         if(m_vCount < 2)
		    return;

             D3DMATERIAL9 materialX;
			 ZeroMemory(&materialX,sizeof(D3DMATERIAL9));
			 materialX.Diffuse.a = 1.0f;
			 materialX.Emissive.r = 1.0f;
			 materialX.Emissive.g = 1.0f;
			 materialX.Emissive.b = 1.0f;
			 materialX.Emissive.a = 1.0f;
			 renderStateObject->SetBlendType(device,-1);
			 renderStateObject->SetCullState(0,device);
			 renderStateObject->SetAlphaPixelTest(FALSE,device);
			 renderStateObject->SetWireFillState(FALSE,device);
			 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_intZero,device,NULL);
			 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int1,device,NULL);
			 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int2,device,NULL);
			 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int3,device,NULL);
			 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int4,device,NULL);
			 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int5,device,NULL);
			 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int6,device,NULL);
			 renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int7,device,NULL);
			 
			 D3DMATRIX matRU;//= m_gStatus->m_matrixInit2;
			 m_matLib.InitMat(matRU.m);
			 matRU._41 = matRU._41 - locationVect.x;
	         matRU._42 = matRU._42 - locationVect.y;
	         matRU._43 = matRU._43 - locationVect.z;
			 renderStateObject->SetMaterial(device,&materialX);
			 renderStateObject->SetWorldMatrix((D3DXMATRIX*)&matRU,device);
			 renderStateObject->SetDiffuseSource(0,device);
			 renderStateObject->SetVertexType(71,device);
		     HRESULT errTemp = device->DrawPrimitiveUP( D3DPT_LINELIST, m_vCount/2,m_lineBuffer, sizeof(D3DVECTOR) );
	}
};

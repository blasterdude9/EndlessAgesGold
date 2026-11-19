#include "stdafx.h"
#include "CTextTempEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CTextEffectDB::SpawnText(CString text,
							  DWORD duration,
							  D3DVECTOR spawnPos,
							  float riseMax,
							  float red,
							  float green,
							  float blue,
							  float fontSzX,
							  float fontSzY,
							  CTextureRenderObjList * textRenderList,
							  CSymbolMapperObjList * symbolMap,
							  LPDIRECT3DDEVICE9 g_pd3dDevice,
							  CGFontObjList * cgFontList)
{
                 if(text.GetLength() < 1)
				    return FALSE;
 

	             CTextEffectClass * newPtr = new CTextEffectClass();
                 newPtr->m_timeStampStart	= GetTickCount();
				 newPtr->m_duration			= duration;
				 newPtr->m_spawnPos			= spawnPos;
				 newPtr->m_riseMax			= riseMax;

				 

	             textRenderList->InitializeExternalMesh(g_pd3dDevice,
														text,
														fontSzX,
														fontSzY,
														symbolMap,
														0,
														0.5f,//center justified
														(void**)&newPtr->m_mesh,
														2,
														red,
								                        green,
								                        blue,
														1.0f);
                 //build constant shader block
				 newPtr->m_mesh->m_constantBlock = new ConstantBlock;
                 newPtr->m_mesh->m_constantBlock->constantCount = 3;
				 newPtr->m_mesh->m_constantBlock->constantArray = new ConstantObj[newPtr->m_mesh->m_constantBlock->constantCount];

				 
				 //store rise max-constant start index = 100
				 ZeroMemory(&newPtr->m_mesh->m_constantBlock->constantArray[0],sizeof(ConstantObj));
				 newPtr->m_mesh->m_constantBlock->constantArray[0].registerIndex = 100;
				 newPtr->m_mesh->m_constantBlock->constantArray[0].type = 0;//float constant type
				 newPtr->m_mesh->m_constantBlock->constantArray[0].register2[0] = newPtr->m_riseMax;
 
				 ZeroMemory(&newPtr->m_mesh->m_constantBlock->constantArray[1],sizeof(ConstantObj));
				 newPtr->m_mesh->m_constantBlock->constantArray[1].timeStamp = GetTickCount();
				 newPtr->m_mesh->m_constantBlock->constantArray[1].duration = newPtr->m_duration;
				 newPtr->m_mesh->m_constantBlock->constantArray[1].registerIndex = 101;
				 newPtr->m_mesh->m_constantBlock->constantArray[1].type = 3;//float and 0.0-1.0 progression

				 ZeroMemory(&newPtr->m_mesh->m_constantBlock->constantArray[2],sizeof(ConstantObj));
				 newPtr->m_mesh->m_constantBlock->constantArray[2].register2[0] = red;
				 newPtr->m_mesh->m_constantBlock->constantArray[2].register2[1] = green;
				 newPtr->m_mesh->m_constantBlock->constantArray[2].register2[2] = blue;
				 newPtr->m_mesh->m_constantBlock->constantArray[2].register2[3] = 1.0f;
				 newPtr->m_mesh->m_constantBlock->constantArray[2].registerIndex = 102;


                 newPtr->m_mesh->List_materialObject->m_vertexShader = 2;
				 newPtr->m_mesh->List_materialObject->m_pixelShader = 1;

				 POSITION fntPos = cgFontList->FindIndex(0);
	             if(fntPos)
			     {
                    CGFontObj * cg_fntObject = (CGFontObj *)cgFontList->GetAt(fntPos);
					newPtr->m_mesh->List_textureSelect[0] = cg_fntObject->m_fontTableIndex;
			     }

                 AddTail(newPtr);
				 return TRUE;
}
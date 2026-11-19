#pragma once

#include "CMenuVisual.h"
#include "CMenuImage.h"

class CMenuProgressBar : public CMenuVisual
{
public:
    CMenuProgressBar();
	~CMenuProgressBar();

	//Visible Objects
	CMenuImage	*m_backgroundLeft;
	CMenuImage	*m_backgroundMiddle;
	CMenuImage	*m_backgroundRight;
	CMenuImage	*m_foregroundLeft;		//Also used as the bottom when progress bar is verticle
	float		 m_foregroundLeftSize;	//This is used to keep the original width/height
	CMenuImage	*m_foregroundMiddle;
	CMenuImage	*m_foregroundRight;		//Also used as the top when progress bar is verticle
	float		 m_foregroundRightSize;	//This is used to keep the original width/height

	//Settable Parameters
	bool  m_isVertical;
	bool  m_isFlipped;
	float m_curPercentage;
	float m_foregroundInset;

	//Engine Variables
	bool m_mousedOver;

	//Scripts
	CMenuScript *m_updateScript;
	CMenuScript *m_mouseOverScript;
	CMenuScript *m_mouseOutScript;

	//Functions
	CMenuProgressBar& operator=(const CMenuProgressBar &rhs);
	int  GetType() { return MENU_VISTYPE_PROGRESSBAR; }
	bool HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice);
	void Render(LPDIRECT3DDEVICE9 pD3DDevice,
				CStateManagementObj *stateManager, 
				CTextureDBObjList *textureDatabase,
				float parentX,
				float parentY);
	void Resize();
	void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
};
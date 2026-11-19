#pragma once

#include "CMenuVisual.h"
#include "CMovementModel.h"
//------------------------------------------------------------//
//The purpose of this class is not to render an entity directly
//But rather to manage the animation and mesh state
//rendering calls will have to be done with the RenderEntityToSurface 
//functions and derivitives, this is required for 3d to 2d translation
//this class is used for character creation style operations
//------------------------------------------------------------//
class CMenuEntity : public CMenuVisual
{
public:
	//Settings
	int			  m_entityID;
	CMovementObj *m_entityObj;
	int			  m_animationIndex;
	float		  m_posZ;
	float		  m_scale;
	D3DVECTOR     m_centerTemp;

	//Functions
	CMenuEntity();
	~CMenuEntity();
	int  GetType() { return MENU_VISTYPE_ENTITY; }
	void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
	BOOL InitFromDBID(int index);
	BOOL UpdateAnimation();
	BOOL SetAnimationIndex(int animIndex);
	BOOL IndexDimUp(int dimIndex);
    BOOL IndexDimDown(int dimIndex);
	BOOL IndexDimUp_PublicOnly(int dimIndex);
	BOOL IndexDimDown_PublicOnly(int dimIndex);
};
// C:\Projects\EXCore_Platform\AuraUI\EditEntity.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditEntity.h"

// CEditEntity dialog
IMPLEMENT_DYNAMIC(CEditEntity, CDialog)

CEditEntity::CEditEntity(CWnd* pParent /*=NULL*/)
	: CDialog(CEditEntity::IDD, pParent)
{
	m_pEntity			= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
}

BEGIN_MESSAGE_MAP(CEditEntity, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Entity Settings Editing
	ON_EN_CHANGE(IDC_EDIT_EntityID, OnChangeEntityID)
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualZ, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_Scale, OnChangeScale)

	//Standard Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditEntity::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pEntity->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pEntity->m_visualID);

	//Entity Settings
	SetDlgItemInt(IDC_EDIT_EntityID, m_pEntity->m_entityID);
	SetDlgItemInt(IDC_EDIT_AnimationIndex, m_pEntity->m_animationIndex);

	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pEntity->m_posX * 100);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pEntity->m_posY * 100);
	SetDlgItemInt(IDC_EDIT_VisualZ, m_pEntity->m_posZ * 100);
	SetDlgItemInt(IDC_EDIT_Scale, m_pEntity->m_scale * 100);*/

	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)(m_pEntity->m_posX * 100));
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)(m_pEntity->m_posY * 100));
	SetDlgItemInt(IDC_EDIT_VisualZ, (UINT)(m_pEntity->m_posZ * 100));
	SetDlgItemInt(IDC_EDIT_Scale, (UINT)(m_pEntity->m_scale * 100));
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	m_initialized = true;

	return TRUE;
}

void CEditEntity::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditEntity::OnChangeEntityID()
{
	if(!m_initialized)
		return;

	if(m_pEntity->m_entityObj != NULL)
	{
		delete m_pEntity->m_entityObj;
		m_pEntity->m_entityObj = NULL;
	}

	m_pEntity->m_entityID = GetDlgItemInt(IDC_EDIT_EntityID);
}

void CEditEntity::OnChangeAnimationIndex()
{
	if(!m_initialized)
		return;

	m_pEntity->m_animationIndex = GetDlgItemInt(IDC_EDIT_AnimationIndex);
	m_pEntity->m_entityObj->List_skeletalObject->SetCurrentAnimation(m_pEntity->m_animationIndex, 100);
}

void CEditEntity::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	m_pEntity->m_posX	= ((float)((int)GetDlgItemInt(IDC_EDIT_VisualX))) / 100.0f;
	m_pEntity->m_posY	= ((float)((int)GetDlgItemInt(IDC_EDIT_VisualY))) / 100.0f;
	m_pEntity->m_posZ	= ((float)((int)GetDlgItemInt(IDC_EDIT_VisualZ))) / 100.0f;
}

void CEditEntity::OnChangeScale()
{
	if(!m_initialized)
		return;

	//Update entity scale
	m_pEntity->m_scale	= ((float)((int)GetDlgItemInt(IDC_EDIT_Scale))) / 100.0f;
}

void CEditEntity::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pEntity->m_name = temp;

	m_pEntity->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);

	CDialog::OnOK();
}

// CEditEntity message handlers

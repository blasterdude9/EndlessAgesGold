// C:\Projects\EXCore_Platform\AuraUI\EditCursors.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditCursors.h"


// CEditCursors dialog

IMPLEMENT_DYNAMIC(CEditCursors, CDialog)

CEditCursors::CEditCursors(CWnd* pParent /*=NULL*/)
	: CDialog(CEditCursors::IDD, pParent)
{
	m_gStatus		= NULL;
	m_curCursor		= NULL;
	m_pCursorImage	= NULL;
	m_initialized	= false;
}

BEGIN_MESSAGE_MAP(CEditCursors, CDialog)
	//Cursors List
	ON_LBN_SELCHANGE(IDC_LIST_Cursors, OnSelectionChangeListCursors)

	//Cursor Settings
	ON_EN_CHANGE(IDC_EDIT_CursorTextureX, OnChangeTextureCoordinates)
	ON_EN_CHANGE(IDC_EDIT_CursorTextureY, OnChangeTextureCoordinates)
	ON_EN_CHANGE(IDC_EDIT_CursorTextureWidth, OnChangeTextureCoordinates)
	ON_EN_CHANGE(IDC_EDIT_CursorTextureHeight, OnChangeTextureCoordinates)
	ON_BN_CLICKED(IDC_BUTTON_EditCursorTexture, OnButtonEditCursorTexture)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditCursors::OnInitDialog()
{
	CDialog::OnInitDialog();

	FillCursorList();

	//Allow the editing of Cursor properties
	m_initialized = true;

	return TRUE;
}

void CEditCursors::OnSelectionChangeListCursors()
{
	//Don't allow the editing of Cursor properties
	m_initialized = false;

	//If they have a cursor selected
	int cursorIndex = SendDlgItemMessage(IDC_LIST_Cursors, LB_GETCURSEL);
	if(cursorIndex != -1)
	{
		//Get a pointer to the selected cursor
		m_curCursor = m_gStatus->m_mouseCursors[cursorIndex];

		//Set the texture coordinates for this cursor
		/*SetDlgItemInt(IDC_EDIT_CursorTextureX, m_curCursor->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_CursorTextureY, m_curCursor->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_CursorTextureWidth, m_curCursor->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_CursorTextureHeight, m_curCursor->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_CursorTextureX, (UINT)m_curCursor->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_CursorTextureY, (UINT)m_curCursor->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_CursorTextureWidth, (UINT)m_curCursor->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_CursorTextureHeight, (UINT)m_curCursor->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data

		//Copy this cursor's settings into the visible image
		m_pCursorImage->m_texturePosX		= m_curCursor->m_texturePosX;
		m_pCursorImage->m_texturePosY		= m_curCursor->m_texturePosY;
		m_pCursorImage->m_width				= m_pCursorImage->m_textureWidth  = m_curCursor->m_textureWidth;
		m_pCursorImage->m_height			= m_pCursorImage->m_textureHeight = m_curCursor->m_textureHeight;
		m_pCursorImage->m_fullTextureWidth	= m_curCursor->m_fullTextureWidth;
		m_pCursorImage->m_fullTextureHeight	= m_curCursor->m_fullTextureHeight;
		m_pCursorImage->UpdateTextureCoords();

		m_pCursorImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.r	= m_curCursor->m_red;
		m_pCursorImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.g	= m_curCursor->m_green;
		m_pCursorImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.b	= m_curCursor->m_blue;
		m_pCursorImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.a	= m_curCursor->m_alpha;
		m_pCursorImage->m_imageMesh->List_materialObject->m_blendMode				= m_curCursor->m_blendMode;
		m_pCursorImage->m_imageMesh->List_textureSelect[0]							= m_curCursor->m_textureSelect;
	}
	
	//Allow the editing of Cursor properties again
	m_initialized = true;
}

void CEditCursors::OnChangeTextureCoordinates()
{
	if(!m_initialized || !m_curCursor)
		return;

	//Copy the new setttings
	/*m_pCursorImage->m_texturePosX	= m_curCursor->m_texturePosX	= (int)GetDlgItemInt(IDC_EDIT_CursorTextureX);
	m_pCursorImage->m_texturePosY	= m_curCursor->m_texturePosY	= (int)GetDlgItemInt(IDC_EDIT_CursorTextureY);
	m_pCursorImage->m_textureWidth	= m_curCursor->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_CursorTextureWidth);
	m_pCursorImage->m_textureHeight	= m_curCursor->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_CursorTextureHeight);*/

	m_pCursorImage->m_texturePosX	= m_curCursor->m_texturePosX	= (float)(int)GetDlgItemInt(IDC_EDIT_CursorTextureX);
	m_pCursorImage->m_texturePosY	= m_curCursor->m_texturePosY	= (float)(int)GetDlgItemInt(IDC_EDIT_CursorTextureY);
	m_pCursorImage->m_textureWidth	= m_curCursor->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_CursorTextureWidth);
	m_pCursorImage->m_textureHeight	= m_curCursor->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_CursorTextureHeight);
	//WARNING: C4244 --> conversion from DWORD to float, possible loss of data

	//Change the width and height of the cursor
	m_pCursorImage->m_width	 = m_curCursor->m_width	 = m_pCursorImage->m_textureWidth;
	m_pCursorImage->m_height = m_curCursor->m_height = m_pCursorImage->m_textureHeight;
	
	//Update Cursor Texture Coordinate Calculations
	UpdateTextureCoords();
}

void CEditCursors::OnButtonEditCursorTexture()
{
	if(!m_curCursor)
		return;

	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.meshRef = m_pCursorImage->m_imageMesh;

	if(dlg.DoModal() == IDOK)
	{
		m_pCursorImage->m_imageMesh->List_materialObject->Reset();

		//Copy new material and texture properties
		m_curCursor->m_red			 = m_pCursorImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.r;
		m_curCursor->m_green		 = m_pCursorImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.g;
		m_curCursor->m_blue			 = m_pCursorImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.b;
		m_curCursor->m_alpha		 = m_pCursorImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.a;
		m_curCursor->m_blendMode	 = m_pCursorImage->m_imageMesh->List_materialObject->m_blendMode;
		m_curCursor->m_textureSelect = m_pCursorImage->m_imageMesh->List_textureSelect[0];

		//Update Cursor Texture Coordinate Calculations
		UpdateTextureCoords();
	}
}

void CEditCursors::OnButtonNext()
{
	CDialog::OnOK();
}

void CEditCursors::UpdateTextureCoords()
{
	//Get the texture dimensions
	float textureWidth = 512.0f;
	float textureHeight = 512.0f;
	char *textureName	= NULL;

	//Get the dimensions of the texture they selected, if any
	POSITION pos = m_gStatus->m_textureDataBase->FindIndex(m_pCursorImage->m_imageMesh->List_textureSelect[0]);
	if(pos != NULL)
	{
		CTextureDBObj *texture = (CTextureDBObj*)m_gStatus->m_textureDataBase->GetAt(pos);

		/*textureWidth	= texture->m_textureObject->m_widthBasis;
		textureHeight	= texture->m_textureObject->m_hieghtBasis;*/

		textureWidth	= (float)texture->m_textureObject->m_widthBasis;
		textureHeight	= (float)texture->m_textureObject->m_hieghtBasis;
		//WARNING: C4244 --> conversion from DWORD to float, possible loss of data

		textureName		= texture->List_textureName;
	}

	//Copy texture file
	if(textureName)
		m_curCursor->m_textureFile = textureName;

	//Update mesh's texture coordinates
	m_pCursorImage->m_fullTextureWidth	= m_curCursor->m_fullTextureWidth	= textureWidth;
	m_pCursorImage->m_fullTextureHeight	= m_curCursor->m_fullTextureHeight	= textureHeight;
	m_pCursorImage->UpdateTextureCoords();
}

void CEditCursors::FillCursorList()
{
	SendDlgItemMessage(IDC_LIST_Cursors, LB_ADDSTRING, 0, (LPARAM)"Normal");
	SendDlgItemMessage(IDC_LIST_Cursors, LB_ADDSTRING, 0, (LPARAM)"Enemy Hover");
	SendDlgItemMessage(IDC_LIST_Cursors, LB_ADDSTRING, 0, (LPARAM)"Examine");
}

// CEditCursors message handlers

// AddMenu.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AddMenu.h"
#include "EditScript.h"

// CAddMenu dialog
CAddMenu::CAddMenu(CWnd* pParent /*=NULL*/)
	: CDialog(CAddMenu::IDD, pParent)
{
	m_pMenu				= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
	m_editTitleDlg		= NULL;
}

BEGIN_MESSAGE_MAP(CAddMenu, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Properties
	ON_CBN_SELCHANGE(IDC_COMBO_Alignment, OnChangeAlignment)
	ON_BN_CLICKED(IDC_CHECK_Draggable, OnToggleDraggable)
	ON_BN_CLICKED(IDC_CHECK_Resizable, OnToggleResizable)
	ON_BN_CLICKED(IDC_CHECK_Escapable, OnToggleEscapable)
	ON_BN_CLICKED(IDC_CHECK_ShowMouseCursor, OnToggleShowMouseCursor)

	//Menu Coord Edit
	ON_EN_CHANGE(IDC_EDIT_MenuX, OnChangeMenuCoords)
	ON_EN_CHANGE(IDC_EDIT_MenuY, OnChangeMenuCoords)
	ON_EN_CHANGE(IDC_EDIT_MenuWidth, OnChangeMenuCoords)
	ON_EN_CHANGE(IDC_EDIT_MenuHeight, OnChangeMenuCoords)

	//Background Texture Edit
	ON_EN_CHANGE(IDC_EDIT_BackgroundTextureX, OnChangeBackgroundTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundTextureY, OnChangeBackgroundTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundTextureWidth, OnChangeBackgroundTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundTextureHeight, OnChangeBackgroundTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuBackgroundTexture, OnButtonEditBackgroundTexture)
	ON_EN_CHANGE(IDC_EDIT_BackgroundInset, OnChangeBackgroundInset)
	
	//Top Border Texture Edit
	ON_EN_CHANGE(IDC_EDIT_TopBorderTextureX, OnChangeTopBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_TopBorderTextureY, OnChangeTopBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_TopBorderTextureWidth, OnChangeTopBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_TopBorderTextureHeight, OnChangeTopBorderTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuTopBorderTexture, OnButtonEditTopBorderTexture)

	//Left Border Texture Edit
	ON_EN_CHANGE(IDC_EDIT_LeftBorderTextureX, OnChangeLeftBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftBorderTextureY, OnChangeLeftBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftBorderTextureWidth, OnChangeLeftBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftBorderTextureHeight, OnChangeLeftBorderTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuLeftBorderTexture, OnButtonEditLeftBorderTexture)

	//Bottom Border Texture Edit
	ON_EN_CHANGE(IDC_EDIT_BottomBorderTextureX, OnChangeBottomBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BottomBorderTextureY, OnChangeBottomBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BottomBorderTextureWidth, OnChangeBottomBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BottomBorderTextureHeight, OnChangeBottomBorderTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuBottomBorderTexture, OnButtonEditBottomBorderTexture)

	//Right Border Texture Edit
	ON_EN_CHANGE(IDC_EDIT_RightBorderTextureX, OnChangeRightBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightBorderTextureY, OnChangeRightBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightBorderTextureWidth, OnChangeRightBorderTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightBorderTextureHeight, OnChangeRightBorderTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuRightBorderTexture, OnButtonEditRightBorderTexture)

	//Top Left Corner Texture Edit
	ON_EN_CHANGE(IDC_EDIT_TopLeftCornerTextureX, OnChangeTopLeftCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_TopLeftCornerTextureY, OnChangeTopLeftCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_TopLeftCornerTextureWidth, OnChangeTopLeftCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_TopLeftCornerTextureHeight, OnChangeTopLeftCornerTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuTopLeftCornerTexture, OnButtonEditTopLeftCornerTexture)

	//Top Right Corner Texture Edit
	ON_EN_CHANGE(IDC_EDIT_TopRightCornerTextureX, OnChangeTopRightCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_TopRightCornerTextureY, OnChangeTopRightCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_TopRightCornerTextureWidth, OnChangeTopRightCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_TopRightCornerTextureHeight, OnChangeTopRightCornerTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuTopRightCornerTexture, OnButtonEditTopRightCornerTexture)

	//Bottom Left Corner Texture Edit
	ON_EN_CHANGE(IDC_EDIT_BottomLeftCornerTextureX, OnChangeBottomLeftCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BottomLeftCornerTextureY, OnChangeBottomLeftCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BottomLeftCornerTextureWidth, OnChangeBottomLeftCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BottomLeftCornerTextureHeight, OnChangeBottomLeftCornerTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuBottomLeftCornerTexture, OnButtonEditBottomLeftCornerTexture)

	//Bottom Right Corner Texture Edit
	ON_EN_CHANGE(IDC_EDIT_BottomRightCornerTextureX, OnChangeBottomRightCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BottomRightCornerTextureY, OnChangeBottomRightCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BottomRightCornerTextureWidth, OnChangeBottomRightCornerTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BottomRightCornerTextureHeight, OnChangeBottomRightCornerTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuBottomRightCornerTexture, OnButtonEditBottomRightCornerTexture)

	//Corner Offset Edit
	ON_EN_CHANGE(IDC_EDIT_MenuCornerOffsetX, OnChangeMenuCornerOffsetCoords)
	ON_EN_CHANGE(IDC_EDIT_MenuCornerOffsetY, OnChangeMenuCornerOffsetCoords)

	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditLoadScript, OnButtonEditLoadScript)
	ON_BN_CLICKED(IDC_BUTTON_EditResizeScript, OnButtonEditResizeScript)
	ON_BN_CLICKED(IDC_BUTTON_EditCloseScript, OnButtonEditCloseScript)

	//Edit Title
	ON_BN_CLICKED(IDC_BUTTON_EditTitle, OnButtonEditTitle)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddMenu message handlers

BOOL CAddMenu::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Identifying Data
	SetDlgItemText(IDC_EDIT_MenuName, m_pMenu->m_name);
	SetDlgItemInt(IDC_EDIT_MenuVisualID, m_pMenu->m_visualID);

	//Set up the alignment combo box
	InitializeAlignmentComboBox();

	//Menu Coordinates
	
	/*SetDlgItemInt(IDC_EDIT_MenuX, m_pMenu->m_posX);
	SetDlgItemInt(IDC_EDIT_MenuY, m_pMenu->m_posY);
	SetDlgItemInt(IDC_EDIT_MenuWidth, m_pMenu->m_width);
	SetDlgItemInt(IDC_EDIT_MenuHeight, m_pMenu->m_height);*/

	SetDlgItemInt(IDC_EDIT_MenuX, (UINT)m_pMenu->m_posX);
	SetDlgItemInt(IDC_EDIT_MenuY, (UINT)m_pMenu->m_posY);
	SetDlgItemInt(IDC_EDIT_MenuWidth, (UINT)m_pMenu->m_width);
	SetDlgItemInt(IDC_EDIT_MenuHeight, (UINT)m_pMenu->m_height);
	//WARNING: C4244 --> possible loss of data; float to UINT

	//Background Texture Coordinates
	if(m_pMenu->m_background)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundTextureX, m_pMenu->m_background->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureY, m_pMenu->m_background->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureWidth, m_pMenu->m_background->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureHeight, m_pMenu->m_background->m_textureHeight);*/		
		
		SetDlgItemInt(IDC_EDIT_BackgroundTextureX, (UINT)m_pMenu->m_background->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureY, (UINT)m_pMenu->m_background->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureWidth, (UINT)m_pMenu->m_background->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureHeight, (UINT)m_pMenu->m_background->m_textureHeight);
		//WARNING: C4244 --> possible loss of data; float to UINT
	}

	//Background Inset
	/*SetDlgItemInt(IDC_EDIT_BackgroundInset, m_pMenu->m_backgroundInset);*/
	SetDlgItemInt(IDC_EDIT_BackgroundInset, (UINT)m_pMenu->m_backgroundInset);
	//WARNING: C4244 --> possible loss of data; float to UINT

	//Top Border Texture Coordinates
	if(m_pMenu->m_borderTop)
	{
		/*SetDlgItemInt(IDC_EDIT_TopBorderTextureX, m_pMenu->m_borderTop->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureY, m_pMenu->m_borderTop->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureWidth, m_pMenu->m_borderTop->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureHeight, m_pMenu->m_borderTop->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_TopBorderTextureX, (UINT)m_pMenu->m_borderTop->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureY, (UINT)m_pMenu->m_borderTop->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureWidth, (UINT)m_pMenu->m_borderTop->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureHeight, (UINT)m_pMenu->m_borderTop->m_textureHeight);
		//WARNING: C4244 --> possible loss of data; float to UINT
	}

	//Left Border Texture Coordinates
	if(m_pMenu->m_borderLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_LeftBorderTextureX, m_pMenu->m_borderLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureY, m_pMenu->m_borderLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureWidth, m_pMenu->m_borderLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureHeight, m_pMenu->m_borderLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_LeftBorderTextureX, (UINT)m_pMenu->m_borderLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureY, (UINT)m_pMenu->m_borderLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureWidth, (UINT)m_pMenu->m_borderLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureHeight, (UINT)m_pMenu->m_borderLeft->m_textureHeight);
		//WARNING: C4244 --> possible loss of data; float to UINT
	}

	//Bottom Border Texture Coordinates
	if(m_pMenu->m_borderBottom)
	{
		/*SetDlgItemInt(IDC_EDIT_BottomBorderTextureX, m_pMenu->m_borderBottom->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureY, m_pMenu->m_borderBottom->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureWidth, m_pMenu->m_borderBottom->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureHeight, m_pMenu->m_borderBottom->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BottomBorderTextureX, (UINT)m_pMenu->m_borderBottom->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureY, (UINT)m_pMenu->m_borderBottom->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureWidth, (UINT)m_pMenu->m_borderBottom->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureHeight, (UINT)m_pMenu->m_borderBottom->m_textureHeight);
		//WARNING: C4244 --> possible loss of data; float to UINT
	}

	//Right Border Texture Coordinates
	if(m_pMenu->m_borderRight)
	{
		/*SetDlgItemInt(IDC_EDIT_RightBorderTextureX, m_pMenu->m_borderRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureY, m_pMenu->m_borderRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureWidth, m_pMenu->m_borderRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureHeight, m_pMenu->m_borderRight->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_RightBorderTextureX, (UINT)m_pMenu->m_borderRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureY, (UINT)m_pMenu->m_borderRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureWidth, (UINT)m_pMenu->m_borderRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureHeight, (UINT)m_pMenu->m_borderRight->m_textureHeight);
		//WARNING: C4244 --> possible loss of data; float to UINT
	}

	//Top Left Corner Texture Coordinates
	if(m_pMenu->m_cornerTopLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureX, m_pMenu->m_cornerTopLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureY, m_pMenu->m_cornerTopLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureWidth, m_pMenu->m_cornerTopLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureHeight, m_pMenu->m_cornerTopLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureX, (UINT)m_pMenu->m_cornerTopLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureY, (UINT)m_pMenu->m_cornerTopLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureWidth, (UINT)m_pMenu->m_cornerTopLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureHeight, (UINT)m_pMenu->m_cornerTopLeft->m_textureHeight);
		//WARNING: C4244 --> possible loss of data; float to UINT
	}
	//Top Right Corner Texture Coordinates
	if(m_pMenu->m_cornerTopRight)
	{
		/*SetDlgItemInt(IDC_EDIT_TopRightCornerTextureX, m_pMenu->m_cornerTopRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureY, m_pMenu->m_cornerTopRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureWidth, m_pMenu->m_cornerTopRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureHeight, m_pMenu->m_cornerTopRight->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureX, (UINT)m_pMenu->m_cornerTopRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureY, (UINT)m_pMenu->m_cornerTopRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureWidth, (UINT)m_pMenu->m_cornerTopRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureHeight, (UINT)m_pMenu->m_cornerTopRight->m_textureHeight);
		//WARNING: C4244 --> possible loss of data; float to UINT
	}

	//Bottom Left Corner Texture Coordinates
	if(m_pMenu->m_cornerBottomLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureX, m_pMenu->m_cornerBottomLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureY, m_pMenu->m_cornerBottomLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureWidth, m_pMenu->m_cornerBottomLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureHeight, m_pMenu->m_cornerBottomLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureX, (UINT)m_pMenu->m_cornerBottomLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureY, (UINT)m_pMenu->m_cornerBottomLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureWidth, (UINT)m_pMenu->m_cornerBottomLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureHeight, (UINT)m_pMenu->m_cornerBottomLeft->m_textureHeight);
		//WARNING: C4244 --> possible loss of data; float to UINT
	}

	//Bottom Right Corner Texture Coordinates
	if(m_pMenu->m_cornerBottomRight)
	{
		/*SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureX, m_pMenu->m_cornerBottomRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureY, m_pMenu->m_cornerBottomRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureWidth, m_pMenu->m_cornerBottomRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureHeight, m_pMenu->m_cornerBottomRight->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureX, (UINT)m_pMenu->m_cornerBottomRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureY, (UINT)m_pMenu->m_cornerBottomRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureWidth, (UINT)m_pMenu->m_cornerBottomRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureHeight, (UINT)m_pMenu->m_cornerBottomRight->m_textureHeight);
		//WARNING: C4244 --> possible loss of data; float to UINT
	}

	//Corner Offset Coordinates
	/*SetDlgItemInt(IDC_EDIT_MenuCornerOffsetX, m_pMenu->m_cornerOffsetX);
	SetDlgItemInt(IDC_EDIT_MenuCornerOffsetY, m_pMenu->m_cornerOffsetY);*/

	SetDlgItemInt(IDC_EDIT_MenuCornerOffsetX, (UINT)m_pMenu->m_cornerOffsetX);
	SetDlgItemInt(IDC_EDIT_MenuCornerOffsetY, (UINT)m_pMenu->m_cornerOffsetY);
	//WARNING: C4244 --> possible loss of data; float to UINT

	//Check Boxes
	/*CheckDlgButton(IDC_CHECK_Draggable, m_pMenu->m_draggable);
	CheckDlgButton(IDC_CHECK_Resizable, m_pMenu->m_resizable);
	CheckDlgButton(IDC_CHECK_Escapable, m_pMenu->m_escapable);
	CheckDlgButton(IDC_CHECK_ShowMouseCursor, m_pMenu->m_showCursor);*/

	CheckDlgButton(IDC_CHECK_Draggable, (UINT)m_pMenu->m_draggable);
	CheckDlgButton(IDC_CHECK_Resizable, (UINT)m_pMenu->m_resizable);
	CheckDlgButton(IDC_CHECK_Escapable, (UINT)m_pMenu->m_escapable);
	CheckDlgButton(IDC_CHECK_ShowMouseCursor, (UINT)m_pMenu->m_showCursor);
	//WARNING: C4244 --> possible loss of data; float to UINT

	m_initialized = true;

	return TRUE;
}

void CAddMenu::OnButtonGetFreeID()
{
	/*SetDlgItemInt(IDC_EDIT_MenuVisualID, m_freeVisualID);*/

	SetDlgItemInt(IDC_EDIT_MenuVisualID, (UINT)m_freeVisualID);
	//WARNING: C4244 --> possible loss of data; float to UINT
}

void CAddMenu::OnChangeAlignment()
{
	switch(SendDlgItemMessage(IDC_COMBO_Alignment, CB_GETCURSEL))
	{
		case 0: //Bottom Left
			m_pMenu->m_horizontalAlignment	= -1.0f;
			m_pMenu->m_verticalAlignment	= -1.0f;
			break;
		case 1: //Left
			m_pMenu->m_horizontalAlignment	= -1.0f;
			m_pMenu->m_verticalAlignment	= 0.0f;
			break;
		case 2: //Top Left
			m_pMenu->m_horizontalAlignment	= -1.0f;
			m_pMenu->m_verticalAlignment	= 1.0f;
			break;
		case 3: //Top
			m_pMenu->m_horizontalAlignment	= 0.0f;
			m_pMenu->m_verticalAlignment	= 1.0f;
			break;
		case 4: //Top Right
			m_pMenu->m_horizontalAlignment	= 1.0f;
			m_pMenu->m_verticalAlignment	= 1.0f;
			break;
		case 5: //Right
			m_pMenu->m_horizontalAlignment	= 1.0f;
			m_pMenu->m_verticalAlignment	= 0.0f;
			break;
		case 6: //Bottom Right
			m_pMenu->m_horizontalAlignment	= 1.0f;
			m_pMenu->m_verticalAlignment	= -1.0f;
			break;
		case 7: //Bottom
			m_pMenu->m_horizontalAlignment	= 0.0f;
			m_pMenu->m_verticalAlignment	= -1.0f;
			break;
		case 8: //Center
			m_pMenu->m_horizontalAlignment	= 0.0f;
			m_pMenu->m_verticalAlignment	= 0.0f;
			break;
	}
}

void CAddMenu::OnToggleDraggable()
{
	m_pMenu->m_draggable = IsDlgButtonChecked(IDC_CHECK_Draggable);
}

void CAddMenu::OnToggleResizable()
{
	m_pMenu->m_resizable = IsDlgButtonChecked(IDC_CHECK_Resizable);
}

void CAddMenu::OnToggleEscapable()
{
	m_pMenu->m_escapable = IsDlgButtonChecked(IDC_CHECK_Escapable);
}

void CAddMenu::OnToggleShowMouseCursor()
{
	m_pMenu->m_showCursor = IsDlgButtonChecked(IDC_CHECK_ShowMouseCursor);
}

void CAddMenu::OnChangeMenuCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pMenu->m_posX		= (int)GetDlgItemInt(IDC_EDIT_MenuX);
	m_pMenu->m_posY		= (int)GetDlgItemInt(IDC_EDIT_MenuY);
	m_pMenu->m_width	= (int)GetDlgItemInt(IDC_EDIT_MenuWidth);
	m_pMenu->m_height	= (int)GetDlgItemInt(IDC_EDIT_MenuHeight);*/

	m_pMenu->m_posX		= (float)(int)GetDlgItemInt(IDC_EDIT_MenuX);
	m_pMenu->m_posY		= (float)(int)GetDlgItemInt(IDC_EDIT_MenuY);
	m_pMenu->m_width	= (float)(int)GetDlgItemInt(IDC_EDIT_MenuWidth);
	m_pMenu->m_height	= (float)(int)GetDlgItemInt(IDC_EDIT_MenuHeight);
	//WARNING: C4244 --> possible loss of data; int to float

	//Update all child coordinates
	m_pMenu->Resize();
}

void CAddMenu::OnChangeBackgroundTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_background)
		m_pMenu->m_background = new CMenuImage();

	/*UpdateImageTextureCoords(m_pMenu->m_background,
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundTextureHeight));*/

	UpdateImageTextureCoords(m_pMenu->m_background,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundTextureHeight));
	//WARNING: C4244 --> possible loss of data; int to float
}

void CAddMenu::OnButtonEditBackgroundTexture()
{
	if(!m_pMenu->m_background)
	{
		m_pMenu->m_background = new CMenuImage();
		return;
	}

	EditTexture(m_pMenu->m_background);
}

void CAddMenu::OnChangeBackgroundInset()
{
	/*m_pMenu->m_backgroundInset = (int)GetDlgItemInt(IDC_EDIT_BackgroundInset);*/

	m_pMenu->m_backgroundInset = (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundInset);
	//WARNING: C4244 --> possible loss of data; int to float
	m_pMenu->Resize();
}

void CAddMenu::OnChangeTopBorderTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_borderTop)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pMenu->m_borderTop,
							 (int)GetDlgItemInt(IDC_EDIT_TopBorderTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_TopBorderTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_TopBorderTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_TopBorderTextureHeight));*/

	UpdateImageTextureCoords(m_pMenu->m_borderTop,
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopBorderTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopBorderTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopBorderTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopBorderTextureHeight));
	//WARNING: C4244 --> possible loss of data; int to float
}

void CAddMenu::OnButtonEditTopBorderTexture()
{
	if(!m_pMenu->m_borderTop)
		return CreateBordersAndCorners();

	EditTexture(m_pMenu->m_borderTop);
}

void CAddMenu::OnChangeLeftBorderTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_borderLeft)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pMenu->m_borderLeft,
							 (int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureHeight));*/

	UpdateImageTextureCoords(m_pMenu->m_borderLeft,
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureHeight));
	//WARNING: C4244 --> possible loss of data; int to float
}

void CAddMenu::OnButtonEditLeftBorderTexture()
{
	if(!m_pMenu->m_borderLeft)
		return CreateBordersAndCorners();

	EditTexture(m_pMenu->m_borderLeft);
}

void CAddMenu::OnChangeBottomBorderTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_borderBottom)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pMenu->m_borderBottom,
							 (int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureHeight));*/

	UpdateImageTextureCoords(m_pMenu->m_borderBottom,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureHeight));
	//WARNING: C4244 --> possible loss of data; int to float
}

void CAddMenu::OnButtonEditBottomBorderTexture()
{
	if(!m_pMenu->m_borderBottom)
		return CreateBordersAndCorners();

	EditTexture(m_pMenu->m_borderBottom);
}

void CAddMenu::OnChangeRightBorderTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_borderRight)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pMenu->m_borderRight,
							 (int)GetDlgItemInt(IDC_EDIT_RightBorderTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_RightBorderTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_RightBorderTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_RightBorderTextureHeight));*/

	UpdateImageTextureCoords(m_pMenu->m_borderRight,
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightBorderTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightBorderTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightBorderTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightBorderTextureHeight));
	//WARNING: C4244 --> possible loss of data; int to float
}

void CAddMenu::OnButtonEditRightBorderTexture()
{
	if(!m_pMenu->m_borderRight)
		return CreateBordersAndCorners();

	EditTexture(m_pMenu->m_borderRight);
}

void CAddMenu::OnChangeTopLeftCornerTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_cornerTopLeft)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pMenu->m_cornerTopLeft,
							 (int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureHeight));*/

	UpdateImageTextureCoords(m_pMenu->m_cornerTopLeft,
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureHeight));
	//WARNING: C4244 --> possible loss of data; int to float
}

void CAddMenu::OnButtonEditTopLeftCornerTexture()
{
	if(!m_pMenu->m_cornerTopLeft)
		return CreateBordersAndCorners();

	EditTexture(m_pMenu->m_cornerTopLeft);
}

void CAddMenu::OnChangeTopRightCornerTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_cornerTopRight)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pMenu->m_cornerTopRight,
							 (int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureHeight));*/

	UpdateImageTextureCoords(m_pMenu->m_cornerTopRight,
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureHeight));
	//WARNING: C4244 --> possible loss of data; int to float
}

void CAddMenu::OnButtonEditTopRightCornerTexture()
{
	if(!m_pMenu->m_cornerTopRight)
		return CreateBordersAndCorners();

	EditTexture(m_pMenu->m_cornerTopRight);
}

void CAddMenu::OnChangeBottomLeftCornerTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_cornerBottomLeft)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pMenu->m_cornerBottomLeft,
							 (int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureHeight));*/

	UpdateImageTextureCoords(m_pMenu->m_cornerBottomLeft,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureHeight));
	//WARNING: C4244 --> possible loss of data; int to float
}

void CAddMenu::OnButtonEditBottomLeftCornerTexture()
{
	if(!m_pMenu->m_cornerBottomLeft)
		return CreateBordersAndCorners();

	EditTexture(m_pMenu->m_cornerBottomLeft);
}

void CAddMenu::OnChangeBottomRightCornerTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_cornerBottomRight)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pMenu->m_cornerBottomRight,
							 (int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureHeight));*/

	UpdateImageTextureCoords(m_pMenu->m_cornerBottomRight,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureHeight));
	//WARNING: C4244 --> possible loss of data; int to float
}

void CAddMenu::OnButtonEditBottomRightCornerTexture()
{
	if(!m_pMenu->m_cornerBottomRight)
		return CreateBordersAndCorners();

	EditTexture(m_pMenu->m_cornerBottomRight);
}

void CAddMenu::OnChangeMenuCornerOffsetCoords()
{
	if(!m_initialized)
		return;

	/*m_pMenu->m_cornerOffsetX = (int)GetDlgItemInt(IDC_EDIT_MenuCornerOffsetX);
	m_pMenu->m_cornerOffsetY = (int)GetDlgItemInt(IDC_EDIT_MenuCornerOffsetY);*/

	m_pMenu->m_cornerOffsetX = (float)(int)GetDlgItemInt(IDC_EDIT_MenuCornerOffsetX);
	m_pMenu->m_cornerOffsetY = (float)(int)GetDlgItemInt(IDC_EDIT_MenuCornerOffsetY);
	//WARNING: C4244 --> possible loss of data; int to float

	m_pMenu->Resize();
}

void CAddMenu::OnButtonEditLoadScript()
{
	if(!m_pMenu->m_loadScript)
	{
		m_pMenu->m_loadScript				 = new CMenuScript();
		m_pMenu->m_loadScript->m_callingMenu = m_pMenu;
	}

	CEditScript dlg;
	dlg.m_script = m_pMenu->m_loadScript;
	dlg.DoModal();
}

void CAddMenu::OnButtonEditResizeScript()
{
	if(!m_pMenu->m_resizeScript)
	{
		m_pMenu->m_resizeScript					= new CMenuScript();
		m_pMenu->m_resizeScript->m_callingMenu	= m_pMenu;
	}

	CEditScript dlg;
	dlg.m_script = m_pMenu->m_resizeScript;
	dlg.DoModal();
}

void CAddMenu::OnButtonEditCloseScript()
{
	if(!m_pMenu->m_closeScript)
	{
		m_pMenu->m_closeScript					= new CMenuScript();
		m_pMenu->m_closeScript->m_callingMenu	= m_pMenu;
	}

	CEditScript dlg;
	dlg.m_script = m_pMenu->m_closeScript;
	dlg.DoModal();
}

void CAddMenu::OnButtonEditTitle()
{
	//Delete the old dialog
	if(m_editTitleDlg)
		delete m_editTitleDlg;

	//Create a new Dialog
	m_editTitleDlg						= new CEditMenuTitle();
	m_editTitleDlg->m_pMenu				= m_pMenu;
	m_editTitleDlg->m_pTextureDatabase	= m_gStatus->m_textureDataBase;
	m_editTitleDlg->m_pFontList			= m_gStatus->m_menuFontList;

	//Show the dialog
	m_editTitleDlg->Create(IDD_DIALOG_MenuSysEditMenuTitle);
}

void CAddMenu::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_MenuName, temp, 255);
	m_pMenu->m_name = temp;

	m_pMenu->m_visualID = (int)GetDlgItemInt(IDC_EDIT_MenuVisualID);

	CDialog::OnOK();
}

void CAddMenu::InitializeAlignmentComboBox()
{
	//Fill the alignment list
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Bottom Left");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Left");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Top Left");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Top");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Top Right");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Right");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Bottom Right");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Bottom");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Center");

	if(m_pMenu->m_horizontalAlignment == -1.0f)
	{
		if(m_pMenu->m_verticalAlignment == -1.0f)
			SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, 0);
		else if(m_pMenu->m_verticalAlignment == 0.0f)
			SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, 1);
		else
			SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, 2);
	}
	else if(m_pMenu->m_horizontalAlignment == 0.0f)
	{
		if(m_pMenu->m_verticalAlignment == -1.0f)
			SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, 7);
		else if(m_pMenu->m_verticalAlignment == 0.0f)
			SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, 8);
		else
			SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, 3);
	}
	else
	{
		if(m_pMenu->m_verticalAlignment == -1.0f)
			SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, 6);
		else if(m_pMenu->m_verticalAlignment == 0.0f)
			SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, 5);
		else
			SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, 4);
	}
}

void CAddMenu::CreateBordersAndCorners()
{
	m_pMenu->m_borderLeft		 = new CMenuImage();
	m_pMenu->m_borderTop		 = new CMenuImage();
	m_pMenu->m_borderRight		 = new CMenuImage();
	m_pMenu->m_borderBottom		 = new CMenuImage();
	m_pMenu->m_cornerTopLeft	 = new CMenuImage();
	m_pMenu->m_cornerTopRight	 = new CMenuImage();
	m_pMenu->m_cornerBottomLeft	 = new CMenuImage();
	m_pMenu->m_cornerBottomRight = new CMenuImage();
}

void CAddMenu::EditTexture(CMenuImage *pImage)
{
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.meshRef = pImage->m_imageMesh;

	if(dlg.DoModal() == IDOK)
	{
		pImage->m_imageMesh->List_materialObject->Reset();
		
		//Copy new material and texture properties
		pImage->m_red			= pImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.r;
		pImage->m_green			= pImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.g;
		pImage->m_blue			= pImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.b;
		pImage->m_alpha			= pImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.a;
		pImage->m_blendMode		= pImage->m_imageMesh->List_materialObject->m_blendMode;
		pImage->m_textureSelect	= pImage->m_imageMesh->List_textureSelect[0];

		UpdateImageTextureCoords(pImage);
	}
}

void CAddMenu::UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, float Height)
{
	pImage->m_texturePosX	= X;
	pImage->m_texturePosY	= Y;
	pImage->m_textureWidth	= Width;
	pImage->m_textureHeight	= Height;

	UpdateImageTextureCoords(pImage);
}

void CAddMenu::UpdateImageTextureCoords(CMenuImage *pImage)
{
	//Get the texture dimensions
	if(pImage->m_imageMesh)
	{
		float textureWidth = 512.0f;
		float textureHeight = 512.0f;
		char *textureName	= NULL;

		//Get the dimensions of the texture they selected, if any
		POSITION pos = m_gStatus->m_textureDataBase->FindIndex(pImage->m_imageMesh->List_textureSelect[0]);
		if(pos != NULL)
		{
			CTextureDBObj *texture = (CTextureDBObj*)m_gStatus->m_textureDataBase->GetAt(pos);
			/*textureWidth	= texture->m_textureObject->m_widthBasis;
			textureHeight	= texture->m_textureObject->m_hieghtBasis;*/

			textureWidth	= (float)texture->m_textureObject->m_widthBasis;
			textureHeight	= (float)texture->m_textureObject->m_hieghtBasis;
			//WARNING: C4244 --> possible loss of data; int to float

			textureName		= texture->List_textureName;
		}

		//Copy texture file
		if(textureName)
			pImage->m_textureFile = textureName;

		//Update mesh's texture coordinates
		pImage->m_fullTextureWidth	= textureWidth;
		pImage->m_fullTextureHeight	= textureHeight;
		pImage->UpdateTextureCoords();
	}

	//Update object coordinates
	m_pMenu->Resize();
}

/*void CAddMenu::OnBnClickedButtongetfreeid()
{
	UpdateData(TRUE);
    m_type = m_indMenuID;
	UpdateData(FALSE);
}

void CAddMenu::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_menuObjRef->m_menuName                = m_name;
	m_menuObjRef->m_specialOperation        = m_specialOperation;
	m_menuObjRef->m_specialOperationMiscInt = m_specialOperationMiscInt;
    m_menuObjRef->m_menuAccessID            = m_type;
    m_menuObjRef->m_instantClose            = m_instantClose.GetCheck();
	m_menuObjRef->m_lockControls            = m_lockControls.GetCheck();
	m_menuObjRef->m_closeOnEsc              = m_closeOnESC.GetCheck();
	m_menuObjRef->m_triggerSoundtrack       = m_soundTrackDB.GetCurSel();
	m_menuObjRef->m_offsetMenu.x            = m_offsetX;
	m_menuObjRef->m_offsetMenu.y            = m_offsetY;
	m_menuObjRef->m_offsetMenu.z            = m_offsetZ;
	m_menuObjRef->m_scale                   = m_scale;
	if(m_menuObjRef->m_triggerSoundtrack < 0)
	   m_menuObjRef->m_triggerSoundtrack = 0;
    
	
	m_menuObjRef->m_resolutionBased = m_resolutionBased.GetCheck();
    m_menuObjRef->m_centerMenu      = m_centerMouseOption.GetCheck();
	m_menuObjRef->m_restrictChat    = m_restrictChat.GetCheck();


    UpdateData(FALSE);
	
	CDialog::OnOK();
}

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CAddMenu::ConvToString(int number)
{    
              if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
		
			  return temp;
}

void CAddMenu::InListMenuMerges(int sel)
{
   m_menuMergeList.ResetContent();
   if(!m_menuObjRef->m_menuMergeList)
	   return;
   for(POSITION posLoc = m_menuObjRef->m_menuMergeList->GetHeadPosition();posLoc != NULL;)
   {
       CMenuMergeObj * mObj = (CMenuMergeObj *) m_menuObjRef->m_menuMergeList->GetNext(posLoc);
 
	   m_menuMergeList.AddString(ConvToString(mObj->m_menuID));
	   

   }
   if(sel > -1)
	  m_menuMergeList.SetCurSel(sel);
      
}

void CAddMenu::OnBnClickedButtonAddmenumerge()
{
	
	if(!m_menuObjRef->m_menuMergeList)
		m_menuObjRef->m_menuMergeList = new CMenuMergeList();

	    CMenuMergeObj * mObj = new CMenuMergeObj();
		UpdateData(TRUE);
		  mObj->m_menuID = m_menuMergeID;

          m_menuObjRef->m_menuMergeList->AddTail(mObj);
		UpdateData(FALSE);

		InListMenuMerges(m_menuObjRef->m_menuMergeList->GetCount());
}

void CAddMenu::OnBnClickedButtondeletemenumerge()
{
	int sel = m_menuMergeList.GetCurSel();
	if(sel == -1)
	{ 
       AfxMessageBox("You must select a menu merge object");
       return;
	}
	if(!m_menuObjRef->m_menuMergeList)
		return;

	POSITION posLoc = m_menuObjRef->m_menuMergeList->FindIndex(sel);
	if(!posLoc)
		return;
	    CMenuMergeObj * mObj = (CMenuMergeObj *) m_menuObjRef->m_menuMergeList->GetAt(posLoc);
        mObj->SafeDelete();
		delete mObj;
		mObj = 0;
		m_menuObjRef->m_menuMergeList->RemoveAt(posLoc);
	
		InListMenuMerges(sel-1);
}

void CAddMenu::OnBnClickedCheckCentermouseoption()
{
}

void CAddMenu::OnBnClickedCheckRestrictchat()
{
	
}*/


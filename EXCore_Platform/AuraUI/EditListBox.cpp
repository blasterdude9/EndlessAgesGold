// C:\Projects\EXCore_Platform\AuraUI\EditListBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditListBox.h"
#include "EditScript.h"

// CEditListBox dialog
IMPLEMENT_DYNAMIC(CEditListBox, CDialog)

CEditListBox::CEditListBox(CWnd* pParent /*=NULL*/)
	: CDialog(CEditListBox::IDD, pParent)
{
	m_pListBox			= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
	m_freezeInput		= NULL;
	m_scrollBarDlg		= NULL;
}

BEGIN_MESSAGE_MAP(CEditListBox, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Properties
	ON_CBN_SELCHANGE(IDC_COMBO_Font, OnChangeFont)
	ON_EN_CHANGE(IDC_EDIT_ColorRed, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorGreen, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorBlue, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorAlpha, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_SelectedRed, OnChangeSelectedColor)
	ON_EN_CHANGE(IDC_EDIT_SelectedGreen, OnChangeSelectedColor)
	ON_EN_CHANGE(IDC_EDIT_SelectedBlue, OnChangeSelectedColor)
	ON_EN_CHANGE(IDC_EDIT_SelectedAlpha, OnChangeSelectedColor)
	ON_EN_CHANGE(IDC_EDIT_SpecialRed, OnChangeSpecialColor)
	ON_EN_CHANGE(IDC_EDIT_SpecialGreen, OnChangeSpecialColor)
	ON_EN_CHANGE(IDC_EDIT_SpecialBlue, OnChangeSpecialColor)
	ON_EN_CHANGE(IDC_EDIT_SpecialAlpha, OnChangeSpecialColor)
	ON_EN_CHANGE(IDC_EDIT_TextInset, OnChangeTextInset)
	ON_EN_CHANGE(IDC_EDIT_NumberOfColumns, OnChangeNumberOfColumns)
	ON_EN_CHANGE(IDC_EDIT_ColumnSpacing, OnChangeColumnSpacing)
	ON_EN_CHANGE(IDC_EDIT_NumTextObjectsPerItem, OnChangeNumTextObjectsPerItem)
	ON_EN_CHANGE(IDC_EDIT_ScrollBarInset, OnChangeScrollBarInset)
	ON_EN_CHANGE(IDC_EDIT_ExtraSpacingBetweenText, OnChangeExtraSpacingBetweenText)
	ON_EN_CHANGE(IDC_EDIT_ExtraSpacingBetweenItems, OnChangeExtraSpacingBetweenItems)
	ON_BN_CLICKED(IDC_CHECK_AlwaysShowScrollBar, OnToggleAlwaysShowScrollBar)
	ON_BN_CLICKED(IDC_CHECK_DraggableItems, OnToggleDraggableItems)
	ON_BN_CLICKED(IDC_BUTTON_EditScrollBar, OnButtonEditScrollBar)

	//List Box Coordinates
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)

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

	//Item Background Texture Edit
	ON_EN_CHANGE(IDC_EDIT_ItemBackgroundTextureX, OnChangeItemBackgroundTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ItemBackgroundTextureY, OnChangeItemBackgroundTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ItemBackgroundTextureWidth, OnChangeItemBackgroundTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ItemBackgroundTextureHeight, OnChangeItemBackgroundTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditMenuItemBackgroundTexture, OnButtonEditItemBackgroundTexture)

	//Corner Offsets
	ON_EN_CHANGE(IDC_EDIT_CornerOffsetX, OnChangeCornerOffsets)
	ON_EN_CHANGE(IDC_EDIT_CornerOffsetY, OnChangeCornerOffsets)

	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditSelectionChangeScript, OnButtonEditSelectionChangeScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOverScript, OnButtonEditMouseOverScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOutScript, OnButtonEditMouseOutScript)
	ON_BN_CLICKED(IDC_BUTTON_EditDropScript, OnButtonEditDropScript)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_EditUpdateScript, &CEditListBox::OnBnClickedButtonEditupdatescript)
	END_MESSAGE_MAP()

BOOL CEditListBox::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Fill the font list
	for(POSITION posCurFont = m_gStatus->m_menuFontList->GetHeadPosition(); posCurFont != NULL;)
	{
		CMenuFont *curFont = (CMenuFont*)m_gStatus->m_menuFontList->GetNext(posCurFont);
		SendDlgItemMessage(IDC_COMBO_Font, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)curFont->m_name);
	}

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pListBox->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pListBox->m_visualID);

	//List Box Coordinates
	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pListBox->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pListBox->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, m_pListBox->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, m_pListBox->m_height);*/

	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)m_pListBox->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)m_pListBox->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)m_pListBox->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)m_pListBox->m_height);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Set Current Font
	SendDlgItemMessage(IDC_COMBO_Font, CB_SETCURSEL, m_gStatus->m_menuFontList->GetIndexFromPointer(m_pListBox->m_font));

	//This must be done because GetIndexFromPointer defaultly returns 0 to select the first font in the list.
	// This means m_font could still be NULL, so we must make sure it's set
	if(!m_gStatus->m_menuFontList->IsEmpty())
		m_pListBox->m_font = m_gStatus->m_menuFontList->GetPointerFromIndex(m_gStatus->m_menuFontList->GetIndexFromPointer(m_pListBox->m_font));

	//Colors
	/*SetDlgItemInt(IDC_EDIT_ColorRed, m_pListBox->m_normalRed * 255.0f);
	SetDlgItemInt(IDC_EDIT_ColorGreen, m_pListBox->m_normalGreen * 255.0f);
	SetDlgItemInt(IDC_EDIT_ColorBlue, m_pListBox->m_normalBlue * 255.0f);
	SetDlgItemInt(IDC_EDIT_ColorAlpha, m_pListBox->m_normalAlpha * 255.0f);
	SetDlgItemInt(IDC_EDIT_SelectedRed, m_pListBox->m_selectedRed * 255.0f);
	SetDlgItemInt(IDC_EDIT_SelectedGreen, m_pListBox->m_selectedGreen * 255.0f);
	SetDlgItemInt(IDC_EDIT_SelectedBlue, m_pListBox->m_selectedBlue * 255.0f);
	SetDlgItemInt(IDC_EDIT_SelectedAlpha, m_pListBox->m_selectedAlpha * 255.0f);
	SetDlgItemInt(IDC_EDIT_SpecialRed, m_pListBox->m_specialRed * 255.0f);
	SetDlgItemInt(IDC_EDIT_SpecialGreen, m_pListBox->m_specialGreen * 255.0f);
	SetDlgItemInt(IDC_EDIT_SpecialBlue, m_pListBox->m_specialBlue * 255.0f);
	SetDlgItemInt(IDC_EDIT_SpecialAlpha, m_pListBox->m_specialAlpha * 255.0f);*/

	SetDlgItemInt(IDC_EDIT_ColorRed, (UINT)(m_pListBox->m_normalRed * 255.0f));
	SetDlgItemInt(IDC_EDIT_ColorGreen, (UINT)(m_pListBox->m_normalGreen * 255.0f));
	SetDlgItemInt(IDC_EDIT_ColorBlue, (UINT)(m_pListBox->m_normalBlue * 255.0f));
	SetDlgItemInt(IDC_EDIT_ColorAlpha, (UINT)(m_pListBox->m_normalAlpha * 255.0f));
	SetDlgItemInt(IDC_EDIT_SelectedRed, (UINT)(m_pListBox->m_selectedRed * 255.0f));
	SetDlgItemInt(IDC_EDIT_SelectedGreen, (UINT)(m_pListBox->m_selectedGreen * 255.0f));
	SetDlgItemInt(IDC_EDIT_SelectedBlue, (UINT)(m_pListBox->m_selectedBlue * 255.0f));
	SetDlgItemInt(IDC_EDIT_SelectedAlpha, (UINT)(m_pListBox->m_selectedAlpha * 255.0f));
	SetDlgItemInt(IDC_EDIT_SpecialRed, (UINT)(m_pListBox->m_specialRed * 255.0f));
	SetDlgItemInt(IDC_EDIT_SpecialGreen, (UINT)(m_pListBox->m_specialGreen * 255.0f));
	SetDlgItemInt(IDC_EDIT_SpecialBlue, (UINT)(m_pListBox->m_specialBlue * 255.0f));
	SetDlgItemInt(IDC_EDIT_SpecialAlpha, (UINT)(m_pListBox->m_specialAlpha * 255.0f));

	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Properties
	/*SetDlgItemInt(IDC_EDIT_TextInset, m_pListBox->m_itemInset);*/
	SetDlgItemInt(IDC_EDIT_TextInset, (UINT)m_pListBox->m_itemInset);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	SetDlgItemInt(IDC_EDIT_NumberOfColumns, m_pListBox->m_numColumns);

	/*SetDlgItemInt(IDC_EDIT_ColumnSpacing, m_pListBox->m_columnSpacing);*/
	SetDlgItemInt(IDC_EDIT_ColumnSpacing, (UINT)m_pListBox->m_columnSpacing);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	SetDlgItemInt(IDC_EDIT_NumTextObjectsPerItem, m_pListBox->m_numTextObjectsPerItem);

	/*SetDlgItemInt(IDC_EDIT_ScrollBarInset, m_pListBox->m_scrollBarInset);
	SetDlgItemInt(IDC_EDIT_ExtraSpacingBetweenText, m_pListBox->m_extraSpacingBetweenText);
	SetDlgItemInt(IDC_EDIT_ExtraSpacingBetweenItems, m_pListBox->m_extraSpacingBetweenItems);*/
	
	SetDlgItemInt(IDC_EDIT_ScrollBarInset, (UINT)m_pListBox->m_scrollBarInset);
	SetDlgItemInt(IDC_EDIT_ExtraSpacingBetweenText, (UINT)m_pListBox->m_extraSpacingBetweenText);
	SetDlgItemInt(IDC_EDIT_ExtraSpacingBetweenItems,(UINT)m_pListBox->m_extraSpacingBetweenItems);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	

	//Background Texture Coordinates
	if(m_pListBox->m_background)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundTextureX, m_pListBox->m_background->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureY, m_pListBox->m_background->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureWidth, m_pListBox->m_background->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureHeight, m_pListBox->m_background->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BackgroundTextureX, (UINT)m_pListBox->m_background->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureY, (UINT)m_pListBox->m_background->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureWidth, (UINT)m_pListBox->m_background->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundTextureHeight, (UINT)m_pListBox->m_background->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Inset Setting
	/*SetDlgItemInt(IDC_EDIT_BackgroundInset, m_pListBox->m_backgroundInset);*/
	SetDlgItemInt(IDC_EDIT_BackgroundInset, (UINT)m_pListBox->m_backgroundInset);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Top Border Texture Coordinates
	if(m_pListBox->m_borderTop)
	{
		/*SetDlgItemInt(IDC_EDIT_TopBorderTextureX, m_pListBox->m_borderTop->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureY, m_pListBox->m_borderTop->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureWidth, m_pListBox->m_borderTop->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureHeight, m_pListBox->m_borderTop->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_TopBorderTextureX, (UINT)m_pListBox->m_borderTop->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureY, (UINT)m_pListBox->m_borderTop->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureWidth, (UINT)m_pListBox->m_borderTop->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopBorderTextureHeight, (UINT)m_pListBox->m_borderTop->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Left Border Texture Coordinates
	if(m_pListBox->m_borderLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_LeftBorderTextureX, m_pListBox->m_borderLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureY, m_pListBox->m_borderLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureWidth, m_pListBox->m_borderLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureHeight, m_pListBox->m_borderLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_LeftBorderTextureX, (UINT)m_pListBox->m_borderLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureY, (UINT)m_pListBox->m_borderLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureWidth, (UINT)m_pListBox->m_borderLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftBorderTextureHeight, (UINT)m_pListBox->m_borderLeft->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Bottom Border Texture Coordinates
	if(m_pListBox->m_borderBottom)
	{
		/*SetDlgItemInt(IDC_EDIT_BottomBorderTextureX, m_pListBox->m_borderBottom->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureY, m_pListBox->m_borderBottom->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureWidth, m_pListBox->m_borderBottom->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureHeight, m_pListBox->m_borderBottom->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BottomBorderTextureX, (UINT)m_pListBox->m_borderBottom->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureY, (UINT)m_pListBox->m_borderBottom->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureWidth, (UINT)m_pListBox->m_borderBottom->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomBorderTextureHeight, (UINT)m_pListBox->m_borderBottom->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Right Border Texture Coordinates
	if(m_pListBox->m_borderRight)
	{
		/*SetDlgItemInt(IDC_EDIT_RightBorderTextureX, m_pListBox->m_borderRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureY, m_pListBox->m_borderRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureWidth, m_pListBox->m_borderRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureHeight, m_pListBox->m_borderRight->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_RightBorderTextureX, (UINT)m_pListBox->m_borderRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureY, (UINT)m_pListBox->m_borderRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureWidth, (UINT)m_pListBox->m_borderRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightBorderTextureHeight, (UINT)m_pListBox->m_borderRight->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Top Left Corner Texture Coordinates
	if(m_pListBox->m_cornerTopLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureX, m_pListBox->m_cornerTopLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureY, m_pListBox->m_cornerTopLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureWidth, m_pListBox->m_cornerTopLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureHeight, m_pListBox->m_cornerTopLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureX, (UINT)m_pListBox->m_cornerTopLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureY, (UINT)m_pListBox->m_cornerTopLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureWidth, (UINT)m_pListBox->m_cornerTopLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopLeftCornerTextureHeight, (UINT)m_pListBox->m_cornerTopLeft->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}
	//Top Right Corner Texture Coordinates
	if(m_pListBox->m_cornerTopRight)
	{
		/*SetDlgItemInt(IDC_EDIT_TopRightCornerTextureX, m_pListBox->m_cornerTopRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureY, m_pListBox->m_cornerTopRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureWidth, m_pListBox->m_cornerTopRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureHeight, m_pListBox->m_cornerTopRight->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureX, (UINT)m_pListBox->m_cornerTopRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureY, (UINT)m_pListBox->m_cornerTopRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureWidth, (UINT)m_pListBox->m_cornerTopRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_TopRightCornerTextureHeight, (UINT)m_pListBox->m_cornerTopRight->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Bottom Left Corner Texture Coordinates
	if(m_pListBox->m_cornerBottomLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureX, m_pListBox->m_cornerBottomLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureY, m_pListBox->m_cornerBottomLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureWidth, m_pListBox->m_cornerBottomLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureHeight, m_pListBox->m_cornerBottomLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureX, (UINT)m_pListBox->m_cornerBottomLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureY, (UINT)m_pListBox->m_cornerBottomLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureWidth, (UINT)m_pListBox->m_cornerBottomLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureHeight, (UINT)m_pListBox->m_cornerBottomLeft->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Bottom Right Corner Texture Coordinates
	if(m_pListBox->m_cornerBottomRight)
	{
		/*SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureX, m_pListBox->m_cornerBottomRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureY, m_pListBox->m_cornerBottomRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureWidth, m_pListBox->m_cornerBottomRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureHeight, m_pListBox->m_cornerBottomRight->m_textureHeight);*/


		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureX, (UINT)m_pListBox->m_cornerBottomRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureY, (UINT)m_pListBox->m_cornerBottomRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureWidth, (UINT)m_pListBox->m_cornerBottomRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BottomRightCornerTextureHeight, (UINT)m_pListBox->m_cornerBottomRight->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Bottom Right Corner Texture Coordinates
	if(m_pListBox->m_itemBackground)
	{
		/*SetDlgItemInt(IDC_EDIT_ItemBackgroundTextureX, m_pListBox->m_itemBackground->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_ItemBackgroundTextureY, m_pListBox->m_itemBackground->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_ItemBackgroundTextureWidth, m_pListBox->m_itemBackground->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_ItemBackgroundTextureHeight, m_pListBox->m_itemBackground->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_ItemBackgroundTextureX, (UINT)m_pListBox->m_itemBackground->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_ItemBackgroundTextureY, (UINT)m_pListBox->m_itemBackground->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_ItemBackgroundTextureWidth, (UINT)m_pListBox->m_itemBackground->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_ItemBackgroundTextureHeight, (UINT)m_pListBox->m_itemBackground->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Corner Offsets
	/*SetDlgItemInt(IDC_EDIT_BackgroundInset, m_pListBox->m_backgroundInset);
	SetDlgItemInt(IDC_EDIT_BackgroundInset, m_pListBox->m_backgroundInset);*/

	SetDlgItemInt(IDC_EDIT_BackgroundInset, (UINT)m_pListBox->m_backgroundInset);
	SetDlgItemInt(IDC_EDIT_BackgroundInset, (UINT)m_pListBox->m_backgroundInset);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Check Boxes
	CheckDlgButton(IDC_CHECK_Resizable, m_pListBox->m_scrollBarAlwaysVisible);
	CheckDlgButton(IDC_CHECK_DraggableItems, m_pListBox->m_dragEnabled);

	m_initialized = true;

	return TRUE;
}

void CEditListBox::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditListBox::OnChangeFont()
{
	if(!m_initialized)
		return;

	//Store the new fonts information
	m_pListBox->m_font		= m_gStatus->m_menuFontList ->GetPointerFromIndex(SendDlgItemMessage(IDC_COMBO_Font, CB_GETCURSEL));
	ASSERT(m_pListBox->m_font);
	if(m_pListBox->m_font)
	   m_pListBox->m_fontName	= m_pListBox->m_font->m_name;

	//Loop through the item list and change the font
	CMenuListItem *curItem = m_pListBox->m_pFirstItem;
	while(curItem)
	{
		//Loop through this item's text objects and set the font for each
		POSITION posText = curItem->m_textList.GetHeadPosition();
		while(posText != NULL)
		{
			CMenuText *curText = (CMenuText*)curItem->m_textList.GetNext(posText);
			curText->m_font = m_pListBox->m_font;
		}

		//Goto the next item in the list
		curItem = curItem->m_pNext;
	}
}

void CEditListBox::OnChangeColor()
{
	if(!m_initialized)
		return;

	m_pListBox->m_normalRed = (int)GetDlgItemInt(IDC_EDIT_ColorRed) / 255.0f;
	m_pListBox->m_normalGreen = (int)GetDlgItemInt(IDC_EDIT_ColorGreen) / 255.0f;
	m_pListBox->m_normalBlue = (int)GetDlgItemInt(IDC_EDIT_ColorBlue) / 255.0f;
	m_pListBox->m_normalAlpha = (int)GetDlgItemInt(IDC_EDIT_ColorAlpha) / 255.0f;

	//Loop through the item list and delete the text meshes
	CMenuListItem *curItem = m_pListBox->m_pFirstItem;
	while(curItem)
	{
		//Loop through this item's text objects and set the font for each
		POSITION posText = curItem->m_textList.GetHeadPosition();
		while(posText != NULL)
		{
			CMenuText *curText = (CMenuText*)curItem->m_textList.GetNext(posText);
			delete curText->m_textMesh;
			curText->m_textMesh = 0;
		}

		//Goto the next item in the list
		curItem = curItem->m_pNext;
	}
}

void CEditListBox::OnChangeSelectedColor()
{
	if(!m_initialized)
		return;

	m_pListBox->m_selectedRed = (int)GetDlgItemInt(IDC_EDIT_SelectedRed) / 255.0f;
	m_pListBox->m_selectedGreen = (int)GetDlgItemInt(IDC_EDIT_SelectedGreen) / 255.0f;
	m_pListBox->m_selectedBlue = (int)GetDlgItemInt(IDC_EDIT_SelectedBlue) / 255.0f;
	m_pListBox->m_selectedAlpha = (int)GetDlgItemInt(IDC_EDIT_SelectedAlpha) / 255.0f;
}

void CEditListBox::OnChangeSpecialColor()
{
	if(!m_initialized)
		return;

	m_pListBox->m_specialRed = (int)GetDlgItemInt(IDC_EDIT_SpecialRed) / 255.0f;
	m_pListBox->m_specialGreen = (int)GetDlgItemInt(IDC_EDIT_SpecialGreen) / 255.0f;
	m_pListBox->m_specialBlue = (int)GetDlgItemInt(IDC_EDIT_SpecialBlue) / 255.0f;
	m_pListBox->m_specialAlpha = (int)GetDlgItemInt(IDC_EDIT_SpecialAlpha) / 255.0f;
}

void CEditListBox::OnChangeTextInset()
{
	/*m_pListBox->m_itemInset = (int)GetDlgItemInt(IDC_EDIT_TextInset);*/

	m_pListBox->m_itemInset = (float)(int)GetDlgItemInt(IDC_EDIT_TextInset);
	m_pListBox->Resize();

	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnChangeColumnSpacing()
{
	/*m_pListBox->m_columnSpacing = (int)GetDlgItemInt(IDC_EDIT_ColumnSpacing);*/

	m_pListBox->m_columnSpacing = (float)(int)GetDlgItemInt(IDC_EDIT_ColumnSpacing);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnChangeNumberOfColumns()
{
	m_pListBox->m_numColumns = (int)GetDlgItemInt(IDC_EDIT_NumberOfColumns);
	m_pListBox->Resize();
}

void CEditListBox::OnChangeNumTextObjectsPerItem()
{
	m_pListBox->m_numTextObjectsPerItem = (int)GetDlgItemInt(IDC_EDIT_NumTextObjectsPerItem);
	m_pListBox->Resize();
}

void CEditListBox::OnChangeScrollBarInset()
{
	/*m_pListBox->m_scrollBarInset = (int)GetDlgItemInt(IDC_EDIT_ScrollBarInset);*/

	m_pListBox->m_scrollBarInset = (float)(int)GetDlgItemInt(IDC_EDIT_ScrollBarInset);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
	m_pListBox->Resize();
}

void CEditListBox::OnChangeExtraSpacingBetweenText()
{
	/*m_pListBox->m_extraSpacingBetweenText = (int)GetDlgItemInt(IDC_EDIT_ExtraSpacingBetweenText);*/
	
	m_pListBox->m_extraSpacingBetweenText = (float)(int)GetDlgItemInt(IDC_EDIT_ExtraSpacingBetweenText);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pListBox->Resize();
}

void CEditListBox::OnChangeExtraSpacingBetweenItems()
{
	/*m_pListBox->m_extraSpacingBetweenItems = (int)GetDlgItemInt(IDC_EDIT_ExtraSpacingBetweenItems);*/
	m_pListBox->m_extraSpacingBetweenItems = (float)(int)GetDlgItemInt(IDC_EDIT_ExtraSpacingBetweenItems);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pListBox->Resize();
}

void CEditListBox::OnToggleAlwaysShowScrollBar()
{
	m_pListBox->m_scrollBarAlwaysVisible = IsDlgButtonChecked(IDC_CHECK_AlwaysShowScrollBar);
}

void CEditListBox::OnToggleDraggableItems()
{
	m_pListBox->m_dragEnabled = IsDlgButtonChecked(IDC_CHECK_DraggableItems);
}

void CEditListBox::OnButtonEditScrollBar()
{
	//If the scroll bar hasn't been created yet
	if(!m_pListBox->m_scrollBar)
	{
		//Create a new scroll bar and set its visual id
		m_pListBox->m_scrollBar				= new CMenuScrollBar();
		m_pListBox->m_scrollBar->m_visualID	= 0;
	}

	//If the Scroll Bar Editing Dialog exists, delete it
	if(m_scrollBarDlg)
		delete m_scrollBarDlg;

	//Create a new dialog
	m_scrollBarDlg					= new CEditScrollBar();
	m_scrollBarDlg->m_pScrollBar	= m_pListBox->m_scrollBar;
	m_scrollBarDlg->m_gStatus		= m_gStatus;
	m_scrollBarDlg->m_freeVisualID	= 0;
	m_scrollBarDlg->m_freezeInput	= m_freezeInput;

	//Run it
	m_scrollBarDlg->Create(IDD_DIALOG_MenuSysEditScrollBar);

	//Disable the Visual Name/ID text boxes and the MouseOver/MouseOut script buttons
	CWnd *curItem = m_scrollBarDlg->GetDlgItem(IDC_EDIT_VisualName);
	curItem->EnableWindow(false);
	curItem = m_scrollBarDlg->GetDlgItem(IDC_BUTTON_getFreeID);
	curItem->EnableWindow(false);
	curItem = m_scrollBarDlg->GetDlgItem(IDC_EDIT_visualID);
	curItem->EnableWindow(false);
	curItem = m_scrollBarDlg->GetDlgItem(IDC_BUTTON_EditMouseOverScript);
	curItem->EnableWindow(false);
	curItem = m_scrollBarDlg->GetDlgItem(IDC_BUTTON_EditMouseOutScript);
	curItem->EnableWindow(false);
}

void CEditListBox::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pListBox->m_posX	 = (int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pListBox->m_posY	 = (int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pListBox->m_width	 = (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pListBox->m_height = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

	m_pListBox->m_posX	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pListBox->m_posY	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pListBox->m_width	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pListBox->m_height = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pListBox->Resize();
}

void CEditListBox::OnChangeBackgroundTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_background)
		m_pListBox->m_background = new CMenuImage();

	/*UpdateImageTextureCoords(m_pListBox->m_background,
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundTextureHeight));*/

	UpdateImageTextureCoords(m_pListBox->m_background,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundTextureHeight));

	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnButtonEditBackgroundTexture()
{
	if(!m_pListBox->m_background)
	{
		m_pListBox->m_background = new CMenuImage();
		return;
	}

	EditTexture(m_pListBox->m_background);
}

void CEditListBox::OnChangeBackgroundInset()
{
	/*m_pListBox->m_backgroundInset = (int)GetDlgItemInt(IDC_EDIT_BackgroundInset);*/

	m_pListBox->m_backgroundInset = (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundInset);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pListBox->Resize();
}

void CEditListBox::OnChangeTopBorderTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_borderTop)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pListBox->m_borderTop,
							 (int)GetDlgItemInt(IDC_EDIT_TopBorderTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_TopBorderTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_TopBorderTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_TopBorderTextureHeight));*/

	UpdateImageTextureCoords(m_pListBox->m_borderTop,
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopBorderTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopBorderTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopBorderTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopBorderTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnButtonEditTopBorderTexture()
{
	if(!m_pListBox->m_borderTop)
		return CreateBordersAndCorners();

	EditTexture(m_pListBox->m_borderTop);
}

void CEditListBox::OnChangeLeftBorderTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_borderLeft)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pListBox->m_borderLeft,
							 (int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureHeight));*/

	UpdateImageTextureCoords(m_pListBox->m_borderLeft,
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftBorderTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnButtonEditLeftBorderTexture()
{
	if(!m_pListBox->m_borderLeft)
		return CreateBordersAndCorners();

	EditTexture(m_pListBox->m_borderLeft);
}

void CEditListBox::OnChangeBottomBorderTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_borderBottom)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pListBox->m_borderBottom,
							 (int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureHeight));*/

	UpdateImageTextureCoords(m_pListBox->m_borderBottom,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomBorderTextureHeight));

	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnButtonEditBottomBorderTexture()
{
	if(!m_pListBox->m_borderBottom)
		return CreateBordersAndCorners();

	EditTexture(m_pListBox->m_borderBottom);
}

void CEditListBox::OnChangeRightBorderTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_borderRight)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pListBox->m_borderRight,
							 (int)GetDlgItemInt(IDC_EDIT_RightBorderTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_RightBorderTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_RightBorderTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_RightBorderTextureHeight));*/

	UpdateImageTextureCoords(m_pListBox->m_borderRight,
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightBorderTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightBorderTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightBorderTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightBorderTextureHeight));

	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnButtonEditRightBorderTexture()
{
	if(!m_pListBox->m_borderRight)
		return CreateBordersAndCorners();

	EditTexture(m_pListBox->m_borderRight);
}

void CEditListBox::OnChangeTopLeftCornerTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_cornerTopLeft)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pListBox->m_cornerTopLeft,
							 (int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureHeight));*/

	UpdateImageTextureCoords(m_pListBox->m_cornerTopLeft,
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopLeftCornerTextureHeight));

	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnButtonEditTopLeftCornerTexture()
{
	if(!m_pListBox->m_cornerTopLeft)
		return CreateBordersAndCorners();

	EditTexture(m_pListBox->m_cornerTopLeft);
}

void CEditListBox::OnChangeTopRightCornerTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_cornerTopRight)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pListBox->m_cornerTopRight,
							 (int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureHeight));*/

	UpdateImageTextureCoords(m_pListBox->m_cornerTopRight,
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_TopRightCornerTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnButtonEditTopRightCornerTexture()
{
	if(!m_pListBox->m_cornerTopRight)
		return CreateBordersAndCorners();

	EditTexture(m_pListBox->m_cornerTopRight);
}

void CEditListBox::OnChangeBottomLeftCornerTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_cornerBottomLeft)
		CreateBordersAndCorners();

	/*UpdateImageTextureCoords(m_pListBox->m_cornerBottomLeft,
							 (int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureHeight));*/
	UpdateImageTextureCoords(m_pListBox->m_cornerBottomLeft,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomLeftCornerTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnButtonEditBottomLeftCornerTexture()
{
	if(!m_pListBox->m_cornerBottomLeft)
		return CreateBordersAndCorners();

	EditTexture(m_pListBox->m_cornerBottomLeft);
}

void CEditListBox::OnChangeBottomRightCornerTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_cornerBottomRight)
		CreateBordersAndCorners();

	//UpdateImageTextureCoords(m_pListBox->m_cornerBottomRight,
	//						 (int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureX),
	//						 (int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureY),
	//						 (int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureWidth),
	//						 (int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureHeight));

		UpdateImageTextureCoords(m_pListBox->m_cornerBottomRight,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BottomRightCornerTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditListBox::OnButtonEditBottomRightCornerTexture()
{
	if(!m_pListBox->m_cornerBottomRight)
		return CreateBordersAndCorners();

	EditTexture(m_pListBox->m_cornerBottomRight);
}

void CEditListBox::OnChangeItemBackgroundTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pListBox->m_itemBackground)
		m_pListBox->m_itemBackground = new CMenuImage();

	/*m_pListBox->m_itemBackground->m_texturePosX		= (int)GetDlgItemInt(IDC_EDIT_ItemBackgroundTextureX);
	m_pListBox->m_itemBackground->m_texturePosY		= (int)GetDlgItemInt(IDC_EDIT_ItemBackgroundTextureY);
	m_pListBox->m_itemBackground->m_textureWidth	= m_pListBox->m_itemBackground->m_width  = (int)GetDlgItemInt(IDC_EDIT_ItemBackgroundTextureWidth);
	m_pListBox->m_itemBackground->m_textureHeight	= m_pListBox->m_itemBackground->m_height = (int)GetDlgItemInt(IDC_EDIT_ItemBackgroundTextureHeight);*/

	m_pListBox->m_itemBackground->m_texturePosX		= (float)(int)GetDlgItemInt(IDC_EDIT_ItemBackgroundTextureX);
	m_pListBox->m_itemBackground->m_texturePosY		= (float)(int)GetDlgItemInt(IDC_EDIT_ItemBackgroundTextureY);
	m_pListBox->m_itemBackground->m_textureWidth	= (float)m_pListBox->m_itemBackground->m_width  = (float)(int)GetDlgItemInt(IDC_EDIT_ItemBackgroundTextureWidth);
	m_pListBox->m_itemBackground->m_textureHeight	= (float)m_pListBox->m_itemBackground->m_height = (float)(int)GetDlgItemInt(IDC_EDIT_ItemBackgroundTextureHeight);

	//WARNING: C4244 --> conversion from int to float, possible loss of data

	UpdateImageTextureCoords(m_pListBox->m_itemBackground);
}

void CEditListBox::OnButtonEditItemBackgroundTexture()
{
	if(!m_pListBox->m_itemBackground)
	{
		m_pListBox->m_itemBackground = new CMenuImage();
		return;
	}

	if(!m_pListBox->m_itemBackground->m_imageMesh)
		return;

	EditTexture(m_pListBox->m_itemBackground);
}
	
void CEditListBox::OnChangeCornerOffsets()
{
	if(!m_initialized)
		return;

	//m_pListBox->m_cornerOffsetX = (int)GetDlgItemInt(IDC_EDIT_CornerOffsetX);
	//m_pListBox->m_cornerOffsetY = (int)GetDlgItemInt(IDC_EDIT_CornerOffsetY);

	m_pListBox->m_cornerOffsetX = (float)(int)GetDlgItemInt(IDC_EDIT_CornerOffsetX);
	m_pListBox->m_cornerOffsetY = (float)(int)GetDlgItemInt(IDC_EDIT_CornerOffsetY);

	//WARNING: C4244 --> conversion from int to float, possible loss of data
	m_pListBox->Resize();
}

void CEditListBox::OnButtonEditSelectionChangeScript()
{
	if(!m_pListBox->m_selectionChangeScript)
	{
		m_pListBox->m_selectionChangeScript					 = new CMenuScript();
		m_pListBox->m_selectionChangeScript->m_callingVisual = (CMenuVisual*)m_pListBox;
	}

	CEditScript dlg;
	dlg.m_script = m_pListBox->m_selectionChangeScript;
	dlg.DoModal();
}

void CEditListBox::OnButtonEditMouseOverScript()
{
	/*CMenuListItem *curItem	= new CMenuListItem();
	CMenuText *text			= new CMenuText();
	text->m_text			= "640 x 480 x 16";
	curItem->m_textList.AddTail(text);
	m_pListBox->AddItem(curItem);

	curItem		 = new CMenuListItem();
	text		 = new CMenuText();
	text->m_text = "640 x 480 x 32";
	curItem->m_textList.AddTail(text);
	m_pListBox->AddItem(curItem);

	curItem		 = new CMenuListItem();
	text		 = new CMenuText();
	text->m_text = "800 x 600 x 16";
	curItem->m_textList.AddTail(text);
	m_pListBox->AddItem(curItem);

	curItem		 = new CMenuListItem();
	text		 = new CMenuText();
	text->m_text = "800 x 600 x 32";
	curItem->m_textList.AddTail(text);
	m_pListBox->AddItem(curItem);

	curItem		 = new CMenuListItem();
	text		 = new CMenuText();
	text->m_text = "1024 x 768 x 16";
	curItem->m_textList.AddTail(text);
	m_pListBox->AddItem(curItem);

	curItem		 = new CMenuListItem();
	text		 = new CMenuText();
	text->m_text = "1024 x 768 x 32";
	curItem->m_textList.AddTail(text);
	m_pListBox->AddItem(curItem);

	curItem		 = new CMenuListItem();
	text		 = new CMenuText();
	text->m_text = "1280 x 1024 x 16";
	curItem->m_textList.AddTail(text);
	m_pListBox->AddItem(curItem);

	curItem		 = new CMenuListItem();
	text		 = new CMenuText();
	text->m_text = "1280 x 1024 x 32";
	curItem->m_textList.AddTail(text);
	m_pListBox->AddItem(curItem);

	if(!m_pListBox->m_mouseOverScript)
	{
		m_pListBox->m_mouseOverScript			 = new CMenuScript();
	}

	CEditScript dlg;
	dlg.m_script = m_pListBox->m_mouseOverScript;
	dlg.DoModal();*/
}

void CEditListBox::OnButtonEditMouseOutScript()
{
	/*
	CMenuListItem *pItem = new CMenuListItem();
	CMenuText *pPlayer = new CMenuText();
	CMenuText *pHealth = new CMenuText();
	CMenuImage *pIcon = new CMenuImage();

	// Clear the image from the hot slot
	pIcon->m_texturePosX = 854.0f;
	pIcon->m_texturePosY = 43.0f;
	pIcon->m_textureWidth = 24.0f;
	pIcon->m_textureHeight = 24.0f;
	pIcon->m_blendMode = 1;
	pIcon->m_bDraggable = false;
	pIcon->Init(m_gStatus->m_3dDevice);
	pIcon->UpdateTexture(pIcon->m_textureSelect);
	pIcon->UpdateTextureCoords();

	pPlayer->SetText("01. Frogcore");
	pHealth->SetText("100%");

	pItem->m_textList.AddHead(pHealth);
	pItem->m_textList.AddHead(pPlayer);
	pItem->m_icon = pIcon;

	m_pListBox->AddItem(pItem);
	*/

	/*curIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetVisualByID(501, false);
	curItem			= new CMenuListItem();
	text			= new CMenuText();
	text->m_text	= "(AP) Dev Chest";
	curItem->m_textList.AddTail(text);
	text			= new CMenuText();
	text->m_text	= "  Quantity: 1";
	curItem->m_textList.AddTail(text);
	curItem->m_icon				= curIcon;
	curItem->m_miscInt1 = 3;
	curItem->m_miscInt3 = 4462;
	curItem->m_miscInt4 = 1;
	m_pListBox->AddItem(curItem);

	curIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetVisualByID(502, false);
	curItem			= new CMenuListItem();
	text			= new CMenuText();
	text->m_text	= "Orb of Ascherion";
	curItem->m_textList.AddTail(text);
	text			= new CMenuText();
	text->m_text	= "  Quantity: 7";
	curItem->m_textList.AddTail(text);
	curItem->m_icon				= curIcon;
	curItem->m_miscInt3 = 5;
	curItem->m_miscInt4 = 7;
	m_pListBox->AddItem(curItem);

	curIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetVisualByID(503, false);
	curItem			= new CMenuListItem();
	text			= new CMenuText();
	text->m_text	= "Ava Egg";
	curItem->m_textList.AddTail(text);
	text			= new CMenuText();
	text->m_text	= "  Quantity: 21";
	curItem->m_textList.AddTail(text);
	curItem->m_icon				= curIcon;
	curItem->m_miscInt3 = 6;
	curItem->m_miscInt4 = 21;
	m_pListBox->AddItem(curItem);

	curIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetVisualByID(504, false);
	curItem			= new CMenuListItem();
	text			= new CMenuText();
	text->m_text	= "Kandra Armor";
	curItem->m_textList.AddTail(text);
	text			= new CMenuText();
	text->m_text	= "  Quantity: 3";
	curItem->m_textList.AddTail(text);
	curItem->m_icon				= curIcon;
	curItem->m_miscInt3 = 7;
	curItem->m_miscInt4 = 3;
	m_pListBox->AddItem(curItem);

	curIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetVisualByID(505, false);
	curItem			= new CMenuListItem();
	text			= new CMenuText();
	text->m_text	= "(AP)GM JetPack";
	curItem->m_textList.AddTail(text);
	text			= new CMenuText();
	text->m_text	= "  Quantity: 1";
	curItem->m_textList.AddTail(text);
	curItem->m_icon				= curIcon;
	curItem->m_miscInt3 = 8;
	curItem->m_miscInt4 = 1;
	m_pListBox->AddItem(curItem);

	curIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetVisualByID(506, false);
	curItem			= new CMenuListItem();
	text			= new CMenuText();
	text->m_text	= "(AP)Majee Helm";
	curItem->m_textList.AddTail(text);
	text			= new CMenuText();
	text->m_text	= "  Quantity: 12";
	curItem->m_textList.AddTail(text);
	curItem->m_icon				= curIcon;
	curItem->m_miscInt3 = 9;
	curItem->m_miscInt4 = 12;
	m_pListBox->AddItem(curItem);

	curIcon = (CMenuImage*)m_gStatus->m_menuIconDatabase->GetVisualByID(507, false);
	curItem			= new CMenuListItem();
	text			= new CMenuText();
	text->m_text	= "(GM)Quadoty";
	curItem->m_textList.AddTail(text);
	text			= new CMenuText();
	text->m_text	= "  Quantity: 45";
	curItem->m_textList.AddTail(text);
	curItem->m_icon				= curIcon;
	curItem->m_miscInt3 = 10;
	curItem->m_miscInt4 = 45;
	m_pListBox->AddItem(curItem);*/

	/*if(!m_pListBox->m_mouseOutScript)
	{
		m_pListBox->m_mouseOutScript			= new CMenuScript();
	}

	CEditScript dlg;
	dlg.m_script = m_pListBox->m_mouseOutScript;
	dlg.DoModal();*/

	CMenuListItem *curItem_01 = new CMenuListItem();
	CMenuText *text_01 = new CMenuText();
	text_01->m_text	= "(0)CroKr Death Match (No Restrictions)";
	curItem_01->AddText(text_01, m_pListBox->m_width);
	m_pListBox->AddItem(curItem_01);

	CMenuListItem *curItem_02 = new CMenuListItem();
	CMenuText *text_02	= new CMenuText();
	text_02->m_text	= "(1)CroKr Domination Match No Restrictions)";
	curItem_02->AddText(text_02, m_pListBox->m_width);
	m_pListBox->AddItem(curItem_02);

	CMenuListItem *curItem_03 = new CMenuListItem();
	CMenuText *text_03	= new CMenuText();
	text_03->m_text	= "(2)CroKr Death Match Arena Guns Only";
	curItem_03->AddText(text_03, m_pListBox->m_width);
	m_pListBox->AddItem(curItem_03);

	CMenuListItem *curItem_04 = new CMenuListItem();
	CMenuText *text_04	= new CMenuText();
	text_04->m_text	= "(3)CroKr Team Death Match Arena Guns Only";
	curItem_04->AddText(text_04, m_pListBox->m_width);
	m_pListBox->AddItem(curItem_04);

	CMenuListItem *curItem_05 = new CMenuListItem();
	CMenuText *text_05	= new CMenuText();
	text_05->m_text	= "(5)CroKr Team Domination Arena Guns Only";
	curItem_05->AddText(text_05, m_pListBox->m_width);
	m_pListBox->AddItem(curItem_05);

	CMenuListItem *curItem_06 = new CMenuListItem();
	CMenuText *text_06	= new CMenuText();
	text_06->m_text	= "(7)Combiine Team Death Match Arena Guns Only";
	curItem_06->AddText(text_06, m_pListBox->m_width);
	m_pListBox->AddItem(curItem_06);

	CMenuListItem *curItem_07 = new CMenuListItem();
	CMenuText *text_07	= new CMenuText();
	text_07->m_text	= "(8)Combiine Domination Arena Guns Only";
	curItem_07->AddText(text_07, m_pListBox->m_width);
	m_pListBox->AddItem(curItem_07);

	CMenuListItem *curItem_08 = new CMenuListItem();
	CMenuText *text_08	= new CMenuText();
	text_08->m_text	= "(9)Combiine Team Domination Arena Guns Only";
	curItem_08->AddText(text_08, m_pListBox->m_width);
	m_pListBox->AddItem(curItem_08);

	CMenuListItem *curItem_09 = new CMenuListItem();
	CMenuText *text_09	= new CMenuText();
	text_09->m_text	= "Test";
	curItem_08->AddText(text_08, m_pListBox->m_width);
	m_pListBox->AddItem(curItem_08);
}

void CEditListBox::OnButtonEditDropScript()
{
	if(!m_pListBox->m_dropScript)
	{
		m_pListBox->m_dropScript			= new CMenuScript();
	}

	CEditScript dlg;
	dlg.m_script = m_pListBox->m_dropScript;
	dlg.DoModal();
}

void CEditListBox::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pListBox->m_name = temp;

	m_pListBox->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);

	CDialog::OnOK();
}

void CEditListBox::CreateBordersAndCorners()
{
	m_pListBox->m_borderLeft		= new CMenuImage();
	m_pListBox->m_borderTop			= new CMenuImage();
	m_pListBox->m_borderRight		= new CMenuImage();
	m_pListBox->m_borderBottom		= new CMenuImage();
	m_pListBox->m_cornerTopLeft		= new CMenuImage();
	m_pListBox->m_cornerTopRight	= new CMenuImage();
	m_pListBox->m_cornerBottomLeft	= new CMenuImage();
	m_pListBox->m_cornerBottomRight	= new CMenuImage();
}

void CEditListBox::EditTexture(CMenuImage *pImage)
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

void CEditListBox::UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, float Height)
{
	pImage->m_texturePosX	= X;
	pImage->m_texturePosY	= Y;
	pImage->m_textureWidth	= Width;
	pImage->m_textureHeight	= Height;

	UpdateImageTextureCoords(pImage);
}

void CEditListBox::UpdateImageTextureCoords(CMenuImage *pImage)
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

			/*textureWidth = texture->m_textureObject->m_widthBasis;
			textureHeight = texture->m_textureObject->m_hieghtBasis;*/
			textureWidth = (float)texture->m_textureObject->m_widthBasis;
			textureHeight = (float)texture->m_textureObject->m_hieghtBasis;
			//WARNING: C4244 --> conversion from DWORD to float, possible loss of data

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
	m_pListBox->Resize();
}

// CEditListBox message handlers

void CEditListBox::OnBnClickedButtonEditupdatescript()
{
	if(!m_pListBox->m_updateScript)
		m_pListBox->m_updateScript			= new CMenuScript();

	CEditScript dlg;
	dlg.m_script = m_pListBox->m_updateScript;
	dlg.DoModal();
}

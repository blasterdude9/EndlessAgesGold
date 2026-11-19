// C:\Projects\EXCore_Platform\AuraUI\EditTextBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditTextBox.h"
#include "EditScript.h"

// CEditTextBox dialog
IMPLEMENT_DYNAMIC(CEditTextBox, CDialog)

CEditTextBox::CEditTextBox(CWnd* pParent /*=NULL*/)
	: CDialog(CEditTextBox::IDD, pParent)
{
	m_pTextBox			= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
	m_freezeInput		= NULL;
	m_scrollBarDlg		= NULL;
}

BEGIN_MESSAGE_MAP(CEditTextBox, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Properties
	ON_EN_CHANGE(IDC_EDIT_Text, OnChangeText)
	ON_CBN_SELCHANGE(IDC_COMBO_Font, OnChangeFont)
	ON_CBN_SELCHANGE(IDC_COMBO_Alignment, OnChangeAlignment)
	ON_EN_CHANGE(IDC_EDIT_ColorRed, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorGreen, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorBlue, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorAlpha, OnChangeColor)
	ON_BN_CLICKED(IDC_CHECK_Multilined, OnToggleMultilined)
	ON_BN_CLICKED(IDC_CHECK_Editable, OnToggleEditable)
	ON_BN_CLICKED(IDC_CHECK_NumericOnly, OnToggleNumericOnly)
	ON_BN_CLICKED(IDC_CHECK_Password, OnTogglePassword)
	ON_EN_CHANGE(IDC_EDIT_MaxLength, OnChangeMaxLength)
	ON_EN_CHANGE(IDC_EDIT_MaxLines, OnChangeMaxLines)
	ON_BN_CLICKED(IDC_BUTTON_EditScrollBar, OnButtonEditScrollBar)

	//Button Coordinates
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)

	//Background Left Texture
	ON_EN_CHANGE(IDC_EDIT_BackgroundLeftTextureX, OnChangeBackgroundLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundLeftTextureY, OnChangeBackgroundLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundLeftTextureWidth, OnChangeBackgroundLeftTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditBackgroundLeftTexture, OnButtonEditBackgroundLeftTexture)

	//Background Middle Texture
	ON_EN_CHANGE(IDC_EDIT_BackgroundMiddleTextureX, OnChangeBackgroundMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundMiddleTextureY, OnChangeBackgroundMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundMiddleTextureWidth, OnChangeBackgroundMiddleTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditBackgroundMiddleTexture, OnButtonEditBackgroundMiddleTexture)

	//Background Right Texture
	ON_EN_CHANGE(IDC_EDIT_BackgroundRightTextureX, OnChangeBackgroundRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundRightTextureY, OnChangeBackgroundRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundRightTextureWidth, OnChangeBackgroundRightTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditBackgroundRightTexture, OnButtonEditBackgroundRightTexture)

	//Cursor Texture
	ON_EN_CHANGE(IDC_EDIT_CursorTextureX, OnChangeCursorCoords)
	ON_EN_CHANGE(IDC_EDIT_CursorTextureY, OnChangeCursorCoords)
	ON_EN_CHANGE(IDC_EDIT_CursorWidth, OnChangeCursorCoords)
	ON_EN_CHANGE(IDC_EDIT_CursorHeight, OnChangeCursorCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditCursorTexture, OnButtonEditCursorTexture)

	//Text Inset
	ON_EN_CHANGE(IDC_EDIT_TextInset, OnChangeTextInset)

	//System Read Out Toggle
	ON_BN_CLICKED(IDC_CHECK_SystemReadout, OnToggleSystemReadout)

	//SystemReadOutColors: Clan
	ON_EN_CHANGE(IDC_EDIT_ClanColorRed, OnChangeClanColor)
	ON_EN_CHANGE(IDC_EDIT_ClanColorGreen, OnChangeClanColor)
	ON_EN_CHANGE(IDC_EDIT_ClanColorBlue, OnChangeClanColor)
	ON_EN_CHANGE(IDC_EDIT_ClanColorAlpha, OnChangeClanColor)

	//SystemReadOutColors: Group
	ON_EN_CHANGE(IDC_EDIT_GroupColorRed, OnChangeGroupColor)
	ON_EN_CHANGE(IDC_EDIT_GroupColorGreen, OnChangeGroupColor)
	ON_EN_CHANGE(IDC_EDIT_GroupColorBlue, OnChangeGroupColor)
	ON_EN_CHANGE(IDC_EDIT_GroupColorAlpha, OnChangeGroupColor)

	//SystemReadOutColors: Server
	ON_EN_CHANGE(IDC_EDIT_ServerColorRed, OnChangeServerColor)
	ON_EN_CHANGE(IDC_EDIT_ServerColorGreen, OnChangeServerColor)
	ON_EN_CHANGE(IDC_EDIT_ServerColorBlue, OnChangeServerColor)
	ON_EN_CHANGE(IDC_EDIT_ServerColorAlpha, OnChangeServerColor)

	//SystemReadOutColors: Damage
	ON_EN_CHANGE(IDC_EDIT_DamageColorRed, OnChangeDamageColor)
	ON_EN_CHANGE(IDC_EDIT_DamageColorGreen, OnChangeDamageColor)
	ON_EN_CHANGE(IDC_EDIT_DamageColorBlue, OnChangeDamageColor)
	ON_EN_CHANGE(IDC_EDIT_DamageColorAlpha, OnChangeDamageColor)

	//SystemReadOutColors: Heal
	ON_EN_CHANGE(IDC_EDIT_HealColorRed, OnChangeHealColor)
	ON_EN_CHANGE(IDC_EDIT_HealColorGreen, OnChangeHealColor)
	ON_EN_CHANGE(IDC_EDIT_HealColorBlue, OnChangeHealColor)
	ON_EN_CHANGE(IDC_EDIT_HealColorAlpha, OnChangeHealColor)

	//SystemReadOutColors: Music
	ON_EN_CHANGE(IDC_EDIT_MusicColorRed, OnChangeMusicColor)
	ON_EN_CHANGE(IDC_EDIT_MusicColorGreen, OnChangeMusicColor)
	ON_EN_CHANGE(IDC_EDIT_MusicColorBlue, OnChangeMusicColor)
	ON_EN_CHANGE(IDC_EDIT_MusicColorAlpha, OnChangeMusicColor)

	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditEnterPressScript, OnButtonEditEnterPressScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOverScript, OnButtonEditMouseOverScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOutScript, OnButtonEditMouseOutScript)

	//Regular Buttons
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditTextBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Fill the font list
	for(POSITION posCurFont = m_gStatus->m_menuFontList->GetHeadPosition(); posCurFont != NULL;)
	{
		CMenuFont *curFont = (CMenuFont*)m_gStatus->m_menuFontList->GetNext(posCurFont);
		SendDlgItemMessage(IDC_COMBO_Font, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)curFont->m_name);
	}

	//Fill the alignment list
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Left");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Center");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Right");

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pTextBox->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pTextBox->m_visualID);

	//Text Properties
	SendDlgItemMessage(IDC_COMBO_Font, CB_SETCURSEL, m_gStatus->m_menuFontList->GetIndexFromPointer(m_pTextBox->m_font));
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, m_pTextBox->m_horAlignment);
	
	/*SetDlgItemInt(IDC_EDIT_ColorRed, m_pTextBox->m_red[MENU_TEXTCOLOR_NORMAL] * 255.0f);
	SetDlgItemInt(IDC_EDIT_ColorGreen, m_pTextBox->m_green[MENU_TEXTCOLOR_NORMAL] * 255.0f);
	SetDlgItemInt(IDC_EDIT_ColorBlue, m_pTextBox->m_blue[MENU_TEXTCOLOR_NORMAL] * 255.0f);
	SetDlgItemInt(IDC_EDIT_ColorAlpha, m_pTextBox->m_alpha[MENU_TEXTCOLOR_NORMAL] * 255.0f);*/

	SetDlgItemInt(IDC_EDIT_ColorRed, (UINT)(m_pTextBox->m_red[MENU_TEXTCOLOR_NORMAL] * 255.0f));
	SetDlgItemInt(IDC_EDIT_ColorGreen, (UINT)(m_pTextBox->m_green[MENU_TEXTCOLOR_NORMAL] * 255.0f));
	SetDlgItemInt(IDC_EDIT_ColorBlue, (UINT)(m_pTextBox->m_blue[MENU_TEXTCOLOR_NORMAL] * 255.0f));
	SetDlgItemInt(IDC_EDIT_ColorAlpha, (UINT)(m_pTextBox->m_alpha[MENU_TEXTCOLOR_NORMAL] * 255.0f));
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	SetDlgItemInt(IDC_EDIT_MaxLines, m_pTextBox->m_maxLinesOfText);
	SetDlgItemInt(IDC_EDIT_MaxLength, m_pTextBox->m_iMaxLength);
	CheckDlgButton(IDC_CHECK_Multilined, m_pTextBox->m_multilined);
	CheckDlgButton(IDC_CHECK_Editable, m_pTextBox->m_editable);
	CheckDlgButton(IDC_CHECK_NumericOnly, m_pTextBox->m_bIsNumericOnly);
	CheckDlgButton(IDC_CHECK_Password, m_pTextBox->m_password);

	//This must be done because GetIndexFromPointer defaultly returns 0 to select the first font in the list.
	// This means m_font could still be NULL, so we must make sure it's set
	m_pTextBox->m_font = m_gStatus->m_menuFontList->GetPointerFromIndex(m_gStatus->m_menuFontList->GetIndexFromPointer(m_pTextBox->m_font));

	//Create the lines of text to keep the editor from crashing
	if(m_pTextBox->m_linesOfText)
		delete [] m_pTextBox->m_linesOfText;

	m_pTextBox->m_linesOfText = new CMenuText[m_pTextBox->m_maxLinesOfText];

	for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
		m_pTextBox->m_linesOfText[i].m_font	 = m_pTextBox->m_font;

	//Button Coordinates
	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pTextBox->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pTextBox->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, m_pTextBox->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, m_pTextBox->m_height);*/

	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)m_pTextBox->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)m_pTextBox->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)m_pTextBox->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)m_pTextBox->m_height);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Background Left Texture
	if(m_pTextBox->m_backgroundLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX, m_pTextBox->m_backgroundLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY, m_pTextBox->m_backgroundLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth, m_pTextBox->m_backgroundLeft->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX, (UINT)m_pTextBox->m_backgroundLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY, (UINT)m_pTextBox->m_backgroundLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth, (UINT)m_pTextBox->m_backgroundLeft->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Middle Texture
	if(m_pTextBox->m_backgroundMiddle)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX, m_pTextBox->m_backgroundMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY, m_pTextBox->m_backgroundMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth, m_pTextBox->m_backgroundMiddle->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX, (UINT)m_pTextBox->m_backgroundMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY, (UINT)m_pTextBox->m_backgroundMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth, (UINT)m_pTextBox->m_backgroundMiddle->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Right Texture
	if(m_pTextBox->m_backgroundRight)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundRightTextureX, m_pTextBox->m_backgroundRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureY, m_pTextBox->m_backgroundRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth, m_pTextBox->m_backgroundRight->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureX, (UINT)m_pTextBox->m_backgroundRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureY, (UINT)m_pTextBox->m_backgroundRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth, (UINT)m_pTextBox->m_backgroundRight->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	// Text Cursor Texture
	/*SetDlgItemInt(IDC_EDIT_CursorWidth, m_pTextBox->m_cursor.m_width);
	SetDlgItemInt(IDC_EDIT_CursorHeight, m_pTextBox->m_cursor.m_height);
	SetDlgItemInt(IDC_EDIT_CursorTextureX, m_pTextBox->m_cursor.m_texturePosX);
	SetDlgItemInt(IDC_EDIT_CursorTextureY, m_pTextBox->m_cursor.m_texturePosY);

	SetDlgItemInt(IDC_EDIT_TextInset, m_pTextBox->m_textInset);*/

	SetDlgItemInt(IDC_EDIT_CursorWidth, (UINT)m_pTextBox->m_cursor.m_width);
	SetDlgItemInt(IDC_EDIT_CursorHeight, (UINT)m_pTextBox->m_cursor.m_height);
	SetDlgItemInt(IDC_EDIT_CursorTextureX, (UINT)m_pTextBox->m_cursor.m_texturePosX);
	SetDlgItemInt(IDC_EDIT_CursorTextureY, (UINT)m_pTextBox->m_cursor.m_texturePosY);

	SetDlgItemInt(IDC_EDIT_TextInset, (UINT)m_pTextBox->m_textInset);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	if(m_pTextBox->m_numColors > 1)
	{
		CheckDlgButton(IDC_CHECK_SystemReadout, true);

		CWnd *curItem = GetDlgItem(IDC_EDIT_ClanColorRed);	curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ClanColorGreen);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ClanColorBlue);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ClanColorAlpha);		curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_GroupColorRed);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_GroupColorGreen);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_GroupColorBlue);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_GroupColorAlpha);		curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_ServerColorRed);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ServerColorGreen);	curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ServerColorBlue);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ServerColorAlpha);	curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_DamageColorRed);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_DamageColorGreen);	curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_DamageColorBlue);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_DamageColorAlpha);	curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_HealColorRed);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_HealColorGreen);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_HealColorBlue);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_HealColorAlpha);		curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_MusicColorRed);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_MusicColorGreen);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_MusicColorBlue);		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_MusicColorAlpha);		curItem->EnableWindow(true);
		
		/*SetDlgItemInt(IDC_EDIT_ClanColorRed, m_pTextBox->m_red[MENU_TEXTCOLOR_CLAN] * 255);
		SetDlgItemInt(IDC_EDIT_ClanColorGreen, m_pTextBox->m_green[MENU_TEXTCOLOR_CLAN] * 255);
		SetDlgItemInt(IDC_EDIT_ClanColorBlue, m_pTextBox->m_blue[MENU_TEXTCOLOR_CLAN] * 255);
		SetDlgItemInt(IDC_EDIT_ClanColorAlpha, m_pTextBox->m_alpha[MENU_TEXTCOLOR_CLAN] * 255);

		SetDlgItemInt(IDC_EDIT_GroupColorRed, m_pTextBox->m_red[MENU_TEXTCOLOR_GROUP] * 255);
		SetDlgItemInt(IDC_EDIT_GroupColorGreen, m_pTextBox->m_green[MENU_TEXTCOLOR_GROUP] * 255);
		SetDlgItemInt(IDC_EDIT_GroupColorBlue, m_pTextBox->m_blue[MENU_TEXTCOLOR_GROUP] * 255);
		SetDlgItemInt(IDC_EDIT_GroupColorAlpha, m_pTextBox->m_alpha[MENU_TEXTCOLOR_GROUP] * 255);

		SetDlgItemInt(IDC_EDIT_ServerColorRed, m_pTextBox->m_red[MENU_TEXTCOLOR_SERVER] * 255);
		SetDlgItemInt(IDC_EDIT_ServerColorGreen, m_pTextBox->m_green[MENU_TEXTCOLOR_SERVER] * 255);
		SetDlgItemInt(IDC_EDIT_ServerColorBlue, m_pTextBox->m_blue[MENU_TEXTCOLOR_SERVER] * 255);
		SetDlgItemInt(IDC_EDIT_ServerColorAlpha, m_pTextBox->m_alpha[MENU_TEXTCOLOR_SERVER] * 255);

		SetDlgItemInt(IDC_EDIT_DamageColorRed, m_pTextBox->m_red[MENU_TEXTCOLOR_DAMAGE] * 255);
		SetDlgItemInt(IDC_EDIT_DamageColorGreen, m_pTextBox->m_green[MENU_TEXTCOLOR_DAMAGE] * 255);
		SetDlgItemInt(IDC_EDIT_DamageColorBlue, m_pTextBox->m_blue[MENU_TEXTCOLOR_DAMAGE] * 255);
		SetDlgItemInt(IDC_EDIT_DamageColorAlpha, m_pTextBox->m_alpha[MENU_TEXTCOLOR_DAMAGE] * 255);

		SetDlgItemInt(IDC_EDIT_HealColorRed, m_pTextBox->m_red[MENU_TEXTCOLOR_HEAL] * 255);
		SetDlgItemInt(IDC_EDIT_HealColorGreen, m_pTextBox->m_green[MENU_TEXTCOLOR_HEAL] * 255);
		SetDlgItemInt(IDC_EDIT_HealColorBlue, m_pTextBox->m_blue[MENU_TEXTCOLOR_HEAL] * 255);
		SetDlgItemInt(IDC_EDIT_HealColorAlpha, m_pTextBox->m_alpha[MENU_TEXTCOLOR_HEAL] * 255);

		SetDlgItemInt(IDC_EDIT_MusicColorRed, m_pTextBox->m_red[MENU_TEXTCOLOR_MUSIC] * 255);
		SetDlgItemInt(IDC_EDIT_MusicColorGreen, m_pTextBox->m_green[MENU_TEXTCOLOR_MUSIC] * 255);
		SetDlgItemInt(IDC_EDIT_MusicColorBlue, m_pTextBox->m_blue[MENU_TEXTCOLOR_MUSIC] * 255);
		SetDlgItemInt(IDC_EDIT_MusicColorAlpha, m_pTextBox->m_alpha[MENU_TEXTCOLOR_MUSIC] * 255);*/

		SetDlgItemInt(IDC_EDIT_ClanColorRed, (UINT)(m_pTextBox->m_red[MENU_TEXTCOLOR_CLAN] * 255));
		SetDlgItemInt(IDC_EDIT_ClanColorGreen, (UINT)(m_pTextBox->m_green[MENU_TEXTCOLOR_CLAN] * 255));
		SetDlgItemInt(IDC_EDIT_ClanColorBlue, (UINT)(m_pTextBox->m_blue[MENU_TEXTCOLOR_CLAN] * 255));
		SetDlgItemInt(IDC_EDIT_ClanColorAlpha, (UINT)(m_pTextBox->m_alpha[MENU_TEXTCOLOR_CLAN] * 255));

		SetDlgItemInt(IDC_EDIT_GroupColorRed, (UINT)(m_pTextBox->m_red[MENU_TEXTCOLOR_GROUP] * 255));
		SetDlgItemInt(IDC_EDIT_GroupColorGreen, (UINT)(m_pTextBox->m_green[MENU_TEXTCOLOR_GROUP] * 255));
		SetDlgItemInt(IDC_EDIT_GroupColorBlue, (UINT)(m_pTextBox->m_blue[MENU_TEXTCOLOR_GROUP] * 255));
		SetDlgItemInt(IDC_EDIT_GroupColorAlpha, (UINT)(m_pTextBox->m_alpha[MENU_TEXTCOLOR_GROUP] * 255));

		SetDlgItemInt(IDC_EDIT_ServerColorRed, (UINT)(m_pTextBox->m_red[MENU_TEXTCOLOR_SERVER] * 255));
		SetDlgItemInt(IDC_EDIT_ServerColorGreen, (UINT)(m_pTextBox->m_green[MENU_TEXTCOLOR_SERVER] * 255));
		SetDlgItemInt(IDC_EDIT_ServerColorBlue, (UINT)(m_pTextBox->m_blue[MENU_TEXTCOLOR_SERVER] * 255));
		SetDlgItemInt(IDC_EDIT_ServerColorAlpha, (UINT)(m_pTextBox->m_alpha[MENU_TEXTCOLOR_SERVER] * 255));

		SetDlgItemInt(IDC_EDIT_DamageColorRed, (UINT)(m_pTextBox->m_red[MENU_TEXTCOLOR_DAMAGE] * 255));
		SetDlgItemInt(IDC_EDIT_DamageColorGreen, (UINT)(m_pTextBox->m_green[MENU_TEXTCOLOR_DAMAGE] * 255));
		SetDlgItemInt(IDC_EDIT_DamageColorBlue, (UINT)(m_pTextBox->m_blue[MENU_TEXTCOLOR_DAMAGE] * 255));
		SetDlgItemInt(IDC_EDIT_DamageColorAlpha, (UINT)(m_pTextBox->m_alpha[MENU_TEXTCOLOR_DAMAGE] * 255));

		SetDlgItemInt(IDC_EDIT_HealColorRed, (UINT)(m_pTextBox->m_red[MENU_TEXTCOLOR_HEAL] * 255));
		SetDlgItemInt(IDC_EDIT_HealColorGreen, (UINT)(m_pTextBox->m_green[MENU_TEXTCOLOR_HEAL] * 255));
		SetDlgItemInt(IDC_EDIT_HealColorBlue, (UINT)(m_pTextBox->m_blue[MENU_TEXTCOLOR_HEAL] * 255));
		SetDlgItemInt(IDC_EDIT_HealColorAlpha, (UINT)(m_pTextBox->m_alpha[MENU_TEXTCOLOR_HEAL] * 255));

		SetDlgItemInt(IDC_EDIT_MusicColorRed, (UINT)(m_pTextBox->m_red[MENU_TEXTCOLOR_MUSIC] * 255));
		SetDlgItemInt(IDC_EDIT_MusicColorGreen, (UINT)(m_pTextBox->m_green[MENU_TEXTCOLOR_MUSIC] * 255));
		SetDlgItemInt(IDC_EDIT_MusicColorBlue, (UINT)(m_pTextBox->m_blue[MENU_TEXTCOLOR_MUSIC] * 255));
		SetDlgItemInt(IDC_EDIT_MusicColorAlpha, (UINT)(m_pTextBox->m_alpha[MENU_TEXTCOLOR_MUSIC] * 255));

		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	m_initialized = true;

	return TRUE;
}

void CEditTextBox::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditTextBox::OnChangeText()
{
	if(!m_initialized)
		return;

	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_Text, temp, 255);
	m_pTextBox->SetText(temp);
}

void CEditTextBox::OnChangeFont()
{
	if(!m_initialized)
		return;

	m_pTextBox->m_font		= m_gStatus->m_menuFontList->GetPointerFromIndex(SendDlgItemMessage(IDC_COMBO_Font, CB_GETCURSEL));
	m_pTextBox->m_fontName	= m_pTextBox->m_font->m_name;

	//Make it rebuild all of the text's mesh
	if(m_pTextBox->m_linesOfText)
	{
		for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
		{
			//Copy the new font
			m_pTextBox->m_linesOfText[i].m_font = m_pTextBox->m_font;

			//Make it rebuild the mesh
			if(m_pTextBox->m_linesOfText[i].m_textMesh)
			{
				m_pTextBox->m_linesOfText[i].m_textMesh->SafeDelete();
				delete m_pTextBox->m_linesOfText[i].m_textMesh;
				m_pTextBox->m_linesOfText[i].m_textMesh = NULL;
			}
		}
	}
}

void CEditTextBox::OnChangeAlignment()
{
	if(!m_initialized)
		return;

	m_pTextBox->m_horAlignment = SendDlgItemMessage(IDC_COMBO_Alignment, CB_GETCURSEL);

	m_pTextBox->Resize();
}

void CEditTextBox::OnChangeColor()
{
	if(!m_initialized)
		return;

	m_pTextBox->m_red[MENU_TEXTCOLOR_NORMAL]	= (int)GetDlgItemInt(IDC_EDIT_ColorRed) / 255.0f;
	m_pTextBox->m_green[MENU_TEXTCOLOR_NORMAL]	= (int)GetDlgItemInt(IDC_EDIT_ColorGreen) / 255.0f;
	m_pTextBox->m_blue[MENU_TEXTCOLOR_NORMAL]	= (int)GetDlgItemInt(IDC_EDIT_ColorBlue) / 255.0f;
	m_pTextBox->m_alpha[MENU_TEXTCOLOR_NORMAL]	= (int)GetDlgItemInt(IDC_EDIT_ColorAlpha) / 255.0f;

	//Make it rebuild all of the text's mesh
	if(m_pTextBox->m_linesOfText)
	{
		for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
		{
			//Set to the clan color for visual editing
			m_pTextBox->m_linesOfText[i].m_red	 = m_pTextBox->m_red[MENU_TEXTCOLOR_NORMAL];
			m_pTextBox->m_linesOfText[i].m_green = m_pTextBox->m_green[MENU_TEXTCOLOR_NORMAL];
			m_pTextBox->m_linesOfText[i].m_blue	 = m_pTextBox->m_blue[MENU_TEXTCOLOR_NORMAL];
			m_pTextBox->m_linesOfText[i].m_alpha = m_pTextBox->m_alpha[MENU_TEXTCOLOR_NORMAL];

			//Make it rebuild the mesh
			if(m_pTextBox->m_linesOfText[i].m_textMesh)
			{
				m_pTextBox->m_linesOfText[i].m_textMesh->SafeDelete();
				delete m_pTextBox->m_linesOfText[i].m_textMesh;
				m_pTextBox->m_linesOfText[i].m_textMesh = NULL;
			}
		}
	}
}

void CEditTextBox::OnToggleMultilined()
{
	m_pTextBox->m_multilined = IsDlgButtonChecked(IDC_CHECK_Multilined);
}

void CEditTextBox::OnToggleEditable()
{
	m_pTextBox->m_editable = IsDlgButtonChecked(IDC_CHECK_Editable);
}

void CEditTextBox::OnTogglePassword()
{
	m_pTextBox->m_password = IsDlgButtonChecked(IDC_CHECK_Password) ? true : false;
}

void CEditTextBox::OnToggleNumericOnly()
{
	m_pTextBox->m_bIsNumericOnly = IsDlgButtonChecked(IDC_CHECK_NumericOnly);
}

void CEditTextBox::OnChangeMaxLength()
{
	m_pTextBox->m_iMaxLength = (int)GetDlgItemInt(IDC_EDIT_MaxLength);
}

void CEditTextBox::OnChangeMaxLines()
{
	m_pTextBox->m_maxLinesOfText = (int)GetDlgItemInt(IDC_EDIT_MaxLines);

	if(m_pTextBox->m_linesOfText)
		delete [] m_pTextBox->m_linesOfText;

	m_pTextBox->m_linesOfText = new CMenuText[m_pTextBox->m_maxLinesOfText];

	//Make sure all the text objects have the font and normal color
	for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
	{
		m_pTextBox->m_linesOfText[i].m_font	 = m_pTextBox->m_font;
		m_pTextBox->m_linesOfText[i].m_red	 = m_pTextBox->m_red[MENU_TEXTCOLOR_NORMAL];
		m_pTextBox->m_linesOfText[i].m_green = m_pTextBox->m_green[MENU_TEXTCOLOR_NORMAL];
		m_pTextBox->m_linesOfText[i].m_blue	 = m_pTextBox->m_blue[MENU_TEXTCOLOR_NORMAL];
		m_pTextBox->m_linesOfText[i].m_alpha = m_pTextBox->m_alpha[MENU_TEXTCOLOR_NORMAL];
	}
}

void CEditTextBox::OnButtonEditScrollBar()
{
	//If the scroll bar hasn't been created yet
	if(!m_pTextBox->m_scrollBar)
	{
		//Create a new scroll bar and set its visual id
		m_pTextBox->m_scrollBar				= new CMenuScrollBar();
		m_pTextBox->m_scrollBar->m_visualID	= 0;
	}

	//If the Scroll Bar Editing Dialog exists, delete it
	if(m_scrollBarDlg)
		delete m_scrollBarDlg;

	//Create a new dialog
	m_scrollBarDlg					= new CEditScrollBar();
	m_scrollBarDlg->m_pScrollBar	= m_pTextBox->m_scrollBar;
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

void CEditTextBox::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pTextBox->m_posX	 = (int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pTextBox->m_posY	 = (int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pTextBox->m_width	 = (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pTextBox->m_height = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

	m_pTextBox->m_posX	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pTextBox->m_posY	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pTextBox->m_width	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pTextBox->m_height = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pTextBox->Resize();
}

void CEditTextBox::OnChangeBackgroundLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pTextBox->m_backgroundLeft)
	{
		m_pTextBox->m_backgroundLeft					= new CMenuImage();

		/*m_pTextBox->m_backgroundLeft->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

		m_pTextBox->m_backgroundLeft->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	/*UpdateImageTextureCoords(m_pTextBox->m_backgroundLeft,
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pTextBox->m_backgroundLeft,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditTextBox::OnButtonEditBackgroundLeftTexture()
{
	if(!m_pTextBox->m_backgroundLeft)
	{
		m_pTextBox->m_backgroundLeft					= new CMenuImage();

		/*m_pTextBox->m_backgroundLeft->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

		m_pTextBox->m_backgroundLeft->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		return;
	}

	EditTexture(m_pTextBox->m_backgroundLeft);
}

void CEditTextBox::OnChangeBackgroundMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pTextBox->m_backgroundMiddle)
	{
		m_pTextBox->m_backgroundMiddle					= new CMenuImage();

		/*m_pTextBox->m_backgroundMiddle->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		
		m_pTextBox->m_backgroundMiddle->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	/*UpdateImageTextureCoords(m_pTextBox->m_backgroundMiddle,
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth));*/

	UpdateImageTextureCoords(m_pTextBox->m_backgroundMiddle,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditTextBox::OnButtonEditBackgroundMiddleTexture()
{
	if(!m_pTextBox->m_backgroundMiddle)
	{
		m_pTextBox->m_backgroundMiddle					= new CMenuImage();

		/*m_pTextBox->m_backgroundMiddle->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

		m_pTextBox->m_backgroundMiddle->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
		return;
	}

	EditTexture(m_pTextBox->m_backgroundMiddle);
}

void CEditTextBox::OnChangeBackgroundRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pTextBox->m_backgroundRight)
	{
		m_pTextBox->m_backgroundRight					= new CMenuImage();

		/*m_pTextBox->m_backgroundRight->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

		m_pTextBox->m_backgroundRight->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	/*UpdateImageTextureCoords(m_pTextBox->m_backgroundRight,
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pTextBox->m_backgroundRight,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditTextBox::OnButtonEditBackgroundRightTexture()
{
	if(!m_pTextBox->m_backgroundRight)
	{
		m_pTextBox->m_backgroundRight					= new CMenuImage();

		/*m_pTextBox->m_backgroundRight->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

		m_pTextBox->m_backgroundRight->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
		return;
	}

	EditTexture(m_pTextBox->m_backgroundRight);
}

void CEditTextBox::OnChangeCursorCoords()
{
	if(!m_initialized)
		return;

	/*m_pTextBox->m_cursor.m_texturePosX	 = (int)GetDlgItemInt(IDC_EDIT_CursorTextureX);
	m_pTextBox->m_cursor.m_texturePosY	 = (int)GetDlgItemInt(IDC_EDIT_CursorTextureY);
	m_pTextBox->m_cursor.m_textureWidth	 = m_pTextBox->m_cursor.m_width	 = (int)GetDlgItemInt(IDC_EDIT_CursorWidth);
	m_pTextBox->m_cursor.m_textureHeight = m_pTextBox->m_cursor.m_height = (int)GetDlgItemInt(IDC_EDIT_CursorHeight);*/

	m_pTextBox->m_cursor.m_texturePosX	 = (float)(int)GetDlgItemInt(IDC_EDIT_CursorTextureX);
	m_pTextBox->m_cursor.m_texturePosY	 = (float)(int)GetDlgItemInt(IDC_EDIT_CursorTextureY);
	m_pTextBox->m_cursor.m_textureWidth	 = (float)m_pTextBox->m_cursor.m_width	 = (float)(int)GetDlgItemInt(IDC_EDIT_CursorWidth);
	m_pTextBox->m_cursor.m_textureHeight = (float)m_pTextBox->m_cursor.m_height = (float)(int)GetDlgItemInt(IDC_EDIT_CursorHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	UpdateImageTextureCoords(&m_pTextBox->m_cursor);
}

void CEditTextBox::OnButtonEditCursorTexture()
{
	if(!m_pTextBox->m_cursor.m_imageMesh)
	{
		m_pTextBox->m_showCursor = true;
		return;
	}

	EditTexture(&m_pTextBox->m_cursor);
}

void CEditTextBox::OnChangeTextInset()
{
	/*m_pTextBox->m_textInset = (int)GetDlgItemInt(IDC_EDIT_TextInset);*/

	m_pTextBox->m_textInset = (float)(int)GetDlgItemInt(IDC_EDIT_TextInset);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pTextBox->Resize();
}

void CEditTextBox::OnToggleSystemReadout()
{
	if(IsDlgButtonChecked(IDC_CHECK_SystemReadout))
	{
		//Delete all the old color arrays
		delete [] m_pTextBox->m_red;
		delete [] m_pTextBox->m_green;
		delete [] m_pTextBox->m_blue;
		delete [] m_pTextBox->m_alpha;

		//Save the number of colors
		m_pTextBox->m_numColors = MENU_TEXTCOLOR_MAX;

		//Create the new ones
		m_pTextBox->m_red	= new float[m_pTextBox->m_numColors];
		m_pTextBox->m_green = new float[m_pTextBox->m_numColors];
		m_pTextBox->m_blue	= new float[m_pTextBox->m_numColors];
		m_pTextBox->m_alpha	= new float[m_pTextBox->m_numColors];

		//Copy the color back in
		OnChangeColor();
		OnChangeClanColor();
		OnChangeGroupColor();
		OnChangeServerColor();
		OnChangeDamageColor();
		OnChangeHealColor();
		OnChangeMusicColor();

		CWnd *curItem = GetDlgItem(IDC_EDIT_ClanColorRed);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ClanColorGreen);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ClanColorBlue);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ClanColorAlpha);
		curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_GroupColorRed);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_GroupColorGreen);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_GroupColorBlue);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_GroupColorAlpha);
		curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_ServerColorRed);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ServerColorGreen);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ServerColorBlue);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_ServerColorAlpha);
		curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_DamageColorRed);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_DamageColorGreen);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_DamageColorBlue);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_DamageColorAlpha);
		curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_HealColorRed);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_HealColorGreen);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_HealColorBlue);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_HealColorAlpha);
		curItem->EnableWindow(true);

		curItem = GetDlgItem(IDC_EDIT_MusicColorRed);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_MusicColorGreen);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_MusicColorBlue);
		curItem->EnableWindow(true);
		curItem = GetDlgItem(IDC_EDIT_MusicColorAlpha);
		curItem->EnableWindow(true);
	}
	else
	{
		//Delete all the old color arrays
		delete [] m_pTextBox->m_red;
		delete [] m_pTextBox->m_green;
		delete [] m_pTextBox->m_blue;
		delete [] m_pTextBox->m_alpha;

		//Save the number of colors
		m_pTextBox->m_numColors = 1;
	
		//Create the new ones
		m_pTextBox->m_red	= new float[m_pTextBox->m_numColors];
		m_pTextBox->m_green = new float[m_pTextBox->m_numColors];
		m_pTextBox->m_blue	= new float[m_pTextBox->m_numColors];
		m_pTextBox->m_alpha	= new float[m_pTextBox->m_numColors];

		//Copy the color back in
		OnChangeColor();

		CWnd *curItem = GetDlgItem(IDC_EDIT_ClanColorRed);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_ClanColorGreen);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_ClanColorBlue);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_ClanColorAlpha);
		curItem->EnableWindow(false);

		curItem = GetDlgItem(IDC_EDIT_GroupColorRed);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_GroupColorGreen);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_GroupColorBlue);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_GroupColorAlpha);
		curItem->EnableWindow(false);

		curItem = GetDlgItem(IDC_EDIT_ServerColorRed);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_ServerColorGreen);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_ServerColorBlue);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_ServerColorAlpha);
		curItem->EnableWindow(false);

		curItem = GetDlgItem(IDC_EDIT_DamageColorRed);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_DamageColorGreen);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_DamageColorBlue);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_DamageColorAlpha);
		curItem->EnableWindow(false);

		curItem = GetDlgItem(IDC_EDIT_HealColorRed);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_HealColorGreen);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_HealColorBlue);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_HealColorAlpha);
		curItem->EnableWindow(false);

		curItem = GetDlgItem(IDC_EDIT_MusicColorRed);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_MusicColorGreen);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_MusicColorBlue);
		curItem->EnableWindow(false);
		curItem = GetDlgItem(IDC_EDIT_MusicColorAlpha);
		curItem->EnableWindow(false);
	}
}

void CEditTextBox::OnChangeClanColor()
{
	if(!m_initialized)
		return;

	m_pTextBox->m_red[MENU_TEXTCOLOR_CLAN]	 = (int)GetDlgItemInt(IDC_EDIT_ClanColorRed) / 255.0f;
	m_pTextBox->m_green[MENU_TEXTCOLOR_CLAN] = (int)GetDlgItemInt(IDC_EDIT_ClanColorGreen) / 255.0f;
	m_pTextBox->m_blue[MENU_TEXTCOLOR_CLAN]	 = (int)GetDlgItemInt(IDC_EDIT_ClanColorBlue) / 255.0f;
	m_pTextBox->m_alpha[MENU_TEXTCOLOR_CLAN] = (int)GetDlgItemInt(IDC_EDIT_ClanColorAlpha) / 255.0f;

	//Make it rebuild all of the text's mesh
	if(m_pTextBox->m_linesOfText)
	{
		for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
		{
			//Set to the clan color for visual editing
			m_pTextBox->m_linesOfText[i].m_red	 = m_pTextBox->m_red[MENU_TEXTCOLOR_CLAN];
			m_pTextBox->m_linesOfText[i].m_green = m_pTextBox->m_green[MENU_TEXTCOLOR_CLAN];
			m_pTextBox->m_linesOfText[i].m_blue	 = m_pTextBox->m_blue[MENU_TEXTCOLOR_CLAN];
			m_pTextBox->m_linesOfText[i].m_alpha = m_pTextBox->m_alpha[MENU_TEXTCOLOR_CLAN];

			//Make it rebuild the mesh
			if(m_pTextBox->m_linesOfText[i].m_textMesh)
			{
				m_pTextBox->m_linesOfText[i].m_textMesh->SafeDelete();
				delete m_pTextBox->m_linesOfText[i].m_textMesh;
				m_pTextBox->m_linesOfText[i].m_textMesh = NULL;
			}
		}
	}
}

void CEditTextBox::OnChangeGroupColor()
{
	if(!m_initialized)
		return;

	m_pTextBox->m_red[MENU_TEXTCOLOR_GROUP]		= (int)GetDlgItemInt(IDC_EDIT_GroupColorRed) / 255.0f;
	m_pTextBox->m_green[MENU_TEXTCOLOR_GROUP]	= (int)GetDlgItemInt(IDC_EDIT_GroupColorGreen) / 255.0f;
	m_pTextBox->m_blue[MENU_TEXTCOLOR_GROUP]	= (int)GetDlgItemInt(IDC_EDIT_GroupColorBlue) / 255.0f;
	m_pTextBox->m_alpha[MENU_TEXTCOLOR_GROUP]	= (int)GetDlgItemInt(IDC_EDIT_GroupColorAlpha) / 255.0f;

	//Make it rebuild all of the text's mesh
	if(m_pTextBox->m_linesOfText)
	{
		for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
		{
			//Set to the clan color for visual editing
			m_pTextBox->m_linesOfText[i].m_red	 = m_pTextBox->m_red[MENU_TEXTCOLOR_GROUP];
			m_pTextBox->m_linesOfText[i].m_green = m_pTextBox->m_green[MENU_TEXTCOLOR_GROUP];
			m_pTextBox->m_linesOfText[i].m_blue	 = m_pTextBox->m_blue[MENU_TEXTCOLOR_GROUP];
			m_pTextBox->m_linesOfText[i].m_alpha = m_pTextBox->m_alpha[MENU_TEXTCOLOR_GROUP];

			//Make it rebuild the mesh
			if(m_pTextBox->m_linesOfText[i].m_textMesh)
			{
				m_pTextBox->m_linesOfText[i].m_textMesh->SafeDelete();
				delete m_pTextBox->m_linesOfText[i].m_textMesh;
				m_pTextBox->m_linesOfText[i].m_textMesh = NULL;
			}
		}
	}
}

void CEditTextBox::OnChangeServerColor()
{
	if(!m_initialized)
		return;

	m_pTextBox->m_red[MENU_TEXTCOLOR_SERVER]	= (int)GetDlgItemInt(IDC_EDIT_ServerColorRed) / 255.0f;
	m_pTextBox->m_green[MENU_TEXTCOLOR_SERVER]	= (int)GetDlgItemInt(IDC_EDIT_ServerColorGreen) / 255.0f;
	m_pTextBox->m_blue[MENU_TEXTCOLOR_SERVER]	= (int)GetDlgItemInt(IDC_EDIT_ServerColorBlue) / 255.0f;
	m_pTextBox->m_alpha[MENU_TEXTCOLOR_SERVER]	= (int)GetDlgItemInt(IDC_EDIT_ServerColorAlpha) / 255.0f;

	//Make it rebuild all of the text's mesh
	if(m_pTextBox->m_linesOfText)
	{
		for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
		{
			//Set to the clan color for visual editing
			m_pTextBox->m_linesOfText[i].m_red	 = m_pTextBox->m_red[MENU_TEXTCOLOR_SERVER];
			m_pTextBox->m_linesOfText[i].m_green = m_pTextBox->m_green[MENU_TEXTCOLOR_SERVER];
			m_pTextBox->m_linesOfText[i].m_blue	 = m_pTextBox->m_blue[MENU_TEXTCOLOR_SERVER];
			m_pTextBox->m_linesOfText[i].m_alpha = m_pTextBox->m_alpha[MENU_TEXTCOLOR_SERVER];

			//Make it rebuild the mesh
			if(m_pTextBox->m_linesOfText[i].m_textMesh)
			{
				m_pTextBox->m_linesOfText[i].m_textMesh->SafeDelete();
				delete m_pTextBox->m_linesOfText[i].m_textMesh;
				m_pTextBox->m_linesOfText[i].m_textMesh = NULL;
			}
		}
	}
}

void CEditTextBox::OnChangeDamageColor()
{
	if(!m_initialized)
		return;

	m_pTextBox->m_red[MENU_TEXTCOLOR_DAMAGE]	= (int)GetDlgItemInt(IDC_EDIT_DamageColorRed) / 255.0f;
	m_pTextBox->m_green[MENU_TEXTCOLOR_DAMAGE]	= (int)GetDlgItemInt(IDC_EDIT_DamageColorGreen) / 255.0f;
	m_pTextBox->m_blue[MENU_TEXTCOLOR_DAMAGE]	= (int)GetDlgItemInt(IDC_EDIT_DamageColorBlue) / 255.0f;
	m_pTextBox->m_alpha[MENU_TEXTCOLOR_DAMAGE]	= (int)GetDlgItemInt(IDC_EDIT_DamageColorAlpha) / 255.0f;

	//Make it rebuild all of the text's mesh
	if(m_pTextBox->m_linesOfText)
	{
		for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
		{
			//Set to the clan color for visual editing
			m_pTextBox->m_linesOfText[i].m_red	 = m_pTextBox->m_red[MENU_TEXTCOLOR_DAMAGE];
			m_pTextBox->m_linesOfText[i].m_green = m_pTextBox->m_green[MENU_TEXTCOLOR_DAMAGE];
			m_pTextBox->m_linesOfText[i].m_blue	 = m_pTextBox->m_blue[MENU_TEXTCOLOR_DAMAGE];
			m_pTextBox->m_linesOfText[i].m_alpha = m_pTextBox->m_alpha[MENU_TEXTCOLOR_DAMAGE];

			//Make it rebuild the mesh
			if(m_pTextBox->m_linesOfText[i].m_textMesh)
			{
				m_pTextBox->m_linesOfText[i].m_textMesh->SafeDelete();
				delete m_pTextBox->m_linesOfText[i].m_textMesh;
				m_pTextBox->m_linesOfText[i].m_textMesh = NULL;
			}
		}
	}
}

void CEditTextBox::OnChangeHealColor()
{
	if(!m_initialized)
		return;

	m_pTextBox->m_red[MENU_TEXTCOLOR_HEAL]	 = (int)GetDlgItemInt(IDC_EDIT_HealColorRed) / 255.0f;
	m_pTextBox->m_green[MENU_TEXTCOLOR_HEAL] = (int)GetDlgItemInt(IDC_EDIT_HealColorGreen) / 255.0f;
	m_pTextBox->m_blue[MENU_TEXTCOLOR_HEAL]	 = (int)GetDlgItemInt(IDC_EDIT_HealColorBlue) / 255.0f;
	m_pTextBox->m_alpha[MENU_TEXTCOLOR_HEAL] = (int)GetDlgItemInt(IDC_EDIT_HealColorAlpha) / 255.0f;

	//Make it rebuild all of the text's mesh
	if(m_pTextBox->m_linesOfText)
	{
		for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
		{
			//Set to the clan color for visual editing
			m_pTextBox->m_linesOfText[i].m_red	 = m_pTextBox->m_red[MENU_TEXTCOLOR_HEAL];
			m_pTextBox->m_linesOfText[i].m_green = m_pTextBox->m_green[MENU_TEXTCOLOR_HEAL];
			m_pTextBox->m_linesOfText[i].m_blue	 = m_pTextBox->m_blue[MENU_TEXTCOLOR_HEAL];
			m_pTextBox->m_linesOfText[i].m_alpha = m_pTextBox->m_alpha[MENU_TEXTCOLOR_HEAL];

			//Make it rebuild the mesh
			if(m_pTextBox->m_linesOfText[i].m_textMesh)
			{
				m_pTextBox->m_linesOfText[i].m_textMesh->SafeDelete();
				delete m_pTextBox->m_linesOfText[i].m_textMesh;
				m_pTextBox->m_linesOfText[i].m_textMesh = NULL;
			}
		}
	}
}

void CEditTextBox::OnChangeMusicColor()
{
	if(!m_initialized)
		return;

	m_pTextBox->m_red[MENU_TEXTCOLOR_MUSIC]		= (int)GetDlgItemInt(IDC_EDIT_MusicColorRed) / 255.0f;
	m_pTextBox->m_green[MENU_TEXTCOLOR_MUSIC]	= (int)GetDlgItemInt(IDC_EDIT_MusicColorGreen) / 255.0f;
	m_pTextBox->m_blue[MENU_TEXTCOLOR_MUSIC]	= (int)GetDlgItemInt(IDC_EDIT_MusicColorBlue) / 255.0f;
	m_pTextBox->m_alpha[MENU_TEXTCOLOR_MUSIC]	= (int)GetDlgItemInt(IDC_EDIT_MusicColorAlpha) / 255.0f;

	//Make it rebuild all of the text's mesh
	if(m_pTextBox->m_linesOfText)
	{
		for(int i = 0; i < m_pTextBox->m_maxLinesOfText; i++)
		{
			//Set to the clan color for visual editing
			m_pTextBox->m_linesOfText[i].m_red	 = m_pTextBox->m_red[MENU_TEXTCOLOR_MUSIC];
			m_pTextBox->m_linesOfText[i].m_green = m_pTextBox->m_green[MENU_TEXTCOLOR_MUSIC];
			m_pTextBox->m_linesOfText[i].m_blue	 = m_pTextBox->m_blue[MENU_TEXTCOLOR_MUSIC];
			m_pTextBox->m_linesOfText[i].m_alpha = m_pTextBox->m_alpha[MENU_TEXTCOLOR_MUSIC];

			//Make it rebuild the mesh
			if(m_pTextBox->m_linesOfText[i].m_textMesh)
			{
				m_pTextBox->m_linesOfText[i].m_textMesh->SafeDelete();
				delete m_pTextBox->m_linesOfText[i].m_textMesh;
				m_pTextBox->m_linesOfText[i].m_textMesh = NULL;
			}
		}
	}
}

void CEditTextBox::OnButtonEditEnterPressScript()
{
	if(!m_pTextBox->m_enterPressScript)
	{
		m_pTextBox->m_enterPressScript					= new CMenuScript();
		m_pTextBox->m_enterPressScript->m_callingVisual	= (CMenuVisual*)m_pTextBox;
	}

	CEditScript dlg;
	dlg.m_script = m_pTextBox->m_enterPressScript;
	dlg.DoModal();
}

void CEditTextBox::OnButtonEditMouseOverScript()
{
	if(!m_pTextBox->m_mouseOverScript)
	{
		m_pTextBox->m_mouseOverScript					= new CMenuScript();
		m_pTextBox->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)m_pTextBox;
	}

	CEditScript dlg;
	dlg.m_script = m_pTextBox->m_mouseOverScript;
	dlg.DoModal();
}

void CEditTextBox::OnButtonEditMouseOutScript()
{
	if(!m_pTextBox->m_mouseOutScript)
	{
		m_pTextBox->m_mouseOutScript					= new CMenuScript();
		m_pTextBox->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)m_pTextBox;
	}

	CEditScript dlg;
	dlg.m_script = m_pTextBox->m_mouseOutScript;
	dlg.DoModal();
}

void CEditTextBox::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pTextBox->m_name = temp;

	m_pTextBox->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);

	CDialog::OnOK();
}

void CEditTextBox::EditTexture(CMenuImage *pImage)
{
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef	= NULL;
	dlg.GL_textureDataBase	= m_gStatus->m_textureDataBase;
	dlg.meshRef				= pImage->m_imageMesh;

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

void CEditTextBox::UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, bool CopyWidth)
{
	pImage->m_texturePosX	= X;
	pImage->m_texturePosY	= Y;
	pImage->m_textureWidth	= Width;

	if(CopyWidth)
		pImage->m_width = Width;

	UpdateImageTextureCoords(pImage);
}

void CEditTextBox::UpdateImageTextureCoords(CMenuImage *pImage)
{
	if(pImage->m_imageMesh)
	{
		//Get the texture dimensions
		float textureWidth	= 512.0f;
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
			
			textureName		= texture->List_textureName;

			//WARNING: --> C4244 conversion from DWORD to float, possible loss of data
		}
			
		//Copy texture file
		if(textureName)
			pImage->m_textureFile = textureName;

		//Update mesh's texture coordinates
		pImage->m_fullTextureWidth	= textureWidth;
		pImage->m_fullTextureHeight	= textureHeight;
		pImage->UpdateTextureCoords();
	}

	m_pTextBox->Resize();
}

// CEditTextBox message handlers
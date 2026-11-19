// C:\Projects\EXCore_Platform\AuraUI\EditButton.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditButton.h"
#include "EditScript.h"

// CEditButton dialog
IMPLEMENT_DYNAMIC(CEditButton, CDialog)

CEditButton::CEditButton(CWnd* pParent /*=NULL*/)
	: CDialog(CEditButton::IDD, pParent)
{
	m_pButton			= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
	m_freezeInput		= NULL;
}

BEGIN_MESSAGE_MAP(CEditButton, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	// Button Properties
	ON_EN_CHANGE(IDC_EDIT_ButtonText, OnChangeButtonText)

	//Button Coordinates
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)

	//Up State Left Texture
	ON_EN_CHANGE(IDC_EDIT_UpStateLeftTextureX, OnChangeUpStateLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_UpStateLeftTextureY, OnChangeUpStateLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_UpStateLeftTextureWidth, OnChangeUpStateLeftTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditUpStateLeftTexture, OnButtonEditUpStateLeftTexture)

	//Up State Middle Texture
	ON_EN_CHANGE(IDC_EDIT_UpStateMiddleTextureX, OnChangeUpStateMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_UpStateMiddleTextureY, OnChangeUpStateMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_UpStateMiddleTextureWidth, OnChangeUpStateMiddleTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditUpStateMiddleTexture, OnButtonEditUpStateMiddleTexture)

	//Up State Right Texture
	ON_EN_CHANGE(IDC_EDIT_UpStateRightTextureX, OnChangeUpStateRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_UpStateRightTextureY, OnChangeUpStateRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_UpStateRightTextureWidth, OnChangeUpStateRightTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditUpStateRightTexture, OnButtonEditUpStateRightTexture)

	//Hover State Left Texture
	ON_EN_CHANGE(IDC_EDIT_HoverStateLeftTextureX, OnChangeHoverStateLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_HoverStateLeftTextureY, OnChangeHoverStateLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_HoverStateLeftTextureWidth, OnChangeHoverStateLeftTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditHoverStateLeftTexture, OnButtonEditHoverStateLeftTexture)

	//Hover State Middle Texture
	ON_EN_CHANGE(IDC_EDIT_HoverStateMiddleTextureX, OnChangeHoverStateMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_HoverStateMiddleTextureY, OnChangeHoverStateMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_HoverStateMiddleTextureWidth, OnChangeHoverStateMiddleTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditHoverStateMiddleTexture, OnButtonEditHoverStateMiddleTexture)

	//Hover State Right Texture
	ON_EN_CHANGE(IDC_EDIT_HoverStateRightTextureX, OnChangeHoverStateRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_HoverStateRightTextureY, OnChangeHoverStateRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_HoverStateRightTextureWidth, OnChangeHoverStateRightTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditHoverStateRightTexture, OnButtonEditHoverStateRightTexture)

	//Down State Left Texture
	ON_EN_CHANGE(IDC_EDIT_DownStateLeftTextureX, OnChangeDownStateLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DownStateLeftTextureY, OnChangeDownStateLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DownStateLeftTextureWidth, OnChangeDownStateLeftTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditDownStateLeftTexture, OnButtonEditDownStateLeftTexture)

	//Down State Middle Texture
	ON_EN_CHANGE(IDC_EDIT_DownStateMiddleTextureX, OnChangeDownStateMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DownStateMiddleTextureY, OnChangeDownStateMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DownStateMiddleTextureWidth, OnChangeDownStateMiddleTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditDownStateMiddleTexture, OnButtonEditDownStateMiddleTexture)

	//Down State Right Texture
	ON_EN_CHANGE(IDC_EDIT_DownStateRightTextureX, OnChangeDownStateRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DownStateRightTextureY, OnChangeDownStateRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DownStateRightTextureWidth, OnChangeDownStateRightTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditDownStateRightTexture, OnButtonEditDownStateRightTexture)

	//Disabled State Left Texture
	ON_EN_CHANGE(IDC_EDIT_DisabledStateLeftTextureX, OnChangeDisabledStateLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DisabledStateLeftTextureY, OnChangeDisabledStateLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DisabledStateLeftTextureWidth, OnChangeDisabledStateLeftTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditDisabledStateLeftTexture, OnButtonEditDisabledStateLeftTexture)

	//Disabled State Middle Texture
	ON_EN_CHANGE(IDC_EDIT_DisabledStateMiddleTextureX, OnChangeDisabledStateMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DisabledStateMiddleTextureY, OnChangeDisabledStateMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DisabledStateMiddleTextureWidth, OnChangeDisabledStateMiddleTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditDisabledStateMiddleTexture, OnButtonEditDisabledStateMiddleTexture)

	//Disabled State Right Texture
	ON_EN_CHANGE(IDC_EDIT_DisabledStateRightTextureX, OnChangeDisabledStateRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DisabledStateRightTextureY, OnChangeDisabledStateRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DisabledStateRightTextureWidth, OnChangeDisabledStateRightTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditDisabledStateRightTexture, OnButtonEditDisabledStateRightTexture)

	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditClickScript, OnButtonEditClickScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOverScript, OnButtonEditMouseOverScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOutScript, OnButtonEditMouseOutScript)
	ON_BN_CLICKED(IDC_BUTTON_EditDropScript, OnButtonEditDropScript)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditButton::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pButton->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pButton->m_visualID);
	SetDlgItemText(IDC_EDIT_ButtonText, m_pButton->m_pDisplayText->m_text);

	//Button Coordinates
	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pButton->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pButton->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, m_pButton->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, m_pButton->m_height);*/

	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)m_pButton->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)m_pButton->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)m_pButton->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)m_pButton->m_height);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data


	//Up State Left Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT])
	{
		/*SetDlgItemInt(IDC_EDIT_UpStateLeftTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_UpStateLeftTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_UpStateLeftTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_UpStateLeftTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_UpStateLeftTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_UpStateLeftTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Up State Middle Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_UpStateMiddleTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_UpStateMiddleTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_UpStateMiddleTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_UpStateMiddleTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_UpStateMiddleTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_UpStateMiddleTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Up State Right Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT])
	{
		/*SetDlgItemInt(IDC_EDIT_UpStateRightTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_UpStateRightTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_UpStateRightTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_UpStateRightTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_UpStateRightTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_UpStateRightTextureWidth,(UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Hover State Left Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT])
	{
		/*SetDlgItemInt(IDC_EDIT_HoverStateLeftTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_HoverStateLeftTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_HoverStateLeftTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_HoverStateLeftTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_HoverStateLeftTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_HoverStateLeftTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Hover State Middle Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Hover State Right Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT])
	{
		/*SetDlgItemInt(IDC_EDIT_HoverStateRightTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_HoverStateRightTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_HoverStateRightTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_HoverStateRightTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_HoverStateRightTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_HoverStateRightTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Down State Left Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT])
	{
		/*SetDlgItemInt(IDC_EDIT_DownStateLeftTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DownStateLeftTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DownStateLeftTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_DownStateLeftTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DownStateLeftTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DownStateLeftTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Down State Middle Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_DownStateMiddleTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DownStateMiddleTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DownStateMiddleTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_DownStateMiddleTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DownStateMiddleTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DownStateMiddleTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Down State Right Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT])
	{
		/*SetDlgItemInt(IDC_EDIT_DownStateRightTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DownStateRightTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DownStateRightTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_DownStateRightTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DownStateRightTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DownStateRightTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Disabled State Left Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT])
	{
		/*SetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Disabled State Middle Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Disabled State Right Texture
	if(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT])
	{
		/*SetDlgItemInt(IDC_EDIT_DisabledStateRightTextureX, m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DisabledStateRightTextureY, m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DisabledStateRightTextureWidth, m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_DisabledStateRightTextureX, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DisabledStateRightTextureY, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_DisabledStateRightTextureWidth, (UINT)m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_textureWidth);		
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Allow us to set the button's state and not have it overwritten
	*m_freezeInput = true;

	m_initialized = true;

	return TRUE;
}

void CEditButton::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void 
CEditButton::OnChangeButtonText()
{
	if(m_pButton->m_pDisplayText)
	{
		LPTSTR strDisplay = new TCHAR[MAX_BUFFER];
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ButtonText);
		pEdit->GetWindowTextA(strDisplay, MAX_BUFFER);
		m_pButton->SetText(strDisplay);
		SafeDelete(strDisplay);
	}
}

void CEditButton::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pButton->m_posX	= (int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pButton->m_posY	= (int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pButton->m_width	= (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pButton->m_height = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

	m_pButton->m_posX	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pButton->m_posY	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pButton->m_width	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pButton->m_height = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	//Loop through every possible state
	for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
	{
		//Update the texture height of each state
		if(m_pButton->m_stateImages[i][MENU_BUTTONIMAGE_LEFT])
		{
			m_pButton->m_stateImages[i][MENU_BUTTONIMAGE_LEFT]->m_textureHeight = m_pButton->m_height;
			m_pButton->m_stateImages[i][MENU_BUTTONIMAGE_LEFT]->UpdateTextureCoords();
		}
		if(m_pButton->m_stateImages[i][MENU_BUTTONIMAGE_MIDDLE])
		{
			m_pButton->m_stateImages[i][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight = m_pButton->m_height;
			m_pButton->m_stateImages[i][MENU_BUTTONIMAGE_MIDDLE]->UpdateTextureCoords();
		}
		if(m_pButton->m_stateImages[i][MENU_BUTTONIMAGE_RIGHT])
		{
			m_pButton->m_stateImages[i][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight = m_pButton->m_height;
			m_pButton->m_stateImages[i][MENU_BUTTONIMAGE_RIGHT]->UpdateTextureCoords();
		}
	}

	m_pButton->Resize();
}

void CEditButton::OnChangeUpStateLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_UP;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT],
							 (int)GetDlgItemInt(IDC_EDIT_UpStateLeftTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_UpStateLeftTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_UpStateLeftTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT],
							 (float)(int)GetDlgItemInt(IDC_EDIT_UpStateLeftTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_UpStateLeftTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_UpStateLeftTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditUpStateLeftTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT] || m_pButton->m_visibleState != MENU_BUTTONSTATE_UP)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_UP;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_LEFT]);
}

void CEditButton::OnChangeUpStateMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_UP;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_UpStateMiddleTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_UpStateMiddleTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_UpStateMiddleTextureWidth));*/

UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_UpStateMiddleTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_UpStateMiddleTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_UpStateMiddleTextureWidth));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditUpStateMiddleTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE] || m_pButton->m_visibleState != MENU_BUTTONSTATE_UP)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_UP;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditButton::OnChangeUpStateRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_UP;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT],
							 (int)GetDlgItemInt(IDC_EDIT_UpStateRightTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_UpStateRightTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_UpStateRightTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT],
							 (float)(int)GetDlgItemInt(IDC_EDIT_UpStateRightTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_UpStateRightTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_UpStateRightTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditUpStateRightTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT] || m_pButton->m_visibleState != MENU_BUTTONSTATE_UP)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]					= new CMenuImage();
		
		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_UP;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_RIGHT]);
}

void CEditButton::OnChangeHoverStateLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]					 = new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_textureHeight = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_textureHeight = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_HOVER;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT],
							 (int)GetDlgItemInt(IDC_EDIT_HoverStateLeftTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_HoverStateLeftTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_HoverStateLeftTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT],
							 (float)(int)GetDlgItemInt(IDC_EDIT_HoverStateLeftTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_HoverStateLeftTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_HoverStateLeftTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditHoverStateLeftTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT] || m_pButton->m_visibleState != MENU_BUTTONSTATE_HOVER)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]					 = new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_textureHeight = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]->m_textureHeight = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_HOVER;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_LEFT]);
}

void CEditButton::OnChangeHoverStateMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_HOVER;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureWidth));*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_HoverStateMiddleTextureWidth));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditHoverStateMiddleTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE] || m_pButton->m_visibleState != MENU_BUTTONSTATE_HOVER)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_HOVER;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditButton::OnChangeHoverStateRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_HOVER;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT],
							 (int)GetDlgItemInt(IDC_EDIT_HoverStateRightTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_HoverStateRightTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_HoverStateRightTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT],
							 (float)(int)GetDlgItemInt(IDC_EDIT_HoverStateRightTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_HoverStateRightTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_HoverStateRightTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditHoverStateRightTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT] || m_pButton->m_visibleState != MENU_BUTTONSTATE_HOVER)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
		m_pButton->m_visibleState = MENU_BUTTONSTATE_HOVER;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_RIGHT]);
}

void CEditButton::OnChangeDownStateLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_DOWN;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT],
							 (int)GetDlgItemInt(IDC_EDIT_DownStateLeftTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_DownStateLeftTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_DownStateLeftTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT],
							 (float)(int)GetDlgItemInt(IDC_EDIT_DownStateLeftTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DownStateLeftTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DownStateLeftTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditDownStateLeftTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT] || m_pButton->m_visibleState != MENU_BUTTONSTATE_DOWN)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_DOWN;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_LEFT]);
}

void CEditButton::OnChangeDownStateMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_DOWN;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_DownStateMiddleTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_DownStateMiddleTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_DownStateMiddleTextureWidth));*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_DownStateMiddleTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DownStateMiddleTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DownStateMiddleTextureWidth));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditDownStateMiddleTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE] || m_pButton->m_visibleState != MENU_BUTTONSTATE_DOWN)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]					= new CMenuImage();
		
		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_DOWN;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditButton::OnChangeDownStateRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]					 = new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_DOWN;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT],
							 (int)GetDlgItemInt(IDC_EDIT_DownStateRightTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_DownStateRightTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_DownStateRightTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT],
							 (float)(int)GetDlgItemInt(IDC_EDIT_DownStateRightTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DownStateRightTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DownStateRightTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditDownStateRightTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT] || m_pButton->m_visibleState != MENU_BUTTONSTATE_DOWN)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]					 = new CMenuImage();
		
		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_DOWN;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_RIGHT]);
}

void CEditButton::OnChangeDisabledStateLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]					= new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT],
							 (int)GetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT],
							 (float)(int)GetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DisabledStateLeftTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditDisabledStateLeftTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT] || m_pButton->m_visibleState != MENU_BUTTONSTATE_DISABLED)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]					= new CMenuImage();
		
		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_LEFT]);
}

void CEditButton::OnChangeDisabledStateMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]					= new CMenuImage();
		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureWidth));*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DisabledStateMiddleTextureWidth));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditDisabledStateMiddleTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE] || m_pButton->m_visibleState != MENU_BUTTONSTATE_DISABLED)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]					= new CMenuImage();
		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditButton::OnChangeDisabledStateRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT])
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]					 = new CMenuImage();
		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data
	}

	m_pButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;

	/*UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT],
							 (int)GetDlgItemInt(IDC_EDIT_DisabledStateRightTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_DisabledStateRightTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_DisabledStateRightTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT],
							 (float)(int)GetDlgItemInt(IDC_EDIT_DisabledStateRightTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DisabledStateRightTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_DisabledStateRightTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditButton::OnButtonEditDisabledStateRightTexture()
{
	if(!m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT] || m_pButton->m_visibleState != MENU_BUTTONSTATE_DISABLED)
	{
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]					 = new CMenuImage();

		/*m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/
		m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]->m_textureHeight = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		m_pButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;
		return;
	}

	EditTexture(m_pButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_RIGHT]);
}

void CEditButton::OnButtonEditClickScript()
{
	if(!m_pButton->m_clickScript)
	{
		m_pButton->m_clickScript					= new CMenuScript();
		m_pButton->m_clickScript->m_callingVisual	= (CMenuVisual*)m_pButton;
	}

	CEditScript dlg;
	dlg.m_script = m_pButton->m_clickScript;
	dlg.DoModal();
}

void CEditButton::OnButtonEditMouseOverScript()
{
	if(!m_pButton->m_mouseOverScript)
	{
		m_pButton->m_mouseOverScript					= new CMenuScript();
		m_pButton->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)m_pButton;
	}

	CEditScript dlg;
	dlg.m_script = m_pButton->m_mouseOverScript;
	dlg.DoModal();
}

void CEditButton::OnButtonEditMouseOutScript()
{
	if(!m_pButton->m_mouseOutScript)
	{
		m_pButton->m_mouseOutScript					 = new CMenuScript();
		m_pButton->m_mouseOutScript->m_callingVisual = (CMenuVisual*)m_pButton;
	}

	CEditScript dlg;
	dlg.m_script = m_pButton->m_mouseOutScript;
	dlg.DoModal();
}

void CEditButton::OnButtonEditDropScript()
{
	if(!m_pButton->m_dropScript)
	{
		m_pButton->m_dropScript					 = new CMenuScript();
		m_pButton->m_dropScript->m_callingVisual = (CMenuVisual*)m_pButton;
	}

	CEditScript dlg;
	dlg.m_script = m_pButton->m_dropScript;
	dlg.DoModal();
}

void CEditButton::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pButton->m_name = temp;

	m_pButton->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);

	*m_freezeInput = false;

	CDialog::OnOK();
}

void CEditButton::EditTexture(CMenuImage *pImage)
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

void CEditButton::UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, bool CopyWidth)
{
	pImage->m_texturePosX	= X;
	pImage->m_texturePosY	= Y;
	pImage->m_textureWidth	= Width;

	if(CopyWidth)
		pImage->m_width = Width;

	UpdateImageTextureCoords(pImage);
}

void CEditButton::UpdateImageTextureCoords(CMenuImage *pImage)
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
			//WARNING: C4244 --> conversion from int to float, possible loss of data

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

	m_pButton->Resize();
}

// CEditButton message handlers

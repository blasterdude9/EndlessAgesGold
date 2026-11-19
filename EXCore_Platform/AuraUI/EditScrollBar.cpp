// C:\Projects\EXCore_Platform\AuraUI\EditScrollBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditScrollBar.h"
#include "EditScript.h"

// CEditScrollBar dialog
IMPLEMENT_DYNAMIC(CEditScrollBar, CDialog)

CEditScrollBar::CEditScrollBar(CWnd* pParent /*=NULL*/)
	: CDialog(CEditScrollBar::IDD, pParent)
{
	m_pScrollBar		= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
	m_freezeInput		= NULL;
}

BEGIN_MESSAGE_MAP(CEditScrollBar, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Scroll Bar Coordinates
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)

	//Properties
	ON_BN_CLICKED(IDC_CHECK_Vertical, OnToggleVertical)

	//Background Left Texture
	ON_EN_CHANGE(IDC_EDIT_BackgroundLeftTextureX, OnChangeBackgroundLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundLeftTextureY, OnChangeBackgroundLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundLeftTextureWidth, OnChangeBackgroundLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundLeftTextureHeight, OnChangeBackgroundLeftTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditBackgroundLeftTexture, OnButtonEditBackgroundLeftTexture)

	//Background Middle Texture
	ON_EN_CHANGE(IDC_EDIT_BackgroundMiddleTextureX, OnChangeBackgroundMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundMiddleTextureY, OnChangeBackgroundMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundMiddleTextureWidth, OnChangeBackgroundMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundMiddleTextureHeight, OnChangeBackgroundMiddleTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditBackgroundMiddleTexture, OnButtonEditBackgroundMiddleTexture)

	//Background Right Texture
	ON_EN_CHANGE(IDC_EDIT_BackgroundRightTextureX, OnChangeBackgroundRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundRightTextureY, OnChangeBackgroundRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundRightTextureWidth, OnChangeBackgroundRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_BackgroundRightTextureHeight, OnChangeBackgroundRightTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditBackgroundRightTexture, OnButtonEditBackgroundRightTexture)

	//Slider Left Texture
	ON_EN_CHANGE(IDC_EDIT_SliderLeftTextureX, OnChangeSliderLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_SliderLeftTextureY, OnChangeSliderLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_SliderLeftTextureWidth, OnChangeSliderLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_SliderLeftTextureHeight, OnChangeSliderLeftTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditSliderLeftTexture, OnButtonEditSliderLeftTexture)

	//Slider Middle Texture
	ON_EN_CHANGE(IDC_EDIT_SliderMiddleTextureX, OnChangeSliderMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_SliderMiddleTextureY, OnChangeSliderMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_SliderMiddleTextureWidth, OnChangeSliderMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_SliderMiddleTextureHeight, OnChangeSliderMiddleTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditSliderMiddleTexture, OnButtonEditSliderMiddleTexture)

	//Slider Right Texture
	ON_EN_CHANGE(IDC_EDIT_SliderRightTextureX, OnChangeSliderRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_SliderRightTextureY, OnChangeSliderRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_SliderRightTextureWidth, OnChangeSliderRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_SliderRightTextureHeight, OnChangeSliderRightTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditSliderRightTexture, OnButtonEditSliderRightTexture)

	//Left Button Up State Texture
	ON_EN_CHANGE(IDC_EDIT_LeftUpStateTextureX, OnChangeLeftUpStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftUpStateTextureY, OnChangeLeftUpStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftUpStateTextureWidth, OnChangeLeftUpStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftUpStateTextureHeight, OnChangeLeftUpStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditLeftUpStateTexture, OnButtonEditLeftUpStateTexture)

	//Left Button Hover State Texture
	ON_EN_CHANGE(IDC_EDIT_LeftHoverStateTextureX, OnChangeLeftHoverStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftHoverStateTextureY, OnChangeLeftHoverStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftHoverStateTextureWidth, OnChangeLeftHoverStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftHoverStateTextureHeight, OnChangeLeftHoverStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditLeftHoverStateTexture, OnButtonEditLeftHoverStateTexture)

	//Left Button Down State Texture
	ON_EN_CHANGE(IDC_EDIT_LeftDownStateTextureX, OnChangeLeftDownStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftDownStateTextureY, OnChangeLeftDownStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftDownStateTextureWidth, OnChangeLeftDownStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftDownStateTextureHeight, OnChangeLeftDownStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditLeftDownStateTexture, OnButtonEditLeftDownStateTexture)

	//Left Button Disabled State Texture
	ON_EN_CHANGE(IDC_EDIT_LeftDisabledStateTextureX, OnChangeLeftDisabledStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftDisabledStateTextureY, OnChangeLeftDisabledStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftDisabledStateTextureWidth, OnChangeLeftDisabledStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_LeftDisabledStateTextureHeight, OnChangeLeftDisabledStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditLeftDisabledStateTexture, OnButtonEditLeftDisabledStateTexture)

	//Right Button Up State Texture
	ON_EN_CHANGE(IDC_EDIT_RightUpStateTextureX, OnChangeRightUpStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightUpStateTextureY, OnChangeRightUpStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightUpStateTextureWidth, OnChangeRightUpStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightUpStateTextureHeight, OnChangeRightUpStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditRightUpStateTexture, OnButtonEditRightUpStateTexture)

	//Right Button Hover State Texture
	ON_EN_CHANGE(IDC_EDIT_RightHoverStateTextureX, OnChangeRightHoverStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightHoverStateTextureY, OnChangeRightHoverStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightHoverStateTextureWidth, OnChangeRightHoverStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightHoverStateTextureHeight, OnChangeRightHoverStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditRightHoverStateTexture, OnButtonEditRightHoverStateTexture)

	//Right Button Down State Texture
	ON_EN_CHANGE(IDC_EDIT_RightDownStateTextureX, OnChangeRightDownStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightDownStateTextureY, OnChangeRightDownStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightDownStateTextureWidth, OnChangeRightDownStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightDownStateTextureHeight, OnChangeRightDownStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditRightDownStateTexture, OnButtonEditRightDownStateTexture)

	//Right Button Disabled State Texture
	ON_EN_CHANGE(IDC_EDIT_RightDisabledStateTextureX, OnChangeRightDisabledStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightDisabledStateTextureY, OnChangeRightDisabledStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightDisabledStateTextureWidth, OnChangeRightDisabledStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_RightDisabledStateTextureHeight, OnChangeRightDisabledStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditRightDisabledStateTexture, OnButtonEditRightDisabledStateTexture)

	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditChangeScript, OnButtonEditChangeScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOverScript, OnButtonEditMouseOverScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOutScript, OnButtonEditMouseOutScript)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditScrollBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pScrollBar->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pScrollBar->m_visualID);

	//Scroll Bar Coordinates
	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pScrollBar->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pScrollBar->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, m_pScrollBar->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, m_pScrollBar->m_height);*/

	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)m_pScrollBar->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)m_pScrollBar->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)m_pScrollBar->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)m_pScrollBar->m_height);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data


	//Properties
	CheckDlgButton(IDC_CHECK_Vertical, m_pScrollBar->m_isVertical);

	//Background Left Texture
	if(m_pScrollBar->m_backgroundLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX, m_pScrollBar->m_backgroundLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY, m_pScrollBar->m_backgroundLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth, m_pScrollBar->m_backgroundLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureHeight, m_pScrollBar->m_backgroundLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX, (UINT)m_pScrollBar->m_backgroundLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY, (UINT)m_pScrollBar->m_backgroundLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth, (UINT)m_pScrollBar->m_backgroundLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureHeight, (UINT)m_pScrollBar->m_backgroundLeft->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Middle Texture
	if(m_pScrollBar->m_backgroundMiddle)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX, m_pScrollBar->m_backgroundMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY, m_pScrollBar->m_backgroundMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth, m_pScrollBar->m_backgroundMiddle->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureHeight, m_pScrollBar->m_backgroundMiddle->m_textureHeight);*/
		

		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX, (UINT)m_pScrollBar->m_backgroundMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY, (UINT)m_pScrollBar->m_backgroundMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth, (UINT)m_pScrollBar->m_backgroundMiddle->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureHeight, (UINT)m_pScrollBar->m_backgroundMiddle->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Right Texture
	if(m_pScrollBar->m_backgroundRight)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundRightTextureX, m_pScrollBar->m_backgroundRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureY, m_pScrollBar->m_backgroundRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth, m_pScrollBar->m_backgroundRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureHeight, m_pScrollBar->m_backgroundRight->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureX, (UINT)m_pScrollBar->m_backgroundRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureY, (UINT)m_pScrollBar->m_backgroundRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth, (UINT)m_pScrollBar->m_backgroundRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureHeight, (UINT)m_pScrollBar->m_backgroundRight->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Slider Left Texture
	if(m_pScrollBar->m_sliderLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_SliderLeftTextureX, m_pScrollBar->m_sliderLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_SliderLeftTextureY, m_pScrollBar->m_sliderLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_SliderLeftTextureWidth, m_pScrollBar->m_sliderLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_SliderLeftTextureHeight, m_pScrollBar->m_sliderLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_SliderLeftTextureX, (UINT)m_pScrollBar->m_sliderLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_SliderLeftTextureY, (UINT)m_pScrollBar->m_sliderLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_SliderLeftTextureWidth, (UINT)m_pScrollBar->m_sliderLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_SliderLeftTextureHeight, (UINT)m_pScrollBar->m_sliderLeft->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Slider Middle Texture
	if(m_pScrollBar->m_sliderMiddle)
	{
		/*SetDlgItemInt(IDC_EDIT_SliderMiddleTextureX, m_pScrollBar->m_sliderMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_SliderMiddleTextureY, m_pScrollBar->m_sliderMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_SliderMiddleTextureWidth, m_pScrollBar->m_sliderMiddle->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_SliderMiddleTextureHeight, m_pScrollBar->m_sliderMiddle->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_SliderMiddleTextureX, (UINT)m_pScrollBar->m_sliderMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_SliderMiddleTextureY, (UINT)m_pScrollBar->m_sliderMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_SliderMiddleTextureWidth, (UINT)m_pScrollBar->m_sliderMiddle->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_SliderMiddleTextureHeight, (UINT)m_pScrollBar->m_sliderMiddle->m_textureHeight);		
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Slider Right Texture
	if(m_pScrollBar->m_sliderRight)
	{
		/*SetDlgItemInt(IDC_EDIT_SliderRightTextureX, m_pScrollBar->m_sliderRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_SliderRightTextureY, m_pScrollBar->m_sliderRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_SliderRightTextureWidth, m_pScrollBar->m_sliderRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_SliderRightTextureHeight, m_pScrollBar->m_sliderRight->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_SliderRightTextureX, (UINT)m_pScrollBar->m_sliderRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_SliderRightTextureY, (UINT)m_pScrollBar->m_sliderRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_SliderRightTextureWidth, (UINT)m_pScrollBar->m_sliderRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_SliderRightTextureHeight, (UINT)m_pScrollBar->m_sliderRight->m_textureHeight);		
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}
	
	//Left Button Up State Texture
	if(m_pScrollBar->m_leftButton)
	if(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_LeftUpStateTextureX, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftUpStateTextureY, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftUpStateTextureWidth, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftUpStateTextureHeight, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_LeftUpStateTextureX, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftUpStateTextureY, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftUpStateTextureWidth, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftUpStateTextureHeight, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Left Button Hover State Texture
	if(m_pScrollBar->m_leftButton)
	if(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_LeftHoverStateTextureX, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftHoverStateTextureY, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftHoverStateTextureWidth, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftHoverStateTextureHeight, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_LeftHoverStateTextureX, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftHoverStateTextureY, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftHoverStateTextureWidth, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftHoverStateTextureHeight, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Left Button Down State Texture
	if(m_pScrollBar->m_leftButton)
	if(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_LeftDownStateTextureX, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftDownStateTextureY, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftDownStateTextureWidth, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftDownStateTextureHeight, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_LeftDownStateTextureX, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftDownStateTextureY, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftDownStateTextureWidth, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftDownStateTextureHeight, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Left Button Disabled State Texture
	if(m_pScrollBar->m_leftButton)
	if(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureX, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureY, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureWidth, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureHeight, m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureX, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureY, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureWidth, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureHeight, (UINT)m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Right Button Up State Texture
	if(m_pScrollBar->m_rightButton)
	if(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_RightUpStateTextureX, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightUpStateTextureY, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightUpStateTextureWidth, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightUpStateTextureHeight, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_RightUpStateTextureX, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightUpStateTextureY, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightUpStateTextureWidth, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightUpStateTextureHeight, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Right Button Hover State Texture
	if(m_pScrollBar->m_rightButton)
	if(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_RightHoverStateTextureX, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightHoverStateTextureY, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightHoverStateTextureWidth, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightHoverStateTextureHeight, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_RightHoverStateTextureX, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightHoverStateTextureY, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightHoverStateTextureWidth, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightHoverStateTextureHeight, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Right Button Down State Texture
	if(m_pScrollBar->m_rightButton)
	if(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_RightDownStateTextureX, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightDownStateTextureY, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightDownStateTextureWidth, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightDownStateTextureHeight, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_RightDownStateTextureX, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightDownStateTextureY, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightDownStateTextureWidth, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightDownStateTextureHeight, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Right Button Disabled State Texture
	if(m_pScrollBar->m_rightButton)
	if(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
	{
		/*SetDlgItemInt(IDC_EDIT_RightDisabledStateTextureX, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightDisabledStateTextureY, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightDisabledStateTextureWidth, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightDisabledStateTextureHeight, m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_RightDisabledStateTextureX, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_RightDisabledStateTextureY, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_RightDisabledStateTextureWidth, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_RightDisabledStateTextureHeight, (UINT)m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Allow us to set the buttons' state and not have it overwritten
	*m_freezeInput = true;

	m_initialized = true;

	return TRUE;
}

void CEditScrollBar::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditScrollBar::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pScrollBar->m_posX	= (int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pScrollBar->m_posY	= (int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pScrollBar->m_width	= (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pScrollBar->m_height	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

	m_pScrollBar->m_posX	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pScrollBar->m_posY	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pScrollBar->m_width	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pScrollBar->m_height	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);	
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pScrollBar->Resize();
}

void CEditScrollBar::OnToggleVertical()
{
	m_pScrollBar->m_isVertical = IsDlgButtonChecked(IDC_CHECK_Vertical);
	m_pScrollBar->Resize();
}

void CEditScrollBar::OnChangeBackgroundLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_backgroundLeft)
		m_pScrollBar->m_backgroundLeft = new CMenuImage();

	/*m_pScrollBar->m_backgroundLeft->m_texturePosX	= (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX);
	m_pScrollBar->m_backgroundLeft->m_texturePosY	= (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY);
	m_pScrollBar->m_backgroundLeft->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth);
	m_pScrollBar->m_backgroundLeft->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureHeight);*/

	m_pScrollBar->m_backgroundLeft->m_texturePosX	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX);
	m_pScrollBar->m_backgroundLeft->m_texturePosY	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY);
	m_pScrollBar->m_backgroundLeft->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth);
	m_pScrollBar->m_backgroundLeft->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	if(m_pScrollBar->m_isVertical)
		m_pScrollBar->m_backgroundLeft->m_height = m_pScrollBar->m_backgroundLeft->m_textureHeight;
	else
		m_pScrollBar->m_backgroundLeft->m_width = m_pScrollBar->m_backgroundLeft->m_textureWidth;

	UpdateImageTextureCoords(m_pScrollBar->m_backgroundLeft);
}

void CEditScrollBar::OnButtonEditBackgroundLeftTexture()
{
	if(!m_pScrollBar->m_backgroundLeft)
	{
		m_pScrollBar->m_backgroundLeft = new CMenuImage();
		return;
	}

	EditTexture(m_pScrollBar->m_backgroundLeft);
}

void CEditScrollBar::OnChangeBackgroundMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_backgroundMiddle)
		m_pScrollBar->m_backgroundMiddle = new CMenuImage();

	/*m_pScrollBar->m_backgroundMiddle->m_texturePosX		= (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX);
	m_pScrollBar->m_backgroundMiddle->m_texturePosY		= (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY);
	m_pScrollBar->m_backgroundMiddle->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth);
	m_pScrollBar->m_backgroundMiddle->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureHeight);*/

	m_pScrollBar->m_backgroundMiddle->m_texturePosX		= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX);
	m_pScrollBar->m_backgroundMiddle->m_texturePosY		= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY);
	m_pScrollBar->m_backgroundMiddle->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth);
	m_pScrollBar->m_backgroundMiddle->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
	
	UpdateImageTextureCoords(m_pScrollBar->m_backgroundMiddle);
}

void CEditScrollBar::OnButtonEditBackgroundMiddleTexture()
{
	if(!m_pScrollBar->m_backgroundMiddle)
	{
		m_pScrollBar->m_backgroundMiddle = new CMenuImage();
		return;
	}

	EditTexture(m_pScrollBar->m_backgroundMiddle);
}

void CEditScrollBar::OnChangeBackgroundRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_backgroundRight)
		m_pScrollBar->m_backgroundRight = new CMenuImage();

	/*m_pScrollBar->m_backgroundRight->m_texturePosX	 = (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureX);
	m_pScrollBar->m_backgroundRight->m_texturePosY	 = (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureY);
	m_pScrollBar->m_backgroundRight->m_textureWidth	 = (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth);
	m_pScrollBar->m_backgroundRight->m_textureHeight = (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureHeight);*/

	m_pScrollBar->m_backgroundRight->m_texturePosX	 = (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureX);
	m_pScrollBar->m_backgroundRight->m_texturePosY	 = (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureY);
	m_pScrollBar->m_backgroundRight->m_textureWidth	 = (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth);
	m_pScrollBar->m_backgroundRight->m_textureHeight = (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	if(m_pScrollBar->m_isVertical)
		m_pScrollBar->m_backgroundRight->m_height = m_pScrollBar->m_backgroundRight->m_textureHeight;
	else
		m_pScrollBar->m_backgroundRight->m_width = m_pScrollBar->m_backgroundRight->m_textureWidth;

	UpdateImageTextureCoords(m_pScrollBar->m_backgroundRight);
}

void CEditScrollBar::OnButtonEditBackgroundRightTexture()
{
	if(!m_pScrollBar->m_backgroundRight)
	{
		m_pScrollBar->m_backgroundRight = new CMenuImage();
		return;
	}

	EditTexture(m_pScrollBar->m_backgroundRight);
}

void CEditScrollBar::OnChangeSliderLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_sliderLeft)
		m_pScrollBar->m_sliderLeft = new CMenuImage();

	/*m_pScrollBar->m_sliderLeft->m_texturePosX	= (int)GetDlgItemInt(IDC_EDIT_SliderLeftTextureX);
	m_pScrollBar->m_sliderLeft->m_texturePosY	= (int)GetDlgItemInt(IDC_EDIT_SliderLeftTextureY);
	m_pScrollBar->m_sliderLeft->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_SliderLeftTextureWidth);
	m_pScrollBar->m_sliderLeft->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_SliderLeftTextureHeight);*/

	m_pScrollBar->m_sliderLeft->m_texturePosX	= (float)(int)GetDlgItemInt(IDC_EDIT_SliderLeftTextureX);
	m_pScrollBar->m_sliderLeft->m_texturePosY	= (float)(int)GetDlgItemInt(IDC_EDIT_SliderLeftTextureY);
	m_pScrollBar->m_sliderLeft->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_SliderLeftTextureWidth);
	m_pScrollBar->m_sliderLeft->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_SliderLeftTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	if(m_pScrollBar->m_isVertical)
		m_pScrollBar->m_sliderLeft->m_height = m_pScrollBar->m_sliderLeft->m_textureHeight;
	else
		m_pScrollBar->m_sliderLeft->m_width = m_pScrollBar->m_sliderLeft->m_textureWidth;

	UpdateImageTextureCoords(m_pScrollBar->m_sliderLeft);
	m_pScrollBar->Resize();
}

void CEditScrollBar::OnButtonEditSliderLeftTexture()
{
	if(!m_pScrollBar->m_sliderLeft)
	{
		m_pScrollBar->m_sliderLeft = new CMenuImage();
		return;
	}

	EditTexture(m_pScrollBar->m_sliderLeft);
}

void CEditScrollBar::OnChangeSliderMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_sliderMiddle)
		m_pScrollBar->m_sliderMiddle = new CMenuImage();

	/*m_pScrollBar->m_sliderMiddle->m_texturePosX		= (int)GetDlgItemInt(IDC_EDIT_SliderMiddleTextureX);
	m_pScrollBar->m_sliderMiddle->m_texturePosY		= (int)GetDlgItemInt(IDC_EDIT_SliderMiddleTextureY);
	m_pScrollBar->m_sliderMiddle->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_SliderMiddleTextureWidth);
	m_pScrollBar->m_sliderMiddle->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_SliderMiddleTextureHeight);*/

	m_pScrollBar->m_sliderMiddle->m_texturePosX		= (float)(int)GetDlgItemInt(IDC_EDIT_SliderMiddleTextureX);
	m_pScrollBar->m_sliderMiddle->m_texturePosY		= (float)(int)GetDlgItemInt(IDC_EDIT_SliderMiddleTextureY);
	m_pScrollBar->m_sliderMiddle->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_SliderMiddleTextureWidth);
	m_pScrollBar->m_sliderMiddle->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_SliderMiddleTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
	
	UpdateImageTextureCoords(m_pScrollBar->m_sliderMiddle);
	m_pScrollBar->Resize();
}

void CEditScrollBar::OnButtonEditSliderMiddleTexture()
{
	if(!m_pScrollBar->m_sliderMiddle)
	{
		m_pScrollBar->m_sliderMiddle = new CMenuImage();
		return;
	}

	EditTexture(m_pScrollBar->m_sliderMiddle);
}

void CEditScrollBar::OnChangeSliderRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_sliderRight)
		m_pScrollBar->m_sliderRight = new CMenuImage();

	/*m_pScrollBar->m_sliderRight->m_texturePosX	 = (int)GetDlgItemInt(IDC_EDIT_SliderRightTextureX);
	m_pScrollBar->m_sliderRight->m_texturePosY	 = (int)GetDlgItemInt(IDC_EDIT_SliderRightTextureY);
	m_pScrollBar->m_sliderRight->m_textureWidth	 = (int)GetDlgItemInt(IDC_EDIT_SliderRightTextureWidth);
	m_pScrollBar->m_sliderRight->m_textureHeight = (int)GetDlgItemInt(IDC_EDIT_SliderRightTextureHeight);*/

	m_pScrollBar->m_sliderRight->m_texturePosX	 = (float)(int)GetDlgItemInt(IDC_EDIT_SliderRightTextureX);
	m_pScrollBar->m_sliderRight->m_texturePosY	 = (float)(int)GetDlgItemInt(IDC_EDIT_SliderRightTextureY);
	m_pScrollBar->m_sliderRight->m_textureWidth	 = (float)(int)GetDlgItemInt(IDC_EDIT_SliderRightTextureWidth);
	m_pScrollBar->m_sliderRight->m_textureHeight = (float)(int)GetDlgItemInt(IDC_EDIT_SliderRightTextureHeight);	
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	if(m_pScrollBar->m_isVertical)
		m_pScrollBar->m_sliderRight->m_height = m_pScrollBar->m_sliderRight->m_textureHeight;
	else
		m_pScrollBar->m_sliderRight->m_width = m_pScrollBar->m_sliderRight->m_textureWidth;

	UpdateImageTextureCoords(m_pScrollBar->m_sliderRight);
	m_pScrollBar->Resize();
}

void CEditScrollBar::OnButtonEditSliderRightTexture()
{
	if(!m_pScrollBar->m_sliderRight)
	{
		m_pScrollBar->m_sliderRight = new CMenuImage();
		return;
	}

	EditTexture(m_pScrollBar->m_sliderRight);
}

void CEditScrollBar::OnChangeLeftUpStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_leftButton)
		m_pScrollBar->m_leftButton = new CMenuButton();

	m_pScrollBar->m_leftButton->m_visibleState = MENU_BUTTONSTATE_UP;

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
		m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();

	/*m_pScrollBar->m_leftButton->m_width	 = (int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureWidth);
	m_pScrollBar->m_leftButton->m_height = (int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureHeight);*/

	m_pScrollBar->m_leftButton->m_width	 = (float)(int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureWidth);
	m_pScrollBar->m_leftButton->m_height = (float)(int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	/*UpdateImageTextureCoords(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureHeight));*/

	UpdateImageTextureCoords(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftUpStateTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditScrollBar::OnButtonEditLeftUpStateTexture()
{
	if(!m_pScrollBar->m_leftButton)
		m_pScrollBar->m_leftButton = new CMenuButton();

	m_pScrollBar->m_leftButton->m_visibleState = MENU_BUTTONSTATE_UP;

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();
		return;
	}

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_imageMesh)
		return;

	EditTexture(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditScrollBar::OnChangeLeftHoverStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_leftButton)
		m_pScrollBar->m_leftButton = new CMenuButton();

	m_pScrollBar->m_leftButton->m_visibleState = MENU_BUTTONSTATE_HOVER;

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
		m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();

	/*UpdateImageTextureCoords(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_LeftHoverStateTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_LeftHoverStateTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_LeftHoverStateTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_LeftHoverStateTextureHeight));*/

	UpdateImageTextureCoords(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftHoverStateTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftHoverStateTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftHoverStateTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftHoverStateTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditScrollBar::OnButtonEditLeftHoverStateTexture()
{
	if(!m_pScrollBar->m_leftButton)
		m_pScrollBar->m_leftButton = new CMenuButton();

	m_pScrollBar->m_leftButton->m_visibleState = MENU_BUTTONSTATE_HOVER;

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();
		return;
	}

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_imageMesh)
		return;

	EditTexture(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditScrollBar::OnChangeLeftDownStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_leftButton)
		m_pScrollBar->m_leftButton = new CMenuButton();

	m_pScrollBar->m_leftButton->m_visibleState = MENU_BUTTONSTATE_DOWN;

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
		m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();

	//UpdateImageTextureCoords(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE],
	//						 (int)GetDlgItemInt(IDC_EDIT_LeftDownStateTextureX),
	//						 (int)GetDlgItemInt(IDC_EDIT_LeftDownStateTextureY),
	//						 (int)GetDlgItemInt(IDC_EDIT_LeftDownStateTextureWidth),
	//						 (int)GetDlgItemInt(IDC_EDIT_LeftDownStateTextureHeight));

	UpdateImageTextureCoords(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftDownStateTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftDownStateTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftDownStateTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftDownStateTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditScrollBar::OnButtonEditLeftDownStateTexture()
{
	if(!m_pScrollBar->m_leftButton)
		m_pScrollBar->m_leftButton = new CMenuButton();

	m_pScrollBar->m_leftButton->m_visibleState = MENU_BUTTONSTATE_DOWN;

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();
		return;
	}

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_imageMesh)
		return;

	EditTexture(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditScrollBar::OnChangeLeftDisabledStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_leftButton)
		m_pScrollBar->m_leftButton = new CMenuButton();

	m_pScrollBar->m_leftButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
		m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();

	/*UpdateImageTextureCoords(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureHeight));*/

	UpdateImageTextureCoords(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_LeftDisabledStateTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditScrollBar::OnButtonEditLeftDisabledStateTexture()
{
	if(!m_pScrollBar->m_leftButton)
		m_pScrollBar->m_leftButton = new CMenuButton();

	m_pScrollBar->m_leftButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();
		return;
	}

	if(!m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_imageMesh)
		return;

	EditTexture(m_pScrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditScrollBar::OnChangeRightUpStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_rightButton)
		m_pScrollBar->m_rightButton = new CMenuButton();

	m_pScrollBar->m_rightButton->m_visibleState = MENU_BUTTONSTATE_UP;

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
		m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();

	/*m_pScrollBar->m_rightButton->m_width	= (int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureWidth);
	m_pScrollBar->m_rightButton->m_height	= (int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureHeight);*/

	m_pScrollBar->m_rightButton->m_width	= (float)(int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureWidth);
	m_pScrollBar->m_rightButton->m_height	= (float)(int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureHeight);	
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	/*UpdateImageTextureCoords(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureHeight));*/

	UpdateImageTextureCoords(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightUpStateTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditScrollBar::OnButtonEditRightUpStateTexture()
{
	if(!m_pScrollBar->m_rightButton)
		m_pScrollBar->m_rightButton = new CMenuButton();

	m_pScrollBar->m_rightButton->m_visibleState = MENU_BUTTONSTATE_UP;

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();
		return;
	}

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_imageMesh)
		return;

	EditTexture(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditScrollBar::OnChangeRightHoverStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_rightButton)
		m_pScrollBar->m_rightButton = new CMenuButton();

	m_pScrollBar->m_rightButton->m_visibleState = MENU_BUTTONSTATE_HOVER;

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
		m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();

	/*UpdateImageTextureCoords(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_RightHoverStateTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_RightHoverStateTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_RightHoverStateTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_RightHoverStateTextureHeight));*/

	UpdateImageTextureCoords(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightHoverStateTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightHoverStateTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightHoverStateTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightHoverStateTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditScrollBar::OnButtonEditRightHoverStateTexture()
{
	if(!m_pScrollBar->m_rightButton)
		m_pScrollBar->m_rightButton = new CMenuButton();

	m_pScrollBar->m_rightButton->m_visibleState = MENU_BUTTONSTATE_HOVER;

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();
		return;
	}

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_imageMesh)
		return;

	EditTexture(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditScrollBar::OnChangeRightDownStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_rightButton)
		m_pScrollBar->m_rightButton = new CMenuButton();

	m_pScrollBar->m_rightButton->m_visibleState = MENU_BUTTONSTATE_DOWN;

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
		m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();

	/*UpdateImageTextureCoords(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_RightDownStateTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_RightDownStateTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_RightDownStateTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_RightDownStateTextureHeight));*/

	UpdateImageTextureCoords(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightDownStateTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightDownStateTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightDownStateTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightDownStateTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditScrollBar::OnButtonEditRightDownStateTexture()
{
	if(!m_pScrollBar->m_rightButton)
		m_pScrollBar->m_rightButton = new CMenuButton();

	m_pScrollBar->m_rightButton->m_visibleState = MENU_BUTTONSTATE_DOWN;

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();
		return;
	}

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_imageMesh)
		return;

	EditTexture(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]);
}

void CEditScrollBar::OnChangeRightDisabledStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pScrollBar->m_rightButton)
		m_pScrollBar->m_rightButton = new CMenuButton();

	m_pScrollBar->m_rightButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
		m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();

	/*UpdateImageTextureCoords(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE],
							 (int)GetDlgItemInt(IDC_EDIT_RightDisabledStateTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_RightDisabledStateTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_RightDisabledStateTextureWidth),
							 (int)GetDlgItemInt(IDC_EDIT_RightDisabledStateTextureHeight));*/

	UpdateImageTextureCoords(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE],
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightDisabledStateTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightDisabledStateTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightDisabledStateTextureWidth),
							 (float)(int)GetDlgItemInt(IDC_EDIT_RightDisabledStateTextureHeight));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditScrollBar::OnButtonEditRightDisabledStateTexture()
{
	if(!m_pScrollBar->m_rightButton)
		m_pScrollBar->m_rightButton = new CMenuButton();

	m_pScrollBar->m_rightButton->m_visibleState = MENU_BUTTONSTATE_DISABLED;

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
	{
		m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE] = new CMenuImage();
		return;
	}

	if(!m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_imageMesh)
		return;

	EditTexture(m_pScrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]);
}

//TODO: Remove These
	void CEditScrollBar::OnChangeNumItems()
	{
		m_pScrollBar->m_numItems = (int)GetDlgItemInt(IDC_EDIT_NumItems);
		m_pScrollBar->Update();
	}

	void CEditScrollBar::OnChangeNumVisibleItems()
	{
		m_pScrollBar->m_visibleItems = (int)GetDlgItemInt(IDC_EDIT_NumVisibleItems);
		m_pScrollBar->Update();
	}
//END TODO

void CEditScrollBar::OnButtonEditChangeScript()
{
	if(!m_pScrollBar->m_changeScript)
	{
		m_pScrollBar->m_changeScript					= new CMenuScript();
		m_pScrollBar->m_changeScript->m_callingVisual	= (CMenuVisual*)m_pScrollBar;
	}

	CEditScript dlg;
	dlg.m_script = m_pScrollBar->m_changeScript;
	dlg.DoModal();
}

void CEditScrollBar::OnButtonEditMouseOverScript()
{
	if(!m_pScrollBar->m_mouseOverScript)
	{
		m_pScrollBar->m_mouseOverScript					= new CMenuScript();
		m_pScrollBar->m_mouseOverScript->m_callingVisual = (CMenuVisual*)m_pScrollBar;
	}

	CEditScript dlg;
	dlg.m_script = m_pScrollBar->m_mouseOverScript;
	dlg.DoModal();
}

void CEditScrollBar::OnButtonEditMouseOutScript()
{
	if(!m_pScrollBar->m_mouseOutScript)
	{
		m_pScrollBar->m_mouseOutScript					= new CMenuScript();
		m_pScrollBar->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)m_pScrollBar;
	}

	CEditScript dlg;
	dlg.m_script = m_pScrollBar->m_mouseOutScript;
	dlg.DoModal();
}

void CEditScrollBar::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pScrollBar->m_name = temp;

	m_pScrollBar->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);

	*m_freezeInput = false;

	CDialog::OnOK();
}

void CEditScrollBar::EditTexture(CMenuImage *pImage)
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

void CEditScrollBar::UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, float Height)
{
	pImage->m_texturePosX	= X;
	pImage->m_texturePosY	= Y;
	pImage->m_textureWidth	= Width;
	pImage->m_textureHeight	= Height;
	pImage->m_width			= Width;
	pImage->m_height		= Height;

	UpdateImageTextureCoords(pImage);
}

void CEditScrollBar::UpdateImageTextureCoords(CMenuImage *pImage)
{
	if(pImage->m_imageMesh)
	{
		//Get the texture dimensions
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

	m_pScrollBar->Resize();
}

// CEditScrollBar message handlers

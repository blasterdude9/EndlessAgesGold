// C:\Projects\EXCore_Platform\AuraUI\EditProgBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditProgBar.h"
#include "EditScript.h"

// CEditProgBar dialog
IMPLEMENT_DYNAMIC(CEditProgBar, CDialog)

CEditProgBar::CEditProgBar(CWnd* pParent /*=NULL*/)
	: CDialog(CEditProgBar::IDD, pParent)
{
	m_pProgressBar		= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
}

BEGIN_MESSAGE_MAP(CEditProgBar, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Progress Bar Coordinates
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)

	//Properties
	ON_BN_CLICKED(IDC_CHECK_Vertical, OnToggleVertical)
	ON_BN_CLICKED(IDC_CHECK_Flipped, OnToggleFlipped)

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

	//Foreground Left Texture
	ON_EN_CHANGE(IDC_EDIT_ForegroundLeftTextureX, OnChangeForegroundLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ForegroundLeftTextureY, OnChangeForegroundLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ForegroundLeftTextureWidth, OnChangeForegroundLeftTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ForegroundLeftTextureHeight, OnChangeForegroundLeftTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditForegroundLeftTexture, OnButtonEditForegroundLeftTexture)

	//Foreground Middle Texture
	ON_EN_CHANGE(IDC_EDIT_ForegroundMiddleTextureX, OnChangeForegroundMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ForegroundMiddleTextureY, OnChangeForegroundMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ForegroundMiddleTextureWidth, OnChangeForegroundMiddleTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ForegroundMiddleTextureHeight, OnChangeForegroundMiddleTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditForegroundMiddleTexture, OnButtonEditForegroundMiddleTexture)

	//Foreground Right Texture
	ON_EN_CHANGE(IDC_EDIT_ForegroundRightTextureX, OnChangeForegroundRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ForegroundRightTextureY, OnChangeForegroundRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ForegroundRightTextureWidth, OnChangeForegroundRightTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ForegroundRightTextureHeight, OnChangeForegroundRightTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditForegroundRightTexture, OnButtonEditForegroundRightTexture)

	//Foreground Inset
	ON_EN_CHANGE(IDC_EDIT_ForegroundInset, OnChangeForegroundInset)

	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOverScript, OnButtonEditMouseOverScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOutScript, OnButtonEditMouseOutScript)
	ON_BN_CLICKED(IDC_BUTTON_EditUpdateScript, OnButtonEditUpdateScript)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditProgBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pProgressBar->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pProgressBar->m_visualID);

	//Progress Bar Coordinates
	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pProgressBar->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pProgressBar->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, m_pProgressBar->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, m_pProgressBar->m_height);*/

	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)m_pProgressBar->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)m_pProgressBar->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)m_pProgressBar->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)m_pProgressBar->m_height);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Properties
	CheckDlgButton(IDC_CHECK_Vertical, m_pProgressBar->m_isVertical);
	CheckDlgButton(IDC_CHECK_Flipped, m_pProgressBar->m_isFlipped);

	//Background Left Texture
	if(m_pProgressBar->m_backgroundLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX, m_pProgressBar->m_backgroundLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY, m_pProgressBar->m_backgroundLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth, m_pProgressBar->m_backgroundLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureHeight, m_pProgressBar->m_backgroundLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX, (UINT)m_pProgressBar->m_backgroundLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY, (UINT)m_pProgressBar->m_backgroundLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth, (UINT)m_pProgressBar->m_backgroundLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureHeight, (UINT)m_pProgressBar->m_backgroundLeft->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Middle Texture
	if(m_pProgressBar->m_backgroundMiddle)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX, m_pProgressBar->m_backgroundMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY, m_pProgressBar->m_backgroundMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth, m_pProgressBar->m_backgroundMiddle->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureHeight, m_pProgressBar->m_backgroundMiddle->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX, (UINT)m_pProgressBar->m_backgroundMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY, (UINT)m_pProgressBar->m_backgroundMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth, (UINT)m_pProgressBar->m_backgroundMiddle->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureHeight, (UINT)m_pProgressBar->m_backgroundMiddle->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Right Texture
	if(m_pProgressBar->m_backgroundRight)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundRightTextureX, m_pProgressBar->m_backgroundRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureY, m_pProgressBar->m_backgroundRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth, m_pProgressBar->m_backgroundRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureHeight, m_pProgressBar->m_backgroundRight->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureX, (UINT)m_pProgressBar->m_backgroundRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureY, (UINT)m_pProgressBar->m_backgroundRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth, (UINT)m_pProgressBar->m_backgroundRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureHeight, (UINT)m_pProgressBar->m_backgroundRight->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Foreground Left Texture
	if(m_pProgressBar->m_foregroundLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_ForegroundLeftTextureX, m_pProgressBar->m_foregroundLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_ForegroundLeftTextureY, m_pProgressBar->m_foregroundLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_ForegroundLeftTextureWidth, m_pProgressBar->m_foregroundLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_ForegroundLeftTextureHeight, m_pProgressBar->m_foregroundLeft->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_ForegroundLeftTextureX, (UINT)m_pProgressBar->m_foregroundLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_ForegroundLeftTextureY, (UINT)m_pProgressBar->m_foregroundLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_ForegroundLeftTextureWidth, (UINT)m_pProgressBar->m_foregroundLeft->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_ForegroundLeftTextureHeight, (UINT)m_pProgressBar->m_foregroundLeft->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Foreground Middle Texture
	if(m_pProgressBar->m_foregroundMiddle)
	{
		/*SetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureX, m_pProgressBar->m_foregroundMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureY, m_pProgressBar->m_foregroundMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureWidth, m_pProgressBar->m_foregroundMiddle->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureHeight, m_pProgressBar->m_foregroundMiddle->m_textureHeight);*/


		SetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureX, (UINT)m_pProgressBar->m_foregroundMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureY, (UINT)m_pProgressBar->m_foregroundMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureWidth, (UINT)m_pProgressBar->m_foregroundMiddle->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureHeight, (UINT)m_pProgressBar->m_foregroundMiddle->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Foreground Right Texture
	if(m_pProgressBar->m_foregroundRight)
	{
		/*SetDlgItemInt(IDC_EDIT_ForegroundRightTextureX, m_pProgressBar->m_foregroundRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_ForegroundRightTextureY, m_pProgressBar->m_foregroundRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_ForegroundRightTextureWidth, m_pProgressBar->m_foregroundRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_ForegroundRightTextureHeight, m_pProgressBar->m_foregroundRight->m_textureHeight);*/

		SetDlgItemInt(IDC_EDIT_ForegroundRightTextureX, (UINT)m_pProgressBar->m_foregroundRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_ForegroundRightTextureY, (UINT)m_pProgressBar->m_foregroundRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_ForegroundRightTextureWidth, (UINT)m_pProgressBar->m_foregroundRight->m_textureWidth);
		SetDlgItemInt(IDC_EDIT_ForegroundRightTextureHeight, (UINT)m_pProgressBar->m_foregroundRight->m_textureHeight);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Foreground Inset
	/*SetDlgItemInt(IDC_EDIT_ForegroundInset, m_pProgressBar->m_foregroundInset);*/

	SetDlgItemInt(IDC_EDIT_ForegroundInset, (UINT)m_pProgressBar->m_foregroundInset);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	m_initialized = true;

	return TRUE;
}

void CEditProgBar::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditProgBar::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pProgressBar->m_posX	 = (int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pProgressBar->m_posY	 = (int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pProgressBar->m_width	 = (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pProgressBar->m_height = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

	m_pProgressBar->m_posX	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pProgressBar->m_posY	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pProgressBar->m_width	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pProgressBar->m_height = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pProgressBar->Resize();
}

void CEditProgBar::OnToggleVertical()
{
	m_pProgressBar->m_isVertical = IsDlgButtonChecked(IDC_CHECK_Vertical);
	m_pProgressBar->Resize();
}

void CEditProgBar::OnToggleFlipped()
{
	m_pProgressBar->m_isFlipped = IsDlgButtonChecked(IDC_CHECK_Flipped);
	m_pProgressBar->Resize();
}

void CEditProgBar::OnChangeBackgroundLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pProgressBar->m_backgroundLeft)
		m_pProgressBar->m_backgroundLeft = new CMenuImage();

	/*m_pProgressBar->m_backgroundLeft->m_texturePosX		= (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX);
	m_pProgressBar->m_backgroundLeft->m_texturePosY		= (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY);
	m_pProgressBar->m_backgroundLeft->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth);
	m_pProgressBar->m_backgroundLeft->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureHeight);*/

	m_pProgressBar->m_backgroundLeft->m_texturePosX		= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX);
	m_pProgressBar->m_backgroundLeft->m_texturePosY		= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY);
	m_pProgressBar->m_backgroundLeft->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth);
	m_pProgressBar->m_backgroundLeft->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	if(m_pProgressBar->m_isVertical)
		m_pProgressBar->m_backgroundLeft->m_height = m_pProgressBar->m_backgroundLeft->m_textureHeight;
	else
		m_pProgressBar->m_backgroundLeft->m_width = m_pProgressBar->m_backgroundLeft->m_textureWidth;

	UpdateImageTextureCoords(m_pProgressBar->m_backgroundLeft);
}

void CEditProgBar::OnButtonEditBackgroundLeftTexture()
{
	if(!m_pProgressBar->m_backgroundLeft)
	{
		m_pProgressBar->m_backgroundLeft = new CMenuImage();
		return;
	}

	EditTexture(m_pProgressBar->m_backgroundLeft);
}

void CEditProgBar::OnChangeBackgroundMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	/*m_pProgressBar->m_backgroundMiddle->m_texturePosX	= (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX);
	m_pProgressBar->m_backgroundMiddle->m_texturePosY	= (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY);
	m_pProgressBar->m_backgroundMiddle->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth);
	m_pProgressBar->m_backgroundMiddle->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureHeight);*/

	m_pProgressBar->m_backgroundMiddle->m_texturePosX	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX);
	m_pProgressBar->m_backgroundMiddle->m_texturePosY	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY);
	m_pProgressBar->m_backgroundMiddle->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth);
	m_pProgressBar->m_backgroundMiddle->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
	
	UpdateImageTextureCoords(m_pProgressBar->m_backgroundMiddle);
}

void CEditProgBar::OnButtonEditBackgroundMiddleTexture()
{
	EditTexture(m_pProgressBar->m_backgroundMiddle);
}

void CEditProgBar::OnChangeBackgroundRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pProgressBar->m_backgroundRight)
		m_pProgressBar->m_backgroundRight = new CMenuImage();

	/*m_pProgressBar->m_backgroundRight->m_texturePosX	= (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureX);
	m_pProgressBar->m_backgroundRight->m_texturePosY	= (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureY);
	m_pProgressBar->m_backgroundRight->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth);
	m_pProgressBar->m_backgroundRight->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureHeight);*/

	m_pProgressBar->m_backgroundRight->m_texturePosX	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureX);
	m_pProgressBar->m_backgroundRight->m_texturePosY	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureY);
	m_pProgressBar->m_backgroundRight->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth);
	m_pProgressBar->m_backgroundRight->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	if(m_pProgressBar->m_isVertical)
		m_pProgressBar->m_backgroundRight->m_height = m_pProgressBar->m_backgroundRight->m_textureHeight;
	else
		m_pProgressBar->m_backgroundRight->m_width = m_pProgressBar->m_backgroundRight->m_textureWidth;

	UpdateImageTextureCoords(m_pProgressBar->m_backgroundRight);
}

void CEditProgBar::OnButtonEditBackgroundRightTexture()
{
	if(!m_pProgressBar->m_backgroundRight)
	{
		m_pProgressBar->m_backgroundRight = new CMenuImage();
		return;
	}

	EditTexture(m_pProgressBar->m_backgroundRight);
}

void CEditProgBar::OnChangeForegroundLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pProgressBar->m_foregroundLeft)
		m_pProgressBar->m_foregroundLeft = new CMenuImage();

	/*m_pProgressBar->m_foregroundLeft->m_texturePosX		= (int)GetDlgItemInt(IDC_EDIT_ForegroundLeftTextureX);
	m_pProgressBar->m_foregroundLeft->m_texturePosY		= (int)GetDlgItemInt(IDC_EDIT_ForegroundLeftTextureY);
	m_pProgressBar->m_foregroundLeft->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_ForegroundLeftTextureWidth);
	m_pProgressBar->m_foregroundLeft->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_ForegroundLeftTextureHeight);*/

	m_pProgressBar->m_foregroundLeft->m_texturePosX		= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundLeftTextureX);
	m_pProgressBar->m_foregroundLeft->m_texturePosY		= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundLeftTextureY);
	m_pProgressBar->m_foregroundLeft->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundLeftTextureWidth);
	m_pProgressBar->m_foregroundLeft->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundLeftTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
	
	if(m_pProgressBar->m_isVertical)
	{
		m_pProgressBar->m_foregroundLeft->m_height	= m_pProgressBar->m_foregroundLeft->m_textureHeight;
		m_pProgressBar->m_foregroundLeftSize		= m_pProgressBar->m_foregroundLeft->m_textureHeight;
	}
	else
	{
		m_pProgressBar->m_foregroundLeft->m_width	= m_pProgressBar->m_foregroundLeft->m_textureWidth;
		m_pProgressBar->m_foregroundLeftSize		= m_pProgressBar->m_foregroundLeft->m_textureWidth;
	}

	UpdateImageTextureCoords(m_pProgressBar->m_foregroundLeft);
}

void CEditProgBar::OnButtonEditForegroundLeftTexture()
{
	if(!m_pProgressBar->m_foregroundLeft)
	{
		m_pProgressBar->m_foregroundLeft = new CMenuImage();
		return;
	}

	EditTexture(m_pProgressBar->m_foregroundLeft);
}

void CEditProgBar::OnChangeForegroundMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	/*m_pProgressBar->m_foregroundMiddle->m_texturePosX	= (int)GetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureX);
	m_pProgressBar->m_foregroundMiddle->m_texturePosY	= (int)GetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureY);
	m_pProgressBar->m_foregroundMiddle->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureWidth);
	m_pProgressBar->m_foregroundMiddle->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureHeight);*/

	m_pProgressBar->m_foregroundMiddle->m_texturePosX	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureX);
	m_pProgressBar->m_foregroundMiddle->m_texturePosY	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureY);
	m_pProgressBar->m_foregroundMiddle->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureWidth);
	m_pProgressBar->m_foregroundMiddle->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundMiddleTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
	
	UpdateImageTextureCoords(m_pProgressBar->m_foregroundMiddle);
}

void CEditProgBar::OnButtonEditForegroundMiddleTexture()
{
	EditTexture(m_pProgressBar->m_foregroundMiddle);
}

void CEditProgBar::OnChangeForegroundRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pProgressBar->m_foregroundRight)
		m_pProgressBar->m_foregroundRight = new CMenuImage();

	/*m_pProgressBar->m_foregroundRight->m_texturePosX	= (int)GetDlgItemInt(IDC_EDIT_ForegroundRightTextureX);
	m_pProgressBar->m_foregroundRight->m_texturePosY	= (int)GetDlgItemInt(IDC_EDIT_ForegroundRightTextureY);
	m_pProgressBar->m_foregroundRight->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_ForegroundRightTextureWidth);
	m_pProgressBar->m_foregroundRight->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_ForegroundRightTextureHeight);*/

	m_pProgressBar->m_foregroundRight->m_texturePosX	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundRightTextureX);
	m_pProgressBar->m_foregroundRight->m_texturePosY	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundRightTextureY);
	m_pProgressBar->m_foregroundRight->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundRightTextureWidth);
	m_pProgressBar->m_foregroundRight->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundRightTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	if(m_pProgressBar->m_isVertical)
	{
		m_pProgressBar->m_foregroundRight->m_height	= m_pProgressBar->m_foregroundRight->m_textureHeight;
		m_pProgressBar->m_foregroundRightSize		= m_pProgressBar->m_foregroundRight->m_textureHeight;
	}
	else
	{
		m_pProgressBar->m_foregroundRight->m_width	= m_pProgressBar->m_foregroundRight->m_textureWidth;
		m_pProgressBar->m_foregroundRightSize		= m_pProgressBar->m_foregroundRight->m_textureWidth;
	}

	UpdateImageTextureCoords(m_pProgressBar->m_foregroundRight);
}

void CEditProgBar::OnButtonEditForegroundRightTexture()
{
	if(!m_pProgressBar->m_foregroundRight)
	{
		m_pProgressBar->m_foregroundRight = new CMenuImage();
		return;
	}

	EditTexture(m_pProgressBar->m_foregroundRight);
}

void CEditProgBar::OnChangeForegroundInset()
{
	if(!m_initialized)
		return;

	/*m_pProgressBar->m_foregroundInset = (int)GetDlgItemInt(IDC_EDIT_ForegroundInset);*/
	m_pProgressBar->m_foregroundInset = (float)(int)GetDlgItemInt(IDC_EDIT_ForegroundInset);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pProgressBar->Resize();
}

void CEditProgBar::OnButtonEditMouseOverScript()
{
	if(!m_pProgressBar->m_mouseOverScript)
	{
		m_pProgressBar->m_mouseOverScript					= new CMenuScript();
		m_pProgressBar->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)m_pProgressBar;
	}

	CEditScript dlg;
	dlg.m_script = m_pProgressBar->m_mouseOverScript;
	dlg.DoModal();
}

void CEditProgBar::OnButtonEditMouseOutScript()
{
	if(!m_pProgressBar->m_mouseOutScript)
	{
		m_pProgressBar->m_mouseOutScript					= new CMenuScript();
		m_pProgressBar->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)m_pProgressBar;
	}

	CEditScript dlg;
	dlg.m_script = m_pProgressBar->m_mouseOutScript;
	dlg.DoModal();
}

void CEditProgBar::OnButtonEditUpdateScript()
{
	if(!m_pProgressBar->m_updateScript)
	{
		m_pProgressBar->m_updateScript					= new CMenuScript();
		m_pProgressBar->m_updateScript->m_callingVisual	= (CMenuVisual*)m_pProgressBar;
	}

	CEditScript dlg;
	dlg.m_script = m_pProgressBar->m_updateScript;
	dlg.DoModal();
}

void CEditProgBar::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pProgressBar->m_name = temp;

	m_pProgressBar->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);

	CDialog::OnOK();
}

void CEditProgBar::EditTexture(CMenuImage *pImage)
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

void CEditProgBar::UpdateImageTextureCoords(CMenuImage *pImage)
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

	m_pProgressBar->Resize();
}

// CEditProgBar message handlers

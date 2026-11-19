// C:\Projects\EXCore_Platform\AuraUI\EditMenuTitle.cpp : implementation file
//

#include "stdafx.h"
#include "EditMenuTitle.h"

// CEditMenuTitle dialog
IMPLEMENT_DYNAMIC(CEditMenuTitle, CDialog)

CEditMenuTitle::CEditMenuTitle(CWnd* pParent /*=NULL*/)
	: CDialog(CEditMenuTitle::IDD, pParent)
{
	m_pMenu				= NULL;
	m_pFontList			= NULL;
	m_pTextureDatabase	= NULL;
	m_initialized		= false;
}

BEGIN_MESSAGE_MAP(CEditMenuTitle, CDialog)
	//Text Editing
	ON_EN_CHANGE(IDC_EDIT_Text, OnChangeText)
	ON_CBN_SELCHANGE(IDC_COMBO_Font, OnChangeFont)
	ON_EN_CHANGE(IDC_EDIT_ColorRed, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorGreen, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorBlue, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorAlpha, OnChangeColor)

	//Title Coordinates
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)

	//Text Coordinates
	ON_EN_CHANGE(IDC_EDIT_TextX, OnChangeTextCoords)
	ON_EN_CHANGE(IDC_EDIT_TextY, OnChangeTextCoords)

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

	//Properties
	ON_BN_CLICKED(IDC_CHECK_StretchTitle, OnToggleStretchTitle)
	ON_BN_CLICKED(IDC_CHECK_DragByTitleOnly, OnToggleDragByTitleOnly)

	//Regular Buttons
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditMenuTitle::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Fill the font list
	for(POSITION posCurFont = m_pFontList->GetHeadPosition(); posCurFont != NULL;)
	{
		CMenuFont *curFont = (CMenuFont*)m_pFontList->GetNext(posCurFont);
		SendDlgItemMessage(IDC_COMBO_Font, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)curFont->m_name);
	}

	//Text Properties
	if(m_pMenu->m_titleText)
	{
		SetDlgItemText(IDC_EDIT_Text, m_pMenu->m_titleText->m_text);
		SendDlgItemMessage(IDC_COMBO_Font, CB_SETCURSEL, m_pFontList->GetIndexFromPointer(m_pMenu->m_titleText->m_font));

		/*SetDlgItemInt(IDC_EDIT_ColorRed, m_pMenu->m_titleText->m_red * 255.0f);
		SetDlgItemInt(IDC_EDIT_ColorGreen, m_pMenu->m_titleText->m_green * 255.0f);
		SetDlgItemInt(IDC_EDIT_ColorBlue, m_pMenu->m_titleText->m_blue * 255.0f);
		SetDlgItemInt(IDC_EDIT_ColorAlpha, m_pMenu->m_titleText->m_alpha * 255.0f);*/

		SetDlgItemInt(IDC_EDIT_ColorRed, (UINT)(m_pMenu->m_titleText->m_red * 255.0f));
		SetDlgItemInt(IDC_EDIT_ColorGreen, (UINT)(m_pMenu->m_titleText->m_green * 255.0f));
		SetDlgItemInt(IDC_EDIT_ColorBlue, (UINT)(m_pMenu->m_titleText->m_blue * 255.0f));
		SetDlgItemInt(IDC_EDIT_ColorAlpha, (UINT)(m_pMenu->m_titleText->m_alpha * 255.0f));
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//This must be done because GetIndexFromPointer defaultly returns 0 to select the first font in the list.
	// This means m_font could still be NULL, so we must make sure it's set
	if(!m_pFontList->IsEmpty() && m_pMenu->m_titleText)
		m_pMenu->m_titleText->m_font = m_pFontList->GetPointerFromIndex(m_pFontList->GetIndexFromPointer(m_pMenu->m_titleText->m_font));

	//Title Coordinates
	float x = 0.0f, y = 0.0f, width = 0.0f, height = 0.0f;
	if(m_pMenu->m_titleLeft)
	{
		x		= m_pMenu->m_titleLeft->m_posX;
		y		= m_pMenu->m_titleLeft->m_posY;
		width	= m_pMenu->m_titleLeft->m_width;
		height	= m_pMenu->m_titleLeft->m_height;

		if(m_pMenu->m_titleMiddle)
			width += m_pMenu->m_titleMiddle->m_width;

		if(m_pMenu->m_titleRight)
			width += m_pMenu->m_titleRight->m_width;
	}
	else if(m_pMenu->m_titleMiddle)
	{
		x		= m_pMenu->m_titleMiddle->m_posX;
		y		= m_pMenu->m_titleMiddle->m_posY;
		width	= m_pMenu->m_titleMiddle->m_width;
		height	= m_pMenu->m_titleMiddle->m_height;
	}
	/*SetDlgItemInt(IDC_EDIT_VisualX, x);
	SetDlgItemInt(IDC_EDIT_VisualY, y);
	SetDlgItemInt(IDC_EDIT_VisualWidth, width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, height);*/

	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)x);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)y);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)height);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Text Coordinates
	if(m_pMenu->m_titleText)
	{
		/*SetDlgItemInt(IDC_EDIT_TextX, m_pMenu->m_titleText->m_posX);
		SetDlgItemInt(IDC_EDIT_TextY, m_pMenu->m_titleText->m_posY);*/

		SetDlgItemInt(IDC_EDIT_TextX, (UINT)m_pMenu->m_titleText->m_posX);
		SetDlgItemInt(IDC_EDIT_TextY, (UINT)m_pMenu->m_titleText->m_posY);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Left Texture
	if(m_pMenu->m_titleLeft)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX, m_pMenu->m_titleLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY, m_pMenu->m_titleLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth, m_pMenu->m_titleLeft->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX, (UINT)m_pMenu->m_titleLeft->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY, (UINT)m_pMenu->m_titleLeft->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth, (UINT)m_pMenu->m_titleLeft->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Middle Texture
	if(m_pMenu->m_titleMiddle)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX, m_pMenu->m_titleMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY, m_pMenu->m_titleMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth, m_pMenu->m_titleMiddle->m_textureWidth);*/


		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX, (UINT)m_pMenu->m_titleMiddle->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY, (UINT)m_pMenu->m_titleMiddle->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth, (UINT)m_pMenu->m_titleMiddle->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Background Right Texture
	if(m_pMenu->m_titleRight)
	{
		/*SetDlgItemInt(IDC_EDIT_BackgroundRightTextureX, m_pMenu->m_titleRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureY, m_pMenu->m_titleRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth, m_pMenu->m_titleRight->m_textureWidth);*/

		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureX, (UINT)m_pMenu->m_titleRight->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureY, (UINT)m_pMenu->m_titleRight->m_texturePosY);
		SetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth, (UINT)m_pMenu->m_titleRight->m_textureWidth);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Properties
	CheckDlgButton(IDC_CHECK_StretchTitle, m_pMenu->m_stretchTitle);
	CheckDlgButton(IDC_CHECK_DragByTitleOnly, m_pMenu->m_dragByTitleOnly);

	m_initialized = true;

	return TRUE;
}

void CEditMenuTitle::OnChangeText()
{
	if(!m_initialized || m_pFontList->IsEmpty())
		return;

	if(!m_pMenu->m_titleText)
		m_pMenu->m_titleText = new CMenuText();

	//Get the font pointer
	m_pMenu->m_titleText->m_font = m_pFontList->GetPointerFromIndex(m_pFontList->GetIndexFromPointer(m_pMenu->m_titleText->m_font));
    ASSERT(m_pMenu->m_titleText->m_font);
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_Text, temp, 255);
	m_pMenu->m_titleText->SetText(temp);
}

void CEditMenuTitle::OnChangeFont()
{
	if(!m_initialized || m_pFontList->IsEmpty())
		return;

	if(!m_pMenu->m_titleText)
		m_pMenu->m_titleText = new CMenuText();

	m_pMenu->m_titleText->m_font		= m_pFontList->GetPointerFromIndex(SendDlgItemMessage(IDC_COMBO_Font, CB_GETCURSEL));
	ASSERT(m_pMenu->m_titleText->m_font);
	if(m_pMenu->m_titleText->m_font)
	   m_pMenu->m_titleText->m_fontName	= m_pMenu->m_titleText->m_font->m_name;
}

void CEditMenuTitle::OnChangeColor()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_titleText)
		m_pMenu->m_titleText = new CMenuText();

	m_pMenu->m_titleText->m_red		= (int)GetDlgItemInt(IDC_EDIT_ColorRed) / 255.0f;
	m_pMenu->m_titleText->m_green	= (int)GetDlgItemInt(IDC_EDIT_ColorGreen) / 255.0f;
	m_pMenu->m_titleText->m_blue	= (int)GetDlgItemInt(IDC_EDIT_ColorBlue) / 255.0f;
	m_pMenu->m_titleText->m_alpha	= (int)GetDlgItemInt(IDC_EDIT_ColorAlpha) / 255.0f;

	// Force rebuild of the text's mesh
	delete m_pMenu->m_titleText->m_textMesh;
	m_pMenu->m_titleText->m_textMesh = NULL;
}

void CEditMenuTitle::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	Resize();
}

void CEditMenuTitle::OnChangeTextCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_titleText)
		m_pMenu->m_titleText = new CMenuText();

	/*m_pMenu->m_titleText->m_posX = (int)GetDlgItemInt(IDC_EDIT_TextX);
	m_pMenu->m_titleText->m_posY = (int)GetDlgItemInt(IDC_EDIT_TextY);*/

	m_pMenu->m_titleText->m_posX = (float)(int)GetDlgItemInt(IDC_EDIT_TextX);
	m_pMenu->m_titleText->m_posY = (float)(int)GetDlgItemInt(IDC_EDIT_TextY);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditMenuTitle::OnChangeBackgroundLeftTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_titleLeft)
		m_pMenu->m_titleLeft = new CMenuImage();

	/*UpdateImageTextureCoords(m_pMenu->m_titleLeft,
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pMenu->m_titleLeft,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundLeftTextureWidth),
							 true);

	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditMenuTitle::OnButtonEditBackgroundLeftTexture()
{
	if(!m_pMenu->m_titleLeft)
	{
		m_pMenu->m_titleLeft = new CMenuImage();
		return;
	}

	EditTexture(m_pMenu->m_titleLeft);
}

void CEditMenuTitle::OnChangeBackgroundMiddleTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_titleMiddle)
		m_pMenu->m_titleMiddle = new CMenuImage();

	/*UpdateImageTextureCoords(m_pMenu->m_titleMiddle,
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth));*/

	UpdateImageTextureCoords(m_pMenu->m_titleMiddle,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundMiddleTextureWidth));
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditMenuTitle::OnButtonEditBackgroundMiddleTexture()
{
	if(!m_pMenu->m_titleMiddle)
	{
		m_pMenu->m_titleMiddle = new CMenuImage();
		return;
	}

	EditTexture(m_pMenu->m_titleMiddle);
}

void CEditMenuTitle::OnChangeBackgroundRightTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pMenu->m_titleRight)
		m_pMenu->m_titleRight = new CMenuImage();

	/*UpdateImageTextureCoords(m_pMenu->m_titleRight,
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureX),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureY),
							 (int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth),
							 true);*/

	UpdateImageTextureCoords(m_pMenu->m_titleRight,
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureX),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureY),
							 (float)(int)GetDlgItemInt(IDC_EDIT_BackgroundRightTextureWidth),
							 true);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditMenuTitle::OnButtonEditBackgroundRightTexture()
{
	if(!m_pMenu->m_titleRight)
	{
		m_pMenu->m_titleRight = new CMenuImage();
		return;
	}

	EditTexture(m_pMenu->m_titleRight);
}

void CEditMenuTitle::OnToggleStretchTitle()
{
	m_pMenu->m_stretchTitle = IsDlgButtonChecked(IDC_CHECK_StretchTitle);
}

void CEditMenuTitle::OnToggleDragByTitleOnly()
{
	m_pMenu->m_dragByTitleOnly = IsDlgButtonChecked(IDC_CHECK_DragByTitleOnly);
}

void CEditMenuTitle::OnButtonNext()
{
	CDialog::OnOK();
}

void CEditMenuTitle::EditTexture(CMenuImage *pImage)
{
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = m_pTextureDatabase;
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

void CEditMenuTitle::UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, bool CopyWidth)
{
	pImage->m_texturePosX	= X;
	pImage->m_texturePosY	= Y;
	pImage->m_textureWidth	= Width;

	if(CopyWidth)
		pImage->m_width = Width;

	UpdateImageTextureCoords(pImage);
}

void CEditMenuTitle::UpdateImageTextureCoords(CMenuImage *pImage)
{
	if(pImage->m_imageMesh)
	{
		//Get the texture dimensions
		float textureWidth = 512.0f;
		float textureHeight = 512.0f;
		char *textureName	= NULL;

		//Get the dimensions of the texture they selected, if any
		POSITION pos = m_pTextureDatabase->FindIndex(pImage->m_imageMesh->List_textureSelect[0]);
		if(pos != NULL)
		{
			CTextureDBObj *texture = (CTextureDBObj*)m_pTextureDatabase->GetAt(pos);

			//textureWidth	= texture->m_textureObject->m_widthBasis;
			//textureHeight	= texture->m_textureObject->m_hieghtBasis;

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

	Resize();
}

void CEditMenuTitle::Resize()
{
	/*float x		 = (int)GetDlgItemInt(IDC_EDIT_VisualX);
	float y		 = (int)GetDlgItemInt(IDC_EDIT_VisualY);
	float width	 = (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	float height = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

	float x		 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	float y		 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	float width	 = (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	float height = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	//Resize the title
	if(m_pMenu->m_titleLeft)
	{
		//Recalc right coordinates
		if(m_pMenu->m_titleRight)
		{
			m_pMenu->m_titleRight->m_posX			= x + width - m_pMenu->m_titleRight->m_width;
			m_pMenu->m_titleRight->m_posY			= y;
			m_pMenu->m_titleRight->m_height			= height;
			m_pMenu->m_titleRight->m_textureHeight	= height;

			//Subtract off the right's width
			width -= m_pMenu->m_titleRight->m_width;
		}

		//Recalc left coordinates
		m_pMenu->m_titleLeft->m_posX			= x;
		m_pMenu->m_titleLeft->m_posY			= y;
		m_pMenu->m_titleLeft->m_height			= height;
		m_pMenu->m_titleLeft->m_textureHeight	= height;

		//Subtract off the left's width
		width -= m_pMenu->m_titleLeft->m_width;

		//Recalc middle coordinates
		if(m_pMenu->m_titleMiddle)
		{
			m_pMenu->m_titleMiddle->m_posX			= x + m_pMenu->m_titleLeft->m_width;
			m_pMenu->m_titleMiddle->m_posY			= y;
			m_pMenu->m_titleMiddle->m_width			= width;
			m_pMenu->m_titleMiddle->m_height		= height;
			m_pMenu->m_titleMiddle->m_textureHeight	= height;
		}
	}
	else if(m_pMenu->m_titleMiddle)
	{
		m_pMenu->m_titleMiddle->m_posX			= x;
		m_pMenu->m_titleMiddle->m_posY			= y;
		m_pMenu->m_titleMiddle->m_width			= width;
		m_pMenu->m_titleMiddle->m_height		= height;
		m_pMenu->m_titleMiddle->m_textureHeight	= height;
	}

	//Let the menu recalc some things
	m_pMenu->Resize();
}

// CEditMenuTitle message handlers

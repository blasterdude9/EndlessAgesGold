// C:\Projects\EXCore_Platform\AuraUI\EditFont.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditFont.h"

// CEditFont dialog
IMPLEMENT_DYNAMIC(CEditFont, CDialog)

CEditFont::CEditFont(CWnd* pParent /*=NULL*/)
	: CDialog(CEditFont::IDD, pParent)
{
	m_pCurFont			= NULL;
	m_pFontImage		= NULL;
	m_initialized		= false;
}

BEGIN_MESSAGE_MAP(CEditFont, CDialog)
	//Fonts List
	ON_LBN_SELCHANGE(IDC_LIST_Fonts, OnSelectionChangeListFonts)
	ON_BN_CLICKED(IDC_BUTTON_AddFont, OnButtonAddFont)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceFont, OnButtonReplaceFont)

	//Font Settings
	ON_EN_CHANGE(IDC_EDIT_FontHeight, OnChangeFontSettings)
	ON_EN_CHANGE(IDC_EDIT_CharactersPerLine, OnChangeFontSettings)
	ON_BN_CLICKED(IDC_BUTTON_EditFontTexture, OnButtonEditFontTexture)

	//Character Settings
	ON_LBN_SELCHANGE(IDC_LIST_Characters, OnSelectionChangeListCharacters)
	ON_EN_CHANGE(IDC_EDIT_CharacterTextureX, OnChangeCharacterSettings)
	ON_EN_CHANGE(IDC_EDIT_CharacterTextureWidth, OnChangeCharacterSettings)
	ON_EN_CHANGE(IDC_EDIT_CharacterSpacingBefore, OnChangeCharacterSettings)
	ON_EN_CHANGE(IDC_EDIT_CharacterSpacingAfter, OnChangeCharacterSettings)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditFont::OnInitDialog()
{
	CDialog::OnInitDialog();

	RefreshFontList(-1);
	FillCharacterList();

	//Allow the editing of Font and Character properties
	m_initialized = true;

	return TRUE;
}

void CEditFont::OnSelectionChangeListFonts()
{
	//Don't allow the editing of Font and Character properties
	m_initialized = false;

	//If they have a font selected
	int fontIndex = SendDlgItemMessage(IDC_LIST_Fonts, LB_GETCURSEL);
	if(fontIndex != -1)
	{
		//Get a pointer to the selected font
		POSITION posFont = m_gStatus->m_menuFontList->FindIndex(fontIndex);
		if(posFont)
		{
			m_pCurFont = (CMenuFont*)m_gStatus->m_menuFontList->GetAt(posFont);

			//Copy the texture settings from the file
			m_pFontImage->m_imageMesh->List_materialObject->m_blendMode	= m_pCurFont->m_blendMode;
			m_pFontImage->m_imageMesh->List_textureSelect[0]			= m_pCurFont->m_textureSelect;
			m_pFontImage->m_fullTextureWidth							= m_pCurFont->m_fullTextureWidth;
			m_pFontImage->m_fullTextureHeight							= m_pCurFont->m_fullTextureHeight;

			//Fill the text boxes with the font's properties
			SetDlgItemText(IDC_EDIT_FontName, m_pCurFont->m_name);
			
			/*SetDlgItemInt(IDC_EDIT_FontHeight, m_pCurFont->m_height);*/
			SetDlgItemInt(IDC_EDIT_FontHeight, (UINT)m_pCurFont->m_height);
			//WARNING: C4244 --> conversion from float to UINT, possible loss of data

			SetDlgItemInt(IDC_EDIT_CharactersPerLine, m_pCurFont->m_charsPerLine);

			//If they have a character selected
			int charIndex = SendDlgItemMessage(IDC_LIST_Characters, LB_GETCURSEL);
			if(charIndex != -1)
			{
				//Fill the text boxes with the character's properties
				/*SetDlgItemInt(IDC_EDIT_CharacterTextureX, m_pCurFont->m_characterTextureX[charIndex]);
				SetDlgItemInt(IDC_EDIT_CharacterTextureWidth, m_pCurFont->m_characterTextureWidth[charIndex]);
				SetDlgItemInt(IDC_EDIT_CharacterSpacingBefore, m_pCurFont->m_characterSpacingBefore[charIndex]);
				SetDlgItemInt(IDC_EDIT_CharacterSpacingAfter, m_pCurFont->m_characterSpacingAfter[charIndex]);*/

				SetDlgItemInt(IDC_EDIT_CharacterTextureX, (UINT)m_pCurFont->m_characterTextureX[charIndex]);
				SetDlgItemInt(IDC_EDIT_CharacterTextureWidth, (UINT)m_pCurFont->m_characterTextureWidth[charIndex]);
				SetDlgItemInt(IDC_EDIT_CharacterSpacingBefore, (UINT)m_pCurFont->m_characterSpacingBefore[charIndex]);
				SetDlgItemInt(IDC_EDIT_CharacterSpacingAfter, (UINT)m_pCurFont->m_characterSpacingAfter[charIndex]);
				//WARNING: C4244 --> conversion from float to UINT, possible loss of data

				//Set the rendering image's coordinates for this character in the new font
				m_pFontImage->m_texturePosX = m_pCurFont->m_characterTextureX[charIndex];
				m_pFontImage->m_texturePosY = (charIndex / m_pCurFont->m_charsPerLine) * m_pCurFont->m_height;
				m_pFontImage->m_width	= m_pFontImage->m_textureWidth	= m_pCurFont->m_characterTextureWidth[charIndex];
				m_pFontImage->m_height	= m_pFontImage->m_textureHeight = m_pCurFont->m_height;

				if(m_pFontImage->m_imageMesh)
					m_pFontImage->UpdateTextureCoords();
			}
			else
			{
				m_pFontImage->m_width	= 0.0f;
				m_pFontImage->m_height	= 0.0f;
			}
		}
	}
	//Allow the editing of Font and Character properties
	m_initialized = true;
}

void CEditFont::OnButtonAddFont()
{
	//Create the new font
	CMenuFont *newFont	= new CMenuFont();

	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_FontName, temp, 255);
	newFont->m_name = temp;

	//Add it to the font list
	m_gStatus->m_menuFontList->AddTail(newFont);

	//Add the new font to the list and select it
	RefreshFontList(-1);
}

void CEditFont::OnButtonReplaceFont()
{
	//If they have a font selected
	int fontIndex = SendDlgItemMessage(IDC_LIST_Fonts, LB_GETCURSEL);
	if(fontIndex != -1)
	{
		//We use a temp because CString.GetBuffer() doesn't update string lengths and such
		char temp[255];
		GetDlgItemText(IDC_EDIT_FontName, temp, 255);
		m_pCurFont->m_name = temp;

		//Clear and Refresh the list
		RefreshFontList(fontIndex);
	}
	else
		MessageBox("You must select a font to replace.");
}

void CEditFont::OnChangeFontSettings()
{
	if(!m_initialized)
		return;

	//If they have a font selected
	if(m_pCurFont)
	{
		//Copy the new settings
		/*m_pCurFont->m_height	= (int)GetDlgItemInt(IDC_EDIT_FontHeight);*/
		m_pCurFont->m_charsPerLine	= (int)GetDlgItemInt(IDC_EDIT_CharactersPerLine);

		m_pCurFont->m_height	= (float)(int)GetDlgItemInt(IDC_EDIT_FontHeight);
		//WARNING: C4244 --> conversion from int to float, possible loss of data

		//If we have a character currently selected, set the rendering images
		// coordinates to reflect the new font settings
		int charIndex = SendDlgItemMessage(IDC_LIST_Characters, LB_GETCURSEL);
		if(charIndex != -1)
		{
			//Set the texture coordinates for the rendering image
			m_pFontImage->m_texturePosY = (charIndex / m_pCurFont->m_charsPerLine) * m_pCurFont->m_height;
			m_pFontImage->m_height	= m_pFontImage->m_textureHeight = m_pCurFont->m_height;
			m_pFontImage->UpdateTextureCoords();
		}
	}
}

void CEditFont::OnButtonEditFontTexture()
{
	//If they have a font selected
	if(m_pCurFont)
	{
		//Create a new material dialog
		CMaterialDlg dlg;
		dlg.m_pxShaderSystemRef = NULL;
		dlg.m_vxShaderSystemRef = NULL;
		dlg.m_globalShaderRef = NULL;
		dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
		dlg.meshRef = m_pFontImage->m_imageMesh;

		if(dlg.DoModal() == IDOK)
		{
			//Grab the image mesh and store it into the font
			m_pFontImage->m_imageMesh->List_materialObject->Reset();

			//Default texture size
			float textureWidth	= 512.0f;
			float textureHeight = 512.0f;
			char *textureName	= NULL;
			
			//Get the dimensions of the texture they selected, if any
			POSITION pos = m_gStatus->m_textureDataBase->FindIndex(m_pFontImage->m_imageMesh->List_textureSelect[0]);
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

			//Copy new material and texture properties
			m_pFontImage->m_red				= m_pFontImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.r;
			m_pFontImage->m_green			= m_pFontImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.g;
			m_pFontImage->m_blue			= m_pFontImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.b;
			m_pFontImage->m_alpha			= m_pFontImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.a;
			m_pFontImage->m_blendMode		= m_pCurFont->m_blendMode	  = m_pFontImage->m_imageMesh->List_materialObject->m_blendMode;
			m_pFontImage->m_textureSelect	= m_pCurFont->m_textureSelect = m_pFontImage->m_imageMesh->List_textureSelect[0];

			//Copy texture file
			if(textureName)
				m_pFontImage->m_textureFile = textureName;

			//Update mesh's texture coordinates
			m_pFontImage->m_fullTextureWidth	= m_pCurFont->m_fullTextureWidth	= textureWidth;
			m_pFontImage->m_fullTextureHeight	= m_pCurFont->m_fullTextureHeight	= textureHeight;
			m_pFontImage->UpdateTextureCoords();
		}
	}
}

void CEditFont::OnSelectionChangeListCharacters()
{
	//Don't allow the editing of Font and Character properties
	m_initialized = false;

	//If they have a font selected
	if(m_pCurFont)
	{
		//If they have a character selected
		int charIndex = SendDlgItemMessage(IDC_LIST_Characters, LB_GETCURSEL);
		if(charIndex != -1)
		{
			//Fill the text boxes with the character's properties
			/*SetDlgItemInt(IDC_EDIT_CharacterTextureX, m_pCurFont->m_characterTextureX[charIndex]);
			SetDlgItemInt(IDC_EDIT_CharacterTextureWidth, m_pCurFont->m_characterTextureWidth[charIndex]);
			SetDlgItemInt(IDC_EDIT_CharacterSpacingBefore, m_pCurFont->m_characterSpacingBefore[charIndex]);
			SetDlgItemInt(IDC_EDIT_CharacterSpacingAfter, m_pCurFont->m_characterSpacingAfter[charIndex]);*/

			SetDlgItemInt(IDC_EDIT_CharacterTextureX, (UINT)m_pCurFont->m_characterTextureX[charIndex]);
			SetDlgItemInt(IDC_EDIT_CharacterTextureWidth, (UINT)m_pCurFont->m_characterTextureWidth[charIndex]);
			SetDlgItemInt(IDC_EDIT_CharacterSpacingBefore, (UINT)m_pCurFont->m_characterSpacingBefore[charIndex]);
			SetDlgItemInt(IDC_EDIT_CharacterSpacingAfter, (UINT)m_pCurFont->m_characterSpacingAfter[charIndex]);
			//WARNING: C4244 --> conversion from float to UINT, possible loss of data

			//Set the texture coordinates for the rendering image
			m_pFontImage->m_texturePosX = m_pCurFont->m_characterTextureX[charIndex];
			m_pFontImage->m_texturePosY = (charIndex / m_pCurFont->m_charsPerLine) * m_pCurFont->m_height;
			m_pFontImage->m_width	= m_pFontImage->m_textureWidth	= m_pCurFont->m_characterTextureWidth[charIndex];
			m_pFontImage->m_height	= m_pFontImage->m_textureHeight = m_pCurFont->m_height;
			m_pFontImage->UpdateTextureCoords();
		}
	}

	//Allow the editing of Font and Character properties
	m_initialized = true;
}

void CEditFont::OnChangeCharacterSettings()
{
	if(!m_initialized)
		return;

	//If they have a font selected
	if(m_pCurFont)
	{
		//If they have a character selected
		int charIndex = SendDlgItemMessage(IDC_LIST_Characters, LB_GETCURSEL);
		if(charIndex != -1)
		{
			//Fill character's properties with the text boxes
			/*m_pCurFont->m_characterTextureX[charIndex]		= (int)GetDlgItemInt(IDC_EDIT_CharacterTextureX);
			m_pCurFont->m_characterTextureWidth[charIndex]	= (int)GetDlgItemInt(IDC_EDIT_CharacterTextureWidth);
			m_pCurFont->m_characterSpacingBefore[charIndex]	= (int)GetDlgItemInt(IDC_EDIT_CharacterSpacingBefore);
			m_pCurFont->m_characterSpacingAfter[charIndex]	= (int)GetDlgItemInt(IDC_EDIT_CharacterSpacingAfter);*/

			m_pCurFont->m_characterTextureX[charIndex]		= (float)(int)GetDlgItemInt(IDC_EDIT_CharacterTextureX);
			m_pCurFont->m_characterTextureWidth[charIndex]	= (float)(int)GetDlgItemInt(IDC_EDIT_CharacterTextureWidth);
			m_pCurFont->m_characterSpacingBefore[charIndex]	= (float)(int)GetDlgItemInt(IDC_EDIT_CharacterSpacingBefore);
			m_pCurFont->m_characterSpacingAfter[charIndex]	= (float)(int)GetDlgItemInt(IDC_EDIT_CharacterSpacingAfter);
			//WARNING: C4244 --> conversion from int to float, possible loss of data

			//Set the texture coordinates for the rendering image
			m_pFontImage->m_texturePosX = m_pCurFont->m_characterTextureX[charIndex];
			m_pFontImage->m_texturePosY = (charIndex / m_pCurFont->m_charsPerLine) * m_pCurFont->m_height;
			m_pFontImage->m_width	= m_pFontImage->m_textureWidth	= m_pCurFont->m_characterTextureWidth[charIndex];
			m_pFontImage->m_height	= m_pFontImage->m_textureHeight = m_pCurFont->m_height;
			m_pFontImage->UpdateTextureCoords();
		}
	}
}

void CEditFont::OnButtonNext()
{
	CDialog::OnOK();
}

void CEditFont::RefreshFontList(int selection)
{
	//Clear the font list
	SendDlgItemMessage(IDC_LIST_Fonts, LB_RESETCONTENT, 0, 0);

	//Refill the font list
	for(POSITION posCurFont = m_gStatus->m_menuFontList->GetHeadPosition(); posCurFont != NULL;)
	{
		CMenuFont *curFont = (CMenuFont*)m_gStatus->m_menuFontList->GetNext(posCurFont);
		SendDlgItemMessage(IDC_LIST_Fonts, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)curFont->m_name);
	}

	//Make sure we're not trying to select beyond the list
	if(selection >= m_gStatus->m_menuFontList->GetCount())
		selection = m_gStatus->m_menuFontList->GetCount() - 1;

	//Set the currently selected font
	if(selection != -1)
		SendDlgItemMessage(IDC_LIST_Fonts, LB_SETCURSEL, selection, 0);
}

void CEditFont::FillCharacterList()
{
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Space");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Exclamation Mark");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Quotation Mark");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Pound Sign");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Dollar Sign");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Percent Sign");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Ampersand");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Single Quote");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Left Parentheses");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Right Parentheses");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Asterisk");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Plus Sign");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Comma");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Dash");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Period");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Forward Slash");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Zero");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"One");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Two");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Three");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Four");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Five");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Six");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Seven");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Eight");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Nine");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Colon");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Semi-colon");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Less Than Sign");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Equals Sign");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Greater Than Sign");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Question Mark");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"At Sign");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"A");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"B");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"C");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"D");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"E");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"F");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"G");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"H");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"I");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"J");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"K");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"L");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"M");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"N");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"O");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"P");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Q");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"R");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"S");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"T");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"U");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"V");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"W");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"X");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Y");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Z");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Left Bracket");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Backslash");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Right Bracket");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Caret");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Underscore");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Grave Accent");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"a");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"b");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"c");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"d");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"e");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"f");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"g");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"h");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"i");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"j");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"k");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"l");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"m");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"n");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"o");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"p");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"q");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"r");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"s");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"t");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"u");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"v");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"w");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"x");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"y");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"z");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Left Brace");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Vertical Bar");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Right Brace");
	SendDlgItemMessage(IDC_LIST_Characters, LB_ADDSTRING, 0, (LPARAM)"Tilde");
}

// CEditFont message handlers
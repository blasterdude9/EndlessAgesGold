#include "stdafx.h"
#include "direct.h"
#include "CMenuFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMenuFont::CMenuFont()
{
	//Blank out the character x positions

	ZeroMemory(m_characterTextureX,sizeof(float)*MENU_FONTCHAR_MAX);
	ZeroMemory(m_characterTextureWidth,sizeof(float)*MENU_FONTCHAR_MAX);
	ZeroMemory(m_characterSpacingBefore,sizeof(float)*MENU_FONTCHAR_MAX);

	//Default the spacings after the character to 1
	for(int i = 0; i < MENU_FONTCHAR_MAX; i++)
		m_characterSpacingAfter[i] = 1.0f;
	
	//Settable Parameters
	m_height	= 1.0f;
	m_charsPerLine	= 1;

	//Engine Variables
	m_fullTextureWidth	= 1.0f;
	m_fullTextureHeight	= 1.0f;

	//Temperary Variables
	m_blendMode		= -1;
	m_textureSelect = -1;
}

/*CMenuFont& 
CMenuFont::operator=(const CMenuFont &rhs)
{
	this->m_name			= rhs.m_name;
	this->m_height			= rhs.m_height;
	this->m_charsPerLine	= rhs.m_charsPerLine;

	
	//this->m_characterSpacingAfter = rhs.m_characterSpacingAfter;
	//this->m_characterSpacingBefore = rhs.m_characterSpacingBefore;
	//this->m_characterTextureWidth = rhs.m_characterTextureWidth;
	//this->m_characterTextureX = rhs.m_characterTextureX;
	
	CopyMemory(this->m_characterTextureX,rhs.m_characterTextureX,sizeof(float)*MENU_FONTCHAR_MAX);
	CopyMemory(this->m_characterTextureWidth,rhs.m_characterTextureWidth,sizeof(float)*MENU_FONTCHAR_MAX);
	CopyMemory(this->m_characterSpacingBefore,rhs.m_characterSpacingBefore,sizeof(float)*MENU_FONTCHAR_MAX);
	CopyMemory(this->m_characterSpacingAfter,rhs.m_characterSpacingAfter,sizeof(float)*MENU_FONTCHAR_MAX);

	this->m_fullTextureWidth	= rhs.m_fullTextureWidth;
	this->m_fullTextureHeight	= rhs.m_fullTextureHeight;

	// Allow for chained assignments
	return *this;
}*/

void CMenuFont::Clone(CMenuFont **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuFont();
	Copy(*pClone, pD3DDevice);
}

void CMenuFont::Copy(CMenuFont *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	pCopy->m_name			= m_name;
	pCopy->m_height			= m_height;
	pCopy->m_charsPerLine	= m_charsPerLine;

    CopyMemory(pCopy->m_characterTextureX,m_characterTextureX,sizeof(float)*MENU_FONTCHAR_MAX);
	CopyMemory(pCopy->m_characterTextureWidth,m_characterTextureWidth,sizeof(float)*MENU_FONTCHAR_MAX);
	CopyMemory(pCopy->m_characterSpacingBefore,m_characterSpacingBefore,sizeof(float)*MENU_FONTCHAR_MAX);
	CopyMemory(pCopy->m_characterSpacingAfter,m_characterSpacingAfter,sizeof(float)*MENU_FONTCHAR_MAX);

	pCopy->m_fullTextureWidth	= m_fullTextureWidth;
	pCopy->m_fullTextureHeight	= m_fullTextureHeight;
}

void CMenuFont::Serialize(CXMLFileIO &xml)
{
	if(xml.m_isStoring)
	{
		xml.WriteAttribute("Version", 1);
		xml.WriteAttribute("Name", m_name.GetBuffer());
		xml.WriteAttribute("FontHeight", (int)m_height);
		xml.WriteAttribute("CharactersPerLine", m_charsPerLine);
		xml.WriteAttribute("FullTextureWidth", (int)m_fullTextureWidth);
		xml.WriteAttribute("FullTextureHeight", (int)m_fullTextureHeight);
		xml.WriteAttribute("BlendMode", m_blendMode);
		xml.WriteAttribute("TextureFile", m_textureFile.GetBuffer());

		for(int i = 0; i < MENU_FONTCHAR_MAX; i++)
		{
			xml.WriteAttribute("CharacterTextureX", (int)m_characterTextureX[i]);
			xml.WriteAttribute("CharacterTextureWidth", (int)m_characterTextureWidth[i]);
			xml.WriteAttribute("CharacterSpacingBefore", (int)m_characterSpacingBefore[i]);
			xml.WriteAttribute("CharacterSpacingAfter", (int)m_characterSpacingAfter[i]);
		}
	}
	else
	{
		int version;
		char buffer[MAX_BUFFER];
		bool closing;

		xml.ReadAttribute(version);
		xml.ReadAttribute(buffer, MAX_BUFFER); m_name = buffer;
		xml.ReadAttribute(m_height);
		xml.ReadAttribute(m_charsPerLine);
		xml.ReadAttribute(m_fullTextureWidth);
		xml.ReadAttribute(m_fullTextureHeight);
		xml.ReadAttribute(m_blendMode);
		xml.ReadAttribute(buffer, MAX_BUFFER); m_textureFile = buffer;

		for(int i = 0; i < MENU_FONTCHAR_MAX; i++)
		{
			xml.ReadAttribute(m_characterTextureX[i]);
			xml.ReadAttribute(m_characterTextureWidth[i]);
			xml.ReadAttribute(m_characterSpacingBefore[i]);
			xml.ReadAttribute(m_characterSpacingAfter[i]);
		}

		xml.ReadTag(buffer, MAX_BUFFER, closing); //Read in the closing tag for the font
	}
}

CMenuFont* CMenuFontList::GetFontByName(CString fontName)
{
	//Loop through all fonts in the list
	POSITION posCurFont = GetHeadPosition();
	CMenuFont *lastFont = NULL;
	while(posCurFont != NULL)
	{
		//Get the current font and check it
		CMenuFont *curFont = (CMenuFont*)GetNext(posCurFont);
		lastFont = curFont;
		if(curFont->m_name.CompareNoCase(fontName)==0 || fontName.CompareNoCase("Default")==0)
			return curFont;
	}

	return lastFont;//just take one if we dont have anything, most functions assume this is going to succeed
}

CMenuFont* CMenuFontList::GetPointerFromIndex(int index)
{
	return (CMenuFont*)GetAt(FindIndex(index));
}

int CMenuFontList::GetIndexFromPointer(CMenuFont *font)
{
	//Loop through all fonts in the list
	int index = 0;
	POSITION posCurFont = GetHeadPosition();
	while(posCurFont != NULL)
	{
		//Get the current font and check it
		CMenuFont *curFont = (CMenuFont*)GetNext(posCurFont);
		if(font == curFont)
			return index;

		//NEXT!
		index++;
	}

	return 0;
}

void CMenuFontList::Clone(CMenuFontList **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuFontList();
	Copy(*pClone, pD3DDevice);
}

void CMenuFontList::Copy(CMenuFontList *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	CMenuFont *curFont, *newFont;

	//Loop through all fonts in the list
	POSITION posCurFont = GetHeadPosition();
	while(posCurFont != NULL)
	{
		//Get the current font and clone it
		curFont = (CMenuFont*)GetNext(posCurFont);
		curFont->Clone(&newFont, pD3DDevice);
		pCopy->AddTail(newFont);
	}
}

CMenuFontList::~CMenuFontList()
{
	Flush();
}

void CMenuFontList::Flush()
{
	//Loop through all fonts in the list
	POSITION posCurFont = GetHeadPosition();
	while(posCurFont != NULL)
	{
		//Get the current font and call its safe delete function
		CMenuFont *curFont = (CMenuFont*)GetNext(posCurFont);
		delete curFont;
		curFont = 0;
	}

	//Delete every pointer in the list
	RemoveAll();
}

void CMenuFontList::Serialize(CXMLFileIO &xml)
{
	if(xml.m_isStoring)
	{
		//Loop through all fonts in the list
		POSITION posCurFont = GetHeadPosition();
		while(posCurFont != NULL)
		{
			//Get the current font and call its safe delete function
			CMenuFont *curFont = (CMenuFont*)GetNext(posCurFont);

			xml.WriteOpeningTag(curFont->m_name.GetBuffer());
			curFont->Serialize(xml);
			xml.WriteClosingTag(curFont->m_name.GetBuffer());
		}
	}
	else
	{
		char buffer[MAX_BUFFER] = {0};
		bool closing;
		CMenuFont *newFont = NULL;

		//Loop til we hit </FontList>
		while(1)
		{
			//Read the opening tag for the visual or </FontList>
			xml.ReadTag(buffer, MAX_BUFFER, closing);

			//If its </FontList>, break out of the loop
			if(closing)
				break;

			//Create a new font, read in the properties, and add it to the list
			newFont = new CMenuFont();
			newFont->Serialize(xml);
			AddTail(newFont);
		}
	}
}

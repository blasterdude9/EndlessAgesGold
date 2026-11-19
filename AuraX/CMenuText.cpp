#include "stdafx.h"
#include "direct.h"
#include "CMenuText.h"
#include "CMenuScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMenuText::CMenuText()
{
	//Visible Objects
	m_textMesh	 = NULL;
	
	//Settable Parameters
	m_font		 = NULL;
	m_alignment	 = MENU_TEXTALIGN_LEFT;
	m_multilined = false;

	//Temperary Variables
	m_red			= 1.0f;
	m_green			= 1.0f;
	m_blue			= 1.0f;
	m_alpha			= 1.0f;
	m_textWidth		= 0;
	m_iNumLines		= 0;

	//Scripts
	m_updateScript = NULL;
}

CMenuText::~CMenuText()
{
    if(m_textMesh)
	{
		m_textMesh->SafeDelete();
		delete m_textMesh;
		m_textMesh = NULL;
	}

	if(m_updateScript)
	{
	   delete m_updateScript;
	   m_updateScript = 0;
	}

	m_font = NULL;

}
/*
CMenuText& 
CMenuText::operator=(const CMenuText &rhs)
{
	//Copy base properties
	//CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy text settings
	//CMenuText *textCopy = (CMenuText*)pCopy;

	//Visible Objects
	if(rhs.m_textMesh)
	//	m_textMesh->Clone(&textCopy->m_textMesh, pD3DDevice);
		this->m_textMesh = rhs.m_textMesh;

	//Settable Parameters
	this->m_font		= rhs.m_font;
	this->m_text		= rhs.m_text;
	this->m_alignment	= rhs.m_alignment;
	this->m_multilined	= rhs.m_multilined;

	if(rhs.m_updateScript)
	   this->m_updateScript = rhs.m_updateScript;

	// Allow for chained assignments
	return *this;
}
*/
BOOL CMenuText::Init(LPDIRECT3DDEVICE9 pD3DDevice)
{
    if(!m_font)
		return FALSE;

	if(m_text.GetLength() < 1)
		return FALSE;
    
	//Create a new mesh for the new string
	m_textMesh = new CEXMeshObj();

	//Set up the vertex buffer
	m_textMesh->m_abVertexArray.m_vertexCount	= m_text.GetLength() * 4; //4 Vertices per Character
	m_textMesh->m_abVertexArray.m_vertexArray1	= new ABVERTEX1L[m_textMesh->m_abVertexArray.m_vertexCount];
	ZeroMemory(m_textMesh->m_abVertexArray.m_vertexArray1, sizeof(ABVERTEX1L) * m_textMesh->m_abVertexArray.m_vertexCount);

	//Set up the index buffer
	m_textMesh->m_abVertexArray.m_indecieCount	= m_text.GetLength() * 6; //6 indecies per Character
	m_textMesh->m_abVertexArray.m_indecieArray	= new WORD[m_textMesh->m_abVertexArray.m_indecieCount];
	ZeroMemory(m_textMesh->m_abVertexArray.m_indecieArray, sizeof(WORD) * m_textMesh->m_abVertexArray.m_indecieCount);

	//Create the material
	m_textMesh->List_materialObject = new CMaterialObject();
	m_textMesh->List_materialObject->m_cullOn = FALSE;

	//Copy texture properties from the selected font
	m_textMesh->List_textureSelect[0]			 = m_font->m_textureSelect;
	m_textMesh->List_materialObject->m_blendMode = m_font->m_blendMode;
	m_textMesh->m_abVertexArray.m_uvCount		 = 1;

	//Copy the material properties from the selected font
	m_textMesh->List_materialObject->m_useMaterial.Diffuse.r  = m_red;
	m_textMesh->List_materialObject->m_useMaterial.Diffuse.g  = m_green;
	m_textMesh->List_materialObject->m_useMaterial.Diffuse.b  = m_blue;
	m_textMesh->List_materialObject->m_useMaterial.Diffuse.a  = m_alpha;
	m_textMesh->List_materialObject->m_useMaterial.Emissive.r = m_red;
	m_textMesh->List_materialObject->m_useMaterial.Emissive.g = m_green;
	m_textMesh->List_materialObject->m_useMaterial.Emissive.b = m_blue;
	m_textMesh->List_materialObject->m_useMaterial.Emissive.a = m_alpha;

	//Get the end character for this line
	int endChar = 0;
	GetLineWidth(0, endChar);

	//Loop through the text and vertices
	int curVertex = 0;
	int curIndex = 0;
	for(int j = 0; j < m_text.GetLength(); j++)
	{
		//Subract 32 because thats the space character, which is the first character we can use
		int curChar = m_text.GetAt(j) - 32;

		//See if we need to wrap to the next line
		if(j > endChar)
		{
			//Skip spaces and new line characters
			while(m_text.GetAt(j) == ' ' || m_text.GetAt(j) == '\n')
			{
				j++;
				curChar = m_text.GetAt(j) - 32;
			}

			//Get the end character for the next line
			GetLineWidth(j, endChar);

			//Subract 32 because thats the space character, which is the first character we can use
			curChar = m_text.GetAt(j) - 32;
		}

		//Calculate the Y texture coordinate for this character
		float textureYCoord = (curChar / m_font->m_charsPerLine) * m_font->m_height;

		//Build the texture coordinates
		if (curChar >= 0)
		{
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex].tx0.tu	 = m_font->m_characterTextureX[curChar] / m_font->m_fullTextureWidth;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex].tx0.tv	 = textureYCoord / m_font->m_fullTextureHeight;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 1].tx0.tu = (m_font->m_characterTextureX[curChar] + m_font->m_characterTextureWidth[curChar]) / m_font->m_fullTextureWidth;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 1].tx0.tv = m_textMesh->m_abVertexArray.m_vertexArray1[curVertex].tx0.tv;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 2].tx0.tu = m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 1].tx0.tu;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 2].tx0.tv = (textureYCoord + m_font->m_height) / m_font->m_fullTextureWidth;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 3].tx0.tu = m_textMesh->m_abVertexArray.m_vertexArray1[curVertex].tx0.tu;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 3].tx0.tv = m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 2].tx0.tv;
		}

		//Build the indecies
		if (curIndex >= 0)
		{
			m_textMesh->m_abVertexArray.m_indecieArray[curIndex]	 = curVertex;
			m_textMesh->m_abVertexArray.m_indecieArray[curIndex + 1] = curVertex + 1;
			m_textMesh->m_abVertexArray.m_indecieArray[curIndex + 2] = curVertex + 3;
			m_textMesh->m_abVertexArray.m_indecieArray[curIndex + 3] = curVertex + 3;
			m_textMesh->m_abVertexArray.m_indecieArray[curIndex + 4] = curVertex + 1;
			m_textMesh->m_abVertexArray.m_indecieArray[curIndex + 5] = curVertex + 2;
		}

		//Next set of indecies and vertice
		curIndex += 6;
		curVertex += 4;
	}

	//Initialize the buffer
	m_textMesh->InitBuffer(pD3DDevice, NULL);
	return TRUE;
}

void CMenuText::Render(	LPDIRECT3DDEVICE9 pD3DDevice, 
						CStateManagementObj *stateManager, 
						CTextureDBObjList *textureDatabase,
						float parentX,
						float parentY)
{
	if(!pD3DDevice)
		return;

	if(!m_font)//referenced mem
		return;

	//Execute the update script, if it exists
	if(m_updateScript)
		m_updateScript->ExecuteScript();

	//If it hasn't been created, create it
	if(!m_textMesh)
		Init(pD3DDevice);

	if(!m_textMesh)
		return;

	//The text visual is now the parent
	parentX += m_posX;
	parentY += m_posY;

	//Calculate the start location
	float curPosX = parentX;
	float curPosY = parentY;
	int endChar = 0;
	if(m_alignment == MENU_TEXTALIGN_CENTER)
		curPosX -= (GetLineWidth(0, endChar) / 2.0f);
	else if(m_alignment == MENU_TEXTALIGN_RIGHT)
		curPosX -= GetLineWidth(0, endChar);
	else
		GetLineWidth(0, endChar);

	//Update the screen coordinates
	int curVertex = 0;
	if (m_text.GetLength() > 0)
		this->m_iNumLines = 1;
	else
		this->m_iNumLines = 0;

	for(int j = 0; j < m_text.GetLength(); j++)
	{
		//Subract 32 because thats the space character, which is the first character we can use
		// This is also where the printable ASCII characters begin.
		int curChar = m_text.GetAt(j) - 32;

		//Add the spacing before this character
		curPosX += m_font->m_characterSpacingBefore[curChar];

		//See if we need to wrap to the next line
		if(j > endChar)
		{
			// INCREMENT number of lines
			this->m_iNumLines++;

			//Set new coordinates
			curPosX = parentX;
			curPosY -= m_font->m_height;

			//Skip spaces and new line characters
			while(m_text.GetAt(j) == ' ' 
			|| m_text.GetAt(j) == '\r' 
			|| m_text.GetAt(j) == '\n')
			{
				j++;
				curChar = m_text.GetAt(j) - 32;
			}

			//Set the alignment for the next line
			if(m_alignment == MENU_TEXTALIGN_CENTER)
				curPosX -= (GetLineWidth(j, endChar) / 2.0f);
			else if(m_alignment == MENU_TEXTALIGN_RIGHT)
				curPosX -= GetLineWidth(j, endChar);
			else
				GetLineWidth(j, endChar);

			//Subract 32 because thats the space character, which is the first character we can use
			curChar = m_text.GetAt(j) - 32;
		}

		//Build the screen coordinates
		// NOTE: Do not render non-printing characters (NPCs)! NPCs are < 32, so curChar for NPCs
		//	will be < 0, creating an invalid index into m_font->m_characterTextureWidth[] and
		//	m_font->m_characterSpacingAfter[].
		if (curChar >= 0)
		{
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex].x		= curPosX;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex].y		= curPosY + m_font->m_height;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 1].x	= curPosX + m_font->m_characterTextureWidth[curChar];
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 1].y	= curPosY + m_font->m_height;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 2].x	= curPosX + m_font->m_characterTextureWidth[curChar];
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 2].y	= curPosY;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 3].x	= curPosX;
			m_textMesh->m_abVertexArray.m_vertexArray1[curVertex + 3].y	= curPosY;

			//Add the character width and the spacing after the character
			curPosX += m_font->m_characterTextureWidth[curChar] + m_font->m_characterSpacingAfter[curChar];
		}

		//Next set of vertices
		curVertex += 4;
	}

	//Update the vertex buffer positions
	m_textMesh->m_abVertexArray.ReinitializeDxVbuffersPositionOnly(pD3DDevice);

	//Set up texture settings
	stateManager->SetBlendType(pD3DDevice, m_textMesh->List_materialObject->m_blendMode);
	stateManager->SetMaterial(pD3DDevice, &m_textMesh->List_materialObject->m_useMaterial);
	stateManager->SetDiffuseSource(m_textMesh->m_abVertexArray.m_advancedFixedMode, pD3DDevice);
	stateManager->SetVertexType(m_textMesh->m_abVertexArray.m_uvCount, pD3DDevice);
	stateManager->SetTextureState(m_textMesh->List_textureSelect[0], stateManager->m_intZero, pD3DDevice, textureDatabase);	

	//Render the text
	m_textMesh->Render(pD3DDevice, stateManager->m_currentVertexType, m_textMesh->m_chromeUvSet);
}

void CMenuText::SetText(const char *newText)
{
	if(newText)
		m_text = newText;

	if(m_textMesh)
	{
		m_textMesh->SafeDelete();
		delete m_textMesh;
		m_textMesh = NULL;
	}
}

void CMenuText::SetText(char *newText)
{
	if(newText)
		m_text = newText;

	if(m_textMesh)
	{
		m_textMesh->SafeDelete();
		delete m_textMesh;
		m_textMesh = NULL;
	}
}

float CMenuText::GetLineWidth(int startChar, int &endChar)
{
	float ret = 0.0f;
    if(!m_font)
	    return ret;

	for(endChar = startChar; endChar < m_text.GetLength(); endChar++)
	{
		//If we hit a new line character, return
		if((m_text.GetAt(endChar) == '\n') && (endChar > startChar))
		{
			return ret;
		}

		//Subract 32 because thats the space character, which is the first character we can use
		int curChar = m_text.GetAt(endChar) - 32;

		//Add in the spacing before the character and the characters width
		ret += m_font->m_characterSpacingBefore[curChar] + m_font->m_characterTextureWidth[curChar];

		//If this is multilined and we're beyond the visual's width, return
		if(m_multilined && (ret > m_width))
		{
			//Loop back to the previous space
			int spaceChar = endChar;
			while(spaceChar >= startChar)
			{
				if(m_text.GetAt(spaceChar) == ' ')
				{
					endChar = spaceChar - 1;
					break;
				}
				spaceChar--;
			}

			m_textWidth = (int)ret;
			return ret;
		}

		//Add the spacing after the current character
		ret += m_font->m_characterSpacingAfter[curChar];
	}

	m_textWidth = (int)ret;
	return ret;
}

float 
CMenuText::GetLongestWidth()
{
	if (this->m_text.GetLength() == 0)
		return 0.0f;

	float	fTemp = 0.0f,
			fResult = 0.0f;
	int		iStartChar = 0,
			iEndChar = 0;

	while (iEndChar < this->m_text.GetLength())
	{
		//Skip spaces and new line characters
		while(m_text.GetAt(iStartChar) == ' ' 
		|| m_text.GetAt(iStartChar) == '\r' 
		|| m_text.GetAt(iStartChar) == '\n')
		{
			iStartChar++;
		}

		fTemp = this->GetLineWidth(iStartChar, iEndChar);
		iStartChar = iEndChar;
		if (fTemp > fResult) 
			fResult = fTemp;
	}

	return fResult;
}

void CMenuText::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuText();
	Copy(*pClone, pD3DDevice);
}

void CMenuText::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy text settings
	CMenuText *textCopy = (CMenuText*)pCopy;

	if(textCopy->m_textMesh)
	{
		textCopy->m_textMesh->SafeDelete();
		delete textCopy->m_textMesh;
		textCopy->m_textMesh = 0;
	}

	//Visible Objects
	if(m_textMesh)
		m_textMesh->Clone(&textCopy->m_textMesh, pD3DDevice);

	//Settable Parameters
	textCopy->m_font		= m_font;
	textCopy->m_text		= m_text;
	textCopy->m_alignment	= m_alignment;
	textCopy->m_multilined	= m_multilined;

	if(m_updateScript)
	   m_updateScript->Clone(&textCopy->m_updateScript);
}

void CMenuText::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		xml.WriteAttribute("Version", 1);
		if(m_font)
		   xml.WriteAttribute("Font", m_font->m_name.GetBuffer());
		else
		   xml.WriteAttribute("Font", "Default");
		xml.WriteAttribute("Text", m_text.GetBuffer());
		xml.WriteAttribute("Alignment", m_alignment);
		xml.WriteAttribute("Multilined", m_multilined);
		xml.WriteAttribute("Red", (int)(m_red * 255));
		xml.WriteAttribute("Green", (int)(m_green * 255));
		xml.WriteAttribute("Blue", (int)(m_blue * 255));
		xml.WriteAttribute("Alpha", (int)(m_alpha * 255));

		//Write out the update script
		if(m_updateScript)
		if(!m_updateScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("UpdateScript");
			xml.WriteMultilinedAttribute(m_updateScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("UpdateScript");
		}
	}
	else
	{
		char buffer[MAX_BUFFER];
		bool closing;
		int  version;

		xml.ReadAttribute(version);
		xml.ReadAttribute(buffer, MAX_BUFFER); m_fontName = buffer;
		xml.ReadAttribute(buffer, MAX_BUFFER); m_text = buffer;
		xml.ReadAttribute(m_alignment);
		xml.ReadAttribute(m_multilined);
		xml.ReadAttribute(m_red); m_red /= 255.0f;
		xml.ReadAttribute(m_green); m_green /= 255.0f;
		xml.ReadAttribute(m_blue); m_blue /= 255.0f;
		xml.ReadAttribute(m_alpha); m_alpha /= 255.0f;

		//Read in <UpdateScript> or </Text>
		xml.ReadTag(buffer, MAX_BUFFER, closing);
		
		//If the UpdateScript is present
		if(strcmp(buffer, "UpdateScript") == 0)
		{
			//We've got a update script, create a new script instance
			m_updateScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </UpdateScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </UpdateScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_updateScript->AddLine(buffer);
			}

			//Read in </Image>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}
	}
}

#include "stdafx.h"
#include "direct.h"
#include "CMenuImage.h"
#include "CMenuScript.h"
#include "GlobalStatusVariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern GlobalStatusVariables *globalStatus;

CMenuImage::CMenuImage()
{
	//Visible Objects
	m_imageMesh			= NULL;
	m_optionalDynTexture= NULL;
	m_surfaceController = NULL;

	//Settable Parameters
	this->m_bIsMouseOver	= false;
	m_texturePosX		= 0.0f;
	m_texturePosY		= 0.0f;
	m_textureWidth		= 0.0f;
	m_textureHeight		= 0.0f;
	m_bDraggable		= true;
	this->m_fLocalOriginX = std::numeric_limits<float>::infinity();
	this->m_fLocalOriginY = std::numeric_limits<float>::infinity();
	this->m_fGlobalOriginX = std::numeric_limits<float>::infinity();
	this->m_fGlobalOriginY = std::numeric_limits<float>::infinity();

	//Engine Variables
	m_fullTextureWidth	= 1.0f; //Prevent divide by 0
	m_fullTextureHeight	= 1.0f; //Prevent divide by 0
	m_mousedDown		= false;
	m_mousedOver		= false;
	m_draggingItem		= NULL;

	//Temperary Variables
	m_red				= 1.0f;
	m_green				= 1.0f;
	m_blue				= 1.0f;
	m_alpha				= 1.0f;
	m_blendMode			= -1;
	m_textureSelect		= -1;

	// Miscellaneous data
	m_GLID				= 0;

	//Scripts
	m_clickScript		= NULL;
	m_mouseOverScript	= NULL;
	m_mouseOutScript	= NULL;
	m_dropScript		= NULL;
	m_updateScript		= NULL;
}

void CMenuImage::Init(LPDIRECT3DDEVICE9 pD3DDevice)
{
	SafeDelete(this->m_imageMesh);
	this->m_imageMesh = new CEXMeshObj();
	this->m_imageMesh->List_materialObject = new CMaterialObject();
	this->m_imageMesh->List_materialObject->m_cullOn = FALSE;

	//Set up texture
	m_imageMesh->List_materialObject->m_baseBlendMode	= m_blendMode;
	m_imageMesh->List_textureSelect[0]					= m_textureSelect;

	//Set up materials
	m_imageMesh->List_materialObject->m_baseMaterial.Diffuse.r	= m_red;
	m_imageMesh->List_materialObject->m_baseMaterial.Diffuse.g	= m_green;
	m_imageMesh->List_materialObject->m_baseMaterial.Diffuse.b	= m_blue;
	m_imageMesh->List_materialObject->m_baseMaterial.Diffuse.a	= m_alpha;
	m_imageMesh->List_materialObject->m_baseMaterial.Emissive.r	= m_red;
	m_imageMesh->List_materialObject->m_baseMaterial.Emissive.g	= m_green;
	m_imageMesh->List_materialObject->m_baseMaterial.Emissive.b	= m_blue;
	m_imageMesh->List_materialObject->m_baseMaterial.Emissive.a	= m_alpha;
	m_imageMesh->List_materialObject->Reset();

	//Create Vertex and Index arrays
	SafeDeleteArray(m_imageMesh->m_abVertexArray.m_indecieArray);
	SafeDeleteArray(m_imageMesh->m_abVertexArray.m_vertexArray1);

	m_imageMesh->m_abVertexArray.m_indecieCount = 6;
	m_imageMesh->m_abVertexArray.m_indecieArray = new WORD[m_imageMesh->m_abVertexArray.m_indecieCount];
	ZeroMemory(m_imageMesh->m_abVertexArray.m_indecieArray, sizeof(WORD)*m_imageMesh->m_abVertexArray.m_indecieCount);

	m_imageMesh->m_abVertexArray.m_vertexCount = 4;
	m_imageMesh->m_abVertexArray.m_vertexArray1 = new ABVERTEX1L[m_imageMesh->m_abVertexArray.m_vertexCount];
	ZeroMemory(m_imageMesh->m_abVertexArray.m_vertexArray1, sizeof(ABVERTEX1L)*m_imageMesh->m_abVertexArray.m_vertexCount);

	/*
	ABVERTEX1L *verts = new ABVERTEX1L[4];
	ZeroMemory(verts,sizeof(ABVERTEX1L)*4);
	WORD *indecies = new WORD[6];
	ZeroMemory(indecies,sizeof(WORD)*6);
	*/

	//indecie
	m_imageMesh->m_abVertexArray.m_indecieArray[0] = 0;
	m_imageMesh->m_abVertexArray.m_indecieArray[1] = 1;
	m_imageMesh->m_abVertexArray.m_indecieArray[2] = 3;
	m_imageMesh->m_abVertexArray.m_indecieArray[3] = 3;
	m_imageMesh->m_abVertexArray.m_indecieArray[4] = 1;
	m_imageMesh->m_abVertexArray.m_indecieArray[5] = 2;

	//N
	m_imageMesh->m_abVertexArray.m_vertexArray1[0].nx = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[0].ny = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[0].nz = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[1].nx = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[1].ny = 0.5f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[1].nz = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[2].nx = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[2].ny = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[2].nz = 1.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[3].nx = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[3].ny = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[3].nz = 1.0f;
	//UVSET1
	m_imageMesh->m_abVertexArray.m_vertexArray1[0].tx0.tu = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[0].tx0.tv = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[1].tx0.tu = 1.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[1].tx0.tv = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[2].tx0.tu = 1.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[2].tx0.tv = 1.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[3].tx0.tu = 0.0f;
	m_imageMesh->m_abVertexArray.m_vertexArray1[3].tx0.tv = 1.0f;

	/*
	m_imageMesh->m_abVertexArray.m_vertexCount  = 4;
	m_imageMesh->m_abVertexArray.m_indecieArray = indecies;
	m_imageMesh->m_abVertexArray.m_indecieCount = 6;
	m_imageMesh->m_abVertexArray.m_vertexArray1 = verts;
	*/
	m_imageMesh->m_abVertexArray.m_uvCount = 1;
	m_imageMesh->m_validUvCount = 1;
	
	m_imageMesh->InitBuffer(pD3DDevice, NULL);

	UpdateTextureCoords();
}

bool
CMenuImage::IsMouseOver(CInput *pInput, float fParentX, float fParentY)
{
	fParentX += this->m_posX;
	fParentY += this->m_posY;

	if ((pInput->m_mouseAbsPosX > fParentX)
	&& (pInput->m_mouseAbsPosX < (fParentX + this->m_width))
	&& (pInput->m_mouseAbsPosY > fParentY) 
	&& (pInput->m_mouseAbsPosY < (fParentY + this->m_height)))
	{
		return true;
	}
	return false;
}

bool CMenuImage::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//The image is now the parent
	//parentX += m_posX;
	//parentY += m_posY;

	// Execute the update script
	if(m_updateScript)
	{
		if (m_updateScript->m_pFirstLine 
		&& (m_updateScript->m_pFirstLine->m_function == -1)) 
			SafeDelete(m_updateScript);
		else
			m_updateScript->ExecuteScript();
	}

	if(!globalStatus->m_showMenuCursor
	|| pInput->m_mouseButtons[MOUSE_LEFTBUTTON].IsHeld()
	|| this->m_alpha == 0.0f)
	{
		return false;
	}

	//If the mouse is inside of the image
	if (this->IsMouseOver(pInput, parentX, parentY))
	{
		// SET the mouse over flag
		this->m_bIsMouseOver = true;

		//If they just pressed the mouse button over the image,
		if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
		{
			//Set moused down and allow the click script to run
			m_mousedDown = true;
			if (this->m_clickScript)
			{
				this->m_clickScript->ExecuteScript();
				return true;
			}
			return false;
		}
		else if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased())
		{
			if (globalStatus->m_draggingItem
			&& (globalStatus->m_draggingItem->m_icon != this))
			{
				//Execute the drop script, if it exists
				if(m_dropScript)
				{
					m_dropScript->ExecuteScript();
					return true;
				}
				return false;
			}
		}
		else
		{
			//Continuously execute the script(allows for pausing)
			if(m_mouseOverScript)
				m_mouseOverScript->ExecuteScript();

			//Allow the mouse out script to run
			m_mousedOver = true;
		}
	}
	else
	{
		// UNSET the mouse over flag
		this->m_bIsMouseOver = false;

		if(m_mousedOver)
		{
			//This is only executed once
			if(m_mouseOutScript)
				m_mouseOutScript->ExecuteScript();

			//We're no longer hovering over the image
			m_mousedOver = false;
		}
	}
	return false;
}

void CMenuImage::Render(LPDIRECT3DDEVICE9 pD3DDevice, 
						CStateManagementObj *stateManager, 
						CTextureDBObjList *textureDatabase,
						float parentX,
						float parentY)
{
	if(!pD3DDevice) return;

	//If it hasn't been created, create it
	if(!m_imageMesh)
		Init(pD3DDevice);
	
	//Update its location
	float left	 = parentX + m_posX;
	float right  = left + m_width;
	float bottom = parentY + m_posY;
	float top	 = bottom + m_height;

    if(m_imageMesh->m_abVertexArray.m_vertexArray1[0].x != left || 
	   m_imageMesh->m_abVertexArray.m_vertexArray1[0].y != top  || 
	   m_imageMesh->m_abVertexArray.m_vertexArray1[1].x != right || 
	   m_imageMesh->m_abVertexArray.m_vertexArray1[1].y != top ||
	   m_imageMesh->m_abVertexArray.m_vertexArray1[2].x != right || 
	   m_imageMesh->m_abVertexArray.m_vertexArray1[2].y != bottom ||
	   m_imageMesh->m_abVertexArray.m_vertexArray1[3].x != left || 
	   m_imageMesh->m_abVertexArray.m_vertexArray1[3].y != bottom)
	{//optimize for locks-only lock memory for changes
			m_imageMesh->m_abVertexArray.m_vertexArray1[0].x = left;
			m_imageMesh->m_abVertexArray.m_vertexArray1[0].y = top;

			m_imageMesh->m_abVertexArray.m_vertexArray1[1].x = right;
			m_imageMesh->m_abVertexArray.m_vertexArray1[1].y = top;

			m_imageMesh->m_abVertexArray.m_vertexArray1[2].x = right;
			m_imageMesh->m_abVertexArray.m_vertexArray1[2].y = bottom;

			m_imageMesh->m_abVertexArray.m_vertexArray1[3].x = left;
			m_imageMesh->m_abVertexArray.m_vertexArray1[3].y = bottom;

			m_imageMesh->m_abVertexArray.ReinitializeDxVbuffersPositionOnly(pD3DDevice);
	}

	//Set up texture settings
	stateManager->SetBlendType(pD3DDevice, m_imageMesh->List_materialObject->m_blendMode);
	stateManager->SetMaterial(pD3DDevice, &m_imageMesh->List_materialObject->m_useMaterial);
	stateManager->SetDiffuseSource(m_imageMesh->m_abVertexArray.m_advancedFixedMode, pD3DDevice);
	stateManager->SetVertexType(m_imageMesh->m_abVertexArray.m_uvCount, pD3DDevice);
	if(!m_optionalDynTexture)
	   stateManager->SetTextureState(m_imageMesh->List_textureSelect[0], stateManager->m_intZero, pD3DDevice, textureDatabase);	
	else
	{
	   stateManager->GlobalTextureIndex[0] = -1;//makes sure general piline knows this is unset
	   pD3DDevice->SetTexture( 0, m_optionalDynTexture);//sets dynamic custom texture ussually set at update script
	}

	// Render the image
	m_imageMesh->Render(pD3DDevice, stateManager->m_currentVertexType, m_imageMesh->m_chromeUvSet);
}

void CMenuImage::UpdateTextureCoords()
{
	if(!m_imageMesh)
		return;

	ABVERTEX1L* pVertices = NULL;

	//If we can successfully lock the vertex buffer
	if(m_imageMesh->m_abVertexArray.m_vertexBuffer
	&& m_imageMesh->m_abVertexArray.m_vertexBuffer->Lock(0, m_imageMesh->m_abVertexArray.m_vertexCount * sizeof(ABVERTEX1L), (VOID**)&pVertices, 0) == 0)
	{
		//Calculate the new texture uvs
		float top	 = m_texturePosY / m_fullTextureHeight;
		float left	 = m_texturePosX / m_fullTextureWidth;
		float bottom = (m_texturePosY + m_textureHeight) / m_fullTextureHeight;
		float right	 = (m_texturePosX + m_textureWidth) / m_fullTextureWidth;

		//Set the each vertex's coordinates
		pVertices[0].tx0.tu = left;
		pVertices[0].tx0.tv = top;

		pVertices[1].tx0.tu = right;
		pVertices[1].tx0.tv = top;

		pVertices[2].tx0.tu = right;
		pVertices[2].tx0.tv = bottom;

		pVertices[3].tx0.tu = left;
		pVertices[3].tx0.tv = bottom;

		//We're done, unlock the vertex buffer
		m_imageMesh->m_abVertexArray.m_vertexBuffer->Unlock();
	}
}

void CMenuImage::UpdateTexture(int newTextureSelect)
{
	m_textureSelect = newTextureSelect;

	if(m_imageMesh)
		m_imageMesh->List_textureSelect[0] = newTextureSelect;
	
	if(m_imageMesh)
	{
		m_imageMesh->SafeDelete();
		delete m_imageMesh;
		m_imageMesh = 0;
	}
}

void CMenuImage::RotateTexture(float angle)
{
	//If we don't have an image mesh, we can't do anything yet
	if(!m_imageMesh)
		return;

	//Add 45 degrees to the angle, because the corners start 
	//  at a 45 degree angle from the center
	angle += .785f;

	//Convert texture coordinates to uv coordinates
	float offsetX = m_texturePosX / m_fullTextureWidth;
	float offsetY = m_texturePosY / m_fullTextureHeight;

	//If we can successfully lock the vertex buffer
	ABVERTEX1L* pVertices = NULL;
	if(m_imageMesh->m_abVertexArray.m_vertexBuffer->Lock(0, m_imageMesh->m_abVertexArray.m_vertexCount * sizeof(ABVERTEX1L), (VOID**)&pVertices, 0) == 0)
	{
		//Calculate the new texture uvs
		float hypLength = sqrt((m_textureWidth * m_textureWidth + m_textureHeight * m_textureHeight) / 4.0f);
		float cosLength = (hypLength * cos(angle)) / m_fullTextureWidth;
		float sinLength = (hypLength * sin(angle)) / m_fullTextureHeight;

		//Set the each vertex's coordinates
		pVertices[0].tx0.tu = offsetX - cosLength;
		pVertices[0].tx0.tv = offsetY - sinLength;

		pVertices[1].tx0.tu = offsetX + sinLength;
		pVertices[1].tx0.tv = offsetY - cosLength;

		pVertices[2].tx0.tu = offsetX + cosLength;
		pVertices[2].tx0.tv = offsetY + sinLength;

		pVertices[3].tx0.tu = offsetX - sinLength;
		pVertices[3].tx0.tv = offsetY + cosLength;

		//We're done, unlock the vertex buffer
		m_imageMesh->m_abVertexArray.m_vertexBuffer->Unlock();
	}
}

/*CMenuImage& 
CMenuImage::operator=(const CMenuImage &rhs)
{
	// Check for self-assignment
	if(this == &rhs) return *this;

	if(this->m_imageMesh)
	{
		this->m_imageMesh->SafeDelete();
		delete this->m_imageMesh;
		this->m_imageMesh = NULL;
	}
	this->m_imageMesh = new CEXMeshObj();

	//Copy base properties
	this->Copy(pCopy, pD3DDevice);

	//Copy image settings
	CMenuImage *imageCopy = (CMenuImage*)pCopy;

	//Visible Objects
	if(m_imageMesh)
		m_imageMesh->Clone(&this->m_imageMesh, pD3DDevice);

	//Settable Parameters
	this->m_texturePosX	= rhs.m_texturePosX;
	this->m_texturePosY	= rhs.m_texturePosY;
	this->m_textureWidth = rhs.m_textureWidth;
	this->m_textureHeight = rhs.m_textureHeight;

	//Engine Variables
	this->m_fullTextureWidth = rhs.m_fullTextureWidth;
	this->m_fullTextureHeight = rhs.m_fullTextureHeight;

	//Temperary Variables
	this->m_red = rhs.m_red;
	this->m_green = rhs.m_green;
	this->m_blue = rhs.m_blue;
	this->m_alpha = rhs.m_alpha;
	this->m_blendMode = rhs.m_blendMode;
	this->m_textureFile	= rhs.m_textureFile;
	this->m_textureSelect = rhs.m_textureSelect;

	//scripts
	if(rhs.m_clickScript)
       this->m_clickScript = rhs.m_clickScript;

	if(rhs.m_mouseOverScript)
       this->m_mouseOverScript = rhs.m_mouseOverScript;

	if(rhs.m_mouseOutScript)
       this->m_mouseOutScript = rhs.m_mouseOutScript;

	if(rhs.m_dropScript)
       this->m_dropScript = rhs.m_dropScript;

	if(rhs.m_updateScript)
       this->m_updateScript = rhs.m_updateScript;

	if(rhs.m_surfaceController)
		this->m_surfaceController = rhs.m_surfaceController;

	// Allow for chained assignments
	return *this;
}*/

void CMenuImage::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	SafeDelete(*pClone);
	*pClone = new CMenuImage();
	Copy(*pClone, pD3DDevice);
}

void CMenuImage::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy image settings
	CMenuImage *imageCopy = (CMenuImage*)pCopy;

	//Settable Parameters
	imageCopy->m_bIsMouseOver	= m_bIsMouseOver;
	imageCopy->m_texturePosX	= m_texturePosX;
	imageCopy->m_texturePosY	= m_texturePosY;
	imageCopy->m_textureWidth	= m_textureWidth;
	imageCopy->m_textureHeight	= m_textureHeight;
	imageCopy->m_fLocalOriginX	= m_fLocalOriginX;
	imageCopy->m_fLocalOriginY	= m_fLocalOriginY;

	//Engine Variables
	imageCopy->m_fullTextureWidth	= m_fullTextureWidth;
	imageCopy->m_fullTextureHeight	= m_fullTextureHeight;

	//Temperary Variables
	imageCopy->m_red			= m_red;
	imageCopy->m_green			= m_green;
	imageCopy->m_blue			= m_blue;
	imageCopy->m_alpha			= m_alpha;
	imageCopy->m_blendMode		= m_blendMode;
	imageCopy->m_textureFile	= m_textureFile;
	imageCopy->m_textureSelect	= m_textureSelect;

	// Miscellaneous data
	imageCopy->m_GLID			= m_GLID;

	//Visible Objects
	if(imageCopy->m_imageMesh)
	{
		imageCopy->m_imageMesh->SafeDelete();
		delete imageCopy->m_imageMesh;
		imageCopy->m_imageMesh = 0;
	}
	
	if (m_imageMesh)
		m_imageMesh->Clone(&imageCopy->m_imageMesh, pD3DDevice);


	imageCopy->Init(pD3DDevice);

	imageCopy->m_visible = true;
	imageCopy->m_alpha = 1.0f;
	imageCopy->m_imageMesh->List_blendType = 1;
	//imageCopy->UpdateTexture(imageCopy->m_textureSelect);
	imageCopy->UpdateTextureCoords();

	// Scripts
	if(m_clickScript)
       m_clickScript->Clone(&imageCopy->m_clickScript);

	if(m_mouseOverScript)
       m_mouseOverScript->Clone(&imageCopy->m_mouseOverScript);

	if(m_mouseOutScript)
       m_mouseOutScript->Clone(&imageCopy->m_mouseOutScript);

	if(m_dropScript)
       m_dropScript->Clone(&imageCopy->m_dropScript);

	if(m_updateScript)
       m_updateScript->Clone(&imageCopy->m_updateScript);

}

CMenuImage::~CMenuImage()
{
	if(m_imageMesh)
	{
		m_imageMesh->SafeDelete();
		delete m_imageMesh;
		m_imageMesh = NULL;
	}

	//Delete all the scripts
	if(m_clickScript)
	{
		delete m_clickScript;
		m_clickScript = NULL;
	}
	if(m_mouseOverScript)
	{
		delete m_mouseOverScript;
		m_mouseOverScript = NULL;
	}
	if(m_mouseOutScript)
	{
		delete m_mouseOutScript;
		m_mouseOutScript = NULL;
	}
	if(m_dropScript)
	{
		delete m_dropScript;
		m_dropScript = NULL;
	}

    if(m_updateScript)
	{
		delete m_updateScript;
		m_updateScript = 0;
	}

	if(m_optionalDynTexture)
	{
       m_optionalDynTexture->Release();
	   m_optionalDynTexture = 0;
	}
	if(m_surfaceController)
	{
       m_surfaceController->Release();
	   m_surfaceController = 0;
	}
}

void CMenuImage::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		xml.WriteAttribute("Version", 1);
		xml.WriteAttribute("TexturePositionX", (int)m_texturePosX);
		xml.WriteAttribute("TexturePositionY", (int)m_texturePosY);
		xml.WriteAttribute("TextureWidth", (int)m_textureWidth);
		xml.WriteAttribute("TextureHeight", (int)m_textureHeight);
		xml.WriteAttribute("FullTextureWidth", (int)m_fullTextureWidth);
		xml.WriteAttribute("FullTextureHeight", (int)m_fullTextureHeight);
		xml.WriteAttribute("Red", (int)(m_red * 255));
		xml.WriteAttribute("Green", (int)(m_green * 255));
		xml.WriteAttribute("Blue", (int)(m_blue * 255));
		xml.WriteAttribute("Alpha", (int)(m_alpha * 255));
		xml.WriteAttribute("BlendMode", m_blendMode);
		xml.WriteAttribute("TextureFile", m_textureFile.GetBuffer());

		//Write out the click script
		if(m_clickScript)
		if(!m_clickScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("ClickScript");
			xml.WriteMultilinedAttribute(m_clickScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("ClickScript");
		}

		//Write out the mouse over script
		if(m_mouseOverScript)
		if(!m_mouseOverScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("MouseOverScript");
			xml.WriteMultilinedAttribute(m_mouseOverScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("MouseOverScript");
		}

		//Write out the mouse out script
		if(m_mouseOutScript)
		if(!m_mouseOutScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("MouseOutScript");
			xml.WriteMultilinedAttribute(m_mouseOutScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("MouseOutScript");
		}

		//Write out the drop script
		if(m_dropScript)
		if(!m_dropScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("DropScript");
			xml.WriteMultilinedAttribute(m_dropScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("DropScript");
		}

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
		xml.ReadAttribute(m_texturePosX);
		xml.ReadAttribute(m_texturePosY);
		xml.ReadAttribute(m_textureWidth);
		xml.ReadAttribute(m_textureHeight);
		xml.ReadAttribute(m_fullTextureWidth);
		xml.ReadAttribute(m_fullTextureHeight);
		xml.ReadAttribute(m_red); m_red /= 255.0f;
		xml.ReadAttribute(m_green); m_green /= 255.0f;
		xml.ReadAttribute(m_blue); m_blue /= 255.0f;
		xml.ReadAttribute(m_alpha); m_alpha /= 255.0f;
		xml.ReadAttribute(m_blendMode);
		xml.ReadAttribute(buffer, MAX_BUFFER); m_textureFile = buffer;

		//Read in <ClickScript>, <MouseOverScript>, <MouseOutScript>, <DropScript>, <UpdateScript>, or </Image>
		xml.ReadTag(buffer, MAX_BUFFER, closing);
		if(strcmp(buffer, "ClickScript") == 0)
		{
			//We've got a click script, create a new script instance
			SafeDelete(m_clickScript);
			m_clickScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </ClickScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </ClickScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_clickScript->AddLine(buffer);
			}

			//Read in <MouseOverScript>, <MouseOutScript>, <DropScript>, <UpdateScript>, or </Image>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If the MouseOverScript is present
		if(strcmp(buffer, "MouseOverScript") == 0)
		{
			//We've got a mouseOver script, create a new script instance
			SafeDelete(m_mouseOverScript);
			m_mouseOverScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </MouseOverScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </MouseOverScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_mouseOverScript->AddLine(buffer);
			}

			//Read in <MouseOutScript>, <DropScript>, <UpdateScript>, or </Image>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If the MouseOutScript is present
		if(strcmp(buffer, "MouseOutScript") == 0)
		{
			//We've got a mouseOut script, create a new script instance
			SafeDelete(m_mouseOutScript);
			m_mouseOutScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </MouseOutScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </MouseOutScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_mouseOutScript->AddLine(buffer);
			}

			//Read in <UpdateScript>, <DropScript>, or </Image>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If the DropScript is present
		if(strcmp(buffer, "DropScript") == 0)
		{
			//We've got a drop script, create a new script instance
			SafeDelete(m_dropScript);
			m_dropScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </DropScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </DropScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_dropScript->AddLine(buffer);
			}

			//Read in <UpdateScript> or </Image>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If the UpdateScript is present
		if(strcmp(buffer, "UpdateScript") == 0)
		{
			//We've got a update script, create a new script instance
			SafeDelete(m_updateScript);
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

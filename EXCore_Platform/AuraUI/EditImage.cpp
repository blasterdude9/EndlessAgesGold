// C:\Projects\EXCore_Platform\AuraUI\EditImage.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditImage.h"
#include "EditScript.h"

// CEditImage dialog
IMPLEMENT_DYNAMIC(CEditImage, CDialog)

CEditImage::CEditImage(CWnd* pParent /*=NULL*/)
	: CDialog(CEditImage::IDD, pParent)
{
	m_pImage			= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
}

BEGIN_MESSAGE_MAP(CEditImage, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Image Coordinates Edit
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)

	//Texture Edit
	ON_EN_CHANGE(IDC_EDIT_ImageTextureX, OnChangeTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ImageTextureY, OnChangeTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ImageTextureWidth, OnChangeTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_ImageTextureHeight, OnChangeTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditImageTexture, OnButtonEditTexture)
	
	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditClickScript, OnButtonEditClickScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOverScript, OnButtonEditMouseOverScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOutScript, OnButtonEditMouseOutScript)
	ON_BN_CLICKED(IDC_BUTTON_EditDropScript, OnButtonEditDropScript)
	ON_BN_CLICKED(IDC_BUTTON_EditUpdateScript, OnButtonEditUpdateScript)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditImage::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pImage->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pImage->m_visualID);

	//Image Coordinates
	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pImage->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pImage->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, m_pImage->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, m_pImage->m_height);*/


	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)m_pImage->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)m_pImage->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)m_pImage->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)m_pImage->m_height);

	//Image Texture Coordinates
	/*SetDlgItemInt(IDC_EDIT_ImageTextureX, m_pImage->m_texturePosX);
	SetDlgItemInt(IDC_EDIT_ImageTextureY, m_pImage->m_texturePosY);
	SetDlgItemInt(IDC_EDIT_ImageTextureWidth, m_pImage->m_textureWidth);
	SetDlgItemInt(IDC_EDIT_ImageTextureHeight, m_pImage->m_textureHeight);*/

	SetDlgItemInt(IDC_EDIT_ImageTextureX, (UINT)m_pImage->m_texturePosX);
	SetDlgItemInt(IDC_EDIT_ImageTextureY, (UINT)m_pImage->m_texturePosY);
	SetDlgItemInt(IDC_EDIT_ImageTextureWidth, (UINT)m_pImage->m_textureWidth);
	SetDlgItemInt(IDC_EDIT_ImageTextureHeight, (UINT)m_pImage->m_textureHeight);

	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	m_initialized = true;

	return TRUE;
}

void CEditImage::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditImage::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pImage->m_posX	= (int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pImage->m_posY	= (int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pImage->m_width	= (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pImage->m_height	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

	m_pImage->m_posX	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pImage->m_posY	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pImage->m_width	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pImage->m_height	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditImage::OnChangeTextureCoords()
{
	if(!m_initialized)
		return;

	/*m_pImage->m_texturePosX		= (int)GetDlgItemInt(IDC_EDIT_ImageTextureX);
	m_pImage->m_texturePosY		= (int)GetDlgItemInt(IDC_EDIT_ImageTextureY);
	m_pImage->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_ImageTextureWidth);
	m_pImage->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_ImageTextureHeight);*/

	m_pImage->m_texturePosX		= (float)(int)GetDlgItemInt(IDC_EDIT_ImageTextureX);
	m_pImage->m_texturePosY		= (float)(int)GetDlgItemInt(IDC_EDIT_ImageTextureY);
	m_pImage->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_ImageTextureWidth);
	m_pImage->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_ImageTextureHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
	
	UpdateTextureCoords();
}

void CEditImage::OnButtonEditTexture()
{
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.meshRef = m_pImage->m_imageMesh;

	if(dlg.DoModal() == IDOK)
	{
		m_pImage->m_imageMesh->List_materialObject->Reset();

		//Copy new material and texture properties
		m_pImage->m_red				= m_pImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.r;
		m_pImage->m_green			= m_pImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.g;
		m_pImage->m_blue			= m_pImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.b;
		m_pImage->m_alpha			= m_pImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.a;
		m_pImage->m_blendMode		= m_pImage->m_imageMesh->List_materialObject->m_blendMode;
		m_pImage->m_textureSelect	= m_pImage->m_imageMesh->List_textureSelect[0];

		UpdateTextureCoords();
	}
}

void CEditImage::OnButtonEditClickScript()
{
	if(!m_pImage->m_clickScript)
	{
		m_pImage->m_clickScript					 = new CMenuScript();
		m_pImage->m_clickScript->m_callingVisual = (CMenuVisual*)m_pImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pImage->m_clickScript;
	dlg.DoModal();
}

void CEditImage::OnButtonEditMouseOverScript()
{
	if(!m_pImage->m_mouseOverScript)
	{
		m_pImage->m_mouseOverScript					 = new CMenuScript();
		m_pImage->m_mouseOverScript->m_callingVisual = (CMenuVisual*)m_pImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pImage->m_mouseOverScript;
	dlg.DoModal();
}

void CEditImage::OnButtonEditMouseOutScript()
{
	if(!m_pImage->m_mouseOutScript)
	{
		m_pImage->m_mouseOutScript					= new CMenuScript();
		m_pImage->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)m_pImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pImage->m_mouseOutScript;
	dlg.DoModal();
}

void CEditImage::OnButtonEditDropScript()
{
	if(!m_pImage->m_dropScript)
	{
		m_pImage->m_dropScript					= new CMenuScript();
		m_pImage->m_dropScript->m_callingVisual	= (CMenuVisual*)m_pImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pImage->m_dropScript;
	dlg.DoModal();
}

void CEditImage::OnButtonEditUpdateScript()
{
	if(!m_pImage->m_updateScript)
	{
		m_pImage->m_updateScript					= new CMenuScript();
		m_pImage->m_updateScript->m_callingVisual	= (CMenuVisual*)m_pImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pImage->m_updateScript;
	dlg.DoModal();
}

void CEditImage::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pImage->m_name = temp;

	m_pImage->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);

	CDialog::OnOK();
}

void CEditImage::UpdateTextureCoords()
{
	//Get the texture dimensions
	float textureWidth = 512.0f;
	float textureHeight = 512.0f;
	char *textureName	= NULL;

	//Get the dimensions of the texture they selected, if any
	POSITION pos = m_gStatus->m_textureDataBase->FindIndex(m_pImage->m_imageMesh->List_textureSelect[0]);
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
		m_pImage->m_textureFile = textureName;

	//Update mesh's texture coordinates
	m_pImage->m_fullTextureWidth	= textureWidth;
	m_pImage->m_fullTextureHeight	= textureHeight;
	m_pImage->UpdateTextureCoords();
}

// CEditImage message handlers

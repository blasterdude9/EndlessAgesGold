// C:\Projects\EXCore_Platform\AuraUI\EditToggle.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditToggle.h"
#include "EditScript.h"

// CEditToggle dialog
IMPLEMENT_DYNAMIC(CEditToggle, CDialog)

CEditToggle::CEditToggle(CWnd* pParent /*=NULL*/)
	: CDialog(CEditToggle::IDD, pParent)
{
	m_pToggle			= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
	m_freezeInput		= NULL;
}

BEGIN_MESSAGE_MAP(CEditToggle, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Control Properties
	ON_EN_CHANGE(IDC_EDIT_GroupID, OnChangeGroupID)
	ON_EN_CHANGE(IDC_EDIT_ToggleValue, OnChangeToggleValue)
	ON_EN_CHANGE(IDC_EDIT_ToggleText, OnChangeToggleText)

	//Progress Bar Coordinates
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)

	//Up State Texture
	ON_EN_CHANGE(IDC_EDIT_UpStateTextureX, OnChangeUpStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_UpStateTextureY, OnChangeUpStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditUpStateTexture, OnButtonEditUpStateTexture)

	//Up Hover State Texture
	ON_EN_CHANGE(IDC_EDIT_UpHoverStateTextureX, OnChangeUpHoverStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_UpHoverStateTextureY, OnChangeUpHoverStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditUpHoverStateTexture, OnButtonEditUpHoverStateTexture)

	//Down State Texture
	ON_EN_CHANGE(IDC_EDIT_DownStateTextureX, OnChangeDownStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DownStateTextureY, OnChangeDownStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditDownStateTexture, OnButtonEditDownStateTexture)

	//Down Hover State Texture
	ON_EN_CHANGE(IDC_EDIT_DownHoverStateTextureX, OnChangeDownHoverStateTextureCoords)
	ON_EN_CHANGE(IDC_EDIT_DownHoverStateTextureY, OnChangeDownHoverStateTextureCoords)
	ON_BN_CLICKED(IDC_BUTTON_EditDownHoverStateTexture, OnButtonEditDownHoverStateTexture)

	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditClickScript, OnButtonEditClickScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOverScript, OnButtonEditMouseOverScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOutScript, OnButtonEditMouseOutScript)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditToggle::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pToggle->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pToggle->m_visualID);

	//Control Properties
	SetDlgItemInt(IDC_EDIT_GroupID, m_pToggle->m_iGroupID);
	SetDlgItemText(IDC_EDIT_ToggleValue, m_pToggle->m_strValue);
	SetDlgItemText(IDC_EDIT_ToggleText, m_pToggle->m_pDisplayText->m_text);

	//Progress Bar Coordinates
	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pToggle->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pToggle->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, m_pToggle->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, m_pToggle->m_height);*/


	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)m_pToggle->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)m_pToggle->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)m_pToggle->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)m_pToggle->m_height);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Up State Texture
	/*SetDlgItemInt(IDC_EDIT_UpStateTextureX, m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_texturePosX);
	SetDlgItemInt(IDC_EDIT_UpStateTextureY, m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_texturePosY);*/

	SetDlgItemInt(IDC_EDIT_UpStateTextureX, (UINT)m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_texturePosX);
	SetDlgItemInt(IDC_EDIT_UpStateTextureY, (UINT)m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_texturePosY);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Up Hover State Texture
	if(m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER])
	{
		/*SetDlgItemInt(IDC_EDIT_UpHoverStateTextureX, m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_UpHoverStateTextureY, m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_texturePosY);*/

		SetDlgItemInt(IDC_EDIT_UpHoverStateTextureX, (UINT)m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_UpHoverStateTextureY, (UINT)m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_texturePosY);		
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

	//Down State Texture
	/*SetDlgItemInt(IDC_EDIT_DownStateTextureX, m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_texturePosX);
	SetDlgItemInt(IDC_EDIT_DownStateTextureY, m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_texturePosY);*/

	SetDlgItemInt(IDC_EDIT_DownStateTextureX, (UINT)m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_texturePosX);
	SetDlgItemInt(IDC_EDIT_DownStateTextureY, (UINT)m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_texturePosY);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Down Hover State Texture
	if(m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER])
	{
		/*SetDlgItemInt(IDC_EDIT_DownHoverStateTextureX, m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DownHoverStateTextureY, m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_texturePosY);*/

		SetDlgItemInt(IDC_EDIT_DownHoverStateTextureX, (UINT)m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_texturePosX);
		SetDlgItemInt(IDC_EDIT_DownHoverStateTextureY, (UINT)m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_texturePosY);
		//WARNING: C4244 --> conversion from float to UINT, possible loss of data
	}

   *m_freezeInput = true;
	m_initialized = true;

	return TRUE;
}

void CEditToggle::OnChangeGroupID()
{
	m_pToggle->m_iGroupID = (int)GetDlgItemInt(IDC_EDIT_GroupID);
}

void 
CEditToggle::OnChangeToggleValue()
{
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ToggleValue);
	LPTSTR strValue = m_pToggle->m_strValue.GetBuffer(MAX_BUFFER);
	pEdit->GetWindowTextA(strValue, MAX_BUFFER);
	m_pToggle->m_strValue.ReleaseBuffer();
}

void 
CEditToggle::OnChangeToggleText()
{
	if(m_pToggle->m_pDisplayText)
	{
		LPTSTR strDisplay = new TCHAR[MAX_BUFFER];
		CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ToggleText);
		pEdit->GetWindowTextA(strDisplay, MAX_BUFFER);
		m_pToggle->m_pDisplayText->SetText(strDisplay);
		SafeDelete(strDisplay);
	}
}

void CEditToggle::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditToggle::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pToggle->m_posX	= (int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pToggle->m_posY	= (int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pToggle->m_width	= (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pToggle->m_height = (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

	m_pToggle->m_posX	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pToggle->m_posY	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pToggle->m_width	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pToggle->m_height = (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	//Update each states texture coordinates
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_width			= m_pToggle->m_width;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_height			= m_pToggle->m_height;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_textureWidth	= m_pToggle->m_width;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_textureHeight	= m_pToggle->m_height;
	UpdateImageTextureCoords(m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]);

	if(m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER])
	{
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_width			= m_pToggle->m_width;
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_height		= m_pToggle->m_height;
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_textureWidth	= m_pToggle->m_width;
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_textureHeight	= m_pToggle->m_height;
		UpdateImageTextureCoords(m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]);
	}

	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_width		 = m_pToggle->m_width;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_height		 = m_pToggle->m_height;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_textureWidth	 = m_pToggle->m_width;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_textureHeight = m_pToggle->m_height;
	UpdateImageTextureCoords(m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]);

	if(m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER])
	{
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_width			= m_pToggle->m_width;
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_height			= m_pToggle->m_height;
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_textureWidth	= m_pToggle->m_width;
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_textureHeight	= m_pToggle->m_height;
		UpdateImageTextureCoords(m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]);
	}
}

void CEditToggle::OnChangeUpStateTextureCoords()
{
	if(!m_initialized)
		return;

	/*m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_texturePosX	= (int)GetDlgItemInt(IDC_EDIT_UpStateTextureX);
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_texturePosY	= (int)GetDlgItemInt(IDC_EDIT_UpStateTextureY);*/

	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_texturePosX	= (float)(int)GetDlgItemInt(IDC_EDIT_UpStateTextureX);
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]->m_texturePosY	= (float)(int)GetDlgItemInt(IDC_EDIT_UpStateTextureY);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pToggle->m_state = MENU_TOGGLESTATE_UP;

	UpdateImageTextureCoords(m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]);
}

void CEditToggle::OnButtonEditUpStateTexture()
{
	if(m_pToggle->m_state != MENU_TOGGLESTATE_UP)
	{
		m_pToggle->m_state = MENU_TOGGLESTATE_UP;
		return;
	}

	EditTexture(m_pToggle->m_stateImages[MENU_TOGGLESTATE_UP]);
}

void CEditToggle::OnChangeUpHoverStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER])
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER] = new CMenuImage();

	/*m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_texturePosX	= (int)GetDlgItemInt(IDC_EDIT_UpHoverStateTextureX);
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_texturePosY	= (int)GetDlgItemInt(IDC_EDIT_UpHoverStateTextureY);*/

	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_texturePosX	= (float)(int)GetDlgItemInt(IDC_EDIT_UpHoverStateTextureX);
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_texturePosY	= (float)(int)GetDlgItemInt(IDC_EDIT_UpHoverStateTextureY);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_width			= m_pToggle->m_width;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_height		= m_pToggle->m_height;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_textureWidth	= m_pToggle->m_width;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]->m_textureHeight	= m_pToggle->m_height;

	m_pToggle->m_state = MENU_TOGGLESTATE_UPHOVER;

	UpdateImageTextureCoords(m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]);
}

void CEditToggle::OnButtonEditUpHoverStateTexture()
{
	m_pToggle->m_state = MENU_TOGGLESTATE_UPHOVER;

	if(!m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER])
	{
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER] = new CMenuImage();
		return;
	}

	EditTexture(m_pToggle->m_stateImages[MENU_TOGGLESTATE_UPHOVER]);
}

void CEditToggle::OnChangeDownStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN])
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN] = new CMenuImage();

	/*m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_texturePosX	 = (int)GetDlgItemInt(IDC_EDIT_DownStateTextureX);
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_texturePosY	 = (int)GetDlgItemInt(IDC_EDIT_DownStateTextureY);*/

	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_texturePosX	 = (float)(int)GetDlgItemInt(IDC_EDIT_DownStateTextureX);
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]->m_texturePosY	 = (float)(int)GetDlgItemInt(IDC_EDIT_DownStateTextureY);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pToggle->m_state = MENU_TOGGLESTATE_DOWN;

	UpdateImageTextureCoords(m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]);
}

void CEditToggle::OnButtonEditDownStateTexture()
{
	if(m_pToggle->m_state != MENU_TOGGLESTATE_DOWN)
	{
		m_pToggle->m_state = MENU_TOGGLESTATE_DOWN;
		return;
	}

	EditTexture(m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWN]);
}

void CEditToggle::OnChangeDownHoverStateTextureCoords()
{
	if(!m_initialized)
		return;

	if(!m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER])
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER] = new CMenuImage();

	/*m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_texturePosX		= (int)GetDlgItemInt(IDC_EDIT_DownHoverStateTextureX);
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_texturePosY		= (int)GetDlgItemInt(IDC_EDIT_DownHoverStateTextureY);*/

	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_texturePosX		= (float)(int)GetDlgItemInt(IDC_EDIT_DownHoverStateTextureX);
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_texturePosY		= (float)(int)GetDlgItemInt(IDC_EDIT_DownHoverStateTextureY);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_width			= m_pToggle->m_width;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_height			= m_pToggle->m_height;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_textureWidth	= m_pToggle->m_width;
	m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]->m_textureHeight	= m_pToggle->m_height;

	m_pToggle->m_state = MENU_TOGGLESTATE_DOWNHOVER;

	UpdateImageTextureCoords(m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]);
}

void CEditToggle::OnButtonEditDownHoverStateTexture()
{
	m_pToggle->m_state = MENU_TOGGLESTATE_DOWNHOVER;

	if(!m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER])
	{
		m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER] = new CMenuImage();
		return;
	}

	EditTexture(m_pToggle->m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]);
}

void CEditToggle::OnButtonEditClickScript()
{
	if(!m_pToggle->m_clickScript)
	{
		m_pToggle->m_clickScript					= new CMenuScript();
		m_pToggle->m_clickScript->m_callingVisual	= (CMenuVisual*)m_pToggle;
	}

	CEditScript dlg;
	dlg.m_script = m_pToggle->m_clickScript;
	dlg.DoModal();
}

void CEditToggle::OnButtonEditMouseOverScript()
{
	if(!m_pToggle->m_mouseOverScript)
	{
		m_pToggle->m_mouseOverScript					= new CMenuScript();
		m_pToggle->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)m_pToggle;
	}

	CEditScript dlg;
	dlg.m_script = m_pToggle->m_mouseOverScript;
	dlg.DoModal();
}

void CEditToggle::OnButtonEditMouseOutScript()
{
	if(!m_pToggle->m_mouseOutScript)
	{
		m_pToggle->m_mouseOutScript					 = new CMenuScript();
		m_pToggle->m_mouseOutScript->m_callingVisual = (CMenuVisual*)m_pToggle;
	}

	CEditScript dlg;
	dlg.m_script = m_pToggle->m_mouseOutScript;
	dlg.DoModal();
}

void CEditToggle::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pToggle->m_name = temp;

	m_pToggle->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);
	*m_freezeInput = false;

	CDialog::OnOK();
}

void CEditToggle::EditTexture(CMenuImage *pImage)
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

void CEditToggle::UpdateImageTextureCoords(CMenuImage *pImage)
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
}

// CEditToggle message handlers

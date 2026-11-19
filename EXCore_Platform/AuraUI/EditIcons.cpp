// C:\Projects\EXCore_Platform\AuraUI\EditIcons.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditIcons.h"

// CEditIcons Dialog
IMPLEMENT_DYNAMIC(CEditIcons, CDialog)

CEditIcons::CEditIcons(CWnd* pParent /*=NULL*/)
	: CDialog(CEditIcons::IDD, pParent)
{
	m_pIconList			= NULL;
	m_pIconImage		= NULL;
	m_pIconWindow		= NULL;
	m_editImageDlg		= NULL;
	m_initialized		= true;
}

BEGIN_MESSAGE_MAP(CEditIcons, CDialog)
	//Fonts List
	ON_LBN_SELCHANGE(IDC_LIST_Icons, OnSelectionChangeListIcons)
	ON_BN_CLICKED(IDC_BUTTON_NewIcon, OnButtonNewIcon)
	ON_BN_CLICKED(IDC_BUTTON_EditIcon, OnButtonEditIcon)
	ON_BN_CLICKED(IDC_BUTTON_DeleteIcon, OnButtonDeleteIcon)

	//Icon Sheet Properties
	ON_BN_CLICKED(IDC_BUTTON_SelectIconSheetTexture, OnButtonSelectIconSheetTexture)
	ON_EN_CHANGE(IDC_EDIT_IconWidth, OnChangeIconSheetProperties)
	ON_EN_CHANGE(IDC_EDIT_IconHeight, OnChangeIconSheetProperties)
	ON_EN_CHANGE(IDC_EDIT_HorizontalSpacing, OnChangeIconSheetProperties)
	ON_EN_CHANGE(IDC_EDIT_VerticalSpacing, OnChangeIconSheetProperties)
	ON_BN_CLICKED(IDC_BUTTON_ImportIconSheet, OnButtonImportIconSheet)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditIcons::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pIconImage = new CMenuImage();
	m_pIconWindow->m_childList->AddTail(m_pIconImage);

	RefreshIconList(-1);

	return TRUE;
}

void CEditIcons::OnSelectionChangeListIcons()
{
	//If they have a icon selected
	int iconIndex = SendDlgItemMessage(IDC_LIST_Icons, LB_GETCURSEL);
	if(iconIndex != -1)
	{
		//Get a pointer to the selected icon
		POSITION posIcon = m_pIconList->FindIndex(iconIndex);
		if(posIcon)
		{
			CMenuImage *curIcon = (CMenuImage*)m_pIconList->GetAt(posIcon);
			m_pIconWindow->m_childList->RemoveAll();
			m_pIconWindow->m_childList->AddTail(curIcon);
		}

		RefreshIconList(iconIndex);
	}
}

void CEditIcons::OnButtonNewIcon()
{
	//Create a new object
	CMenuImage *newIcon = new CMenuImage();
	newIcon->m_name		= "New Icon";

	//Delete the old dialog
	if(m_editImageDlg)
		delete m_editImageDlg;

	//Create a new dialog
	m_editImageDlg					= new CEditImage();
	m_editImageDlg->m_pImage		= newIcon;
	m_editImageDlg->m_gStatus		= m_gStatus;
	m_editImageDlg->m_freeVisualID	= -1;

	//Open the dialog
	m_editImageDlg->Create(IDD_DIALOG_MenuSysEditImage);

	//Add it to the icon list
	m_pIconList->AddTail(newIcon);

	//Make our new icon the only visible icon
	m_pIconWindow->m_childList->RemoveAll();
	m_pIconWindow->m_childList->AddTail(newIcon);

	//Make our new icon show up in the list
	RefreshIconList(m_pIconList->GetCount() - 1);
}

void CEditIcons::OnButtonEditIcon()
{
	//Don't allow the editing of Icon properties
	m_initialized = false;

	//If they have a icon selected
	int iconIndex = SendDlgItemMessage(IDC_LIST_Icons, LB_GETCURSEL);
	if(iconIndex != -1)
	{
		//Get a pointer to the selected font
		POSITION posIcon = m_pIconList->FindIndex(iconIndex);
		if(posIcon)
		{
			CMenuImage *curIcon = (CMenuImage*)m_pIconList->GetAt(posIcon);

			//Delete the old dialog
			if(m_editImageDlg)
				delete m_editImageDlg;

			//Create a new dialog
			m_editImageDlg					= new CEditImage();
			m_editImageDlg->m_pImage		= curIcon;
			m_editImageDlg->m_gStatus		= m_gStatus;
			m_editImageDlg->m_freeVisualID	= -1;

			//Open the new dialog
			m_editImageDlg->Create(IDD_DIALOG_MenuSysEditImage);
		}
	}

	//Allow the editing of Icon properties
	m_initialized = true;
}

void CEditIcons::OnButtonDeleteIcon()
{
	//If they have a icon selected
	int iconIndex = SendDlgItemMessage(IDC_LIST_Icons, LB_GETCURSEL);
	if(iconIndex > -1)
	{
		//Get a pointer to the selected font
		POSITION posIcon = m_pIconList->FindIndex(iconIndex);
		if(posIcon)
		{
			if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
				return;

			CMenuVisual * mVis = (CMenuVisual*)m_pIconList->GetAt(posIcon);
			m_pIconList->DeleteByObject(mVis);
			m_pIconList->RemoveAt(posIcon);
			RefreshIconList(iconIndex);
		}
	}
}

void CEditIcons::OnButtonSelectIconSheetTexture()
{
	//Create a new material dialog
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef	= NULL;
	dlg.GL_textureDataBase	= m_gStatus->m_textureDataBase;
	dlg.meshRef = m_pIconImage->m_imageMesh;

	if(dlg.DoModal() == IDOK)
	{
		//Grab the image mesh and store it into the font
		m_pIconImage->m_imageMesh->List_materialObject->Reset();

		//Default texture size
		float textureWidth	= 512.0f;
		float textureHeight = 512.0f;
		char *textureName	= NULL;
		
		//Get the dimensions of the texture they selected, if any
		POSITION pos = m_gStatus->m_textureDataBase->FindIndex(m_pIconImage->m_imageMesh->List_textureSelect[0]);
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
		m_pIconImage->m_red				= m_pIconImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.r;
		m_pIconImage->m_green			= m_pIconImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.g;
		m_pIconImage->m_blue			= m_pIconImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.b;
		m_pIconImage->m_alpha			= m_pIconImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.a;
		m_pIconImage->m_blendMode		= m_pIconImage->m_imageMesh->List_materialObject->m_blendMode;
		m_pIconImage->m_textureSelect	= m_pIconImage->m_imageMesh->List_textureSelect[0];

		//Copy texture file
		if(textureName)
			m_pIconImage->m_textureFile = textureName;

		//Update mesh's texture coordinates
		m_pIconImage->m_fullTextureWidth	= textureWidth;
		m_pIconImage->m_fullTextureHeight	= textureHeight;
		m_pIconImage->UpdateTextureCoords();
	}

	//Make sure its the only the visible icon
	m_pIconWindow->m_childList->RemoveAll();
	m_pIconWindow->m_childList->AddTail(m_pIconImage);
}

void CEditIcons::OnChangeIconSheetProperties()
{
	if(!m_initialized)
		return;

	//Grab the spacings
	/*float horizontalSpacing = (int)GetDlgItemInt(IDC_EDIT_HorizontalSpacing);
	float verticalSpacing	= (int)GetDlgItemInt(IDC_EDIT_VerticalSpacing);*/

	float horizontalSpacing = (float)(int)GetDlgItemInt(IDC_EDIT_HorizontalSpacing);
	float verticalSpacing	= (float)(int)GetDlgItemInt(IDC_EDIT_VerticalSpacing);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	//Pull the width and height
	/*m_pIconImage->m_width	= m_pIconImage->m_textureWidth	= (int)GetDlgItemInt(IDC_EDIT_IconWidth);
	m_pIconImage->m_height	= m_pIconImage->m_textureHeight	= (int)GetDlgItemInt(IDC_EDIT_IconHeight);	*/

	m_pIconImage->m_width	= m_pIconImage->m_textureWidth	= (float)(int)GetDlgItemInt(IDC_EDIT_IconWidth);
	m_pIconImage->m_height	= m_pIconImage->m_textureHeight	= (float)(int)GetDlgItemInt(IDC_EDIT_IconHeight);	
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	//If we have horizontal spacing, make the 2nd column of icons visible
	if(horizontalSpacing != 0.0f)
		m_pIconImage->m_texturePosX = m_pIconImage->m_textureWidth + horizontalSpacing;
	else
		m_pIconImage->m_texturePosX = 0.0f;
	
	//If we have vertical spacing, make the 2nd row of icons visible
	if(verticalSpacing != 0.0f)
		m_pIconImage->m_texturePosY = m_pIconImage->m_textureHeight + verticalSpacing;
	else
		m_pIconImage->m_texturePosY = 0.0f;

	//Make sure its the only the visible icon
	m_pIconWindow->m_childList->RemoveAll();
	m_pIconWindow->m_childList->AddTail(m_pIconImage);
}

void CEditIcons::OnButtonImportIconSheet()
{
	//Grab all the text data
	char  baseName[255];
	GetDlgItemText(IDC_EDIT_BaseName, baseName, 255);
	/*float iconWidth			= (int)GetDlgItemInt(IDC_EDIT_IconWidth);
	float iconHeight		= (int)GetDlgItemInt(IDC_EDIT_IconHeight);
	float horizontalSpacing = (int)GetDlgItemInt(IDC_EDIT_HorizontalSpacing);
	float verticalSpacing	= (int)GetDlgItemInt(IDC_EDIT_VerticalSpacing);*/

	float iconWidth			= (float)(int)GetDlgItemInt(IDC_EDIT_IconWidth);
	float iconHeight		= (float)(int)GetDlgItemInt(IDC_EDIT_IconHeight);
	float horizontalSpacing = (float)(int)GetDlgItemInt(IDC_EDIT_HorizontalSpacing);
	float verticalSpacing	= (float)(int)GetDlgItemInt(IDC_EDIT_VerticalSpacing);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	int	  numIcons			= (int)GetDlgItemInt(IDC_EDIT_NumberOfIcons);

	/*int   iconsPerRow		= m_pIconImage->m_fullTextureWidth / (iconWidth + horizontalSpacing);*/

	int   iconsPerRow		= (int)(m_pIconImage->m_fullTextureWidth / (iconWidth + horizontalSpacing));	
	//WARNING: C4244 --> conversion from float to int, possible loss of data

	int	  curVisualID		= (int)GetDlgItemInt(IDC_EDIT_BaseVisualID);

	//Loop through and create all the new icons
	int curIndex = 0;
	while(curIndex < numIcons)
	{
		//Create a new icon and copy the base properties from the icon image
		CMenuImage *newIcon = new CMenuImage();
		m_pIconImage->Copy((CMenuVisual*)newIcon, m_gStatus->m_textureDataBase->d3dDeviceRef);

		//Set this icons properties
		newIcon->m_texturePosX	= (curIndex % iconsPerRow) * (iconWidth + horizontalSpacing);
		newIcon->m_texturePosY	= (curIndex / iconsPerRow) * (iconHeight + verticalSpacing);
		newIcon->m_name			= baseName + m_gStatus->m_cabLib.ConvToString(curVisualID);
		newIcon->m_visualID		= curVisualID++;

		//Update the texture coordinates(puts them into the mesh, which needs 0.0f-1.0f coordinates)
		newIcon->UpdateTextureCoords();

		//Add it to the icon list
		m_pIconList->AddTail(newIcon);

		curIndex++;
	}	

	//Delete the icons from the list
	m_pIconWindow->m_childList->RemoveAll();

	//Refill the list with the new stuff
	RefreshIconList(m_pIconList->GetCount() - 1);
}

void CEditIcons::OnButtonNext()
{
	//Delete the image we created before we exit
	delete m_pIconImage;
	m_pIconImage = NULL;

	CDialog::OnOK();
}

void CEditIcons::RefreshIconList(int selection)
{
	//Clear the font list
	SendDlgItemMessage(IDC_LIST_Icons, LB_RESETCONTENT, 0, 0);

	//Refill the font list
	for(POSITION posCurIcon = m_pIconList->GetHeadPosition(); posCurIcon != NULL;)
	{
		CMenuImage *curIcon = (CMenuImage*)m_pIconList->GetNext(posCurIcon);
		SendDlgItemMessage(IDC_LIST_Icons, LB_ADDSTRING, 0, (LPARAM)(LPCTSTR)curIcon->m_name);
	}

	//Make sure we're not trying to select beyond the list
	if(selection >= m_pIconList->GetCount())
		selection = m_pIconList->GetCount() - 1;

	//Set the currently selected font
	if(selection != -1)
		SendDlgItemMessage(IDC_LIST_Icons, LB_SETCURSEL, selection, 0);
}

// CEditIcons message handlers

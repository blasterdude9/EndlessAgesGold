#include "stdafx.h"
#include "EditInventory.h"

IMPLEMENT_DYNAMIC(CEditInventory, CDialog)

CEditInventory::CEditInventory(CWnd* pParent): 
	CDialog(CEditInventory::IDD, pParent)
{
	m_pInventory		= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
}

BEGIN_MESSAGE_MAP(CEditInventory, CDialog)
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
	ON_BN_CLICKED(IDC_BUTTON_EditRightClickScript, OnButtonEditRightClickScript)

	//Regular Control Events
	ON_BN_CLICKED(IDOK, &CEditInventory::OnButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_ClearInventoryGrid, &CEditInventory::OnBnClickedButtonClearInventoryGrid)
	ON_BN_CLICKED(IDC_BUTTON_CreateInventoryGrid, &CEditInventory::OnBnClickedButtonCreateInventoryGrid)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_InventoryNumRows, &CEditInventory::OnDeltaposSpinInventoryNumRows)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_InventoryNumColumns, &CEditInventory::OnDeltaposSpinInventoryNumColumns)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_InventoryRowSpacing, &CEditInventory::OnDeltaposSpinInventoryRowSpacing)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_InventoryColumnSpacing, &CEditInventory::OnDeltaposSpinInventoryColumnSpacing)
	ON_EN_UPDATE(IDC_EDIT_InventoryNumRows, &CEditInventory::OnChangeInventoryNumRows)
	ON_EN_UPDATE(IDC_EDIT_InventoryNumColumns, &CEditInventory::OnChangeInventoryNumColumns)
	ON_EN_UPDATE(IDC_EDIT_InventoryRowSpacing, &CEditInventory::OnChangeInventoryRowSpacing)
	ON_EN_UPDATE(IDC_EDIT_InventoryColumnSpacing, &CEditInventory::OnChangeInventoryColumnSpacing)
	ON_BN_CLICKED(IDC_BUTTON_EditPrototypeClickScript, &CEditInventory::OnBnClickedButtonEditPrototypeClickScript)
	ON_BN_CLICKED(IDC_BUTTON_EditPrototypeMouseOverScript, &CEditInventory::OnBnClickedButtonEditPrototypeMouseOverScript)
	ON_BN_CLICKED(IDC_BUTTON_EditPrototypeMouseOutScript, &CEditInventory::OnBnClickedButtonEditPrototypeMouseOutScript)
	ON_BN_CLICKED(IDC_BUTTON_EditPrototypeDropScript, &CEditInventory::OnBnClickedButtonEditPrototypeDropScript)
	ON_BN_CLICKED(IDC_BUTTON_EditPrototypeUpdateScript, &CEditInventory::OnBnClickedButtonEditPrototypeUpdateScript)
END_MESSAGE_MAP()

BOOL CEditInventory::OnInitDialog()
{
	CDialog::OnInitDialog();

	// KLUDGE: This button's text is getting set to 0 somewhere. WHY?!
	CButton *pButton = reinterpret_cast<CButton*>(GetDlgItem(IDC_BUTTON_ClearInventoryGrid));
	pButton->SetWindowTextA("Clear");

	// SET buddy controls and ranges for the spin buttons
	CEdit *pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_InventoryNumRows));
	CSpinButtonCtrl *pSpin = reinterpret_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_SPIN_InventoryNumRows));
	if (pSpin && pEdit)
	{
		pSpin->SetBuddy(pEdit);
		pSpin->SetRange(SPIN_MIN_RANGE, SPIN_MAX_RANGE);
	}

	pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_InventoryNumColumns));
	pSpin = reinterpret_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_SPIN_InventoryNumColumns));
	if (pSpin && pEdit)
	{
		pSpin->SetBuddy(pEdit);
		pSpin->SetRange(SPIN_MIN_RANGE, SPIN_MAX_RANGE);
	}

	pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_InventoryRowSpacing));
	pSpin = reinterpret_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_SPIN_InventoryRowSpacing));
	if (pSpin && pEdit)
	{
		pSpin->SetBuddy(pEdit);
		pSpin->SetRange(SPIN_MIN_RANGE, SPIN_MAX_RANGE);
	}

	pEdit = reinterpret_cast<CEdit*>(GetDlgItem(IDC_EDIT_InventoryColumnSpacing));
	pSpin = reinterpret_cast<CSpinButtonCtrl*>(GetDlgItem(IDC_SPIN_InventoryColumnSpacing));
	if (pSpin && pEdit)
	{
		pSpin->SetBuddy(pEdit);
		pSpin->SetRange(SPIN_MIN_RANGE, SPIN_MAX_RANGE);
	}

	// Menu Properties
	SetDlgItemText(IDC_EDIT_VisualName, this->m_pInventory->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, this->m_pInventory->m_visualID);
	SetDlgItemInt(IDC_EDIT_InventoryNumRows, this->m_pInventory->m_iNumRows);
	SetDlgItemInt(IDC_EDIT_InventoryNumColumns, this->m_pInventory->m_iNumColumns);
	SetDlgItemInt(IDC_EDIT_InventoryRowSpacing, this->m_pInventory->m_iRowSpacing);
	SetDlgItemInt(IDC_EDIT_InventoryColumnSpacing, this->m_pInventory->m_iColumnSpacing);
	SetDlgItemInt(IDC_SPIN_InventoryNumRows, this->m_pInventory->m_iNumRows);
	SetDlgItemInt(IDC_SPIN_InventoryNumColumns, this->m_pInventory->m_iNumColumns);
	SetDlgItemInt(IDC_SPIN_InventoryRowSpacing, this->m_pInventory->m_iRowSpacing);
	SetDlgItemInt(IDC_SPIN_InventoryColumnSpacing, this->m_pInventory->m_iColumnSpacing);

	// Menu Coordinates
	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)this->m_pInventory->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)this->m_pInventory->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)this->m_pInventory->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)this->m_pInventory->m_height);

	// Prototype Image Coordinates
	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)this->m_pInventory->m_pPrototypeImage->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)this->m_pInventory->m_pPrototypeImage->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)this->m_pInventory->m_pPrototypeImage->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)this->m_pInventory->m_pPrototypeImage->m_height);

	// Prototype Image Texture Coordinates
	SetDlgItemInt(IDC_EDIT_ImageTextureX, (UINT)this->m_pInventory->m_pPrototypeImage->m_texturePosX);
	SetDlgItemInt(IDC_EDIT_ImageTextureY, (UINT)this->m_pInventory->m_pPrototypeImage->m_texturePosY);
	SetDlgItemInt(IDC_EDIT_ImageTextureWidth, (UINT)this->m_pInventory->m_pPrototypeImage->m_textureWidth);
	SetDlgItemInt(IDC_EDIT_ImageTextureHeight, (UINT)this->m_pInventory->m_pPrototypeImage->m_textureHeight);

	// We're INITIALIZED and done!
	m_initialized = true;
	return TRUE;
}

void CEditInventory::OnButtonGetFreeID()
{
	//this->m_freeVisualID = this->m_gStatus->m_menuDatabase->GetFreeID(-1);
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditInventory::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	this->m_pInventory->m_pPrototypeImage->m_posX	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	this->m_pInventory->m_pPrototypeImage->m_posY	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	this->m_pInventory->m_pPrototypeImage->m_width	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	this->m_pInventory->m_pPrototypeImage->m_height	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	UpdateTextureCoords();
}

void CEditInventory::OnChangeTextureCoords()
{
	if(!m_initialized)
		return;

	this->m_pInventory->m_pPrototypeImage->m_texturePosX = (float)(int)GetDlgItemInt(IDC_EDIT_ImageTextureX);
	this->m_pInventory->m_pPrototypeImage->m_texturePosY = (float)(int)GetDlgItemInt(IDC_EDIT_ImageTextureY);
	this->m_pInventory->m_pPrototypeImage->m_textureWidth = (float)(int)GetDlgItemInt(IDC_EDIT_ImageTextureWidth);
	this->m_pInventory->m_pPrototypeImage->m_textureHeight = (float)(int)GetDlgItemInt(IDC_EDIT_ImageTextureHeight);
	UpdateTextureCoords();
}

void CEditInventory::OnButtonEditTexture()
{
	if (!m_pInventory->m_pPrototypeImage)
		this->m_pInventory->m_pPrototypeImage = new CMenuImage();

	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = m_gStatus->m_textureDataBase;
	dlg.meshRef = m_pInventory->m_pPrototypeImage->m_imageMesh;

	this->m_pInventory->m_pPrototypeImage->UpdateTextureCoords();	
	if(dlg.DoModal() == IDOK)
	{
		m_pInventory->m_pPrototypeImage->m_imageMesh->List_materialObject->Reset();

		//Copy new material and texture properties
		m_pInventory->m_pPrototypeImage->m_red				= m_pInventory->m_pPrototypeImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.r;
		m_pInventory->m_pPrototypeImage->m_green			= m_pInventory->m_pPrototypeImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.g;
		m_pInventory->m_pPrototypeImage->m_blue				= m_pInventory->m_pPrototypeImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.b;
		m_pInventory->m_pPrototypeImage->m_alpha			= m_pInventory->m_pPrototypeImage->m_imageMesh->List_materialObject->m_useMaterial.Diffuse.a;
		m_pInventory->m_pPrototypeImage->m_blendMode		= m_pInventory->m_pPrototypeImage->m_imageMesh->List_materialObject->m_blendMode;
		m_pInventory->m_pPrototypeImage->m_textureSelect	= m_pInventory->m_pPrototypeImage->m_imageMesh->List_textureSelect[0];

		this->UpdateTextureCoords();
	}
}

void CEditInventory::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pInventory->m_name = temp;
	m_pInventory->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);
	CDialog::OnOK();
}

void CEditInventory::UpdateTextureCoords()
{
	//Get the texture dimensions
	float textureWidth = 512.0f;
	float textureHeight = 512.0f;
	char *textureName = NULL;

	//Get the dimensions of the texture they selected, if any
	POSITION pos = m_gStatus->m_textureDataBase->FindIndex(m_pInventory->m_pPrototypeImage->m_imageMesh->List_textureSelect[0]);
	if(pos != NULL)
	{
		CTextureDBObj *texture	= (CTextureDBObj*)m_gStatus->m_textureDataBase->GetAt(pos);
		textureWidth = (float)texture->m_textureObject->m_widthBasis;
		textureHeight = (float)texture->m_textureObject->m_hieghtBasis;
		textureName = texture->List_textureName;
	}

	//Copy texture file
	if(textureName)
		m_pInventory->m_pPrototypeImage->m_textureFile = textureName;

	//Update mesh's texture coordinates
	m_pInventory->m_pPrototypeImage->m_fullTextureWidth	= textureWidth;
	m_pInventory->m_pPrototypeImage->m_fullTextureHeight = textureHeight;
	m_pInventory->m_pPrototypeImage->UpdateTextureCoords();
}

void CEditInventory::OnBnClickedButtonClearInventoryGrid()
{
	this->m_pInventory->ClearGrid();
}

void CEditInventory::OnBnClickedButtonCreateInventoryGrid()
{
	this->m_pInventory->CreateGrid();
}

void CEditInventory::OnDeltaposSpinInventoryNumRows(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!this->m_initialized)
		return;
	int iValue = GetDlgItemInt(IDC_SPIN_InventoryNumRows);
	this->m_pInventory->m_iNumRows = iValue; 
	SetDlgItemInt(IDC_EDIT_InventoryNumRows, this->m_pInventory->m_iNumRows);
}

void CEditInventory::OnDeltaposSpinInventoryNumColumns(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!this->m_initialized)
		return;
	int iValue = GetDlgItemInt(IDC_SPIN_InventoryNumColumns);
	this->m_pInventory->m_iNumColumns = iValue; 
	SetDlgItemInt(IDC_EDIT_InventoryNumColumns, this->m_pInventory->m_iNumColumns);
}

void CEditInventory::OnDeltaposSpinInventoryRowSpacing(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!this->m_initialized)
		return;
	int iValue = GetDlgItemInt(IDC_SPIN_InventoryRowSpacing);
	this->m_pInventory->m_iRowSpacing = iValue; 
	SetDlgItemInt(IDC_EDIT_InventoryRowSpacing, this->m_pInventory->m_iRowSpacing);
}

void CEditInventory::OnDeltaposSpinInventoryColumnSpacing(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (!this->m_initialized)
		return;
	int iValue = GetDlgItemInt(IDC_SPIN_InventoryColumnSpacing);
	this->m_pInventory->m_iColumnSpacing = iValue; 
	SetDlgItemInt(IDC_EDIT_InventoryColumnSpacing, this->m_pInventory->m_iColumnSpacing);
}

void CEditInventory::OnChangeInventoryNumRows()
{
	int iValue = GetDlgItemInt(IDC_EDIT_InventoryNumRows);
	this->m_pInventory->m_iNumRows = iValue;
	SetDlgItemInt(IDC_SPIN_InventoryNumRows, iValue);
}

void CEditInventory::OnChangeInventoryNumColumns()
{
	int iValue = GetDlgItemInt(IDC_EDIT_InventoryNumColumns);
	this->m_pInventory->m_iNumColumns = iValue;
	SetDlgItemInt(IDC_SPIN_InventoryNumColumns, iValue);
}

void CEditInventory::OnChangeInventoryRowSpacing()
{
	int iValue = GetDlgItemInt(IDC_EDIT_InventoryRowSpacing);
	this->m_pInventory->m_iRowSpacing = iValue;
	SetDlgItemInt(IDC_SPIN_InventoryRowSpacing, iValue);
}

void CEditInventory::OnChangeInventoryColumnSpacing()
{
	int iValue = GetDlgItemInt(IDC_EDIT_InventoryColumnSpacing);
	this->m_pInventory->m_iColumnSpacing = iValue;
	SetDlgItemInt(IDC_SPIN_InventoryColumnSpacing, iValue);
}

void CEditInventory::OnButtonEditClickScript()
{
	if(!m_pInventory->m_clickScript)
	{
		m_pInventory->m_clickScript = new CMenuScript();
		m_pInventory->m_clickScript->m_callingVisual = (CMenuVisual*)m_pInventory;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_clickScript;
	dlg.DoModal();
}

void CEditInventory::OnButtonEditRightClickScript()
{
	if(!m_pInventory->m_rightClickScript)
	{
		m_pInventory->m_rightClickScript = new CMenuScript();
		m_pInventory->m_rightClickScript->m_callingVisual = (CMenuVisual*)m_pInventory;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_rightClickScript;
	dlg.DoModal();
}

void CEditInventory::OnButtonEditMouseOverScript()
{
	if(!m_pInventory->m_mouseOverScript)
	{
		m_pInventory->m_mouseOverScript					 = new CMenuScript();
		m_pInventory->m_mouseOverScript->m_callingVisual = (CMenuVisual*)m_pInventory;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_mouseOverScript;
	dlg.DoModal();
}

void CEditInventory::OnButtonEditMouseOutScript()
{
#ifndef _ClientOutput
	ITEMDATA_STRUCT_EX item_01;
	item_01.m_GLID = 616;
	item_01.m_quantity = 1;
	item_01.m_armed = 0;
	item_01.m_index = 0;
	this->m_pInventory->AddItem(item_01);

	ITEMDATA_STRUCT_EX item_02;
	item_02.m_GLID = 205;
	item_02.m_quantity = 5;
	item_02.m_armed = 0;
	item_02.m_index = 1;
	this->m_pInventory->AddItem(item_02);

	ITEMDATA_STRUCT_EX item_03;
	item_03.m_GLID = 999;
	item_03.m_quantity = 5;
	item_03.m_armed = 0;
	item_03.m_index = 2;
	this->m_pInventory->AddItem(item_03);

	ITEMDATA_STRUCT_EX item_04;
	item_04.m_GLID = 998;
	item_04.m_quantity = 5;
	item_04.m_armed = 0;
	item_04.m_index = 3;
	this->m_pInventory->AddItem(item_04);

	ITEMDATA_STRUCT_EX item_05;
	item_05.m_GLID = 997;
	item_05.m_quantity = 5;
	item_05.m_armed = 0;
	item_05.m_index = 4;
	this->m_pInventory->AddItem(item_05);

	ITEMDATA_STRUCT_EX item_06;
	item_06.m_GLID = 996;
	item_06.m_quantity = 5;
	item_06.m_armed = 0;
	item_06.m_index = 5;
	this->m_pInventory->AddItem(item_06);

	ITEMDATA_STRUCT_EX item_07;
	item_07.m_GLID = 995;
	item_07.m_quantity = 5;
	item_07.m_armed = 0;
	item_07.m_index = 6;
	this->m_pInventory->AddItem(item_07);
#endif
	if(!m_pInventory->m_mouseOutScript)
	{
		m_pInventory->m_mouseOutScript					= new CMenuScript();
		m_pInventory->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)m_pInventory;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_mouseOutScript;
	dlg.DoModal();
}

void CEditInventory::OnButtonEditDropScript()
{
	if(!m_pInventory->m_dropScript)
	{
		m_pInventory->m_dropScript					= new CMenuScript();
		m_pInventory->m_dropScript->m_callingVisual	= (CMenuVisual*)m_pInventory;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_dropScript;
	dlg.DoModal();
}

void CEditInventory::OnButtonEditUpdateScript()
{
	if(!m_pInventory->m_updateScript)
	{
		m_pInventory->m_updateScript					= new CMenuScript();
		m_pInventory->m_updateScript->m_callingVisual	= (CMenuVisual*)m_pInventory;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_updateScript;
	dlg.DoModal();
}

void CEditInventory::OnBnClickedButtonEditPrototypeClickScript()
{
	if(!m_pInventory->m_pPrototypeImage->m_clickScript)
	{
		m_pInventory->m_pPrototypeImage->m_clickScript = new CMenuScript();
		m_pInventory->m_pPrototypeImage->m_clickScript->m_callingVisual = (CMenuVisual*)m_pInventory->m_pPrototypeImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_pPrototypeImage->m_clickScript;
	dlg.DoModal();
}

void CEditInventory::OnBnClickedButtonEditPrototypeMouseOverScript()
{
	if(!m_pInventory->m_pPrototypeImage->m_mouseOverScript)
	{
		m_pInventory->m_pPrototypeImage->m_mouseOverScript = new CMenuScript();
		m_pInventory->m_pPrototypeImage->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)m_pInventory->m_pPrototypeImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_pPrototypeImage->m_mouseOverScript;
	dlg.DoModal();
}

void CEditInventory::OnBnClickedButtonEditPrototypeMouseOutScript()
{
	if(!m_pInventory->m_pPrototypeImage->m_mouseOutScript)
	{
		m_pInventory->m_pPrototypeImage->m_mouseOutScript = new CMenuScript();
		m_pInventory->m_pPrototypeImage->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)m_pInventory->m_pPrototypeImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_pPrototypeImage->m_mouseOutScript;
	dlg.DoModal();
}

void CEditInventory::OnBnClickedButtonEditPrototypeDropScript()
{
	if(!m_pInventory->m_pPrototypeImage->m_dropScript)
	{
		m_pInventory->m_pPrototypeImage->m_dropScript = new CMenuScript();
		m_pInventory->m_pPrototypeImage->m_dropScript->m_callingVisual	= (CMenuVisual*)m_pInventory->m_pPrototypeImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_pPrototypeImage->m_dropScript;
	dlg.DoModal();
}

void CEditInventory::OnBnClickedButtonEditPrototypeUpdateScript()
{
	if(!m_pInventory->m_pPrototypeImage->m_updateScript)
	{
		m_pInventory->m_pPrototypeImage->m_updateScript = new CMenuScript();
		m_pInventory->m_pPrototypeImage->m_updateScript->m_callingVisual	= (CMenuVisual*)m_pInventory->m_pPrototypeImage;
	}

	CEditScript dlg;
	dlg.m_script = m_pInventory->m_pPrototypeImage->m_updateScript;
	dlg.DoModal();
}

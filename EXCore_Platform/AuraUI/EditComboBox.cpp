// C:\Projects\EXCore_Platform\AuraUI\EditComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditComboBox.h"
#include "EditScript.h"

// CEditComboBox dialog
IMPLEMENT_DYNAMIC(CEditComboBox, CDialog)

CEditComboBox::CEditComboBox(CWnd* pParent /*=NULL*/)
	: CDialog(CEditComboBox::IDD, pParent)
{
	m_pComboBox			= NULL;
	m_freeVisualID		= -1;
	m_textBoxDlg		= NULL;
	m_buttonDlg			= NULL;
	m_listBoxDlg		= NULL;
	m_initialized		= false;
	m_freezeInput		= NULL;
}

BEGIN_MESSAGE_MAP(CEditComboBox, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Properties
	ON_CBN_SELCHANGE(IDC_COMBO_Font, OnChangeFont)
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)

	//Visual Component Editing
	ON_BN_CLICKED(IDC_BUTTON_EditTextBox, OnButtonEditTextBox)
	ON_BN_CLICKED(IDC_BUTTON_EditButton, OnButtonEditButton)
	ON_BN_CLICKED(IDC_BUTTON_EditListBox, OnButtonEditListBox)

	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditSelectionChangeScript, OnButtonEditSelectionChangeScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOverScript, OnButtonEditMouseOverScript)
	ON_BN_CLICKED(IDC_BUTTON_EditMouseOutScript, OnButtonEditMouseOutScript)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditComboBox::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//Fill the font list
	for(POSITION posCurFont = m_gStatus->m_menuFontList->GetHeadPosition(); posCurFont != NULL;)
	{
		CMenuFont *curFont = (CMenuFont*)m_gStatus->m_menuFontList->GetNext(posCurFont);
		SendDlgItemMessage(IDC_COMBO_Font, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)curFont->m_name);
	}

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pComboBox->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pComboBox->m_visualID);

	//Combo Box Coordinates
	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pComboBox->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pComboBox->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, m_pComboBox->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, m_pComboBox->m_height);*/

	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)m_pComboBox->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)m_pComboBox->m_posY);
	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)m_pComboBox->m_width);
	SetDlgItemInt(IDC_EDIT_VisualHeight, (UINT)m_pComboBox->m_height);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Set Current Font
	SendDlgItemMessage(IDC_COMBO_Font, CB_SETCURSEL, m_gStatus->m_menuFontList->GetIndexFromPointer(m_pComboBox->m_textBox.m_font));

	//This must be done because GetIndexFromPointer defaultly returns 0 to select the first font in the list.
	// This means m_font could still be NULL, so we must make sure it's set
	if(!m_gStatus->m_menuFontList->IsEmpty())
	{
		m_pComboBox->m_textBox.m_font = m_gStatus->m_menuFontList->GetPointerFromIndex(m_gStatus->m_menuFontList->GetIndexFromPointer(m_pComboBox->m_textBox.m_font));
		m_pComboBox->m_listBox.m_font = m_pComboBox->m_textBox.m_font;
	}

	m_initialized = true;

	return TRUE;
}

void CEditComboBox::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditComboBox::OnChangeFont()
{
	if(!m_initialized)
		return;

	//Store the new fonts information
	m_pComboBox->m_textBox.m_font		= m_gStatus->m_menuFontList->GetPointerFromIndex(SendDlgItemMessage(IDC_COMBO_Font, CB_GETCURSEL));
	ASSERT(m_pComboBox->m_textBox.m_font);
	if(m_pComboBox->m_textBox.m_font)
	{
	   m_pComboBox->m_listBox.m_font		= m_pComboBox->m_textBox.m_font;
	   m_pComboBox->m_textBox.m_fontName	= m_pComboBox->m_textBox.m_font->m_name;
	}

	//Loop through the item list and change the font
	CMenuListItem *curItem = m_pComboBox->m_listBox.m_pFirstItem;
	while(curItem)
	{
		//Loop through this item's text objects and set the font for each
		POSITION posText = curItem->m_textList.GetHeadPosition();
		while(posText != NULL)
		{
			CMenuText *curText = (CMenuText*)curItem->m_textList.GetNext(posText);
			curText->m_font = m_pComboBox->m_listBox.m_font;
		}

		//Goto the next item in the list
		curItem = curItem->m_pNext;
	}
}

void CEditComboBox::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pComboBox->m_posX		= (int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pComboBox->m_posY		= (int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pComboBox->m_width	= (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pComboBox->m_height	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/

	m_pComboBox->m_posX		= (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pComboBox->m_posY		= (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pComboBox->m_width	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pComboBox->m_height	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data

	m_pComboBox->Resize();
}

void CEditComboBox::OnButtonEditTextBox()
{
	//If the Scroll Bar Editing Dialog exists, delete it
	if(m_textBoxDlg)
		delete m_textBoxDlg;

	//Create a new dialog
	m_textBoxDlg					= new CEditTextBox();
	m_textBoxDlg->m_pTextBox		= &m_pComboBox->m_textBox;
	m_textBoxDlg->m_gStatus			= m_gStatus;
	m_textBoxDlg->m_freeVisualID	= 0;

	//Run it
	m_textBoxDlg->Create(IDD_DIALOG_MenuSysEditTextBox);

	//Disable the Visual Name/ID text boxes, Font combo, and the MouseOver/MouseOut script buttons
	CWnd *curItem = m_textBoxDlg->GetDlgItem(IDC_EDIT_VisualName);
	curItem->EnableWindow(false);
	curItem = m_textBoxDlg->GetDlgItem(IDC_BUTTON_getFreeID);
	curItem->EnableWindow(false);
	curItem = m_textBoxDlg->GetDlgItem(IDC_EDIT_visualID);
	curItem->EnableWindow(false);
	curItem = m_textBoxDlg->GetDlgItem(IDC_COMBO_Font);
	curItem->EnableWindow(false);
	curItem = m_textBoxDlg->GetDlgItem(IDC_BUTTON_EditMouseOverScript);
	curItem->EnableWindow(false);
	curItem = m_textBoxDlg->GetDlgItem(IDC_BUTTON_EditMouseOutScript);
	curItem->EnableWindow(false);
}

void CEditComboBox::OnButtonEditButton()
{
	//If the Scroll Bar Editing Dialog exists, delete it
	if(m_buttonDlg)
		delete m_buttonDlg;

	//Create a new dialog
	m_buttonDlg					= new CEditButton();
	m_buttonDlg->m_pButton		= &m_pComboBox->m_button;
	m_buttonDlg->m_gStatus		= m_gStatus;
	m_buttonDlg->m_freeVisualID	= 0;
	m_buttonDlg->m_freezeInput	= m_freezeInput;

	//Run it
	m_buttonDlg->Create(IDD_DIALOG_MenuSysEditButton);

	//Disable visual settings
	CWnd *curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_VisualName);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_getFreeID);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_visualID);
	curItem->EnableWindow(false);

	//Disable Up state's Left components
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_UpStateLeftTextureX);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_UpStateLeftTextureY);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_UpStateLeftTextureWidth);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditUpStateLeftTexture);
	curItem->EnableWindow(false);

	//Disable Up state's Right components
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_UpStateRightTextureX);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_UpStateRightTextureY);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_UpStateRightTextureWidth);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditUpStateRightTexture);
	curItem->EnableWindow(false);

	//Disable Hover state's Left components
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_HoverStateLeftTextureX);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_HoverStateLeftTextureY);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_HoverStateLeftTextureWidth);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditHoverStateLeftTexture);
	curItem->EnableWindow(false);

	//Disable Hover state's Right components
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_HoverStateRightTextureX);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_HoverStateRightTextureY);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_HoverStateRightTextureWidth);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditHoverStateRightTexture);
	curItem->EnableWindow(false);

	//Disable Down state's Left components
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DownStateLeftTextureX);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DownStateLeftTextureY);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DownStateLeftTextureWidth);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditDownStateLeftTexture);
	curItem->EnableWindow(false);

	//Disable Down state's Right components
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DownStateRightTextureX);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DownStateRightTextureY);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DownStateRightTextureWidth);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditDownStateRightTexture);
	curItem->EnableWindow(false);

	//Disable Disabled state's Left components
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DisabledStateLeftTextureX);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DisabledStateLeftTextureY);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DisabledStateLeftTextureWidth);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditDisabledStateLeftTexture);
	curItem->EnableWindow(false);

	//Disable Disabled state's Right components
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DisabledStateRightTextureX);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DisabledStateRightTextureY);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_EDIT_DisabledStateRightTextureWidth);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditDisabledStateRightTexture);
	curItem->EnableWindow(false);

	//Disable all scripts except for click
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditMouseOverScript);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditMouseOutScript);
	curItem->EnableWindow(false);
	curItem = m_buttonDlg->GetDlgItem(IDC_BUTTON_EditDropScript);
	curItem->EnableWindow(false);
}

void CEditComboBox::OnButtonEditListBox()
{
	//If the Scroll Bar Editing Dialog exists, delete it
	if(m_listBoxDlg)
		delete m_listBoxDlg;

	//Make sure the list box is visible
	m_pComboBox->m_listBoxShown = true;

	//Create a new dialog
	m_listBoxDlg				 = new CEditListBox();
	m_listBoxDlg->m_pListBox	 = &m_pComboBox->m_listBox;
	m_listBoxDlg->m_gStatus		 = m_gStatus;
	m_listBoxDlg->m_freeVisualID = 0;
	m_listBoxDlg->m_freezeInput	 = m_freezeInput;

	//Run it
	m_listBoxDlg->Create(IDD_DIALOG_MenuSysEditListBox);

	//Disable Visual Name/ID, Font combo, and MouseOver/MouseOut/Drop script buttons
	CWnd *curItem = m_listBoxDlg->GetDlgItem(IDC_EDIT_VisualName);
	curItem->EnableWindow(false);
	curItem = m_listBoxDlg->GetDlgItem(IDC_BUTTON_getFreeID);
	curItem->EnableWindow(false);
	curItem = m_listBoxDlg->GetDlgItem(IDC_EDIT_visualID);
	curItem->EnableWindow(false);
	curItem = m_listBoxDlg->GetDlgItem(IDC_COMBO_Font);
	curItem->EnableWindow(false);
	curItem = m_listBoxDlg->GetDlgItem(IDC_BUTTON_EditMouseOverScript);
	curItem->EnableWindow(false);
	curItem = m_listBoxDlg->GetDlgItem(IDC_BUTTON_EditMouseOutScript);
	curItem->EnableWindow(false);
	curItem = m_listBoxDlg->GetDlgItem(IDC_BUTTON_EditDropScript);
	curItem->EnableWindow(false);
}

void CEditComboBox::OnButtonEditSelectionChangeScript()
{
	if(!m_pComboBox->m_selectionChangeScript)
	{
		m_pComboBox->m_selectionChangeScript					= new CMenuScript();
		m_pComboBox->m_selectionChangeScript->m_callingVisual	= (CMenuVisual*)m_pComboBox;
	}

	CEditScript dlg;
	dlg.m_script = m_pComboBox->m_selectionChangeScript;
	dlg.DoModal();
}

void CEditComboBox::OnButtonEditMouseOverScript()
{
	if(!m_pComboBox->m_mouseOverScript)
	{
		m_pComboBox->m_mouseOverScript					= new CMenuScript();
		m_pComboBox->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)m_pComboBox;
	}

	CEditScript dlg;
	dlg.m_script = m_pComboBox->m_mouseOverScript;
	dlg.DoModal();
}

void CEditComboBox::OnButtonEditMouseOutScript()
{
	if(!m_pComboBox->m_mouseOutScript)
	{
		m_pComboBox->m_mouseOutScript					= new CMenuScript();
		m_pComboBox->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)m_pComboBox;
	}

	CEditScript dlg;
	dlg.m_script = m_pComboBox->m_mouseOutScript;
	dlg.DoModal();
}

void CEditComboBox::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pComboBox->m_name = temp;

	m_pComboBox->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);

	CDialog::OnOK();
}

// CEditComboBox message handlers

// C:\Projects\EXCore_Platform\AuraUI\EditText.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditText.h"
#include "EditScript.h"

// CEditText dialog
IMPLEMENT_DYNAMIC(CEditText, CDialog)

CEditText::CEditText(CWnd* pParent /*=NULL*/)
	: CDialog(CEditText::IDD, pParent)
{
	m_pText				= NULL;
	m_freeVisualID		= -1;
	m_initialized		= false;
}

BEGIN_MESSAGE_MAP(CEditText, CDialog)
	//Generate Button Clicked
	ON_BN_CLICKED(IDC_BUTTON_getFreeID, OnButtonGetFreeID)

	//Property Editing
	ON_EN_CHANGE(IDC_EDIT_Text, OnChangeText)
	ON_CBN_SELCHANGE(IDC_COMBO_Font, OnChangeFont)
	ON_CBN_SELCHANGE(IDC_COMBO_Alignment, OnChangeAlignment)
	ON_EN_CHANGE(IDC_EDIT_ColorRed, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorGreen, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorBlue, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_ColorAlpha, OnChangeColor)
	ON_EN_CHANGE(IDC_EDIT_VisualX, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualY, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualWidth, OnChangeVisualCoords)
	ON_EN_CHANGE(IDC_EDIT_VisualHeight, OnChangeVisualCoords)
	ON_BN_CLICKED(IDC_CHECK_Multilined, OnToggleMultilined)
	
	//Script Editing
	ON_BN_CLICKED(IDC_BUTTON_EditClickScript, OnButtonEditClickScript)
	ON_BN_CLICKED(IDC_BUTTON_EditUpdateScript, OnButtonEditUpdateScript)

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel()
	ON_BN_CLICKED(IDOK, OnButtonNext)
END_MESSAGE_MAP()

BOOL CEditText::OnInitDialog()
{
	CDialog::OnInitDialog();

	//Fill the font list
	for(POSITION posCurFont = m_gStatus->m_menuFontList->GetHeadPosition(); posCurFont != NULL;)
	{
		CMenuFont *curFont = (CMenuFont*)m_gStatus->m_menuFontList->GetNext(posCurFont);
		SendDlgItemMessage(IDC_COMBO_Font, CB_ADDSTRING, 0, (LPARAM)(LPCTSTR)curFont->m_name);
	}

	//Fill the alignment list
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Left");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Center");
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_ADDSTRING, 0, (LPARAM)"Right");

	//Identifying Data
	SetDlgItemText(IDC_EDIT_VisualName, m_pText->m_name);
	SetDlgItemInt(IDC_EDIT_visualID, m_pText->m_visualID);

	//Properties
	SetDlgItemText(IDC_EDIT_Text, m_pText->m_text);
	SendDlgItemMessage(IDC_COMBO_Font, CB_SETCURSEL, m_gStatus->m_menuFontList->GetIndexFromPointer(m_pText->m_font));
	SendDlgItemMessage(IDC_COMBO_Alignment, CB_SETCURSEL, m_pText->m_alignment);

	//SetDlgItemInt(IDC_EDIT_ColorRed, m_pText->m_red * 255.0f);
	//SetDlgItemInt(IDC_EDIT_ColorGreen, m_pText->m_green * 255.0f);
	//SetDlgItemInt(IDC_EDIT_ColorBlue, m_pText->m_blue * 255.0f);
	//SetDlgItemInt(IDC_EDIT_ColorAlpha, m_pText->m_alpha * 255.0f);
	
	SetDlgItemInt(IDC_EDIT_ColorRed, (UINT)(m_pText->m_red * 255.0f));
	SetDlgItemInt(IDC_EDIT_ColorGreen, (UINT)(m_pText->m_green * 255.0f));
	SetDlgItemInt(IDC_EDIT_ColorBlue, (UINT)(m_pText->m_blue * 255.0f));
	SetDlgItemInt(IDC_EDIT_ColorAlpha, (UINT)(m_pText->m_alpha * 255.0f));
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Coordinates
	/*SetDlgItemInt(IDC_EDIT_VisualX, m_pText->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, m_pText->m_posY);*/
	
	SetDlgItemInt(IDC_EDIT_VisualX, (UINT)m_pText->m_posX);
	SetDlgItemInt(IDC_EDIT_VisualY, (UINT)m_pText->m_posY);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//Multilined Settings
	CheckDlgButton(IDC_CHECK_Multilined, m_pText->m_multilined);

	/*SetDlgItemInt(IDC_EDIT_VisualWidth, m_pText->m_width);*/

	SetDlgItemInt(IDC_EDIT_VisualWidth, (UINT)m_pText->m_width);
	//WARNING: C4244 --> conversion from float to UINT, possible loss of data

	//This must be done because GetIndexFromPointer defaultly returns 0 to select the first font in the list.
	// This means m_font could still be NULL, so we must make sure it's set
	if(!m_gStatus->m_menuFontList->IsEmpty())
		m_pText->m_font = m_gStatus->m_menuFontList->GetPointerFromIndex(m_gStatus->m_menuFontList->GetIndexFromPointer(m_pText->m_font));

	m_initialized = true;

	return TRUE;
}

void CEditText::OnButtonGetFreeID()
{
	SetDlgItemInt(IDC_EDIT_visualID, m_freeVisualID);
}

void CEditText::OnChangeText()
{
	if(!m_initialized)
		return;

	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_Text, temp, 255);
	m_pText->SetText(temp);
}

void CEditText::OnChangeFont()
{
	if(!m_initialized)
		return;

	m_pText->m_font		= m_gStatus->m_menuFontList->GetPointerFromIndex(SendDlgItemMessage(IDC_COMBO_Font, CB_GETCURSEL));
	ASSERT(m_pText->m_font);
	if(m_pText->m_font)
	   m_pText->m_fontName	= m_pText->m_font->m_name;

	//Make it rebuild the text's mesh
	delete m_pText->m_textMesh;
	m_pText->m_textMesh = NULL;
}

void CEditText::OnChangeAlignment()
{
	if(!m_initialized)
		return;

	m_pText->m_alignment = SendDlgItemMessage(IDC_COMBO_Alignment, CB_GETCURSEL);
}

void CEditText::OnChangeColor()
{
	if(!m_initialized)
		return;

	m_pText->m_red = (int)GetDlgItemInt(IDC_EDIT_ColorRed) / 255.0f;
	m_pText->m_green = (int)GetDlgItemInt(IDC_EDIT_ColorGreen) / 255.0f;
	m_pText->m_blue = (int)GetDlgItemInt(IDC_EDIT_ColorBlue) / 255.0f;
	m_pText->m_alpha = (int)GetDlgItemInt(IDC_EDIT_ColorAlpha) / 255.0f;

	//Make it rebuild the text's mesh
	delete m_pText->m_textMesh;
	m_pText->m_textMesh = NULL;
}

void CEditText::OnChangeVisualCoords()
{
	if(!m_initialized)
		return;

	//Update object coordinates
	/*m_pText->m_posX		= (int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pText->m_posY		= (int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pText->m_width	= (int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pText->m_height	= (int)GetDlgItemInt(IDC_EDIT_VisualHeight);*/


	m_pText->m_posX		= (float)(int)GetDlgItemInt(IDC_EDIT_VisualX);
	m_pText->m_posY		= (float)(int)GetDlgItemInt(IDC_EDIT_VisualY);
	m_pText->m_width	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualWidth);
	m_pText->m_height	= (float)(int)GetDlgItemInt(IDC_EDIT_VisualHeight);
	//WARNING: C4244 --> conversion from int to float, possible loss of data
}

void CEditText::OnToggleMultilined()
{
	m_pText->m_multilined = IsDlgButtonChecked(IDC_CHECK_Multilined);

	//Enable the width setting if it's a multilined text
	CWnd *widthBox = GetDlgItem(IDC_EDIT_VisualWidth);
	if(m_pText->m_multilined)
		widthBox->EnableWindow(true);
	else
		widthBox->EnableWindow(false);
}

void CEditText::OnButtonEditClickScript()
{
}

void CEditText::OnButtonEditUpdateScript()
{
	if(!m_pText->m_updateScript)
	{
		m_pText->m_updateScript					= new CMenuScript();
		m_pText->m_updateScript->m_callingVisual = (CMenuVisual*)m_pText;
	}

	CEditScript dlg;
	dlg.m_script = m_pText->m_updateScript;
	dlg.DoModal();
}

void CEditText::OnButtonNext()
{
	//We use a temp because CString.GetBuffer() doesn't update string lengths and such
	char temp[255];
	GetDlgItemText(IDC_EDIT_VisualName, temp, 255);
	m_pText->m_name = temp;

	m_pText->m_visualID = (int)GetDlgItemInt(IDC_EDIT_visualID);

	CDialog::OnOK();
}

// CEditText message handlers

#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"
#include "EditScrollBar.h"

// CEditTextBox dialog
class CEditTextBox : public CDialog
{
	DECLARE_DYNAMIC(CEditTextBox)

public:
	CEditTextBox(CWnd* pParent = NULL);   // standard constructor

	CMenuTextBox			*m_pTextBox;
	GlobalStatusVariables	*m_gStatus;
	CEditScrollBar			*m_scrollBarDlg;
	int						 m_freeVisualID;
	bool					 m_initialized;
	bool					*m_freezeInput;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditTextBox };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Utility Functions
	void EditTexture(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, bool CopyWidth = false);

	//Generate Button
	afx_msg void OnButtonGetFreeID();

	//Properties
	afx_msg void OnChangeText();
	afx_msg void OnChangeFont();
	afx_msg void OnChangeAlignment();
	afx_msg void OnChangeColor();
	afx_msg void OnToggleMultilined();
	afx_msg void OnToggleEditable();
	afx_msg void OnToggleNumericOnly();
	afx_msg void OnTogglePassword();
	afx_msg void OnChangeMaxLines();
	afx_msg void OnChangeMaxLength();
	afx_msg void OnButtonEditScrollBar();

	//Visual Coordinates
	afx_msg void OnChangeVisualCoords();

	//Background Textures
	afx_msg void OnChangeBackgroundLeftTextureCoords();
	afx_msg void OnButtonEditBackgroundLeftTexture();
	afx_msg void OnChangeBackgroundMiddleTextureCoords();
	afx_msg void OnButtonEditBackgroundMiddleTexture();
	afx_msg void OnChangeBackgroundRightTextureCoords();
	afx_msg void OnButtonEditBackgroundRightTexture();

	//Cursor Texture
	afx_msg void OnChangeCursorCoords();
	afx_msg void OnButtonEditCursorTexture();

	//Text Inset
	afx_msg void OnChangeTextInset();

	//System Readout Settings
	afx_msg void OnToggleSystemReadout();
	afx_msg void OnChangeClanColor();
	afx_msg void OnChangeGroupColor();
	afx_msg void OnChangeServerColor();
	afx_msg void OnChangeDamageColor();
	afx_msg void OnChangeHealColor();
	afx_msg void OnChangeMusicColor();

	//Scripts
	afx_msg void OnButtonEditEnterPressScript();
	afx_msg void OnButtonEditMouseOverScript();
	afx_msg void OnButtonEditMouseOutScript();

	//Regular Buttons
	afx_msg void OnButtonNext();
};

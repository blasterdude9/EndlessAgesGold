#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"
#include "EditMenuTitle.h"

// CAddMenu dialog
class CAddMenu : public CDialog
{
// Construction
public:
	CAddMenu(CWnd* pParent = NULL);   // standard constructor
    
	CMenuWindow				*m_pMenu;
	GlobalStatusVariables	*m_gStatus;
	int						 m_freeVisualID;
	bool					 m_initialized;
	CEditMenuTitle			*m_editTitleDlg;

	enum { IDD = IDD_DIALOG_MenuSysAddMenu };

// Implementation
protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	//Utility Functions
	void InitializeAlignmentComboBox();
	void CreateBordersAndCorners();
	void EditTexture(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, float Height);
	void UpdateImageTextureCoords(CMenuImage *pImage);

	//Generate Button Clicked
	afx_msg void OnButtonGetFreeID();

	//Properties
	afx_msg void OnChangeAlignment();
	afx_msg void OnToggleDraggable();
	afx_msg void OnToggleResizable();
	afx_msg void OnToggleEscapable();
	afx_msg void OnToggleShowMouseCursor();

	//Menu Coord Edit
	afx_msg void OnChangeMenuCoords();

	//Background Texture Edit
	afx_msg void OnChangeBackgroundTextureCoords();
	afx_msg void OnButtonEditBackgroundTexture();
	afx_msg void OnChangeBackgroundInset();
	
	//Top Border Texture Edit
	afx_msg void OnChangeTopBorderTextureCoords();
	afx_msg void OnButtonEditTopBorderTexture();

	//Left Border Texture Edit
	afx_msg void OnChangeLeftBorderTextureCoords();
	afx_msg void OnButtonEditLeftBorderTexture();

	//Bottom Border Texture Edit
	afx_msg void OnChangeBottomBorderTextureCoords();
	afx_msg void OnButtonEditBottomBorderTexture();

	//Right Border Texture Edit
	afx_msg void OnChangeRightBorderTextureCoords();
	afx_msg void OnButtonEditRightBorderTexture();

	//Top Left Corner Texture Edit
	afx_msg void OnChangeTopLeftCornerTextureCoords();
	afx_msg void OnButtonEditTopLeftCornerTexture();

	//Top Right Corner Texture Edit
	afx_msg void OnChangeTopRightCornerTextureCoords();
	afx_msg void OnButtonEditTopRightCornerTexture();

	//Bottom Left Corner Texture Edit
	afx_msg void OnChangeBottomLeftCornerTextureCoords();
	afx_msg void OnButtonEditBottomLeftCornerTexture();

	//Bottom Right Corner Texture Edit
	afx_msg void OnChangeBottomRightCornerTextureCoords();
	afx_msg void OnButtonEditBottomRightCornerTexture();

	//Corner Offset Edit
	afx_msg void OnChangeMenuCornerOffsetCoords();

	//Script Editing
	afx_msg void OnButtonEditLoadScript();
	afx_msg void OnButtonEditResizeScript();
	afx_msg void OnButtonEditCloseScript();

	//Edit Title
	afx_msg void OnButtonEditTitle();

	//Regular Buttons
	//IDCANCEL is handled by CDialog::OnCancel();
	afx_msg void OnButtonNext();
};

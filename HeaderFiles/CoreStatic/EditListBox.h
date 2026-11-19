#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"
#include "EditScrollBar.h"

// CEditListBox dialog
class CEditListBox : public CDialog
{
	DECLARE_DYNAMIC(CEditListBox)

public:
	CEditListBox(CWnd* pParent = NULL);   // standard constructor

	CMenuListBox			*m_pListBox;
	GlobalStatusVariables	*m_gStatus;
	CEditScrollBar			*m_scrollBarDlg;
	int						 m_freeVisualID;
	bool					 m_initialized;
	bool					*m_freezeInput;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditListBox };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Utility Functions
	void CreateBordersAndCorners();
	void EditTexture(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, float Height);

	//Generate Button
	afx_msg void OnButtonGetFreeID();

	//Properties
	afx_msg void OnChangeFont();
	afx_msg void OnChangeColor();
	afx_msg void OnChangeSelectedColor();
	afx_msg void OnChangeSpecialColor();
	afx_msg void OnChangeTextInset();
	afx_msg void OnChangeColumnSpacing();
	afx_msg void OnChangeNumberOfColumns();
	afx_msg void OnChangeNumTextObjectsPerItem();
	afx_msg void OnChangeScrollBarInset();
	afx_msg void OnChangeExtraSpacingBetweenText();
	afx_msg void OnChangeExtraSpacingBetweenItems();
	afx_msg void OnToggleAlwaysShowScrollBar();
	afx_msg void OnToggleDraggableItems();
	afx_msg void OnButtonEditScrollBar();

	//Visual Coordinates
	afx_msg void OnChangeVisualCoords();

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

	//Item Background Texture Edit
	afx_msg void OnChangeItemBackgroundTextureCoords();
	afx_msg void OnButtonEditItemBackgroundTexture();

	//Corner Offsets
	afx_msg void OnChangeCornerOffsets();

	//Scripts
	afx_msg void OnButtonEditSelectionChangeScript();
	afx_msg void OnButtonEditMouseOverScript();
	afx_msg void OnButtonEditMouseOutScript();
	afx_msg void OnButtonEditDropScript();

	//Regular Buttons
	afx_msg void OnButtonNext();
public:
	afx_msg void OnBnClickedButtonEditupdatescript();
};

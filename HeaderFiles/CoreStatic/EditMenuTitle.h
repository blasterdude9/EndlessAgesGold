#pragma once

#include "resource.h"
#include "CMenuWindow.h"
#include "MaterialDlg.h"
#include "CTextureDBClass.h"

// CEditMenuTitle dialog
class CEditMenuTitle : public CDialog
{
	DECLARE_DYNAMIC(CEditMenuTitle)

public:
	CEditMenuTitle(CWnd* pParent = NULL);   // standard constructor

	CMenuWindow			*m_pMenu;
	CTextureDBObjList	*m_pTextureDatabase;
	CMenuFontList		*m_pFontList;
	bool				 m_initialized;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditMenuTitle };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Utility Functions
	void EditTexture(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, bool CopyWidth = false);
	void Resize();

	//Text Properties
	afx_msg void OnChangeText();
	afx_msg void OnChangeFont();
	afx_msg void OnChangeColor();

	//Title Coordinates
	afx_msg void OnChangeVisualCoords();

	//Text Coordinates
	afx_msg void OnChangeTextCoords();

	//Background Textures
	afx_msg void OnChangeBackgroundLeftTextureCoords();
	afx_msg void OnButtonEditBackgroundLeftTexture();
	afx_msg void OnChangeBackgroundMiddleTextureCoords();
	afx_msg void OnButtonEditBackgroundMiddleTexture();
	afx_msg void OnChangeBackgroundRightTextureCoords();
	afx_msg void OnButtonEditBackgroundRightTexture();

	//Check Boxes
	afx_msg void OnToggleStretchTitle();
	afx_msg void OnToggleDragByTitleOnly();

	//Regular Buttons
	afx_msg void OnButtonNext();
};

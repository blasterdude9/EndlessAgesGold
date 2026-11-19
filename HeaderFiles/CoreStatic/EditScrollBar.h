#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"

// CEditScrollBar dialog
class CEditScrollBar : public CDialog
{
	DECLARE_DYNAMIC(CEditScrollBar)

public:
	CEditScrollBar(CWnd* pParent = NULL);   // standard constructor

	CMenuScrollBar			*m_pScrollBar;
	GlobalStatusVariables	*m_gStatus;
	int						 m_freeVisualID;
	bool					 m_initialized;
	bool					*m_freezeInput;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditScrollBar };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Utility Functions
	void EditTexture(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage, float X, float Y, float Width, float Height);

	//Generate Button
	afx_msg void OnButtonGetFreeID();

	//Visual Coordinates
	afx_msg void OnChangeVisualCoords();

	//Properties
	afx_msg void OnToggleVertical();

	//Background Textures
	afx_msg void OnChangeBackgroundLeftTextureCoords();
	afx_msg void OnButtonEditBackgroundLeftTexture();
	afx_msg void OnChangeBackgroundMiddleTextureCoords();
	afx_msg void OnButtonEditBackgroundMiddleTexture();
	afx_msg void OnChangeBackgroundRightTextureCoords();
	afx_msg void OnButtonEditBackgroundRightTexture();

	//Slider Textures
	afx_msg void OnChangeSliderLeftTextureCoords();
	afx_msg void OnButtonEditSliderLeftTexture();
	afx_msg void OnChangeSliderMiddleTextureCoords();
	afx_msg void OnButtonEditSliderMiddleTexture();
	afx_msg void OnChangeSliderRightTextureCoords();
	afx_msg void OnButtonEditSliderRightTexture();

	//Left Button Textures
	afx_msg void OnChangeLeftUpStateTextureCoords();
	afx_msg void OnButtonEditLeftUpStateTexture();
	afx_msg void OnChangeLeftHoverStateTextureCoords();
	afx_msg void OnButtonEditLeftHoverStateTexture();
	afx_msg void OnChangeLeftDownStateTextureCoords();
	afx_msg void OnButtonEditLeftDownStateTexture();
	afx_msg void OnChangeLeftDisabledStateTextureCoords();
	afx_msg void OnButtonEditLeftDisabledStateTexture();

	//Right Button Textures
	afx_msg void OnChangeRightUpStateTextureCoords();
	afx_msg void OnButtonEditRightUpStateTexture();
	afx_msg void OnChangeRightHoverStateTextureCoords();
	afx_msg void OnButtonEditRightHoverStateTexture();
	afx_msg void OnChangeRightDownStateTextureCoords();
	afx_msg void OnButtonEditRightDownStateTexture();
	afx_msg void OnChangeRightDisabledStateTextureCoords();
	afx_msg void OnButtonEditRightDisabledStateTexture();

	//TODO: Remove These
	afx_msg void OnChangeNumItems();
	afx_msg void OnChangeNumVisibleItems();

	//Scripts
	afx_msg void OnButtonEditChangeScript();
	afx_msg void OnButtonEditMouseOverScript();
	afx_msg void OnButtonEditMouseOutScript();

	//Regular Buttons
	afx_msg void OnButtonNext();
};

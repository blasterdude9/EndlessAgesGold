#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"

// CEditButton dialog
class CEditButton : public CDialog
{
	DECLARE_DYNAMIC(CEditButton)

public:
	CEditButton(CWnd* pParent = NULL);   // standard constructor

	CMenuButton				*m_pButton;
	GlobalStatusVariables	*m_gStatus;
	int						 m_freeVisualID;
	bool					 m_initialized;
	bool					*m_freezeInput;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditButton };

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

	//Visual Coordinates
	afx_msg void OnChangeVisualCoords();

	// Properties
	afx_msg void OnChangeButtonText();

	//Up State Textures
	afx_msg void OnChangeUpStateLeftTextureCoords();
	afx_msg void OnButtonEditUpStateLeftTexture();
	afx_msg void OnChangeUpStateMiddleTextureCoords();
	afx_msg void OnButtonEditUpStateMiddleTexture();
	afx_msg void OnChangeUpStateRightTextureCoords();
	afx_msg void OnButtonEditUpStateRightTexture();

	//Hover State Textures
	afx_msg void OnChangeHoverStateLeftTextureCoords();
	afx_msg void OnButtonEditHoverStateLeftTexture();
	afx_msg void OnChangeHoverStateMiddleTextureCoords();
	afx_msg void OnButtonEditHoverStateMiddleTexture();
	afx_msg void OnChangeHoverStateRightTextureCoords();
	afx_msg void OnButtonEditHoverStateRightTexture();

	//Down State Textures
	afx_msg void OnChangeDownStateLeftTextureCoords();
	afx_msg void OnButtonEditDownStateLeftTexture();
	afx_msg void OnChangeDownStateMiddleTextureCoords();
	afx_msg void OnButtonEditDownStateMiddleTexture();
	afx_msg void OnChangeDownStateRightTextureCoords();
	afx_msg void OnButtonEditDownStateRightTexture();

	//Disabled State Textures
	afx_msg void OnChangeDisabledStateLeftTextureCoords();
	afx_msg void OnButtonEditDisabledStateLeftTexture();
	afx_msg void OnChangeDisabledStateMiddleTextureCoords();
	afx_msg void OnButtonEditDisabledStateMiddleTexture();
	afx_msg void OnChangeDisabledStateRightTextureCoords();
	afx_msg void OnButtonEditDisabledStateRightTexture();

	//Scripts
	afx_msg void OnButtonEditClickScript();
	afx_msg void OnButtonEditMouseOverScript();
	afx_msg void OnButtonEditMouseOutScript();
	afx_msg void OnButtonEditDropScript();

	//Regular Buttons
	afx_msg void OnButtonNext();
};

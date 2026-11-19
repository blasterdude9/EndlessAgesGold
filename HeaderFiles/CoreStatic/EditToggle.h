#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"

// CEditToggle dialog
class CEditToggle : public CDialog
{
	DECLARE_DYNAMIC(CEditToggle)

public:
	CEditToggle(CWnd* pParent = NULL);   // standard constructor

	CMenuToggle				*m_pToggle;
	GlobalStatusVariables	*m_gStatus;
	int						 m_freeVisualID;
	bool					 m_initialized;
	bool					*m_freezeInput;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditToggle };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Utility Functions
	void EditTexture(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage);

	//Control Properties
	afx_msg void OnChangeGroupID();
	afx_msg void OnChangeToggleValue();
	afx_msg void OnChangeToggleText();

	//Generate Button
	afx_msg void OnButtonGetFreeID();

	//Visual Coordinates
	afx_msg void OnChangeVisualCoords();

	//State Textures
	afx_msg void OnChangeUpStateTextureCoords();
	afx_msg void OnButtonEditUpStateTexture();
	afx_msg void OnChangeUpHoverStateTextureCoords();
	afx_msg void OnButtonEditUpHoverStateTexture();
	afx_msg void OnChangeDownStateTextureCoords();
	afx_msg void OnButtonEditDownStateTexture();
	afx_msg void OnChangeDownHoverStateTextureCoords();
	afx_msg void OnButtonEditDownHoverStateTexture();

	//Scripts
	afx_msg void OnButtonEditClickScript();
	afx_msg void OnButtonEditMouseOverScript();
	afx_msg void OnButtonEditMouseOutScript();

	//Regular Buttons
	afx_msg void OnButtonNext();
};

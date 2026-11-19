#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"

// CEditProgBar dialog
class CEditProgBar : public CDialog
{
	DECLARE_DYNAMIC(CEditProgBar)

public:
	CEditProgBar(CWnd* pParent = NULL);   // standard constructor

	CMenuProgressBar		*m_pProgressBar;
	GlobalStatusVariables	*m_gStatus;
	int						 m_freeVisualID;
	bool					 m_initialized;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditProgBar };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Utility Functions
	void EditTexture(CMenuImage *pImage);
	void UpdateImageTextureCoords(CMenuImage *pImage);

	//Generate Button
	afx_msg void OnButtonGetFreeID();

	//Visual Coordinates
	afx_msg void OnChangeVisualCoords();

	//Properties
	afx_msg void OnToggleVertical();
	afx_msg void OnToggleFlipped();

	//Background Textures
	afx_msg void OnChangeBackgroundLeftTextureCoords();
	afx_msg void OnButtonEditBackgroundLeftTexture();
	afx_msg void OnChangeBackgroundMiddleTextureCoords();
	afx_msg void OnButtonEditBackgroundMiddleTexture();
	afx_msg void OnChangeBackgroundRightTextureCoords();
	afx_msg void OnButtonEditBackgroundRightTexture();

	//Foreground Textures
	afx_msg void OnChangeForegroundLeftTextureCoords();
	afx_msg void OnButtonEditForegroundLeftTexture();
	afx_msg void OnChangeForegroundMiddleTextureCoords();
	afx_msg void OnButtonEditForegroundMiddleTexture();
	afx_msg void OnChangeForegroundRightTextureCoords();
	afx_msg void OnButtonEditForegroundRightTexture();
	afx_msg void OnChangeForegroundInset();

	//Scripts
	afx_msg void OnButtonEditMouseOverScript();
	afx_msg void OnButtonEditMouseOutScript();
	afx_msg void OnButtonEditUpdateScript();

	//Regular Buttons
	afx_msg void OnButtonNext();
};

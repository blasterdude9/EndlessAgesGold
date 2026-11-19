#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"

// CEditImage dialog
class CEditImage : public CDialog
{
	DECLARE_DYNAMIC(CEditImage)

public:
	CEditImage(CWnd* pParent = NULL);   // standard constructor

	CMenuImage				*m_pImage;
	GlobalStatusVariables	*m_gStatus;
	int						 m_freeVisualID;
	bool					 m_initialized;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditImage };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Utility Functions
	void UpdateTextureCoords();

	//Input Callback Functions
	afx_msg void OnButtonGetFreeID();
	afx_msg void OnChangeVisualCoords();
	afx_msg void OnChangeTextureCoords();
	afx_msg void OnButtonEditTexture();

	//Scripts
	afx_msg void OnButtonEditClickScript();
	afx_msg void OnButtonEditMouseOverScript();
	afx_msg void OnButtonEditMouseOutScript();
	afx_msg void OnButtonEditDropScript();
	afx_msg void OnButtonEditUpdateScript();

	//Regular Buttons
	afx_msg void OnButtonNext();
};

#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"

// CEditFont dialog
class CEditFont : public CDialog
{
	DECLARE_DYNAMIC(CEditFont)

public:
	CEditFont(CWnd* pParent = NULL);   // standard constructor

	GlobalStatusVariables	*m_gStatus;
	CMenuFont				*m_pCurFont;
	CMenuImage				*m_pFontImage;
	bool					 m_initialized;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysFonts };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Utility Functions
	void RefreshFontList(int selection);
	void FillCharacterList();

	//Input Callback Functions
	afx_msg void OnSelectionChangeListFonts();
	afx_msg void OnButtonAddFont();
	afx_msg void OnButtonReplaceFont();
	afx_msg void OnChangeFontSettings();
	afx_msg void OnButtonEditFontTexture();
	afx_msg void OnSelectionChangeListCharacters();
	afx_msg void OnChangeCharacterSettings();
	afx_msg void OnButtonNext();
};

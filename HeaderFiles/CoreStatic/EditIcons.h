#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"
#include "EditImage.h"

// CEditIcons dialog

class CEditIcons : public CDialog
{
	DECLARE_DYNAMIC(CEditIcons)

public:
	CEditIcons(CWnd* pParent = NULL);   // standard constructor

	CMenuVisualList			*m_pIconList;
	CMenuImage				*m_pIconImage;
	CMenuWindow				*m_pIconWindow;
	GlobalStatusVariables	*m_gStatus;
	CEditImage				*m_editImageDlg;
	bool					 m_initialized;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysIcons };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP();

public:
	//Utility Functions
	void RefreshIconList(int selection);

	//Input Callback Functions
	afx_msg void OnSelectionChangeListIcons();
	afx_msg void OnButtonNewIcon();
	afx_msg void OnButtonEditIcon();
	afx_msg void OnButtonDeleteIcon();
	afx_msg void OnButtonSelectIconSheetTexture();
	afx_msg void OnChangeIconSheetProperties();
	afx_msg void OnButtonImportIconSheet();
	afx_msg void OnButtonNext();
};

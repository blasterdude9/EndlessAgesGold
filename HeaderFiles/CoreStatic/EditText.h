#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"

// CEditText dialog
class CEditText : public CDialog
{
	DECLARE_DYNAMIC(CEditText)

public:
	CEditText(CWnd* pParent = NULL);   // standard constructor

	CMenuText				*m_pText;
	GlobalStatusVariables	*m_gStatus;
	int						 m_freeVisualID;
	bool					 m_initialized;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditText };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Input Callback Functions
	afx_msg void OnButtonGetFreeID();
	afx_msg void OnChangeText();
	afx_msg void OnChangeFont();
	afx_msg void OnChangeAlignment();
	afx_msg void OnChangeColor();
	afx_msg void OnChangeVisualCoords();
	afx_msg void OnToggleMultilined();
	afx_msg void OnButtonEditClickScript();
	afx_msg void OnButtonEditUpdateScript();
	afx_msg void OnButtonNext();
};

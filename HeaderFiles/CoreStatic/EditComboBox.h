#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"
#include "EditTextBox.h"
#include "EditButton.h"
#include "EditListBox.h"

// CEditComboBox dialog
class CEditComboBox : public CDialog
{
	DECLARE_DYNAMIC(CEditComboBox)

public:
	CEditComboBox(CWnd* pParent = NULL);   // standard constructor

	CMenuComboBox			*m_pComboBox;
	GlobalStatusVariables	*m_gStatus;
	int						 m_freeVisualID;
	CEditTextBox			*m_textBoxDlg;
	CEditButton				*m_buttonDlg;
	CEditListBox			*m_listBoxDlg;
	bool					 m_initialized;
	bool					*m_freezeInput;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditComboBox };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Generate Button
	afx_msg void OnButtonGetFreeID();

	//Edit Visual
	afx_msg void OnChangeVisualCoords();
	afx_msg void OnChangeFont();
	afx_msg void OnButtonEditTextBox();
	afx_msg void OnButtonEditButton();
	afx_msg void OnButtonEditListBox();

	//Scripts
	afx_msg void OnButtonEditSelectionChangeScript();
	afx_msg void OnButtonEditMouseOverScript();
	afx_msg void OnButtonEditMouseOutScript();

	//Regular Buttons
	afx_msg void OnButtonNext();
};

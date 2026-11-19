#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"

// CEditCursors dialog
class CEditCursors : public CDialog
{
	DECLARE_DYNAMIC(CEditCursors)

public:
	CEditCursors(CWnd* pParent = NULL);   // standard constructor

	GlobalStatusVariables	*m_gStatus;
	CMenuImage				*m_curCursor;
	CMenuImage				*m_pCursorImage;
	bool					 m_initialized;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysCursors };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Utility Functions
	void UpdateTextureCoords();
	void FillCursorList();

	//Input Callback Functions
	afx_msg void OnSelectionChangeListCursors();
	afx_msg void OnChangeTextureCoordinates();
	afx_msg void OnButtonEditCursorTexture();
	afx_msg void OnButtonNext();
};

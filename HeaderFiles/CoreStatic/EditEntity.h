#pragma once

#include "GlobalStatusVariables.h"
#include "MaterialDlg.h"

// CEditEntity dialog
class CEditEntity : public CDialog
{
	DECLARE_DYNAMIC(CEditEntity)

public:
	CEditEntity(CWnd* pParent = NULL);   // standard constructor
	
	CMenuEntity				*m_pEntity;
	GlobalStatusVariables	*m_gStatus;
	int						 m_freeVisualID;
	bool					 m_initialized;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditEntity };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	//Generate Button
	afx_msg void OnButtonGetFreeID();

	//Settings
	afx_msg void OnChangeEntityID();
	afx_msg void OnChangeAnimationIndex();
	afx_msg void OnChangeVisualCoords();
	afx_msg void OnChangeScale();

	//Standard Buttons
	afx_msg void OnButtonNext();
};

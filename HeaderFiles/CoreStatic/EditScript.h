#pragma once
#include "CMenuScript.h"

// CEditScript dialog
class CEditScript : public CDialog
{
	DECLARE_DYNAMIC(CEditScript)

public:
	CEditScript(CWnd* pParent = NULL);   // standard constructor
	
	CMenuScript *m_script;

	// Dialog Data
	enum { IDD = IDD_DIALOG_MenuSysEditScript };

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void FillVariableList();
	void FillFunctionList();

	afx_msg void OnSelectFunction();
	afx_msg void OnDoubleClickVariable();
	afx_msg void OnButtonNext();
};

#if !defined(AFX_EDITHALODLG_H__E2088801_4EA4_11D4_A9C0_0000B4C06895__INCLUDED_)
#define AFX_EDITHALODLG_H__E2088801_4EA4_11D4_A9C0_0000B4C06895__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditHaloDLG.h : header file
//

#include "CHaloClass.h"

/////////////////////////////////////////////////////////////////////////////
// EditHaloDLG dialog

class AFX_EXT_CLASS   EditHaloDLG : public CDialog
{
// Construction
public:
	EditHaloDLG(CTextureDBObjList *texList, CWnd* pParent = NULL);
	CTextureDBObjList *m_textureList;
	CString m_textureName;
	int m_textureIndex;

// Dialog Data
	//{{AFX_DATA(EditHaloDLG)
	enum { IDD = IDD_DIALOG_EditHalo };
	CComboBox	m_textureSelect;
	float	m_growthRate;
	float	m_maxSize;
	CString m_meshFileName;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditHaloDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EditHaloDLG)
	afx_msg void OnBrowseFileName();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCOMBOEditHalotextureSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITHALODLG_H__E2088801_4EA4_11D4_A9C0_0000B4C06895__INCLUDED_)

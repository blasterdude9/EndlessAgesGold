#if !defined(AFX_ADDHALODLG_H__AE06C0C3_4BBC_11D4_B4E0_0000B4BF12A9__INCLUDED_)
#define AFX_ADDHALODLG_H__AE06C0C3_4BBC_11D4_B4E0_0000B4BF12A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddHaloDLG.h : header file
//

#include "CHaloClass.h"

/////////////////////////////////////////////////////////////////////////////
// AddHaloDLG dialog

class AFX_EXT_CLASS   AddHaloDLG : public CDialog
{
	
// Construction
public:
	AddHaloDLG(CTextureDBObjList *texList, CWnd* pParent = NULL);
	CTextureDBObjList *m_textureList;
	CString m_textureName;
	int m_textureIndex;

// Dialog Data
	//{{AFX_DATA(AddHaloDLG)
	enum { IDD = IDD_DIALOG_AddHalo };
	CComboBox	m_textureSelect;
	float	m_growthRate;
	CString m_meshFileName;
	float	m_maxSize;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AddHaloDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	
	void OnOK();
	// Generated message map functions
	//{{AFX_MSG(AddHaloDLG)
	afx_msg void OnBrowseFileName();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCOMBOAddHalotextureSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDHALODLG_H__AE06C0C3_4BBC_11D4_B4E0_0000B4BF12A9__INCLUDED_)

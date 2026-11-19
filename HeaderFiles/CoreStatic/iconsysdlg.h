#if !defined(AFX_ICONSYSDLG_H__C017D2E9_DBB4_4205_9FD6_3ADAFD747E77__INCLUDED_)
#define AFX_ICONSYSDLG_H__C017D2E9_DBB4_4205_9FD6_3ADAFD747E77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IconSysDlg.h : header file
//
#include "MaterialDlg.h"
#include "CIconSystemClass.h"
#include "CTextureDBClass.h"

/////////////////////////////////////////////////////////////////////////////
// CIconSysDlg dialog

class AFX_EXT_CLASS   CIconSysDlg : public CDialog
{
// Construction
public:
	CIconSysDlg(CWnd* pParent = NULL);   // standard constructor

	CIconSysList * m_iconDatabaseREF;
	CTextureDBObjList * m_texturedatabaseREF; 
// Dialog Data
	//{{AFX_DATA(CIconSysDlg)
	enum { IDD = IDD_DIALOG_IconSys };
	CComboBox	m_textureDatabaseCombo;
	CListBox	m_iconList;
	float	m_gridDensity;
	CString	m_iconName;
	float	m_iconSize;
	float	m_xIndex;
	float	m_yIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIconSysDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListIcons(int sel);
	void InListTexturedatabase(int sel);
	CString ConvToString(int number);
	// Generated message map functions
	//{{AFX_MSG(CIconSysDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAccessMaterial();
	afx_msg void OnBUTTONAddIcon();
	afx_msg void OnBUTTONDeleteIcon();
	afx_msg void OnBUTTONReplaceIcon();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	afx_msg void OnSelchangeLISTIconDatabase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICONSYSDLG_H__C017D2E9_DBB4_4205_9FD6_3ADAFD747E77__INCLUDED_)

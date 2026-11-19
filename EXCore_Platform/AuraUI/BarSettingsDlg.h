#if !defined(AFX_BARSETTINGSDLG_H__8BDD13FE_D87A_4426_8F82_A004DD4DB316__INCLUDED_)
#define AFX_BARSETTINGSDLG_H__8BDD13FE_D87A_4426_8F82_A004DD4DB316__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarSettingsDlg.h : header file
//
#include "MaterialDlg.h"
#include "CMenuBarClass.h"
#include "ctextureDBClass.h"
/////////////////////////////////////////////////////////////////////////////
// CBarSettingsDlg dialog

class CBarSettingsDlg : public CDialog
{
// Construction
public:
	CBarSettingsDlg(CWnd* pParent = NULL);   // standard constructor
    CMenuBarObj * menuBarRef;
	CTextureDBObjList * GL_textureDataBase;
// Dialog Data
	//{{AFX_DATA(CBarSettingsDlg)
	enum { IDD = IDD_DIALOG_BarSettings };
	CComboBox	m_cmboLink;
	float	m_barLength;
	float	m_barWidth;
	int		m_visualReference;
	float	m_rootX;
	float	m_rootY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBarSettingsDlg)
	afx_msg void OnBUTTONstaticAccessMaterial();
	afx_msg void OnBUTTONdynamicAccessMaterial();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARSETTINGSDLG_H__8BDD13FE_D87A_4426_8F82_A004DD4DB316__INCLUDED_)

#if !defined(AFX_CHARONADLG_H__8CF507E1_BAE6_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_CHARONADLG_H__8CF507E1_BAE6_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharonaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharonaDlg dialog
#include "ctextureDBClass.h"
#include "CCharonaClass.h"
#include "CABFunctionLib.h"

class AFX_EXT_CLASS   CCharonaDlg : public CDialog
{
// Construction
public:
	CCharonaDlg(CWnd* pParent = NULL);   // standard constructor

    CABFunctionLib      cablib;
	CTextureDBObjList * m_textureDataBase;
	CCharonaObj       * m_charonaRef;
// Dialog Data
	//{{AFX_DATA(CCharonaDlg)
	enum { IDD = IDD_DIALOG_CharonaDlg };
	CButton	m_enable;
	CComboBox	m_textureList;
	float	m_endDistance;
	long	m_fadeTime;
	float	m_size;
	float	m_startDistance;
	float	m_visibleDistance;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharonaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCharonaDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCHECKEnable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARONADLG_H__8CF507E1_BAE6_11D4_B1EE_0000B4BD56DD__INCLUDED_)
